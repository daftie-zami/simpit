#pragma once

#include <board.h>
#include <libopencm3/stm32/gpio.h>

#define LED_ON() gpio_clear(LED_PORT, LED_PIN)
#define LED_OFF() gpio_set(LED_PORT, LED_PIN)
#define LED_TOGGLE() gpio_toggle(LED_PORT, LED_PIN)

void led_init(void);