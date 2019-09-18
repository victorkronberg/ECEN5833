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
#include "em_i2c.h"

// Power Load control
// Port D
// Pin 15

void i2c_init(void);
uint16_t i2c_read_2_bytes(uint8_t addr, uint8_t cmd, uint32_t delay);

#endif /* SRC_I2C_H_ */
