

//#define MSGPROCARGDEF(TYPE)  TYPE* s, IRECT rc, const xmsg_t* msg, xctrlinfo_t* ci, int cid, img_t* im, const uistyle_t* g_scr

int lay_data(int x)
{
  return x & 0xffff;
}
int lay_type(int x)
{
  return x >> 24;
}
int lay_node_off(int n, int d)
{
  d = lay_data(n) + d;
  d = BOUND(d, 0, 10000);
  return laynode(lay_type(n), d);
}

int xlayout_set(xlayout_t* s, int rows, int cols, int* id, int gapxy, int marginxy)
{
  s->gap = iSIZE(gapxy, gapxy);
  s->margin = iSIZE(marginxy, marginxy);
  s->rows = rows, s->cols = cols, s->id = id;
  return 0;
}

typedef struct xtreelay_node_t {
  int ind, d, id;
} xtreelay_node_t;

int xtreelay_node_calcrect(IRECT rc, int n, const xtreelay_node_t* nd, xctrlinfo_t* ci, ISIZE gap, ISIZE margin)
{
  int i = 1, j, k, maxind = 0, ind = 0, nr = 0, sumd, cnt, cntx, x, y, h, w, d;
  IRECT rc2;
  struct st_node_t {
    IRECT rc;
    int i;
  } r[32];
  rc = iRectOffset(rc, margin.w, margin.h, -margin.w, -margin.h);
  r[nr].rc = rc, r[nr].i = 0, ++nr;

  for (; nr > 0;) {
    --nr, rc = r[nr].rc, i = r[nr].i;
    ind = nd[i].ind;
    sumd = 0, cnt = 0, cntx = 0, k = 0;
    x = rc.l, y = rc.t;
    w = RCW(&rc), h = RCH(&rc);

    for (j = i + 1; j < n && nd[j].ind > ind; ++j) {
      if (nd[j].ind == (ind + 1)) {
        ++cnt;

        if (nd[j].d > 0) {
          sumd += nd[j].d;
        }
        else {
          ++cntx;
        }
      }
    }

    if (layh == nd[i].id || layv == nd[i].id) {
      int w2 = w - sumd - (cnt - 1) * gap.w;
      int h2 = h - sumd - (cnt - 1) * gap.h;

      for (j = i + 1; j < n && nd[j].ind > ind; ++j) {
        if (nd[j].ind == (ind + 1)) {
          if (layh == nd[i].id) {
            if (nd[j].d > 0) {
              d = x + nd[j].d;
            }
            else {
              ++k;
              //d = k==cntx ? rc.r : (x + w2/cntx);
              d = (x + w2 / cntx);
            }

            rc2 = iRECT(x, y, d, y + h);
            x = d + gap.w;
          }
          else {
            if (nd[j].d > 0) {
              d = y + nd[j].d;
            }
            else {
              ++k;
              //d = k==cntx ? rc.b : (y + h2/cntx);
              d = (y + h2 / cntx);
            }

            rc2 = iRECT(x, y, x + w, d);
            y = d + gap.h;
          }

          if (layv == nd[j].id || layh == nd[j].id) {
            r[nr].rc = rc2, r[nr].i = j, ++nr;
          }
          else {
            ci[nd[j].id].rc = rc2;
          }
        }
      }
    }
    else {
      ci[nd[i].id].rc = rc;
    }
  }

  return 0;
}

typedef struct xtreelay_t {
  ISIZE gap;
  ISIZE margin;
  xtreelay_node_t* tr;
  int ntr;
  int hit, hot;
  IPOINT hitbd;
  IPOINT lastdownpt;
  int ldown;
} xtreelay_t;

int xtreelay_set(xtreelay_t* s, int gapxy, int marginxy, int n, xtreelay_node_t* tr)
{
  s->gap.h = gapxy, s->gap.w = gapxy;
  s->margin.h = gapxy, s->margin.w = gapxy;
  s->ntr = n;
  s->tr = tr;
  return 0;
}

int xtreelay_msg_proc(MSGPROCARGDEF(xtreelay_t))
{
  int i;
  int size_changed = 0;
  int ret = 0;

  if (im) {
    if (ci[cid].redraw) {
      for (i = 0; i < s->ntr; ++i) {
        int d = s->tr[i].id;

        if (d >= 0) {
          ci[d].redraw = 1;
        }
      }
    }

    xtreelay_node_calcrect(rc, s->ntr, s->tr, ci, s->gap, s->margin);

    for (i = 0; i < s->ntr; ++i) {
      int d = s->tr[i].id;

      if (d >= 0) {
        xctrl_msg_proc(msg, ci, d, im, g_scr);
      }
    }

    ci[cid].redraw = 0;
  }
  else if (msg) {
    for (i = 0; i < s->ntr; ++i) {
      int d = s->tr[i].id;

      if (d >= 0) {
        xctrl_msg_proc(msg, ci, d, im, g_scr);
      }
    }

    switch (msg->msg) {
    case WM_INIT:
      if (s) {
        s->hit = -1;
        s->hot = -1;
        s->hitbd = iPOINT(-1, -1);
        force_redraw();
      }

      break;

    case WM_SIZE:
      if (s) {
        force_redraw();
      }

      break;

    case WM_LBUTTONUP:
      if (s) {
        s->hit = 0;
        s->ldown = FALSE;
        s->hitbd = iPOINT(-1, -1);
      }

      break;

    case WM_LBUTTONDBLCLK:
    case WM_LBUTTONDOWN:
      if (iPtInRect(&rc, x, y)) {
        s->ldown = TRUE;
        s->lastdownpt = iPOINT(x, y);
      }

      break;

    case WM_MOUSEMOVE:
      if (iPtInRect(&rc, x, y)) {
      }

      break;
    }

    if (size_changed) {
      for (i = 0; i < s->ntr; ++i) {
        int d = s->tr[i].id;

        if (d >= 0) {
          xctrl_size_proc(ci, d, im, g_scr);
        }
      }
    }

  }

  return FALSE;
}

#endif
