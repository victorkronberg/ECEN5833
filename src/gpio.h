/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>

#define pin4	(4)
#define pin5	(5)

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
#endif /* SRC_GPIO_H_ */
