#include "stm32f1xx.h"
#include "stdlib.h"
#include "math.h"
#include "Rs485.h"
#include "delay.h"
#include "LCD.h"
#include "text.h"
#include "usart.h"
#include "gui.h"
#define LCD_DrawPoint1(a,b,c) POINT_COLOR = c;LCD_DrawPoint(a,b);
//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
u16 vx=15242,vy=11131;  																					//比例因子，此值除以1000之后表示多少个AD值代表一个像素点
u16 chx=3898,chy=145;																							//默认像素点坐标为0时的AD起始值
																																	//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数

struct tp_pix_  tp_pixad,tp_pixlcd;	 															//当前触控坐标的AD值,前触控坐标的像素值   

uint8_t touch_fig = 0;																						//接触完成标志位
uint8_t ffiigg = 0;																								//校准完成标志位
uint32_t px =0,py =0,px1 = 0,py1 = 0, px2= 0,py2 = 0;							//存放获取到的坐标信息		
unsigned char ERR_RANGE = 50;
u16 BACK_COLOR, POINT_COLOR;   																		//背景色，画笔色	  
u8 touch_point = 0;

Pen_Holder Pen_Point;																							//定义笔实体
//默认为touchtype=0的数据.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;
u8 FM_Mode = 1;  																									//当前设备所处的状态 1为收音  0为对讲
float Channel = 99.0;  																						//设备收音初始频段
float Channel_set[11] = {0,77.0,80.0,82.0,83.0,84.0,85.0,86.0,87.0,100.0,110.0}; //设定用于对讲功能时的几个频段
u8 channel_set = 1;
//==========================================================
//	函数名称：	LCD_Init
//
//	函数功能：	LCD显示器端口初始化
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_Init(void)
{
				GPIO_InitTypeDef GPIO_Initure;

		__HAL_RCC_GPIOA_CLK_ENABLE();           											//开启GPIOA时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();           											//开启GPIOB时钟	

		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_SWJ_NOJTAG();

		//LCD显示器模块
		GPIO_Initure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5|GPIO_PIN_7;//TFT_CS,TFT_RST,TFT_CLK,
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;											 //低速
		GPIO_Initure.Mode =  GPIO_MODE_OUTPUT_PP;											 //推挽输出
		GPIO_Initure.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);

		GPIO_Initure.Pin=GPIO_PIN_6;    															 //PA6
		GPIO_Initure.Mode=GPIO_MODE_INPUT;      											 //输入
		GPIO_Initure.Pull=GPIO_NOPULL;         												 //浮空
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;     											 //高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);

		GPIO_Initure.Pin = GPIO_PIN_0|GPIO_PIN_1;   									 //TFT_DC  T_CS
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);

//		GPIO_Initure.Pin=GPIO_PIN_14;    				//PB14
//		GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
//		GPIO_Initure.Pull=GPIO_PULLDOWN;        //上拉
//		GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
//		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//==========================================================
//	函数名称：	Touch_Init
//
//	函数功能：	触摸屏初始化
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void Touch_Init(void)
{
		u16 x1,y1;
	
		GPIO_InitTypeDef GPIO_Initure;

		__HAL_RCC_GPIOB_CLK_ENABLE();           								//开启GPIOB时钟	
	
		Read_ADS(&x1,&y1);																			//第一次读取初始化	

		GPIO_Initure.Pin=GPIO_PIN_14;    												//PB14

		GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     						//下降沿触发
		GPIO_Initure.Pull=GPIO_PULLUP;        									//上拉

		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
		HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,4);  		 					//抢占优先级为3，子优先级为3
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         				//使能中断线14 
}


void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);  								//调用中断处理公用函数
}

//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

    delay_ms(50);      																			//消抖
	if(touch_point  == 0)
		touch_point = 1;
