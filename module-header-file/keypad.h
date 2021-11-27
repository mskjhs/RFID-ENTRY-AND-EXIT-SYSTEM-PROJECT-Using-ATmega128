/*
 * keypad.h
 *
 * Created: 2021-09-23 오전 11:40:16
 *  Author: kccistc
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define F_CPU 16000000UL
#include <avr/io.h>

void keypad_init();
uint8_t keypad_scan();
uint8_t get_button(int row, int col);



#endif /* KEYPAD_H_ */