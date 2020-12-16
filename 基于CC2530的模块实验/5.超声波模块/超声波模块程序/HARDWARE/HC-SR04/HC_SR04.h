#ifndef __HC_SR04_H
#define __HC_SR04_H 			   

#include "stm32f1xx_hal.h" 
#include "stm32f1xx.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define  	HCSR04_GPIO_PORT1  	GPIOA
#define  	HCSR04_GPIO_PORT2  	GPIOA
#define  	HCSR04_GPIO_PORT3  	GPIOB
#define  	HCSR04_GPIO_PORT4  	GPIOA

#define		Trigl1							GPIO_PIN_7
#define		Trigl2							GPIO_PIN_5		
#define		Trigl3							GPIO_PIN_7		
#define		Trigl4							GPIO_PIN_0		

#define		Echo1								GPIO_PIN_6		
#define		Echo2								GPIO_PIN_4
#define		Echo3								GPIO_PIN_6
#define		Echo4								GPIO_PIN_1	

#define 	Trigl1_HIGH()				HAL_GPIO_WritePin(HCSR04_GPIO_PORT1,Trigl1,GPIO_PIN_SET);
#define 	Trigl1_LOW()				HAL_GPIO_WritePin(HCSR04_GPIO_PORT1,Trigl1,GPIO_PIN_RESET);

#define 	Trigl2_HIGH()				HAL_GPIO_WritePin(HCSR04_GPIO_PORT2,Trigl2,GPIO_PIN_SET);
#define 	Trigl2_LOW()				HAL_GPIO_WritePin(HCSR04_GPIO_PORT2,Trigl2,GPIO_PIN_RESET);

#define 	Trigl3_HIGH()				HAL_GPIO_WritePin(HCSR04_GPIO_PORT3,Trigl3,GPIO_PIN_SET);
#define 	Trigl3_LOW()				HAL_GPIO_WritePin(HCSR04_GPIO_PORT3,Trigl3,GPIO_PIN_RESET);

#define 	Trigl4_HIGH()				HAL_GPIO_WritePin(HCSR04_GPIO_PORT4,Trigl4,GPIO_PIN_SET);
#define 	Trigl4_LOW()				HAL_GPIO_WritePin(HCSR04_GPIO_PORT4,Trigl4,GPIO_PIN_RESET);

#define 	Echo1_STATE()    		HAL_GPIO_ReadPin(HCSR04_GPIO_PORT1,Echo1)	//读取返回电平
#define 	Echo2_STATE()    		HAL_GPIO_ReadPin(HCSR04_GPIO_PORT2,Echo2)	//读取返回电平
#define 	Echo3_STATE()    		HAL_GPIO_ReadPin(HCSR04_GPIO_PORT3,Echo3)	//读取返回电平
#define 	Echo4_STATE()    		HAL_GPIO_ReadPin(HCSR04_GPIO_PORT4,Echo4)	//读取返回电平

extern void     HCSR04_Init(void); 											//初始化超声波模块
extern uint16_t HCSR04_StartMeasure(uint8_t Num);				//超声波模块测距

#endif

