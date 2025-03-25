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

void mpu_init(){
	uint8_t cmd[2];
	uint8_t data;

	i2c_setup();

	cmd[0] = WHO_AM_I_REG;
	i2c_transfer7(I2C1, MPU6050_ADDR, cmd, 1, &data, 1);
	if(data == 104){
		cmd[0] = PWR_MGMT_1_REG;
		cmd[1] = 0;
        i2c_transfer7(I2C1, MPU6050_ADDR, cmd, 1, NULL, 0); //PRogram stucks here

        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		cmd[0] = SMPLRT_DIV_REG;
		cmd[1] = 0x07;
        i2c_transfer7(I2C1, MPU6050_ADDR, cmd, 1, NULL, 0);

        // Set accelerometer configuration in ACCEL_CONFIG Register
		cmd[0] = ACCEL_CONFIG_REG;
		cmd[1] = 0x00;
        i2c_transfer7(I2C1, MPU6050_ADDR, cmd, 1, NULL, 0);

        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
		cmd[0] = GYRO_CONFIG_REG;
		cmd[1] = 0x00;
        i2c_transfer7(I2C1, MPU6050_ADDR, cmd, 1, NULL, 0);
	}
}


uint8_t rec_data[14];
void mpu_read(void){
	const uint8_t cmd = ACCEL_XOUT_H_REG;
    i2c_transfer7(I2C1, MPU6050_ADDR, &cmd, 14, rec_data, 6);
}

static void i2c_setup(void){
	/* Enable clocks for I2C1 and AFIO. */
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_AFIO);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO_I2C1_SCL | GPIO_I2C1_SDA);

    rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_reset_pulse(RST_I2C1);

	/* Disable the I2C before changing any configuration. */
	i2c_peripheral_disable(I2C1);

	i2c_set_speed(I2C1, i2c_speed_fm_400k, 36);

	/*
	 * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
	 * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
	 * Datasheet suggests 0x1e.
	 */
	i2c_set_ccr(I2C1, 0x1e);

	/* If everything is configured -> enable the peripheral. */
	i2c_peripheral_enable(I2C1);
	

}