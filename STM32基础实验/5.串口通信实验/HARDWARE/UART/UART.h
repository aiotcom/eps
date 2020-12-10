#ifndef __USART_H
#define __USART_H
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

void UART2_Init(int bound);				//´®¿Ú2³õÊ¼»¯
extern UART_HandleTypeDef UART2_Handler; 
#endif

