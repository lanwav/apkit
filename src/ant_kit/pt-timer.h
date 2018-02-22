/*
 * pt-timer.h
 *
 * Created: 2011/8/26 11:09:01
 *  Author: Steven
 */ 

#ifndef CYG_TIMER_H_
#define CYG_TIMER_H_

/**
 * A timer.
 *
 * This structure is used for declaring a timer. The timer must be set
 * with timer_set() before it can be used.
 *
 * \hideinitializer
 */

#include "cyg_clock.h"

struct timer {
  clock_time_t start;
  clock_time_t interval;
};

void timer_set(struct timer *t, clock_time_t interval);
void timer_reset(struct timer *t);
void timer_restart(struct timer *t);
int timer_expired(struct timer *t);
clock_time_t timer_remaining(struct timer *t);

#endif /* CYG_TIMER_H_ */