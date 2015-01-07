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

unsigned char data tick;
unsigned char data second;

unsigned int uartTimeout = 0;
unsigned int ioTimeout = 0;
unsigned int irTimeout = 0;
unsigned int ireTimeout = 0;


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
	second = 0;
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
	if(irTimeout) irTimeout--;
	if(ireTimeout) ireTimeout--;
	if(tick >= 100)
	{
		tick = 0;
		if(second > 0)
			second--;
	}
}



