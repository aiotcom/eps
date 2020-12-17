#ifndef __DELAY_H
#define __DELAY_H 			   

#include <ioCC2530.h>

#define   uint8_t    unsigned char
#define   uint16_t   unsigned int
#define   uint32_t   unsigned long int

#define   u8    uint8_t
#define   u16   uint16_t
#define   u32   uint32_t

extern void delay_ms(uint32_t nms);
extern void delay_us(uint32_t nus);

#endif





























