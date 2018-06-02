
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "macro.h"
#include "fractal.h"


int InitDC(IDC* pDC, int height, int width, unsigned char* data, int step, int cn,
    color_t pen_color, int thickness)
{
  if (pDC) {
    MEMSET(pDC, 0, 0);
    pDC->h = height;
    pDC->w = width;
    pDC->data = data;
    pDC->s = step;
    pDC->c = cn;
    pDC->pen_color = pen_color;
    pDC->thickness = thickness;
    pDC->line_type = CC_AA;
    pDC->bItalic = 0; // 协体
    pDC->bBold = 0; // 粗体
    pDC->strFontName = "宋体"; // 字体
    pDC->font_cx = 16;
    pDC->font_cy = 32;
  }

  return 0;
}

//一维随机元胞自动机（分形频道：fractal.cn）2004
void DoDrawCa(void* ssf, int dst_height, int dst_width, unsigned char* dst, int dst_step, int cn)
{
  char* status = MALLOC(char, dst_width + 1);
  char* status2 = MALLOC(char, dst_width + 1);

  int i, j;

  for (i = 0; i < dst_width; i++) {
    status[ i ] = 0;
    status2[ i ] = 0;
  }

  status++;
  status2++;

  for (j = 0; j < dst_width; j++) {
    int c = rand() % 2;

    if (c == 1) {
      unsigned char* ptr = dst + j * cn;
      status[ j ] = 1;
      PUTPIX3(ptr, 0xff, 0, 0xff);
    }
  }

  for (i = 1; i < dst_height ; i++) {
    for (j = 1; j < dst_width; j++) {
      if (status[ j - 1 ] == 0 && status[ j ] == 0 && status[ j + 1 ] == 1) {
        status2[ j ] = 1;
      }
      else if (status[ j - 1 ] == 1 && status[ j ] == 0 && status[ j + 1 ] == 0) {
        status2[ j ] = 1;
      }
      else {
        status2[ j ] = 0;
      }

      if (status2[ j ]) {
        unsigned char* ptr = dst + i * dst_step + j * cn;
        PUTPIX3(ptr, 0, 0, 0);
        //pDC->SetPixel(j, i+2, RGB(0, 0, 0));
      }

    }

    memcpy(status - 1, status2 - 1, dst_width + 1);
  }

  status--;
  status2--;

  SAFEFREE(status);
  SAFEFREE(status2);
}

void Canto(IDC* pDC, int ax, int ay, int bx, int by)
{
  int c = 10;

  if ((bx - ax) < c) {
    MoveTo(pDC, ax, ay);
    LineTo(pDC, bx, by);
  }
  else {
    int cx, cy, dx, dy;
    MoveTo(pDC, ax, ay);
    LineTo(pDC, bx, by);
    cx = ax + (bx - ax) / 3;
    cy = ay + 50;
    dx = bx - (bx - ax) / 3;
    dy = by + 50;
    ay = ay + 50;
    by = by + 50;

    Canto(pDC, ax, ay, cx, cy);
    Canto(pDC, dx, dy, bx, by);
  }
}

