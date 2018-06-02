

//#define ARGB(a,r,g,b) (((DWORD)(a)<<24) + ((DWORD)(r)<<16) + ((DWORD)(g)<<8) + (DWORD)(b))
//#define GETA(col) ((col)>>24)
//#define GETR(col) (((col)>>16) & 0xFF)
//#define GETG(col) (((col)>>8) & 0xFF)
//#define GETB(col) ((col) & 0xFF)
//#define SETA(col,a) (((col) & 0x00FFFFFF) + (DWORD(a)<<24))
//#define SETR(col,r) (((col) & 0xFF00FFFF) + (DWORD(r)<<16))
//#define SETG(col,g) (((col) & 0xFFFF00FF) + (DWORD(g)<<8))
//#define SETB(col,b) (((col) & 0xFFFFFF00) + DWORD(b))

//#pragma comment( linker, "/subsystem:/"windows/" /entry:/"mainCRTStartup/"" )
#include <windowsx.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
const char* g_szWindowClassName = "MyWindowClass";
static HINSTANCE g_hinstance = 0;
//#include "win2.inl"

int UI_Init(WNDPROC Proc)
{
  WNDCLASSEX wndc;
  //INITCOMMONCONTROLSEX InitCtrlEx;
  //InitCommonControlsEx(&InitCtrlEx);
  // first fill in the window struct stucture
  wndc.cbSize = sizeof(WNDCLASSEX);
  wndc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wndc.lpfnWndProc = Proc;
  wndc.cbClsExtra = 0;
  wndc.cbWndExtra = 0;
  wndc.hInstance = g_hinstance;
  wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndc.hCursor = LoadCursor(NULL, IDC_ARROW);
  //wndc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
  wndc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
  wndc.lpszClassName = g_szWindowClassName;
  wndc.lpszMenuName = g_szWindowClassName;
  wndc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  //register the window struct
  if (!RegisterClassEx(&wndc)) {
    MessageBox(NULL, "Class Registration Failed!", "Error", 0);
    //exit the application
    return 0;
  }

  return 1;
}
int Win_Main()
{
  //this will hold any windows messages
  MSG msg;

  //entry point of our message handler
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}
#if 0
int test_win()
{
  HWND hwnd;
  INITCOMMONCONTROLSEX InitCtrlEx;
  InitCommonControlsEx(&InitCtrlEx);
  hwnd = Win_Create(0, test_isee_WndProc);
  Win_Main();
  return 0;
}
#endif

int UpdateLWindow(HWND hwnd, HDC hdc, img_t* im, int isLay)
{
  if (im->h > 0 && im->w > 0) {
    POINT ptSrc = {0, 0};
    POINT ptDst = {0, 0};
    HDC hMemDC;
    hMemDC = CreateCompatibleDC(hdc);
    //printf("UpdateLayeredWindow\n");
    SelectImagePal(hMemDC, im->h, im->w, im->data, im->s, im->c, 0);

    //SetStretchBltMode(hdc, COLORONCOLOR);
    if (isLay) {
      SIZE size = {im->w, im->h};
      BLENDFUNCTION m_Blend;
      RECT rc[1];
      GetWindowRect(hwnd, rc);
      m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
      m_Blend.BlendFlags = 0; //nothingelseisspecial
      m_Blend.AlphaFormat = AC_SRC_ALPHA; //
      m_Blend.SourceConstantAlpha = 255; //AC_SRC_ALPHA Í¸Ã÷¶È
      ptDst.x = rc->left, ptDst.y = rc->top;
      UpdateLayeredWindow(hwnd, hdc, &ptDst, &size, hMemDC, &ptSrc, 0, &m_Blend, 2);
    }
    else {
      RECT rc[1];
      GetClientRect(hwnd, rc);
      BitBlt(hdc, ptDst.x, ptDst.y, RCW(rc), RCH(rc), hMemDC, ptSrc.x, ptSrc.y, SRCCOPY);
    }

    DeleteDC(hMemDC);
  }

  return 0;
}

enum {
  EVENT_INIT11 = WM_USER + 1,
  EVENT_LCLICK,
  EVENT_INIT,
  EVENT_EXIT,
  EVENT_SETATTRIBUTE,
  EVENT_DATA,
  EVENT_GETSIZE, // wp=&SIZE
  EVENT_SETSIZE, // wp=&SIZE
  EVENT_SETCTRL, // wp=&SIZE
  EVENT_SCROLL_SETPOS, //
  EVENT_GETMINMAXINFO, //
};


