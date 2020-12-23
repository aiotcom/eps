#include "beep.h"
//==========================================================
//	函数名称：	Beep_Init()
//
//	函数功能：	初始化蜂鸣器控制IO
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void Beep_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           				//开启GPIOA时钟

    GPIO_Initure.Pin   = GPIO_PIN_7; 						//PA7
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull  = GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	//高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
  
    BEEP_IO_LOW();//蜂鸣器IO默认输出低
}
//==========================================================
//	函数名称：	void Beep_On(uint16_t ms)
//
//	函数功能：	依据参数ms，蜂鸣器响ms时间
//
//	入口参数：	@ms,蜂鸣器响动时间长度
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void Beep_ON(uint16_t ms)
{
    while(ms){
        ms--;
        HAL_Delay(1);
        BEEP_IO_TOGGLE();
    }
}
//==========================================================
//	函数名称：	void Beep_OFF(void)
//
//	函数功能：	停止蜂鸣器
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void Beep_OFF(void)
{
    BEEP_IO_LOW();
}
