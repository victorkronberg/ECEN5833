/*
 * Si7021.h
 * Si7021 initialization and interface
 *
 *  Created on: Sep 14, 2019
 *      Author: Victor Kronberg
 */

#ifndef SRC_SI7021_H_
#define SRC_SI7021_H_

#include "main.h"

#define	SENSOR_ENABLE_PIN	(15)
#define	SENSOR_ENABLE_PORT	(gpioPortD)
#define Si7021Addr			(0x40)
#define READ_TEMP_NO_HOLD	(0xF3)
#define CONVERSION_DELAY	(10800)
#define POWER_UP_DELAY		(80000)

/**
  * @description: Initializes Si7021 humidity and temperature sensor
 */
void init_si7021(void);

/**
 * @description: Enables power block to Si7021
 */
void enable_si7021_power(void);

/**
 * @description: Disables power block to Si7021
 */
void disable_si7021_power(void);

/**
 * @description:  Read current temperature from Si7021
 * @return        [temperature in degrees C - floating point]
 */
float read_temp(void);

#endif /* SRC_SI7021_H_ */