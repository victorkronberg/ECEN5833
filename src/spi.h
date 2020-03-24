/*
 * spi.h
 *
 *  Created on: Feb 15, 2020
 *      Author: vkronber
 */

#ifndef SRC_SPI_H_
#define SRC_SPI_H_

#define TX_BUFFER_SIZE   14
#define RX_BUFFER_SIZE   TX_BUFFER_SIZE

#define READ	(0x80)

#define SPI_DEVICE_NO (2)

#define CS_PORT	gpioPortD

#define BMP388_CS_PIN	(13)
#define BMP388_DEV_ID	BMP388_CS_PIN

#define ICM_CS_PIN		(10)
#define ICM_DEV_ID		ICM_CS_PIN


void initUSART1 (void);
int8_t spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

#endif /* SRC_SPI_H_ */
