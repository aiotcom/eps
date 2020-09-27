#include "stm32f1xx.h"
#include "delay.h"
#include "string.h"
#include "M5310.h"
#include "nbiot.h"
#include "coap.h"
#include "Usart.h"
//==========================================================
//	函数名称：	SendCmd
//
//	函数功能：	初始化串口2，波特率依据入口参数9600
//
//	入口参数：	uint8_t  *cmd,     	将要发送的命令通常是个字符串
//              uint8_t  *result，	期待返回的一个结果，
//              uint8_t  retry,   	重发次数，
//              uint16_t waittime, 	等待回应时间，时间单位(毫秒ms)
//	返回参数：	返回1，命令发送成功。
//              返回0，命令发送失败。
//	说明：		
//==========================================================
char AT_SendCmd(char* cmd, char *expected_result,uint8_t retry,uint16_t waittime)
{
	 uint8_t  ret = 0;
	 uint16_t wt=0;
	 do
	 {
			USART2_RecvIndex = 0;
			memset((void*)USART2_RecvBuffer,0,USART2_RECV_BUFFER_LENGTH);

			USART2_SendBytes(cmd,strlen((char*)cmd));
			wt = waittime;
			while(wt--)
			{
					if(strstr((void*)USART2_RecvBuffer,(const char*)expected_result))
					{//收到了期待的返回
							retry = 0;
							ret = 1;
							break;//跳出while(waittime--)
					}
					delay_ms(1);
			}
	 }while(retry--);

	 return ret;//程序运行到此，ret非零表明收到了正确回复，反则之。
}
 
//==========================================================
//	函数名称：	Msg_Handler
//
//	函数功能：	解析USART2_RecvBuffer[]是否有期望的消息格式，针对不同的消息格式进行相应的操作
//
//	入口参数：	无
//
//	返回参数：	无
// 
//	说明：		
//==========================================================
uint8_t Msg_Handler(void)
{
	uint8_t code = 0;
	char *read=NULL,*write=NULL,*excute=NULL;
	char *ack=NULL,*observe=NULL,*reg_rsp=NULL,*close_rsp=NULL,*update_rsp=NULL,*discover=NULL;
		 
	read       = strstr((const char *)USART2_RecvBuffer, "+MIPLREAD");    
	write      = strstr((const char *)USART2_RecvBuffer, "+MIPLWRITE");
	excute     = strstr((const char *)USART2_RecvBuffer, "+MIPLEXECUTE");
	ack        = strstr((const char *)USART2_RecvBuffer, "+MIPLNOTIFY");
	observe    = strstr((const char *)USART2_RecvBuffer, "+MIPLOBSERVE");
	reg_rsp    = strstr((const char *)USART2_RecvBuffer, "+MIPLOPEN");
	close_rsp  = strstr((const char *)USART2_RecvBuffer, "+MIPLCLOSE");
	update_rsp = strstr((const char *)USART2_RecvBuffer, "+MIPLUPDATE");    
	discover   = strstr((const char *)USART2_RecvBuffer, "+MIPLDISCOVER");  
	if(read != NULL)
	{
		 code = COAP_READ;
	}
	else if(write != NULL)
	{
		 code = COAP_WRITE;
	}
	else if(excute != NULL)
	{
		 code = COAP_EXECUTE;
	}
	else if(ack != NULL)
	{
		 code = COAP_ACK;
	}
	else if(observe != NULL)
	{
		 code = COAP_OBSERVE;
	}
	else if(reg_rsp!=NULL)
	{
		 code = COAP_REG_RSP;
	}
	else if(close_rsp != NULL)
	{
		 code = COAP_CLOSE_RSP;
	}
	else if(update_rsp != NULL)
	{
		 code = UPDATE_RSP;  
	}
	else if(discover != NULL)
	{
		 code = DISCOVER;  
	}
	if((write != NULL) ||(read != NULL)||(excute != NULL))
	{
			delay_ms(100);
		 // m5310_write_rsp(1, ackid);
			handle_request((nbiot_device_t *)0,code,USART2_RecvBuffer,strlen((char*)USART2_RecvBuffer),USART2_RECV_BUFFER_LENGTH);
	}
	
	return code;
}

 


 