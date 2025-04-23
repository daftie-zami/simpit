#include <console/console_usart.h>
#include <board.h>
#include <utils.h>
#include <led/led.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

#define USART_RX_BUFFER_SIZE 128

static char usart_rx_buffer[USART_RX_BUFFER_SIZE];
static volatile uint16_t usart_rx_head = 0;
static volatile uint16_t usart_rx_tail = 0;

void console_usart_init(void){
	/* Setup GPIO pin GPIO_USART1_TX/GPIO9 and GPIO_USART1_RX/GPIO10 on GPIO port A. */
	gpio_set_mode(CONSOLE_USART_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, CONSOLE_USART_PIN_TX);
    gpio_set_mode(CONSOLE_USART_GPIO_PORT, GPIO_MODE_INPUT,
			GPIO_CNF_INPUT_FLOAT, CONSOLE_USART_PIN_RX);

    // /* Enable clocks for USART1 port */
    rcc_periph_clock_enable(RCC_USART1);

	/* Setup USART parameters. */
	usart_set_baudrate(CONSOLE_USART_PORT, CONSOLE_USART_BAUDRATE);
	usart_set_databits(CONSOLE_USART_PORT, 8);
	usart_set_stopbits(CONSOLE_USART_PORT, USART_STOPBITS_1);
	usart_set_mode(CONSOLE_USART_PORT, USART_MODE_TX_RX);
	usart_set_parity(CONSOLE_USART_PORT, USART_PARITY_NONE);
	usart_set_flow_control(CONSOLE_USART_PORT, USART_FLOWCONTROL_NONE);

	nvic_enable_irq(NVIC_USART1_IRQ);
    usart_enable_rx_interrupt(CONSOLE_USART_PORT);

	/* Finally enable the USART. */
	usart_enable(CONSOLE_USART_PORT);

}

void usart1_isr(void) {
	if (((USART_CR1(CONSOLE_USART_PORT) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(CONSOLE_USART_PORT) & USART_SR_RXNE) != 0)) {

		char c = usart_recv(CONSOLE_USART_PORT);
		
		uint16_t next_head = (usart_rx_head + 1) % USART_RX_BUFFER_SIZE;
		if (next_head != usart_rx_tail) { // Buffer dolu değilse
			usart_rx_buffer[usart_rx_head] = c;
			usart_rx_head = next_head;
		}
		LED_TOGGLE();
	}
}


void console_usart_write(uint8_t *buf, uint16_t len){
	for(uint16_t i = 0; i < len; i++){
		usart_send_blocking(CONSOLE_USART_PORT, buf[i]);
	}
}

uint8_t console_usart_read_line(char *buffer, size_t max_len){
	uint16_t i = 0;
	
	while (usart_rx_tail != usart_rx_head && i < max_len - 1) {
		char c = usart_rx_buffer[usart_rx_tail];
		usart_rx_tail = (usart_rx_tail + 1) % USART_RX_BUFFER_SIZE;

		buffer[i++] = c;

		if (c == '\n') {
			buffer[i] = '\0';
			return 1; // satır tamamlandı
		}
	}

	return 0; // henüz satır tamamlanmadı
}
