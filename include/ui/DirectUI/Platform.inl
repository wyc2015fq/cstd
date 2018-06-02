
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#undef _WIN32_WINNT
#define _WIN32_WINNT  0x0500
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include <windowsx.h>

#include "UniConversion.inl"

// We want to use multi monitor functions, but via LoadLibrary etc
// Luckily microsoft has done the heavy lifting for us, so we'll just use their stub functions!
#if defined(_MSC_VER) && (MSC_VER > 1200)
#define COMPILE_MULTIMON_STUBS
#include "MultiMon.h"
#endif

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

typedef BOOL (WINAPI *AlphaBlendSig)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);

static CRITICAL_SECTION crPlatformLock;
static HINSTANCE hinstPlatformRes = 0;
static bool onNT = false;
static HMODULE hDLLImage = 0;
static AlphaBlendSig AlphaBlendFn = 0;

bool IsNT() {
  return onNT;
}

int Minimum(int a, int b) {
  if (a < b)
    return a;
  else
    return b;
}

int Maximum(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}

Point ptFromLong(long lpoint) {
  return ptPoint((short)(LOWORD(lpoint)), (short)(HIWORD(lpoint)));
}

static RECT RectFromPRectangle(PRectangle prc) {
  RECT rc = {prc.left, prc.top, prc.right, prc.bottom};
  return rc;
}

Palette palNew() {
  Palette pal;
  pal.used = 0;
  pal.allowRealization = false;
  pal.hpal = 0;
  pal.size = 100;
  pal.entries = (ColourPair*)malloc(sizeof(ColourPair)*pal.size);
  return pal;
}
void palRelease(Palette* pal) {
  pal->used = 0;
  if (pal->hpal)
    DeleteObject(pal->hpal);
  pal->hpal = 0;
  free(pal->entries);
  pal->size = 100;
  pal->entries = (ColourPair*)malloc(sizeof(ColourPair)*pal->size);
}

void palDel(Palette* pal) {
  palRelease(pal);
  free(pal->entries);
  pal->entries = 0;
}

/**
 * This method either adds a colour to the list of wanted colours (want==true)
 * or retrieves the allocated colour back to the ColourPair.
 * This is one method to make it easier to keep the code for wanting and retrieving in sync.
*/
#define NEW(T, N)  (T*)malloc(sizeof(T)*(N))
#define DEL(P)  free((P))
ColourPair palWantFind(Palette* pal, bool want) {
  int i, j;
  ColourPair cp = {0,0};
  if (want) {
    for (i=0; i < pal->used; i++) {
      if (pal->entries[i].desired == cp.desired)
        return cp;
    }

    if (pal->used >= pal->size) {
      int sizeNew = pal->size * 2;
      ColourPair *entriesNew = NEW(ColourPair, sizeNew);
      for (j=0; j<pal->size; j++) {
        entriesNew[j] = pal->entries[j];
      }
      DEL(pal->entries);
      pal->entries = entriesNew;
      pal->size = sizeNew;
    }

    pal->entries[pal->used].desired = cp.desired;
    pal->entries[pal->used].allocated=(cp.desired);
    pal->used++;
  } else {
    for (i=0; i < pal->used; i++) {
      if (pal->entries[i].desired == cp.desired) {
        cp.allocated = pal->entries[i].allocated;
        return cp;
      }
    }
    cp.allocated=(cp.desired);
  }
  return cp;
}

void palAllocate(Palette* pal) {
  if (pal->hpal)
    DeleteObject(pal->hpal);
  pal->hpal = 0;

  if (pal->allowRealization) {int iPal;
    char *pal0 = NEW(char, sizeof(LOGPALETTE) + (pal->used-1) * sizeof(PALETTEENTRY));
    LOGPALETTE *logpal = (LOGPALETTE *)(pal0);
    logpal->palVersion = 0x300;
    logpal->palNumEntries = (WORD)(pal->used);
    for (iPal=0;iPal<pal->used;iPal++) {
      long desired = pal->entries[iPal].desired;
      logpal->palPalEntry[iPal].peRed   = (BYTE)(clrGetRed(desired));
      logpal->palPalEntry[iPal].peGreen = (BYTE)(clrGetGreen(desired));
      logpal->palPalEntry[iPal].peBlue  = (BYTE)(clrGetBlue(desired));
      pal->entries[iPal].allocated = (
        PALETTERGB(clrGetRed(desired), clrGetGreen(desired), clrGetBlue(desired)));
      // PC_NOCOLLAPSE means exact colours allocated even when in background this means other windows
      // are less likely to get their colours and also flashes more when switching windows
      logpal->palPalEntry[iPal].peFlags = PC_NOCOLLAPSE;
      // 0 allows approximate colours when in background, yielding moe colours to other windows
      //logpal->palPalEntry[iPal].peFlags = 0;
    }
    pal->hpal = CreatePalette(logpal);
    DEL(pal);
  }
}

