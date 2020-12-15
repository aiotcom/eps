/**
 * Copyright (c) 2017 China Mobile IOT.
 * All rights reserved.
**/
#include "MsgHandler.h"
#include "stm32f1xx.h"
#include "stdio.h"
#include "M5310.h"
#include "nbiot.h"
#include "utils.h"
#include "usart.h"
#include "delay.h"
#include "coap.h"
//http://www.openmobilealliance.org/wp/OMNA/LwM2M/LwM2MRegistry.html#x_label
char coap_uri[] = "coap://183.230.40.40:5683"; //
char *serv_addr="183.230.40.40";
//****************************IMEI码         ;IMSI码         应与创建的应用的IMEI码，IMSI码一致
extern const char endpoint_name[];

uint8_t msgcode;
uint8_t AppStart = 0;
uint8_t light_control = 1;
uint8_t MemCpyEndComma(uint8_t *pdst,uint8_t *psrc,uint8_t maxlen);
uint16_t PreTempValue = 0,PreHumiValue = 0,PreIllumValue = 0,u16Temp;

uint8_t ReadRspTemp[]={"+MIPLREAD:0,62551,3303,0,5700,1,\"123\",1\r\n"};
uint8_t ReadRspHumi[]={"+MIPLREAD:0,62551,3304,0,5700,1,\"456\",1\r\n"};
uint8_t ReadRspIllum[]={"+MIPLREAD:0,62551,3301,0,5700,1,\"789\",1\r\n"};
uint8_t ReadRspLight[]={"+MIPLREAD:0,62551,3311,0,5850,5,\"true\",1\r\n"};

nbiot_uri_t temp_uri;
nbiot_uri_t humi_uri;
nbiot_uri_t illum_uri;
nbiot_uri_t led_uri;

//==========================================================
//	函数名称：	Subscription_esources()
//
//	函数功能：	订阅 Object 组配置
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void Subscription_esources(void)
{
//***************************温度********************************************<<<    
    m5310_addobj(OBJ_SENSOR_Temp_ID,INST_ID_Temp);       	//添加对象object,温度

		temp_uri.objid  = OBJ_SENSOR_Temp_ID;
		temp_uri.instid = INST_ID_Temp;
		temp_uri.resid  = RES_ID_SENSOR;
		temp_uri.flag  |= NBIOT_SET_OBJID;
		temp_uri.flag  |= NBIOT_SET_INSTID;

    m5310_notify_upload(&temp_uri,NBIOT_FLOAT,"1",0,0); 	//订阅资源
//***************************温度********************************************>>>  
    delay_ms(1000);
//***************************湿度********************************************<<<    
    m5310_addobj(OBJ_SENSOR_Humi_ID,INST_ID_Humi);       	//添加对象object 湿度
		humi_uri.objid  = OBJ_SENSOR_Humi_ID;
		humi_uri.instid = INST_ID_Humi;
		humi_uri.resid  = RES_ID_SENSOR;
		humi_uri.flag  |= NBIOT_SET_OBJID;
		humi_uri.flag  |= NBIOT_SET_INSTID;
    m5310_notify_upload(&humi_uri,NBIOT_FLOAT,"2",0,0); 	//订阅资源 
//***************************湿度********************************************>>>   
   delay_ms(1000);
////***************************光照强度****************************************<<< 
//    m5310_addobj(OBJ_SENSOR_illum_ID,INST_ID_illum);     	//添加对象object 光照强度
//		illum_uri.objid  = OBJ_SENSOR_illum_ID;
//		illum_uri.instid = INST_ID_illum;
//		illum_uri.resid  = RES_ID_SENSOR;
//		illum_uri.flag  |= NBIOT_SET_OBJID;
//		illum_uri.flag  |= NBIOT_SET_INSTID;
//    m5310_notify_upload(&illum_uri,NBIOT_FLOAT,"3",0,0); //订阅资源 
////***************************光照强度*****************************************>>> 
//   delay_ms(1000);
////***************************LED**********************************************<<< 
//    m5310_addobj(OBJ_LED_ID,INST_ID_LED);                 //添加对象object LED
//		led_uri.objid  = OBJ_LED_ID;
//		led_uri.instid = INST_ID_LED;
//		led_uri.resid  = RES_ID_LED;
//		led_uri.flag  |= NBIOT_SET_OBJID;
//		led_uri.flag  |= NBIOT_SET_INSTID;
//    m5310_notify_upload(&led_uri,NBIOT_BOOLEAN,"4",0,0); 	//订阅资源    
////***************************LED**********************************************>>>  
}

//==========================================================
//	函数名称：	MemCpyEndComma()
//
//	函数功能：	
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
uint8_t MemCpyEndComma(uint8_t *pdst,uint8_t *psrc,uint8_t maxlen)
{
    uint8_t j=0;
    while(maxlen--)
    {
        pdst[j] = psrc[j];
        if(psrc[++j] == ',')
        {
            break;
        }
    }
    return j;
}


static void handle_read( nbiot_device_t *dev,nbiot_uri_t *uri,uint8_t *buffer,size_t buffer_len,uint16_t ackid)
{
    uint8_t ret = 0;
    nbiot_send_buffer(uri,buffer,ret,ackid,false);//false 是回复平台下发的写操作。
}

static void handle_write( nbiot_device_t *dev,nbiot_uri_t *uri,uint16_t ackid,uint8_t *buffer,size_t buffer_len)
{
    m5310_write_rsp(1,ackid);
}

