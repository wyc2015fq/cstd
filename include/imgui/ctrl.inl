#ifndef _CTRL_INL_
#define _CTRL_INL_

#define GETGUIIO2() SYSIO
#define force_redraw(c)
#define color_background_ctrl2(rc)    color_background_ctrl(rc, _RGB(222, 222, 222), _RGB(122, 122, 122), true, true)
static int color_background_ctrl(IRECT rc, COLOR clr1, COLOR clr2, int isv, BOOL redraw) {
  SYSIO;
  if (redraw) {
    FORCE_REDRAW(rc);
  }
  gcGradientRect2(g, rc, clr1, clr2, !isv);
  return 0;
}
static int label_ctrl(ctrl_t* c, const char* buf)
{
  SYSIO;
  IRECT rc = c->rc;
  const int fmtFlag = TF_VCENTER | TF_LEFT;
  //
  if (1) {
    ISIZE sz = measureText(buf, NULL, io->font, 0, fmtFlag, &c->rc);
    c->sz = iSIZE(sz.w + 6, sz.h + 6);
  }
  if (1) {
    gcSolidText(g, rc, buf, NULL, io->font, 0, fmtFlag, ColorBlack);
  }
  return 0;
}
static int labelfmt_ctrlV(ctrl_t* c, const char* fmt, va_list args)
{
  SYSIO;
  int ret = 0, len = cstr_format_lenv(fmt, args)+10;
  char* buf = (char*)pmalloc(len);
  _vsnprintf(buf, len, fmt, args);
  ret = label_ctrl(c, buf);
  pfree(buf);
  return ret;
}
static int fmtlabel_ctrl(ctrl_t* c, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  labelfmt_ctrlV(c, fmt, args);
  va_end(args);
  return 0;
}
static int progress_ctrl(ctrl_t* c, const char* label, double value) {
  SYSIO;
  IRECT rc = c->rc;
  int fmt = TF_CENTER|TF_VCENTER;
  if (label) {
    ISIZE sz = measureText("H", NULL, io->font, 0, fmt, &c->rc);
    c->sz = iSIZE(40, sz.h + 6);
  }
  value = BOUND(value, 0, 1);
  if (1) {
    IRECT rc2;
    gcEdgeRectSys(g, rc, BDS_SUNKEN, 15, RGBMUL(CLR_BTNFACE, 1.2));
    gcSolidText(g, rc, label, NULL, io->font, 0, fmt, ColorBlack);
    rc = iRectIn(rc, 3);
    rc2 = iRECT2(rc.l, rc.t, RCW(&rc)*value, RCH(&rc));
    gcSolidRect2(g, rc2, _rgba(0, 0, 1, 0.3));
  }
  return 0;
}
BOOL button_ctrl(ctrl_t* c, const char* label)
{
  SYSIO;
  int clicked = 0, hot = 0;
  int fmt = TF_CENTER | TF_VCENTER | TF_NoWrap;
  IRECT rc = c->rc;
  if (1) {
    ISIZE sz = measureText(label, NULL, io->font, 0, fmt, NULL);
    c->sz = iSIZE(sz.w + 6, sz.h + 6);
    //if (!calc_rect(sz.w + 6, sz.h + 6, la->layoutflags, &rc)) { return 0; }
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (hot) {
          //xSendCmd(XCMD_CLICK);
          clicked = 1;
        }
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        FORCE_REDRAW(c->rc);
        SETFOCUS(c);
      }
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        if (ISHOT(c) != hot) {
          SETHOT2(c, hot);
          FORCE_REDRAW(c->rc);
        }
      }
      break;
    }
  }
  if (1) {
    int hit = ISHIT(c);
    COLOR clrText = ColorBlack;
    int btn_style = 1;
    switch (btn_style) {
    case 1:
      if (1) {
        COLOR clrFace = myGetSysColor(CLR_BTNFACE);
        clrText = ColorBlack;
        gcEdgeRectSys(g, rc, hit ? BDS_SUNKEN : BDS_HIGH, 15, clrFace);
        if (hot) {
          gcEdgeRect1(g, rc, _rgba(1, 1, 1, 0.2), 0, 0);
        }
      }
      break;
    case 2:
      if (1) {
        //边框颜色
        COLOR m_border_color = _RGB(0, 0, 0);
        COLOR m_text_color = _RGB(0, 0, 0);
        COLOR clr2 = hit ? _RGB(100, 100, 100) : hot ? _RGB(140, 140, 140) : _RGB(200, 200, 200);
        COLOR crBack[] = {_RGB(255, 255, 255), clr2};
        //画按钮的外边框，它是一个半径为5的圆角矩形
        //draw_roundrectR( im, &rect, 5, 5, s->m_border_color );
        //iRectOff2(&rc1, -5, -5);
        //draw_roundrectR(g, &rcclip1, 5, 5, m_border_color, 0);
        gcEdgeRect1(g, rc, 0, m_border_color, 1);
        //draw_roundrect_aaR( im, &rect, 5, 5, s->m_border_color, 0 );
        if (1) {
          IRECT rc1 = iRectIn(rc, 1);
          //InflateRect(&rc1, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));
          gcGradientRect(g, rc1, 0, NULL, crBack, 2);
        }
      }
      break;
    }
    if (label) {
      rc.l += !!hit, rc.t += !!hit;
      gcSolidText(g, rc, label, NULL, io->font, 0, fmt, clrText);
    }
  }
  return clicked;
}

