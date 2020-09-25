#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx.h"
#include "M5310.h"
#include "nbiot.h"
#include "utils.h"
#include "MsgHandler.h"
#include "delay.h"
extern int8_t dl_buf_id;
static char cmd_buff[1024];

void hex_to_hexstring(char *src,char *dst,int len)
{
    int i = 0,j = 0;
    for(i=0,j=0;j<len*2;i++,j++)
    {
      if((src[i]<=9)&&(src[i]>=0))
	    {
				dst[j] =  '0';
		    dst[++j] = src[i] + '0';	
      }
      else  if(src[i]>=0x0A&&src[i]<=0x0F)
      {
        dst[j] =  '0';
		    dst[++j] = src[i] + 55;	
      }
      else if (src[i]>=0x0F)
      {
				dst[j] =  src[i]/16 +'0';
				switch (dst[j])
				{
                case '0': dst[j] = '0';break;
                case '1': dst[j] = '1';break;
                case '2': dst[j] = '2';break;
                case '3': dst[j] = '3';break;
                case '4': dst[j] = '4';break;
                case '5': dst[j] = '5';break;
                case '6': dst[j] = '6';break;
                case '7': dst[j] = '7';break;
                case '8': dst[j] = '8';break;
                case '9': dst[j] = '9';break;
                case ':': dst[j] = 'A';break;
                case ';': dst[j] = 'B';break;
                case '<': dst[j] = 'C';break;
                case '=': dst[j] = 'D';break;
                case '>': dst[j] = 'E';break;
                case '?': dst[j] = 'F';break;
                default : break;
				}
				dst[++j] =  src[i]%16 +'0';
				switch (dst[j])
				{
                case '0': dst[j] = '0';break;
                case '1': dst[j] = '1';break;
                case '2': dst[j] = '2';break;
                case '3': dst[j] = '3';break;
                case '4': dst[j] = '4';break;
                case '5': dst[j] = '5';break;
                case '6': dst[j] = '6';break;
                case '7': dst[j] = '7';break;
                case '8': dst[j] = '8';break;
                case '9': dst[j] = '9';break;
                case ':': dst[j] = 'A';break;
                case ';': dst[j] = 'B';break;
                case '<': dst[j] = 'C';break;
                case '=': dst[j] = 'D';break;
                case '>': dst[j] = 'E';break;
                case '?': dst[j] = 'F';break;
                default : break;
					}
				}
               
				}	 
		 strcat(dst,",1,1\r\n");
		// nbiot_printf(dst);
		 return;
}

uint32_t ip_SendData(uint8_t * buf, uint32_t len)
{
     AT_SendCmd((char*)buf,"OK",0,100);
     return len;
}

void netif_rx(uint8_t*buf,uint16_t *read)
{

}
void M5310_Power_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           	 //开启GPIOB时钟
	
    GPIO_Initure.Pin    = GPIO_PIN_7; 	         //PA7
    GPIO_Initure.Mode   = GPIO_MODE_OUTPUT_PP;   //推挽输出
    GPIO_Initure.Pull   = GPIO_PULLUP;           //上拉
    GPIO_Initure.Speed  = GPIO_SPEED_HIGH;    	 //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);	//控制PA7输出低是平
}

void netdev_init(void)
{
    AT_SendCmd("AT+NRB\r\n", "OK",0,10000); //reboot 模块
    delay_ms(5000);
    AT_SendCmd("AT+CIMI\r\n", "OK",3,100); //返回国标移动设备描述号
		AT_SendCmd("AT+CGSN\r\n", "OK",3,100); //返回国标移动设备描述号
    // SendCmd("AT+CGMR\r\n", "OK", 5000,0,10);//
    AT_SendCmd("AT+CMVER\r\n", "3.",0,500);//检测M5310的固件版本是否为sp3以上
    AT_SendCmd("AT+CMEE=1\r\n","OK",0,500);//设置请允许返回错误码 对于M5310，只能设置0或者1，设置0为禁止返回错误功能
    AT_SendCmd("AT+CSCON=1\r\n","OK",0,500); //查询信号连接状态 
    AT_SendCmd("AT+CEREG=2\r\n","OK",0,500); //使能网络连接状态，选择2类型的返回
    AT_SendCmd("AT+CEDRXS=0,5\r\n","OK",0,500); //禁止eDRX 功能，并选择5这个接入网技术
    AT_SendCmd("AT+CPSMS=0\r\n","OK",0,500); //禁止省电模式(PSM),
    //   SendCmd("AT+CSCON?\r\n","CSCON:1,1", 5000); 
    AT_SendCmd("AT+CEREG?\r\n","CEREG:2,1",30,2000);//查网络状态 
    AT_SendCmd("AT+csq\r\n", "OK",0,500); //返回网络信号强度
    //   SendCmd("AT+CGDCONT?\r\n", "OK", 5000,0,10);
}

