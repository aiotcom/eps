#include "DZD.h"

//==========================================================
//	函数名称：	DZD_Init()
//
//	函数功能：	初始化底座灯控制IO
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		底座灯(DZD)
//==========================================================
void DZD_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_AFIO_CLK_ENABLE();	
    __HAL_AFIO_REMAP_SWJ_NOJTAG(); 

    __HAL_RCC_GPIOB_CLK_ENABLE();           	                
    __HAL_RCC_GPIOA_CLK_ENABLE();           	              

    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;  	                
    GPIO_Initure.Pull  = GPIO_PULLUP;          	                
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    	 	       

    GPIO_Initure.Pin   = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15; 	
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    GPIO_Initure.Pin   = GPIO_PIN_15; 	                        	 	            
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);


    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15,GPIO_PIN_SET);	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15                      ,GPIO_PIN_SET);      
}
//==========================================================
//	函数名称：	DZD_ColSet()
//
//	函数功能：	设置底座灯颜色
//
//	入口参数：	val颜色值， 可以是 DZD_BLACKOUT,DZD_GREEN,DZD_RED,DZD_BLUE，或者以上四个参数中的任意合集。
//
//	返回参数：	无
//
//	说明：		底座灯颜色设置
//==========================================================
void DZD_ColSet(uint8_t val)
{
    /*全都熄灭*/
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);   
  
    if(val&DZD_BLUE){//设置蓝色
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);    
    }
    
    if(val&DZD_RED){//设置红色
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);    
    }

    if(val&DZD_GREEN){//设置绿色
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4 ,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3 ,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);    
    }    
}




