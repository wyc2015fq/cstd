
#ifndef _WIN32DEF_H_
#define _WIN32DEF_H_

#define ARCH "win32"

#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#pragma warning(disable : 4996)
#pragma warning(disable : 4819)
#pragma warning(disable : 4018)

// kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "kernel32.lib")
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#if 1
#define _CRT_SECURE_NO_WARNINGS
#undef WINVER
#undef _WIN32_WINNT
#define WINVER  0x0500 // this is needed for UpdateLayeredWindow
#define _WIN32_WINNT 0x0501 // this is needed for the WS_EX_LAYERED
#undef _WIN32_IE
#define _WIN32_IE 0x0501
#include <windows.h>
//#undef WINVER
//#define WINVER 0x0500 // this is needed for UpdateLayeredWindow
//#define WM_MOUSEWHEEL                   0x020A
//#include <windows.h>
#include <commctrl.h>
#include <winuser.h>
#else

typedef long LONG;
typedef char CHAR;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef unsigned long ulong;
typedef int BOOL;
//#include "img/imgopt.inl"
typedef void* HANDLE;
typedef void* HWND;
typedef void* HDC;
typedef void* HBITMAP;
typedef void* HICON;
typedef void* HGLOBAL;
typedef void* HGDIOBJ;
typedef void* HMODULE;
typedef void* LPVOID;
typedef void* HMETAFILE;
typedef void* HPALETTE;
typedef void* HINSTANCE;
typedef void* HRGN;
typedef void* DWORD_PTR;
typedef ulong COLORREF;
typedef const char* LPCSTR;
#define VOID void
typedef ulong WPARAM;
typedef ulong LPARAM;
typedef long LRESULT;
typedef struct {int x, y;} POINT;
typedef struct {int l, t, r, b;} RECT;
#define CALLBACK    __stdcall
#define CURSOR_SHOWING     0x00000001
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef struct tagRGBQUAD {
  BYTE    rgbBlue;
  BYTE    rgbGreen;
  BYTE    rgbRed;
  BYTE    rgbReserved;
} RGBQUAD;
typedef struct tagBITMAP
{
  LONG        bmType;
  LONG        bmWidth;
  LONG        bmHeight;
  LONG        bmWidthBytes;
  WORD        bmPlanes;
  WORD        bmBitsPixel;
  LPVOID      bmBits;
} BITMAP;

typedef struct tagBITMAPINFOHEADER{
  DWORD      biSize;
  LONG       biWidth;
  LONG       biHeight;
  WORD       biPlanes;
  WORD       biBitCount;
  DWORD      biCompression;
  DWORD      biSizeImage;
  LONG       biXPelsPerMeter;
  LONG       biYPelsPerMeter;
  DWORD      biClrUsed;
  DWORD      biClrImportant;
} BITMAPINFOHEADER;
typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER    bmiHeader;
  RGBQUAD             bmiColors[1];
} BITMAPINFO;

