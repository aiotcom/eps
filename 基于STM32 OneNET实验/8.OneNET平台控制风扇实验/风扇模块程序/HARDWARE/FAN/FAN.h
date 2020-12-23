#ifndef _FAN_H
#define _FAN_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef TIM4_Handler;      //定时器句柄 

void TIM4_Init(uint16_t arr,uint16_t psc);
void TIM4_PWM_Init(uint16_t psc,uint16_t arr);
void TIM_SetTIM4Compare2(uint32_t compare);
extern void PWM_SetTIM4Compare2(uint32_t compare);//PWM设置占空比

#endif

