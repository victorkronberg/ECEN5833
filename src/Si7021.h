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
#define CONVERSION_DELAY	(11)
#define POWER_UP_DELAY		(80)

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
 * [si7021_init_i2c_temp_write]
 * @description: Calls I2C function to write "Read" command to Si7021
 */
void si7021_init_i2c_temp_write(void);

/**
 * [si7021_init_i2c_temp_read]
 * @description: Calls I2C function to read from Si7021.  This is called after
 *               si7021_init_i2c_temp_write.
 */
void si7021_init_i2c_temp_read(void);

/**
 * [si7021_return_last_temp]
 * @description: Read temperature from Si7021. This function is called after
 *               si7021_init_i2c_temp_read once the I2C transaction is complete.
 *               It takes the 16 bit unsigned int raw output from the si7021 and
 *               converts it into milli-degrees C.
 * @return        [Temperature in degrees C (uint32_t)]
 */
uint32_t si7021_return_last_temp(void);

#endif /* SRC_SI7021_H_ */
