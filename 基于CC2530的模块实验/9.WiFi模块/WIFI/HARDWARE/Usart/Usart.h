#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include <ioCC2530.h>
#include "delay.h"

#define UART0  1
#define UART1  1

#if UART0

#define USART0_REC_LEN          200  		        //定义最大接收字节数 200

extern unsigned char WiFi_AT_STA;                       //WiFi初始化完成标志位
extern unsigned char USART0_RX_BUF[USART0_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned int  USART0_RX_STA;         		//接收状态标记	

extern uint8_t Temp;				        //温度数据
extern uint8_t Humi;				        //湿度数据
extern uint8_t Command_LED[ ];				//控制LED灯命令码
extern uint8_t Send_OneNET[ ];				//发送到OneNET显示数据

extern void USRT0_Init(void);                           //串口0初始化
extern void USART0_IRQHand(void); 	                //串口0服务函数
extern void USART0_Receive(uint8_t data);               //串口0接收处理
extern void USART0_SendByte(unsigned char dat);         //串口0发送单字节
extern void USART0_SendString(unsigned char *str,uint16_t Len);            //串口0发送字符串
extern uint8_t* WiFi_Send_Cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime);//向WiFi发送命令
#endif


#if UART1

#define USART1_REC_LEN          200  		        //定义最大接收字节数 200

extern unsigned char  USART1_RX_BUF[USART1_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned int   USART1_RX_STA;         		//接收状态标记

extern void USRT1_Init(void);                           //串口1初始化
extern void USART1_IRQHand(void); 	                //串口1服务函数
extern void USART_Data_Send(void);                      //串口数据发送函数
extern void USART1_SendByte(unsigned char dat);         //串口1发送单字节
extern void USART1_SendString(unsigned char *str,uint16_t Len);             //串口1发送字符串

#endif

#endif



