
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "matmul.h"
#include "matrix.h"
#include "macro.h"

/****************************************************************************************\
*                                    Trace of the matrix                                 *
\****************************************************************************************/ 
/*
返回矩阵的迹
mat 输入矩阵
函数 cvTrace 返回矩阵mat的对角线元素的和。
tr(src) = ∑ mat(i,i)
i
*/
real
cvTrace( int size_height, int size_width, const real* data, int step ) {
  real sum = 0.f;
  int len = MIN( size_width, size_height );
  int step1 = step + 1;
  //memset(data, 0, size);
  SKIP_SUM1D( len, data, step1, sum );
  return sum;
}

/* Calculates covariation matrix for a set of vectors */
/* transpose([v1-avg, v2-avg,...]) * [v1-avg,v2-avg,...] */
#define CV_COVAR_SCRAMBLED 0

/* [v1-avg, v2-avg,...] * transpose([v1-avg,v2-avg,...]) */
#define CV_COVAR_NORMAL    1

/* do not calc average (i.e. mean vector) - use the input vector instead
   (useful for calculating covariance matrix by parts) */
#define CV_COVAR_USE_AVG   2

/* scale the covariance matrix coefficients by number of the vectors */
#define CV_COVAR_SCALE     4

/* all the input vectors are stored in a single matrix, as its rows */
#define CV_COVAR_ROWS      8

/* all the input vectors are stored in a single matrix, as its columns */
#define CV_COVAR_COLS     16

#define CV_PCA_DATA_AS_ROW 0
#define CV_PCA_DATA_AS_COL 1
#define CV_PCA_USE_AVG 2

/****************************************************************************************\
*                              LU decomposition/back substitution                        *
\****************************************************************************************/

typedef real arrtype;

//#define ICV_DEF_LU_DECOMP_FUNC(flavor, arrtype)
// A[n*n]  B[n*m]
real
icvLUDecomp( int n, int m, real* A, int stepA, real* B, int stepB ) {
  typedef double temptype;
  int i;
  real det = 1;
  
  for ( i = 0; i < n; i++, A += stepA, B += stepB ) {
    int j, k = i;
    real* tA = A;
    arrtype* tB = 0;
    real kval = FABS( A[ i ] ), tval;
    
    /* find the pivot element */
    for ( j = i + 1; j < n; j++ ) {
      tA += stepA;
      tval = FABS( tA[ i ] );
      
      if ( tval > kval ) {
        kval = tval;
        k = j;
      }
    }
    
    if ( kval == 0 ) {
      det = 0;
      break;
    }
    
    /* swap rows */
    if ( k != i ) {
      tA = A + stepA * ( k - i );
      det = -det;
      
      for ( j = i; j < n; j++ ) {
        real t;
        CV_SWAP( A[ j ], tA[ j ], t );
      }
      
      if ( m > 0 ) {
        tB = B + stepB * ( k - i );
        
        for ( j = 0; j < m; j++ ) {
          arrtype t = B[ j ];
          CV_SWAP( B[ j ], tB[ j ], t );
        }
      }
    }
    
    tval = 1.f / A[ i ];
    det *= A[ i ];
    tA = A;
    tB = B;
    A[ i ] = tval; /* to replace division with multiplication in LUBack */
    
    /* update matrix and the right side of the system */
    for ( j = i + 1; j < n; j++ ) {
      real alpha;
      tA += stepA;
      tB += stepB;
      alpha = -tA[ i ] * tval;
      
      for ( k = i + 1; k < n; k++ )
        tA[ k ] = tA[ k ] + alpha * A[ k ];
        
      if ( m > 0 )
        for ( k = 0; k < m; k++ )
          tB[ k ] = ( arrtype ) ( tB[ k ] + alpha * B[ k ] );
    }
  }
  
  return det;
}

//#define ICV_DEF_LU_BACK_FUNC(flavor, arrtype)
// A[n*n] B[n*M]
int
icvLUBack( int n, int m, real* A, int stepA, arrtype* B, int stepB ) {
  typedef real temptype;
  int i;
  
  A += stepA * ( n - 1 );
  B += stepB * ( n - 1 );
  
  for ( i = n - 1; i >= 0; i--, A -= stepA ) {
    int j, k;
    for ( j = 0; j < m; j++ ) {
      arrtype* tB = B + j;
      double x = 0;
      
      for ( k = n - 1; k > i; k--, tB -= stepB )
        x += A[ k ] * tB[ 0 ];
        
      tB[ 0 ] = ( arrtype ) ( ( tB[ 0 ] - x ) * A[ i ] );
    }
  }
  
  return 0;
}


