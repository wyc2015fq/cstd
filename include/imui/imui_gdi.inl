#include <windows.h>
typedef struct {
  int textAlign;
  HDC memory_dc;
  HDC window_dc;
  int width;
  int height;
  int lineStyle;
  int line_thickness;
  COLOR fillColor;
  COLOR lineColor;
  float miterLimit;
  HBITMAP bitmap;
  POINT* points;
  dpath_t path[1];
} imui_gdi_t;
#define gdi_color(clr) ((ImRGBAToBGRA(clr)) & 0x00ffffff)
IM_STATIC int gdi_DeleteFont(ImGuiHost* host, ImFont* f)
{
  DeleteObject(f->font);
  f->font = NULL;
  return 0;
}
IM_STATIC int gdi_CreateFont(ImGuiHost* host, ImFont* f, const char* name, int size)
{
  TEXTMETRICW metric;
  imui_gdi_t* g = (imui_gdi_t*)host->Driver;
  HFONT font = CreateFont(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name);
  SelectObject(g->memory_dc, font);
  GetTextMetricsW(g->memory_dc, &metric);
  f->size = metric.tmHeight;
  f->font = font;
  return font != NULL;
}
IM_STATIC int gdi_OpenImage(ImGuiHost* host, ImImage* f, const char* filename_or_mem, int mem_size, int h, int w, int bytes_pre_row, int bit_per_pix, const uchar* data)
{
  void* image = NULL;
  return image != NULL;
}
IM_STATIC ImFont* gdi_Font_opt(ImGuiHost* host, int opt, ImFontID id, const char* name, int size)
{
  static ImFont fontx[100] = {0};
  return ImGuiFont_opt(opt, host, fontx, 100, id, name, size, gdi_CreateFont, gdi_DeleteFont);
}
IM_STATIC float gdi_font_get_text_width(imui_gdi_t* g, ImFontID font_id, float height, const char* text, int len)
{
  HFONT font = NULL;
  SIZE size = {0};
  imuiGetFontByID(font_id, &font, NULL);
  if (!font || !text) {
    return 0;
  }
  if (GetTextExtentPoint32A(g->memory_dc, text, len, &size)) {
    return (float)size.cx;
  }
  return 0;
}
IM_STATIC int gdi_measureText(ImGuiHost* host, const char* text, const char* text_end, ImFontID font_id, int formatFlags, IRECT* rc)
{
  int len = text_end ? (text_end - text) : strlen(text);
  imui_gdi_t* g = (imui_gdi_t*)host->Driver;
  HFONT font = NULL;
  SIZE size = {0};
  imuiGetFontByID(font_id, &font, NULL);
  GetTextExtentPoint32A(g->memory_dc, text, len, &size);
  *rc = iRECT2(rc->l, rc->t, size.cx, size.cy);
  return 0;
}
LRESULT gdi_WndProc(ImGuiHost* host, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  imui_gdi_t* g = (imui_gdi_t*)host->Driver;
  imui_win32_t* s = (imui_win32_t*)host->Window;
  switch (msg) {
  case WM_SIZE:
    if (g) {
      int width = LOWORD(lParam);
      int height = HIWORD(lParam);
      if (width != g->width || height != g->height) {
        DeleteObject(g->bitmap);
        g->bitmap = CreateCompatibleBitmap(g->window_dc, width, height);
        g->width = width;
        g->height = height;
        SelectObject(g->memory_dc, g->bitmap);
      }
    }
    break;
  case WM_PAINT: {
    PAINTSTRUCT paint;
    HDC dc = BeginPaint(hWnd, &paint);
    BitBlt(dc, 0, 0, g->width, g->height, g->memory_dc, 0, 0, SRCCOPY);
    EndPaint(hWnd, &paint);
    return 1;
  }
  }
  return 0;
}
int gdi_UnInit(ImGuiHost* host)
{
  imui_gdi_t* g = (imui_gdi_t*)host->Driver;
  imui_win32_t* s = (imui_win32_t*)host->Window;
  DeleteObject(g->memory_dc);
  DeleteObject(g->bitmap);
  ReleaseDC(s->g_hWnd, g->window_dc);
  memset(g, 0, sizeof(*g));
  gdi_Font_opt(host, 2, 0, 0, 0);
  memset(g, 0, sizeof(*g));
  free(g);
  host->Driver = NULL;
  return 0;
}
static void gdi_fill_polygon(ImGuiHost* host, BOOL fill, BOOL line, const dpath_t* path)
{
  imui_win32_t* s = (imui_win32_t*)host->Window;
  imui_gdi_t* g = (imui_gdi_t*)host->Driver;
  int i = 0, j = 0;
  HDC dc = g->memory_dc;
  if (fill) {
    DPOINT* pt = path->pt;
    int pos = 0;
    for (i = 0; pos < path->n; ++i) {
      int len = i < path->c ? path->ln[i] : (path->n - pos);
      MYREALLOC(g->points, len);
      for (i = 0; i < len; ++i) {
        g->points[i].x = round(pt[i].x);
        g->points[i].y = round(pt[i].y);
      }
      SetDCBrushColor(dc, g->fillColor);
      SetDCPenColor(dc, g->fillColor);
      Polygon(dc, g->points, i);
      pos += len;
      pt += len;
    }
  }
  if (line) {
    HPEN pen = NULL;
    if (g->line_thickness == 1) {
      SetDCPenColor(dc, g->lineColor);
    }
    else {
      pen = CreatePen(PS_SOLID, g->line_thickness, g->lineColor);
      SelectObject(dc, pen);
    }
    if (1) {
      DPOINT* pt = path->pt;
      int pos = 0;
      for (i = 0; pos < path->n; ++i) {
        int len = i < path->c ? path->ln[i] : (path->n - pos);
        MoveToEx(dc, round(pt[0].x), round(pt[pos + 0].y), NULL);
        for (i = 1; i < len; ++i) {
          LineTo(dc, round(pt[i].x), round(pt[i].y));
        }
        if (i < path->c) {
          LineTo(dc, round(pt[0].x), round(pt[0].y));
        }
        pos += len;
        pt += len;
      }
    }
    if (pen) {
      SelectObject(dc, GetStockObject(DC_PEN));
      DeleteObject(pen);
    }
  }
}
int gdi_Rander(ImGuiHost* host, const gc_t* ctx)
{
  imui_gdi_t* g = (imui_gdi_t*)host->Driver;
  const char* va = ctx->buf;
  const char* vb = ctx->buf + ctx->len;
  int i;
  HDC dc = g->memory_dc;
  dpath_reset(g->path);
  affine_init(g->path->matrix);
  SelectObject(dc, GetStockObject(DC_PEN));
  SelectObject(dc, GetStockObject(DC_BRUSH));
  {
    COLOR clrbg = gdi_color(host->clrbg);
    RECT rect = { 0, 0, g->width, g->height };
    SetBkColor(dc, clrbg);
    ExtTextOutA(dc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
    //BitBlt(g->window_dc, 0, 0, g->width, g->height, g->memory_dc, 0, 0, SRCCOPY);
    //return 0;
  }
  for (; va < vb;) {
    int type = VA_ARG(char);
    if (dpath_run(g->path, type, va, &va)) {
      continue;
    }
    switch (type) {
    case ImDrawCmd_styleColor: {
      char fill = VA_ARG(char);
      ImColor clr = VA_ARG(ImColor);
      clr = gdi_color(clr);
      if (fill) {
        g->fillColor = clr;
      }
      else {
        g->lineColor = clr;
      }
    }
    break;
    case ImDrawCmd_styleGradient: {
      char fill = VA_ARG(char);
      FPOINT pt1, pt2;
      int count;
      COLOR blend[100];
      float positions[100];
      pt1.x = VA_ARG(float);
      pt1.y = VA_ARG(float);
      pt2.x = VA_ARG(float);
      pt2.y = VA_ARG(float);
      count = VA_ARG(int);
      ASSERT(count < 100);
      for (i = 0; i < count; ++i) {
        positions[i] = VA_ARG(float);
        blend[i] = VA_ARG(ImColor);
        blend[i] = gdi_color(blend[i]);
      }
      if (count > 0) {
        if (fill) {
          g->fillColor = blend[0];
        }
        else {
          g->lineColor = blend[0];
        }
      }
    }
    break;
    case ImDrawCmd_stylePattern:
      ASSERT(0);
      break;
    case ImDrawCmd_lineWidth: {
      float width = VA_ARG(float);
      g->line_thickness = (int)(MAX(1, width));
    }
    break;
    case ImDrawCmd_lineStyle: {
      g->lineStyle = VA_ARG(int);
    }
    break;
    case ImDrawCmd_miterLimit: {
      float miterLimit = VA_ARG(float);
      g->miterLimit = miterLimit;
    }
    break;
#define VA_ARG_EQ(T, A) T A = VA_ARG(T)
    case ImDrawCmd_fillRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      RECT rect = { x, y, x + w, y + h };
      SetBkColor(dc, g->fillColor);
      ExtTextOutW(dc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
    }
    break;
    case ImDrawCmd_strokeRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      HPEN pen = NULL;
      if (g->line_thickness == 1) {
        SetDCPenColor(dc, g->lineColor);
      }
      else {
        pen = CreatePen(PS_SOLID, g->line_thickness, g->lineColor);
        SelectObject(dc, pen);
      }
      Rectangle(dc, (int)x, (int)y, (int)(x + w), (int)(y + h));
      if (pen) {
        SelectObject(dc, GetStockObject(DC_PEN));
        DeleteObject(pen);
      }
    }
    break;
    case ImDrawCmd_clearRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
    }
    break;
    case ImDrawCmd_clip:
      break;
    case ImDrawCmd_clipRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      if (w < 0 || h < 0) {
        SelectClipRgn(dc, NULL);
      }
      else {
        SelectClipRgn(dc, NULL);
        IntersectClipRect(dc, (int)x, (int)y, (int)(x + w + 1), (int)(y + h + 1));
      }
    }
    break;
    case ImDrawCmd_fill:
      gdi_fill_polygon(host, 1, 0, g->path);
      break;
    case ImDrawCmd_stroke:
      gdi_fill_polygon(host, 0, 1, g->path);
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
      HFONT font = NULL;
      imuiGetFontByID(font_id, &font, NULL);
      //SetBkColor(dc, g->fillColor);
      //SetBkColor(dc, 0);
      if (font) {
        IRECT rc = iRECT2((int)x, (int)y, (int)w, (int)h);
        SIZE size;
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, g->fillColor);
        SelectObject(dc, font);
        GetTextExtentPoint32A(dc, text, textlen, &size);
        iRectAlign(rc, size.cx, size.cy, formatFlags, &rc);
        //ExtTextOutA(dc, rc.l, rc.t, ETO_OPAQUE, NULL, text, textlen, NULL);
        ExtTextOutA(dc, rc.l, rc.t, 0, (RECT*)&rc, text, textlen, NULL);
        //TextOutA(dc, rc.l, rc.t, text, textlen);
      }
    }
    break;
    default:
      ASSERT(0);
      break;
    }
  }
  BitBlt(g->window_dc, 0, 0, g->width, g->height, g->memory_dc, 0, 0, SRCCOPY);
  return 0;
}
int gdi_Init(ImGuiHost* host)
{
  imui_win32_t* s = (imui_win32_t*)host->Window;
  imui_gdi_t* g = NULL;
  int width, height;
  RECT rc;
  myReAlloc(g, 1);
  host->Driver = g;
  host->DrvUnInit = gdi_UnInit;
  host->DrvRander = gdi_Rander;
  s->DrvWndProc = gdi_WndProc;
  host->DrvFont_opt = gdi_Font_opt;
  host->DrvMeasureText = gdi_measureText;
  //host->font_opt = gdi_measureText;
  GetClientRect(s->g_hWnd, &rc);
  width = rc.right - rc.left;
  height = rc.bottom - rc.top;
  g->window_dc = GetDC(s->g_hWnd);
  g->bitmap = CreateCompatibleBitmap(g->window_dc, width, height);
  g->memory_dc = CreateCompatibleDC(g->window_dc);
  SelectObject(g->memory_dc, g->bitmap);
  g->width = width;
  g->height = height;
  g->line_thickness = 1;
  return 0;
}

