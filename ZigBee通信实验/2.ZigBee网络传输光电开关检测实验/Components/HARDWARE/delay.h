#ifndef __DELAY_H
#define __DELAY_H 			   
#include <hal_types.h>

extern void Hal_Init_Mcu(void);
extern void delay_ms(uint32 nms);
extern void delay_us(uint32 nus);
extern void HAL_Set_MainClock(uint16 source);

#endif






























