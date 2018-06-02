
#include <math.h>
#include "_MILTrack.h"

// 在距离坐标(X,Y)半径为[beita1，beita2]圆环范围内随机选择负样本(指的是样本矩形的左上角点)
int randomSelectSample1( int X, int Y, int sample_w, int sample_h,
                         int pic_w, int pic_h, int beita1, int beita2,
                         int tempnum_neg, MILPOINT* YX_neg ) {
                         
  int domainX1 = X - beita2;
  int domainY1 = Y - beita2;
  int domainX3 = X + beita2;
  int domainY3 = Y + beita2;
  int num, times = 10000;
  
  if ( domainX1 < 1 ) {
    domainX1 = 1;
  }
  if ( domainY1 < 1 ) {
    domainY1 = 1;
  }
  if ( domainX3 >= ( pic_w - sample_w - 1 ) ) {
    domainX3 = ( pic_w - sample_w );
  }
  if ( domainY3 >= ( pic_h - sample_h - 1 ) ) {
    domainY3 = ( pic_h - sample_h );
  }
  
  /* 判断环形的外接矩形面积 */
  if ( domainX1 >= domainX3 || domainY1 >= domainY3 ) {
    return 0;
  }
  
  for ( num = 0; num < tempnum_neg; --times ) {
    int y = unidrnd( domainY3 - domainY1, 1, 1 ) + domainY1;
    int x = unidrnd( domainX3 - domainX1, 1, 1 ) + domainX1;
    int dis = ( Y - y ) * ( Y - y ) + ( X - x ) * ( X - x );
    int tempY = y + sample_h;
    int tempX = x + sample_w;
    if ( dis < ( beita2 * beita2 ) && dis >= ( beita1 * beita1 ) && tempY < pic_h && tempX < pic_w ) { //要防止超出图像范围
      YX_neg[ num ].y = y;
      YX_neg[ num ].x = x;
      ++num;
    }
    if ( times < 0 ) {
      return 0;
    }
  }
  return num;
}

//函数功能：在一定范围内随机选择正样本和负样本，函数返回负样本和正样本的左上角点坐标
//(X,Y)为当前位置矩形框左上角点坐标
//sample_w和sample_h为(矩形)样本宽和高
//pic_w和pic_h为帧的宽和高
//[beita1，beita2]，gama为负样本和正样本的范围
//tempnum_neg和tempnum_pos为负样本和正样本数
int randomSelectSample( int X, int Y, int sample_w, int sample_h, int pic_w, int pic_h,
                        int beita1, int beita2, int gama, int tempnum_neg, int tempnum_pos,
                        int frame, MILPOINT* YX_all ) {
  MILPOINT * YX_neg = YX_all;
  MILPOINT*YX_pos = YX_all + tempnum_neg;
  //在距离坐标(X,Y)半径为[beita1，beita2]圆环范围内随机选择负样本(指的是样本矩形的左上角点)
  int ret;
  ret = randomSelectSample1( X, Y, sample_w, sample_h, pic_w, pic_h, beita1, beita2, tempnum_neg, YX_neg );
  if ( 0 == ret )
    return 0;
  //在距离坐标(X,Y)半径为[0，gama]圆环范围内随机选择选择正样本
  ret = randomSelectSample1( X, Y, sample_w, sample_h, pic_w, pic_h, 0, gama, tempnum_pos, YX_pos );
  return ret;
}

//函数功能：返回在一定范围内随机选择的候选位置(矩形左上角点坐标)
//(X,Y)为当前位置矩形框左上角点坐标
//sample_w和sample_h为(矩形)样本宽和高
//pic_w和pic_h为帧的宽和高
//s为候选样本的范围
//num_candidate_result 为候选样本数
//num_fix_cr 要在原位置附近固定的样本数
int randomSelectSample_for_result( int X, int Y, int sample_w, int sample_h, int pic_w, int pic_h,
                                   int s, int num_candidate_result, int num_fix_cr, MILPOINT* YX_candidate ) {
  int y, x;
  int num = 0;
  int r = ( int ) ( sqrt( num_fix_cr ) * 1. / 2 );
  if ( r >= s ) {
    return 0;
  }
  for ( y = ( Y - r ); y < ( Y + r ) && num < num_fix_cr; ++y ) {
    for ( x = ( X - r ); x < ( X + r ) && num < num_fix_cr; ++x ) {
      int tempY = ( y + sample_h );
      int tempX = ( x + sample_w );
      int dis = ( ( Y - y ) * ( Y - y ) + ( X - x ) * ( X - x ) );
      if ( y > 0 && x > 0 && tempY < pic_h && tempX < pic_w // && dis<(r*r)
         ) {
        YX_candidate[ num ].y = y;
        YX_candidate[ num ].x = x;
        ++num;
      }
    }
  }
  if ( num < num_candidate_result ) {
    return randomSelectSample1( X, Y, sample_w, sample_h, pic_w, pic_h, r, s,
                                num_candidate_result - num, YX_candidate + num );
  }
  return 0;
}
