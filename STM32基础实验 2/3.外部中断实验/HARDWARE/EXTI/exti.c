#include "led.h"
#include "key.h"
#include "exti.h"
#include "stm32f1xx.h"

unsigned char Key_flag = 0;  //按键标志

//==========================================================
//	函数名称：	EXTI4_Init
//
//	函数功能：	外部中断初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void EXTI4_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();              //开启GPIOA时钟

    GPIO_Initure.Pin  = S2_PIN; 					     //按键S1
    GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;   //下降沿触发,按键无按下时，IO口为高电平。
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(S2_PORT,&GPIO_Initure);

		HAL_NVIC_SetPriority(EXTI4_IRQn,1,1);      	//抢占优先级为1，子优先级为1
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);            	//使能中断线	
}
