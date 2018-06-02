
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "macro.h"

/*
初始化带尺度的单位矩阵
mat 待初始化的矩阵 (不一定是方阵)。
value 赋值给对角线元素的值。
函数 cvSetIdentity 初始化带尺度的单位矩阵:
arr(i,j)=value 如果 i=j,
否则为 0
*/
#define cvSetIdentity(_HEIGHT, _WIDTH, _DATA, _STEP, _VALUE)                  \
  do {                                                                          \
    int _LEN = MIN(_HEIGHT, _WIDTH), _STEP1 = _STEP + 1;                        \
    SKIP_FILL1D(_LEN, _DATA, _STEP1, _VALUE);                                   \
  } while(0)

#define icvCopyVector( src, dst, len ) memcpy( (dst), (src), (len)*sizeof((dst)[0]))
#define icvSetZero( dst, len ) memset( (dst), 0, (len)*sizeof((dst)[0]))

#define icvCopyVector_32f( src, len, dst ) memcpy((dst),(src),(len)*sizeof(float))
#define icvSetZero_32f( dst, cols, rows ) memset((dst),0,(rows)*(cols)*sizeof(float))
#define icvCopyVector_64d( src, len, dst ) memcpy((dst),(src),(len)*sizeof(double))
#define icvSetZero_64d( dst, cols, rows ) memset((dst),0,(rows)*(cols)*sizeof(double))
#define icvCopyMatrix_32f( src, w, h, dst ) memcpy((dst),(src),(w)*(h)*sizeof(float))
#define icvCopyMatrix_64d( src, w, h, dst ) memcpy((dst),(src),(w)*(h)*sizeof(double))

#define icvCreateVector_32f( len )  (float*)cvAlloc( (len)*sizeof(float))
#define icvCreateVector_64d( len )  (double*)cvAlloc( (len)*sizeof(double))
#define icvCreateMatrix_32f( w, h )  (float*)cvAlloc( (w)*(h)*sizeof(float))
#define icvCreateMatrix_64d( w, h )  (double*)cvAlloc( (w)*(h)*sizeof(double))

#define icvDeleteVector( vec )  cvFree( &(vec) )
#define icvDeleteMatrix icvDeleteVector

#define icvAddMatrix_32f( src1, src2, dst, w, h ) \
  icvAddVector_32f( (src1), (src2), (dst), (w)*(h))

#define icvSubMatrix_32f( src1, src2, dst, w, h ) \
  icvSubVector_32f( (src1), (src2), (dst), (w)*(h))

#define icvNormVector_32f( src, len )  \
  sqrt(icvDotProduct_32f( src, src, len ))

#define icvNormVector_64d( src, len )  \
  sqrt(icvDotProduct_64d( src, src, len ))


#define icvDeleteMatrix icvDeleteVector

#define icvCheckVector_64f( ptr, len )
#define icvCheckVector_32f( ptr, len )

CV_INLINE double icvSum_32f(const float* src, int len)
{
  int i;
  double s = 0;

  for (i = 0; i < len; i++) {
    s += src[ i ];
  }

  icvCheckVector_64f(&s, 1);

  return s;
}

CV_INLINE double icvDotProduct_32f(const float* src1, const float* src2, int len)
{
  int i;
  double s = 0;

  for (i = 0; i < len; i++) {
    s += src1[ i ] * src2[ i ];
  }

  icvCheckVector_64f(&s, 1);

  return s;
}


CV_INLINE double icvDotProduct_64f(const double* src1, const double* src2, int len)
{
  int i;
  double s = 0;

  for (i = 0; i < len; i++) {
    s += src1[ i ] * src2[ i ];
  }

  icvCheckVector_64f(&s, 1);

  return s;
}


CV_INLINE void icvMulVectors_32f(const float* src1, const float* src2,
    float* dst, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = src1[ i ] * src2[ i ];
  }

  icvCheckVector_32f(dst, len);
}

CV_INLINE void icvMulVectors_64d(const double* src1, const double* src2,
    double* dst, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = src1[ i ] * src2[ i ];
  }

  icvCheckVector_64f(dst, len);
}


CV_INLINE void icvAddVector_32f(const float* src1, const float* src2,
    float* dst, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = src1[ i ] + src2[ i ];
  }

  icvCheckVector_32f(dst, len);
}

CV_INLINE void icvAddVector_64d(const double* src1, const double* src2,
    double* dst, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = src1[ i ] + src2[ i ];
  }

  icvCheckVector_64f(dst, len);
}


