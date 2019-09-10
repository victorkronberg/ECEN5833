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
#define LEDOnTime		(1)	// Max = 1 second
#define LEDBlinkPeriod	(7)	// Max = 7 seconds

#define LETIMER_LED_Init	                                                  \
  {                                                                            \
    true,              /* Enable timer when initialization completes. */       \
    false,             /* Stop counter during debug halt. */                   \
    false,             /* Do not load COMP0 into CNT on underflow. */          \
    false,             /* Do not load COMP1 into COMP0 when REP0 reaches 0. */ \
    0,                 /* Idle value 0 for output 0. */                        \
    0,                 /* Idle value 0 for output 1. */                        \
    letimerUFOANone,   /* No action on underflow on output 0. */               \
    letimerUFOANone,   /* No action on underflow on output 1. */               \
    letimerRepeatFree, /* Count until stopped by SW. */                        \
    0                  /* Use default top Value. */                            \
  }

void init_timer_interrupt(void);

#endif /* SRC_TIMER_H_ */