/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   (m(0,0)*m(1,1) - m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*(m(1,1)*m(2,2) - m(1,2)*m(2,1)) -  \
                   m(0,1)*(m(1,0)*m(2,2) - m(1,2)*m(2,0)) +  \
                   m(0,2)*(m(1,0)*m(2,1) - m(1,1)*m(2,0)))

/*
返回方阵 mat 的行列式值
mat 输入矩阵 
对小矩阵直接计算，对大矩阵用高斯(GAUSSIAN)消去法。
对于对称正定(positive-determined)矩阵也可以用 SVD 函数来求，U=V=NULL ，
然后用 w 的对角线元素的乘积来计算行列式。 
*/ 
// The matrix must be square
double
cvDet( int matwidth, const real* mat, int matstep ) {
  real result = 0;
  real* buffer = 0;
  int local_alloc = 0;
  
#define Mf(y, x) ((m + y*step))[x]
  
  if ( matwidth == 2 ) {
    const real * m = mat;
    int step = matstep;
    
    result = det2( Mf );
  } else if ( matwidth == 3 ) {
    const real * m = mat;
    int step = matstep;
    
    result = det3( Mf );
  } else if ( matwidth == 1 ) {
    result = mat[ 0 ];
  } else {
    int buf_size = matwidth * matwidth;
    
    if ( matwidth <= CV_MAX_LOCAL_MAT_SIZE ) {
      buffer = ( real* ) cvStackAlloc( buf_size * sizeof( real ) );
      local_alloc = 1;
    } else {
      buffer = ( real* ) cvAlloc( buf_size * sizeof( real ) );
    }
    
    MEMCPY2D( matwidth, matwidth, mat, matstep, buffer, matwidth );
    
    result = icvLUDecomp( matwidth, matwidth, buffer, matwidth, 0, 0 );
  }
  
#undef Mf
  
  if ( buffer && !local_alloc )
    cvFree( &buffer );
    
  return result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of the matrix                     *
\****************************************************************************************/

#define Sf(y, x) ((srcdata + y*srcstep))[x]
#define Df(y, x) ((dstdata + y*dststep))[x]

/*
查找矩阵的逆矩阵或伪逆矩阵
#define cvInv cvInvert
src 输入矩阵
dst 目标矩阵
method 求逆方法:
CV_LU  - 最佳主元选取的高斯消除法
CV_SVD - 奇异值分解法 (SVD)
CV_SVD_SYM - 正定对称矩阵的 SVD 方法
函数 cvInvert 对矩阵 src 求逆并将结果存储到 dst。
如果是 LU 方法该函数返回 src 的行列式值 (src 必须是方阵)。
如果是 0, 矩阵不求逆， dst 用 0 填充。
如果 SVD 方法该函数返回 src 的条件数的倒数(最小奇异值和最大奇异值的比值) ，
如果 src 全为 0 则返回0。 如果 src 是奇异的， SVD 方法计算一个伪逆矩阵。
*/
real
cvInvert( int srcrows, int srccols, const real* srcarr, int srcstep,
          real* dstarr, int dststep, int method ) {
  real * u = 0;
  real* v = 0;
  real* w = 0;
  
  real* buffer = 0;
  int local_alloc = 0;
  double result = 0;
  
  const real *src = srcarr;
  real *dst = dstarr;
  
  if ( method == CV_SVD || method == CV_SVD_SYM ) {
    int n = MIN( srcrows, srccols );
    if ( method == CV_SVD_SYM && srcrows != srccols )
      CV_ERROR( CV_StsBadSize, "CV_SVD_SYM method is used for non-square matrix" );
      
    u = MALLOC( real, n * srcrows );
    if ( method != CV_SVD_SYM )
      v = MALLOC( real, n * srccols );
    w = MALLOC( real, n );
    cvSVD( srcrows, srccols, ( real* ) src, srcstep, w, 1, srcrows, u, srcrows, v, srccols, CV_SVD_U_T + CV_SVD_V_T );
    
    result = w[ 0 ] >= FLT_EPSILON ? w[ n - 1 ] / w[ 0 ] : 0;
    
    cvSVBkSb( w, 1, u, srcrows, v ? v : u, v ? srccols : srcrows, 0, 0, 0, srcrows, srccols, dst, dststep, CV_SVD_U_T + CV_SVD_V_T );
    goto exit;
  } else if ( method != CV_LU ) {
    CV_ERROR( CV_StsBadArg, "Unknown inversion method" );
  }
  
  if ( srccols != srcrows )
    CV_ERROR( CV_StsBadSize, "The matrix must be square" );
    
  if ( srccols <= 3 ) {
    const real * srcdata = src;
    real* dstdata = dst;
    
    if ( srccols == 2 ) {
      double d = det2( Sf );
      if ( d != 0. ) {
        double t0, t1;
        result = d;
        d = 1. / d;
        t0 = Sf( 0, 0 ) * d;
        t1 = Sf( 1, 1 ) * d;
        Df( 1, 1 ) = ( float ) t0;
        Df( 0, 0 ) = ( float ) t1;
        t0 = -Sf( 0, 1 ) * d;
        t1 = -Sf( 1, 0 ) * d;
        Df( 0, 1 ) = ( float ) t0;
        Df( 1, 0 ) = ( float ) t1;
      }
    } else if ( srccols == 3 ) {
      double d = det3( Sf );
      if ( d != 0. ) {
        float t[ 9 ];
        result = d;
        d = 1. / d;
        
        t[ 0 ] = ( float ) ( ( Sf( 1, 1 ) * Sf( 2, 2 ) - Sf( 1, 2 ) * Sf( 2, 1 ) ) * d );
        t[ 1 ] = ( float ) ( ( Sf( 0, 2 ) * Sf( 2, 1 ) - Sf( 0, 1 ) * Sf( 2, 2 ) ) * d );
        t[ 2 ] = ( float ) ( ( Sf( 0, 1 ) * Sf( 1, 2 ) - Sf( 0, 2 ) * Sf( 1, 1 ) ) * d );
        
        t[ 3 ] = ( float ) ( ( Sf( 1, 2 ) * Sf( 2, 0 ) - Sf( 1, 0 ) * Sf( 2, 2 ) ) * d );
        t[ 4 ] = ( float ) ( ( Sf( 0, 0 ) * Sf( 2, 2 ) - Sf( 0, 2 ) * Sf( 2, 0 ) ) * d );
        t[ 5 ] = ( float ) ( ( Sf( 0, 2 ) * Sf( 1, 0 ) - Sf( 0, 0 ) * Sf( 1, 2 ) ) * d );
        
        t[ 6 ] = ( float ) ( ( Sf( 1, 0 ) * Sf( 2, 1 ) - Sf( 1, 1 ) * Sf( 2, 0 ) ) * d );
        t[ 7 ] = ( float ) ( ( Sf( 0, 1 ) * Sf( 2, 0 ) - Sf( 0, 0 ) * Sf( 2, 1 ) ) * d );
        t[ 8 ] = ( float ) ( ( Sf( 0, 0 ) * Sf( 1, 1 ) - Sf( 0, 1 ) * Sf( 1, 0 ) ) * d );
        
        Df( 0, 0 ) = t[ 0 ];
        Df( 0, 1 ) = t[ 1 ];
        Df( 0, 2 ) = t[ 2 ];
        Df( 1, 0 ) = t[ 3 ];
        Df( 1, 1 ) = t[ 4 ];
        Df( 1, 2 ) = t[ 5 ];
        Df( 2, 0 ) = t[ 6 ];
        Df( 2, 1 ) = t[ 7 ];
        Df( 2, 2 ) = t[ 8 ];
      }
    } else {
      double d = Sf( 0, 0 );
      assert( srccols == 1 );
      if ( d != 0. ) {
        result = d;
        Df( 0, 0 ) = ( float ) ( 1. / d );
      }
    }
  } else {
    int buf_size = srccols * srccols;
    
    if ( srccols <= CV_MAX_LOCAL_MAT_SIZE ) {
      buffer = ( real* ) cvStackAlloc( buf_size * sizeof( real ) );
      local_alloc = 1;
    } else {
      buffer = ( real* ) cvAlloc( buf_size * sizeof( real ) );
    }
    
    MEMCPY2D( srccols, srccols, src, srcstep, buffer, srccols );
    cvSetIdentity( srccols, srccols, dst, dststep, 1.f );
    
    result = icvLUDecomp( srccols, srccols, buffer, srccols, dst, dststep );
    
    if ( result != 0 ) {
      icvLUBack( srccols, srccols, buffer, srccols, dst, dststep );
    }
  }
  
  if ( !result )
    SKIP_FILL( srccols, srccols, dst, dststep, 1, 0.f );
    
exit:

  if ( buffer && !local_alloc )
    cvFree( &buffer );
    
  if ( u || v || w ) {
    free( u );
    free( v );
    free( w );
  }
  
  return ( real ) result;
}

/****************************************************************************************\
*                               Linear system [least-squares] solution                   *
\****************************************************************************************/

/*
求解线性系统或者最小二乘法问题 
src1 输入矩阵 
src2 线性系统的右部 
dst 输出解答 
method 解决方法(矩阵求逆) : 
CV_LU  - 最佳主元选取的高斯消除法 
CV_SVD - 奇异值分解法 (SVD) 
CV_SVD_SYM - 对正定对称矩阵的 SVD 方法 
函数 cvSolve 解决线性系统或者最小二乘法问题 (后者用 SVD 方法可以解决): 
dst = argmin(x)|src*x - src2|
如果使用 CV_LU 方法。 如果 src1 是非奇异的，该函数则返回 1 ，否则返回 0 ，在后一种情况下 dst 是无效的。 
*/
int
cvSolve( int srcrows, int srccols, const real* A, int srcstep,
         int bcols, const real* b, int bstep, real* x, int xstep, int method ) {
  real * u = 0;
  real* v = 0;
  real* w = 0;
  
  real* buffer = 0;
  int local_alloc = 0;
  int result = 1;
  
  const real *src = A;
  const real *src2 = b;
  real *dst = x;
  
  if ( method == CV_SVD || method == CV_SVD_SYM ) {
    int n = MIN( srcrows, srccols );
    
    if ( method == CV_SVD_SYM && srcrows != srccols )
      CV_ERROR( CV_StsBadSize, "CV_SVD_SYM method is used for non-square matrix" );
      
    u = MALLOC( real, n * srcrows );
    if ( method != CV_SVD_SYM )
      v = MALLOC( real, n * srccols );
    w = MALLOC( real, n );
    cvSVD( srcrows, srccols, ( real* ) src, srcstep, w, 1, srcrows, u, srcrows, v, srccols, CV_SVD_U_T + CV_SVD_V_T );
    cvSVBkSb( w, 1, u, srcrows, v ? v : u, v ? srccols : srcrows, bcols, src2, bstep, srcrows, srccols, dst, xstep, CV_SVD_U_T + CV_SVD_V_T );
    goto exit;
  } else if ( method != CV_LU )
    CV_ERROR( CV_StsBadArg, "Unknown inversion method" );
    
  if ( srccols != srcrows )
    CV_ERROR( CV_StsBadSize, "The matrix must be square" );
    
  // check case of a single equation and small matrix
  if ( srccols <= 3 && bcols == 1 ) {
#define bf(y) ((bdata + y*src2step))[0]
  
    const real * srcdata = src;
    const real* bdata = src2;
    real* dstdata = dst;
    int src2step = bstep;
    int dststep = xstep;
    
    if ( srccols == 2 ) {
      double d = det2( Sf );
      if ( d != 0. ) {
        float t;
        d = 1. / d;
        t = ( float ) ( ( bf( 0 ) * Sf( 1, 1 ) - bf( 1 ) * Sf( 0, 1 ) ) * d );
        Df( 1, 0 ) = ( float ) ( ( bf( 1 ) * Sf( 0, 0 ) - bf( 0 ) * Sf( 1, 0 ) ) * d );
        Df( 0, 0 ) = t;
      } else
        result = 0;
    } else if ( srccols == 3 ) {
      double d = det3( Sf );
      if ( d != 0. ) {
        float t[ 3 ];
        d = 1.f / d;
        
        t[ 0 ] = ( float ) ( d *
                             ( bf( 0 ) * ( Sf( 1, 1 ) * Sf( 2, 2 ) - Sf( 1, 2 ) * Sf( 2, 1 ) ) -
                               Sf( 0, 1 ) * ( bf( 1 ) * Sf( 2, 2 ) - Sf( 1, 2 ) * bf( 2 ) ) +
                               Sf( 0, 2 ) * ( bf( 1 ) * Sf( 2, 1 ) - Sf( 1, 1 ) * bf( 2 ) ) ) );
                               
        t[ 1 ] = ( float ) ( d *
                             ( Sf( 0, 0 ) * ( bf( 1 ) * Sf( 2, 2 ) - Sf( 1, 2 ) * bf( 2 ) ) -
                               bf( 0 ) * ( Sf( 1, 0 ) * Sf( 2, 2 ) - Sf( 1, 2 ) * Sf( 2, 0 ) ) +
                               Sf( 0, 2 ) * ( Sf( 1, 0 ) * bf( 2 ) - bf( 1 ) * Sf( 2, 0 ) ) ) );
                               
        t[ 2 ] = ( float ) ( d *
                             ( Sf( 0, 0 ) * ( Sf( 1, 1 ) * bf( 2 ) - bf( 1 ) * Sf( 2, 1 ) ) -
                               Sf( 0, 1 ) * ( Sf( 1, 0 ) * bf( 2 ) - bf( 1 ) * Sf( 2, 0 ) ) +
                               bf( 0 ) * ( Sf( 1, 0 ) * Sf( 2, 1 ) - Sf( 1, 1 ) * Sf( 2, 0 ) ) ) );
                               
        Df( 0, 0 ) = t[ 0 ];
        Df( 1, 0 ) = t[ 1 ];
        Df( 2, 0 ) = t[ 2 ];
      } else
        result = 0;
    } else {
      double d = Sf( 0, 0 );
      assert( srccols == 1 );
      if ( d != 0. )
        Df( 0, 0 ) = ( float ) ( bf( 0 ) / d );
      else
        result = 0;
    }
  } else {
    int buf_size = srccols * srccols;
    double d = 0;
    
    if ( srccols <= CV_MAX_LOCAL_MAT_SIZE ) {
      buffer = ( real* ) cvStackAlloc( buf_size * sizeof( real ) );
      local_alloc = 1;
    } else {
      buffer = ( real* ) cvAlloc( buf_size * sizeof( real ) );
    }
    
    MEMCPY2D( srccols, srccols, src, srcstep, buffer, srccols );
    
    if ( src2 != dst ) {
      //cvCopy(src2, dst);
      MEMCPY2D( srccols, srccols, src2, bstep, dst, xstep );
    }
    
    d = icvLUDecomp( srccols, srccols, buffer, srccols, dst, xstep );
    
    if ( d != 0 ) {
      icvLUBack( srccols, srccols, buffer, srccols, dst, xstep );
    } else
      result = 0;
  }
  
  if ( !result ) {
    //CV_CALL(cvSetZero(dst));
    SKIP_FILL( srccols, srccols, dst, xstep, 1, 0.f );
  }
  
exit:

  if ( buffer && !local_alloc )
    cvFree( &buffer );
    
  if ( u || v || w ) {
    free( u );
    free( v );
    free( w );
  }
  
  return result;
}

/****************************************************************************************\
*                               3D vector cross-product                                  *
\****************************************************************************************/ 
/*
计算两个三维向量的叉积 
src1 第一输入向量。 
src2 第二输入向量。 
dst 输出向量 
函数 cvCrossProduct 计算两个三维向量的差积： 
dst = src1 × src2, (dst1 = src12src23 - src13src22 , dst2 = src13src21 - src11src23 , dst3 = src11src22 - src12src21).
*/
void
cvCrossProduct( const real* srcA, int srcAstep, const real* srcB, int srcBstep, real* dst, int dststep ) {
  real * dstdata = ( dst );
  const real* src1data = ( srcA );
  const real* src2data = ( srcB );
  
  if ( 1 == srcAstep || 1 == srcBstep || 1 == dststep ) {
    dstdata[ 2 ] = src1data[ 0 ] * src2data[ 1 ] - src1data[ 1 ] * src2data[ 0 ];
    dstdata[ 0 ] = src1data[ 1 ] * src2data[ 2 ] - src1data[ 2 ] * src2data[ 1 ];
    dstdata[ 1 ] = src1data[ 2 ] * src2data[ 0 ] - src1data[ 0 ] * src2data[ 2 ];
  } else {
    int step1 = srcAstep ? srcAstep : 1;
    int step2 = srcBstep ? srcBstep : 1;
    int step = dststep ? dststep : 1;
    
    dstdata[ 2 * step ] = src1data[ 0 ] * src2data[ step2 ] - src1data[ step1 ] * src2data[ 0 ];
    dstdata[ 0 ] = src1data[ step1 ] * src2data[ step2 * 2 ] - src1data[ step1 * 2 ] * src2data[ step2 ];
    dstdata[ step ] = src1data[ step1 * 2 ] * src2data[ 0 ] - src1data[ 0 ] * src2data[ step2 * 2 ];
  }
}

/*
对一个向量集做PCA变换 
data 输入数据,每个向量是单行向量(CV_PCA_DATA_AS_ROW)或者单列向量(CV_PCA_DATA_AS_COL). 
avg 平均向量,在函数内部计算或者由调用者提供 
eigenvalues 输出的协方差矩阵的特征值 
eigenvectors 输出的协方差矩阵的特征向量(也就是主分量),每个向量一行 
flags 操作标志,可以是以下几种方式的组合: 
CV_PCA_DATA_AS_ROW - 向量以行的方式存放(也就是说任何一个向量都是连续存放的) 
CV_PCA_DATA_AS_COL - 向量以列的方式存放(也就是说某一个向量成分的数值是连续存放的) 
(上面两种标志是互相排斥的) 
CV_PCA_USE_AVG - 使用预先计算好的平均值 
该函数对某个向量集做PCA变换.它首先利用cvCalcCovarMatrix计算协方差矩阵然后计算协方差矩阵的特征值与特征向量.输出的特征值/特征向量的个数小于或者等于MIN(rows(data),cols(data)). 
*/
void
cvCalcPCA( int data_rows, int data_cols, const real* data_arr, int datastep,
           real* avg_arr, real* eigenvals, real* eigenvects, int eigenvectsstep, int flags, int out_count ) {
  real * tmp_avg = 0;
  real* tmp_avg_r = 0;
  real* tmp_cov = 0;
  real* tmp_evals = 0;
  real* tmp_evects = 0;
  real* tmp_evects2 = 0;
  real* tmp_data = 0;
  
  const real *data = data_arr;
  real *avg = avg_arr;
  real *evals = eigenvals;
  real *evects = eigenvects;
  int covar_flags = CV_COVAR_SCALE;
  int i, len, in_count, count;
  
  if ( flags & CV_PCA_DATA_AS_COL ) {
    len = data_rows;
    in_count = data_cols;
    covar_flags |= CV_COVAR_COLS;
  } else {
    len = data_cols;
    in_count = data_rows;
    covar_flags |= CV_COVAR_ROWS;
  }
  tmp_avg = MALLOC( real, len );
  
  count = MIN( len, in_count );
  //out_count = evalscols + evalsrows - 1;
  
  // "scrambled" way to compute PCA (when cols(A)>rows(A)):
  // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
  if ( len <= in_count )
    covar_flags |= CV_COVAR_NORMAL;
    
  if ( flags & CV_PCA_USE_AVG ) {
    covar_flags |= CV_COVAR_USE_AVG;
    // CV_CALL(cvConvert(avg, tmp_avg));
    MEMCPY1D( len, avg, tmp_avg );
  }
  
  tmp_cov = MALLOC( real, count * count );
  tmp_evals = MALLOC( real, count );
  tmp_evects = MALLOC( real, count * count );
  
  //CV_CALL(cvCalcCovarMatrix(&data_arr, 0, tmp_cov, tmp_avg, covar_flags));
  cvCalcCovarMatrix( data_rows, data_cols, data_arr, datastep,
                     tmp_cov, count, tmp_avg, covar_flags );
  cvSVD( count, count, tmp_cov, count, tmp_evals, 1, count, tmp_evects, count, 0, 0, CV_SVD_MODIFY_A + CV_SVD_U_T );
  //cvZero(evects);
  //cvZero(evals);
  SKIP_FILL( count, count, evects, count, 1, 0.f );
  FILL( count, evals, 0.f );
  
  if ( covar_flags & CV_COVAR_NORMAL ) {
    //cvConvert(tmp_evects, evects);
    MEMCPY2D( count, count, tmp_evects, count, evects, eigenvectsstep );
  } else {
    // CV_PCA_DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
    // CV_PCA_DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
    int block_count = 0;
    
    tmp_data = MALLOC( real, count * count );
    tmp_avg_r = MALLOC( real, count * count );
    tmp_evects2 = MALLOC( real, count * count );
    
    for ( i = 0; i < len; i += block_count ) {
      const real * data_part;
      real *tdata_part, *part, *dst_part, *avg_part, *tmp_avg_part;
      int gemm_flags, partrows, partcols;
      
      block_count = MIN( count, len - i );
      
#define cvGetRows(a, b, c)  *(b) = (a) + (c)
      
      if ( flags & CV_PCA_DATA_AS_COL ) {
        data_part = data + i * datastep;
        tdata_part = tmp_data;
        avg_part = tmp_avg + i;
        tmp_avg_part = tmp_avg_r + i * count;
        gemm_flags = CV_GEMM_B_T;
        partrows = count;
        partcols = block_count;
      } else {
        data_part = data + i;
        tdata_part = tmp_data;
        avg_part = tmp_avg + i;
        tmp_avg_part = tmp_avg_r + i;
        gemm_flags = 0;
        partrows = block_count;
        partcols = count;
      }
      part = tmp_evects2;
      dst_part = evects + i;
      
      MEMCPY2D( partrows, partcols, data_part, datastep, tdata_part, count );
      //REPEAT2D(1, block_count, avg_part, 1, partrows, partcols, tmp_avg_part, count);
      //cvRepeat(&avg_part, &tmp_avg_part);
      //cvSub(&tdata_part, &tmp_avg_part, &tdata_part);
      FOR_EACH2D_OP( partrows, partcols, tdata_part, count, tdata_part, count, tmp_avg_part, count, - );
      cvGEMM( partrows, count, partcols, tmp_evects, count, tdata_part, count,
              1, 0, 0, 0, part, count, gemm_flags );
      MEMCPY2D( partrows, partcols, part, count, dst_part, count );
    }
    
    // normalize eigenvectors
    for ( i = 0; i < count; i++ ) {
      real* ei = evects + i * eigenvectsstep;
      //cvNormalize(ei);
    }
  }
  
  //if(tmp_evalsrows != evalsrows)    cvReshape(tmp_evals, tmp_evals, 1, evalsrows);
  //cvConvert(tmp_evals, evals);
  MEMCPY1D( count, tmp_evals, evals );
  MEMCPY1D( len, tmp_avg, avg );
  
  SAFEFREE( tmp_avg );
  SAFEFREE( tmp_avg_r );
  SAFEFREE( tmp_cov );
  SAFEFREE( tmp_evals );
  SAFEFREE( tmp_evects );
  SAFEFREE( tmp_evects2 );
  SAFEFREE( tmp_data );
}

#if 0 
/*
把向量向某个子空间投影 
data 输入数据,每个向量可以是单行或者单列 
avg 平均向量.要么它是单行向量那么意味着输入数据以行数据的形式存放,
要么就是单列向量,那么就意味着那么输入向量就是以列的方式存放. 
eigenvectors 特征向量(主分量),每个向量一行. 
result 输出的分解系数矩阵,矩阵的行数必须与输入向量的个数相等,矩阵的列数必须小于特征向量的行数. 
该函数将输入向量向一个正交系(eigenvectors)投影.在计算点乘之前,输入向量要减去平均向量: 
 
result(i,:)=(data(i,:)-avg)*eigenvectors' // for CV_PCA_DATA_AS_ROW layout. 
*/
void
cvProjectPCA( int data_rows, int data_cols, const real* data_arr, int data_step,
              int avglen, const real* avg_arr,
              int evectsrows, int evectscols, const real* eigenvects, int eigenvects_step,
              int resultcols, real* result_arr, int result_step ) {
  real * buffer = 0;
  int local_alloc = 0;
  
  const real *data = data_arr;
  const real *avg = avg_arr;
  const real *evects = eigenvects;
  real *result = result_arr;
  real *avg_repeated;
  int i, len, in_count;
  int gemm_flags, as_cols;
  int block_count0, block_count, buf_size;
  real* tmp_data_ptr;
  
  if ( ( avglen != data_rows ) && ( avglen != data_cols ) )
    CV_ERROR( CV_StsBadSize,
              "The mean (average) vector should be either 1 x data->cols or data->rows x 1" );
              
  if ( avglen == data_rows ) {
    len = data_rows;
    in_count = data_cols;
    gemm_flags = CV_GEMM_A_T + CV_GEMM_B_T;
    as_cols = 1;
  } else {
    len = data_cols;
    in_count = data_rows;
    
    gemm_flags = CV_GEMM_B_T;
    as_cols = 0;
  }
  
  if ( evectscols != len )
    CV_ERROR( CV_StsUnmatchedSizes,
              "Eigenvectors must be stored as rows and be of the same size as input vectors" );
              
  if ( resultcols > evectsrows )
    CV_ERROR( CV_StsOutOfRange,
              "The output matrix of coefficients must have the number of columns "
              "less than or equal to the number of eigenvectors (number of rows in eigenvectors matrix)" );
              
  //evects = cvGetRows(evects, &evectstub, 0, resultcols);
  
  block_count0 = ( 1 << 16 ) / len;
  block_count0 = MAX( block_count0, 4 );
  block_count0 = MIN( block_count0, in_count );
  
  buf_size = block_count0 * len * ( ( block_count0 > 1 ) + 1 );
  
  if ( buf_size < CV_MAX_LOCAL_SIZE ) {
    buffer = ( real* ) cvStackAlloc( buf_size * sizeof( real ) );
    local_alloc = 1;
  } else
    CV_CALL( buffer = ( real* ) cvAlloc( buf_size * sizeof( real ) ) );
    
  tmp_data_ptr = buffer;
  if ( block_count0 > 1 ) {
    int avg_r_rows = as_cols ? len : block_count0;
    int avg_r_cols = as_cols ? block_count0 : len;
    avg_repeated = buffer;
    //cvRepeat(avg, &avg_repeated);
    //REPEAT2D(1, len, avg, 1, avg_r_rows, avg_r_cols, avg_repeated, len);
    tmp_data_ptr += block_count0 * len;
  } else
    avg_repeated = ( real* ) avg;
    
  for ( i = 0; i < in_count; i += block_count ) {
    real * data_part, *norm_data, *avg_part, *out_part;
    int norm_rows = as_cols ? len : block_count;
    int norm_cols = as_cols ? block_count : len;
    block_count = MIN( block_count0, in_count - i );
    norm_data = tmp_data_ptr;
    out_part, avg_part, data_part;
    //cvSub(data_part, &avg_part, &norm_data);
    
    //out_part = result + i*resultstep;
    // cvGEMM(&norm_data, evects, 1, 0, 0, &out_part, gemm_flags);
    //MAT_AXB();
  }
  
  if ( !local_alloc )
    cvFree( &buffer );
}


/*
根据投影系数重构原来的向量
proj 输入数据,与cvProjectPCA里面的格式一致 
avg  平均向量.如果它是单行向量,那么意味着输出向量是以行的方式存放.
否则就是单列向量,那么输出向量就是以列的方式存放. 
eigenvectors 特征向量(主分量),每个向量一行. 
result 输出的重构出来的矩阵 
该函数根据投影系数重构原来的向量: 
 
result(i,:)=proj(i,:)*eigenvectors + avg // for CV_PCA_DATA_AS_ROW layout. 
*/
void
cvBackProjectPCA( const CvArr* proj_arr, const CvArr* avg_arr,
                  const CvArr* eigenvects, CvArr* result_arr ) {
  unsigned char * buffer = 0;
  int local_alloc = 0;
  
  CV_FUNCNAME( "cvProjectPCA" );
  
  __BEGIN__;
  
  real pstub, *data = ( real* ) proj_arr;
  real astub, *avg = ( real* ) avg_arr;
  real evectstub, *evects = ( real* ) eigenvects;
  real rstub, *result = ( real* ) result_arr;
  real avg_repeated;
  int i, len, in_count, as_cols;
  int block_count0, block_count, buf_size, elem_size;
  
  if ( !CV_IS_MAT( data ) )
    CV_CALL( data = cvGetMat( data, &pstub ) );
    
  if ( !CV_IS_MAT( avg ) )
    CV_CALL( avg = cvGetMat( avg, &astub ) );
    
  if ( !CV_IS_MAT( evects ) )
    CV_CALL( evects = cvGetMat( evects, &evectstub ) );
    
  if ( !CV_IS_MAT( result ) )
    CV_CALL( result = cvGetMat( result, &rstub ) );
    
  if ( CV_MAT_TYPE( avg->type ) != CV_32FC1 && CV_MAT_TYPE( avg->type ) != CV_64FC1 ||
       !CV_ARE_TYPES_EQ( avg, data ) || !CV_ARE_TYPES_EQ( avg, evects ) || !CV_ARE_TYPES_EQ( avg, result ) )
    CV_ERROR( CV_StsUnsupportedFormat,
              "All the input and output arrays must have the same type, 32fC1 or 64fC1" );
              
  if ( ( avgcols != 1 || avgrows != resultrows ) &&
       ( avgrows != 1 || avgcols != resultcols ) )
    CV_ERROR( CV_StsBadSize,
              "The mean (average) vector should be either 1 x resultcols or resultrows x 1" );
              
  if ( avgcols == 1 ) {
    len = resultrows;
    in_count = resultcols;
    as_cols = 1;
  } else {
    len = resultcols;
    in_count = resultrows;
    as_cols = 0;
  }
  
  if ( evectscols != len )
    CV_ERROR( CV_StsUnmatchedSizes,
              "Eigenvectors must be stored as rows and be of the same size as the output vectors" );
              
  if ( data_cols > evectsrows )
    CV_ERROR( CV_StsOutOfRange,
              "The input matrix of coefficients must have the number of columns "
              "less than or equal to the number of eigenvectors (number of rows in eigenvectors matrix)" );
              
  evects = cvGetRows( evects, &evectstub, 0, data_cols );
  
  block_count0 = ( 1 << 16 ) / len;
  block_count0 = MAX( block_count0, 4 );
  block_count0 = MIN( block_count0, in_count );
  elem_size = CV_ELEM_SIZE( avg->type );
  
  buf_size = block_count0 * len * ( block_count0 > 1 ) * elem_size;
  
  if ( buf_size < CV_MAX_LOCAL_SIZE ) {
    buffer = ( unsigned char* ) cvStackAlloc( MAX( buf_size, 16 ) );
    local_alloc = 1;
  } else
    CV_CALL( buffer = ( unsigned char* ) cvAlloc( buf_size ) );
    
  if ( block_count0 > 1 ) {
    avg_repeated = real( as_cols ? len : block_count0,
                         as_cols ? block_count0 : len, avg->type, buffer );
    cvRepeat( avg, &avg_repeated );
  } else
    avg_repeated = *avg;
    
  for ( i = 0; i < in_count; i += block_count ) {
    real data_part, avg_part, out_part;
    
    block_count = MIN( block_count0, in_count - i );
    cvGetRows( data, &data_part, i, i + block_count );
    
    if ( as_cols ) {
      cvGetCols( result, &out_part, i, i + block_count );
      cvGetCols( &avg_repeated, &avg_part, 0, block_count );
      cvGEMM( evects, &data_part, 1, &avg_part, 1, &out_part, CV_GEMM_A_T + CV_GEMM_B_T );
    } else {
      cvGetRows( result, &out_part, i, i + block_count );
      cvGetRows( &avg_repeated, &avg_part, 0, block_count );
      cvGEMM( &data_part, evects, 1, &avg_part, 1, &out_part, 0 );
    }
  }
  
  __END__;
  
  if ( !local_alloc )
    cvFree( &buffer );
}
#endif

/* End of file. */