#ifndef CLEARTYPE_QUALITY
#define CLEARTYPE_QUALITY 5
#endif
#define SC_EFF_QUALITY_MASK 0xF

static BYTE Win32MapFontQuality(int extraFontFlag) {
  return 0;
}

static void SetLogFont(LOGFONTA* lf, const char *faceName, int characterSet, int size, bool bold, bool italic, int extraFontFlag) {
  memset(&lf, 0, sizeof(lf));
  // The negative is to allow for leading
  lf->lfHeight = -(abs(size));
  lf->lfWeight = bold ? FW_BOLD : FW_NORMAL;
  lf->lfItalic = (BYTE)(italic ? 1 : 0);
  lf->lfCharSet = (BYTE)(characterSet);
  lf->lfQuality = Win32MapFontQuality(extraFontFlag);
  strncpy(lf->lfFaceName, faceName, sizeof(lf->lfFaceName));
}

/**
 * Create a hash from the parameters for a font to allow easy checking for identity.
 * If one font is the same as another, its hash will be the same, but if the hash is the
 * same then they may still be different.
 */
static int HashFont(const char *faceName, int characterSet, int size, bool bold, bool italic, int extraFontFlag) {
  return
    size ^
    (characterSet << 10) ^
    ((extraFontFlag & SC_EFF_QUALITY_MASK) << 9) ^
    (bold ? 0x10000000 : 0) ^
    (italic ? 0x20000000 : 0) ^
    faceName[0];
}

typedef struct _FontCached {
  Font font;
  struct _FontCached *next;
  int usage;
  LOGFONTA lf;
  int hash;
}FontCached;

FontCached *FontCachedfirst = 0;

FontCached* newFontCached(const char *faceName_, int characterSet_, int size_, bool bold_, bool italic_, int extraFontFlag_) {
  FontCached* ft = NEW(FontCached, 1);
  ft->next=(0), ft->usage=(0), ft->hash=(0);
  SetLogFont(&ft->lf, faceName_, characterSet_, size_, bold_, italic_, extraFontFlag_);
  ft->hash = HashFont(faceName_, characterSet_, size_, bold_, italic_, extraFontFlag_);
  ft->font.fid = CreateFontIndirectA(&ft->lf);
  ft->usage = 1;
  return ft;
}

bool ftcSameAs(FontCached* ft, const char *faceName_, int characterSet_, int size_, bool bold_, bool italic_, int extraFontFlag_) {
  return
    (ft->lf.lfHeight == -(abs(size_))) &&
    (ft->lf.lfWeight == (bold_ ? FW_BOLD : FW_NORMAL)) &&
    (ft->lf.lfItalic == (BYTE)(italic_ ? 1 : 0)) &&
    (ft->lf.lfCharSet == characterSet_) &&
    (ft->lf.lfQuality == Win32MapFontQuality(extraFontFlag_)) &&
    0 == strcmp(ft->lf.lfFaceName,faceName_);
}

void ftcRelease(FontCached* ft) {
  if (ft->font.fid)
    DeleteObject(ft->font.fid);
  ft->font.fid = 0;
}

FontID ftcFindOrCreate(const char *faceName_, int characterSet_, int size_, bool bold_, bool italic_, int extraFontFlag_) {
  FontID ret = 0;
  int hashFind;
  FontCached* cur;
  EnterCriticalSection(&crPlatformLock);
  hashFind = HashFont(faceName_, characterSet_, size_, bold_, italic_, extraFontFlag_);
  for (cur=FontCachedfirst; cur; cur=cur->next) {
    if ((cur->hash == hashFind) &&
      ftcSameAs(cur, faceName_, characterSet_, size_, bold_, italic_, extraFontFlag_)) {
      cur->usage++;
      ret = cur->font.fid;
    }
  }
  if (ret == 0) {
    FontCached *fc = newFontCached(faceName_, characterSet_, size_, bold_, italic_, extraFontFlag_);
    if (fc) {
      fc->next = FontCachedfirst;
      FontCachedfirst = fc;
      ret = fc->font.fid;
    }
  }
  LeaveCriticalSection(&crPlatformLock);
  return ret;
}

void ftcReleaseId(FontID fid_) {
  FontCached* cur;
  FontCached **pcur=&FontCachedfirst;
  EnterCriticalSection(&crPlatformLock);
  for (cur=FontCachedfirst; cur; cur=cur->next) {
    if (cur->font.fid == fid_) {
      cur->usage--;
      if (cur->usage == 0) {
        *pcur = cur->next;
        ftcRelease(cur);
        cur->next = 0;
        DEL(cur);
      }
      break;
    }
    pcur=&cur->next;
  }
  LeaveCriticalSection(&crPlatformLock);
}

#define FONTS_CACHED

Font ftCreate(const char *faceName, int characterSet, int size,
  bool bold, bool italic, int extraFontFlag) {
  Font ft;
#ifndef FONTS_CACHED
  LOGFONT lf;
  SetLogFont(lf, faceName, characterSet, size, bold, italic, extraFontFlag);
  fid = CreateFontIndirect(&lf);
#else
  if (faceName)
    ft.fid = ftcFindOrCreate(faceName, characterSet, size, bold, italic, extraFontFlag);
#endif
  return ft;
}

