/*
 * button.h
 *
 * Created: 2021-03-30 오후 3:48:13
 *  Author: kccistc
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#define F_CPU 16000000UL //delay함수 사용할 때 필요
#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_DDR  DDRD
#define BUTTON_PIN  PIND
#define BUTTON1   PD4
#define BUTTON2   PD5
#define BUTTON3   PD6

void button_Init();
uint8_t getButton1State();
uint8_t getButton2State();
uint8_t getButton3State();



#endif /* BUTTON_H_ */