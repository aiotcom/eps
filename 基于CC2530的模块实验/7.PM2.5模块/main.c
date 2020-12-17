#include "OLED.h"
#include "PM2p5.h"
#include "string.h"
#include "delay.h"
#include "stdio.h"
#include "USART.h"
#define SAMPLE_SIZE 20
void bubbleSort(int arr[], int n);//排序算法
void Hal_Init_32M(void);//晶振时钟初始化，32M主频率，16M定时器频率。
void Delay_280us(void);
void Dealy_10ms(void);
int PM2P5_ADC[SAMPLE_SIZE],PM2P5_ADC_Count = 0;
uint8_t OLED_DispBuf[16];
float voltage = 0;

void main(void)
{
  uint16_t ADC_Sum,ADC_Value,i;
	uint16_t PM2p5_ugm3 = 0;
	uint8_t  tmpbuf[16];
	uint16_t PM2p5_Data,ADC_Data;
	Hal_Init_32M(); //初始化系统时钟32M
	USRT0_Init();   //初始化串口1，波物就给9600bps
	printf("this is uart0 print\r\n");
	OLED_Init();   //OLED 屏初始化
	PM2p5_Init();  //PM2.5模块初始化，初始ADC采集及传感器使能控制IO
	while(1){
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
		sprintf(OLED_DispBuf,"PM2.5=%d ug/m3 ",PM2p5_ugm3);//转成字符串
		OLED_P8x16Str(0,4,OLED_DispBuf);//OLED屏显示
		delay_ms(500);
	}
}
//==========================================================
//	函数名称：	Hal_Init_32M()
//
//	函数功能：	32M时钟初始化
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void Hal_Init_32M(void)
{
    CLKCONCMD &= ~0x40;         // 设置系统时钟源为 32MHZ晶振
    while(CLKCONSTA & 0x40);    // 等待晶振稳定
    CLKCONCMD &= ~0x47;         // 设置系统主时钟频率为 32MHZ
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