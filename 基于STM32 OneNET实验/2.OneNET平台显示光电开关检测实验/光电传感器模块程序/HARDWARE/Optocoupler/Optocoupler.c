#include "Optocoupler.h"
#include "stm32f1xx.h"

//==========================================================
//	函数名称：	Optocoupler_Init()
//
//	函数功能：	光电传感器初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Optocoupler_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    
    GPIO_Initure.Pin=Optocoupler_State;    	//Optocoupler_State
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(Optocoupler_PORT,&GPIO_Initure);
	
	  GPIO_Initure.Pin=Optocoupler_Switch; 		//Optocoupler_Switch
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	//高速
    HAL_GPIO_Init(Optocoupler_PORT,&GPIO_Initure);
	
	Optocoupler_Switch_ON();//初始化开启光电传感器
}





