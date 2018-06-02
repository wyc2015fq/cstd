#ifndef _RJ_ALGO_H_
#define _RJ_ALGO_H_


#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>


typedef unsigned char uchar;

/* 向量, 矩阵, 宏.
 * M22          : 行列式(2 x 2)求值.
 * M33          : 行列式(3 x 3)求值.
 * M44          : 行列式(4 x 4)求值.
 
 * MAT_INV22    : 矩阵(2 x 2)求逆.
 * MAT_INV33    : 矩阵(3 x 3)求逆.
 * MAT_INV44    : 矩阵(4 x 4)求逆.
 
 * Init         : 向量初始化.
 * InnerProduct : 向量内积.
 * Sum          : 向量和.
 * Avg          : 向量均值.
 * SumFabs      : 向量一范数.
 * SumSquard    : 向量二范数平方.
 
 * Transform    : 矩阵转置.
 * MatMul       : 矩阵乘法.
 * MatAdd       : 矩阵加法.
 * MatSub       : 矩阵减法.
 
 * InsertSort   : 插入排序.
 * QuickSort    : 递归快速排序.
 */

/* 简单类.
 * LineSection  : 直线AB类.
 * BinMap       : 二值图中的连通区域属性.
 */

/* 距离.
 * KDist        : 卡方距离.
 * FabsDist     : 差的绝对值.
 * HistoSim     : 直方图交.
 */

/* 图像.
 * RGB2Gray     : RGB数据转换为灰度数据.
 */


// 行列式(2 * 2, 3 * 3)求值, 矩阵(2 * 2, 3 * 3)求逆.
#define M22(A00, A01, A10, A11) ((A00) * (A11) - (A01) * (A10))

#define M33(A0, A1, A2, B0, B1, B2, C0, C1, C2) \
  (                                               \
      (A0) * M22((B1), (B2), (C1), (C2)) - \
      (A1) * M22((B0), (B2), (C0), (C2)) + \
      (A2) * M22((B0), (B1), (C0), (C1))   \
  )

#define M44(A00, A01, A02, A03, \
            A10, A11, A12, A13, \
            A20, A21, A22, A23, \
            A30, A31, A32, A33) \
(           (A00) * M33((A11), (A12), (A13),   \
                        (A21), (A22), (A23),   \
                        (A31), (A32), (A33)) - \
            (A01) * M33((A10), (A12), (A13),   \
                        (A20), (A22), (A23),   \
                        (A30), (A32), (A33)) + \
            (A02) * M33((A10), (A11), (A13),   \
                        (A20), (A21), (A23),   \
                        (A30), (A31), (A33)) - \
            (A03) * M33((A10), (A11), (A12),   \
                        (A20), (A21), (A22),   \
                        (A30), (A31), (A32))   \
)

#define MAT_INV22(A, A_INV) do                          \
  {                                                       \
    const double X = 1.0 / M22(*((A) + 0), *((A) + 1),  \
                               *((A) + 2), *((A) + 3)); \
    *((A_INV) + 0) = *((A) + 3) *  X; \
    *((A_INV) + 1) = *((A) + 1) * -X; \
    *((A_INV) + 2) = *((A) + 2) * -X; \
    *((A_INV) + 3) = *((A) + 0) *  X; \
  } while (0);

#define MAT_INV33(A, A_INV) do                                      \
  {                                                                   \
    const double X = 1.0 / M33(*((A) + 0), *((A) + 1), *((A) + 2),  \
                               *((A) + 3), *((A) + 4), *((A) + 5),  \
                               *((A) + 6), *((A) + 7), *((A) + 8)); \
    *((A_INV) + 0) =  X * M22(*((A) + 4), *((A) + 5),  \
                              *((A) + 7), *((A) + 8)); \
    *((A_INV) + 1) = -X * M22(*((A) + 1), *((A) + 2),  \
                              *((A) + 7), *((A) + 8)); \
    *((A_INV) + 2) =  X * M22(*((A) + 1), *((A) + 2),  \
                              *((A) + 4), *((A) + 5)); \
    *((A_INV) + 3) = -X * M22(*((A) + 3), *((A) + 5),  \
                              *((A) + 6), *((A) + 8)); \
    *((A_INV) + 4) =  X * M22(*((A) + 0), *((A) + 2),  \
                              *((A) + 6), *((A) + 8)); \
    *((A_INV) + 5) = -X * M22(*((A) + 0), *((A) + 2),  \
                              *((A) + 3), *((A) + 5)); \
    *((A_INV) + 6) =  X * M22(*((A) + 3), *((A) + 4),  \
                              *((A) + 6), *((A) + 7)); \
    *((A_INV) + 7) = -X * M22(*((A) + 0), *((A) + 1),  \
                              *((A) + 6), *((A) + 7)); \
    *((A_INV) + 8) =  X * M22(*((A) + 0), *((A) + 1),  \
                              *((A) + 3), *((A) + 4)); \
  } while (0);

