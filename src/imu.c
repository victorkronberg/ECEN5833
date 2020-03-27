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

	if(bank > USER_BANK3)
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
eIMU_ERRORS icm20948_sampling_mode(struct imu_dev *dev, uint8_t sensors, uint8_t sample_mode, imu_sample_rate_t *sample_rate_t)
{
	eIMU_ERRORS rslt;
	uint8_t reg_data[2];

	if(!sensors & (ICM_20948_GYR_CYC_MASK | ICM_20948_ACC_CYC_MASK))
	{
		return eIMUErrorInvalidRange;
	}

	rslt = icm20948_set_bank(dev,USER_BANK0);

	if(rslt != eIMUErrorIMUok)
	{
		return rslt;
	}

	rslt = icm20948_get_regs(LP_CONFIG,reg_data,ONE_BYTE,dev);

	if(rslt != eIMUErrorIMUok)
	{
		return rslt;
	}

	reg_data |= (sensors & sample_mode);

	rslt = icm20948_set_regs(LP_CONFIG,reg_data,ONE_BYTE,dev);

	if(rslt != eIMUErrorIMUok)
	{
		return rslt;
	}

	// Set sample rate of in duty-cycled mode
	if(sample_mode == ICM_20948_CYCLED)
	{
		// Change to Bank2 to set sample rate divider
		rslt = icm20948_set_bank(dev,USER_BANK2);

		if(rslt != eIMUErrorIMUok)
		{
			return rslt;
		}

		if(sensors & ICM_20948_GYR_CYC_MASK)
		{
			reg_data[0] = sample_rate_t->gyro;

			rslt = icm20948_set_regs(GYRO_SMPLRT_DIV,reg_data,ONE_BYTE,dev);

			if(rslt != eIMUErrorIMUok)
			{
				return rslt;
			}
		}

		if(sensors & ICM_20948_ACC_CYC_MASK)
		{
			reg_data[0] = 0xFF & sample_rate_t->accel;
			reg_data[1] = 0xFF & (sample_rate_t->accel >> 8);

			rslt = icm20948_set_regs(ACCEL_SMPLRT_DIV_1,reg_data,TWO_BYTES,dev);

			if(rslt != eIMUErrorIMUok)
			{
				return rslt;
			}
		}
	}

	return rslt;

}

// Set sampling mode
eIMU_ERRORS icm20948_sensor_enable(struct imu_dev *dev, uint8_t sensors, bool enable)
{
	eIMU_ERRORS rslt;
	uint8_t reg_data[1];

	if(!(sensors & (PWR_MGMT_GYR_MASK | PWR_MGMT_ACC_MASK)))
	{
		return eIMUErrorInvalidRange;
	}

	rslt = icm20948_set_bank(dev,USER_BANK0);

	if(rslt != eIMUErrorIMUok)
	{
		return rslt;
	}

	rslt = icm20948_get_regs(PWR_MGMT_2,reg_data,ONE_BYTE,dev);

	if(rslt != eIMUErrorIMUok)
	{
		return rslt;
	}

	if(enable)
	{
		reg_data &= ~sensors;
	}
	else
	{
		reg_data |= sensors;
	}

	rslt = icm20948_set_regs(PWR_MGMT_2,reg_data,ONE_BYTE,dev);

	return rslt;

}