typedef long FXPT16DOT16;
typedef long FXPT2DOT30;
typedef struct tagCIEXYZ
{
  FXPT2DOT30 ciexyzX;
  FXPT2DOT30 ciexyzY;
  FXPT2DOT30 ciexyzZ;
} CIEXYZ;
typedef struct tagICEXYZTRIPLE
{
  CIEXYZ  ciexyzRed;
  CIEXYZ  ciexyzGreen;
  CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

typedef struct {
  DWORD        bV4Size;
  LONG         bV4Width;
  LONG         bV4Height;
  WORD         bV4Planes;
  WORD         bV4BitCount;
  DWORD        bV4V4Compression;
  DWORD        bV4SizeImage;
  LONG         bV4XPelsPerMeter;
  LONG         bV4YPelsPerMeter;
  DWORD        bV4ClrUsed;
  DWORD        bV4ClrImportant;
  DWORD        bV4RedMask;
  DWORD        bV4GreenMask;
  DWORD        bV4BlueMask;
  DWORD        bV4AlphaMask;
  DWORD        bV4CSType;
  CIEXYZTRIPLE bV4Endpoints;
  DWORD        bV4GammaRed;
  DWORD        bV4GammaGreen;
  DWORD        bV4GammaBlue;
} BITMAPV4HEADER;

typedef struct tagPALETTEENTRY {
  BYTE        peRed;
  BYTE        peGreen;
  BYTE        peBlue;
  BYTE        peFlags;
} PALETTEENTRY;

/* Logical Palette */
typedef struct tagLOGPALETTE {
  WORD        palVersion;
  WORD        palNumEntries;
  PALETTEENTRY        palPalEntry[1];
} LOGPALETTE;

typedef struct _ICONINFO {
  BOOL    fIcon;
  DWORD   xHotspot;
  DWORD   yHotspot;
  HBITMAP hbmMask;
  HBITMAP hbmColor;
} ICONINFO;

#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_FORCEMINIMIZE    11
#define SW_MAX              11

#define SMTO_NORMAL         0x0000
#define SMTO_BLOCK          0x0001
#define SMTO_ABORTIFHUNG    0x0002
#define SMTO_NOTIMEOUTIFNOTHUNG 0x0008


#define WINAPI  __stdcall
#define CONST const
COLORREF SetBkColor(HDC hdc, COLORREF color);
BOOL ExtTextOutA(HDC hdc, int X, int Y, UINT fuOptions, const RECT *lprc, LPCSTR lpString, UINT cbCount, const int *lpDx);
#define ETO_OPAQUE 2
int GetObject(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject);
LONG GetBitmapBits(HBITMAP hbmp, LONG cbBuffer, LPVOID lpvBits);
int WINAPI GetSystemMetrics( int nIndex);
HWND WINAPI GetCapture(VOID);
HWND WINAPI SetCapture(HWND hWnd);
BOOL WINAPI ReleaseCapture(VOID);
BOOL WINAPI ClientToScreen(HWND hWnd, POINT* lpPoint);
BOOL WINAPI ShowWindow(HWND hWnd, int nCmdShow);
LRESULT WINAPI SendMessageTimeoutA(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam,UINT fuFlags,UINT uTimeout,DWORD* lpdwResult);
typedef struct _POINTL {
  LONG  x, y;
} POINTL;
#define CCHDEVICENAME 32
#define CCHFORMNAME 32
typedef struct _devicemodeA {
  BYTE   dmDeviceName[CCHDEVICENAME];
  WORD dmSpecVersion;
  WORD dmDriverVersion;
  WORD dmSize;
  WORD dmDriverExtra;
  DWORD dmFields;
  union {
    struct {
      short dmOrientation;
      short dmPaperSize;
      short dmPaperLength;
      short dmPaperWidth;
    };
    POINTL dmPosition;
  };
  short dmScale;
  short dmCopies;
  short dmDefaultSource;
  short dmPrintQuality;
  short dmColor;
  short dmDuplex;
  short dmYResolution;
  short dmTTOption;
  short dmCollate;
  BYTE   dmFormName[CCHFORMNAME];
  WORD   dmLogPixels;
  DWORD  dmBitsPerPel;
  DWORD  dmPelsWidth;
  DWORD  dmPelsHeight;
  DWORD  dmDisplayFlags;
  DWORD  dmDisplayFrequency;
#if(WINVER >= 0x0400)
  DWORD  dmICMMethod;
  DWORD  dmICMIntent;
  DWORD  dmMediaType;
  DWORD  dmDitherType;
  DWORD  dmReserved1;
  DWORD  dmReserved2;
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
  DWORD  dmPanningWidth;
  DWORD  dmPanningHeight;
#endif
#endif /* WINVER >= 0x0400 */
} DEVMODEA;

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define MK_LBUTTON          0x0001
#define MK_RBUTTON          0x0002
#define MK_SHIFT            0x0004
#define MK_CONTROL          0x0008
#define MK_MBUTTON          0x0010

typedef struct tagPAINTSTRUCT {
 HDC hdc;
 BOOL fErase;
 RECT rcPaint;
 BOOL fRestore;
 BOOL fIncUpdate;
 BYTE rgbReserved[32];
} PAINTSTRUCT;
HDC WINAPI BeginPaint(HWND hWnd, PAINTSTRUCT* lpPaint);
HDC WINAPI EndPaint(HWND hWnd, PAINTSTRUCT* lpPaint);
UINT WINAPI MapVirtualKeyA( UINT uCode, UINT uMapType);
BOOL WINAPI GetKeyboardState( BYTE* lpKeyState);
typedef void* HKL;
int WINAPI ToAsciiEx(UINT uVirtKey,UINT uScanCode, BYTE* lpKeyState, WORD* lpChar,UINT uFlags,HKL dwhkl);
#define MB_PRECOMPOSED            0x00000001  // use precomposed chars
#define MB_COMPOSITE              0x00000002  // use composite chars
#define MB_USEGLYPHCHARS          0x00000004  // use glyph chars, not ctrl chars
#define MB_ERR_INVALID_CHARS      0x00000008  // error for invalid chars

#define WC_COMPOSITECHECK         0x00000200  // convert composite to precomposed
#define WC_DISCARDNS              0x00000010  // discard non-spacing chars
#define WC_SEPCHARS               0x00000020  // generate separate chars
#define WC_DEFAULTCHAR            0x00000040  // replace w/ default char

int WINAPI MultiByteToWideChar( UINT CodePage, DWORD dwFlags, const char* lpMultiByteStr, int cbMultiByte, WORD* lpWideCharStr, int cchWideChar);
LRESULT CALLBACK DefWindowProcA(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
VOID WINAPI PostQuitMessage(int nExitCode);

/*
 * System Menu Command Values
 */
#define SC_SIZE         0xF000
#define SC_MOVE         0xF010
#define SC_MINIMIZE     0xF020
#define SC_MAXIMIZE     0xF030
#define SC_NEXTWINDOW   0xF040
#define SC_PREVWINDOW   0xF050
#define SC_CLOSE        0xF060
#define SC_VSCROLL      0xF070
#define SC_HSCROLL      0xF080
#define SC_MOUSEMENU    0xF090
#define SC_KEYMENU      0xF100
#define SC_ARRANGE      0xF110
#define SC_RESTORE      0xF120
#define SC_TASKLIST     0xF130
#define SC_SCREENSAVE   0xF140
#define SC_HOTKEY       0xF150
#define SC_DEFAULT      0xF160
#define SC_MONITORPOWER 0xF170
#define SC_CONTEXTHELP  0xF180
#define SC_SEPARATOR    0xF00F


#define CS_VREDRAW          0x0001
#define CS_HREDRAW          0x0002
#define CS_DBLCLKS          0x0008
#define CS_OWNDC            0x0020
#define CS_CLASSDC          0x0040
#define CS_PARENTDC         0x0080
#define CS_NOCLOSE          0x0200
#define CS_SAVEBITS         0x0800
#define CS_BYTEALIGNCLIENT  0x1000
#define CS_BYTEALIGNWINDOW  0x2000
#define CS_GLOBALCLASS      0x4000

#define CS_IME              0x00010000

#define DM_ORIENTATION      0x00000001L
#define DM_PAPERSIZE        0x00000002L
#define DM_PAPERLENGTH      0x00000004L
#define DM_PAPERWIDTH       0x00000008L
#define DM_SCALE            0x00000010L

#define DM_POSITION         0x00000020L

#define DM_COPIES           0x00000100L
#define DM_DEFAULTSOURCE    0x00000200L
#define DM_PRINTQUALITY     0x00000400L
#define DM_COLOR            0x00000800L
#define DM_DUPLEX           0x00001000L
#define DM_YRESOLUTION      0x00002000L
#define DM_TTOPTION         0x00004000L
#define DM_COLLATE          0x00008000L
#define DM_FORMNAME         0x00010000L
#define DM_LOGPIXELS        0x00020000L
#define DM_BITSPERPEL       0x00040000L
#define DM_PELSWIDTH        0x00080000L
#define DM_PELSHEIGHT       0x00100000L
#define DM_DISPLAYFLAGS     0x00200000L
#define DM_DISPLAYFREQUENCY 0x00400000L

#define DM_ICMMETHOD        0x00800000L
#define DM_ICMINTENT        0x01000000L
#define DM_MEDIATYPE        0x02000000L
#define DM_DITHERTYPE       0x04000000L
#define DM_PANNINGWIDTH     0x08000000L
#define DM_PANNINGHEIGHT    0x10000000L

#define IMAGE_BITMAP        0
#define IMAGE_ICON          1
#define IMAGE_CURSOR        2
#define IMAGE_ENHMETAFILE   3

#define LR_DEFAULTCOLOR     0x0000
#define LR_MONOCHROME       0x0001
#define LR_COLOR            0x0002
#define LR_COPYRETURNORG    0x0004
#define LR_COPYDELETEORG    0x0008
#define LR_LOADFROMFILE     0x0010
#define LR_LOADTRANSPARENT  0x0020
#define LR_DEFAULTSIZE      0x0040
#define LR_VGACOLOR         0x0080
#define LR_LOADMAP3DCOLORS  0x1000
#define LR_CREATEDIBSECTION 0x2000
#define LR_COPYFROMRESOURCE 0x4000
#define LR_SHARED           0x8000


#define CTLCOLOR_MSGBOX         0
#define CTLCOLOR_EDIT           1
#define CTLCOLOR_LISTBOX        2
#define CTLCOLOR_BTN            3
#define CTLCOLOR_DLG            4
#define CTLCOLOR_SCROLLBAR      5
#define CTLCOLOR_STATIC         6
#define CTLCOLOR_MAX            7

#define COLOR_SCROLLBAR         0
#define COLOR_BACKGROUND        1
#define COLOR_ACTIVECAPTION     2
#define COLOR_INACTIVECAPTION   3
#define COLOR_MENU              4
#define COLOR_WINDOW            5
#define COLOR_WINDOWFRAME       6
#define COLOR_MENUTEXT          7
#define COLOR_WINDOWTEXT        8
#define COLOR_CAPTIONTEXT       9
#define COLOR_ACTIVEBORDER      10
#define COLOR_INACTIVEBORDER    11
#define COLOR_APPWORKSPACE      12
#define COLOR_HIGHLIGHT         13
#define COLOR_HIGHLIGHTTEXT     14
#define COLOR_BTNFACE           15
#define COLOR_BTNSHADOW         16
#define COLOR_GRAYTEXT          17
#define COLOR_BTNTEXT           18
#define COLOR_INACTIVECAPTIONTEXT 19
#define COLOR_BTNHIGHLIGHT      20

#define COLOR_3DDKSHADOW        21
#define COLOR_3DLIGHT           22
#define COLOR_INFOTEXT          23
#define COLOR_INFOBK            24

#define COLOR_HOTLIGHT                  26
#define COLOR_GRADIENTACTIVECAPTION     27
#define COLOR_GRADIENTINACTIVECAPTION   28

#define COLOR_DESKTOP           COLOR_BACKGROUND
#define COLOR_3DFACE            COLOR_BTNFACE
#define COLOR_3DSHADOW          COLOR_BTNSHADOW
#define COLOR_3DHIGHLIGHT       COLOR_BTNHIGHLIGHT
#define COLOR_3DHILIGHT         COLOR_BTNHIGHLIGHT
#define COLOR_BTNHILIGHT        COLOR_BTNHIGHLIGHT


#define HCF_HIGHCONTRASTON  0x00000001
#define HCF_AVAILABLE       0x00000002
#define HCF_HOTKEYACTIVE    0x00000004
#define HCF_CONFIRMHOTKEY   0x00000008
#define HCF_HOTKEYSOUND     0x00000010
#define HCF_INDICATOR       0x00000020
#define HCF_HOTKEYAVAILABLE 0x00000040

/* Flags for ChangeDisplaySettings */
#define CDS_UPDATEREGISTRY  0x00000001
#define CDS_TEST            0x00000002
#define CDS_FULLSCREEN      0x00000004
#define CDS_GLOBAL          0x00000008
#define CDS_SET_PRIMARY     0x00000010
#define CDS_RESET           0x40000000
#define CDS_SETRECT         0x20000000
#define CDS_NORESET         0x10000000

/* Return values for ChangeDisplaySettings */
#define DISP_CHANGE_SUCCESSFUL       0
#define DISP_CHANGE_RESTART          1
#define DISP_CHANGE_FAILED          -1
#define DISP_CHANGE_BADMODE         -2
#define DISP_CHANGE_NOTUPDATED      -3
#define DISP_CHANGE_BADFLAGS        -4
#define DISP_CHANGE_BADPARAM        -5

/*
 * Virtual Keys, Standard Set
 */
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define VK_BACK           0x08
#define VK_TAB            0x09

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5

#define VK_PROCESSKEY     0xE5

#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE

// end_r_winuser



#define WM_NULL                         0x0000
#define WM_CREATE                       0x0001
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005

#define WM_ACTIVATE                     0x0006
/*
 * WM_ACTIVATE state values
 */
#define     WA_INACTIVE     0
#define     WA_ACTIVE       1
#define     WA_CLICKACTIVE  2

#define WM_SETFOCUS                     0x0007
#define WM_KILLFOCUS                    0x0008
#define WM_ENABLE                       0x000A
#define WM_SETREDRAW                    0x000B
#define WM_SETTEXT                      0x000C
#define WM_GETTEXT                      0x000D
#define WM_GETTEXTLENGTH                0x000E
#define WM_PAINT                        0x000F
#define WM_CLOSE                        0x0010
#define WM_QUERYENDSESSION              0x0011
#define WM_QUIT                         0x0012
#define WM_QUERYOPEN                    0x0013
#define WM_ERASEBKGND                   0x0014
#define WM_SYSCOLORCHANGE               0x0015
#define WM_ENDSESSION                   0x0016
#define WM_SHOWWINDOW                   0x0018
#define WM_WININICHANGE                 0x001A
#define WM_SETTINGCHANGE                WM_WININICHANGE


#define WM_DEVMODECHANGE                0x001B
#define WM_ACTIVATEAPP                  0x001C
#define WM_FONTCHANGE                   0x001D
#define WM_TIMECHANGE                   0x001E
#define WM_CANCELMODE                   0x001F
#define WM_SETCURSOR                    0x0020
#define WM_MOUSEACTIVATE                0x0021
#define WM_CHILDACTIVATE                0x0022
#define WM_QUEUESYNC                    0x0023

#define WM_GETMINMAXINFO                0x0024
#define WM_PAINTICON                    0x0026
#define WM_ICONERASEBKGND               0x0027
#define WM_NEXTDLGCTL                   0x0028
#define WM_SPOOLERSTATUS                0x002A
#define WM_DRAWITEM                     0x002B
#define WM_MEASUREITEM                  0x002C
#define WM_DELETEITEM                   0x002D
#define WM_VKEYTOITEM                   0x002E
#define WM_CHARTOITEM                   0x002F
#define WM_SETFONT                      0x0030
#define WM_GETFONT                      0x0031
#define WM_SETHOTKEY                    0x0032
#define WM_GETHOTKEY                    0x0033
#define WM_QUERYDRAGICON                0x0037
#define WM_COMPAREITEM                  0x0039

#define WM_GETOBJECT                    0x003D

#define WM_COMPACTING                   0x0041
#define WM_COMMNOTIFY                   0x0044  /* no longer suported */
#define WM_WINDOWPOSCHANGING            0x0046
#define WM_WINDOWPOSCHANGED             0x0047

#define WM_POWER                        0x0048


#define WM_NOTIFY                       0x004E
#define WM_INPUTLANGCHANGEREQUEST       0x0050
#define WM_INPUTLANGCHANGE              0x0051
#define WM_TCARD                        0x0052
#define WM_HELP                         0x0053
#define WM_USERCHANGED                  0x0054
#define WM_NOTIFYFORMAT                 0x0055

#define NFR_ANSI                             1
#define NFR_UNICODE                          2
#define NF_QUERY                             3
#define NF_REQUERY                           4

#define WM_CONTEXTMENU                  0x007B
#define WM_STYLECHANGING                0x007C
#define WM_STYLECHANGED                 0x007D
#define WM_DISPLAYCHANGE                0x007E
#define WM_GETICON                      0x007F
#define WM_SETICON                      0x0080


#define WM_NCCREATE                     0x0081
#define WM_NCDESTROY                    0x0082
#define WM_NCCALCSIZE                   0x0083
#define WM_NCHITTEST                    0x0084
#define WM_NCPAINT                      0x0085
#define WM_NCACTIVATE                   0x0086
#define WM_GETDLGCODE                   0x0087
#define WM_SYNCPAINT                    0x0088
#define WM_NCMOUSEMOVE                  0x00A0
#define WM_NCLBUTTONDOWN                0x00A1
#define WM_NCLBUTTONUP                  0x00A2
#define WM_NCLBUTTONDBLCLK              0x00A3
#define WM_NCRBUTTONDOWN                0x00A4
#define WM_NCRBUTTONUP                  0x00A5
#define WM_NCRBUTTONDBLCLK              0x00A6
#define WM_NCMBUTTONDOWN                0x00A7
#define WM_NCMBUTTONUP                  0x00A8
#define WM_NCMBUTTONDBLCLK              0x00A9

#define WM_KEYFIRST                     0x0100
#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_CHAR                         0x0102
#define WM_DEADCHAR                     0x0103
#define WM_SYSKEYDOWN                   0x0104
#define WM_SYSKEYUP                     0x0105
#define WM_SYSCHAR                      0x0106
#define WM_SYSDEADCHAR                  0x0107
#define WM_KEYLAST                      0x0108

#define WM_IME_STARTCOMPOSITION         0x010D
#define WM_IME_ENDCOMPOSITION           0x010E
#define WM_IME_COMPOSITION              0x010F
#define WM_IME_KEYLAST                  0x010F

#define WM_INITDIALOG                   0x0110
#define WM_COMMAND                      0x0111
#define WM_SYSCOMMAND                   0x0112
#define WM_TIMER                        0x0113
#define WM_HSCROLL                      0x0114
#define WM_VSCROLL                      0x0115
#define WM_INITMENU                     0x0116
#define WM_INITMENUPOPUP                0x0117
#define WM_MENUSELECT                   0x011F
#define WM_MENUCHAR                     0x0120
#define WM_ENTERIDLE                    0x0121
#define WM_MENURBUTTONUP                0x0122
#define WM_MENUDRAG                     0x0123
#define WM_MENUGETOBJECT                0x0124
#define WM_UNINITMENUPOPUP              0x0125
#define WM_MENUCOMMAND                  0x0126


#define WM_CTLCOLORMSGBOX               0x0132
#define WM_CTLCOLOREDIT                 0x0133
#define WM_CTLCOLORLISTBOX              0x0134
#define WM_CTLCOLORBTN                  0x0135
#define WM_CTLCOLORDLG                  0x0136
#define WM_CTLCOLORSCROLLBAR            0x0137
#define WM_CTLCOLORSTATIC               0x0138


#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209

#define WM_MOUSEWHEEL                   0x020A
#define WM_MOUSELAST                    0x020A

#define WHEEL_DELTA                     120     /* Value for rolling one detent */
#define WHEEL_PAGESCROLL                (UINT_MAX) /* Scroll one page */

#define WM_PARENTNOTIFY                 0x0210
#define WM_ENTERMENULOOP                0x0211
#define WM_EXITMENULOOP                 0x0212

#define WM_SIZING                       0x0214
#define WM_CAPTURECHANGED               0x0215
#define WM_MOVING                       0x0216
// end_r_winuser
#define WM_POWERBROADCAST               0x0218      // r_winuser pbt

#define WM_MDICREATE                    0x0220
#define WM_MDIDESTROY                   0x0221
#define WM_MDIACTIVATE                  0x0222
#define WM_MDIRESTORE                   0x0223
#define WM_MDINEXT                      0x0224
#define WM_MDIMAXIMIZE                  0x0225
#define WM_MDITILE                      0x0226
#define WM_MDICASCADE                   0x0227
#define WM_MDIICONARRANGE               0x0228
#define WM_MDIGETACTIVE                 0x0229


#define WM_MDISETMENU                   0x0230
#define WM_ENTERSIZEMOVE                0x0231
#define WM_EXITSIZEMOVE                 0x0232
#define WM_DROPFILES                    0x0233
#define WM_MDIREFRESHMENU               0x0234


#define WM_IME_SETCONTEXT               0x0281
#define WM_IME_NOTIFY                   0x0282
#define WM_IME_CONTROL                  0x0283
#define WM_IME_COMPOSITIONFULL          0x0284
#define WM_IME_SELECT                   0x0285
#define WM_IME_CHAR                     0x0286
#define WM_IME_REQUEST                  0x0288
#define WM_IME_KEYDOWN                  0x0290
#define WM_IME_KEYUP                    0x0291


#define WM_MOUSEHOVER                   0x02A1
#define WM_MOUSELEAVE                   0x02A3

#define WM_CUT                          0x0300
#define WM_COPY                         0x0301
#define WM_PASTE                        0x0302
#define WM_CLEAR                        0x0303
#define WM_UNDO                         0x0304
#define WM_RENDERFORMAT                 0x0305
#define WM_RENDERALLFORMATS             0x0306
#define WM_DESTROYCLIPBOARD             0x0307
#define WM_DRAWCLIPBOARD                0x0308
#define WM_PAINTCLIPBOARD               0x0309
#define WM_VSCROLLCLIPBOARD             0x030A
#define WM_SIZECLIPBOARD                0x030B
#define WM_ASKCBFORMATNAME              0x030C
#define WM_CHANGECBCHAIN                0x030D
#define WM_HSCROLLCLIPBOARD             0x030E
#define WM_QUERYNEWPALETTE              0x030F
#define WM_PALETTEISCHANGING            0x0310
#define WM_PALETTECHANGED               0x0311
#define WM_HOTKEY                       0x0312

#define WM_PRINT                        0x0317
#define WM_PRINTCLIENT                  0x0318

#define WM_HANDHELDFIRST                0x0358
#define WM_HANDHELDLAST                 0x035F

#define WM_AFXFIRST                     0x0360
#define WM_AFXLAST                      0x037F

#define WM_PENWINFIRST                  0x0380
#define WM_PENWINLAST                   0x038F

#define WM_APP                          0x8000



#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L

#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L


#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW

/*
 * Common Window Styles
 */
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)

#define WS_POPUPWINDOW      (WS_POPUP          | \
                             WS_BORDER         | \
                             WS_SYSMENU)

