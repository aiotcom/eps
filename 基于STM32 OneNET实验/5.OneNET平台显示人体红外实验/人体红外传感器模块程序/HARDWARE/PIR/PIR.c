#include "PIR.h"
#include "delay.h"
#include "stm32f1xx.h"

uint8_t PIR_Sta[2];	//人体红外状态

//==========================================================
//	函数名称：	PIR_Init()
//
//	函数功能：	人体红外初始化函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void PIR_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟

    GPIO_Initure.Pin=PIR_PIN;	
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     	//低速
    HAL_GPIO_Init(PIR_PORT,&GPIO_Initure);
	
		GPIO_Initure.Pin=PIR_LED_PIN;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
    HAL_GPIO_Init(PIR_LED_PORT,&GPIO_Initure);
    PIR_LED_OFF();
}



