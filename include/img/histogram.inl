
#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include <float.h>

#define CC_COMP_CORREL        0
#define CC_COMP_CHISQR        1
#define CC_COMP_INTERSECT     2
#define CC_COMP_BHATTACHARYYA 3

#define CC_MAX_DIM            32

#define CC_ERROR(X, Y)   do {} while(0)

#define cvSqrt(value)  ((float)sqrt(value))

/*
CompareHist
比较两个稠密直方图

hist1 第一个稠密直方图
hist2 第二个稠密直方图
method 比较方法，采用其中之一：
CC_COMP_CORREL
CC_COMP_CHISQR
CC_COMP_INTERSECT
函数 cvCompareHist 采用下面指定的方法比较两个稠密直方图(H1 表示第一个， H2 - 第二个):

相关 (method=CC_COMP_CORREL):
  d(H1,H2)=sumI(H'1(I)?H'2(I))/sqrt(sumI[H'1(I)2]?sumI[H'2(I)2])
其中
  H'k(I)=Hk(I)-1/N?sumJHk(J) (N=number of histogram bins)
  Chi-square(method=CC_COMP_CHISQR):
  d(H1,H2)=sumI[(H1(I)-H2(I))/(H1(I)+H2(I))]

交叉 (method=CC_COMP_INTERSECT):
  d(H1,H2)=sumImax(H1(I),H2(I))

函数返回 d(H1,H2) 的值。
为了比较稀疏直方图或更一般的加权稀疏点集(译者注：直方图匹配是图像检索中的常用方法)，考虑使用函数 cvCalcEMD 。
 */
double CompareHist(int total, float* ptr1, float* ptr2, int method)
{
  double result = 0.;
  int i;

  switch (method) {
  case CC_COMP_CHISQR:
    for (i = 0; i < total; i++) {
      double a = ptr1[ i ] - ptr2[ i ];
      double b = ptr1[ i ] + ptr2[ i ];

      if (fabs(b) > DBL_EPSILON) {
        result += a * a / b;
      }
    }

    break;

  case CC_COMP_CORREL: {
    double s1 = 0, s11 = 0;
    double s2 = 0, s22 = 0;
    double s12 = 0;
    double num, denom2, scale = 1. / total;

    for (i = 0; i < total; i++) {
      double a = ptr1[ i ];
      double b = ptr2[ i ];

      s12 += a * b;
      s1 += a;
      s11 += a * a;
      s2 += b;
      s22 += b * b;
    }

    num = s12 - s1 * s2 * scale;
    denom2 = (s11 - s1 * s1 * scale) * (s22 - s2 * s2 * scale);
    result = fabs(denom2) > DBL_EPSILON ? num / sqrt(denom2) : 1;
  }
  break;

  case CC_COMP_INTERSECT:
    for (i = 0; i < total; i++) {
      float a = ptr1[ i ];
      float b = ptr2[ i ];

      if (a <= b) {
        result += a;
      }
      else {
        result += b;
      }
    }

    break;

  case CC_COMP_BHATTACHARYYA:
    for (i = 0; i < total; i++) {
      result += cvSqrt(ptr1 [ i ] * ptr2 [ i ]);
    }

    result = cvSqrt(1. - result);
    break;

  default:
    CC_ERROR(CC_StsBadArg, "Unknown comparison method");
  }

  return result;
}

