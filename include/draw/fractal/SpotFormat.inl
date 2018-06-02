
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"

typedef struct CSceneSpotFormat {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  CPointMover* m_MoverList;
  CColorMover* m_ColorMoverList;
  int m_MoverListSize;
  int m_ColorMoverListSize;

  long m_DrawCount;
  long m_SpotCount;        //固定点点数
  long m_IteratCount;      //迭代次数
  long m_dataLength;
  double* m_S0x;
  double* m_S0y;
  double* m_S1x;
  double* m_S1y;
}
CSceneSpotFormat;

void CSceneSpotFormatIntiMover(CSceneSpotFormat* ssf)
{
  const double MinV = 0.5 / 10000;
  const double MaxV = 2.0 / 10000;
  srand(time(0));
  ssf->m_MoverListSize = 2;
  ssf->m_ColorMoverListSize = 1;
  ssf->m_MoverList = MALLOC(CPointMover, ssf->m_MoverListSize);
  ssf->m_ColorMoverList = MALLOC(CColorMover, ssf->m_ColorMoverListSize);
  CPointMoverInti(ssf->m_MoverList, -1.7, 1.7, MinV, MaxV);
  CPointMoverInti(ssf->m_MoverList + 1, -1.1, 1.1, MinV, MaxV);
  CColorMoverInti(ssf->m_ColorMoverList, 2, 4, 20.0 / 1000, 40.0 / 1000);
}

void CSceneSpotFormatIntiData(CSceneSpotFormat* ssf)
{
  int i;
  const double rev_log2_log10 = 1.0 / log10(2.0);

  if (rand() < (RAND_MAX / 2)) {
#define SpotCountListCount  6
    static const int SpotCountList[ SpotCountListCount ] = {
      3, 4, 5, 11, 36, 73
    };
    ssf->m_SpotCount = SpotCountList[ rand() % SpotCountListCount ];
  }
  else {
    ssf->m_SpotCount = rand() % (13 - 3) + 3;
  }

  //ssf->m_SpotCount=3;
  ssf->m_IteratCount = (long)(0.5 + rev_log2_log10 * log10((double) ssf->m_DrawCount / ssf->m_SpotCount)) - 2;
  ssf->m_dataLength = ssf->m_SpotCount * (1 << (ssf->m_IteratCount + 2)) + 7;
  ssf->m_S0x = MALLOC(double, ssf->m_dataLength);
  ssf->m_S0y = MALLOC(double, ssf->m_dataLength);
  ssf->m_S1x = MALLOC(double, ssf->m_dataLength);
  ssf->m_S1y = MALLOC(double, ssf->m_dataLength);

  for (i = 0; i < ssf->m_dataLength; ++i) {
    ssf->m_S0x[ i ] = 0.;
    ssf->m_S0y[ i ] = 0.;
    ssf->m_S1x[ i ] = 0.;
    ssf->m_S1y[ i ] = 0.;
  }
}

void CSceneSpotFormatClear(CSceneSpotFormat* ssf)
{
  if (ssf) {
    SAFEFREE(ssf->m_MoverList);
    SAFEFREE(ssf->m_ColorMoverList);
    SAFEFREE(ssf->m_S0x);
    SAFEFREE(ssf->m_S0y);
    SAFEFREE(ssf->m_S1x);
    SAFEFREE(ssf->m_S1y);
    SAFEFREE(ssf);
  }
}

