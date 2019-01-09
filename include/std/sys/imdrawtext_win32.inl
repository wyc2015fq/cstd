
#include <windows.h>
#include <stdio.h>
#include "cstd.h"

#pragma comment(lib, "gdi32.lib")

//
typedef struct XFONT {
  int m_intBold;
  int m_intAngle;
  BOOL m_bCheckItalic;
  BOOL m_bCheckReverse;
  int charLineW;
  int m_intScrollHPos, m_intScrollVPos;
  LPBITMAPINFO pbmpinfo;
  LPBITMAPFILEHEADER pfileinfo;
  LPBYTE pBuf;
  GLYPHMETRICS glpm;

  int m_intHeight;
  int m_intWidth;
  int m_intBits;
  HDC hdc;
  HGDIOBJ hOldFont;
  HFONT newfont;
}
XFONT;

int CALLBACK MyEnumFontProc(ENUMLOGFONTEX* lpelf, NEWTEXTMETRICEX* lpntm, DWORD nFontType, long lParam)
{
  if (lpelf) {
    printf("%s\n", lpelf->elfLogFont.lfFaceName);
    return 1;
  }

  return 0;
}

int DispFontList(void)
{
  HWND hW = GetFocus();
  HDC hdc = GetDC(hW);
  LOGFONT lf;
  lf.lfCharSet = DEFAULT_CHARSET;
  lf.lfFaceName[ 0 ] = '\0';
  lf.lfPitchAndFamily = 0;
  EnumFontFamiliesEx((HDC) hdc, &lf, (FONTENUMPROC) MyEnumFontProc, (LPARAM) NULL, 0);
  return 1;
}

#include <windowsx.h>

