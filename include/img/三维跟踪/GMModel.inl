// #include "stdafx.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//#include "algo.h"
#include "GMModel.h"


// 基础函数.
template <typename _II, typename IO>
inline
void rgb2gray( _II s, IO d, int h, int w, int ss, int ds ) {
  for ( int i = 0; i < h; ++i ) {
    for ( int j = 0; j < w; ++j ) {
      const double TMP_B = 0.114 * ( int ) ( *( s + i * ss + j * 3 + 0 ) + 0.5 );
      const double TMP_G = 0.587 * ( int ) ( *( s + i * ss + j * 3 + 1 ) + 0.5 );
      const double TMP_R = 0.299 * ( int ) ( *( s + i * ss + j * 3 + 2 ) + 0.5 );
      *( d + i * ds + j ) = int( TMP_B + TMP_G + TMP_R + 0.5 );
    }
  }
}

template <typename _II, typename IO>
inline
void rgb2grayx( const _II *s, IO *d, int h, int w, int ss, int ds ) {
  for ( int i = 0; i < h; ++i ) {
    for ( int j = 0; j < w; ++j ) {
      const IO TMP_B = IO( 0.114 * *( s + i * ss + j * 3 + 0 ) );
      const IO TMP_G = IO( 0.587 * *( s + i * ss + j * 3 + 1 ) );
      const IO TMP_R = IO( 0.299 * *( s + i * ss + j * 3 + 2 ) );
      
      *( d + i * ds + j ) = TMP_B + TMP_G + TMP_R;
    }
  }
}

template <typename _T0, typename _T1>
inline
void mat2gray( const _T0 *s, _T1 *d, int len ) {
  _T0 min = *s;
  _T0 max = *s;
  const _T0 *p = s + 1;
  
  for ( ; p < s + len; ++p ) {
    const _T0 t = *p;
    
    if ( t > max )
      max = t;
    if ( t < min )
      min = t;
  }
  
  const double dist = max - min;
  const _T0 * const ps = s + ( len & ( ~0x00000003 ) );
  
  const _T0 *p0 = s ;
  const _T0 *p1 = s + 1;
  const _T0 *p2 = s + 2;
  const _T0 *p3 = s + 3;
  
  _T1 *q0 = d ;
  _T1 *q1 = d + 1;
  _T1 *q2 = d + 2;
  _T1 *q3 = d + 3;
  
  for ( ; p0 < ps; ) {
    const _T1 t0 = ( _T1 ) ( ( *p0 - min ) / dist );
    const _T1 t1 = ( _T1 ) ( ( *p1 - min ) / dist );
    const _T1 t2 = ( _T1 ) ( ( *p2 - min ) / dist );
    const _T1 t3 = ( _T1 ) ( ( *p3 - min ) / dist );
    {
      *q0 = t0 > 1 ? 1 : ( t0 < 0 ? 0 : t0 );
      *q1 = t1 > 1 ? 1 : ( t1 < 0 ? 0 : t1 );
      *q2 = t2 > 1 ? 1 : ( t2 < 0 ? 0 : t2 );
      *q3 = t3 > 1 ? 1 : ( t3 < 0 ? 0 : t3 );
    }
    {
      p0 += 4;
      p1 += 4;
      p2 += 4;
      p3 += 4;
      
      q0 += 4;
      q1 += 4;
      q2 += 4;
      q3 += 4;
    }
  }
  
  for ( ; p0 < s + len; ) {
    const _T1 t0 = ( _T1 ) ( ( *p0 - min ) / dist );
    *q0 = t0 > 1 ? 1 : ( t0 < 0 ? 0 : t0 );
    ++q0;
    ++p0;
  }
}

template <typename _T>
inline
void copy( const _T *s, _T *d, int len ) {
  const _T * const r = s + ( len & ( ~0x00000003 ) );
  
  const _T *q0 = s ;
  const _T *q1 = s + 1;
  const _T *q2 = s + 2;
  const _T *q3 = s + 3;
  
  _T *p0 = d ;
  _T *p1 = d + 1;
  _T *p2 = d + 2;
  _T *p3 = d + 3;
  
  for ( ; q0 < r; ) {
    *( p0 ) = *( q0 );
    *( p1 ) = *( q1 );
    *( p2 ) = *( q2 );
    *( p3 ) = *( q3 );
    
    p0 += 4;
    p1 += 4;
    p2 += 4;
    p3 += 4;
    
    q0 += 4;
    q1 += 4;
    q2 += 4;
    q3 += 4;
  }
  
  for ( ; q0 < s + len; ++p0, ++q0 )
    *( p0 ) = *( q0 );
}

template <typename _T>
inline
void init( _T *s, _T v, int len ) {
  const _T * const r = s + ( len & ( ~0x00000003 ) );
  
  _T *p0 = s ;
  _T *p1 = s + 1;
  _T *p2 = s + 2;
  _T *p3 = s + 3;
  
  for ( ; p0 < r; ) {
    *( p0 ) = v;
    *( p1 ) = v;
    *( p2 ) = v;
    *( p3 ) = v;
    
    p0 += 4;
    p1 += 4;
    p2 += 4;
    p3 += 4;
  }
  
  for ( ; p0 < s + len; ++p0 )
    *( p0 ) = v;
}

// 从大到小排序
template <typename _II>
inline
void sort( _II *array, int len ) {
  assert( NULL != array );
  
  if ( len < 2 )
    return ;
    
  for ( int i = 1; i < len; ++i ) {
    assert( 0 < i && len > i );
    const _II t = *( array + i );
    
    for ( int j = i - 1; j > -1 && t > *( array + j ); --j ) {
      assert( -1 < j && len - 1 > j );
      *( array + j + 1 ) = *( array + j );
    }
    
    assert( -1 < j + 1 && len > j + 1 );
    *( array + j + 1 ) = t;
  }
}

static inline int root( const int *elem, int pos ) {
  for ( int t = *( elem + pos ); t != pos; t = *( elem + pos ) ) {
    pos = t;
  }
  return pos;
}

