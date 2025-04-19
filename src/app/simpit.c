#include "my_assert.h"
#include <lib/led/led.h>
#include <lib/systick/systick.h>
#include <lib/hid/hid.h>
#include <lib/mpu6050/mpu6050.h>

static void clock_setup(void);

int main(void) {
    clock_setup();

    //Modules
    led_init(); //First LED
    systick_init();
    hid_init();
    mpu_init();

	for (;;) {
        // hid_run();
        LED_TOGGLE();
        delay(50);
        mpu_read();
	}
    return 0;
}


static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // set system clock to 72MHz using 8MHz HSE
    rcc_periph_clock_enable(RCC_GPIOB);

    rcc_periph_clock_enable(RCC_GPIOA);

    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);
}
