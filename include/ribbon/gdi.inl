typedef double REAL;
typedef DPOLYGON GraphicsPath;
int dPOINT_AddLine(DPOINT* pt, int i, int n, REAL x1, REAL y1, REAL x2, REAL y2)
{
  ASSERT(i + 2 <= n);
  pt[i++] = dPOINT(x1, y1);
  pt[i++] = dPOINT(x2, y2);
  return i;
}
int dPOINT_AddLine2(DPOINT* pt, int i, int n, IPOINT pt1, IPOINT pt2)
{
  return dPOINT_AddLine(pt, i, n, pt1.x, pt1.y, pt2.x, pt2.y);
}
static int dPOINT_AddArc(DPOINT* pt, int i, int n, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
{
  i = vcgen_arc(pt, i, n, sweepAngle > 0, x + width / 2.00, y + height / 2.00, width / 2.00, height / 2.00, deg2rad(startAngle), deg2rad(startAngle + sweepAngle));
  return i;
}
static int dPOINT_AddArc2(DPOINT* pt, int i, int n, DRECT rc, REAL startAngle, REAL sweepAngle)
{
  return dPOINT_AddArc(pt, i, n, rc.l, rc.t, RCW(&rc), RCH(&rc), startAngle, sweepAngle);
}
static int dPOINT_AddArc3(DPOINT* pt, int i, int n, IRECT rc, REAL startAngle, REAL sweepAngle)
{
  return dPOINT_AddArc(pt, i, n, rc.l, rc.t, RCW(&rc), RCH(&rc), startAngle, sweepAngle);
}
static int dPOINT_AddRectangle(DPOINT* pt, int i, int n, REAL x1, REAL y1, REAL x2, REAL y2)
{
  i = vcgen_rect(pt, i, n, x1, y1, x2, y2);
  return i;
}
static int dPOINT_AddEllipse(DPOINT* pt, int i, int num, IRECT rc)
{
  i += vcgen_ellipseR(pt + i, num - i, (&rc));
  return i;
}
int dPOINT_AddCurve(DPOINT* pt, int i, int n, int npt, const DPOINT* pt1)
{
  return vcgen_curve(pt, i, n, npt, pt1);
}
int dPOINT_AddCurve2(DPOINT* pt, int i, int n, IPOINT a, IPOINT b, IPOINT c)
{
  DPOINT pt1[] = {a.x, a.y, b.x, b.y, c.x, c.y};
  return dPOINT_AddCurve(pt, i, n, 3, pt1);
}
/// Creates a rectangle with the specified corners rounded
/// <param name="r"></param>
/// <param name="radius"></param>
/// <param name="corners"></param>
static int RoundRectangle(DPOINT* pt, int i, int n, IRECT r, int radius, int corners)
{
  int d = radius * 2;
  int nw = (corners & Corners_NorthWest) == Corners_NorthWest ? d : 0;
  int ne = (corners & Corners_NorthEast) == Corners_NorthEast ? d : 0;
  int se = (corners & Corners_SouthEast) == Corners_SouthEast ? d : 0;
  int sw = (corners & Corners_SouthWest) == Corners_SouthWest ? d : 0;
#if 0
  i = vcgen_roundrect(pt, i, n, r.l, r.t, r.r, r.b, nw, nw, ne, ne, se, se, sw, sw);
#else
  i = dPOINT_AddLine(pt, i, n, r.l + nw, r.t, r.r - ne, r.t);
  if (ne > 0) {
    i = dPOINT_AddArc2(pt, i, n, dRECT(r.r - ne, r.t, r.r, r.t + ne), -90, 90);
  }
  i = dPOINT_AddLine(pt, i, n, r.r, r.t + ne, r.r, r.b - se);
  if (se > 0) {
    i = dPOINT_AddArc2(pt, i, n, dRECT(r.r - se, r.b - se, r.r, r.b), 0, 90);
  }
  i = dPOINT_AddLine(pt, i, n, r.r - se, r.b, r.l + sw, r.b);
  if (sw > 0) {
    i = dPOINT_AddArc2(pt, i, n, dRECT(r.l, r.b - sw, r.l + sw, r.b), 90, 90);
  }
  i = dPOINT_AddLine(pt, i, n, r.l, r.b - sw, r.l, r.t + nw);
  if (nw > 0) {
    i = dPOINT_AddArc2(pt, i, n, dRECT(r.l, r.t, r.l + nw, r.t + nw), 180, 90);
  }
  //dPOLYGON_CloseFigure();
#endif
  return i;
}
/// Creates a rectangle with rounded corners
/// <param name="r"></param>
/// <param name="radius"></param>
static int RoundRectangle2(DPOINT* pt, int i, int n, IRECT r, int radius)
{
  return RoundRectangle(pt, i, n, r, radius, Corners_All);
}
#if 0
static int dPOLYGON_CloseFigure(DPOLYGON* path)
{
  return 0;
}
static int ClonePath(GraphicsPath* path, GraphicsPath** clonePath)
{
  *clonePath = new GraphicsPath;
  (*clonePath)->m_FillMode = path->m_FillMode;
  (*clonePath)->m_agg_ps = path->m_agg_ps;
  return 0;
}
static int
GraphicsPath_DeletePath(GraphicsPath* path)
{
  apf_SafeCheckParameter(path);
  delete path;
  return 0;
}
static int
GraphicsPath_ResetPath(GraphicsPath* path)
{
  apf_SafeCheckParameter(path);
  path->m_agg_ps.remove_all();
  return 0;
}
static int
GraphicsPath_GetPointCount(GraphicsPath* path, INT* count)
{
  apf_SafeCheckParameter(path);
  apf_SafeCheckParameter(count);
  unsigned int nPointCount = 0;
  unsigned int nTotal = path->m_agg_ps.total_vertices();
  double x, y;
  for (unsigned int i = 0; i < nTotal; i++) {
    unsigned int nTip = path->m_agg_ps.vertex(i, &x, &y);
    if (nTip) {
      if (!(nTip & agg::path_flags_close)) {
        nPointCount++;
      }
    }
  }
  *count = (INT) nPointCount;
  return 0;
}
/*
static int
GraphicsPath_GetPathTypes(GraphicsPath* path, BYTE* types, INT count);
*/
static int
GraphicsPath_GetPathPoints(GraphicsPath* path, GpPointF* points, INT count)
{
  apf_SafeCheckParameter(path);
  apf_SafeCheckParameter(points);
  int nTotal = path->m_agg_ps.total_vertices();
  double x, y;
  int i = 0, k = 0;
  while (k < count && i < nTotal) {
    unsigned int nTip = path->m_agg_ps.vertex(i, &x, &y);
    if (nTip) {
      if (!(nTip & agg::path_flags_close)) {
        points[k].X = REAL(x);
        points[k].Y = REAL(y);
        k++;
      }
    }
    i++;
  }
  return 0;
}
static int
GraphicsPath_GetPathPointsI(GraphicsPath* path, GpPoint* points, INT count)
{
  apf_SafeCheckParameter(path);
  apf_SafeCheckParameter(points);
  int nTotal = path->m_agg_ps.total_vertices();
  double x, y;
  int i = 0, k = 0;
  while (k < count && i < nTotal) {
    unsigned int nTip = path->m_agg_ps.vertex(i, &x, &y);
    if (nTip) {
      if (!(nTip & agg::path_flags_close)) {
        points[k].X = INT(x);
        points[k].Y = INT(y);
        k++;
      }
    }
    i++;
  }
  return 0;
}
static int
GraphicsPath_GetPathFillMode(GraphicsPath* path, GpFillMode* fillmode)
{
  apf_SafeCheckParameter(path);
  apf_SafeCheckParameter(fillmode);
  *fillmode = path->m_FillMode;
  return 0;
}
static int
GraphicsPath_SetPathFillMode(GraphicsPath* path, GpFillMode fillmode)
{
  apf_SafeCheckParameter(path);
  path->m_FillMode = fillmode;
  return 0;
}
/*
static int
GraphicsPath_GetPathData(GraphicsPath *path, GpPathData* pathData);
*/
static int
GraphicsPath_StartPathFigure(GraphicsPath* path)
{
  apf_SafeCheckParameter(path);
  path->m_agg_ps.start_new_path();
  return 0;
}
static int
GraphicsPath_ClosePathFigure(GraphicsPath* path)
{
  apf_SafeCheckParameter(path);
  path->m_agg_ps.close_polygon();
  return 0;
}
/*
static int
GraphicsPath_ClosePathFigures(GraphicsPath *path);
static int
GraphicsPath_SetPathMarker(GraphicsPath* path);
static int
GraphicsPath_ClearPathMarkers(GraphicsPath* path);
static int
GraphicsPath_ReversePath(GraphicsPath* path);
static int
GraphicsPath_GetPathLastPoint(GraphicsPath* path, GpPointF* lastPoint);
*/
static int
GraphicsPath_AddPathBezier(GraphicsPath* path, REAL x1, REAL y1, REAL x2, REAL y2,
    REAL x3, REAL y3, REAL x4, REAL y4)
{
  apf_SafeCheckParameter(path);
  if (z_Path_is_poly_closed(path)) {
    path->m_agg_ps.move_to(x1, y1);
  }
  else {
    path->m_agg_ps.line_to(x1, y1);
  }
  path->m_agg_ps.curve4(x2, y2, x3, y3, x4, y4);
  return 0;
}
/*
static int
GraphicsPath_AddPathBeziers(GraphicsPath *path, GDIPCONST GpPointF *points, INT count);
static int
GraphicsPath_AddPathCurve(GraphicsPath *path, GDIPCONST GpPointF *points, INT count);
static int
GraphicsPath_AddPathCurve2(GraphicsPath *path, GDIPCONST GpPointF *points, INT count,
                           REAL tension);
static int
GraphicsPath_AddPathCurve3(GraphicsPath *path, GDIPCONST GpPointF *points, INT count,
                           INT offset, INT numberOfSegments, REAL tension);
static int
GraphicsPath_AddPathClosedCurve(GraphicsPath *path, GDIPCONST GpPointF *points, INT count);
static int
GraphicsPath_AddPathClosedCurve2(GraphicsPath *path, GDIPCONST GpPointF *points, INT count,
                                 REAL tension);
*/
static int
GraphicsPath_AddPathRectangle(GraphicsPath* path, REAL x, REAL y, REAL width, REAL height)
{
  apf_SafeCheckParameter(path);
  path->m_agg_ps.move_to(x, y);
  path->m_agg_ps.line_to(x + width, y);
  path->m_agg_ps.line_to(x + width, y + height);
  path->m_agg_ps.line_to(x, y + height);
  path->m_agg_ps.close_polygon();
  return 0;
}
/*
static int
GraphicsPath_AddPathRectangles(GraphicsPath *path, GDIPCONST GpRectF *rects, INT count);
*/
static int
GraphicsPath_AddPathEllipse(GraphicsPath* path, REAL x, REAL y, REAL width, REAL height)
{
  apf_SafeCheckParameter(path);
  agg::bezier_arc arc(x + width / 2.00, y + height / 2.00, width / 2.00, height / 2.00, 0.00, 2.00 * agg::pi);
  path->m_agg_ps.add_path(arc, 0, true);
  return 0;
}
/*
static int
GraphicsPath_AddPathPie(GraphicsPath *path, REAL x, REAL y, REAL width, REAL height,
                        REAL startAngle, REAL sweepAngle);
static int
GraphicsPath_AddPathPolygon(GraphicsPath *path, GDIPCONST GpPointF *points, INT count);
static int
GraphicsPath_AddPathPath(GraphicsPath *path, GDIPCONST GraphicsPath* addingPath, BOOL connect);
*/
static int
GraphicsPath_AddPathString(GraphicsPath* path, GDIPCONST WCHAR* string,
    INT length, GDIPCONST GpFontFamily* family, INT style,
    REAL emSize, GDIPCONST RectF* layoutRect,
    GDIPCONST GpStringFormat* format)
{
  apf_SafeCheckParameter(path);
  apf_SafeCheckParameter(string);
  apf_SafeCheckParameter(family);
  Unit unit = UnitWorld; //or UnitPixel, ZTODO: check this
  GpFont* font = NULL;
  GpStatus Stts = GraphicsPath_CreateFont(family, emSize, style, unit, &font);
  if (Stts == 0) {
    Stts = aggplus_flat_add_string_byfont_getbounds(path, string, length, font, *layoutRect, format, NULL, NULL, NULL);
    GraphicsPath_DeleteFont(font);
  }
  return Stts;
}
static int
GraphicsPath_AddPathStringI(GraphicsPath* path, GDIPCONST WCHAR* string,
    INT length, GDIPCONST GpFontFamily* family, INT style,
    REAL emSize, GDIPCONST IRECT* layoutRect,
    GDIPCONST GpStringFormat* format)
{
  RectF flayRect((REAL)layoutRect->X, (REAL)layoutRect->Y, (REAL)layoutRect->Width, (REAL)layoutRect->Height);
  return GraphicsPath_AddPathString(path, string, length, family, style, emSize, &flayRect, format);
}
static int
GraphicsPath_AddPathLineI(GraphicsPath* path, INT x1, INT y1, INT x2, INT y2)
{
  return GraphicsPath_AddPathLine(path, (REAL) x1, (REAL) y1, (REAL) x2, (REAL) y2);
}
/*
static int
GraphicsPath_AddPathLine2I(GraphicsPath *path, GDIPCONST GpPoint *points, INT count);
*/
static int
GraphicsPath_AddPathArcI(GraphicsPath* path, INT x, INT y, INT width, INT height,
    REAL startAngle, REAL sweepAngle)
{
  return GraphicsPath_AddPathArc(path, (REAL) x, (REAL) y, (REAL) width, (REAL) height,
      (REAL) startAngle, (REAL) sweepAngle);
}
static int
GraphicsPath_AddPathBezierI(GraphicsPath* path, INT x1, INT y1, INT x2, INT y2,
    INT x3, INT y3, INT x4, INT y4)
{
  return GraphicsPath_AddPathBezier(path, (REAL) x1, (REAL) y1, (REAL) x2, (REAL) y2,
      (REAL) x3, (REAL) y3, (REAL) x4, (REAL) y4);
}
/*
static int
GraphicsPath_AddPathBeziersI(GraphicsPath *path, GDIPCONST GpPoint *points, INT count);
static int
GraphicsPath_AddPathCurveI(GraphicsPath *path, GDIPCONST GpPoint *points, INT count);
static int
GraphicsPath_AddPathCurve2I(GraphicsPath *path, GDIPCONST GpPoint *points, INT count,
                           REAL tension);
static int
GraphicsPath_AddPathCurve3I(GraphicsPath *path, GDIPCONST GpPoint *points, INT count,
                           INT offset, INT numberOfSegments, REAL tension);
static int
GraphicsPath_AddPathClosedCurveI(GraphicsPath *path, GDIPCONST GpPoint *points, INT count);
static int
GraphicsPath_AddPathClosedCurve2I(GraphicsPath *path, GDIPCONST GpPoint *points, INT count,
                                 REAL tension);
*/
static int
GraphicsPath_AddPathRectangleI(GraphicsPath* path, INT x, INT y, INT width, INT height)
{
  apf_SafeCheckParameter(path);
  path->m_agg_ps.move_to(x, y);
  path->m_agg_ps.line_to(x + width, y);
  path->m_agg_ps.line_to(x + width, y + height);
  path->m_agg_ps.line_to(x, y + height);
  path->m_agg_ps.close_polygon();
  return 0;
}
/*
static int
GraphicsPath_AddPathRectanglesI(GraphicsPath *path, GDIPCONST GpRect *rects, INT count);
*/
static int
GraphicsPath_AddPathEllipseI(GraphicsPath* path, INT x, INT y, INT width, INT height)
{
  return GraphicsPath_AddPathEllipse(path, (REAL) x, (REAL) y, (REAL) width, (REAL) height);
}
/*
static int
GraphicsPath_AddPathPieI(GraphicsPath *path, INT x, INT y, INT width, INT height,
                        REAL startAngle, REAL sweepAngle);
static int
GraphicsPath_AddPathPolygonI(GraphicsPath *path, GDIPCONST GpPoint *points, INT count);
*/
static int
GraphicsPath_FlattenPath(GraphicsPath* path, GpMatrix* matrix, REAL flatness)
{
  apf_SafeCheckParameter(path);
  //ZTODO: matrix and flatness
  typedef agg::conv_curve<agg::path_storage> conv_crv_type;
  conv_crv_type p_conv(path->m_agg_ps);
  //TRACE(">>>%.2f\n", flatness);
  p_conv.approximation_scale(2.00 * flatness);
  agg::path_storage p_new;
  p_new.add_path(p_conv);
  path->m_agg_ps.remove_all();
  path->m_agg_ps = p_new;
  return 0;
}
/*
static int
GraphicsPath_WindingModeOutline(
    GraphicsPath *path,
    GpMatrix *matrix,
    REAL flatness
);
*/
static int
GraphicsPath_WidenPath(GraphicsPath* nativePath, GpPen* pen, GpMatrix* matrix, REAL flatness)
{
  apf_SafeCheckParameter(nativePath);
  apf_SafeCheckParameter(pen);
  //ASSERT(matrix==NULL && flatness==0.00); //ZTODO
  typedef agg::conv_curve<agg::path_storage> conv_crv_type;
  typedef agg::conv_contour<conv_crv_type> Path_Conv_Contour;
  conv_crv_type crv(nativePath->m_agg_ps);
  Path_Conv_Contour pg(crv);
  pg.miter_limit(0.50);
  //?pg.miter_limit_theta(0.05);
  //?pg.approximation_scale(2.00);
  pg.width(pen->m_fWidth);
  agg::line_join_e LineJoin;
  switch (pen->m_LineJoin) {
  case LineJoinMiter       :
    LineJoin = agg::miter_join;
    break;
  case LineJoinBevel       :
    LineJoin = agg::bevel_join;
    break;
  default:
  case LineJoinRound       :
    LineJoin = agg::round_join;
    break;
  case LineJoinMiterClipped:
    LineJoin = agg::miter_join_revert;
    break;
  }
  pg.line_join(LineJoin);
  pg.auto_detect_orientation(false);
  agg::path_storage psNew;
  psNew.add_path(pg, 0, false);
  nativePath->m_agg_ps = psNew;
  return 0;
}
/*
static int
GraphicsPath_WarpPath(GraphicsPath *path, GpMatrix* matrix,
            GDIPCONST GpPointF *points, INT count,
            REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight,
            WarpMode warpMode, REAL flatness);
*/
static int
GraphicsPath_TransformPath(GraphicsPath* path, GpMatrix* matrix)
{
  apf_SafeCheckParameter(path);
  if (matrix) {
    agg::path_storage p2(path->m_agg_ps);
    agg::conv_transform<agg::path_storage> trans(p2, matrix->m_agg_mtx);
    path->m_agg_ps.remove_all();
    path->m_agg_ps.add_path(trans, 0, false);
  }
  return 0;
}
static int
GraphicsPath_GetPathWorldBounds(GraphicsPath* path, GpRectF* bounds,
    GDIPCONST GpMatrix* matrix, GDIPCONST GpPen* pen)
{
  apf_SafeCheckParameter(path);
  apf_SafeCheckParameter(bounds);
  //apf_SafeCheckParameter(matrix);
  //apf_SafeCheckParameter(pen);
  unsigned int nTotal = path->m_agg_ps.total_vertices();
  if (nTotal) {
    agg::rect_d agg_bnds(1e100, 1e100, -1e100, -1e100);
    double x, y;
    for (unsigned int i = 0; i < nTotal; i++) {
      unsigned int nTip = path->m_agg_ps.vertex(i, &x, &y);
      if (nTip) {
        if (!(nTip & agg::path_flags_close)) {
          if (x < agg_bnds.x1) {
            agg_bnds.x1 = x;
          }
          if (y < agg_bnds.y1) {
            agg_bnds.y1 = y;
          }
          if (x > agg_bnds.x2) {
            agg_bnds.x2 = x;
          }
          if (y > agg_bnds.y2) {
            agg_bnds.y2 = y;
          }
        }
      }
    }
    bounds->X = (REAL)agg_bnds.x1;
    bounds->Y = (REAL)agg_bnds.y1;
    bounds->Width = (REAL)(agg_bnds.x2 - agg_bnds.x1);
    bounds->Height = (REAL)(agg_bnds.y2 - agg_bnds.y1);
  }
  else {
    *bounds = RectF(0, 0, 0, 0);
  }
  return(0);
}
static int
GraphicsPath_GetPathWorldBoundsI(GraphicsPath* path, GpRect* bounds,
    GDIPCONST GpMatrix* matrix, GDIPCONST GpPen* pen)
{
  GpRectF fBnds;
  Status Stts = GraphicsPath_GetPathWorldBounds(path, &fBnds, matrix, pen);
  bounds->X = (INT)fBnds.X;
  bounds->Y = (INT)fBnds.Y;
  bounds->Width = (INT)fBnds.Width;
  bounds->Height = (INT)fBnds.Height;
  return Stts;
}
static int
GraphicsPath_IsVisiblePathPoint(GraphicsPath* path, REAL x, REAL y,
    GpGraphics* graphics, BOOL* result)
{
  apf_SafeCheckParameter(path);
  agg::rasterizer_scanline_aa<> ras;
  typedef agg::conv_curve<agg::path_storage> conv_crv_type;
  agg::path_storage p_copy(path->m_agg_ps);
  conv_crv_type p3(p_copy);
  ras.add_path(p3);
  *result = (BOOL) ras.hit_test((int)x, (int)y);
  return 0;
}
static int
GraphicsPath_IsVisiblePathPointI(GraphicsPath* path, INT x, INT y,
    GpGraphics* graphics, BOOL* result)
{
  return GraphicsPath_IsVisiblePathPoint(path, (REAL) x, (REAL) y, graphics, result);
}
/*
static int
GraphicsPath_IsOutlineVisiblePathPoint(GraphicsPath* path, REAL x, REAL y, GpPen *pen,
                              GpGraphics *graphics, BOOL *result);
static int
GraphicsPath_IsOutlineVisiblePathPointI(GraphicsPath* path, INT x, INT y, GpPen *pen,
                               GpGraphics *graphics, BOOL *result);
*/
#endif

