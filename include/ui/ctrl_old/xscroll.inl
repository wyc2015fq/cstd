
#define SCW1  16
// 最小宽度
#define SCW2  10

enum {
  SCHT_LINE_U,
  SCHT_LINE_D,
  SCHT_LINE_L,
  SCHT_LINE_R,

  SCHT_PAGE_U,
  SCHT_PAGE_D,
  SCHT_PAGE_L,
  SCHT_PAGE_R,

  SCHT_THUMB_H,
  SCHT_THUMB_V,

  SCHT_CLIENT,
  SCHT_GRIPPER,

  SCHT_RCA,
  SCHT_RCV,
  SCHT_RCC,
  SCHT_RCS,
};
//SCROLLINFO
typedef struct xscroll_t {
  IRECT rcsub[16];
  IPOINT pt;
  int hit;
  int id;
} xscroll_t;

int xscroll_set(xscroll_t* s, int id)
{
  s->id = id;
  return 0;
}

// rca  内容区域
// rcv  可视区域
// rcc  滚动条区域
// szsc 滚动条宽高
int getscrollrect(IRECT rca, IRECT rcv, IRECT rcc, ISIZE szsc, int fmt, IRECT* out)
{
  IRECT rc0, rc1, rc2, rc3 = {0}, a;
  int dx = 0, dy = 0;
  ISIZE sza = RCSZ(&rca), szv = RCSZ(&rcv);
  memset(out, 0, 16 * sizeof(IRECT));

  if (sza.h <= szv.h && sza.w <= szv.w) {
    dx = dy = 0;
  }
  else {
    dx = ((sza.w > szv.w) && ((sza.h + szsc.h) <= szv.h)) ? 0 : szsc.w;
    dy = ((sza.h > szv.h) && ((sza.w + szsc.w) <= szv.w)) ? 0 : szsc.h;
  }

  rcv = iRectOffset(rcv, 0, 0, -dx, -dy);
  rc1 = iRectOffset(rcv, -rcv.l, -rcv.t, -rcv.l, -rcv.t);
  rcv = rc1;
  iRectCover(rca, rc1, &rca, fmt);
  //iRectOffset(rca, -rcv.l, -rcv.t, -rcv.l, -rcv.t, &rca);
  rc2 = iRectOffset(rcc, dy, dx, -dx - dy, -dx - dy);
  a = iRectZoom(&rc2, &rc1, &rca);
  rc0 = rcc;
  out[SCHT_RCA] = rca;
  out[SCHT_RCV] = rcv;
  out[SCHT_RCC] = rc2;
  out[SCHT_RCS] = a;

  if (dx) {
    iRectCutL(&rc0, -dx, 0, &rc1);
    iRectCutT(&rc1, -dy, 0, &rc3);
    iRectCutT(&rc1, dx, 0, out + SCHT_LINE_U);
    iRectCutT(&rc1, -dx, 0, out + SCHT_LINE_D);
    iRectCutT(&rc1, a.t - rc2.t, 0, out + SCHT_PAGE_U);
    iRectCutT(&rc1, a.b - rc2.b, 0, out + SCHT_PAGE_D);
    out[SCHT_THUMB_V] = rc1;
  }

  if (dy) {
    iRectCutT(&rc0, -dy, 0, &rc1);
    iRectCutL(&rc1, dy, 0, out + SCHT_LINE_L);
    iRectCutL(&rc1, -dy, 0, out + SCHT_LINE_R);
    iRectCutL(&rc1, a.l - rc2.l, 0, out + SCHT_PAGE_L);
    iRectCutL(&rc1, a.r - rc2.r, 0, out + SCHT_PAGE_R);
    out[SCHT_THUMB_H] = rc1;
  }

  out[SCHT_CLIENT] = rc0;
  out[SCHT_GRIPPER] = rc3;

  return 0;
}

int xscroll_getrect(xscroll_t* s, IRECT rca, IRECT rcv)
{
  ISIZE szsc = iSIZE(SCW1, SCW1);
  getscrollrect(rca, rcv, rcv, szsc, 0, s->rcsub);
  return 0;
}

int xscroll_hittest(const xscroll_t* s, int x, int y)
{
  int i;

  for (i = 0; i < 10; ++i) {
    if (iPtInRect(s->rcsub + i, x, y)) {
      return i;
    }
  }

  return -1;
}

