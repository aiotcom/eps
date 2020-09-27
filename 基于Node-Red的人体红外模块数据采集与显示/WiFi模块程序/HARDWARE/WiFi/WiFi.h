#ifndef __WiFi_H__
#define __WiFi_H__

#include "stm32f1xx_hal.h"

#define WiFi_RST_PIN 						GPIO_PIN_8
#define WiFi_RST_GPIO_RCC_EN() 	__HAL_RCC_GPIOA_CLK_ENABLE()
#define WiFi_RST_PORT 					GPIOA
#define WiFi_RST_High() 				HAL_GPIO_WritePin(WiFi_RST_PORT,WiFi_RST_PIN,GPIO_PIN_SET)
#define	CLOSE_TCP		 "+++"																						//关闭TCP连接，退出透传模式
#define AT         	 "AT\r\n"
#define CWMODE     	 "AT+CWMODE=3\r\n"																//设置模式为AP+STA
#define RST        	 "AT+RST\r\n"																			//软重启模块
#define CIFSR        "AT+CIFSR\r\n"			 															//查询IP地址
#define CWJAP        "AT+CWJAP=\"bacheng\",\"Stepiot123\"\r\n"			//设置SID和PWD连接网络
#define TCP_CIPSTART "AT+CIPSTART=\"TCP\",\"192.168.3.11\",9080\r\n"	//设置为TCP协议传输并指定IP和端口号
#define CIPMODE      "AT+CIPMODE=1\r\n"																//透传
#define CIPSEND      "AT+CIPSEND\r\n"																	//开始传输

extern uint8_t WiFi_AT_STA;    /*AT指令发送完成标志位*/
void WiFi_Init(void);

#endif


