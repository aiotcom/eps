#include "stm32f1xx.h"
#include "led.h"
#include "key.h"

/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2020-01-19
*
*	版本： 		V2.0
*
*	说明： 		IO输入函数操作实验-HAL库版本
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/

int main(void)
{
    static uint8_t key = 0;  
    HAL_Init();       //初始化HAL库，为随后用到的HAL_Delay()函数提供时钟。
    LED_Init();				//初始化LED灯控制的IO口	
    KEY_Init();				//初始化按键输入的IO口

    while(1)
    {
        key = KEY_Scan();//获取按键      
        switch(key)
        {
              case S1_PRES:
                HAL_GPIO_TogglePin(LED_PORT,LED1_PIN);	//S1 按下，翻转LED1灯状态
                break;
              case S2_PRES:
                HAL_GPIO_TogglePin(LED_PORT,LED2_PIN);	//S2 按下，翻转LED2灯状态
                break;
              case S3_PRES:
                HAL_GPIO_TogglePin(LED_PORT,LED3_PIN);	//S3 按下，翻转LED3灯状态
                break;
              case S4_PRES:
                HAL_GPIO_TogglePin(LED_PORT,LED4_PIN);	//S4 按下，翻转LED4灯状态
                break;
              default:break;
        }
    }
}



