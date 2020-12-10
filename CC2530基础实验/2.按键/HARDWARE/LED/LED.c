#include "delay.h"
#include "LED.h"

//==========================================================
//	函数名称：	LED_Init()
//
//	函数功能：	初始化LED
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void LED_Init(void)
{
   P0DIR |= 0x0C;      //P0.2、P0.3定义为输出
   P1DIR |= 0xC0;      //P1.6、P1.7定义为输出
   LED1=1;LED2=1;LED3=1;LED4=1;
}

//==========================================================
//	函数名称：	LED_Set()
//
//	函数功能：	LED控制
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void LED_Set(void)
{ 
  LED1=0;delay_ms(500);LED1=1;
  LED2=0;delay_ms(500);LED2=1;
  LED4=0;delay_ms(500);LED4=1;
  LED3=0;delay_ms(500);LED3=1;
  
  LED1=0;delay_ms(400);LED1=1;
  LED2=0;delay_ms(400);LED2=1;
  LED4=0;delay_ms(400);LED4=1;
  LED3=0;delay_ms(400);LED3=1;
  
  LED1=0;delay_ms(300);LED1=1;
  LED2=0;delay_ms(300);LED2=1;
  LED4=0;delay_ms(300);LED4=1;
  LED3=0;delay_ms(300);LED3=1;
  
  LED1=0;delay_ms(200);LED1=1;
  LED2=0;delay_ms(200);LED2=1;
  LED4=0;delay_ms(200);LED4=1;
  LED3=0;delay_ms(200);LED3=1;
  
  LED1=0;delay_ms(100);LED1=1;
  LED2=0;delay_ms(100);LED2=1;
  LED4=0;delay_ms(100);LED4=1;
  LED3=0;delay_ms(100);LED3=1;
  
  LED1=0;LED2=0;LED3=0;LED4=0;delay_ms(500);
  LED1=1;LED2=1;LED3=1;LED4=1;delay_ms(500);
  LED1=0;LED2=0;LED3=0;LED4=0;delay_ms(300);
  LED1=1;LED2=1;LED3=1;LED4=1;delay_ms(300);
}
