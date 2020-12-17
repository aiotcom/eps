#include "Usart.h"	
#include "delay.h"
#include "Rs485.h"
#include "Time.h"

#if UART0   //使能接收

unsigned char WiFi_AT_STA = 0;                  //WiFi初始化完成标志位
unsigned char USART0_RX_BUF[USART0_REC_LEN];    //接收缓冲,最大USART0_REC_LEN个字节.
unsigned int  USART0_RX_STA = 0;       		//接收状态标记	
uint8_t Temp = 0;				//温度数据
uint8_t Humi = 0;				//湿度数据
uint8_t Command_LED[10];			//控制LED灯命令码
uint8_t Send_OneNET[10];			//发送到OneNET显示数据

//==========================================================
//	函数名称：	USRT0_Init()
//
//	函数功能：	初始化串口0
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void USRT0_Init(void)
{
  PERCFG = 0x00;    //串口0的引脚映射到位置1，即P0_2和P0_3
  P0SEL = 0x0C;     //将P0_2和P0_3端口设置成外设功能
  U0BAUD = 216;     //32MHz的系统时钟产生115200BPS的波特率
  U0GCR = 11;
  U0UCR |= 0x80;    //禁止流控，8位数据，清除缓冲器
  U0CSR |= 0xC0;    //选择UART模式，使能接收器
  UTX0IF = 0;       //清除TX发送中断标志
  URX0IF = 0;       //清除RX接收中断标志
  URX0IE = 1;       //使能URAT0的接收中断
  EA = 1;           //使能总中断
}

//==========================================================
//	函数名称：	UR0_RecvInt()
//
//	函数功能：	串口0接收中断服务函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
#pragma vector = URX0_VECTOR
__interrupt void UR0_RecvInt()
{
  URX0IF = 0;                             //清除RX接收中断标志

  if((USART0_RX_STA&0x8000)==0)	          //接收未完成
  {
    if(WiFi_AT_STA)                       //WiFi初始化完毕
    {
        USART0_Receive(U0DBUF);		  //串口0数据处理
    }
    else
    {
        USART0_RX_BUF[USART0_RX_STA&0X3FFF]=U0DBUF ;
        USART0_RX_STA++;
        if(USART0_RX_STA>(200-1))USART0_RX_STA=0;//接收数据错误,重新开始接收	  
    }
  } 
}

//==========================================================
//	函数名称：	USART0_Receive()
//
//	函数功能：	串口0接收
//
//	入口参数：	data:串口0数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void USART0_Receive(uint8_t data)
{
  static uint8_t End_Flag 	= 0;	//帧尾标志位
  static uint8_t Header_Flag 	= 0;	//帧头标志位
  static uint8_t Receive_Flag   = 0;	//接收标志位

/*
*
*	帧头接收
*
*/
  /*帧头1 数据帧头中第一个字节*/
  if(Header_Flag == 0 && data == 0x43)
  {
      Header_Flag = 1;
  }

  /*帧头2 数据帧头中第二个字节*/
  else if(Header_Flag == 1 && data == 0x47)
  {
      USART0_RX_STA = 1;  //存放数据数组下标清零
      Receive_Flag = 1;	  //开始接收数据标志位
      USART0_RX_BUF[0] = 0x41;//将缓存值存到接收数组中
  }

  else Header_Flag = 0;
  
/*
*
*	数据+帧尾接收
*
*/
  if(Receive_Flag)	//帧头接收完成
  {
      if(USART0_RX_STA>=(Receive_REC_LEN-1))
      {
          End_Flag = 0;
          Receive_Flag = 0;
          USART0_RX_STA = 0;
      }
      
      USART0_RX_BUF[USART0_RX_STA++] = data;//数据接收

      /*帧尾1  数据帧尾中第一个字节*/
      if(!End_Flag && data == 0x53)
      {
          End_Flag = 1;
      }
      /*帧尾2  数据帧尾中第二个字节*/
      else if(End_Flag && data == 0x57)
      {
          USART0_RX_STA = 0x8000;	//接收完成
          Receive_Flag = 0;
          End_Flag = 0;
      }

      /*接收错误 重新接收*/
      else End_Flag = 0;
  }
}

//==========================================================
//	函数名称：	USART0_SendByte()
//
//	函数功能：	串口0发送单字节函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void USART0_SendByte(unsigned char dat)
{
  U0DBUF = dat;         //将要发送的1字节数据写入U0DBUF
  while(!UTX0IF);       //等待TX中断标志，即数据发送完成
  UTX0IF = 0;           //清除TX中断标志，准备下一次发送
}

//==========================================================
//	函数名称：	USART0_SendString()
//
//	函数功能：	串口0发送字符串函数
//
//	入口参数：	*str: 发送的数据
//                      Len:  数据长度
//          
//	返回参数：	无
//
//	说明：		
//==========================================================
void USART0_SendString(unsigned char *str,uint16_t Len)
{
  while(Len--)
  {
    USART0_SendByte(*str++);    //逐个发送字符串中的字节
  }
}

//==========================================================
//	函数名称：	WiFi_Send_Cmd()
//
//	函数功能：	向WiFi发送命令
//
//	入口参数：	waittime:等待时间(单位:ms)
//			cmd:发送的命令字符串
//			ack:期待的应答结果,如果为空,则表示不需要等待应答
//
//	返回参数：	
//		        返回值:	非0,发送成功(得到了期待的应答结果)
//       			  0,发送失败
//
//	说明：		
//==========================================================
uint8_t* WiFi_Send_Cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
    static uint8_t len = 0;	//存放数据长度
    static uint16_t j = 0;
    char *strx=0;
  
    len = strlen((char *)cmd);	//获取数据长度
  
    memset((void*)USART0_RX_BUF,0,USART0_REC_LEN);//清空数组
    
    USART0_SendString(cmd,len); //串口0发送数据
    
    for(j=0;j<waittime;j++)
    {
        delay_ms(50);
        strx=strstr((const char*)USART0_RX_BUF,(const char*)ack);
        if(strx)
          break;
    }

    USART0_RX_STA=0;
    return (uint8_t*)strx;
}