/*
CalcHist
计算图像image(s) 的直方图

image 输入图像s (虽然也可以使用 CvMat** ).
hist 直方图指针
accumulate
累计标识。如果设置，则直方图在开始时不被清零。这个特征保证可以为多个图像计算一个单独的直方图，或者在线更新直方图。
mask
操作 mask, 确定输入图像的哪个象素被计数
函数 cvCalcHist 计算单通道或多通道图像的直方图。 用来增加直方块的数组元素可从相应输入图像的同样位置提取。
*/
void CalcHist_8u(int height, int width, unsigned char** img, int step, int dims, unsigned char* mask, int maskStep, int* bins)
{
  int total = 1, i, x, histsize[ CC_MAX_DIM ];
  int* tab;

  for (i = 0; i < dims; i++) {
    total *= histsize[ i ];
  }

  switch (dims) {
  case 1: {
    int tab1d[ 256 ];
    memset(tab1d, 0, sizeof(tab1d));

    for (; height--; img[ 0 ] += step) {
      unsigned char* ptr = img[ 0 ];

      if (!mask) {
        for (x = 0; x <= width - 4; x += 4) {
          int v0 = ptr[ x ];
          int v1 = ptr[ x + 1 ];

          tab1d[ v0 ] ++;
          tab1d[ v1 ] ++;

          v0 = ptr[ x + 2 ];
          v1 = ptr[ x + 3 ];

          tab1d[ v0 ] ++;
          tab1d[ v1 ] ++;
        }

        for (; x < width; x++) {
          tab1d[ ptr[ x ] ] ++;
        }
      }
      else {
        for (x = 0; x < width; x++)
          if (mask[ x ]) {
            tab1d[ ptr[ x ] ] ++;
          }

        mask += maskStep;
      }
    }

    for (i = 0; i < 256; i++) {
      int idx = tab[ i ];

      if (idx >= 0) {
        bins[ idx ] += tab1d[ i ];
      }
    }
  }
  break;

  case 2:
    for (; height--; img[ 0 ] += step, img[ 1 ] += step) {
      unsigned char* ptr0 = img[ 0 ];
      unsigned char* ptr1 = img[ 1 ];

      if (!mask) {
        for (x = 0; x < width; x++) {
          int v0 = ptr0[ x ];
          int v1 = ptr1[ x ];
          int idx = tab[ v0 ] + tab[ 256 + v1 ];

          if (idx >= 0) {
            bins[ idx ] ++;
          }
        }
      }
      else {
        for (x = 0; x < width; x++) {
          if (mask[ x ]) {
            int v0 = ptr0[ x ];
            int v1 = ptr1[ x ];

            int idx = tab[ v0 ] + tab[ 256 + v1 ];

            if (idx >= 0) {
              bins[ idx ] ++;
            }
          }
        }

        mask += maskStep;
      }
    }

    break;

  case 3:
    for (; height--; img[ 0 ] += step, img[ 1 ] += step, img[ 2 ] += step) {
      unsigned char* ptr0 = img[ 0 ];
      unsigned char* ptr1 = img[ 1 ];
      unsigned char* ptr2 = img[ 2 ];

      if (!mask) {
        for (x = 0; x < width; x++) {
          int v0 = ptr0[ x ];
          int v1 = ptr1[ x ];
          int v2 = ptr2[ x ];
          int idx = tab[ v0 ] + tab[ 256 + v1 ] + tab[ 512 + v2 ];

          if (idx >= 0) {
            bins[ idx ] ++;
          }
        }
      }
      else {
        for (x = 0; x < width; x++) {
          if (mask[ x ]) {
            int v0 = ptr0[ x ];
            int v1 = ptr1[ x ];
            int v2 = ptr2[ x ];
            int idx = tab[ v0 ] + tab[ 256 + v1 ] + tab[ 512 + v2 ];

            if (idx >= 0) {
              bins[ idx ] ++;
            }
          }
        }

        mask += maskStep;
      }
    }

    break;

  default:
    for (; height--;) {
      if (!mask) {
        for (x = 0; x < width; x++) {
          int* binptr = bins;

          for (i = 0; i < dims; i++) {
            int idx = tab[ i * 256 + img[ i ][ x ] ];

            if (idx < 0) {
              break;
            }

            binptr += idx;
          }

          if (i == dims) {
            binptr[ 0 ] ++;
          }
        }
      }
      else {
        for (x = 0; x < width; x++) {
          if (mask[ x ]) {
            int* binptr = bins;

            for (i = 0; i < dims; i++) {
              int idx = tab[ i * 256 + img[ i ][ x ] ];

              if (idx < 0) {
                break;
              }

              binptr += idx;
            }

            if (i == dims) {
              binptr[ 0 ] ++;
            }
          }
        }

        mask += maskStep;
      }

      for (i = 0; i < dims; i++) {
        img[ i ] += step;
      }
    }
  }
}

#endif // _HISTOGRAM_H_
