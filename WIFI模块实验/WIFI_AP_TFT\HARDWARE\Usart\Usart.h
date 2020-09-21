#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "string.h"
#include "stm32f1xx.h"

#define UART1  1
#define UART2  1

#if UART1

#define USART1_REC_LEN  	200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1			//使能（1）/禁止（0）串口1接收
	  	
extern unsigned char  USART1_RX_BUF[USART1_REC_LEN]; 			//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned int USART1_RX_STA;        //接收状态标记	
extern UART_HandleTypeDef UART1_Handler; 	//UART句柄

#define UART1_RXBUFFERSIZE   1 						//缓存大小
extern unsigned char UART1_RxBuffer[UART1_RXBUFFERSIZE];	//HAL库USART接收Buffer

//如果想串口中断接收，请不要注释以下宏定义
void UART1_Init(int bound);
extern void USART1_Send_Data(uint8_t *IDE,double Data,uint16_t Len,uint8_t *NewLine);	//串口一发送数据
#endif



#if UART2

#define USART2_REC_LEN  		600  		//定义最大接收字节数 600
#define USART2_MAX_SEND_LEN		600		//最大发送缓存字节数 600
#define EN_USART2_RX 			1					//使能（1）/禁止（0）串口1接收
	  	
extern unsigned char  USART2_RX_BUF[USART2_REC_LEN]; 			//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned char  USART2_TX_BUF[USART2_MAX_SEND_LEN];	//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern unsigned int USART2_RX_STA;         			//接收状态标记	
extern UART_HandleTypeDef UART2_Handler; 				//UART句柄

#define UART2_RXBUFFERSIZE   1 									//缓存大小
extern unsigned char UART2_RxBuffer[UART2_RXBUFFERSIZE];			//HAL库USART接收Buffer

void UART2_Init(int bound);
void USART3_Init(int baud);
void PrintForBUffer(uint8_t *pp,uint8_t len);
#endif


#endif






