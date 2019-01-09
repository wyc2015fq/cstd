
#include "sys\gdi.inl"

// font styles, these map to GDI+ FontStyle
enum FontStyles {
  FONT_REG = 0,
  FONT_BOLD = 1,
  FONT_ITALIC = 2,
  FONT_BOLDITALIC = 3,
  FONT_UNDERLINE = 4,
  FONT_STRIKEOUT = 8
};
typedef struct glyph_info {
  GLYPHMETRICS gm;
  BYTE* bit;
} glyph_info;
enum {CHARMAX = 1 << 16};
typedef struct font1_t {
  char fontname[32];
  int size;
  UINT uStyle;
  UINT uFormat;
  HFONT font;
  TEXTMETRIC tm;
  glyph_info* glyph[CHARMAX];
} font1_t;
#define FONTINFO_EQ(FI, _N, _SZ, _SY) (0==strcmp((FI)->fontname, _N) && ((FI)->size==_SZ) && ((FI)->uStyle==_SY) )
#define MAXFONT 10
#define MAXIMAGE 100
#define SelectFontToMemDC(lpsFontName, nFaceSize, uStyle) TSelectFont(font_create(lpsFontName, nFaceSize, uStyle))
#define GetCurFontId() (ASSERT(g_res.g_CurFont>=0 && g_res.g_CurFont<CHARMAX), g_res.g_CurFont)
#define GetCurFont()   (g_res.g_hFontInfo[GetCurFontId()])

#define GetImageRef(_I)   (ASSERT((_I)>=0 && (_I)<g_res.nref), g_res.ref+(_I))
typedef struct resimg_t {
  img_t* img;
  int nimg;
  img_t* ref;
  int nref;
  // GDI设备
  HDC hMemDC;

  font1_t* g_hFontInfo[MAXFONT];
  int g_CurFont;
} resimg_t;

resimg_t g_res = {0};

int TSelectFont(int font)
{
  int oldfont = g_res.g_CurFont;
  g_res.g_CurFont = font;
  return oldfont;
}
int font_free(font1_t* f)
{
  int j;

  if (f) {
    if (f->font) {
      DeleteObject(f->font);
    }

    for (j = 0; j < CHARMAX; ++j) {
      if (f->glyph[j]) {
        free(f->glyph[j]);
      }
    }

    free(f);
  }

  return 0;
}
void res_free(void)
{
  int i;

  for (i = 0; i < MAXFONT; ++i) {
    font_free(g_res.g_hFontInfo[i]);
    g_res.g_hFontInfo[i] = 0;
  }

  memset(g_res.g_hFontInfo, 0, sizeof(g_res.g_hFontInfo));

  if (g_res.hMemDC) {
    DeleteDC(g_res.hMemDC);
    g_res.hMemDC = 0;
  }

  if (g_res.img) {
    freeims(g_res.img, g_res.nimg);
    free(g_res.img);
    g_res.img = 0;
    g_res.nimg = 0;
  }

  if (g_res.ref) {
    free(g_res.ref);
    g_res.ref = 0;
    g_res.nref = 0;
  }
}
UINT nextChar(const char* s, char** ps)
{
  UINT c;

  if (*s < 0) {
    unsigned char* u = (unsigned char*)s;
    c = u[1] | (u[0] << 8);
    *ps = (char*)s + 2;
  }
  else {
    c = *s;
    *ps = (char*)s + 1;
  }

  return c;
}

static void SetLogFont(LOGFONTA* lf, const char* faceName, int characterSet, int size, BOOL bold, BOOL italic, int nQuality)
{
  memset(lf, 0, sizeof(*lf));
  // The negative is to allow for leading
  lf->lfHeight = -(abs(size));
  lf->lfWeight = bold ? FW_BOLD : FW_NORMAL;
  lf->lfItalic = (BYTE)(italic ? 1 : 0);
  lf->lfCharSet = (BYTE)(characterSet);
  lf->lfQuality = nQuality;
  strncpy(lf->lfFaceName, faceName, sizeof(lf->lfFaceName));
}