void ftRelease(Font* ft) {
#ifndef FONTS_CACHED
  if (ft->fid)
    DeleteObject(ft->fid);
#else
  if (ft->fid)
    ftcReleaseId(ft->fid);
#endif
  ft->fid = 0;
}

typedef struct _SurfaceImpl {
  Surface_vtable* vtable;
  bool unicodeMode;
  HDC hdc;
  bool hdcOwned;
  HPEN pen;
  HPEN penOld;
  HBRUSH brush;
  HBRUSH brushOld;
  HFONT font;
  HFONT fontOld;
  HBITMAP bitmap;
  HBITMAP bitmapOld;
  HPALETTE paletteOld;
  int maxWidthMeasure;
  int maxLenText;
  int codePage;
  // If 9x OS and current code page is same as ANSI code page.
  bool win9xACPSame;
}SurfaceImpl;

void sfRelease(Surface* s) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  if (sf->penOld) {
    SelectObject((HDC)(sf->hdc), sf->penOld);
    DeleteObject(sf->pen);
    sf->penOld = 0;
  }
  sf->pen = 0;
  if (sf->brushOld) {
    SelectObject((HDC)(sf->hdc), sf->brushOld);
    DeleteObject(sf->brush);
    sf->brushOld = 0;
  }
  sf->brush = 0;
  if (sf->fontOld) {
    // Fonts are not deleted as they are owned by a Font object
    SelectObject((HDC)(sf->hdc), sf->fontOld);
    sf->fontOld = 0;
  }
  sf->font = 0;
  if (sf->bitmapOld) {
    SelectObject((HDC)(sf->hdc), sf->bitmapOld);
    DeleteObject(sf->bitmap);
    sf->bitmapOld = 0;
  }
  sf->bitmap = 0;
  if (sf->paletteOld) {
    // Palettes are not deleted as they are owned by a Palette object
    SelectPalette((HDC)(sf->hdc),
      (HPALETTE)(sf->paletteOld), TRUE);
    sf->paletteOld = 0;
  }
  if (sf->hdcOwned) {
    DeleteDC((HDC)(sf->hdc));
    sf->hdc = 0;
    sf->hdcOwned = false;
  }
}

bool sfInitialised(Surface* s) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  return sf->hdc != 0;
}
void sfInitPixMap(Surface* s, int width, int height, Surface *surface_, WindowID wid) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  SurfaceImpl* sf2 = (SurfaceImpl*)(surface_);
  sfRelease(s);
  sf->hdc = CreateCompatibleDC(sf2->hdc);
  sf->hdcOwned = true;
  sf->bitmap = CreateCompatibleBitmap(sf2->hdc, width, height);
  sf->bitmapOld = (HBITMAP)(SelectObject(sf->hdc, sf->bitmap));
  SetTextAlign((HDC)(sf->hdc), TA_BASELINE);
}

void sfPenColour(Surface* s, ColourAllocated fore) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  if (sf->pen) {
    SelectObject(sf->hdc, sf->penOld);
    DeleteObject(sf->pen);
    sf->pen = 0;
    sf->penOld = 0;
  }
  sf->pen = CreatePen(0,1,fore);
  sf->penOld = (HPEN)(SelectObject((HDC)(sf->hdc), sf->pen));
}

void sfBrushColor(Surface* s, ColourAllocated back) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  ColourAllocated colourNearest;
  if (sf->brush) {
    SelectObject(sf->hdc, sf->brushOld);
    DeleteObject(sf->brush);
    sf->brush = 0;
    sf->brushOld = 0;
  }
  // Only ever want pure, non-dithered brushes
  colourNearest = GetNearestColor(sf->hdc, back);
  sf->brush = CreateSolidBrush(colourNearest);
  sf->brushOld = (HBRUSH)(SelectObject(sf->hdc, sf->brush));
}

int sfLogPixelsY(Surface* s) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  return GetDeviceCaps(sf->hdc, LOGPIXELSY);
}

int sfDeviceHeightFont(Surface* s, int points) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  return MulDiv(points, sfLogPixelsY(s), 72);
}

void sfMoveTo(Surface* s, int x_, int y_) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  MoveToEx(sf->hdc, x_, y_, 0);
}

void sfLineTo(Surface* s, int x_, int y_) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  LineTo(sf->hdc, x_, y_);
}

void sfPolygon(Surface* s, Point *pts, int npts, ColourAllocated fore, ColourAllocated back) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfPenColour(s, fore);
  sfBrushColor(s, back);
  Polygon(sf->hdc, (POINT *)(pts), npts);
}

void sfRectangleDraw(Surface* s, PRectangle rc, ColourAllocated fore, ColourAllocated back) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfPenColour(s, fore);
  sfBrushColor(s, back);
  Rectangle(sf->hdc, rc.left, rc.top, rc.right, rc.bottom);
}

