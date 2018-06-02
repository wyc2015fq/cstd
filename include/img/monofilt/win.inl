
#include "../imgproc/draw.inl"
#include "VirtualKey.inl"
#include "input.inl"

typedef struct WINDEF WINDEF;
typedef struct WINDEF {
  LRESULT (*WndProc)(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam, WINDEF* wd);
  HWND hWnd;
  Image* im;
}WINDEF;

#define MYWINCLASS  "MYWINCLASS"
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
  WINDEF* wd = (WINDEF*)GetWindowLong(hWnd, GWL_USERDATA);
  
  if (NULL!=wd)
  {
    InputEvent event;
    Input_BuildEvent(hWnd, nMsg, wParam, lParam, &event);
    switch (nMsg) {
    case WM_SIZE:
      {
        int w = LOWORD(lParam); // width of client area
        int h = HIWORD(lParam); // height of client area
        int asdf=0;
        if (wd->im) {
          if (wd->im->h!=h && wd->im->w!=w) {
            pfree(wd->im);
            wd->im=newImage(w, h, 4);
          }
        } else {
          wd->im=newImage(w, h, 4);
        }
      }
      break;
    }
    return wd->WndProc(hWnd, nMsg, wParam, lParam, wd);
  }
  return DefWindowProc(hWnd, nMsg, wParam, lParam);
}
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEX wcex;
  
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style      = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;
  wcex.lpfnWndProc  = (WNDPROC)MyWndProc;
  wcex.cbClsExtra   = 0;
  wcex.cbWndExtra   = 0;
  wcex.hInstance    = hInstance;
  wcex.hIcon      = NULL;
  wcex.hCursor    = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground  = (HBRUSH) GetStockObject(DKGRAY_BRUSH);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName  = MYWINCLASS;
  wcex.hIconSm    = NULL;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
  
  return RegisterClassEx(&wcex);
}

HWND newwin(WINDEF* wd) {
  static HINSTANCE hInstance=0;
  static WORD atom=0;
  int w=327, h=300;
  if (0==hInstance) {
    hInstance=GetModuleHandle(0);
    atom=MyRegisterClass(hInstance);
  }
  wd->hWnd = CreateWindowEx(0, MYWINCLASS, "", WS_POPUP,
    CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, 0, hInstance, NULL);
  SetWindowLong(wd->hWnd, GWL_USERDATA, (LONG)wd);
  //MoveWindow(wd->hWnd, CW_USEDEFAULT, CW_USEDEFAULT, 327, 300, 1);
  wd->im=newImage(w, h, 4);
  return wd->hWnd;
}

