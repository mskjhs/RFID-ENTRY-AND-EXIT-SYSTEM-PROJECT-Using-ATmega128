/*
 * uart0.c
 *
 * Created: 2021-08-17 오전 10:26:57
 *  Author: kcci
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>   // interrupt 관련 lib가 들어 있다. 
#include "uart0.h"

volatile uint8_t rxString[100] = {0};
volatile uint8_t rxReadyFlag = 0;
volatile int rxindex = 0;

// UART0로 부터 1byte가 들어 오면 이것을 자동 실행
// RX interrupt 서비스 루틴
ISR(USART0_RX_vect)
{
	volatile uint8_t data;
	
	data = UDR0;    // UART 하드웨어 버퍼 UDR0를 읽어서 data에 저장 
	
	if (data == '\n' || data == '\r')   // led1on\n
	{                                   // 
		rxString[rxindex] = '\0';
		rxindex=0;
		rxReadyFlag=1;      // 완전한 msg를 받았다는 신호를 알려준다. 
	}
	else
	{
		rxString[rxindex++] = data;   //  rxString[rxindex] = data;   rxindex++;
	}
}

uint8_t isRxString()
{
	return rxReadyFlag;
}

uint8_t *getRxString()
{
	rxReadyFlag=0;
	return rxString;
}
// uart0를 초기화 하는 함수
#define BAUD 9600
#define BAUD_9600  ( (F_CPU / (BAUD * 8)) -1)   // 207
void UART0_init(void)
{
	UBRR0H = 0x00;
	UBRR0L = 207;   // 9600으로 설정
	
	UCSR0A |= ( 1 << U2X0);   // 2배속 sampling속도를 8로 
	UCSR0C |= 0x06;   // 비동기, data길이: 8bit/nonpairty/1stopbit
	// UART0를 송신.수신이 가능 하도록 enable
	// RXEN0 : 수신이 가능 하도록 enable 
	// TXEN0  : 송신이 가능 하도록 
	// RXCIE0  : UART0로 부터 1byte가 들어 오면 interrupt가 뜨도록 설정 
	UCSR0B = ( (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0));
}
// UART0로 1byte를 전송하는 함수
void UART0_transmit(char data)
{
	while ( !(UCSR0A & (1 << UDRE0)))   // UDRE0의 bit가 1이 되는순간 exit
		;
	UDR0 = data;   // UDR(Uart Data Register)에 값을 넣는다. 
}
// UART0로 부터 1byte를 읽어 오는 함수
unsigned char UART0_receive(void)
{
	// 76543210
	// 1   ===> RXC0 bit (RX complet)
	while ( !(UCSR0A & (1 << RXC0)))   // RXC0의 bit가 1이 되는순간 exit
		;      // nop : no operation
	
	return UDR0;   // 수신된 1byte를 return 	
}
/*
*  UART0로 string(문자열: 끝은 \0)을 출력 하는 함수 
*/
void UART0_print_string(char *str)
{
	for (int i=0; str[i] != '\0'; i++)
	{
		UART0_transmit(str[i]);
	}
}