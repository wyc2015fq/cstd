
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"
#include "xtypes.h"
#include "fractal.h"

//#include "MMTime.h"

//设置桌面图片API调用
#define SetWallPaper(pBmpFile)  SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, pBmpFile, \
    SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE)

typedef struct CSceneNewton2 {
  //控制参数
  int m_nRanSpec;
  int m_nFC;
  int m_nNN;
  int m_nR, m_nG, m_nB; //调色参数
  int m_nFA;
  double m_dX1, m_dY1, m_dX2, m_dY2;
  double m_dYsjb;
  double m_dAA, m_dRP, m_dRV;
  double m_dT17, m_dT30, m_dT31;
  int m_dSeData16; //特效编号

  //图片大小
  DWORD m_dwHeight, m_dwWidth;
}
CSceneNewton2;

//16位色时返回图片具体编码方式
int HGetPixelFormat()   //Pic As PictureBox)
{
  //由于没有查到相关资料（GetPixelFormat()和SetPixelFormat()），望高手指教: HouSisong@263.net
  //其实可以向屏幕绘制一个指定颜色，然后返回实际绘制的颜色值来判断颜色编码方式。
  //这里留给读者解决
  //HGetPixelFormat = 565;
  return 565;
}
void* CSceneNewton2Init(CSceneNewton2* sn)
{
  //设置默认参数值
  sn->m_nRanSpec = 0;     //颜色0:随机; 1:指定
  sn->m_dX1 = 2;          //坐标 X1
  sn->m_dY1 = 1.5;        //坐标 y1
  sn->m_dX2 = -2;         //坐标 X2
  sn->m_dY2 = -1.5;       //坐标 y2
  sn->m_nR = 118; //150;   //颜色 R
  sn->m_nG = 158; //250;   //颜色 G
  sn->m_nB = 108; //50;    //颜色 B
  sn->m_dYsjb = 5; //0;    //颜色 渐变参数a
  sn->m_dT30 = 0;         //颜色 渐变参数b （没有用到）
  sn->m_dT31 = 0;         //颜色 渐变参数c （没有用到）
  sn->m_dT17 = 0;         //颜色 渐变参数d （没有用到）
  sn->m_nFC = 2;          //选定的函数编号
  sn->m_nNN = 5;          //X^N-1=0 中的 N值
  sn->m_dAA = 3;          //Z*(1+Z^A)/(1-Z^A)=R 中的 A
  sn->m_dRP = 0;          //Z*(1+Z^A)/(1-Z^A)=R 中的 R的实部
  sn->m_dRV = -3;         //Z*(1+Z^A)/(1-Z^A)=R 中的 R的虚部
  sn->m_nFA = 6;          //着色方案编号
  sn->m_dSeData16 = 0;    //特效编号

  sn->m_nFC = 2;

  if (sn->m_nRanSpec) {
    sn->m_nR = rand() % 255;
    sn->m_nG = rand() % 255;
    sn->m_nB = rand() % 255;
    sn->m_nFC = 2; //rand()%4;
    sn->m_dYsjb += 1 + rand() * sn->m_dYsjb / RAND_MAX;
    //sn->m_nFA = rand()%32;
    //sn->m_dSeData16 = rand()%8;
  }

  //检查命令行参数，若是 *.HTXT 文件 则打开
  //文件名称中有可能在前后加上双引号(")，有的时候就去掉
  //刷新显示
  //调用开始绘图

  return sn;
}

//一些复变函数
//============================

//Arg(Z)  （复数的辐角）
double ZArg(double x, double y, double k)
{
  double ret = 0;

  if (!x) {
    if (!y) {
      ret = 0;
    }
    else if (y > 0) {
      ret = PI / 2;  //2#
    }
    else { //if y<0 then
      ret = PI * 1.5;
    }
  }
  else if (x > 0) {
    ret = atan2(y, x);

    if (ret < 0) {
      ret = PI * 2 + ret;
    }
  }
  else { //if x<0 then
    ret = atan2(y, x) + PI;
  }

  ret = ret + PI * 2 * k;

  return ret;
}

//Z1^Z2  （复数的复数次方）
void ZZPow(double x1, double y1, double x2, double y2, double k, double* pX, double* pY)
{
  double T, TT;
  double P, Fai;
  //    On Error Resume Next
  P = sqrt(x1 * x1 + y1 * y1);

  if (!P) {
    *pX = 0;
    *pY = 0;
    return ;
  }

  Fai = ZArg(x1, y1, k);
  T = pow(P, x2) * exp(-y2 * Fai);
  TT = log(P) * y2 + x2 * Fai;
  *pX = T * cos(TT);
  *pY = T * sin(TT);
}

//Z1*Z2  （复数乘积）
void ZMul(double x1, double y1, double x2, double y2, double* pX, double* pY)
{
  *pX = x1 * x2 - y1 * y2;
  *pY = x1 * y2 + y1 * x2;
}

//Z1/Z2  （复数商）
void ZDiv(double x1, double y1, double x2, double y2, double* pX, double* pY)
{
  double T;
  T = x2 * x2 + y2 * y2;

  if (!T) {
    if (!x1) {
      *pX = 1;
      *pY = 1;
    }
    else {
      *pX = (x1 > 0 ? 1 : (x1 < 0 ? -1 : 0)) * 1E+50;
      *pY = (y1 > 0 ? 1 : (y1 < 0 ? -1 : 0)) * 1E+50;
    }
  }
  else {
    *pX = (x1 * x2 + y1 * y2) / T;
    *pY = (-x1 * y2 + y1 * x2) / T;
  }
}

//Z^N  （复数的实数次方）
void ZPow(double x1, double y1, double N, double* pX, double* pY)
{
  double T, TT, AtnYX;
  T = sqrt(x1 * x1 + y1 * y1);
  AtnYX = atan2(y1, x1);

  if (x1 < 0) {
    TT = PI + AtnYX;
  }
  else if (y1 > 0) {
    TT = AtnYX;
  }
  else { //if y1<=0 then
    TT = PI * 2 + AtnYX;  //2#
  }

  T = pow(T, N);
  TT = TT * N;
  *pX = T * cos(TT);
  *pY = T * sin(TT);
}

