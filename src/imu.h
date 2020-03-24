/*
 * imu.h
 *
 *  Created on: Mar 21, 2020
 *      Author: vkronber
 */

#ifndef SRC_IMU_H_
#define SRC_IMU_H_

#include "main.h"

#define 	WHO_AM_I		(0x00)
#define		WHO_AM_I_ID		(0xEA)

//#define		PWR_MGMT_1	(0x06)
#define		SLEEP_MODE		(0X40)
#define		WAKE_MODE		(0x00)
#define		AUTO_CLK		(0x01)

//#define		PWR_MGMT_2	(0x07)
#define		ACCEL_DIS		(0x38)
#define		GYRO_DIS		(0x03)

#define		ACCEL_DATA		(0x45)
#define		GYRO_DATA		(0x51)

#define 	BANK_SELECT		(0x7F)

#define		USER_BANK_MASK	(0x30)
#define		USER_BANK_SHIFT	(4)

#define		USER_BANK0		(0x00)

#define 	PWR_MGMT_LOW_PWR_MASK	(0x20)
#define 	PWR_MGMT_SLEEP_MASK		(0x40)
#define 	PWR_MGMT_RESET_MASK		(0x80)

#define		ONE_BYTE		(1)


/* User Bank 0 Registers */
#define WHO_AM_I_ICM20948  		(0x00) // Should return 0xEA
#define USER_CTRL         	 	(0x03)  // Bit 7 enable DMP, bit 3 reset DMP
#define LP_CONFIG		   		(0x05) 
#define PWR_MGMT_1        		(0x06) // Device defaults to the SLEEP mode
#define PWR_MGMT_2        		(0x07)
#define INT_PIN_CFG       		(0x0F)
#define INT_ENABLE        		(0x10)
#define INT_ENABLE_1	   		(0x11) 
#define INT_ENABLE_2	   		(0x12)
#define INT_ENABLE_3	   		(0x13)
#define I2C_MST_STATUS     		(0x17)
#define INT_STATUS         		(0x19)
#define INT_STATUS_1	   		(0x1A)
#define INT_STATUS_2	   		(0x1B) 
#define INT_STATUS_3	   		(0x1C) 
#define DELAY_TIMEH				(0x28)	
#define DELAY_TIMEL				(0x29)	
#define ACCEL_XOUT_H       		(0x2D)
#define ACCEL_XOUT_L      		(0x2E)
#define ACCEL_YOUT_H       		(0x2F)
#define ACCEL_YOUT_L       		(0x30)
#define ACCEL_ZOUT_H      	 	(0x31)
#define ACCEL_ZOUT_L      		(0x32)
#define GYRO_XOUT_H        		(0x33)
#define GYRO_XOUT_L        		(0x34)
#define GYRO_YOUT_H        		(0x35)
#define GYRO_YOUT_L        		(0x36)
#define GYRO_ZOUT_H        		(0x37)
#define GYRO_ZOUT_L        		(0x38)
#define TEMP_OUT_H         		(0x39)
#define TEMP_OUT_L         		(0x3A)
#define EXT_SENS_DATA_00   		(0x3B)
#define EXT_SENS_DATA_01   		(0x3C)
#define EXT_SENS_DATA_02   		(0x3D)
#define EXT_SENS_DATA_03   		(0x3E)
#define EXT_SENS_DATA_04   		(0x3F)
#define EXT_SENS_DATA_05   		(0x40)
#define EXT_SENS_DATA_06   		(0x41)
#define EXT_SENS_DATA_07   		(0x42)
#define EXT_SENS_DATA_08   		(0x43)
#define EXT_SENS_DATA_09   		(0x44)
#define EXT_SENS_DATA_10   		(0x45)
#define EXT_SENS_DATA_11   		(0x46)
#define EXT_SENS_DATA_12   		(0x47)
#define EXT_SENS_DATA_13   		(0x48)
#define EXT_SENS_DATA_14   		(0x49)
#define EXT_SENS_DATA_15   		(0x4A)
#define EXT_SENS_DATA_16  		(0x4B)
#define EXT_SENS_DATA_17  		(0x4C)
#define EXT_SENS_DATA_18   		(0x4D)
#define EXT_SENS_DATA_19   		(0x4E)
#define EXT_SENS_DATA_20   		(0x4F)
#define EXT_SENS_DATA_21   		(0x50)
#define EXT_SENS_DATA_22   		(0x51)
#define EXT_SENS_DATA_23   		(0x52)
#define FIFO_EN_1          		(0x66)
#define FIFO_EN_2          		(0x67) 
#define FIFO_RST		   		(0x68) 
#define FIFO_MODE		   		(0x69)
#define FIFO_COUNTH        		(0x70)
#define FIFO_COUNTL        		(0x71)
#define FIFO_R_W           		(0x72)
#define DATA_RDY_STATUS			(0x74) 
#define FIFO_CFG		   		(0x76) 
#define REG_BANK_SEL	   		(0x7F) 

