import os
import time
import numpy as np
from threading import Event, Thread
import signal
import sys
TARGET_INTERVAL = float(os.getenv('TARGET_INTERVAL', '0.01'))
REPORT_INTERVAL = float(os.getenv('REPORT_INTERVAL', '5')) 
SCHED_POLICY = os.getenv('SCHED_POLICY', 'SCHED_FIFO')
PRIORITY = int(os.getenv('PRIORITY', '80'))

SCHED_POLICIES = {
    'SCHED_FIFO': 1,  
    'SCHED_RR': 2,    
    'SCHED_OTHER': 0  
}

class RealtimeMonitor:
    def __init__(self):
        self.stop_event = Event()
        self.delays = []
        self.jitters = []
        self.start_time = time.time()
        self.last_time = self.start_time
        signal.signal(signal.SIGINT, self.signal_handler)
        signal.signal(signal.SIGTERM, self.signal_handler)

    def signal_handler(self, signum, frame):
        print("\nПолучен сигнал остановки...")
        self.stop_event.set()

    def set_realtime_scheduling(self):
        try:
            policy = SCHED_POLICIES.get(SCHED_POLICY.upper(), 1)
            param = os.sched_param(PRIORITY)
            os.sched_setscheduler(0, policy, param)
            print(f"Установлена политика планирования: {SCHED_POLICY}, приоритет: {PRIORITY}")
        except Exception as e:
            print(f"Ошибка настройки реального времени: {e} (требуются права CAP_SYS_NICE)")

    def monitor_loop(self):
        self.set_realtime_scheduling()
        
        print(f"Начало теста реального времени, целевой интервал: {TARGET_INTERVAL*1000:.1f}мс")
        print("Нажмите Ctrl+C для остановки")
        
        next_report = time.time() + REPORT_INTERVAL
        
        while not self.stop_event.is_set():
            ideal_time = self.last_time + TARGET_INTERVAL
            actual_time = time.time()
            
            delay = actual_time - ideal_time
            jitter = abs(delay)
            
            self.delays.append(delay)
            self.jitters.append(jitter)
            
            if actual_time >= next_report:
                self.report_stats()
                next_report = actual_time + REPORT_INTERVAL
            
            self.last_time += TARGET_INTERVAL
            sleep_time = max(0, self.last_time - time.time())
            time.sleep(sleep_time)
        
        self.report_stats(final=True)

    def report_stats(self, final=False):
        if not self.delays:
            return
            
        delays = np.array(self.delays)
        jitters = np.array(self.jitters)
        
        avg_delay = np.mean(delays) * 1000  
        max_delay = np.max(delays) * 1000
        min_delay = np.min(delays) * 1000
        std_delay = np.std(delays) * 1000
        
        avg_jitter = np.mean(jitters) * 1000
        max_jitter = np.max(jitters) * 1000
        
        num_samples = len(self.delays)
        actual_interval = (time.time() - self.start_time) / num_samples * 1000
        
        report_type = "ФИНАЛЬНЫЙ ОТЧЕТ" if final else "ПЕРИОДИЧЕСКИЙ ОТЧЕТ"
        
        print(f"\n===== {report_type} =====")
        print(f"Количество образцов: {num_samples}")
        print(f"Фактический средний интервал: {actual_interval:.3f}мс")
        print(f"Средняя задержка: {avg_delay:.3f}мс")
        print(f"Максимальная задержка: {max_delay:.3f}мс")
        print(f"Минимальная задержка: {min_delay:.3f}мс")
        print(f"Стандартное отклонение: {std_delay:.3f}мс")
        print(f"Средний джиттер: {avg_jitter:.3f}мс")
        print(f"Максимальный джиттер: {max_jitter:.3f}мс")
        
        if not final:
            self.delays = []
            self.jitters = []
            self.start_time = time.time()

if __name__ == "__main__":
    monitor = RealtimeMonitor()
    try:
        monitor.monitor_loop()
    except KeyboardInterrupt:
        monitor.stop_event.set()
        monitor.report_stats(final=True)
