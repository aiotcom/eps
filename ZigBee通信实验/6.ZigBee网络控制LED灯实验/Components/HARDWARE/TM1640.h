#ifndef __TM1640_H
#define __TM1640_H

#include "hal_types.h"

/*			TM1604			*/	 
#define SET_DATA_ORDER	0X40 //数据命令设置 普通模式

#define SET_DISPLAY 0X8c    //显示命令控制，脉冲宽度 11/16

#define	sda_H P1_5 = 1;P1_5 = 1;
#define	scl_H P1_4 = 1;P1_4 = 1;

#define	sda_L P1_5 = 0;P1_5 = 0;
#define	scl_L P1_4 = 0;P1_4 = 0;
	 
/*			TM1604			*/		
extern void TM1640_Init(void);
extern void start(void);
extern void stop(void);
extern void write(unsigned char date);
extern void init_tm1640(void);
extern void send_LED_Display(unsigned char address,uint16 data,unsigned char type);
extern void send_z(uint8 address,uint8 date);
extern void send(uint8 address, uint8 date);

#endif

