#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "TM1640.h"	
#include "Rs485.h"
#include "SHT20.h"	
#include "delay.h"
#include "timer.h"
#include "Usart.h"
#include "ADC.h"

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
uint32_t Tick_Disp;
int main(void)
{
  uint8_t state = 0;     //显示温度 还是湿度
  HAL_Init();             					//初始化HAL库  
  Rs485_Init();											//初始化485
  SHT2x_Init();											//初始化SHT20
  TM1640_Init();										//初始化TM1640
  USART3_Init(115200);              //用于调试
  UART1_Init(115200);								//初始化串口1,RS485通信
  
  /*中断频率20HZ,关联RS485_HandlerCb()回调函数*/
  TIM3_Init(10000-1,320-1,RS485_HandlerCb);    
	while(1)
	{
      if((state == 0) && (HAL_GetTick()>(Tick_Disp))){//更新温度
          state = 1;
          SHT2x_GetTempHumi();//读取一次温湿度，保存在g_sht2x_param.TEMP_HM
          send_LED_Display(0xC0,g_sht2x_param.TEMP_HM,1);//数码管显示
          SendBuffer[0] = g_sht2x_param.TEMP_HM;//保存用于发送到WIFI节点
          Tick_Disp = HAL_GetTick()+1000/*1000ms*/;//设置下一次更新数据的时间点
      }
      else if((state == 1) && (HAL_GetTick()>(Tick_Disp))){//更新湿度
          state = 0;
          SHT2x_GetTempHumi();//读取一次温湿度,保存在g_sht2x_param.HUMI_HM        
          send_LED_Display(0xC0,g_sht2x_param.HUMI_HM,2);
          SendBuffer[1] = g_sht2x_param.HUMI_HM;//保存用于发送到WIFI节点        
          Tick_Disp = HAL_GetTick()+1000/*1000ms*/;//设置下一次更新数据的时间点        
      }
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
    if(!DataHandling_485(Addr_SHT20)){	//是本机期望的485数据处理
          printf("get requery\r\n");
          /*485发送数据*/
          Rs485_Send(Addr_SHT20,Addr_WiFi,SHT20_All,2,&SendBuffer[0]);
    }
}




