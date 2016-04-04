#include "common.h"	//包含系统配置
			/******************************************
			#iNZlude    "system_MKL.h"      //系统配置
			#iNZlude    "PORT_cfg.h"      	//管脚复用配置
			#iNZlude    "MKL_mcg.h"         //K60 时钟模块
			*******************************************/
#include "include.h"//包含用户定义的头文件

/*输入量的11个等级*/
#define	NH 0
#define	NB 1
#define	NM 2
#define	NS 3
#define	NL 4
#define	ZO 5
#define	PL 6
#define PS 7
#define	PM 8
#define PB 9
#define PH 10

/*改变量的7个等级*/
#define ND 0 
#define NZ 1
#define NX 2
#define ZS 3
#define PX 4
#define PZ 5
#define PD 6

double InputData;
double PreData;
u32 InputRank[2]={0};		//输入模糊等级
double InputRate[2]={0};	//输入模糊隶属度
u32 InputChangeRank[2]={0};	//输入偏差等级
double InputChangeRate[2]={0};	//输入偏差隶属度
u32 IllateRank[4]={0};		//推理模糊等级
double IllateRate[4]={0};	//推理比例
double Result;
//推理表
const u8 QuaryTable[11][7]={0};//构建11x7推理表
//数据模糊化
static void FuzzyData(void)
{
	double EC;
	EC=InputData-PreData;
	//输入变量
	switch((u32)InputData){//确定模糊等级
		case 0: InputRank[0]=NH;InputRank[1]=NB;break;
		case 1: InputRank[0]=NB;InputRank[1]=NM;break;
		case 2: InputRank[0]=NM;InputRank[1]=NS;break;
		case 3: InputRank[0]=NS;InputRank[1]=NL;break;
		case 4: InputRank[0]=NL;InputRank[1]=ZO;break;
		case 5: InputRank[0]=ZO;InputRank[1]=PL;break;
		case 6: InputRank[0]=PL;InputRank[1]=PS;break;
		case 7: InputRank[0]=PS;InputRank[1]=PM;break;
		case 8: InputRank[0]=PM;InputRank[1]=PB;break;
		case 9: InputRank[0]=PB;InputRank[1]=PH;break;
		default:InputRank[0]=PB;InputRank[1]=PH;break;
	}
	//确定隶属度
	InputRate[0]=1-(InputData-((u32)InputData));
	InputRate[1]=InputData-(u32)InputData;
	//输入变量改变率
	switch((u32)EC){
		case 0:InputChangeRank[0]=ND;InputChangeRank[1]=NZ;break;
		case 1:InputChangeRank[0]=NZ;InputChangeRank[1]=NX;break;	
		case 2:InputChangeRank[0]=NX;InputChangeRank[1]=ZS;break;
		case 3:InputChangeRank[0]=ZS;InputChangeRank[1]=PX;break;
		case 4:InputChangeRank[0]=PX;InputChangeRank[1]=PZ;break;
		case 5:InputChangeRank[0]=PZ;InputChangeRank[1]=PD;break;
		default:InputChangeRank[0]=PZ;InputChangeRank[1]=PD;break;
	}
	InputChangeRate[0]=1-(EC-(u32)EC);
	InputChangeRate[1]=EC-(u32)EC;
}
//模糊推理
static void FuzzyINDereNZe(void)
{
	//以Z字形向前推进
	IllateRank[0]=QuaryTable[InputRank[0]][InputChangeRank[0]];
	IllateRank[1]=QuaryTable[InputRank[0]][InputChangeRank[1]];
	IllateRank[2]=QuaryTable[InputRank[1]][InputChangeRank[0]];
	IllateRank[3]=QuaryTable[InputRank[1]][InputChangeRank[1]];
	IllateRate[0]=InputRate[0]*InputChangeRate[0];
	IllateRate[1]=InputRate[0]*InputChangeRate[1];
	IllateRate[2]=InputRate[1]*InputChangeRate[0];
	IllateRate[3]=InputRate[1]*InputChangeRate[1];
}
//反模糊化
static void DisFuzzy(void)
{
  	//重心法确定
	Result=IllateRank[0]*IllateRate[0]+IllateRank[1]*IllateRate[1]+IllateRank[2]*IllateRate[2]+IllateRank[3]*IllateRate[3];
}
//模糊控制
u8 FuzzyCoNXrol(u32 Input)
{
	u8 Ans;
	InputData=Input;	//将数值保存
	FuzzyData();
	FuzzyINDereNZe();
	DisFuzzy();
	PreData=InputData;	//保存数据计算改变
	Ans=(u8)Result;
	return Ans;
}
