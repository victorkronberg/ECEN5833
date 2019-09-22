/*
 * scheduler.h
 *
 *  Created on: Sep 21, 2019
 *      Author: vkronber
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include "main.h"

typedef enum states  {
	STATE0_WAIT_FOR_TIMER,
	STATE1_I2C_POWER_UP,
	STATE2_I2C_WRITE,
	STATE3_I2C_WAIT,
	STATE4_I2C_READ,
	MY_NUM_STATES
} myState;

typedef struct	{
	myState current_state;
	myState next_state;
	uint32_t event_bitmask;
	uint32_t periodic_timer0;
} myStateTypeDef;

void my_scheduler(myStateTypeDef *state_struct);

#endif /* SRC_SCHEDULER_H_ */
