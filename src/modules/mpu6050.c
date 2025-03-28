#include "mpu6050.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>

#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_CONFIG_REG 0x1B
#define GYRO_XOUT_H_REG 0x43

#define MPU6050_ADDR 0x68

static void i2c_setup(void);

void mpu_init(void){
	uint8_t cmd[2];
	uint8_t data;

	i2c_setup();

	cmd[0] = WHO_AM_I_REG;

	i2c_transfer7(I2C2, MPU6050_ADDR, cmd, 1, &data, 1);
	i2c_send_stop(I2C2);
	if(data == 104){
		cmd[0] = PWR_MGMT_1_REG;
		cmd[1] = 0;
        i2c_transfer7(I2C2, MPU6050_ADDR, cmd, 1, NULL, 0); //PRogram stucks here

        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		cmd[0] = SMPLRT_DIV_REG;
		cmd[1] = 0x07;
        i2c_transfer7(I2C2, MPU6050_ADDR, cmd, 1, NULL, 0);

        // Set accelerometer configuration in ACCEL_CONFIG Register
		cmd[0] = ACCEL_CONFIG_REG;
		cmd[1] = 0x00;
        i2c_transfer7(I2C2, MPU6050_ADDR, cmd, 1, NULL, 0);

        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
		cmd[0] = GYRO_CONFIG_REG;
		cmd[1] = 0x00;
        i2c_transfer7(I2C2, MPU6050_ADDR, cmd, 1, NULL, 0);
	}
}

uint8_t rec_data[14];
void mpu_read(void){
	const uint8_t cmd = ACCEL_XOUT_H_REG;
    i2c_transfer7(I2C2, MPU6050_ADDR, &cmd, 1, rec_data, 14);
}

static void i2c_setup(void) {
	// Errata
    // Enable clocks for I2C2 and AFIO
    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);

    // Reset the I2C peripheral
    I2C_CR1(I2C2) |= I2C_CR1_SWRST;
    I2C_CR1(I2C2) &= ~I2C_CR1_SWRST;

    // Disable the I2C peripheral
    i2c_peripheral_disable(I2C2);

    // Apply workaround for I2C BUSY flag issue
    // Configure SCL and SDA as General Purpose Output Open-Drain, High level
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO_I2C2_SCL | GPIO_I2C2_SDA);
    gpio_set(GPIOB, GPIO_I2C2_SCL | GPIO_I2C2_SDA);

    // Ensure SCL and SDA are High
    while (!(gpio_get(GPIOB, GPIO_I2C2_SCL) && gpio_get(GPIOB, GPIO_I2C2_SDA)));
 
    // Pull SDA Low
    gpio_clear(GPIOB, GPIO_I2C2_SDA);
    while (gpio_get(GPIOB, GPIO_I2C2_SDA));

    // Pull SCL Low
    gpio_clear(GPIOB, GPIO_I2C2_SCL);
    while (gpio_get(GPIOB, GPIO_I2C2_SCL));

    // Release SCL High
    gpio_set(GPIOB, GPIO_I2C2_SCL);
    while (!gpio_get(GPIOB, GPIO_I2C2_SCL));

    // Release SDA High
    gpio_set(GPIOB, GPIO_I2C2_SDA);
    while (!gpio_get(GPIOB, GPIO_I2C2_SDA));

    // Configure SCL and SDA back to Alternate function Open-Drain
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C2_SCL | GPIO_I2C2_SDA);

    // Reset I2C peripheral again
    I2C_CR1(I2C2) |= I2C_CR1_SWRST;
    I2C_CR1(I2C2) &= ~I2C_CR1_SWRST;

    // Set clock frequency for I2C (APB1 is running at 36MHz)
    i2c_set_clock_frequency(I2C2, 36);

    // Set I2C Fast Mode (400KHz)
    i2c_set_fast_mode(I2C2);

    // Set CCR and TRISE values
    i2c_set_ccr(I2C2, 0x1e);
    i2c_set_trise(I2C2, 0x0b);

    // Set own 7-bit slave address
    i2c_set_own_7bit_slave_address(I2C2, 0x32);

    // Enable the I2C peripheral
    i2c_peripheral_enable(I2C2);
}
