#include "Usart.h"	
#include "delay.h"
#include "OLED.h"

#if UART0   //使能接收

unsigned char WiFi_AT_STA = 0;                  //WiFi初始化完成标志位
unsigned char USART0_RX_BUF[USART0_REC_LEN];    //接收缓冲,最大USART0_REC_LEN个字节.
unsigned int  USART0_RX_STA = 0;       		//接收状态标记	
uint8_t Command_LED[10];		        //控制LED灯命令码

//==========================================================
//	函数名称：	Init_Uart0()
//
//	函数功能：	UR0初始化函数
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
  U0BAUD = 59;      //16MHz的系统时钟产生9600BPS的波特率
  U0GCR = 8;
  U0UCR |= 0x80;    //禁止流控，8位数据，清除缓冲器
  U0CSR |= 0xC0;    //选择UART模式，使能接收器
  UTX0IF = 0;       //清除TX发送中断标志
  URX0IF = 0;       //清除RX接收中断标志
  URX0IE = 1;       //使能URAT0的接收中断
  EA = 1;           //使能总中断
}

//==========================================================
//	函数名称：	StrnCmp
//
//	函数功能：	比较两个字符的 n 位是否相等
//
//	入口参数：	uint8_t *p1,指向数据
//              uint8_t *p2,指向数据
//              uint8_t n  ,期望比较的位数
//	返回参数：	无
//
//	说明：		
//==========================================================
uint8_t StrnCmp(uint8_t *p1,uint8_t *p2,uint8_t n)
{
    uint8_t i=0,ret =1;
    
    for(i=0;i<n;i++)
    {
        if((*p1) != (*p2))
        {
            ret = 0;
            break;
        }
    }
    
    return ret;
}

//==========================================================
//	函数名称：	UR0_RecvInt()
//
//	函数功能：	UR0接收中断服务函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
uint8_t  RecvSta = 0;
uint8_t  GetGGA_Msg=0;
#pragma vector = URX0_VECTOR
__interrupt void UR0_RecvInt()
{
  URX0IF = 0;                             //清除RX接收中断标志

    switch(RecvSta)
    {
        case 1:
           USART0_RX_BUF[USART0_RX_STA++]=U0DBUF;
           if(USART0_RX_STA == 6)
           {
               if(StrnCmp(USART0_RX_BUF,(uint8_t*)"$GNGGA",sizeof("$GNGGA")))
               {//如果收到的消息头是GNGGA，格式的。
                    RecvSta       = 2;   
               }
               else
               {//如果不是GNGGA,格式的重新接收
                    USART0_RX_STA = 0;
                    RecvSta       = 0; 
               }    
           }
        break;
        case 2://如是GNGGA格式的消息，余下的数据在这里接收，
           USART0_RX_BUF[USART0_RX_STA++] = U0DBUF;
           if(U0DBUF == '\n')//消息以 “\r\n”结尾
           {//接收完成
                GetGGA_Msg = 1;                    //置位GetGGA_Msg，在main()->while(1){...},检测这个标志位
                RecvSta    = 0;                    //RecvSta = 0,继续等待下一条协议到来
           }
           
           if(USART0_RX_STA >= 200)
           {//USART2_RX_STA 大于 USART2_RX_BUF数据的长度，异常情况下会发生这种情况。
                USART0_RX_STA = 0;
           }
        break;
        default:break;
    }
    
    if(U0DBUF == '$')//GPS消息的的头均于 $ 开头，\r\n 结尾
    {
        USART0_RX_STA                  = 0;
        USART0_RX_BUF[USART0_RX_STA++] = U0DBUF;
        RecvSta                        = 1; //接收到 $ ,RecvSta 进入 状态 1进行数据接收。 
    }
}

//==========================================================
//	函数名称：	UR0SendByte()
//
//	函数功能：	UR0发送单字节函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void UR0SendByte(unsigned char dat)
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
  PERCFG = 0x00;  //串口1的引脚映射到位置1，即P0_4和P0_5
  P0SEL |= 0x30;  //将P0_4和P0_5端口设置成外设功能
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

#endif

//==========================================================
//	函数名称：	DataHandling_GPS()
//
//	函数功能：	GPS数据处理
//
//	入口参数：	huart:串口句柄
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void DataHandling_GPS(void)
{

}


__near_func int putchar(int c)
{
#if 1  
    UTX0IF = 0;
    U0DBUF = (char)c;
    while(UTX0IF == 0);
    return(c);
#else
    UTX1IF = 0;
    U1DBUF = (char)c;
    while(UTX1IF == 0);
    return(c);    
#endif    
}


