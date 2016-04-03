#include "common.h"	//包含系统配置
			/******************************************
			#include    "system_MKL.h"      //系统配置
			#include    "PORT_cfg.h"      	//管脚复用配置
			#include    "MKL_mcg.h"         //K60 时钟模块
			*******************************************/
#include "include.h"//包含用户定义的头文件
u8 adv1,adv2,adv3;
u8 var[8];
#define CMD_WARE     3	//虚拟示波器
u8 FinishOfRx=0;        //串口接收数据完成
u32 RxData;
u8 FinishOfRead=0;
/*卡尔曼滤波使用变量*/
//extern volatile u32 AdjustVar1=0;//调整值
//extern volatile u32 AdjustVar2=0;
///extern volatile u32 AdjustVar3=0;
u8 KalmanFitterAD1(u8 MeasVar);
u8 KalmanFitterAD2(u8 MeasVar);
u8 KalmanFitterAD3(u8 MeasVar);
/*********************************************/
//发送至虚拟示波器
void VIEW_send(uint8 *buff, uint32 size)//上位机	
{
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};      //串口调试 使用的前命令
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};      //串口调试 使用的后命令
    uart_putbuff(UART0,cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(UART0,buff, size);             //发送数据
    uart_putbuff(UART0,cmdr, sizeof(cmdr));    //发送后命令
}

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
        sum=0;max=0;min=255;
        for(i=0;i<=9;i++){
          sum+= AD1[i];
            if(max<AD1[i])
              max=AD1[i];
            if(min>AD1[i])
              min=AD1[i];
        }
        adv1=(sum-max-min)/8; 
        
        
        sum=0;max=0;min=255;
        for(i=0;i<=9;i++){
          sum+= AD2[i];
            if(max<AD2[i])
              max=AD2[i];
            if(min>AD2[i])
              min=AD2[i];
        }
        adv2=(sum-max-min)/8;
        
        
        sum=0;max=0;min=255;
        for(i=0;i<=9;i++){
          sum+= AD3[i];
            if(max<AD3[i])
              max=AD3[i];
            if(min>AD3[i])
              min=AD3[i];
        }
        adv3=(sum-max-min)/8;
}





void uart0_handler(void)
{
    char ch;

    if(uart_query(UART0) == 1)   //接收数据寄存器满
    {
        //用户需要处理接收数据
        uart_getchar   (UART0, &ch);                    //无限等待接受1个字节
        uart_putchar   (UART0 , ch);                    //发送字符串
        if(ch!=' '){
          FinishOfRx=0;         //接收没有完成
          if(FinishOfRead==1){     //读取完毕
            RxData=RxData*10;
            RxData+=ch-'0';
            }
        }else{
            FinishOfRx = 1; //接收完成
            printf("您输入的数据是%d\r\n",RxData);
        }
        
    }
}

void main()
{
        unsigned char A1,A2,A3;
	unsigned char K1,K2,K3;
        DisableInterrupts; //关闭中断使能
	KEY_INIT(); 
	LED_INIT();
	uart_init(UART0,115200);     //初始化串口(由于 printf 函数 所用的端口就是 UART0，已经初始化了，因此此处不需要再初始化)
	adc_init(ADC0_DP0);//ADC初始化
        set_vector_handler(UART0_VECTORn,uart0_handler);   // 设置中断服务函数到中断向量表里  uart0_handler 函数添加到中断向量表，不需要我们手动调用
        uart_rx_irq_en(UART0);
        //printf("请输入系统参数\r\n");
        
	while(1)
	{
            AD_AVR();
            A1=(u8)(126*adc_once(ADC0_SE8,  ADC_8bit)/160);
            A2=(u8)(126*adc_once(ADC0_SE9, ADC_8bit)/198);
            A3=(u8)(126*adc_once(ADC0_SE13, ADC_8bit)/192);
            K1=KalmanFitterAD1(A1);
            K2=KalmanFitterAD2(A2);
            K3=KalmanFitterAD3(A3);
            //printf("\r\n三个电感的平均数据:      %d %d %d",adv1,adv2,adv3);
           // printf("\r\n三个电感的立即值:        %d %d %d",A1,A2,A3);
            //printf("\r\n中间电感的卡尔曼数据:       %d ",KalmanFitterAD1(adv2));
          //启用虚拟示波器
            //printf("\n\nADC转换结果：%d\r\n",ADC_data);
           // printf("\r\n");
          DELAY_MS(10);
           var[0]=K1;                   
           var[1]=K2;
           var[2]=K3;  
           var[3]=A1;	
           var[4]=A2;
           var[5]=A3;          
           var[6]=adv1;
           var[7]=adv2;
           VIEW_send((uint8 *)var,sizeof(var)); //上位机

	   
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



