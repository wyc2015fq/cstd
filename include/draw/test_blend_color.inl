
typedef struct test_blend_color_t {
  uictrl base;
  rbox_ctrl m_method[1];
  slider_ctrl m_radius[1];
  polygon_ctrl m_shadow[1];
  RECT rc[10];
  int m_hit;
} test_blend_color_t;
#include ".\imgproc\blur.inl"
int test_blend_color_paint_proc(const test_blend_color_t* s, const event* e)
{
  static int8u g_gradient_colors[] = {
#include "g_gradient_colors.inl"
  };
  UIHDC(e);
  //SelectFontToMemDC("ËÎÌו", (int)s->m_slider[0].m_value, 0, 0);
  draw_clear(hDC, rgb(1, 0.95, 0.95));

  //draw_fillrectR(hDC1, &win->rcClient, rgb(1, 1, 1));
  if (1) {
    double t;
    utime_start(_start_time);
    {
#if 0
      DPOINT pt[10000];
      int lens[10];
      int i, n;
      path_point pp[countof(g_pp)];
      trans_perspective shadow_persp[1];
      path_bounding_rect(g_pp, countof(g_pp), pt);
      trans_perspective_init(shadow_persp);
      trans_perspective_rect_to_quad(shadow_persp, pt[0].x, pt[0].y, pt[1].x, pt[1].y, (double*)s->m_shadow->m_polygon);
      memcpy(pp, g_pp, sizeof(pp));

      for (i = 0; i < countof(g_pp); ++i) {
        trans_perspective_transform(shadow_persp, &pp[i].x, &pp[i].y);
      }

      n = vcgen_path(pp, countof(pp), pt, lens);
      drawaa_fillpoly_solid(hDC, pt, lens, n, rgb(1, 0, 0));
      //stack_blur_rgb24(im, (int)s->m_radius->m_value, (int)s->m_radius->m_value);
      //drawaa_fillpoly_solid(hDC, pt, lens, n, rgba(0, 1, 0, 0.8));
#endif
    }
    t = 1000 * utime_elapsed(_start_time);

    if (1) {
      char buf[256];
      _snprintf(buf, 256, "Time: %.3fms", t);
      draw_polygon_ctrl(s->m_shadow, e);
      draw_rbox_ctrl(s->m_method, e);
      draw_slider_ctrl(s->m_radius, e);
      draw_textR(hDC, buf, -1, &s->rc[2], 0, rgb(0, 0, 0));
    }
  }

  return 0;
}
LRESULT test_blend_color_frame_proc(test_blend_color_t* s, event* e)
{
  switch (e->msg) {
  case EVENT_INIT:
    if (s) {
    }

    break;

  case EVENT_EXIT:
    if (e->sender == s) {
      PostQuitMessage(0);
    }

    break;

  case WM_SIZE: {
    RECT rc2 = s->base.rc;
    InflateRect(&rc2, -3, -3);
    layout_itR("[0:40]:200,[1:20,2:20]", &rc2, 10, 10, s->rc);
    s->m_method->base.rc = s->rc[0];
    s->m_radius->base.rc = s->rc[1];
  }

  return 0;
  break;

  case WM_LBUTTONUP:
    if (s) {
      s->m_hit = 0;
    }

    break;

  case WM_LBUTTONDOWN:
    if (s) {
    }

    break;

  case WM_MOUSEMOVE:
    if (s->m_hit) {
      force_redraw(e);
    }

    break;

  default:
    break;
  }

  rbox_ctrl_event(s->m_method, e);
  slider_ctrl_event(s->m_radius, e);
  polygon_ctrl_event(s->m_shadow, e);
  return 0;
}
int test_blend_color()
{
  UI_engine ui[1] = {0};
  test_blend_color_t s[1] = {0};
  DPOINT pt[4];
  RECT padd[1] = {30, 40, 30, 40};
  COLORREF trans_color = _RGB(255, 0, 255);
  char* m_method_items[] = {"Single Color", "Color LUT"};
  //rbox_ctrl_set(s->m_method, 1, 2, m_method_items);
  slider_ctrl_set(s->m_radius, "Blur Radius=%1.2f", 15.0, 0.0, 40.0, 0);
  vcgen_rect(pt, 100, 400, 400, 100);
  polygon_ctrl_set(s->m_shadow, 4, pt, 0);
  UISETCALL(s, test_blend_color_paint_proc, test_blend_color_frame_proc);
  UI_set(ui, "test_blend_color", s, 0);
  //s->win->hwnd = Win_Create(0, test_blend_color_WndProc, s);
  waitkey(ui, -1);
  res_free();
  return 0;
}