//	HAL_UART_Transmit(&UART2_Handler,EXIT_state,30,1000);
	EXTI->PR=1<<14;
}
//PEN中断设置	 
void Pen_Int_Set(u8 en)
{
	if(en)EXTI->IMR|=1<<14;   																//开启line14上的中断	  	
	else EXTI->IMR&=~(1<<14); 																//关闭line14上的中断	   
}	
//==========================================================
//	函数名称：	LCD_Writ_Bus
//
//	函数功能：	串行数据写入
//
//	入口参数：	da:需要写入的数据
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void  LCD_Writ_Bus(char da)
{
	 static char i;
	 for(i=0;i<8;i++)
	 {
		 if(da &0x80)
		 {
			 LCD_SDI_H;
		 }
		 else LCD_SDI_L;
		 LCD_SCK_L;
		 LCD_SCK_H;
		 da <<=1;
	 }	
}

//==========================================================
//	函数名称：	LCD_WR_DATA8
//
//	函数功能：	发送数据-8位参数
//
//	入口参数：	da:需要写入的数据
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_WR_DATA8(char da) //发送数据-8位参数
{
    LCD_DC_H;
    LCD_Writ_Bus(da);
}  

//==========================================================
//	函数名称：	LCD_WR_DATA
//
//	函数功能：	发送数据参数
//
//	入口参数：	da:需要写入的数据
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_WR_DATA(int da)
{
    LCD_DC_H;
    LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
}	  

//==========================================================
//	函数名称：	LCD_WR_REG
//
//	函数功能：	REG参数写入
//
//	入口参数：	da:需要写入的数据
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_WR_REG(char da)	 
{
    LCD_DC_L;
    LCD_Writ_Bus(da);
}
//==========================================================
//	函数名称：	LCD_WR_REG_DATA
//
//	函数功能：	REG参数写入
//
//	入口参数：	da:需要写入的数据
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
 void LCD_WR_REG_DATA(int reg,int da)
{
  LCD_WR_REG(reg);
	LCD_WR_DATA(da);
}

//==========================================================
//	函数名称：	Address_set
//
//	函数功能：	REG参数写入
//
//	入口参数：	x1,y1,x2,y2为坐标点
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);

}

