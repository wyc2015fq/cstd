#include "grid_ctrl.inl"
typedef struct test_grid_t {
  uictrl base;
  UI_engine ui[1];
  grid_ctrl grid[1];
} test_grid_t;
int lena_test_arr[] = {
#include "..\faceDetect\lena.txt"
};
int test_grid_paint_proc(const test_grid_t* g, const event* e)
{
  UIHDC(e);
  drawaa_grid_ctrl(g->grid, e);
  return 0;
}
int test_grid_frame_proc(test_grid_t* g, const event* e)
{
  switch (e->msg) {
  case EVENT_EXIT:
    if (e->sender == g) {
      PostQuitMessage(0);
    }
    break;
  case EVENT_INIT:
    if (e->sender == g) {
      //Win_MoveCenter(e->ui->hwnd, 400, 300);
    }
    break;
  case EVENT_LCLICK:
    if (1) {
      printf("EVENT_LCLICK %d %d\n", 0, e->wp);
      return 1;
    }
    break;
  case EVENT_DATA:
    if (e->sender == g->grid) {
      char* buf = (char*)e->wp;
      if (1 == e->pt.x && 1 == e->pt.y) {
        //printf("%d\n", g_ttt);
        //g_ttt = 0;
      }
      _snprintf(buf, 256, "%5.1f", lena_test_arr[(e->pt.y - 1) * 128 + (e->pt.x - 1)] + .2);
      //++g_ttt;
      return 1;
    }
    break;
  case WM_SIZE:
    if (e->sender == g) {
      RECT rect;
      UIGetWorkArea(g->ui, &rect);
      RCOFFSET(&rect, 10, 10, -10, -10);
    }
    break;
  }
  grid_ctrl_event(g->grid, e);
  return 0;
}
int test_grid()
{
  test_grid_t g[1] = {0};
  grid_ctrl_set(g->grid, 120, 12);
  //scroll_set(g->sc, &g->grid->base);
  UISETCALL(g, test_grid_paint_proc, test_grid_frame_proc);
  UI_set(g->ui, "test_grid", &g->base, 0);
  //ShowWindow(g->ui->hwnd, SW_SHOW);
  waitkey(g->ui, -1);
  res_free();
  return 0;
}

