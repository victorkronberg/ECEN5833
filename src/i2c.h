/*
 * i2c.h
 * I2C initialization and control for EFR32BG13
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

/**
 * @description:  Initialize EFR32 I2C to Si7021 interface
 */
void i2c_init(void);

/**
 * @description:  Write a single byte command, delay a specified number of
 *    microseconds, then read 2 bytes from the I2C interface
 * @param        addr  [Slave address to read from]
 * @param        cmd   [Command to send to slave device]
 * @param        delay [Microseconds to delay between write and read]
 * @return             [Two bytes read from I2C slave device - 0 returned on error]
 */
uint16_t i2c_read_2_bytes(uint8_t addr, uint8_t cmd, uint32_t delay);

#endif /* SRC_I2C_H_ */
