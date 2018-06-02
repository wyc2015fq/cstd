
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <float.h>
#include <string.h>
#include <math.h>
#include "matmul.h"
#include "macro.h"


typedef unsigned int size_t;

typedef double worktype;
typedef real arrtype;
typedef real srctype;
typedef real dsttype;

/****************************************************************************************\
*                                         cvGEMM                                         *
\****************************************************************************************/

int
cvGEMMSingleMul( int a_height, int a_width, int d_width,
                 const arrtype* a_data, size_t a_step,
                 const arrtype* b_data, size_t b_step,
                 const arrtype* c_data, size_t c_step,
                 arrtype* d_data, size_t d_step,
                 real alpha, real beta, int flags ) {
  int d_height = a_height;
  int i, j, k, n = a_width, m = d_width, drows = d_height;
  const arrtype *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
  arrtype* a_buf = 0;
  size_t a_step0, a_step1, c_step0, c_step1, t_step;
  
  a_step0 = a_step;
  a_step1 = 1;
  
  if ( !c_data )
    c_step0 = c_step1 = 0;
  else if ( !( flags & CV_GEMM_C_T ) )
    c_step0 = c_step, c_step1 = 1;
  else
    c_step0 = 1, c_step1 = c_step;
    
  if ( flags & CV_GEMM_A_T ) {
    CV_SWAP( a_step0, a_step1, t_step );
    n = a_height;
    if ( a_step > 1 && n > 1 )
      a_buf = ( arrtype* ) cvStackAlloc( n * sizeof( a_data[ 0 ] ) );
  }
  
  if ( n == 1 ) { /* external product */
    arrtype * b_buf = 0;
    
    if ( a_step > 1 ) {
      a_buf = ( arrtype* ) cvStackAlloc( drows * sizeof( a_data[ 0 ] ) );
      for ( k = 0; k < drows; k++ )
        a_buf[ k ] = a_data[ a_step * k ];
      a_data = a_buf;
    }
    
    if ( b_step > 1 ) {
      b_buf = ( arrtype* ) cvStackAlloc( d_width * sizeof( b_buf[ 0 ] ) );
      for ( j = 0; j < d_width; j++ )
        b_buf[ j ] = b_data[ j * b_step ];
      b_data = b_buf;
    }
    
    for ( i = 0; i < drows; i++, _c_data += c_step0,
          d_data += d_step ) {
      worktype al = ( worktype ) ( a_data[ i ] ) * alpha;
      c_data = _c_data;
      for ( j = 0; j <= d_width - 2; j += 2, c_data += 2 * c_step1 ) {
        worktype s0 = al * b_data[ j ];
        worktype s1 = al * b_data[ j + 1 ];
        if ( !c_data ) {
          d_data[ j ] = ( arrtype ) ( s0 );
          d_data[ j + 1 ] = ( arrtype ) ( s1 );
        } else {
          d_data[ j ] = ( arrtype ) ( s0 + c_data[ 0 ] * beta );
          d_data[ j + 1 ] = ( arrtype ) ( s1 + c_data[ c_step1 ] * beta );
        }
      }
      
      for ( ; j < d_width; j++, c_data += c_step1 ) {
        worktype s0 = al * b_data[ j ];
        if ( !c_data )
          d_data[ j ] = ( arrtype ) ( s0 );
        else
          d_data[ j ] = ( arrtype ) ( s0 + c_data[ 0 ] * beta );
      }
    }
  } else if ( flags & CV_GEMM_B_T ) { /* A * Bt */
    for ( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step ) {
      a_data = _a_data;
      b_data = _b_data;
      c_data = _c_data;
      
      if ( a_buf ) {
        for ( k = 0; k < n; k++ )
          a_buf[ k ] = a_data[ a_step1 * k ];
        a_data = a_buf;
      }
      
      for ( j = 0; j < d_width; j++, b_data += b_step, c_data += c_step1 ) {
        worktype s0 = ( 0 ), s1 = ( 0 ), s2 = ( 0 ), s3 = ( 0 );
        
        for ( k = 0; k <= n - 4; k += 4 ) {
          s0 += ( worktype ) ( a_data[ k ] ) * b_data[ k ];
          s1 += ( worktype ) ( a_data[ k + 1 ] ) * b_data[ k + 1 ];
          s2 += ( worktype ) ( a_data[ k + 2 ] ) * b_data[ k + 2 ];
          s3 += ( worktype ) ( a_data[ k + 3 ] ) * b_data[ k + 3 ];
        }
        
        for ( ; k < n; k++ )
          s0 += ( worktype ) ( a_data[ k ] ) * b_data[ k ];
        s0 = ( s0 + s1 + s2 + s3 ) * alpha;
        
        if ( !c_data )
          d_data[ j ] = ( arrtype ) ( s0 );
        else
          d_data[ j ] = ( arrtype ) ( s0 + c_data[ 0 ] * beta );
      }
    }
  } else if ( d_width * sizeof( d_data[ 0 ] ) <= 1600 ) {
    for ( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step ) {
      a_data = _a_data, c_data = _c_data;
      
      if ( a_buf ) {
        for ( k = 0; k < n; k++ )
          a_buf[ k ] = a_data[ a_step1 * k ];
        a_data = a_buf;
      }
      
      for ( j = 0; j <= m - 4; j += 4, c_data += 4 * c_step1 ) {
        const arrtype * b = _b_data + j;
        worktype s0 = ( 0 ), s1 = ( 0 ), s2 = ( 0 ), s3 = ( 0 );
        
        for ( k = 0; k < n; k++, b += b_step ) {
          worktype a = ( a_data[ k ] );
          s0 += a * b[ 0 ];
          s1 += a * b[ 1 ];
          s2 += a * b[ 2 ];
          s3 += a * b[ 3 ];
        }
        
        if ( !c_data ) {
          d_data[ j ] = ( arrtype ) ( s0 * alpha );
          d_data[ j + 1 ] = ( arrtype ) ( s1 * alpha );
          d_data[ j + 2 ] = ( arrtype ) ( s2 * alpha );
          d_data[ j + 3 ] = ( arrtype ) ( s3 * alpha );
        } else {
          s0 = s0 * alpha;
          s1 = s1 * alpha;
          s2 = s2 * alpha;
          s3 = s3 * alpha;
          d_data[ j ] = ( arrtype ) ( s0 + c_data[ 0 ] * beta );
          d_data[ j + 1 ] = ( arrtype ) ( s1 + c_data[ c_step1 ] * beta );
          d_data[ j + 2 ] = ( arrtype ) ( s2 + c_data[ c_step1 * 2 ] * beta );
          d_data[ j + 3 ] = ( arrtype ) ( s3 + c_data[ c_step1 * 3 ] * beta );
        }
      }
      
      for ( ; j < m; j++, c_data += c_step1 ) {
        const arrtype * b = _b_data + j;
        worktype s0 = ( 0 );
        
        for ( k = 0; k < n; k++, b += b_step )
          s0 += ( worktype ) ( a_data[ k ] ) * b[ 0 ];
          
        s0 = s0 * alpha;
        if ( !c_data )
          d_data[ j ] = ( arrtype ) ( s0 );
        else
          d_data[ j ] = ( arrtype ) ( s0 + c_data[ 0 ] * beta );
      }
    }
  } else {
    worktype* d_buf = ( worktype* ) cvStackAlloc( m * sizeof( d_buf[ 0 ] ) );
    
    for ( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step ) {
      a_data = _a_data;
      b_data = _b_data;
      c_data = _c_data;
      
      if ( a_buf ) {
        for ( k = 0; k < n; k++ )
          a_buf[ k ] = _a_data[ a_step1 * k ];
        a_data = a_buf;
      }
      
      for ( j = 0; j < m; j++ )
        d_buf[ j ] = ( worktype ) ( 0 );
        
      for ( k = 0; k < n; k++, b_data += b_step ) {
        worktype al = ( a_data[ k ] );
        
        for ( j = 0; j <= m - 4; j += 4 ) {
          worktype t0 = d_buf[ j ] + b_data[ j ] * al;
          worktype t1 = d_buf[ j + 1 ] + b_data[ j + 1 ] * al;
          d_buf[ j ] = t0;
          d_buf[ j + 1 ] = t1;
          t0 = d_buf[ j + 2 ] + b_data[ j + 2 ] * al;
          t1 = d_buf[ j + 3 ] + b_data[ j + 3 ] * al;
          d_buf[ j + 2 ] = t0;
          d_buf[ j + 3 ] = t1;
        }
        
        for ( ; j < m; j++ )
          d_buf[ j ] += b_data[ j ] * al;
      }
      
      if ( !c_data )
        for ( j = 0; j < m; j++ )
          d_data[ j ] = ( arrtype ) ( d_buf[ j ] * alpha );
      else
        for ( j = 0; j < m; j++, c_data += c_step1 ) {
          worktype t = d_buf[ j ] * alpha;
          d_data[ j ] = ( arrtype ) ( t + c_data[ 0 ] * beta );
        }
    }
  }
  return 0;
}


