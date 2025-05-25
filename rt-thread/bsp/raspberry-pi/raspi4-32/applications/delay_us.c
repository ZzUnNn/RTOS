#include <rtdevice.h>
#include <delay_us.h>

void rt_hw_us_delay(rt_uint32_t us) 
{
  
  uint32_t start = STIMER_CLO;  
  while ((STIMER_CLO - start) < us) 
  {
  }
  
}

/*void rt_hw_us_delay(rt_uint32_t us) {
  rt_hw_interrupt_umask(SYSTEM_TIMER1_IRQ);
  uint32_t start = get_i_number();
  while((get_i_number() - start) < us)
  {
    rt_kprintf("udelay_i_number:%d   \n\r",get_i_number());
    rt_kprintf("STIMER_C1_number:%d   \n\r",STIMER_C1);
    rt_kprintf("STIMER_CLO_number:%d   \n\r",STIMER_CLO);
    rt_thread_delay(1000);
  }
  rt_hw_interrupt_mask(SYSTEM_TIMER1_IRQ);
}*/

