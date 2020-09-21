#ifndef _GUI_H
#define _GUI_H
#include "stm32f1xx.h"
void GUI_DrawSymbolString_H48xW24(uint16_t x0,uint16_t y0,uint8_t *pstring);
void GUI_DrawTempTitle(uint16_t x0,uint16_t y0);
void GUI_DrawHumiTitle(uint16_t x0,uint16_t y0);
void GUI_DrawAirPressTitle(uint16_t x0,uint16_t y0);
void GUI_DrawFanSwitchIco(uint16_t x0,uint16_t y0);
void GUI_DrawLinghtIntensityTitle(uint8_t x0,uint8_t y0);

void GUI_DrawAirPressData(uint16_t x0,uint16_t y0,float press);
void GUI_DrawHumiData(uint16_t x0,uint16_t y0,uint8_t humidity);
void GUI_DrawTempData(uint16_t x0,uint16_t y0,uint8_t temperature);    
void GUI_DrawLinghtIntensityData(uint16_t x0,uint16_t y0,uint16_t li);


void GUI_DrawFanTurnOn(uint16_t x0,uint16_t y0);
void GUI_DrawFanTurnOff(uint16_t x0,uint16_t y0);
#endif
