typedef struct gridctrl_t {
  uictrl base;
} gridctrl_t;

int drawaa_gridctrl(const gridctrl_t* s, const event* e)
{
  UIHDC(e);
  const RECT* prc;
  RECT rect, rcRow, rcCol, rc1, rc, rcSelect;
  int i, j;
  DWORD _clr;
  char buf[256];
  COLORREF crBakH = _GetSysColor(COLOR_3DLIGHT);
  COLORREF crText = _GetSysColor(COLOR_WINDOWTEXT);
  COLORREF crBakW = _GetSysColor(COLOR_WINDOW);
  COLORREF crBorder = _RGBA(0, 0, 0, 80);

  draw_scroll(s->sc, e);
  prc = &s->sc->rcsub[SCHT_CLIENT];

  rc = *prc;
  draw_rectR(hDC, &rc, crBorder);
  RCOFFSET(&rc, 1, 1, -1, -1);

  gridctrl_getclip(s, prc, &rc1);
  rect = rc1;
  HDDC_BEGIN(hDC, &rc1);
  _clr = _RGB(255, 255, 255);
  draw_fillrectR(hDC, &rect, _clr);

  rcSelect = RCNORM4(s->hit.x, s->hit.y, s->end.x, s->end.y);
  RCOFFSET(&rcSelect, 0, 0, 1, 1);

  for (i = 0; i < s->rows; ++i) {
    if (i > 0 && i < s->lt.y) {
      continue;
    }

    RectCutTop(&rect, s->rowh[i], 1, &rcRow);

    if (rcRow.top > rc1.bottom) {
      break;
    }

    for (j = 0; j < s->cols; ++j) {
      int isselect = 0;
      int inx = rcSelect.left <= j && j < rcSelect.right;
      int iny = rcSelect.top <= i && i < rcSelect.bottom;

      if (j > 0 && j < s->lt.x) {
        continue;
      }

      RectCutLeft(&rcRow, s->colw[j], 1, &rcCol);

      if (rcCol.left > rc1.right) {
        break;
      }

      buf[0] = 0;

      if (0 == i && j > 0) {
        //int2abc(j - 1, buf);
        _snprintf(buf, 256, "%d", j);
        _clr = crBakH;
        isselect = inx || (0 == s->hit.x);
      }
      else if (0 == j && i > 0) {
        _snprintf(buf, 256, "%d", i);
        _clr = crBakH;
        isselect = iny || (0 == s->hit.y);
      }
      else if (i > 0 && j > 0) {
        UISendEvent(e, s, e->toer, EVENT_DATA, (UINT)buf, MAKELONG(j, i));
        _clr = crBakW;
        isselect = (0 == s->hit.x && 0 == s->hit.y) || (0 == s->hit.x && iny)
            || (0 == s->hit.y && inx) || (inx && iny);
      }
      else {
        _clr = crBakH;
      }

      draw_fillrectR(hDC, &rcCol, _clr);
      draw_textR(hDC, buf, -1, &rcCol, DT_CENTER | DT_VCENTER, crText);

      if (isselect) {
        _clr = _RGBA(0, 0, 255, 50);
        draw_fillrectR(hDC, &rcCol, _clr);
      }
    }
  }

  rect = rc1;

  for (i = 0; i < s->rows; ++i) {
    if (i > 0 && i < s->lt.y) {
      continue;
    }

    RectCutTop(&rect, s->rowh[i], 0, &rcRow);
    RectCutTop(&rect, 1, 0, &rcRow);
    draw_fillrectR(hDC, &rcRow, crBorder);
  }

  rect = rc1;

  for (i = 0; i < s->cols; ++i) {
    if (i > 0 && i < s->lt.x) {
      continue;
    }

    RectCutLeft(&rect, s->colw[i], 0, &rcCol);
    RectCutLeft(&rect, 1, 0, &rcCol);
    draw_fillrectR(hDC, &rcCol, crBorder);
  }

  HDDC_END(hDC);
  return 0;
}

int gridctrl_setscroll(gridctrl_t* s)
{
  RECT rca, rcv = s->sc->rect;
  gridctrl_getallrect(s, &rca);
  RectOffset(rcv, s->colw[0] + 1, s->rowh[0] + 1, 0, 0, &rcv);
  scroll_getrect(s->sc, rca, rcv);
  //s->org = RCLT(&s->sc->rcsub[SCHT_RCA]);
  return 0;
}

int gridctrl_event(gridctrl_t* s, const event* e)
{
  UIEVT(e);
  RECT r = *prc;

  if (scroll_event(s->sc, e, s)) {
    return 1;
  }

  switch (e->msg) {
  case WM_SIZE:
    s->sc->rect = *prc;
    gridctrl_setscroll(s);
    break;

  case WM_MOUSEWHEEL:
    if (PtInRect(prc, e->pt)) {
      s->lt.y += GET_WHEEL_DELTA_WPARAM(e->wp) > 0 ? -3 : 3;
      s->lt.y = BOUND(s->lt.y, 0, s->rows - 1);
      gridctrl_setscroll(s);
      force_redraw(e);
    }

    break;

  case EVENT_SCROLL_SETPOS:
    if (1) {
      if (e->wp < 8 && e->wp >= 0) {
        POINT wwp[8] = {0};
        int lx = 1, ly = 1, px = 20, py = 20;
        wwp[SCHT_LINEUP] = iPOINT(0, ly);
        wwp[SCHT_LINELEFT] = iPOINT(lx, 0);
        wwp[SCHT_LINEDOWN] = iPOINT(0, -ly);
        wwp[SCHT_LINERIGHT] = iPOINT(-lx, 0);

        wwp[SCHT_PAGEUP] = iPOINT(0, py);
        wwp[SCHT_PAGELEFT] = iPOINT(px, 0);
        wwp[SCHT_PAGEDOWN] = iPOINT(0, -py);
        wwp[SCHT_PAGERIGHT] = iPOINT(-px, 0);
        PTADD(s->lt, s->lt, wwp[e->wp]);
      }
      else {
        int i;
        POINT pt = RCLT(&s->sc->rcsub[SCHT_RCA]);
        s->lt = iPOINT(1, 1);

        for (i = 1; i < s->rows && pt.y < 0; ++i) {
          pt.y += s->rowh[i] + 1;
          s->lt.y++;
        }

        for (i = 1; i < s->cols && pt.x < 0; ++i) {
          pt.x += s->colw[i] + 1;
          s->lt.x++;
        }
      }

      if (e->lp) {
        gridctrl_setscroll(s);
      }

      force_redraw(e);
    }

    break;

  case WM_LBUTTONDOWN:

    break;

  case WM_LBUTTONUP:
    break;

  case WM_MOUSEMOVE:
    break;

  case EVENT_EXIT:
    if (e->sender == e->ui->subctrl) {
      SAFEFREE(s->rowh);
      SAFEFREE(s->colw);
    }

    break;
  }

  return FALSE;
}
