/*
 * timeClock.h
 *
 * Created: 2021-04-01 오후 1:55:42
 *  Author: kccistc
 */ 


#ifndef TIMECLOCK_H_
#define TIMECLOCK_H_

#include <avr/io.h>

typedef struct _time //사용자 정의형
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
}TIME;


void getTimeClock(TIME *Time);
void setTimeClock(TIME Time);
void incTimeSec(void);

#endif /* TIMECLOCK_H_ */