typedef struct UI_engine UI_engine;
typedef struct event {
  UI_engine* ui;
  void* sender;
  void* toer;
  uictrl** wid;
  //HWND hwnd;
  UINT msg;
  WPARAM wp;
  union {
    LPARAM lp;
    struct {
      short x, y;
    };
  };
  POINT pt;
} event;
typedef struct uictrl {
  RECT rc;
  draw_t draw_f;
  event_t event_f;
} uictrl;
typedef struct widget_t1 {
  RECT rc;
  uictrl* subctrl;
} widget_t1;

#include "layout.inl"

#define UISETCALL(p, d, e) ((p)->base.draw_f=(draw_t)(d), (p)->base.event_f=(event_t)(e))
#define UICALL_EVENT(p, e)  (((p) && (p)->event_f) ? (p)->event_f(p, e) : 0)
#define UICALL_DRAW(p, hDC) (((p) && (p)->draw_f) ? (p)->draw_f(p, e) : 0)
#define UICALL_DRAW2(p, e, type)   {event e2[1]; e2[0]=*e; e2->lp=type; e->ui->res->dm_draw_f(&p->base, e2);}
#define UIGETID(e, id)  (e)->wid[id]
//#define UIID_EVENT(id, e)  ((e)->wid ? UICALL_EVENT(UIGETID(e, id), e) : 0)
//#define UIID_DRAW(id, e)   ((e)->wid ? UICALL_DRAW(UIGETID(e, id), e) : 0)

#define UITOER(s, WID)  event e[1]={0}; int __1=(*e=*e0, 0), __2=(e->toer=(void*)s, 0), __3=(e->wid=WID, 0)
#define UIHDC(e)  UI_engine* ui=e->ui; const res_t* res = (const res_t* )(e)->sender; HDDC hDC = (HDDC)(e)->wp; int type=e->lp
#define UIEVT(e)  UI_engine* ui=e->ui; const RECT* prc=&s->base.rc
typedef struct UI_engine {
  HWND hwnd;
  res_t* res;
  img_t im[1];
  int isLayeredWindow; // ÊÇ·ñÊ¹ÓÃLayeredWindow·½·¨»æÖÆ
  void* focus;
  void* press;
  void* hot;
  uictrl* subctrl;
  uictrl* popup;
} UI_engine;

typedef struct uiwids {
  uictrl** wid;
  int widlen;
} uiwids;
typedef UI_engine* HUI;
#define toUI(e)        (e->ui)
//½¹µã£ºÊó±ê×îºó°´ÏÂµÄ¿Ø¼þ
#define UIIsFocus(s)   (toUI(e)->focus==(void*)(s))
#define UISetFocus(s)  (toUI(e)->focus=(s))

//Êó±ê°´ÏÂÊ±µÄ¿Ø¼þ
#define UIIsPress(s)   (toUI(e)->press==(s))
#define UISetPress(s)  (toUI(e)->press=(s))

//Êó±ê×îºóÅÌÐý
#define UIIsHot(s)     (toUI(e)->hot==(s))
#define UISetHot(s)    (toUI(e)->hot=(s))
#define UISetPopup(s)  (toUI(e)->popup=(s))
#define UIStage(s, t)  (t|(UIIsHot(s)?UIS_HOT:0)|(UIIsPress(s)?UIS_PUSHED:0)|(UIIsFocus(s)?UIS_FOCUSED:0))