#define MAT_INV44(A, A_INV) do                                                  \
  {                                                                               \
    const double X = 1.0 / M44(*((A) + 0), *((A) + 1), *((A) + 2), *((A) + 3),  \
                               *((A) + 4), *((A) + 5), *((A) + 6), *((A) + 7),  \
                               *((A) + 8), *((A) + 9), *((A) +10), *((A) +11),  \
                               *((A) +12), *((A) +13), *((A) +14), *((A) +15)); \
    *((A_INV) + 0) =  X * M33(*((A) + 5), *((A) + 6), *((A) + 7),  \
                              *((A) + 9), *((A) +10), *((A) +11),  \
                              *((A) +13), *((A) +14), *((A) +15)); \
    *((A_INV) + 1) = -X * M33(*((A) + 1), *((A) + 2), *((A) + 3),  \
                              *((A) + 9), *((A) +10), *((A) +11),  \
                              *((A) +13), *((A) +14), *((A) +15)); \
    *((A_INV) + 2) =  X * M33(*((A) + 1), *((A) + 2), *((A) + 3),  \
                              *((A) + 5), *((A) + 6), *((A) + 7),  \
                              *((A) +13), *((A) +14), *((A) +15)); \
    *((A_INV) + 3) = -X * M33(*((A) + 1), *((A) + 2), *((A) + 3),  \
                              *((A) + 5), *((A) + 6), *((A) + 7),  \
                              *((A) + 9), *((A) +10), *((A) +11)); \
    *((A_INV) + 4) = -X * M33(*((A) + 4), *((A) + 6), *((A) + 7),  \
                              *((A) + 8), *((A) +10), *((A) +11),  \
                              *((A) +12), *((A) +14), *((A) +15)); \
    *((A_INV) + 5) =  X * M33(*((A) + 0), *((A) + 2), *((A) + 3),  \
                              *((A) + 8), *((A) +10), *((A) +11),  \
                              *((A) +12), *((A) +14), *((A) +15)); \
    *((A_INV) + 6) = -X * M33(*((A) + 0), *((A) + 2), *((A) + 3),  \
                              *((A) + 4), *((A) + 6), *((A) + 7),  \
                              *((A) +12), *((A) +14), *((A) +15)); \
    *((A_INV) + 7) =  X * M33(*((A) + 0), *((A) + 2), *((A) + 3),  \
                              *((A) + 4), *((A) + 6), *((A) + 7),  \
                              *((A) + 8), *((A) +10), *((A) +11)); \
    *((A_INV) + 8) =  X * M33(*((A) + 4), *((A) + 5), *((A) + 7),  \
                              *((A) + 8), *((A) + 9), *((A) +11),  \
                              *((A) +12), *((A) +13), *((A) +15)); \
    *((A_INV) + 9) = -X * M33(*((A) + 0), *((A) + 1), *((A) + 3),  \
                              *((A) + 8), *((A) + 9), *((A) +11),  \
                              *((A) +12), *((A) +13), *((A) +15)); \
    *((A_INV) +10) =  X * M33(*((A) + 0), *((A) + 1), *((A) + 3),  \
                              *((A) + 4), *((A) + 5), *((A) + 7),  \
                              *((A) +12), *((A) +13), *((A) +15)); \
    *((A_INV) +11) = -X * M33(*((A) + 0), *((A) + 1), *((A) + 3),  \
                              *((A) + 4), *((A) + 5), *((A) + 7),  \
                              *((A) + 8), *((A) + 9), *((A) +11)); \
    *((A_INV) +12) = -X * M33(*((A) + 4), *((A) + 5), *((A) + 6),  \
                              *((A) + 8), *((A) + 9), *((A) +10),  \
                              *((A) +12), *((A) +13), *((A) +14)); \
    *((A_INV) +13) =  X * M33(*((A) + 0), *((A) + 1), *((A) + 2),  \
                              *((A) + 8), *((A) + 9), *((A) +10),  \
                              *((A) +12), *((A) +13), *((A) +14)); \
    *((A_INV) +14) = -X * M33(*((A) + 0), *((A) + 1), *((A) + 2),  \
                              *((A) + 4), *((A) + 5), *((A) + 6),  \
                              *((A) +12), *((A) +13), *((A) +14)); \
    *((A_INV) +15) =  X * M33(*((A) + 0), *((A) + 1), *((A) + 2),  \
                              *((A) + 4), *((A) + 5), *((A) + 6),  \
                              *((A) + 8), *((A) + 9), *((A) +10)); \
  } while (0);