static inline int region( const unsigned char *s, unsigned int *d, int h, int w, int ss ) {
  int i, j, m, n;
  
  int kind = 1;
  int c = 0;
  int x = h * w;
  
  int *label = new int[ h * w + 1 ];
  unsigned char *p = new unsigned char[ ( h + 2 ) * ( w + 1 ) ];
  
  if ( NULL == label || NULL == p ) {
    exit( -1 );
  }
  
  // 初始化结果数组 & 区域标识数组
  for ( i = 0; i < x; ++i ) {
    *( d + i ) = 0;
    *( label + i ) = i;
  }
  
  *( label + x ) = x;
  x = x + h + 2 * w + 2;
  
  // p指向的空间全部赋值为0
  for ( i = 0; i < x; ++i ) {
    *( p + i ) = 0;
  }
  
  for ( i = 0; i < h; ++i ) {
    for ( j = 0; j < w; ++j ) {
      *( p + ( i + 1 ) * ( w + 1 ) + j ) = *( s + i * ss + j );
    }
  }
  
  for ( i = 0; i < h; ++i ) {
    for ( j = 0; j < w; ++j ) {
    
      if ( *( p + ( i + 1 ) * ( w + 1 ) + j ) > 0 )                            // 当前点需要分类
      {
        if ( *( d + i * w + j ) == 0 )           // 当前点未被分类
        {
          *( d + i * w + j ) = kind++;
        }
        
        if ( *( p + ( i + 2 ) * ( w + 1 ) + j ) > 0 )         // 下面的点
        {
          *( d + ( i + 1 ) * w + j ) = *( d + i * w + j );
        }
        
        if ( *( p + ( i + 2 ) * ( w + 1 ) + ( j + 1 ) ) > 0 )       // 右下面的点
        {
          *( d + ( i + 1 ) * w + ( j + 1 ) ) = *( d + i * w + j );
        }
        
        if ( *( p + ( i + 1 ) * ( w + 1 ) + ( j + 1 ) ) > 0 )       // 右面的点
        {
          if ( *( d + i * w + j + 1 ) == 0 )         // 未被分类
          {
            *( d + i * w + j + 1 ) = *( d + i * w + j );
          }
          else               // 已经分类
          {
            m = *( d + i * w + j ), n = *( d + i * w + j + 1 );
            
            if ( m != n ) {
              *( label + root( label, n ) ) = root( label, m );
            }
          }
        }
        
        if ( *( p + i * ( w + 1 ) + ( j + 1 ) ) > 0 )         // 右上面的点
        {
          if ( *( d + ( i - 1 ) * w + ( j + 1 ) ) == 0 )       // 未被分类
          {
            *( d + ( i - 1 ) * w + ( j + 1 ) ) = *( d + i * w + j );
          }
          else               // 已经分类
          {
            m = *( d + i * w + j ), n = *( d + ( i - 1 ) * w + ( j + 1 ) );
            
            if ( m != n ) {
              *( label + root( label, n ) ) = root( label, m );
            }
          }
        }
      }
    }
  }
  
  delete [] p;
  p = NULL;
  
  int counter = 1;
  int *px = new int[ kind ];
  
  *px = 0; // 第一个位置赋值
  
  // 任意类别都回溯到对应的根类别，同时统计类别总数
  for ( i = 1; i < kind; ++i ) {
    *( label + i ) = root( label, i );
    
    if ( *( label + i ) == i ) {
      ++c;
      *( px + i ) = counter++;
    }
  }
  
  for ( i = 1; i < kind; ++i ) {
    if ( *( label + i ) != i ) {
      *( px + i ) = *( px + *( label + i ) );
    }
  }
  
  unsigned int xu;
  
  for ( i = 0; i < h; ++i ) {
    for ( j = 0; j < w; ++j ) {
      xu = *( d + i * w + j );
      if ( xu != 0 ) {
        *( d + i * w + j ) = *( px + xu );
      }
    }
  }
  
  delete [] px;
  px = NULL;
  delete [] label;
  label = NULL;
  
  return c;
}

static inline void RegionRectify( unsigned char *pl, int col, int row, int step, unsigned int min_pts ) {
  int c, r;
  unsigned int t, *pt = new unsigned int[ col * row ];  // 存放连通区域标记(结果)的数组
  
  int kind = region( pl, pt, col, row, step );
  
  unsigned int* pn = new unsigned int[ kind + 1 ];   // 存放每个连通区域中点的总个数的数组
  
  for ( c = 0; c < kind + 1; ++c )
    *( pn + c ) = 0;
  for ( c = 0; c < col * row; ++c )
    ++pn[ *( pt + c ) ];
    
  for ( c = 1; c < kind + 1; ++c ) {
    if ( pn[ c ] < min_pts )
      pn[ c ] = 0;
  }
  
  for ( c = 0; c < col; ++c ) {
    for ( r = 0; r < row; ++r ) {
      t = *( pt + c * row + r );
      if ( t != 0 && pn[ t ] == 0 )
        * ( pl + c * step + r ) = 0;
    }
  }
  
  delete [] pn;
  pn = 0;
  delete [] pt;
  pt = 0;
}

static inline void Sobel( const double* s, double* d, int h, int w, int ss, int sd, int method ) {
  double t[ 3 ];
  int i, j;
  
  // 竖直方向
  if ( method == 0 ) {
    for ( i = 1; i < h + 1; ++i ) {
      for ( j = 1; j < w + 1; ++j ) {
        *( t + 0 ) = *( s + ( i - 1 ) * ss + ( j - 1 ) ) - *( s + ( i + 1 ) * ss + ( j - 1 ) );
        *( t + 1 ) = *( s + ( i - 1 ) * ss + j ) - *( s + ( i + 1 ) * ss + j );
        *( t + 2 ) = *( s + ( i - 1 ) * ss + ( j + 1 ) ) - *( s + ( i + 1 ) * ss + ( j + 1 ) );
        
        *( d + ( i - 1 ) * sd + ( j - 1 ) ) = fabs( *( t + 0 ) + *( t + 1 ) + *( t + 1 ) + *( t + 2 ) );
      }
    }
  }
  
  // 水平方向
  else {
    for ( i = 1; i < h + 1; ++i ) {
      for ( j = 1; j < w + 1; ++j ) {
        *( t + 0 ) = *( s + ( i - 1 ) * ss + ( j - 1 ) ) - *( s + ( i - 1 ) * ss + ( j + 1 ) );
        *( t + 1 ) = *( s + i * ss + ( j - 1 ) ) - *( s + i * ss + ( j + 1 ) );
        *( t + 2 ) = *( s + ( i + 1 ) * ss + ( j - 1 ) ) - *( s + ( i + 1 ) * ss + ( j + 1 ) );
        
        *( d + ( i - 1 ) * sd + ( j - 1 ) ) = fabs( *( t + 0 ) + *( t + 1 ) + *( t + 1 ) + *( t + 2 ) );
      }
    }
  }
}

template <class _II, class IO, class _T>
inline
void Erode( _II s, IO d, _T h, _T w, _T ds, _T dd ) {
  for ( int i = 0; i < h; ++i ) {
    for ( int j = 0; j < w; ++j ) {
      _II t = s + i * ds + j;
      
      if ( *( t - ds - 1 ) == 0 || *( t - ds - 0 ) == 0 || *( t - ds + 1 ) == 0 || *( t - 1 ) == 0 || *( t + 1 ) == 0 || *( t + ds - 1 ) == 0 || *( t + ds - 0 ) == 0 || *( t + ds + 1 ) == 0 ) {
        *( d + i * dd + j ) = 0;
      }
      else {
        *( d + i * dd + j ) = *t;
      }
    }
  }
}

template <class _II, class IO, class _T>
inline
void Expand( _II s, IO d, _T h, _T w, _T ds, _T dd ) {
  for ( int i = 0; i < h; ++i ) {
    for ( int j = 0; j < w; ++j ) {
      _II t = s + i * ds + j;
      
      if ( *( t - ds - 1 ) != 0 || *( t - ds - 0 ) != 0 || *( t - ds + 1 ) != 0 || *( t - 1 ) != 0 || *( t + 1 ) != 0 || *( t + ds - 1 ) != 0 || *( t + ds - 0 ) != 0 || *( t + ds + 1 ) != 0 ) {
        *( d + i * dd + j ) = 1;
      }
      else {
        *( d + i * dd + j ) = *t;
      }
    }
  }
}