void Win_Cleanup(void)
{
  UnregisterClass(g_szWindowClassName, g_hinstance);
}
int UISetLayeredWindow(UI_engine* ui, BOOL isLay)
{
  HWND hwnd = ui->hwnd;
  DWORD nStyle;
  ui->isLayeredWindow = isLay;

  if (!isLay) {
    SetWindowLong(hwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
    SetWindowLong(hwnd, GWL_EXSTYLE, 0);
  }
  else {
    nStyle = WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_POPUP;
    SetWindowLong(hwnd, GWL_STYLE, nStyle);
    SetWindowLong(hwnd, GWL_EXSTYLE, 0x80000);
  }

  //Ò»¶¨Òª¼ÓÕâ¾äÉèÖÃ²Å»áÁ¢¼´ÉúÐ§
  SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
  return 0;
}
// ½«´°¿ÚÏÔÊ¾ÔÚÆÁÄ»ÖÐÑë
int Win_MoveCenter(HWND hwnd, int w, int h)
{
  int cx = GetSystemMetrics(SM_CXSCREEN);
  int cy = GetSystemMetrics(SM_CYSCREEN);

  if (w <= 0 || h <= 0) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    w = RCW(&rect);
    h = RCH(&rect);
  }

  cx = (cx - w) / 2;
  cy = (cy - h) / 2;
  MoveWindow(hwnd, cx, cy, w, h, TRUE);
  return 0;
}
LRESULT CALLBACK UI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int Win_Init()
{
  static int inited = 0;

  if (0 == inited) {
    inited = 1;
    UI_Init(UI_WndProc);
    atexit(Win_Cleanup);

    if (1) {
      // ³õÊ¼»¯Ä¬ÈÏ×ÖÌå
      NONCLIENTMETRICS info;
      int minfontsize = 12;
      info.cbSize = sizeof(info);
      SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);

      if (abs(info.lfCaptionFont.lfHeight) < minfontsize) {
        info.lfCaptionFont.lfHeight = minfontsize;
      }

      //strcpy(info.lfCaptionFont.lfFaceName, "Fixedsys");
      SelectFontToMemDC(info.lfCaptionFont.lfFaceName, info.lfCaptionFont.lfHeight, 0);
    }
  }

  return 0;
}
int Win_Create(UI_engine* ui, const char* name, DWORD dwExStyle, DWORD dwStyle, HWND hParent)
{
  if (NULL == ui->hwnd) {
    HWND hwnd;
    //nStyle = WS_OVERLAPPEDWINDOW;
    hwnd = CreateWindowEx(dwExStyle, g_szWindowClassName, name, dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, hParent, NULL, g_hinstance, ui);

    if (!hwnd) {
      MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
      return 0;
    }

    ui->hwnd = hwnd;
    UpdateWindow(hwnd);
  }

  return 0;
}
int UI_setImpl(UI_engine* ui, const char* name, void* subctrl, HWND hParent)
{
  Win_Init();
  ui->subctrl = (uictrl*)subctrl;
  BCGPGLOBAL_DATA_set();
  ui->res = &globalData;
  Win_Create(ui, name, 0, 0, hParent);
  ShowWindow(ui->hwnd, 1);
  UISetLayeredWindow(ui, ui->isLayeredWindow);
  return 0;
}
int UI_set_popup(UI_engine* ui, const char* name, uictrl* subctrl, HWND hParent)
{
  Win_Init();
  ui->subctrl = subctrl;
  Win_Create(ui, name, 0, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, hParent);
  ShowWindow(ui->hwnd, 1);
  return 0;
}

#define UISendEvent(e, SENDER, TO, MSG, WP, LP)     if (TO) {event e1[1] = {0}; e1[0]=*e; \
    e1->sender = (uictrl*)(SENDER); e1->msg = MSG, e1->wp = WP, e1->lp = LP; \
    e1->pt.x = GET_X_LPARAM(LP), e1->pt.y = GET_Y_LPARAM(LP); UICALL_EVENT((uictrl*)(TO), e1); }

#define UISendRect(e, SENDER, TO, RC) if (TO) { ((uictrl*)(TO))->rc=RC; UISendEvent(e, SENDER, TO, WM_SIZE, 0, 0);}

#define force_redraw(s) (InvalidateRect((s)->ui->hwnd, 0, 0))
int force_redraw1(event* s)
{
  InvalidateRect((s)->ui->hwnd, 0, 0);
  return 0;
}
int force_redraw2(HUI ui)
{
  InvalidateRect(ui->hwnd, 0, 0);
  return 0;
}
int UIGetWorkArea(const UI_engine* ui, RECT* rc)
{
  GetClientRect(ui->hwnd, rc);
  return 0;
}

