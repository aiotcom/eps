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

#define APP_PAYLOAD_LENGTH     0x02  //收发的数据长度
#define CMD_LIGHT_CTRL         0x01  //灯控制命令
/*基本参数设置，很重要*/
#define RF_CHANNEL                25  // 11~26,收发双方必须是同样的信道
#define PAN_ID                0x2007 	//PANID，收发双方必须是同样PAIN
#define SWITCH_ADDR           0x2520 	//按键节点的网络短地址
#define LIGHT_ADDR            0xBEEF	//灯光节点的网络短地址

static uint8 pTxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;
//==========================================================
//	函数名称：	appSwitch()
//
//	函数功能：	数据发送函数，按键发送数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void appSwitch(void)
{    
    basicRfConfig.panId      = PAN_ID;     //PANID
    basicRfConfig.channel    = RF_CHANNEL; //信道
    basicRfConfig.ackRequest = TRUE;			 //需要ACK回复
    basicRfConfig.myAddr     = SWITCH_ADDR;  // 设置发送地址
		
    if(basicRfInit(&basicRfConfig)==FAILED) //配置无线通信参数
    {//如果配置错误
      HAL_ASSERT(FALSE);
    }

    pTxData[0] = CMD_LIGHT_CTRL;//灯控制命令码

    //不开启接收,只发送
    basicRfReceiveOff();

    //程序进入死循环
    while (TRUE)
    {
        pTxData[1] = KEY_Scan(0);//按键扫描

        switch(pTxData[1])
        {
          case S1_PRES:  //按键1
            basicRfSendPacket(LIGHT_ADDR, pTxData, APP_PAYLOAD_LENGTH); //发送数据
            break;
          case S2_PRES:  //按键2
            basicRfSendPacket(LIGHT_ADDR, pTxData, APP_PAYLOAD_LENGTH); //发送数据
            break;
          case S3_PRES:  //按键3
            basicRfSendPacket(LIGHT_ADDR, pTxData, APP_PAYLOAD_LENGTH); //发送数据
            break;
          case S4_PRES:  //按键4
            basicRfSendPacket(LIGHT_ADDR, pTxData, APP_PAYLOAD_LENGTH); //发送数据
            break;
          default: break;
        }
        
        delay_ms(100);//100ms检测一次按键
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
		KEY_Init();   //初始化LED模块上的按键
    // Initalise hal_rf 硬件抽象层的 rf 进行初始化
    if(halRfInit()==FAILED)
    {
      HAL_ASSERT(FALSE);
    }
    appSwitch();        //节点为按键    
}
