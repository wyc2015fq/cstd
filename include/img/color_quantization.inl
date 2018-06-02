
//#include "sort.h"

typedef struct SOctant {
  int m_iChd; // 孩子数
  int m_iCnt; //记录通过该节点颜色的总数（记录该颜色出现的次数）
  int m_iPix[3];
  int m_iIdx;
} SOctant;
#define pack_color(pix, k)  ( (((pix[2] >> (7 - k)) & 1) << 2) | (((pix[1] >> (7 - k)) & 1) << 1) | ((pix[0] >> (7 - k)) & 1) )
#define CLIP256(a)  ((a)<0?0:((a)>255?255:(a)))

// 颜色量化
int color_quantization(int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl, int bi, const PaletteEntry* inpal, int inpalLen, PaletteEntry* outpal)
{
  int i, j, k, m, n, tt, outpalLen = 0;
  enum {level = 4, buflen = 1 << (3 * level)};

  if (inpal) {
    int col[256];
    int idx[256];
    ASSERT(inpalLen <= 256);

    for (i = 0; i < inpalLen; ++i) {
      int pix[3];
      pix[0] = inpal[i].b, pix[1] = inpal[i].g, pix[2] = inpal[i].r;

      for (m = 0, k = 0; k < 8; k++) {
        m = (m << 3) | pack_color(pix, k);
      }

      col[i] = m;
      idx[i] = i;
    }

#define LESSPTR1(a, b)   (col[(a)] < col[(b)])
    QSORT(idx, idx + inpalLen, LESSPTR1, int);
#undef LESSPTR1

    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        const unsigned char* pixA = A + i * al + j * ai;
        unsigned char* pix = B + i * bl + j * bi;

        for (m = 0, k = 0; k < 8; k++) {
          m = (m << 3) | pack_color(pixA, k);
        }

        for (k = 0, n = inpalLen - 1; 1;) {
          if (k < n - 1) {
            int t = (k + n) >> 1;

            if (col[idx[t]] < m) {
              k = t;
            }
            else {
              n = t;
            }
          }
          else {
            break;
          }
        }

        pix[0] = inpal[idx[k]].b;
        pix[1] = inpal[idx[k]].g;
        pix[2] = inpal[idx[k]].r;
      }
    }
  }
  else {
    SOctant oct[buflen + (buflen >> 2)];
    SOctant* octlev[level];
    SOctant* poct;
    int octLen[level];
    int m_iNumColors[level] = {0};
    memset(oct, 0, sizeof(oct));

    for (i = 0; i < countof(oct); ++i) {
      oct[i].m_iIdx = -1;
    }

    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        const unsigned char* pixA = A + i * al + j * ai;

        for (m = 0, k = 0; k < level; k++) {
          m = (m << 3) | pack_color(pixA, k);
        }

        oct[m].m_iCnt++;
        oct[m].m_iIdx = 0;
        oct[m].m_iPix[0] += pixA[0];
        oct[m].m_iPix[1] += pixA[1];
        oct[m].m_iPix[2] += pixA[2];
      }
    }

    poct = oct;
    octlev[level - 1] = poct;
    octLen[level - 1] = 1 << (3 * level);

    for (i = level - 1; i > 0; --i) {
      poct += octLen[i];
      octlev[i - 1] = poct;
      octLen[i - 1] = 1 << (3 * i);

      for (j = 0; j < octLen[i]; ++j) {
        if (octlev[i][j].m_iCnt) {
          octlev[i - 1][j >> 3].m_iChd++;
          octlev[i - 1][j >> 3].m_iIdx = 0;
          octlev[i - 1][j >> 3].m_iCnt += octlev[i][j].m_iCnt;
          octlev[i - 1][j >> 3].m_iPix[0] += octlev[i][j].m_iPix[0];
          octlev[i - 1][j >> 3].m_iPix[1] += octlev[i][j].m_iPix[1];
          octlev[i - 1][j >> 3].m_iPix[2] += octlev[i][j].m_iPix[2];
        }
      }
    }

    for (i = 0; i < level; ++i) {
      for (j = 0; j < octLen[i]; ++j) {
        m_iNumColors[i] += !!octlev[i][j].m_iCnt;
      }

      if (m_iNumColors[i] > 255) {
        n = i;
        break;
      }
    }

    if (1) {
      int idx[buflen];

      for (i = 0; i < octLen[n - 1]; ++i) {
        idx[i] = i;
      }

#define LESSPTR1(a, b)   (octlev[n-1][(a)].m_iChd == octlev[n-1][(b)].m_iChd ? (octlev[n-1][(a)].m_iCnt < octlev[n-1][(b)].m_iCnt) : (octlev[n-1][(a)].m_iChd > octlev[n-1][(b)].m_iChd))
      QSORT(idx, idx + octLen[n - 1], LESSPTR1, int);
#undef LESSPTR1

      for (i = 0; i < octLen[n - 1] && m_iNumColors[n] > 255; ++i) {
        if (octlev[n - 1][idx[i]].m_iCnt > 0) {
          ASSERT(octlev[n - 1][idx[i]].m_iChd > 0);
          m_iNumColors[n] -= octlev[n - 1][idx[i]].m_iChd - 1;

          for (j = 0; j < 8; ++j) {
            octlev[n][(idx[i] << 3) + j].m_iIdx = -1;
          }
        }
      }
    }
    else {
      --n;
    }

    // 建立索引
    for (i = 0, m = 0; m < octLen[n]; ++m) {
      if (octlev[n][m].m_iCnt > 0) {
        if (octlev[n][m].m_iIdx == 0) {
          octlev[n][m].m_iIdx = ++i;
        }
        else if (octlev[n - 1][m >> 3].m_iIdx == 0) {
          octlev[n - 1][m >> 3].m_iIdx = ++i;
        }
      }
    }

    outpalLen = i;
    ASSERT(i == outpalLen);

    if (outpal) {
      for (m = 0; m < octLen[n]; ++m) {
        if (octlev[n][m].m_iCnt > 0) {
          if (octlev[n][m].m_iIdx > 0) {
            i = octlev[n][m].m_iIdx - 1;
            tt = octlev[n][m].m_iCnt;
            outpal[i].b = octlev[n][m].m_iPix[0] / tt;
            outpal[i].g = octlev[n][m].m_iPix[1] / tt;
            outpal[i].r  = octlev[n][m].m_iPix[2] / tt;
          }
          else if (octlev[n - 1][m >> 3].m_iIdx > 0) {
            i = octlev[n - 1][m >> 3].m_iIdx - 1;
            tt = octlev[n - 1][m >> 3].m_iCnt;
            outpal[i].b = octlev[n - 1][m >> 3].m_iPix[0] / tt;
            outpal[i].g = octlev[n - 1][m >> 3].m_iPix[1] / tt;
            outpal[i].r  = octlev[n - 1][m >> 3].m_iPix[2] / tt;
          }
        }
      }

      //ASSERT(i==outpalLen);
    }

    poct = octlev[n];

    if (bi >= 3) {
      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          const unsigned char* pixA = A + i * al + j * ai;
          unsigned char* pix = B + i * bl + j * bi;

          for (m = 0, k = 0; k < n + 1; k++) {
            m = (m << 3) | pack_color(pixA, k);
          }

          ASSERT(m < octLen[n]);

          if (octlev[n][m].m_iIdx >= 0) {
            tt = octlev[n][m].m_iCnt;
            pix[0] = octlev[n][m].m_iPix[0] / tt;
            pix[1] = octlev[n][m].m_iPix[1] / tt;
            pix[2] = octlev[n][m].m_iPix[2] / tt;
          }
          else {
            tt = octlev[n - 1][m >> 3].m_iCnt;
            ASSERT(tt > 0);
            pix[0] = octlev[n - 1][m >> 3].m_iPix[0] / tt;
            pix[1] = octlev[n - 1][m >> 3].m_iPix[1] / tt;
            pix[2] = octlev[n - 1][m >> 3].m_iPix[2] / tt;
          }
        }
      }
    }
    else {
      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          const unsigned char* pixA = A + i * al + j * ai;
          unsigned char* pix = B + i * bl + j * bi;

          for (m = 0, k = 0; k < n + 1; k++) {
            m = (m << 3) | pack_color(pixA, k);
          }

          ASSERT(m < octLen[n]);

          if (octlev[n][m].m_iIdx > 0) {
            pix[0] = octlev[n][m].m_iIdx - 1;
          }
          else {
            //ASSERT(octlev[n-1][m>>3].m_iIdx>0);
            pix[0] = octlev[n - 1][m >> 3].m_iIdx - 1;
          }
        }
      }
    }
  }

  return 0;
}
