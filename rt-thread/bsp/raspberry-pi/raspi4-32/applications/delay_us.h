#ifndef __DELAY_US_H
#define __DELAY_US_H
#include <rtdevice.h>
#include <board.h>
#define DELAY_TEST_PIN GPIO_PIN_40

void rt_hw_us_delay(rt_uint32_t us);

#endif