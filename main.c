#include "common.h"	//包含系统配置
			/******************************************
			#include    "system_MKL.h"      //系统配置
			#include    "PORT_cfg.h"      	//管脚复用配置
			#include    "MKL_mcg.h"         //K60 时钟模块
			*******************************************/
#include "include.h"//包含用户定义的头文件
u8 adv1,adv2,adv3;
void AD_AVR(void)
{
        u8 AD1[10],AD2[10],AD3[10];
        u8 i,max,min;
        u32 sum;
        for(i=0;i<=9;i++){
          AD1[i]=(u8)(126*adc_once(ADC0_SE8,  ADC_8bit)/160);
          AD2[i]=(u8)(126*adc_once(ADC0_SE9, ADC_8bit)/198);
          AD3[i]=(u8)(126*adc_once(ADC0_SE13, ADC_8bit)/192);
        }
        max=0;min=255;
        for(i=0;i<=9;i++){
          sum+= AD1[i];
            if(max<AD1[i])
              max=AD1[i];
            if(min>AD1[i])
              min=AD1[i];
        }
        adv1=(sum-max-min)/8; 
        max=0;min=255;
        for(i=0;i<=9;i++){
          sum+= AD2[i];
            if(max<AD2[i])
              max=AD2[i];
            if(min>AD2[i])
              min=AD2[i];
        }
        adv2=(sum-max-min)/8;
        max=0;min=255;
        for(i=0;i<=9;i++){
          sum+= AD3[i];
            if(max<AD3[i])
              max=AD3[i];
            if(min>AD3[i])
              min=AD3[i];
        }
        adv3=(sum-max-min)/8;
}
void main()
{
        
        DisableInterrupts; //关闭中断使能
	KEY_INIT(); 
	LED_INIT();
	uart_init(UART0,115200);     //初始化串口(由于 printf 函数 所用的端口就是 UART0，已经初始化了，因此此处不需要再初始化)
	DELAY_MS(100);//上电延时
	adc_init(ADC0_DP0);//ADC初始化
	while(1)
	{
            AD_AVR();
          //启用虚拟示波器
            printf("三个电感的数据:%d,%d,%d",adv1,adv2,adv3);
            printf("两电感之和:%d",adv1+adv3);
            printf("两电感只差:%d",abs(adv1-adv3));
            //printf("\n\nADC转换结果：%d\r\n",ADC_data);
            printf("\r\n");
           DELAY_MS(500);
            //dianya = ADC_data*3.3/65535;
            //printf("\r\n");
	   
	}
	
}




/******************************************************************************/










/***************************   ADC学习   ***************************************
extern void     adc_init    (ADCn_Ch_e);                //ADC初始化
extern uint16   adc_once    (ADCn_Ch_e, ADC_nbit);      //采集一次一路模拟量的AD值
	ADCn_Ch_e//ADC通道
	ADC_nbit//n = 8,12,16

extern void     adc_stop    (ADCn_e);                   //停止ADC转换

*******************************************************************************/

















/******************************延时函数*************************************/
/***************************************************************************
VCAN_MK60_conf.h
#if  0
#include "MKL_lptmr.h"//定时器延时
#define     DELAY()         lptmr_delay_ms(500)
#define     DELAY_MS(ms)    lptmr_delay_ms(ms)
#define     DELAY_US(us)    lptmr_delay_us(us)
#elif   0
#include "MKL_pit.h"
#define DELAY()         pit_delay_ms(PIT1,500)
#define DELAY_MS(ms)    pit_delay_ms(PIT1,ms)
#define DELAY_US(us)    pit_delay_us(PIT1,us)
#else
#include "MKL_SysTick.h"//系统时钟
#define DELAY()         systick_delay_ms(500)
#define DELAY_MS(ms)    systick_delay_ms(ms)
#define DELAY_US(us)    systick_delay_us(us)

也就是说要想更改延时就直接去 VCAN_MK60_conf.h，换一下选择函数即可
DELAY()       //500ms延时 
DELAY_MS(ms)  //ms级延时
DELAY_US(us)  //us级延时
****************************************************************************/



