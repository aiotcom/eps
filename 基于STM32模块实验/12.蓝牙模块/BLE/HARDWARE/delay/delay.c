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
		for(j = 0;j<6400;j++)
		{;}
	}
}


void delay_us(unsigned int nus)
{		
	unsigned int i = 0,j = 0;
	for(i = 0;i<nus;i++)
	{
		for(j = 0;j<1142;j++)
		{;}
	}
}


