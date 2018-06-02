
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "macro.h"
#include "fractal.h"

// IFS系统
void IFS_system(IDC* pDC)
{
  double x = 0;  // '仿射变换中的自变量
  double y = 0;
  double newx, newy; //'仿射变换产生的新点
  double a, b, c, d, e, f; //'仿射变幻中的系数
  long n = 100000;    // '迭代次数
  double R;   //'随机变量
  double m[ 7 ][ 7 ] = {0};    // '存放IFS码

  //'IFS码赋值
  //Sierpinski垫片
  m[ 0 ][ 0 ] = 0.5;
  m[ 0 ][ 1 ] = 0;
  m[ 0 ][ 2 ] = 0;
  m[ 0 ][ 3 ] = 0.5;
  m[ 0 ][ 4 ] = 0;
  m[ 0 ][ 5 ] = 0;
  m[ 0 ][ 6 ] = 0.333;
  m[ 1 ][ 0 ] = 0.5;
  m[ 1 ][ 1 ] = 0;
  m[ 1 ][ 2 ] = 0;
  m[ 1 ][ 3 ] = 0.5;
  m[ 1 ][ 4 ] = 0.5;
  m[ 1 ][ 5 ] = 0;
  m[ 1 ][ 6 ] = 0.333;
  m[ 2 ][ 0 ] = 0.5;
  m[ 2 ][ 1 ] = 0;
  m[ 2 ][ 2 ] = 0;
  m[ 2 ][ 3 ] = 0.5;
  m[ 2 ][ 4 ] = 0.25;
  m[ 2 ][ 5 ] = 0.5;
  m[ 2 ][ 6 ] = 0.334;

  //枫叶
  /* m[0][0] = 0.25; m[0][1] = 0; m[0][2] = -1.0;  m[0][3] = 0.25;  m[0][4] = 0;    m[0][5] = 0;   m[0][6] = 0.25;
   m[1][0] = 0.2;  m[1][1] = 0; m[1][2] = 0;  m[1][3] = 0.2;   m[1][4] = 0;    m[1][5] = 0.5; m[1][6] = 0.25;
   m[2][0] = 0.25; m[2][1] = 0; m[2][2] = 0;  m[2][3] = 0.25;  m[2][4] = 0.25; m[2][5] = 0;   m[2][6] = 0.25;
   m[3][0] = 0.50; m[3][1] = 0; m[3][2] =1.0;  m[3][3] = 0.50;  m[3][4] = 0;    m[3][5] = 0;   m[3][6] = 0.25;
  */
  //'循环迭代，在不同的概率空间下，赋不同的IFS码值
  while (n > 0) {
    R = (double) rand() / RAND_MAX;

    if (R <= m[ 0 ][ 6 ]) {
      a = m[ 0 ][ 0 ];
      b = m[ 0 ][ 1 ];
      c = m[ 0 ][ 2 ];
      d = m[ 0 ][ 3 ];
      e = m[ 0 ][ 4 ];
      f = m[ 0 ][ 5 ];
    }
    else if (R <= m[ 0 ][ 6 ] + m[ 1 ][ 6 ]) {
      a = m[ 1 ][ 0 ];
      b = m[ 1 ][ 1 ];
      c = m[ 1 ][ 2 ];
      d = m[ 1 ][ 3 ];
      e = m[ 1 ][ 4 ];
      f = m[ 1 ][ 5 ];
    }
    else if (R <= m[ 0 ][ 6 ] + m[ 1 ][ 6 ] + m[ 2 ][ 6 ]) {
      a = m[ 2 ][ 0 ];
      b = m[ 2 ][ 1 ];
      c = m[ 2 ][ 2 ];
      d = m[ 2 ][ 3 ];
      e = m[ 2 ][ 4 ];
      f = m[ 2 ][ 5 ];
    }
    else { // if(R <= m[0][6] + m[1][6] + m[2][6])//+m[3][6])
      a = m[ 3 ][ 0 ];
      b = m[ 3 ][ 1 ];
      c = m[ 3 ][ 2 ];
      d = m[ 3 ][ 3 ];
      e = m[ 3 ][ 4 ];
      f = m[ 3 ][ 5 ];
    }

    // else
    // {
    //   a = m[4][0]; b = m[4][1]; c = m[4][2]; d = m[4][3]; e = m[4][4]; f = m[4][5];
    //  }
    newx = (a * x) + (b * y) + e;
    newy = (c * x) + (d * y) + f;
    x = newx;
    y = newy;
    SetPixel(pDC, (1000 + 7000 * x) / 15, (6500 - 6000 * y) / 15, (unsigned char)(x * 500 * R), (unsigned char)(R * 100), (unsigned char)(y * 2000 * R));
    //pDC->SetPixelV((1000 + 7000 * x)/15, (6500 - 6000 * y)/15, RGB(0,255,0));
    n--;
  }
}

typedef struct Julia_IFS {
  int h;
  int w;
  int depth;
  double t;
  double b;
  double l;
  double r;
  // BOOL drawing;
  double c_x;
  double c_y;
  BYTE* map;
  color_t color;
  int times;
}
Julia_IFS;

#define transX(x) (int)((double)(x - ifs->l) / (ifs->r - ifs->l) * ifs->w)
#define transY(y) (int)((double)(y - ifs->t) / (ifs->b - ifs->t) * ifs->h)

void CJulia_IFSViewplot(IDC* pDC, Julia_IFS* ifs, double x, double y)
{
  double r, newx, newy;
  int sgn;
  int xtmp = transX(x);
  int ytmp = transY(y);

  ifs->map[ xtmp + ytmp * ifs->w ] ++;
  SetPixelV(pDC, xtmp, ytmp, pDC->pen_color);

  //color += 0x00ffff;
  if (ifs->map[ xtmp + ytmp * ifs->w ] < ifs->depth && ifs->times++ < 10000) {
    x -= ifs->c_x;
    y -= ifs->c_y;
    r = sqrt(x * x + y * y);
    sgn = (y > 0) ? 1 : (y < 0) ? -1 : 0;
    newx = sqrt((r + x) / 2);
    newy = sqrt((r - x) / 2) * sgn;

    CJulia_IFSViewplot(pDC, ifs, newx, newy);
    CJulia_IFSViewplot(pDC, ifs, -newx, -newy);
  }
}

// 基于IFS的Julia集
void Julia_IFSViewDraw(IDC* pDC)
{
  Julia_IFS fs;
  Julia_IFS* ifs;
  ifs = &fs;
  ifs->h = pDC->h;
  ifs->w = pDC->w;
  ifs->times = 0;
  ifs->depth = 4;
  ifs->t = 1.5;
  ifs->b = -1.5;
  ifs->l = -2.0;
  ifs->r = 2.0;
  ifs->c_x = -0.74543;
  ifs->c_y = 0.11301;
  ifs->map = MALLOC(BYTE, ifs->h * ifs->w);
  memset(ifs->map, 0, ifs->h * ifs->w);

  CJulia_IFSViewplot(pDC, ifs, 1.0, 1.0);
  SAFEFREE(ifs->map);
}

void CJulia_IFSViewSetPt(Julia_IFS* ifs, int x, int y)
{
  ifs->c_x = (double) x / ifs->w * (ifs->r - ifs->l) + ifs->l;
  ifs->c_y = (double) y / ifs->h * (ifs->b - ifs->t) + ifs->t;
}
