/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           timer.c
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        定时器中断        
********************************************************************************************************/
#include "..\config.h"

#define T10MS 			(0xDC00) //(65536 - 11059200 / 12 / 100)  == 0xDC00

volatile unsigned char data tick;


volatile unsigned int uartTimeout = 0;
volatile unsigned int ioTimeout = 0;

volatile unsigned char isspeacil = 0;


/*********************************************************************************************************
** Function name:     	timer0Init
** Descriptions:	    定时器0初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void timer0Init(void)
{
	AUXR &= 0x7F;// 定时器时钟设定为12T 模式
	TMOD &= 0xF0;
	TMOD |= 0x01;//set timer0 as 16bit mode
	TL0 = T10MS;
    TH0 = T10MS >> 8;
	tick = 0;
	TR0 = 1; //timer0 start running
	ET0 = 1;//enable timer0 interrupt
	EA = 1;//enable global interrupt
	delayMs(5);
}


/*********************************************************************************************************
** Function name:     	timer0_isr
** Descriptions:	    定时器0中断服务程序	 定义10ms级定时器
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void timer0_ISR( void ) interrupt 1 using 1 
{
	TL0 = T10MS;
    TH0 = T10MS >> 8;
	tick++;
	if(uartTimeout) uartTimeout--;
	if(ioTimeout) ioTimeout--;
	if(tick >= 10)
	{
		if(IO_DOOR_A_SIGNAL == 1)//闭锁
		{
			st_A.door = 0;
			DB_ledAControl(0);
		}	
		else
		{
			st_A.door = 1;
			DB_ledAControl(1);
		}
		if(IO_DOOR_B_SIGNAL == 1)//闭锁
		{
			st_B.door = 0;
			DB_ledBControl(0);
		}	
		else
		{
			st_B.door = 1;
			DB_ledBControl(1);
		}
		tick = 0;
	}
}