int
cvGEMMBlockMul( int a_height, int a_width, int d_width,
                const arrtype* a_data, size_t a_step,
                const arrtype* b_data, size_t b_step,
                arrtype* d_data, size_t d_step, int flags ) {
  int d_height = a_height;
  int i, j, k, n = a_width, m = d_width;
  const arrtype *_a_data = a_data, *_b_data = b_data;
  arrtype* a_buf = 0;
  size_t a_step0, a_step1, t_step;
  int do_acc = flags & 16;
  
  a_step0 = a_step;
  a_step1 = 1;
  
  if ( flags & CV_GEMM_A_T ) {
    CV_SWAP( a_step0, a_step1, t_step );
    n = a_height;
    a_buf = ( arrtype* ) cvStackAlloc( n * sizeof( a_data[ 0 ] ) );
  }
  
  if ( flags & CV_GEMM_B_T ) {
    /* second operand is transposed */
    for ( i = 0; i < d_height; i++, _a_data += a_step0,
          d_data += d_step ) {
      a_data = _a_data;
      b_data = _b_data;
      
      if ( a_buf ) {
        for ( k = 0; k < n; k++ )
          a_buf[ k ] = a_data[ a_step1 * k ];
        a_data = a_buf;
      }
      
      for ( j = 0; j < d_width; j++, b_data += b_step ) {
        worktype s0 = do_acc ? d_data[ j ] : ( worktype ) ( 0 ), s1 = ( 0 );
        for ( k = 0; k <= n - 2; k += 2 ) {
          s0 += ( worktype ) ( a_data[ k ] ) * b_data[ k ];
          s1 += ( worktype ) ( a_data[ k + 1 ] ) * b_data[ k + 1 ];
        }
        
        for ( ; k < n; k++ )
          s0 += ( worktype ) ( a_data[ k ] ) * b_data[ k ];
          
        d_data[ j ] = ( arrtype ) ( s0 + s1 );
      }
    }
  } else {
    for ( i = 0; i < d_height; i++, _a_data += a_step0,
          d_data += d_step ) {
      a_data = _a_data, b_data = _b_data;
      
      if ( a_buf ) {
        for ( k = 0; k < n; k++ )
          a_buf[ k ] = a_data[ a_step1 * k ];
        a_data = a_buf;
      }
      
      for ( j = 0; j <= m - 4; j += 4 ) {
        worktype s0, s1, s2, s3;
        const arrtype* b = b_data + j;
        
        if ( do_acc ) {
          s0 = d_data[ j ];
          s1 = d_data[ j + 1 ];
          s2 = d_data[ j + 2 ];
          s3 = d_data[ j + 3 ];
        } else
          s0 = s1 = s2 = s3 = ( worktype ) ( 0 );
          
        for ( k = 0; k < n; k++, b += b_step ) {
          worktype a = ( a_data[ k ] );
          s0 += a * b[ 0 ];
          s1 += a * b[ 1 ];
          s2 += a * b[ 2 ];
          s3 += a * b[ 3 ];
        }
        
        d_data[ j ] = ( arrtype ) ( s0 );
        d_data[ j + 1 ] = ( arrtype ) ( s1 );
        d_data[ j + 2 ] = ( arrtype ) ( s2 );
        d_data[ j + 3 ] = ( arrtype ) ( s3 );
      }
      
      for ( ; j < m; j++ ) {
        const arrtype* b = b_data + j;
        worktype s0 = do_acc ? d_data[ j ] : ( worktype ) ( 0 );
        
        for ( k = 0; k < n; k++, b += b_step )
          s0 += ( worktype ) ( a_data[ k ] ) * b[ 0 ];
          
        d_data[ j ] = ( arrtype ) ( s0 );
      }
    }
  }
  
  return ( 0 );
}


int
cvGEMMStore( int d_height, int d_width,
             const arrtype* c_data, size_t c_step,
             const arrtype* d_buf, size_t d_buf_step,
             arrtype* d_data, size_t d_step,
             real alpha, real beta, int flags ) {
  const arrtype * _c_data = c_data;
  int j;
  size_t c_step0, c_step1;
  
  if ( !c_data )
    c_step0 = c_step1 = 0;
  else if ( !( flags & CV_GEMM_C_T ) )
    c_step0 = c_step, c_step1 = 1;
  else
    c_step0 = 1, c_step1 = c_step;
    
  for ( ; d_height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step ) {
    if ( _c_data ) {
      c_data = _c_data;
      for ( j = 0; j <= d_width - 4; j += 4, c_data += 4 * c_step1 ) {
        worktype t0 = alpha * d_buf[ j ];
        worktype t1 = alpha * d_buf[ j + 1 ];
        t0 += beta * ( worktype ) ( c_data[ 0 ] );
        t1 += beta * ( worktype ) ( c_data[ c_step1 ] );
        d_data[ j ] = ( arrtype ) ( t0 );
        d_data[ j + 1 ] = ( arrtype ) ( t1 );
        t0 = alpha * d_buf[ j + 2 ];
        t1 = alpha * d_buf[ j + 3 ];
        t0 += beta * ( worktype ) ( c_data[ c_step1 * 2 ] );
        t1 += beta * ( worktype ) ( c_data[ c_step1 * 3 ] );
        d_data[ j + 2 ] = ( arrtype ) ( t0 );
        d_data[ j + 3 ] = ( arrtype ) ( t1 );
      }
      for ( ; j < d_width; j++, c_data += c_step1 ) {
        worktype t0 = alpha * d_buf[ j ];
        d_data[ j ] = ( arrtype ) ( t0 + beta * c_data[ 0 ] );
      }
    } else {
      for ( j = 0; j <= d_width - 4; j += 4 ) {
        worktype t0 = alpha * d_buf[ j ];
        worktype t1 = alpha * d_buf[ j + 1 ];
        d_data[ j ] = ( arrtype ) ( t0 );
        d_data[ j + 1 ] = ( arrtype ) ( t1 );
        t0 = alpha * d_buf[ j + 2 ];
        t1 = alpha * d_buf[ j + 3 ];
        d_data[ j + 2 ] = ( arrtype ) ( t0 );
        d_data[ j + 3 ] = ( arrtype ) ( t1 );
      }
      for ( ; j < d_width; j++ )
        d_data[ j ] = ( arrtype ) ( alpha * d_buf[ j ] );
    }
  }
  return ( 0 );
}

