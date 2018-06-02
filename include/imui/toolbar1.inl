enum {
  TB_BDR_NONE = 0,
  TB_BDR_RAISED,
  TB_BDR_STRONG_RAISED,
  TB_BDR_SUNKEN,
  TB_BDR_STRONG_SUNKEN,
  TB_BDR_COLORED,
  TB_BDR_STRONG_COLORED,
};
enum {
  TB_BTN_NONE = 0,
  TB_BTN_RAISED,
  TB_BTN_COLORED,
  TB_BTN_LARGE_NONE,
  TB_BTN_LARGE_RAISED,
  TB_BTN_LARGE_COLORED,
  TB_BTN_STRONG_RAISED,
  TB_BTN_STRONG_COLORED,
  TB_BTN_SUNKEN_RAISED,
  TB_BTN_RAISED_SUNKEN,
  TB_BTN_LARGE_STRONG_RAISED,
  TB_BTN_LARGE_STRONG_COLORED,
  TB_BTN_LARGE_SUNKEN_RAISED,
  TB_BTN_LARGE_RAISED_SUNKEN,
};
char* P_GetStyleString(int iStyle)
{
  char* cs = "";
  switch (iStyle) {
  case TB_BTN_NONE:
    cs = "NONE";
    break;
  case TB_BTN_RAISED:
    cs = "RAISED";
    break;
  case TB_BTN_COLORED:
    cs = "COLORED";
    break;
  case TB_BTN_LARGE_NONE:
    cs = "LARGE_NONE";
    break;
  case TB_BTN_LARGE_RAISED:
    cs = "LARGE_RAISED";
    break;
  case TB_BTN_LARGE_COLORED:
    cs = "LARGE_COLORED";
    break;
  case TB_BTN_STRONG_RAISED:
    cs = "STRONG_RAISED";
    break;
  case TB_BTN_STRONG_COLORED:
    cs = "STRONG_COLORED";
    break;
  case TB_BTN_SUNKEN_RAISED:
    cs = "SUNKEN_RAISED";
    break;
  case TB_BTN_RAISED_SUNKEN:
    cs = "RAISED_SUNKEN";
    break;
  case TB_BTN_LARGE_STRONG_RAISED:
    cs = "LARGE_STRONG_RAISED";
    break;
  case TB_BTN_LARGE_STRONG_COLORED:
    cs = "LARGE_STRONG_COLORED";
    break;
  case TB_BTN_LARGE_SUNKEN_RAISED:
    cs = "LARGE_SUNKEN_RAISED";
    break;
  case TB_BTN_LARGE_RAISED_SUNKEN:
    cs = "LARGE_RAISED_SUNKEN";
    break;
  default:
    cs = "I don't know";
    break;
  }
  return cs;
}
int border_w(int button_style)
{
  int iBorder = 0;
  switch (button_style) {
  case TB_BTN_NONE:
  case TB_BTN_RAISED:
  case TB_BTN_COLORED:
    iBorder = 1;
    break;
  case TB_BTN_LARGE_NONE:
  case TB_BTN_LARGE_RAISED:
  case TB_BTN_LARGE_COLORED:
  case TB_BTN_STRONG_RAISED:
  case TB_BTN_STRONG_COLORED:
  case TB_BTN_SUNKEN_RAISED:
  case TB_BTN_RAISED_SUNKEN:
    iBorder = 2;
    break;
  case TB_BTN_LARGE_STRONG_RAISED:
  case TB_BTN_LARGE_STRONG_COLORED:
  case TB_BTN_LARGE_SUNKEN_RAISED:
  case TB_BTN_LARGE_RAISED_SUNKEN:
    iBorder = 3;
    break;
  default:
    //error
    return 0;
  }
  return iBorder;
}
int P_DrawBarBorder(img_t* im, IRECT rc, int border_style)
{
  COLOR clr255 = _RGB(255, 255, 255), clr000 = _RGB(0, 0, 0);
  COLOR crBorder = _RGB(0, 128, 0);
  switch (border_style) {
  case TB_BDR_RAISED:
    imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    break;
  case TB_BDR_STRONG_RAISED:
    imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    iRectDeflate2(&rc, 1, 1);
    imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    break;
  case TB_BDR_SUNKEN:
    imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    break;
  case TB_BDR_STRONG_SUNKEN:
    imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    iRectDeflate2(&rc, 1, 1);
    imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    break;
  case TB_BDR_COLORED:
    imdraw_Draw3dRect(im, 0, &rc, crBorder, crBorder, 0);
    break;
  case TB_BDR_STRONG_COLORED:
    imdraw_Draw3dRect(im, 0, &rc, crBorder, crBorder, 0);
    iRectDeflate2(&rc, 1, 1);
    imdraw_Draw3dRect(im, 0, &rc, crBorder, crBorder, 0);
    break;
  }
  return 0;
}
int P_DrawButtonBorder(img_t* im, IRECT rc, int button_style, BOOL bDown, BOOL bHot)
{
  COLOR clr255 = _RGB(255, 255, 255), clr000 = _RGB(0, 0, 0);
  COLOR crButton = _RGB(0, 128, 0);
  switch (button_style) {
    //button=1
  case TB_BTN_NONE:
  case TB_BTN_LARGE_NONE:
    if (bDown) {
      imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    }
    else if (bHot) {
      imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    }
    break;
  case TB_BTN_RAISED:
  case TB_BTN_LARGE_RAISED:
    if (bDown) {
      imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    }
    else if (bHot) {
      imdraw_Draw3dRect(im, 0, &rc, crButton, crButton, 0);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    }
    break;
  case TB_BTN_COLORED:
  case TB_BTN_LARGE_COLORED:
  case TB_BTN_STRONG_COLORED:
  case TB_BTN_LARGE_STRONG_COLORED:
    if (bDown) {
      imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
      iRectDeflate2(&rc, 1, 1);
    }
    else if (bHot) {
      imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
      iRectDeflate2(&rc, 1, 1);
    }
    imdraw_Draw3dRect(im, 0, &rc, crButton, crButton, 0);
    switch (button_style) {
    case TB_BTN_STRONG_COLORED:
    case TB_BTN_LARGE_STRONG_COLORED:
      iRectDeflate2(&rc, 1, 1);
      imdraw_Draw3dRect(im, 0, &rc, crButton, crButton, 0);
      break;
    }
    break;
  case TB_BTN_STRONG_RAISED:
  case TB_BTN_LARGE_STRONG_RAISED:
    if (bDown) {
      imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    }
    else if (bHot) {
      imdraw_Draw3dRect(im, 0, &rc, crButton, crButton, 0);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    }
    iRectDeflate2(&rc, 1, 1);
    if (bDown) {
      imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    }
    break;
  case TB_BTN_SUNKEN_RAISED:
  case TB_BTN_LARGE_SUNKEN_RAISED:
    imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    if (button_style == TB_BTN_SUNKEN_RAISED) {
      iRectDeflate2(&rc, 1, 1);
    }
    else {
      iRectDeflate2(&rc, 2, 2);
    }
    if (bDown) {
      imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    }
    else if (bHot) {
      imdraw_Draw3dRect(im, 0, &rc, crButton, crButton, 0);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    }
    break;
  case TB_BTN_RAISED_SUNKEN:
  case TB_BTN_LARGE_RAISED_SUNKEN:
    if (bDown) {
      imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    }
    else if (bHot) {
      imdraw_Draw3dRect(im, 0, &rc, crButton, crButton, 0);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rc, clr255, clr000, 0);
    }
    if (button_style == TB_BTN_RAISED_SUNKEN) {
      iRectDeflate2(&rc, 1, 1);
    }
    else {
      iRectDeflate2(&rc, 2, 2);
    }
    imdraw_Draw3dRect(im, 0, &rc, clr000, clr255, 0);
    break;
  }
  return 0;
}
typedef struct tbbutton_t {
  int nImage;
  int nStyle;
  int nState;
  char* text;
  IRECT rc;
} tbbutton_t;
//CToolBarCtrl
//TBBUTTON
typedef struct toolbar_t {
  ctrl_t c[1];
  //img_t img[1];
  img_t* img;
  tbbutton_t* tbb;
  int ntbb;
  int border_style;
  int gap;
  int margin;
  int button_style;
  int hit;
  int hot;
} toolbar_t;
#if 0
BOOL toolbar_addbuttons(toolbar_t* s, int nNumButtons, const tbbutton_t* lpButtons)
{
  int i, n = s->ntbb;
  s->tbb = (tbbutton_t*)my_memins(s->tbb, s->ntbb, s->ntbb, nNumButtons, sizeof(tbbutton_t));
  for (i = 0; i < nNumButtons; ++i) {
    str_t str[1] = {0};
    str_setstr1(str, lpButtons[i].text[0]);
    s->tbb[i + n] = lpButtons[i];
    s->tbb[i + n].text[0] = str[0];
  }
  s->ntbb += nNumButtons;
  return TRUE;
}
#endif
ISIZE toolbar_get_button_size(const toolbar_t* s)
{
  int d;
  ISIZE si = {0, 0};
  if (s->ntbb > 0) {
    si = imsize(s->img);
    d = (s->margin + border_w(s->button_style)) * 2;
    si.w += d;
    si.h += d;
  }
  return si;
}
int toolbar_hitrect(const toolbar_t* s)
{
  const ctrl_t* c = s->c;
  ISIZE sz = toolbar_get_button_size(s);
  IRECT out[256];
  int i;
  iRectMatrix2(c->rc, sz.w, sz.h, s->gap, s->gap, s->ntbb, out);
  for (i = 0; i < s->ntbb; ++i) {
    s->tbb[i].rc = out[i];
  }
  return 0;
}
int draw_toolbar_ctrl(const toolbar_t* s, img_t* im)
{
  int i;
  COLOR clrBk;
  const ctrl_t* c = s->c;
  IRECT rc = c->rc;
  IRECT rcclip = c->rcclip;
  iRectOff0(&rcclip);
  iRectOff0(&rc);
  clrBk = _GetSysColor(COLOR_BTNFACE);
  if (s->ntbb == 0) {
    return 0;
  }
  imdraw_rect(im, &rcclip, rc, clrBk, 0, 0, 0);
  P_DrawBarBorder(im, rc, s->border_style);
  toolbar_hitrect(s);
  for (i = 0; i < s->ntbb; ++i) {
    IRECT rci = s->tbb[i].rc;
    int d = (s->margin + border_w(s->button_style));
    P_DrawButtonBorder(im, rci, s->button_style, i == s->hit, i == s->hot);
    RCOFFSET(&rci, d, d, -d, -d);
    imdraw_imagelist(im, &rcclip, rci, s->img, s->tbb[i].nImage, 0, 0, 0);
  }
  return 0;
}
int toolbar_hittest(toolbar_t* s, int x, int y)
{
  int i;
  for (i = 0; i < s->ntbb; i++) {
    if (iPtInRect(&s->tbb[i].rc, x, y)) {
      return i;
    }
  }
  return -1;
}
static LRESULT toolbar_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  toolbar_t* s = (toolbar_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  switch (uMsg) {
  case WM_CTRLINIT:
    if (s) {
      s->tbb = 0;
      s->ntbb = 0;
    }
    return 0;
  case WM_CTRLFREE:
    if (s) {
      s->tbb = 0;
      s->ntbb = 0;
    }
    return 0;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
  }
  break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      int hit = toolbar_hittest(s, x, y);
      if (hit != s->hot) {
        s->hot = hit;
        force_redraw(c);
      }
      return 1;
    }
    break;
  case WM_LBUTTONUP:
    if (iPtInRect(&rc, x, y)) {
      int hit = toolbar_hittest(s, x, y);
      if (hit >= 0 && hit == s->hit) {
        sendmsg(c, c->listener, WM_COMMAND, 0, (WPARAM)c);
      }
      s->hit = -1;
      force_redraw(c);
      return 1;
    }
    s->hit = -1;
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      int hit = toolbar_hittest(s, x, y);
      if (hit != s->hit) {
        s->hit = hit;
        force_redraw(c);
      }
      return 1;
    }
    break;
  }
  return 0;
}
int toolbar_set(toolbar_t* s)
{
  //ctrl_set(button_proc, s, s->c, li, text, sys);
  //UISETCALL(s, draw_toolbar_ctrl, toolbar_event);
  s->gap = 1;
  s->margin = 2;
  s->button_style = TB_BTN_RAISED;
  s->border_style = TB_BDR_NONE;
  s->hit = -1;
  s->hot = -1;
  return 0;
}

