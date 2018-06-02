#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "inc/GdiPlus_c.h"
//#include "E:\code\c\Graphics\GDI+\gdiplus20071007\include\Gdiplus_c.h"
#pragma comment(lib,"GdiPlus.lib")
typedef struct {
  ULONG_PTR token;
  GpGraphics* window;
  GpGraphics* memory;
  //GpPath* path;
  dpath_t path[1];
  GpImage* bitmap;
  GpPen* pen;
  GpBrush* brush;
  GpStringFormat* format;
  float x, y;
  BOOL ismoveed;
  int textAlign;
} imui_gdip_t;
#define gdip_color(clr) (clr)
//IM_STATIC void gdip_scissor(imui_gdip_t* g, float x, float y, float w, float h){ GdipSetClipRect(g->memory, (INT)x, (INT)y, (INT)(w + 1), (INT)(h + 1), CombineModeReplace);}
#define TEXT_LEN(text, text_end) (text_end ? text_end-text : strlen(text))
int my_mb2wc(unsigned short** ppw, const char* str, int len)
{
  int wlen = 0;
  len = len > 0 ? len : strlen(str);
  if (len > 0) {
    wlen = MultiByteToWideChar(CP_ACP, 0, str, len, NULL, 0);
    myrealloc((void**)ppw, (wlen + 1)*sizeof(short), __FILE__, __LINE__);
    wlen = MultiByteToWideChar(CP_ACP, 0, str, len, *ppw, wlen);
    (*ppw)[wlen] = 0;
  }
  return wlen;
}
GpPath* gdip_GetStringPath(GpPath* path, WORD* s, int len, float dpi, const RectF* rect, GpFont* font, GpStringFormat* format)
{
  GpFontFamily* family = NULL;
  float emSize = 0;
  INT style;
  // Convert font size into appropriate coordinates
  GdipGetFontSize(font, &emSize);
  GdipGetFamily(font, &family);
  GdipGetFontStyle(font, &style);
  GdipAddPathString(path, s, len, family, style, emSize, rect, format);
  return path;
}
IM_STATIC int gdip_DrawStringStroke(GpGraphics* g, WORD* s, int len, GpPen* pen, GpBrush* brush, const RectF* rect, GpFont* font, GpStringFormat* format)
{
  float dpi = 0;
  GpPath* path = NULL;
  GdipCreatePath(FillModeAlternate, &path);
  GdipGetDpiY(g, &dpi);
  gdip_GetStringPath(path, s, len, dpi, rect, font, format);
  //ÒõÓ°´úÂë
  //RectangleF off = rect;
  //off.Offset(5, 5);//ÒõÓ°Æ«ÒÆ
  //using (GraphicsPath offPath = gdip_GetStringPath(s, dpi, off, font, format))
  //{
  // Brush b = new SolidBrush(Color.FromArgb(100, 0, 0, 0));
  // g.FillPath(b, offPath);
  // b.Dispose();
  //}
  // g->SmoothingMode = SmoothingMode.AntiAlias;//ÉèÖÃ×ÖÌåÖÊÁ¿
  if (pen) {
    GdipDrawPath(g, pen, path);//»æÖÆÂÖÀª£¨Ãè±ß£©
  }
  if (brush) {
    GdipFillPath(g, brush, path);//Ìî³äÂÖÀª£¨Ìî³ä£©
  }
  GdipDeletePath(path);
  return 0;
}
IM_STATIC int gdip_DeleteFont(ImGuiHost* host, ImFont* f)
{
  GdipDeleteFont((GpFont*)f->font);
  f->font = NULL;
  return 0;
}
IM_STATIC int gdip_StrFormatTrans(int formatFlags, int* pAlign, int* pVAlign)
{
  int Fmt = 0, Align = 0, VAlign = 0;
#define FLAGSET(x, a, b) if (formatFlags&a) {x|=b;}
  FLAGSET(Fmt, TF_RightToLeft, StringFormatFlagsDirectionRightToLeft);
  FLAGSET(Fmt, TF_Vertical, StringFormatFlagsDirectionVertical);
  FLAGSET(Fmt, TF_NoFitBlackBox, StringFormatFlagsNoFitBlackBox);
  FLAGSET(Fmt, TF_DisplayFormatControl, StringFormatFlagsDisplayFormatControl);
  FLAGSET(Fmt, TF_NoFontFallback, StringFormatFlagsNoFontFallback);
  FLAGSET(Fmt, TF_MeasureTrailingSpaces, StringFormatFlagsMeasureTrailingSpaces);
  FLAGSET(Fmt, TF_NoWrap, StringFormatFlagsNoWrap);
  FLAGSET(Fmt, TF_LineLimit, StringFormatFlagsLineLimit);
  FLAGSET(Fmt, TF_NoClip, StringFormatFlagsNoClip);
  FLAGSET(Fmt, TF_WrapWidgetWidth, 0);
  FLAGSET(Fmt, TF_WrapFixedColumnWidth, 0);
  FLAGSET(Fmt, TF_WrapAtWhiteSpace, 0);
  FLAGSET(Fmt, TF_WrapAnyWhere, 0);
  FLAGSET(Align, TF_LEFT, StringAlignmentNear);
  FLAGSET(Align, TF_CENTER, StringAlignmentCenter);
  FLAGSET(Align, TF_RIGHT, StringAlignmentFar);
  FLAGSET(VAlign, TF_TOP, StringAlignmentNear);
  FLAGSET(VAlign, TF_VCENTER, StringAlignmentCenter);
  FLAGSET(VAlign, TF_BOTTOM, StringAlignmentFar);
#undef FLAGSET
  *pAlign = Align;
  *pVAlign = VAlign;
  return Fmt;
}
IM_STATIC GpStringFormat* gdip_CreateStrFormat(int formatFlags)
{
  GpStringFormat* nativeFormat = NULL;
  int align = 0, valign = 0;
  int Fmt = gdip_StrFormatTrans(formatFlags, &align, &valign);
  GdipCreateStringFormat(Fmt, LANG_NEUTRAL, &nativeFormat);
  GdipSetStringFormatAlign(nativeFormat, (StringAlignment)align);
  GdipSetStringFormatLineAlign(nativeFormat, (StringAlignment)valign);
  return nativeFormat;
}
IM_STATIC int gdip_CreateFont(ImGuiHost* host, ImFont* f, const char* name, int size)
{
  GpFontFamily* family = NULL;
  WCHAR wname[64] = {0};
  GpFont* font = NULL;
  int wsize = MultiByteToWideChar(CP_ACP, 0, name, -1, wname, 64);
  GpStatus ret;
  wname[wsize] = 0;
  size = abs(size);
  ret = GdipCreateFontFamilyFromName(wname, NULL, &family);
  GdipCreateFont(family, (float)size, FontStyleRegular, UnitPixel, &font);
  GdipDeleteFontFamily(family);
  f->font = font;
  return font != NULL;
}
#ifdef __cplusplus
#define IStream_Release(This) (This)->Release()
#else
#define IStream_Release(This) (This)->lpVtbl -> Release(This)
#endif
int FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin, const RGBQUAD* inpal);
IM_STATIC int gdip_OpenImage(ImGuiHost* host, ImImage* f, const char* filename_or_mem, int mem_size, int h, int w, int bytes_pre_row, int bit_per_pix, const uchar* data)
{
  GpStatus ret;
  GpImage* image = NULL;
  if (data) {
    BITMAPINFO gdiBitmapInfo[1] = {0};
    FillBitmapInfo(gdiBitmapInfo, w, h, bit_per_pix, 1, NULL);
    GdipCreateBitmapFromGdiDib(gdiBitmapInfo, (void*)data, &image);
  }
  else if (mem_size > 0) {
    IStream* pStream = NULL;
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, mem_size);
    void* pData = GlobalLock(hGlobal);
    memcpy(pData, filename_or_mem, mem_size);
    GlobalUnlock(hGlobal);
    if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK) {
      ret = GdipLoadImageFromStream(pStream, &image);
      IStream_Release(pStream);
      pStream = NULL;
    }
    GlobalFree(hGlobal);
  }
  else {
    WCHAR wname[64] = {0};
    int wsize = MultiByteToWideChar(CP_ACP, 0, filename_or_mem, -1, wname, 64);
    wname[wsize] = 0;
    ret = GdipLoadImageFromFile(wname, &image);
  }
  f->image = image;
  if (image) {
    UINT width = 0, height = 0;
    PixelFormat format = 0;
    GdipGetImageWidth(image, &width);
    GdipGetImageHeight(image, &height);
    GdipGetImagePixelFormat(image, &format);
    f->width = width;
    f->height = height;
    f->format = format;
  }
  return image != NULL;
}
IM_STATIC ImFont* gdip_Font_opt(ImGuiHost* host, int opt, ImFontID id, const char* name, int size)
{
  static ImFont fontx[100] = {0};
  return ImGuiFont_opt(opt, host, fontx, 100, id, name, size, gdip_CreateFont, gdip_DeleteFont);
}
IM_STATIC void gdip_blit(imui_gdip_t* g, GpGraphics* graphics)
{
  GdipDrawImageI(graphics, g->bitmap, 0, 0);
}
IM_STATIC float gdip_font_get_text_width(imui_gdip_t* g, ImFontID font_id, float height, const char* text, int len)
{
  GpFont* font = NULL;
  RectF layout = { 0.0f, 0.0f, 65536.0f, 65536.0f };
  RectF bbox;
  int wsize;
  WCHAR* wstr;
  imuiGetFontByID(font_id, &font, NULL);
  if (!font || !text) {
    return 0;
  }
  (void)height;
  wsize = MultiByteToWideChar(CP_ACP, 0, text, len, NULL, 0);
  wstr = (WCHAR*)malloc(wsize * sizeof(wchar_t));
  MultiByteToWideChar(CP_ACP, 0, text, len, wstr, wsize);
  GdipMeasureString(g->memory, wstr, wsize, font, &layout, g->format, &bbox, NULL, NULL);
  free(wstr);
  return bbox.width;
}
IM_STATIC int gdip_measureText(ImGuiHost* host, const char* text, const char* text_end, ImFontID font_id, int formatFlags, IRECT* rc)
{
  int textlen = text_end ? (text_end - text) : strlen(text);
  imui_gdip_t* g = (imui_gdip_t*)host->Driver;
  WORD* wtext = NULL;
  int wlen = my_mb2wc(&wtext, text, textlen);
  GpStringFormat* format = gdip_CreateStrFormat(formatFlags);
  GpFont* font = NULL;
  int w = RCW(rc), h = RCH(rc);
  RectF rcIn = {rc->l, rc->t, w < 1 ? 65536.f : w, h < 1 ? 65536.f : h}, rcOut = {0};
  imuiGetFontByID(font_id, &font, NULL);
  GdipMeasureString(g->memory, wtext, wlen, font, &rcIn, format, &rcOut, NULL, NULL);
  rc->l = (int)rcOut.x;
  rc->t = (int)rcOut.y;
  rc->r = (int)(rcOut.x + rcOut.width);
  rc->b = (int)(rcOut.y + rcOut.height);
  GdipDeleteStringFormat(format);
  FREE(wtext);
  return 0;
}
LRESULT gdip_WndProc(ImGuiHost* host, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  imui_gdip_t* g = (imui_gdip_t*)host->Driver;
  imui_win32_t* s = (imui_win32_t*)host->Window;
  switch (msg) {
  case WM_SIZE:
    if (g->window) {
      unsigned int width = LOWORD(lParam);
      unsigned int height = HIWORD(lParam);
      GdipDeleteGraphics(g->window);
      GdipDeleteGraphics(g->memory);
      GdipDisposeImage(g->bitmap);
      GdipCreateFromHWND(s->g_hWnd, &g->window);
      GdipCreateBitmapFromGraphics(width, height, g->window, &g->bitmap);
      GdipGetImageGraphicsContext(g->bitmap, &g->memory);
    }
    break;
  case WM_PAINT: {
    PAINTSTRUCT paint;
    HDC dc = BeginPaint(hWnd, &paint);
    GpGraphics* graphics;
    GdipCreateFromHDC(dc, &graphics);
    gdip_blit(g, graphics);
    GdipDeleteGraphics(graphics);
    EndPaint(hWnd, &paint);
    return 1;
  }
  }
  return 0;
}
int gdip_UnInit(ImGuiHost* host)
{
  imui_gdip_t* g = (imui_gdip_t*)host->Driver;
  GdipDeleteGraphics(g->window);
  GdipDeleteGraphics(g->memory);
  GdipDisposeImage(g->bitmap);
  GdipDeletePen(g->pen);
  GdipDeleteBrush(g->brush);
  GdipDeleteStringFormat(g->format);
  GdiplusShutdown(g->token);
  dpath_free(g->path);
  memset(g, 0, sizeof(*g));
  gdip_Font_opt(host, 2, 0, 0, 0);
  memset(g, 0, sizeof(*g));
  free(g);
  host->Driver = NULL;
  return 0;
}
int gdip_Rander(ImGuiHost* host, const gc_t* ctx)
{
  imui_gdip_t* g = (imui_gdip_t*)host->Driver;
  const char* va = ctx->buf;
  const char* vb = ctx->buf + ctx->len;
  int i;
  GpStatus lastResult = Ok;
  dpath_reset(g->path);
  affine_init(g->path->matrix);
  {
    TextRenderingHint Hint;
    SmoothingMode Mode;
    Hint = host->NoSoothMode ? TextRenderingHintClearTypeGridFit : TextRenderingHintSingleBitPerPixelGridFit;
    Mode = host->NoSoothMode ? SmoothingModeNone : SmoothingModeHighQuality;
    GdipSetTextRenderingHint(g->memory, Hint);
    GdipSetSmoothingMode(g->memory, Mode);
  }
  GdipGraphicsClear(g->memory, gdip_color(host->clrbg));
  for (; va < vb;) {
    int type = VA_ARG(char);
    if (dpath_run(g->path, type, va, &va)) {
      continue;
    }
    switch (type) {
    case ImDrawCmd_styleColor: {
      char fill = VA_ARG(char);
      ImColor clr = VA_ARG(ImColor);
      clr = gdip_color(clr);
      if (fill) {
        GpSolidFill* brush = NULL;
        lastResult = GdipCreateSolidFill(clr, &brush);
        lastResult = GdipDeleteBrush(g->brush);
        g->brush = brush;
        //lastResult = GdipSetSolidFillColor(g->brush, clr);
      }
      else {
        lastResult = GdipSetPenColor(g->pen, clr);
      }
    }
    break;
    case ImDrawCmd_styleGradient: {
      char fill = VA_ARG(char);
      PointF pt1, pt2;
      int count;
      ARGB blend[100];
      float positions[100];
      GpLineGradient* brush = NULL;
      pt1.x = VA_ARG(float);
      pt1.y = VA_ARG(float);
      pt2.x = VA_ARG(float);
      pt2.y = VA_ARG(float);
      count = VA_ARG(int);
      ASSERT(count < 100);
      for (i = 0; i < count; ++i) {
        positions[i] = VA_ARG(float);
        blend[i] = VA_ARG(ImColor);
        blend[i] = gdip_color(blend[i]);
      }
      lastResult = GdipCreateLineBrush(&pt1, &pt2, _RGBA(255, 0, 0, 0), _RGBA(255, 255, 255, 255), WrapModeTile, &brush);
      lastResult = GdipSetLinePresetBlend(brush, blend, positions, count);
      if (fill) {
        lastResult = GdipDeleteBrush(g->brush);
        g->brush = brush;
      }
      else {
        lastResult = GdipSetPenBrushFill(g->pen, brush);
        lastResult = GdipDeleteBrush(brush);
      }
    }
    break;
    case ImDrawCmd_stylePattern:
      ASSERT(0);
      break;
    case ImDrawCmd_lineWidth: {
      float width = VA_ARG(float);
      lastResult = GdipSetPenWidth(g->pen, width);
    }
    break;
    case ImDrawCmd_lineStyle: {
      int lineStyle = VA_ARG(int);
      int lineCap1 = lineStyle & CAP_MASK;
      int lineJoin1 = lineStyle & JOIN_MASK;
      LineCap lineCap = LineCapFlat;
      DashCap dashCap = DashCapFlat;
      LineJoin lineJoin = LineJoinMiter;
      switch (lineCap1) {
      case CAP_FLAT:
        lineCap = LineCapFlat;
        dashCap = DashCapFlat;
        break;
      case CAP_ROUND:
        lineCap = LineCapRound;
        dashCap = DashCapRound;
        break;
      case CAP_SQUARE:
        lineCap = LineCapSquare;
        dashCap = DashCapTriangle;
        break;
      default:
        break;
      }
      lastResult = GdipSetPenLineCap197819(g->pen, lineCap, lineCap, dashCap);
      switch (lineJoin1) {
      case JOIN_BEVEL:
        lineJoin = LineJoinBevel;
        break;
      case JOIN_ROUND:
        lineJoin = LineJoinRound;
        break;
      case JOIN_MITER:
        lineJoin = LineJoinMiter;
        break;
      default:
        break;
      }
      lastResult = GdipSetPenLineJoin(g->pen, lineJoin);
    }
    break;
    case ImDrawCmd_miterLimit: {
      float miterLimit = VA_ARG(float);
      lastResult = GdipSetPenMiterLimit(g->pen, miterLimit);
    }
    break;
#define VA_ARG_EQ(T, A) T A = VA_ARG(T)
    case ImDrawCmd_fillRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      x += 0.5, y += 0.5;
      lastResult = GdipFillRectangle(g->memory, g->brush, x, y, w, h);
    }
    break;
    case ImDrawCmd_strokeRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      x += 0.5, y += 0.5;
      lastResult = GdipDrawRectangle(g->memory, g->pen, x, y, w, h);
    }
    break;
    case ImDrawCmd_clearRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      x += 0.5, y += 0.5;
      lastResult = GdipSetClipRect(g->memory, x, y, w, h, CombineModeExclude);
    }
    break;
    case ImDrawCmd_clip:
      //lastResult = GdipSetClipPath(g->memory, path, CombineModeReplace);
      break;
    case ImDrawCmd_clipRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      x += 0.5, y += 0.5;
      if (w < 0 || h < 0) {
        lastResult = GdipResetClip(g->memory);
      }
      else {
        lastResult = GdipSetClipRect(g->memory, x, y, w, h, CombineModeReplace);
      }
    }
    break;
    case ImDrawCmd_fill:
      //lastResult = GdipFillPath(g->memory, g->brush, g->path);
    {
      GpPointF* pt0 = (GpPointF*)malloc(sizeof(GpPointF) * g->path->n);
      GpPointF* pt = pt0;
      for (i = 0; i < g->path->n; ++i) {
        pt[i].x = (float)g->path->pt[i].x;
        pt[i].y = (float)g->path->pt[i].y;
      }
      for (i = 0; i < g->path->c; ++i) {
        int len = g->path->ln[i];
        lastResult = GdipFillPolygon2(g->memory, g->brush, pt, len);
        pt += len;
      }
      free(pt0);
    }
    break;
    case ImDrawCmd_stroke:
      //lastResult = GdipDrawPath(g->memory, g->pen, g->path);
    {
      GpPointF* pt0 = (GpPointF*)malloc(sizeof(GpPointF) * (g->path->n + 1));
      GpPointF* pt = pt0;
      const DPOINT* dpt = g->path->pt;
      int j = 0, pos = 0;
      for (i = 0; pos < g->path->n; ++i) {
        int len1, len = (i < g->path->c) ? g->path->ln[i] : (g->path->n - pos);
        for (j = i; j < len; ++j) {
          pt[j].x = (float)dpt[j].x;
          pt[j].y = (float)dpt[j].y;
        }
        len1 = len;
        if (i < g->path->c) {
          pt[len].x = (float)dpt[0].x;
          pt[len].y = (float)dpt[0].y;
          len1 = len + 1;
        }
        else {
          len1 = len;
        }
        lastResult = GdipDrawPolygon(g->memory, g->pen, pt, len1);
        pos += len;
        dpt += len;
      }
      free(pt0);
    }
    break;
    case ImDrawCmd_textAlign: {
      VA_ARG_EQ(char, textAlign);
      g->textAlign = textAlign;
    }
    break;
    case ImDrawCmd_text: {
      VA_ARG_EQ(char, fill);
      VA_ARG_EQ(int, textlen);
      const char* text = (va += textlen) - textlen;
      VA_ARG_EQ(int, font_id);
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      VA_ARG_EQ(int, formatFlags);
      WORD* wtext = NULL;
      RectF rc = {x, y, w, h};
      int wlen = my_mb2wc(&wtext, text, textlen);
      GpFont* font = NULL;
      GpStringFormat* format = gdip_CreateStrFormat(formatFlags);
      imuiGetFontByID(font_id, &font, NULL);
      if (fill) {
        GdipDrawString(g->memory, wtext, wlen, font, &rc, format, g->brush);
      }
      else {
        gdip_DrawStringStroke(g->memory, wtext, wlen, g->pen, g->brush, &rc, font, format);
      }
      GdipDeleteStringFormat(format);
      FREE(wtext);
    }
    break;
    default:
      ASSERT(0);
      break;
    }
  }
  GdipResetClip(g->memory);
  gdip_blit(g, g->window);
  return 0;
}
static int gdip_Init(ImGuiHost* host)
{
  imui_win32_t* s = (imui_win32_t*)host->Window;
  imui_gdip_t* g = NULL;
  GdiplusStartupInput startup = { 1, NULL, FALSE, TRUE };
  int width, height;
  RECT rc;
  int lastResult;
  myReAlloc(g, 1);
  host->Driver = g;
  host->DrvUnInit = gdip_UnInit;
  host->DrvRander = gdip_Rander;
  s->DrvWndProc = gdip_WndProc;
  host->DrvFont_opt = gdip_Font_opt;
  host->DrvMeasureText = gdip_measureText;
  //host->font_opt = gdip_measureText;
  GetClientRect(s->g_hWnd, &rc);
  width = rc.right - rc.left;
  height = rc.bottom - rc.top;
  lastResult = GdiplusStartup(&g->token, &startup, NULL);
  lastResult = GdipCreateFromHWND(s->g_hWnd, &g->window);
  lastResult = GdipCreateBitmapFromGraphics(width, height, g->window, &g->bitmap);
  lastResult = GdipGetImageGraphicsContext(g->bitmap, &g->memory);
  lastResult = GdipCreatePen1(0, 1.0f, UnitPixel, &g->pen);
  lastResult = GdipCreateSolidFill(0, &g->brush);
  lastResult = GdipStringFormatGetGenericTypographic(&g->format);
  lastResult = GdipSetStringFormatFlags(g->format, StringFormatFlagsNoFitBlackBox |
      StringFormatFlagsMeasureTrailingSpaces | StringFormatFlagsNoWrap |
      StringFormatFlagsNoClip);
  return 0;
}

