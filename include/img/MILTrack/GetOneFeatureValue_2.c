
#include "_MILTrack.h"

#ifdef _USE_LBP_FEATURE 
// 初始化滤波算子
/*  0,  1,  2,  3, */
/*  4,  5,  6,  7, */
/*  8,  9, 10, 11, */
/* 12, 13, 14, 15, */
void lbp_ex_init( int k0, int al, int ai, int* u ) {
  int i;
  int k2 = k0 + k0 + 1;
  int pos = -( k0 + k2 ) * ( ai + al );
  ai *= k2;
  al *= k2;
  for ( i = 0; i < 4; ++i, pos += al ) {
    u[ i * 4 + 0 ] = pos;
    u[ i * 4 + 1 ] = pos + ai;
    u[ i * 4 + 2 ] = pos + ai + ai;
    u[ i * 4 + 3 ] = pos + ai + ai + ai;
  }
}

//函数功能：返回每个特征在积分图中对应的特征值
int GetOneFeatureValue_2( int FeatureType, MILPOINT FeatureStPos, MILPOINT FeatureScale,
                          int* _InterData, int InterStep ) {
  int umap[ 16 ];
  lbp_ex_init( 1, InterStep, 1, umap );
  //LBP_EX_FT((y+A0), yl, 1, (img2+B0));
#define EVELV(_A, a, b, c, d)   (*(_A+umap[a]) - *(_A+umap[b]) - *(_A+umap[c]) + *(_A+umap[d]))
  
  // lbp滤波算子
  {
    int p = ( unsigned char ) 0;
    int A0 = FeatureStPos.y * InterStep + FeatureStPos.x;
    const int* yy = ( _InterData + A0 );
    int v1, v2, v3, v4, v5, v6, v7, v8, v9;
    
#if 0
    
    v5 = ( EVELV( yy, 5, 6, 9, 10 ) );
    v1 = ( EVELV( yy, 0, 1, 4, 5 ) >= v5 ) << 7;
    v2 = ( EVELV( yy, 1, 2, 5, 6 ) >= v5 ) << 6;
    v3 = ( EVELV( yy, 2, 3, 6, 7 ) >= v5 ) << 5;
    v4 = ( EVELV( yy, 6, 7, 10, 11 ) >= v5 ) << 4;
    v6 = ( EVELV( yy, 10, 11, 14, 15 ) >= v5 ) << 3;
    v7 = ( EVELV( yy, 9, 10, 13, 14 ) >= v5 ) << 2;
    v8 = ( EVELV( yy, 8, 9, 12, 13 ) >= v5 ) << 1;
    v9 = ( EVELV( yy, 4, 5, 8, 9 ) >= v5 );
    p = ( v1 | v2 | v3 | v4 | v6 | v7 | v8 | v9 );
#else
    
    v5;
    v1 = ( EVELV( yy, 0, 1, 4, 5 ) ) << 7;
    v2 = ( EVELV( yy, 1, 2, 5, 6 ) ) << 6;
    v3 = ( EVELV( yy, 2, 3, 6, 7 ) ) << 5;
    v4 = ( EVELV( yy, 6, 7, 10, 11 ) ) << 4;
    v6 = ( EVELV( yy, 10, 11, 14, 15 ) ) << 3;
    v7 = ( EVELV( yy, 9, 10, 13, 14 ) ) << 2;
    v8 = ( EVELV( yy, 8, 9, 12, 13 ) ) << 1;
    v9 = ( EVELV( yy, 4, 5, 8, 9 ) );
    p = ( v1 + v2 + v3 + v4 + v6 + v7 + v8 + v9 );
#endif
    
    return p;
  }
}
#else 
//函数功能：返回每个特征在积分图中对应的特征值
int GetOneFeatureValue_2( int FeatureType, MILPOINT FeatureStPos, MILPOINT FeatureScale,
                          int* _InterData, int InterStep ) {
  int y = 0;
#define InterData(y, x)  _InterData[(y+1)*InterStep+(x+1)]
  //#define InterData(y, x)  _InterData[(y)*InterStep+(x)]
  //////////////////////////////////////////////
  //  如果计算的是A类特征 //
  //////////////////////////////////////////////
  if ( FeatureType == 0 ) {

    int posLeftTop0Y = FeatureStPos.y;
    int posLeftTop0X = FeatureStPos.x;
    int posMiddleTop0Y = FeatureStPos.y;
    int posMiddleTop0X = posLeftTop0X + FeatureScale.x;
    int posRightTop0Y = FeatureStPos.y;
    int posRightTop0X = posMiddleTop0X + FeatureScale.x;
    int posLeftBottom0Y = posLeftTop0Y + FeatureScale.y;
    int posLeftBottom0X = posLeftTop0X;
    int posMiddleBottom0Y = posLeftBottom0Y;
    int posMiddleBottom0X = posMiddleTop0X;
    int posRightBottom0Y = posLeftBottom0Y;
    int posRightBottom0X = posRightTop0X;

    //         y = InterData ( posLeftTop0Y,posLeftTop0X ) + InterData ( posRightTop0Y,posRightTop0X ) ...
    //             - InterData (posLeftBottom0Y,posLeftBottom0X) - InterData (posRightBottom0Y,posRightBottom0X)...
    //             + 2 * (InterData (posMiddleBottom0Y,posMiddleBottom0X) - InterData (posMiddleTop0Y,posMiddleTop0X));
    y = 1 * ( InterData( posLeftTop0Y, posLeftTop0X ) + InterData( posRightBottom0Y, posRightBottom0X ) - InterData( posLeftBottom0Y, posLeftBottom0X ) - InterData( posRightTop0Y, posRightTop0X ) )
        - 2 * ( InterData( posMiddleTop0Y, posMiddleTop0X ) + InterData( posRightBottom0Y, posRightBottom0X ) - InterData( posMiddleBottom0Y, posMiddleBottom0X ) - InterData( posRightTop0Y, posRightTop0X ) );
  }
  //////////////////////////////////////////////
  //  如果计算的是B类特征 //
  //////////////////////////////////////////////
  if ( FeatureType == 1 ) {

    int posLeftTop1Y = FeatureStPos.y;
    int posLeftTop1X = FeatureStPos.x;
    int posRightTop1Y = posLeftTop1Y;
    int posRightTop1X = posLeftTop1X + FeatureScale.x;
    int posLeftMiddle1Y = posLeftTop1Y + FeatureScale.y;
    int posLeftMiddle1X = posLeftTop1X;
    int posRightMiddle1Y = posLeftMiddle1Y;
    int posRightMiddle1X = posRightTop1X;
    int posLeftBottom1Y = posLeftMiddle1Y + FeatureScale.y;
    int posLeftBottom1X = posLeftTop1X;
    int posRightBottom1Y = posLeftBottom1Y;
    int posRightBottom1X = posRightTop1X;

    //         y = InterData (posLeftTop1Y,posLeftTop1X) + InterData (posLeftBottom1Y,posLeftBottom1X)...
    //             - InterData (posRightTop1Y,posRightTop1X) - InterData (posRightBottom1Y,posRightBottom1X)...
    //             + 2 * (InterData (posRightMiddle1Y,posRightMiddle1X) - InterData (posLeftMiddle1Y,posLeftMiddle1X));
    y = 1 * ( InterData( posLeftTop1Y, posLeftTop1X ) + InterData( posRightBottom1Y, posRightBottom1X ) - InterData( posLeftBottom1Y, posLeftBottom1X ) - InterData( posRightTop1Y, posRightTop1X ) )
        - 2 * ( InterData( posLeftMiddle1Y, posLeftMiddle1X ) + InterData( posRightBottom1Y, posRightBottom1X ) - InterData( posLeftBottom1Y, posLeftBottom1X ) - InterData( posRightMiddle1Y, posRightMiddle1X ) );


  }
  //////////////////////////////////////////////
  //  如果计算的是C类特征 //
  //////////////////////////////////////////////
  if ( FeatureType == 2 ) {

    int posLeftTop2Y = FeatureStPos.y;
    int posLeftTop2X = FeatureStPos.x;
    int posMiddleLTop2Y = posLeftTop2Y;
    int posMiddleLTop2X = posLeftTop2X + FeatureScale.x;
    int posMiddleRTop2Y = posLeftTop2Y;
    int posMiddleRTop2X = posMiddleLTop2X + FeatureScale.x;
    int posRightTop2Y = posLeftTop2Y;
    int posRightTop2X = posMiddleRTop2X + FeatureScale.x;
    int posLeftBottom2Y = posLeftTop2Y + FeatureScale.y;
    int posLeftBottom2X = posLeftTop2X;
    int posMiddleLBottom2Y = posLeftBottom2Y;
    int posMiddleLBottom2X = posMiddleLTop2X;
    int posMiddleRBottom2Y = posLeftBottom2Y;
    int posMiddleRBottom2X = posMiddleRTop2X;
    int posRightBottom2Y = posLeftBottom2Y;
    int posRightBottom2X = posRightTop2X;

    //         y = InterData (posLeftTop2Y,posLeftTop2X) + InterData (posRightBottom2Y,posRightBottom2X)...
    //              - InterData (posRightTop2Y,posRightTop2X) - InterData (posLeftBottom2Y,posLeftBottom2X)...
    //              + 2*(InterData (posMiddleRTop2Y,posMiddleRTop2X) + InterData (posMiddleLBottom2Y,posMiddleLBottom2X)...
    //              - InterData (posMiddleLTop2Y,posMiddleLTop2X) - InterData (posMiddleRBottom2Y,posMiddleRBottom2X));

    y = 1 * ( InterData( posLeftTop2Y, posLeftTop2X ) + InterData( posRightBottom2Y, posRightBottom2X ) - InterData( posLeftBottom2Y, posLeftBottom2X ) - InterData( posRightTop2Y, posRightTop2X ) )
        - 3 * ( InterData( posMiddleLTop2Y, posMiddleLTop2X ) + InterData( posMiddleRBottom2Y, posMiddleRBottom2X ) - InterData( posMiddleLBottom2Y, posMiddleLBottom2X ) - InterData( posMiddleRTop2Y, posMiddleRTop2X ) );
  }

  //////////////////////////////////////////////
  //  如果计算的是D类特征 //
  //////////////////////////////////////////////
  if ( FeatureType == 3 ) {

    int posLeftTop3Y = FeatureStPos.y;
    int posLeftTop3X = FeatureStPos.x;
    int posMiddleTop3Y = posLeftTop3Y;
    int posMiddleTop3X = posLeftTop3X + FeatureScale.x;
    int posRightTop3Y = posLeftTop3Y;
    int posRightTop3X = posMiddleTop3X + FeatureScale.x;
    int posLeftMiddle3Y = posLeftTop3Y + FeatureScale.y;
    int posLeftMiddle3X = posLeftTop3X;
    int posMiddleMiddle3Y = posLeftMiddle3Y;
    int posMiddleMiddle3X = posMiddleTop3X;
    int posRightMiddle3Y = posLeftMiddle3Y;
    int posRightMiddle3X = posRightTop3X;
    int posLeftBottom3Y = posLeftMiddle3Y + FeatureScale.y;
    int posLeftBottom3X = posLeftTop3X;
    int posMiddleBottom3Y = posLeftBottom3Y;
    int posMiddleBottom3X = posMiddleTop3X;
    int posRightBottom3Y = posLeftBottom3Y;
    int posRightBottom3X = posRightTop3X;

    //         y = 2 * (2*InterData (posMiddleMiddle3Y,posMiddleMiddle3X) - InterData (posMiddleTop3Y,posMiddleTop3X)...
    //            - InterData (posLeftMiddle3Y,posLeftMiddle3X) - InterData (posRightMiddle3Y,posRightMiddle3X)...
    //            - InterData (posMiddleBottom3Y,posMiddleBottom3X)) + InterData (posLeftTop3Y,posLeftTop3X)...
    //            + InterData (posRightTop3Y,posRightTop3X) + InterData (posLeftBottom3Y,posLeftBottom3X)...
    //            +  InterData (posRightBottom3Y,posRightBottom3X);
#if 1

    y = 1 * ( InterData( posLeftTop3Y, posLeftTop3X ) + InterData( posRightBottom3Y, posRightBottom3X ) - InterData( posLeftBottom3Y, posLeftBottom3X ) - InterData( posRightTop3Y, posRightTop3X ) )
        - 2 * ( InterData( posLeftMiddle3Y, posLeftMiddle3X ) + InterData( posMiddleBottom3Y, posMiddleBottom3X ) - InterData( posLeftBottom3Y, posLeftBottom3X ) - InterData( posMiddleMiddle3Y, posMiddleMiddle3X ) )
        - 2 * ( InterData( posMiddleTop3Y, posMiddleTop3X ) + InterData( posRightMiddle3Y, posRightMiddle3X ) - InterData( posRightTop3Y, posRightTop3X ) - InterData( posMiddleMiddle3Y, posMiddleMiddle3X ) );

#else

int a = InterData( posLeftTop3Y, posLeftTop3X ) + InterData( posRightBottom3Y, posRightBottom3X ) - InterData( posLeftBottom3Y, posLeftBottom3X ) - InterData( posRightTop3Y, posRightTop3X );
int b = InterData( posLeftMiddle3Y, posLeftMiddle3X ) + InterData( posMiddleBottom3Y, posMiddleBottom3X ) - InterData( posLeftBottom3Y, posLeftBottom3X ) - InterData( posMiddleMiddle3Y, posMiddleTop3X );
int c = InterData( posMiddleTop3Y, posMiddleTop3X ) + InterData( posRightMiddle3Y, posRightMiddle3X ) - InterData( posRightTop3Y, posRightTop3X ) - InterData( posMiddleMiddle3Y, posMiddleMiddle3X );

a = _InterData[ posLeftTop3Y * InterStep + posLeftTop3X ] + _InterData[ posRightBottom3Y * InterStep + posRightBottom3X ] - _InterData[ posLeftBottom3Y * InterStep + posLeftBottom3X ] - _InterData[ posRightTop3Y * InterStep + posRightTop3X ];
b = _InterData[ posLeftMiddle3Y * InterStep + posLeftMiddle3X ] + _InterData[ posMiddleBottom3Y * InterStep + posMiddleBottom3X ] - _InterData[ posLeftBottom3Y * InterStep + posLeftBottom3X ] - _InterData[ posMiddleMiddle3Y * InterStep + posMiddleTop3X ];
c = _InterData[ posMiddleTop3Y * InterStep + posMiddleTop3X ] + _InterData[ posRightMiddle3Y * InterStep + posRightMiddle3X ] - _InterData[ posRightTop3Y * InterStep + posRightTop3X ] - _InterData[ posMiddleMiddle3Y * InterStep + posMiddleMiddle3X ];

y = 1 * ( InterData( posLeftTop3Y, posLeftTop3X ) + InterData( posRightBottom3Y, posRightBottom3X ) - InterData( posLeftBottom3Y, posLeftBottom3X ) - InterData( posRightTop3Y, posRightTop3X ) )
    - 2 * ( InterData( posLeftMiddle3Y, posLeftMiddle3X ) + InterData( posMiddleBottom3Y, posMiddleBottom3X ) - InterData( posLeftBottom3Y, posLeftBottom3X ) - InterData( posMiddleMiddle3Y, posMiddleTop3X ) )
    - 2 * ( InterData( posMiddleTop3Y, posMiddleTop3X ) + InterData( posRightMiddle3Y, posRightMiddle3X ) - InterData( posRightTop3Y, posRightTop3X ) - InterData( posMiddleMiddle3Y, posMiddleMiddle3X ) );


#endif

  }
  //////////////////////////////////////////////
  //  如果计算的是E类特征 //
  //////////////////////////////////////////////
  if ( FeatureType == 4 ) {

    int posLeftTop4Y = FeatureStPos.y;
    int posLeftTop4X = FeatureStPos.x;
    int posRightTop4Y = posLeftTop4Y;
    int posRightTop4X = posLeftTop4X + FeatureScale.x;
    int posLeftTMiddle4Y = posLeftTop4Y + FeatureScale.y;
    int posLeftTMiddle4X = posLeftTop4X;
    int posRightTMiddle4Y = posLeftTMiddle4Y;
    int posRightTMiddle4X = posRightTop4X;
    int posLeftBMiddle4Y = posLeftTMiddle4Y + FeatureScale.y;
    int posLeftBMiddle4X = posLeftTop4X;
    int posRightBMiddle4Y = posLeftBMiddle4Y;
    int posRightBMiddle4X = posRightTop4X;
    int posLeftBottom4Y = posLeftBMiddle4Y + FeatureScale.y;
    int posLeftBottom4X = posLeftTop4X;
    int posRightBottom4Y = posLeftBottom4Y;
    int posRightBottom4X = posRightTop4X;

    //         y = InterData (posLeftTop4Y,posLeftTop4X) + InterData (posRightBottom4Y,posRightBottom4X)...
    //                 - InterData (posRightTop4Y,posRightTop4X) - InterData (posLeftBottom4Y,posLeftBottom4X)...
    //                 + 2*(InterData (posRightTMiddle4Y,posRightTMiddle4X) + InterData (posLeftBMiddle4Y,posLeftBMiddle4X)...
    //                 - InterData (posLeftTMiddle4Y,posLeftTMiddle4X) - InterData (posRightBMiddle4Y,posRightBMiddle4X));
    int a = InterData( posLeftTop4Y, posLeftTop4X ) + InterData( posRightBottom4Y, posRightBottom4X ) - InterData( posLeftBottom4Y, posLeftBottom4X ) - InterData( posRightTop4Y, posRightTop4X );
    int b = InterData( posLeftTMiddle4Y, posLeftTMiddle4X ) + InterData( posRightBMiddle4Y, posRightBMiddle4X ) - InterData( posLeftBMiddle4Y, posLeftBMiddle4X ) - InterData ( posRightTMiddle4Y, posRightTMiddle4X );
    y = 1 * ( InterData( posLeftTop4Y, posLeftTop4X ) + InterData( posRightBottom4Y, posRightBottom4X ) - InterData( posLeftBottom4Y, posLeftBottom4X ) - InterData( posRightTop4Y, posRightTop4X ) )
        - 3 * ( InterData( posLeftTMiddle4Y, posLeftTMiddle4X ) + InterData( posRightBMiddle4Y, posRightBMiddle4X ) - InterData( posLeftBMiddle4Y, posLeftBMiddle4X ) - InterData ( posRightTMiddle4Y, posRightTMiddle4X ) );
  }

#undef InterData

  return y;
}

#endif
