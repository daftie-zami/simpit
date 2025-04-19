#pragma once

#include <stdint.h>
#include "board.h"

void systick_init(void);
void delay(uint32_t ms);
uint32_t systick_get_ms(void);