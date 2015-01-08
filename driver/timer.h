/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           timer.h
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        定时器中断头文件        
********************************************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

extern volatile unsigned int uartTimeout;
extern volatile unsigned int ioTimeout;


void timer0Init(void);



#endif
