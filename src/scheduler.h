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

/**
 * [my_scheduler]
 * @description: Interrupt driven scheduler which controls state machine. Handles
 * 							changes between states and calling functions relevant to state
 * 							operations.
 * @param        state_struct [Struct containing context for the state machine]
 */
void my_scheduler(myStateTypeDef *state_struct);

/**
 * [scheduler_power_up_si7021]
 * @description: Reset periodice timer, enable power to the si7021,
 * 							and set delay to wait for si7021 power-up. To be called in STATE0_WAIT_FOR_TIMER
 */
void scheduler_power_up_si7021(void);

/**
 * [scheduler_start_i2c_write]
 * @description: Set deepest sleep state to EM1 and begin I2C write. To be called
 * 				 			in STATE1_I2C_POWER_UP.
 */
void scheduler_start_i2c_write(void);

/**
 * [scheduler_wait_for_temp_conversion]
 * @description: Set deepest sleep state to EM3 and set delay timer to wait for conversion.
 * 							To be called in STATE2_I2C_WRITE.
 */
void scheduler_wait_for_temp_conversion(void);

/**
 * [scheduler_start_i2c_read]
 * @description: Set deepest sleep to EM1 and start I2C read. To be called in
 * 							STATE3_I2C_WAIT
 */
void scheduler_start_i2c_read(void);

/**
 * [scheduler_return_temp_then_wait]
 * @description: Retrieve temperature from I2C buffer, set deepest sleep to EM3,
 * 							and power down Si7021.  To be called in STATE4_I2C_READ.
 */
void scheduler_return_temp_then_wait(void);

#endif /* SRC_SCHEDULER_H_ */