void sfFillRectangle(Surface* s, PRectangle rc, ColourAllocated back) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  // Using ExtTextOut rather than a FillRect ensures that no dithering occurs.
  // There is no need to allocate a brush either.
  RECT rcw = RectFromPRectangle(rc);
  SetBkColor(sf->hdc, back);
  ExtTextOut(sf->hdc, rc.left, rc.top, ETO_OPAQUE, &rcw, TEXT(""), 0, NULL);
}

void sfFillRectangle2(Surface* s, PRectangle rc, Surface* surfacePattern) {
  RECT rcw;
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  SurfaceImpl* sf2 = (SurfaceImpl*)(surfacePattern);
  HBRUSH br;
  if (sf2->bitmap)
    br = CreatePatternBrush(sf2->bitmap);
  else  // Something is wrong so display in red
    br = CreateSolidBrush(RGB(0xff, 0, 0));
  rcw = RectFromPRectangle(rc);
  FillRect(sf->hdc, &rcw, br);
  DeleteObject(br);
}

void sfRoundedRectangle(Surface* s, PRectangle rc, ColourAllocated fore, ColourAllocated back) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfPenColour(s, fore);
  sfBrushColor(s, back);
  RoundRect(sf->hdc, rc.left + 1, rc.top, rc.right - 1, rc.bottom, 8, 8);
}

// Plot a point into a DWORD buffer symetrically to all 4 qudrants
static void AllFour(DWORD *pixels, int width, int height, int x, int y, DWORD val) {
  pixels[y*width+x] = val;
  pixels[y*width+width-1-x] = val;
  pixels[(height-1-y)*width+x] = val;
  pixels[(height-1-y)*width+width-1-x] = val;
}

#ifndef AC_SRC_OVER
#define AC_SRC_OVER                 0x00
#endif
#ifndef AC_SRC_ALPHA
#define AC_SRC_ALPHA    0x01
#endif

static DWORD dwordFromBGRA(byte b, byte g, byte r, byte a) {
  union {
    byte pixVal[4];
    DWORD val;
  } converter;
  converter.pixVal[0] = b;
  converter.pixVal[1] = g;
  converter.pixVal[2] = r;
  converter.pixVal[3] = a;
  return converter.val;
}

void sfAlphaRectangle(Surface* s, PRectangle rc, int cornerSize, ColourAllocated fill, int alphaFill,
                      ColourAllocated outline, int alphaOutline, int flags ) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  if (AlphaBlendFn && rcWidth(rc) > 0) {
    HDC hMemDC = CreateCompatibleDC((HDC)(sf->hdc));
    int width = rcWidth(rc);
    int height = rcHeight(rc);
    // Ensure not distorted too much by corners when small
    BITMAPINFO bpih = {sizeof(BITMAPINFOHEADER), width, height, 1, 32, BI_RGB, 0, 0, 0, 0, 0};
    void *image = 0;
    HBITMAP hbmMem = CreateDIBSection((HDC)(hMemDC), &bpih,
      DIB_RGB_COLORS, &image, NULL, 0);

    HBITMAP hbmOld = SelectBitmap(hMemDC, hbmMem);

    DWORD valEmpty = dwordFromBGRA(0,0,0,0);
    DWORD valFill = dwordFromBGRA(
      (byte)(GetBValue(fill) * alphaFill / 255),
      (byte)(GetGValue(fill) * alphaFill / 255),
      (byte)(GetRValue(fill) * alphaFill / 255),
      (byte)(alphaFill));
    DWORD valOutline = dwordFromBGRA(
      (byte)(GetBValue(outline) * alphaOutline / 255),
      (byte)(GetGValue(outline) * alphaOutline / 255),
      (byte)(GetRValue(outline) * alphaOutline / 255),
      (byte)(alphaOutline));
    DWORD *pixels = (DWORD *)(image);
    BLENDFUNCTION merge = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    int x, y, c;
    cornerSize = Minimum(cornerSize, (Minimum(width, height) / 2) - 2);
    for (y=0; y<height; y++) {
      for (x=0; x<width; x++) {
        if ((x==0) || (x==width-1) || (y == 0) || (y == height-1)) {
          pixels[y*width+x] = valOutline;
        } else {
          pixels[y*width+x] = valFill;
        }
      }
    }
    for (c=0;c<cornerSize; c++) {
      for (x=0;x<c+1; x++) {
        AllFour(pixels, width, height, x, c-x, valEmpty);
      }
    }
    for (x=1;x<cornerSize; x++) {
      AllFour(pixels, width, height, x, cornerSize-x, valOutline);
    }

    AlphaBlendFn((HDC)(sf->hdc), rc.left, rc.top, width, height, hMemDC, 0, 0, width, height, merge);

    SelectBitmap(hMemDC, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hMemDC);
  } else {
    RECT rcw;
    sfBrushColor(s, outline);
    rcw = RectFromPRectangle(rc);
    FrameRect(sf->hdc, &rcw, sf->brush);
  }
}

