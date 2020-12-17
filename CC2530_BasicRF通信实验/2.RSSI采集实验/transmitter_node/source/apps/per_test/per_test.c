/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2018-10-30
*
*	版本： 		V1.0
*   
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/

#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_int.h"
#include "hal_timer_32k.h"
#include "hal_joystick.h"
#include "hal_button.h"
#include "hal_board.h"
#include "hal_rf.h"
#include "hal_assert.h"
#include "util_lcd.h"
#include "basic_rf.h"
#include "per_test.h"
#include "string.h"
#include "delay.h"
#include "OLED.h"
#include "IIC.h"

#define IDLE                      0
#define TRANSMIT_PACKET           1

/*****************************important select or shelt***********************************/
//#define MODE_SEND              //屏蔽时：  appReceiver
                                 //不屏蔽时：appTransmitter
/*******************************************************************************/

static basicRfCfg_t basicRfConfig;
static perTestPacket_t txPacket;
static volatile uint8 appState;
static volatile uint8 appStarted;

static void appTimerISR(void);
//static void appStartStop(void);
static void appTransmitter();
void initUART(void);
void UartTX_Send_String(int8 *Data,int len);

/****************************************************************
串口初始化函数
****************************************************************/
void initUART(void)
{
    PERCFG = 0x00;		      //位置1 P0口
    P0SEL = 0x0c;		      //P0_2,P0_3用作串口（外部设备功能）
    P2DIR &= ~0XC0;                   //P0优先作为UART0

    U0CSR |= 0x80;		      //设置为UART方式
    U0GCR |= 11;				       
    U0BAUD |= 216;		      //波特率设为115200
    UTX0IF = 0;                       //UART0 TX中断标志初始置位0
}
/****************************************************************
串口发送字符串函数			
****************************************************************/
void UartTX_Send_String(int8 *Data,int len)
{
  int j;
  for(j=0;j<len;j++)
  {
    U0DBUF = *Data++;
    while(UTX0IF == 0);
    UTX0IF = 0;
  }
}

/***********************************************************************************
* @fn          appTimerISR
*
* @brief       32KHz timer interrupt service routine. Signals PER test transmitter
*              application to transmit a packet by setting application state.
*
* @param       none
*
* @return      none
*/
static void appTimerISR(void)
{
    appState = TRANSMIT_PACKET;
}



/***********************************************************************************
* @fn          appConfigTimer
*
* @brief       Configure timer interrupts for application. Uses 32KHz timer
*
* @param       uint16 rate - Frequency of timer interrupt. This value must be
*              between 1 and 32768 Hz
*
* @return      none
*/
static void appConfigTimer(uint16 rate)
{
    halTimer32kInit(TIMER_32K_CLK_FREQ/rate);
    halTimer32kIntConnect(&appTimerISR);
}
//==========================================================
//	函数名称：	appTransmitter()
//
//	函数功能：	发送函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void appTransmitter()
{
  uint32 burstSize=0;
  uint32 pktsSent=0;
  //uint8 appTxPower;
  uint8 n;
  
	// 配置 Basic RF
	basicRfConfig.panId = PAN_ID;
	basicRfConfig.ackRequest = FALSE;
	//设置信道，规范要求信道只能为为 11——25。这里选择信道 11
	basicRfConfig.channel = 0x0B;
  basicRfConfig.myAddr = TX_ADDR;
  if(basicRfInit(&basicRfConfig)==FAILED)
  {
    HAL_ASSERT(FALSE);
  }

   /* 设置输出功率 */
  halRfSetTxPower(2);

  /* 设置进行一次测试所发送的数据包数量 */
  burstSize = 1000;

  basicRfReceiveOff();

   /* 配置定时器和IO *************************/
  appConfigTimer(0xC8);

  /* 初始化数据包载荷 */
  txPacket.seqNumber = 0;
  for(n = 0; n < sizeof(txPacket.padding); n++) 
  {
    txPacket.padding[n] = n;
  }
  
   /* 主循环 */
  while (TRUE)
  {
   // 等待用户启动应用  

    while(appStarted)
      {

      if (pktsSent < burstSize) 
      {
          UINT32_HTON(txPacket.seqNumber);  // 改变发送序号的字节顺序
          basicRfSendPacket(RX_ADDR, (uint8*)&txPacket, PACKET_SIZE);

          /* 在增加序号前将字节顺序改回为主机顺序 */
          UINT32_NTOH(txPacket.seqNumber);
          txPacket.seqNumber++;

          pktsSent++;

          appState = IDLE;
          halLedToggle(1);   //改变LED1的亮灭状态
          halMcuWaitMs(500);
      }
      else
        appStarted = !appStarted;
      
      /* 复位统计和序号 */
     pktsSent = 0;

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
void main (void)
{
    // 初始化外围硬件
    halBoardInit();
    appStarted = TRUE;//应用程序开始
    // 初始化 hal_rf
    if(halRfInit()==FAILED) {
      HAL_ASSERT(FALSE);
    }  

    halMcuWaitMs(350);//延时350ms

		appTransmitter(); //发射模式
}

