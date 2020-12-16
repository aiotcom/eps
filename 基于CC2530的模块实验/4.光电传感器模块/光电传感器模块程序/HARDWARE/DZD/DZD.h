#ifndef _DZD_H
#define _DZD_H
#include "stm32f1xx.h"
enum{
  DZD_BLACKOUT,
  DZD_GREEN=0x01,
  DZD_RED=0x02,
  DZD_BLUE=0x04
};
void DZD_Init(void);//初始化底座灯控制IO。
void DZD_ColSet(uint8_t val);//设置颜色。
#endif