//为了实现 Mandelbrot(曼德勃罗特集) 特效定义的函数 （其实就是Mandelbrot函数迭代）
void fz2(double x0, double y0, double xx, double yy, double* pX, double* pY, int N)     //Z=Z2+μ
{
  double t1, t2, t3, t4;
  int i;

  t1 = x0;
  t2 = y0;

  for (i = 1; i <= N; i++) {
    t3 = t1 * t1 - t2 * t2 + xx;
    t4 = 2 * t1 * t2 + yy;
    t1 = t3;
    t2 = t4;
  }

  *pX = t1;
  *pY = t2;
}
//实现不同方程的牛顿迭代法，并返回方程的各种基本性质
//调用时MMi(x0, y0, Int(SeData(0, 13)), M, nM, dX, dY, dL1, dL2, dL3, dL4)
// 牛顿迭代法解方程原理：
// 不失一般性设方程为： f(Z] = 0  （关于复数Z的函数）
// 对 f(Z) 求导函数得： f//(Z)
// 对任意复数Z0可以有Z1, Z1 = Z0 - f(Z0)/f//(Z0)
// 同样，对复数Z1可以有Z2, Z2 = Z1 - f(Z1)/f//(Z1)
// …… …… ……
// 则，有迭代式：Z(n+1)=Z(n)-f(Z(n))/f//(Z(n))
// 对于选定的起始点，迭代大多都会收敛于方程f(z] = 0 的某个根，
// 这就是牛顿迭代法解方程的基本方式；
// 但也可能存在许多点，使迭代根本就不会收敛，
// 甚至可能出现混沌的状态。

