
#include <stdlib.h>
#include <assert.h>
#include "macro.h"


/* 1/16[1    4    6    4    1]       */
/* 6538 3966 885 73 */
void gauss_filter(int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl, int bi, int win)
{
  int i, j, k;
  //int win=2;
  const unsigned char* line1[ 7 ];
  const unsigned char** line = line1 + win;
  int* buf = MALLOC(int, (w + 2 * win) * ai);
  int* row = buf + win * ai;
  int ai2 = ai + ai, ai3 = ai + ai + ai;
  assert(2 == win || 3 == win);

  for (i = 0; i < h; ++i, B += bl) {
    for (j = 0; j < (win * 2 + 1); ++j) {
      k = i + j - win;
      k = MIN(MAX(k, 0), (h - 1));
      line1[ j ] = A + k * al;
    }

    for (j = 0; j < w * ai; ++j) {
      if (2 == win) {
        row[ j ] = 6 * line[ 0 ][ j ] + 4 * (line[ -1 ][ j ] + line[ 1 ][ j ]) + (line[ -2 ][ j ] + line[ 2 ][ j ]);
      }
      else if (3 == win) {
        row[ j ] = (6538 * line[ 0 ][ j ] + 3966 * (line[ -1 ][ j ] + line[ 1 ][ j ]) + 885 * (line[ -2 ][ j ] + line[ 2 ][ j ]) + 73 * (line[ -3 ][ j ] + line[ 3 ][ j ])) >> 14;
      }
    }

    for (k = 0; k < bi; ++k) {
      int* rowk = row + k;
      unsigned char* Bk = B + k;

      for (j = 1; j <= win; ++j) {
        rowk[ -j * ai ] = rowk[ 0 ];
        rowk[(w - 1 + j) * ai ] = rowk[(w - 1) * ai ];
      }

      for (j = 0; j < w; ++j) {
        int* rowkj = rowk + j * ai;

        if (2 == win) {
          Bk[ j * bi ] = (6 * rowkj[ 0 ] + 4 * (rowkj[ -ai ] + rowkj[ ai ]) + rowkj[ -ai2 ] + rowkj[ ai2 ]) >> 8;
        }
        else if (3 == win) {
          Bk[ j * bi ] = (6538 * rowkj[ 0 ] + 3966 * (rowkj[ -ai ] + rowkj[ ai ]) + 885 * (rowkj[ -ai2 ] + rowkj[ ai2 ]) + 73 * (rowkj[ -ai3 ] + rowkj[ ai3 ])) >> 14;
        }
      }
    }
  }

  pfree(buf);
}

int bilateral_table[ 256 ] = {6, 4, 1, 0, };

#if 1
#define BILATERAL_IF(pix, pixmin, pixmax, out, outidx, idxval)                \
  if ((pixmin)<=(pix) && (pix)<(pixmax)) {                                      \
    (out)+=(idxval)*(pix); (outidx)+=(idxval);                                  \
  }
#else
#define BILATERAL_IF(pix, pixmin, pixmax, out, outidx, idxval)                \
  do {                                                                          \
    int fff = (pix>pixcur)?(pix-pixcur):(pixcur-pix);                           \
    int ggg = (idxval)*bilateral_table[fff>>2];                                 \
    (out)+=ggg*(pix);                                                           \
    (outidx)+=ggg;                                                              \
  } while(0)
#endif


