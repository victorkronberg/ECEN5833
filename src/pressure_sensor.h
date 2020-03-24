/*
 * pressure_sensor.h
 *
 *  Created on: Mar 16, 2020
 *      Author: vkronber
 */

#ifndef SRC_PRESSURE_SENSOR_H_
#define SRC_PRESSURE_SENSOR_H_

#include "main.h"


int8_t bmp388_set_normal_mode(struct bmp3_dev *dev);

int8_t bmp388_get_sensor_data(struct bmp3_dev *dev);

int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev);

void print_sensor_data(struct bme280_data *comp_data);

#endif /* SRC_PRESSURE_SENSOR_H_ */