//==========================================================
//	函数名称：	Lcd_Init
//
//	函数功能：	LCD寄存器初始化
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void Lcd_Init(void)
{

//调用一次这些函数，免得编译的时候提示警告
//   	LCD_CS_H;
// 	if(LCD_CS==0)
// 	{
// 	   LCD_WR_REG_DATA(0,0);
// 	   LCD_ShowString(0,0," ");
// 	   LCD_ShowNum(0,0,0,0);
// 	   LCD_Show2Num(0,0,0,0);
// 	   LCD_DrawPoint_big(0,0);
// 	   LCD_DrawRectangle(0,0,0,0);
// 	   Draw_Circle(0,0,0);
//  	 }    
	LCD_REST_L;
	delay_ms(10);
	LCD_REST_H;
	delay_ms(10);
	LCD_CS_L;  //打开片选使能

	LCD_WR_REG(0xCB);  
	LCD_WR_DATA8(0x39); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x34); 
	LCD_WR_DATA8(0x02); 

	LCD_WR_REG(0xCF);  
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0XC1); 
	LCD_WR_DATA8(0X30); 
 	
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA8(0x85); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x78); 

	LCD_WR_REG(0xEA);  
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x00); 
 	
	LCD_WR_REG(0xED);  
	LCD_WR_DATA8(0x64); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0X12); 
	LCD_WR_DATA8(0X81); 

	LCD_WR_REG(0xF7);  
	LCD_WR_DATA8(0x20); 
 
	LCD_WR_REG(0xC0);    	//Power control 
	LCD_WR_DATA8(0x23);   //VRH[5:0] 
 
	LCD_WR_REG(0xC1);    	//Power control 
	LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 

	LCD_WR_REG(0xC5);    	//VCM control 
	LCD_WR_DATA8(0x3e); 	//对比度调节
	LCD_WR_DATA8(0x28); 

	LCD_WR_REG(0xC7);    	//VCM control2 
	LCD_WR_DATA8(0x86); 	//--

	LCD_WR_REG(0x36);    	// Memory Access Control 
	LCD_WR_DATA8(0x48); 	//	   //48 68竖屏//28 E8 横屏

	LCD_WR_REG(0x3A);    
	LCD_WR_DATA8(0x55); 

	LCD_WR_REG(0xB1);    
	LCD_WR_DATA8(0x00);  
	LCD_WR_DATA8(0x18); 

	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA8(0x08); 
	LCD_WR_DATA8(0x82);
	LCD_WR_DATA8(0x27);  
 	
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA8(0x00); 
 	
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA8(0x01); 
 
 	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA8(0x0F); 
	LCD_WR_DATA8(0x31); 
	LCD_WR_DATA8(0x2B); 
	LCD_WR_DATA8(0x0C); 
	LCD_WR_DATA8(0x0E); 
	LCD_WR_DATA8(0x08); 
	LCD_WR_DATA8(0x4E); 
	LCD_WR_DATA8(0xF1); 
	LCD_WR_DATA8(0x37); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x10); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0x0E); 
	LCD_WR_DATA8(0x09); 
	LCD_WR_DATA8(0x00); 

	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x0E); 
	LCD_WR_DATA8(0x14); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0x11); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x31); 
	LCD_WR_DATA8(0xC1); 
	LCD_WR_DATA8(0x48); 
	LCD_WR_DATA8(0x08); 
	LCD_WR_DATA8(0x0F); 
	LCD_WR_DATA8(0x0C); 
	LCD_WR_DATA8(0x31); 
	LCD_WR_DATA8(0x36); 
	LCD_WR_DATA8(0x0F); 
 
	LCD_WR_REG(0x2A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xEF);

	LCD_WR_REG(0x2B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x3F);
	LCD_WR_REG(0x11);    	//Exit Sleep 
	delay_ms(60); 
			
  LCD_WR_REG(0x29);    	//Display on 
  LCD_WR_REG(0x2c); 
  BACK_COLOR=WHITE; 		//设置LCD液晶背景色
  POINT_COLOR=RED;  		//设置LCD液晶前景色
}

//==========================================================
//	函数名称：	LCD_Clear
//
//	函数功能：	清屏函数
//
//	入口参数：	Color:要清屏的填充色
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_Clear(u16 Color)
{
	u8 VH,VL;
	u16 i,j;
	VH=Color>>8;
	VL=Color;	
	Address_set(0,0,LCD_W-1,LCD_H-1);
	for(i=0;i<LCD_W;i++)
	 {
		for (j=0;j<LCD_H;j++)
			{
					LCD_WR_DATA8(VH);
					LCD_WR_DATA8(VL);	
			}
		}
}

//==========================================================
//	函数名称：	LCD_DrawPoint
//
//	函数功能：	画点
//
//	入口参数：	位置光标
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_DrawPoint(u16 x,u16 y)
{

	Address_set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 

//==========================================================
//	函数名称：	LCD_DrawPoint_big
//
//	函数功能：	画一个大点
//
//	入口参数：	位置光标
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
} 

//  (xend-xsta)*(yend-ysta)
//==========================================================
//	函数名称：	LCD_Fill
//
//	函数功能：	在指定区域内填充指定颜色
//
//	入口参数：	区域大小: (xend-xsta)*(yend-ysta)
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_set(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//设置光标位置 	    
	} 					  	    
}  

//==========================================================
//	函数名称：	LCD_DrawLine
//
//	函数功能：	画线
//
//	入口参数：	x1,y1:起点坐标
//              x2,y2:终点坐标  
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

