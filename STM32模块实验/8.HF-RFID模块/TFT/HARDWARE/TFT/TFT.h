#ifndef __TFT__H
#define __TFT__H
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "font.h"

#define u8 			uint8_t
#define u16 		uint16_t
#define u32 		uint32_t

#define LCD_REST_H 	GPIOA->BSRR = GPIO_PIN_1;//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
#define LCD_SDI_H  	GPIOA->BSRR = GPIO_PIN_7;//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
#define LCD_SCK_H  	GPIOA->BSRR = GPIO_PIN_5;//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
#define LCD_CS_H   	GPIOA->BSRR = GPIO_PIN_0;//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
#define LCD_DC_H   	GPIOB->BSRR = GPIO_PIN_0;//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);

#define LCD_REST_L 	GPIOA->BRR = GPIO_PIN_1;//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
#define LCD_SDI_L  	GPIOA->BRR = GPIO_PIN_7;//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
#define LCD_SCK_L  	GPIOA->BRR = GPIO_PIN_5;//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
#define LCD_CS_L   	GPIOA->BRR = GPIO_PIN_0;//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
#define LCD_DC_L   	GPIOB->BRR = GPIO_PIN_0;//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);

extern  u16 BACK_COLOR,POINT_COLOR;   	//背景色，画笔色

extern uint8_t PassWord_Flag;	
u16  LCD_ReadPoint(u16 x,u16 y); 				//读点

void LCD_UI(void);											//UI界面
void TFT_Init(void);
void Lcd_Init(void); 
void TFT_Update(void);
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);
void LCD_Clear(u16 Color);
void LCD_WR_DATA8(char da);
void Status_ON(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void Status_OFF(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void LCD_DrawPoint(u16 x,u16 y);		//画点
void LCD_DrawPoint_big(u16 x,u16 y);//画一个大点
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);					//显示数字

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);				//显示一个字符
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);				//显示2个数字
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u8 mode);	//显示一个字符串,16字体
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void showhanzi(unsigned int x,unsigned int y,unsigned char index);
void DataRep(uint8_t data,uint8_t num,uint8_t flag,uint8_t flg);
 //定义LCD的尺寸	
#define LCD_W 240
#define LCD_H 320

//画笔颜色
#define WHITE         	 	0xFFFF
#define BLACK         	 	0x0000	  
#define BLUE         	 		0x001F  
#define BRED             	0XF81F
#define GRED 			 				0XFFE0
#define GBLUE			 				0X07FF
#define RED           	 	0xF800
#define MAGENTA       	 	0xF81F
#define GREEN         	 	0x07E0
#define CYAN          	 	0x7FFF
#define YELLOW        	 	0xFFE0
#define BROWN 			 			0XBC40 	//棕色
#define BRRED 			 			0XFC07 	//棕红色
#define GRAY  			 			0X8430 	//灰色
//GUI颜色

#define DARKBLUE      	 	0X01CF	//深蓝色
#define LIGHTBLUE      	 	0X7D7C	//浅蓝色  
#define GRAYBLUE       	 	0X5458 	//灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     		0X841F 	//浅绿色
#define LGRAY 			 			0XC618 	//浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        	0XA651 	//浅灰蓝色(中间层颜色)
#define LBBLUE           	0X2B12 	//浅棕蓝色(选择条目的反色)

#endif



