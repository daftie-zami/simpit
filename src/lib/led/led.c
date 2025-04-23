#include <led.h>

void led_init(void) {
    LED_RESET();
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
}