void usage( const char *name )
{
//    printf( "Usage: %s [OPTION]\r\n", name );
//    printf( "Launch a client.\r\n" );
//    printf( "version: v1.2\r\n" );
//    printf( "Options:\r\n" );
//    printf( "-b BOOT\t\tSet the bootstrap mode of the client.Default: 0\r\n" );
//    printf( "-d DEBUG\tSet the debug mode of the client.Default: 0\r\n" );
//    printf( "-e ENCRYPT\tSet the encrypt of the client.\r\n" );
//    printf( "-i URI\t\tSet the coap uri of the server to connect to. For example: coap://localhost:5683\r\n" );
//    printf( "-n NAME\t\tSet the endpoint name[imei;imsi] of the client.\r\n" );
//    printf( "-p PORT\t\tSet the local port of the client to bind to. Default: srand\r\n" );
//    printf( "-t TIME\t\tSet the lifetime of the client. Default: 300\r\n" );
//    printf( "-u BLOCK1\tSet COAP option BLOCK1(PUT or POST),0-6. Default 5(512B),2^(4+n)\r\n");
//    printf( "-g BLOCK2\tSet COAP option BLOCK2(GET),0-6. Default 5(512B),2^(4+n)\r\n");
//    printf( "-x BLOCK2TH\tSet max size to trigger block-wise operation,0-2. Default 2(1024B),2^(8+n)\r\n");
//    printf( "\r\n" );
}

void output_buffer1(unsigned char *buffer, int length,int index, int flag)
{

}
/*****************************************************************
       0               1               2               3 
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| boot | encrypt|     debug     |          local_port         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          life_time                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           host_len            |             host            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          ep_name_len          |      ep_name(imei;imsi)     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  coap_block1  |  coap_block2  |    block2th   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*****************************************************************/
void mipl_generate(char *buf, size_t buflen, MIPL_T *mipl)//生成注册码
{
	//strcpy(buf,"AT+MIPLCONF=46,000000002C0100001900636F61703A2F2F3138332E3233302E34302E34303A353638330900313233343B6675636B,1,1\r\n");
//	SendCmd(buf,"OK",300);
	uint16_t offset = 0;
	int mipl_num = 0;
	char mode,temp[5];
	memset(buf, 0, buflen);

	mode = ((mipl->boot & 0x1) << 4) | (mipl->encrypt & 0x1);
	memcpy(buf + offset, &mode, 1);
	offset++;
	memcpy(buf + offset, &mipl->debug, 1);
	offset++;
	memcpy(buf + offset, &mipl->port, 2);
	offset += 2;
	memcpy(buf+offset, &mipl->keep_alive, 4);
	offset += 4;

	memcpy(buf + offset, &mipl->uri_len, 2);
	offset += 2;
	memcpy(buf + offset, mipl->uri, mipl->uri_len);
	offset += mipl->uri_len;
	memcpy(buf + offset, &mipl->ep_len, 2);
	offset += 2;
	memcpy(buf + offset, mipl->ep, mipl->ep_len);
	offset += mipl->ep_len;

    *(buf + offset) = mipl->block1;
	offset++;
	*(buf + offset) = mipl->block2;
	offset++;
	*(buf + offset) = mipl->block2th;
	offset++;

	mipl_num = 0;
	while(offset > 512)//??+MIPLCONF??????1024??????
	{
		//printf("MIPLCONF is too long\r\n");
		return ;
	}

	strcpy((char*)cmd_buff,"AT+MIPLCONF=");
  nbiot_itoa(offset,temp,5);
	strcat((char*)cmd_buff,temp);
	strcat((char*)cmd_buff,",");
	hex_to_hexstring(buf,(char*)(cmd_buff+strlen((char*)cmd_buff)),offset);
	AT_SendCmd((char*)cmd_buff,"OK",0,500);
	//output_buffer1( (unsigned char *)(buf + (mipl_num >> 10)),offset,mipl_num + 1,1);//??
}
//生成，发送注册码
void init_miplconf(uint32_t lifetime,const char *uri,const char *ep)//初始注册码
{
    MIPL_T mipl;
    char buffer[512];
    mipl.boot=MIPL_BOOT;
    mipl.encrypt=MIPL_ENCRYPT;
    mipl.debug=MIPL_DEBUG;
    mipl.port=MIPL_PORT;
    mipl.keep_alive=lifetime;
    mipl.uri=uri;
    mipl.uri_len=strlen(uri);
    mipl.ep=ep;
    mipl.ep_len=strlen(ep);
    mipl.block1 = MIPL_BLOCK1,	//COAP option BLOCK1(PUT or POST),0-6. 2^(4+n)  bytes
    mipl.block2 = MIPL_BLOCK2,	//COAP option BLOCK2(GET),0-6. 2^(4+n)  bytes
    mipl.block2th = MIPL_BLOCK2TH,
    mipl_generate(buffer,sizeof(buffer),&mipl);
}

