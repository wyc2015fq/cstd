#include "cstd.h"
//#include "region.inl"
#include "draw/draw2d.inl"
//#include "draw2/gdiplus.inl"
#include "imgui/imgui.inl"
#include "imgui/ctrl.inl"
//#include "imgui/tree_ctrl.inl"
//#include "graph/test_graph.inl"
typedef struct {
  window_host_t host[1];
  softgc sg[1];
  font_t fo[1];
} soft_host_t;
int soft_host_init(soft_host_t* sh, int h, int w, void* buf, int step, int cn)
{
  int ret1 = 0;
  sysio_t* io = sys_getio();
#if 0
  easydraw_font_init(sh->fo);
#else
  font_init_memraster(sh->fo, "songti", songti_12);
#endif
  io->font = sh->fo;
  soft_drv_Init(sh->host, 0, 1);
  IMINIT(sh->sg->im, h, w, buf, step, cn, 1);
  sh->host->rect = iRECT2(0, 0, w, h);
  softgc_init(sh->sg, 1);
  gc_set_softgc(sh->host->root->g, sh->sg);
  io->g = sh->host->root->g;
  ret1 = host_begin(sh->host);
  return 0;
}

typedef int (*test_draw_f)(ctrl_t* c);

int test_draw_impl(ctrl_t* c, const test_draw_f* test_fun, int funs_count, const char* s_test) {
  SYSIO;SOFTGC_ARG;
  int i;
  static int testid = 150;
  static int inited = 0;
  static double t1 = 0;
  static double times[20];
  static int ntime=0;
  static int itime=0;
  static ctrl_t c1[1] = {0};
  double t2 = 0;
  IRECT rc = c->rc, rcRight;
  iRectCutR(&rc, -250, 0, &rcRight);
  if (!inited) {
    inited = 1;
  }
  testid = BOUND(testid, 0, funs_count-1);
  c1->rc = rc;
  color_background_ctrl(c->rc, _RGB(222, 222, 222), _RGB(122, 122, 122), true, true);
  //color_background_ctrl(c->rc, ColorWhite, ColorWhite, true, true);
  //softgc_reset(sg);
  if (test_fun && test_fun[testid]) {
    utime_start(time1);
    test_fun[testid](c1);
    t1 = utime_elapsed(time1);
    QUEUE_PUSH(times, ntime, itime, t1);
    QUEUE_SUM(times, ntime, t2);
    t2/=ntime;
  }
  //softgc_reset(sg);
  gcPenAlignment(g, PenAlignmentInset);
  {
    static ctrl_t cc[10] = {0};
    i = 0;
    flexboxlay(rcRight, countof(cc), cc, 2, flex_column);
    fmtlabel_ctrl(cc + i++, "time1 %5.2fms", t2 * 1000);
    rbox_ctrl(cc + i++, "GDI+²âÊÔ", s_test, &testid);
  }
  return 0;
}
#include "get_poly.inl"
#include "test_font.inl"
//#include "test_sysfont.inl"

#include "draw/test_gdip.inl"
#include "draw/test_gdiplusui.inl"
#include "draw/agg/test_agg.inl"
#include "geo/test_spirograph.inl"
#include "geo/test_RoundedCorners.inl"
#include "Ribbon2013/test_Ribbon2013.inl"
//#include "test_hatch.inl"
//#include "test_brush.inl"
//#include "img/imgio.inl"
int test_draw()
{
  img_t im[1] = {0};
  int h = 800, w = 1200;
  soft_host_t sh[1] = {0};
  static ctrl_t cc[10] = {0};
  //test_sysfont();
  imsetsize(im, h, w, 4, 1);
  for (; sys_waitio(100) && host_begin(sh->host);) {
    imclear(im, NULL, _rgb(1, 1, 1));
    soft_host_init(sh, im->h, im->w, im->tt.data, im->s, im->c);
    if (1) {
      sh->host->root->client->rc = sh->host->rect;
      //test_scanline_boolean2(sh->host->root->client, im);
      //test_graph(sh->host->root->client, im);
      test_gdiplusui(sh->host->root->client);
      //test_gdip(sh->host->root->client);
      //test_spirograph(sh->host->root->client);
      //test_RoundedCorners(sh->host->root->client);
      //test_Ribbon2013ui(sh->host->root->client);
      //test_Ribbon2013ui(sh->host->root->client, im);
      //test_hatch(sh->host->root->client, im);
      //test_agg1(sh->host->root->client, im);
      //tictoc;
      //test_agg(sh->host->root->client, im);
      //test_brush(sh->host->root->client, im);
      //test_font(sh->host->root->client, im);
    }
    if (0) {
      FPOINT pt[] = {
        0, 0,
          4, 0,
          4, 4,
          0, 4,
          1, 1,
          1, 3,
          3, 3,
          3, 1,
      };
      int len[] = {4, 4};
      pt_scaling(countof(pt), pt, 10, 10, 0, 0);
      //imdrawaa_poly_solid(im, flip_y, NULL, pt, len, 2, _rgb(1, 0, 0));
    }
    //imwrite("C:/aaa.bmp", im);
    //imdrawaa_rect(im, NULL, 10, 10, 100, 100, 0, solid_fun, _rgb(1,0,0));
    //imdrawaa_rect(im, NULL, 10, 10, 100, 100, 0, hatch_fun, HatchStyleCross, 0, 0, _rgb(1,0,0), _rgb(0,1,0));
    show_im("asdf", im);
    host_end(sh->host);
  }
  imfree(im);
  return 0;
}

