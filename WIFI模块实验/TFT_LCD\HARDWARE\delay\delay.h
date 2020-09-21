#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f1xx_hal.h"

void delay_ms(unsigned int nms);
void delay_us(unsigned int ums);
uint8_t PollDelay(uint32_t *tick_counter,uint32_t nms);
#endif





























