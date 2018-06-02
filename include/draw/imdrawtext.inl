unsigned char G_font[354] = /* The 8x6 font */
{
#include "G_font.txt"
};
// Rendering a text string, no clipping is performed !
#define HW_SCREEN_LINE_SIZE (hDC->bw/sizeof(DWORD))
void G_text(HDDC hDC, int x, int y, char* txt, COLORREF colour)
{
  int nm, i, j;
  unsigned char cur;
  DWORD* adr, *l_adr;
  adr = &_Pixel(hDC, x, y);

  while (*txt != '\0') {
    /* until the end of the string */
    nm = ((*txt++) - ' '); /* starting from space */

    if (nm > 58) {
      nm -= ' '; /* to uppercase 58='Z'-' ' */
    }

    nm *= 6; /* 6 bytes per char */

    for (i = 0, l_adr = adr; i < 6; i++, l_adr += HW_SCREEN_LINE_SIZE) {
      cur = G_font[nm++];

      for (j = 0; j < 8; j++)
        if (cur & (0x80 >> j)) {
          *(l_adr + j) = colour;
        }
    }

    adr += 8; /* next word */
  }
}




SIZE TGetTextExtent(const TCHAR* strFontName, int cy, int cx, int bItalic, int bBold)
{
  SIZE sz = {0};
  return sz;
}

typedef struct draw_text_t {
  HDDC hDC;
  const char* s;
  int slen;
  POINT pt0;
  POINT pthit;
  DWORD clr, crbk; // 文字颜色 背景颜色
  DWORD* clrs;
  DWORD* crbks;
  SIZE sz;
  int max_cx;
  int charhit;// 输出 pthit 在哪个字符上
  int fmt;
  //int angle; // 0 向右 90 向上 180 向左 270 向下
} draw_text_t;

int draw_text_impl1(draw_text_t* d)
{
  HDDC hDC = d->hDC;
  int slen = d->slen;
  const char* s = d->s;
  const char* s0 = d->s;
  const char* e = s + slen;
  char* s2;
  RECT clip = hDC ? (hDC->clip ? *hDC->clip : iRECT(0, 0, hDC->w, hDC->h)) : iRECT(0, 0, 0, 0);
  int x0 = d->pt0.x, y0 = d->pt0.y;
  int x00 = x0, y00 = y0, max_x0 = x0;
  font_t* font;
  const uchar* mapA = 0;
  int depth;
  // 65级灰度表
  const uchar g_byAlphaLevel1[2] = {0, 255};
  uint nChar;
  int ww;
  int bSingleline = d->fmt & DT_SINGLELINE;
  font = zimo_songti_12;

  if (slen < 0) {
    slen = strlen(s0);
  }

  if (hDC && !bSingleline) {
    const char* e0 = e;
    int y1;

    for (; s < e0 && y0 + font->tmHeight < clip.top;) {
      for (; s < e0; ++s) {
        if ('\n' == *s) {
          y0 += font->tmHeight;
          ++s;
          break;
        }
      }
    }

    y1 = y0;
    e = s;

    for (; e < e0 && y0 - font->tm.tmHeight < clip.bottom;) {
      for (; e < e0; ++e) {
        if ('\n' == *e) {
          y0 += font->tmHeight;
          ++e;
          break;
        }
      }
    }

    y0 = y1;
  }

  for (; s < e; s = s2) {
    nChar = nextChar(s, &s2);
    ww = font->tm.tmAveCharWidth * (s2 - s) / 2;

    if (' ' == nChar) {
      if (hDC) {
        int i = s - s0;
        DWORD crbk = d->crbks ? d->crbks[i] : d->crbk;
        DWORD clr = d->clrs ? d->clrs[i] : d->clr;

        if (GetAV(crbk)) {
          draw_fillrect(hDC, x0, y0, x0 + font->tm.tmAveCharWidth, y0 + font->tm.tmHeight, crbk);
        }
      }

      x0 += font->tm.tmAveCharWidth;
    }
    else if ('\n' == nChar && !bSingleline) {
      y0 += font->tm.tmHeight;
      x0 = x00;

      if (y0 < d->pthit.y) {
        d->charhit = s2 - s0;
      }

      continue;
    }
    else if (nChar != 13) {
      glyph_info* gi = FontGlyphOutline(nChar);

      if (NULL == gi) {
        x0 += font->tm.tmAveCharWidth;
        max_x0 = MAX(max_x0, x0);
        continue;
      }

      if (hDC) {
#define BITALIGN(x, n) (((x)+n-1)&(~(n-1)))
        int y2 = y0 + font->tm.tmAscent - gi->gm.gmptGlyphOrigin.y, x2 = x0 + gi->gm.gmptGlyphOrigin.x;
        int pitch = BITALIGN(gi->gm.gmBlackBoxX * depth, 32) >> 3;
        int i = s - s0;
        DWORD crbk = d->crbks ? d->crbks[i] : d->crbk;
        DWORD clr = d->clrs ? d->clrs[i] : d->clr;

        if (GetAV(crbk)) {
          draw_fillrect(hDC, x0, y0, x0 + gi->gm.gmCellIncX, y0 + font->tm.tmHeight, crbk);
        }

        draw_bit(hDC, x2, y2, gi->gm.gmBlackBoxX, gi->gm.gmBlackBoxY, gi->bit, pitch, depth, clr, mapA, 0);
      }

      x0 += gi->gm.gmCellIncX;
    }

    if (y0 < d->pthit.y && x0 - ww < d->pthit.x) {
      d->charhit = s2 - s0 + (s2 - s) * ((x0 + ww) < d->pthit.x);
    }

    max_x0 = MAX(max_x0, x0);
  }

  y0 += font->tm.tmHeight;

  d->sz.w = x0 - x00;
  d->sz.h = y0 - y00;
  d->max_cx = max_x0 - x00;

  if (d->pthit.x > 0) {
    d->charhit = text_hit(font, d->s, d->slen, d->pthit);
  }

  return 0;
}