int GetLogFont(HFONT hFont, LOGFONT* pLogFont)
{
  return GetObject(hFont, sizeof(LOGFONT), pLogFont);
}

#if 0
// 从文件载入字体
BOOL LoadFontFromFile(const char* fname)
{
  int dwLen;
  char* pData = 0;
  pData = loaddata0(fname, &dwLen);

  if (pData) {
    DWORD dwFonts = 0;
    HANDLE m_hFont = AddFontMemResourceEx(pData, dwLen, NULL, &dwFonts);
    free(pData);
    return TRUE;
  }

  return FALSE;
}

HFONT loadfont(const char* resname)
{
  buf_t bf[1] = {0};
  DWORD nFonts;
  HFONT m_hFont;
  loadbuf(resname, bf);
  m_hFont = (HFONT)AddFontMemResourceEx(bf->data, bf->len, NULL, &nFonts);
  bffree(bf);
  return m_hFont;
}
#endif

int LogPixelsY()
{
  HDC hMemDC = GetMemDC();
  return GetDeviceCaps(hMemDC, LOGPIXELSY);
}

int font_set(int i, const char* lpsFontName, int nFaceSize, UINT uStyle)
{
  font1_t* f = 0;
  int g_font_antialias = nFaceSize > 20;
  HDC hMemDC = GetMemDC();
  i = i % MAXFONT;
  //SetGFont(i, lpsFontName, nFaceSize, bBold, bItalic);

  if (g_res.g_hFontInfo[i]) {
    font_free(g_res.g_hFontInfo[i]);
    g_res.g_hFontInfo[i] = 0;
  }

  f = GMALLOC(font1_t, 1);
  memset(f, 0, sizeof(font1_t));
  ASSERT(i >= 0 && i < MAXFONT);
  {
    int nHeight = nFaceSize;
    BOOL bBold = uStyle == FONT_BOLD || uStyle == FONT_BOLDITALIC;
    BOOL bItalic = uStyle == FONT_ITALIC || uStyle == FONT_BOLDITALIC;
    BOOL bUnderline = uStyle == FONT_UNDERLINE;

    //nHeight=MulDiv(nFaceSize, GetDeviceCaps(GetMemDC(), LOGPIXELSY),72);
    if (f->font) {
      DeleteObject(f->font);
    }

#if 1
    f->font = CreateFont(-nHeight, 0, 0, 0, (bBold) ? FW_BOLD : FW_NORMAL, bItalic,
        bUnderline, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE | DEFAULT_PITCH, lpsFontName);
#else
    {
      LOGFONTA lf;
      int ly = LogPixelsY();
      int sz = MulDiv(12, ly, 72);
      SetLogFont(&lf, lpsFontName, DEFAULT_CHARSET, -nHeight, bBold, bItalic, DEFAULT_QUALITY);
      //hash = HashFont(faceName_, characterSet_, size_, bold_, italic_, extraFontFlag_);
      f->font = CreateFontIndirectA(&lf);
    }
#endif
    strcpy(f->fontname, lpsFontName);
    f->size = nFaceSize;
    f->uStyle = uStyle;
    SelectObject(hMemDC, f->font);
    GetTextMetrics(hMemDC, &f->tm);
    f->uFormat = g_font_antialias ? GGO_GRAY8_BITMAP : GGO_BITMAP;
    //f->uFormat = GGO_GRAY8_BITMAP;
    g_res.g_hFontInfo[i] = f;
  }
  return i;
}