//增加object
void m5310_addobj(uint16_t objid,uint16_t instid)
{
	char tmp[10];
	memset(cmd_buff,0,50);
	memcpy(cmd_buff,"AT+MIPLADDOBJ=0,",strlen("AT+MIPLADDOBJ=0,"));
	nbiot_itoa(objid,tmp,10);
	strcat((char*)cmd_buff,tmp);
	strcat((char*)cmd_buff,",");
	nbiot_itoa(instid,tmp,1);
	strcat((char*)cmd_buff,tmp);
	strcat((char*)cmd_buff,"\r\n");
	AT_SendCmd((char*)cmd_buff,"OK",0,500);
}


//向平台注册，
size_t m5310_register_request(void)
{
	uint8_t status=0;

	status=AT_SendCmd("AT+MIPLOPEN=0,30\r\n","OK",0,500);

	if(status==0)
	{
		AT_SendCmd("AT+MIPLCLOSE=0\r\n","OK",0,5); 
	}
	return 0;
}

size_t m5310_register_update (uint16_t lifttime, uint8_t *buffer,size_t  buffer_len)
{
  size_t  len=0;
	//memcpy(buffer,"AT+MIPLUPDATE=0,1000,0\r\n",len);
	char ative[6]={0};
  nbiot_itoa(lifttime,ative,6);
	memcpy(buffer,"AT+MIPLUPDATE=0,",sizeof("AT+MIPLUPDATE=0,"));
	strcat((char*)buffer,ative);
	strcat((char*)buffer,",0\r\n");
	len=strlen((const char*)buffer)+1;
	if(len<buffer_len)
	{
		 AT_SendCmd((char*)buffer,"OK",0,500);
		 return len;
  }
	return 0;
}

void m5310_notify_upload(const nbiot_uri_t *uri,uint8_t type,char *data,int tranmmit,uint16_t ackid)
{
  char tmp[10];
	memset(cmd_buff,0,sizeof(cmd_buff));
	memcpy(cmd_buff,"AT+MIPLNOTIFY=0,",strlen("AT+MIPLNOTIFY=0,"));
	nbiot_itoa(uri->objid,tmp,10);
	strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
	nbiot_itoa(uri->instid,tmp,10);
	strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
	nbiot_itoa(uri->resid,tmp,10);
	strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
  nbiot_itoa(type,tmp,1);
  strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
	strcat(cmd_buff,"\"");
	strcat(cmd_buff,data);
	strcat(cmd_buff,"\"");
	strcat(cmd_buff,",");
  nbiot_itoa(tranmmit,tmp,1);
	strcat(cmd_buff,tmp);
#ifdef NOTIFY_ACK
if(tranmmit==1){
	strcat(cmd_buff,",");
	nbiot_itoa(ackid,tmp,10);
	strcat(cmd_buff,tmp);
	strcat(cmd_buff,"\r\n");
}else{

   strcat(cmd_buff,"\r\n");

}

#else

    strcat(cmd_buff,"\r\n");

#endif
	  AT_SendCmd(cmd_buff,"OK",0,100);
}

