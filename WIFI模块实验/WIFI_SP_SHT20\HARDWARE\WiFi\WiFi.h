#ifndef __WiFi_H__
#define __WiFi_H__

#include "stm32f1xx_hal.h"

#define	CLOSE_TCP    (uint8_t*)"+++"																						//关闭TCP连接，退出透传模式
#define AT         	 (uint8_t*)"AT\r\n"
#define CWMODE     	 (uint8_t*)"AT+CWMODE=1\r\n"																//设置模式为STA
#define RST        	 (uint8_t*)"AT+RST\r\n"																			//软重启模块
#define CIFSR        (uint8_t*)"AT+CIFSR\r\n"																		//查询IP地址
#define CWJAP        (uint8_t*)"AT+CWJAP=\"BBCC\",\"12345678\"\r\n"				//设置SID和PWD连接网络
#define TCP_CIPSTART (uint8_t*)"AT+CIPSTART=\"TCP\",\"192.168.4.3\",8080\r\n"	//设置为TCP协议传输并指定IP和端口号
#define CIPMODE      (uint8_t*)"AT+CIPMODE=1\r\n"																//透传
#define CIPSEND      (uint8_t*)"AT+CIPSEND\r\n"																	//开始传输
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