BOOL image_button_ctrl(ctrl_t* c, const char* label, COLOR clrbg, texture_t* tex, int flexfmt)
{
  SYSIO;
  int clicked = 0, hot = 0;
  int fmt = TF_CENTER | TF_VCENTER | TF_NoWrap;
  IRECT rc = c->rc;
  ISIZE sz;
  if (1) {
    sz = measureText(label, NULL, io->font, 0, fmt, NULL);
    c->sz = iSIZE(sz.w + 6, sz.h + 6);
    //if (!calc_rect(sz.w + 6, sz.h + 6, la->layoutflags, &rc)) { return 0; }
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (hot) {
          //xSendCmd(XCMD_CLICK);
          clicked = 1;
        }
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        FORCE_REDRAW(c->rc);
        SETFOCUS(c);
      }
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        if (ISHOT(c) != hot) {
          SETHOT2(c, hot);
          FORCE_REDRAW(c->rc);
        }
      }
      break;
    }
  }
  if (1) {
    int hit = ISHIT(c);
    int btn_style = 1;
    COLOR crBorder[3] = {_rgb_a(clrbg, 0.1), _rgb_a(clrbg, 0.7), _rgb_a(clrbg, 0.9)};
    ctrl_t cc[2];
    cc[0].sz = iSIZE(tex->w, tex->h);
    sz = measureText(label, NULL, io->font, 0, fmt, NULL);
    cc[1].sz = iSIZE(sz.w + 6, sz.h + 6);
    flexboxlay(rc, 2, cc, 2, flexfmt);
    {
      IRECT rcimg;
      COLOR clrbg_hit = RGBMUL(clrbg, 0.7);
      COLOR clr1 = hit ? _rgb_a(clrbg_hit, 0.7) : hot ? _rgb_a(clrbg, 0.7) : _rgb_a(clrbg, 0.5);
      gcEdgeRect2(g, rc, countof(crBorder), clr1, crBorder, crBorder, crBorder, crBorder);
      iRectAlign(cc[0].rc, tex->w, tex->h, TF_VCENTER|TF_CENTER, &rcimg);
      rcimg.l += !!hit;
      gcRectImageR(g, rcimg, tex, NULL);
    }
    if (label) {
      rc = cc[1].rc;
      rc.l += !!hit, rc.t += !!hit;
      gcSolidText(g, rc, label, NULL, io->font, 0, TF_VCENTER|TF_CENTER, _rgba(1, 1, 1, 0.8));
    }
  }
  return clicked;
}
static int cbox_ctrl(ctrl_t* c, const char* label, int* v)
{
  GETGUIIO2();
  int fmt = TF_VCENTER | TF_NoWrap, hot = 0, changed = 0;
  int m_text_height = 0;
  IRECT rc = c->rc;
  if (1) {
    ISIZE sz = measureText(label, NULL, io->font, 0, fmt, NULL);
    c->sz = iSIZE(sz.w + sz.h + 20, sz.h + 6);
    //if (!calc_rect(sz.w + sz.h + 20, sz.h + 6, io->layoutflags, &rc)) { return 0; }
    m_text_height = sz.h + 2;
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (ISHIT(v) && hot) {
        *v = !*v;
        changed = 1;
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(v);
      }
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        hot = 1;
      }
      break;
    }
  }
  if (1) {
    COLOR m_text_color = ColorBlack;
    COLOR m_active_color = 1 ? _rgba(0, 0.5, 0, 1) : _rgba(0, 0.8, 0, 1);
    IRECT rcCheck, rect, rcCheckIn;
    int style = 0;
    int m_bIsDrawOnGlass = 0;
    BOOL bEnabled = 1;
    int bHighlighted = io->hitvalue;
    COLOR clrFace = myGetSysColor(CLR_BTNFACE);
    rect = rc;
    iRectCutL(&rc, RCH(&rc), 2, &rcCheck);
    iRectAlign(rcCheck, m_text_height, m_text_height, TF_CENTER | TF_VCENTER, &rcCheck);
    rcCheckIn = iRectDeflate2(rcCheck, 4, 4);
    switch (style) {
    case 0:
      gcSolidRect2(g, rect, clrFace);
      gcEdgeRectSys(g, rcCheck, BDS_SUNKEN, 15, ColorWhite);
      if (*v) {
        gcEdgeRect1(g, rcCheckIn, m_active_color, 0, 0);
      }
      break;
    case 1:
      gcEdgeRect1(g, rcCheck, hot ? _rgba(1, 1, 1, 0.5) : ColorWhite, ColorBlack, 2);
      if (*v) {
        gcEdgeRect1(g, rcCheckIn, m_active_color, 0, 0);
      }
      break;
    }
    gcSolidText(g, rc, label, NULL, io->font, 0, fmt, m_text_color);
  }
  return changed;
}
//#define XDRAW_BEG(g) {img_t im2[1] = {0}; imsubref(g, rc, im2); rc = iRECT(0, 0, im2->w, im2->h); im = im2;
//#define XDRAW_END() }
static int rbox_ctrl_hittest(int x, int y, int nitem, int row_height)
{
  int i;
  int dy = row_height;
  for (i = 0; i < nitem; i++) {
    if (y > (i)*dy && y < (i + 1)*dy) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int rbox_ctrl0(ctrl_t* c, const char* label, int nitem, const str_t* items, int* pid)
{
  GETGUIIO2();
  int i, hit = *pid, hot = 0, changed = 0, hotid = -1;
  int fmt = TF_VCENTER;
  IRECT rc = c->rc;
  IRECT rcLabel = {0}, rect;
  int row_height = 0;
  *pid = hit = BOUND(hit, 0, nitem-1);
  if (1) {
    int w = 0, h = 0;
    ISIZE sz = measureText("H", NULL, io->font, 0, fmt, NULL);
    row_height = h = MAX(20, sz.h + 2);
    for (i = 0; i < nitem; ++i) {
      ISIZE sz2 = measureText(items[i].s, items[i].s+items[i].l, io->font, 0, fmt, NULL);
      w = MAX(w, sz2.w);
    }
    c->sz = iSIZE(w + h + 4, h * (nitem + !!label) + 2);
    //if (!calc_rect(x1, y1, flag, &rc)) { return 0; }
    if (label) {
      iRectCutT(&rc, h, 0, &rcLabel);
    }
  }
  rect = rc;
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c) && hot) {
        //hit = rbox_ctrl_hittest(x - rc.l, y - rc.t, nitem, row_height);
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        hit = rbox_ctrl_hittest(x - rc.l, y - rc.t, nitem, row_height);
        if (hit >= 0) {
          SETHIT(c);
          if (hit != *pid) {
            changed = 1;
            *pid = hit;
          }
        }
      }
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        hotid = rbox_ctrl_hittest(x - rc.l, y - rc.t, nitem, row_height);
        if (ISHOT(c) != hot) {
          SETHOT2(c, hot);
          FORCE_REDRAW(c->rc);
        }
      }
      break;
    }
  }
  if (1) {
    int i;
    int m_text_height = (int)measureText("H", NULL, io->font, 0, fmt, NULL).h;
    int dy = row_height;
    double m_text_thickness = (1.);
    COLOR m_text_color = (_rgb(0.0, 0.0, 0.0));
    COLOR m_inactive_color = (_rgb(0.0, 0.0, 0.0));
    COLOR m_active_color = (_rgb(0.4, 0.0, 0.0));
    COLOR m_background_color = (_rgb(1.0, 1.0, 0.9));
    COLOR m_border_color = (_rgb(0.0, 0.0, 0.0));
    IRECT rcItem, rcText, rc2 = rc;
    double r1 = m_text_height * 0.5; // 外圈半径
    double r2 = r1 * 0.4; // 内圆半径
    //gcClipRectR(g, rect);
    // Text
    {
      // background
      if (1) {
        m_active_color = ColorBlack;
        m_background_color = myGetSysColor(CLR_3DFACE);
        gcEdgeRectSys(g, rect, BDS_ETCHED, 15, m_background_color);
        //gcEdgeRect1(g, rc, m_background_color, m_border_color, 1);
      }
      else if (1) {
        gcEdgeRect1(g, rect, m_background_color, m_border_color, 1);
      }
    }
    if (label) {
      gcEdgeRectSys(g, rcLabel, BDS_SUNKEN, 15, CLR_3DFACE);
      gcSolidRect2(g, rcLabel, CLR_3DFACE);
      gcSolidText(g, rcLabel, label, NULL, io->font, 0, fmt, m_text_color);
    }
    for (i = 0; i < nitem; ++i) {
      if (iRectCutT(&rc2, dy, 0, &rcText) || 1) {
        int k = iRectCutL(&rcText, dy, 0, &rcItem);
        DPOINT pt1 = RCCENTERF(&rcItem);
        COLOR clr = (hotid == i) ? ColorGreen : m_text_color;
        // Inactive items
        gcSolidCircle(g, pt1.x, pt1.y, r1, ColorWhite, clr, m_text_thickness);
        if (*pid == i) {
          gcSolidCircle(g, pt1.x, pt1.y, r2, ColorGreen, 0, 0);
        }
        gcSolidText(g, rcText, items[i].s, items[i].s+items[i].l, io->font, 0, fmt, m_text_color);
      }
    }
    //gcClipReset(g);
  }
  return changed;
}
CC_INLINE int rbox_ctrl(ctrl_t* c, const char* label, const char* items1, int* pid)
{
  str_t items[100] = {0};
  int nitem = cstr_split_c(items1, 0, -1, '|', items, countof(items)), isbeg = 1, changed = 0;
  changed = rbox_ctrl0(c, label, nitem, items, pid);
  return changed;
}
CC_INLINE int rbox_ctrl2(ctrl_t* c, const char* label, int nitem, const char** items1, int* pid)
{
  str_t items[100] = {0};
  int i, j=0, changed = 0;
  for (i=0; i<nitem; ++i) {
    if (items1[i]) {
      items[j].s = (char*)items1[i];
      items[j].l = strlen(items1[i]);
      if (items[j].l>0) {
        ++j;
      }
    }
  }
  nitem = j;
  changed = rbox_ctrl0(c, label, nitem, items, pid);
  return changed;
}
CC_INLINE int rbox_ctrl3(ctrl_t* c, const char* label, int nitem, const void* items1, int step, int* pid)
{
  str_t items[100] = {0};
  int i, j=0, changed = 0;
  char* items2 = (char*)items1;
  for (i=0; i<nitem; ++i) {
    items[j].s = items2;
    items[j].l = strlen(items2);
    if (items[j].l>0) {
      ++j;
    }
    items2 += step;
  }
  nitem = j;
  changed = rbox_ctrl0(c, label, nitem, items, pid);
  return changed;
}
int slider_float_impl(ctrl_t* c, const char* lable_fmt, float* v, float v_min, float v_max, BOOL is_int)
{
  GETGUIIO2();
  IRECT rc = c->rc;
  IRECT rc1 = rc, grabrc;
  static float v2 = 0;
  float ss;
  int h, w, changed = 0, hot = 0, hit = 0;
  static IPOINT downpt = {0};
  int fmt = TF_VCENTER | TF_NoWrap;
  if (1) {
    ISIZE sz = measureText(lable_fmt, NULL, io->font, 0, fmt, NULL);
    c->sz = iSIZE(sz.w + 6, sz.h + 6);
  }
  rc1 = iRectIn(rc1, 2);
  h = RCH(&rc1), w = (int)(h * 0.75);
  *v = BOUND(*v, v_min, v_max);
  ss = (RCW(&rc1) - w) / (v_max - v_min);
  grabrc = iRECT2(rc1.l + (int)((*v - v_min) * ss), rc1.t, w, h);
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    hit = iPtInRect(&grabrc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETFOCUS(c);
        //if (SETHITV() { }
        if (!hit) {
          *v = v_min + (x - (w / 2) - rc1.l) / ss;
          changed = 1;
        } else {
          SETHIT(c);
          FORCE_REDRAW(c->rc);
          downpt = iPOINT(x, y);
          v2 = *v;
        }
        io->msg = MSG_NOP;
      }
      break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        *v = v2 + (x - downpt.x) / ss;
        changed = 1;
      }
      if (hot) {
        SETHOT(c);
      }
      break;
    }
  }
  if (changed) {
    *v = BOUND(*v, v_min, v_max);
    if (is_int) {
      *v = (int)(*v+0.5);
    }
    grabrc = iRECT2(rc1.l + (int)((*v - v_min) * ss), rc1.t, w, h);
  }
  {
    char buf[64];
    gcEdgeRectSys(g, rc, BDS_ETCHED, 15, Col_FrameBg);
    if (is_int) {
      _snprintf(buf, 64, lable_fmt, (int)*v);
    }
    else {
      _snprintf(buf, 64, lable_fmt, *v);
    }
    //imuiRenderFrame(g, grabrc, imuiGetColorU32(changed ? Col_SliderGrabActive : ISHOT(c) ? Col_SliderGrabHovered : Col_SliderGrab), true, style->GrabRounding, 0);
    gcEdgeRect1(g, grabrc, (changed ? Col_SliderGrabActive : ISHOT(c) ? Col_SliderGrabHovered : Col_SliderGrab), (Col_Border), 1);
    gcSolidText(g, rc1, buf, NULL, io->font, 0, fmt, ColorBlack);
    if (is_int) {
      int i, len = (int)(v_max - v_min+0.1);
      if (len*2 < RCW(&rc1)) {
        for (i=0; i<=len; ++i) {
          IRECT rc2 = iRECT2(rc1.l + (int)((i) * ss) + RCW(&grabrc)/2, rc1.b-5, 1, 5);
          gcSolidRect2(g, rc2, ColorBlack);
        }
      }
    }
  }
  return changed;
}
int slider_float_ctrl(ctrl_t* c, const char* lable_fmt, float* v, float v_min, float v_max)
{
  return slider_float_impl(c, lable_fmt, v, v_min, v_max, 0);
}
int slider_int_ctrl(ctrl_t* c, const char* lable_fmt, int* v, float v_min, float v_max)
{
  float fv = *v;
  int ret = slider_float_impl(c, lable_fmt, &fv, v_min, v_max, 1);
  *v = (int)fv;
  return ret;
}
CC_INLINE int groupbox_calcrect(IRECT rc, const char* text, const char* text_end, font_t* font, IRECT* outrc)
{
  int d = measureText(text, text_end, font, 0, 0, NULL).h;
  *outrc = iRectOffset(rc, 4, d, -4, -4);
  return 0;
}
static int groupbox_draw(gc_t* g, IRECT rc, const char* text, const char* text_end, BorderStyle bds, COLOR crBk, font_t* font, int fmt)
{
  COLOR clr = crBk;
  //DrawSunken3dFrame(hDC, rect3d);
  if (crBk) {
    //imdraw_fillrect(g, &rcclip, rc, crBk);
  }
  if (1) {
    IRECT rc1 = rc, rc2;
    ISIZE sz = measureText(text, text_end, font, 0, 0, NULL);
    int d = sz.h / 2;
    rc1.t += d;
    iRectCutL(&rc1, d + 2, 0, &rc2);
    gcEdgeRectSys(g, rc2, bds, BDM_L | BDM_T | BDM_B, 0);
    iRectCutL(&rc1, sz.w + 4, 0, &rc2);
    rc2.t -= d;
    gcEdgeRectSys(g, rc2, bds, BDM_B, 0);
    gcEdgeRectSys(g, rc1, bds, BDM_R | BDM_T | BDM_B, 0);
    if (text) {
      gcSolidText(g, rc2, text, text_end, font, 0, fmt, ColorBlack);
    }
  }
  return 0;
}
static int groupbox_ctrl(ctrl_t* c, const char* label, ctrl_t* cc)
{
  GETGUIIO2();
  IRECT rc = c->rc;
  int ret = 1;
  int fmt = TF_CENTER;
  if (1) {
    ISIZE sz = measureText(label, NULL, io->font, 0, fmt, NULL);
    c->sz = iSIZE(sz.w + 6, sz.h + 20);
  }
  groupbox_calcrect(rc, label, NULL, io->font, &cc->rc);
  if (1) {
    //int fwSizeType = nChar; // resizing flag
    BorderStyle bds = BDS_FRAME;
    COLOR clr = ColorWhite;
    if (1) {
      bds = BDS_ETCHED;
      clr = myGetSysColor(CLR_3DFACE);
    }
    else {
      bds = BDS_SIMPLE;
      clr = ColorWhite;
    }
    groupbox_draw(g, rc, label, NULL, bds, clr, io->font, fmt);
  }
  return ret;
}
int tooltip_ctrl(const char* label)
{
  SYSIO;
  window_t* win = io->cur_host->tooltip;
  int fmt = TF_VCENTER | TF_CENTER;
  ISIZE sz = measureText(label, NULL, io->font, 0, fmt, NULL);
  IRECT rc = iRECT2(x + 20, y + 20, sz.w + 6, sz.h + 10);
  win->rc = rc;
  if (1) {
    COLOR clr = myGetSysColor(CLR_INFOBK);
    g = win->g;
    gcEdgeRect1(g, rc, clr, _RGB(0, 0, 0), 1);
    gcSolidText(g, rc, label, NULL, io->font, 0, fmt, ColorBlack);
    //nHeight = dc.DrawText(m_strTitle, rect, 0 | TF_LEFT | TF_EDITCONTROL | TF_NOPREFIX | TF_WORDBREAK );
  }
  return 0;
}
int listheader_hittest(IRECT rc, int n, const int* width, int gap, int x, int y)
{
  int i, w = rc.l, hit = -1, gap2 = gap / 2;
  int height = RCH(&rc);
  IRECT rc1;
  w = rc.l;
  for (i = 0; i < n; ++i) {
    w += width[i];
    rc1 = iRECT2(w - gap2, rc.t, gap, height);
    if (iPtInRect(&rc1, x, y)) {
      return (i + 1) << 16;
    }
  }
  w = rc.l;
  for (i = 0; i < n; ++i) {
    rc1 = iRECT2(w, rc.t, width[i], height);
    if (iPtInRect(&rc1, x, y)) {
      hit = i;
      return i;
    }
    w += width[i];
  }
  return -1;
}
int drag_hittest(IRECT rc, int n, const int* v, int gap, int x, int y)
{
  int i, hit = -1, gap2 = gap / 2, w;
  int height = RCH(&rc);
  IRECT rc1;
  w = rc.l;
  for (i = 0; i < n; ++i) {
    w += v[i];
    rc1 = iRECT2(w - gap2, rc.t, gap, height);
    if (iPtInRect(&rc1, x, y)) {
      return i;
    }
  }
  return -1;
}
int drag(ctrl_t* c, IRECT rc, int gap, int count, int* v, int x1, int y1)
{
  SYSIO;
  int i, changed = 0, hit = -1;
  static int v0;
  static IPOINT last_lmouse_down_pos = {0};
  switch (io->msg) {
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y) && (hit = drag_hittest(rc, count, v, gap, x1, y1)) >= 0) {
      SETHIT(c);
      SETHITV(hit);
      FORCE_REDRAW(c->rc);
      last_lmouse_down_pos = iPOINT(x, y);
      //v = (io->MousePos.x - (gap/2) - prc[i].l)/ss;
      v0 = v[hit];
      changed = 1;
      io->msg = MSG_NOP;
    }
    break;
  case MSG_MOUSEMOVE:
    i = io->hitvalue;
    if (ISHIT(c) && i >= 0 && i < count) {
      int off = 0;
      int v1 = 0;
      int v2 = RCW(&rc);
      int j;
      for (j = 0; j < i - 1; ++j) {
        v1 += v[j];
      }
      {
        int x2 = BOUND(x, rc.l + v1 + gap, rc.l + v2 - gap);
        off = x2 - last_lmouse_down_pos.x;
      }
      v[i] = v0 + off;
      changed = 1;
    }
    break;
  }
  return changed;
}
int header(ctrl_t* c, int n, const char** strs, int* width, int* psel)
{
  GETGUIIO2();
  static int sel = 0;
  int w, i, fmt = TF_CENTER | TF_VCENTER | TF_NoWrap, gap = 6, hotid = -1;
  int hot = 0, changed = 0, hit = -1;
  IRECT rc = c->rc;
  ISIZE sz = measureText("H", NULL, io->font, 0, fmt, NULL);
  int height = sz.h + 6;
  int ret = 0;
  int* width0 = NULL;
  if (NULL==width) {
    MYREALLOC(width, n);
  }
  if (1) {
    int i;
    sz.w = 0;
    for (i = 0; i < n; ++i) {
      if (width[i] <= 1) {
        ISIZE sz1 = measureText(strs[i], NULL, io->font, 0, fmt, NULL);
        width[i] = sz1.w + 8;
      }
      sz.w += width[i];
    }
    c->sz = iSIZE(sz.w, sz.h + 6);
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      changed = drag(c, rc, gap, n, width, x, y);
      hotid = listheader_hittest(rc, n, width, gap, x, y);
    }
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        io->hitvalue = hotid;
        sel = hotid;
      }
      break;
    case MSG_MOUSEMOVE:
      //set_cursor("sb_v_double_arrow");
      break;
    }
  }
  if (1) {
    COLOR clrFace = myGetSysColor(CLR_BTNFACE);
    COLOR clrText = ColorBlack;
    COLOR clrGap = _RGBA(255, 0, 0, 50);
    IRECT rc1;
    int hotgapid = (hotid >> 16) - 1;
    int gap2 = gap / 2;
    w = rc.l;
    gcClipRectR(g, rc);
    gcSolidRect2(g, rc, clrFace);
    if (psel) {
      *psel = sel;
      for (i = 0; i < n; ++i) {
        rc1 = iRECT2(w, rc.t, width[i], RCH(&rc));
        //rc1.t+=(sel!=i)*2;
        gcEdgeRectSys(g, rc1, BDS_HIGH, 15, sel == i ? ColorMul(clrFace, 1.3) : clrFace);
        gcSolidText(g, rc1, strs[i], NULL, io->font, 0, fmt, clrText);
        w += width[i];
      }
    }
    else {
      for (i = 0; i < n; ++i) {
        rc1 = iRECT2(w, rc.t, width[i], RCH(&rc));
        gcEdgeRectSys(g, rc1, (ISHIT(c) && io->hitvalue == i) ? BDS_SUNKEN : BDS_HIGH, 15, clrFace);
        gcSolidText(g, rc1, strs[i], NULL, io->font, 0, fmt, clrText);
        w += width[i];
      }
    }
    w = rc.l;
    for (i = 0; i < n; ++i) {
      w += width[i];
      if (hotgapid == i) {
        rc1 = iRECT2(w - gap2, rc.t, gap, height);
        gcSolidRect2(g, rc1, clrGap);
        break;
      }
    }
    gcClipReset(g);
  }
  FREE(width0);
  return changed;
}
int listheader(ctrl_t* c, int n, const char** strs, int* width)
{
  return header(c, n, strs, width, NULL);
}
int tab_ctrl(ctrl_t* c, int n, const char** strs, int* width, int* psel)
{
  return header(c, n, strs, width, psel);
}
typedef enum {
  Drag_x = 1,
  Drag_y = 2,
} DragFlags;
int dragrect(void* c, IRECT grabrc, IRECT boundrc, int flag, int wheel, IRECT* prc)
{
  SYSIO;
  static IPOINT pt;
  static IPOINT lastMouseDownPos;
  IRECT rc = grabrc;
  int changed = 0, hot = 0;
  int w = RCW(&rc);
  int h = RCH(&rc);
  int x_min = boundrc.l, x_max = boundrc.r - w, y_min = boundrc.t, y_max = boundrc.b - h;
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        FORCE_REDRAW(rc);
        //SETFOCUS(c);
        //if (SETHITV(iPtInRect2(&grabrc, io->MousePos))) {}
        pt.x = rc.l;
        pt.y = rc.t;
        lastMouseDownPos = iPOINT(x, y);
        io->msg = MSG_NOP;
      }
      else {
        if (flag & Drag_x && grabrc.t <= y && y < grabrc.b) {
          IRECT rc_x = iRECT(x_min, rc.t, x_max + w, rc.b);
          if (iPtInRect(&rc_x, x, y)) {
            rc.l = x - w / 2;
            changed = 1;
          }
        }
        if (flag & Drag_y && grabrc.l <= x && x < grabrc.r) {
          IRECT rc_y = iRECT(rc.l, y_min, rc.r, y_max + h);
          if (iPtInRect(&rc_y, x, y)) {
            rc.t = y - h / 2;
            changed = 1;
          }
        }
      }
      break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        if (flag & Drag_x) {
          rc.l = pt.x + (x - lastMouseDownPos.x);
          changed = 1;
        }
        if (flag & Drag_y) {
          rc.t = pt.y + (y - lastMouseDownPos.y);
          changed = 1;
        }
      }
      break;
    case MSG_MOUSEWHEEL:
      if (wheel) {
        if (flag & Drag_y) {
          rc.t -= (int)(mouse->wheelDelta * wheel);
          changed = 1;
        }
      }
      break;
    }
  }
  if (1) {
    if (flag & Drag_x) {
      rc.l = BOUND(rc.l, x_min, x_max);
      rc.r = rc.l + w;
    }
    if (flag & Drag_y) {
      rc.t = BOUND(rc.t, y_min, y_max);
      rc.b = rc.t + h;
    }
  }
  *prc = rc;
  {
    //const ImGuiStyle* style = GetImGuiStyle();
    //COLOR clrFace = myGetSysColor(CLR_BTNFACE);
    //gcEdgeRectSys(g, rc, BDS_HIGH, 15, clrFace);
  }
  return changed;
}
int scrollbar_ctrl(IRECT rc, int client_h, int* client_off_y, IRECT* pclient)
{
  GETGUIIO2();
  IRECT rc1 = rc, grabrc, rc2;
  static float v2 = 0;
  int changed = 0, hot = 0;
  int ch = client_h, h = RCH(&rc);
  void* c = pclient;
  enum {grab_w = 16};
  *pclient = rc;
  if (ch > h && h > 0) {
    int grab_h = MAX((h * h / ch), 10);
    int h2 = h - grab_h, ch2 = ch - h;
    int grab_y = (ch2 - 1 - *client_off_y * h2) / ch2;
    iRectCutR(&rc1, -grab_w, 0, &rc2);
    grabrc = iRECT2(rc2.l, rc2.t + grab_y, grab_w, grab_h);
    //if (iPtInRect(&rc2, x, y))
    {
      int wheel = grab_h * 100 / h;
      IRECT boundrc = iRECT(0, 0, rc2.t, rc2.b - grab_h);
      wheel = MAX(wheel, 2);
      wheel = iPtInRect(&rc, x, y) ? wheel : 0;
      hot = iPtInRect(&grabrc, x, y);
      changed = dragrect(c, grabrc, boundrc, Drag_y, wheel, &grabrc);
    }
    if (changed) {
      *client_off_y = -(((grabrc.t - rc2.t) * ch2 + h2 - 1) / h2);
    }
    *client_off_y = BOUND(*client_off_y, h - ch, 0);
    {
      COLOR clrFace = myGetSysColor(CLR_BTNFACE);
      IRECT client = iRECT2(rc.l, rc.t + *client_off_y, RCW(&rc1), ch);
      *pclient = client;
      //gcEdgeRect1(g, rc, 0, ColorBlack, 1);
      //gcEdgeRect1(g, client, 0, ColorRed, 1);
      gcEdgeRectSys(g, rc2, BDS_LOW, 15, clrFace);
      gcEdgeRectSys(g, grabrc, BDS_HIGH, 15, clrFace);
      if (hot) {
        gcEdgeRect1(g, grabrc, _rgba(1, 1, 1, 0.1), 0, 0);
      }
    }
  }
  return changed;
}


