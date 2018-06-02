#include "skinctrl.inl"
#include "codec/Encry.inl"
static int FM_PB_DelDirectory(const char* foldName)
{
  SHFILEOPSTRUCT fileop = {0};
  fileop.hwnd = NULL;
  fileop.wFunc = FO_DELETE;
  fileop.pFrom = foldName;
  fileop.pTo = NULL;
  fileop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
  SHFileOperation(&fileop);
  return 0;
}
typedef struct test_ChzzMain_t {
  ctrl_t c[1];
  slider_t m_alpha0[1];
  slider_t m_alpha1[1];
  cut_layout_t lay0[1];
  SkinButton_t btn0[1];
  SkinButton_t btn_close[1];
  //frame_t frame[1];
  cut_layout_item_t cli[10];
} test_ChzzMain_t;
static ctrl_t* ctrl_lay(ctrl_t* c, ctrl_t* next, ctrl_t* sub)
{
  c->next = next;
  c->sub = sub;
  if (next) {
    next->prev = c;
  }
  return 0;
}
static LRESULT test_ChzzMain_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_ChzzMain_t* s = (test_ChzzMain_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    strcpy(c->sys->imagepaths, "E:/bak/pub/cstd/ZSJ/D/通用/ProfessionP2005/CHZZCOL_STAND/EroadThemes/SkyBlue/chzzCollect");
    {
      slider_set(s->m_alpha0, c, "m_alpha0=%1.0f", 0, 255, 10);
      slider_set(s->m_alpha1, c, "m_alpha1=%1.0f", 0, 255, 10);
      ctrl_set(SkinButton_proc, s->btn0, s->btn0->c, c, WT_CHILD, "确定");
      s->btn0->m_sImage = "YanZhengOK";
      s->btn0->m_buttonCount = 4;
      s->btn0->m_nStyle = STYLE_BITMAP;
      ctrl_set(SkinButton_proc, s->btn_close, s->btn_close->c, c, WT_CHILD, NULL);
      s->btn_close->m_sImage = "Close";
      s->btn_close->m_buttonCount = 4;
      s->btn_close->m_nStyle = STYLE_BITMAP;
    }
    {
      cut_layout_item_t cli[] = {
        0, 0, 0, 0,
        1, 't', 30, 0,
        1, 'l', 30, 0,
        1, 'r', 30, 0,
        1, 't', 30, s->m_alpha0->c,
        1, 't', 30, 0,
        1, 't', 30, s->m_alpha1->c,
        1, 't', 30, 0,
        //1, 't', 30, s->btn0->c,
      };
      //"t30l30r30t30%t30t30t30%"
      memcpy(s->cli, cli, sizeof(cli));
      ctrl_set(cut_layout_proc, s->lay0, s->lay0->c, c, WT_CHILD, NULL);
      s->lay0->li = s->cli;
      s->lay0->len = countof(cli);
      s->lay0->gap = 4;
      //ctrl_set(frame_proc, s->frame, s->frame->c, c, c);
      //ctrl_lay(s->lay0->c, 0, ctrl_lay(s->m_alpha0->c, ctrl_lay(s->m_alpha1->c, s->btn0->c, 0), 0));
    }
    ctrl_addsub_head(c, s->lay0->c);
    ctrl_addsub_head(c, s->btn_close->c);
    return 0;
  }
  switch (uMsg) {
  case WM_NCHITTEST: {
    //return xNCHitTest(c, 40, 4, x, y);
  }
  break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x += 120;
    mminfo->ptMinTrackSize.y += 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (1) {
      ctrl_setrect(c->sub, c->rc);
      ctrl_setrect(s->btn_close->c, iRECT2(rc.r - 35, 1, 108 / 4, 21));
    }
    //SetWindowRound(hwnd, 3);
    break;
  case WM_COMMAND:
    if (s) {
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      show_tooltip(s->m_alpha0->c, x, y, "m_alpha0") ||
      show_tooltip(s->m_alpha1->c, x, y, "m_alpha1");
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT rcCorner = {40, 40, 40, 40};
      img_t* im1 = sys_find_img(c->sys, "BG");
      //imdraw_fillrect(im, &rc, rc, _RGB(0,0,0));
      if (im1) {
        imdraw_image_corner(im, &rc, rc, im1, 0, &rcCorner, 0);
      }
      s->btn_close->c->redraw = c->redraw;
    }
    break;
  }
  return 0;
}