#define WS_CHILDWINDOW      (WS_CHILD)

#define SM_CXSCREEN 0
#define SM_CYSCREEN 1

/*
 * NOTE: All Message Numbers below 0x0400 are RESERVED.
 *
 * Private Window Messages Start Here:
 */
#define WM_USER                         0x0400

typedef void* HCURSOR;
typedef void* HBRUSH;

typedef struct tagWNDCLASSA {
  UINT        style;
  WNDPROC     lpfnWndProc;
  int         cbClsExtra;
  int         cbWndExtra;
  HINSTANCE   hInstance;
  HICON       hIcon;
  HCURSOR     hCursor;
  HBRUSH      hbrBackground;
  LPCSTR      lpszMenuName;
  LPCSTR      lpszClassName;
} WNDCLASSA;
typedef WNDCLASSA WNDCLASS;

typedef struct tagWNDCLASSEXA {
  UINT        cbSize;
  /* Win 3.x */
  UINT        style;
  WNDPROC     lpfnWndProc;
  int         cbClsExtra;
  int         cbWndExtra;
  HINSTANCE   hInstance;
  HICON       hIcon;
  HCURSOR     hCursor;
  HBRUSH      hbrBackground;
  LPCSTR      lpszMenuName;
  LPCSTR      lpszClassName;
  /* Win 4.0 */
  HICON       hIconSm;
} WNDCLASSEXA, WNDCLASSEX;
#define ENUM_CURRENT_SETTINGS       ((DWORD)-1)
#define ENUM_REGISTRY_SETTINGS      ((DWORD)-2)

typedef struct tagMSG {
  HWND        hwnd;
  UINT        message;
  WPARAM      wParam;
  LPARAM      lParam;
  DWORD       time;
  POINT       pt;
  //DWORD       lPrivate;
} MSG;

#define PM_NOREMOVE         0x0000
#define PM_REMOVE           0x0001
#define PM_NOYIELD          0x0002

#define MAKEINTRESOURCE(i) (LPSTR)((DWORD)((WORD)(i)))

#define IDC_ARROW           MAKEINTRESOURCE(32512)
#define IDC_IBEAM           MAKEINTRESOURCE(32513)
#define IDC_WAIT            MAKEINTRESOURCE(32514)
#define IDC_CROSS           MAKEINTRESOURCE(32515)
#define IDC_UPARROW         MAKEINTRESOURCE(32516)
#define IDC_SIZE            MAKEINTRESOURCE(32640) /* OBSOLETE: use IDC_SIZEALL */
#define IDC_ICON            MAKEINTRESOURCE(32641) /* OBSOLETE: use IDC_ARROW */
#define IDC_SIZENWSE        MAKEINTRESOURCE(32642)
#define IDC_SIZENESW        MAKEINTRESOURCE(32643)
#define IDC_SIZEWE          MAKEINTRESOURCE(32644)
#define IDC_SIZENS          MAKEINTRESOURCE(32645)
#define IDC_SIZEALL         MAKEINTRESOURCE(32646)
#define IDC_NO              MAKEINTRESOURCE(32648) /* not in win3.1 */
#define IDC_HAND            MAKEINTRESOURCE(32649)
#define IDC_APPSTARTING     MAKEINTRESOURCE(32650) /* not in win3.1 */
#define IDC_HELP            MAKEINTRESOURCE(32651)


