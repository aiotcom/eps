#include "Ble.h"	
#include "delay.h"
#include "Usart.h"

void Ble_Init(void)
{
	delay_ms(3000);												/*延时等待*/		
	ble_send_cmd("AT+TXPW0\r\n","OK",2000);						/*设置发射功率4dBm*/
    HAL_Delay(100);
    ble_send_cmd("AT+ROLE0\r\n","OK",2000);						/*设置为从设备*/   
    HAL_Delay(100);    
    ble_send_cmd("AT+MODE0\r\n","OK",2000);						/*设置为从设备*/
	USART_AT_STA = 1;										    /*AT指令发送完成标志位*/
}