void m5310_read_upload(const nbiot_uri_t *uri,uint8_t type,char *data,int tranmmit,uint16_t ackid)
{
  char tmp[10];
	memset(cmd_buff,0,sizeof(cmd_buff));
	memcpy(cmd_buff,"AT+MIPLREAD=0,",strlen("AT+MIPLREAD=0,"));
	nbiot_itoa(ackid,tmp,10);
	strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
	nbiot_itoa(uri->objid,tmp,10);
	strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
	nbiot_itoa(uri->instid,tmp,10);
	strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
	nbiot_itoa(uri->resid,tmp,10);
	strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
  nbiot_itoa(type,tmp,1);
  strcat(cmd_buff,tmp);
	strcat(cmd_buff,",");
	strcat(cmd_buff,"\"");
	strcat(cmd_buff,data);
	strcat(cmd_buff,"\"");
	strcat(cmd_buff,",");
  nbiot_itoa(tranmmit,tmp,1);
	strcat(cmd_buff,tmp);
  strcat(cmd_buff,"\r\n");

	AT_SendCmd(cmd_buff,"OK",0,100);
}
//收到写数据后+MIPLWRITE，用这个返回，suc,是执行结果1表明执行成功，0失败
//ackid,在接收的+MIPLWRITE信息中，自带的消息id,根据手册说明回复必须回应这个消息ID。
void m5310_write_rsp(int suc,uint16_t ackid)
{
   char tmp[10];
	 memset(cmd_buff,0,50);
   memcpy(cmd_buff,"AT+MIPLWRITE=0,",strlen("AT+MIPLWRITE=0,"));
	 nbiot_itoa(ackid,tmp,10);
	 strcat(cmd_buff,tmp);
	 strcat(cmd_buff,",");
	 nbiot_itoa(suc,tmp,1);
	 strcat(cmd_buff,tmp);
	 strcat(cmd_buff,"\r\n");

	 AT_SendCmd(cmd_buff,"OK",0,100);
}

void m5310_execute_rsp(int suc,uint16_t ackid)
{
   char tmp[10];
	 memset(cmd_buff,0,50);
	 memcpy(cmd_buff,"AT+MIPLEXECUTE=0,",strlen("AT+MIPLEXECUTE=0,"));
	 nbiot_itoa(ackid,tmp,10);
	 strcat(cmd_buff,tmp);
	 strcat(cmd_buff,",");
	 nbiot_itoa(suc,tmp,1);
	 strcat(cmd_buff,tmp);
	 strcat(cmd_buff,"\r\n");

   AT_SendCmd(cmd_buff,"OK",0,100);
}
//注销登陆
size_t m5310_close_request( uint8_t *buffer, size_t  buffer_len)
{   
  size_t  len=0;
	len=strlen("AT+MIPLCLOSE=0\r\n")+1;
	if(len<buffer_len)
	{
			 memcpy(buffer,"AT+MIPLCLOSE=0\r\n",len);
			 AT_SendCmd("AT+MIPLCLOSE=0\r\n","OK",0,5);
			 return len;
	}
	return 0;
} 
//删除object
void m5310_delobj(uint16_t  objid,uint16_t  instid)
{
	 char tmp[10];
	 memset(cmd_buff,0,50);
	 memcpy(cmd_buff,"AT+MIPLDELOBJ=0,",strlen("AT+MIPLDELOBJ=0,"));
	 nbiot_itoa(objid,tmp,10);
	 strcat((char*)cmd_buff,tmp);
	 strcat((char*)cmd_buff,",");
	 nbiot_itoa(instid,tmp,10);
	 strcat((char*)cmd_buff,tmp);
	 strcat((char*)cmd_buff,"\r\n");
	 AT_SendCmd((char*)cmd_buff,"OK",0,5);
 } 

