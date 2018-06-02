
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"

typedef struct CSceneMira {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  CPointMover* m_MoverList;
  CColorMover* m_ColorMoverList;
  int m_MoverListSize;
  int m_ColorMoverListSize;
  long m_DrawCount;
}
CSceneMira;


void CSceneMiraIntiMover(CSceneMira* ssf)
{
  const double MinV = 0.3 / 10000;
  const double MaxV = 1.5 / 10000;
  ssf->m_MoverListSize = 3;
  ssf->m_ColorMoverListSize = 1;
  ssf->m_MoverList = MALLOC(CPointMover, ssf->m_MoverListSize);
  ssf->m_ColorMoverList = MALLOC(CColorMover, ssf->m_ColorMoverListSize);
  CPointMoverInti(ssf->m_MoverList + 0, -0.5, 1, MinV * 0.5, MaxV * 0.5);
  CPointMoverInti(ssf->m_MoverList + 1, 0.5, 1, MinV * 1, MaxV * 1);
  CPointMoverInti(ssf->m_MoverList + 2, 1.5, 3, MinV * 3, MaxV * 3);
  CColorMoverInti(ssf->m_ColorMoverList, 3, 5, 10.0 / 1000, 60.0 / 1000);
}

void CSceneMiraNextPos(CSceneMira* ssf, const double x0, const double y0, double* new_x, double* new_y)
{
  const double a = CPointMoverGetValue(ssf->m_MoverList + 0);
  const double b = CPointMoverGetValue(ssf->m_MoverList + 1);
  const double c = CPointMoverGetValue(ssf->m_MoverList + 2);
  double x1;
  *new_x = x1 = b * y0 + (a * x0 + (1 - a) * c / (1 + 1 / (x0 * x0)));
  *new_y = -x0 + (a * x1 + (1 - a) * c / (1 + 1 / (x1 * x1)));
}

void CSceneMiraDoDraw(CSceneMira* ssf, int dst_height, int dst_width, unsigned char* dst, int dst_step, int cn)
{
  //dst_fill(Color32(0,0,0));
  //darkle(dst_height, dst_width, dst, dst_step, cn, 1<<4);

  {
    int i, j, skip;
    const int sCount = 200; //初始点数目
    const int sIteratCount = ssf->m_DrawCount / (sCount);   // 每个初始点迭代的次数

    const double scrLeft = -14;   //屏幕映射参数
    const double scrRight = 16;
    const double scrTop = -17;
    const double scrButtom = 13;

    const CColorMover* ColorMover = ssf->m_ColorMoverList;

    const double vmapp_x = dst_width * (1.0 / (scrRight - scrLeft));
    const double vmapp_y = dst_height * (1.0 / (scrButtom - scrTop));

    for (i = 0; i < (sCount); i++) {
      double sx0, sy0;
      const long csSkipCount = 23;
      sx0 = rand() * (1.0 / RAND_MAX) * (scrRight - scrLeft) + scrLeft;
      sy0 = rand() * (1.0 / RAND_MAX) * (scrButtom - scrTop) + scrTop;

      for (skip = 0; skip < csSkipCount; skip++) {
        double sxn;
        double syn;
        CSceneMiraNextPos(ssf, sx0, sy0, &sxn, &syn);
        sx0 = sxn;
        sy0 = syn;
      }

      for (j = 0; j < sIteratCount; j++) {
        long px;
        long py;
        double sxn;
        double syn;
        CSceneMiraNextPos(ssf, sx0, sy0, &sxn, &syn);

        if (fabs(sxn) + fabs(syn) > 1e8) {
          break;
        }

        px = (long)(vmapp_x * (sxn - scrLeft));
        py = (long)(vmapp_y * (syn - scrTop));

        if ((px >= 0) && (py >= 0) && (px < dst_width) && (py < dst_height)) {
          double dx = (sxn - sx0);
          double dy = (syn - sy0);
          unsigned char* pix = dst + px * cn + py * dst_step;
          //dst_Pixels(px,py)=Color32(ColorMover.getR8(dx),ColorMover.getG8(dy),ColorMover.getB8(dx+dy));
          PUTPIX3(pix, getR8(ColorMover, dx), getG8(ColorMover, dy), getB8(ColorMover, dx + dy));
        }

        sx0 = sxn;
        sy0 = syn;
      }
    }
  }
}

void CSceneMiraClear(CSceneMira* ssf)
{
  if (ssf) {
    SAFEFREE(ssf->m_MoverList);
    SAFEFREE(ssf->m_ColorMoverList);
    SAFEFREE(ssf);
  }
}

#define CSceneMiraUpdate CScenePointFractalUpdateMover

void* CSceneMiraInti()
{
  CSceneMira* ssf;
  ssf = MALLOC(CSceneMira, 1);
  CSceneMiraIntiMover(ssf);
  ssf->m_DrawCount = 50000;
  SCENE_FRACTAL_SET_CLASS(ssf, CSceneMira);
  return ssf;
}
