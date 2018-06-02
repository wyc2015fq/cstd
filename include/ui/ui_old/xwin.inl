
#include "cstd.h"
#include "img.h"
#include "xwin.h"
#include <windowsx.h>

//#include "ui/window_w32.inl"
//#include "img/dib.inl"
#include "img/imgopt.inl"
#include "xctrl.inl"
#include "tooltip.inl"

#define XWINCLASS  ("XWIN")
xdisp_t g_disp[1] = {0};

#define TraceEvt(e) TraceMsg(0, e->msg, e->wp, e->lp)

int xReDraw(xwin_t* win)
{
  if (win) {
    InvalidateRect(win->hwnd, 0, 0);
  }

  return 0;
}


// static xwin_t* iXFindWindowById(xdisp_t* disp, int winid)
// {
//   int i;
//
//   for (i = 0; i < disp->nwin; ++i) {
//     if (disp->win[i].winid == winid) {
//       return disp->win + i;
//     }
//   }
//
//   return 0;
// }
// int xReDrawId(xdisp_t* disp, int winid)
// {
//   xwin_t* win = iXFindWindowById(disp, winid);
//   return xReDraw(win);
// }

int XRemoveWindow(xwin_t* win)
{
  int i;

  //if (win->hDC) {
  //  DeleteDC(win->hDC), win->hDC = 0;
  //}

  //if (win->hBmp) {
  //  DeleteObject(win->hBmp), win->hBmp = 0;
  //}

  imfree(win->im);
  //ReleaseDC(win->hwnd, hdc);

  for (i = 0; i < g_disp->nwin; ++i) {
    if (g_disp->win[i] == win) {
      memmove(g_disp->win + i, g_disp->win + i + 1, g_disp->nwin - i - 1);
      g_disp->nwin--;
      break;
    }
  }

  return 0;
}
int xSetSize(xwin_t* win, int h, int w)
{
  imsetsize(win->im, h, w, 3, 1);
  //if (win->hBmp) {
  //  DeleteObject(win->hBmp), win->hBmp = 0;
  //  IMINIT(win->imbmp, 0, 0, 0, 0, 0, 0);
  //}

  if (h > 0 && w > 0) {
    //BITMAP bmp;
    //HBITMAP hOldBmp = 0;
    //uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
    //BITMAPINFO* binfo = (BITMAPINFO*) buffer;
    //int channels = 3;
    //uchar* ptr = 0;
    //xFillBitmapInfo(binfo, w, h, channels * 8, 1, 0);
    //win->hBmp = CreateDIBSection(win->hDC, binfo, DIB_RGB_COLORS, (VOID**)&ptr, 0, 0);
    //GetObject(win->hBmp, sizeof(bmp), &bmp);
    //IMINIT(win->imbmp, bmp.bmHeight, bmp.bmWidth, bmp.bmBits, bmp.bmWidthBytes, bmp.bmBitsPixel / 8, 1);
    //imsetsamesize(win->im, win->imbmp);
  }

  return 0;
}



int FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin, const RGBQUAD* inpal)
{
  BITMAPINFOHEADER* bmih;
  assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

  bmih = &(bmi->bmiHeader);

  memset(bmih, 0, sizeof(*bmih));
  bmih->biSize = sizeof(BITMAPINFOHEADER);
  bmih->biWidth = width;
  bmih->biHeight = origin ? abs(height) : -abs(height);
  bmih->biPlanes = 1;
  bmih->biBitCount = (unsigned short) bpp;
  bmih->biCompression = BI_RGB;

  if (bpp <= 8) {
    RGBQUAD* palette = bmi->bmiColors;

    if (inpal) {
      memcpy(palette, inpal, 256 * sizeof(RGBQUAD));
    }
    else {
      int i;

      for (i = 0; i < 256; i++) {
        palette[ i ].rgbBlue = palette[ i ].rgbGreen = palette[ i ].rgbRed = (BYTE) i;
        palette[ i ].rgbReserved = 0;
      }
    }
  }

  return 0;
}

