/*
 * timer.h
 *
 *  Created on: Sep 8, 2019
 *      Author: vkronber
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include "main.h"
#include "em_letimer.h"

// LED timing in seconds
#define LEDOnTime		(175)	// Max = 1000 milliseconds
#define LEDBlinkPeriod	(2250)	// Max = 7000 milliseconds
#define TimerPeriod		(3000)	// Three second timer interval

#define MAX_COUNTER		(0xFFFF)
#define LFXO_FREQUENCY	(32768)

// Compare and Repeat registers
#define LETimerCOMP0	(0)
#define LETimerCOMP1	(1)
#define LETimerREP0		(0)
#define LETimerREP1		(1)

#define LETIMER_TEMP_INIT	                                                   		\
  {                                                                            		\
    false,              	/* Do not enable timer when init completes. */         	\
    true,             		/* Do not halt counter during debug. */                 \
    true,              		/* Load COMP0 into CNT on underflow. */  	           	\
    true,            		/* Load COMP1 into COMP0 when REP0 reaches 0. */ 		\
    0,                		/* Idle value 0 for output 0. */                        \
    0,                		/* Idle value 0 for output 1. */                        \
    letimerUFOANone,   		/* No action on underflow on output 0. */               \
    letimerUFOANone,   		/* No action on underflow on output 1. */               \
	letimerRepeatFree,		/* Repeat until stopped by SW */            			\
    0                  		/* Use default top Value. */                            \
  }

#define LETIMER_COMPARE_REG_VALUE_FROM_MS(time_in_ms,LFACLK_freq_in_Hz,LFA_Prescaler)	\
	(uint16_t)(((time_in_ms) * (LFACLK_freq_in_Hz)/(LFA_Prescaler))/1000)

#define LETIMER_COMPARE_REG_VALUE_FROM_US(time_in_us,LFACLK_freq_in_Hz,LFA_Prescaler)	\
	(uint16_t)(((time_in_us) * (LFACLK_freq_in_Hz)/(LFA_Prescaler))/1000000)
/*
typedef struct {
	uint32_t 		led_status;			// LED on/off-status
	uint32_t		clock_frequency;	// Frequency of LETIMER clock
	uint32_t		LFA_prescaler;		//
	uint16_t		LED_on_time;		// Time (in sec) that LED is on
	uint16_t		LED_off_time;		// Time (in sec) of LED blink period
} LED_timer_TypeDef;
*/

typedef struct {
	uint32_t 		led_status;			// LED on/off-status
	uint32_t		osc_frequency;		// Frequency of oscillator
	uint32_t		LFA_prescaler;		// LFA clock prescaler
	uint32_t		period_in_ms;		// Time in ms
	uint16_t		timer_period;		// Value for letimer compare register
} Timer_TypeDef;


void init_lfxo(void);
void init_ulfrco(void);
void init_letimer(void);
void init_timer_interrupt(void);
void reset_timer_interrupt(void);
void delay_us(uint32_t time_in_us);
void enable_letimer(void);
void disable_letimer(void);
void clear_letimer(void);
void calculate_timer(Timer_TypeDef *timer_struct);

#endif /* SRC_TIMER_H_ */
