#ifndef __BOTTOM_LED_H
#define __BOTTOM_LED_H 			   

#include <ioCC2530.h> 
#include "hal_types.h"
#define LED P2_0          //定义P2.0口为LED控制端

void bottom_led_init(void);
void RGB_SET(long int G8R8B8);
void RGB_REST(void);
void bottom_led(void);
void Bottom_Green(void);
void Bottom_Red(void);
void Bottom_Blue(void);
void Bottom_Yellow(void);
void Bottom_BlackOut(void);
#endif





























