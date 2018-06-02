
typedef struct test_menu_t {
  uictrl base;
  UI_engine ui[1];
  uipopup pop[1];
  menu_ctrl menu1[1];
  menu_ctrl menu2[1];
  RECT rcsub[10];
  slider_ctrl si[1];
  rbox_ctrl rb[1];
} test_menu_t;
int test_menu_paint_proc(const test_menu_t* s, const event* e)
{
  UIHDC(e);
  RECT rc = s->base.rc;

  RCOFFSET(&rc, 100, 100, -100, -100);
  //draw_uipopup(hDC, s->pop, prc);
  draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));
  //DrawMenuImages(hDC, IdArowUpLarge, rc, ImageBlack);
  //draw_groupbox(hDC, rc, "adsfasdf");

  //draw_slider(hDC, s->si, s->rcsub + 1, s->ui->res);
  draw_slider_ctrl(s->si, e);
  draw_rbox_ctrl(s->rb, e);
  draw_menu_ctrl(s->menu1, e);
  return 0;
}

int test_menu_frame_proc(test_menu_t* s, const event* e)
{
  UI_engine* ui = e->ui;

  switch (e->msg) {
  case EVENT_LCLICK:
    if (e->sender == s->menu1) {
      printf("menu2 %d\n", e->wp);
    }

    if (e->sender == s->rb) {
      const draw_t dd[] = {dm_win2000, dm_agg};
      printf("rbox %d\n", e->wp);
      s->ui->res->dm_draw_f = dd[e->wp];
    }

    break;

  case EVENT_EXIT:
    if (e->sender == s) {
      PostQuitMessage(0);
    }

    break;

  case EVENT_INIT:
    if (e->sender == s) {
      Win_MoveCenter(ui->hwnd, 400, 300);
    }

    break;

  case WM_SIZE:
    if (1) {
      RECT rect = s->base.rc;
      RectCutTop(&rect, 20, 0, &s->menu1->base.rc);
      RectCutTop(&rect, -20, 0, &s->si->base.rc);
      RectCutLeft(&rect, -200, 0, &s->rb->base.rc);
    }

    break;

  case WM_RBUTTONUP:
    if (e->sender == s) {
      //uipopup_set(s->pop);
      force_redraw(e);
    }

    break;
  }

  slider_ctrl_event(s->si, e);
  rbox_ctrl_event(s->rb, e);
  menu_ctrl_event(s->menu1, e);
  //uipopup_event(s->pop, e, prc);
  return 0;
}
int test_menu()
{
  test_menu_t s[10] = {0};
  MENUITEM mi1[] = {
    0, -1, "item1",
    1, 1, "item11",
    1, 2, "item12",
    2, 3, "item121",
    2, -1, 0,
    2, 4, "item122",
    1, 5, "item13",
    2, 0, "item131",
    2, 0, "item132",
    2, 0, "item133",
    3, 0, "item1331",
    3, 0, "item1332",
#if 1
    0, -1, "item2",
    1, 0, "item21",
    2, 0, "item211",
    2, 0, "item212",
    1, 0, "item22",
    0, -1, "item3",
    1, 0, "item31",
    1, 0, "item32",
    1, 0, "item33",
    0, -1, "item4",
#endif
  };
  MENUITEM mi2[] = {
    0, 0, "item1",
    0, 0, "item2",
    0, 0, "item3",
    0, 0, "item4",
    0, 0, "item5",
    0, 0, "item6 adsfasdf",
  };
  char* sss[] = {
    "dm_win2000",
    "dm_agg",
  };
  DWORD crt = _GetSysColor(COLOR_BTNFACE);
  crt = 0xffc0c0c0;

  slider_ctrl_set(s->si, "aa %4.2f", 0.5, 0, 2, 10);
  rbox_ctrl_set(s->rb);
  GetImageList1("D:\\code\\c\\Docking\\raw toolbar\\res\\bitmap1.bmp", s->menu1->m_img, crt, 16, 0);
  menu_ctrl_set(s->menu1, mi1, countof(mi1));
  menu_ctrl_set(s->menu2, mi2, countof(mi2));
  s->menu1->ishor = 1;

  UISETCALL(s, test_menu_paint_proc, test_menu_frame_proc);
  UI_set(s->ui, "test_menu", &s->base, 0);

  waitkey(s->ui, -1);
  return 0;
}