// USER BANK 1 REGISTER MAP
#define SELF_TEST_X_GYRO  		(0x02)
#define SELF_TEST_Y_GYRO  		(0x03)
#define SELF_TEST_Z_GYRO  		(0x04)
#define SELF_TEST_X_ACCEL 		(0x0E)
#define SELF_TEST_Y_ACCEL 		(0x0F)
#define SELF_TEST_Z_ACCEL 		(0x10)
#define XA_OFFSET_H       		(0x14)
#define XA_OFFSET_L       		(0x15)
#define YA_OFFSET_H       		(0x17)
#define YA_OFFSET_L       		(0x18)
#define ZA_OFFSET_H       		(0x1A)
#define ZA_OFFSET_L       		(0x1B)
#define TIMEBASE_CORRECTION_PLL	(0x28)

// USER BANK 2 REGISTER MAP
#define GYRO_SMPLRT_DIV        	(0x00) 
#define GYRO_CONFIG_1      		(0x01) 
#define GYRO_CONFIG_2      		(0x02) 
#define XG_OFFSET_H       		(0x03)  // User-defined trim values for gyroscope
#define XG_OFFSET_L       		(0x04)
#define YG_OFFSET_H       		(0x05)
#define YG_OFFSET_L       		(0x06)
#define ZG_OFFSET_H       		(0x07)
#define ZG_OFFSET_L       		(0x08)
#define ODR_ALIGN_EN			(0x09) 
#define ACCEL_SMPLRT_DIV_1     	(0x10) 
#define ACCEL_SMPLRT_DIV_2     	(0x11) 
#define ACCEL_INTEL_CTRL		(0x12) 
#define ACCEL_WOM_THR			(0x13) 
#define ACCEL_CONFIG      		(0x14)
#define ACCEL_CONFIG_2     		(0x15) 
#define FSYNC_CONFIG			(0x52) 
#define TEMP_CONFIG				(0x53) 
#define MOD_CTRL_USR			(0x54) 

/* Magnetometer Register Addresses */
#define WHO_AM_I_AK09916	(0x01)  // (AKA WIA2) should return 0x09
#define AK09916_ST1     	(0x10)  // data ready status bit 0
#define AK09916_XOUT_L  	(0x11)  // data
#define AK09916_XOUT_H  	(0x12)
#define AK09916_YOUT_L  	(0x13)
#define AK09916_YOUT_H  	(0x14)
#define AK09916_ZOUT_L  	(0x15)
#define AK09916_ZOUT_H  	(0x16)
#define AK09916_ST2     	(0x18)  // Data overflow bit 3 and data read error status bit 2
#define AK09916_CNTL    	(0x31)  // Power down (0000), single-measurement (0001), self-test (1000) modes on bits 3:0
#define AK09916_CNTL2   	(0x32)   // Normal (0), Reset (1)

/* Error enumerations */
typedef enum imu_errors {
	eIMUErrorInvalidRange = -10,
	eIMUErrorNullPointer = -1,
	eIMUErrorIMUok = 0
} eIMU_ERRORS;


/*!
 * @brief Type definitions
 */
typedef int8_t (*imu_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
typedef void (*imu_delay_fptr_t)(uint32_t period);

/*
 * @brief IMU device struct
 */

struct imu_dev
{
    /*! Chip Id */
    uint8_t chip_id;

    /*! Device Id */
    uint8_t dev_id;

    /*! Read function pointer */
    imu_com_fptr_t read;

    /*! Write function pointer */
    imu_com_fptr_t write;

    /*! Delay function pointer */
    imu_delay_fptr_t delay_ms;
};

#endif /* SRC_IMU_H_ */
