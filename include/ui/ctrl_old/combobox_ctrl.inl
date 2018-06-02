typedef struct combobox_ctrl {
  uictrl base;
  listbox_ctrl m_listbox[1];
} combobox_ctrl;
int draw_combobox_ctrl(const combobox_ctrl* s, const event* e)
{
  UIHDC(e);
  RECT rectArrow;
  RECT rc = s->base.rc;
  RECT rect = s->base.rc;
  const listbox_ctrl* li = s->m_listbox;
  str_t curstr = {0};

  if (li->cur >= 0 && li->cur < li->items->n) {
    curstr = li->items->v[li->cur];
  }

  HDDC_BEGIN(hDC, &rc);
  //DrawSunkenFramed3dWindow(hDC, rect);
  DrawRect(hDC, rect, res->clrBtnWindow, res->clrBtnText);
  InflateRect(&rect, -2, -2);
  RectCutLeft(&rect, -RCH(&rect), 0, &rectArrow);
  DrawFramed3dBox(hDC, rectArrow, 0);
  //draw_frame_ctrl(hDC, &rectArrow, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
  DrawMenuImages(hDC, IdArowDownLarge, rectArrow, ImageBlack);

  if (curstr.s) {
    draw_textR(hDC, curstr.s, curstr.l, &rect, DT_VCENTER, res->clrBtnText);
  }

  HDDC_END(hDC);
  return 0;
}
int combobox_ctrl_event(combobox_ctrl* s, const event* e)
{
  UIEVT(e);
  listbox_ctrl* li = s->m_listbox;

  switch (e->msg) {
  case WM_SIZE:
    if (1) {
      MINMAXINFO mmi[1] = {0};
      SIZE sz;
      RECT rcui;
      UIGetWorkArea(ui, &rcui);
      UISendEvent(e, s, s->m_listbox, EVENT_GETMINMAXINFO, (UINT)mmi, 0);
      sz.w = MAX(mmi->ptMaxTrackSize.x, RCW(prc));
      sz.h = MIN(mmi->ptMaxTrackSize.y, rcui.bottom - prc->bottom);
      UISendRect(e, s, s->m_listbox, iRECT3(RCLB(prc), sz));
    }

    break;

  case EVENT_INIT:
    UISETCALL(s, draw_combobox_ctrl, combobox_ctrl_event);
    listbox_ctrl_set(s->m_listbox);
    break;

  case EVENT_EXIT:
  case EVENT_SETATTRIBUTE:
    UICALL_EVENT(&s->m_listbox->base, e);
    break;

  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (PtInRect(prc, e->pt)) {
      UISetFocus(s);

      if (NULL == ui->popup) {
        ui->popup = &s->m_listbox->base;
      }
      else {
        ui->popup = 0;
      }
    }
    else {
      ui->popup = 0;
    }

    force_redraw(e);
    break;
  }

  return FALSE;
}
int combobox_ctrl_set(combobox_ctrl* s, int ci, int ni, const char** items)
{
  UISETCALL(s, draw_combobox_ctrl, combobox_ctrl_event);
  listbox_ctrl_set(s->m_listbox);
  return 0;
}

