/*
 * scheduler.h
 *
 * Header file for scheduler
 *
 *  Created on: Sep 21, 2019
 *      Author: Victor Kronberg
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include "main.h"

#define ZERO_DELAY	(0)

typedef enum states  {
	STATE0_WAIT_FOR_BLE,
	STATE1_WAIT_FOR_TIMER,
	STATE2_I2C_POWER_UP,
	STATE3_I2C_WRITE,
	STATE4_I2C_WAIT,
	STATE5_I2C_READ,
	MY_NUM_STATES
} myState;

typedef struct	StateStruct {
	myState current_state;
	myState next_state;
	uint32_t event_bitmask;
	uint32_t periodic_timer0;
} myStateTypeDef;


// Global state structure
myStateTypeDef my_state_struct;

/**
 * [my_scheduler]
 * @description: Interrupt driven scheduler which controls server state machine. Handles
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

/**
 * [scheduler_enter_temperature_polling_loop]
 * @description:	Re-enters the temperature polling loop of the state machine.
 * 								To be called when a BLE connection event occurs.
 */
void scheduler_enter_temperature_polling_loop(void);

/**
 * [scheduler_exit_temperature_polling_loop]
 * @description:	Exits the temperature polling loop of the state machine.
 * 								To be called when a BLE connection is dropped.
 * @param        state_struct [Struct containing context for the state machine]
 */
void scheduler_exit_temperature_polling_loop(myStateTypeDef *state_struct);

/**
 * [scheduler_one_hz_event_handler]
 * @description: Event handler for 1Hz events. Updates TX power based on current
 * 							RSSI value and toggles EXTCOMIN on LCD to prevent charge buildup
 */
void scheduler_one_hz_event_handler(void);

/**
 * [scheduler_confirm_passkey]
 * @description:	Scheduler event to handle user confirmation of BLE pairing
 * 								passkey.  Clears bitmask and calls BLE function to confirm
 * 								passkey.
 * @param        state_struct [Struct containing context for the state machine]
 */
void scheduler_confirm_passkey(myStateTypeDef *state_struct);

/**
 * [scheduler_update_button_status]
 * @description:	Scheduler event to handle pressing or releasing of PB0.  Upon
 * 								change to button's state, status is sent as an indication to
 * 								bonded devices.
 * @param        state_struct [Struct containing context for the state machine]
 */
void scheduler_update_button_status(myStateTypeDef *state_struct);

/**
 * [client_scheduler description]
 * @description: Interrupt driven scheduler which controls client state machine. Handles
 * 							changes between states and calling functions relevant to state
 * 							operations.
 * @param        state_struct [Struct containing context for the state machine]
 */
void client_scheduler(myStateTypeDef *state_struct);

#endif /* SRC_SCHEDULER_H_ */
