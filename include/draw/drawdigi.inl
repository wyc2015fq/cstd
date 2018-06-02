enum {
  DS_SMOOTH = 1, // Pioneer kind of characters
  DS_STYLE14 = 2, // use allways 14 segment display.
  DS_SZ_PROP = 4, // size proportional(default)
  DS_NO_OFF = 8, // Don't draw the off segments
  DS_SOFT = 16 // ambient bleeding to background
};
// Segment numbering:
// ----- 13 ----- 0
//|\ | /| 8 0 12 | | 1 2
//| \|/ | 1 2 | |
// -- -- == 6 7 ----- == 3
//| /|\ | 3 4 | |
//|/ | \| 9 5 11 | | 4 5
// ----- 10 ----- 6
#define MAXSEGCHAR7 12 // Number of supported 7 segment characters
#define MAXSEGCHAR14 45 // Number of supported 14 segment characters
#define MAXSEGSEMCOL 2 // Number of supported 3 segment characters
#define NORM_DIGIHEIGHT 83 // All characters must have this height
// please uncomment the following line if you want nicer scrolling
// and all characters get same width
//#define _WIDE_SEMICOLON
// Functions for mirroring points...
DPOINT PointMirror(const DPOINT* P, double x, double y)
{
  return dPOINT(P->x + 2 * (x - P->x), P->y + 2 * (y - P->y));
}
DPOINT LineMirrorX(const DPOINT* P, int X)
{
  return dPOINT(P->x + 2 * (X - P->x), P->y);
}
DPOINT LineMirrorY(const DPOINT* P, int Y)
{
  return dPOINT(P->x, P->y + 2 * (Y - P->y));
}
int Digi7Segment(int iIndex, int iDispStyle, DPOINT* pt, int* lens)
{
  // For 7 segments display... SP 0 1 2 3 4 5 6 7 8 9 -
  static const BYTE CHAR_SEGM7[MAXSEGCHAR7] = {0x00, 0x77, 0x24, 0x5D, 0x6D, 0x2E, 0x6B, 0x7B, 0x25, 0x7F, 0x6F, 0x08};
  // straight style
  static const DPOINT PtSeg7N0[4] = { 5, 4, 12, 11, 36, 11, 43, 4};
  static const DPOINT PtSeg7N1[4] = { 4, 6, 4, 40, 11, 36, 11, 13};
  static const DPOINT PtSeg7N3[6] = { 6, 41, 14, 45, 34, 45, 42, 41, 34, 37, 14, 37}; // 3
  static const DPOINT* PtSeg7N[] = {PtSeg7N0, PtSeg7N1, PtSeg7N1, PtSeg7N3};
  static const int ptlen1[] = {4, 4, 4, 6};
  // smooth style PT_BEZIERTO
  static const DPOINT PtSeg7N0S[7] = { 6, 4, 5, 5, 5, 6, 8, 9, 12, 11, 36, 11, 39, 4};
  static const DPOINT PtSeg7N1S[7] = { 4, 9, 4, 39, 6, 40, 7, 40, 9, 38, 11, 36, 11, 12};
  static const DPOINT PtSeg7N2S[10] = {37, 36, 39, 38, 41, 40, 42, 40, 44, 39, 44, 6, 42, 4, 41, 4, 39, 8, 37, 12};
  static const DPOINT PtSeg7N3S[6] = { 8, 41, 12, 45, 36, 45, 40, 41, 36, 37, 12, 37};
  static const DPOINT* PtSeg7NS[] = {PtSeg7N0S, PtSeg7N1S, PtSeg7N2S, PtSeg7N3S};
  static const int ptlen2[] = {7, 7, 10, 6};
  static const int id1[] = {0, 1, 2, 3, 1, 2, 0};
  WORD wSegmData = CHAR_SEGM7[iIndex];
  int i, p, j = 0;
  DPOINT* pt0 = pt;
  const DPOINT* pts = NULL;
  const DPOINT** ppts = (iDispStyle & DS_SMOOTH) ? PtSeg7NS : PtSeg7N;
  const int* ptlens = (iDispStyle & DS_SMOOTH) ? ptlen2 : ptlen1;
  int nCount = 0;
  int m_Width = 49;
  int m_NSegments = 7;

  for (i = 0; i < m_NSegments; i++) {
    if ((wSegmData & (1 << i)) == 0) {
      continue;
    }

    // Find data for segment in correct style...
    pts = ppts[id1[i]];
    nCount = ptlens[id1[i]];

    // For nondefined segments use mirroring...
    switch (i) {
    case 6:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorY(&pts[p], 41);
      }

      break;

    case 2:
      if (!(iDispStyle & DS_SMOOTH)) {
        for (p = 0; p < nCount; p++) {
          pt0[p] = LineMirrorX(&pts[p], (m_Width - 1) / 2);
        }
      }
      else {
        for (p = 0; p < nCount; p++) {
          pt0[p] = pts[p];
        }
      }

      break;

    case 4:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorY(&pts[p], 41);
      }

      break;

    case 5:
      for (p = 0; p < nCount; p++)
        if (iDispStyle & DS_SMOOTH) {
          pt0[p] = LineMirrorY(&pts[p], 41);
        }
        else {
          pt0[p] = PointMirror(&pts[p], (m_Width - 1) / 2, 41);
        }

      break;

    default:
      for (p = 0; p < nCount; p++) {
        pt0[p] = pts[p];
      }

      break;
    }

    pt0 += nCount;
    lens[j++] = nCount;
  }

  return j;
}
int Digi14Segment(int iIndex, int iDispStyle, DPOINT* pt, int* lens)
{
  ////////////////////////////////////////////////////////////////////////////////
  // For 14 segments display... SP 0 1 2 3 4 5 6
  static const WORD CHAR_SEGM14[MAXSEGCHAR14] = {0x0000, 0x3F00, 0x1800, 0x36C0, 0x3CC0, 0x19C0, 0x2DC0, 0x2FC0,
      //7 8 9 - A B C D E F G H
      0x3800, 0x3FC0, 0x3DC0, 0x00C0, 0x3BC0, 0x3CA1, 0x2700, 0x3C21, 0x27C0, 0x23C0, 0x2F80, 0x1BC0,
      //I J K L M N O P Q R S T
      0x2421, 0x1E00, 0x0354, 0x0700, 0x1B06, 0x1B12, 0x3F00, 0x33C0, 0x3F10, 0x33D0, 0x2DC0, 0x2021,
      //U V W X Y Z * + ( ) Back/ /
      0x1F00, 0x030C, 0x1B18, 0x001E, 0x11E0, 0x240C, 0x00FF, 0x00E1, 0x0014, 0x000A, 0x0012, 0x000C,
      // '
      0x0001
                                                };
  // straight style
  static const DPOINT PtSeg14N0[5] = {20, 13, 20, 36, 24, 40, 28, 36, 28, 13};
  static const DPOINT PtSeg14N1[4] = { 5, 5, 15, 35, 20, 37, 18, 25};
  static const DPOINT PtSeg14N6[6] = { 6, 41, 14, 45, 18, 45, 22, 41, 18, 37, 14, 37};
  static const DPOINT PtSeg14N8[4] = { 4, 7, 4, 40, 11, 36, 11, 26};
  static const DPOINT PtSeg14N13[4] = { 6, 4, 11, 11, 37, 11, 42, 4};
  static const DPOINT* Pteg14N[] = {PtSeg14N0, PtSeg14N1, PtSeg14N6, PtSeg14N8, PtSeg14N13};
  static const int ptlen1[] = {5, 4, 6, 4, 4};
  // smooth style PT_BEZIERTO
  static const DPOINT PtSeg14N0S[13] = {20, 12, 20, 25, 22, 36, 23, 39, 24, 40, 25, 39, 26, 36, 28, 25, 28, 12, 26, 10, 24, 9, 22, 10, 20, 12};
  static const DPOINT PtSeg14N1S[10] = {10, 10, 10, 13, 11, 20, 13, 28, 21, 38, 21, 37, 19, 26, 15, 16, 11, 10, 10, 10};
  static const DPOINT PtSeg14N6S[6] = { 8, 41, 12, 45, 16, 45, 23, 41, 16, 37, 12, 37};
  static const DPOINT PtSeg14N8S[10] = { 4, 7, 4, 39, 5, 40, 6, 40, 9, 37, 11, 33, 11, 25, 9, 14, 5, 6, 4, 7};
  static const DPOINT PtSeg14N13S[17] = { 8, 4, 7, 5, 7, 6, 9, 8, 12, 9, 14, 11, 19, 11, 21, 9, 24, 7, 27, 9, 29, 11, 34, 11, 36, 9, 39, 8, 41, 6, 41, 5, 40, 4};
  static const DPOINT* PtSeg14NS[] = {PtSeg14N0S, PtSeg14N1S, PtSeg14N6S, PtSeg14N8S, PtSeg14N13S};
  static const int ptlen2[] = {13, 10, 6, 10, 17};
  WORD wSegmData = CHAR_SEGM14[iIndex];
  int i, p, j = 0;
  DPOINT* pt0 = pt;
  const DPOINT* pts;
  const DPOINT** ppts = (iDispStyle & DS_SMOOTH) ? PtSeg14NS : Pteg14N;
  const int* ptlens = (iDispStyle & DS_SMOOTH) ? ptlen2 : ptlen1;
  int nCount = 0;
  int m_Width = 49;
  int m_NSegments = 14;
  int id[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
  int id1[] = {0, 1, 1, 1, 1, 0, 2, 2, 3, 3, 4, 3, 3, 4};
  int id2[] = {0, 1, 1, 1, 1, 0, 2, 2, 3, 3, 4, 3, 3, 4};

  for (i = 0; i < m_NSegments; i++) {
    if ((wSegmData & (1 << i)) == 0) {
      continue;
    }

    // Find data for segment in correct style...
    pts = ppts[id1[i]];
    nCount = ptlens[id1[i]];

    // For nondefined segments use mirroring...
    switch (i) {
    case 5:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorY(&pts[p], 41);
      }

      break;

    case 2:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorX(&pts[p], (m_Width - 1) / 2);
      }

      break;

    case 3:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorY(&pts[p], 41);
      }

      break;

    case 4:
      for (p = 0; p < nCount; p++) {
        pt0[p] = PointMirror(&pts[p], (m_Width - 1) / 2, 41);
      }

      break;

    case 7:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorX(&pts[p], (m_Width - 1) / 2);
      }

      break;

    case 9:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorY(&pts[p], 41);
      }

      break;

    case 11:
      for (p = 0; p < nCount; p++) {
        pt0[p] = PointMirror(&pts[p], (m_Width - 1) / 2, 41);
      }

      break;

    case 12:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorX(&pts[p], (m_Width - 1) / 2);
      }

      break;

    case 10:
      for (p = 0; p < nCount; p++) {
        pt0[p] = LineMirrorY(&pts[p], 41);
      }

      break;

    default:
      for (p = 0; p < nCount; p++) {
        pt0[p] = pts[p];
      }

      break;
    }

    pt0 += nCount;
    lens[j++] = nCount;
  }

  return j;
}
int DigiColonDotChar(int iIndex, int iDispStyle, DPOINT* pt, int* lens)
{
  // For 3 segments semicoloncombi display...
  // . :
  BYTE CHAR_SEMCOL[MAXSEGSEMCOL] = {0x04, 0x03};
#ifdef _WIDE_SEMICOLON
  // for wide semicolon character space
  static const DPOINT PtSegScN0[4] = {19, 23, 19, 32, 32, 32, 32, 23};
  static const DPOINT PtSegScN1[4] = {19, 50, 19, 59, 32, 59, 32, 50};
  static const DPOINT PtSegScN2[4] = {19, 68, 19, 77, 32, 77, 32, 68};
  int m_Width = 49;
#else
  // small semicolon character space
  static const DPOINT PtSegScN0[] = {4, 23, 4, 32, 13, 32, 13, 23};
  static const DPOINT PtSegScN1[] = {4, 50, 4, 59, 13, 59, 13, 50};
  static const DPOINT PtSegScN2[] = {4, 68, 4, 77, 13, 77, 13, 68};
  int m_Width = 18;
#endif
  static const DPOINT* PtSegScN[] = {PtSegScN0, PtSegScN1, PtSegScN2};
  static const int ptlens[3] = {4, 4, 4};
  static const int id1[] = {0, 1, 2};
  WORD wSegmData = CHAR_SEMCOL[iIndex];
  int i, p, j = 0, nCount;
  DPOINT* pt0 = pt;
  const DPOINT* pts = 0;
  const DPOINT** ppts = PtSegScN;

  for (i = 0; i < countof(ptlens); i++) {
    if ((wSegmData & (1 << i)) == 0) {
      continue;
    }

    pts = ppts[id1[i]];
    nCount = ptlens[id1[i]];

    // Find data for segment in correct style...
    for (p = 0; p < nCount; p++) {
      pt0[p] = pts[p];
    }

    pt0 += nCount;
    lens[j++] = nCount;
  }

  return j;
}
int GetDigiVectex(uint cChar, DWORD m_DispStyle, DPOINT* pt, int* lens)
{
  int iIndex, n;

  if (cChar >= _T('0') && cChar <= _T('9') || cChar == _T(' ') || cChar == _T('-')) {
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
  }
  else if (cChar >= _T('A') && cChar <= _T('Z')) {
    // 14 segment only...
    iIndex = cChar - _T('A') + 12;
    m_DispStyle |= DS_STYLE14;
  }
  else if (cChar >= _T('a') && cChar <= _T('z')) {
    // 14 segment only...
    iIndex = cChar - _T('a') + 12;
    m_DispStyle |= DS_STYLE14;
  }
  else {
    // Spcial characters...
    iIndex = 0;

    switch (cChar) {
    case _T(':'):
      iIndex++;

    case _T('.'):
      break;

    case _T('*'):
      iIndex = MAXSEGCHAR14 - 7;
      break;

    case _T('+'):
      iIndex = MAXSEGCHAR14 - 6;
      break;

    case _T('('):
      iIndex = MAXSEGCHAR14 - 5;
      break;

    case _T(')'):
      iIndex = MAXSEGCHAR14 - 4;
      break;

    case _T('\\'):
      iIndex = MAXSEGCHAR14 - 3;
      break;

    case _T('/'):
      iIndex = MAXSEGCHAR14 - 2;
      break;

    case _T('\''):
      iIndex = MAXSEGCHAR14 - 1;
      break;
      //default : ASSERT(FALSE);
    }
  }

  if (_T(':') == cChar || _T('.') == cChar) {
    //iIndex = _T('.')==cChar;
    n = DigiColonDotChar(iIndex, m_DispStyle, pt, lens);
  }
  else if (m_DispStyle & DS_STYLE14) {
    n = Digi14Segment(iIndex, m_DispStyle, pt, lens);
  }
  else {
    n = Digi7Segment(iIndex, m_DispStyle, pt, lens);
  }

  return n;
}
int sumi(int n, int* a)
{
  int i, s = 0;

  for (i = 0; i < n; ++i) {
    s += a[i];
  }

  return s;
}
int DrawDigi(HDDC hDC, const char* s, int slen, double x, double y, double cx, double cy, DWORD clr, DWORD DispStyle)
{
  DPOINT pt[1 << 10];
  int i, len, n, lens[1 << 5];
  const char* e = s + slen, *s2;
  DWORD m_DispStyle = DispStyle;
  HDC hMemDC = GetMemDC();

  for (; s < e; s = s2) {
    uint thisChar = nextChar(s, &s2);
    n = GetDigiVectex(thisChar, m_DispStyle, pt, lens);

    for (len = 0, i = 0; i < n; ++i) {
      len += lens[i];
    }

    for (i = 0; i < len; ++i) {
      pt[i].x = pt[i].x * cx / 49 + x;
      pt[i].y = pt[i].y * cy / NORM_DIGIHEIGHT + y;
    }

    drawaa_fillpoly_solid(hDC, pt, lens, n, clr);
    //DrawPoly_tt(hDC, pt, lens, n, clr);
    x += cx;
  }

  return 0;
}
int test_drawdigi(HDDC hDC, int ch)
{
  static double cx = 50;
  char* s = ".:8magic";
  DWORD DispStyle = DS_SMOOTH;
  int slen = strlen(s);
  DWORD clr = _RGB(255, 0, 0);

  if ('1' == ch) {
    cx *= 1.1;
    cx = MIN(cx, 800);
  }

  if ('2' == ch) {
    cx /= 1.1;
    cx = MAX(cx, 8);
  }

  DispStyle = 0;
  DrawDigi(hDC, s, slen, 100, 100, cx, cx * 1.5, clr, DispStyle);
  return 0;
}

