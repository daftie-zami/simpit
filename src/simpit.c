#include "my_assert.h"
#include "led.h"
#include "systick.h"
#include "hid.h"

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // set system clock to 72MHz using 8MHz HSE

    //Modules
    systick_init();
    led_init();
    hid_init();

	for (;;) {
        LED_TOGGLE();
        hid_run();
        // delay(50);
	}
    return 0;
}



