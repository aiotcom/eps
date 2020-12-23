#ifndef __OLED_H
#define __OLED_H 			   


#include "delay.h"

#define	Brightness	0xCF
#define X_WIDTH 	128
#define Y_WIDTH 	64

extern void OLED_CLS(void);
extern void OLED_Init(void);
extern void OLED_Two_UI(void);
extern void WiFi_UP_Data(void);
extern void OLED_UI_Task(void);
extern void OLED_Powr_UI(void);
extern void OLED_Init_UI(void);
extern void OLED_Main_UI(void);
extern void OLED_Data_Task(void);
extern void OLED_Fill(unsigned char bmp_dat) ;
extern void OLED_WrDat(unsigned char IIC_Data);
extern void OLED_WrCmd(unsigned char IIC_Command);
extern void OLED_Set_Pos(unsigned char x, unsigned char y) ;
extern void OLED_DrawLineX(uint16 x1, uint16 y1, uint16 y2,uint16 Data);
extern void OLED_DrawLineY(uint16 x1, uint16 y1, uint16 y2,uint16 Data);
extern void OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N);
extern void OLED_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void OLED_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

extern const unsigned char  F16x16[];
extern const unsigned char  F8X16[];
extern const unsigned char  F6x8[][6];


#endif


