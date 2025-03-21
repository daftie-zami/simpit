#include "mpu6050.h"

#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void i2c_setup(void)
{
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO_I2C1_SCL | GPIO_I2C1_SDA);

	i2c_peripheral_disable(I2C1);
	i2c_reset(I2C1);
	i2c_set_speed(I2C1, i2c_speed_fm_400k, I2C_CR2_FREQ_36MHZ);
	i2c_peripheral_enable(I2C1);

}










