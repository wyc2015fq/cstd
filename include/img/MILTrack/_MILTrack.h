
#ifndef __MILTRACK_H_
#define __MILTRACK_H_

#include "findfile.h"

#define ESP  0.000000001
#define INV_SQ_PI  0.39894228040143270  // 1/sqrt(2*PI)

#define _USE_LBP_FEATURE
#define _DEBUG_DQ

#if 0
#define DO_FPRINT2D(_F, FMT, _H, _W, _A, _L, _I) FPRINT2D(_F, FMT, _H, _W, _A, _L, _I)
#else
#define DO_FPRINT2D(_F, FMT, _H, _W, _A, _L, _I) {}
#endif

#define FOREACH_DO(_N, _I, _DO)  for (_I=0; _I<(_N); ++_I) {_DO;}

typedef double real;


typedef struct MILPOINT {
  int y;
  int x;
}
MILPOINT;

typedef struct MILHAAR {
  MILPOINT StPos;
  MILPOINT Scale;
  int Kind;
  int Type;
  //高斯分布均值和标准差
  real MIU1;  // 正样本平均值
  real DELTA1; // 正样本方差
  real MIU2;  // 负样本平均值
  real DELTA2; // 负样本方差
}
MILHAAR;

// 函数功能：对于矩阵m*n中的每个元素，随机产生范围为[1,a]的整数
int unidrnd( int a, int m, int n );

//函数功能：从特征池中随机选择M个随机特征(包括位置、尺度和类型)
int randomMFeatures( int sample_w, int sample_h, int M, MILHAAR* MFeatures );

//函数功能：在一定范围内随机选择正样本和负样本，函数返回负样本和正样本的左上角点坐标
//(X,Y)为当前位置矩形框左上角点坐标
//sample_w和sample_h为(矩形)样本宽和高
//pic_w和pic_h为帧的宽和高
//[beita1，beita2]，gama为负样本和正样本的范围
//tempnum_neg和tempnum_pos为负样本和正样本数
int randomSelectSample( int X, int Y, int sample_w, int sample_h, int pic_w, int pic_h,
                        int beita1, int beita2, int gama, int tempnum_neg, int tempnum_pos,
                        int frame, MILPOINT*YX_all );
                        
//函数功能：返回在一定范围内随机选择的候选位置(矩形左上角点坐标)
//(X,Y)为当前位置矩形框左上角点坐标
//sample_w和sample_h为(矩形)样本宽和高
//pic_w和pic_h为帧的宽和高
//s为候选样本的范围
//num_candidate_result 为候选样本数
//num_fix_cr 要在原位置附近固定的样本数
int randomSelectSample_for_result( int X, int Y, int sample_w, int sample_h, int pic_w, int pic_h,
                                   int s, int num_candidate_result, int num_fix_cr, MILPOINT* YX_candidate );
                                   
                                   
//函数功能：返回每个特征在积分图中对应的特征值
int GetOneFeatureValue_2( int FeatureType, MILPOINT FeatureStPos, MILPOINT FeatureScale,
                          int* _InterData, int InterStep );
                          
void my_imadjust( int _N, const unsigned char* f, unsigned char* p );
void histeq( int _N, const unsigned char* f, unsigned char* p );

#ifdef _DEBUG_DQ
#define imadjust(_N, f, p) {}//my_imadjust(_N, f, p)
#else
#define imadjust(_N, f, p) {}
#endif

//函数功能：返回最佳位置(矩形左上角点坐标)
//YX_candidate为候选(矩形)位置的左上角点坐标
//sample_w和sample_h为(矩形)样本宽和高
//SelectKFeatures为选择出来的K个特征(包括位置、尺度、类型)
//SelectMIU1,SelectDELTA1,SelectMIU2,SelectDELTA2为选择出来的K个特征对应的特征均值和标准差
MILPOINT compute_reult( int num_candidate, const MILPOINT* YX_candidate, int sample_w, int sample_h,
                        int K, const MILHAAR* SelectKFeatures, const int* L_ind,
                        unsigned char* I, int pic_h, int pic_w, int step );
                        
void candidateWeakClf( int tempnum_neg, int tempnum_pos, const MILPOINT* YX_all, int sample_w, int sample_h,
                       MILHAAR* MFeatures, int M, unsigned char* I, int pic_h, int pic_w, int step,
                       real v, int frame, real* _h_candidateWeakClf );
                       
int icvIntegralImage( const unsigned char* src, int srcstep, int* sum, int sumstep, int size_height, int size_width );

/*参数设置
影响跟踪精度的主要参数为M、K (尤为重要)、正样本数、
搜索样本数、学习率v 通过把学习率调节到接近1，如0.99，
可大幅度提高跟踪质量，但这样会使特征均值和标准差的更新速度变慢，
可能不利于表面在很短时间内变化较大的情况)
*/

typedef struct MILTRACK {
  int M; // = 1000; //候选特征数(候选分类器数)
  int K; // = 100; //选出特征数(选出分类器数)
  int num_neg; // = 65;//负样本数
  int num_pos; // = 45;//正样本数
  //int num_all; // = 110; //正样本数
  int num_candidate_result; // = 500; //搜索样本数,要>=num_fix_cr
  //int num_replace_clk = 5;//用于替换的分类器(特征)数
  int beita1; // = 50;//随机产生负样本用到的距离1
  int beita2; // = 80;//随机产生负样本用到的距离2
  int gama; //  = 5; //随机产生正样本用到的距离
  int s; // = 30;//候选样本区域,要>=gama,且最好保证s > (num_fix_cr/10)/2
  int num_fix_cr; // = 300;//要在原位置附近固定的样本数,目前取值为100、200和300，见函数randomSelectSample_for_result.m
  real v; // = 0.95;//学习率,0.85~1,v越小特征均值和标准差就更新得越快，v = 1则不更新特征均值和标准差，对于表面不变或者变化很缓慢的对象v应该取到接近1
  
  int X1, Y1, sample_w, sample_h;
  MILHAAR* MFeatures; // [M]
  real* L; // [M]
  int* L_ind; // [M]
  
  MILPOINT* YX_all; // = NULL; // [(num_neg+num_pos)]
  MILPOINT* YX_candidate; // = NULL; // [num_candidate_result]
  real* _h_candidateWeakClf; // [M*(num_neg+num_pos)]
}
MILTRACK;

MILTRACK* NewMilTrack(
  int M,  // = 1000, //候选特征数(候选分类器数)
  int K,  // = 100, //选出特征数(选出分类器数)
  int num_neg,  // = 65,//负样本数
  int num_pos,  // = 45,//正样本数
  //int num_all, // = 110, //正样本数
  int num_candidate_result, // = 500, //搜索样本数,要>=num_fix_cr
  //int num_replace_clk = 5,//用于替换的分类器(特征)数
  int beita1, // = 50,//随机产生负样本用到的距离1
  int beita2, // = 80,//随机产生负样本用到的距离2
  int gama, //  = 5, //随机产生正样本用到的距离
  int s, // = 30,//候选样本区域,要>=gama,且最好保证s > (num_fix_cr/10)/2
  int num_fix_cr, // = 300,//要在原位置附近固定的样本数,目前取值为100、200和300，见函数randomSelectSample_for_result.m
  real v // = 0.95,//学习率,0.85~1,v越小特征均值和标准差就更新得越快，v = 1则不更新特征均值和标准差，对于表面不变或者变化很缓慢的对象v应该取到接近1
);

#endif // __MILTRACK_H_
