#include "sudokuctrl.inl"
typedef struct test_sudoku_t {
  ctrl_t c[1];
  sudokuctrl_t m_sudoku[1];
} test_sudoku_t;
static LRESULT test_sudoku_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_sudoku_t* s = (test_sudoku_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    char* strSudoku = ("708000300000201000500000000040000026300080000000100090090600004000070500000000000");
    sudokuctrl_set(s->m_sudoku, c, c, strSudoku);
    sudoku_init2(s->m_sudoku->m_nGivens, 5);
    c->sub = s->m_sudoku->c;
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
    mminfo->ptMinTrackSize.x += 0;
    mminfo->ptMinTrackSize.y += 0;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (1) {
      ctrl_setrect(c->sub, c->rc);
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
    }
    break;
  case WM_COMMAND:
    if (s) {
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
      //imdraw_fill(im, _GetSysColor(COLOR_BTNFACE));
      c->redraw = 0;
    }
    break;
  }
  return 0;
}

