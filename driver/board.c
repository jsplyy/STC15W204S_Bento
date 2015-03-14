/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           board.c
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        ����֧��       
********************************************************************************************************/


#include "..\config.h"

#include "intrins.h"


ST_CABINET_DATA st_A,st_B;
volatile unsigned char canOpen_A = 1,canOpen_B = 1;

sbit IO_LED   = P0^7;


/*********************************************************************************************************
**EEPROM ��д����
**add by yoc  2015.1.3
**ע��STC15W204SоƬ�ڲ�EEPROM ֻ��1024�ֽڵĴ洢�ռ�
*********************************************************************************************************/
#define CMD_IDLE    0               //����ģʽ
#define CMD_READ    1               //IAP�ֽڶ�����
#define CMD_PROGRAM 2               //IAP�ֽڱ������
#define CMD_ERASE   3               //IAP������������
#define ENABLE_IAP 0x83           //if SYSCLK<12MHz




#define DB_IR_DELAY() do{_nop_();_nop_();}while(0)


void Delay10us()		//@11.0592MHz
{
	unsigned char i;
	_nop_();
	i = 25;
	while (--i);
}



void DB_delay100us()		//@11.0592MHz
{
	unsigned char i, j;
	_nop_();_nop_();
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}





/*********************************************************************************************************
** Function name:     	IAP_idle
** Descriptions:	   	�ر�IAP����
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void IAP_idle()
{
    IAP_CONTR = 0;                  //�ر�IAP����
    IAP_CMD = 0;                    //�������Ĵ���
    IAP_TRIG = 0;                   //��������Ĵ���
    IAP_ADDRH = 0x80;               //����ַ���õ���IAP����
    IAP_ADDRL = 0;
}



/*********************************************************************************************************
** Function name:     	IAP_readByte
** Descriptions:	   	��EEPROM�����ȡһ�ֽ�
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
unsigned char IAP_readByte(unsigned int addr)
{
    unsigned char dat;                       //���ݻ�����
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_READ;             //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    dat = IAP_DATA;                 //��ISP/IAP/EEPROM����
    IAP_idle();                      //�ر�IAP����

    return dat;                     //����
}


/*********************************************************************************************************
** Function name:     	IAP_writeByte
** Descriptions:	   	дһ�ֽ����ݵ�ISP/IAP/EEPROM���� ��д����֮ǰҪ��������
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void IAP_writeByte(unsigned int addr, unsigned char dat)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_PROGRAM;          //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_DATA = dat;                 //дISP/IAP/EEPROM����
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    IAP_idle();
}


/*********************************************************************************************************
** Function name:     	IAP_eraseSector
** Descriptions:	   	�������� һ�ο��Բ���512�ֽ�
** input parameters:    
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void IAP_eraseSector(unsigned int addr)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_ERASE;            //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    IAP_idle();
}






/*********************************************************************************************************
** Function name:     	delayMs
** Descriptions:	    ������ʱ����
** input parameters:    ms ��λ����
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    ��ʼ���������IO
** input parameters:    ��	    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitGpio(void)
{

	// set P0.0
	P0M1 = 0x00;
	P0M0 = 0xFF;

    P1M1 = 0x00;	// 
    P1M0 = 0x36;	//
    
    P2M1 = 0x00;
    P2M0 = 0xFF;
    P3M1 = 0x50;
    P3M0 = 0xAC;
	P5M1 = 0x00;
	P5M0 = 0x20;

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
	RS485_ENABLE = 1;//����485����ģʽ
	st_A.openTimeout = 6500;
	st_B.openTimeout = 6500;
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
	RS485_74HC123_nB= 0;
	DB_delay100us();
	RS485_74HC123_nB = 1;
	DB_delay100us();
	DB_delay100us();
	RS485_74HC123_nB= 0;
	DB_delay100us();
	DB_delay100us();
	RS485_74HC123_nB = 1;
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
** Returned value:      1�ɹ� 0ʧ��
*********************************************************************************************************/
unsigned char DB_openAdoor()
{
	unsigned char i,flag = 0;
	unsigned int j;
	IO_DOOR_A_OUT = 1;
	for(i = 0;i < BT_OPEN_RCX;i++)
	{	
		for(j = 0;j < 2000;j++){
			IO_DOOR_A_PULSE = 0;
			Delay10us();
			IO_DOOR_A_PULSE = 1;
			if(IO_DOOR_A_SIGNAL == 0)//�����ɹ�
				flag = 1;
			DB_delay100us();
			DB_delay100us();
		}		
		if(flag == 1)//�����ɹ�
		{
			st_A.openTimeout = 6000;
			break;
		}
		else
			delayMs(500);
	}

	
	IO_DOOR_A_OUT = 0;	
	IO_DOOR_A_PULSE = 0;
	return flag;
	
}

unsigned char DB_openBdoor()
{
	unsigned char i,flag = 0;
	unsigned int j;
	IO_DOOR_B_OUT = 1;
	for(i = 0;i < BT_OPEN_RCX;i++)
	{
		for(j = 0;j < 2000;j++){
			IO_DOOR_B_PULSE = 0;
			Delay10us();
			IO_DOOR_B_PULSE = 1;
			if(IO_DOOR_B_SIGNAL == 0)//�����ɹ�
				flag = 1;
			DB_delay100us();
			DB_delay100us();
		}		
		if(flag == 1)//�����ɹ�
		{
			st_B.openTimeout = 6000;
			break;
		}
		else
			delayMs(500);
		
	}
	IO_DOOR_B_OUT = 0;
	IO_DOOR_B_PULSE = 0;	
	return flag;
}


/*********************************************************************************************************
** Function name:     	DB_AgoodsEmpty
** Descriptions:	   
** input parameters:    
** output parameters:   
** Returned value:      0�޻�  1�л�
*********************************************************************************************************/
unsigned char DB_AgoodsFull()
{
	unsigned char i,j,rcx = 0,flag = 0;
	for(i = 0; i < 3;i++){
		IO_IR_A_OUT = 1;
		for(j = 0;j < 30;j++){
			if(IO_IR_A_SIGNAL == 1){
				rcx = 1;
			}
			Delay10us();
		}
		if(rcx) flag++;
		rcx = 0;
		IO_IR_A_OUT = 0;
		delayMs(1);
	}
	IO_IR_A_OUT = 0;
	return (flag >= 2) ?  0 : 1;
	
}


unsigned char DB_BgoodsFull()
{
	unsigned char i,j,flag = 0,t = 0;
	
	for(i = 0; i < 3;i++)
	{
		IO_IR_B_OUT = 1;
		for(j = 0;j < 30;j++){
			if(IO_IR_B_SIGNAL == 1){
				t = 1;
			}
			Delay10us();
		}
		if(t == 1) flag++;
		t = 0;
		IO_IR_B_OUT = 0;
		delayMs(1);
	}
	IO_IR_B_OUT = 0;
	return (flag >= 2) ?  0 : 1;
}