#define VER_PLATFORM_WIN32s             0
#define VER_PLATFORM_WIN32_WINDOWS      1
#define VER_PLATFORM_WIN32_NT           2

#define VER_NT_WORKSTATION              0x0000001
#define VER_NT_DOMAIN_CONTROLLER        0x0000002
#define VER_NT_SERVER                   0x0000003



#define SM_CXSCREEN             0
#define SM_CYSCREEN             1
#define SM_CXVSCROLL            2
#define SM_CYHSCROLL            3
#define SM_CYCAPTION            4
#define SM_CXBORDER             5
#define SM_CYBORDER             6
#define SM_CXDLGFRAME           7
#define SM_CYDLGFRAME           8
#define SM_CYVTHUMB             9
#define SM_CXHTHUMB             10
#define SM_CXICON               11
#define SM_CYICON               12
#define SM_CXCURSOR             13
#define SM_CYCURSOR             14
#define SM_CYMENU               15
#define SM_CXFULLSCREEN         16
#define SM_CYFULLSCREEN         17
#define SM_CYKANJIWINDOW        18
#define SM_MOUSEPRESENT         19
#define SM_CYVSCROLL            20
#define SM_CXHSCROLL            21
#define SM_DEBUG                22
#define SM_SWAPBUTTON           23
#define SM_RESERVED1            24
#define SM_RESERVED2            25
#define SM_RESERVED3            26
#define SM_RESERVED4            27
#define SM_CXMIN                28
#define SM_CYMIN                29
#define SM_CXSIZE               30
#define SM_CYSIZE               31
#define SM_CXFRAME              32
#define SM_CYFRAME              33
#define SM_CXMINTRACK           34
#define SM_CYMINTRACK           35
#define SM_CXDOUBLECLK          36
#define SM_CYDOUBLECLK          37
#define SM_CXICONSPACING        38
#define SM_CYICONSPACING        39
#define SM_MENUDROPALIGNMENT    40
#define SM_PENWINDOWS           41
#define SM_DBCSENABLED          42
#define SM_CMOUSEBUTTONS        43

#define SM_CXFIXEDFRAME           SM_CXDLGFRAME  /* ;win40 name change */
#define SM_CYFIXEDFRAME           SM_CYDLGFRAME  /* ;win40 name change */
#define SM_CXSIZEFRAME            SM_CXFRAME     /* ;win40 name change */
#define SM_CYSIZEFRAME            SM_CYFRAME     /* ;win40 name change */

#define SM_SECURE               44
#define SM_CXEDGE               45
#define SM_CYEDGE               46
#define SM_CXMINSPACING         47
#define SM_CYMINSPACING         48
#define SM_CXSMICON             49
#define SM_CYSMICON             50
#define SM_CYSMCAPTION          51
#define SM_CXSMSIZE             52
#define SM_CYSMSIZE             53
#define SM_CXMENUSIZE           54
#define SM_CYMENUSIZE           55
#define SM_ARRANGE              56
#define SM_CXMINIMIZED          57
#define SM_CYMINIMIZED          58
#define SM_CXMAXTRACK           59
#define SM_CYMAXTRACK           60
#define SM_CXMAXIMIZED          61
#define SM_CYMAXIMIZED          62
#define SM_NETWORK              63
#define SM_CLEANBOOT            67
#define SM_CXDRAG               68
#define SM_CYDRAG               69
#define SM_SHOWSOUNDS           70
#define SM_CXMENUCHECK          71   /* Use instead of GetMenuCheckMarkDimensions()! */
#define SM_CYMENUCHECK          72
#define SM_SLOWMACHINE          73
#define SM_MIDEASTENABLED       74
#define SM_MOUSEWHEELPRESENT    75
#define SM_XVIRTUALSCREEN       76
#define SM_YVIRTUALSCREEN       77
#define SM_CXVIRTUALSCREEN      78
#define SM_CYVIRTUALSCREEN      79
#define SM_CMONITORS            80
#define SM_SAMEDISPLAYFORMAT    81

#define SM_CMETRICS             83

#define GMEM_FIXED          0x0000
#define GMEM_MOVEABLE       0x0002
#define GMEM_NOCOMPACT      0x0010
#define GMEM_NODISCARD      0x0020
#define GMEM_ZEROINIT       0x0040
#define GMEM_MODIFY         0x0080
#define GMEM_DISCARDABLE    0x0100
#define GMEM_NOT_BANKED     0x1000
#define GMEM_SHARE          0x2000
#define GMEM_DDESHARE       0x2000
#define GMEM_NOTIFY         0x4000
#define GMEM_LOWER          GMEM_NOT_BANKED
#define GMEM_VALID_FLAGS    0x7F72
#define GMEM_INVALID_HANDLE 0x8000

#define GHND                (GMEM_MOVEABLE | GMEM_ZEROINIT)
#define GPTR                (GMEM_FIXED | GMEM_ZEROINIT)

#define CF_TEXT             1
#define CF_BITMAP           2
#define CF_METAFILEPICT     3
#define CF_SYLK             4
#define CF_DIF              5
#define CF_TIFF             6
#define CF_OEMTEXT          7
#define CF_DIB              8
#define CF_PALETTE          9
#define CF_PENDATA          10
#define CF_RIFF             11
#define CF_WAVE             12
#define CF_UNICODETEXT      13
#define CF_ENHMETAFILE      14
#define CF_HDROP            15
#define CF_LOCALE           16
#define CF_MAX              17

#define CF_OWNERDISPLAY     0x0080
#define CF_DSPTEXT          0x0081
#define CF_DSPBITMAP        0x0082
#define CF_DSPMETAFILEPICT  0x0083
#define CF_DSPENHMETAFILE   0x008E

#define GWL_WNDPROC         (-4)
#define GWL_HINSTANCE       (-6)
#define GWL_HWNDPARENT      (-8)
#define GWL_STYLE           (-16)
#define GWL_EXSTYLE         (-20)
#define GWL_USERDATA        (-21)
#define GWL_ID              (-12)

#define SWP_NOSIZE          0x0001
#define SWP_NOMOVE          0x0002
#define SWP_NOZORDER        0x0004
#define SWP_NOREDRAW        0x0008
#define SWP_NOACTIVATE      0x0010
#define SWP_FRAMECHANGED    0x0020  /* The frame changed: send WM_NCCALCSIZE */
#define SWP_SHOWWINDOW      0x0040
#define SWP_HIDEWINDOW      0x0080
#define SWP_NOCOPYBITS      0x0100
#define SWP_NOOWNERZORDER   0x0200  /* Don't do owner Z ordering */
#define SWP_NOSENDCHANGING  0x0400  /* Don't send WM_WINDOWPOSCHANGING */

#define SWP_DRAWFRAME       SWP_FRAMECHANGED
#define SWP_NOREPOSITION    SWP_NOOWNERZORDER
#define SWP_DEFERERASE      0x2000
#define SWP_ASYNCWINDOWPOS  0x4000

#define HWND_TOP        ((HWND)0)
#define HWND_BOTTOM     ((HWND)1)
#define HWND_TOPMOST    ((HWND)-1)
#define HWND_NOTOPMOST  ((HWND)-2)


typedef struct tagCURSORINFO
{
  DWORD   cbSize;
  DWORD   flags;
  HCURSOR hCursor;
  POINT   ptScreenPos;
} CURSORINFO;

typedef struct _MEMORYSTATUS {
  DWORD dwLength;
  DWORD dwMemoryLoad;
  DWORD dwTotalPhys;
  DWORD dwAvailPhys;
  DWORD dwTotalPageFile;
  DWORD dwAvailPageFile;
  DWORD dwTotalVirtual;
  DWORD dwAvailVirtual;
} MEMORYSTATUS;

