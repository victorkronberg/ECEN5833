/**************************************************************************//**
 * @main_series1_PG1_EFR.c
 * @brief Demonstrates USART1 as SPI master.
 * @version 0.0.1
 ******************************************************************************
 * @section License
 * <b>Copyright 2018 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_ldma.h"
#include "main.h"




uint8_t TxBuffer[TX_BUFFER_SIZE] = {0x80, 0x80, 0x80, 0x80};
uint32_t TxBufferIndex = 0;

uint8_t RxBuffer[RX_BUFFER_SIZE] = {0};
uint32_t RxBufferIndex = 0;

/**************************************************************************//**
 * @brief USART1 TX IRQ Handler
 *****************************************************************************/
void USART1_TX_IRQHandler(void)
{
  // Send and receive incoming data
  USART_Tx(USART1, TxBuffer[TxBufferIndex++]);

  // Stop sending once we've gone through the whole TxBuffer
  if (TxBufferIndex == TX_BUFFER_SIZE)
  {
    TxBufferIndex = 0;
  }
}

/**************************************************************************//**
 * @brief USART1 RX IRQ Handler
 *****************************************************************************/
void USART1_RX_IRQHandler(void)
{
  // Send and receive incoming data
  RxBuffer[RxBufferIndex++] = USART_Rx(USART1);

  // Stop once we've filled up our buffer
  if (RxBufferIndex == RX_BUFFER_SIZE)
  {
    // Place breakpoint here and observe RxBuffer
    // RxBuffer should contain 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9
    RxBufferIndex = 0;
    LOG_INFO("%d,%d,%d,%d",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3]);
  }
}

/**************************************************************************//**
 * @brief Initialize USART1
 *****************************************************************************/
/*
void initUSART1 (void)
{
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART1, true);

    // Configure GPIO mode
    GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 0); // US1_CLK is push pull
    GPIO_PinModeSet(gpioPortC, 9, gpioModePushPull, 1); // US1_CS is push pull
    GPIO_PinModeSet(gpioPortC, 6, gpioModePushPull, 1); // US1_TX (MOSI) is push pull
    GPIO_PinModeSet(gpioPortC, 7, gpioModeInput, 1);    // US1_RX (MISO) is input

    // Start with default config, then modify as necessary
    USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
    config.master       = true;            // master mode
    config.baudrate     = 1000000;         // CLK freq is 1 MHz
    config.autoCsEnable = true;            // CS pin controlled by hardware, not firmware
    config.clockMode    = usartClockMode0; // clock idle low, sample on rising/first edge
    config.msbf         = true;            // send MSB first
    config.enable       = usartDisable;    // Make sure to keep USART disabled until it's all set up
    USART_InitSync(USART1, &config);

    // Set USART pin locations
    USART1->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC11) | // US1_CLK       on location 11 = PC8 per datasheet section 6.4 = EXP Header pin 8
                        (USART_ROUTELOC0_CSLOC_LOC11)  | // US1_CS        on location 11 = PC9 per datasheet section 6.4 = EXP Header pin 10
                        (USART_ROUTELOC0_TXLOC_LOC11)  | // US1_TX (MOSI) on location 11 = PC6 per datasheet section 6.4 = EXP Header pin 4
                        (USART_ROUTELOC0_RXLOC_LOC11);   // US1_RX (MISO) on location 11 = PC7 per datasheet section 6.4 = EXP Header pin 6

    // Enable USART pins
    USART1->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_CSPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

    // Enabling TX interrupts to transfer whenever
    // there is room in the transmit buffer
    USART_IntClear(USART1, USART_IF_TXBL);
    USART_IntEnable(USART1, USART_IF_TXBL);
    NVIC_ClearPendingIRQ(USART1_TX_IRQn);
    NVIC_EnableIRQ(USART1_TX_IRQn);

    // Enabling RX interrupts to trigger whenever
    // a new packet arrives from the slave
    USART_IntClear(USART1, USART_IF_RXDATAV);
    USART_IntEnable(USART1, USART_IF_RXDATAV);
    NVIC_ClearPendingIRQ(USART1_RX_IRQn);
    NVIC_EnableIRQ(USART1_RX_IRQn);

    // Enable USART1
    USART_Enable(USART1, usartEnable);
}
*/

