/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           API_bento.c	 
** Created by:          yoc
** Created date:        2014-12-10
** Version:             V1.0 
** Descriptions:        便利柜通信协议接口  所有函数均已BT 开头 即 bento   
********************************************************************************************************/
#include "..\config.h"
#include "API_Bento.h"




static unsigned char cur_cabinet = BT_CABINET_NO;



#define PC_HEAD 		0
#define PC_ADDR			1
#define PC_CMD			2
#define PC_PARA			3
#define PC_CRC			4
#define PC_END			5
#define PC_LEN			6


static unsigned char recvbuf[8] = {0};







/*********************************************************************************************************
** Function name:     	BT_read_flash
** Descriptions:	    读取flash数据
** input parameters:   
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void BT_read_flash(void)
{
	unsigned char crc;
	st_A.addr = IAP_readByte(BT_FLASH_ADRR);
	st_B.addr = IAP_readByte(BT_FLASH_ADRR + 4);
	crc = IAP_readByte(BT_FLASH_ADRR + 8);
	if(crc != 0xE5)
	{
		st_A.addr = 31;
		st_B.addr = 21;
	}	
}



/*********************************************************************************************************
** Function name:     	BT_read_flash
** Descriptions:	    读取flash数据
** input parameters:   
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void BT_write_flash(unsigned char cabinet,unsigned char addr)
{
	if(cabinet == BT_CABINET_A)
		st_A.addr = addr;
	else if(cabinet == BT_CABINET_B)
		st_B.addr = addr;
	else return;
	IAP_eraseSector(0x0000);
	IAP_writeByte(BT_FLASH_ADRR, st_A.addr);
	IAP_writeByte(BT_FLASH_ADRR + 4, st_B.addr);
	IAP_writeByte(BT_FLASH_ADRR + 8, 0xE5);
}







/*********************************************************************************************************
** Function name:     	BT_send_data
** Descriptions:	    便利柜回应数据码
** input parameters:    cmd  命令码
** output parameters:   无
** Returned value:      1成功  0失败
*********************************************************************************************************/
unsigned char BT_send_data(unsigned char data1,unsigned char data2)
{
	unsigned char crc = 0;
	crc = recvbuf[PC_ADDR] + recvbuf[PC_CMD] + data1  + data2;
	uart1Clear();
	SetRS485AsTxdMode();
	uart1PutCh(BT_START);	//同步码
	uart1PutCh(recvbuf[PC_ADDR]);		//柜子号码
	uart1PutCh(recvbuf[PC_CMD]);		//命令码
	uart1PutCh(data1);		//门状态
	uart1PutCh(data2);		//物品状态
	uart1PutCh(crc);		//校验码
	uart1PutCh(BT_STOP);		//结束吗
	SetRS485AsRxdMode();
	return 1;
}


/*********************************************************************************************************
** Function name:     	BT_send_state
** Descriptions:	    便利柜回应状态码
** input parameters:    
** output parameters:   无
** Returned value:      1成功  0失败
*********************************************************************************************************/
unsigned char BT_send_state()
{
	uart1Clear();
	SetRS485AsTxdMode();
	uart1PutCh(BT_START);	//同步码
	uart1PutCh(st_A.addr);		//柜子A号码
	uart1PutCh(st_A.door);		//门状态
	uart1PutCh(st_A.goods);		//物品状态
	
	uart1PutCh(st_B.addr);		//柜子B号码
	uart1PutCh(st_B.door);		//门状态
	uart1PutCh(st_B.goods);		//物品状态
	uart1PutCh(BT_STOP);		//结束码
	SetRS485AsRxdMode();
	return 1;
}


/*********************************************************************************************************
** Function name:     	BT_recv_cmd
** Descriptions:	    便利柜接收数据包
** input parameters:    
** output parameters:   无
** Returned value:      0 无数据接收  1接收正确数据  2接收数据错误
*********************************************************************************************************/
unsigned char BT_recv_cmd()
{
	unsigned char index = 0,temp;
	if(!uart1IsNotEmpty())
		return 0;
	recvbuf[index++] = uart1GetCh();
	if(recvbuf[PC_HEAD] != BT_START)
	{
		return 2;
	}
	uartTimeout = 8; //接收剩余数据//200ms超时
	while(uartTimeout)
	{
		if(uart1IsNotEmpty())
		{
			recvbuf[index++] = uart1GetCh();
			if(index >= PC_LEN && recvbuf[PC_END] == BT_PC_STOP )//收到结束码 且长度正确
			{
				temp = recvbuf[PC_ADDR] +recvbuf[PC_CMD]+recvbuf[PC_PARA];
				if(temp == recvbuf[PC_CRC])//校验码也正确 白哦是收到正确指令
				{
					return 1;				
				}
				else 
					return 2;
			}
			else
				_nop_();
			
		}
		else
			_nop_();
	}
	return 2;
}



