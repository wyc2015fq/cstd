// #region Types

typedef gc_t Graphics;
#include "RibbonColorPart.inl"
typedef struct {
  // #region Theme Information
  char* ThemeName;
  char* ThemeAuthor;
  char* ThemeAuthorEmail;
  char* ThemeAuthorWebsite;
  char* ThemeDateCreated;
  // #endregion
  COLOR ColorTable[RibbonColorPartMax];
} RibbonProfesionalRendererTheme;

static RibbonProfesionalRendererTheme Theme[1] = {0};
#include "ColorTables/RibbonProfesionalRendererColorTable.inl"

// #endregion

// #region Fields

ISIZE arrowSize = iSIZE(5, 3);
ISIZE moreSize = iSIZE(7, 7);

// #endregion

// #region Ctor

// #endregion

// #region Props
// RibbonProfesionalRendererColorTable _colorTable;

// RibbonProfesionalRendererColorTable ColorTable
//{
//    get { return _colorTable; }
//    set { _colorTable = value; }
//}

// #endregion

// #region Methods

// #region Util

Color GetTextColor(bool enabled, Color alternative)
{
  if (enabled)
  {
    return alternative;
  }
  else
  {
    return ThemeColorTable(ArrowDisabled);
  }
}

/// <summary>
/// Creates a rectangle with the specified corners rounded
/// </summary>
/// <param name="r"></param>
/// <param name="radius"></param>
/// <param name="corners"></param>
/// <returns></returns>
static int RoundRectangle(Path* path, IRECT r, float radius, Corners corners)
{
  float d = radius * 2;
  float nw = (corners & CornersNorthWest) == CornersNorthWest ? d : 0;
  float ne = (corners & CornersNorthEast) == CornersNorthEast ? d : 0;
  float se = (corners & CornersSouthEast) == CornersSouthEast ? d : 0;
  float sw = (corners & CornersSouthWest) == CornersSouthWest ? d : 0;
  PathReset(path);
  PathAddArc3(path, r.r - ne, r.t, r.r, r.t + ne, -90, 90);
  PathAddArc3(path, r.r - se, r.b - se, r.r, r.b, 0, 90);
  PathAddArc3(path, r.l, r.b - sw, r.l + sw, r.b, 90, 90);
  PathAddArc3(path, r.l, r.t, r.l + nw, r.t + nw, 180, 90);
  PathCloseFigure(path);
  return 0;
}

/// <summary>
/// Creates a rectangle with rounded corners
/// </summary>
/// <param name="r"></param>
/// <param name="radius"></param>
/// <returns></returns>
static int RoundRectangle(Path* path, IRECT r, int radius)
{
  return RoundRectangle(path, r, radius, CornersAll);
}

//Michael Spradlin - 05/03/2013 Office 2013 Style Changes
static int FlatRectangle(Path* path, IRECT r)
{
  PathReset(path);
  PathAddLine(path, r.l, r.t, r.r, r.t);
  PathAddLine(path, r.r, r.t, r.r, r.b);
  PathAddLine(path, r.r, r.b, r.l, r.b);
  PathAddLine(path, r.l, r.b, r.l, r.t);
  PathCloseFigure(path);
  return 0;
}

void LinearGradientBrush(Graphics* g, double x1, double y1, double x2, double y2, COLOR clr1, COLOR clr2) {
  gcGradientBrushS(g, x1, y1, x2, y2, clr1, clr2);
}

void LinearGradientBrush(Graphics* g, IRECT r, COLOR clr1, COLOR clr2, double angle) {
  gcGradientBrushRectAngle(g, r.l, r.t, RCW(&r), RCH(&r), clr1, clr2, angle, false);
}

/// <summary>
/// Draws a rectangle with a vertical gradient
/// </summary>
/// <param name="g"></param>
/// <param name="r"></param>
/// <param name="northColor"></param>
/// <param name="southColor"></param>
void GradientRect(Graphics* g, IRECT r, Color northColor, Color southColor)
{
  LinearGradientBrush(g, r.l, r.t - 1, r.l, r.b, northColor, southColor);
  gcFillRectangle2(g, r);
}

/// <summary>
/// Draws a shadow that indicates that the element is pressed
/// </summary>
/// <param name="g"></param>
/// <param name="r"></param>
void DrawPressedShadow(Graphics* g, IRECT r)
{
  IRECT shadow = iRECT(r.l, r.t, r.r, r.t + 4);
  Path path[1] = {0};
  RoundRectangle(path, shadow, 3, CornersNorth);
  LinearGradientBrush(g, shadow, _A_RGB(50, ColorBlack), _A_RGB(0, ColorBlack), 90);
  gcWrapMode2(g, WrapModeTileFlip);
  gcFillPath2(g, path);
  PathFree(path);
}

/// <summary>
/// Draws an arrow on the specified bounds
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
/// <param name="c"></param>
void DrawArrow(Graphics* g, IRECT b, Color c, RibbonArrowDirection d)
{
  Path path[1] = {0};
  IRECT bounds = b;
  int bWidth = RCW(&b);
  int bHeight = RCH(&b);
  float boundsWidth, boundsHeight;
  
  if (bWidth % 2 != 0 && (d == RibbonArrowDirectionUp))
    bounds = iRECT2(b.l - 1, b.t - 1, bWidth + 1, bHeight + 1);
  
  boundsWidth = RCW(&bounds);
  boundsHeight = RCH(&bounds);
  if (d == RibbonArrowDirectionUp)
  {
    PathAddLine(path, bounds.l, bounds.b, bounds.r, bounds.b);
    PathAddLine(path, bounds.r, bounds.b, bounds.l + boundsWidth / 2, bounds.t);
  }
  else if (d == RibbonArrowDirectionDown)
  {
    PathAddLine(path, bounds.l, bounds.t, bounds.r, bounds.t);
    PathAddLine(path, bounds.r, bounds.t, bounds.l + boundsWidth / 2, bounds.b);
  }
  else if (d == RibbonArrowDirectionLeft)
  {
    PathAddLine(path, bounds.l, bounds.t, bounds.r, bounds.t + boundsHeight / 2);
    PathAddLine(path, bounds.r, bounds.t + boundsHeight / 2, bounds.l, bounds.b);
  }
  else
  {
    PathAddLine(path, bounds.r, bounds.t, bounds.l, bounds.t + boundsHeight / 2);
    PathAddLine(path, bounds.l, bounds.t + boundsHeight / 2, bounds.r, bounds.b);
  }
  
  PathCloseFigure(path);
  
  gcSolidBrush(g, c);
  gcPath2(g, GcOptFill2, path);
  PathFree(path);
}

/// <summary>
/// Draws the pair of arrows that make a shadded arrow, centered on the specified bounds
/// </summary>
/// <param name="g"></param>
/// <param name="b"></param>
/// <param name="d"></param>
/// <param name="enabled"></param>
void DrawArrowShaded(Graphics* g, IRECT b, RibbonArrowDirection d, bool enabled)
{
  ISIZE arrSize = arrowSize;
  
  if (d == RibbonArrowDirectionLeft || d == RibbonArrowDirectionRight)
  {
    //Invert size
    arrSize = iSIZE(arrowSize.h, arrowSize.w);
  }
  int bWidth = RCW(&b);
  int bHeight = RCH(&b);
  IRECT bounds = iRECT2(b.l + (bWidth - arrSize.w) / 2, b.t + (bHeight - arrSize.h) / 2, arrSize.w, arrSize.h);
  IRECT boundsLight = bounds;
  boundsLight = iRectOffset2(boundsLight, 0, 1);
  
  Color lt = ThemeColorTable(ArrowLight);
  Color dk = ThemeColorTable(Arrow);
  
  if (!enabled)
  {
    lt = ColorTransparent;
    dk = ThemeColorTable(ArrowDisabled);
  }
  
  DrawArrow(g, boundsLight, lt, d);
  DrawArrow(g, bounds, dk, d);
}

/// <summary>
/// Centers the specified rectangle on the specified container
/// </summary>
/// <param name="container"></param>
/// <param name="r"></param>
/// <returns></returns>
IRECT CenterOn(IRECT container, IRECT r)
{
  IRECT result = iRECT2(
    container.l + ((RCW(&container) - RCW(&r)) / 2),
    container.t + ((RCH(&container) - RCH(&r)) / 2),
    RCW(&r), RCH(&r));
  return result;
}

/// <summary>
/// Draws a dot of the grip
/// </summary>
/// <param name="g"></param>
/// <param name="location"></param>
void DrawGripDot(Graphics* g, IPOINT location)
{
  IRECT lt = iRECT2(location.x - 1, location.y + 1, 2, 2);
  IRECT dk = iRECT2(location.x, location.y, 2, 2);
  
  gcSolidBrush(g, ThemeColorTable(DropDownGripLight));
  gcFillRectangle2(g, lt);
  gcSolidBrush(g, ThemeColorTable(DropDownGripDark));
  gcFillRectangle2(g, dk);
}


// #endregion
// #region Tab
/// <summary>
/// Creates the path of the tab and its contents
/// </summary>
/// <param name="tab"></param>
/// <returns></returns>
int CreateCompleteTabPath_2010(Path* path, IRECT TabBounds, IRECT TabContentBounds)
{
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  int corner = 6;
  PathReset(path);
  PathAddLine(path, TabBounds.l + corner, TabBounds.t, TabBounds.r - corner, TabBounds.t);
  PathAddArcR(path, iRECT(TabBounds.r - corner, TabBounds.t, TabBounds.r, TabBounds.t + corner), -90, 90);
  PathAddLine(path, TabBounds.r, TabBounds.t + corner, TabBounds.r, TabBounds.b - corner);
  PathAddArcR(path, iRECT(TabBounds.r, TabBounds.b - corner, TabBounds.r + corner, TabBounds.b), -180, -90);
  PathAddLine(path, TabBounds.r + corner, TabBounds.b, TabContentBounds.r - corner, TabBounds.b);
  PathAddArcR(path, iRECT(TabContentBounds.r - corner, TabBounds.b, TabContentBounds.r, TabBounds.b + corner), -90, 90);
  PathAddLine(path, TabContentBounds.r, TabContentBounds.t + corner, TabContentBounds.r, TabContentBounds.b - corner);
  PathAddArcR(path, iRECT(TabContentBounds.r - corner, TabContentBounds.b - corner, TabContentBounds.r, TabContentBounds.b), 0, 90);
  PathAddLine(path, TabContentBounds.r - corner, TabContentBounds.b, TabContentBounds.l + corner, TabContentBounds.b);
  PathAddArcR(path, iRECT( TabContentBounds.l, TabContentBounds.b - corner, TabContentBounds.l + corner, TabContentBounds.b), 90, 90);
  PathAddLine(path, TabContentBounds.l, TabContentBounds.b - corner, TabContentBounds.l, TabBounds.b + corner);
  PathAddArcR(path, iRECT(TabContentBounds.l, TabBounds.b, TabContentBounds.l + corner, TabBounds.b + corner), 180, 90);
  PathAddLine(path, TabContentBounds.l + corner, TabContentBounds.t, TabBounds.l - corner, TabBounds.b);
  PathAddArcR(path, iRECT(TabBounds.l - corner, TabBounds.b - corner, TabBounds.l, TabBounds.b), 90, -90);
  PathAddLine(path, TabBounds.l, TabBounds.b - corner, TabBounds.l, TabBounds.t + corner);
  PathAddArcR(path, iRECT(TabBounds.l, TabBounds.t, TabBounds.l + corner, TabBounds.t + corner), 180, 90);
  PathCloseFigure(path);
  return 0;
}

int CreateCompleteTabPath_2013(Path* path, IRECT TabBounds, IRECT TabContentBounds)
{
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  //int corner = 6;
  PathReset(path);
  PathAddLine(path, TabBounds.l, TabBounds.t, TabBounds.r, TabBounds.t);
  PathAddLine(path, TabBounds.r, TabBounds.t, TabBounds.r, TabBounds.b);
  PathAddLine(path, TabBounds.r, TabBounds.b, TabContentBounds.r, TabBounds.b);
  PathAddLine(path, TabContentBounds.r, TabContentBounds.t, TabContentBounds.r, TabContentBounds.b);
  PathAddLine(path, TabContentBounds.r, TabContentBounds.b, TabContentBounds.l, TabContentBounds.b);
  PathAddLine(path, TabContentBounds.l, TabContentBounds.b, TabContentBounds.l, TabBounds.b);
  PathAddLine(path, TabContentBounds.l, TabContentBounds.t, TabBounds.l, TabBounds.b);
  PathAddLine(path, TabBounds.l, TabBounds.b, TabBounds.l, TabBounds.t);
  PathCloseFigure(path);
  return 0;
}

/// <summary>
/// Creates the path of the tab and its contents
/// </summary>
/// <param name="tab"></param>
/// <returns></returns>
int CreateTabPath_2010(Path* path, IRECT TabBounds)
{
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  int corner = 6;
  int rightOffset = 1;
  PathReset(path);
  PathAddLine(path, TabBounds.l, TabBounds.b, TabBounds.l, TabBounds.t + corner);
  PathAddArcR(path, iRECT(TabBounds.l, TabBounds.t, corner, corner), 180, 90);
  PathAddLine(path, TabBounds.l + corner, TabBounds.t, TabBounds.r - corner - rightOffset, TabBounds.t);
  PathAddArcR(path, iRECT(TabBounds.r - corner - rightOffset, TabBounds.t, corner, corner), -90, 90);
  PathAddLine(path, TabBounds.r - rightOffset, TabBounds.t + corner, TabBounds.r - rightOffset, TabBounds.b);
  return 0;
}

int CreateTabPath_2013(Path* path, IRECT TabBounds)
{
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  int rightOffset = 1;
  PathReset(path);
  PathAddLine(path, TabBounds.l, TabBounds.b, TabBounds.l, TabBounds.t);
  PathAddLine(path, TabBounds.l, TabBounds.t, TabBounds.r - rightOffset, TabBounds.t);
  PathAddLine(path, TabBounds.r - rightOffset, TabBounds.t, TabBounds.r - rightOffset, TabBounds.b);
  return 0;
}

/// <summary>
/// Draws a complete tab
/// </summary>
/// <param name="e"></param>
void DrawTabNormal(Graphics* g, IRECT TabBounds, RibbonOrbStyle OrbStyle)
{
  IRECT clip = iRECT(TabBounds.l,TabBounds.t, TabBounds.r, TabBounds.b);
  IRECT r = iRECT(TabBounds.l - 1,TabBounds.t - 1,TabBounds.r, TabBounds.b);
  gcSetClip(g, clip);
  
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  if (OrbStyle == RibbonOrbStyleOffice_2007 || OrbStyle == RibbonOrbStyleOffice_2010) {
    gcSolidBrush(g, ThemeColorTable(RibbonBackground));
    gcFillRectangle2(g, r);
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    gcSolidBrush(g, ThemeColorTable(TabNormalBackground_2013));
    gcFillRectangle2(g, r);
  }
  gcClipReset(g);
}

/// <summary>
/// Draws an active tab
/// </summary>
/// <param name="e"></param>
//Michael Spradlin - 05/03/2013 Office 2013 Style Changes
void DrawTabActive(Graphics* g, IRECT TabBounds, RibbonOrbStyle OrbStyle, RibbonContext* Context)
{
  Path path[1] = {0};
  int TabBoundsWidth = RCW(&TabBounds);
  //DrawTabNormal(g, TabBounds, OrbStyle);
  
  if (OrbStyle == RibbonOrbStyleOffice_2010)
  {
    IRECT glossy = iRECT2(TabBounds.l, TabBounds.t, TabBoundsWidth, 4);
    IRECT shadow = iRectOffset2(TabBounds, 2, 1);
    IRECT tab = TabBounds; //tab.Inflate(0, 1);
    
    RoundRectangle(path, shadow, 6, CornersNorth);
    if (1) {
      Color Colors[] = {ColorTransparent, _A_RGB(50, ColorBlack), _A_RGB(100, ColorBlack)};
      float Positions[]  = { 0.f, 0.1f, 1.f };
      gcWrapMode2(g, WrapModeClamp);
      gcPathRadialBrush3(g, path, countof(Colors), Colors, Positions, NULL);
      gcFillPath2(g, path);
    }
    RoundRectangle(path, tab, 6, CornersNorth);
    {
      Color north = ThemeColorTable(TabNorth);
      Color south = ThemeColorTable(TabSouth);
      
      if (Context) {
        north = Context->GlowColor;
        south = _A_RGB(10, north);
      }
      
      gcPen(g, ThemeColorTable(TabNorth), 1.6f);
      gcDrawPath2(g, path);
      gcGradientBrushRectAngle2(g, TabBounds, ThemeColorTable(TabNorth), ThemeColorTable(TabSouth), 90);
      gcFillPath2(g, path);
    }
    
    RoundRectangle(path, glossy, 6, CornersNorth);
    gcSolidBrush(g, _A_RGB(180, ColorWhite));
    gcFillPath2(g, path);
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    IRECT tab = TabBounds; //tab.Inflate(0, 1);
    FlatRectangle(path, tab);
    gcPen(g, ThemeColorTable(TabActiveBorder_2013), 1.6f);
    gcDrawPath2(g, path);
    gcSolidBrush(g, ThemeColorTable(TabActiveBackbround_2013));
    gcFillPath2(g, path);
  }
  PathFree(path);
}

