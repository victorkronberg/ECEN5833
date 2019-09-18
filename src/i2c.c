/*
 * i2c.c
 *
 *  Created on: Sep 15, 2019
 *      Author: vkronber
 */


#include "i2c.h"

#define I2C_RXBUFFER_SIZE	2


I2C_TransferReturn_TypeDef ret;
I2C_TransferSeq_TypeDef txstruct;



void i2c_init(void)
{
	I2CSPM_Init_TypeDef i2c_values = I2CSPM_INIT_DEFAULT;
	I2CSPM_Init(&i2c_values);

}


uint16_t i2c_read_2_bytes(uint8_t addr, uint8_t cmd, uint32_t delay)
{
	// Buffers++
	uint8_t i2c_txBuffer[1] = {cmd};
	uint8_t i2c_txBufferSize = sizeof(i2c_txBuffer);
	uint8_t i2c_rxBuffer[I2C_RXBUFFER_SIZE];
	uint16_t RXdata;

	txstruct.addr = addr << 1;

	// Configure I2C structure for Write
	txstruct.buf[0].data = i2c_txBuffer;	// CMD
	txstruct.buf[0].len = i2c_txBufferSize;
	txstruct.flags = I2C_FLAG_WRITE;

	// Write to I2C slave
	ret = I2CSPM_Transfer(I2C0,&txstruct);

	if(ret != i2cTransferDone)
	{
		// Error logging
		LOG_ERROR("Failed to write %d bytes, return value was %d. Next temp value is invalid",i2c_txBufferSize,ret);
	}

	// Wait delay # of microseconds for slave processing
	delay_us(delay);

	// Configure I2C structure for Read
	txstruct.buf[0].data = i2c_rxBuffer;
	txstruct.buf[0].len = I2C_RXBUFFER_SIZE;
	txstruct.flags = I2C_FLAG_READ;

	// Read from I2C slave
	ret = I2CSPM_Transfer(I2C0,&txstruct);

	if(ret != i2cTransferDone)
	{
		// Error logging
		LOG_ERROR("Failed to read %d bytes, return value was %d. Next temp value is invalid",I2C_RXBUFFER_SIZE,ret);
		RXdata = 0;
	}
	else
	{
		RXdata = i2c_rxBuffer[1] + (i2c_rxBuffer[0]<<8);
	}

	return RXdata;
}
