
#include <stdio.h>
#include "cstd.h"

typedef IPOINT IPOINT;

// Functions for mirroring points...
IPOINT PointMirror(IPOINT P, IPOINT M)
{
  return iPOINT(P.x + 2 * (M.x - P.x), P.y + 2 * (M.y - P.y));
}

IPOINT LineMirrorX(IPOINT P, int X)
{
  return iPOINT(P.x + 2 * (X - P.x), P.y);
}

IPOINT LineMirrorY(IPOINT P, int Y)
{
  return iPOINT(P.x, P.y + 2 * (Y - P.y));
}

enum {
  DS_SMOOTH = 1,    // Pioneer kind of characters
  DS_STYLE14 = 2,   // use allways 14 segment display.
  DS_SZ_PROP = 4,   // size proportional(default)
  DS_NO_OFF = 8,    // Don't draw the off segments
  DS_SOFT = 16,     // ambient bleeding to background
};

enum {
  DS_STYLE_3,
  DS_STYLE_7,
  DS_STYLE_14,
};
typedef unsigned short WORD;
typedef unsigned char BYTE;

#define MAXSEGCHAR7        12        // Number of supported 7 segment characters
#define MAXSEGCHAR14    45        // Number of supported 14 segment characters
#define MAXSEGSEMCOL     2        // Number of supported 3 segment characters
#define NORM_DIGIHEIGHT    83        // All characters must have this height

// -----          13         -----          0
//|\ | /|      8  0  12     |     |      1     2
//| \|/ |        1 2        |     |
// -- --   ==    6 7         -----   ==     3
//| /|\ |        3 4        |     |
//|/ | \|      9  5  11     |     |      4     5
// -----          10         -----          6

///////////////////////////////////////////////////////////////////////////////
// For 7 segments display...
//                                 SP    0     1      2    3     4     5
BYTE CHAR_SEGM7[ MAXSEGCHAR7 ] = {0x00, 0x77, 0x24, 0x5D, 0x6D, 0x2E, 0x6B,
    // 6     7     8     9     -
    0x7B, 0x25, 0x7F, 0x6F, 0x08
                                 };
// straight style
IPOINT PtSeg7N0[] = {{ 5, 4}, {12, 11}, {36, 11}, {43, 4}};
IPOINT PtSeg7N1[] = {{ 4, 6}, { 4, 40}, {11, 36}, {11, 13}};
IPOINT PtSeg7N3[] = {{ 6, 41}, {14, 45}, {34, 45}, {42, 41}, {34, 37}, {14, 37}}; // 3
int PtSegCnt[][ 14 ] = {{4, 4, 4}, {4, 4, 6}, {5, 4, 6, 4, 4}};

////////////////////////////////////////////////////////////////////////////////
// For 14 segments display...
//                                       SP        0        1        2        3        4        5        6
WORD CHAR_SEGM14[ MAXSEGCHAR14 ] = {0x0000, 0x3F00, 0x1800, 0x36C0, 0x3CC0, 0x19C0, 0x2DC0, 0x2FC0,
    //        7        8        9        -        A        B        C        D        E        F        G        H
    0x3800, 0x3FC0, 0x3DC0, 0x00C0, 0x3BC0, 0x3CA1, 0x2700, 0x3C21, 0x27C0, 0x23C0, 0x2F80, 0x1BC0,
    //        I        J        K        L        M        N        O        P        Q        R        S        T
    0x2421, 0x1E00, 0x0354, 0x0700, 0x1B06, 0x1B12, 0x3F00, 0x33C0, 0x3F10, 0x33D0, 0x2DC0, 0x2021,
    //        U        V        W        X        Y        Z        *        +        (        )    Back/        /
    0x1F00, 0x030C, 0x1B18, 0x001E, 0x11E0, 0x240C, 0x00FF, 0x00E1, 0x0014, 0x000A, 0x0012, 0x000C,
    //        '
    0x0001
                                   };
// straight style
IPOINT PtSeg14N0[ 5 ] = {{20, 13}, {20, 36}, {24, 40}, {28, 36}, {28, 13}};
IPOINT PtSeg14N1[ 4 ] = {{ 5, 5}, {15, 35}, {20, 37}, {18, 25}};
IPOINT PtSeg14N6[ 6 ] = {{ 6, 41}, {14, 45}, {18, 45}, {22, 41}, {18, 37}, {14, 37}};
IPOINT PtSeg14N8[ 4 ] = {{ 4, 7}, { 4, 40}, {11, 36}, {11, 26}};
IPOINT PtSeg14N13[ 4 ] = {{ 6, 4}, {11, 11}, {37, 11}, {42, 4}};

// For 3 segments semicoloncombi display...
//                                      .        :
BYTE CHAR_SEGM3[ MAXSEGSEMCOL ] = {0x04, 0x03};