static void handle_execute( nbiot_device_t *dev,nbiot_uri_t *uri,uint16_t ackid,uint8_t *buffer,size_t buffer_len)
{
	  m5310_execute_rsp(1,ackid);
}

uint8_t *pMsgHeader;
uint8_t  RspMsgID[10],MsgID_Index=0; 
nbiot_uri_t uri;
void handle_request( nbiot_device_t *dev,uint16_t code, uint8_t *buffer,size_t buffer_len,size_t max_buffer_len )
{
	uint16_t  ackid;
	
	char tmp[10],i=0;
	/* initialize response */
	char *msg=NULL;
	msg=strstr((char*)buffer,":0,");
	if(msg==NULL)
	{
			return ;
	}
	msg=msg+3;
	pMsgHeader = (uint8_t*)msg;
	while(*msg!=',')
	{
			RspMsgID[MsgID_Index++] = *(msg);
			tmp[i++]=*(msg++);
			
	}
	tmp[i]='\0';
	i=0;
	msg=msg+1;
	ackid=nbiot_atoi(tmp,strlen(tmp));
    
	while(*msg!=',')
	{
	 tmp[i++]=*(msg++);
	}
	tmp[i]='\0';
	i=0;
	msg=msg+1;
	uri.objid=nbiot_atoi(tmp,strlen(tmp));
    
	while(*msg!=',')
	{
			tmp[i++]=*(msg++);
	}
	tmp[i]='\0';
	i=0;
	msg=msg+1;
	uri.instid=nbiot_atoi(tmp,strlen(tmp));
    
	while((*msg!=',')&&(*msg!=0x0D))
	{
			tmp[i++]=*(msg++);
	}
	tmp[i]='\0';
	i=0;
	msg=msg+1;
	uri.resid=nbiot_atoi(tmp,strlen(tmp));
    
	 if(uri.objid !=1)
   {
      uri.flag|= NBIOT_SET_OBJID;
   }
    
   if(uri.instid !=1)
   {
      uri.flag|= NBIOT_SET_INSTID;
   }
    
	 if(uri.resid !=1)
	 {
			uri.flag|= NBIOT_SET_RESID;
	 }
    
   if( COAP_READ == code )
	 {
			delay_ms(50);
			memcpy((void*)&ReadRspTemp[12],(const void*)&RspMsgID[0],5);
			AT_SendCmd((char*)ReadRspTemp,"OK",0,500);
			memcpy((void*)&ReadRspHumi[12],(const void*)&RspMsgID[0],5);
			AT_SendCmd((char*)ReadRspHumi, "OK",0,500);
			memcpy((void*)&ReadRspIllum[12],(const void*)&RspMsgID[0],5);
			AT_SendCmd((char*)ReadRspIllum, "OK",0,500);
			memcpy((void*)&ReadRspLight[12],(const void*)&RspMsgID[0],5);
			AT_SendCmd((char*)ReadRspLight, "OK",0,500);
//        handle_read(dev,&uri,(uint8_t*) pMsgHeader,strlen(msg),ackid);
	 }
	 if( COAP_WRITE == code )
	 {
			 light_control = (*msg)  - 0x30;
			 handle_write(  dev,&uri,ackid,(uint8_t*) msg,strlen(msg) );
	 }

	 if( COAP_EXECUTE == code )
	 {
			 handle_execute( dev,&uri,ackid,(uint8_t*)msg,strlen(msg));
	 }
}

uint8_t buf[1024];
char *pp;
int nbiot_send_buffer(const nbiot_uri_t* uri,uint8_t *buffer,size_t buffer_len,uint16_t ackid,bool updated )
{
  char tmp[8];
	
	size_t  len=0;
	uint8_t type=0;
	uint8_t trigger=0;
	nbiot_uri_t uri1;
	uri1.objid=uri->objid;
	pp=strstr((void*)USART2_RecvBuffer,":0,") + 3;
	while(1)
	{
		if(pp!=NULL)
	  {
			while(*pp!=',')
			{
				tmp[len++]=*pp++;
			}
			tmp[len]='\0';
			pp = pp + 1;
			len=0;
			uri1.instid=nbiot_atoi(tmp,strlen(tmp));
			pp=pp;
			while(*pp!=',')
			{
				tmp[len++]=*pp++;
			}
			tmp[len]='\0';
			pp=pp+1;
			len=0;
			uri1.resid=nbiot_atoi(tmp,strlen(tmp));

			while(*pp!=',')
				tmp[len++]=*pp++;
			tmp[len]='\0';
			pp=pp+1;
			len=0;
			type=nbiot_atoi(tmp,strlen(tmp));
			while(*pp!=';')
				 buf[len++]=*pp++;
			buf[len]='\0';
			pp=pp+1;
			len=0;
			if(*pp=='\0')
				 trigger=1;
			if(updated==true)
			{
					m5310_notify_upload(&uri1,type,(char*)buf,trigger,ackid);
			}
			else
					m5310_read_upload(&uri1,type,(char*)buf,trigger,ackid);
			if(1==trigger)
					break;
		 }
		 else
		 {
				 break;
		 }
	}
    return NBIOT_ERR_OK;
}

int nbiot_recv_buffer( uint8_t *buffer,size_t buffer_len )
{
    return 0;
}