int CopyImageToDC(HDC hDC, int x, int y, const img_t* im, const RGBQUAD* inpal)
{
  int channels = 0;
  void* dst_ptr = 0;
  int origin = 0;
  uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
  BITMAPINFO* binfo = (BITMAPINFO*) buffer;
  int cnbit[] = {1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  HBITMAP hBmp = 0;
  HDC hMemDC = CreateCompatibleDC(hDC);
  int h, w;
  h = im->h;
  w = im->w;
  //int height, int width, const unsigned char* arr, int step, int cn;

  if (inpal) {
    LPLOGPALETTE lpPal = (LPLOGPALETTE)buffer;  // 指向逻辑调色板的指针
    HPALETTE hPal = NULL; // 调色板的句柄
    int i;
    lpPal->palVersion = 0x300;   // 设置版本号
    lpPal->palNumEntries = (WORD) 256;   // 设置颜色数目

    for (i = 0; i < (int) 256; i++) {
      lpPal->palPalEntry[ i ].peRed = inpal[ i ].rgbRed;
      lpPal->palPalEntry[ i ].peGreen = inpal[ i ].rgbGreen;
      lpPal->palPalEntry[ i ].peBlue = inpal[ i ].rgbBlue;
      lpPal->palPalEntry[ i ].peFlags = 0;
    }

    hPal = CreatePalette(lpPal);
    DeleteObject(SelectPalette(hDC, hPal, TRUE));
  }

  FillBitmapInfo(binfo, w, h, cnbit[im->c] * 8, 1, inpal);
  DeleteObject(SelectObject(hMemDC, hBmp = CreateDIBSection(hDC, binfo, DIB_RGB_COLORS, &dst_ptr, 0, 0)));

  {
    int i, dststep = (w * im->c + 3) & -4;
    unsigned char* dst = (unsigned char*)dst_ptr;
    dst += (h - 1) * dststep;

    //IM2IM( im->h, im->w, arr, step, im->c, dst, -dststep, channels );
    for (i = 0; i < h; ++i) {
      memcpy(dst + i * (-dststep), im->tt.data + i * im->s, im->w * im->c);
    }
  }

  BitBlt(hDC, x, y, w, h, hMemDC, 0, 0, SRCCOPY);
  DeleteObject(hBmp);
  DeleteDC(hMemDC);
  return 0;
}

int GetRectW(const RECT* prc)
{
  return prc->right - prc->left;
}
int GetRectH(const RECT* prc)
{
  return prc->bottom - prc->top;
}
int myGetClientRect(HWND hwnd, IRECT* prc)
{
  RECT rc;
  GetWindowRect(hwnd, &rc);
  ScreenToClient(hwnd, (POINT*)&rc);
  GetClientRect(hwnd, (RECT*)prc);
  OffsetRect((RECT*)prc, -rc.left, -rc.top);
  return 0;
}

#include "Vfw.h"
#pragma comment (lib,"Vfw32.lib")
HDRAWDIB hdd = 0;

int xEndPaint(xwin_t* win)
{
  HDC hDC;
  img_t* im = win->im;
  int step = (im->w * im->c + 3)&~3;
  uchar* data = MALLOC(uchar, im->h * step);
  BITMAPINFOHEADER biHeader;
  IRECT rc = {0};
  int h, w;
  imsetsamesize(im, win->im);
  memset(&biHeader, 0, sizeof(BITMAPINFOHEADER));
  biHeader.biBitCount = im->c * 8;
  biHeader.biCompression = BI_RGB;
  biHeader.biHeight = im->h;
  biHeader.biPlanes = 1;
  biHeader.biSize = sizeof(BITMAPINFOHEADER);
  biHeader.biWidth = im->w;

  hDC = GetWindowDC(win->hwnd);

  if (0 == hdd) {
    hdd = DrawDibOpen();
  }

  //imflip_ud(im);
  flip_ud_copy(im->h, im->w * im->c, data, step, im->tt.data, im->s);
  // 显示真彩位图
  DrawDibRealize(hdd, hDC, TRUE);

  myGetClientRect(win->hwnd, &rc);
  w = RCW(&rc), h = RCH(&rc);
  DrawDibDraw(hdd, hDC, rc.l, rc.t, w, h, &biHeader, (LPVOID)data, rc.l, rc.t, w, h, DDF_BACKGROUNDPAL);
  //DrawDibClose(hdd);
  ReleaseDC(win->hwnd, hDC);
  FREE(data);
  return 0;
}

int win_draw(xwin_t* win, HDC hDC, int x, int y)
{
  IRECT rc;
  //int h, w;
  img_t* im = win->im;
  img_t im1[1] = {0};
  //HWND hwnd = win->hwnd;
  //w = rc.r - rc.left, h = rc.bottom - rc.top;
  //imsetsize(im, h, w, 3, 1);
  //imdraw_circle(im, im->w/2, im->h/2, 200, _RGB(255, 0, 0), _RGB(0, 255, 0), 2);
  //imdraw_rect(im, iRECT(100, 100, 150, 150), _RGB(0, 255, 0), _RGB(255, 255, 255), 0);
  //imdraw_rect(im, iRECT(0, 0, im->w, im->h), _RGB(0, 255, 0), _RGB(255, 255, 0), 10);
  //imdraw_fill(im, _RGB(100, 100, 100));
  myGetClientRect(win->hwnd, &rc);
  imsubref(im, rc, im1);
  CopyImageToDC(hDC, x, y, im1, 0);
  //imfree(im);
  return 0;
}

LRESULT CALLBACK XMainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  xwin_t* win = (xwin_t*)GetWindowLong(hwnd, GWL_USERDATA);

  switch (message) {
  case WM_NOTIFY:
    switch (((LPNMHDR)lParam)->code) {
    case TTN_GETDISPINFO: {
      LPTOOLTIPTEXT lpttt;
      UINT idButton;
      lpttt = (LPTOOLTIPTEXT) lParam;
      lpttt->hinst = 0;
      idButton = lpttt->hdr.idFrom;

      switch (idButton) {
      }

      break;
    }

    default:
      break;
    }

    break;

  case WM_CREATE:
    //InvalidateRect(hwnd, 0, 0);
    break;

  case WM_SIZE:
    if (win) {
      IRECT rc;
      GetWindowRect(hwnd, (RECT*)&rc);
      xSetSize(win, RCH(&rc), RCW(&rc));
      //xSetSize(win, HIWORD(lParam), LOWORD(lParam));
      //PostMessage(win->hwnd, WM_REPAINT, 0, 0);
      //SendMessage(win->hwnd, WM_REPAINT, 0, 0);
      //printf("PostMessage WM_REPAINT\n");
    }

    break;

  case WM_ERASEBKGND:
    if (win) {
      win_draw(win, (HDC)wParam, 0, 0);
      return TRUE;
    }

    break;

    //case WM_NCPAINT:
  case WM_PAINT:
    if (win) {
      PAINTSTRUCT paint = {0};
      HDC hDC = BeginPaint(hwnd, &paint);
      win_draw(win, hDC, 0, 0);
      EndPaint(hwnd, &paint);
      return TRUE;
    }

    break;

  case WM_DESTROY:
    XRemoveWindow(win);
    break;

  default:
    break;
  }

  return DefWindowProc(hwnd, message, wParam, lParam);
}

