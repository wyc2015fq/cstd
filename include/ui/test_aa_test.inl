CC_INLINE double frand(double x)
{
  return ((((rand() << 15) | rand()) & 0x3FFFFFFF) % 1000000) * x / 1000000.0;
}
// Calculate the affine transformation matrix for the linear gradient
// from (x1, y1) to (x2, y2). gradient_d2 is the "base" to scale the
// gradient. Here d1 must be 0.0, and d2 must equal gradient_d2.
CC_INLINE int calc_linear_gradient_transform(double x1, double y1, double x2, double y2, double* mtx, double gradient_d2/* = 100.0*/)
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  double s = sqrt(dx * dx + dy * dy) / gradient_d2;
  double at = atan2(dy, dx);
  double m[9] = {0};
  affine_init(mtx);
  affine_multiply(mtx, affine_scaling(m, s, s));
  affine_multiply(mtx, affine_rotation(m, at));
  affine_multiply(mtx, affine_translation(m, x1 + 0.5, y1 + 0.5));
  affine_invert(mtx);
  return 0;
}
static int test_asdf(img_t* im, IRECT rc)
{
  if (0) {
    // 定义一个线性渐变画刷，按红黄蓝绿的顺序四种颜色渐变
    COLOR colors[] = {_RGB(255, 0, 0), _RGB(255, 255, 0), _RGB(0, 0, 255), _RGB(0, 255, 0)};
    double positions[] = {0.0, 0.33, 0.66, 1.0};
    COLOR lut[256] = {0};
    fill_color_array2(256, lut, 4, colors, positions);
    // 填充指定区域矩形
    imdraw_rect_gradient_lut(im, 0, rc, rc.l, rc.t, rc.l, rc.b, lut);
  }
  if (1) {
    int i;
    DPOINT points[] = { 175, 0, 200, 50, 250, 50, 212, 75, 250, 150, 175, 100,
        100, 150, 137, 75, 100, 50, 150, 50
                      };
    // define the changing path
    COLOR colors[] = {_RGB(0, 0, 0), _RGB(0, 255, 0), _RGB(0, 0, 255), _RGB(255, 255, 255), _RGB(0, 0, 0), _RGB(255, 255, 255),
        _RGB(0, 0, 255), _RGB(255, 255, 255), _RGB(0, 0, 0), _RGB(0, 255, 0)
                     };
    double pos[10] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1};
    ASSERT(countof(points) == countof(colors));
    imdrawaa_poly_path_gouraud(im, 0, points, countof(points), colors, _RGB(255, 0, 0), 0);
    //imdrawaa_poly_path_gradient(im, 0, points, countof(points), colors, pos, 10, 0);
    for (i = 0; i < countof(points); ++i) {
      imdrawaa_ellipse(im, 0, points[i].x, points[i].y, 5, 5, _A_RGB(200, colors[i]), 0, 0);
    }
  }
  return 0;
}
static int test_aa_test_paint(img_t* im, int tt)
{
  int i, k = 6;
  //int gamma[aa_scale];
  //RECT rc;
  //RECT rcClient;
  int h = im->h, w = im->w, step = im->s, cn = im->c;
  uchar* img = im->tt.data;
  double x1, y1, x2, y2;
  //rc.top+=s->m_slider_gamma->m_rect.bottom;
  imdraw_fill(im, _rgb(0, 0, 0));
  tt = 4;
  if (0) {
    DPOINT pt[] = {
      10, 10,
      10, 50,
      50, 50,
      50, 10,
      20, 20,
    };
    int n = countof(pt);
    imdrawaa_poly_solid(im, 0, pt, &n, 1, _RGB(255, 0, 0));
    return 0;
  }
  //tt=1;
  // gamma correction
  //ras.gamma(agg::gamma_power());
  //m_gamma.gamma(m_slider_gamma.value());
  // radial line test
  if (0 == tt) {
    COLOR clrL[256];
    COLOR clrT[256];
    COLOR clrR[256];
    COLOR clrB[256];
    int ww = MIN(w / 3, 128), hh = MIN(h / 3, 128);
    int nL = 128 + rand() % ww, nT = 128 + rand() % hh, nR = 128 + rand() % ww, nB = 128 + rand() % hh;
    fill_color_array(nL, clrL, _rgb(frand(1.0), frand(1.0), frand(1.0)), _rgb(frand(1.0), frand(1.0), frand(1.0)));
    fill_color_array(nT, clrT, _rgb(frand(1.0), frand(1.0), frand(1.0)), _rgb(frand(1.0), frand(1.0), frand(1.0)));
    fill_color_array(nR, clrR, _rgb(frand(1.0), frand(1.0), frand(1.0)), _rgb(frand(1.0), frand(1.0), frand(1.0)));
    fill_color_array(nB, clrB, _rgb(frand(1.0), frand(1.0), frand(1.0)), _rgb(frand(1.0), frand(1.0), frand(1.0)));
    imdraw_border(im, 0, iRECT(0, 0, im->w, im->h), 0, nL, clrL, nT, clrT, nR, clrR, nB, clrB);
  }
  if (1 == tt) {
    double cx = (w / 2.);
    double cy = (h / 2.);
    COLOR color = (_rgba(1.0, 1.0, 1.0, 0.5));
    for (i = 180; i > 0; i--) {
      double aa = 2.0 * PI * i / 180.0;
      x1 = (cx + MIN(cx, cy) * sin(aa));
      y1 = (cy + MIN(cx, cy) * cos(aa));
      imdrawaa_dashed_line(im, 0, x1, y1, cx, cy, 1.0, (i < 90) ? i : 0.0, color);
      //imdrawaa_lined(im, x1, y1, cx, cy, color, 1.);
    }
  }
  if (1 == tt) {
    DPOINT pt[1000];
    int len[10];
    COLOR gradient_colors[256];
    double gradient_mtx[9];
    for (i = 1; i <= 20; i++) {
      COLOR color = (_rgb(1, 1, 1));
      int cw = 1;
      // integral point sizes 1..20
      imdrawaa_ellipse(im, 0, 20 + i * (i + 1) + 0.5, 20.5, i / 2.0, i / 2.0, color, 0, 0);
      // fractional point sizes 0..2
      imdrawaa_ellipse(im, 0, 18 + i * 4 + 0.5, 33 + 0.5, i / 20.0, i / 20.0, color, 0, 0);
      // fractional point positioning
      imdrawaa_ellipse(im, 0, 18 + i * 4 + (i - 1) / 10.0 + 0.5, 27 + (i - 1) / 10.0 + 0.5, 0.5, 0.5, color, 0, 0);
      // integral line widths 1..20
      fill_color_array(256, gradient_colors, _rgb(1, 1, 1), _rgb(i % 2, (i % 3) * 0.5, (i % 5) * 0.25));
      x1 = (20 + i * (i + 1));
      y1 = (40.5);
      x2 = (20 + i * (i + 1) + (i - 1) * 4);
      y2 = (100.5);
      calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx, 100);
      imdrawaa_line_gradient(im, 0, x1, y1, x2, y2, _rgb(1, 1, 1), _rgb(i % 2, (i % 3) * 0.5, (i % 5) * 0.25), i);
      fill_color_array(256, gradient_colors, _rgb(1, 0, 0), _rgb(0, 0, 1));
      // fractional line lengths H (red/blue)
      x1 = 17.5 + i * 4;
      y1 = 107;
      x2 = 17.5 + i * 4 + i / 6.66666667;
      y2 = 107;
      calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx, 100);
      imdrawaa_line_gradient(im, 0, x1, y1, x2, y2, _rgb(1, 0, 0), _rgb(0, 0, 1), 1.0);
      // fractional line lengths V (red/blue)
      x1 = 18 + i * 4;
      y1 = 112.5;
      x2 = 18 + i * 4;
      y2 = 112.5 + i / 6.66666667;
      calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx, 100);
      imdrawaa_line_gradient(im, 0, x1, y1, x2, y2, _rgb(1, 0, 0), _rgb(0, 0, 1), 1.0);
      // fractional line positioning (red)
      fill_color_array(256, gradient_colors, _rgb(1, 0, 0), _rgb(1, 1, 1));
      x1 = 21.5;
      y1 = 120 + (i - 1) * 3.1;
      x2 = 52.5;
      y2 = 120 + (i - 1) * 3.1;
      calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx, 100);
      imdrawaa_line_gradient(im, 0, x1, y1, x2, y2, _rgb(1, 0, 0), _rgb(1, 1, 1), 1.0);
      // fractional line width 2..0 (green)
      fill_color_array(256, gradient_colors, _rgb(0, 1, 0), _rgb(1, 1, 1));
      x1 = 52.5;
      y1 = 118 + i * 3;
      x2 = 83.5;
      y2 = 118 + i * 3;
      calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx, 100);
      imdrawaa_line_gradient(im, 0, x1, y1, x2, y2, _rgb(0, 1, 0), _rgb(1, 1, 1), 2.0 - (i - 1) / 10.0);
      // stippled fractional width 2..0 (blue)
      fill_color_array(256, gradient_colors, _rgb(0, 0, 1), _rgb(1, 1, 1));
      x1 = 83.5;
      y1 = 119 + i * 3;
      x2 = 114.5;
      y2 = 119 + i * 3;
      calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx, 100);
      //imdrawaa_line_gradientd(im, x1, y1, x2, y2, _rgb(0,0,1), _rgb(1,1,1), 2.0 - (i - 1) / 10.0, 3.0);
      color = (_rgb(1, 1, 1));
      if (i <= 10) {
        // integral line width, horz aligned (mipmap test)
        imdrawaa_dashed_line(im, 0, 125.5, 119.5 + (i + 2) * (i / 2.0), 135.5, 119.5 + (i + 2) * (i / 2.0), i, 0.0, color);
      }
      // fractional line width 0..2, 1 px H
      imdrawaa_dashed_line(im, 0, 17.5 + i * 4, 192, 18.5 + i * 4, 192, i / 10.0, 0, color);
      // fractional line positioning, 1 px H
      imdrawaa_dashed_line(im, 0, 17.5 + i * 4 + (i - 1) / 10.0, 186, 18.5 + i * 4 + (i - 1) / 10.0, 186, 1.0, 0, color);
    }
    // Triangles
    for (i = 1; i <= 13; i++) {
      fill_color_array(256, gradient_colors, _rgb(1, 1, 1), _rgb(i % 2, (i % 3) * 0.5, (i % 5) * 0.25));
      calc_linear_gradient_transform(w - 150, h - 20 - i * (i + 1.5), w - 20, h - 20 - i * (i + 1), gradient_mtx, 100);
      pt[0] = dPOINT(w - 150, h - 20 - i * (i + 1.5));
      pt[1] = dPOINT(w - 20, h - 20 - i * (i + 1));
      pt[2] = dPOINT(w - 20, h - 20 - i * (i + 2));
      len[0] = 3;
      //imdrawaa_poly_gradient(im, 0, pt, len, 1, _rgb(0,1,0), _rgb(1,1,1), 2.0 - (i - 1) / 10.0);
      //imdrawaa_poly_gradient(im, 0, pt, len, 1, w - 150, pt[1].y, w - 20, pt[1].y,_rgb(0,1,0), _rgb(1,1,1));
      //imdrawaa_poly_solid(im, 0, pt, len, 1, _rgb(0,1,0));
    }
  }
  {
    RECT rc[10] = {0};
    //layout_it("[[,0,1,]:20,2:20,,[,3:200]:150]", 4, rcClient.top, w - 8, h - rcClient.top - 10, 10, 10, rc);
    //s->m_slider_gamma->base.rect = rc[0];
    //drawaa_slider_ctrl(im, s->m_slider_gamma);
  }
  if (1) {
    double t1 = 0, t2 = 0, t3 = 0;
    double x1, y1, x2, y2, x3, y3;
    char buf[256];
    COLOR clr1, clr2, clr3;
    COLOR gradient_colors[256];
    utime_start(_start_time);
    if (2 == tt) {
      utime_restart(_start_time);
      for (i = 0; i < 1000; i++) {
        double r = frand(40.0) + 1.0;
        clr1 = _rgba(frand(1.0), frand(1.0), frand(1.0), 0.5 + frand(0.5));
        imdrawaa_ellipse(im, 0, frand(w), frand(h), r / 2, r / 2, clr1, 0, 0);
      }
      t1 = utime_elapsed(_start_time);
    }
    if (3 == tt) {
      utime_restart(_start_time);
      for (i = 0; i < 1000; i++) {
        x1 = frand(w);
        y1 = frand(h);
        x2 = x1 + frand(w * 0.5) - w * 0.25;
        y2 = y1 + frand(h * 0.5) - h * 0.25;
        clr1 = _rgba(frand(1.0), frand(1.0), frand(1.0), 0.5 + frand(0.5));
        clr2 = _rgba(frand(1.0), frand(1.0), frand(1.0), frand(1.0));
        fill_color_array(256, gradient_colors, clr1, clr2);
        //calc_linear_gradient_transform(x1, y1, x2, y2, gradient_mtx);
        imdrawaa_dashline_gradient(im, 0, x1, y1, x2, y2, clr1, clr2, 10.0, 0, 0, 0);
      }
      t2 = utime_elapsed(_start_time);
    }
    srand(0);
    if (4 == tt) {
      utime_restart(_start_time);
      for (i = 0; i < 1000; i++) {
        x1 = frand(w);
        y1 = frand(h);
        x2 = x1 + frand(w * 0.4) - w * 0.2;
        y2 = y1 + frand(h * 0.4) - h * 0.2;
        x3 = x1 + frand(w * 0.4) - w * 0.2;
        y3 = y1 + frand(h * 0.4) - h * 0.2;
        clr1 = _rgba(frand(1.0), frand(1.0), frand(1.0), 0.5 + frand(0.5));
        clr2 = _rgba(frand(1.0), frand(1.0), frand(1.0), frand(1.0));
        clr3 = _rgba(frand(1.0), frand(1.0), frand(1.0), frand(1.0));
        imdrawaa_triangle_gouraud(im, 0, x1, y1, x2, y2, x3, y3, clr1, clr2, clr3);
      }
      t3 = utime_elapsed(_start_time);
    }
    _snprintf(buf, 256, "Points=%.4fms, Lines=%.4fms, Triangles=%.4fms", t1, t2, t3);
    {
#if 0
      CvFont font[1] = {0};
      double sc = 0.5;
      cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX, sc, sc, 0, 1);
      imdraw_text_v(im, buf, 10, 100, font, _rgb(1, 1, 1));
#endif
    }
    //printf("%s\n", buf);
  }
  return 0;
}
//#include "draw/js1k/pleaseDontHotlinkMe.inl"
typedef struct test_aa_test_t {
  ctrl_t c[1];
  int hit;
  int tt;
  tabctrl_t tab[1];
} test_aa_test_t;
static LRESULT test_aa_test_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static test_aa_test_t s[1] = {0};
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    free(s);
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    static tab_item ti[] = {
      {0, 0, "item 0"},
      {1, 0, "item 1"},
      {2, 0, "item 2"},
      {3, 0, "item 3"},
      {4, 0, "item 4"},
      {5, 0, "item 5"},
      {6, 0, "item 6"},
    };
    c->x = s;
    tabctrl_set(s->tab, c, c);
    tabctrl_additems(s->tab, countof(ti), ti);
    return 0;
  }
  c->sub = s->tab->c;
  switch (uMsg) {
  case WM_GETMINMAXINFO:
    if (1) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      mminfo->ptMinTrackSize.x = 400;
      mminfo->ptMinTrackSize.y = 300;
      //printf("WM_MOUSEMOVE\n");
    }
    break;
  case WM_TIMER:
    force_redraw(c);
    break;
  case WM_NOTIFY:
    if ((ctrl_t*)wParam == s->tab->c) {
      s->tt = s->tab->hit;
      force_redraw(c);
    }
    break;
  case WM_LBUTTONUP:
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
    }
    break;
  case WM_KEYDOWN:
    if (1) {
      if (' ' == wParam) {
        s->tt++;
        s->tt %= 5;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      ctrl_setrect(c->sub, c->rc);
    }
    if (c->redraw || 1) {
      img_t* im = (img_t*)wParam;
      img_t im1[1] = {0};
      IRECT clientin;
      tabctrl_calc_rect(s->tab, 0, 0, &clientin, 0);
      imsubref(im, clientin, im1);
      if (s->tab->hit >= 0 && s->tab->hit < s->tab->nitem) {
        s->tt = s->tab->items[s->tab->hit].id;
      }
      s->tt = 1;
      test_aa_test_paint(im1, s->tt);
      c->redraw = 0;
    }
    break;
  }
  return 0;
}

