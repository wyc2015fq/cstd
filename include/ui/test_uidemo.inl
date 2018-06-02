#define TEST_UIDEMO_MAXWIN 4
typedef struct test_uidemo_t {
  ctrl_t c[1];
  button_t btn0[1];
  button_t btn1[1];
  slider_t sli0[1];
  groupbox_t grp0[1];
  layout_t lay0[1];
  scale_t sca0[1];
  imageview_t img0[1];
  gridctrl_t grd0[1];
  dockctrl_t doc0[1];
  editctrl_t edt0[1];
  menuctrl_t menu0[TEST_UIDEMO_MAXWIN];
  combobox_t comb0[TEST_UIDEMO_MAXWIN];
  framectrl_t frame0[TEST_UIDEMO_MAXWIN];
  ctrl_t rib0[1];
  char buf[256];
} test_uidemo_t;
#include "draw/fractal1_4.inl"
static LRESULT test_uidemo_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_uidemo_t* s = (test_uidemo_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  //printf("sys_count %d\n", sys_count(c->sys));
  if (WM_CTRLINIT == uMsg) {
    sys_t* sys = c->sys;
    button_set(s->btn0, c, "按钮0");
    button_set(s->btn1, c, "按钮1");
    groupbox_set(s->grp0, c, "组合框0", 0);
    slider_set(s->sli0, c, "slider %6.3f", 0, 100, 10);
    if (0) {
      scale_set(s->sca0, c, c);
      gridctrl_set(s->grd0, c, 10, 10);
      //ctrl_set(dockctrl_proc, s->doc0, s->doc0->c, c);
    }
    if (0) {
      TViewRect rect;
      rect.x0 = -0.5;
      rect.y0 = 0;
      rect.r = 2;
      imageview_set(s->img0, c, c);
      imsetsize(s->img0->im, 600, 800, 3, 1);
      //imdraw_mandelbrot1(s->img0->im, rect, 10);
      imdraw_mandelbrot1(s->img0->im, rect, 10);
      //scrollview_set(s->scv0, c, c);
      //s->scv0->c->style = WS_HSCROLL|WS_VSCROLL;
    }
    if (0) {
      str_t s1[1] = {0};
      str_setsize(s1, 1 * _1K);
      memset(s1->s, 'y', s1->l);
      randstr(s1->s, s1->l, "\n123456 abcdefghijklmkopqrstuvwzyz ABCDEFGHIJKLMKOPQRSTUVWZYZ ~!@#$%^&*()_+=-{}[]|<>,.?`");
      str_save("D:\\big.txt", s1);
      editctrl_set_buf(s->edt0, c, 0, s->buf, 256, 0);
      str_free(s1);
    }
    {
      static const char* combo_items[] = {
        "item 0",
        "item 1",
        "item 2",
        "item 3",
        "item 4",
        "item 5",
        "item 6",
        "item 7",
        "item 8",
        "item 9",
        "item 10",
      };
      combobox_set(s->comb0, c, countof(combo_items), combo_items);
    }
    {
      static menuitem_t menuitems[] = {
        0, -1, -1, "item 1",
        1, -1, -1, "item 11",
        2, -1, -1, "item 111",
        2, -1, -1, "item 112",
        2, -1, -1, "item 113",
        2, -1, -1, "item 114",
        2, -1, -1, "item 115",
        1, -1, -1, "item 12",
        1, -1, -1, "item 13",
        1, -1, -1, "item 14",
        1, -1, -1, "item 15",
        0, -1, -1, "item 2",
        1, -1, -1, "item 21",
        1, -1, -1, "item 22",
        1, -1, -1, "item 23",
        1, -1, -1, "item 24",
        1, -1, -1, "item 25",
        0, -1, -1, "item 3",
        1, -1, -1, "item 31",
        1, -1, -1, "item 32",
        1, -1, -1, "item 33",
        1, -1, -1, "item 34",
        1, -1, -1, "item 35",
        0, -1, -1, "item 4",
        1, -1, -1, "item 41",
        1, -1, -1, "item 42",
        1, -1, -1, "item 43",
        1, -1, -1, "item 44",
        1, -1, -1, "item 45",
        0, -1, -1, "item 5",
        1, -1, -1, "item 51",
        1, -1, -1, "item 52",
        1, -1, -1, "item 53",
        1, -1, -1, "item 54",
        1, -1, -1, "item 55",
      };
      menuctrl_set(s->menu0, c, countof(menuitems), menuitems, 0);
    }
    if (1) {
      const char* lay =
          " xx;"
          "x ;"
          "a %;"
          "a %;"
          "a %;"
          ;
      const char* lay1 =
          " xx;"
          "a%-;"
          "a% ;"
          "a% ;"
          "x ;"
          ;
      layout_set(s->lay0, c, 4, 0, lay1, s->menu0->c, s->comb0->c, s->btn0->c);
      //ctrl_addsub_head(c, s->lay0->c);
      //c->sub = s->lay0->c;
    }
    if (1) {
      int i, x = 0, y = 0;
      static char frame_text[countof(s->frame0)][100];
      ctrl_addsub_head(s->frame0[0].c, s->lay0->c);
      for (i = 0; i < countof(s->frame0); ++i) {
        _snprintf(frame_text[i], 100, "frame%d", i);
        rc = iRECT2(x, y, 200, 200);
        framectrl_set(s->frame0 + i, c, frame_text[i], rc);
        ctrl_addsub_tail(c, s->frame0[i].c);
        x += 20;
        y += 20;
      }
      //ctrl_addsub_head(s->frame0[0].c, s->btn0->c);
    }
#if 0
    //ctrl_set(s->rib0, ribbon_proc, "CaptionText:s", "Ribbon Demo");
    c->sub = s->doc0->c;
    c->sub = s->edt0->c;
    c->sub = s->grd0->c;
    c->sub = s->img0->c;
    c->sub = s->frame0->c;
#endif
    //c->sub = s->scv0->c;
    //ctrl_addsub_head(c, s->lay0->c);
    return 0;
  }
  //sub = s->rib0;
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    //c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
    mminfo->ptMinTrackSize.x = 820;
    mminfo->ptMinTrackSize.y = 620;
  }
  break;
  case WM_SIZE:
    if (1) {
      //ctrl_setrect(c->sub, c->rc);
    }
    break;
  case WM_LBUTTONUP:
    if (1) {
    }
    break;
  case WM_COMMAND:
    if (sender == s->menu0->c) {
      printf("menu click %d\n", wParam);
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      imdraw_fill(im, _rgb(0, 0, 0));
      imdraw_LayoutPane(im, 0, rc, MarkersLayoutA, 1, 0, c->sys);
      //printf("test_uidemo_proc WM_PAINT\n");
      //imdraw_rect(im, 0, rc, _rgb(0,0,0), 0, 0, 0);
      c->redraw = 0;
    }
    break;
  }
  return 0;
}