//nN 方程序号(>=1, 整数)
// M 迭代次数
//nM 最大迭代次数
//dX 第一次迭代x轴的变化率
//dY 第一次迭代y轴的变化率
//dL1  第一次迭代移动距离
//dL2  第nM次迭代移动距离
//dL3  第nM次迭代距离（0，0）点的距离
//dL4  迭代得到解以后距离解的大概距离
int CSceneNewton2MMi(CSceneNewton2* sn, double x0, double y0, int nN, int M, int nM,
    double* Hx, double* Hy, double* dL1, double* dL2, double* dL3, double* dL4)
{
  int i, k = 0;
  double x1, x2, y1, y2, m0;
  double SeTa1, P1, A;
  //  double SeTa2, P2, B;

  double temp0, temp1, temp2, temp3 = 0, temp4 = 0;
  double temp5, temp6, temp7, temp8; //, temp9;
  double R1, R2, N7;
  double bx, by; //, xb, yb;

  int N, ret;
  int nSum;  //nSum值用来判断迭代是否已经到达根（方程的解），这是一种比较好的方式

  //On Error GoTo aaa:

  x1 = x0;
  y1 = y0;
  N = (int)(sn->m_nNN);
  N7 = 0.01;
  nSum = 0;

  switch (nN) {
  case 1:     //方程 Z^N-1=0
    for (i = 1; i <= M; i++) {
      P1 = sqrt(x1 * x1 + y1 * y1);
      SeTa1 = ZArg(x1, y1, 0);
      x2 = ((N - 1) * P1 * cos(SeTa1) + pow(P1, 1 - N) * cos((1 - N) * SeTa1)) / N;
      y2 = ((N - 1) * P1 * sin(SeTa1) + pow(P1, 1 - N) * sin((1 - N) * SeTa1)) / N;
      temp0 = pow(fabs(fabs(x1) - fabs(x2)), 2) + pow(fabs(fabs(y1) - fabs(y2)), 2);

      if (temp0 < N7) {
        nSum++;

        if (nSum > 2) {
          *dL4 = temp0 / N7;

          if (i > nM) {
            break;
          }
        }
      }

      if (i == 1) {
        *Hx = fabs(x1 - x2);
        *Hy = fabs(y1 - y2);
        *dL1 = pow((*Hx) * (*Hx) + (*Hy) * (*Hy), 0.5);
      }

      if (i == nM) {
        *dL2 = pow((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2), 0.5);
        *dL3 = pow(x2 * x2 + y2 * y2, 0.5);
      }

      x1 = x2;
      y1 = y2;
    }

    ret = (k - 1) * M / N + i;
    break;

  case 0:    //方程 Z^3-1=0 的特解
  case 2:    //方程 Z^3-1=0 的特解
    x1 = -x1;  //其实没有必要，这里做了一下水平翻转

    for (i = 1; i <= M; i++) {
      double txy1, txy2;
      //double x1_2 = x1*x1;
      //double y1_2 = y1*y1;
      x2 = x1;
      y2 = y1;
      m0 = (x1 * x1 + y1 * y1);
      m0 *= m0;
      txy1 = (x1 * x1 * x1 - x1 * y1 * y1 * 3 - 1);
      txy2 = (x1 * x1 * y1 * 3 - y1 * y1 * y1);
      txy1 *= txy1;
      txy2 *= txy2;

      if (txy1 + txy2 < N7) {
        *dL4 = fabs(sqrt(x1 * x1 + y1 * y1) - 1) / N7;

        if (i > nM) {
          break;
        }
      }

      x1 = x2 - (x2 + y2) / 6 + (x2 * x2 - y2 * y2 - x2 * y2 * 2) / 6 / m0;
      y1 = y2 + (x2 - y2) / 6 + (y2 * y2 - x2 * x2 - x2 * y2 * 2) / 6 / m0;

      if (i == 1) {
        *Hx = fabs(x1 - x2);
        *Hy = fabs(y1 - y2);
        *dL1 = sqrt((*Hx) * (*Hx) + (*Hy) * (*Hy));
      }

      if (i == nM) {
        *dL2 = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        *dL3 = sqrt(x1 * x1 + y1 * y1);
      }
    }

    if (x1 > 0.9) {
      ret = (int)(i + 1 * 0.33 * M);
    }
    else if (y1 > 0.8) {
      ret = (int)(i + (0.33 + 1 * 0.33) * M);
    }
    else if (y1 < -0.8) {
      ret = (int)(i + (0.66 + 0.33 * 1) * M);
    }

    break;

  case 3:     //方程 Z*(1+Z^A)/(1-Z^A)=R
    bx = x0;
    by = y0;
    A = sn->m_dAA; //: B = SeData(0, 16)
    R1 = sn->m_dRP;
    R2 = sn->m_dRV;

    for (i = 1; i <= M; i++) {
      temp1 = bx;
      temp2 = by;
      ZPow(temp1, temp2, A, &temp3, &temp4);
      ZMul(temp3, temp4, temp1, temp2, &temp5, &temp6);
      ZMul(temp3, temp4, R1, R2, &temp7, &temp8);
      temp5 += temp7 - R1 + temp1;
      temp6 += temp8 - R2 + temp2;
      ZDiv(temp3, temp4, temp1, temp2, &temp7, &temp8);
      temp7 += 1 + (A + 1) * temp3;
      temp8 += (A + 1) * temp4;
      ZDiv(temp5, temp6, temp7, temp8, &temp3, &temp4);

      bx = temp1 - temp3;
      by = temp2 - temp4;

      temp0 = pow(fabs(fabs(bx) - fabs(temp1)), 2) +
          pow(fabs(fabs(by) - fabs(temp2)), 2);

      if (temp0 < N7) {
        nSum++;

        if (nSum > 2) {
          *dL4 = temp0 / N7;

          if (i > nM) {
            break;
          }
        }
      }

      if (i == 1) {
        *Hx = fabs(bx - temp1);
        *Hy = fabs(by - temp2);
        *dL1 = pow(((*Hx) * (*Hx) + (*Hy) * (*Hy)), 0.5);
      }

      if (i == nM) {
        *dL2 = pow(((bx - temp1) * (bx - temp1) + (by - temp2) * (by - temp2)), 0.5);
        *dL3 = pow((bx * bx + by * by), 0.5);
      }
    }

    ret = i;
    break;

  case 4:
#if 0

    {
      //    曼德勃罗特集的数学模型非常简单。假设有复数Z和μ，用下式
      //迭代计算：Z=Z2+μ，由于μ的取值不同，经过若干次数迭代以后Z
      //的幅值可能趋向无穷，也可能保持有界，曼德勃罗特集就是那些使
      //Z保持有界的μ的集合，把μ在复平面上的分布作成图像，就像这里
      //演示的那样具有极其复杂的结构。
      int nThre = 100;    //nThre代表设置的门限值，当迭代后Z的幅值的平方大于nThre则认为趋于无穷
      //    int xCord,yCord;  //xCord和yCord分别代表平面上的点的坐标位置
      int nColor = 0, nTimes; //nColor代表作图时所使用的颜色，nTimes表示迭代的次数
      double reP, rePmin = -1.5, rePmax = 0.75; //reP表示由平面上一点所代表的μ值的实部，rePmax、rePmin分别代表μ值实部可取的最大值和最小值
      double imP, imPmin = -1.5, imPmax = 1.5; //imP、imPmax和imPmin分别代表μ的虚部及其容许的最大值和最小值
      double reZ, imZ; //reZ和imZ分别代表Z值的实部和虚部
      double deltImP, deltReP; //，deltImP、deltReP表示每一个象素代表的实部和虚部值的大小
      double absZ; //absZ代表Z值的幅值
      double tmpReZ;

      rePmin = (double) sn->m_dX1;
      imPmin = (double) sn->m_dY1;
      rePmax = (double) sn->m_dX2;
      imPmax = (double) sn->m_dY2;
      //计算屏幕上一个像素表示的实部和虚部值的大小
      deltReP = (rePmax - rePmin) / sn->m_dwWidth ;
      deltImP = (imPmax - imPmin) / sn->m_dwHeight;
      //计算屏幕上一点所代表的μ值大小
      reP = rePmin + deltReP * x0;
      imP = imPmin + deltImP * y0;
      reZ = 0;
      imZ = 0;

      for (nTimes = 0; nTimes < M; nTimes++) { //160
        tmpReZ = reZ * reZ - imZ * imZ + reP; //计算迭代后的Z
        imZ = 2 * reZ * imZ + imP; //值及其幅值大小
        reZ = tmpReZ;
        absZ = reZ * reZ + imZ * imZ;

        if (absZ > nThre) {
          //nColor=nTimes/10;
          break;
        }

        //如果幅值的平方大于门限值终止迭代
        //nColor=0;
      }

      *Hx = reZ;
      *Hy = imZ;
      *dL1 = absZ;
    }

#endif
    break;
  }

  return ret;
  //  Exit Function
  //aaa:
  //  ret = i;
}

