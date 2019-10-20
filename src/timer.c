/*
 * timer.c
 * LETIMER0 intialization and control for EFR32BG13
 *
 *  Created on: Sep 8, 2019
 *      Author: Victor Kronberg
 */

#include "timer.h"

Timer_TypeDef letimer_struct;
extern myStateTypeDef my_state_struct;

uint32_t overflow_counter;

// Periodic counter used to distinguish between 1 and 3 second wakeups
uint32_t periodic_counter;


// Initializes oscillator and clock tree for letimer
void init_letimer(void)
{
	overflow_counter = 0;
	periodic_counter = 0;

	letimer_struct.period_in_ms = TimerPeriod;

	// Enables LFA
	CMU_ClockEnable(cmuClock_LFA,true);

	letimer_struct.osc_frequency = ULFRCO_FREQUENCY;
	letimer_struct.LFA_prescaler = cmuClkDiv_1;

	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);

	// Enable LETIMER0
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	// Configure LETIMER0
	LETIMER_Init_TypeDef init = LETIMER_TEMP_INIT;

	// Initialize LETIMER w/ defaults - Set compare registers prior to initialization when starting timer from initialization
	LETIMER_Init(LETIMER0,&init);

	// Load Repeat register with max => REP0IF set every (65,536*65,536)/osc_freq seconds
	// Approx every 50 days for ULFRCO
	// Add overflow handling mechanism if time
	LETIMER_RepeatSet(LETIMER0,LETimerREP0,MAX_COUNTER);

	NVIC_EnableIRQ(LETIMER0_IRQn);

	enable_letimer();

	reset_periodic_timer();

	// Disable timer-based interrupts until BLE connection occurs
	disable_timer_interrupts();

	return;
}

void init_lfxo(void)
{
	// Enable LFXO oscillator:
	/* 1 - Asserts that LXFO is not used for HF clock
	 * 2 - Sets LXFO config registers
	 * 3 - Does not disable if already enabled and running
	 */
	CMU_LFXOInit_TypeDef lxfoinit = CMU_LFXOINIT_DEFAULT;	// Default values for LXFO config register
	CMU_LFXOInit(&lxfoinit);

	// Enable LXFO - wait for oscillator to stabilize
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

	return;
}


void reset_periodic_timer(void)
{

	uint32_t timer_delta;

	__disable_irq();
	// Calculate timer value based on current CNT and time to next interrupt
	timer_delta = calculate_timer(TimerPeriod);

	// Pre-load Compare registers
	LETIMER_CompareSet(LETIMER0,LETimerCOMP1,timer_delta);

	// Ensure COMP1 interrupt flag is not set
	LETIMER_IntClear(LETIMER0, LETIMER_IEN_COMP1);

	// Enable LETIMER COMP1 interrupt
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP1);
	__enable_irq();

}

void disable_timer_interrupts(void)
{
	__disable_irq();
	// Clear any pending timer interrupt flags
	uint32_t flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	// Reset periodic counter
	periodic_counter = 0;

	// Disable interrupts
	LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP1);
	LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP0);
	__enable_irq();
}


void delay_ms(uint32_t time_in_ms)
{
	uint32_t timer_delta;

	__disable_irq();
	// Calculate timer value based on current CNT and time to next interrupt
	timer_delta = calculate_timer(time_in_ms);

	//Load compare register
	LETIMER_CompareSet(LETIMER0,LETimerCOMP0,timer_delta);

	// Clear COMP0 interrupt flag
	LETIMER_IntClear(LETIMER0, LETIMER_IEN_COMP0);

	// Enable LETIMER Interrupts on COMP0
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP0);
	__enable_irq();

	return;

}

void enable_letimer(void)
{
	// Enable LETIMER0
	LETIMER_Enable(LETIMER0, true);
}

void disable_letimer(void)
{
	// Enable LETIMER0
	LETIMER_Enable(LETIMER0, false);
}

void clear_letimer(void)
{
	// Clear LEMTIMER0
	LETIMER0->CMD = LETIMER_CMD_CLEAR;
}

uint32_t calculate_timer(uint32_t time_in_ms)
{

	//Calculate required number of ticks
	letimer_struct.timer_period = LETIMER_COMPARE_REG_VALUE_FROM_MS(time_in_ms,letimer_struct.osc_frequency,letimer_struct.LFA_prescaler);

	// Calculate required value for COMP0 = (current CNT) - timer_period
	return(LETIMER0->CNT - letimer_struct.timer_period);

}

uint32_t timerGetRunTimeMilliseconds(void)
{
	//float time_in_ms;
	uint32_t time_in_ms;

	// Time = timer ticks * 1000 * seconds/tick (prescaler/frequency)
	//time_in_ms = (65535 - LETIMER0->CNT) * ((1000 * letimer_struct.LFA_prescaler)/letimer_struct.osc_frequency);

	time_in_ms = LETIMER_MS_FROM_REGISTER_TICS((MAX_COUNTER - LETIMER0->CNT),letimer_struct.osc_frequency,letimer_struct.LFA_prescaler);

	time_in_ms += (overflow_counter * 65535);

	return	time_in_ms;
}

void LETIMER0_IRQHandler(void)
{
	// Disable interrupt nesting
	__disable_irq();

	// Acknowledge the interrupt and clear flags
	uint32_t flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	LOG_INFO("Timer interrupt thrown with flag %d and state %d",flags,my_state_struct.current_state);

#ifdef BUILD_INCLUDES_BLE_SERVER
	// Process interrupts
	// Check for COMP1 flag - periodic interrupt
	if(((flags & LETIMER_IF_COMP1) >> _LETIMER_IF_COMP1_SHIFT) == 1 )
	{
		// Increment periodic counter
		periodic_counter++;

		// Check if we are at three second interrupt
		if(periodic_counter >= 3)
		{
			// Set mask to handle periodic timer interrupt
			gecko_external_signal(TIMER_EVENT_MASK);
			// Disable COMP1 interrupt
			LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP1);

			// Reset periodic counter
			periodic_counter = 0;
		}
		else
		{
			// Set mask for 1 HZ event
			gecko_external_signal(ONE_HZ_EVENT_MASK);
		}

	}

	// Check for COMP0 flag - should only be set during delay states
	if(((flags & LETIMER_IF_COMP0) >> _LETIMER_IF_COMP0_SHIFT) == 1 )
	{
		if( (my_state_struct.current_state == STATE4_I2C_WAIT) || (my_state_struct.current_state == STATE2_I2C_POWER_UP) )
		{
			// Set bit for delay
			//my_state_struct.event_bitmask |= DELAY_EVENT_MASK;
			gecko_external_signal(DELAY_EVENT_MASK);
			// Disable COMP0 interrupt
			LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP0);
		}
		else
		{
			LOG_ERROR("Incorrect interrupt flag set.  COMP0 flagged during %d",my_state_struct.current_state);
		}
	}
#endif

#ifdef BUILD_INCLUDES_BLE_CLIENT
	// Process interrupts
	// Check for COMP1 flag - periodic interrupt
	if(((flags & LETIMER_IF_COMP1) >> _LETIMER_IF_COMP1_SHIFT) == 1 )
	{
		// Set mask for 1 HZ event
		gecko_external_signal(ONE_HZ_EVENT_MASK);
	}
#endif

	// Check for timer roll over
	if(((flags & LETIMER_IF_UF) >> _LETIMER_IF_UF_SHIFT) == 1 )
	{
		overflow_counter += 1;
	}

	__enable_irq();


	return;

}
