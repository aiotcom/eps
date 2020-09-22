#include "WiFi.h"
#include "delay.h"
#include "Usart.h"
#include "WS2812b.h"
#define WIFI_RECEIVE_BUFFER_LEN 128
uint8_t WiFi_ReceiveBuffer[WIFI_RECEIVE_BUFFER_LEN+1],ReceiveLenght=0;
uint8_t WiFi_AT_STA = 0;
uint8_t* WiFi_Send_Cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime);
/**
  * @brief  WIFI连接ONE.NET平台AT指令
  * @param  void
  * @retval void
  **/
void WiFi_Init()
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟

    GPIO_Initure.Pin   = GPIO_PIN_5;   //GPIO_PIN_4:WIFI模块复位控制管脚，GPIO_PIN_6:WS2812B彩灯控制管脚。
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;	//推挽输出
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW; //低速
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
    
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);//初始电平均为高
    
    //(以下三行代码)输出2毫秒的低电平信号复位WIFI模块
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
    HAL_Delay(10);//延时2毫秒
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
    
    HAL_Delay(2000);//延时2000毫秒，因WIFI模块复位重启后会打印出许多开机信息，延时两秒避开。
    
    
    while(!WiFi_Send_Cmd(AT,(uint8_t*)"OK",20));				    /*进入AT命令模式*/
    //WS2812B_SetColour(0x000055);                                    //红色提示
    
    while(!WiFi_Send_Cmd(CWMODE,(uint8_t*)"OK",30));				/*设置模块应用模式*/

    while(!WiFi_Send_Cmd(CWJAP,(uint8_t*)"OK",1000));				/*配置要连接的路由器的SSID和密码*/   
    //WS2812B_SetColour(0x005500);                                    //红色提示    
    
    
    while(!WiFi_Send_Cmd(TCP_CIPSTART,(uint8_t*)"OK",3000));	/*与服务器建立TCP连接*/
    //WS2812B_SetColour(0x550000);                                    //绿色提示
    
    while(!WiFi_Send_Cmd(CIPMODE,(uint8_t*)"OK",500));				/*透传模式*/

    while(!WiFi_Send_Cmd(CIPSEND,(uint8_t*)"OK",500));				/*开始传输*/

    WiFi_AT_STA = 1;           										/*AT指令发送完成标志位*/
}
//==========================================================
//	函数名称：	WiFi_Send_Cmd()
//
//	函数功能：	向WiFi发送命令
//
//	入口参数：	waittime:等待时间(单位:ms)
//							cmd:发送的命令字符串
//							ack:期待的应答结果,如果为空,则表示不需要等待应答
//
//	返回参数：	
//						返回值:	非0,发送成功(得到了期待的应答结果)
//       							0,发送失败
//
//	说明：		
//==========================================================
uint8_t* WiFi_Send_Cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
	static uint8_t len = 0;							//存放数据长度
	static uint16_t j = 0;
	char *strx=0;

	len = strlen((char *)cmd);					//获取数据长度

	memset((void*)WiFi_ReceiveBuffer,0,WIFI_RECEIVE_BUFFER_LEN);	//清空数组
	HAL_UART_Transmit(&UART2_Handler,cmd,len,1000);	//串口2发送数据
	ReceiveLenght = 0;
    
    if((*ack) != 0)
    {
        for(j=0;j<waittime;j++)
        {
            HAL_Delay(1);
            strx=strstr((const char*)WiFi_ReceiveBuffer,(const char*)ack);
            if((strx))
            {
                break;
            }
        }
    }
    else
    {
        HAL_Delay(waittime);
        return (uint8_t*)1;
    }
	return (uint8_t*)strx;
}
//==========================================================
//	函数名称：	void WiFi_Send(uint8_t *pdata,uint8_t len)
//
//	函数功能：	向WiFi发送命令
//
//	入口参数：	waittime:等待时间(单位:ms)
//							cmd:发送的命令字符串
//							ack:期待的应答结果,如果为空,则表示不需要等待应答
//
//	返回参数：	
//						返回值:	非0,发送成功(得到了期待的应答结果)
//       							0,发送失败
//
//	说明：		
//==========================================================
void WiFi_Send(uint8_t *pdata,uint8_t len)
{
    HAL_UART_Transmit(&UART2_Handler,pdata,len,1000);	//串口2发送数据
}
//==========================================================
//	函数名称：	void WiFi_Send(uint8_t *pdata,uint8_t len)
//
//	函数功能：	向WiFi发送命令
//
//	入口参数：	waittime:等待时间(单位:ms)
//							cmd:发送的命令字符串
//							ack:期待的应答结果,如果为空,则表示不需要等待应答
//
//	返回参数：	
//						返回值:	非0,发送成功(得到了期待的应答结果)
//       							0,发送失败
//
//	说明：		
//+IPD,0,23:this is sample data;接收的数据格式，从第二个","开始到“:” 为数据长度，“:”之后为数据内容
//==========================================================
uint8_t *pbyte;
uint8_t datalen;
uint8_t WiFi_Receive(uint8_t *precv)
{
    static uint8_t state;
    uint8_t i = 0,len = 0,markcount=0;
    
    if(ReceiveLenght)
    {
        HAL_Delay(20);    
        if(strstr((const void*)WiFi_ReceiveBuffer,"+IPD,"))
        {
            while(i<100)
            {//找到第二个逗号位置
                if(WiFi_ReceiveBuffer[i] == ',')
                {
                    markcount++;
                }
                if(markcount == 2)
                {
                    pbyte = &WiFi_ReceiveBuffer[i+1];
                    break;
                }
                i++;
            }
            
            datalen = 0;
            while((*pbyte) != ':')
            {//解出接长度
                datalen *= 10;
                datalen  = datalen + (*pbyte-0x30);
                pbyte++;
            }
            printf("dl=%d\r\n",datalen);
            
            pbyte++;
            while(datalen--)
            {
                *precv = *(pbyte);
                pbyte++;
                precv++;
                len++;
            }                
        }
        ReceiveLenght = 0;
    }           
    return len;
}
//==========================================================
//	函数名称：	WiFi_Send_Cmd()
//
//	函数功能：	向WiFi发送命令
//
//	入口参数：	waittime:等待时间(单位:ms)
//							cmd:发送的命令字符串
//							ack:期待的应答结果,如果为空,则表示不需要等待应答
//
//	返回参数：	
//						返回值:	非0,发送成功(得到了期待的应答结果)
//       							0,发送失败
//
//	说明：		
//==========================================================
void WiFi_ByteReceive(uint8_t byte)
{
    WiFi_ReceiveBuffer[ReceiveLenght++] = byte;
    if(ReceiveLenght >= WIFI_RECEIVE_BUFFER_LEN)
    {
        ReceiveLenght = 0;
    }
}

















