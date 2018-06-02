
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"
//#include "MMTime.h"

typedef struct CSceneNewton {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  CColorMover m_ColorMover;
  double m_ColorK1;
  double m_ColorK2;
  double m_ColorK3;
  long m_UpdateTime;
  BOOL m_IsExtract3Ex;
  long m_ExtractNumber;
  BOOL m_isTanRev;
  long m_iteratInc;
}
CSceneNewton;

double intpow(double x, int N)
{
  switch (N) {
  case 0: {
    return 1;
  }
  break;

  case 1: {
    return x;
  }
  break;

  case 2: {
    return x * x;
  }
  break;

  case 3: {
    return x * x * x;
  }
  break;

  case 4: {
    return sqr(x * x);
  }
  break;

  case 5: {
    return sqr(x * x) * x;
  }
  break;

  case 6: {
    return sqr(x * x * x);
  }
  break;

  default: {
    long half = N >> 1;
    double xh;
    xh = intpow(x, half);
    xh = sqr(xh);

    if ((N & 1) == 0) {
      return xh;
    }
    else {
      return xh * x;
    }
  }
  }
}

void CSceneNewtonUpdate(CSceneNewton* sj, unsigned long StepTime_ms)
{
#define ExtractNumberCountListCount 11
  static const int ExtractNumberCountList[ ExtractNumberCountListCount ] = {
    3, 3, 3, 3, 4, 4, 5, 5, 6, 7, 8
  };
  double r;
  sj->m_ExtractNumber = ExtractNumberCountList[ rand() % ExtractNumberCountListCount ];
  sj->m_IsExtract3Ex = (sj->m_ExtractNumber == 3) && (rand() > (RAND_MAX / 2));

  sj->m_ColorK1 = 0;
  sj->m_ColorK2 = 0;
  sj->m_ColorK3 = 0;

  while ((sj->m_ColorK1 + sj->m_ColorK2 + sj->m_ColorK3) < 0.8) {
    sj->m_ColorK1 = (rand() * (1.0 / RAND_MAX) * rand() * (1.0 / RAND_MAX) * rand() * (1.0 / RAND_MAX));
    sj->m_ColorK2 = (rand() * (1.0 / RAND_MAX));
    sj->m_ColorK3 = (rand() * (1.0 / RAND_MAX));
  }

  if (rand() < (RAND_MAX / 2)) {
    sj->m_ColorK1 *= -1;
  }

  if (rand() < (RAND_MAX / 2)) {
    sj->m_ColorK2 *= -1;
  }

  if (rand() < (RAND_MAX / 2)) {
    sj->m_ColorK3 *= -1;
  }

  r = 1.0 / (1 << (sj->m_ExtractNumber - 3));
  r = pow(r, 0.095);
  sj->m_ColorK1 *= r;
  sj->m_ColorK2 *= r;
  sj->m_ColorK3 *= r;
  CColorMoverInti(&sj->m_ColorMover, 50 * r, 90 * r, 200000.0 / 1000, 400000.0 / 1000);

  sj->m_isTanRev = (rand() > (RAND_MAX / 4));

  if (sj->m_isTanRev) {
    if (sj->m_ExtractNumber == 3) {
      sj->m_iteratInc = 1 + (rand() % 6);
    }
    else {
      sj->m_iteratInc = 1 + (rand() % 4);
    }
  }
  else {
    sj->m_iteratInc = 1 + (rand() % 3);
  }
}


const double eValue = 0.01;
double mLog(double x)
{
  if (x < eValue) {
    x = pow(x * (1.0 / eValue), 0.3) * eValue;
  }

  return log(x);
}

void getNextPos_3Ex(const double x0, const double y0, double* out_x, double* out_y)
{
  double x2 = x0 * x0;
  double y2 = y0 * y0;
  double r = (1.0 / 6) / sqr(x2 + y2 + 1e-300);
  double a = x2 - y2;
  double b = x0 * y0 * 2;
  *out_x = -y0 + (a - b) * r;
  *out_y = x0 - (a + b) * r;
}

