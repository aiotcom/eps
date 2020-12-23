//#include "delay.h"
#include "bottom_led.h"
#include "delay.h"
#include <ioCC2530.h> 
long int rgb = 0x070707;  //需要变换的颜色
uint16 fig = 0,num = 23;    
void RGB_REST(void);
void Set_White(void);
void bottom_led(void);
void _delayunit(void);
void RGB_Set_UP();
void RGB_Set_Down(void);
void Bottom_Green(void);
void Bottom_Red(void);
void Bottom_Blue(void);
void Bottom_Yellow(void);
void Bottom_BlackOut(void);
void RGB_Set_UP(void);
/***********************************************************************************
* 函数名称        InitLed
*
* 函数功能        设置LED灯相应的IO口
*
* 入口参数        无
*
* 返回参数        无 
*
* 说明
***********************************************************************************/
void bottom_led_init(void)
{
    P2DIR |= 0x01;      //P1.0定义为输出
    RGB_REST();
}
void _delayunit(void)
{
  uint8 i=0;
  for(i=0;i<2;i++)
  {
  
  }
}
/*  1码 850ns高 400ns低*/
void RGB_Set_UP(void)
{
      LED = 1;      //1
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
      LED = 0;
}

/*  0码  400ns高  850ns低*/
void RGB_Set_Down(void)
{
      LED = 1;     //0
      asm("NOP");
      asm("NOP");
      LED = 0;
      asm("NOP");
      asm("NOP");
}

#define SEND_HI {LED = 1;LED = 1;LED = 1;LED = 1;LED = 1;LED = 1;LED = 1;LED = 1;LED = 1;LED = 0;LED = 0;LED = 0;LED = 0;LED = 0;}

#define SEND_LO {LED = 1;LED = 1;LED = 1;LED = 1;LED = 1;LED = 0;LED = 0;LED = 0;LED = 0;LED = 0;LED = 0;LED = 0;LED = 0;LED = 0;}
//#define SEND_HI {LED = 1;LED = 1;LED = 1;LED = 1;LED = 0;LED = 0;LED = 0; }
//
//#define SEND_LO {LED = 1;LED = 1;LED = 1;LED = 0;LED = 0;LED = 0;LED = 0; }
void Bottom_White(void)
{
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;

  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;  
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;  
}

void Bottom_Green(void)
{
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;  
}

void Bottom_Red(void)
{ 
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;  
}
void Bottom_Blue(void)
{ 
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
   
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO; 
  
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;  
}
void Bottom_Yellow(void)
{ 
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;
  SEND_HI;  
  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;  
}
void Bottom_BlackOut(void)
{ 
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;  
  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;   
  
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;
  SEND_LO;  
}
/***********************************************************************************
* 函数名称        RGB_SET()
*
* 函数功能        设置颜色 
*
* 入口参数        G8R8B8： 24位RGB颜色
*
* 返回参数        无 
*
* 说明
***********************************************************************************/


/* 拉低电平等待下一次命令  */
void RGB_REST(void)
{
    LED = 0;
    delay_ms(1);  
}
