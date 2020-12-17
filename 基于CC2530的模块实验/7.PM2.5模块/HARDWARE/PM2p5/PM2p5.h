#ifndef FLAME_H
#define FLAME_H
#include "typedef.h"

#define PM2P5_LED_ENABLE()  P1_0 = 1
#define PM2P5_LED_DISABLE() P1_0 = 0
void PM2p5_Init(void);
uint16 PM2p5_ADC(void);
#endif