/*
 * servo_motor.h
 *
 * Created: 2021-09-23 오후 8:07:58
 *  Author: kim hyeong ju
 */ 


#ifndef SERVO_MOTOR_H_
#define SERVO_MOTOR_H_
#include <avr/io.h>
void timer3_init(void);
void svangle (float angle);
#endif /* SERVO_MOTOR_H_ */