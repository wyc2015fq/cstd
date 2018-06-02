#ifndef _IMUI_CTRL_INL_
#define _IMUI_CTRL_INL_
#if 0
ImU32 imuiGetColorU32(ImGuiCol idx)
{
  ImGuiStyle* style = GetImGuiStyle();
  ImU32 c = style->Colors[idx];
  //c.w *= GImGui->Style.Alpha * alpha_mul;
  return (c);
}
#else
#define imuiGetColorU32(idx) style->Colors[idx]
#endif
// Render a rectangle shaped with optional rounding and borders
void imuiRenderFrame(gc_t* ctx, IRECT rc, ImColor fill_col, bool border, float rounding, int flag)
{
  ImGuiStyle* style = GetImGuiStyle();
  ImColor clr = imuiGetColorU32(ImGuiCol_BorderShadow);
  gcEdgeRect1(ctx, rc, fill_col, clr, 1);
}
int ImFontID_CalcFontSize(texfont_t* font)
{
  int size = 0;
  size = font->FontSize;
  return size;
}
int ImWindow_TitleBarHeight(ImGuiWindowFlags flags)
{
  GETGUIIO1();
  ImGuiStyle* style = GetImGuiStyle();
  return (flags & ImGuiWindowFlags_NoTitleBar) ? 0.0f : ImFontID_CalcFontSize(io->font) + style->FramePadding.y * 2.0f;
}
#define SET_ALPHA(color, alpha) ((color) & 0x00ffffff) | ((uchar)(alpha)<<24)
#define set_alpha(color, alpha) SET_ALPHA(color, (alpha*255))
//#include "ctrl.inl"
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
    window_t* last = c;
    for (; last->next; last = last->next) {
    }
    last->next = c;
    c->prev = last;
    c->next = NULL;
  }
}
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
} ImGuiWindowHitValue;
static int window_calcrect(IRECT rect, ImGuiWindowFlags flags, IRECT* prc)
{
  IRECT rc = rect;
  int has_cycaption = !(flags & ImGuiWindowFlags_NoTitleBar);
  int has_bordr = 1;
  //int cxicon = 16;
  //int cxsize = GetSystemMetrics(SM_CXSIZE);
  //int cysize = GetSystemMetrics(SM_CYSIZE);
  //int cxborder = GetSystemMetrics(SM_CXBORDER);
  //int cyborder = GetSystemMetrics(SM_CYBORDER);
  int cxdlgframe = 4;
  int TitleBarHeight = ImWindow_TitleBarHeight(flags);
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
    iRectOffset(&rc, b, b, -b, -b);
  }
  if (d > 0) {
    t = 2;
    b = 2;
    rc = rect;
    iRectOffset(&rc, b, b, -b, -b);
    iRectCutT(&rc, d, 0, &prc[Hit_CAPTION]);
    prc[Hit_CLIENT] = rc;
    rc = prc[Hit_CAPTION];
    iRectOffset(&rc, t, t, -t, -1);
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
static int window_hittest(IRECT rect, ImGuiWindowFlags flags, int x, int y, const IRECT* pos)
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
int pushrect(IRECT rc)
{
  GETGUIIO2();
  io->rcs[io->nrcs++] = rc;
  return io->nrcs - 1;
}
int poprect()
{
  GETGUIIO2();
  io->nrcs--;
  return 0;
}
bool window_begin(window_t* c, const char* label, IRECT* prc, bool* p_open, ImGuiWindowFlags flags)
{
  GUIIO_BEGIN();
  static IRECT rc1;
  const IRECT rc = *prc;
  int hot = -1, x = io->MousePos.x, y = io->MousePos.y;
  int ret = 0, ismax = 0;
  int has_bdr = 1;
  IRECT pos[Hit_RECTNUM] = {0};
  int font_sz = ImFontID_CalcFontSize(io->font);
  io->CurrentWindow = c;
  if (1) {
    window_calcrect(rc, flags, pos);
    *layer_push(0, pos[Hit_CLIENT], 1) = pos[Hit_CLIENT];
  }
  //IPOINT sz = iPOINT(font_sz, font_sz);
  //TraceMsg(hwnd, uMsg, nChar, lParam);
  if (ISHOVEREDWIN(c)) {
    hot = window_hittest(rc, flags, x, y, pos);
    switch (io->lastMsg) {
    case MSG_LBUTTONDOWN:
      if (iPtInRect(&rc, x, y)) {
        SETHITV(hot);
        rc1 = rc;
        SETHIT(c);
        window_addtolast(c);
        SETFOCUSWIN(c);
      }
      break;
    case MSG_LBUTTONDBLCLK:
      if (iPtInRect(&rc1, x, y)) {
        SETHITV(hot);
        if (Hit_CAPTION == io->HitValue) {
          //window_cmd_proc_max(s);
          FORCE_REDRAW(c);
        }
      }
      break;
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (Hit_CLOSE == io->HitValue || Hit_MAXBUTTON == io->HitValue || Hit_MINBUTTON == io->HitValue) {
          int hit = hot;
          if (io->HitValue == hit) {
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
          FORCE_REDRAW(c);
        }
        SETHITV(0);
      }
      break;
    case MSG_MOUSEMOVE:
      //myScreenToClient(hwnd, &x, &y);
      if (ISHIT(c)) {
        if (!ismax) {
          IRECT rcoff = {0};
          IPOINT lpt = {RCW(&rc1) - 100, RCH(&rc1) - 100};
          int dx, dy, donul = 0;
          dx = x - io->lastMouseDownPos.x, dy = y - io->lastMouseDownPos.y;
          //s->hot = hit;
          switch (io->HitValue) {
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
            *prc = rc1;
            iRectAdd(prc, &rcoff);
            //rc1 = ctrl_calc_rect_impl(c, rc1);
            //ctrl_setrect(c, rc1);
            //xMoveWindow(hwnd, rc1, 1, 1);
            //printf("%d %d %d %d %d %d %s\n", rc1.l, rc1.t, rcoff.l, rcoff.t, rcoff.r, rcoff.b, c->text);
            //c->rc = rc1;
            //s->x = x;
            //s->y = y;
            FORCE_REDRAW(c);
          }
          //printf("%d %d %d %d\n", x, y, rc.l, rc.t);
        }
      }
      {
        window_calcrect(rc, flags, pos);
        hot = window_hittest(rc, flags, x, y, pos);
        if (has_bdr && !ismax) {
          //ctrl_setcursor(c, hit);
        }
        if (hot > 0) {
          if (Hit_CLOSE == hot || Hit_MAXBUTTON == hot || Hit_MINBUTTON == hot) {
            FORCE_REDRAW(c);
          }
          if (Hit_CLOSE == hot || Hit_MAXBUTTON == hot || Hit_MINBUTTON == hot) {
            FORCE_REDRAW(c);
          }
        }
      }
      break;
    }
  }
  c->rect = *prc;
  strncpy(c->text, label, 16);
  if (1) {
    int i = 0;
    COLOR clr = 0, clrText;
    static const int g_hitbtn[3] = {Hit_CLOSE, Hit_MAXBUTTON, Hit_MINBUTTON};
    int tt[] = {IdClose, ismax ? IdRestore : IdMaximize, IdMinimize};
    IRECT rc1;
    IRECT rc2 = rc;
    //IRECT rcclip = rc;
    int has_cap = !(flags & ImGuiWindowFlags_NoTitleBar);
    int style = 0;
    //iRectOff0(&rcclip);
    //iRectOff0(&rc);
    rc1 = rc;
    rc2 = rc;
    window_calcrect(rc, flags, pos);
    //iRectOffset(&rc2, -c->rc.l, -c->rc.t, -c->rc.l, -c->rc.t);
    //printf("WM_PAINT\n");
    //window_calcrect(rc2, s->rcs, s->has_cap, has_bdr && !ismax);
    switch (style) {
    case 0: {
      COLOR clrFace = myGetSysColor(CLR_BTNFACE);
      gcEdgeRectSys(ctx, rc2, BDS_HIGH, (has_bdr && !ismax) ? 15 : 0, clrFace);
      if (has_cap) {
        COLOR clr1 = ISFOCUSWIN(c) ? _RGB(10, 36, 106) : _RGB(106, 106, 106);
        COLOR clr2 = ISFOCUSWIN(c) ? _RGB(166, 202, 240) : _RGB(240, 240, 240);
        ImDraw_gradientRect2(ctx, pos[Hit_CAPTION], clr1, clr2, 1);
        for (i = 0; i < 3; ++i) {
          int pushed = (ISHIT(c)) && io->HitValue == g_hitbtn[i];
          IRECT rc1 = pos[g_hitbtn[i]];
          gcEdgeRectSys(ctx, rc1, pushed ? BDS_SUNKEN : BDS_HIGH, 15, clrFace);
          rc1.l += 2 * pushed - 1;
          //imdraw_menuimg(ctx, &rcclip, rc1, tt[i], Color_Black);
        }
      }
      if (hot >= 0 && hot < Hit_CLIENT) {
        gcSolidRect2(ctx, pos[hot], _rgba(1, 0, 0, 0.2));
      }
      if (has_cap) {
        rc1 = pos[Hit_CAPTION];
        rc1.l = pos[Hit_SYSMENU].r;
        clrText = Color_White;
        //rcclip.r = pos[Hit_MINBUTTON].l;
        gcSolidText(ctx, rc1, label, NULL, 0, TF_VCENTER | TF_NoWrap, clrText);
      }
    }
    break;
    case 1: {
      COLOR clrs[] = {Color_Blue, Color_Blue, Color_Blue};
      gcEdgeRect2(ctx, rc2, countof(clrs), Color_White, clrs, clrs, clrs, clrs);
      if (has_cap) {
        //imdraw_rect_gradient(ctx, pos[Hit_CAPTION], _RGB(10, 36, 106), _RGB(166, 202, 240), 1);
        gcSolidRect2(ctx, pos[Hit_CAPTION], Color_Green);
        for (i = 0; i < 3; ++i) {
          int hoted = hot == g_hitbtn[i];
          int pushed = io->HitValue == g_hitbtn[i];
          IRECT rc1 = pos[g_hitbtn[i]];
          gcSolidRect2(ctx, rc1, pushed ? Color_Gray : hoted ? Color_Khaki : Color_White);
          rc1.l += 2 * pushed - 1;
          //imdraw_menuimg(ctx, &rcclip, rc1, tt[i], Color_Black);
        }
      }
      clrText = Color_White;
      if (has_cap) {
        rc1 = pos[Hit_CAPTION];
        rc1.l = pos[Hit_SYSMENU].r;
        //rcclip.r = pos[Hit_MINBUTTON].l;
        gcSolidText(ctx, rc1, label, NULL, 0, TF_VCENTER | TF_NoWrap, clrText);
      }
    }
    break;
    case 2: {
      const ImGuiStyle* style = GetImGuiStyle();
      int TitleBarHeight = ImWindow_TitleBarHeight(flags);
      ImGuiCol bg_color_idx;
      COLOR bg_color;
      float window_rounding;
      if (flags & ImGuiWindowFlags_NoInputs) {
        flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
      }
      bg_color_idx = ImGuiCol_WindowBg;
      if ((flags & ImGuiWindowFlags_ComboBox) != 0) {
        bg_color_idx = ImGuiCol_ComboBg;
      }
      else if ((flags & ImGuiWindowFlags_Tooltip) != 0 || (flags & ImGuiWindowFlags_Popup) != 0) {
        bg_color_idx = ImGuiCol_PopupBg;
      }
      else if ((flags & ImGuiWindowFlags_ChildWindow) != 0) {
        bg_color_idx = ImGuiCol_ChildWindowBg;
      }
      bg_color = style->Colors[bg_color_idx];
      //if (bg_alpha >= 0.0f) { bg_color = set_alpha(bg_color, bg_alpha); }
      window_rounding = (flags & ImGuiWindowFlags_ChildWindow) ? style->ChildWindowRounding : style->WindowRounding;
      gcFillColor(ctx, bg_color);
      gcFillRoundRect2(ctx, rc.l, rc.t + TitleBarHeight, RCW(&rc), RCH(&rc) - TitleBarHeight, window_rounding, (flags & ImGuiWindowFlags_NoTitleBar) ? 0 : 3);
      clrText = imuiGetColorU32(ImGuiCol_Text);
      // Title bar
      if (has_cap) {
        COLOR clrTitleBar = imuiGetColorU32(ISFOCUSWIN(c) ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg);
        gcFillColor(ctx, clrTitleBar);
        gcFillRoundRect2(ctx, rc.l, rc.t, RCW(&rc), TitleBarHeight, window_rounding, 12);
        gcSolidText(ctx, iRECT2(rc.l + TitleBarHeight, rc.t, RCW(&rc), TitleBarHeight),
            label, NULL, 0, TF_VCENTER | TF_NoWrap, imuiGetColorU32(ImGuiCol_Text));
      }
    }
    break;
    }
  }
  return true;
}
int window_end()
{
  GETGUIIO1();
  window_t* c = io->CurrentWindow;
  layer_pop();
  GUIIO_END();
  return 0;
}
BOOL button_ctrl(void* c, const char* label)
{
  GETGUIIO2();
  int clicked = 0, hot = 0;
  int fmt = TF_CENTER | TF_VCENTER | TF_NoWrap;
  IRECT rc = {0};
  if (1) {
    ISIZE sz = imuimeasureText(label, NULL, io->font, io->layoutflags, NULL);
    if (!calc_rect(sz.w + 6, sz.h + 6, io->layoutflags, &rc)) {
      return 0;
    }
  }
  if (ISHOVEREDWIN2()) {
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rc, x, y);
    switch (io->lastMsg) {
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
        FORCE_REDRAW(c);
        SETFOCUS(c);
      }
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        if (ISHOT(c) != hot) {
          SETHOT2(c, hot);
          FORCE_REDRAW(c);
        }
      }
      break;
    }
  }
  if (1) {
    int hit = ISHIT(c);
    COLOR clrText = Color_Black;
    int btn_style = 1;
    //iRectOff0(&rc);
    //switch (c->sys->style)
    //case UIS_WIN2K:
    switch (btn_style) {
    case 1:
      if (1) {
        COLOR clrFace = myGetSysColor(CLR_BTNFACE);
        clrText = Color_Black;
        gcEdgeRectSys(ctx, rc, hit ? BDS_SUNKEN : BDS_HIGH, 15, clrFace);
        if (hot) {
          gcEdgeRect1(ctx, rc, _rgba(1, 1, 1, 0.2), 0, 0);
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
        //draw_roundrectR(ctx, &rcclip1, 5, 5, m_border_color, 0);
        gcEdgeRect1(ctx, rc, 0, m_border_color, 1);
        //draw_roundrect_aaR( im, &rect, 5, 5, s->m_border_color, 0 );
        if (1) {
          IRECT rc1 = rc;
          iRectIn(&rc1, 1);
          //InflateRect(&rc1, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));
          ImDraw_gradientRect(ctx, rc1, 0, 2, NULL, crBack);
        }
      }
      break;
    case 3:
      if (1) {
        ImGuiStyle* style = GetImGuiStyle();
        //printf("%d %d \n", hovered, held);
        ImU32 col = imuiGetColorU32(hit ? ImGuiCol_ButtonActive : hot ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        //IRECT rc2 = rc;
        //gcClipRectR(ctx, rc);
        imuiRenderFrame(ctx, rc, col, TRUE, style->FrameRounding, 0);
        //imuimeasureText(label, NULL, 0, fmt, &rc2);
        clrText = imuiGetColorU32(ImGuiCol_Text);
      }
      break;
    }
    if (label) {
      rc.l += !!hit, rc.t += !!hit;
      gcSolidText(ctx, rc, label, NULL, 0, fmt, clrText);
    }
  }
  return clicked;
}
int slider_float_ctrl_impl(void* c, IRECT rc, const char* lable_fmt, float* v, float v_min, float v_max, BOOL is_int)
{
  GETGUIIO2();
  IRECT rc1 = rc, grabrc;
  static float v2 = 0;
  float ss;
  int h, w, changed = 0;
  iRectIn(&rc1, 2);
  h = RCH(&rc1), w = (int)(h * 0.75);
  *v = BOUND(*v, v_min, v_max);
  ss = (RCW(&rc1) - w) / (v_max - v_min);
  grabrc = iRECT2(rc1.l + (int)((*v - v_min) * ss), rc1.t, w, h);
  if (ISHOVEREDWIN2()) {
    switch (io->lastMsg) {
    case MSG_LBUTTONDOWN:
      if (iPtInRect2(&rc, io->MousePos)) {
        SETHIT(c);
        FORCE_REDRAW(c);
        SETFOCUS(c);
        //if (SETHITV(iPtInRect2(&grabrc, io->MousePos))) { }
        *v = (io->MousePos.x - (w / 2) - rc1.l) / ss;
        v2 = *v;
        changed = 1;
        io->lastMsg = MSG_NOP;
      }
      break;
    case MSG_MOUSEMOVE:
      if (ISHIT(c)) {
        *v = v2 + (io->MousePos.x - io->lastMouseDownPos.x) / ss;
        changed = 1;
      }
      if (iPtInRect2(&rc, io->MousePos)) {
        SETHOT(c);
      }
      break;
    }
  }
  if (changed) {
    *v = BOUND(*v, v_min, v_max);
    grabrc = iRECT2(rc1.l + (int)((*v - v_min) * ss), rc1.t, w, h);
  }
  {
    const ImGuiStyle* style = GetImGuiStyle();
    char buf[64];
    COLOR clr = imuiGetColorU32(ImGuiCol_FrameBg);
    imuiRenderFrame(ctx, rc, clr, true, style->FrameRounding, 0);
    if (is_int) {
      _snprintf(buf, 64, lable_fmt, (int)*v);
    }
    else {
      _snprintf(buf, 64, lable_fmt, *v);
    }
    //imuiRenderFrame(ctx, grabrc, imuiGetColorU32(changed ? ImGuiCol_SliderGrabActive : ISHOT(c) ? ImGuiCol_SliderGrabHovered : ImGuiCol_SliderGrab), true, style->GrabRounding, 0);
    gcEdgeRect1(ctx, grabrc, imuiGetColorU32(changed ? ImGuiCol_SliderGrabActive : ISHOT(c) ? ImGuiCol_SliderGrabHovered : ImGuiCol_SliderGrab), imuiGetColorU32(ImGuiCol_Border), 1);
    gcSolidText(ctx, rc, buf, NULL, 0, TF_VCENTER | TF_NoWrap, imuiGetColorU32(ImGuiCol_Text));
  }
  return changed;
}
int slider_float_ctrl(IRECT rc, const char* lable_fmt, float* v, float v_min, float v_max)
{
  return slider_float_ctrl_impl(v, rc, lable_fmt, v, v_min, v_max, 0);
}
int slider_int_ctrl(IRECT rc, const char* lable_fmt, int* v, float v_min, float v_max)
{
  float fv = *v;
  int ret = slider_float_ctrl_impl(v, rc, lable_fmt, &fv, v_min, v_max, 1);
  *v = (int)fv;
  return ret;
}
CC_INLINE int groupbox_calcrect(IRECT rc, const char* text, const char* text_end, texfont_t* font, IRECT* outrc)
{
  int d = imuimeasureText(text, text_end, font, 0, NULL).h;
  *outrc = rc;
  iRectOffset(outrc, 4, d, -4, -4);
  return 0;
}
static int groupbox_draw(gc_t* ctx, IRECT rc, const char* text, const char* text_end, BorderStyle bds, COLOR crBk, texfont_t* font, int fmt)
{
  COLOR clr = crBk;
  //DrawSunken3dFrame(hDC, rect3d);
  if (crBk) {
    //imdraw_fillrect(ctx, &rcclip, rc, crBk);
  }
  if (1) {
    IRECT rc1 = rc, rc2;
    ISIZE sz = imuimeasureText(text, text_end, font, 0, NULL);
    int d = sz.h / 2;
    rc1.t += d;
    iRectCutL(&rc1, d + 2, 0, &rc2);
    gcEdgeRectSys(ctx, rc2, bds, BDM_L | BDM_T | BDM_B, 0);
    iRectCutL(&rc1, sz.w + 4, 0, &rc2);
    rc2.t -= d;
    gcEdgeRectSys(ctx, rc2, bds, BDM_B, 0);
    gcEdgeRectSys(ctx, rc1, bds, BDM_R | BDM_T | BDM_B, 0);
    if (text) {
      gcSolidText(ctx, rc2, text, text_end, font, fmt, Color_Black);
    }
  }
  return 0;
}
static int groupbox_ctrl(const char* label)
{
  GETGUIIO2();
  IRECT rc = {0}, rcSub = {0};
  int x = io->MousePos.x, y = io->MousePos.y;
  int clicked = 0, hot = iPtInRect(&rc, x, y);
  int ret = 1;
  int fmt = TF_CENTER;
  if (1) {
    ISIZE sz = imuimeasureText(label, NULL, io->font, fmt, NULL);
    if (!calc_rect(sz.w + 6, sz.h + 20, 0, &rc)) {
      return 0;
    }
  }
  {
    groupbox_calcrect(rc, label, NULL, io->font, &rcSub);
    io->rcs[io->nrcs++] = rcSub;
  }
  if (1) {
    //int fwSizeType = nChar; // resizing flag
    if (1) {
      BorderStyle bds = BDS_FRAME;
      COLOR clr = Color_White;
      if (1) {
        bds = BDS_ETCHED;
        clr = myGetSysColor(CLR_3DFACE);
      }
      else {
        bds = BDS_SIMPLE;
        clr = Color_White;
      }
      groupbox_draw(ctx, rc, label, NULL, bds, clr, io->font, fmt);
    }
  }
  return ret;
}
int linearlayout_begin(const char* label, BOOL horz, bool can_change, int count, float* v)
{
  GETGUIIO2();
  void* c = v;
  int x = io->MousePos.x, y = io->MousePos.y;
  int i, changed = 0, hit = 0, hot = 0, HotValue = -1;
  static float v0, v1;
  const float max_v = 2;
  int gap = io->gap;
  float ss;
  IRECT rc = {0};
  if (1) {
    IRECT rect = {0};
    int x = 0, y = 0;
    {
      layerinfo_t* li;
      li = io->layinfo + io->deep;
      x += host->lsz[host->ilsz].sz.x;
      y += host->lsz[host->ilsz].sz.y;
      host->lsz[host->ilsz].sz = iPOINT(0, 0);
      ASSERT(host->ilsz < 100);
    }
    if (count > 0) {
      if (horz) {
        x += gap * (count - 1);
      }
      else {
        y += gap * (count - 1);
      }
    }
    if (label) {
      ISIZE sz = imuimeasureText(label, NULL, io->font, 0, NULL);
      x += sz.w + 8;
      y += sz.h + 6;
    }
    if (!calc_rect(x, y, io->layoutflags, &rc)) {
      return 0;
    }
  }
  if (label) {
    IRECT rcSub = {0};
    groupbox_calcrect(rc, label, NULL, io->font, &rcSub);
    //int fwSizeType = nChar; // resizing flag
    if (1) {
      BorderStyle bds = BDS_FRAME;
      COLOR clr = Color_White;
      if (1) {
        bds = BDS_ETCHED;
        clr = myGetSysColor(CLR_3DFACE);
      }
      else {
        bds = BDS_SIMPLE;
        clr = Color_White;
      }
      groupbox_draw(ctx, rc, label, NULL, bds, clr, io->font, TF_CENTER);
    }
    rc = rcSub;
  }
  if (1) {
    IRECT* prc;
    prc = layer_push(horz, rc, count);
    ss = iRectSplit(rc, horz, gap, count, v, prc);
  }
  if (can_change) {
    if (ISHOVEREDWIN2()) {
      switch (io->lastMsg) {
      case MSG_LBUTTONDOWN:
        if (iPtInRect2(&rc, io->MousePos) && (hit = iPtInRectsBorder(rc, horz, count, v, gap, x, y, 2)) >= 0) {
          SETHIT(c);
          SETHITV(hit);
          FORCE_REDRAW(c);
          SETFOCUS(c);
          //v = (io->MousePos.x - (gap/2) - prc[i].l)/ss;
          v0 = v[io->HitValue + 0];
          v1 = v[io->HitValue + 1];
          changed = 1;
          io->lastMsg = MSG_NOP;
        }
        break;
      case MSG_MOUSEMOVE:
        i = io->HitValue;
        if (ISHIT(c) && i >= 0 && i < count - 1) {
          int j, off = 0;
          float v3 = 0, v2 = 0;
          for (j = 0; j < i - 1; ++j) {
            v3 += v[j] + gap;
          }
          v2 = v3 + v[i] + v[i + 1];
          if (horz) {
            int x2 = BOUND(io->MousePos.x, rc.l + v3 + gap, rc.l + v2 - gap);
            off = x2 - io->lastMouseDownPos.x;
          }
          else {
            int x2 = BOUND(io->MousePos.y, rc.t + v3 + gap, rc.t + v2 - gap);
            off = x2 - io->lastMouseDownPos.y;
          }
          v[i] = v0 + off;
          v[i + 1] = v1 - off;
          v[i + 1] = MAX(v[i + 1], 0);
          changed = 1;
        }
        if (iPtInRect2(&rc, io->MousePos) && (HotValue = iPtInRectsBorder(rc, horz, count, v, gap, x, y, 2)) >= 0) {
          SETHOT(c);
          hot = 1;
        }
        break;
      }
    }
    if (changed) {
      //iRectLayout(rc, horz, gap, count, v, prc);
    }
  }
  if (count) {
    const ImGuiStyle* style = GetImGuiStyle();
    int i;
    float v2 = horz ? rc.l : rc.t;
    for (i = 0; i < (count - 1); ++i) {
      float v1 = v2 + v[i];
      IRECT rcb = horz ? iRECT(v1, rc.t, v1 + gap, rc.b) : iRECT(rc.l, v1, rc.r, v1 + gap);
      int clridx = ((ISHIT(c) && (io->HitValue == i)) ? ImGuiCol_LayoutSplitActive : (hot && (HotValue == i)) ? ImGuiCol_LayoutSplitHovered : ImGuiCol_LayoutSplit);
      gcEdgeRect1(ctx, rcb, imuiGetColorU32(clridx), imuiGetColorU32(ImGuiCol_Border), 1);
      v2 = v1 + gap;
    }
  }
  return 1;
}
static int autolayout_beginV(const char* fmt, va_list args)
{
  GETGUIIO2();
  IRECT rc = {0};
  char TempBuffer[4028];
  const int fmtFlag = TF_VCENTER | TF_LEFT;
  const char* text_end = TempBuffer + _vsnprintf(TempBuffer, IM_ARRAYSIZE(TempBuffer), fmt, args);
  int ret = linearlayout_begin(TempBuffer, 1, 0, 0, 0);
  return ret;
}
static int autolayout_begin(const char* fmt, ...)
{
  int ret;
  if (fmt) {
    va_list args;
    va_start(args, fmt);
    ret = autolayout_beginV(fmt, args);
    va_end(args);
  }
  else {
    ret = linearlayout_begin(NULL, 1, 0, 0, 0);
  }
  return ret;
}
int linearlayout_end()
{
  layer_pop();
  return 0;
}
static int pushbutton_ctrl(window_t* c, IRECT rc, const char* label, ImGuiButtonFlags flags)
{
  GUIIO_BEGIN();
  int x = io->MousePos.x, y = io->MousePos.y;
  switch (io->lastMsg) {
  case MSG_TIMER:
    if (1) {
    }
    break;
  case MSG_MOUSEMOVE:
    //printf("WM_MOUSEMOVE\n");
    break;
  }
  if (1) {
    iRectOff0(&rc);
    gcSolidCircle(ctx, (rc.l + rc.r) / 2., (rc.t + rc.b) / 2., (rc.r - rc.l) / 2., _RGB(255, 0, 0), _RGB(0, 0, 255), 1);
  }
  GUIIO_END();
  return 0;
}
static int label_ctrlV(const char* fmt, va_list args)
{
  GETGUIIO2();
  IRECT rc = {0};
  char TempBuffer[4028];
  const int fmtFlag = TF_VCENTER | TF_LEFT;
  const char* text_end = TempBuffer + _vsnprintf(TempBuffer, IM_ARRAYSIZE(TempBuffer), fmt, args);
  //int x = io->MousePos.x, y = io->MousePos.y;
  if (1) {
    ISIZE sz = imuimeasureText(TempBuffer, NULL, io->font, fmtFlag, NULL);
    if (!calc_rect(sz.w + 6, sz.h + 6, io->layoutflags, &rc)) {
      return 0;
    }
  }
  if (1) {
    gcSolidText(ctx, rc, TempBuffer, text_end, 0, fmtFlag, Color_Black);
  }
  return 0;
}
static int label_ctrl(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  label_ctrlV(fmt, args);
  va_end(args);
  return 0;
}
int tooltip_ctrl(const char* label)
{
  GETGUIIO1();
  window_t* c = host->tooltip;
  gc_t* ctx = c->ctx;
  int fmt = TF_VCENTER | TF_CENTER;
  ISIZE sz = imuimeasureText(label, NULL, io->font, fmt, NULL);
  IRECT rc = iRECT2(io->MousePos.x + 20, io->MousePos.y + 20, sz.w + 6, sz.h + 10);
  c->rect = rc;
  if (1) {
    COLOR clr = myGetSysColor(CLR_INFOBK);
    gcEdgeRect1(ctx, rc, clr, _RGB(0, 0, 0), 1);
    gcSolidText(ctx, rc, label, NULL, io->font, fmt, Color_Black);
    //nHeight = dc.DrawText(m_strTitle, rect, 0 | TF_LEFT | TF_EDITCONTROL | TF_NOPREFIX | TF_WORDBREAK );
  }
  return 0;
}
//#define XDRAW_BEG(ctx) {img_t im2[1] = {0}; imsubref(ctx, rc, im2); rc = iRECT(0, 0, im2->w, im2->h); im = im2;
//#define XDRAW_END() }
static int rbox_ctrl_get_row_height(int fmt)
{
  GETGUIIO2();
  ISIZE sz = imuimeasureText("H", NULL, io->font, fmt, NULL);
  return MAX(20, sz.h + 2);
}
static int rbox_ctrl_hittest(int x, int y, int nitem, int fmt)
{
  int i;
  int dy = rbox_ctrl_get_row_height(fmt);
  for (i = 0; i < nitem; i++) {
    if (y > (i)*dy && y < (i + 1)*dy) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int rbox_ctrl(const char* label, int nitem, const char** items, int* pid, int flag)
{
  GETGUIIO2();
  void* c = pid;
  int i, hit = *pid, hot = 0, changed = 0, hotid = -1, x = io->MousePos.x, y = io->MousePos.y;
  int fmt = TF_VCENTER;
  IRECT rc;
  IRECT rcLabel = {0}, rect;
  hit = BOUND(hit, 0, nitem);
  if (1) {
    int w = 0, h = rbox_ctrl_get_row_height(fmt);
    int x1, y1;
    for (i = 0; i < nitem; ++i) {
      ISIZE sz2 = imuimeasureText(items[i], NULL, io->font, fmt, NULL);
      w = MAX(w, sz2.w);
    }
    x1 = w + h + 4;
    y1 = h * nitem + 2;
    if (!calc_rect(x1, y1, flag, &rc)) {
      return 0;
    }
    if (label) {
      iRectCutT(&rc, h, 0, &rcLabel);
    }
  }
  rect = rc;
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    switch (io->lastMsg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c) && hot) {
        //hit = rbox_ctrl_hittest(x - rc.l, y - rc.t, nitem, fmt);
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        hit = rbox_ctrl_hittest(x - rc.l, y - rc.t, nitem, fmt);
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
        hotid = rbox_ctrl_hittest(x - rc.l, y - rc.t, nitem, fmt);
        if (ISHOT(c) != hot) {
          SETHOT2(c, hot);
          FORCE_REDRAW(c);
        }
      }
      break;
    }
  }
  {
    int i;
    int m_text_height = (int)imuimeasureText("H", NULL, io->font, fmt, NULL).h;
    int dy = rbox_ctrl_get_row_height(fmt);
    double m_text_thickness = (1.);
    COLOR m_text_color = (_rgb(0.0, 0.0, 0.0));
    COLOR m_inactive_color = (_rgb(0.0, 0.0, 0.0));
    COLOR m_active_color = (_rgb(0.4, 0.0, 0.0));
    COLOR m_background_color = (_rgb(1.0, 1.0, 0.9));
    COLOR m_border_color = (_rgb(0.0, 0.0, 0.0));
    IRECT rcItem, rcText, rc2 = rc;
    double r1 = m_text_height * 0.5; // 外圈半径
    double r2 = r1 * 0.4; // 内圆半径
    //gcClipRectR(ctx, rect);
    // Text
    {
      // background
      if (1) {
        m_active_color = Color_Black;
        m_background_color = myGetSysColor(CLR_3DFACE);
        gcEdgeRectSys(ctx, rect, BDS_ETCHED, 15, m_background_color);
        //gcEdgeRect1(ctx, rc, m_background_color, m_border_color, 1);
      }
      else if (1) {
        gcEdgeRect1(ctx, rect, m_background_color, m_border_color, 1);
      }
    }
    if (label) {
      //clr = myGetSysColor(CLR_3DFACE);
      //gcEdgeRectSys(ctx, rcLabel, BDS_SUNKEN, 15, clr);
      gcSolidText(ctx, rcLabel, label, NULL, io->font, fmt, m_text_color);
    }
    for (i = 0; i < nitem; ++i) {
      if (iRectCutT(&rc2, dy, 0, &rcText) || 1) {
        const char* str = items[i];
        int k = iRectCutL(&rcText, dy, 0, &rcItem);
        DPOINT pt1 = RCCENTERF(&rcItem);
        COLOR clr = (hotid == i) ? Color_Green : m_text_color;
        // Inactive items
        gcSolidCircle(ctx, pt1.x, pt1.y, r1, Color_White, clr, m_text_thickness);
        if (*pid == i) {
          gcSolidCircle(ctx, pt1.x, pt1.y, r2, Color_Green, 0, 0);
        }
        //draw_textR(ctx, str.s, str.l, &rcText, TF_VCENTER, m_text_color);
        gcSolidText(ctx, rcText, str, NULL, io->font, fmt, m_text_color);
      }
    }
    //gcClipReset(ctx);
  }
  return changed;
}
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
  GETGUIIO2();
  int char_w = imuimeasureText("H", NULL, io->font, 0, NULL).h / 2;
  return char_w;
}
int editctrl_get_line_h(const editctrl_t* s)
{
  GETGUIIO2();
  int line_h = imuimeasureText("H", NULL, io->font, 0, NULL).h;
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
  //char* str = (char*)s->sd->bf->data;
  //int len = s->sd->bf->len;
  //int i0 = str_skip_nline(str, len, 0, yy, "\n", 0);
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
CC_INLINE int edit_ctrl(const char* label, char* buf, int len)
{
  GETGUIIO2();
  static editctrl_t s11[1] = {0};
  editctrl_t* s = s11;
  int ret = 0, hot = 0;
  int x = io->MousePos.x, y = io->MousePos.y;
  IRECT rcc, rc = {0};
  int mg = 2;
  void* c = buf;
  IRECT rcLabel = {0};
  static int ttt = 0;
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
    ISIZE sz2 = {0};
    ISIZE sz = imuimeasureText("H", NULL, io->font, io->layoutflags, NULL);
    if (label) {
      sz2 = imuimeasureText(label, NULL, io->font, io->layoutflags, NULL);
      sz.w += sz2.w + 8;
    }
    if (!calc_rect(sz.w + 200, sz.h + 6, io->layoutflags, &rc)) {
      return 0;
    }
    if (label) {
      iRectCutL(&rc, sz2.w + 6, 4, &rcLabel);
      rc.l += 2;
    }
  }
  rcc = rc;
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rcc, x, y);
    switch (io->lastMsg) {
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
        if (io->MouseDoubleClicked[0]) {
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
          if (io->KeyShift) {
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
        uchar nChar = (uchar)io->lastChar;
        if (nChar >= 25 || '\n' == nChar || '\r' == nChar || '\t' == nChar) {
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
        UINT nChar = io->lastChar;
        //const char* cText = s->str->s + s->sd->pos;
        //printf("%d %d\n", nChar, GetKeyState(KEY_SHIFT));
        // 此处处理Ctrl+C、Ctrl+V和Ctrl+X操作
        switch (nChar) {
        case 13:
          if (!(s->fmt & TF_NoWrap)) {
            char str[8] = {'\n', 0};
            editctrl_ins_str(s, str, 1, 0);
          }
          break;
        case 'C':
          if (io->KeyCtrl) { //被按下
            str_t str[1] = {0};
            s->sd->get_sel(s->sd, str);
            sys_set_clipboard_text(str->s, str->l);
            str_free(str);
          }
          break;
        case 'V':
          if (io->KeyCtrl) { //被按下
            char* str = sys_get_clipboard_text_static();
            int slen = strlen(str);
            editctrl_ins_str(s, str, slen, 0);
            //editctrl_calc_rect(s, c->rc);
            free(str);
          }
          break;
        case 'X':
          if (io->KeyCtrl) { //被按下
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
            if (io->KeyShift) {
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
            if (io->KeyShift) {
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
      gcSolidText(ctx, rcLabel, label, NULL, io->font, fmt, crText);
    }
    gcEdgeRect1(ctx, rc, hot ? _rgba(1, 1, 1, 0.9) : _rgba(1, 1, 1, 0.5), crText, 1);
    //ctrl_get_rect(s->c, rc, 0, 0, &rc);
    RCOFFSET(&rc, mg, mg, -mg, -mg);
    gcClipRectR(ctx, rc);
    for (; (len = sd->get_line(sd, i, 0, range, countof(range), isfocus)) >= 0 && (rc.t + y) < rc.b; ++i) {
      //IRECT rc1 = iRectInter(rc, );
      IRECT rc1 = iRECT2(rc.l + s->org.x, rc.t + y, 9999, line_h);
      int beg = 0, end = 0;
      int begw = 0, endw = 0, w = 0, k = 0;
      for (j = 0; j < len; ++j) {
        char* str = (char*)s->sd->bf->data;
        ISIZE sz = imuimeasureText(str + range[j].bbeg, str + range[j].bend, io->font, s->fmt, NULL);
        rc1 = iRECT2(rc.l + w, rc.t + y, sz.w + 1, line_h);
        gcSolidRect2(ctx, rc1, range[j].bg);
        if (range[j].bg) {
        }
        if (s->fmt & TF_NoWrap) {
          //iRectAlign(rc1, sz.w, sz.h, TF_VCENTER, &rc1);
        }
        gcSolidText(ctx, rc1, str + range[j].bbeg, str + range[j].bend, io->font, s->fmt, range[j].fg);
        w += sz.w;
      }
      y += line_h;
    }
    gcClipReset(ctx);
    if (ISFOCUS(c)) {
      IRECT rcCursor = editctrl_get_cursor_rect(s, rc);
      int a = ((io->Time * 100 / (sys_utime_frequency()))) % 100;
      if (a > 50) {
        COLOR crCursor = _RGB(0, 0, 0);
        gcEdgeRect1(ctx, rcCursor, crCursor, 0, 0);
      }
    }
  }
  return ret;
}
static int cbox_ctrl(const char* label, int* v)
{
  GETGUIIO2();
  int x = io->MousePos.x, y = io->MousePos.y;
  int fmt = TF_VCENTER | TF_NoWrap, hot = 0, changed = 0;
  int m_text_height = 0;
  IRECT rc = {0};
  if (1) {
    ISIZE sz = imuimeasureText(label, NULL, io->font, io->layoutflags, NULL);
    if (!calc_rect(sz.w + sz.h + 20, sz.h + 6, io->layoutflags, &rc)) {
      return 0;
    }
    m_text_height = sz.h + 2;
  }
  if (ISHOVEREDWIN2()) {
    hot = iPtInRect(&rc, x, y);
    switch (io->lastMsg) {
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
    COLOR m_text_color = Color_Black;
    COLOR m_active_color = 1 ? _rgba(0, 0.5, 0, 1) : _rgba(0, 0.8, 0, 1);
    IRECT rcCheck, rect, rcCheckIn;
    int style = 1;
    int m_bIsDrawOnGlass = 0;
    BOOL bEnabled = 1;
    int bHighlighted = io->HitValue;
    rect = rc;
    iRectCutL(&rc, RCH(&rc), 2, &rcCheck);
    iRectAlign(rcCheck, m_text_height, m_text_height, TF_CENTER | TF_VCENTER, &rcCheck);
    rcCheckIn = rcCheck;
    iRectDeflate2(&rcCheckIn, 4, 4);
    switch (style) {
    case 0:
      gcEdgeRectSys(ctx, rcCheck, BDS_SUNKEN, 15, Color_White);
      if (*v) {
        gcEdgeRect1(ctx, rcCheckIn, m_active_color, 0, 0);
      }
      break;
    case 1:
      gcEdgeRect1(ctx, rcCheck, hot ? _rgba(1, 1, 1, 0.5) : Color_White, Color_Black, 2);
      if (*v) {
        gcEdgeRect1(ctx, rcCheckIn, m_active_color, 0, 0);
      }
      break;
    }
    gcSolidText(ctx, rc, label, NULL, io->font, fmt, m_text_color);
  }
  return changed;
}
#if 0
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
static int listbox(IRECT rc, const char* label)
{
  listbox_t* s = (listbox_t*)c->x;
  IRECT rcc;//, rc1 = c->rc;
  int x = io->MousePos.x, y = io->MousePos.y;
  int ret = 0, hit = -1;
  rcc = s->sc->rcc;
  switch (io->lastMsg) {
  case MSG_CTRLINIT:
    scrollbar_set(s->sc, c, c);
    ctrl_addsub_head(c, s->sc->c);
    s->sc->styles = WS_VSCROLL;
    return 0;
    break;
  case MSG_CTRLFREE:
    c->ctrlproc = 0;
    return 0;
    break;
  case LB_GETCURSEL:
    return s->cursel < s->nitem ? s->cursel : -1;
  case MSG_SETFOCUS:
    if (!s->hot) {
      s->hot = 1;
      FORCE_REDRAW(c);
    }
    break;
  case MSG_KILLFOCUS:
    if (s->hot) {
      s->hot = 0;
      FORCE_REDRAW(c);
    }
    break;
  case MSG_HSCROLL:
  case MSG_VSCROLL:
    if (sender == s->sc->c) {
      SCROLLINFO* p = &s->sc->si[uMsg == WM_VSCROLL];
      p->nPos = p->nTrackPos;
      //printf("WM_VSCROLL \n");
      FORCE_REDRAW(c);
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
  case MSG_GETMINMAXINFO: {
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
  case MSG_MOUSEMOVE:
    if (1) {
      hit = listbox_hittest(s, x, y);
      if (hit != s->hot) {
        s->hot = hit;
        FORCE_REDRAW(c);
      }
    }
    break;
  case MSG_MOUSEWHEEL:
    if (iPtInRect(&rcc, x, y)) {
      //s->org.y += GET_WHEEL_DELTA_WPARAM(wParam) / 3;
      SCROLLINFO* p = &s->sc->si[1];
      scrollbar_set_pos(p, p->nPos - (GET_WHEEL_DELTA_WPARAM(wParam)));
      FORCE_REDRAW(c);
    }
    break;
  case MSG_LBUTTONDBLCLK:
  case MSG_LBUTTONDOWN:
    if (iPtInRect(&rcc, x, y)) {
      //UISetFocus(s);
      s->ldown = TRUE;
      s->rdown = 0;
      // 窗体获得焦点以后设置光标的位置
      // 从当前鼠标的位置获得对应字符的序号
      hit = listbox_hittest(s, x, y);
      if (GetKeyState(KEY_SHIFT) >= 0) {
      }
      s->hot = hit;
      ctrl_setfocus(c);
      return 1;
      //listbox_update_org(s);
    }
    break;
  case MSG_LBUTTONUP:
    if (s->ldown && iPtInRect(&rcc, x, y)) {
      hit = listbox_hittest(s, x, y);
      s->ldown = 0;
      if (hit != s->cursel) {
        s->cursel = hit;
        FORCE_REDRAW(c);
      }
      sendmsg(c, c->listener, WM_COMMAND, LBN_SELCHANGE, hit);
    }
    //FORCE_REDRAW(c);
    break;
  case MSG_PAINT:
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
      //FORCE_REDRAW(c);
    }
    if (1) {
      IRECT rc = s->sc->rcc;
      IRECT rcclip = c->rcclip;
      int i, y1;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      y1 = rc.t - s->sc->si[1].nPos;
      imdraw_rect(ctx, 0, rc, _rgb(1, 1, 1), 0, 0, 0);
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
#endif
int popup_begin(BOOL* popened)
{
  if (*popened) {
    GETGUIIO1();
    window_t* c = host->popup + host->pop_deep++;
    gc_t* ctx = ImGuiBase_Begin(io, host, c);
    IRECT rc = c->rect;
    int hot = 0;
    if (1) {
      *layer_push(0, rc, 1) = rc;
    }
    if (1) {
      int x = io->MousePos.x, y = io->MousePos.y;
      hot = iPtInRect(&rc, x, y);
      switch (io->lastMsg) {
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
int drag(void* c, IRECT rc, int gap, int count, int* v, int x, int y)
{
  GETGUIIO2();
  int i, changed = 0, hit = -1;
  static int v0;
  switch (io->lastMsg) {
  case MSG_LBUTTONDOWN:
    if (iPtInRect2(&rc, io->MousePos) && (hit = drag_hittest(rc, count, v, gap, x, y)) >= 0) {
      SETHIT(c);
      SETHITV(hit);
      FORCE_REDRAW(c);
      //v = (io->MousePos.x - (gap/2) - prc[i].l)/ss;
      v0 = v[hit];
      changed = 1;
      io->lastMsg = MSG_NOP;
    }
    break;
  case MSG_MOUSEMOVE:
    i = io->HitValue;
    if (ISHIT(c) && i >= 0 && i < count) {
      int off = 0;
      int v1 = 0;
      int v2 = RCW(&rc);
      int j;
      for (j = 0; j < i - 1; ++j) {
        v1 += v[j];
      }
      {
        int x2 = BOUND(io->MousePos.x, rc.l + v1 + gap, rc.l + v2 - gap);
        off = x2 - io->lastMouseDownPos.x;
      }
      v[i] = v0 + off;
      changed = 1;
    }
    break;
  }
  return changed;
}
int header(int n, const char** strs, int* width, int* psel)
{
  GETGUIIO2();
  static int sel = 0;
  int x = io->MousePos.x, y = io->MousePos.y, w;
  int i, fmt = TF_CENTER | TF_VCENTER | TF_NoWrap, gap = 6, hotid = -1;
  int hot = 0, changed = 0, hit = -1;
  void* c = (void*)strs;
  IRECT rc = {0};
  ISIZE sz = imuimeasureText("H", NULL, io->font, fmt, NULL);
  int height = sz.h + 6;
  if (1) {
    int i;
    sz.w = 0;
    for (i = 0; i < n; ++i) {
      if (width[i] <= 1) {
        ISIZE sz1 = imuimeasureText(strs[i], NULL, io->font, fmt, NULL);
        width[i] = sz1.w + 8;
      }
      sz.w += width[i];
    }
    if (!calc_rect(sz.w, sz.h + 6, LF_alighParentTop, &rc)) {
      return 0;
    }
  }
  if (ISHOVEREDWIN2()) {
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      changed = drag(width, rc, gap, n, width, x, y);
      hotid = listheader_hittest(rc, n, width, gap, x, y);
    }
    switch (io->lastMsg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        io->HitValue = hotid;
        sel = hotid;
      }
      break;
    case MSG_MOUSEMOVE:
      break;
    }
  }
  if (1) {
    COLOR clrFace = myGetSysColor(CLR_BTNFACE);
    COLOR clrText = Color_Black;
    COLOR clrGap = _RGBA(255, 0, 0, 50);
    IRECT rc1;
    int hotgapid = (hotid >> 16) - 1;
    int gap2 = gap / 2;
    w = rc.l;
    gcClipRectR(ctx, rc);
    gcSolidRect2(ctx, rc, clrFace);
    if (psel) {
      *psel = sel;
      for (i = 0; i < n; ++i) {
        rc1 = iRECT2(w, rc.t, width[i], RCH(&rc));
        //rc1.t+=(sel!=i)*2;
        gcEdgeRectSys(ctx, rc1, BDS_HIGH, 15, sel == i ? ColorMul(clrFace, 1.3) : clrFace);
        gcSolidText(ctx, rc1, strs[i], NULL, 0, fmt, clrText);
        w += width[i];
      }
    }
    else {
      for (i = 0; i < n; ++i) {
        rc1 = iRECT2(w, rc.t, width[i], RCH(&rc));
        gcEdgeRectSys(ctx, rc1, (ISHIT(c) && io->HitValue == i) ? BDS_SUNKEN : BDS_HIGH, 15, clrFace);
        gcSolidText(ctx, rc1, strs[i], NULL, 0, fmt, clrText);
        w += width[i];
      }
    }
    w = rc.l;
    for (i = 0; i < n; ++i) {
      w += width[i];
      if (hotgapid == i) {
        rc1 = iRECT2(w - gap2, rc.t, gap, height);
        gcSolidRect2(ctx, rc1, clrGap);
        break;
      }
    }
    gcClipReset(ctx);
  }
  return changed;
}
int listheader(int n, const char** strs, int* width)
{
  return header(n, strs, width, NULL);
}
int tab_ctrl(int n, const char** strs, int* width, int* psel)
{
  return header(n, strs, width, psel);
}
typedef enum {
  Drag_x = 1,
  Drag_y = 2,
} DragFlags;
int ImGuiDrag(void* c, IRECT rc1, IRECT grabrc, int x_min, int x_max, int y_min, int y_max, int flag, int wheel, IRECT* prc)
{
  GETGUIIO1();
  int x = io->MousePos.x, y = io->MousePos.y;
  static IPOINT pt;
  IRECT rc = grabrc;
  int changed = 0, hot = iPtInRect(&rc, x, y);
  int w = RCW(&rc);
  int h = RCH(&rc);
  if (ISHOVEREDWIN2()) {
    switch (io->lastMsg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        FORCE_REDRAW(c);
        //SETFOCUS(c);
        //if (SETHITV(iPtInRect2(&grabrc, io->MousePos))) {}
        pt.x = rc.l;
        pt.y = rc.t;
        io->lastMsg = MSG_NOP;
      }
      else {
        if (flag & Drag_x && grabrc.t <= y && y < grabrc.b) {
          IRECT rc_x = iRECT(x_min, rc.t, x_max + w, rc.b);
          if (iPtInRect(&rc_x, x, y)) {
            rc.l = io->MousePos.x - w / 2;
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
          rc.l = pt.x + (x - io->lastMouseDownPos.x);
          changed = 1;
        }
        if (flag & Drag_y) {
          rc.t = pt.y + (y - io->lastMouseDownPos.y);
          changed = 1;
        }
      }
      break;
    case MSG_MOUSEWHEEL:
      if (iPtInRect(&rc1, x, y)) {
        if (flag & Drag_y) {
          rc.t -= (int)(io->MouseWheel * wheel);
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
    //gcEdgeRectSys(ctx, rc, BDS_HIGH, 15, clrFace);
  }
  return changed;
}
int scrollbar_ctrl(IRECT rc, int client_h, int* client_off_y, IRECT* pclient)
{
  GETGUIIO2();
  int x = io->MousePos.x, y = io->MousePos.y;
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
      wheel = MAX(wheel, 2);
      hot = iPtInRect(&grabrc, x, y);
      changed = ImGuiDrag(c, rc, grabrc, 0, 0, rc2.t, rc2.b - grab_h, Drag_y, wheel, &grabrc);
    }
    if (changed) {
      *client_off_y = -(((grabrc.t - rc2.t) * ch2 + h2 - 1) / h2);
    }
    *client_off_y = BOUND(*client_off_y, h - ch, 0);
    {
      const ImGuiStyle* style = GetImGuiStyle();
      COLOR clrFace = myGetSysColor(CLR_BTNFACE);
      IRECT client = iRECT2(rc.l, rc.t + *client_off_y, RCW(&rc1), ch);
      *pclient = client;
      //gcEdgeRect1(ctx, rc, 0, Color_Black, 1);
      //gcEdgeRect1(ctx, client, 0, Color_Red, 1);
      gcEdgeRectSys(ctx, rc2, BDS_LOW, 15, clrFace);
      gcEdgeRectSys(ctx, grabrc, BDS_HIGH, 15, clrFace);
      if (hot) {
        gcEdgeRect1(ctx, grabrc, _rgba(1, 1, 1, 0.1), 0, 0);
      }
    }
  }
  return changed;
}
int strlist_ctrl(const vstr_t* sv, int ncol, int* width, int* client_off_y, int* pSelItem, int* pHotItem)
{
  GETGUIIO2();
  int x = io->MousePos.x, y = io->MousePos.y;
  int i, fmt = TF_VCENTER | TF_NoWrap, hot = 0, changed = 0, hit = -1;
  void* c = (void*)sv;
  int n = sv->n;
  IRECT rc = {0}, rcclient = {0};
  IRECT rc1;
  ISIZE sz = imuimeasureText("H", NULL, io->font, fmt, NULL);
  int height = sz.h + 6;
  int client_h = height * n;
  if (1) {
    if (!calc_rect(sz.w + 20, 100, 0, &rc)) {
      return 0;
    }
  }
  scrollbar_ctrl(rc, client_h, client_off_y, &rcclient);
  rcclient = iRectInter(rcclient, rc);
  if (ISHOVEREDWIN2()) {
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rcclient, x, y);
    *pHotItem = hot ? ((y - (rcclient.t + *client_off_y)) / height) : -1;
    switch (io->lastMsg) {
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
    gcEdgeRect1(ctx, rcclient, Color_White, 0, 0);
    //gcEdgeRectSys(ctx, rect, BDS_SUNKEN, 15, Color_White);
    gcClipRectR(ctx, rcclient);
    i = (-*client_off_y) / height;
    n2 = 1 + (rcclient.b - rcclient.t - *client_off_y) / height;
    n2 = BOUND(n2, 0, n);
    for (; i < n2; ++i) {
      COLOR clrText = Color_Black;
      rc1 = iRECT2(rcclient.l, rcclient.t + i * height + *client_off_y, RCW(&rcclient), height);
      if (i == *pSelItem) {
        gcSolidRect2(ctx, rc1, _RGBA(0, 0, 255, 188));
        clrText = Color_White;
      }
      else if (i == *pHotItem) {
        gcSolidRect2(ctx, rc1, _RGBA(0, 0, 255, 90));
        clrText = Color_White;
      }
      if (0) {
        gcSolidText(ctx, rc1, sv->v[i].s, NULL, 0, fmt, clrText);
      }
      else {
        strv_split_str(sv2, sv->v[i], "|", " \n\t", 0);
        w = 0;
        for (j = 0; j < MIN(sv2->n, ncol); ++j) {
          ISIZE sz = imuimeasureText(sv2->v[j].s, NULL, io->font, fmt, NULL);
          sz.w += 20;
          width[j] = MAX(width[j], sz.w);
          rc2 = iRECT2(rc1.l + w, rc1.t, width[j], RCH(&rc1));
          gcSolidText(ctx, rc2, sv2->v[j].s, NULL, 0, fmt | TF_CENTER, clrText);
          w += width[j];
        }
      }
      rc1.t = rc1.b - 1;
      gcSolidRect2(ctx, rc1, clrGrid);
    }
    w = 0;
    for (j = 0; j < ncol; ++j) {
      w += width[j];
      rc2 = iRECT2(rcclient.l + w, rcclient.t, 1, RCH(&rcclient));
      gcSolidRect2(ctx, rc2, clrGrid);
    }
    gcClipReset(ctx);
    strv_free(sv2);
  }
  return changed;
}
int listbody_begin(int n, IRECT* pclient)
{
  GETGUIIO2();
  int x = io->MousePos.x, y = io->MousePos.y;
  int fmt = TF_CENTER | TF_VCENTER | TF_NoWrap, hot = 0, changed = 0;
  void* c = (void*)0;
  IRECT rc = {0};
  ISIZE sz = imuimeasureText("H", NULL, io->font, fmt, NULL);
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
    ISIZE sz = imuimeasureText("H", NULL, io->font, fmt, NULL);
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
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rect, x, y);
    if (hot) {
      hotid = iPtInRects(countof(rcs), rcs, x, y);
    }
    switch (io->lastMsg) {
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
    gcEdgeRect1(ctx, rect, Color_White, Color_Black, 1);
    gcEdgeRect1(ctx, *rcTitle, _RGB(0, 0, 255), 0, 0);
    _snprintf(buf, 80, "%d/%d", year, month);
    gcSolidText(ctx, *rcTitle, buf, NULL, io->font, fmt, Color_White);
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
        gcEdgeRect1(ctx, rcWeak[i], _RGBA(0, 0, 255, 12), 0, 0);
        gcSolidText(ctx, rcWeak[i], buf, NULL, io->font, fmt, Color_Black);
        tm2.tm_yday++;
        tm2.tm_mday++;
        tm2.tm_wday++;
      }
    }
    for (i = 0; i < 6 * 7; ++i) {
      IRECT rc1 = rcDay[i];
      COLOR crText = Color_Black;
      date = days_to_date(days + i);
      _snprintf(buf, 80, "%d", date % 100);
      if (((date / 100) % 100) != (uint)month) {
        crText = _RGBA(50, 50, 50, 90);
      }
      else if (i == curpos) {
        gcEdgeRect1(ctx, rc1, _RGBA(0, 0, 255, 125), 0, 0);
        crText = Color_White;
      }
      if (hot && (hotid) == (i + offDay)) {
        gcEdgeRect1(ctx, rc1, 0, Color_Black, 1);
      }
      gcSolidText(ctx, rc1, buf, NULL, io->font, fmt, crText);
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
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rc, x, y);
    if (hot) {
      hotid = iRectMatrix2_HitText(rcIn, cx, cy, gap, gap, nrc, x, y);
    }
    switch (io->lastMsg) {
    case MSG_LBUTTONUP:
      if (hit) {
        if (io->HitValue == hotid) {
          hitid = hotid;
          clicked = 1;
        }
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        io->HitValue = hotid;
      }
      break;
    }
  }
  if (1) {
    COLOR clrBk;
    clrBk = _GetSysColor(CLR_BTNFACE);
    IRECT rc1, rc2 = gcEdgeRectSys(ctx, rc, BDS_UP, BDM_ALL, clrBk);
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
        gcEdgeRectSys(ctx, rc1, hit ? BDS_LOW : BDS_UP, BDM_ALL, clrBk);
        iRectOff2(&rc2, !!hit, !!hit);
      }
      gcRectImageR(ctx, rc2, tex, rcs_tex + i);
      //button_ctrl
    }
  }
  return hitid;
}
#if 0
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
  IRECT rc = {0};
  int hot = 0;
  static int hitid = -1;
  static IPOINT downPt;
  void* c = points;
  if (!calc_rect(100, 100, LF_alighParentTop, &rc)) {
    return 0;
  }
  if (ISHOVEREDWIN2()) {
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rc, x, y);
    switch (io->lastMsg) {
    case MSG_LBUTTONDOWN:
      if (hot) {
        hitid = curve_hittest(points, npt, x, y);
        if (hitid >= 0 && hitid < npt) {
          downPt = points[hitid];
        }
        SETHIT(c);
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
            points[hitid] = iPOINT(downPt.x + x - io->lastMouseDownPos.x, downPt.y + y - io->lastMouseDownPos.y);
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
      //imdrawaa_spline(ctx, &rcclip, pt, npt, 0, 0, Color_Black, 1);
    }
    //if (mouseDown) {
    //imdrawaa_line(ctx, 0, p.Y, clientRect.Width, p.Y, Gray);
    //g.DrawLine(Pens.Gray, p.X, 0, p.X, clientRect.Height);
    //}
    for (i = 0; i < npt; ++i) {
      IPOINT p = points[i];
      int d = 3;
      COLOR clr = i == hot ? Color_Black;
      IRECT rc = iRECT(p.x - d, p.y - d, p.x + d, p.y + d);
      if (i == hot) {
        //imdrawaa_rect(ctx, p.x-d, p.y-d, p.x+d, p.y+d, Color_Black, 0, 0);
        //imdraw_fillrect(ctx, &rcclip, rc, Color_Black);
      }
      else {
        //imdraw_border_n(ctx, &rcclip, rc, 0, 1, &clr, &clr, &clr, &clr);
        //imdrawaa_rect(ctx, p.x-d, p.y-d, p.x+d, p.y+d, 0, Color_Black, 1);
      }
      gcSolidCircle();
    }
    FREE(pt);
  }
  return 0;
}
#endif
#include "bbb.inl"


#endif // _IMUI_CTRL_INL_
