/*
 * scheduler.c
 *
 *  Created on: Sep 21, 2019
 *      Author: vkronber
 */


#include "scheduler.h"


void scheduler(myStateTypeDef *state_struct)
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

				// Set next state
				state_struct->next_state = STATE1_I2C_POWER_UP;

				// Call I2C power-up and enable timer event
			}
			break;
		case STATE1_I2C_POWER_UP:
			break;
		case STATE2_I2C_WRITE:
			break;
		case STATE3_I2C_WAIT:
			break;
		case STATE4_I2C_READ:
			break;
		case STATE5_I2C_POWER_DOWN:
			break;
		default:
			break;
	}

	if(state_struct->current_state != state_struct->next_state)
	{
		LOG_DEBUG("State transitioned from state %d to %d",
				state_struct->current_state,state_struct->next_state);
		state_struct->current_state = state_struct->next_state;
	}

}
