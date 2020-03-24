/*
 * pressure_sensor.c
 *
 *  Created on: Mar 16, 2020
 *      Author: vkronber
 */

#include "pressure_sensor.h"


/*
 *   LOG_INFO("Result: %d", rslt);

  // Low power handheld device settings
  bmp_device.settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
  bmp_device.settings.odr_filter.press_os = BMP3_NO_OVERSAMPLING; //BMP3_OVERSAMPLING_8X;
  bmp_device.settings.odr_filter.iir_filter = BMP3_IIR_FILTER_DISABLE; //BMP3_IIR_FILTER_COEFF_1;
  bmp_device.settings.odr_filter.odr = BMP3_ODR_200_HZ; //BMP3_ODR_12_5_HZ;

  bmp_device.settings.press_en = BMP3_ENABLE;
  bmp_device.settings.temp_en = BMP3_ENABLE;

  setting_sel = BMP3_PRESS_OS_SEL|BMP3_TEMP_OS_SEL|BMP3_ODR_SEL|BMP3_PRESS_EN_SEL|BMP3_TEMP_EN_SEL;

  rslt = bmp3_set_sensor_settings(setting_sel,&bmp_device); //BMP3_IIR_FILTER_SEL

  LOG_INFO("Sensor setting result: %d", rslt);

  bmp_device.settings.op_mode = BMP3_NORMAL_MODE;

  rslt = bmp3_set_op_mode(&bmp_device);

  LOG_INFO("Op mode result: %d", rslt);
 */
/*
int8_t bmp388_set_normal_mode(struct bmp3_dev *dev)
{
    int8_t rslt;
    /* Used to select the settings user needs to change
    uint16_t settings_sel;

    /* Select the pressure and temperature sensor to be enabled
    dev->settings.press_en = BMP3_ENABLE;
    dev->settings.temp_en = BMP3_ENABLE;
    /* Select the output data rate and oversampling settings for pressure and temperature
    dev->settings.odr_filter.press_os = BMP3_NO_OVERSAMPLING;
    dev->settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
    dev->settings.odr_filter.odr = BMP3_ODR_200_HZ;
    /* Assign the settings which needs to be set in the sensor
    settings_sel = BMP3_PRESS_EN_SEL | BMP3_TEMP_EN_SEL | BMP3_PRESS_OS_SEL | BMP3_TEMP_OS_SEL | BMP3_ODR_SEL;
    rslt = bmp3_set_sensor_settings(settings_sel, dev);

    /* Set the power mode to normal mode
    dev->settings.op_mode = BMP3_NORMAL_MODE;
    rslt = bmp3_set_op_mode(dev);

    return rslt;
}

int8_t bmp388_get_sensor_data(struct bmp3_dev *dev)
{
    int8_t rslt;
    /* Variable used to select the sensor component
    uint8_t sensor_comp;
    /* Variable used to store the compensated data
    struct bmp3_data data;

    /* Sensor component selection
    sensor_comp = BMP3_PRESS | BMP3_TEMP;
    /* Temperature and Pressure data are read and stored in the bmp3_data instance
    rslt = bmp3_get_sensor_data(sensor_comp, &data, dev);

    /* Print the temperature and pressure data
    LOG_INFO("Temperature in deg celsius\t Pressure in Pascal\t\n");
	#ifdef BMP3_DOUBLE_PRECISION_COMPENSATION
    printf("%0.2f\t\t %0.2f\t\t\n",data.temperature, data.pressure);
	#else
	/* for fixed point the compensated temperature and pressure output has a multiplication factor of 100
    LOG_INFO("%lld\t\t %llu\t\t\n",data.temperature, data.pressure);
	#endif

    return rslt;
}
*/
int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev)
{
	int8_t rslt;
	uint8_t settings_sel;
	struct bme280_data comp_data;

	/* Recommended mode of operation: Indoor navigation */
	dev->settings.osr_h = BME280_OVERSAMPLING_1X;
	dev->settings.osr_p = BME280_OVERSAMPLING_16X;
	dev->settings.osr_t = BME280_OVERSAMPLING_2X;
	dev->settings.filter = BME280_FILTER_COEFF_16;
	dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, dev);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

	printf("Temperature, Pressure, Humidity\r\n");
	while (1) {
		/* Delay while the sensor completes a measurement */
		dev->delay_ms(70);
		rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
		print_sensor_data(&comp_data);
	}

	return rslt;
}

void print_sensor_data(struct bme280_data *comp_data)
{
#ifdef BME280_FLOAT_ENABLE
        printf("%0.2f, %0.2f, %0.2f\r\n",comp_data->temperature, comp_data->pressure, comp_data->humidity);
#else
        printf("%ld, %ld, %ld\r\n",comp_data->temperature, comp_data->pressure, comp_data->humidity);
#endif
}
