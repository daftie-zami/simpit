#pragma once

#include <stddef.h>
#include <stdint.h>

void    console_usart_init(void);
void    console_usart_write(uint8_t* buf, uint16_t len);
void    console_usart_read(uint8_t* buf, uint16_t len);
uint8_t console_usart_read_line(char* buffer, size_t max_len);