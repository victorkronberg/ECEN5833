/*
 * i2c.c
 * I2C initialization and control for EFR32BG13
 *
 *  Created on: Sep 15, 2019
 *      Author: vkronber
 */


#include "i2c.h"

extern myStateTypeDef my_state_struct;

// Buffers for I2C
uint8_t i2c_txBuffer[1];
uint8_t i2c_txBufferSize;
uint8_t i2c_rxBuffer[I2C_RXBUFFER_SIZE];

void i2c_init(void)
{
	// Initialize I2C TX buffers
	i2c_txBuffer[0] = 0;
	i2c_txBufferSize = sizeof(i2c_txBuffer);

	// Initialize I2C using SiLabs driver
	I2CSPM_Init_TypeDef i2c_values = I2CSPM_INIT_DEFAULT;
	I2CSPM_Init(&i2c_values);

	NVIC_EnableIRQ(I2C0_IRQHandler);
}

void i2c_interrupt_write(uint8_t addr, uint8_t cmd)
{
	// Load I2c with slave address, write command, and slave command
	// Enable I2C interrupts

	// Initialize I2C TX buffers
	i2c_txBuffer = cmd;
	i2c_txBufferSize = sizeof(i2c_txBuffer);

	// Shift slave address to 8-bit format
	txstruct.addr = addr << 1;

	// Configure I2C structure for Write
	txstruct.buf[0].data = i2c_txBuffer;	// CMD
	txstruct.buf[0].len = i2c_txBufferSize;
	txstruct.flags = I2C_FLAG_WRITE;

	ret = I2C_TransferInit(I2C0, &txstruct);

	if(ret != i2cTransferInProgress)
	{
		LOG_ERROR("I2C write initialization failed with return code %d",ret)
	}
	else
	{
		// Enable interrupts
	}

}

void i2c_interrupt_read_2_byte(uint8_t addr)
{
	// Load I2c with slave address read command
	// Enable I2C interrupts and return

	// Initialize I2C TX buffers

	// Configure I2C structure for Read
	txstruct.buf[0].data = i2c_rxBuffer;
	txstruct.buf[0].len = I2C_RXBUFFER_SIZE;
	txstruct.flags = I2C_FLAG_READ;

	// Shift slave address to 8-bit format
	txstruct.addr = addr << 1;

	ret = I2C_TransferInit(I2C0, &txstruct);

	if(ret != i2cTransferInProgress)
	{
		LOG_ERROR("I2C read initialization failed with return code %d",ret)
	}
	else
	{
		// Enable interrupts
	}
}

uint16_t i2c_return_last_2_bytes(void)
{
	uint16_t RXdata;
	RXdata = i2c_rxBuffer[1] + (i2c_rxBuffer[0]<<8);
	return RXdata;
}

uint16_t i2c_read_2_bytes(uint8_t addr, uint8_t cmd, uint32_t delay)
{
	// Buffers for I2C
	i2c_txBuffer[0] = {cmd};
	i2c_txBufferSize = sizeof(i2c_txBuffer);
	uint8_t i2c_rxBuffer[I2C_RXBUFFER_SIZE];
	uint16_t RXdata;

	// Shift slave address to 8-bit format
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

void I2C0_IRQHandler(void)
{

	__disable_irq();
	// IRQ called when event has occurred that needs to be handled

	// If read and transfer is complete - set event flag to read temperature, otherwise continue I2CTransfer and report any errors
	ret = I2C_TransferInit(I2C0, &txstruct);

	if(ret == i2cTransferInProgress)
	{
		// Reset interrupts
	}
	else if(ret == i2cTransferDone)
	{
		// Set bit for I2C event complete
		my_state_struct.event_bitmask |= I2C_EVENT_MASK;

		// Disable interrupts
	}
	else
	{
		LOG_ERROR("I2C transfer ended prematurely with return of %d",ret);
	}

	__enable_irq();

}
