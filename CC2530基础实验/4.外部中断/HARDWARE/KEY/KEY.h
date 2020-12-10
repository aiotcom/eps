#ifndef _KEY_H
#define _KEY_H

#include <ioCC2530.h>

#define   S1    P1_5        //定义P1.5口为S1控制端
#define   S2    P1_4        //定义P1.4口为S2控制端
#define   S3    P0_0        //定义P0.0口为S3控制端
#define   S4    P0_1        //定义P0.1口为S4控制端

#define   S1_Read()   P1_5  //S1按键P1_5
#define   S2_Read()   P1_4  //S2按键P1_4
#define   S3_Read()   P0_0  //S3按键P0_0
#define   S4_Read()   P0_1  //S4按键P0_1

#define   S1_PRES     1
#define   S2_PRES     2
#define   S3_PRES     3
#define   S4_PRES     4

void KEY_Init(void);
void key_Operatio(void);
uint8_t KEY_Scan(uint8_t mode);

#endif