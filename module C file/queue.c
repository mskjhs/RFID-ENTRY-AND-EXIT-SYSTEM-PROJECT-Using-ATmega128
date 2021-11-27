/*
 * queue.c
 *
 * Created: 2021-09-23 오전 10:25:00
 *  Author: kccistc
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define  TRUE 1
#define  FALSE 0
#define  QUEUE_MAX 100

int front = -1; // read index
int rear = -1; // insert index
uint8_t queue[QUEUE_MAX];

int queue_full(void)
{
	// queue에서 rear+1 % QUEUE_MAX 값이 front와 같으면 queue full
	int tmp = (rear+1) % QUEUE_MAX;
	
	if(tmp == front) // front와 같으면 queue_full
		return TRUE;
	else return FALSE;
}

int queue_empty()
{
	if(rear == front) // rear와 같으면 queue_empty
		return TRUE;
	else return FALSE;	
}

uint8_t read_queue() // queue를 읽어오는 함수
{
	if(queue_empty())
		printf("Queue is empty !!!");
	else
	{
		front = (front+1) % QUEUE_MAX; // front의 초기값이 -1
//printf("%c \n", queue[front]);
		return (queue[front]);
	}

	
}

void queue_init() // queue가 텅 빈 경우, front 와 rear가 동일한 위치를 가리키는 경우
{
	int front = -1; // read index
	int rear = -1; // insert index
}

void insert_queue(uint8_t value)
{
	if(queue_full())
	{
		printf("Queue is full !!!\n");
		return;
	}
	else // save
	{
		rear = (rear+1) % QUEUE_MAX; // rear의 초기값이 -1
		queue[rear] = value;
	}
}

