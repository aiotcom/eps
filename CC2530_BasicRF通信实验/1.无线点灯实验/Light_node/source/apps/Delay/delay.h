#ifndef __DELAY_H
#define __DELAY_H 			   

#include <ioCC2530.h>

#define   uint8_t    unsigned char
#define   uint16_t   unsigned int
#define   uint32_t   unsigned long int

extern void delay_ms(uint32_t nms);
extern void delay_us(uint32_t nus);

#endif





