int font_setstr(int i, str_t s)
{
  char lpsFontName[LF_FACESIZE];
  int nFaceSize = 0, uStyle = 0;
  str_t s1;
  s1 = STRsplit(s, ',', " \t", &s);
  STRncpy(lpsFontName, LF_FACESIZE, s1);
  s1 = STRsplit(s, ',', " \t", &s);
  nFaceSize = -atoi(s1.s);

  for (; s.l > 0;) {
    char ch;
    s1 = STRsplit(s, ',', " \t", &s);
    ch = tolower(*s1.s);

    switch (ch) {
    case 'b':
      uStyle |= FONT_BOLD;
      break;

    case 'i':
      uStyle |= FONT_ITALIC;
      break;

    case 'u':
      uStyle |= FONT_UNDERLINE;
      break;

    default:
      assert(0);
      break;
    }
  }

  return font_set(i, lpsFontName, nFaceSize, uStyle);
}
int font_create(const char* lpsFontName, int nFaceSize, UINT uStyle)
{
  int i;

  for (i = 0; i < MAXFONT && g_res.g_hFontInfo[i] && g_res.g_hFontInfo[i]->font; ++i) {
    if (g_res.g_hFontInfo[i] && FONTINFO_EQ(g_res.g_hFontInfo[i], lpsFontName, nFaceSize, uStyle)) {
      return i;
    }
  }

  return font_set(i, lpsFontName, nFaceSize, uStyle);
}
glyph_info* FontGlyphOutline(UINT uChar)
{
  static const MAT2 mat2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
  glyph_info* gi = GetCurFont()->glyph[uChar];
  HDC hMemDC = GetMemDC();
  ASSERT(uChar < CHARMAX);

  if (NULL == gi) {
    int nLen;
    GLYPHMETRICS gm;
    ASSERT(GetCurFont()->font);
    SelectObject(hMemDC, GetCurFont()->font);
    nLen = GetGlyphOutline(hMemDC, uChar, GetCurFont()->uFormat, &gm, 0, 0, &mat2);

    if (nLen > 0) {
      gi = (glyph_info*)malloc(nLen + sizeof(glyph_info));
      gi->bit = (BYTE*)(gi + 1);
      nLen = GetGlyphOutline(hMemDC, uChar, GetCurFont()->uFormat, &gi->gm, nLen, gi->bit, &mat2);
      GetCurFont()->glyph[uChar] = gi;
    }
  }

  return gi;
}

//汉字笔画计算代码
//通过提取楷体的字形轮廓可以获得汉字笔画。
//只不过选用的字体必须是楷体。其他字体不行。
//运行结果： "疆"的笔画19
int GetCharStrokes(UINT ch)
{
  HFONT hFont;
  HDC hDC;
  HGDIOBJ hOldFont;
  MAT2 mat;
  GLYPHMETRICS gm;
  LOGFONT lfFont;
  DWORD dwSize;
  int cnt = 0;
  LPVOID pBuff = NULL;

  // create font;
  memset(&lfFont, 0, sizeof(lfFont));
  lstrcpy(lfFont.lfFaceName, _T("楷体_GB2312"));
  lfFont.lfHeight = 72;
  lfFont.lfWeight = FW_NORMAL;
  lfFont.lfItalic = FALSE;

  lfFont.lfStrikeOut = FALSE;
  lfFont.lfCharSet = DEFAULT_CHARSET;
  lfFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
  lfFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  lfFont.lfQuality = DRAFT_QUALITY;
  lfFont.lfPitchAndFamily = DEFAULT_PITCH;

  hFont = CreateFontIndirect(&lfFont);

  hDC = GetDC(NULL);

  if (hDC == NULL) {
    return 0;
  }

  hOldFont = SelectObject(hDC, hFont);

  memset(&mat, 0, sizeof(mat));
  mat.eM11.value = 1;
  mat.eM22.value = -1;

  memset(&gm, 0, sizeof(gm));

  dwSize = GetGlyphOutline(hDC, ch, GGO_NATIVE, &gm, 0, NULL, &mat);

  if (dwSize > 0) {
    DWORD res;
    pBuff = malloc(dwSize);
    memset(&gm, 0, sizeof(gm));

    res = GetGlyphOutline(hDC, ch, GGO_NATIVE, &gm, dwSize, pBuff, &mat);

    if (res != dwSize) {
      free(pBuff);
      pBuff = NULL;
    }
  }

  if (pBuff) {
    BYTE* ptr = (BYTE*) pBuff;

    while (dwSize > 0) {
      TTPOLYGONHEADER* header = (TTPOLYGONHEADER*) ptr;
      cnt ++;
      ptr += header->cb;
      dwSize -= header->cb;
    }

    free(pBuff);
  }

  SelectObject(hDC, hOldFont);
  ReleaseDC(NULL, hDC);

  return cnt;
}

