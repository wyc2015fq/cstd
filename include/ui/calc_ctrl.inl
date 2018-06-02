enum CalculatorCommands {
  idCommandNull,
  idCommandNum,
  idCommandClear,
  idCommandReset,
  idCommandMemClear,
  idCommandMemRead,
  idCommandMemAdd,
  idCommandAdd,
  idCommandSub,
  idCommandMult,
  idCommandDiv,
  idCommandSign,
  idCommandDot,
  idCommandSqrt,
  idCommandPercent,
  idCommandResult,
  idCommandRev,
  idCommandBackspace,
  idCommandUser = 100
};
#define CALC_CTRL_DEF_DEF(CALC_CTRL_DEF) \
  CALC_CTRL_DEF("<-", idCommandBackspace, VK_BACK) \
  CALC_CTRL_DEF("MC", idCommandMemClear, 'L') \
  CALC_CTRL_DEF("MR", idCommandMemRead, 'R') \
  CALC_CTRL_DEF("M+", idCommandMemAdd, 'P') \
  CALC_CTRL_DEF("C", idCommandReset, VK_ESCAPE) \
  CALC_CTRL_DEF("CE", idCommandClear, VK_DELETE) \
  CALC_CTRL_DEF("7", idCommandNum, '7') \
  CALC_CTRL_DEF("8", idCommandNum, '8') \
  CALC_CTRL_DEF("9", idCommandNum, '9') \
  CALC_CTRL_DEF("/", idCommandDiv, '/') \
  CALC_CTRL_DEF("*", idCommandMult, '*') \
  CALC_CTRL_DEF("4", idCommandNum, '4') \
  CALC_CTRL_DEF("5", idCommandNum, '5') \
  CALC_CTRL_DEF("6", idCommandNum, '6') \
  CALC_CTRL_DEF("+/-", idCommandSign, VK_F9) \
  CALC_CTRL_DEF("-", idCommandSub, '-') \
  CALC_CTRL_DEF("1", idCommandNum, '1') \
  CALC_CTRL_DEF("2", idCommandNum, '2') \
  CALC_CTRL_DEF("3", idCommandNum, '3') \
  CALC_CTRL_DEF("Sqrt",idCommandSqrt, '@') \
  CALC_CTRL_DEF("+", idCommandAdd, '+') \
  CALC_CTRL_DEF("0", idCommandNum, '3') \
  CALC_CTRL_DEF(".", idCommandDot, '.') \
  CALC_CTRL_DEF("=", idCommandResult, VK_RETURN) \
  CALC_CTRL_DEF("%", idCommandPercent, '%') \
  CALC_CTRL_DEF("1/x", idCommandRev, 'R')
typedef struct calc_btn_info_t {
  const char* str;
  int type;
  int key;
} calc_btn_info_t;
static const calc_btn_info_t g_calc_btn_info[] = {
#define CALC_CTRL_DEF(str, id, ac) {str, id, ac},
  CALC_CTRL_DEF_DEF(CALC_CTRL_DEF)
#undef CALC_CTRL_DEF
};
typedef struct calc_t {
  ctrl_t c[1];
  double value, memvalue;
  int hit, hot;
  int ldown;
  int border;
  int gap;
} calc_t;
int calc_hit(calc_t* s, IRECT rc, int x, int y)
{
  int i = 3, row, col, w, h, hit = 0;
  IRECT rc1;
  i = 3;
  w = (RCW(&rc) + s->gap) / 5;
  h = (RCH(&rc) + s->gap) / 6;
  for (i = 4; i - 4 < countof(g_calc_btn_info); ++i) {
    row = i / 5;
    col = i % 5;
    rc1 = iRECT2(rc.l + col * w, rc.t + row * h, w - s->gap, h - s->gap);
    if (iPtInRect(&rc1, x, y)) {
      hit = i;
      return hit;
    }
  }
  return hit;
}
CC_INLINE LRESULT calc_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  calc_t* s = (calc_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int i = 3, row, col, w, h, hit = 0;
  IRECT rc1;
  //IPOINT pt = iPOINT(x, y);
  switch (uMsg) {
  case WM_CTRLINIT:
    s->value = 0;
    s->hit = 0;
    s->border = 2;
    s->gap = 2;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO:
    if (s) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      mminfo->ptMinTrackSize.x = 160;
      mminfo->ptMinTrackSize.y = 200;
    }
    break;
  case WM_SIZE:
    break;
  case WM_LBUTTONUP:
    if (s->hit) {
      if (iPtInRect(&rc, x, y)) {
        s->hit = 0;
        force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    hit = calc_hit(s, rc, x, y);
    s->hot = s->hit = hit;
    force_redraw(c);
    break;
  case WM_MOUSEMOVE:
    if (1) {
      hit = calc_hit(s, rc, x, y);
      if (hit != s->hot) {
        s->hot = hit;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (wParam) {
      img_t* im = (img_t*)wParam;
      uidraw_t* drawer = c->sys->drawer;
      imdraw_rect(im, &rc, rc, _rgb(0.4, 0.4, 0.4), 0, 0, 0);
      iRectInflate2(&rc, -s->border, -s->border);
      i = 3;
      w = (RCW(&rc) + s->gap) / 5;
      h = (RCH(&rc) + s->gap) / 6;
      rc1 = iRECT2(rc.l, rc.t, 4 * w - s->gap, h - s->gap);
      drawer->OnDrawCalculatorDisplay(drawer, im, rc1, "123123.12312");
      for (i = 4; i - 4 < countof(g_calc_btn_info); ++i) {
        row = i / 5;
        col = i % 5;
        rc1 = iRECT2(rc.l + col * w, rc.t + row * h, w - s->gap, h - s->gap);
        drawer->OnDrawCalculatorButton(drawer, im, rc1, i == s->hot ? (i == s->hit ? ButtonsIsPressed : ButtonsIsHighlighted) : 0, g_calc_btn_info[i - 4].str);
      }
      c->redraw = 0;
    }
    break;
  }
  return FALSE;
}