/*********************************************************************************************************
** Function name:     	BT_handle_req
** Descriptions:	    处理数据请求
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void BT_handle_req()
{
	
	if(recvbuf[PC_CMD] == BT_OPEN_REQ) //开门命令
	{
		if(recvbuf[PC_ADDR] == st_A.addr)
		{
			st_A.door = DB_openAdoor();
			st_A.goods = DB_AgoodsFull();
			BT_send_data(st_A.door,st_A.goods);
		}			
		else
		{
			st_B.door = DB_openBdoor();
			st_B.goods = DB_BgoodsFull();
			BT_send_data(st_B.door,st_B.goods);
		}
	}
	else if(recvbuf[PC_CMD] == BT_DOOR_STATE_REQ || recvbuf[PC_CMD] == BT_IR_STATE_REQ)//查询门状态//查询红外线状态
	{
		if(recvbuf[PC_ADDR] == st_A.addr)
		{
			st_A.goods = DB_AgoodsFull();
			BT_send_data(st_A.door,st_A.goods);	
		}	
		else
		{
			st_B.goods = DB_BgoodsFull();
			BT_send_data(st_B.door,st_B.goods);
		}
			
	}
	else if(recvbuf[PC_CMD] == BT_REAL_TIME_REQ)//查询实时状态
	{
		st_A.goods = DB_AgoodsFull();
		st_B.goods = DB_BgoodsFull();
		BT_send_state();
	}
		
	else 
		_nop_();
	
	

}


/*********************************************************************************************************
** Function name:     	BT_config_req
** Descriptions:	    配置请求
** input parameters:    
** output parameters:   无
** Returned value:      
*********************************************************************************************************/
void BT_config_req()
{
	unsigned char data1,data2;
	//static unsigned char testA = 0,testB = 0;
	if(recvbuf[PC_CMD] == BT_CONFIG_START_REQ)
	{
		data1 = rand();
		data2 = data1 + 1;
	}
	else if(recvbuf[PC_CMD] == BT_CONFIG_A_REQ)
	{
		BT_write_flash(BT_CABINET_A,recvbuf[PC_PARA]);
		data1 = 0;data2 = 0;
		BT_read_flash();
		recvbuf[PC_ADDR] = st_A.addr;
	}
	else if(recvbuf[PC_CMD] == BT_CONFIG_B_REQ)
	{
		BT_write_flash(BT_CABINET_B,recvbuf[PC_PARA]);
		data1 = 0;data2 = 0;	
		BT_read_flash();
		recvbuf[PC_ADDR] = st_B.addr;
	}
	else if(recvbuf[PC_CMD] == BT_CONFIG_CHECK_REQ)
	{
		data1 = st_A.addr;
		data2 = st_B.addr;
	}
	else if(recvbuf[PC_CMD] == BT_CONFIG_TEST_REQ)//进入生产测试模式
	{
		if(DB_AgoodsFull()) 
		{
			if(st_A.istest == 0)
			{
				st_A.istest = 1;
				DB_openAdoor();
			}
		}
		else
			st_A.istest = 0;
		if(DB_BgoodsFull()) 
		{
			if(st_B.istest == 0)
			{
				st_B.istest = 1;
				DB_openBdoor();
			}
			
		}
		else
			st_B.istest = 0;
		return;
	}
	else
		return;
	BT_send_data(data1, data2);

}





/*********************************************************************************************************
** Function name:     	BT_task
** Descriptions:	    便利柜任务
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void BT_task(void)
{
	unsigned char res;
	res = BT_recv_cmd();//接收数据 
	if(res == 1)//有回应 并且数据正确 
	{
		//delayMs(5);
		if(recvbuf[PC_ADDR] == 0xFF) //配置模式
		{
			BT_config_req();
		}
		else if(recvbuf[PC_ADDR] == st_A.addr || recvbuf[PC_ADDR] == st_B.addr)//柜命令
		{
			BT_handle_req();//处理命令 并发送回应
		}
		else  //不是本机的命令直接抛弃
			_nop_();
	}
	else
		_nop_();

	
#if 0
	if(irTimeout == 0)
	{
		if(ir_choose == 0)
		{
			ir_choose = 1;
			DB_AgoodsFull();
		}
		else 
		{
			ir_choose = 0;
			DB_BgoodsFull();
		}
		irTimeout = 50;//100ms扫描
		
	}
#endif

}


