#include "WiFiGate.h"

#define SEND_TO_WIFI(d,l)        HalUARTWrite(HAL_UART_PORT_0,d,l);
#define WIFI_RECV_DATA_BUFFER_LEN 100
uint8 WiFiRecvDataBuffer[ WIFI_RECV_DATA_BUFFER_LEN ];
uint8 WiFiRecvLenght = 0;
uint8 NextEvent=0;
uint8 WiFiCmdHandle = 0;

#define GET_RECV_LENGHT()        Hal_UART_RxBufLen(HAL_UART_PORT_0);
#define GET_RECV_DATA(b,l)       HalUARTRead(HAL_UART_PORT_0,b,l);
/*
WiFi网关功能初始化，并不是直正的初始化WiFi模块，WiFi模块的初始化在WiFiGate_ProcessEvent()中
这里主要是启动WiFi的初始化以及收发数据。
osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_SEND_PERIODIC_MSG_EVT,TIME);
uint8 osal_msg_send( uint8 destination_task, uint8 *msg_ptr )
osal_set_event();
*/
uint8 WiFiGate_TaskId = 0;
uint8 WiFiModeInitDone = 0;
void WiFiGate_Init( uint8 task_id )
{
  WiFiGate_TaskId = task_id;
//  osal_set_event(WiFiGate_TaskId,WIFI_INIT_SEND_AT_EVT);
  osal_start_timerEx( WiFiGate_TaskId, WIFI_INIT_SEND_AT_EVT,2000); 
  UartInit(HAL_UART_PORT_0,HAL_UART_BR_115200);
}
/*
配置WIFI参数
*/
void ConfigWiFiArg(uint8 *pConfig,uint16 NextSta)
{
  memset((void*)WiFiRecvDataBuffer,0,WIFI_RECV_DATA_BUFFER_LEN);
  WiFiRecvLenght = 0;
  HalUARTResetRxQueue();
  SEND_TO_WIFI(pConfig,strlen(pConfig)); 
  osal_start_timerEx( WiFiGate_TaskId, WIFI_GET_RSPONE,500);
  NextEvent = NextSta;
}
/*
wifi网关处理任务，包括初始化WiFi模块，收发数据
*/
uint16 WiFiGate_ProcessEvent( uint8 task_id, uint16 events )
{
  uint8 len=0;
  (void)task_id;  // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG )
  {//如果是系统任务
    
  }
  else
  {//如果是用户自定义任务
    switch(events)
    {
      case WIFI_INIT_SEND_AT_EVT:
           printf("WifiConfig:Send AT\r\n");
           ConfigWiFiArg("AT\r\n",WIFI_INIT_SET_MODE_EVT);
        break;
      case WIFI_INIT_SET_MODE_EVT:
           printf("WifiConfig:AT+CWMODE\r\n");
           ConfigWiFiArg("AT+CWMODE=3\r\n",WIFI_INIT_CONNECT_AP_EVT);
        break;
      case WIFI_INIT_CONNECT_AP_EVT:
           printf("WifiConfig:AT+CWJAP\r\n");
           ConfigWiFiArg("AT+CWJAP=\"Honor 8X\",\"1234567890\"\r\n",WIFI_INIT_CONNECT_SERVER_EVT);
        break;
      case WIFI_INIT_CONNECT_SERVER_EVT:
           printf("WifiConfig:AT+CIPSTART\r\n");
           ConfigWiFiArg("AT+CIPSTART=\"TCP\",\"183.230.40.40\",1811\r\n",WIFI_INIT_TRANSMIT_MODE_EVT);
        break;
      case WIFI_INIT_TRANSMIT_MODE_EVT:
           printf("WifiConfig:AT+CIPMODE\r\n");
           ConfigWiFiArg("AT+CIPMODE=1\r\n",WIFI_INIT_START_SEND_EVT);
        break;
      case WIFI_INIT_START_SEND_EVT:
           printf("WifiConfig:AT+CIPSEND\r\n");
           ConfigWiFiArg("AT+CIPSEND\r\n",WIFI_INIT_CONNECT_ONENET);
        break;
      case WIFI_INIT_CONNECT_ONENET:
           osal_start_timerEx( WiFiGate_TaskId, WIFI_RECEIVE_MSG_EVT,50);
           printf("Try to connect OneNET\r\n");
           
           //发送连接OneNET的命令连接到OneNET平台
           SEND_TO_WIFI(CONNECT_ONENET_KEYSTRING,strlen(CONNECT_ONENET_KEYSTRING));
           WiFiModeInitDone = 1;//wifi模块初始化正常，并连接到oneNET上。
        break;        
      case WIFI_GET_RSPONE:
           len = GET_RECV_LENGHT();
            if(len)
            {              
              GET_RECV_DATA(&WiFiRecvDataBuffer[WiFiRecvLenght],len);
              WiFiRecvLenght = WiFiRecvLenght + len;
              //printf("%s",WiFiRecvDataBuffer);
              if(WiFiRecvLenght >= WIFI_RECV_DATA_BUFFER_LEN)
              {
                WiFiRecvLenght = 0;
              }
            }
            if(strstr(WiFiRecvDataBuffer,"OK\r\n"))
            {
                osal_set_event(WiFiGate_TaskId,NextEvent);
            }
            else
            {
                osal_start_timerEx( WiFiGate_TaskId, WIFI_GET_RSPONE,100); 
            }
        break;
      case WIFI_RECEIVE_MSG_EVT:
           len = GET_RECV_LENGHT();
            if(len)
            {              
              GET_RECV_DATA(&WiFiRecvDataBuffer[WiFiRecvLenght],len);
              WiFiRecvLenght = WiFiRecvLenght + len;
              if(WiFiRecvLenght >= WIFI_RECV_DATA_BUFFER_LEN)
              {
                WiFiRecvLenght = 0;
              }
            }
            if(WiFiCmdHandle)
            {
              if(strstr(WiFiRecvDataBuffer,"~BUTTON_ON"))
              {
		  printf("CMD:~BUTTON_ON\r\n");
                  PointToPoint_FanControl(1);                  
              }
              else if(strstr(WiFiRecvDataBuffer,"~BUTTON_OFF"))
              {
                  printf("CMD:~BUTTON_OFF\r\n");
                  PointToPoint_FanControl(0); 
              }
              WiFiCmdHandle = 0;
              WiFiRecvLenght = 0;
            }
            if(!len)
            {
              osal_start_timerEx( WiFiGate_TaskId, WIFI_RECEIVE_MSG_EVT,100);
            }
            else
            {
              osal_start_timerEx( WiFiGate_TaskId, WIFI_RECEIVE_MSG_EVT,10);
              WiFiCmdHandle = 1;
            }
      break;
      case WIFI_SEND_DATA_MSG_EVT:
           osal_msg_receive( WiFiGate_TaskId );
           //SEND_TO_WIFI();
      break;
      
    }
    
  }
  return 0;
} 
/*
发送数据到WIFI网络上
*/
void SendToWiFiNetwork(uint8 *pData,uint8 len)
{
  if(WiFiModeInitDone)
  {
    SEND_TO_WIFI(pData,len);
  }
}