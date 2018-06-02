
#ifndef _EDIT_INL_
#define _EDIT_INL_

typedef IPOINT charpos_t;
typedef struct {
  int c;
  int nch;
  COLOR fg, bg;
} char_color_t;
typedef struct {
  int bbeg, bend;
  int cbeg, cend;
  COLOR fg, bg;
} text_range_t;
typedef struct strdata_t_ strdata_t;
struct strdata_t_ {
  int (*get_line)(strdata_t* s, int irow, int icol, text_range_t* out, int maxoutlen, int stage); // 获取i行str不含 \n
  int (*ins_str)(strdata_t* s, const char* str, int len); // 用str 替换 [p1,p2]的内容
  int (*set_pos)(strdata_t* s, int x, int y);
  int (*move_pos)(strdata_t* s, int ichr, int iline);
  int (*get_sel)(strdata_t* s, str_t* str);
  charpos_t beg, pos;
  str_t s[1];
  vstr_t sv[1];
  buf_t bf[1];
};
typedef struct {
  IRECT rc;
  int fmt;
  IPOINT org;
  font_t* font;
  strdata_t sd[1];
} editctrl_t;


#define STRDATA_INITFONC(s, name) s->get_sel = name##_get_sel, s->get_line = name##_get_line, s->ins_str = name##_ins_str, s->set_pos = name##_set_pos, s->move_pos = name##_move_pos
#define PTTOINT(pt) ((pt).y*(1<<16) + (pt).x)
int charpos_inrange(charpos_t beg, charpos_t pos, int x, int y)
{
  IPOINT pt;
  pt.x = x, pt.y = y;
  return (PTTOINT(beg) <= PTTOINT(pt) && PTTOINT(pt) < PTTOINT(pos)) || (PTTOINT(pos) <= PTTOINT(pt) && PTTOINT(pt) < PTTOINT(beg));
}
int strdata_strv_get_sel(strdata_t* s, str_t* s0)
{
  return 0;
}
int strdata_buf_get_sel(strdata_t* s, str_t* s0)
{
  char* str = (char*)s->bf->data;
  int len = strlen(str);
  int ibeg = str_skip_nline(str, len, 0, s->beg.y, "\n", 0) + s->beg.x;
  int ipos = str_skip_nline(str, len, 0, s->pos.y, "\n", 0) + s->pos.x;
  int i = MIN(ibeg, ipos);
  int n = MAX(ibeg, ipos) - i;
  str_setstr(s0, str + i, n);
  return 0;
}
int strdata_buf_get_line1(strdata_t* s, int irow, int icol, char_color_t* out, int maxoutlen)
{
  char* str = (char*)s->bf->data;
  int len = strlen(str);
  int i, i1, i0 = str_skip_nline(str, len, 0, irow, "\n", 0), j = 0;
  if (i0 == len) {
    return -1;
  }
  for (i = i0; i < len && str[i] != '\n' && str[i] != '\r' && j < maxoutlen; ++j) {
    ushort ch = chr_next(str, i, &i1);
    out[j].c = ch;
    out[j].nch = (i1 - i);
    if (charpos_inrange(s->beg, s->pos, i - i0, irow)) {
      out[j].bg = _rgb(0, 0, 0);
      out[j].fg = _rgb(1, 1, 1);
    }
    else {
      out[j].bg = _rgb(1, 1, 1);
      out[j].fg = _rgb(0, 0, 0);
    }
    if ('\t' == ch) {
      out[j].fg = 0;
    }
    i = i1;
  }
  return j;
}
int strdata_buf_get_line(strdata_t* s, int irow, int icol, text_range_t* out, int maxoutlen, int isfocus)
{
  char* str = (char*)s->bf->data;
  int len = strlen(str);
  int i, i1, i0 = str_skip_nline(str, len, 0, irow, "\n", 0), j = 0;
  int bpos = 9999, cpos = 9999;
  int prev_char_inrange = -1, n = 0;
  if (i0 == len) {
    return -1;
  }
  for (i = i0; n < maxoutlen && i < len && str[i] != '\n' && str[i] != '\r' && j < maxoutlen; ++j) {
    ushort ch = chr_next(str, i, &i1);
    int isinrange = !isfocus ? 0 : charpos_inrange(s->beg, s->pos, i - i0, irow);
    if (prev_char_inrange != isinrange) {
      if (prev_char_inrange >= 0) {
        out[n].bbeg = bpos;
        out[n].bend = i;
        out[n].cbeg = cpos;
        out[n].cend = j;
        out[n].bg = prev_char_inrange ? _rgb(0, 0, 1) : 0;
        out[n].fg = prev_char_inrange ? _rgb(1, 1, 1) : _rgb(0, 0, 0);
        ++n;
      }
      prev_char_inrange = isinrange;
      bpos = i;
      cpos = j;
    }
    if ('\t' == ch) {
      out[j].fg = 0;
    }
    i = i1;
  }
  if (i1 > bpos) {
    out[n].bbeg = bpos;
    out[n].bend = i1;
    out[n].cbeg = cpos;
    out[n].cend = j + 1;
    out[n].bg = prev_char_inrange ? _rgb(0, 0, 1) : 0;
    out[n].fg = prev_char_inrange ? _rgb(1, 1, 1) : _rgb(0, 0, 0);
    ++n;
  }
  return n;
}
int strdata_buf_ins_str(strdata_t* s, const char* s0, int l0)
{
  char* str = (char*)s->bf->data;
  int len = strlen(str);
  int ibeg = str_skip_nline(str, len, 0, s->beg.y, "\n", 0) + s->beg.x;
  int ipos = str_skip_nline(str, len, 0, s->pos.y, "\n", 0) + s->pos.x;
  int i = MIN(ibeg, ipos);
  int n = MAX(ibeg, ipos) - i;
  int m = memrep(str, len, i, n, s0, l0, str, s->bf->len);
  str[m] = 0;
  s->beg = s->pos = str_pos2pt(str, i + l0, "\n");
  return 0;
}
int strdata_buf_set_pos(strdata_t* s, int x, int y)
{
  char* str = (char*)s->bf->data;
  int len = strlen(str);
  int i, ipos, i0 = str_skip_nline(str, len, 0, y, "\n", &s->pos.y);
  if (i0 == len) {
    i0 = str_skip_nline(str, len, 0, s->pos.y, "\n", &s->pos.y);
  }
  for (i = i0; i < len && str[i] != '\n' && (i - i0) < x;) {
    chr_next(str, i, &i);
  }
  s->pos.x = i - i0;
  ipos = i;
  return 0;
}
int strdata_buf_move_pos(strdata_t* s, int ichr, int iline)
{
  char* str = (char*)s->bf->data;
  int len = strlen(str);
  int ipos;
  ipos = str_skip_nline(str, len, 0, s->pos.y + iline, "\n", 0) + s->pos.x;
  if (ichr < 0) {
    for (; ichr++ && ipos >= 0;) {
      chr_prev(str, ipos, &ipos);
    }
  }
  else {
    for (; ichr-- && ipos < len;) {
      chr_next(str, ipos, &ipos);
    }
  }
  s->pos = str_pos2pt(str, ipos, "\n");
  return 0;
}
int editctrl_get_char_w(const editctrl_t* s)
{
  int char_w = measureText("H", NULL, s->font, 0, 0, NULL).h / 2;
  return char_w;
}
int editctrl_get_line_h(const editctrl_t* s)
{
  int line_h = measureText("H", NULL, s->font, 0, 0, NULL).h;
  return line_h;
}
charpos_t editctrl_HitTest(editctrl_t* s, int x, int y)
{
  charpos_t pos = {0};
  return pos;
}
int editctrl_set_pos(editctrl_t* s, int x, int y)
{
  int line_h = editctrl_get_line_h(s);
  int char_w = editctrl_get_char_w(s);
  int xx = (x - s->rc.l - s->org.x) / char_w;
  int yy = (y - s->rc.t - s->org.y) / line_h;
  s->sd->set_pos(s->sd, xx, yy);
  return 0;
}
int editctrl_ins_str(editctrl_t* s, const char* str, int len, int to_del)
{
  if (to_del && PTTOINT(s->sd->beg) == PTTOINT(s->sd->pos)) {
    s->sd->move_pos(s->sd, to_del, 0);
  }
  s->sd->ins_str(s->sd, str, len);
  return 0;
}
//#define EDIT_MARGIN 1
IRECT editctrl_get_cursor_rect(editctrl_t* s, IRECT rc)
{
  IRECT rc2;
  int x, y, b, line_h, char_w;
  line_h = editctrl_get_line_h(s);
  char_w = editctrl_get_char_w(s);
  //ctrl_get_rect(s->c, s->rc, 0, 0, &rc);
  x = rc.l + s->org.x + char_w * s->sd->pos.x;
  y = rc.t + s->org.y + line_h * s->sd->pos.y;
  b = MAX(1, line_h / 20);
  rc2 = iRECT(x, y, x + b, y + line_h);
  return rc2;
}
CC_INLINE int edit_ctrl(ctrl_t* c, const char* label, char* buf, int len)
{
  SYSIO;
  static editctrl_t s11[1] = {0};
  editctrl_t* s = s11;
  int ret = 0, hot = 0;
  IRECT rcc, rc = c->rc;
  int mg = 2;
  IRECT rcLabel = {0};
  static int ttt = 0;
  s11->font = io->font;
  STRDATA_INITFONC(s->sd, strdata_buf);
  if (2 == ttt) {
    int asdfasdf = 0;
  }
  if (*buf == 0 && s->sd->pos.x > 0) {
    int asdf = 0;
  }
  s->sd->bf->data = (uchar*)buf;
  s->sd->bf->len = len;
  s->fmt = TF_VCENTER | TF_NoWrap;
  if (1) {
    c->sz = measureText("H", NULL, io->font, 0, 0, NULL);
    c->sz.h +=4;
    if (label) {
      ISIZE sz2 = measureText(label, NULL, io->font, 0, 0, NULL);
      c->sz.w += sz2.w + 48;
    }
  }
  rcc = rc;
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rcc, x, y);
    switch (io->msg) {
    case MSG_MOUSEMOVE:
      if (ISFOCUS(c) && ISHIT(c)) {
        editctrl_set_pos(s, x - rc.l, y - rc.t);
        //editctrl_update_org(s);
        //force_redraw(c);
      }
      break;
    case MSG_MOUSEWHEEL:
      if (hot) {
        //s->org.y += GET_WHEEL_DELTA_WPARAM(nChar) / 3;
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        SETFOCUS(c);
        ttt++;
        if (mouse->double_clicked[0]) {
          //s->c->sys->cursor_show = 1;
          editctrl_set_pos(s, x - rc.l, y - rc.t);
          s->sd->set_pos(s->sd, 0, s->sd->pos.y);
          s->sd->beg = s->sd->pos;
          s->sd->set_pos(s->sd, 9999999, s->sd->pos.y);
          //force_redraw(c);
        }
        else {
          //s->c->sys->cursor_show = 1;
          //hit = editctrl_HitTest(s, x, y);
          //s->rdown = 0;
          // 窗体获得焦点以后设置光标的位置
          // 从当前鼠标的位置获得对应字符的序号
          //editctrl_charfrompos(s, rcc, x, y);
          if (io->keyshift) {
            s->sd->beg = s->sd->pos;
          }
          editctrl_set_pos(s, x - rc.l, y - rc.t);
          s->sd->beg = s->sd->pos;
          //force_redraw(c);
        }
      }
      break;
    case MSG_LBUTTONUP:
      //if (io->Mouse) { force_redraw(c); }
      break;
    case MSG_CHAR:
      if (ISFOCUS(c)) {
        int nChar = io->last_char;
        if (nChar > KEY_COUNT || '\t' == nChar || (('\r' == nChar || '\n' == nChar) && !(s->fmt & TF_NoWrap))) {
          static char str[8] = {0};
          if (str[0] == 0 && nChar & 0x80) {
            str[0] = nChar;
          }
          else {
            //printf("key %d\n", nChar);
            // 此处进行一般输入字符的处理，如果当前有被选中的字符串，先进行删除操作
            int cn = 1;
            if (str[0]) {
              str[1] = nChar;
              cn = 2;
            }
            else {
              str[0] = nChar;
            }
            editctrl_ins_str(s, str, cn, 0);
            str[0] = 0;
          }
          //editctrl_calc_rect(s, c->rc);
          //force_redraw(c);
        }
      }
      break;
    case MSG_KEYDOWN:
      if (ISFOCUS(c)) {
        UINT nChar = io->last_key;
        //const char* cText = s->str->s + s->sd->pos;
        //printf("%d %d\n", nChar, GetKeyState(KEY_SHIFT));
        // 此处处理Ctrl+C、Ctrl+V和Ctrl+X操作
        switch (nChar) {
        case 'C':
          if (io->keyctrl) { //被按下
            str_t str[1] = {0};
            s->sd->get_sel(s->sd, str);
            sys_set_clipboard_text(str->s, str->l);
            str_free(str);
          }
          break;
        case 'V':
          if (io->keyctrl) { //被按下
            char* str = sys_get_clipboard_text_static();
            int slen = strlen(str);
            editctrl_ins_str(s, str, slen, 0);
            //editctrl_calc_rect(s, c->rc);
            free(str);
          }
          break;
        case 'X':
          if (io->keyctrl) { //被按下
            if (1) {//Ctrl+X
              str_t str[1] = {0};
              s->sd->get_sel(s->sd, str);
              sys_set_clipboard_text(str->s, str->l);
              str_free(str);
              editctrl_ins_str(s, 0, 0, 0);
              //editctrl_del_select(s);
              //editctrl_calc_rect(s, c->rc);
            }
          }
          break;
        case KEY_UP:
        case KEY_DOWN:
        case KEY_RIGHT:
        case KEY_LEFT:
          if (s) {
            IPOINT pos = {0};
            if (KEY_LEFT == nChar || KEY_RIGHT == nChar) {
              pos.x = (KEY_LEFT == nChar) ? -1 : 1;
            }
            else {
              pos.y = (KEY_UP == nChar) ? -1 : 1;
            }
            s->sd->move_pos(s->sd, pos.x, pos.y);
            if (!io->keyshift) {
              s->sd->beg = s->sd->pos;
            }
            force_redraw(c);
          }
          break;
        case KEY_END:
        case KEY_HOME:
          if (s) {
            IPOINT pos = s->sd->pos;
            pos.x = (KEY_HOME == nChar) ? 0 : 9999999;
            editctrl_set_pos(s, pos.x, pos.y);
            if (io->keyshift) {
              s->sd->beg = s->sd->pos;
            }
            force_redraw(c);
          }
          break;
        case KEY_BACK:
          if (s) {
            // 响应用户删除操作 backspace
            editctrl_ins_str(s, 0, 0, -1);
            force_redraw(c);
          }
          break;
        case KEY_DELETE:
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
    }
  }
  if (1) {
    //IRECT rcc;
    //iRectOffset2(&rcc, -rcc.l, -rcc.t);
    int i, j, len, y = s->org.y;
    strdata_t* sd = s->sd;
    text_range_t range[16];
    int line_h = editctrl_get_line_h(s);
    int char_w = editctrl_get_char_w(s);
    COLOR crSel = _rgba(0, 0, 1, 1);
    COLOR crText = _rgb(0, 0, 0);
    BOOL isfocus = ISFOCUS(c);
    line_h = MAX(line_h, 1);
    i = (-y) / line_h;
    y += i * line_h;
    if (label) {
      int fmt = TF_RIGHT | TF_VCENTER;
      gcSolidText(g, rcLabel, label, NULL, io->font, 0, fmt, crText);
    }
    gcEdgeRect1(g, rc, hot ? _rgba(1, 1, 1, 0.9) : _rgba(1, 1, 1, 0.5), crText, 1);
    //ctrl_get_rect(s->c, rc, 0, 0, &rc);
    RCOFFSET(&rc, mg, mg, -mg, -mg);
    gcClipRectR(g, rc);
    for (; (len = sd->get_line(sd, i, 0, range, countof(range), isfocus)) >= 0 && (rc.t + y) < rc.b; ++i) {
      //IRECT rc1 = iRectInter(rc, );
      IRECT rc1 = iRECT2(rc.l + s->org.x, rc.t + y, 9999, line_h);
      int beg = 0, end = 0;
      int begw = 0, endw = 0, w = 0, k = 0;
      for (j = 0; j < len; ++j) {
        char* str = (char*)s->sd->bf->data;
        ISIZE sz = measureText(str + range[j].bbeg, str + range[j].bend, io->font, 0, s->fmt, NULL);
        rc1 = iRECT2(rc.l + w, rc.t + y, sz.w + 1, line_h);
        gcSolidRect2(g, rc1, range[j].bg);
        if (range[j].bg) {
        }
        if (s->fmt & TF_NoWrap) {
          //iRectAlign(rc1, sz.w, sz.h, TF_VCENTER, &rc1);
        }
        gcSolidText(g, rc1, str + range[j].bbeg, str + range[j].bend, io->font, 0, s->fmt, range[j].fg);
        w += sz.w;
      }
      y += line_h;
    }
    gcClipReset(g);
    if (ISFOCUS(c)) {
      IRECT rcCursor = editctrl_get_cursor_rect(s, rc);
      int n = 500;
      int a = (io->time) % (n*2);
      if (a>n) {
        COLOR crCursor = _RGB(0, 0, 0);
        gcEdgeRect1(g, rcCursor, crCursor, 0, 0);
      }
    }
  }
  return ret;
}


#endif // _EDIT_INL_