//#define force_redraw2(s) UIRedraw(&(s)->base), force_redraw1(s);
LRESULT CALLBACK UI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int ret = 0;
  UI_engine* ui = (UI_engine*)GetWindowLong(hwnd, GWL_USERDATA);
  event e[1] = {0};
  img_t* im;
  RECT rcClient = { 0 };
  GetClientRect(hwnd, &rcClient);

  if (WM_NCCREATE == uMsg) {
    LPCREATESTRUCT lpcs = (LPCREATESTRUCT)(lParam);
    ui = (UI_engine*)(lpcs->lpCreateParams);
    ui->hwnd = hwnd;
    SetWindowLong(hwnd, GWLP_USERDATA, (LPARAM)(ui));
  }

  if (NULL == ui) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    return 0;
  }

  im = ui->im;

  //e->hwnd = hwnd;
  e->msg = uMsg, e->wp = wParam, e->lp = lParam;
  e->ui = ui, e->sender = ui->subctrl, e->pt.x = e->x, e->pt.y = e->y;
  e->toer = ui->subctrl;

  if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN) {
    SetCapture(hwnd);
  }

  if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP) {
    ReleaseCapture();
  }

  //TraceEvt(e);
  switch (uMsg) {
  case WM_ERASEBKGND:
    // ±ÜÃâ¸Ä±ä´°¿Ú´óÐ¡Ê±ÉÁË¸
    return 1;
    break;

  case WM_SETFOCUS:
    break;

  case WM_CREATE:
    if (ui) {
      ui->hwnd = hwnd;
      UISendEvent(e, ui->subctrl, ui->subctrl, EVENT_INIT, 0, 0);
    }

    break;

  case WM_MOUSEMOVE:
  case WM_NCMOUSEMOVE:
    //printf("%d\n", GET_X_LPARAM(lParam));
    //e->msg = WM_MOUSEMOVE;
    break;

  case WM_MOUSEWHEEL:
    if (ui) {
      ScreenToClient(hwnd, &e->pt);
      e->x = (short)e->pt.x, e->y = (short)e->pt.y;
    }

    break;

  case WM_GETMINMAXINFO:
    if (1) {
      LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
      POINT pt = {100, 100};
      RECT rc, rc2;
      GetWindowRect(hwnd, &rc);
      GetClientRect(hwnd, &rc2);
      pt.x += RCW(&rc) - RCW(&rc2);
      pt.y += RCH(&rc) - RCH(&rc2);
      lpMMI->ptMinTrackSize = pt;
      //lpMMI->ptMaxPosition.x = lpMMI->ptMaxPosition.x = 0;
      //lpMMI->ptMaxTrackSize.x = GetSystemMetrics(SM_CXSCREEN);
      //lpMMI->ptMaxTrackSize.y = GetSystemMetrics(SM_CYSCREEN);
      SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
      lpMMI->ptMaxTrackSize.x = RCW(&rc);
      lpMMI->ptMaxTrackSize.y = RCH(&rc);
      return 0;
    }

    break;

  }

  if (ret = UICALL_EVENT(ui->popup, e)) {
    return ret;
  }

  //utime_start(_start_time);
  if (ui->subctrl) {
    ui->subctrl->rc = rcClient;
  }

  if (ret = UICALL_EVENT(ui->subctrl, e)) {
    return ret;
  }

  //TraceEvt(e);
  switch (uMsg) {
  case WM_TIMER:
    if (ui) {
      InvalidateRect(hwnd, 0, 0);
    }

    break;

  case WM_CREATE:
    if (1) {
      LONG nStyle = GetWindowLong(hwnd, GWL_STYLE);
      nStyle &= ~WS_CAPTION;
      SetWindowLong(hwnd, GWL_STYLE, nStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
      //SetTimer(hwnd, 11, 30, 0);
    }

    break;

  case WM_NCHITTEST:
    if (ui->isLayeredWindow) {
      //int hit = UICALL_EVENT(ui->win, e);
      //return hit;
    }

    break;

  case WM_LBUTTONDOWN:
    break;

  case WM_LBUTTONUP:
    UISetPress(0);
    break;

  case WM_SIZE: {
    //printf("%d %d   %d %d\n", RCH(&rcClient), RCW(&rcClient), HIWORD(lParam), LOWORD(lParam));
    imsetsize(im, HIWORD(lParam), LOWORD(lParam), 4, 1);
    InvalidateRect(hwnd, &rcClient, FALSE);
  }
  break;

  case WM_NCPAINT:
    if (0) {
      HDC hdc;
      RECT rcWin;
      hdc = GetWindowDC(hwnd);
      GetWindowRect(hwnd, &rcWin);
      OffsetRect(&rcWin, -rcWin.left, -rcWin.top);
      rcWin.bottom = rcWin.top + 20;
      FillSolidRect(hdc, &rcWin, RGB(255, 0, 0));
      ReleaseDC(hwnd, hdc);
      return 1;
    }

    break;

  case WM_PAINT:

    //memset(im->data, 255, im->h*im->w/4);
    if (ui->subctrl && im->h > 0 && im->w > 0 && im->data) {
      PAINTSTRUCT ps;
      DDC hDC[1] = {0};
      HDC hdc = BeginPaint(hwnd, &ps);
      e->sender = ui->res;
      e->wp = (UINT)hDC;
      HDDCSET_IMAGE(hDC, im);
      //memset(im->data, 0, im->h * im->s);
      UICALL_DRAW(ui->subctrl, e);
      UICALL_DRAW(ui->popup, e);

      //GetWindowRect(hwnd, &rct); ptWinPos.x = rct.left, ptWinPos.y = rct.top;
      UpdateLWindow(hwnd, hdc, im, ui->isLayeredWindow);
      EndPaint(hwnd, &ps);
      return 0;
    }

    break;

  case WM_DESTROY:
    UISendEvent(e, ui->subctrl, ui->subctrl, EVENT_EXIT, 0, 0);
    freeims(ui->im, countof(ui->im));
    //imfree(win->im);
    ShowWindow(ui->hwnd, FALSE);
    //PostQuitMessage(0);
    ui->hwnd = 0;
    return 0;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
HBRUSH GetHalftoneBrush()
{
  int i;
  HBRUSH g_HalftoneBrush;
  WORD grayPattern[8];
  HBITMAP grayBitmap;

  for (i = 0; i < 8; i++) {
    grayPattern[i] = (WORD)(0x5555 << (i & 1));
  }

  grayBitmap = CreateBitmap(8, 8, 1, 1, &grayPattern);

  if (grayBitmap != NULL) {
    g_HalftoneBrush = CreatePatternBrush(grayBitmap);
    DeleteObject(grayBitmap);
  }

  return g_HalftoneBrush;
}
int DrawDragRect(HWND hwnd, const RECT* pRect)
{
  HDC pDC = GetDC(hwnd);
  HBRUSH hHalftoneBrush = GetHalftoneBrush();
  HBRUSH pOldBrush = (HBRUSH)SelectObject(pDC, hHalftoneBrush);
  RECT rc;
  GetWindowRect(hwnd, &rc);
  PatBlt(pDC, pRect->left, pRect->top, RCW(pRect), RCH(pRect), PATINVERT);
  SelectObject(pDC, pOldBrush);
  DeleteObject(hHalftoneBrush);
  ReleaseDC(hwnd, pDC);
  return 0;
}
#define UI_start(user, islay) UI_startLoop(0, user, islay, TRUE)
enum {
  WK_CLOSEED = -2,
  WK_TIMEOUT = -1,
};
int winclose(UI_engine* ui)
{
  DestroyWindow(ui->hwnd);
  return 0;
}

int waitkey(UI_engine* ui, int delay)
{
  int time0 = GetTickCount();

  for (; ui;) {
    MSG message;
    int is_processed = 0;

    if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay)) {
      return WK_TIMEOUT;
    }

    if (delay <= 0) {
      GetMessage(&message, 0, 0, 0);
    }
    else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
      Sleep(1);
      continue;
    }

    if (NULL == ui->hwnd) {
      return WK_CLOSEED;
    }

    if (!is_processed) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  return 0;
}
int UISetIcon(UI_engine* ui, int nID)
{
  HWND hwnd = ui->hwnd;
  HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(0);
  HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(nID));
  SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
  SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
  return 0;
}
int SetFullScreen(HWND hWnd, RECT* rc_back)
{
  int frameCX, frameCY;
  int screenCX, screenCY;
  RECT rc;
  RECT rect;
  SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & (~(WS_CAPTION | WS_BORDER | WS_OVERLAPPED)));
  frameCX = GetSystemMetrics(SM_CXFRAME) - 1;
  frameCY = GetSystemMetrics(SM_CYFRAME) - 1;
  screenCX = GetSystemMetrics(SM_CXSCREEN);
  screenCY = GetSystemMetrics(SM_CYSCREEN);
  //captionCY = GetSystemMetrics(SM_CYCAPTION);

  GetWindowRect(hWnd, rc_back);
  GetClientRect(hWnd, &rc);
  rect.left = -frameCX, rect.right = frameCX + screenCX;
  rect.top = -frameCY, rect.bottom = frameCY + screenCY;
  ShowWindow(hWnd, SW_HIDE);
  SetForegroundWindow(hWnd);
  SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, RCW(&rect), RCH(&rect), SWP_SHOWWINDOW | SWP_NOZORDER);
  //SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
  return 0;
}
int SwitchFullScreen(HWND hWnd)
{
  static RECT rc_back = {0, 0, 300, 300};
  static int is_FullScreen = 0;


  //ShowTaskBar(is_FullScreen);
  if (is_FullScreen) {
    RECT rect;
    rect = rc_back;
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | ((WS_CAPTION | WS_BORDER)));
    ShowWindow(hWnd, SW_HIDE);
    SetWindowPos(hWnd, 0, rect.left, rect.top, RCW(&rect), RCH(&rect), SWP_SHOWWINDOW);
  }
  else {
    SetFullScreen(hWnd, &rc_back);
  }

  is_FullScreen = !is_FullScreen;
  return 0;
}