enum {
  SBHT_LINE_0,
    SBHT_PAGE_0,
    SBHT_THUMB,
    SBHT_PAGE_1,
    SBHT_LINE_1,
    SBHT_MAX,
};

CC_INLINE int scroll_calcrect_impl(int nMin, int nMax, int nPage, int nPos, const int* pt0, const int* pt1, int isv, int line_w, int thumb_min_w, int* opt0, int* opt1, int step)
{
  int i, len = 1 + nPage + nMax - nMin, a, b;
  int c = 0, d = 0;
  int x = !!isv;
  int y = !isv;
  thumb_min_w = (len > 0) ? thumb_min_w : 0;
  a = pt0[x] + line_w;
  b = pt1[x] - line_w;
  if (len > 0) {
    double ll = b - a - thumb_min_w;
    nPos = BOUND(nPos, nMin, nMax);
    c = a + (nPos-nMin) * ll / len;
    d = b + (nPos-nMax) * ll / len;
  }
  {
    int xarr[SBHT_MAX+1] = {pt0[x], a, c, d, b, pt1[x]};
    for (i=0; i<SBHT_MAX; ++i) {
      opt0[i*step + x] = xarr[i];
      opt1[i*step + x] = xarr[i+1];
      opt0[i*step + y] = pt0[y];
      opt1[i*step + y] = pt1[y];
    }
  }
  return 0;
}
CC_INLINE int scroll_calcrect(int nMin, int nMax, int nPage, int nPos, IRECT rc, int isv, int line_w, int thumb_min_w, IRECT* prc) {
  return scroll_calcrect_impl(nMin, nMax, nPage, nPos, &rc.l, &rc.r, isv, line_w, thumb_min_w, &prc->l, &prc->r, 4);
}

//nMin 滚动范围最小值
//nMax 滚动范围最大值
//nPage 页尺寸，用来确定比例滚动框的大小
//nPos 滚动框的位置
//nTrackPos 拖动时滚动框的位置，该参数只能查询，不能设置。
int scroll_ctrl(ctrl_t* c, int nMin, int nMax, int nPage, int* pPos, int* pTrackPos, bool isv, bool hasbtn, int wheel, IRECT* prcclient) {
  SYSIO;
  int changed = 0, hot = 0, hotid = -1, off = 0;
  int fmt = TF_CENTER | TF_VCENTER | TF_NoWrap;
  IRECT rc = c->rc;
  int tmptt = iRectCutR(prcclient, -16, 0, &rc);
  IRECT rcs[SBHT_MAX] = {0};
  int thumb_min_w = 10;
  int max_w = isv?RCW(&rc):RCH(&rc);
  int max_h = isv?RCH(&rc):RCW(&rc);
  int thumb_sz = MIN(max_w, max_h/3);
  int line_w = hasbtn ? thumb_sz : 0;
  int line_w2 = line_w * 2;
  static IPOINT downpt;
  static IRECT thumbrc;
  static int nPos = 0;
  int len = 1 + nPage + nMax - nMin;
  int ll = max_h - line_w2 - thumb_min_w;
  *pPos = BOUND(*pPos, nMin, nMax);
  if (1) {
    int sz[] = {thumb_sz, hasbtn ? (thumb_sz*3) : thumb_sz};
    //c->sz = iSIZE(sz[!!isv], sz[!isv]);
  }
  scroll_calcrect(nMin, nMax, nPage, *pTrackPos, rc, isv, line_w, thumb_min_w, rcs);
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      hotid = iPtInRects(SBHT_MAX, rcs, x, y);
    }
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (MOUSE_LBUTTON==mouse->code && ISHIT(c)) {
        if (*pPos != *pTrackPos) {
          changed = 1;
        }
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        FORCE_REDRAW(c->rc);
        SETFOCUS(c);
        downpt = iPOINT(x, y);
        thumbrc = rcs[SBHT_THUMB];
        if (hotid>=0) {
          SETHITV(hotid);
        }
        nPos = *pPos;
      }
      break;
    case MSG_MOUSEMOVE:
      break;
    case MSG_MOUSEWHEEL:
      if (1) {
        *pTrackPos += -nPage * wheel * (int)(mouse->wheelDelta);
      }
      break;
    }
  }
  if (ISHIT(c)) {
    if (SBHT_THUMB == io->hitvalue) {
      rcs[SBHT_THUMB] = thumbrc;
      if (isv) {
        off = y - downpt.y;
        off = BOUND(off, rcs[SBHT_PAGE_0].t - thumbrc.t, rcs[SBHT_PAGE_1].b - thumbrc.b);
        rcs[SBHT_THUMB] = iRectOff2(rcs[SBHT_THUMB], 0, off);
      } else {
        off = x - downpt.x;
        off = BOUND(off, rcs[SBHT_PAGE_0].l - thumbrc.l, rcs[SBHT_PAGE_1].r - thumbrc.r);
        rcs[SBHT_THUMB] = iRectOff2(rcs[SBHT_THUMB], off, 0);
      }
      if (off)  {
        *pTrackPos = nPos + (off * len + CC_SIGN(off)*(ll/2)) / ll;
      }
    }
    if (iPtInRect(&rcs[SBHT_PAGE_0], x, y)) {
      *pTrackPos += -1 * nPage;
    }
    if (iPtInRect(&rcs[SBHT_PAGE_1], x, y)) {
      *pTrackPos += 1 * nPage;
    }
    if (iPtInRect(&rcs[SBHT_LINE_0], x, y)) {
      *pTrackPos += -1;
    }
    if (iPtInRect(&rcs[SBHT_LINE_1], x, y)) {
      *pTrackPos += 1;
    }
  }
  *pTrackPos = BOUND(*pTrackPos, nMin, nMax);
  if (1) {
    int hit = ISHIT(c);
    COLOR cr3dFace = CLR_3DFACE;
    COLOR clrs[] = {_RGB(255, 255, 255), cr3dFace};
    int nState[4] = {IdArowLeftLarge, IdArowRightLarge, IdArowUpLarge, IdArowDownLarge, };
    int i;
    gcEdgeRectSys(g, rc, BDS_LOW, 15, CLR_BTNFACE);
    for (i=0; i<SBHT_MAX; ++i) {
      COLOR clr = CLR_BTNFACE;
      if (i==io->hitvalue) {
        if (hit) {
          clr = RGBMUL(clr, 0.75);
        } else if (hot) {
          clr = RGBMUL(clr, 0.9);
        }
      }
      if (i&1) {
        //clr = RGBMUL(clr, 0.5);
        if (hit && i==io->hitvalue) {
          gcSolidRect2(g, rcs[i], _rgba(0, 0, 0, 0.5));
        }
      } else {
        gcEdgeRectSys(g, rcs[i], BDS_HIGH, 15, clr);
      }
    }
  }
  return changed;
}