#ifdef _WIDE_SEMICOLON
// for wide semicolon character space
IPOINT PtSegScN0[ 4 ] = {{ 19, 23}, { 19, 32}, {32, 32}, {32, 23}};
IPOINT PtSegScN1[ 4 ] = {{ 19, 50}, { 19, 59}, {32, 59}, {32, 50}};
IPOINT PtSegScN2[ 4 ] = {{ 19, 68}, { 19, 77}, {32, 77}, {32, 68}};
#else
// small semicolon character space
IPOINT PtSegScN0[ 4 ] = {{ 4, 23}, { 4, 32}, {13, 32}, {13, 23}};
IPOINT PtSegScN1[ 4 ] = {{ 4, 50}, { 4, 59}, {13, 59}, {13, 50}};
IPOINT PtSegScN2[ 4 ] = {{ 4, 68}, { 4, 77}, {13, 77}, {13, 68}};
#endif

int DefineChar(TCHAR cChar, int* pDispStyle)
{
  int iIndex;
  int pDChar = 0;
  int m_DispStyle = 0;

#define SetElementData(A)  (pDChar = CHAR_SEGM##A[iIndex], m_DispStyle = DS_STYLE_##A)

  if (cChar >= _T('0') && cChar <= _T('9')
      || cChar == _T(' ') || cChar == _T('-')) {
    // these characters can be 7 or 14 segments...
    if (cChar == _T(' ')) {
      iIndex = 0;
    }
    else if (cChar == _T('-')) {
      iIndex = 11;
    }
    else {
      iIndex = cChar - _T('0') + 1;
    }

    if (m_DispStyle & DS_STYLE14) {
      SetElementData(14);
    }
    else {
      SetElementData(7);
    }
  }
  else if (cChar >= _T('A') && cChar <= _T('Z')) {
    // 14 segment only...
    iIndex = cChar - _T('A') + 12;
    SetElementData(14);
  }
  else {
    // Spcial characters...
    iIndex = 0;

    switch (cChar) {
    case _T(':') :
      iIndex++;

    case _T('.') :
      SetElementData(3);
      break;

    case _T('*') :
      iIndex = MAXSEGCHAR14 - 7;
      SetElementData(14);
      break;

    case _T('+') :
      iIndex = MAXSEGCHAR14 - 6;
      SetElementData(14);
      break;

    case _T('(') :
      iIndex = MAXSEGCHAR14 - 5;
      SetElementData(14);
      break;

    case _T(')') :
      iIndex = MAXSEGCHAR14 - 4;
      SetElementData(14);
      break;

    case _T('\\') :
      iIndex = MAXSEGCHAR14 - 3;
      SetElementData(14);
      break;

    case _T('/') :
      iIndex = MAXSEGCHAR14 - 2;
      SetElementData(14);
      break;

    case _T('\'') :
      iIndex = MAXSEGCHAR14 - 1;
      SetElementData(14);
      break;
      //default : ASSERT(FALSE);
    }
  }

  *pDispStyle = m_DispStyle;
  return pDChar;
}

int InitDigi3(IPOINT* pt, int i, int cy, int cx, int y, int x)
{
  int bb = DS_STYLE_3;
  int p;
  int m_Width = 49;
  int m_Hidth = 82;
  int kx = (cx << 14) / m_Width;
  int ky = (cy << 14) / m_Hidth;
  int nCount = 0;

  if (0 == ky) {
    ky = kx;
  }

  switch (i) {
#define FOR_CNT(_i, _DO) { nCount = PtSegCnt[bb][_i]; for (p = 0; p < nCount; p++) { pt[p] = _DO; pt[p].x=((pt[p].x * kx)>>14) + x; pt[p].y=((pt[p].y * ky)>>14) + y;} }

  case 0:
    FOR_CNT(0, PtSegScN0[ p ]) break;

  case 1:
    FOR_CNT(1, PtSegScN1[ p ]) break;

  case 2:
    FOR_CNT(2, PtSegScN2[ p ]) break;
  }

  return nCount;
}

int InitDigi7(IPOINT* pt, int i, int cy, int cx, int y, int x)
{
  int bb = DS_STYLE_7;
  int p;
  int m_Width = 49;
  int m_Hidth = 82;
  int kx = (cx << 14) / m_Width;
  int ky = (cy << 14) / m_Hidth;
  int nCount = 0;

  if (0 == ky) {
    ky = kx;
  }

  switch (i) {
#define FOR_CNT(_i, _DO) { nCount = PtSegCnt[bb][_i]; for (p = 0; p < nCount; p++) { pt[p] = _DO; pt[p].x=((pt[p].x * kx)>>14) + x; pt[p].y=((pt[p].y * ky)>>14) + y;} }

  case 0:
    FOR_CNT(0, PtSeg7N0[ p ]) break;

  case 1:
    FOR_CNT(1, PtSeg7N1[ p ]) break;

  case 2:
    FOR_CNT(1, LineMirrorX(PtSeg7N1[ p ], (m_Width - 1) / 2)) break;

  case 3:
    FOR_CNT(2, PtSeg7N3[ p ]) break;

  case 4:
    FOR_CNT(1, LineMirrorY(PtSeg7N1[ p ], 41)) break;

  case 5:
    FOR_CNT(1, PointMirror(PtSeg7N1[ p ], iPOINT((m_Width - 1) / 2, 41))) break;

  case 6:
    FOR_CNT(0, LineMirrorY(PtSeg7N0[ p ], 41)) break;
  }

  return nCount;
}

