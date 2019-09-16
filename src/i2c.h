/*
 * i2c.h
 *
 *  Created on: Sep 15, 2019
 *      Author: vkronber
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include "main.h"
#include "i2cspm.h"

#define i2c_init_EFR32BG13															\
{																			\
	I2C0,          				/**< Peripheral port */						\
	gpioPortC,        			/**< SCL pin port number */					\
	10,         				/**< SCL pin number */						\
	gpioPortC,        			/**< SDA pin port number */					\
	11,         				/**< SDA pin number */						\
	15, 						/**< Port location of SCL signal */			\
	15, 						/**< Port location of SDA signal */			\
	0,     						/**< I2C reference clock */					\
	I2C_FREQ_STANDARD_MAX,     /**< I2C max bus frequency to use */			\
	i2cClockHLRStandard        /**< Clock low/high ratio control */			\
}

// Power Load control
// Port D
// Pin 15

void i2c_init(void);

#endif /* SRC_I2C_H_ */
