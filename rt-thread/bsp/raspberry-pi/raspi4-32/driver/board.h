/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-16     bigmagic       first version
 */

#ifndef BOARD_H__
#define BOARD_H__

#include <stdint.h>
#include "raspi4.h"
#define SYSTEM_TIMER1_IRQ  0x60
extern unsigned char __bss_start;
extern unsigned char __bss_end;

#define RT_HW_HEAP_BEGIN    (void*)&__bss_end
#define RT_HW_HEAP_END      (void*)(RT_HW_HEAP_BEGIN + 64 * 1024 * 1024)
void rt_hw_board_init(void);
void rt_hw_us_delay_init(void);
int get_i_number(void);
void pwm_freq_init(void);
void rt_pwm_disable(void);
void rt_pwm_enable(void);

#endif