int strlist_ctrl(ctrl_t* c, const vstr_t* sv, int ncol, int* width, IPOINT* pos, int* pSelItem, int* pHotItem)
{
  GETGUIIO2();
  int i, fmt = TF_VCENTER | TF_NoWrap, hot = 0, changed = 0, hit = -1;
  int n = sv->n;
  IRECT rc = c->rc, rcclient = {0};
  IRECT rc1;
  ISIZE sz = measureText("H", NULL, io->font, 0, fmt, NULL);
  int height = sz.h + 6;
  int client_h = height * n;
  int client_off_y[1];
  int hot1 = iPtInRect(&rc, x, y);
  if (1) {
    c->sz = iSIZE(sz.w + 20, 100);
  }
  //scrollbar_ctrl(rc, client_h, client_off_y, &rcclient);
  if (1) {
    int page = RCH(&rc);
    int nMax = height*sv->n-page;
    rcclient = rc;
    if (nMax>0) {
      if (scroll_ctrl(c, 0, nMax, page, &pos->y, &pos->y, 1, 0, hot1, &rcclient)) {
      }
    }
    *client_off_y = -pos->y;
  } else {
    int page = RCH(&rc)/height;
    rcclient = rc;
    if (scroll_ctrl(c, 0, sv->n-page, page, &pos->y, &pos->y, 1, 0, hot1, &rcclient)) {
    }
    *client_off_y = -height * pos->y;
  }
  rcclient = iRectInter(rcclient, rc);
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rcclient, x, y);
    *pHotItem = hot ? ((y - (rcclient.t + *client_off_y)) / height) : -1;
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        hit = *pHotItem;
        if (hit != *pSelItem) {
          *pSelItem = hit;
          changed = 1;
        }
      }
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        SETHOT(c);
      }
      break;
    }
  }
  if (1) {
    int n2;
    COLOR clrFace = myGetSysColor(CLR_BTNFACE);
    COLOR clrGrid = _RGB(100, 100, 100);
    vstr_t sv2[1] = {0};
    IRECT rc2;
    int j, w = 0;
    gcEdgeRect1(g, rcclient, ColorWhite, 0, 0);
    //gcEdgeRectSys(g, rect, BDS_SUNKEN, 15, ColorWhite);
    gcClipRectR(g, rcclient);
    i = (-*client_off_y) / height;
    n2 = 1 + (rcclient.b - rcclient.t - *client_off_y) / height;
    n2 = BOUND(n2, 0, n);
    for (; i < n2; ++i) {
      COLOR clrText = ColorBlack;
      rc1 = iRECT2(rcclient.l, rcclient.t + i * height + *client_off_y, RCW(&rcclient), height);
      if (i == *pSelItem) {
        gcSolidRect2(g, rc1, _RGBA(0, 0, 255, 188));
        clrText = ColorWhite;
      }
      else if (i == *pHotItem) {
        gcSolidRect2(g, rc1, _RGBA(0, 0, 255, 90));
        clrText = ColorWhite;
      }
      if (0) {
        gcSolidText(g, rc1, sv->v[i].s, NULL, io->font, 0, fmt, clrText);
      }
      else {
        vstr_split_str(sv2, sv->v[i], "|", " \n\t", 0);
        w = 0;
        for (j = 0; j < MIN(sv2->n, ncol); ++j) {
          ISIZE sz = measureText(sv2->v[j].s, NULL, io->font, 0, fmt, NULL);
          sz.w += 20;
          width[j] = MAX(width[j], sz.w);
          rc2 = iRECT2(rc1.l + w, rc1.t, width[j], RCH(&rc1));
          gcSolidText(g, rc2, sv2->v[j].s, NULL, io->font, 0, fmt | TF_CENTER, clrText);
          w += width[j];
        }
      }
      rc1.t = rc1.b - 1;
      gcSolidRect2(g, rc1, clrGrid);
    }
    w = 0;
    for (j = 0; j < ncol; ++j) {
      w += width[j];
      rc2 = iRECT2(rcclient.l + w, rcclient.t, 1, RCH(&rcclient));
      gcSolidRect2(g, rc2, clrGrid);
    }
    gcClipReset(g);
    vstr_free(sv2);
  }
  return changed;
}
static int listbox_ctrl(ctrl_t* c, const vstr_t* sv, IPOINT* pos, int* pSelItem, int* pHotItem)
{
  int width[1] = {1};
  return strlist_ctrl(c, sv, 1, width, pos, pSelItem, pHotItem);
}
void window_remove(window_t* c)
{
  // remove
  if (!c->parent) {
    return ;
  }
  if (!c->parent->child) {
    return ;
  }
  if (!c->next) {
    return ;
  }
  if (c->parent->child == c) {
    c->parent->child = c->next;
    c->next->prev = NULL;
  }
  if (c->prev) {
    c->prev->next = c->next;
  }
  if (c->next) {
    c->next->prev = c->prev;
  }
}
void window_addtolast(window_t* c)
{
  // remove
  if (!c->parent) {
    return ;
  }
  if (!c->parent->child) {
    return ;
  }
  if (!c->next) {
    return ;
  }
  if (c->parent->child == c) {
    c->parent->child = c->next;
    c->next->prev = NULL;
  }
  if (c->prev) {
    c->prev->next = c->next;
  }
  if (c->next) {
    c->next->prev = c->prev;
  }
  // add
  if (c->parent) {
    window_t* last = c->parent->child;
    for (; last->next; last = last->next) {
    }
    last->next = c;
    c->prev = last;
    c->next = NULL;
  }
}

typedef enum {
  // Default: 0
  WF_NoTitleBar = 1 << 0, // Disable title-bar
  WF_NoResize = 1 << 1, // Disable user resizing with the lower-right grip
  WF_NoMove = 1 << 2, // Disable user moving the window
  WF_NoScrollbar = 1 << 3, // Disable scrollbars (window can still scroll with mouse or programatically)
  WF_NoScrollWithMouse = 1 << 4, // Disable user vertically scrolling with mouse wheel
  WF_NoCollapse = 1 << 5, // Disable user collapsing window by double-clicking on it
  WF_AlwaysAutoResize = 1 << 6, // Resize every window to its content every frame
  WF_ShowBorders = 1 << 7, // Show borders around windows and items
  WF_NoSavedSettings = 1 << 8, // Never load/save settings in .ini file
  WF_NoInputs = 1 << 9, // Disable catching mouse or keyboard inputs
  WF_MenuBar = 1 << 10, // Has a menu-bar
  WF_HorizontalScrollbar = 1 << 11, // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
  WF_NoFocusOnAppearing = 1 << 12, // Disable taking focus when transitioning from hidden to visible state
  WF_NoBringToFrontOnFocus = 1 << 13, // Disable bringing window to front when taking focus (e.g. clicking on it or programatically giving it focus)
  WF_AlwaysVerticalScrollbar = 1 << 14, // Always show vertical scrollbar (even if ContentSize.y < Size.y)
  WF_AlwaysHorizontalScrollbar = 1 << 15, // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
  WF_AlwaysUseWindowPadding = 1 << 16, // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
  // [Internal]
  WF_ChildWindow = 1 << 20, // Don't use! For internal use by BeginChild()
  WF_ChildWindowAutoFitX = 1 << 21, // Don't use! For internal use by BeginChild()
  WF_ChildWindowAutoFitY = 1 << 22, // Don't use! For internal use by BeginChild()
  WF_ComboBox = 1 << 23, // Don't use! For internal use by ComboBox()
  WF_Tooltip = 1 << 24, // Don't use! For internal use by BeginTooltip()
  WF_Popup = 1 << 25, // Don't use! For internal use by BeginPopup()
  WF_Modal = 1 << 26, // Don't use! For internal use by BeginPopupModal()
  WF_ChildMenu = 1 << 27 // Don't use! For internal use by BeginMenu()
} WindowFlags;
typedef enum {
  Hit_ERROR,
  Hit_SYSMENU,
  Hit_GROWBOX,
  Hit_SIZE,
  Hit_MENU,
  Hit_HSCROLL,
  Hit_VSCROLL,
  Hit_MINBUTTON,
  Hit_MAXBUTTON,
  Hit_CLOSE,
  Hit_TOPLEFT,
  Hit_TOPRIGHT,
  Hit_BOTTOMLEFT,
  Hit_BOTTOMRIGHT,
  Hit_LEFT,
  Hit_RIGHT,
  Hit_TOP,
  Hit_BOTTOM,
  Hit_BORDER,
  Hit_REDUCE,
  Hit_ZOOM,
  Hit_HELP,
  Hit_CLIENT,
  Hit_CAPTION,
  Hit_RECTNUM,
} WindowHitValue;
static int window_calcrect(IRECT rect, int flags, int font_height, IRECT* prc)
{
  IRECT rc = rect;
  int has_cycaption = !(flags & WF_NoTitleBar);
  int has_bordr = 1;
  //int cxicon = 16;
  //int cxsize = GetSystemMetrics(SM_CXSIZE);
  //int cysize = GetSystemMetrics(SM_CYSIZE);
  //int cxborder = GetSystemMetrics(SM_CXBORDER);
  //int cyborder = GetSystemMetrics(SM_CYBORDER);
  //ISIZE sz = measureText("H", NULL, io->font, 0, fmt, NULL);
  int cxdlgframe = 4;
  int TitleBarHeight = font_height + 4;
  int d = has_cycaption ? TitleBarHeight : 0, b = has_bordr ? cxdlgframe : 0, c = 0, t = 2, tt;
  memset(prc, 0, sizeof(*prc)*Hit_RECTNUM);
  if (has_bordr) {
    rc = rect;
    t = b;
    tt = t;
    //iRectOffset(rc, -t, -t, t, t, &rc);
    iRectCutT(&rc, tt, 0, &prc[Hit_TOP]);
    iRectCutT(&rc, -tt, 0, &prc[Hit_BOTTOM]);
    iRectCutL(&rc, tt, 0, &prc[Hit_LEFT]);
    iRectCutL(&rc, -tt, 0, &prc[Hit_RIGHT]);
    iRectCutL(&prc[Hit_TOP], tt, 0, &prc[Hit_TOPLEFT]);
    iRectCutL(&prc[Hit_TOP], -tt, 0, &prc[Hit_TOPRIGHT]);
    iRectCutL(&prc[Hit_BOTTOM], tt, 0, &prc[Hit_BOTTOMLEFT]);
    iRectCutL(&prc[Hit_BOTTOM], -tt, 0, &prc[Hit_BOTTOMRIGHT]);
    rc = iRectOffset(rc, b, b, -b, -b);
  }
  if (d > 0) {
    t = 2;
    b = 2;
    rc = rect;
    rc = iRectOffset(rc, b, b, -b, -b);
    iRectCutT(&rc, d, 0, &prc[Hit_CAPTION]);
    prc[Hit_CLIENT] = rc;
    rc = prc[Hit_CAPTION];
    rc = iRectOffset(rc, t, t, -t, -1);
    //c = RCH(&rc);
    //iRectCutL(&rc, c, 0, &prc[Hit_SYSMENU]);
    iRectAlign(rc, 16, 16, TF_LEFT | TF_VCENTER, &prc[Hit_SYSMENU]);
    c = RCH(&rc) + 2;
    iRectCutL(&rc, -c, 0, &prc[Hit_CLOSE]);
    iRectCutL(&rc, -2, 0, &prc[Hit_MAXBUTTON]);
    iRectCutL(&rc, -c, 0, &prc[Hit_MAXBUTTON]);
    iRectCutL(&rc, -c, 0, &prc[Hit_MINBUTTON]);
  }
  else {
    prc[Hit_CLIENT] = rc;
  }
  return 0;
}
static int window_hittest(IRECT rect, int flags, int x, int y, const IRECT* pos)
{
  int i = 0, ret = -1, z = -1, n = Hit_RECTNUM;
  //iRectOffset(&pos[Hit_BOTTOMRIGHT], -10, -10, 10, 10);
  //x -= rc.l; y -= rc.t;
  for (; i < n; ++i) {
    if (iPtInRect(&pos[i], x, y)) {
      ret = i;
      break;
    }
  }
  if (0) {
#define XWINHITDEF(a, b) if (ret==a) printf("%s\n", b);
    //XWINHITDEF(HITERROR,"屏幕背景或窗口间的分界线上（与HTNOWHERE相同，但不包括DefWindowProc函数引发beep的错误的位置）")
    //XWINHITDEF(Hit_TRANSPARENT, "在被同一线程的另一个窗口覆盖的窗口上")
    XWINHITDEF(Hit_BORDER, "不能改变大小的窗口的边框")
    XWINHITDEF(Hit_CAPTION, "标题栏")
    XWINHITDEF(Hit_CLIENT, "客户区域")
    XWINHITDEF(Hit_GROWBOX, "缩放格(同HTSIZE)")
    XWINHITDEF(Hit_MENU, "菜单中")
    XWINHITDEF(Hit_HELP, "帮助按钮")
    XWINHITDEF(Hit_MAXBUTTON, "最大化按钮")
    XWINHITDEF(Hit_MINBUTTON, "最小化按钮")
    XWINHITDEF(Hit_REDUCE, "最小化按钮")
    XWINHITDEF(Hit_CLOSE, "关闭按钮")
    //XWINHITDEF(Hit_NOWHERE, "屏幕背景或窗口间的分界线上")
    XWINHITDEF(Hit_LEFT, "窗口的左边框")
    XWINHITDEF(Hit_RIGHT, "窗口的右边框")
    XWINHITDEF(Hit_SIZE, "缩放格(同HTGROWBOX)")
    XWINHITDEF(Hit_TOP, "窗口的上边框")
    XWINHITDEF(Hit_BOTTOM, "下边框")
    XWINHITDEF(Hit_TOPLEFT, "边框的左上角")
    XWINHITDEF(Hit_TOPRIGHT, "边框的右上角")
    XWINHITDEF(Hit_BOTTOMLEFT, "边框的左下角")
    XWINHITDEF(Hit_BOTTOMRIGHT, "边框的右下角")
    XWINHITDEF(Hit_VSCROLL, "垂直滚动条")
    XWINHITDEF(Hit_HSCROLL, "水平滚动条")
    XWINHITDEF(Hit_ZOOM, "最大化按钮")
    XWINHITDEF(Hit_SYSMENU, "系统菜单或子窗体的关闭按钮上")
#undef XWINHITDEF
  }
  return ret;
}

