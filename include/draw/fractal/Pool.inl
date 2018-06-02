
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"
#include "xtypes.h"

#define CScenePoolSTONE_TYPE_1_1 0
#define CScenePoolSTONE_TYPE_2_2 1
#define CScenePoolSTONE_TYPE_4_4 2
#define CScenePoolSTONE_TYPE_ROUND_8_8 3

int CScenePoolRoundStone_1_1[ 1 ] = {128};

int CScenePoolRoundStone_2_2[ 16 ] = {
  127, 127,
  127, 127,
};
int CScenePoolRoundStone_4_4[ 16 ] = {
  65, 95, 95, 65,
  95, 100, 100, 95,
  95, 100, 100, 95,
  65, 95, 95, 65
};
int CScenePoolRoundStone_8_8[ 64 ] = {
  0, 1, 5, 10, 10, 5, 1, 0,
  1, 6, 10, 60, 60, 10, 6, 1,
  5, 10, 65, 95, 95, 65, 10, 5,
  10, 60, 95, 100, 100, 95, 60, 10,
  10, 60, 95, 100, 100, 95, 60, 10,
  5, 10, 65, 95, 95, 65, 10, 5,
  1, 6, 10, 60, 60, 10, 6, 1,
  0, 1, 5, 10, 10, 5, 1, 0
};

typedef struct CScenePool {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
}
CScenePool;

typedef int COLOR_INT;

unsigned char m_decayFactor;
int m_iWidth, m_iHeight;
int m_iArrayLen;
COLOR_INT* waterA, *waterB;
unsigned char* backImage;

void CScenePoolThrowStone(int x, int y, int type, int power)
{
  int i, j;
  int xOff, yOff;
  int stoneW = 1;
  int stoneH = 1;
  int* RoundStone;

  if (x < 0 || y < 0 || x > m_iWidth || y > m_iHeight) {
    return ;
  }

  switch (type) {
  case CScenePoolSTONE_TYPE_ROUND_8_8: {
    stoneW = 8;
    stoneH = 8;
    RoundStone = CScenePoolRoundStone_8_8;
  }
  break;

  case CScenePoolSTONE_TYPE_4_4: {
    stoneW = 4;
    stoneH = 4;
    RoundStone = CScenePoolRoundStone_4_4;
  }
  break;

  case CScenePoolSTONE_TYPE_2_2: {
    stoneW = 2;
    stoneH = 2;
    RoundStone = CScenePoolRoundStone_2_2;
  }

  case CScenePoolSTONE_TYPE_1_1: {
    stoneW = 1;
    stoneH = 1;
    RoundStone = CScenePoolRoundStone_1_1;
  }

  default:
    stoneW = 1;
    stoneH = 1;
  }

  xOff = x; //(m_iWidth-stoneW)>>1;
  yOff = y; //(m_iHeight-stoneH)>>1;

  if (waterB) {
    for (j = 0; j < stoneH; j++)
      for (i = 0; i < stoneW; i++) {
        waterB[ i + xOff + (yOff + j) * m_iWidth ] += RoundStone[ i + j * stoneW ] << power;
      }
  }
}

void CScenePoolSetDecayFactor(unsigned char decayFactor)
{
  if (decayFactor >= 0 && decayFactor <= 15) {
    m_decayFactor = decayFactor;
  }
}
int m_iBackImageStep;

void CScenePoolCreat(CScenePool* sw, int h, int w, unsigned char* image, int dst_step, int cn)
{
  int len = w * h;
  int a;

  if (m_iWidth == w && m_iHeight == h) {
    return ;
  }

  m_iWidth = w;
  m_iHeight = h;
  m_iArrayLen = m_iWidth * m_iHeight;
  //backImage=(COLOR_INT *)pImage;
  {
    m_iBackImageStep = w * 3;
    SAFEFREE(backImage);
    backImage = (unsigned char*) malloc(h * m_iBackImageStep);
    {
      SKIP_COPY(h, w * cn, image, dst_step, 1, backImage, m_iBackImageStep, 1);
    }
  }
  waterA = MALLOC(COLOR_INT, m_iArrayLen);
  waterB = MALLOC(COLOR_INT, m_iArrayLen);

  for (a = 0; a < m_iArrayLen; a++) {
    waterA[ a ] = waterB[ a ] = 0;
  }

  CScenePoolThrowStone(w >> 2, (h >> 1) + (h >> 2), CScenePoolSTONE_TYPE_2_2, 4);
}