/*
  通用矩阵乘法
  #define cvMatMulAdd( A, B, C, D ) cvGEMM( A, B, 1, C, 1, D, 0 )
  #define cvMatMul( A, B, D ) cvMatMulAdd( A, B, 0, D )
  A 第一输入数组
  B 第二输入数组
  C 第三输入数组 (偏移量)，如果没有偏移量，可以为空（ NULL） 。
  D  输出数组
  tABC T操作标志，可以是 0 或者下面列举的值的组合:
  CV_GEMM_A_T - 转置 A
  CV_GEMM_B_T - 转置 B
  CV_GEMM_C_T - 转置 C
  例如, CV_GEMM_A_T+CV_GEMM_C_T 对应
  alpha*AT*B + beta*CT
  函数 cvGEMM 执行通用矩阵乘法:
  D = alpha*op(A)*op(B) + beta*op(C), 这里 op(X) 是 X 或者 XT
  所有的矩阵应该有相同的数据类型和协调的矩阵大小。支持实数浮点矩阵或者复数浮点矩阵
*/
void
cvGEMM( int a_height, int a_width, int d_width, const real* A, int A_step,
        const real* B, int B_step, real alpha, const real* C, int C_step,
        real beta, real* D, int D_step, int flags ) {
  const int block_lin_size = 128;
  const int block_size = block_lin_size * block_lin_size;
  int d_height = a_height;
  static int inittab = 0;
  static real zero[] = {0, 0, 0, 0};
  
  real* buffer = 0;
  int local_alloc = 0;
  real* block_buffer = 0;
  int len = 0;
  
  if ( beta == 0 )
    C = 0;
    
  if ( C ) {
    if ( ( flags & CV_GEMM_C_T ) && C != D ) {
      //CvSize c_size = cvSize(d_height, d_width);
      //cvTranspose( C, C_step, D, D_step, c_size, 1, real);
      TRANSPOSE( d_width, d_height, C, C_step, D, D_step );
      C = D;
      C_step = D_step;
      flags &= ~CV_GEMM_C_T;
    }
  } else {
    //C = &stub3;
    C = 0;
    C_step = 0;
  }
  
  switch ( flags & ( CV_GEMM_A_T | CV_GEMM_B_T ) ) {
  case 0:
    len = a_width;
    break;
  case 1:
    len = a_width;
    break;
  case 2:
    len = d_width;
    break;
  case 3:
    len = d_width;
    break;
  }
  
  if ( flags == 0 && 2 <= len && len <= 4 && ( len == d_width || len == d_height ) ) {
    int i;
    {
      real* d = D;
      const real *a = A, *b = B, *c = C;
      size_t d_step = D_step;
      size_t a_step = A_step;
      size_t b_step = B_step;
      size_t c_step = C_step;
      
      if ( !c )
        c = zero;
        
      switch ( len ) {
      case 2:
        if ( len == d_width && b != d ) {
          for ( i = 0; i < d_height; i++, d += d_step, a += a_step, c += c_step ) {
            real t0 = a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ b_step ];
            real t1 = a[ 0 ] * b[ 1 ] + a[ 1 ] * b[ b_step + 1 ];
            d[ 0 ] = t0 * alpha + c[ 0 ] * beta;
            d[ 1 ] = t1 * alpha + c[ 1 ] * beta;
          }
        } else if ( a != d ) {
          int c_step0 = 1;
          if ( c == zero ) {
            c_step0 = 0;
            c_step = 1;
          }
          
          for ( i = 0; i < d_width; i++, d++, b++, c += c_step0 ) {
            real t0 = a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ b_step ];
            real t1 = a[ a_step ] * b[ 0 ] + a[ a_step + 1 ] * b[ b_step ];
            d[ 0 ] = t0 * alpha + c[ 0 ] * beta;
            d[ d_step ] = t1 * alpha + c[ c_step ] * beta;
          }
        } else
          break;
        return ;
      case 3:
        if ( len == d_width && b != d ) {
          for ( i = 0; i < d_height; i++, d += d_step, a += a_step, c += c_step ) {
            real t0 = a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ b_step ] + a[ 2 ] * b[ b_step * 2 ];
            real t1 = a[ 0 ] * b[ 1 ] + a[ 1 ] * b[ b_step + 1 ] + a[ 2 ] * b[ b_step * 2 + 1 ];
            real t2 = a[ 0 ] * b[ 2 ] + a[ 1 ] * b[ b_step + 2 ] + a[ 2 ] * b[ b_step * 2 + 2 ];
            d[ 0 ] = t0 * alpha + c[ 0 ] * beta;
            d[ 1 ] = t1 * alpha + c[ 1 ] * beta;
            d[ 2 ] = t2 * alpha + c[ 2 ] * beta;
          }
        } else if ( a != d ) {
          int c_step0 = 1;
          if ( c == zero ) {
            c_step0 = 0;
            c_step = 1;
          }
          
          for ( i = 0; i < d_width; i++, d++, b++, c += c_step0 ) {
            real t0 = a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ b_step ] + a[ 2 ] * b[ b_step * 2 ];
            real t1 = a[ a_step ] * b[ 0 ] + a[ a_step + 1 ] * b[ b_step ] + a[ a_step + 2 ] * b[ b_step * 2 ];
            real t2 = a[ a_step * 2 ] * b[ 0 ] + a[ a_step * 2 + 1 ] * b[ b_step ] + a[ a_step * 2 + 2 ] * b[ b_step * 2 ];
            
            d[ 0 ] = t0 * alpha + c[ 0 ] * beta;
            d[ d_step ] = t1 * alpha + c[ c_step ] * beta;
            d[ d_step * 2 ] = t2 * alpha + c[ c_step * 2 ] * beta;
          }
        } else
          break;
        return ;
      case 4:
        if ( len == d_width && b != d ) {
          for ( i = 0; i < d_height; i++, d += d_step, a += a_step, c += c_step ) {
            real t0 = a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ b_step ] + a[ 2 ] * b[ b_step * 2 ] + a[ 3 ] * b[ b_step * 3 ];
            real t1 = a[ 0 ] * b[ 1 ] + a[ 1 ] * b[ b_step + 1 ] + a[ 2 ] * b[ b_step * 2 + 1 ] + a[ 3 ] * b[ b_step * 3 + 1 ];
            real t2 = a[ 0 ] * b[ 2 ] + a[ 1 ] * b[ b_step + 2 ] + a[ 2 ] * b[ b_step * 2 + 2 ] + a[ 3 ] * b[ b_step * 3 + 2 ];
            real t3 = a[ 0 ] * b[ 3 ] + a[ 1 ] * b[ b_step + 3 ] + a[ 2 ] * b[ b_step * 2 + 3 ] + a[ 3 ] * b[ b_step * 3 + 3 ];
            d[ 0 ] = t0 * alpha + c[ 0 ] * beta;
            d[ 1 ] = t1 * alpha + c[ 1 ] * beta;
            d[ 2 ] = t2 * alpha + c[ 2 ] * beta;
            d[ 3 ] = t3 * alpha + c[ 3 ] * beta;
          }
        } else if ( d_width <= 16 && a != d ) {
          int c_step0 = 1;
          if ( c == zero ) {
            c_step0 = 0;
            c_step = 1;
          }
          
          for ( i = 0; i < d_width; i++, d++, b++, c += c_step0 ) {
            real t0 = a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ b_step ] + a[ 2 ] * b[ b_step * 2 ] + a[ 3 ] * b[ b_step * 3 ];
            real t1 = a[ a_step ] * b[ 0 ] + a[ a_step + 1 ] * b[ b_step ] +
                      a[ a_step + 2 ] * b[ b_step * 2 ] + a[ a_step + 3 ] * b[ b_step * 3 ];
            real t2 = a[ a_step * 2 ] * b[ 0 ] + a[ a_step * 2 + 1 ] * b[ b_step ] +
                      a[ a_step * 2 + 2 ] * b[ b_step * 2 ] + a[ a_step * 2 + 3 ] * b[ b_step * 3 ];
            real t3 = a[ a_step * 3 ] * b[ 0 ] + a[ a_step * 3 + 1 ] * b[ b_step ] +
                      a[ a_step * 3 + 2 ] * b[ b_step * 2 ] + a[ a_step * 3 + 3 ] * b[ b_step * 3 ];
            d[ 0 ] = t0 * alpha + c[ 0 ] * beta;
            d[ d_step ] = t1 * alpha + c[ c_step ] * beta;
            d[ d_step * 2 ] = t2 * alpha + c[ c_step * 2 ] * beta;
            d[ d_step * 3 ] = t3 * alpha + c[ c_step * 3 ] * beta;
          }
        } else
          break;
        return ;
      }
    }
  }
  
  {
    int b_step = B_step;
    real *D0 = D;
    
    if ( D == A || D == B ) {
      int buf_size = d_width * d_height;
      if ( d_width <= CV_MAX_LOCAL_MAT_SIZE ) {
        buffer = ( real* ) cvStackAlloc( buf_size * sizeof( real ) );
        local_alloc = 1;
      } else
        ( buffer = ( real* ) cvAlloc( buf_size ) );
        
      D = ( real* ) buffer;
    }
    
    if ( ( d_width == 1 || len == 1 ) && !( flags & CV_GEMM_B_T ) ) {
      b_step = d_width == 1 ? 0 : 1;
      flags |= CV_GEMM_B_T;
    }
    
    if ( d_height <= block_lin_size / 2 || d_width <= block_lin_size / 2 || len <= 10 ||
         d_width <= block_lin_size && d_height <= block_lin_size && len <= block_lin_size ) {
      cvGEMMSingleMul( a_height, a_width, d_width,
                       A, A_step, B, b_step,
                       C, C_step, D, D_step,
                       alpha, beta, flags );
    } else {
      int is_a_t = flags & CV_GEMM_A_T;
      int is_b_t = flags & CV_GEMM_B_T;
      int dk0_1, dk0_2;
      int a_buf_size = 0, b_buf_size, d_buf_size;
      real* a_buf = 0;
      real* b_buf = 0;
      real* d_buf = 0;
      int i, j, k, di = 0, dj = 0, dk = 0;
      int dm0, dn0, dk0;
      int a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
      int elem_size = 1;
      int work_elem_size = 1;
      
      if ( !is_a_t )
        a_step0 = A_step, a_step1 = elem_size;
      else
        a_step0 = elem_size, a_step1 = A_step;
        
      if ( !is_b_t )
        b_step0 = b_step, b_step1 = elem_size;
      else
        b_step0 = elem_size, b_step1 = b_step;
        
      if ( !C ) {
        c_step0 = c_step1 = 0;
        flags &= ~CV_GEMM_C_T;
      } else if ( !( flags & CV_GEMM_C_T ) )
        c_step0 = C_step, c_step1 = elem_size;
      else
        c_step0 = elem_size, c_step1 = C_step;
        
      dm0 = MIN( block_lin_size, d_height );
      dn0 = MIN( block_lin_size, d_width );
      dk0_1 = block_size / dm0;
      dk0_2 = block_size / dn0;
      dk0 = MAX( dk0_1, dk0_2 );
      dk0 = MIN( dk0, len );
      if ( dk0 * dm0 > block_size )
        dm0 = block_size / dk0;
      if ( dk0 * dn0 > block_size )
        dn0 = block_size / dk0;
        
      dk0_1 = ( dn0 + dn0 / 8 + 2 ) & -2;
      b_buf_size = ( dk0 + dk0 / 8 + 1 ) * dk0_1;
      d_buf_size = ( dk0 + dk0 / 8 + 1 ) * dk0_1;
      
      if ( is_a_t ) {
        a_buf_size = ( dm0 + dm0 / 8 + 1 ) * ( ( dk0 + dk0 / 8 + 2 ) & -2 );
        flags &= ~CV_GEMM_A_T;
      }
      
      ( block_buffer = ( real* ) cvAlloc( ( a_buf_size + b_buf_size + d_buf_size ) * sizeof( real ) ) );
      d_buf = block_buffer;
      b_buf = d_buf + d_buf_size;
      
      if ( is_a_t )
        a_buf = b_buf + b_buf_size;
        
      for ( i = 0; i < d_height; i += di ) {
        di = dm0;
        if ( i + di >= d_height || 8 * ( i + di ) + di > 8 * d_height )
          di = d_height - i;
          
        for ( j = 0; j < d_width; j += dj ) {
          real * _d = D + i * D_step + j * elem_size;
          const real* _c = C + i * c_step0 + j * c_step1;
          int _d_step = D_step;
          dj = dn0;
          
          if ( j + dj >= d_width || 8 * ( j + dj ) + dj > 8 * d_width )
            dj = d_width - j;
            
          flags &= 15;
          if ( dk0 < len ) {
            _d = d_buf;
            _d_step = dj * work_elem_size;
          }
          
          for ( k = 0; k < len; k += dk ) {
            const real * _a = A + i * a_step0 + k * a_step1;
            int _a_step = A_step;
            const real* _b = B + k * b_step0 + j * b_step1;
            int _b_step = b_step;
            int a_bl_height, a_bl_width;
            
            dk = dk0;
            if ( k + dk >= len || 8 * ( k + dk ) + dk > 8 * len )
              dk = len - k;
              
            if ( !is_a_t )
              a_bl_width = dk, a_bl_height = di;
            else
              a_bl_width = di, a_bl_height = dk;
              
            if ( a_buf && is_a_t ) {
              int t;
              _a_step = dk * elem_size;
              //cvGEMM_TransposeBlock( _a, A_step, a_buf, _a_step, a_bl_size, elem_size );
              TRANSPOSE( a_bl_height, a_bl_width, _a, A_step, a_buf, _a_step );
              CV_SWAP( a_bl_width, a_bl_height, t );
              _a = a_buf;
            }
            
            if ( dj < d_width ) {
              int b_height, b_width;
              if ( !is_b_t )
                b_width = dj, b_height = dk;
              else
                b_width = dk, b_height = dj;
                
              _b_step = b_width * elem_size;
              //cvGEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
              MEMCPY2D( b_height, b_width, _b, b_step, b_buf, _b_step );
              _b = b_buf;
            }
            
            if ( dk0 < len ) {
              cvGEMMBlockMul( a_bl_height, a_bl_width, dj,
                              _a, _a_step, _b, _b_step, _d, _d_step, flags );
            } else {
              cvGEMMSingleMul( a_bl_height, a_bl_width, dj,
                               _a, _a_step, _b, _b_step, _c, C_step, _d, _d_step,
                               alpha, beta, flags );
            }
            flags |= 16;
          }
          
          if ( dk0 < len )
            cvGEMMStore( di, dj, _c, C_step, _d, _d_step, D + i * D_step + j * elem_size,
                         D_step, alpha, beta, flags );
        }
      }
    }
    
    if ( D0 != D ) {
      MEMCPY2D( d_height, d_width, D, D_step, D0, d_width );
    }
  }
  
  if ( buffer && !local_alloc )
    free( buffer );
  if ( block_buffer )
    free( block_buffer );
}

