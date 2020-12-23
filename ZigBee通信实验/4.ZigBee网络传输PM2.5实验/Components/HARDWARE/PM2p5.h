#ifndef FLAME_H
#define FLAME_H
#include "sampleApp.h"

#define PM2P5_LED_ENABLE()  P1_0 = 1
#define PM2P5_LED_DISABLE() P1_0 = 0
void PM2p5_Init(void);
uint16_t PM2p5_Read(void);
#endif