//Initialization for polled SPI
void initUSART1 (void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART1, true);

  GPIO_DriveStrengthSet(gpioPortC, gpioDriveStrengthWeakAlternateWeak);
  // Configure GPIO mode
  GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 1); // US1_CLK is push pull
  GPIO_PinModeSet(gpioPortC, 9, gpioModePushPull, 1); // US1_CS is push pull
  GPIO_PinModeSet(gpioPortC, 6, gpioModePushPull, 1); // US1_TX (MOSI) is push pull
  GPIO_PinModeSet(gpioPortC, 7, gpioModeInput, 1);    // US1_RX (MISO) is input

  // Start with default config, then modify as necessary
  USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
  config.master       = true;            // master mode
  config.baudrate     = 1000000;         // CLK freq is 1 MHz
  config.autoCsEnable = true;            // CS pin controlled by hardware, not firmware
  config.clockMode    = usartClockMode3; // Clock idle high, sample on rising edge.
  config.msbf         = true;            // send MSB first
  config.enable       = usartDisable;    // Make sure to keep USART disabled until it's all set up
  USART_InitSync(USART1, &config);

  // Set USART pin locations
  USART1->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC11) | // US1_CLK       on location 11 = PC8 per datasheet section 6.4 = EXP Header pin 8
                      (USART_ROUTELOC0_CSLOC_LOC11)  | // US1_CS        on location 11 = PC9 per datasheet section 6.4 = EXP Header pin 10
                      (USART_ROUTELOC0_TXLOC_LOC11)  | // US1_TX (MOSI) on location 11 = PC6 per datasheet section 6.4 = EXP Header pin 4
                      (USART_ROUTELOC0_RXLOC_LOC11);   // US1_RX (MISO) on location 11 = PC7 per datasheet section 6.4 = EXP Header pin 6

  // Enable USART pins
  USART1->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_CSPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

  // Enable pins for Chip Select
  GPIO_DriveStrengthSet(CS_PORT, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(CS_PORT, ICM_CS_PIN, gpioModePushPull, 1);

  // Enable USART1
  USART_Enable(USART1, usartEnable);
}


int8_t spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	//dev_id = chip select
	uint32_t i;
	GPIO_PinOutClear(CS_PORT,dev_id);

	USART_SpiTransfer(USART1, reg_addr|READ);

	for (i = 0; i < len; i++)
	{
		*reg_data = USART_SpiTransfer(USART1, 0x00);
		reg_data++;
	}

	GPIO_PinOutSet(CS_PORT,dev_id);

	return 0;
}

int8_t spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	//dev_id = chip select
	uint32_t i;
	GPIO_PinOutClear(CS_PORT,dev_id);

	USART_SpiTransfer(USART1, reg_addr);

	for (i = 0; i < len; i++)
	{
		USART_SpiTransfer(USART1, *reg_data);
		reg_data++;
	}

	GPIO_PinOutSet(CS_PORT,dev_id);

	return 0;
}

void delay_msec(uint32_t ms)
{
	// Configure Bluetooth stack timer delay
}


/**************************************************************************//**
 * @brief Main function
 *****************************************************************************/
/*
int main(void)
{
  // Initialize chip
  CHIP_Init();

  // Initialize USART1 as SPI master
  initUSART1();

  // After the USART is initialized transfers will automatically start based off of certain interrupt flag conditions
  // A new packet will be sent to the slave whenever the TX Buffer is empty after a previous successful transfer
  // and a packet will be sent from the slave back whenever it receives a packet from the master and will be handled
  // by the USART_RX_IRQHandler
  while(1);
}
*/

//int8_t EFR32_spi_read()
//int8_t EFR32_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
//int8_t EFR32_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
