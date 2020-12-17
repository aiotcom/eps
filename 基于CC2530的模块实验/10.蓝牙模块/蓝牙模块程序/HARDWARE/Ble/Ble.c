#include "Ble.h"	
#include "delay.h"
#include "Usart.h"

uint8_t BLE_AT_STA = 0;

//==========================================================
//	函数名称：	BLE_Init()
//
//	函数功能：	蓝牙初始化函数
//
//	入口参数：	无
//				
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void BLE_Init(void)
{
	delay_ms(2000);
	ble_send_cmd("AT+TXPW0\r\n","OK",2000);	//设置发射功率4dBm
		delay_ms(1000); 
	ble_send_cmd("AT+ROLE0\r\n","OK",20);	//设置为从设备  
	delay_ms(1000);    
	ble_send_cmd("AT+MODE0\r\n","OK",20);	//设置为从设备
    
  BLE_AT_STA = 1;        /*AT指令发送完成标志位*/
}


