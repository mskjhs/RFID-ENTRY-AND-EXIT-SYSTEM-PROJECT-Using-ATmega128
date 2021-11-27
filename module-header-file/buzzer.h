/*
 * buzzer.h
 *
 * Created: 2021-09-13 오후 4:08:12
 *  Author: kccistc
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_
#define F_CPU 16000000UL //delay함수 사용할 때 필요
#include <avr/io.h>
#include <util/delay.h>

#define BUZZER_DDR DDRG
#define BUZZER_PORT PORTG
#define BUZZER1 PG3

void buzzer_init();
void buzzer_on();
void buzzer_off();
void buzzer_off_check();



#endif /* BUZZER_H_ */