static int iXRegisterClass(const char* szClassName)
{
  HINSTANCE hInstance = 0;
  HINSTANCE hPrevInstance = 0;
  WNDCLASS wndclass;

  wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wndclass.lpfnWndProc = XMainWndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = szClassName;

  if (!RegisterClass(&wndclass)) {
    MessageBox(NULL, TEXT("This program requires Windows NT!"), szClassName, MB_ICONERROR);
    return 0;
  }

  return 1;
}


static int showWindow(xwin_t* win, int x, int y, int cx, int cy, void* sub)
{
  xdisp_t* disp = g_disp;
  static HWND g_hWndParent = NULL;
  HINSTANCE hInstance = 0;
  static int xwin_inited = 0;
  DWORD dwStyle = 0;

  if (0 == xwin_inited) {
    xwin_inited = iXRegisterClass(XWINCLASS);
  }

  //win = iXFindWindowById(disp, winid);
  if (NULL == win->hwnd) {
    ASSERT(disp->nwin < XWINMAX);
    disp->win[disp->nwin++] = win;
    win->disp = disp;
    //win->winid = winid;
    //win->fr->text = text;
    //win->fr->has_cap = !!(window_styles&WS_CAPTION);
    //win->fr->has_bdr = !!(window_styles&WS_BORDER);
    //win->draw = draw;
    //win->user = user;
    dwStyle =  WS_POPUP;
    dwStyle =  WS_OVERLAPPED;
    dwStyle =  WS_OVERLAPPEDWINDOW;
    dwStyle |=  WS_VISIBLE;
    win->hwnd = CreateWindow(XWINCLASS, "", dwStyle, x, y, cx, cy, g_hWndParent, NULL, hInstance, NULL);
    xSetSize(win, cy, cx);
    SetWindowLong(win->hwnd, GWL_USERDATA, (LONG)win);
    SetClassLong(win->hwnd, GCL_HBRBACKGROUND, (LONG)(HBRUSH)0); // 去掉背景画刷 避免闪烁

    if (NULL == g_hWndParent) {
      g_hWndParent = win->hwnd;
    }
  }

  win->sub = sub;

  //win->hDC = CreateCompatibleDC(0);
  //CreateToolTipForRect(hwnd);
  //CreateTrackingToolTip(hwnd);
  if (win->hwnd) {
    //PostMessage(win->hwnd, WM_INIT, 0, 0);
    MoveWindow(win->hwnd, x, y, cx, cy, 0);
    ShowWindow(win->hwnd, 1);
    UpdateWindow(win->hwnd);
  }

  return 0;
}