//==========================================================
//	函数名称：	USART0_IRQHand()
//
//	函数功能：	处理串口0数据
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：				
//==========================================================
void USART0_IRQHand(void)
{
  static uint16_t Command;	//云平台下发命令
  
  if(USART0_RX_STA == 0x8000)
  {
    Command = ((USART0_RX_BUF[2]-0x30)*1000)+((USART0_RX_BUF[3]-0x30)*100)
    +((USART0_RX_BUF[4]-0x30)*10)+(USART0_RX_BUF[5]-0x30);//获取平台下发命令码
  
    switch(Command)
    {
      case LED_ON1:
        Command_LED[0] = 1;	//将命令码装载到LED命令码中
        break;
      case LED_OFF1:
        Command_LED[0] = 0;	//将命令码装载到LED命令码中
        break;
      case LED_ON2:
        Command_LED[1] = 1;	//将命令码装载到LED命令码中
        break;
      case LED_OFF2:
        Command_LED[1] = 0;	//将命令码装载到LED命令码中
        break;
      case LED_ON3:
        Command_LED[2] = 1;	//将命令码装载到LED命令码中
        break;
      case LED_OFF3:
        Command_LED[2] = 0;	//将命令码装载到LED命令码中
        break;
      case LED_ON4:
        Command_LED[3] = 1;	//将命令码装载到LED命令码中
        break;
      case LED_OFF4:
        Command_LED[3] = 0;	//将命令码装载到LED命令码中
        break;
      default: break;
    }
    memset((void*)USART0_RX_BUF,0,10);//清空数组
    USART0_RX_STA = 0;
  }
}

#endif


#if UART1   //使能接收

unsigned char USART1_RX_BUF[USART1_REC_LEN];    //接收缓冲,最大USART1_REC_LEN个字节.
unsigned int  USART1_RX_STA=0;       		//接收状态标记	

//==========================================================
//	函数名称：	USRT1_Init()
//
//	函数功能：	串口1初始化函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void USRT1_Init(void)
{
  P2DIR = 0x40;
  PERCFG = 0x00;    //串口1的引脚映射到位置1，即P0_4和P0_5
  P0SEL |= 0x30;    //将P0_4和P0_5端口设置成外设功能
  U1BAUD = 216;     //32MHz的系统时钟产生115200BPS的波特率
  U1GCR = 11;
  U1UCR = 0x80;     //禁止流控，8位数据，清除缓冲器
  U1CSR = 0xC0;     //选择UART模式，使能接收器
  UTX1IF = 0;       //清除TX发送中断标志
  URX1IF = 0;       //清除RX接收中断标志
  URX1IE = 1;       //使能URAT1的接收中断
  EA = 1;           //使能总中断
}

//==========================================================
//	函数名称：	UR1_RecvInt()
//
//	函数功能：	UR1接收中断服务函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
#pragma vector = URX1_VECTOR
__interrupt void UR1_RecvInt()
{
  URX1IF = 0;                   //清除RX接收中断标志
  
  if((USART1_RX_STA&0x8000)==0)	//接收未完成
  {
  } 
}

//==========================================================
//	函数名称：	USART1_SendByte()
//
//	函数功能：	串口1发送单字节函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void USART1_SendByte(unsigned char dat)
{
  U1DBUF = dat;         //将要发送的1字节数据写入U1DBUF
  while(!UTX1IF);       //等待TX中断标志，即数据发送完成
  UTX1IF = 0;           //清除TX中断标志，准备下一次发送
}

//==========================================================
//	函数名称：	USART1_SendString()
//
//	函数功能：	串口1发送字符串函数
//
//	入口参数：	*str: 发送的数据
//                      Len:  数据长度
//          
//	返回参数：	无
//
//	说明：		
//==========================================================
void USART1_SendString(unsigned char *str,uint16_t Len)
{
  while(Len--)
  {
    USART1_SendByte(*str++);    //逐个发送字符串中的字节
  }
}

//==========================================================
//	函数名称：	USART1_IRQHand()
//
//	函数功能：	处理485数据
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：				
//==========================================================
void USART1_IRQHand(void)
{
  if(!DataHandling_485(Addr_WiFi))	//485数据处理
  {
    if(Rx_Stack.Src_Adr == Addr_SHT20)	//获取温湿度数据
    {
      Temp = Rx_Stack.Data[0];		//温度数据
      Humi = Rx_Stack.Data[1];		//湿度数据
    }
  }
}

//==========================================================
//	函数名称：	USART_Data_Send()
//
//	函数功能：	串口数据发送函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
//==========================================================
void USART_Data_Send(void)
{
  static uint8_t Send_Flag = 0;
  
  if(Task_Count>=200 && Send_Flag == 0)	//200ms	控制LED模块
  {
    Rs485_Send(Addr_WiFi,Addr_LED,LED_Control,4,Command_LED);
    Send_Flag = 1;
  }
  else if(Task_Count>=300 && Send_Flag == 1)//300ms 获取温湿度数据
  {
    Rs485_Send(Addr_WiFi,Addr_SHT20,SHT20_Get_All,0,"");
    Send_Flag = 0;
    Task_Count = 0;
  }

  if(Send_OneNET_Count>=1550)
  {
    sprintf((char *)Send_OneNET,"%d%d",Temp,Humi);
    USART0_SendString(Send_OneNET,4);//发送数据到OneNET
    Send_OneNET_Count = 0;
  }
}

#endif