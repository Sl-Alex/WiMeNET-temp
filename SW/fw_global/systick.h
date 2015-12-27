#ifndef SYSTICK_H
#define SYSTICK_H

#include "stdint.h"

extern uint32_t sys_time;

void systick_init(void);
uint8_t systick_is_elapsed (uint32_t start_time, uint32_t timeout);

#endif /* SYSTICK_H_ */
