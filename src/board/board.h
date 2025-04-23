#pragma once

#define SYSTICK_FREQUENCY 1000 // 1ms

#define LED_PORT            GPIOC
#define LED_PIN             GPIO13

#define I2C_PORT            I2C1
#define I2C_PIN_SDA         GPIO7
#define I2C_PIN_SCL         GPIO6

#define CONSOLE_USART_PORT          USART1
#define CONSOLE_USART_GPIO_PORT     GPIOA
#define CONSOLE_USART_PIN_TX        GPIO9
#define CONSOLE_USART_PIN_RX        GPIO10
#define CONSOLE_USART_BAUDRATE      115200
#define CONSOLE_USART_USE_DMA