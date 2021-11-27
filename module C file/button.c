/*
 * button.c
 *
 * Created: 2021-03-30 오후 3:47:58
 *  Author: kccistc
 */ 

#include "buzzer.h"
#include "button.h"

void button_Init()
{
	BUTTON_DDR &= ~( (1<<BUTTON1) | (1<<BUTTON2) | (1<<BUTTON3) );
}

uint8_t getButton1State_sonic()
{
	static uint8_t prevState = 1; //기존 값 유지
	uint8_t curState;
	curState = BUTTON_PIN &(1<<BUTTON1);
	if((prevState == 1)&&(curState == 0)) //처음 누른 상태
	{	
		
		_delay_ms(30); //debouncing 때문에 딜레이를 준다. 노이즈때문에, 두 번 바뀌는 경우가 있다.
		prevState = 0;
		return 0;
	}
	else if((prevState == 0)&&(curState != 0)) //버튼을 땠을때
	{	
		buzzer_on();
		_delay_ms(30);
		prevState = 1;
		return 1;
	}
	return 0;
}
uint8_t getButton1State()
{
	static uint8_t prevState = 1; //기존 값 유지
	uint8_t curState;
	
	curState = BUTTON_PIN &(1<<BUTTON1);
	if((prevState == 1)&&(curState == 0)) //처음 누른 상태
	{
		buzzer_on();
		_delay_ms(30); //debouncing 때문에 딜레이를 준다. 노이즈때문에, 두 번 바뀌는 경우가 있다.
		prevState = 0;
		return 0;
	}
	else if((prevState == 0)&&(curState != 0)) //버튼을 땠을때
	{
		_delay_ms(30);
		prevState = 1;
		return 1;
	}
	return 0;
}

uint8_t getButton2State()
{
	static uint8_t prevState = 1; //기존 값 유지
	uint8_t curState;
	curState = BUTTON_PIN &(1<<BUTTON2);
	if((prevState == 1)&&(curState == 0)) //처음 누른 상태
	{
		_delay_ms(30); //debouncing 때문에 딜레이를 준다. 노이즈때문에, 두 번 바뀌는 경우가 있다.
		prevState = 0;
		return 0;
	}
	else if((prevState == 0)&&(curState != 0)) //버튼을 땠을때
	{
		_delay_ms(30);
		prevState = 1;
		return 1;
	}
	return 0;
}


uint8_t getButton3State()
{
	static uint8_t prevState = 1; //기존 값 유지
	uint8_t curState;
	curState = BUTTON_PIN &(1<<BUTTON3);
	if((prevState == 1)&&(curState == 0)) //처음 누른 상태
	{
		_delay_ms(30); //debouncing 때문에 딜레이를 준다. 노이즈때문에, 두 번 바뀌는 경우가 있다.
		prevState = 0;
		return 0;
	}
	else if((prevState == 0)&&(curState != 0)) //버튼을 땠을때
	{
		_delay_ms(30);
		prevState = 1;
		return 1;
	}
	return 0;
}