#ifndef __PIR_H
#define __PIR_H			   

#include <ioCC2530.h>

#define   PIR_LED  P0_3        //定义P0.3口为LED控制端

#define   PIR_LED_ON()      PIR_LED = 0	//点亮LED1
#define   PIR_LED_OFF()	    PIR_LED = 1 //关闭LED1

#define   PIR_Read()        P1_1  //人体红外使用P1_1

extern void PIR_Init(void); //LED初始化

#endif

