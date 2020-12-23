#include "led_mode.h"
/*
初始化LED模块上的S4->P1.6按键，LED2->P1.2按键的IO串口口
*/
void LedMode_Init(void)
{
    //LED灯IO口初始化
    P1SEL &= ~BV(2);     //设置P1.2为普通IO口  
    P1DIR |= BV(2);      //P1.2定义为输出

    //按键IO口初始化    
    P1SEL &= ~BV(6);     //设置P1.6为普通IO口  
    P1DIR &= ~BV(6);      //P1.6定义为输入 
    P1INP &= ~BV(6);     //打开P1.6上拉电阻
}
/*
LED模块上LED2点亮
*/
void Led2_On(void)   
{
  P1_2 = 0;
}
/*
LED模块上LED2熄灭
*/
void Led2_Off(void)  
{
  P1_2 = 1;
}
/*
读LED模块上的按键S4
*/
uint8 Key_ReadS4(void)
{
  return P1_6?KEY_RELASE:KEY_PRESS;
}