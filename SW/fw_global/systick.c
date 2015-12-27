#include "systick.h"

#include "stm32f10x.h"
#include "misc.h"

uint32_t sys_time;

void systick_init(void)
{
  SysTick_Config(SystemCoreClock / 1000);
}

uint8_t systick_is_elapsed (uint32_t start_time, uint32_t timeout)
{
    uint32_t end_time = start_time + timeout;
    uint32_t cur_time = sys_time;
    uint32_t overflow = (end_time < start_time) ? 1 : 0;

    if (overflow)
    {
        if (cur_time < start_time)
        {
            if (cur_time > end_time)
                return 1;
        }
    }
    else
    {
        if (cur_time > end_time)
            return 1;
    }
    return 0;
}
