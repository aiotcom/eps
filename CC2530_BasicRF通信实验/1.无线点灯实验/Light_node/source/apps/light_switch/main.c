/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		
*
*	日期： 		2018-10-30
*
*	版本： 		V1.0
*
*	说明： 		实验操作：
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
#include <hal_lcd.h>
#include <hal_led.h>
#include <hal_joystick.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <ioCC2530.h>
#include <hal_int.h>
#include "hal_mcu.h"
#include "hal_button.h"
#include "hal_rf.h"
#include "util_lcd.h"
#include "basic_rf.h"
#include "stdlib.h"
#include "string.h"
#include "delay.h"
#include "stdio.h"
#include "KEY.h"
#include "LED.h"

#define RF_CHANNEL                25      // 2.4 GHz RF channel
#define PAN_ID                0x2007
#define SWITCH_ADDR           0x2520
#define LIGHT_ADDR            0xBEEF

#define APP_PAYLOAD_LENGTH    0x02 //收发的数据长度
#define CMD_LIGHT_CTRL        0x01 //灯光控制命令


static uint8 pRxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;
//==========================================================
//	函数名称：	appLight()
//
//	函数功能：	数据接收函数，接收其他模块发送的数据
//
//	入口参数：	@ PARAM BaseCfFiFig--文件范围变量。基本射频配置数据
//                        RX数据缓冲区指针
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void appLight()
{
    basicRfConfig.panId      = PAN_ID;     //PANID
    basicRfConfig.channel    = RF_CHANNEL; //信道
    basicRfConfig.ackRequest = TRUE;			 //需要ACK回复        
    basicRfConfig.myAddr     = LIGHT_ADDR;  // 设置本机地址
    
    if(basicRfInit(&basicRfConfig)==FAILED) 
    {//如果配置失败
      HAL_ASSERT(FALSE);
    }
    //开启RF接收
    basicRfReceiveOn();
    
    while (TRUE)
    {
        while(!basicRfPacketIsReady());   //开启接收

        if(basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL)>0)  //接收发送到该地址的数据
        {
						if(pRxData[0] == CMD_LIGHT_CTRL){//是灯控制命令
								switch(pRxData[1])
								{
									case S1_PRES:  //按键1
										MCU_IO_TGL_PREP(0,2);//P0.2 IO输出反转
										break;
									case S2_PRES:  //按键2
										MCU_IO_TGL_PREP(0,3);//P0.3 IO输出反转
										break;
									case S3_PRES:  //按键3
										MCU_IO_TGL_PREP(1,6);//P1.6 IO输出反转
										break;
									case S4_PRES:  //按键4
										MCU_IO_TGL_PREP(1,7);//P1.7 IO输出反转
										break;
									default: break;
								}
								/*清空缓冲*/
								pRxData[0] = 0;
								pRxData[1] = 0;
						}
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
    halBoardInit(); //单片机初始化
		LED_Init();     //LED模块上的LED灯控制IO初始化
		
    // Initalise hal_rf 硬件抽象层的 rf 进行初始化
    if(halRfInit()==FAILED)
    {//如果配置失败
      HAL_ASSERT(FALSE);
    }
    
    appLight();         //无线控制命令接收处理
}