void CSceneSpotFormatNextPos(CSceneSpotFormat* ssf)
{
  long i, k;
  double t1, t2;
  const double a = CPointMoverGetValue(ssf->m_MoverList);
  const double b = CPointMoverGetValue(ssf->m_MoverList + 1);
  //const double PI=3.1415926535897932384626433832795;
  const double rev_SpotCount = 1.0 / ssf->m_SpotCount;

  for (i = 0; i <= ssf->m_SpotCount + 4; ++i) {
    double seta = (1.5 + (i * 2 - 3) * rev_SpotCount) * PI;
    ssf->m_S0x[ i ] = cos(seta);
    ssf->m_S0y[ i ] = sin(seta);
  }

  t2 = a;
  t1 = 1 - t2 + sqr(sqr(b));
  t1 *= 0.5;
  t2 *= 0.5;

  for (k = 0; k <= ssf->m_IteratCount; ++k) {
    long posCount;
    double* srcSx;
    double* srcSy;
    double* dstSx;
    double* dstSy;

    if ((k % 2) == 0) {
      srcSx = &ssf->m_S0x[ 0 ];
      srcSy = &ssf->m_S0y[ 0 ];
      dstSx = &ssf->m_S1x[ 0 ];
      dstSy = &ssf->m_S1y[ 0 ];
    }
    else {
      srcSx = &ssf->m_S1x[ 0 ];
      srcSy = &ssf->m_S1y[ 0 ];
      dstSx = &ssf->m_S0x[ 0 ];
      dstSy = &ssf->m_S0y[ 0 ];
    }

    posCount = ssf->m_SpotCount * (1 << k) + 2;

    for (i = 0; i <= posCount; ++i) {
      dstSx[ i * 2 ] = srcSx[ i + 1 ];
      dstSy[ i * 2 ] = srcSy[ i + 1 ];
      dstSx[ i * 2 + 1 ] = t1 * (srcSx[ i + 1 ] + srcSx[ i + 2 ]) + t2 * (srcSx[ i ] + srcSx[ i + 3 ]);
      dstSy[ i * 2 + 1 ] = t1 * (srcSy[ i + 1 ] + srcSy[ i + 2 ]) + t2 * (srcSy[ i ] + srcSy[ i + 3 ]);
    }
  }
}


void CSceneSpotFormatDoDraw(CSceneSpotFormat* ssf, int dst_height, int dst_width, unsigned char* dst, int dst_step, int cn)
{
  //darkle(dst_height, dst_width, dst, dst_step, cn, 10);

  CSceneSpotFormatNextPos(ssf);

  {
    int i;
    long posCount;
    const double scrTop = -3;
    const double scrButtom = 3;
    const double scrLeft = -scrButtom * dst_width / dst_height;   //屏幕映射参数
    const double scrRight = -scrLeft;

    const CColorMover* ColorMover = ssf->m_ColorMoverList;

    const double vmapp_x = dst_width * (1.0 / (scrRight - scrLeft));
    const double vmapp_y = dst_height * (1.0 / (scrButtom - scrTop));

    const double* srcSx;
    const double* srcSy;

    if ((ssf->m_IteratCount % 2) == 0) {
      srcSx = &ssf->m_S1x[ 0 ];
      srcSy = &ssf->m_S1y[ 0 ];
    }
    else {
      srcSx = &ssf->m_S0x[ 0 ];
      srcSy = &ssf->m_S0y[ 0 ];
    }

    posCount = (ssf->m_SpotCount * (1 << (ssf->m_IteratCount + 1)));

    for (i = 1; i <= posCount; ++i) {
      long px = (long)(vmapp_x * (srcSx[ i ] - scrLeft));
      long py = (long)(vmapp_y * (srcSy[ i ] - scrTop));

      if ((px >= 0) && (py >= 0) && (px < dst_width) && (py < dst_height)) {
        double dx = (srcSx[ i ] - srcSx[ i - 1 ]);
        double dy = (srcSy[ i ] - srcSy[ i - 1 ]);
        unsigned char* pix = dst + px * cn + py * dst_step;
        PUTPIX3(pix, getR8(ColorMover, dx), getG8(ColorMover, dy), getB8(ColorMover, dx + dy));
      }
    }
  }
}

#define CSceneSpotFormatUpdate CScenePointFractalUpdateMover

CSceneSpotFormat* CSceneSpotFormatInti()
{
  CSceneSpotFormat* ssf;
  ssf = MALLOC(CSceneSpotFormat, 1);
  CSceneSpotFormatIntiMover(ssf);
  ssf->m_DrawCount = 80000;
  CSceneSpotFormatIntiData(ssf);
  SCENE_FRACTAL_SET_CLASS(ssf, CSceneSpotFormat);
  return ssf;
}
