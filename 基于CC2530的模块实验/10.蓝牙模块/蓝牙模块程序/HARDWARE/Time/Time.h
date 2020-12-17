#ifndef __Time_H
#define __Time_H
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include <ioCC2530.h>
#include "delay.h"

extern	uint8_t Time_Flag;
extern	uint32_t Time_Count;
void Init_Timer1();

#endif