//==========================================================
//	函数名称：	LCD_DrawRectangle
//
//	函数功能：	画矩形
//
//	入口参数：	区域大小: (x2-x1)*(y2-y1)
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
	LCD_DrawPoint1(xc + x, yc + y, c);
	LCD_DrawPoint1(xc - x, yc + y, c);
	LCD_DrawPoint1(xc + x, yc - y, c);
	LCD_DrawPoint1(xc - x, yc - y, c);
	LCD_DrawPoint1(xc + y, yc + x, c);
	LCD_DrawPoint1(xc - y, yc + x, c);
	LCD_DrawPoint1(xc + y, yc - x, c);
	LCD_DrawPoint1(xc - y, yc - x, c);
}
void LCD_circle(int xc, int yc,u16 c,int r, int fill)
{
	int x = 0, y = r, yi, d;
    uint16_t temp;
    temp = POINT_COLOR;
	d = 3 - 2 * r;


	if (fill) 
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else 
	{
		// 如果不填充（画空心圆）
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
    POINT_COLOR = temp;
}
//==========================================================
//	函数名称：	Draw_Circle
//
//	函数功能：	在指定位置画一个指定大小的圆
//
//	入口参数：	(x,y):中心点
//              r    :半径
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 

//==========================================================
//	函数名称：	LCD_ShowChar
//
//	函数功能：	在指定位置显示一个字符
//
//	入口参数：	(x,y):显示坐标
//              num:要显示的字符:" "--->"~"
//              mode:叠加方式(1)还是非叠加方式(0)
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	Address_set(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 

//==========================================================
//	函数名称：	LCD_ShowNum
//
//	函数功能：	显示多个数字
//
//	入口参数：	x,y :起点坐标	 
//              len :数字的位数
//              num:数值(0~4294967295);
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u32)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 

//==========================================================
//	函数名称：	LCD_Show2Num
//
//	函数功能：	显示2个数字
//
//	入口参数：	x,y :起点坐标	 
//              len :数字的位数
//              num:数值(0~99);
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 

//==========================================================
//	函数名称：	LCD_ShowString
//
//	函数功能：	显示字符串
//
//	入口参数：	x,y:起点坐标  
//              *p:字符串起始地址
//							mode:叠加方式(1)还是非叠加方式(0)
//              
//	返回参数：	无
//
//	说明：	    用16字体	
//==========================================================
void LCD_ShowString(u16 x,u16 y,const u8 *p,u8 mode)
{
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;}
        LCD_ShowChar(x,y,*p,mode);
        x+=8;
        p++;
    } 
}


//==========================================================
//	函数名称：	Drow_Touch_Point
//
//	函数功能：	与LCD部分有关的函数  
//
//	入口参数：	x,y:起点坐标  
//              
//	返回参数：	无
//
//	说明：	   	用来校准用的
//==========================================================
void Drow_Touch_Point(u16 x,u16 y)
{
	LCD_DrawLine(x-12,y,x+13,y);//横线
	LCD_DrawLine(x,y-12,x,y+13);//竖线
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
//	Draw_Circle(x,y,6);//画中心圈
}	
u16 ReadFromCharFrom7843()             //SPI 读数据
{
	u8 count=0;
	u16 Num=0;
	for(count=0;count<12;count++)
	{
		Num<<=1;		
		LCD_SCK_H;               //下降沿有效
		LCD_SCK_L; 
		if(T_OUT)
		{
			Num|=1;
		}
	}

return(Num);
}
//从7846/7843/XPT2046/UH7843/UH7846读取adc值	  0x90=y   0xd0-x
u16 ADS_Read_AD(unsigned char CMD)          
{
    u16 l;
    T_CS_L;     
    LCD_Writ_Bus(CMD);//送控制字即用差分方式读X坐标 详细请见有关资料
    LCD_SCK_H;
    LCD_SCK_L;
    l=ReadFromCharFrom7843();
    T_CS_H;
    return l;
}	

//==========================================================
//	函数名称：	ADS_Read_XY
//
//	函数功能：	读取一个坐标值
//
//	入口参数：	 
//              
//	返回参数：	无
//
//	说明：	   	连续读取READ_TIMES次数据,对这些数据升序排列,
//							然后去掉最低和最高LOST_VAL个数,取平均值 
//==========================================================
#define READ_TIMES 15 //读取次数
#define LOST_VAL 5	  //丢弃值
u16 ADS_Read_XY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
    {
        sum+=buf[i];
    }
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

