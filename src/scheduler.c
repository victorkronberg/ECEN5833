/*
 * scheduler.c
 *
 *  Created on: Sep 21, 2019
 *      Author: vkronber
 */


#include "scheduler.h"

float temperature;


void my_scheduler(myStateTypeDef *state_struct)
{
	switch (state_struct->current_state)
	{
		case STATE0_WAIT_FOR_TIMER:
			if( ((state_struct->event_bitmask & TIMER_EVENT_MASK) >> TIMER_EVENT_MASK_POS) == 1 )
			{
				// Clear event bitmask
				state_struct->event_bitmask &= ~TIMER_EVENT_MASK;

				// Reset period interrupt
				reset_periodic_timer();

				// Call I2C power-up
				enable_si7021_power();

				// Set timer for power-up delay
				delay_ms(POWER_UP_DELAY);

				// Set next state
				state_struct->next_state = STATE1_I2C_POWER_UP;

			}
			break;
		case STATE1_I2C_POWER_UP:
			if( ((state_struct->event_bitmask & DELAY_EVENT_MASK) >> DELAY_EVENT_MASK_POS) == 1 )
			{
				// Set deepest sleep mode for I2C write
				SLEEP_SleepBlockBegin(sleepEM2);
				// Clear event bitmask
				state_struct->event_bitmask &= ~DELAY_EVENT_MASK;

				// Start I2C write
				si7021_init_i2c_temp_write();
				// Set next state
				state_struct->next_state = STATE2_I2C_WRITE;
			}
			break;
		case STATE2_I2C_WRITE:
			if( ((state_struct->event_bitmask & I2C_EVENT_MASK) >> I2C_EVENT_MASK_POS) == 1 )
			{
				// Clear event bitmask
				state_struct->event_bitmask &= ~I2C_EVENT_MASK;

				// Handle I2C event - check for complete

				// What to do while we wait for write to complete? Do we need an if here?

				// Go to EM3 for conversion delay wait
				SLEEP_SleepBlockEnd(sleepEM2);
				delay_ms(CONVERSION_DELAY);
				// Set next state if complete
				state_struct->next_state = STATE3_I2C_WAIT;
			}
			break;
		case STATE3_I2C_WAIT:
			if( ((state_struct->event_bitmask & DELAY_EVENT_MASK) >> DELAY_EVENT_MASK_POS) == 1 )
			{
				// Conversion delay is complete, init I2C for read
				si7021_init_i2c_temp_read();
				// Set deepest sleep as EM1 for I2C read
				SLEEP_SleepBlockBegin(sleepEM2);
				state_struct->next_state = STATE4_I2C_READ;
			}
			break;
		case STATE4_I2C_READ:
			// If event flag set for read transfer complete, read temperature from return registers and calc temp
			if( ((state_struct->event_bitmask & I2C_EVENT_MASK) >> I2C_EVENT_MASK_POS) == 1 )
			{
				// Calculate temperature
				temperature = si7021_return_last_temp();

				LOG_INFO("Current temperature in degrees C: %f",temperature);

				disable_si7021_power();
				// Go back to deepest sleep to wait for next temperature reading
				SLEEP_SleepBlockEnd(sleepEM2);
				state_struct->next_state = STATE0_WAIT_FOR_TIMER;
			}
			break;
		default:
			break;
	}

	if(state_struct->current_state != state_struct->next_state)
	{
		LOG_INFO("State transitioned from state %d to %d",state_struct->current_state,state_struct->next_state);
		state_struct->current_state = state_struct->next_state;
	}

}
