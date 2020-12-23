#include "ioCC2530.h"      
#include "stdio.h"
#include "PM2p5.h"
#include "sampleApp.h"
#define SAMPLE_SIZE 20
static uint16_t ADC_Sum,ADC_Value,i;
static uint16_t PM2p5_ugm3 = 0;
static int PM2P5_ADC[SAMPLE_SIZE],PM2P5_ADC_Count = 0;
float voltage = 0;
//==========================================================
//	函数名称：	void PM2p5_Init(void)
//
//	函数功能：	初始PM2.5模块
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void PM2p5_Init(void)
{
	APCFG |= BV(2);	
	P0SEL |= BV(2);        //片上外设
	P0DIR &= ~(BV(2));    //设置输入模式	
	
	P1SEL &= ~BV(0);
	P1DIR |= BV(0);
	PM2P5_LED_ENABLE();
}
//==========================================================
//	函数名称：	uint16 PM2p5_ADC(void)
//
//	函数功能：	PM2.5模块的ADC采集值
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
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
//==========================================================
//	函数名称：	void Delay_280us(void)
//
//	函数功能：	280us延时
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Delay_280us(void)
{
	volatile int count = 100;
	while(count--);
}
//==========================================================
//	函数名称：	void Dealy_10ms(void)
//
//	函数功能：	10ms延时
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Dealy_10ms(void)
{
	volatile int  count = 4300;
	while(count--);
}
//==========================================================
//	函数名称：	void bubbleSort(int arr[], int n)
//
//	函数功能：	实现由小到大的排
//
//	入口参数：	@arr，要进行排序的数组
//					    @n,排序的数组大小
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void bubbleSort(int arr[], int n)
{
    int i, j, tmp;

    for(i = 0; i < n - 1; i++){
        for(j = 1; j < n; j++){
            if(arr[j] < arr[j - 1]){
                tmp        = arr[j];
                arr[j]     = arr[j - 1];
                arr[j - 1] = tmp;
            }
        }
    }
}
uint16_t PM2p5_Read(void)
{
		/*进行采集*/
		for(PM2P5_ADC_Count = 0;PM2P5_ADC_Count<SAMPLE_SIZE;PM2P5_ADC_Count++){
			PM2P5_LED_ENABLE();
			Delay_280us();//延时280us
			PM2P5_ADC[PM2P5_ADC_Count++] = PM2p5_ADC();//ADC值保存到数据中
			PM2P5_LED_DISABLE();
			Dealy_10ms();//延时10ms
		}
		bubbleSort(&PM2P5_ADC[0], PM2P5_ADC_Count);//排序
		ADC_Sum = 0;        
		for(i=1;i<(SAMPLE_SIZE-1);i++){//去除采样数据中的最高值及高低值
			ADC_Sum += PM2P5_ADC[i];
		}
		ADC_Value  = ADC_Sum / (SAMPLE_SIZE-2);//减去最大和最小的两个	
		voltage    = (ADC_Value/2048.0)*3.3;//将ADC值转化成PM2.5值(ug/m3)
		PM2p5_ugm3 = voltage*0.13*1000;
		printf("PM2p5_ugm3=%d ug/m3\r\n",PM2p5_ugm3);   //调试打印
		return PM2p5_ugm3;
}