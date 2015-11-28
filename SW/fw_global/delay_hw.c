/**
 * @file delay_hw.c
 * @brief Delay function based on hardware timer
 *
 * @author Aleksey Slabchenko, alexslabchenko@gmail.com
*/

#include "delay_hw.h"
#include "stm32f10x_tim.h"

/// @brief Timer initializion
void delay_hw_init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    DELAY_HW_TIMER_RCC_INIT(DELAY_HW_TIMER_RCC, ENABLE);
    TIM_DeInit(DELAY_HW_TIMER);

    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = DELAY_HW_TIMER_US_VALUE;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(DELAY_HW_TIMER, &TIM_TimeBaseStructure);
    TIM_SetCounter(DELAY_HW_TIMER, 0);
    TIM_Cmd(DELAY_HW_TIMER, ENABLE);
}

/// @brief Delay for specified amount of microsecond.
void delay_hw_us(uint16_t us)
{
// Just a bit of magic :)
uint16_t old_count;
uint16_t count;
uint8_t wait_for_overflow = 0;

	old_count = DELAY_HW_TIMER->CNT;
    us += old_count;
    if (us < old_count) wait_for_overflow = 1;
    while (1) // <= modern implementation of eternity :)
    {
        count = DELAY_HW_TIMER->CNT;
        if (count < old_count)
        {
            if (wait_for_overflow)
                wait_for_overflow = 0;
            else
                break;
        }
        if ((wait_for_overflow == 0) && (count >= us))
            break;
        old_count = count;
    }
}

void delay_hw_us_clear(void)
{
    DELAY_HW_TIMER->CNT = 0;
}

uint8_t delay_hw_us_is_elapsed(uint16_t delay)
{
    if (DELAY_HW_TIMER->CNT > delay)
        return 1;
    else
        return 0;
}

/// @brief Delay for specified amount of millisecond
void delay_hw_ms(uint32_t ms)
{
    while (ms >= 65)
    {
        delay_hw_us(65000); // Delay for 65ms (HW timer is 16-bit)
        ms -= 65;
    }
    while (ms >= 10)
    {
        delay_hw_us(10000); // Delay for 10ms
        ms -= 10;
    }
    while (ms >= 1)
    {
        delay_hw_us(1000); // Delay for 1ms
        ms -= 1;
    }
}