/// <summary>
/// Draws a complete tab
/// </summary>
/// <param name="e"></param>
//Michael Spradlin - 05/03/2013 Office 2013 Style Changes
void DrawCompleteTab(Graphics* g, IRECT TabBounds, RibbonOrbStyle OrbStyle, RibbonContext* Context, IRECT TabContentBounds, bool TabSelected)
{
  Path path[1] = {0};
  int TabContentBoundsHeight = RCH(&TabContentBounds);
  DrawTabActive(g, TabBounds, OrbStyle, Context);
  
  if (OrbStyle == RibbonOrbStyleOffice_2010)
  {
    //Background gradient
    RoundRectangle(path, TabContentBounds, 4);
    {
      Color north = ThemeColorTable(TabContentNorth);
      Color south = ThemeColorTable(TabContentSouth);
      
      if (Context)
      {
        north = ThemeColorTable(DropDownBg);
        south = north;
      }
      int tabCenter = TabContentBoundsHeight / 2;
      
      gcGradientBrushS(g, 0, TabContentBounds.t + tabCenter, 0, TabContentBounds.b - 10, north, south);
      gcWrapMode2(g, WrapModeTileFlip);
      gcFillPath2(g, path);
    }
    
    //Glossy effect
    IRECT glossy = iRECT(TabContentBounds.l, TabContentBounds.t + 0, TabContentBounds.r, TabContentBounds.t + 18);
    RoundRectangle(path, glossy, 6, CornersNorth);
    gcSolidBrush(g, _A_RGB(30, ColorWhite));
    gcFillPath2(g, path);
    //Tab border
    CreateCompleteTabPath_2010(path, TabBounds, TabContentBounds);
    
    gcPenColor(g, ThemeColorTable(TabBorder));
    gcDrawPath2(g, path);

    if (TabSelected)
    {
      //Selected glow
      CreateTabPath_2010(path, TabBounds);
      gcPen(g, _A_RGB(150, ColorGold), 2);
      gcDrawPath2(g, path);
    }
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    //Background
    FlatRectangle(path, TabContentBounds);
    
    gcSolidBrush(g, ThemeColorTable(TabCompleteBackground_2013));
    gcFillPath2(g, path);
    //Tab border
    CreateCompleteTabPath_2013(path, TabBounds, TabContentBounds);
    
    gcPenColor(g, ThemeColorTable(TabBorder_2013));
    gcDrawPath2(g, path);
  }
  PathFree(path);
}

/// <summary>
/// Draws a selected tab
/// </summary>
/// <param name="e"></param>
void DrawTabSelected(Graphics* g, IRECT TabBounds)
{
  Path path[10] = {0};
  int TabBoundsHeight = RCH(&TabBounds);
  IRECT outerR = iRECT(TabBounds.l,TabBounds.t,TabBounds.r - 1,TabBounds.b);
  IRECT innerR = iRECT(outerR.l + 1,outerR.t + 1,outerR.r - 1,outerR.b);
  IRECT glossyR = iRECT(innerR.l + 1,innerR.t + 1,innerR.r - 1,innerR.t + TabBoundsHeight / 2);
  int i=0;
  Path* outer = path + i++;
  Path* inner = path + i++;
  Path* glossy = path + i++;
  Path* radialPath = path + i++;
  RoundRectangle(outer, outerR, 3, CornersNorth);
  RoundRectangle(inner, innerR, 3, CornersNorth);
  RoundRectangle(glossy, glossyR, 3, CornersNorth);
  
  gcPenColor(g, ThemeColorTable(TabBorder));
  gcDrawPath2(g, outer);
  
  gcPenColor(g, _A_RGB(200, ColorWhite));
  gcDrawPath2(g, inner);
#if 0
  {
    PathAddRectangle2(radialPath, innerR);
    //radialPath.AddEllipse(innerR);
    PathCloseFigure(radialPath);
    
    gcRadialBrushRect2(g, innerR, );
    gr.CenterPoint = new PointF(
      Convert.ToSingle(innerR.l + innerRWidth / 2),
      Convert.ToSingle(innerR.t - 5));
    gr.CenterColor = ColorTransparent;
    gr.SurroundColors = new Color[] { ThemeColorTable.TabSelectedGlow };
    
    Blend blend = new Blend(3);
    blend.Factors = new float[] { 0.0f, 0.9f, 0.0f };
    blend.Positions = new float[] { 0.0f, 0.8f, 1.0f };
    gr.Blend = blend;
    FillPath(gr, radialPath);
    
    gr.Dispose();
  }
#endif
  {
    int innerRW = RCW(&innerR);
    int innerRH = RCH(&innerR);
    float Factors[] = { 0.0f, 0.9f, 0.0f };
    float Positions[] = { 0.0f, 0.8f, 1.0f };
    COLOR SurroundColors[] = { ColorTransparent, ThemeColorTable(TabSelectedGlow), ThemeColorTable(TabSelectedGlow) };
    gcRadialBrush(g, innerR.l + innerRW / 2, innerR.t - 5, innerRW / 2, innerRH, Positions, SurroundColors, countof(SurroundColors));
    gcFillPath2(g, inner);
  }
  gcSolidBrush(g, _A_RGB(100, ColorWhite));
  gcFillPath2(g, glossy);
  PathFrees(path, countof(path));
}

/// <summary>
/// Draws a pressed tab
/// </summary>
/// <param name="e"></param>
void DrawTabPressed(Graphics* g)
{
  
}

/// <summary>
/// Draws a selected tab
/// </summary>
/// <param name="e"></param>
//Michael Spradlin - 05/03/2013 Office 2013 Style Changes
void DrawTabMinimized(Graphics* g, IRECT TabBounds, bool TabSelected, RibbonOrbStyle OrbStyle)
{
  if (TabSelected)
  {
    if (OrbStyle == RibbonOrbStyleOffice_2010)
    {
      int TabBoundsHeight = RCH(&TabBounds);
      //background
      IRECT outerR = iRECT(TabBounds.l, TabBounds.t, TabBounds.r - 1, TabBounds.b);
      IRECT innerR = iRECT(outerR.l + 1, outerR.t + 1, outerR.r - 1, outerR.b);
      IRECT glossyR = iRECT(innerR.l + 1, innerR.t + 1, innerR.r - 1, innerR.t + TabBoundsHeight);
      Path path[10] = {0};
      int i = 0;
      Path* outer = path + i++;
      Path* inner = path + i++;
      Path* glossy = path + i++;
      Path* path2 = path + i++;

      RoundRectangle(outer, outerR, 3, CornersNorth);
      RoundRectangle(inner, innerR, 3, CornersNorth);
      RoundRectangle(glossy, glossyR, 3, CornersNorth);
      
      gcSolidBrush(g, _A_RGB(100, ColorWhite));
      gcFillPath2(g, glossy);
      
      //Tab border
      CreateTabPath_2010(path2, TabBounds);
      gcPen(g, ThemeColorTable(TabBorder), 1);
      gcDrawPath2(g, path2);

      //Selected glow
      CreateTabPath_2010(path2, TabBounds);
      gcPen(g, _RGBA(252, 184, 11, 150), 2);
      gcDrawPath2(g, path2);
      PathFrees(path, countof(path));
    }
    else if (OrbStyle == RibbonOrbStyleOffice_2013)
    {
    }
  }
  else
  {
    IRECT clip = iRECT(TabBounds.l,TabBounds.t,TabBounds.r,TabBounds.b);
    IRECT r = iRECT(TabBounds.l - 1,TabBounds.t - 1,TabBounds.r,TabBounds.b);
    gcSetClip(g, clip);
    gcSolidBrush(g, ThemeColorTable(RibbonBackground));
    gcFillRectangle2(g, r);
    gcClipReset(g);
  }
}

#if 0

// #endregion

// #region Panel
/// <summary>
/// Draws a panel in normal state (unselected)
/// </summary>
/// <param name="e"></param>
void DrawPanelNormal(Graphics* g)
{
  if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010) //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  {
    IRECT darkBorder = iRECT(e.Panel.Bounds.l,e.Panel.Bounds.t,e.Panel.Bounds.r,e.Panel.Bounds.b);
    IRECT lightBorder = iRECT(e.Panel.Bounds.l + 1,e.Panel.Bounds.t + 1,e.Panel.Bounds.r + 1,e.Panel.Bounds.b);
    IRECT textArea = iRECT(e.Panel.Bounds.l + 1,e.Panel.ContentBounds.b,e.Panel.Bounds.r - 1,e.Panel.Bounds.b - 1);    
    Path dark = RoundRectangle(darkBorder, 3);
    Path light = RoundRectangle(lightBorder, 3);
    Path txt = RoundRectangle(textArea, 3, CornersSouthEast | CornersSouthWest);
    
    using (Pen p = new Pen(ThemeColorTable.PanelLightBorder))
    {
      DrawPath(p, light);
    }
    using (Pen p = new Pen(ThemeColorTable.PanelDarkBorder))
    {
      DrawPath(p, dark);
    }
    
    using (SolidBrush b = new SolidBrush(ThemeColorTable.PanelTextBackground))
    {
      FillPath(b, txt);
    }
    
    txt.Dispose();
    dark.Dispose();
    light.Dispose();
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013) //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  {
    using (Pen p = new Pen(ThemeColorTable.PanelBorder_2013))
    {
      DrawLine(p, new IPOINT(e.Panel.Bounds.r, e.Panel.Bounds.t), new IPOINT(e.Panel.Bounds.r, e.Panel.Bounds.b));
    }
  }
  
  if (e.Panel.ButtonMoreVisible)
  {
    DrawButtonMoreGlyph(e.Graphics, e.Panel.ButtonMoreBounds, e.Panel.ButtonMoreEnabled && e.Panel.Enabled);
  }
}

/// <summary>
/// Draws a panel in selected state
/// </summary>
/// <param name="e"></param>
void DrawPanelSelected(Graphics* g)
{
  IRECT darkBorder = iRECT(
    e.Panel.Bounds.l,
    e.Panel.Bounds.t,
    e.Panel.Bounds.r,
    e.Panel.Bounds.b);
  
  IRECT lightBorder = iRECT(
    e.Panel.Bounds.l + 1,
    e.Panel.Bounds.t + 1,
    e.Panel.Bounds.r - 1,
    e.Panel.Bounds.b - 1);
  
  IRECT textArea =
    iRECT(
    e.Panel.Bounds.l + 1,
    e.Panel.ContentBounds.b,
    e.Panel.Bounds.r - 1,
    e.Panel.Bounds.b - 1);
  
  Path dark = RoundRectangle(darkBorder, 3);
  Path light = RoundRectangle(lightBorder, 3);
  Path txt = RoundRectangle(textArea, 3, CornersSouthEast | CornersSouthWest);
  
  using (Pen p = new Pen(ThemeColorTable.PanelLightBorder))
  {
    DrawPath(p, light);
  }
  
  using (Pen p = new Pen(ThemeColorTable.PanelDarkBorder))
  {
    DrawPath(p, dark);
  }
  
  using (SolidBrush b = new SolidBrush(ThemeColorTable.PanelBackgroundSelected))
  {
    FillPath(b, light);
  }
  
  using (SolidBrush b = new SolidBrush(ThemeColorTable.PanelTextBackgroundSelected))
  {
    FillPath(b, txt);
  }
  
  if (e.Panel.ButtonMoreVisible)
  {
    if (e.Panel.ButtonMorePressed)
    {
      DrawButtonPressed(e.Graphics, e.Panel.ButtonMoreBounds, CornersSouthEast, e.Ribbon);
    }
    else if (e.Panel.ButtonMoreSelected)
    {
      DrawButtonSelected(e.Graphics, e.Panel.ButtonMoreBounds, CornersSouthEast, e.Ribbon);
    }
    
    DrawButtonMoreGlyph(e.Graphics, e.Panel.ButtonMoreBounds, e.Panel.ButtonMoreEnabled && e.Panel.Enabled);
  }
  
  txt.Dispose();
  dark.Dispose();
  light.Dispose();
}

void DrawButtonMoreGlyph(Graphics* g, IRECT b, bool enabled)
{
  Color dark = enabled ? ThemeColorTable.Arrow : ThemeColorTable.ArrowDisabled;
  Color light = ThemeColorTable.ArrowLight;
  
  IRECT bounds = CenterOn(b, iRECT(IPOINT.Empty, moreSize));
  IRECT boundsLight = bounds; boundsLight.Offset(1, 1);
  
  DrawButtonMoreGlyph(g, boundsLight.Location, light);
  DrawButtonMoreGlyph(g, bounds.Location, dark);
}

void DrawButtonMoreGlyph(Graphics* gr, IPOINT p, Color color)
{
/*

  a-------b-+
  |         |
  |         d
  c     g   |
  |         |
  +----e----f
  
  */
  
  IPOINT a = p;
  IPOINT b = new IPOINT(p.X + moreSize.w - 1, p.Y);
  IPOINT c = new IPOINT(p.X, p.Y + moreSize.h - 1);
  IPOINT f = new IPOINT(p.X + moreSize.w, p.Y + moreSize.h);
  IPOINT d = new IPOINT(f.X, f.Y - 3);
  IPOINT e = new IPOINT(f.X - 3, f.Y);
  IPOINT g = new IPOINT(f.X - 3, f.Y - 3);
  
  SmoothingMode lastMode = gr.SmoothingMode;
  
  gr.SmoothingMode = SmoothingMode.None;
  
  using (Pen pen = new Pen(color))
  {
    gr.DrawLine(pen, a, b);
    gr.DrawLine(pen, a, c);
    gr.DrawLine(pen, e, f);
    gr.DrawLine(pen, d, f);
    gr.DrawLine(pen, e, d);
    gr.DrawLine(pen, g, f);
  }
  
  gr.SmoothingMode = lastMode;
}

/// <summary>
/// Draws an overflown panel in normal state
/// </summary>
/// <param name="e"></param>
void DrawPanelOverflowNormal(Graphics* g)
{
  IRECT darkBorder = iRECT(
    e.Panel.Bounds.l,
    e.Panel.Bounds.t,
    e.Panel.Bounds.r,
    e.Panel.Bounds.b);
  
  IRECT lightBorder = iRECT(
    e.Panel.Bounds.l + 1,
    e.Panel.Bounds.t + 1,
    e.Panel.Bounds.r - 1,
    e.Panel.Bounds.b - 1);
  
  
  Path dark = RoundRectangle(darkBorder, 3);
  Path light = RoundRectangle(lightBorder, 3);
  
  using (Pen p = new Pen(ThemeColorTable.PanelLightBorder))
  {
    DrawPath(p, light);
  }
  
  using (Pen p = new Pen(ThemeColorTable.PanelDarkBorder))
  {
    DrawPath(p, dark);
  }
  
  DrawPanelOverflowImage(e);
  
  dark.Dispose();
  light.Dispose();
}

/// <summary>
/// Draws an overflown panel in selected state
/// </summary>
/// <param name="e"></param>
void DrawPannelOveflowSelected(Graphics* g)
{
  IRECT darkBorder = iRECT(
    e.Panel.Bounds.l,
    e.Panel.Bounds.t,
    e.Panel.Bounds.r,
    e.Panel.Bounds.b);
  
  IRECT lightBorder = iRECT(
    e.Panel.Bounds.l + 1,
    e.Panel.Bounds.t + 1,
    e.Panel.Bounds.r - 1,
    e.Panel.Bounds.b - 1);
  
  
  Path dark = RoundRectangle(darkBorder, 3);
  Path light = RoundRectangle(lightBorder, 3);
  
  using (Pen p = new Pen(ThemeColorTable.PanelLightBorder))
  {
    DrawPath(p, light);
  }
  
  using (Pen p = new Pen(ThemeColorTable.PanelDarkBorder))
  {
    DrawPath(p, dark);
  }
  
  using (LinearGradientBrush b = new LinearGradientBrush(
    lightBorder, ThemeColorTable.PanelOverflowBackgroundSelectedNorth, ColorTransparent, 90))
  {
    FillPath(b, light);
  }
  
  DrawPanelOverflowImage(e);
  
  dark.Dispose();
  light.Dispose();
  
}