//==========================================================
//	函数名称：	Read_ADS
//
//	函数功能：	带滤波的坐标读取
//
//	入口参数：	 
//              
//	返回参数：	无
//
//	说明：	   	最小值不能少于100.
//==========================================================
u8 Read_ADS(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	
	*x=xtemp;
	*y=ytemp;    
	if(xtemp<100||ytemp<100)
    {
        return 0;//读数失败
    }
	return 1;//读数成功
}

//==========================================================
//	函数名称：	Read_ADS2
//
//	函数功能：	2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过
//
//	入口参数：	 
//              
//	返回参数：	无
//
//	说明：	   	该函数能大大提高准确度
//==========================================================
#define ERR_RANGE 20 //误差范围 
u8 Read_ADS2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    uint32_t tick;
    flag=Read_ADS(&x1,&y1);     
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    printf("x1:%d,y1:%d,x2:%d,y2:%d\r\n",x1,y1,x2,y2);
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE)) //前后两次采样在+-ERR_RANGE内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)>>1;
        *y=(y1+y2)>>1;		
        return 1;
    }
    else 
    {
        return 0;
    }        
} 

//==========================================================
//	函数名称：	Read_TP_Once
//
//	函数功能：	精确读取一次坐标,校准的时候用的	 
//
//	入口参数：	 
//              
//	返回参数：	无
//
//	说明：	   	
//==========================================================
u8 Read_TP_Once(void)
{
	u8 re=0;
	u16 x1,y1;
	Pen_Int_Set(0);//关闭中断
	while(re==0)
	{
		while(!Read_ADS2(&tp_pixad.x,&tp_pixad.y));
		delay_ms(10);
		while(!Read_ADS2(&x1,&y1));
		if((tp_pixad.x == x1)&&(tp_pixad.y == y1))
		{
			re=1; 
		}
	} 
	Pen_Int_Set(1);//开启中断	   
	return re;
}

//==========================================================
//	函数名称：	LCD_UI
//
//	函数功能：	LCD屏FM界面
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：			
//==========================================================
void LCD_UI(void)
{

}
//==========================================================
//	函数名称：	FM_Mode_display
//
//	函数功能：	与LCD部分有关的函数  
//
//	入口参数：	data:模式：对讲或收音模式
//              
//	返回参数：	无
//
//	说明：	   	画选框，红色
//==========================================================
void FM_Mode_display(u8 data) //模式选框
{	
}
//==========================================================
//	函数名称：	FM_Mode_display
//
//	函数功能：	与LCD部分有关的函数  
//
//	入口参数：	data,mode,channel 收音模式下的频率，模式，对讲模式下的频率
//              
//	返回参数：	无
//
//	说明：	   	显示频率并通过485发送数据
//==========================================================
void Channel_display(float data,u8 mode,u8 channel) 
{
    
}

//==========================================================
//	函数名称：	FanSwitch
//
//	函数功能：	处理触摸屏操作
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：	
//==========================================================
uint8_t FanSwitch(void)
{
    static uint8_t state;
    static uint16_t lx,ly;
    uint8_t ret = 0;
    if(state == 0)//按键按下了
    {
        if(Read_ADS2(&lx,&ly))	//得到单次按键值
        {
            if(lx>1563 && lx<2486 && ly>2525 && ly<3215)				//按键"对讲"
            {
                ret = 1;
                state = 1;
            }
        }
        
    }
    else
    {//等待触摸松开
        if(!Read_ADS2(&lx,&ly))
        {
            printf("lx=%d,ly=%d\r\n",lx,ly);
            state = 0;
        }
    }
    return ret;
}

//触摸屏校准代码
//得到四个校准参数 
//提示字符串


