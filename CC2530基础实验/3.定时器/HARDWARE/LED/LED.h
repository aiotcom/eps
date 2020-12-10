#ifndef __LED_H
#define __LED_H 			   

#include <ioCC2530.h>

#define   LED1  P0_2        //定义P0.2口为LED1控制端
#define   LED2  P0_3        //定义P0.3口为LED1控制端
#define   LED3  P1_6        //定义P1.6口为LED1控制端
#define   LED4  P1_7        //定义P1.7口为LED1控制端

#define   LED1_ON()     LED1 = 0	//点亮LED1
#define   LED1_OFF()	LED1 = 1        //关闭LED1

#define   LED2_ON()	LED2 = 0        //点亮LED2
#define   LED2_OFF()	LED2 = 1	//关闭LED2

#define   LED3_ON()	LED3 = 0        //点亮LED3
#define   LED3_OFF()	LED3 = 1	//关闭LED3

#define   LED4_ON()	LED4 = 0	//点亮LED4
#define   LED4_OFF()	LED4 = 1	//关闭LED4

#define   LED_ON()	LED1=0;LED2=0;LED3=0;LED4=0;  //点亮全部LED
#define   LED_OFF()	LED1=1;LED2=1;LED3=1;LED4=1;  //关闭全部LED

extern void LED_Init(void); //LED初始化
extern void Water_Lamp(uint16_t time);

#endif

