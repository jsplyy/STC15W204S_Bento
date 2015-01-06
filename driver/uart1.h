/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           uart.h
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        ´®¿ÚÇý¶¯        
********************************************************************************************************/
#ifndef _UART1_H_
#define _UART1_H_


#define BT_DEBUG	0

#define UART1_BAUD		(9600) 


void  uart1PutCh( unsigned char ch );
void uart1PutStr(unsigned char *str,unsigned int len);
unsigned char uart1GetCh(void);
unsigned char uart1IsNotEmpty(void);
void uart1Clear(void);
unsigned char uart1Init(void);
#if (BT_DEBUG == 1)
void trace(unsigned char *format,...);
#endif
#endif