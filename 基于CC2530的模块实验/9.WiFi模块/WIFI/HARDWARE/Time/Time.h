#ifndef __Time_H
#define __Time_H 			   

#include <ioCC2530.h>
#include "delay.h"

extern uint32_t Task_Count,Send_OneNET_Count;//分时任务计数值
extern void Init_Timer1(void); //LED初始化
extern long Time1_GetTicks(void);
#endif

