#ifndef _BOARD_H_
#define _BOARD_H_



#define FCLIK      11059200UL   /*使用22.1184M晶体*/


//
sbit RS485_ENABLE 	= P3^2;  
sbit RS485_74HC123_nB 	= P1^1; 	//上升沿触发



/*********************************************************************************************************
**设备IO定义
*********************************************************************************************************/
sbit IO_DOOR_A_OUT    	= P3^7;//A柜 门IO口
sbit IO_DOOR_A_PULSE    = P1^1;//A柜 门IO口 脉冲
sbit IO_DOOR_A_SIGNAL   = P3^6; //低电平为开锁，高电平为 闭锁
sbit IO_IR_A_OUT		= P5^5;//先打开红外发射管（P5.5设为高电平）检测完后要把P5.5发射管设为低电平。
sbit IO_IR_A_SIGNAL		= P5^4;//检测（P5.4电平）高电平为无货，低电平为有货物
sbit IO_LED_A		= P1^5;//P1.5设为高电平开照明灯，低电平关灯。



sbit IO_DOOR_B_OUT    	= P3^3;//B柜 门IO口
sbit IO_DOOR_B_PULSE    = P1^1;//B柜 门IO口 脉冲
sbit IO_DOOR_B_SIGNAL   = P1^0; //B低电平为开锁成功，高电平为开锁失败
sbit IO_IR_B_OUT		= P1^4;//
sbit IO_IR_B_SIGNAL		= P1^3;//B检测高电平为无货，低电平为有货物
sbit IO_LED_B		= P1^2;//设为高电平开照明灯，低电平关灯。



// A B两柜地址
#define BT_CABINET_A	0
#define BT_CABINET_B	1
#define BT_CABINET_NO	0xAA
#define BT_CABINET_CON	0xFF  //配置命令

#define BT_OPEN_RCX		3



typedef struct _st_cabinet_data_{

	unsigned char addr;
	unsigned char goods;
	unsigned char door;

}ST_CABINET_DATA;

extern ST_CABINET_DATA st_A,st_B;



/*********************************************************************************************************
**定义通用宏函数
*********************************************************************************************************/

#define HUINT16(v)   	(((v) >> 8) & 0x0F)
#define LUINT16(v)   	((v) & 0x0F)
#define INTEG16(h,l)  	(((unsigned int)h << 8) | l)

#define DB_ledAControl(s)	do{IO_LED_A = s;}while(0)
#define DB_ledBControl(s)	do{IO_LED_B = s;}while(0)


void delayMs(unsigned int ms);
void systemInit(void);
void SetRS485AsTxdMode(void);
void SetRS485AsRxdMode(void);






unsigned char DB_openAdoor();
unsigned char DB_openBdoor();
unsigned char DB_AgoodsFull();
unsigned char DB_BgoodsFull();


unsigned char IAP_readByte(unsigned int addr);
void IAP_writeByte(unsigned int addr, unsigned char dat);
void IAP_eraseSector(unsigned int addr);


#endif