// 消息循环
#define MSGLOOP_BEGIN(FPS)  {\
MSG msg; int nFixedDelta=1000/FPS, dt=0, t0=GetTickCount();\
while (msg.message != WM_QUIT) { if (PeekMessage(&msg, NULL, 0, 0 , PM_REMOVE)) {\
 TranslateMessage(&msg); DispatchMessage(&msg); \
  } else { int t1=GetTickCount(); if(t1-t0>=nFixedDelta) { t0=t1; {

#define MSGLOOP_END()  } } else {Sleep(1);} } } }

// 获取鼠标所在的像素颜色
COLORREF GetCursorColor() {
  POINT ptScreen;
  HDC hDeskDC = GetDC(0);
  COLORREF clr;
  GetCursorPos(&ptScreen);
  clr = GetPixel(hDeskDC, ptScreen.x, ptScreen.y);
  ReleaseDC(0, hDeskDC);
  return clr;
}

#include "ctrl.inl"

int ClickBtnOk(WINDEF* wd, TButton* btn) {
  //btnrc.top+=10;
  //btnrc.bottom+=10;
  //MessageBox(wd->hWnd, "text", "cap", 0);
  return 0;
}

int PtInRects(int n, const RECT *lprc, POINT pt) {
  int i;
  for (i=0; i<n; ++i) {if (PtInRect(lprc++, pt))return 1;}
  return 0;
}

int GetBackPic(int x, int y, int w, int h, void* data, int bw) {
  HWND hwnd=GetDesktopWindow();//获取桌面句柄
  HDC  hdc=GetWindowDC(hwnd);//获取设备环境 此处必须用GetWindowDC( ) 千万不要用GetDC( ) 不然会失败
  HDC hMemDC = CreateCompatibleDC(hdc);
  HBITMAP hBitmap, hOldBitmap;
  BITMAPINFO lpbiSrc[1];
  COLORREF* pSrcBits = NULL;
  
  // Fill in the BITMAPINFOHEADER
  lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  lpbiSrc->bmiHeader.biWidth = w;
  lpbiSrc->bmiHeader.biHeight = h;
  lpbiSrc->bmiHeader.biPlanes = 1;
  lpbiSrc->bmiHeader.biBitCount = 32;
  lpbiSrc->bmiHeader.biCompression = BI_RGB;
  lpbiSrc->bmiHeader.biSizeImage = w * h;
  lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
  lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
  lpbiSrc->bmiHeader.biClrUsed = 0;
  lpbiSrc->bmiHeader.biClrImportant = 0;
  hBitmap = CreateDIBSection(hdc, lpbiSrc, DIB_RGB_COLORS, (void**)&pSrcBits, 0, 0);
  // 把新位图选到内存设备描述表中
  hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
  
  BitBlt(hMemDC, 0, 0, w, h, hdc, x, y, SRCCOPY);
  //FLIP2D(h, w, pSrcBits, w, 1, dwt);
  //memcpy(data, pSrcBits, h*bw);
  memcpy2d(data, bw, pSrcBits+h*w-w, -w*4, h, w);
  DeleteObject(hBitmap);
  
  ReleaseDC(hwnd,hdc);//释放设备环境
  DeleteDC(hMemDC);
  return 0;
}

#include "..\hge\perpixelalpha.inl"

int UpdateFrameToWnd(HWND hWnd, int w, int h, LPVOID lpBits, BYTE nAlpha) {
  BITMAPINFO bmpInfo = { { sizeof(BITMAPINFOHEADER), w, h, 1, 32 } };
  RGBQUAD* pBitmapData = NULL;
  HBITMAP hBmp;
  int bytes_per_line=w*4;
  hBmp = CreateDIBSection(NULL, &bmpInfo, DIB_RGB_COLORS, (void**)&pBitmapData, NULL, 0);
  //memcpy(pBitmapData, lpBits, sizeof(DWORD)*h*w);
  memcpy2d(pBitmapData + (h-1)*w,
    -bytes_per_line, lpBits, bytes_per_line, h, bytes_per_line);
  UpdateLWindow(hWnd, hBmp, nAlpha);
  DeleteObject(hBmp);
  return 0;
}

//#include "../imgproc/test_draw.inl"
LRESULT test_newwin_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, WINDEF* wd)
{
  static RECT rcs[]={10, 10, 120, 30, 10, 40, 130, 70};
  static TButton btnok={0, "确定定a", ClickBtnOk};
  static GEdit edit1;
  PAINTSTRUCT ps;
  HDC hdc;
  TDrawDC ddc={0};
  if (wd->im) {
    INITDC(&ddc, wd->im->h, wd->im->w, wd->im->bw, wd->im+1);
    //TFillRect(&ddc, 0, GetCursorColor());
  }
  Button_WndProc(hWnd, message, wParam, lParam, wd, &btnok, rcs);
  Edit_WndProc(hWnd, message, wParam, lParam, wd, &edit1, rcs+1);
  
  switch (message) {
  case   WM_CREATE:
    break;
  case   WM_ERASEBKGND:
    if (0) {
      char* fn="D:/TDDOWNLOAD/code/Transparent/perpixelalpha/images/garden.psp";
      HBITMAP hb = LoadPSPFile(fn);
      UpdateLWindow(hWnd, hb, 130);
      DeleteObject(hb);
    }
    {
      RECT rcWin;
      GetWindowRect(hWnd, &rcWin);
      TFillRect(&ddc, 0, _GetSysColor(COLOR_3DFACE));
      TFillRect(&ddc, 0, _RGBA(75, 23, 43, 199));
      TFillRect(&ddc, 0, 0xEEA9C9E2);
      //GetBackPic(rcWin.left, rcWin.top, wd->im->w, wd->im->h, wd->im+1, wd->im->bw);
    }
    break;
  case   WM_SIZE:
    {
      if (wd->im) {
        const char* ttt="支持光标定位、选字、全选、复制、粘贴、剪切等操作。";
        //TDrawEdit(&ddc, rcs, GEDIT_SHOWOUTERFRAME, ttt, 22, 16, 20);
        //TDrawButton(&ddc, rcs+1, btnitemState, "asdf", 4);
      }
    }
    break;
  case   WM_COMMAND:
#if 0
    switch (LOWORD(wParam)) {
    case IDR_EXIT:
      PostQuitMessage(0);
      break;

    case IDR_START:
      InitGame(level);
      break;

    case IDR_SLECTPASS:
      DialogBox(hInst, (LPCTSTR)IDD_DIALOG1, hWnd, (DLGPROC)SelectPass);
      break;
    }

#endif
    break;
    
    case WM_KEYDOWN:
      {
        if (wParam == VK_ESCAPE)
        {
          // Destroy main window
          DestroyWindow(hWnd);
        }
        else if ((_TCHAR)wParam == '0')
        {
        }
        else if ((_TCHAR)wParam == '1')
        {
        }
        else if ((_TCHAR)wParam == '2')
        {
        }
        else if ((_TCHAR)wParam == '3')
        {
        }
        else if ((_TCHAR)wParam == '4')
        {
        }
        else if ((_TCHAR)wParam == '5')
        {
        }
        else if ((_TCHAR)wParam == '6')
        {
        }
        else if ((_TCHAR)wParam == 'B')
        {
        }
        else if ((_TCHAR)wParam == 'P')
        {
        }
        else if ((_TCHAR)wParam == ' ')
        {
        }
        else if ((_TCHAR)wParam == VK_LEFT)
        {
        }
        else if ((_TCHAR)wParam == VK_RIGHT)
        {
        }
        else if ((_TCHAR)wParam == VK_UP)
        {
        }
        else if ((_TCHAR)wParam == VK_DOWN)
        {
        }
        else if ((_TCHAR)wParam == 'R')
        {
        }
      }
      break;

  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;
    
  case WM_LBUTTONDBLCLK:
    break;
  case WM_LBUTTONDOWN:
    if (!PtInRects(countof(rcs), rcs, cPOINT(LOWORD(lParam), HIWORD(lParam)))) {
      SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    }
    break;
  case WM_LBUTTONUP:
    break;
    
  case WM_MOUSEMOVE:
    break;
  case WM_DESTROY:
    {
      PostQuitMessage(0);
    }
    break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}

int test_newwin_mainloop(WINDEF* wd) {
  if (wd->im) {
    //DrawFrameToHDC(wd->hWnd, wd->im->w, wd->im->h, wd->im+1, 0);
    UpdateFrameToWnd(wd->hWnd, wd->im->w, wd->im->h, wd->im+1, 255);
  }
  return 0;
}

int test_newwin()
{
  WINDEF wd={test_newwin_WndProc};
  const char* fontname;
  fontname="宋体";
  newwin(&wd);
  SelectFontToMemDC(fontname,18,0,0);
  
  //SetWindowLongPtr( wd.hWnd, GWL_STYLE, WS_POPUP );
  ShowWindow(wd.hWnd, 1);
  UpdateWindow(wd.hWnd);
  
  MSGLOOP_BEGIN(30);
  //mydraw.ProLG(hWnd);
  test_newwin_mainloop(&wd);
  MSGLOOP_END();
  return 0;
}