gc_t* window_begin_base(window_t* c)
{
  sysio_t* io = sys_getio();
  if (c->parent == NULL) {
    c->parent = io->cur_win;
    if (io->cur_win->child) {
      window_t* last = io->cur_win->child;
      for (; last->next; last = last->next);
      if (c->prev = last) {
        last->next = c;
      }
    }
    else {
      io->cur_win->child = c;
    }
  }
  io->cur_win = c;
  return io->cur_win->g;
}
bool window_begin(window_t* c, const char* label, int flags)
{
  SYSIO;
  static IRECT rc1;
  static IPOINT lastMouseDownPos;
  IRECT rc = c->rc;
  int hot = 0, hotid = -1;
  int ret = 0, ismax = 0;
  int has_bdr = 1;
  IRECT pos[Hit_RECTNUM] = {0};
  int fmt = TF_VCENTER;
  ISIZE font_sz = measureText("H", NULL, io->font, 0, fmt, NULL);
  g = window_begin_base(c);
  io->cur_win = c;
  if (1) {
    window_calcrect(rc, flags, font_sz.h, pos);
    c->client->rc = pos[Hit_CLIENT];
  }
  //IPOINT sz = iPOINT(font_sz, font_sz);
  //TraceMsg(hwnd, uMsg, nChar, lParam);
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      hotid = window_hittest(rc, flags, x, y, pos);
    }
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHITV(hotid);
        rc1 = rc;
        SETHIT(c);
        lastMouseDownPos = iPOINT(x, y);
        window_addtolast(c);
        SETFOCUSWIN(c);
      }
      break;
    case MSG_LBUTTONDBLCLK:
      if (hot) {
        SETHITV(hotid);
        if (Hit_CAPTION == io->hitvalue) {
          //window_cmd_proc_max(s);
          FORCE_REDRAW(c->rc);
        }
      }
      break;
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (Hit_CLOSE == io->hitvalue || Hit_MAXBUTTON == io->hitvalue || Hit_MINBUTTON == io->hitvalue) {
          int hit = hotid;
          if (io->hitvalue == hit) {
            if (Hit_CLOSE == hit) {
              //sendmsg(c, c->listener, WM_COMMAND, XCMD_CLOSE, (LPARAM)c);
            }
            if (Hit_MAXBUTTON == hit) {
              //window_cmd_proc_max(s);
            }
            if (Hit_MINBUTTON == hit) {
              //PostMessage(sys->hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
            }
          }
          FORCE_REDRAW(c->rc);
        }
        SETHITV(0);
      }
      break;
    case MSG_MOUSEMOVE:
      //myScreenToClient(hwnd, &x, &y);
      if (ISHIT(c)) {
        if (!ismax) {
          IRECT rcoff = {0};
          ISIZE minsz = iSIZE(BOUND(c->client->sz.w, 40, 100), font_sz.h + BOUND(c->client->sz.h, 40, 100));
          IPOINT lpt = {RCW(&rc1) - minsz.w, RCH(&rc1) - minsz.h};
          int dx, dy, donul = 0;
          dx = x - lastMouseDownPos.x, dy = y - lastMouseDownPos.y;
          //s->hotid = hit;
          switch (io->hitvalue) {
          case Hit_CLIENT:
          case Hit_CAPTION:
            rcoff.l = rcoff.r = dx;
            rcoff.t = rcoff.b = dy;
            break;
          case Hit_LEFT:
            rcoff.l = MIN(dx, lpt.x);
            break;
          case Hit_RIGHT:
            rcoff.r = MAX(dx, -lpt.x);
            break;
          case Hit_TOP:
            rcoff.t = MIN(dy, lpt.y);
            break;
          case Hit_BOTTOM:
            rcoff.b = MAX(dy, -lpt.y);
            break;
          case Hit_TOPLEFT:
            rcoff.t = MIN(dy, lpt.y);
            rcoff.l = MIN(dx, lpt.x);
            break;
          case Hit_TOPRIGHT:
            rcoff.t = MIN(dy, lpt.y);
            rcoff.r = MAX(dx, -lpt.x);
            break;
          case Hit_BOTTOMLEFT:
            rcoff.b = MAX(dy, -lpt.y);
            rcoff.l = MIN(dx, lpt.x);
            break;
          case Hit_BOTTOMRIGHT:
            rcoff.b = MAX(dy, -lpt.y);
            rcoff.r = MAX(dx, -lpt.x);
            break;
          default:
            donul = 1;
            //hit = window_hittest(rc, flags, x, y);
            break;
          }
          if (!donul) {
            c->rc = iRectAdd(rc1, rcoff);
            //rc1 = ctrl_calc_rect_impl(c, rc1);
            //ctrl_setrect(c, rc1);
            //xMoveWindow(hwnd, rc1, 1, 1);
            //printf("%d %d %d %d %d %d %s\n", rc1.l, rc1.t, rcoff.l, rcoff.t, rcoff.r, rcoff.b, c->text);
            //c->rc = rc1;
            //s->x = x;
            //s->y = y;
            FORCE_REDRAW(c->rc);
          }
          //printf("%d %d %d %d\n", x, y, rc.l, rc.t);
        }
      } else {
        SETHOT(c);
      }
      {
        window_calcrect(rc, flags, font_sz.h, pos);
        hotid = window_hittest(rc, flags, x, y, pos);
        if (has_bdr && !ismax) {
          //ctrl_setcursor(c, hit);
        }
        if (hotid > 0) {
          if (Hit_CLOSE == hotid || Hit_MAXBUTTON == hotid || Hit_MINBUTTON == hotid) {
            FORCE_REDRAW(c->rc);
          }
          if (Hit_CLOSE == hotid || Hit_MAXBUTTON == hotid || Hit_MINBUTTON == hotid) {
            FORCE_REDRAW(c->rc);
          }
        }
      }
      break;
    }
  }
  
  strncpy(c->text, label, 16);
  if (1) {
    int i = 0;
    COLOR clr = 0, clrText;
    static const int g_hitbtn[3] = {Hit_CLOSE, Hit_MAXBUTTON, Hit_MINBUTTON};
    int tt[] = {IdClose, ismax ? IdRestore : IdMaximize, IdMinimize};
    IRECT rc1;
    IRECT rc2 = rc;
    //IRECT rcclip = rc;
    int has_cap = !(flags & WF_NoTitleBar);
    int style = 0;
    //iRectOff0(&rcclip);
    //iRectOff0(&rc);
    rc1 = rc;
    rc2 = rc;
    window_calcrect(rc, flags, font_sz.h, pos);
    //iRectOffset(&rc2, -c->rc.l, -c->rc.t, -c->rc.l, -c->rc.t);
    //printf("WM_PAINT\n");
    //window_calcrect(rc2, s->rcs, s->has_cap, has_bdr && !ismax);
    switch (style) {
    case 0: {
      COLOR clrFace = myGetSysColor(CLR_BTNFACE);
      gcEdgeRectSys(g, rc2, BDS_HIGH, (has_bdr && !ismax) ? 15 : 0, clrFace);
      if (has_cap) {
        COLOR clr1 = ISFOCUSWIN(c) ? _RGB(10, 36, 106) : _RGB(106, 106, 106);
        COLOR clr2 = ISFOCUSWIN(c) ? _RGB(166, 202, 240) : _RGB(240, 240, 240);
        gcGradientRect2(g, pos[Hit_CAPTION], clr1, clr2, 1);
        for (i = 0; i < 3; ++i) {
          int pushed = (ISHIT(c)) && io->hitvalue == g_hitbtn[i];
          IRECT rc1 = pos[g_hitbtn[i]];
          gcEdgeRectSys(g, rc1, pushed ? BDS_SUNKEN : BDS_HIGH, 15, clrFace);
          rc1.l += 2 * pushed - 1;
          //gcDrawMenuimg(g, rc1, tt[i], ColorBlack);
        }
      }
      if (hotid >= 0 && hotid < Hit_CLIENT) {
        gcSolidRect2(g, pos[hotid], _rgba(1, 0, 0, 0.2));
      }
      if (has_cap) {
        rc1 = pos[Hit_CAPTION];
        rc1.l = pos[Hit_SYSMENU].r;
        clrText = ColorWhite;
        //rcclip.r = pos[Hit_MINBUTTON].l;
        gcSolidText(g, rc1, label, NULL, io->font, 0, TF_VCENTER | TF_NoWrap, clrText);
      }
    }
    break;
    case 1: {
      COLOR clrs[] = {ColorBlue, ColorBlue, ColorBlue};
      gcEdgeRect2(g, rc2, countof(clrs), ColorWhite, clrs, clrs, clrs, clrs);
      if (has_cap) {
        //imdraw_rect_gradient(g, pos[Hit_CAPTION], _RGB(10, 36, 106), _RGB(166, 202, 240), 1);
        gcSolidRect2(g, pos[Hit_CAPTION], ColorGreen);
        for (i = 0; i < 3; ++i) {
          int hoted = hotid == g_hitbtn[i];
          int pushed = io->hitvalue == g_hitbtn[i];
          IRECT rc1 = pos[g_hitbtn[i]];
          gcSolidRect2(g, rc1, pushed ? ColorGray : hoted ? ColorKhaki : ColorWhite);
          rc1.l += 2 * pushed - 1;
          //imdraw_menuimg(g, &rcclip, rc1, tt[i], ColorBlack);
        }
      }
      clrText = ColorWhite;
      if (has_cap) {
        rc1 = pos[Hit_CAPTION];
        rc1.l = pos[Hit_SYSMENU].r;
        //rcclip.r = pos[Hit_MINBUTTON].l;
        gcSolidText(g, rc1, label, NULL, io->font, 0, TF_VCENTER | TF_NoWrap, clrText);
      }
    }
    break;
    case 2: {
      int TitleBarHeight = font_sz.h + 4;
      COLOR bg_color;
      float window_rounding;
      if (flags & WF_NoInputs) {
        flags |= WF_NoMove | WF_NoResize;
      }
      bg_color = Col_WindowBg;
      if ((flags & WF_ComboBox) != 0) {
        bg_color = Col_ComboBg;
      }
      else if ((flags & WF_Tooltip) != 0 || (flags & WF_Popup) != 0) {
        bg_color = Col_PopupBg;
      }
      else if ((flags & WF_ChildWindow) != 0) {
        bg_color = Col_ChildWindowBg;
      }
      //if (bg_alpha >= 0.0f) { bg_color = set_alpha(bg_color, bg_alpha); }
      window_rounding = 4;
      gcFillColor(g, bg_color);
      gcFillRoundRect2(g, rc.l, rc.t + TitleBarHeight, RCW(&rc), RCH(&rc) - TitleBarHeight, window_rounding, (flags & WF_NoTitleBar) ? 0 : 3);
      clrText = Col_Text;
      // Title bar
      if (has_cap) {
        COLOR clrTitleBar = ISFOCUSWIN(c) ? Col_TitleBgActive : Col_TitleBg;
        gcFillColor(g, clrTitleBar);
        gcFillRoundRect2(g, rc.l, rc.t, RCW(&rc), TitleBarHeight, window_rounding, 12);
        gcSolidText(g, iRECT2(rc.l + TitleBarHeight, rc.t, RCW(&rc), TitleBarHeight),
            label, NULL, io->font, 0, TF_VCENTER | TF_NoWrap, Col_Text);
      }
    }
    break;
    }
  }
  return true;
}
int window_end()
{
  sysio_t* io = sys_getio();
  ASSERT(io->cur_win);
  io->cur_win = io->cur_win->parent;
  return 0;
}



