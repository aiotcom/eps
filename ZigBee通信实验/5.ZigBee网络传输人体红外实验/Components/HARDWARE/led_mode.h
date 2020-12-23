#ifndef __LED_MODE_H
#define __LED_MODE_H 			   

#include <ioCC2530.h>
#include "hal_types.h"
#include "Hal_Defs.H"

#define KEY_PRESS   0x00
#define KEY_RELASE  0x01

void LedMode_Init(void);
void Led2_On(void);
void Led2_Off(void);
uint8 Key_ReadS4(void);

#endif