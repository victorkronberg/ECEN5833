/*
 * Si7021.h
 *
 *  Created on: Sep 14, 2019
 *      Author: vkronber
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

void init_si7021(void);
void enable_si7021_power(void);
void disable_si7021_power(void);
float read_temp(void);

#endif /* SRC_SI7021_H_ */
