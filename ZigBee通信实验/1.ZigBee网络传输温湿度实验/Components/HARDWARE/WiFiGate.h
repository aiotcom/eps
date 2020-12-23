#ifndef WIFIGATE_H
#define WIFIGATE_H
#include "hal_types.h"
#include "SampleApp.h"
#include "string.h"
#include "stdio.h"
#include "hal_uart.h"
#include "OSAL.h"


#define CONNECT_ONENET_KEYSTRING "*201298#1234567890#sample*"

typedef struct
{
  uint8 len;
  uint8 *ptr; 
}_WiFi_Packet;
extern uint8 WiFiGate_TaskId;
uint16 WiFiGate_ProcessEvent( uint8 task_id, uint16 events );
void WiFiGate_Init( uint8 task_id );
void SendToWiFiNetwork(uint8 *pData,uint8 len);
#endif