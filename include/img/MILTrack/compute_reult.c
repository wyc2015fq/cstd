
#include <string.h>
#include <stdlib.h>
#include <math.h> // for sqrt
#include "_MILTrack.h"
#include "macro.h"

//函数功能：返回最佳位置(矩形左上角点坐标)
//YX_candidate为候选(矩形)位置的左上角点坐标
//sample_w和sample_h为(矩形)样本宽和高
//SelectKFeatures为选择出来的K个特征(包括位置、尺度、类型)
//SelectMIU1,SelectDELTA1,SelectMIU2,SelectDELTA2为选择出来的K个特征对应的特征均值和标准差
MILPOINT compute_reult( int num_candidate, const MILPOINT* YX_candidate, int sample_w, int sample_h,
                        int K, const MILHAAR* SelectKFeatures, const int* L_ind,
                        unsigned char* I, int pic_h, int pic_w, int step ) {
                        
  int i, k;
  real max_H_result = -100000000.;
  int max_ind = 0;
  MILPOINT ret;
  int intg_step = ( pic_w + 1 );
  int* intg_pic = MALLOCSET( int, ( pic_h + 1 ) * ( pic_w + 1 ), 0 );
  
  //I = double(imadjust(I));//增加对比度
  imadjust( pic_w * pic_h, I, I );
  //AllFeatures = zeros(num_candidate,K);
  //YX_all = YX_candidate;
  //求积分图
  icvIntegralImage( I, step, intg_pic, intg_step, pic_h, pic_w );
  for ( i = 0; i < num_candidate; ++i ) {
    int X1 = YX_candidate[ i ].x;
    int Y1 = YX_candidate[ i ].y;
    real H_result = 0.;
    int* intg_im;
    //X3 = X1 + sample_w - 1;
    //Y3 = Y1 + sample_h - 1;
    //patch_I = I(Y1:Y3,X1:X3);
#if 1
    
    intg_im = intg_pic + Y1 * intg_step + X1;
#else
    
    intg_step = ( sample_w + 1 );
    intg_im = MALLOC( int, intg_step * ( sample_h + 1 ) );
    icvIntegralImage( I + Y1 * pic_w + X1, pic_w, intg_im, intg_step, sample_h, sample_w ); //求积分图
    
#endif
    
    for ( k = 0; k < K; ++k ) {
      int j = L_ind[ k ];
      real h_result = 0.;
      real miu1 = SelectKFeatures[ j ].MIU1;
      real delta1 = SelectKFeatures[ j ].DELTA1;
      real miu2 = SelectKFeatures[ j ].MIU2;
      real delta2 = SelectKFeatures[ j ].DELTA2;
      MILPOINT FeatureStPos = SelectKFeatures[ j ].StPos;
      MILPOINT FeatureScale = SelectKFeatures[ j ].Scale;
      int FeatureType = SelectKFeatures[ j ].Type;
      real tempFeature = GetOneFeatureValue_2( FeatureType, FeatureStPos, FeatureScale, intg_im, intg_step );
      real P_F_1 = INV_SQ_PI * ( 1 / ( delta1 + ESP ) ) * exp( -( tempFeature - miu1 ) * ( tempFeature - miu1 ) / ( 2 * delta1 * delta1 + ESP ) );
      real P_F_0 = INV_SQ_PI * ( 1 / ( delta2 + ESP ) ) * exp( -( tempFeature - miu2 ) * ( tempFeature - miu2 ) / ( 2 * delta2 * delta2 + ESP ) );
      h_result = log( ( P_F_1 + ESP ) / ( P_F_0 + ESP ) ); //弱分类器输出
      H_result += h_result;
      if ( i == 400 && j == 99 ) {
        int asdf = 0;
      }
    }
    if ( H_result > max_H_result ) {
      max_H_result = H_result;
      max_ind = i;
    }
    if ( intg_im != intg_pic + Y1 * intg_step + X1 ) {
      SAFEFREE( intg_im );
    }
  }
  
  //H_result = sum(h_result,2);//强分类器输出
  //P_y_x = 1/(1+exp(-H_result));
  //[value,max_ind] = max(P_y_x);
  //[value,max_ind] = max_H_result;//若用上两式则可能-H_resul中的所有元素太大使P_y_x中的所有元素均为0
  
  ret.x = YX_candidate[ max_ind ].x;
  ret.y = YX_candidate[ max_ind ].y;
  SAFEFREE( intg_pic );
  
  return ret;
}
