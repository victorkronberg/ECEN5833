/*
 * timer.c
 *
 *  Created on: Sep 8, 2019
 *      Author: vkronber
 */

#include "timer.h"

LED_timer_TypeDef led_timer;



void init_timer_interrupt(void)
{
	gpioLed0SetOff();
	led_timer.led_status = 0;


	// Enable LFXO oscillator:
	/* 1 - Asserts that LXFO is not used for HF clock
	 * 2 - Sets LXFO config registers
	 * 3 - Does not disable if already enabled and running
	 */
	CMU_LFXOInit_TypeDef lxfoinit = CMU_LFXOINIT_DEFAULT;	// Default values for LXFO config register
	CMU_LFXOInit(&lxfoinit);

	// Enable LXFO - wait for oscillator to stabilize
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

	// Enables LFA
	CMU_ClockEnable(cmuClock_LFA,true);


	// Select LXFO oscillator for LFA clock branch
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	// Enable LETIMER0
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	// Divide LETIMER by 4 - 8192Hz
	CMU_ClockPrescSet(cmuClock_LETIMER0,cmuClkDiv_4);

	// Configure LETIMER0
	LETIMER_Init_TypeDef init = LETIMER_LED_Init;

	// Initialize LETIMER w/ defaults - Set compare registers prior to initialization when starting timer from initialization
	LETIMER_Init(LETIMER0,&init);

	calculate_led_timer(&led_timer);

	// Pre-load Compare registers
	LETIMER_CompareSet(LETIMER0,LETimerCOMP0,led_timer.LED_on_time);

	// Enable LETIMER Interrupts on repeat
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF);
	NVIC_EnableIRQ(LETIMER0_IRQn);

	// Enable LETIMER0
	LETIMER_Enable(LETIMER0, true);

}

void calculate_led_timer(LED_timer_TypeDef *led_timer_struct)
{

	led_timer_struct->LED_on_time = LETIMER_COMPARE_REG_VALUE_FROM_TIME(LEDOnTime,32768,4);
	// Number of ticks required = LEDOnTime * 32768
	// Each register can hold 65536 ticks
	led_timer_struct->LED_off_time = LETIMER_COMPARE_REG_VALUE_FROM_TIME((LEDBlinkPeriod - LEDOnTime),32768,4);

}

void LETIMER0_IRQHandler(void)
{
	LETIMER_IntDisable(LETIMER0,LETIMER_IEN_UF);

	 // Acknowledge the interrupt
	 uint32_t flags = LETIMER_IntGet(LETIMER0);
	 LETIMER_IntClear(LETIMER0, flags);

	//Toggle LED
	if(led_timer.led_status ==0)
	{
		gpioLed0SetOn();
		LETIMER_CompareSet(LETIMER0,LETimerCOMP0,led_timer.LED_off_time);
		led_timer.led_status = 1;
	}
	else
	{
		gpioLed0SetOff();
		LETIMER_CompareSet(LETIMER0,LETimerCOMP0,led_timer.LED_on_time);
		led_timer.led_status = 0;
	}

	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF);

	return;

}