// 消除由光线突变引起的误检，其中it是灰度化的检测图片，ib是灰度化的背景图片
void RectifyLight( const unsigned char *pt, int step, double *it, double *ib, unsigned char* label, int row, int col ) {
  double dSum=0;
  int i; for (i=0; i<row * col; ++i) dSum+=label[i];
  // 判断是否需要做光照突变修正
  if ( dSum > ( 0.25 * row * col ) ) {
    double xa, xb, t[ 3 ];
    int i, j, k, p, q;
    
    double* itx = new double[ ( row + 2 ) * ( col + 2 ) ];
    double* ibx = new double[ ( row + 2 ) * ( col + 2 ) ];
    double* gx0 = new double[ row * col ];
    double* gy0 = new double[ row * col ];
    double* gx1 = new double[ row * col ];
    double* gy1 = new double[ row * col ];
    
    for ( i = 0; i < ( row + 2 ) * ( col + 2 ); ++i ) {
      *( itx + i ) = 0.0;
      *( ibx + i ) = 0.0;
    }
    
    for ( i = 0; i < row; ++i ) {
      for ( j = 0; j < col; ++j ) {
        *( itx + ( i + 1 ) * ( col + 2 ) + ( j + 1 ) ) = *( it + i * col + j );
        *( ibx + ( i + 1 ) * ( col + 2 ) + ( j + 1 ) ) = *( ib + i * col + j );
      }
    }
    
    Sobel( itx, gy0, row, col, col + 2, col, 0 );
    Sobel( itx, gx0, row, col, col + 2, col, 1 );
    Sobel( ibx, gy1, row, col, col + 2, col, 0 );
    Sobel( ibx, gx1, row, col, col + 2, col, 1 );
    
    for ( i = 0; i < row; ++i ) {
      for ( j = 0; j < col; ++j ) {
        k = i * col + j;
        
        xa = *( gx0 + k ) * *( gx1 + k ) + *( gy0 + k ) * *( gy1 + k ) + *( gx0 + k ) * *( gx1 + k ) + *( gy0 + k ) * *( gy1 + k );
        xb = *( gx0 + k ) * *( gx0 + k ) + *( gy0 + k ) * *( gy0 + k ) + *( gx1 + k ) * *( gx1 + k ) + *( gy1 + k ) * *( gy1 + k );
        
        *( itx + ( i + 1 ) * ( col + 2 ) + ( j + 1 ) ) = xa;
        *( ibx + ( i + 1 ) * ( col + 2 ) + ( j + 1 ) ) = xb;
      }
    }
    
    // 上一个循环结束后就已经可以释放掉不需要再用的动态分配内存
    delete [] gx1;
    gx1 = 0;
    delete [] gy1;
    gy1 = 0;
    delete [] gx0;
    gx0 = 0;
    delete [] gy0;
    gy0 = 0;
    
    // 寻找被判定为前景的象素点
    int *r = new int[ row * col ], *c = new int[ row * col ], counter = 0;
    
    for ( i = 0; i < row; ++i ) {
      for ( j = 0; j < col; ++j ) {
        if ( *( label + i * col + j ) != 0 ) {
          *( r + counter ) = i;
          *( c + counter++ ) = j;
        }
      }
    }
    
    // 修正
    for ( i = 0; i < counter; ++i ) {
      p = *( r + i );
      q = *( c + i );
      
      *( t + 0 ) = *( itx + ( p + 0 ) * ( col + 2 ) + q ) + *( itx + ( p + 0 ) * ( col + 2 ) + ( q + 1 ) ) + *( itx + ( p + 0 ) * ( col + 2 ) + ( q + 2 ) );
      *( t + 1 ) = *( itx + ( p + 1 ) * ( col + 2 ) + q ) + *( itx + ( p + 1 ) * ( col + 2 ) + ( q + 1 ) ) + *( itx + ( p + 1 ) * ( col + 2 ) + ( q + 2 ) );
      *( t + 2 ) = *( itx + ( p + 2 ) * ( col + 2 ) + q ) + *( itx + ( p + 2 ) * ( col + 2 ) + ( q + 1 ) ) + *( itx + ( p + 2 ) * ( col + 2 ) + ( q + 2 ) );
      
      xa = *( t + 0 ) + *( t + 1 ) + *( t + 2 );
      
      *( t + 0 ) = *( ibx + ( p + 0 ) * ( col + 2 ) + q ) + *( ibx + ( p + 0 ) * ( col + 2 ) + ( q + 1 ) ) + *( ibx + ( p + 0 ) * ( col + 2 ) + ( q + 2 ) );
      *( t + 1 ) = *( ibx + ( p + 1 ) * ( col + 2 ) + q ) + *( ibx + ( p + 1 ) * ( col + 2 ) + ( q + 1 ) ) + *( ibx + ( p + 1 ) * ( col + 2 ) + ( q + 2 ) );
      *( t + 2 ) = *( ibx + ( p + 2 ) * ( col + 2 ) + q ) + *( ibx + ( p + 2 ) * ( col + 2 ) + ( q + 1 ) ) + *( ibx + ( p + 2 ) * ( col + 2 ) + ( q + 2 ) );
      
      xb = *( t + 0 ) + *( t + 1 ) + *( t + 2 );
      
      ( xb > 0 ) ? ( xa /= xb ) : ( xa = 1.0 );
      
      if ( xa > 0.7 )
        * ( label + p * col + q ) = 0;
    }
    
    // 替代matlab代码中的数组a和b，使用完后才能释放
    delete [] r;
    r = 0;
    delete [] c;
    c = 0;
    delete [] itx;
    itx = 0;
    delete [] ibx;
    ibx = 0;
  }
}

// 基于NCC的去阴影方法
void RectifyShadow( const unsigned char *pt, int step, const double *b, const double *it, const double *ib, unsigned char *label, int row, int col ) {
  double B[ 81 ], T[ 81 ], et, eb, er, ncc;
  int i, j, k, p, q;
  
  double *itx = new double[ ( row + 8 ) * ( col + 8 ) ];
  double *ibx = new double[ ( row + 8 ) * ( col + 8 ) ];
  double *itb = new double[ row * col ];
  
  // 初始化动态分配的内存
  for ( i = 0; i < ( row + 8 ) * ( col + 8 ); ++i ) {
    *( itx + i ) = 0.0;
    *( ibx + i ) = 0.0;
  }
  
  for ( i = 0; i < row * col; ++i )
    *( itb + i ) = 0.0;
    
  for ( i = 0; i < row; ++i ) {
    for ( j = 0; j < col; ++j ) {
      *( itx + ( i + 4 ) * ( col + 8 ) + ( j + 4 ) ) = *( it + i * col + j );
      *( ibx + ( i + 4 ) * ( col + 8 ) + ( j + 4 ) ) = *( ib + i * col + j );
    }
  }
  
  // 给itb赋值
  for ( i = 0; i < row * col; ++i ) {
    if ( *( ib + i ) > 0 ) {
      *( itb + i ) = *( it + i ) / *( ib + i );
    }
  }
  
  // 寻找被判定为前景的象素点
  int *r = new int[ row * col ];
  int *c = new int[ row * col ];
  int counter = 0;
  
  for ( i = 0; i < row; ++i ) {
    for ( j = 0; j < col; ++j ) {
      if ( *( label + i * col + j ) != 0 ) {
        *( r + counter ) = i;
        *( c + counter++ ) = j;
      }
    }
  }
  
  // 修正
  for ( i = 0; i < counter; ++i ) {
    p = *( r + i );
    q = *( c + i );
    
    for ( j = 0; j < 9; ++j ) {
      *( B + j * 9 + 0 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 0 ) );
      *( T + j * 9 + 0 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 0 ) );
      *( B + j * 9 + 1 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 1 ) );
      *( T + j * 9 + 1 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 1 ) );
      *( B + j * 9 + 2 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 2 ) );
      *( T + j * 9 + 2 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 2 ) );
      *( B + j * 9 + 3 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 3 ) );
      *( T + j * 9 + 3 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 3 ) );
      *( B + j * 9 + 4 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 4 ) );
      *( T + j * 9 + 4 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 4 ) );
      *( B + j * 9 + 5 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 5 ) );
      *( T + j * 9 + 5 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 5 ) );
      *( B + j * 9 + 6 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 6 ) );
      *( T + j * 9 + 6 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 6 ) );
      *( B + j * 9 + 7 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 7 ) );
      *( T + j * 9 + 7 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 7 ) );
      *( B + j * 9 + 8 ) = *( ibx + ( p + j ) * ( col + 8 ) + ( q + 8 ) );
      *( T + j * 9 + 8 ) = *( itx + ( p + j ) * ( col + 8 ) + ( q + 8 ) );
    }
    
    et = eb = er = 0.0;
    
    for ( k = 0; k < 81; ++k ) {
      et += ( *( T + k ) * *( T + k ) );
      eb += ( *( B + k ) * *( B + k ) );
      er += ( *( B + k ) * *( T + k ) );
    }
    
    et = sqrt( et );
    eb = sqrt( eb );
    
    ncc = er / ( et * eb );
    
    if ( ncc > 0.95 && et < eb && *( itb + p * col + q ) < 1.0 ) {
      *( label + p * col + q ) = 0;
    }
    
    if ( *( label + p * col + q ) == 0 ) {
      et = *( pt + p * step + q * 3 + 0 ) / *( b + p * col * 3 + q * 3 + 0 );
      eb = *( pt + p * step + q * 3 + 1 ) / *( b + p * col * 3 + q * 3 + 1 );
      er = *( pt + p * step + q * 3 + 2 ) / *( b + p * col * 3 + q * 3 + 2 );
      
      ncc = ( et + eb + er ) / 3.0;
      
      ncc = sqrt( ( ( et - ncc ) * ( et - ncc ) + ( eb - ncc ) * ( eb - ncc ) + ( er - ncc ) * ( er - ncc ) ) / 2 );
      
      if ( ncc < 0.05 && et < 1 && eb < 1 && er < 1 ) {
        *( label + p * col + q ) = 0;
      } else {
        *( label + p * col + q ) = 1;
      }
    }
  }
  
  delete [] r;
  r = 0;
  delete [] c;
  c = 0;
  delete [] itx;
  itx = 0;
  delete [] ibx;
  ibx = 0;
  delete [] itb;
  itb = 0;
}