typedef struct _OSVERSIONINFOEXA {
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  CHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
  WORD wServicePackMajor;
  WORD wServicePackMinor;
  WORD   wSuiteMask;
  BYTE  wProductType;
  BYTE  wReserved;
} OSVERSIONINFOEXA, OSVERSIONINFOEX;
typedef struct _OSVERSIONINFOA {
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  CHAR   szCSDVersion[ 128 ];     // Maintenance string for PSS usage
} OSVERSIONINFOA, OSVERSIONINFO;
typedef struct tagWINDOWPLACEMENT {
  UINT  length;
  UINT  flags;
  UINT  showCmd;
  POINT ptMinPosition;
  POINT ptMaxPosition;
  RECT  rcNormalPosition;
#ifdef _MAC
  RECT  rcDevice;
#endif
} WINDOWPLACEMENT;
#define ZeroMemory(p, sz)  memset(p, 0, sz)
BOOL WINAPI GetVersionExA(OSVERSIONINFOA* lpVersionInformation );
VOID WINAPI Sleep(DWORD dwMilliseconds);
LONG WINAPI DispatchMessageA(CONST MSG *lpMsg);
BOOL WINAPI PeekMessageA( MSG* lpMsg,HWND hWnd ,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg);
HDC WINAPI CreateDCA(LPCSTR, LPCSTR , LPCSTR , CONST DEVMODEA *);
HDC     WINAPI CreateCompatibleDC(HDC);
HBITMAP WINAPI CreateCompatibleBitmap(HDC, int, int);
HGDIOBJ WINAPI SelectObject(HDC, HGDIOBJ);
BOOL  WINAPI BitBlt(HDC, int, int, int, int, HDC, int, int, DWORD);
BOOL WINAPI DeleteDC(HDC);
BOOL WINAPI DeleteObject(HGDIOBJ);
HPALETTE WINAPI CreatePalette(CONST LOGPALETTE *);
HPALETTE WINAPI SelectPalette(HDC, HPALETTE, BOOL);
HDC WINAPI GetDC(HWND hWnd);
int WINAPI ReleaseDC( HWND hWnd, HDC hDC);
LRESULT CALLBACK DefWindowProcA(HWND hWnd,UINT Msg, WPARAM wParam, LPARAM lParam);
HANDLE WINAPI LoadImageA(HINSTANCE,LPCSTR,UINT,int,int,UINT);
BOOL WINAPI GetClassInfoA(HINSTANCE hInstance,LPCSTR lpClassName,WNDCLASSA* lpWndClass);
typedef WORD ATOM;
ATOM WINAPI RegisterClassExA(CONST WNDCLASSEXA *);
int WINAPI GetSystemMetrics(int nIndex);
BOOL WINAPI AdjustWindowRect(RECT* lpRect, DWORD dwStyle, BOOL bMenu);
BOOL WINAPI SetForegroundWindow(HWND hWnd);
BOOL WINAPI UpdateWindow(HWND hWnd);
BOOL WINAPI MoveWindow(HWND hWnd,int X,int Y,int nWidth,int nHeight,BOOL bRepaint);
typedef void* HMENU;
HWND WINAPI CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
DWORD dwStyle,int X,int Y,int nWidth,int nHeight,
HWND hWndParent ,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
BOOL WINAPI GetWindowRect(HWND hWnd, RECT* lpRect);
BOOL WINAPI GetClientRect(HWND hWnd, RECT* lpRect);
UINT WINAPI GetDoubleClickTime(VOID);
HWND WINAPI SetActiveWindow(HWND hWnd);
BOOL WINAPI PeekMessageA(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL WINAPI DestroyWindow(HWND hWnd);
BOOL WINAPI UnregisterClassA(LPCSTR lpClassName, HINSTANCE hInstance);
HWND WINAPI GetActiveWindow(VOID);
HWND WINAPI GetFocus(VOID);
BOOL WINAPI GetWindowPlacement(HWND hWnd, WINDOWPLACEMENT *lpwndpl);

 int   WINAPI SetBkMode(HDC, int);
 LONG  WINAPI SetBitmapBits(HBITMAP, DWORD, CONST VOID *);

 UINT  WINAPI SetBoundsRect(HDC, CONST RECT *, UINT);
 int   WINAPI SetDIBits(HDC, HBITMAP, UINT, UINT, CONST VOID *, CONST BITMAPINFO *, UINT);
 int   WINAPI SetDIBitsToDevice(HDC, int, int, DWORD, DWORD, int,
                                         int, UINT, UINT, CONST VOID *, CONST BITMAPINFO *, UINT);
 DWORD WINAPI SetMapperFlags(HDC, DWORD);
 int   WINAPI SetGraphicsMode(HDC hdc, int iMode);
 int   WINAPI SetMapMode(HDC, int);
 HMETAFILE   WINAPI SetMetaFileBitsEx(UINT, CONST BYTE *);
 UINT  WINAPI SetPaletteEntries(HPALETTE, UINT, UINT, CONST PALETTEENTRY *);
 COLORREF WINAPI SetPixel(HDC, int, int, COLORREF);
 BOOL   WINAPI SetPixelV(HDC, int, int, COLORREF);
// BOOL  WINAPI SetPixelFormat(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
 int   WINAPI SetPolyFillMode(HDC, int);
 BOOL   WINAPI StretchBlt(HDC, int, int, int, int, HDC, int, int, int, int, DWORD);
 BOOL   WINAPI SetRectRgn(HRGN, int, int, int, int);
 int   WINAPI StretchDIBits(HDC, int, int, int, int, int, int, int, int, CONST
                                     VOID *, CONST BITMAPINFO *, UINT, DWORD);
 int   WINAPI SetROP2(HDC, int);
 int   WINAPI SetStretchBltMode(HDC, int);
 UINT  WINAPI SetSystemPaletteUse(HDC, UINT);
 int   WINAPI SetTextCharacterExtra(HDC, int);
 COLORREF WINAPI SetTextColor(HDC, COLORREF);
 UINT  WINAPI SetTextAlign(HDC, UINT);
 BOOL  WINAPI SetTextJustification(HDC, int, int);
 BOOL  WINAPI UpdateColors(HDC);
BOOL WINAPI OpenClipboard(HWND hWndNewOwner);
BOOL WINAPI CloseClipboard(VOID);
BOOL WINAPI EmptyClipboard(VOID);
HGLOBAL WINAPI GlobalAlloc(UINT uFlags, UINT dwBytes);
void* WINAPI GlobalLock(HGLOBAL hMem);
BOOL WINAPI GlobalUnlock(HGLOBAL hMem);
HANDLE WINAPI SetClipboardData(UINT uFormat, HANDLE hMem);
HANDLE WINAPI GetClipboardData(UINT uFormat);
VOID WINAPI GlobalMemoryStatus(MEMORYSTATUS* lpBuffer);
BOOL WINAPI GetCursorInfo(CURSORINFO* pci);
int WINAPI ShowCursor(BOOL bShow);
BOOL WINAPI GetCursorPos(POINT* lpPoint);
BOOL WINAPI SetCursorPos(int X, int Y);
DWORD WINAPI GetMessagePos(VOID);
LONG WINAPI SetWindowLongPtrA(HWND hWnd, int nIndex, LONG dwNewLong);
BOOL WINAPI MoveWindow(HWND hWnd,int X,int Y,int nWidth,int nHeight,BOOL bRepaint);
BOOL WINAPI SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X,int Y,int cx,int cy,UINT uFlags);
BOOL WINAPI GetWindowPlacement(HWND hWnd, WINDOWPLACEMENT *lpwndpl);
BOOL WINAPI SetWindowPlacement(HWND hWnd, CONST WINDOWPLACEMENT *lpwndpl);

 BOOL        WINAPI GetDeviceGammaRamp(HDC,LPVOID);
 BOOL        WINAPI SetDeviceGammaRamp(HDC,LPVOID);
 BOOL        WINAPI ColorMatchToTarget(HDC,HDC,DWORD);
DWORD WINAPI GetLastError(VOID);

#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
#define FORMAT_MESSAGE_FROM_STRING     0x00000400
#define FORMAT_MESSAGE_FROM_HMODULE    0x00000800
#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
#define FORMAT_MESSAGE_MAX_WIDTH_MASK  0x000000FF


#define NOERROR             0
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L

#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L

#define MB_USERICON                 0x00000080L
#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND

int WINAPI MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
DWORD WINAPI FormatMessageA(DWORD dwFlags, void* lpSource, DWORD dwMessageId, DWORD dwLanguageId, char* lpBuffer, DWORD nSize, void* Arguments);
void* WINAPI LocalFree(void* hMem);


#define DIB_RGB_COLORS      0 /* color table in RGBs */
#define DIB_PAL_COLORS      1 /* color table in palette indices */

typedef void* HIMAGELIST;
#define FAR
int         WINAPI ImageList_GetImageCount(HIMAGELIST himl);
COLORREF    WINAPI ImageList_GetBkColor(HIMAGELIST himl);
BOOL        WINAPI ImageList_GetIconSize(HIMAGELIST himl, int FAR *cx, int FAR *cy);
HICON       WINAPI ImageList_GetIcon(HIMAGELIST himl, int i, UINT flags);
BOOL     WINAPI  DestroyIcon(HICON hIcon);

BOOL  WINAPI GdiFlush();
HGDIOBJ WINAPI GetCurrentObject(HDC, UINT);
#define OBJ_PEN             1
#define OBJ_BRUSH           2
#define OBJ_DC              3
#define OBJ_METADC          4
#define OBJ_PAL             5
#define OBJ_FONT            6
#define OBJ_BITMAP          7
#define OBJ_REGION          8
#define OBJ_METAFILE        9
#define OBJ_MEMDC           10
#define OBJ_EXTPEN          11
#define OBJ_ENHMETADC       12
#define OBJ_ENHMETAFILE     13

#define DELETE                           (0x00010000L)
#define READ_CONTROL                     (0x00020000L)
#define WRITE_DAC                        (0x00040000L)
#define WRITE_OWNER                      (0x00080000L)
#define SYNCHRONIZE                      (0x00100000L)

#define STANDARD_RIGHTS_REQUIRED         (0x000F0000L)

#define STANDARD_RIGHTS_READ             (READ_CONTROL)
#define STANDARD_RIGHTS_WRITE            (READ_CONTROL)
#define STANDARD_RIGHTS_EXECUTE          (READ_CONTROL)

#define STANDARD_RIGHTS_ALL              (0x001F0000L)

#define SPECIFIC_RIGHTS_ALL              (0x0000FFFFL)



#define HKEY_CLASSES_ROOT           (( HKEY ) 0x80000000 )
#define HKEY_CURRENT_USER           (( HKEY ) 0x80000001 )
#define HKEY_LOCAL_MACHINE          (( HKEY ) 0x80000002 )
#define HKEY_USERS                  (( HKEY ) 0x80000003 )
#define HKEY_PERFORMANCE_DATA       (( HKEY ) 0x80000004 )
#define HKEY_CURRENT_CONFIG         (( HKEY ) 0x80000005 )
#define HKEY_DYN_DATA               (( HKEY ) 0x80000006 )

#define KEY_QUERY_VALUE         (0x0001)
#define KEY_SET_VALUE           (0x0002)
#define KEY_CREATE_SUB_KEY      (0x0004)
#define KEY_ENUMERATE_SUB_KEYS  (0x0008)
#define KEY_NOTIFY              (0x0010)
#define KEY_CREATE_LINK         (0x0020)

