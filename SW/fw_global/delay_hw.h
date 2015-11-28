/**
 * @file delay_hw.h
 * @brief Header file for delay_hw.c
 *
 * @author Aleksey Slabchenko, alexslabchenko@gmail.com
 */

#ifndef DELAY_HW_H // Include only if necessary
#define DELAY_HW_H

// Include standard headers
#include <stdint.h>
// Include necessary SPL headers
#include <stm32f10x.h>
#include <misc.h>
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#define DELAY_HW_TIMER_RCC	RCC_APB1Periph_TIM3
#define DELAY_HW_TIMER_APB	1						// APB number (1 or 2, can be taken from value above)
#define DELAY_HW_TIMER		TIM3					// HW Timer number

#define DELAY_HW_TIMER_US_VALUE 70 // Value which match to 1us

#if	(DELAY_HW_TIMER_APB == 1)
	#define DELAY_HW_TIMER_RCC_INIT	RCC_APB1PeriphClockCmd
#elif (DELAY_HW_TIMER_APB == 2)
	#define DELAY_HW_TIMER_RCC_INIT	RCC_APB2PeriphClockCmd
#endif

void delay_hw_init(void);
void delay_hw_us(uint16_t us);
void delay_hw_us_clear(void);
uint8_t delay_hw_us_is_elapsed(uint16_t delay);
void delay_hw_ms(uint32_t ms);

#endif // DELAY_HW_H