static int XMoveWindow(xwin_t* win, int x, int y, int cx, int cy)
{
  return MoveWindow(win->hwnd, x, y, cx, cy, 0);
}

int xdisp_setwin(xdisp_t* disp, xwin_t* win, int nwin)
{
  //disp->win = win, disp->nwin = nwin;
  return 0;
}

#define XCTRLADD(disp, type, ctrl)  x##type##_t ctrl[1]={0}; int id_##ctrl = xwin_addctrl2(disp, dm_##type , ctrl)
#define XCTRLADD2(disp, type, ctrl, args )  x##type##_t ctrl[1]={0}; int id_##ctrl = ( x##type##_set args , xwin_addctrl2(disp, dm_##type , ctrl))
int xwin_addctrl2(xdisp_t* disp, int type, void* ctrl)
{
  int id = MAX(disp->ndi, 1);
  xctrlinfo_t* di = disp->ci + id;
  disp->ndi = MAX(disp->ndi, id + 1);
  ASSERT(disp->ndi < XDRAWST);
  memset(di, 0, sizeof(xctrlinfo_t));
  di->s = ctrl;
  di->t = type;
  di->id = id;
  return id;
}

int xwin_addctrl(xdisp_t* disp, int id, int type, void* ctrl)
{
  xctrlinfo_t* di = disp->ci + id;
  disp->ndi = MAX(disp->ndi, id + 1);
  ASSERT(disp->ndi < XDRAWST);
  memset(di, 0, sizeof(xctrlinfo_t));
  di->s = ctrl;
  di->t = type;
  di->id = id;
  return id;
}

