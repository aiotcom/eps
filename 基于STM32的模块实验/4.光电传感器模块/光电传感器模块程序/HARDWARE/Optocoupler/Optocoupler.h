#ifndef _Optocoupler_H
#define _Optocoupler_H

#include "stm32f1xx.h"

#define 	Optocoupler_PORT 		GPIOA

#define 	Optocoupler_Switch 	GPIO_PIN_0
#define 	Optocoupler_State 	GPIO_PIN_1

#define 	Optocoupler_State_Read()   	HAL_GPIO_ReadPin(Optocoupler_PORT,Optocoupler_State)  //读取光电传感器状态

#define 	Optocoupler_Switch_ON()			HAL_GPIO_WritePin(Optocoupler_PORT,Optocoupler_Switch,GPIO_PIN_RESET)	//开启光电传感器
#define 	Optocoupler_Switch_OFF()		HAL_GPIO_WritePin(Optocoupler_PORT,Optocoupler_Switch,GPIO_PIN_SET)		//关闭光电传感器

extern void Optocoupler_Init(void);//光电传感器初始化

#endif


