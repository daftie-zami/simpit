#include <console/console_usart.h>
#include <board.h>
#include <utils.h>
#include <led/led.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

#if defined(CONSOLE_USART_USE_DMA)
static void dma_write(uint8_t *data, uint16_t size);
static void dma_read(uint8_t *data, uint16_t size);
#endif

// volatile uint8_t transfered = 0;
// volatile uint8_t received = 0;

void console_usart_init(void){
	/* Setup GPIO pin GPIO_USART1_TX/GPIO9 and GPIO_USART1_RX/GPIO10 on GPIO port A. */
	gpio_set_mode(CONSOLE_USART_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, CONSOLE_USART_PIN_TX);
    gpio_set_mode(CONSOLE_USART_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, CONSOLE_USART_PIN_RX);



    // /* Enable clocks for USART1 port */
    rcc_periph_clock_enable(RCC_USART1);

	/* Setup USART parameters. */
	usart_set_baudrate(CONSOLE_USART_PORT, CONSOLE_USART_BAUDRATE);
	usart_set_databits(CONSOLE_USART_PORT, 8);
	usart_set_stopbits(CONSOLE_USART_PORT, USART_STOPBITS_1);
	usart_set_mode(CONSOLE_USART_PORT, USART_MODE_TX_RX);
	usart_set_parity(CONSOLE_USART_PORT, USART_PARITY_NONE);
	usart_set_flow_control(CONSOLE_USART_PORT, USART_FLOWCONTROL_NONE);

    usart_enable_rx_interrupt(CONSOLE_USART_PORT);

	nvic_set_priority(NVIC_USART1_IRQ, 0);
	nvic_enable_irq(NVIC_USART1_IRQ);
	/* Finally enable the USART. */
	usart_enable(CONSOLE_USART_PORT);

#ifdef CONSOLE_USART_USE_DMA
	/* Enable clocks for DMA1 */
	rcc_periph_clock_enable(RCC_DMA1);

    nvic_set_priority(NVIC_DMA1_CHANNEL4_IRQ, 2);
	nvic_enable_irq(NVIC_DMA1_CHANNEL4_IRQ);

	nvic_set_priority(NVIC_DMA1_CHANNEL5_IRQ, 2);
	nvic_enable_irq(NVIC_DMA1_CHANNEL5_IRQ);
#endif
}

void usart1_isr(void) {
	if (((USART_CR1(CONSOLE_USART_PORT) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(CONSOLE_USART_PORT) & USART_SR_RXNE) != 0)) {

		/* Indicate that we got data. */
		LED_TOGGLE();

		/* Retrieve the data from the peripheral. */
		// data = usart_recv(CONSOLE_USART_PORT);

		// /* Enable transmit interrupt so it sends back the data. */
		// usart_enable_tx_interrupt(CONSOLE_USART_PORT);
	}
}

void console_usart_write(uint8_t *buf, uint16_t len){
#ifdef CONSOLE_USART_USE_DMA
    dma_write(buf, len);
#else
    for (uint16_t i = 0; i < len; i++) {
        usart_send_blocking(CONSOLE_USART_PORT, buf[i]);
    }
#endif
}

void console_usart_read(uint8_t *buf, uint16_t len){
#ifdef CONSOLE_USART_USE_DMA
    dma_read(buf, len);
#else
    for (uint16_t i = 0; i < len; i++) {
        buf[i] = usart_recv(CONSOLE_USART_PORT);
    }
#endif
}

//TODO: implement this function
void console_usart_read_line(uint8_t *buf, uint16_t len){
	UNUSED(len);
	UNUSED(buf);
}

#if defined(CONSOLE_USART_USE_DMA)
static void dma_write(uint8_t *data, uint16_t size){
	/* Reset DMA channel*/
	dma_channel_reset(DMA1, DMA_CHANNEL4);

	dma_set_peripheral_address(DMA1, DMA_CHANNEL4, (uint32_t)&USART_DR(DMA_CHANNEL4));
	dma_set_memory_address(DMA1, DMA_CHANNEL4, (uint32_t)data);
	dma_set_number_of_data(DMA1, DMA_CHANNEL4, size);
	dma_set_read_from_memory(DMA1, DMA_CHANNEL4);
	dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL4);
	dma_set_peripheral_size(DMA1, DMA_CHANNEL4, DMA_CCR_PSIZE_8BIT);
	dma_set_memory_size(DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT);
	dma_set_priority(DMA1, DMA_CHANNEL4, DMA_CCR_PL_VERY_HIGH);

	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL4);

	dma_enable_channel(DMA1, DMA_CHANNEL4);

    usart_enable_tx_dma(CONSOLE_USART_PORT);
}

void dma1_channel4_isr(void)
{
	// if((dma_get_interrupt_flag(DMA1, DMA_CHANNEL4, DMA_ISR_TCIF(DMA_CHANNEL4)) != 0)){
	// 	dma_clear_interrupt_flags(DMA1, DMA_CHANNEL4, DMA_IFCR_CTCIF(DMA_CHANNEL4));
	// 	transfered = 1;
	// }

	dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL4);

	usart_disable_tx_dma(CONSOLE_USART_PORT);

	dma_disable_channel(DMA1, DMA_CHANNEL4);
}

static void dma_read(uint8_t *data, uint16_t size){
	/* Reset DMA channel*/
	dma_channel_reset(DMA1, DMA_CHANNEL5);

	dma_set_peripheral_address(DMA1, DMA_CHANNEL5, (uint32_t)&USART_DR(DMA_CHANNEL5));
	dma_set_memory_address(DMA1, DMA_CHANNEL5, (uint32_t)data);
	dma_set_number_of_data(DMA1, DMA_CHANNEL5, size);
	dma_set_read_from_peripheral(DMA1, DMA_CHANNEL5);
	dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL5);
	dma_set_peripheral_size(DMA1, DMA_CHANNEL5, DMA_CCR_PSIZE_8BIT);
	dma_set_memory_size(DMA1, DMA_CHANNEL5, DMA_CCR_MSIZE_8BIT);
	dma_set_priority(DMA1, DMA_CHANNEL5, DMA_CCR_PL_HIGH);

	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL5);

	dma_enable_channel(DMA1, DMA_CHANNEL5);

    usart_enable_rx_dma(CONSOLE_USART_PORT);
	usart_enable_idle_interrupt(CONSOLE_USART_PORT);
}

void dma1_channel5_isr(void)
{
	// if(dma_get_interrupt_flag(DMA1, DMA_CHANNEL5, DMA_ISR_TCIF(DMA_CHANNEL5)) != 0){
	// 	dma_clear_interrupt_flags(DMA1, DMA_CHANNEL5, DMA_IFCR_CTCIF(DMA_CHANNEL5));
	// 	received = 1;
	// }

	dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL5);

	usart_disable_rx_dma(CONSOLE_USART_PORT);

	dma_disable_channel(DMA1, DMA_CHANNEL5);
}
#endif