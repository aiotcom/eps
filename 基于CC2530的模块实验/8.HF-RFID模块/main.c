/**
************************************************************
************************************************************
************************************************************
*
*	作者： 		重庆八城科技
*
*	日期： 		2019-12-11
*
*	版本： 		V2.0
*
*	说明： 		HF-RFID实验
*
*	修改记录：	
************************************************************
************************************************************
************************************************************
**/
#include <ioCC2530.h>
#include "OLEDIIC.h"
#include "delay.h"
#include "RC522.h"
#include "Usart.h"	
#include "Rs485.h"
#include "Time.h"
#include "OLED.h"
#include "Lamp.h"

//==========================================================
//	函数名称：	main()
//
//	函数功能：	程序入口函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
uint8 Buffer_CarID[5];//保存卡片卡号
uint8 Buffer_Tmp[16];
void main(void)
{
		Hal_Init_32M();           //初始化32M时钟
		OLED_IIC_Init();          //初始化OLEDIIC
		OLED_Init();              //初始化OLED
		RC522_Init();             //初始化RFID芯片
		
		/*显示标题及默认值*/
		OLED_P8x16Str(0,0,"     HF_RFID   ");
		sprintf((void*)&Buffer_Tmp[0],"ID:XX XX XX XX",Buffer_CarID[0],Buffer_CarID[1],Buffer_CarID[2],Buffer_CarID[3]);
		OLED_P8x16Str(0,4,&Buffer_Tmp[0]);//OLED显示
		while(1)
		{
				if(IC_Card_Search(&Buffer_CarID[0])){      //读取卡信息
						/*将卡号转成字符器*/
						sprintf((void*)&Buffer_Tmp[0],"ID:%02X %02X %02X %02X",
										       Buffer_CarID[0],Buffer_CarID[1],Buffer_CarID[2],Buffer_CarID[3]);
						OLED_P8x16Str(0,4,&Buffer_Tmp[0]);//OLED显示
				}
		}
}
