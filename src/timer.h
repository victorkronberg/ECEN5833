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

// Compare and Repeat registers
#define LETimerCOMP0	(0)
#define LETimerCOMP1	(1)
#define LETimerREP0		(0)
#define LETimerREP1		(1)

#define LETIMER_LED_Init	                                                   		\
  {                                                                            		\
    false,              	/* Do not enable timer when init completes. */         	\
    false,             		/* Stop counter during debug halt. */                   \
    true,              		/* Load COMP0 into CNT on underflow. */  	           	\
    true,            		/* Load COMP1 into COMP0 when REP0 reaches 0. */ 		\
    0,                		/* Idle value 0 for output 0. */                        \
    0,                		/* Idle value 0 for output 1. */                        \
    letimerUFOANone,   		/* No action on underflow on output 0. */               \
    letimerUFOANone,   		/* No action on underflow on output 1. */               \
	letimerRepeatFree,		/* Repeat until stopped by SW */            			\
    0                  		/* Use default top Value. */                            \
  }

#define LETIMER_COMPARE_REG_VALUE_FROM_TIME(time_in_ms,LFACLK_freq_in_Hz,LFA_Prescaler)	\
	(uint16_t)(((time_in_ms) * (LFACLK_freq_in_Hz)/(LFA_Prescaler))/1000)

typedef struct {
	uint32_t 		led_status;			// LED on/off-status
	uint16_t		LED_on_time;		// Time (in sec) that LED is on
	uint16_t		LED_off_time;		// Time (in sec) of LED blink period
} LED_timer_TypeDef;

void init_timer_interrupt(void);
void calculate_led_timer(LED_timer_TypeDef *led_timer_struct);

#endif /* SRC_TIMER_H_ */