#define FontGetTextExtent(s) FontGetTextSize(s, strlen(s))
#define PosFromChar(S, SLEN) FontGetTextSize(S, SLEN).w
#define CharFromPos(S, SLEN, ptx, pty) draw_text_impl(0, S, SLEN, 0, 0, 0, 0, ptx, pty)
#define FontHeight()     FontGetTextSize("H", 1).h
// 获得字符串尺寸
SIZE FontGetTextSize(const char* s, int slen)
{
  SIZE sz = {0, 0};
  draw_text_impl(0, s, slen, 0, 0, 0, &sz, 0, 0);
  return sz;
}

// outsz 获取字符尺寸
// pix 由坐标获取字符位置
#define draw_text_out(hDC, x0, y0, s) draw_text_impl(hDC, s, -1, x0, y0, _clr, 0, 0)

#define TTextOut(hDC, x, y, text) draw_text_impl(hDC, (text).s, (text).l, x, y, _clr, 0, 0, 0)
#define TTextOut2(hDC, x, y, text) draw_text_impl(hDC, (text), strlen(text), x, y, _clr, 0, 0, 0)
#define TDrawText(hDC, lpString, lpRect, uFormat) drawaa_text(hDC, (lpRect)->left, (lpRect)->top, RCWIDTH(lpRect), RCHEIGHT(lpRect), lpString, strlen(lpString), uFormat, _RGB(0,0,0))
#define TDrawText2(hDC, lpString, nLen, lpRect, uFormat, clr) DrawText11(hDC, (lpRect)->left, (lpRect)->top, RCWIDTH(lpRect), RCHEIGHT(lpRect), lpString, nLen, uFormat|FONT_ANTIALIAS, clr)

// typedef struct _GLYPHMETRICS {
// uint gmBlackBoxX; //指定完全包围字体结构的最小矩阵的宽度
// uint gmBlackBoxY; //指定完全包围字体结构的最小矩阵的高度
// POINT gmptGlyphOrigin; //指定完全包围字体结构的最小矩阵左上角的点坐标
// short gmCellIncX; //指定当前的起点到下一个字符的起点的水平距离
// short gmCellIncY; //...垂直距离
// } GLYPHMETRICS, *LPGLYPHMETRICS;
#define draw_textPt(hDC, s, slen, x, y, uF, clr) draw_text(hDC, s, slen, (int)x, (int)y, (int)x+1300, (int)y+300, uF, clr)
#define draw_textR(hDC, s, slen, prc, uF, clr) draw_text(hDC, s, slen, (prc)->left, (prc)->top, (prc)->right, (prc)->bottom, uF, clr)
#define draw_textR2(hDC, s, rc, uF) draw_textR(hDC, s, -1, &(rc), uF, _clr)
int draw_text(HDDC hDC, const char* s, int slen, int x0, int y0, int x1, int y1, uint uFormat, DWORD clr)
{
  RECT rc1, rc = {x0, y0, x1, y1};
  SIZE sz;
  slen = (slen < 0) ? strlen(s) : slen;
  sz = FontGetTextSize(s, slen);
  RectAlign(rc, sz.w, sz.h, uFormat, &rc1);
  HDDC_BEGIN(hDC, &rc);
  draw_text_impl(hDC, s, slen, rc1.left, rc1.top, clr, 0, 0, 0);
  HDDC_END(hDC);
  return 0;
}

int nextLine(const char* s, int l, char** ps)
{
  const char* s0 = s;
  const char* s2 = s;
  const char* e = s + l;

  for (; s < e; ++s) {
    if ('\n' == *s) {
      *ps = (char*)s + 1;
      return 1;
    }
  }

  return 0;
}
// maxlinewidth 每行最大宽度
int nextLineWrap(const char* s, int l, font_t* f, int maxlinewidth, char** ps)
{
  const char* s0 = s;
  const char* s2 = s;
  const char* e = s + l;
  int x = 0, w = f->tm.tmAveCharWidth;

  for (; s < e; s = s2) {
    glyph_info* gi;
    int cw = w, nChar = nextChar(s, ps);

    if ('\n' == nChar) {
      return 1;
    }

    gi = FontGlyphOutline(nChar);

    if (gi) {
      cw = gi->gm.gmCellIncX;
    }

    if (x + cw > maxlinewidth) {
      return 1;
    }

    x += cw;
  }

  return 0;
}

int text_hit(font_t* f, const char* s, int l, POINT pt)
{
  const char* s0 = s;
  const char* s2 = s;
  const char* e = s + l;
  int x = 0, y = 0, h = f->tm.tmHeight, w = f->tm.tmAveCharWidth;

  if (h < pt.y) {
    for (; s < e; ++s) {
      if ('\n' == *s) {
        y += h;

        if (y + h >= pt.y) {
          ++s;
          break;
        }
      }
    }
  }

  for (; s < e && '\n' != *s; s = s2) {
    int cw = w, nChar = nextChar(s, (char**)&s2);
    glyph_info* gi = FontGlyphOutline(nChar);

    if (gi) {
      cw = gi->gm.gmCellIncX;
    }

    if (x + cw / 2 > pt.x) {
      break;
    }

    x += cw;
  }

  return s - s0;
}

