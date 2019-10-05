/*
 * i2c.h
 * I2C initialization and control for EFR32BG13
 *
 *  Created on: Sep 15, 2019
 *      Author: Victor Kronberg
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include "main.h"
#include "i2cspm.h"
#include "em_i2c.h"

#define I2C_RXBUFFER_SIZE	2

I2C_TransferReturn_TypeDef ret;
I2C_TransferSeq_TypeDef txstruct;


/**
 * @description:  Initialize EFR32 I2C to Si7021 interface
 */
void i2c_init(void);

/**
 * [i2c_interrupt_write]
 * @description: Initialize interrupt driven I2C write sequence to write a 1 byte
 *               command to an I2C device at a given address
 * @param        addr [Address of I2C slave device]
 * @param        cmd  [Command to write to slave device register]
 */
void i2c_interrupt_write(uint8_t addr, uint8_t cmd);

/**
 * [i2c_interrupt_read_2_byte]
 * @description: Initialize interrupt drive I2C read sequence to read two bytes
 *               from an I2C device
 * @param        addr [Address of slave device to read from]
 */
void i2c_interrupt_read_2_byte(uint8_t addr);

/**
 * [i2c_return_last_2_bytes]
 * @description: Returns last two bytes written into the I2C transfer structure.
 *               This is to be called after i2c_interrupt_read_2_byte in order to
 *               return the bytes read from the slave device.
 * @return        [2 bytes from the i2c trasnfer structure as a 16 bit unisgned integer]
 */
uint16_t i2c_return_last_2_bytes(void);

#endif /* SRC_I2C_H_ */
