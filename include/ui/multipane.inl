typedef struct pane_t {
  int ind;
  int d;
  int off;
  ctrl_t* c;
  IRECT rc;
} pane_t;
#define MINPANDSZ 10
#define PANE_INIT(p, IND, D, C) ((p)->ind = IND, (p)->d = D, (p)->c = C)
typedef struct multipane_t {
  ctrl_t c[1];
  pane_t* p;
  int len;
  int gap;
  int hot, ldown;
  int hit0, hit1;
  IPOINT pt;
} multipane_t;
static int multipane_set(multipane_t* s, int gap, pane_t* p, int len)
{
  s->gap = gap;
  s->p = p;
  s->len = len;
  return 0;
}
static int multipane_calcrect(multipane_t* s, IRECT rc, int pos, int x, int y, int* phit0, int* phit1)
{
  int i, ind = s->p[pos].ind, hit0 = -1;
  int sum0 = 0, sum1 = 0, cnt = 0, gap = s->gap;
  for (i = pos; i < s->len && s->p[i].ind >= ind; ++i) {
    if (s->p[i].ind == ind) {
      ++cnt;
      sum0 += s->p[i].d;
    }
  }
  if (ind & 1) {
    int ss = RCH(&rc) - (cnt - 1) * gap, y0 = rc.t, cnt1 = 0;
    for (i = pos; i < s->len && s->p[i].ind >= ind; ++i) {
      if (s->p[i].ind == ind) {
        cnt1++;
        s->p[i].rc = rc;
        s->p[i].rc.t = y0;
        if (cnt1 == cnt) {
          s->p[i].d = rc.b - y0;
        }
        else {
          s->p[i].d = (ss * s->p[i].d + sum0 / 2) / sum0;
        }
        s->p[i].rc.b = y0 + s->p[i].d;
        y0 += s->p[i].d + gap;
        if (x >= rc.l && x < rc.r && y >= s->p[i].rc.b && y <= (s->p[i].rc.b + gap)) {
          hit0 = i;
        }
      }
    }
  }
  else {
    int ss = RCW(&rc) - (cnt - 1) * gap, x0 = rc.l, cnt1 = 0;
    for (i = pos; i < s->len && s->p[i].ind >= ind; ++i) {
      if (s->p[i].ind == ind) {
        cnt1++;
        s->p[i].rc = rc;
        s->p[i].rc.l = x0;
        if (cnt1 == cnt) {
          s->p[i].d = rc.r - x0;
        }
        else {
          s->p[i].d = (ss * s->p[i].d + sum0 / 2) / sum0;
        }
        s->p[i].rc.r = x0 + s->p[i].d;
        x0 += s->p[i].d + gap;
        if (y >= rc.t && y < rc.b && x >= s->p[i].rc.r && x <= (s->p[i].rc.r + gap)) {
          hit0 = i;
        }
      }
    }
  }
  if (phit0) {
    *phit0 = hit0;
  }
  if (phit1) {
    if (hit0 >= 0) {
      int cnt = 0;
      for (i = hit0 + 1; i < s->len && s->p[i].ind >= ind; ++i) {
        if (s->p[i].ind == ind) {
          ++cnt;
          *phit1 = i;
          break;
        }
      }
    }
    else {
      *phit1 = -1;
    }
  }
  for (i = pos; i < s->len && s->p[i].ind >= ind; ++i) {
    if (s->p[i].ind == ind && (i + 1) < s->len && s->p[i + 1].ind > ind) {
      int hit2 = -1, hit3 = -1;
      multipane_calcrect(s, s->p[i].rc, i + 1, x, y, &hit2, &hit3);
      if (hit2 >= 0) {
        hit0 = hit2;
        if (phit0) {
          *phit0 = hit2;
        }
        if (phit1) {
          *phit1 = hit3;
        }
      }
    }
  }
  return hit0;
}
#define MOVEBACK(p, n, i, off) memmove(p+i+off, p+i, (n-i)*sizeof(*p))
enum DOCKSIDE { DOCKSIDE_NONE, DOCKSIDE_LEFT, DOCKSIDE_TOP, DOCKSIDE_RIGHT, DOCKSIDE_BOTTOM, DOCKSIDE_TABS };
static int multipane_add(pane_t* p, int n, int pos, int side, ctrl_t* c)
{
  if (pos < 0) {
    PANE_INIT(p, 0, 1, c);
    n = 1;
  }
  else {
    if (side) {
      switch (side) {
      case DOCKSIDE_LEFT:
        //MOVEBACK();
        break;
      }
    }
  }
  return n;
}
static LRESULT multipane_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  multipane_t* s = (multipane_t*)c->x;
  IRECT rc = c->rc;
  int i, hit0 = -1, hit1 = -1;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    s->hot = -1;
    s->hit0 = -1;
    s->hit1 = -1;
    return 0;
  }
  //sub = s->lay0->c;
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x += 120;
    mminfo->ptMinTrackSize.y += 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (1) {
      multipane_calcrect(s, rc, 0, x, y, 0, 0);
      //sub->rc = c->rc;
      //sub->redraw = 1;
      //sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_COMMAND:
    if (s) {
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
      s->hit0 = -1;
      s->hit1 = -1;
      s->ldown = FALSE;
      force_redraw(c);
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      multipane_calcrect(s, rc, 0, x, y, &hit0, &hit1);
      if (hit0 != s->hit0) {
        if (s->p[hit0].ind & 1) {
          SetCursor(LoadCursor(NULL, IDC_SIZENS));
        }
        else {
          SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        }
        s->hit0 = hit0;
        s->hit1 = hit1;
        s->pt = iPOINT(x, y);
        force_redraw(c);
        return TRUE;
      }
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      if (s->hit0 < 0) {
        multipane_calcrect(s, rc, 0, x, y, &hit0, &hit1);
        if (hit0 >= 0) {
          if (s->p[hit0].ind & 1) {
            SetCursor(LoadCursor(NULL, IDC_SIZENS));
          }
          else {
            SetCursor(LoadCursor(NULL, IDC_SIZEWE));
          }
        }
        if (s->hot != hit0) {
          s->hot = hit0;
          force_redraw(c);
        }
      }
      else {
        int d = 0, d0, d1;
        hit0 = s->hit0;
        hit1 = s->hit1;
        d0 = s->p[hit0].d;
        d1 = s->p[hit1].d;
        if (s->p[hit0].ind & 1) {
          d += s->pt.y - y;
        }
        else {
          d += s->pt.x - x;
        }
        if (d < 0) {
          int adf = 0;
        }
        d0 -= d;
        d1 += d;
        if (d0 < MINPANDSZ) {
          d1 = d0 + d1 - MINPANDSZ;
          d0 = MINPANDSZ;
        }
        if (d1 < MINPANDSZ) {
          d0 = d0 + d1 - MINPANDSZ;
          d1 = MINPANDSZ;
        }
        s->p[hit0].d = d0;
        s->p[hit1].d = d1;
        s->pt = iPOINT(x, y);
        multipane_calcrect(s, rc, 0, x, y, &hit0, &hit1);
        force_redraw(c);
        return TRUE;
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      imdraw_fill(im, _rgb(1, 1, 1));
      for (i = 0; i < s->len; ++i) {
        ctrl_t* sub = s->p[i].c;
        imdraw_rect(im, &rc, s->p[i].rc, 0, _RGB(100, 100, 100), 1, BDM_ALL);
        if (sub) {
          //sub->ctrlproc(c, sub, hwnd, WM_SIZE, wParam, lParam);
        }
      }
    }
    c->redraw = 0;
    break;
  }
  return 0;
}