int xscroll_msg_proc(MSGPROCARGDEF(xscroll_t))
{
  IPOINT pt = iPOINT(msg->x, msg->y);

  if (im) {
  }
  else if (msg) {
    switch (msg->msg) {
    case MSG_INIT:
      if (s) {
        IRECT rc2 = iRectOffset(rc, -100, -100, 100, 100);
        xscroll_getrect(s, rc2, rc);
        force_redraw(msg);
        s->hit = -1;
      }

      break;

    case MSG_SIZE:
      if (s) {
        IRECT rc2 = iRectOffset(rc, -100, -100, 100, 100);
        xscroll_getrect(s, rc2, rc);
        force_redraw(msg);
      }

      break;

    case MSG_MOUSEMOVE:
      if (s->hit >= 0) {
        if (SCHT_THUMB_V == s->hit || SCHT_THUMB_H == s->hit) {
          IRECT rcc = s->rcsub[SCHT_RCC];
          IRECT rcs = s->rcsub[SCHT_RCS];
          IRECT rca = s->rcsub[SCHT_RCA];
          IRECT rcv = s->rcsub[SCHT_RCV];
          ISIZE szs = RCSZ(&rcs);
          ISIZE sza = RCSZ(&rca);
          pt = ipt2_add(s->pt, pt);

          if (SCHT_THUMB_V == s->hit) {
            //s->rcsub[SCHT_PAGE_U].t = s->rcsub[SCHT_LINE_U].b;
            //s->rcsub[SCHT_PAGE_D].b = s->rcsub[SCHT_LINE_D].t;
            s->rcsub[SCHT_PAGE_U].b = s->rcsub[SCHT_THUMB_V].t = rcs.t = BOUND(pt.y, rcc.t, rcc.b - szs.h);
            s->rcsub[SCHT_PAGE_D].t = s->rcsub[SCHT_THUMB_V].b = rcs.b = rcs.t + szs.h;
          }
          else {
            //s->rcsub[SCHT_PAGE_L].l = s->rcsub[SCHT_LINE_L].r;
            //s->rcsub[SCHT_PAGE_R].r = s->rcsub[SCHT_LINE_R].l;
            s->rcsub[SCHT_PAGE_L].r = s->rcsub[SCHT_THUMB_H].l = rcs.l = BOUND(pt.x, rcc.l, rcc.r - szs.w);
            s->rcsub[SCHT_PAGE_R].l = s->rcsub[SCHT_THUMB_H].r = rcs.r = rcs.l + szs.w;
          }

          rca = iRectZoom(&rcv, &rcc, &rcs);

          if (rcv.t - rca.t < rca.b - rcv.b) {
            rca.b = rca.t + sza.h;
          }
          else {
            rca.t = rca.b - sza.h;
          }

          if (rcv.l - rca.l < rca.r - rcv.r) {
            rca.r = rca.l + sza.w;
          }
          else {
            rca.l = rca.r - sza.w;
          }

          iRectCover(rca, rcv, &rca, 0);
          //iRectOffset(rca, -rcv.l, -rcv.t, -rcv.l, -rcv.t, &rca);
          s->rcsub[SCHT_RCS] = rcs;
          s->rcsub[SCHT_RCA] = rca;
          //printf("%d, %d\n", rca.b, ppt->y);
          //UISendEvent(msg, s, subctrl, MSG_SCROLL_SETPOS, s->hit, 0);
          force_redraw(msg);
        }
      }

      break;

    case MSG_LBUTTONUP:
      if (s->hit >= 0) {
        s->hit = -1;
        //UISendEvent(msg, s, subctrl, MSG_SCROLL_SETPOS, s->hit, 1);
        force_redraw(msg);
      }

      break;

    case MSG_LBUTTONDBLCLK:
    case MSG_LBUTTONDOWN:
      if (s) {
        int hit = xscroll_hittest(s, msg->x, msg->y);

        if (hit >= 0) {
          if (hit != s->hit) {
            IRECT cs = s->rcsub[SCHT_RCS];
            s->hit = hit;

            if (SCHT_THUMB_H == hit || SCHT_THUMB_V == hit) {
              s->pt = ipt2_sub(RCLT(&cs), pt);
            }
            else {
              //UISendEvent(msg, s, subctrl, MSG_SCROLL_SETPOS, s->hit, 1);
              force_redraw(msg);
            }
          }
        }
      }

      break;
    }
  }

  return 0;
}