/// <summary>
/// Draws an overflown panel in pressed state
/// </summary>
/// <param name="e"></param>
void DrawPanelOverflowPressed(Graphics* g)
{
  IRECT darkBorder = iRECT(
    e.Panel.Bounds.l,
    e.Panel.Bounds.t,
    e.Panel.Bounds.r,
    e.Panel.Bounds.b);
  
  IRECT lightBorder = iRECT(
    e.Panel.Bounds.l + 1,
    e.Panel.Bounds.t + 1,
    e.Panel.Bounds.r - 1,
    e.Panel.Bounds.b - 1);
  
  IRECT glossy = iRECT(
    e.Panel.Bounds.l,
    e.Panel.Bounds.t,
    e.Panel.Bounds.r,
    e.Panel.Bounds.t + 17);
  
  
  Path dark = RoundRectangle(darkBorder, 3);
  Path light = RoundRectangle(lightBorder, 3);
  
  
  
  using (LinearGradientBrush b = new LinearGradientBrush(lightBorder,
    ThemeColorTable.PanelOverflowBackgroundPressed,
    ThemeColorTable.PanelOverflowBackgroundSelectedSouth, 90))
  {
    b.WrapMode = WrapMode.TileFlipXY;
    FillPath(b, dark);
  }
  
  using (Path path = RoundRectangle(glossy, 3, CornersNorth))
  {
    using (LinearGradientBrush b = new LinearGradientBrush(
      glossy,
      _A_RGB(150, ColorWhite),
      _A_RGB(50, ColorWhite), 90
      ))
    {
      b.WrapMode = WrapMode.TileFlipXY;
      FillPath(b, path);
    }
  }
  
  using (Pen p = new Pen(_A_RGB(40, ColorWhite)))
  {
    DrawPath(p, light);
  }
  
  using (Pen p = new Pen(ThemeColorTable.PanelDarkBorder))
  {
    DrawPath(p, dark);
  }
  
  DrawPanelOverflowImage(e);
  
  DrawPressedShadow(e.Graphics, glossy);
  
  dark.Dispose();
  light.Dispose();
}

/// <summary>
/// Draws the image of the panel when collapsed
/// </summary>
/// <param name="e"></param>
void DrawPanelOverflowImage(Graphics* g)
{
  int margin = 3;
  ISIZE imgSquareSize = new ISIZE(32, 32);
  IRECT imgSquareR = iRECT(new IPOINT(
    e.Panel.Bounds.l + (e.Panel.BoundsWidth - imgSquareSize.w) / 2,
    e.Panel.Bounds.t + 5), imgSquareSize);
  
  IRECT imgSquareBottomR = iRECT(
    imgSquareR.l, imgSquareR.b - 10, imgSquareR.r, imgSquareR.b);
  
  IRECT textR = iRECT(
    e.Panel.Bounds.l + margin,
    imgSquareR.b + margin,
    e.Panel.Bounds.r - margin,
    e.Panel.Bounds.b - margin);
  
  using (Path imgSq = RoundRectangle(imgSquareR, 5))
  {
    using (Path imgSqB = RoundRectangle(imgSquareBottomR, 5, CornersSouth))
    {
      using (LinearGradientBrush b = new LinearGradientBrush(
        imgSquareR, ThemeColorTable.TabContentNorth, ThemeColorTable.TabContentSouth, 90
        ))
      {
        FillPath(b, imgSq);
      }
      
      using (SolidBrush b = new SolidBrush(ThemeColorTable.PanelTextBackground))
      {
        FillPath(b, imgSqB);
      }
      
      using (Pen p = new Pen(ThemeColorTable.PanelDarkBorder))
      {
        DrawPath(p, imgSq);
      }
      
      if (e.Panel.Image != null)
      {
        DrawImage(e.Panel.Image,
          
          imgSquareR.l + (imgSquareRWidth - e.Panel.ImageWidth) / 2,
          imgSquareR.t + ((imgSquareRHeight - imgSquareBottomRHeight) - e.Panel.ImageHeight) / 2, e.Panel.ImageWidth, e.Panel.ImageHeight);
        
      }
      
      if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010)
      {
        using (SolidBrush b = new SolidBrush(GetTextColor(e.Panel.Enabled, ThemeColorTable.Text)))
        {
          StringFormat sf = new StringFormat();
          sf.Alignment = StringAlignment.Center;
          sf.LineAlignment = StringAlignment.Near;
          sf.Trimming = StringTrimming.Character;
          
          DrawString(e.Panel.Text, Font, b, textR, sf);
        }
      }
      else if (OrbStyle == RibbonOrbStyleOffice_2013)
      {
        using (SolidBrush b = new SolidBrush(GetTextColor(e.Panel.Enabled, ThemeColorTable.RibbonItemText_2013)))
        {
          StringFormat sf = new StringFormat();
          sf.Alignment = StringAlignment.Center;
          sf.LineAlignment = StringAlignment.Near;
          sf.Trimming = StringTrimming.Character;
          
          DrawString(e.Panel.Text, Font, b, textR, sf);
        }
      }
      
      if (e.Panel.Text != null)
      {
        IRECT bounds = LargeButtonDropDownArrowBounds(e.Graphics, e.Panel.Owner.Font, e.Panel.Text, textR);
        
        if (bounds.r < e.Panel.Bounds.r)
        {
          IRECT boundsLight = bounds; boundsLight.Offset(0, 1);
          Color lt = ThemeColorTable.ArrowLight;
          Color dk = ThemeColorTable.Arrow;
          
          DrawArrow(e.Graphics, boundsLight, lt, RibbonArrowDirectionDown);
          DrawArrow(e.Graphics, bounds, dk, RibbonArrowDirectionDown);
        }
      }
    }
  }
}

// #endregion

// #region Button

/// <summary>
/// Gets the corners to round on the specified button
/// </summary>
/// <param name="r"></param>
/// <param name="button"></param>
/// <returns></returns>
Corners ButtonCorners(RibbonButton button)
{
  if (!(button.OwnerItem is RibbonItemGroup))
  {
    return CornersAll;
  }
  else
  {
    RibbonItemGroup g = button.OwnerItem as RibbonItemGroup;
    Corners c = CornersNone;
    if (button == g.FirstItem)
    {
      c |= CornersWest;
    }
    
    if (button == g.LastItem)
    {
      c |= CornersEast;
    }
    
    return c;
  }
}