void CScenePoolDoDraw(CScenePool* sw, int h, int w, unsigned char* image, int dst_step, int cn)
{
  int a, u, v;
  int t, b, l, r;
  int xoff, yoff;
  COLOR_INT* temp;
  CScenePoolCreat(sw, h, w, image, dst_step, cn);

  for (a = m_iWidth; a < m_iArrayLen - m_iWidth; a++) {
    if (a % m_iWidth == 0) {
      continue;
    }

    //能量的扩散
    t = waterB[ a - m_iWidth ];
    b = waterB[ a + m_iWidth ];
    r = waterB[ a + 1 ];
    l = waterB[ a - 1 ];
    waterA[ a ] = ((t + b + l + r) >> 1) - waterA[ a ];

    //能量的衰减
    if (m_decayFactor > 0) {
      waterA[ a ] -= (waterA[ a ] >> m_decayFactor);
    }

    //(waterA[a]>>7)+(waterA[a]>>8)+(waterA[a]>>9);

  }

  temp = waterA;
  waterA = waterB;
  waterB = temp;

  for (v = 1; v < m_iHeight - 1; v++) {
    for (u = 1; u < m_iWidth - 1; u++) {
      //判断坐标是否在窗口范围内
      xoff = waterA[ u - 1 + v * m_iWidth ] - waterA[ u + 1 + v * m_iWidth ];
      yoff = waterA[ u + (v - 1) * m_iWidth ] - waterA[ u + 1 + (v + 1) * m_iWidth ];

      if ((v + yoff) < 0) {
        continue;
      }

      if ((v + yoff) > m_iHeight) {
        continue;
      }

      if ((u + xoff) < 0) {
        continue;
      }

      if ((u + xoff) > m_iWidth) {
        continue;
      }

      //计算出偏移象素和原始象素的内存地址偏移量
      image[ u + dst_step * v ] = backImage[ u + xoff + (v + yoff) * m_iWidth * cn ];
      image[ u + dst_step * v + 1 ] = backImage[ u + xoff + (v + yoff) * m_iWidth * cn + 1 ];
      image[ u + dst_step * v + 2 ] = backImage[ u + xoff + (v + yoff) * m_iWidth * cn + 2 ];
      //result[u+m_iWidth*v]=RGB(yoff,xoff,0);
    }
  }
}

void CScenePoolClear(CScenePool* sw)
{
  // Cleanup
  SAFEFREE(waterA);
  SAFEFREE(waterB);
  SAFEFREE(backImage);
  SAFEFREE(sw);
}

void CScenePoolUpdate(CScenePool* sw, unsigned long StepTime_ms)
{
#if 0
  int type = 1 + StepTime_ms % 4;
  int h, w;
  h = m_iHeight;
  w = m_iWidth;

  switch (type) {
  case 1:
    CScenePoolThrowStone(w >> 2, (h >> 1) + (h >> 2), CScenePoolSTONE_TYPE_1_1, 1);
    break;

  case 2:
    CScenePoolThrowStone(w >> 2, (h >> 1) + (h >> 2), CScenePoolSTONE_TYPE_2_2, 4);
    break;

  case 3:
    CScenePoolThrowStone(w >> 2, (h >> 1) + (h >> 2), CScenePoolSTONE_TYPE_4_4, 16);
    break;

  case 4:
    CScenePoolThrowStone(w >> 2, (h >> 1) + (h >> 2), CScenePoolSTONE_TYPE_ROUND_8_8, 64);
    break;
  };

#endif
}

CScenePool* CScenePoolInti()
{
  CScenePool* sw = MALLOC(CScenePool, 1);
  MEMSET(sw, 0, 1);

  m_decayFactor = 6;
  SCENE_FRACTAL_SET_CLASS(sw, CScenePool);
  return sw;
}