CV_INLINE void icvSubVector_32f(const float* src1, const float* src2,
    float* dst, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = src1[ i ] - src2[ i ];
  }

  icvCheckVector_32f(dst, len);
}

CV_INLINE void icvSubVector_64d(const double* src1, const double* src2,
    double* dst, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = src1[ i ] - src2[ i ];
  }

  icvCheckVector_64f(dst, len);
}


#define icvAddMatrix_64d( src1, src2, dst, w, h ) \
  icvAddVector_64d( (src1), (src2), (dst), (w)*(h))

#define icvSubMatrix_64d( src1, src2, dst, w, h ) \
  icvSubVector_64d( (src1), (src2), (dst), (w)*(h))


CV_INLINE void icvSetIdentity_32f(float* dst, int w, int h)
{
  int i, len = MIN(w, h);
  icvSetZero_32f(dst, w, h);

  for (i = 0; len--; i += w + 1) {
    dst[ i ] = 1.f;
  }
}


CV_INLINE void icvSetIdentity_64d(double* dst, int w, int h)
{
  int i, len = MIN(w, h);
  icvSetZero_64d(dst, w, h);

  for (i = 0; len--; i += w + 1) {
    dst[ i ] = 1.;
  }
}


CV_INLINE void icvTrace_32f(const float* src, int w, int h, float* trace)
{
  int i, len = MIN(w, h);
  double sum = 0;

  for (i = 0; len--; i += w + 1) {
    sum += src[ i ];
  }

  *trace = (float) sum;

  icvCheckVector_64f(&sum, 1);
}


CV_INLINE void icvTrace_64d(const double* src, int w, int h, double* trace)
{
  int i, len = MIN(w, h);
  double sum = 0;

  for (i = 0; len--; i += w + 1) {
    sum += src[ i ];
  }

  *trace = sum;

  icvCheckVector_64f(&sum, 1);
}


CV_INLINE void icvScaleVector_32f(const float* src, float* dst,
    int len, double scale)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = (float)(src[ i ] * scale);
  }

  icvCheckVector_32f(dst, len);
}


CV_INLINE void icvScaleVector_64d(const double* src, double* dst,
    int len, double scale)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = src[ i ] * scale;
  }

  icvCheckVector_64f(dst, len);
}


CV_INLINE void icvTransposeMatrix_32f(const float* src, int w, int h, float* dst)
{
  int i, j;

  for (i = 0; i < w; i++)
    for (j = 0; j < h; j++) {
      *dst++ = src[ j * w + i ];
    }

  icvCheckVector_32f(dst, w * h);
}

CV_INLINE void icvTransposeMatrix_64d(const double* src, int w, int h, double* dst)
{
  int i, j;

  for (i = 0; i < w; i++)
    for (j = 0; j < h; j++) {
      *dst++ = src[ j * w + i ];
    }

  icvCheckVector_64f(dst, w * h);
}

CV_INLINE void icvDetMatrix3x3_64d(const double* mat, double* det)
{
#define m(y,x) mat[(y)*3 + (x)]

  * det = m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) -
      m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
      m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));

#undef m

  icvCheckVector_64f(det, 1);
}


CV_INLINE void icvMulMatrix_32f(const float* src1, int w1, int h1,
    const float* src2, int w2, int h2,
    float* dst)
{
  int i, j, k;

  if (w1 != h2) {
    assert(0);
    return ;
  }

  for (i = 0; i < h1; i++, src1 += w1, dst += w2)
    for (j = 0; j < w2; j++) {
      double s = 0;

      for (k = 0; k < w1; k++) {
        s += src1[ k ] * src2[ j + k * w2 ];
      }

      dst[ j ] = (float) s;
    }

  icvCheckVector_32f(dst, h1 * w2);
}


CV_INLINE void icvMulMatrix_64d(const double* src1, int w1, int h1,
    const double* src2, int w2, int h2,
    double* dst)
{
  int i, j, k;

  if (w1 != h2) {
    assert(0);
    return ;
  }

  for (i = 0; i < h1; i++, src1 += w1, dst += w2)
    for (j = 0; j < w2; j++) {
      double s = 0;

      for (k = 0; k < w1; k++) {
        s += src1[ k ] * src2[ j + k * w2 ];
      }

      dst[ j ] = s;
    }

  icvCheckVector_64f(dst, h1 * w2);
}


#define icvTransformVector_32f( matr, src, dst, w, h ) \
  icvMulMatrix_32f( matr, w, h, src, 1, w, dst )

#define icvTransformVector_64d( matr, src, dst, w, h ) \
  icvMulMatrix_64d( matr, w, h, src, 1, w, dst )


