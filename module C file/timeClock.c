/*
 * timeClock.c
 *
 * Created: 2021-04-01 오후 1:55:29
 *  Author: kccistc
 */ 

#include "timeClock.h"

TIME timeClock = {0,0,0};  //시간에 대한 속성값
	
void getTimeClock(TIME *Time) //매개변수를 포인터 변수로 하지 않으면 값이 반환되지 않음
{
	*Time = timeClock;
}

void setTimeClock(TIME Time)  //현재 시간을 셋팅하는 함수
{
	timeClock = Time;
}

void incTimeSec(void)
{
	timeClock.sec++;
	if(timeClock.sec >= 60)
	{
		timeClock.sec = 0;
		timeClock.min++;
		if(timeClock.min >= 60)
		{
			timeClock.min = 0;
			timeClock.hour++;
			if(timeClock.hour >= 24)
			{
				timeClock.hour = 0;
			}
		}
	}
}