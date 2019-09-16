/*
 * Si7021.c
 *
 *  Created on: Sep 14, 2019
 *      Author: vkronber
 */

#include "Si7021.h"

#define	SENSOR_ENABLE_PIN	(15)
#define	SENSOR_ENABLE_PORT	(gpioPortD)

void init_si7021(void)
{
	// Initialize sensor enable pin
	GPIO_PinModeSet(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN, gpioModePushPull, false);

	// Wait for power-up

	// Initialization I2C??? Wait for sensor data??
}

void enable_si7021_power(void)
{
	GPIO_PinOutSet(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN);
}

void disable_si7021_power(void)
{
	GPIO_PinOutClear(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN);
}
