/*
 * CARD_READ.c
 *
 * Created: 2021-09-24 오후 2:10:45
 *  Author: kccistc
 */ 
#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "CARD_READ.h"

void card_read_prog()
{
byte = mfrc522_request(PICC_REQALL, str); // 카드의 tagging 유무 확인

if(byte == CARD_FOUND)
{
	byte = mfrc522_get_card_serial(str);
	for (int i = 0; i < 5; i++)
	printf("%02x ",str[i]);      //0x20 0x40 0x20 0x10 0x50
	printf("\n");
	
	valid_card = 0;
	for(int i = 0; i < 10; i++)
	{
		if(strncmp(str,card_record_table[i],5) == 0)
		{
			valid_card = TRUE;
			printf("valid card !!\n");
			svangle(90);
			
			
			break;
		}
	}
	if(!valid_card)        //valid == 0
	{
		printf("invalid card!!\n");
		svangle(0);
		
	}

}
}