#ifndef __Fan_H
#define __Fan_H 			   

#include "hal_types.h"
#define Fan P0_1            //定义P0.1口为LED7控制端
#include <ioCC2530.h>

extern void Fan_Init(void); //风扇初始化
extern void FanOn(void);
extern void FanOff(void);
#endif

