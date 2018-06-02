typedef struct xlistbox_t {
  uictrl base;
  int hit;
  int hot;
  scroll sc[1];
  vstr_t items[1];
  int cur;
  IPOINT org;
} xlistbox_t;
int listbox_ctrl_get_item_height()
{
  int m_text_height = FontGetTextSize("H", 1).h;
  int dy = m_text_height + 6, dx = 0;
  return dy;
}
int listbox_ctrl_get_item_maxwidth(xlistbox_t* s)
{
  int i, mw = 100;

  for (i = 0; i < s->items->n; ++i) {
    str_t str = s->items->v[i];
    int dx = FontGetTextSize(str.s, str.l).w;
    mw = MAX(mw, dx);
  }

  return mw;
}

int listbox_ctrl_hittest(xlistbox_t* s, POINT pt)
{
  int dy = listbox_ctrl_get_item_height();
  RECT rc = s->sc->rcsub[SCHT_CLIENT];
  RECT rca = s->sc->rcsub[SCHT_RCA];
  int i = (pt.y - rca.top - rc.top) / dy;

  if (i >= 0 && i < s->items->n) {
    return i;
  }

  return -1;
}

int draw_listbox_ctrl(const xlistbox_t* s, const event* e)
{
  UIHDC(e);
  RECT rc, rcClient, rc0;
  int i, first_show_item, last_show_item;
  int dy = listbox_ctrl_get_item_height(), dx = 0;
  RECT rca = s->sc->rcsub[SCHT_RCA];
  DrawRect(hDC, s->base.rc, res->clrBtnWindow, res->clrBtnText);
  first_show_item = (-rca.top) / dy;
  last_show_item = 1 + first_show_item + RCH(&s->base.rc) / dy;
  first_show_item = MAX(first_show_item, 0);
  last_show_item = MIN(last_show_item, s->items->n);
  draw_scroll(s->sc, e);
  rcClient = s->sc->rcsub[SCHT_CLIENT];
  HDDC_BEGIN(hDC, &rcClient);
  //draw_fillrectR(hDC, &rcClient, res->clrBtnWindow);
  RectOffset(rca, rcClient.left, rcClient.top, rcClient.left, rcClient.top, &rc);
  rc.top += first_show_item * dy;
  rc.right = rcClient.right;

  for (i = first_show_item; i < last_show_item; ++i) {
    if (RectCutTop(&rc, dy, 0, &rc0)) {
      str_t str = s->items->v[i];
      DWORD crText = res->clrWindowText;

      if (i == s->cur) {
        draw_fillrectR(hDC, &rc0, res->clrHilite);
        crText = res->clrTextHilite;
      }
      else if (i == s->hot) {
        draw_fillrectR(hDC, &rc0, _RGB_A(res->clrHilite, 150));
        crText = res->clrTextHilite;
      }

      draw_textR(hDC, str.s, str.l, &rc0, DT_VCENTER, crText);
    }
  }

  HDDC_END(hDC);
  return 0;
}

int listbox_ctrl_setscroll(xlistbox_t* s)
{
  int dy = listbox_ctrl_get_item_height();
  SIZE sz = iSIZE(50, dy * s->items->n);
  RECT rca = cRECT3(s->org, sz);
  RECT rcv = s->sc->rect;
  scroll_getrect(s->sc, rca, rcv);
  s->org = RCLT(&s->sc->rcsub[SCHT_RCA]);
  return 0;
}
int listbox_ctrl_event(xlistbox_t* s, const event* e)
{
  UIEVT(e);

  if (scroll_event(s->sc, e, s)) {
    return TRUE;
  }

  switch (e->msg) {
  case WM_SIZE:
    s->sc->rect = s->base.rc;
    RCOFFSET(&s->sc->rect, 1, 1, -1, -1);
    listbox_ctrl_setscroll(s);
    break;

  case EVENT_GETMINMAXINFO:
    if (e->wp) {
      LPMINMAXINFO lpMMI = (LPMINMAXINFO) e->wp;
      int dy = listbox_ctrl_get_item_height() * s->items->n;
      int dx = listbox_ctrl_get_item_maxwidth(s);
      lpMMI->ptMinTrackSize = iPOINT(100, 100);
      lpMMI->ptMaxTrackSize = iPOINT(dx + 2, dy + 2);
    }

    break;

  case WM_MOUSEWHEEL:
    if (PtInRect(prc, e->pt)) {
      s->org.y += GET_WHEEL_DELTA_WPARAM(e->wp) / 3;
      listbox_ctrl_setscroll(s);
      force_redraw(e);
    }

    break;

  case EVENT_INIT:
    UISETCALL(s, draw_listbox_ctrl, listbox_ctrl_event);
    s->hit = -1;
    break;

  case EVENT_EXIT:
    strv_free(s->items);
    break;

  case EVENT_SETATTRIBUTE:
    if (e->wp) {
      str_t* str = (str_t*)e->wp;

      if (0 == mystricmp(str->s, str->l, "data", 4)) {
        strv_setstr(s->items, str[1], '\n', " \t\t\n");
      }

      return 1;
    }

    break;

  case EVENT_SCROLL_SETPOS:
    if (1) {
      if (e->wp < 8 && e->wp >= 0) {
        POINT wwp[8] = {0};
        int lx = 40, ly = 40, px = RCW(&s->sc->rcsub[SCHT_RCV]), py = RCH(&s->sc->rcsub[SCHT_RCV]);
        wwp[SCHT_LINEUP] = iPOINT(0, ly);
        wwp[SCHT_LINELEFT] = iPOINT(lx, 0);
        wwp[SCHT_LINEDOWN] = iPOINT(0, -ly);
        wwp[SCHT_LINERIGHT] = iPOINT(-lx, 0);

        wwp[SCHT_PAGEUP] = iPOINT(0, py);
        wwp[SCHT_PAGELEFT] = iPOINT(px, 0);
        wwp[SCHT_PAGEDOWN] = iPOINT(0, -py);
        wwp[SCHT_PAGERIGHT] = iPOINT(-px, 0);
        PTADD(s->org, s->org, wwp[e->wp]);
      }
      else {
        s->org = RCLT(&s->sc->rcsub[SCHT_RCA]);
      }

      if (e->lp) {
        listbox_ctrl_setscroll(s);
      }

      force_redraw(e);
    }

    break;

  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (PtInRect(&s->sc->rcsub[SCHT_CLIENT], e->pt)) {
      int hit = listbox_ctrl_hittest(s, e->pt);

      if (s->hit != hit) {
        s->hit = hit;

        if (hit >= 0 && hit < s->items->n) {
          s->cur = hit;
        }

        force_redraw(e);
        return TRUE;
      }
    }

    break;

  case WM_LBUTTONUP:
    if (1) {
      s->hit = -1;

      if (PtInRect(&s->sc->rcsub[SCHT_CLIENT], e->pt)) {
        UISetPopup(0);
      }

      force_redraw(e);
    }

    break;

  case WM_MOUSEMOVE:
    if (PtInRect(&s->sc->rcsub[SCHT_CLIENT], e->pt)) {
      int hit = listbox_ctrl_hittest(s, e->pt);

      if (s->hot != hit) {
        s->hot = hit;
        force_redraw(e);
      }

      return TRUE;
    }

    break;
  }

  return FALSE;
}
int listbox_ctrl_set(xlistbox_t* s)
{
  UISETCALL(s, draw_listbox_ctrl, listbox_ctrl_event);
  s->hit = -1;
  return 0;
}

