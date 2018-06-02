
typedef struct test_imshop_t {
  uictrl base;
  UI_engine ui[1];
  img_t im[1];
  POINT org;
  int hit;
  POINT downpt;
  //RECT rcsub[3];
  scroll sc[1];
} test_imshop_t;
int draw_image_pt(HDDC hDC, RECT rc, POINT org, const img_t* imlist)
{
  img_t im[1] = {0};
  RECT rc2 = rc;
  OffsetRect(&rc2, -org.x, -org.y);
  imsubref(imlist, rc2, im);

  rc2.left = MAX(rc.left, org.x);
  rc2.top = MAX(rc.top, org.y);
  rc2.right = rc2.left + im->width;
  rc2.bottom = rc2.top + im->height;
  draw_imageR(hDC, &rc2, im, 0, 0, 0);
  return 0;
}
//WM_VSCROLL SB_LINELEFT
//GetScrollInfo
int test_imshop_hitrect(test_imshop_t* s)
{
  //RECT rect = *prc;
  //RCOFFSET(&rect, 10, 10, -10, -10);
  //s->rcsub[0] = rect;
  return 0;
}
#define RCMAX(r1, r2)  iRECT(MIN((r1)->left, (r1)->left), MIN((r1)->top, (r1)->top), MAX((r1)->right, (r1)->right), MAX((r1)->bottom, (r1)->bottom))
int test_imshop_paint_proc(const test_imshop_t* s, const event* e)
{
  UIHDC(e);
  draw_clear(hDC, res->clrBtnFace);
  //drawaa_scroll(s->sc, e, prc);

  draw_scroll(s->sc, e);
  draw_image_pt(hDC, s->sc->rcsub[SCHT_RCV], s->org, s->im);

  return 0;
}

#define RCNORM4(l, t, r, b)  iRECT(MIN(l, r), MIN(t, b), MAX(l, r), MAX(t, b))
#define RCNORM(rc)  RCNORM4((rc)->left, (rc)->top, (rc)->right, (rc)->bottom)

int test_imshop_setscroll(test_imshop_t* s)
{
  SIZE imsz = imsize(s->im);
  RECT rca = iRECT3(s->org, imsz);
  RECT rcv = s->sc->rect;
  RectIn(rcv, 30, &rcv);
  scroll_getrect(s->sc, rca, rcv);
  s->org = RCLT(&s->sc->rcsub[SCHT_RCA]);
  //printf("%d \n", s->org.y);
  //printf("%d %d %d %d %d\n", s->sc->rcsub[SCHT_THUMBVERT].bottom, s->sc->rcsub[SCHT_LINEDOWN].top,
  //  s->sc->rcsub[SCHT_RCA].bottom, s->sc->rcsub[SCHT_RCV].bottom, s->sc->rcsub[SCHT_RCS].bottom);
  return 0;
}

int test_imshop_frame_proc(test_imshop_t* s, const event* e)
{
  UI_engine* ui = e->ui;

  if (scroll_event(s->sc, e, s)) {
    return 1;
  }

  switch (e->msg) {
  case EVENT_EXIT:
    if (e->sender == s) {
      PostQuitMessage(0);
      return 1;
    }

    break;

  case EVENT_INIT:
    if (e->sender == s) {
      Win_MoveCenter(e->ui->hwnd, 400, 300);
      return 1;
    }

    break;

  case EVENT_SCROLL_SETPOS:
    if (1) {
      if (e->wp < 8 && e->wp >= 0) {
        POINT wwp[8] = {0};
        int lx = 40, ly = 40, px = RCW(&s->sc->rcsub[SCHT_RCV]), py = RCH(&s->sc->rcsub[SCHT_RCV]);
        wwp[SCHT_LINEUP] = cPOINT(0, ly);
        wwp[SCHT_LINELEFT] = cPOINT(lx, 0);
        wwp[SCHT_LINEDOWN] = cPOINT(0, -ly);
        wwp[SCHT_LINERIGHT] = cPOINT(-lx, 0);

        wwp[SCHT_PAGEUP] = cPOINT(0, py);
        wwp[SCHT_PAGELEFT] = cPOINT(px, 0);
        wwp[SCHT_PAGEDOWN] = cPOINT(0, -py);
        wwp[SCHT_PAGERIGHT] = cPOINT(-px, 0);
        PTADD(s->org, s->org, wwp[e->wp]);
      }
      else {
        s->org = RCLT(&s->sc->rcsub[SCHT_RCA]);
      }

      if (e->lp) {
        test_imshop_setscroll(s);
      }

      force_redraw(e);
    }

    break;

  case EVENT_LCLICK:
    if (1) {
      printf("EVENT_LCLICK %d %d\n", 0, e->wp);
      return 1;
    }

    break;

  case WM_LBUTTONUP:
    s->hit = 0;
    break;

  case WM_SIZE:
    s->sc->rect = s->base.rc;
    test_imshop_hitrect(s);
    break;

  case WM_LBUTTONDOWN:
    if (PtInRect(&s->sc->rcsub[SCHT_CLIENT], e->pt)) {
      s->hit = 1;
      PTSUB(s->downpt, s->org, e->pt);
      force_redraw(e);
      return TRUE;
    }

    break;

  case WM_MOUSEMOVE:
    if (s->hit) {
      PTADD(s->org, s->downpt, e->pt);
      test_imshop_setscroll(s);
      force_redraw(e);
    }

    break;
  }

  return 0;
}

int test_imshop()
{
  test_imshop_t s[1] = {0};
  DWORD crt = _GetSysColor(COLOR_BTNFACE);

  GetImage(s->im, "D:\\pub\\pic\\lena1.jpg", 0);
  imsetalpha(s->im, 255);
  UISETCALL(s, test_imshop_paint_proc, test_imshop_frame_proc);
  UI_set(s->ui, "test_imshop", &s->base, 0);
  //ShowWindow(s->ui->hwnd, SW_SHOW);

  waitkey(s->ui, -1);
  res_free();
  return 0;
}

