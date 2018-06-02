typedef struct editctrl_t {
  ctrl_t c[1];
  int pos; // 当前光标处于第几个字符的后面，从0开始计数
  int rpos;
  int lpos;
  int rows; // 折行数
  int line; // 总行数
  int hit;
  int beg; // 选择的字符的起始位置
  int hot;
  int rdown;
  int fmt;
  IPOINT org; // 字符块原点
  str_t str[1]; // 当前包含的文本
  scrollbar_t sc[1]; // 滚动条
} editctrl_t;
#define SELECTLEN abs(s->pos-s->beg)
#define EDIT_MARGIN 2
static int chr_next(const str_t* s, int i)
{
#if 0
  if (i < 0) {
    return 0;
  }
  if (i >= s->l) {
    return s->l;
  }
  return (s->s[i] & 0x80) ? i + 2 : i + 1;
#else
  char* p = s->s + i;
  return CharNext(p) - s->s;
#endif
}
static int chr_prev(const str_t* s, int i)
{
#if 0
  if (1) {
    if (i <= 1) {
      return 0;
    }
    if (i > s->l) {
      return s->l;
    }
    return (s->s[i - 1] & 0x80) ? i - 2 : i - 1;
  }
  else {
    int j = 0, j1;
    for (; (j1 = chr_next(s, j)) < i; j = j1) {
    }
    return j;
  }
#else
  char* p = s->s + i;
  return CharPrev(s->s, p) - s->s;
#endif
}
static int editctrl_clear(editctrl_t* s)
{
  str_free(s->str);
  s->beg = s->pos = 0;
  return 0;
}
static int editctrl_del_select(editctrl_t* s)
{
  if (SELECTLEN > 0) {
    int nBeg = MIN(s->beg, s->pos);
    int nEnd = MAX(s->beg, s->pos);
    str_del(s->str, nBeg, nEnd - nBeg);
    s->beg = s->pos = nBeg;
  }
  return 0;
}
static int editctrl_insertstr(editctrl_t* s, const char* str, int slen)
{
  editctrl_del_select(s);
  str_ins(s->str, s->pos, str, slen);
  s->pos += slen;
  s->beg = s->pos;
  return 0;
}
static int editctrl_char_info(editctrl_t* s, IRECT rc, int x, int y, char_hittest_t* ch)
{
  font_t* fo = s->c->fo;
  char_font_map_t fm[1] = {0};
  int mg = EDIT_MARGIN;
  IPOINT org = iPOINT(-s->sc->si[0].nPos * EDIT_SB_CHARW, -s->sc->si[1].nPos * fo->h);
  char_font_map_set(fm, 0, s->str->l, fo, 0, 0);
  ch->x = x, ch->y = y;
  RCOFFSET(&rc, mg, mg, -mg, -mg);
  //s->pos = font_hittest(fo, s->str->s, s->str->l, s->fmt, x - (s->org.x + EDIT_MARGIN), y - (s->org.y), 0);
  imdraw_textex(0, &rc, rc, org, s->str->s, s->str->l, fo->h, fo->bl, s->fmt, 1, fm, ch);
  return 0;
}
static int editctrl_calc_rect(editctrl_t* s, IRECT rc)
{
  font_t* fo = s->c->fo;
  char_hittest_t ch[1] = {0};
  if (s->fmt & DT_SINGLELINE) {
    editctrl_char_info(s, rc, 0, 0, ch);
    s->line = ch->line;
    s->rows = ch->rows;
  }
  else {
    IRECT rcc = rc;
    int page = 0;
    iRectOffset(&rcc, 0, 0, -SBW1, -SBW1);
    s->sc->c->rc = rc;
    editctrl_char_info(s, rcc, 0, 0, ch);
    s->line = ch->line;
    s->rows = ch->rows;
    //scrollview_set_info(s->sc->si + 0, 0, 0, 0);
    page = MAX((RCH(&rcc)) / fo->h, 1);
    scrollview_set_info(s->sc->si + 1, 0, s->rows, page);
  }
  return 0;
}
static int editctrl_charfrompos(editctrl_t* s, IRECT rc, int x, int y)
{
  char_hittest_t ch[1] = {0};
  editctrl_char_info(s, rc, x, y, ch);
  s->pos = ch->pos;
  s->rpos = ch->rpos;
  s->line = ch->line;
  s->rows = ch->rows;
  return s->pos;
}
static int editctrl_update_org1(editctrl_t* s, IRECT rc, const font_t* fo)
{
  int tmHeight = fo->h; // 文本字体的信息
  ISIZE szstr = font_text_size(fo, s->str->s, s->pos, s->fmt);
  int mg = EDIT_MARGIN;
  RCOFFSET(&rc, mg, mg, -mg, -mg);
  if (s->fmt & DT_SINGLELINE) {
    s->org.y = (RCH(&rc) - tmHeight) / 2;
  }
  s->org.x = BOUND(s->org.x, -szstr.w, RCW(&rc) - szstr.w);
  s->org.y = BOUND(s->org.y, tmHeight - szstr.h, RCH(&rc) - szstr.h);
  return 0;
}
static LRESULT editctrl_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  editctrl_t* s = (editctrl_t*)c->x;
  IRECT rcc;//, rc1 = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int ret = 0;
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    str_free(s->str);
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    scrollbar_set(s->sc, c, c->sys);
    //s->fmt = DT_SINGLELINE;
    s->hot = 0;
    return 0;
  }
  rcc = s->fmt & DT_SINGLELINE ? c->rc : s->sc->rcc;
  if (WM_PAINT != uMsg && WM_SIZE != uMsg) {
    if (ret = s->sc->c->ctrlproc(c, s->sc->c, hwnd, uMsg, wParam, lParam)) {
      return ret;
    }
  }
  switch (uMsg) {
  case WM_SETFOCUS:
    if (!s->hot) {
      s->hot = 1;
      force_redraw(c);
    }
    break;
  case WM_KILLFOCUS:
    if (s->hot) {
      s->hot = 0;
      force_redraw(c);
    }
    break;
  case WM_HSCROLL:
  case WM_VSCROLL:
    if (sender == s->sc->c) {
      SCROLLINFO* p = &s->sc->si[uMsg == WM_VSCROLL];
      p->nPos = p->nTrackPos;
      force_redraw(c);
    }
    break;
  case WM_SIZE:
    editctrl_calc_rect(s, c->rc);
    s->sc->c->ctrlproc(c, s->sc->c, hwnd, uMsg, wParam, lParam);
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    if (s->fmt & DT_SINGLELINE) {
      mminfo->ptMinTrackSize.x = 120;
      mminfo->ptMinTrackSize.y = 20;
    }
    else {
      mminfo->ptMinTrackSize.x = 320;
      mminfo->ptMinTrackSize.y = 320;
    }
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (s->hot) {
      //printf("%d\n", s->str->l);
      force_redraw(c);
    }
    break;
  case WM_LBUTTONDBLCLK:
    if (iPtInRect(&rcc, x, y)) {
      IPOINT pt;
      editctrl_charfrompos(s, rcc, x, y);
      pt = str_get_word(s->str, s->pos);
      s->beg = pt.x;
      s->pos = pt.y;
      force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (c->sys->ldown && iPtInRect(&rcc, x, y)) {
      if (ctrl_isfocus(c)) {
        editctrl_charfrompos(s, rcc, x, y);
        //editctrl_update_org(s);
        force_redraw(c);
      }
    }
    break;
  case WM_MOUSEWHEEL:
    if (!(s->fmt & DT_SINGLELINE) && iPtInRect(&rcc, x, y)) {
      //s->org.y += GET_WHEEL_DELTA_WPARAM(wParam) / 3;
    }
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rcc, x, y)) {
      s->hot = TRUE;
      s->rdown = 0;
      // 窗体获得焦点以后设置光标的位置
      // 从当前鼠标的位置获得对应字符的序号
      editctrl_charfrompos(s, rcc, x, y);
      if (GetKeyState(VK_SHIFT) >= 0) {
        s->beg = s->pos;
      }
      //editctrl_update_org(s);
      force_redraw(c);
    }
    else if (s->hot) {
      s->hot = FALSE;
      force_redraw(c);
    }
    return TRUE;
    break;
  case WM_LBUTTONUP:
    if (c->sys->ldown) {
      force_redraw(c);
    }
    break;
  case WM_CHAR:
    if (s->hot) {
      UINT nChar = wParam;
      if (nChar >= 20 || '\n' == nChar || '\r' == nChar || '\t' == nChar) {
        char str[8] = {nChar, 0};
        //printf("key %d\n", nChar);
        // 此处进行一般输入字符的处理，如果当前有被选中的字符串，先进行删除操作
        editctrl_insertstr(s, str, 1);
        editctrl_calc_rect(s, c->rc);
      }
      //printf("%d\n", nChar);
    }
    break;
  case WM_KEYDOWN:
    if (s->hot) {
      UINT nChar = wParam;
      const char* cText = s->str->s + s->pos;
      //printf("%d %d\n", nChar, GetKeyState(VK_SHIFT));
      // 此处处理Ctrl+C、Ctrl+V和Ctrl+X操作
      switch (nChar) {
      case 13:
        if (!(s->fmt & DT_SINGLELINE)) {
          char str[8] = {'\n', 0};
          editctrl_insertstr(s, str, 1);
        }
        break;
      case 'C':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          CopyString(hwnd, s->str->s + s->beg, SELECTLEN);
        }
        break;
      case 'V':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          char* str = PasteString(hwnd);
          int slen = _tcslen(str);
          editctrl_insertstr(s, str, slen);
          editctrl_calc_rect(s, c->rc);
          free(str);
        }
        break;
      case 'X':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          if (SELECTLEN > 0) {//Ctrl+X
            CopyString(hwnd, s->str->s + s->beg, SELECTLEN);
            editctrl_del_select(s);
            editctrl_calc_rect(s, c->rc);
          }
        }
        break;
      case VK_UP:
      case VK_DOWN:
      case VK_RIGHT:
      case VK_LEFT:
        if (s) {
          IPOINT pt;
          if (VK_LEFT == wParam || VK_RIGHT == wParam) {
            if (VK_LEFT == wParam) {
              s->pos = chr_prev(s->str, s->pos);
            }
            else if (VK_RIGHT == wParam) {
              int k = CharNext(cText) - cText;
              s->pos += k;
            }
            pt = str_pos2pt(s->str, s->pos);
          }
          else {
            IPOINT ptmax = str_pos2pt(s->str, s->str->l);
            pt = str_pos2pt(s->str, s->pos);
            if (VK_UP == wParam) {
              pt.y--;
            }
            else if (VK_DOWN == wParam) {
              pt.y++;
            }
            pt.y = BOUND(pt.y, 0, ptmax.y);
            s->pos = str_pt2pos(s->str, pt);
          }
          s->rpos = pt.y;
          printf("pos %d pt %d %d\n", s->pos, pt.x, pt.y);
          s->pos = BOUND(s->pos, 0, s->str->l);
          if (GetKeyState(VK_SHIFT) >= 0) {
            s->beg = s->pos;
          }
          force_redraw(c);
        }
        break;
      case VK_END:
      case VK_HOME:
        if (s) {
          IPOINT pt = str_get_line(s->str, s->pos);
          s->pos = (VK_HOME == wParam) ? pt.x : pt.y;
          if (GetKeyState(VK_SHIFT) > 0) {
            s->beg = s->pos;
          }
          force_redraw(c);
        }
        break;
      case VK_BACK:
        if (s) {
          // 响应用户删除操作 backspace
          if (0 >= SELECTLEN && s->pos > 0) {
            s->beg = chr_prev(s->str, s->pos);
          }
          editctrl_del_select(s);
          editctrl_calc_rect(s, c->rc);
          force_redraw(c);
        }
        break;
      case VK_DELETE:
        if (s) {
          if (0 >= SELECTLEN && s->pos < s->str->l) {
            s->beg = chr_next(s->str, s->pos);
          }
          editctrl_del_select(s);
          editctrl_calc_rect(s, c->rc);
          force_redraw(c);
        }
        break;
      default:
        if (s) {
          char str[8] = {nChar, 0};
          // 此处进行一般输入字符的处理，如果当前有被选中的字符串，先进行删除操作
          //editctrl_insertstr(s, str, 1);
          //printf("%d\n", nChar);
        }
        break;
      }
    }
    //force_redraw(c);
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      editctrl_calc_rect(s, c->rc);
      {
        font_t* fo = s->c->fo;
        int tmHeight = fo->h; // 文本字体的信息
        COLOR _clr = _RGB(0, 0, 0);
        int n = s->str->l;
        int mg = EDIT_MARGIN;
        COLOR crBack = rgb(1, 1, 1);
        COLOR crTxBack = rgb(1, 1, 1);
        COLOR crText = rgb(0, 0, 0);
        COLOR crSelTxBack = rgb(0, 0, 1);
        COLOR crSelText = rgb(1, 1, 1);
        //static int asdf=0; //printf("adfadf %d\n", asdf++);
        imdraw_rect(im, &c->rcclip, rc, crBack, crText, 1, BDM_ALL);
        RCOFFSET(&rc, mg, mg, -mg, -mg);
        // 绘制文本
        if (1) {
          int nBeg = MIN(s->beg, s->pos);
          int nEnd = MAX(s->beg, s->pos);
          char_font_map_t fm[2] = {0};
          char_hittest_t ch[1] = {0};
          IPOINT org = iPOINT(-s->sc->si[0].nPos * EDIT_SB_CHARW, -s->sc->si[1].nPos * fo->h);
          char_font_map_set(fm + 0, s->beg, s->pos, fo, crSelText, crSelTxBack);
          char_font_map_set(fm + 1, 0, s->str->l, fo, crText, crTxBack);
          ch->cursor_pos = s->pos;
          ch->cursor_rpos = s->rpos;
          ch->clrCursor = (s->hot && (GetTickCount() % 1000 > 500)) ? _RGBA(0, 0, 0, 250) : 0;
          //printf("clrCursor = %d\n", ch->clrCursor);
          imdraw_textex(im, &c->rcclip, rc, org, s->str->s, s->str->l, fo->h, fo->bl, s->fmt, 2, fm, ch);
        }
        //printf("%d %d\n", s->org.x, s->org.y);
        //printf("hot = %d\n", s->hot);
        // 设置光标的位置
        if (s->hot && (GetTickCount() % 1000) > 500 && 0) { // && (GetTickCount()/500)&1
          IRECT rc2;
          ISIZE szstr = font_text_size1(fo, s->str->s, s->pos, s->fmt);
          int x = rc.l + s->org.x + szstr.w;
          int y = rc.t + s->org.y + szstr.h - tmHeight;
          int b = 1 + tmHeight / 16;
          rc2 = iRECT(x, y, x + b, y + tmHeight);
          imdraw_fillrect(im, &c->rcclip, rc2, _RGB(0, 0, 0));
        }
      }
      s->sc->c->ctrlproc(c, s->sc->c, hwnd, uMsg, wParam, lParam);
      c->redraw = 0;
      s->sc->c->redraw = 0;
    }
    break;
  }
  return 0;
}
CC_INLINE int editctrl_set(editctrl_t* s, ctrl_t* li, sys_t* sys, const char* text, const char* str, int fmt)
{
  ctrl_set(editctrl_proc, s, s->c, li, text, sys);
  str_setstr(s->str, str, -1);
  s->fmt = fmt;
  return 0;
}

