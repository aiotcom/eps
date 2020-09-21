#ifndef __WiFi_H__
#define __WiFi_H__

#include "stm32f1xx_hal.h"

#define AT         	 "AT\r\n"
#define CWMODE     	 "AT+CWMODE=2\r\n"												//设置为AP模式
#define RST        	 "AT+RST\r\n"															//软复位
#define CWSAP        "AT+CWSAP=\"BBCC\",\"12345678\",1,4\r\n"	//ssid: BBCC  pwd:123456
#define CIFSR        "AT+CIFSR\r\n"														//查询IP 
#define CIPMUX       "AT+CIPMUX=1\r\n"												//使能多连接
#define CIPSERVER    "AT+CIPSERVER=1,8080\r\n"								//配置为服务器-设置端口
#define CIPAPMAC     "AT+CIPAPMAC=\"18:fe:35:98:d3:7b\"\r\n"	//设置MAC地址
#define CIPAP      	 "AT+CIPAP=\"192.168.4.3\"\r\n"						//设置IP地址
/**
  * @brief  初始化ESP8266,并配置路由和连接平台
  * @param  server:按照AT指令配置服务器地址和端口字符串
    * @param  ssid_pwd: 按照AT指令配置路由器的SSID和PWD
  * @retval NONE
  **/
extern uint8_t WiFi_AT_STA;    /*AT指令发送完成标志位*/
void WiFi_Init(void);
uint8_t WiFi_Receive(uint8_t *precv);
void WiFi_Send(uint8_t *pdata,uint8_t len);
void WiFi_ByteReceive(uint8_t byte);
#endif


