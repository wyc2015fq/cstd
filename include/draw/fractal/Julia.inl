
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"

#define MWidth    (512)
#define MHeight   (MWidth+1)
#define rLeft     (-2.02)
#define rRight    ( 2.02)
#define rTop      (-2.02)
#define rBottom   ( 2.02)
#define xMScale  ((rRight-rLeft)/MWidth)
#define yMScale  ((rBottom-rTop)/MHeight)


typedef struct CSceneJulia {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  CColorMover m_ColorMoverIn;
  CColorMover m_ColorMoverOut;
  double m_Mx0;
  double m_My0;
  char m_MData[ MWidth* MHeight ]; //Mandelbrot集合的逃逸数据轮廓  来得到较合适的Julia集参数
}
CSceneJulia;

#if 0
BOOL M_getIsOut4(const double x, const double y)
{
  return (x * x + y * y) >= 4;
}
double sqr(const double x)
{
  return x * x;
}
long MIndex(long x, long y)
{
  return y * MWidth + x;
}
#else
#define M_getIsOut4(_x, _y) (((_x)*(_x)+(_y)*(_y))>=4)
#define MIndex(_x, _y)      ((_y)*MWidth+(_x))
#endif

BOOL M_getIsInRegion(const double x0, const double y0, const long MaxTestIteratCount)
{
  long i;
  double x = x0;
  double y = y0;

  if (M_getIsOut4(x0, y0)) {
    return FALSE;
  }

  for (i = 0; i < MaxTestIteratCount; ++i) {
    //复函数迭代  f(C)=C^2+C0
    double new_x = x * x - y * y + x0;
    double new_y = 2 * x * y + y0;

    if (M_getIsOut4(new_x, new_y)) {
      return FALSE;
    }

    x = new_x;
    y = new_y;
  }

  return TRUE;
}

BOOL J_getIsInRegion(double Mx0, double My0, double Jx0, double Jy0, long TestIteratCount,
    double* out_new_DJx, double* out_new_DJy, long* out_IteratCount)
{
  long i;
  const long csTestDXYCount = 20;
  double x = Jx0;
  double y = Jy0;
  double new_dx, new_dy;

  if (M_getIsOut4(Jx0, Jy0)) {
    *out_IteratCount = 0;
    return FALSE;
  }

  for (i = 0; i < csTestDXYCount; ++i) {
    //复函数迭代  f(C)=C^2+MC0;
    double new_x = x * x - y * y + Mx0;
    double new_y = 2 * x * y + My0;

    if (M_getIsOut4(new_x, new_y)) {
      *out_IteratCount = i + 1;
      return FALSE;
    }

    x = new_x;
    y = new_y;
  }

  {
    double new_x = x * x - y * y + Mx0;
    double new_y = 2 * x * y + My0;
    i = csTestDXYCount;

    if (M_getIsOut4(new_x, new_y)) {
      *out_IteratCount = i + 1;
      return FALSE;
    }

    new_dx = new_x - x;
    new_dy = new_y - y;
  }

  for (i = csTestDXYCount + 1; i < TestIteratCount + 1; ++i) {
    //复函数迭代  f(C)=C^2+MC0;
    double new_x = x * x - y * y + Mx0;
    double new_y = 2 * x * y + My0;

    if (M_getIsOut4(new_x, new_y)) {
      *out_IteratCount = i + 1;
      return FALSE;
    }

    x = new_x;
    y = new_y;
  }

  *out_new_DJx = (fabs(new_dx) + fabs(new_dy));
  *out_new_DJy = sqrt(fabs(sqr(new_dx) + sqr(new_dy)));
  return TRUE;
}

void CSceneJuliaClear(CSceneJulia* sj)
{
  SAFEFREE(sj);
}

void CSceneJuliaUpdate(CSceneJulia* sj, unsigned long StepTime_ms)
{
  // CColorMoverUpdate(&(sj->m_ColorMoverIn), StepTime_ms);
  // CColorMoverUpdate(&(sj->m_ColorMoverOut), StepTime_ms);
  CColorMoverInti(&(sj->m_ColorMoverIn), 300, 450, 200000.0 / 1000, 400000.0 / 1000);
  CColorMoverInti(&(sj->m_ColorMoverOut), 250, 350, 200000.0 / 1000, 400000.0 / 1000);

  while (TRUE) {
    double Mx0Rnd = ((rand() * RAND_MAX) + rand()) * (1.0 / RAND_MAX / RAND_MAX);
    double My0Rnd = ((rand() * RAND_MAX) + rand()) * (1.0 / RAND_MAX / RAND_MAX);
    long iMx0 = (long)(Mx0Rnd * (MWidth - 0.0001));
    long iMy0 = (long)(My0Rnd * (MHeight - 0.0001));

    if (sj->m_MData[ MIndex(iMx0, iMy0) ]) {
      sj->m_Mx0 = Mx0Rnd * (rRight - rLeft) + rLeft;
      sj->m_My0 = My0Rnd * (rBottom - rTop) + rTop;
      break;
    }
  }
}