template <typename _II>
inline
void Init( _II *first, _II *last, const _II &value ) {
  for ( ; first < last; ++first )
    *first = value;
}

template <typename _II , typename _IO>
inline
_IO InnerProduct( const _II *first, const _II *last, const _II *first_, _IO prod ) {
  for ( ; first < last; ++first, ++first_ )
    prod += *first * *first_;
  return prod;
}

template <typename _II, typename _IO>
inline
_IO Sum( const _II *first, const _II *last, _IO sum ) {
  for ( ; first < last; ++first )
    sum += *first;
  return sum;
}

template <typename _II, typename _IO>
inline
_IO Avg( const _II *first, const _II *last, _IO avg ) {
  return avg + Sum( first, last, 0.0 ) / ( last - first );
}

template <typename _II, typename _IO>
inline
_IO SumFabs( const _II *first, const _II *last, _IO sum ) {
  for ( ; first < last; ++first )
    sum += fabs( *first );
  return sum;
}

template <typename _II, typename _IO>
inline
_IO SumSquard( const _II *first, const _II *last, _IO sum ) {
  for ( ; first < last; ++first )
    sum += *first * *first;
  return sum;
}

template <typename _I>
inline
void Transform( const _I *srcArr, int m, int n, _I *dstArr ) {
  for ( int i = 0; i < m; ++i ) {
    for ( int j = 0; j < n; ++j ) {
      *( dstArr + j * m + i ) = *( srcArr + i * n + j );
    }
  }
}

template <typename _I, typename _II, typename IO>
inline
void MatMul( const _I *first, const _II *first_, IO *mul, int x, int y, int z ) {
  double t;
  int i, j, k;
  
  for ( i = 0; i < x; ++i ) {
    for ( j = 0; j < z; ++j ) {
      for ( t = 0, k = 0; k < y; ++k ) {
        t += *( first + i * y + k ) * *( first_ + k * z + j );
      }
      
      *( mul + i * z + j ) = IO( t );
    }
  }
}

template <typename _I, typename _II, typename _IO>
inline
void MatAdd( const _I *first, const _II *first_, _IO *add
             , int m, int n ) {
  for ( int i = 0; i < m * n; ++i ) {
    *( add
         + i ) = *( first + i ) + *( first_ + i );
  }
}

template <typename _I, typename _II, typename _IO>
inline
void MatSub( const _I *first, const _II *first_, _IO *sub, int m, int n ) {
  for ( int i = 0; i < m * n; ++i ) {
    *( sub + i ) = *( first + i ) - *( first_ + i );
  }
}

template <typename _II>
inline
void InsertSort( _II *array, int len ) {
  assert( NULL != array && 1 < len );
  
  for ( int i = 1; i < len; ++i ) {
    assert( 0 < i && len > i );
    const _II t = *( array + i );
    
    for ( int j = i - 1; j > -1 && t < *( array + j ); --j ) {
      assert( -1 < j && len - 1 > j );
      *( array + j + 1 ) = *( array + j );
    }
    
    assert( -1 < j + 1 && len > j + 1 );
    *( array + j + 1 ) = t;
  }
}

template <typename _II>
inline
_II* Divide( _II *head, _II *tail ) {
  _II var = *head;
  
  while ( head < tail ) {
    while ( head < tail && *tail >= var )
      --tail;
    *head = *tail;
    
    while ( head < tail && *head <= var )
      ++head;
    *tail = *head;
  }
  
  *head = var;
  
  return head;
}

template <typename _II>
inline
void QuickSort( _II *head, _II *tail ) {
  if ( head < tail ) {
    _II * mid = Divide( head, tail );
    
    QuickSort( head, mid );
    QuickSort( mid + 1, tail );
  }
}

template <typename _II>
inline
double KDist( _II a, _II b ) {
  const double SUM = a + b;
  const _II SUB = a - b;
  return 0.0 == SUM ? 0.0 : ( SUB * SUB / SUM );
}

template <typename _II>
inline
double FabsDist( _II a, _II b ) {
  return fabs( a - b );
}

template <typename _II>
inline
double HistoSim( const _II *arrX, const _II *arrY, int len ) {
  double sim = 0.0;
  
  const _II *px = arrX;
  const _II *py = arrY;
  
  for ( ; px < arrX + len; ) {
    const _II X = *px++;
    const _II Y = *py++;
    
    sim += X < Y ? X : Y;
  }
  
  return sim;
}

