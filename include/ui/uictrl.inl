static LRESULT null_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  null_t* s = (null_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 20;
    mminfo->ptMinTrackSize.y = 20;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (s->hit) {
      if (iPtInRect(&rc, x, y)) {
        //xSendCmd(XCMD_CLICK);
        force_redraw(c);
      }
      s->hit = 0;
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->hit = 1;
      force_redraw(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    if (s && iPtInRect(&rc, x, y)) {
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
static LRESULT pushbutton_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  pushbutton_t* s = (pushbutton_t*)c->x;
  switch (uMsg) {
  case WM_CTRLINIT:
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo;
    mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
#if 0
      POINT pt = MyGetCursorPos(hwnd);
      int xx = s->xx;
      if (iPtInRect(&c->rc, pt.x, pt.y)) {
        s->xx = MIN(s->xx + 10, 200);
      }
      else {
        s->xx = MAX(s->xx - 10, 0);
      }
      if (xx != s->xx) {
        //InvalidateRect(hwnd, 0, 0);
      }
#endif
    }
    break;
  case WM_MOUSEMOVE:
    //printf("WM_MOUSEMOVE\n");
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      imdrawaa_circle(im, &rcclip, (rc.l + rc.r) / 2., (rc.t + rc.b) / 2., (rc.r - rc.l) / 2., _RGB(255, 0, 0), _RGB(0, 0, 255), 2);
    }
    break;
  }
  return 0;
}
static LRESULT lable_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  lable_t* s = (lable_t*)c->x;
  //IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = font_text_size(c->fo, c->text, -1, 0);
    mminfo->ptMinTrackSize.x = sz.w + 2;
    mminfo->ptMinTrackSize.y = sz.h + 2;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      imdraw_text(im, &rcclip, rc, c->text, -1, c->fo, _RGB(0, 0, 0), 0, DT_VCENTER | DT_LEFT);
    }
    break;
  }
  return 0;
}
static LRESULT button_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  button_t* s = (button_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = font_text_size(c->fo, c->text, -1, 0);
    mminfo->ptMinTrackSize.x = sz.w + 2;
    mminfo->ptMinTrackSize.y = sz.h + 2;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (s->hit) {
      if (iPtInRect(&rc, x, y)) {
        //xSendCmd(XCMD_CLICK);
        sendmsg(c, c->listener, WM_COMMAND, 0, (WPARAM)c);
        force_redraw(c);
      }
      s->hit = 0;
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->hit = 1;
      force_redraw(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
      int hot = iPtInRect(&rc, x, y);
      if (s->hot != hot) {
        s->hot = hot;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {}
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rc);
      iRectOff0(&rcclip);
      //switch (c->sys->style)
      //case UIS_WIN2K:
      if (0) {
        imdraw_border_syscolor(im, &rcclip, rc, s->hit ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
        imdraw_text(im, &rcclip, rc, c->text, -1, c->fo, _RGB(0, 0, 0), 0, DT_VCENTER | DT_CENTER);
      }
      if (c) {
        COLOR crBack = _RGB(255, 255, 255);
        //边框颜色
        COLOR m_border_color = _RGB(0, 0, 0);
        COLOR m_text_color = _RGB(0, 0, 0);
        COLOR clr2 = s->hit ? _RGB(100, 100, 100) : s->hot ? _RGB(140, 140, 140) : _RGB(200, 200, 200);
        IRECT rc1 = rc;
        //画按钮的外边框，它是一个半径为5的圆角矩形
        //draw_roundrectR( im, &rect, 5, 5, s->m_border_color );
        //iRectOff2(&rc1, -5, -5);
        //draw_roundrectR(im, &rcclip1, 5, 5, m_border_color, 0);
        imdraw_rect(im, &rcclip, rc1, 0, m_border_color, 1, BDM_ALL);
        //draw_roundrect_aaR( im, &rect, 5, 5, s->m_border_color, 0 );
        rc1 = rc;
        iRectIn(&rc1, 1);
        //InflateRect(&rc1, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));
        imdraw_rect_gradient(im, &rcclip, rc1, crBack, clr2, 0);
        imdraw_text(im, &rcclip, rc1, c->text, -1, c->fo, m_text_color, 0, DT_VCENTER | DT_CENTER);
      }
    }
    break;
  }
  return 0;
}
CC_INLINE int button_set(button_t* s, ctrl_t* li, const char* text)
{
  ctrl_set(button_proc, s, s->c, li, WT_CHILD, text);
  return 0;
}
CC_INLINE int groupbox_calcrect(IRECT rc, const char* text, int textlen, const font_t* font, IRECT* outrc)
{
  int len = textlen < 0 ? strlen(text) : textlen;
  int d = font_text_size(font, text, len, 0).h;
  *outrc = rc;
  iRectOffset(outrc, 4, d, -4, -4);
  return 0;
}
static int groupbox_draw(img_t* im, const IRECT* pclip, IRECT rc, const char* text, int textlen, int bds, COLOR crBk, const font_t* font)
{
  COLOR clr = crBk;
  IRECT rcclip = iRectInter(rc, iRectClip(pclip, 0, 0, im->w, im->h));
  textlen = textlen < 0 ? strlen(text) : textlen;
  //DrawSunken3dFrame(hDC, rect3d);
  if (crBk) {
    //imdraw_fillrect(im, &rcclip, rc, crBk);
  }
  if (1) {
    IRECT rc1 = rc, rc2;
    ISIZE sz = font_text_size(font, text, textlen, 0);
    int d = sz.h / 2;
    rc1.t += d;
    iRectCutL(&rc1, d + 2, 0, &rc2);
    imdraw_border_syscolor_impl(im, &rcclip, rc2, bds, BDM_L | BDM_T | BDM_B, 0);
    iRectCutL(&rc1, sz.w + 4, 0, &rc2);
    rc2.t -= d;
    imdraw_border_syscolor_impl(im, &rcclip, rc2, bds, BDM_B, 0);
    imdraw_border_syscolor_impl(im, &rcclip, rc1, bds, BDM_R | BDM_T | BDM_B, 0);
    if (textlen > 0) {
      imdraw_text(im, &rcclip, rc2, text, textlen, font, Color_Black, 0, DT_CENTER);
    }
  }
  return 0;
}
static LRESULT groupbox_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  IRECT rc = c->rc;
  groupbox_t* s = (groupbox_t*)c->x;
  LRESULT ret = 0;
  switch (uMsg) {
  case WM_CTRLINIT:
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = font_text_size(c->fo, c->text, -1, 0);
    mminfo->ptMinTrackSize.x = MAX(sz.w, mminfo->ptMinTrackSize.x) + 8;
    mminfo->ptMinTrackSize.y += sz.h + 4;
    //printf("WM_MOUSEMOVE\n");
    return 0;
  }
  break;
  case WM_PAINT:
    if (c->recalcsize) {
      //int fwSizeType = wParam; // resizing flag
      groupbox_calcrect(rc, c->text, -1, c->fo, &rc);
      if (c->sub) {
        ctrl_setrect(c->sub, rc);
      }
    }
    if (c->redraw) {
      int bds = BDS_FRAME;
      COLOR clr = Color_White;
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      if (1) {
        bds = BDS_ETCHED;
        clr = _GetSysColor(CLR_3DFACE);
      }
      else {
        bds = BDS_SIMPLE;
        clr = Color_White;
      }
      groupbox_draw(im, &rcclip, rc, c->text, -1, bds, clr, c->fo);
    }
    break;
  }
  return 0;
}
CC_INLINE int groupbox_set(groupbox_t* s, ctrl_t* li, const char* text, ctrl_t* sub)
{
  ctrl_set(groupbox_proc, s, s->c, li, WT_CHILD, text);
  ctrl_addsub_head(s->c, sub);
  return 0;
}
CC_INLINE int xcurve_add(curve_t* s, int x, int y)
{
  int i;
  for (i = s->npt - 1; i >= 0 && x < s->points[i].x; --i) {
    s->points[i + 1] = s->points[i];
  }
  s->npt++;
  s->points[i + 1] = iPOINT(x, y);
  s->hot = i + 1;
  return 0;
}
CC_INLINE int xcurve_remove(curve_t* s, int i)
{
  MEMMOVE(s->points + i, s->points + 1, s->npt - i - 1);
  s->npt--;
  return 0;
}
CC_INLINE int xcurve_hittest(curve_t* s, int x, int y)
{
  int i;
  for (i = 0; i < s->npt; ++i) {
    IPOINT p = s->points[i];
    int d = 3;
    IRECT rc = iRECT(p.x - d, p.y - d, p.x + d, p.y + d);
    if (iPtInRect(&rc, x, y)) {
      return i;
    }
  }
  return -1;
}
CC_INLINE LRESULT curve_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  curve_t* s = (curve_t*)c->x;
  IRECT rc = c->rc;
  int hit, x = LOWORD(lParam) - rc.l, y = HIWORD(lParam) - rc.t;
  switch (uMsg) {
  case WM_CTRLINIT:
    s->hit = -1;
    s->hot = -1;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo;
    mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 20;
    mminfo->ptMinTrackSize.y = 20;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      hit = xcurve_hittest(s, x, y);
      if (hit != s->hit) {
        s->hit = hit;
        s->downPoint = iPOINT(x, y);
        force_redraw(c);
      }
      if (hit >= 0) {
        s->hot = hit;
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_LBUTTONDBLCLK:
    if (iPtInRect(&rc, x, y)) {
      hit = xcurve_hittest(s, x, y);
      if (hit < 0) {
        xcurve_add(s, x, y);
      }
      else {
        xcurve_remove(s, hit);
      }
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
      s->hit = -1;
    }
    break;
  case WM_MOUSEMOVE:
    if (c->sys->ldown) {
      if (iPtInRect(&rc, x, y)) {
        if (s->hit >= 0 && s->hit < s->npt) {
          IPOINT p = s->points[s->hit], p1 = s->downPoint;
          s->points[s->hit] = iPOINT(p.x + x - p1.x, p.y + y - p1.y);
          s->downPoint = iPOINT(x, y);
        }
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i = 0, j = 0;
      int npt = s->npt + 2;
      DPOINT* pt = MALLOC(DPOINT, npt);
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //COLOR BackColor = Color_White;
      //imdraw_fill(im, Gray);
      pt[i++] = dPOINT(0, 0);
      for (j = 0; j < s->npt; ++j) {
        pt[i++] = dPOINT(s->points[j].x, s->points[j].y);
      }
      pt[i++] = dPOINT(im->w, im->h);
      imdrawaa_spline(im, &rcclip, pt, npt, 0, 0, Color_Black, 1);
      //if (s->mouseDown) {
      //imdrawaa_line(im, 0, p.Y, clientRect.Width, p.Y, Gray);
      //g.DrawLine(Pens.Gray, p.X, 0, p.X, clientRect.Height);
      //}
      for (i = 0; i < s->npt; ++i) {
        IPOINT p = s->points[i];
        int d = 3;
        COLOR clr = Color_Black;
        IRECT rc = iRECT(p.x - d, p.y - d, p.x + d, p.y + d);
        if (i == s->hot) {
          //imdrawaa_rect(im, p.x-d, p.y-d, p.x+d, p.y+d, Color_Black, 0, 0);
          imdraw_fillrect(im, &rcclip, rc, Color_Black);
        }
        else {
          imdraw_border_n(im, &rcclip, rc, 0, 1, &clr, &clr, &clr, &clr);
          //imdrawaa_rect(im, p.x-d, p.y-d, p.x+d, p.y+d, 0, Color_Black, 1);
        }
      }
      FREE(pt);
    }
    break;
  }
  return 0;
}
CC_INLINE int slider_hitrect(slider_t* s, const IRECT rc0)
{
  IRECT rc = rc0;
  int h = RCH(&rc), w = RCW(&rc);
  int b = MIN(h, w) / 2;
  s->rcsub[1] = s->rcsub[0] = rc;
  RCOFFSET(&rc, b, b / 2, -b, -b / 2);
  s->rad = MIN(h, w) * 0.5;
  s->pt.x = rc.l + RCW(&rc) * (s->m_value - s->m_min) / (s->m_max - s->m_min);
  s->pt.y = rc0.t + h * .5;
  s->rcsub[0] = rc;
  s->rcsub[1].l = (int)(s->pt.x - s->rad);
  s->rcsub[1].r = s->rcsub[1].l + w;
  return 0;
}
CC_INLINE LRESULT slider_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  slider_t* s = (slider_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  IPOINT pt = iPOINT(x, y);
  switch (uMsg) {
  case WM_CTRLINIT:
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo;
    ISIZE sz = font_text_size(c->fo, s->text, -1, 0);
    mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = sz.w + sz.h;
    mminfo->ptMinTrackSize.y = sz.h + sz.h;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      if (calc_distance(pt.x, pt.y, s->pt.x, s->pt.y) < RCH(&rc) * 0.5) {
        PTSUB(s->m_pdx, s->pt, pt);
        s->hit = 1;
        force_redraw(c);
      }
      else {
        if (iPtInRect(s->rcsub, x, y)) {
          s->m_value = s->m_min + (s->m_max - s->m_min) * (pt.x - s->rcsub->l) / RCW(s->rcsub);
          s->m_value = BOUND(s->m_value, s->m_min, s->m_max);
          slider_hitrect(s, rc);
        }
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
      s->hit = 0;
      //slider_hitrect(s, RCH(&rc), RCW(&rc));
      force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
      if (s->hit) {
        PTADD(s->pt, s->m_pdx, pt);
        s->m_value = s->m_min + (s->pt.x - s->rcsub->l) * (s->m_max - s->m_min) / RCW(s->rcsub);
        s->m_value = BOUND(s->m_value, s->m_min, s->m_max);
        sendmsg(c, c->listener, WM_COMMAND, XCMD_CHANGE, 0);
        slider_hitrect(s, rc);
        force_redraw(c);
      }
      else {
        BOOL bIn = iPtInRect(&rc, x, y);
        if (s->hot != bIn) {
          s->hot = bIn;
        }
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      slider_hitrect(s, rc);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      if (1) {
        IRECT rect = rc;
        int b = MIN(RCH(&rc), RCW(&rc)) / 2 - 1;
        BOOL bVert = RCH(&rc) > RCW(&rc);
        RCOFFSET(&rect, b, b, -b, -b);
        imdraw_border_syscolor(im, &rcclip, rc, BDS_LOW, 0);
        imdraw_border_syscolor(im, &rcclip, rect, BDS_LOW, BDM_ALL);
        {
          IRECT rect = rc;
          BOOL bIsHighlighted = 0, bIsPressed = s->hit, bIsDisabled = 0;
          //COLOR clrLine = bIsDisabled ? res->clrBtnShadow : res->clrBtnDkShadow;
          //COLOR clrFill = !bIsDisabled && (bIsHighlighted || bIsPressed) ? res->clrBtnHilite : res->clrBtnFace;
          BOOL bVert = RCH(&rc) > RCW(&rc);
          double value = (s->m_value - s->m_min) / (s->m_max - s->m_min);
          if (bVert) {
            int b = RCW(&rc);
            rect.t = (int)((RCH(&rc) - b) * value) + rc.t + b / 4;
            rect.b = rect.t + b / 2;
          }
          else {
            int b = RCH(&rc);
            rect.l = (int)((RCW(&rc) - b) * value) + rc.l + b / 4;
            rect.r = rect.l + b / 2;
          }
          imdraw_border_syscolor(im, &rcclip, rect, BDS_HIGH, BDM_ALL);
        }
        if (s->text) {
          char buf[256];
          _snprintf(buf, 256, s->text, s->m_value);
          imdraw_text(im, 0, rc, buf, -1, c->fo, Color_Black, 0, DT_VCENTER);
        }
      }
      else {
        COLOR m_background_color = (_rgb(1.0, 0.9, 0.8));
        COLOR m_triangle_color = (_rgb(0.7, 0.6, 0.6));
        COLOR m_text_color = (_rgb(0.0, 0.0, 0.0));
        COLOR m_pointer_preview_color = (_rgba(0.6, 0.4, 0.4, 0.4));
        COLOR m_pointer_color[2] = {(_rgba(0.8, 0.0, 0.0, 0.6)), (_rgba(0.9, 0.0, 0.0, 0.8))};
        // Background
        imdraw_rect(im, &rcclip, rc, m_background_color, 0, 0, BDM_ALL);
        imdraw_rect(im, &rcclip, s->rcsub[0], m_triangle_color, 0, 0, BDM_ALL);
        imdrawaa_ellipse(im, &rcclip, s->pt.x, s->pt.y, s->rad, s->rad, m_pointer_color[1], 0, 0);
        if (s->m_num_steps) {
          int i;
          BOOL bb = RCH(&rc) > RCW(&rc);
          DPOINT pt[10];
          double dx = s->rad / 8;
          double dy = s->rad / 5;
          int y2 = rc.b;
          for (i = 0; i < s->m_num_steps + 1; i++) {
            double x = s->rcsub->l + RCW(s->rcsub) * i / s->m_num_steps;
            pt[0] = dPOINT(x, y2 - dy - dy);
            pt[1] = dPOINT(x - dx, y2 - dy);
            pt[2] = dPOINT(x + dx, y2 - dy);
            imdrawaa_poly(im, &rcclip, pt, 3, 1, m_text_color, 0, 0);
          }
        }
        if (s->text) {
          char buf[256];
          _snprintf(buf, 256, s->text, s->m_value);
          imdraw_text(im, 0, rc, buf, -1, c->fo, m_text_color, 0, DT_VCENTER);
        }
      }
    }
    break;
  }
  return 0;
}
CC_INLINE int slider_set(slider_t* s, ctrl_t* li, const char* text, double m_min, double m_max, double m_value)
{
  ctrl_set(slider_proc, s, s->c, li, WT_CHILD, text);
  s->text = text, s->m_min = m_min, s->m_max = m_max, s->m_value = m_value;
  return 0;
}
CC_INLINE LRESULT scale_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  double m_xs1[2];
  double m_xs2[2];
  scale_t* s = (scale_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  //IPOINT pt = iPOINT(x, y);
  enum {
    move_nothing = 0,
    move_value1,
    move_value2,
    move_slider
  };
  int xx[2] = {x, y};
  double xx1[2];
  double xx2[2];
  double m_x1 = rc.l, m_y1 = rc.t;
  double m_x2 = rc.r, m_y2 = rc.b;
  BOOL bb = fabs(m_x2 - m_x1) <= fabs(m_y2 - m_y1);
  int x1 = rc.l, y1 = rc.t, x2 = rc.r, y2 = rc.b;
  double m_border_extra = MIN(fabs(x2 - x1), fabs(y2 - y1)) / 2;
  double be = m_border_extra;
  m_xs1[0] = x1 + be / 1;
  m_xs2[0] = x2 - be / 1;
  m_xs1[1] = y1 + be / 2;
  m_xs2[1] = y2 - be / 2;
  xx1[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * s->m_value1;
  xx2[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * s->m_value2;
  xx1[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
  xx2[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
  switch (uMsg) {
  case WM_CTRLINIT:
    s->hit = 0;
    s->m_pdx[0] = (0.0);
    s->m_pdx[1] = (0.0);
    s->m_move_what = 0;
    s->m_value1 = (0.3);
    s->m_value2 = (0.7);
    s->m_min_d = (0.01);
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      double r = m_border_extra;
      xx1[!bb] = m_xs1[!bb];
      xx2[!bb] = m_xs2[!bb];
      if (calc_distance(x, y, xx1[0], xx1[1]) <= r) {
        //if(x < xp1 && calc_distance(x, y, xp1, yp) <= m_y2 - m_y1)
        s->m_pdx[0] = xx[0] - xx1[0];
        s->m_pdx[1] = xx[1] - xx1[1];
        s->m_move_what = move_value1;
        force_redraw(c);
      }
      else if (calc_distance(x, y, xx2[0], xx2[1]) <= r) {
        //if(x > xp2 && calc_distance(x, y, xp2, yp) <= m_y2 - m_y1)
        s->m_pdx[0] = xx[0] - xx2[0];
        s->m_pdx[1] = xx[1] - xx2[1];
        s->m_move_what = move_value2;
        force_redraw(c);
      }
      else if (xx[0] > xx1[0] && xx[0] < xx2[0] && xx[1] > xx1[1] && xx[1] < xx2[1]) {
        s->m_pdx[0] = xx[0] - xx1[0];
        s->m_pdx[1] = xx[1] - xx1[1];
        s->m_move_what = move_slider;
        force_redraw(c);
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_LBUTTONUP:
    s->m_move_what = move_nothing;
    force_redraw(c);
    break;
  case WM_MOUSEMOVE:
    if (s->m_move_what > 0) {
      if (iPtInRect(&rc, x, y)) {
        double dv;
        switch (s->m_move_what) {
        case move_value1:
          if (s) {
            s->m_value1 = (xx[bb] - s->m_pdx[bb] - m_xs1[bb]) / (m_xs2[bb] - m_xs1[bb]);
            if (s->m_value1 < 0.0) {
              s->m_value1 = 0.0;
            }
            if (s->m_value1 > s->m_value2 - s->m_min_d) {
              s->m_value1 = s->m_value2 - s->m_min_d;
            }
            force_redraw(c);
          }
          break;
        case move_value2:
          if (s) {
            s->m_value2 = (xx[bb] - s->m_pdx[bb] - m_xs1[bb]) / (m_xs2[bb] - m_xs1[bb]);
            if (s->m_value2 > 1.0) {
              s->m_value2 = 1.0;
            }
            if (s->m_value2 < s->m_value1 + s-> m_min_d) {
              s->m_value2 = s->m_value1 + s->m_min_d;
            }
            force_redraw(c);
          }
          break;
        case move_slider:
          if (s) {
            dv = s->m_value2 - s->m_value1;
            s->m_value1 = (xx[bb] - s->m_pdx[bb] - m_xs1[bb]) / (m_xs2[bb] - m_xs1[bb]);
            //printf("%f, %f, %f, %f\n", xx1[bb], xx[bb], s->m_pdx[bb],m_xs1[bb]);
            s->m_value2 = s->m_value1 + dv;
            if (s->m_value1 < 0.0) {
              dv = s->m_value2 - s->m_value1;
              s->m_value1 = 0.0;
              s->m_value2 = s->m_value1 + dv;
            }
            if (s->m_value2 > 1.0) {
              dv = s->m_value2 - s->m_value1;
              s->m_value2 = 1.0;
              s->m_value1 = s->m_value2 - dv;
            }
            force_redraw(c);
          }
          break;
        }
      }
    }
    break;
  case WM_PAINT:
    if (wParam) {
      img_t* im = (img_t*)wParam;
      double m_border_thickness = 1;
      double r, bw = m_border_thickness, be = m_border_extra;
      int m_descending = (FALSE);
      COLOR m_background_color = _rgb(1.0, 0.9, 0.8);
      COLOR m_border_color = _rgb(0.0, 0.0, 0.0);
      COLOR m_pointers_color = _rgba(0.8, 0.0, 0.0, 0.8);
      COLOR m_slider_color = _rgba(0.2, 0.1, 0.0, 0.6);
      COLOR m_text_color = _rgb(0.0, 0.0, 0.0);
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      m_x1 = rc.l, m_y1 = rc.t;
      m_x2 = rc.r, m_y2 = rc.b;
      bb = fabs(m_x2 - m_x1) <= fabs(m_y2 - m_y1);
      x1 = rc.l, y1 = rc.t, x2 = rc.r, y2 = rc.b;
      m_border_extra = MIN(fabs(x2 - x1), fabs(y2 - y1)) / 2;
      be = m_border_extra;
      m_xs1[0] = x1 + be / 1;
      m_xs2[0] = x2 - be / 1;
      m_xs1[1] = y1 + be / 2;
      m_xs2[1] = y2 - be / 2;
      xx1[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * s->m_value1;
      xx2[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * s->m_value2;
      xx1[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
      xx2[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
      {
        if (c->sys) {
          double xx1[2];
          double xx2[2];
          // Background
          imdrawaa_rect(im, &rcclip, x1, y1, x2, y2, m_background_color, 0, 0);
          //lens[0] = vcgen_rect(pt, x1, y1, x2, y2);
          imdrawaa_rect(im, &rcclip, m_xs1[0], m_xs1[1], m_xs2[0], m_xs2[1], 0, m_border_color, 1);
          r = m_border_extra;
          xx1[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * s->m_value1;
          xx2[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * s->m_value2;
          xx1[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
          xx2[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
          imdrawaa_ellipse(im, &rcclip, xx1[0], xx1[1], r, r, m_pointers_color, 0, 0);
          imdrawaa_ellipse(im, &rcclip, xx2[0], xx2[1], r, r, m_pointers_color, 0, 0);
          xx1[!bb] = m_xs1[!bb];
          xx2[!bb] = m_xs2[!bb];
          imdrawaa_rect(im, &rcclip, xx1[0], xx1[1], xx2[0], xx2[1], m_slider_color, 0, 0);
        }
      }
#if 1
      {
        char buf[256];
        _snprintf(buf, 256, "%.3f %.3f", s->m_value1, s->m_value2);
        imdraw_text(im, 0, rc, buf, -1, c->fo, m_text_color, 0, DT_VCENTER);
      }
#endif
    }
    break;
  }
  return FALSE;
}
CC_INLINE int scale_set(scale_t* s, ctrl_t* li, ctrl_t* fa)
{
  ctrl_set(scale_proc, s, s->c, li, WT_CHILD, 0);
  return 0;
}
CC_INLINE int layout_setsize_1d(int n, const char* p, int pl, int* ro, int gap, int cx)
{
  int i, cn = 0, cd = 0, cc = 0, ii = -1;
  cx -= gap * (n - 1);
  for (i = 0; i < n; ++i) {
    ro[i] = MAX(ro[i], 1);
    if (_lay_x != p[i * pl]) {
      cd += ro[i];
    }
    else {
      cn += ro[i];
    }
  }
  cx = MAX(cx - cd, 0);
  for (i = 0; i < n; ++i) {
    if (_lay_x == p[i * pl]) {
      ro[i] = cx * ro[i] / cn;
      ii = i;
      cc += ro[i];
    }
  }
  if (ii < 0) {
    ii = n - 1;
  }
  ro[ii] += cx - cc;
  return cd + (n - 1) * gap;
}
//static LRESULT layout_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam);
// pt 左上角坐标
CC_INLINE int layout_getrect(layout_t* s, IRECT rc1, int x, int y, IPOINT* phitbd)
{
  int i, j, k, ret = 0;
  IPOINT pt;
  IRECT rc = rc1, rc0;
  int al = s->cols + 2;
  const char* id0 = s->id;
  const char* id = 0;
  ctrl_t* ctrl = s->c->sub;
  if (s->text) {
    groupbox_calcrect(rc1, s->text, -1, s->c->fo, &rc);
  }
  pt = RCLT(&rc);
  layout_setsize_1d(s->cols, id0 + 1, 1, s->co, s->gap.w, RCW(&rc) - s->margin.w * 2);
  layout_setsize_1d(s->rows, id0 + al, al, s->ro, s->gap.h, RCH(&rc) - s->margin.h * 2);
  id = id0 + al + 1;
  pt.x += s->margin.w;
  pt.y += s->margin.h;
  *phitbd = iPOINT(-1, -1);
  for (i = 0; i < s->rows; pt.y += s->ro[i] + s->gap.h, ++i) {
    IPOINT pt0 = pt, pt1;
    for (j = 0; j < s->cols; pt0.x += s->co[j] + s->gap.w, ++j) {
      if ('%' == id[i * al + j]) {
        pt1.x = pt0.x + s->co[j];
        pt1.y = pt0.y + s->ro[i];
        for (k = j + 1; k < s->cols && _lay_h == id[i * al + k]; ++k) {
          pt1.x += s->co[k] + s->gap.w;
        }
        for (k = i + 1; k < s->rows && _lay_v == id[k * al + j]; ++k) {
          pt1.y += s->ro[k] + s->gap.h;
        }
        rc0 = iRECT(pt0.x, pt0.y, pt1.x, pt1.y);
        if (ctrl) {
          ctrl_setrect(ctrl, rc0);
          ctrl = ctrl->next;
        }
        // 只判断左边界和上边界 5 = (1<<0 | 1<<2)
        if (0 == (ret & 5)) {
          ret = iPtInRectBorder(&rc0, x, y, s->gap.w, s->gap.h, s->gap.w, s->gap.h);
          if (ret) {
            if (phitbd->x < 0 && j > 0 && (1 & ret)) {
              phitbd->x = j - 1;
            }
            if (phitbd->x < 0 && j == 0 && (2 & ret)) {
              phitbd->x = j;
              ret |= 1;
            }
            if (phitbd->y < 0 && i > 0 && (4 & ret)) {
              phitbd->y = i - 1;
            }
            if (phitbd->y < 0 && i == 0 && (8 & ret)) {
              phitbd->y = i;
              ret |= 4;
            }
          }
        }
      }
    }
  }
  return (phitbd->x >= 0 || phitbd->y >= 0) ? ret : 0;
}
CC_INLINE LRESULT layout_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  IRECT rc = c->rc;
  layout_t* s = (layout_t*)c->x;
  LRESULT ret = 0;
  int i, j, al;
  const char* id0;
  const char* id;
  IPOINT hitpt = { -1, -1};
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  al = s->cols + 2;
  id0 = s->id;
  id = id0 + al + 1;
  switch (uMsg) {
  case WM_CTRLINIT:
    s->hit = -1;
    s->hot = -1;
    s->hitbd = iPOINT(-1, -1);
    //c->isshow = SW_HIDE;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    FREE(s->co);
    FREE(s->ro);
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    int cx = 0, cy = 0;
    ctrl_t* ctrl = c->sub;
    MEMSET(s->ro, 0, s->rows);
    MEMSET(s->co, 0, s->cols);
    for (i = 0; i < s->rows; ++i) {
      int cx1 = 0, cy1 = 0;
      for (j = 0; j < s->cols; ++j) {
        if ('%' == id[i * al + j] && ctrl) {
          ctrl->ctrlproc(c, ctrl, uMsg, wParam, lParam);
          cx1 += mminfo->ptMinTrackSize.x;
          cy1 = MAX(cy1, mminfo->ptMinTrackSize.y);
          if ('a' == id0[(i + 1)*al]) {
            s->ro[i] = MAX(s->ro[i], mminfo->ptMinTrackSize.y);
          }
          if ('a' == id0[j + 1]) {
            s->co[j] = MAX(s->co[i], mminfo->ptMinTrackSize.x);
          }
          ctrl = ctrl->next;
        }
      }
      cx = MAX(cx, cx1);
      cy += cy1;
    }
    mminfo->ptMinTrackSize.x = cx + MAX(0, s->cols - 1) * s->gap.w;
    mminfo->ptMinTrackSize.y = cy + MAX(0, s->rows - 1) * s->gap.h;
    if (s->text) {
      ISIZE sz = font_text_size(c->fo, s->text, -1, 0);
      mminfo->ptMinTrackSize.x = MAX(sz.w, mminfo->ptMinTrackSize.x) + 8;
      mminfo->ptMinTrackSize.y += sz.h + 4;
    }
    return 0;
  }
  break;
  case WM_LBUTTONUP:
    if (c->sys->ldown) {
      s->hit = 0;
      s->hitbd = iPOINT(-1, -1);
      if (iPtInRect(&rc, x, y)) {
        force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      int ret = layout_getrect(s, rc, x, y, &hitpt);
      if ((1 & ret || 2 & ret)) {
        sys_setcursor(c->sys, IDC_SIZEWE);
      }
      else if (4 & ret || 8 & ret) {
        sys_setcursor(c->sys, IDC_SIZENS);
      }
      if (ret) {
        force_redraw(c);
      }
      s->lastdownpt = iPOINT(x, y);
      s->hitbd = hitpt;
      ctrl_setfocus(c);
      return s->hitbd.x >= 0 || s->hitbd.y >= 0;
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      int hit = -1;
      int size_changed = 0;
      if (!c->sys->ldown) {
        int ret = layout_getrect(s, rc, x, y, &hitpt);
        if (1 & ret || 2 & ret) {
          if ('x' == id0[(hitpt.x + 1)] && 'x' == id0[(hitpt.x + 2)]) {
            sys_setcursor(c->sys, IDC_SIZEWE);
          }
        }
        else if (4 & ret || 8 & ret) {
          if ('x' == id0[(hitpt.y + 1)*al] && 'x' == id0[(hitpt.y + 2)*al]) {
            sys_setcursor(c->sys, IDC_SIZENS);
          }
        }
      }
      else {
        if (s->hitbd.x >= 0 || s->hitbd.y >= 0) {
          if (s->hitbd.x >= 0) {
            int d = x - s->lastdownpt.x;
            s->co[s->hitbd.x] += d;
            s->co[s->hitbd.x + 1] -= d;
          }
          if (s->hitbd.y >= 0) {
            int d = y - s->lastdownpt.y;
            s->ro[s->hitbd.y] += d;
            s->ro[s->hitbd.y + 1] -= d;
          }
          force_redraw(c);
        }
        s->lastdownpt = iPOINT(x, y);
        ret = layout_getrect(s, rc, x, y, &hitpt);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      layout_getrect(s, rc, 0, 0, &hitpt);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      if (s->text) {
        img_t* im = (img_t*)wParam;
        groupbox_draw(im, &rcclip, rc, s->text, -1, BDS_ETCHED, 0, c->fo);
      }
      //imdraw_rect(im, 0, rc, _rgb(1,0,1), _rgb(0,0,0), 1, 0);
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
CC_INLINE int layout_set(layout_t* s, ctrl_t* li, int gap, const char* text, const char* fmt, ...)
{
  const char* id0 = fmt;
  const char* id;
  int i, r = 0, c = 0, al, idl = strlen(fmt);
  ctrl_set(layout_proc, s, s->c, li, WT_CHILD, text);
  s->rows = s->cols = 0;
  s->gap.h = s->gap.w = gap;
  s->id = fmt;
  s->text = text;
  for (i = 0; i < idl; ++i) {
    if (';' == id0[i]) {
      if (s->cols > 0) {
        ASSERT(s->cols == c - 1);
      }
      s->cols = c - 1;
      r++;
      c = 0;
    }
    else {
      ++c;
    }
  }
  s->rows = r - 1;
  al = s->cols + 2;
  id = id0 + al + 1;
  s->ro = MALLOC(int, s->rows);
  s->co = MALLOC(int, s->cols);
  if (fmt) {
    int j;
    va_list arglist;
    va_start(arglist, fmt);
    for (i = 0; i < s->rows; ++i) {
      for (j = 0; j < s->cols; ++j) {
        if ('%' == id[i * al + j]) {
          ctrl_t* sub = va_arg(arglist, ctrl_t*);
          ctrl_addsub_tail(s->c, sub);
        }
      }
    }
    va_end(arglist);
  }
  return 0;
}
static int cut_layout_calc_rect(cut_layout_t* s, int x, int y)
{
  int i, ret = -1;
  IRECT rc[10] = {0};
  int id[10] = {0};
  rc[0] = s->c->rc;
  id[0] = 0;
  if (s->len > 0 && s->li[0].c) {
    s->li[0].c->rc = s->c->rc;
  }
  for (i = 1; i < s->len; ++i) {
    int ind = s->li[i].ind;
    IRECT* prc0 = (s->li[id[ind - 1]].c) ? &s->li[id[ind - 1]].c->rc : (rc + ind - 1);
    IRECT* prc1 = (s->li[i].c) ? &s->li[i].c->rc : (rc + ind);
    int d = s->li[i].d, cmd = s->li[i].cmd;
    id[ind] = i;
    ASSERT(ind < 10);
#if 0
    if (0 == d && s->li[i].c) {
      MINMAXINFO mminfo[1] = {0};
      ctrl_t* c = s->li[i].c;
      //GetWindowRect(win->hwnd, &rc);
      c->ctrlproc(0, c, 0, WM_GETMINMAXINFO, 0, (LPARAM)mminfo);
      d = ('l' == cmd || 'r' == cmd) ? mminfo->ptMinTrackSize.x : mminfo->ptMinTrackSize.y;
    }
#endif
    if (iRectCut(prc0, cmd, d, s->gap, x, y, s->mouserange, prc1)) {
      ret = i;
    }
  }
  return ret;
}
static LRESULT cut_layout_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  cut_layout_t* s = (cut_layout_t*)c->x;
  IRECT rc = c->rc;
  LRESULT ret = 0;
  int hit, cmd, i;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    s->hit = -1;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x += 120;
    mminfo->ptMinTrackSize.y += 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_LBUTTONUP:
    if (s) {
      s->hit = -1;
      if (iPtInRect(&rc, x, y)) {
        force_redraw(c);
      }
    }
    break;
  case WM_COMMAND:
    if (s) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      int hit = cut_layout_calc_rect(s, x, y);
      if (hit >= 0) {
        cmd = s->li[hit].cmd;
        if ('l' == cmd || 'r' == cmd) {
          sys_setcursor(c->sys, IDC_SIZEWE);
        }
        if ('t' == cmd || 'b' == cmd) {
          sys_setcursor(c->sys, IDC_SIZENS);
        }
        s->hit = hit;
        s->pt = iPOINT(x, y);
        force_redraw(c);
        return TRUE;
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      if (s->hit < 0) {
        hit = cut_layout_calc_rect(s, x, y);
        if (hit >= 0 && !c->sys->ldown) {
          cmd = s->li[hit].cmd;
          if ('l' == cmd || 'r' == cmd) {
            sys_setcursor(c->sys, IDC_SIZEWE);
          }
          if ('t' == cmd || 'b' == cmd) {
            sys_setcursor(c->sys, IDC_SIZENS);
          }
          s->pt = iPOINT(x, y);
          force_redraw(c);
        }
        else {
          sys_setcursor(c->sys, IDC_ARROW);
        }
      }
      else {
        hit = s->hit;
        cmd = s->li[hit].cmd;
        if ('l' == cmd) {
          s->li[hit].d -= s->pt.x - x;
        }
        if ('r' == cmd) {
          s->li[hit].d += s->pt.x - x;
        }
        if ('t' == cmd) {
          s->li[hit].d -= s->pt.y - y;
        }
        if ('b' == cmd) {
          s->li[hit].d += s->pt.y - y;
        }
        s->pt = iPOINT(x, y);
        cut_layout_calc_rect(s, x, y);
        for (i = 0; i < s->len; ++i) {
          ctrl_t* sub = s->li[i].c;
          if (sub) {
            sub->recalcsize = 1;
          }
        }
        force_redraw(c);
        return TRUE;
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      cut_layout_calc_rect(s, 0, 0);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int ret = -1, d = 0;
      //imdraw_fillrect(im, &rcclip, rc, _rgb(1, 1, 1));
    }
    break;
  }
  return 0;
}
static tooltip_t g_tip[1] = {0};
static int myGetCursorPos(int* x, int* y)
{
  POINT pt;
  GetCursorPos(&pt);
  *x = pt.x, *y = pt.y;
  return 0;
}
static LRESULT tooltip_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  tooltip_t* s = (tooltip_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    s->time_start = GetTickCount();
    s->delay = 5000;
    s->show = 0;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    s->to = NULL;
    s->delay = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = font_text_size(c->fo, c->text, -1, DT_SINGLELINE);
    mminfo->ptMinTrackSize.x = sz.w + 4;
    mminfo->ptMinTrackSize.y = sz.h + 4;
  }
  break;
  case WM_TIMER: {
    int x, y;
    myGetCursorPos(&x, &y);
    if (c->text) {
      int tt = (int)(GetTickCount() - s->time_start);
      if (tt > s->delay) {
        ctrl_close(c);
        //printf("ctrl_close %d\n", tt);
      }
      else if (!s->show && tt > 1000) {
        s->show = 1;
        ctrl_move(c, x + 16, y + 16, RCW(&rc), RCH(&rc), 1);
        c->isshow = SW_SHOWNOACTIVATE;
      }
    }
    if ((abs(x - s->x) + abs(y - s->y)) > 40) {
      s->to = NULL;
      if (c->text) {
        ctrl_close(c);
      }
    }
  }
  break;
  case WM_LBUTTONUP:
    if (s) {
    }
    break;
  case WM_COMMAND:
    if (s) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {}
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      imdraw_rect(im, &rcclip, rc, CLR_INFOBK, _RGB(0, 0, 0), 1, BDM_ALL);
      imdraw_text(im, &rcclip, rc, c->text, -1, c->fo, _RGB(0, 0, 0), 0, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
      //nHeight = dc.DrawText(m_strTitle, rect, 0 | DT_LEFT | DT_EDITCONTROL | DT_NOPREFIX | DT_WORDBREAK );
    }
    break;
  }
  return 0;
}
static int show_tooltip(ctrl_t* to, int x, int y, const char* text)
{
  if (g_tip->to != to && iPtInRect(&to->rc, x, y)) {
    ctrl_t* c = to;
    sys_t* sys = c->sys;
    ctrl_close(g_tip->c);
    ctrl_set(tooltip_proc, g_tip, g_tip->c, c->listener, WT_TOOLTIP, text);
    g_tip->delay = 4000;
    g_tip->to = to;
    g_tip->time_start = GetTickCount();
    //printf("show_tooltip\n");
    //myClientToScreen(hwnd, &x, &y);
    g_tip->x = x;
    g_tip->y = y;
    //ctrl_setrect(g_tip->c, x + 1, y + 1, 0, 0);
    return 1;
  }
  return 0;
}
// logprintf
static int tabctrl_additems(tabctrl_t* s, int n, tab_item* items)
{
  s->nitem = n;
  s->items = items;
  return 0;
}
CC_INLINE int tabctrl_get_tabheight(const tabctrl_t* s)
{
  int text_height = font_height(s->c->fo);
  return text_height + 8;
}
CC_INLINE int tabctrl_get_tabwidth(const tabctrl_t* s, int i)
{
  return font_text_size(s->c->fo, s->items[i].text, -1, DT_SINGLELINE).w + 12;
}
#define TABBD 2
CC_INLINE int tabctrl_calc_rect(tabctrl_t* s, IRECT* pheader, IRECT* pclient, IRECT* pclientin, IRECT* rcItems)
{
  IRECT rc = s->c->rc;
  IRECT rc2 = rc, rc3;
  int i, tabheight = tabctrl_get_tabheight(s);
  iRectCutT(&rc, tabheight, 0, &rc2);
  if (pheader) {
    *pheader = rc2;
  }
  if (pclient) {
    *pclient = rc;
  }
  if (pclientin) {
    *pclientin = rc;
    RCINFLATE(pclientin, -TABBD, -TABBD);
  }
  if (rcItems) {
    rc2.l += s->headoff;
    for (i = 0; i < s->nitem; ++i) {
      int cx = tabctrl_get_tabwidth(s, i);
      rcItems[i] = iRECT(0, 0, 0, 0);
      if (iRectCutL(&rc2, cx, 0, &rc3)) {
        RCOFFSET(&rc3, 0, 2, 0, 0);
        rcItems[i] = rc3;
      }
    }
  }
  return 0;
}
CC_INLINE int tabctrl_calc_item_rect(tabctrl_t* s, int i, IRECT* prc2, IRECT* prcItem)
{
  int cx = tabctrl_get_tabwidth(s, i);
  *prcItem = iRECT(0, 0, 0, 0);
  if (iRectCutL(prc2, cx, 0, prcItem)) {
    RCOFFSET(prcItem, 0, 2, 0, 0);
    return 1;
  }
  return 0;
}
CC_INLINE ctrl_t* ctrl_getsub(ctrl_t* c, int i)
{
  int j = 0;
  ctrl_t* sub = c->sub;
  for (; j < i && sub; sub = sub->next, ++j) {}
  return sub;
}
CC_INLINE int tabctrl_hittest(tabctrl_t* s, int x, int y)
{
  int i;
  IRECT header = {0}, rcItem;
  tabctrl_calc_rect(s, &header, 0, 0, 0);
  header.l += s->headoff;
  for (i = 0; i < s->nitem && tabctrl_calc_item_rect(s, i, &header, &rcItem); ++i) {
    if (iPtInRect(&rcItem, x, y)) {
      return i;
    }
  }
  return -1;
}
static tabctrl_drawitem(img_t* im, const IRECT* pclip, tabctrl_t* s, int i, IRECT rc3, COLOR m_border_color, COLOR m_background_color, COLOR clrText, const font_t* fo)
{
  if (RCH(&rc3) > 0) {
    int flag;
    if (0) {
      flag = BDM_T | (i != s->cur ? BDM_B : 0) | (i <= s->cur ? BDM_L : 0) | (i >= s->cur ? BDM_R : 0);
      rc3.r -= (i == (s->cur - 1));
      rc3.l -= (i == s->cur);
      rc3.t -= (i == s->cur);
      rc3.r += (i == s->cur);
      rc3.l += (i == (s->cur + 1));
    }
    else {
      rc3.r += 1;
      flag = BDM_T | (i != s->cur ? BDM_B : 0) | BDM_L | BDM_R;
    }
    imdraw_rect(im, &rc3, rc3, m_background_color, m_border_color, 1, flag);
    imdraw_text(im, &rc3, rc3, s->items[i].text, -1, fo, clrText, 0, DT_VCENTER | DT_CENTER);
  }
  return 0;
}
static LRESULT tabctrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  tabctrl_t* s = (tabctrl_t*)c->x;
  IRECT rc = c->rc, header, client, clientin;
  int i, hit, x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    s->hot = s->hit = -1;
    s->headoff = 4;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 20;
    mminfo->ptMinTrackSize.y = 20;
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
      if (s->hit >= 0 && s->hit < s->nitem) {
        s->cur = s->hit;
      }
      s->hit = -1;
      if (iPtInRect(&rc, x, y)) {
        force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      ctrl_t* sub;
      hit = tabctrl_hittest(s, x, y);
      if (hit != s->hit) {
        s->hit = hit;
        if (hit >= 0 && hit < s->nitem) {
          if (hit != s->cur) {
            //LPNMHDR
            s->cur = hit;
            sendmsg(c, c->listener, WM_NOTIFY, (WPARAM)c, TCN_SELCHANGE);
          }
        }
        //tab_ctrl_hitrect(s, rc, fo);
        sub = ctrl_getsub(c, s->cur);
        if (sub) {
          sub->redraw = 1;
          sub->recalcsize = 1;
        }
        force_redraw(c);
      }
      s->cur_old = s->cur;
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
      int hot = iPtInRect(&rc, x, y);
      if (s->hot != hot) {
        s->hot = hot;
        //force_redraw(c);
      }
      if (c->sys->ldown && s->hit >= 0) {
        tabctrl_calc_rect(s, &header, &client, &clientin, 0);
        hit = tabctrl_hittest(s, x, header.b - 2);
        if (hit >= 0 && hit < s->nitem && s->hit != hit) {
          int aa = MIN(s->hit, hit);
          int bb = MAX(s->hit, hit);
          for (; aa < bb; ++aa) {
            tab_item tt;
            CV_SWAP(s->items[aa], s->items[aa + 1], tt);
          }
          s->cur = s->hit = hit;
        }
        //sys_setcursor(c->sys, IDC_HAND);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      tabctrl_calc_rect(s, &header, &client, &clientin, 0);
      for (i = 0; i < s->nitem; ++i) {
        ctrl_t* sub = ctrl_getsub(c, i);
        if (sub) {
          ctrl_setrect(sub, clientin);
        }
      }
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i;
      IRECT rc3, rcItem;
      COLOR m_background_color[2] = { _rgb(1.0, 0.9, 0.8), _rgb(1.0, 1.0, 1.0)};
      COLOR m_border_color = (_rgb(0.3, 0.3, 0.3));
      COLOR clrText = Color_Black;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      IRECT rccur = {0};
      IRECT rccur_old = {0};
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      tabctrl_calc_rect(s, &header, &client, &clientin, 0);
      //imdraw_rect(im, rc, m_background_color[0], m_border_color, 1);
      imdraw_rect(im, &rcclip, header, m_background_color[0], m_border_color, 1, BDM_B);
      imdraw_rect(im, &rcclip, client, 0, m_background_color[1], TABBD, BDM_ALL);
      header.l += s->headoff;
      for (i = 0; i < s->nitem && tabctrl_calc_item_rect(s, i, &header, &rcItem); ++i) {
        rc3 = rcItem;
        if (s->cur != i) {
          tabctrl_drawitem(im, &rc3, s, i, rc3, m_border_color, m_background_color[0], clrText, c->fo);
        }
        else {
          rccur = rc3;
        }
        if (s->cur_old == i) {
          rccur_old = rc3;
        }
      }
      if (s->cur >= 0 && s->cur < s->nitem) {
        if (s->hit >= 0) {
          rccur = rccur_old;
          iRectOffset2(&rccur, c->sys->ptMouse.x - c->sys->ptLDown.x, 0);
        }
        tabctrl_drawitem(im, &rcclip, s, s->cur, rccur, m_border_color, m_background_color[1], clrText, c->fo);
      }
      //draw_border(im, s->client, BDS_HIGH2);
      //draw_round_border(im, s->client, bdt, BDM_ALL);
      if (i >= 0 && i < s->nitem) {
        //UICALL_DRAW(msg->wid[s->items[i].id], msg);
      }
    }
    break;
  }
  return 0;
}
CC_INLINE int tabctrl_set(tabctrl_t* s, ctrl_t* li, ctrl_t* fa)
{
  ctrl_set(tabctrl_proc, s, s->c, li, WT_CHILD, 0);
  return 0;
}
static int scrollview_set_pos(SCROLLINFO* si, int nPos)
{
  si->nPos = (int)BOUND(nPos, si->nMin, si->nMax - (int)si->nPage);
  return 0;
}
static int scrollview_set_info(SCROLLINFO* si, int nMin, int nMax, int nPage)
{
  ASSERT(nMin < nMax);
  si->nMin = nMin;
  si->nMax = nMax;
  si->nPage = MIN(nPage, nMax);
  scrollview_set_pos(si, si->nPos);
  return 0;
}
CC_INLINE int scrollview_calc_rect_impl(SCROLLINFO* si, IRECT rc, int sz, int isv, IRECT* out)
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
CC_INLINE int scrollview_calc_rect(scrollview_t* s, ctrl_t* c, IRECT* prc)
{
  int i = 0, sz = SBW1;
  IRECT rcc, rc = c->rc;
  rcc = rc;
  if (c->style & WS_HSCROLL) {
    IRECT rc1 = rc;
    i = 0;
    rc1.r -= (c->style & WS_VSCROLL) ? sz : 0;
    if (scrollview_calc_rect_impl(s->si + i, rc1, sz, i, prc + 2)) {
    }
    rcc.r -= sz;
  }
  if (c->style & WS_VSCROLL) {
    IRECT rc1 = rc;
    i = 1;
    rc1.b -= (c->style & WS_HSCROLL) ? sz : 0;
    if (scrollview_calc_rect_impl(s->si + i, rc1, sz, i, prc + 2 + 6)) {
    }
    rcc.b -= sz;
  }
  prc[0] = rcc;
  prc[1] = iRECT2(rcc.r, rcc.b, sz, sz);
  return 0;
}
CC_INLINE int scrollview_hittest(scrollview_t* s, ctrl_t* c, int x, int y)
{
  int i = 0, j;
  IRECT prc[14] = {0};
  scrollview_calc_rect(s, c, prc);
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
CC_INLINE int scrollview_draw(scrollview_t* s, ctrl_t* c, img_t* im, IRECT* prc)
{
  COLOR cr3dFace = _GetSysColor(CLR_3DFACE);
  COLOR clrs[] = {_RGB(255, 255, 255), cr3dFace};
  int nState[4] = {IdArowLeftLarge, IdArowRightLarge, IdArowUpLarge, IdArowDownLarge, };
  int i;
  scrollview_calc_rect(s, c, prc);
  imdraw_rect(im, &c->rcclip, prc[1], cr3dFace, 0, 0, 0);
  //printf("WM_PAINT scrollview\n");
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
CC_INLINE LRESULT scrollview_proc(scrollview_t* s, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
      hit = scrollview_hittest(s, c, x, y);
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
          scrollview_set_pos(s->si + i, s->si[i].nTrackPos);
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
        scrollview_set_pos(s->si + i, s->si[i].nTrackPos);
        //sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, SB_THUMBTRACK), s->si[i].nTrackPos);
        force_redraw(c);
      }
    }
    else if (iPtInRect(&rc, x, y)) {
      hit = scrollview_hittest(s, c, x, y);
      if (s->hot != hit) {
        s->hot = hit;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      IRECT prc[14] = {0};
      scrollview_calc_rect(s, c, prc);
    }
    if (c->redraw && RCH(&rc) > 0 && RCW(&rc) > 0) {
      img_t* im = (img_t*)wParam;
      IRECT prc[14] = {0};
      //scrollview_draw(s, c, im, prc);
    }
    break;
  }
  return 0;
}
#if 1
static int scrollbar_set_info(SCROLLINFO* si, int nMin, int nMax, int nPage)
{
  si->nMin = nMin;
  si->nMax = nMax;
  si->nPage = MIN(nPage, nMax);
  return 0;
}
static int scrollbar_set_pos(SCROLLINFO* si, int nPos)
{
  si->nPos = (int)BOUND(nPos, si->nMin, 1 + si->nMax - (int)si->nPage);
  return 0;
}
CC_INLINE int scrollbar_calc_rect_impl(IRECT rc, int sz, int isv, int nPos, int nPage, int nMin, int nMax, IRECT* out)
{
  int len = nMax + 1 - nMin;
  memset(out, 0, sizeof(IRECT) * 6);
  if (isv) {
    int l = rc.r - sz, a = rc.t + sz, b = rc.b - sz, ll = b - a - THUMB_MIN;
    int c = a + nPos * ll / len, d = THUMB_MIN + a + ((nPos + nPage) * ll + len - 1) / len;
    d = BOUND(d, c, b);
    if (d < b) {
      out[SBHT_LINE_0] = iRECT(l, rc.t, rc.r, a);
      out[SBHT_LINE_1] = iRECT(l, b, rc.r, rc.b);
      out[SBHT_PAGE_0] = iRECT(l, a, rc.r, c);
      out[SBHT_PAGE_1] = iRECT(l, d, rc.r, b);
      out[SBHT_THUMB] = iRECT(l, c, rc.r, d);
    }
    return d < b;
  }
  else {
    int l = rc.b - sz, a = rc.l + sz, b = rc.r - sz, ll = b - a - THUMB_MIN;
    int c = a + nPos * ll / len, d = THUMB_MIN + c + (nPage * ll + len / 2) / len;
    d = BOUND(d, c, b);
    if (d < b) {
      out[SBHT_LINE_0] = iRECT(rc.l, l, a, rc.b);
      out[SBHT_LINE_1] = iRECT(b, l, rc.r, rc.b);
      out[SBHT_PAGE_0] = iRECT(a, l, c, rc.b);
      out[SBHT_PAGE_1] = iRECT(d, l, b, rc.b);
      out[SBHT_THUMB] = iRECT(c, l, d, rc.b);
    }
    return d < b;
  }
  return 0;
}
CC_INLINE int scrollbar_calc_rect(scrollbar_t* s, IRECT rc)
{
  int i = 0, sz = SBW1;
  IRECT rc1 = rc;
  s->rcc = rc;
  if (s->styles & WS_HSCROLL) {
    i = 0, rc1 = s->rcc, rc1.b += sz;
    if (scrollbar_calc_rect_impl(rc1, sz, i, s->si[i].nPos, s->si[i].nPage, s->si[i].nMin, s->si[i].nMax, s->rc[i])) {
      s->rcc.r -= sz;
    }
  }
  if (s->styles & WS_VSCROLL) {
    i = 1, rc1 = s->rcc, rc1.r += sz;
    if (scrollbar_calc_rect_impl(rc1, sz, i, s->si[i].nPos, s->si[i].nPage, s->si[i].nMin, s->si[i].nMax, s->rc[i])) {
      s->rcc.b -= sz;
    }
  }
  return 0;
}
CC_INLINE int scrollbar_hittest(scrollbar_t* s, int x, int y)
{
  int i = 0, j;
  for (i = 0; i < 2; ++i) {
    IRECT* rc = s->rc[i];
    for (j = 0; j < 5; ++j) {
      if (iPtInRect(rc + j, x, y)) {
        return (i << 8) | (j + 1);
      }
    }
  }
  return 0;
}
CC_INLINE LRESULT scrollbar_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int i;
  scrollbar_t* s = (scrollbar_t*)c->x;
  IRECT rc = c->rc;
  int hit = 0, x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    //s->si[0].nPos = (int)((0.1) * (1 << 16));
    //s->si[0].nPage = (int)((0.5) * (1 << 16));
    //s->si[1].nPos = (int)((0.1) * (1 << 16));
    //s->si[1].nPage = (int)((0.5) * (1 << 16));
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
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
      hit = scrollbar_hittest(s, x, y);
      s->pthit = iPOINT(x, y);
      if (hit != s->hit) {
        int i = hit >> 8;
        s->hit = hit;
        if (SBHT_THUMB == ((s->hit & 0xff) - 1)) {
          s->hit_pos = s->si[s->hit >> 8].nPos;
        }
        else {
          int page = (int)s->si[i].nPage;
          int line = MAX(1, page / 8);
          int steps[5] = { -line, line, -page, page};
          int msgs[5] = { SB_LINEUP, SB_LINEDOWN, SB_PAGEUP, SB_PAGEDOWN};
          s->si[i].nTrackPos = s->si[i].nPos + steps[(s->hit & 0xff) - 1];
          s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - page);
          //s->si[i].nTrackPos = s->si[i].nPos + steps[(s->hit & 0xff) - 1];
          //BOUND(, 0, (1 << 16) - page);
          sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, msgs[(s->hit & 0xff) - 1]), 0);
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
        sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, SB_THUMBTRACK), s->si[i].nTrackPos);
        force_redraw(c);
      }
    }
    else if (iPtInRect(&rc, x, y)) {
      hit = scrollbar_hittest(s, x, y);
      if (s->hot != hit) {
        s->hot = hit;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      scrollbar_calc_rect(s, rc);
    }
    if (c->redraw && RCH(&rc) > 0 && RCW(&rc) > 0) {
      img_t* im = (img_t*)wParam;
      COLOR clrs[] = {_RGB(255, 255, 255), _GetSysColor(CLR_3DFACE)};
      int nState[4] = {IdArowLeftLarge, IdArowRightLarge, IdArowUpLarge, IdArowDownLarge, };
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      scrollbar_calc_rect(s, rc);
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //printf("WM_PAINT scrollbar\n");
      for (i = 0; i < 2; ++i) {
        IRECT* rc1 = s->rc[i];
        imdraw_rect_colors(im, &rcclip, rc1[SBHT_PAGE_0], clrs, 2, 0);
        imdraw_rect_colors(im, &rcclip, rc1[SBHT_PAGE_1], clrs, 2, 0);
        imdraw_border_syscolor(im, &rcclip, rc1[SBHT_LINE_0], (i == (s->hit >> 8) && SBHT_LINE_0 == ((s->hit & 0xff) - 1)) ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
        imdraw_menuimg(im, &rcclip, rc1[SBHT_LINE_0], nState[i * 2 + 0], _rgb(0, 0, 0));
        imdraw_border_syscolor(im, &rcclip, rc1[SBHT_LINE_1], (i == (s->hit >> 8) && SBHT_LINE_1 == ((s->hit & 0xff) - 1)) ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
        imdraw_menuimg(im, &rcclip, rc1[SBHT_LINE_1], nState[i * 2 + 1], _rgb(0, 0, 0));
        imdraw_border_syscolor(im, &rcclip, rc1[SBHT_THUMB], BDS_HIGH, BDM_ALL);
      }
    }
    break;
  }
  return 0;
}
CC_INLINE int scrollbar_set(scrollbar_t* s, ctrl_t* li, ctrl_t* fa)
{
  ctrl_set(scrollbar_proc, s, s->c, li, WT_CHILD, 0);
  return 0;
}
#endif
static int imageview_update_scroll(imageview_t* s)
{
  int w = (int)(s->im->w * s->scale);
  int h = (int)(s->im->h * s->scale);
  int w2 = RCW(&s->c->rc) - SBW1;
  int h2 = RCH(&s->c->rc) - SBW1;
  scrollview_set_info(s->sc->si + 0, 0, w, w2);
  scrollview_set_info(s->sc->si + 1, 0, h, h2);
  scrollview_set_pos(s->sc->si + 0, s->sc->si[0].nPos);
  scrollview_set_pos(s->sc->si + 1, s->sc->si[1].nPos);
  return 0;
}
static int imageview_setimg(imageview_t* s, const img_t* im)
{
  if (im) {
    imclone2(im, s->im);
    imageview_update_scroll(s);
  }
  return 0;
}
static LRESULT imageview_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  imageview_t* s = (imageview_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int ret = 0;
  if (WM_PAINT != uMsg) {
    if (ret = scrollview_proc(s->sc, c, uMsg, wParam, lParam)) {
      return ret;
    }
  }
  switch (uMsg) {
  case WM_CTRLINIT:
    c->style = WS_HSCROLL | WS_VSCROLL;
    s->scale = 1.;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    imfree(s->im);
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 120;
  }
  break;
  case WM_TIMER:
    if (1) {
      force_redraw(c);
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      sys_setcursor(c->sys, IDC_HAND);
      force_redraw(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEWHEEL:
    if (iPtInRect(&rc, x, y)) {
      if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
        s->scale /= 1.1;
      }
      else {
        s->scale *= 1.1;
      }
      s->scale = BOUND(s->scale, 0.1, 10);
      imageview_update_scroll(s);
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
      int hot = iPtInRect(&rc, x, y);
      if (hot) {
        sys_setcursor(c->sys, IDC_HAND);
      }
      if (s->hot != hot) {
        s->hot = hot;
        force_redraw(c);
      }
      if (c->sys->ldown && ctrl_isfocus(c)) {
        //printf("x = %d ptMouse.x = %d\n", x, c->sys->ptMouse.x);
        scrollview_set_pos(s->sc->si + 0, s->sc->si[0].nPos - (x - c->sys->ptMouse.x));
        scrollview_set_pos(s->sc->si + 1, s->sc->si[1].nPos - (y - c->sys->ptMouse.y));
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      imageview_update_scroll(s);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rca, prc[14] = {0};
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      scrollview_draw(s->sc, c, im, prc);
      //rcb = iRECT2(s->sc->si[0].nPos, s->sc->si[1].nPos, s->sc->si[0].nPage, s->sc->si[1].nPage);
      imdraw_rect(im, &rcclip, *prc, _rgb(1, 1, 1), 0, 0, 0);
      {
        int w = (int)(s->im->w * s->scale);
        int h = (int)(s->im->h * s->scale);
        rca = iRECT2(rc.l - s->sc->si[0].nPos, rc.t - s->sc->si[1].nPos, w, h);
        if (RCW(&rca) < RCW(prc)) {
          int d = (RCW(prc) - RCW(&rca)) / 2;
          iRectOff2(&rca, d, 0);
        }
        if (RCH(&rca) < RCH(prc)) {
          int d = (RCH(prc) - RCH(&rca)) / 2;
          iRectOff2(&rca, 0, d);
        }
      }
      imdraw_image_corner(im, prc, rca, s->im, 0, 0, 0);
      iRectIn(&rca, -1);
      imdraw_rect(im, prc, rca, 0, _rgb(0, 0, 0), 1, BDM_ALL);
      {
        font_t* fo = sys_font_load_win32(c->sys, "宋体|72");
        imdraw_char(im, prc, rca, 100, *(unsigned short*)"我", fo, _rgb(0, 0, 0), _rgba(1, 1, 1, 0.5));
      }
    }
    break;
  }
  return 0;
}
CC_INLINE int imageview_set(imageview_t* s, ctrl_t* li, ctrl_t* fa)
{
  ctrl_set(imageview_proc, s, s->c, li, WT_CHILD, 0);
  return 0;
}
CC_INLINE int grid_ctrl_getallrect(const gridctrl_t* s, IRECT* prc)
{
  int i;
  *prc = iRECT(0, 0, 0, 0);
  for (i = 1; i < s->rows; ++i) {
    if (i < s->lt.y) {
      prc->t -= s->rowh[i] + 1;
    }
    else {
      prc->b += s->rowh[i] + 1;
    }
  }
  for (i = 1; i < s->cols; ++i) {
    if (i < s->lt.x) {
      prc->l -= s->colw[i] + 1;
    }
    else {
      prc->r += s->colw[i] + 1;
    }
  }
  return 0;
}
static int gridctrl_getclip(const gridctrl_t* s, const IRECT* prc, IRECT* out)
{
  int i;
  IRECT rc;
  ISIZE sz;
  rc = *prc;
  RCOFFSET(&rc, 1, 1, -1, -1);
  sz = iSIZE(rc.l, rc.t);
  for (i = 0; i < s->rows && sz.h < rc.b; ++i) {
    if (i > 0 && i < s->lt.y) {
      continue;
    }
    sz.h += s->rowh[i] + 1;
  }
  for (i = 0; i < s->cols && sz.w < rc.r; ++i) {
    if (i > 0 && i < s->lt.x) {
      continue;
    }
    sz.w += s->colw[i] + 1;
  }
  *out = iRECT(rc.l, rc.t, sz.w - 1, sz.h - 1);
  //RectClip();
  *out = iRectInter(*out, rc);
  return 0;
}
#define CELLBD 1
#define CELLBDW 1
//#define HITONLINE (1<<30)
//#define HITGET(x) ((HITONLINE-1)&(x)-1)
// ret 0 nohit 1 cell 2 line
static int gridctrl_hithelp(const int* arr, int beg, int n, int x, int b, int* hit)
{
  int i, xx = 0;
  *hit = 0;
  if (x < 0 || !arr) {
    return 0;
  }
  for (i = 0; i < n; ++i) {
    if (i > 0 && i < beg) {
      continue;
    }
    xx += arr[i] + CELLBD;
    if (x < (xx - b - CELLBD)) {
      *hit = i;
      return 1;
    }
    if (x < (xx + b)) {
      *hit = i;
      return 2;
    }
  }
  return 0;
}
static int gridctrl_hittest(gridctrl_t* s, int x, int y, IPOINT* hit)
{
  int b = CELLBDW;
  int tx = gridctrl_hithelp(s->colw, s->lt.x, s->cols, x, b, &hit->x);
  int ty = gridctrl_hithelp(s->rowh, s->lt.y, s->rows, y, b, &hit->y);
  int tt = (tx << 2) | ty;
  if (5 != (tt & 5)) {
    //tt &= ~5;
  }
  if (tt) {
    int asdf = 0;
  }
  return tt;
}
static int gridctrl_draw(gridctrl_t* s, img_t* im, const IRECT* pclip, IRECT rc)
{
  IRECT rcclip = iRectInter(rc, iRectClip(pclip, 0, 0, im->w, im->h));
  COLOR crBakH = _GetSysColor(CLR_3DLIGHT);
  COLOR crText = _GetSysColor(CLR_WINDOWTEXT);
  COLOR crBakW = _GetSysColor(CLR_WINDOW);
  COLOR crBorder = _RGB(100, 100, 100);
  COLOR _clr;
  int y, x;
  IRECT rcSelect = iRECT(rc.r, rc.b, 0, 0);
  IRECT rcLineH, rcRow, rcCell, rcLineV, rc1;
  IPOINT sela = {0}, selb = {0};
  rcRow = rcLineH = rc;
  if (5 == (s->hittype & 5)) {
    sela.x = MIN(s->hit.x, s->end.x);
    sela.y = MIN(s->hit.y, s->end.y);
    selb.x = MAX(s->hit.x, s->end.x);
    selb.y = MAX(s->hit.y, s->end.y);
  }
  //imdraw_rect(im, rc, _RGB(255, 255, 255), 0, 0, 0);
  for (y = 0; y < s->rows; ++y) {
    if (y > 0 && y < s->lt.y) {
      continue;
    }
    rcRow.b = rcRow.t + s->rowh[y];
    rcRow.l = rc.l;
    rcCell = rcLineV = rcRow;
    for (x = 0; x < s->cols; ++x) {
      COLOR crSelect = 0;
      int inall = 0, inx = 0, iny = 0;
      if (5 == (s->hittype & 5)) {
        iny = y >= sela.y && y <= selb.y;
        inx = x >= sela.x && x <= selb.x;
        inall = 0 == s->hit.x && 0 == s->hit.y;
      }
      if (x > 0 && x < s->lt.x) {
        continue;
      }
      rcCell.r = rcCell.l + s->colw[x];
      rcLineH.r = rcCell.r + CELLBD;
      rc1 = rcCell;
      if (y > 0 && x > 0) {
        _clr = crBakW;
        if (5 == (s->hittype & 5)) {
          crSelect = inall || (iny && inx) ? _RGB(0, 0, 255) : 0;
          crSelect = (x == s->hit.x && y == s->hit.y) ? _RGB(255, 255, 255) : crSelect;
          if (inx) {
            rcSelect.l = MIN(rcSelect.l, rcCell.l);
            rcSelect.r = MAX(rcSelect.r, rcCell.r);
          }
          if (iny) {
            rcSelect.t = MIN(rcSelect.t, rcCell.t);
            rcSelect.b = MAX(rcSelect.b, rcCell.b);
          }
        }
      }
      else if (0 == y && 0 == x) {
        _clr = crBakH;
        crSelect = inall ? _RGB(255, 255, 0) : 0;
      }
      else {
        _clr = crBakH;
        if (y == 0) {
          crSelect = inx || inall ? _RGB(255, 255, 0) : 0;
        }
        else {
          crSelect = iny || inall ? _RGB(255, 255, 0) : 0;
        }
      }
      if (crSelect) {
        _clr = PixelAlpha3(_clr, crSelect, 80);
      }
      imdraw_rect(im, &rcclip, rc1, _clr, 0, 0, 0);
      if (!iRectIsEmpty(&rc1)) {
        //sendmsg(WM_NOTIFY, (WPARAM)c, TCN_SELCHANGE);
        if (y && x) {
          char buf[256];
          _snprintf(buf, 256, "x=%d, y=%d", x, y);
          imdraw_text(im, &rcclip, rcCell, buf, -1, s->c->fo, crText, 0, DT_VCENTER | DT_CENTER);
        }
      }
      rcCell.l = rcCell.r + CELLBD;
      rcLineV.l += s->colw[x], rcLineV.r = rcLineV.l + CELLBD;
      imdraw_rect(im, &rcclip, rcLineV, crBorder, 0, 0, 0);
      rcLineV.l += CELLBD;
      rcCell.l = rcCell.r + CELLBD;
      if (rcLineV.l >= rc.r) {
        break;
      }
    }
    rcLineH.t += s->rowh[y], rcLineH.b = rcLineH.t + CELLBD;
    if (rcLineH.t >= rc.b) {
      break;
    }
    imdraw_rect(im, &rcclip, rcLineH, crBorder, 0, 0, 0);
    rcLineH.t += CELLBD;
    rcRow.t = rcRow.b + CELLBD;
  }
  if (5 == (s->hittype & 5) && rcSelect.l < rcSelect.r && rcSelect.t < rcSelect.b) {
    rc1 = rcSelect;
    iRectOffset(&rc1, -2, -2, 2, 2);
    imdraw_rect(im, &rcclip, rc1, 0, _RGB(0, 0, 0), 3, BDM_ALL);
  }
  return 0;
}
static LRESULT gridctrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  gridctrl_t* s = (gridctrl_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    //scrollbar_set(s->sc, c, c->sys);
    //gridctrl_set(s, 10, 10);
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (s->down) {
      s->down = 0;
      s->adjusthit = iPOINT(0, 0);
      s->adjusthittype = 0;
      force_redraw(c);
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      IPOINT hit = {0};
      int hitty = gridctrl_hittest(s, x - rc.l, y - rc.t, &hit);
      const char* icur[] = {0, IDC_SIZENS, IDC_SIZEWE, IDC_SIZEALL};
      int tt = ((hitty & 8) >> 2) | ((hitty & 2) >> 1);
      //printf("%d %d %d\n", hitty, hit.x, hit.y);
      s->down = 1;
      if (hitty & 10) {
        sys_setcursor(c->sys, icur[tt]);
        s->adjusthit = hit;
        s->adjusthittype = hitty;
        s->adjustpt.x = s->colw[hit.x] - x;
        s->adjustpt.y = s->rowh[hit.y] - y;
        force_redraw(c);
      }
      else if ((hit.x != s->hit.x || hit.y != s->hit.y) && (hitty & 5)) {
        s->hit = s->end = hit;
        s->hittype = hitty;
        force_redraw(c);
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      IPOINT hit;
      int hitty = gridctrl_hittest(s, x - rc.l, y - rc.t, &hit);
      if (s->down) {
        if (s->adjusthittype & 10) {
          if (s->adjusthittype & 8) {
            s->colw[s->adjusthit.x] = s->adjustpt.x + x;
          }
          if (s->adjusthittype & 2) {
            s->rowh[s->adjusthit.y] = s->adjustpt.y + y;
            //printf("%d\n", s->rowh[HITGET(ahit.y)]);
          }
          force_redraw(c);
        }
        else if ((s->hittype & 5) && (hitty & 5)) {
          if (hitty & 4) {
            s->end.x = hit.x;
          }
          if (hitty & 1) {
            s->end.y = hit.y;
          }
          force_redraw(c);
        }
      }
      else {
        const char* icur[] = {0, IDC_SIZENS, IDC_SIZEWE, IDC_SIZEALL};
        int tt = ((hitty & 8) >> 2) | ((hitty & 2) >> 1);
        //printf("%d %d\n", hitty, tt);
        if (tt) {
          sys_setcursor(c->sys, icur[tt]);
        }
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      gridctrl_draw(s, im, &rcclip, rc);
    }
    break;
  }
  return 0;
}
CC_INLINE int gridctrl_set(gridctrl_t* s, ctrl_t* li, int row, int col)
{
  int i;
  ctrl_set(gridctrl_proc, s, s->c, li, WT_CHILD, 0);
  ++row, ++col;
  REALLOC(int, s->rowh, row);
  REALLOC(int, s->colw, col);
  s->rows = row;
  s->cols = col;
  for (i = 0; i < s->rows; ++i) {
    s->rowh[i] = 20;
  }
  for (i = 0; i < s->cols; ++i) {
    s->colw[i] = 40;
  }
  return 0;
}
static LRESULT dockctrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  dockctrl_t* s = (dockctrl_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    //ctrl_set(button_proc, s->di[0].ctrl, c, c->sys);
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 200;
    mminfo->ptMinTrackSize.y = 200;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (s->hit) {
      if (iPtInRect(&rc, x, y)) {
        //xSendCmd(XCMD_CLICK);
        force_redraw(c);
      }
      ReleaseCapture();
      s->hit = 0;
    }
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->hit = 1;
      ctrl_set_capture(c);
      ctrl_close(s->di[0].win);
      force_redraw(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_LBUTTONDBLCLK:
    if (iPtInRect(&rc, x, y)) {
      IRECT gprc = iRECT(rc.l, rc.t, rc.l + GRIPPER_W, rc.b);
      if (iPtInRect(&gprc, x, y)) {
        //win_setctrl(s->di[0].win, "dock", s->di[0].ctrl, 1, hwnd);
      }
    }
    break;
  case WM_CANCELMODE:
  case WM_MOUSEMOVE:
    if (s) {
      int hot = iPtInRect(&rc, x, y);
      if (s->hot != hot) {
        s->hot = hot;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      imdraw_fill(im, _GetSysColor(CLR_3DFACE));
      imdraw_gripper(im, &rcclip, rc, 2, 3, 1, 0);
    }
    break;
  }
  return 0;
}
#include "sys/clipboard.h"
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
int strdata_buf_get_line(strdata_t* s, int irow, int icol, char_color_t* out, int maxoutlen)
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
    for (; ichr++;) {
      chr_prev(str, ipos, &ipos);
    }
  }
  else {
    for (; ichr--;) {
      chr_next(str, ipos, &ipos);
    }
  }
  s->pos = str_pos2pt(str, ipos, "\n");
  return 0;
}
int editctrl_get_char_w(const editctrl_t* s)
{
  int char_w = s->c->fo->h / 2;
  return char_w;
}
int editctrl_get_line_h(const editctrl_t* s)
{
  int line_h = font_text_size(s->c->fo, "H", 1, 0).h;
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
  int xx = (x - s->c->rc.l - s->org.x) / char_w;
  int yy = (y - s->c->rc.t - s->org.y) / line_h;
  s->sd->set_pos(s->sd, xx, yy);
  return 0;
}
int editctrl_ins_str(editctrl_t* s, const char* str, int len, int to_del)
{
  if (to_del > 0 && PTTOINT(s->sd->beg) == PTTOINT(s->sd->pos)) {
    s->sd->move_pos(s->sd, to_del, 0);
  }
  s->sd->ins_str(s->sd, str, len);
  return 0;
}
//#include "editctrl1.inl"
#include "editctrl.inl"
//menuitem_t and LPMENUITEM
static int menuctrl_removeall(menuctrl_t* s)
{
  s->nitem = 0;
  s->items = NULL;
  return 0;
}
// logprintf
CC_INLINE int menuctrl_setitems(menuctrl_t* s, int n, menuitem_t* items)
{
  if (n > 0) {
    s->items = items;
    s->nitem = n;
  }
  return 0;
}
static ISIZE menuctrl_measureitem(const menuctrl_t* s, int i)
{
  ISIZE sztxt, szimg = {0, 16}, sz;
  int d = MENU_PENDING * 2;
  menuitem_t* mi = s->items + i;
  if (mi->text) {
    sztxt = font_text_size(s->c->fo, mi->text, -1, 0);
    if (!(s->ishor)) {
      szimg = imsize(s->m_img);
      szimg = iSIZE(MAX(szimg.w, 16), MAX(szimg.h, 16));
    }
    szimg.w += d, szimg.h += d;
    sz = iSIZE(sztxt.w + szimg.w + 8, MAX(sztxt.h, szimg.h));
    if (!(s->ishor)) {
      sz.w += sz.h;
    }
  }
  else {
    sz = iSIZE(6, 6);
  }
  return sz;
}
static ISIZE menuctrl_measure(const menuctrl_t* s)
{
  ISIZE sz = {0}, sz2;
  int i, ind = s->items[s->pos].ind;
  for (i = s->pos; i < s->nitem && s->items[i].ind >= ind; ++i) {
    if (s->items[i].ind > ind) {
      continue;
    }
    sz2 = menuctrl_measureitem(s, i);
    if (s->ishor && 0 == s->items[i].ind) {
      sz.h = MAX(sz.h, sz2.h);
      sz.w += sz2.w;
    }
    else {
      sz.w = MAX(sz.w, sz2.w);
      sz.h += sz2.h;
    }
  }
  sz.w += MENU_BDW * 2;
  sz.h += MENU_BDW * 2;
  return sz;
}
static int menuctrl_hittest(const menuctrl_t* s, int x0, int y0, IRECT* prcItem)
{
  int i, ind = s->items[s->pos].ind, x, y;
  ISIZE sz2;
  IRECT rcItem, rc = s->c->rc;
  iRectIn(&rc, MENU_BDW);
  x = rc.l, y = rc.t;
  for (i = s->pos; i < s->nitem && s->items[i].ind >= ind; ++i) {
    if (s->items[i].ind > ind) {
      continue;
    }
    sz2 = menuctrl_measureitem(s, i);
    if (s->ishor && 0 == s->items[i].ind) {
      rcItem = iRECT(x, rc.t, x + sz2.w, rc.b);
      x += sz2.w;
    }
    else {
      rcItem = iRECT(rc.l, y, rc.r, y + sz2.h);
      y += sz2.h;
    }
    if (iPtInRect(&rcItem, x0, y0)) {
      if (prcItem) {
        *prcItem = rcItem;
      }
      return i;
    }
  }
  return -1;
}
CC_INLINE int menuctrl_hideall(menuctrl_t* s)
{
  int i, ind = s->items[s->pos].ind, maxind = 0;
  s -= ind;
  for (i = 0; i < s->nitem; ++i) {
    maxind = MAX(maxind, s->items[i].ind);
  }
  s->open0 = 0;
  s->hit = -1;
  for (i = maxind; i; --i) {
    ctrl_close(s[i].c);
  }
  return 0;
}
CC_INLINE int menuctrl_hide(menuctrl_t* s, int pos)
{
  int i, ind = s->items[pos].ind, maxind = 0;
  for (i = 0; i < s->nitem; ++i) {
    maxind = MAX(maxind, s->items[i].ind);
  }
  for (i = maxind - ind; i; --i) {
    ctrl_close(s[i].c);
  }
  return 0;
}
static LRESULT menuctrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam);
CC_INLINE int menuctrl_set(menuctrl_t* s, ctrl_t* li, int nitem, menuitem_t* items, int pos)
{
  ctrl_set(menuctrl_proc, s, s->c, li, WT_MENU, 0);
  s->items = items, s->nitem = nitem, s->pos = pos;
  return 0;
}
static int menuctrl_popup(menuctrl_t* s, int hit, IRECT rcItem)
{
  menuctrl_hide(s, s->pos);
  if (hit < s->nitem && s->nitem > 0 && (s->items[hit].ind + 1) == s->items[hit + 1].ind) {
    //IPOINT pt = 0 == s->pos ? iPOINT(rcItem.l, rcItem.b) : iPOINT(rcItem.r, rcItem.t);
    menuctrl_set(s + 1, s->c->listener, s->nitem, s->items, hit + 1);
    //win_show(s[1].c, NULL, WT_MENU, pt.x, pt.y, 0, 0, 0);
  }
  return 0;
}
static LRESULT menuctrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  menuctrl_t* s = (menuctrl_t*)c->x;
  IRECT rc = c->rc, rcItem;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int hit = -1;
  //printf("sys_count %d\n", sys_count(c->sys));
  switch (uMsg) {
  case WM_CTRLINIT:
    s->ishor = 1;
    c->isshow = SW_SHOWNOACTIVATE;
    s->hit = s->hot = -1;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    if (0 == s->pos) {
      FREE(s->items);
    }
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = menuctrl_measure(s);
    mminfo->ptMinTrackSize.x = sz.w;
    mminfo->ptMinTrackSize.y = sz.h;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_KILLFOCUS:
    if (0 == s->pos) {
      //if (GetParent(hwnd)!=GetFocus())
      //SendMessage(hwnd, WM_CLOSE, 0, 0);
      s->hit = s->hot = -1;
      s->open0 = 0;
      menuctrl_hideall(s);
      force_redraw(c);
    }
    break;
  case WM_LBUTTONUP:
    if (s->hit >= 0) {
      if (iPtInRect(&rc, x, y)) {
        hit = menuctrl_hittest(s, x, y, &rcItem);
        //xSendCmd(XCMD_CLICK);
        {
          menuctrl_t* s0 = s - s->items[s->pos].ind;
          sendmsg(s0->c, s0->c->listener, WM_COMMAND, hit, (LPARAM)c);
        }
        force_redraw(c);
        if (s->pos > 0) {
          menuctrl_hideall(s);
          s->hit = -1;
          s->open0 = 0;
        }
      }
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_NCLBUTTONDOWN: {
    //IPOINT pt = MyScreenToClient(hwnd, x, y);
    //x = pt.x, y = pt.y;
  }
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      //printf("%x menu WM_LBUTTONDOWN %d\n", s, s->pos);
      hit = menuctrl_hittest(s, x, y, &rcItem);
      if (s->pos == 0) {
        s->open0 = !s->open0;
        if (!s->open0) {
          menuctrl_hideall(s);
          s->hit = -1;
        }
        else {
          if (hit != s->hit) {
            s->hit = hit;
            menuctrl_popup(s, hit, rcItem);
            //printf("menuctrl_hide11\n");
          }
        }
      }
      else {
        if (hit != s->hit) {
          s->hit = hit;
          menuctrl_popup(s, hit, rcItem);
        }
      }
      force_redraw(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      hit = menuctrl_hittest(s, x, y, &rcItem);
      if (s->hot != hit) {
        if (hit >= 0 && s->hit != hit && (s->pos > 0 || s->open0)) {
          menuctrl_popup(s, hit, rcItem);
          s->hit = hit;
        }
        //printf("WM_MOUSEMOVE %08x\n", s);
        s->hot = hit;
        force_redraw(c);
      }
    }
    else if (s->hot >= 0) {
      s->hot = -1;
      force_redraw(c);
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      //menuctrl_draw(s, rc, im);
      //int menuctrl_draw(menuctrl_t* s, IRECT rc, img_t* im)
      COLOR m_crBackground = _GetSysColor(CLR_MENU);
      COLOR m_crText = _GetSysColor(CLR_MENUTEXT);
      COLOR m_crTextSelected = _RGB(255, 255, 255);
      COLOR m_crLeft = _RGB(236, 233, 216);
      COLOR m_crSelectedBroder = _RGB(49, 106, 197);
      COLOR m_crSelectedFill = _RGB(200, 220, 253);
      COLOR m_crSelectedFill1 = _GetSysColor(CLR_ACTIVECAPTION);
      IRECT rcclip = c->rcclip;
      IRECT rc = c->rc;
      iRectOff0(&rc);
      iRectOff0(&rcclip);
      //printf("menuctrl WM_PAINT pos = %d\n", s->pos);
      if (0 == s->pos) {
        imdraw_fillrect(im, &rcclip, rc, m_crBackground);
      }
      else {
        imdraw_border_syscolor(im, &rcclip, rc, BDS_HIGH, BDM_ALL);
      }
      {
        int i, ind = s->items[s->pos].ind, x, y;
        ISIZE sz2;
        IRECT rcItem;
        iRectIn(&rc, MENU_BDW);
        x = rc.l, y = rc.t;
        for (i = s->pos; i < s->nitem && s->items[i].ind >= ind; ++i) {
          if (s->items[i].ind > ind) {
            continue;
          }
          sz2 = menuctrl_measureitem(s, i);
          if (s->ishor && 0 == s->items[i].ind) {
            rcItem = iRECT(x, rc.t, x + sz2.w, rc.b);
            x += sz2.w;
          }
          else {
            rcItem = iRECT(rc.l, y, rc.r, y + sz2.h);
            y += sz2.h;
          }
          {
            //int isselect = i == s->select, ishot = i == s->hot;
            const char* text = s->items[i].text;
            IRECT rc1 = rcItem;
            COLOR crArow = m_crText, crText = m_crText;
            if (0 == s->items[i].ind) {
              if (i == s->hit) {
                imdraw_border_syscolor(im, &rcclip, rc1, BDS_LOW, BDM_ALL);
              }
              else if (i == s->hot) {
                imdraw_border_syscolor(im, &rcclip, rc1, BDS_UP, BDM_ALL);
              }
            }
            else {
              if (i == s->hit) {
                imdraw_fillrect(im, &rcclip, rc1, m_crSelectedFill1);
                crText = m_crTextSelected;
                crArow = _RGB(240, 240, 240);
              }
              else if (i == s->hot) {
                imdraw_fillrect(im, &rcclip, rc1, m_crSelectedFill1);
                crText = m_crTextSelected;
                crArow = _RGB(240, 240, 240);
              }
              if (i < (s->nitem - 1) && s->items[i + 1].ind > ind) {
                IRECT rcArow;
                iRectCutL(&rcItem, -RCH(&rcItem), 0, &rcArow);
                imdraw_menuimg(im, &rcclip, rcArow, IdArowRightLarge, crArow);
              }
            }
            rcItem.l += 4;
            imdraw_text(im, &rcclip, rcItem, text, -1, c->fo, crText, 0, DT_VCENTER);
          }
        }
      }
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
#define listbox_getcursel(c) ((int)(DWORD)sendmsg((0), (c), sys->hwnd, LB_GETCURSEL, 0L, 0L))
#define listbox_setcursel(c, index) ((int)(DWORD)sendmsg((0), (c), LB_SETCURSEL, (WPARAM)(int)(index), 0L))
static int listbox_set_item_count(listbox_t* s, int nitem)
{
  s->nitem = nitem;
  s->c->redraw = 1;
  s->c->recalcsize = 1;
  return 0;
}
static int listbox_hittest(listbox_t* s, int x, int y)
{
  IRECT rcItem, rc = s->sc->rcc;
  int i, y1 = rc.t - s->sc->si[1].nPos;
  for (i = 0; i < s->nitem; ++i) {
    ISIZE ms[1] = {0};
    sendmsg(s->c, s->c->listener, WM_MEASUREITEM, i, (LPARAM)ms);
    rcItem = iRECT(rc.l, y1, rc.r, y1 + ms->h);
    if (iPtInRect(&rcItem, x, y)) {
      return i;
    }
    y1 += ms->h;
  }
  return -1;
}
static ISIZE listbox_calc_size(listbox_t* s)
{
  int i;
  ISIZE sz = {0};
  for (i = 0; i < s->nitem; ++i) {
    ISIZE ms[1] = {0};
    sendmsg(s->c, s->c->listener, WM_MEASUREITEM, i, (LPARAM)ms);
    sz.h += ms->h;
    sz.w = MAX(sz.w, ms->w);
  }
  return sz;
}
static LRESULT listbox_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  listbox_t* s = (listbox_t*)c->x;
  IRECT rcc;//, rc1 = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int ret = 0, hit = -1;
  rcc = s->sc->rcc;
  switch (uMsg) {
  case WM_CTRLINIT:
    scrollbar_set(s->sc, c, c);
    ctrl_addsub_head(c, s->sc->c);
    s->sc->styles = WS_VSCROLL;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case LB_GETCURSEL:
    return s->cursel < s->nitem ? s->cursel : -1;
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
      //printf("WM_VSCROLL \n");
      force_redraw(c);
    }
    else {
      SCROLLINFO* si = s->sc->si + (uMsg == WM_VSCROLL);
      int page = (int)si->nPage;
      int line = MAX(1, page / 8);
      switch (LOWORD(lParam)) {
      case SB_LINEUP:
        scrollbar_set_pos(si, si->nPos - line);
        break;
      case SB_LINEDOWN:
        scrollbar_set_pos(si, si->nPos + line);
        break;
      case SB_PAGEUP:
        scrollbar_set_pos(si, si->nPos - page);
        break;
      case SB_PAGEDOWN:
        scrollbar_set_pos(si, si->nPos + page);
        break;
      case SB_TOP:
        scrollbar_set_pos(si, 0);
        break;
      case SB_BOTTOM:
        scrollbar_set_pos(si, si->nMax);
        break;
      }
    }
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = listbox_calc_size(s);
    s->sc->c->ctrlproc(c, s->sc->c, uMsg, wParam, lParam);
    if (!(s->sc->styles & WS_HSCROLL)) {
      mminfo->ptMinTrackSize.x = sz.w;
    }
    if (!(s->sc->styles & WS_VSCROLL)) {
      mminfo->ptMinTrackSize.y = sz.h;
    }
  }
  break;
  case WM_MOUSEMOVE:
    if (1) {
      hit = listbox_hittest(s, x, y);
      if (hit != s->hot) {
        s->hot = hit;
        force_redraw(c);
      }
    }
    break;
  case WM_MOUSEWHEEL:
    if (iPtInRect(&rcc, x, y)) {
      //s->org.y += GET_WHEEL_DELTA_WPARAM(wParam) / 3;
      SCROLLINFO* p = &s->sc->si[1];
      scrollbar_set_pos(p, p->nPos - (GET_WHEEL_DELTA_WPARAM(wParam)));
      force_redraw(c);
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rcc, x, y)) {
      //UISetFocus(s);
      s->ldown = TRUE;
      s->rdown = 0;
      // 窗体获得焦点以后设置光标的位置
      // 从当前鼠标的位置获得对应字符的序号
      hit = listbox_hittest(s, x, y);
      if (GetKeyState(VK_SHIFT) >= 0) {
      }
      s->hot = hit;
      ctrl_setfocus(c);
      return 1;
      //listbox_update_org(s);
    }
    break;
  case WM_LBUTTONUP:
    if (s->ldown && iPtInRect(&rcc, x, y)) {
      hit = listbox_hittest(s, x, y);
      s->ldown = 0;
      if (hit != s->cursel) {
        s->cursel = hit;
        force_redraw(c);
      }
      sendmsg(c, c->listener, WM_COMMAND, LBN_SELCHANGE, hit);
    }
    //force_redraw(c);
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      if (1) {
        int i;
        int allheight = 0;
        for (i = 0; i < s->nitem; ++i) {
          ISIZE ms[1] = {0};
          sendmsg(c, c->listener, WM_MEASUREITEM, i, (LPARAM)ms);
          allheight += ms->h;
        }
        scrollbar_set_info(s->sc->si + 1, 0, allheight, RCH(&c->rc));
      }
      else {
        scrollbar_set_info(s->sc->si + 1, 0, s->nitem, 2);
      }
      ctrl_setrect(s->sc->c, c->rc);
      //force_redraw(c);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = s->sc->rcc;
      IRECT rcclip = c->rcclip;
      int i, y1;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      y1 = rc.t - s->sc->si[1].nPos;
      imdraw_rect(im, 0, rc, _rgb(1, 1, 1), 0, 0, 0);
      for (i = 0; i < s->nitem; ++i) {
        ISIZE ms[1] = {0};
        drawitem_t ds[1] = {0};
        sendmsg(c, c->listener, WM_MEASUREITEM, i, (LPARAM)ms);
        ds->itemrc = iRECT(rc.l, y1, rc.r, y1 + ms->h);
        y1 += ms->h;
        if (ds->itemrc.t > rc.b) {
          break;
        }
        if (ds->itemrc.b < rc.t) {
          continue;
        }
        ds->im = im;
        if (i == s->cursel) {
          ds->state |= ODS_SELECTED;
        }
        if (i == s->hot) {
          ds->state |= ODS_HOTLIGHT;
        }
        ds->cliprc = &rc;
        sendmsg(c, c->listener, WM_DRAWITEM, i, (LPARAM)ds);
      }
      s->sc->c->redraw = 1;
      s->sc->c->ctrlproc(c, s->sc->c, uMsg, wParam, lParam);
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
CC_INLINE int listbox_set(listbox_t* s, ctrl_t* li)
{
  ctrl_set(listbox_proc, s, s->c, li, WT_CHILD, 0);
  return 0;
}
//#define XDRAW_BEG(im) {img_t im2[1] = {0}; imsubref(im, rc, im2); rc = iRECT(0, 0, im2->w, im2->h); im = im2;
//#define XDRAW_END() }
static int rbox_ctrl_get_row_height(rbox_t* s)
{
  ISIZE sz = font_text_size(s->c->fo, "H", -1, 0);
  return MAX(20, sz.h + 2);
}
static int rbox_ctrl_hittest(rbox_t* s, int x, int y)
{
  int i;
  double dy = rbox_ctrl_get_row_height(s);
  double xp = dy / 1.3;
  for (i = 0; i < s->nitem; i++) {
    if (calc_distance(xp, y, xp, dy * (i + 0.5)) <= 9) {
      return i;
    }
  }
  return -1;
}
CC_INLINE LRESULT rbox_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  rbox_t* s = (rbox_t*)c->x;
  IRECT rc = c->rc;
  int i, hit, x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    int w = 0, h = rbox_ctrl_get_row_height(s);
    for (i = 0; i < s->nitem; ++i) {
      ISIZE sz2 = font_text_size(c->fo, s->m_items[i], -1, 0);
      w = MAX(w, sz2.w);
    }
    mminfo->ptMinTrackSize.x = w + h + 4;
    mminfo->ptMinTrackSize.y = h * s->nitem + 2;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_LBUTTONUP:
    if (iPtInRect(&rc, x, y)) {
      hit = rbox_ctrl_hittest(s, x - rc.l, y - rc.t);
      if (hit >= 0 && hit == s->hot) {
        s->hit = hit;
        //XMSGINIT(msg, WM_COMMAND, 0);
        sendmsg(c, c->listener, WM_COMMAND, XCMD_CHANGE, (WPARAM)c);
        force_redraw(c);
        //force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      hit = rbox_ctrl_hittest(s, x - rc.l, y - rc.t);
      s->hot = hit;
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i;
      const font_t* fo = c->fo;
      int m_text_height = fo->h;
      int dy = rbox_ctrl_get_row_height(s);
      double m_text_thickness = (1.);
      COLOR m_text_color = (_rgb(0.0, 0.0, 0.0));
      COLOR m_inactive_color = (_rgb(0.0, 0.0, 0.0));
      COLOR m_active_color = (_rgb(0.4, 0.0, 0.0));
      COLOR m_background_color = (_rgb(1.0, 1.0, 0.9));
      COLOR m_border_color = (_rgb(0.0, 0.0, 0.0));
      IRECT rcItem, rcText, rc2 = rc;
      double r1 = m_text_height * 0.5; // 外圈半径
      double r2 = r1 * 0.4; // 内圆半径
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      // Text
      {
        if (1) {
          m_active_color = Color_Black;
          imdraw_border_syscolor(im, &rcclip, rc, BDS_ETCHED, BDM_ALL);
        }
        else if (1) {
          imdraw_rect(im, &rcclip, rc, m_background_color, m_border_color, 1, BDM_ALL);
        }
      }
      for (i = 0; i < s->nitem; ++i) {
        if (iRectCutT(&rc2, dy, 0, &rcText)) {
          const char* str = s->m_items[i];
          int k = iRectCutL(&rcText, dy, 0, &rcItem);
          DPOINT pt1 = RCCENTERF(&rcItem);
          // Inactive items
          imdrawaa_circle(im, &rcclip, pt1.x, pt1.y, r1, Color_White, m_text_color, m_text_thickness);
          if (s->hit == i) {
            imdrawaa_circle(im, &rcclip, pt1.x, pt1.y, r2, m_active_color, 0, 0);
          }
          //draw_textR(im, str.s, str.l, &rcText, DT_VCENTER, m_text_color);
          imdraw_text(im, &rcclip, rcText, str, -1, c->fo, m_text_color, 0, DT_VCENTER);
        }
      }
      c->redraw = 0;
    }
    break;
  }
  return FALSE;
}
CC_INLINE int rbox_set(rbox_t* s, ctrl_t* li, int n, const char** items)
{
  ctrl_set(rbox_proc, s, s->c, li, WT_CHILD, 0);
  s->nitem = n, s->m_items = items;
  return 0;
}
static int cbox_set(cbox_t* s, const char* text)
{
  s->text = text;
  return 0;
}
static LRESULT cbox_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  cbox_t* s = (cbox_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    s->hit = 0;
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = font_text_size(c->fo, s->text, -1, 0);
    mminfo->ptMinTrackSize.x = sz.w + 2;
    mminfo->ptMinTrackSize.y = sz.h + 2;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->hit = !s->hit;
      force_redraw(c);
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int m_text_height = font_text_size(c->fo, "H", 1, 0).h + 2;
      COLOR m_text_color = (_rgb(0.0, 0.0, 0.0));
      COLOR m_inactive_color = (_rgb(0.0, 0.0, 0.0));
      COLOR m_active_color = (_rgb(0.4, 0.0, 0.0));
      IRECT rcCheck, rect;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      {
        if (1) {
          int m_bIsDrawOnGlass = 0;
          BOOL bEnabled = 1;
          int bHighlighted = s->hit;
          rect = rc;
          iRectCutL(&rc, RCH(&rc), 2, &rcCheck);
          iRectAlign(rcCheck, m_text_height, m_text_height, DT_CENTER | DT_VCENTER, &rcCheck);
          if (m_bIsDrawOnGlass) {
            RCDEFLATE(&rect, 1, 1);
            imdraw_border_syscolor(im, &rcclip, rect, BDS_SUNKEN, BDM_ALL);
            if (1 == s->hit) {
              imdraw_menuimg(im, &rcclip, rect, IdCheck, Color_Black);
            }
          }
          else {
            if (bHighlighted) {
              //TDrawFocusRect(hDC, rect);
            }
            RCDEFLATE(&rect, 1, 1);
            //imdraw_rect(im, &rect, bEnabled ? res->clrWindow : res->clrBarFace);
            imdraw_border_syscolor_impl(im, &rcclip, rcCheck, BDS_SUNKEN, BDM_ALL, Color_White);
            if (1 == s->hit) {
              imdraw_menuimg(im, &rcclip, rcCheck, IdCheck, Color_Black);
            }
            else if (2 == s->hit) {
              uchar HatchBits[8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };
              RCDEFLATE(&rect, 1, 1);
              imdraw_bit(im, &rcclip, rcCheck, DT_CENTER | DT_VCENTER, 8, 8, HatchBits, 1, 0, 0, 1, _RGB(0, 0, 0));
            }
            imdraw_text(im, &rcclip, rc, s->text, -1, c->fo, m_text_color, 0, DT_VCENTER);
          }
        }
        else if (1) {
          //XDRAW_BEG(im);
          iRectCutL(&rc, im->h, 2, &rcCheck);
          iRectAlign(rcCheck, m_text_height, m_text_height, DT_CENTER | DT_VCENTER, &rcCheck);
          imdraw_rect(im, &rcclip, rcCheck, Color_White, m_text_color, 2, BDM_ALL);
          if (1 == s->hit) {
            double rad[] = {m_text_height / 8., m_text_height / 2.};
            imdrawaa_star(im, &rcclip, RCX(&rcCheck) / 2., RCY(&rcCheck) / 2., rad, 2, 4, m_active_color, 0, 0);
          }
          imdraw_text(im, &rcclip, rc, s->text, -1, c->fo, m_text_color, 0, DT_VCENTER);
          //XDRAW_END();
        }
        break;
      }
    }
    c->redraw = 0;
    break;
  }
  return FALSE;
}
CC_INLINE int combobox_getcount(const combobox_t* s)
{
  return s->nitem;
}
CC_INLINE int combobox_getcursel(const combobox_t* s)
{
  return s->m_listbox->cursel;
}
CC_INLINE int combobox_setcursel(combobox_t* s, int nSelect)
{
  return s->m_listbox->cursel = nSelect;
}
static LRESULT combobox_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  combobox_t* s = (combobox_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  enum {margin_x = 4};
  switch (uMsg) {
  case WM_CTRLINIT:
    listbox_set(s->m_listbox, c);
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = font_text_size(c->fo, "H", -1, 0);
    mminfo->ptMinTrackSize.x = sz.w + 4 + margin_x;
    mminfo->ptMinTrackSize.y = sz.h + 4;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_MEASUREITEM:
    if (sender == s->m_listbox->c) {
      ISIZE* ds = (ISIZE*)lParam;
      ISIZE sz = font_text_size(c->fo, "H", -1, 0);
      ds->h = sz.h + 4;
      ds->w = RCW(&rc);
    }
    break;
  case WM_DRAWITEM:
    if (sender == s->m_listbox->c) {
      drawitem_t* ds = (drawitem_t*)lParam;
      COLOR clrBk = _rgb(1, 1, 1);
      COLOR clrText = _rgb(0, 0, 0);
      int k = wParam;
      IRECT rcTxt = ds->itemrc;
      rcTxt.l += margin_x;
      if (ds->state & ODS_HOTLIGHT) {
        clrBk = _RGBA(0, 0, 255, 150);
        clrText = _rgb(1, 1, 1);
      }
      if (ds->state & ODS_SELECTED) {
        clrBk = _RGB(0, 0, 255);
        clrText = _rgb(1, 1, 1);
      }
      imdraw_rect(ds->im, ds->cliprc, ds->itemrc, clrBk, 0, 0, 0);
      imdraw_text(ds->im, ds->cliprc, rcTxt, s->m_items[k], -1, c->fo, clrText, 0, DT_VCENTER);
    }
    break;
  case WM_COMMAND:
    if (LBN_SELCHANGE == wParam && sender == s->m_listbox->c) {
      s->hot = 0;
      s->hit = 0;
      ctrl_close(s->m_listbox->c);
      force_redraw(c);
    }
    break;
  case WM_KILLFOCUS:
    if (1) {
      s->hot = 0;
      s->hit = 0;
      ctrl_close(s->m_listbox->c);
      force_redraw(c);
    }
    break;
  case WM_NCLBUTTONDOWN:
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->hot = 1;
      if (!s->hit) {
        //ISIZE sz = listbox_calc_size(s->m_listbox);
        x = rc.l, y = rc.b;
        //myClientToScreen(hwnd, &x, &y);
        listbox_set(s->m_listbox, c);
        //s->m_listbox->sc->styles = 0;
        //win_show(s->m_listbox->c, 0, WT_MENU, x, y, RCW(&rc), MIN(sz.h, 500), 0);
      }
      else {
        ctrl_close(s->m_listbox->c);
      }
      s->hit = !s->hit;
      force_redraw(c);
      ctrl_setfocus(s->m_listbox->c);
      return 1;
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      COLOR clrText = _rgb(0, 0, 0);
      IRECT rcTxt, rcDown;
      int pushed = (c->sys->ldown && s->hit);
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      iRectCutR(&rc, -RCH(&rc), 0, &rcDown);
      rcTxt = rc;
      rcTxt.l += margin_x;
      imdraw_rect(im, &rcclip, rc, _rgb(1, 1, 1), _rgb(0, 0, 0), 0, 0);
      imdraw_border_syscolor(im, &rcclip, rcDown, pushed ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
      rcDown.l += 2 * pushed - 1;
      imdraw_menuimg(im, &rcclip, rcDown, IdArowDownLarge, _rgb(0, 0, 0));
      if (s->m_listbox->cursel >= 0 && s->m_listbox->cursel < s->nitem) {
        imdraw_text(im, &rcclip, rcTxt, s->m_items[s->m_listbox->cursel], -1, c->fo, clrText, 0, DT_VCENTER);
        //printf("%d %s\n", s->m_listbox->cursel, s->m_items[s->m_listbox->cursel]);
      }
    }
    c->redraw = 0;
    break;
  }
  return FALSE;
}
CC_INLINE int combobox_set(combobox_t* s, ctrl_t* li, int n, const char** items)
{
  ctrl_set(combobox_proc, s, s->c, li, WT_CHILD, 0);
  s->nitem = n, s->m_items = items;
  listbox_set_item_count(s->m_listbox, n);
  return 0;
}
static int polygon_set(polygon_t* s, BOOL m_close, int m_hitmark)
{
  s->hit = -1;
  s->m_close = m_close;
  s->m_hitmark = m_close;
  //s->m_hitmark = POHIT_POINT|POHIT_EDGE|POHIT_FACE;
  s->m_hitmark = m_hitmark;
  return 0;
}
static int dPOLYGON_hittest(const DPOLYGON* s, double x, double y, double m_point_radius, int m_hitmark)
{
  int i, npt, ret;
  const DPOINT* pt = s->pt;
  for (i = 0; i < s->n; ++i) {
    npt = s->len[i];
    if (ret = point_in_polygon_hittest(pt, npt, x, y, m_point_radius, m_hitmark)) {
      return ret;
    }
    pt += npt;
  }
  return 0;
}
static int polygon_ctrl_hittest(polygon_t* s, double x, double y)
{
  return dPOLYGON_hittest(s->m_polygon, x, y, 5, s->m_hitmark);
}
static int imdrawaa_polygon_ctrl(img_t* im, const IRECT* pclip, int n, const DPOINT* pt, const font_t* fnt)
{
  int i;
  char buf[256];
  for (i = 0; i < n; ++i) {
    int x = (int)pt[i].x, y = (int)pt[i].y;
    IRECT rc1 = iRECT(x, y, x + 30, y + 20);
    _snprintf(buf, 256, "%d", i);
    imdraw_text(im, pclip, rc1, buf, -1, fnt, Color_Black, 0, DT_VCENTER);
  }
  return 0;
}
static int imdrawaa_dPOLYGON_solid(img_t* im, const IRECT* pclip, const DPOLYGON* s, COLOR crFill, COLOR crLine, double wline)
{
  imdrawaa_polygon(im, pclip, s->pt, s->len, s->n, crFill, crLine, wline);
  return 0;
}
static int imdrawui_poly2d(img_t* im, const IRECT* pclip, const DPOLYGON* s, int closed, const font_t* fnt, int hit)
{
  int i, j;
  char buf[256];
  COLOR m_color = (_rgba(0.0, 0.0, 0.0, 0.8));
  COLOR m_node_color = (_rgba(0.0, 0.5, 0.0, 0.5));
  COLOR m_text_color = (_rgba(0.0, 0.0, 0.0, 1));
  double m_stroke_width = 1;
  double m_point_radius = 5;
  const DPOINT* pt = s->pt;
  for (j = 0; j < s->n; ++j) {
    int npt = s->len[j];
    imdrawaa_poly(im, pclip, pt, npt, closed, 0, m_color, m_stroke_width);
    for (i = 0; i < npt; ++i) {
      double r = i == (hit & POHIT_MARK) ? m_point_radius * 1.2 : m_point_radius;
      imdrawaa_ellipse(im, pclip, pt[i].x, pt[i].y, r, r, m_node_color, 0, 0);
      if (1) {
        int x = (int)pt[i].x, y = (int)pt[i].y;
        _snprintf(buf, 256, "%d", i);
        imdraw_text(im, pclip, iRECT(x, y, x + 30, y + 20), buf, -1, fnt, m_text_color, 0, DT_VCENTER);
      }
    }
    pt += npt;
  }
  return 0;
}
static LRESULT polygon_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  polygon_t* s = (polygon_t*)c->x;
  IRECT rc = c->rc;
  int hit, x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    s->hit = 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 200;
    mminfo->ptMinTrackSize.y = 200;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (1) {
      s->hit = 0;
    }
    break;
  case WM_LBUTTONDOWN:
    if (s) {
      hit = polygon_ctrl_hittest(s, x, y);
      if (hit != s->hit) {
        s->hit = hit;
        s->m_pt.x = x, s->m_pt.y = y;
        force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONDBLCLK:
    if (s) {
      hit = polygon_ctrl_hittest(s, x, y);
      if (hit & POHIT_POINT) {
        int i = hit & POHIT_MARK;
        dPOLYGON_delpt(s->m_polygon, i);
        force_redraw(c);
      }
      else {
        int j = dPOLYGON_line_nearest(s->m_polygon, x, y, 20);
        if (j >= 0) {
          dPOLYGON_addpt(s->m_polygon, j, x, y);
          force_redraw(c);
        }
      }
    }
    break;
  case WM_MOUSEMOVE:
    if (s->hit) {
      if (iPtInRect(&rc, x, y)) {
        int i = s->hit & POHIT_MARK;
        double dx, dy;
        DPOLYGON* po = s->m_polygon;
        dx = x - s->m_pt.x;
        dy = y - s->m_pt.y;
        if (s->hit & POHIT_POINT) {
          i = s->hit;
          po->pt[i].x += dx;
          po->pt[i].y += dy;
        }
        else if (s->hit & POHIT_FACE) {
          int npt = dPOLYGON_npt(po);
          for (i = 0; i < npt; i++) {
            po->pt[i].x += dx;
            po->pt[i].y += dy;
          }
        }
        else if (s->hit & POHIT_EDGE) {
          int n1 = i;
          int npt = dPOLYGON_npt(po);
          int n2 = NEXT_INDEX(i, npt);
          po->pt[n1].x += dx;
          po->pt[n1].y += dy;
          po->pt[n2].x += dx;
          po->pt[n2].y += dy;
        }
        s->m_pt.x = x, s->m_pt.y = y;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      imdraw_rect(im, &rcclip, rc, _rgb(1, 1, 1), Color_Black, 1, BDM_ALL);
      imdrawui_poly2d(im, &rcclip, s->m_polygon, s->m_close, c->fo, s->hit);
    }
    c->redraw = 0;
    break;
  }
  return 0;
}
static double spline_calc_xp(const spline_t* s, int idx)
{
  return s->m_xs1 + (s->m_xs2 - s->m_xs1) * s->m_xp[idx];
}
static double spline_calc_yp(const spline_t* s, int idx)
{
  return s->m_ys1 + (s->m_ys2 - s->m_ys1) * s->m_yp[idx];
}
static int spline_getys(const spline_t* s, int n, double* m_values)
{
  int i;
  double m_am[32], dn = n - 1;
  bspline_set(s->m_num_pnt, s->m_xp, s->m_yp, m_am);
  for (i = 0; i < n; i++) {
    m_values[i] = bspline_get(s->m_num_pnt, s->m_xp, s->m_yp, m_am, i / dn);
    m_values[i] = BOUND(m_values[i], 0, 1.);
  }
  return 0;
}
static int spline_update(spline_t* s)
{
  int i;
  for (i = 0; i < s->m_num_pnt; i++) {
    s->m_xp[i] = BOUND(s->m_xp[i], 0, 1);
    s->m_yp[i] = BOUND(s->m_yp[i], 0, 1);
    if (i == 0) {
      s->m_xp[i] = 0.0;
    }
    else if (i == s->m_num_pnt - 1) {
      s->m_xp[i] = 1.0;
    }
    else {
      if (s->m_xp[i] < s->m_xp[i - 1] + 0.001) {
        s->m_xp[i] = s->m_xp[i - 1] + 0.001;
      }
      if (s->m_xp[i] > s->m_xp[i + 1] - 0.001) {
        s->m_xp[i] = s->m_xp[i + 1] - 0.001;
      }
    }
  }
  spline_getys(s, 256, s->m_values);
  return 0;
}
static int spline_gety1(const spline_t* s, double x)
{
  //double ret = bspline_get(s->m_num_pnt, s->m_xp, s->m_yp, s->m_am, x);
  //ret = BOUND(ret, 0, 1.);
  return 0;
}
static int spline_set(spline_t* s, int num_pnt, const double* pt)
{
  int i;
  s->hit = 0;
  s->m_num_pnt = (num_pnt);
  s->m_vertex = (0);
  s->m_active_pnt = (-1);
  s->m_move_pnt = (-1);
  s->m_pdx = (0.0);
  s->m_pdy = (0.0);
  if (s->m_num_pnt < 4) {
    s->m_num_pnt = 4;
  }
  if (s->m_num_pnt > 32) {
    s->m_num_pnt = 32;
  }
  for (i = 0; i < s->m_num_pnt; i++) {
    s->m_xp[i] = i * 1. / (s->m_num_pnt - 1);
    s->m_yp[i] = pt[i];
  }
  for (i = 0; i < s->m_num_pnt; i++) {
    //s->m_xp[i] = pt[i * 2 + 0];
    //s->m_yp[i] = pt[i * 2 + 1];
  }
  spline_update(s);
  return 0;
}
static LRESULT spline_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  spline_t* s = (spline_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int i;
  double bw, be, bb;
  int m_border_width = 1;
  int m_border_extra = 2;
  int m_curve_width = 1;
  double m_point_size = 3;
  bw = m_border_width;
  be = m_border_extra;
  bb = bw + be;
  s->m_xs1 = bb;
  s->m_ys1 = bb;
  s->m_xs2 = RCW(&rc) - bb;
  s->m_ys2 = RCH(&rc) - bb;
  switch (uMsg) {
  case WM_CTRLINIT:
    return 0;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 20;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      for (i = 0; i < s->m_num_pnt; i++) {
        double xp = spline_calc_xp(s, i);
        double yp = spline_calc_yp(s, i);
        if (calc_distance(x - rc.l, y - rc.t, xp, yp) <= m_point_size + 1) {
          s->m_pdx = xp - x;
          s->m_pdy = yp - y;
          s->m_active_pnt = s->m_move_pnt = (i);
          force_redraw(c);
        }
      }
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
      s->hit = 0;
      if (s->m_move_pnt >= 0) {
        s->m_move_pnt = -1;
        force_redraw(c);
      }
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      if (s->m_move_pnt >= 0) {
        double xp = x + s->m_pdx;
        double yp = y + s->m_pdy;
        s->m_xp[s->m_move_pnt] = (s->m_move_pnt, (xp - s->m_xs1) / (s->m_xs2 - s->m_xs1));
        s->m_yp[s->m_move_pnt] = (s->m_move_pnt, (yp - s->m_ys1) / (s->m_ys2 - s->m_ys1));
        spline_update(s);
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i, n;
      double be;
      DPOINT pt[256];
      int m_border_width = 1;
      int m_border_extra = 2;
      int m_curve_width = 1;
      double m_point_size = 3;
      COLOR m_background_color = (_rgb(1.0, 1.0, 0.9));
      COLOR m_border_color = (_rgb(0.0, 0.0, 0.0));
      COLOR m_curve_color = (_rgb(0.0, 0.0, 0.0));
      COLOR m_inactive_pnt_color = (_rgb(0.0, 0.0, 0.0));
      COLOR m_active_pnt_color = (_rgb(1.0, 0.0, 0.0));
      double x, y;
      COLOR clr;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //XDRAW_BEG(im);
      be = m_border_extra;
      // Background Border
      imdraw_rect(im, &rcclip, rc, m_background_color, m_border_color, m_border_width, BDM_ALL);
      // Curve
      n = 0;
      pt[n++] = dPOINT(s->m_xs1, s->m_ys1 + (s->m_ys2 - s->m_ys1) * s->m_values[0]);
      for (i = 1; i < 256; i++) {
        x = (i) / 255.0, y = s->m_values[i];
        pt[n++] = dPOINT(s->m_xs1 + (s->m_xs2 - s->m_xs1) * x,
            s->m_ys1 + (s->m_ys2 - s->m_ys1) * y);
      }
      imdrawaa_poly(im, &rcclip, pt, n, 0, 0, m_curve_color, m_curve_width);
      // Inactive points
      for (i = 0; i < s->m_num_pnt; i++) {
        // Active point
        x = spline_calc_xp(s, i), y = spline_calc_yp(s, i);
        clr = i != s->m_active_pnt ? m_inactive_pnt_color : m_active_pnt_color;
        imdrawaa_ellipse(im, &rcclip, x, y, m_point_size, m_point_size, clr, 0, 0);
      }
      //XDRAW_END();
    }
    c->redraw = 0;
    break;
  }
  return FALSE;
}
static int gamma_calc_points(gamma_t* s)
{
  double kx1, ky1, kx2, ky2;
  gamma_spline_get_values(s->m_gamma_spline, &kx1, &ky1, &kx2, &ky2);
  s->m_p1.x = s->m_xs1 + (s->m_xs2 - s->m_xs1) * kx1 * 0.25;
  s->m_p1.y = s->m_ys2 - (s->m_ys2 - s->m_ys1) * ky1 * 0.25;
  s->m_p2.x = s->m_xs2 - (s->m_xs2 - s->m_xs1) * kx2 * 0.25;
  s->m_p2.y = s->m_ys1 + (s->m_ys2 - s->m_ys1) * ky2 * 0.25;
  return 0;
}
static int gamma_calc_values(gamma_t* s)
{
  double kx1, ky1, kx2, ky2;
  kx1 = (s->m_p1.x - s->m_xs1) * 4.0 / (s->m_xs2 - s->m_xs1);
  ky1 = (s->m_ys2 - s->m_p1.y) * 4.0 / (s->m_ys2 - s->m_ys1);
  kx2 = (s->m_xs2 - s->m_p2.x) * 4.0 / (s->m_xs2 - s->m_xs1);
  ky2 = (s->m_p2.y - s->m_ys1) * 4.0 / (s->m_ys2 - s->m_ys1);
  gamma_spline_set_values(s->m_gamma_spline, kx1, ky1, kx2, ky2);
  return 0;
}
static int gamma_hitrect(gamma_t* s, IRECT rc)
{
  int x1 = 0, y1 = 0, x2 = RCW(&rc), y2 = RCH(&rc);
  double bw, be, bb, gw;
  bw = s->m_border_width;
  be = s->m_border_extra;
  gw = s->m_grid_width;
  bb = bw + be;
  s->m_xs1 = x1 + bb;
  s->m_ys1 = y1 + s->m_text_height + bw;
  s->m_xs2 = x2 - bb;
  s->m_ys2 = y2 - bb;
  return 0;
}
static LRESULT gamma_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  gamma_t* s = (gamma_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    s->hit = 0;
    s->m_border_width = (2.0);
    s->m_border_extra = (2.0);
    s->m_curve_width = (2.0);
    s->m_grid_width = (0.2);
    s->m_text_thickness = (1.5);
    s->m_point_size = (3.0);
    s->m_text_height = (20.0);
    s->m_p1_active = (0);
    s->m_mouse_point = (0);
    s->m_pdx = (0.0);
    s->m_pdy = (0.0);
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->m_p1_active = -1;
      if (calc_distance(x - rc.l, y - rc.t, s->m_p1.x, s->m_p1.y) <= s->m_point_size + 1) {
        s->m_mouse_point = 1;
        s->m_pdx = s->m_p1.x - x;
        s->m_pdy = s->m_p1.y - y;
        s->m_p1_active = 1;
        force_redraw(c);
      }
      if (calc_distance(x - rc.l, y - rc.t, s->m_p2.x, s->m_p2.y) <= s->m_point_size + 1) {
        s->m_mouse_point = 2;
        s->m_pdx = s->m_p2.x - x;
        s->m_pdy = s->m_p2.y - y;
        s->m_p1_active = 2;
        force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONUP:
    if (s->m_mouse_point) {
      s->m_mouse_point = 0;
      force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      if (s->m_mouse_point == 1) {
        s->m_p1.x = x + s->m_pdx;
        s->m_p1.y = y + s->m_pdy;
        gamma_calc_values(s);
        gamma_calc_points(s);
        force_redraw(c);
      }
      else if (s->m_mouse_point == 2) {
        s->m_p2.x = x + s->m_pdx;
        s->m_p2.y = y + s->m_pdy;
        gamma_calc_values(s);
        gamma_calc_points(s);
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      gamma_hitrect(s, rc);
      gamma_calc_points(s);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      //int i;
      DPOINT pt[1000];
      int lens[10], n;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //XDRAW_BEG(im);
      {
        int x1 = rc.l, y1 = rc.t, x2 = rc.r, y2 = rc.b;
        double bw, be, bb, xc, yc, gw;
        COLOR clr;
        COLOR m_background_color = (_rgb(1.0, 1.0, 0.9));
        COLOR m_border_color = (_rgb(0.0, 0.0, 0.0));
        COLOR m_curve_color = (_rgb(0.0, 0.0, 0.0));
        COLOR m_grid_color = (_rgb(0.2, 0.2, 0.0));
        COLOR m_inactive_pnt_color = (_rgb(0.0, 0.0, 0.0));
        COLOR m_active_pnt_color = (_rgb(1.0, 0.0, 0.0));
        COLOR m_text_color = (_rgb(0.0, 0.0, 0.0));
        bw = s->m_border_width;
        be = s->m_border_extra;
        gw = s->m_grid_width;
        bb = bw + be;
        // Background Border
        //imdraw_rect(im, &rcclip, rc, m_background_color, m_border_color, m_border_width);
        lens[0] = vcgen_rect(pt, 0, countof(pt), x1, y1, x2, y2);
        imdrawaa_poly_solid(im, &rcclip, pt, lens, 1, m_background_color);
        // Border
        lens[0] = vcgen_rect2(pt, x1, y1, x2, y2, s->m_border_extra, 0);
        lens[1] = vcgen_rect2(pt + lens[0], x1, y1, x2, y1 + s->m_text_height + bb, bb, 1);
        lens[2] = vcgen_rect2(pt + lens[0] + lens[1], x1, y1 + s->m_text_height - bb + bw, x2, y2, bb, 1);
        imdrawaa_poly_solid(im, &rcclip, pt, lens, 3, m_border_color);
        // Curve
        n = vcgen_gamma_spline(pt, s->m_gamma_spline, s->m_xs1, s->m_ys1, s->m_xs2, s->m_ys2);
        imdrawaa_poly_stroke_solid(im, &rcclip, pt, n, 0, m_curve_color, s->m_curve_width);
        // Grid
        xc = (s->m_xs1 + s->m_xs2) / 2;
        yc = (s->m_ys1 + s->m_ys2) / 2;
        lens[0] = vcgen_rect2(pt, xc - gw / 2, s->m_ys1, xc + gw / 2, s->m_ys2, 0, 0);
        lens[1] = vcgen_rect2(pt + lens[0], s->m_xs1, yc - gw / 2, s->m_xs2, yc + gw / 2, 0, 0);
        imdrawaa_poly_solid(im, &rcclip, pt, lens, 2, m_grid_color);
        lens[0] = vcgen_rect2(pt, s->m_xs1, s->m_p1.y - gw / 2, s->m_p1.x, s->m_p1.y + gw / 2, 0, 0);
        imdrawaa_poly_solid(im, &rcclip, pt, lens, 1, m_grid_color);
        lens[0] = vcgen_rect2(pt, s->m_p1.x - gw / 2, s->m_p1.y, s->m_p1.x + gw / 2, s->m_ys2, 0, 0);
        imdrawaa_poly_solid(im, &rcclip, pt, lens, 1, m_grid_color);
        lens[0] = vcgen_rect2(pt, s->m_p2.x, s->m_p2.y - gw / 2, s->m_xs2, s->m_p2.y + gw / 2, 0, 0);
        imdrawaa_poly_solid(im, &rcclip, pt, lens, 1, m_grid_color);
        lens[0] = vcgen_rect2(pt, s->m_p2.x - gw / 2, s->m_ys1, s->m_p2.x + gw / 2, s->m_p2.y, 0, 0);
        imdrawaa_poly_solid(im, &rcclip, pt, lens, 1, m_grid_color);
        // Point1
        clr = 1 == s->m_p1_active ? m_active_pnt_color : m_inactive_pnt_color;
        imdrawaa_ellipse(im, &rcclip, s->m_p1.x, s->m_p1.y, s->m_point_size, s->m_point_size, clr, 0, 0);
        // Point2
        clr = 2 == s->m_p1_active ? m_active_pnt_color : m_inactive_pnt_color;
        imdrawaa_ellipse(im, &rcclip, s->m_p2.x, s->m_p2.y, s->m_point_size, s->m_point_size, clr, 0, 0);
        // Text
        {
          char buf[256];
          double kx1, ky1, kx2, ky2;
          IRECT rc2 = iRECT(x1 + (int)bb, y1 + (int)bb, x2, y1 + (int)s->m_text_height);
          gamma_spline_get_values(s->m_gamma_spline, &kx1, &ky1, &kx2, &ky2);
          _snprintf(buf, 256, "%5.3f %5.3f %5.3f %5.3f", kx1, ky1, kx2, ky2);
          imdraw_text(im, &rcclip, rc2, buf, -1, c->fo, m_text_color, 0, DT_VCENTER | DT_CENTER);
        }
      }
      //XDRAW_END();
    }
    c->redraw = 0;
    break;
  }
  return FALSE;
}
static int gamma_set(gamma_t* s, ctrl_t* li, double kx1, double ky1, double kx2, double ky2)
{
  ctrl_set(gamma_proc, s, s->c, li, WT_CHILD, 0);
  s->hit = 0;
  s->m_border_width = (2.0);
  s->m_border_extra = (2.0);
  s->m_curve_width = (2.0);
  s->m_grid_width = (0.2);
  s->m_text_thickness = (1.5);
  s->m_point_size = (5.0);
  s->m_text_height = (20.0);
  s->m_p1_active = (0);
  s->m_mouse_point = (0);
  s->m_pdx = (0.0);
  s->m_pdy = (0.0);
  gamma_spline_set_values(s->m_gamma_spline, kx1, ky1, kx2, ky2);
  return 0;
}
static int framectrl_cmd_proc_max(framectrl_t* s)
{
  //IRECT rc1;
  ctrl_t* c = s->c;
  if (s->ismax) {
    //rc1 = s->rc2;
    //xSendCmd(XCMD_MAXIMIZE);
  }
  else {
    s->rc2 = c->rc;
    //MyClientToScreen2(hwnd, &s->rc2, 2);
    //rc1 = xMaxRect();
    //xSendCmd(XCMD_MAXIMIZE);
    //SendMessage(sys->hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  }
  //xMoveWindow(hwnd, rc1, 0, 1);
  s->ismax = !s->ismax;
  return 0;
}
static int framectrl_hitrect(const framectrl_t* s, IRECT rect, xpos_t* prc)
{
  IRECT rc = rect;
  int has_cycaption = s->has_cap;
  int has_bordr = s->has_bdr && !s->ismax;
  //int cxicon = 16;
  //int cxsize = GetSystemMetrics(SM_CXSIZE);
  //int cysize = GetSystemMetrics(SM_CYSIZE);
  //int cxborder = GetSystemMetrics(SM_CXBORDER);
  //int cyborder = GetSystemMetrics(SM_CYBORDER);
  int cxdlgframe = GetSystemMetrics(SM_CXDLGFRAME);
  int cycaption = GetSystemMetrics(SM_CYCAPTION);
  int d = has_cycaption ? cycaption : 0, b = has_bordr ? cxdlgframe : 0, c = 0, t = 2, tt;
  memset(prc, 0, sizeof(xpos_t)*HITRECTNUM);
  if (has_bordr) {
    rc = rect;
    t = b;
    tt = t;
    //iRectOffset(rc, -t, -t, t, t, &rc);
    iRectCutT(&rc, tt, 0, &prc[HTTOP].rc);
    iRectCutT(&rc, -tt, 0, &prc[HTBOTTOM].rc);
    iRectCutL(&rc, tt, 0, &prc[HTLEFT].rc);
    iRectCutL(&rc, -tt, 0, &prc[HTRIGHT].rc);
    iRectCutL(&prc[HTTOP].rc, tt, 0, &prc[HTTOPLEFT].rc);
    iRectCutL(&prc[HTTOP].rc, -tt, 0, &prc[HTTOPRIGHT].rc);
    iRectCutL(&prc[HTBOTTOM].rc, tt, 0, &prc[HTBOTTOMLEFT].rc);
    iRectCutL(&prc[HTBOTTOM].rc, -tt, 0, &prc[HTBOTTOMRIGHT].rc);
    iRectOffset(&rc, b, b, -b, -b);
  }
  prc[HTCLOSE].z = prc[HTMAXBUTTON].z = prc[HTMINBUTTON].z = prc[HTSYSMENU].z = 1;
  if (d > 0) {
    t = 2;
    rc = rect;
    iRectOffset(&rc, b, b, -b, -b);
    iRectCutT(&rc, d, 0, &prc[HTCAPTION].rc);
    prc[HTCLIENT].rc = rc;
    rc = prc[HTCAPTION].rc;
    iRectOffset(&rc, t, t, -t, -1);
    //c = RCH(&rc);
    //iRectCutL(&rc, c, 0, &prc[HTSYSMENU].rc);
    iRectAlign(rc, 16, 16, DT_LEFT | DT_VCENTER, &prc[HTSYSMENU].rc);
    c = RCH(&rc) + 2;
    iRectCutL(&rc, -c, 0, &prc[HTCLOSE].rc);
    iRectCutL(&rc, -2, 0, &prc[HTMAXBUTTON].rc);
    iRectCutL(&rc, -c, 0, &prc[HTMAXBUTTON].rc);
    iRectCutL(&rc, -c, 0, &prc[HTMINBUTTON].rc);
  }
  else {
    prc[HTCLIENT].rc = rc;
  }
  return 0;
}
static int framectrl_hittest(const framectrl_t* s, int x, int y)
{
  int i = 0, ret = -1, z = -1, n = HITRECTNUM;
  xpos_t pos[HITRECTNUM];
  framectrl_hitrect(s, s->c->rc, pos);
  //x -= s->c->rc.l; y -= s->c->rc.t;
  for (; i < n; ++i) {
    if (pos[i].z > z && iPtInRect(&pos[i].rc, x, y)) {
      z = pos[i].z;
      ret = i;
    }
  }
  if (0) {
#define XWINHITDEF(a, b) if (ret==a) printf("%s\n", b);
    //XWINHITDEF(HITERROR,"屏幕背景或窗口间的分界线上（与HTNOWHERE相同，但不包括DefWindowProc函数引发beep的错误的位置）")
    XWINHITDEF(HTTRANSPARENT, "在被同一线程的另一个窗口覆盖的窗口上")
    XWINHITDEF(HTBORDER, "不能改变大小的窗口的边框")
    XWINHITDEF(HTCAPTION, "标题栏")
    XWINHITDEF(HTCLIENT, "客户区域")
    XWINHITDEF(HTGROWBOX, "缩放格(同HTSIZE)")
    XWINHITDEF(HTMENU, "菜单中")
    XWINHITDEF(HTHELP, "帮助按钮")
    XWINHITDEF(HTMAXBUTTON, "最大化按钮")
    XWINHITDEF(HTMINBUTTON, "最小化按钮")
    XWINHITDEF(HTREDUCE, "最小化按钮")
    XWINHITDEF(HTCLOSE, "关闭按钮")
    XWINHITDEF(HTNOWHERE, "屏幕背景或窗口间的分界线上")
    XWINHITDEF(HTLEFT, "窗口的左边框")
    XWINHITDEF(HTRIGHT, "窗口的右边框")
    XWINHITDEF(HTSIZE, "缩放格(同HTGROWBOX)")
    XWINHITDEF(HTTOP, "窗口的上边框")
    XWINHITDEF(HTBOTTOM, "下边框")
    XWINHITDEF(HTTOPLEFT, "边框的左上角")
    XWINHITDEF(HTTOPRIGHT, "边框的右上角")
    XWINHITDEF(HTBOTTOMLEFT, "边框的左下角")
    XWINHITDEF(HTBOTTOMRIGHT, "边框的右下角")
    XWINHITDEF(HTVSCROLL, "垂直滚动条")
    XWINHITDEF(HTHSCROLL, "水平滚动条")
    XWINHITDEF(HTZOOM, "最大化按钮")
    XWINHITDEF(HTSYSMENU, "系统菜单或子窗体的关闭按钮上")
#undef XWINHITDEF
  }
  return ret;
}
static LRESULT framectrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  framectrl_t* s = (framectrl_t*)c->x;
  //IRECT rc = c->rc;
  IRECT rc1 = c->rc;
  int hit = 0, x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  int ret = 0;
  xpos_t pos[HITRECTNUM];
  //TraceMsg(hwnd, uMsg, wParam, lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    s->has_cap = 1;
    s->has_bdr = 1;
    break;
  case WM_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
#if 0
  case WM_NCHITTEST:
    myScreenToClient(hwnd, &x, &y);
    if (iPtInRect(&rc1, x, y)) {
      hit = framectrl_hittest(s, x, y);
      if (hit >= 0) {
        return hit;
      }
    }
    break;
#endif
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = font_text_size(c->fo, s->c->text, -1, 0);
    if (c->sub) {
      c->sub->ctrlproc(0, c->sub, WM_GETMINMAXINFO, 0, (LPARAM)mminfo);
    }
    mminfo->ptMinTrackSize.x += sz.w + sz.h;
    mminfo->ptMinTrackSize.y += sz.h + sz.h;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_NCLBUTTONUP:
  case WM_LBUTTONUP:
    if (s) {
      if (HTCLOSE == s->hit || HTMAXBUTTON == s->hit || HTMINBUTTON == s->hit) {
        hit = framectrl_hittest(s, x, y);
        if (s->hit == hit) {
          if (HTCLOSE == hit) {
            sendmsg(c, c->listener, WM_COMMAND, XCMD_CLOSE, (LPARAM)c);
          }
          if (HTMAXBUTTON == hit) {
            framectrl_cmd_proc_max(s);
          }
          if (HTMINBUTTON == hit) {
            //PostMessage(sys->hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
          }
        }
        force_redraw(c);
      }
      s->hit = 0;
    }
    break;
  case WM_LBUTTONDBLCLK:
    if (s) {
      hit = framectrl_hittest(s, x, y);
      if (HTCAPTION == hit) {
        framectrl_cmd_proc_max(s);
        force_redraw(c);
      }
    }
    break;
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc1, x, y)) {
      s->x = x, s->y = y;
      s->rc1 = c->rc;
      s->hit = hit = framectrl_hittest(s, x, y);
      ctrl_setcursor(c, hit);
      if (HTMAXBUTTON == hit || HTMINBUTTON == hit || HTCLOSE == hit) {
        force_redraw(c);
      }
      {
        ctrl_t* fa = c->father;
        ctrl_remove(c);
        ctrl_addsub_tail(fa, c);
        force_redraw(c);
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    //myScreenToClient(hwnd, &x, &y);
    if (c->sys->ldown) {
      int hit = s->hit;
      if (!s->ismax) {
        IRECT rcoff = {0};
        int dx, dy, donul = 0;
        dx = x - s->x, dy = y - s->y;
        //s->hot = hit;
        switch (hit) {
        case HTCAPTION:
          rcoff.l = rcoff.r = dx;
          rcoff.t = rcoff.b = dy;
          break;
        case HTLEFT:
          rcoff.l = dx;
          break;
        case HTRIGHT:
          rcoff.r = dx;
          break;
        case HTTOP:
          rcoff.t = dy;
          break;
        case HTBOTTOM:
          rcoff.b = dy;
          break;
        case HTTOPLEFT:
          rcoff.t = dy;
          rcoff.l = dx;
          break;
        case HTTOPRIGHT:
          rcoff.t = dy;
          rcoff.r = dx;
          break;
        case HTBOTTOMLEFT:
          rcoff.b = dy;
          rcoff.l = dx;
          break;
        case HTBOTTOMRIGHT:
          rcoff.b = dy;
          rcoff.r = dx;
          break;
        default:
          donul = 1;
          hit = framectrl_hittest(s, x, y);
          break;
        }
        if (!donul) {
          rc1 = s->rc1;
          iRectAdd(&rc1, &rcoff);
          rc1 = ctrl_calc_rect_impl(c, rc1);
          ctrl_setrect(c, rc1);
          //xMoveWindow(hwnd, rc1, 1, 1);
          //printf("%d %d %d %d %d %d %s\n", rc1.l, rc1.t, rcoff.l, rcoff.t, rcoff.r, rcoff.b, c->text);
          //c->rc = rc1;
          //s->x = x;
          //s->y = y;
          force_redraw(c);
        }
        //printf("%d %d %d %d\n", x, y, rc.l, rc.t);
      }
      return 1;
    }
    else {
      hit = framectrl_hittest(s, x, y);
      if (s->has_bdr && !s->ismax) {
        ctrl_setcursor(c, hit);
      }
      if (s->hot != hit) {
        if (HTCLOSE == s->hot || HTMAXBUTTON == s->hot || HTMINBUTTON == s->hot) {
          force_redraw(c);
        }
        s->hot = hit;
        if (HTCLOSE == s->hot || HTMAXBUTTON == s->hot || HTMINBUTTON == s->hot) {
          force_redraw(c);
        }
      }
    }
    break;
  case WM_PAINT:
    framectrl_hitrect(s, c->rc, pos);
    if (c->recalcsize) {
      ctrl_setrect(c->sub, pos[HTCLIENT].rc);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i;
      COLOR clr = 0;
      static const int g_hitbtn[3] = {HTCLOSE, HTMAXBUTTON, HTMINBUTTON};
      int tt[] = {IdClose, s->ismax ? IdRestore : IdMaximize, IdMinimize};
      IRECT rc1;
      IRECT rc2 = c->rc;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      rc1 = rc;
      rc2 = rc;
      framectrl_hitrect(s, rc, pos);
      //iRectOffset(&rc2, -c->rc.l, -c->rc.t, -c->rc.l, -c->rc.t);
      //printf("WM_PAINT\n");
      //framectrl_hitrect(rc2, s->rcs, s->has_cap, s->has_bdr && !s->ismax);
      if (1) {
        imdraw_border_syscolor(im, &rcclip, rc2, BDS_HIGH, (s->has_bdr && !s->ismax) ? BDM_ALL : 0);
        if (s->has_cap) {
          COLOR clr1 = ctrl_isfocus(c) ? _RGB(10, 36, 106) : _RGB(106, 106, 106);
          COLOR clr2 = ctrl_isfocus(c) ? _RGB(166, 202, 240) : _RGB(240, 240, 240);
          imdraw_rect_gradient(im, &rcclip, pos[HTCAPTION].rc, clr1, clr2, 1);
          for (i = 0; i < 3; ++i) {
            int pushed = s->hit == g_hitbtn[i];
            IRECT rc1 = pos[g_hitbtn[i]].rc;
            imdraw_border_syscolor(im, &rcclip, rc1, pushed ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
            rc1.l += 2 * pushed - 1;
            imdraw_menuimg(im, &rcclip, rc1, tt[i], Color_Black);
          }
        }
      }
      else {
        COLOR clrs[] = {Color_Blue, Color_Blue, Color_Blue};
        imdraw_border_n(im, &rcclip, rc2, Color_White, countof(clrs), clrs, clrs, clrs, clrs);
        if (s->has_cap) {
          //imdraw_rect_gradient(im, pos[HTCAPTION].rc, _RGB(10, 36, 106), _RGB(166, 202, 240), 1);
          imdraw_rect(im, &rcclip, pos[HTCAPTION].rc, Color_Green, 0, 0, 0);
          for (i = 0; i < 3; ++i) {
            int hoted = s->hot == g_hitbtn[i];
            int pushed = s->hit == g_hitbtn[i];
            IRECT rc1 = pos[g_hitbtn[i]].rc;
            imdraw_rect(im, &rcclip, rc1, pushed ? Color_Gray : (hoted ? Color_Khaki : Color_White), 0, 0, 0);
            rc1.l += 2 * pushed - 1;
            imdraw_menuimg(im, &rcclip, rc1, tt[i], Color_Black);
          }
        }
      }
      if (s->has_cap) {
        rc1 = pos[HTCAPTION].rc;
        rc1.l = pos[HTSYSMENU].rc.r;
        rcclip.r = pos[HTMINBUTTON].rc.l;
        imdraw_text(im, &rcclip, rc1, s->c->text, -1, c->fo, Color_White, 0, DT_VCENTER);
      }
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
CC_INLINE int framectrl_set(framectrl_t* s, ctrl_t* li, const char* text, IRECT rc)
{
  ctrl_set(framectrl_proc, s, s->c, li, WT_CHILD, text);
  s->hit = 0;
  rc = ctrl_calc_rect_impl(s->c, rc);
  ctrl_setrect(s->c, rc);
  ctrl_setfocus(s->c);
  return 0;
}

