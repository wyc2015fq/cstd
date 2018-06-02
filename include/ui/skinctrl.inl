//#include "uirender.inl"
// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP 0x00000001
#define UIFLAG_SETCURSOR 0x00000002
#define UIFLAG_WANTRETURN 0x00000004
// Flags for FindControl()
#define UIFIND_ALL 0x00000000
#define UIFIND_VISIBLE 0x00000001
#define UIFIND_ENABLED 0x00000002
#define UIFIND_HITTEST 0x00000004
#define UIFIND_TOP_FIRST 0x00000008
#define UIFIND_ME_FIRST 0x80000000
// Flags for Draw Style
#define UIDRAWSTYLE_INPLACE 0x00000001
#define UIDRAWSTYLE_FOCUS 0x00000002
// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP 0x00000001
#define UISTRETCH_NEWLINE 0x00000002
#define UISTRETCH_MOVE_X 0x00000004
#define UISTRETCH_MOVE_Y 0x00000008
#define UISTRETCH_SIZE_X 0x00000010
#define UISTRETCH_SIZE_Y 0x00000020
// Flags used for controlling the paint
#define UISTATE_FOCUSED 0x00000001
#define UISTATE_SELECTED 0x00000002
#define UISTATE_DISABLED 0x00000004
#define UISTATE_HOT 0x00000008
#define UISTATE_PUSHED 0x00000010
#define UISTATE_READONLY 0x00000020
#define UISTATE_CAPTURED 0x00000040
#define STYLE_TOOLBAR 0x0001
#define STYLE_BITMAP 0x0002
typedef struct SkinButton_t {
  ctrl_t c[1];
#if 0
  int m_uButtonState;
  char* text;
  char* m_sNormalImage;
  char* m_sHotImage;
  char* m_sPushedImage;
  char* m_sFocusedImage;
  char* m_sDisabledImage;
  char* m_sForeImage;
#endif
  const char* m_sImage;
  IRECT corner;
  BOOL isFirst;
  int m_buttonCount;
  DWORD m_dwToolTipStyle;
  BOOL m_bMouseIn;
  BOOL m_bDown;
  BOOL m_bFocus;
  BOOL m_bEnable;
  int m_nStyle;
} SkinButton_t;
static IRECT iRectMatrixAt(IRECT rc, int x, int y, int cols, int rows)
{
  int ww = RCW(&rc), hh = RCH(&rc);
  //单个图片的长和宽
  int w = ww / cols, h = hh / rows;
  return iRECT(rc.l + x * w, rc.t + y * h, rc.l + (x + 1) * w, rc.t + (y + 1) * h);
}
static int imdraw_image_grid(img_t* im, const IRECT* pclip, IRECT rc, const img_t* im1, int x, int y, int cols, int rows, const IRECT* rcCorner, COLOR clrMask)
{
  if (im1 && cols > 0 && rows > 0 && x < cols && y < rows && x >= 0 && y >= 0) {
    IRECT rc0 = iRECT(0, 0, im1->w, im1->h);
    IRECT rc1 = iRectMatrixAt(rc0, x, y, cols, rows);
    return imdraw_image_corner(im, pclip, rc, im1, &rc1, rcCorner, clrMask);
  }
  return 0;
}
static int sys_draw_image_grid_string(ctrl_t* c, img_t* im, const char* imagename, int x, int y, int cols, int rows, const IRECT* rcCorner)
{
  img_t* im1 = sys_find_img(c->sys, imagename);
  if (im1) {
    return imdraw_image_grid(im, &c->rc, c->rc, im1, x, y, cols, rows, rcCorner, 0);
  }
  return 0;
}
static LRESULT SkinButton_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  SkinButton_t* s = (SkinButton_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    //s->m_uButtonState = 0;
    int b = 4;
    s->m_bEnable = 1;
    s->corner = iRECT(b, b, b, b);
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x += 120;
    mminfo->ptMinTrackSize.y += 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (iPtInRect(&rc, x, y)) {
      // TODO: Add your message handler code here and/or call default
      s->m_bDown = FALSE;
      if (s->m_bMouseIn) {
        s->m_bMouseIn = FALSE;
        //GetRegion();
        force_redraw(c);
        sendmsg(c, c->listener, WM_COMMAND, 0, (WPARAM)c);
      }
      else {
        //GetRegion();
        force_redraw(c);
      }
    }
    else {
      //s->m_uButtonState &= ~UISTATE_FOCUSED;
    }
    break;
  case WM_COMMAND:
    if (s) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      //s->m_uButtonState |= UISTATE_FOCUSED;
      s->m_bDown = TRUE;
      s->m_bMouseIn = TRUE;
      c->sys->focus = c;
      force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      //SetCursor(LoadCursor(GetModuleHandle(0), IDC_HAND));
      //移入
      if (!s->m_bMouseIn) {
        s->m_bMouseIn = TRUE;
        force_redraw(c);
      }
    }
    else {
      //移出
      if (s->m_bMouseIn) {
        s->m_bMouseIn = FALSE;
        //GetRegion();
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      {
#if 0
        char* imginfo = NULL;
        imginfo_t im1[1] = {0};
        if (c->type & WT_DISABLED) {
          s->m_uButtonState |= UISTATE_DISABLED;
        }
        else {
          s->m_uButtonState &= ~ UISTATE_DISABLED;
        }
        if ((s->m_uButtonState & UISTATE_DISABLED) != 0) {
          imginfo = s->m_sDisabledImage;
        }
        else if ((s->m_uButtonState & UISTATE_PUSHED) != 0) {
          imginfo = s->m_sPushedImage;
        }
        else if ((s->m_uButtonState & UISTATE_HOT) != 0) {
          imginfo = s->m_sHotImage;
        }
        else if ((s->m_uButtonState & UISTATE_FOCUSED) != 0) {
          imginfo = s->m_sFocusedImage;
        }
        else {
          imginfo = s->m_sNormalImage;
        }
        if (imageinfo_load(c->sys, imginfo, im1)) {
          imdraw_image_corner(im, &rc, rc, im1->imd->im, &im1->source, &im1->corner);
        }
#else
        if (s->isFirst) {
          s->m_bFocus = TRUE;
        }
        else {
          s->m_bFocus = c->sys->focus == c;
        }
        //s->m_bEnable = 1;
        if (s->m_nStyle & STYLE_BITMAP) {
          if (!s->m_bEnable) {
            sys_draw_image_grid_string(c, im, s->m_sImage , 3 , 0 , s->m_buttonCount , 1, &s->corner);
          }
          else if (s->m_bDown) {
            sys_draw_image_grid_string(c, im, s->m_sImage , 2 , 0 , s->m_buttonCount , 1, &s->corner);
          }
          else if (s->m_bMouseIn && !s->m_bFocus) {
            //hover
            sys_draw_image_grid_string(c, im, s->m_sImage , 1 , 0 , s->m_buttonCount , 1, &s->corner);
          }
          else if (s->m_bFocus || (s->m_bFocus && s->m_bMouseIn)) {
            if (s->m_buttonCount == 5) {
              sys_draw_image_grid_string(c, im, s->m_sImage , 4 , 0 , s->m_buttonCount , 1, &s->corner);
            }
            else {
              sys_draw_image_grid_string(c, im, s->m_sImage , 0 , 0 , s->m_buttonCount , 1, &s->corner);
            }
          }
          else {
            //normal
            sys_draw_image_grid_string(c, im, s->m_sImage , 0 , 0 , s->m_buttonCount , 1, &s->corner);
          }
          imdraw_text(im, &rc, rc, c->text, -1, c->fo, _RGB(0, 0, 0), 0, DT_VCENTER | DT_CENTER);
        }
        else if (s->m_nStyle & STYLE_TOOLBAR) {
#if 0
          IRECT rcImage, rcText;
          rcImage = s->m_bmpButton.GetRect();
          int nMaigin = (rcButton.Height() - rcImage.Height()) / 2;
          rcImage.OffsetRect(nMaigin , nMaigin);
          rcText = rcButton;
          rcText.left = rcImage.right ;
          //先更新背景
          IRECT rc = rcButton;
          rc.DeflateRect(1, 1);
          if (!s->m_bEnable) {
            //disable
          }
          else {
            if (s->m_bMouseIn && s->m_bDown) {
              //down
              imdraw_3drect(im, &rc, rc, 0, _GetSysColor(CLR_3DDKSHADOW), _GetSysColor(CLR_3DHILIGHT), 1, BDM_ALL);
            }
            else if (s->m_bFocus || (s->m_bMouseIn && s->m_bFocus)) {
              imdraw_3drect(im, &rc, rc, 0, _GetSysColor(CLR_3DDKSHADOW), _GetSysColor(CLR_3DHILIGHT), 1, BDM_ALL);
              //dc.Draw3dRect(&rc,GetSysColor(COLOR_3DHILIGHT),GetSysColor(COLOR_3DDKSHADOW));
            }
            else if (s->m_bMouseIn) {
              imdraw_3drect(im, &rc, rc, 0, _GetSysColor(CLR_3DHILIGHT), _GetSysColor(CLR_3DDKSHADOW), 1, BDM_ALL);
            }
            else {
            }
          }
          s->m_bmpButton.TransparentBlt(dc, &rcImage, RGB(255, 0, 255));
          DrawText(&dc , rcText, s->m_crText);
#endif
        }
#endif
      }
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
static LRESULT SetWindowRound(HWND hwnd, int r)
{
  SIZE szRoundCorner = {r, r};
  if (!IsIconic(hwnd) && (szRoundCorner.cx > 0 && szRoundCorner.cy > 0)) {
    RECT rcClient;
    int ret = GetClientRect(hwnd, &rcClient);
    HRGN hRgn1 = CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right + 1, rcClient.bottom + 1, szRoundCorner.cx, szRoundCorner.cy);
    SetWindowRgn(hwnd, hRgn1, TRUE);
    DeleteObject(hRgn1);
  }
  return 0;
}

