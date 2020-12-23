#ifndef __RELAY_H
#define __RELAY_H 			   

#include <ioCC2530.h>

#define   Relay1  P0_2    //定义P0.2口为继电器1控制端
#define   Relay2  P0_3    //定义P0.3口为继电器2控制端

#define   Relay1_ON()   Relay1 = 1    //打开继电器1
#define   Relay1_OFF()	Relay1 = 0    //关闭继电器1

#define   Relay2_ON()	  Relay2 = 1    //打开继电器2
#define   Relay2_OFF()	Relay2 = 0    //关闭继电器2

extern void Relay_Init(void); //LED初始化

#endif

