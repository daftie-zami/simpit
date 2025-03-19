#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "my_assert.h"

// cihibos kullanmalıyım :/

void system_init(void);
static void led_init(void);

int main(void) {
    system_init();
    led_init();
	for (;;) {
        gpio_toggle(GPIOB, GPIO2);
	}
    return 0;
}

void system_init(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // set system clock to 72MHz using 8MHz HSE
}

static void led_init(void) {
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
}

