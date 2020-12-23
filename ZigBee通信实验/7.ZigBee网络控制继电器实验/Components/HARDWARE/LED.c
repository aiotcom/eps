#include "delay.h"
#include "LED.h"
void KEY_Init(void);
//==========================================================
//	函数名称：	LED_Init()
//
//	函数功能：	初始化LED
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void LED_Init(void)
{
	 P0SEL &= ~0x0C; //GPIO功能		
   P0DIR |= 0x0C;      //P0.2、P0.3定义为输出
   P1DIR |= 0xC0;      //P1.6、P1.7定义为输出
   LED1=1;LED2=1;LED3=1;LED4=1;
	 KEY_Init();
}
//==========================================================
//	函数名称：	KEY_Init()
//
//	函数功能：	按键初始化函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void KEY_Init(void)
{
    P0SEL &= ~0x03;     //设置P0.0、P0.1为普通IO口  
    P0DIR &= ~0x03;     //按键接在P0.0、P0.1口上，设P0.0、P0.1为输入模式 
    P0INP &= ~0x03;     //打开P0.0、P0.1上拉电阻
    
    P1SEL &= ~0x30;     //设置P1.4、P1.5为普通IO口  
    P1DIR &= ~0x30;     //按键接在P1.4、P1.5口上，设P1.4、P1.5为输入模式 
    P1INP &= ~0x30;     //打开P1.4、P1.5上拉电阻
}
//==========================================================
//	函数名称：	unsigned char key_all_read(void)
//
//	函数功能：	按键初始化函数
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
unsigned char key_all_read(void)
{
	unsigned char kv = 0;
	kv  = S1_Read()<<0;
	kv |= S2_Read()<<1;
	kv |= S3_Read()<<2;
	kv |= S4_Read()<<3;	
	
	return (~kv)&0x0F;
}
//==========================================================
//	函数名称：	KEY_Scan()
//
//	函数功能：	按键处理函数
//
//	入口参数：	mode:	0,不支持连续按;
//				1,支持连续按;
//
//	返回参数：	返回按键值
//				0，没有任何按键按下
//				1、2、3、4对应相应的按键
//	说明：			注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4!!
//==========================================================
uint8_t KEY_Scan(void)
{
	static unsigned char state = KEY_RELASE,prekv = 0,kv = 0;
	kv = key_all_read();	
	if(KEY_RELASE == state)
	{
		if((kv)&&(prekv == kv))
		{
			printf("key pressed\r\n");
			state = KEY_PRESSED;
			return kv;
		}		
	}
	else if((KEY_PRESSED == state)&&(kv == 0))
	{
		printf("key relase\r\n");
		state = KEY_RELASE;
	}
	prekv = kv;
	return 0;
}