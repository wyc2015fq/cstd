
#include "win.h"
//#include "debug.h"
#include "utime.h"
#include "chalg.h"

#include "draw/imdraw.inl"
#include "draw/imdrawaa.inl"
#include "draw/imdraw_text_raster.inl"
#include "draw/imdraw_text_vector.inl"
#include "draw/imdrawrect.inl"
#include "draw/dc.inl"
#include "ui/xwin.inl"

#include "utime.h"
#include "test_css.inl"
#if 1
#include "test_xwin.inl"
#else
#include "test_pattern_fill.inl"
#include "test_edit.inl"
#include "ui/window.inl"
#include "test_cvui.inl"
#include "test_best_fit.inl"
#include "test_svd.inl"
#include "fmtimg/fmtimg.inl"
#include "draw/imdrawimage.inl"

//#include "dib.h"
//#include "img/rgb.inl"
//#include "geo/geo.inl"
#include "test_imdrawimage.inl"
#include "test_cam.inl"
#include "test_aa_demo.inl"
#include "test_aa_test.inl"
#include "test_component_rendering.inl"
#include "test_cam_face_recog.inl"
#include "test_ArtGalleryProblem.inl"
#include "test_wykobi.inl"
#include "test_AddPointsToPolyline.inl"
#include "test_spirograph.inl"
#include "test_fractal.inl"
//#include "test_screen_magnifier.inl"
#include "test_conv_stroke.inl"
#include "test_circles.inl"
#include "fmtimg/fmtimg.inl"
//#include "test_subdiv.inl"
//#include "test_delaunay.inl"

//#include "cv/cvcontours.inl"
//#include "agg_conv_clipper.inl"

//#include "no_fit_polygon.inl"
//#include "test_clip.inl"
#include "test_alpha_gradient.inl"
#include "test_gradients.inl"
//#include "test_graph_test.inl"
#include "test_conv_contour.inl"
//#include "test_font.inl"
//#include "test_spline.inl"
//#include "test_imdrawui.inl"

//#include "../test/test_strlistfind.inl"
//#include "test_sdk.inl"
#endif

int main()
{
#if 1
  int aa = color_parse(STR1("#123"));
  MEM_CHECK_BEGIN();
  test_xwin();
  MEM_CHECK_END();
#else
  test_css();
  test_strlistfind();
  test_edit();
  test_pattern_fill();
  test_cvui();
  test_component_rendering();
  test_spirograph();
  test_aa_test();
  test_imdrawimage();
  test_cam();
  test_aa_demo();
  test_cam_face_recog();
  test_best_fit(0);
  test_svd();
  test_ArtGalleryProblem();
  test_triangle();
  test_wykobi();
  test_AddPointsToPolyline();
  test_fractal();
  test_wykobi();
  test_fractal();
  test_gradients();
  test_xwin();
  test_edit();
  test_gradients();
  test_font();
  test_conv_contour();
  test_component_rendering();
  test_graph_test();
  test_alpha_gradient();
  test_pt_sort();
  test_clip();
  test_delaunay();
  test_subdiv();
  test_circles();
  test_conv_stroke();
  test_spline();
  test_xwin();
  test_test_sdk();
#endif
  return 0;
}

