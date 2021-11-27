/*
 * keypadcode.c
 *
 * Created: 2021-09-24 오전 11:16:02
 *  Author: kccistc
 */ 
/*#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>   // interrupt 관련 lib가 들어 있다.
#include "I2C_LCD.h"
#include "I2C.h"
#include "uart0.h"
#include "queue.h"

#define FIRST  0
#define MIDDLE 1
#define LAST   2
uint16_t mode = 0;
int index=0;
char buff[40];
extern void UART0_transmit(char data);
// printf를 동작 시키위한 mapping작업
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile uint32_t keypad_timer=0;

ISR(TIMER0_COMP_vect)
{
	volatile uint8_t keydata=0;
	
	keypad_timer++;
	if (keypad_timer >= 60)  // 60ms reached
	{
		keypad_timer=0;

		if ((keydata=keypad_scan()))
		{
			insert_queue(keydata);
		}
	}
}

void timer0_init()
{
	TCCR0 |= (1 << CS02) | (0 << CS01) | (0 << CS00);   // 64분주
	TCCR0 |= (1 << WGM01) | (0 << WGM00); // CTC mode
	TIMSK |= ( 1<< OCIE0);   // 비교 일치
	OCR0 = 250;  // 1ms 간격으로  INT 발생
}
int main(void)
{
	uint8_t key_value;
	
	timer0_init();
	I2C_LCD_init();
	stdout = &OUTPUT;
	UART0_init();
	keypad_int();
	
	sei();    // grobal int enb
	
	printf("main start !!!!\n");
	while (1)
	{
		if (queue_empty() != TRUE)
		{
			key_value=read_queue();
			I2C_LCD_goto_XY(0,index);
			I2C_LCD_write_data(key_value);
			buff[index++]=key_value;
			
			if (key_value == '=')
			{
				calculator(buff);
				
			}
			if (key_value == ' ')
			{
				clear_buff();
			}
		}

		//		I2C_LCD_write_string_XY(0, 0, buff);
	}
}

// 112+10=
void calculator(char *buff)
{
	char num1buff[10];
	char num2buff[10];
	char calculator_mode=' ';
	
	int num1, num2, result;
	int i,j;
	
	num1=num2=result=0;
	
	for (i=0; buff[i] >= '0' && buff[i] <= '9'; i++)
	num1buff[i] =  buff[i];
	
	num1buff[i]='\0';
	calculator_mode=buff[i];
	i++;
	
	for (j=0; buff[i] >= '0' && buff[i] <= '9'; i++, j++)
	num2buff[j] =  buff[i];
	num2buff[j]='\0';
	
	num1=atoi(num1buff);
	num2=atoi(num2buff);
	
	switch(calculator_mode) {
		case '+':
		result = num1 + num2;
		break;

		case '-':
		result = num1 - num2;
		break;
		
		case '*':
		result = num1 * num2;
		break;
		
		case '/':
		result = num1 / num2;
		break;
		
		default:
		break;
	}
	sprintf(buff, "%d%c%d=%d      ",num1,calculator_mode,num2,result);
	I2C_LCD_write_string_XY(0, 0, buff);
	printf("%s\n", buff);
}

void clear_buff()
{
	for (int i=0; i < 40; i++)
	{buff[i] = ' ';}
	I2C_LCD_clear();
	index=0;
}
*/