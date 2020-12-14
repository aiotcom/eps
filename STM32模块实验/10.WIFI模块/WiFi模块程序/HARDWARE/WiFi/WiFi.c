#include "WiFi.h"
#include "delay.h"
#include "Usart.h"
#include "WS2812b.h"
uint8_t WiFi_AT_STA = 0;
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
    HAL_Delay(2);//延时2毫秒
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
    
    HAL_Delay(2000);//延时2000毫秒，因WIFI模块复位重启后会打印出许多开机信息，延时两秒避开。
    
    
    while(!WiFi_Send_Cmd(AT,(uint8_t*)"OK",20));				    /*进入AT命令模式*/
    WS2812B_SetColour(0x000055);                                    //红色提示
    
    while(!WiFi_Send_Cmd(CWMODE,(uint8_t*)"OK",30));				/*设置模块应用模式*/

    while(!WiFi_Send_Cmd(CWJAP,(uint8_t*)"OK",300));				/*配置要连接的路由器的SSID和密码*/   
    WS2812B_SetColour(0x005500);                                    //红色提示    
    
    
    while(!WiFi_Send_Cmd(TCP_CIPSTART,(uint8_t*)"OK",60));	/*与服务器建立TCP连接*/
    WS2812B_SetColour(0x550000);                                    //绿色提示
    
    while(!WiFi_Send_Cmd(CIPMODE,(uint8_t*)"OK",60));				/*透传模式*/

    while(!WiFi_Send_Cmd(CIPSEND,(uint8_t*)"OK",30));				/*开始传输*/

    while(!WiFi_Send_Cmd(CIPSTATUS,(uint8_t*)"",30));				/*连接设备*/

    WiFi_AT_STA = 1;           										/*AT指令发送完成标志位*/
}