#define icvScaleMatrix_32f( src, dst, w, h, scale ) \
  icvScaleVector_32f( (src), (dst), (w)*(h), (scale) )

#define icvScaleMatrix_64d( src, dst, w, h, scale ) \
  icvScaleVector_64d( (src), (dst), (w)*(h), (scale) )

#define icvDotProduct_64d icvDotProduct_64f


#if 0
CV_INLINE void icvInvertMatrix_64d(double* A, int n, double* invA)
{
  CvMat Am = cvMat(n, n, CV_64F, A);
  CvMat invAm = cvMat(n, n, CV_64F, invA);

  cvInvert(&Am, &invAm, CV_SVD);
}

CV_INLINE void icvMulTransMatrixR_64d(double* src, int width, int height, double* dst)
{
  CvMat srcMat = cvMat(height, width, CV_64F, src);
  CvMat dstMat = cvMat(width, width, CV_64F, dst);

  cvMulTransposed(&srcMat, &dstMat, 1);
}

CV_INLINE void icvMulTransMatrixL_64d(double* src, int width, int height, double* dst)
{
  CvMat srcMat = cvMat(height, width, CV_64F, src);
  CvMat dstMat = cvMat(height, height, CV_64F, dst);

  cvMulTransposed(&srcMat, &dstMat, 0);
}

CV_INLINE void icvMulTransMatrixR_32f(float* src, int width, int height, float* dst)
{
  CvMat srcMat = cvMat(height, width, CV_32F, src);
  CvMat dstMat = cvMat(width, width, CV_32F, dst);

  cvMulTransposed(&srcMat, &dstMat, 1);
}

CV_INLINE void icvMulTransMatrixL_32f(float* src, int width, int height, float* dst)
{
  CvMat srcMat = cvMat(height, width, CV_32F, src);
  CvMat dstMat = cvMat(height, height, CV_32F, dst);

  cvMulTransposed(&srcMat, &dstMat, 0);
}

CV_INLINE void icvCvt_32f_64d(const float* src, double* dst, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = src[ i ];
  }
}

CV_INLINE void icvCvt_64d_32f(const double* src, float* dst, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    dst[ i ] = (float) src[ i ];
  }
}

#endif

/*
矩阵的转置
void cvTranspose( const CvArr* src, CvArr* dst );
#define cvT cvTranspose
src 输入矩阵
dst 目标矩阵
函数 cvTranspose 对矩阵 src 求转置:
dst(i,j)=src(j,i)
注意，假设是复数矩阵不会求得复数的共轭。共轭应该是独立的：查看的 cvXorS 例子代码。
*/
/****************************************************************************************\
*                                     Matrix transpose                                   *
\****************************************************************************************/

/////////////////// macros for inplace transposition of square matrix ////////////////////

#define ICV_DEF_TRANSP_INP_CASE_C1(arr, step, len, arrtype)                   \
  do {                                                                          \
    arrtype* arr0 = arr, *arr1 = arr, *arr2, *arr3;                             \
    int len1 = len;                                                             \
    while( --len1 ) {                                                           \
      arr0 += step, arr1++;                                                     \
      arr2 = arr0;                                                              \
      arr3 = arr1;                                                              \
      do {                                                                      \
        arrtype t0 = arr2[0];                                                   \
        arrtype t1 = arr3[0];                                                   \
        arr2[0] = t1;                                                           \
        arr3[0] = t0;                                                           \
        arr2++;                                                                 \
        arr3 += step;                                                           \
      } while( arr2 != arr3  );                                                 \
    }                                                                           \
  } while(0)

#define ICV_DEF_TRANSP_INP_CASE_C3(arr, step, len, arrtype)                   \
  do {                                                                          \
    arrtype* arr0 = arr, *arr1 = arr, *arr2, *arr3;                             \
    int y;                                                                      \
    for( y = 1; y < len; y++ ) {                                                \
      arr0 += step, arr1 += 3;                                                  \
      arr2 = arr0;                                                              \
      arr3 = arr1;                                                              \
      for( ; arr2!=arr3; arr2+=3, arr3+=step) {                                 \
        arrtype t0 = arr2[0];                                                   \
        arrtype t1 = arr3[0];                                                   \
        arr2[0] = t1;                                                           \
        arr3[0] = t0;                                                           \
        t0 = arr2[1];                                                           \
        t1 = arr3[1];                                                           \
        arr2[1] = t1;                                                           \
        arr3[1] = t0;                                                           \
        t0 = arr2[2];                                                           \
        t1 = arr3[2];                                                           \
        arr2[2] = t1;                                                           \
        arr3[2] = t0;                                                           \
      }                                                                         \
    }                                                                           \
  } while(0)


