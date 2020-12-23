#include "WiFiGate.h"
#include "ZComDef.h"
#include "hal_drivers.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "nwk.h"
#include "APS.h"
#include "ZDApp.h"
#if defined ( ZIGBEE_FREQ_AGILITY ) || defined ( ZIGBEE_PANID_CONFLICT )
  #include "ZDNwkMgr.h"
#endif
#if defined ( ZIGBEE_FRAGMENTATION )
  #include "aps_frag.h"
#endif

#include "SampleApp.h"
#include "WiFiGate.h"
#include "OSAL.h"
#include "ZGlobals.h"
#include "AF.h"
#include "aps_groups.h"
#include "ZDApp.h"

#include "SampleApp.h"
#include "SampleAppHw.h"

#include "OnBoard.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "string.h"

#include "MT_UART.h"
#include "MT_APP.h"
#include "MT.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */

// The order in this table must be identical to the task initialization calls below in osalInitTask.
const pTaskEventHandlerFn tasksArr[] = {
  macEventLoop,
  nwk_event_loop,
  Hal_ProcessEvent,
#if defined( MT_TASK )
  MT_ProcessEvent,
#endif
  APS_event_loop,
#if defined ( ZIGBEE_FRAGMENTATION )
  APSF_ProcessEvent,
#endif
  ZDApp_event_loop,
#if defined ( ZIGBEE_FREQ_AGILITY ) || defined ( ZIGBEE_PANID_CONFLICT )
  ZDNwkMgr_event_loop,
#endif
  SampleApp_ProcessEvent,
};

const uint8 tasksCnt = sizeof( tasksArr ) / sizeof( tasksArr[0] );
uint16 *tasksEvents;
/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalInitTasks
 *
 * @brief   This function invokes the initialization function for each task.
 *
 * @param   void
 *
 * @return  none
 */
void osalInitTasks( void )
{
  uint8 taskID = 0;

  tasksEvents = (uint16 *)osal_mem_alloc( sizeof( uint16 ) * tasksCnt);
  osal_memset( tasksEvents, 0, (sizeof( uint16 ) * tasksCnt));

  macTaskInit( taskID++ );
  nwk_init( taskID++ );
  Hal_Init( taskID++ );
#if defined( MT_TASK )
  MT_TaskInit( taskID++ );
#endif
  APS_Init( taskID++ );
#if defined ( ZIGBEE_FRAGMENTATION )
  APSF_Init( taskID++ );
#endif
  ZDApp_Init( taskID++ );
#if defined ( ZIGBEE_FREQ_AGILITY ) || defined ( ZIGBEE_PANID_CONFLICT )
  ZDNwkMgr_Init( taskID++ );
#endif
  SampleApp_Init( taskID++ );
}

const cId_t InClusterList[1] =
{
  0
};
const cId_t OutClusterList[1] =
{
  FAN_CONTROL_CLUSTERID
};

const SimpleDescriptionFormat_t Coordinator_SimpleDesc =
{
  SAMPLEAPP_ENDPOINT,                 //  int Endpoint;
  SAMPLEAPP_PROFID,                   //  uint16 AppProfId[2];
  SAMPLEAPP_DEVICEID,                 //  uint16 AppDeviceId[2];
  SAMPLEAPP_DEVICE_VERSION,           //  int   AppDevVer:4;
  SAMPLEAPP_FLAGS,                    //  int   AppFlags:4;
  0,                                  //  uint8  AppNumInClusters;
  (cId_t *)InClusterList,             //  uint8 *pAppInClusterList;
  1,                                  //  uint8  AppNumOutClusters;
  (cId_t *)OutClusterList             //  uint8 *pAppOutClusterList;
};
// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in SampleApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t Coordinator_epDesc;
/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
uint8 SampleApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // SampleApp_Init() is called.
devStates_t SampleApp_NwkState;

uint8 SampleApp_TransID;  // This is the unique message ID (counter)

afAddrType_t Point_To_Point_DstAddr;//点对点通信定义

afAddrType_t Boardcast_DstAddr;//广播通信定义
/*********************************************************************
 * LOCAL FUNCTIONS
 */
void SampleApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void Send_FANCtrl(uint8 ctrl);
/*********************************************************************
 * @fn      SampleApp_Init
 *
 * @brief   Initialization function for the Generic App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void SampleApp_Init( uint8 task_id )
{
  SampleApp_TaskID = task_id;
  SampleApp_NwkState = DEV_INIT;
  SampleApp_TransID = 0;  

  OLED_Init();  //初始化OLED
  LED_Init(); //初始化LED模块 
  UartInit(HAL_UART_PORT_1,HAL_UART_BR_115200);//初始化串口用于调试
  
  OLED_P8x16Str(0,0,"coordinator");
  
  Point_To_Point_DstAddr.addrMode       = (afAddrMode_t)Addr16Bit; //点播
  Point_To_Point_DstAddr.endPoint       = SAMPLEAPP_ENDPOINT;      
  Point_To_Point_DstAddr.addr.shortAddr = 0; 

  //广播通信定义 
  Boardcast_DstAddr.addrMode       = (afAddrMode_t)AddrBroadcast;
  Boardcast_DstAddr.endPoint       = SAMPLEAPP_ENDPOINT;
  Boardcast_DstAddr.addr.shortAddr = 0Xffff;
	
  // 填写端点
  Coordinator_epDesc.endPoint   = SAMPLEAPP_ENDPOINT;
  Coordinator_epDesc.task_id    = &SampleApp_TaskID;
  Coordinator_epDesc.simpleDesc = (SimpleDescriptionFormat_t *)&Coordinator_SimpleDesc;
  Coordinator_epDesc.latencyReq = noLatencyReqs;
  // 注册端点
  afRegister( &Coordinator_epDesc );
  osal_start_timerEx( SampleApp_TaskID, READ_KEY_MSG_EVT,3000); 
}

/*********************************************************************
 * @fn      SampleApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
uint8 SendTemperatureIndicate = 1;
uint8 tmpbuf[64];
uint8 ReceiveIndicate = 0;
uint8 Sensor_Temp,Sensor_Humi,Flag_OnOff = 0;
uint8 FAN_State=0;
uint16 SampleApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  (void)task_id;  // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {        
        // Received when a messages is received (OTA) for this endpoint
        case AF_INCOMING_MSG_CMD:
          SampleApp_MessageMSGCB( MSGpkt );
          break;

        // Received whenever the device changes state in the network
        case ZDO_STATE_CHANGE:
          SampleApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if(SampleApp_NwkState == DEV_ZB_COORD)
          {
						StartBind(1,(const cId_t*)Coordinator_SimpleDesc.pAppOutClusterList);
						osal_start_timerEx( SampleApp_TaskID, BIND_TIME_MSG_EVT,2000);
            printf("coord ready!");
          }
          break;

        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next - if one is available
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }//if ( events & SYS_EVENT_MSG )
  
  if(events & BIND_TIME_MSG_EVT){//绑定超时
		printf("bind timeout\r\n");
		StartBind(1,(const cId_t*)Coordinator_SimpleDesc.pAppOutClusterList);
		osal_start_timerEx( SampleApp_TaskID, BIND_TIME_MSG_EVT,2000);        
  } 
  if(events & READ_KEY_MSG_EVT){//是READ_KEY_MSG_EVT事件 
		if(KEY_Scan()&S1_PRES){//检测按键
			printf("S1_PRES\r\n");
				FAN_State = 1 - FAN_State;
				Send_FANCtrl(FAN_State);//发送控制命令
		}
		/*100ms 后触发READ_KEY_MSG_EVT*/
		osal_start_timerEx( SampleApp_TaskID, READ_KEY_MSG_EVT,100); 	
  }
  // Discard unknown events
  return 0;
}
/*
发送风扇控制指令
*/
void Send_FANCtrl(uint8 ctrl)
{
  uint8 tmp;
  tmp = ctrl;
	if(!Point_To_Point_DstAddr.addr.shortAddr){//如果目标地址不为0，才能发送数据
			return;
	}
  if ( AF_DataRequest( &Point_To_Point_DstAddr,&Coordinator_epDesc,
                       FAN_CONTROL_CLUSTERID,
                       1,
                       &tmp,
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    //printf("tx success\r\n");//串口打印提示，发送成功
  }
}
/*********************************************************************
 * @fn      SampleApp_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
*/
void SampleApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{ 
  switch ( pkt->clusterId )
  {
  	case BIND_RSP_CMD_ID:
         osal_stop_timerEx(SampleApp_TaskID,BIND_TIME_MSG_EVT); 
         printf("source shortAddr=0x%x",pkt->srcAddr.addr.shortAddr);
         Point_To_Point_DstAddr.addr.shortAddr = pkt->srcAddr.addr.shortAddr;  
				 break;
  }
}


/*********************************************************************
*********************************************************************/