/****************************************************************************************\
*                                        cvTransform                                     *
\****************************************************************************************/

#define  ICV_DEF_TRANSFORM_CASE_C1( arrtype, temptype, _ld_,        \
                                   _cast_macro1_, _cast_macro2_ )   \
{                                                                   \
    for( i = 0; i < size_width; i++, dst += dst_cn )                \
    {                                                               \
        const real* _mat = mat;                                     \
        real v0 = _ld_(src[i]);                                     \
        for( k = 0; k < dst_cn; k++, _mat += 2 )                    \
        {                                                           \
            temptype t0 = _cast_macro1_(_mat[0]*v0 + _mat[1]);      \
            dst[k] = _cast_macro2_(t0);                             \
        }                                                           \
    }                                                               \
    src += size_width;                                              \
}


#define  ICV_DEF_DIAG_TRANSFORM_CASE_C1( arrtype, temptype, _ld_,   \
                                  _cast_macro1_, _cast_macro2_ )    \
    for( i = 0; i < size_width; i++ )                               \
    {                                                               \
        real ft0;                                                   \
        temptype t0;                                                \
        ft0 = mat[0]*_ld_(src[i]) + mat[1];                         \
        t0 = _cast_macro1_(ft0);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
    }


#define  ICV_DEF_TRANSFORM_CASE_C2( arrtype, temptype, _ld_,        \
                                  _cast_macro1_, _cast_macro2_ )    \
if( dst_cn == 2 )                                                   \
{                                                                   \
    for( i = 0; i < size_width*2; i += 2 )                          \
    {                                                               \
        real ft0, ft1;                                              \
        temptype t0, t1;                                            \
        ft0 = mat[0]*_ld_(src[i]) + mat[1]*_ld_(src[i+1]) + mat[2]; \
        ft1 = mat[3]*_ld_(src[i]) + mat[4]*_ld_(src[i+1]) + mat[5]; \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
    }                                                               \
    src += size_width*2; dst += size_width*2;                       \
}                                                                   \
else                                                                \
    for( i = 0; i < size_width; i++, src += 2, dst += dst_cn )      \
    {                                                               \
        const real* _mat = mat;                                     \
        real v0 = _ld_(src[0]), v1 = src[1];                        \
        for( k = 0; k < dst_cn; k++, _mat += 3 )                    \
        {                                                           \
            temptype t0 =                                           \
                _cast_macro1_(_mat[0]*v0 + _mat[1]*v1 + _mat[2]);   \
            dst[k] = _cast_macro2_(t0);                             \
        }                                                           \
    }


#define  ICV_DEF_DIAG_TRANSFORM_CASE_C2( arrtype, temptype, _ld_,   \
                                  _cast_macro1_, _cast_macro2_ )    \
    for( i = 0; i < size_width*2; i += 2 )                          \
    {                                                               \
        real ft0, ft1;                                              \
        temptype t0, t1;                                            \
        ft0 = mat[0]*_ld_(src[i]) + mat[2];                         \
        ft1 = mat[4]*_ld_(src[i+1]) + mat[5];                       \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
    }


#define  ICV_DEF_TRANSFORM_CASE_C3( arrtype, temptype, _ld_,        \
                                  _cast_macro1_, _cast_macro2_ )    \
if( dst_cn == 3 )                                                   \
{                                                                   \
    for( i = 0; i < size_width*3; i += 3 )                          \
    {                                                               \
        real ft0, ft1, ft2;                                         \
        temptype t0, t1, t2;                                        \
        ft0 = mat[0]*_ld_(src[i]) + mat[1]*_ld_(src[i+1]) +         \
              mat[2]*_ld_(src[i+2]) + mat[3];                       \
        ft1 = mat[4]*_ld_(src[i]) + mat[5]*_ld_(src[i+1]) +         \
              mat[6]*_ld_(src[i+2]) + mat[7];                       \
        ft2 = mat[8]*_ld_(src[i]) + mat[9]*_ld_(src[i+1]) +         \
              mat[10]*_ld_(src[i+2]) + mat[11];                     \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        t2 = _cast_macro1_(ft2);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
        dst[i+2] = _cast_macro2_(t2);                               \
    }                                                               \
    src += size_width*3; dst += size_width*3;                       \
}                                                                   \
else if( dst_cn == 1 )                                              \
{                                                                   \
    for( i = 0; i < size_width; i++, src += 3 )                     \
    {                                                               \
        temptype t0 = _cast_macro1_(mat[0]*_ld_(src[0]) +           \
            mat[1]*_ld_(src[1]) + mat[2]*_ld_(src[2]) + mat[3]);    \
        dst[i] = _cast_macro2_(t0);                                 \
    }                                                               \
    dst += size_width;                                              \
}                                                                   \
else                                                                \
    for( i = 0; i < size_width; i++, src += 3, dst += dst_cn )      \
    {                                                               \
        const real* _mat = mat;                                     \
        real v0=_ld_(src[0]), v1=_ld_(src[1]), v2=_ld_(src[2]);     \
        for( k = 0; k < dst_cn; k++, _mat += 4 )                    \
        {                                                           \
            temptype t0 = _cast_macro1_(_mat[0]*v0 +                \
                    _mat[1]*v1 + _mat[2]*v2 + _mat[3]);             \
            dst[k] = _cast_macro2_(t0);                             \
        }                                                           \
    }


#define  ICV_DEF_DIAG_TRANSFORM_CASE_C3( arrtype, temptype, _ld_,   \
                                  _cast_macro1_, _cast_macro2_ )    \
    for( i = 0; i < size_width*3; i += 3 )                          \
    {                                                               \
        real ft0, ft1, ft2;                                         \
        temptype t0, t1, t2;                                        \
        ft0 = mat[0]*_ld_(src[i]) + mat[3];                         \
        ft1 = mat[5]*_ld_(src[i+1]) + mat[7];                       \
        ft2 = mat[10]*_ld_(src[i+2]) + mat[11];                     \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        t2 = _cast_macro1_(ft2);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
        dst[i+2] = _cast_macro2_(t2);                               \
    }


