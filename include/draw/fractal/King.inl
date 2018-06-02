
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"

typedef struct CSceneKing {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  CPointMover* m_MoverList;
  CColorMover* m_ColorMoverList;
  int m_MoverListSize;
  int m_ColorMoverListSize;
  long m_DrawCount;
}
CSceneKing;


void CSceneKingIntiMover(CSceneKing* ssf)
{
  const double MinV = 10.0 / 100000;
  const double MaxV = 20.0 / 100000;
  ssf->m_MoverListSize = 8;
  ssf->m_ColorMoverListSize = 1;
  ssf->m_MoverList = MALLOC(CPointMover, ssf->m_MoverListSize);
  ssf->m_ColorMoverList = MALLOC(CColorMover, ssf->m_ColorMoverListSize);
  CPointMoverInti(ssf->m_MoverList + 0, -1.5, -0.5, MinV, MaxV);
  CPointMoverInti(ssf->m_MoverList + 1, -1.5, -0.5, MinV, MaxV);
  CPointMoverInti(ssf->m_MoverList + 2, -3.5, 4, MinV, MaxV);
  CPointMoverInti(ssf->m_MoverList + 3, -3.5, 4, MinV, MaxV);
  CPointMoverInti(ssf->m_MoverList + 4, -1, 1, MinV, MaxV);
  CPointMoverInti(ssf->m_MoverList + 5, -1, 1, MinV, MaxV);
  CPointMoverInti(ssf->m_MoverList + 6, 0.8, 1.2, MinV / 5, MaxV / 5);
  CPointMoverInti(ssf->m_MoverList + 7, 0.8, 1.2, MinV / 5, MaxV / 5);

  CColorMoverInti(ssf->m_ColorMoverList, 20, 50, 10.0 / 1000, 80.0 / 1000);
}

void CSceneKingNextPos(CSceneKing* ssf, const double x0, const double y0, double* new_x, double* new_y)
{
  const double a = CPointMoverGetValue(ssf->m_MoverList + 0);
  const double al = CPointMoverGetValue(ssf->m_MoverList + 1);
  const double b = CPointMoverGetValue(ssf->m_MoverList + 2);
  const double bl = CPointMoverGetValue(ssf->m_MoverList + 3);
  const double c = CPointMoverGetValue(ssf->m_MoverList + 4);
  const double d = CPointMoverGetValue(ssf->m_MoverList + 5);
  const double e = CPointMoverGetValue(ssf->m_MoverList + 6);
  const double f = CPointMoverGetValue(ssf->m_MoverList + 7);
  *new_x = c * sin(b * x0) + e * sin(bl * y0);
  *new_y = f * sin(a * x0) + d * sin(al * y0);
  /*
  使用的方程:
  x_(n+1)=c*sin(b*x_n)+sin(b*y_n),
  y_(n+1)=sin(a*x_n)+d*sin(a*y_n),

  x:=0.1;y:=0.1;
  a:=-0.9666918; {-0.97}
  b:=2.679879; {2.8}
  c:=0.565145; {0.45,0.76}
  d:=0.744728; {0.71}
  {-1.86＜x＜1.86}; {-1.51＜y＜1.51};

  //================
  x_(n+1)=sin(a*y_n)-z*cos(b*x_n),
  y_(n+1)=z*sin(c*x_n)-cos(d*y_n),
  z_(n+1)=e*sin(x_n).

  a=2.24,b=0.43,c=-0.65,d=-2.43，0.5<e<1.0

  */
}

void CSceneKingDoDraw(CSceneKing* ssf, int dst_height, int dst_width, unsigned char* dst, int dst_step, int cn)
{
  //dst_fill(Color32(0,0,0));
  //darkle(dst_height, dst_width, dst, dst_step, cn, 1<<6);

  {
    int i, j, skip;
    const int sCountCr = 300;   //圆上 初始点数目
    const int sCountLine = 300; //直线上 初始点数目
    const int sIteratCount = ssf->m_DrawCount / (sCountCr + sCountLine);   // 每个初始点迭代的次数

    const double scrLeft = -2.2;   //屏幕映射参数
    const double scrRight = 2.2;
    const double scrTop = -2.2;
    const double scrButtom = 2.2;

    const CColorMover* ColorMover = ssf->m_ColorMoverList;

    const double vmapp_x = dst_width * (1.0 / (scrRight - scrLeft));
    const double vmapp_y = dst_height * (1.0 / (scrButtom - scrTop));

    for (i = 0; i < (sCountCr + sCountLine); i++) {
      const long csSkipCount = 15;
      double sx0, sy0;
      /*if (i<sCountCr)
      {
          const double  PI=3.1415926535897932384626433832795;
          double seta=i*(PI*2/sCountCr);
          sx0=sin(seta);
          sy0=cos(seta);
      }
      else
      {
          sx0=(i-sCountCr)*(2.0/sCountLine)+0.00-1;
          sy0=(i-sCountCr)*(2.0/sCountLine)+0.05-1;
      }*/
      sx0 = rand() * (1.0 / RAND_MAX) * (scrRight - scrLeft) + scrLeft;
      sy0 = rand() * (1.0 / RAND_MAX) * (scrButtom - scrTop) + scrTop;

      for (skip = 0; skip < csSkipCount; skip++) {
        double sxn;
        double syn;
        CSceneKingNextPos(ssf, sx0, sy0, &sxn, &syn);
        sx0 = sxn;
        sy0 = syn;
      }

      for (j = 0; j < sIteratCount; j++) {
        long px;
        long py;
        double sxn;
        double syn;
        CSceneKingNextPos(ssf, sx0, sy0, &sxn, &syn);

        px = (long)(vmapp_x * (sxn - scrLeft));
        py = (long)(vmapp_y * (syn - scrTop));

        if ((px >= 0) && (py >= 0) && (px < dst_width) && (py < dst_height)) {
          double dx = (sxn - sx0);
          double dy = (syn - sy0);
          unsigned char* pix = dst + px * cn + py * dst_step;
          PUTPIX3(pix, getR8(ColorMover, dx), getG8(ColorMover, dy), getB8(ColorMover, dx + dy));
        }

        sx0 = sxn;
        sy0 = syn;
      }
    }
  }
}

void CSceneKingClear(CSceneKing* ssf)
{
  if (ssf) {
    SAFEFREE(ssf->m_MoverList);
    SAFEFREE(ssf->m_ColorMoverList);
    SAFEFREE(ssf);
  }
}

#define CSceneKingUpdate CScenePointFractalUpdateMover

CSceneKing* CSceneKingInti()
{
  CSceneKing* ssf;
  ssf = MALLOC(CSceneKing, 1);
  CSceneKingIntiMover(ssf);
  ssf->m_DrawCount = 30000;
  SCENE_FRACTAL_SET_CLASS(ssf, CSceneKing);
  return ssf;
}