void shadow( const uchar *src, int step, const double *b, const double *it, const double *ib, uchar *label, int r, int c, const double *shadow_p ) {
  double B[ 81 ], T[ 81 ], TB[ 9 ];
  
  const double th_NCC = *( shadow_p );
  const double L_std = *( shadow_p + 1 );
  const double L_low = *( shadow_p + 2 );
  const double C_th = *( shadow_p + 3 );
  const int len = r * c;
  const int lenx = ( r + 8 ) * ( c + 8 );
  
  int i, j, k, l;
  
  uchar *shad = new uchar[ len ];
  init( shad, ( uchar ) 0, len );
  
  // first 3 steps
  double *ibx = new double[ lenx ];
  double *itx = new double[ lenx ];
  
  init( ibx, 0.0, lenx );
  init( itx, 0.0, lenx );
  
  for ( i = 0; i < r; ++i ) {
    for ( j = 0; j < c; ++j ) {
      *( itx + ( i + 4 ) * ( c + 8 ) + ( j + 4 ) ) = *( it + i * c + j );
      *( ibx + ( i + 4 ) * ( c + 8 ) + ( j + 4 ) ) = *( ib + i * c + j );
    }
  }
  
  for ( i = 0; i < r; ++i ) {
    for ( j = 0; j < c; ++j ) {
      if ( *( label + i * c + j ) > 0 ) {
        // first step
        const int size0 = i * step + j * 3;
        const int size1 = i * c * 3 + j * 3;
        
        if ( *( src + size0 ) < *( b + size1 ) &&
             *( src + size0 + 1 ) < *( b + size1 + 1 ) &&
             *( src + size0 + 2 ) < *( b + size1 + 2 ) ) {
          // condition for ncc; second step
          // *(shad + i * c + j) = '\1';
          for ( k = 0; k < 9; ++k ) {
            *( B + k * 9 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j ) );
            *( B + k * 9 + 1 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j + 1 ) );
            *( B + k * 9 + 2 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j + 2 ) );
            *( B + k * 9 + 3 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j + 3 ) );
            *( B + k * 9 + 4 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j + 4 ) );
            *( B + k * 9 + 5 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j + 5 ) );
            *( B + k * 9 + 6 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j + 6 ) );
            *( B + k * 9 + 7 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j + 7 ) );
            *( B + k * 9 + 8 ) = *( ibx + ( i + k ) * ( c + 8 ) + ( j + 8 ) );
            
            *( T + k * 9 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j ) );
            *( T + k * 9 + 1 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j + 1 ) );
            *( T + k * 9 + 2 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j + 2 ) );
            *( T + k * 9 + 3 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j + 3 ) );
            *( T + k * 9 + 4 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j + 4 ) );
            *( T + k * 9 + 5 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j + 5 ) );
            *( T + k * 9 + 6 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j + 6 ) );
            *( T + k * 9 + 7 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j + 7 ) );
            *( T + k * 9 + 8 ) = *( itx + ( i + k ) * ( c + 8 ) + ( j + 8 ) );
          }
#if 0
          const double er = InnerProduct( B, B + 81, T, 0.0 );
          const double eb = InnerProduct( B, B + 81, B, 0.0 );
          const double et = InnerProduct( T, T + 81, T, 0.0 );
#else
          double er=0, eb=0, et=0;
          for (int ii=0; ii<91; ++ii) {
            er+=B[ii]*T[ii];
            eb+=B[ii]*B[ii];
            et+=T[ii]*T[ii];
          }
