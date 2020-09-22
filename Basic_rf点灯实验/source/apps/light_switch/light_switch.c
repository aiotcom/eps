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

#define RF_CHANNEL                25                         // 2.4 GHz RF channel

// BasicRF address definitions
#define PAN_ID                0x2007
#define SWITCH_ADDR           0x2520
#define LIGHT_ADDR            0xBEEF

#define APP_PAYLOAD_LENGTH        2
#define LIGHT_TOGGLE_CMD          0

// Application states
#define IDLE                      0
#define SEND_CMD                  1

// Application role
#define NONE                      0
#define SWITCH                    1
#define LIGHT                     2
#define APP_MODES                 2

/***********************************************************
* LOCAL VARIABLES
*/
static uint8 pTxData[APP_PAYLOAD_LENGTH];
static uint8 pRxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;

// Mode menu
static menuItem_t pMenuItems[] =
{
#ifdef ASSY_EXP4618_CC2420
  // Using Softbaugh 7-seg display
  " L S    ", SWITCH,
  " LIGHT  ", LIGHT
#else
  // SRF04EB and SRF05EB
  "Switch",   SWITCH,
  "Light",    LIGHT
#endif
};

static menu_t pMenu =
{
  pMenuItems,
  N_ITEMS(pMenuItems)
};


#ifdef SECURITY_CCM
// Security key
static uint8 key[]= {
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
};
#endif

static void appLight();
static void appSwitch();
static uint8 appSelectMode(void);

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

#ifdef ASSY_EXP4618_CC2420
    halLcdClearLine(1);
    halLcdWriteSymbol(HAL_LCD_SYMBOL_RX, 1);
#endif
        
    basicRfConfig.myAddr = LIGHT_ADDR;                                        // 设置本机地址
    
    if(basicRfInit(&basicRfConfig)==FAILED) 
    {
      HAL_ASSERT(FALSE);
    }

    //开启RF接收
    basicRfReceiveOn();
    
    //初始化LED灯
    LED1_SET();LED2_SET();LED3_SET();LED4_SET();   

    // Main loop
    while (TRUE)
    {
        while(!basicRfPacketIsReady());                                     //开启接收

        if(basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL)>0)             //接收发送到该地址的数据
        {
            switch(pRxData[0])
            {
              case S1_PRES:                                                 //按键1
                MCU_IO_TGL_PREP(0,2);
                break;
              case S2_PRES:                                                 //按键2
                MCU_IO_TGL_PREP(0,3);
                break;
              case S3_PRES:                                                 //按键3
                MCU_IO_TGL_PREP(1,6);
                break;
              case S4_PRES:                                                 //按键4
                MCU_IO_TGL_PREP(1,7);
                break;
              default: break;
            }
            pRxData[0] = 0;
        }
    }
}

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
static void appSwitch()
{

#ifdef ASSY_EXP4618_CC2420
    halLcdClearLine(1);
    halLcdWriteSymbol(HAL_LCD_SYMBOL_TX, 1);
#endif

    KEY_Init();                                                           //初始化按键
    
    // 设置发送地址
    basicRfConfig.myAddr = SWITCH_ADDR;
    
    if(basicRfInit(&basicRfConfig)==FAILED) 
    {
      HAL_ASSERT(FALSE);
    }

    pTxData[0] = LIGHT_TOGGLE_CMD;

    //不开启接收
    basicRfReceiveOff();

    //程序进入死循环
    while (TRUE)
    {
        pTxData[0] = KEY_Scan(0);

        switch(pTxData[0])
        {
          case S1_PRES:                                                 //按键1
            MCU_IO_TGL_PREP(0,2);
            basicRfSendPacket(LIGHT_ADDR, pTxData, APP_PAYLOAD_LENGTH); //发送数据
            break;
          case S2_PRES:                                                 //按键2
            MCU_IO_TGL_PREP(0,3);
            basicRfSendPacket(LIGHT_ADDR, pTxData, APP_PAYLOAD_LENGTH); //发送数据
            break;
          case S3_PRES:                                                 //按键3
            MCU_IO_TGL_PREP(1,6);
            basicRfSendPacket(LIGHT_ADDR, pTxData, APP_PAYLOAD_LENGTH); //发送数据
            break;
          case S4_PRES:                                                 //按键4
            MCU_IO_TGL_PREP(1,7);
            basicRfSendPacket(LIGHT_ADDR, pTxData, APP_PAYLOAD_LENGTH); //发送数据
            break;
          default: break;
        }
        
        delay_ms(100);

        // 让单片机进入睡眠状态。它会在中断中醒来
        halIntOff();
        halMcuSetLowPowerMode(HAL_MCU_LPM_3); 
        // interrupt enable
        halIntOn(); 
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
    uint8 appMode = NONE;                                               //不设置模块的模式

    LED_Init();                                                         //初始化LED灯
    
    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
#ifdef SECURITY_CCM                                                     //密钥安全通信，本例程不加密
    basicRfConfig.securityKey = key; 
#endif

    // Initalise board peripherals 初始化外围设备
    halBoardInit();
    halJoystickInit();

    // Initalise hal_rf 硬件抽象层的 rf 进行初始化
    if(halRfInit()==FAILED)
    {
      HAL_ASSERT(FALSE);
    }
   
    
    /******选择性下载程序，发送模块和接收模块******/
      //appSwitch();                                                    //节点为按键
      appLight();                                                       //节点为指示灯LED
     
   // Role is undefined. This code should not be reached
    HAL_ASSERT(FALSE);
}

/*****************************************************************************
* @fn          appSelectMode
*
* @brief       Select application mode
*
* @param       none
*
* @return      uint8 - Application mode chosen
*/
static uint8 appSelectMode(void)
{
    halLcdWriteLine(1, "Device Mode: ");

    return utilMenuSelect(&pMenu);
}
