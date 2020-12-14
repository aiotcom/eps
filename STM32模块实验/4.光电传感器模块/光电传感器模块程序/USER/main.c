#include "Optocoupler.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "Rs485.h"
#include "usart.h"

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
*	说明： 		光电开关模块
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/

int main(void)
{
	HAL_Init();						//初始化HAL库    
	Optocoupler_Init();		//初始化光电传感器

	while(1)
	{
		if(Optocoupler_State_Read())	//读取光电状态
		{
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
		}
		else
		{
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
		}
	}
}