#endif
          
          const double ncc = er / sqrt( eb * et );
          
          if ( eb > et && ncc >= th_NCC ) {
            // condition for more accurate judgement
            // *(shad + i * c + j) = '\1';
            {
              *( TB ) = *( T + 30 ) / ( *( B + 30 ) + 0.0000000001 );
              *( TB + 1 ) = *( T + 31 ) / ( *( B + 31 ) + 0.0000000001 );
              *( TB + 2 ) = *( T + 32 ) / ( *( B + 32 ) + 0.0000000001 );
              *( TB + 3 ) = *( T + 39 ) / ( *( B + 39 ) + 0.0000000001 );
              *( TB + 4 ) = *( T + 40 ) / ( *( B + 40 ) + 0.0000000001 );
              *( TB + 5 ) = *( T + 41 ) / ( *( B + 41 ) + 0.0000000001 );
              *( TB + 6 ) = *( T + 48 ) / ( *( B + 48 ) + 0.0000000001 );
              *( TB + 7 ) = *( T + 49 ) / ( *( B + 49 ) + 0.0000000001 );
              *( TB + 8 ) = *( T + 50 ) / ( *( B + 50 ) + 0.0000000001 );
            }
            
            const double prop_ij = *( TB + 4 );
            const double avg = (TB[0]+TB[1]+TB[2]+TB[3]+TB[4]+TB[5]+TB[6]+TB[7]+TB[8])/9.0;
            
            {
              TB[0] -= avg;
              TB[1] -= avg;
              TB[2] -= avg;
              TB[3] -= avg;
              TB[4] -= avg;
              TB[5] -= avg;
              TB[6] -= avg;
              TB[7] -= avg;
              TB[8] -= avg;
            }
            
            //const double sum = InnerProduct( TB, TB + 9, TB, 0.0 );
            const double sum = TB[0]*TB[0]+TB[1]*TB[1]+TB[2]*TB[2]+TB[3]*TB[3]+TB[4]*TB[4]+TB[5]*TB[5]+TB[6]*TB[6]+TB[7]*TB[7]+TB[8]*TB[8];
            const double std_avg = sqrt( sum / 8.0 );
            
            if ( prop_ij < 1.0 && prop_ij > L_low && std_avg < L_std ) {
              *( shad + i * c + j ) = 1;
            }
          }
        }
      }
    }
  }
  
  delete [] ibx;
  delete [] itx;
  
  // the fourth step
  const int lenz = ( r + 4 ) * ( c + 4 ) * 3;
  
  ibx = new double[ lenz ];
  itx = new double[ lenz ];
  
  double ibo[ 75 ];
  double ito[ 75 ];
  
  init( ibx, 0.0, lenz );
  init( itx, 0.0, lenz );
  
  for ( i = 0; i < r; ++i ) {
    for ( j = 0; j < c; ++j ) {
      *( itx + ( i + 2 ) * ( c + 4 ) * 3 + ( j + 2 ) * 3 ) = *( src + i * step + j * 3 );
      *( itx + ( i + 2 ) * ( c + 4 ) * 3 + ( j + 2 ) * 3 + 1 ) = *( src + i * step + j * 3 + 1 );
      *( itx + ( i + 2 ) * ( c + 4 ) * 3 + ( j + 2 ) * 3 + 2 ) = *( src + i * step + j * 3 + 2 );
      
      *( ibx + ( i + 2 ) * ( c + 4 ) * 3 + ( j + 2 ) * 3 ) = *( b + i * c * 3 + j * 3 );
      *( ibx + ( i + 2 ) * ( c + 4 ) * 3 + ( j + 2 ) * 3 + 1 ) = *( b + i * c * 3 + j * 3 + 1 );
      *( ibx + ( i + 2 ) * ( c + 4 ) * 3 + ( j + 2 ) * 3 + 2 ) = *( b + i * c * 3 + j * 3 + 2 );
    }
  }
  
  for ( i = 0; i < r; ++i ) {
    for ( j = 0; j < c; ++j ) {
      if ( 1 == *( shad + i * c + j ) ) {
        for ( k = 0; k < 5; ++k ) {
          *( ito + k * 15 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 );
          *( ito + k * 15 + 1 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 1 );
          *( ito + k * 15 + 2 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 2 );
          
          *( ito + k * 15 + 3 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 3 );
          *( ito + k * 15 + 4 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 4 );
          *( ito + k * 15 + 5 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 5 );
          
          *( ito + k * 15 + 6 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 6 );
          *( ito + k * 15 + 7 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 7 );
          *( ito + k * 15 + 8 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 8 );
          
          *( ito + k * 15 + 9 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 9 );
          *( ito + k * 15 + 10 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 10 );
          *( ito + k * 15 + 11 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 11 );
          
          *( ito + k * 15 + 12 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 12 );
          *( ito + k * 15 + 13 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 13 );
          *( ito + k * 15 + 14 ) = *( itx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 14 );
          
          *( ibo + k * 15 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 );
          *( ibo + k * 15 + 1 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 1 );
          *( ibo + k * 15 + 2 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 2 );
          
          *( ibo + k * 15 + 3 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 3 );
          *( ibo + k * 15 + 4 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 4 );
          *( ibo + k * 15 + 5 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 5 );
          
          *( ibo + k * 15 + 6 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 6 );
          *( ibo + k * 15 + 7 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 7 );
          *( ibo + k * 15 + 8 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 8 );
          
          *( ibo + k * 15 + 9 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 9 );
          *( ibo + k * 15 + 10 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 10 );
          *( ibo + k * 15 + 11 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 11 );
          
          *( ibo + k * 15 + 12 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 12 );
          *( ibo + k * 15 + 13 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 13 );
          *( ibo + k * 15 + 14 ) = *( ibx + ( i + k ) * ( c + 4 ) * 3 + ( j ) * 3 + 14 );
        }
        
        double c0 = 0.0, c1 = 0.0, c2 = 0.0;
        
        for ( k = 0; k < 5; ++k ) {
          for ( l = 0; l < 5; ++l ) {
            const double t0 = *( ito + k * 15 + l * 3 );
            const double t1 = *( ito + k * 15 + l * 3 + 1 );
            const double t2 = *( ito + k * 15 + l * 3 + 2 );
            
            const double b0 = *( ibo + k * 15 + l * 3 );
            const double b1 = *( ibo + k * 15 + l * 3 + 1 );
            const double b2 = *( ibo + k * 15 + l * 3 + 2 );
            
            {
              c0 += ( t0 / ( t1 > t2 ? t1 + 0.0000000001 : t2 + 0.0000000001 ) - b0 / ( b1 > b2 ? b1 + 0.0000000001 : b2 + 0.0000000001 ) );
              c1 += ( t1 / ( t0 > t2 ? t0 + 0.0000000001 : t2 + 0.0000000001 ) - b1 / ( b0 > b2 ? b0 + 0.0000000001 : b2 + 0.0000000001 ) );
              c2 += ( t2 / ( t0 > t1 ? t0 + 0.0000000001 : t1 + 0.0000000001 ) - b2 / ( b0 > b1 ? b0 + 0.0000000001 : b1 + 0.0000000001 ) );
            }
          }
        }
        
        c0 /= 25.0;
        c1 /= 25.0;
        c2 /= 25.0;
        
        if ( c0 > C_th || c1 > C_th || c2 > C_th ) {
          *( shad + i * c + j ) = 0;
        }
      }
    }
  }
  
  delete [] ibx;
  ibx = 0;
  delete [] itx;
  itx = 0;
  
  uchar *plabel = label;
  uchar *pshad = shad;
  
  for ( ; pshad < shad + len; ++plabel, ++pshad ) {
    if ( *( plabel ) > 0 && *( pshad ) > 0 ) {
      *( plabel ) = 0;
    }
  }
  
  delete [] shad;
  shad = 0;
}


