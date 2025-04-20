#include <console.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <board.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

static void console_usart_init(void);
static void console_usart_write(uint8_t *buf, uint16_t len);

static void dma_write(uint8_t *data, uint16_t size);
static void dma_read(uint8_t *data, uint16_t size);

// volatile uint8_t transfered = 0;
// volatile uint8_t received = 0;

void console_init(void){
    console_usart_init();
	int *p = (void *)0x08000000;
	console_usart_write((void *)p,1000);
}

void console_run(void){
    
}


/**
 * Print a text on the console
 */
void console_print(const char *format, ...) {
    char buf[128];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(buf, format, argptr);
    va_end(argptr);
  
    console_usart_write((uint8_t*)buf, strlen(buf));
}



static void console_usart_init(void){

	/* Setup GPIO pin GPIO_USART1_TX/GPIO9 and GPIO_USART1_RX/GPIO10 on GPIO port A. */
	gpio_set_mode(CONSOLE_USART_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, CONSOLE_USART_PIN_TX);
    gpio_set_mode(CONSOLE_USART_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, CONSOLE_USART_PIN_RX);

    // /* Enable clocks for USART port */
    rcc_periph_clock_enable(CONSOLE_USART_RCC_CLK);

	/* Setup USART parameters. */
	usart_set_baudrate(CONSOLE_USART_PORT, CONSOLE_USART_BAUDRATE);
	usart_set_databits(CONSOLE_USART_PORT, 8);
	usart_set_stopbits(CONSOLE_USART_PORT, USART_STOPBITS_1);
	usart_set_mode(CONSOLE_USART_PORT, USART_MODE_TX_RX);
	usart_set_parity(CONSOLE_USART_PORT, USART_PARITY_NONE);
	usart_set_flow_control(CONSOLE_USART_PORT, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(CONSOLE_USART_PORT);

#ifdef CONSOLE_USART_USE_DMA
    nvic_set_priority(CONSOLE_USART_TX_DMA_IRQ, 0);
	nvic_enable_irq(CONSOLE_USART_TX_DMA_IRQ);

	nvic_set_priority(CONSOLE_USART_RX_DMA_IRQ, 0);
	nvic_enable_irq(CONSOLE_USART_RX_DMA_IRQ);
#endif
}

static void console_usart_write(uint8_t *buf, uint16_t len){
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

static void dma_write(uint8_t *data, uint16_t size){

	/* Reset DMA channel*/
	dma_channel_reset(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL);

	dma_set_peripheral_address(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL, (uint32_t)&USART_DR(CONSOLE_USART_PORT));
	dma_set_memory_address(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL, (uint32_t)data);
	dma_set_number_of_data(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL, size);
	dma_set_read_from_memory(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL);
	dma_enable_memory_increment_mode(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL);
	dma_set_peripheral_size(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL, DMA_CCR_PSIZE_8BIT);
	dma_set_memory_size(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL, DMA_CCR_MSIZE_8BIT);
	dma_set_priority(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL, DMA_CCR_PL_VERY_HIGH);

	dma_enable_transfer_complete_interrupt(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL);

	dma_enable_channel(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL);

    usart_enable_tx_dma(CONSOLE_USART_PORT);
}

void dma1_channel4_isr(void)
{
	// if((dma_get_interrupt_flag(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL, DMA_ISR_TCIF(CONSOLE_USART_TX_DMA_CHANNEL)) != 0)){
	// 	dma_clear_interrupt_flags(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL, DMA_IFCR_CTCIF(CONSOLE_USART_TX_DMA_CHANNEL));
	// 	transfered = 1;
	// }

	dma_disable_transfer_complete_interrupt(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL);

	usart_disable_tx_dma(CONSOLE_USART_PORT);

	dma_disable_channel(CONSOLE_USART_TX_DMA_STREAM, CONSOLE_USART_TX_DMA_CHANNEL);
}

static void dma_read(uint8_t *data, uint16_t size){
	/* Reset DMA channel*/
	dma_channel_reset(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL);

	dma_set_peripheral_address(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL, (uint32_t)&USART_DR(CONSOLE_USART_PORT));
	dma_set_memory_address(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL, (uint32_t)data);
	dma_set_number_of_data(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL, size);
	dma_set_read_from_peripheral(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL);
	dma_enable_memory_increment_mode(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL);
	dma_set_peripheral_size(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL, DMA_CCR_PSIZE_8BIT);
	dma_set_memory_size(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL, DMA_CCR_MSIZE_8BIT);
	dma_set_priority(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL, DMA_CCR_PL_HIGH);

	dma_enable_transfer_complete_interrupt(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL);

	dma_enable_channel(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL);

    usart_enable_rx_dma(CONSOLE_USART_PORT);
}

void dma1_channel5_isr(void)
{
	// if(dma_get_interrupt_flag(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL, DMA_ISR_TCIF(CONSOLE_USART_RX_DMA_CHANNEL)) != 0){
	// 	dma_clear_interrupt_flags(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL, DMA_IFCR_CTCIF(CONSOLE_USART_RX_DMA_CHANNEL));
	// 	received = 1;
	// }

	dma_disable_transfer_complete_interrupt(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL);

	usart_disable_rx_dma(CONSOLE_USART_PORT);

	dma_disable_channel(CONSOLE_USART_RX_DMA_STREAM, CONSOLE_USART_RX_DMA_CHANNEL);
}