void ADJ_INFO_SHOW(u8*str)
{
	LCD_ShowString(40,40,"x1:       y1:       ",1);
	LCD_ShowString(40,60,"x2:       y2:       ",1);
	LCD_ShowString(40,80,"x3:       y3:       ",1);
	LCD_ShowString(40,100,"x4:       y4:       ",1);
 	LCD_ShowString(40,120,str,0);
}
//触摸屏校准代码
//得到四个校准参数
void Touch_Adjust(void)
{
	signed short pos_temp[4][2];//坐标缓存值
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	float fac; 	   
	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//清屏   
	POINT_COLOR=RED;//红色 
	LCD_Clear(WHITE);//清屏 
	Drow_Touch_Point(20,20);//画点1 
	
//	Pen_Point.Key_Sta=Key_Up;//消除触发信号 
	Pen_Point.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误	 
	while(1)
	{
		if(touch_point == 1)//按键按下了
		{
			touch_point  = 0;
			if(Read_TP_Once())//得到单次按键值
			{  								   
				pos_temp[cnt][0]=tp_pixad.x;
				pos_temp[cnt][1]=tp_pixad.y;
				cnt++;
			}			 
			switch(cnt)
			{			   
				case 1:
					LCD_Clear(WHITE);//清屏 
					Drow_Touch_Point(220,20);//画点2
					break;
				case 2:
					LCD_Clear(WHITE);//清屏 
					Drow_Touch_Point(20,300);//画点3
					break;
				case 3:
					LCD_Clear(WHITE);//清屏 
					Drow_Touch_Point(220,300);//画点4
					break;
				case 4:	 //全部四个点已经得到
	    		    //对边相等
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,2的距离
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到3,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//不合格
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("ver fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4);		//显示数值
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4);	//显示数值
						LCD_ShowNum(40+24,60,pos_temp[1][0],4);		//显示数值
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4);	//显示数值
						LCD_ShowNum(40+24,80,pos_temp[2][0],4);		//显示数值
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4);	//显示数值
						LCD_ShowNum(40+24,100,pos_temp[3][0],4);		//显示数值
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4);	//显示数值
						//扩大100倍显示
						LCD_ShowNum(40,140,fac*100,3);//显示数值,该数值必须在95~105范围之内.
						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,3的距离
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("hor fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4);		//显示数值
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4);	//显示数值
						LCD_ShowNum(40+24,60,pos_temp[1][0],4);		//显示数值
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4);	//显示数值
						LCD_ShowNum(40+24,80,pos_temp[2][0],4);		//显示数值
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4);	//显示数值
						LCD_ShowNum(40+24,100,pos_temp[3][0],4);		//显示数值
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4);	//显示数值
						//扩大100倍显示
						LCD_ShowNum(40,140,fac*100,3);//显示数值,该数值必须在95~105范围之内.
 						continue;
					}//正确了
								   
					//对角线相等
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,4的距离
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,3的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("dia fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4);		//显示数值
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4);	//显示数值
						LCD_ShowNum(40+24,60,pos_temp[1][0],4);		//显示数值
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4);	//显示数值
						LCD_ShowNum(40+24,80,pos_temp[2][0],4);		//显示数值
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4);	//显示数值
						LCD_ShowNum(40+24,100,pos_temp[3][0],4);		//显示数值
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4);	//显示数值
						//扩大100倍显示
						LCD_ShowNum(40,140,fac*100,3);//显示数值,该数值必须在95~105范围之内.
						continue;
					}//正确了
					//计算结果
					Pen_Point.xfac=(float)200/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
						  
					Pen_Point.yfac=(float)280/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff  

					if(abs(Pen_Point.xfac)>2||abs(Pen_Point.yfac)>2)//触屏和预设的相反了.
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20);
						LCD_ShowString(35,110,"TP Need readjust!",1);
						Pen_Point.touchtype=!Pen_Point.touchtype;//修改触屏类型.
						if(Pen_Point.touchtype)//X,Y方向与屏幕相反
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y方向与屏幕相同
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}
						delay_ms(500);
						continue;
					}
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);//清屏
					LCD_ShowString(35,110,"Touch Screen Adjust OK!",1);//校正完成
					delay_ms(500);
					LCD_Clear(WHITE);//清屏 
                    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); 
                    Pen_Int_Set(0);                    
					return;//校正完成				 
			}
		}
	}

}		