// 3.1 高斯背景模型建立.
// * 每次传入一帧RGB模式的图像数据, 连续对n帧图像调用此函数完成背景模型的建立, n的建议值为500.
// * pGmm: 指向高斯背景建模对象的指针.
// * src : RGB模式图像数据的头指针.
// * step: 图像数据步长.
// * num : 当前传入帧的序号, 例如第一帧num值为1.
void GMM::SetupBG( const unsigned char *pt, int step, int num ) {
  double t[ 9 ], ts[ 9 ], w1[ 3 ], w2[ 3 ], t0, theta;
  int i, j, k, x, y, z, index[ 3 ];
  
  // 传入第num(num > 1)张图片
  if ( num > 1 ) {
    for ( i = 0; i < row; ++i ) {
      int ic = i* col * kind;
      for ( j = 0; j < col; ++j ) {
        k = *( s + i * col + j );
        
        for ( x = 0; x < k; ++x ) {
          *( ts + x * 3 + 0 ) = *( pt + i * step + j * 3 + 0 ) - *( m + ic * 3 + j * kind * 3 + x * 3 + 0 );
          *( ts + x * 3 + 1 ) = *( pt + i * step + j * 3 + 1 ) - *( m + ic * 3 + j * kind * 3 + x * 3 + 1 );
          *( ts + x * 3 + 2 ) = *( pt + i * step + j * 3 + 2 ) - *( m + ic * 3 + j * kind * 3 + x * 3 + 2 );
          
          *( t + x * 3 + 0 ) = fabs( *( ts + x * 3 + 0 ) );
          *( t + x * 3 + 1 ) = fabs( *( ts + x * 3 + 1 ) );
          *( t + x * 3 + 2 ) = fabs( *( ts + x * 3 + 2 ) );
        }
        
        if ( k == 1 ) {
          t0 = 2.5 * *( v + ic + j * kind + 0 );
          
          if ( *( t + 0 ) < t0 && *( t + 1 ) < t0 && *( t + 2 ) < t0 ) {
            *( w + ic + j * kind + 0 ) += ( afa - afa * *( w + ic + j * kind + 0 ) );
            
            theta = afa / *( w + ic + j * kind + 0 );
            
            *( m + ic * 3 + j * kind * 3 + 0 ) += ( theta * *( ts + 0 ) );
            *( m + ic * 3 + j * kind * 3 + 1 ) += ( theta * *( ts + 1 ) );
            *( m + ic * 3 + j * kind * 3 + 2 ) += ( theta * *( ts + 2 ) );
            
            t0 = *( v + ic + j * kind + 0 );
            double tp = t[0]*t[0]+t[1]*t[1]+t[2]*t[2];
            //tp = InnerProduct( t, t + 3, t, 0.0 );
            *( v + ic + j * kind + 0 ) = sqrt( fabs( ( 1 - theta ) * ( t0 * t0 ) + theta * tp ) );
          }
          else {
            *( w + ic + j * kind + 0 ) = ( 1 - afa ) * *( w + ic + j * kind + 0 );
            *( w + ic + j * kind + 1 ) = vir_w;
            *( v + ic + j * kind + 1 ) = vir_v;
            *( m + ic * 3 + j * kind * 3 + 3 ) = *( pt + i * step + j * 3 + 0 );
            *( m + ic * 3 + j * kind * 3 + 4 ) = *( pt + i * step + j * 3 + 1 );
            *( m + ic * 3 + j * kind * 3 + 5 ) = *( pt + i * step + j * 3 + 2 );
            
            *( s + i * col + j ) = 2;
          }
        }
        
        // k != 1
        else {
          // get index
          for ( x = 0; x < k; ++x ) {
            t0 = *( w + ic + j * kind + x ) / *( v + ic + j * kind + x );
            *( w1 + x ) = t0;
            *( w2 + x ) = t0;
          }
          
          sort( w2, k );
          
          for ( x = 0, z = 0; x < k; ++x ) {
            for ( y = 0; y < k; ++y ) {
              if ( fabs( *( w1 + y ) - *( w2 + x ) ) < 1e-99 ) {
                *( index + z++ ) = y;
                break;
              }
            }
          }
          // get index end
          for ( x = 0, y = 0; y < k; ++y ) {
            z = *( index + y );
            t0 = 2.5 * *( v + ic + j * kind + z );
            
            if ( x == 0 && *( t + z * 3 + 0 ) < t0 && *( t + z * 3 + 1 ) < t0 && *( t + z * 3 + 2 ) < t0 ) {
              x = 1;
              *( w + ic + j * kind + z ) = ( 1 - afa ) * *( w + ic + j * kind + z ) + afa;
              theta = afa / *( w + ic + j * kind + z );
              
              *( m + ic * 3 + j * kind * 3 + z * 3 + 0 ) += theta * *( ts + z * 3 + 0 );
              *( m + ic * 3 + j * kind * 3 + z * 3 + 1 ) += theta * *( ts + z * 3 + 1 );
              *( m + ic * 3 + j * kind * 3 + z * 3 + 2 ) += theta * *( ts + z * 3 + 2 );
              
              t0 = *( v + ic + j * kind + z );
              double* tz = t + z * 3;
              double tp = tz[0]*tz[0]+tz[1]*tz[1]+tz[2]*tz[2];
              //tp = InnerProduct( t + z * 3, t + ( z + 1 ) * 3, t + z * 3, 0.0 );
              *( v + ic + j * kind + z ) = sqrt( fabs( ( 1 - theta ) * ( t0 * t0 ) + theta * tp ) );
            }
            else {
              *( w + ic + j * kind + z ) = ( 1 - afa ) * *( w + ic + j * kind + z );
            }
          }
          
          if ( x == 0 ) {
            if ( k == 3 ) {
              // find min w/v
              z = 0;
              t0 = *( w1 + z );
              
              for ( y = 1; y < k; ++y ) {
                if ( *( w1 + y ) < t0 ) {
                  t0 = *( w1 + y );
                  z = y;
                }
              }
              
              *( w + ic + j * kind + z ) = vir_w;
              *( v + ic + j * kind + z ) = vir_v;
              *( m + ic * 3 + j * kind * 3 + z * 3 + 0 ) = *( pt + i * step + j * 3 + 0 );
              *( m + ic * 3 + j * kind * 3 + z * 3 + 1 ) = *( pt + i * step + j * 3 + 1 );
              *( m + ic * 3 + j * kind * 3 + z * 3 + 2 ) = *( pt + i * step + j * 3 + 2 );
            }
            else if ( k == 2 ) {
              *( w + ic + j * kind + 2 ) = vir_w;
              *( v + ic + j * kind + 2 ) = vir_v;
              *( m + ic * 3 + j * kind * 3 + 6 ) = *( pt + i * step + j * 3 + 0 );
              *( m + ic * 3 + j * kind * 3 + 7 ) = *( pt + i * step + j * 3 + 1 );
              *( m + ic * 3 + j * kind * 3 + 8 ) = *( pt + i * step + j * 3 + 2 );
              *( s + i * col + j ) = 3;
            }
          }
        }
        
        // 权重归一
        {
          //t0 = Sum( w + ic + j * kind, w + ic + ( j + 1 ) * kind, 0.0 );
          t0=0;
          for (int ii=0; ii<kind; ++ii) t0+=w[ic + j * kind+ii];
        }
        *( w + ic + j * kind + 0 ) /= t0;
        *( w + ic + j * kind + 1 ) /= t0;
        *( w + ic + j * kind + 2 ) /= t0;
      }
    }
  }
  
  // 传入第一张图片的数据
  else {
    SetS();
    
    for ( i = 0; i < row; ++i ) {
      int ic = i* col * kind;
      for ( j = 0; j < col; ++j ) {
        *( w + ic + j * kind ) = 1.0;
        *( v + ic + j * kind ) = vir_v;
        
        *( m + ic * 3 + j * kind * 3 + 0 ) = *( pt + i * step + j * 3 + 0 );
        *( m + ic * 3 + j * kind * 3 + 1 ) = *( pt + i * step + j * 3 + 1 );
        *( m + ic * 3 + j * kind * 3 + 2 ) = *( pt + i * step + j * 3 + 2 );
        
        *( s + i * col + j ) = 1;
      }
    }
  }
}

void GMM::Export2File( const char* wf, const char* mf, const char* vf ) const {
  FILE * pf0 = fopen( wf, "w" );
  FILE* pf1 = fopen( mf, "w" );
  FILE* pf2 = fopen( vf, "w" );
  
  for ( int r = 0; r < row; ++r ) {
    for ( int c = 0; c < ( int ) col; ++c ) {
      fprintf( pf0, "%1.9f %1.9f %1.9f\n", *( w + ( r * col + c ) * kind + 0 ), *( w + ( r * col + c ) * kind + 1 ), *( w + ( r * col + c ) * kind + 2 ) );
      fprintf( pf2, "%1.6f %1.6f %1.6f\n", *( v + ( r * col + c ) * kind + 0 ), *( v + ( r * col + c ) * kind + 1 ), *( v + ( r * col + c ) * kind + 2 ) );
      
      for ( int l = 0; l < kind; ++l ) {
        fprintf( pf1, "%1.6f %1.6f %1.6f   ", *( m + ( ( r * col + c ) * kind + l ) * 3 + 2 ), *( m + ( ( r * col + c ) * kind + l ) * 3 + 1 ), *( m + ( ( r * col + c ) * kind + l ) * 3 + 0 ) );
      }
      fprintf( pf1, "\n" );
    }
  }
  
  fclose( pf0 );
  fclose( pf1 );
  fclose( pf2 );
}

