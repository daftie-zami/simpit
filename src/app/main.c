#include "my_assert.h"
#include <led/led.h>
#include <systick/systick.h>
#include <hid/hid.h>
#include <mpu6050/mpu6050.h>

static void clock_setup(void);

int main(void) {
    clock_setup();

    //Modules
    led_init();
    hid_init();
    systick_init();
    // mpu_init();

    delay(50);
	for (;;) {
        // hid_test();
        LED_TOGGLE();
        delay(50);
        // mpu_read();
	}
    return 0;
}


static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // set system clock to 72MHz using 8MHz HSE

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);
}
