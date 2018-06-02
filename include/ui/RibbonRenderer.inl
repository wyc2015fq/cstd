// Types
enum Corners {
  None = 0,
  NorthWest = 2,
  NorthEast = 4,
  SouthEast = 8,
  SouthWest = 16,
  All = NorthWest | NorthEast | SouthEast | SouthWest,
  North = NorthWest | NorthEast,
  South = SouthEast | SouthWest,
  East = NorthEast | SouthEast,
  West = NorthWest | SouthWest
};
enum RibbonWindowMode {
  InsideWindow,
  NonClientAreaCustomDrawn,
  NonClientAreaGlass
};
enum RibbonButtonStyle {
  /// Simple clickable button
  RibbonButtonStyle_Normal,
  /// Button with a right side drop down
  RibbonButtonStyle_DropDown,
  /// Button with an optional dropdown attachment on the right
  RibbonButtonStyle_SplitDropDown,
};
/// <summary>
/// Represents the size modes that a RibbonElement can be
/// </summary>
enum RibbonElementSizeMode {
  /// The item is being shown on a dropdown
  RibbonElementSizeMode_DropDown = 5,
  /// Maximum size the element can reach
  RibbonElementSizeMode_Large = 4,
  /// A medium size for the element when not much space is available
  RibbonElementSizeMode_Medium = 3,
  /// The minimum size the element can be
  RibbonElementSizeMode_Compact = 2,
  /// The item doesn't fit as compact, so it must be shown as a single button
  RibbonElementSizeMode_Overflow = 1,
  /// No size mode specified
  RibbonElementSizeMode_None = 0,
};
/// Represents the directions that arrows can have
enum RibbonArrowDirection {
  /// The arrow points up
  RibbonArrowDirection_Up,
  /// The arrow points down
  RibbonArrowDirection_Down,
  /// The arrow points right
  RibbonArrowDirection_Right,
  /// The arrow points left
  RibbonArrowDirection_Left
};
// Fields
static ISIZE arrowSize = {5, 3};
static ISIZE moreSize = {7, 7};
enum {
#define RIBBONCOLORTABLEDEF(a, b) ColorTable_##a = b,
#include "RibbonColorTable.txt"
#undef RIBBONCOLORTABLEDEF
};
COLOR GetTextColor2(BOOL enabled, COLOR alternative)
{
  if (enabled) {
    return alternative;
  }
  else {
    return ColorTable_ArrowDisabled;
  }
}
COLOR GetTextColor1(BOOL enabled)
{
  return GetTextColor2(enabled, ColorTable_Text);
}
#define ADDLINE(x1, y1, x2, y2) (pt[pos++]=dPOINT(x1, y1), pt[pos++]=dPOINT(x2, y2))
#define ADDARC(rc, a1, a2) pos+=vcgen_arc_FromIRECT(pt+pos, 8, 1, rc, a1, a2)
/// <summary>
/// Creates a rectangle with the specified corners rounded
/// </summary>
/// <param name="r"></param>
/// <param name="radius"></param>
/// <param name="corners"></param>
/// <returns></returns>
static int RoundRectangle(DPOINT* pt, IRECT r, int radius, int corners)
{
  int d = radius * 2;
  int nw = (corners & NorthWest) == NorthWest ? d : 0;
  int ne = (corners & NorthEast) == NorthEast ? d : 0;
  int se = (corners & SouthEast) == SouthEast ? d : 0;
  int sw = (corners & SouthWest) == SouthWest ? d : 0;
  int pos = 0;
  ADDLINE(r.l + nw, r.t, r.r - ne, r.t);
  if (ne > 0) {
    ADDARC(iRECT(r.r - ne, r.t, r.r, r.t + ne), -90, 90);
  }
  ADDLINE(r.r, r.t + ne, r.r, r.b - se);
  if (se > 0) {
    ADDARC(iRECT(r.r - se, r.b - se, r.r, r.b), 0, 90);
  }
  ADDLINE(r.r - se, r.b, r.l + sw, r.b);
  if (sw > 0) {
    ADDARC(iRECT(r.l, r.b - sw, r.l + sw, r.b), 90, 90);
  }
  ADDLINE(r.l, r.b - sw, r.l, r.t + nw);
  if (nw > 0) {
    ADDARC(iRECT(r.l, r.t, r.l + nw, r.t + nw), 180, 90);
  }
  return pos;
}
CC_INLINE int imdrawaa_roundrect(img_t* im, const IRECT* pclip, IRECT rc, int radius, int corners, COLOR crFill, COLOR crLine, double wline)
{
  DPOINT pt[100];
  int n = RoundRectangle(pt, rc, radius, corners);
  imdrawaa_poly(im, pclip, pt, n, AlignmentInset, crFill, crLine, wline);
  return 0;
}
CC_INLINE int imdrawaa_roundrect_gradient_hv(img_t* im, const IRECT* pclip, IRECT rc, int radius, int corners, int isv, COLOR clr1, COLOR clr2)
{
  xbrush_t br[1] = {0};
  DPOINT pt[100];
  int n = RoundRectangle(pt, rc, radius, corners);
  xbrush_set_gradient(br, rc.l, rc.t, isv ? rc.l : rc.r, isv ? rc.b : rc.t, clr1, clr2);
  imdrawaa_poly_brush(im, pclip, pt, &n, 1, br);
  return 0;
}
CC_INLINE BOOL imdrawaa_roundrect_path_gradient(img_t* im, const IRECT* pclip, IRECT rc, int radius, int corners, const COLOR* clrs, const double* pos, int nclr, const DPOINT* pcenter)
{
  COLOR lut[256];
  DPOINT pt[100];
  int n = RoundRectangle(pt, rc, radius, corners);
  fill_color_array2(256, lut, nclr, clrs, pos);
  return imdrawaa_poly_path_gradient_lut(im, pclip, pt, n, lut, pcenter);
}
/// <summary>
/// Creates a rectangle with rounded corners
/// </summary>
/// <param name="r"></param>
/// <param name="radius"></param>
/// <returns></returns>
static int RoundRectangle2(DPOINT* pt, IRECT r, int radius)
{
  return RoundRectangle(pt, r, radius, All);
}
/// <summary>
/// Draws a rectangle with a vertical gradient
/// </summary>
/// <param name="g"></param>
/// <param name="r"></param>
/// <param name="northColor"></param>
/// <param name="southColor"></param>
int GradientRect(img_t* im, IRECT r, COLOR northColor, COLOR southColor)
{
  imdraw_rect_gradient(im, &r, r, northColor, southColor, 0);
  return 0;
}
/// <summary>
/// Draws a shadow that indicates that the element is pressed
/// </summary>
/// <param name="g"></param>
/// <param name="r"></param>
int DrawPressedShadow(img_t* im, IRECT r)
{
  imdrawaa_roundrect_gradient_hv(im, 0, iRECT(r.l, r.t, r.r, r.t + 4), 3, NorthEast | NorthWest, 1, _ARGB(50, Black), _ARGB(0, Black));
  return 0;
}
/// <summary>
/// Draws an arrow on the specified bounds
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
/// <param name="c"></param>
int DrawArrow(img_t* im, IRECT b, COLOR c, int d)
{
  DPOINT pt[100];
  IRECT bounds = b;
  int pos = 0;
  if (RCW(&b) % 2 != 0 && (d == RibbonArrowDirection_Up)) {
    bounds = iRECT2(b.l - 1, b.t - 1, RCW(&b) + 1, RCH(&b) + 1);
  }
  if (d == RibbonArrowDirection_Up) {
    ADDLINE(bounds.l, bounds.b, bounds.r, bounds.b);
    ADDLINE(bounds.r, bounds.b, bounds.l + RCW(&bounds) / 2, bounds.t);
  }
  else if (d == RibbonArrowDirection_Down) {
    ADDLINE(bounds.l, bounds.t, bounds.r , bounds.t);
    ADDLINE(bounds.r, bounds.t, bounds.l + RCW(&bounds) / 2, bounds.b);
  }
  else if (d == RibbonArrowDirection_Left) {
    ADDLINE(bounds.l, bounds.t, bounds.r, bounds.t + RCH(&bounds) / 2);
    ADDLINE(bounds.r, bounds.t + RCH(&bounds) / 2, bounds.l, bounds.b);
  }
  else {
    ADDLINE(bounds.r, bounds.t, bounds.l, bounds.t + RCH(&bounds) / 2);
    ADDLINE(bounds.l, bounds.t + RCH(&bounds) / 2, bounds.r, bounds.b);
  }
  imdrawaa_poly(im, &bounds, pt, pos, 1, c, 0, 0);
  return 0;
}
/// <summary>
/// Draws the pair of arrows that make a shadded arrow, centered on the specified bounds
/// </summary>
/// <param name="g"></param>
/// <param name="b"></param>
/// <param name="d"></param>
/// <param name="enabled"></param>
int DrawArrowShaded(img_t* im, IRECT b, int d, BOOL enabled)
{
  ISIZE arrSize = arrowSize;
  IRECT bounds, boundsLight;
  COLOR lt = ColorTable_ArrowLight;
  COLOR dk = ColorTable_Arrow;
  if (d == RibbonArrowDirection_Left || d == RibbonArrowDirection_Right) {
    //Invert size
    arrSize = iSIZE(arrowSize.h, arrowSize.w);
  }
  bounds = iRECT(b.l + (RCW(&b) - arrSize.w) / 2, b.t + (RCH(&b) - arrSize.h) / 2, arrSize.w, arrSize.h);
  boundsLight = iRectOff2(bounds, 0, 1);
  if (!enabled) {
    lt = Transparent;
    dk = ColorTable_ArrowDisabled;
  }
  DrawArrow(im, boundsLight, lt, d);
  DrawArrow(im, bounds, dk, d);
  return 0;
}
/// <summary>
/// Centers the specified rectangle on the specified container
/// </summary>
/// <param name="container"></param>
/// <param name="r"></param>
/// <returns></returns>
IRECT CenterOn(IRECT container, IRECT r)
{
  IRECT result = iRECT2(container.l + ((RCW(&container) - RCW(&r)) / 2),
      container.t + ((RCH(&container) - RCH(&r)) / 2),
      RCW(&r), RCH(&r));
  return result;
}
/// <summary>
/// Draws a dot of the grip
/// </summary>
/// <param name="g"></param>
/// <param name="location"></param>
int DrawGripDot(img_t* im, IPOINT location)
{
  IRECT lt = iRECT2(location.x - 1, location.y + 1, 2, 2);
  IRECT dk = iRECT2(location.x, location.y, 2, 2);
  imdraw_rect(im, 0, lt, ColorTable_DropDownGripLight, 0, 0, 0);
  imdraw_rect(im, 0, dk, ColorTable_DropDownGripDark, 0, 0, 0);
  return 0;
}
// Tab
/// <summary>
/// Creates the path of the tab and its contents
/// </summary>
/// <param name="tab"></param>
/// <returns></returns>
int CreateCompleteTabPath(DPOINT* pt, const ribbon_tab_t* t)
{
  int corner = 6;
  int pos = 0;
  ADDLINE(t->TabBounds.l + corner, t->TabBounds.t, t->TabBounds.r - corner, t->TabBounds.t);
  ADDARC(iRECT(t->TabBounds.r - corner, t->TabBounds.t, t->TabBounds.r, t->TabBounds.t + corner), -90, 90);
  ADDLINE(t->TabBounds.r, t->TabBounds.t + corner, t->TabBounds.r, t->TabBounds.b - corner);
  ADDARC(iRECT(t->TabBounds.r, t->TabBounds.b - corner, t->TabBounds.r + corner, t->TabBounds.b), -180, -90);
  ADDLINE(t->TabBounds.r + corner, t->TabBounds.b, t->TabContentBounds.r - corner, t->TabBounds.b);
  ADDARC(iRECT(t->TabContentBounds.r - corner, t->TabBounds.b, t->TabContentBounds.r, t->TabBounds.b + corner), -90, 90);
  ADDLINE(t->TabContentBounds.r, t->TabContentBounds.t + corner, t->TabContentBounds.r, t->TabContentBounds.b - corner);
  ADDARC(iRECT(t->TabContentBounds.r - corner, t->TabContentBounds.b - corner, t->TabContentBounds.r, t->TabContentBounds.b), 0, 90);
  ADDLINE(t->TabContentBounds.r - corner, t->TabContentBounds.b, t->TabContentBounds.l + corner, t->TabContentBounds.b);
  ADDARC(iRECT(t->TabContentBounds.l, t->TabContentBounds.b - corner, t->TabContentBounds.l + corner, t->TabContentBounds.b), 90, 90);
  ADDLINE(t->TabContentBounds.l, t->TabContentBounds.b - corner, t->TabContentBounds.l, t->TabBounds.b + corner);
  ADDARC(iRECT(t->TabContentBounds.l, t->TabBounds.b, t->TabContentBounds.l + corner, t->TabBounds.b + corner), 180, 90);
  ADDLINE(t->TabContentBounds.l + corner, t->TabContentBounds.t, t->TabBounds.l - corner, t->TabBounds.b);
  ADDARC(iRECT(t->TabBounds.l - corner, t->TabBounds.b - corner, t->TabBounds.l, t->TabBounds.b), 90, -90);
  ADDLINE(t->TabBounds.l, t->TabBounds.b - corner, t->TabBounds.l, t->TabBounds.t + corner);
  ADDARC(iRECT(t->TabBounds.l, t->TabBounds.t, t->TabBounds.l + corner, t->TabBounds.t + corner), 180, 90);
  return pos;
}
/// <summary>
/// Creates the path of the tab and its contents
/// </summary>
/// <param name="tab"></param>
/// <returns></returns>
int CreateTabPath(DPOINT* pt, const RibbonTab* t)
{
  int corner = 6;
  int rightOffset = 1;
  int pos = 0;
  ADDLINE(t->TabBounds.l, t->TabBounds.b, t->TabBounds.l, t->TabBounds.t + corner);
  ADDARC(iRECT(t->TabBounds.l, t->TabBounds.t, corner, corner), 180, 90);
  ADDLINE(t->TabBounds.l + corner, t->TabBounds.t, t->TabBounds.r - corner - rightOffset, t->TabBounds.t);
  ADDARC(iRECT(t->TabBounds.r - corner - rightOffset, t->TabBounds.t, corner, corner), -90, 90);
  ADDLINE(t->TabBounds.r - rightOffset, t->TabBounds.t + corner, t->TabBounds.r - rightOffset, t->TabBounds.b);
  return pos;
}
/// <summary>
/// Draws a complete tab
/// </summary>
/// <param name="e"></param>
int DrawTabNormal(img_t* im, const RibbonTab* t)
{
  IRECT clip = iRECT(t->TabBounds.l, t->TabBounds.t, t->TabBounds.r, t->TabBounds.b);
  IRECT r = iRECT(t->TabBounds.l - 1, t->TabBounds.t - 1, t->TabBounds.r, t->TabBounds.b);
  imdraw_rect(im, &clip, r, ColorTable_RibbonBackground, 0, 0, 0);
  return 0;
}
/// <summary>
/// Draws an active tab
/// </summary>
/// <param name="e"></param>
int DrawTabActive(img_t* im, const RibbonTab* t)
{
  int xx = DrawTabNormal(im, t);
  IRECT glossy = iRECT2(t->TabBounds.l, t->TabBounds.t, RCW(&t->TabBounds), 4);
  IRECT shadow = iRectOff2(t->TabBounds, 2, 1);
  IRECT tab = t->TabBounds; //tab.Inflate(0, 1);
  COLOR clrs[] = {Transparent, _ARGB(50, Black), _ARGB(100, Black)};
  double pos[] = { 0, .1, 1 };
  imdrawaa_roundrect_path_gradient(im, 0, shadow, 6, NorthWest | NorthEast, clrs, pos, 3, 0);
  imdrawaa_roundrect_gradient_hv(im, 0, tab, 6, North, 1, ColorTable_TabNorth, ColorTable_TabSouth);
  imdrawaa_roundrect(im, 0, tab, 6, North, 0, ColorTable_TabNorth, 1.6);
  imdrawaa_roundrect(im, 0, glossy, 6, North, _ARGB(180, White), 0, 0);
  return 0;
}
/// <summary>
/// Draws a complete tab
/// </summary>
/// <param name="e"></param>
int DrawCompleteTab(img_t* im, const RibbonTab* t)
{
  int xx = DrawTabActive(im, t);
  IRECT glossy = iRECT(t->TabContentBounds.l, t->TabContentBounds.t + 0, t->TabContentBounds.r, t->TabContentBounds.t + 18);
  DPOINT pt[100];
  int n;
  //Background gradient
  imdrawaa_roundrect_gradient_hv(im, 0, t->TabContentBounds, 4, All, 1, ColorTable_TabContentNorth, ColorTable_TabContentSouth);
  //Glossy effect
  imdrawaa_roundrect(im, &glossy, glossy, 6, NorthWest | NorthEast, _ARGB(30, White), 0, 0);
  //Tab border
  n = CreateCompleteTabPath(pt, t);
  imdrawaa_poly(im, 0, pt, n, 1, 0, ColorTable_TabBorder, 1);
  if (t->Selected) {
    //Selected glow
    n = CreateTabPath(pt, t);
    imdrawaa_poly(im, 0, pt, n, 1, 0, _ARGB(150, Gold), 2);
  }
  return 0;
}
/// <summary>
/// Draws a selected tab
/// </summary>
/// <param name="e"></param>
int DrawTabSelected(img_t* im, const RibbonTab* t)
{
  IRECT outerR = iRECT(t->TabBounds.l, t->TabBounds.t, t->TabBounds.r - 1, t->TabBounds.b);
  IRECT innerR = iRECT(outerR.l + 1, outerR.t + 1, outerR.r - 1, outerR.b);
  IRECT glossyR = iRECT(innerR.l + 1, innerR.t + 1, innerR.r - 1, innerR.t + RCH(&t->TabBounds) / 2);
  imdrawaa_roundrect(im, 0, outerR, 3, NorthEast | NorthWest, 0, ColorTable_TabBorder, 1);
  imdrawaa_roundrect(im, 0, innerR, 3, NorthEast | NorthWest, 0, _ARGB(200, White), 1);
  {
    DPOINT Center = dPOINT((innerR.l + innerR.r) * .5, innerR.t - 5);
    double Factors[] = { 0.0f, 0.9f, 0.0f };
    double Positions[] = { 0.0f, 0.8f, 1.0f };
    COLOR lut[256];
    fill_color_array3(256, lut, Transparent, ColorTable_TabSelectedGlow, 3, Positions, Factors);
    imdrawaa_rect_path_gradient_lut(im, &innerR, innerR.l, innerR.t, innerR.r, innerR.b, lut, &Center);
  }
  imdrawaa_roundrect(im, 0, glossyR, 3, NorthEast | NorthWest, _ARGB(100, White), 0, 0);
  return 0;
}
/// <summary>
/// Draws a pressed tab
/// </summary>
/// <param name="e"></param>
int DrawTabPressed(const RibbonTab* t)
{
  return 0;
}
//a-------b-+
//| |
//| d
//c g |
//| |
//+----e----f
int DrawButtonMoreGlyph1(img_t* im, IPOINT p, COLOR color)
{
  IPOINT a = p;
  IPOINT b = iPOINT(p.x + moreSize.w - 1, p.y);
  IPOINT c = iPOINT(p.x, p.y + moreSize.h - 1);
  IPOINT f = iPOINT(p.x + moreSize.w, p.y + moreSize.h);
  IPOINT d = iPOINT(f.x, f.y - 3);
  IPOINT e = iPOINT(f.x - 3, f.y);
  IPOINT g = iPOINT(f.x - 3, f.y - 3);
  imdraw_line(im, a, b, color, 1);
  imdraw_line(im, a, c, color, 1);
  imdraw_line(im, e, f, color, 1);
  imdraw_line(im, d, f, color, 1);
  imdraw_line(im, e, d, color, 1);
  imdraw_line(im, g, f, color, 1);
  return 0;
}
int DrawButtonMoreGlyph(img_t* im, IRECT b, BOOL enabled)
{
  COLOR dark = enabled ? ColorTable_Arrow : ColorTable_ArrowDisabled;
  COLOR light = ColorTable_ArrowLight;
  IRECT bounds = CenterOn(b, iRECT2(0, 0, moreSize.w, moreSize.h));
  IRECT boundsLight = iRectOff2(bounds, 1, 1);
  DrawButtonMoreGlyph1(im, RCLT(&boundsLight), light);
  DrawButtonMoreGlyph1(im, RCLT(&bounds), dark);
  return 0;
}
// Panel
/// <summary>
/// Draws a panel in normal state (unselected)
/// </summary>
/// <param name="e"></param>
int DrawPanelNormal(img_t* im, RibbonPanel* Panel)
{
  IRECT darkBorder = iRECT(Panel->Bounds.l, Panel->Bounds.t, Panel->Bounds.r, Panel->Bounds.b);
  IRECT lightBorder = iRECT(Panel->Bounds.l + 1, Panel->Bounds.t + 1, Panel->Bounds.r + 1, Panel->Bounds.b);
  IRECT textArea = iRECT(Panel->Bounds.l + 1, Panel->ContentBounds.b, Panel->Bounds.r - 1, Panel->Bounds.b - 1);
  imdrawaa_roundrect(im, 0, lightBorder, 3, All, 0, ColorTable_PanelLightBorder, 1);
  imdrawaa_roundrect(im, 0, darkBorder, 3, All, 0, ColorTable_PanelDarkBorder, 1);
  imdrawaa_roundrect(im, 0, textArea, 3, SouthEast | SouthWest, ColorTable_PanelTextBackground, 0, 0);
  if (Panel->ButtonMoreVisible) {
    DrawButtonMoreGlyph(im, Panel->ButtonMoreBounds, Panel->ButtonMoreEnabled && Panel->Enabled);
  }
  return 0;
}
/// <summary>
/// Draws a regular button in pressed state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawButtonPressed(img_t* im, IRECT bounds, int corners)
{
  IRECT outerR = iRECT(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
  IRECT innerR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
  IRECT glossyR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + (int)(RCH(&bounds) * .36));
  double Factors[] = { 0, 0.8, 0 };
  double Positions[] = { 0, 0.30, 1 };
  COLOR lut[256];
  imdrawaa_roundrect(im, &bounds, outerR, 3, corners, ColorTable_ButtonPressedBgOut, 0, 0);
  fill_color_array3(256, lut, ColorTable_ButtonPressedBgCenter, ColorTable_ButtonPressedBgOut, 3, Positions, Factors);
  imdrawaa_ellipse_lut_radial2(im, &bounds, RCX(&bounds)*.5, bounds.b, RCW(&bounds)*.5, RCH(&bounds), lut);
  imdrawaa_roundrect(im, 0, outerR, 3, corners, 0, ColorTable_ButtonPressedBorderOut, 1);
  imdrawaa_roundrect(im, 0, innerR, 3, corners, 0, ColorTable_ButtonPressedBorderIn, 1);
  imdrawaa_roundrect_gradient_hv(im, 0, glossyR, 3, (corners & NorthWest) | (corners & NorthEast), 1, ColorTable_ButtonPressedGlossyNorth, ColorTable_ButtonPressedGlossySouth);
  DrawPressedShadow(im, outerR);
  return 0;
}
/// <summary>
/// Draws a regular buttton in selected state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawButtonSelected(img_t* im, IRECT bounds, int corners)
{
  IRECT outerR = iRECT(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
  IRECT innerR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
  IRECT glossyR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + (int)(RCH(&bounds) * .36));
  double Factors[] = { 0, 0.8, 0 };
  double Positions[] = { 0, 0.30, 1 };
  COLOR lut[256];
  imdrawaa_roundrect(im, 0, outerR, 3, corners, ColorTable_ButtonSelectedBgOut, ColorTable_ButtonSelectedBorderOut, 1);
  fill_color_array3(256, lut, ColorTable_ButtonSelectedBgCenter, ColorTable_ButtonSelectedBgOut, 3, Positions, Factors);
  imdrawaa_ellipse_lut_radial2(im, &bounds, RCX(&bounds)*.5, bounds.b, RCW(&bounds)*.5, RCH(&bounds), lut);
  imdrawaa_roundrect(im, 0, innerR, 3, corners, 0, ColorTable_ButtonSelectedBorderIn, 1);
  imdrawaa_roundrect_gradient_hv(im, 0, glossyR, 3, (corners & NorthWest) | (corners & NorthEast), 1, ColorTable_ButtonSelectedGlossyNorth, ColorTable_ButtonSelectedGlossySouth);
  return 0;
}
/// <summary>
/// Draws a panel in selected state
/// </summary>
/// <param name="e"></param>
int DrawPanelSelected(img_t* im, RibbonPanel* Panel)
{
  IRECT darkBorder = iRECT(Panel->Bounds.l, Panel->Bounds.t, Panel->Bounds.r, Panel->Bounds.b);
  IRECT lightBorder = iRECT(Panel->Bounds.l + 1, Panel->Bounds.t + 1, Panel->Bounds.r - 1, Panel->Bounds.b - 1);
  IRECT textArea = iRECT(Panel->Bounds.l + 1, Panel->ContentBounds.b, Panel->Bounds.r - 1, Panel->Bounds.b - 1);
  imdrawaa_roundrect(im, 0, lightBorder, 3, All, ColorTable_PanelBackgroundSelected, ColorTable_PanelLightBorder, 1);
  imdrawaa_roundrect(im, 0, darkBorder, 3, All, 0, ColorTable_PanelDarkBorder, 1);
  imdrawaa_roundrect(im, 0, textArea, 3, SouthEast | SouthWest, ColorTable_PanelTextBackgroundSelected, 0, 0);
  if (Panel->ButtonMoreVisible) {
    if (Panel->ButtonMorePressed) {
      DrawButtonPressed(im, Panel->ButtonMoreBounds, SouthEast);
    }
    else if (Panel->ButtonMoreSelected) {
      DrawButtonSelected(im, Panel->ButtonMoreBounds, SouthEast);
    }
    DrawButtonMoreGlyph(im, Panel->ButtonMoreBounds, Panel->ButtonMoreEnabled && Panel->Enabled);
  }
  return 0;
}
IRECT LargeButtonDropDownArrowBounds(const str_t* text, IRECT textLayout)
{
#if 1
  ISIZE textS = font_text_size(g_scr->font, text->s, text->l, 0);
  IRECT lastCharBounds = iRECT2(textLayout.l, textLayout.t, textS.w, textS.h);
  return iRECT(
      textLayout.l + (RCW(&textLayout) - arrowSize.w) / 2,
      lastCharBounds.b + ((textLayout.b - lastCharBounds.b) - arrowSize.h) / 2, arrowSize.w, arrowSize.h);
#else
  IRECT bounds = {0};
  BOOL moreWords = str_find_chr(text, ' ');
  sf.Alignment = StringAlignment.Center;
  sf.LineAlignment = moreWords ? StringAlignment.Center : StringAlignment.Near;
  sf.Trimming = StringTrimming.EllipsisCharacter;
  sf.SetMeasurableCharacterRanges(new CharacterRange[] { new CharacterRange(0, text.Length) });
  Region[] regions = g.MeasureCharacterRanges(text, font, textLayout, sf);
  IRECT lastCharBounds = IRECT.Round(regions[regions.Length - 1].GetBounds(g));
  if (moreWords) {
    return iRECT(lastCharBounds.r + 3,
        lastCharBounds.t + (lastCharBounds.Height - arrowSize.h) / 2, arrowSize.w, arrowSize.h);
  }
  else {
    return iRECT(
        textLayout.l + (textLayout.Width - arrowSize.w) / 2,
        lastCharBounds.b + ((textLayout.b - lastCharBounds.b) - arrowSize.h) / 2, arrowSize.w, arrowSize.h);
  }
#endif
}
/// <summary>
/// Draws the image of the panel when collapsed
/// </summary>
/// <param name="e"></param>
int DrawPanelOverflowImage(img_t* im, RibbonPanel* Panel)
{
  int margin = 3;
  IRECT bounds;
  ISIZE imgSquareSize = iSIZE(32, 32);
  IRECT imgSquareR = iRECT2(Panel->Bounds.l + (RCW(&Panel->Bounds) - imgSquareSize.w) / 2, Panel->Bounds.t + 5, imgSquareSize.w, imgSquareSize.h);
  IRECT imgSquareBottomR = iRECT(imgSquareR.l, imgSquareR.b - 10, imgSquareR.r, imgSquareR.b);
  IRECT textR = iRECT(Panel->Bounds.l + margin, imgSquareR.b + margin, Panel->Bounds.r - margin, Panel->Bounds.b - margin);
  imdrawaa_roundrect_gradient_hv(im, 0, imgSquareR, 5, All, 1, ColorTable_TabContentNorth, ColorTable_TabContentSouth);
  imdrawaa_roundrect(im, 0, imgSquareBottomR, 5, South, ColorTable_PanelTextBackground, 0, 0);
  imdrawaa_roundrect(im, 0, imgSquareR, 5, All, 0, ColorTable_PanelDarkBorder, 1);
  if (Panel->Image != NULL) {
    int x = imgSquareR.l + (RCW(&imgSquareR) - Panel->Image->w) / 2;
    int y = imgSquareR.t + ((RCH(&imgSquareR) - RCH(&imgSquareBottomR)) - Panel->Image->h) / 2;
    imdraw_image2(im, Panel->Image, x, y, Panel->Image->w, Panel->Image->h);
  }
  imdraw_text(im, &textR, textR, Panel->Text->s, Panel->Text->l, g_scr->font, GetTextColor1(Panel->Enabled), 0, TT_VCENTER | TT_CENTER);
  bounds = LargeButtonDropDownArrowBounds(Panel->Text, textR);
  if (bounds.r < Panel->Bounds.r) {
    IRECT boundsLight = iRectOff2(bounds, 0, 1);
    DrawArrow(im, boundsLight, ColorTable_ArrowLight, RibbonArrowDirection_Down);
    DrawArrow(im, bounds, ColorTable_Arrow, RibbonArrowDirection_Down);
  }
  return 0;
}
/// <summary>
/// Draws an overflown panel in normal state
/// </summary>
/// <param name="e"></param>
int DrawPanelOverflowNormal(img_t* im, RibbonPanel* Panel)
{
  IRECT darkBorder = iRECT(Panel->Bounds.l, Panel->Bounds.t, Panel->Bounds.r, Panel->Bounds.b);
  IRECT lightBorder = iRECT(Panel->Bounds.l + 1, Panel->Bounds.t + 1, Panel->Bounds.r - 1, Panel->Bounds.b - 1);
  DPOINT pt[100];
  int n;
  n = RoundRectangle2(pt, lightBorder, 3);
  imdrawaa_poly(im, 0, pt, n, 1, 0, ColorTable_PanelLightBorder, 1);
  n = RoundRectangle2(pt, darkBorder, 3);
  imdrawaa_poly(im, 0, pt, n, 1, 0, ColorTable_PanelDarkBorder, 1);
  DrawPanelOverflowImage(im, Panel);
  return 0;
}
/// <summary>
/// Draws an overflown panel in selected state
/// </summary>
/// <param name="e"></param>
int DrawPannelOveflowSelected(img_t* im, RibbonPanel* Panel)
{
  IRECT darkBorder = iRECT(Panel->Bounds.l, Panel->Bounds.t, Panel->Bounds.r, Panel->Bounds.b);
  IRECT lightBorder = iRECT(Panel->Bounds.l + 1, Panel->Bounds.t + 1, Panel->Bounds.r - 1, Panel->Bounds.b - 1);
  DPOINT pt[100];
  int n;
  n = RoundRectangle2(pt, lightBorder, 3);
  imdrawaa_poly(im, 0, pt, n, 1, 0, ColorTable_PanelLightBorder, 1);
  imdrawaa_poly_gradient(im, 0, pt, &n, 1, lightBorder.l, lightBorder.t, lightBorder.l, lightBorder.b, ColorTable_PanelOverflowBackgroundSelectedNorth, Transparent);
  n = RoundRectangle2(pt, darkBorder, 3);
  imdrawaa_poly(im, 0, pt, n, 1, 0, ColorTable_PanelDarkBorder, 1);
  DrawPanelOverflowImage(im, Panel);
  return 0;
}
/// <summary>
/// Draws an overflown panel in pressed state
/// </summary>
/// <param name="e"></param>
int DrawPanelOverflowPressed(img_t* im, RibbonPanel* Panel)
{
  IRECT darkBorder = iRECT(Panel->Bounds.l, Panel->Bounds.t, Panel->Bounds.r, Panel->Bounds.b);
  IRECT lightBorder = iRECT(Panel->Bounds.l + 1, Panel->Bounds.t + 1, Panel->Bounds.r - 1, Panel->Bounds.b - 1);
  IRECT glossy = iRECT(Panel->Bounds.l, Panel->Bounds.t, Panel->Bounds.r, Panel->Bounds.t + 17);
  DPOINT pt[100];
  int n;
  n = RoundRectangle2(pt, darkBorder, 3);
  imdrawaa_poly_gradient_hv(im, 0, pt, &n, 1, lightBorder, 1, ColorTable_PanelOverflowBackgroundPressed, ColorTable_PanelOverflowBackgroundSelectedSouth);
  n = RoundRectangle(pt, glossy, 3, NorthEast | NorthWest);
  imdrawaa_poly_gradient_hv(im, 0, pt, &n, 1, glossy, 1, _ARGB(150, White), _ARGB(50, White));
  n = RoundRectangle2(pt, lightBorder, 3);
  imdrawaa_poly(im, 0, pt, n, 1, 0, _ARGB(40, White), 1);
  n = RoundRectangle2(pt, darkBorder, 3);
  imdrawaa_poly(im, 0, pt, n, 1, 0, ColorTable_PanelDarkBorder, 1);
  DrawPanelOverflowImage(im, Panel);
  DrawPressedShadow(im, glossy);
  return 0;
}
#if 0
// Button
/// <summary>
/// Gets the corners to round on the specified button
/// </summary>
/// <param name="r"></param>
/// <param name="button"></param>
/// <returns></returns>
int ButtonCorners(RibbonButton button)
{
  if (!(button.OwnerItem is RibbonItemGroup)) {
    return All;
  }
  else {
    RibbonItemGroup g = button.OwnerItem as RibbonItemGroup;
    Corners c = None;
    if (button == g.FirstItem) {
      c |= West;
    }
    if (button == g.LastItem) {
      c |= East;
    }
    return c;
  }
}
/// <summary>
/// Determines buttonface corners
/// </summary>
/// <param name="button"></param>
/// <returns></returns>
int ButtonFaceRounding(RibbonButton button)
{
  if (!(button.OwnerItem is RibbonItemGroup)) {
    if (button.SizeMode == RibbonElementSizeMode_Large) {
      return North;
    }
    else {
      return West;
    }
  }
  else {
    Corners c = None;
    RibbonItemGroup g = button.OwnerItem as RibbonItemGroup;
    if (button == g.FirstItem) {
      c |= West;
    }
    return c;
  }
}
/// <summary>
/// Determines button's dropDown corners
/// </summary>
/// <param name="button"></param>
/// <returns></returns>
Corners ButtonDdRounding(RibbonButton button)
{
  if (!(button.OwnerItem is RibbonItemGroup)) {
    if (button.SizeMode == RibbonElementSizeMode_Large) {
      return South;
    }
    else {
      return East;
    }
  }
  else {
    Corners c = None;
    RibbonItemGroup g = button.OwnerItem as RibbonItemGroup;
    if (button == g.LastItem) {
      c |= East;
    }
    return c;
  }
}
#endif
/// <summary>
/// Draws the orb's option buttons background
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
int DrawOrbOptionButton(img_t* im, IRECT bounds)
{
  IRECT rc1;
  bounds.r -= 1;
  bounds.b -= 1;
  imdrawaa_roundrect(im, 0, bounds, 3, All, ColorTable_OrbOptionBackground, 0, 0);
  rc1 = iRECT(bounds.l, bounds.t + RCH(&bounds) / 2, bounds.r, bounds.b - 2);
  imdraw_rect_gradient(im, &rc1, rc1, ColorTable_OrbOptionShine, ColorTable_OrbOptionBackground, 0);
  imdrawaa_roundrect(im, 0, bounds, 3, All, 0, ColorTable_OrbOptionBorder, 1);
  return 0;
}
/// <summary>
/// Draws a regular button in normal state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawButton(img_t* im, IRECT bounds, int corners)
{
  IRECT outerR = iRECT(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
  IRECT innerR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
  IRECT glossyR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + (int)(RCH(&bounds) * .36));
  double Factors[] = { 0, 0.8f, 0 };
  double Positions[] = { 0, 0.30f, 1 };
  COLOR lut[256];
  if (RCH(&bounds) <= 0 || RCW(&bounds) <= 0) {
    return 0;
  }
  imdrawaa_roundrect(im, 0, outerR, 3, corners, ColorTable_ButtonBgOut, 0, 0);
  fill_color_array3(256, lut, ColorTable_ButtonBgCenter, ColorTable_ButtonBgOut, 3, Positions, Factors);
  imdrawaa_ellipse_lut_radial2(im, 0, RCX(&bounds), bounds.b, RCW(&bounds)*.5, RCH(&bounds), lut);
  imdrawaa_roundrect(im, 0, outerR, 3, corners, 0, ColorTable_ButtonBorderOut, 1);
  imdrawaa_roundrect(im, 0, innerR, 3, corners, 0, ColorTable_ButtonBorderIn, 1);
  if (RCW(&glossyR) > 0 && RCH(&glossyR) > 0) {
    imdrawaa_roundrect_gradient_hv(im, 0, glossyR, 3, (corners & NorthWest) | (corners & NorthEast), 1, ColorTable_ButtonGlossyNorth, ColorTable_ButtonGlossySouth);
  }
  return 0;
}
/// <summary>
/// Draws the arrow of buttons
/// </summary>
/// <param name="g"></param>
/// <param name="button"></param>
int DrawButtonDropDownArrow(img_t* im, RibbonButton button, IRECT textLayout)
{
  IRECT bounds = {0};
  if (button.SizeMode == RibbonElementSizeMode_Large || button.SizeMode == RibbonElementSizeMode_Overflow) {
    bounds = LargeButtonDropDownArrowBounds(button.Text, textLayout);
  }
  else {
    //bounds = iRECT(
    // button.ButtonFaceBounds.r + (button.DropDownBounds.Width - arrowSize.w) / 2,
    // button.Bounds.t + (button.Bounds.Height - arrowSize.h) / 2,
    // arrowSize.w, arrowSize.h);
    bounds = textLayout;
  }
  DrawArrowShaded(im, bounds, button.DropDownArrowDirection, button.Enabled);
  return 0;
}
/// <summary>
/// Draws a regular button in disabled state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawButtonDisabled(img_t* im, IRECT bounds, int corners)
{
  IRECT outerR = iRECT(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
  IRECT innerR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
  IRECT glossyR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + (int)((double)RCH(&bounds) * .36));
  double Factors[] = { 0, 0.8f, 0 };
  double Positions[] = { 0, 0.30f, 1 };
  COLOR lut[256];
  imdrawaa_roundrect(im, 0, outerR, 3, corners, ColorTable_ButtonDisabledBgOut, 0, 0);
  fill_color_array3(256, lut, ColorTable_ButtonDisabledBgCenter, ColorTable_ButtonDisabledBgOut, 3, Positions, Factors);
  imdrawaa_ellipse_lut_radial2(im, 0, RCX(&bounds), bounds.b, RCW(&bounds)*.5, RCH(&bounds), lut);
  imdrawaa_roundrect(im, 0, outerR, 3, corners, 0, ColorTable_ButtonDisabledBorderOut, 1);
  imdrawaa_roundrect(im, 0, innerR, 3, corners, 0, ColorTable_ButtonDisabledBorderIn, 1);
  if (RCW(&glossyR) > 0 && RCH(&glossyR) > 0) {
    imdrawaa_roundrect_gradient_hv(im, 0, glossyR, 3, (corners & NorthWest) | (corners & NorthEast), 1, ColorTable_ButtonDisabledGlossyNorth, ColorTable_ButtonDisabledGlossySouth);
  }
  return 0;
}
/// <summary>
/// Draws the button as checked
/// </summary>
/// <param name="g"></param>
/// <param name="button"></param>
int DrawButtonChecked(img_t* im, IRECT bounds, int corners)
{
  IRECT outerR = iRECT(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
  IRECT innerR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
  IRECT glossyR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + (int)((double)RCH(&bounds) * .36));
  double Factors[] = { 0, 0.8f, 0 };
  double Positions[] = { 0, 0.30f, 1 };
  COLOR lut[256];
  imdrawaa_roundrect(im, 0, outerR, 3, corners, ColorTable_ButtonCheckedBgOut, 0, 0);
  fill_color_array3(256, lut, ColorTable_ButtonCheckedBgCenter, ColorTable_ButtonCheckedBgOut, 3, Positions, Factors);
  imdrawaa_ellipse_lut_radial2(im, 0, RCX(&bounds), bounds.b, RCW(&bounds)*.5, RCH(&bounds), lut);
  imdrawaa_roundrect(im, 0, outerR, 3, corners, 0, ColorTable_ButtonCheckedBorderOut, 1);
  imdrawaa_roundrect(im, 0, innerR, 3, corners, 0, ColorTable_ButtonCheckedBorderIn, 1);
  if (RCW(&glossyR) > 0 && RCH(&glossyR) > 0) {
    imdrawaa_roundrect_gradient_hv(im, 0, glossyR, 3, (corners & NorthWest) | (corners & NorthEast), 1, ColorTable_ButtonCheckedGlossyNorth, ColorTable_ButtonCheckedGlossySouth);
  }
  DrawPressedShadow(im, outerR);
  return 0;
}
/// <summary>
/// Draws a SplitDropDown button in normal state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawSplitButton(img_t* im, RibbonButton button)
{
  return 0;
}
/// <summary>
/// Draws a SplitDropDown button in pressed state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawSplitButtonPressed(img_t* im, RibbonButton button)
{
  return 0;
}
/// <summary>
/// Draws a SplitDropDown button in selected state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawSplitButtonSelected(img_t* im, RibbonButton button)
{
  IRECT outerR = iRECT(button.DropDownBounds.l, button.DropDownBounds.t, button.DropDownBounds.r - 1, button.DropDownBounds.b - 1);
  IRECT innerR = iRECT(outerR.l + 1, outerR.t + 1, outerR.r - 1, outerR.b - 1);
  IRECT faceOuterR = iRECT(button.ButtonFaceBounds.l, button.ButtonFaceBounds.t, button.ButtonFaceBounds.r - 1, button.ButtonFaceBounds.b - 1);
  IRECT faceInnerR = iRECT(faceOuterR.l + 1, faceOuterR.t + 1, faceOuterR.r + (button.SizeMode == RibbonElementSizeMode_Large ? -1 : 0), faceOuterR.b + (button.SizeMode == RibbonElementSizeMode_Large ? 0 : -1));
  int faceCorners = button.faceCorners;//ButtonFaceRounding(button);
  int ddCorners = button.ddCorners;//ButtonDdRounding(button);
  COLOR _clr;
  imdrawaa_roundrect(im, 0, innerR, 2, ddCorners, _ARGB(150, White), 0, 0);
  _clr = button.Pressed && button.SizeMode != RibbonElementSizeMode_DropDown ? ColorTable_ButtonPressedBorderOut : ColorTable_ButtonSelectedBorderOut;
  imdrawaa_roundrect(im, 0, outerR, 3, ddCorners, 0, _clr, 1);
  _clr = button.Pressed && button.SizeMode != RibbonElementSizeMode_DropDown ? ColorTable_ButtonPressedBorderIn : ColorTable_ButtonSelectedBorderIn;
  imdrawaa_roundrect(im, 0, faceInnerR, 2, faceCorners, 0, _clr, 1);
  return 0;
}
/// <summary>
/// Draws a SplitDropDown button with the dropdown area pressed
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawSplitButtonDropDownPressed(img_t* im, RibbonButton button)
{
  return 0;
}
/// <summary>
/// Draws a SplitDropDown button with the dropdown area selected
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
int DrawSplitButtonDropDownSelected(img_t* im, RibbonButton button)
{
  //IRECT outerR = iRECT(button.DropDownBounds.l,button.DropDownBounds.t,button.DropDownBounds.r - 1,button.DropDownBounds.b - 1);
  //IRECT innerR = iRECT(outerR.l + 1,outerR.t + (button.SizeMode == RibbonElementSizeMode_Large ? 1 : 0),outerR.r - 1,outerR.b - 1);
  IRECT faceOuterR = iRECT(button.ButtonFaceBounds.l, button.ButtonFaceBounds.t, button.ButtonFaceBounds.r - 1, button.ButtonFaceBounds.b - 1);
  IRECT faceInnerR = iRECT(faceOuterR.l + 1, faceOuterR.t + 1, faceOuterR.r + (button.SizeMode == RibbonElementSizeMode_Large ? -1 : 0), faceOuterR.b + (button.SizeMode == RibbonElementSizeMode_Large ? 0 : -1));
  int faceCorners = button.faceCorners;//ButtonFaceRounding(button);
  int ddCorners = button.ddCorners;//ButtonDdRounding(button);
  COLOR _clr;
  _clr = button.Pressed && button.SizeMode != RibbonElementSizeMode_DropDown ? ColorTable_ButtonPressedBorderIn : ColorTable_ButtonSelectedBorderIn;
  imdrawaa_roundrect(im, 0, faceInnerR, 2, faceCorners, _ARGB(150, White), _clr, 1);
  _clr = button.Pressed && button.SizeMode != RibbonElementSizeMode_DropDown ? ColorTable_ButtonPressedBorderOut : ColorTable_ButtonSelectedBorderOut;
  imdrawaa_roundrect(im, 0, faceOuterR, 3, faceCorners, 0, _clr, 1);
  return 0;
}
// Group
/// <summary>
/// Draws the background of the specified RibbonItemGroup
/// </summary>
/// <param name="e"></param>
/// <param name="?"></param>
int DrawItemGroup(img_t* im, RibbonItemGroup grp)
{
  IRECT outerR = iRECT(grp.Bounds.l, grp.Bounds.t, grp.Bounds.r - 1, grp.Bounds.b - 1);
  IRECT innerR = iRECT(outerR.l + 1, outerR.t + 1, outerR.r - 1, outerR.b - 1);
  IRECT glossyR = iRECT(outerR.l + 1, outerR.t + RCH(&outerR) / 2 + 1, outerR.r - 1, outerR.b - 1);
  imdrawaa_roundrect_gradient_hv(im, 0, innerR, 2, All, 1, ColorTable_ItemGroupBgNorth, ColorTable_ItemGroupBgSouth);
  imdrawaa_roundrect_gradient_hv(im, 0, glossyR, 2, All, 1, ColorTable_ItemGroupBgGlossy, Transparent);
  return 0;
}
/// <summary>
/// Draws the background of the specified RibbonItemGroup
/// </summary>
/// <param name="e"></param>
/// <param name="?"></param>
int DrawItemGroupBorder(img_t* im, RibbonItemGroup grp)
{
  IRECT outerR = iRECT(grp.Bounds.l, grp.Bounds.t, grp.Bounds.r - 1, grp.Bounds.b - 1);
  IRECT innerR = iRECT(outerR.l + 1, outerR.t + 1, outerR.r - 1, outerR.b - 1);
  int i;
  for (i = 0; i < grp.nitem; ++i) {
    RibbonItem* item = grp.items + i;
    imdraw_line(im, iPOINT(item->Bounds.r, item->Bounds.t), iPOINT(item->Bounds.r, item->Bounds.b), ColorTable_ItemGroupSeparatorDark, 1);
    imdraw_line(im, iPOINT(item->Bounds.r + 1, item->Bounds.t), iPOINT(item->Bounds.r + 1, item->Bounds.b), ColorTable_ItemGroupSeparatorLight, 1);
  }
  imdrawaa_roundrect(im, 0, outerR, 2, All, 0, ColorTable_ItemGroupOuterBorder, 1);
  imdrawaa_roundrect(im, 0, innerR, 2, All, 0, ColorTable_ItemGroupInnerBorder, 1);
  return 0;
}
#if 0
// ButtonList
int DrawButtonList(img_t* im, RibbonButtonList list)
{
  IRECT outerR = iRECT(list.Bounds.l, list.Bounds.t, list.Bounds.r - 1, list.Bounds.b);
  using(GraphicsPath path = RoundRectangle(outerR, 3, East)) {
    COLOR bgcolor = list.Selected ? ColorTable_ButtonListBgSelected : ColorTable_ButtonListBg;
    if (list.Canvas is RibbonDropDown) {
      bgcolor = ColorTable_DropDownBg;
    }
    using(SolidBrush b = new SolidBrush(bgcolor)) {
      g.FillPath(b, path);
    }
    using(Pen p = new Pen(ColorTable_ButtonListBorder)) {
      g.DrawPath(p, path);
    }
  }
  if (list.ScrollType == RibbonButtonList.ListScrollType.Scrollbar && ScrollBarRenderer.IsSupported) {
    ScrollBarRenderer.DrawUpperVerticalTrack(g, list.ScrollBarBounds, ScrollBarState.Normal);
    if (list.ThumbPressed) {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Pressed);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Pressed);
    }
    else if (list.ThumbSelected) {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Hot);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Hot);
    }
    else {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Normal);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Normal);
    }
    if (list.ButtonUpPressed) {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpPressed);
    }
    else if (list.ButtonUpSelected) {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpHot);
    }
    else {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpNormal);
    }
    if (list.ButtonDownPressed) {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownPressed);
    }
    else if (list.ButtonDownSelected) {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownHot);
    }
    else {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownNormal);
    }
  }
  else {
    // Control Buttons
    if (list.ScrollType == RibbonButtonList.ListScrollType.Scrollbar) {
      using(SolidBrush b = new SolidBrush(ColorTable_ButtonGlossyNorth)) {
        g.FillRectangle(b, list.ScrollBarBounds);
      }
    }
    if (!list.ButtonDownEnabled) {
      DrawButtonDisabled(g, list.ButtonDownBounds, list.ButtonDropDownPresent ? None : SouthEast);
    }
    else if (list.ButtonDownPressed) {
      DrawButtonPressed(g, list.ButtonDownBounds, list.ButtonDropDownPresent ? None : SouthEast);
    }
    else if (list.ButtonDownSelected) {
      DrawButtonSelected(g, list.ButtonDownBounds, list.ButtonDropDownPresent ? None : SouthEast);
    }
    else {
      DrawButton(g, list.ButtonDownBounds, None);
    }
    if (!list.ButtonUpEnabled) {
      DrawButtonDisabled(g, list.ButtonUpBounds, NorthEast);
    }
    else if (list.ButtonUpPressed) {
      DrawButtonPressed(g, list.ButtonUpBounds, NorthEast);
    }
    else if (list.ButtonUpSelected) {
      DrawButtonSelected(g, list.ButtonUpBounds, NorthEast);
    }
    else {
      DrawButton(g, list.ButtonUpBounds, NorthEast);
    }
    if (list.ButtonDropDownPresent) {
      if (list.ButtonDropDownPressed) {
        DrawButtonPressed(g, list.ButtonDropDownBounds, SouthEast);
      }
      else if (list.ButtonDropDownSelected) {
        DrawButtonSelected(g, list.ButtonDropDownBounds, SouthEast);
      }
      else {
        DrawButton(g, list.ButtonDropDownBounds, SouthEast);
      }
    }
    if (list.ScrollType == RibbonButtonList.ListScrollType.Scrollbar && list.ScrollBarEnabled) {
      if (list.ThumbPressed) {
        DrawButtonPressed(g, list.ThumbBounds, All);
      }
      else if (list.ThumbSelected) {
        DrawButtonSelected(g, list.ThumbBounds, All);
      }
      else {
        DrawButton(g, list.ThumbBounds, All);
      }
    }
    COLOR dk = ColorTable_Arrow;
    COLOR lt = ColorTable_ArrowLight;
    COLOR ds = ColorTable_ArrowDisabled;
    IRECT arrUp = CenterOn(list.ButtonUpBounds, iRECT(IPOINT.Empty, arrowSize));
    arrUp.Offset(0, 1);
    IRECT arrD = CenterOn(list.ButtonDownBounds, iRECT(IPOINT.Empty, arrowSize));
    arrD.Offset(0, 1);
    IRECT arrdd = CenterOn(list.ButtonDropDownBounds, iRECT(IPOINT.Empty, arrowSize));
    arrdd.Offset(0, 3);
    DrawArrow(g, arrUp, list.ButtonUpEnabled ? lt : COLOR.Transparent, RibbonArrowDirection_Up);
    arrUp.Offset(0, -1);
    DrawArrow(g, arrUp, list.ButtonUpEnabled ? dk : ds, RibbonArrowDirection_Up);
    DrawArrow(g, arrD, list.ButtonDownEnabled ? lt : COLOR.Transparent, RibbonArrowDirection_Down);
    arrD.Offset(0, -1);
    DrawArrow(g, arrD, list.ButtonDownEnabled ? dk : ds, RibbonArrowDirection_Down);
    if (list.ButtonDropDownPresent) {
      using(SolidBrush b = new SolidBrush(ColorTable_Arrow)) {
        SmoothingMode sm = g.SmoothingMode;
        g.SmoothingMode = SmoothingMode.None;
        g.FillRectangle(b, iRECT(iPOINT(arrdd.l - 1, arrdd.t - 4), iSIZE(arrowSize.w + 2, 1)));
        g.SmoothingMode = sm;
      }
      DrawArrow(g, arrdd, lt, RibbonArrowDirection_Down);
      arrdd.Offset(0, -1);
      DrawArrow(g, arrdd, dk, RibbonArrowDirection_Down);
    }
  }
}
// Separator
int DrawSeparator(img_t* im, RibbonSeparator separator)
{
  if (separator.SizeMode == RibbonElementSizeMode_DropDown) {
    if (!string.IsNullOrEmpty(separator.Text)) {
      using(SolidBrush b = new SolidBrush(ColorTable_SeparatorBg)) {
        g.FillRectangle(b, separator.Bounds);
      }
      using(Pen p = new Pen(ColorTable_SeparatorLine)) {
        g.DrawLine(p,
            iPOINT(separator.Bounds.l, separator.Bounds.b),
            iPOINT(separator.Bounds.r, separator.Bounds.b));
      }
    }
    else {
      using(Pen p = new Pen(ColorTable_DropDownImageSeparator)) {
        g.DrawLine(p,
            iPOINT(separator.Bounds.l + 30, separator.Bounds.t + 1),
            iPOINT(separator.Bounds.r, separator.Bounds.t + 1));
      }
    }
  }
  else {
    using(Pen p = new Pen(ColorTable_SeparatorDark)) {
      g.DrawLine(p,
          iPOINT(separator.Bounds.l, separator.Bounds.t),
          iPOINT(separator.Bounds.l, separator.Bounds.b));
    }
    using(Pen p = new Pen(ColorTable_SeparatorLight)) {
      g.DrawLine(p,
          iPOINT(separator.Bounds.l + 1, separator.Bounds.t),
          iPOINT(separator.Bounds.l + 1, separator.Bounds.b));
    }
  }
}
// TextBox
/// <summary>
/// Draws a disabled textbox
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
int DrawTextBoxDisabled(img_t* im, IRECT bounds)
{
  using(SolidBrush b = new SolidBrush(SystemColors.Control)) {
    g.FillRectangle(b, bounds);
  }
  using(Pen p = new Pen(ColorTable_TextBoxBorder)) {
    g.DrawRectangle(p, bounds);
  }
}
/// <summary>
/// Draws an unselected textbox
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
int DrawTextBoxUnselected(img_t* im, IRECT bounds)
{
  using(SolidBrush b = new SolidBrush(ColorTable_TextBoxUnselectedBg)) {
    g.FillRectangle(b, bounds);
  }
  using(Pen p = new Pen(ColorTable_TextBoxBorder)) {
    g.DrawRectangle(p, bounds);
  }
}
/// <summary>
/// Draws an unselected textbox
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
int DrawTextBoxSelected(img_t* im, IRECT bounds)
{
  using(GraphicsPath path = RoundRectangle(bounds, 3)) {
    using(SolidBrush b = new SolidBrush(SystemColors.Window)) {
      //g.FillPath(b, path);
      g.FillRectangle(b, bounds);
    }
    using(Pen p = new Pen(ColorTable_TextBoxBorder)) {
      //g.DrawPath(p, path);
      g.DrawRectangle(p, bounds);
    }
  }
}
// ComboBox
int DrawComboxDropDown(img_t* im, RibbonComboBox b)
{
  if (b.DropDownButtonPressed) {
    DrawButtonPressed(g, b.DropDownButtonBounds, None);
  }
  else if (b.DropDownButtonSelected) {
    DrawButtonSelected(g, b.DropDownButtonBounds, None);
  }
  else if (b.Selected) {
    DrawButton(g, b.DropDownButtonBounds, None);
  }
  DrawArrowShaded(g, b.DropDownButtonBounds, RibbonArrowDirection_Down, true);//b.Enabled);
}
override int OnRenderRibbonOrb(RibbonRenderEventArgs e)
{
  if (s->OrbVisible) {
    DrawOrb(im, s->OrbBounds, s->OrbImage, s->OrbSelected, s->OrbPressed);
  }
}
override int OnRenderRibbonQuickAccessToolbarBackground(RibbonRenderEventArgs e)
{
  /// a-----b a-----b
  /// z | z |
  /// c---d c-----d
  IRECT bounds = s->QuickAcessToolbar.Bounds;
  Padding padding = s->QuickAcessToolbar.Padding;
  Padding margin = s->QuickAcessToolbar.Margin;
  IPOINT a = iPOINT(bounds.l - (s->OrbVisible ? margin.l : 0), bounds.t);
  IPOINT b = iPOINT(bounds.r + padding.r, bounds.t);
  IPOINT c = iPOINT(bounds.l, bounds.b);
  IPOINT d = iPOINT(b.X, c.Y);
  IPOINT z = iPOINT(c.X - 2, a.Y + RCH(&bounds) / 2 - 1);
  BOOL aero = s->ActualBorderMode == NonClientAreaGlass && RibbonDesigner.Current == null;
  if (!aero) {
    using(Pen p = new Pen(ColorTable_QuickAccessBorderLight, 3)) {
      using(GraphicsPath path = CreateQuickAccessPath(a, b, c, d, z, bounds, 0, 0, e.Ribbon)) {
        im.DrawPath(p, path);
      }
    }
  }
  using(GraphicsPath path = CreateQuickAccessPath(a, b, c, d, z, bounds, 0, 0, e.Ribbon)) {
    using(Pen p = new Pen(ColorTable_QuickAccessBorderDark)) {
      if (aero) {
        p.COLOR = _ARGB(150, 150, 150) ;
      }
      im.DrawPath(p, path);
    }
    if (!aero) {
      using(LinearGradientBrush br = new LinearGradientBrush(
          b, d, _ARGB(150, ColorTable_QuickAccessUpper), _ARGB(150, ColorTable_QuickAccessLower)
      )) {
        im.FillPath(br, path);
      }
    }
    else {
      using(LinearGradientBrush br = new LinearGradientBrush(
          b, d,
          _ARGB(66, RibbonProfesionalRendererColorTable_ToGray(ColorTable_QuickAccessUpper)),
          _ARGB(66, RibbonProfesionalRendererColorTable_ToGray(ColorTable_QuickAccessLower))
      )) {
        im.FillPath(br, path);
      }
    }
  }
}
GraphicsPath CreateQuickAccessPath(IPOINT a, IPOINT b, IPOINT c, IPOINT d, IPOINT e, IRECT bounds, int offsetx, int offsety, Ribbon ribbon)
{
  a.Offset(offsetx, offsety);
  b.Offset(offsetx, offsety);
  c.Offset(offsetx, offsety);
  d.Offset(offsetx, offsety);
  e.Offset(offsetx, offsety);
  GraphicsPath path = new GraphicsPath();
  ADDLINE(a, b);
  ADDARC(iRECT(b.X - RCH(&bounds) / 2, b.Y, RCH(&bounds), RCH(&bounds)), -90, 180);
  ADDLINE(d, c);
  if (ribbon.OrbVisible) {
    path.AddCurve(iPOINT[] { c, e, a });
  }
  else {
    ADDARC(iRECT(a.X - RCH(&bounds) / 2, a.Y, RCH(&bounds), RCH(&bounds)), 90, 180);
  }
  return path;
}
/// <summary>
/// Draws the orb on the specified state
/// </summary>
/// <param name="g">Device to draw</param>
/// <param name="ribbon">Ribbon that the orb belongs to</param>
/// <param name="r">Layout rectangle for the orb</param>
/// <param name="selected">Specifies if the orb should be drawn as selected</param>
/// <param name="pressed">Specifies if the orb should be drawn as pressed</param>
int DrawOrb(img_t* im, IRECT r, Image image, BOOL selected, BOOL pressed)
{
  int sweep, start;
  IPOINT p1, p2, p3;
  COLOR bgdark, bgmed, bglight, light;
  IRECT rinner = r;
  rinner.Inflate(-1, -1);
  IRECT shadow = r;
  shadow.Offset(1, 1);
  shadow.Inflate(2, 2);
  // COLOR Selection
  if (pressed) {
    bgdark = ColorTable_OrbPressedBackgroundDark;
    bgmed = ColorTable_OrbPressedBackgroundMedium;
    bglight = ColorTable_OrbPressedBackgroundLight;
    light = ColorTable_OrbPressedLight;
  }
  else if (selected) {
    bgdark = ColorTable_OrbSelectedBackgroundDark;
    bgmed = ColorTable_OrbSelectedBackgroundDark;
    bglight = ColorTable_OrbSelectedBackgroundLight;
    light = ColorTable_OrbSelectedLight;
  }
  else {
    bgdark = ColorTable_OrbBackgroundDark;
    bgmed = ColorTable_OrbBackgroundMedium;
    bglight = ColorTable_OrbBackgroundLight;
    light = ColorTable_OrbLight;
  }
  // Shadow
  using(GraphicsPath p = new GraphicsPath()) {
    p.AddEllipse(shadow);
    using(PathGradientBrush gradient = new PathGradientBrush(p)) {
      gradient.WrapMode = WrapMode.Clamp;
      gradient.CenterPoint = new PointF(shadow.l + shadow.Width / 2, shadow.t + shadow.Height / 2);
      gradient.CenterColor = _ARGB(180, COLOR.Black);
      gradient.SurroundColors = new COLOR[] { COLOR.Transparent };
      Blend blend = new Blend(3);
      blend.Factors = new float[] { 0f, 1f, 1f };
      blend.Positions = new float[] { 0, 0.2f, 1f };
      gradient.Blend = blend;
      g.FillPath(gradient, p);
    }
  }
  // Orb Background
  using(Pen p = new Pen(bgdark, 1)) {
    g.DrawEllipse(p, r);
  }
  using(GraphicsPath p = new GraphicsPath()) {
    p.AddEllipse(r);
    using(PathGradientBrush gradient = new PathGradientBrush(p)) {
      gradient.WrapMode = WrapMode.Clamp;
      gradient.CenterPoint = new PointF(Convert.ToSingle(r.l + r.Width / 2), Convert.ToSingle(r.b));
      gradient.CenterColor = bglight;
      gradient.SurroundColors = new COLOR[] { bgmed };
      Blend blend = new Blend(3);
      blend.Factors = new float[] { 0f, .8f, 1f };
      blend.Positions = new float[] { 0, 0.50f, 1f };
      gradient.Blend = blend;
      g.FillPath(gradient, p);
    }
  }
  // Bottom round shine
  IRECT bshine = iRECT(0, 0, r.Width / 2, r.Height / 2);
  bshine.X = r.X + (r.Width - bshine.Width) / 2;
  bshine.Y = r.Y + r.Height / 2;
  using(GraphicsPath p = new GraphicsPath()) {
    p.AddEllipse(bshine);
    using(PathGradientBrush gradient = new PathGradientBrush(p)) {
      gradient.WrapMode = WrapMode.Clamp;
      gradient.CenterPoint = new PointF(Convert.ToSingle(r.l + r.Width / 2), Convert.ToSingle(r.b));
      gradient.CenterColor = COLOR.White;
      gradient.SurroundColors = new COLOR[] { COLOR.Transparent };
      g.FillPath(gradient, p);
    }
  }
  // Upper Glossy
  using(GraphicsPath p = new GraphicsPath()) {
    sweep = 160;
    start = 180 + (180 - sweep) / 2;
    p.AddArc(rinner, start, sweep);
    p1 = IPOINT.Round(p.PathData.Points[0]);
    p2 = IPOINT.Round(p.PathData.Points[p.PathData.Points.Length - 1]);
    p3 = iPOINT(rinner.l + rinner.Width / 2, p2.Y - 3);
    p.AddCurve(iPOINT[] { p2, p3, p1 });
    using(PathGradientBrush gradient = new PathGradientBrush(p)) {
      gradient.WrapMode = WrapMode.Clamp;
      gradient.CenterPoint = p3;
      gradient.CenterColor = COLOR.Transparent;
      gradient.SurroundColors = new COLOR[] { light };
      Blend blend = new Blend(3);
      blend.Factors = new float[] { .3f, .8f, 1f };
      blend.Positions = new float[] { 0, 0.50f, 1f };
      gradient.Blend = blend;
      g.FillPath(gradient, p);
    }
    using(LinearGradientBrush b = new LinearGradientBrush(iPOINT(r.l, r.t), iPOINT(r.l, p1.Y), COLOR.White, COLOR.Transparent)) {
      Blend blend = new Blend(4);
      blend.Factors = new float[] { 0f, .4f, .8f, 1f };
      blend.Positions = new float[] { 0f, .3f, .4f, 1f };
      b.Blend = blend;
      g.FillPath(b, p);
    }
  }
  // Upper Shine
  /////Lower gloss
  //using (GraphicsPath p = new GraphicsPath())
  //{
  // sweep = 140;
  // start = (180 - sweep) / 2;
  // p.AddArc(rinner, start, sweep);
  // p1 = IPOINT.Round(p.PathData.Points[0]);
  // p2 = IPOINT.Round(p.PathData.Points[p.PathData.Points.Length - 1]);
  // p3 = iPOINT(rinner.l + rinner.Width / 2, p1.Y + 3);
  // p.AddCurve(iPOINT[] { p2, p3, p1 });
  // g.FillPath(Brushes.White, p);
  //}
  ///Upper shine
  using(GraphicsPath p = new GraphicsPath()) {
    sweep = 160;
    start = 180 + (180 - sweep) / 2;
    p.AddArc(rinner, start, sweep);
    using(Pen pen = new Pen(COLOR.White)) {
      g.DrawPath(pen, p);
    }
  }
  // Lower Shine
  using(GraphicsPath p = new GraphicsPath()) {
    sweep = 160;
    start = (180 - sweep) / 2;
    p.AddArc(rinner, start, sweep);
    IPOINT pt = IPOINT.Round(p.PathData.Points[0]);
    IRECT rrinner = rinner;
    rrinner.Inflate(-1, -1);
    sweep = 160;
    start = (180 - sweep) / 2;
    p.AddArc(rrinner, start, sweep);
    using(LinearGradientBrush b = new LinearGradientBrush(
        iPOINT(rinner.l, rinner.b),
        iPOINT(rinner.l, pt.Y - 1),
        light, _ARGB(50, light))) {
      g.FillPath(b, p);
    }
    //p1 = IPOINT.Round(p.PathData.Points[0]);
    //p2 = IPOINT.Round(p.PathData.Points[p.PathData.Points.Length - 1]);
    //p3 = iPOINT(rinner.l + rinner.Width / 2, rinner.b - 1);
    //p.AddCurve(iPOINT[] { p2, p3, p1 });
    //using (LinearGradientBrush b = new LinearGradientBrush(
    // iPOINT(rinner.l, rinner.b + 1),
    // iPOINT(rinner.l, p1.Y),
    // _ARGB(200, COLOR.White), COLOR.Transparent))
    //{
    // g.FillPath(b, p);
    //}
  }
  // Orb Icon
  if (image != null) {
    IRECT irect = iRECT(IPOINT.Empty, image.ISIZE);
    irect.X = r.X + (r.Width - irect.Width) / 2;
    irect.Y = r.Y + (r.Height - irect.Height) / 2;
    g.DrawImage(image, irect);
  }
}
// Ribbon Orb DropDown
override int OnRenderOrbDropDownBackground(RibbonOrbDropDownEventArgs e)
{
  int Width = e.RibbonOrbDropDown.Width;
  int Height = e.RibbonOrbDropDown.Height;
  IRECT OrbDDContent = e.RibbonOrbDropDown.ContentBounds;
  IRECT Bcontent = e.RibbonOrbDropDown.ContentButtonsBounds;
  IRECT OuterRect = iRECT(0, 0, Width - 1, Height - 1);
  IRECT InnerRect = iRECT(1, 1, Width - 3, Height - 3);
  IRECT NorthNorthRect = iRECT(1, 1, Width - 3, OrbDDContent.t / 2);
  IRECT northSouthRect = iRECT(1, NorthNorthRect.b, NorthNorthRect.Width, OrbDDContent.t / 2);
  IRECT southSouthRect = iRECT(1,
      (Height - OrbDDContent.b) / 2 + OrbDDContent.b, Width - 1, Height - 1);
  COLOR OrbDropDownDarkBorder = ColorTable_OrbDropDownDarkBorder;
  COLOR OrbDropDownLightBorder = ColorTable_OrbDropDownLightBorder;
  COLOR OrbDropDownBack = ColorTable_OrbDropDownBack;
  COLOR OrbDropDownNorthA = ColorTable_OrbDropDownNorthA;
  COLOR OrbDropDownNorthB = ColorTable_OrbDropDownNorthB;
  COLOR OrbDropDownNorthC = ColorTable_OrbDropDownNorthC;
  COLOR OrbDropDownNorthD = ColorTable_OrbDropDownNorthD;
  COLOR OrbDropDownSouthC = ColorTable_OrbDropDownSouthC;
  COLOR OrbDropDownSouthD = ColorTable_OrbDropDownSouthD;
  COLOR OrbDropDownContentbg = ColorTable_OrbDropDownContentbg;
  COLOR OrbDropDownContentbglight = ColorTable_OrbDropDownContentbglight;
  COLOR OrbDropDownSeparatorlight = ColorTable_OrbDropDownSeparatorlight;
  COLOR OrbDropDownSeparatordark = ColorTable_OrbDropDownSeparatordark;
  GraphicsPath innerPath = RoundRectangle(InnerRect, 6);
  GraphicsPath outerPath = RoundRectangle(OuterRect, 6);
  im.SmoothingMode = SmoothingMode.None;
  using(Brush b = new SolidBrush(_ARGB(0x8e, 0x8e, 0x8e))) {
    im.FillRectangle(b, iRECT(Width - 10, Height - 10, 10, 10));
  }
  using(Brush b = new SolidBrush(OrbDropDownBack)) {
    im.FillPath(b, outerPath);
  }
  GradientRect(im, NorthNorthRect, OrbDropDownNorthA, OrbDropDownNorthB);
  GradientRect(im, northSouthRect, OrbDropDownNorthC, OrbDropDownNorthD);
  GradientRect(im, southSouthRect, OrbDropDownSouthC, OrbDropDownSouthD);
  using(Pen p = new Pen(OrbDropDownDarkBorder)) {
    im.DrawPath(p, outerPath);
  }
  im.SmoothingMode = SmoothingMode.AntiAlias;
  using(Pen p = new Pen(OrbDropDownLightBorder)) {
    im.DrawPath(p, innerPath);
  }
  innerPath.Dispose();
  outerPath.Dispose();
  // Content
  InnerRect = OrbDDContent;
  InnerRect.Inflate(0, 0);
  OuterRect = OrbDDContent;
  OuterRect.Inflate(1, 1);
  using(SolidBrush b = new SolidBrush(OrbDropDownContentbg)) {
    im.FillRectangle(b, OrbDDContent);
  }
  using(SolidBrush b = new SolidBrush(OrbDropDownContentbglight)) {
    im.FillRectangle(b, Bcontent);
  }
  using(Pen p = new Pen(OrbDropDownSeparatorlight)) {
    im.DrawLine(p, Bcontent.r, Bcontent.t, Bcontent.r, Bcontent.b);
  }
  using(Pen p = new Pen(OrbDropDownSeparatordark)) {
    im.DrawLine(p, Bcontent.r - 1, Bcontent.t, Bcontent.r - 1, Bcontent.b);
  }
  using(Pen p = new Pen(OrbDropDownLightBorder)) {
    im.DrawRectangle(p, OuterRect);
  }
  using(Pen p = new Pen(OrbDropDownDarkBorder)) {
    im.DrawRectangle(p, InnerRect);
  }
  // Orb
  IRECT orbb = s->RectangleToScreen(s->OrbBounds);
  orbb = e.RibbonOrbDropDown.RectangleToClient(orbb);
  DrawOrb(im, orbb, s->OrbImage, s->OrbSelected, s->OrbPressed);
}
// Overrides
override int OnRenderRibbonTab(const RibbonTab* t)
{
  if (t->Active) {
    DrawCompleteTab(e);
  }
  else if (t->Pressed) {
    DrawTabPressed(e);
  }
  else if (t->Selected) {
    DrawTabSelected(e);
  }
  else {
    DrawTabNormal(e);
  }
}
override int OnRenderRibbonTabText(const RibbonTab* t)
{
  StringFormat sf = new StringFormat();
  sf.Alignment = StringAlignment.Center;
  sf.Trimming = StringTrimming.EllipsisCharacter;
  sf.LineAlignment = StringAlignment.Center;
  sf.FormatFlags |= StringFormatFlags.NoWrap;
  IRECT r = iRECT(t->TabBounds.l + s->TabTextMargin.l, t->TabBounds.t + s->TabTextMargin.t, t->TabBounds.r - s->TabTextMargin.r, t->TabBounds.b - s->TabTextMargin.b);
  using(Brush b = new SolidBrush(GetTextColor(true, t->Active ? ColorTable_TabActiveText : ColorTable_TabText))) {
    im.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
    im.DrawString(t->Text, s->Font, b, r, sf);
  }
}
override int OnRenderRibbonPanelBackground(img_t* im, RibbonPanel* Panel)
{
  if (Panel->OverflowMode && !(e.Canvas is RibbonPanelPopup)) {
    if (Panel->Pressed) {
      DrawPanelOverflowPressed(e);
    }
    else if (Panel->Selected) {
      DrawPannelOveflowSelected(e);
    }
    else {
      DrawPanelOverflowNormal(e);
    }
  }
  else {
    if (Panel->Selected) {
      DrawPanelSelected(e);
    }
    else {
      DrawPanelNormal(e);
    }
  }
}
override int OnRenderRibbonPanelText(img_t* im, RibbonPanel* Panel)
{
  if (Panel->OverflowMode && !(e.Canvas is RibbonPanelPopup)) {
    return;
  }
  IRECT textArea =
      iRECT(
          Panel->Bounds.l + 1,
          Panel->ContentBounds.b,
          Panel->Bounds.r - 1,
          Panel->Bounds.b - 1);
  StringFormat sf = new StringFormat();
  sf.Alignment = StringAlignment.Center;
  sf.LineAlignment = StringAlignment.Center;
  using(Brush b = new SolidBrush(GetTextColor(Panel->Enabled, ColorTable_PanelText))) {
    im.DrawString(Panel->Text, s->Font, b, textArea, sf);
  }
}
override int OnRenderRibbonItemBorder(img_t* im)
{
  if (e.Item is RibbonItemGroup) {
    DrawItemGroupBorder(e, e.Item as RibbonItemGroup);
  }
}
override int OnRenderRibbonItemText(RibbonTextEventArgs e)
{
  COLOR foreColor = e.COLOR;
  StringFormat sf = e.Format;
  Font f = s->Font;
  BOOL embedded = FALSE;
  if (e.Item is RibbonButton) {
    // Button
    RibbonButton b = e.Item as RibbonButton;
    if (b is RibbonCaptionButton) {
      if (WinApi.IsWindows) {
        f = new Font("Marlett", f.ISIZE);
      }
      embedded = true;
      foreColor = ColorTable_Arrow;
    }
    if (b.Style == RibbonButtonStyle_DropDown && b.SizeMode == RibbonElementSizeMode_Large) {
      DrawButtonDropDownArrow(im, b, e.Bounds);
    }
  }
  else if (e.Item is RibbonSeparator) {
    foreColor = GetTextColor(e.Item.Enabled);
  }
  embedded = embedded || !e.Item.Enabled;
  if (embedded) {
    IRECT cbr = e.Bounds;
    cbr.Y++;
    using(SolidBrush b = new SolidBrush(ColorTable_ArrowLight)) {
      im.DrawString(e.Text, new Font(f, e.Style), b, cbr, sf);
    }
  }
  if (foreColor.Equals(COLOR.Empty)) {
    foreColor = GetTextColor(e.Item.Enabled);
  }
  using(SolidBrush b = new SolidBrush(foreColor)) {
    im.DrawString(e.Text, new Font(f, e.Style), b, e.Bounds, sf);
  }
}
override int OnRenderRibbonItemImage(RibbonItemBoundsEventArgs e)
{
  Image img = e.Item.Image;
  if (e.Item is RibbonButton) {
    if (!(e.Item.SizeMode == RibbonElementSizeMode_Large || e.Item.SizeMode == RibbonElementSizeMode_Overflow)) {
      img = (e.Item as RibbonButton).SmallImage;
    }
  }
  if (img != null) {
    if (!e.Item.Enabled) {
      img = CreateDisabledImage(img);
    }
    im.DrawImage(img, e.Bounds);
  }
}
override int OnRenderPanelPopupBackground(RibbonCanvasEventArgs e)
{
  RibbonPanel pnl = e.RelatedObject as RibbonPanel;
  if (pnl == null) {
    return;
  }
  IRECT darkBorder = iRECT(
      e.Bounds.l,
      e.Bounds.t,
      e.Bounds.r,
      e.Bounds.b);
  IRECT lightBorder = iRECT(
      e.Bounds.l + 1,
      e.Bounds.t + 1,
      e.Bounds.r - 1,
      e.Bounds.b - 1);
  IRECT textArea =
      iRECT(
          e.Bounds.l + 1,
          pnl.ContentBounds.b,
          e.Bounds.r - 1,
          e.Bounds.b - 1);
  GraphicsPath dark = RoundRectangle(darkBorder, 3);
  GraphicsPath light = RoundRectangle(lightBorder, 3);
  GraphicsPath txt = RoundRectangle(textArea, 3, SouthEast | SouthWest);
  using(Pen p = new Pen(ColorTable_PanelLightBorder)) {
    im.DrawPath(p, light);
  }
  using(Pen p = new Pen(ColorTable_PanelDarkBorder)) {
    im.DrawPath(p, dark);
  }
  using(SolidBrush b = new SolidBrush(ColorTable_PanelBackgroundSelected)) {
    im.FillPath(b, light);
  }
  using(SolidBrush b = new SolidBrush(ColorTable_PanelTextBackground)) {
    im.FillPath(b, txt);
  }
  txt.Dispose();
  dark.Dispose();
  light.Dispose();
}
override int OnRenderDropDownBackground(RibbonCanvasEventArgs e)
{
  IRECT outerR = iRECT(0, 0, e.Bounds.Width - 1, e.Bounds.Height - 1);
  IRECT imgsR = iRECT(0, 0, 26, e.Bounds.Height);
  RibbonDropDown dd = e.Canvas as RibbonDropDown;
  using(SolidBrush b = new SolidBrush(ColorTable_DropDownBg)) {
    im.Clear(COLOR.Transparent);
    SmoothingMode sbuff = im.SmoothingMode;
    im.SmoothingMode = SmoothingMode.AntiAlias;
    im.FillRectangle(b, outerR);
    im.SmoothingMode = sbuff;
  }
  if (dd != null && dd.DrawIconsBar) {
    using(SolidBrush b = new SolidBrush(ColorTable_DropDownImageBg)) {
      im.FillRectangle(b, imgsR);
    }
    using(Pen p = new Pen(ColorTable_DropDownImageSeparator)) {
      im.DrawLine(p,
          iPOINT(imgsR.r, imgsR.t),
          iPOINT(imgsR.r, imgsR.b));
    }
  }
  using(Pen p = new Pen(ColorTable_DropDownBorder)) {
    if (dd != null) {
      using(GraphicsPath r = RoundRectangle(iRECT(IPOINT.Empty, iSIZE(dd.ISIZE.Width - 1 , dd.ISIZE.Height - 1)), dd.BorderRoundness)) {
        SmoothingMode smb = im.SmoothingMode;
        im.SmoothingMode = SmoothingMode.AntiAlias;
        im.DrawPath(p, r);
        im.SmoothingMode = smb;
      }
    }
    else {
      im.DrawRectangle(p, outerR);
    }
  }
  if (dd.ShowSizingGrip) {
    IRECT gripArea = iRECT(
        e.Bounds.l + 1,
        e.Bounds.b - dd.SizingGripHeight,
        e.Bounds.r - 1,
        e.Bounds.b - 1);
    using(LinearGradientBrush b = new LinearGradientBrush(
        gripArea, ColorTable_DropDownGripNorth, ColorTable_DropDownGripSouth, 90)) {
      im.FillRectangle(b, gripArea);
    }
    using(Pen p = new Pen(ColorTable_DropDownGripBorder)) {
      im.DrawLine(p,
          gripArea.Location,
          iPOINT(gripArea.r - 1, gripArea.t));
    }
    DrawGripDot(im, iPOINT(gripArea.r - 7, gripArea.b - 3));
    DrawGripDot(im, iPOINT(gripArea.r - 3, gripArea.b - 7));
    DrawGripDot(im, iPOINT(gripArea.r - 3, gripArea.b - 3));
  }
}
override int OnRenderTabScrollButtons(const RibbonTab* t)
{
  if (t->ScrollLeftVisible) {
    if (t->ScrollLeftSelected) {
      DrawButtonSelected(im, t->ScrollLeftBounds, West);
    }
    else {
      DrawButton(im, t->ScrollLeftBounds, West);
    }
    DrawArrowShaded(im, t->ScrollLeftBounds, RibbonArrowDirection_Right, true);
  }
  if (t->ScrollRightVisible) {
    if (t->ScrollRightSelected) {
      DrawButtonSelected(im, t->ScrollRightBounds, East);
    }
    else {
      DrawButton(im, t->ScrollRightBounds, East);
    }
    DrawArrowShaded(im, t->ScrollRightBounds, RibbonArrowDirection_Left, true);
  }
}
#endif