void KochFractal(IDC* pDC, double ax, double ay, double bx, double by)
{
  int c = 100;

  if (((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) < c) {
    MoveTo(pDC, (int) ax, (int) ay);
    LineTo(pDC, (int) bx, (int) by);
  }
  else {
    double cx, cy, dx, dy, ex, ey, l, alfa;
    cx = ax + (bx - ax) / 3;
    cy = ay + (by - ay) / 3;
    ex = bx - (bx - ax) / 3;
    ey = by - (by - ay) / 3;
    KochFractal(pDC, ax, ay, cx, cy);
    KochFractal(pDC, ex, ey, bx, by);
    l = sqrt((ex - cx) * (ex - cx) + (ey - cy) * (ey - cy));
    alfa = atan((ey - cy) / (ex - cx));

    if ((alfa >= 0 && (ex - cx) < 0) || (alfa < 0 && (ex - cx) < 0)) {
      alfa = alfa + PI;
    }

    dx = cx + cos(alfa + PI / 3) * l;
    dy = cy + sin(alfa + PI / 3) * l;
    KochFractal(pDC, cx, cy, dx, dy);
    KochFractal(pDC, dx, dy, ex, ey);
  }
}

#include <math.h>

//一个分形图像生成的程序，用IFS系统和L系统产生各种分形图形。
// a fractal image generation process, using IFS system and L system to produce various fractal graphics
// FractalView.cpp : implementation of the CFractalView struct
//

#define RAND_MAX 0x7fff
//#define RGB(R, G, B) {R, G, B, 0}

color_t col[ 15 ] = {
  RGBDEF(248, 118, 112), RGBDEF(248, 169, 112), RGBDEF(248, 214, 112),
  RGBDEF(237, 248, 112), RGBDEF(183, 248, 112), RGBDEF(132, 248, 112),
  RGBDEF(112, 248, 152), RGBDEF(111, 249, 204), RGBDEF(112, 248, 244),
  RGBDEF(111, 204, 249), RGBDEF(109, 158, 250), RGBDEF(116, 108, 251),
  RGBDEF(162, 108, 251), RGBDEF(240, 108, 251), RGBDEF(250, 108, 176)
};

void sqrz(double a, double b, double* x, double* y, double* theta)
{
  if (a > 0.0) {
    * theta = atan(b / a);
  }
  else if (a < 0.0) {
    * theta = atan(b / a) + 3.141593;
  }
  else if (b >= 0.0) {
    * theta = 1.57079;
  }
  else {
    *theta = -1.57079;
  }

  *x = sqrt(sqrt(a * a + b * b)) * cos((*theta) / 2);
  *y = sqrt(sqrt(a * a + b * b)) * sin((*theta) / 2);

}

// "von Koch 曲线";
void Koch(IDC* pDC, int m_Itn)
{
  //定义变量
  int Itn, i, j, k;
  double Angle = 0, Len = 200, X = 80, Y = 200;
  char P[ 50000 ] = "CD", C[] = "CACBBCAC";
  int Prelen, Curlen, Clen = strlen(C);
  //定义获取迭代次数的对话框
  ASSERT(m_Itn > 0 && m_Itn < 8);

  if (m_Itn > 0 && m_Itn < 8) {
    Itn = m_Itn;

    for (i = 0; i < Itn; i++) {
      Len = Len / 3;
      j = 0;

      while (P[ j ] != 'D') {
        if (P[ j ] == 'C') {
          Prelen = strlen(P);
          Curlen = Prelen + Clen - 1;

          for (k = Curlen - 1; k > j + Clen - 1; k--) {
            P[ k ] = P[ k - Clen + 1 ];
          }

          for (k = j; k < j + Clen; k++) {
            P[ k ] = C[ k - j ];
          }

          j = j + Clen - 1;
        }

        j++;
      }
    }

    //根据生成的字符串进行画图
    for (k = 0; k < (int) strlen(P) - 1; k++) {
      //A代表沿逆时针方向旋转PI/3
      if (P[ k ] == 'A') {
        Angle = Angle + PI / 3;
      }
      //A代表沿顺时针方向旋转PI/3
      else if (P[ k ] == 'B') {
        Angle = Angle - PI / 3;
      }
      //C从当前点沿当前方向画一长度为Len的线段
      else if (P[ k ] == 'C') {
        X = X + Len * cos(Angle);
        Y = Y + Len * sin(Angle);
        MoveTo(pDC, (int)(X - Len * cos(Angle)), 450 - (int)(Y - Len * sin(Angle)));
        LineTo(pDC, (int) X, 450 - (int) Y);
        MoveTo(pDC, (int)(X), (int) Y);
      }
    }
  }
}

//"分形树";
void FractalTree1(IDC* pDC)
{
  //定义变量
  int Itn = 5, i, j, k, StackPointer = 0;
  int Prelen, EGHIJlen, Curlen;
  double Angle = PI / 2, Len = 1200, X = 400, Y = 0;
  double CurX[ 500 ], CurY[ 500 ], CurAngle[ 500 ];
  //定义每一步线段长度比例因子
  double L[] = {0.333333, 0.25, 0.9, 0.9, 0.8, 0.65, 0.68, 0.73, 0.77, 0.77, 0.8};
  //定义存放“图形数据”及各个复合“操作”的数组
  char P[ 50000 ] = "JIFFFZ", E[] = "EI", G[] = "BHCAGDI", H[] = "AGCBHDI";
  char I[] = "CAFFFDCBFFFDF", J[] = "CBBBGDCAAAGDEJ";
  char* TempStr;

  //根据迭代次数生成代表图形的字符串
  for (i = 0; i < Itn; i++) {
    Len = Len * L[ i ];
    j = 0;

    while (P[ j ] != 'Z') {
      if (P[ j ] == 'E' || P[ j ] == 'G' || P[ j ] == 'H' || P[ j ] == 'I' || P[ j ] == 'J') {
        if (P[ j ] == 'E') {
          TempStr = E;
        }
        else if (P[ j ] == 'G') {
          TempStr = G;
        }
        else if (P[ j ] == 'H') {
          TempStr = H;
        }
        else if (P[ j ] == 'I') {
          TempStr = I;
        }
        else if (P[ j ] == 'J') {
          TempStr = J;
        }

        Prelen = strlen(P);
        EGHIJlen = strlen(TempStr);
        Curlen = Prelen + EGHIJlen - 1;

        for (k = Curlen - 1; k == j + EGHIJlen - 1; k--) {
          assert(k < 50000);
          P[ k ] = P[ k - EGHIJlen + 1 ];
        }

        for (k = j; k < j + EGHIJlen && k < 50000; k++) {
          //assert(k<50000);
          P[ k ] = TempStr[ k - j ];
        }

        j = j + EGHIJlen - 1;
      }

      j++;
    }
  }

  //根据生成的字符串进行画图
  for (k = 0; k < (int) strlen(P) - 1; k++) {
    //A代表沿逆时针方向旋转PI/10
    if (P[ k ] == 'A') {
      Angle = Angle + PI / 10;
    }
    //A代表沿顺时针方向旋转PI/10
    else if (P[ k ] == 'B') {
      Angle = Angle - PI / 10;
    }
    //C代表把当前状态压栈（记录当前点的坐标及角度）
    else if (P[ k ] == 'C') {
      CurX[ StackPointer ] = X;
      CurY[ StackPointer ] = Y;
      CurAngle[ StackPointer ] = Angle;
      StackPointer++;
    }
    //D代表出栈（取最近一次压入栈内的信息）
    else if (P[ k ] == 'D') {
      StackPointer--;
      X = CurX[ StackPointer ];
      Y = CurY[ StackPointer ];
      Angle = CurAngle[ StackPointer ];
    }
    //F代表从当前点沿当前方向画一长度为Len的线段
    else if (P[ k ] == 'F') {
      X = X + Len * cos(Angle);
      Y = Y + Len * sin(Angle);
      MoveTo(pDC, (int)(X - Len * cos(Angle)), 450 - (int)(Y - Len * sin(Angle)));
      LineTo(pDC, (int) X, 450 - (int) Y);
      MoveTo(pDC, (int) X, (int) Y);
    }
  }

}

//"Hillbert曲线";
void Hillbert(IDC* pDC, int m_Itn)
{
  int Itn, i, j, k, m = 0;
  int Prelen, DElen = 11, Curlen;
  double Angle = 0, Len, L = 480, X = 160, Y = 0;
  char P[ 500000 ] = "DZ", D[] = "AECBDCDBCEA", E[] = "BDCAECEACDB";
  char* TempStr;

  if (m_Itn > 0 && m_Itn < 8) {
    Itn = m_Itn;

    for (i = 0; i < Itn; i++) {
      m = 2 * m + 1;
      Len = L / m;
      j = 0;

      while (P[ j ] != 'Z') {
        if (P[ j ] == 'D' || P[ j ] == 'E') {
          if (P[ j ] == 'D') {
            TempStr = D;
          }
          else if (P[ j ] == 'E') {
            TempStr = E;
          }

          Prelen = strlen(P);
          Curlen = Prelen + DElen - 1;

          for (k = Curlen - 1; k < j + DElen - 1 && k > 0 && k < 500000; k--) {
            P[ k ] = P[ k - DElen + 1 ];
          }

          for (k = j; k < j + DElen && k > 0 && k < 500000; k++) {
            P[ k ] = TempStr[ k - j ];
          }

          j = j + DElen - 1;
        }

        j++;
      }
    }

    for (k = 0; k < (int) strlen(P) - 1; k++) {
      if (P[ k ] == 'A') {
        Angle = Angle + PI / 2;
      }
      else if (P[ k ] == 'B') {
        Angle = Angle - PI / 2;
      }
      else if (P[ k ] == 'C') {
        X = X + Len * cos(Angle);
        Y = Y + Len * sin(Angle);
        MoveTo(pDC, (int)(X - Len * cos(Angle)), 500 - (int)(Y - Len * sin(Angle)));
        LineTo(pDC, (int) X, 500 - (int) Y);
        MoveTo(pDC, (int) X, (int) Y);
      }
    }
  }

}

//"分形树";
void FractalTree2(IDC* pDC)
{
  //定义变量
  int Itn = 4, i, j, k, StackPointer = 0;
  double Angle = PI / 2, Len = 9, X = 400, Y = 0;
  double CurX[ 400 ], CurY[ 400 ], CurAngle[ 400 ];
  //定义存放“图形数据”及各个复合“操作”的数组
  char P[ 15000 ] = "FZ", F[] = "FFACAFBFBFDBCBFAFAFD";
  int Prelen, Curlen, Flen = strlen(F);

  //根据迭代次数生成代表图形的字符串
  for (i = 0; i < Itn; i++) {
    j = 0;

    while (P[ j ] != 'Z') {
      if (P[ j ] == 'F') {
        Prelen = strlen(P);
        Curlen = Prelen + Flen - 1;

        for (k = Curlen - 1; k = j + Flen - 1; k--) {
          P[ k ] = P[ k - Flen + 1 ];
        }

        for (k = j; k < j + Flen; k++) {
          P[ k ] = F[ k - j ];
        }

        j = j + Flen - 1;
      }

      j++;
    }
  }

  //根据生成的字符串进行画图
  for (k = 0; k < (int) strlen(P) - 1; k++) {
    //A代表沿逆时针方向旋转PI/10
    if (P[ k ] == 'B') {
      Angle = Angle + 22.5 * PI / 180;
    }
    //A代表沿顺时针方向旋转PI/10
    else if (P[ k ] == 'A') {
      Angle = Angle - 22.5 * PI / 180;
    }
    //C代表把当前状态压栈（记录当前点的坐标及角度）
    else if (P[ k ] == 'C') {
      CurX[ StackPointer ] = X;
      CurY[ StackPointer ] = Y;
      CurAngle[ StackPointer ] = Angle;
      StackPointer++;
    }
    //D代表出栈（取最近一次压入栈内的信息）
    else if (P[ k ] == 'D') {
      StackPointer--;
      X = CurX[ StackPointer ];
      Y = CurY[ StackPointer ];
      Angle = CurAngle[ StackPointer ];
    }
    //F代表从当前点沿当前方向画一长度为Len的线段
    else if (P[ k ] == 'F') {
      X = X + Len * cos(Angle);
      Y = Y + Len * sin(Angle);
      MoveTo(pDC, (int)(X - Len * cos(Angle)), 520 - (int)(Y - Len * sin(Angle)));
      LineTo(pDC, (int) X, 520 - (int) Y);
      MoveTo(pDC, (int) X, (int) Y);
    }
  }

}

//"Barnsley羊齿叶";
void Barnsley(IDC* pDC)
{
  int Itn = 500000, i = 0;
  double X = 0, Y = 0, TempX = 0, CurP = 0;
  double A[] = {0.0, 0.85, 0.2, -0.15};
  double B[] = {0.0, 0.04, -0.26, 0.28};
  double C[] = {0.0, -0.04, 0.23, 0.26};
  double D[] = {0.16, 0.85, 0.22, 0.24};
  double E[] = {0.0, 0.0, 0.0, 0.0};
  double F[] = {0.0, 80.0, 80.0, 20.0};
  double P[] = {0.01, 0.85, 0.07, 0.07};
  CurP = (double) rand() / (double) RAND_MAX;

  while (i < Itn) {
    TempX = X;

    if (CurP < P[ 0 ]) {
      X = A[ 0 ] * X + B[ 0 ] * Y + E[ 0 ];
      Y = C[ 0 ] * TempX + D[ 0 ] * Y + F[ 0 ];
    }
    else if (CurP > P[ 0 ] && CurP < (P[ 0 ] + P[ 1 ])) {
      X = A[ 1 ] * X + B[ 1 ] * Y + E[ 1 ];
      Y = C[ 1 ] * TempX + D[ 1 ] * Y + F[ 1 ];
    }
    else if (CurP > (P[ 0 ] + P[ 1 ]) && CurP < (P[ 0 ] + P[ 1 ] + P[ 2 ])) {
      X = A[ 2 ] * X + B[ 2 ] * Y + E[ 2 ];
      Y = C[ 2 ] * TempX + D[ 2 ] * Y + F[ 2 ];
    }
    else {
      X = A[ 3 ] * X + B[ 3 ] * Y + E[ 3 ];
      Y = C[ 3 ] * TempX + D[ 3 ] * Y + F[ 3 ];
    }

    SetPixel(pDC, 400 + (int) X, 510 - (int) Y, 80, 180, 100);
    CurP = (double) rand() / (double) RAND_MAX;
    i++;
  }
}

void SierpinskiPad2(IDC* pDC)
{

  //"Sierpinski垫";

  int Itn = 500000, i = 0;
  double X = 0, Y = 0, TempX = 0, CurP = 0;
  double A[] = {0.5, 0.5, 0.5};
  double B[] = {0, 0, 0};
  double C[] = {0, 0, 0};
  double D[] = {0.5, 0.5, 0.5};
  double E[] = {0, 125, 250};
  double F[] = {0, 216.5, 0};
  double P[] = {0.333, 0.333, 0.333};
  CurP = (double) rand() / (double) RAND_MAX;

  while (i < Itn) {
    TempX = X;

    if (CurP < P[ 0 ]) {
      X = A[ 0 ] * X + B[ 0 ] * Y + E[ 0 ];
      Y = C[ 0 ] * TempX + D[ 0 ] * Y + F[ 0 ];
    }
    else if (CurP > P[ 0 ] && CurP < (P[ 0 ] + P[ 1 ])) {
      X = A[ 1 ] * X + B[ 1 ] * Y + E[ 1 ];
      Y = C[ 1 ] * TempX + D[ 1 ] * Y + F[ 1 ];
    }
    else {
      X = A[ 2 ] * X + B[ 2 ] * Y + E[ 2 ];
      Y = C[ 2 ] * TempX + D[ 2 ] * Y + F[ 2 ];
    }

    SetPixel(pDC, 150 + (int) X, 480 - (int) Y, 255, 0, 0);
    CurP = (double) rand() / (double) RAND_MAX;
    i++;
  }

}

void FractalTree3(IDC* pDC)
{

  //"分形树";

  int Itn = 500000, i = 0;
  double X = 0, Y = 0, TempX = 0, CurP = 0;
  double A[] = {0.1, 0.42, 0.42, 0.1};
  double B[] = {0.0, -0.42, 0.42, 0.0};
  double C[] = {0.0, 0.42, -0.42, 0.0};
  double D[] = {0.5, 0.42, 0.42, 0.1};
  double E[] = {0.0, 0.0, 0.0, 0.0};
  double F[] = {0.0, 200.0, 200.0, 200.0};
  double P[] = {0.05, 0.4, 0.4, 0.15};
  CurP = (double) rand() / (double) RAND_MAX;

  while (i < Itn) {
    TempX = X;

    if (CurP < P[ 0 ]) {
      X = A[ 0 ] * X + B[ 0 ] * Y + E[ 0 ];
      Y = C[ 0 ] * TempX + D[ 0 ] * Y + F[ 0 ];
    }
    else if (CurP > P[ 0 ] && CurP < (P[ 0 ] + P[ 1 ])) {
      X = A[ 1 ] * X + B[ 1 ] * Y + E[ 1 ];
      Y = C[ 1 ] * TempX + D[ 1 ] * Y + F[ 1 ];
    }
    else if (CurP > (P[ 0 ] + P[ 1 ]) && CurP < (P[ 0 ] + P[ 1 ] + P[ 2 ])) {
      X = A[ 2 ] * X + B[ 2 ] * Y + E[ 2 ];
      Y = C[ 2 ] * TempX + D[ 2 ] * Y + F[ 2 ];
    }
    else {
      X = A[ 3 ] * X + B[ 3 ] * Y + E[ 3 ];
      Y = C[ 3 ] * TempX + D[ 3 ] * Y + F[ 3 ];
    }

    SetPixel(pDC, 400 + (int) X, 510 - (int) Y, 80, 180, 100);
    CurP = (double) rand() / (double) RAND_MAX;
    i++;
  }

}

void Levy(IDC* pDC)
{

  //"Levy曲线";

  int Itn = 500000, i = 0;
  double X = 0, Y = 0, TempX = 0, CurP = 0;
  double A[] = {0.5, 0.5};
  double B[] = { -0.5, 0.5};
  double C[] = {0.5, -0.5};
  double D[] = {0.5, 0.5};
  double E[] = {0, 150};
  double F[] = {0, 150};
  double P[] = {0.5, 0.5};
  CurP = (double) rand() / (double) RAND_MAX;

  while (i < Itn) {
    TempX = X;

    if (CurP < P[ 0 ]) {
      X = A[ 0 ] * X + B[ 0 ] * Y + E[ 0 ];
      Y = C[ 0 ] * TempX + D[ 0 ] * Y + F[ 0 ];
    }
    else {
      X = A[ 1 ] * X + B[ 1 ] * Y + E[ 1 ];
      Y = C[ 1 ] * TempX + D[ 1 ] * Y + F[ 1 ];
    }

    SetPixel(pDC, 250 + (int) X, 380 - (int) Y, 0, 0, 200);
    CurP = (double) rand() / (double) RAND_MAX;
    i++;
  }

}

void Flamboyent(IDC* pDC)
{

  //"Flamboyent皇冠";

  int Itn = 500000, i = 0;
  double X = 0, Y = 0, TempX = 0, CurP = 0;
  double A[] = {0.25, 0.5, -0.25, 0.5, 0.5};
  double B[] = {0, 0, 0, 0, 0};
  double C[] = {0, 0, 0, 0, 0};
  double D[] = {0.5, 0.5, -0.25, 0.5, -0.25};
  double E[] = {0.0, -75, 75, 0.0, 150};
  double F[] = {0.0, 150.0, 300.0, 225.0, 375};
  double P[] = {0.154, 0.307, 0.078, 0.307, 0.154};
  CurP = (double) rand() / (double) RAND_MAX;

  while (i < Itn) {
    TempX = X;

    if (CurP < P[ 0 ]) {
      X = A[ 0 ] * X + B[ 0 ] * Y + E[ 0 ];
      Y = C[ 0 ] * TempX + D[ 0 ] * Y + F[ 0 ];
    }
    else if (CurP > P[ 0 ] && CurP < (P[ 0 ] + P[ 1 ])) {
      X = A[ 1 ] * X + B[ 1 ] * Y + E[ 1 ];
      Y = C[ 1 ] * TempX + D[ 1 ] * Y + F[ 1 ];
    }
    else if (CurP > (P[ 0 ] + P[ 1 ]) && CurP < (P[ 0 ] + P[ 1 ] + P[ 2 ])) {
      X = A[ 2 ] * X + B[ 2 ] * Y + E[ 2 ];
      Y = C[ 2 ] * TempX + D[ 2 ] * Y + F[ 2 ];
    }
    else if (CurP > (P[ 0 ] + P[ 1 ] + P[ 2 ]) && CurP < (P[ 0 ] + P[ 1 ] + P[ 2 ] + P[ 3 ])) {
      X = A[ 3 ] * X + B[ 3 ] * Y + E[ 3 ];
      Y = C[ 3 ] * TempX + D[ 3 ] * Y + F[ 3 ];
    }
    else {
      X = A[ 4 ] * X + B[ 4 ] * Y + E[ 4 ];
      Y = C[ 4 ] * TempX + D[ 4 ] * Y + F[ 4 ];
    }

    SetPixel(pDC, 400 + (int) X, 480 - (int) Y, 0, 200, 0);
    CurP = (double) rand() / (double) RAND_MAX;
    i++;
  }
}

//"Sierpinski三角形";
void SierpinskiTriangele1(IDC* pDC)
{
  //定义变量
  int i, j, k, n, S[ 100 ][ 100 ], T[ 100 ][ 100 ], X[ 12 ], Y[ 12 ];
  //定义所用的IFS的映射变换参数
  double A[] = {0.5, 0.5, 0.5};
  double B[] = {0, 0, 0};
  double C[] = {0, 0, 0};
  double D[] = {0.5, 0.5, 0.5};
  double E[] = {1, 50, 1};
  double F[] = {1, 1, 50};

  //初始化
  for (i = 0; i < 6; i++) {
    X[ i ] = i * 105;
    X[ i + 6 ] = X[ i ];
    Y[ i ] = 0;
    Y[ i + 6 ] = 115;
  }

  for (i = 0; i < 100; i++) {
    T[ 0 ][ i ] = 1;
    T[ i ][ 0 ] = 1;
    T[ 99 ][ i ] = 1;
    T[ i ][ 99 ] = 1;
  }

  //画图，n为迭代次数
  Rectangle(pDC, 80, 260, 180, 360);

  for (n = 1; n < 12; n++) {
    for (i = 0; i < 100; i++) {
      for (j = 0; j < 100; j++) {
        if (T[ i ][ j ] == 1) {
          for (k = 0; k < 3; k++) {
            S[(int)(A[ k ] * i + B[ k ] * j + E[ k ]) ][(int)(C[ k ] * j + D[ k ] * j + F[ k ]) ] = 1;
          }
        }
      }
    }

    for (i = 0; i < 100; i++) {
      for (j = 0; j < 100; j++) {
        T[ i ][ j ] = S[ i ][ j ];
        S[ i ][ j ] = 0;

        if (T[ i ][ j ] == 1) {
          SetPixel(pDC, 80 + i + X[ n ], 360 - (j + Y[ n ]), 255, 0, 0);
        }
      }
    }
  }

}

//"Sierpinski三角形(改进)";
void SierpinskiTriangele2(IDC* pDC)
{
  //定义变量
  int i, j, k, l, m, n, Adr, Adr1, j0, Temp = 1;
  int S[ 1800 ], T[ 1800 ], X[ 10 ], Y[ 10 ], I[ 3 ], J[ 3 ];
  //定义所用的IFS的映射变换参数
  double A[] = {0.5, 0.5, 0.5};
  double B[] = {0, 0, 0};
  double C[] = {0, 0, 0};
  double D[] = {0.5, 0.5, 0.5};
  double E[] = {0, 60, 0};
  double F[] = {0, 0, 60};

  //初始化
  for (i = 0; i < 1800; i++) {
    S[ i ] = 0;
    T[ i ] = 0;
  }

  //设置初始集正方形B0的边界
  for (i = 0; i < 15; i++) {
    S[ i ] = 255;
    S[ i + 1785 ] = 255;
  }

  i = 15;

  while (i < 1785) {
    S[ i ] = 128;
    S[ i + 14 ] = 1;
    i = i + 15;
  }

  //确定每次图形的位置
  for (i = 0; i < 5; i++) {
    X[ i ] = i * 125;
    X[ i + 5 ] = X[ i ];
    Y[ i ] = 0;
    Y[ i + 5 ] = 130;
  }

  //画图，n为迭代次数
  Rectangle(pDC, 80, 240, 200, 360);

  for (n = 1; n < 10; n++) {
    //Adr指示数据在S[1800]中的存放位置
    for (Adr = 0; Adr < 1800; Adr++) {
      if (S[ Adr ] != 0) {
        //I是图像的行数，i从0到199
        i = (int)(Adr / 15.0);
        j0 = (Adr % 15) * 8;

        for (k = 0; k < 8; k++) {
          Temp = 1;

          for (m = 0; m < 7 - k; m++) {
            Temp = 2 * Temp;
          }

          if ((S[ Adr ] & Temp) != 0) {
            //j是图像的列数，j从0到199，(i，j)确定图像的位置
            j = j0 + k;

            //I[l],J[l]存放迭代后点的位置
            for (l = 0; l < 3; l++) {
              I[ l ] = (int)(A[ l ] * i + B[ l ] * j + E[ l ]);
              J[ l ] = (int)(C[ l ] * i + D[ l ] * j + F[ l ]);
              SetPixel(pDC, 80 + I[ l ] + X[ n ], 360 - (J[ l ] + Y[ n ]), 255, 0, 0);
              // adr1指示本次迭代后的结果在T〔1800]中的存放位置
              Adr1 = I[ l ] * 15 + (int)(J[ l ] / 8);
              //计算并存放本次迭代后的结果，采用或运算以便不破坏其他位的值
              Temp = 1;

              for (m = 0; m < (7 - (J[ l ] % 8)); m++) {
                Temp = Temp * 2;
              }

              T[ Adr1 ] = T[ Adr1 ] | Temp;
            }
          }
        }
      }
    }

    for (k = 0; k < 1800; k++) {
      S[ k ] = T[ k ];
      T[ k ] = 0;
    }
  }

}

//"树";
void FractalTree4(IDC* pDC)
{
  int Itn = 500000, i = 0;
  double X = 0, Y = 0, TempX = 0, CurP = 0;
  double A[] = {0.05, 0.05, 0.45, 0.46, 0.433, 0.421};
  double B[] = {0, 0, -0.321, -0.154, 0.275, 0.257};
  double C[] = {0, 0, 0.3856, 0.171, -0.25, -0.353};
  double D[] = {0.6, -0.5, 0.383, 0.423, 0.476, 0.3064};
  double E[] = {0, 0, 0, 0, 0, 0};
  double F[] = {0, 200, 120, 220, 200, 140};
  double P[] = {0.1, 0.1, 0.2, 0.2, 0.2, 0.2};
  CurP = (double) rand() / (double) RAND_MAX;

  while (i < Itn) {
    TempX = X;

    if (CurP < P[ 0 ]) {
      X = A[ 0 ] * X + B[ 0 ] * Y + E[ 0 ];
      Y = C[ 0 ] * TempX + D[ 0 ] * Y + F[ 0 ];
    }
    else if (CurP > P[ 0 ] && CurP < (P[ 0 ] + P[ 1 ])) {
      X = A[ 1 ] * X + B[ 1 ] * Y + E[ 1 ];
      Y = C[ 1 ] * TempX + D[ 1 ] * Y + F[ 1 ];
    }
    else if (CurP > (P[ 0 ] + P[ 1 ]) && CurP < (P[ 0 ] + P[ 1 ] + P[ 2 ])) {
      X = A[ 2 ] * X + B[ 2 ] * Y + E[ 2 ];
      Y = C[ 2 ] * TempX + D[ 2 ] * Y + F[ 2 ];
    }
    else if (CurP > (P[ 0 ] + P[ 1 ] + P[ 2 ]) && CurP < (P[ 0 ] + P[ 1 ] + P[ 2 ] + P[ 3 ])) {
      X = A[ 3 ] * X + B[ 3 ] * Y + E[ 3 ];
      Y = C[ 3 ] * TempX + D[ 3 ] * Y + F[ 3 ];
    }
    else if (CurP > (P[ 0 ] + P[ 1 ] + P[ 2 ] + P[ 3 ]) && CurP < (P[ 0 ] + P[ 1 ] + P[ 2 ] + P[ 3 ] + P[ 4 ])) {
      X = A[ 4 ] * X + B[ 4 ] * Y + E[ 4 ];
      Y = C[ 4 ] * TempX + D[ 4 ] * Y + F[ 4 ];

    }
    else {
      X = A[ 5 ] * X + B[ 5 ] * Y + E[ 5 ];
      Y = C[ 5 ] * TempX + D[ 5 ] * Y + F[ 5 ];
    }

    SetPixel(pDC, 400 + (int) X, 460 - (int) Y, 255, 0, 0);
    CurP = (double) rand() / (double) RAND_MAX;
    i++;
  }

}

//"树枝";
void Branch(IDC* pDC)
{
  int Itn = 5, i, j, k, StackPointer = 0;
  double Angle = PI / 2, Len = 90, X = 400, Y = 0;
  double CurX[ 500 ], CurY[ 500 ], CurAngle[ 500 ];
  double L[] = {0.333333, 0.25, 0.9, 0.9, 0.8};
  char P[ 500000 ] = "FZ";
  char F[] = "FCAFDFCBFDF";
  int Prelen, Flen = strlen(F), Curlen;

  for (i = 0; i < Itn; i++) {
    Len = Len * L[ i ];
    j = 0;

    while (P[ j ] != 'Z') {
      if (P[ j ] = 'F') {
        Prelen = strlen(P);
        Curlen = Prelen + Flen - 1;

        for (k = Curlen - 1; k > j + Flen - 1; k--) {
          P[ k ] = P[ k - Flen + 1 ];
        }

        for (k = j; k < j + Flen; k++) {
          P[ k ] = F[ k - j ];
        }

        j = j + Flen - 1;
      }

      j++;
    }
  }

  //根据生成的字符串进行画图
  for (k = 0; k < (int) strlen(P) - 1; k++) {
    //A代表沿逆时针方向旋转PI/7
    if (P[ k ] == 'A') {
      Angle = Angle + PI / 6;
    }
    //A代表沿顺时针方向旋转PI/7
    else if (P[ k ] == 'B') {
      Angle = Angle - PI / 6;
    }
    //C代表把当前状态压栈（记录当前点的坐标及角度）
    else if (P[ k ] == 'C') {
      CurX[ StackPointer ] = X;
      CurY[ StackPointer ] = Y;
      CurAngle[ StackPointer ] = Angle;
      StackPointer++;
    }
    //D代表出栈（取最近一次压入栈内的信息）
    else if (P[ k ] == 'D') {
      StackPointer--;
      X = CurX[ StackPointer ];
      Y = CurY[ StackPointer ];
      Angle = CurAngle[ StackPointer ];
    }
    //F代表从当前点沿当前方向画一长度为Len的线段
    else if (P[ k ] == 'F') {
      X = X + Len * cos(Angle);
      Y = Y + Len * sin(Angle);
      MoveTo(pDC, (int)(X - Len * cos(Angle)), 520 - (int)(Y - Len * sin(Angle)));
      LineTo(pDC, (int) X, 520 - (int) Y);
      MoveTo(pDC, (int) X, (int) Y);
    }
  }
}

//"Mandelbrot集";
void Mandelbrot(IDC* pDC)
{
  //定义变量
  int i, j, k;
  double X, Y, X1, Y1, U, V;

  //画图
  for (i = 40; i <= 540; i++) {
    for (j = -249; j <= 250; j++) {
      U = i / 200.0 - 0.8;
      V = j / 200.0;
      X = 0.0;
      Y = 0.0;

      for (k = 0; k < 30; k++) {
        X1 = X * X - Y * Y - U;
        Y1 = 2 * X * Y - V;

        if ((X1 * X1 + Y1 * Y1) > 9.0) {
          break;
        }

        X = X1;
        Y = Y1;
      }

      if (k == 30) {
        SetPixel(pDC, 100 + i, 260 - j, 255, 0, 0);
        SetPixel(pDC, 100 + i, 260 + j, 255, 0, 0);
      }
      else {
        SetPixelV(pDC, 100 + i, 260 - j, col[ k % 15 ]);
        SetPixelV(pDC, 100 + i, 260 + j, col[ k % 15 ]);
      }
    }
  }
}

//"牛顿函数的Julia集";
void JuliaofNewton(IDC* pDC)
{
  //定义变量
  int i, j, k, Itn = 40;
  double X, Y, FM, NX, NY, Epthl = 0.001;

  //画图
  for (i = -200; i <= 200; i++) {
    for (j = -200; j <= 200; j++) {
      X = i / 100.0;
      Y = j / 100.0;

      for (k = 0; k < Itn; k++) {
        if (i == 0 && j == 0) {
          break;
        }

        if ((((X - 1) * (X - 1) + Y * Y)) < Epthl) {
          break;
        }

        FM = 3.0 * (X * X + Y * Y) * (X * X + Y * Y);
        NX = 2 * X / 3.0 + (X * X - Y * Y) / FM;
        NY = 2 * Y / 3.0 - 2 * X * Y / FM;
        X = NX;
        Y = NY;
      }

      if (k < Itn) {
        SetPixel(pDC, 400 + i, 250 - j, 0, 0, 200);
        SetPixel(pDC, 400 + i, 250 + j, 0, 0, 200);
      }
    }
  }
}

//"Julia集";
void Julia1(IDC* pDC, double m_CX, double m_CY)
{
  //定义变量
  int i, j, k, sx = 400, sy = 260;
  double X, Y, X1, Y1, A, B;

  //复数lmd
  if ((m_CX * m_CX + m_CY * m_CY) <= 4) {
    //获取复数lmd的实部A和虚部B
    A = m_CX;
    B = m_CY;

    //画图
    for (i = -339; i <= 340; i++) {
      for (j = -249; j <= 250; j++) {
        X = i / 160.0;
        Y = j / 160.0;

        for (k = 0; k < 30; k++) {
          X1 = X * X - Y * Y - A;
          Y1 = 2 * X * Y - B;

          if ((X1 * X1 + Y1 * Y1) > 9.0) {
            break;
          }

          X = X1;
          Y = Y1;
        }

        if (k == 30) {
          SetPixel(pDC, sx + i, sy - j, 255, 0, 0);
        }
        else {
          SetPixelV(pDC, sx + i, sy - j, col[ k % 15 ]);
        }
      }
    }
  }

}

//"Sierpinski三角形";
void SierpinskiTriangele3(IDC* pDC, int m_Itn)
{
  //定义变量
  int Itn, M = 400, p, q, k;
  double A = 0.0, B = 0.0, C = 1.0, D = 1.0, X = 0.0, Y = 0.0, X0 = 0.0, Y0 = 0.0, R = 200.0;
  //定义获取迭代次数的对话框

  if (m_Itn > 0) {
    Itn = m_Itn;

    for (p = 0; p < M; p++) {
      X0 = A + (C - A) * p / (double) M;

      for (q = 0; q < M; q++) {
        Y0 = B + (D - B) * q / (double) M;
        X = X0;
        Y = Y0;

        for (k = 0; k < Itn; k++) {
          if (Y > 0.5) {
            X = 2 * X;
            Y = 2 * Y - 1;
          }
          else if (X >= 0.5) {
            X = 2 * X - 1;
            Y = 2 * Y;
          }
          else {
            X = 2 * X;
            Y = 2 * Y;
          }

          if (X * X + Y * Y > R) {
            break;
          }
        }

        if (k == Itn) {
          SetPixel(pDC, 200 + p, 450 - q, 0, 0, 255);
        }
      }
    }
  }
}

#include "fractal_Branch.inl"
#include "fractal_Cloud.inl"
#include "fractal_Fire.inl"
#include "fractal_IFS_system.inl"
#include "fractal_Julia.inl"
#include "fractal_King.inl"
#include "fractal_King2.inl"
#include "fractal_L_system.inl"
#include "fractal_mand.inl"
#include "fractal_Mira.inl"
#include "fractal_moun.inl"
#include "fractal_Newton.inl"
#include "fractal_Newton2.inl"
#include "fractal_Plasma.inl"
#include "fractal_Pool.inl"
#include "fractal_ripple.inl"
#include "fractal_sierpinski.inl"
#include "fractal_SpotFormat.inl"
#include "fractal_Water.inl"
#include "fractal_ColorMover.inl"