void getExtractByNewton_3Ex(double x0, double y0, long iteratInc, double* dL1, double* dL2, double* dL3)
{
  int i;
  double x1, y1;
  x0 *= 0.75;
  y0 *= 0.75;
  //Z^3-1=0
  x1 = x0;
  y1 = y0;

  for (i = 0; i < iteratInc; ++i) {
    x0 = x1;
    y0 = y1;
    getNextPos_3Ex(x0, y0, &x1, &y1);
  }

  *dL1 = mLog(fabs(x1 - x0) * fabs(y1 - y0)) * 0.6;
  *dL2 = mLog(sqr(x1 - x0) + sqr(y1 - y0)) * 0.6;

  getNextPos_3Ex(x1, y1, &x0, &y0);
  *dL3 = mLog(fabs(x1 - x0) + fabs(y1 - y0)) * 2.0;
}


void sqr1(const double x, const double y, double* out_x, double* out_y)
{
  *out_x = x * x - y * y;
  *out_y = 2 * x * y;
}

void mul1(const double x0, const double y0, const double x1, const double y1, double* out_x, double* out_y)
{
  *out_x = x0 * x1 - y0 * y1;
  *out_y = x0 * y1 + x1 * y0;
}

void pow1(double x, double y, long N, double* out_x, double* out_y)
{
  switch (N) {
  case 0: {
    *out_x = 1;
    *out_y = 0;
  }
  break;

  case 1: {
    *out_x = x;
    *out_y = y;
  }
  break;

  case 2: {
    sqr1(x, y, out_x, out_y);
  }
  break;

  case 3: {
    double x1, y1;
    sqr1(x, y, &x1, &y1);
    mul1(x, y, x1, y1, out_x, out_y);
  }
  break;

  case 4: {
    double x1, y1;
    sqr1(x, y, &x1, &y1);
    sqr1(x1, y1, out_x, out_y);
  }
  break;

  case 5: {
    double x1, y1, x2, y2;
    sqr1(x, y, &x1, &y1);
    sqr1(x1, y1, &x2, &y2);
    mul1(x, y, x2, y2, out_x, out_y);
  }
  break;

  case 6: {
    double x1, y1, x2, y2;
    sqr1(x, y, &x1, &y1);
    sqr1(x1, y1, &x2, &y2);
    mul1(x1, y1, x2, y2, out_x, out_y);
  }
  break;

  default: {
    long half = N >> 1;
    double xh, yh;
    pow1(x, y, half, &xh, &yh);

    if ((N & 1) == 0) {
      sqr1(xh, yh, out_x, out_y);
    }
    else {
      double xsqr, ysqr;
      sqr1(xh, yh, &xsqr, &ysqr);
      mul1(x, y, xsqr, ysqr, out_x, out_y);
    }
  }
  }
}
void div1(double x0, double y0, double x1, double y1, double* out_x, double* out_y)
{
  double r = 1 / (x1 * x1 + y1 * y1 + 1e-300);
  *out_x = (x0 * x1 + y0 * y1) * r;
  *out_y = (y0 * x1 - x0 * y1) * r;
}



