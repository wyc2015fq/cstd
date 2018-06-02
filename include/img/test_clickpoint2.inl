
typedef struct test_clickpoint2_t {
  uictrl base;
  UI_engine ui[1];
  RECT rect;
  DPOINT m_polygon[200];
  int m_num_points;
  double m_point_radius;
  int hit;
  int hot;
  POINT m_pt;
} test_clickpoint2_t;
int test_clickpoint2_paint_proc(const test_clickpoint2_t* s, const event* e0)
{
  UIHDC(e0);
  //UITOER(s);
  draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));
  draw_clear(hDC, rgb(1, 1, 1));
  {
    int i;
    RECT rc1, rc = s->rect;
    DPOINT pt[100];
    int lens[1];
    DWORD _clr;
    char buf[256];
    ColorT m_color;
    ColorT m_node_color;
    double m_stroke_width = 1.0;
    int m_close = 1;
    HDDC_BEGIN(hDC, &rc);
    m_color = (rgba(0.0, 0.0, 0.0, 0.8));
    m_node_color = rgba(0.0, 0.5, 0.0, 0.5);
    drawaa_strokepoly(hDC, s->m_polygon, s->m_num_points, m_close, m_color, m_stroke_width);

    for (i = 0; i < s->m_num_points; ++i) {
      double r = i == s->hit ? s->m_point_radius * 1.2 : s->m_point_radius;
      lens[0] = vcgen_ellipse(s->m_polygon[i].x, s->m_polygon[i].y, r, r, 32, 0, pt);
      _clr = (i == s->hot) ? rgba(0.7, 0.0, 0.0, 0.5) : m_node_color;
      drawaa_fillpoly_solid(hDC, pt, lens, 1, _clr);
      rc1 = iRECT(s->m_polygon[i].x + 10, s->m_polygon[i].y - 10, s->m_polygon[i].x + 100, s->m_polygon[i].y + 10);
      _snprintf(buf, 256, "%d", i);
      _clr = rgb(0, 0, 0);
      draw_textR2(hDC, buf, rc1, DT_VCENTER);
    }

    HDDC_END(hDC);
  }
  return 0;
}
int test_clickpoint2_frame_proc(test_clickpoint2_t* s, const event* e)
{
  UIEVT(e);

  //UITOER(s);
  switch (e->msg) {
  case WM_LBUTTONUP:
    if (1) {
      s->hit = -1;
    }

    break;

  case WM_RBUTTONDBLCLK:
  case WM_RBUTTONDOWN:
    s->m_num_points = MAX(0, s->m_num_points - 1);
    //printf("WM_RBUTTONDOWN\n");
    break;

  case WM_MBUTTONDOWN:
    if (PtInRect(&s->rect, e->pt)) {
      int hit = polygon_ctrl_hittest(s, e->pt.x, e->pt.y);
      s->hit = hit;
      s->m_pt = e->pt;
      force_redraw(e);
    }

    break;

  case WM_LBUTTONDOWN:
    if (PtInRect(&s->rect, e->pt)) {
      int hit = polygon_ctrl_hittest(s, e->pt.x, e->pt.y);
      s->hit = hit;
      s->m_pt = e->pt;

      if (hit < 0 || hit >= s->m_num_points) {
        s->m_polygon[s->m_num_points].x = e->pt.x;
        s->m_polygon[s->m_num_points].y = e->pt.y;
        s->hit = s->hot = s->m_num_points;
        s->m_num_points++;
      }

      force_redraw(e);
    }

    break;

  case WM_MOUSEMOVE:
    if (s->hit < 0) {
      int hit = polygon_ctrl_hittest(s, e->pt.x, e->pt.y);

      if (hit >= 0 && hit < s->m_num_points && s->hot != hit) {
        s->hot = hit;
        force_redraw(e);
      }

      s->hot = hit;
    }
    else {
      int i;
      double dx, dy;
      dx = e->pt.x - s->m_pt.x;
      dy = e->pt.y - s->m_pt.y;

      if (s->hit < s->m_num_points) {
        int i = s->hit;
        s->m_polygon[i].x += dx;
        s->m_polygon[i].y += dy;
      }

      if ((MK_MBUTTON & e->wp) && s->hit == s->m_num_points) {
        for (i = 0; i < s->m_num_points; i++) {
          s->m_polygon[i].x += dx;
          s->m_polygon[i].y += dy;
        }
      }

      if (0 && s->hit >= (1 << 16)) {
        int n1 = s->hit & 0xffff;
        int n2 = (n1 + s->m_num_points - 1) % s->m_num_points;
        s->m_polygon[n1].x += dx;
        s->m_polygon[n1].y += dy;
        s->m_polygon[n2].x += dx;
        s->m_polygon[n2].y += dy;
      }

      s->m_pt.x = e->pt.x;
      s->m_pt.y = e->pt.y;
      force_redraw(e);
      return true;
    }

    break;

  }

  return 0;
}

int test_clickpoint2()
{
  test_clickpoint2_t s[1] = {0};
  int frame = 0;
  s->m_point_radius = 5;
  s->hit = s->hot = -1;

  UISETCALL(s, test_clickpoint2_paint_proc, test_clickpoint2_frame_proc);
  UI_set(s->ui, "test_clickpoint2", s, 0);

  for (; WK_CLOSEED != waitkey(s->ui, 10); ++frame) {
    force_redraw2(s->ui);
  }

  FreeMemDC();
  return 0;
}

