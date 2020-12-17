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
*	说明： 		RSSI采集实验

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
static perTestPacket_t rxPacket;
static volatile uint8 appState;
static volatile uint8 appStarted;

static void appTimerISR(void);
static void appReceiver();
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
//	函数名称：	appReceiver()
//
//	函数功能：	接收函数，将接收到的数据解析后发送到PC端
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
static void appReceiver()
{
  uint32 segNumber=0;                              // 数据包序列号 
  int16 perRssiBuf[RSSI_AVG_WINDOW_SIZE] = {0};    // Ring buffer for RSSI 存储RSSI的环形缓冲区
  uint8 perRssiBufCounter = 0;                     // Counter to keep track of the 计数器用于RSSI缓冲区统计

  perRxStats_t rxStats = {0,0,0,0};      
   int16 rssi;
  uint8 resetStats=FALSE;
  
  int8 Myper[6];        
  int8 Myrssi[3];
  int8 Myreceive[5];
  int32 temp_per;           //存放掉包率
  int32 temp_receive;       //存放接收的包的个数
  int32 temp_rssi;          //存放前32个rssi值的平均值
  initUART();               // 初始化串口
  
    
	//初始化Basic RF 
	basicRfConfig.panId = PAN_ID;
	basicRfConfig.ackRequest = FALSE;
	//设置信道，规范要求信道只能为为 11——25。这里选择信道 11
	basicRfConfig.channel = 0x0B;	
	basicRfConfig.myAddr = RX_ADDR;
  if(basicRfInit(&basicRfConfig)==FAILED) 
  {
    HAL_ASSERT(FALSE);
  }
  basicRfReceiveOn();
  OLED_CLS();     //清屏

  OLED_P16x16Ch(1,2,5);   //数
  OLED_P16x16Ch(15,2,6);  //据
  OLED_P16x16Ch(30,2,7);  //包
  OLED_P16x16Ch(45,2,8);  //个
  OLED_P16x16Ch(60,2,5);  //数
  OLED_P8x16Str(75,2,":");
  
  OLED_P16x16Ch(1,4,9);    //误
  OLED_P16x16Ch(30,4,7);   //包
  OLED_P16x16Ch(60,4,10);  //率
  OLED_P8x16Str(75,4,":");
  
  OLED_P8x16Str(1,6,"RSSI");
  OLED_P16x16Ch(30,6,11);  //平
  OLED_P16x16Ch(45,6,12);  //均
  OLED_P16x16Ch(60,6,13);  //值
  OLED_P8x16Str(75,6,":");
  while (TRUE)
  {
    while(!basicRfPacketIsReady());     // 等待新的数据包
    if(basicRfReceive((uint8*)&rxPacket, MAX_PAYLOAD_LENGTH, &rssi)>0) {
         halLedSet(3);//P1_4
			
      UINT32_NTOH(rxPacket.seqNumber);  // 改变接收序号的字节顺序
      segNumber = rxPacket.seqNumber;
            
     // 若统计被复位，设置期望收到的数据包序号为已经收到的数据包序号     
      if(resetStats)
      {
        rxStats.expectedSeqNum = segNumber;
        
        resetStats=FALSE;
      }      

      rxStats.rssiSum -= perRssiBuf[perRssiBufCounter];   // 从sum中减去旧的RSSI值

      perRssiBuf[perRssiBufCounter] =  rssi;              // 存储新的RSSI值到环形缓冲区，之后它将被加入sum

      rxStats.rssiSum += perRssiBuf[perRssiBufCounter];   // 增加新的RSSI值到sum
      if(++perRssiBufCounter == RSSI_AVG_WINDOW_SIZE) {
        perRssiBufCounter = 0;    
      }

      
     // 检查接收到的数据包是否是所期望收到的数据包
      if(rxStats.expectedSeqNum == segNumber)  // 是所期望收到的数据包 
      {
        rxStats.expectedSeqNum++;  
      }
      

      else if(rxStats.expectedSeqNum < segNumber)  // 不是所期望收到的数据包（大于期望收到的数据包的序号）
      {                                            // 认为丢包
        rxStats.lostPkts += segNumber - rxStats.expectedSeqNum;
        rxStats.expectedSeqNum = segNumber + 1;
      }
      
      else  // (小于期望收到的数据包的序号）
      {      
        //新的测试开始，复位统计变量
        rxStats.expectedSeqNum = segNumber + 1;
        rxStats.rcvdPkts = 0;
        rxStats.lostPkts = 0;
      }
      rxStats.rcvdPkts++;
      
           // reset statistics if button 1 is pressed
            temp_receive=(int32)rxStats.rcvdPkts;
            if(temp_receive>=1000)
            {
               if(halButtonPushed()==HAL_BUTTON_1)
               {
                   resetStats = TRUE;
                   rxStats.rcvdPkts = 1;
                   rxStats.lostPkts = 0;
               }
               rxStats.rcvdPkts = 1;
            }

            Myreceive[0]=temp_receive/100+'0';
            Myreceive[1]=temp_receive%100/10+'0';
            Myreceive[2]=temp_receive%10+'0';
            Myreceive[3]='\0';
            UartTX_Send_String("RECE:",strlen("RECE:"));
            UartTX_Send_String(Myreceive,4);
            UartTX_Send_String("    ",strlen("    "));   
            
            temp_per = (int32)((rxStats.lostPkts*1000)/(rxStats.lostPkts+rxStats.rcvdPkts));
            Myper[0]=temp_per/100+'0';
            Myper[1]=temp_per%100/10+'0'; 
            Myper[2]='.';
            Myper[3]=temp_per%10+'0';
            Myper[4]='%';
            Myper[5]='\0';
            UartTX_Send_String("PER:",strlen("PER:"));
            UartTX_Send_String(Myper,5);
            UartTX_Send_String("    ",strlen("    "));
           
            temp_rssi=(0-(int32)rxStats.rssiSum/32);
            Myrssi[0]=temp_rssi/10+'0';
            Myrssi[1]=temp_rssi%10+'0';
            Myrssi[2]='\0';
            UartTX_Send_String("RSSI:-",strlen("RSSI:-"));
            UartTX_Send_String(Myrssi,2);        
            UartTX_Send_String("\n",strlen("\n"));
            
            halLedClear(3);
            
            OLED_P8x16Str(107,2," ");                     //数据包个数   
            OLED_P8x16Str(83,2,(uint8_t *)Myreceive);     //数据包个数
            
            OLED_P8x16Str(83,4,(uint8_t *)Myper);         //误包率
            
            OLED_P8x16Str(98,6,"   ");                    //RSSI平均值
            OLED_P8x16Str(83,6,(uint8_t *)Myrssi);        //RSSI平均值
            
            halMcuWaitMs(300);
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
		
    IIC_Init();       //IIC初始化
    OLED_Init();      //初始化OLED
    OLED_Init_UI();  //开机界面
    
    appStarted = TRUE;//应用程序运行模块

    // 初始化 hal_rf
    if(halRfInit()==FAILED) {
      HAL_ASSERT(FALSE);
    }
    
    halMcuWaitMs(350);//350ms
       
		appReceiver(); //接收模式,接收数据并测量出误码率及RSSi
}

