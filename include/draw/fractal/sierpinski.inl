
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "macro.h"
#include "fractal.h"

//serpirski曲线迭代
void serpirski(IDC* pDC, double a, double b, double c, double d, int n, int m, int r)
{
  static int p , q ;
  double x , y , x0 , y0 ;
  int k;

  for (p = 0 ; p < m - 1 ; p++) {
    x0 = a + (c - a) * p / m ;

    for (q = 0 ; q < m - 1 ;) {

      y0 = b + (d - b) * q / m ;
      x = x0 ;
      y = y0 ;

      for (k = 1; k < n; k++) {
        if (y > 0.5) {
          x = 2 * x ;
          y = 2 * y - 1 ;
        }
        else if (x >= 0.5) {
          x = 2 * x - 1 ;
          y = 2 * y ;
        }
        else {
          x = 2 * x ;
          y = 2 * y ;
        }

        if (x * x + y * y > r) {
          goto np ;
        }
      }

      SetPixel(pDC, 2 * p + 50, 2 * q + 50 , 0 , 0, 250);   //修改值2，可得到不同的放大效果

np:
      q++;

    }
  }

}

void SierpinskiDraw(IDC* pDC)
{
  serpirski(pDC, 0, 0, 1, 1, 12, 200, 200);   //调用绘图函数；
}

// Julia集
void JuliaDraw(IDC* pDC)
{
  double xl, xs, yl, ys, x0, y0, xb, yb, xk, yk, r, p, q;
  int H, K, k, m, Mx, My, nx, ny;
  {
    K = 100;
    m = 500;
    Mx = 800;
    My = 600;
    xs = -1.5;
    xl = 1.5;
    ys = -1.5;
    yl = 1.5;
    p = 0.32;
    q = 0.043;
  }
  xb = (xl - xs) / Mx;
  yb = (yl - ys) / My;

  for (nx = 0; nx <= Mx; nx++) {
    for (ny = 0; ny <= My; ny++) {
      x0 = xs + nx * xb;
      y0 = ys + ny * yb;
      k = 0;
loop1:
      xk = x0 * x0 - y0 * y0 + p;
      yk = 2 * x0 * y0 + q;
      k = k + 1;
      r = xk * xk + yk * yk;
      x0 = xk, y0 = yk;

      if (r > m) {
        H = k;
        goto loop2;
      }

      if (k == K) {
        H = (int)(r * 10);
        goto loop2;
      }

      if (r <= m && k < K) {
        goto loop1;
      }

loop2:
      SetPixelI(pDC, nx, ny, (H * 1000));
    }
  }
}

// Mandlbrot
void MandlbrotDraw(IDC* pDC)
{
  double xl, xs, yl, ys, x0, y0, xb, yb, xk, yk, r, p, q;
  int H, K, k, m, Mx, My, nx, ny;
  {
    K = 100;
    m = 500;
    Mx = 800;
    My = 600;
    xs = -1.5;
    xl = 1.5;
    ys = -1.5;
    yl = 1.5;
    p = 0.32;
    q = 0.043;
  }
  xb = (xl - xs) / Mx;
  yb = (yl - ys) / My;

  for (nx = 0; nx <= Mx; nx++) {
    for (ny = 0; ny <= My; ny++) {
      x0 = xs + nx * xb;
      y0 = ys + ny * yb;
      k = 0;
loop1:
      xk = x0 * x0 - y0 * y0 + p;
      yk = 2 * x0 * y0 + q;
      k = k + 1;
      r = xk * xk + yk * yk;
      x0 = xk, y0 = yk;

      if (r > m) {
        H = k;
        goto loop2;
      }

      if (k == K) {
        H = (int)(r * 10);
        goto loop2;
      }

      if (r <= m && k < K) {
        goto loop1;
      }

loop2:
      SetPixelI(pDC, nx, ny, H * 1000);
    }

  }
}

