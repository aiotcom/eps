#ifndef __BEEP_H
#define __BEEP_H 			   
#include "ioCC2530.h"
#include "hal_types.h"
#include "stdio.h"
#include "sampleApp.h"
/*电平取反*/
#define BEEP_IO_TOGGLE()  P1_6 = ~P1_6;
#define BEEP_IO_LOW()  P1_6 = 0;
void Beep_Init(void);
#endif