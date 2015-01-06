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
    P0M1 = 0x00;
    P0M0 = 0xff;
    P1M1 = 0x40;	// 01000000b
    P1M0 = 0xB0;	// 10110000b
    P2M1 = 0x00;
    P2M0 = 0xff;
    P3M1 = 0x00;
    P3M0 = 0x0C;
}



void systemInit(void)
{
	InitGpio();
	timer0Init();
	uart1Init();


	IO_DOOR_A = 0;
	IO_DOOR_B = 0;

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
	//RS485_ENABLE = 1;
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
	
	//RS485_ENABLE = 0;
	//delayMs(500);
}



/*********************************************************************************************************
** Function name:     	DB_opendoor
** Descriptions:	    开锁
** input parameters:    no 1:A柜  2:B 柜 
** output parameters:   
** Returned value:      1成功 0失败
*********************************************************************************************************/
unsigned char DB_opendoor(unsigned char no)
{
	no = no;
	return no;
}

/*********************************************************************************************************
** Function name:     	DB_goodsNotEmpty
** Descriptions:	    查询货物检测
** input parameters:    no 1:A柜  2:B 柜 
** output parameters:   
** Returned value:      0无货  1有货
*********************************************************************************************************/
unsigned char DB_goodsNotEmpty(unsigned char no)
{
	no = no;return no;
}


/*********************************************************************************************************
** Function name:     	DB_ledControl
** Descriptions:	    照明控制
** input parameters:    no 1:A柜  2:B 柜 
** output parameters:   
** Returned value:      1成功  0失败
*********************************************************************************************************/
unsigned char DB_ledControl(unsigned char no)
{
	no = no;return no;
}



