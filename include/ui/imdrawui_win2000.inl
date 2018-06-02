int imdrawui_3dbox(int h, int w, uchar* img, int step, int cn, IRECT rc)
{
  enum {NNN = 2};
  COLOR clrLT[NNN], clrRB[NNN];
  clrLT[0] = _GetSysColor(CLR_3DHILIGHT);
  clrLT[1] = _GetSysColor(CLR_3DFACE);
  clrRB[0] = _GetSysColor(CLR_3DDKSHADOW);
  clrRB[1] = _GetSysColor(CLR_3DSHADOW);
  imdraw_border(h, w, img, step, cn, rc, _GetSysColor(CLR_3DFACE), NNN, clrLT, NNN, clrLT, NNN, clrRB, NNN, clrRB);
  return 0;
}
int imdrawui_win2000_button(img_t* im, xbutton_t* btn)
{
  COLOR clrLT[3], clrRB[3];
  int nom[] = {CLR_3DHILIGHT, CLR_3DFACE, CLR_3DDKSHADOW, CLR_3DSHADOW};
  int hit[] = {CLR_INFOTEXT, CLR_3DDKSHADOW, CLR_INFOTEXT, CLR_3DDKSHADOW};
  //int foc[]={CLR_INFOTEXT, CLR_3DHILIGHT, CLR_3DFACE, CLR_INFOTEXT, CLR_3DDKSHADOW, CLR_3DSHADOW};
  int* pp;
  int np = 2;
  pp = (btn->hit) ? hit : nom;
  clrLT[0] = _GetSysColor(pp[0]);
  clrLT[1] = _GetSysColor(pp[1]);
  clrRB[0] = _GetSysColor(pp[2]);
  clrRB[1] = _GetSysColor(pp[3]);
  imdrawui_3dbox(IMARG(im), iRECT(0, 0, im->w, im->h));
  imdraw_border(IMARG(im), iRECT(0, 0, im->w, im->h), _GetSysColor(CLR_3DFACE), np, clrLT, np, clrLT, np, clrRB, np, clrRB);
  return 0;
}
#if 0
int imdrawui_win2000(const void* drawer, int type, const void* ctrl, const img_t* im)
{
  switch (type) {
  case dm_button:
    if (1) {
    }
    break;
  }
  return 0;
}
int imdrawui_win2000(const uictrl* ctrl, const event* e)
{
  const RECT* prc = &ctrl->rc;
  switch (type) {
  case dm_button:
    if (ctrl) {
      const button_ctrl* s = (const button_ctrl*)ctrl;
      RECT rc1 = {0};
      COLORREF _clr;
      RECT rc = *prc;
      BOOL m_bFocus = UIIsFocus(s);
      BOOL m_bPushed = s->hit;
      BOOL m_bChecked = 0;
      _clr = _RGB(0, 0, 0);
      if (0) {
        DrawFramed3dBox(hDC, rc, s->hit);
      }
      else {
        //DrawRect(hDC, rc, res->clrBarFace, res->clrBarDkShadow);
        //DrawRect(hDC, rc, res->m_clrMenuItemBorder, res->clrBarDkShadow);
        if (s->hit) {
          DrawRect(hDC, rc, res->m_clrHighlightDn, res->clrBarDkShadow);
        }
        else if (s->hot) {
          DrawRect(hDC, rc, res->m_clrHighlight, res->m_clrMenuItemBorder);
        }
        else {
          DrawRect(hDC, rc, res->clrBarFace, res->clrBarDkShadow);
        }
      }
      if (s->hit) {
        rc.left++, rc.top++;
      }
      draw_textR(hDC, s->m_text->s, s->m_text->l, &rc, DT_CENTER | DT_VCENTER, _clr);
      if (m_bFocus) {
        RectIn(*prc, 2, &rc);
        TDrawFocusRect(hDC, rc);
      }
    }
    break;
  case dm_edit:
    if (ctrl) {
      const edit_ctrl* s = (const edit_ctrl*)ctrl;
    }
    break;
  case dm_slider:
    if (ctrl) {
      const slider_ctrl* s = (const slider_ctrl*)ctrl;
      RECT rect = *prc;
      int b = MIN(RCH(prc), RCW(prc)) / 2 - 1;
      BOOL bVert = RCH(prc) > RCW(prc);
      RCOFFSET(&rect, b, b, -b, -b);
      if (0) {
        DrawRect(hDC, rect, res->clrBtnFace, res->clrBtnShadow);
      }
      else {
        Draw3DRect(hDC, rect, res->clrBtnFace, res->clrBtnShadow, res->clrBtnHilite);
      }
      {
        const slider_ctrl* s = (const slider_ctrl*)ctrl;
        RECT rect = *prc;
        BOOL bIsHighlighted = 0, bIsPressed = s->hit, bIsDisabled = 0;
        COLORREF clrLine = bIsDisabled ? res->clrBtnShadow : res->clrBtnDkShadow;
        COLORREF clrFill = !bIsDisabled && (bIsHighlighted || bIsPressed) ?
            res->clrBtnHilite : res->clrBtnFace;
        BOOL bVert = RCH(prc) > RCW(prc);
        double value = (s->m_value - s->m_min) / (s->m_max - s->m_min);
        if (bVert) {
          int b = RCW(prc);
          rect.top = (int)((RCH(prc) - b) * value) + prc->top + b / 4;
          rect.bottom = rect.top + b / 2;
        }
        else {
          int b = RCH(prc);
          rect.left = (int)((RCW(prc) - b) * value) + prc->left + b / 4;
          rect.right = rect.left + b / 2;
        }
        //DrawRect(hDC, rect, clrFill, clrLine);
        Draw3DRect(hDC, rect, clrFill, res->clrBtnHilite, clrLine);
      }
      if (s->m_label) {
        char buf[256];
        _snprintf(buf, 256, s->m_label->s, s->m_value);
        draw_textR(hDC, buf, strlen(buf), prc, DT_VCENTER | DT_SINGLELINE, res->clrBtnText);
      }
      //DrawSeparator();
    }
    break;
  case dm_rbox:
    if (ctrl) {
      const rbox_ctrl* s = (const rbox_ctrl*)ctrl;
      int i, m_text_height = FontGetTextSize("H", 1).h;
      COLORREF m_text_color = _GetSysColor(COLOR_BTNTEXT);
      int dy = m_text_height * 2;
      RECT rc0, rc1;
      RECT rc = *prc;
      BOOL bEnabled = 1;
      //s->m_dy = dy;
      draw_border(hDC, rc, BDS_ETCHED);
      for (i = 0; i < s->m_items->n; ++i) {
        if (RectCutTop(&rc, dy, 0, &rc0)) {
          str_t str = s->m_items->v[i];
          RectCutLeft(&rc0, dy, 0, &rc1);
          RectAlign(rc1, m_text_height, m_text_height, DT_VCENTER | DT_CENTER, &rc1);
          if (0) {
            //draw_frame_ctrl(hDC, &rc1, DFC_BUTTON, DFCS_BUTTONRADIO | (s->m_cur_item == i ? DFCS_CHECKED : 0));
          }
          else {
            RECT rect = rc1;
            BOOL bHighlighted = 0;
            BOOL bOn = s->m_cur_item == i;
            //DeflateRect(&rect, 1, 1);
            TDrawEllipse(hDC, rect,
                bEnabled ? globalData.clrBtnHilite : globalData.clrBtnFace,
                bHighlighted && bEnabled ? globalData.clrBtnDkShadow : globalData.clrBtnShadow);
            if (bOn) {
              DeflateRect(&rect, RCW(&rect) / 3, RCW(&rect) / 3);
              TDrawEllipse(hDC, rect, bHighlighted && bEnabled ? globalData.clrBtnDkShadow : globalData.clrBtnShadow, 0);
            }
          }
          draw_textR(hDC, str.s, str.l, &rc0, DT_VCENTER, m_text_color);
        }
        else {
          break;
        }
      }
    }
    break;
  case dm_cbox:
    if (ctrl) {
      const cbox_ctrl* s = (const cbox_ctrl*)ctrl;
      int m_bIsDrawOnGlass = 0;
      int m_text_height = FontGetTextSize("H", 1).h + 4;
      BOOL bEnabled = 1;
      RECT rc = *prc, rcCheck;
      RECT rect = *prc;
      int bHighlighted = s->hot;
      RectCutLeft(&rc, RCH(prc), 2, &rcCheck);
      RectAlign(rcCheck, m_text_height, m_text_height, DT_CENTER | DT_VCENTER, &rect);
      if (m_bIsDrawOnGlass) {
        DeflateRect(&rect, 1, 1);
        DrawRect(hDC, rect,
            bEnabled ? res->clrWindow : res->clrBarFace,
            res->clrBarShadow);
        if (1 == s->m_status) {
          DrawMenuImages(hDC, IdCheck, rect, ImageBlack);
        }
      }
      else {
        if (bHighlighted) {
          TDrawFocusRect(hDC, rect);
        }
#define TFillSolidRect draw_fillrectR
        DeflateRect(&rect, 1, 1);
        TFillSolidRect(hDC, &rect, bEnabled ? res->clrWindow : res->clrBarFace);
        Draw3dRect(hDC, &rect, res->clrBarDkShadow, res->clrBarHilite);
        DeflateRect(&rect, 1, 1);
        Draw3dRect(hDC, &rect, res->clrBarShadow, res->clrBarLight);
        if (1 == s->m_status) {
          DrawMenuImages(hDC, IdCheck, rect, ImageBlack);
        }
        else if (2 == s->m_status) {
          uchar mapA[2] = {255, 0};
          uchar HatchBits [8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };
          DeflateRect(&rect, 1, 1);
          RectAlign(rect, 8, 8, DT_CENTER | DT_VCENTER, &rect);
          draw_bit(hDC, rect.left, rect.top, 8, 8, HatchBits, 1, 1, _RGB(0, 0, 0), mapA, 0);
        }
        draw_textR(hDC, s->m_label->s, s->m_label->l, &rc, DT_VCENTER, _RGB(0, 0, 0));
      }
    }
    break;
  }
  return 0;
}
#endif

