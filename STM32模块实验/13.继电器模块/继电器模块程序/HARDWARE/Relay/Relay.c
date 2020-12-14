#include "Relay.h"

//==========================================================
//	函数名称：	Relay_Init
//
//	函数功能：	初始化485
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Relay_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           				//开启GPIOA时钟
	
		GPIO_Initure.Pin = RELAY1_PIN | RELAY2_PIN;			//PA3 PA2
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;				//推挽输出
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW; 			//低速
		HAL_GPIO_Init(RELAY1_PORT, &GPIO_Initure);
	
		RELAY1_CLOSE();RELAY2_CLOSE(); //初始化关闭继电器
}

//==========================================================
//	函数名称：	Relay_Config
//
//	函数功能：	开关继电器 
//
//	入口参数：	1(开)  0(关)
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Relay_Config(uint8_t fag0)
{
	HAL_Delay(10);
	if(fag0){
			RELAY1_OPEN();	 //打开继电器1
			RELAY2_OPEN();	 //打开继电器2
	}
	else{
			RELAY1_CLOSE();  //关闭继电器1
			RELAY2_CLOSE();  //关闭继电器2
	}
}



