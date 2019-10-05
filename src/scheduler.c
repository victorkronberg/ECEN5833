/*
 * scheduler.c
 *
 * State-machine for handling event-driven processes external to BLE
 *
 *  Created on: Sep 21, 2019
 *      Author: Victor Kronberg
 */


#include "scheduler.h"

void my_scheduler(myStateTypeDef *state_struct)
{

	// Handle events not tied to a given state first

	// Check for exit condition
	if( ((state_struct->event_bitmask & EXIT_EVENT_MASK) >> EXIT_EVENT_MASK_POS) == 1 )
	{
		scheduler_exit_temperature_polling_loop(state_struct);
	}

	// Check for RSSI check event - does not impact state
	if( ((state_struct->event_bitmask & CHECK_RSSI_EVENT_MASK) >> CHECK_RSSI_EVENT_MASK_POS) == 1 )
	{
		// Clear event bitmask
		state_struct->event_bitmask &= ~CHECK_RSSI_EVENT_MASK;

		gecko_ble_get_rssi();

		// Reset period interrupt
		reset_periodic_timer();

	}


	// Handle state-driven events

	switch (state_struct->current_state)
	{
		case STATE0_WAIT_FOR_BLE:
			if( ((state_struct->event_bitmask & BLE_EVENT_MASK) >> BLE_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~BLE_EVENT_MASK;
				__enable_irq();

				// Enter temperature polling loop (STATE1-5) if BLE connection has occurred
				scheduler_enter_temperature_polling_loop();

				// Set next state
				state_struct->next_state = STATE1_WAIT_FOR_TIMER;
			}
			break;
		case STATE1_WAIT_FOR_TIMER:
			if( ((state_struct->event_bitmask & TIMER_EVENT_MASK) >> TIMER_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~TIMER_EVENT_MASK;
				__enable_irq();

				// Enable an RSSI retrieval
				gecko_ble_get_rssi();

				// Reset timer, power up si7021, and set delay to wait for power-up
				scheduler_power_up_si7021();

				// Set next state
				state_struct->next_state = STATE2_I2C_POWER_UP;

			}
			break;
		case STATE2_I2C_POWER_UP:
			if( ((state_struct->event_bitmask & DELAY_EVENT_MASK) >> DELAY_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~DELAY_EVENT_MASK;
				__enable_irq();

				// Set deepest sleep state to EM1 and begin i2c write
				scheduler_start_i2c_write();

				// Set next state
				state_struct->next_state = STATE3_I2C_WRITE;
			}
			break;
		case STATE3_I2C_WRITE:
			if( ((state_struct->event_bitmask & I2C_EVENT_MASK) >> I2C_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~I2C_EVENT_MASK;
				__enable_irq();

				// Set deepest sleep state to EM3 and set delay timer to wait for conversion
				scheduler_wait_for_temp_conversion();

				// Set next state if complete
				state_struct->next_state = STATE4_I2C_WAIT;
			}
			break;
		case STATE4_I2C_WAIT:
			if( ((state_struct->event_bitmask & DELAY_EVENT_MASK) >> DELAY_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~DELAY_EVENT_MASK;
				__enable_irq();

				// Set deepest sleep to EM1 and start i2c read
				scheduler_start_i2c_read();

				// Set next state if complete
				state_struct->next_state = STATE5_I2C_READ;
			}
			break;
		case STATE5_I2C_READ:
			// If event flag set for read transfer complete, read temperature from return registers and calc temp
			if( ((state_struct->event_bitmask & I2C_EVENT_MASK) >> I2C_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~I2C_EVENT_MASK;
				__enable_irq();

				// Retrieve temperature from I2C buffer, set deepest sleep to EM3, and power down Si7021
				scheduler_return_temp_then_wait();

				state_struct->next_state = STATE1_WAIT_FOR_TIMER;
			}
			break;
		default:
			break;
	}

	if(state_struct->current_state != state_struct->next_state)
	{
		//LOG_INFO("State transitioned from state %d to %d",state_struct->current_state,state_struct->next_state);
		state_struct->current_state = state_struct->next_state;
	}

}

void scheduler_power_up_si7021(void)
{
	// Reset period interrupt
	reset_periodic_timer();

	// Call I2C power-up
	enable_si7021_power();

	// Set timer for power-up delay
	delay_ms(POWER_UP_DELAY);
}

void scheduler_start_i2c_write(void)
{
	// Set deepest sleep mode for I2C write
	SLEEP_SleepBlockBegin(sleepEM2);

	// Start I2C write
	si7021_init_i2c_temp_write();
}

void scheduler_wait_for_temp_conversion(void)
{
	// Go to EM3 for conversion delay wait
	SLEEP_SleepBlockEnd(sleepEM2);

	// Set delay to wait form Si7021 conversion
	delay_ms(CONVERSION_DELAY);
}

void scheduler_start_i2c_read(void)
{
	// Conversion delay is complete, init I2C for read
	si7021_init_i2c_temp_read();

	// Set deepest sleep as EM1 for I2C read
	SLEEP_SleepBlockBegin(sleepEM2);
}

void scheduler_return_temp_then_wait(void)
{
	float temperature;
	// Calculate temperature from last measured value
	temperature = si7021_return_last_temp();

	//LOG_INFO("Current temperature in degrees C: %f",temperature);

	// Power down Si7021
	disable_si7021_power();

	// Send temperature out BLE
	gecko_ble_send_temperature(temperature);

	// Go back to deepest sleep to wait for next temperature reading
	SLEEP_SleepBlockEnd(sleepEM2);
}

void scheduler_enter_temperature_polling_loop(void)
{
	// Reset period interrupt to occur every 3 seconds
	reset_periodic_timer();
}

void scheduler_exit_temperature_polling_loop(myStateTypeDef *state_struct)
{
	// Disable timer interrupts
	disable_timer_interrupts();

	__disable_irq();
	// Clear any pending external event bitmasks
	state_struct->event_bitmask = 0;
	__enable_irq();

	// Find out if sleep block needs to be removed
	SLEEP_EnergyMode_t deepest_sleep = SLEEP_LowestEnergyModeGet();

	// Remove block if needed
	if(deepest_sleep == sleepEM1)
	{
		SLEEP_SleepBlockEnd(sleepEM2);
	}

	// Set state to waiting for BLE connection
	state_struct->current_state = STATE0_WAIT_FOR_BLE;
	state_struct->next_state = STATE0_WAIT_FOR_BLE;
}