#define  ICV_DEF_TRANSFORM_CASE_C4( arrtype, temptype, _ld_,        \
                                  _cast_macro1_, _cast_macro2_ )    \
for( i = 0; i < size_width; i++, src += 4, dst += dst_cn )          \
{                                                                   \
    const real* _mat = mat;                                         \
    real v0 = _ld_(src[0]), v1 = _ld_(src[1]),                      \
           v2 = _ld_(src[2]), v3 = _ld_(src[3]);                    \
    for( k = 0; k < dst_cn; k++, _mat += 5 )                        \
    {                                                               \
        temptype t0 =_cast_macro1_(_mat[0]*v0+_mat[1]*v1+           \
                                   _mat[2]*v2+_mat[3]*v3+_mat[4]);  \
        dst[k] = _cast_macro2_(t0);                                 \
    }                                                               \
}


#define  ICV_DEF_DIAG_TRANSFORM_CASE_C4( arrtype, temptype, _ld_,   \
                                  _cast_macro1_, _cast_macro2_ )    \
    for( i = 0; i < size_width*4; i += 4 )                          \
    {                                                               \
        real ft0, ft1;                                              \
        temptype t0, t1;                                            \
        ft0 = mat[0]*_ld_(src[i]) + mat[4];                         \
        ft1 = mat[6]*_ld_(src[i+1]) + mat[9];                       \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
        ft0 = mat[12]*_ld_(src[i+2]) + mat[14];                     \
        ft1 = mat[18]*_ld_(src[i+3]) + mat[19];                     \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        dst[i+2] = _cast_macro2_(t0);                               \
        dst[i+3] = _cast_macro2_(t1);                               \
    }


#define ICV_DEF_TRANSFORM_FUNC( size_height, size_width, src, _srcstep, _cn,  \
  dst, _dststep, dst_cn, mat, _cast1_, _cast2_, _ld_, arrtype, temptype )     \
do {                                                                          \
  int i, k, srcstep, dststep;                                                 \
  srcstep = _srcstep - size_width*cn;                                         \
  dststep = _dststep - size_width*dst_cn;                                     \
  if (1==_cn) {                                                               \
    for( ; size_height--; src += srcstep, dst += dststep ) {                  \
      ICV_DEF_TRANSFORM_CASE_C1(arrtype, temptype, _ld_, _cast1_, _cast2_);   \
    }                                                                         \
  } else if (2==_cn) {                                                        \
    for( ; size_height--; src +=srcstep, dst += dststep ) {                   \
      ICV_DEF_TRANSFORM_CASE_C2(arrtype, temptype, _ld_, _cast1_, _cast2_);   \
    }                                                                         \
  } else if (3==_cn) {                                                        \
    for( ; size_height--; src +=srcstep, dst += dststep ) {                   \
      ICV_DEF_TRANSFORM_CASE_C3(arrtype, temptype, _ld_, _cast1_, _cast2_);   \
    }                                                                         \
  } else if (4==_cn) {                                                        \
    for( ; size_height--; src +=srcstep, dst += dststep ) {                   \
      ICV_DEF_TRANSFORM_CASE_C4(arrtype, temptype, _ld_, _cast1_, _cast2_);   \
    }                                                                         \
  }                                                                           \
} while(0)

#define  ICV_DEF_DIAG_TRANSFORM_FUNC(size_height, size_width, src, srcstep,   \
  _cn, dst, dststep, mat, _cast1_, _cast2_, _ld_, arrtype, temptype)          \
do {                                                                          \
  int i;                                                                      \
  if (1==_cn) {                                                               \
    for( ; size_height--; src += srcstep, dst += dststep ) {                  \
      ICV_DEF_DIAG_TRANSFORM_CASE_C1(arrtype,temptype,_ld_,_cast1_,_cast2_);  \
    }                                                                         \
  } else if (2==_cn) {                                                        \
    for( ; size_height--; src += srcstep, dst += dststep ) {                  \
      ICV_DEF_DIAG_TRANSFORM_CASE_C2(arrtype,temptype,_ld_,_cast1_,_cast2_);  \
    }                                                                         \
  } else if (3==_cn) {                                                        \
    for( ; size_height--; src += srcstep, dst += dststep ) {                  \
      ICV_DEF_DIAG_TRANSFORM_CASE_C3(arrtype,temptype,_ld_,_cast1_,_cast2_);  \
    }                                                                         \
  } else if (4==_cn) {                                                        \
    for( ; size_height--; src += srcstep, dst += dststep ) {                  \
      ICV_DEF_DIAG_TRANSFORM_CASE_C4(arrtype,temptype,_ld_,_cast1_,_cast2_);  \
    }                                                                         \
  }                                                                           \
} while(0)

#ifndef CV_NOP
#define CV_NOP(X)  (X)
#endif

#define icvTransform(size_height, size_width, src, _srcstep, _cn,             \
  dst, dststep, dst_cn, mat, arrtype, temptype)                               \
  ICV_DEF_TRANSFORM_FUNC( size_height, size_width, src, _srcstep, _cn,        \
  dst, dststep, dst_cn, mat, CV_NOP, CV_NOP, CV_NOP, arrtype, temptype )

#define icvDiagTransform(size_height, size_width, src, _srcstep,              \
  _cn, dst, dststep, mat, arrtype, temptype)                                  \
  ICV_DEF_DIAG_TRANSFORM_FUNC(size_height, size_width, src, _srcstep,         \
  _cn, dst, dststep, mat, CV_NOP, CV_NOP, CV_NOP, arrtype, temptype)


////////////////////////////////////////////////////////////////
/*
对数组每一个元素执行矩阵变换
src 输入数组
dst 输出数组
transmat 变换矩阵[dst_cn*(cn + 1|cn)]
shiftvec 可选偏移向量
函数 cvTransform 对数组 src 每一个元素执行矩阵变换并将结果存储到 dst:
dst(I)=transmat*src(I) + shiftvec
或者 dst(I)k=sumj(transmat(k,j)*src(I)j) + shiftvec(k)
N-通道数组 src 的每一个元素都被视为一个N元向量，使用一个 M×N 的变换矩阵 transmat
和偏移向量 shiftvec 把它变换到一个 M-通道的数组 dst 的一个元素中。
这里可以选择将偏移向量 shiftvec 嵌入到 transmat 中。
这样的话 transmat 应该是 M×N+1 的矩阵，并且最右边的一列被看作是偏移向量 。
输入数组和输出数组应该有相同的位深（depth）和同样的大小或者 ROI 大小。
transmat 和 shiftvec 应该是实数浮点矩阵。
该函数可以用来进行 ND 点集的几何变换，任意的线性颜色空间变换，通道转换等。
*/
void
cvTransform( int size_height, int size_width, const real* src, int _srcstep, int cn,
             real* dst, int _dststep, int dst_cn, const real* rot, int rot_cols, int rot_step ) {
  int i, j;
  real mat[ 32 ];
  
  if ( cn > 4 || dst_cn > 4 )
    CV_ERROR( CV_StsOutOfRange, "Both input and output array must have at most 4 channels" );
    
  if ( rot_cols == cn + 1 || rot_cols == cn ) {
    for ( i = 0; i < dst_cn; i++ ) {
      mat[ i * ( cn + 1 ) + cn ] = 0;
      for ( j = 0; j < rot_cols; j++ )
        mat[ i * ( cn + 1 ) + j ] = ( ( rot + rot_step * i ) ) [ j ];
    }
  } else
    CV_ERROR( CV_StsUnmatchedSizes, "If the source array has <cn> channels, "
              "the transformation matrix must have <cn> x <cn>+1 or <cn> x <cn> size" );
  {
    int diag_transform = 0;
    
    if ( dst_cn == cn ) {
      diag_transform = 1;
      for ( i = 0; i < dst_cn; i++ )
        for ( j = 0; j < cn; j++ ) {
          if ( i != j && fabs( mat[ i * ( cn + 1 ) + j ] ) > DBL_EPSILON ) {
            diag_transform = 0;
            break;
          }
        }
    }
    
    if ( diag_transform ) {
      icvDiagTransform( size_height, size_width, src, _srcstep,
                        cn, dst, _dststep, mat, real, real );
    } else {
      icvTransform( size_height, size_width, src, _srcstep, cn,
                    dst, _dststep, dst_cn, mat, real, real );
    }
  }
}

/****************************************************************************************\
*                                        cvPerspectiveTransform                          *
\****************************************************************************************/

