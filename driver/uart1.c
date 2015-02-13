/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           uart.c
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        串口驱动        
********************************************************************************************************/

#include "..\config.h"
#include <stdarg.h>



#define FOSC 			11059200L //33177600//22118400 // 


//unsigned char uartbuf[BUF_SIZE] = {0};//接收缓冲区
unsigned char recvbuf[BUF_SIZE] = {0};//接收缓冲区
volatile unsigned char tx;//缓冲区写索引
volatile unsigned char isRequest = 0;
unsigned char uart1Init(void)
{
	uart1_clear();
	SCON  = 0X50; 		//8位可变波特率，无奇偶校验	
	//BRT   = -(FCLIK/32/UART1_BAUD);
	AUXR  = 0x15; 
	REN   =	1;      		//允许接收
	T2L = (65536 - (FOSC/4/UART1_BAUD));   //设置波特率重装值
  	T2H = (65536 - (FOSC/4/UART1_BAUD))>>8;
	
	AUXR |= 0x10;			//start timer2
	ES    =	1;   			// 允许串口1中断
	EA    = 1;   		// 开总中断

	
	return 1;
}





void uart1_isr( void ) interrupt 4 using 1 
{
	unsigned char temp;

	if(TI)
	{
		TI = 0;
	}	
	if(RI)//recv isr
	{
		RI = 0;
		temp = SBUF;	
		if(isRequest == 1)
			return;
		if(temp == BT_START && tx == 0)//开头
		{
			recvbuf[tx++] = temp;
		}
		else
		{
			if(tx > 0)//继续接受数据
			{
				recvbuf[tx++] = temp;
				if(tx >= PC_LEN)//收满数据
				{
					if(recvbuf[PC_END] == BT_PC_STOP)
					{
						temp = recvbuf[PC_ADDR]+recvbuf[PC_CMD]+recvbuf[PC_PARA];
						if(temp == recvbuf[PC_CRC])//校验正确
						{
							if((recvbuf[PC_ADDR] == st_A.addr) || 
								(recvbuf[PC_ADDR] == st_B.addr) || 
								(recvbuf[PC_ADDR] == 0xFF))//视为本机命令
							{																
								isRequest = 1;																
							}
						}
					}	
					tx = 0;
				}
				
			}
			
		}
		
	}
	

}


/*********************************************************************************************************
** Function name:     	uartPutChar
** Descriptions:	    发送一字节数据
** input parameters:    ch 发送的字节
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void uart1PutCh(unsigned char ch)
{
	//SetRS485AsTxdMode1();
	ES=0;
	TI=0;
	SBUF=ch;
	while(TI==0);
	TI=0;
	ES=1;

}




/*********************************************************************************************************
** Function name:     	uart1IsNotEmpty
** Descriptions:	    判断接收缓冲区不为空
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
unsigned char uart1_isRequest(void)
{
	return (isRequest);
}




/*********************************************************************************************************
** Function name:     	uartClear
** Descriptions:	    清串口缓存区
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void uart1_clear(void)
{
	tx = 0;
	isRequest = 0;
}










