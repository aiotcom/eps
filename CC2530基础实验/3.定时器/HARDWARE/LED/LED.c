#include "delay.h"
#include "Time.h"
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
   LED_OFF();
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

//==========================================================
//	函数名称：	Water_Lamp
//
//	函数功能：	LED流水灯
//
//	入口参数：	time:控制流水灯时间间隔
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Water_Lamp(uint16_t time)
{
  static uint8_t Time_Flag = 1;		//执行标志位
  static uint16_t Time_Count = 0;	//计数值
  
  switch(Time_Flag)
  {
    case 1://控制LED1
      LED1_ON();
      delay_ms(1);
      Time_Count++;

      if(Time_Count>=time)
      {
        LED1_OFF();
        Time_Flag = 2;
        Time_Count = 0;
      }
      break;
    case 2://控制LED2
      LED2_ON();
      delay_ms(1);
      Time_Count++;

      if(Time_Count>=time)
      {
        LED2_OFF();
        Time_Flag = 3;
        Time_Count = 0;
      }
      break;
    case 3://控制LED4 
      LED4_ON();
      delay_ms(1);
      Time_Count++;

      if(Time_Count>=time)
      {
        LED4_OFF();
        Time_Flag = 4;
        Time_Count = 0;
      }
      break;
    case 4://控制LED3
      LED3_ON();
      delay_ms(1);
      Time_Count++;

      if(Time_Count>=time)
      {
        LED3_OFF();
        Time_Flag = 1;
        Time_Count = 0;
      }
      break;
    default: break;
  }
}