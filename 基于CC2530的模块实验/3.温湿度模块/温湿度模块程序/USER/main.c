#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "TM1640.h"	
#include "SHT20.h"	
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
*	说明： 		温湿度模块
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
uint32_t Tick_Disp;
int main(void)
{
  uint8_t state = 0;     //显示温度 还是湿度
  
  HAL_Init();            //初始化HAL库
	SHT2x_Init();				  //初始化温湿度传感器SHT20
	TM1640_Init();				//初始化数码管驱动芯片TM1640
  
  /*HAL_GetTick(),返回值为单片机开机后运行时间值，单位为ms*/
  Tick_Disp = HAL_GetTick(); //HAL_GetTick(),HAL库函数，返回值以ms为单位。
	while(1)
	{
      if((state == 0) && (HAL_GetTick()>(Tick_Disp))){//更新温度
          state = 1;
          SHT2x_GetTempHumi();//读取一次温湿度，保存在g_sht2x_param.TEMP_HM
          send_LED_Display(0xC0,g_sht2x_param.TEMP_HM,1);//数码管显示
          
          Tick_Disp = HAL_GetTick()+1000/*1000ms*/;//设置下一次更新数据的时间点
      }
      else if((state == 1) && (HAL_GetTick()>(Tick_Disp))){//更新湿度
          state = 0;
          SHT2x_GetTempHumi();//读取一次温湿度,保存在g_sht2x_param.HUMI_HM        
          send_LED_Display(0xC0,g_sht2x_param.HUMI_HM,2);
          Tick_Disp = HAL_GetTick()+1000/*1000ms*/;//设置下一次更新数据的时间点        
      }
  }
}





