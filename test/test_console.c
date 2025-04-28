#include <console.h>

int main(void) {
    rcc_clock_setup_pll(
        &rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // set system clock to 72MHz using 8MHz HSE
    console_init();
    console_register_command("ping", "Responds with pong", console_ping);
    console_register_command("led", "Toggles LED", console_led);

    for (;;) {
        delay(25);
    }
    return 0;
}

void console_ping(void) {
    char* ping_msg = "pong\n";
    console_usart_write((uint8_t*) ping_msg, strlen(ping_msg));
}

void console_led(void) {
    static uint8_t led_state = 0;
    led_state                = !led_state;
    if (led_state) {
        // YEYEY LED_ON
    } else {
        // LED_OFF
    }
}