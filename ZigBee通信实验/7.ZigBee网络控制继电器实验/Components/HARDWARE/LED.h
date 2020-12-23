#ifndef __LED_H
#define __LED_H 			   

#include "ioCC2530.h"
#include "hal_types.h"
#include "stdio.h"

#define   LED1  P0_2        //定义P0.2口为LED1控制端
#define   LED2  P0_3        //定义P0.3口为LED1控制端
#define   LED3  P1_6        //定义P1.6口为LED1控制端
#define   LED4  P1_7        //定义P1.7口为LED1控制端

#define   LED1_ON()     LED1 = 0	//点亮LED1
#define   LED1_OFF()	LED1 = 1    //关闭LED1

#define   LED2_ON()	LED2 = 0        //点亮LED2
#define   LED2_OFF()	LED2 = 1	//关闭LED2

#define   LED3_ON()	LED3 = 0        //点亮LED3
#define   LED3_OFF()	LED3 = 1	//关闭LED3

#define   LED4_ON()	LED4 = 0	//点亮LED4
#define   LED4_OFF()	LED4 = 1	//关闭LED4

#define   LED_ON()	LED1=0;LED2=0;LED3=0;LED4=0;  //点亮全部LED
#define   LED_OFF()	LED1=1;LED2=1;LED3=1;LED4=1;  //关闭全部LED


#define   S1    P1_5        //定义P1.5口为S1控制端
#define   S2    P1_4        //定义P1.4口为S2控制端
#define   S3    P0_0        //定义P0.0口为S3控制端
#define   S4    P0_1        //定义P0.1口为S4控制端

#define   S1_Read()   P1_5  //S1按键P1_5
#define   S2_Read()   P1_4  //S2按键P1_4
#define   S3_Read()   P0_0  //S3按键P0_0
#define   S4_Read()   P0_1  //S4按键P0_1

#define   S1_PRES     BV(0)
#define   S2_PRES     BV(1)
#define   S3_PRES     BV(2)
#define   S4_PRES     BV(3)
enum{
	KEY_RELASE,
	KEY_PRESSED
};
void LED_Init(void); //LED初始化
uint8_t KEY_Scan(void); //按键扫描
#endif

