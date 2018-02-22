/*
 * cyg_clock.h
 *
 * Created: 2011/8/26 11:09:01
 *  Author: Steven
 */ 


#ifndef CYG_CLOCK_H_
#define CYG_CLOCK_H_

typedef uint32_t clock_time_t;

void cygOpenClock(void);
void cygCloseClock(void);
clock_time_t cygClockTime(void);

#endif /* CYG_CLOCK_H_ */