/*
向量数组的透视变换
src 输入的三通道浮点数组
dst 输出三通道浮点数组
mat 4 × 4 变换矩阵
函数 cvPerspectiveTransform 用下面的方式变换 src 的每一个元素 (通过将其视为二维或者三维的向量):
(x, y, z) -> (x'/w, y'/w, z'/w)
或者 (x, y) -> (x'/w, y'/w),
这里 (x', y', z', w') = mat*(x, y, z, 1)
或者 (x', y', w') = mat*(x, y, 1)
并且 w = w' 如果 w'!=0, 否则 w = inf
*/

void
cvPerspectiveTransform( int height, int width, const real* src, int srcstep, int cn,
                        real* dst, int dststep, const real* mat, int mat_step ) {
  real buffer[ 16 ];
  
  int i, j;
  
  if ( cn != 2 && cn != 3 )
    CV_ERROR( CV_BadNumChannels, cvUnsupportedFormat );
    
  for ( i = 0; i <= cn; i++ ) {
    for ( j = 0; j <= cn; j++ )
      buffer[ i * ( cn + 1 ) + j ] = ( mat + mat_step * i ) [ j ];
  }
  
  if ( width * cn == srcstep && width * cn == dststep ) {
    width *= height;
    height = 1;
  }
  
  // ( func( src, src_step, dst, dst_step, size, buffer));
  mat = buffer;
  if ( 2 == cn ) {
    int i;
    width *= 2;
    srcstep /= sizeof( src[ 0 ] );
    dststep /= sizeof( dst[ 0 ] );
    
    for ( ; height--; src += srcstep, dst += dststep ) {
      for ( i = 0; i < width; i += 2 ) {
        arrtype x = src[ i ], y = src[ i + 1 ];
        real w = x * mat[ 6 ] + y * mat[ 7 ] + mat[ 8 ];
        
        if ( fabs( w ) > FLT_EPSILON ) {
          w = 1.f / w;
          dst[ i ] = ( arrtype ) ( ( x * mat[ 0 ] + y * mat[ 1 ] + mat[ 2 ] ) * w );
          dst[ i + 1 ] = ( arrtype ) ( ( x * mat[ 3 ] + y * mat[ 4 ] + mat[ 5 ] ) * w );
        } else {
          dst[ i ] = ( arrtype ) 0;
          dst[ i + 1 ] = ( arrtype ) 0;
        }
      }
    }
    
    return ;
  } else if ( 3 == cn ) {
    int i;
    width *= 3;
    
    for ( ; height--; src += srcstep, dst += dststep ) {
      for ( i = 0; i < width; i += 3 ) {
        arrtype x = src[ i ], y = src[ i + 1 ], z = src[ i + 2 ];
        real w = x * mat[ 12 ] + y * mat[ 13 ] + z * mat[ 14 ] + mat[ 15 ];
        
        if ( fabs( w ) > FLT_EPSILON ) {
          w = 1.f / w;
          dst[ i ] = ( arrtype ) ( ( x * mat[ 0 ] + y * mat[ 1 ] + z * mat[ 2 ] + mat[ 3 ] ) * w );
          dst[ i + 1 ] = ( arrtype ) ( ( x * mat[ 4 ] + y * mat[ 5 ] + z * mat[ 6 ] + mat[ 7 ] ) * w );
          dst[ i + 2 ] = ( arrtype ) ( ( x * mat[ 8 ] + y * mat[ 9 ] + z * mat[ 10 ] + mat[ 11 ] ) * w );
        } else {
          dst[ i ] = ( arrtype ) 0;
          dst[ i + 1 ] = ( arrtype ) 0;
          dst[ i + 2 ] = ( arrtype ) 0;
        }
      }
    }
    
    return ;
  }
  
  //CV_CHECK_NANS( dst );
  
}

/****************************************************************************************\
*                                       cvScaleAdd                                       *
\****************************************************************************************/

#define  ICV_DEF_MULADDC_CASE_C1( arrtype, temptype, src1, src2, dst, len )     \
{                                                                               \
    int i;                                                                      \
                                                                                \
    for( i = 0; i <= (len) - 4; i += 4 )                                        \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 + (src2)[i];                                 \
        temptype t1 = (src1)[i+1]*s0 + (src2)[i+1];                             \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
                                                                                \
        t0 = (src1)[i+2]*s0 + (src2)[i+2];                                      \
        t1 = (src1)[i+3]*s0 + (src2)[i+3];                                      \
                                                                                \
        (dst)[i+2] = (arrtype)t0;                                               \
        (dst)[i+3] = (arrtype)t1;                                               \
    }                                                                           \
                                                                                \
    for( ; i < (len); i++ )                                                     \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 + (src2)[i];                                 \
        (dst)[i] = (arrtype)t0;                                                 \
    }                                                                           \
}


#define  ICV_DEF_MULADDC_CASE_C2( arrtype, temptype, src1, src2, dst, len )     \
{                                                                               \
    int i;                                                                      \
                                                                                \
    for( i = 0; i <= (len) - 4; i += 4 )                                        \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 - (src1)[i+1]*s1 + (src2)[i];                \
        temptype t1 = (src1)[i]*s1 + (src1)[i+1]*s0 + (src2)[i+1];              \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
                                                                                \
        t0 = (src1)[i+2]*s0 - (src1)[i+3]*s1 + (src2)[i+2];                     \
        t1 = (src1)[i+2]*s1 + (src1)[i+3]*s0 + (src2)[i+3];                     \
                                                                                \
        (dst)[i+2] = (arrtype)t0;                                               \
        (dst)[i+3] = (arrtype)t1;                                               \
    }                                                                           \
                                                                                \
    for( ; i < (len); i += 2 )                                                  \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 - (src1)[i+1]*s1 + (src2)[i];                \
        temptype t1 = (src1)[i]*s1 + (src1)[i+1]*s0 + (src2)[i+1];              \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
    }                                                                           \
}

/*
  计算一个数组缩放后与另一个数组的和
  #define cvMulAddS cvScaleAdd
  src1 第一输入数组
  scale 第一输入数组的缩放因子
  src2 第二输入数组
  dst 输出数组
  函数 cvScaleAdd 计算一个数组缩放后与另一个数组的和:
  dst(I)=src1(I)*scale + src2(I)
  所有的数组参数必须有相同的类型和大小。
*/
void
cvScaleAdd( int height, int width, const real* src1, int src1_step, real s0, real s1,
            const real* src2, int src2_step, real* dst, int dst_step, int cn ) {
  if ( width == src1_step && width == src2_step && width == dst_step ) {
    width *= height;
    
    if ( width <= CV_MAX_INLINE_MAT_OP_SIZE ) {
      const real * mA = src1;
      const real* mB = src2;
      real* mC = dst;
      
      do {
        mC[ width - 1 ] = mA[ width - 1 ] * s0 + mB[ width - 1 ];
      } while ( --width );
      
      return ;
    }
    
    height = 1;
  }
  
  if ( cn > 2 ) {
    CV_ERROR( CV_StsOutOfRange, "The function only supports 1- and 2-channel arrays" );
  }
  
  width *= ( cn );
  
  if ( 1 == cn ) {
    for ( ; height--; src1 += src1_step, src2 += src2_step, dst += dst_step ) {
      ICV_DEF_MULADDC_CASE_C1( real, real, src1, src2, dst, width )
    }
  } else if ( 2 == cn ) {
    for ( ; height--; src1 += src1_step, src2 += src2_step, dst += dst_step ) {
      ICV_DEF_MULADDC_CASE_C1( real, real, src1, src2, dst, width )
    }
  }
  
  //CV_CHECK_NANS( dst );
}



/****************************************************************************************\
*                                        cvMulTransposed                                 *
\****************************************************************************************/

#define load_macro(x) (x)

