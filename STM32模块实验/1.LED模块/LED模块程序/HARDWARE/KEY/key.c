#include "led.h"
#include "key.h"
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
//	说明：		初始化按键输入IO
//==========================================================
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    KEY_IO_RCC();//使用IO端口时钟
  
    GPIO_Initure.Pin   = S1_PIN|S2_PIN;        //S1_PIN,S2_PIN定义在key.h中 
    GPIO_Initure.Mode  = GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull  = GPIO_PULLUP;        	//上拉
    GPIO_Initure.Speed = GPIO_SPEED_LOW;     	//低速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);       
	
    GPIO_Initure.Pin   = S3_PIN|S4_PIN;        //S3_PIN、S4_PIN定义在key.h中 
    GPIO_Initure.Mode  = GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull  = GPIO_PULLUP;        	//上拉
    GPIO_Initure.Speed = GPIO_SPEED_LOW;     	//低速
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
//S1_Read() == 0,有按键按下，S1_Read() == 1,按键松开，以此类推
//==========================================================
uint8_t KEY_Scan(void)
{
    static uint8_t key_up=1;     	//按键松开标志
    
    /*在按键松开的情况，有按键按下*/
    if(key_up&&(S1_Read()==0||S2_Read()==0||S3_Read()==0||S4_Read()==0)){
        HAL_Delay(10);  //如果有按键按下延时10个毫秒消抖。
        key_up=0;       //按键己按下
        if(S1_Read()==0){
            return S1_PRES; //返回 S1 按下
        }
        else if(S2_Read()==0){
            return S2_PRES; //返回 S2 按下
        }
        else if(S3_Read()==0){
            return S3_PRES; //返回 S3 按下
        }          
				else if(S4_Read()==0){
            return S4_PRES; //返回 S4 按下
        }
    }
    else if(S1_Read()==1&&S2_Read()==1&&S3_Read()==1&&S4_Read()==1){
        /*按键全部松开了*/
        key_up=1;
		}    
    return 0;   //无按键按下
}




