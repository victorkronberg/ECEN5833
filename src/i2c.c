/*
 * i2c.c
 *
 *  Created on: Sep 15, 2019
 *      Author: vkronber
 */


#include "i2c.h"

void i2c_init(void)
{
	I2CSPM_Init_TypeDef i2c_values = i2c_init_EFR32BG13;
	I2CSPM_Init(i2c_values);

}
