#ifndef __USART2_H
#define __USART2_H
#include "stm32f1xx.h"
uint8_t Msg_Handler(void);
char AT_SendCmd(char* cmd, char *expected_result,uint8_t retry,uint16_t waittime);
#endif