#define KEY_READ                ((STANDARD_RIGHTS_READ       |\
  KEY_QUERY_VALUE            |\
  KEY_ENUMERATE_SUB_KEYS     |\
  KEY_NOTIFY)                 \
  &                           \
(~SYNCHRONIZE))


#define KEY_WRITE               ((STANDARD_RIGHTS_WRITE      |\
  KEY_SET_VALUE              |\
  KEY_CREATE_SUB_KEY)         \
  &                           \
(~SYNCHRONIZE))

#define KEY_EXECUTE             ((KEY_READ)                   \
  &                           \
(~SYNCHRONIZE))

#define KEY_ALL_ACCESS          ((STANDARD_RIGHTS_ALL        |\
  KEY_QUERY_VALUE            |\
  KEY_SET_VALUE              |\
  KEY_CREATE_SUB_KEY         |\
  KEY_ENUMERATE_SUB_KEYS     |\
  KEY_NOTIFY                 |\
  KEY_CREATE_LINK)            \
  &                           \
(~SYNCHRONIZE))



#define ERROR_SUCCESS                    0L

typedef void* HKEY;
typedef void** PHKEY;
typedef ulong REGSAM;
#define APIENTRY  __stdcall

LONG APIENTRY RegOpenKeyExA(HKEY hKey,LPCSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult);
LONG APIENTRY RegQueryValueExA(HKEY hKey,LPCSTR lpValueName,DWORD* lpReserved, DWORD* lpType, BYTE* lpData, DWORD* lpcbData);
LONG APIENTRY RegCloseKey(HKEY hKey);

typedef struct _FILETIME {
		DWORD dwLowDateTime;
    DWORD dwHighDateTime;
}	FILETIME;

typedef char* LPSTR;
LONG APIENTRY RegCreateKeyExA(HKEY hKey,LPCSTR lpSubKey,DWORD Reserved,LPSTR lpClass,DWORD dwOptions,
                              REGSAM samDesired,void* lpSecurityAttributes,PHKEY phkResult,DWORD* lpdwDisposition);

LONG APIENTRY RegEnumKeyExA(HKEY hKey,DWORD dwIndex,LPSTR lpName,DWORD* lpcbName,DWORD* lpReserved,
                            LPSTR lpClass,DWORD* lpcbClass,FILETIME* lpftLastWriteTime);
#define ERROR_MORE_DATA                  234L    // dderror

LONG APIENTRY RegDeleteKeyA(HKEY hKey, LPCSTR lpSubKey);

LONG APIENTRY RegSetValueExA( HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);

#define REG_NONE                    ( 0 )   // No value type
#define REG_SZ                      ( 1 )   // Unicode nul terminated string
#define REG_EXPAND_SZ               ( 2 )   // Unicode nul terminated string
// (with environment variable references)
#define REG_BINARY                  ( 3 )   // Free form binary
#define REG_DWORD                   ( 4 )   // 32-bit number
#define REG_DWORD_LITTLE_ENDIAN     ( 4 )   // 32-bit number (same as REG_DWORD)
#define REG_DWORD_BIG_ENDIAN        ( 5 )   // 32-bit number
#define REG_LINK                    ( 6 )   // Symbolic Link (unicode)
#define REG_MULTI_SZ                ( 7 )   // Multiple Unicode strings
#define REG_RESOURCE_LIST           ( 8 )   // Resource list in the resource map
#define REG_FULL_RESOURCE_DESCRIPTOR ( 9 )  // Resource list in the hardware description
#define REG_RESOURCE_REQUIREMENTS_LIST ( 10 )

typedef __int64 LONGLONG;
typedef union _LARGE_INTEGER {
  struct {
    DWORD LowPart;
    LONG HighPart;
  };
  LONGLONG QuadPart;
} LARGE_INTEGER;
BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);
BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount);
VOID WINAPI OutputDebugStringA(const char* lpOutputString);
void* WINAPI GetModuleHandleA(LPCSTR lpModuleName);
BOOL WINAPI EnumDisplaySettingsA(LPCSTR lpszDeviceName, DWORD iModeNum, DEVMODEA* lpDevMode);
HKL WINAPI GetKeyboardLayout(DWORD dwLayout);
HICON WINAPI CreateIconIndirect(ICONINFO* piconinfo);
DWORD WINAPI GetTickCount(VOID);
LONG WINAPI ChangeDisplaySettingsA(DEVMODEA* lpDevMode, DWORD dwFlags);
BOOL WINAPI EnumDisplaySettingsA(LPCSTR lpszDeviceName, DWORD iModeNum, DEVMODEA* lpDevMode);
LONG WINAPI ChangeDisplaySettingsA(DEVMODEA* lpDevMode, DWORD dwFlags);
LONG WINAPI SetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong);
typedef DEVMODEA DEVMODE;
void* WINAPI GetModuleHandleA(const char * lpModuleName);
void* WINAPI GetProcAddress(void * hModule, const char * lpProcName);
DWORD WINAPI GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename,DWORD nSize);
typedef WORD* LPWSTR;
typedef const WORD* LPCWSTR;
typedef const char* LPCSTR;
int WINAPI MultiByteToWideChar(UINT CodePage,DWORD dwFlags,LPCSTR lpMultiByteStr,int cchMultiByte,LPWSTR lpWideCharStr,int cchWideChar);
int WINAPI WideCharToMultiByte(UINT CodePage,DWORD dwFlags,LPCWSTR lpWideCharStr,int cchWideChar,LPSTR lpMultiByteStr,int cchMultiByte,LPCSTR lpDefaultChar,BOOL* lpUsedDefaultChar);
typedef float FLOAT;
typedef void* LPXFORM;
typedef void XFORM;
BOOL WINAPI AngleArc(HDC, int, int, DWORD, FLOAT, FLOAT);
BOOL WINAPI PolyPolyline(HDC, CONST POINT *, CONST DWORD *, DWORD);
BOOL WINAPI GetWorldTransform(HDC, LPXFORM);
BOOL WINAPI SetWorldTransform(HDC, CONST XFORM *);
BOOL WINAPI ModifyWorldTransform(HDC, CONST XFORM *, DWORD);
BOOL WINAPI CombineTransform(LPXFORM, CONST XFORM *, CONST XFORM *);
HBITMAP WINAPI CreateDIBSection(HDC, CONST BITMAPINFO *, UINT, VOID **, HANDLE, DWORD);
UINT WINAPI GetDIBColorTable(HDC, UINT, UINT, RGBQUAD *);
UINT WINAPI SetDIBColorTable(HDC, UINT, UINT, CONST RGBQUAD *);


#endif


// Needed for old windows apis
// depending on the SDK version and compilers some defines might be available
// or not
#ifndef PRODUCT_ULTIMATE
#define PRODUCT_ULTIMATE  0x00000001
#define PRODUCT_HOME_BASIC  0x00000002
#define PRODUCT_HOME_PREMIUM  0x00000003
#define PRODUCT_ENTERPRISE  0x00000004
#define PRODUCT_HOME_BASIC_N  0x00000005
#define PRODUCT_BUSINESS  0x00000006
#define PRODUCT_STARTER   0x0000000B
#endif
#ifndef PRODUCT_ULTIMATE_N
#define PRODUCT_BUSINESS_N  0x00000010
#define PRODUCT_HOME_PREMIUM_N  0x0000001A
#define PRODUCT_ENTERPRISE_N  0x0000001B
#define PRODUCT_ULTIMATE_N  0x0000001C
#endif
#ifndef PRODUCT_STARTER_N
#define PRODUCT_STARTER_N 0x0000002F
#endif
#ifndef PRODUCT_PROFESSIONAL
#define PRODUCT_PROFESSIONAL  0x00000030
#define PRODUCT_PROFESSIONAL_N  0x00000031
#endif
#ifndef PRODUCT_ULTIMATE_E
#define PRODUCT_STARTER_E 0x00000042
#define PRODUCT_HOME_BASIC_E  0x00000043
#define PRODUCT_HOME_PREMIUM_E  0x00000044
#define PRODUCT_PROFESSIONAL_E  0x00000045
#define PRODUCT_ENTERPRISE_E  0x00000046
#define PRODUCT_ULTIMATE_E  0x00000047
#endif


#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

#if 0
{char buf[256];utime_start(_start_time);
t=utime_elapsed(_start_time);_snprintf(buf, 256, "%.3f", t);}
{utime_start(_start_time);
printf("%f\n", utime_elapsed(_start_time));}
#endif
// 高精度计时 (单位为秒, 精确到微秒)
static __int64 utime_frequency()
{
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
  return frequency.QuadPart;
}
static __int64 utime_counter()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter((LARGE_INTEGER*)&counter);
  return counter.QuadPart;
}
#if 1
typedef __int64 utime_type;
#define utime_start(_start_time)     __int64 _start_time = utime_counter()
#define utime_restart(_start_time)   (_start_time = utime_counter())
#define utime_count(_start_time)     (utime_counter() - _start_time)
#define utime_elapsed(_start_time)   (utime_count(_start_time) * 1.0 / utime_frequency())
#else
typedef clock_t utime_type;
#define utime_start(_start_time)     clock_t _start_time = clock()
#define utime_restart(_start_time)   (_start_time = clock())
#define utime_count(_start_time)     (clock() - _start_time)
#define utime_elapsed(_start_time)   (utime_count(_start_time) * 1.0 / CLOCKS_PER_SEC)
#endif

static char* get_exe_path(char* buf, int count) {
  GetModuleFileNameA(0, buf, count);
  return buf;
}

