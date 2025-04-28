#include "motor_controller.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

static void timer_init(void);

void motors_init(void) { timer_init(); }

static void timer_init(void) {
    rcc_periph_clock_enable(RCC_TIM2);

    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
}