void MandelDraw(IDC* pDC)
{
  double x = 0.0, y = 0.0, r = 0.0, dp, dq, p, q, x0, y0;
  int i, j, i1 = 0, j1 = 0, i2 = 0, j2 = 0;

  int m_times = (10);
  int k = 0;
  double m_deep = (50.0);
  double pmax = 0.8;
  double pmin = -2.2;
  double qmax = 1.2;
  double qmin = -1.2;

  dp = (pmax - pmin) / (pDC->w);
  dq = (qmax - qmin) / (pDC->h);

  for (j1 = 0, j = 0; j < pDC->h; j++, j1++) {
    for (i1 = 0, i = 0; i < pDC->w; i++, i1++) {
      x0 = y0 = 0.0;

      p = pmin + dp * i1;
      q = qmin + dq * j1;

      for (k = 0; k < m_times; k++) {
        x = x0 * x0 - y0 * y0 + p;
        y = 2 * x0 * y0 + q;
        r = x * x + y * y;
        x0 = x;
        y0 = y;

        if (r > m_deep) {
          break;
        }
      }

      if (k >= m_times) {
        k = (int)(r * 100);
      }

      k *= 0x1111ff;

      SetPixelI(pDC, i, j, k);
    }
  }
}



void JuliaDraw1(IDC* pDC)
{
  double x = 0.0, y = 0.0, r = 0.0, dp, dq, x0, y0;
  int i, j, k = 0, i1 = 0, j1 = 0, i2 = 0, j2 = 0;
  double m_deep;
  double p, q;

  int m_times = (10);
  double pmax = 1.5;
  double pmin = -1.5;
  double qmax = 1.5;
  double qmin = -1.5;

  p = 0.32;
  q = 0.043;
  m_deep = (50.0) ;
  dp = (pmax - pmin) / (pDC->w);
  dq = (qmax - qmin) / (pDC->h);

  for (j1 = 0, j = 0; j < pDC->h; j++, j1++) {

    for (i1 = 0, i = 0; i < pDC->w; i++, i1++) {
      x0 = pmin + dp * i1;
      y0 = qmin + dq * j1;

      for (k = 0; k < m_times; k++) {

        x = x0 * x0 - y0 * y0 + p;
        y = 2 * x0 * y0 + q;
        r = x * x + y * y;
        x0 = x;
        y0 = y;

        if (r > m_deep) {
          break;
        }
      }

      if (k >= m_times) {
        k = (int)(r * 100);
      }

      k *= 0x1111ff;


      SetPixelI(pDC, i, j, k);
    }
  }
}

// 基于牛顿求根法的分形
void MandelDraw1(IDC* pDC)
{
  double x = 0.0, y = 0.0, r = 0.0, dp, dq;
  int i = 0, j = 0, k = 0, i1 = 0, j1 = 0, i2 = 0, j2 = 0;
  int m_times = 10;
  double m_deep;
  double p, q;
  double tmp, xx, yy, d;
  int color, count;

  double pmax = 1.5;
  double pmin = -1.5;
  double qmax = 1.5;
  double qmin = -1.5;
  m_deep = (50.0) ;
  p = 0.32;
  q = 0.043;
  dp = (pmax - pmin) / (pDC->w - 0);
  dq = (qmax - qmin) / (pDC->h - 0);

  for (j1 = 0, j = 0; j < pDC->h; j++, j1++) {

    for (i1 = 0, i = 0; i < pDC->w; i++, i1++) {

      x = pmin + dp * i1;
      y = qmin + dq * j1;


      count = 0;

      while (count < 2048) {
        xx = x * x;
        yy = y * y;
        d = 3.0 * ((xx - yy) * (xx - yy) + 4.0 * xx * yy);

        if (d == 0.0) {
          d = 0.000001;
        }

        tmp = x;
        x = (2.0 / 3.0) * x + (xx - yy) / d;
        y = (2.0 / 3.0) * y - 2.0 * tmp * y / d;
        count += 1;
      }

      if (x > 0.0) {
        color = count % 100;
      }
      else {
        if ((x < -0.3) && (y > 0.0)) {
          color = (count % 100) + 100;
        }
        else {
          color = (count % 100) + 155;
        }
      }


      if (k >= m_times) {
        k = (int)(r * 100);
      }

      k *= 0x1111ff;


      SetPixelI(pDC, i, j, color);


    }
  }
}

