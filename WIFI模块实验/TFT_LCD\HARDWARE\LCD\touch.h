#ifndef __TOUCH_H
#define __TOUCH_H	
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
/* touch panel interface define */
#define  CMD_RDX  0xD0	 //触摸IC读坐标积存器
#define  CMD_RDY  0x90
//IO连接
//sbit DCLK	   =    P1^6;   
//sbit CS        =    P1^4;
//sbit DIN       =    P3^0;	
//sbit DOUT      =    P3^1;																						   
//sbit Penirq    =    P3^4;   //检测触摸屏响应信号

struct tp_pix_
{
	unsigned int x;
	unsigned int y;
};
struct tp_pixu32_
{
	int x;
	int y;
};
extern struct tp_pix_  tp_pixad,tp_pixlcd;	 //当前触控坐标的AD值,前触控坐标的像素值   
extern unsigned int vx,vy;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern unsigned int chx,chy;//默认像素点坐标为0时的AD起始值
unsigned char tpstate(void);
void spistar(void);  
void Touch_Adjust(void);
void point(void); //绘图函数
unsigned int ReadFromCharFrom7843();         //SPI 读数据
#endif  
	 
	 



