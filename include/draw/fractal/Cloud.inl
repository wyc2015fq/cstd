//二维空间中的分形插值算法
//① 在平面上划分正方形网络n×n，随机给A，B，C，D这4个角点的颜色。
//② 根据4个角点的颜色值的平均值，产生中点M的颜色。
//③ 根据A，B，M点和网格外一虚拟点（颜色值为0）求平均，得到边中点E的颜色；根据B，C，M点和网格外一虚拟点（颜色值为0）求平均，得到边中点F的颜色；根据C，D，M点和网格外一虚拟点（颜色值为0）求平均，得到边中点G的颜色；根据A，D，M点和网格外一虚拟点（颜色值为0）求平均，得到边中点H的颜色。
//④ 根据小正方形EBFM的4角点颜色的平均值，求出小正方形中点以及小正方形的边中点的颜色；然后再沿用上述规则计算小正方形MFCG的中点及边中点的颜色，以此类推，计算小正方形HMGD，AEMH的中点和边中点的颜色。 递归步骤
//，使其正方形网格不断细化，直至达到预期的递归深度 -Two-dimensional space fractal interpolation algorithm

#include <time.h>
#include <stdlib.h>
#include "macro.h"
#include "fractal.h"

#define MAX_LOADSTRING 100

double random()
{
  return rand() * 1. / RAND_MAX;
}
typedef color_t Color;
//依赖于网格中央位置的随机位移颜色值
double Displace(double height, double width, double num)
{
  return ((double) random() - 0.5f) * num / (width + height) * 3;
}

//返回一个基于颜色值的颜色c
Color ComputeColor(double c)
{
  double Red = 0;
  double Green = 0;
  double Blue = 0;

  if (c < 0.5f) {
    Red = c * 2;
  }
  else {
    Red = (1.0f - c) * 2;
  }

  if (c >= 0.3f && c < 0.8f) {
    Green = (c - 0.3f) * 2;
  }
  else if (c < 0.3f) {
    Green = (0.3f - c) * 2;
  }
  else {
    Green = (1.3f - c) * 2;
  }

  if (c >= 0.5f) {
    Blue = (c - 0.5f) * 2;
  }
  else {
    Blue = (0.5f - c) * 2;
  }

  return _RGB(255 * Red, 255 * Green, 255 * Blue);
}


//这是一个执行中点位移算法的递归函数，它将自己成为小于像素点的网格面片
void Midpoint(IDC* pDC, double x, double y,
    double width, double height,
    double r1, double r2, double r3, double r4)
{
  double M1, M2, M3, M4, Middle;
  double newWidth = width / 2;
  double newHeight = height / 2;

  if (width > 2. || height > 2.) {
    //随机中点位移
    Middle = ((r1 + r2 + r3 + r4) / 4. + Displace(height, width, newWidth + newHeight));
    M1 = (r1 + r2) / 2;   //计算网格两个角点中间的平均值
    M2 = (r2 + r3) / 2;
    M3 = (r3 + r4) / 2;
    M4 = (r4 + r1) / 2;

    //保证中点不能在“随机位移”确定的范围之外
    if (Middle < 0) {
      Middle = 0;
    }
    else if (Middle > 1.0f) {
      Middle = 1.0f;
    }

    //再次操作4个新的网格点
    Midpoint(pDC, x, y, newWidth, newHeight, r1, M1, Middle, M4);
    Midpoint(pDC, x + newWidth, y, newWidth, newHeight, M1, r2, M2, Middle);
    Midpoint(pDC, x + newWidth, y + newHeight, newWidth, newHeight, Middle, M2, r3, M3);
    Midpoint(pDC, x, y + newHeight, newWidth, newHeight, M4, Middle, M3, r4);
  }
  else {
    //这是基本状况，一些网格面片小于像素点的尺寸
    //网格面片的4个角点求平均，并画这个单独的点
    double c = (r1 + r2 + r3 + r4) / 4.;

    Color col = ComputeColor(c);
    //Java没有画单独点的函数，所以用1到1的矩形来绘制点
    SetPixelV(pDC, (int) x, (int) y, col) ;
  }
}

//这是一个“助手”函数，用于在递归函数之前创造一个初始网格
void DrawCloud(IDC* pDC)
{
  double r1, r2, r3, r4;

  //分配初始网格4个角点的随机颜色值
  //这将结束存在于applet中的4个角点的颜色
  r1 = (double) random();
  r2 = (double) random();
  r3 = (double) random();
  r4 = (double) random();

  Midpoint(pDC, 0, 0, pDC->w , pDC->h , r1, r2, r3, r4);
}
