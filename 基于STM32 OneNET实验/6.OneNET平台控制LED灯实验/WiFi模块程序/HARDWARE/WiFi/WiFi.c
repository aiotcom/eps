#include "WiFi.h"
#include "delay.h"
#include "Usart.h"

uint8_t WiFi_AT_STA = 0;

//==========================================================
//	函数名称：	WiFi_Init()
//
//	函数功能：	WIFI连接ONE.NET平台AT指令
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void WiFi_Init()
{
		WiFi_RST_Init();	//初始化WiFi模块RST
		WiFi_LED_Init();	//初始化WiFi模块LED灯
		WiFi_KEY_Init();	//初始化WiFi模块按键
	
		while(!WiFi_Send_Cmd(AT,"OK",20));						/*进入AT命令模式*/
        printf("%s\r\n",AT);
		WiFi_RGB_SET(0x3300);													//红灯
		printf("%s\r\n",CWMODE);	
		while(!WiFi_Send_Cmd(CWMODE,"OK",30));				/*设置模块应用模式*/
        printf("%s\r\n",CWJAP);	
		while(!WiFi_Send_Cmd(CWJAP,"OK",300));				/*配置要连接的路由器的SSID和密码*/
        
		WiFi_RGB_SET(0x33);														//蓝灯
        printf("%s\r\n",TCP_CIPSTART);	
		while(!WiFi_Send_Cmd(TCP_CIPSTART,"OK",60));	/*与服务器建立TCP连接*/
        	
		WiFi_RGB_SET(0x330000);												//绿灯
        printf("%s\r\n",CIPMODE);	
		while(!WiFi_Send_Cmd(CIPMODE,"OK",60));				/*透传模式*/
        printf("%s\r\n",CIPSEND);	       	
		while(!WiFi_Send_Cmd(CIPSEND,"OK",30));				/*开始传输*/
		while(!WiFi_Send_Cmd(OnenNET,"",30));				/*连接OneNET*/     
		WiFi_RGB_SET(0x000000);												//灭灯
		WiFi_RGB_REST();															//灭灯
	
		WiFi_AT_STA = 1;           										/*AT指令发送完成标志位*/
}

//==========================================================
//	函数名称：	WiFi_RST_Init()
//
//	函数功能：	WiFi模块RST初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void WiFi_SerialSend(uint8_t *p,uint16_t len)
{
    HAL_UART_Transmit(&UART2_Handler,p,len,1000);
}
//==========================================================
//	函数名称：	WiFi_RST_Init()
//
//	函数功能：	WiFi模块RST初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void WiFi_RST_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	WiFi_RST_GPIO_RCC_EN();           				//开启GPIOA时钟

	GPIO_Initure.Pin=WiFi_RST_PIN; 						//PA5
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
	HAL_GPIO_Init(WiFi_RST_PORT,&GPIO_Initure);

    WiFi_RST_Low(); //拉低复位信号，WIFI模块复位
    HAL_Delay(100);
	WiFi_RST_High();//拉高复位信号
}

//==========================================================
//	函数名称：	WiFi_LED_Init()
//
//	函数功能：	WiFi模块上LED灯初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void WiFi_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	WiFi_LED_GPIO_RCC_EN();           				//开启GPIOA时钟

	GPIO_Initure.Pin=WiFi_LED_PIN; 						//PA7
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
	HAL_GPIO_Init(WiFi_LED_PORT,&GPIO_Initure);

	HAL_GPIO_WritePin(WiFi_LED_PORT,WiFi_LED_PIN,GPIO_PIN_RESET);	//PA7置0 
}

//==========================================================
//	函数名称：	WiFi_KEY_Init()
//
//	函数功能：	按键初始化函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void WiFi_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    WiFi_KEY_GPIO_RCC_EN();           //开启GPIOA时钟
    
    GPIO_Initure.Pin=WiFi_KEY_PIN;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     	//低速
    HAL_GPIO_Init(WiFi_KEY_PORT,&GPIO_Initure);
}

//==========================================================
//	函数名称：	WiFi_RGB_Set_UP()
//
//	函数功能：	1码  850ns高电平  400ns低电平
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void WiFi_RGB_Set_UP()
{
	/*
			写高电平
	*/
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
	/*
			写低电平
	*/
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
}

//==========================================================
//	函数名称：	WiFi_RGB_Set_Down()
//
//	函数功能：	0码  400ns高电平  850ns低电平
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void WiFi_RGB_Set_Down()
{
	/*
			写高电平
	*/
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
		WiFi_LED_High();
	/*
			写低电平
	*/
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
		WiFi_LED_Low();
}

//==========================================================
//	函数名称：	WiFi_RGB_SET()
//
//	函数功能：	设置颜色
//
//	入口参数：	G8R8B8:颜色
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void WiFi_RGB_SET(uint32_t G8R8B8)
{
	static int i = 0;
	unsigned char byte = 0;
	
	for(i=23;i>=0;i--)
	{
		byte = ((G8R8B8 >> i) & 0x01);
		if(byte == 1)
		{
			WiFi_RGB_Set_UP();//1
		}
		else
		{
			WiFi_RGB_Set_Down(); //0
		}
	}
}

//==========================================================
//	函数名称：	WiFi_RGB_REST()
//
//	函数功能：	拉低电平等待下一次命令
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：			
//==========================================================
void WiFi_RGB_REST(void)
{
	WiFi_LED_Low();
	HAL_Delay(2);  //延时80us
}


