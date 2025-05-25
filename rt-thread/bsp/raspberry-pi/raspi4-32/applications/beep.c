#include <rtdevice.h>
#include <beep.h>
struct rt_device_pwm *pwm_device = RT_NULL;
char name[] = "beep"; 
int beep_init(void)
{
    if(pwm_device == RT_NULL)
    {
        rt_kprintf("pwm_device '%s' not found",name);
        return -RT_ERROR;
    }
    return 0;
}
void beep_on()
{
    rt_pwm_enable();
}

void beep_off()
{
    rt_pwm_disable();
}
void rt_pwm_set(uint16_t period, uint16_t pulse)
{
    PWM_RANGE_0 = period;
    PWM_DAT_0 = pulse;
}
int beep_set(uint16_t freq, uint8_t volume)
{
    rt_uint32_t period, pulse;

    period = 100000000 / freq;  
    pulse = period - period / 100 * volume;

    rt_pwm_set(period, pulse);

    return 0;
}
