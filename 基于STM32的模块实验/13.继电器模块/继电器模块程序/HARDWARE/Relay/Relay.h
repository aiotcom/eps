#ifndef __Relay_H
#define __Relay_H

#include "stm32f1xx_hal.h"

#define 	RELAY1_PORT 		GPIOA
#define 	RELAY2_PORT 		GPIOA

#define 	RELAY1_PIN 			GPIO_PIN_3
#define 	RELAY2_PIN 			GPIO_PIN_2

#define  RELAY1_OPEN()  	HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, GPIO_PIN_SET); 		//RL1开启
#define  RELAY1_CLOSE()   HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, GPIO_PIN_RESET);	  //RL1关闭

#define  RELAY2_OPEN()  	HAL_GPIO_WritePin(RELAY2_PORT, RELAY2_PIN, GPIO_PIN_SET);  		//RL2开启
#define  RELAY2_CLOSE()   HAL_GPIO_WritePin(RELAY2_PORT, RELAY2_PIN, GPIO_PIN_RESET);  	//RL2关闭

void Relay_Config(uint8_t fag0);  //控制继电器开关

void Relay_Init(void);  //继电器初始化

#endif /*__Bsp_Relay_H */