void Debug_DrawM(int dst_height, int dst_width, unsigned char* dst_pdata, int dst_step, unsigned char* MData)
{
  long x, y;
  unsigned char* dstLine = dst_pdata;

  for (y = 0; y < dst_height; ++y) {
    long my = y % MHeight;

    for (x = 0; x < dst_width; ++x) {
      long mx = x % MWidth;
      unsigned char* ptr = dstLine + 3 * x;
      unsigned char c = MData[ MIndex(mx, my) ] * 255;
      //dstLine[x]=color32(c,c,c);
      PUTPIX3(ptr, c, c, c);
    }

    dstLine += dst_step;
  }
}

void CSceneJuliagetColor(const CColorMover* ColorMoverIn, const CColorMover* ColorMoverOut,
    double Mx0, double My0, double rx0, double ry0, unsigned char* out)
{
  double dx, dy;
  long iteratCount;

  if (J_getIsInRegion(Mx0, My0, rx0, ry0, 1000, &dx, &dy, &iteratCount)) {
    PUTPIX3(out, getR8(ColorMoverIn, dx), getG8(ColorMoverIn, dy), getB8(ColorMoverIn, dx + dy));
  }
  else {
    if (iteratCount < 40) {
      PUTPIX3(out, 0, 0, 0);
    }
    else {
      double d = sqrt(sqrt((double) iteratCount));
      PUTPIX3(out, getR8(ColorMoverOut, d), getG8(ColorMoverOut, d + 63), getB8(ColorMoverOut, d + 127));
    }
  }
}

void CSceneJuliaDoDraw(CSceneJulia* sj, int dst_height, int dst_width, unsigned char* dst_pdata, int dst_step, int cn)
{
  //Debug_DrawM(dst,sj->m_MData);  return;

  long x, y;
  const double zZoomX = 0.75;
  const double zZoomY = 0.6;
  const double xJScale = ((rRight - rLeft) * zZoomX / dst_width);
  const double yJScale = ((rBottom - rTop) * zZoomY / dst_height);

  for (y = 0; y < dst_height; ++y, dst_pdata += dst_step) {
    double ry0 = y * yJScale + rTop * zZoomY;
    unsigned char* dstLine = dst_pdata;

    for (x = 0; x < dst_width; ++x, dstLine += cn) {
      double rx0 = x * xJScale + rLeft * zZoomX;
      CSceneJuliagetColor(&sj->m_ColorMoverIn, &sj->m_ColorMoverOut,
          sj->m_Mx0, sj->m_My0, rx0, ry0, dstLine);
    }

    (dstLine) += dst_step;
  }

  return ; //留到下一帧更新
}

CSceneJulia* CSceneJuliaInti()
{
  long x, y;
  char MDataTmp[ MWidth * MHeight ];
  const long TestIteratCount = 80;
  CSceneJulia* sj = NULL;

  //得到M集合的迭代逸出数据

  sj = MALLOC(CSceneJulia, 1);

  if (NULL == sj) {
    return 0;
  }

  //MDataTmp = MALLOC(char, MWidth*MHeight);
  memset(MDataTmp, 0, MWidth * MHeight);

  for (y = 0; y <= MHeight / 2; ++y) {
    double ry0 = y * yMScale + rTop;

    for (x = 0; x < MWidth; ++x) {
      double rx0 = x * xMScale + rLeft;
      BOOL isInRegion = M_getIsInRegion(rx0, ry0, TestIteratCount);
      MDataTmp[ MIndex(x, y) ] = (char) isInRegion;
      MDataTmp[ MIndex(x, MHeight - 1 - y) ] = (char) isInRegion;
    }
  }

  //sj->m_MData = MALLOC(char, MWidth*MHeight);
  memset(sj->m_MData, 0, MWidth * MHeight);

  for (y = 1; y <= MHeight / 2; ++y) {
    for (x = 1; x < MWidth - 1; ++x) {
      long sum = MDataTmp[ MIndex(x - 1, y - 1) ] + MDataTmp[ MIndex(x, y - 1) ] + MDataTmp[ MIndex(x + 1, y - 1) ]
          + MDataTmp[ MIndex(x - 1, y) ] + MDataTmp[ MIndex(x, y) ] + MDataTmp[ MIndex(x + 1, y) ]
          + MDataTmp[ MIndex(x - 1, y + 1) ] + MDataTmp[ MIndex(x, y + 1) ] + MDataTmp[ MIndex(x + 1, y + 1) ] ;

      BOOL isInRegion = ((1 << sum) & ((1 << 9) - 2)) != 0;
      sj->m_MData[ MIndex(x, y) ] = (char) isInRegion;
      sj->m_MData[ MIndex(x, MHeight - 1 - y) ] = (char) isInRegion;
    }
  }

  CSceneJuliaUpdate(sj, 0);
  //SAFEFREE(MDataTmp);
  SCENE_FRACTAL_SET_CLASS(sj, CSceneJulia);
  return sj;
}

#undef MWidth
#undef MHeight
#undef rLeft
#undef rRight
#undef rTop
#undef rBottom
#undef xMScale
#undef yMScale