#if 0
#include "aaa.inl"
int popup_begin(BOOL* popened)
{
  if (*popened) {
    GETGUIIO1();
    window_t* c = host->popup + host->pop_deep++;
    gc_t* g = ImGuiBase_Begin(io, host, c);
    IRECT rc = c->rect;
    int hot = 0;
    if (1) {
      *layer_push(0, rc, 1) = rc;
    }
    if (1) {
      hot = iPtInRect(&rc, x, y);
      switch (io->msg) {
      case MSG_LBUTTONDOWN:
        if (!hot) {
          *popened = !*popened;
        }
        break;
      }
    }
    return 1;
  }
  return 0;
}
int popup_end()
{
  GETGUIIO1();
  window_t* c = host->popup + host->pop_deep - 1;
  layer_pop();
  host->pop_deep--;
  GUIIO_END();
  return 0;
}
int listbody_begin(int n, IRECT* pclient)
{
  GETGUIIO2();
  int fmt = TF_CENTER | TF_VCENTER | TF_NoWrap, hot = 0, changed = 0;
  void* c = (void*)0;
  IRECT rc = c->rc;
  ISIZE sz = measureText("H", NULL, io->font, 0, fmt, NULL);
  int height = sz.w + 8;
  if (1) {
    if (!calc_rect(height, sz.h + 20, 0, &rc)) {
      return 0;
    }
  }
  return 1;
}
int listbody_end(int n)
{
  return 1;
}
int monthcal_ctrl(uint* pdate)
{
  GETGUIIO2();
  IRECT rc;
  int i, fmt = TF_CENTER | TF_VCENTER | TF_NoWrap, changed = 0, hot = 0, hotid = -1;
  enum {offTitle = 0, offInfo = 1, offWeak = 2, offDay = 9};
  IRECT rcs[7 * 7 + 7] = {0}, rect;
  IRECT* rcTitle = rcs + offTitle;
  IRECT* rcInfo = rcs + offInfo;
  IRECT* rcWeak = rcs + offWeak;
  IRECT* rcDay = rcs + offDay;
  char buf[80];
  uint date = *pdate;
  void* c = pdate;
  int days, year, month, day, curpos = 0, date2;
  if (0) {
    int days2;
    days = date_to_days(2016, 1, 2);
    days2 = date_to_days(2015, 12, 31);
    date = days_to_date(days);
  }
  //setlocale(LC_ALL, ".936");
  if (0 == *pdate) {
    time_t rawtime;
    tm_t* info = NULL;
    time(&rawtime);
    info = localtime(&rawtime);
    DATESET(date, info->tm_year + 1900, info->tm_mon + 1, info->tm_mday);
    changed = 1;
  }
  if (1) {
    ISIZE sz = measureText("H", NULL, io->font, 0, fmt, NULL);
    int line_h = sz.h + 4, changed = 0;
    int w = sz.w * 30;
    int h = line_h * 10;
    //rc = iRECT2(20, 20, w, h);
    if (!calc_rect(w, h, io->layoutflags, &rc)) {
      return 0;
    }
    rect = rc;
    iRectIn(&rc, 1);
    iRectCutT(&rc, line_h * 3 / 2, 0, rcTitle);
    iRectCutB(&rc, -line_h, 0, rcInfo);
    iRectMatrix(rc, 7, 7, 0, 0, TF_CENTER | TF_VCENTER, 49, rcWeak);
  }
  DATEGET(date, year, month, day);
  {
    int wday;
    days = date_to_days(year, month, day);
    wday = days_to_wday(days);
    curpos = (day - wday + 6) / 7;
    curpos = curpos * 7 + wday;
    days -= curpos;
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rect, x, y);
    if (hot) {
      hotid = iPtInRects(countof(rcs), rcs, x, y);
    }
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        if (hotid >= offDay) {
          if ((hotid - offDay) != curpos) {
            curpos = hotid - offDay;
            date = days_to_date(days + curpos);
            changed = 1;
          }
        }
      }
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        SETHOT(c);
      }
      break;
    case MSG_MOUSEWHEEL:
      date2 = months_to_date(date_to_months(year, month) + (io->MouseWheel > 0 ? -1 : 1), 1);
      if (date2 != (int)date) {
        date = date2;
        changed = 1;
      }
      break;
    }
  }
  if (changed) {
    DATEGET(date, year, month, day);
  }
  *pdate = date;
  if (1) {
    gcEdgeRect1(g, rect, ColorWhite, ColorBlack, 1);
    gcEdgeRect1(g, *rcTitle, _RGB(0, 0, 255), 0, 0);
    _snprintf(buf, 80, "%d/%d", year, month);
    gcSolidText(g, *rcTitle, buf, NULL, io->font, 0, fmt, ColorWhite);
    if (1) {
      tm_t tm2;
      time_t rawtime;
      tm_t* info = NULL;
      time(&rawtime);
      info = localtime(&rawtime);
      tm2 = *info;
      tm2.tm_yday -= tm2.tm_wday;
      tm2.tm_mday -= tm2.tm_wday;
      tm2.tm_wday -= tm2.tm_wday;
      for (i = 0; i < 7; ++i) {
        strftime(buf, 80, "%a", &tm2);
        gcEdgeRect1(g, rcWeak[i], _RGBA(0, 0, 255, 12), 0, 0);
        gcSolidText(g, rcWeak[i], buf, NULL, io->font, 0, fmt, ColorBlack);
        tm2.tm_yday++;
        tm2.tm_mday++;
        tm2.tm_wday++;
      }
    }
    for (i = 0; i < 6 * 7; ++i) {
      IRECT rc1 = rcDay[i];
      COLOR crText = ColorBlack;
      date = days_to_date(days + i);
      _snprintf(buf, 80, "%d", date % 100);
      if (((date / 100) % 100) != (uint)month) {
        crText = _RGBA(50, 50, 50, 90);
      }
      else if (i == curpos) {
        gcEdgeRect1(g, rc1, _RGBA(0, 0, 255, 125), 0, 0);
        crText = ColorWhite;
      }
      if (hot && (hotid) == (i + offDay)) {
        gcEdgeRect1(g, rc1, 0, ColorBlack, 1);
      }
      gcSolidText(g, rc1, buf, NULL, io->font, 0, fmt, crText);
      date = date_add(date, 1);
    }
  }
  return changed;
}
static int toolbar_ctrl(texture_t* tex, int nrc, const IRECT* rcs_tex)
{
  GETGUIIO2();
  int cx = 22, cy = 22, border = 2, gap = 0;
  int clicked = 0, hot = 0, hit, hotid = -1, hitid = -1;
  int fmt = TF_CENTER | TF_VCENTER | TF_NoWrap;
  IRECT rc = {0}, rcIn;
  void* c = (void*)rcs_tex;
  if (1) {
    if (!calc_rect(cx * nrc + gap * (nrc - 1) + 2 * border, cy + 2 * border, LF_alighParentTop, &rc)) {
      return 0;
    }
  }
  rcIn = rc;
  iRectDeflate2(&rcIn, border, border);
  hit = ISHIT(c);
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      hotid = iRectMatrix2_HitText(rcIn, cx, cy, gap, gap, nrc, x, y);
    }
    switch (io->msg) {
    case MSG_LBUTTONUP:
      if (hit) {
        if (io->hitvalue == hotid) {
          hitid = hotid;
          clicked = 1;
        }
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        io->hitvalue = hotid;
      }
      break;
    }
  }
  if (1) {
    COLOR clrBk;
    clrBk = _GetSysColor(CLR_BTNFACE);
    IRECT rc1, rc2 = gcEdgeRectSys(g, rc, BDS_UP, BDM_ALL, clrBk);
    int n = nrc, i, h, w, col, row;
    int gapy = gap, gapx = gap;
    h = RCH(&rcIn) + gapy, w = RCW(&rcIn) + gapx;
    cx += gapx, cy += gapy;
    col = w / cx, row = h / cy;
    n = MIN(n, row * col);
    for (i = 0; i < n; ++i) {
      IPOINT off = {0};
      rc1.l = rcIn.l + (int)(cx * (i % col));
      rc1.r = rc1.l - gapx + (int)cx;
      rc1.t = rcIn.t + (int)(cy * (i / col));
      rc1.b = rc1.t - gapy + (int)cy;
      iRectAlign(rc1, RCW(&rcs_tex[i]), RCH(&rcs_tex[i]), fmt, &rc2);
      if (hotid == i) {
        gcEdgeRectSys(g, rc1, hit ? BDS_LOW : BDS_UP, BDM_ALL, clrBk);
        iRectOff2(&rc2, !!hit, !!hit);
      }
      gcRectImageR(g, rc2, tex, rcs_tex + i);
      //button_ctrl
    }
  }
  return hitid;
}
CC_INLINE int curve_hittest(IPOINT* points, int npt, int x, int y)
{
  int i;
  for (i = 0; i < npt; ++i) {
    IPOINT p = points[i];
    int d = 3;
    IRECT rc = iRECT(p.x - d, p.y - d, p.x + d, p.y + d);
    if (iPtInRect(&rc, x, y)) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int curve_proc(IPOINT* points, int npt)
{
  GETGUIIO2();
  IRECT rc = c->rc;
  int hot = 0;
  static int hitid = -1;
  static IPOINT downPt;
  void* c = points;
  if (!calc_rect(100, 100, LF_alighParentTop, &rc)) {
    return 0;
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        hitid = curve_hittest(points, npt, x, y);
        if (hitid >= 0 && hitid < npt) {
          downPt = points[hitid];
        }
        SETHIT(c);
        lastMouseDownPos = iPOINT(x, y);
      }
      break;
    case MSG_LBUTTONDBLCLK:
      if (iPtInRect(&rc, x, y)) {
        //hit = curve_hittest(s, x, y);
      }
      break;
    case MSG_LBUTTONUP:
      if (c) {
        hitid = -1;
      }
      break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        if (iPtInRect(&rc, x, y)) {
          if (hitid >= 0 && hitid < npt) {
            points[hitid] = iPOINT(downPt.x + x - lastMouseDownPos.x, downPt.y + y - lastMouseDownPos.y);
          }
        }
      }
      break;
    }
  }
  if (1) {
    int i = 0, j = 0;
    DPOINT* pt = MALLOC(DPOINT, npt);
    IRECT rcclip = rc;
    int closed = 0;
    for (j = 0; j < npt; ++j) {
      pt[j] = dPOINT(points[j].x, points[j].y);
    }
    {
      DPOINT pt2[128];
      int n = vcgen_bspline(countof(pt2), pt2, npt, pt, closed);
      //imdrawaa_spline(g, &rcclip, pt, npt, 0, 0, ColorBlack, 1);
    }
    //if (mouseDown) {
    //imdrawaa_line(g, 0, p.Y, clientRect.Width, p.Y, Gray);
    //g.DrawLine(Pens.Gray, p.X, 0, p.X, clientRect.Height);
    //}
    for (i = 0; i < npt; ++i) {
      IPOINT p = points[i];
      int d = 3;
      COLOR clr = i == hot ? ColorBlack;
      IRECT rc = iRECT(p.x - d, p.y - d, p.x + d, p.y + d);
      if (i == hot) {
        //imdrawaa_rect(g, p.x-d, p.y-d, p.x+d, p.y+d, ColorBlack, 0, 0);
        //imdraw_fillrect(g, &rcclip, rc, ColorBlack);
      }
      else {
        //imdraw_border_n(g, &rcclip, rc, 0, 1, &clr, &clr, &clr, &clr);
        //imdrawaa_rect(g, p.x-d, p.y-d, p.x+d, p.y+d, 0, ColorBlack, 1);
      }
      gcSolidCircle();
    }
    FREE(pt);
  }
  return 0;
}
#include "bbb.inl"
#endif


