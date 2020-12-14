#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f1xx_hal.h"

extern uint32_t Task_Count;
extern TIM_HandleTypeDef TIM3_Handler;      //¶¨Ê±Æ÷¾ä±ú 
extern void TIM3_Init(uint16_t arr,uint16_t psc,void (*cb)(void));
#endif

