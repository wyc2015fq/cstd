
//#include "gdiplus.inl"
//#include "draw/imdraw.inl"
#include "polymorphic_renderer.inl"
#include "aa_demo.inl"
#include "aa_test.inl"
#include "circles.inl"
#include "alpha_gradient.inl"
#include "bezier_div.inl"
#include "conv_contour.inl"
#include "parse_lion.inl"
//#include "agg_svg_path_tokenizer.inl"
//#include "agg_svg_path_renderer.inl"
#include "lion.inl"
#include "conv_stroke.inl"
#include "rasterizers.inl"
#include "conv_dash_marker.inl"
#include "test_scanline_boolean2.inl"
#include "agg_svg_parser.inl"
#include "svg_test.inl"

#if 0
#include "alpha_mask.inl"
#include "alpha_mask2.inl"
#include "alpha_mask3.inl"
#include "blend_color.inl"
#include "blur.inl"
#include "bspline.inl"
#include "component_rendering.inl"
#include "compositing.inl"
#include "compositing2.inl"
#include "distortions.inl"
#include "flash_rasterizer.inl"
#include "flash_rasterizer2.inl"
#include "freetype_test.inl"
#include "gamma_correction.inl"
#include "gamma_ctrl.inl"
#include "gamma_tuner.inl"
#include "gouraud.inl"
#include "gouraud_mesh.inl"
#include "gpc_test.inl"
#include "gradients.inl"
#include "gradient_focal.inl"
#include "graph_test.inl"
#include "idea.inl"
#include "image1.inl"
#include "image_alpha.inl"
#include "image_filters.inl"
#include "image_filters2.inl"
#include "image_fltr_graph.inl"
#include "image_perspective.inl"
#include "image_resample.inl"
#include "image_transforms.inl"
#include "interactive_polygon.inl"
#include "line_patterns.inl"
#include "line_patterns_clip.inl"
#include "make_arrows.inl"
#include "make_gb_poly.inl"
#include "mol_view.inl"
#include "multi_clip.inl"
#include "pattern_fill.inl"
#include "pattern_perspective.inl"
#include "pattern_resample.inl"
#include "perspective.inl"
#include "pure_api.inl"
#include "rasterizers2.inl"
#include "rasterizer_compound.inl"
#include "raster_text.inl"
#include "rounded_rect.inl"
#include "scanline_boolean.inl"
#include "scanline_boolean2.inl"
#include "simple_blur.inl"
#include "trans_curve1.inl"
#include "trans_curve1a.inl"
#include "trans_curve1_ft.inl"
#include "trans_curve2.inl"
#include "trans_curve2_ft.inl"
#include "trans_polar.inl"
#include "truetype_test.inl"
#endif

int draw_gouraud_triangle(softgc* sg, double x1, double y1, double x2, double y2, double x3, double y3, COLOR c1, COLOR c2, COLOR c3) {
  c1 = _rgb(1,0,0);
  c2 = _rgb(0,1,0);
  c3 = _rgb(0,0,1);
  brush_set_gouraud(sg->brush, x1, y1, x2, y2, x3, y3, c1, c2, c3);
  return scanline_set_triangle(sg->sl, x1, y1, x2, y2, x3, y3);
}

int test_agg1(ctrl_t* c) {
  SYSIO;SOFTGC_ARG;
  //IRECT rc = c->rc, rc1 = iRECT2(100, 100, 100, 100);
  //int w = im->w, h = im->h;
  //gcSolidRect2(g, rc, ColorBlack);
  //gcSolidCircle(g, 100, 100, 50, 0, ColorRed, 2);
  //scanline_setSmoothingMode(sg, SmoothingModeNone);
  //gcSolidText(g, rc1, "asdfasdf", NULL, io->font, 0, TF_CENTER, ColorBlack);
  //draw_gouraud_triangle(sg, 709.02490, 269.02044, 766.09368, 134.93719, 599.38274, 277.52743, 0xa4a2d81d, 0xa499a159, 0xad194140);
  brush_set_solid(sg->brush, _rgb(1, 0, 0));
  //scanline_bin_circle(sg->sl, NULL, 100, 200, 50, true);
  //scanline_bin_ellipse(sg->sl, 100, 100, 500, 200, 1, 0);
  //brush_set_solid(sg->brush, _rgb(0, 0, 1));
  //scanline_bin_ellipse(sg->sl, 100, 100, 500, 200, 0, 3);
  //scanline_bin_line2(sg->sl, NULL, 100, 100, 500, 200);
  return 0;
}

#define TESTAGGDEF_DEF(TESTDEF) \
TESTDEF(test_agg1) \
  TESTDEF(polymorphic_renderer) \
  TESTDEF(alpha_gradient) \
  TESTDEF(bezier_div) \
  TESTDEF(conv_contour) \
  TESTDEF(lion) \
  TESTDEF(conv_stroke) \
  TESTDEF(aa_test) \
  TESTDEF(rasterizers) \
  TESTDEF(conv_dash_marker) \
  TESTDEF(test_scanline_boolean2) \
  TESTDEF(aa_demo) \
  TESTDEF(circles) \
  TESTDEF(svg_test) \

  
int test_agg(ctrl_t* c) {
  const char* s_test = "|"
#define TESTDEF(FUN)   #FUN "|"
    TESTAGGDEF_DEF(TESTDEF)
#undef TESTDEF
    ;
  const test_draw_f test_agg_fun[] = {
#define TESTDEF(FUN)   FUN,
    TESTAGGDEF_DEF(TESTDEF)
#undef TESTDEF
  };
  test_draw_impl(c, test_agg_fun, countof(test_agg_fun), s_test);
  //tictoc(NULL);
  return 0;
}

