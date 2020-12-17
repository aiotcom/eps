#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include <ioCC2530.h>
#include "delay.h"
#define USART0_RX_SIZE 100
extern uint8_t USART0_RX_STA;
extern uint8_t USART0_RX_BUF[USART0_RX_SIZE];

extern void Init_Uart0();                     //串口0初始化
extern void UR0SendByte(unsigned char dat);   //串口0发送单字节
extern void UR0SendString(unsigned char *str);//串口0发送字符串
extern uint8_t* ble_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime);

#endif



