#include <led.h>

void led_init(void) {
    LED_OFF();
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
}