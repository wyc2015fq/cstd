//#define EDIT_MARGIN 1
IRECT editctrl_get_cursor_rect(editctrl_t* s)
{
  IRECT rc2, rc;
  int x, y, b, line_h, char_w;
  line_h = editctrl_get_line_h(s);
  char_w = editctrl_get_char_w(s);
  ctrl_get_rect(s->c, s->c->rc, 0, 0, &rc);
  x = rc.l + s->org.x + char_w * s->sd->pos.x;
  y = rc.t + s->org.y + line_h * s->sd->pos.y;
  b = MAX(1, line_h / 20);
  rc2 = iRECT(x, y, x + b, y + line_h);
  return rc2;
}
int editctrl_draw(editctrl_t* s, IRECT rc, img_t* im)
{
  int i, len, y = s->org.y;
  strdata_t* sd = s->sd;
  font_t* fo = s->c->fo;
  char_color_t str[256];
  int line_h = editctrl_get_line_h(s);
  int char_w = editctrl_get_char_w(s);
  COLOR crBack = _rgb(1, 1, 1);
  COLOR crText = _rgb(0, 0, 0);
  i = (-y) / line_h;
  y += i * line_h;
  imdraw_rect(im, &rc, rc, crBack, crText, 1, BDM_ALL);
  ctrl_get_rect(s->c, rc, 0, 0, &rc);
  //RCOFFSET(&rc, mg, mg, -mg, -mg);
  for (; (len = sd->get_line(sd, i, 0, str, 256)) >= 0 && (rc.t + y) < rc.b; ++i) {
    imdraw_text_line(im, &rc, s->org.x, rc.t + y, str, len, line_h, char_w, 0, s->c->fo);
    y += line_h;
  }
  if (ctrl_isfocus(s->c)) {
    s->c->sys->rcCursor = editctrl_get_cursor_rect(s);
  }
  return 0;
}
int sys_clipboard_copy_string(sys_t* sys, const char* s, int l)
{
  return CopyString(sys->hwnd, s, l);
}
char* sys_clipboard_paste_string(sys_t* sys)
{
  return PasteString(sys->hwnd);
}
static LRESULT editctrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  editctrl_t* s = (editctrl_t*)c->x;
  IRECT rcc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int ret = 0;
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    c->border = iRECT(1, 1, 1, 1);
    //c->style |= WS_VSCROLL;
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    //mminfo->ptMinTrackSize.x = 120;
    //mminfo->ptMinTrackSize.y = editctrl_get_line_h(s);
    mminfo->ptMinTrackSize.x = 20;
    mminfo->ptMinTrackSize.y = 4 + font_text_size(c->fo, "H", 1, 0).h;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
      //printf("%d\n", s->str->l);
      //printf("%d %d %d %d\n", s->sd->pos.x, s->sd->pos.y, s->sd->beg.x, s->sd->beg.y);
      //force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (c->sys->ldown && iPtInRect(&rcc, x, y)) {
      if (ctrl_isfocus(c)) {
        editctrl_set_pos(s, x, y);
        //editctrl_update_org(s);
        force_redraw(c);
      }
    }
    break;
  case WM_MOUSEWHEEL:
    if (iPtInRect(&rcc, x, y)) {
      //s->org.y += GET_WHEEL_DELTA_WPARAM(wParam) / 3;
    }
    break;
  case WM_LBUTTONDBLCLK:
    if (iPtInRect(&rcc, x, y)) {
      s->c->sys->cursor_show = 1;
      editctrl_set_pos(s, x, y);
      s->sd->set_pos(s->sd, 0, s->sd->pos.y);
      s->sd->beg = s->sd->pos;
      s->sd->set_pos(s->sd, 9999999, s->sd->pos.y);
      force_redraw(c);
    }
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rcc, x, y)) {
      s->c->sys->cursor_show = 1;
      //hit = editctrl_HitTest(s, x, y);
      //s->rdown = 0;
      // 窗体获得焦点以后设置光标的位置
      // 从当前鼠标的位置获得对应字符的序号
      //editctrl_charfrompos(s, rcc, x, y);
      if (GetKeyState(VK_SHIFT) >= 0) {
        s->sd->beg = s->sd->pos;
      }
      editctrl_set_pos(s, x, y);
      s->sd->beg = s->sd->pos;
      force_redraw(c);
      return TRUE;
    }
    break;
  case WM_LBUTTONUP:
    if (c->sys->ldown) {
      force_redraw(c);
    }
    break;
  case WM_CHAR:
    if (ctrl_isfocus(c)) {
      UINT nChar = wParam;
      if (nChar >= 25 || '\n' == nChar || '\r' == nChar || '\t' == nChar) {
        char str[8] = {nChar, 0};
        //printf("key %d\n", nChar);
        // 此处进行一般输入字符的处理，如果当前有被选中的字符串，先进行删除操作
        editctrl_ins_str(s, str, 1, 0);
        //editctrl_calc_rect(s, c->rc);
        force_redraw(c);
      }
      //printf("%d\n", nChar);
    }
    break;
  case WM_KEYDOWN:
    if (ctrl_isfocus(c)) {
      UINT nChar = wParam;
      //const char* cText = s->str->s + s->sd->pos;
      //printf("%d %d\n", nChar, GetKeyState(VK_SHIFT));
      // 此处处理Ctrl+C、Ctrl+V和Ctrl+X操作
      switch (nChar) {
      case 13:
        if (!(s->fmt & DT_SINGLELINE)) {
          char str[8] = {'\n', 0};
          editctrl_ins_str(s, str, 1, 0);
        }
        break;
      case 'C':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          str_t str[1] = {0};
          s->sd->get_sel(s->sd, str);
          sys_clipboard_copy_string(c->sys, str->s, str->l);
          str_free(str);
        }
        break;
      case 'V':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          char* str = sys_clipboard_paste_string(c->sys);
          int slen = strlen(str);
          editctrl_ins_str(s, str, slen, 0);
          //editctrl_calc_rect(s, c->rc);
          free(str);
        }
        break;
      case 'X':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          if (1) {//Ctrl+X
            str_t str[1] = {0};
            s->sd->get_sel(s->sd, str);
            sys_clipboard_copy_string(c->sys, str->s, str->l);
            str_free(str);
            editctrl_ins_str(s, 0, 0, 0);
            //editctrl_del_select(s);
            //editctrl_calc_rect(s, c->rc);
          }
        }
        break;
      case VK_UP:
      case VK_DOWN:
      case VK_RIGHT:
      case VK_LEFT:
        if (s) {
          IPOINT pos = {0};
          if (VK_LEFT == wParam || VK_RIGHT == wParam) {
            pos.x = (VK_LEFT == wParam) ? -1 : 1;
          }
          else {
            pos.y = (VK_UP == wParam) ? -1 : 1;
          }
          s->sd->move_pos(s->sd, pos.x, pos.y);
          if (GetKeyState(VK_SHIFT) >= 0) {
            s->sd->beg = s->sd->pos;
          }
          force_redraw(c);
        }
        break;
      case VK_END:
      case VK_HOME:
        if (s) {
          IPOINT pos = s->sd->pos;
          pos.x = (VK_HOME == wParam) ? 0 : 9999999;
          editctrl_set_pos(s, pos.x, pos.y);
          if (GetKeyState(VK_SHIFT) > 0) {
            s->sd->beg = s->sd->pos;
          }
          force_redraw(c);
        }
        break;
      case VK_BACK:
        if (s) {
          // 响应用户删除操作 backspace
          editctrl_ins_str(s, 0, 0, -1);
          force_redraw(c);
        }
        break;
      case VK_DELETE:
        if (s) {
          editctrl_ins_str(s, 0, 0, 1);
          //editctrl_calc_rect(s, c->rc);
          force_redraw(c);
        }
        break;
      default:
        if (s) {
          char str[4] = {nChar, 0};
          // 此处进行一般输入字符的处理，如果当前有被选中的字符串，先进行删除操作
          //editctrl_insertstr(s, str, 1);
          //printf("%d\n", nChar);
          //s->sd->ins_str(s->sd, str, 1);
        }
        break;
      }
    }
    //force_redraw(c);
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rcc;
      ctrl_get_rect(c, c->rc, 0, &rcc, 0);
      //scrollview_draw(s->sc, c, im, prc);
      iRectOffset2(&rcc, -rcc.l, -rcc.t);
      editctrl_draw(s, rcc, im);
    }
    c->redraw = 0;
    break;
  }
  return 0;
}
CC_INLINE int editctrl_set_buf(editctrl_t* s, ctrl_t* li, const char* text, char* buf, int len, int fmt)
{
  ctrl_set(editctrl_proc, s, s->c, li, WT_CHILD, text);
  STRDATA_INITFONC(s->sd, strdata_buf);
  s->sd->bf->data = (uchar*)buf;
  s->sd->bf->len = len;
  s->fmt = fmt;
  return 0;
}