/// <summary>
/// Determines buttonface corners
/// </summary>
/// <param name="button"></param>
/// <returns></returns>
Corners ButtonFaceRounding(RibbonButton button)
{
  if (!(button.OwnerItem is RibbonItemGroup))
  {
    if (button.SizeMode == RibbonElementSizeMode.Large)
    {
      return CornersNorth;
    }
    else
    {
      return CornersWest;
    }
  }
  else
  {
    Corners c = CornersNone;
    RibbonItemGroup g = button.OwnerItem as RibbonItemGroup;
    if (button == g.FirstItem)
    {
      c |= CornersWest;
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
  if (!(button.OwnerItem is RibbonItemGroup))
  {
    if (button.SizeMode == RibbonElementSizeMode.Large)
    {
      return CornersSouth;
    }
    else
    {
      return CornersEast;
    }
  }
  else
  {
    Corners c = CornersNone;
    RibbonItemGroup g = button.OwnerItem as RibbonItemGroup;
    if (button == g.LastItem)
    {
      c |= CornersEast;
    }
    
    return c;
  }
}

/// <summary>
/// Draws the orb's option buttons background
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
void DrawOrbOptionButton(Graphics* g, IRECT bounds)
{
  boundsWidth -= 1; boundsHeight -= 1;
  
  using (Path p = RoundRectangle(bounds, 3))
  {
    using (SolidBrush b = new SolidBrush(ThemeColorTable.OrbOptionBackground))
    {
      g.FillPath(b, p);
    }
    
    GradientRect(g, iRECT(bounds.l, bounds.t + boundsHeight / 2, bounds.r, bounds.b - 2),
      ThemeColorTable.OrbOptionShine, ThemeColorTable.OrbOptionBackground);
    
    using (Pen pen = new Pen(ThemeColorTable.OrbOptionBorder))
    {
      g.DrawPath(pen, p);
    }
  }
}

/// <summary>
/// Draws a regular button in normal state 
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawButton(Graphics* g, IRECT bounds, Corners corners)
{
  if (boundsHeight <= 0 || boundsWidth <= 0) return;
  
  IRECT outerR = iRECT(
    bounds.l,
    bounds.t,
    bounds.r - 1,
    bounds.b - 1);
  
  IRECT innerR = iRECT(
    bounds.l + 1,
    bounds.t + 1,
    bounds.r - 2,
    bounds.b - 2);
  
  IRECT glossyR = iRECT(
    bounds.l + 1,
    bounds.t + 1,
    bounds.r - 2,
    bounds.t + Convert.ToInt32((double)boundsHeight * .36));
  
  using (Path boundsPath = RoundRectangle(outerR, 3, corners))
  {
    using (SolidBrush brus = new SolidBrush(ThemeColorTable.ButtonBgOut))
    {
      g.FillPath(brus, boundsPath);
    }
    
    // #region Main Bg
    using (Path path = new Path())
    {
      path.AddEllipse(iRECT(bounds.l, bounds.t, boundsWidth, boundsHeight * 2));
      PathCloseFigure(path);
      using (PathGradientBrush gradient = new PathGradientBrush(path))
      {
        gradient.WrapMode = WrapMode.Clamp;
        gradient.CenterPoint = new PointF(
          Convert.ToSingle(bounds.l + boundsWidth / 2),
          Convert.ToSingle(bounds.b));
        gradient.CenterColor = ThemeColorTable.ButtonBgCenter;
        gradient.SurroundColors = new Color[] { ThemeColorTable.ButtonBgOut };
        
        Blend blend = new Blend(3);
        blend.Factors = new float[] { 0f, 0.8f, 0f };
        blend.Positions = new float[] { 0f, 0.30f, 1f };
        
        
        Region lastClip = g.Clip;
        Region newClip = new Region(boundsPath);
        newClip.Intersect(lastClip);
        g.SetClip(newClip.GetBounds(g));
        g.FillPath(gradient, path);
        g.Clip = lastClip;
      }
    }
    // #endregion
    
    //Border
    using (Pen p = new Pen(ThemeColorTable.ButtonBorderOut))
    {
      g.DrawPath(p, boundsPath);
    }
    
    //Inner border
    using (Path path = RoundRectangle(innerR, 3, corners))
    {
      using (Pen p = new Pen(ThemeColorTable.ButtonBorderIn))
      {
        g.DrawPath(p, path);
      }
    }
    
    //Glossy effect
    using (Path path = RoundRectangle(glossyR, 3, (corners & CornersNorthWest) | (corners & CornersNorthEast)))
    {
      if (glossyRWidth > 0 && glossyRHeight > 0)
        using (LinearGradientBrush b = new LinearGradientBrush(
        glossyR, ThemeColorTable.ButtonGlossyNorth, ThemeColorTable.ButtonGlossySouth, 90))
      {
        b.WrapMode = WrapMode.TileFlipXY;
        g.FillPath(b, path);
      }
    }
  }
}

IRECT LargeButtonDropDownArrowBounds(Graphics* g, Font font, string text, IRECT textLayout)
{
  //Kevin- This function will throw an error if the text is null or empty.
  
  IRECT bounds = IRECT.Empty;
  
  bool moreWords = text.Contains(" ");
  StringFormat sf = new StringFormat();
  sf.Alignment = StringAlignment.Center;
  sf.LineAlignment = moreWords ? StringAlignment.Center : StringAlignment.Near;
  sf.Trimming = StringTrimming.EllipsisCharacter;
  
  sf.SetMeasurableCharacterRanges(new CharacterRange[] { new CharacterRange(0, text.Length) });
  Region[] regions = g.MeasureCharacterRanges(text, font, textLayout, sf);
  
  IRECT lastCharBounds = IRECT.Round(regions[regions.Length - 1].GetBounds(g));
  if (moreWords)
  {
    return iRECT(lastCharBounds.r + 3,
      lastCharBounds.t + (lastCharBoundsHeight - arrowSize.h) / 2, arrowSize.w, arrowSize.h);
  }
  else
  {
    return iRECT(
      textLayout.l + (textLayoutWidth - arrowSize.w) / 2,
      lastCharBounds.b + ((textLayout.b - lastCharBounds.b) - arrowSize.h) / 2, arrowSize.w, arrowSize.h);
  }
}

/// <summary>
/// Draws the arrow of buttons
/// </summary>
/// <param name="g"></param>
/// <param name="button"></param>
void DrawButtonDropDownArrow(Graphics* g, RibbonButton button, IRECT textLayout)
{
  IRECT bounds = IRECT.Empty;
  
  if (button.SizeMode == RibbonElementSizeMode.Large || button.SizeMode == RibbonElementSizeMode.Overflow)
  {
    
    bounds = LargeButtonDropDownArrowBounds(g, button.Owner.Font, button.Text, textLayout);
    
  }
  else
  {
    //bounds = iRECT(
    //    button.ButtonFaceBounds.r + (button.DropDownBoundsWidth - arrowSize.w) / 2,
    //    button.Bounds.t + (button.BoundsHeight - arrowSize.h) / 2,
    //    arrowSize.w, arrowSize.h);
    bounds = textLayout;
  }
  
  DrawArrowShaded(g, bounds, button.DropDownArrowDirection, button.Enabled);
}

/// <summary>
/// Draws a regular button in disabled state 
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawButtonDisabled(Graphics* g, IRECT bounds, Corners corners)
{
  if (boundsHeight <= 0 || boundsWidth <= 0) return;
  
  IRECT outerR = iRECT(
    bounds.l,
    bounds.t,
    bounds.r - 1,
    bounds.b - 1);
  
  IRECT innerR = iRECT(
    bounds.l + 1,
    bounds.t + 1,
    bounds.r - 2,
    bounds.b - 2);
  
  IRECT glossyR = iRECT(
    bounds.l + 1,
    bounds.t + 1,
    bounds.r - 2,
    bounds.t + Convert.ToInt32((double)boundsHeight * .36));
  
  using (Path boundsPath = RoundRectangle(outerR, 3, corners))
  {
    using (SolidBrush brus = new SolidBrush(ThemeColorTable.ButtonDisabledBgOut))
    {
      g.FillPath(brus, boundsPath);
    }
    
    // #region Main Bg
    using (Path path = new Path())
    {
      path.AddEllipse(iRECT(bounds.l, bounds.t, boundsWidth, boundsHeight * 2));
      PathCloseFigure(path);
      using (PathGradientBrush gradient = new PathGradientBrush(path))
      {
        gradient.WrapMode = WrapMode.Clamp;
        gradient.CenterPoint = new PointF(
          Convert.ToSingle(bounds.l + boundsWidth / 2),
          Convert.ToSingle(bounds.b));
        gradient.CenterColor = ThemeColorTable.ButtonDisabledBgCenter;
        gradient.SurroundColors = new Color[] { ThemeColorTable.ButtonDisabledBgOut };
        
        Blend blend = new Blend(3);
        blend.Factors = new float[] { 0f, 0.8f, 0f };
        blend.Positions = new float[] { 0f, 0.30f, 1f };
        
        
        Region lastClip = g.Clip;
        Region newClip = new Region(boundsPath);
        newClip.Intersect(lastClip);
        g.SetClip(newClip.GetBounds(g));
        g.FillPath(gradient, path);
        g.Clip = lastClip;
      }
    }
    // #endregion
    
    //Border
    using (Pen p = new Pen(ThemeColorTable.ButtonDisabledBorderOut))
    {
      g.DrawPath(p, boundsPath);
    }
    
    //Inner border
    using (Path path = RoundRectangle(innerR, 3, corners))
    {
      using (Pen p = new Pen(ThemeColorTable.ButtonDisabledBorderIn))
      {
        g.DrawPath(p, path);
      }
    }
    
    //Glossy effect
    using (Path path = RoundRectangle(glossyR, 3, (corners & CornersNorthWest) | (corners & CornersNorthEast)))
    {
      using (LinearGradientBrush b = new LinearGradientBrush(
        glossyR, ThemeColorTable.ButtonDisabledGlossyNorth, ThemeColorTable.ButtonDisabledGlossySouth, 90))
      {
        b.WrapMode = WrapMode.TileFlipXY;
        g.FillPath(b, path);
      }
    }
  }
}

/// <summary>
/// Draws a regular button in pressed state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawButtonPressed(Graphics* g, IRECT bounds, Corners corners, Ribbon ribbon)
{
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  if (ribbon.OrbStyle == RibbonOrbStyleOffice_2007 | ribbon.OrbStyle == RibbonOrbStyleOffice_2010)
  {
    IRECT outerR = iRECT(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
    
    using (Path boundsPath = RoundRectangle(outerR, 3, corners))
    {
      IRECT innerR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
      IRECT glossyR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + Convert.ToInt32((double)boundsHeight * .36));
      
      using (SolidBrush brus = new SolidBrush(ThemeColorTable.ButtonPressedBgOut))
      {
        g.FillPath(brus, boundsPath);
      }
      
      //Border
      using (Pen p = new Pen(ThemeColorTable.ButtonPressedBorderOut))
      {
        g.DrawPath(p, boundsPath);
      }
      
      //Inner border
      using (Path path = RoundRectangle(innerR, 3, corners))
      {
        using (Pen p = new Pen(ThemeColorTable.ButtonPressedBorderIn))
        {
          g.DrawPath(p, path);
        }
      }
      
      // #region Main Bg
      using (Path path = new Path())
      {
        path.AddEllipse(iRECT(bounds.l, bounds.t, boundsWidth, boundsHeight * 2));
        PathCloseFigure(path);
        using (PathGradientBrush gradient = new PathGradientBrush(path))
        {
          gradient.WrapMode = WrapMode.Clamp;
          gradient.CenterPoint = new PointF(
            Convert.ToSingle(bounds.l + boundsWidth / 2),
            Convert.ToSingle(bounds.b));
          gradient.CenterColor = ThemeColorTable.ButtonPressedBgCenter;
          gradient.SurroundColors = new Color[] { ThemeColorTable.ButtonPressedBgOut };
          
          Blend blend = new Blend(3);
          blend.Factors = new float[] { 0f, 0.8f, 0f };
          blend.Positions = new float[] { 0f, 0.30f, 1f };
          
          
          Region lastClip = g.Clip;
          Region newClip = new Region(boundsPath);
          newClip.Intersect(lastClip);
          g.SetClip(newClip.GetBounds(g));
          g.FillPath(gradient, path);
          g.Clip = lastClip;
        }
      }
      // #endregion
      
      //Glossy effect
      using (Path path = RoundRectangle(glossyR, 3, (corners & CornersNorthWest) | (corners & CornersNorthEast)))
      {
        using (LinearGradientBrush b = new LinearGradientBrush(glossyR, ThemeColorTable.ButtonPressedGlossyNorth, ThemeColorTable.ButtonPressedGlossySouth, 90))
        {
          b.WrapMode = WrapMode.TileFlipXY;
          g.FillPath(b, path);
        }
      }
      
      DrawPressedShadow(g, outerR);
    }
  }
  else if (ribbon.OrbStyle == RibbonOrbStyleOffice_2013)
  {
    //Flat Effect
    using (Path path = FlatRectangle(bounds))
    {
      using (SolidBrush b = new SolidBrush(ThemeColorTable.ButtonPressed_2013))
      {
        g.FillPath(b, path);
      }
    }
  }
}

/// <summary>
/// Draws a regular buttton in selected state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawButtonSelected(Graphics* g, IRECT bounds, Corners corners, Ribbon ribbon)
{
  if (boundsHeight <= 0 || boundsWidth <= 0) return;
  
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  if (ribbon.OrbStyle == RibbonOrbStyleOffice_2007 | ribbon.OrbStyle == RibbonOrbStyleOffice_2010)
  {
    IRECT outerR = iRECT(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
    
    using (Path boundsPath = RoundRectangle(outerR, 3, corners))
    {
      IRECT innerR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
      IRECT glossyR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + Convert.ToInt32((double)boundsHeight * .36));
      
      using (SolidBrush brus = new SolidBrush(ThemeColorTable.ButtonSelectedBgOut))
      {
        g.FillPath(brus, boundsPath);
      }
      
      //Border
      using (Pen p = new Pen(ThemeColorTable.ButtonSelectedBorderOut))
      {
        g.DrawPath(p, boundsPath);
      }
      
      //Inner border
      using (Path path = RoundRectangle(innerR, 3, corners))
      {
        using (Pen p = new Pen(ThemeColorTable.ButtonSelectedBorderIn))
        {
          g.DrawPath(p, path);
        }
      }
      
      // #region Main Bg
      using (Path path = new Path())
      {
        path.AddEllipse(iRECT(bounds.l, bounds.t, boundsWidth, boundsHeight * 2));
        PathCloseFigure(path);
        using (PathGradientBrush gradient = new PathGradientBrush(path))
        {
          gradient.WrapMode = WrapMode.Clamp;
          gradient.CenterPoint = new PointF(
            Convert.ToSingle(bounds.l + boundsWidth / 2),
            Convert.ToSingle(bounds.b));
          gradient.CenterColor = ThemeColorTable.ButtonSelectedBgCenter;
          gradient.SurroundColors = new Color[] { ThemeColorTable.ButtonSelectedBgOut };
          
          Blend blend = new Blend(3);
          blend.Factors = new float[] { 0f, 0.8f, 0f };
          blend.Positions = new float[] { 0f, 0.30f, 1f };
          
          
          Region lastClip = g.Clip;
          Region newClip = new Region(boundsPath);
          newClip.Intersect(lastClip);
          g.SetClip(newClip.GetBounds(g));
          g.FillPath(gradient, path);
          g.Clip = lastClip;
        }
      }
      // #endregion
      
      //Glossy effect
      using (Path path = RoundRectangle(glossyR, 3, (corners & CornersNorthWest) | (corners & CornersNorthEast)))
      {
        using (LinearGradientBrush b = new LinearGradientBrush(glossyR, ThemeColorTable.ButtonSelectedGlossyNorth, ThemeColorTable.ButtonSelectedGlossySouth, 90))
        {
          b.WrapMode = WrapMode.TileFlipXY;
          g.FillPath(b, path);
        }
      }
    }
  }
  else if (ribbon.OrbStyle == RibbonOrbStyleOffice_2013)
  {
    //Flat Effect
    using (Path path = FlatRectangle(bounds))
    {
      using (SolidBrush b = new SolidBrush(ThemeColorTable.ButtonSelected_2013))
      {
        g.FillPath(b, path);
      }
    }
  }
}

/// <summary>
/// Draws the button as pressed
/// </summary>
/// <param name="g"></param>
/// <param name="button"></param>
void DrawButtonPressed(Graphics* g, RibbonButton button, Ribbon ribbon)
{
  DrawButtonPressed(g, button.Bounds, ButtonCorners(button), ribbon);
}

/// <summary>
/// Draws the button as Checked
/// </summary>
/// <param name="g"></param>
/// <param name="button"></param>
void DrawButtonChecked(Graphics* g, RibbonButton button)
{
  DrawButtonChecked(g, button.Bounds, ButtonCorners(button));
}

/// <summary>
/// Draws the button as checked
/// </summary>
/// <param name="g"></param>
/// <param name="button"></param>
void DrawButtonChecked(Graphics* g, IRECT bounds, Corners corners)
{
  if (boundsHeight <= 0 || boundsWidth <= 0) return;
  
  IRECT outerR = iRECT(bounds.l, bounds.t, bounds.r - 1, bounds.b - 1);
  IRECT innerR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.b - 2);
  IRECT glossyR = iRECT(bounds.l + 1, bounds.t + 1, bounds.r - 2, bounds.t + Convert.ToInt32((double)boundsHeight * .36));
  
  using (Path boundsPath = RoundRectangle(outerR, 3, corners))
  {
    using (SolidBrush brus = new SolidBrush(ThemeColorTable.ButtonCheckedBgOut))
    {
      g.FillPath(brus, boundsPath);
    }
    
    // #region Main Bg
    using (Path path = new Path())
    {
      path.AddEllipse(iRECT(bounds.l, bounds.t, boundsWidth, boundsHeight * 2));
      PathCloseFigure(path);
      using (PathGradientBrush gradient = new PathGradientBrush(path))
      {
        gradient.WrapMode = WrapMode.Clamp;
        gradient.CenterPoint = new PointF(
          Convert.ToSingle(bounds.l + boundsWidth / 2),
          Convert.ToSingle(bounds.b));
        gradient.CenterColor = ThemeColorTable.ButtonCheckedBgCenter;
        gradient.SurroundColors = new Color[] { ThemeColorTable.ButtonCheckedBgOut };
        
        Blend blend = new Blend(3);
        blend.Factors = new float[] { 0f, 0.8f, 0f };
        blend.Positions = new float[] { 0f, 0.30f, 1f };
        
        
        Region lastClip = g.Clip;
        Region newClip = new Region(boundsPath);
        newClip.Intersect(lastClip);
        g.SetClip(newClip.GetBounds(g));
        g.FillPath(gradient, path);
        g.Clip = lastClip;
      }
    }
    // #endregion
    
    //Border
    using (Pen p = new Pen(ThemeColorTable.ButtonCheckedBorderOut))
    {
      g.DrawPath(p, boundsPath);
    }
    
    //Inner border
    using (Path path = RoundRectangle(innerR, 3, corners))
    {
      using (Pen p = new Pen(ThemeColorTable.ButtonCheckedBorderIn))
      {
        g.DrawPath(p, path);
      }
    }
    
    //Glossy effect
    using (Path path = RoundRectangle(glossyR, 3, (corners & CornersNorthWest) | (corners & CornersNorthEast)))
    {
      using (LinearGradientBrush b = new LinearGradientBrush(
        glossyR, ThemeColorTable.ButtonCheckedGlossyNorth, ThemeColorTable.ButtonCheckedGlossySouth, 90))
      {
        b.WrapMode = WrapMode.TileFlipXY;
        g.FillPath(b, path);
      }
    }
  }
  
  DrawPressedShadow(g, outerR);
}

/// <summary>
/// Draws the button as a selected button
/// </summary>
/// <param name="g"></param>
/// <param name="button"></param>
void DrawButtonSelected(Graphics* g, RibbonButton button, Ribbon ribbon)
{
  DrawButtonSelected(g, button.Bounds, ButtonCorners(button), ribbon);
}

/// <summary>
/// Draws a SplitDropDown button in normal state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawSplitButton(RibbonItemRenderEventArgs e, RibbonButton button)
{
}

/// <summary>
/// Draws a SplitDropDown button in pressed state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawSplitButtonPressed(RibbonItemRenderEventArgs e, RibbonButton button)
{
}

/// <summary>
/// Draws a SplitDropDown button in selected state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawSplitButtonSelected(RibbonItemRenderEventArgs e, RibbonButton button)
{
  IRECT outerR = iRECT(
    button.DropDownBounds.l,
    button.DropDownBounds.t,
    button.DropDownBounds.r - 1,
    button.DropDownBounds.b - 1);
  
  IRECT innerR = iRECT(
    outerR.l + 1,
    outerR.t + 1,
    outerR.r - 1,
    outerR.b - 1);
  
  IRECT faceOuterR = iRECT(
    button.ButtonFaceBounds.l,
    button.ButtonFaceBounds.t,
    button.ButtonFaceBounds.r - 1,
    button.ButtonFaceBounds.b - 1);
  
  IRECT faceInnerR = iRECT(
    faceOuterR.l + 1,
    faceOuterR.t + 1,
    faceOuterR.r + (button.SizeMode == RibbonElementSizeMode.Large ? -1 : 0),
    faceOuterR.b + (button.SizeMode == RibbonElementSizeMode.Large ? 0 : -1));
  
  Corners faceCorners = ButtonFaceRounding(button);
  Corners ddCorners = ButtonDdRounding(button);
  
  Path outer = RoundRectangle(outerR, 3, ddCorners);
  Path inner = RoundRectangle(innerR, 2, ddCorners);
  Path faceOuter = RoundRectangle(faceOuterR, 3, faceCorners);
  Path faceInner = RoundRectangle(faceInnerR, 2, faceCorners);
  
  using (SolidBrush b = new SolidBrush(_A_RGB(150, ColorWhite)))
  {
    FillPath(b, inner);
  }
  
  
  using (Pen p = new Pen(button.Pressed && button.SizeMode != RibbonElementSizeMode.DropDown ? ThemeColorTable.ButtonPressedBorderOut : ThemeColorTable.ButtonSelectedBorderOut))
  {
    DrawPath(p, outer);
  }
  
  using (Pen p = new Pen(button.Pressed && button.SizeMode != RibbonElementSizeMode.DropDown ? ThemeColorTable.ButtonPressedBorderIn : ThemeColorTable.ButtonSelectedBorderIn))
  {
    DrawPath(p, faceInner);
  }
  
  
  outer.Dispose(); inner.Dispose(); faceOuter.Dispose(); faceInner.Dispose();
}

/// <summary>
/// Draws a SplitDropDown button with the dropdown area pressed
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawSplitButtonDropDownPressed(RibbonItemRenderEventArgs e, RibbonButton button)
{
}

/// <summary>
/// Draws a SplitDropDown button with the dropdown area selected
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawSplitButtonDropDownSelected(RibbonItemRenderEventArgs e, RibbonButton button)
{
  IRECT outerR = iRECT(
    button.DropDownBounds.l,
    button.DropDownBounds.t,
    button.DropDownBounds.r - 1,
    button.DropDownBounds.b - 1);
  
  IRECT innerR = iRECT(
    outerR.l + 1,
    outerR.t + (button.SizeMode == RibbonElementSizeMode.Large ? 1 : 0),
    outerR.r - 1,
    outerR.b - 1);
  
  IRECT faceOuterR = iRECT(
    button.ButtonFaceBounds.l,
    button.ButtonFaceBounds.t,
    button.ButtonFaceBounds.r - 1,
    button.ButtonFaceBounds.b - 1);
  
  IRECT faceInnerR = iRECT(
    faceOuterR.l + 1,
    faceOuterR.t + 1,
    faceOuterR.r + (button.SizeMode == RibbonElementSizeMode.Large ? -1 : 0),
    faceOuterR.b + (button.SizeMode == RibbonElementSizeMode.Large ? 0 : -1));
  
  Corners faceCorners = ButtonFaceRounding(button);
  Corners ddCorners = ButtonDdRounding(button);
  
  Path outer = RoundRectangle(outerR, 3, ddCorners);
  Path inner = RoundRectangle(innerR, 2, ddCorners);
  Path faceOuter = RoundRectangle(faceOuterR, 3, faceCorners);
  Path faceInner = RoundRectangle(faceInnerR, 2, faceCorners);
  
  using (SolidBrush b = new SolidBrush(_A_RGB(150, ColorWhite)))
  {
    FillPath(b, faceInner);
  }
  
  using (Pen p = new Pen(button.Pressed && button.SizeMode != RibbonElementSizeMode.DropDown ? ThemeColorTable.ButtonPressedBorderIn : ThemeColorTable.ButtonSelectedBorderIn))
  {
    DrawPath(p, faceInner);
  }
  
  using (Pen p = new Pen(button.Pressed && button.SizeMode != RibbonElementSizeMode.DropDown ? ThemeColorTable.ButtonPressedBorderOut : ThemeColorTable.ButtonSelectedBorderOut))
  {
    DrawPath(p, faceOuter);
  }
  
  outer.Dispose(); inner.Dispose(); faceOuter.Dispose(); faceInner.Dispose();
}
// #endregion

// #region Group
/// <summary>
/// Draws the background of the specified  RibbonItemGroup
/// </summary>
/// <param name="e"></param>
/// <param name="?"></param>
void DrawItemGroup(RibbonItemRenderEventArgs e, RibbonItemGroup grp)
{
  IRECT outerR = iRECT(
    grp.Bounds.l,
    grp.Bounds.t,
    grp.Bounds.r - 1,
    grp.Bounds.b - 1);
  
  IRECT innerR = iRECT(
    outerR.l + 1,
    outerR.t + 1,
    outerR.r - 1,
    outerR.b - 1);
  
  IRECT glossyR = iRECT(
    outerR.l + 1,
    outerR.t + outerRHeight / 2 + 1,
    outerR.r - 1,
    outerR.b - 1);
  
  Path outer = RoundRectangle(outerR, 2);
  Path inner = RoundRectangle(innerR, 2);
  Path glossy = RoundRectangle(glossyR, 2);
  
  using (LinearGradientBrush b = new LinearGradientBrush(
    innerR, ThemeColorTable.ItemGroupBgNorth, ThemeColorTable.ItemGroupBgSouth, 90))
  {
    FillPath(b, inner);
  }
  
  using (LinearGradientBrush b = new LinearGradientBrush(
    glossyR, ThemeColorTable.ItemGroupBgGlossy, ColorTransparent, 90))
  {
    FillPath(b, glossy);
  }
  
  outer.Dispose();
  inner.Dispose();
}

/// <summary>
/// Draws the background of the specified  RibbonItemGroup
/// </summary>
/// <param name="e"></param>
/// <param name="?"></param>
void DrawItemGroupBorder(RibbonItemRenderEventArgs e, RibbonItemGroup grp)
{
  IRECT outerR = iRECT(
    grp.Bounds.l,
    grp.Bounds.t,
    grp.Bounds.r - 1,
    grp.Bounds.b - 1);
  
  IRECT innerR = iRECT(
    outerR.l + 1,
    outerR.t + 1,
    outerR.r - 1,
    outerR.b - 1);
  
  Path outer = RoundRectangle(outerR, 2);
  Path inner = RoundRectangle(innerR, 2);
  
  using (Pen dark = new Pen(ThemeColorTable.ItemGroupSeparatorDark))
  {
    using (Pen light = new Pen(ThemeColorTable.ItemGroupSeparatorLight))
    {
      foreach (RibbonItem item in grp.Items)
      {
        if (item == grp.LastItem) break;
        
        DrawLine(dark,
          new IPOINT(item.Bounds.r, item.Bounds.t),
          new IPOINT(item.Bounds.r, item.Bounds.b));
        
        DrawLine(light,
          new IPOINT(item.Bounds.r + 1, item.Bounds.t),
          new IPOINT(item.Bounds.r + 1, item.Bounds.b));
      }
    }
  }
  
  using (Pen p = new Pen(ThemeColorTable.ItemGroupOuterBorder))
  {
    DrawPath(p, outer);
  }
  
  using (Pen p = new Pen(ThemeColorTable.ItemGroupInnerBorder))
  {
    DrawPath(p, inner);
  }
  
  outer.Dispose();
  inner.Dispose();
}

// #endregion

// #region ButtonList

void DrawButtonList(Graphics* g, RibbonButtonList list, Ribbon ribbon)
{
  IRECT outerR = iRECT(
    list.Bounds.l,
    list.Bounds.t,
    list.Bounds.r - 1,
    list.Bounds.b);
  
  using (Path path = RoundRectangle(outerR, 3, CornersEast))
  {
    Color bgcolor = list.Selected ? ThemeColorTable.ButtonListBgSelected : ThemeColorTable.ButtonListBg;
    
    if (list.Canvas is RibbonDropDown) bgcolor = ThemeColorTable.DropDownBg;
    
    using (SolidBrush b = new SolidBrush(bgcolor))
    {
      g.FillPath(b, path);
    }
    
    using (Pen p = new Pen(ThemeColorTable.ButtonListBorder))
    {
      g.DrawPath(p, path);
    }
  }
  
  if (list.ScrollType == RibbonButtonList.ListScrollType.Scrollbar && ScrollBarRenderer.IsSupported)
  {
    
    ScrollBarRenderer.DrawUpperVerticalTrack(g, list.ScrollBarBounds, ScrollBarState.Normal);
    
    if (list.ThumbPressed)
    {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Pressed);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Pressed);
    }
    else if (list.ThumbSelected)
    {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Hot);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Hot);
    }
    else
    {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Normal);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Normal);
    }
    
    if (list.ButtonUpPressed)
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpPressed);
    }
    else if (list.ButtonUpSelected)
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpHot);
    }
    else
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpNormal);
    }
    
    if (list.ButtonDownPressed)
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownPressed);
    }
    else if (list.ButtonDownSelected)
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownHot);
    }
    else
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownNormal);
    }
  }
  else
  {
    // #region Control Buttons
    
    if (list.ScrollType == RibbonButtonList.ListScrollType.Scrollbar)
    {
      using (SolidBrush b = new SolidBrush(ThemeColorTable.ButtonGlossyNorth))
      {
        g.gcFillRectangle2(b, list.ScrollBarBounds);
      }
    }
    
    if (!list.ButtonDownEnabled)
    {
      DrawButtonDisabled(g, list.ButtonDownBounds, list.ButtonDropDownPresent ? CornersNone : CornersSouthEast);
    }
    else if (list.ButtonDownPressed)
    {
      DrawButtonPressed(g, list.ButtonDownBounds, list.ButtonDropDownPresent ? CornersNone : CornersSouthEast, ribbon);
    }
    else if (list.ButtonDownSelected)
    {
      DrawButtonSelected(g, list.ButtonDownBounds, list.ButtonDropDownPresent ? CornersNone : CornersSouthEast, ribbon);
    }
    else
    {
      DrawButton(g, list.ButtonDownBounds, CornersNone);
    }
    
    if (!list.ButtonUpEnabled)
    {
      DrawButtonDisabled(g, list.ButtonUpBounds, CornersNorthEast);
    }
    else if (list.ButtonUpPressed)
    {
      DrawButtonPressed(g, list.ButtonUpBounds, CornersNorthEast, ribbon);
    }
    else if (list.ButtonUpSelected)
    {
      DrawButtonSelected(g, list.ButtonUpBounds, CornersNorthEast, ribbon);
    }
    else
    {
      DrawButton(g, list.ButtonUpBounds, CornersNorthEast);
    }
    
    if (list.ButtonDropDownPresent)
    {
      if (list.ButtonDropDownPressed)
      {
        DrawButtonPressed(g, list.ButtonDropDownBounds, CornersSouthEast, ribbon);
      }
      else if (list.ButtonDropDownSelected)
      {
        DrawButtonSelected(g, list.ButtonDropDownBounds, CornersSouthEast, ribbon);
      }
      else
      {
        DrawButton(g, list.ButtonDropDownBounds, CornersSouthEast);
      }
    }
    
    if (list.ScrollType == RibbonButtonList.ListScrollType.Scrollbar && list.ScrollBarEnabled)
    {
      if (list.ThumbPressed)
      {
        DrawButtonPressed(g, list.ThumbBounds, CornersAll, ribbon);
      }
      else if (list.ThumbSelected)
      {
        DrawButtonSelected(g, list.ThumbBounds, CornersAll, ribbon);
      }
      else
      {
        DrawButton(g, list.ThumbBounds, CornersAll);
      }
      
    }
    
    Color dk = ThemeColorTable.Arrow;
    Color lt = ThemeColorTable.ArrowLight;
    Color ds = ThemeColorTable.ArrowDisabled;
    
    IRECT arrUp = CenterOn(list.ButtonUpBounds, iRECT(IPOINT.Empty, arrowSize)); arrUp.Offset(0, 1);
    IRECT arrD = CenterOn(list.ButtonDownBounds, iRECT(IPOINT.Empty, arrowSize)); arrD.Offset(0, 1);
    IRECT arrdd = CenterOn(list.ButtonDropDownBounds, iRECT(IPOINT.Empty, arrowSize)); arrdd.Offset(0, 3);
    
    DrawArrow(g, arrUp, list.ButtonUpEnabled ? lt : ColorTransparent, RibbonArrowDirectionUp); arrUp.Offset(0, -1);
    DrawArrow(g, arrUp, list.ButtonUpEnabled ? dk : ds, RibbonArrowDirectionUp);
    
    DrawArrow(g, arrD, list.ButtonDownEnabled ? lt : ColorTransparent, RibbonArrowDirectionDown); arrD.Offset(0, -1);
    DrawArrow(g, arrD, list.ButtonDownEnabled ? dk : ds, RibbonArrowDirectionDown);
    
    if (list.ButtonDropDownPresent)
    {
      
      
      using (SolidBrush b = new SolidBrush(ThemeColorTable.Arrow))
      {
        SmoothingMode sm = g.SmoothingMode;
        g.SmoothingMode = SmoothingMode.None;
        g.gcFillRectangle2(b, iRECT(new IPOINT(arrdd.l - 1, arrdd.t - 4), new ISIZE(arrowSize.w + 2, 1)));
        g.SmoothingMode = sm;
      }
      
      DrawArrow(g, arrdd, lt, RibbonArrowDirectionDown); arrdd.Offset(0, -1);
      DrawArrow(g, arrdd, dk, RibbonArrowDirectionDown);
    }
    // #endregion
  }
}

