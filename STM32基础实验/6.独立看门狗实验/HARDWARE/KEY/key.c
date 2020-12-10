#include "led.h"
#include "key.h"
#include "delay.h"
#include "stm32f1xx.h"

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
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
    
    GPIO_Initure.Pin=S1_PIN|S2_PIN;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     	//低速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    GPIO_Initure.Pin=S3_PIN|S4_PIN;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     	//低速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//==========================================================
//	函数名称：	KEY_Scan()
//
//	函数功能：	按键处理函数
//
//	入口参数：	mode:	0,不支持连续按;
//										1,支持连续按;
//
//	返回参数：	返回按键值
//										0，没有任何按键按下
//										1、2、3对应相应的按键
//	说明：			注意此函数有响应优先级,KEY1>KEY2>KEY3!!
//==========================================================
uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up=1;     	//按键松开标志
    if(mode==1)key_up=1;    			//支持连按
    if(key_up&&(S1_Read()==0||S2_Read()==0||S3_Read()==0||S4_Read()==0))
    {
        delay_ms(10);
        key_up=0;
        if(S1_Read()==0)       return S1_PRES;
        else if(S2_Read()==0)  return S2_PRES;
        else if(S3_Read()==0)  return S3_PRES;  
				else if(S4_Read()==0)  return S4_PRES;
    }else if(S1_Read()==1&&S2_Read()==1&&S3_Read()==1&&S4_Read()==1)key_up=1;
		
    return 0;   //无按键按下
}

//==========================================================
//	函数名称：	key_Operatio()
//
//	函数功能：	按键操作-控制LED灯亮灭
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void key_Operatio(void)
{
	static uint8_t key = 0;
	
	key = KEY_Scan(0);//获取按键
	
	switch(key)
	{
		case 1:
			HAL_GPIO_TogglePin(LED_PORT,LED1_PIN);	//翻转LED1
			break;
		case 2:
			HAL_GPIO_TogglePin(LED_PORT,LED2_PIN);	//翻转LED2
			break;
		case 3:
			HAL_GPIO_TogglePin(LED_PORT,LED3_PIN);	//翻转LED3
			break;
		case 4:
			HAL_GPIO_TogglePin(LED_PORT,LED4_PIN);	//翻转LED4
			break;
		default:break;
	}
}




