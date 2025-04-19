#include "systick.h"

#include <lib/led/led.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>

static volatile uint32_t systick_delay = 0;

extern uint32_t rcc_ahb_frequency;

void systick_init(void)
{
  /*
    * Set SysTick to count at 1ms intervals.
    * SysTick clock = AHB_clock / 8
    * Reload value = (AHB_clock / 8) / 1000 - 1
    */
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
  systick_set_reload(rcc_ahb_frequency / 8 / SYSTICK_FREQUENCY - 1);
  nvic_set_priority(NVIC_SYSTICK_IRQ, 0xFF);
  systick_interrupt_enable();
  systick_counter_enable();
}

uint32_t systick_get_ms(void){
  return systick_delay;
}

void delay(uint32_t ms){
  uint32_t tickstart = systick_delay;
  while ((systick_delay - tickstart) < ms);
}

void sys_tick_handler(void){
    systick_delay++;
    static volatile uint32_t led_tick = 50;
    if((--led_tick) == 0){
      // LED_TOGGLE();
      led_tick = 50;
    }
}