void GMM::GetIb() {
  int index, ti, k;
  double *pd0, *pd1, t;
  
  for ( int r = 0; r < row; ++r ) {
    for ( int c = 0; c < col; ++c ) {
      ti = ( r * col + c ) * kind;
      pd0 = w + ti;
      pd1 = v + ti;
      k = *( s + r * col + c );
      
      index = 0;
      t = *( pd0 + 0 ) / *( pd1 + 0 );
      
      for ( int in = 1; in < k; ++in ) {
        if ( *( pd0 + in ) / *( pd1 + in ) > t ) {
          index = in;
          t = *( pd0 + in ) / *( pd1 + in );
        }
      }
      
      pd1 = b + ( r * col + c ) * 3;
      pd0 = m + ( ti << 2 ) - ti + ( index << 2 ) - index;
      
      *( pd1 + 0 ) = *( pd0 + 0 );
      *( pd1 + 1 ) = *( pd0 + 1 );
      *( pd1 + 2 ) = *( pd0 + 2 );
    }
  }
}

void GMM::DetectForeGround( const unsigned char *pt, int step, const unsigned char *pmask ) {
  int i, j, k, r, x, y, z, index[ 3 ];
  double t[ 9 ], ts[ 9 ], w1[ 3 ], w2[ 3 ], t0, sum, theta;
  
  if ( pt != 0 ) {
    GetIb();
    
    for ( i = 0; i < row; ++i ) {
      int ic = i* col * kind;
      for ( j = 0; j < col; ++j ) {
        *( label + i * col + j ) = 0;
        
        if ( pmask == 0 || *( pmask + i * col + j ) != 0 ) {
          r = 0;
          k = *( s + i * col + j );
          
          const unsigned char rgb0 = *( pt + i * step + j * 3 );
          const unsigned char rgb1 = *( pt + i * step + j * 3 + 1 );
          const unsigned char rgb2 = *( pt + i * step + j * 3 + 2 );
          
          for ( x = 0; x < k; ++x ) {
            {
              *( ts + x * 3 + 0 ) = rgb0 - *( m + ic * 3 + j * kind * 3 + x * 3 + 0 );
              *( ts + x * 3 + 1 ) = rgb1 - *( m + ic * 3 + j * kind * 3 + x * 3 + 1 );
              *( ts + x * 3 + 2 ) = rgb2 - *( m + ic * 3 + j * kind * 3 + x * 3 + 2 );
            }
            {
              *( t + x * 3 + 0 ) = fabs( *( ts + x * 3 + 0 ) );
              *( t + x * 3 + 1 ) = fabs( *( ts + x * 3 + 1 ) );
              *( t + x * 3 + 2 ) = fabs( *( ts + x * 3 + 2 ) );
            }
            
            t0 = 2.5 * *( v + ic + j * kind + x );
            
            if ( r != 1 && *( t + x * 3 + 0 ) < t0 && *( t + x * 3 + 1 ) < t0 && *( t + x * 3 + 2 ) < t0 )
              r = 1;
          }
          
          // get index
          for ( x = 0; x < k; ++x ) {
            t0 = *( w + ic + j * kind + x ) / *( v + ic + j * kind + x );
            
            *( w1 + x ) = t0;
            *( w2 + x ) = t0;
          }
          
          sort( w2, k );
          z = 0;
          
          for ( x = 0; x < k; ++x ) {
            for ( y = 0; y < k; ++y ) {
              if ( fabs( *( w1 + y ) - *( w2 + x ) ) < 1e-99 ) {
                *( index + z++ ) = y;
                break;
              }
            }
          }
          // get index end
          sum = *( w + ic + j * kind + *( index + 0 ) );
          z = 1;
          
          while ( sum < thrd && z < k )
            sum += *( w + ic + j * kind + *( index + z++ ) );
            
          for ( x = 0, y = 0; x < z; ++x ) {
            t0 = 2.5 * *( v + ic + j * kind + *( index + x ) );
            
            if ( *( t + *( index + x ) * 3 + 0 ) > t0 || *( t + *( index + x ) * 3 + 1 ) > t0 || *( t + *( index + x ) * 3 + 2 ) > t0 )
              ++y;
          }
          
          if ( y == z )
            * ( label + i * col + j ) = 1;
            
          // 匹配
          if ( r == 1 ) {
            for ( z = 0, x = 0; x < k; ++x ) {
              t0 = *( v + ic + j * kind + *( index + x ) ) * 2.5;
              
              if ( z == 0 && *( t + *( index + x ) * 3 + 0 ) < t0 && *( t + *( index + x ) * 3 + 1 ) < t0 && *( t + *( index + x ) * 3 + 2 ) < t0 ) {
                z = 1;
                
                *( w + ic + j * kind + *( index + x ) ) = ( 1 - afa ) * *( w + ic + j * kind + *( index + x ) ) + afa;
                
                theta = afa / *( w + ic + j * kind + *( index + x ) );
                
                *( m + ic * 3 + j * kind * 3 + *( index + x ) * 3 + 0 ) += ( theta * *( ts + *( index + x ) * 3 + 0 ) );
                *( m + ic * 3 + j * kind * 3 + *( index + x ) * 3 + 1 ) += ( theta * *( ts + *( index + x ) * 3 + 1 ) );
                *( m + ic * 3 + j * kind * 3 + *( index + x ) * 3 + 2 ) += ( theta * *( ts + *( index + x ) * 3 + 2 ) );
                
                t0 = *( v + ic + j * kind + *( index + x ) );
                double* tz = t + *( index + x ) * 3;
                double tp = tz[0]*tz[0]+tz[1]*tz[1]+tz[2]*tz[2];
                //tp = InnerProduct( t + *( index + x ) * 3, t + ( 1 + *( index + x ) ) * 3, t + *( index + x ) * 3, 0.0 );
                *( v + ic + j * kind + *( index + x ) ) = sqrt( fabs( ( 1 - theta ) * ( t0 * t0 ) + theta * tp ) );
              }
              else {
                *( w + ic + j * kind + *( index + x ) ) = ( 1 - afa ) * *( w + ic + j * kind + *( index + x ) );
              }
            }
          }
          
          // 不匹配
          else {
            if ( k == 3 ) {
              z = *( index + 2 );
              
              *( w + ic + j * kind + z ) = vir_w;
              *( v + ic + j * kind + z ) = vir_v;
              *( m + ic * 3 + j * kind * 3 + z * 3 + 0 ) = rgb0;
              *( m + ic * 3 + j * kind * 3 + z * 3 + 1 ) = rgb1;
              *( m + ic * 3 + j * kind * 3 + z * 3 + 2 ) = rgb2;
            }
            else {
              if ( k == 2 ) {
                *( w + ic + j * kind + 2 ) = vir_w;
                *( v + ic + j * kind + 2 ) = vir_v;
                *( m + ic * 3 + j * kind * 3 + 6 ) = rgb0;
                *( m + ic * 3 + j * kind * 3 + 7 ) = rgb1;
                *( m + ic * 3 + j * kind * 3 + 8 ) = rgb2;
                *( s + i * col + j ) = 3;
              }
              else {
                *( w + ic + j * kind + 1 ) = vir_w;
                *( v + ic + j * kind + 1 ) = vir_v;
                *( m + ic * 3 + j * kind * 3 + 3 ) = rgb0;
                *( m + ic * 3 + j * kind * 3 + 4 ) = rgb1;
                *( m + ic * 3 + j * kind * 3 + 5 ) = rgb2;
                *( s + i * col + j ) = 2;
              }
            }
          }
          
          // 权重归一化
          {
            //t0 = Sum( w + ic + j * kind, w + ic + ( j + 1 ) * kind, 0.0 );
            t0=0;
            for (int ii=0; ii<kind; ++ii) t0+=w[ic + j * kind+ii];
          }
          *( w + ic + j * kind + 0 ) /= t0;
          *( w + ic + j * kind + 1 ) /= t0;
          *( w + ic + j * kind + 2 ) /= t0;
        }
      }
    }
  }
}

