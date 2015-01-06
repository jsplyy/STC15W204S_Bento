#ifndef _BOARD_H_
#define _BOARD_H_



#define FCLIK      11059200UL   /*使用22.1184M晶体*/


//
sbit RS485_ENABLE 	= P3^2;  
sbit RS485_74HC123_nB 	= P1^1; 	//上升沿触发



/*********************************************************************************************************
**设备IO定义
*********************************************************************************************************/
sbit IO_DOOR_A    = P3^3;//A柜 门IO口
sbit IO_DOOR_B    = P3^7;//B柜 门IO口






// A B两柜地址
#define BT_CABINET_A	0
#define BT_CABINET_B	1
#define BT_CABINET_NO	0xAA
#define BT_CABINET_CON	0xFF  //配置命令




/*********************************************************************************************************
**定义通用宏函数
*********************************************************************************************************/

#define HUINT16(v)   	(((v) >> 8) & 0x0F)
#define LUINT16(v)   	((v) & 0x0F)
#define INTEG16(h,l)  	(((unsigned int)h << 8) | l)




void delayMs(unsigned int ms);
void systemInit(void);
void SetRS485AsTxdMode(void);
void SetRS485AsRxdMode(void);




unsigned char IAP_readByte(unsigned int addr);
void IAP_writeByte(unsigned int addr, unsigned char dat);
void IAP_eraseSector(unsigned int addr);
unsigned char DB_opendoor(unsigned char no);
unsigned char DB_goodsNotEmpty(unsigned char no);
unsigned char DB_ledControl(unsigned char no);

#endif
