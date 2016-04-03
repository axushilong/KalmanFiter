#include "common.h"	//包含系统配置
			/******************************************
			#include    "system_MKL.h"      //系统配置
			#include    "PORT_cfg.h"      	//管脚复用配置
			#include    "MKL_mcg.h"         //K60 时钟模块
			*******************************************/
#include "include.h"//包含用户定义的头文件
#define SysNoiseCoVar         (0.0123)
#define MeasNoiseCoVar        (16.7415926-1)           //系统噪声协方差，测量噪声协方差
//SysNoiseCoVar越大，精度越低，速度越快MeasNoiseCovar越大，精度越高，速度越慢
volatile u32 AdjustVar1=0;//调整值
volatile u32 AdjustVar2=0;
volatile u32 AdjustVar3=0;
/*卡尔曼滤波需要设置4个值，其余均可自动调整*/
u8 KalmanFitterAD1(u8 MeasVar)
{
    static double PreOptimalVar=1;                       //先前最优值
    static double CurMeasVar=0,CurEstimateVar=0;        //当前测量值，当前预测值
    static double CurOptimalVar=0;                      //当前最优值
    static double CurSysCoVar=0,PreSysCoVar=30;         //当前系统协方差，先前系统协方差
    static double KalmanGain=0;                         //卡尔曼增益
    CurMeasVar=MeasVar;                         //当前系统测量值
    //先前系统最优值和先前系统协方差需要设置非零初始值；；；；
    
    
    
    /*当前估计值=先前最优值+调整值*/
    CurEstimateVar=PreOptimalVar+AdjustVar1;
    AdjustVar1=0;                                //设定调整值的原因是加快调整速度
    
    
    
    /*当前系统协方差=先前系统协方差+系统噪声协方差*/
    CurSysCoVar=PreSysCoVar+SysNoiseCoVar;
    
    
    
    /*卡尔曼增益=当前系统协方差/(当前系统协方差+测量噪声协方差)*/
    KalmanGain=CurSysCoVar/(CurSysCoVar+MeasNoiseCoVar);
    
    
    
    
    /*当前系统最优值=当前系统估计值+卡尔曼增益*（测量值-当前系统估计值）*/
    CurOptimalVar=CurEstimateVar+KalmanGain*(CurMeasVar-CurEstimateVar);
    
    
    
    /*先前系统噪声协方差=（1-卡尔曼增益）x当前系统协方差*/
    PreSysCoVar =(1-KalmanGain)*CurSysCoVar;
    
    
    
    PreOptimalVar=CurOptimalVar;		//递归处理当前系统最优值
    
    /*返回数据*/
    return (u8)CurOptimalVar;
    
}
u8 KalmanFitterAD2(u8 MeasVar)
{
    static double PreOptimalVar=1;                       //先前最优值
    static double CurMeasVar=0,CurEstimateVar=0;        //当前测量值，当前预测值
    static double CurOptimalVar=0;                      //当前最优值

    static double CurSysCoVar=0,PreSysCoVar=30;         //当前系统协方差，先前系统协方差
    static double KalmanGain=0;                         //卡尔曼增益
    CurMeasVar=MeasVar;                         //当前系统测量值
    //先前系统最优值和先前系统协方差需要设置非零初始值；；；；
    
    
    
    /*当前估计值=先前最优值+调整值*/
    CurEstimateVar=PreOptimalVar+AdjustVar2;
    AdjustVar2=0;                                //设定调整值的原因是加快调整
    
    
    
    /*当前系统协方差=先前系统协方差+系统噪声协方差*/
    CurSysCoVar=PreSysCoVar+SysNoiseCoVar;
    
    
    
    /*卡尔曼增益=当前系统协方差/(当前系统协方差+测量噪声协方差)*/
    KalmanGain=CurSysCoVar/(CurSysCoVar+MeasNoiseCoVar);
    
    
    
    
    /*当前系统最优值=当前系统估计值+卡尔曼增益*（测量值-当前系统估计值）*/
    CurOptimalVar=CurEstimateVar+KalmanGain*(CurMeasVar-CurEstimateVar);
    
    
    
    /*先前系统噪声协方差=（1-卡尔曼增益）x当前系统协方差*/
    PreSysCoVar =(1-KalmanGain)*CurSysCoVar;
    
    /*处理递归的任务*/
    PreOptimalVar=CurOptimalVar;
    
    /*返回数据*/
    return (u8)CurOptimalVar;
}
u8 KalmanFitterAD3(u8 MeasVar)
{
    static double PreOptimalVar=1;                       //先前最优值
    static double CurMeasVar=0,CurEstimateVar=0;        //当前测量值，当前预测值
    static double CurOptimalVar=0;                      //当前最优值
    static double CurSysCoVar=0,PreSysCoVar=30;         //当前系统协方差，先前系统协方差
    static double KalmanGain=0;                         //卡尔曼增益
    CurMeasVar=MeasVar;                         //当前系统测量值
    //先前系统最优值和先前系统协方差需要设置非零初始值；；；；
    
    
    
    /*当前估计值=先前最优值+调整值*/
    CurEstimateVar=PreOptimalVar+AdjustVar3;
    AdjustVar3=0;                                //设定调整值的原因是加快调整
    
    
    
    /*当前系统协方差=先前系统协方差+系统噪声协方差*/
    CurSysCoVar=PreSysCoVar+SysNoiseCoVar;
    
    
    
    /*卡尔曼增益=当前系统协方差/(当前系统协方差+测量噪声协方差)*/
    KalmanGain=CurSysCoVar/(CurSysCoVar+MeasNoiseCoVar);
    
    
    
    
    /*当前系统最优值=当前系统估计值+卡尔曼增益*（测量值-当前系统估计值）*/
    CurOptimalVar=CurEstimateVar+KalmanGain*(CurMeasVar-CurEstimateVar);
    
    
    
    /*先前系统噪声协方差=（1-卡尔曼增益）x当前系统协方差*/
    PreSysCoVar =(1-KalmanGain)*CurSysCoVar;
    
    PreOptimalVar=CurOptimalVar;
    
    /*返回数据*/
    return (u8)CurOptimalVar;
}

//根据三个电感数据计算偏离距离

u8 Distance(u8 L,u8 M,u8 R)
{
	double Result=0;
    
	return (u8)Result;
}