// -1 没有窗口
// 0 有窗口，无消息
// 1 有窗口，有消息
int waitkey(int delay)
{
  HWND hwnd;
  UINT uMsg;
  WPARAM wParam;
  LPARAM lParam;
  xdisp_t* disp = g_disp;
  MSG message;
  int time0 = GetTickCount();

  for (;;) {
    int i, is_processed = 0;

    if (0 >= disp->nwin) {
      return -1;
    }

    //printf("GetMessage\n");
    if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay)) {
      return 0;
    }

    if (delay <= 0) {
      GetMessage(&message, 0, 0, 0);
    }
    else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
      Sleep(1);
      continue;
    }

    hwnd = message.hwnd, uMsg = message.message, wParam = message.wParam, lParam = message.lParam;

    if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_NCLBUTTONDOWN) {
      // disp->x = ;
    }

    for (i = 0; i < disp->nwin; ++i) {
      xwin_t* win = disp->win[i];

      if (win->hwnd == message.hwnd) {
        switch (uMsg) {
        case WM_CHAR:
          if (1) {
            DispatchMessage(&message);
            return (int)wParam;
          }

          break;

        case WM_KEYDOWN:
          if (1) {
            if ((message.wParam >= VK_F1 && message.wParam <= VK_F24) ||
                message.wParam == VK_HOME || message.wParam == VK_END ||
                message.wParam == VK_UP || message.wParam == VK_DOWN ||
                message.wParam == VK_LEFT || message.wParam == VK_RIGHT ||
                message.wParam == VK_INSERT || message.wParam == VK_DELETE ||
                message.wParam == VK_PRIOR || message.wParam == VK_NEXT) {
              DispatchMessage(&message);
              return (int)(wParam << 16);
            }
          }

          break;

          //case WM_NCLBUTTONDOWN:
          //case WM_NCLBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_MOUSELEAVE:
          if (win->onmouse) {
            int event = uMsg == WM_LBUTTONDOWN ? MSG_LBUTTONDOWN :
                uMsg == WM_RBUTTONDOWN ? MSG_RBUTTONDOWN :
                uMsg == WM_MBUTTONDOWN ? MSG_MBUTTONDOWN :
                uMsg == WM_LBUTTONUP ? MSG_LBUTTONUP :
                uMsg == WM_RBUTTONUP ? MSG_RBUTTONUP :
                uMsg == WM_MBUTTONUP ? MSG_MBUTTONUP :
                uMsg == WM_LBUTTONDBLCLK ? MSG_LBUTTONDBLCLK :
                uMsg == WM_MOUSEWHEEL ? MSG_MOUSEWHEEL :
                uMsg == WM_RBUTTONDBLCLK ? MSG_RBUTTONDBLCLK :
                uMsg == WM_MBUTTONDBLCLK ? MSG_MBUTTONDBLCLK : MSG_MOUSEMOVE;
            int flags = (wParam & MK_LBUTTON ? MSG_FLAG_LBUTTON : 0) |
                (wParam & MK_RBUTTON ? MSG_FLAG_RBUTTON : 0) |
                (wParam & MK_MBUTTON ? MSG_FLAG_MBUTTON : 0) |
                (wParam & MK_CONTROL ? MSG_FLAG_CTRLKEY : 0) |
                (wParam & MK_SHIFT ? MSG_FLAG_SHIFTKEY : 0) |
                (GetKeyState(VK_MENU) < 0 ? MSG_FLAG_ALTKEY : 0);
            int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);

            if (uMsg == WM_MOUSEWHEEL) {
              flags = GET_WHEEL_DELTA_WPARAM(wParam);
            }

            //GetCursorPos(&pt2);
            if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP || uMsg == WM_NCLBUTTONUP) {
              if (uMsg == WM_LBUTTONUP) {
                disp->ldown = 0;
              }

              ReleaseCapture();
            }
            else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_NCLBUTTONDOWN) {
              if (uMsg == WM_LBUTTONDOWN) {
                disp->ldown = 1;
              }

              SetCapture(hwnd);
            }

            win->onmouse(win, win->userdata, event, x, y, flags);
          }

          break;
        }
      }
    }

    if (!is_processed) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  return 0;
}

int xwin_exit(xdisp_t* disp)
{
  int i;

  for (i = 0; i < disp->nwin; ++i) {
    xwin_t* win = disp->win[i];
    XRemoveWindow(win);
  }

  disp->nwin = 0;
  return 0;
}