static char* GetAppPath()
{
  int i;
  static char AppPath[256];
  get_exe_path(AppPath, 256);
  i = strlen(AppPath);
  while (i > 0 && AppPath[i] != '\\') {
    i--;
  }
  if (i > 0) {
    AppPath[i] = 0;
  }
  return (AppPath);
}
#include <direct.h>
#include <stdio.h>
#include <io.h>
enum FILE_ACCESS_MODE {
  AS_EXIST = 0 , AS_EXECUTE = 1, AS_WRITE = 2, AS_READ = 4, AS_WRITE_READ = 6
};
#define file_exist(name)       (0 == _access(name, AS_EXIST))
#define file_execute(name)     (0 == _access(name, AS_EXECUTE))
#define file_write(name)       (0 == _access(name, AS_WRITE))
#define file_read(name)        (0 == _access(name, AS_READ))
#define file_write_read(name)  (0 == _access(name, AS_WRITE_READ))
#define file_subdir(name)      (0 == _access(name, _A_SUBDIR))
#define fileok(name)           (file_exist( name ) && filesize(name)>0)
static char* find_not_eixst_file(const char* filename, char* buf, int buflen)
{
  int i = 0;
  static char _buf[ 256 ];
  if (NULL == buf) {
    buf = _buf;
    buflen = 256;
  }
  for (; ; ++i) {
    _snprintf(buf, buflen, filename, i);
    if (!file_exist(buf)) {
      return buf;
    }
  }
}
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#define S_ISDIR(mode)  ((mode)&_A_SUBDIR)
#define UNEIXST_FILE(_FNAME) find_not_eixst_file(_FNAME, 0, 0)
#define FF_FULLNAME  (1<<0)
#define FF_SUBDIR    (1<<1)
#define FF_NODIR     (1<<3)
typedef struct fileinfo_t {
  char name[MAX_PATH];
  unsigned attrib; // _A_SUBDIR
  time_t ctime;//_create;    /* -1 for FAT file systems */
  time_t atime;//access;    /* -1 for FAT file systems */
  time_t wtime;//write;
  unsigned __int64 size;
} fileinfo_t;
typedef struct findinfo_t {
  int hFile;
  int count;
} findinfo_t;
static int find_next_file(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag) {
  struct _finddata_t info[1];
  if (0==s->hFile || -1==s->hFile) {
    char buf[256];
    _snprintf(buf, 256, "%s/%s", path, filters?filters:"*", -1);
    s->hFile = _findfirst(buf, info);
    s->count++;
    f->size = info->size;
    f->ctime = info->time_create;
    f->atime = info->time_access;
    f->wtime = info->time_write;
    f->attrib = info->attrib;
    if (flag & (FF_FULLNAME|FF_SUBDIR)) {
      _snprintf(f->name, MAX_PATH, "%s/%s", path, info->name);
    } else {
      strncpy(f->name, info->name, MAX_PATH);
    }
    return (-1 != s->hFile);
  } else {
    if (_findnext(s->hFile, info) == 0) {
      s->count++;
      f->size = info->size;
      f->ctime = info->time_create;
      f->atime = info->time_access;
      f->wtime = info->time_write;
      f->attrib = info->attrib;
      if (flag & (FF_FULLNAME|FF_SUBDIR)) {
        _snprintf(f->name, MAX_PATH, "%s/%s", path, info->name);
      } else {
        strncpy(f->name, info->name, MAX_PATH);
      }
      return TRUE;
    } else {
      _findclose(s->hFile);
      s->hFile = 0;
    }
  }
  return FALSE;
}

#define  BMPMARK(A)  (((A)+3)&~3)

// returns TRUE if there is a problem such as ERROR_IO_PENDING.
static BOOL GetBitmapData(HDC dc, int* pwidth, int* pheight, int* channels, int** data)
{
  BITMAP bmp;
  HGDIOBJ h;
  GdiFlush();
  h = GetCurrentObject(dc, OBJ_BITMAP);
  
  if (pwidth) {
    *pwidth = 0;
  }
  if (pheight) {
    *pheight = 0;
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
  
  if (pwidth) {
    *pwidth = abs(bmp.bmWidth);
  }
  if (pheight) {
    *pheight = abs(bmp.bmHeight);
  }
  
  if (channels) {
    * channels = bmp.bmBitsPixel / 8;
  }
  
  if (data) {
    *data = (int*)bmp.bmBits;
  }
  
  return FALSE;
}
static const char* icvWindowPosRootKey = "Software\\OpenCV\\HighGUI\\Windows\\";
static void FillSolidRect(HDC m_hDC, void* lpRect, COLORREF clr)
{
  SetBkColor(m_hDC, clr);
  ExtTextOutA(m_hDC, 0, 0, ETO_OPAQUE, (RECT*)lpRect, NULL, 0, NULL);
}
static int FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin, const RGBQUAD* inpal)
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

static int cvShowDC1(HDC dc,  int height, int width, const unsigned char* arr, int step, int cn, const RGBQUAD* inpal)
{
  int i, j, h, w;
  int channels = 0;
  int* dst_ptr = 0;
  const int channels0 = 3;
  int origin = 0;
  HBITMAP image = NULL;
  
  // if there is something wrong with these system calls, we cannot display image...
  if (GetBitmapData(dc, &w, &h, &channels, &dst_ptr)) {
    //return 0;
  }
  
  if (w != width || h != height || channels != channels0) {
    unsigned char buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
    BITMAPINFO* binfo = (BITMAPINFO*) buffer;
    int cnbit[] = {1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    
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
      DeleteObject(SelectPalette(dc, hPal, TRUE));
    }
    
    //DeleteObject(SelectObject(dc, image));
    image = 0;
    
    w = width;
    h = height;
    FillBitmapInfo(binfo, w, h, cnbit[cn] * 8, 1, inpal);
    image = (HBITMAP)SelectObject(dc, CreateDIBSection(dc, binfo, DIB_RGB_COLORS, (void**)&dst_ptr, 0, 0));
  }
  
  if (8 == cn) {
    int dststep = (w + 3) & -4, mini = 0, maxi = 0;
    unsigned char* dst = (unsigned char*)dst_ptr;
    const double* darr = ((const double*)arr);
    for (i = 0; i < h*w; ++i) {
      if (darr[i]<darr[mini]) {
        mini = i;
      } else if (darr[i]>darr[maxi]) {
        maxi = i;
      }
    }
    
    for (i = 0; i < h; ++i) {
      unsigned char* dst1 = dst + (h - 1 - i) * dststep;
      const double* srcd = (const double*)(arr + i * step);
      
      for (j = 0; j < w; ++j) {
        int t = (int)((srcd[j] - darr[mini]) * 256 / (darr[maxi] - darr[mini]));
        if (t>255) {
          t = 255;
        }else if (t<0) {
          t = 0;
        }
        dst1[j] = t;
      }
    }
  }
  else if (2 == cn) {
    int dststep = (w + 3) & -4, mini = 0, maxi = 0;
    unsigned char* dst = (unsigned char*)dst_ptr;
    const short* darr = ((const short*)arr);
    for (i = 0; i < h*w; ++i) {
      if (darr[i]<darr[mini]) {
        mini = i;
      } else if (darr[i]>darr[maxi]) {
        maxi = i;
      }
    }
    
    for (i = 0; i < h; ++i) {
      unsigned char* dst1 = dst + (h - 1 - i) * dststep;
      const double* srcd = (const double*)(arr + i * step);
      
      for (j = 0; j < w; ++j) {
        int t = (int)((srcd[j] - darr[mini]) * 256 / (darr[maxi] - darr[mini]));
        if (t>255) {
          t = 255;
        }else if (t<0) {
          t = 0;
        }
        dst1[j] = t;
      }
    }
  }
  else {
    int dststep = (w * cn + 3) & -4;
    unsigned char* dst = (unsigned char*)dst_ptr;
    dst += (h - 1) * dststep;
    
    //IM2IM( h, w, arr, step, cn, dst, -dststep, channels );
    for (i = 0; i < h; ++i) {
      memcpy(dst + i * (-dststep), arr + i * step, w * cn);
    }
    
    if (1 == cn) {
      int s = 0;
      //SKIP_MAX_ELEMENT(height, width, arr, step, cn, s);
      for (i = 0; i < h; ++i) {
        const unsigned char* srcd = (const unsigned char*)(arr + i * step);
        
        for (j = 0; j < w; ++j) {
          if (srcd[j]>s) {
            s = srcd[j];
          }
        }
      }
      dst = (unsigned char*)dst_ptr;
      
      if (1 == arr[s]) {
        for (i = 0; i < h * dststep; ++i) {
          dst[i] = dst[i] ? 255 : 0;
        }
      }
    }
  }

  //BitBlt(hDC, 0, 0, width, height, hDC, x, y, SRCCOPY);
  return 0;
}

#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

typedef struct waveout_t waveout_t;
typedef int (*wavegetdata_f)(waveout_t* w, short* out, int len);

#define MAXWAVEBUFFER 4096

typedef struct waveout_t {
  char pBuffer1[2*MAXWAVEBUFFER];
  WAVEHDR pWaveHdr1[2];
  HWAVEOUT hWaveOut;
  HANDLE hCallBackEvent;
  const void* data;
  int len;
  int bit;
  int fs;
  int pos;
  //int stop;
  wavegetdata_f getdata;
} waveout_t;

static int _wav_getdata(waveout_t* w, short* out) {
  if (w) {
    int i, len = MAXWAVEBUFFER/2;
    if ((len + w->pos) > w->len) {
      len = w->len - w->pos;
    }
    if (w->getdata) {
      len = w->getdata(w, out, len);
    } else if (w->data) {
      if (16==w->bit) {
        memcpy(out, ((short*)w->data) + w->pos, len*sizeof(short));
      } else {
        const unsigned char* pc = ((const unsigned char*)w->data) + w->pos;
        for (i=0; i<len; ++i) {
          out[i] = (pc[i] * 256) - 32768;
        }
      }
    }
    w->pos += len;
    printf("pos = %d\n", w->pos);
    if (w->pos>=w->len) {
      //w->stop = 1;
      SetEvent(w->hCallBackEvent);
    }
  }
  return 0;
}

