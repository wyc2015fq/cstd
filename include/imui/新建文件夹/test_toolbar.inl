
#define BAR_NUM  14

typedef struct test_toolbar_t {
  uictrl base;
  UI_engine ui[1];
  toolbar_ctrl bar[14];
  RECT rctxt[BAR_NUM];
} test_toolbar_t;
int test_toolbar_hitrect(test_toolbar_t* s)
{
  int i;
  RECT rc = s->base.rc;
  RCOFFSET(&rc, 10, 10, -10, -10);

  for (i = 0; i < BAR_NUM; i++) {
    RectCutTop(&rc, 30, 4, &s->bar[i].base.rc);
    RectCutLeft(&s->bar[i].base.rc, 200, 0, s->rctxt + i);
  }

  return 0;
}
int test_toolbar_paint_proc(const test_toolbar_t* s, const event* e)
{
  UIHDC(e);
  int i;
  char buf[256];
  draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));

  for (i = 0; i < BAR_NUM; ++i) {
    _snprintf(buf, 256, "%2d : %s", i, P_GetStyleString(i));
    draw_textR(hDC, buf, strlen(buf), s->rctxt + i, DT_VCENTER, _RGB(0, 0, 0));
    draw_toolbar_ctrl(s->bar + i, e);
  }

  return 0;
}
int test_toolbar_frame_proc(test_toolbar_t* s, const event* e)
{
  int i;

  for (i = 0; i < BAR_NUM; ++i) {
    if (toolbar_ctrl_event(s->bar + i, e)) {
      return 1;
    }
  }

  switch (e->msg) {
  case EVENT_EXIT:
    if (e->sender == s) {
      PostQuitMessage(0);
    }

    break;

  case EVENT_INIT:
    if (e->sender == s) {
    }

    break;

  case WM_SIZE:
    if (1) {
      test_toolbar_hitrect(s);
    }

    break;

  case EVENT_LCLICK:
    if (1) {
      toolbar_ctrl* tb = (toolbar_ctrl*)e->sender;
      printf("EVENT_LCLICK %d %d\n", tb - s->bar, e->wp);
      return 1;
    }

    break;
  }

  return 0;
}

int test_toolbar()
{
  test_toolbar_t s[1] = {0};
  DWORD crt = _GetSysColor(COLOR_BTNFACE);
  int i, n;
  tbbutton tbb[20] = {0};
  crt = 0xffc0c0c0;

#define RESDEF(type, id, str)  resload(#type, id, #str);
  //#include "test_listctrl_resdef.txt"
  n = RESDEF(image, 0, (. / res / Toolbar2.bmp, 0xffc0c0c0, t, 16, 16, 0, 40));
#undef RESDEF

  for (i = 0; i < n; i++) {
    tbb[i].nImage = i;
  }

  for (i = 0; i < BAR_NUM; i++) {
    toolbar_ctrl_set(s->bar + i);
    toolbar_ctrl_addbuttons(s->bar + i, n, tbb);
    s->bar[i].button_style = i;
    s->bar[i].border_style = TB_BDR_RAISED;
  }

  UISETCALL(s, test_toolbar_paint_proc, test_toolbar_frame_proc);
  UI_set(s->ui, "test_toolbar", &s->base, 0);
  //ShowWindow(s->ui->hwnd, SW_SHOW);

  waitkey(s->ui, -1);
  res_free();
  return 0;
}

