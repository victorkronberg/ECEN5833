/*
 * timer.c
 *
 *  Created on: Sep 8, 2019
 *      Author: vkronber
 */

#include "timer.h"

Timer_TypeDef letimer_struct;


// Initializes oscillator and clock tree for letimer
void init_letimer(void)
{

	letimer_struct.osc_frequency = LFXO_FREQUENCY;
	letimer_struct.LFA_prescaler = cmuClkDiv_4;
	letimer_struct.period_in_ms = TimerPeriod - (CONVERSION_DELAY + POWER_UP_DELAY)/1000;

	// Calculate default value based on #define
	calculate_timer(&letimer_struct);

	// Enable osciallator for letimer
	init_lfxo();


	// Enables LFA
	CMU_ClockEnable(cmuClock_LFA,true);

#ifdef EnergyMode3
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);
#else
	// Select LXFO oscillator for LFA clock branch
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	// Divide LETIMER by 4 - 8192Hz
	CMU_ClockPrescSet(cmuClock_LETIMER0,letimer_struct.LFA_prescaler);
#endif

	// Enable LETIMER0
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	// Configure LETIMER0
	LETIMER_Init_TypeDef init = LETIMER_TEMP_INIT;

	// Initialize LETIMER w/ defaults - Set compare registers prior to initialization when starting timer from initialization
	LETIMER_Init(LETIMER0,&init);

	// Pre-load Compare registers
	LETIMER_CompareSet(LETIMER0,LETimerCOMP0,letimer_struct.timer_period);

	enable_letimer();

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

void init_timer_interrupt(void)
{
	// Stop and clear LETIMER
	disable_letimer();
	clear_letimer();

	// Pre-load Compare registers
	LETIMER_CompareSet(LETIMER0,LETimerCOMP0,letimer_struct.timer_period);

	// Enable LETIMER Interrupts on repeat
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF);
	NVIC_EnableIRQ(LETIMER0_IRQn);


	enable_letimer();

	return;

}


void reset_timer_interrupt(void)
{

	// Stop and clear LETIMER
	disable_letimer();
	clear_letimer();

	// Pre-load Compare registers
	LETIMER_CompareSet(LETIMER0,LETimerCOMP0,letimer_struct.timer_period);

	// Clear interrupt flags
	uint32_t flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	// Enable LETIMER Interrupts on repeat
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF);

	enable_letimer();

}


void delay_us(uint32_t time_in_us)
{
	float time_in_ms = time_in_us/1000;
	// Store previous timer period
	uint16_t old_timer = letimer_struct.timer_period;

	//Clear counter in case it is still running
	disable_letimer();
	clear_letimer();

	// Clear interrupt flags
	uint32_t flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	//Calculate required number of ticks
	letimer_struct.timer_period = LETIMER_COMPARE_REG_VALUE_FROM_MS(time_in_ms,letimer_struct.osc_frequency,letimer_struct.LFA_prescaler);

	//Reload compare register
	LETIMER_CompareSet(LETIMER0,LETimerCOMP0,letimer_struct.timer_period);

	enable_letimer();

	while( ! (LETIMER0->IF & (1 << _LETIMER_IFC_UF_SHIFT)) );

	//Clear counter
	disable_letimer();

	// Restore original timer period
	letimer_struct.timer_period = old_timer;

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
	LETIMER0->CMD = LETIMER_CMD_CLEAR;
}

void calculate_timer(Timer_TypeDef *timer_struct)
{

	// Number of ticks required = OnTime * clock frequency
	// Each register can hold 65536 ticks
	timer_struct->timer_period = LETIMER_COMPARE_REG_VALUE_FROM_MS(timer_struct->period_in_ms,timer_struct->osc_frequency,timer_struct->LFA_prescaler);

}

void LETIMER0_IRQHandler(void)
{

	LETIMER_IntDisable(LETIMER0,LETIMER_IEN_UF);

	// Acknowledge the interrupt
	uint32_t flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	//LETIMER_CompareSet(LETIMER0,LETimerCOMP0,letimer_struct.timer_period);

	// Set bit for timer
	event_bitmask |= 1;

	//LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF);

	return;

}

