#ifndef __TEXT_H
#define __TEXT_H			  	 
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"    
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//OLED汉字显示驱动代码
//驱动方式:8080并口
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/03/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	   
////////////////////////////////////////////////////////////////////////////////// 	

void LCD_Show_Font(unsigned int x,unsigned int y,unsigned char index);
void LCD_one_DrawPoint(unsigned int x,unsigned int y,unsigned int COLOR);
#endif  
	 



