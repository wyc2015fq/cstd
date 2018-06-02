
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>



typedef struct tagFPAIR {
  int first;
  float second;
}
FPAIR, *PFPAIR;

typedef struct tagIPAIR {
  int first;
  int second;
}
IPAIR, *PIPAIR;

PIPAIR Spherize_generateTable(PIPAIR pTable, int Radius, double m_Rate)
{
  int h, w;
  int R = (int) sqrt(Radius * Radius / (1 - m_Rate * m_Rate));
#if 0

  int d = sqrt(R * R - Radius * Radius);
#else

  int d = (int)(m_Rate * R);
#endif

  int num = (Radius + 1) * (Radius + 1);

  if (pTable) {
    pfree(pTable);
    pTable = NULL;
  }

  pTable = (IPAIR*) pmalloc(num * sizeof(IPAIR));

  for (h = 0; h <= Radius; h++) {
    for (w = 0; w <= Radius; w++) {
      int srcR2 = h * h + w * w;
      int srcR = (int)(sqrt(srcR2));
      int desR = (int)(srcR * R / sqrt(srcR2 + d * d));
      pTable[ h * Radius + w ].first = (desR == 0) ? 0 : srcR * w / desR;   //x
      pTable[ h * Radius + w ].second = (desR == 0) ? 0 : srcR * h / desR;   //y
    }
  }

  return pTable;
}

#define getMapDeltaXY( y, x )  pTable[(y)*(Radius)+(x)]
#define getDeltaWidth( h )     (sqrt( Radius*Radius-(h)*(h) + 0.5))

#define COPY_PIX(__src, __dst, __pixcn)                                       \
  do {                                                                          \
    int __cn;                                                                   \
    for (__cn = 0; __cn < __pixcn; ++__cn)                                      \
      *(__src + __cn) = *(__dst + __cn);                                        \
  } while(0)

//double m_Rate;//球心到图像圆心的距离比上球的半径的值
int Spherize_trans(int height, int width, unsigned char* src, int src_step, int src_cn,
    int x, int y, int R2, double m_Rate)
{
  int Radius = 0;
  IPAIR* pTable = 0;
  int i, top, bottom;

  if (Radius == 0) {
    Radius = ((int)(R2 + 0.5));
    pTable = Spherize_generateTable(pTable, Radius, m_Rate);
  }
  else {
    double drate = (double) Radius / R2;

    if (drate < 0.81 || drate > 1.21) {   //0.81与1.21应该以参数的形式保存起来哦
      Radius = ((int)((Radius + sqrt(R2)) / 2 + 0.5));
      pTable = Spherize_generateTable(pTable, Radius, m_Rate);
    }
  }

  top = y - Radius;
  top = top < 0 ? 0 : top;
  bottom = y + Radius;
  bottom = bottom > height - 1 ? height - 1 : bottom;

  for (; top < y; top++) {
    int length = (int) getDeltaWidth(y - top);
    int left = (int)((x - length)) < 0 ? 0 : (x - length);
    int right = (x + length) > width - 1 ? width - 1 : (x + length);

    for (; left < x; left++) {
      IPAIR p = getMapDeltaXY(y - top, x - left);
      int tx = x - p.first;
      int ty = y - p.second;

      for (i = 0; i < src_cn; i++) {
        src[ top * src_step + (left) * src_cn + i ] = src[ ty * src_step + (tx) * src_cn + i ];
      }
    }

    for (; right >= x; right--) {
      IPAIR p = getMapDeltaXY(y - top, right - x);
      int tx = x + p.first;
      int ty = y - p.second;

      for (i = 0; i < src_cn; i++) {
        src[ top * src_step + (right) * src_cn + i ] = src[ ty * src_step + (tx) * src_cn + i ];
      }
    }
  }

  for (; bottom >= y; bottom--) {
    int length = (int) getDeltaWidth(bottom - y);
    int left = (int)((x - length)) < 0 ? 0 : (x - length);
    int right = (x + length) > width - 1 ? width - 1 : (x + length);

    for (; left <= x; left++) {
      IPAIR p = getMapDeltaXY(bottom - y, x - left);
      int tx = x - p.first;
      int ty = y + p.second;

      for (i = 0; i < src_cn; i++) {
        src[ bottom * src_step + (left) * src_cn + i ] = src[ ty * src_step + (tx) * src_cn + i ];
      }
    }

    for (; right > x; right--) {
      IPAIR p = getMapDeltaXY(bottom - y, right - x);
      int tx = x + p.first;
      int ty = y + p.second;

      for (i = 0; i < src_cn; i++) {
        src[ bottom * src_step + (right) * src_cn + i ] = src[ ty * src_step + (tx) * src_cn + i ];
      }
    }
  }

  if (pTable) {
    pfree(pTable);
    pTable = NULL;
  }

  return 1;
}

#define getDeltaX(nR, nY ) sqrt( (nR) * (nR) - (nY) * (nY) )

int UnSpherize_trans(int height, int width, unsigned char* src, int src_step, int src_cn,
    int x, int y, int nRadius, double m_dRate)
{
  double R2;
  int i, j, k, topMost, nMax, bottomMost, rightMost, newY;
  char* ptmp;
  ptmp = (unsigned char*) pmalloc(width * height * src_cn);
  nMax = width < height ? width / 2 : height / 2;
  memcpy(ptmp, src, width * height * src_cn);
  nRadius = nRadius > nMax ? nMax : nRadius;
  R2 = nRadius * nRadius / (1 - 1.0 / (m_dRate * m_dRate));
  topMost = y - nRadius;

  if (topMost < 0) {
    topMost = 0;
  }

  bottomMost = y + nRadius;

  if (bottomMost >= height) {
    bottomMost = height - 1;
  }

  for (i = topMost; i <= bottomMost; i++) {
    int nDeltaX = (int) getDeltaX(nRadius, y - i);
    int leftMost = x - nDeltaX;

    if (leftMost < 0) {
      leftMost = 0;
    }

    rightMost = x + nDeltaX;

    if (rightMost >= width) {
      rightMost = width - 1;
    }

    for (j = leftMost; j <= rightMost; j++) {
      double dtmp = (j - x) * (j - x) + (i - y) * (i - y);
      double r = sqrt(dtmp);
      double d2 = R2 / (m_dRate * m_dRate);
      double r1 = r * sqrt(R2 / (d2 + dtmp));
      int newX = (int)(0.5 + r1 * (j - (int) x) / r + (int) x);

      if (newX < 0) {
        newX = 0;
      }
      else if (newX >= width) {
        newX = width - 1;
      }

      newY = (int)(0.5 + r1 * (i - (int) y) / r + (int) y);

      if (newY < 0) {
        newY = 0;
      }
      else if (newY >= height) {
        newY = height - 1;
      }

      for (k = 0; k < src_cn; k++) {
        ptmp[(i * width + j) * src_cn + k ] = src[(newY * width + newX) * src_cn + k ];
      }
    }
  }

  memcpy(src, ptmp, width * height * src_cn);
  pfree(ptmp);
  return 1;
}


