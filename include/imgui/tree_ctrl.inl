typedef struct tree_item {
  UINT m_state;
  int ind;
  int m_icon;
  char* m_text;
} tree_item;
typedef struct tree_ctrl {
  uictrl base;
  int hit;
  scroll sc[1];
  tree_item* m_items;
  int m_num_items;
  int m_num_memsz;
  int m_cur_items;
  img_t m_im[1];
} tree_ctrl;
int tree_ctrl_get_item_height(const tree_ctrl* s)
{
  int m_text_height = FontHeight();
  int dy = s->m_im->data ? MAX(s->m_im->h, m_text_height) : m_text_height;
  return dy + 2;
}
int tree_ctrl_updata(tree_ctrl* s)
{
  int i;
  int vsib = 0;
  int dy = tree_ctrl_get_item_height(s), dx = 0;

  for (i = 0; i < s->m_num_items; ++i) {
    if (s->m_items[i].m_state & UIS_VISIBLE) {
      const char* buf = s->m_items[i].m_text;
      int dx0 = dy * (s->m_items[i].ind + 2) + FontGetTextSize(buf, -1).w;
      dx = MAX(dx, dx0);
      ++vsib;
    }
  }

  //SetRect(&s->sc->marging, 10, 10, 10, 10);
  //SetRect(&s->sc->marging, 0, 0, 0, 0);
  //scroll_ctrl_updata(s->sc, dx, dy * vsib);
  return 0;
}
int tree_ctrl_insert(tree_ctrl* s, int i, const char* text, int ind, int icon, img_t* im, int state)
{
  i = MIN(i, s->m_num_items);

  if (s->m_num_items >= s->m_num_memsz) {
    s->m_num_memsz = 2 * MAX(1, s->m_num_items);
    REALLOC(tree_item, s->m_items, s->m_num_memsz);
    MEMSET(s->m_items + s->m_num_items, 0, s->m_num_memsz - s->m_num_items);
  }

  MEMMOVE(s->m_items + i + 1, s->m_items + i, (s->m_num_items - i));
  s->m_items[i].m_text = strdup(text);
  s->m_items[i].m_icon = icon;
  s->m_items[i].ind = ind;
  s->m_items[i].ind = ind;
  s->m_items[i].m_state = state | UIS_VISIBLE;
  imsetframe(s->m_im, icon, im);
  s->m_num_items++;
  tree_ctrl_updata(s);
  return 0;
}
int tree_ctrl_next_brother(tree_ctrl* s, int i)
{
  int ind = s->m_items[i].ind;

  for (++i; i < s->m_num_items && s->m_items[i].ind > ind; ++i) {}

  return i;
}
int tree_ctrl_set_visible(tree_ctrl* s, int i, BOOL b)
{
  //if (s->m_items[i].m_state)
  int ind = s->m_items[i].ind;

  if (b) {
    for (++i; i < s->m_num_items && s->m_items[i].ind > ind;) {
      s->m_items[i].m_state |= UIS_VISIBLE;

      if (!(s->m_items[i].m_state & UIS_EXPANDED)) {
        i = tree_ctrl_next_brother(s, i);
      }
      else {
        ++i;
      }
    }
  }
  else {
    for (++i; i < s->m_num_items && s->m_items[i].ind > ind; ++i) {
      s->m_items[i].m_state &= ~UIS_VISIBLE;
    }
  }

  tree_ctrl_updata(s);
  return 0;
}
// 
int tree_ctrl_get_chi_num(tree_ctrl* s, int i)
{
  int j = 0;

  if (s->m_items && i < s->m_num_items) {
    int ind = s->m_items[i].ind;

    for (++i; i < s->m_num_items && s->m_items[i].ind > ind; ++i) {
      ++j;
    }
  }

  return j;
}
// 
int tree_ctrl_get_parent(tree_ctrl* s, int i)
{
  if (s->m_items && i < s->m_num_items) {
    int ind = s->m_items[i].ind;

    for (--i; i >= 0 && s->m_items[i].ind >= ind; --i) {
    }

    return i;
  }

  return -1;
}
BOOL tree_ctrl_is_visible(tree_ctrl* s, int i)
{
  for (; (i = tree_ctrl_get_parent(s, i)) >= 0;) {
    if (!(s->m_items[i].m_state & UIS_EXPANDED)) {
      return FALSE;
    }
  }

  return TRUE;
}
char* tree_ctrl_get_full_path(tree_ctrl* s, int i, const char* slip)
{
  short pathind[256];
  int k = strlen(slip);
  int j, p, n = strlen(s->m_items[i].m_text) + k;
  char* str;
  p = i;

  for (j = 0; (p = tree_ctrl_get_parent(s, p)) >= 0;) {
    n += strlen(s->m_items[p].m_text) + k;
    pathind[j++] = p;
  }

  str = (char*)malloc(n + 1);
  str[0] = 0;
  ASSERT(j < 250);

  for (; --j >= 0;) {
    p = pathind[j];
    strcat(str, s->m_items[p].m_text);
    strcat(str, slip);
  }

  strcat(str, s->m_items[i].m_text);
  return str;
}
// ÅÐ¶ÏÒ»¸ö½ÚµãÊÇ·ñÊÇ×îºóÒ»¸ö
int tree_ctrl_is_last(tree_ctrl* s, int i)
{
  if (s->m_items && i < s->m_num_items - 1) {
    return s->m_items[i].ind > s->m_items[i + 1].ind;
  }

  return TRUE;
}
int tree_ctrl_hittest(const tree_ctrl* s, int y)
{
  int dy = tree_ctrl_get_item_height(s);
  int i, j, n = (y - s->sc->rcsub[SCHT_RCA].top - s->sc->rcsub[SCHT_CLIENT].top) / dy;

  for (j = 0, i = 0; i < n && j < s->m_num_items; ++j) {
    if (s->m_items[j].m_state & UIS_VISIBLE) {
      ++i;
    }
  }

  for (; j < s->m_num_items && !(s->m_items[j].m_state & UIS_VISIBLE); ++j) {}

  return j;
}
int tree_ctrl_get_visible_chi_num(tree_ctrl* s)
{
  int j = 0, i = 0;

  for (j = 0; j < s->m_num_items; ++j) {
    if (s->m_items[j].m_state & UIS_VISIBLE) {
      ++i;
    }
  }

  return i;
}
int tree_ctrl_get_next_visible(tree_ctrl* s, int i)
{
  int j = i + 1;

  for (; j < s->m_num_items; ++j) {
    if (s->m_items[j].m_state & UIS_VISIBLE) {
      return j;
    }
  }

  return j;
}
//»­¼ÓºÅ
// d0 ºñ¶È
int draw_jiahao(HDDC hDC, RECT rc, int d0, int b, COLORREF clr, COLORREF clrBorder)
{
  RECT rc0, rc1, rc2, rc3;
  int d = d0;
  int cx0 = (MIN(RCH(&rc), RCW(&rc)) / 2 - d) / 2;
  POINT pt = RCCENTER(&rc);
  int cx = MAX(cx0 - d - d, d);
  RectAlign(rc, d, d, DT_VCENTER | DT_CENTER, &rc0);
  RectAlign(rc, 0, 0, DT_VCENTER | DT_CENTER, &rc3);

  if (b & 7) {
    if (b & 1) {
      rc1 = rc0, RCOFFSET(&rc1, -cx, 0, cx, 0);
      draw_fillrectR(hDC, &rc1, clr);
    }

    if (b & 2) {
      rc2 = rc0, RCOFFSET(&rc2, 0, -cx, 0, cx);
      draw_fillrectR(hDC, &rc2, clr);
    }

    if (b & 4) {
      rc2 = rc0, RCOFFSET(&rc2, -cx0, -cx0, cx0, cx0);
      draw_rect_broad(hDC, rc2, d, clrBorder);
    }

    rc3 = rc0, RCOFFSET(&rc3, -cx0, -cx0, cx0, cx0);
  }

  if (b & 8) {
    draw_line_dot(hDC, rc.left, pt.y, rc3.left, pt.y, clrBorder);
  }

  if (b & 16) {
    draw_line_dot(hDC, rc3.right, pt.y, rc.right, pt.y, clrBorder);
  }

  if (b & 32) {
    draw_line_dot(hDC, pt.x, rc.top, pt.x, rc3.top, clrBorder);
  }

  if (b & 64) {
    draw_line_dot(hDC, pt.x, rc3.bottom, pt.x, rc.bottom, clrBorder);
  }

  return 0;
}
int draw_tree_ctrl(const tree_ctrl* s, const event* e)
{
  UIHDC(e);
  RECT rc = s->base.rc, rc1;
  int i, first_show_item, last_show_item;
  int dy = tree_ctrl_get_item_height(s);
  COLORREF clrHilite = _GetSysColor(COLOR_HIGHLIGHT);
  COLORREF clrTextHilite = _GetSysColor(COLOR_HIGHLIGHTTEXT);
  COLORREF clrWindowText = _GetSysColor(COLOR_WINDOWTEXT);
  RECT rca = s->sc->rcsub[SCHT_RCA];
  first_show_item = (-rca.top) / dy;
  last_show_item = first_show_item + RCH(&s->base.rc) / dy;
  first_show_item = MAX(first_show_item, 0);
  last_show_item = MIN(last_show_item, s->m_num_items);
  //s->sc->base.rect = *prc;
  //drawaa_scroll_ctrl(hDC, s->sc, prc);
  rc1 = s->sc->rcsub[SCHT_CLIENT];
  HDDC_BEGIN(hDC, &rc1);
  draw_clear(hDC, _RGB(255, 255, 255));
  rc = s->sc->rcsub[SCHT_RCV];
  first_show_item = tree_ctrl_hittest(s, s->base.rc.top);
  last_show_item = tree_ctrl_hittest(s, s->base.rc.bottom);

  if (s->m_num_items > 0 && first_show_item >= 0) {
    last_show_item = last_show_item < 0 ? s->m_num_items - 1 : last_show_item;

    //rc.top += first_show_item * dy;
    for (i = 0; i <= last_show_item; ++i) {
      RECT rc0, rc1;

      if ((s->m_items[i].m_state & UIS_VISIBLE) && RectCutTop(&rc, dy, 0, &rc0)) {
        const char* buf = s->m_items[i].m_text;
        DWORD _clr = clrWindowText;

        if (1) {
          int expanded = s->m_items[i].m_state & UIS_EXPANDED;
          int haschild = s->m_items[i].m_state & UIS_HASCHILD;
          int ind = s->m_items[i].ind;
          int b = haschild ? (expanded ? 5 : 7) : 0;
          //int islast = tree_ctrl_is_last(s, i);
          RectCutLeft(&rc0, dy * ind, 0, &rc1);
          RectCutLeft(&rc0, dy, 0, &rc1);
          draw_jiahao(hDC, rc1, dy / 12, b, rgb(0, 0, 0), _GetSysColor(COLOR_3DSHADOW));
        }

        if (s->m_items[i].m_icon < s->m_im->f) {
          RectCutLeft(&rc0, dy, 0, &rc1);
          RectAlign(rc1, s->m_im->w, s->m_im->h, DT_CENTER | DT_VCENTER, &rc1);
          draw_imageR(hDC, &rc1, s->m_im, s->m_items[i].m_icon, 0, 0);
        }

        rc0.right = s->sc->rcsub[SCHT_CLIENT].right;

        if (i == s->m_cur_items) {
          draw_fillrectR(hDC, &rc0, clrHilite);
          _clr = clrTextHilite;
        }

        draw_textR(hDC, buf, -1, &rc0, DT_VCENTER, _clr);
      }
    }
  }

  HDDC_END(hDC);
  return 0;
}
int tree_ctrl_event(tree_ctrl* s, const event* e)
{
  UIEVT(e);

  if (scroll_event(s->sc, e, s)) {
    return TRUE;
  }

  switch (e->msg) {
  case WM_SIZE:
    s->sc->rect = s->base.rc;
    RCOFFSET(&s->sc->rect, 1, 1, -1, -1);
    //listbox_ctrl_setscroll(s);
    break;

  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (PtInRect(prc, e->pt)) {
      int hit = tree_ctrl_hittest(s, e->pt.y);

      if (hit >= 0 && hit < s->m_num_items) {
        s->hit = hit;
        s->m_cur_items = hit;
        s->m_items[hit].m_state |= UIS_PUSHED;
        force_redraw(e);
        return TRUE;
      }

      return FALSE;
    }

    break;

  case WM_LBUTTONUP:
    if (PtInRect(prc, e->pt)) {
      int hit = tree_ctrl_hittest(s, e->pt.y);

      if (hit == s->hit && hit >= 0 && hit < s->m_num_items) {
        if (s->m_items[hit].m_state & UIS_EXPANDED) {
          s->m_items[hit].m_state &= ~UIS_EXPANDED;
        }
        else {
          s->m_items[hit].m_state |= UIS_EXPANDED;
        }

        UISendEvent(e, s, e->toer, EVENT_LCLICK, hit, 0);
        tree_ctrl_set_visible(s, hit, s->m_items[hit].m_state & UIS_EXPANDED);
      }

      s->hit = -1;
      force_redraw(e);
      return TRUE;
    }

    break;

  case WM_MOUSEMOVE:
    return FALSE;

  case WM_DESTROY:
    if (s->m_items) {
      int i;

      for (i = 0; i < s->m_num_items; ++i) {
        free(s->m_items[i].m_text);
      }

      free(s->m_items);
      s->m_items = 0;
      s->m_num_items = 0;
      s->m_num_memsz = 0;
    }

    return FALSE;
  }

  return FALSE;
}
int tree_ctrl_set(tree_ctrl* s, int n, tree_item* li)
{
  UISETCALL(s, draw_tree_ctrl, tree_ctrl_event);
  s->hit = -1;
  s->m_num_items = n;
  s->m_items = li;
  return 0;
}

