#include "led.h"

#include "board.h"

void led_init(void) {
    rcc_periph_clock_enable(LED_RCC_CLK);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
}