
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "_faceRecognition.h"



#define INTCAST(_X)  (int)(_X)

const unsigned char lbpmap[ 6 ][ 256 ] = {
                                   {
#include ".\txt\mapping_3.txt"
                                   
                                   },
                                   {
#include ".\txt\mapping_9.txt"
                                   },
                                   {
#include ".\txt\mapping_15.txt"
                                   },
                                   {
#include ".\txt\mapping_21.txt"
                                   },
                                   {
#include ".\txt\mapping_27.txt"
                                   },
                                   {
#include ".\txt\mapping_33.txt"
                                   },
                                 };
const unsigned char* lbpmap59bin = lbpmap[ 0 ];

static int lbp_Feature_List_3[] = {
#include ".\txt\Feature_List_3.txt"
                                  };
                                  
static int lbp_Feature_List_9[] = {
#include ".\txt\Feature_List_9.txt"
                                  };
                                  
static int lbp_Feature_List_15[] = {
#include ".\txt\Feature_List_15.txt"
                                   };
                                   
static int lbp_Feature_List_21[] = {
#include ".\txt\Feature_List_21.txt"
                                   };
                                   
static int lbp_Feature_List_27[] = {
#include ".\txt\Feature_List_27.txt"
                                   };
                                   
static int lbp_Feature_List_33[] = {
#include ".\txt\Feature_List_33.txt"
                                   };
                                   
#if 1
static int lbp_Feature_idyw[] = {
#include ".\txt\exclude1\idyw6.txt"
                                };
                                
static double lbp_Feature_weightyw[] = {
#include ".\txt\exclude1\weightyw6.txt"
                                       };
                                       
#else
static int lbp_Feature_idyw[] = {
#include ".\txt\include1\idyw.txt"
                                };
                                       
static double lbp_Feature_weightyw[] = {
#include ".\txt\include1\weightyw.txt"
                                       };
                                       
#endif
                                       
typedef struct lbp_Feature_List_T {
  int* ptr;
  int len;
  int bin;
}
lbp_Feature_List_T;

static lbp_Feature_List_T lbp_Feature_List[] =
  {
    { lbp_Feature_List_3, sizeof( lbp_Feature_List_3 ) / ( sizeof( int ) * 4 ), 59 },
    { lbp_Feature_List_9, sizeof( lbp_Feature_List_9 ) / ( sizeof( int ) * 4 ), 64 },
    { lbp_Feature_List_15, sizeof( lbp_Feature_List_15 ) / ( sizeof( int ) * 4 ), 64 },
    { lbp_Feature_List_21, sizeof( lbp_Feature_List_21 ) / ( sizeof( int ) * 4 ), 64 },
    { lbp_Feature_List_27, sizeof( lbp_Feature_List_27 ) / ( sizeof( int ) * 4 ), 64 },
    { lbp_Feature_List_33, sizeof( lbp_Feature_List_33 ) / ( sizeof( int ) * 4 ), 64 }
  };
  
#define MAP_AND_MUL(_N, _F, _P, _M, _W)                                    \
do {                                                                       \
  int _I;                                                                  \
  for (_I = 0; _I < _N; ++_I) {                                            \
    *(_F + _I) = _P[*(_M + _I)];/* *(*w++); */                             \
  }                                                                        \
} while(0)
  
#define EVELV(_A, a, b, c, d)   (*(_A+u[a]) - *(_A+u[b]) - *(_A+u[c]) + *(_A+u[d]))
  
// lbp滤波算子
#define LBP_EX_FT(_A, srcstep, cn, B)                                           \
do {                                                                       \
    unsigned char p = (unsigned char)0;                                                    \
    int v5 = (int)EVELV(_A, 5, 6, 9, 10);                                  \
    p |= (unsigned char)( EVELV(_A,   0,  1,  4,  5) >= v5 ); p <<= 1;             \
    p |= (unsigned char)( EVELV(_A,   1,  2,  5,  6) >= v5 ); p <<= 1;             \
    p |= (unsigned char)( EVELV(_A,   2,  3,  6,  7) >= v5 ); p <<= 1;             \
    p |= (unsigned char)( EVELV(_A,   6,  7, 10, 11) >= v5 ); p <<= 1;             \
    p |= (unsigned char)( EVELV(_A,  10, 11, 14, 15) >= v5 ); p <<= 1;             \
    p |= (unsigned char)( EVELV(_A,   9, 10, 13, 14) >= v5 ); p <<= 1;             \
    p |= (unsigned char)( EVELV(_A,   8,  9, 12, 13) >= v5 ); p <<= 1;             \
    p |= (unsigned char)( EVELV(_A,   4,  5,  8,  9) >= v5 );                      \
    *(B) = __lbpmap0[p];                                                   \
} while(0)

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

size_t lbp_ex_size_bin( size_t i ) {
  return lbp_Feature_List[ i ].bin;
}
size_t lbp_ex_size_len( size_t i ) {
  return lbp_Feature_List[ i ].len;
}
const int* lbp_ex_begin_ptr( size_t i ) {
  return lbp_Feature_List[ i ].ptr;
}

