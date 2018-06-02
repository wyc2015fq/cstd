
typedef struct test_mdi_t {
  uictrl base;
  UI_engine ui[1];
  uimdi mdi[1];
  button_ctrl btn[1];
  test_evalayout_t te[256];
  colorpicker_ctrl cp[1];
} test_mdi_t;
int test_mdi_paint_proc(const test_mdi_t* s, const event* e)
{
  UIHDC(e);
  draw_clear(hDC, _RGB(0, 0, 0));
  draw_clear(hDC, _RGB(255, 255, 255));
  //draw_uimdi(s->mdi, e);
  draw_button_ctrl(s->btn, e);
  //test_packing(hDC);

  if (0) {
    utime_start(_start_time);
    draw_colorpicker_ctrl(s->cp, hDC, s->base.rc);
    printf("%f\n", utime_elapsed(_start_time));
  }

  if (1) {
    //CCustomDrawHeaderCtrl hd;
    //RECT rc1 = s->base.rc, rc = s->base.rc;
    //hd.nColCount = 5;
    //RectCutTop(&rc1, 20, 0, &rc);
    //hd.header_ctrl_paint(hDC, rc);
  }

  //test_draweffects(hDC, s->base.rc);
  return 0;
}
int test_mdi_frame_proc(test_mdi_t* s, const event* e)
{
  UIEVT(e);
  char buf[256];
  RECT rc;
  RECT rect;

  switch (e->msg) {
  case WM_MOUSEMOVE:
  case EVENT_LCLICK:
    if (e->sender == s->btn) {
      if (0) {
        static int tt = 3;
        test_evalayout_t* te;
        UIGetWorkArea(ui, &rect);
        tt++;
        te = s->te + tt;
        printf("EVENT_LCLICK %d\n", e->wp);
        _snprintf(buf, 256, "win %d", tt);
        SetRect(&rc, tt * 20, tt * 20, tt * 20 + 300, tt * 20 + 200);
        UISETCALL(te, test_evalayout_paint_proc, test_evalayout_frame_proc);
        uimdi_addwin(s->mdi, e, te, buf, rc, WS_BORDER | WS_CAPTION);
      }
      else {
      }
    }

    break;

  case WM_SIZE:
    if (1) {
      int d = 50;
      UIGetWorkArea(s->ui, &rect);
      s->btn->base.rc = iRECT(rect.right - 100, 10, rect.right - 10, 40);
      RCOFFSET(&rect, 0, d, 0, 0);
      s->mdi->base.rc = rect;
      s->cp->base.rc = rect;
    }

    break;

  case EVENT_EXIT:
    if (e->sender == s) {
      printf("EVENT_EXIT %x\n", e->sender);
      PostQuitMessage(0);
    }

    break;

  case EVENT_INIT:
    if (e->sender == s) {
      Win_MoveCenter(s->ui->hwnd, 800, 600);
    }

    break;
  }

  uimdi_event(s->mdi, e);
  button_ctrl_event(s->btn, e);
  colorpicker_ctrl_event(s->cp, e);
  return 0;
}

int test_mdi()
{
  test_mdi_t s[1] = {0};

  UISETCALL(s, test_mdi_paint_proc, test_mdi_frame_proc);
  colorpicker_ctrl_set(s->cp);
  s->cp->m_COLORTYPE = HEX;
  button_ctrl_set(s->btn, "new");
  UI_set(s->ui, "test_mdi", &s->base, 0);
  waitkey(s->ui, -1);
  res_free();
  return 0;
}