void GMM::RectifyLabel() {
  int i, j;
  unsigned char* t = new unsigned char[ row * col ];
  
  // 取反为填补小洞做准备
  for ( i = 0; i < row; ++i ) {
    for ( j = 0; j < col; ++j ) {
      *( t + i * col + j ) = !( *( label + i * col + j ) );
    }
  }
  
  // 填补小洞
  RegionRectify( t, row, col, col, min_pts );
  
  // 再一次取反得到补洞结果
  for ( i = 0; i < row; ++i ) {
    for ( j = 0; j < col; ++j ) {
      *( label + i * col + j ) = !( *( t + i * col + j ) );
    }
  }
  
  // 去处较小的连通区域
  RegionRectify( label, row, col, col, min_pts );
  
  delete [] t;
  t = NULL;
}

void GMM::Rectify( const unsigned char *pt, int step ) {
  const int size = row * col;
  
  double *it = new double[ size ];
  double *ib = new double[ size ];
  
  if ( rshadowx ) {
    rgb2gray( pt, it, row, col, step, col );
    rgb2gray( b, ib, row, col, col * 3, col );
    
    // RectifyLight(pt, step, it, ib, label, row, col);
    RectifyShadow( pt, step, b, it, ib, label, row, col );
  }
  
  if ( rshadowy ) {
    double * itx = new double[ size * 3 ];
    double * ibx = new double[ size * 3 ];
    
    mat2gray( b, ibx, size * 3 );
    mat2gray( pt, itx, size * 3 );
    
    rgb2grayx( ibx, ib, row, col, col * 3, col );
    rgb2grayx( itx, it, row, col, col * 3, col );
    
    delete [] itx;
    itx = 0;
    delete [] ibx;
    ibx = 0;
    
    shadow( pt, step, b, it, ib, label, row, col, shadowp );
  }
  
  delete [] it;
  it = NULL;
  delete [] ib;
  ib = NULL;
}

int GMM::RectifyShape() {
  int i, j, k;
  unsigned int *pt = new unsigned int[ row * col ];
  unsigned char *tl = new unsigned char[ row * col ];
  unsigned char *label0 = new unsigned char[ ( row + 2 ) * ( col + 2 ) ];
  
  // label0初始化
  for ( i = 0; i < ( row + 2 ) * ( col + 2 ); ++i ) {
    *( label0 + i ) = 0;
  }
  
  // label对应拷贝到label0
  for ( i = 0; i < row; ++i ) {
    for ( j = 0; j < col; ++j ) {
      *( label0 + ( i + 1 ) * ( col + 2 ) + ( j + 1 ) ) = *( label + i * col + j );
    }
  }
  
  // label0膨胀
  Expand( label0 + ( col + 2 ) + 1, tl, row, col, col + 2, col );
  // 膨胀结果->label0
  for ( i = 0; i < row; ++i ) {
    for ( j = 0; j < col; ++j ) {
      *( label0 + ( i + 1 ) * ( col + 2 ) + ( j + 1 ) ) = *( tl + i * col + j );
    }
  }
  // label0腐蚀
  Erode( label0 + ( col + 2 ) + 1, tl, row, col, col + 2, col );
  // 腐蚀结果->label0
  for ( i = 0; i < row; ++i ) {
    for ( j = 0; j < col; ++j ) {
      *( label0 + ( i + 1 ) * ( col + 2 ) + ( j + 1 ) ) = *( tl + i * col + j );
    }
  }
  
  // 连通区域
  int kind = region( label0 + ( col + 2 ) + 1, pt, row, col, col + 2 );
  
  int* pn = new int[ kind + 1 ]; // 存放每个连通区域中点的总个数的数组
  
  for ( i = 0; i < kind + 1; ++i ) {
    *( pn + i ) = 0;
  }
  
  for ( i = 0; i < col * row; ++i ) {
    ++pn[ *( pt + i ) ];
  }
  
  // label全部置0
  for ( i = 0; i < row * col; ++i ) {
    *( label + i ) = 0;
  }
  
  // 处理象素点个数>20的连通区域
  for ( i = 1; i < kind + 1; ++i ) {
    if ( pn[ i ] > ( int ) min_pts ) {
      for ( j = 0; j < ( row + 2 ) * ( col + 2 ); ++j ) {
        *( label0 + j ) = 0;
      }
      
      for ( j = 0; j < row; ++j ) {
        for ( k = 0; k < col; ++k ) {
          *( label0 + ( j + 1 ) * ( col + 2 ) + ( k + 1 ) ) = ( *( pt + j * col + k ) == ( unsigned int ) i ) ? 1 : 0;
        }
      }
      
      Expand( label0 + ( col + 2 ) + 1, tl, row, col, col + 2, col );
      
      for ( j = 0; j < row; ++j ) {
        for ( k = 0; k < col; ++k ) {
          *( label0 + ( j + 1 ) * ( col + 2 ) + ( k + 1 ) ) = *( tl + j * col + k );
        }
      }
      
      Erode( label0 + ( col + 2 ) + 1, tl, row, col, col + 2, col );
      
      for ( j = 0; j < row; ++j ) {
        for ( k = 0; k < col; ++k ) {
          *( label0 + ( j + 1 ) * ( col + 2 ) + ( k + 1 ) ) = *( tl + j * col + k );
        }
      }
      
      for ( j = 0; j < row; ++j ) {
        for ( k = 0; k < col; ++k ) {
          *( label + j * col + k ) = *( label + j * col + k ) || *( label0 + ( j + 1 ) * ( col + 2 ) + ( k + 1 ) );
        }
      }
    }
  }
  
  delete [] pn;
  pn = NULL;
  delete [] tl;
  tl = NULL;
  delete [] pt;
  pt = NULL;
  delete [] label0;
  label0 = NULL;
  return kind;
}

//  3.2 前景检测.
// * 背景模型建立后, 传入RGB图像数据以检测前景团块. 函数返回当前帧检测结果个数.
// * pGmm: 指向高斯背景建模对象的指针.
// * src : RGB模式图像数据的头指针.
// * step: 图像数据步长.
// * mask: 检测区域遮罩. 与场景像素大小相同的二值图像, 只对遮罩上大于0的RGB图像数据对应点作检测.
//     该指针指空时对整个场景都作前景检测.
int GMM::Detect( const unsigned char *pt, int step, const unsigned char *pmask ) {
  int blobnum;
  DetectForeGround( pt, step, pmask );
  
  if ( rlabel ) {
    RectifyLabel();
  }
  
  if ( rshadowx || rshadowy ) {
    Rectify( pt, step );
  }
  
  if ( rshape ) {
    RectifyShape();
  }
  
  // 计算连通区域个数(检测结果团块个数)
  blobnum = region( label, blob, row, col, col );
  return blobnum;
}