int font_GetFont(HDC hDC, int gBits, UINT chartemp, GLYPHMETRICS* glpm, unsigned char* pBuf, int nMaxLen)
{
  //if(m_strFontName.GetLength()>0)
  MAT2 mmat2 = { 0, 1, 0, 0, 0, 0, 0, 1 };
  int nLen, nLentemp = 0;

  //TRACE("chartemp is %d, sizeof(uint) is %d\n", chartemp, sizeof(UINT));

  nLen = GetGlyphOutline(hDC, chartemp, gBits, glpm, 0, NULL, &mmat2);

  ASSERT(nLen <= nMaxLen);

  //GGO_BITMAP 1
  // GGO_GRAY2_BITMAP   4
  // GGO_GRAY4_BITMAP   5
  //GGO_GRAY8_BITMAP   6
  if (nLen > 0) {
    nLentemp = GetGlyphOutline(hDC, chartemp, gBits, glpm, nLen, pBuf, &mmat2);
  }
  else {
    //TRACE("字体过大！不支持！\n");
  }

  return nLentemp;
}

int print_chrbit_txt(FILE* pf, int h, int w, const uchar* bit0, int step)
{
  int j, k, n = 0;

  for (j = 0; j < h; ++j) {
    const uchar* bit = bit0 + j * step;

    for (k = 0; k < w; k ++) {
      fprintf(pf, "0x%02X,", bit[k]);
      ++n;
    }
  }

  return n;
}

