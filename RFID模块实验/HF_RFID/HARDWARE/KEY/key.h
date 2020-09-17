#ifndef _KEY_H
#define _KEY_H

#include "stm32f1xx.h"

#define KEY_0   0
#define KEY_1   1
#define KEY_2   2
#define KEY_3   3
#define KEY_4   4
#define KEY_5   5
#define KEY_6   6
#define KEY_7   7
#define KEY_8   8
#define KEY_9   9
#define KEY_Enter  10
#define KEY_Canncle  11
uint8_t GetTheKey(uint8_t *p);
void KEY_Init(void); 
unsigned char KEY_Scan(void);

#endif
