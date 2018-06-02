#include <gdiplus.h>
#pragma comment(lib,"GdiPlus.lib")
#define is0_9(c) ('0'<=(c) && (c)<='9')
using namespace Gdiplus;
#define GDIPARAM(s) canvas_gdiplus_t* g = (canvas_gdiplus_t*)(s)->x;
typedef struct canvas_gdiplus_t {
  Graphics* myg;
  Gdiplus::GraphicsPath* path;
  Gdiplus::Pen* pen;
  Gdiplus::Brush* brush;
  Gdiplus::Font* font;
  int textAlign;
  xypos_t x, y;
  BOOL ismoveed;
} canvas_gdiplus_t;
// color|gradient|pattern;
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
BOOL str2Color(const char* s, int l, Color* pcolor)
{
  COLOR clr = 0;
  BOOL ret = color_parse(s, l, &clr);
  if (ret) {
    *pcolor = Color(GetAV(clr), GetRV(clr), GetGV(clr), GetBV(clr));
  }
  return ret;
}
// 颜色、样式
int gdiplus_fillStyle(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  str_t ss[1] = {0};
  int len = strlen(str);
  int i = 0;
  Color clr;
  i = cstr_split2(str, i, len, " ,", " ,", ss);
  if (str2Color(ss->s, ss->l, &clr)) {
    delete g->brush;
    g->brush = new SolidBrush(clr);
  }
  else {
    int k = cstr_splitfind("|color|gradient|pattern|RadialGradient|", -1, ss->s, ss->l, 1, 0);
    switch (k) {
    case 0:
      cstr_split2(str, 0, len, " ", " ", ss);
      if (str2Color(ss->s, ss->l, &clr)) {
        delete g->brush;
        g->brush = new SolidBrush(clr);
      }
      break;
    case 1: {
      PointF pt0(0, 0), pt1(0, 0);
      LinearGradientBrush* brush = NULL;
      enum {MAXCOLOR1 = 20};
      Color presetColors[MAXCOLOR1];
      REAL blendPositions[MAXCOLOR1];
      int count = 0;
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt0.X = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt0.Y = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt1.X = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt1.Y = atof(ss->s);
      brush = new LinearGradientBrush(pt0, pt1, Color(255, 0, 0, 0), Color(255, 255, 255, 255));
      for (; i < len && count < MAXCOLOR1;) {
        i = cstr_split2(str, i, len, " ,", " ,", ss), blendPositions[count] = atof(ss->s);
        i = cstr_split2(str, i, len, " ,", " ,", ss), str2Color(ss->s, ss->l, &presetColors[count]);
        ++count;
      }
      brush->SetInterpolationColors(presetColors, blendPositions, count);
      delete g->brush;
      g->brush = brush;
    }
    break;
    case 2: {
      //RadialGradientBrush ;
      //PathGradientBrush ;
    }
    break;
    case 3: {
      PointF pt0(0, 0), pt1(0, 0);
      float r0 = 0, r1 = 0;
      PathGradientBrush* brush = NULL;
      GraphicsPath* path = new GraphicsPath;
      enum {MAXCOLOR1 = 20};
      Color presetColors[MAXCOLOR1];
      REAL blendPositions[MAXCOLOR1];
      int count = 0;
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt0.X = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt0.Y = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), r0 = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt1.X = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt1.Y = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), r1 = atof(ss->s);
      path->AddEllipse(pt1.X, pt1.Y, r1, r1);
      brush = new PathGradientBrush(path);
      brush->SetCenterPoint(pt0);
      for (; i < len && count < MAXCOLOR1;) {
        i = cstr_split2(str, i, len, " ,", " ,", ss), blendPositions[count] = atof(ss->s);
        i = cstr_split2(str, i, len, " ,", " ,", ss), str2Color(ss->s, ss->l, &presetColors[count]);
        ++count;
      }
      brush->SetInterpolationColors(presetColors, blendPositions, count);
      brush->SetCenterColor(presetColors[0]);
      delete g->brush;
      g->brush = brush;
      delete path;
    }
    break;
    default:
      break;
    }
  }
  return 0;
}
int gdiplus_strokeStyle(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  str_t ss[1] = {0};
  int len = strlen(str);
  int i = 0;
  Color clr;
  i = cstr_split2(str, i, len, " ,", " ,", ss);
  if (str2Color(ss->s, ss->l, &clr)) {
    g->pen->SetColor(clr);
  }
  else {
    int k = cstr_splitfind("|color|gradient|pattern|", -1, ss->s, ss->l, 1, 0);
    switch (k) {
    case 0: {
      SolidBrush* brush = NULL;
      cstr_split2(str, 0, len, " ", " ", ss);
      str2Color(ss->s, ss->l, &clr);
      brush = new SolidBrush(clr);
      g->pen->SetBrush(brush);
      delete brush;
    }
    break;
    case 1: {
      PointF pt0(0, 0), pt1(0, 0);
      LinearGradientBrush* brush = NULL;
      enum {MAXCOLOR1 = 20};
      Color presetColors[MAXCOLOR1];
      REAL blendPositions[MAXCOLOR1];
      int count = 0;
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt0.X = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt0.Y = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt1.X = atof(ss->s);
      i = cstr_split2(str, i, len, " ,", " ,", ss), pt1.Y = atof(ss->s);
      brush = new LinearGradientBrush(pt0, pt1, Color(255, 0, 0, 0), Color(255, 255, 255, 255));
      for (; i < len && count < MAXCOLOR1;) {
        i = cstr_split2(str, i, len, " ,", " ,", ss), blendPositions[count] = atof(ss->s);
        i = cstr_split2(str, i, len, " ,", " ,", ss), str2Color(ss->s, ss->l, &presetColors[count]);
        ++count;
      }
      brush->SetInterpolationColors(presetColors, blendPositions, count);
      g->pen->SetBrush(brush);
      delete brush;
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
// 阴影
int gdiplus_shadowColor(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_shadowBlur(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_shadowOffsetX(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_shadowOffsetY(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
// 线条样式
int gdiplus_lineWidth(canvas_t* s, float px)
{
  GDIPARAM(s);
  g->pen->SetWidth(px);
  return 0;
}
int gdiplus_lineCap(canvas_t* s, int type)
{
  GDIPARAM(s);
  Gdiplus::LineCap lineCap = Gdiplus::LineCapFlat;
  Gdiplus::DashCap dashCap = Gdiplus::DashCapFlat;
  switch (type) {
  case lineCap_butt:
    lineCap = Gdiplus::LineCapFlat;
    dashCap = Gdiplus::DashCapFlat;
    break;
  case lineCap_round:
    lineCap = Gdiplus::LineCapRound;
    dashCap = Gdiplus::DashCapRound;
    break;
  case lineCap_square:
    lineCap = Gdiplus::LineCapSquare;
    dashCap = Gdiplus::DashCapTriangle;
    break;
  default:
    break;
  }
  g->pen->SetLineCap(lineCap, lineCap, dashCap);
  return 0;
}
int gdiplus_lineJoin(canvas_t* s, int type)
{
  GDIPARAM(s);
  Gdiplus::LineJoin lineJoin = Gdiplus::LineJoinMiter;
  switch (type) {
  case lineJoin_bevel:
    lineJoin = Gdiplus::LineJoinBevel;
    break;
  case lineJoin_round:
    lineJoin = Gdiplus::LineJoinRound;
    break;
  case lineCap_square:
    lineJoin = Gdiplus::LineJoinMiter;
    break;
  default:
    break;
  }
  g->pen->SetLineJoin(lineJoin);
  return 0;
}
int gdiplus_miterLimit(canvas_t* s, float x)
{
  GDIPARAM(s);
  g->pen->SetMiterLimit(x);
  return 0;
}
// 矩形
int gdiplus_rect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  g->path->AddRectangle(Rect(left, top, width, height));
  return 0;
}
int gdiplus_fillRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  g->myg->FillRectangle(g->brush, left, top, width, height);
  return 0;
}
int gdiplus_strokeRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  g->myg->DrawRectangle(g->pen, left, top, width, height);
  return 0;
}
int gdiplus_clearRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  g->myg->SetClip(Rect(left, top, width, height), CombineModeExclude);
  return 0;
}
int gdiplus_resetPath(canvas_t* s)
{
  GDIPARAM(s);
  g->path->Reset();
  g->ismoveed = FALSE;
  return 0;
}
// 路径
int gdiplus_beginPath(canvas_t* s)
{
  GDIPARAM(s);
  gdiplus_resetPath(s);
  return 0;
}
int gdiplus_closePath(canvas_t* s)
{
  GDIPARAM(s);
  g->path->CloseFigure();
  return 0;
}
int gdiplus_clip(canvas_t* s)
{
  GDIPARAM(s);
  g->myg->SetClip(g->path);
  return 0;
}
int gdiplus_fill(canvas_t* s)
{
  GDIPARAM(s);
  g->myg->FillPath(g->brush, g->path);
  return 0;
}
int gdiplus_stroke(canvas_t* s)
{
  GDIPARAM(s);
  g->myg->DrawPath(g->pen, g->path);
  return 0;
}
int gdiplus_moveTo(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  g->x = x, g->y = y;
  g->ismoveed = TRUE;
  return 0;
}
int gdiplus_lineTo(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  if (g->ismoveed) {
    g->path->AddLine(g->x, g->y, x, y);
  }
  g->x = x, g->y = y;
  g->ismoveed = TRUE;
  return 0;
}
int gdiplus_arcTo(canvas_t* s, xypos_t x1, xypos_t y1, xypos_t x2, xypos_t y2, xypos_t radius)
{
  GDIPARAM(s);
  circle_t c[2] = {{x1, y1, radius}, {x2, y2, radius}};
  DPOINT p[2] = {0};
  if (circle_insect(c, p)) {
    float x = p[1].x, y = p[1].y;
    float startAngle = atan2(y1 - y, x1 - x);
    float endAngle = atan2(y2 - y, x2 - x);
    g->path->AddArc(x - radius, y - radius, 2 * radius, 2 * radius, startAngle / RADIAN, (endAngle - startAngle) / RADIAN);
  }
  g->x = x2, g->y = y2;
  return 0;
}
int gdiplus_arc(canvas_t* s, xypos_t x, xypos_t y, xypos_t radius, xypos_t startAngle, xypos_t endAngle, BOOL anticlockwise)
{
  GDIPARAM(s);
  if (anticlockwise) {
    endAngle += 2 * PI;
  }
  g->path->AddArc(x - radius, y - radius, 2 * radius, 2 * radius, startAngle / RADIAN, (endAngle - startAngle) / RADIAN);
  return 0;
}
int gdiplus_quadraticCurveTo(canvas_t* s, xypos_t cpx, xypos_t cpy, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  Point pts[3] = {Point(g->x, g->y), Point(cpx, cpy), Point(x, y)};
  g->path->AddCurve(pts, 3);
  g->x = x, g->y = y;
  return 0;
}
int gdiplus_bezierCurveTo(canvas_t* s, xypos_t cpx1, xypos_t cpy1, xypos_t cpx2, xypos_t cpy2, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  g->path->AddBezier(g->x, g->y, cpx1, cpy1, cpx2, cpy2, x, y);
  g->x = x, g->y = y;
  return 0;
}
int gdiplus_isPointInPath(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0; // 如果指定的点位于当前路径中，则返回 true，否则返回 false
}
// 转换
int gdiplus_scale(canvas_t* s, float scalewidth, float scaleheight)
{
  GDIPARAM(s);
  Matrix mx;
  g->myg->GetTransform(&mx);
  mx.Scale(scalewidth, scaleheight);
  g->myg->SetTransform(&mx);
  return 0;
}
int gdiplus_rotate(canvas_t* s, float angle)
{
  GDIPARAM(s);
  Matrix mx;
  g->myg->GetTransform(&mx);
  mx.Rotate(angle / RADIAN);
  g->myg->SetTransform(&mx);
  return 0;
}
int gdiplus_translate(canvas_t* s, float x, float y)
{
  GDIPARAM(s);
  Matrix mx;
  g->myg->GetTransform(&mx);
  mx.Translate(x, y);
  g->myg->SetTransform(&mx);
  return 0;
}
int gdiplus_transform(canvas_t* s, float a, float b, float c, float d, float e, float f)
{
  GDIPARAM(s);
  Matrix mx, mx2;
  g->myg->GetTransform(&mx);
  mx2.SetElements(a, b, c, d, e, f);
  mx.Multiply(&mx2);
  g->myg->SetTransform(&mx);
  return 0;
}
int gdiplus_setTransform(canvas_t* s, float a, float b, float c, float d, float e, float f)
{
  GDIPARAM(s);
  Matrix mx;
  g->myg->GetTransform(&mx);
  mx.SetElements(a, b, c, d, e, f);
  g->myg->SetTransform(&mx);
  return 0;
}
// 文本
int gdiplus_font(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  LOGFONTA logfont[1] = {0};
  int i, j, k, l = strlen(str);
  str_t ss[1] = {0};
  int style = FontStyleRegular, weight = 400;
  Unit unit = UnitPixel;
  float size = 12;
  WORD* family = NULL;
  for (; ' ' == str[l - 1] || '\t' == str[l - 1];) {
    --l;
  }
  g->font->GetLogFontA(g->myg, logfont);
  for (i = 0; i < l;) {
    i = cstr_split2(str, i, l, " ", " ", ss);
    k = cstr_splitfind("|normal|italic|oblique|small-caps|bold|bolder|lighter|100|200|300|400|500|600|700|800|900|", -1, ss->s, ss->l, 1, NULL);
    switch (k) {
    case 1:
    case 2:
      style |= FontStyleItalic;
      break;
    case 4:
      style |= FontStyleBold;
      break;
    default:
      if (k > 6) {
        weight = atoi(ss->s);
      }
      else if (k < 0) {
        if (is0_9(ss->s[0])) {
          size = atof(ss->s);
          for (j = 0; is0_9(ss->s[j]) || '.' == ss->s[j]; ++j);
          j = cstr_splitfind("|px|em|pt|inch|", -1, ss->s + j, ss->l - j, 1, NULL);
          switch (j) {
          case 0:
          case 1:
            unit = UnitPixel;
            break;
          case 2:
            unit = UnitPoint;
            break;
          case 3:
            unit = UnitInch;
            break;
          default:
            break;
          }
        }
        else {
          my_mb2wc(&family, str + i, l - i);
        }
      }
      break;
    }
  }
  delete g->font;
  g->font = new Font(family, size, style, unit, NULL);
  FREE(family);
  return 0;
}
int gdiplus_textAlign(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  int k = cstr_splitfind("|left|start|end|right|center|", -1, str, -1, 1, NULL);
  if (k >= 0) {
    g->textAlign = k;
  }
  return 0;
}
int gdiplus_textBaseline(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_fillText(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth)
{
  GDIPARAM(s);
  WORD* wtext = NULL;
  int wlen = my_mb2wc(&wtext, text, textlen);
  PointF ptf(x, y);
  RectF rc;
  if (maxWidth < 0) {
    g->myg->MeasureString(wtext, wlen, g->font, ptf, &rc);
    maxWidth = rc.Width;
  }
  else {
    RectF rc1 = RectF(x, y, maxWidth, 1024);
    g->myg->MeasureString(wtext, wlen, g->font, ptf, &rc);
  }
  switch (g->textAlign) {
  case 2:
  case 3:
    x -= maxWidth;
    break;
  case 4:
    x -= maxWidth * 0.5f;
    break;
  default:
    break;
  }
  if (maxWidth > 0) {
    g->myg->DrawString(wtext, wlen, g->font, RectF(x, y, maxWidth, rc.Height), NULL, g->brush);
  }
  else {
    g->myg->DrawString(wtext, wlen, g->font, PointF(x, y), g->brush);
  }
  FREE(wtext);
  return 0;
}
GraphicsPath* GetStringPath(WORD* s, int len, float dpi, const RectF* rect, const Font* font, const StringFormat* format)
{
  GraphicsPath* path = new GraphicsPath();
  // Convert font size into appropriate coordinates
  float emSize = font->GetSize();
  FontFamily family;
  font->GetFamily(&family);
  path->AddString(s, len, &family, font->GetStyle(), emSize, *rect, format);
  return path;
}
int DrawStringStroke(Graphics* g, WORD* s, int len, const Pen* pen, const Brush* brush, const RectF* rect, const Font* font, const StringFormat* format)
{
  float dpi = g->GetDpiY();
  GraphicsPath* path = GetStringPath(s, len, dpi, rect, font, format);
  //阴影代码
  //RectangleF off = rect;
  //off.Offset(5, 5);//阴影偏移
  //using (GraphicsPath offPath = GetStringPath(s, dpi, off, font, format))
  //{
  // Brush b = new SolidBrush(Color.FromArgb(100, 0, 0, 0));
  // g.FillPath(b, offPath);
  // b.Dispose();
  //}
  // g->SmoothingMode = SmoothingMode.AntiAlias;//设置字体质量
  if (pen) {
    g->DrawPath(pen, path);//绘制轮廓（描边）
  }
  if (brush) {
    g->FillPath(brush, path);//填充轮廓（填充）
  }
  delete path;
  return 0;
}
int gdiplus_strokeText(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth)
{
  GDIPARAM(s);
  WORD* wtext = NULL;
  int wlen = my_mb2wc(&wtext, text, textlen);
  PointF ptf(x, y);
  RectF rc;
  if (maxWidth < 0) {
    g->myg->MeasureString(wtext, wlen, g->font, ptf, NULL, &rc);
    maxWidth = rc.Width;
  }
  else {
    RectF rc1 = RectF(x, y, maxWidth, 1024);
    g->myg->MeasureString(wtext, wlen, g->font, ptf, &rc);
  }
  switch (g->textAlign) {
  case 2:
  case 3:
    x -= maxWidth;
    break;
  case 4:
    x -= maxWidth * 0.5f;
    break;
  default:
    break;
  }
  if (maxWidth > 0) {
    RectF rc(x, y, maxWidth, rc.Height);
    DrawStringStroke(g->myg, wtext, wlen, g->pen, NULL, &rc, g->font, NULL);
  }
  else {
    g->myg->DrawString(wtext, wlen, g->font, PointF(x, y), g->brush);
  }
  FREE(wtext);
  return 0;
}
int gdiplus_measureText(canvas_t* s, const char* text, int textlen)
{
  GDIPARAM(s);
  RectF rc;
  PointF ptf(0, 0);
  WORD* wtext = NULL;
  int wlen = my_mb2wc(&wtext, text, textlen);
  g->myg->MeasureString(wtext, wlen, g->font, ptf, &rc);
  FREE(wtext);
  return rc.Width;
}
// 图像绘制
int gdiplus_drawImage(canvas_t* s, const img_t* img, int frame, const IRECT* rcimg, xypos_t x, xypos_t y, const ISIZE* sz)
{
  GDIPARAM(s);
  PixelFormat fmt[8] = {0};
  fmt[1] = PixelFormat8bppIndexed;
  fmt[3] = PixelFormat24bppRGB;
  fmt[4] = PixelFormat32bppARGB;
  frame %= MAX(img->f, 1);
  Bitmap* bmp = new Bitmap(img->w, img->h, img->s, fmt[img->c], img->tt.data + frame * img->s * img->h);
  if (sz == NULL) {
    g->myg->DrawImage(bmp, x, y);
  }
  else {
    if (NULL == rcimg) {
      g->myg->DrawImage(bmp, x, y, (REAL)sz->w, (REAL)sz->h);
    }
    else {
      g->myg->DrawImage(bmp, RectF(x, y, (REAL)sz->w, (REAL)sz->h), rcimg->l, rcimg->t, RCW(rcimg), RCH(rcimg), UnitPixel);
    }
  }
  delete bmp;
  return 0;
}
// 像素操作
int gdiplus_width(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_height(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_data(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_createImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_getImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_putImageData(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
// 合成
int gdiplus_globalAlpha(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdiplus_globalCompositeOperation(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // 设置或返回新图像如何绘制到已有的图像上
}
// 其他
int gdiplus_save(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // 保存当前环境的状态
}
int gdiplus_restore(canvas_t* s)
{
  GDIPARAM(s);
  return 0; // 返回之前保存过的路径状态和属性
}
int gdiplus_createEvent(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int gdiplus_getContext(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int gdiplus_toDataURL(canvas_t* s)
{
  GDIPARAM(s);
  return 0; //
}
int gdiplus_release(canvas_t* s)
{
  GDIPARAM(s);
  delete g->pen;
  delete g->brush;
  delete g->path;
  delete g->myg;
  memset(g, 0, sizeof(*g));
  FREE(s->x);
  return 0;
}
ULONG_PTR m_gdiplusToken = 0;
GdiplusStartupInput gdiplusStartupInput;
void gdiplus_exit(void)
{
  GdiplusShutdown(m_gdiplusToken);
}
int gdiplus_init()
{
  static int g_gdiplus_inited = 0;
  if (!g_gdiplus_inited) {
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    atexit(gdiplus_exit);
  }
  return 0;
}
#define GDI_FUNSET(name) s->name = gdiplus_##name;
int canvas_gdiplus_set(canvas_t* s, HDC hDC)
{
  canvas_gdiplus_t* g = NULL;
  CANVAS_FUN(GDI_FUNSET);
  MYREALLOC(g, 1);
  gdiplus_init();
  s->x = g;
  g->myg = Graphics::FromHDC(hDC);
  g->myg->SetSmoothingMode(SmoothingModeAntiAlias);
  g->pen = new Gdiplus::Pen(Color(Color::Black), 1);
  g->brush = new Gdiplus::SolidBrush(Color(Color::Black));
  g->font = new Gdiplus::Font(hDC);
  g->path = new Gdiplus::GraphicsPath();
  return 0;
}
#undef GDI_FUNSET
#undef GDIPARAM