#include "font/font.inl"
// 生成汉字字模
BOOL make_zimo(const char* fname, const char* lpsFontName, int nHeight, int nWidth, int bItalic, int bBold)
{
  HDC hDC;
  HFONT hOldFont, hFont;
  int i, j, nAngle = 0, bUnderline = 0;
  unsigned char buf[1024];
  GLYPHMETRICS glpm[1] = {0};
  hDC = GetDC(0);

  //m_intAngle = -190;
  if (NULL == lpsFontName) {
    LOGFONT lf;
    SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    //lpsFontName = lf.lfFaceName;
    lf.lfHeight *= 2;
    nHeight = lf.lfHeight;
    hFont = (HFONT)CreateFontIndirect(&lf);
    //hFont = (HFONT)GetStockObject(SYSTEM_FONT);
  }
  else {
    hFont = CreateFont(-nHeight, 0, 0, 0, (bBold) ? FW_BOLD : FW_NORMAL, bItalic,
        bUnderline, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE | DEFAULT_PITCH, lpsFontName);
  }

  hOldFont = SelectObject(hDC, hFont);
  {
    TEXTMETRIC tm;
    FILE* pf = NULL;
    int stp = (nHeight + 7) / 8;
    font_t fo[1] = {0};
    int ppp = nHeight * stp;
    GetTextMetrics(hDC, &tm);
    fo->as = 33, fo->ae = 126;
    fo->hs = 0xb0, fo->he = 0xf7;
    fo->ls = 0xa1, fo->le = 0xfe;
    fo->sz = ppp + 4;
    fo->h = nHeight;
    fo->bl = (uchar)tm.tmAscent;
    pf = fopen(fname, "wb");

    fprintf(pf, "%d,%d,\n", fo->sz, fo->h);
    fprintf(pf, "%d,\n", fo->bl);
    fprintf(pf, "0x%02X,0x%02X,\n", fo->as, fo->ae);
    fprintf(pf, "0x%02X,0x%02X,\n", fo->hs, fo->he);
    fprintf(pf, "0x%02X,0x%02X,\n", fo->ls, fo->le);

    //fprintf(pf, "%d, %d, ", glpm->gmBlackBoxY, glpm->gmBlackBoxX);
    for (i = fo->as; i <= fo->ae; ++i) {
      int ch = i;
      char chr[3] = {i, 0, 0};
      int charLineW, len, k;
      memset(glpm, 0, sizeof(*glpm));
      len = font_GetFont(hDC, GGO_BITMAP, ch, glpm, buf, countof(buf));
      charLineW = (glpm->gmBlackBoxX / 32 + (glpm->gmBlackBoxX % 32 == 0 ? 0 : 1)) * 4;
      //print_chrbit2(0, glpm->gmBlackBoxY, glpm->gmBlackBoxX, buf, charLineW),  printf("\n");
      //ASSERT(len==(int)(glpm->gmBlackBoxY*charLineW));
      fprintf(pf, "/*%2s %04X*/", chr, ch);
      fprintf(pf, "%2d,", glpm->gmBlackBoxX);
      fprintf(pf, "%2d,", glpm->gmBlackBoxY);
      fprintf(pf, "%2d,", glpm->gmptGlyphOrigin.x);
      fprintf(pf, "%2d,", glpm->gmptGlyphOrigin.y);
      //fprintf(pf, "%2d,", glpm->gmCellIncY);
      //fprintf(pf, "%2d,", glpm->gmCellIncX);
      k = print_chrbit_txt(pf, glpm->gmBlackBoxY, (glpm->gmBlackBoxX + 7) / 8, buf, charLineW);

      for (; k < ppp; ++k) {
        fprintf(pf, "0x%02X,", 0);
      }

      fprintf(pf, "\n");
    }

    for (i = fo->hs; i <= fo->he; ++i) {
      for (j = fo->ls; j <= fo->le; ++j) {
        int ch = (i << 8) | (j & 0xff);
        char chr[3] = {i, j, 0};
        int charLineW, len, k;
        len = font_GetFont(hDC, GGO_BITMAP, ch, glpm, buf, countof(buf));
        charLineW = (glpm->gmBlackBoxX / 32 + (glpm->gmBlackBoxX % 32 == 0 ? 0 : 1)) * 4;
        //print_chrbit2(0, glpm->gmBlackBoxY, glpm->gmBlackBoxX, buf, charLineW),  printf("\n");
        ASSERT(len == (int)(glpm->gmBlackBoxY * charLineW));
        fprintf(pf, "/*%2s %04X*/", chr, ch);
        fprintf(pf, "%2d,", glpm->gmBlackBoxX);
        fprintf(pf, "%2d,", glpm->gmBlackBoxY);
        fprintf(pf, "%2d,", glpm->gmptGlyphOrigin.x);
        fprintf(pf, "%2d,", glpm->gmptGlyphOrigin.y);
        //fprintf(pf, "%2d,", glpm->gmCellIncY);
        //fprintf(pf, "%2d,", glpm->gmCellIncX);
        k = print_chrbit_txt(pf, glpm->gmBlackBoxY, (glpm->gmBlackBoxX + 7) / 8, buf, charLineW);

        for (; k < ppp; ++k) {
          fprintf(pf, "0x%02X,", 0);
        }

        fprintf(pf, "\n");
      }
    }

    fclose(pf);
  }
  DeleteObject(hFont);
  SelectObject(hDC, hOldFont);
  ReleaseDC(0, hDC);
  return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}
