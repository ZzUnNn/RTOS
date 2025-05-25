#ifndef BEEP_H
#define BEEP_H

#include <rtthread.h>
#include <raspi4.h>
#include <board.h>
#define BEEP_PWM_DEVICE  "pwm1"
#define BEEP_PWM_CH      0

int beep_init(void);                         
void beep_on(void);                           
void beep_off(void);                          
int beep_set(uint16_t freq, uint8_t volume); 
void rt_pwm_set(uint16_t period, uint16_t pulse);
void beep_on(void);
void beep_off(void);

#endif