inline void RGB2Gray( const uchar *rgb, uchar *gray, int h, int w, int rgbStep, int grayStep ) {
  assert( NULL != rgb && NULL != gray && h > 0 && w > 0 && rgbStep > 0 && grayStep > 0 );
  
  for ( int i = 0; i < h; ++i ) {
    for ( int j = 0; j < w; ++j ) {
      *( gray + i * grayStep + j ) = uchar( 0.114 * *( rgb + i * rgbStep + j * 3 + 0 ) +
                                            0.587 * *( rgb + i * rgbStep + j * 3 + 1 ) +
                                            0.299 * *( rgb + i * rgbStep + j * 3 + 2 ) + 0.5 );
    }
  }
}

// 直线AB模板类.
template <typename _II>
class LineSection {
public:
  LineSection( _II xa = 0, _II ya = 0, _II xb = 0, _II yb = 0 ) : m_XA( xa ), m_XB( xb ), m_YA( ya ), m_YB( yb ) {}
  
public:
  // 斜率.
  bool Slope( double &slope ) const {
    if ( m_XB == m_XA ) {
      return false;
    }
    else {
      slope = ( m_YB - m_YA ) / double( m_XB - m_XA );
      return true;
    }
  }
  
  // 给定x坐标求y坐标.
  bool PosY( _II x, double &y ) {
    if ( m_XA == m_XB ) {
      return false;
    }
    else {
      y = m_YA - ( m_YA - m_YB ) * ( m_XA - x ) / double( m_XA - m_XB );
      return true;
    }
  }
  
  // 给定y坐标求x坐标.
  bool PosX( _II y, double &x ) {
    if ( m_YA == m_YB ) {
      return false;
    }
    else {
      x = m_XA - ( m_XA - m_XB ) * ( m_YA - y ) / double( m_YA - m_YB );
      return true;
    }
  }
  
  // 点在直线上的投影.
  void Shadow( _II x, _II y, double &sx, double &sy ) const {
    double curSlope;
    
    if ( !Slope( curSlope ) ) {
      sx = m_XA;
      sy = y;
    } else if ( 0 == curSlope ) {
      sy = m_YA;
      sx = x;
    } else {
      sx = ( x + curSlope * ( curSlope * m_XA + y - m_YA ) ) / ( 1.0 + curSlope * curSlope );
      sy = curSlope * ( sx - m_XA ) + m_YA;
    }
  }
  
  // 点到直线的距离的平方.
  double Dist2Point2( _II x, _II y ) const {
    double sx, sy;
    
    Shadow( x, y, sx, sy );
    return ( sx - x ) * ( sx - x ) + ( sy - y ) * ( sy - y );
  }
  
  // 直线交点.
  bool Intersection( _II xa, _II ya, _II xb, _II yb, double &x, double &y ) const {
    const double SUB_XA = m_XB - m_XA;
    const double SUB_YA = m_YB - m_YA;
    const double SUB_XB = xb - xa;
    const double SUB_YB = yb - ya;
    
    if ( ( 0 == SUB_XA && 0 == SUB_XB ) || ( 0 != SUB_XA && 0 != SUB_XB && SUB_YA / SUB_XA == SUB_YB / SUB_XB ) )
      return false;
      
    else {
      if ( 0 == SUB_XA ) {
        x = m_XA;
        y = ( SUB_YB * x + SUB_XB * yb - SUB_YB * xb ) / SUB_XB;
      } else if ( 0 == SUB_XB ) {
        x = xa;
        y = ( SUB_YA * x + SUB_XA * m_YB - SUB_YA * m_XB ) / SUB_XA;
      } else if ( 0 == SUB_YA ) {
        y = m_YA;
        x = ( SUB_XB * y + SUB_YB * xb - SUB_XB * yb ) / SUB_YB;
      } else if ( 0 == SUB_YB ) {
        y = ya;
        x = ( SUB_XA * y + SUB_YA * m_XB - SUB_XA * m_YB ) / SUB_YA;
      } else {
        y = ( SUB_YA * ( SUB_XB * yb - SUB_YB * xb ) + SUB_YB * ( SUB_YA * m_XB - SUB_XA * m_YB ) ) /
            ( SUB_XB * SUB_YA - SUB_YB * SUB_XA );
        x = ( SUB_XB * y + SUB_YB * xb - SUB_XB * yb ) / SUB_YB;
      }
      
      return true;
    }
  }
  // 直线交点.
  bool Intersection( const LineSection &lineX, double &x, double &y ) const {
    return Intersection( lineX.XA(), lineX.YA(), lineX.XB(), lineX.YB(), x, y );
  }
  
public:
  _II XA( void ) const {
    return m_XA;
  }
  _II XB( void ) const {
    return m_XB;
  }
  _II YA( void ) const {
    return m_YA;
  }
  _II YB( void ) const {
    return m_YB;
  }
  
private:
  _II m_XA, m_XB;
  _II m_YA, m_YB;
};

