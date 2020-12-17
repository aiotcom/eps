#include "Usart.h"	
#include "WiFi.h"

//==========================================================
//	函数名称：	WiFi_Init()
//
//	函数功能：	初始化WiFi
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void WiFi_Init(void)
{
    P1DIR |= 0x60;      //P1.5、P1.6定义为输出
    P1SEL &= ~0x80;     //设置P1.7为普通IO口  
    P1DIR &= ~0x80;     //按键接在P1.7口上，设P1.7为输入模式 
    P1INP &= ~0x80;     //打开P1.7上拉电阻

//    WiFi_Send_Cmd(CLOSE_TCP,"",5);			/*退出WiFi透传模式*/
//
//    delay_ms(3000);

    WiFi_RES = 0;
    delay_ms(1);
    WiFi_RES = 1;

    WiFi_LED_SET(0x17);

    while(!WiFi_Send_Cmd(AT,"OK",20));		        /*进入AT命令模式*/		

    WiFi_LED_SET(0x1700);

    while(!WiFi_Send_Cmd(CWMODE,"OK",30));		/*设置模块应用模式*/	

    while(!WiFi_Send_Cmd(CWJAP,"OK",300));		/*配置要连接的路由器的SSID和密码*/	

    WiFi_LED_SET(0x170000);

    while(!WiFi_Send_Cmd(TCP_CIPSTART,"OK",60));	/*与服务器建立TCP连接*/								

    while(!WiFi_Send_Cmd(CIPMODE,"OK",60));		/*透传模式*/

    while(!WiFi_Send_Cmd(CIPSEND,"OK",30));		/*开始传输*/									

    while(!WiFi_Send_Cmd(CIPSTATUS,"",30));		/*连接设备*/

    WiFi_LED_SET(0x000000);
    
    WiFi_AT_STA = 1;           			        /*AT指令发送完成标志位*/	
}

/***********************************************************************************
* 函数名称        WiFi_LED_REST()
*
* 函数功能        拉低电平等待
*
* 入口参数        无
*
* 返回参数        无 
*
* 说明            
***********************************************************************************/
/* 拉低电平等待下一次命令  */
void WiFi_LED_REST(void)
{
    WiFi_LED = 0;
    delay_ms(1);  
}

//==========================================================
//	函数名称：	WiFi_LED_SET()
//
//	函数功能：	设置颜色
//
//	入口参数：	G8R8B8:颜色
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void WiFi_LED_SET(uint32_t G8R8B8)
{
    static int i = 0;
    static unsigned char byte = 0;
    
    for(i=23;i>=0;i--)
    {
        byte = ((G8R8B8 >> i) & 0x01);
        if(byte == 1)
        {
            WiFi_LED_HI;//1
        }
        else
        {
            WiFi_LED_LO; //0
        }
    }
    WiFi_LED_REST();
}

//==========================================================
//	函数名称：	WiFi_Set_White()
//
//	函数功能：	白色
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void WiFi_Set_White(void)
{
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;

  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;  
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;
  WiFi_LED_HI;  
}