#define ICV_DEF_TRANSP_INP_CASE_C4(arr, step, len, arrtype)                   \
  do {                                                                          \
    arrtype* arr0 = arr, *arr1 = arr, *arr2, *arr3;                             \
    int y;                                                                      \
    for( y = 1; y < len; y++ ) {                                                \
      arr0 += step, arr1 += 4;                                                  \
      arr2 = arr0;                                                              \
      arr3 = arr1;                                                              \
      for( ; arr2!=arr3; arr2+=4, arr3+=step) {                                 \
        arrtype t0 = arr2[0];                                                   \
        arrtype t1 = arr3[0];                                                   \
        arr2[0] = t1;                                                           \
        arr3[0] = t0;                                                           \
        t0 = arr2[1];                                                           \
        t1 = arr3[1];                                                           \
        arr2[1] = t1;                                                           \
        arr3[1] = t0;                                                           \
        t0 = arr2[2];                                                           \
        t1 = arr3[2];                                                           \
        arr2[2] = t1;                                                           \
        arr3[2] = t0;                                                           \
        t0 = arr2[3];                                                           \
        t1 = arr3[3];                                                           \
        arr2[3] = t1;                                                           \
        arr3[3] = t0;                                                           \
      }                                                                         \
    }                                                                           \
  } while(0)


// macros for non-inplace transposition of rectangular matrix
#define ICV_DEF_TRANSP_CASE_C1(src, srcstep, dst, dststep, size, arrtype)     \
  do {                                                                          \
    int x, y;                                                                   \
    for( y = 0; y <= size.height - 2; y += 2, src += 2*srcstep, dst += 2 ) {    \
      const arrtype* src0 = src + srcstep;                                      \
      const arrtype* src1 = src + srcstep;                                      \
      arrtype* dst1 = dst;                                                      \
      for( x = 0; x <= size.width - 2; x += 2, dst1 += dststep ) {              \
        arrtype t0 = src0[x];                                                   \
        arrtype t1 = src1[x];                                                   \
        dst1[0] = t0;                                                           \
        dst1[1] = t1;                                                           \
        dst1 += dststep;                                                        \
        t0 = src0[x + 1];                                                       \
        t1 = src1[x + 1];                                                       \
        dst1[0] = t0;                                                           \
        dst1[1] = t1;                                                           \
      }                                                                         \
      if( x < size.width ) {                                                    \
        arrtype t0 = src0[x];                                                   \
        arrtype t1 = src1[x];                                                   \
        dst1[0] = t0;                                                           \
        dst1[1] = t1;                                                           \
      }                                                                         \
    }                                                                           \
    if( y < size.height ) {                                                     \
      const arrtype* src1 = src;                                                \
      arrtype* dst1 = dst;                                                      \
      for( x = 0; x <= size.width - 2; x += 2, dst1 += 2*dststep ) {            \
        arrtype t0 = src1[x];                                                   \
        arrtype t1 = src1[x + 1];                                               \
        dst1[0] = t0;                                                           \
        dst1[dststep] = t1;                                                     \
      }                                                                         \
      if( x < size.width ) {                                                    \
        arrtype t0 = src1[x];                                                   \
        dst1[0] = t0;                                                           \
      }                                                                         \
    }                                                                           \
  } while(0)


#define ICV_DEF_TRANSP_CASE_C3(src, srcstep, dst, dststep, size, arrtype)     \
  do {                                                                          \
    int x, sizewidth = size.width * 3;                                          \
    const arrtype* src0 = src;                                                  \
    arrtype* dst0 = dst;                                                        \
    for( ; size.height--; src0+=srcstep, dst0+=3 ) {                            \
      arrtype* dst1 = dst0;                                                       \
      for( x = 0; x < sizewidth; x += 3, dst1 += dststep ) {                      \
        arrtype t0 = src0[x];                                                   \
        arrtype t1 = src0[x + 1];                                               \
        arrtype t2 = src0[x + 2];                                               \
        dst1[0] = t0;                                                           \
        dst1[1] = t1;                                                           \
        dst1[2] = t2;                                                           \
      }                                                                         \
    }                                                                           \
  } while(0)


