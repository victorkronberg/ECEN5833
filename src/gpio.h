/*
 * gpio.h
 * GPIO control for LED0 and LED1
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>

#define pin4	(4)
#define pin5	(5)

// GPIO related LCD display #defines
#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED 	(1)
#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED		(1)

/**
 * @description:  Initialize GPIO pins for LED0 and 1
 */
void gpioInit();

/**
 * @description:  Turn on LED0
 */
void gpioLed0SetOn();

/**
 * @description:  Turn off LED0
 */
void gpioLed0SetOff();

/**
 * @description:  Turn on LED1
 */
void gpioLed1SetOn();

/**
 * @description:  Turn off LED1
 */
void gpioLed1SetOff();

void gpioEnableDisplay(void);

void gpioSetDisplayExtcomin(bool high);

#endif /* SRC_GPIO_H_ */
