
#include "Renderers/RibbonRenderer.inl"
#include "draw/path.inl"
#include "Enums/Enums.inl"
#include "Component/RibbonContext.inl"
#include "Renderers/RibbonProfessionalRenderer.inl"
//#include "draw/brush2.inl"
#include "draw/draw3d.inl"

int test_Ribbon2013() {
  int x = cstr_hex2int("112233aa", 8);
  char str[16];
  cstr_int2hex(str, 0, x, 8);
  return 0;
}

int test_Ribbon2013ui(ctrl_t* c) {
  SYSIO;SOFTGC_ARG;
  Path path[10] = {0};
  static ctrl_t cc[10] = {0};
  int i = 0;
  ctrl_t* m_pathctrl = cc + i++;
  IRECT rc = c->rc, rc1, rc2;
  m_pathctrl->rc = rc;
  rc1 = iRECT2(100, 100, 200, 200);
  rc2 = iRECT2(0, 300, 500, 200);
  ColorTableDefault();
  gcSolidRect(g, 0, 0, 800, 800, ColorBlack);
  //RoundRectangle(path, rc1, 50, CornersAll);
  path_ctrl2(c, path, 0.1);
  //CreateCompleteTabPath_2013(path, rc1, rc2);
  //PathPrint(path);
  //sg->brush->wrapX = WrapModeTileFlip;
  //DrawTabActive(g, rc1, RibbonOrbStyleOffice_2010, NULL);
  DrawTabSelected(g, rc1);
  //DrawTabNormal(g, rc1, RibbonOrbStyleOffice_2013);
  //  brush_set_path_radial2(sg->brush, 200, 200, 50, 100, ColorRed, ColorBlue);
  //scanline_set_ellipse(sg->sl, 200, 200, 200, 200);
  //path_ctrl2(m_pathctrl, path, 0.2);
  //scanline_set_path_stroke2(sg->sl, path, NULL);
  //GradientRect(g, rc1, ColorRed, ColorBlue);
  //DrawArrow(g, rc1, ColorRed, RibbonArrowDirectionUp);
  //DrawArrowShaded(g, rc1, RibbonArrowDirectionUp, true);
  //DrawGripDot(g, iPOINT(200, 200));
  PathFrees(path, countof(path));
  return 0;
}