// #endregion

// #region Separator

void DrawSeparator(Graphics* g, RibbonSeparator separator)
{
  if (separator.SizeMode == RibbonElementSizeMode.DropDown)
  {
    if (!string.IsNullOrEmpty(separator.Text))
    {
      using (SolidBrush b = new SolidBrush(ThemeColorTable.SeparatorBg))
      {
        g.gcFillRectangle2(b, separator.Bounds);
      }
      
      using (Pen p = new Pen(ThemeColorTable.SeparatorLine))
      {
        g.DrawLine(p,
          new IPOINT(separator.Bounds.l, separator.Bounds.b),
          new IPOINT(separator.Bounds.r, separator.Bounds.b));
      }
    }
    else
    {
      using (Pen p = new Pen(ThemeColorTable.DropDownImageSeparator))
      {
        g.DrawLine(p,
          new IPOINT(separator.Bounds.l + 30, separator.Bounds.t + 1),
          new IPOINT(separator.Bounds.r, separator.Bounds.t + 1));
      }
    }
  }
  else
  {
    using (Pen p = new Pen(ThemeColorTable.SeparatorDark))
    {
      g.DrawLine(p,
        new IPOINT(separator.Bounds.l, separator.Bounds.t),
        new IPOINT(separator.Bounds.l, separator.Bounds.b));
    }
    
    using (Pen p = new Pen(ThemeColorTable.SeparatorLight))
    {
      g.DrawLine(p,
        new IPOINT(separator.Bounds.l + 1, separator.Bounds.t),
        new IPOINT(separator.Bounds.l + 1, separator.Bounds.b));
    }
  }
}

// #endregion

// #region TextBox

/// <summary>
/// Draws a disabled textbox
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
void DrawTextBoxDisabled(Graphics* g, IRECT bounds)
{
  
  using (SolidBrush b = new SolidBrush(SystemColors.Control))
  {
    g.gcFillRectangle2(b, bounds);
  }
  
  using (Pen p = new Pen(ThemeColorTable.TextBoxBorder))
  {
    g.DrawRectangle(p, bounds);
  }
  
}

/// <summary>
/// Draws an unselected textbox
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
void DrawTextBoxUnselected(Graphics* g, IRECT bounds)
{
  
  using (SolidBrush b = new SolidBrush(ThemeColorTable.TextBoxUnselectedBg))
  {
    g.gcFillRectangle2(b, bounds);
  }
  
  using (Pen p = new Pen(ThemeColorTable.TextBoxBorder))
  {
    g.DrawRectangle(p, bounds);
  }
  
}

/// <summary>
/// Draws an unselected textbox
/// </summary>
/// <param name="g"></param>
/// <param name="bounds"></param>
void DrawTextBoxSelected(Graphics* g, IRECT bounds)
{
  using (Path path = RoundRectangle(bounds, 3))
  {
    using (SolidBrush b = new SolidBrush(SystemColors.Window))
    {
      //g.FillPath(b, path);
      g.gcFillRectangle2(b, bounds);
    }
    
    using (Pen p = new Pen(ThemeColorTable.TextBoxBorder))
    {
      //g.DrawPath(p, path);
      g.DrawRectangle(p, bounds);
    }
  }
}


// #endregion

// #region ComboBox

void DrawComboxDropDown(Graphics* g, RibbonComboBox b, Ribbon ribbon)
{
  if (b.DropDownButtonPressed)
  {
    DrawButtonPressed(g, b.DropDownButtonBounds, CornersNone, ribbon);
  }
  else if (b.DropDownButtonSelected)
  {
    DrawButtonSelected(g, b.DropDownButtonBounds, CornersNone, ribbon);
  }
  else if (b.Selected)
  {
    DrawButton(g, b.DropDownButtonBounds, CornersNone);
  }
  
  DrawArrowShaded(g, b.DropDownButtonBounds, RibbonArrowDirectionDown, true);//b.Enabled);
}

void DrawUpDownButtons(Graphics* g, RibbonUpDown b, Ribbon ribbon)
{
  if (b.UpButtonPressed)
    DrawButtonPressed(g, b.UpButtonBounds, CornersNone, ribbon);
  else if (b.UpButtonSelected)
    DrawButtonSelected(g, b.UpButtonBounds, CornersNone, ribbon);
  else
    DrawButton(g, b.UpButtonBounds, CornersNone);
  
  if (b.DownButtonPressed)
    DrawButtonPressed(g, b.DownButtonBounds, CornersNone, ribbon);
  else if (b.DownButtonSelected)
    DrawButtonSelected(g, b.DownButtonBounds, CornersNone, ribbon);
  else
    DrawButton(g, b.DownButtonBounds, CornersNone);
  
  DrawArrowShaded(g, b.UpButtonBounds, RibbonArrowDirectionUp, true);
  DrawArrowShaded(g, b.DownButtonBounds, RibbonArrowDirectionDown, true);
}
// #endregion

// #region Quick Access and Caption Bar

void DrawCaptionBarBackground(IRECT r, Graphics g)
{
  
  SmoothingMode smbuff = g.SmoothingMode;
  IRECT r1 = iRECT(r.l, r.t, rWidth, 4);
  IRECT r2 = iRECT(r.l, r1.b, rWidth, 4);
  IRECT r3 = iRECT(r.l, r2.b, rWidth, rHeight - 8);
  IRECT r4 = iRECT(r.l, r3.b, rWidth, 1);
  
  IRECT[] rects = iRECT[] { r1, r2, r3, r4 };
  Color[,] colors = new Color[,] { 
    { ThemeColorTable.Caption1, ThemeColorTable.Caption2 },
    { ThemeColorTable.Caption3, ThemeColorTable.Caption4 },
    { ThemeColorTable.Caption5, ThemeColorTable.Caption6 },
    { ThemeColorTable.Caption7, ThemeColorTable.Caption7 }
  };
  
  g.SmoothingMode = SmoothingMode.None;
  
  for (int i = 0; i < rects.Length; i++)
  {
    IRECT grect = rects[i]; grectHeight += 2; grect.Y -= 1;
    using (LinearGradientBrush b = new LinearGradientBrush(grect, colors[i, 0], colors[i, 1], 90))
    {
      g.gcFillRectangle2(b, rects[i]);
    }
  }
  
  g.SmoothingMode = smbuff;
}

void DrawCaptionBarText(IRECT captionBar, RibbonRenderEventArgs e)
{
  Form f = FindForm();
  
  if (f == null)
    return;
  
  StringFormat sf = new StringFormat(); sf.LineAlignment = sf.Alignment = StringAlignment.Center;
  sf.Trimming = StringTrimming.EllipsisCharacter; sf.FormatFlags |= StringFormatFlags.NoWrap;
  Font ft = new Font(SystemFonts.CaptionFont, FontStyle.Regular);
  
  
  if (ActualBorderMode == RibbonWindowMode.NonClientAreaGlass)
  {
    WinApi.DrawTextOnGlass(e.Graphics, f.Text, SystemFonts.CaptionFont, captionBar, 10);
  }
  else if (ActualBorderMode == RibbonWindowMode.NonClientAreaCustomDrawn)
  {
    TextRenderer.DrawText(e.Graphics, f.Text, ft, captionBar, ThemeColorTable.FormBorder);
    
  }
  //Console.WriteLine("capt " + DateTime.Now.Millisecond + e.ClipRectangle.ToString());
  //WinApi.FillForGlass(e.Graphics, captionBar);
  //WinApi.DrawTextOnGlass(Handle, f.Text, SystemFonts.CaptionFont, captionBar, 10);
}

Path CreateQuickAccessPath(IPOINT a, IPOINT b, IPOINT c, IPOINT d, IPOINT e, IRECT bounds, int offsetx, int offsety, Ribbon ribbon)
{
  a.Offset(offsetx, offsety); b.Offset(offsetx, offsety); c.Offset(offsetx, offsety);
  d.Offset(offsetx, offsety); e.Offset(offsetx, offsety);
  
  
  if (ribbon.rToLeft == RightToLeft.No)
  {
    PathAddLine(path, a, b);
    PathAddArcR(path, iRECT(b.X - boundsHeight / 2, b.Y, boundsHeight, boundsHeight), -90, 180);
    PathAddLine(path, d, c);
    if (ribbon.OrbVisible)
    {
      path.AddCurve(new IPOINT[] { c, e, a });
    }
    else
    {
      PathAddArcR(path, iRECT(a.X - boundsHeight / 2, a.Y, boundsHeight, boundsHeight), 90, 180);
    }
  }
  else
  {
    ///   a-----b    a-----b
    ///  |     z    |       z
    ///   c---d      c-----d
    PathAddLine(path, d, c);
    PathAddArcR(path, iRECT(a.X - boundsHeight / 2, a.Y, boundsHeight, boundsHeight), 90, 180);
    PathAddLine(path, a, b);
    if (ribbon.OrbVisible)
    {
      path.AddCurve(new IPOINT[] { b, e, d });
    }
    else
    {
      PathAddArcR(path, iRECT(b.X - boundsHeight / 2, b.Y, boundsHeight, boundsHeight), -90, 180);
    }
  }
  return 0;
}

// #endregion

