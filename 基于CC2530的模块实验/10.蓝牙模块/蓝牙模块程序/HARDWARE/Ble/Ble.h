#ifndef __Ble_H
#define __Ble_H

#include <ioCC2530.h>
#include "delay.h"

#define	AT		"AT\r\n"
#define	TXPW		"AT+TXPW0\r\n"		//设置信号强度
#define	ROLE		"AT+ROLE0\r\n"		//设置主从1：主设备   0：从设备
#define	NAME 		"AT+NAMEBaCheng\r\n"	//修改蓝牙名称
#define	ADDR 		"AT+ADDR?\r\n"		//查询本机MAC地址
#define	CON 		"AT+CON987BF362C25D\r\n"//连接该MAC地址的从机

extern uint8_t BLE_AT_STA;    /*AT指令发送完成标志位*/
extern void BLE_Init(void);//蓝牙初始化

#endif

