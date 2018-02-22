/*
 * pt-timer.c
 *
 * Created: 2011/8/26 11:08:49
* Copyright (C) 2012-2014 Jiaxing Lanwan Electronics co ., ltd . 
 *  Author: Steven
 */ 

#include "cyg_config.h"

#include "pt-timer.h"

/*---------------------------------------------------------------------------*/
/**
 * Set a timer.
 *
 * This function is used to set a timer for a time sometime in the
 * future. The function timer_expired() will evaluate to true after
 * the timer has expired.
 *
 * \param t A pointer to the timer
 * \param interval The interval before the timer expires.
 *
 */
void
timer_set(struct timer *t, clock_time_t interval)
{
  t->interval = interval;
  t->start = cygClockTime();
}
/*---------------------------------------------------------------------------*/
/**
 * Reset the timer with the same interval.
 *
 * This function resets the timer with the same interval that was
 * given to the timer_set() function. The start point of the interval
 * is the exact time that the timer last expired. Therefore, this
 * function will cause the timer to be stable over time, unlike the
 * timer_restart() function.
 *
 * \param t A pointer to the timer.
 *
 * \sa timer_restart()
 */
void
timer_reset(struct timer *t)
{
  t->start += t->interval;
}
/*---------------------------------------------------------------------------*/
/**
 * Restart the timer from the current point in time
 *
 * This function restarts a timer with the same interval that was
 * given to the timer_set() function. The timer will start at the
 * current time.
 *
 * \note A periodic timer will drift if this function is used to reset
 * it. For preioric timers, use the timer_reset() function instead.
 *
 * \param t A pointer to the timer.
 *
 * \sa timer_reset()
 */
void
timer_restart(struct timer *t)
{
  t->start = cygClockTime();
}
/*---------------------------------------------------------------------------*/
/**
 * Check if a timer has expired.
 *
 * This function tests if a timer has expired and returns true or
 * false depending on its status.
 *
 * \param t A pointer to the timer
 *
 * \return Non-zero if the timer has expired, zero otherwise.
 *
 */
int
timer_expired(struct timer *t)
{
  /* Note: Can not return diff >= t->interval so we add 1 to diff and return
     t->interval < diff - required to avoid an internal error in mspgcc. */    
  clock_time_t diff = (cygClockTime() - t->start) + 1;
  return t->interval < diff;
}
/*---------------------------------------------------------------------------*/
/**
 * The time until the timer expires
 *
 * This function returns the time until the timer expires.
 *
 * \param t A pointer to the timer
 *
 * \return The time until the timer expires
 *
 */
clock_time_t
timer_remaining(struct timer *t)
{
  return t->start + t->interval - cygClockTime();
}
/*---------------------------------------------------------------------------*/



