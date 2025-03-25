#pragma once

#define SYSTICK_FREQUENCY 1000 // 1ms

#define LED_RCC_CLK         RCC_GPIOB
#define LED_PIN             GPIO2
#define LED_PORT            GPIOB

#define I2C_PORT            I2C1
#define I2C_GPIO_PORT       GPIOB
#define I2C_PIN_SDA         GPIO7
#define I2C_PIN_SCL         GPIO6