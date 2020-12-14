#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f1xx_hal.h"

extern uint32_t number;
extern TIM_HandleTypeDef TIM3_Handler;      //¶¨Ê±Æ÷¾ä±ú 
void TIM_ESP8266_FIG(uint8_t f);
void TIM3_Init(uint16_t arr,uint16_t psc);
#endif

