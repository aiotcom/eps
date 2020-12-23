#ifndef _BEEP_H
#define _BEEP_H
#include "stm32f1xx_hal.h"
#define BEEP_IO_HIGHT()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)
#define BEEP_IO_LOW()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)
#define BEEP_IO_TOGGLE() HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_7);

void Beep_Init(void);
void Beep_ON(uint16_t ms);
void Beep_OFF(void);
#endif
