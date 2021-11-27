/*
 * menu.h
 *
 * Created: 2021-08-17 오후 3:06:34
 *  Author: kcci
 */ 


#ifndef MENU_H_
#define MENU_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>   // interrupt 관련 lib가 들어 있다.
#include "uart0.h"
#include "ds1302.h"

void show_menu(void);
void menu_exec();
void updateDateTime();
uint8_t get_number();
void display_command_menu(void);
int digit_check(char *buff, int len);

#endif /* MENU_H_ */