//曼德勃罗特集的数学模型非常简单。假设有复数Z和μ，用下式迭代计算：Z=Z2+μ
int CSceneNewton2MDBLT(CSceneNewton2* sn, int x0, int y0)
{
  //    曼德勃罗特集的数学模型非常简单。假设有复数Z和μ，用下式
  //迭代计算：Z=Z2+μ，由于μ的取值不同，经过若干次数迭代以后Z
  //的幅值可能趋向无穷，也可能保持有界，曼德勃罗特集就是那些使
  //Z保持有界的μ的集合，把μ在复平面上的分布作成图像，就像这里
  //演示的那样具有极其复杂的结构。
  //    曼德勃罗特集的数学模型非常简单。假设有复数Z和μ，用下式
  //迭代计算：Z=Z2+μ，由于μ的取值不同，经过若干次数迭代以后Z
  //的幅值可能趋向无穷，也可能保持有界，曼德勃罗特集就是那些使
  //Z保持有界的μ的集合，把μ在复平面上的分布作成图像，就像上
  //面演示的那样具有极其复杂的结构。
  int nThre = 10000000; //100;    //nThre代表设置的门限值，当迭代后Z的幅值的平方大于nThre则认为趋于无穷
  //    int xCord,yCord;  //xCord和yCord分别代表平面上的点的坐标位置
  int nColor, nTimes; //nColor代表作图时所使用的颜色，nTimes表示迭代的次数
  double reP, rePmin = -1.5, rePmax = 0.75; //reP表示由平面上一点所代表的μ值的实部，rePmax、rePmin分别代表μ值实部可取的最大值和最小值
  double imP, imPmin = -1.5, imPmax = 1.5; //imP、imPmax和imPmin分别代表μ的虚部及其容许的最大值和最小值
  double reZ, imZ; //reZ和imZ分别代表Z值的实部和虚部
  double deltImP, deltReP; //，deltImP、deltReP表示每一个象素代表的实部和虚部值的大小
  double absZ; //absZ代表Z值的幅值
  double tmpReZ;

  rePmin = (double) sn->m_dX1;
  imPmin = (double) sn->m_dY1;
  rePmax = (double) sn->m_dX2;
  imPmax = (double) sn->m_dY2;
  //计算屏幕上一个像素表示的实部和虚部值的大小
  deltReP = (rePmax - rePmin) / sn->m_dwWidth ;
  deltImP = (imPmax - imPmin) / sn->m_dwHeight;
  //计算屏幕上一点所代表的μ值大小
  reP = rePmin + deltReP * x0;
  imP = imPmin + deltImP * y0;
  reZ = 0;
  imZ = 0;

  for (nTimes = 0; nTimes < 256; nTimes++) { //160
    tmpReZ = reZ * reZ - imZ * imZ + reP; //计算迭代后的Z
    imZ = 2 * reZ * imZ + imP; //值及其幅值大小
    reZ = tmpReZ;
    absZ = reZ * reZ + imZ * imZ;

    if (absZ > nThre) {
      nColor = nTimes * 10; // /10;

      if (nColor > 255) {
        nColor %= 256;
      }

      break;
    }

    //如果幅值的平方大于门限值终止迭代
    nColor = 0;
  }

  return nColor;
  //    上面的程序中，可以通过改变imPmin、imPmax以及rePmin与rePmax的值
  //来对图像的局部细节进行放大，从而可以观察到更加精美复杂的图像，领悟
  //分形图像的神韵。
  //    曼德勃罗特集是人们发现的最早的分形图形之一，也是人们研究最多的
  //分形图形之一，今年的菲尔兹奖（数学界的最高奖）得主麦克马兰就是因为
  //在曼德勃罗特集研究中的成绩而获此殊荣的。这些年关于分形的研究已经渗
  //透到科学领域的各个学科，在计算机领域利用分形的方法来对自然景物进行
  //逼真的模拟是一个很具挑战性的研究方向。
  //      (兰州大学2041信箱 梁昌霖 730000)
}


//=======================================================

//BYTE PicDataOld[]; //实现RGB调整时用来保存原图像的数据
//BYTE PicDataNew[]; //实现RGB调整时用来保存调整后的图像的数据
//int PicLeng;       //实现RGB调整时用来保存图像数据的长度(字节)
//int PicBit;        //实现RGB调整时用来保存图像的颜色位数
//double sn->m_dRGBTime;

//++++++++++++更改图像RGB数据++++++++++++++
void CSceneNewton2EditRGB(BYTE* pRGBData0, BYTE* pRGBData1, int dLeng, int PicBit,
    int dR, int dG, int dB, double RGBTimeNow, double m_dRGBTime)    //更改图像RGB数据（不考虑256色和16色）
{
  int i, temp, tempR, tempG, tempB, tempColor;

  //如果把图片的颜色位数一早定为24位，则程序将变得很好处理，而且还有很多其他好处...
  switch (PicBit) {
  case 32:        //32位色深
    for (i = 0; i < dLeng; i++) {
      switch (i % 4) {
      case 0:
        temp = pRGBData0[ i ] + dB;
        break;

      case 1:
        temp = pRGBData0[ i ] + dG;
        break;

      case 2:
        temp = pRGBData0[ i ] + dR;
        break;

      case 3:
        temp = pRGBData0[ i ]; //透明度，一般给0就可以了
        break;
      }

      if (temp > 255) {
        temp = 255;
      }
      else if (temp < 0) {
        temp = 0;
      }

      pRGBData1[ i ] = temp;

      if (!(i % 50)) {
        //        DoEvents
        if (RGBTimeNow != m_dRGBTime) {
          return ;
        }
      }
    }

    break;

  case 24:       //24位色深
    for (i = 0; i < dLeng; i++) {
      switch (i % 3) {
      case 0:
        temp = pRGBData0[ i ] + dB;
        break;

      case 1:
        temp = pRGBData0[ i ] + dG;
        break;

      case 2:
        temp = pRGBData0[ i ] + dR;
        break;
      }

      if (temp > 255) {
        temp = 255;
      }
      else if (temp < 0) {
        temp = 0;
      }

      pRGBData1[ i ] = temp;

      if (!(i % 50)) {
        //        DoEvents
        if (RGBTimeNow != m_dRGBTime) {
          return ;
        }
      }
    }

    break;

  case 16:
    //16位色深在某些机子上可能有问题,因为16位色编码方式是由硬件厂商决定的:
    //大部分机子采用的编码方式：R:9-13,G:14-16,1-3,B:4-8 (5,6,5)
    //某些机子上可能为 R:10-14,G:15-16,1-3,B:4-8,空（或作为是否透明标志）:9  (5,5,5,1)
    //还有的机子为：R:9-14,G:15-16,1-3,B:4-8 (6,5,5)
    //由于没有查到相关资料（API函数：GetPixelFormat()和SetPixelFormat()），望高手指教: HouSisong@263.net

    tempColor = HGetPixelFormat(); //frmMain.Picture1);  //自己编的替代函数，16位色时返回具体编码方式。

    if (tempColor == 565) {    //默认方式  R:9-13,G:14-16,1-3,B:4-8
      for (i = 0; i < dLeng / 2; i++) {
        tempB = pRGBData0[ 2 * i ] % 32;
        tempG = pRGBData0[ 2 * i ] / 32 + (pRGBData0[ 2 * i + 1 ] % 8) * 8;
        tempR = pRGBData0[ 2 * i + 1 ] / 8;

        tempB = tempB + dB * 5 / 8;
        tempG = tempG + dG * 5 / 8;
        tempR = tempR + dR * 5 / 8;

        if (tempB > 31) {
          tempB = 31;
        }
        else if (tempB < 0) {
          tempB = 0;
        }

        if (tempG > 63) {
          tempG = 63;
        }
        else if (tempG < 0) {
          tempG = 0;
        }

        if (tempR > 31) {
          tempR = 31;
        }
        else if (tempR < 0) {
          tempR = 0;
        }

        pRGBData1[ 2 * i ] = tempB + 32 * (tempG % 8);
        pRGBData1[ 2 * i + 1 ] = tempG / 8 + tempR * 8;

        if (!(i % 50)) {
          //DoEvents
          if (RGBTimeNow != m_dRGBTime) {
            return ;
          }
        }
      }
    }
    else if (tempColor == 5551) {    // R:10-14,G:15-16,1-3,B:4-8,空:9
      for (i = 0; i < dLeng / 2; i++) {
        temp = pRGBData0[ 2 * i + 1 ] / 128;
        tempB = pRGBData0[ 2 * i ] % 32;
        tempG = pRGBData0[ 2 * i ] / 32 + (pRGBData0[ 2 * i + 1 ] % 4) * 8;
        tempR = (pRGBData0[ 2 * i + 1 ] % 128) / 4;

        tempB = tempB + dB * 5 / 8;
        tempG = tempG + dG * 5 / 8;
        tempR = tempR + dR * 5 / 8;

        tempB = tempB > 31 ? 31 : (tempB < 0 ? 0 : tempB);
        tempG = tempG > 31 ? 31 : (tempG < 0 ? 0 : tempG);
        tempR = tempR > 31 ? 31 : (tempR < 0 ? 0 : tempR);

        pRGBData1[ 2 * i ] = tempB + 32 * (tempG % 8);
        pRGBData1[ 2 * i + 1 ] = tempG / 8 + tempR * 4 + temp * 128;

        if (!(i % 50)) {
          //DoEvents
          if (RGBTimeNow != m_dRGBTime) {
            return ;
          }
        }
      }
    }
    else if (tempColor == 655) {   //R:6,G:5,B:5
      //没有处理代码
      //我在几台电脑上只测到了上面两种情况，这里留给读者解决
    }
  }
}

