#include "led.h"
#include "stm32f1xx.h"
//==========================================================
//	函数名称：	LED_Init
//
//	函数功能：	LED灯初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		设置LED灯控制IO输出。
//==========================================================
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
  
    LED_IO_RCC();

    GPIO_Initure.Pin   = LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN;
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  	  //推挽输出
    GPIO_Initure.Pull  = GPIO_PULLUP;          	  //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	  //高速
    HAL_GPIO_Init(LED_PORT,&GPIO_Initure);
	
    LED_OFF();///初始化完成全部LED灯灭
}


