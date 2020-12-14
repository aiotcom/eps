#ifndef __OLED_H
#define __OLED_H 			   

#include "stm32f1xx_hal.h" 
#include "stm32f1xx.h"

#define	Brightness	0xCF
#define X_WIDTH 	128
#define Y_WIDTH 	64

extern void OLED_CLS(void);
extern void OLED_Init(void);
extern void OLED_Main_UI(void);
extern void OLED_Fill(unsigned char bmp_dat) ;
extern void OLED_WrDat(unsigned char IIC_Data);
extern void OLED_WrCmd(unsigned char IIC_Command);
extern void OLED_Set_Pos(unsigned char x, unsigned char y) ;
extern void OLED_P24x24h(unsigned char x, unsigned char y, unsigned char N);
extern void OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N);
extern void OLED_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void OLED_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void OLED_DrawLineX(uint16_t x1, uint16_t y1, uint16_t y2,uint16_t Data);
extern void OLED_DrawLineY(uint16_t x1, uint16_t y1, uint16_t y2,uint16_t Data);
extern void OLED_P16x16Ch_Power(unsigned char x, unsigned char y, unsigned char N);
extern void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

extern unsigned char  F128x64[];
extern unsigned char  F24x24[];
extern unsigned char  F16x16[];
extern unsigned char  F16x16_Power[];
extern unsigned char  F8X16[];
extern unsigned char  F6x8[][6];

#endif


