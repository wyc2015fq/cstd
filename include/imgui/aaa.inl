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
void imuiRenderFrame(gc_t* g, IRECT rc, ImColor fill_col, bool border, float rounding, int flag)
{
  ImGuiStyle* style = GetImGuiStyle();
  ImColor clr = imuiGetColorU32(Col_BorderShadow);
  gcEdgeRect1(g, rc, fill_col, clr, 1);
}
int ImFontID_CalcFontSize(texfont_t* font)
{
  int size = 0;
  size = font->FontSize;
  return size;
}
int ImWindow_TitleBarHeight(int flags)
{
  GETGUIIO1();
  ImGuiStyle* style = GetImGuiStyle();
  return (flags & ImGuiWindowFlags_NoTitleBar) ? 0.0f : ImFontID_CalcFontSize(io->font) + style->FramePadding.y * 2.0f;
}
#define SET_ALPHA(color, alpha) ((color) & 0x00ffffff) | ((uchar)(alpha)<<24)
#define set_alpha(color, alpha) SET_ALPHA(color, (alpha*255))

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
int linearlayout_begin(const char* label, BOOL horz, bool can_change, int count, float* v)
{
  GETGUIIO2();
  void* c = v;
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
      ISIZE sz = measureText(label, NULL, io->font, 0, NULL);
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
      groupbox_draw(g, rc, label, NULL, bds, clr, io->font, TF_CENTER);
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
      switch (io->msg) {
      case MSG_LBUTTONDOWN:
        if (iPtInRect2(&rc, io->MousePos) && (hit = iPtInRectsBorder(rc, horz, count, v, gap, x, y, 2)) >= 0) {
          SETHIT(c);
          SETHITV(hit);
          FORCE_REDRAW(c);
          SETFOCUS(c);
          //v = (io->MousePos.x - (gap/2) - prc[i].l)/ss;
          v0 = v[io->hitvalue + 0];
          v1 = v[io->hitvalue + 1];
          changed = 1;
          io->msg = MSG_NOP;
        }
        break;
      case MSG_MOUSEMOVE:
        i = io->hitvalue;
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
      int clridx = ((ISHIT(c) && (io->hitvalue == i)) ? Col_LayoutSplitActive : (hot && (HotValue == i)) ? Col_LayoutSplitHovered : Col_LayoutSplit);
      gcEdgeRect1(g, rcb, imuiGetColorU32(clridx), imuiGetColorU32(Col_Border), 1);
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

