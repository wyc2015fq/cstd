//#region Static
/// Multiplies the color matrix (Extracted from .NET Framework
/// <param name="matrix1"></param>
/// <param name="matrix2"></param>
static int MultiplyColorMatrix(const float* matrix1, const float* matrix2, float* newColorMatrix)
{
  int j, k, m, num = 5;
  float numArray2[5];
  for (j = 0; j < num; j++) {
    for (k = 0; k < num; k++) {
      numArray2[k] = matrix1[k * 5 + j];
    }
    for (m = 0; m < num; m++) {
      const float* numArray3 = matrix2 + m * 5;
      float num6 = 0.f;
      for (int n = 0; n < num; n++) {
        num6 += numArray3[n] * numArray2[n];
      }
      newColorMatrix[m * 5 + j] = num6;
    }
  }
  return 0;
}
/// Gets the disabled image color matrix
static const float* DisabledImageColorMatrix()
{
  static float _disabledImageColorMatrix[25] = {0};
  static int inited = 0;
  if (0 == inited) {
    inited = 1;
    float numArray[25] = {
      0.2125f, 0.2125f, 0.2125f, 0.f, 0.f,
      0.2577f, 0.2577f, 0.2577f, 0.f, 0.f,
      0.0361f, 0.0361f, 0.0361f, 0.f, 0.f,
      0.f, 0.f, 0.f, 1.f, 0.f,
      0.38f, 0.38f, 0.38f, 0.f, 1.f
    };
    float numArray2[25] = {0};
    numArray2[0] = 1.f;
    numArray2[1 * 5 + 1] = 1.f;
    numArray2[2 * 5 + 2] = 1.f;
    numArray2[3 * 5 + 3] = 0.7f;
    MultiplyColorMatrix(numArray2, numArray, _disabledImageColorMatrix);
  }
  return _disabledImageColorMatrix;
}
static int trans_colormatrix_f32(int h, int w, const uchar* A, int al, int ai, const float* mf, uchar* B, int bl, int bi)
{
  int i, j, mi[25] = {0}, nn = ai * 10 + bi;
  int* m1[5];
  for (i = 0; i < 5; ++i) {
    m1[i] = mi + i * 5;
    for (j = 0; j < 5; ++j) {
      m1[i][j] = (int)(mf[i * 5 + j] * (1 << 8) + (1 << 7));
    }
  }
  switch (nn) {
#define REPEAT1(hh, X)  X(hh, 0)
#define REPEAT2(hh, X)  X(hh, 0) X(hh, 1)
#define REPEAT3(hh, X)  X(hh, 0) X(hh, 1) X(hh, 2)
#define REPEAT4(hh, X)  X(hh, 0) X(hh, 1) X(hh, 2) X(hh, 3)
#define TRANS_COLORMATRIX_DOT(DOTX, _i)   b1[_i] = (DOTX(a1, m1[_i])+m1[_i][5])>>8;
#define TRANS_COLORMATRIX_CASE(nn, hh)   case nn: { for (i=0; i<h; ++i) {const uchar* a1 = A + i*al;uchar* b1 = B + i*bl; for (j=0; j<w; ++j, a1+=ai, b1+=bi) { hh; } } } break;
    TRANS_COLORMATRIX_CASE(34, REPEAT4(DOT3, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(33, REPEAT3(DOT3, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(32, REPEAT2(DOT3, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(31, REPEAT1(DOT3, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(44, REPEAT4(DOT4, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(43, REPEAT3(DOT4, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(42, REPEAT2(DOT4, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(41, REPEAT1(DOT4, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(14, REPEAT4(DOT4, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(13, REPEAT3(DOT4, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(12, REPEAT2(DOT4, TRANS_COLORMATRIX_DOT));
    TRANS_COLORMATRIX_CASE(11, REPEAT1(DOT4, TRANS_COLORMATRIX_DOT));
#undef TRANS_COLORMATRIX_CASE
#undef TRANS_COLORMATRIX_DOT
    break;
  }
  return 0;
}
/// Creates the disabled image for the specified img_t
/// <param name="normalImage"></param>
static int CreateDisabledImage(const img_t* normalImage, img_t* out)
{
  const float* mf = DisabledImageColorMatrix();
  int h = normalImage->h, w = normalImage->w, c = normalImage->c;
  imsetsize(out, h, w, c, 1);
  trans_colormatrix_f32(h, w, normalImage->tt.data, normalImage->s, c, mf, out->tt.data, out->s, c);
  return 0;
}
//#endregion
COLOR ColorFromArgb(int R, int G, int B)
{
  return _RGB(R, G, B);
}
COLOR ColorFromArgb(int A, int R, int G, int B)
{
  return _RGBA(R, G, B, A);
}
COLOR ColorFromArgb(int A, int RGB)
{
  return _RGB_A(RGB, A);
}
struct RibbonProfesionalRendererColorTable {
  //#region Pendent for black
#define RIBBONPROFESIONALRENDERERCOLORTABLEDEF(a, b)  COLOR a;
#include "RibbonProfesionalRendererColorTabledef.txt"
#undef RIBBONPROFESIONALRENDERERCOLORTABLEDEF
  int init() {
#define RIBBONPROFESIONALRENDERERCOLORTABLEDEF(a, b)  a = b;
#include "RibbonProfesionalRendererColorTabledef.txt"
#undef RIBBONPROFESIONALRENDERERCOLORTABLEDEF
    return 0;
  }
  //#region Methods
  static char* Substring(char* buf, const char* str, int i, int len) {
    strncpy(buf, str + i, len);
    buf[len] = 0;
    return buf;
  }
  static COLOR FromHex(const char* hex) {
    int R = 0, G = 0, B = 0;
    COLOR clr = 0;
    if (hex[0] == ('#')) {
      ++hex;
    }
    if (strlen(hex) != 6) {
      ASSERT(0 && "Color not valid");
    }
    R = str_hex2int(hex + 0, 2);
    G = str_hex2int(hex + 2, 2);
    B = str_hex2int(hex + 4, 2);
    clr = ColorFromArgb(R, G, B);
    return clr;
  }
  static COLOR ToGray(COLOR c) {
    int m = (GetRV(c) + GetGV(c) + GetBV(c)) / 3;
    return ColorFromArgb(m, m, m);
  }
};
ISIZE arrowSize = iSIZE(5, 3);
ISIZE moreSize = iSIZE(7, 7);
struct RibbonProfessionalRenderer {
  //#region Props
  RibbonProfesionalRendererColorTable _colorTable;
  int init() {
    _colorTable.init();
    return 0;
  }
  COLOR GetTextColor(bool enabled) {
    return GetTextColor(enabled, _colorTable.Text);
  }
  COLOR GetTextColor(bool enabled, COLOR alternative) {
    if (enabled) {
      return alternative;
    }
    else {
      return _colorTable.ArrowDisabled;
    }
  }
  /// Draws a rectangle with a vertical gradient
  /// <param name="r"></param>
  /// <param name="northColor"></param>
  /// <param name="southColor"></param>
  int GradientRect(img_t* im, const IRECT* pclip, IRECT r, COLOR northColor, COLOR southColor) {
    brush_t br[1] = {0};
    brush_set_gradient(br, r.l, r.t, r.l, r.b, northColor, southColor);
    imdrawaa_rect_brush(im, pclip, r.l, r.t, r.r, r.b, br);
    return 0;
  }
  /// Draws a shadow that indicates that the element is pressed
  /// <param name="r"></param>
  int DrawPressedShadow(img_t* im, const IRECT* pclip, IRECT r) {
    int n;
    DPOINT pt[100];
    brush_t br[1] = {0};
    IRECT shadow = FromLTRB(r.l, r.t, r.r, r.t + 4);
    n = RoundRectangle(pt, 0, countof(pt), shadow, 3, Corners_NorthEast | Corners_NorthWest);
    brush_set_gradient(br, shadow.l, shadow.t, shadow.l, shadow.b, ColorFromArgb(50, Color_Black), ColorFromArgb(0, Color_Black));
    //b.WrapMode = WrapMode.TileFlipXY;
    //g.FillPath(b, path);
    imdrawaa_poly_brush(im, pclip, pt, &n, 1, br);
    return 0;
  }
  /// Draws an arrow on the specified bounds
  /// <param name="bounds"></param>
  /// <param name="c"></param>
  int DrawArrow(img_t* im, const IRECT* pclip, IRECT b, COLOR c, int d) {
    brush_t br[1] = {0};
    IRECT bounds = b;
    DPOINT pt[100] = {0};
    int i = 0;
    int n = countof(pt);
    if (RCW(&b) % 2 != 0 && (d == RibbonArrowDirection_Up)) {
      bounds = iRECT2(b.l - 1, b.t - 1, RCW(&b) + 1, RCH(&b) + 1);
    }
    if (d == RibbonArrowDirection_Up) {
      i = dPOINT_AddLine(pt, i, n, bounds.l, bounds.b, bounds.r, bounds.b);
      i = dPOINT_AddLine(pt, i, n, bounds.r, bounds.b, bounds.l + RCW(&bounds) / 2, bounds.t);
    }
    else if (d == RibbonArrowDirection_Down) {
      i = dPOINT_AddLine(pt, i, n, bounds.l, bounds.t, bounds.r , bounds.t);
      i = dPOINT_AddLine(pt, i, n, bounds.r, bounds.t, bounds.l + RCW(&bounds) / 2, bounds.b);
    }
    else if (d == RibbonArrowDirection_Left) {
      i = dPOINT_AddLine(pt, i, n, bounds.l, bounds.t, bounds.r, bounds.t + RCH(&bounds) / 2);
      i = dPOINT_AddLine(pt, i, n, bounds.r, bounds.t + RCH(&bounds) / 2, bounds.l, bounds.b);
    }
    else {
      i = dPOINT_AddLine(pt, i, n, bounds.r, bounds.t, bounds.l, bounds.t + RCH(&bounds) / 2);
      i = dPOINT_AddLine(pt, i, n, bounds.l, bounds.t + RCH(&bounds) / 2, bounds.r, bounds.b);
    }
    //dPOLYGON_CloseFigure(path);
    //(SolidBrush bb = new SolidBrush(c))
    brush_set_solid(br, c);
    brush_setaa(br, 0);
    imdrawaa_poly_brush(im, pclip, pt, &i, 1, br);
    return 0;
  }
  /// Draws the pair of arrows that make a shadded arrow, centered on the specified bounds
  /// <param name="b"></param>
  /// <param name="d"></param>
  /// <param name="enabled"></param>
  int DrawArrowShaded(img_t* im, const IRECT* pclip, IRECT b, int d, bool enabled) {
    ISIZE arrSize = arrowSize;
    if (d == RibbonArrowDirection_Left || d == RibbonArrowDirection_Right) {
      //Invert size
      arrSize = iSIZE(arrowSize.h, arrowSize.w);
    }
    IPOINT arrowP = iPOINT(b.l + (RCW(&b) - arrSize.w) / 2, b.t + (RCH(&b) - arrSize.h) / 2);
    IRECT bounds = iRECT2(arrowP.x, arrowP.y, arrSize.w, arrSize.h);
    IRECT boundsLight = bounds;
    iRectOffset2(&boundsLight, 0, 1);
    COLOR lt = _colorTable.ArrowLight;
    COLOR dk = _colorTable.Arrow;
    if (!enabled) {
      lt = Color_Transparent;
      dk = _colorTable.ArrowDisabled;
    }
    DrawArrow(im, pclip, boundsLight, lt, d);
    DrawArrow(im, pclip, bounds, dk, d);
    return 0;
  }
  /// Centers the specified rectangle on the specified container
  /// <param name="container"></param>
  /// <param name="r"></param>
  IRECT CenterOn(IRECT container, IRECT r) {
    IRECT result = iRECT(
        container.l + ((RCW(&container) - RCW(&r)) / 2),
        container.t + ((RCH(&container) - RCH(&r)) / 2),
        RCW(&r), RCH(&r));
    return result;
  }
  /// Draws a dot of the grip
  int DrawGripDot(img_t* im, const IRECT* pclip, int x, int y) {
    IRECT lt = iRECT(x - 1, y + 1, 2, 2);
    IRECT dk = iRECT2(x, y, 2, 2);
    imdraw_rect(im, pclip, lt, _colorTable.DropDownGripLight, 0, 0, 0);
    imdraw_rect(im, pclip, dk, _colorTable.DropDownGripDark, 0, 0, 0);
  }
  /// Creates the path of the tab and its contents
  int CreateCompleteTabPath(DPOINT* pt, int i, int n, const RibbonTab* t) {
    int corner = 6;
    const IRECT* tb = &t->_bounds;
    const IRECT* tcb = &t->_tabContentBounds;
    i = dPOINT_AddLine(pt, i, n, tb->l + corner, tb->t, tb->r - corner, tb->t);
    i = dPOINT_AddArc(pt, i, n, dRECT(tb->r - corner, tb->t, tb->r, tb->t + corner), -90, 90);
    i = dPOINT_AddLine(pt, i, n, tb->r, tb->t + corner, tb->r, tb->b - corner);
    i = dPOINT_AddArc(pt, i, n, dRECT(tb->r, tb->b - corner, tb->r + corner, tb->b), -180, -90);
    i = dPOINT_AddLine(pt, i, n, tb->r + corner, tb->b, tcb->r - corner, tb->b);
    i = dPOINT_AddArc(pt, i, n, dRECT(tcb->r - corner, tb->b, tcb->r, tb->b + corner), -90, 90);
    i = dPOINT_AddLine(pt, i, n, tcb->r, tcb->t + corner, tcb->r, tcb->b - corner);
    i = dPOINT_AddArc(pt, i, n, dRECT(tcb->r - corner, tcb->b - corner, tcb->r, tcb->b), 0, 90);
    i = dPOINT_AddLine(pt, i, n, tcb->r - corner, tcb->b, tcb->l + corner, tcb->b);
    i = dPOINT_AddArc(pt, i, n, dRECT(tcb->l, tcb->b - corner, tcb->l + corner, tcb->b), 90, 90);
    i = dPOINT_AddLine(pt, i, n, tcb->l, tcb->b - corner, tcb->l, tb->b + corner);
    i = dPOINT_AddArc(pt, i, n, dRECT(tcb->l, tb->b, tcb->l + corner, tb->b + corner), 180, 90);
    i = dPOINT_AddLine(pt, i, n, tcb->l + corner, tcb->t, tb->l - corner, tb->b);
    i = dPOINT_AddArc(pt, i, n, dRECT(tb->l - corner, tb->b - corner, tb->l, tb->b), 90, -90);
    i = dPOINT_AddLine(pt, i, n, tb->l, tb->b - corner, tb->l, tb->t + corner);
    i = dPOINT_AddArc(pt, i, n, dRECT(tb->l, tb->t, tb->l + corner, tb->t + corner), 180, 90);
    return i;
  }
  /// Creates the path of the tab and its contents
  /// <param name="tab"></param>
  int CreateTabPath(DPOINT* pt, int i, int n, const RibbonTab* t) {
    int corner = 6;
    int rightOffset = 1;
    const IRECT* tb = &t->_bounds;
    const IRECT* tcb = &t->_tabContentBounds;
    i = dPOINT_AddLine(pt, i, n, tb->l, tb->b, tb->l, tb->t + corner);
    i = dPOINT_AddArc(pt, i, n, dRECT(tb->l, tb->t, corner, corner), 180, 90);
    i = dPOINT_AddLine(pt, i, n, tb->l + corner, tb->t, tb->r - corner - rightOffset, tb->t);
    i = dPOINT_AddArc(pt, i, n, dRECT(tb->r - corner - rightOffset, tb->t, corner, corner), -90, 90);
    i = dPOINT_AddLine(pt, i, n, tb->r - rightOffset, tb->t + corner, tb->r - rightOffset, tb->b);
    return i;
  }
  /// Draws a complete tab
  int DrawTabNormal(img_t* im, const IRECT* pclip, RibbonTab* t) {
    const IRECT* tb = &t->_bounds;
    IRECT clip = FromLTRB(tb->l, tb->t, tb->r, tb->b);
    IRECT r = FromLTRB(tb->l - 1, tb->t - 1, tb->r, tb->b);
    imdraw_rect(im, &clip, r, _colorTable.RibbonBackground, 0, 0, 0);
    return 0;
  }
  /// Draws an active tab
  int DrawTabActive(img_t* im, const IRECT* pclip, RibbonTab* t) {
    const IRECT* tb = &t->_bounds;
    IRECT glossy = iRECT2(tb->l, tb->t, RCW(tb), 4);
    IRECT shadow = *tb, tab = *tb;
    DPOINT pt[100];
    int n;
    brush_t br[1] = {0};
    COLOR Colors[] = {Color_Transparent, ColorFromArgb(50, Color_Black), ColorFromArgb(100, Color_Black)};
    double pos[] = { 0, .1, 1 };
    iRectOffset2(&shadow, 2, 1);
    //tab.Inflate(0, 1);
    //DrawTabNormal(im, pclip, t);
    n = RoundRectangle(pt, 0, countof(pt), shadow, 6, Corners_NorthWest | Corners_NorthEast);
    fill_color_array2(256, br->clrs, 3, Colors, pos);
    brush_set_lut_gradient(br, shadow.l, shadow.t, shadow.l, shadow.b, br->clrs);
    imdrawaa_poly_brush(im, pclip, pt, &n, 1, br);
    n = RoundRectangle(pt, 0, countof(pt), tab, 6, Corners_North);
    {
      COLOR north = _colorTable.TabNorth;
      COLOR south = _colorTable.TabSouth;
      brush_set_lut_gradient(br, shadow.l, shadow.t, shadow.l, shadow.b, br->clrs);
      imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.TabNorth, 1.6f);
      brush_set_gradient(br, shadow.l, shadow.t, shadow.l, shadow.b, _colorTable.TabNorth, _colorTable.TabSouth);
      imdrawaa_poly_brush(im, pclip, pt, &n, 1, br);
    }
    n = RoundRectangle(pt, 0, countof(pt), glossy, 6, Corners_North);
    imdrawaa_poly_solid(im, pclip, pt, &n, 1, ColorFromArgb(180, Color_White));
    return 0;
  }
  /// Draws a complete tab
  int DrawCompleteTab(img_t* im, const IRECT* pclip, RibbonTab* t) {
    DPOINT pt[100];
    int n;
    brush_t br[1] = {0};
    const IRECT* tb = &t->_bounds;
    const IRECT* tcb = &t->_tabContentBounds;
    DrawTabActive(im, pclip, t);
    n = RoundRectangle(pt, 0, countof(pt), *tcb, 4);
    //Background gradient
    {
      COLOR north = _colorTable.TabContentNorth;
      COLOR south = _colorTable.TabContentSouth;
      brush_set_gradient(br, 0, tcb->t + 30, 0, tcb->b - 10, north, south);
      imdrawaa_poly_brush(im, tcb, pt, &n, 1, br);
    }
    //Glossy effect
    IRECT glossy = FromLTRB(tcb->l, tcb->t + 0, tcb->r, tcb->t + 18);
    n = RoundRectangle(pt, 0, countof(pt), glossy, 6, Corners_NorthWest | Corners_NorthEast);
    imdrawaa_poly_solid(im, pclip, pt, &n, 1, ColorFromArgb(30, Color_White));
    //Tab border
    n = CreateCompleteTabPath(pt, 0, countof(pt), t);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.TabBorder, 1);
    if (t->_selected) {
      //_selected glow
      n = CreateTabPath(pt, 0, countof(pt), t);
      imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, ColorFromArgb(150, Color_Gold), 2);
    }
    return 0;
  }
  /// Draws a selected tab
  int DrawTabSelected(img_t* im, const IRECT* pclip, RibbonTab* t) {
    int n;
    DPOINT pt[100];
    brush_t br[1] = {0};
    const IRECT* tb = &t->_bounds;
    const IRECT* tcb = &t->_tabContentBounds;
    IRECT outerR = FromLTRB(tb->l, tb->t, tb->r - 1, tb->b);
    IRECT innerR = FromLTRB(outerR.l + 1, outerR.t + 1, outerR.r - 1, outerR.b);
    IRECT glossyR = FromLTRB(innerR.l + 1, innerR.t + 1, innerR.r - 1, innerR.t + RCH(tb) / 2);
    n = RoundRectangle(pt, 0, countof(pt), outerR, 3, Corners_NorthEast | Corners_NorthWest);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.TabBorder, 1);
    n = RoundRectangle(pt, 0, countof(pt), innerR, 3, Corners_NorthEast | Corners_NorthWest);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, ColorFromArgb(200, Color_White), 1);
    {
      double Factors[] = { 0.0f, 0.9f, 0.0f };
      double Positions[] = { 0.0f, 0.8f, 1.0f };
      fill_color_array3(256, br->clrs, Color_Transparent, _colorTable.TabSelectedGlow, 3, Positions, Factors);
      brush_set_lut_radial(br, innerR.l + RCW(&innerR) / 2, innerR.t - 5, RCW(&innerR) / 2, RCH(&innerR) / 2, br->clrs);
      imdrawaa_rect_brush(im, &innerR, innerR.l, innerR.t, innerR.r, innerR.b, br);
    }
    n = RoundRectangle(pt, 0, countof(pt), glossyR, 3, Corners_NorthEast | Corners_NorthWest);
    imdrawaa_poly_solid(im, pclip, pt, &n, 1, ColorFromArgb(100, Color_White));
    return 0;
  }
  /// Draws a pressed tab
  int DrawTabPressed(img_t* im, const IRECT* pclip, RibbonTab* t) {
    return 0;
  }
  int DrawButtonMoreGlyph(img_t* im, const IRECT* pclip, int x, int y, COLOR color) {
    /*
     a-------b-+
     |         |
     |         d
     c     g   |
     |         |
     +----e----f
     */
    IPOINT a = iPOINT(x, y);
    IPOINT b = iPOINT(x + moreSize.w - 1, y);
    IPOINT c = iPOINT(x, y + moreSize.h - 1);
    IPOINT f = iPOINT(x + moreSize.w, y + moreSize.h);
    IPOINT d = iPOINT(f.x, f.y - 3);
    IPOINT e = iPOINT(f.x - 3, f.y);
    IPOINT g = iPOINT(f.x - 3, f.y - 3);
    imdraw_line(im, pclip, a, b, color, 1);
    imdraw_line(im, pclip, a, c, color, 1);
    imdraw_line(im, pclip, e, f, color, 1);
    imdraw_line(im, pclip, d, f, color, 1);
    imdraw_line(im, pclip, e, d, color, 1);
    imdraw_line(im, pclip, g, f, color, 1);
    return 0;
  }
  int DrawButtonMoreGlyph(img_t* im, const IRECT* pclip, IRECT b, bool enabled) {
    COLOR dark = enabled ? _colorTable.Arrow : _colorTable.ArrowDisabled;
    COLOR light = _colorTable.ArrowLight;
    IRECT bounds = CenterOn(b, iRECT2(0, 0, moreSize.w, moreSize.h));
    IRECT boundsLight = bounds;
    iRectOffset2(&boundsLight, 1, 1);
    DrawButtonMoreGlyph(im, pclip, boundsLight.l, boundsLight.t, light);
    DrawButtonMoreGlyph(im, pclip, bounds.l, bounds.t, dark);
    return 0;
  }
  IRECT LargeButtonDropDownArrowBounds(const font_t* font, const char* text, IRECT textLayout) {
    IRECT bounds = {0};
    ISIZE lastCharSize = font_text_size(font, text, -1, DT_CENTER | DT_VCENTER);
    bool moreWords = NULL != strchr(text, ' ');
    if (moreWords) {
      return iRECT2(lastCharSize.w + 3, (lastCharSize.h - arrowSize.h) / 2, arrowSize.w, arrowSize.h);
    }
    else {
      return iRECT(textLayout.l + (RCW(&textLayout) - arrowSize.w) / 2,
          lastCharSize.h + ((textLayout.b - lastCharSize.h) - arrowSize.h) / 2, arrowSize.w, arrowSize.h);
    }
  }
  /// Draws the arrow of buttons
  int DrawButtonDropDownArrow(img_t* im, const IRECT* pclip, RibbonButton* button, IRECT textLayout, const font_t* font) {
    IRECT bounds = {0};
    if (button->c->_sizeMode == RibbonElementSizeMode_Large || button->c->_sizeMode == RibbonElementSizeMode_Overflow) {
      bounds = LargeButtonDropDownArrowBounds(font, button->c->_text, textLayout);
    }
    else {
      //bounds = iRECT(
      //    fb->r + (ddb->Width() - arrowSize.w) / 2,
      //    button->_bounds.t + (button->RCH(&_bounds) - arrowSize.h) / 2,
      //    arrowSize.w, arrowSize.h);
      bounds = textLayout;
    }
    DrawArrowShaded(im, pclip, bounds, button->_DropDownArrowDirection, button->c->_enabled);
    return 0;
  }
#include "aaa.inl"
  /// Draws a regular button in normal state
  static int DrawButtonImpl(img_t* im, const IRECT* pclip, IRECT bounds, int corners, COLOR clrOut, COLOR clrCenter, COLOR clrBorderOut, COLOR clrBorderIn, COLOR clrGlossyNorth, COLOR clrGlossySouth) {
    int n, n2;
    DPOINT pt[100];
    DPOINT pt2[100];
    brush_t br[1] = {0};
    if (RCH(&bounds) <= 0 || RCW(&bounds) <= 0) {
      return 0;
    }
    IRECT outerR = FromLTRB(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
    IRECT innerR = FromLTRB(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
    IRECT glossyR = FromLTRB(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + (int)((double)RCH(&bounds) * .36));
    n = RoundRectangle(pt, 0, countof(pt), outerR, 3, corners);
    imdrawaa_poly_solid(im, &bounds, pt, &n, 1, clrOut);
    //#region Main Bg
    n2 = dPOINT_AddEllipse(pt2, 0, 100, iRECT(bounds.l, bounds.t, RCW(&bounds), RCH(&bounds) * 2));
    double Factors[] = { 0, 0.8, 0 };
    double Positions[] = { 0, 0.30, 1 };
    fill_color_array3(256, br->clrs, clrCenter, clrOut, 3, Positions, Factors);
    //brush_set_lut_radial(br, bounds.l + RCW(&bounds) / 2, bounds.b, RCW(&bounds) / 2, RCH(&bounds) / 2, br->clrs);
    //imdrawaa_poly_brush(im, &bounds, pt2, &n2, 1, br);
    imdrawaa_poly_lut_radial(im, &bounds, pt2, &n2, 1, bounds.l + RCW(&bounds) / 2, bounds.b, RCW(&bounds) / 2, RCH(&bounds) / 2, br->clrs);
    //Border
    imdrawaa_poly_stroke_solid(im, &bounds, pt, n, 1, clrBorderOut, 1);
    //Inner border
    n = RoundRectangle(pt, 0, countof(pt), innerR, 3, corners);
    imdrawaa_poly_stroke_solid(im, &bounds, pt, n, 1, clrBorderIn, 1);
    //Glossy effect
    if (RCW(&glossyR) > 0 && RCH(&glossyR) > 0) {
      n = RoundRectangle(pt, 0, countof(pt), glossyR, 3, (corners & Corners_NorthWest) | (corners & Corners_NorthEast));
      imdrawaa_poly_gradient_hv(im, &bounds, pt, &n, 1, glossyR, 1, clrGlossyNorth, clrGlossySouth);
    }
    return 0;
  }
  /// Draws a regular button in normal state
  int DrawButton(img_t* im, const IRECT* pclip, IRECT bounds, int corners) {
    DrawButtonImpl(im, pclip, bounds, corners, _colorTable.ButtonBgOut, _colorTable.ButtonBgCenter, _colorTable.ButtonBorderOut, _colorTable.ButtonBorderIn, _colorTable.ButtonGlossyNorth, _colorTable.ButtonGlossySouth);
    return 0;
  }
  /// Draws the button as checked
  int DrawButtonChecked(img_t* im, const IRECT* pclip, IRECT bounds, Corners corners) {
    IRECT outerR = FromLTRB(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
    DrawButtonImpl(im, pclip, bounds, corners, _colorTable.ButtonCheckedBgOut, _colorTable.ButtonCheckedBgCenter, _colorTable.ButtonCheckedBorderOut, _colorTable.ButtonCheckedBorderIn, _colorTable.ButtonCheckedGlossyNorth, _colorTable.ButtonCheckedGlossySouth);
    DrawPressedShadow(im, pclip, outerR);
    return 0;
  }
  /// Draws a regular button in disabled state
  int DrawButtonDisabled(img_t* im, const IRECT* pclip, IRECT bounds, Corners corners) {
    DrawButtonImpl(im, pclip, bounds, corners, _colorTable.ButtonDisabledBgOut, _colorTable.ButtonDisabledBgCenter, _colorTable.ButtonDisabledBorderOut, _colorTable.ButtonDisabledBorderIn, _colorTable.ButtonDisabledGlossyNorth, _colorTable.ButtonDisabledGlossySouth);
    return 0;
  }
  /// Draws a regular button in pressed state
  int DrawButtonPressed(img_t* im, const IRECT* pclip, IRECT bounds, Corners corners) {
    IRECT outerR = FromLTRB(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
    DrawButtonImpl(im, &bounds, bounds, corners, _colorTable.ButtonPressedBgOut, _colorTable.ButtonPressedBgCenter, _colorTable.ButtonPressedBorderOut, _colorTable.ButtonPressedBorderIn, _colorTable.ButtonPressedGlossyNorth, _colorTable.ButtonPressedGlossySouth);
    DrawPressedShadow(im, &bounds, outerR);
    return 0;
  }
  /// Draws a regular buttton in selected state
  int DrawButtonSelected(img_t* im, const IRECT* pclip, IRECT bounds, Corners corners) {
    return DrawButtonImpl(im, pclip, bounds, corners, _colorTable.ButtonSelectedBgOut, _colorTable.ButtonSelectedBgCenter, _colorTable.ButtonSelectedBorderOut, _colorTable.ButtonSelectedBorderIn, _colorTable.ButtonSelectedGlossyNorth, _colorTable.ButtonSelectedGlossySouth);
  }
  /// Gets the corners to round on the specified button
  /// <param name="r"></param>
  Corners ButtonCorners(RibbonButton* button) {
    Corners c = (Corners)button->_corners;
#if 0
    if (!(button->OwnerItem is RibbonItemGroup)) {
      c = Corners_All;
    }
    else {
      RibbonItemGroup g = button->OwnerItem as RibbonItemGroup;
      c = Corners_None;
      if (button == g.FirstItem) {
        c |= Corners_West;
      }
      if (button == g.LastItem) {
        c |= Corners_East;
      }
    }
#endif
    return c;
  }
  /// Determines buttonface corners
  Corners ButtonFaceRounding(RibbonButton* button) {
    return (Corners)button->_faceRounding;
#if 0
    if (!(button->OwnerItem is RibbonItemGroup)) {
      if (button->c->_sizeMode == RibbonElementSizeMode_Large) {
        return Corners_North;
      }
      else {
        return Corners_West;
      }
    }
    else {
      Corners c = Corners_None;
      RibbonItemGroup g = button->OwnerItem as RibbonItemGroup;
      if (button == g.FirstItem) {
        c |= Corners_West;
      }
      return c;
    }
#endif
  }
  /// Determines button's dropDown corners
  Corners ButtonDdRounding(RibbonButton* button) {
    return (Corners)button->_ddRounding;
#if 0
    if (!(button->OwnerItem is RibbonItemGroup)) {
      if (button->c->_sizeMode == RibbonElementSizeMode_Large) {
        return Corners_South;
      }
      else {
        return Corners_East;
      }
    }
    else {
      Corners c = Corners_None;
      RibbonItemGroup g = button->OwnerItem as RibbonItemGroup;
      if (button == g.LastItem) {
        c |= Corners_East;
      }
      return c;
    }
#endif
  }
  /// Draws the orb's option buttons background
  /// <param name="bounds"></param>
  int DrawOrbOptionButton(img_t* im, const IRECT* pclip, IRECT bounds) {
    int n;
    DPOINT pt[100];
    bounds.r -= 1;
    bounds.b -= 1;
    n = RoundRectangle(pt, 0, countof(pt), bounds, 3);
    imdrawaa_poly_solid(im, pclip, pt, &n, 1, _colorTable.OrbOptionBackground);
    GradientRect(im, pclip, FromLTRB(bounds.l, bounds.t + RCH(&bounds) / 2, bounds.r, bounds.b - 2),
        _colorTable.OrbOptionShine, _colorTable.OrbOptionBackground);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.OrbOptionBorder, 1);
    return 0;
  }
  /// Draws the button as pressed
  int DrawButtonPressed(img_t* im, const IRECT* pclip, RibbonButton* button) {
    DrawButtonPressed(im, pclip, button->c->_bounds, ButtonCorners(button));
    return 0;
  }
  /// Draws the button as Checked
  int DrawButtonChecked(img_t* im, const IRECT* pclip, RibbonButton* button) {
    DrawButtonChecked(im, pclip, button->c->_bounds, ButtonCorners(button));
    return 0;
  }
  /// Draws the button as a selected button
  int DrawButtonSelected(img_t* im, const IRECT* pclip, RibbonButton* button) {
    DrawButtonSelected(im, pclip, button->c->_bounds, ButtonCorners(button));
    return 0;
  }
  //#region Panel
  /// Draws a panel in normal state (unselected)
  int DrawPanelNormal(img_t* im, const IRECT* pclip, const RibbonPanel* t) {
    int n;
    DPOINT pt[100];
    brush_t br[1] = {0};
    const IRECT* b = &t->_bounds;
    const IRECT* cb = &t->_contentBounds;
    IRECT darkBorder = FromLTRB(b->l, b->t, b->r, b->b);
    IRECT lightBorder = FromLTRB(b->l + 1, b->t + 1, b->r + 1, b->b);
    IRECT textArea = FromLTRB(b->l + 1, cb->b, b->r - 1, b->b - 1);
    n = RoundRectangle(pt, 0, countof(pt), lightBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelLightBorder, 1);
    n = RoundRectangle(pt, 0, countof(pt), darkBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelDarkBorder, 1);
    n = RoundRectangle(pt, 0, countof(pt), textArea, 3, Corners_SouthEast | Corners_SouthWest);
    imdrawaa_poly_solid(im, pclip, pt, &n, 1, _colorTable.PanelTextBackground);
    if (t->_buttonMoreVisible) {
      DrawButtonMoreGlyph(im, pclip, t->_buttonMoreBounds, t->_buttonMoreEnabled && t->_enabled);
    }
    return 0;
  }
  /// Draws a panel in selected state
  int DrawPanelSelected(img_t* im, const IRECT* pclip, const RibbonPanel* t) {
    int n;
    DPOINT pt[100];
    const IRECT* b = &t->_bounds;
    const IRECT* cb = &t->_contentBounds;
    IRECT darkBorder = FromLTRB(b->l, b->t, b->r, b->b);
    IRECT lightBorder = FromLTRB(b->l + 1, b->t + 1, b->r - 1, b->b - 1);
    IRECT textArea = FromLTRB(b->l + 1, cb->b, b->r - 1, b->b - 1);
    n = RoundRectangle(pt, 0, countof(pt), lightBorder, 3);
    imdrawaa_poly(im, pclip, pt, n, 1, _colorTable.PanelBackgroundSelected, _colorTable.PanelLightBorder, 1);
    n = RoundRectangle(pt, 0, countof(pt), darkBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelDarkBorder, 1);
    n = RoundRectangle(pt, 0, countof(pt), textArea, 3, Corners_SouthEast | Corners_SouthWest);
    imdrawaa_poly_solid(im, pclip, pt, &n, 1, _colorTable.PanelTextBackgroundSelected);
    if (t->_buttonMoreVisible) {
      if (t->_buttonMorePressed) {
        DrawButtonPressed(im, pclip, t->_buttonMoreBounds, Corners_SouthEast);
      }
      else if (t->_buttonMoreSelected) {
        DrawButtonSelected(im, pclip, t->_buttonMoreBounds, Corners_SouthEast);
      }
      DrawButtonMoreGlyph(im, pclip, t->_buttonMoreBounds, t->_buttonMoreEnabled && t->_enabled);
    }
    return 0;
  }
  /// Draws an overflown panel in normal state
  int DrawPanelOverflowNormal(img_t* im, const IRECT* pclip, const RibbonPanel* t, const font_t* fo) {
    int n;
    DPOINT pt[100];
    const IRECT* b = &t->_bounds;
    const IRECT* cb = &t->_contentBounds;
    IRECT darkBorder = FromLTRB(b->l, b->t, b->r, b->b);
    IRECT lightBorder = FromLTRB(b->l + 1, b->t + 1, b->r - 1, b->b - 1);
    n = RoundRectangle(pt, 0, countof(pt), lightBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelLightBorder, 1);
    n = RoundRectangle(pt, 0, countof(pt), darkBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelDarkBorder, 1);
    DrawPanelOverflowImage(im, pclip, t, fo);
    return 0;
  }
  /// Draws an overflown panel in selected state
  int DrawPannelOveflowSelected(img_t* im, const IRECT* pclip, const RibbonPanel* t, const font_t* fo) {
    int n;
    DPOINT pt[100];
    const IRECT* b = &t->_bounds;
    const IRECT* cb = &t->_contentBounds;
    IRECT darkBorder = FromLTRB(b->l, b->t, b->r, b->b);
    IRECT lightBorder = FromLTRB(b->l + 1, b->t + 1, b->r - 1, b->b - 1);
    n = RoundRectangle(pt, 0, countof(pt), darkBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelDarkBorder, 1);
    n = RoundRectangle(pt, 0, countof(pt), lightBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelLightBorder, 1);
    imdrawaa_poly_gradient_hv(im, pclip, pt, &n, 1, lightBorder, 1, _colorTable.PanelOverflowBackgroundSelectedNorth, Color_Transparent);
    DrawPanelOverflowImage(im, pclip, t, fo);
    return 0;
  }
  /// Draws an overflown panel in pressed state
  int DrawPanelOverflowPressed(img_t* im, const IRECT* pclip, const RibbonPanel* t, const font_t* fo) {
    int n, n2;
    DPOINT pt[100];
    DPOINT pt2[100];
    const IRECT* b = &t->_bounds;
    const IRECT* cb = &t->_contentBounds;
    IRECT darkBorder = FromLTRB(b->l, b->t, b->r, b->b);
    IRECT lightBorder = FromLTRB(b->l + 1, b->t + 1, b->r - 1, b->b - 1);
    IRECT glossy = FromLTRB(b->l, b->t, b->r, b->t + 17);
    n = RoundRectangle(pt, 0, countof(pt), darkBorder, 3);
    imdrawaa_poly_gradient_hv(im, pclip, pt, &n, 1, lightBorder, 1, _colorTable.PanelOverflowBackgroundPressed, _colorTable.PanelOverflowBackgroundSelectedSouth);
    n2 = RoundRectangle(pt2, 0, countof(pt), glossy, 3, Corners_NorthEast | Corners_NorthWest);
    imdrawaa_poly_gradient_hv(im, pclip, pt2, &n2, 1, glossy, 1, ColorFromArgb(150, Color_White), ColorFromArgb(50, Color_White));
    n2 = RoundRectangle(pt2, 0, countof(pt), lightBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt2, n2, 1, ColorFromArgb(40, Color_White), 1);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelDarkBorder, 1);
    DrawPanelOverflowImage(im, pclip, t, fo);
    DrawPressedShadow(im, pclip, glossy);
    return 0;
  }
  /// Draws the image of the panel when collapsed
  int DrawPanelOverflowImage(img_t* im, const IRECT* pclip, const RibbonPanel* t, const font_t* fo) {
    int n, n2;
    DPOINT pt[100];
    DPOINT pt2[100];
    const IRECT* b = &t->_bounds;
    const IRECT* cb = &t->_contentBounds;
    int margin = 3;
    ISIZE imgSquareSize = iSIZE(32, 32);
    IRECT imgSquareR = iRECT2(b->l + (RCW(b) - imgSquareSize.w) / 2, b->t + 5, imgSquareSize.w, imgSquareSize.h);
    IRECT imgSquareBottomR = FromLTRB(imgSquareR.l, imgSquareR.b - 10, imgSquareR.r, imgSquareR.b);
    IRECT textR = FromLTRB(b->l + margin, imgSquareR.b + margin, b->r - margin, b->b - margin);
    n = RoundRectangle(pt, 0, countof(pt), imgSquareR, 5);
    imdrawaa_poly_gradient_hv(im, pclip, pt, &n, 1, imgSquareR, 1, _colorTable.TabContentNorth, _colorTable.TabContentSouth);
    n2 = RoundRectangle(pt2, 0, countof(pt), imgSquareBottomR, 5, Corners_South);
    imdrawaa_poly_solid(im, pclip, pt2, &n2, 1, _colorTable.PanelTextBackground);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelDarkBorder, 1);
    if (t->_image != NULL) {
      IRECT rc = iRECT2(imgSquareR.l + (RCW(&imgSquareR) - t->_image->w) / 2,
          imgSquareR.t + ((RCH(&imgSquareR) - RCH(&imgSquareBottomR)) - t->_image->h) / 2, t->_image->w, t->_image->h);
      imdraw_image(im, pclip, &rc, t->_image, 0);
    }
    COLOR crText;
    crText = GetTextColor(t->_enabled);
    imdraw_text(im, b, textR, t->_text, -1, fo, crText, 0, DT_CENTER | DT_VCENTER);
    IRECT bounds = LargeButtonDropDownArrowBounds(fo, t->_text, textR);
    if (bounds.r < b->r) {
      IRECT boundsLight = bounds;
      iRectOffset2(&boundsLight, 0, 1);
      COLOR lt = _colorTable.ArrowLight;
      COLOR dk = _colorTable.Arrow;
      DrawArrow(im, pclip, boundsLight, lt, RibbonArrowDirection_Down);
      DrawArrow(im, pclip, bounds, dk, RibbonArrowDirection_Down);
    }
    return 0;
  }
  /// Draws a SplitDropDown button in normal state
  int DrawSplitButton(img_t* im, const IRECT* pclip, RibbonButton* button) {
    return 0;
  }
  /// Draws a SplitDropDown button in pressed state
  int DrawSplitButtonPressed(img_t* im, const IRECT* pclip, RibbonButton* button) {
  }
  /// Draws a SplitDropDown button in selected state
  int DrawSplitButtonSelected(img_t* im, const IRECT* pclip, RibbonButton* button) {
    int n;
    DPOINT pt[100];
    const IRECT* ddb = &button->_dropDownBounds;
    const IRECT* fb = &button->_buttonFaceBounds;
    IRECT outerR = FromLTRB(ddb->l, ddb->t, ddb->r - 1, ddb->b - 1);
    //IRECT innerR = FromLTRB(outerR.l + 1,outerR.t + 1,outerR.r - 1,outerR.b - 1);
    IRECT faceOuterR = FromLTRB(fb->l, fb->t, fb->r - 1, fb->b - 1);
    IRECT faceInnerR = FromLTRB(faceOuterR.l + 1, faceOuterR.t + 1, faceOuterR.r + (button->c->_sizeMode == RibbonElementSizeMode_Large ? -1 : 0), faceOuterR.b + (button->c->_sizeMode == RibbonElementSizeMode_Large ? 0 : -1));
    Corners faceCorners = ButtonFaceRounding(button);
    Corners ddCorners = ButtonDdRounding(button);
    COLOR clr;
    //n = RoundRectangle(pt, 0, countof(pt), faceOuterR, 3, faceCorners);
    n = RoundRectangle(pt, 0, countof(pt), outerR, 3, ddCorners);
    clr = button->c->_pressed && button->c->_sizeMode != RibbonElementSizeMode_DropDown ? _colorTable.ButtonPressedBorderOut : _colorTable.ButtonSelectedBorderOut;
    imdrawaa_poly(im, pclip, pt, n, 1, ColorFromArgb(150, Color_White), clr, 1);
    n = RoundRectangle(pt, 0, countof(pt), faceInnerR, 2, faceCorners);
    clr = button->c->_pressed && button->c->_sizeMode != RibbonElementSizeMode_DropDown ? _colorTable.ButtonPressedBorderIn : _colorTable.ButtonSelectedBorderIn;
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, clr, 1);
    return 0;
  }
  /// Draws a SplitDropDown button with the dropdown area pressed
  int DrawSplitButtonDropDownPressed(img_t* im, const IRECT* pclip, RibbonButton* button) {
  }
  /// Draws a SplitDropDown button with the dropdown area selected
  int DrawSplitButtonDropDownSelected(img_t* im, const IRECT* pclip, RibbonButton* button) {
    int n;
    DPOINT pt[100];
    const IRECT* ddb = &button->_dropDownBounds;
    const IRECT* fb = &button->_buttonFaceBounds;
    //IRECT outerR = FromLTRB(ddb->l,ddb->t,ddb->r - 1,ddb->b - 1);
    //IRECT innerR = FromLTRB(outerR.l + 1,outerR.t + (button->c->_sizeMode == RibbonElementSizeMode_Large ? 1 : 0),outerR.r - 1,outerR.b - 1);
    IRECT faceOuterR = FromLTRB(fb->l, fb->t, fb->r - 1, fb->b - 1);
    IRECT faceInnerR = FromLTRB(faceOuterR.l + 1, faceOuterR.t + 1, faceOuterR.r + (button->c->_sizeMode == RibbonElementSizeMode_Large ? -1 : 0), faceOuterR.b + (button->c->_sizeMode == RibbonElementSizeMode_Large ? 0 : -1));
    Corners faceCorners = ButtonFaceRounding(button);
    Corners ddCorners = ButtonDdRounding(button);
    COLOR clr;
    n = RoundRectangle(pt, 0, countof(pt), faceInnerR, 2, faceCorners);
    clr = button->c->_pressed && button->c->_sizeMode != RibbonElementSizeMode_DropDown ? _colorTable.ButtonPressedBorderIn : _colorTable.ButtonSelectedBorderIn;
    imdrawaa_poly(im, pclip, pt, n, 1, ColorFromArgb(150, Color_White), clr, 1);
    //n = RoundRectangle(pt, 0, countof(pt), outerR, 3, ddCorners);
    //n = RoundRectangle(pt, 0, countof(pt), innerR, 2, ddCorners);
    n = RoundRectangle(pt, 0, countof(pt), faceOuterR, 3, faceCorners);
    clr = button->c->_pressed && button->c->_sizeMode != RibbonElementSizeMode_DropDown ? _colorTable.ButtonPressedBorderOut : _colorTable.ButtonSelectedBorderOut;
    return 0;
  }
  //#region Group
  /// Draws the background of the specified  RibbonItemGroup
  /// <param name="?"></param>
  int DrawItemGroup(img_t* im, const IRECT* pclip, RibbonItemGroup* grp) {
    int n;
    DPOINT pt[100];
    IRECT outerR = FromLTRB(grp->_bounds.l, grp->_bounds.t, grp->_bounds.r - 1, grp->_bounds.b - 1);
    IRECT innerR = FromLTRB(outerR.l + 1, outerR.t + 1, outerR.r - 1, outerR.b - 1);
    IRECT glossyR = FromLTRB(outerR.l + 1, outerR.t + RCH(&outerR) / 2 + 1, outerR.r - 1, outerR.b - 1);
    n = RoundRectangle(pt, 0, countof(pt), outerR, 2);
    n = RoundRectangle(pt, 0, countof(pt), innerR, 2);
    imdrawaa_poly_gradient_hv(im, pclip, pt, &n, 1, innerR, 1, _colorTable.ItemGroupBgNorth, _colorTable.ItemGroupBgSouth);
    n = RoundRectangle(pt, 0, countof(pt), glossyR, 2);
    imdrawaa_poly_gradient_hv(im, pclip, pt, &n, 1, glossyR, 1, _colorTable.ItemGroupBgGlossy, Color_Transparent);
    return 0;
  }
  /// Draws the background of the specified  RibbonItemGroup
  /// <param name="?"></param>
  int DrawItemGroupBorder(img_t* im, const IRECT* pclip, RibbonItemGroup* grp) {
    int n;
    DPOINT pt[100];
    IRECT outerR = FromLTRB(grp->_bounds.l, grp->_bounds.t, grp->_bounds.r - 1, grp->_bounds.b - 1);
    IRECT innerR = FromLTRB(outerR.l + 1, outerR.t + 1, outerR.r - 1, outerR.b - 1);
    COLOR dark = _colorTable.ItemGroupSeparatorDark;
    COLOR light = _colorTable.ItemGroupSeparatorLight;
#if 0
    foreach(RibbonItem item in grp.Items) {
      if (item == grp.LastItem) {
        break;
      }
      im.DrawLine(dark,
          iPOINT(item->_bounds.r, item->_bounds.t),
          iPOINT(item->_bounds.r, item->_bounds.b));
      im.DrawLine(light,
          iPOINT(item->_bounds.r + 1, item->_bounds.t),
          iPOINT(item->_bounds.r + 1, item->_bounds.b));
    }
#endif
    n = RoundRectangle(pt, 0, countof(pt), outerR, 2);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.ItemGroupOuterBorder, 1);
    n = RoundRectangle(pt, 0, countof(pt), innerR, 2);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.ItemGroupInnerBorder, 1);
    return 0;
  }
  //#region ButtonList
  int DrawButtonList(img_t* im, const IRECT* pclip, RibbonButtonList* list) {
#if 0
    int n;
    DPOINT pt[100];
    IRECT outerR = FromLTRB(list->_bounds.l, list->_bounds.t, list->_bounds.r - 1, list->_bounds.b);
    n = RoundRectangle(pt, 0, countof(pt), outerR, 3, Corners_East);
    {
      COLOR bgcolor = list->_selected ? _colorTable.ButtonListBgSelected : _colorTable.ButtonListBg;
      if (list->Canvas is RibbonDropDown) {
        bgcolor = _colorTable.DropDownBg;
      }
      using(brush_set_solid(br, bgcolor)) {
        g.FillPath(b, path);
      }
      using(Pen p = new Pen(_colorTable.ButtonListBorder)) {
        g.DrawPath(p, path);
      }
    }
    {
      //#region Control Buttons
      if (list->ScrollType == RibbonButtonList.ListScrollType.Scrollbar) {
        using(brush_set_solid(br, _colorTable.ButtonGlossyNorth)) {
          g.FillRectangle(b, list->ScrollBarBounds);
        }
      }
      if (!list->ButtonDownEnabled) {
        DrawButtonDisabled(im, list->ButtonDownBounds, list->ButtonDropDownPresent ? Corners_None : Corners_SouthEast);
      }
      else if (list->ButtonDownPressed) {
        DrawButtonPressed(im, list->ButtonDownBounds, list->ButtonDropDownPresent ? Corners_None : Corners_SouthEast);
      }
      else if (list->ButtonDownSelected) {
        DrawButtonSelected(im, list->ButtonDownBounds, list->ButtonDropDownPresent ? Corners_None : Corners_SouthEast);
      }
      else {
        DrawButton(im, list->ButtonDownBounds, Corners_None);
      }
      if (!list->ButtonUpEnabled) {
        DrawButtonDisabled(im, list->_buttonUpBounds, Corners_NorthEast);
      }
      else if (list->ButtonUpPressed) {
        DrawButtonPressed(im, list->_buttonUpBounds, Corners_NorthEast);
      }
      else if (list->ButtonUpSelected) {
        DrawButtonSelected(im, list->_buttonUpBounds, Corners_NorthEast);
      }
      else {
        DrawButton(im, list->_buttonUpBounds, Corners_NorthEast);
      }
      if (list->ButtonDropDownPresent) {
        if (list->ButtonDropDownPressed) {
          DrawButtonPressed(im, list->ButtonDropDownBounds, Corners_SouthEast);
        }
        else if (list->ButtonDropDownSelected) {
          DrawButtonSelected(im, list->ButtonDropDownBounds, Corners_SouthEast);
        }
        else {
          DrawButton(im, list->ButtonDropDownBounds, Corners_SouthEast);
        }
      }
      if (list->ScrollType == RibbonButtonList.ListScrollType.Scrollbar && list->ScrollBarEnabled) {
        if (list->ThumbPressed) {
          DrawButtonPressed(im, list->ThumbBounds, Corners_All);
        }
        else if (list->ThumbSelected) {
          DrawButtonSelected(im, list->ThumbBounds, Corners_All);
        }
        else {
          DrawButton(im, list->ThumbBounds, Corners_All);
        }
      }
      COLOR dk = _colorTable.Arrow;
      COLOR lt = _colorTable.ArrowLight;
      COLOR ds = _colorTable.ArrowDisabled;
      IRECT arrUp = CenterOn(list->_buttonUpBounds, iRECT(IPOINT.Empty, arrowSize));
      arrUp.Offset(0, 1);
      IRECT arrD = CenterOn(list->ButtonDownBounds, iRECT(IPOINT.Empty, arrowSize));
      arrD.Offset(0, 1);
      IRECT arrdd = CenterOn(list->ButtonDropDownBounds, iRECT(IPOINT.Empty, arrowSize));
      arrdd.Offset(0, 3);
      DrawArrow(im, arrUp, list->ButtonUpEnabled ? lt : Color_Transparent, RibbonArrowDirection_Up);
      arrUp.Offset(0, -1);
      DrawArrow(im, arrUp, list->ButtonUpEnabled ? dk : ds, RibbonArrowDirection_Up);
      DrawArrow(im, arrD, list->ButtonDownEnabled ? lt : Color_Transparent, RibbonArrowDirection_Down);
      arrD.Offset(0, -1);
      DrawArrow(im, arrD, list->ButtonDownEnabled ? dk : ds, RibbonArrowDirection_Down);
      if (list->ButtonDropDownPresent) {
        using(brush_set_solid(br, _colorTable.Arrow)) {
          SmoothingMode sm = g.SmoothingMode;
          g.SmoothingMode = SmoothingMode.None;
          g.FillRectangle(b, iRECT(iPOINT(arrdd.l - 1, arrdd.t - 4), iSIZE(arrowSize.w + 2, 1)));
          g.SmoothingMode = sm;
        }
        DrawArrow(im, arrdd, lt, RibbonArrowDirection_Down);
        arrdd.Offset(0, -1);
        DrawArrow(im, arrdd, dk, RibbonArrowDirection_Down);
      }
    }
#endif
    return 0;
  }
  //#region Separator
  int DrawSeparator(img_t* im, const IRECT* pclip, IRECT bounds, const char* text, int sizeMode) {
    if (sizeMode == RibbonElementSizeMode_DropDown) {
      if (text) {
        imdraw_rect(im, pclip, bounds, _colorTable.SeparatorBg, 0, 0, 0);
        imdrawaa_line(im, pclip, bounds.l, bounds.b, bounds.r, bounds.b, _colorTable.SeparatorLine, 1);
      }
      else {
        imdrawaa_line(im, pclip, bounds.l + 30, bounds.t + 1, bounds.r, bounds.t + 1, _colorTable.DropDownImageSeparator, 1);
      }
    }
    else {
      imdrawaa_line(im, pclip, bounds.l, bounds.t, bounds.l, bounds.b, _colorTable.SeparatorDark, 1);
      imdrawaa_line(im, pclip, bounds.l + 1, bounds.t, bounds.l + 1, bounds.b, _colorTable.SeparatorLight, 1);
    }
    return 0;
  }
  //#region TextBox
  /// Draws a disabled textbox
  /// <param name="bounds"></param>
  int DrawTextBoxDisabled(img_t* im, const IRECT* pclip, IRECT bounds) {
    imdraw_rect(im, pclip, bounds, COLOR_3DFACE, _colorTable.TextBoxBorder, 1, BDM_ALL);
    return 0;
  }
  /// Draws an unselected textbox
  /// <param name="bounds"></param>
  int DrawTextBoxUnselected(img_t* im, const IRECT* pclip, IRECT bounds) {
    imdraw_rect(im, pclip, bounds, _colorTable.TextBoxUnselectedBg, _colorTable.TextBoxBorder, 1, BDM_ALL);
    return 0;
  }
  /// Draws an unselected textbox
  /// <param name="bounds"></param>
  int DrawTextBoxSelected(img_t* im, const IRECT* pclip, IRECT bounds) {
    imdraw_rect(im, pclip, bounds, COLOR_WINDOW, _colorTable.TextBoxBorder, 1, BDM_ALL);
    return 0;
  }
  //#region ComboBox
  int DrawComboxDropDown(img_t* im, const IRECT* pclip, RibbonComboBox* combo) {
    if (combo->_dropDownPressed) {
      DrawButtonPressed(im, pclip, combo->_dropDownBounds, Corners_None);
    }
    else if (combo->_dropDownSelected) {
      DrawButtonSelected(im, pclip, combo->_dropDownBounds, Corners_None);
    }
    else if (combo->_selected) {
      DrawButton(im, pclip, combo->_dropDownBounds, Corners_None);
    }
    DrawArrowShaded(im, pclip, combo->_dropDownBounds, RibbonArrowDirection_Down, true);//b->c->_enabled);
  }
  //#region Quick Access and Caption Bar
  int DrawCaptionBarBackground(img_t* im, const IRECT* pclip, IRECT r) {
    int i;
    COLOR colors[][2] = {
      { _colorTable.Caption1, _colorTable.Caption2 },
      { _colorTable.Caption3, _colorTable.Caption4 },
      { _colorTable.Caption5, _colorTable.Caption6 },
      { _colorTable.Caption7, _colorTable.Caption7 }
    };
    IRECT rects[4] = {0};
    rects[0] = iRECT2(r.l, r.t, RCW(&r), 4);
    rects[1] = iRECT2(r.l, rects[0].b, RCW(&r), 4);
    rects[2] = iRECT2(r.l, rects[1].b, RCW(&r), RCH(&r) - 9);
    rects[3] = iRECT2(r.l, rects[2].b, RCW(&r), 1);
    for (i = 0; i < 4; ++i) {
      IRECT grect = rects[i];
      grect.b += 2;
      grect.t -= 1;
      imdraw_rect_gradient(im, pclip, rects[i], colors[i][0], colors[i][1], 0);
    }
    return 0;
  }
  int DrawCaptionBarText(img_t* im, const IRECT* pclip, IRECT captionBar, const char* text, const font_t* fo) {
#if 0
    if (t->_actualBorderMode == RibbonWindowMode_NonClientAreaGlass) {
      path.AddString(f._text, ft.FontFamily, (int)ft.Style, ft.SizeInPoints + 3, captionBar, sf);
      if (f.WindowState != FormWindowState.Maximized) {
      }
      COLOR clr = ColorFromArgb(90, Color_White);
      im.FillPath(f.WindowState == FormWindowState.Maximized ? Brushes.White : Brushes.Black, path);
    }
    else if (t->_actualBorderMode == RibbonWindowMode_NonClientAreaCustomDrawn) {
      TextRenderer.DrawText(im, f._text, ft, captionBar, _colorTable.FormBorder);
    }
    //Console.WriteLine("capt " + DateTime.Now.Millisecond + e.ClipRectangle.ToString());
    //WinApi.FillForGlass(im, captionBar);
    //WinApi.DrawTextOnGlass(t->Handle, f._text, SystemFonts.CaptionFont, captionBar, 10);
#endif
    imdraw_text(im, pclip, captionBar, text, -1, fo, _colorTable.FormBorder, 0, DT_VCENTER);
    return 0;
  }
  IPOINT iPointOffset(IPOINT pt, int x, int y) {
    pt.x += x;
    pt.y += y;
    return pt;
  }
  int CreateQuickAccessPath(DPOINT* pt, int i, int n, IPOINT a, IPOINT b, IPOINT c, IPOINT d, IPOINT e, IRECT bounds, int offsetx, int offsety, bool orbVisible) {
    a = iPointOffset(a, offsetx, offsety);
    b = iPointOffset(b, offsetx, offsety);
    c = iPointOffset(c, offsetx, offsety);
    d = iPointOffset(d, offsetx, offsety);
    e = iPointOffset(e, offsetx, offsety);
    i = dPOINT_AddLine(pt, i, n, a.x, a.y, b.x, b.y);
    i = dPOINT_AddArc(pt, i, n, dRECT2(b.x - RCH(&bounds) / 2, b.y, RCH(&bounds), RCH(&bounds)), -90, 180);
    i = dPOINT_AddLine(pt, i, n, d, c);
    if (orbVisible) {
      i = dPOINT_AddCurve(pt, i, n, c, e, a);
    }
    else {
      i = dPOINT_AddArc(pt, i, n, dRECT2(a.x - RCH(&bounds) / 2, a.y, RCH(&bounds), RCH(&bounds)), 90, 180);
    }
    return i;
  }
  int OnRenderRibbonQuickAccessToolbarBackground(img_t* im, const IRECT* pclip, RibbonQuickAccessToolbar* t, bool orbVisible, int actualBorderMode) {
    /// a-----b    a-----b
    ///  z    |   z      |
    ///   c---d    c-----d
    IRECT bounds = t->_bounds;
    IRECT padding = t->_padding;
    IRECT margin = t->_margin;
    IPOINT a = iPOINT(bounds.l - (orbVisible ?  margin.l : 0), bounds.t);
    IPOINT b = iPOINT(bounds.r + padding.r, bounds.t);
    IPOINT c = iPOINT(bounds.l, bounds.b);
    IPOINT d = iPOINT(b.x, c.y);
    IPOINT z = iPOINT(c.x - 2, a.y + RCH(&bounds) / 2 - 1);
    bool aero = actualBorderMode == RibbonWindowMode_NonClientAreaGlass;
    DPOINT pt[100];
    int n;
    if (!aero) {
      n = CreateQuickAccessPath(pt, 0, countof(pt), a, b, c, d, z, bounds, 0, 0, orbVisible);
      imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.QuickAccessBorderLight, 3);
    }
#if 0
    using(GraphicsPath path = CreateQuickAccessPath(a, b, c, d, z, bounds, 0, 0, orbVisible)) {
      using(Pen p = new Pen(_colorTable.QuickAccessBorderDark)) {
        if (aero) {
          p.Color = ColorFromArgb(150, 150, 150) ;
        }
        im.DrawPath(p, path);
      }
      if (!aero) {
        using(LinearGradientBrush br = new LinearGradientBrush(
            b, d, ColorFromArgb(150, _colorTable.QuickAccessUpper), ColorFromArgb(150, _colorTable.QuickAccessLower)
        )) {
          im.FillPath(br, path);
        }
      }
      else {
        using(LinearGradientBrush br = new LinearGradientBrush(
            b, d,
            ColorFromArgb(66, RibbonProfesionalRendererColorTable.ToGray(_colorTable.QuickAccessUpper)),
            ColorFromArgb(66, RibbonProfesionalRendererColorTable.ToGray(_colorTable.QuickAccessLower))
        )) {
          im.FillPath(br, path);
        }
      }
    }
#endif
    return 0;
  }
  /// Draws the orb on the specified state
  /// <param name="g">Device to draw</param>
  /// <param name="ribbon">Ribbon that the orb belongs to</param>
  /// <param name="r">Layout rectangle for the orb</param>
  /// <param name="selected">Specifies if the orb should be drawn as selected</param>
  /// <param name="pressed">Specifies if the orb should be drawn as pressed</param>
  int DrawOrb(img_t* im, const IRECT* pclip, IRECT r, const img_t* image, bool selected, bool pressed) {
    int sweep, start;
    IPOINT p1, p2, p3;
    COLOR bgdark, bgmed, bglight, light;
    IRECT rinner = r;
    iRectInflate2(&rinner, -1, -1);
    IRECT shadow = r;
    iRectOffset2(&shadow, 1, 1);
    iRectInflate2(&shadow, 2, 2);
    int n;
    DPOINT pt[100];
    brush_t br[1] = {0};
    //#region COLOR Selection
    if (pressed) {
      bgdark = _colorTable.OrbPressedBackgroundDark;
      bgmed = _colorTable.OrbPressedBackgroundMedium;
      bglight = _colorTable.OrbPressedBackgroundLight;
      light = _colorTable.OrbPressedLight;
    }
    else if (selected) {
      bgdark = _colorTable.OrbSelectedBackgroundDark;
      bgmed = _colorTable.OrbSelectedBackgroundDark;
      bglight = _colorTable.OrbSelectedBackgroundLight;
      light = _colorTable.OrbSelectedLight;
    }
    else {
      bgdark = _colorTable.OrbBackgroundDark;
      bgmed = _colorTable.OrbBackgroundMedium;
      bglight = _colorTable.OrbBackgroundLight;
      light = _colorTable.OrbLight;
    }
    //#region Shadow
    {
      double Factors[] = { 0, 1, 1 };
      double Positions[] = { 0, 0.2, 1 };
      n = dPOINT_AddEllipse(pt, 0, 100, shadow);
      fill_color_array3(256, br->clrs, ColorFromArgb(180, Color_Black), Color_Transparent, 3, Positions, Factors);
      brush_set_lut_radial(br, shadow.l + RCW(&shadow) / 2, shadow.t + RCH(&shadow) / 2, RCW(&shadow) / 2, RCH(&shadow) / 2, br->clrs);
      imdrawaa_irect_brush(im, &shadow, shadow, br);
    }
    //#region Orb Background
    imdrawaa_ellipse_irect(im, &shadow, shadow, 0, bgdark, 1);
    {
      double Factors[] = { 0, 0.8, 1 };
      double Positions[] = { 0, 0.5, 1 };
      n = dPOINT_AddEllipse(pt, 0, 100, r);
      fill_color_array3(256, br->clrs, bglight, bgmed, 3, Positions, Factors);
      brush_set_lut_radial(br, r.l + RCW(&r) / 2, r.b, RCW(&r) / 2, RCH(&r) / 2, br->clrs);
      imdrawaa_irect_brush(im, &r, r, br);
    }
    //#region Bottom round shine
    IRECT bshine = iRECT2(r.l + (RCW(&r) / 2) / 2, r.t + RCH(&r) / 2, RCW(&r) / 2, RCH(&r) / 2);
    {
      n = dPOINT_AddEllipse(pt, 0, 100, bshine);
      fill_color_array(256, br->clrs, Color_White, Color_Transparent);
      brush_set_lut_radial(br, r.l + RCW(&r) / 2, r.b, RCW(&r) / 2, RCH(&r) / 2, br->clrs);
      imdrawaa_irect_brush(im, &r, r, br);
    }
    //#region Upper Glossy
    {
      sweep = 160;
      start = 180 + (180 - sweep) / 2;
      n = dPOINT_AddArc(pt, 0, countof(pt), rinner, start, sweep);
      p1 = iPOINT_PT(pt[0]);
      p2 = iPOINT_PT(pt[n - 1]);
      p3 = iPOINT(rinner.l + RCW(&rinner) / 2, p2.y - 3);
      n = dPOINT_AddCurve(pt, n, countof(pt), p2, p3, p1);
      {
        double Factors[] = { .3, .8, 1 };
        double Positions[] = { 0, 0.5, 1 };
        n = dPOINT_AddEllipse(pt, 0, 100, r);
        fill_color_array3(256, br->clrs, Color_Transparent, light, 3, Positions, Factors);
        brush_set_lut_radial(br, p3.x, p3.y, RCW(&rinner), RCH(&rinner) / 2, br->clrs);
        imdrawaa_irect_brush(im, &r, r, br);
      }
      {
        double Factors[] = { 0, .4, .8, 1 };
        double Positions[] = { 0, .3, .4, 1 };
        fill_color_array3(256, br->clrs, Color_White, Color_Transparent, 4, Positions, Factors);
        brush_set_lut_gradient(br, r.l, r.t, r.l, p1.y, br->clrs);
        imdrawaa_poly_brush(im, &r, pt, &n, 1, br);
      }
    }
    //#region Upper Shine
    /////Lower gloss
    //using (GraphicsPath p = new GraphicsPath())
    //{
    //    sweep = 140;
    //    start = (180 - sweep) / 2;
    //    p.AddArc(rinner, start, sweep);
    //    p1 = IPOINT.Round(p.PathData.Points[0]);
    //    p2 = IPOINT.Round(p.PathData.Points[p.PathData.Points.Length - 1]);
    //    p3 = iPOINT(rinner.l + rinneRCW(&r) / 2, p1.y + 3);
    //    p.AddCurve(iPOINT[] { p2, p3, p1 });
    //    g.FillPath(Brushes.White, p);
    //}
    ///Upper shine
    {
      sweep = 160;
      start = 180 + (180 - sweep) / 2;
      n = dPOINT_AddArc(pt, 0, countof(pt), rinner, start, sweep);
      imdrawaa_poly(im, pclip, pt, n, 1, 0, Color_White, 1);
    }
    //#region Lower Shine
    {
      sweep = 160;
      start = (180 - sweep) / 2;
      n = dPOINT_AddArc(pt, 0, countof(pt), rinner, start, sweep);
      IPOINT pt1 = iPOINT_PT(pt[0]);
      IRECT rrinner = rinner;
      iRectInflate2(&rrinner, -1, -1);
      sweep = 160;
      start = (180 - sweep) / 2;
      n = dPOINT_AddArc(pt, n, countof(pt), rrinner, start, sweep);
      imdrawaa_poly_gradient(im, pclip, pt, &n, 1, rinner.l, rinner.b, rinner.l, pt1.y - 1, light, ColorFromArgb(50, light));
      //p1 = IPOINT.Round(p.PathData.Points[0]);
      //p2 = IPOINT.Round(p.PathData.Points[p.PathData.Points.Length - 1]);
      //p3 = iPOINT(rinner.l + rinneRCW(&r) / 2, rinner.b - 1);
      //p.AddCurve(iPOINT[] { p2, p3, p1 });
      //using (LinearGradientBrush b = new LinearGradientBrush(
      //    iPOINT(rinner.l, rinner.b + 1),
      //    iPOINT(rinner.l, p1.y),
      //    ColorFromArgb(200, Color_White), Color_Transparent))
      //{
      //    g.FillPath(b, p);
      //}
    }
    //#region Orb Icon
    if (image != NULL) {
      IRECT irect = iRECT(r.l + (RCW(&r) - image->w) / 2, r.t + (RCH(&r) - image->h) / 2, image->w, image->h);
      imdraw_image(im, pclip, &irect, image, 0);
    }
    return 0;
  }
  int OnRenderRibbonOrb(img_t* im, const IRECT* pclip, Ribbon* t) {
    if (t->_orbVisible) {
      DrawOrb(im, pclip, t->_orbBounds, t->_orbImage, t->_orbSelected, t->_orbPressed);
    }
    return 0;
  }
  //#region Ribbon Orb DropDown
  int OnRenderOrbDropDownBackground(img_t* im, const IRECT* pclip, Ribbon* t, IRECT _contentBounds, IRECT ContentButtonsBounds) {
    int Width = RCW(&_contentBounds);
    int Height = RCH(&_contentBounds);
    IRECT OrbDDContent = _contentBounds;
    IRECT Bcontent = ContentButtonsBounds;
    IRECT OuterRect = iRECT2(0, 0, Width - 1, Height - 1);
    IRECT InnerRect = iRECT2(1, 1, Width - 3, Height - 3);
    IRECT NorthNorthRect = iRECT2(1, 1, Width - 3, OrbDDContent.t / 2);
    IRECT northSouthRect = iRECT2(1, NorthNorthRect.b, RCW(&NorthNorthRect), OrbDDContent.t / 2);
    IRECT southSouthRect = FromLTRB(1, (Height - OrbDDContent.b) / 2 + OrbDDContent.b, Width - 1, Height - 1);
    COLOR OrbDropDownDarkBorder = _colorTable.OrbDropDownDarkBorder;
    COLOR OrbDropDownLightBorder = _colorTable.OrbDropDownLightBorder;
    COLOR OrbDropDownBack = _colorTable.OrbDropDownBack;
    COLOR OrbDropDownNorthA = _colorTable.OrbDropDownNorthA;
    COLOR OrbDropDownNorthB = _colorTable.OrbDropDownNorthB;
    COLOR OrbDropDownNorthC = _colorTable.OrbDropDownNorthC;
    COLOR OrbDropDownNorthD = _colorTable.OrbDropDownNorthD;
    COLOR OrbDropDownSouthC = _colorTable.OrbDropDownSouthC;
    COLOR OrbDropDownSouthD = _colorTable.OrbDropDownSouthD;
    COLOR OrbDropDownContentbg = _colorTable.OrbDropDownContentbg;
    COLOR OrbDropDownContentbglight = _colorTable.OrbDropDownContentbglight;
    COLOR OrbDropDownSeparatorlight = _colorTable.OrbDropDownSeparatorlight;
    COLOR OrbDropDownSeparatordark = _colorTable.OrbDropDownSeparatordark;
    DPOINT pt[100];
    int n;
    COLOR clr;
    brush_t br[1] = {0};
    //im.SmoothingMode = SmoothingMode.None;
    clr = ColorFromArgb(0x8e, 0x8e, 0x8e);
    imdraw_rect(im, pclip, iRECT2(Width - 10, Height - 10, 10, 10), clr, 0, 0, 0);
    brush_set_solid(br, OrbDropDownBack);
    brush_setaa(br, 0);
    n = RoundRectangle(pt, 0, countof(pt), OuterRect, 6);
    imdrawaa_poly_brush(im, pclip, pt, &n, 1, br);
    GradientRect(im, pclip, NorthNorthRect, OrbDropDownNorthA, OrbDropDownNorthB);
    GradientRect(im, pclip, northSouthRect, OrbDropDownNorthC, OrbDropDownNorthD);
    GradientRect(im, pclip, southSouthRect, OrbDropDownSouthC, OrbDropDownSouthD);
    brush_set_solid(br, OrbDropDownDarkBorder);
    brush_setaa(br, 0);
    imdrawaa_poly_stroke_brush(im, pclip, pt, n, 1, br, 1, round_cap, round_join, inner_round);
    n = RoundRectangle(pt, 0, countof(pt), InnerRect, 6);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, OrbDropDownLightBorder, 1);
    //#region Content
    InnerRect = OrbDDContent;
    iRectInflate2(&InnerRect, 0, 0);
    OuterRect = OrbDDContent;
    iRectInflate2(&OuterRect, 1, 1);
    imdraw_rect(im, pclip, OrbDDContent, OrbDropDownContentbg, 0, 0, 0);
    imdraw_rect(im, pclip, Bcontent, OrbDropDownContentbglight, 0, 0, 0);
    imdrawaa_line(im, pclip, Bcontent.r, Bcontent.t, Bcontent.r, Bcontent.b, OrbDropDownSeparatorlight, 1);
    imdrawaa_line(im, pclip, Bcontent.r - 1, Bcontent.t, Bcontent.r - 1, Bcontent.b, OrbDropDownSeparatordark, 1);
    imdraw_rect(im, pclip, OuterRect, 0, OrbDropDownLightBorder, 1, BDM_ALL);
    imdraw_rect(im, pclip, InnerRect, 0, OrbDropDownDarkBorder, 1, BDM_ALL);
    //#region Orb
    DrawOrb(im, pclip, t->_orbBounds, t->_orbImage, t->_orbSelected, t->_orbPressed);
    return 0;
  }
  //#region Overrides
  int OnRenderRibbonTab(img_t* im, const IRECT* pclip, RibbonTab* t) {
    if (t->_active) {
      DrawCompleteTab(im, pclip, t);
    }
    else if (t->_pressed) {
      DrawTabPressed(im, pclip, t);
    }
    else if (t->_selected) {
      DrawTabSelected(im, pclip, t);
    }
    else {
      DrawTabNormal(im, pclip, t);
    }
    return 0;
  }
  int OnRenderRibbonTabText(img_t* im, const IRECT* pclip, RibbonTab* tab, IRECT _tabTextMargin, const font_t* fo) {
    const IRECT* tb = &tab->_bounds;
    IRECT r = FromLTRB(
        tb->l + _tabTextMargin.l,
        tb->t + _tabTextMargin.t,
        tb->r - _tabTextMargin.r,
        tb->b - _tabTextMargin.b);
    COLOR clr = GetTextColor(true, tab->_active ? _colorTable.TabActiveText : _colorTable.TabText);
    imdraw_text(im, pclip, r, tab->_text, -1, fo, clr, 0, DT_CENTER | DT_VCENTER);
    return 0;
  }
  int OnRenderRibbonPanelBackground(img_t* im, const IRECT* pclip, const RibbonPanel* t) {
#if 0
    if (t->OverflowMode() && !(e.Canvas is RibbonPanelPopup)) {
      if (t->_pressed) {
        DrawPanelOverflowPressed(e);
      }
      else if (t->_selected) {
        DrawPannelOveflowSelected(e);
      }
      else {
        DrawPanelOverflowNormal(e);
      }
    }
    else
#endif
    {
      if (t->_selected) {
        DrawPanelSelected(im, pclip, t);
      }
      else {
        DrawPanelNormal(im, pclip, t);
      }
    }
    return 0;
  }
  int OnRenderRibbonPanelText(img_t* im, const IRECT* pclip, const RibbonPanel* t, const font_t* fo) {
    const IRECT* b = &t->_bounds;
    const IRECT* cb = &t->_contentBounds;
#if 0
    if (t->OverflowMode() && !(e.Canvas is RibbonPanelPopup)) {
      return 0;
    }
#endif
    IRECT textArea =  FromLTRB(b->l + 1,  cb->b,  b->r - 1,  b->b - 1);
    COLOR clr = GetTextColor(t->_enabled, _colorTable.PanelText);
    imdraw_text(im, pclip, textArea, t->_text, -1, fo, clr, 0, DT_CENTER | DT_VCENTER);
    return 0;
  }
  int OnRenderRibbonItem(img_t* im, const IRECT* pclip, RibbonItem* eItem, IRECT ItemMargin, const font_t* fo) {
    int type = eItem->type;
    switch (type) {
    case RibbonItemClass_RibbonOrbOptionButton:
    case RibbonItemClass_RibbonButton: {
      //#region Button
      RibbonButton* b = (RibbonButton*)eItem;
      if (b->c->_enabled) {
        if (b->_style == RibbonButtonStyle_Normal) {
          if (b->c->_pressed && b->c->_sizeMode != RibbonElementSizeMode_DropDown) {
            DrawButtonPressed(im, pclip, b);
          }
          else if (b->c->_selected) {
            if (b->c->_checked) {
              DrawButtonPressed(im, pclip, b);
            }
            else {
              DrawButtonSelected(im, pclip, b);
            }
          }
          else if (b->c->_checked) {
            DrawButtonChecked(im, pclip, b);
          }
          else if (type == RibbonItemClass_RibbonOrbOptionButton) {
            DrawOrbOptionButton(im, pclip, b->c->_bounds);
          }
          else {
            //No background
          }
        }
        else {
          if (b->_dropDownPressed && b->c->_sizeMode != RibbonElementSizeMode_DropDown) {
            DrawButtonPressed(im, pclip, b);
            DrawSplitButtonDropDownSelected(im, pclip, b);
          }
          else if (b->c->_pressed && b->c->_sizeMode != RibbonElementSizeMode_DropDown) {
            DrawButtonPressed(im, pclip, b);
            DrawSplitButtonSelected(im, pclip, b);
          }
          else if (b->_dropDownSelected) {
            DrawButtonSelected(im, pclip, b);
            DrawSplitButtonDropDownSelected(im, pclip, b);
          }
          else if (b->c->_selected) {
            DrawButtonSelected(im, pclip, b);
            DrawSplitButtonSelected(im, pclip, b);
          }
          else if (b->c->_checked) {
            DrawButtonChecked(im, pclip, b);
          }
          else {
            DrawSplitButton(im, pclip, b);
          }
        }
      }
      if (b->_style != RibbonButtonStyle_Normal && !(b->_style == RibbonButtonStyle_DropDown && b->c->_sizeMode == RibbonElementSizeMode_Large)) {
        if (b->_style == RibbonButtonStyle_DropDown) {
          DrawButtonDropDownArrow(im, pclip, b, b->OnGetDropDownBounds(b->c->_sizeMode, b->c->_bounds, ItemMargin), fo);
        }
        else {
          DrawButtonDropDownArrow(im, pclip, b, b->_dropDownBounds, fo);
        }
      }
    }
    break;
    case RibbonItemClass_RibbonItemGroup: {
      //#region Group
      DrawItemGroup(im, pclip, (RibbonItemGroup*)eItem);
    }
    break;
    case RibbonItemClass_RibbonButtonList: {
      //#region ButtonList
      DrawButtonList(im, pclip, (RibbonButtonList*)eItem);
    }
    break;
    case RibbonItemClass_RibbonSeparator: {
      //#region Separator
      DrawSeparator(im, pclip, eItem->_bounds, eItem->_text, eItem->_sizeMode);
    }
    break;
    case RibbonItemClass_RibbonTextBox: {
      //#region TextBox
      RibbonTextBox* t = (RibbonTextBox*)eItem;
#if 0
      if (t->_enabled) {
        if (t != NULL && (t->_selected || (t->Editing))) {
          DrawTextBoxSelected(im, pclip, t->_textBoxBounds);
        }
        else {
          DrawTextBoxUnselected(im, pclip, t->_textBoxBounds);
        }
      }
      else {
        DrawTextBoxDisabled(im, pclip, t->_textBoxBounds);
      }
      if (t is RibbonComboBox) {
        DrawComboxDropDown(im, t as RibbonComboBox);
      }
#endif
    }
    break;
    }
    return 0;
  }
  int OnRenderRibbonItemBorder(img_t* im, const IRECT* pclip, RibbonItem* eItem) {
    if (eItem->type == RibbonItemClass_RibbonItemGroup) {
      DrawItemGroupBorder(im, pclip, (RibbonItemGroup*)eItem);
    }
  }
  int OnRenderRibbonItemText(img_t* im, const IRECT* pclip, IRECT rc, RibbonItem* eItem, const font_t* fo, int sf) {
    bool embedded = false;
    IRECT _bounds = rc;
    COLOR foreColor = 0;
    int type = eItem->type;
    if (type == RibbonItemClass_RibbonButton || type == RibbonItemClass_RibbonCaptionButton) {
      //#region Button
      RibbonButton* b = (RibbonButton*)eItem;
      if (type == RibbonItemClass_RibbonCaptionButton) {
        //if(WinApi.IsWindows) f = new Font("Marlett", f.ISIZE);
        embedded = true;
        foreColor = _colorTable.Arrow;
      }
      if (b->_style == RibbonButtonStyle_DropDown && b->c->_sizeMode == RibbonElementSizeMode_Large) {
        DrawButtonDropDownArrow(im, pclip, b, _bounds, fo);
      }
    }
    else if (type == RibbonItemClass_RibbonSeparator) {
      foreColor = GetTextColor(eItem->_enabled);
    }
    embedded = embedded || !eItem->_enabled;
    if (embedded) {
      IRECT cbr = eItem->_bounds;
      cbr.t++;
      imdraw_text(im, pclip, _bounds, eItem->_text, -1, fo, _colorTable.ArrowLight, 0, sf);
    }
    if (foreColor == 0) {
      foreColor = GetTextColor(eItem->_enabled);
    }
    imdraw_text(im, pclip, eItem->_bounds, eItem->_text, -1, fo, foreColor, 0, sf);
    return 0;
  }
  int OnRenderRibbonItemImage(img_t* im, const IRECT* pclip, IRECT rc, RibbonItem* eItem) {
    img_t* img = eItem->_image;
    if (eItem->type == RibbonItemClass_RibbonButton) {
      if (!(eItem->_sizeMode == RibbonElementSizeMode_Large || eItem->_sizeMode == RibbonElementSizeMode_Overflow)) {
        img = ((RibbonButton*)(eItem))->_smallImage;
      }
    }
    if (img != NULL) {
      img_t im2[1] = {0};
      if (!eItem->_enabled) {
        CreateDisabledImage(img, im2);
        img = im2;
      }
      imdraw_image(im, pclip, &eItem->_bounds, img, 0);
      imfree(im2);
    }
    return 0;
  }
  int OnRenderPanelPopupBackground(img_t* im, const IRECT* pclip, RibbonPanel* pnl) {
    if (pnl == NULL) {
      return 0;
    }
    IRECT darkBorder = FromLTRB(pnl->_bounds.l, pnl->_bounds.t, pnl->_bounds.r, pnl->_bounds.b);
    IRECT lightBorder = FromLTRB(pnl->_bounds.l + 1, pnl->_bounds.t + 1, pnl->_bounds.r - 1, pnl->_bounds.b - 1);
    IRECT textArea = FromLTRB(pnl->_bounds.l + 1, pnl->_contentBounds.b, pnl->_bounds.r - 1, pnl->_bounds.b - 1);
    DPOINT pt[100] = {0};
    DPOINT pt2[100] = {0};
    int n, n2;
    n = RoundRectangle(pt, 0, countof(pt), lightBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, _colorTable.PanelLightBorder, 1);
    n2 = RoundRectangle(pt2, 0, countof(pt), darkBorder, 3);
    imdrawaa_poly_stroke_solid(im, pclip, pt2, n2, 1, _colorTable.PanelDarkBorder, 1);
    imdrawaa_poly_solid(im, pclip, pt, &n, 1, _colorTable.PanelBackgroundSelected);
    n = RoundRectangle(pt, 0, countof(pt), textArea, 3, Corners_SouthEast | Corners_SouthWest);
    imdrawaa_poly_solid(im, pclip, pt, &n, 1, _colorTable.PanelTextBackground);
    return 0;
  }
  int OnRenderDropDownBackground(img_t* im, const IRECT* pclip, RibbonDropDown* dd) {
    int ddw = RCW(&dd->_bounds), ddh = RCH(&dd->_bounds);
    IRECT outerR = iRECT2(0, 0, ddw - 1, ddh - 1);
    IRECT imgsR = iRECT(0, 0, 26, RCH(&dd->_bounds));
    DPOINT pt[100];
    int n;
    COLOR clr;
    imdraw_rect(im, pclip, outerR, _colorTable.DropDownBg, 0, 0, 0);
    if (dd->_drawIconsBar) {
      imdraw_rect(im, pclip, imgsR, _colorTable.DropDownImageBg, 0, 0, 0);
      imdrawaa_line(im, pclip, imgsR.r, imgsR.t, imgsR.r, imgsR.b, _colorTable.DropDownImageSeparator, 1);
    }
    clr = _colorTable.DropDownBorder;
    if (dd != NULL) {
      n = RoundRectangle(pt, 0, countof(pt), iRECT2(0, 0, ddw - 1 , ddh - 1), dd->_borderRoundness);
      imdrawaa_poly_stroke_solid(im, pclip, pt, n, 1, clr, 1);
    }
    else {
      imdraw_rect(im, pclip, outerR, 0, clr, 1, 0);
    }
    if (dd->_showSizingGrip) {
      IRECT gripArea = FromLTRB(dd->_bounds.l + 1, dd->_bounds.b - dd->_sizingGripHeight, dd->_bounds.r - 1, dd->_bounds.b - 1);
      imdraw_rect_gradient(im, pclip, gripArea, _colorTable.DropDownGripNorth, _colorTable.DropDownGripSouth, 0);
      imdrawaa_line(im, pclip, gripArea.l, gripArea.t, gripArea.r - 1, gripArea.t, _colorTable.DropDownGripBorder, 1);
      DrawGripDot(im, pclip, gripArea.r - 7, gripArea.b - 3);
      DrawGripDot(im, pclip, gripArea.r - 3, gripArea.b - 7);
      DrawGripDot(im, pclip, gripArea.r - 3, gripArea.b - 3);
    }
  }
  /// Raises the <see cref="Paint"/> event
  void RibbonPopup_OnPaint(RibbonPopup* dd) {
#if 0
    //base.OnPaint(e);
    int ddw = RCW(&dd->_bounds), ddh = RCH(&dd->_bounds);
    DPOINT pt[100];
    int n;
    n = RoundRectangle(pt, 0, countof(pt), iRECT(0, 0, ddw, ddh), dd->_borderRoundness);
    {
      using(Region r = new Region(p)) {
        WrappedDropDown.Region = r;
      }
    }
#endif
  }
  void RibbonDropDown_OnPaint(img_t* im, const IRECT* pclip, RibbonDropDown* dd) {
    RibbonPopup_OnPaint(dd);
    OnRenderDropDownBackground(im, pclip, dd);
#if 0
    for (RibbonItem item in Items) {
      item.OnPaint(this, new RibbonElementPaintEventArgs(item.Bounds, e.Graphics, RibbonElementSizeMode_DropDown));
    }
#endif
    return ;
  }
  int OnRenderTabScrollButtons(img_t* im, const IRECT* pclip, RibbonTab* t) {
    if (t->_scrollLeftVisible) {
      if (t->_scrollLeftSelected) {
        DrawButtonSelected(im, pclip, t->_scrollLeftBounds, Corners_West);
      }
      else {
        DrawButton(im, pclip, t->_scrollLeftBounds, Corners_West);
      }
      DrawArrowShaded(im, pclip, t->_scrollLeftBounds, RibbonArrowDirection_Right, true);
    }
    if (t->_scrollRightVisible) {
      if (t->_scrollRightSelected) {
        DrawButtonSelected(im, pclip, t->_scrollRightBounds, Corners_East);
      }
      else {
        DrawButton(im, pclip, t->_scrollRightBounds, Corners_East);
      }
      DrawArrowShaded(im, pclip, t->_scrollRightBounds, RibbonArrowDirection_Left, true);
    }
    return 0;
  }
  /// Renders text of the button
  void RibbonButton_OnPaintText(img_t* im, const IRECT* pclip, RibbonButton* b, int _sizeMode, const font_t* fo) {
    if (_sizeMode != RibbonElementSizeMode_Compact) {
#if 0
      if (_sizeMode == RibbonElementSizeMode_Large) {
        sf.Alignment = StringAlignment.Center;
        if (!string.IsNullOrEmpty(Text) && !Text.Contains(" ")) {
          sf.LineAlignment = StringAlignment.Near;
        }
      }
#endif
      OnRenderRibbonItemText(im, pclip, b->_textBounds, b->c, fo, DT_CENTER | DT_VCENTER);
    }
  }
  /// Renders the image of the button
  void RibbonButton_OnPaintImage(img_t* im, const IRECT* pclip, RibbonButton* b, int _sizeMode, IRECT ItemMargin) {
    RibbonElementSizeMode theSize = b->c->GetNearestSize(_sizeMode);
    if ((theSize == RibbonElementSizeMode_Large && b->c->_image != NULL) || b->_smallImage != NULL) {
      OnRenderRibbonItemImage(im, pclip, b->OnGetImageBounds(theSize, ItemMargin), b->c);
    }
  }
  /// Renders the background of the buton
  void RibbonButton_OnPaintBackground(img_t* im, const IRECT* pclip, RibbonButton* b, IRECT ItemMargin, const font_t* fo) {
    OnRenderRibbonItem(im, pclip, b->c, ItemMargin, fo);
  }
  /// Raises the Paint event.
  /// <param name="sender"></param>
  void RibbonButton_OnPaint(img_t* im, const IRECT* pclip, RibbonButton* b, IRECT ItemMargin, int _sizeMode, const font_t* fo) {
    RibbonButton_OnPaintBackground(im, pclip, b, ItemMargin, fo);
    RibbonButton_OnPaintImage(im, pclip, b, _sizeMode, ItemMargin);
    RibbonButton_OnPaintText(im, pclip, b, _sizeMode, fo);
  }
  void RibbonQuickAccessToolbar_OnPaint(img_t* im, const IRECT* pclip, RibbonQuickAccessToolbar* t, bool orbVisible, int actualBorderMode) {
    OnRenderRibbonQuickAccessToolbarBackground(im, pclip, t, orbVisible, actualBorderMode);
#if 0
    foreach(RibbonItem item in Items) {
      item->OnPaint(this, new RibbonElementPaintEventArgs(item->Bounds, e.Graphics, RibbonElementSizeMode_Compact));
    }
#endif
  }
  /// Raises the paint event and draws the
  /// <param name="sender"></param>
  int RibbonPanel_OnPaint(img_t* im, const IRECT* pclip, RibbonPanel* p, int sizeMode, const font_t* fo) {
    //if (Paint != NULL)      {        Paint(this, new PaintEventArgs(e.Graphics, e.Clip));      }
    if (p->_popupShowed) {
      //Draw a fake collapsed and pressed panel
      //#region Create fake panel
      RibbonPanel fakePanel[1] = {0};
      fakePanel->_text = p->_text;
      fakePanel->_image = p->_image;
      fakePanel->SetSizeMode(RibbonElementSizeMode_Overflow);
      fakePanel->SetBounds(p->_overflowBoundsBuffer);
      fakePanel->SetPressed(true);
      OnRenderRibbonPanelBackground(im, pclip, fakePanel);
      OnRenderRibbonPanelText(im, pclip, fakePanel, fo);
    }
    else {
      //Draw normal
      OnRenderRibbonPanelBackground(im, pclip, p);
      OnRenderRibbonPanelText(im, pclip, p, fo);
    }
#if 0
    if (sizeMode != RibbonElementSizeMode_Overflow) {
      for (RibbonItem item in Items) {
        item->OnPaint(this, new RibbonElementPaintEventArgs(item->_bounds, e.Graphics, item->_sizeMode));
      }
    }
#endif
    return 0;
  }
  void RibbonTab_OnPaint(img_t* im, const IRECT* pclip, RibbonTab* tab, IRECT _tabTextMargin, const font_t* fo) {
    int i;
    OnRenderRibbonTab(im, pclip, tab);
    OnRenderRibbonTabText(im, pclip, tab, _tabTextMargin, fo);
    if (tab->_active) {
      for (i = 0; i < tab->_panels_count; ++i) {
        RibbonPanel* panel = tab->_panels + i;
        RibbonPanel_OnPaint(im, pclip, panel, panel->_sizeMode, fo);
      }
    }
    OnRenderTabScrollButtons(im, pclip, tab);
  }
  /// Paints the Ribbon on the specified device
  /// <param name="g">Device where to paint on</param>
  /// <param name="clip">Clip rectangle</param>
  void PaintOn(img_t* im, const IRECT* pclip, IRECT rc, Ribbon* t) {
    int i;
    //Caption Background
    imdraw_rect(im, pclip, rc, _colorTable.RibbonBackground, 0, 0, 0);
    //Caption Bar
    //OnRenderRibbonCaptionBar(im, pclip, t);
    //int OnRenderRibbonCaptionBar(img_t* im, const IRECT* pclip, IRECT rc, Ribbon* t)
    {
      IRECT captionBar = iRECT(0, 0, RCW(&rc), t->_captionBarHeight);
      if (!(t->_actualBorderMode == RibbonWindowMode_NonClientAreaGlass// && RibbonDesigner.Current == NULL
           )) {
        DrawCaptionBarBackground(im, pclip, captionBar);
      }
      DrawCaptionBarText(im, pclip, captionBar, t->_text, t->fo);
      //return 0;
    }
    //Caption Buttons
    if (t->_CaptionButtonsVisible) {
      RibbonButton_OnPaint(im, pclip, t->_MinimizeButton, t->_itemMargin, RibbonElementSizeMode_Medium, t->fo);
      RibbonButton_OnPaint(im, pclip, t->_MaximizeRestoreButton, t->_itemMargin, RibbonElementSizeMode_Medium, t->fo);
      RibbonButton_OnPaint(im, pclip, t->_CloseButton, t->_itemMargin, RibbonElementSizeMode_Medium, t->fo);
    }
    //Orb
    OnRenderRibbonOrb(im, pclip, t);
    //QuickAcess toolbar
    RibbonQuickAccessToolbar_OnPaint(im, pclip, t->_quickAcessToolbar, t->_orbVisible, t->_actualBorderMode);
    //Render Tabs
    for (i = 0; i < t->_tabs_count; ++i) {
      RibbonTab* tab = t->_tabs + i;
      //tab->OnPaint(this, new RibbonElementPaintEventArgs(tab->_bounds, g, RibbonElementSizeMode_None, this));
    }
  }
};