static int CALLBACK waveOutProc(HWAVEOUT hWaveOut,UINT uMsg, DWORD_PTR dwInstance,DWORD dwParam1,DWORD dwParam2)
{
  LPWAVEHDR pWaveHeader = (LPWAVEHDR)dwParam1;
  waveout_t* w = (waveout_t*)dwInstance;
  switch (uMsg) {
  case MM_WOM_OPEN:
    /* FillBuffer (pBuffer1, iFreq);
    waveOutWrite (hWaveOut, pWaveHdr1, sizeof (WAVEHDR));
    FillBuffer (pBuffer2, iFreq);
    waveOutWrite (hWaveOut, pWaveHdr2, sizeof (WAVEHDR));*/
    return TRUE;
  case MM_WOM_DONE:
    waveOutUnprepareHeader(hWaveOut, pWaveHeader, sizeof(WAVEHDR));
    _wav_getdata(w, (short*)pWaveHeader->lpData);
    waveOutPrepareHeader(hWaveOut, pWaveHeader, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, pWaveHeader, sizeof(WAVEHDR));
    return TRUE;
  case MM_WOM_CLOSE:
    waveOutUnprepareHeader(hWaveOut, w->pWaveHdr1, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, w->pWaveHdr1+1, sizeof(WAVEHDR));
    hWaveOut = NULL;
    return TRUE;
  default:
    return TRUE;
  }
}
static int wavplay(const void* data, int len, int fs, int bit, wavegetdata_f getdata) {
  waveout_t w[1] = {0};
  WAVEFORMATEX waveformat[1];
  int i, ret, nChannels = 1;
  int outbit = 16;
  w->hCallBackEvent = CreateEvent( NULL, TRUE, FALSE, "callBackOut" );
  if (fs<1) {
    fs = 11025;
  }
  if (bit<1) {
    bit = 16;
  }
  w->data = data;
  w->len = len;
  w->fs = fs;
  w->bit = bit;
  w->getdata = getdata;
  waveformat->wFormatTag = WAVE_FORMAT_PCM;
  waveformat->nChannels = nChannels;
  waveformat->nSamplesPerSec = fs;
  waveformat->wBitsPerSample = outbit;
  waveformat->nBlockAlign = (outbit*nChannels)>>3;
  waveformat->nAvgBytesPerSec = waveformat->nBlockAlign * fs;
  waveformat->cbSize = 0;

  ret = waveOutOpen(&w->hWaveOut, WAVE_MAPPER, waveformat, (DWORD)waveOutProc, (DWORD)w, CALLBACK_FUNCTION);
  if (ret != MMSYSERR_NOERROR) {
    return 0;
  }
  for (i=0;i<2;i++) {
    WAVEHDR* waveHdr = w->pWaveHdr1+i;
    waveHdr->lpData = (char*)w->pBuffer1 + i*MAXWAVEBUFFER;
    waveHdr->dwBufferLength = MAXWAVEBUFFER;
    waveHdr->dwBytesRecorded = 0;
    waveHdr->dwUser = 0;
    waveHdr->dwFlags = 0;
    waveHdr->dwLoops = 1;
    waveHdr->lpNext = 0;//i==0 ? NULL : (w->pWaveHdr1+i-1);
    waveHdr->reserved = 0;
    waveOutPrepareHeader(w->hWaveOut, waveHdr, sizeof(WAVEHDR));
  }
  for (i=0;i<2;i++) {
    _wav_getdata(w, (short*)(w->pBuffer1 + i*MAXWAVEBUFFER));
    waveOutWrite(w->hWaveOut, w->pWaveHdr1+i, sizeof(WAVEHDR));
  }
  //getchar();
  WaitForSingleObject(w->hCallBackEvent, INFINITE);
  ReleaseMutex(w->hCallBackEvent);
  //while(!w->stop) {    Sleep(10);  }
  waveOutClose(w->hWaveOut);
  return 0;
}

typedef struct wavein_t {
  MMRESULT mmError;
  HWAVEIN hWaveIn;
  HANDLE hCallBackEvent;
  WAVEFORMATEX waveFmt[1]; /* Pointer to PCMWAVEFORMAT */
  char pBuffer1[2*MAXWAVEBUFFER];
  WAVEHDR pWaveHdr1[2];
  short* data;
  int pos;
  int len;
} wavein_t;

static int _wav_setdata(wavein_t* w, const void* inwav) {
  int len = MAXWAVEBUFFER/2;
  short* data = w->data;
  len = min(len, w->len-w->pos);
  memcpy(data + w->pos, inwav, len*2);
  w->pos += len;
  if (w->pos>=w->len) {
    //w->stop = 1;
    SetEvent(w->hCallBackEvent);
  }
  return 0;
}

int CALLBACK callBackIn(HWAVEIN hWaveIn, UINT msg, DWORD_PTR magic, LPARAM dwParam1, LPARAM dwParam2)
{
  wavein_t* w = (wavein_t*)magic;
  LPWAVEHDR curHdr = (LPWAVEHDR)dwParam1;
  MMRESULT result;
  
  if (w==NULL) return 0;
  
  switch(msg) {
  case MM_WIM_OPEN:
    return TRUE;
  case MM_WIM_DATA:
    //EnterCriticalSection(&w->c);
    //录音设备返回数据时，触发该事件
    if(!waveInUnprepareHeader(hWaveIn, curHdr,sizeof(WAVEHDR))) {//可以释放内存。有的情况虽然返回数据，但不能释放内存，
      //如使用同一个WAVEHDR增加多个内存块时，会出现上述情况
      //memcpy(buffer, curHdr->lpData, MAXWAVEBUFFER);//复制返回数据
      //buf_len = curHdr->dwBytesRecorded;//记录录音数据长度
      _wav_setdata(w, curHdr->lpData);
      curHdr->dwUser=0;//恢复设置
      curHdr->dwBufferLength=MAXWAVEBUFFER;
      curHdr->dwFlags=0;
      //if(m_Opened)
      result=waveInPrepareHeader(hWaveIn,curHdr,sizeof(WAVEHDR));
      result=waveInAddBuffer(hWaveIn,curHdr,sizeof(WAVEHDR));
      
    }
    //LeaveCriticalSection(&w->c);
    return TRUE;
  case MM_WIM_CLOSE:
    //SetEvent(w->callBackEvent);
    return TRUE;
  default:
    break;
  }
  return TRUE;
}

static int wavrecord(void* data, int len, int fs, int bit, wavegetdata_f getdata) {
  int i=0, nChannels = 1;
  int nBlockAlign = (bit*nChannels)>>3;
  wavein_t w[1] = {0};
  /* Set up required format */
  w->waveFmt->wFormatTag = WAVE_FORMAT_PCM;
  w->waveFmt->nChannels = nChannels;
  w->waveFmt->nSamplesPerSec = fs;
  w->waveFmt->nBlockAlign = nBlockAlign;
  w->waveFmt->wBitsPerSample = bit;
  w->waveFmt->nAvgBytesPerSec = nBlockAlign * fs;
  w->waveFmt->cbSize = 0;
  w->len = len;
  w->data = (short*)data;
  
  /* Set up position query */
  //w->wavePos->wType = TIME_SAMPLES;
  
  //InitializeCriticalSection( &(w->c) );
  w->hCallBackEvent = CreateEvent( NULL, TRUE, FALSE, "callBackIn" );
  
  //ret = waveOutOpen(&wo->hWaveOut, WAVE_MAPPER, waveformat, (DWORD)waveOutProc, (DWORD)wo, CALLBACK_FUNCTION);
  if ((w->mmError=waveInOpen(NULL, WAVE_MAPPER, w->waveFmt, 0, 0, WAVE_FORMAT_QUERY))!=MMSYSERR_NOERROR) {
    printf("Requested data format is not supported [ERR=%d]",w->mmError);
  }
  if ((w->mmError=waveInOpen(&w->hWaveIn, WAVE_MAPPER, w->waveFmt, (DWORD_PTR)callBackIn, (DWORD_PTR)w,CALLBACK_FUNCTION))!=MMSYSERR_NOERROR) {
    printf("Cannot open MMAPI audio input [ERR=%d]",w->mmError);
  }
  for (i=0;i<2;i++) {
    WAVEHDR* waveHdr = w->pWaveHdr1+i;
    /* Set up header */
    waveHdr->lpData = w->pBuffer1 + i*MAXWAVEBUFFER;
    waveHdr->dwBufferLength = MAXWAVEBUFFER;
    waveHdr->dwBytesRecorded = 0;
    waveHdr->dwUser = 0;
    waveHdr->dwFlags = 0;
    waveHdr->dwLoops = 1;
    waveHdr->lpNext = i==0 ? NULL : (w->pWaveHdr1+i-1);
    if ((w->mmError=waveInPrepareHeader(w->hWaveIn, waveHdr, sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
      printf("Header preparation failed");
    }
    
    if ((w->mmError=waveInAddBuffer(w->hWaveIn, waveHdr, sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
      printf("Cannot add input buffer %d",w->mmError);
    }
  }
  
  if ((w->mmError=waveInStart( w->hWaveIn ))!=MMSYSERR_NOERROR) {
    printf("Cannot start MMAPI input audio port [ERR=%d]",w->mmError);
  }
  WaitForSingleObject(w->hCallBackEvent, INFINITE);
  ReleaseMutex(w->hCallBackEvent);
  waveInStop(w->hWaveIn);
  waveInClose(w->hWaveIn);
  return 0;
}

#endif // _WIN32DEF_H_