void sfEllipse(Surface* s, PRectangle rc, ColourAllocated fore, ColourAllocated back) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfPenColour(s, fore);
  sfBrushColor(s, back);
  Ellipse(sf->hdc, rc.left, rc.top, rc.right, rc.bottom);
}

void sfCopy(Surface* s, PRectangle rc, Point from, Surface* surfaceSource) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  SurfaceImpl* sf2 = (SurfaceImpl*)(surfaceSource);
  BitBlt(sf->hdc, rc.left, rc.top, rcWidth(rc), rcHeight(rc), sf2->hdc, from.x, from.y, SRCCOPY);
}

#define VarBufferNew(_n, T, lengthStandard, length)  { T _n##_bufferStandard[lengthStandard]; T * _n##_buffer =  (length > lengthStandard) ?  NEW(T, length) : _n##_bufferStandard;
#define VarBufferDel(_n)   if (_n##_buffer != _n##_bufferStandard) { DEL(_n##_buffer); _n##_buffer = 0; } }
#define stackBufferLength  10000
#define TextWideNew(_n, s, len, unicodeMode, codePage)   VarBufferNew(_n, wchar_t, stackBufferLength, len) \
int _n##_tlen = (unicodeMode) ? UTF16FromUTF8(s, len, _n##_buffer, len) : MultiByteToWideChar(codePage, 0, s, len, _n##_buffer, len);

#define TextPositionsNew(_n, len)  VarBufferNew(_n, int, stackBufferLength, len)

void sfSetFont(Surface* s, Font* font_) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  if (font_->fid != sf->font) {
    if (sf->fontOld) {
      SelectObject(sf->hdc, font_->fid);
    } else {
      sf->fontOld = (HFONT)(SelectObject(sf->hdc, font_->fid));
    }
    sf->font = (HFONT)(font_->fid);
  }
}

void sfDrawTextCommon(Surface* s0, PRectangle rc, Font* font_, int ybase, const char *s, int len, UINT fuOptions) {
  SurfaceImpl* sf = (SurfaceImpl*)(s0);
  RECT rcw = RectFromPRectangle(rc);
  SIZE sz={0,0};
  int pos = 0;
  int x = rc.left;

  // Text drawing may fail if the text is too big.
  // If it does fail, slice up into segments and draw each segment.
  const int maxSegmentLength = 0x200;
  sfSetFont(s0, font_);

  if ((!sf->unicodeMode) && (IsNT() || (sf->codePage==0) || sf->win9xACPSame)) {
    // Use ANSI calls
    int lenDraw = Minimum(len, sf->maxLenText);
    if (!ExtTextOutA(sf->hdc, x, ybase, fuOptions, &rcw, s, lenDraw, NULL)) {
      while (lenDraw > pos) {
        int seglen = Minimum(maxSegmentLength, lenDraw - pos);
        if (!ExtTextOutA(sf->hdc, x, ybase, fuOptions, &rcw, s+pos, seglen, NULL)) {
          PLATFORM_ASSERT(false);
          return;
        }
        GetTextExtentPoint32A(sf->hdc, s+pos, seglen, &sz);
        x += sz.cx;
        pos += seglen;
      }
    }
  } else {
    // Use Unicode calls
    TextWideNew(tbuf, s, len, sf->unicodeMode, sf->codePage);
    if (!ExtTextOutW(sf->hdc, x, ybase, fuOptions, &rcw, tbuf_buffer, tbuf_tlen, NULL)) {
      while (tbuf_tlen > pos) {
        int seglen = Minimum(maxSegmentLength, tbuf_tlen - pos);
        if (!ExtTextOutW(sf->hdc, x, ybase, fuOptions, &rcw, tbuf_buffer+pos, seglen, NULL)) {
          PLATFORM_ASSERT(false);
          return;
        }
        GetTextExtentPoint32W(sf->hdc, tbuf_buffer+pos, seglen, &sz);
        x += sz.cx;
        pos += seglen;
      }
    }
    VarBufferDel(tbuf);
  }
}

void sfDrawTextNoClip(Surface* s0, PRectangle rc, Font* font_, int ybase, const char *s, int len,
                      ColourAllocated fore, ColourAllocated back) {
  SurfaceImpl* sf = (SurfaceImpl*)(s0);
  SetTextColor(sf->hdc, fore);
  SetBkColor(sf->hdc, back);
  sfDrawTextCommon(s0, rc, font_, ybase, s, len, ETO_OPAQUE);
}

void sfDrawTextClipped(Surface* s0, PRectangle rc, Font* font_, int ybase, const char *s, int len,
                       ColourAllocated fore, ColourAllocated back) {
  SurfaceImpl* sf = (SurfaceImpl*)(s0);
  SetTextColor(sf->hdc, fore);
  SetBkColor(sf->hdc, back);
  sfDrawTextCommon(s0, rc, font_, ybase, s, len, ETO_OPAQUE | ETO_CLIPPED);
}

