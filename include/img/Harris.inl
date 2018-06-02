
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"

// 定义模板运算函数
// im：输入图像
// tp：模板参数
int mbys(double* im, int imW, int imH, const double* tp, int tpW, int tpH, double* out)
{
  int i, j, m, n;
#define im(ROW,COL) im[imW*(ROW)+(COL)]
#define tp(ROW,COL) tp[tpW*(ROW)+(COL)]
#define out(ROW,COL) out[imW*(ROW)+(COL)]

  memset(out, 0, imW * imH * sizeof(double));

  for (i = 0; i < imH; i++) {
    for (j = 0; j < imW; j++) {
      double a = 0;

      //去掉靠近边界的行
      if (i > (tpH / 2) && i < imH - (tpH / 2) && j > (tpW / 2) && j < imW - (tpW / 2)) {
        for (m = 0; m < tpH; m++) {
          for (n = 0; n < tpW; n++) {
            a += im(i + m - (tpH / 2), j + n - (tpW / 2)) * tp(m, n);
          }
        }
      }

      out(i, j) = a;
    }
  }

  return 0;
}

// int cxDIB          图像宽度
// int cyDIB          图像高度
// long lLineBytes    计算灰度图像每行的字节数
// gausswidth =5      二维高斯窗口宽度
// sigma=0.8          高斯函数的方差
// size =5            非极大值抑制的邻域宽度
// thresh=5000        最终确定角点所需的阈值

