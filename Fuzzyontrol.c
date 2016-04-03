#include "common.h"	//包含系统配置
			/******************************************
			#include    "system_MKL.h"      //系统配置
			#include    "PORT_cfg.h"      	//管脚复用配置
			#include    "MKL_mcg.h"         //K60 时钟模块
			*******************************************/
#include "include.h"//包含用户定义的头文件
u32 InputClass[2]={0};	//输入
u32 InputRatio[2]={0};
u32 InputChangeClass[2]={0};//输入偏差
u32 InputChangeRatio[2]={0};
u32 IllateClass[4]={0};	//推理模糊等级
u32 IllateRatio[4]={0};	//推理比例

//数据模糊化
static void FuzzyData(u8 value)
{
	 
}
//模糊推理
static void FuzzyInference(void)
{
  
}
//反模糊化
static void DisFuzzy(void)
{

}
//模糊控制
u8 FuzzyControl(u32 Input)
{
  return 0;
}
