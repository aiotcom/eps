#include "stm32f1xx.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"
#include "Rs485.h"
#include "timer.h"
#include "TFT.h"

uint8_t PassWord_Flag = 0;							//密码正确标志位
u16 BACK_COLOR, POINT_COLOR;   					//背景色，画笔色	  

//==========================================================
//	函数名称：	TFT_Init
//
//	函数功能：	TFT显示器端口初始化
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：		
//==========================================================
void TFT_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟	
	
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_SWJ_NOJTAG();
		
		//TFT显示器模块
	  GPIO_Initure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5|GPIO_PIN_7;   //TFT_CS,TFT_RST,TFT_CLK,
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;	//低速
    GPIO_Initure.Mode =  GPIO_MODE_OUTPUT_PP;//推挽输出
		GPIO_Initure.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	  GPIO_Initure.Pin = GPIO_PIN_0;   //TFT_DC
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
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
     char i;
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
//	函数功能：	TFT寄存器初始化
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
	delay_ms(1);
	LCD_REST_H;
	delay_ms(1);
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
  BACK_COLOR=WHITE; 		//设置TFT液晶背景色
  POINT_COLOR=RED;  		//设置TFT液晶前景色
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
	num=(u16)num;
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
//	函数名称：	DataRep()
//
//	函数功能：	显示数据装换
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
uint8_t date1[20];
uint8_t SendBuf[40];
extern uint8_t SendData[ ];
extern uint8_t ucArray_ID[ ];
void DataRep(uint8_t data,uint8_t num,uint8_t flag,uint8_t flg)
{
  if(flg)
  {
    switch(data)
    {
      case 0x0A:
        SendData[num] = 65;
        break;
      case 0x0B:
        SendData[num] = 66;
        break;
      case 0x0C:
        SendData[num] = 67;
        break;
      case 0x0D:
        SendData[num] = 68;
        break;
      case 0x0E:
        SendData[num] = 69;
        break; 
      case 0x0F:
        SendData[num] = 70;
        break; 
      default:
        if(flag)
        {
          SendData[num] = (ucArray_ID[(num/2)]>>4)+0x30;
        }
        else
        {
          SendData[num] = (ucArray_ID[(num/2)]&0x0f)+0x30;
        }     
      break;
    } 
  }
  else
  {
    switch(data)
    {
      case 0x0A:
        SendBuf[num] = 65;
        break;
      case 0x0B:
        SendBuf[num] = 66;
        break;
      case 0x0C:
        SendBuf[num] = 67;
        break;
      case 0x0D:
        SendBuf[num] = 68;
        break;
      case 0x0E:
        SendBuf[num] = 69;
        break; 
      case 0x0F:
        SendBuf[num] = 70;
        break; 
      default:
        if(flag)
        {
          SendBuf[num] = (date1[(num/2)]>>4)+0x30;
        }
        else
        {
          SendBuf[num] = (date1[(num/2)]&0x0f)+0x30;
        }     
      break;
    }
  }
}

//==========================================================
//	函数名称：	LCD_UI
//
//	函数功能：	显示初始化界面
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：	  
//==========================================================
void LCD_UI(void)
{
	LCD_Clear(BLUE);
	BACK_COLOR = 0x0004;
	POINT_COLOR = WHITE;									//画笔颜色
	LCD_ShowString(120-3.5*8,30,"HF-RFID",1);
	LCD_Fill(11,63,228,148,BLACK);
    LCD_DrawRectangle(10,61,229,149);			//数据显示框
	LCD_ShowString(16,80,"ID:0x",1);
    BACK_COLOR = BLACK;
}

//==========================================================
//	函数名称：	TFT_Update
//
//	函数功能：	数据更新并显示
//
//	入口参数：	无
//              
//	返回参数：	无
//
//	说明：	  	 
//==========================================================
void TFT_Update(void)
{

}




