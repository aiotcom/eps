#ifndef __USART_H
#define __USART_H(null)

#include "stm32f1xx.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define UART1  1
#define UART2  1

#if UART1

#define USART1_REC_LEN  			200  										//定义最大接收字节数 200
#define EN_USART1_RX 			1														//使能（1）/禁止（0）串口1接收
	  	
extern unsigned char  USART1_RX_BUF[USART1_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned int USART1_RX_STA;         						//接收状态标记	
extern UART_HandleTypeDef UART1_Handler; 							//UART句柄

#define UART1_RXBUFFERSIZE   1 												//缓存大小
extern unsigned char UART1_RxBuffer[UART1_RXBUFFERSIZE];			//HAL库USART接收Buffer

void USART1_IRQHand(void); 														//串口1中断服务函数
//如果想串口中断接收，请不要注释以下宏定义
void UART1_Init(int bound);
void USART3_Init(int baud);
#endif



#if UART2

#define UART_RX_DONE_MASK   0X8000
#define USART2_REC_LEN  	600  												//定义最大接收字节数 200
#define EN_USART2_RX 			1														//使能（1）/禁止（0）串口1接收
extern uint8_t USART_AT_STA;         									//AT指令发送完成标志位
extern uint8_t BLE_FIG;         											//蓝牙已连接标志位
	  	
extern unsigned char  USART2_RX_BUF[USART2_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned int USART2_RX_STA;         						//接收状态标记	
extern UART_HandleTypeDef UART2_Handler; 							//UART句柄

#define UART2_RXBUFFERSIZE   1 												//缓存大小
extern unsigned char UART2_RxBuffer[UART2_RXBUFFERSIZE];//HAL库USART接收Buffer
void USART2_IRQHand(void);														//串口二中断处理函数

uint8_t* ble_check_cmd(uint8_t *str);									//ATK-ESP8266发送命令后,检测接收到的应答
void ble_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime);	//向ATK-ESP8266发送命令
//如果想串口中断接收，请不要注释以下宏定义
void UART2_Init(int bound);

#endif


#endif