int image_ctrl(ctrl_t* c, const texture_t* tex, IPOINT* pPos, double* pscale, FPOINT* fPos) {
  SYSIO;
  int hot = 0, changed = 0;
  static IPOINT downpt = {0};
  static IPOINT pos = {0};
  IRECT rc = c->rc, rcdst1;
  IRECT rcdst = {0, 0, 500, 500}, boundrc;
  int h = tex->h, w = tex->w, dh, dw;
  int show_w;
  double scale1 = 1;
  const double scale_min = 0.2;
  const double scale_max = 32;
  const double sc1 = 1.2;
  const double sc[] = {sc1, 1./sc1};
  FPOINT fPos1[1] = {0};
  IPOINT pPos1[1] = {0};
  bool mode1 = 1;
  bool can_scale = pscale!=NULL;
  bool can_move = pPos!=NULL;
  fPos = fPos ? fPos : fPos1;
  pPos = pPos ? pPos : pPos1;
  if (!can_scale) {
    scale1 = iRectScaling(rc, tex->h, tex->w, &rcdst1);
  }
  if (!can_move) {
    *pPos1 = iPOINT(rcdst1.l, rcdst1.t);
  }
  pscale = pscale ? pscale : &scale1;
  *pscale = BOUND(*pscale, scale_min, scale_max);
  rcdst = iRECT2(rc.l + pPos->x, rc.t + pPos->y, dw = (int)(w*(*pscale)), dh = (int)(h*(*pscale)));
  rcdst1 = iRectIn(rcdst, -1);
  show_w = MIN3(100, RCW(&rc), RCH(&rc));
  show_w = MIN3(show_w, dw, dh);
  //
  if (1) {
    c->sz = iSIZE(w, h);
  }
  if (mode1) {
    boundrc = iRECT(RCW(&rc) - dw - show_w, RCH(&rc) - dh - show_w, show_w, show_w);
  } else {
    boundrc = iRECT(show_w - dw, show_w - dh, RCW(&rc) - show_w, RCH(&rc) - show_w);
  }
  if (dw<RCW(&rc)) {
    pPos->x = (RCW(&rc)-dw)/2;
  }
  if (dh<RCH(&rc)) {
    pPos->y = (RCH(&rc)-dh)/2;
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      if (io->keydown[' ']) {
        //res_set_cursor(mouse->down[0] ? "cursor-closedhand" : "cursor-openhand");
      } else {
        //res_set_cursor(NULL);
      }
    }
    switch (io->msg) {
    //case MSG_MBUTTONDOWN:
    case MSG_LBUTTONDOWN:
      if (hot) {
        //if (io->keydown[' '])
        {
          SETHIT(c);
          downpt = iPOINT(x, y);
          pos = *pPos;
        }
      }
      break;
    case MSG_MOUSEMOVE:
      if (can_scale && ISHIT(c)) {
        //if (io->keydown[' '])
        {
          pPos->x = pos.x + x - downpt.x;
          pPos->y = pos.y + y - downpt.y;
          changed = 1;
          pPos->x = BOUND(pPos->x, boundrc.l, boundrc.r);
          pPos->y = BOUND(pPos->y, boundrc.t, boundrc.b);
        }
      }
      break;
    case MSG_MOUSEWHEEL:
      if (can_move && hot) {
        double ss = *pscale;
        *pscale *= sc[mouse->wheelDelta>0];
        *pscale = BOUND(*pscale, scale_min, scale_max);
        ss = (*pscale)/ss - 1;
        pPos->x -= (x - rcdst.l) * ss;
        pPos->y -= (y - rcdst.t) * ss;
        changed = 1;
      }
      break;
    case MSG_KEYDOWN:
      break;
    case MSG_KEYUP:
      if (hot) {
      }
      //res_set_cursor(NULL);
      break;
    }
  }
  
  if (1) {
    dw = (int)(w*(*pscale));
    dh = (int)(h*(*pscale));

    pPos->x = BOUND(pPos->x, boundrc.l, boundrc.r);
    pPos->y = BOUND(pPos->y, boundrc.t, boundrc.b);
    
    if (mode1) {
      boundrc = iRECT(RCW(&rc) - dw - show_w, RCH(&rc) - dh - show_w, show_w, show_w);
    } else {
      boundrc = iRECT(show_w - dw, show_w - dh, RCW(&rc) - show_w, RCH(&rc) - show_w);
    }
    if (dw<RCW(&rc)) {
      pPos->x = (RCW(&rc)-dw)/2;
    }
    if (dh<RCH(&rc)) {
      pPos->y = (RCH(&rc)-dh)/2;
    }
    rcdst = iRECT2(rc.l + pPos->x, rc.t + pPos->y, dw, dh);
    rcdst1 = iRectIn(rcdst, -1);
  }
  fPos->x = (x - rcdst.l)/(*pscale);
  fPos->y = (y - rcdst.t)/(*pscale);
  if (1) {
    gcClipRectR(g, rc);
    gcEdgeRect1(g, rc, 0, ColorBlack, 1);
    gcEdgeRect1(g, rcdst1, 0, ColorBlack, 1);
    gcRectImageR(g, rcdst, tex, NULL);
    if (*pscale>2) {
      int x = (int)fPos->x;
      int y = (int)fPos->y;
      IRECT rc = iRECT(x*(*pscale)+rcdst.l, y*(*pscale)+rcdst.t, (x+1)*(*pscale)+rcdst.l, (y+1)*(*pscale)+rcdst.t);
      gcEdgeRect1(g, rc, 0, _rgb(1,0,1), 1);
    }
    gcClipReset(g);
  }
  return 0;
}

#if 0
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
  imdrawaa_polygon(im, pclip, pt, len, n, crFill, crLine, wline);
  return 0;
}
#endif
static int gcDrawPolygonCtrl(gc_t* g, const FPOINT* pt, int npt, int closed, font_t* fnt, int hit)
{
  int i;
  char buf[256];
  COLOR m_stroke_color = _rgba(0.0, 0.0, 0.0, 0.8);
  COLOR m_text_color = (_rgba(0.0, 0.0, 0.0, 1));
  double m_stroke_width = 1;
  double m_point_radius = 5;
  m_stroke_color = _RGBA(0,150,160, 200);
  gcPen(g, m_stroke_color, m_stroke_width);
  gcDrawPolygon(g, pt, npt, closed, GC_FLOAT, sizeof(*pt));
  for (i = 0; i < npt; ++i) {
    bool hitnode = (hit&POHIT_POINT) && (i == (hit & POHIT_MARK));
    double r = (hitnode) ? m_point_radius * 1.2 : m_point_radius;
    COLOR m_node_color = hitnode ? _rgba(0.5, 0.0, 0.0, 0.5) : _rgba(0.0, 0.5, 0.0, 0.5);
    gcSolidBrush(g, m_node_color);
    gcFillEllipse2(g, pt[i].x, pt[i].y, r, r);
    if (1) {
      int x = (int)pt[i].x, y = (int)pt[i].y;
      _snprintf(buf, 256, "%d", i);
      gcSolidText(g, iRECT(x, y, x + 30, y + 20), buf, NULL, fnt, 0, TF_VCENTER, m_text_color);
    }
  }
  if (hit&POHIT_EDGE) {
    int i = hit & POHIT_MARK;
    int j = (i+1)%npt;
    gcPen(g, _rgba(0.5, 0.0, 0.0, 0.5), m_stroke_width*2);
    gcDrawLine(g, pt[i].x, pt[i].y, pt[j].x, pt[j].y);
  }
  if (hit&POHIT_FACE) {
    gcSolidBrush(g, _rgba(0.0, 0.0, 0.5, 0.2));
    gcFillPolygon(g, pt, npt, closed, GC_FLOAT, sizeof(*pt));
  }
  return 0;
}
static int polygon_ctrl(ctrl_t* c, FPOINT* pt, int npt, bool close, double m_point_radius)
{
  SYSIO;
  //IRECT rc = c->rc;
  int hot=0, hit = 0, changed=0;
  int m_hitmark = POHIT_POINT|POHIT_EDGE|POHIT_FACE;
  static IPOINT m_pt;
  if (ISHOVEREDWIN2()) {
    //hot = iPtInRect(&rc, x, y);
    if (ISHIT(c)) {
      hit = io->hitvalue;
    } else {
      hit = point_in_polygon_hittest(pt, npt, x, y, m_point_radius, m_hitmark);
    }
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hit) {
        SETHIT(c);
        m_pt.x = x, m_pt.y = y;
        SETHITV(hit);
        force_redraw(c);
      }
      break;
#if 0
    case MSG_LBUTTONDBLCLK:
      if (hit & POHIT_POINT) {
        int i = hit & POHIT_MARK;
        dPOLYGON_delpt(m_polygon, i);
        force_redraw(c);
      }
      else {
        int j = dPOLYGON_line_nearest(m_polygon, x, y, 20);
        if (j >= 0) {
          dPOLYGON_addpt(m_polygon, j, x, y);
          force_redraw(c);
        }
      }
      break;
#endif
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        int i = hit & POHIT_MARK;
        double dx, dy;
        dx = x - m_pt.x;
        dy = y - m_pt.y;
        if (hit & POHIT_POINT) {
          pt[i].x += dx;
          pt[i].y += dy;
          changed = 1;
        }
        else if (hit & POHIT_FACE) {
          for (i = 0; i < npt; i++) {
            pt[i].x += dx;
            pt[i].y += dy;
          }
          changed = 1;
        }
        else if (hit & POHIT_EDGE) {
          int n1 = i;
          int n2 = NEXT_INDEX(i, npt);
          pt[n1].x += dx;
          pt[n1].y += dy;
          pt[n2].x += dx;
          pt[n2].y += dy;
          changed = 1;
        }
        m_pt.x = x, m_pt.y = y;
        if (changed) {
          force_redraw(c);
        }
      }
      break;
    }
  }
  
  if (1) {
    //IRECT rc = c->rc;
    //imdraw_rect(im, &rcclip, rc, _rgb(1, 1, 1), Color_Black, 1, BDM_ALL);
    gcDrawPolygonCtrl(g, pt, npt, close, io->font, hit);
  }
  return changed;
}

static int gcDrawPathCtrl(gc_t* g, const FPOINT* Points, const BYTE* Types, int Count,
                          font_t* fnt, int hit, double m_point_radius, bool face, bool edge, bool point, bool text) {
  int i, j;
  char buf[256];
  COLOR m_stroke_color = _rgba(0.0, 0.0, 0.0, 0.8);
  COLOR m_text_color = (_rgba(0.0, 0.0, 0.0, 1));
  double m_stroke_width = 1;
  m_stroke_color = _RGBA(0,150,160, 200);
  gcSolidBrush(g, _rgba(0.0, 0.0, 0.5, 0.2));
  gcPen(g, m_stroke_color, m_stroke_width);
  PATH_LOOP_BEGIN;
  if (face) {
    gcFillPolygon(g, Points + pos, end - pos, true, GC_FLOAT, sizeof(*Points));
  }
  if (edge) {
    gcDrawPolygon(g, Points + pos, end - pos, closed, GC_FLOAT, sizeof(*Points));
  }
  PATH_LOOP_END;
  if (point) {
    for (i=0; i<Count; ++i) {
      bool hitnode = (hit&POHIT_POINT) && (i == (hit & POHIT_MARK));
      double r = (hitnode) ? m_point_radius * 1.2 : m_point_radius;
      COLOR m_node_color = hitnode ? _rgba(0.5, 0.0, 0.0, 0.5) : _rgba(0.0, 0.5, 0.0, 0.5);
      gcSolidBrush(g, m_node_color);
      gcFillEllipse2(g, Points[i].x, Points[i].y, r, r);
    }
  }
  if (text) {
    for (i=0; i<Count; ++i) {
      int x = (int)Points[i].x, y = (int)Points[i].y;
      _snprintf(buf, 256, "%d", i);
      gcSolidText(g, iRECT(x, y, x + 30, y + 20), buf, NULL, fnt, 0, TF_VCENTER, m_text_color);
    }
  }
  if (hit&POHIT_EDGE) {
    i = hit & POHIT_MARK;
    j = (i+1)%Count;
    gcPen(g, _rgba(0.5, 0.0, 0.0, 0.5), m_stroke_width*2);
    gcDrawLine(g, Points[i].x, Points[i].y, Points[j].x, Points[j].y);
  }
  return 0;
}

static int gcDrawPathCtrl2(gc_t* g, const FPOINT* Points, const BYTE* Types, int Count,
                          font_t* fnt, int hit, double m_point_radius, double flatness) {
  if (PathIsFlattenImpl(Types, Count)) {
    gcDrawPathCtrl(g, Points, Types, Count, fnt, hit, m_point_radius, true, true, true, true);
  } else {
    Path flat_path[1] = {0};
    flatness = BOUND(flatness, 0.01, 2);
    PathFlatten2(Points, Types, Count, flatness, flat_path);
    gcDrawPathCtrl(g, flat_path->Points, flat_path->Types, flat_path->Count, fnt, 0, m_point_radius/2, true, true, true, false);
    PathFree(flat_path);
    gcDrawPathCtrl(g, Points, Types, Count, fnt, hit, m_point_radius, false, true, true, true);
  }
  return 0;
}
static int path_ctrl(ctrl_t* c, FPOINT* Points, const BYTE* Types, int Count, double flatness) {
  SYSIO;
  //IRECT rc = c->rc;
  int hot=0, hit = 0, changed=0;
  int m_hitmark = POHIT_POINT|POHIT_EDGE|POHIT_FACE;
  double m_point_radius = 5;
  static IPOINT m_pt;

  if (ISHOVEREDWIN2()) {
    //hot = iPtInRect(&rc, x, y);
    if (ISHIT(c)) {
      hit = io->hitvalue;
    } else {
      hit = point_in_polygon_hittest(Points, Count, x, y, m_point_radius, m_hitmark);
    }
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hit) {
        SETHIT(c);
        m_pt.x = x, m_pt.y = y;
        SETHITV(hit);
        force_redraw(c);
      }
      break;
#if 0
    case MSG_LBUTTONDBLCLK:
      if (hit & POHIT_POINT) {
        int i = hit & POHIT_MARK;
        dPOLYGON_delpt(m_polygon, i);
        force_redraw(c);
      }
      else {
        int j = dPOLYGON_line_nearest(m_polygon, x, y, 20);
        if (j >= 0) {
          dPOLYGON_addpt(m_polygon, j, x, y);
          force_redraw(c);
        }
      }
      break;
#endif
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        int i = hit & POHIT_MARK;
        double dx, dy;
        dx = x - m_pt.x;
        dy = y - m_pt.y;
        if (hit & POHIT_POINT) {
          Points[i].x += dx;
          Points[i].y += dy;
          changed = 1;
        }
        else if (hit & POHIT_FACE) {
          for (i = 0; i < Count; i++) {
            Points[i].x += dx;
            Points[i].y += dy;
          }
          changed = 1;
        }
        else if (hit & POHIT_EDGE) {
          int n1 = i;
          int n2 = NEXT_INDEX(i, Count);
          Points[n1].x += dx;
          Points[n1].y += dy;
          Points[n2].x += dx;
          Points[n2].y += dy;
          changed = 1;
        }
        m_pt.x = x, m_pt.y = y;
        if (changed) {
          force_redraw(c);
        }
      }
      break;
    }
  }
  
  if (1) {
    gcDrawPathCtrl2(g, Points, Types, Count, io->font, hit, m_point_radius, flatness);
  }
  return changed;
}
static int path_ctrl2(ctrl_t* c, Path* path, double flatness) {
  return path_ctrl(c, path->Points, path->Types, path->Count, flatness);
}

