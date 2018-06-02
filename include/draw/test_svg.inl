#include "svg_parser.inl"
int test_svg()
{
  STRTREE mm = {0};
  svgfile sf[1] = {0};
  char* xmlfn;
  xmlfn = "D:/code/c/Í¼ÐÎÍ¼Ïñ/Anti-Grain Geometry/svg_test/tiger.svg";
  xmlfn = "D:/code/c/Í¼ÐÎÍ¼Ïñ/Anti-Grain Geometry/svg_test/lion.svg";
  svg_parser(sf, xmlfn);
  return 0;
}

typedef struct test_showsvg_t {
  UI_engine ui[1];
  COLORREF clr;
} test_showsvg_t;

int test_showsvg_event_proc(test_showsvg_t* g, event* e)
{
  UI_engine* ui = g->ui;

  if (e->msg == EVENT_INIT) {
  }

  if (e->msg == EVENT_LCLICK) {
  }

  return 0;
}

int test_showsvg_paint_proc(test_showsvg_t* g, HDDC hDC)
{
  RECT rc;
  UI_engine* ui = g->ui;
  UIGetWorkArea(ui, &rc);

  {
    DPOINT pt[1000];
    int lens[10];
    int i, n, pplen;
    path_point pp[100] = {
      m_path_move_to(23.728, 31.152),
      m_path_curve4(-1.234, 0.42, -2.547, 0.658, -3.922, 0.658),
#if 0
      m_path_curve4(-6.724, 0, -12.177, -5.453, -12.177, -12.176),
      m_path_curve4(0, -2.552, 0.794, -4.912, 2.136, -6.867),
      m_path_vline_to(6.215),
      m_path_curve4(5.686, 9.274, 3.04, 14.142, 3.04, 19.634),
      m_path_curve4(0, 9.26, 7.507, 16.765, 16.766, 16.765),
      m_path_curve4(2.988, 0, 5.792, -0.785, 8.222, -2.153),
      m_path_hline_to(0.003),
      m_path_vline_to(-0.004),
      m_path_vline_to(-5.648),
      m_path_vline_to(-3.206),
      m_path_vline_to(15.057),
      m_path_line_to(-4.303, 4.577),
      m_path_vline_to(31.152),
#endif
      m_path_close_polygon(),
      //10    <path fill="#CB1E1D" d="M19.806,2.867c-3.059,0-5.927,0.824-8.396,2.256l0,0v5.706v2.94v11.22l4.284-4.462V8.182
      //11      c1.287-0.459,2.667-0.726,4.111-0.726c6.727,0,12.178,5.451,12.178,12.178c0,2.569-0.808,4.938-2.169,6.9v6.544
      //12      c4.1-3.057,6.76-7.936,6.76-13.444C36.574,10.373,29.067,2.867,19.806,2.867z"/>
      //13    <polygon fill="#CB1E1D" points="21.846,8.923 17.703,13.511 17.703,30.344 21.846,26.168  "/>
    };
    const char* sss =
        "M23.728,31.152c-1.234,0.42-2.547,0.658-3.922,0.658c-6.724,0-12.177-5.453-12.177-12.176"
        "c0-2.552,0.794-4.912,2.136-6.867V6.215C5.686,9.274,3.04,14.142,3.04,19.634c0,9.26,7.507,16.765,16.766,16.765"
        "c2.988,0,5.792-0.785,8.222-2.153h0.003v-0.004v-5.648v-3.206V15.057l-4.303,4.577V31.152z"
        "M19.806,2.867c-3.059,0-5.927,0.824-8.396,2.256l0,0v5.706v2.94v11.22l4.284-4.462V8.182"
        "c1.287-0.459,2.667-0.726,4.111-0.726c6.727,0,12.178,5.451,12.178,12.178c0,2.569-0.808,4.938-2.169,6.9v6.544"
        "c4.1-3.057,6.76-7.936,6.76-13.444C36.574,10.373,29.067,2.867,19.806,2.867z"
        "M21.846,8.923L17.703,13.511L17.703,30.344L21.846,26.168z"
        ;
    pplen = countof(pp);
    pplen = vcgen_path_str(sss, pp, countof(pp));

    for (i = 0; i < pplen; ++i) {
      pp[i].x += 20, pp[i].y += 20;
    }

    for (i = 0; i < pplen; ++i) {
      pp[i].x *= 10, pp[i].y *= 10;
    }

    n = vcgen_path(pp, pplen, pt, lens);
    drawaa_fillpoly_solid(hDC, pt, lens, n, g->clr);
  }

  return 0;
}

int test_showsvg_frame_proc(test_showsvg_t* g, event* e)
{
  UI_engine* ui = g->ui;
  //RECT rc;
  //layout_ctrl_event(g->m_lay, e);
  return 0;
}
int test_showsvg()
{
  int aa, i = 0;
  test_showsvg_t g[1] = {0};
  UI_set(g->ui, "test_showsvg", g,
      test_showsvg_paint_proc, test_showsvg_frame_proc, test_showsvg_event_proc, 0);

  for (; WK_CLOSEED != waitkey(g->ui, 30); i += 10) {
    int kk = 128;
    aa = abs(i % (2 * kk) - kk);
    printf("%d\n", aa);
    g->clr = _SetAlpha(0xCB1E1D, aa);
    force_redraw2(g->ui);
  }

  return 0;
}
