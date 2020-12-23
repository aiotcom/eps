#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "TM1640.h"	
#include "Rs485.h"
#include "SHT20.h"	
#include "delay.h"
#include "timer.h"
#include "Usart.h"
#include "Optocoupler.h"

/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-03-18
*
*	版本： 		V1.0
*
*	说明： 		温湿度模块
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
void RS485_HandlerCb(void);
uint8_t SendBuffer[3];
uint8_t ReadSensorState = 0;
uint8_t DetectResult = 0;
int main(void)
{
  uint8_t state = 0;     //显示温度 还是湿度
  HAL_Init();             					//初始化HAL库  
  Optocoupler_Init();               //初始化光电开关模块
  Rs485_Init();											//初始化485
  USART3_Init(115200);              //用于调试
  UART1_Init(115200);								//初始化串口1,RS485通信
  
  /*中断频率20HZ,关联RS485_HandlerCb()回调函数*/
  TIM3_Init(10000-1,320-1,RS485_HandlerCb);    
	while(1)
	{
      DetectResult = Optocoupler_State_Read();//=1,检测到遮挡物，0=未检测到
	}
}
//==========================================================
//	函数名称：	void RS485_HandlerCb(void)
//
//	函数功能：	通过485总线获取数据处理函数
//
//	入口参数：	void
//
//	返回参数：	无
//
//	说明：		接收通过485总线发送过来的数据请求
//==========================================================
void RS485_HandlerCb(void)
{
    if(!DataHandling_485(Addr_PH)){	//是本机期望的485数据处理
        printf("get requery\r\n");
        /*485发送数据*/
        Rs485_Send(Addr_PH,Addr_WiFi,PH_State,1,&DetectResult);
    }
}




