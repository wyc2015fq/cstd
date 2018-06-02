#include "multipane.inl"
typedef struct test_multipane_t {
  ctrl_t c[1];
  multipane_t mp0[1];
  DockingMarkers ma1[1];
  Timer tm[1];
  IRECT rc1;
  int hot;
} test_multipane_t;
static TabCtrl tab0[1];
static int test_multipane_OnTimer(struct Timer* timer)
{
  test_multipane_t* s = (test_multipane_t*)timer->user;
  int w = RCW(&s->rc1), h = RCH(&s->rc1);
  ctrl_t* c = s->c;
  //HWND hwnd = c->hwnd;
  switch (timer->id) {
  case 10:
    printf("---h = %d, w = %d\n", h, w);
    if (w < 100) {
      ++w;
    }
    else {
      w = 100;
      myKillTimer(s->tm);
      mySetTime(s->tm, test_multipane_OnTimer, s, 11, 30);
    }
    break;
  case 11:
    if (h < 100) {
      ++h;
      printf("h = %d, w = %d\n", h, w);
    }
    else {
      h = 100;
      myKillTimer(s->tm);
    }
    break;
  }
  s->rc1 = iRECT2(s->rc1.l, s->rc1.t, w, h);
  force_redraw(c);
  return 0;
}
static LRESULT test_multipane_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_multipane_t* s = (test_multipane_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  IRECT rc1 = s->rc1;
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    static pane_t pane[10] = {0};
    pane_t* p = pane;
    PANE_INIT(p, 0, 1, 0), ++p;
    PANE_INIT(p, 1, 1, 0), ++p;
    PANE_INIT(p, 2, 1, 0), ++p;
    PANE_INIT(p, 2, 1, 0), ++p;
    PANE_INIT(p, 1, 1, 0), ++p;
    PANE_INIT(p, 0, 1, 0), ++p;
    PANE_INIT(p, 0, 1, 0), ++p;
    ctrl_set(multipane_proc, s->mp0, s->mp0->c, c, WT_CHILD, NULL);
    ctrl_set(TabCtrl_proc, tab0, tab0->c, c, WT_CHILD, NULL);
    multipane_set(s->mp0, 4, pane, p - pane);
    TabCtrl_set(tab0);
    {
      int i;
      static Tab tab[10] = {0};
      static char* aaa[] = {
        "adsfadsf 1",
        "adsfadsf 2",
        "adsfadsf 3",
        "adsfadsf 4",
        "adsfadsf 5",
        "adsfadsf 6",
        "adsfadsf 7",
        "adsfadsf 8"
      };
      tab0->m_vtrtabs = tab;
      for (i = 0; i < countof(aaa); ++i) {
        TabCtrl_Add(tab0, 0, aaa[i], -1);
      }
      TabCtrlStyle_VS2010_client_custom1_set(tab0);
      s->rc1 = iRECT(100, 100, 140, 140);
    }
    c->sub = s->mp0->c;
    c->sub = tab0->c;
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
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
  case WM_COMMAND:
    if (s) {
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
      ctrl_close(s->ma1->c);
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->ma1->m_Layout = MarkersLayoutA;
      if (s->ma1->m_Layout) {
        iRectAlign(rc, s->ma1->m_Layout->m_iWidthTotal, s->ma1->m_Layout->m_iHeightTotal, DT_CENTER | DT_VCENTER, &rc1);
        //myClientToScreen(c->sys->hwnd, &rc1.l, &rc1.t);
        //win_set(c->sys, DockingMarkers_proc, s->ma1, s->ma1->c, c, 0, WT_POPUP, rc1.l, rc1.t, 0, 0);
      }
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
      if (iPtInRect(&rc1, x, y)) {
        if (!s->hot) {
          s->hot = 1;
          mySetTime(s->tm, test_multipane_OnTimer, s, 10, 30);
        }
      }
      else {
        if (s->hot) {
          s->hot = 0;
          myKillTimer(s->tm);
        }
      }
#if 0
      if (s->ma1->c->hwnd) {
        myClientToScreen(hwnd, &x, &y);
        myScreenToClient(s->ma1->c->hwnd, &x, &y);
        DockingMarkers_OnMouseMove(s->ma1, x, y);
      }
#endif
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      //printf("w = %d\n", RCW(&rc1));
      imdraw_rect(im, &rc, rc, _rgba(1, 1, 1, 1), 0, 0, 0);
      imdraw_rect(im, &rc, rc1, _rgba(1, 0, 0, 1), 0, 0, 0);
      c->redraw = 0;
    }
    break;
  }
  return 0;
}

