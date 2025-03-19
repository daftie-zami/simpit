#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define LED_SET() gpio_set(LED_PORT,LED_PIN)
#define LED_RESET() gpio_reset(LED_PORT,LED_PIN)

void led_init(void);