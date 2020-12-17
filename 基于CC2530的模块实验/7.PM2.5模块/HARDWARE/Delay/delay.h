#ifndef __DELAY_H
#define __DELAY_H 			   

#include <ioCC2530.h>
#include "typedef.h"

extern void Hal_Init_32M(void);
extern void delay_ms(uint32_t nms);
extern void delay_us(uint32_t nus);

#endif





























