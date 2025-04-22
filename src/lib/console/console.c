#include <console.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <console_usart.h>

/**
 * @brief Initialize the console and USART low level driver
 * @param None
 * @return None
 */
void console_init(void){
    console_usart_init();
	uint8_t *p = (void*)0x00000000;
	console_usart_write(p,1000);
}

/**
 * @brief Own implementation of printf to send data via console
 * @param format: format string
 * @param ...: variable arguments
 * @return None
 */
void console_print(const char *format, ...) {
    char buf[128];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(buf, format, argptr);
    va_end(argptr);
  
    console_usart_write((uint8_t*)buf, strlen(buf));
}