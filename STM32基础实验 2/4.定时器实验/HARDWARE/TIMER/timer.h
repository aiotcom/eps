#ifndef _TIMER_H
#define _TIMER_H
#include "stm32f1xx.h"

extern uint32_t Time_Count;
extern TIM_HandleTypeDef TIM2_Handler;      //¶¨Ê±Æ÷¾ä±ú 
extern void TIM2_Init(unsigned int arr,unsigned int psc);

#endif