// #region Ribbon Orb
/// <summary>
/// Draws the orb on the specified state
/// </summary>
/// <param name="g">Device to draw</param>
/// <param name="ribbon">Ribbon that the orb belongs to</param>
/// <param name="r">Layout rectangle for the orb</param>
/// <param name="selected">Specifies if the orb should be drawn as selected</param>
/// <param name="pressed">Specifies if the orb should be drawn as pressed</param>
void DrawOrb(Graphics* g, IRECT r, Image image, bool selected, bool pressed)
{
  int sweep, start;
  IPOINT p1, p2, p3;
  Color bgdark, bgmed, bglight, light;
  IRECT rinner = r; rinner.Inflate(-1, -1);
  IRECT shadow = r; shadow.Offset(1, 1); shadow.Inflate(2, 2);
  
  // #region Color Selection
  
  if (pressed)
  {
    bgdark = ThemeColorTable.OrbPressedBackgroundDark;
    bgmed = ThemeColorTable.OrbPressedBackgroundMedium;
    bglight = ThemeColorTable.OrbPressedBackgroundLight;
    light = ThemeColorTable.OrbPressedLight;
  }
  else if (selected)
  {
    bgdark = ThemeColorTable.OrbSelectedBackgroundDark;
    bgmed = ThemeColorTable.OrbSelectedBackgroundDark;
    bglight = ThemeColorTable.OrbSelectedBackgroundLight;
    light = ThemeColorTable.OrbSelectedLight;
  }
  else
  {
    bgdark = ThemeColorTable.OrbBackgroundDark;
    bgmed = ThemeColorTable.OrbBackgroundMedium;
    bglight = ThemeColorTable.OrbBackgroundLight;
    light = ThemeColorTable.OrbLight;
  }
  
  // #endregion
  
  // #region Shadow
  
  using (Path p = new Path())
  {
    p.AddEllipse(shadow);
    
    using (PathGradientBrush gradient = new PathGradientBrush(p))
    {
      gradient.WrapMode = WrapMode.Clamp;
      gradient.CenterPoint = new PointF(shadow.l + shadowWidth / 2, shadow.t + shadowHeight / 2);
      gradient.CenterColor = _A_RGB(180, ColorBlack);
      gradient.SurroundColors = new Color[] { ColorTransparent };
      
      Blend blend = new Blend(3);
      blend.Factors = new float[] { 0f, 1f, 1f };
      blend.Positions = new float[] { 0, 0.2f, 1f };
      gradient.Blend = blend;
      
      g.FillPath(gradient, p);
    }
    
  }
  
  
  
  // #endregion
  
  // #region Orb Background
  
  using (Pen p = new Pen(bgdark, 1))
  {
    g.DrawEllipse(p, r);
  }
  
  using (Path p = new Path())
  {
    p.AddEllipse(r);
    using (PathGradientBrush gradient = new PathGradientBrush(p))
    {
      gradient.WrapMode = WrapMode.Clamp;
      gradient.CenterPoint = new PointF(Convert.ToSingle(r.l + rWidth / 2), Convert.ToSingle(r.b));
      gradient.CenterColor = bglight;
      gradient.SurroundColors = new Color[] { bgmed };
      
      Blend blend = new Blend(3);
      blend.Factors = new float[] { 0f, .8f, 1f };
      blend.Positions = new float[] { 0, 0.50f, 1f };
      gradient.Blend = blend;
      
      
      g.FillPath(gradient, p);
    }
  }
  
  // #endregion
  
  // #region Bottom round shine
  
  IRECT bshine = iRECT(0, 0, rWidth / 2, rHeight / 2);
  bshine.X = r.X + (rWidth - bshineWidth) / 2;
  bshine.Y = r.Y + rHeight / 2;
  
  
  
  using (Path p = new Path())
  {
    p.AddEllipse(bshine);
    
    using (PathGradientBrush gradient = new PathGradientBrush(p))
    {
      gradient.WrapMode = WrapMode.Clamp;
      gradient.CenterPoint = new PointF(Convert.ToSingle(r.l + rWidth / 2), Convert.ToSingle(r.b));
      gradient.CenterColor = ColorWhite;
      gradient.SurroundColors = new Color[] { ColorTransparent };
      
      g.FillPath(gradient, p);
    }
  }
  
  
  
  // #endregion
  
  // #region Upper Glossy
  using (Path p = new Path())
  {
    sweep = 160;
    start = 180 + (180 - sweep) / 2;
    p.AddArc(rinner, start, sweep);
    
    p1 = IPOINT.Round(p.PathData.Points[0]);
    p2 = IPOINT.Round(p.PathData.Points[p.PathData.Points.Length - 1]);
    p3 = new IPOINT(rinner.l + rinnerWidth / 2, p2.Y - 3);
    p.AddCurve(new IPOINT[] { p2, p3, p1 });
    
    using (PathGradientBrush gradient = new PathGradientBrush(p))
    {
      gradient.WrapMode = WrapMode.Clamp;
      gradient.CenterPoint = p3;
      gradient.CenterColor = ColorTransparent;
      gradient.SurroundColors = new Color[] { light };
      
      Blend blend = new Blend(3);
      blend.Factors = new float[] { .3f, .8f, 1f };
      blend.Positions = new float[] { 0, 0.50f, 1f };
      gradient.Blend = blend;
      
      g.FillPath(gradient, p);
    }
    
    using (LinearGradientBrush b = new LinearGradientBrush(new IPOINT(r.l, r.t), new IPOINT(r.l, p1.Y), ColorWhite, ColorTransparent))
    {
      Blend blend = new Blend(4);
      blend.Factors = new float[] { 0f, .4f, .8f, 1f };
      blend.Positions = new float[] { 0f, .3f, .4f, 1f };
      b.Blend = blend;
      g.FillPath(b, p);
    }
  }
  // #endregion
  
  // #region Upper Shine
  /////Lower gloss
  //using (Path p = new Path())
  //{
  //    sweep = 140;
  //    start = (180 - sweep) / 2;
  //    p.AddArc(rinner, start, sweep);
  
  //    p1 = IPOINT.Round(p.PathData.Points[0]);
  //    p2 = IPOINT.Round(p.PathData.Points[p.PathData.Points.Length - 1]);
  //    p3 = new IPOINT(rinner.l + rinnerWidth / 2, p1.Y + 3);
  //    p.AddCurve(new IPOINT[] { p2, p3, p1 });
  
  //    g.FillPath(Brushes.White, p);
  //}
  
  ///Upper shine
  using (Path p = new Path())
  {
    sweep = 160;
    start = 180 + (180 - sweep) / 2;
    p.AddArc(rinner, start, sweep);
    
    using (Pen pen = new Pen(ColorWhite))
    {
      g.DrawPath(pen, p);
    }
  }
  // #endregion
  
  // #region Lower Shine
  using (Path p = new Path())
  {
    sweep = 160;
    start = (180 - sweep) / 2;
    p.AddArc(rinner, start, sweep);
    IPOINT pt = IPOINT.Round(p.PathData.Points[0]);
    
    IRECT rrinner = rinner; rrinner.Inflate(-1, -1);
    sweep = 160;
    start = (180 - sweep) / 2;
    p.AddArc(rrinner, start, sweep);
    
    using (LinearGradientBrush b = new LinearGradientBrush(
      new IPOINT(rinner.l, rinner.b),
      new IPOINT(rinner.l, pt.Y - 1),
      light, _A_RGB(50, light)))
    {
      g.FillPath(b, p);
    }
    
    //p1 = IPOINT.Round(p.PathData.Points[0]);
    //p2 = IPOINT.Round(p.PathData.Points[p.PathData.Points.Length - 1]);
    //p3 = new IPOINT(rinner.l + rinnerWidth / 2, rinner.b - 1);
    //p.AddCurve(new IPOINT[] { p2, p3, p1 });
    
    //using (LinearGradientBrush b = new LinearGradientBrush(
    //    new IPOINT(rinner.l, rinner.b + 1),
    //    new IPOINT(rinner.l, p1.Y),
    //    _A_RGB(200, ColorWhite), ColorTransparent))
    //{
    //    g.FillPath(b, p);
    //}
  }
  
  // #endregion
  
  // #region Orb Icon
  
  if (image != null)
  {
    IRECT irect = iRECT(IPOINT.Empty, image.ISIZE);
    irect.X = r.X + (rWidth - irectWidth) / 2;
    irect.Y = r.Y + (rHeight - irectHeight) / 2;
    g.DrawImage(image, irect);
  }
  
  // #endregion
  
}

/// <summary>
/// Draws the orb button in a normal state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawOrbNormal(RibbonRenderEventArgs e)
{
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  if (OrbStyle == RibbonOrbStyleOffice_2010)
  {
    using (Path path = RoundRectangle(e.ClipRectangle, 2, CornersNorth))
    {
      FillPath(new SolidBrush(ThemeColorTable.OrbButtonBackground), path);
      
      //Border
      using (Pen pOut = new Pen(ThemeColorTable.OrbButtonBorderDark))
      {
        DrawPath(pOut, path);
      }
      
      //Inner border
      IRECT innerR = iRECT(e.ClipRectangle.l + 1, e.ClipRectangle.t + 1, e.ClipRectangle.r - 1, e.ClipRectangle.b);
      
      using (Path inpath = RoundRectangle(innerR, 2, CornersNorth))
      {
        using (Pen pIn = new Pen(ThemeColorTable.OrbButtonMedium))
        {
          DrawPath(pIn, inpath);
        }
      }
      
      int intCenter = e.ClipRectangleHeight / 2;
      
      IRECT rec = iRECT(e.ClipRectangle.l + 1, e.ClipRectangle.t + intCenter, e.ClipRectangle.r - 2, e.ClipRectangle.b - 1);
      
      Color north = ThemeColorTable.OrbButtonDark;
      Color south = ThemeColorTable.OrbButtonLight;
      
      using (LinearGradientBrush b = new LinearGradientBrush(new IPOINT(0, e.ClipRectangle.t + intCenter), new IPOINT(0, e.ClipRectangle.b), north, south))
      {
        b.WrapMode = WrapMode.TileFlipXY;
        gcFillRectangle2(b, rec);
      }
    }
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    using (Path path = FlatRectangle(e.ClipRectangle))
    {
      FillPath(new SolidBrush(ThemeColorTable.OrbButton_2013), path);
    }
  }
}

/// <summary>
/// Draws the orb button in a selected state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawOrbSelected(RibbonRenderEventArgs e)
{
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  if (OrbStyle == RibbonOrbStyleOffice_2010)
  {
    using (Path path = RoundRectangle(e.ClipRectangle, 2, CornersNorth))
    {
      FillPath(new SolidBrush(ThemeColorTable.ButtonPressedGlossySouth), path);
      
      //Border
      using (Pen p = new Pen(ThemeColorTable.ButtonPressedBorderOut))
      {
        DrawPath(p, path);
      }
      
      //Inner border
      IRECT innerR = iRECT(e.ClipRectangle.l + 1, e.ClipRectangle.t + 1, e.ClipRectangle.r - 1, e.ClipRectangle.b);
      
      using (Path inpath = RoundRectangle(innerR, 2, CornersNorth))
      {
        using (Pen p = new Pen(ThemeColorTable.ButtonPressedBorderIn))
        {
          DrawPath(p, inpath);
        }
      }
      
      Color north = ThemeColorTable.ButtonSelectedGlossyNorth;
      Color south = ThemeColorTable.ButtonSelectedGlossySouth;
      
      int intCenter = e.ClipRectangleHeight / 2;
      IRECT rec = iRECT(e.ClipRectangle.l + 1, e.ClipRectangle.t + intCenter, e.ClipRectangle.r - 2, e.ClipRectangle.b - 1);
      
      using (LinearGradientBrush b = new LinearGradientBrush(new IPOINT(0, e.ClipRectangle.t + intCenter), new IPOINT(0, e.ClipRectangle.b), north, south))
      {
        b.WrapMode = WrapMode.TileFlipXY;
        gcFillRectangle2(b, rec);
      }
    }
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    using (Path path = FlatRectangle(e.ClipRectangle))
    {
      FillPath(new SolidBrush(ThemeColorTable.OrbButtonSelected_2013), path);
    }
  }
}

/// <summary>
/// Draws the orb button in pressed state
/// </summary>
/// <param name="e"></param>
/// <param name="button"></param>
void DrawOrbPressed(RibbonRenderEventArgs e)
{
  //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  if (OrbStyle == RibbonOrbStyleOffice_2010)
  {
    using (Path path = RoundRectangle(e.ClipRectangle, 2, CornersNorth))
    {
      FillPath(new SolidBrush(ThemeColorTable.ButtonPressedGlossySouth), path);
      
      //Border
      using (Pen p = new Pen(ThemeColorTable.ButtonPressedBorderOut))
      {
        DrawPath(p, path);
      }
      
      //Inner border
      IRECT innerR = iRECT(e.ClipRectangle.l + 1, e.ClipRectangle.t + 1, e.ClipRectangle.r - 1, e.ClipRectangle.b);
      
      using (Path inpath = RoundRectangle(innerR, 2, CornersNorth))
      {
        using (Pen p = new Pen(ThemeColorTable.ButtonPressedBorderIn))
        {
          DrawPath(p, inpath);
        }
      }
      
      Color north = ThemeColorTable.ButtonPressedGlossyNorth;
      Color south = ThemeColorTable.ButtonPressedGlossySouth;
      
      int intCenter = e.ClipRectangleHeight / 2;
      IRECT rec = iRECT(e.ClipRectangle.l + 1, e.ClipRectangle.t + intCenter, e.ClipRectangle.r - 2, e.ClipRectangle.b - 1);
      
      using (LinearGradientBrush b = new LinearGradientBrush(new IPOINT(0, e.ClipRectangle.t + intCenter), new IPOINT(0, e.ClipRectangle.b), north, south))
      {
        b.WrapMode = WrapMode.TileFlipXY;
        gcFillRectangle2(b, rec);
      }
    }
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    using (Path path = FlatRectangle(e.ClipRectangle))
    {
      FillPath(new SolidBrush(ThemeColorTable.OrbButtonPressed_2013), path);
    }
  }
}
// #endregion

// #endregion

// #region Overrides
void OnRenderRibbonCaptionBar(RibbonRenderEventArgs e)
{
  if (CaptionBarVisible)
  {
    IRECT captionBar = iRECT(0, 0, e.RibbonWidth, CaptionBarSize);
    if (!(ActualBorderMode == RibbonWindowMode.NonClientAreaGlass && RibbonDesigner.Current == null))
    {
      //DrawCaptionBarBackground(captionBar, e.Graphics);
    }
    DrawCaptionBarText(CaptionTextBounds, e);
  }
}

