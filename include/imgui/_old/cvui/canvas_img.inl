#define GDIPARAM(s) canvas_img_t* g = (canvas_img_t*)(s)->x;
#include "cstd.h"
#include "draw/imdraw.inl"
#include "font_win32_tt.inl"
typedef struct canvas_img_t {
  img_t im[1];
  double matrix[9];
  HDC hDC;
  int hdc_x, hdc_y;
  FRECT clip;
  brush_t pen[1];
  brush_t brush[1];
  dpath_t path[1];
  line_cap_e m_line_cap;
  line_join_e m_line_join;
  inner_join_e m_inner_join;
  double wline;
  font_win32_tt_t* f;
} canvas_img_t;
// 颜色、样式
int img_setStyle(canvas_t* s, const char* str, brush_t* brush)
{
  GDIPARAM(s);
  str_t ss[1] = {0};
  int len = strlen(str);
  int i = 0;
  COLOR clr;
  i = cstr_split2(str, i, len, " ,", " ,", ss);
  if (color_parse(ss->s, ss->l, &clr)) {
    brush_set_solid(brush, clr);
  }
  else {
    int k = cstr_splitfind("|color|gradient|pattern|", -1, ss->s, ss->l, 1, 0);
    switch (k) {
    case 0: {
      cstr_split2(str, 0, len, " ", " ", ss);
      color_parse(ss->s, ss->l, &clr);
      brush_set_solid(brush, clr);
    }
    break;
    case 1: {
      DPOINT pt[2] = {0};
      enum {MAXCOLOR1 = 20};
      COLOR presetColors[MAXCOLOR1];
      double blendPositions[MAXCOLOR1];
      int count = 0;
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt[0].x = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt[0].y = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt[1].x = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt[1].y = atof(ss->s);
      for (; i < len && count < MAXCOLOR1;) {
        i = cstr_split2(str, i, len, " ,", " ,", ss), blendPositions[count] = atof(ss->s);
        i = cstr_split2(str, i, len, " ,", " ,", ss), color_parse(ss->s, ss->l, &presetColors[count]);
        ++count;
      }
      affine_transform_pt(g->matrix, pt, 2);
      brush_set_gradient2(brush, pt[0].x, pt[0].y, pt[1].x, pt[1].y, count, presetColors, blendPositions);
    }
    break;
    case 2:
      break;
    default:
      break;
    }
  }
  return 0;
}
int img_fillStyle(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  img_setStyle(s, str, g->brush);
  return 0;
}
int img_strokeStyle(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  img_setStyle(s, str, g->pen);
  return 0;
}
// 阴影
int img_shadowColor(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_shadowBlur(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_shadowOffsetX(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_shadowOffsetY(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
// 线条样式
int img_lineWidth(canvas_t* s, float px)
{
  GDIPARAM(s);
  g->wline = px;
  return 0;
}
int img_lineCap(canvas_t* s, int type)
{
  GDIPARAM(s);
  return 0;
}
int img_lineJoin(canvas_t* s, int type)
{
  GDIPARAM(s);
  return 0;
}
int img_miterLimit(canvas_t* s, float x)
{
  GDIPARAM(s);
  return 0;
}
// 矩形
int img_rect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  DPOINT pt[4];
  int n = vcgen_rect(pt, 0, 4, left, top, left + width, top + height);
  dpath_addpts(g->path, n, pt, g->matrix);
  return 0;
}
int img_clearRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  return 0;
}
// 路径
int img_beginPath(canvas_t* s)
{
  GDIPARAM(s);
  dpath_free(g->path);
  return 0;
}
int img_closePath(canvas_t* s)
{
  GDIPARAM(s);
  dpath_close(g->path);
  return 0;
}
int img_clip(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_fill_impl(canvas_t* s, dpath_t* path)
{
  GDIPARAM(s);
  int i, j = 0;
  int len[256];
  ASSERT(path->c < 256);
  dpath_close(path);
  for (i = 0; i < path->c; ++i) {
    len[i] = path->ln[i] - j;
    j = path->ln[i];
  }
  imdrawaa_poly_brush(g->im, NULL, path->pt, len, path->c, g->brush);
  return 0;
}
int img_stroke_path(canvas_t* s, const DPOINT* pt, int n, int isclose)
{
  GDIPARAM(s);
  imdrawaa_poly_stroke_brush(g->im, NULL, pt, n, isclose, g->pen, g->wline, g->m_line_cap, g->m_line_join, g->m_inner_join);
  return 0;
}
int img_stroke_impl(canvas_t* s, const dpath_t* path)
{
  GDIPARAM(s);
  int i, j = 0;
  for (i = 0; i < path->c; ++i) {
    img_stroke_path(s, path->pt + j, path->ln[i] - j, 1);
    j = path->ln[i];
  }
  if (0 == path->c || j < path->n) {
    img_stroke_path(s, path->pt + j, path->n, 0);
  }
  return 0;
}
int img_fill(canvas_t* s)
{
  GDIPARAM(s);
  img_fill_impl(s, g->path);
  return 0;
}
int img_stroke(canvas_t* s)
{
  GDIPARAM(s);
  img_stroke_impl(s, g->path);
  return 0;
}
int img_draw_text(canvas_t* s, const char* text, int len, double x, double y, double maxwidth, int isfill)
{
  GDIPARAM(s);
  int num_glyphs = 0;
  font_win32_tt_t* f = g->f;
  //double width = (-m_weight.value() * m_height.value() * 0.05);
  BOOL m_hinting = 1;
  BOOL m_flip_y = 1;
  BOOL m_kerning = 1;
  WORD* wtext = NULL;
  int ret, wlen = my_mb2wc(&wtext, text, len);
  WORD* p = wtext;
  double x0;
  //affine_transform(g->matrix, &x, &y);
  x0 = x;
  //y += f->m_fi->m_height;
  //affine_transform_pt(mtx, glyph->path->pt, glyph->path->n);
  glyph_t glyph[1] = {0};
  while (*p) {
    dpath_reset(glyph->path);
    ret = font_tt_glyph(g->hDC, f, glyph_outline, m_hinting, m_flip_y, *p, g->matrix, glyph);
    if (ret) {
      affine_translation_pt(glyph->path->pt, glyph->path->n, x, y);
      if (m_kerning) {
        //m_fman.add_kerning(&x, &y);
      }
      if (isfill) {
        img_fill_impl(s, glyph->path);
      }
      else {
        img_stroke_impl(s, glyph->path);
      }
      {
        x += glyph->m_advance_x;
        y += glyph->m_advance_y;
      }
      if (maxwidth > 0 && x >= (x0 + maxwidth - f->m_fi->m_height)) {
        x = x0;
        y += f->m_fi->m_height;
        break;
      }
      // increment pen position
      ++num_glyphs;
    }
    ++p;
  }
  dpath_free(glyph->path);
  FREE(wtext);
  return num_glyphs;
}
int img_fillRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  DPOINT pt[4];
  dpath_t path[1] = {0};
  int n = vcgen_rect(pt, 0, 4, left, top, left + width, top + height);
  affine_transform_pt(g->matrix, pt, n);
  imdrawaa_poly_brush(g->im, NULL, pt, &n, 1, g->brush);
  return 0;
}
int img_strokeRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  DPOINT pt[4];
  dpath_t path[1] = {0};
  int n = vcgen_rect(pt, 0, 4, left, top, left + width, top + height);
  affine_transform_pt(g->matrix, pt, n);
  img_stroke_path(s, pt, n, 1);
  return 0;
}
int img_moveTo(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  dpath_moveto(g->path, x, y, g->matrix);
  return 0;
}
int img_lineTo(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  dpath_lineto(g->path, x, y, g->matrix);
  return 0;
}
int img_arcTo(canvas_t* s, xypos_t x1, xypos_t y1, xypos_t x2, xypos_t y2, xypos_t radius)
{
  GDIPARAM(s);
  return 0;
}
int img_arc(canvas_t* s, xypos_t x, xypos_t y, xypos_t radius, xypos_t startAngle, xypos_t endAngle, BOOL anticlockwise)
{
  GDIPARAM(s);
  DPOINT pt[64];
  int n = vcgen_arc(pt, 0, 64, 1, x, y, radius, radius, startAngle, endAngle);
  dpath_addpts(g->path, n, pt, g->matrix);
  return 0;
}
int img_quadraticCurveTo(canvas_t* s, xypos_t cpx, xypos_t cpy, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  dpath_curve3(g->path, cpx, cpy, x, y, g->matrix);
  return 0;
}
int img_bezierCurveTo(canvas_t* s, xypos_t cpx1, xypos_t cpy1, xypos_t cpx2, xypos_t cpy2, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0;
}
int img_isPointInPath(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0; // 如果指定的点位于当前路径中，则返回 true，否则返回 false
}
// 转换
int img_scale(canvas_t* s, float scalewidth, float scaleheight)
{
  GDIPARAM(s);
  affine_scaling(g->matrix, scalewidth, scaleheight);
  return 0;
}
int img_rotate(canvas_t* s, float angle)
{
  GDIPARAM(s);
  affine_rotation(g->matrix, angle);
  return 0;
}
int img_translate(canvas_t* s, float x, float y)
{
  GDIPARAM(s);
  affine_translation(g->matrix, x, y);
  return 0;
}
int img_transform(canvas_t* s, float a, float b, float c, float d, float e, float f)
{
  GDIPARAM(s);
  affine_setmul(g->matrix, a, b, c, d, e, f);
  return 0;
}
int img_setTransform(canvas_t* s, float a, float b, float c, float d, float e, float f)
{
  GDIPARAM(s);
  affine_set(g->matrix, a, b, c, d, e, f);
  return 0;
}
// 文本
int img_font(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  g->f = font_tt_find(g->hDC, str);
  return 0;
}
int img_textAlign(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  return 0;
}
int img_textBaseline(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  return 0;
}
int img_fillText(canvas_t* s, const char* text, int len, xypos_t x, xypos_t y, xypos_t maxWidth)
{
  GDIPARAM(s);
  img_draw_text(s, text, len, x, y, maxWidth, 1);
  return 0;
}
int img_strokeText(canvas_t* s, const char* text, int len, xypos_t x, xypos_t y, xypos_t maxWidth)
{
  GDIPARAM(s);
  img_draw_text(s, text, len, x, y, maxWidth, 0);
  return 0;
}
int img_measureText(canvas_t* s, const char* text, int textlen)
{
  GDIPARAM(s);
  return 0;
}
// 图像绘制
int img_drawImage(canvas_t* s, const img_t* img, int frame, const IRECT* rcimg, xypos_t x, xypos_t y, const ISIZE* sz)
{
  GDIPARAM(s);
  return 0;
}
// 像素操作
int img_width(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_height(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_data(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_createImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_getImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_putImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
// 合成
int img_globalAlpha(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int img_globalCompositeOperation(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // 设置或返回新图像如何绘制到已有的图像上
}
// 其他
int img_save(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // 保存当前环境的状态
}
int img_restore(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // 返回之前保存过的路径状态和属性
}
int img_createEvent(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int img_getContext(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int img_toDataURL(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int img_release(canvas_t* s)
{
  GDIPARAM(s);
  BITMAPINFO hDib[1] = {0};
  hDib->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  hDib->bmiHeader.biWidth = g->im->w;
  hDib->bmiHeader.biHeight = g->im->h;
  hDib->bmiHeader.biPlanes = 1;
  hDib->bmiHeader.biBitCount = 32;
  hDib->bmiHeader.biCompression = BI_RGB;
  hDib->bmiHeader.biSizeImage = g->im->w * g->im->h * 4;
  SetDIBitsToDevice(g->hDC, g->hdc_x, g->hdc_y, g->im->w, g->im->h,
      0, 0, 0, g->im->h, (uchar*)(g + 1), hDib, DIB_RGB_COLORS);
  FREE(s->x);
  s->x = NULL;
  return 0;
}
#define IMG_FUNSET(name) s->name = img_##name;
int canvas_img_set(canvas_t* s, HDC hDC)
{
  int nW, nH;
  RECT clipBox;
  canvas_img_t* g = NULL;
  COLORREF clr = GetPixel(hDC, 0, 0);
  CANVAS_FUN(IMG_FUNSET);
  GetClipBox(hDC, &clipBox);
  clr = TORGB(clr);
  nW = clipBox.right - clipBox.left;
  nH = clipBox.bottom - clipBox.top;
  ASSERT(nW > 0 && nH > 0);
  myrealloc((void**)&g, sizeof(canvas_img_t) + nW * nH * 4, __FILE__, __LINE__);
  memset4(g + 1, clr, nW * nH);
  IMINIT(g->im, nH, nW, (uchar*)(g + 1) + (nH - 1)*nW * 4, -4 * nW, 4, 1);
  g->hDC = hDC;
  g->hdc_x = clipBox.left;
  g->hdc_y = clipBox.top;
  s->x = g;
  affine_init(g->matrix);
  g->m_line_cap = round_cap;
  g->m_line_join = round_join;
  g->m_inner_join = inner_round;
  brush_set_solid(g->pen, _rgb(0, 0, 0));
  brush_set_solid(g->brush, _rgb(0, 0, 0));
  g->wline = 1;
  return 0;
}
#undef IMG_FUNSET
#undef GDIPARAM