// 二值图中的连通区域属性.
template <typename _II>
class BinMap {
public:
  uchar** GetBinPtr() const {
    return m_BinImg;
  }
  
  int* GetArea() const {
    return m_Area;
  }
  
  const int* GetRect() const // 指针指向数据存放顺序为[left, top, right, bottom]的整型数组.
  {
    return m_Rect;
  }
  
  const int* GetCentroid() const {
    return m_Centroid;
  }
  
  int GetKind( void ) const {
    return m_Kind;
  }
  
public:
  BinMap( const _II *map, int kind, int h, int w ) : m_BinImg( new uchar*[ kind ] ),
      m_Kind( kind ),
      m_Area( new int[ kind ] ),
      m_Rect( new int[ kind * 4 ] ),
  m_Centroid( new int[ kind * 2 ] ) {
    for ( uchar **p = m_BinImg; p < m_BinImg + kind; ++p ) {
      *p = new uchar[ h * w ];
      
      memset( *p, 0, h * w * sizeof( uchar ) );
    }
    
    for ( int i = 0; i < kind; ++i ) {
      *( m_Area + i ) = 0;
      
      *( m_Rect + i * 4 + 0 ) = w - 1;
      *( m_Rect + i * 4 + 1 ) = h - 1;
      *( m_Rect + i * 4 + 2 ) = 0;
      *( m_Rect + i * 4 + 3 ) = 0;
      
      *( m_Centroid + i * 2 + 0 ) = 0;
      *( m_Centroid + i * 2 + 1 ) = 0;
    }
    
    AnalyzeMap( map, h, w, kind );
  }
  
  ~BinMap() {
    delete [] m_Centroid;
    m_Centroid = NULL;
    delete [] m_Rect;
    m_Rect = NULL;
    delete [] m_Area;
    m_Area = NULL;
    
    for ( uchar **p = m_BinImg; p < m_BinImg + m_Kind; ++p ) {
      delete [] ( *p );
      *p = NULL;
    }
    
    delete [] m_BinImg;
    m_BinImg = NULL;
  }
  
private:
  void AnalyzeMap( const _II *map, int h, int w, int kind ) {
    for ( int i = 0; i < h; ++i ) {
      for ( int j = 0; j < w; ++j ) {
        if ( 0 < *( map + i * w + j ) ) {
          const int INDEX = *( map + i * w + j ) - 1;
          
          *( *( m_BinImg + INDEX ) + i * w + j ) = 1;
          
          ++*( m_Area + INDEX );
          
          *( m_Centroid + INDEX * 2 + 0 ) += i;
          *( m_Centroid + INDEX * 2 + 1 ) += j;
          
          if ( j < *( m_Rect + INDEX * 4 + 0 ) )
            * ( m_Rect + INDEX * 4 + 0 ) = j;
          if ( i < *( m_Rect + INDEX * 4 + 1 ) )
            * ( m_Rect + INDEX * 4 + 1 ) = i;
          if ( j > *( m_Rect + INDEX * 4 + 2 ) )
            * ( m_Rect + INDEX * 4 + 2 ) = j;
          if ( i > *( m_Rect + INDEX * 4 + 3 ) )
            * ( m_Rect + INDEX * 4 + 3 ) = i;
        }
      }
    }
    
    for ( i = 0; i < kind; ++i ) {
      *( m_Centroid + i * 2 + 0 ) = int( *( m_Centroid + i * 2 + 0 ) / double( *( m_Area + i ) ) + 0.5 );
      *( m_Centroid + i * 2 + 1 ) = int( *( m_Centroid + i * 2 + 1 ) / double( *( m_Area + i ) ) + 0.5 );
    }
  }
  
private:
  uchar **m_BinImg; // 指向动态分配(uchar*)数组, 数组中的指针指向动态分配的二值图像数据.
  int m_Kind;   // 连通区域个数.
  int *m_Area;   // 连通区域面积数组.
  int *m_Rect;   // 连通区域外框数组, 存放顺序为(left, top, right, bottom).
  int *m_Centroid; // 连通区域质心序列(y, x).
};

#endif
