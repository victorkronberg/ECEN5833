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


// Initializes oscillator and clock tree for letimer
void init_letimer(void)
{
	overflow_counter = 0;

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

	// Calculate timer value based on current CNT and time to next interrupt
	timer_delta = calculate_timer(TimerPeriod);

	// Pre-load Compare registers
	LETIMER_CompareSet(LETIMER0,LETimerCOMP1,timer_delta);

	// Ensure COMP1 interrupt flag is not set
	LETIMER_IntClear(LETIMER0, LETIMER_IEN_COMP1);

	// Enable LETIMER COMP1 interrupt
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP1);

}

void disable_timer_interrupts(void)
{
	// Clear any pending timer interrupt flags
	uint32_t flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	// Disable interrupts
	LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP1);
	LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP0);
}


void delay_ms(uint32_t time_in_ms)
{
	uint32_t timer_delta;

	// Calculate timer value based on current CNT and time to next interrupt
	timer_delta = calculate_timer(time_in_ms);

	//Load compare register
	LETIMER_CompareSet(LETIMER0,LETimerCOMP0,timer_delta);

	// Clear COMP0 interrupt flag
	LETIMER_IntClear(LETIMER0, LETIMER_IEN_COMP0);

	// Enable LETIMER Interrupts on COMP0
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP0);

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
	__disable_irq();

	// Acknowledge the interrupt and clear flags
	uint32_t flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	LOG_INFO("Timer interrupt thrown with flag %d and state %d",flags,my_state_struct.current_state);

	// Process interrupts
	// Check for COMP1 flag - periodic interrupt
	if(((flags & LETIMER_IF_COMP1) >> _LETIMER_IF_COMP1_SHIFT) == 1 )
	{
		// Set bit for timer
		my_state_struct.event_bitmask |= TIMER_EVENT_MASK;
		gecko_external_signal(TIMER_EVENT_MASK);
		// Disable COMP1 interrupt
		LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP1);

	}

	// Check for COMP0 flag - should only be set during delay states
	if(((flags & LETIMER_IF_COMP0) >> _LETIMER_IF_COMP0_SHIFT) == 1 )
	{
		if( (my_state_struct.current_state == STATE4_I2C_WAIT) || (my_state_struct.current_state == STATE2_I2C_POWER_UP) )
		{
			// Set bit for delay
			my_state_struct.event_bitmask |= DELAY_EVENT_MASK;
			gecko_external_signal(DELAY_EVENT_MASK);
			// Disable COMP0 interrupt
			LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP0);
		}
		else
		{
			LOG_ERROR("Incorrect interrupt flag set.  COMP0 flagged during %d",my_state_struct.current_state);
		}
	}

	// Check for timer roll over
	if(((flags & LETIMER_IF_UF) >> _LETIMER_IF_UF_SHIFT) == 1 )
	{
		overflow_counter += 1;
	}

	__enable_irq();


	return;

}
