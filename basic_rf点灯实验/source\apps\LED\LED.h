#ifndef __LED_H
#define __LED_H 			   

#include <ioCC2530.h>
#include "hal_cc8051.h"

#define   LED1  P0_2        //定义P0.2口为LED1控制端
#define   LED2  P0_3        //定义P0.3口为LED2控制端
#define   LED3  P1_6        //定义P1.6口为LED3控制端
#define   LED4  P1_7        //定义P1.7口为LED4控制端

#define   LED1_SET()    MCU_IO_SET_HIGH(0,2);      //定义P0.2口为LED1控制端
#define   LED1_RESET()  MCU_IO_SET_LOW(0,2);       //定义P0.2口为LED1控制端

#define   LED2_SET()    MCU_IO_SET_HIGH(0,3);      //定义P0.3口为LED2控制端
#define   LED2_RESET()  MCU_IO_SET_LOW(0,3);       //定义P0.3口为LED2控制端

#define   LED3_SET()    MCU_IO_SET_HIGH(1,6);      //定义P1.6口为LED3控制端
#define   LED3_RESET()  MCU_IO_SET_LOW(1,6);       //定义P1.6口为LED3控制端

#define   LED4_SET()    MCU_IO_SET_HIGH(1,7);      //定义P1.7口为LED4控制端
#define   LED4_RESET()  MCU_IO_SET_LOW(1,7);       //定义P1.7口为LED4控制端

extern void LED_Init(void); //LED初始化
extern void LED_Set(void);  //LED控制

#endif

