/*
 * imu.h
 *
 *  Created on: Mar 21, 2020
 *      Author: vkronber
 */

#ifndef SRC_IMU_H_
#define SRC_IMU_H_

#include "main.h"

#define 	WHO_AM_I					(0x00)
#define		ICM20948_WHO_AM_I_ID		(0xEA)

#define		ICM_ENABLE		(true)
#define		ICM_DISABLE		(false)


//#define		PWR_MGMT_1	(0x06)
#define		SLEEP_MODE		(0X40)
#define		WAKE_MODE		(0x00)
#define		AUTO_CLK		(0x01)



#define 	BANK_SELECT		(0x7F)

//ICM20948 User Bank Register Masks
#define		USER_BANK_MASK	(0x30)
#define		USER_BANK_SHIFT	(4)

#define		USER_BANK0		(0x00)
#define		USER_BANK1		(0x01)
#define		USER_BANK2		(0x02)
#define		USER_BANK3		(0x03)

// ICM20948 User Bank 0: USER_CTRL Macros
#define		ICM_20948_I2C_IF_DIS		(0x10)

// ICM20948 User Bank 0: LP_CONFIG Macros
#define		ICM_20948_GYR_CYC_MASK		(0x10)
#define 	ICM_20948_ACC_CYC_MASK		(0x20)
#define		ICM_20948_CONTINUOUS		(0x00)
#define		ICM_20948_CYCLED			(0xFF)

// ICM20948 User Bank 0: PWR_MGMT_1 Macros
#define 	PWR_MGMT_LOW_PWR_MASK	(0x20)
#define 	PWR_MGMT_SLEEP_MASK		(0x40)
#define 	PWR_MGMT_RESET_MASK		(0x80)

// ICM20948 User Bank 0: PWR_MGMT_2 Macros
#define		PWR_MGMT_ACC_MASK		(0x38)
#define		PWR_MGMT_GYR_MASK		(0x03)

// ICM20948 User Bank 2: GYRO_CONFIG_1 Macros
#define		GYRO_CONFIGURATION		(0x01)
#define		GFSS_250_DPS			(0x00)
#define		GFSS_500_DPS			(0x01)
#define		GFSS_1000_DPS			(0x02)
#define		GFSS_2000_DPS			(0x03)
#define		GFSS_SHIFT				(1)

#define 	GDPLF_SHIFT				(3)


// ICM20948 User Bank 2: ACCEL_CONFIG_1 Macros
#define		ACCEL_CONFIGURATION		(0x02)
#define		AFSS_2_G				(0x00)
#define		AFSS_4_G				(0x01)
#define		AFSS_8_G				(0x02)
#define		AFSS_16_G				(0x03)
#define		AFSS_SHIFT				(1)

#define		ADPLF_SHIFT				(1)

// General ICM20948 Macros

#define 	ACCELEROMETER			(0x01)
#define		GYROSCOPE				(0x02)
#define		THERMOMETER				(0x04)

#define		X_AXIS_H				(0)
#define		X_AXIS_L				(1)
#define		Y_AXIS_H				(2)
#define		Y_AXIS_L				(3)
#define		Z_AXIS_H				(4)
#define		Z_AXIS_L				(5)

// Compensation values
#define		ACCEL_2G				(16384)
#define		ACCEL_4G				(8192)
#define		ACCEL_8G				(4096)
#define		ACCEL_16G				(2048)

#define		GYRO_250DPS_100X		(13107)
#define		GYRO_500DPS_100X		(6554)
#define		GYRO_1000DPS_100X		(3277)
#define		GYRO_2000DPS_100X		(1638)

#define		ROOM_TEMP_OFFSET		(21)
#define		TEMP_SENSITIVITY		(334)

#define		TEMP_H					(0)
#define		TEMP_L					(1)

#define		ONE_BYTE		(1)
#define		TWO_BYTES		(2)
#define		SIX_BYTES		(6)



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
	eIMUErrorDevNotFound = -2,
	eIMUErrorNullPointer = -1,
	eIMUErrorIMUok = 0
} eIMU_ERRORS;


/*!
 * @brief Type definitions
 */
typedef int8_t (*imu_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
typedef void (*imu_delay_fptr_t)(uint32_t period);

typedef struct{
	uint8_t gyro;
	uint16_t accel;
} imu_sample_rate_t;

typedef struct{
	uint8_t gyro;
	uint32_t gyro_divisor[4];
	uint8_t accel;
	uint32_t accel_divisor[4];
} imu_full_scale_t;

typedef struct{
	uint8_t gyro;
	uint8_t accel;
} imu_dplf_t;

typedef struct{
	int16_t gyroscope_x;
	int16_t gyroscope_y;
	int16_t gyroscope_z;
	int16_t accelerometer_x;
	int16_t accelerometer_y;
	int16_t accelerometer_z;
	int16_t temperature;
} imu_sensor_data_t;

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

    /*! DPLF settings */
    imu_dplf_t dplf_settings;

    /*! Sample rate settings */
    imu_sample_rate_t sample_rate;

    /*! Full Scale range settings */
    imu_full_scale_t full_scale;

    /*! Sensor data */
    imu_sensor_data_t sensor_data;

};


eIMU_ERRORS icm20948_init(struct imu_dev *dev);

eIMU_ERRORS icm20948_who_am_i(struct imu_dev *dev);

eIMU_ERRORS icm20948_set_bank(struct imu_dev *dev,uint8_t bank);

eIMU_ERRORS icm20948_sw_reset(struct imu_dev *dev);

eIMU_ERRORS icm20948_spi_enable(struct imu_dev *dev);

eIMU_ERRORS icm20948_sleep(struct imu_dev *dev, bool sleep);

eIMU_ERRORS icm20948_low_power(struct imu_dev *dev, bool low_power);

eIMU_ERRORS icm20948_sampling_mode(struct imu_dev *dev, uint8_t sensors, uint8_t sample_mode);

eIMU_ERRORS icm20948_sensor_enable(struct imu_dev *dev, uint8_t sensors, bool enable);

eIMU_ERRORS icm20948_set_full_scale(struct imu_dev *dev, uint8_t sensors);

eIMU_ERRORS icm20948_set_dlpf(struct imu_dev *dev, uint8_t sensors, bool enable_dplf);

eIMU_ERRORS icm20948_get_agmt(struct imu_dev *dev, uint8_t sensors);

eIMU_ERRORS icm20948_parse_sensor_data(struct imu_dev *dev, uint8_t *reg_data, uint8_t sensors);

eIMU_ERRORS icm20948_compensate_data(struct imu_dev *dev,uint8_t sensors);

eIMU_ERRORS icm20948_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, const struct imu_dev *dev);

eIMU_ERRORS icm20948_set_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, const struct imu_dev *dev);

eIMU_ERRORS icm20948_null_ptr_check(const struct imu_dev *dev);

#endif /* SRC_IMU_H_ */