//#define ICV_DEF_MULTRANS_R_FUNC( flavor, srctype, dsttype, load_macro )
int
cvMulTransposedR( int size_height, int size_width, const srctype* src, int srcstep,
                  dsttype* dst, int dststep, const dsttype* delta, int deltastep,
                  int delta_cols, real scale ) {
  int i, j, k;
  dsttype* tdst = dst;
  dsttype* col_buf = 0;
  dsttype* delta_buf = 0;
  int local_alloc = 0;
  int buf_size = size_height * sizeof( dsttype );
  
  if ( delta && delta_cols < size_width ) {
    assert( delta_cols == 1 );
    buf_size += 4 * buf_size;
  }
  
  if ( buf_size <= CV_MAX_LOCAL_SIZE ) {
    col_buf = ( dsttype* ) cvStackAlloc( buf_size );
    local_alloc = 1;
  } else {
    col_buf = ( dsttype* ) cvAlloc( buf_size );
    if ( !col_buf )
      return 0;
  }
  
  if ( delta && delta_cols < size_width ) {
    delta_buf = col_buf + size_height;
    for ( i = 0; i < size_height; i++ )
      delta_buf[ i * 4 ] = delta_buf[ i * 4 + 1 ] =
                             delta_buf[ i * 4 + 2 ] = delta_buf[ i * 4 + 3 ] = delta[ i * deltastep ];
    delta = delta_buf;
    deltastep = deltastep ? 4 : 0;
  }
  
  if ( !delta ) {
    for ( i = 0; i < size_width; i++, tdst += dststep ) {
      for ( k = 0; k < size_height; k++ )
        col_buf[ k ] = src[ k * srcstep + i ];
        
      for ( j = i; j <= size_width - 4; j += 4 ) {
        real s0 = 0, s1 = 0, s2 = 0, s3 = 0;
        const srctype *tsrc = src + j;
        
        for ( k = 0; k < size_height; k++, tsrc += srcstep ) {
          real a = col_buf[ k ];
          s0 += a * load_macro( tsrc[ 0 ] );
          s1 += a * load_macro( tsrc[ 1 ] );
          s2 += a * load_macro( tsrc[ 2 ] );
          s3 += a * load_macro( tsrc[ 3 ] );
        }
        
        tdst[ j ] = ( dsttype ) ( s0 * scale );
        tdst[ j + 1 ] = ( dsttype ) ( s1 * scale );
        tdst[ j + 2 ] = ( dsttype ) ( s2 * scale );
        tdst[ j + 3 ] = ( dsttype ) ( s3 * scale );
      }
      
      for ( ; j < size_width; j++ ) {
        real s0 = 0;
        const srctype *tsrc = src + j;
        
        for ( k = 0; k < size_height; k++, tsrc += srcstep )
          s0 += col_buf[ k ] * tsrc[ 0 ];
          
        tdst[ j ] = ( dsttype ) ( s0 * scale );
      }
    }
  } else {
    for ( i = 0; i < size_width; i++, tdst += dststep ) {
      if ( !delta_buf )
        for ( k = 0; k < size_height; k++ )
          col_buf[ k ] = load_macro( src[ k * srcstep + i ] ) - delta[ k * deltastep + i ];
      else
        for ( k = 0; k < size_height; k++ )
          col_buf[ k ] = load_macro( src[ k * srcstep + i ] ) - delta_buf[ k * deltastep ];
          
      for ( j = i; j <= size_width - 4; j += 4 ) {
        real s0 = 0, s1 = 0, s2 = 0, s3 = 0;
        const srctype *tsrc = src + j;
        const dsttype *d = delta_buf ? delta_buf : delta + j;
        
        for ( k = 0; k < size_height; k++, tsrc += srcstep, d += deltastep ) {
          real a = col_buf[ k ];
          s0 += a * ( load_macro( tsrc[ 0 ] ) - d[ 0 ] );
          s1 += a * ( load_macro( tsrc[ 1 ] ) - d[ 1 ] );
          s2 += a * ( load_macro( tsrc[ 2 ] ) - d[ 2 ] );
          s3 += a * ( load_macro( tsrc[ 3 ] ) - d[ 3 ] );
        }
        
        tdst[ j ] = ( dsttype ) ( s0 * scale );
        tdst[ j + 1 ] = ( dsttype ) ( s1 * scale );
        tdst[ j + 2 ] = ( dsttype ) ( s2 * scale );
        tdst[ j + 3 ] = ( dsttype ) ( s3 * scale );
      }
      
      for ( ; j < size_width; j++ ) {
        real s0 = 0;
        const srctype *tsrc = src + j;
        const dsttype *d = delta_buf ? delta_buf : delta + j;
        
        for ( k = 0; k < size_height; k++, tsrc += srcstep, d += deltastep )
          s0 += col_buf[ k ] * ( load_macro( tsrc[ 0 ] ) - d[ 0 ] );
          
        tdst[ j ] = ( dsttype ) ( s0 * scale );
      }
    }
  }
  
  /* fill the lower part of the destination matrix */
  for ( i = 1; i < size_width; i++ )
    for ( j = 0; j < i; j++ )
      dst[ dststep * i + j ] = dst[ dststep * j + i ];
      
  if ( col_buf && !local_alloc )
    free( col_buf );
    
  return 0;
}

// #define ICV_DEF_MULTRANS_L_FUNC( flavor, srctype, dsttype, load_macro )
int
cvMulTransposedL( int size_height, int size_width, const srctype* src, int srcstep,
                  dsttype* dst, int dststep, dsttype* delta, int deltastep,
                  int delta_cols, real scale ) {
  int i, j, k;
  dsttype* tdst = dst;
  
  if ( !delta ) {
    for ( i = 0; i < size_height; i++, tdst += dststep )
      for ( j = i; j < size_height; j++ ) {
        real s = 0;
        const srctype *tsrc1 = src + i * srcstep;
        const srctype *tsrc2 = src + j * srcstep;
        
        for ( k = 0; k <= size_width - 4; k += 4 )
          s += tsrc1[ k ] * tsrc2[ k ] + tsrc1[ k + 1 ] * tsrc2[ k + 1 ] +
               tsrc1[ k + 2 ] * tsrc2[ k + 2 ] + tsrc1[ k + 3 ] * tsrc2[ k + 3 ];
        for ( ; k < size_width; k++ )
          s += tsrc1[ k ] * tsrc2[ k ];
        tdst[ j ] = ( dsttype ) ( s * scale );
      }
  } else {
    dsttype* row_buf = 0;
    int local_alloc = 0;
    int buf_size = size_width * sizeof( dsttype );
    dsttype delta_buf[ 4 ];
    int delta_shift = delta_cols == size_width ? 4 : 0;
    
    if ( buf_size <= CV_MAX_LOCAL_SIZE ) {
      row_buf = ( dsttype* ) cvStackAlloc( buf_size );
      local_alloc = 1;
    } else {
      row_buf = ( dsttype* ) cvAlloc( buf_size );
      if ( !row_buf )
        return 0;
    }
    
    for ( i = 0; i < size_height; i++, tdst += dststep ) {
      const srctype * tsrc1 = src + i * srcstep;
      const dsttype *tdelta1 = delta + i * deltastep;
      
      if ( delta_cols < size_width )
        for ( k = 0; k < size_width; k++ )
          row_buf[ k ] = tsrc1[ k ] - tdelta1[ 0 ];
      else
        for ( k = 0; k < size_width; k++ )
          row_buf[ k ] = tsrc1[ k ] - tdelta1[ k ];
          
      for ( j = i; j < size_height; j++ ) {
        real s = 0;
        const srctype *tsrc2 = src + j * srcstep;
        const dsttype *tdelta2 = delta + j * deltastep;
        if ( delta_cols < size_width ) {
          delta_buf[ 0 ] = delta_buf[ 1 ] =
                             delta_buf[ 2 ] = delta_buf[ 3 ] = tdelta2[ 0 ];
          tdelta2 = delta_buf;
        }
        for ( k = 0; k <= size_width - 4; k += 4, tdelta2 += delta_shift )
          s += row_buf[ k ] * ( load_macro( tsrc2[ k ] ) - tdelta2[ 0 ] ) +
               row_buf[ k + 1 ] * ( load_macro( tsrc2[ k + 1 ] ) - tdelta2[ 1 ] ) +
               row_buf[ k + 2 ] * ( load_macro( tsrc2[ k + 2 ] ) - tdelta2[ 2 ] ) +
               row_buf[ k + 3 ] * ( load_macro( tsrc2[ k + 3 ] ) - tdelta2[ 3 ] );
        for ( ; k < size_width; k++, tdelta2++ )
          s += row_buf[ k ] * ( load_macro( tsrc2[ k ] ) - tdelta2[ 0 ] );
        tdst[ j ] = ( dsttype ) ( s * scale );
      }
    }
    
    if ( row_buf && !local_alloc )
      free( row_buf );
  }
  
  /* fill the lower part of the destination matrix */
  for ( j = 0; j < size_height - 1; j++ )
    for ( i = j; i < size_height; i++ )
      dst[ dststep * i + j ] = dst[ dststep * j + i ];
      
  return 0;
}

/*
  计算数组和数组的转置的乘积
  src 输入矩阵
  dst 目标矩阵
  order 乘法顺序
  delta 一个可选数组， 在乘法之前从 src 中减去该数组。
  函数 cvMulTransposed 计算 src 和它的转置的乘积。
  函数求值公式：
  如果 order=0
  dst=(src-delta)*(src-delta)T
  否则
  dst=(src-delta)T*(src-delta)
*/
void
cvMulTransposed( int size_height, int size_width, const real* src, int src_step,
                 real* dst, int dst_step, int order, real* delta, int delta_step,
                 int delta_cols, real scale ) {
  const int gemm_level = 100; // boundary above which GEMM is faster.
  
  if ( order ) {
    cvMulTransposedR( size_height, size_width, src, src_step, dst, dst_step,
                      delta, delta_step, delta_cols, scale );
  } else {
    cvMulTransposedL( size_height, size_width, src, src_step, dst, dst_step,
                      delta, delta_step, delta_cols, scale );
  }
}