BOOL XFONT_Init(XFONT* This, const TCHAR* strFontName, int cy, int cx, int bItalic, int nBold)
{
  HWND hW = NULL;
  // TODO: 在此添加额外的初始化代码
  This->m_intHeight = cy;
  This->m_intWidth = cx;
  This->m_intAngle = 0;
  This->m_intBold = nBold;
  This->m_bCheckItalic = bItalic;
  This->m_bCheckReverse = FALSE;
  This->charLineW = 0;
  This->pbmpinfo = NULL;
  This->pfileinfo = NULL;
  This->pBuf = NULL;
  This->m_intBits = 1;
  This->newfont = NULL;
  This->hOldFont = NULL;
  This->hdc = NULL;
  This->m_intScrollHPos = 0;
  This->m_intScrollVPos = 0;
  hW = GetDesktopWindow();
  This->hdc = GetDC(hW);

  //This->m_intAngle = -190;
  if (NULL == strFontName) {
    LOGFONT lf;
    SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    //strFontName = lf.lfFaceName;
    lf.lfHeight *= 2;
    This->m_intHeight = lf.lfHeight;
    This->newfont = (HFONT)CreateFontIndirect(&lf);
    //This->newfont = (HFONT)GetStockObject(SYSTEM_FONT);
  }
  else {
    This->newfont = CreateFont(This->m_intHeight, This->m_intWidth, This->m_intAngle,
        0, This->m_intBold, This->m_bCheckItalic, 0, 0,
        DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, strFontName);
  }

  //CDC* pdc=This->m_ctlDisplay.GetDC();
  //CFont *poldfont=pdc->SelectObject(&newfont);
  This->hOldFont = SelectObject(This->hdc, This->newfont);
  return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

int XFONT_UnInit(XFONT* This)
{
  SAFEFREE(This->pBuf);
  SAFEFREE(This->pfileinfo);
  SAFEFREE(This->pbmpinfo);

  if (This->newfont) {
    DeleteObject(This->newfont);
    This->newfont = 0;
  }

  if (This->hOldFont) {
    SelectObject(This->hdc, This->hOldFont);
    This->hOldFont = 0;
  }

  if (This->hdc) {
    HWND hW = NULL;
    hW = GetDesktopWindow();
    ReleaseDC(hW, This->hdc);
    This->hdc = 0;
  }

  return 0;
}

const int BITSTABLE[] = { 2, 5, 17, 65 };
void FormBitmapInfo(XFONT* This, int nWidth, int nHeight, int nBits)
{
  int i;
  RGBQUAD* pquad = NULL;

  if (This->pbmpinfo != NULL) {
    free(This->pbmpinfo);
  }

  This->pbmpinfo = (LPBITMAPINFO) malloc(sizeof(BYTE) * (sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * BITSTABLE[ nBits - 1 ]));
  This->pbmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  This->pbmpinfo->bmiHeader.biWidth = nWidth;
  This->pbmpinfo->bmiHeader.biHeight = -nHeight;
  This->pbmpinfo->bmiHeader.biPlanes = 1;
  This->pbmpinfo->bmiHeader.biBitCount = nBits == 1 ? 1 : 8;
  This->pbmpinfo->bmiHeader.biCompression = BI_RGB;
  This->pbmpinfo->bmiHeader.biSizeImage = 0;
  This->pbmpinfo->bmiHeader.biXPelsPerMeter = 0;
  This->pbmpinfo->bmiHeader.biYPelsPerMeter = 0;
  This->pbmpinfo->bmiHeader.biClrUsed = BITSTABLE[ nBits - 1 ];
  This->pbmpinfo->bmiHeader.biClrImportant = BITSTABLE[ nBits - 1 ];
  pquad = (RGBQUAD*) malloc(sizeof(RGBQUAD) * (BITSTABLE[ nBits - 1 ]));

  if (nBits == 1) {
    if (This->m_bCheckReverse == FALSE) {
      (*pquad).rgbRed = 255;
      (*pquad).rgbGreen = 255;
      (*pquad).rgbBlue = 255;
      (*pquad).rgbReserved = 0;
      (*(pquad + 1)).rgbRed = 0;
      (*(pquad + 1)).rgbGreen = 0;
      (*(pquad + 1)).rgbBlue = 0;
      (*(pquad + 1)).rgbReserved = 0;
    }
    else {
      (*pquad).rgbRed = 0;
      (*pquad).rgbGreen = 0;
      (*pquad).rgbBlue = 0;
      (*pquad).rgbReserved = 0;
      (*(pquad + 1)).rgbRed = 255;
      (*(pquad + 1)).rgbGreen = 255;
      (*(pquad + 1)).rgbBlue = 255;
      (*(pquad + 1)).rgbReserved = 0;
    }
  }
  else {
    if (This->m_bCheckReverse == FALSE) {
      for (i = 0; i < BITSTABLE[ nBits - 1 ]; i++) {
        (*(pquad + i)).rgbRed = (*(pquad + i)).rgbGreen = (*(pquad + i)).rgbBlue = 255 - 255 * i / (BITSTABLE[ nBits - 1 ] - 1);
        (*(pquad + i)).rgbReserved = 0;
      }
    }
    else {
      for (i = 0; i < BITSTABLE[ nBits - 1 ]; i++) {
        (*(pquad + i)).rgbRed = (*(pquad + i)).rgbGreen = (*(pquad + i)).rgbBlue = 255 * i / (BITSTABLE[ nBits - 1 ] - 1);
        (*(pquad + i)).rgbReserved = 0;
      }
    }
  }

  memcpy(&This->pbmpinfo->bmiColors, pquad, sizeof(RGBQUAD) * BITSTABLE[ nBits - 1 ]);
  free(pquad);
}

void FormFileHeader(XFONT* This, int nWidth, int nHeight, int nBits)
{
  if (This->pfileinfo != NULL) {
    free(This->pfileinfo);
  }

  This->pfileinfo = (LPBITMAPFILEHEADER) malloc(sizeof(BYTE) * (sizeof(BITMAPFILEHEADER)));
  This->pfileinfo->bfType = *(WORD*)(&"BM");
  This->pfileinfo->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * BITSTABLE[ nBits - 1 ];
  This->pfileinfo->bfSize = This->pfileinfo->bfOffBits + This->charLineW * nHeight;
}

void Save(XFONT* This, const char* strfile)
{
  if (This && This->pbmpinfo && This->pBuf) {
    FILE* cf;
    cf = fopen(strfile, "wb");

    if (cf) {
      FormFileHeader(This, This->glpm.gmBlackBoxX, This->glpm.gmBlackBoxY, This->m_intBits);
      fwrite(This->pfileinfo, sizeof(BITMAPFILEHEADER), 1, cf);
      fwrite(This->pbmpinfo, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * BITSTABLE[ This->m_intBits - 1 ], 1, cf);
      fwrite(This->pBuf, This->charLineW * This->glpm.gmBlackBoxY, 1, cf);
      fclose(cf);
    }
  }
}

int XFONT_GetFont(XFONT* This, uint chartemp)
{
  //if(m_strFontName.GetLength()>0)
  MAT2 mmat2 = { 0, 1, 0, 0, 0, 0, 0, 1 };
  int nLen;

  TRACE("chartemp is %d, sizeof(uint) is %d\n", chartemp, sizeof(uint));

  if (This->m_intBits == 1) {
    nLen = GetGlyphOutline(This->hdc, chartemp, This->m_intBits, &This->glpm, 0, NULL, &mmat2);
  }
  else {
    nLen = GetGlyphOutline(This->hdc, chartemp, This->m_intBits + 2, &This->glpm, 0, NULL, &mmat2);
  }

  //GGO_BITMAP 1
  // GGO_GRAY2_BITMAP   4
  // GGO_GRAY4_BITMAP   5
  //GGO_GRAY8_BITMAP   6
  if (nLen > 0) {
    int nLentemp;

    if (This->pBuf != NULL) {
      free(This->pBuf);
    }

    This->pBuf = (BYTE*) malloc(nLen * sizeof(BYTE));

    if (This->m_intBits == 1) {
      nLentemp = GetGlyphOutline(This->hdc, chartemp, This->m_intBits, &This->glpm, nLen, This->pBuf, &mmat2);
      This->charLineW = (This->glpm.gmBlackBoxX / 32 + (This->glpm.gmBlackBoxX % 32 == 0 ? 0 : 1)) * 4;
      FormBitmapInfo(This, This->glpm.gmBlackBoxX, This->glpm.gmBlackBoxY, This->m_intBits);

      if (nLentemp != nLen) {
        TRACE("字体过大！不支持！");
      }
      else {
        TRACE("正常！");
      }
    }
    else {
      nLentemp = GetGlyphOutline(This->hdc, chartemp, This->m_intBits + 2, &This->glpm, nLen, This->pBuf, &mmat2);
      This->charLineW = (This->glpm.gmBlackBoxX / 4 + (This->glpm.gmBlackBoxX % 4 == 0 ? 0 : 1)) * 4;
      FormBitmapInfo(This, This->glpm.gmBlackBoxX, This->glpm.gmBlackBoxY, This->m_intBits);

      if (nLentemp != nLen) {
        TRACE("字体过大！不支持！");
      }
      else {
        TRACE("正常！");
      }
    }

    TRACE("x is %d,y is %d\n", This->glpm.gmBlackBoxX, This->glpm.gmBlackBoxY);
  }
  else {
    TRACE("字体过大！不支持！");
  }

  return 0;
}

#define PUTPIX3(_PTR, _T)   if (_T) { (_PTR)[0] = cb, (_PTR)[1] = cg, (_PTR)[2] = cr; }
#define PUTPIX(_PTR, _T)   if (_T) { (_PTR)[0] = cb; }

void PutOneChar(int h, int w, unsigned char* img, int al, int ai,
    int y, int x, int cy, int cx, const unsigned char* font, int fl, DWORD color)
{
  int i, j;
  int cb = ((unsigned char*) color) [ 0 ], cg = ((unsigned char*) color) [ 1 ], cr = ((unsigned char*) color) [ 2 ];
  unsigned char* ptr = img + y * al + x * ai;
  cy = MIN(cy, (h - y));
  cx = MIN(cx, (h - x));

  for (i = 0; i < cy; ++i, font += fl, ptr += al) {
    unsigned char* ptr2 = ptr;

    if ((i + y) < 0 || (i + y) >= h) {
      continue;
    }

    if (3 <= ai) {
      for (j = 0; j < fl; ++j, ptr2 += ai * 8) {
        char t = font[ j ];
        int k = 0;

        if ((j + x) < 0 || (j + x) >= w) {
          continue;
        }

        if (t) {
          for (k = 0; k < 8; ++k) {
            PUTPIX3(ptr2 + k * ai, t & (1 << (7 - k)));
          }
        }
      }
    }
    else if (1 == ai) {
      for (j = 0; j < fl; ++j, ptr2 += 8) {
        char t = font[ j ];
        int k = 0;

        if ((j + x) < 0 || (j + x) >= w) {
          continue;
        }

        if (t) {
          for (k = 0; k < 8; ++k) {
            PUTPIX(ptr2 + k, t & (1 << (7 - k)));
          }
        }
      }
    }
  }

  return ;
}

SIZE TGetTextExtent(const TCHAR* strFontName, int cy, int cx, int bItalic, int bBold)
{
  SIZE sz;
  XFONT fd;
  XFONT_Init(&fd, strFontName, cy, cx, bItalic, bBold ? FW_BOLD : FW_THIN);
  sz.h = fd.glpm.gmBlackBoxY, sz.w = fd.glpm.gmBlackBoxX;
  XFONT_UnInit(&fd);
  return sz;
}

int xDrawTextWithFont(int h, int w, unsigned char* img, int al, int ai,
    int y, int x, const TCHAR* strFontName, int cy, int cx,
    const char* str, int slen, XFONT* pfd,
    uint uFormat, DWORD color)
{
  const char* str0 = str;
  int y0, x0;

  for (; str - str0 < slen;) {
    BYTE temp;
    uint chartemp;
    temp = *str;

    if (temp >= 128) {
      chartemp = ((uint) temp) << 8;
      temp = str[ 1 ];
      chartemp += temp;
      str += 2;
    }
    else {
      ++str;
      chartemp = temp;
    }

    if (' ' != chartemp) {
      XFONT_GetFont(pfd, chartemp);
      y0 = y + cy - pfd->glpm.gmptGlyphOrigin.y - (cy / 8);
      x0 = x + pfd->glpm.gmptGlyphOrigin.x;

      if (DT_CENTER & uFormat) {
        int kk = abs(pfd->m_intHeight);
        y0 -= (cy - kk) / 2;
        x0 += (cx - (kk / 2) * slen) / 2;
      }

      PutOneChar(h, w, img, al, ai, y0, x0, pfd->glpm.gmBlackBoxY, pfd->glpm.gmBlackBoxX, pfd->pBuf, pfd->charLineW, color);
    }

    x += pfd->glpm.gmCellIncX;
    //Save(&fd, "a.bmp");
  }

  return 0;
}
void xDrawText1(int h, int w, unsigned char* img, int al, int ai,
    int y, int x, const TCHAR* strFontName, int cy, int cx,
    int bItalic, int bBold, const char* str, int slen,
    uint uFormat, DWORD color)
{
  XFONT fd;
  XFONT_Init(&fd, strFontName, cy, cx, bItalic, bBold ? FW_BOLD : FW_THIN);
  xDrawTextWithFont(h, w, img, al, ai, y, x, strFontName, cy, cx, str, slen, &fd, uFormat, color);
  XFONT_UnInit(&fd);
}

void xDrawText(int h, int w, unsigned char* img, int al, int ai,
    int y, int x, const TCHAR* strFontName, int cy, int cx,
    int bItalic, int bBold, const char* str,
    DWORD color)
{
  xDrawText1(h, w, img, al, ai, y, x, strFontName, cy, cx, bItalic, bBold, str, strlen(str), 0, color);
}

#undef PUTPIX3
#undef PUTPIX
