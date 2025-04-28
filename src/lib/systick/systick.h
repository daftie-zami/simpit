#pragma once

#include "board.h"
#include <stdint.h>

void     systick_init(void);
void     delay(uint32_t ms);
uint32_t systick_get_counter(void);