void sfDrawTextTransparent(Surface* s0, PRectangle rc, Font* font_, int ybase, const char *s, int len,
                           ColourAllocated fore) {
  SurfaceImpl* sf = (SurfaceImpl*)(s0);
  int i;
  // Avoid drawing spaces in transparent mode
  for (i=0;i<len;i++) {
    if (s[i] != ' ') {
      SetTextColor(sf->hdc, fore);
      SetBkMode(sf->hdc, TRANSPARENT);
      sfDrawTextCommon(s0, rc, font_, ybase, s, len, 0);
      SetBkMode(sf->hdc, OPAQUE);
      return;
    }
  }
}

int sfWidthText(Surface* s0, Font* font_, const char *s, int len) {
  SurfaceImpl* sf = (SurfaceImpl*)(s0);
  SIZE sz={0,0};
  sfSetFont(s0, font_);
  if ((!sf->unicodeMode) && (IsNT() || (sf->codePage==0) || sf->win9xACPSame)) {
    GetTextExtentPoint32A(sf->hdc, s, Minimum(len, sf->maxLenText), &sz);
  } else {
    TextWideNew(tbuf, s, len, sf->unicodeMode, sf->codePage);
    GetTextExtentPoint32W(sf->hdc, tbuf_buffer, tbuf_tlen, &sz);
    VarBufferDel(tbuf);
  }
  return sz.cx;
}

void sfMeasureWidths(Surface* s0, Font* font_, const char *s, int len, int *positions) {
  SurfaceImpl* sf = (SurfaceImpl*)(s0);
  SIZE sz={0,0};
  int fit = 0;
  sfSetFont(s0, font_);
  if (sf->unicodeMode) {
    int lastPos;
    int widthSS;
    int ui=0;
    int i=0;
    const unsigned char *us;
    TextWideNew(tbuf, s, len, sf->unicodeMode, sf->codePage);
    TextPositionsNew(poses, tbuf_tlen);
    fit = tbuf_tlen;
    if (!GetTextExtentExPointW(sf->hdc, tbuf_buffer, tbuf_tlen, sf->maxWidthMeasure, &fit, poses_buffer, &sz)) {
      // Likely to have failed because on Windows 9x where function not available
      // So measure the character widths by measuring each initial substring
      // Turns a linear operation into a qudratic but seems fast enough on test files
      for (widthSS=0; widthSS < tbuf_tlen; widthSS++) {
        GetTextExtentPoint32W(sf->hdc, tbuf_buffer, widthSS+1, &sz);
        poses_buffer[widthSS] = sz.cx;
      }
    }
    // Map the widths given for UTF-16 characters back onto the UTF-8 input string
    us = (const unsigned char *)(s);
    ui=0;
    i=0;
    while (ui<fit) {
      unsigned int bytePos;
      unsigned char uch = us[i];
      unsigned int lenChar = 1;
      if (uch >= (0x80 + 0x40 + 0x20 + 0x10)) {
        lenChar = 4;
        ui++;
      } else if (uch >= (0x80 + 0x40 + 0x20)) {
        lenChar = 3;
      } else if (uch >= (0x80)) {
        lenChar = 2;
      }
      for (bytePos=0; (bytePos<lenChar) && (i<len); bytePos++) {
        positions[i++] = poses_buffer[ui];
      }
      ui++;
    }
    lastPos = 0;
    if (i > 0)
      lastPos = positions[i-1];
    while (i<len) {
      positions[i++] = lastPos;
    }
    VarBufferDel(poses);
    VarBufferDel(tbuf);
  } else if (IsNT() || (sf->codePage==0) || sf->win9xACPSame) {
    int startOffset = 0;
    // Zero positions to avoid random behaviour on failure.
    memset(positions, 0, len * sizeof(*positions));
    // len may be larger than platform supports so loop over segments small enough for platform
    while (len > 0) {
      int i;
      int lenBlock = Minimum(len, sf->maxLenText);
      if (!GetTextExtentExPointA(sf->hdc, s, lenBlock, sf->maxWidthMeasure, &fit, positions, &sz)) {
        // Eeek - a NULL DC or other foolishness could cause this.
        return;
      } else if (fit < lenBlock) {
        // For some reason, such as an incomplete DBCS character
        // Not all the positions are filled in so make them equal to end.
        for (i=fit;i<lenBlock;i++)
          positions[i] = positions[fit-1];
      } else if (startOffset > 0) {
        for (i=0;i<lenBlock;i++)
          positions[i] += startOffset;
      }
      startOffset = positions[lenBlock-1];
      len -= lenBlock;
      positions += lenBlock;
      s += lenBlock;
    }
  } else {
    int i, ui, widthSS;
    // Support Asian string display in 9x English
    TextWideNew(tbuf, s, len, sf->unicodeMode, sf->codePage);
    TextPositionsNew(poses, tbuf_tlen);
    for (widthSS=0; widthSS<tbuf_tlen; widthSS++) {
      GetTextExtentPoint32W(sf->hdc, tbuf_buffer, widthSS+1, &sz);
      poses_buffer[widthSS] = sz.cx;
    }

    ui = 0;
    for (i=0;i<len;) {
      if (IsDBCSLeadByteEx(sf->codePage, s[i])) {
        positions[i] = poses_buffer[ui];
        positions[i+1] = poses_buffer[ui];
        i += 2;
      } else {
        positions[i] = poses_buffer[ui];
        i++;
      }

      ui++;
    }
    VarBufferDel(poses);
    VarBufferDel(tbuf);
  }
}

