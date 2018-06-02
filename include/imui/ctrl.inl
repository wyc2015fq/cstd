#if 0
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
CC_INLINE int slider_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  IPOINT pt = iPOINT(x, y);
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo;
    ISIZE sz = imuimeasureText(c->fo, s->text, -1, 0);
    mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = sz.w + sz.h;
    mminfo->ptMinTrackSize.y = sz.h + sz.h;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      if (calc_distance(pt.x, pt.y, s->pt.x, s->pt.y) < RCH(&rc) * 0.5) {
        PTSUB(s->m_pdx, s->pt, pt);
        io->HitValue = 1;
        FORCE_REDRAW(c);
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
  case MSG_LBUTTONUP:
    if (s) {
      io->HitValue = 0;
      //slider_hitrect(s, RCH(&rc), RCW(&rc));
      FORCE_REDRAW(c);
    }
    break;
  case MSG_MOUSEMOVE:
    if (s) {
      if (io->HitValue) {
        PTADD(s->pt, s->m_pdx, pt);
        s->m_value = s->m_min + (s->pt.x - s->rcsub->l) * (s->m_max - s->m_min) / RCW(s->rcsub);
        s->m_value = BOUND(s->m_value, s->m_min, s->m_max);
        sendmsg(c, c->listener, WM_COMMAND, XCMD_CHANGE, 0);
        slider_hitrect(s, rc);
        FORCE_REDRAW(c);
      }
      else {
        BOOL bIn = iPtInRect(&rc, x, y);
        if (s->hot != bIn) {
          s->hot = bIn;
        }
      }
    }
    break;
  case MSG_PAINT:
    if (c->recalcsize) {
      slider_hitrect(s, rc);
    }
    if (1) {
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      if (1) {
        IRECT rect = rc;
        int b = MIN(RCH(&rc), RCW(&rc)) / 2 - 1;
        BOOL bVert = RCH(&rc) > RCW(&rc);
        RCOFFSET(&rect, b, b, -b, -b);
        ImDraw_edgeRectSys(ctx, &rcclip, rc, BDS_LOW, 0);
        ImDraw_edgeRectSys(ctx, &rcclip, rect, BDS_LOW, BDM_ALL);
        {
          IRECT rect = rc;
          BOOL bIsHighlighted = 0, bIsPressed = io->HitValue, bIsDisabled = 0;
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
          ImDraw_edgeRectSys(ctx, &rcclip, rect, BDS_HIGH, BDM_ALL);
        }
        if (s->text) {
          char buf[256];
          _snprintf(buf, 256, s->text, s->m_value);
          imdraw_text(ctx, 0, rc, buf, -1, c->fo, Color_Black, 0, TF_VCENTER);
        }
      }
      else {
        COLOR m_background_color = (_rgb(1.0, 0.9, 0.8));
        COLOR m_triangle_color = (_rgb(0.7, 0.6, 0.6));
        COLOR m_text_color = (_rgb(0.0, 0.0, 0.0));
        COLOR m_pointer_preview_color = (_rgba(0.6, 0.4, 0.4, 0.4));
        COLOR m_pointer_color[2] = {(_rgba(0.8, 0.0, 0.0, 0.6)), (_rgba(0.9, 0.0, 0.0, 0.8))};
        // Background
        imdraw_rect(ctx, &rcclip, rc, m_background_color, 0, 0, BDM_ALL);
        imdraw_rect(ctx, &rcclip, s->rcsub[0], m_triangle_color, 0, 0, BDM_ALL);
        imdrawaa_ellipse(ctx, &rcclip, s->pt.x, s->pt.y, s->rad, s->rad, m_pointer_color[1], 0, 0);
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
            imdrawaa_poly(ctx, &rcclip, pt, 3, 1, m_text_color, 0, 0);
          }
        }
        if (s->text) {
          char buf[256];
          _snprintf(buf, 256, s->text, s->m_value);
          imdraw_text(ctx, 0, rc, buf, -1, c->fo, m_text_color, 0, TF_VCENTER);
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
CC_INLINE int scale_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  double m_xs1[2];
  double m_xs2[2];
  scale_t* s = (scale_t*)c->x;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
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
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    io->HitValue = 0;
    s->m_pdx[0] = (0.0);
    s->m_pdx[1] = (0.0);
    s->m_move_what = 0;
    s->m_value1 = (0.3);
    s->m_value2 = (0.7);
    s->m_min_d = (0.01);
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      double r = m_border_extra;
      xx1[!bb] = m_xs1[!bb];
      xx2[!bb] = m_xs2[!bb];
      if (calc_distance(x, y, xx1[0], xx1[1]) <= r) {
        //if(x < xp1 && calc_distance(x, y, xp1, yp) <= m_y2 - m_y1)
        s->m_pdx[0] = xx[0] - xx1[0];
        s->m_pdx[1] = xx[1] - xx1[1];
        s->m_move_what = move_value1;
        FORCE_REDRAW(c);
      }
      else if (calc_distance(x, y, xx2[0], xx2[1]) <= r) {
        //if(x > xp2 && calc_distance(x, y, xp2, yp) <= m_y2 - m_y1)
        s->m_pdx[0] = xx[0] - xx2[0];
        s->m_pdx[1] = xx[1] - xx2[1];
        s->m_move_what = move_value2;
        FORCE_REDRAW(c);
      }
      else if (xx[0] > xx1[0] && xx[0] < xx2[0] && xx[1] > xx1[1] && xx[1] < xx2[1]) {
        s->m_pdx[0] = xx[0] - xx1[0];
        s->m_pdx[1] = xx[1] - xx1[1];
        s->m_move_what = move_slider;
        FORCE_REDRAW(c);
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case MSG_LBUTTONUP:
    s->m_move_what = move_nothing;
    FORCE_REDRAW(c);
    break;
  case MSG_MOUSEMOVE:
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
            FORCE_REDRAW(c);
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
            FORCE_REDRAW(c);
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
            FORCE_REDRAW(c);
          }
          break;
        }
      }
    }
    break;
  case MSG_PAINT:
    if (wParam) {
      double m_border_thickness = 1;
      double r, bw = m_border_thickness, be = m_border_extra;
      int m_descending = (FALSE);
      COLOR m_background_color = _rgb(1.0, 0.9, 0.8);
      COLOR m_border_color = _rgb(0.0, 0.0, 0.0);
      COLOR m_pointers_color = _rgba(0.8, 0.0, 0.0, 0.8);
      COLOR m_slider_color = _rgba(0.2, 0.1, 0.0, 0.6);
      COLOR m_text_color = _rgb(0.0, 0.0, 0.0);
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
          imdrawaa_rect(ctx, &rcclip, x1, y1, x2, y2, m_background_color, 0, 0);
          //lens[0] = vcgen_rect(pt, x1, y1, x2, y2);
          imdrawaa_rect(ctx, &rcclip, m_xs1[0], m_xs1[1], m_xs2[0], m_xs2[1], 0, m_border_color, 1);
          r = m_border_extra;
          xx1[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * s->m_value1;
          xx2[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * s->m_value2;
          xx1[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
          xx2[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
          imdrawaa_ellipse(ctx, &rcclip, xx1[0], xx1[1], r, r, m_pointers_color, 0, 0);
          imdrawaa_ellipse(ctx, &rcclip, xx2[0], xx2[1], r, r, m_pointers_color, 0, 0);
          xx1[!bb] = m_xs1[!bb];
          xx2[!bb] = m_xs2[!bb];
          imdrawaa_rect(ctx, &rcclip, xx1[0], xx1[1], xx2[0], xx2[1], m_slider_color, 0, 0);
        }
      }
#if 1
      {
        char buf[256];
        _snprintf(buf, 256, "%.3f %.3f", s->m_value1, s->m_value2);
        imdraw_text(ctx, 0, rc, buf, -1, c->fo, m_text_color, 0, TF_VCENTER);
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
//static int layout_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags);
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
CC_INLINE int layout_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  layout_t* s = (layout_t*)c->x;
  int ret = 0;
  int i, j, al;
  const char* id0;
  const char* id;
  IPOINT hitpt = { -1, -1};
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  al = s->cols + 2;
  id0 = s->id;
  id = id0 + al + 1;
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    io->HitValue = -1;
    s->hot = -1;
    io->HitValuebd = iPOINT(-1, -1);
    //c->isshow = SW_HIDE;
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    FREE(s->co);
    FREE(s->ro);
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
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
      ISIZE sz = imuimeasureText(c->fo, s->text, -1, 0);
      mminfo->ptMinTrackSize.x = MAX(sz.w, mminfo->ptMinTrackSize.x) + 8;
      mminfo->ptMinTrackSize.y += sz.h + 4;
    }
    return 0;
  }
  break;
  case MSG_LBUTTONUP:
    if (c->sys->ldown) {
      io->HitValue = 0;
      io->HitValuebd = iPOINT(-1, -1);
      if (iPtInRect(&rc, x, y)) {
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_LBUTTONDBLCLK:
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      int ret = layout_getrect(s, rc, x, y, &hitpt);
      if ((1 & ret || 2 & ret)) {
        sys_setcursor(c->sys, IDC_SIZEWE);
      }
      else if (4 & ret || 8 & ret) {
        sys_setcursor(c->sys, IDC_SIZENS);
      }
      if (ret) {
        FORCE_REDRAW(c);
      }
      s->lastdownpt = iPOINT(x, y);
      io->HitValuebd = hitpt;
      ctrl_setfocus(c);
      return io->HitValuebd.x >= 0 || io->HitValuebd.y >= 0;
    }
    break;
  case MSG_MOUSEMOVE:
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
        if (io->HitValuebd.x >= 0 || io->HitValuebd.y >= 0) {
          if (io->HitValuebd.x >= 0) {
            int d = x - s->lastdownpt.x;
            s->co[io->HitValuebd.x] += d;
            s->co[io->HitValuebd.x + 1] -= d;
          }
          if (io->HitValuebd.y >= 0) {
            int d = y - s->lastdownpt.y;
            s->ro[io->HitValuebd.y] += d;
            s->ro[io->HitValuebd.y + 1] -= d;
          }
          FORCE_REDRAW(c);
        }
        s->lastdownpt = iPOINT(x, y);
        ret = layout_getrect(s, rc, x, y, &hitpt);
      }
    }
    break;
  case MSG_PAINT:
    if (c->recalcsize) {
      layout_getrect(s, rc, 0, 0, &hitpt);
    }
    if (1) {
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      if (s->text) {
        groupbox_draw(ctx, &rcclip, rc, s->text, -1, BDS_ETCHED, 0, c->fo);
      }
      //imdraw_rect(ctx, 0, rc, _rgb(1,0,1), _rgb(0,0,0), 1, 0);
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
static int cut_layout_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  cut_layout_t* s = (cut_layout_t*)c->x;
  int ret = 0;
  int hit, cmd, i;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    io->HitValue = -1;
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x += 120;
    mminfo->ptMinTrackSize.y += 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case MSG_LBUTTONUP:
    if (s) {
      io->HitValue = -1;
      if (iPtInRect(&rc, x, y)) {
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_COMMAND:
    if (s) {
    }
    break;
  case MSG_LBUTTONDBLCLK:
  case MSG_LBUTTONDOWN:
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
        io->HitValue = hit;
        s->pt = iPOINT(x, y);
        FORCE_REDRAW(c);
        return TRUE;
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case MSG_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      if (io->HitValue < 0) {
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
          FORCE_REDRAW(c);
        }
        else {
          sys_setcursor(c->sys, IDC_ARROW);
        }
      }
      else {
        hit = io->HitValue;
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
        FORCE_REDRAW(c);
        return TRUE;
      }
    }
    break;
  case MSG_PAINT:
    if (c->recalcsize) {
      cut_layout_calc_rect(s, 0, 0);
    }
    if (1) {
      int ret = -1, d = 0;
      //imdraw_fillrect(ctx, &rcclip, rc, _rgb(1, 1, 1));
    }
    break;
  }
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
  COLOR cr3dFace = myGetSysColor(CLR_3DFACE);
  COLOR clrs[] = {_RGB(255, 255, 255), cr3dFace};
  int nState[4] = {IdArowLeftLarge, IdArowRightLarge, IdArowUpLarge, IdArowDownLarge, };
  int i;
  scrollview_calc_rect(s, c, prc);
  imdraw_rect(ctx, &c->rcclip, prc[1], cr3dFace, 0, 0, 0);
  //printf("WM_PAINT scrollview\n");
  for (i = 0; i < 2; ++i) {
    IRECT* rcb = prc + 2 + i * 6;
    imdraw_rect_colors(ctx, &c->rcclip, rcb[SBHT_PAGE_0], clrs, 2, 0);
    imdraw_rect_colors(ctx, &c->rcclip, rcb[SBHT_PAGE_1], clrs, 2, 0);
    ImDraw_edgeRectSys(ctx, &c->rcclip, rcb[SBHT_LINE_0], (i == (io->HitValue >> 8) && SBHT_LINE_0 == ((io->HitValue & 0xff) - 1)) ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
    imdraw_menuimg(ctx, &c->rcclip, rcb[SBHT_LINE_0], nState[i * 2 + 0], _rgb(0, 0, 0));
    ImDraw_edgeRectSys(ctx, &c->rcclip, rcb[SBHT_LINE_1], (i == (io->HitValue >> 8) && SBHT_LINE_1 == ((io->HitValue & 0xff) - 1)) ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
    imdraw_menuimg(ctx, &c->rcclip, rcb[SBHT_LINE_1], nState[i * 2 + 1], _rgb(0, 0, 0));
    ImDraw_edgeRectSys(ctx, &c->rcclip, rcb[SBHT_THUMB], BDS_HIGH, BDM_ALL);
  }
  return 0;
}
CC_INLINE int scrollview_proc(scrollview_t* s, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int hit = 0, x = LOWORD(lParam), y = HIWORD(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    return 0;
  }
  switch (io->lastMsg) {
  case MSG_GETMINMAXINFO:
    if (s) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      mminfo->ptMinTrackSize.x = SBW1 * 3;
      mminfo->ptMinTrackSize.y = SBW1 * 3;
    }
    break;
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_LBUTTONUP:
    if (io->HitValue) {
      io->HitValue = 0;
      FORCE_REDRAW(c);
    }
    break;
#if 0
  case MSG_MOUSEWHEEL:
    if (iPtInRect(&s->rcc, x, y)) {
      int i = 1;
      //printf("WM_MOUSEWHEEL SBHT_PAGE_1\n");
      s->si[i].nTrackPos -= GET_WHEEL_DELTA_WPARAM(wParam);
      s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - (int)s->si[i].nPage);
      sendmsg(c, sender, WM_VSCROLL, 0, 0);
    }
    break;
#endif
  case MSG_LBUTTONDBLCLK:
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      hit = scrollview_hittest(s, c, x, y);
      s->pthit = iPOINT(x, y);
      if (hit != io->HitValue) {
        int i = hit >> 8;
        io->HitValue = hit;
        if (SBHT_THUMB == ((io->HitValue & 0xff) - 1)) {
          io->HitValue_pos = s->si[i].nPos;
        }
        else {
          int page = (int)s->si[i].nPage;
          int line = MAX(1, page / 8);
          int steps[5] = { -line, line, -page, page};
          int msgs[5] = { SB_LINEUP, SB_LINEDOWN, SB_PAGEUP, SB_PAGEDOWN};
          s->si[i].nTrackPos = s->si[i].nPos + steps[(io->HitValue & 0xff) - 1];
          s->si[i].nTrackPos = s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - page);
          //s->si[i].nTrackPos = s->si[i].nPos + steps[(io->HitValue & 0xff) - 1];
          //BOUND(, 0, (1 << 16) - page);
          scrollview_set_pos(s->si + i, s->si[i].nTrackPos);
          //sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, msgs[(io->HitValue & 0xff) - 1]), 0);
        }
        FORCE_REDRAW(c);
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case MSG_MOUSEMOVE:
    //printf("WM_MOUSEMOVE\n");
    if (io->HitValue > 0) {
      int i = io->HitValue >> 8;
      if (SBHT_THUMB == ((io->HitValue & 0xff) - 1)) {
        int page = (int)s->si[i].nPage;
        int len = s->si[i].nMax - s->si[i].nMin + 1;
        if (i) {
          int ccc = RCH(&rc) - 2 * SBW1;
          if (ccc) {
            s->si[i].nTrackPos = io->HitValue_pos + ((y - s->pthit.y) * len + ccc / 2) / ccc;
          }
        }
        else {
          int ccc = RCW(&rc) - 2 * SBW1;
          if (ccc) {
            s->si[i].nTrackPos = io->HitValue_pos + ((x - s->pthit.x) * len + ccc / 2) / ccc;
          }
        }
        s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - page);
        scrollview_set_pos(s->si + i, s->si[i].nTrackPos);
        //sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, SB_THUMBTRACK), s->si[i].nTrackPos);
        FORCE_REDRAW(c);
      }
    }
    else if (iPtInRect(&rc, x, y)) {
      hit = scrollview_hittest(s, c, x, y);
      if (s->hot != hit) {
        s->hot = hit;
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_PAINT:
    if (c->recalcsize) {
      IRECT prc[14] = {0};
      scrollview_calc_rect(s, c, prc);
    }
    if (c->redraw && RCH(&rc) > 0 && RCW(&rc) > 0) {
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
CC_INLINE int scrollbar_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  int i;
  scrollbar_t* s = (scrollbar_t*)c->x;
  int hit = 0, x = io->MousePos.x, y = io->MousePos.y;
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    //s->si[0].nPos = (int)((0.1) * (1 << 16));
    //s->si[0].nPage = (int)((0.5) * (1 << 16));
    //s->si[1].nPos = (int)((0.1) * (1 << 16));
    //s->si[1].nPage = (int)((0.5) * (1 << 16));
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_GETMINMAXINFO:
    if (s) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      mminfo->ptMinTrackSize.x = SBW1 * 3;
      mminfo->ptMinTrackSize.y = SBW1 * 3;
    }
    break;
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_LBUTTONUP:
    if (io->HitValue) {
      io->HitValue = 0;
      FORCE_REDRAW(c);
    }
    break;
#if 0
  case MSG_MOUSEWHEEL:
    if (iPtInRect(&s->rcc, x, y)) {
      int i = 1;
      //printf("WM_MOUSEWHEEL SBHT_PAGE_1\n");
      s->si[i].nTrackPos -= GET_WHEEL_DELTA_WPARAM(wParam);
      s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - (int)s->si[i].nPage);
      sendmsg(c, sender, WM_VSCROLL, 0, 0);
    }
    break;
#endif
  case MSG_LBUTTONDBLCLK:
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      hit = scrollbar_hittest(s, x, y);
      s->pthit = iPOINT(x, y);
      if (hit != io->HitValue) {
        int i = hit >> 8;
        io->HitValue = hit;
        if (SBHT_THUMB == ((io->HitValue & 0xff) - 1)) {
          io->HitValue_pos = s->si[io->HitValue >> 8].nPos;
        }
        else {
          int page = (int)s->si[i].nPage;
          int line = MAX(1, page / 8);
          int steps[5] = { -line, line, -page, page};
          int msgs[5] = { SB_LINEUP, SB_LINEDOWN, SB_PAGEUP, SB_PAGEDOWN};
          s->si[i].nTrackPos = s->si[i].nPos + steps[(io->HitValue & 0xff) - 1];
          s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - page);
          //s->si[i].nTrackPos = s->si[i].nPos + steps[(io->HitValue & 0xff) - 1];
          //BOUND(, 0, (1 << 16) - page);
          sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, msgs[(io->HitValue & 0xff) - 1]), 0);
        }
        FORCE_REDRAW(c);
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case MSG_MOUSEMOVE:
    //printf("WM_MOUSEMOVE\n");
    if (io->HitValue > 0) {
      int i = io->HitValue >> 8;
      if (SBHT_THUMB == ((io->HitValue & 0xff) - 1)) {
        int page = (int)s->si[i].nPage;
        int len = s->si[i].nMax - s->si[i].nMin + 1;
        if (i) {
          int ccc = RCH(&rc) - 2 * SBW1;
          if (ccc) {
            s->si[i].nTrackPos = io->HitValue_pos + ((y - s->pthit.y) * len + ccc / 2) / ccc;
          }
        }
        else {
          int ccc = RCW(&rc) - 2 * SBW1;
          if (ccc) {
            s->si[i].nTrackPos = io->HitValue_pos + ((x - s->pthit.x) * len + ccc / 2) / ccc;
          }
        }
        s->si[i].nTrackPos = BOUND(s->si[i].nTrackPos, 0, 1 + s->si[i].nMax - page);
        sendmsg(c, c->listener, i ? WM_VSCROLL : WM_HSCROLL, MAKELONG(0, SB_THUMBTRACK), s->si[i].nTrackPos);
        FORCE_REDRAW(c);
      }
    }
    else if (iPtInRect(&rc, x, y)) {
      hit = scrollbar_hittest(s, x, y);
      if (s->hot != hit) {
        s->hot = hit;
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_PAINT:
    if (c->recalcsize) {
      scrollbar_calc_rect(s, rc);
    }
    if (c->redraw && RCH(&rc) > 0 && RCW(&rc) > 0) {
      COLOR clrs[] = {_RGB(255, 255, 255), myGetSysColor(CLR_3DFACE)};
      int nState[4] = {IdArowLeftLarge, IdArowRightLarge, IdArowUpLarge, IdArowDownLarge, };
      IRECT rcclip = c->rcclip;
      scrollbar_calc_rect(s, rc);
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //printf("WM_PAINT scrollbar\n");
      for (i = 0; i < 2; ++i) {
        IRECT* rc1 = s->rc[i];
        imdraw_rect_colors(ctx, &rcclip, rc1[SBHT_PAGE_0], clrs, 2, 0);
        imdraw_rect_colors(ctx, &rcclip, rc1[SBHT_PAGE_1], clrs, 2, 0);
        ImDraw_edgeRectSys(ctx, &rcclip, rc1[SBHT_LINE_0], (i == (io->HitValue >> 8) && SBHT_LINE_0 == ((io->HitValue & 0xff) - 1)) ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
        imdraw_menuimg(ctx, &rcclip, rc1[SBHT_LINE_0], nState[i * 2 + 0], _rgb(0, 0, 0));
        ImDraw_edgeRectSys(ctx, &rcclip, rc1[SBHT_LINE_1], (i == (io->HitValue >> 8) && SBHT_LINE_1 == ((io->HitValue & 0xff) - 1)) ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
        imdraw_menuimg(ctx, &rcclip, rc1[SBHT_LINE_1], nState[i * 2 + 1], _rgb(0, 0, 0));
        ImDraw_edgeRectSys(ctx, &rcclip, rc1[SBHT_THUMB], BDS_HIGH, BDM_ALL);
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
static int imageview_update_scroll(ctxageview_t* s)
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
static int imageview_setimg(ctxageview_t* s, const img_t* im)
{
  if (ctx) {
    imclone2(ctx, s->im);
    imageview_update_scroll(s);
  }
  return 0;
}
static int imageview_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  imageview_t* s = (ctxageview_t*)c->x;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  int ret = 0;
  if (WM_PAINT != uMsg) {
    if (ret = scrollview_proc(s->sc, c, uMsg, wParam, lParam)) {
      return ret;
    }
  }
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    c->style = WS_HSCROLL | WS_VSCROLL;
    s->scale = 1.;
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    imfree(s->im);
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 120;
  }
  break;
  case MSG_TIMER:
    if (1) {
      FORCE_REDRAW(c);
    }
    break;
  case MSG_LBUTTONDBLCLK:
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      sys_setcursor(c->sys, IDC_HAND);
      FORCE_REDRAW(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case MSG_MOUSEWHEEL:
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
  case MSG_MOUSEMOVE:
    if (s) {
      int hot = iPtInRect(&rc, x, y);
      if (hot) {
        sys_setcursor(c->sys, IDC_HAND);
      }
      if (s->hot != hot) {
        s->hot = hot;
        FORCE_REDRAW(c);
      }
      if (c->sys->ldown && ctrl_isfocus(c)) {
        //printf("x = %d ptMouse.x = %d\n", x, c->sys->ptMouse.x);
        scrollview_set_pos(s->sc->si + 0, s->sc->si[0].nPos - (x - c->sys->ptMouse.x));
        scrollview_set_pos(s->sc->si + 1, s->sc->si[1].nPos - (y - c->sys->ptMouse.y));
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_PAINT:
    if (c->recalcsize) {
      imageview_update_scroll(s);
    }
    if (1) {
      IRECT rca, prc[14] = {0};
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      scrollview_draw(s->sc, c, im, prc);
      //rcb = iRECT2(s->sc->si[0].nPos, s->sc->si[1].nPos, s->sc->si[0].nPage, s->sc->si[1].nPage);
      imdraw_rect(ctx, &rcclip, *prc, _rgb(1, 1, 1), 0, 0, 0);
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
      imdraw_image_corner(ctx, prc, rca, s->im, 0, 0, 0);
      iRectIn(&rca, -1);
      imdraw_rect(ctx, prc, rca, 0, _rgb(0, 0, 0), 1, BDM_ALL);
      {
        font_t* fo = sys_font_load_win32(c->sys, "宋体|72");
        imdraw_char(ctx, prc, rca, 100, *(unsigned short*)"我", fo, _rgb(0, 0, 0), _rgba(1, 1, 1, 0.5));
      }
    }
    break;
  }
  return 0;
}
CC_INLINE int imageview_set(ctxageview_t* s, ctrl_t* li, ctrl_t* fa)
{
  ctrl_set(ctxageview_proc, s, s->c, li, WT_CHILD, 0);
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
  COLOR crBakH = myGetSysColor(CLR_3DLIGHT);
  COLOR crText = myGetSysColor(CLR_WINDOWTEXT);
  COLOR crBakW = myGetSysColor(CLR_WINDOW);
  COLOR crBorder = _RGB(100, 100, 100);
  COLOR _clr;
  int y, x;
  IRECT rcSelect = iRECT(rc.r, rc.b, 0, 0);
  IRECT rcLineH, rcRow, rcCell, rcLineV, rc1;
  IPOINT sela = {0}, selb = {0};
  rcRow = rcLineH = rc;
  if (5 == (io->HitValuetype & 5)) {
    sela.x = MIN(io->HitValue.x, s->end.x);
    sela.y = MIN(io->HitValue.y, s->end.y);
    selb.x = MAX(io->HitValue.x, s->end.x);
    selb.y = MAX(io->HitValue.y, s->end.y);
  }
  //imdraw_rect(ctx, rc, _RGB(255, 255, 255), 0, 0, 0);
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
      if (5 == (io->HitValuetype & 5)) {
        iny = y >= sela.y && y <= selb.y;
        inx = x >= sela.x && x <= selb.x;
        inall = 0 == io->HitValue.x && 0 == io->HitValue.y;
      }
      if (x > 0 && x < s->lt.x) {
        continue;
      }
      rcCell.r = rcCell.l + s->colw[x];
      rcLineH.r = rcCell.r + CELLBD;
      rc1 = rcCell;
      if (y > 0 && x > 0) {
        _clr = crBakW;
        if (5 == (io->HitValuetype & 5)) {
          crSelect = inall || (iny && inx) ? _RGB(0, 0, 255) : 0;
          crSelect = (x == io->HitValue.x && y == io->HitValue.y) ? _RGB(255, 255, 255) : crSelect;
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
      imdraw_rect(ctx, &rcclip, rc1, _clr, 0, 0, 0);
      if (!iRectIsEmpty(&rc1)) {
        //sendmsg(WM_NOTIFY, (WPARAM)c, TCN_SELCHANGE);
        if (y && x) {
          char buf[256];
          _snprintf(buf, 256, "x=%d, y=%d", x, y);
          imdraw_text(ctx, &rcclip, rcCell, buf, -1, s->c->fo, crText, 0, TF_VCENTER | TF_CENTER);
        }
      }
      rcCell.l = rcCell.r + CELLBD;
      rcLineV.l += s->colw[x], rcLineV.r = rcLineV.l + CELLBD;
      imdraw_rect(ctx, &rcclip, rcLineV, crBorder, 0, 0, 0);
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
    imdraw_rect(ctx, &rcclip, rcLineH, crBorder, 0, 0, 0);
    rcLineH.t += CELLBD;
    rcRow.t = rcRow.b + CELLBD;
  }
  if (5 == (io->HitValuetype & 5) && rcSelect.l < rcSelect.r && rcSelect.t < rcSelect.b) {
    rc1 = rcSelect;
    iRectOffset(&rc1, -2, -2, 2, 2);
    imdraw_rect(ctx, &rcclip, rc1, 0, _RGB(0, 0, 0), 3, BDM_ALL);
  }
  return 0;
}
static int gridctrl_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  gridctrl_t* s = (gridctrl_t*)c->x;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    //scrollbar_set(s->sc, c, c->sys);
    //gridctrl_set(s, 10, 10);
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_LBUTTONUP:
    if (s->down) {
      s->down = 0;
      s->adjusthit = iPOINT(0, 0);
      s->adjusthittype = 0;
      FORCE_REDRAW(c);
    }
    break;
  case MSG_LBUTTONDBLCLK:
  case MSG_LBUTTONDOWN:
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
        FORCE_REDRAW(c);
      }
      else if ((hit.x != io->HitValue.x || hit.y != io->HitValue.y) && (hitty & 5)) {
        io->HitValue = s->end = hit;
        io->HitValuetype = hitty;
        FORCE_REDRAW(c);
      }
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case MSG_MOUSEMOVE:
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
          FORCE_REDRAW(c);
        }
        else if ((io->HitValuetype & 5) && (hitty & 5)) {
          if (hitty & 4) {
            s->end.x = hit.x;
          }
          if (hitty & 1) {
            s->end.y = hit.y;
          }
          FORCE_REDRAW(c);
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
  case MSG_PAINT:
    if (1) {
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
static int dockctrl_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  dockctrl_t* s = (dockctrl_t*)c->x;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    //ctrl_set(button_proc, s->di[0].ctrl, c, c->sys);
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 200;
    mminfo->ptMinTrackSize.y = 200;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_LBUTTONUP:
    if (io->HitValue) {
      if (iPtInRect(&rc, x, y)) {
        //xSendCmd(XCMD_CLICK);
        FORCE_REDRAW(c);
      }
      ReleaseCapture();
      io->HitValue = 0;
    }
    break;
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      io->HitValue = 1;
      ctrl_set_capture(c);
      ctrl_close(s->di[0].win);
      FORCE_REDRAW(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case MSG_LBUTTONDBLCLK:
    if (iPtInRect(&rc, x, y)) {
      IRECT gprc = iRECT(rc.l, rc.t, rc.l + GRIPPER_W, rc.b);
      if (iPtInRect(&gprc, x, y)) {
        //win_setctrl(s->di[0].win, "dock", s->di[0].ctrl, 1, hwnd);
      }
    }
    break;
  case MSG_CANCELMODE:
  case MSG_MOUSEMOVE:
    if (s) {
      int hot = iPtInRect(&rc, x, y);
      if (s->hot != hot) {
        s->hot = hot;
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_PAINT:
    if (1) {
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      imdraw_fill(ctx, myGetSysColor(CLR_3DFACE));
      imdraw_gripper(ctx, &rcclip, rc, 2, 3, 1, 0);
    }
    break;
  }
  return 0;
}
#include "sys/clipboard.h"
//#include "editctrl1.inl"
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
    sztxt = imuimeasureText(s->c->fo, mi->text, -1, 0);
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
  io->HitValue = -1;
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
static int menuctrl_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags);
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
static int menuctrl_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  menuctrl_t* s = (menuctrl_t*)c->x;
  IRECT rc = c->rc, rcItem;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  int hit = -1;
  //printf("sys_count %d\n", sys_count(c->sys));
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    s->ishor = 1;
    c->isshow = SW_SHOWNOACTIVATE;
    io->HitValue = s->hot = -1;
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    if (0 == s->pos) {
      FREE(s->items);
    }
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = menuctrl_measure(s);
    mminfo->ptMinTrackSize.x = sz.w;
    mminfo->ptMinTrackSize.y = sz.h;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_KILLFOCUS:
    if (0 == s->pos) {
      //if (GetParent(hwnd)!=GetFocus())
      //SendMessage(hwnd, WM_CLOSE, 0, 0);
      io->HitValue = s->hot = -1;
      s->open0 = 0;
      menuctrl_hideall(s);
      FORCE_REDRAW(c);
    }
    break;
  case MSG_LBUTTONUP:
    if (io->HitValue >= 0) {
      if (iPtInRect(&rc, x, y)) {
        hit = menuctrl_hittest(s, x, y, &rcItem);
        //xSendCmd(XCMD_CLICK);
        {
          menuctrl_t* s0 = s - s->items[s->pos].ind;
          sendmsg(s0->c, s0->c->listener, WM_COMMAND, hit, (LPARAM)c);
        }
        FORCE_REDRAW(c);
        if (s->pos > 0) {
          menuctrl_hideall(s);
          io->HitValue = -1;
          s->open0 = 0;
        }
      }
    }
    break;
  case MSG_LBUTTONDBLCLK:
  case MSG_NCLBUTTONDOWN: {
    //IPOINT pt = MyScreenToClient(hwnd, x, y);
    //x = pt.x, y = pt.y;
  }
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      //printf("%x menu WM_LBUTTONDOWN %d\n", s, s->pos);
      hit = menuctrl_hittest(s, x, y, &rcItem);
      if (s->pos == 0) {
        s->open0 = !s->open0;
        if (!s->open0) {
          menuctrl_hideall(s);
          io->HitValue = -1;
        }
        else {
          if (hit != io->HitValue) {
            io->HitValue = hit;
            menuctrl_popup(s, hit, rcItem);
            //printf("menuctrl_hide11\n");
          }
        }
      }
      else {
        if (hit != io->HitValue) {
          io->HitValue = hit;
          menuctrl_popup(s, hit, rcItem);
        }
      }
      FORCE_REDRAW(c);
      ctrl_setfocus(c);
      return 1;
    }
    break;
  case MSG_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      hit = menuctrl_hittest(s, x, y, &rcItem);
      if (s->hot != hit) {
        if (hit >= 0 && io->HitValue != hit && (s->pos > 0 || s->open0)) {
          menuctrl_popup(s, hit, rcItem);
          io->HitValue = hit;
        }
        //printf("WM_MOUSEMOVE %08x\n", s);
        s->hot = hit;
        FORCE_REDRAW(c);
      }
    }
    else if (s->hot >= 0) {
      s->hot = -1;
      FORCE_REDRAW(c);
    }
    break;
  case MSG_PAINT:
    if (1) {
      //menuctrl_draw(s, rc, im);
      //int menuctrl_draw(menuctrl_t* s, IRECT rc, img_t* im)
      COLOR m_crBackground = myGetSysColor(CLR_MENU);
      COLOR m_crText = myGetSysColor(CLR_MENUTEXT);
      COLOR m_crTextSelected = _RGB(255, 255, 255);
      COLOR m_crLeft = _RGB(236, 233, 216);
      COLOR m_crSelectedBroder = _RGB(49, 106, 197);
      COLOR m_crSelectedFill = _RGB(200, 220, 253);
      COLOR m_crSelectedFill1 = myGetSysColor(CLR_ACTIVECAPTION);
      IRECT rcclip = c->rcclip;
      iRectOff0(&rc);
      iRectOff0(&rcclip);
      //printf("menuctrl WM_PAINT pos = %d\n", s->pos);
      if (0 == s->pos) {
        imdraw_fillrect(ctx, &rcclip, rc, m_crBackground);
      }
      else {
        ImDraw_edgeRectSys(ctx, &rcclip, rc, BDS_HIGH, BDM_ALL);
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
              if (i == io->HitValue) {
                ImDraw_edgeRectSys(ctx, &rcclip, rc1, BDS_LOW, BDM_ALL);
              }
              else if (i == s->hot) {
                ImDraw_edgeRectSys(ctx, &rcclip, rc1, BDS_UP, BDM_ALL);
              }
            }
            else {
              if (i == io->HitValue) {
                imdraw_fillrect(ctx, &rcclip, rc1, m_crSelectedFill1);
                crText = m_crTextSelected;
                crArow = _RGB(240, 240, 240);
              }
              else if (i == s->hot) {
                imdraw_fillrect(ctx, &rcclip, rc1, m_crSelectedFill1);
                crText = m_crTextSelected;
                crArow = _RGB(240, 240, 240);
              }
              if (i < (s->nitem - 1) && s->items[i + 1].ind > ind) {
                IRECT rcArow;
                iRectCutL(&rcItem, -RCH(&rcItem), 0, &rcArow);
                imdraw_menuimg(ctx, &rcclip, rcArow, IdArowRightLarge, crArow);
              }
            }
            rcItem.l += 4;
            imdraw_text(ctx, &rcclip, rcItem, text, -1, c->fo, crText, 0, TF_VCENTER);
          }
        }
      }
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
#endif
#if 0
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
static int combobox_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  combobox_t* s = (combobox_t*)c->x;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  enum {margin_x = 4};
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    listbox_set(s->m_listbox, c);
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    ISIZE sz = imuimeasureText(c->fo, "H", -1, 0);
    mminfo->ptMinTrackSize.x = sz.w + 4 + margin_x;
    mminfo->ptMinTrackSize.y = sz.h + 4;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_MEASUREITEM:
    if (sender == s->m_listbox->c) {
      ISIZE* ds = (ISIZE*)lParam;
      ISIZE sz = imuimeasureText(c->fo, "H", -1, 0);
      ds->h = sz.h + 4;
      ds->w = RCW(&rc);
    }
    break;
  case MSG_DRAWITEM:
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
      imdraw_text(ds->im, ds->cliprc, rcTxt, s->m_items[k], -1, c->fo, clrText, 0, TF_VCENTER);
    }
    break;
  case MSG_COMMAND:
    if (LBN_SELCHANGE == wParam && sender == s->m_listbox->c) {
      s->hot = 0;
      io->HitValue = 0;
      ctrl_close(s->m_listbox->c);
      FORCE_REDRAW(c);
    }
    break;
  case MSG_KILLFOCUS:
    if (1) {
      s->hot = 0;
      io->HitValue = 0;
      ctrl_close(s->m_listbox->c);
      FORCE_REDRAW(c);
    }
    break;
  case MSG_NCLBUTTONDOWN:
  case MSG_LBUTTONDBLCLK:
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->hot = 1;
      if (!io->HitValue) {
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
      io->HitValue = !io->HitValue;
      FORCE_REDRAW(c);
      ctrl_setfocus(s->m_listbox->c);
      return 1;
    }
    break;
  case MSG_PAINT:
    if (1) {
      COLOR clrText = _rgb(0, 0, 0);
      IRECT rcTxt, rcDown;
      int pushed = (c->sys->ldown && io->HitValue);
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      iRectCutR(&rc, -RCH(&rc), 0, &rcDown);
      rcTxt = rc;
      rcTxt.l += margin_x;
      imdraw_rect(ctx, &rcclip, rc, _rgb(1, 1, 1), _rgb(0, 0, 0), 0, 0);
      ImDraw_edgeRectSys(ctx, &rcclip, rcDown, pushed ? BDS_SUNKEN : BDS_HIGH, BDM_ALL);
      rcDown.l += 2 * pushed - 1;
      imdraw_menuimg(ctx, &rcclip, rcDown, IdArowDownLarge, _rgb(0, 0, 0));
      if (s->m_listbox->cursel >= 0 && s->m_listbox->cursel < s->nitem) {
        imdraw_text(ctx, &rcclip, rcTxt, s->m_items[s->m_listbox->cursel], -1, c->fo, clrText, 0, TF_VCENTER);
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
  io->HitValue = 0;
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
static int spline_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  spline_t* s = (spline_t*)c->x;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
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
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 20;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      for (i = 0; i < s->m_num_pnt; i++) {
        double xp = spline_calc_xp(s, i);
        double yp = spline_calc_yp(s, i);
        if (calc_distance(x - rc.l, y - rc.t, xp, yp) <= m_point_size + 1) {
          s->m_pdx = xp - x;
          s->m_pdy = yp - y;
          s->m_active_pnt = s->m_move_pnt = (i);
          FORCE_REDRAW(c);
        }
      }
    }
    break;
  case MSG_LBUTTONUP:
    if (s) {
      io->HitValue = 0;
      if (s->m_move_pnt >= 0) {
        s->m_move_pnt = -1;
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      if (s->m_move_pnt >= 0) {
        double xp = x + s->m_pdx;
        double yp = y + s->m_pdy;
        s->m_xp[s->m_move_pnt] = (s->m_move_pnt, (xp - s->m_xs1) / (s->m_xs2 - s->m_xs1));
        s->m_yp[s->m_move_pnt] = (s->m_move_pnt, (yp - s->m_ys1) / (s->m_ys2 - s->m_ys1));
        spline_update(s);
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_PAINT:
    if (1) {
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
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //XDRAW_BEG(ctx);
      be = m_border_extra;
      // Background Border
      imdraw_rect(ctx, &rcclip, rc, m_background_color, m_border_color, m_border_width, BDM_ALL);
      // Curve
      n = 0;
      pt[n++] = dPOINT(s->m_xs1, s->m_ys1 + (s->m_ys2 - s->m_ys1) * s->m_values[0]);
      for (i = 1; i < 256; i++) {
        x = (i) / 255.0, y = s->m_values[i];
        pt[n++] = dPOINT(s->m_xs1 + (s->m_xs2 - s->m_xs1) * x,
            s->m_ys1 + (s->m_ys2 - s->m_ys1) * y);
      }
      imdrawaa_poly(ctx, &rcclip, pt, n, 0, 0, m_curve_color, m_curve_width);
      // Inactive points
      for (i = 0; i < s->m_num_pnt; i++) {
        // Active point
        x = spline_calc_xp(s, i), y = spline_calc_yp(s, i);
        clr = i != s->m_active_pnt ? m_inactive_pnt_color : m_active_pnt_color;
        imdrawaa_ellipse(ctx, &rcclip, x, y, m_point_size, m_point_size, clr, 0, 0);
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
static int gamma_proc(ImGuiWindow* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  gamma_t* s = (gamma_t*)c->x;
  GETIMGUIIO();
  int x = io->MousePos.x, y = io->MousePos.y;
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    io->HitValue = 0;
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
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case MSG_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 120;
    mminfo->ptMinTrackSize.y = 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->m_p1_active = -1;
      if (calc_distance(x - rc.l, y - rc.t, s->m_p1.x, s->m_p1.y) <= s->m_point_size + 1) {
        s->m_mouse_point = 1;
        s->m_pdx = s->m_p1.x - x;
        s->m_pdy = s->m_p1.y - y;
        s->m_p1_active = 1;
        FORCE_REDRAW(c);
      }
      if (calc_distance(x - rc.l, y - rc.t, s->m_p2.x, s->m_p2.y) <= s->m_point_size + 1) {
        s->m_mouse_point = 2;
        s->m_pdx = s->m_p2.x - x;
        s->m_pdy = s->m_p2.y - y;
        s->m_p1_active = 2;
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_LBUTTONUP:
    if (s->m_mouse_point) {
      s->m_mouse_point = 0;
      FORCE_REDRAW(c);
    }
    break;
  case MSG_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      if (s->m_mouse_point == 1) {
        s->m_p1.x = x + s->m_pdx;
        s->m_p1.y = y + s->m_pdy;
        gamma_calc_values(s);
        gamma_calc_points(s);
        FORCE_REDRAW(c);
      }
      else if (s->m_mouse_point == 2) {
        s->m_p2.x = x + s->m_pdx;
        s->m_p2.y = y + s->m_pdy;
        gamma_calc_values(s);
        gamma_calc_points(s);
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_PAINT:
    if (c->recalcsize) {
      gamma_hitrect(s, rc);
      gamma_calc_points(s);
    }
    if (1) {
      //int i;
      DPOINT pt[1000];
      int lens[10], n;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //XDRAW_BEG(ctx);
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
        //imdraw_rect(ctx, &rcclip, rc, m_background_color, m_border_color, m_border_width);
        lens[0] = vcgen_rect(pt, 0, countof(pt), x1, y1, x2, y2);
        imdrawaa_poly_solid(ctx, &rcclip, pt, lens, 1, m_background_color);
        // Border
        lens[0] = vcgen_rect2(pt, x1, y1, x2, y2, s->m_border_extra, 0);
        lens[1] = vcgen_rect2(pt + lens[0], x1, y1, x2, y1 + s->m_text_height + bb, bb, 1);
        lens[2] = vcgen_rect2(pt + lens[0] + lens[1], x1, y1 + s->m_text_height - bb + bw, x2, y2, bb, 1);
        imdrawaa_poly_solid(ctx, &rcclip, pt, lens, 3, m_border_color);
        // Curve
        n = vcgen_gamma_spline(pt, s->m_gamma_spline, s->m_xs1, s->m_ys1, s->m_xs2, s->m_ys2);
        imdrawaa_poly_stroke_solid(ctx, &rcclip, pt, n, 0, m_curve_color, s->m_curve_width);
        // Grid
        xc = (s->m_xs1 + s->m_xs2) / 2;
        yc = (s->m_ys1 + s->m_ys2) / 2;
        lens[0] = vcgen_rect2(pt, xc - gw / 2, s->m_ys1, xc + gw / 2, s->m_ys2, 0, 0);
        lens[1] = vcgen_rect2(pt + lens[0], s->m_xs1, yc - gw / 2, s->m_xs2, yc + gw / 2, 0, 0);
        imdrawaa_poly_solid(ctx, &rcclip, pt, lens, 2, m_grid_color);
        lens[0] = vcgen_rect2(pt, s->m_xs1, s->m_p1.y - gw / 2, s->m_p1.x, s->m_p1.y + gw / 2, 0, 0);
        imdrawaa_poly_solid(ctx, &rcclip, pt, lens, 1, m_grid_color);
        lens[0] = vcgen_rect2(pt, s->m_p1.x - gw / 2, s->m_p1.y, s->m_p1.x + gw / 2, s->m_ys2, 0, 0);
        imdrawaa_poly_solid(ctx, &rcclip, pt, lens, 1, m_grid_color);
        lens[0] = vcgen_rect2(pt, s->m_p2.x, s->m_p2.y - gw / 2, s->m_xs2, s->m_p2.y + gw / 2, 0, 0);
        imdrawaa_poly_solid(ctx, &rcclip, pt, lens, 1, m_grid_color);
        lens[0] = vcgen_rect2(pt, s->m_p2.x - gw / 2, s->m_ys1, s->m_p2.x + gw / 2, s->m_p2.y, 0, 0);
        imdrawaa_poly_solid(ctx, &rcclip, pt, lens, 1, m_grid_color);
        // Point1
        clr = 1 == s->m_p1_active ? m_active_pnt_color : m_inactive_pnt_color;
        imdrawaa_ellipse(ctx, &rcclip, s->m_p1.x, s->m_p1.y, s->m_point_size, s->m_point_size, clr, 0, 0);
        // Point2
        clr = 2 == s->m_p1_active ? m_active_pnt_color : m_inactive_pnt_color;
        imdrawaa_ellipse(ctx, &rcclip, s->m_p2.x, s->m_p2.y, s->m_point_size, s->m_point_size, clr, 0, 0);
        // Text
        {
          char buf[256];
          double kx1, ky1, kx2, ky2;
          IRECT rc2 = iRECT(x1 + (int)bb, y1 + (int)bb, x2, y1 + (int)s->m_text_height);
          gamma_spline_get_values(s->m_gamma_spline, &kx1, &ky1, &kx2, &ky2);
          _snprintf(buf, 256, "%5.3f %5.3f %5.3f %5.3f", kx1, ky1, kx2, ky2);
          imdraw_text(ctx, &rcclip, rc2, buf, -1, c->fo, m_text_color, 0, TF_VCENTER | TF_CENTER);
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
  io->HitValue = 0;
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
  if (ismax) {
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
  ismax = !ismax;
  return 0;
}
#endif

