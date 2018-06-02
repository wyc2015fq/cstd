
typedef struct test_RubixCubeui_t {
  ctrl_t c[1];
  RubixCube dv[1];
} test_RubixCubeui_t;

static LRESULT test_RubixCubeui_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_RubixCubeui_t* s = (test_RubixCubeui_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  IPOINT pt = iPOINT(x, y);

  switch (uMsg) {
  case WM_CTRLINIT:
    if (s) {
      RubixCube_Init(s->dv);
      return 0;
    }

    break;

  case WM_COMMAND:
    break;

  case WM_LBUTTONUP:
    RubixCube_OnLButtonUp(s->dv, 0, pt);
    force_redraw(c);
    break;

  case WM_MOUSEMOVE:
    RubixCube_OnMouseMove(s->dv, 0, pt);
    force_redraw(c);
    break;

  case WM_LBUTTONDOWN:
    RubixCube_OnLButtonDown(s->dv, 0, pt);
    force_redraw(c);
    break;

  case WM_GETMINMAXINFO:
    if (1) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      //c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
      mminfo->ptMinTrackSize.x = 400;
      mminfo->ptMinTrackSize.y = 300;
      //printf("WM_MOUSEMOVE\n");
    }

    break;

  case WM_TIMER:
    if (s) {
      //force_redraw(c);
    }

    break;

  case WM_PAINT:
    if (c->recalcsize) {
      //ctrl_setrect(c->sub, c->rc);
    }

    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      RubixCube_OnPaint(s->dv, im, 0, rc, c->fo, c->recalcsize);
      c->redraw = 0;
    }

    break;
  }

  return 0;
}