int sfWidthChar(Surface* s, Font* font_, char ch) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  SIZE sz;
  sfSetFont(s, font_);
  GetTextExtentPoint32A(sf->hdc, &ch, 1, &sz);
  return sz.cx;
}

int sfAscent(Surface* s, Font* font_) {
  TEXTMETRIC tm;
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfSetFont(s, font_);
  GetTextMetrics(sf->hdc, &tm);
  return tm.tmAscent;
}

int sfDescent(Surface* s, Font* font_) {
  TEXTMETRIC tm;
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfSetFont(s, font_);
  GetTextMetrics(sf->hdc, &tm);
  return tm.tmDescent;
}

int sfInternalLeading(Surface* s, Font* font_) {
  TEXTMETRIC tm;
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfSetFont(s, font_);
  GetTextMetrics(sf->hdc, &tm);
  return tm.tmInternalLeading;
}

int sfExternalLeading(Surface* s, Font* font_) {
  TEXTMETRIC tm;
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfSetFont(s, font_);
  GetTextMetrics(sf->hdc, &tm);
  return tm.tmExternalLeading;
}

int sfHeight(Surface* s, Font* font_) {
  TEXTMETRIC tm;
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfSetFont(s, font_);
  GetTextMetrics(sf->hdc, &tm);
  return tm.tmHeight;
}

int sfAverageCharWidth(Surface* s, Font* font_) {
  TEXTMETRIC tm;
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sfSetFont(s, font_);
  GetTextMetrics(sf->hdc, &tm);
  return tm.tmAveCharWidth;
}

int sfSetPalette(Surface* s, Palette *pal, bool inBackGround) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  int changes = 0;
  if (sf->paletteOld) {
    SelectPalette(sf->hdc, sf->paletteOld, TRUE);
  }
  sf->paletteOld = 0;
  if (pal->allowRealization) {
    sf->paletteOld = SelectPalette(sf->hdc,
      (HPALETTE)(pal->hpal), inBackGround);
    changes = RealizePalette(sf->hdc);
  }
  return changes;
}

void sfSetClip(Surface* s, PRectangle rc) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  IntersectClipRect(sf->hdc, rc.left, rc.top, rc.right, rc.bottom);
}

void sfFlushCachedState(Surface* s) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sf->pen = 0;
  sf->brush = 0;
  sf->font = 0;
}

void sfSetUnicodeMode(Surface* s, bool unicodeMode_) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  sf->unicodeMode=unicodeMode_;
}

void sfSetDBCSMode(Surface* s, int codePage_) {
  SurfaceImpl* sf = (SurfaceImpl*)(s);
  // No action on window as automatically handled by system.
  sf->codePage = codePage_;
  sf->win9xACPSame = !IsNT() && ((unsigned int)sf->codePage == GetACP());
}

Surface* sfCreate() {
  SurfaceImpl* sf;
  static Surface_vtable vtable;
  static int inited=0;
  if (inited==0) {
    inited=1;
    vtable.sfRelease = sfRelease;
    vtable.sfMoveTo = sfMoveTo;
    vtable.sfLineTo = sfLineTo;
  }
  sf = (SurfaceImpl*)malloc(sizeof(SurfaceImpl));
  memset(sf, 0, sizeof(SurfaceImpl));
  sf->vtable = &vtable;
  // Windows 9x has only a 16 bit coordinate system so break after 30000 pixels
  sf->maxWidthMeasure = IsNT() ? 1000000 : 30000;
  // There appears to be a 16 bit string length limit in GDI on NT and a limit of
  // 8192 characters on Windows 95.
  sf->maxLenText = IsNT() ? 65535 : 8192;
  
  sf->codePage = 0;
  sf->win9xACPSame = false;
  sf->hdc = CreateCompatibleDC(NULL);
  sf->hdcOwned = true;
  SetTextAlign((HDC)(sf->hdc), TA_BASELINE);
  return (Surface*)sf;
}

static bool initialisedET = false;
static bool usePerformanceCounter = false;
static LARGE_INTEGER frequency;

ElapsedTime tStart() {
  ElapsedTime t;
  if (!initialisedET) {
    usePerformanceCounter = QueryPerformanceFrequency(&frequency) != 0;
    initialisedET = true;
  }
  if (usePerformanceCounter) {
    LARGE_INTEGER timeVal;
    QueryPerformanceCounter(&timeVal);
    t.bigBit = timeVal.HighPart;
    t.littleBit = timeVal.LowPart;
  } else {
    t.bigBit = clock();
  }
  return t;
}

