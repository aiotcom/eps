#ifndef __WiFi_H
#define __WiFi_H 			   

#include <ioCC2530.h>
#include "delay.h"

#define WiFi_RES P1_5  //定义P1.5口为RES控制端
#define WiFi_LED P1_6  //定义P1.6口为LED控制端
#define WiFi_KEY P1_7  //定义P1.7口为KEY控制端

#define WiFi_KEY_Read()   WiFi_KEY  //按键

#define WiFi_LED_HI {WiFi_LED = 1;WiFi_LED = 1;WiFi_LED = 1;WiFi_LED = 1;WiFi_LED = 1;\
        WiFi_LED = 1;WiFi_LED = 1;WiFi_LED = 1;WiFi_LED = 0;WiFi_LED = 0;WiFi_LED = 0;WiFi_LED = 0; }

#define WiFi_LED_LO {WiFi_LED = 1;WiFi_LED = 1;WiFi_LED = 1;WiFi_LED = 1;WiFi_LED = 0;\
        WiFi_LED = 0;WiFi_LED = 0;WiFi_LED = 0;WiFi_LED = 0;WiFi_LED = 0;WiFi_LED = 0;WiFi_LED = 0; }

#define	CLOSE_TCP     "+++"						//关闭TCP连接，退出透传模式
#define AT            "AT\r\n"
#define CWMODE        "AT+CWMODE=3\r\n"					//设置模式为AP+STA
#define RST           "AT+RST\r\n"					//软重启模块
#define CIFSR         "AT+CIFSR\r\n"					//查询IP地址
#define CWJAP         "AT+CWJAP=\"bacheng\",\"Stepiot123\"\r\n"		//设置SID和PWD连接网络
#define TCP_CIPSTART  "AT+CIPSTART=\"TCP\",\"183.230.40.40\",1811\r\n"	//设置为TCP协议传输并指定IP和端口号
#define CIPMODE       "AT+CIPMODE=1\r\n"				//透传
#define CIPSEND       "AT+CIPSEND\r\n"					//开始传输
#define CIPSTATUS     "*378470#123456789#sample*"				//ONE.NET平台个人账号识别码

extern void WiFi_Init(void); //WiFi初始化
extern void WiFi_LED_REST(void);
extern void WiFi_Set_White(void);
extern void WiFi_LED_SET(uint32_t G8R8B8);

#endif

