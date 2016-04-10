#include "common.h"	//包含系统配置
			/******************************************
			#include    "system_MKL.h"      //系统配置
			#include    "PORT_cfg.h"      	//管脚复用配置
			#include    "MKL_mcg.h"         //K60 时钟模块
			*******************************************/
#include "include.h"//包含用户定义的头文件
/*
*程序说明：
1、首先计算电感构成的3个点
2、然后整理成为矩阵
3、计算逆矩阵
4、最后计算逆矩阵和结果矩阵即为系数矩阵
*/



//左中右三个电感数据
u8 ComputMatrix(unsigned char X1,unsigned char X2,unsigned char X3)
{
#define Y1	-110
#define Y2	0
#define Y3	110
  double Sum=0;
  double C1,C2,C3;		//计算最后的结果
  double Matrix[3][3];		//3X3存储电磁数据
  //计算行列式a1·b2·c3+b1·c2·a3+c1·a2·b3-a3·b2·c1-b3·c2·a1-c3·a2·b1
  Sum= X2*X3*X3+X1*X2*X2+X1*X3*X3-X2*X1*X1-X3*X2*X2-X1*X1*X3;	//计算三阶矩阵的行列式
  //计算（电感，距离）组成的逆矩阵
  //1,X,X*X
	Matrix[0][0]	=	X3*X2*(X3-X2)		/Sum;
	Matrix[1][0]	=	X3*X1*(X1-X3)		/Sum;
	Matrix[2][0]	=	X1*X2*(X2-X1)		/Sum;
	Matrix[0][1]	=	(X3+X2)*(X2-X3)		/Sum;
	Matrix[1][1]	=	(X3+X1)*(X3-X1)		/Sum;
	Matrix[2][1]	=	(X2+X1)*(X1-X2)		/Sum;
	Matrix[0][2]	=	(X3-X2)			/Sum;
	Matrix[1][2]	=	(X1-X3)			/Sum;
	Matrix[2][2]	=	(X2-X1)			/Sum;
  
  
  //计算系数矩阵
  C1=Matrix[0][0]*Y1+Matrix[0][1]*Y2+Matrix[0][1]*Y3;
  C2=Matrix[1][0]*Y1+Matrix[1][1]*Y2+Matrix[1][2]*Y3;
  C3=Matrix[2][0]*Y1+Matrix[2][1]*Y2+Matrix[2][2]*Y3;
  
  return ((u8)(-C2/C3))*10;//求拟合曲线的中间值
  
  
}
