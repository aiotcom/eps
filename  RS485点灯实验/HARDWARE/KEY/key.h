#ifndef _KEY_H
#define _KEY_H

#include "stm32f1xx.h"

#define KEY1_PORT 	GPIOA
#define S1_PIN			GPIO_PIN_5

#define KEY2_PORT 	GPIOA
#define S2_PIN			GPIO_PIN_4

#define KEY3_PORT 	GPIOB
#define S3_PIN			GPIO_PIN_7

#define KEY4_PORT 	GPIOB
#define S4_PIN			GPIO_PIN_6

#define S1_Read()   HAL_GPIO_ReadPin(KEY1_PORT,S1_PIN)  //S1按键
#define S2_Read()   HAL_GPIO_ReadPin(KEY2_PORT,S2_PIN)  //S2按键
#define S3_Read()   HAL_GPIO_ReadPin(KEY3_PORT,S3_PIN)  //S3按键
#define S4_Read()   HAL_GPIO_ReadPin(KEY4_PORT,S4_PIN)  //S4按键

#define S1_PRES   	1
#define S2_PRES	  	2
#define S3_PRES   	3
#define S4_PRES   	4

void KEY_Init(void);
void key_Operatio(void);
uint8_t KEY_Scan(uint8_t mode);

#endif
