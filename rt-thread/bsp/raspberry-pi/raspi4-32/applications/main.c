#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <dht11.h>
#include <beep.h>
#include <delay_us.h>

struct data_struct data;
static rt_timer_t timer1;

static rt_sem_t dht11_sem; 
static rt_mailbox_t data_mail;  
static rt_mutex_t data_mutex= RT_NULL;   
#define TEMP_ALARM_THRESHOLD 10

struct sensor_data {
    int temperature;
    int temperature_decimal;
    int humidity;
    int humidity_decimal;
};

static void dht11_thread_entry(void *parameter)
{
    struct sensor_data data;
    while (1) {
        /* 获取传感器数据 */
        struct data_struct raw_data = read_data(&data.temperature, 
                                              &data.temperature_decimal,
                                              &data.humidity,
                                              &data.humidity_decimal);
        
        rt_mutex_take(data_mutex, RT_WAITING_FOREVER);
        rt_mb_send(data_mail, (rt_uint32_t)&data);
        rt_sem_release(dht11_sem);
        rt_mutex_release(data_mutex);
        rt_thread_delay(rt_tick_from_millisecond(500));
    }
}

static void beep_alarm_thread_entry(void *parameter)
{
    struct sensor_data *data;
    
    while (1) {
        if (rt_sem_take(dht11_sem, RT_WAITING_FOREVER) == RT_EOK) {
            if (rt_mb_recv(data_mail, (rt_ubase_t*)&data, RT_WAITING_FOREVER) == RT_EOK) {
                if (data->temperature > TEMP_ALARM_THRESHOLD) {
                    beep_on();
                    rt_kprintf("[ALARM] Temperature too high: %d.%d\n", 
                              data->temperature, data->temperature_decimal);
                    rt_thread_delay(rt_tick_from_millisecond(500));
                    beep_off();
                }
            }
        }
    }
}

static void data_processing_thread_entry(void *parameter)
{
    struct sensor_data *data;
    float temp_avg = 0;
    float humi_avg = 0;
    int sample_count = 0;
    
    while (1) {
        if (rt_sem_take(dht11_sem, RT_WAITING_FOREVER) == RT_EOK) {
            if (rt_mb_recv(data_mail, (rt_ubase_t*)&data, RT_WAITING_FOREVER) == RT_EOK) {
                // 计算温度湿度平均值
                float current_temp = data->temperature + data->temperature_decimal / 10.0;
                float current_humi = data->humidity + data->humidity_decimal / 10.0;
                
                temp_avg = (temp_avg * sample_count + current_temp) / (sample_count + 1);
                humi_avg = (humi_avg * sample_count + current_humi) / (sample_count + 1);
                sample_count++;
                
                // 每5个样本输出一次统计信息
                if (sample_count % 5 == 0) {
                    rt_kprintf("[DATA PROCESSING] Temperature Avg: %.1fC, Humidity Avg: %.1f%%\n",
                              temp_avg, humi_avg);
                    
                    // 检测温湿度变化趋势
                    static float last_temp = 0;
                    if (sample_count > 1) {
                        float temp_change = current_temp - last_temp;
                        if (fabs(temp_change) > 0.5) {
                            rt_kprintf("[TREND] Temperature %s by %.1fC\n",
                                      temp_change > 0 ? "rising" : "falling",
                                      fabs(temp_change));
                        }
                    }
                    last_temp = current_temp;
                }
            }
        }
    }
}

static void timer1_callback(void *parameter)
{
    // 定时器回调函数
}

int main(int argc, char** argv)
{
    rt_hw_us_delay_init();
    beep_init();
    pwm_freq_init();
    
    dht11_sem = rt_sem_create("dht11_sem", 0, RT_IPC_FLAG_FIFO);
    data_mail = rt_mb_create("data_mail", 4, RT_IPC_FLAG_FIFO);
    data_mutex = rt_mutex_create("data_mutex", RT_IPC_FLAG_FIFO);
    if (!data_mutex) {
        rt_kprintf("Mutex creation failed!\n");
        return -1;
    }
    
    rt_thread_t dht11_thread = rt_thread_create("dht11", 
                                              dht11_thread_entry, 
                                              RT_NULL,
                                              1024, 
                                              10, 
                                              10);
    
    rt_thread_t alarm_thread = rt_thread_create("alarm", 
                                             beep_alarm_thread_entry, 
                                             RT_NULL,
                                             1024, 
                                             12, 
                                             12);
    
    rt_thread_t data_process_thread = rt_thread_create("data_proc", 
                                                    data_processing_thread_entry, 
                                                    RT_NULL,
                                                    2048, 
                                                    14, 
                                                    14);
    
    timer1 = rt_timer_create("timer1", 
                            timer1_callback, 
                            RT_NULL,
                            rt_tick_from_millisecond(500),
                            RT_TIMER_FLAG_PERIODIC);
    
    if (timer1 != RT_NULL) {
        rt_timer_start(timer1);
    }
    
    rt_thread_startup(dht11_thread);
    rt_thread_startup(alarm_thread);
    rt_thread_startup(data_process_thread);
    
    return 0;
}