enum { FN = sizeof( lbp_Feature_List ) / sizeof( lbp_Feature_List_T ) };

// 一个bin产生的特征数目
int lbp_ex_size( int i ) {
  if ( i < FN ) {
    return lbp_Feature_List[ i ].len * lbp_Feature_List[ i ].bin;
  }
  return 0;
}

// 总特征数目
int init_size() {
  int k, i;
  for ( k = 0, i = 0; i < FN; ++i ) {
    k += lbp_Feature_List[ i ].len * lbp_Feature_List[ i ].bin;
  }
  return k;
}

/* 最高位最多乘2^7，所以<<操作最多7次， */
/* 最后一次按位与即可完成+1或+0的操作(即1*2^0或者0*2^0)。 */
#define SKIP_BLOCK_HIST(h, w, src, srcstep, cn, B, bin, rect, n)              \
do {                                                                          \
  int _N=(n)*bin, _bin;                                                       \
  for (_bin=0; _bin<_N; rect+=4, _bin+=bin) {                                 \
    int x = rect[1], y = rect[0], cx=rect[3], cy=rect[2];                     \
    int pos = y*(srcstep)+x*(cn);                                             \
    SKIP_HIST(cy, cx, (src+pos), srcstep, cn, (B+_bin));                      \
  }                                                                           \
} while(0)


#define INTINT int

// 多bin的lbp滤波
int lbp_ex_ft_all( int srch, int srcw, const unsigned char* src, int srcstep, int cn, int* me ) {
  int * it = me;
  int i, yh = srch + 1, yw = srcw + 1;
  int ysize = yh * yw;
  int* img2 = MALLOC( int, ysize );
  int exx = ( 2 * FN + 1 ) * 3 + 1;
  int yl = yw + exx;
  int aah = srch + exx, aaw = srcw + exx;
  int aal = aaw;
  unsigned char* AA = MALLOC( unsigned char, ( aah ) * ( aal ) );
  INTINT* y1 = MALLOC( INTINT, ( yh + exx ) * ( yl ) );
  int exx2 = ( exx / 2 );
  INTINT* y = y1 + exx2 * yl + exx2;
  int u[ 16 ];
  //Mati& me = *this;
  MEMSET( AA, 0, ( aah ) * ( aal ) );
  MEMSET( y1, 0, ( yh + exx ) * ( yl ) );
  
  {
    //PRINT(srch, srcw, src, srcstep, 1, ".\\src.txt", "%d\n");
    COPY_REPLICATE_BORDER( srch, srcw, src, srcstep, aah, aaw, AA, aal, 1, exx2, exx2 );
    //PRINT(aah, aaw, Asrc, aal, 1, ".\\Asrc.txt", "%d\n");
    //SKIP_COPY(srch, srcw, src, srcstep, 1, Asrc, aal, 1);
    //SKIP_COPYEX(srch, srcw, src, srcstep, 1, exx, exx, Asrc, aal, 1);
    SKIP_INTEGRAL( aah, aaw, AA, aal, 1, y1, yl, 1 );
  }
  for ( i = 0; i < FN; ++i ) {
    int ex = ( 2 * i + 1 ) * 3 + 1;
    int bin = lbp_Feature_List[ i ].bin;
    int len = lbp_Feature_List[ i ].len;
    int* ptr = lbp_Feature_List[ i ].ptr;
    int eex = ex * 1;
    const unsigned char* __lbpmap0 = lbpmap[ i ];
    lbp_ex_init( i, yl, 1, u );
    MEMSET( img2, 0, ysize );
    //SKIP_FILTEREX(srch, srcw, y, yl, 1, img2, yw, 1, eex, eex, LBP_EX_FT, int);
    SKIP_FILTER( srch, srcw, y, yl, 1, img2, yw, 1, LBP_EX_FT );
    SKIP_BLOCK_HIST( srch, srcw, img2, yw, 1, it, bin, ptr, len );
    it += bin * len;
  }
  free( AA );
  free( y1 );
  free( img2 );
  return 0;
}

// 加特征选择的多bin的lbp滤波，只用于100×100的图片
int lbp_ex_ft( int srch, int srcw, const unsigned char* src, int srcstep, int cn, real* featptr, int isall ) {
  int i, yh = srch + 1, yw = srcw + 1;
  int all_size = init_size();
  int _size = isall ? all_size : ( sizeof( lbp_Feature_idyw ) / sizeof( int ) );
  if ( featptr ) {
    int * me = MALLOC( int, all_size );
    MEMSET( me, 0, all_size );
    lbp_ex_ft_all( srch, srcw, src, srcstep, cn, me );
    if ( isall ) {
      for ( i = 0; i < all_size; ++i ) {
        featptr[ i ] = ( real ) me[ i ];
      }
    } else {
      for ( i = 0; i < _size; ++i ) {
        int idx = lbp_Feature_idyw[ i ];
        featptr[ i ] = ( real ) me[ idx ];
      }
    }
    free( me );
  }
  return _size;
}