//绘制M集
void MandelDraw2(IDC* pDC)
{
  double x0, y0, pk, qk, r, p0, q0, p, q;
  int H, k, np, nq;

  double pl = 0.5;
  double ps = -2.0;
  double ql = 1.3;
  double qs = -1.3;
  double xs = -1.5;
  double xl = 1.5;
  double ys = -1.5;
  double yl = 1.5;
  int K = 100;
  int m = 500;

  p = (pl - ps) / pDC->w;
  q = (ql - qs) / pDC->h;

  for (nq = 0; nq < pDC->h; nq++) {
    for (np = 0; np < pDC->w; np++) {
      x0 = ps + np * p;
      y0 = qs + nq * q;
      k = 0, p0 = q0 = 0;
loop1:
      pk = p0 * p0 - q0 * q0 + x0;
      qk = 2 * p0 * q0 + y0;
      k = k + 1;
      r = pk * pk + qk * qk;
      p0 = pk, q0 = qk;

      if (r > m) {
        H = k;
        goto loop2;
      }

      if (k == K) {
        H = (int)(r * 1);
        goto loop2;
      }

      if (r <= m && k < K) {
        goto loop1;
      }

loop2:
      SetPixelI(pDC, np, nq, H * 1000);
    }

  }
}
//绘制J集
void JuliaDraw2(IDC* pDC)
{
  double x0, y0, xb, yb, xk, yk, r;
  int H, k, nx, ny;

  double pl = 0.5;
  double ps = -2.0;
  double ql = 1.3;
  double qs = -1.3;
  double xs = -1.5;
  double xl = 1.5;
  double ys = -1.5;
  double yl = 1.5;
  int K = 100;
  int m = 500;

  double p, q;
  p = 0.32;
  q = 0.043;

  xb = (xl - xs) / pDC->w;
  yb = (yl - ys) / pDC->h;

  for (ny = 0; ny < pDC->h; ny++) {
    for (nx = 0; nx < pDC->w; nx++) {
      x0 = xs + nx * xb;
      y0 = ys + ny * yb;
      k = 0;
loop1:
      xk = x0 * x0 - y0 * y0 + p;
      yk = 2 * x0 * y0 + q;
      k = k + 1;
      r = xk * xk + yk * yk;
      x0 = xk, y0 = yk;

      if (r > m) {
        H = k;
        goto loop2;
      }

      if (k == K) {
        H = (int)(r * 10);
        goto loop2;
      }

      if (r <= m && k < K) {
        goto loop1;
      }

loop2:
      SetPixelI(pDC, nx, ny, H * 1000);
    }

  }
}

// 一维元胞自动机
void DrawCa(IDC* pDC, int is_rand)
{
  char* status = MALLOC(char, pDC->w + 1);
  char* status2 = MALLOC(char, pDC->w + 1);


  int i;
  int j;

  for (i = 0; i < pDC->w; i++) {
    status[ i ] = 0;
    status2[ i ] = 0;
  }

  status++;
  status2++;


  if (is_rand) {
    for (j = 0; j < pDC->w; j++) {
      int c = rand() % 2;

      if (c == 1) {
        status[ j ] = 1;
        SetPixel(pDC, 0, j, 0xff, 0, 0xff);
      }
    }

  }
  else {
    status[(pDC->w) / 2 ] = 1;
    SetPixel(pDC, 0, (pDC->w) / 2, 0xff, 0, 0xff);
  }

  for (i = 1; i < pDC->h ; i++) {
    for (j = 1; j < pDC->w; j++) {
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
        SetPixel(pDC, j, i + 2, 0, 255, 0);
      }
    }

    memcpy(status - 1, status2 - 1, pDC->w + 1);
  }


  status--;
  status2--;

  SAFEFREE(status);
  SAFEFREE(status2);

}

