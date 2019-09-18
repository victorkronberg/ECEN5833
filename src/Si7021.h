/*
 * Si7021.h
 *
 *  Created on: Sep 14, 2019
 *      Author: vkronber
 */

#ifndef SRC_SI7021_H_
#define SRC_SI7021_H_

#include "main.h"

void init_si7021(void);
void enable_si7021_power(void);
void disable_si7021_power(void);
float read_temp(void);

#endif /* SRC_SI7021_H_ */
