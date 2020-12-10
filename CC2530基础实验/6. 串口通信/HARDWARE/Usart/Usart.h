#ifndef __USART_H
#define __USART_H 			   

#include <ioCC2530.h>
#include "delay.h"

#define USART0_RX_SIZE  200
extern uint32_t USART0_RX_STA;       //串口接收标志位
extern uint8_t USART0_RX_BUF[USART0_RX_SIZE];   //串口接收数组

extern void Init_Uart0();
extern void UR0SendByte(unsigned char dat);
extern void UR0SendString(unsigned char *str);

#endif

