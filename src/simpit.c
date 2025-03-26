#include "my_assert.h"
#include "led.h"
#include "systick.h"
#include "hid.h"
#include "mpu6050.h"

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // set system clock to 72MHz using 8MHz HSE

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



