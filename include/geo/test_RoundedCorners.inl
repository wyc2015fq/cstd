
#include "RoundedCorners.inl"

static void drawPath(gc_t* g, const Path* path, Color color)
{
  IRECT rc;
  PathGetBoundsI(path, &rc, NULL, NULL);
  gcGradientBrushRectS2(g,rc, color, ColorWhite, LinearGradientModeVertical);
  gcPen(g, ColorBlack, 5);
  gcPath2(g, GcOptFillStroke, path);
}

static void drawFigure(gc_t* g, FPOINT* points, int count, double radius)
{
  Path path[1] = {0};
  PathAddPolygon(path, points, count, true);
  drawPath(g, path, ColorBlue);
  PathReset(path);
  PathAddRoundPoly(path, points, count, true, radius);
  //PathPrint(path);
  PathTranslate(path, 0, 200);
  drawPath(g, path, ColorRed);
  PathFree(path);
}

int test_RoundedCorners(ctrl_t* c) {
  SYSIO;SOFTGC_ARG;
  FPOINT butterfly[9] = {
  10,10,
    30,20,
    50,10,
    30,30,
    50,50,
    30,40,
    10,50,
    30,30,
    10,10,
};
FPOINT roundedRectangle[5] = {
  60,15,
    100,15,
    100,45,
    60,45,
    60,15};
  double radius = 40;
  pt_scaling(countof(butterfly), butterfly, 5, 5, 0, 0);
  pt_scaling(countof(roundedRectangle), roundedRectangle, 5, 5, 0, 0);
  drawFigure(g, butterfly, countof(butterfly), radius);
  drawFigure(g, roundedRectangle, countof(roundedRectangle), radius);
  return 0;
}
