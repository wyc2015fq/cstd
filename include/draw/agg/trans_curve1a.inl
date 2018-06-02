#include "cstd.h"
#include "draw/draw2d.inl"
#include "draw/path.inl"
#include "trans_single_path.inl"

const char* text = 
"Anti-Grain Geometry is designed as a set of loosely coupled "
"algorithms and class templates united with a common idea, "
"so that all the components can be easily combined. Also, "
"the template based design allows you to replace any part of "
"the library without the necessity to modify a single byte in "
      "the existing code. ";

int test_trans_curve1() {
  FPOINT m_poly[] = {50,50,150 + 20,150 - 20,250 - 20,250 + 20,350 + 20,350 - 20,450 - 20,450 + 20,550,550};
  FPOINT pt[100];
  vertex_dist vpt[100];
  int len = countof(pt);
  img_t im[1] = {0};
  imsetsize(im, 800, 1200, 4, 1);
  imclear(im, NULL, ColorWhite);
  vcgen_bspline(countof(pt), pt, countof(m_poly), m_poly, 0);
  imdrawaa_poly_stroke_solid(im, 0, NULL, pt, len, false, ColorBlack, 1);
  {
    ttffont_t f[1] = {0};
    Path path[1] = {0};
    const char* fontname = "Times New Roman";
    sys_font_set(f, fontname, 0);
    PathAddStringA(path, text, -1, f, 0, 40/f->info->size, 0, 0, 0, NULL, NULL);
    trans_single_path(len, pt, 1120, 1, path->Points, path->Count);
    spanv_t sp[1] = {0};
    PathFlatten1(path, NULL, 0.1);
    spanv_set_path(sp, im, NULL, path->Points, path->Types, path->Count);
    spanv_solid(sp, im, 0, ColorBlack);
    spanv_free(sp);
  }
  imshow(im);
  cvWaitKey(-1);
  return 0;
}

#include "trans_double_path.inl"

int test_trans_curve2() {
  FPOINT m_poly1[] = {10 + 50,-10 + 50,10 + 150 + 20,-10 + 150 - 20,10 + 250 - 20,-10 + 250 + 20,10 + 350 + 20,-10 + 350 - 20,10 + 450 - 20,-10 + 450 + 20,10 + 550,-10 + 550,};
  FPOINT m_poly2[] = {-10 + 50,10 + 50,-10 + 150 + 20,10 + 150 - 20,-10 + 250 - 20,10 + 250 + 20,-10 + 350 + 20,10 + 350 - 20,-10 + 450 - 20,10 + 450 + 20,-10 + 550,10 + 550,};
  FPOINT pt1[1000];
  FPOINT pt2[1000];
  vertex_dist vpt[100];
  int len = countof(pt1);
  img_t im[1] = {0};
  imsetsize(im, 800, 1200, 4, 1);
  imclear(im, NULL, ColorWhite);
  vcgen_bspline(countof(pt1), pt1, countof(m_poly1), m_poly1, 0);
  vcgen_bspline(countof(pt2), pt2, countof(m_poly2), m_poly2, 0);
  imdrawaa_poly_stroke_solid(im, 0, NULL, pt1, len, false, ColorBlack, 1);
  imdrawaa_poly_stroke_solid(im, 0, NULL, pt2, len, false, ColorBlack, 1);
  {
    ttffont_t f[1] = {0};
    Path path[1] = {0};
    const char* fontname = "Times New Roman";
    sys_font_set(f, fontname, 0);
    PathAddStringA(path, text, -1, f, 0, 40/f->info->size, 0, 0, 0, NULL, NULL);
    trans_double_path(countof(pt1), pt1, countof(pt2), pt2, 1140, 30, true, path->Points, path->Count);
    spanv_t sp[1] = {0};
    PathFlatten1(path, NULL, 0.1);
    spanv_set_path(sp, im, NULL, path->Points, path->Types, path->Count);
    spanv_solid(sp, im, 0, ColorBlack);
    spanv_free(sp);
  }
  imshow(im);
  cvWaitKey(-1);
  return 0;
}
