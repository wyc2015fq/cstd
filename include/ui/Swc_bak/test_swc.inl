
#include "ui/test_ui.inl"
#include "swc.inl"

typedef struct test_swc_t {
  ctrl_t c[1];
} test_swc_t;

static LRESULT test_swc_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_swc_t* s = (test_swc_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  //printf("sys_count %d\n", sys_count(c->sys));

  if (WM_CTRLINIT == uMsg) {
    sys_t* sys = c->sys;
    //c->sub = s->lay0->c;
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
    mminfo->ptMinTrackSize.x = 320;
    mminfo->ptMinTrackSize.y = 220;
  }
  break;

  case WM_SIZE:
    if (c->sub) {
      ctrl_setrect(c->sub, c->rc);
    }

    break;

  case WM_LBUTTONUP:
    if (1) {
    }

    break;

  case WM_COMMAND:
    if (1) {
      //printf("click %d\n", wParam);
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
      //printf("test_uidemo_proc WM_PAINT\n");
      //imdraw_rect(im, 0, rc, rgb(0,0,0), 0, 0, 0);
      c->redraw = 0;
    }

    break;
  }

  return 0;
}


int test_swc() {
  sys_t sys[1] = {0};
  test_swc_t test_swc[1] = {0};
  return test_ui();
  win_set(sys, test_swc_proc, test_swc, test_swc->c, 0, "test_swc", WT_WINDOWS, 0, 0, 0, 0, 0);
  for (;;) {
    int ch = waitkey(sys, -1);
    
    if (-2 == ch) {
      break;
    }
    
    //printf("--%08x\n", &ch);
  }
  
  sys_exit(sys);
  return 0;
}