int InitDigi14(IPOINT* pt, int i, int cy, int cx, int y, int x)
{
  int bb = DS_STYLE_14;
  int p;
  int m_Width = 49;
  int m_Hidth = 82;
  int kx = (cx << 14) / m_Width;
  int ky = (cy << 14) / m_Hidth;
  int nCount = 0;

  if (0 == ky) {
    ky = kx;
  }

  switch (i) {
#define FOR_CNT(_i, _DO) { nCount = PtSegCnt[bb][_i]; for (p = 0; p < nCount; p++) { pt[p] = _DO; pt[p].x=((pt[p].x * kx)>>14) + x; pt[p].y=((pt[p].y * ky)>>14) + y;} }

  case 0:
    FOR_CNT(0, PtSeg14N0[ p ]) break;

  case 1:
    FOR_CNT(1, PtSeg14N1[ p ]) break;

  case 2:
    FOR_CNT(1, LineMirrorX(PtSeg14N1[ p ], (m_Width - 1) / 2)) break;

  case 3:
    FOR_CNT(1, LineMirrorY(PtSeg14N1[ p ], 41)) break;

  case 4:
    FOR_CNT(1, PointMirror(PtSeg14N1[ p ], iPOINT((m_Width - 1) / 2, 41))) break;

  case 5:
    FOR_CNT(0, LineMirrorY(PtSeg14N0[ p ], 41)) break;

  case 6:
    FOR_CNT(2, PtSeg14N6[ p ]) break;

  case 7:
    FOR_CNT(2, LineMirrorX(PtSeg14N6[ p ], (m_Width - 1) / 2)) break;

  case 8:
    FOR_CNT(3, PtSeg14N8[ p ]) break;

  case 9:
    FOR_CNT(3, LineMirrorY(PtSeg14N8[ p ], 41)) break;

  case 10:
    FOR_CNT(4, LineMirrorY(PtSeg14N13[ p ], 41)) break;

  case 11:
    FOR_CNT(3, PointMirror(PtSeg14N8[ p ], iPOINT((m_Width - 1) / 2, 41))) break;

  case 12:
    FOR_CNT(3, LineMirrorX(PtSeg14N8[ p ], (m_Width - 1) / 2)) break;

  case 13:
    FOR_CNT(4, PtSeg14N13[ p ]) break;
  }

  return nCount;
}

// bgmix ±³¾°ÏÔÊ¾±ÈÀý
void imdraw_digi(int h, int w, unsigned char* ptr, int step, int pix_size,
    const char* str, DWORD color,
    int cy, int cx, int y, int x, int fill, double bgmix)
{
  int i, nCount;
  int line_type = CC_AA;
  IPOINT pt[ 6 ];
  DWORD bg = color;

  if (bgmix > 0.001) {
    bg = RGBMUL(bg, bgmix);
  }

  for (; str && *str; ++str, x += cx) {
    int aa, bb;
    nCount = 0;
    aa = DefineChar(*str, &bb);

    for (i = 0; i < 32; ++i) {
      DWORD col = color;
      nCount = 0;

      if (0 == (aa & (1 << i))) {
        if (bgmix < 0.001) {
          continue;
        }
        else {
          col = bg;
        }
      }

      if (DS_STYLE_3 == bb) {
        nCount = InitDigi3(pt, i, cy, cx, y, x);
      }

      if (DS_STYLE_7 == bb) {
        nCount = InitDigi7(pt, i, cy, cx, y, x);
      }

      if (DS_STYLE_14 == bb) {
        nCount = InitDigi14(pt, i, cy, cx, y, x);
      }

      if (!fill) {
        IPOINT* pp = pt;
        int thickness = MAX((cx / 140), 1);
        imdraw_poly(h, w, ptr, step, pix_size, &pp, &nCount, 1, 1,
            col, 3, line_type, 0);
        //FillPolyM( height, width, image, step, cn, pt, arr, 3,
        //           random_color(&rng), line_type, 0 );
      }
      else {
        FillConvexPoly(h, w, ptr, step, pix_size,
            pt, nCount, col, line_type, 0);
      }
    }
  }
}
