#include "stm32f1xx.h"   
#include "delay.h"

//==========================================================
//	函数名称：	delay_ms
//
//	函数功能：	毫秒延时函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void delay_ms(unsigned int nms)
{		
	unsigned int i = 0,j = 0;
	for(i = 0;i<nms;i++)
	{
		for(j = 0;j<10600;j++)
		{;}
	}
}
/*
轮询延时函数
*/
uint8_t PollDelay(uint32_t *tick_counter,uint32_t nms)
{    
    if(HAL_GetTick() >= *tick_counter)
    {
        *tick_counter = HAL_GetTick() + nms;
        return 1;
    }
    return 0;
}


void delay_us(unsigned int nus)
{		
	unsigned int j = 0;
	while(nus--)
	{
        j = 2;
		while(j--)
		{;}
	}
}







































