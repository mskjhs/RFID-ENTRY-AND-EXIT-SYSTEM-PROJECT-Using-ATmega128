/*
* servo_motor.c
*
* Created: 2021-09-23 오후 8:07:33
*  Author: kim hyeong ju
*/

#include "servo_motor.h"
volatile uint8_t sever_motor_start_flag = 0; 
volatile int motor_timer = 0;
volatile uint8_t display_dalay_start_flag = 0;
volatile int display_dalay_timer = 0;
void timer3_init(void)
{
	// 고속 PWM 14
	TCCR3A |=(1 << WGM30);
	TCCR3A |=(1 << WGM31);
	TCCR3B |=(1 << WGM32);
	TCCR3B |=(1 << WGM32);
	
	// 비반전 모드 TOP : 0xff 비교 일치 값 : OCR3B : PE4
	TCCR3A |=(1<<COM3B1);
	TCCR3B |= (1<<CS31)|(1<<CS30); // 분주비 64
	// 	// 비반전 모드	 비교일치값 : OCR3B 레지스터.	 PE4

	// 분주율 8 : 2MHZ
	//T=1/f = 1/2000000HZ =0.0000005 sec
	// 16bit counter 가지고 측정 할 수 있는 펄스
	// 0.0000005 sec x 65535 =0.32765 sec
	// 0.0000005 sec x 40000 개 = 0.2sec
	//20ms PWM
	//	ICR1 =40000-1; // 20ms 주기 39999
	DDRE |= (1 <<4);
	OCR3B =0;
}

void svangle (float angle) // 각도를 입력하면 해당 각도 만큼 움직인다

{

	unsigned int pulse;
	sever_motor_start_flag = 1;
	display_dalay_start_flag = 1; 
	motor_timer =0;
	display_dalay_timer = 0;
	
	pulse = angle*225/90 +375 +0.5;

	OCR3B = pulse;

}
void svangle_reset()
{
	float angle =0;
	unsigned int pulse;
	sever_motor_start_flag = 0;
	display_dalay_start_flag = 0; 
	motor_timer =0;
	display_dalay_timer = 0;

	pulse = angle*225/90 +375 +0.5;

	OCR3B = pulse;
}

void servo_motor_check()
{
	if(motor_timer > 2000)
		svangle_reset();
		
}


//EXAMPLE
/*

int main(void)
{
	timer3_init();
	OCR3B=0;
	while (1)
	{

	svangle(90);
	_delay_ms(1000);
	svangle(0);
	_delay_ms(1000);

	}
}
*/