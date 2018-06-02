// 最大公约数
CC_INLINE int gcd(int a, int b)
{
  while (a != b) {
    if (a > b) {
      a = a - b;
    }
    else {
      b = b - a;
    }
  }
  return a;
}
// 最小公倍数
CC_INLINE int gbd(int a, int b)
{
  return a * b / gcd(a, b); //gcd是上面求最大公约数那个
}
// http://www.codeproject.com/Articles/476138/Spirograph-Ultrabook-Windows-8-Application
CC_INLINE int vcgen_spirograph(int n, DPOINT* pt, double x, double y, int g, int a, int b, double c)
{
  int tt, i, iter;
  double t1, t2;
  if (0 == b || 0 == a) {
    return 0;
  }
  a *= g;
  b *= g;
  t1 = a - b;
  tt = (2 * b) / gcd(a, b);
  iter = n / tt;
  if (tt == 0) {
    return 0;
  }
  n = iter * tt;
  t2 = t1 / b;
  if (iter == 0) {
    return 0;
  }
  for (i = 0; i < n; ++i) {
    double t = i * PI / iter;
    pt[i].x = x + t1 * cos(t) + c * cos(t * t2);
    pt[i].y = y + t1 * sin(t) - c * sin(t * t2);
  }
  return n;
}
typedef struct test_spirograph_t {
  ctrl_t c[1];
  null_t nul[1];
  slider_t sliG[1];
  slider_t sliA[1];
  slider_t sliB[1];
  slider_t sliC[1];
  slider_t sliD[1];
  layout_t lay1[1];
} test_spirograph_t;
static LRESULT test_spirograph_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_spirograph_t* s = (test_spirograph_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  switch (uMsg) {
  case WM_CTRLFREE:
    if (WM_CTRLFREE == uMsg) {
      c->ctrlproc = 0;
      return 0;
    }
  case WM_CTRLINIT:
    if (WM_CTRLINIT == uMsg) {
      ctrl_set(null_proc, s->nul, s->nul->c, c, WT_CHILD, NULL);
      slider_set(s->sliG, c, "G=%.f", 2, 30, 2);
      slider_set(s->sliA, c, "A=%.f", 2, 100, 2);
      slider_set(s->sliB, c, "B=%.f", 2, 100, 5);
      slider_set(s->sliC, c, "C=%.f", 2, 200, 20);
      slider_set(s->sliD, c, "D=%.f", -3, 5, 2);
      layout_set(s->lay1, c, 4, 0,
          " xx;"
          "x%-;"
          "a%-;"
          "a%-;"
          "a%-;"
          "a%-;"
          "a%-;",
          s->nul->c,
          s->sliG->c,
          s->sliA->c,
          s->sliB->c,
          s->sliC->c,
          s->sliD->c
                );
      c->sub = s->lay1->c;
      return 0;
    }
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
    mminfo->ptMinTrackSize.x += 420;
    mminfo->ptMinTrackSize.y += 420;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (s) {
      ctrl_setrect(c->sub, c->rc);
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
    }
    break;
  case WM_COMMAND:
    if (s) {
      if (XCMD_CHANGE == wParam) {
        force_redraw(c);
      }
    }
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
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int n_drawn = 0;
      imdraw_fill(im, _rgb(1, 1, 1));
      {
        //draw_fillrectR(hDC1, &win->rcClient, _rgb(1, 1, 1));
        char buf[256];
        DPOINT pt[2000];
        int n;
        {
          double t;
          utime_start(_start_time);
          DWORD clr = _rgb(0, 0, 0);
          n = vcgen_spirograph(countof(pt), pt, im->w / 2, im->h / 2, (int)s->sliG->m_value, (int)s->sliA->m_value, (int)s->sliB->m_value, s->sliC->m_value);
          imdrawaa_poly(im, &rc, pt, n, 1, 0, clr, 1);
          t = utime_elapsed(_start_time);
          _snprintf(buf, 256, "点数n=%d time=%.5f", n, t);
          //imdraw_text(im, rcClient, buf, -1, disp->drawer->font, _rgb(0, 0, 0), 0, TT_VCENTER);
        }
      }
      //imdrawui_default_polygon(uidrawer, im, m_poly1);
      //imdrawui_default_polygon(uidrawer, im, m_poly2);
    }
    break;
  }
  return 0;
}

