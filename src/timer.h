/*
 * timer.h
 * LETIMER0 intialization and control for EFR32BG13
 *
 *  Created on: Sep 8, 2019
 *      Author: Victor Kronberg
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include "main.h"
#include "em_letimer.h"

// LED timing in seconds
#define LEDOnTime			(175)	// Max = 1000 milliseconds
#define LEDBlinkPeriod		(2250)	// Max = 7000 milliseconds
#define TimerPeriod			(3000)	// Three second timer interval

#define MAX_COUNTER			(0xFFFF)
#define LFXO_FREQUENCY		(32768)
#define ULFRCO_FREQUENCY	(1000)

// Compare and Repeat registers
#define LETimerCOMP0	(0)
#define LETimerCOMP1	(1)
#define LETimerREP0		(0)
#define LETimerREP1		(1)

#define LETIMER_TEMP_INIT	                                                   		\
  {                                                                            		\
    false,              	/* Do not enable timer when init completes. */         	\
    false,             		/* Halt counter during debug. */                 		\
    false,             		/* Do not COMP0 into CNT on underflow. */  	           	\
    false,            		/* Do not COMP1 into COMP0 when REP0 reaches 0. */ 		\
    0,                		/* Idle value 0 for output 0. */                        \
    0,                		/* Idle value 0 for output 1. */                        \
    letimerUFOANone,   		/* No action on underflow on output 0. */               \
    letimerUFOANone,   		/* No action on underflow on output 1. */               \
	letimerRepeatOneshot,	/* Repeat Rep0 times, then load Rep1 into Rep0 */    	\
    0                  		/* Use default top Value. */                            \
  }

#define LETIMER_COMPARE_REG_VALUE_FROM_MS(time_in_ms,LFACLK_freq_in_Hz,LFA_Prescaler)	\
	(uint16_t)(((time_in_ms) * (LFACLK_freq_in_Hz)/(LFA_Prescaler))/1000)

#define LETIMER_COMPARE_REG_VALUE_FROM_US(time_in_us,LFACLK_freq_in_Hz,LFA_Prescaler)	\
	(uint16_t)(((time_in_us) * (LFACLK_freq_in_Hz)/(LFA_Prescaler))/1000000)

#define LETIMER_MS_FROM_REGISTER_TICS(register_value,LFACLK_freq_in_Hz,LFA_Prescaler)	\
	(uint32_t) ( (register_value) * ((1000 * LFA_Prescaler) / (LFACLK_freq_in_Hz)))

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

/**
 * @description:  Initializes LFXO oscillator
 */
void init_lfxo(void);
/**
 * @description:  Initializes LETIMER0 clock tree and required oscillator
 */
void init_letimer(void);

/**
 * [reset_periodic_timer]
 * @description: Resets periodic 3 sec interrupt timer
 */
void reset_periodic_timer(void);

/**
 * [delay_ms]
 * @description: Sets an interrupt-driven delay timer for a specified number of ms.
 *               Enables the interrupt based on an offset from the current counter.
 * @param        time_in_ms [Number of milliseconds for delay (unsigned int)]
 */
void delay_ms(uint32_t time_in_ms);

/**
 *
 * @description:  Enable LETIMER0 counter
 */
void enable_letimer(void);

/**
 * @description:  Disable LETIMER0 counter
 */
void disable_letimer(void);

/**
 * @description:  Clear LETIMER0 counter
 */
void clear_letimer(void);

/**
 * [calculate_timer]
 * @description: Calculates value for COMP registers in order to satisfy a timer
 *               of a specified number of milliseconds.  Calculates number of
 *               ticks based on oscillator frequency, prescaler, and current CNT.
 * @param        time_in_ms [Number of milliseconds to delay]
 * @return                  [Value for COMP register to trigger COMP IF]
 */
uint32_t calculate_timer(uint32_t time_in_ms);

/**
 * [timerGetRunTimeMilliseconds]
 * @description: Retrieves the number of milliseconds that have elapsed since
 *               the program began running. Overflows at 65536.
 * @return        [Number of milliseconds since program initiated]
 */
uint32_t timerGetRunTimeMilliseconds(void);

#endif /* SRC_TIMER_H_ */
