#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "stm32f1xx.h"

unsigned char  led_flag = 0;  //LED灯状态标志

//==========================================================
//	函数名称：	EXTI_Init
//
//	函数功能：	外部中断初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOA时钟

    GPIO_Initure.Pin=GPIO_PIN_7; 				//PA7
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,2);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //使能中断线

}

//==========================================================
//	函数名称：	EXTI9_5_IRQHandler
//
//	函数功能：	中断服务函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);		//调用中断处理公用函数
}

//==========================================================
//	函数名称：	HAL_GPIO_EXTI_Callback
//
//	函数功能：	中断服务程序
//
//	入口参数：	GPIO_Pin:中断引脚号
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(100);    
    
    led_flag = !led_flag; //标志取反
    
    
    if(led_flag)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET); 	//LED1对应引脚PB0拉低，亮，等同于LED1(0)	
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET); 	    //LED1对应引脚PB0拉高，灭，等同于LED1(1)		
    }
		
}