void bilateral_filter(int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl, int bi, int win, int delta)
{
  int i, j, k;
  const unsigned char* line1[ 64 ];
  const unsigned char** line = line1 + win;
  int ai2 = ai + ai, ai3 = ai + ai + ai;
  int* buf = MALLOC(int, (w + 2 * win) * ai);
  int* row = buf + win * ai;
  assert(32 > win);

  for (i = 0; i < h; ++i, B += bl) {
    for (j = 0; j < (win * 2 + 1); ++j) {
      k = i + j - win;
      k = MIN(MAX(k, 0), (h - 1));
      line1[ j ] = A + k * al;
    }

    for (j = 0; j < w * ai; ++j) {
      int pixcur = line[ 0 ][ j ], pixmin = line[ 0 ][ j ] - delta, pixmax = line[ 0 ][ j ] + delta, out, outidx;

      if (1 == win) {
        outidx = 2;
        out = 2 * line[ 0 ][ j ];
        BILATERAL_IF(line[ -1 ][ j ], pixmin, pixmax, out, outidx, 1);
        BILATERAL_IF(line[ 1 ][ j ], pixmin, pixmax, out, outidx, 1);
        row[ j ] = out / outidx;
      }
      else if (2 == win) {
        outidx = 6;
        out = 6 * line[ 0 ][ j ];
        BILATERAL_IF(line[ -1 ][ j ], pixmin, pixmax, out, outidx, 4);
        BILATERAL_IF(line[ 1 ][ j ], pixmin, pixmax, out, outidx, 4);
        BILATERAL_IF(line[ -2 ][ j ], pixmin, pixmax, out, outidx, 1);
        BILATERAL_IF(line[ 2 ][ j ], pixmin, pixmax, out, outidx, 1);
        row[ j ] = out / outidx;
      }
      else if (3 <= win) {
        outidx = 6538;
        out = 6538 * line[ 0 ][ j ];
        BILATERAL_IF(line[ -1 ][ j ], pixmin, pixmax, out, outidx, 3966);
        BILATERAL_IF(line[ 1 ][ j ], pixmin, pixmax, out, outidx, 3966);
        BILATERAL_IF(line[ -2 ][ j ], pixmin, pixmax, out, outidx, 885);
        BILATERAL_IF(line[ 2 ][ j ], pixmin, pixmax, out, outidx, 885);

        for (k = 3; k < win; ++k) {
          BILATERAL_IF(line[ -k ][ j ], pixmin, pixmax, out, outidx, 73);
          BILATERAL_IF(line[ k ][ j ], pixmin, pixmax, out, outidx, 73);
        }

        row[ j ] = out / outidx;
      }
    }

    for (k = 0; k < bi; ++k) {
      int* rowk = row + k;
      unsigned char* Bk = B + k;

      for (j = 1; j <= win; ++j) {
        rowk[ -j * ai ] = rowk[ 0 ];
        rowk[(w - 1 + j) * ai ] = rowk[(w - 1) * ai ];
      }

      for (j = 0; j < w; ++j) {
        int* rowkj = rowk + j * ai;
        int pixcur = line[ 0 ][ j ], pixmin = rowkj[ 0 ] - delta, pixmax = rowkj[ 0 ] + delta, out, outidx;

        if (1 == win) {
          outidx = 2;
          out = 2 * rowkj[ 0 ];
          BILATERAL_IF(rowkj[ -ai ], pixmin, pixmax, out, outidx, 1);
          BILATERAL_IF(rowkj[ ai ], pixmin, pixmax, out, outidx, 1);
          Bk[ j * bi ] = out / outidx;
        }
        else if (2 == win) {
          outidx = 6;
          out = 6 * rowkj[ 0 ];
          BILATERAL_IF(rowkj[ -ai ], pixmin, pixmax, out, outidx, 4);
          BILATERAL_IF(rowkj[ ai ], pixmin, pixmax, out, outidx, 4);
          BILATERAL_IF(rowkj[ -ai2 ], pixmin, pixmax, out, outidx, 1);
          BILATERAL_IF(rowkj[ ai2 ], pixmin, pixmax, out, outidx, 1);
          Bk[ j * bi ] = out / outidx;
        }
        else if (3 <= win) {
          outidx = 6538;
          out = 6538 * rowkj[ 0 ];
          BILATERAL_IF(rowkj[ -ai ], pixmin, pixmax, out, outidx, 3966);
          BILATERAL_IF(rowkj[ ai ], pixmin, pixmax, out, outidx, 3966);
          BILATERAL_IF(rowkj[ -ai2 ], pixmin, pixmax, out, outidx, 885);
          BILATERAL_IF(rowkj[ ai2 ], pixmin, pixmax, out, outidx, 885);

          for (k = 3; k < win; ++k) {
            int aik = ai * k;
            BILATERAL_IF(rowkj[ -aik ], pixmin, pixmax, out, outidx, 73);
            BILATERAL_IF(rowkj[ aik ], pixmin, pixmax, out, outidx, 73);
          }

          Bk[ j * bi ] = out / outidx;
        }
      }
    }
  }

  pfree(buf);
}
