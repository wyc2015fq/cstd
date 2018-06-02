
#ifndef _MILTRACK_H_
#define _MILTRACK_H_

#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct MILTRACK MILTRACK;
  
  MILTRACK* CPM_NewMilTrack(
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
    double v // = 0.95,//学习率,0.85~1,v越小特征均值和标准差就更新得越快，v = 1则不更新特征均值和标准差，对于表面不变或者变化很缓慢的对象v应该取到接近1
  );
  
  int CPM_DelMilTrack( MILTRACK * pmt );
  
  /*
  函数功能：选择一个跟踪目标
  参数说明：
  pic_h     - 图片高
  pic_w     - 图片宽
  I         - 图片左上角指针
  step      - 图片每行象素数
  Y1        - 跟踪目标矩形框左上角 y坐标
  X1        - 跟踪目标矩形框左上角 x坐标
  sample_h  - 跟踪目标矩形框高
  sample_w  - 跟踪目标矩形框宽
  返回：0表示失败，1表示成功
  */
  int CPM_MilTrackSelectObject( MILTRACK * pmt,
                                int pic_h, int pic_w, unsigned char * I, int step,
                                int Y1, int X1, int sample_h, int sample_w );
                                
  /*
  函数功能：估计待跟踪目标在图片中的左上角坐标(X1, Y1)
  参数说明：
  pic_h     - 图片高
  pic_w     - 图片宽
  I         - 图片左上角指针
  step      - 图片每行象素数
  Y1        - 跟踪目标矩形框左上角 y坐标指针
  X1        - 跟踪目标矩形框左上角 x坐标指针
  返回：0表示失败，1表示成功
  */
  int CPM_MilTrack( MILTRACK * pmt, int pic_h, int pic_w, unsigned char * I, int step,
                    int * Y1, int * X1 );
                    
#ifdef __cplusplus
}
#endif

#endif // _MILTRACK_H_
