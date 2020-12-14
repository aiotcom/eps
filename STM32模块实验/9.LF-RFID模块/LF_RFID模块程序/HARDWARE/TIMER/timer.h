#ifndef _TIMER_H
#define _TIMER_H
#include "stm32f1xx.h"

void TIM3_Init(uint16_t arr,uint16_t psc,void (*cb)(void));
void TIM2_Init(unsigned int arr,unsigned int psc);
uint8_t TIM2_Delay(uint16_t nus);

extern TIM_HandleTypeDef TIM2_Handler;      //¶¨Ê±Æ÷¾ä±ú 
extern uint8_t TIM2_IntFlag;
#endif