double tDuration(ElapsedTime t) {
  double result;
  long endBigBit;
  long endLittleBit;
  double elapsed;

  if (usePerformanceCounter) {
    LARGE_INTEGER lEnd;
    LARGE_INTEGER lBegin;
    QueryPerformanceCounter(&lEnd);
    endBigBit = lEnd.HighPart;
    endLittleBit = lEnd.LowPart;
    lBegin.HighPart = t.bigBit;
    lBegin.LowPart = t.littleBit;
    elapsed = (double)(lEnd.QuadPart - lBegin.QuadPart);
    result = elapsed / (double)(frequency.QuadPart);
  } else {
    endBigBit = clock();
    endLittleBit = 0;
    elapsed = endBigBit - t.bigBit;
    result = elapsed / CLOCKS_PER_SEC;
  }
  return result;
}

DynamicLibrary dllLoad(const char *modulePath) {
  return (DynamicLibrary)LoadLibraryA(modulePath);
}

void dllFree(DynamicLibrary dll) {
  HMODULE h = (HMODULE)dll;
  if (h != NULL)
    FreeLibrary(h);
}

// Use GetProcAddress to get a pointer to the relevant function.
Function dllFindFunction(DynamicLibrary dll, const char *name) {
  HMODULE h = (HMODULE)dll;
  if (h != NULL) {
    // C++ standard doesn't like casts betwen function pointers and void pointers so use a union
    union {
      FARPROC fp;
      Function f;
    } fnConv;
    fnConv.fp = GetProcAddress(h, name);
    return fnConv.f;
  } else
    return NULL;
}

ColourDesired Platform_Chrome() {
  return GetSysColor(COLOR_3DFACE);
}

ColourDesired Platform_ChromeHighlight() {
  return GetSysColor(COLOR_3DHIGHLIGHT);
}

const char *Platform_DefaultFont() {
  return "Verdana";
}

int Platform_DefaultFontSize() {
  return 8;
}

unsigned int Platform_DoubleClickTime() {
  return GetDoubleClickTime();
}

bool Platform_MouseButtonBounce() {
  return false;
}

void Platform_DebugDisplay(const char *s) {
  OutputDebugStringA(s);
}

bool Platform_IsKeyDown(int key) {
  return (GetKeyState(key) & 0x80000000) != 0;
}

bool Platform_IsDBCSLeadByte(int codePage, char ch) {
  return IsDBCSLeadByteEx(codePage, ch) != 0;
}

int Platform_DBCSCharLength(int codePage, const char *s) {
  return (IsDBCSLeadByteEx(codePage, s[0]) != 0) ? 2 : 1;
}

int Platform_DBCSCharMaxLength() {
  return 2;
}

//#define TRACE

#ifdef TRACE
void Platform_DebugPrintf(const char *format, ...) {
  char buffer[2000];
  va_list pArguments;
  va_start(pArguments, format);
  vsprintf(buffer,format,pArguments);
  va_end(pArguments);
  Platform_DebugDisplay(buffer);
}
#else
void Platform_DebugPrintf(const char *format, ...) {}
#endif

static bool assertionPopUps = true;

bool Platform_ShowAssertionPopUps(bool assertionPopUps_) {
  bool ret = assertionPopUps;
  assertionPopUps = assertionPopUps_;
  return ret;
}

void Platform_Assert(const char *c, const char *file, int line) {
  char buffer[2000];
  sprintf(buffer, "Assertion [%s] failed at %s %d", c, file, line);
  if (assertionPopUps) {
    int idButton = MessageBoxA(0, buffer, "Assertion failure",
      MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);
    if (idButton == IDRETRY) {
      DebugBreak();
    } else if (idButton == IDIGNORE) {
      // all OK
    } else {
      abort();
    }
  } else {
    strcat(buffer, "\r\n");
    Platform_DebugDisplay(buffer);
    DebugBreak();
    abort();
  }
}

int Platform_Clamp(int val, int minVal, int maxVal) {
  if (val > maxVal)
    val = maxVal;
  if (val < minVal)
    val = minVal;
  return val;
}

void Platform_Initialise(void *hInstance) {
  OSVERSIONINFO osv = {sizeof(OSVERSIONINFO),0,0,0,0,TEXT("")};
  GetVersionEx(&osv);
  onNT = osv.dwPlatformId == VER_PLATFORM_WIN32_NT;
  InitializeCriticalSection(&crPlatformLock);
  hinstPlatformRes = (HINSTANCE)(hInstance);
  // This may be called from DllMain, in which case the call to LoadLibrary
  // is bad because it can upset the DLL load order.
  if (!hDLLImage) {
    hDLLImage = LoadLibrary(TEXT("Msimg32"));
  }
  if (hDLLImage) {
    AlphaBlendFn = (AlphaBlendSig)GetProcAddress(hDLLImage, "AlphaBlend");
  }
}

void Platform_Finalise() {
  DeleteCriticalSection(&crPlatformLock);
}
