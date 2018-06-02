typedef struct canvas_gdi_t {
  HDC hDC;
  COLOR pen_color;
  int pen_width;
  int pen_style;
  COLOR brush_color;
  BOOL isbegpath;
  float miterLimit;
} canvas_gdi_t;
#define TORGB(x) (RGB(GetRV(x), GetGV(x), GetBV(x)))
#define GDIPARAM(s) canvas_gdi_t* g = (canvas_gdi_t*)(s)->x;
// 颜色、样式
int gdi_fillStyle(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  str_t ss[1] = {0};
  int len = strlen(str);
  int i = cstr_split(str, 0, len, " ", " ", ss);
  if ('#' == ss->s[0]) {
    color_parse(ss->s, ss->l, &g->brush_color);
  }
  else {
    int k = cstr_splitfind("|color|gradient|pattern|", -1, ss->s, ss->l, 1, 0);
    switch (k) {
    case 0:
      cstr_split(str, 0, len, " ", " ", ss);
      color_parse(ss->s, ss->l, &g->brush_color);
      break;
    case 1:
      break;
    }
  }
  DeleteObject(SelectObject(g->hDC, CreateSolidBrush(g->brush_color)));
  return 0;
}
int gdi_strokeStyle(canvas_t* s, const char* str)
{
  GDIPARAM(s);
  str_t ss[1] = {0};
  int len = strlen(str);
  int i = cstr_split(str, 0, len, " ", " ", ss);
  if ('#' == ss->s[0]) {
    color_parse(ss->s, ss->l, &g->brush_color);
  }
  else {
    int k = cstr_splitfind("|color|gradient|pattern|", -1, ss->s, ss->l, 1, 0);
    switch (k) {
    case 0:
      cstr_split(str, 0, len, " ", " ", ss);
      color_parse(ss->s, ss->l, &g->brush_color);
      break;
    case 1:
      break;
    }
  }
  DeleteObject(SelectObject(g->hDC, CreatePen(g->pen_style, g->pen_width, g->pen_color)));
  return 0;
}
// 阴影
int gdi_shadowColor(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdi_shadowBlur(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdi_shadowOffsetX(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdi_shadowOffsetY(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
// 线条样式
int gdi_lineWidth(canvas_t* s, float px)
{
  GDIPARAM(s);
  g->pen_width = xypos_toint(px);
  DeleteObject(SelectObject(g->hDC, CreatePen(g->pen_style, g->pen_width, g->pen_color)));
  return 0;
}
int gdi_lineCap(canvas_t* s, int type)
{
  GDIPARAM(s);
  g->pen_style &= ~PS_ENDCAP_MASK;
  switch (type) {
  case lineCap_butt:
    g->pen_style |= PS_ENDCAP_FLAT;
    break;
  case lineCap_round:
    g->pen_style |= PS_ENDCAP_ROUND;
    break;
  case lineCap_square:
    g->pen_style |= PS_ENDCAP_SQUARE;
    break;
  default:
    break;
  }
  DeleteObject(SelectObject(g->hDC, CreatePen(g->pen_style, g->pen_width, g->pen_color)));
  return 0;
}
int gdi_lineJoin(canvas_t* s, int type)
{
  GDIPARAM(s);
  g->pen_style &= ~PS_JOIN_MASK;
  switch (type) {
  case lineJoin_bevel:
    g->pen_style |= PS_JOIN_BEVEL;
    break;
  case lineJoin_round:
    g->pen_style |= PS_JOIN_ROUND;
    break;
  case lineCap_square:
    g->pen_style |= PS_JOIN_MITER;
    break;
  default:
    break;
  }
  DeleteObject(SelectObject(g->hDC, CreatePen(g->pen_style, g->pen_width, g->pen_color)));
  return 0;
}
int gdi_miterLimit(canvas_t* s, float x)
{
  GDIPARAM(s);
  SetMiterLimit(g->hDC, x, NULL);
  return 0;
}
// 矩形
int gdi_rect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  Rectangle(g->hDC, xypos_toint(left), xypos_toint(top), xypos_toint(left + width), xypos_toint(top + height));
  return 0;
}
int gdi_fillRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  return 0;
}
int gdi_strokeRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  BeginPath(g->hDC);
  DeleteObject(SelectObject(g->hDC, CreatePen(g->pen_style, g->pen_width, g->pen_color)));
  Rectangle(g->hDC, xypos_toint(left), xypos_toint(top), xypos_toint(left + width), xypos_toint(top + height));
  EndPath(g->hDC);
  StrokePath(g->hDC);
  return 0;
}
int gdi_clearRect(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height)
{
  GDIPARAM(s);
  return 0;
}
// 路径
int gdi_beginPath(canvas_t* s)
{
  GDIPARAM(s);
  if (!g->isbegpath) {
    g->isbegpath = 1;
    BeginPath(g->hDC);
  }
  return 0;
}
int gdi_closePath(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdi_clip(canvas_t* s)
{
  GDIPARAM(s);
  return 0;
}
int gdi_fill(canvas_t* s)
{
  GDIPARAM(s);
  if (g->isbegpath) {
    EndPath(g->hDC);
    g->isbegpath = 0;
  }
  FillPath(g->hDC);
  return 0;
}
int gdi_stroke(canvas_t* s)
{
  GDIPARAM(s);
  if (g->isbegpath) {
    EndPath(g->hDC);
    g->isbegpath = 0;
  }
  StrokePath(g->hDC);
  return 0;
}
int gdi_moveTo(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return MoveToEx(g->hDC, xypos_toint(x), xypos_toint(y), NULL);
}
int gdi_lineTo(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return LineTo(g->hDC, xypos_toint(x), xypos_toint(y));
}
int gdi_arcTo(canvas_t* s, xypos_t x1, xypos_t y1, xypos_t x2, xypos_t y2, xypos_t radius)
{
  GDIPARAM(s);
  return 0;
}
int gdi_arc(canvas_t* s, xypos_t x, xypos_t y, xypos_t radius, xypos_t startAngle, xypos_t endAngle, BOOL anticlockwise)
{
  double x1, y1;
  GDIPARAM(s);
  startAngle = -startAngle;
  endAngle = -endAngle;
  if (anticlockwise) {
    endAngle += 2 * PI;
  }
  x1 = x + radius * cos(startAngle);
  y1 = y - radius * sin(startAngle);
  MoveToEx(g->hDC, xypos_toint(x1), xypos_toint(y1), NULL);
  AngleArc(g->hDC, xypos_toint(x), xypos_toint(y), xypos_toint(radius), startAngle / RADIAN, (endAngle - startAngle) / RADIAN);
  //MoveToEx(g->hDC, xypos_toint(x + radius*cos(endAngle)), xypos_toint(y + radius*sin(endAngle)), NULL);
  return 0;
}
int gdi_quadraticCurveTo(canvas_t* s, xypos_t cpx, xypos_t cpy, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0;
}
int gdi_bezierCurveTo(canvas_t* s, xypos_t cpx1, xypos_t cpy1, xypos_t cpx2, xypos_t cpy2, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0;
}
int gdi_isPointInPath(canvas_t* s, xypos_t x, xypos_t y)
{
  GDIPARAM(s);
  return 0; // 如果指定的点位于当前路径中，则返回 true，否则返回 false
}
// 转换
int gdi_scale(canvas_t* s, float scalewidth, float scaleheight)
{
  GDIPARAM(s);
  return 0;
}
int gdi_rotate(canvas_t* s, float angle)
{
  GDIPARAM(s);
  return 0;
}
int gdi_translate(canvas_t* s, float x, float y)
{
  GDIPARAM(s);
  return 0;
}
int gdi_transform(canvas_t* s, float a, float b, float c, float d, float e, float f)
{
  GDIPARAM(s);
  return 0;
}
int gdi_setTransform(canvas_t* s, float a, float b, float c, float d, float e, float f)
{
  GDIPARAM(s);
  return 0;
}
// 文本
int gdi_font(canvas_t* s, const char* font)
{
  GDIPARAM(s);
  return 0;
}
int gdi_textAlign(canvas_t* s, const char* textAlign)
{
  GDIPARAM(s);
  return 0;
}
int gdi_textBaseline(canvas_t* s, const char* textBaseline)
{
  GDIPARAM(s);
  return 0;
}
int gdi_fillText(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth)
{
  GDIPARAM(s);
  return 0;
}
int gdi_strokeText(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth)
{
  GDIPARAM(s);
  return 0;
}
int gdi_measureText(canvas_t* s, const char* text, int textlen)
{
  return 0;
}
// 图像绘制
int gdi_drawImage(canvas_t* s, const img_t* img, int frame, const IRECT* rcimg, xypos_t x, xypos_t y, const ISIZE* sz)
{
  return 0;
}
// 像素操作
int gdi_width(canvas_t* s)
{
  return 0;
}
int gdi_height(canvas_t* s)
{
  return 0;
}
int gdi_data(canvas_t* s)
{
  return 0;
}
int gdi_createImageData(canvas_t* s)
{
  return 0;
}
int gdi_getImageData(canvas_t* s)
{
  return 0;
}
int gdi_putImageData(canvas_t* s)
{
  return 0;
}
// 合成
int gdi_globalAlpha(canvas_t* s)
{
  return 0;
}
int gdi_globalCompositeOperation(canvas_t* s)
{
  return 0; // 设置或返回新图像如何绘制到已有的图像上
}
// 其他
int gdi_save(canvas_t* s)
{
  return 0; // 保存当前环境的状态
}
int gdi_restore(canvas_t* s)
{
  return 0; // 返回之前保存过的路径状态和属性
}
int gdi_createEvent(canvas_t* s)
{
  return 0; //
}
int gdi_getContext(canvas_t* s)
{
  return 0; //
}
int gdi_toDataURL(canvas_t* s)
{
  return 0; //
}
int gdi_release(canvas_t* s)
{
  FREE(s->x);
  s->x = NULL;
  return 0;
}
#define GDI_FUNSET(name) s->name = gdi_##name;
int canvas_gdi_set(canvas_t* s, HDC hDC)
{
  canvas_gdi_t* g = {0};
  MYREALLOC(g, 1);
  g->hDC = hDC;
  g->pen_width = 1;
  g->miterLimit = 10;
  g->pen_color = RGB(0, 0, 0);
  g->pen_style = PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER;
  s->x = g;
  CANVAS_FUN(GDI_FUNSET);
  return 0;
}
#undef GDI_FUNSET
#undef GDIPARAM

