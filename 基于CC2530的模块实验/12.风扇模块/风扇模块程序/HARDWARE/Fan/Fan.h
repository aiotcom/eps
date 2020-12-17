#ifndef __Fan_H
#define __Fan_H 			   

#include <ioCC2530.h>

#define Fan P0_1            //定义P0.1口为LED7控制端

extern void Fan_Init(void); //风扇初始化
extern void Fan_Set(uint8_t state);//控制风扇

#endif

