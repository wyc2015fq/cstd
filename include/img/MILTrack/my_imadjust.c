
#include <math.h>
#include <stdio.h>
#include "_MILTrack.h"
#include "direct.h"
#include "utime.h"

// 用映射表m把f 映射到 p
#define MAPPING(_N, _A, _B, _M) {int _I=0; for (; _I<_N; ++_I) { _B[_I] = _M[(int)(_A[_I])]; } }

#define HIST(_N, _A, _H) {int _I=0; for (; _I<_N; ++_I) { ++_H[(int)(_A[_I])]; } }

void histeq( int _N, const unsigned char* f, unsigned char* p ) {
  enum { BIT = 8, SCAL = 14, };
  enum { LV = 1 << BIT, };
  const int MN = ( LV << SCAL ) / ( _N );
  int t, i; // 循环变量
  unsigned char bMap[ LV ] = {0}; // 灰度映射表
  int nCnt[ LV ] = {0}; // 累积直方图
  HIST( _N, f, nCnt );
  bMap[ 0 ] = ( nCnt[ 0 ] * MN );
  for ( i = 1; i < LV; ++i ) {
    nCnt[ i ] += nCnt[ i - 1 ];
    t = ( nCnt[ i ] * MN ) >> SCAL;
    bMap[ i ] = ( t >= LV ) ? ( LV - 1 ) : ( t ); // 计算对应的新灰度值
  }
  MAPPING( _N, f, p, bMap );
  return ;
}

//函数功能：增加灰度图像的对比度，等效于matlab自带函数imadjust,即等效于OutImage = imadjust(InImage);
//InImage为输入灰度的图像
//OutImage为增加对比度后的灰度图像
void my_imadjust( int _N, const unsigned char* f, unsigned char* p ) {
  enum { BIT = 8, SCAL = 14, };
  enum { LV = 1 << BIT, };
  int t, i; // 循环变量
  unsigned char bMap[ LV ] = {0}; // 灰度映射表
  int nCnt[ LV ] = {0}; // 累积直方图
  int low_in;
  int high_in;
  int tol_low = ( int ) ( 0.01 * _N );
  int tol_high = ( int ) ( 0.01 * _N );
  HIST( _N, f, nCnt );
  for ( t = 0, low_in = 0; low_in < LV; ++low_in ) {
    t += nCnt[ low_in ];
    if ( t > tol_low ) {
      break;
    }
  }
  for ( t = 0, high_in = LV; high_in-- > 0; ) {
    t += nCnt[ high_in ];
    if ( t > tol_high ) {
      break;
    }
  }
  {
    int len = ( high_in - low_in );
    for ( i = 0; i < LV; ++i ) {
      if ( i < low_in ) {
        bMap[ i ] = 0;
      } else if ( i >= high_in ) {
        bMap[ i ] = LV - 1;
      } else {
        //查找表所用到的表，即lut = uint8(round(temp2.*255));
        bMap[ i ] = ( LV * ( i - low_in ) ) / len; // 计算对应的新灰度值
      }
    }
  }
  //如InImage中第100个位置的元素值为a,则OutImage相同位置的元素值为lut(a+1)
  MAPPING( _N, f, p, bMap );
  return ;
}
