#ifndef __DELAY_H
#define __DELAY_H 			   

#include <ioCC2530.h>

#define   u8          unsigned char
#define   u16         unsigned int
#define   u32         unsigned long int

#define   uint8_t     u8
#define   uint16_t    u16
#define   uint32_t    u32

#define   uint8       uint8_t
#define   uint16      uint16_t
#define   uint32      uint32_t

extern void Hal_Init_32M(void);
extern void delay_ms(uint32_t nms);
extern void delay_us(uint32_t nus);

#endif





























