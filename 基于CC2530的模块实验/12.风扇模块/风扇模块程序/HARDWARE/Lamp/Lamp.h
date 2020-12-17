#ifndef __Lamp_H
#define __Lamp_H 			   

#include <ioCC2530.h> 

#define LED P2_0          //定义P2.0口为LED控制端

extern void Set_Red(void);
extern void Set_Blue(void);
extern void LampInit(void);
extern void Set_Green(void);
extern void Set_White(void);
extern void Set_Yellow(void);
extern void RGB_SET(long int G8R8B8);

#endif





























