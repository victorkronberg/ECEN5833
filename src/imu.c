/*
 * imu.c
 *
 *  Created on: Mar 21, 2020
 *      Author: vkronber
 */


#include "imu.h"




// Init

eIMU_ERRORS icm20948_init(struct imu_dev *dev)
{

}

// Set Bank
eIMU_ERRORS icm20948_set_bank(struct imu_dev *dev,uint8_t bank)
{

	eIMU_ERRORS rslt;

	if(bank > 3)
	{
		return eIMUErrorInvalidRange;
	}

	uint8_t reg_data[1];

	bank = (bank << USER_BANK_SHIFT) & USER_BANK_MASK;

	reg_data[0] = bank;

	rslt = icm20948_set_regs(REG_BANK_SEL,reg_data,ONE_BYTE,dev);

	return rslt;

}

// SW Reset
eIMU_ERRORS icm20948_sw_reset(struct imu_dev *dev)
{
	eIMU_ERRORS rslt;
	uint8_t reg_data[1];

	rslt = icm20948_set_bank(dev,USER_BANK0);

	if(rslt == eIMUErrorIMUok)
	{
			reg_data[0] = PWR_MGMT_RESET_MASK;

			rslt = icm20948_set_regs(PWR_MGMT_1,reg_data,ONE_BYTE,dev);

			// Delay for
	}

	return rslt;

}

// Enable/Disable sleep
eIMU_ERRORS icm20948_sleep(struct imu_dev *dev, bool sleep)
{
	eIMU_ERRORS rslt;
	uint8_t reg_data[1];

	rslt = icm20948_set_bank(dev,USER_BANK0);

	if(rslt == eIMUErrorIMUok)
	{
		rslt = icm20948_get_regs(PWR_MGMT_1,reg_data,ONE_BYTE,dev);

		if(rslt == eIMUErrorIMUok)
		{
			if(sleep)
			{
				reg_data |= PWR_MGMT_SLEEP_MASK;
				rslt = icm20948_set_regs(PWR_MGMT_1,reg_data,ONE_BYTE,dev);
			}
			else
			{
				reg_data &= ~PWR_MGMT_SLEEP_MASK;
				rslt = icm20948_set_regs(PWR_MGMT_1,reg_data,ONE_BYTE,dev);
				// Delay 20ms for accelerometer or 35ms for gyroscope
				dev->delay_ms(35);
			}
		}
	}

	return rslt;

}

// Enable/disable low power
eIMU_ERRORS icm20948_low_power(struct imu_dev *dev, bool low_power)
{
	eIMU_ERRORS rslt;
	uint8_t reg_data[1];

	rslt = icm20948_set_bank(dev,USER_BANK0);

	if(rslt == eIMUErrorIMUok)
	{
		rslt = icm20948_get_regs(PWR_MGMT_1,reg_data,ONE_BYTE,dev);

		if(rslt == eIMUErrorIMUok)
		{
			if(low_power)
			{
				reg_data |= PWR_MGMT_LOW_PWR_MASK;
				rslt = icm20948_get_regs(PWR_MGMT_1,reg_data,ONE_BYTE,dev);
			}
			else
			{
				reg_data &= ~PWR_MGMT_LOW_PWR_MASK;
				rslt = icm20948_get_regs(PWR_MGMT_1,reg_data,ONE_BYTE,dev);
			}
		}

	}

	return rslt;

}

// Set sampling mode


// Set scale

// Set DLPF

// Get AGMT data

eIMU_ERRORS icm20948_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, const struct imu_dev *dev)
{
	eIMU_ERRORS rslt;
	rslt = icm20948_null_ptr_check(dev);

	if(rslt == eIMUErrorIMUok)
	{
		reg_addr = reg_addr | READ;
		rslt = dev->read(dev->dev_id, reg_addr, reg_data, len);
	}

	return rslt;
}

eIMU_ERRORS icm20948_set_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, const struct imu_dev *dev)
{
	eIMU_ERRORS rslt;
	rslt = icm20948_null_ptr_check(dev);

	if(rslt == eIMUErrorIMUok)
	{
		rslt = dev->write(dev->dev_id, reg_addr, reg_data, len);
	}

	return rslt;

}

static eIMU_ERRORS icm20948_null_ptr_check(const struct imu_dev *dev)
{
	if(dev->read == NULL | dev->write == NULL | dev->delay_ms == NULL | dev == NULL)
	{
		return eIMUErrorNullPointer;
	}
	else
	{
		return eIMUErrorIMUok;
	}
}
