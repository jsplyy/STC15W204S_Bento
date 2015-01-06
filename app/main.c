/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           main.c
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        STC  C语言入口        
********************************************************************************************************/

#include "..\config.h"
#include "API_Bento.h"





/*********************************************************************************************************
** Function name:     	main
** Descriptions:	    入口函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void main(void)
{
	unsigned char ch = 0;
	unsigned int i = 0;
	unsigned int temp= 0;
	
	systemInit();
	BT_read_flash();
	delayMs(1000);
	
	SetRS485AsTxdMode();
	uart1PutStr("Hello STC15W204S1....\r\n",sizeof("Hello STC15W204S1....\r\n"));
	SetRS485AsRxdMode();
	while(1)
	{
		BT_task();
		delayMs(50);
	}
			
}
