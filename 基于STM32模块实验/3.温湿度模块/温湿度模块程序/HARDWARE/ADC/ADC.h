#ifndef _ADC_H
#define _ADC_H

#include "stm32f1xx_hal.h"

uint16_t  Get_Adc(uint32_t ch);	//获得某个通道值 
uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);//得到某个通道给定次数采样的平均值
void ADC_Init(void); 						//ADC通道初始化

#endif

