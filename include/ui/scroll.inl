static int scroll_set_pos(SCROLLINFO* si, int nPos)
{
  si->nPos = (int)BOUND(nPos, si->nMin, si->nMax - (int)si->nPage);
  return 0;
}
static int scroll_set_info(SCROLLINFO* si, int nMin, int nMax, int nPage)
{
  ASSERT(nMin < nMax);
  si->nMin = nMin;
  si->nMax = nMax;
  si->nPage = MIN(nPage, nMax);
  scroll_set_pos(si, si->nPos);
  return 0;
}
CC_INLINE int scroll_calc_rect_impl(SCROLLINFO* si, IRECT rc, int sz, int isv, IRECT* out)
{
  int nPos = si->nPos, nPage = si->nPage, nMin = si->nMin, nMax = si->nMax;
  int len = nMax - nMin;
  int thumb_min = (len > 0) ? THUMB_MIN : 0;
  int c = 0, d = 0;
  memset(out, 0, sizeof(IRECT) * 6);
  if (isv) {
    int l = rc.r - sz, a = rc.t + sz, b = rc.b - sz;
    if (len > 0) {
      int ll = b - a - THUMB_MIN;
      c = a + nPos * ll / len, d = THUMB_MIN + a + ((nPos + nPage) * ll + len - 1) / len;
      d = BOUND(d, c, b);
      if (c == a && d == b) {
        c = d = 0;
      }
    }
    out[SBHT_LINE_0] = iRECT(l, rc.t, rc.r, a);
    out[SBHT_LINE_1] = iRECT(l, b, rc.r, rc.b);
    out[SBHT_PAGE_0] = iRECT(l, a, rc.r, c);
    out[SBHT_PAGE_1] = iRECT(l, d, rc.r, b);
    out[SBHT_THUMB] = iRECT(l, c, rc.r, d);
    return d < b;
  }
  else {
    int l = rc.b - sz, a = rc.l + sz, b = rc.r - sz;
    if (len > 0) {
      int ll = b - a - THUMB_MIN;
      c = a + nPos * ll / len, d = THUMB_MIN + a + ((nPos + nPage) * ll + len - 1) / len;
      d = BOUND(d, c, b);
      if (c == a && d == b) {
        c = d = 0;
      }
    }
    out[SBHT_LINE_0] = iRECT(rc.l, l, a, rc.b);
    out[SBHT_LINE_1] = iRECT(b, l, rc.r, rc.b);
    out[SBHT_PAGE_0] = iRECT(a, l, c, rc.b);
    out[SBHT_PAGE_1] = iRECT(d, l, b, rc.b);
    out[SBHT_THUMB] = iRECT(c, l, d, rc.b);
    return d < b;
  }
  return 0;
}
CC_INLINE int scroll_calc_rect(scroll_t* s, ctrl_t* c, IRECT* prc)
{
  int i = 0, sz = SBW1;
  IRECT rcc, rc = c->rc;
  rcc = rc;
  if (c->style & WS_HSCROLL) {
    IRECT rc1 = rc;
    i = 0;
    rc1.r -= (c->style & WS_VSCROLL) ? sz : 0;
    if (scroll_calc_rect_impl(s->si + i, rc1, sz, i, prc + 2)) {
    }
    rcc.r -= sz;
  }
  if (c->style & WS_VSCROLL) {
    IRECT rc1 = rc;
    i = 1;
    rc1.b -= (c->style & WS_HSCROLL) ? sz : 0;
    if (scroll_calc_rect_impl(s->si + i, rc1, sz, i, prc + 2 + 6)) {
    }
    rcc.b -= sz;
  }
  prc[0] = rcc;
  prc[1] = iRECT2(rcc.r, rcc.b, sz, sz);
  return 0;
}
CC_INLINE int scroll_hittest(scroll_t* s, ctrl_t* c, int x, int y)
{
  int i = 0, j;
  IRECT prc[14] = {0};
  scroll_calc_rect(s, c, prc);
  if (iPtInRect(prc, x, y) || iPtInRect(prc + 1, x, y)) {
    return 0;
  }
  for (i = 0; i < 2; ++i) {
    IRECT* rcb = prc + 2 + i * 6;
    for (j = 0; j < 5; ++j) {
      if (iPtInRect(rcb + j, x, y)) {
        return (i << 8) | (j + 1);
      }
    }
  }
  return 0;
}
CC_INLINE int scroll_draw(scroll_t* s, ctrl_t* c, img_t* im, IRECT* prc)
{
  COLOR cr3dFace = _GetSysColor(CLR_3DFACE);
  COLOR clrs[] = {_RGB(255, 255, 255), cr3dFace};
  int nState[4] = {IdArowLeftLarge, IdArowRightLarge, IdArowUpLarge, IdArowDownLarge, };
  int i;
  scroll_calc_rect(s, c, prc);
  imdraw_rect(im, &c->rcclip, prc[1], cr3dFace, 0, 0, 0);
  //printf("WM_PAINT scroll\n");
  for (i = 0; i < 2; ++i) {
    IRECT* rcb = prc + 2 + i * 6;
    imdraw_rect_colors(im, &c->rcclip, rcb[SBHT_PAGE_0], clrs, 2, 0);
    imdraw_rect_colors(im, &c->rcclip, rcb[SBHT_PAGE_1], clrs, 2, 0);
    imdraw_border_syscolor(im, &c->rcclip, rcb[SBHT_LINE_0], (i == (s->hit >> 8) && SBHT_LINE_0 == ((s->hit & 0xff) - 1)) ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
    imdraw_menuimg(im, &c->rcclip, rcb[SBHT_LINE_0], nState[i * 2 + 0], _rgb(0, 0, 0));
    imdraw_border_syscolor(im, &c->rcclip, rcb[SBHT_LINE_1], (i == (s->hit >> 8) && SBHT_LINE_1 == ((s->hit & 0xff) - 1)) ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
    imdraw_menuimg(im, &c->rcclip, rcb[SBHT_LINE_1], nState[i * 2 + 1], _rgb(0, 0, 0));
    imdraw_border_syscolor(im, &c->rcclip, rcb[SBHT_THUMB], BDS_HIGH, BDM_ALL);
  }
  return 0;
}
//scroll_proc
CC_INLINE LRESULT scroll_proc(scroll_t* s, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  IRECT rc = c->rc;
  int hit = 0, x = LOWORD(lParam), y = HIWORD(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO:
    if (s) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      mminfo->ptMinTrackSize.x = SBW1 * 3;
      mminfo->ptMinTrackSize.y = SBW1 * 3;
    }
    break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (s->hit) {
      s->hit = 0;
      force_redraw(c);
    }
    break;
#if 0
  case WM_MOUSEWHEEL:
    if (iPtInRect(&s->rcc, x, y)) {
      int i = 1;
      //printf("WM_MOUSEWHEEL SBHT_PAGE_1\n");
      s->si[i].nTrackPos -= GET_WHEEL_DELTA_WPARAM(wParam);
      s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - (int)s->si[i].nPage);
      sendmsg(c, sender, WM_VSCROLL, 0, 0);
    }
    break;
#endif
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      hit = scroll_hittest(s, c, x, y);
      s->pthit = iPOINT(x, y);
      if (hit != s->hit) {
        int i = hit >> 8;
        s->hit = hit;
        if (SBHT_THUMB == ((s->hit & 0xff) - 1)) {
          s->hit_pos = s->si[i].nPos;
        }
        else {
          int page = (int)s->si[i].nPage;
          int line = MAX(1, page / 8);
          int steps[5] = { -line, line, -page, page};
          int msgs[5] = { SB_LINEUP, SB_LINEDOWN, SB_PAGEUP, SB_PAGEDOWN};
          s->si[i].nTrackPos = s->si[i].nPos + steps[(s->hit & 0xff) - 1];
          s->si[i].nTrackPos = s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - page);
          //s->si[i].nTrackPos = s->si[i].nPos + steps[(s->hit & 0xff) - 1];
          //BOUND(, 0, (1 << 16) - page);
          scroll_set_pos(s->si + i, s->si[i].nTrackPos);
          //sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, msgs[(s->hit & 0xff) - 1]), 0);
        }
        force_redraw(c);
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    //printf("WM_MOUSEMOVE\n");
    if (s->hit > 0) {
      int i = s->hit >> 8;
      if (SBHT_THUMB == ((s->hit & 0xff) - 1)) {
        int page = (int)s->si[i].nPage;
        int len = s->si[i].nMax - s->si[i].nMin + 1;
        if (i) {
          int ccc = RCH(&rc) - 2 * SBW1;
          if (ccc) {
            s->si[i].nTrackPos = s->hit_pos + ((y - s->pthit.y) * len + ccc / 2) / ccc;
          }
        }
        else {
          int ccc = RCW(&rc) - 2 * SBW1;
          if (ccc) {
            s->si[i].nTrackPos = s->hit_pos + ((x - s->pthit.x) * len + ccc / 2) / ccc;
          }
        }
        s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - page);
        scroll_set_pos(s->si + i, s->si[i].nTrackPos);
        //sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, SB_THUMBTRACK), s->si[i].nTrackPos);
        force_redraw(c);
      }
    }
    else if (iPtInRect(&rc, x, y)) {
      hit = scroll_hittest(s, c, x, y);
      if (s->hot != hit) {
        s->hot = hit;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      IRECT prc[14] = {0};
      scroll_calc_rect(s, c, prc);
    }
    if (c->redraw && RCH(&rc) > 0 && RCW(&rc) > 0) {
      img_t* im = (img_t*)wParam;
      IRECT prc[14] = {0};
      //scroll_draw(s, c, im, prc);
    }
    break;
  }
  return 0;
}

