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

float read_temp(void)
{
	float temperature;
	uint16_t temp_code;

	enable_si7021_power();

	delay_us(80000);

	temp_code = i2c_read_2_bytes(Si7021Addr,READ_TEMP_NO_HOLD,CONVERSION_DELAY);

	temperature = ((175.72 * temp_code)/65536)-46.85;

	LOG_INFO("Temperature in degrees C: %f",temperature);

	disable_si7021_power();

	return temperature;

}
