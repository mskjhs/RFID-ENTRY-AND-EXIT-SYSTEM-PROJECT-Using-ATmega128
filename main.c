/*
 * 15.KEYPAD_CALCULATOR.c
 *
 * Created: 2021-09-23 오전 9:49:59
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>   // interrupt 관련 lib가 들어 있다.

#include "myutils.h" // Add for RFID
#include "mfrc522.h" // for RFID
#include "spi.h"	 // for RFID
#include "queue.h"
#include "uart0.h"
#include "keypad.h"
#include "servo_motor.h"
#include "buzzer.h"
#include "DS1302.h"
#include "menu.h"
#include "timeClock.h"
#include "button.h"

#define MENU 0
#define DOOR_ROCK 1
#define CAL 2
#define CARD_SCAN 3
#define KEY_PASSWORD 4
#define WATCH 5
#define CARD_REG 6
#define DELET_CARD 7
#define CARD_CHECK 8
uint16_t mode = 0;
uint16_t mode_not = 0;
uint16_t mode_not1 = 0;
uint16_t mode_not2 = 0;
extern void UART0_transmit(char data);
// printf를 동작 시키기 위한 mapping 작업
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);
void password(char *passwo);
void calculator(char *cal);
void clear_buff();
void clear_password();
void display_dalay_check();
void card_scan_ch();
void card_check_display();
void card_delete_display();
uint8_t delay_counter_1;
uint8_t key_value_menu;
uint8_t key_value_door;
uint8_t key_value_cal;
uint8_t key_value_password;
uint8_t valid_card;
uint8_t byte;
uint8_t str[MAX_LEN];
uint8_t buff_str[MAX_LEN];
uint8_t counter_flag =0;
uint8_t counter = 0;
uint8_t option = 0;

extern volatile uint8_t display_dalay_start_flag;
extern volatile int display_dalay_timer;

extern volatile int buzzer_timer;         // 부저 동작시간 200~300ms
extern volatile uint8_t buzzer_start_flag;  //button을 눌렀을때 1로 set, buzzer timeout시 0 set

extern volatile uint8_t sever_motor_start_flag;
extern volatile int motor_timer;

uint8_t delay_counter = 0;
uint8_t card_record_table[10][5] =    //현재는 하드코딩이지만 자동으로 등록하고/ 삭제 할수 있도록 삭제
{
	//{0x06,0x5a, 0xec, 0x1b, 0xab},
	{0},	
	
};
uint8_t card_record_table_copy[10][5] =    //현재는 하드코딩이지만 자동으로 등록하고/ 삭제 할수 있도록 삭제
{
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},
	{0,0, 0, 0, 0},					
	//{0},
	
};

volatile uint32_t keypad_timer=0;// ISR내부에서 쓰이는 변수는 최적화를 방지하기 위해 volatile을 붙여준다.
ISR(TIMER0_COMP_vect)
{
	volatile uint8_t keydata = 0;
	keypad_timer++;
	
	if(keypad_timer >= 60) // 60ms reached
	{
		keypad_timer = 0;
		if(keydata = keypad_scan())
		{
			insert_queue(keydata);
		}
	}
	if(display_dalay_start_flag)
	{
		display_dalay_timer++;
	}
	if (buzzer_start_flag)
	{
		buzzer_timer++;
	}
	if(sever_motor_start_flag)
	{
		motor_timer++;
	}
	if(counter_flag)
	{
		counter++;
	}
}

void timer0_init()
{
	TCCR0 |= (1 << CS02) | (0 << CS01) | (0 << CS00); // 64분주
	TCCR0 |= (1 << WGM01) | (0 << WGM00); // CTC mode, 사용자가 지정해 둔 타이머 카운터 값이 도달 했을 때 자동으로 reset 해준다.
	TIMSK |= (1 << OCIE0); // 비교 일치
	OCR0 |= 250; // 1ms 마다 interrupt 발생
}
char sbuff[40];
char buff[40];
char buff_reg[40];
char buff_password[4];
char menu[40];
char cal[40];
char door[40];
char passwo[40];

int index_menu = 0;
int index_cal = 0;
int index_door = 0;
int index_passwo =0;
int index =0;
int card_list = 0;
int main(void)
{
	
	TIME myTime = {0, 0, 0};
	mode =WATCH;
	DS1302_Init();	
	spi_init();		// for RFID
	mfrc522_init(); // for RFID
	timer0_init();
	I2C_LCD_init();
	stdout = &OUTPUT;
	UART0_init();
    keypad_init();
	queue_init();
	timer3_init();
	buzzer_init();
	display_command_menu();
	setTimeClock(myTime);
#if 0
     printf("mfrc522_init !!\n ");	
	_delay_ms(1500); // 안정화 시간
	byte = 0x55;	 // 01010101 메모리가 제대로 Access되는지 확인하기 위해 초기값 비트가 변화함을 보기 위함.
	//byte = 0xAA;	 // 10101010
	// check version of RFID reader
	byte = mfrc522_read(VersionReg); // version을 읽고 return
    printf("ver --> %0x\n", byte);
	rfid_version_check();
#endif	
	sei(); // global interrupt enable

	printf("main start !!!\n");
	
	while (1)
	{
	mode_not = !mode_not;	
	mode_not1 = !mode_not1; 	
	mode_not2 = !mode_not2;	

		
			
			switch(mode)
			{
	//===============================
	//=========MENU MODE============
	//===============================
				case MENU:
				I2C_LCD_write_string_XY(0,0, "SELECT : 1.WATCH");
				I2C_LCD_write_string_XY(1,0, "2.CAL 3.DOOR");
				if (queue_empty() != TRUE)
				{
					key_value_menu=read_queue();
					menu[index_menu++]=key_value_menu;
					if(key_value_menu == '1')
					{
						I2C_LCD_clear();
						mode=WATCH;
					}
			    	else if(key_value_menu == '2')
						{
						I2C_LCD_clear();
						mode =CAL;
						} 
				    else if (key_value_menu == '3')
						{
						I2C_LCD_clear();
						mode = DOOR_ROCK;
						}
				}
				if(getButton3State())
				{
					I2C_LCD_clear();
					mode = CARD_CHECK;
				}
				
				if (getButton2State())
				{
						I2C_LCD_clear();
						mode = CARD_REG;
					}
					
				
				break;
	//===============================
	//=========WATCH MODE============
	//===============================		
				case WATCH:
				getTimeClock(&myTime);
				menu_exec();
				display_clock_lcd();
				if (queue_empty() != TRUE)
				{
					key_value_menu=read_queue();
					menu[index_menu++]=key_value_menu;
					if(key_value_menu == ' ')
					{
						I2C_LCD_clear();
						mode=MENU;
					}
				}
				break;		
	//===============================					
	//=========DOOR ROCK MODE========
	//===============================	
				case DOOR_ROCK:
				I2C_LCD_write_string_XY(0,0, "Use CARD or PAD ");
				I2C_LCD_write_string_XY(1,0, "1.CARD 2.PAD");
				if (queue_empty() != TRUE)
				{
					key_value_door=read_queue();
					door[index_door++]=key_value_door;
					if (key_value_door =='1')
					{
					I2C_LCD_clear();
					mode = CARD_SCAN;
					}
					else if (key_value_door =='2')
					{
					
					I2C_LCD_clear();
					mode =KEY_PASSWORD;
					}
					if (key_value_door ==' ')
					{
						I2C_LCD_clear();
						mode = MENU;
					}
				}
				break;
	//===============================					
	//=========CARD SCAN MODE========
	//===============================				
				case CARD_SCAN:
				display_dalay_check();
				servo_motor_check();
				buzzer_off_check();
				card_scan_ch();
				I2C_LCD_write_string_XY(0,0, "=SCAN YOUR CARD=");
				if (queue_empty() != TRUE)
				{
					key_value_door=read_queue();
					door[index_door++]=key_value_door;
					if (key_value_door ==' ')
					{
						
						I2C_LCD_clear();
						mode = MENU;
					}
					
				}
				break;
				
	//==================================
	//=========KEY_PASSWORD MODE========
	//==================================		
				case KEY_PASSWORD:
				display_dalay_check();
				servo_motor_check();
				
				I2C_LCD_write_string_XY(0,0, "ENTER PASSWORD");
				
				if (queue_empty() != TRUE)
				{
					key_value_password=read_queue();
					passwo[index_passwo++]=key_value_password;
					
					I2C_LCD_goto_XY(1,index_passwo);
				    I2C_LCD_write_data(key_value_password);
					if (key_value_password == '=')
					{
						password(passwo);
					}
					if (key_value_password == ' ')
					{
						switch(mode_not1)
						{
							case 1 :
							clear_password();
							break;
							default :
							mode = MENU;
							break;
						}
					}
  	     			if (key_value_door ==' ')
					{
 						I2C_LCD_clear();
						mode = MENU;
 					}
				}
				break;
	//==================================
	//=========CALCULATOR MODE==========
	//==================================			
				case CAL:
				I2C_LCD_write_string_XY(0,0, "=Calculator=");
				if (queue_empty() != TRUE)
				{
					key_value_cal=read_queue();
					I2C_LCD_goto_XY(1,index_cal);
					I2C_LCD_write_data(key_value_cal);
					cal[index_cal++]=key_value_cal;
					
					if (key_value_cal == '=')
					{
						calculator(cal);
						
					}
					if (key_value_cal == ' ')
					{	
						switch(mode_not)
						{
						case 1 :
						clear_buff();
						break;
						default :
						mode = MENU;
						break;
						}
						
					}
				}
				break;	
			//==================================
			//=========CARD RESISTOR MODE==========
			//==================================		
				case CARD_REG:
				card_resister();
				I2C_LCD_write_string_XY(0,0, "CARD RESISTER");
				if(getButton2State())
				{
					I2C_LCD_clear();
					mode = DELET_CARD;
				}
				if (queue_empty() != TRUE)
				{
					key_value_menu=read_queue();
					menu[index_menu++]=key_value_menu;
					if(key_value_menu == ' ')
					{
						I2C_LCD_clear();
						mode=MENU;
					}
				}
				break;
			//==================================
			//=========CARD DELETE MODE==========
			//==================================	
				case DELET_CARD:
				I2C_LCD_write_string_XY(0,0, "CARD DELETE");
				if(getButton2State())
				{
					I2C_LCD_clear();
					mode = CARD_REG;
				}
				if (queue_empty() != TRUE)
				{
					key_value_menu=read_queue();
					menu[index_menu++]=key_value_menu;
					if(key_value_menu == ' ')
					{
						I2C_LCD_clear();
						mode=MENU;
					}
					if(key_value_menu == '=')
					{
						card_delete_display();
						I2C_LCD_write_string_XY(1,0, "DELETE");
					}
				}
				break;
				case CARD_CHECK:
				card_check_display();
				I2C_LCD_write_string_XY(0,0, "CARD CHECK");
				if (queue_empty() != TRUE)
				{
					key_value_menu=read_queue();
					menu[index_menu++]=key_value_menu;
					if(key_value_menu == ' ')
					{
						I2C_LCD_clear();
						mode=MENU;
					}
				}
				break;
			}
}
}



void rfid_version_check(uint8_t byte)
{
	if (byte == 0x92)
	{
		printf("RC522v2 Detect !!\n");
	}
	else if(byte == 0x91 || byte == 0x90)
	{
		printf("RC522v1 Detect !!\n");
	}
	else
	{
		printf("No Reader found !!\n");
	}
}
void calculator(char *cal)
{
	char num1buff[10];
	char num2buff[10];
	char calculator_mode=' ';
	
	int num1, num2, result;
	int i,j;
	
	num1=num2=result=0;
	
	for (i=0; cal[i] >= '0' && cal[i] <= '9'; i++)
	num1buff[i] =  cal[i];
	
	num1buff[i]='\0';
	calculator_mode=cal[i];
	i++;
	
	for (j=0; cal[i] >= '0' && cal[i] <= '9'; i++, j++)
	num2buff[j] =  cal[i];
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
	sprintf(cal, "%d%c%d=%d      ",num1,calculator_mode,num2,result);
	I2C_LCD_write_string_XY(1, 0, cal);
	printf("%s\n", cal);
}

void clear_buff()
{
	for (int i=0; i < 40; i++)
	{cal[i] = ' ';}
	I2C_LCD_clear();
	index_cal=0;
}
void clear_password()
{
	for (int i=0; i < 4; i++)
	{passwo[i] = ' ';}
	I2C_LCD_clear();
	index_passwo=0;
}
void password(char *passwo)
{
	
	char num1buff[3];
	
	char calculator_mode=' ';
	
	int num1;
	int i,j;

	num1=0;

	
	for (i=0; passwo[i] >= '0' && passwo[i] <= '9'; i++)
	num1buff[i] =  passwo[i];
	
	
	num1=atoi(num1buff);
	
	
	printf("num1 = %d \n", num1);
	if(num1 == 3059)
	{
		I2C_LCD_write_string_XY(1,0, "=OPEN DOOR=");
		svangle(90);
		
	}
	else
	{
		I2C_LCD_write_string_XY(1,0, "=wrong=");
	}
	
}
void card_resister()
{
	byte = mfrc522_request(PICC_REQALL, str); // 카드의 tagging 유무 확인
	
	if(byte == CARD_FOUND)
	{
		byte = mfrc522_get_card_serial(str);
		
		for (int i = 0; i < 5; i++)
		{
		strncpy(card_record_table[card_list],str,5);
		printf("%02x ",str[i]);      //0x20 0x40 0x20 0x10 0x50
		
		}
		for(int i = 0; i < 10; i++)
		{
			sprintf(sbuff,"%02x %02x %02x %02x %02x    ", card_record_table[i][0],card_record_table[i][1],card_record_table[i][2],card_record_table[i][3],card_record_table[i][4],card_record_table[i][5]);
			I2C_LCD_write_string_XY(1,0, sbuff);
			
			break;
		}
	
	}
}

void card_delete_display()
{
	strncpy(card_record_table,card_record_table_copy,5);

}

void card_check_display()
{
	
		for(int i = 0; i < 10; i++)
		{
			sprintf(sbuff,"%02x %02x %02x %02x %02x    ", card_record_table[i][0],card_record_table[i][1],card_record_table[i][2],card_record_table[i][3],card_record_table[i][4],card_record_table[i][5]);
			I2C_LCD_write_string_XY(1,0, sbuff);
		
			break;
		}
}


void card_scan_ch()
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
				buzzer_on();
				I2C_LCD_write_string_XY(1,0, "!!!OPEN DOOR!!! ");
				
				break;
			}
		}
		if(!valid_card)        //valid == 0
		{	
			counter_flag = 1;
			counter = 0;
			printf("invalid card!!\n");
			buzzer_on();
			I2C_LCD_write_string_XY(1,0, "!!!Wrong CARD!!! ");
		}
		
		

	}
}
void display_dalay_check()
{
	if(display_dalay_timer > 2000)
	{	
		I2C_LCD_clear();
		mode = MENU;
	}
}