#define ICV_EXT_PRODUCT_CASE(vec, vecstep, avg, avgstep, dst, dststep, size, tempbuf) \
do {                                                                          \
  int x, y, dstsize = size_width * size_height, i;                            \
  for( y = 0; y < size_height; y++, vec += vecstep, avg += avgstep )          \
    for( x = 0; x < size_width; x++ )                                         \
      *(tempbuf + i++) = *(vec+x) - *(avg+x);                                 \
  for( y = 0; y < dstsize; y++, dst += dststep ) {                            \
    real ty = *(tempbuf+y);                                                   \
    for( x = 0; x <= y - 3; x += 4 ) {                                        \
      real t0 = *(dst+x) + ty* *(tempbuf+x);                                  \
      real t1 = *(dst+x+1) + ty* *(tempbuf+x+1);                              \
      *(dst+x) = t0;                                                          \
      *(dst+x+1) = t1;                                                        \
      t0 = *(dst+x+2) + ty* *(tempbuf+x+2);                                   \
      t1 = *(dst+x+3) + ty* *(tempbuf+x+3);                                   \
      *(dst+x+2) = t0;                                                        \
      *(dst+x+3) = t1;                                                        \
    }                                                                         \
    for( ; x <= y; x++ )                                                      \
      *(dst+x) = *(dst+x) + ty* *(tempbuf+x);                                 \
  }                                                                           \
} while(0)

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

#define SKIP_SUM_N(_N, _M, _A, _AL, _AI, _S)                                  \
do {                                                                          \
  int _I, _J, _K;                                                             \
  for (_I=0, _K=0; _I<(_N)*(_AL); _I+=(_AL), ++_K)                            \
    for (_J=0; _J<(_M)*(_AI); _J+=(_AI))                                      \
      *(_S+_K) += *(_A + _I + _J);                                            \
} while(0)

/*
计算向量集合的协方差矩阵
vects 输入向量。他们必须有同样的数据类型和大小。这个向量不一定非是一维的，他们也可以是二维（例如，图像）等等。
count 输入向量的数目
cov_mat 输出协方差矩阵，它是浮点型的方阵。
avg 输入或者输出数组 (依赖于标记“flags”) - 输入向量的平均向量。
flags 操作标志,下面值的组合:
CV_COVAR_SCRAMBLED - 输出协方差矩阵按下面计算:
scale * [vects[0] ? avg,vects[1] ? avg,...]T * [vects[0] ? avg,vects[1] ? avg,...],
即协方差矩阵是 count×count. 这样一个不寻常的矩阵用于一组大型向量的快速PCA方法
(例如, 人脸识别的 EigenFaces 技术)。这个混杂（"scrambled"）矩阵的特征值将和真正的
协方差矩阵的特征值匹配，真正的特征向量可以很容易的从混杂（"scrambled"）协方差矩阵的特征向量中计算出来。
CV_COVAR_NORMAL - 输出协方差矩阵被计算成:
scale * [vects[0] ? avg,vects[1] ? avg,...] * [vects[0] ? avg,vects[1] ? avg,...]T,
也就是说, cov_mat 将是一个和每一个输入向量的元素数目具有同样线性大小的通常协方差矩阵。
CV_COVAR_SCRAMBLED 和 CV_COVAR_NORMAL 只能同时指定其中一个。
CV_COVAR_USE_AVG - 如果这个标志被指定， 该函数将不会从输入向量中计算 avg ，
而是用过去的 avg 向量，如果 avg 已经以某种方式计算出来了这样做是很有用的。
或者如果协方差矩阵是部分计算出来的 - 倘若这样, avg 不是输入向量的子集的平均值，而是整个集合的平均向量。
CV_COVAR_SCALE - 如果这个标志被指定，协方差矩阵被缩放了。 the covariation matrix is scaled.
在 "normal" 模式下缩放比例是 1./count, 在 "scrambled" 模式下缩放比例是每一个输入向量的元素总和的倒数。
缺省地(如果没有指定标志) 协方差矩阵不被缩放 (scale=1)。
函数 cvCalcCovarMatrix 计算输入向量的协方差矩阵和平均向量。该函数可以被运用到主成分分析中（PCA），
以及马氏距离（Mahalanobis distance）比较向量中等等。
*/

#define MALLOC_COPY(TYPE, _N, _A, _B) do { _B = MALLOC(TYPE, _N); COPY(_N, _A, _B); } while(0)

void
cvCalcCovarMatrix( int vecheight, int vecwidth, const real* _vec, int vecstep,
                   real* cov, int covstep, real* avg, int flags ) {
  int dims = vecheight, count = vecwidth, i;
  int local_alloc_avg = 0;
  real* vec = NULL;
  MALLOC_COPY( real, vecheight * vecstep, _vec, vec );
  if ( !avg ) {
    avg = MALLOC( real, dims );
    FILL( dims, avg, 0.f );
    SKIP_SUM_N( dims, count, vec, vecstep, 1, avg );
    FOR_EACH_OP_1D1A( dims, avg, 1, /= count );
  }
  
  for ( i = 0; i < dims; ++i ) {
    real avgi = avg[ i ];
    real *veci = vec + i * vecstep;
    FOR_EACH_OP_1D1A( count, veci, 1, -= avgi );
  }
  if ( ( flags & CV_COVAR_NORMAL ) != 0 ) {
    cvMatMul_AxAT( vecheight, vecwidth, vec, vecstep, cov, covstep );
  } else {
    cvMatMul_ATxA( vecheight, vecwidth, vec, vecstep, cov, covstep );
  }
  if ( local_alloc_avg ) {
    SAFEFREE( avg );
  }
  SAFEFREE( vec );
}

/****************************************************************************************\
*                                        cvMahalanobis                                   *
\****************************************************************************************/

static int
icvMahalanobis( const real* mat, int matstep, const real* vec, int len, real* _result ) {
  int i, j;
  real result = 0;
  
  for ( i = 0; i < len; i++, mat += matstep ) {
    real row_sum = 0;
    for ( j = 0; j <= len - 4; j += 4 )
      row_sum += vec[ j ] * mat[ j ] + vec[ j + 1 ] * mat[ j + 1 ] +
                 vec[ j + 2 ] * mat[ j + 2 ] + vec[ j + 3 ] * mat[ j + 3 ];
    for ( ; j < len; j++ )
      row_sum += vec[ j ] * mat[ j ];
    result += row_sum * vec[ i ];
  }
  *_result = result;
  
  return ( 0 );
}

/*
计算两个向量之间的马氏距离（Mahalanobis distance）
vec1 第一个一维输入向量
vec2 第二个一维输入向量
mat 协方差矩阵的逆矩阵
函数 cvMahalanobis 计算两个向量之间的加权距离，其返回结果是：
协方差矩阵可以用函数cvCalcCovarMatrix 计算出来，逆矩阵可以用函数 cvInvert 计算出来 (CV_SVD 方法是一个比较好的选择, 因为矩阵可能是奇异的).
*/

real
cvMahalanobis( int len, const real* srcA, const real* srcB, real* mat, int mat_step ) {
  real * buffer = 0;
  int local_alloc = 0;
  real dist = 0;
  int buf_size;
  
  buf_size = len * sizeof( real );
  
  if ( buf_size <= CV_MAX_LOCAL_SIZE ) {
    buffer = ( real* ) cvStackAlloc( buf_size );
    local_alloc = 1;
  } else {
    buffer = ( real* ) cvAlloc( buf_size );
  }
  
  FOR_EACH1D_OP( len, buffer, 1, srcA, 1, srcB, 1, - );
  
  icvMahalanobis( mat, mat_step, buffer, len, &dist );
  dist = ( real ) sqrt( dist );
  
  if ( buffer && !local_alloc )
    free( buffer );
    
  return dist;
}

/****************************************************************************************\
*                                        cvDotProduct                                    *
\****************************************************************************************/

/*
  用欧几里得准则计算两个数组的点积
  src1 第一输入数组。
  src2 第二输入数组。
  函数 cvDotProduct 计算并返回两个数组的欧几里得点积。
  src1?src2 = sumI(src1(I)*src2(I))
  如果是多通道数组，所有通道的结果是累加在一起的。特别地， cvDotProduct(a,a),
  将返回 ||a||2 ，这里 a 是一个复向量。 该函数可以处理多通道数组,逐行或逐层等等。
*/
real
cvDotProduct( int height, int width, const real* srcA, int srcA_step, const real* srcB, int srcB_step ) {
  int i;
  real sum = 0;
  
  if ( width == srcA_step && width == srcB_step ) {
    width *= height;
    if ( width <= CV_MAX_INLINE_MAT_OP_SIZE ) {
      do {
        sum += srcA[ width - 1 ] * srcB[ width - 1 ];
      } while ( --width );
      return sum;
    }
    height = 1;
  }
  
  for ( ; height--; srcA += srcA_step, srcB += srcB_step ) {
    for ( i = 0; i <= width - 4; i += 4 ) {
      real t0 = ( real ) srcA[ i ] * srcB[ i ];
      real t1 = ( real ) srcA[ i + 1 ] * srcB[ i + 1 ];
      t0 += ( real ) srcA[ i + 2 ] * srcB[ i + 2 ];
      t1 += ( real ) srcA[ i + 3 ] * srcB[ i + 3 ];
      sum += t0 + t1;
    }
    
    for ( ; i < width; i++ ) {
      sum += ( real ) srcA[ i ] * srcB[ i ];
    }
  }
  
  return sum;
}

/* End of file. */