void drawLeaf(IDC* pDC, double x, double y, double L, double A, double B, double C)
{
  double s1, s2, s3;
  int x1, y1, x1L, y1L, x1R, y1R, x2, y2, x2R, y2R, x2L, y2L;

  s1 = 2;
  s2 = 3;
  s3 = 1.3;

  if (L > s1) {
    x2 = (int)(x + L * cos(A));
    y2 = (int)(y + L * sin(A));
    x2R = (int)(x2 + L / s2 * cos((A + B)));
    y2R = (int)(y2 + L / s2 * sin((A + B)));
    x2L = (int)(x2 + L / s2 * cos((A - B)));
    y2L = (int)(y2 + L / s2 * sin((A - B)));

    x1 = (int)(x + L / s2 * cos(A));
    y1 = (int)(y + L / s2 * sin(A));
    x1L = (int)(x1 + L / s2 * cos((A - B)));
    y1L = (int)(y1 + L / s2 * sin((A - B)));
    x1R = (int)(x1 + L / s2 * cos((A + B)));
    y1R = (int)(y1 + L / s2 * sin((A + B)));

    MoveTo(pDC, (int) x, (int) y);
    LineTo(pDC, x2, y2);
    LineTo(pDC, x2R, y2R);
    MoveTo(pDC, x2, y2);
    LineTo(pDC, x2L, y2L);
    MoveTo(pDC, x1, y1);
    LineTo(pDC, x1L, y1L);
    MoveTo(pDC, x1, y1);
    LineTo(pDC, x1R, y1R);

    drawLeaf(pDC, x2, y2, L / s3, A + C, B, C);
    drawLeaf(pDC, x2R, y2R, L / s2, A + B, B, C);
    drawLeaf(pDC, x2L, y2L, L / s2, A - B, B, C);
    drawLeaf(pDC, x1L, y1L, L / s2, A - B, B, C);
    drawLeaf(pDC, x1R, y1R, L / s2, A + B, B, C);
  }
}

//摇曳的分形树
void DrawLeaf1(IDC* pDC)
{
  int x, y;
  static int m_dir = 1;
  static double m_C = 9; //初始弯曲角度;
  x = pDC->w / 2;
  y = pDC->h - 50;
  //在内存画布中绘制树
  drawLeaf(pDC, x, y, pDC->h / 5, 270 * PI / 180., 50 * PI / 180., m_C * PI / 180.);

  if (m_dir) {
    m_C += 0.5;

    if (m_C > 18) {
      m_dir = 0;
    }
  }
  else {
    m_C -= 0.5;

    if (m_C < -18) {
      m_dir = 1;
    }
  }
}


// 生长出来的Sierpinski三角形
void DrawIFS(IDC* pDC)
{
  double x = 0;  // 仿射变换中的自变量
  double y = 0;
  double newx, newy; //仿射变换产生的新点
  double a, b, c, d, e, f; //仿射变幻中的系数
  long n = 10000;    // 迭代次数
  double R;   //随机变量
  double m[ 7 ][ 7 ] = {0};    // 存放IFS码
  static double k = 0.;
  static m_dir = 0;

  //IFS码赋值
  m[ 0 ][ 0 ] = 0.5;
  m[ 0 ][ 1 ] = 0;
  m[ 0 ][ 2 ] = 0;
  m[ 0 ][ 3 ] = k;
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
  m[ 2 ][ 0 ] = k;
  m[ 2 ][ 1 ] = 0;
  m[ 2 ][ 2 ] = 0;
  m[ 2 ][ 3 ] = 0.5;
  m[ 2 ][ 4 ] = 0.25;
  m[ 2 ][ 5 ] = 0.5;
  m[ 2 ][ 6 ] = 0.334;

  //循环迭代，在不同的概率空间下，赋不同的IFS码值
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
    else {
      a = m[ 2 ][ 0 ];
      b = m[ 2 ][ 1 ];
      c = m[ 2 ][ 2 ];
      d = m[ 2 ][ 3 ];
      e = m[ 2 ][ 4 ];
      f = m[ 2 ][ 5 ];
    }

    newx = (a * x) + (b * y) + e;
    newy = (c * x) + (d * y) + f;
    x = newx;
    y = newy;
    SetPixel(pDC, (1000 + 7000 * x) / 15, (6500 - 6000 * y) / 15,
        (unsigned char)(x * 500 * R), (unsigned char)(R * 100), (unsigned char)(y * 2000 * R));
    n--;
  }

  if (m_dir) {
    k += 0.01;

    if (k > 0.5) {
      m_dir = FALSE;
    }
  }
  else {
    k -= 0.01;

    if (k < 0) {
      m_dir = TRUE;
    }
  }
}

