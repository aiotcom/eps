/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-12-19
*
*	版本： 		V2.0
*
*	说明： 		WiFi实验
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/

#include <ioCC2530.h>
#include "Usart.h"
#include "Rs485.h"
#include "delay.h"
#include "Lamp.h"
#include "Time.h"
#include "WiFi.h"
#include "stdio.h"
#include "string.h"
//==========================================================
//	函数名称：	main()
//
//	函数功能：	程序入口函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
uint8 temp=10,humi=20;
uint16 Linght_Intensity = 1000;
uint8 Buffer_Tmp[32];
long Tick_SendOneNET = 0;
void main(void)
{
    Hal_Init_32M();     //初始化32M时钟
    USRT0_Init();       //初始化串口0
		Init_Timer1();			//定时器1初始化，中断周期1ms。为发送数据到OneNET提供时间基准。
    WiFi_Init();        //初始化WiFi模块    
	  Tick_SendOneNET = Time1_GetTicks()+2000;//2秒发送一次
    while(1)
    {
				if(Time1_GetTicks() > Tick_SendOneNET){
					/*转化成字符串*/
					sprintf((void*)&Buffer_Tmp[0],"%02d%02d%04d",temp,humi,Linght_Intensity);
					USART0_SendString(Buffer_Tmp,strlen((char const*)Buffer_Tmp));//串口发送到OneNET 
					
					Tick_SendOneNET = Time1_GetTicks()+2000;//2秒发送一次
					
					/*temp,humi,Linght_Intensity为模拟数据*/
					temp++;
					humi++;
					temp = temp%100;//等于100，清零
					humi = humi%100;//等于100，清零
					Linght_Intensity += 50;
					if(Linght_Intensity>2000){//大于2000，赋值1000
						Linght_Intensity = 1000;
					}
				}
    }
}
