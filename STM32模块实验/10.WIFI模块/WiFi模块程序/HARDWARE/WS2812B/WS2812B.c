#include "stm32f1xx_hal.h"
//==========================================================
//	函数名称：	static void delay(uint32_t n) 
//
//	函数功能：	WS2812B 控制过程中的延时函数
//
//	入口参数：	n,延时单位时间
//              
//	返回参数：	无
//
//	说明：	无	
//==========================================================
static void delay(uint32_t delay)   
{
    int i=0;
    for(i = 0;i<delay;i++)
    {;}
}

//==========================================================
//	函数名称：	void RGB_Set_UP(void) 
//
//	函数功能：	输出WS2812B 控制信号高电平“1”的电信号
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：	/*  1码  850ns高电平  400ns低电平*/	
//==========================================================
void RGB_Set_UP(void)
{ 
	/*
			写高电平
	*/
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
	/*
			写低电平
	*/
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
}
//==========================================================
//	函数名称：	void RGB_Set_UP(void) 
//
//	函数功能：	输出WS2812B 控制信号高电平“1”的电信号
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：	    /*  0码  400ns高电平  850ns低电平*/
//==========================================================
void RGB_Set_Down()
{
	/*
			写高电平
	*/
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
		GPIOA->BSRR = 0x80;
	/*
			写低电平
	*/
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
		GPIOA->BRR = 0x80;
}
//==========================================================
//	函数名称：	void RGB_REST(void)
//
//	函数功能：	WS2812B 初始化信号
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：	   
//==========================================================
void RGB_REST(void)
{
	GPIOA->BSRR = GPIO_PIN_7;
	delay(8000);  //延时58us
}
//==========================================================
//	函数名称：	void WS2812B_SetColour(uint32_t G8R8B8)
//
//	函数功能：	设置WS2812B色彩
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：	    /*  0码  400ns高电平  850ns低电平*/
//==========================================================
void WS2812B_SetColour(uint32_t G8R8B8)
{
	int byte = 0;
	int i = 0;
	for(i = 23;i>=0;i--)
	{
		byte = ((G8R8B8 >> i) & 0x01);
		if(byte == 1)
		{
			RGB_Set_UP();//1
		}
		else
		{
			RGB_Set_Down(); //0
		}
	}
}
//==========================================================
//	函数名称：	WS2812B_Init()
//
//	函数功能：	初始化WS2812B上的彩灯控制管脚，设置为推挽输出
//
//	入口参数：	void
//              
//	返回参数：	无
//
//	说明：	该WS2812B 灯 位WIFI模块上	
//==========================================================
void WS2812B_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();              //开启GPIOA时钟

    GPIO_Initure.Pin   = GPIO_PIN_7;           //GPIO_PIN_6:WS2812B彩灯控制管脚。
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;  //低速
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
    
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);//初始电平均为高
}