// 摇摆的Sierpinski三角形
void DrawIFS2(IDC* pDC)
{
  double x = 0;  // '仿射变换中的自变量
  double y = 0;
  double newx, newy; //'仿射变换产生的新点
  double a, b, c, d, e, f; //'仿射变幻中的系数
  long n = 10000;    // '迭代次数
  double R;   //'随机变量
  double m[ 7 ][ 7 ] = {0};    // '存放IFS码
  static double k = 0.;
  static m_dir = 0;

  //'IFS码赋值
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
  m[ 2 ][ 1 ] = k;
  m[ 2 ][ 2 ] = 0;
  m[ 2 ][ 3 ] = 0.5;
  m[ 2 ][ 4 ] = 0.25;
  m[ 2 ][ 5 ] = 0.5;
  m[ 2 ][ 6 ] = 0.334;

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
    else {
      a = m[ 2 ][ 0 ];
      b = m[ 2 ][ 1 ];
      c = m[ 2 ][ 2 ];
      d = m[ 2 ][ 3 ];
      e = m[ 2 ][ 4 ];
      f = m[ 2 ][ 5 ];
    }

    newx = (a * x) + (b * y) + e;
    newy = (c * x) + (d * y) + f;
    x = newx;
    y = newy;
    SetPixel(pDC, (int)((2000 + 7000 * x) / 15), (int)((6500 - 6000 * y) / 15),
        (unsigned char)(x * 500 * R), (unsigned char)(R * 100), (unsigned char)(y * 2000 * R));
    n--;
  }

  if (m_dir) {
    k += 0.01;

    if (k > 0.3) {
      m_dir = FALSE;
    }
  }
  else {
    k -= 0.01;

    if (k < -0.3) {
      m_dir = TRUE;
    }
  }
}

// 旋转万花筒
void DrawIFS3(IDC* pDC)
{
  double x = 0;  // '仿射变换中的自变量
  double y = 0;
  double newx, newy; //'仿射变换产生的新点
  double a, b, c, d, e, f; //'仿射变幻中的系数
  long n = 10000;    // '迭代次数
  double R;   //'随机变量
  double m[ 7 ][ 7 ] = {0};    // '存放IFS码
  static double k = 0.;
  static m_dir = 0;

  //'IFS码赋值
  m[ 0 ][ 0 ] = 0.2;
  m[ 0 ][ 1 ] = 0.2;
  m[ 0 ][ 2 ] = 0;
  m[ 0 ][ 3 ] = 0;
  m[ 0 ][ 4 ] = 0.7;
  m[ 0 ][ 5 ] = 0;
  m[ 0 ][ 6 ] = 0.2;
  m[ 1 ][ 0 ] = 0.2;
  m[ 1 ][ 1 ] = 0.2;
  m[ 1 ][ 2 ] = 0;
  m[ 1 ][ 3 ] = 0;
  m[ 1 ][ 4 ] = -0.7;
  m[ 1 ][ 5 ] = 0;
  m[ 1 ][ 6 ] = 0.2;
  m[ 2 ][ 0 ] = 0.2;
  m[ 2 ][ 1 ] = 0.2;
  m[ 2 ][ 2 ] = 0;
  m[ 2 ][ 3 ] = 0;
  m[ 2 ][ 4 ] = 0;
  m[ 2 ][ 5 ] = 0.7;
  m[ 2 ][ 6 ] = 0.2;
  m[ 3 ][ 0 ] = 0.2;
  m[ 3 ][ 1 ] = 0.2;
  m[ 3 ][ 2 ] = 0;
  m[ 3 ][ 3 ] = 0;
  m[ 3 ][ 4 ] = 0;
  m[ 3 ][ 5 ] = -0.7;
  m[ 3 ][ 6 ] = 0.2;
  m[ 4 ][ 0 ] = 0.85;
  m[ 4 ][ 1 ] = 0.85;
  m[ 4 ][ 2 ] = k;
  m[ 4 ][ 3 ] = k;
  m[ 4 ][ 4 ] = 0;
  m[ 4 ][ 5 ] = 0;
  m[ 4 ][ 6 ] = 0.2;

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
    else if (R <= m[ 0 ][ 6 ] + m[ 1 ][ 6 ] + m[ 2 ][ 6 ] + m[ 3 ][ 6 ]) {
      a = m[ 3 ][ 0 ];
      b = m[ 3 ][ 1 ];
      c = m[ 3 ][ 2 ];
      d = m[ 3 ][ 3 ];
      e = m[ 3 ][ 4 ];
      f = m[ 3 ][ 5 ];
    }
    else {
      a = m[ 4 ][ 0 ];
      b = m[ 4 ][ 1 ];
      c = m[ 4 ][ 2 ];
      d = m[ 4 ][ 3 ];
      e = m[ 4 ][ 4 ];
      f = m[ 4 ][ 5 ];
    }

    newx = (a * x * cos(c / 180)) - (b * y * sin(d / 180)) + e;
    newy = (a * x * sin(c / 180)) + (b * y * cos(d / 180)) + f;
    x = newx;
    y = newy;
    SetPixel(pDC, (300 + 200 * x), (220 - 200 * y),
        (unsigned char)(x * 500 * R), (unsigned char)(R * 100), (unsigned char)(y * 2000 * R));
    n--;
  }

  if (m_dir) {
    k += 5;

    if (k > 500) {
      m_dir = FALSE;
    }
  }
  else {
    k -= 5;

    if (k < -500) {
      m_dir = TRUE;
    }
  }

}

