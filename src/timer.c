/*
 * timer.c
 *
 *  Created on: Sep 8, 2019
 *      Author: vkronber
 */

#include "timer.h"



void init_timer_interrupt(void)
{
	// Enable LFXO oscillator:
	/* 1 - Asserts that LXFO is not used for HF clock
	 * 2 - Sets LXFO config registers
	 * 3 - Enables LXFO oscillator
	 */
	CMU_LFXOInit_TypeDef lxfoinit = CMU_LFXOINIT_DEFAULT;	// Default values for LXFO config register
	CMU_LFXOInit(&lxfoinit);

	// Select LXFO oscillator for LFA clock branch
	// LFA w/
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	LETIMER_TypeDef letimer;
	LETIMER_Init_TypeDef init = LETIMER_LED_Init;
	// Initialize LETIMER w/ defaults - Set compare registers prior to initialization when starting timer from initialization
	LETIMER_Init(&letimer,&init);

}