// Set scale
eIMU_ERRORS icm20948_set_full_scale(struct imu_dev *dev, uint8_t sensors,imu_full_scale_select_t *fss_t)
{
	eIMU_ERRORS rslt;
	uint8_t reg_data[1];

	if(!(sensors & (GYRO_CONFIG | ACCEL_CONFIG)))
	{
		return eIMUErrorInvalidRange;
	}

	rslt = icm20948_set_bank(dev,USER_BANK2);

	if(rslt != eIMUErrorIMUok)
	{
		return rslt;
	}

	if(sensors & GYRO_CONFIG)
	{

		if(fss_t->gyro > GFSS_2000_DPS)
		{
			return eIMUErrorInvalidRange;
		}

		rslt = icm20948_get_regs(GYRO_CONFIG_1,reg_data,ONE_BYTE,dev);

		if(rslt != eIMUErrorIMUok)
		{
			return rslt;
		}

		reg_data[0] |= (fss_t->gyro << GFSS_SHIFT);

		rslt = icm20948_set_regs(GYRO_CONFIG_1,reg_data,ONE_BYTE,dev);

		if(rslt != eIMUErrorIMUok)
		{
			return rslt;
		}

	}

	if(sensors & ACCEL_CONFIG)
	{

		if(fss_t->accel > AFSS_16_G)
		{
			return eIMUErrorInvalidRange;
		}

		rslt = icm20948_get_regs(ACCEL_CONFIG,reg_data,ONE_BYTE,dev);

		if(rslt != eIMUErrorIMUok)
		{
			return rslt;
		}

		reg_data[0] |= (fss_t->accel << AFSS_SHIFT);

		rslt = icm20948_set_regs(ACCEL_CONFIG,reg_data,ONE_BYTE,dev);

	}


	return rslt;
}


// Set DLPF
eIMU_ERRORS icm20948_set_dlpf(struct imu_dev *dev, uint8_t sensors, imu_dplf_t *dplf_t, bool enable_dplf)
{
	eIMU_ERRORS rslt;
	uint8_t reg_data[1];

	if(!(sensors & (GYRO_CONFIG | ACCEL_CONFIG)))
	{
		return eIMUErrorInvalidRange;
	}

	rslt = icm20948_set_bank(dev,USER_BANK2);

	if(rslt != eIMUErrorIMUok)
	{
		return rslt;
	}

	if(sensors & GYRO_CONFIG)
	{

		if(dplf_t->gyro > 7)
		{
			return eIMUErrorInvalidRange;
		}

		rslt = icm20948_get_regs(GYRO_CONFIG_1,reg_data,ONE_BYTE,dev);

		if(rslt != eIMUErrorIMUok)
		{
			return rslt;
		}

		reg_data[0] |= (dplf_t->gyro << GDPLF_SHIFT);

		rslt = icm20948_set_regs(GYRO_CONFIG_1,reg_data,ONE_BYTE,dev);

		if(rslt != eIMUErrorIMUok)
		{
			return rslt;
		}

	}

	if(sensors & ACCEL_CONFIG)
	{

		if(dplf_t->accel > 7)
		{
			return eIMUErrorInvalidRange;
		}

		rslt = icm20948_get_regs(ACCEL_CONFIG,reg_data,ONE_BYTE,dev);

		if(rslt != eIMUErrorIMUok)
		{
			return rslt;
		}

		reg_data[0] |= (dplf_t->accel << ADPLF_SHIFT);

		rslt = icm20948_set_regs(ACCEL_CONFIG,reg_data,ONE_BYTE,dev);

	}


	return rslt;
}

// Set sampling frequency

// Get AGMT data
eIMU_ERRORS icm20948_get_agmt(struct imu_dev *dev, uint8_t sensors, imu_sensor_data_t *sensor_data)
{

	eIMU_ERRORS rslt;
	uint8_t reg_data[6];

	if(!(sensors & (ACCELEROMETER | GYROSCOPE | THERMOMETER)))
	{
		return eIMUErrorInvalidRange;
	}

	rslt = icm20948_set_bank(dev, USER_BANK0);

	if(rslt != eIMUErrorIMUok)
	{
		return rslt;
	}

	if(sensors & ACCELEROMETER)
	{
		rslt = icm20948_get_regs(ACCEL_XOUT_H,reg_data,SIX_BYTES,dev);

		//if(rslt != )
	}

}

eIMU_ERRORS icm20948_parse_sensor_data(uint8_t *reg_data, uint8_t sensors, imu_sensor_data_t *sensor_data)
{
	uint8_t shift,count;
	if(reg_data == NULL | sensor_data == NULL)
	{
		return eIMUErrorNullPointer;
	}

	switch (sensors)
	{
		case ACCELEROMETER:
			//count =
			break;
		case GYROSCOPE:
			break;
		case THERMOMETER:
			break;
	}

}

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