// 变换的芦苇
void DrawIFS4(IDC* pDC)
{
  double x = 0;  // '仿射变换中的自变量
  double y = 0;
  double newx, newy; //'仿射变换产生的新点
  double a, b, c, d, e, f; //'仿射变幻中的系数
  long n = 10000;    // '迭代次数
  double R;   //'随机变量
  double m[ 7 ][ 7 ] = {0};    // '存放IFS码
  static double k = 0.;
  static m_dir = 0;

  //'IFS码赋值
  m[ 0 ][ 0 ] = 0.5;
  m[ 0 ][ 1 ] = 0.5;
  m[ 0 ][ 2 ] = 0;
  m[ 0 ][ 3 ] = 0;
  m[ 0 ][ 4 ] = 0;
  m[ 0 ][ 5 ] = 0;
  m[ 0 ][ 6 ] = 0.3;
  m[ 1 ][ 0 ] = 0.5;
  m[ 1 ][ 1 ] = 0.5;
  m[ 1 ][ 2 ] = k;
  m[ 1 ][ 3 ] = k;
  m[ 1 ][ 4 ] = 1;
  m[ 1 ][ 5 ] = k / 1600;
  m[ 1 ][ 6 ] = 0.3;
  m[ 2 ][ 0 ] = 0.5;
  m[ 2 ][ 1 ] = 0.5;
  m[ 2 ][ 2 ] = 0;
  m[ 2 ][ 3 ] = 0;
  m[ 2 ][ 4 ] = 0.5;
  m[ 2 ][ 5 ] = 0;
  m[ 2 ][ 6 ] = 0.4;


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
    else {
      a = m[ 2 ][ 0 ];
      b = m[ 2 ][ 1 ];
      c = m[ 2 ][ 2 ];
      d = m[ 2 ][ 3 ];
      e = m[ 2 ][ 4 ];
      f = m[ 2 ][ 5 ];
    }

    newx = (a * x * cos(c / 180)) - (b * y * sin(d / 180)) + e;
    newy = (a * x * sin(c / 180)) + (b * y * cos(d / 180)) + f;
    x = newx;
    y = newy;
    SetPixel(pDC, (150 + 250 * x), (500 - 250 * y),
        (unsigned char)(x * 500 * R), (unsigned char)(R * 100), (unsigned char)(y * 2000 * R));
    n--;
  }

  if (m_dir) {
    k += 10;

    if (k > 1900) {
      m_dir = FALSE;
    }
  }
  else {
    k -= 10;

    if (k < 0) {
      m_dir = TRUE;
    }
  }
}
