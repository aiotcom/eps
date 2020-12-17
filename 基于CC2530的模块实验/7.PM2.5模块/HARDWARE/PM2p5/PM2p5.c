#include <ioCC2530.h>        
#include "stdio.h"
#include "PM2p5.h"
#include "typedef.h"
void PM2p5_Init(void)
{
	APCFG |= BV(2);	
	P0SEL |= BV(2);        //片上外设
	P0DIR &= ~(BV(2));    //设置输入模式	
	
	P1SEL &= ~BV(0);
	P1DIR |= BV(0);
	PM2P5_LED_ENABLE();
}

uint16 PM2p5_ADC(void)
{
	short value=0;
	ADCCON3 = (0x80 | 0x30 | 0x02);
	ADCCON1 |= 0x30;     //选择ADC的启动模式为手动
	ADCCON1 |= 0x40;       //启动AD转化             
	
	while(!(ADCCON1 & 0x80));    //等待ADC转化结束
	
	value  =  ADCL;
	value |= (ADCH << 8);    //取得最终转化结果，存入value中  
	if(value<0){
		value = 0;
	}
	value  = value>>4;	
	return value;
}