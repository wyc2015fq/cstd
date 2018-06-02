typedef struct editctrl_t {
  ctrl_t c[1];
  int wrapColumnOrWidth; // fmt&WRAP_FIXEDCOLUMNWIDTH 时是列数 否则是像素数
  int charAlign;
  int fmt;
  charpos_t beg, pos;
  int hot;
  scrollview_t sc[1]; // 滚动条
  strdata_t* sd;
} editctrl_t;
int font_char_width(font_t* fo, int c)
{
  font_bit_t fb[1] = {0};
  int ret = fo->getbit(fo, c, fb);
  return ret ? (fo->h / 2) : fb->bx;
}
typedef struct text_style_t {
  COLOR fg, bg;
  //font_t* fo;
} text_style_t;
int str_wrapline(strdata_t* s, str_t str, int i, font_t* fo, int fmt, int wrapColumnOrWidth, int charAlign, int x, int* out_xi, str_t* sline)
{
  int c, cw, ii, w = 0, i0 = i, disx, mindisx = 999999;
  x = MIN(x, wrapColumnOrWidth);
  if (i < str.l) {
    if (WRAP_WIDGETWIDTH & fmt) {
      for (; i < str.l;) {
        c = s->nextchar(&str, i, &ii);
        cw = font_char_width(fo, c);
        w += cw;
        w = ((w + charAlign - 1) / charAlign) * charAlign;
        if (out_xi) {
          disx = abs(x - w);
          if (disx < mindisx) {
            mindisx = disx;
            *out_xi = i;
          }
        }
        if (w > wrapColumnOrWidth) {
          sline->s = str.s + i0;
          sline->l = i - i0;
          return i;
        }
        i = ii;
      }
    }
    else {
      sline->s = str.s + i;
      sline->l = str.l - i;
      i = str.l;
    }
    return i;
  }
  return -1;
}
int editctrl_GetRowHeight(editctrl_t* s)
{
  int h = font_text_size(s->c->fo, "H", 1, 0).h;
  return h + 4;
}
#define MKCHARPOS(row, col) (((int64)(row)<<32)|(col))
charpos_t editctrl_HitTest(editctrl_t* s, int x, int y)
{
  charpos_t pos = 0;
  int i, j, row = 0, col = 0, yline;
  strdata_t* sd = s->sd;
  font_t* fo = s->c->fo;
  int rowHeight = editctrl_GetRowHeight(s);
  ASSERT(rowHeight > 0);
  yline = y / rowHeight;
  yline = MAX(0, yline);
  if (DT_SINGLELINE & s->fmt) {
  }
  else {
    str_t srow, str;
    for (j = 0; (j = sd->nextline(sd, j, &str)) >= 0;) {
      col = 0;
      if (str.l > 0) {
        for (i = 0; (i = str_wrapline(sd, str, i, fo, s->fmt, s->wrapColumnOrWidth, s->charAlign, x, &col, &srow)) > 0;) {
          if (row == yline) {
            return MKCHARPOS(row, col);
          }
          ++row;
        }
      }
      else {
        if (row == yline) {
          return MKCHARPOS(row, col);
        }
        ++row;
      }
    }
  }
  return MKCHARPOS(row, col);
}
charpos_t editctrl_draw(editctrl_t* s, IRECT rc, img_t* im)
{
  charpos_t pos = 0;
  int i, j, row = 0, col;
  strdata_t* sd = s->sd;
  int rowHeight = editctrl_GetRowHeight(s);
  font_t* fo = s->c->fo;
  ASSERT(rowHeight > 0);
  if (DT_SINGLELINE & s->fmt) {
  }
  else {
    str_t srow, str;
    for (j = 0; (j = sd->nextline(sd, j, &str)) >= 0;) {
      for (i = 0; (i = str_wrapline(sd, str, i, fo, s->fmt, s->wrapColumnOrWidth, s->charAlign, 0, &col, &srow)) > 0;) {
      }
    }
  }
  return 0;
}
static LRESULT editctrl_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  editctrl_t* s = (editctrl_t*)c->x;
  IRECT rcc;//, rc1 = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int ret = 0;
  charpos_t hit;
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    s->hot = 0;
    s->charAlign = c->fo->h / 2;
    return 0;
  }
  if (WM_PAINT != uMsg) {
    if (ret = scrollview_proc(s->sc, c, hwnd, uMsg, wParam, lParam)) {
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
  case WM_SIZE:
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    if (s->fmt & DT_SINGLELINE) {
      mminfo->ptMinTrackSize.x = 120;
      mminfo->ptMinTrackSize.y = editctrl_GetRowHeight(s);
    }
    else {
      mminfo->ptMinTrackSize.x += 320;
      mminfo->ptMinTrackSize.y += 320;
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
      force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (c->sys->ldown && iPtInRect(&rcc, x, y)) {
      if (ctrl_isfocus(c)) {
        //editctrl_charfrompos(s, rcc, x, y);
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
      hit = editctrl_HitTest(s, x, y);
      s->hot = TRUE;
      //s->rdown = 0;
      // 窗体获得焦点以后设置光标的位置
      // 从当前鼠标的位置获得对应字符的序号
      //editctrl_charfrompos(s, rcc, x, y);
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
        //editctrl_insertstr(s, str, 1);
        //editctrl_calc_rect(s, c->rc);
      }
      //printf("%d\n", nChar);
    }
    break;
  case WM_KEYDOWN:
    if (s->hot) {
      UINT nChar = wParam;
      //const char* cText = s->str->s + s->pos;
      //printf("%d %d\n", nChar, GetKeyState(VK_SHIFT));
      // 此处处理Ctrl+C、Ctrl+V和Ctrl+X操作
      switch (nChar) {
      case 13:
        if (!(s->fmt & DT_SINGLELINE)) {
          char str[8] = {'\n', 0};
          //editctrl_insertstr(s, str, 1);
        }
        break;
      case 'C':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          //CopyString(hwnd, s->str->s + s->beg, SELECTLEN);
        }
        break;
      case 'V':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          char* str = PasteString(hwnd);
          int slen = _tcslen(str);
          //editctrl_insertstr(s, str, slen);
          //editctrl_calc_rect(s, c->rc);
          free(str);
        }
        break;
      case 'X':
        if (GetKeyState(VK_CONTROL) < 0) { //被按下
          if (s->beg != s->pos) {//Ctrl+X
            //CopyString(hwnd, s->str->s + s->beg, SELECTLEN);
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
          IPOINT pt = {0};
          if (VK_LEFT == wParam || VK_RIGHT == wParam) {
            if (VK_LEFT == wParam) {
              //s->pos = chr_prev(s->str, s->pos);
            }
            else if (VK_RIGHT == wParam) {
              //int k = CharNext(cText) - cText;
              //s->pos += k;
            }
            //pt = str_pos2pt(s->str, s->pos);
          }
          else {
            //IPOINT ptmax = str_pos2pt(s->str, s->str->l);
            //pt = str_pos2pt(s->str, s->pos);
            if (VK_UP == wParam) {
              pt.y--;
            }
            else if (VK_DOWN == wParam) {
              pt.y++;
            }
            //pt.y = BOUND(pt.y, 0, ptmax.y);
            //s->pos = str_pt2pos(s->str, pt);
          }
          //s->rpos = pt.y;
          //printf("pos %d pt %d %d\n", s->pos, pt.x, pt.y);
          //s->pos = BOUND(s->pos, 0, s->str->l);
          if (GetKeyState(VK_SHIFT) >= 0) {
            s->beg = s->pos;
          }
          force_redraw(c);
        }
        break;
      case VK_END:
      case VK_HOME:
        if (s) {
          //IPOINT pt = str_get_line(s->str, s->pos);
          //s->pos = (VK_HOME == wParam) ? pt.x : pt.y;
          if (GetKeyState(VK_SHIFT) > 0) {
            s->beg = s->pos;
          }
          force_redraw(c);
        }
        break;
      case VK_BACK:
        if (s) {
          // 响应用户删除操作 backspace
          if (1) {
            //s->beg = chr_prev(s->str, s->pos);
          }
          //editctrl_del_select(s);
          //editctrl_calc_rect(s, c->rc);
          force_redraw(c);
        }
        break;
      case VK_DELETE:
        if (s) {
          if (1) {
            //s->beg = chr_next(s->str, s->pos);
          }
          //editctrl_del_select(s);
          //editctrl_calc_rect(s, c->rc);
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
      IRECT prc[14] = {0};
      scrollview_draw(s->sc, c, im, prc);
      editctrl_draw(s, *prc, im);
    }
    c->redraw = 0;
    break;
  }
  return 0;
}
CC_INLINE int editctrl_set(editctrl_t* s, ctrl_t* li, sys_t* sys, const char* text, const char* str, int fmt)
{
  ctrl_set(editctrl_proc, s, s->c, li, text, sys);
  s->fmt = fmt;
  return 0;
}

