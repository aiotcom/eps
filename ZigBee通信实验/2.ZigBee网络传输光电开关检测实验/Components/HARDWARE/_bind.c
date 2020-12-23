#include "_bind.h"
/*
启动邦定注要，都输出方去申请绑定
*/
void StartBind(uint8 NumOfId,const cId_t *pId )
{
  if ( AF_DataRequest( &Boardcast_DstAddr,
                       &Coordinator_epDesc,
                       BIND_REQUEST_CMD_ID,
                       NumOfId*2,
                       (uint8*)pId,
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    printf("request bind\r\n");//串口打印提示，发送成功
  }  
}
/*
设备反馈绑定请求
*/
void ResponeBind(uint16 *pId,afAddrType_t *dstAddr)
{
  if ( AF_DataRequest( dstAddr,
                       &EndDevice_epDesc,
                       BIND_RSP_CMD_ID,
                       2,
                       (uint8*)pId,
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    printf("response bind\r\n");//串口打印提示，发送成功
  } 
}