void OnRenderOrbDropDownBackground(RibbonOrbDropDownEventArgs e)
{
  int Width = e.RibbonOrbDropDownWidth;
  int Height = e.RibbonOrbDropDownHeight;
  
  IRECT OrbDDContent = e.RibbonOrbDropDown.ContentBounds;
  IRECT Bcontent = e.RibbonOrbDropDown.ContentButtonsBounds;
  IRECT OuterRect = iRECT(0, 0, Width - 1, Height - 1);
  IRECT InnerRect = iRECT(1, 1, Width - 3, Height - 3);
  IRECT NorthNorthRect = iRECT(1, 1, Width - 3, OrbDDContent.t / 2);
  IRECT northSouthRect = iRECT(1, NorthNorthRect.b, NorthNorthRectWidth, OrbDDContent.t / 2);
  IRECT southSouthRect = iRECT(1,
    (Height - OrbDDContent.b) / 2 + OrbDDContent.b, Width - 1, Height - 1);
  
  Color OrbDropDownDarkBorder = ThemeColorTable.OrbDropDownDarkBorder;
  Color OrbDropDownLightBorder = ThemeColorTable.OrbDropDownLightBorder;
  Color OrbDropDownBack = ThemeColorTable.OrbDropDownBack;
  Color OrbDropDownNorthA = ThemeColorTable.OrbDropDownNorthA;
  Color OrbDropDownNorthB = ThemeColorTable.OrbDropDownNorthB;
  Color OrbDropDownNorthC = ThemeColorTable.OrbDropDownNorthC;
  Color OrbDropDownNorthD = ThemeColorTable.OrbDropDownNorthD;
  Color OrbDropDownSouthC = ThemeColorTable.OrbDropDownSouthC;
  Color OrbDropDownSouthD = ThemeColorTable.OrbDropDownSouthD;
  Color OrbDropDownContentbg = ThemeColorTable.OrbDropDownContentbg;
  Color OrbDropDownContentbglight = ThemeColorTable.OrbDropDownContentbglight;
  Color OrbDropDownSeparatorlight = ThemeColorTable.OrbDropDownSeparatorlight;
  Color OrbDropDownSeparatordark = ThemeColorTable.OrbDropDownSeparatordark;
  
  Path innerPath = RoundRectangle(InnerRect, 6);
  Path outerPath = RoundRectangle(OuterRect, 6);
  
  SmoothingMode = SmoothingMode.None;
  
  using (Brush b = new SolidBrush(_A_RGB(0x8e, 0x8e, 0x8e)))
  {
    gcFillRectangle2(b, iRECT(Width - 10, Height - 10, 10, 10));
  }
  
  using (Brush b = new SolidBrush(OrbDropDownBack))
  {
    FillPath(b, outerPath);
  }
  
  GradientRect(e.Graphics, NorthNorthRect, OrbDropDownNorthA, OrbDropDownNorthB);
  GradientRect(e.Graphics, northSouthRect, OrbDropDownNorthC, OrbDropDownNorthD);
  GradientRect(e.Graphics, southSouthRect, OrbDropDownSouthC, OrbDropDownSouthD);
  
  using (Pen p = new Pen(OrbDropDownDarkBorder))
  {
    DrawPath(p, outerPath);
  }
  
  SmoothingMode = SmoothingMode.AntiAlias;
  
  using (Pen p = new Pen(OrbDropDownLightBorder))
  {
    DrawPath(p, innerPath);
  }
  
  innerPath.Dispose();
  outerPath.Dispose();
  
  // #region Content
  InnerRect = OrbDDContent; InnerRect.Inflate(0, 0);
  OuterRect = OrbDDContent; OuterRect.Inflate(1, 1);
  
  using (SolidBrush b = new SolidBrush(OrbDropDownContentbg))
  {
    gcFillRectangle2(b, OrbDDContent);
  }
  
  //Steve - Recent Items Caption
  if (e.RibbonOrbDropDown.ContentRecentItemsCaptionBoundsHeight > 0)
  {
    IRECT cb = e.RibbonOrbDropDown.ContentRecentItemsCaptionBounds;
    
    //draw the lines first since we need to adjust the bounds for the text portion
    int linePos = Convert.ToInt32(e.RibbonOrbDropDown.RecentItemsCaptionLineSpacing / 2);
    
    using (Pen p = new Pen(OrbDropDownSeparatorlight))
    {
      DrawLine(p,
        new IPOINT(OrbDDContent.l, cb.b - linePos),
        new IPOINT(OrbDDContent.r, cb.b - linePos));
    }
    using (Pen p = new Pen(OrbDropDownSeparatordark))
    {
      DrawLine(p,
        new IPOINT(OrbDDContent.l, cb.b - linePos - 1),
        new IPOINT(OrbDDContent.r, cb.b - linePos - 1));
    }
    
    //adjust the bounds for the text margins and line height
    cb.X += ItemMargin.l;
    cbWidth -= (ItemMargin.l + ItemMargin.r);
    cbHeight -= e.RibbonOrbDropDown.RecentItemsCaptionLineSpacing;
    
    StringFormat sf = new StringFormat();
    sf.LineAlignment = StringAlignment.Center;
    if (rToLeft == RightToLeft.Yes)
    {
      sf.Alignment = StringAlignment.Far;
    }
    else
    {
      sf.Alignment = StringAlignment.Near;
    }
    DrawString(e.RibbonOrbDropDown.RecentItemsCaption, new Font(RibbonTabFont.FontFamily, RibbonTabFont.ISIZE, FontStyle.Bold), Brushes.DarkBlue, cb, sf);
  }
  
  using (SolidBrush b = new SolidBrush(OrbDropDownContentbglight))
  {
    //Menu items
    gcFillRectangle2(b, Bcontent);
  }
  
  using (Pen p = new Pen(OrbDropDownSeparatorlight))
  {
    DrawLine(p, Bcontent.r, Bcontent.t, Bcontent.r, Bcontent.b);
  }
  
  using (Pen p = new Pen(OrbDropDownSeparatordark))
  {
    DrawLine(p, Bcontent.r - 1, Bcontent.t, Bcontent.r - 1, Bcontent.b);
  }
  
  using (Pen p = new Pen(OrbDropDownLightBorder))
  {
    DrawRectangle(p, OuterRect);
  }
  
  using (Pen p = new Pen(OrbDropDownDarkBorder))
  {
    DrawRectangle(p, InnerRect);
  }
  // #endregion
  
  // #region Orb
  if (OrbVisible && CaptionBarVisible && OrbStyle == RibbonOrbStyleOffice_2007)
  {
    IRECT orbb = RectangleToScreen(OrbBounds);
    orbb = e.RibbonOrbDropDown.RectangleToClient(orbb);
    DrawOrb(e.Graphics, orbb, OrbImage, OrbSelected, OrbPressed);
  }
  // #endregion
}

void OnRenderRibbonQuickAccessToolbarBackground(RibbonRenderEventArgs e)
{
  /// a-----b    a-----b
  ///  z    |   z       |
  ///   c---d    c-----d
  IRECT bounds = QuickAcessToolbar.Bounds;
  Padding padding = QuickAcessToolbar.Padding;
  Padding margin = QuickAcessToolbar.Margin;
  IPOINT a = new IPOINT(bounds.l - (OrbVisible ? margin.l : 0), bounds.t);
  IPOINT b = new IPOINT(bounds.r + padding.r, bounds.t);
  IPOINT c = new IPOINT(bounds.l, bounds.b);
  IPOINT d = new IPOINT(b.X, c.Y);
  IPOINT z = new IPOINT(c.X - 2, a.Y + boundsHeight / 2 - 1);
  
  bool aero = ActualBorderMode == RibbonWindowMode.NonClientAreaGlass && RibbonDesigner.Current == null;
  if (rToLeft == RightToLeft.Yes)
  {
    ///   a-----b    a-----b
    ///  |     z    |       z
    ///   c---d      c-----d
    a = new IPOINT(bounds.l + padding.l, bounds.t);
    b = new IPOINT(bounds.r + (OrbVisible ? margin.r : 0), bounds.t);
    c = new IPOINT(a.X, bounds.b);
    d = new IPOINT(bounds.r, bounds.b);
    z = new IPOINT(d.X + 2, b.Y + (boundsHeight / 2) - 1);
  }
  
  using (Path path = CreateQuickAccessPath(a, b, c, d, z, bounds, 0, 0, e.Ribbon))
  {
    if (!aero)
    {
      using (Pen p = new Pen(ThemeColorTable.QuickAccessBorderLight, 3))
      {
        DrawPath(p, path);
      }
    }
    using (Pen p = new Pen(ThemeColorTable.QuickAccessBorderDark, 1))
    {
      if (aero) p.Color = _A_RGB(150, 150, 150);
      DrawPath(p, path);
    }
    if (rToLeft == RightToLeft.Yes)
    {
      b = a;
      d = c;
    }
    if (!aero)
    {
      using (LinearGradientBrush br = new LinearGradientBrush(
        b, d, _A_RGB(150, ThemeColorTable.QuickAccessUpper), _A_RGB(150, ThemeColorTable.QuickAccessLower)
        ))
      {
        FillPath(br, path);
      }
    }
    else
    {
      using (LinearGradientBrush br = new LinearGradientBrush(
        b, d,
        _A_RGB(66, RibbonProfesionalRendererColorTable.ToGray(ThemeColorTable.QuickAccessUpper)),
        _A_RGB(66, RibbonProfesionalRendererColorTable.ToGray(ThemeColorTable.QuickAccessLower))
        ))
      {
        FillPath(br, path);
      }
    }
  }
}

void OnRenderRibbonOrb(RibbonRenderEventArgs e)
{
  if (OrbVisible)
  {
    if (OrbStyle == RibbonOrbStyleOffice_2007)
    {
      if (CaptionBarVisible)
        DrawOrb(e.Graphics, OrbBounds, OrbImage, OrbSelected, OrbPressed);
    }
    else if (OrbStyle == RibbonOrbStyleOffice_2010 | OrbStyle == RibbonOrbStyleOffice_2013) //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
    {
      //draw 2010 style
      RibbonRenderEventArgs args = new RibbonRenderEventArgs(e.Ribbon, e.Graphics, OrbBounds);
      if (OrbPressed)
      {
        DrawOrbPressed(args);
      }
      else if (OrbSelected)
      {
        DrawOrbSelected(args);
      }
      else
      {
        DrawOrbNormal(args);
      }
      
      if (OrbStyle == RibbonOrbStyleOffice_2010)
      {
        if (OrbText != string.Empty)
          TextRenderer.DrawText(e.Graphics, OrbText, RibbonTabFont, OrbBounds, ThemeColorTable.OrbButtonText);
      }
      else if (OrbStyle == RibbonOrbStyleOffice_2013)
      {
        if (OrbText != string.Empty)
          TextRenderer.DrawText(e.Graphics, OrbText, RibbonTabFont, OrbBounds, ThemeColorTable.OrbButtonText_2013);
      }
      
      if (OrbImage != null)
      {
        IRECT irect = iRECT(IPOINT.Empty, OrbImage.ISIZE);
        irect.X = OrbBounds.X + (OrbBoundsWidth - irectWidth) / 2;
        irect.Y = OrbBounds.Y + (OrbBoundsHeight - irectHeight) / 2;
        DrawImage(OrbImage, irect);
      }
    }
  }
}

void OnRenderRibbonBackground(RibbonRenderEventArgs e)
{
  //if (ActualBorderMode == RibbonWindowMode.NonClientAreaGlass)
  //{
  //    Clear(ColorTransparent);
  //    SmoothingMode sbuff = SmoothingMode;
  //    SmoothingMode = SmoothingMode.None;
  //    gcFillRectangle2(new SolidBrush(ThemeColorTable.RibbonBackground), iRECT(0, CaptionBarSize + 1, e.RibbonWidth, e.RibbonHeight));
  //    SmoothingMode = sbuff;
  //}
  //else
  //{
  //   Clear(ThemeColorTable.RibbonBackground);
  //}
  
  if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010)
  {
    Clear(ThemeColorTable.RibbonBackground);
  }
  if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    Clear(ThemeColorTable.RibbonBackground_2013);
  }
  
  if (ActualBorderMode == RibbonWindowMode.NonClientAreaGlass)
  {
    WinApi.FillForGlass(e.Graphics, iRECT(0, 0, e.RibbonWidth, CaptionBarSize + 1));
  }
}

void OnRenderRibbonTab(Graphics* g)
{
  if (Minimized && !Expanded)
  {
    DrawTabMinimized(e);
  }
  else if (TabActive)
  {
    DrawCompleteTab(e);
  }
  else if (TabPressed)
  {
    //DrawTabPressed(e);
  }
  else if (TabSelected)
  {
    if (OrbStyle == RibbonOrbStyleOffice_2010 | OrbStyle == RibbonOrbStyleOffice_2013) //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
      DrawTabMinimized(e);
    else if (OrbStyle == RibbonOrbStyleOffice_2007)
      DrawTabSelected(e);
  }
  else
  {
    DrawTabNormal(e);
  }
}

void OnRenderRibbonTabText(Graphics* g)
{
  StringFormat sf = new StringFormat();
  
  sf.Alignment = StringAlignment.Center;
  sf.Trimming = StringTrimming.EllipsisCharacter;
  sf.LineAlignment = StringAlignment.Center;
  sf.FormatFlags |= StringFormatFlags.NoWrap;
  
  IRECT r = iRECT(TabBounds.l + TabTextMargin.l, TabBounds.t + TabTextMargin.t, TabBounds.r - TabTextMargin.r, TabBounds.b - TabTextMargin.b);
  
  if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010) //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
  {
    using (Brush b = new SolidBrush(GetTextColor(true, TabActive ? ThemeColorTable.TabActiveText : ThemeColorTable.TabText)))
    {
      TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
      DrawString(TabText, RibbonTabFont, b, r, sf);
    }
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    using (Brush b = new SolidBrush(GetTextColor(true, TabActive | TabSelected ? ThemeColorTable.TabText_2013 : ThemeColorTable.TabTextSelected_2013)))
    {
      TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
      DrawString(TabText, RibbonTabFont, b, r, sf);
    }
  }
}

void OnRenderRibbonPanelBackground(Graphics* g)
{
  if (e.Panel.OverflowMode && !(e.Canvas is RibbonPanelPopup))
  {
    if (e.Panel.Pressed)
    {
      DrawPanelOverflowPressed(e);
    }
    else if (e.Panel.Selected)
    {
      DrawPannelOveflowSelected(e);
    }
    else
    {
      DrawPanelOverflowNormal(e);
    }
  }
  else
  {
    if (e.Panel.Selected)
    {
      if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010) //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
      {
        DrawPanelSelected(e);
      }
      else if (OrbStyle == RibbonOrbStyleOffice_2013) //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
      {
        //DO NOT DRAW ANYTHING
      }
    }
    else
    {
      DrawPanelNormal(e);
    }
  }
}

void OnRenderRibbonPanelText(Graphics* g)
{
  if (e.Panel.OverflowMode && !(e.Canvas is RibbonPanelPopup))
  {
    return;
  }
  
  IRECT textArea =
    iRECT(
    e.Panel.Bounds.l + 1,
    e.Panel.ContentBounds.b,
    e.Panel.Bounds.r - 1,
    e.Panel.Bounds.b - 1);
  
  StringFormat sf = new StringFormat();
  sf.Alignment = StringAlignment.Center;
  sf.LineAlignment = StringAlignment.Center;
  
  if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010)
  {
    using (Brush b = new SolidBrush(GetTextColor(e.Panel.Enabled, ThemeColorTable.PanelText)))
    {
      DrawString(e.Panel.Text, Font, b, textArea, sf);
    }
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    using (Brush b = new SolidBrush(GetTextColor(e.Panel.Enabled, ThemeColorTable.PanelText_2013)))
    {
      DrawString(e.Panel.Text, Font, b, textArea, sf);
    }
  }
  
}

void OnRenderRibbonItem(RibbonItemRenderEventArgs e)
{
  if (e.Item is RibbonButton)
  {
    // #region Button
    RibbonButton b = e.Item as RibbonButton;
    
    if (b.Enabled)
    {
      if (b.Style == RibbonButtonStyle.Normal)
      {
        if (b.Pressed && b.SizeMode != RibbonElementSizeMode.DropDown)
        {
          DrawButtonPressed(e.Graphics, b, e.Ribbon);
        }
        else if (b.Selected)
        {
          if (b.Checked)
          {
            DrawButtonPressed(e.Graphics, b, e.Ribbon);
          }
          else
          {
            DrawButtonSelected(e.Graphics, b, e.Ribbon);
          }
        }
        else if (b.Checked)
        {
          DrawButtonChecked(e.Graphics, b);
        }
        else if (b is RibbonOrbOptionButton)
        {
          DrawOrbOptionButton(e.Graphics, b.Bounds);
        }
        else
        {
          //No background
        }
      }
      else
      {
        if (b.Style == RibbonButtonStyle.DropDownListItem)
        {
          //clear out the drowdown background so we don't see the image area shading
          using (SolidBrush br = new SolidBrush(ThemeColorTable.DropDownBg))
          {
            //Clear(ColorTransparent);
            SmoothingMode sbuff = SmoothingMode;
            SmoothingMode = SmoothingMode.AntiAlias;
            gcFillRectangle2(br, b.Bounds);
            SmoothingMode = sbuff;
          }
        }
        
        if (b.DropDownPressed && b.SizeMode != RibbonElementSizeMode.DropDown)
        {
          DrawButtonPressed(e.Graphics, b, e.Ribbon);
          DrawSplitButtonDropDownSelected(e, b);
        }
        else if (b.Pressed && b.SizeMode != RibbonElementSizeMode.DropDown)
        {
          DrawButtonPressed(e.Graphics, b, e.Ribbon);
          DrawSplitButtonSelected(e, b);
        }
        
        else if (b.DropDownSelected)
        {
          DrawButtonSelected(e.Graphics, b, e.Ribbon);
          DrawSplitButtonDropDownSelected(e, b);
        }
        else if (b.Selected)
        {
          DrawButtonSelected(e.Graphics, b, e.Ribbon);
          DrawSplitButtonSelected(e, b);
        }
        else if (b.Checked)
        {
          DrawButtonChecked(e.Graphics, b);
        }
        else
        {
          DrawSplitButton(e, b);
        }
      }
    }
    
    
    
    
    if (b.Style != RibbonButtonStyle.Normal && !(b.Style == RibbonButtonStyle.DropDown && b.SizeMode == RibbonElementSizeMode.Large))
    {
      if (b.Style == RibbonButtonStyle.DropDown)
      {
        DrawButtonDropDownArrow(e.Graphics, b, b.OnGetDropDownBounds(b.SizeMode, b.Bounds));
      }
      else
      {
        DrawButtonDropDownArrow(e.Graphics, b, b.DropDownBounds);
      }
    }
    
    // #endregion
  }
  else if (e.Item is RibbonItemGroup)
  {
    // #region Group
    DrawItemGroup(e, e.Item as RibbonItemGroup);
    // #endregion
  }
  else if (e.Item is RibbonButtonList)
  {
    // #region ButtonList
    DrawButtonList(e.Graphics, e.Item as RibbonButtonList, e.Ribbon);
    // #endregion
  }
  else if (e.Item is RibbonSeparator)
  {
    // #region Separator
    if (e.Item.Visible)
    {
      DrawSeparator(e.Graphics, e.Item as RibbonSeparator);
    }
    // #endregion
  }
  else if (e.Item is RibbonUpDown)
  {
    // #region UpDown
    
    RibbonUpDown t = e.Item as RibbonUpDown;
    
    if (t.Enabled)
    {
      if (t != null && (t.Selected || (t.Editing)))
      {
        DrawTextBoxSelected(e.Graphics, t.TextBoxBounds);
      }
      else
      {
        DrawTextBoxUnselected(e.Graphics, t.TextBoxBounds);
      }
    }
    else
    {
      DrawTextBoxDisabled(e.Graphics, t.TextBoxBounds);
    }
    
    DrawUpDownButtons(e.Graphics, t, e.Ribbon);
    // #endregion
  }
  else if (e.Item is RibbonTextBox)
  {
    // #region TextBox
    
    RibbonTextBox t = e.Item as RibbonTextBox;
    
    if (t.Enabled)
    {
      if (t != null && (t.Selected || (t.Editing)))
      {
        DrawTextBoxSelected(e.Graphics, t.TextBoxBounds);
      }
      else
      {
        DrawTextBoxUnselected(e.Graphics, t.TextBoxBounds);
      }
      
    }
    else
    {
      DrawTextBoxDisabled(e.Graphics, t.TextBoxBounds);
    }
    
    if (t is RibbonComboBox)
    {
      DrawComboxDropDown(e.Graphics, t as RibbonComboBox, e.Ribbon);
    }
    
    // #endregion
  }
}

