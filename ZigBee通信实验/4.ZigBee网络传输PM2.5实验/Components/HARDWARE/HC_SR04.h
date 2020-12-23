#ifndef __HC_SR04_H
#define __HC_SR04_H 			   

#include "ioCC2530.h"
#include <hal_types.h>
#include "delay.h"

#define		Trigl1			P1_6
#define		Trigl2			P1_5		
#define		Trigl3			P0_0		
#define		Trigl4			P1_1		

#define		Echo1			P1_7		
#define		Echo2			P1_4
#define		Echo3			P0_1
#define		Echo4			P1_0	

#define 	Trigl1_HIGH()		Trigl1 = 1;
#define 	Trigl1_LOW()		Trigl1 = 0;

#define 	Trigl2_HIGH()		Trigl2 = 1;
#define 	Trigl2_LOW()		Trigl2 = 0;

#define 	Trigl3_HIGH()		Trigl3 = 1;
#define 	Trigl3_LOW()		Trigl3 = 0;

#define 	Trigl4_HIGH()		Trigl4 = 1;
#define 	Trigl4_LOW()		Trigl4 = 0;

#define 	Echo1_STATE()    	Echo1           //读取返回电平
#define 	Echo2_STATE()    	Echo2	        //读取返回电平
#define 	Echo3_STATE()    	Echo3	        //读取返回电平
#define 	Echo4_STATE()    	Echo4           //读取返回电平

extern void     HCSR04_Init(void); 				//初始化超声波模块
extern uint16_t HCSR04_StartMeasure(uint8 number);	//超声波模块测距

#endif