int ImgHarris(int cyDIB, int cxDIB, const unsigned char* lpDIBBits, int lLineBytes, unsigned char* corner,
    int gausswidth, float sigma, int size, int thresh)
{
  int i, j, m, n;
  const unsigned char* lpSrc; //一个指向源、目的像素的移动指针

  //创建I、Ix、Ix2、Iy、Iy2、Ixy、cim、mx、corner数组
  double* I = MALLOC(double, cxDIB * cyDIB);
  double* Ix = MALLOC(double, cxDIB * cyDIB);
  double* Ix2 = MALLOC(double, cxDIB * cyDIB);
  double* Iy = MALLOC(double, cxDIB * cyDIB);
  double* Iy2 = MALLOC(double, cxDIB * cyDIB);
  double* Ixy = MALLOC(double, cxDIB * cyDIB);
  double* cim = MALLOC(double, cxDIB * cyDIB);
  double* mx = MALLOC(double, cxDIB * cyDIB);

  // corner=MALLOC(unsigned char, cxDIB*cyDIB);
  // memset(corner, 0, cxDIB*cyDIB*sizeof(unsigned char));

  //定义宏以方便访问元素
#define I(ROW,COL)      I[cxDIB*(ROW)+(COL)]
#define Ix(ROW,COL)     Ix[cxDIB*(ROW)+(COL)]
#define Ix2(ROW,COL)    Ix2[cxDIB*(ROW)+(COL)]
#define Iy(ROW,COL)     Iy[cxDIB*(ROW)+(COL)]
#define Iy2(ROW,COL)    Iy2[cxDIB*(ROW)+(COL)]
#define Ixy(ROW,COL)    Ixy[cxDIB*(ROW)+(COL)]
#define cim(ROW,COL)    cim[cxDIB*(ROW)+(COL)]
#define mx(ROW,COL)     mx[cxDIB*(ROW)+(COL)]
#define corner(ROW,COL) corner[cxDIB*(ROW)+(COL)]

  //将图像灰度值复制到I中，这步很重要！想想为什么？
  for (i = 0; i < cyDIB; i++) {
    for (j = 0; j < cxDIB; j++) {
      lpSrc = (unsigned char*) lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
      //将256级灰度图像转化为double型
      I(i, j) = (double)(*lpSrc);
    }
  }

  //--------------------------------------------------------------------------
  //                     第一步：利用差分算子对图像进行滤波
  //--------------------------------------------------------------------------
  {
    //定义水平方向差分算子并求Ix
    const double dx[ 9 ] = {
      -1, 0, 1, -1, 0, 1, -1, 0, 1
    };
    const double dy[ 9 ] = {
      -1, -1, -1, 0, 0, 0, 1, 1, 1
    };
    mbys(I, cxDIB, cyDIB, dx, 3, 3, Ix);

    //定义垂直方向差分算子并求Iy
    mbys(I, cxDIB, cyDIB, dy, 3, 3, Iy);

    //将中间结果Ix写入到文本文件以便后续分析
    /*FILE *fp;
    fp=fopen("Ix.txt","w+");
    for(i = 0; i < cyDIB; i++) {
      for(j = 0; j < cxDIB; j++)
        fprintf(fp,"%f ",Ix(i,j));
      fprintf(fp,"\n");
    }
    */

    //计算Ix2、Iy2、Ixy
    for (i = 0; i < cyDIB; i++) {
      for (j = 0; j < cxDIB; j++) {
        Ix2(i, j) = Ix(i, j) * Ix(i, j);
        Iy2(i, j) = Iy(i, j) * Iy(i, j);
        Ixy(i, j) = Ix(i, j) * Iy(i, j);
      }
    }

  }

  //--------------------------------------------------------------------------
  //                  第二步：对Ix2/Iy2/Ixy进行高斯平滑，以去除噪声
  //--------------------------------------------------------------------------
  {
    // 本例中使用5×5的高斯模板
    // 计算模板参数
    double total = 0;
    double* g = MALLOC(double, gausswidth * gausswidth);

    for (i = 0; i < gausswidth; i++) {
      for (j = 0; j < gausswidth; j++) {
        g[ i * gausswidth + j ] = exp(-((i - (gausswidth / 2)) * (i - (gausswidth / 2)) + (j - (gausswidth / 2)) * (j - (gausswidth / 2))) / (2 * sigma));
      }
    }

    //归一化：使模板参数之和为1（其实此步可以省略）
    for (i = 0; i < gausswidth * gausswidth; i++) {
      total += g[ i ];
    }

    for (i = 0; i < gausswidth; i++) {
      for (j = 0; j < gausswidth; j++) {
        g[ i * gausswidth + j ] /= total;
      }
    }

    // 进行高斯平滑
    mbys(Ix2, cxDIB, cyDIB, g, gausswidth, gausswidth, Ix2);
    mbys(Iy2, cxDIB, cyDIB, g, gausswidth, gausswidth, Iy2);
    mbys(Ixy, cxDIB, cyDIB, g, gausswidth, gausswidth, Ixy);
    SAFEFREE(g);
  }

  //--------------------------------------------------------------------------
  //                        第三步：计算角点量
  //--------------------------------------------------------------------------
  //计算cim：即cornerness of image，我们把它称做‘角点量’
  for (i = 0; i < cyDIB; i++) {
    for (j = 0; j < cxDIB; j++) {
      //注意：要在分母中加入一个极小量以防止除数为零溢出
      cim(i, j) = (Ix2(i, j) * Iy2(i, j) - Ixy(i, j) * Ixy(i, j)) / (Ix2(i, j) + Iy2(i, j) + 0.000001);
    }
  }

  //--------------------------------------------------------------------------
  //                 第四步：进行局部非极大值抑制以获得最终角点
  //--------------------------------------------------------------------------
  //注意进行局部极大值抑制的思路
  //const double size=7;
  //对每个点在邻域内做极大值滤波：即将该点的值设为邻域中最大的那个值（跟中值滤波有点类似）
  for (i = 0; i < cyDIB; i++) {
    for (j = 0; j < cxDIB; j++) {
      double maxpix = -1000000;

      if (i > (size / 2) && i < cyDIB - (size / 2) && j > (size / 2) && j < cxDIB - (size / 2)) {
        for (m = 0; m < size; m++) {
          for (n = 0; n < size; n++) {
            if (cim(i + m - (size / 2), j + n - (size / 2)) > maxpix) {
              maxpix = cim(i + m - (size / 2), j + n - (size / 2));
            }
          }
        }
      }

      mx(i, j) = maxpix > 0 ? maxpix : 0;
    }
  }

  // 最终确定角点
  // const double thresh=4500;
  for (i = 0; i < cyDIB; i++) {
    for (j = 0; j < cxDIB; j++) {
      if (cim(i, j) == mx(i, j))         // 首先取得局部极大值
        if (mx(i, j) > thresh) {      // 然后大于这个阈值
          corner(i, j) = 1;  // 满足上两个条件，才是角点！
        }
    }
  }

  SAFEFREE(I);
  SAFEFREE(Ix);
  SAFEFREE(Ix2);
  SAFEFREE(Iy);
  SAFEFREE(Iy2);
  SAFEFREE(Ixy);
  SAFEFREE(cim);
  SAFEFREE(mx);
  return 0;
}
