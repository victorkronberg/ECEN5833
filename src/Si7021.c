/*
 * Si7021.c
 * Si7021 initialization and interface
 *
 *  Created on: Sep 14, 2019
 *      Author: vkronber
 */

#include "Si7021.h"

void init_si7021(void)
{
	// Initialize sensor enable pin
	GPIO_PinModeSet(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN, gpioModePushPull, false);

}

void enable_si7021_power(void)
{
	GPIO_PinOutSet(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN);
}

void disable_si7021_power(void)
{
	GPIO_PinOutClear(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN);
}

void si7021_init_i2c_temp_write(void)
{
	// Load I2c with slave address, write command, and 7021 command
	// Enable I2C interrupts
	i2c_interrupt_write(Si7021Addr,READ_TEMP_NO_HOLD);

}

void si7021_init_i2c_temp_read(void)
{
	// Load I2c with slave address read command
	// Enable I2C interrupts and return
	i2c_interrupt_read_2_byte(Si7021Addr);

}

float si7021_return_last_temp(void)
{
	float temperature;
	uint16_t temp_code;

	temp_code = i2c_return_last_2_bytes();

	// Calculate temperature from raw Si7021 data
	temperature = ((175.72 * temp_code)/65536)-46.85;

	return temperature;

}