//double PI=3.1415926535897932384626433832795;
double asin2(double x, double y, double r)
{
  double seta = asin(y / r);

  if (x >= 0) {
    return seta;
  }
  else if (y >= 0) {
    return PI - seta;
  }
  else {
    return -PI - seta;
  }
}
void getNextPos(double x0, double y0, long N, BOOL isTanRev, double* out_x, double* out_y)
{
  //Z^N-1=0
  double seta, r, sl;

  if (isTanRev) {
    seta = atan2(x0, y0);
  }
  else {
    seta = atan2(y0, x0);
  }

  r = sqrt(x0 * x0 + y0 * y0);
  r = r * (N - 1) / N;
  sl = 1.0 / (N * intpow(r, (N - 1)));
  *out_x = (r * cos(seta) + sl * cos((1 - N) * seta));
  *out_y = (r * sin(seta) + sl * sin((1 - N) * seta));

  /*
  //实际牛顿迭代方程
  double xndel,yndel;
  pow(x0,y0,N-1,xndel,yndel);
  double xn,yn;
  mul(x0,y0,xndel,yndel,xn,yn);
  double x,y;
  div(xn-1,yn,N*xndel,N*yndel,x,y);
  out_x=x0-x;
  out_y=y0-y;
  */
}
void getExtractByNewton(double x0, double y0, long N, long iteratInc, BOOL isTanRev, double* dL1, double* dL2, double* dL3)
{
  long i;
  //Z^N-1=0
  double x1 = x0, y1 = y0;

  for (i = 0; i < iteratInc; ++i) {
    x0 = x1;
    y0 = y1;
    getNextPos(x0, y0, N, isTanRev, &x1, &y1);
  }

  *dL1 = mLog(fabs((x1 - x0) * (y1 - y0))) * 0.6;
  *dL2 = mLog(sqr(x1 - x0) + sqr(y1 - y0)) * 0.6;

  getNextPos(x1, y1, N, isTanRev, &x0, &y0);
  *dL3 = mLog(fabs(x1 - x0) + fabs(y1 - y0)) * 2.0;
}

void CSceneNewtongetColor(CSceneNewton* sj, double dL1, const double dL2, double dL3, unsigned char* out)
{
  const CColorMover* m_ColorMover = &sj->m_ColorMover;
  double kR = dL1 * sj->m_ColorK1 + dL2 * sj->m_ColorK2 - dL3 * sj->m_ColorK3;
  double kG = dL1 * sj->m_ColorK1 - dL2 * sj->m_ColorK2 + dL3 * sj->m_ColorK3;
  double kB = -dL1 * sj->m_ColorK1 + dL2 * sj->m_ColorK2 + dL3 * sj->m_ColorK3;
  // Color32(sj->m_ColorMover.getR8(kR),sj->m_ColorMover.getG8(kG),sj->m_ColorMover.getB8(kB));
  PUTPIX3(out, getR8(m_ColorMover, kR), getG8(m_ColorMover, kG), getB8(m_ColorMover, kB));
}

void CSceneNewtonDoDraw(CSceneNewton* sj, int dst_height, int dst_width, unsigned char* dst_pdata, int dst_step, int cn)
{
  long x, y;
  const double rTop = -1.37;
  const double rBottom = -rTop + 0.000001;
  const double rLeft = -(rBottom * dst_width) / dst_height;
  const double rRight = -rLeft + 0.000002;

  const double xJScale = ((rRight - rLeft) / dst_width);
  const double yJScale = ((rBottom - rTop) / dst_height);

  for (y = 0; y < dst_height; ++y, dst_pdata += dst_step) {
    double ry0 = y * yJScale + rTop;
    unsigned char* dstLine = dst_pdata;

    for (x = 0; x < dst_width; ++x, dstLine += cn) {
      double rx0 = x * xJScale + rLeft;

      double dL1, dL2, dL3;

      if (sj->m_IsExtract3Ex) {
        getExtractByNewton_3Ex(rx0, ry0, sj->m_iteratInc, &dL1, &dL2, &dL3);
      }
      else {
        getExtractByNewton(rx0, ry0, sj->m_ExtractNumber, sj->m_iteratInc, sj->m_isTanRev, &dL1, &dL2, &dL3);
      }

      CSceneNewtongetColor(sj, dL1, dL2, dL3, dstLine);
    }
  }

  return ; //留到下一帧更新
}

void CSceneNewtonClear(CSceneNewton* sj)
{
  SAFEFREE(sj);
}

void* CSceneNewtonInti()
{
  CSceneNewton* sj = NULL;

  sj = MALLOC(CSceneNewton, 1);
  sj->m_UpdateTime = 0;
  CSceneNewtonUpdate(sj, 0);
  SCENE_FRACTAL_SET_CLASS(sj, CSceneNewton);
  return sj;
}