// 文本
int GetCharGlyph(UINT thisChar, double x, double y, GLYPHMETRICS* gm, DPOINT* pt, int* lens)
{
  MAT2 mat2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
  char buffer[1 << 12];
  HDC hMemDC = GetMemDC();
  int n = 0;
  DPOINT* pt0 = pt;
  DWORD returnBytes = GetGlyphOutlineA(hMemDC, thisChar, GGO_NATIVE, gm, sizeof(buffer), buffer, &mat2);

  if (returnBytes != GDI_ERROR) {
    TTPOLYGONHEADER* pTTPH = (TTPOLYGONHEADER*)buffer;

    //ASSERT(pTTPH->dwType == TT_POLYGON_TYPE);
    if (' ' == thisChar) {
      int sdf = 0;
    }

    //对闭合路径进行循环
    while (returnBytes > 0) {
      TTPOLYCURVE* pc = (TTPOLYCURVE*)(pTTPH + 1);
      int i, count, remainBytes = pTTPH->cb - sizeof(TTPOLYGONHEADER);
      DPOINT* pt1 = pt0;
      *pt0++ = dPOINT(x + fx_to_dbl(pTTPH->pfxStart.x), y - fx_to_dbl(pTTPH->pfxStart.y));

      while (remainBytes > 0) {
        switch (pc->wType) {
        case TT_PRIM_LINE: {
          for (i = 0; i < pc->cpfx; ++i) {
            *pt0++ = dPOINT(x + fx_to_dbl(pc->apfx[i].x), y - fx_to_dbl(pc->apfx[i].y));
          }
        }
        break;

        case TT_PRIM_QSPLINE:
        case TT_PRIM_CSPLINE: {
#if 1
          DPOINT p0 = pt0[-1];//dPOINT(x + fx_to_dbl(pc->apfx[0].x), y+tm.tmAscent - fx_to_dbl(pc->apfx[0].y));

          for (i = 0; i < pc->cpfx - 1; ++i) {
            DPOINT p1 = dPOINT(x + fx_to_dbl(pc->apfx[i].x), y - fx_to_dbl(pc->apfx[i].y));
            DPOINT p2 = dPOINT(x + fx_to_dbl(pc->apfx[i + 1].x), y - fx_to_dbl(pc->apfx[i + 1].y));

            // If not on last spline, compute C
            if (i < pc->cpfx - 2) {
              p2 = dPOINT((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
            }

            pt0--;
            pt0 += vcgen_curve3_div(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, pt0);
            p0 = p2;
          }

#else

          for (i = 0; i < pc->cpfx; ++i) {
            *pt0++ = dPOINT(x + fx_to_dbl(pc->apfx[i].x), y - fx_to_dbl(pc->apfx[i].y));
          }

#endif
        }
        break;
        }

        count = sizeof(TTPOLYCURVE) + (pc->cpfx - 1) * sizeof(POINTFX);
        pc = (TTPOLYCURVE*)((char*)pc + count);
        remainBytes -= count;
        //ASSERT(pt0-pt < maxpt);
      }

      lens[n++] = pt0 - pt1;
      returnBytes -= pTTPH->cb;
      pTTPH = (TTPOLYGONHEADER*)((char*)pTTPH + pTTPH->cb);
    }
  }

  lens[n] = pt0 - pt;
  return n;
}
int GetTextGlyph(const char* s, int slen, double x, double y, DPOINT* pt, int* lens)
{
  GLYPHMETRICS gm;
  TEXTMETRIC tm;
  int n, i = 0;
  DPOINT* pt0 = pt;
  const char* e = s + slen;
  char* s2;
  HDC hMemDC = GetMemDC();
  GetTextMetrics(hMemDC, &tm);

  for (; s < e; s = s2) {
    UINT thisChar = nextChar(s, &s2);
    n = GetCharGlyph(thisChar, x, y + tm.tmAscent, &gm, pt0, lens + i);
    i += n;
    pt0 += lens[i];
    x += gm.gmCellIncX;
  }

  return i;
}

