#ifndef _WINCV_INL_
#define _WINCV_INL_
#undef WINVER
#undef _WIN32_WINNT
#define WINVER 0x0500 // this is needed for UpdateLayeredWindow
#define _WIN32_WINNT 0x0500 // this is needed for the WS_EX_LAYERED
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <assert.h>
#include "xml.h"
//#include "gdi.inl"
enum { CV_WINDOW_AUTOSIZE = 1 };
// YV
enum {
  CV_WND_PROP_FULLSCREEN = 0,
  CV_WND_PROP_AUTOSIZE = 1,
  CV_WINDOW_NORMAL = 0,
  CV_WINDOW_FULLSCREEN = 1
};
#define CV_CDECL
typedef void (CV_CDECL* CvTrackbarCallback)(int pos);
enum {
  CV_EVENT_MOUSEMOVE = 0,
  CV_EVENT_LBUTTONDOWN = 1,
  CV_EVENT_RBUTTONDOWN = 2,
  CV_EVENT_MBUTTONDOWN = 3,
  CV_EVENT_LBUTTONUP = 4,
  CV_EVENT_RBUTTONUP = 5,
  CV_EVENT_MBUTTONUP = 6,
  CV_EVENT_LBUTTONDBLCLK = 7,
  CV_EVENT_RBUTTONDBLCLK = 8,
  CV_EVENT_MBUTTONDBLCLK = 9,
  CV_EVENT_MOUSEWHEELUP,
  CV_EVENT_MOUSEWHEELDOWN
};
enum {
  CV_EVENT_FLAG_LBUTTON = 1,
  CV_EVENT_FLAG_RBUTTON = 2,
  CV_EVENT_FLAG_MBUTTON = 4,
  CV_EVENT_FLAG_CTRLKEY = 8,
  CV_EVENT_FLAG_SHIFTKEY = 16,
  CV_EVENT_FLAG_ALTKEY = 32
};
#pragma comment(lib,"comctl32.lib")
//#include "../../imgproc/cap.inl"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
const char* g_szApplicationName = "test_win";
const char* g_szWindowClassName = "MyWindowClass";
WNDCLASSEX g_winclass;
HWND Win_Create(HINSTANCE hInstance, WNDPROC Proc)
{
  //handle to our window
  HWND hWnd;
  // first fill in the window class stucture
  g_winclass.cbSize = sizeof(WNDCLASSEX);
  g_winclass.style = CS_HREDRAW | CS_VREDRAW;
  g_winclass.lpfnWndProc = Proc;
  g_winclass.cbClsExtra = 0;
  g_winclass.cbWndExtra = 0;
  g_winclass.hInstance = hInstance;
  g_winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  g_winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  //g_winclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
  g_winclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
  g_winclass.lpszMenuName = NULL;
  g_winclass.lpszClassName = g_szWindowClassName;
  g_winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  //register the window class
  if (!RegisterClassEx(&g_winclass)) {
    MessageBox(NULL, "Class Registration Failed!", "Error", 0);
    //exit the application
    return 0;
  }
  //create the window and assign its ID to hwnd
  hWnd = CreateWindowEx(0, // extended style
      g_szWindowClassName, // window class name
      g_szApplicationName, // window caption
      WS_OVERLAPPEDWINDOW, // window style
      0, // initial x position
      0, // initial y position
      WINDOW_WIDTH, // initial x size
      WINDOW_HEIGHT, // initial y size
      0, // parent window handle
      0, // window menu handle
      hInstance, // program instance handle
      0); // creation parameters
  //make sure the window creation has gone OK
  if (!hWnd) {
    MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
  }
  //make the window visible
  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);
  return hWnd;
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
  UnregisterClass(g_szWindowClassName, g_winclass.hInstance);
  return msg.wParam;
}
int ToolBar_SetImageList(HWND hWndTool, const char* buf2)
{
  int i, n, cx, cy;
  TBBUTTON tbb[100];
  HIMAGELIST Cil1 = ImageList_LoadImage(NULL, buf2, 16, 0, 0, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
  //TreeView_SetImageList(m_TreeCtrl, Cil1, TVSIL_NORMAL); //设置图象列表
  n = ImageList_GetImageCount(Cil1);
  SendMessage(hWndTool, TB_SETIMAGELIST, 0, (LPARAM)Cil1);
  assert(n < 100);
  for (i = 0; i < n; ++i) {
    tbb[i].iBitmap = i;
    tbb[i].fsState = TBSTATE_ENABLED;
    tbb[i].fsStyle = TBSTYLE_BUTTON;
    tbb[i].idCommand = i;
    tbb[i].iString = (INT_PTR)0;
  }
  ImageList_GetIconSize(Cil1, &cx, &cy);
  SendMessage(hWndTool, TB_ADDBUTTONS, n, (LPARAM)&tbb);
  SendMessage(hWndTool, TB_SETBUTTONSIZE, 0, (LPARAM)MAKELONG(cx, cy));
  SendMessage(hWndTool, TB_AUTOSIZE, 0, 0);
  return 0;
}
void make_bitmap_info_header(int iWidth, int iHeight, int dd, BITMAPINFOHEADER* bmih)
{
  bmih->biSize = sizeof(BITMAPINFOHEADER);
  bmih->biWidth = iWidth;
  bmih->biHeight = iHeight;
  bmih->biPlanes = 1;
  bmih->biBitCount = (unsigned short) dd;
  bmih->biCompression = BI_RGB;
  // 每行数据存储空间的四字节对齐
  bmih->biSizeImage = ((iWidth * dd / 8 + 3) & ~3) * iHeight;
  bmih->biXPelsPerMeter = 0;
  bmih->biYPelsPerMeter = 0;
  bmih->biClrUsed = 0;
  bmih->biClrImportant = 0;
}
#define BMPMARK(A) (((A)+3)&~3)
typedef struct _BITMAP_INFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[ 256 ];
} BITMAP_INFO;
// 矩阵拷贝 dst[n×sl] = src[n×dl]
void mat_copy(void* dst, int h, int w, int dl, const void* src, int sl)
{
  char* cdst, * csrc;
  cdst = (char*) dst;
  csrc = (char*) src;
  for (; h--; cdst += dl, csrc += sl) {
    memcpy(cdst, csrc, w);
  }
}
void make_bitmap_rgbquad(RGBQUAD pRGB[], int n)
{
  int i;
  for (i = 0; i < n; ++i) {
    pRGB->rgbBlue = pRGB->rgbGreen = pRGB->rgbRed = (BYTE) i;
    pRGB->rgbReserved = 0;
    pRGB++;
  }
}
// fixhw 保持高宽比
int ImgDrawOnDC(HDC hdc, int h, int w, const unsigned char* src, int al, int ai, int y, int x, int h2, int w2, int fixhw, int isflip)
{
  BITMAP_INFO bmih;
  unsigned char* img = NULL;
  unsigned char* img2 = NULL;
  int ret, iBicnt = ai * 8;
  //int w2 = w > 600 ? 600 : w;
  //int h2 = w > 600 ? ( 600 * h / w ) : h;
  int al2;
  if (h2 <= 0 || w2 <= 0 || h <= 0 || w <= 0) {
    return 0;
  }
  if (!src) {
    return 0;
  }
  if (fixhw) {
    int h3, w3;
    if (h2 * w > h * w2) {
      // 目标过高
      w3 = w2, h3 = h * w3 / w;
    }
    else {
      h3 = h2, w3 = w * h3 / h;
    }
    y += (h2 - h3) / 2, x += (w2 - w3) / 2;
    h2 = h3, w2 = w3;
  }
  al2 = BMPMARK((w2 * ai));
  make_bitmap_info_header(w2, h2, iBicnt, &(bmih.bmiHeader));
  make_bitmap_rgbquad(bmih.bmiColors, 256);
  img = (unsigned char*) malloc(h2 * al2);
  img2 = (unsigned char*) malloc(h2 * al2);
  IMRESIZE(h, w, src, al, ai, h2, w2, img2, al2, ai, INTER_BILINEARITY);
  if (isflip) {
    mat_copy(img, h2, al2, al2, img2 + (h2 - 1)*al2, -al2);
  }
  else {
    mat_copy(img, h2, al2, al2, (img2), al2);
  }
  ret = StretchDIBits(hdc, x, y, w2, h2, 0, 0, w2, h2, img, (BITMAPINFO*)(&(bmih.bmiHeader)), DIB_RGB_COLORS, SRCCOPY);
  free(img);
  free(img2);
  return ret;
}
HFONT AddFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic)
{
  LOGFONT lf = { 0 };
  HFONT hFont;
  GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
  _tcscpy(lf.lfFaceName, pStrFontName);
  lf.lfHeight = -nSize;
  if (bBold) {
    lf.lfWeight += FW_BOLD;
  }
  lf.lfUnderline = bUnderline;
  lf.lfItalic = bItalic;
  hFont = CreateFontIndirect(&lf);
  return hFont;
}
typedef struct _FontInfo {
  TCHAR szName[32];
  HFONT hFont;
} FontInfo;
#define STRncpy(s1, l1, s2) cstr_cpy(s1, l1, s2.s, s2.l)
int CreateFontInfo(FontInfo* fi, str_t s, str_t name)
{
  str_t s1;
  LOGFONT lf = { 0 };
  STRncpy(fi->szName, 32, name);
  GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
  if (0 < s.l) {
    s1 = STRsplit_str(s, ",", "\"", &s, lf.lfFaceName, LF_FACESIZE);
  }
  if (0 < s.l) {
    lf.lfHeight = -STRsplit_int(s, ",", "\"", &s);
  }
  for (; 0 < s.l;) {
    char ch;
    s1 = STRsplit(s, ",", "\"", &s);
    ch = tolower(*s1.s);
    switch (ch) {
    case 'b':
      lf.lfWeight += FW_BOLD;
      break;
    case 'i':
      lf.lfItalic = TRUE;
      break;
    case 'u':
      lf.lfUnderline = TRUE;
      break;
    default:
      assert(0);
      break;
    }
  }
  fi->hFont = CreateFontIndirect(&lf);
  return fi->hFont != NULL;
}
// Control styles
#define STYLE_EDIT (WS_TABSTOP|SS_LEFT)
#define STYLE_MULTIEDIT (WS_TABSTOP|SS_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL|ES_WANTRETURN)
#define STYLE_STATIC (SS_LEFT)
#define STYLE_STATIC_CENTER (SS_CENTER)
#define STYLE_STATIC_RIGHT (SS_RIGHT)
#define STYLE_RADIO (BS_AUTORADIOBUTTON)
#define STYLE_RADIO_GROUP (STYLE_RADIO|WS_GROUP|WS_TABSTOP)
#define STYLE_BUTTON (WS_TABSTOP|BS_PUSHBUTTON)
#define STYLE_GROUPBOX (BS_GROUPBOX)
#define STYLE_CHECKBOX (WS_TABSTOP|WS_GROUP|BS_AUTOCHECKBOX)
#define STYLE_COMBOBOX_DROPDOWN (WS_TABSTOP|WS_GROUP|WS_VSCROLL|CBS_DROPDOWN|CBS_SORT|CBS_AUTOHSCROLL) //|CBS_DISABLENOSCROLL)
#define STYLE_COMBOBOX_DROPDOWN_NOSORT (WS_TABSTOP|WS_GROUP|WS_VSCROLL|CBS_DROPDOWN|CBS_AUTOHSCROLL) //|CBS_DISABLENOSCROLL)
#define STYLE_COMBOBOX_SIMPLE (WS_TABSTOP|WS_GROUP|WS_VSCROLL|CBS_SIMPLE|CBS_SORT|CBS_AUTOHSCROLL) //|CBS_DISABLENOSCROLL)
#define STYLE_COMBOBOX_SIMPLE_NOSORT (WS_TABSTOP|WS_GROUP|WS_VSCROLL|CBS_SIMPLE|CBS_AUTOHSCROLL) //|CBS_DISABLENOSCROLL)
#define STYLE_COMBOBOX_DROPDOWNLIST (WS_TABSTOP|WS_GROUP|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_SORT|CBS_AUTOHSCROLL) //|CBS_DISABLENOSCROLL)
#define STYLE_COMBOBOX_DROPDOWNLIST_NOSORT (WS_TABSTOP|WS_GROUP|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_AUTOHSCROLL) //|CBS_DISABLENOSCROLL)
#define STYLE_LISTBOX (WS_TABSTOP|LBS_STANDARD)
#define STYLE_DATETIMEPICKER (WS_TABSTOP|DTS_RIGHTALIGN)
#define STYLE_TIMEPICKER (WS_TABSTOP|DTS_RIGHTALIGN|DTS_TIMEFORMAT)
// Default combo-style
//#define STYLE_COMBOBOX (STYLE_COMBOBOX_DROPDOWNLIST)
#define STYLE_COMBOBOX (WS_TABSTOP|WS_BORDER|WS_VSCROLL|CBS_DROPDOWNLIST)
#define STYLE_COMBOBOX_NOSORT (STYLE_COMBOBOX_DROPDOWNLIST_NOSORT)
// Control Extended styles
#define EXSTYLE_EDIT (WS_EX_CLIENTEDGE)
#define EXSTYLE_MULTIEDIT (WS_EX_CLIENTEDGE)
#define EXSTYLE_LISTBOX (WS_EX_CLIENTEDGE)
#define EXSTYLE_STATIC (0)
#define EXSTYLE_RADIO (0)
#define EXSTYLE_BUTTON (0)
#define EXSTYLE_GROUPBOX (0)
#define EXSTYLE_CHECKBOX (0)
#define EXSTYLE_COMBOBOX (0)
#define EXSTYLE_DATETIMEPICKER (0)
#define EXSTYLE_TIMEPICKER (0)
#define ROWSTEPSIZE 12
#define FIRSTROW1 10
#define FIRSTROW2 37
#define FIXEDCOL1 10
#define FIXEDCOL2 120
#define INPUTCOL 150
#define GROWLIMIT 6
typedef struct wininfo_t {
  TCHAR* szWinClass;
  TCHAR* szClass;
  DWORD dwStyle;
  DWORD dwExStyle;
} wininfo_t;
wininfo_t wininfo[] = {
  WC_BUTTON, _T("BUTTON"), BS_PUSHBUTTON, EXSTYLE_BUTTON,
  WC_BUTTON, _T("DEFPUSHBUTTON"), BS_DEFPUSHBUTTON, EXSTYLE_BUTTON,
  WC_BUTTON, _T("CHECKBOX"), BS_CHECKBOX, EXSTYLE_BUTTON,
  WC_BUTTON, _T("AUTOCHECKBOX"), BS_AUTOCHECKBOX, EXSTYLE_BUTTON,
  WC_BUTTON, _T("RADIOBUTTON"), BS_RADIOBUTTON, EXSTYLE_BUTTON,
  WC_BUTTON, _T("3STATE"), BS_3STATE, EXSTYLE_BUTTON,
  WC_BUTTON, _T("AUTO3STATE"), BS_AUTO3STATE, EXSTYLE_BUTTON,
  WC_BUTTON, _T("GROUPBOX"), BS_GROUPBOX, EXSTYLE_BUTTON,
  WC_BUTTON, _T("RADIO"), STYLE_RADIO, EXSTYLE_RADIO,
  WC_BUTTON, _T("RADIOGROUP"), STYLE_RADIO_GROUP, EXSTYLE_RADIO,
  WC_BUTTON, _T("OWNERDRAW"), BS_OWNERDRAW, EXSTYLE_BUTTON,
  WC_STATIC, _T("STATIC"), STYLE_STATIC, EXSTYLE_STATIC,
  WC_STATIC, _T("STATICFRAME"), STYLE_STATIC | SS_BLACKFRAME, EXSTYLE_STATIC,
  WC_EDIT, _T("EDIT"), STYLE_EDIT, EXSTYLE_EDIT,
  WC_EDIT, _T("MULTIEDIT"), STYLE_MULTIEDIT, EXSTYLE_MULTIEDIT,
  WC_LISTBOX, _T("LISTBOX"), STYLE_LISTBOX, EXSTYLE_LISTBOX,
  WC_COMBOBOX, _T("COMBOBOX"), STYLE_COMBOBOX, EXSTYLE_COMBOBOX,
  WC_COMBOBOXEX, _T("COMBOBOXEX"), STYLE_COMBOBOX, EXSTYLE_COMBOBOX,
  WC_COMBOBOXEX, _T("SimpleComboText"), STYLE_COMBOBOX_SIMPLE, EXSTYLE_COMBOBOX,
  WC_COMBOBOXEX, _T("DropDownListComboText"), STYLE_COMBOBOX_DROPDOWNLIST, EXSTYLE_COMBOBOX,
  WC_IPADDRESS, _T("IpAddress"), WS_TABSTOP, 0,
  WC_LISTVIEW, _T("ListView"), WS_TABSTOP | WS_BORDER | LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS, 0,
  WC_TREEVIEW, _T("TreeView"), WS_TABSTOP | WS_BORDER, 0,
  WC_TABCONTROL, _T("Tab"), WS_TABSTOP | TCS_FIXEDWIDTH, 0,
  WC_SCROLLBAR, _T("SCROLLBAR"), WS_TABSTOP | WS_BORDER, 0,
  WC_HEADER, _T("HEADER"), WS_TABSTOP | WS_BORDER, 0,
  WC_PAGESCROLLER, _T("PAGESCROLLER"), WS_TABSTOP | WS_BORDER, 0,
  WC_NATIVEFONTCTL, _T("NATIVEFONTCTL"), WS_TABSTOP | WS_BORDER, 0,
  WC_SCROLLBAR, _T("SCROLLBAR"), WS_TABSTOP | WS_BORDER, 0,
  TOOLTIPS_CLASS, _T("TOOLTIPS"), 0, 0,
  UPDOWN_CLASS, _T("updown"), WS_TABSTOP | UDS_SETBUDDYINT | UDS_ARROWKEYS | UDS_NOTHOUSANDS, 0,
  PROGRESS_CLASS, _T("progress"), WS_TABSTOP | WS_BORDER, 0,
  TRACKBAR_CLASS, _T("trackbar"), WS_TABSTOP | TBS_NOTICKS | TBS_RIGHT, 0,
  HOTKEY_CLASS, _T("hotkey"), WS_TABSTOP | WS_BORDER, 0,
  ANIMATE_CLASS, _T("Animate"), WS_TABSTOP | WS_BORDER, 0,
  MONTHCAL_CLASS, _T("MonthCal"), WS_TABSTOP | MCS_NOTODAY, 0,
  DATETIMEPICK_CLASS, _T("DateTimePick"), WS_TABSTOP | DTS_RIGHTALIGN, 0,
  TOOLBARCLASSNAME, _T("Toolbar"), WS_TABSTOP | DTS_RIGHTALIGN | TBSTYLE_TOOLTIPS, 0,
  STATUSCLASSNAME, _T("StatusBar"), CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE, 0,
  _T("MediaPlayer.MediaPlayer.1"), _T("MediaPlayer"), WS_TABSTOP | WS_BORDER, 0,
  _T("Menu"), _T("Menu"), 0, 0,
  WC_STATIC, _T("cap"), 0, 0,
  _T("MAPEDIT"), _T("MAPEDIT"), 0, 0,
  WC_STATIC, _T("image"), WS_TABSTOP | WS_BORDER | STYLE_STATIC, EXSTYLE_STATIC,
  //TOOLTIPS_CLASS, _T("tooltip"), 0, 0,
} ;
#define WININFOLEN (sizeof(wininfo) / sizeof(wininfo[0]))
typedef struct _Widget Widget;
typedef int (*on_click_t)(struct _Widget* wid, int lParam, void* userdata);
typedef struct _Widget {
  HWND hwnd;
  TCHAR szName[32];
  wininfo_t* winfo;
  DWORD dwStyle;
  DWORD dwExStyle;
  RECT iniRect; // initial pos and size of the component
  on_click_t on_click;
  on_click_t on_notify;
} Widget;
int CreateWidget(Widget* w, STRTREE* pmm, int pos, HWND hParent, int nId, HINSTANCE hInstance)
{
  //hwnd = Wnd_CreateEx(szClass, szWindow, dwStyle, dwExStyle, hParent);
  int i, ai, ni;
  char szClass[32];
  char szText[256];
  char buf[256];
  ni = XmlFindNode(pmm, pos, XML_MARK_ATTRIBUTENAME, "name", 4);
  if (ni == 0) {
    return 0;
  }
  //printf("%s", pmm->node[j].name.s);
  ai = XmlFindNode(pmm, pos, XML_MARK_ATTRIBUTENAME, "text", 4);
  STRncpy(szClass, 256, pmm->node[pos].name);
  STRncpy(w->szName, 32, pmm->node[ni + 1].name);
  if (ai >= 0) {
    STRncpy(szText, 256, pmm->node[ai + 1].name);
  }
  w->hwnd = NULL;
  w->winfo = NULL;
  for (i = 0; i < WININFOLEN; ++i) {
    if (0 == stricmp(wininfo[i].szClass, szClass)) {
      w->winfo = wininfo + i;
      break;
    }
  }
  if (w->winfo && 0 == stricmp("Menu", w->winfo->szWinClass)) {
    char buf[256]; //获取顶级菜单
    HMENU hMenu[10];
    int j, i = XmlFindFirstNode(pmm, pos, XML_MARK_TEXT);
    str_t s = {0};
    if (i >= 0) {
      int preind = -1;
      s = pmm->node[i].name;
#if 1
      hMenu[0] = CreateMenu();
      for (j = 0; 0 < s.l; ++j) {
        STRsplit_str(s, "\n", "\n\r", &s, buf, 256);
        if (strlen(buf) < 1) {
          continue;
        }
        {
          int ind = 0;
          char* buf2 = buf;
          for (; *buf2 && (*buf2 == ' ' || *buf2 == '\t'); ++buf2, ++ind) {}
          if (ind > preind) {
            //TreeView_Expand(m_TreeCtrl, hCur[preind],TVE_EXPAND);//展开树
          }
          {
            int ind2 = 0;
            str_t s2 = s;
            char buf21[256];
            char* buf3;
            STRsplit_str(s2, "\n", "\n\r", &s2, buf21, 256);
            buf3 = buf21;
            for (; *buf3 && (*buf3 == ' ' || *buf3 == '\t'); ++buf3, ++ind2) {}
            if (ind2 == ind + 1) {
              hMenu[ind2] = CreateMenu();
              AppendMenu(hMenu[ind], MF_STRING | MF_POPUP, (UINT)hMenu[ind2], buf2);
            }
            else {
              if (0 == strcmp(buf2, "-")) {
                AppendMenu(hMenu[ind], MF_SEPARATOR, 0, 0);
              }
              else {
                AppendMenu(hMenu[ind], MF_STRING, j, buf2);
              }
            }
          }
          //AppendMenu(hMenu, MF_SEPARATOR, 0);
          preind = ind;
        }
      }
#else
      hMenu[0] = CreateMenu();
      hMenu[1] = CreateMenu();
      AppendMenu(hMenu[0], MF_POPUP, (UINT)hMenu[1], _T("VC++"));
      for (i = 0; i < 5; i++) {
        _snprintf(buf, 256, "Op &%d", i + 1);
        AppendMenu(hMenu[1], MF_STRING, 78 + i, buf);
        AppendMenu(hMenu[1], MF_SEPARATOR, 0, 0);
      }
#endif
      SetMenu(hParent, hMenu[0]);
      //POINT p;
      //GetCursorPos(&p);
      //m_Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
    }
    return NULL != hMenu[0];
  }
  if (NULL != w->winfo) {
    w->dwStyle = w->winfo->dwStyle | WS_CHILD | WS_VISIBLE;
    w->dwExStyle = w->winfo->dwExStyle;
    if (0 == stricmp(WC_COMBOBOX, w->winfo->szWinClass)) {
      w->hwnd = CreateWindowEx(w->dwExStyle, w->winfo->szWinClass, szText, w->dwStyle, 0, 0, 72, 512, hParent, (HMENU)nId, hInstance, NULL);
    }
    else {
      //hwnd = CreateWindowEx(dwExStyle, szClass, szWindow, dwStyle, 0, 0, 50, 20, hParent, NULL, hInstance, NULL);
      w->hwnd = CreateWindowEx(w->dwExStyle, w->winfo->szWinClass, szText, w->dwStyle, 0, 0, 72, 24, hParent, (HMENU)nId, hInstance, NULL);
    }
  }
  if (w->hwnd) {
    int j, i = XmlFindFirstNode(pmm, pos, XML_MARK_TEXT);
    str_t s = {0, 0};
    if (i >= 0) {
      s = pmm->node[i].name;
    }
    if (0 == stricmp(WC_TABCONTROL, w->winfo->szWinClass)) {
      TCITEM tie = {0};
      tie.mask = TCIF_TEXT;
      for (j = 0; 0 < s.l; ++j) {
        STRsplit_str(s, "\n", "\n\r", &s, buf, 256);
        //设置TabControl控件的标签
        tie.pszText = buf;
        TabCtrl_InsertItem(w->hwnd, j, (LPARAM)&tie);
      }
    }
    if (0 == stricmp(WC_COMBOBOX, w->winfo->szWinClass)) {
      ComboBox_ResetContent(w->hwnd);
      for (j = 0; 0 < s.l; ++j) {
        STRsplit_str(s, "\n", "\n\r", &s, buf, 256);
        ComboBox_InsertString(w->hwnd, -1, buf);
      }
      ComboBox_SetCurSel(w->hwnd, 0);
    }
    if (0 == stricmp(WC_LISTBOX, w->winfo->szWinClass)) {
      ListBox_ResetContent(w->hwnd);
      for (j = 0; 0 < s.l; ++j) {
        STRsplit_str(s, "\n", "\n\r", &s, buf, 256);
        ListBox_InsertString(w->hwnd, -1, buf);
      }
      ListBox_SetCurSel(w->hwnd, 0);
    }
    if (0 == stricmp(WC_LISTVIEW, w->winfo->szWinClass)) {
      int n = 0, k;
      int nCol = 0;
      char buf2[256];
      LVCOLUMN lvColumn = {0};
      LV_ITEM item = {0}; // 项
      HWND hwndListView = w->hwnd;
      DWORD dwExStyle =
          LVS_EX_FULLROWSELECT | //设置可选整行
          LVS_EX_GRIDLINES | //listView的item的每个栏目之间添加分割线
          LVS_EX_HEADERDRAGDROP |//允许litview的目录通过拖放重排序
          LVS_EDITLABELS;
      ListView_SetExtendedListViewStyle(hwndListView, dwExStyle);
      //ListView_SetTextColor(hwndListView,RGB(12,12,12));//字体颜色。
      ZeroMemory(&item, sizeof(LV_ITEM));
      ZeroMemory(&lvColumn, sizeof(LV_COLUMN));
      lvColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
      lvColumn.cx = 100;
      lvColumn.fmt = LVCFMT_CENTER;
      item.mask = LVIF_TEXT; // 文字
      item.cchTextMax = MAX_PATH; // 文字长度
      item.iItem = 0;
      item.iSubItem = 0;
#if 1
      for (j = 0; 0 < s.l; ++j) {
        str_t s2 = STRsplit_str(s, "\n", "\n\r", &s, buf, 256);
        int m, llen = strlen(buf);
        char* s = buf;
        if (0 == j) {
          for (k = 0, m = 0; 0 < s2.l; ++k) {
            STRsplit_str(s2, ",", " \r\n\"", &s2, buf2, 256);
            lvColumn.pszText = buf2;
            ListView_InsertColumn(hwndListView, k, (LPARAM)&lvColumn);
          }
          nCol = k;
        }
        else {
          int now;
          for (k = 0, m = 0; 0 < s2.l && k < nCol; ++k) {
            STRsplit_str(s2, ",", " \r\n\"", &s2, buf2, 256);
            if (0 == k) {
              item.iItem = j;
              item.pszText = buf2;
              now = ListView_InsertItem(hwndListView, (LPARAM)&item);
            }
            else {
              ListView_SetItemText(hwndListView, now, k, buf2);
            }
          }
        }
      }
#else
      {
        int i, now;
        char* MyColumn[4] = {TEXT("序号"), TEXT("列二"), TEXT("列三"), TEXT("列四")};
        for (i = 0; i < 4; i++) {
          lvColumn.pszText = MyColumn[i];
          ListView_InsertColumn(hwndListView, i, (LPARAM)&lvColumn);
        }
        //ListView_SetItemText(hwndListView,now,0,TEXT("Column 0"));
        ListView_SetItemText(hwndListView, now, 1, TEXT("Column 1"));
        ListView_SetItemText(hwndListView, now, 2, TEXT("Column 2"));
        item.pszText = "日本";
        item.iItem = 1;
        ListView_InsertItem(hwndListView, (LPARAM)&item);
      }
#endif
    }
    if (0 == stricmp(WC_TREEVIEW, w->winfo->szWinClass)) {
      HWND m_TreeCtrl = w->hwnd;
      char* buf2;
      int preind = 0;
      HTREEITEM hCur[100];//树控制项目句柄
      TV_INSERTSTRUCT TCItem;//?迦胧菹钍萁峁?
      DWORD dwStyles = GetWindowLong(m_TreeCtrl, GWL_STYLE); //获取树控制原风格
      dwStyles |= TVS_EDITLABELS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
      SetWindowLong(m_TreeCtrl, GWL_STYLE, dwStyles); //设置风格
      TCItem.hParent = TVI_ROOT; //增加根项
      TCItem.hInsertAfter = TVI_LAST; //在最后项之后
      TCItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; //设屏蔽
      TCItem.item.pszText = "数据选择";
      TCItem.item.lParam = 0; //序号
      TCItem.item.iImage = 0; //正常图标
      TCItem.item.iSelectedImage = 1; //选中时图标
      for (j = 0; 0 < s.l; ++j) {
        STRsplit_str(s, "\n", "\n\r", &s, buf, 256);
        if (strlen(buf) < 1) {
          continue;
        }
        if (0 == strncmp("ImageList=", buf, 10)) {
          HIMAGELIST Cil1;//大小图标像列表
          buf2 = buf + 10;
          for (; *buf2 && *buf2 == ' '; ++buf2) {}
          Cil1 = ImageList_LoadImage(NULL, buf2, 16, 0, 0, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
          TreeView_SetImageList(m_TreeCtrl, Cil1, TVSIL_NORMAL); //设置图象列表
          continue;
        }
        else {
          int ind = 0;
          buf2 = buf;
          for (; *buf2 && (*buf2 == ' ' || *buf2 == '\t'); ++buf2, ++ind) {}
          TCItem.hParent = ind == 0 ? TVI_ROOT : hCur[ind - 1];
          TCItem.item.pszText = buf2;
          hCur[ind] = TreeView_InsertItem(m_TreeCtrl, &TCItem);
          if (ind > preind) {
            TreeView_Expand(m_TreeCtrl, hCur[preind], TVE_EXPAND); //展开树
          }
          preind = ind;
        }
      }
#if 0
      char* CJ[4] = {"玉溪卷烟厂", "云南卷烟厂", "沈阳卷烟厂", "成都卷烟厂"}; //根数据名称
      char* PM[4][5] = {
        {"红梅一", "红梅二", "红梅三", "红梅四", "红梅五"}, //产品数据项
        {"白梅一", "白梅二", "白梅三", "白梅四", "白梅五"},
        {
          "绿梅一", "绿梅二", "绿梅三", "绿梅四", "绿?肺?},
          {
          "青 ? 芬 ? , "青梅二", "青梅三", "青梅四", "青梅五"
        }
      };
      int i, j;
      hRoot = TreeView_InsertItem(m_TreeCtrl, &TCItem); //返回根项句柄
      for (i = 0; i < 4; i++) { //增加各厂家
        TCItem.hParent = hRoot;
        TCItem.item.pszText = CJ[i];
        TCItem.item.lParam = (i + 1) * 10; //子项序号
        hCur = TreeView_InsertItem(m_TreeCtrl, &TCItem);
        for (j = 0; j < 5; j++) { //增加各产品
          TCItem.hParent = hCur;
          TCItem.item.pszText = PM[i][j];
          TCItem.item.lParam = (i + 1) * 10 + (j + 1); //子项序号
          TreeView_InsertItem(m_TreeCtrl, &TCItem);
        }
        TreeView_Expand(m_TreeCtrl, hCur, TVE_EXPAND); //展开树
      }
      TreeView_Expand(m_TreeCtrl, hRoot, TVE_EXPAND); //展开上一级树
#endif
    }
    if (0 == stricmp(TOOLBARCLASSNAME, w->winfo->szWinClass)) {
      TBBUTTON tbb[30];
      TBADDBITMAP tbab = {0};
      HWND hWndTool = w->hwnd;
      char* buf2 = buf;
      HIMAGELIST Cil1 = 0; //大小图标像列表
      SendMessage(hWndTool, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
#if 1
      for (j = 0; 0 < s.l; ++j) {
        STRsplit_str(s, "\n", "\n\r", &s, buf, 256);
        if (strlen(buf) < 1) {
          continue;
        }
        if (0 == strncmp("ImageList = ", buf, 10)) {
          buf2 = buf + 10;
          for (; *buf2 && *buf2 == ' '; ++buf2) {}
          Cil1 = ImageList_LoadImage(NULL, buf2, 16, 0, 0, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
          //TreeView_SetImageList(m_TreeCtrl, Cil1, TVSIL_NORMAL); //设置图象列表
          break;
        }
      }
      if (Cil1) {
        int i, n = ImageList_GetImageCount(Cil1), cx, cy;
        SendMessage(hWndTool, TB_SETIMAGELIST, 0, (LPARAM)Cil1);
        for (i = 0; i < n; ++i) {
          tbb[i].iBitmap = i;
          tbb[i].fsState = TBSTATE_ENABLED;
          tbb[i].fsStyle = TBSTYLE_BUTTON;
          tbb[i].idCommand = 0;
          tbb[i].iString = (INT_PTR)0;
        }
        ImageList_GetIconSize(Cil1, &cx, &cy);
        SendMessage(hWndTool, TB_ADDBUTTONS, n, (LPARAM)&tbb);
        SendMessage(hWndTool, TB_SETBUTTONSIZE, 0, (LPARAM)MAKELONG(cx, cy));
        SendMessage(hWndTool, TB_AUTOSIZE, 0, 0);
      }
#else
      tbab.hInst = HINST_COMMCTRL;
      tbab.nID = IDB_STD_SMALL_COLOR;
      if (SendMessage(hWndTool, TB_ADDBITMAP, 1, (LPARAM)&tbab) == -1) {
        MessageBox(hwnd, TEXT("失败"), TEXT("创建"), 0);
      }
      ZeroMemory(tbb, sizeof(tbb));
      tbb[0].iBitmap = STD_FILENEW;
      tbb[0].fsState = TBSTATE_ENABLED;
      tbb[0].fsStyle = TBSTYLE_BUTTON;
      tbb[0].idCommand = 0;
      tbb[0].iString = (INT_PTR)"new";
      tbb[1].iBitmap = STD_FILEOPEN;
      tbb[1].fsState = TBSTATE_ENABLED;
      tbb[1].fsStyle = TBSTYLE_BUTTON;
      tbb[1].idCommand = 1;
      tbb[1].iString = (INT_PTR)"open";
      tbb[2].iBitmap = STD_FILESAVE;
      tbb[2].fsState = TBSTATE_ENABLED;
      tbb[2].fsStyle = TBSTYLE_BUTTON;
      tbb[2].idCommand = 2;
      tbb[2].iString = (INT_PTR)"save";
      SendMessage(hWndTool, TB_ADDBUTTONS, 3, (LPARAM)&tbb);
      SendMessage(hWndTool, TB_SETBUTTONSIZE, 0, (LPARAM)MAKELONG(35, 35));
      SendMessage(hWndTool, TB_AUTOSIZE, 0, 0);
#endif
    }
    if (0 == stricmp(STATUSCLASSNAME, w->winfo->szWinClass)) {
      int nCount, array[30] = {0};
      str_t s0 = s;
      for (j = 0; 0 < s.l;) {
        str_t s1 = STRsplit_str(s, "\n", " \t\n\r", &s, buf, 256);
        if (0 < s1.l) {
          array[j] = atoi(s1.s);
          ++j;
        }
      }
      nCount = j;
      SendMessage(w->hwnd, SB_SETPARTS, (WPARAM)nCount, (LPARAM)array);
      s = s0;
      for (j = 0; 0 < s.l && j < nCount;) {
        str_t s1 = STRsplit_str(s, "\n", " \t\n\r", &s, buf, 256);
        if (0 < s1.l) {
          STRsplit_str(s1, ", ", " \t\n\r\"", &s1, buf, 256);
          STRsplit_str(s1, ",", " \t\n\r\"", &s1, buf, 256);
          SendMessage(w->hwnd, SB_SETTEXT, (LPARAM)(j), (WPARAM)buf);
          ++j;
        }
      }
    }
  }
  return NULL != w->hwnd;
}
#define LAY_HEADER_ADAPT 'A'
#define LAY_HEADER_EXPAND 'X'
#define LAY_EXPAND_HORIZONTAL '-'
#define LAY_EXPAND_VERTICAL '+'
#define MAXCTRL 1000000
typedef struct _Layout {
  RECT margin; // left and right margin
  int Hgap; // distance between columns
  int Vgap; // distance between rows
  int* id; // 控件id对应wid
  int rows, cols;
  RECT iniRect;
  HWND hwnd;
} Layout;
typedef struct _LayoutData {
  BOOL isLaidOut; // if the component has been found in the layout array
  // and therefore if indxPos is a valid calculated field
  RECT indxPos;
  RECT iniRect;
  // left = column index
  // right = last column index
  // top = row index
  // right = last row index
  int x, y, dx, dy;
  BOOL inited;
  BOOL inited2;
} LayoutData;
int GetLayoutRect(Layout* lay, Widget* wid, int widlen, RECT* prect)
{
  int i, j;
  prect->left = 0, prect->top = 0;
  for (i = 1; i < lay->rows; ++i) {
    int w = 0;
    for (j = 1; j < lay->cols; ++j) {
      int indx = lay->id[i * lay->cols + j];
      if (indx < 0 || indx >= MAXCTRL || indx >= widlen) {
        continue;
      }
      w += wid[indx].iniRect.right - wid[indx].iniRect.left;
    }
    prect->right = MAX(w, prect->right);
  }
  for (j = 1; j < lay->cols; ++j) {
    int h = 0;
    for (i = 1; i < lay->rows; ++i) {
      int indx = lay->id[i * lay->cols + j];
      if (indx < 0 || indx >= MAXCTRL || indx >= widlen) {
        continue;
      }
      h += wid[indx].iniRect.bottom - wid[indx].iniRect.top;
    }
    prect->bottom = MAX(h, prect->bottom);
  }
  return 0;
}
int FindID(str_t s, Widget* wid, int widlen)
{
  int i;
  for (i = 0; i < widlen; ++i) {
    if (0 == cstr_cmp(s.s, s.l, wid[i].szName, strlen(wid[i].szName))) {
      return i;
    }
  }
  return -1;
}
int CreateLayout(Layout* lay, str_t s, Widget* wid, int widlen, str_t* layname, int laylen)
{
  int i, j, k;
  str_t s0 = s;
  str_t s1 = STRsplit(s, "\n", "\r\n", &s);
  lay->rows = lay->cols = 0;
  sscanf(s1.s, "EvaLayout,%d,%d,%d,%d,%d,%d", &lay->margin.left, &lay->margin.top, &lay->margin.right, &lay->margin.bottom, &lay->Hgap, &lay->Vgap);
  s0 = s;
  for (j = 0; 0 < s.l; ++j) {
    str_t s1 = STRsplit_str(s, "\n", "\r\n", &s, 0, 256);
    s1 = STRtrim(s1, " \t\r\n");
    if (0 < s1.l) {
      lay->rows++;
    }
    for (i = 0; 0 < s1.l; ++i) {
      STRsplit_str(s1, ",", " \t", &s1, 0, 256);
    }
    if (2 == lay->rows && i > lay->cols) {
      lay->cols = i;
    }
  }
  lay->id = (int*)malloc(lay->rows * lay->cols * sizeof(int));
  s = s0;
  for (j = 0; 0 < s.l;) {
    str_t s2, s1 = STRsplit_str(s, "\n", " \t\r\n", &s, 0, 256);
    if (0 < s1.l) {
      j++;
    }
    for (i = 0; 0 < s1.l && i < lay->cols; ++i) {
      s2 = STRsplit_str(s1, ",", " \t", &s1, 0, 256);
      k = -1;
      if (1 == j || 0 == i) {
        if (0 < s2.l) {
          if (LAY_HEADER_ADAPT == s2.s[0]) {
            k = -LAY_HEADER_ADAPT;
          }
          else if (LAY_HEADER_EXPAND == s2.s[0]) {
            k = -LAY_HEADER_EXPAND;
          }
          else {
            k = atoi(s2.s);
          }
        }
        else {
          k = -LAY_HEADER_ADAPT;
        }
      }
      else {
        if (0 < s2.l) {
          if (LAY_EXPAND_HORIZONTAL == s2.s[0]) {
            k = -LAY_EXPAND_HORIZONTAL;
          }
          else if (LAY_EXPAND_VERTICAL == s2.s[0]) {
            k = -LAY_EXPAND_VERTICAL;
          }
          else {
            k = FindID(s2, wid, widlen);
            if (k < 0) {
              int kk;
#define STRcmp(s1, s2) str_cmp(s1, s2, 0)
              for (kk = 0; kk < laylen; ++kk) {
                if (0 == STRcmp(s2, layname[kk])) {
                  k = kk + MAXCTRL;
                  break;
                }
              }
            }
            if (k < 0) {
              int adsf = 0;
            }
            ASSERT(k >= 0 && "未知控件");
          }
        }
        else {
          k = -1;
        }
      }
      lay->id[(j - 1)*lay->cols + i] = k;
    }
  }
  if (0) {
    for (j = 0; j < lay->rows; ++j) {
      for (i = 0; i < lay->cols; ++i) {
        printf("%3d, ", lay->id[j * lay->cols + i]);
      }
      printf("\n");
    }
  }
  GetLayoutRect(lay, wid, widlen, &lay->iniRect);
  return 1;
}
//max-min height of the row
int minHeightOfRow(const Layout* lay, int nrow, const LayoutData* wid, int widlen)
{
  int cc, height, maxheight = 0;
  for (cc = 0; cc < lay->cols; cc ++) {
    int indx = lay->id[nrow * lay->cols + cc];
    if (indx < 0) {
      continue;
    }
    if (indx >= MAXCTRL) {
      indx = widlen + indx - MAXCTRL;
    }
    // 不计算被扩展的控件
    if (nrow < lay->rows - 1 && lay->id[(nrow + 1)*lay->cols + cc] == -LAY_EXPAND_VERTICAL) {
      continue;
    }
    height = wid[indx].iniRect.bottom - wid[indx].iniRect.top;
    maxheight = MAX(maxheight, height);
  }
  return maxheight;
}
//max-min width of the column
int minWidthOfColumn(const Layout* lay, int ncol, const LayoutData* wid, int widlen)
{
  int rr, width, maxwidth = 0;
  for (rr = 0; rr < lay->rows; rr ++) {
    int indx = lay->id[rr * lay->cols + ncol];
    if (indx < 0) {
      continue;
    }
    if (indx >= MAXCTRL) {
      indx = widlen + indx - MAXCTRL;
    }
    // 不计算被扩展的控件
    if (ncol < lay->cols - 1 && lay->id[rr * lay->cols + ncol + 1] == -LAY_EXPAND_HORIZONTAL) {
      continue;
    }
    width = wid[indx].iniRect.right - wid[indx].iniRect.left;
    maxwidth = MAX(maxwidth, width);
  }
  return maxwidth;
}
#define MAXROW 100
#define MAXCOL 20
int doLayout(Layout* lay0, int curr, int toX, int toY, int totWidth, int totHeight, LayoutData* ld, int widlen, int ldlen)
{
  Layout* lay = lay0 + curr;
  int Hpos[MAXCOL] = {0}; // calculated initial horizontal position
  int Hdim[MAXCOL] = {0}; // calculated column width
  int HextraPos[MAXCOL] = {0}; // calculated on every paint
  int columnsReparto[MAXCOL] = {0}; // indexes of columns where the size need to be distributed
  int Vdim[MAXROW] = {0}; // calculated initial vertical position
  int Vpos[MAXROW] = {0}; // calculated row width
  int VextraPos[MAXROW] = {0}; // calculated on every paint
  int rowsReparto[MAXROW] = {0}; // indexes of rows where the size need to be distributed
  int rowsRepartoLen = 0;
  int columnsRepartoLen = 0;
  int fijoH; // intern precalculated not variable width
  int fijoV; // intern precalculated not variable height
  int i, ii, cc, rr, res, repartH, repartV;
  int HextraPosLen = lay->cols;
  int VextraPosLen = lay->rows;
  if (lay->hwnd) {
    ShowWindow(lay->hwnd, (totHeight > 0) ? SW_SHOW : SW_HIDE);
    MoveWindow(lay->hwnd, toX, toY, totWidth, totHeight, TRUE);
  }
  if (totWidth <= 0 || totHeight <= 0) {
    return 0;
  }
  ASSERT(lay->rows < MAXROW && lay->cols < MAXCOL);
  for (i = 0; i < lay->cols; ++i) {
    Hdim[i] = Hpos[i] = HextraPos[i] = columnsReparto[i] = 0;
  }
  for (i = 0; i < lay->rows; ++i) {
    Vdim[i] = Vpos[i] = VextraPos[i] = rowsReparto[i] = 0;
  }
  // compute Vdim
  fijoV = lay->margin.top;
  for (rr = 0; rr < lay->rows; rr ++) {
    int heaRow = lay->id[rr * lay->cols];
    int gap = (rr == 0) ? 0 : lay->Vgap;
    Vdim[rr] = (0);
    Vpos[rr] = (0);
    VextraPos[rr] = (0);
    if (heaRow == -LAY_HEADER_EXPAND) {
      rowsReparto[rowsRepartoLen++] = rr; // compute later
    }
    else if (heaRow == -LAY_HEADER_ADAPT) {
      Vdim[rr] = minHeightOfRow(lay, rr, ld, widlen); // maximum-minimum of the row
    }
    else {
      Vdim[rr] = heaRow; // indicated size
    }
    Vpos[rr] = fijoV + gap;
    fijoV += Vdim[rr];
    fijoV += gap;
  }
  fijoV += lay->margin.bottom;
  // compute Hdim
  fijoH = lay->margin.left;
  for (cc = 0; cc < lay->cols; cc ++) {
    int heaCol = lay->id[cc];
    int gap = (cc == 0) ? 0 : lay->Hgap;
    Hdim[cc] = (0);
    Hpos[cc] = (0);
    HextraPos[cc] = (0);
    if (heaCol == -LAY_HEADER_EXPAND) {
      columnsReparto[columnsRepartoLen++] = (cc); // compute later
    }
    else if (heaCol == -LAY_HEADER_ADAPT) {
      Hdim[cc] = minWidthOfColumn(lay, cc, ld, widlen); // maximum-minimum of the column
    }
    else {
      Hdim[cc] = heaCol; // indicated size
    }
    Hpos[cc] = fijoH + gap;
    fijoH += Hdim[cc];
    fijoH += gap;
  }
  fijoH += lay->margin.right;
  // finding all components in the layout array
  for (cc = 1; cc < lay->cols; cc ++) {
    for (rr = 1; rr < lay->rows; rr ++) {
      int ava, indx = lay->id[rr * lay->cols + cc];
      if (indx < 0) {
        continue;
      }
      if (indx >= MAXCTRL) {
        indx = widlen + indx - MAXCTRL;
      }
      // set position x,y
      ld[indx].indxPos.left = cc;
      ld[indx].indxPos.top = rr;
      // set position x2,y2
      ava = cc;
      while (ava + 1 < lay->cols && lay->id[rr * lay->cols + ava + 1] == -LAY_EXPAND_HORIZONTAL) {
        ava ++;
      }
      ld[indx].indxPos.right = ava;
      ava = rr;
      while (ava + 1 < lay->rows && lay->id[(ava + 1)*lay->cols + cc] == -LAY_EXPAND_VERTICAL) {
        ava ++;
      }
      ld[indx].indxPos.bottom = ava;
      ld[indx].isLaidOut = TRUE;
    }
  }
  // repartir H
  for (ii = 0; ii < HextraPosLen; ii ++) {
    HextraPos[ii] = 0;
  }
  // 对自适应X 的处理
  repartH = (totWidth - fijoH) / ((columnsRepartoLen == 0) ? 1 : columnsRepartoLen);
  for (ii = 0; ii < columnsRepartoLen; ii ++) {
    int indx = columnsReparto[ii];
    Hdim[indx] = repartH;
    for (res = indx + 1; res < lay->cols; res ++) {
      HextraPos[res] += repartH;
    }
  }
  // repartir V
  for (ii = 0; ii < VextraPosLen; ii ++) {
    VextraPos[ii] = 0;
  }
  repartV = (totHeight - fijoV) / ((rowsRepartoLen == 0) ? 1 : rowsRepartoLen);
  for (ii = 0; ii < rowsRepartoLen; ii ++) {
    int indx = rowsReparto[ii];
    Vdim[indx] = repartV;
    for (res = indx + 1; res < lay->rows; res ++) {
      VextraPos[res] += repartV;
    }
  }
  for (ii = 0; ii < ldlen; ii ++) {
    int x, y, dx, dy, mm;
    LayoutData* li = ld + ii;
    //ShowWindow(wi->hwnd, (wi->isLaidOut) ? SW_SHOW : SW_HIDE);
    if (li->inited2) {
      continue;
    }
    if (!li->isLaidOut) {
      continue;
    }
    x = Hpos[li->indxPos.left] + HextraPos[li->indxPos.left];
    y = Vpos[li->indxPos.top] + VextraPos[li->indxPos.top];
    dx = 0, dy = 0;
    for (mm = li->indxPos.left; mm <= li->indxPos.right; mm ++) {
      if (mm != li->indxPos.left) {
        dx += lay->Hgap;
      }
      dx += Hdim[mm];
    }
    for (mm = li->indxPos.top; mm <= li->indxPos.bottom; mm ++) {
      if (mm != li->indxPos.top) {
        dy += lay->Vgap;
      }
      dy += Vdim[mm];
    }
    assert(dy <= totHeight);
    assert(dx <= totWidth);
    if (x < 0 || y < 0 || dx < 0 || dy < 0) {
      continue;
    }
    //printf("%16s, %4d,%4d,%4d,%4d\n", wi->szName, x, y, dx, dy);
    //MoveWindow(wi->hwnd, x, y, dx, dy, TRUE);
    li->x = toX + x, li->y = toY + y, li->dx = dx, li->dy = dy;
    li->inited2 = 1;
    //InvalidateRect(hControl, NULL, TRUE);
  }
  for (cc = 1; cc < lay->cols; cc ++) {
    for (rr = 1; rr < lay->rows; rr ++) {
      int indx = lay->id[rr * lay->cols + cc];
      if (indx < 0) {
        continue;
      }
      if (indx >= MAXCTRL) {
        int tt = indx - MAXCTRL;
        if (ld[tt + widlen].inited == 0) {
          doLayout(lay0, tt, ld[tt + widlen].x, ld[tt + widlen].y, ld[tt + widlen].dx, ld[tt + widlen].dy, ld, widlen, ldlen);
          ld[tt + widlen].inited = 1;
        }
      }
    }
  }
  return 0;
}
typedef struct _Widgets {
  Widget* wid;
  Layout* lay;
  FontInfo* fi;
  int widlen, laylen, filen, currlay;
} Widgets;
#define mystrcmp(s1, l1, s2, l2) cstr_cmp(s1, l1, s2, l2)
int FindFont(Widgets* ws, const char* fn, int fnl)
{
  int i;
  for (i = 0; i < ws->filen; ++i) {
    int len = strlen(ws->fi[i].szName);
    if (0 == mystrcmp(ws->fi[i].szName, len, fn, fnl)) {
      return i;
    }
  }
  return -1;
}
int ReShowLayout(Widgets* ws, HWND hDlg)
{
  RECT rc;
  int ii;
  LayoutData ld[1000] = {0};
  if (NULL == ws->lay) {
    return 0;
  }
  for (ii = 0; ii < ws->widlen; ii ++) {
    ld[ii].isLaidOut = FALSE;
    ld[ii].iniRect = ws->wid[ii].iniRect;
  }
  for (ii = 0; ii < ws->laylen; ii ++) {
    ld[ws->widlen + ii].isLaidOut = FALSE;
    ld[ws->widlen + ii].iniRect = ws->lay[ii].iniRect;
  }
  GetClientRect(hDlg, &rc);
  doLayout(ws->lay, ws->currlay, 0, 0, (int)(rc.right - rc.left), (int)(rc.bottom - rc.top), ld, ws->widlen, ws->widlen + ws->laylen);
  for (ii = 0; ii < ws->widlen; ii ++) {
    ShowWindow(ws->wid[ii].hwnd, (ld[ii].isLaidOut) ? SW_SHOW : SW_HIDE);
    if (0 == stricmp(WC_COMBOBOX, ws->wid[ii].winfo->szWinClass)) {
      MoveWindow(ws->wid[ii].hwnd, ld[ii].x, ld[ii].y, ld[ii].dx, ld[ii].dy + 300, TRUE);
    }
    else {
      MoveWindow(ws->wid[ii].hwnd, ld[ii].x, ld[ii].y, ld[ii].dx, ld[ii].dy, TRUE);
    }
    if (ld[ii].dx) {
      int asdf = 0;
    }
  }
  return 0;
}
int ShowLayout(Widgets* ws, HWND hDlg, int currlay)
{
  LayoutData ld[1000] = {0};
  if (NULL == ws->lay) {
    return 0;
  }
  if (ws->currlay != currlay) {
    ws->currlay = currlay;
    ReShowLayout(ws, hDlg);
  }
  return 0;
}
int CreateFontXml(Widgets* ws, STRTREE* pmm, const char* fontsname, int fontsnamelen)
{
  int j, k = 0, elemnum = 0, elempos, el = fontsnamelen, eind;
  elempos = XmlFindElemWithAttribute(pmm, "name", 4, fontsname, el);
  assert(elempos >= 0);
  eind = pmm->node[elempos].ind;
  ++elempos;
  for (j = elempos; j < pmm->len && (pmm->node[j].ind > eind); ++j) {
    if (XML_ELEMENTNAME == pmm->node[j].type) {
      ++elemnum;
    }
  }
  ws->fi = (FontInfo*)malloc(sizeof(FontInfo) * elemnum);
  memset(ws->fi, 0, sizeof(FontInfo)*elemnum);
  for (j = elempos; j < pmm->len && (pmm->node[j].ind > eind); ++j) {
    if (XML_ELEMENTNAME == pmm->node[j].type) {
      int id = XmlFindFirstNode(pmm, j, XML_MARK_TEXT);
      if (id >= 0) {
        str_t s = pmm->node[id].name;
        int isok = CreateFontInfo(ws->fi + k, s, pmm->node[j + 2].name);
        assert(j + 2 < id && pmm->node[j + 2].type == XML_ATTRIBUTEVALUE);
        if (isok) {
          ++k;
        }
        else {
          int asdf = 0;
        }
      }
    }
  }
  ws->filen = k;
  return 0;
}
int CreateFromXml(Widgets* ws, STRTREE* pmm, const char* ctrlsname, int ctrlsnamelen, const char* laysname, int laysnamelen,
    const char* fontsname, int fontsnamelen, HWND hParent, HINSTANCE hInstance)
{
  int j, k = 0, elemnum = 0, elempos, el = ctrlsnamelen, eind, ni;
  CreateFontXml(ws, pmm, fontsname, fontsnamelen);
  elempos = XmlFindElemWithAttribute(pmm, "name", 4, ctrlsname, el);
  assert(elempos >= 0);
  eind = pmm->node[elempos].ind;
  if (elempos < 0) {
    return 0;
  }
  ++elempos;
  for (j = elempos; j < pmm->len && (pmm->node[j].ind > eind); ++j) {
    if (XML_ELEMENTNAME == pmm->node[j].type) {
      ++elemnum;
    }
  }
  ws->wid = (Widget*)malloc(sizeof(Widget) * elemnum);
  memset(ws->wid, 0, sizeof(Widget)*elemnum);
  for (j = elempos; j < pmm->len && (pmm->node[j].ind > eind); ++j) {
    if (XML_ELEMENTNAME == pmm->node[j].type) {
      int isok = CreateWidget(ws->wid + k, pmm, j, hParent, k, hInstance);
      if (isok) {
        if (ws->filen > 0) {
          int fid = -1;
          ni = XmlFindNode(pmm, j, XML_MARK_ATTRIBUTENAME, "font", 4);
          if (ni >= 0) {
            str_t s1 = STRtrim(pmm->node[ni + 1].name, "\"");
            fid = FindFont(ws, s1.s, s1.l);
          }
          if (fid < 0) {
            fid = FindFont(ws, "default", 7);
          }
          if (fid >= 0) {
            SetWindowFont(ws->wid[k].hwnd, ws->fi[fid].hFont, TRUE);
          }
        }
        GetWindowRect(ws->wid[k].hwnd, &ws->wid[k].iniRect);
        ++k;
      }
      else {
        int asdf = 0;
      }
    }
  }
  ws->widlen = k;
  //CreateLayXml(ws, pmm, laysname, laysnamelen);
  //int CreateLayXml(Widgets* ws, STRTREE* pmm, const char* laysname, int laysnamelen)
  {
    int j, k = 0, elemnum = 0, elempos, el = laysnamelen, eind;
    str_t layname[100] = {0};
    elempos = XmlFindElemWithAttribute(pmm, "name", 4, laysname, el);
    assert(elempos >= 0);
    eind = pmm->node[elempos].ind;
    ++elempos;
    for (j = elempos; j < pmm->len && (pmm->node[j].ind > eind); ++j) {
      if (XML_ELEMENTNAME == pmm->node[j].type) {
        ni = XmlFindNode(pmm, j, XML_MARK_ATTRIBUTENAME, "name", 4);
        if (ni > 0) {
          layname[elemnum] = pmm->node[ni + 1].name;
        }
        ++elemnum;
      }
    }
    ws->lay = (Layout*)malloc(sizeof(Layout) * elemnum);
    memset(ws->lay, 0, sizeof(Layout)*elemnum);
    for (j = elempos; j < pmm->len && (pmm->node[j].ind > eind); ++j) {
      if (XML_ELEMENTNAME == pmm->node[j].type) {
        ni = XmlFindFirstNode(pmm, j, XML_MARK_TEXT);
        if (ni >= 0) {
          str_t s = pmm->node[ni].name;
          int isok = CreateLayout(ws->lay + k, s, ws->wid, ws->widlen, layname, elemnum);
          if (isok) {
            ni = XmlFindNode(pmm, j, XML_MARK_ATTRIBUTENAME, "text", 4);
            if (ni >= 0) {
              str_t s1 = pmm->node[ni + 1].name;
              char szText[256];
              STRncpy(szText, 256, s1);
              ws->lay[k].hwnd = CreateWindowEx(BS_GROUPBOX, WC_BUTTON, szText, BS_GROUPBOX | WS_CHILD | WS_VISIBLE, 0, 0, 72, 512, hParent, (HMENU)0, hInstance, NULL);
              if (1) {
                int fid = -1;
                ni = XmlFindNode(pmm, j, XML_MARK_ATTRIBUTENAME, "font", 4);
                if (ni >= 0) {
                  str_t s1 = STRtrim(pmm->node[ni + 1].name, "\"");
                  fid = FindFont(ws, s1.s, s1.l);
                }
                if (fid < 0) {
                  fid = FindFont(ws, "default", 7);
                }
                if (fid >= 0) {
                  SetWindowFont(ws->lay[k].hwnd, ws->fi[fid].hFont, TRUE);
                }
              }
            }
            ++k;
          }
          else {
            int asdf = 0;
          }
        }
      }
    }
    ws->laylen = k;
  }
  return ws->wid != NULL;
}
int ReleaseWidgets(Widgets* ws)
{
  int i;
  for (i = 0; i < ws->filen; ++i) {
    if (ws->fi[i].hFont) {
      DeleteObject(ws->fi[i].hFont);
    }
  }
  for (i = 0; i < ws->laylen; ++i) {
    if (ws->lay[i].id) {
      free(ws->lay[i].id);
    }
  }
  for (i = 0; i < ws->widlen; ++i) {
    if (ws->wid[i].hwnd) {
      DestroyWindow(ws->wid[i].hwnd);
    }
  }
  free(ws->fi);
  free(ws->lay);
  free(ws->wid);
  free(ws);
  return 0;
}
/* 设置应用程序图标 */
HICON SetIcon(HWND m_hWnd, HICON hIcon, BOOL bBigIcon)
{
  ASSERT(IsWindow(m_hWnd));
  return (HICON)SendMessage(m_hWnd, WM_SETICON, bBigIcon, (LPARAM)hIcon);
}
Widgets* CreateWidgetsFromXmlFile(const char* xmlfn, HWND hParent, HINSTANCE hInstance)
{
  STRTREE mm = {0};
  STRTREE* pmm = &mm;
  int ret = 0, ni0, ni1, ni2, ni3, ni4, elempos;
  str_t ctrlsname, laysname, fontsname;
  str_t ss[1] = {0};
  Widgets* ws = NULL;
  if (!str_loadasni(xmlfn, ss)) {
    ASSERT(0 && "xml文件打开失败!");
    return ws;
  }
  SetXml(pmm, ss->s, ss->l);
  elempos = XmlFindElemWithAttribute(pmm, "name", 4, "main", 4);
  ni0 = XmlFindNode(pmm, elempos, XML_MARK_ATTRIBUTENAME, "Layouts", 7);
  ni1 = XmlFindNode(pmm, elempos, XML_MARK_ATTRIBUTENAME, "fonts", 5);
  ni2 = XmlFindNode(pmm, elempos, XML_MARK_ATTRIBUTENAME, "ctrls", 5);
  ni3 = XmlFindNode(pmm, elempos, XML_MARK_ATTRIBUTENAME, "text", 4);
  ni4 = XmlFindNode(pmm, elempos, XML_MARK_ATTRIBUTENAME, "icon", 4);
  if (ni3 >= 0) {
    char buf[256];
    STRncpy(buf, 256, pmm->node[ni3 + 1].name);
    SetWindowText(hParent, buf);
  }
  if (ni4 >= 0) {
    char buf[256];
    HICON hIcon;
    STRncpy(buf, 256, pmm->node[ni4 + 1].name);
    hIcon = (HICON)LoadImage(NULL, buf, IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    SetIcon(hParent, hIcon, 0);
  }
  else {
#define IDR_MAINFRAME 100
    HICON hIcon = (HICON)LoadIcon(GetModuleHandle(0), (LPCTSTR)IDR_MAINFRAME);
    SetIcon(hParent, hIcon, 0);
  }
  if (ni0 >= 0 && ni1 >= 0 && ni2 >= 0) {
    ws = (Widgets*)malloc(sizeof(Widgets));
    memset(ws, 0, sizeof(Widgets));
    laysname = STRtrim(pmm->node[ni0 + 1].name, "\"");
    fontsname = STRtrim(pmm->node[ni1 + 1].name, "\"");
    ctrlsname = STRtrim(pmm->node[ni2 + 1].name, "\"");
    ret = CreateFromXml(ws, pmm, ctrlsname.s, ctrlsname.l, laysname.s, laysname.l, fontsname.s, fontsname.l, hParent, hInstance);
  }
  FreeXml(&mm);
  ShowLayout(ws, hParent, 0);
  return ws;
}
/**
 // ---- EvaLayout: method for MFC to recalculate the layout
 //
 call this function after setting a new layout
 or resizing the window
*/
int FindCtrlWithName(Widgets* ws, LPCTSTR szName)
{
  int i;
  for (i = 0; i < ws->widlen; ++i) {
    if (0 == stricmp(ws->wid[i].szName, szName)) {
      return i;
    }
  }
  return -1;
}
int FindCtrlWithHwnd(Widgets* ws, HWND hwnd)
{
  int i;
  for (i = 0; i < ws->widlen; ++i) {
    if (ws->wid[i].hwnd == hwnd) {
      return i;
    }
  }
  return -1;
}
LRESULT System_TraceMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  char* str = 0;
  switch (msg) {
    //default:str="未知消息";break;
#define MSG2STR(a, b, c) case a: str=#a","c;break;
#include "ui/msg2string.txt"
#undef MSG2STR
  }
  if (str) {
    printf("%s\n", str);
  }
  else {
    int asdf = 0;
    //printf("%x\n", msg.message);
    //printf("%d\n", msg.message);
    //printf("%x\n", 0x8000);
  }
  return 0;
}
#if defined WIN64 || defined _WIN64
#define icvGetWindowLongPtr GetWindowLongPtr
#define icvSetWindowLongPtr( hwnd, id, ptr ) SetWindowLongPtr( hwnd, id, (LONG_PTR)(ptr) )
#define icvGetClassLongPtr GetClassLongPtr
#define CV_USERDATA GWLP_USERDATA
#define CV_WNDPROC GWLP_WNDPROC
#define CV_HCURSOR GCLP_HCURSOR
#define CV_HBRBACKGROUND GCLP_HBRBACKGROUND
#else
#define icvGetWindowLongPtr GetWindowLong
#define icvSetWindowLongPtr( hwnd, id, ptr ) SetWindowLong( hwnd, id, (size_t)ptr )
#define icvGetClassLongPtr GetClassLong
#define CV_USERDATA GWL_USERDATA
#define CV_WNDPROC GWL_WNDPROC
#define CV_HCURSOR GCL_HCURSOR
#define CV_HBRBACKGROUND GCL_HBRBACKGROUND
#endif
struct CvWindow;
typedef int (*CvMouseCallback)(void* param, int event, int x, int y, int flags);
typedef int (*CvPaintCallback)(void* param, canvas_t* ctx);
typedef struct CvWindow {
  int signature;
  HWND hwnd;
  char* name;
  HDC dc;
  int last_key;
  int flags;
  int status; //0 normal, 1 fullscreen (YV)
  CvMouseCallback on_mouse;
  CvPaintCallback on_paint;
  struct CvWindow* prev;
  struct CvWindow* next;
  void* userdata;
  Widgets* wis;
}
CvWindow;
static CvWindow* hg_windows = 0;
static CvWindow* focus_window = 0;
static HINSTANCE hg_hinstance = 0;
static const char* highGUIclassName = "HighGUI class";
static void icvRemoveWindow(CvWindow* window)
{
  RECT wrect = {0, 0, 0, 0};
  if (window->hwnd) {
    icvSetWindowLongPtr(window->hwnd, CV_USERDATA, 0);
  }
  if (window->prev) {
    window->prev->next = window->next;
  }
  else {
    hg_windows = window->next;
  }
  if (window->next) {
    window->next->prev = window->prev;
  }
  window->prev = window->next = 0;
  if (window->dc) {
    DeleteDC(window->dc);
  }
  cvFree(&window);
}
static CvWindow* icvWindowByHWND(HWND hwnd)
{
  CvWindow* window = (CvWindow*) icvGetWindowLongPtr(hwnd, CV_USERDATA);
  return window != 0 && hg_windows != 0 &&
      window->signature == CV_WINDOW_MAGIC_VAL ? window : 0;
}
static BOOL icvGetBitmapData(CvWindow* window, SIZE* size, int* channels, void** data)
{
  BITMAP bmp;
  HGDIOBJ h;
  GdiFlush();
  h = GetCurrentObject(window->dc, OBJ_BITMAP);
  if (size) {
    size->cx = size->cy = 0;
  }
  if (data) {
    * data = 0;
  }
  if (h == NULL) {
    return TRUE;
  }
  if (GetObject(h, sizeof(bmp), &bmp) == 0) {
    return TRUE;
  }
  if (size) {
    size->cx = abs(bmp.bmWidth);
    size->cy = abs(bmp.bmHeight);
  }
  if (channels) {
    * channels = bmp.bmBitsPixel / 8;
  }
  if (data) {
    * data = bmp.bmBits;
  }
  return FALSE;
}
static RECT icvCalcWindowRect(CvWindow* window)
{
  RECT crect;
  assert(window);
  GetClientRect(window->hwnd, &crect);
  return crect;
}
static void icvUpdateWindowPos(CvWindow* window)
{
  RECT rect;
  assert(window);
  if ((window->flags & CV_WINDOW_AUTOSIZE)) {
    SIZE size = {0, 0};
    icvGetBitmapData(window, &size, 0, 0);
  }
  rect = icvCalcWindowRect(window);
  MoveWindow(window->hwnd, rect.left, rect.top, rect.right - rect.left + 1,
      rect.bottom - rect.top + 1, TRUE);
}
static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  CvWindow* window = icvWindowByHWND(hwnd);
  if (!window) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  switch (uMsg) {
    //#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
#ifdef WM_MOUSEWHEEL
  case WM_MOUSEWHEEL:
    if (window->on_mouse) {
      int x, y;
      int flags = (wParam & MK_LBUTTON ? CV_EVENT_FLAG_LBUTTON : 0) |
          (wParam & MK_RBUTTON ? CV_EVENT_FLAG_RBUTTON : 0) |
          (wParam & MK_MBUTTON ? CV_EVENT_FLAG_MBUTTON : 0) |
          (wParam & MK_CONTROL ? CV_EVENT_FLAG_CTRLKEY : 0) |
          (wParam & MK_SHIFT ? CV_EVENT_FLAG_SHIFTKEY : 0) |
          (GetKeyState(VK_MENU) < 0 ? CV_EVENT_FLAG_ALTKEY : 0);
      int event = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? CV_EVENT_MOUSEWHEELUP : CV_EVENT_MOUSEWHEELDOWN;
      x = LOWORD(lParam);
      y = HIWORD(lParam);
      window->on_mouse(window->userdata, event, x, y, flags);
    }
    break;
#endif
  case WM_DESTROY:
    icvRemoveWindow(window);
    // Do nothing!!!
    //PostQuitMessage(0);
    break;
  case WM_GETMINMAXINFO:
    if (!(window->flags & CV_WINDOW_AUTOSIZE)) {
      MINMAXINFO* minmax = (MINMAXINFO*) lParam;
      LRESULT retval = DefWindowProc(hwnd, uMsg, wParam, lParam);
      minmax->ptMinTrackSize.y = 100;
      minmax->ptMinTrackSize.x = 100;
      return retval;
    }
    break;
  case WM_WINDOWPOSCHANGED: {
    WINDOWPOS* pos = (WINDOWPOS*) lParam;
    if (!(window->flags & CV_WINDOW_AUTOSIZE)) {
      icvUpdateWindowPos(window);
    }
    break;
  }
  case WM_ACTIVATE:
    if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
      SetFocus(window->hwnd);
    }
    break;
  case WM_SETFOCUS: {
    focus_window = window;
  }
  break;
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
    if (window->on_mouse) {
      POINT pt;
      int flags = (wParam & MK_LBUTTON ? CV_EVENT_FLAG_LBUTTON : 0) |
          (wParam & MK_RBUTTON ? CV_EVENT_FLAG_RBUTTON : 0) |
          (wParam & MK_MBUTTON ? CV_EVENT_FLAG_MBUTTON : 0) |
          (wParam & MK_CONTROL ? CV_EVENT_FLAG_CTRLKEY : 0) |
          (wParam & MK_SHIFT ? CV_EVENT_FLAG_SHIFTKEY : 0) |
          (GetKeyState(VK_MENU) < 0 ? CV_EVENT_FLAG_ALTKEY : 0);
      int event = uMsg == WM_LBUTTONDOWN ? CV_EVENT_LBUTTONDOWN :
          uMsg == WM_RBUTTONDOWN ? CV_EVENT_RBUTTONDOWN :
          uMsg == WM_MBUTTONDOWN ? CV_EVENT_MBUTTONDOWN :
          uMsg == WM_LBUTTONUP ? CV_EVENT_LBUTTONUP :
          uMsg == WM_RBUTTONUP ? CV_EVENT_RBUTTONUP :
          uMsg == WM_MBUTTONUP ? CV_EVENT_MBUTTONUP :
          uMsg == WM_LBUTTONDBLCLK ? CV_EVENT_LBUTTONDBLCLK :
          uMsg == WM_RBUTTONDBLCLK ? CV_EVENT_RBUTTONDBLCLK :
          uMsg == WM_MBUTTONDBLCLK ? CV_EVENT_MBUTTONDBLCLK :
          CV_EVENT_MOUSEMOVE;
      if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN) {
        SetCapture(hwnd);
      }
      if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP) {
        ReleaseCapture();
      }
      pt.x = LOWORD(lParam);
      pt.y = HIWORD(lParam);
      window->on_mouse(window->userdata, event, pt.x, pt.y, flags);
    }
    break;
  case WM_SIZE:
    if (window->wis) {
      ReShowLayout(window->wis, hwnd);
    }
    return 0;
  case WM_PAINT:
    if (window->on_paint) {
      PAINTSTRUCT paint = {0};
      HDC hDC = BeginPaint(hwnd, &paint);
      canvas_t s[1] = {0};
      CANVAS_SETHDC(s, hDC);
      window->on_paint(window->userdata, s);
      canvas_release(s);
      EndPaint(hwnd, &paint);
    }
    else {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_SETCURSOR:
    //SetCursor( ( HCURSOR ) icvGetClassLongPtr( hwnd, CV_HCURSOR ) );
    break;
  case WM_KEYDOWN:
    window->last_key = (int) wParam;
    return 0;
  case WM_COMMAND: {
    //WORD cmd = LOWORD(wParam);
    //printf(TEXT("%-16s%04X-%04X %04X-%04X %s\n"), TEXT("WM_COMMAND"), HIWORD(wParam), LOWORD(wParam), HIWORD(lParam), LOWORD(lParam));
    if (window && window->wis && window->wis->wid && (int)wParam < window->wis->widlen && window->wis->wid[wParam].on_click) {
      window->wis->wid[wParam].on_click(window->wis->wid + wParam, lParam, window->userdata);
    }
  }
  break;
  case WM_NOTIFY: {
    if (window && window->wis && window->wis->wid && (int)wParam < window->wis->widlen && window->wis->wid[wParam].on_notify) {
      window->wis->wid[wParam].on_notify(window->wis->wid + wParam, lParam, window->userdata);
    }
  }
  break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
void
cvDestroyAllWindows(void)
{
  CvWindow* window = hg_windows;
  while (window) {
    HWND hwnd = window->hwnd;
    window = window->next;
    SendMessage(hwnd, WM_CLOSE, 0, 0);
  }
}
static void icvCleanupHighgui(void)
{
  cvDestroyAllWindows();
  UnregisterClass(highGUIclassName, hg_hinstance);
}
int cvInitSystem(int a, char** b)
{
  static int wasInitialized = 0;
  // check initialization status
  if (!wasInitialized) {
    WNDCLASSEX wndc;
    // Initialize the stogare
    hg_windows = 0;
    // Register the class
    wndc.cbSize = sizeof(WNDCLASSEX);
    wndc.style = CS_HREDRAW | CS_VREDRAW;
    //wndc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wndc.cbClsExtra = 0;
    wndc.cbWndExtra = 0;
    wndc.hInstance = hg_hinstance;
    wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wndc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wndc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wndc.lpszMenuName = NULL;
    wndc.lpszClassName = highGUIclassName;
    wndc.lpszMenuName = highGUIclassName;
    wndc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndc.lpfnWndProc = MainWindowProc;
    if (!RegisterClassEx(&wndc)) {
      MessageBox(NULL, "Class Registration Failed!", "Error", 0);
      //exit the application
      return 0;
    }
    atexit(icvCleanupHighgui);
    wasInitialized = 1;
  }
  return 0;
}
static CvWindow* icvFindWindowByName(const char* name)
{
  CvWindow* window = hg_windows;
  for (; window != 0 && strcmp(name, window->name) != 0; window = window->next)
    ;
  return window;
}
int cvNamedWindow(const char* name, int flags)
{
  int result = 0;
  HWND hWnd;
  CvWindow* window;
  DWORD defStyle;
  int len;
  int x, y, w, h;
  defStyle = WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
  cvInitSystem(0, 0);
  // Check the name in the storage
  if (icvFindWindowByName(name) != 0) {
    result = 1;
    return 0;
  }
  //if ( !( flags & CV_WINDOW_AUTOSIZE ) ) //YV add border in order to resize the window
  defStyle |= WS_SIZEBOX;
  //icvLoadWindowPos( name, &rect );
  x = 0;
  y = 0;
  h = 600;
  w = 800;
  //YV- remove one border by changing the style
  hWnd = CreateWindow(highGUIclassName, name, defStyle | WS_OVERLAPPED,
      x, y, w, h, 0, 0, hg_hinstance, 0);
  if (!hWnd) {
    return 0;
  }
  ShowWindow(hWnd, SW_SHOW);
  len = (int) strlen(name);
  CV_CALL(window = (CvWindow*) cvAlloc(sizeof(CvWindow) + len + 1));
  memset(window, 0, sizeof(CvWindow) + len + 1);
  window->signature = CV_WINDOW_MAGIC_VAL;
  window->hwnd = hWnd;
  window->name = (char*)(window + 1);
  memcpy(window->name, name, len + 1);
  window->flags = flags;
  window->dc = CreateCompatibleDC(0);
  window->last_key = 0;
  window->status = CV_WINDOW_NORMAL; //YV
  window->on_mouse = 0;
  window->userdata = 0;
  window->next = hg_windows;
  window->prev = 0;
  if (hg_windows) {
    hg_windows->prev = window;
  }
  hg_windows = window;
  icvSetWindowLongPtr(hWnd, CV_USERDATA, window);
  // Recalculate window position
  icvUpdateWindowPos(window);
  result = 1;
  return result;
}
int cvWaitKey(int delay)
{
  int time0 = GetTickCount();
  if (NULL == hg_windows) {
    return -2;
  }
  for (;;) {
    CvWindow* window = NULL;
    MSG message;
    int is_processed = 0;
    if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay) || hg_windows == 0) {
      return -1;
    }
    if (delay <= 0) {
      GetMessage(&message, 0, 0, 0);
    }
    else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
      Sleep(1);
      continue;
    }
    for (window = hg_windows; window != 0 && is_processed == 0; window = window->next) {
      if (window->hwnd == message.hwnd) {
        is_processed = 1;
        switch (message.message) {
        case WM_DESTROY:
        case WM_CHAR:
          DispatchMessage(&message);
          return (int) message.wParam;
        case WM_SYSKEYDOWN:
          if (message.wParam == VK_F10) {
            is_processed = 1;
            return (int)(message.wParam << 16);
          }
          break;
        case WM_KEYDOWN:
          TranslateMessage(&message);
          if ((message.wParam >= VK_F1 && message.wParam <= VK_F24) ||
              message.wParam == VK_HOME || message.wParam == VK_END ||
              message.wParam == VK_UP || message.wParam == VK_DOWN ||
              message.wParam == VK_LEFT || message.wParam == VK_RIGHT ||
              message.wParam == VK_INSERT || message.wParam == VK_DELETE ||
              message.wParam == VK_PRIOR || message.wParam == VK_NEXT) {
            DispatchMessage(&message);
            is_processed = 1;
            return (int)(message.wParam << 16);
          }
        default:
          DispatchMessage(&message);
          is_processed = 1;
          break;
        }
      }
    }
    if (!is_processed) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }
  return -1;
}
int cvSetWidgetsXml(const char* name, const char* xmlfn)
{
  CvWindow* window;
  window = icvFindWindowByName(name);
  if (window) {
    if (window->wis) {
      ReleaseWidgets(window->wis);
      window->wis = 0;
    }
    window->wis = CreateWidgetsFromXmlFile(xmlfn, window->hwnd, hg_hinstance);
    ReShowLayout(window->wis, window->hwnd);
  }
  return 0;
}
int cvShowLayout(const char* name, int layid)
{
  CvWindow* window = icvFindWindowByName(name);
  if (window) {
    ShowLayout(window->wis, window->hwnd, layid);
  }
  return 0;
}
int cvReShowLayout(const char* name)
{
  CvWindow* window = icvFindWindowByName(name);
  if (window) {
    ReShowLayout(window->wis, window->hwnd);
  }
  return 0;
}
Widget* cvFindCtrlWithName(const char* name, const char* ctrlname)
{
  Widget* wid = 0;
  CvWindow* window = icvFindWindowByName(name);
  if (window && window->wis) {
    int id = FindCtrlWithName(window->wis, ctrlname);
    if (id >= 0) {
      wid = window->wis->wid + id;
    }
  }
  return wid;
}
int cvGetWindowText(const char* name, const char* ctrlname, char* buf, int len)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    GetWindowText(wid->hwnd, buf, len);
    return 1;
  }
  return 0;
}
double cvGetWindowReal(const char* name, const char* ctrlname, double def)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    char buf[256];
    GetWindowText(wid->hwnd, buf, 256);
    return atof(buf);
  }
  return def;
}
int cvGetComboBoxSel(const char* name, const char* ctrlname)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    return ComboBox_GetCurSel(wid->hwnd);
  }
  return 0;
}
int cvGetCtrlState(const char* name, const char* ctrlname)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    return SendMessage(wid->hwnd, BM_GETSTATE, 0, 0);
  }
  return 0;
}
int cvGetWindowRect(const char* name, const char* ctrlname, RECT* pRect)
{
  if (name && ctrlname) {
    Widget* wid = cvFindCtrlWithName(name, ctrlname);
    if (ctrlname && wid) {
      GetWindowRect(wid->hwnd, pRect);
      return 1;
    }
  }
  if (name) {
    CvWindow* window = icvFindWindowByName(name);
    GetWindowRect(window->hwnd, pRect);
    return 1;
  }
  return 0;
}
int cvGetClientRect(const char* name, const char* ctrlname, RECT* pRect)
{
  if (name && ctrlname) {
    Widget* wid = cvFindCtrlWithName(name, ctrlname);
    if (ctrlname && wid) {
      GetClientRect(wid->hwnd, pRect);
      return 1;
    }
  }
  if (name) {
    CvWindow* window = icvFindWindowByName(name);
    GetClientRect(window->hwnd, pRect);
    return 1;
  }
  return 0;
}
int cvSetWindowText(const char* name, const char* ctrlname, const char* buf)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    SetWindowText(wid->hwnd, buf);
    return 0;
  }
  return 0;
}
int cvSetWindowPos(const char* name, int x, int y, int cx, int cy)
{
  CvWindow* window = icvFindWindowByName(name);
  if (window) {
    SetWindowPos(window->hwnd, 0, x, y, cx, cy, 0);
    return 0;
  }
  return 0;
}
int cvSetStatusBarText(const char* name, const char* ctrlname, int i, const char* buf)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    SendMessage(wid->hwnd, SB_SETTEXT, (LPARAM)(i), (WPARAM)buf);
    return 0;
  }
  return 0;
}
int cvGetListViewItemCount(const char* name, const char* ctrlname)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    int nItem = ListView_GetItemCount(wid->hwnd);
    return nItem;
  }
  return 0;
}
int cvGetListViewItemText(const char* name, const char* ctrlname, int row, int col, char* buf, int buflen)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    ListView_GetItemText(wid->hwnd, row, col, buf, buflen);
    return 0;
  }
  return 0;
}
int cvListView_EnsureVisible(const char* name, const char* ctrlname, int nItem, BOOL bPartialOK)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    return SendMessage(wid->hwnd, LVM_ENSUREVISIBLE, nItem, MAKELPARAM(bPartialOK, 0));
  }
  return 0;
}
int cvListView_SetColumnWidth(const char* name, const char* ctrlname, int iCol, int cx)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    return ListView_SetColumnWidth(wid->hwnd, iCol, cx);
  }
  return 0;
}
int cvListView_DeleteAllItems(const char* name, const char* ctrlname)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    return ListView_DeleteAllItems(wid->hwnd);
  }
  return 0;
}
int cvSetListViewItemText(const char* name, const char* ctrlname, int row, int col, const char* text)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    int nItem = ListView_GetItemCount(wid->hwnd);
    char buf[256];
    if (nItem == row) {
      LV_ITEM item = {0}; // 项
      item.mask = LVIF_TEXT; // 文字
      item.cchTextMax = MAX_PATH; // 文字长度
      item.iItem = 0;
      item.iSubItem = 0;
      item.iItem = row;
      item.pszText = "";
      ListView_InsertItem(wid->hwnd, (LPARAM)&item);
    }
    ListView_GetItemText(wid->hwnd, row, col, buf, 256);
    if (0 != strcmp(buf, text)) {
      ListView_SetItemText(wid->hwnd, row, col, (char*)text);
    }
    return 0;
  }
  return 0;
}
int cvSetListViewDeleteItem(const char* name, const char* ctrlname, int row)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    int nItem = ListView_GetItemCount(wid->hwnd);
    if (nItem > row) {
      ListView_DeleteItem(wid->hwnd, row);
    }
    return 0;
  }
  return 0;
}
int cvInvalidateRect(const char* name, const char* ctrlname)
{
  if (ctrlname) {
    Widget* wid = cvFindCtrlWithName(name, ctrlname);
    if (wid && wid->hwnd) {
      InvalidateRect(wid->hwnd, 0, 0);
    }
  }
  else {
    CvWindow* window = icvFindWindowByName(name);
    InvalidateRect(window->hwnd, 0, TRUE);
  }
  return 0;
}
int cvSetImage(const char* name, const char* ctrlname, int h, int w, const unsigned char* img, int al, int ai, int isflip)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid && wid->hwnd) {
    if (img) {
      HWND hwnd = wid->hwnd;
      HDC hdc = GetDC(hwnd);
      HDC memDC = CreateCompatibleDC(hdc);
      HBITMAP bmp;
      RECT rc, rc2 = {0};
      int h2, w2;
      GetClientRect(hwnd, &rc);
      w2 = rc.right - rc.left, h2 = rc.bottom - rc.top;
      bmp = CreateCompatibleBitmap(hdc, w2, h2);
      SelectObject(memDC, bmp);
      rc2.right = w2, rc2.bottom = h2;
      FillRect(memDC, &rc, CreateSolidBrush(GetSysColor(COLOR_BTNFACE)));
      ImgDrawOnDC(memDC, h, w, img, al, ai, rc.top, rc.left, rc.bottom - rc.top, rc.right - rc.left, 1, isflip);
      BitBlt(hdc, 0, 0, w2, h2, memDC, 0, 0, SRCCOPY);
      DeleteDC(memDC);
      DeleteObject(bmp);
      ReleaseDC(hwnd, hdc);
    }
    else {
      InvalidateRect(wid->hwnd, 0, 0);
    }
  }
  return 0;
}
int cvSetIm(const char* name, const char* ctrlname, const img_t* im, int isflip)
{
  return cvSetImage(name, ctrlname, im->h, im->w, im->tt.data, im->s, im->c, isflip);
}
int cvSetOnClick(const char* name, const char* ctrlname, on_click_t on_click)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    wid->on_click = on_click;
  }
  return 0;
}
int cvSetOnNotify(const char* name, const char* ctrlname, on_click_t on_notify)
{
  Widget* wid = cvFindCtrlWithName(name, ctrlname);
  if (wid) {
    wid->on_notify = on_notify;
  }
  return 0;
}
void cvSetCallback(const char* window_name, CvMouseCallback on_mouse, CvPaintCallback on_paint, void* param)
{
  CvWindow* window = 0;
  CV_FUNCNAME("cvSetMouseCallback");
  __BEGIN__;
  if (!window_name) {
    CV_ERROR(CV_StsNullPtr, "NULL window name");
  }
  window = icvFindWindowByName(window_name);
  if (!window) {
    EXIT;
  }
  window->on_paint = on_paint;
  window->on_mouse = on_mouse;
  window->userdata = param;
  __END__;
}
int test_wincv_onbtncap(Widget* wid, int xx, void* tt)
{
  printf("test_wincv_onbtncap\n");
  return 0;
}
int test_wincv_onbtnstop(Widget* wid, int xx, void* tt)
{
  printf("test_wincv_onbtnstop\n");
  return 0;
}
int cvGetOpenFileName(const char* window_name, const TCHAR* szFilter, TCHAR* szFile, int szFileLen)
{
  CvWindow* window = 0;
  OPENFILENAME ofn = {0}; // 公共对话框结构。
  window = icvFindWindowByName(window_name);
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = window->hwnd;
  ofn.lpstrFile = szFile;
  ofn.lpstrFile[ 0 ] = _T('\0');
  ofn.nMaxFile = szFileLen;
  ofn.lpstrFilter = szFilter ? szFilter : _T("All\0*.*\0图片\0*.bmp\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  // 显示打开选择文件对话框。
  if (GetOpenFileName(&ofn)) {
    //显示选择的文件。
    //OutputDebugString(szFile);
    //OutputDebugString(_T("\r\n"));
  }
  return 0;
}
int test_wincv()
{
  int ch;
  const char* winname = "asdfasdf";
  int h = 600, w = 800;
  img_t im[1] = {0};
  //unsigned char buf[_H*_W*3]={0};
  //memset(buf, 111, 20*100*3);
  imsetsize(im, h, w, 3, 1);
  _chdir("E:/code/cstd/include/cvui");
  cvNamedWindow(winname, 1);
  cvSetWidgetsXml(winname, "test001.xml");
  cvSetOnClick(winname, "btncap", test_wincv_onbtncap);
  cvSetOnClick(winname, "btnstop", test_wincv_onbtnstop);
  for (;;) {
    ch = cvWaitKey(-1);
    if (-2 == ch) {
      break;
    }
    if ('a' == ch) {
      cvSetImage(winname, "image1", w, w, im->tt.data, im->s, im->c, 0);
    }
    //cvSetWidgetsXml("asdf", "test001.xml");
    //cvSetImage(winname, "image1", _W, _W, buf, _W*3, 3, 0);
    //printf("%d\n", ch);
  }
  imfree(im);
  return 0;
}
#endif // _WINCV_INL_

