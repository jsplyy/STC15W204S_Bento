/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           API_bento.h
** Created by:          yoc
** Created date:        2014-12-10
** Version:             V1.0 
** Descriptions:        便利柜驱动板通信协议        
********************************************************************************************************/
#ifndef _API_BENTO_H_
#define _API_BENTO_H_

/*********************************************************************************************************
** 协议包宏定义  
** add by yoc
*********************************************************************************************************/
#define BT_START			0xBB
#define BT_PC_STOP			0x03
#define BT_STOP				0xFE

#define BT_OPEN_REQ			0x0A	//表示开锁，开对应柜子的锁；
#define BT_DOOR_STATE_REQ	0x0B	//表示门状态检测，检测对应柜子的门状态；
#define BT_IR_STATE_REQ		0x0C	//表示红外线检测，检测对应柜子的储物状态，是否为空箱；
#define BT_REAL_TIME_REQ	0x0D   //表示实时门状态，实时物品状态


#define BT_CONFIG_START_REQ		0xF3  //开始标志
#define BT_CONFIG_B_REQ		0xF2  //b柜设置地址命令
#define BT_CONFIG_A_REQ		0xF1  //a柜设置地址命令
#define BT_CONFIG_TEST_REQ	0xF8  //生产模式命令
#define BT_CONFIG_CHECK_REQ	0xF9  //查询地址命令

#define BT_ADDR_FLASH_PAGE	0x0000


#define BT_CALL_DIS			0 //定义回显




void BT_task(void);

void BT_read_flash(void);
#endif