CC_INLINE int scale_float_ctrl(ctrl_t* c, float* m_value1, float* m_value2)
{
  SYSIO;
  double m_xs1[2];
  double m_xs2[2];
  IRECT rc = c->rc;
  //IPOINT pt = iPOINT(x, y);
  enum move_e {
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
  int m_border_extra = MIN(abs(x2 - x1), abs(y2 - y1)) / 2;
  double be = m_border_extra;
  int hot = 0, hit = move_nothing;
  static double m_pdx[2] = {0, 0};
  double m_min_d = 0.01;
  
  if (1) {
    ISIZE sz = measureText("H", NULL, io->font, 0, TF_VCENTER, NULL);
    c->sz = iSIZE(sz.w + 6, sz.h + 6);
  }
  m_xs1[0] = x1 + be / 1;
  m_xs2[0] = x2 - be / 1;
  m_xs1[1] = y1 + be / 2;
  m_xs2[1] = y2 - be / 2;
  xx1[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * *m_value1;
  xx2[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * *m_value2;
  xx1[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
  xx2[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
  //*m_value1 = (0.3);
  //*m_value2 = (0.7);
  
  
  if (ISHOVEREDWIN2()) {
    double r = m_border_extra;
    hot = iPtInRect(&rc, x, y);
    xx1[!bb] = m_xs1[!bb];
    xx2[!bb] = m_xs2[!bb];
    if (1) {
      if (calc_distance(x, y, xx1[0], xx1[1]) <= r) {
        hit = (move_value1);
      } else if (calc_distance(x, y, xx2[0], xx2[1]) <= r) {
        hit = (move_value2);
      } else if (xx[0] > xx1[0] && xx[0] < xx2[0] && xx[1] > xx1[1] && xx[1] < xx2[1]) {
        hit = move_slider;
      }
    }
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hit) {
        SETHIT(c);
        SETHITV(hit);
        switch(hit) {
        case move_value1:
          //if(x < xp1 && calc_distance(x, y, xp1, yp) <= m_y2 - m_y1)
          m_pdx[0] = xx[0] - xx1[0];
          m_pdx[1] = xx[1] - xx1[1];
          break;
        case move_value2:
          //if(x > xp2 && calc_distance(x, y, xp2, yp) <= m_y2 - m_y1)
          m_pdx[0] = xx[0] - xx2[0];
          m_pdx[1] = xx[1] - xx2[1];
          break;
        case move_slider:
          m_pdx[0] = xx[0] - xx1[0];
          m_pdx[1] = xx[1] - xx1[1];
          break;
        }
      }
      break;
    case MSG_MOUSEMOVE:
      if (1) {
        if (ISHIT(c)) {
          int m_move_what = hit = io->hitvalue;
          double dv;
          switch (m_move_what) {
          case move_value1:
            if (1) {
              *m_value1 = (xx[bb] - m_pdx[bb] - m_xs1[bb]) / (m_xs2[bb] - m_xs1[bb]);
              if (*m_value1 < 0.0) {
                *m_value1 = 0.0;
              }
              if (*m_value1 > *m_value2 - m_min_d) {
                *m_value1 = *m_value2 - m_min_d;
              }
              force_redraw(c);
            }
            break;
          case move_value2:
            if (1) {
              *m_value2 = (xx[bb] - m_pdx[bb] - m_xs1[bb]) / (m_xs2[bb] - m_xs1[bb]);
              if (*m_value2 > 1.0) {
                *m_value2 = 1.0;
              }
              if (*m_value2 < *m_value1 + m_min_d) {
                *m_value2 = *m_value1 + m_min_d;
              }
              force_redraw(c);
            }
            break;
          case move_slider:
            if (1) {
              dv = *m_value2 - *m_value1;
              *m_value1 = (xx[bb] - m_pdx[bb] - m_xs1[bb]) / (m_xs2[bb] - m_xs1[bb]);
              //printf("%f, %f, %f, %f\n", xx1[bb], xx[bb], m_pdx[bb],m_xs1[bb]);
              *m_value2 = *m_value1 + dv;
              if (*m_value1 < 0.0) {
                dv = *m_value2 - *m_value1;
                *m_value1 = 0.0;
                *m_value2 = *m_value1 + dv;
              }
              if (*m_value2 > 1.0) {
                dv = *m_value2 - *m_value1;
                *m_value2 = 1.0;
                *m_value1 = *m_value2 - dv;
              }
              force_redraw(c);
            }
            break;
          }
        }
      }
      break;
    }
  }
  if (1) {
    double m_border_thickness = 1;
    double r, bw = m_border_thickness, be = m_border_extra;
    int m_descending = (FALSE);
    COLOR m_background_color = _rgb(1.0, 0.9, 0.8);
    COLOR m_border_color = _rgb(0.0, 0.0, 0.0);
    COLOR m_pointers_color = _rgba(0.8, 0.0, 0.0, 0.8);
    COLOR m_active_pointers_color = _rgba(0.8, 0.2, 0.2, 0.8);
    COLOR m_slider_color = _rgba(0.2, 0.1, 0.0, 0.6);
    COLOR m_active_slider_color = _rgba(0.2, 0.1, 0.2, 0.6);
    COLOR m_text_color = _rgb(0.0, 0.0, 0.0);
    IRECT rc = c->rc;
    m_x1 = rc.l, m_y1 = rc.t;
    m_x2 = rc.r, m_y2 = rc.b;
    bb = fabs(m_x2 - m_x1) <= fabs(m_y2 - m_y1);
    x1 = rc.l, y1 = rc.t, x2 = rc.r, y2 = rc.b;
    m_border_extra = MIN(abs(x2 - x1), abs(y2 - y1)) / 2;
    be = m_border_extra;
    m_xs1[0] = x1 + be / 1;
    m_xs2[0] = x2 - be / 1;
    m_xs1[1] = y1 + be / 2;
    m_xs2[1] = y2 - be / 2;
    xx1[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * *m_value1;
    xx2[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * *m_value2;
    xx1[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
    xx2[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
    
    if (1) {
      double xx1[2];
      double xx2[2];
      // Background
      gcEdgeRect1(g, iRECT(x1, y1, x2, y2), m_background_color, 0, 0);
      //lens[0] = vcgen_rect(pt, x1, y1, x2, y2);
      gcEdgeRect1(g, iRECT(m_xs1[0], m_xs1[1], m_xs2[0], m_xs2[1]), 0, m_border_color, 1);
      r = m_border_extra;
      xx1[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * *m_value1;
      xx2[bb] = m_xs1[bb] + (m_xs2[bb] - m_xs1[bb]) * *m_value2;
      xx1[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
      xx2[!bb] = (m_xs1[!bb] + m_xs2[!bb]) / 2.0;
      gcSolidBrush(g, hit == move_value1 ? m_active_pointers_color : m_pointers_color);
      gcFillEllipse2(g, xx1[0], xx1[1], r, r);
      gcSolidBrush(g, hit == move_value2 ? m_active_pointers_color : m_pointers_color);
      gcFillEllipse2(g, xx2[0], xx2[1], r, r);
      xx1[!bb] = m_xs1[!bb];
      xx2[!bb] = m_xs2[!bb];
      gcEdgeRect1(g, iRECT(xx1[0], xx1[1], xx2[0], xx2[1]), hit == move_value1 ? m_active_slider_color : m_slider_color, 0, 0);
    }
    
#if 1
    {
      char buf[256];
      _snprintf(buf, 256, "%.3f %.3f", *m_value1, *m_value2);
      gcSolidText(g, rc, buf, NULL, io->font, 0, TF_VCENTER, m_text_color);
    }
#endif
  }
  return FALSE;
}



static int spline_ctrl(ctrl_t* c, int m_num_pnt, double* m_x, double* m_y, int nvalues, double* m_values)
{
#define spline_calc_xp(idx)   (m_xs1 + (m_xs2 - m_xs1) * m_x[idx])
#define spline_calc_yp(idx)   (m_ys1 + (m_ys2 - m_ys1) * m_y[idx])
  SYSIO;
  IRECT rc = c->rc;
  int i;
  double bw, be, bb;
  int m_border_width = 1;
  int m_border_extra = 2;
  int m_curve_width = 1;
  double m_point_size = 3;
  int hot=0, hit=0, changed = 0;
  double m_xs1;
  double m_ys1;
  double m_xs2;
  double m_ys2;
  int m_vertex = 0;
  int m_active_pnt = -1;
  int m_move_pnt = -1;
  double m_pdx = 0.0;
  double m_pdy = 0.0;
  
  ASSERT(m_num_pnt>=4 && m_num_pnt<=32);
  if (0==RCW(&rc)) {
    changed = 1;
  }
  if (1) {
    ISIZE sz = measureText("H", NULL, io->font, 0, TF_VCENTER, NULL);
    c->sz = iSIZE(sz.w + 6, sz.h + 6);
  }
  bw = m_border_width;
  be = m_border_extra;
  bb = bw + be;
  m_xs1 = bb;
  m_ys1 = bb;
  m_xs2 = RCW(&rc) - bb;
  m_ys2 = RCH(&rc) - bb;
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    if (ISHIT(c)) {
      m_active_pnt = m_move_pnt = GETHITV();
    }
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        for (i = 0; i < m_num_pnt; i++) {
          double xp = spline_calc_xp(i);
          double yp = spline_calc_yp(i);
          if (calc_distance(x - rc.l, y - rc.t, xp, yp) <= m_point_size + 1) {
            m_pdx = xp - x;
            m_pdy = yp - y;
            m_active_pnt = m_move_pnt = (i);
            SETHIT(c);
            SETHITV(m_move_pnt);
            force_redraw(c);
          }
        }
      }
      break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        m_move_pnt = GETHITV();
        if (m_move_pnt >= 0) {
          double xp = x + m_pdx;
          double yp = y + m_pdy;
          m_x[m_move_pnt] = (xp - m_xs1) / (m_xs2 - m_xs1);
          m_y[m_move_pnt] = (yp - m_ys1) / (m_ys2 - m_ys1);
          changed = 1;
          force_redraw(c);
        }
      }
      break;
    }
  }
  
  if (changed) {
    int i;
    for (i = 0; i < m_num_pnt; i++) {
      m_x[i] = BOUND(m_x[i], 0, 1);
      m_y[i] = BOUND(m_y[i], 0, 1);
      if (i == 0) {
        m_x[i] = 0.0;
      }
      else if (i == m_num_pnt - 1) {
        m_x[i] = 1.0;
      }
      else {
        if (m_x[i] < m_x[i - 1] + 0.001) {
          m_x[i] = m_x[i - 1] + 0.001;
        }
        if (m_x[i] > m_x[i + 1] - 0.001) {
          m_x[i] = m_x[i + 1] - 0.001;
        }
      }
    }
    {
      double m_am[32], dn = nvalues - 1;
      bspline_set(m_num_pnt, m_x, m_y, m_am);
      for (i = 0; i < nvalues; i++) {
        m_values[i] = bspline_get(m_num_pnt, m_x, m_y, m_am, i / dn);
        m_values[i] = BOUND(m_values[i], 0, 1.);
      }
    }
  }

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
      be = m_border_extra;
      // Background Border
      gcEdgeRect1(g, rc, m_background_color, m_border_color, m_border_width);
      // Curve
      n = 0;
      pt[n++] = dPOINT(m_xs1, m_ys1 + (m_ys2 - m_ys1) * m_values[0]);
      for (i = 1; i < 256; i++) {
        x = (i) / 255.0, y = m_values[i];
        pt[n++] = dPOINT(m_xs1 + (m_xs2 - m_xs1) * x,
            m_ys1 + (m_ys2 - m_ys1) * y);
      }
      gcPen(g, m_curve_color, m_curve_width);
      gcDrawPolygon(g, pt, n, 0, GC_DOUBLE, sizeof(DPOINT));
      // Inactive points
      for (i = 0; i < m_num_pnt; i++) {
        // Active point
        x = spline_calc_xp(i);
        y = spline_calc_yp(i);
        clr = i != m_active_pnt ? m_inactive_pnt_color : m_active_pnt_color;
        gcSolidBrush(g, clr);
        gcFillEllipse2(g, x, y, m_point_size, m_point_size);
      }
      //XDRAW_END();
    }
  
  return changed;
}

#undef spline_calc_xp
#undef spline_calc_yp



#include "spline_ctrl.inl"
#endif // _CTRL_INL_
