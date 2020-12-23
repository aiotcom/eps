#ifndef __USART_H
#define __USART_H(null)

#include "stm32f1xx.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define UART1  1
#define UART2  0

#if UART1

#define USART1_REC_LEN  	200  												//定义最大接收字节数 200
#define EN_USART1_RX 			1														//使能（1）/禁止（0）串口1接收
#define UART1_RXBUFFERSIZE   1 												//缓存大小

extern unsigned char  USART1_RX_BUF[USART1_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t LDR_Data;															//光强数据
extern unsigned int USART1_RX_STA;         						//接收状态标记	
extern UART_HandleTypeDef UART1_Handler; 							//UART句柄
extern unsigned char UART1_RxBuffer[UART1_RXBUFFERSIZE];//HAL库USART接收Buffer

//如果想串口中断接收，请不要注释以下宏定义
void UART1_Init(int bound);
extern void Display_Data(void);

#endif

#if UART2

#define USART2_REC_LEN  	200  												//定义最大接收字节数 200
#define EN_USART2_RX 			1														//使能（1）/禁止（0）串口1接收
extern uint8_t USART_AT_STA;         									//AT指令发送完成标志位
	  	
extern unsigned char  USART2_RX_BUF[USART2_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned int USART2_RX_STA;         						//接收状态标记	
extern UART_HandleTypeDef UART2_Handler; 							//UART句柄

#define UART2_RXBUFFERSIZE   1 												//缓存大小
extern unsigned char UART2_RxBuffer[UART2_RXBUFFERSIZE];//HAL库USART接收Buffer

//如果想串口中断接收，请不要注释以下宏定义
void UART2_Init(int bound);

#endif
void USART3_Init(int baud);
#endif