void OnRenderRibbonItemBorder(RibbonItemRenderEventArgs e)
{
  if (e.Item is RibbonItemGroup)
  {
    DrawItemGroupBorder(e, e.Item as RibbonItemGroup);
  }
}

void OnRenderRibbonItemText(RibbonTextEventArgs e)
{
  Color foreColor = e.Color;
  StringFormat sf = e.Format;
  Font f = Font;
  bool embedded = false;
  
  if (e.Item is RibbonButton)
  {
    // #region Button
    RibbonButton b = e.Item as RibbonButton;
    
    if (b is RibbonCaptionButton)
    {
      if (WinApi.IsWindows) f = new Font(RibbonCaptionButton.WindowsIconsFont, f.ISIZE);
      embedded = true;
      foreColor = ThemeColorTable.Arrow;
    }
    
    if (b.Style == RibbonButtonStyle.DropDown && b.SizeMode == RibbonElementSizeMode.Large)
    {
      DrawButtonDropDownArrow(e.Graphics, b, e.Bounds);
    }
    
    // #endregion
  }
  else if (e.Item is RibbonSeparator)
  {
    if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010)
      foreColor = GetTextColor(e.Item.Enabled, ThemeColorTable.Text);
    else if (OrbStyle == RibbonOrbStyleOffice_2013)
      foreColor = GetTextColor(e.Item.Enabled, ThemeColorTable.RibbonItemText_2013);
  }
  
  embedded = embedded || !e.Item.Enabled;
  
  if (embedded)
  {
    IRECT cbr = e.Bounds; cbr.Y++;
    using (SolidBrush b = new SolidBrush(ThemeColorTable.ArrowLight))
    {
      DrawString(e.Text, new Font(f, e.Style), b, cbr, sf);
    }
  }
  
  if (foreColorEquals(ColorEmpty))
  {
    if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010)
      foreColor = GetTextColor(e.Item.Enabled, ThemeColorTable.Text);
    else if (OrbStyle == RibbonOrbStyleOffice_2013)
      foreColor = GetTextColor(e.Item.Enabled, ThemeColorTable.RibbonItemText_2013);
  }
  
  using (SolidBrush b = new SolidBrush(foreColor))
  {
    DrawString(e.Text, new Font(f, e.Style), b, e.Bounds, sf);
  }
}

void OnRenderRibbonItemImage(RibbonItemBoundsEventArgs e)
{
  Image img = e.Item.ShowFlashImage ? e.Item.FlashImage : e.Item.Image;
  
  if (e.Item is RibbonButton)
  {
    if (!(e.Item.SizeMode == RibbonElementSizeMode.Large || e.Item.SizeMode == RibbonElementSizeMode.Overflow))
    {
      img = e.Item.ShowFlashImage ? (e.Item as RibbonButton).SmallImage : (e.Item as RibbonButton).SmallImage;
    }
  }
  
  if (img != null)
  {
    
    if (!e.Item.Enabled)
      img = CreateDisabledImage(img);
    
    DrawImage(img, e.Bounds);
  }
  
}

void OnRenderPanelPopupBackground(RibbonCanvasEventArgs e)
{
  RibbonPanel pnl = e.RelatedObject as RibbonPanel;
  
  if (pnl == null) return;
  
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
  
  Path dark = RoundRectangle(darkBorder, 3);
  Path light = RoundRectangle(lightBorder, 3);
  Path txt = RoundRectangle(textArea, 3, CornersSouthEast | CornersSouthWest);
  
  using (Pen p = new Pen(ThemeColorTable.PanelLightBorder))
  {
    DrawPath(p, light);
  }
  
  using (Pen p = new Pen(ThemeColorTable.PanelDarkBorder))
  {
    DrawPath(p, dark);
  }
  
  using (SolidBrush b = new SolidBrush(ThemeColorTable.PanelBackgroundSelected))
  {
    FillPath(b, light);
  }
  
  using (SolidBrush b = new SolidBrush(ThemeColorTable.PanelTextBackground))
  {
    FillPath(b, txt);
  }
  
  txt.Dispose();
  dark.Dispose();
  light.Dispose();
}

void OnRenderDropDownBackground(RibbonCanvasEventArgs e)
{
  IRECT outerR = iRECT(0, 0, e.BoundsWidth - 1, e.BoundsHeight - 1);
  IRECT imgsR = iRECT(0, 0, 26, e.BoundsHeight);
  RibbonDropDown dd = e.Canvas as RibbonDropDown;
  
  using (SolidBrush b = new SolidBrush(ThemeColorTable.DropDownBg))
  {
    Clear(ColorTransparent);
    SmoothingMode sbuff = SmoothingMode;
    SmoothingMode = SmoothingMode.AntiAlias;
    gcFillRectangle2(b, outerR);
    SmoothingMode = sbuff;
  }
  
  if (dd != null && dd.DrawIconsBar)
  {
    using (SolidBrush b = new SolidBrush(ThemeColorTable.DropDownImageBg))
    {
      gcFillRectangle2(b, imgsR);
    }
    
    using (Pen p = new Pen(ThemeColorTable.DropDownImageSeparator))
    {
      DrawLine(p,
        new IPOINT(imgsR.r, imgsR.t),
        new IPOINT(imgsR.r, imgsR.b));
    }
  }
  
  using (Pen p = new Pen(ThemeColorTable.DropDownBorder))
  {
    if (dd != null)
    {
      using (Path r = RoundRectangle(iRECT(IPOINT.Empty, new ISIZE(dd.ISIZEWidth - 1, dd.ISIZEHeight - 1)), dd.BorderRoundness))
      {
        SmoothingMode smb = SmoothingMode;
        SmoothingMode = SmoothingMode.AntiAlias;
        DrawPath(p, r);
        SmoothingMode = smb;
      }
    }
    else
    {
      DrawRectangle(p, outerR);
    }
  }
  
  if (dd.ShowSizingGrip)
  {
    IRECT gripArea = iRECT(
      e.Bounds.l + 1,
      e.Bounds.b - dd.SizingGripHeight,
      e.Bounds.r - 1,
      e.Bounds.b - 1);
    
    using (LinearGradientBrush b = new LinearGradientBrush(
      gripArea, ThemeColorTable.DropDownGripNorth, ThemeColorTable.DropDownGripSouth, 90))
    {
      gcFillRectangle2(b, gripArea);
    }
    
    using (Pen p = new Pen(ThemeColorTable.DropDownGripBorder))
    {
      DrawLine(p,
        gripArea.Location,
        new IPOINT(gripArea.r - 1, gripArea.t));
    }
    
    DrawGripDot(e.Graphics, new IPOINT(gripArea.r - 7, gripArea.b - 3));
    DrawGripDot(e.Graphics, new IPOINT(gripArea.r - 3, gripArea.b - 7));
    DrawGripDot(e.Graphics, new IPOINT(gripArea.r - 3, gripArea.b - 3));
  }
}

void OnRenderTabScrollButtons(Graphics* g)
{
  if (TabScrollLeftVisible)
  {
    if (TabScrollLeftSelected)
    {
      DrawButtonSelected(e.Graphics, TabScrollLeftBounds, CornersWest, e.Ribbon);
    }
    else
    {
      DrawButton(e.Graphics, TabScrollLeftBounds, CornersWest);
    }
    
    DrawArrowShaded(e.Graphics, TabScrollLeftBounds, RibbonArrowDirectionRight, true);
    
  }
  
  if (TabScrollRightVisible)
  {
    if (TabScrollRightSelected)
    {
      DrawButtonSelected(e.Graphics, TabScrollRightBounds, CornersEast, e.Ribbon);
    }
    else
    {
      DrawButton(e.Graphics, TabScrollRightBounds, CornersEast);
    }
    
    DrawArrowShaded(e.Graphics, TabScrollRightBounds, RibbonArrowDirectionLeft, true);
  }
}

void OnRenderScrollbar(Graphics* g, Control ctl, Ribbon ribbon)
{
  RibbonDropDown list = (RibbonDropDown)ctl;
  if (ScrollBarRenderer.IsSupported)
  {
    ScrollBarRenderer.DrawUpperVerticalTrack(g, list.ScrollBarBounds, ScrollBarState.Normal);
    
    if (list.ThumbPressed)
    {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Pressed);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Pressed);
    }
    else if (list.ThumbSelected)
    {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Hot);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Hot);
    }
    else
    {
      ScrollBarRenderer.DrawVerticalThumb(g, list.ThumbBounds, ScrollBarState.Normal);
      ScrollBarRenderer.DrawVerticalThumbGrip(g, list.ThumbBounds, ScrollBarState.Normal);
    }
    
    if (list.ButtonUpPressed)
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpPressed);
    }
    else if (list.ButtonUpSelected)
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpHot);
    }
    else
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonUpBounds, ScrollBarArrowButtonState.UpNormal);
    }
    
    if (list.ButtonDownPressed)
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownPressed);
    }
    else if (list.ButtonDownSelected)
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownHot);
    }
    else
    {
      ScrollBarRenderer.DrawArrowButton(g, list.ButtonDownBounds, ScrollBarArrowButtonState.DownNormal);
    }
  }
  else
  {
    // #region Control Buttons
    
    using (SolidBrush b = new SolidBrush(ThemeColorTable.ButtonGlossyNorth))
    {
      g.gcFillRectangle2(b, list.ScrollBarBounds);
    }
    
    if (!list.ButtonDownEnabled)
    {
      DrawButtonDisabled(g, list.ButtonDownBounds, CornersSouthEast);
    }
    else if (list.ButtonDownPressed)
    {
      DrawButtonPressed(g, list.ButtonDownBounds, CornersSouthEast, ribbon);
    }
    else if (list.ButtonDownSelected)
    {
      DrawButtonSelected(g, list.ButtonDownBounds, CornersSouthEast, ribbon);
    }
    else
    {
      DrawButton(g, list.ButtonDownBounds, CornersNone);
    }
    
    if (!list.ButtonUpEnabled)
    {
      DrawButtonDisabled(g, list.ButtonUpBounds, CornersNorthEast);
    }
    else if (list.ButtonUpPressed)
    {
      DrawButtonPressed(g, list.ButtonUpBounds, CornersNorthEast, ribbon);
    }
    else if (list.ButtonUpSelected)
    {
      DrawButtonSelected(g, list.ButtonUpBounds, CornersNorthEast, ribbon);
    }
    else
    {
      DrawButton(g, list.ButtonUpBounds, CornersNorthEast);
    }
    
    if (list.ScrollBarEnabled)
    {
      if (list.ThumbPressed)
      {
        DrawButtonPressed(g, list.ThumbBounds, CornersAll, ribbon);
      }
      else if (list.ThumbSelected)
      {
        DrawButtonSelected(g, list.ThumbBounds, CornersAll, ribbon);
      }
      else
      {
        DrawButton(g, list.ThumbBounds, CornersAll);
      }
    }
    
    Color dk = ThemeColorTable.Arrow;
    Color lt = ThemeColorTable.ArrowLight;
    Color ds = ThemeColorTable.ArrowDisabled;
    
    IRECT arrUp = CenterOn(list.ButtonUpBounds, iRECT(IPOINT.Empty, arrowSize)); arrUp.Offset(0, 1);
    IRECT arrD = CenterOn(list.ButtonDownBounds, iRECT(IPOINT.Empty, arrowSize)); arrD.Offset(0, 1);
    
    DrawArrow(g, arrUp, list.ButtonUpEnabled ? lt : ColorTransparent, RibbonArrowDirectionUp); arrUp.Offset(0, -1);
    DrawArrow(g, arrUp, list.ButtonUpEnabled ? dk : ds, RibbonArrowDirectionUp);
    
    DrawArrow(g, arrD, list.ButtonDownEnabled ? lt : ColorTransparent, RibbonArrowDirectionDown); arrD.Offset(0, -1);
    DrawArrow(g, arrD, list.ButtonDownEnabled ? dk : ds, RibbonArrowDirectionDown);
    // #endregion
  }
}

void OnRenderToolTipBackground(RibbonToolTipRenderEventArgs e)
{
  IRECT darkBorder = iRECT(
    e.ClipRectangle.l,
    e.ClipRectangle.t,
    e.ClipRectangle.r - 1,
    e.ClipRectangle.b - 1);
  
  IRECT lightBorder = iRECT(
    e.ClipRectangle.l + 1,
    e.ClipRectangle.t + 1,
    e.ClipRectangle.r - 2,
    e.ClipRectangle.b - 1);
  
  Path dark = RoundRectangle(darkBorder, 3);
  Path light = RoundRectangle(lightBorder, 3);
  
  //Draw the Drop Shadow
  IRECT shadow = e.ClipRectangle; shadow.Offset(2, 1);
  using (Path path = RoundRectangle(shadow, 3, CornersAll))
  {
    using (PathGradientBrush b = new PathGradientBrush(path))
    {
      b.WrapMode = WrapMode.Clamp;
      
      ColorBlend cb = new ColorBlend(3);
      cb.Colors = new Color[]{ColorTransparent, 
        _A_RGB(50, ColorBlack), 
        _A_RGB(100, ColorBlack)};
      cb.Positions = new float[] { 0f, .1f, 1f };
      
      b.InterpolationColors = cb;
      
      FillPath(b, path);
    }
  }
  
  //Fill the background
  //using (SolidBrush b = new SolidBrush(ThemeColorTable.ToolTipContentSouth))
  //{
  //   FillPath(b, dark);
  //}
  using (LinearGradientBrush b = new LinearGradientBrush(
    e.ClipRectangle, ThemeColorTable.ToolTipContentNorth, ThemeColorTable.ToolTipContentSouth, 90))
  {
    FillPath(b, dark);
  }
  
  //Draw the borders
  using (Pen p = new Pen(ThemeColorTable.ToolTipLightBorder))
  {
    DrawPath(p, light);
  }
  using (Pen p = new Pen(ThemeColorTable.ToolTipDarkBorder))
  {
    DrawPath(p, dark);
  }
  
  dark.Dispose();
  light.Dispose();
}

void OnRenderToolTipText(RibbonToolTipRenderEventArgs e)
{
  if (OrbStyle == RibbonOrbStyleOffice_2007 | OrbStyle == RibbonOrbStyleOffice_2010)
  {
    using (Brush b = new SolidBrush(ThemeColorTable.ToolTipText))
    {
      TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
      DrawString(e.Text, e.Font, b, e.ClipRectangle, e.Format);
    }
  }
  else if (OrbStyle == RibbonOrbStyleOffice_2013)
  {
    using (Brush b = new SolidBrush(ThemeColorTable.ToolTipText_2013))
    {
      TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
      DrawString(e.Text, e.Font, b, e.ClipRectangle, e.Format);
    }
  }
}

void OnRenderToolTipImage(RibbonToolTipRenderEventArgs e)
{
  DrawImage(e.TipImage, e.ClipRectangle);
}
// #endregion



#endif