#define ICV_DEF_TRANSP_CASE_C4(src, srcstep, dst, dststep, size, arrtype)     \
  do {                                                                          \
    int x, sizewidth = size.width * 4;                                          \
    const arrtype* src0 = src;                                                  \
    arrtype* dst0 = dst;                                                        \
    for( ; size.height--; src0+=srcstep, dst0+=4 ) {                            \
      arrtype* dst1 = dst0;                                                     \
      for( x = 0; x < sizewidth; x += 4, dst1 += dststep ) {                    \
        arrtype t0 = src0[x];                                                   \
        arrtype t1 = src0[x + 1];                                               \
        dst1[0] = t0;                                                           \
        dst1[1] = t1;                                                           \
        t0 = src0[x + 2];                                                       \
        t1 = src0[x + 3];                                                       \
        dst1[2] = t0;                                                           \
        dst1[3] = t1;                                                           \
      }                                                                         \
    }                                                                           \
  } while(0)

#define cvTranspose_CX( src, srcstep, dst, dststep, size, cn, arrtype )       \
  do {                                                                          \
    if( src == dst ) {                                                          \
      assert( size.width == size.height );                                      \
      ICV_DEF_TRANSP_INP_CASE_C##cn(dst, dststep, size.width, arrtype);         \
    } else {                                                                    \
      ICV_DEF_TRANSP_CASE_C##cn(src, srcstep, dst, dststep, size, arrtype);     \
    }                                                                           \
  } while(0)


#define cvTranspose( src, srcstep, dst, dststep, size, cn, arrtype )          \
  do {                                                                          \
    if (1==(cn)) {                                                              \
      cvTranspose_CX( src, srcstep, dst, dststep, size, 1, arrtype );           \
    } else if (2=(cn)) {                                                        \
      cvTranspose_CX( src, srcstep, dst, dststep, size, 2, arrtype );           \
    } else if (3=(cn)) {                                                        \
      cvTranspose_CX( src, srcstep, dst, dststep, size, 3, arrtype );           \
    } else if (4=(cn)) {                                                        \
      cvTranspose_CX( src, srcstep, dst, dststep, size, 4, arrtype );           \
    }                                                                           \
  } while(0)

#define cvRepeat(srcheight, srcwidth, src, srcstep, dstheight, dstwidth, dst, dststep)\
  do {                                                                          \
    int x, y, k, l;                                                             \
    for( y = 0, k = 0; y < dstheight; y++ ) {                                   \
      for( x = 0; x < dstwidth; x += srcwidth ) {                               \
        l = srcwidth;                                                           \
        if( l > dstwidth - x )                                                  \
          l = dstwidth - x;                                                     \
        memcpy( dst + y*dststep + x*sizeof(src[0]),                             \
            src + k*srcstep, l*sizeof(src[0]) );                            \
      }                                                                         \
      if( ++k == srcheight )                                                    \
        k = 0;                                                                  \
    }                                                                           \
  } while(0)



/****************************************************************************************\
*                                    cvCalcCovarMatrix                                   *
\****************************************************************************************/

#define ICV_DOT_PRODUCT_CASE(size, vec1, veccn1, vec2, veccn2, avg, result)   \
  do {                                                                          \
    int x=0, x2=0, x3=0;                                                        \
    result=0;                                                                   \
    for(; x < (size);++x, x1+=veccn1, x2+=veccn2)                               \
      result += (*(vec1+x1) - *(avg+x))*(*(vec2+x2) - *(avg+x));                \
  } while(0)

#define CV_CALC_COVAR_MATRIX(size1, size2, size, vec1, vecstep1, veccn1, vec2, vecstep2, veccn2, avg, cov, covstep)\
  do {                                                                          \
    int i, j;                                                                   \
    if ((vec1 == vec2) && (size1==size2)) {                                     \
      for (i=0; i<size1; ++i) {                                                 \
        int pos1 = i*vecstep1;                                                  \
        for (j=0; j<=i; ++j) {                                                  \
          int pos2 = j*vecstep2;                                                \
          ICV_DOT_PRODUCT_CASE((vec1+pos1), veccn1, (vec2+pos2), vecstep2,      \
              (avg), veccn2, size, *(cov+i*(convstep)+j));                        \
          *(cov+i*(convstep)+j) = *(cov+j*(convstep)+i);                        \
        }                                                                       \
      }                                                                         \
    } else {                                                                    \
      for (i=0; i<size1; ++i) {                                                 \
        int pos1 = i*vecstep1;                                                  \
        for (j=0; j<size2; ++j) {                                               \
          int pos2 = j*vecstep2;                                                \
          ICV_DOT_PRODUCT_CASE((vec1+pos1), veccn1, (vec2+pos2), vecstep2,      \
              (avg), veccn2, size, *(cov+i*(convstep)+j));                        \
        }                                                                       \
      }                                                                         \
    }                                                                           \
  } while(0)


#endif // _MATRIX_H_
