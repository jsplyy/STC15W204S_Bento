/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           board.c
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        板载支持       
********************************************************************************************************/


#include "..\config.h"

#include "intrins.h"


ST_CABINET_DATA st_A,st_B;


volatile unsigned char canOpen_A = 1,canOpen_B = 1;

sbit IO_LED   = P0^7;


/*********************************************************************************************************
**EEPROM 读写程序
**add by yoc  2015.1.3
**注意STC15W204S芯片内部EEPROM 只有1024字节的存储空间
*********************************************************************************************************/
#define CMD_IDLE    0               //空闲模式
#define CMD_READ    1               //IAP字节读命令
#define CMD_PROGRAM 2               //IAP字节编程命令
#define CMD_ERASE   3               //IAP扇区擦除命令
#define ENABLE_IAP 0x83           //if SYSCLK<12MHz




#define DB_IR_DELAY() do{_nop_();_nop_();}while(0)





void DB_delay100us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}

/*********************************************************************************************************
** Function name:     	IAP_idle
** Descriptions:	   	关闭IAP功能
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void IAP_idle()
{
    IAP_CONTR = 0;                  //关闭IAP功能
    IAP_CMD = 0;                    //清除命令寄存器
    IAP_TRIG = 0;                   //清除触发寄存器
    IAP_ADDRH = 0x80;               //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}



/*********************************************************************************************************
** Function name:     	IAP_readByte
** Descriptions:	   	从EEPROM区域读取一字节
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
unsigned char IAP_readByte(unsigned int addr)
{
    unsigned char dat;                       //数据缓冲区
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_READ;             //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    dat = IAP_DATA;                 //读ISP/IAP/EEPROM数据
    IAP_idle();                      //关闭IAP功能

    return dat;                     //返回
}


/*********************************************************************************************************
** Function name:     	IAP_writeByte
** Descriptions:	   	写一字节数据到ISP/IAP/EEPROM区域 当写数据之前要擦除扇区
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void IAP_writeByte(unsigned int addr, unsigned char dat)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_PROGRAM;          //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_DATA = dat;                 //写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IAP_idle();
}


/*********************************************************************************************************
** Function name:     	IAP_eraseSector
** Descriptions:	   	扇区擦除 一次可以擦除512字节
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void IAP_eraseSector(unsigned int addr)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_ERASE;            //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IAP_idle();
}






/*********************************************************************************************************
** Function name:     	delayMs
** Descriptions:	    毫秒延时程序
** input parameters:    ms 单位毫秒
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void delayMs(unsigned int ms)
{	 
	unsigned char i, j;
	while(ms--)
	{
		for(i = 0;i < 11;i++)
			for(j = 0;j < 190;j++);
	}
}




/*********************************************************************************************************
** Function name:     	InitGpio
** Descriptions:	    初始化电机控制IO
** input parameters:    无	    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void InitGpio(void)
{

	// set P0.0
	P0M1 = 0x00;
	P0M0 = 0xFF;

    P1M1 = 0x00;	// 
    P1M0 = 0x24;	//
    
    P2M1 = 0x00;
    P2M0 = 0xFF;
    P3M1 = 0x50;
    P3M0 = 0xAC;
	P5M1 = 0x00;
	P5M0 = 0x00;

	//p1.0



	IO_DOOR_A_OUT = 0;
	IO_DOOR_B_OUT = 0;
	IO_LED_A = 0;
	IO_LED_B = 0;
	IO_IR_A_OUT  = 0;
	IO_IR_B_OUT  = 0;
}



void systemInit(void)
{

	InitGpio();
	timer0Init();
	uart1Init();
	RS485_ENABLE = 1;//开启485发送模式
}


/*********************************************************************************************************
** Function name:     	SetRS485AsTxdMode
** Descriptions:	   
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void SetRS485AsTxdMode(void)
{	
	RS485_ENABLE = 1;
	//_nop_();_nop_();
	RS485_74HC123_nB= 0;
	_nop_();_nop_();
	RS485_74HC123_nB = 1;
	_nop_();	
}



/*********************************************************************************************************
** Function name:     	SetRS485AsRxdMode
** Descriptions:	    
** input parameters:    
** output parameters:   
** Returned value:     
*********************************************************************************************************/
void SetRS485AsRxdMode(void)
{
	RS485_74HC123_nB  = 0;
	RS485_ENABLE = 0;
}



/*********************************************************************************************************
** Function name:     	DB_opendoor
** Descriptions:	    
** input parameters:    
** output parameters:   
** Returned value:      1成功 0失败
*********************************************************************************************************/
unsigned char DB_openAdoor()
{
	unsigned char i;

	IO_DOOR_A_OUT = 1;
	for(i = 0;i < BT_OPEN_RCX;i++)
	{
		IO_DOOR_A_PULSE = 0;
		_nop_();_nop_();
		IO_DOOR_A_PULSE = 1;
		delayMs(10);
		if(IO_DOOR_A_SIGNAL == 0)//开锁成功
		{
			IO_DOOR_A_OUT = 0;
			return 1;
		}	
		delayMs(500);
	}
	IO_DOOR_A_OUT = 0;	
	return 0;
	
}

unsigned char DB_openBdoor()
{
	unsigned char i;
	IO_DOOR_B_OUT = 1;
	for(i = 0;i < BT_OPEN_RCX;i++)
	{
		IO_DOOR_A_PULSE = 0;
		_nop_();_nop_();
		IO_DOOR_B_PULSE = 1;
		delayMs(10);
		if(IO_DOOR_B_SIGNAL == 0)//
		{
			IO_DOOR_B_OUT = 0;
			return 1;
		}	
		delayMs(500);
	}
	IO_DOOR_B_OUT = 0;	
	return 0;
}


/*********************************************************************************************************
** Function name:     	DB_AgoodsEmpty
** Descriptions:	   
** input parameters:    
** output parameters:   
** Returned value:      0无货  1有货
*********************************************************************************************************/
unsigned char DB_AgoodsFull()
{
	unsigned char rcx = 50;//5ms延时
	IO_IR_A_OUT = 1;
	while(rcx--)
	{
		if(IO_IR_A_SIGNAL == 1)//去抖
		{
			DB_IR_DELAY();
			if(IO_IR_A_SIGNAL == 1) //表示收到发射管的红外信号 即 无货
			{
				IO_IR_A_OUT = 0;
				return 0;
			}
		}
		else
		{
			DB_delay100us();
		}
	}	
	IO_IR_A_OUT = 0;			
	return 1;
}


unsigned char DB_BgoodsFull()
{
	unsigned char rcx = 50;//5ms延时
	IO_IR_B_OUT = 1;
	while(rcx--)
	{		
		if(IO_IR_B_SIGNAL == 1)//not empty
		{
			DB_IR_DELAY();
			if(IO_IR_B_SIGNAL == 1) //可判断有货
			{
				IO_IR_B_OUT = 0;
				return 0;
			}			
		}	
		else
		{
			DB_delay100us();
		}
	}	
	IO_IR_B_OUT = 0;		
	return 1;
}





