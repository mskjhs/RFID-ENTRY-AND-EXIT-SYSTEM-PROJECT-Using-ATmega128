/*
 * keypad.c
 *
 * Created: 2021-09-23 오전 11:40:05
 *  Author: kccistc
 */ 

#include "keypad.h"
#define BUZZER1 PG3
#define KEYPAD_DDR DDRA
#define KEYPAD_PIN PINA
#define KEYPAD_PORT PORTA

void keypad_init()
{
	KEYPAD_DDR=0x0f;
	KEYPAD_PORT=0xff;
}

uint8_t keypad_scan()
{
	uint8_t data = 0; // key 값 저장
	
	for(int row = 0; row < 4; row++)
	{
		for(int col = 0; col < 4; col++)
		{
			data = get_button(row, col);
			
			if(data)
				return data;
		}
	}
}

uint8_t get_button(int row, int col)
{
	uint8_t keypad_char[4][4] = { // 초기 변수를 설정해준다면 배열의 크기를 지정 안해줘도 되긴한다.
		{'/', '*', '-', '+'}, 	  // 초기 변수를 설정을 안한다면 배열 크기를 지정해줘야 한다.
		{'9', '6', '3', '='},
		{'8', '5', '2', '0'},
		{'7', '4', '1', ' '},
	};
		
	static int8_t prev_state[4][4]= // static으로 함수를 빠져나가도 값이 변한 그대로 저장되어 있다. 
	{
		{1, 1, 1, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1},			
	};
	
	int8_t current_state = 1; // 버튼이 눌리지 않은 상태로 시작.
	
	KEYPAD_PORT=0xff;
	KEYPAD_PORT &= ~(1 << col); // 해당되는 col에 전류를 흘린다. 해당되는 col을 low로 떨어트려 전류가 흐를 수 있게 한다.
	
	for(int delay =0; delay < 20; delay++)
			;
	
	current_state = ((KEYPAD_PIN & (1 << (row+4))) >> (row+4));
	// ex) row 0 : PA4에 연결, row 0 + 4
	// 76543210
	// 11101111 : KEYPAD_PIN & (1 << (row+4)
	// 00001110 :  >> (row+4)
	
	if(current_state == 0 && prev_state[col][row] == 1) 
	{		// 처음 버튼이 눌린 상태
		prev_state[col][row] = current_state;
		PORTG |= 1<<BUZZER1;  
		return 0;
	}
	else if(current_state == 1 && prev_state[col][row] == 0)
	{		// 이전에 버튼이 눌러졌고 현재는 버튼을 뗀 상태이면 버튼을 누른 상태로 인정
		prev_state[col][row] = 1; // key status tabel을 release  상태로 바꾼다.
//printf("key : %c\n", keypad_char[col][row]);		
		PORTG &= ~(1 << BUZZER1);
		return (keypad_char[col][row]);
		
	}
	
	
	return 0;
}