void DrawNewton2(IDC* pDC)
{
  // 具体执行绘制操作的过程
  int i;
  //临时用变量 temp
  int temp;
  double temp1, temp2; //, tempA, temp3, temp4, temp5;

  double x1, y1, x2, y2;  //记录坐标范围
  int M, nM;              //最大迭代次数；最小迭代次数

  //  double Tmang;            //防止同时运行多个本线程的代码
  int Se1, Se2, Se3;  //记录颜色的变量
  color_t se;
  int A, B, n;            //从中间开始向两边绘制时使用的变量
  double x0, y0;          //复平面上的一个点

  double Kn2; // As Single  //颜色渐变强度调节参数

  double Hssx, Hssy; //用来保存返回的函数的一项性质，用作颜色函数的参数,（下同 ）
  double dL1, dL2, dL3, dL4;

  //picX = Picture2.ScaleWidth: picY = Picture2.ScaleHeight  //绘图显示尺寸
  int picX = pDC->w;
  int picY = pDC->h;
  CSceneNewton2* sn;
  CSceneNewton2 ssn;
  CSceneNewton2Init(&ssn);
  sn = &ssn;

  sn->m_dwWidth = picX;
  sn->m_dwHeight = picY;
  //坐标范围
  x1 = sn->m_dX1;
  y1 = sn->m_dY1;
  x2 = sn->m_dX2;
  y2 = sn->m_dY2;

  Se1 = (int) sn->m_nR;
  Se2 = (int) sn->m_nG;
  Se3 = (int) sn->m_nB;   //调色参数
  A = picX / 2;  //从中间开始向两边绘制
  temp1 = -1;
  temp2 = -1;

  for (n = 0; n <= picX; n++) {
    A = A - ((n % 2) * 2 - 1) * n;            //总体上从中间向两边绘制

    for (B = 0; B <= picY; B++) {         //从上到下绘制
      if ((int)(sn->m_nFC) == 4) {
        int nColor = CSceneNewton2MDBLT(sn, A, B);
        SetPixel(pDC, A, B, nColor, nColor > 127 ? nColor : 128 + nColor, 255 - nColor);
        continue;
      }

      x0 = (x2 - x1) * A / picX + x1;
      y0 = (y2 - y1) * B / picY + y1;   //获得复平面上的一个点

      if (x0 == 0) {
        x0 = 1E-150;  //最好不要有（0,0）点
      }

      if (y0 == 0) {
        y0 = 1E-150;
      }

      //特效处理（4重），原理是对（x0,y0）点进行某种变换，使点映射得一个新的复表面上
      for (i = 3; i >= 0; i--) {
        switch ((int)(sn->m_dSeData16 / (100 ^ i)) % 100) {
        case 0:
          //无
          break;

        case 1:    //圆
          temp1 = sqrt(x0 * x0 + y0 * y0);
          temp2 = ZArg(y0, x0, 0);

          temp = (int)(temp1);

          if (temp != 1) {
            temp1 -= temp;
            temp = (temp1 > 0 ? 1 : (temp1 < 0 ? -1 : 0));
            x0 = temp * (sqrt(2 * temp1 * temp1 / (1 - temp1 * temp1)) + temp1);
            y0 = temp2;
          }
          else {
            x0 = temp1;
            y0 = 45 / 180 * PI;
          }

          temp1 = x0;
          temp2 = y0;
          x0 = temp1 * cos(temp2);
          y0 = temp1 * sin(temp2);
          break;

        case 2:    //加倍
          temp1 = x0 * x0 - y0 * y0;
          temp2 = 2 * x0 * y0;
          x0 = temp1;
          y0 = temp2;
          break;

        case 3:    //平铺
          temp1 = tan(x0 / 1.5);
          temp2 = tan(y0);
          x0 = temp1;
          y0 = temp2;
          break;

        case 4:    //网
          temp1 = sin(x0 * 2 + y0 * 2);
          temp2 = cos(x0 * 2 - y0 * 2);
          x0 = temp1;
          y0 = temp2;
          break;

        case 5:    //黎曼面
          temp1 = sqrt(x0 * x0 + y0 * y0);

          temp2 = ZArg(x0, y0, 0);
          temp1 = tan(temp1 * 1.5) * 2;

          x0 = temp1 * sin(temp2);
          y0 = temp1 * cos(temp2);
          break;

        case 6:      //1/c
          ZDiv(1, 0, x0, y0, &temp1, &temp2);
          x0 = temp1;
          y0 = temp2;
          break;

        case 7:      //Mandelbrot
          fz2(x0 - 0.5, y0, x0 - 0.5, y0, &temp1, &temp2, 4);
          x0 = temp1;
          y0 = temp2;
          break;
        }
      }


      //颜色方案处理原理
      //      _RGB(256 - ((x1 % 512) - 256),
      //          256 - ((x2 % 512) - 256),
      //          256 - ((x3 % 512) - 256))
      //程序中这样可以保持颜色的渐变

      //这样更好：  _RGB(255 - ((x1 % 511) - 255),
      //                255 - ((x2 % 511) - 255),
      //                255 - ((x3 % 511) - 255))
      //
      se = _RGB((int) sn->m_nR, (int) sn->m_nG, (int) sn->m_nB);         //默认颜色
#define iabs abs
#define XABS(_X)  (256 - iabs(((int)fabs(_X) % 512) - 256))
#define XRGB(_XR, _XG, _XB) _RGB(XABS(_XR), XABS(_XG), XABS(_XB))
#define YRGB(_XR, _XG, _XB) XRGB(Se1 - (2*Se2-256) * (_XR), Se2 - (2*Se3-256) * (_XG), Se3 - (2*Se1-256) * (_XB))

      //颜色方案 （其中有的效果有上面使用的特效处理方式，特效独立出来是由伍胜富先生提出的）
      switch ((int) sn->m_nFA + 1) {
      case 1:
        M = 10000;
        nM = 11;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb + 10;
        se = YRGB(pow((M - i) * 2, Kn2) / M,
            pow((M - i) * 2, Kn2) / M,
            pow((M - i) * 2, Kn2) / M);
        break;

      case 2:
        M = 3;
        nM = 1;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb / 2;
        se = YRGB(pow((log(fabs(Hssy))) * 2, Kn2),
            pow((log(dL1)) * 2, Kn2),
            pow((log(fabs(Hssx))) * 2, Kn2));
        break;

      case 3:
        M = 3;
        nM = 1;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = (sn->m_dYsjb + 5) / 2;
        se = YRGB(pow(log(dL1) * 2, Kn2),
            pow((fabs(cos(Hssx))) * 2, Kn2),
            pow((fabs(cos(Hssy))) * 2, Kn2));
        break;

      case 4:
        M = 3;
        nM = 1;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb / 2;

        se = YRGB(pow((dL1 / Hssx) * 2, Kn2),
            pow((tan(Hssy) / dL1) * 2, Kn2),
            pow((Hssx / Hssy) * 2, Kn2));
        break;

      case 5:

        temp1 = sqrt(x0 * x0 + y0 * y0);
        temp2 = ZArg(y0, x0, 0);

        temp = (int)(temp1);

        if (temp != 1) {
          int sgn;
          temp1 -= temp;
          sgn = temp1 > 0 ? 1 : (temp1 < 0 ? -1 : 0);
          x0 = sgn * (sqrt(2 * temp1 * temp1 / (1 - temp1 * temp1)) + temp1);
          y0 = temp2;
        }
        else {
          x0 = temp1;
          y0 = 45 / 180 * PI;
        }

        temp1 = x0;
        temp2 = y0;
        x0 = temp1 * cos(temp2);
        y0 = temp1 * sin(temp2);

        M = 1000;
        nM = 15;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        if (!i) {
          i = 7;
        }

        dL2 = dL2;
        Kn2 = sn->m_dYsjb + 10;
        se = YRGB(pow((sin(log(1 / i) + log(fabs(dL2)))) * 2, Kn2) / M,
            pow((cos(log(1 / i) + log(fabs(dL2)))) * 2, Kn2) / M,
            pow((sin(log(1 / i) + log(fabs(dL2)))) * 2, Kn2) / M);
        break;

      case 6:
        M = 3;
        nM = 1;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb / 2;
        se = _RGB(66, 124, 221);
        se = YRGB(pow((log(log(fabs(Hssy)))) * 2, Kn2),
            pow((log(log(dL1))) * 2, Kn2),
            pow((log(log(fabs(Hssx)))) * 2, Kn2));
        break;

      case 7:
        M = 13;
        nM = 11;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1.5f;
        se = YRGB(pow((log(fabs(Hssx * Hssx + 0.000001))) * 2, Kn2),
            pow((log(dL2 + 0.000001)) * 2, Kn2),
            pow((log(fabs(Hssy * Hssy + 0.000001))) * 2, Kn2));
        break;

      case 8:
        M = 4;
        nM = 2;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1.5f;
        se = YRGB(pow((log(fabs(Hssx * Hssx + 0.000001))) * 2, Kn2),
            pow((log((dL2 * 100 + Hssy + 0.000001))) * 2, Kn2),
            pow((log(fabs(Hssy * Hssy + 0.000001))) * 2, Kn2));
        break;

      case 9:
        M = 5;
        nM = 3;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1.5f;
        se = YRGB(pow((log(fabs((Hssx + Hssy) * (Hssx + Hssy) + 0.000001))) * 2, Kn2),
            pow((log((dL2 * 100 + 0.000001))) * 2, Kn2),
            pow((log(fabs((Hssx + Hssy) * (Hssx + Hssy) + 0.000001))) * 2, Kn2));
        break;

      case 10:
        M = 4;
        nM = 3;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1;
        se = YRGB(pow((log(fabs((Hssy * Hssy) / (Hssx * Hssx) + 0.000001))) * 2, Kn2),
            pow((log((dL2 + 0.000001))) * 2, Kn2),
            pow((log(fabs((Hssx + Hssy) * (Hssx + Hssy) + 0.000001))) * 2, Kn2));
        break;

      case 11:
        M = 22;
        nM = 20;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1;
        se = YRGB(pow((cos(log(fabs(Hssy + 0.000001)))) * 2, Kn2),
            pow((log((dL2 / dL1 * Hssx * Hssy + 0.000001))) * 2, Kn2),
            pow((cos(log(fabs(Hssx + 0.000001)))) * 2, Kn2));
        break;

      case 12:
        M = 14;
        nM = 13;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1;

        temp1 = atan2(Hssy, Hssx) + dL2;
        temp2 = atan2(Hssx, Hssy) - dL2;
        se = YRGB(pow((fabs(temp1)) * 2, Kn2),
            pow(log(fabs(atan(log(fabs(sin(dL2) * sin(Hssx) + cos(Hssy)))))) * 2, Kn2),
            pow((fabs(temp2) - fabs(temp1)) * 2, Kn2));
        break;

      case 13:
        M = 10;
        nM = 9;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f + 3;

        se = YRGB(pow(log(fabs(sin(cos(dL3)))) * 2, Kn2),
            pow(log(fabs(cos(sin(dL3)))) * 2, Kn2),
            pow(log(fabs(sin(dL3))) * 2, Kn2));
        break;

      case 14:
        M = 1000;
        nM = 1;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f + 3;

        se = YRGB(pow(log(fabs(sin(cos(log(dL4))))) * 2, Kn2),
            pow(log(fabs(cos(sin(log(dL4))))) * 2, Kn2),
            pow(log(fabs(sin(log(dL4)))) * 2, Kn2));
        break;

      case 15:
        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;
        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;
        M = 23;
        nM = 21;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 3.5f;
        se = YRGB(pow((log(fabs(Hssx * Hssx + 0.000001))) * 2, Kn2),
            pow((10 * log((dL2 + 0.000001))) * 2, Kn2),
            pow((log(fabs(Hssy * Hssy + 0.000001))) * 2, Kn2));
        break;

      case 16:
        M = 1000;
        nM = 11;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb + 12;
        se = YRGB(pow((sin(M - i / 1000 + log(fabs(dL2) + log(fabs(dL1))))) * 2, Kn2) / M,
            pow((cos(M - i / 1000 + log(fabs(dL2) + log(fabs(dL3))))) * 2, Kn2) / M,
            pow((sin(M - i / 1000 + (fabs(dL2)))) * 2, Kn2) / M);
        break;

      case 17:
        M = 1000;
        nM = 11;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        Kn2 = sn->m_dYsjb + 9;
        se = YRGB(pow((log(dL1 + 0.001 * dL2)) * 2, Kn2) / M,
            pow((log(dL1 + 0.001 * dL2)) * 2, Kn2) / M,
            pow((log(dL2 + 0.001 * dL2)) * 2, Kn2) / M);
        break;

      case 18:
        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;
        temp1 = sin(x0);
        temp2 = sin(y0);
        x0 = temp1;
        y0 = temp2;
        M = 23;
        nM = 21;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 3.5f;
        se = YRGB(pow((log(fabs(Hssx * Hssx + 0.000001))) * 2, Kn2),
            pow((10 * log((dL2 + 0.000001))) * 2, Kn2),
            pow((log(fabs(Hssy * Hssy + 0.000001))) * 2, Kn2));
        break;

      case 19:
        M = 1000;
        nM = 11;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        Kn2 = sn->m_dYsjb + 9;
        se = YRGB(pow(tan(fabs(log(log(dL2 + dL3) + sin(log(dL1 + dL3))))) * 2, Kn2) / M,
            pow(tan(fabs(log(log(dL2 + dL3) + sin(log(dL1 + dL3))))) * 2, Kn2) / M,
            pow(tan(fabs(log(log(dL2 + dL3) + sin(log(dL1 + dL3))))) * 2, Kn2) / M);
        break;

      case 20:
        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;

        M = 5;
        nM = 3;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1;
        se = YRGB(pow((log(fabs((Hssx + Hssy) * (Hssx + Hssy) + 0.000001))) * 2, Kn2),
            pow((log((dL2 * 100 + 0.000001))) * 2, Kn2),
            pow((log(fabs((Hssx + Hssy) * (Hssx + Hssy) + 0.000001))) * 2, Kn2));
        break;

      case 21:
        M = 1000;
        nM = 15;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        if (!i) {
          i = 21;
        }

        Kn2 = sn->m_dYsjb + 10;
        se = YRGB(pow((sin(log(1 / i) + log(fabs(dL2)))) * 2, Kn2) / M,
            pow((cos(log(1 / i) + log(fabs(dL2)))) * 2, Kn2) / M,
            pow((sin(log(1 / i) + log(fabs(dL2)))) * 2, Kn2) / M);
        break;

      case 22:
        M = 1000;
        nM = 15;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        if (!i) {
          i = 22;
        }

        Kn2 = sn->m_dYsjb + 10;
        se = YRGB(pow((sin(log(10 / i) + log(fabs(log(fabs(dL2)))))) * 2, Kn2) / M,
            pow((cos(log(10 / i) + log(fabs(log(fabs(dL2)))))) * 2, Kn2) / M,
            pow((sin(log(10 / i) + log(fabs(log(fabs(dL2)))))) * 2, Kn2) / M);
        break;

      case 23:
        temp1 = (x0 * x0) - (y0 * y0);
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;
        ZDiv(0.7, 0, x0, y0, &temp1, &temp2);
        x0 = temp1;
        y0 = temp2;

        M = 1000;
        nM = 30;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        if (!i) {
          i = 23;
        }

        Kn2 = sn->m_dYsjb + 10;
        se = YRGB(pow((sin(log(100 / i) + sin(fabs(log(fabs(dL2)))))) * 2, Kn2) / M,
            pow((cos(log(100 / i) + sin(fabs(log(fabs(dL2)))))) * 2, Kn2) / M,
            pow((sin(log(100 / i) + cos(fabs(log(fabs(dL2)))))) * 2, Kn2) / M);
        break;

      case 24:
        M = 1000;
        nM = 30;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        if (!i) {
          i = 24;
        }

        Kn2 = sn->m_dYsjb + 10;
        se = YRGB(pow((sin(log(100 / i) + 1 / (sin(fabs(log(fabs(dL2))))))) * 2, Kn2) / M,
            pow((cos(log(100 / i) + 1 / (sin(fabs(log(fabs(dL2))))))) * 2, Kn2) / M,
            pow((sin(log(100 / i) + 1 / (cos(fabs(log(fabs(dL2))))))) * 2, Kn2) / M);
        break;

      case 25:
        M = 1000;
        nM = 10;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        if (!i) {
          i = 25;
        }

        Kn2 = sn->m_dYsjb + 11;
        temp1 = tan(fabs(sin(fabs(log(fabs(dL4))))));
        temp2 = tan(fabs(sin(fabs(log(fabs(dL2))))));
        se = YRGB(pow((sin(log(100 / i) + temp1)) * 2, Kn2) / M,
            pow((cos(log(100 / i) + temp2)) * 2, Kn2) / M,
            pow((sin(log(100 / i) + temp1)) * 2, Kn2) / M);
        break;

      case 26:
        M = 1000;
        nM = 30;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i

        if (!i) {
          i = 26;
        }

        Kn2 = sn->m_dYsjb + 11;
        temp1 = tan(fabs(sin(fabs(log(fabs(dL4))))));
        temp2 = tan(fabs(sin(fabs(log(fabs(dL2))))));
        se = YRGB(pow((sin(log(100 / i) + temp1)) * 2, Kn2) / M,
            pow((cos(log(100 / i) + temp2)) * 2, Kn2) / M,
            pow((sin(log(100 / i) + temp1)) * 2, Kn2) / M);
        break;

      case 27:
        temp1 = (x0 * x0) - (y0 * y0);
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;
        ZDiv(0.7, 0, x0, y0, &temp1, &temp2);
        x0 = temp1;
        y0 = temp2;

        M = 5;
        nM = 3;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1;
        se = YRGB(pow((log(fabs((Hssy * Hssy) / (Hssx * Hssx) + 0.000001))) * 2, Kn2),
            pow((log((dL2 + 0.000001))) * 2, Kn2),
            pow((log(fabs((Hssx + Hssy) * (Hssx + Hssy) + 0.000001))) * 2, Kn2));
        break;

      case 28:

        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;

        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;

        M = 4;
        nM = 2;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1.5f;
        se = YRGB(pow((log(fabs(Hssx * Hssx + 0.000001))) * 2, Kn2),
            pow((log((dL2 * 100 + Hssy + 0.000001))) * 2, Kn2),
            pow((log(fabs(Hssy * Hssy + 0.000001))) * 2, Kn2));
        break;

      case 29:
        temp1 = tan(x0 / 1.5);
        temp2 = tan(y0);
        x0 = temp1;
        y0 = temp2;
        M = 13;
        nM = 11;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1.5f;
        se = YRGB(pow((log(fabs(pow(Hssy, 2.5) + 0.000001))) * 2, Kn2),
            pow((log((dL2 + 0.000001))) * 2, Kn2),
            pow((log(fabs(pow(Hssx, 2.5) + 0.000001))) * 2, Kn2));
        break;

      case 30:
        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;
        M = 23;
        nM = 21;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 3.5f;
        se = YRGB(pow((log(fabs(Hssx * Hssx + 0.000001))) * 2, Kn2),
            pow((3 * log((dL2 + 0.000001))) * 2, Kn2),
            pow((log(fabs(Hssy * Hssy + 0.000001))) * 2, Kn2));
        break;

      case 31:

        fz2(x0, y0, 0, 1, &temp1, &temp2, 2);
        x0 = temp1;
        y0 = temp2;

        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;
        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;

        M = 5;
        nM = 3;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1;
        se = YRGB(pow((log(fabs((Hssx + Hssy) * (Hssx + Hssy) + 0.000001))) * 2, Kn2),
            pow((log((dL2 * 100 + 0.000001))) * 2, Kn2),
            pow((log(fabs((Hssx + Hssy) * (Hssx + Hssy) + 0.000001))) * 2, Kn2));
        break;

      case 32:
        fz2(y0, x0, 0.3, 0.3, &temp1, &temp2, 4);
        x0 = temp1;
        y0 = temp2;

        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;

        temp1 = x0 * x0 - y0 * y0;
        temp2 = 2 * x0 * y0;
        x0 = temp1;
        y0 = temp2;

        M = 4;
        nM = 2;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb * 0.5f - 1.5f;
        se = YRGB(pow((log(fabs(Hssx * Hssx + 0.000001))) * 2, Kn2),
            pow((log((dL2 * 100 + Hssy + 0.000001))) * 2, Kn2),
            pow((log(fabs(Hssy * Hssy + 0.000001))) * 2, Kn2));
        break;

      default:
        M = 10000;
        nM = 11;
        i = CSceneNewton2MMi(sn, x0, y0, (int)(sn->m_nFC), M, nM, &Hssx, &Hssy, &dL1, &dL2, &dL3, &dL4);        //i
        Kn2 = sn->m_dYsjb + 11.5f;
        se = YRGB(pow((M - i) * 2, Kn2) / M,
            pow((M - i) * 2, Kn2) / M,
            pow((M - i) * 2, Kn2) / M);
      }

      //temp = memDC.SetPixelV(A, B, se);
      temp = SetPixelV(pDC, A, B, se);
      //因为单独的绘图操作占整个显示图像的时间比很少，所以这里没有做优化， _
      //但用API的函数还是比VB自带的快很多倍，（SetPixelV比SetPixel快）

    }

  }
}
