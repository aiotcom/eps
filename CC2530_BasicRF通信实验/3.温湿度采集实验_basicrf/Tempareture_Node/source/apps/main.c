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

//基本通信参数
#define RF_CHANNEL                25  // 2.4 GHz RF channel 值范围11~26
#define PAN_ID                0x2007 //PANID
#define Coordinator_ADDR      0x2520 //协调器的地址
#define SHT20_ADDR            0xBEEF //本机地址

#define APP_PAYLOAD_LENGTH        30

static uint8 pTxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;

static void SHT20_Node(void);   //温湿度节点

//==========================================================
//	函数名称：	SHT20_Node()
//
//	函数功能：	温湿度节点
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void SHT20_Node(void)
{ 
    // 配置 basicRF
    basicRfConfig.panId      = PAN_ID;      //设置PANID
    basicRfConfig.channel    = RF_CHANNEL; //设置信道
    basicRfConfig.ackRequest = TRUE;       //需要ACK
    basicRfConfig.myAddr     = SHT20_ADDR;    // 设置本机地址

    if(basicRfInit(&basicRfConfig)==FAILED)
    {
      HAL_ASSERT(FALSE);
    }

    //关闭接收
    basicRfReceiveOff();				
    // Main loop
    while (TRUE)
    {
				/*获取温湿度值.分别保存g_sht2x_param.TEMP_HM，g_sht2x_param.HUMI_HM*/
        SHT2x_GetTempHumi(); 
        send_LED_Display(0xC0,(uint16_t)g_sht2x_param.TEMP_HM,1);   //显示温度
				delay_ms(500);      //延时1秒    
        send_LED_Display(0xC0,(uint16_t)g_sht2x_param.HUMI_HM,2);  //显示湿度
				delay_ms(500);      //延时1秒   

				/*温湿度转化成字符串*/
        sprintf((char *)pTxData,"Temp:%d,\tHumi:%d.\r\n", 
								(uint16_t)g_sht2x_param.TEMP_HM,(uint16_t)g_sht2x_param.HUMI_HM);
				
        basicRfSendPacket(Coordinator_ADDR, pTxData,APP_PAYLOAD_LENGTH); //发送温湿度数据到协调器节点

        memset(pTxData,0,APP_PAYLOAD_LENGTH);//清楚缓冲
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
    IIC_Init();           //初始化IIC 
    TM1640_Init();        //初始化TM1640
    // Initalise hal_rf 硬件抽象层的 rf 进行初始化
    if(halRfInit()==FAILED)
    {
      HAL_ASSERT(FALSE);
    }
    SHT20_Node();       //温湿度节点
}
