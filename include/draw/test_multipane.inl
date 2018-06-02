

#include "DockingMarkers.inl"
#include "multipane_ctrl.inl"
typedef struct test_multipane_t {
  uictrl base;
  UI_engine ui[1];
  multipane_ctrl_t mp[1];
} test_multipane_t;
int test_multipane_event_proc(test_multipane_t* g, event* e)
{
  switch (e->msg) {
  case EVENT_EXIT:
    if (e->sender == g) {
      PostQuitMessage(0);
      return 1;
    }

    break;

  case EVENT_INIT:
    if (e->sender == g) {
      Win_MoveCenter(e->ui->hwnd, 400, 300);
      return 1;
    }

    break;

  case EVENT_LCLICK:
    if (1) {
      printf("EVENT_LCLICK %d %d\n", 0, e->wp);
      return 1;
    }

    break;
  }

  return 0;
}

int test_multipane_paint_proc(test_multipane_t* g, HDDC hDC)
{
  RECT rcs[256] = {0};
  RECT rect;
  UIGetWorkArea(g->ui, &rect);
  draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));
  RCOFFSET(&rect, 10, 10, -10, -10);

  g->mp->base.rect = rect;
  drawaa_multipane_ctrl(hDC, g->mp);

  return 0;
}
int test_multipane_frame_proc(test_multipane_t* g, event* e)
{
  //editex_ctrl_event(g->m_edit, e);
  multipane_ctrl_event(g->mp, e);
  return 0;
}

int test_multipane()
{
  test_multipane_t g[1] = {0};
  DWORD crt = _GetSysColor(COLOR_BTNFACE);
  tab_item li[] = {
    -1, "ttt1", 0,
    -1, "ttt2", 0,
    -1, "ttt3", 0,
    -1, "ttt4", 0,
    -1, "ttt5", 0,
    -1, "ttt6", 0,
  };
  layitem_t la[10] = {
    0, 0, 0, 0,
    'r', 300, 0, 0,
    'l', 300, 0, 0,
  };
  int pn[] = {2, 2, 2};
  multipane_ctrl_set(g->mp, li, pn, 3, la);

  UISETCALL(g, test_toolbar_paint_proc, test_toolbar_frame_proc);
  UI_set(g->ui, "test_multipane", &g->base, 0);
  //ShowWindow(g->ui->hwnd, SW_SHOW);

  waitkey(g->ui, -1);
  FreeMemDC();
  return 0;
}

