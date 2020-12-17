/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-07-13
*
*	版本： 		V1.0
*
*	说明： 		温湿度节点获取环境温湿度数据后发送给协调器
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/

#include <hal_joystick.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <ioCC2530.h>
#include <hal_int.h>
#include <hal_lcd.h>
#include <hal_led.h>
#include "hal_button.h"
#include "util_lcd.h"
#include "basic_rf.h"
#include "hal_mcu.h"
#include "OLEDIIC.h"
#include "hal_rf.h"
#include "TM1640.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "Usart.h"	
#include "delay.h"
#include "SHT20.h"
#include "OLED.h"
#include "IIC.h"

//基本参数参数 
#define RF_CHANNEL                25  // 2.4 GHz 信道 11~26
#define PAN_ID                0x2007  //PANID
#define Coordinator_ADDR      0x2520  //本机地址

#define APP_PAYLOAD_LENGTH        30
/***********************************************************************************
* LOCAL VARIABLES
*/
static uint8 pRxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;

static void Coordinator_Node(void); //协调器节点
//==========================================================
//	函数名称：	Coordinator_Node()
//
//	函数功能：	协调器节点
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void Coordinator_Node(void)
{
  uint8_t i = 0,Temp_Count = 0,Humi_Count = 0,Temp_Flag = 0,Humi_Flag = 0,Temp_Data[30],Humi_Data[30];
		    // 配置 basicRF
    basicRfConfig.panId      = PAN_ID;     //PAN_ID
    basicRfConfig.channel    = RF_CHANNEL; //信道
    basicRfConfig.ackRequest = TRUE;       //需要ACk
    basicRfConfig.myAddr = Coordinator_ADDR;    // 设置本机地址
    
    if(basicRfInit(&basicRfConfig)==FAILED) 
    {
      HAL_ASSERT(FALSE);
    }
    //开启接收
    basicRfReceiveOn();

    //程序进入死循环
    while (TRUE)
    {
				/*接收发送到该地址的数据*/
        if(basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL)>0)  
        {
          Temp_Count = 0,Humi_Count = 0,Temp_Flag = 0,Humi_Flag = 0;

          
          for(i=0;i<APP_PAYLOAD_LENGTH;i++)
          {
             //温度数据
            if(pRxData[i] == ',')
              Temp_Flag = 2;
            
            if(Temp_Flag == 1)
            {
              Temp_Data[Temp_Count] = pRxData[i];
              Temp_Count++;
            }
            
            if(pRxData[i] == ':' && Temp_Flag == 0)
              Temp_Flag = 1;

            //湿度数据
            if(Temp_Flag == 2)
            {
              if(pRxData[i] == '.')
                Humi_Flag = 2;
              
              if(Humi_Flag == 1)
              {
                Humi_Data[Humi_Count] = pRxData[i];
                Humi_Count++;
              }
              
              if(pRxData[i] == ':')
                Humi_Flag = 1;
            }
          }

          Temp_Data[2] = '\0';Humi_Data[2] = '\0';
          OLED_P8x16Str(45,3,Temp_Data);
          OLED_P8x16Str(45,6,Humi_Data);
          memset(Temp_Data,'\0',30);
          memset(Humi_Data,'\0',30);
        }
    }
}

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
void main(void)
{    
    // Initalise board peripherals 初始化外围设备
    halBoardInit();
		
    OLED_IIC_Init();  //初始化OLED屏IIC
    OLED_Init();      //初始化OLED屏
    OLED_Init_UI();   //初始化屏幕显示
		
    // Initalise hal_rf 硬件抽象层的 rf 进行初始化
    if(halRfInit()==FAILED)
    {
      HAL_ASSERT(FALSE);
    }
    Coordinator_Node(); //无线接收显示数据
}
