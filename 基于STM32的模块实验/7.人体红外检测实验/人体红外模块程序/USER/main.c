#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "PIR.h"

/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-12-11
*
*	版本： 		V2.0
*
*	说明： 		人体红外模块
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
void Platform_RGB_LED_Init(void);
void Platform_RGB_LED_Red(void);
void Platform_RGB_LED_Blackout(void);
int main(void)
{
  HAL_Init();						//初始化HAL库 
  PIR_Init();						//初始化人体红外
  Platform_RGB_LED_Init();
	while(1)
	{
		if(PIR_Read())
		{
			PIR_LED_ON();
            Platform_RGB_LED_Red();
		}
		else
		{
			PIR_LED_OFF();
            Platform_RGB_LED_Blackout();
		}
	}
}
//==========================================================
//	函数名称：	Platform_RGB_LED_Init(void)
//
//	函数功能：	初始化底座的RGB LED灯的控制IO,PB3、PB4、PB15。控制输出电平占空比为50%，周期为1s。
//              该函数在systick中断处理函数中调用。
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Platform_RGB_LED_Init(void)
{    
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_AFIO_CLK_ENABLE();	
    __HAL_AFIO_REMAP_SWJ_NOJTAG(); 

    __HAL_RCC_GPIOB_CLK_ENABLE();           	                //开启GPIOB时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();           	                //开启GPIOB时钟

    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  	                //推挽输出
    GPIO_Initure.Pull  = GPIO_PULLUP;          	                //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	            //高速

    GPIO_Initure.Pin   = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15; 	//PB3,PB4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    GPIO_Initure.Pin   = GPIO_PIN_15; 	                        //PA15 	 	            
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    ////PB3,PB4,PA15 置1，默认初始化后灯灭 
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15,GPIO_PIN_SET);	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15                      ,GPIO_PIN_SET);        
}
//==========================================================
//	函数名称：	Platform_RGB_LED_Red(void)
//
//	函数功能：	将底座灯 点为红色
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Platform_RGB_LED_Green(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
}
//==========================================================
//	函数名称：	Platform_RGB_LED_Red(void)
//
//	函数功能：	将底座灯 点为绿色
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Platform_RGB_LED_Red(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3            ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4|GPIO_PIN_15,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15           ,GPIO_PIN_SET);    
}
//==========================================================
//	函数名称：	Platform_RGB_LED_Red(void)
//
//	函数功能：	将底座灯 点为蓝色
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Platform_RGB_LED_Blue(void)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15          ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_4,GPIO_PIN_SET);    
}
//==========================================================
//	函数名称：	void Platform_RGB_LED_Blackout(void)
//
//	函数功能：	将底座灯 点为蓝色
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Platform_RGB_LED_Blackout(void)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15          ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_4,GPIO_PIN_SET);    
}
