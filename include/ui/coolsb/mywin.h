
typedef int BOOL;
#define FALSE 0
#define TRUE 1
typedef void* HWND;
typedef void* HRESULT;
typedef void* HBITMAP;
typedef void* HENHMETAFILE;
typedef void* HCURSOR;
typedef void* HDC;
#define WINAPI

typedef unsigned int UINT;
typedef int* LPINT;

typedef struct tagSCROLLINFO {
  UINT cbSize;
  UINT fMask;
  int nMin;
  int nMax;
  UINT nPage;
  int nPos;
  int nTrackPos;
} SCROLLINFO, *LPSCROLLINFO;

typedef unsigned int UINT_PTR, *PUINT_PTR;

typedef struct tagNMHDR
{
  HWND      hwndFrom;
  UINT_PTR  idFrom;
  UINT      code;         // NM_ code
} NMHDR;

#define NEAR
#define FAR
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef NMHDR FAR * LPNMHDR;
typedef long LONG;

typedef struct tagRECT
{
  LONG    left;
  LONG    top;
  LONG    right;
  LONG    bottom;
} RECT, *PRECT, NEAR *NPRECT, FAR *LPRECT;

#define OUT
#define IN

BOOL SetRect(OUT LPRECT lprc, IN int xLeft, IN int yTop, IN int xRight, IN int yBottom) {
  lprc->left = xLeft, lprc->top = yTop, lprc->right = xRight, lprc->bottom = yBottom;
  return 0;
}

typedef struct tagPOINT
{
  LONG  x;
  LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;

typedef LONG LONG_PTR;

typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;

#define CALLBACK
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);



#define SB_HORZ             0
#define SB_VERT             1
#define SB_CTL              2
#define SB_BOTH             3

#define SB_LINEUP           0
#define SB_LINELEFT         0
#define SB_LINEDOWN         1
#define SB_LINERIGHT        1
#define SB_PAGEUP           2
#define SB_PAGELEFT         2
#define SB_PAGEDOWN         3
#define SB_PAGERIGHT        3
#define SB_THUMBPOSITION    4
#define SB_THUMBTRACK       5
#define SB_TOP              6
#define SB_LEFT             6
#define SB_BOTTOM           7
#define SB_RIGHT            7
#define SB_ENDSCROLL        8


#define ESB_ENABLE_BOTH     0x0000
#define ESB_DISABLE_BOTH    0x0003

#define ESB_DISABLE_LEFT    0x0001
#define ESB_DISABLE_RIGHT   0x0002

#define ESB_DISABLE_UP      0x0001
#define ESB_DISABLE_DOWN    0x0002

#define ESB_DISABLE_LTUP    ESB_DISABLE_LEFT
#define ESB_DISABLE_RTDN    ESB_DISABLE_RIGHT




#define DFC_CAPTION             1
#define DFC_MENU                2
#define DFC_SCROLL              3
#define DFC_BUTTON              4
#if(WINVER >= 0x0500)
#define DFC_POPUPMENU           5
#endif /* WINVER >= 0x0500 */

#define DFCS_CAPTIONCLOSE       0x0000
#define DFCS_CAPTIONMIN         0x0001
#define DFCS_CAPTIONMAX         0x0002
#define DFCS_CAPTIONRESTORE     0x0003
#define DFCS_CAPTIONHELP        0x0004

#define DFCS_MENUARROW          0x0000
#define DFCS_MENUCHECK          0x0001
#define DFCS_MENUBULLET         0x0002
#define DFCS_MENUARROWRIGHT     0x0004
#define DFCS_SCROLLUP           0x0000
#define DFCS_SCROLLDOWN         0x0001
#define DFCS_SCROLLLEFT         0x0002
#define DFCS_SCROLLRIGHT        0x0003
#define DFCS_SCROLLCOMBOBOX     0x0005
#define DFCS_SCROLLSIZEGRIP     0x0008
#define DFCS_SCROLLSIZEGRIPRIGHT 0x0010

#define DFCS_BUTTONCHECK        0x0000
#define DFCS_BUTTONRADIOIMAGE   0x0001
#define DFCS_BUTTONRADIOMASK    0x0002
#define DFCS_BUTTONRADIO        0x0004
#define DFCS_BUTTON3STATE       0x0008
#define DFCS_BUTTONPUSH         0x0010

#define DFCS_INACTIVE           0x0100
#define DFCS_PUSHED             0x0200
#define DFCS_CHECKED            0x0400

#if(WINVER >= 0x0500)
#define DFCS_TRANSPARENT        0x0800
#define DFCS_HOT                0x1000
#endif /* WINVER >= 0x0500 */

#define DFCS_ADJUSTRECT         0x2000
#define DFCS_FLAT               0x4000
#define DFCS_MONO               0x8000

HDC CreateCompatibleDC( IN HDC) {
  return 0;
}

