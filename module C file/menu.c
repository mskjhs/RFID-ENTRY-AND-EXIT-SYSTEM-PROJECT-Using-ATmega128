/*
 * menu.c
 *
 * Created: 2021-08-17 오후 3:06:51
 *  Author: kcci
 */ 
#include "menu.h"

void display_command_menu(void)
{
	printf("\n-- help --\n");
	printf("gettime\n");
	printf("settimeyymmddhhmmss\n");
}

void show_menu(void)
{
	printf("----- MENU ------\n");
	printf("1. show Time\n");   // 시 분 초
	printf("2. show Date\n");   // 년 월 일
	printf("3. update Date_time\n");
	printf("Select : ");
}

void menu_exec()
{
	char *rxData;
	char yy[4], mm[4], dd[4], hh[4], min[4], ss[4];
	struct _ds1302 updateData;
	
	//gettime
	//settimeyymmddhhmmss
	//led0on
	//led0off
	//ledallon
	//ledalloff
	if (isRxString())
	{
		rxData = getRxString();   // Message를 읽어 온다.
		
		if (strncmp(rxData, "help", sizeof("help")) == 0)
		{
			printf("\n-- help --\n");
			printf("gettime\n");
			printf("settimeyymmddhhmmss\n");
		} 
		else if (strncmp(rxData, "gettime", sizeof("gettime")) == 0)
		{
			DS1302_GetTime(&stTime);
			DS1302_GetDate(&stTime);
			
			printf("date %04d:%02d:%02d\n", 2000+stTime.year, stTime.month, stTime.date);
			printf("time %02d:%02d:%02d\n", stTime.hour, stTime.minutes, stTime.seconds);
		}
		// 0123456789012345678
		// settimeyymmddhhmmss
		else if (strncmp(rxData, "settime", 7) == 0)
		{
			printf("%s\n", rxData);

			if (digit_check(rxData+7, 2) == NULL)
			{
				strncpy(yy,rxData+7,2); 
			}
			else
			{
				printf("\nIncorrect Year !!!\n");
				return;
			}
			 
			if (digit_check(rxData+9, 2) == NULL)
			{
				strncpy(mm,rxData+9,2); 
			}
			else
			{
				printf("\nIncorrect Month !!!\n");
				return;
			}
			
			if (digit_check(rxData+11, 2) == NULL)
			{
				strncpy(dd,rxData+11,2); 
			}
			else
			{
				printf("\nIncorrect Date !!!\n");
				return;
			}

			if (digit_check(rxData+13, 2) == NULL)
			{
				strncpy(hh,rxData+13,2);
			}
			else
			{
				printf("\nIncorrect hour !!!\n");
				return;
			}

			if (digit_check(rxData+15, 2) == NULL)
			{
				strncpy(min,rxData+15,2);
			}
			else
			{
				printf("\nIncorrect minutes !!!\n");
				return;
			}

			if (digit_check(rxData+17, 2) == NULL)
			{
				strncpy(ss,rxData+17,2);
			}
			else
			{
				printf("\nIncorrect second !!!\n");
				return;
			}

			updateData.year = atoi(yy);
			updateData.month = atoi(mm);
			updateData.date = atoi(dd);
			updateData.hour = atoi(hh);
			updateData.minutes = atoi(min);
			updateData.seconds = atoi(ss);
			
			DS1302_SetTimeDates(updateData);
			
			printf("\n\ntime update success!!!!\n");
			DS1302_GetTime(&stTime);
			DS1302_GetDate(&stTime);			
			printf("date %04d:%02d:%02d\n", 2000+stTime.year, stTime.month, stTime.date);
			printf("time %02d:%02d:%02d\n", stTime.hour, stTime.minutes, stTime.seconds);
 
		} 
		else
		{
			printf("\n\nInvalid Command !!!\n");			
		} 		
	}
}

void display_clock_lcd(void)
{
	static uint8_t prevSec=0;    // 1초 1번씩 시간을 출력 하기 위한 변수 
	char buff[40];
	
	DS1302_GetTime(&stTime);
	DS1302_GetDate(&stTime);
	
	if (prevSec != stTime.seconds)   // 초가 변경 되었으면
	{
		prevSec = stTime.seconds;
		sprintf(buff, "date 2021-%02d-%02d", stTime.month, stTime.date);   //수정함 고정값 2021
		I2C_LCD_write_string_XY(0, 0, buff);
		
		sprintf(buff, "time %02d:%02d:%02d", stTime.hour, stTime.minutes, stTime.seconds);
		I2C_LCD_write_string_XY(1, 0, buff);
	}
}

int digit_check(char *buff, int len)
{
	for (int i=0; i < len; i++)
	{
		if (buff[i] >= '0' && buff[i] <= '9')
			;
		else return 1;
	}
	
	return 0;
}
void updateDateTime()
{
	uint8_t rxdata=0;
	struct _ds1302 updateData;
	
	// year:
	// month:
	// date:
	// hour:
	// minute:
	// sec: 
	for (int i=0; i < 6; i++)
	{
		switch (i)
		{
			case 0:  // 년
				printf("year(00~99): ");
				rxdata = get_number();
				updateData.year = rxdata;
				printf("set year: %d\n", updateData.year);
				break;
			case 1:  // 월
				printf("month: ");
				rxdata = get_number();
				updateData.month = rxdata;
				printf("set month: %d\n", updateData.month);
				break;
			case 2:   // 일
				printf("date: ");
				rxdata = get_number();
				updateData.date = rxdata;
				printf("set date: %d\n", updateData.date);
				break;
			case 3:   // 시 
				printf("hour: ");
				rxdata = get_number();
				updateData.hour = rxdata;
				printf("set hour: %d\n", updateData.hour);
				break;
			case 4:   // 분 
				printf("minutes: ");
				rxdata = get_number();
				updateData.minutes = rxdata;
				printf("set minutes: %d\n", updateData.minutes);
				break;
			case 5:   // 초
				printf("sec: ");
				rxdata = get_number();
				updateData.seconds = rxdata;
				printf("set sec: %d\n", updateData.seconds);
				break;
			break;			
		}	
	}
	DS1302_SetTimeDates(updateData);
	return 0;
}

uint8_t get_number()
{
	uint8_t rxdata=0, *rxstring;
	
	while(isRxString()==0)
		;
	rxstring = getRxString();
	rxdata = atoi(rxstring);
	
	return rxdata;
}