#ifndef SAMPLEAPP_H
#define SAMPLEAPP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "AF.h"
#include "Switch.h"
#include "HC_SR04.h"
#include "Time.h"
#include "TM1640.h"
#include "PM2p5.h"
/*********************************************************************
 * CONSTANTS
 */

// These constants are only for example and should be changed to the
// device's needs
#define SAMPLEAPP_ENDPOINT           20

#define SAMPLEAPP_PROFID             0x0F08
#define SAMPLEAPP_DEVICEID           0x0001
#define SAMPLEAPP_DEVICE_VERSION     0
#define SAMPLEAPP_FLAGS              0

#define PM2P5_CLUSTERID            0x0001  
  
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the Generic Application
 */
extern void SampleApp_Init( uint8 task_id );
extern void SampleApp1_Init( uint8 task_id );
extern void PointToPoint_FanControl(uint8 on_off);
/*
 * Task Event Processor for the Generic Application
 */
extern UINT16 SampleApp_ProcessEvent( uint8 task_id, uint16 events );
extern UINT16 SampleApp1_ProcessEvent( uint8 task_id, uint16 events );
extern afAddrType_t Boardcast_DstAddr;
extern endPointDesc_t Coordinator_epDesc;
extern endPointDesc_t EndDevice_epDesc;
extern uint8 SampleApp_TransID;
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SAMPLEAPP_H */
