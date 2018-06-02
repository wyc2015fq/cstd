#include "softgc.inl"
//#include "sys/win32.inl"
#ifdef _WIN32
typedef struct {
  HDC hMemDC;
  softgc g[1];
} imui_soft_t;
CC_INLINE int hdc_resize(HDC* hdc, int h, int w, int cn);
BOOL hdc_get_image(HDC hdc, img_t* im);
#else
//////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct imui_soft_t {
  GC gc;
  Drawable drawable;
  softgc g[1];
} imui_soft_t;
#endif
IM_STATIC int soft_SetTexture(ImGuiHost* host, texture_t* tex, int fmt, int height, int width, void* data, int step)
{
  if (height != tex->h || width != tex->w || fmt != tex->fmt) {
    uint32* pFontTexture = (uint32*)tex->tex;
    if (pFontTexture) {
      free(pFontTexture);
      tex->tex = NULL;
      memset(tex, 0, sizeof(*tex));
    }
    if ((height * width) > 0) {
      MYREALLOC(pFontTexture, height * width);
      tex->tex = pFontTexture;
      tex->h = height;
      tex->w = width;
      tex->fmt = fmt;
    }
  }
  if (tex->tex && data) {
    int y;
    uchar* pixels = (uchar*)data;
    uint32* pFontTexture = (uint32*)tex->tex;
    int bytes_per_pixel = 4;
    for (y = 0; y < height; y++) {
      memcpy(pFontTexture + width * y, pixels + step * y, (width * bytes_per_pixel));
    }
  }
  return 0;
}
static int soft_CreateFont(ImGuiHost* host, ImFont* f, const char* name, int size)
{
  texfont_t* font = NULL;
  myReAlloc(font, 1);
  //texfont_Init_Default(font);
  packing_raster_font(songti_12, font);
  f->font = font;
  f->size = abs((int)font->FontSize);
  return 0;
}
IM_STATIC int soft_DeleteFont(ImGuiHost* host, ImFont* f)
{
  imui_soft_t* g = (imui_soft_t*)host->Driver;
  texfont_t* font = (texfont_t*)(f->font);
  if (font) {
    texfont_Free(font);
    f->font = NULL;
    f->size = 0;
  }
  return 0;
}
IM_STATIC int soft_measureText(ImGuiHost* host, const char* text, const char* text_end, ImFontID font_id, int formatFlags, IRECT* prc)
{
  texfont_t* font = NULL;
  IRECT rc = prc ? *prc : iRECT2(0, 0, 10000, 10000);
  float w = RCW(&rc);
  ImVec2 sz;
  imuiGetFontByID(font_id, &font, 0);
  sz = texfont_CalcTextSizeA(font, font->FontSize, w, w, text, text_end, NULL);
  iRectAlign(rc, (int)sz.x, (int)sz.y, formatFlags, &rc);
  if (prc) {
    *prc = rc;
  }
  return 0;
}
IM_STATIC int soft_OpenImage(ImGuiHost* host, ImImage* f, const char* filename_or_mem, int mem_size, int h, int w, int step, int bpp, const uchar* data)
{
  uchar* image = NULL;
  if (data) {
    MYREALLOC(image, h * step);
    memcpy(image, data, h * step);
    f->width = w;
    f->height = h;
    f->bpp = bpp;
    f->step = step;
  }
  else if (mem_size > 0) {
    ASSERT(0);
  }
  else {
    ASSERT(0);
  }
  f->image = image;
  return image != NULL;
}
int soft_DeleteImage(ImGuiHost* host, ImImage* f)
{
  ASSERT(0);
  return 0;
}
IM_STATIC ImFont* soft_Font_opt(ImGuiHost* host, int opt, ImFontID id, const char* name, int size)
{
  static ImFont fontx[100] = {0};
  return ImGuiFont_opt(opt, host, fontx, 100, id, name, size, soft_CreateFont, soft_DeleteFont);
}
#ifdef _WIN32
int soft_Rander(ImGuiHost* host, const gc_t* ctx)
{
  imui_soft_t* g = (imui_soft_t*)host->Driver;
  imui_win32_t* s = (imui_win32_t*)host->Window;
  const char* va = ctx->buf;
  const char* vb = ctx->buf + ctx->len;
  HDC hDC = GetDC(s->g_hWnd);
  imclear(g->g->im, &host->rcDirty, host->clrbg);
  softgc_run(g->g, va, vb, host->rcDirty);
  BitBlt(hDC, 0, 0, g->g->im->w, g->g->im->h, g->hMemDC, 0, 0, SRCCOPY);
  ReleaseDC(s->g_hWnd, hDC);
  return 0;
}
// Data
LRESULT soft_WndProc(ImGuiHost* host, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  imui_soft_t* g = (imui_soft_t*)host->Driver;
  switch (msg) {
  case WM_SIZE:
    if (wParam != SIZE_MINIMIZED) {
      int w = LOWORD(lParam), h = HIWORD(lParam);
      hdc_resize(&g->hMemDC, h, w, WINBMPCN);
      hdc_get_image(g->hMemDC, g->g->im);
      *g->g->clip = iRECT(0, 0, w, h);
      ASSERT(g->g->im->w == w && g->g->im->h == h);
    }
    return 0;
  case WM_PAINT:
    if (host) {
      PAINTSTRUCT ps = {0};
      HDC hDC = BeginPaint(hWnd, &ps);
      BitBlt(hDC, 0, 0, g->g->im->w, g->g->im->h, g->hMemDC, 0, 0, SRCCOPY);
      EndPaint(hWnd, &ps);
      return 1;
    }
    break;
  }
  return 0;
}
int soft_UnInit(ImGuiHost* host)
{
  imui_soft_t* g = (imui_soft_t*)host->Driver;
  DeleteDC(g->hMemDC);
  softgc_free(g->g);
  return 0;
}
int soft_Init(ImGuiHost* host)
{
  imui_win32_t* s = (imui_win32_t*)host->Window;
  imui_soft_t* g = NULL;
  int w, h;
  RECT rc;
  myReAlloc(g, 1);
  host->Driver = g;
  host->DrvUnInit = soft_UnInit;
  host->DrvRander = soft_Rander;
  host->DrvSetTexture = soft_SetTexture;
  s->DrvWndProc = soft_WndProc;
  host->DrvFont_opt = soft_Font_opt;
  host->DrvMeasureText = soft_measureText;
  //host->font_opt = soft_measureText;
  GetClientRect(s->g_hWnd, &rc);
  w = rc.right - rc.left;
  h = rc.bottom - rc.top;
  hdc_resize(&g->hMemDC, h, w, WINBMPCN);
  hdc_get_image(g->hMemDC, g->g->im);
  softgc_init(g->g);
  g->g->flip_y = TRUE;
  *g->g->clip = iRECT(0, 0, w, h);
  host->w = w;
  host->h = h;
  ASSERT(g->g->im->w == w && g->g->im->h == h);
  return 1;
}
#else
int soft_Rander(ImGuiHost* host, const ImDrawContent* ctx)
{
  imui_soft_t* g = (imui_soft_t*)host->Driver;
  imui_win32_t* s = (imui_win32_t*)host->Window;
  const char* va = ctx->buf;
  const char* vb = ctx->buf + ctx->len;
  HDC hDC = GetDC(s->g_hWnd);
  imclear(g->g->im, g->g->clip, host->clrbg);
  *g->g->clip = host->rcDirty;
  softgc_run(g->g, va, vb);
  BitBlt(hDC, 0, 0, g->g->im->w, g->g->im->h, g->hMemDC, 0, 0, SRCCOPY);
  ReleaseDC(s->g_hWnd, hDC);
  return 0;
}
// Data
LRESULT soft_WndProc(ImGuiHost* host, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  imui_soft_t* g = (imui_soft_t*)host->Driver;
  switch (msg) {
  case WM_SIZE:
    if (wParam != SIZE_MINIMIZED) {
      int w = LOWORD(lParam), h = HIWORD(lParam);
      hdc_resize(&g->hMemDC, h, w, WINBMPCN);
      hdc_get_image(g->hMemDC, g->g->im);
      *g->g->clip = iRECT(0, 0, w, h);
      ASSERT(g->g->im->w == w && g->g->im->h == h);
    }
    return 0;
  case WM_PAINT:
    if (host) {
      PAINTSTRUCT ps = {0};
      HDC hDC = BeginPaint(hWnd, &ps);
      BitBlt(hDC, 0, 0, g->g->im->w, g->g->im->h, g->hMemDC, 0, 0, SRCCOPY);
      EndPaint(hWnd, &ps);
      return 1;
    }
    break;
  }
  return 0;
}
int soft_UnInit(ImGuiHost* host)
{
  imui_soft_t* g = (imui_soft_t*)host->Driver;
  DeleteDC(g->hMemDC);
  softgc_free(g->g);
  return 0;
}
int soft_Init(ImGuiHost* host)
{
  imui_win32_t* s = (imui_win32_t*)host->Window;
  imui_soft_t* g = NULL;
  int w, h;
  RECT rc;
  imuiReAlloc(g, 1);
  host->Driver = g;
  host->DrvUnInit = soft_UnInit;
  host->DrvRander = soft_Rander;
  host->DrvSetTexture = soft_SetTexture;
  s->DrvWndProc = soft_WndProc;
  host->DrvFont_opt = soft_Font_opt;
  host->DrvMeasureText = soft_measureText;
  //host->font_opt = soft_measureText;
  GetClientRect(s->g_hWnd, &rc);
  w = rc.right - rc.left;
  h = rc.bottom - rc.top;
  hdc_resize(&g->hMemDC, h, w, WINBMPCN);
  hdc_get_image(g->hMemDC, g->g->im);
  softgc_init(g->g);
  g->g->flip_y = TRUE;
  *g->g->clip = iRECT(0, 0, w, h);
  ASSERT(g->g->im->w == w && g->g->im->h == h);
  return 1;
}
#endif

