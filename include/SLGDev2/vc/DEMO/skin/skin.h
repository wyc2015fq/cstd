#ifndef _SKIN_H_
#define _SKIN_H_
//窗口子类基类 头文件
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  // for TTF
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500
#endif // _WIN32_WINDOWS
#ifdef _DEBUG
#define SKINFILE  "F:\\pub\\skin\\Vista3\\theme.ini"
#else
#define SKINFILE  ".\\theme.ini"
#endif
#include <windows.h>
#include <assert.h>
// 控件类型
enum {
  SW_CSkinButton = 0,           //
  SW_CSkinStatic,           //
  SW_CSkinGroupBox,           //
  SW_CSkinEdit,           //
  SW_CEditWnd,           //
  SW_CXPEdit,           //
  SW_CSkinWin,           //
  SW_CSkinOutlookBar,           // 
  SW_CSkinListVeiw,
  SW_CSkinHeaderCtrl2,
  SW_CSkinVerticleScrollbar2,           // 
  SW_CSkinHorizontalScrollbar2,           // 
  SW_CCreditsCtrl,           // 
  SW_CPPToolTip,           // 
  SW_CScrollWnd,           // 
  SW_CSkinScrollBar,           // 
  SW_CSkinScrollWnd,           // 
  SW_SCROLLWND,           // 
  SW_CButtonST,           // 
  SW_CSuperGridCtrl,           // 
  //SW_WINDOW, // 
};
// 
enum {
  SW_PUSHBUTTON = 0,         // 0 
  SW_CHECKBUTTON,           // 1 
  SW_RADIOBUTTON,           // 2 
  SW_GROUPBOX,              // 3 
};
typedef struct CSubclassWnd CSubclassWnd;
typedef BOOL ( *Init_f ) ( struct CSubclassWnd* win );
typedef BOOL ( *LoadSkinRes_f ) ( struct CSubclassWnd* win, const TCHAR * skinfile );
typedef LRESULT ( *WindowProc_f ) ( struct CSubclassWnd* win, UINT msg, WPARAM wp, LPARAM lp );
typedef struct CSubclassWndVftab {
  int m_nSize;
  Init_f Init;
  LoadSkinRes_f LoadSkinRes;
  WindowProc_f WindowProc;
}
CSubclassWndVftab;
//state
enum { ST_NOMOR = 0,      //0 
	       ST_HOVER,        //1 
       ST_PRESS,        //2 
       ST_DISABLED,      //3 
       ST_FOCUS    //4 
     };
typedef struct TrackMouse {
  //按钮的状态
  BOOL m_bOver;       //
  BOOL m_bTracking;   //
  BOOL m_bSelected;   //
  BOOL m_bFocus;      //
  BOOL m_bEnable;     //
  BOOL m_bSelectOver; //
  POINT m_pt;
}
TrackMouse;
#define MOUSE_OVER(THIS)       (THIS)->m_tmTrack.m_bOver
#define MOUSE_TRACKING(THIS)   (THIS)->m_tmTrack.m_bTracking
#define MOUSE_SELECTED(THIS)   (THIS)->m_tmTrack.m_bSelected
#define MOUSE_FOCUS(THIS)      (THIS)->m_tmTrack.m_bFocus
#define MOUSE_ENABLE(THIS)     (THIS)->m_tmTrack.m_bEnable
#define MOUSE_SELECTOVER(THIS) (THIS)->m_tmTrack.m_bSelectOver
#define MOUSE_STAGE(THIS)      mouse_stage(&(THIS)->m_tmTrack)
#define CSUBCLASSWNDDEF  CSubclassWndVftab* m_vftab;\
  HWND m_hWnd; DWORD m_dStyle; DWORD m_dState; WNDPROC m_pOldWndProc;\
  TrackMouse m_tmTrack; CSubclassWnd* m_pNext
typedef struct CSubclassWnd {
  CSUBCLASSWNDDEF;
}
CSubclassWnd;
#define SW_UNKNOWN  -1 // 
///////////////////////////////////////////////////////////////////////////////
// 按钮皮肤控件
typedef struct CSkinButton {
  CSUBCLASSWNDDEF;
  HBITMAP m_bmpPush;   //PushButton Bitmap
  HBITMAP m_bmpCheck;  //CheckButton Bitmap
  HBITMAP m_bmpRadio;  //RadioButton Bitmap
  COLORREF m_crText;
  COLORREF m_crDisableText;
  COLORREF m_crWindow;
  COLORREF m_crTransparent;
  COLORREF m_crBorder;
  int m_nCheckState;
  int m_nRadioState;
  BOOL m_bDrawFocus; //
}
CSkinButton;
///////////////////////////////////////////////////////////////////////////////
// 窗口皮肤控件
typedef struct CSkinWin {
  CSUBCLASSWNDDEF;
  HBITMAP m_bmpCaption;       // 
  HBITMAP m_bmpLeftBorder;    //
  HBITMAP m_bmpRightBorder;   //
  HBITMAP m_bmpBottomBorder;  //
  HBITMAP m_bmpMaxButton;     //
  HBITMAP m_bmpMinButton;     //
  HBITMAP m_bmpCloseButton;   //
  HBITMAP m_bmpRestoreButton; //
  HBITMAP m_bmpHelpButton;    //
  int m_CaptionHeight;        //
  int m_BorderLeftWidth;      //
  int m_BorderRightWidth;     //
  int m_BorderBottomHeight;   //
  int m_nGray;                //
  COLORREF m_CaptionColor;
  COLORREF m_CaptionGrayColor;
  COLORREF m_WindowColor;
  COLORREF m_TransparentColor;
  UINT m_downHitTest;
  // UINT m_moveHitTest;
  UINT m_oldHitTest;
  UINT m_bOver; //0 - not in window    1-client area   2-not_client area
  BOOL m_bTracking;
  BOOL m_bSizable ; //
  BOOL m_bMinable ; //
  BOOL m_bMaxable ; //
  BOOL m_bHelp;     //
  BOOL m_bSysmenu ; //
  BOOL m_bClose;
  BOOL m_bCaption ; //
  BOOL m_bFrame;    //
  BOOL m_bActive;
  BOOL m_InitialShow; //
  HICON m_hIcon;
  HRGN m_hRgn;
  TCHAR m_szWinText[ MAX_PATH ];
}
CSkinWin;
typedef struct CSkinToolTips {
  CSUBCLASSWNDDEF;
  BOOL m_bTrans;
  COLORREF m_crText;
  COLORREF m_crDisableText;
  COLORREF m_crWindow;
  int nOldWidth, nOldHeight;
}
CSkinToolTips;
///////////////////////////////////////////////////////////////////////////////
// OutlookBar 控件
#define OB_MAXITEMNUM  (100)
#define WM_OUTBAR_NOTIFY           WM_USER + 1
#define NM_OB_ITEMCLICK            1
#define NM_OB_ONLABELENDEDIT       2
#define NM_OB_ONGROUPENDEDIT       3
#define NM_OB_DRAGITEM             4
#define NM_FOLDERCHANGE            5
typedef struct CSkinOutlookBar {
  CSUBCLASSWNDDEF;
  HBITMAP m_bmpSkin;          //
  RECT m_rcSkin;              //
  int m_nSelectedItem;        //
  int m_nSelectedFolderItem;  //
  int m_nItemNumber;          //
  int m_nItemHeight;          //
  unsigned char m_bLevel[ OB_MAXITEMNUM ]; //
  unsigned char m_bShowIndex[ OB_MAXITEMNUM ];
  BOOL m_bOnlyOneExpend;      //
  int m_nShowIndexLen;
  int m_nHitOn;
  COLORREF m_crText;
  COLORREF m_crDisableText;
  COLORREF m_crWindow;
}
CSkinOutlookBar;
typedef struct CSkinStatic {
  CSUBCLASSWNDDEF;
  HBITMAP m_bmpStaticBackGround;    //
  COLORREF m_crText;
  COLORREF m_crDisableText;
  COLORREF m_crWindow;
  COLORREF m_crLinkColour, m_crVisitedColour;     // Hyperlink colours
  COLORREF m_crHoverColour;                       // Hover colour
  BOOL m_bOverControl;                        // cursor over control?
  BOOL m_bVisited;                            // Has it been visited?
  BOOL m_bAdjustToFit;                        // Adjust window size to fit text?
  int m_nUnderline;                          // underline hyperlink?
  TCHAR m_strURL[ _MAX_PATH ];                 // hyperlink URL
  HCURSOR m_hLinkCursor;                         // Cursor for hyperlink
}
CSkinStatic;
enum UnderLineOptions { ulHover = -1, ulNone = 0, ulAlways = 1};
typedef struct CHyperLink {
  CSUBCLASSWNDDEF;
  COLORREF m_crLinkColour, m_crVisitedColour;     // Hyperlink colours
  COLORREF m_crHoverColour;                       // Hover colour
  BOOL m_bOverControl;                        // cursor over control?
  BOOL m_bVisited;                            // Has it been visited?
  BOOL m_bAdjustToFit;                        // Adjust window size to fit text?
  int m_nUnderline;                          // underline hyperlink?
  TCHAR m_strURL[ _MAX_PATH ];                 // hyperlink URL
  HCURSOR m_hLinkCursor;                         // Cursor for hyperlink
  HFONT m_UnderlineFont;                       // Font for underline display
  HFONT m_StdFont;                             // Standard font
}
CHyperLink;
typedef struct CSkinEdit {
  CSUBCLASSWNDDEF;
  HBITMAP m_bmpEditBackGround;    //
  COLORREF m_crText;
  COLORREF m_crDisableText;
  COLORREF m_crBorder;
  COLORREF m_crMainDialog;
  COLORREF m_crWindow;
}
CSkinEdit;
typedef struct CXPEdit {
  CSUBCLASSWNDDEF;
  HBITMAP m_bmpEditBackGround;    //
  COLORREF m_crText;
  COLORREF m_crDisableText;
  COLORREF m_crBorder;
  COLORREF m_crMainDialog;
  COLORREF m_crWindow;
}
CXPEdit;
typedef struct CSkinGroupBox {
  CSUBCLASSWNDDEF;
  COLORREF m_crLine; // 
  COLORREF m_crLineShade; //
  BOOL m_bDrawLineShade; //
}
CSkinGroupBox;
///////////////////////////////////////////////////////////////////////////////
// CPPToolTip 提示控件
#define PPTOOLTIP_USE_SHADE
#define PPTOOLTIP_CLASSNAME    _T("CPPTooltip")  // Window class name
#define UDM_TOOLTIP_FIRST             (WM_USER + 100)
#define UDM_TOOLTIP_DISPLAY           (UDM_TOOLTIP_FIRST) //User was changed the data
//The styles
#define PPTOOLTIP_ANCHOR                   0x00000001
#define PPTOOLTIP_SHADOW                   0x00000002
#define PPTOOLTIP_ROUNDED                  0x00000004
#define PPTOOLTIP_BALLOON                  0x00000007
#define PPTOOLTIP_VCENTER_ALIGN            0x00000008
#define PPTOOLTIP_BOTTOM_ALIGN             0x00000010
#define PPTOOLTIP_ICON_VCENTER_ALIGN       0x00000020
#define PPTOOLTIP_ICON_BOTTOM_ALIGN        0x00000040
#define PPTOOLTIP_SHOW_INACTIVE            0x00000100 //Show tooltip for inacive window
#define PPTOOLTIP_SHOW_DISABLED            0x00000200 //Show tooltip for disabled control
//The behaviours
#define PPTOOLTIP_MULTIPLE_SHOW            0x00000001 //Multiple show for single control
#define PPTOOLTIP_TRACKING_MOUSE           0x00000002 //Tracking for mouse
#define PPTOOLTIP_CLOSE_LEAVEWND           0x00000004 //Close tooltip if mouse leave the control
#define PPTOOLTIP_NOCLOSE_OVER             0x00000008 //No close tooltip if mouse over him
#define PPTOOLTIP_DISABLE_AUTOPOP          0x00000010 //Disables autopop tooltip from timer
//#define PPTOOLTIP_NO_CLOSE_OVER        0x00000002 //No closes tooltip if the mouse over the control
//#define PPTOOLTIP_NO_CLOSE_MOVE        0x00000004 //No closes for any mouse's move.
//#define PPTOOLTIP_TRACK_MOUSE          0x00000008 //Tracking window for the mouse's move
//The masks
#define PPTOOLTIP_MASK_STYLES              0x0001    // The styles for the tooltip gets from the structures
#define PPTOOLTIP_MASK_EFFECT              0x0002    // The background's type for the tooltip gets from the structures
#define PPTOOLTIP_MASK_COLORS              0x0004    // The background's colors for the tooltip gets from the structures
#define PPTOOLTIP_MASK_DIRECTION           0x0008  // The align for the tooltip gets from the structures
#define PPTOOLTIP_MASK_BEHAVIOUR           0x0010  // The behaviour for the tooltip gets from the structures 
//#define PPTOOLTIP_MASK_GRANULARITY        0x0010    // The effect's granularity
enum { PPTTSZ_ROUNDED_CX = 0,
       PPTTSZ_ROUNDED_CY,
       PPTTSZ_MARGIN_CX,
       PPTTSZ_MARGIN_CY,
       PPTTSZ_SHADOW_CX,
       PPTTSZ_SHADOW_CY,
       PPTTSZ_WIDTH_ANCHOR,
       PPTTSZ_HEIGHT_ANCHOR,
       PPTTSZ_MARGIN_ANCHOR,
       PPTTSZ_BORDER_CX,
       PPTTSZ_BORDER_CY,
       PPTTSZ_MAX_SIZES
     };
enum { TYPE_RES_ICON = 0,
       TYPE_RES_BITMAP,
       TYPE_RES_MASK_BITMAP,
       MAX_TYPES_RES
     };
enum { PPTOOLTIP_COLOR_0 = 0,
       PPTOOLTIP_COLOR_1,
       PPTOOLTIP_COLOR_2,
       PPTOOLTIP_COLOR_3,
       PPTOOLTIP_COLOR_4,
       PPTOOLTIP_COLOR_5,
       PPTOOLTIP_COLOR_6,
       PPTOOLTIP_COLOR_7,
       PPTOOLTIP_COLOR_8,
       PPTOOLTIP_COLOR_9,
       PPTOOLTIP_COLOR_10,
       PPTOOLTIP_COLOR_11,
       PPTOOLTIP_COLOR_12,
       PPTOOLTIP_COLOR_13,
       PPTOOLTIP_COLOR_14,
       PPTOOLTIP_COLOR_15,
       PPTOOLTIP_COLOR_16,
       PPTOOLTIP_COLOR_FG,
       PPTOOLTIP_COLOR_BK_BEGIN,
       PPTOOLTIP_COLOR_BK_MID,
       PPTOOLTIP_COLOR_BK_END,
       PPTOOLTIP_COLOR_LINK,                  // Color for hyperlink text
       PPTOOLTIP_COLOR_VISITED,              // Color for visited hyperlink text
       PPTOOLTIP_COLOR_HOVER,                  // Color for hover hyperlink text
       PPTOOLTIP_COLOR_SHADOW,                  // Color for the shadow
       PPTOOLTIP_COLOR_BORDER,                  // Color for border of the tooltip
       PPTOOLTIP_MAX_COLORS
     };
enum { PPTOOLTIP_LEFT_TOP = 0,
       PPTOOLTIP_RIGHT_TOP,
       PPTOOLTIP_LEFT_BOTTOM,
       PPTOOLTIP_RIGHT_BOTTOM,
       PPTOOLTIP_MAX_DIRECTIONS
     };
enum { PPTOOLTIP_EFFECT_SOLID = 0,
       PPTOOLTIP_EFFECT_HGRADIENT,
       PPTOOLTIP_EFFECT_VGRADIENT,
       PPTOOLTIP_EFFECT_HCGRADIENT,
       PPTOOLTIP_EFFECT_VCGRADIENT,
       PPTOOLTIP_EFFECT_3HGRADIENT,
       PPTOOLTIP_EFFECT_3VGRADIENT,
#ifdef PPTOOLTIP_USE_SHADE
       PPTOOLTIP_EFFECT_NOISE,
       PPTOOLTIP_EFFECT_DIAGSHADE,
       PPTOOLTIP_EFFECT_HSHADE,
       PPTOOLTIP_EFFECT_VSHADE,
       PPTOOLTIP_EFFECT_HBUMP,
       PPTOOLTIP_EFFECT_VBUMP,
       PPTOOLTIP_EFFECT_SOFTBUMP,
       PPTOOLTIP_EFFECT_HARDBUMP,
       PPTOOLTIP_EFFECT_METAL,
#endif
       PPTOOLTIP_MAX_EFFECTS
     };
enum { PPTOOLTIP_SHOW = 0x100,           //the identifier of the timer for show the tooltip
       PPTOOLTIP_HIDE = 0x101  //the identifier of the timer for hide the tooltip
     };
enum { PPTOOLTIP_TOOL_NOEXIST = -1,           //the current tool is not exist
       PPTOOLTIP_TOOL_HELPER = -2   //the current tool is a helper
     };
// This info structure
typedef struct tagPPTOOLTIP_INFO {
  HWND hWnd;             // Window handle of the control
  UINT nIDTool;          // ID of tool
  RECT rectBounds;       // Bounding rect for toolinfo to be displayed
  HICON hIcon;            // The icon of the tooltip
  const TCHAR* sTooltip;         // The string of the tooltip
  UINT nMask;            // The mask
  UINT nStyles;          // The tooltip's styles
  UINT nDirection;       // Direction display the tooltip relate cursor point
  UINT nEffect;          // The color's type or effects
  UINT nBehaviour;       // The tooltip's behaviour
  BYTE nGranularity;     // The effect's granularity
  COLORREF crBegin;          // Begin Color
  COLORREF crMid;            // Mid Color
  COLORREF crEnd;            // End Color
}
PPTOOLTIP_INFO;
typedef struct tagPPTOOLTIP_NAME_RES {
  const TCHAR* sName;    // Name of the resource (IDB_ - Bitmap, IDI_ - Icon)
  UINT nID;         // ID of the resource
  BYTE nTypeRes;    // TYPE_RES_ICON = Icon, TYPE_RES_BITMAP = bitmap, TYPE_RES_TRAN_BITMAP = transparent bitmap
  COLORREF crMask;  // Color mask of the resource
}
PPTOOLTIP_NAME_RES;
// This structure sent to PPToolTip parent in a WM_NOTIFY message
typedef struct tagNM_PPTOOLTIP_DISPLAY {
  NMHDR hdr;
  POINT * pt;
  PPTOOLTIP_INFO * ti;
}
NM_PPTOOLTIP_DISPLAY;
/////////////////////////////////////////////////////////////////////////////
// CPPToolTip window
#define VECTORDEF(_TYPE, _NAME)  _TYPE* _NAME; int _NAME##Len
#define STATICVECTORDEF(_TYPE, _NAME, _N)  _TYPE _NAME[_N]; int _NAME##Len
typedef struct CPPToolTip {
  CSUBCLASSWNDDEF;
  VECTORDEF ( PPTOOLTIP_INFO, m_arrTools );  //Tools Archive
  STATICVECTORDEF ( PPTOOLTIP_NAME_RES, m_arrNameRes, 100 );  //Names of the resource
  STATICVECTORDEF ( UINT, m_nLengthLines, 100 );  //The real length of the lines in the tooltip
  STATICVECTORDEF ( UINT, m_nHeightLines, 100 );  //The real heigth of the lines in the tooltip
  // Attributes
  //    typedef CArray<PPTOOLTIP_HOTRECT, PPTOOLTIP_HOTRECT&> m_arrToolsWnd;
  //    CMap <HWND, HWND, m_arrToolsWnd, m_arrToolsWnd&> m_mapTools;
  HWND m_hNotifyWnd; // Handle to window for notification about change data
  HWND m_pParentWnd; // The pointer to the parent window
  UINT m_nLastDirection;
  int m_nIndexCurrentWnd;
  int m_nIndexDisplayWnd;
  LOGFONT m_LogFont;                  // Current font in use
  //Default setting
  COLORREF m_crColor [ PPTOOLTIP_MAX_COLORS ]; //The indexing colors
  UINT m_nSizes [ PPTTSZ_MAX_SIZES ]; //All sizes
  UINT m_nStyles;
  UINT m_nDirection;
  UINT m_nEffect;
  UINT m_nGranularity;
  UINT m_nBehaviour;     //The tooltip's behaviour
  UINT m_nTimeAutoPop; //Retrieve the length of time the tool tip window remains visible if the pointer is stationary within a tool's bounding rectangle
  UINT m_nTimeInitial; //Retrieve the length of time the pointer must remain stationary within a tool's bounding rectangle before the tool tip window appears
  //The properties of the current tooltip
  POINT m_ptOriginal;
  HRGN m_rgnToolTip;
  HRGN m_rgnShadow;
  SIZE m_szToolIcon; //the size of the current icon
  SIZE m_szTextTooltip; //the size of the tooltip's text
  // HIMAGELIST m_imgTooltip; //the image list
  struct _IMAGELIST* m_imgTooltip; //the image list
  SIZE m_szImage; // Dimensions of each image, in pixels.
  PPTOOLTIP_INFO m_pToolInfo; //info of the current tooltip
  RECT m_ShadeRect;
}
CPPToolTip;
///////////////////////////////////////////////////////////////////////////////
typedef struct CEditWnd {
  CSUBCLASSWNDDEF;
  TCHAR m_strEdit[ 256 ];  //
  POINT m_ptCursor;    //
  POINT m_ptTxtBegin;  //
  POINT m_ptFirst;     //
  POINT m_ptSecond;    //
  int m_nCharPos;      //
  int m_nShowChar;     //
  int m_nMaxShowChars; //
  int m_nSelectBegin;  //
  int m_nSelectEnd;    //
  HBITMAP m_bmpBK;  //
  TEXTMETRIC m_tm;  //
  BOOL m_bLBtDown;  //
  int m_nShiftBegin; //
}
CEditWnd;
///////////////////////////////////////////////////////////////////////////////
typedef struct CustomDrawTable {
  int x, y;
  int width, height;
}
CustomDrawTable;
//
// Scrollbar button structure, for inserted buttons only
//
typedef struct {
  UINT fMask;   //which members are in use
  UINT uPlacement;  //is this button to the left/right (above/below) of the scrollbar??
  UINT uCmdId;   //command identifier (WM_COMMAND value to send)
  UINT uButType;  //
  UINT uState;   //toggled etc
  int nSize;   //size in pixels. -1 for autosize
  HBITMAP hBmp;   //handle to a bitmap to use as the button face
  HENHMETAFILE hEmf;   //handle to an enhanced metafile
  HCURSOR hCurs;   //handle to a user-supplied mouse cursor to apply
  //to this button
  int nSizeReserved; //internal variable used for resizing
  int nMinSize;  //min size
  int nMaxSize;  //max size
}
SCROLLBUT;
/* maximum number of inserted buttons per bar */
#define MAX_COOLSB_BUTS 16
// SCROLLBAR datatype. There are two of these structures per window
typedef struct SCROLLBAR {
  UINT fScrollFlags;    //flags
  BOOL fScrollVisible;  //if this scrollbar visible?
  SCROLLINFO scrollInfo;   //positional data (range, position, page size etc)
  int nArrowLength;  //perpendicular size (height of a horizontal, width of a vertical)
  int nArrowWidth;   //parallel size (width of horz, height of vert)
  //data for inserted buttons
  SCROLLBUT sbButtons[ MAX_COOLSB_BUTS ];
  int nButtons;
  int nButSizeBefore;  //size to the left / above the bar
  int nButSizeAfter;  //size to the right / below the bar
  BOOL fButVisibleBefore; //if the buttons to the left are visible
  BOOL fButVisibleAfter; //if the buttons to the right are visible
  int nBarType;   //SB_HORZ / SB_VERT
  UINT fFlatScrollbar;  //do we display flat scrollbars?
  int nMinThumbSize;
}
SCROLLBAR;
// Container structure for a cool scrollbar window.
typedef struct SCROLLWND {
  CSUBCLASSWNDDEF;
  HBITMAP hSkinBmp;
  CustomDrawTable cdt_horz_normal_hot_active[ 24 ];
  CustomDrawTable cdt_vert_normal_hot_active[ 24 ];
  CustomDrawTable cdt_both; //
  UINT bars;    //which of the scrollbars do we handle? SB_VERT / SB_HORZ / SB_BOTH
  SCROLLBAR sbarHorz;  //one scrollbar structure each for
  SCROLLBAR sbarVert;  //the horizontal and vertical scrollbars
  BOOL fThumbTracking; // are we currently thumb-tracking??
  BOOL fLeftScrollbar; // support the WS_EX_LEFTSCROLLBAR style
  HWND hwndToolTip;  // tooltip support!!!
  //size of the window borders
  int cxLeftEdge, cxRightEdge;
  int cyTopEdge, cyBottomEdge;
  // To prevent calling original WindowProc in response
  // to our own temporary style change (fixes TreeView problem)
  BOOL bPreventStyleChange;
}
SCROLLWND;
///////////////////////////////////////////////////////////////////////////////
// Uncomment the following 2 lines to enable support for BCMenu class
//#define BTNST_USE_BCMENU
//#include "BCMenu.h"
// Uncomment the following line to enable support for sound effects
#define BTNST_USE_SOUND
// Return values
#ifndef BTNST_OK
#define BTNST_OK      0
#endif
#ifndef BTNST_INVALIDRESOURCE
#define BTNST_INVALIDRESOURCE   1
#endif
#ifndef BTNST_FAILEDMASK
#define BTNST_FAILEDMASK    2
#endif
#ifndef BTNST_INVALIDINDEX
#define BTNST_INVALIDINDEX    3
#endif
#ifndef BTNST_INVALIDALIGN
#define BTNST_INVALIDALIGN    4
#endif
#ifndef BTNST_BADPARAM
#define BTNST_BADPARAM     5
#endif
#ifndef BTNST_INVALIDPRESSEDSTYLE
#define BTNST_INVALIDPRESSEDSTYLE  6
#endif
// Dummy identifier for grayscale icon
#ifndef BTNST_AUTO_GRAY
#define BTNST_AUTO_GRAY     (HICON)(0xffffffff - 1L)
#endif 
// Dummy identifier for 15% darker icon
#ifndef BTNST_AUTO_DARKER
#define BTNST_AUTO_DARKER    (HICON)(0xffffffff - 2L)
#endif
enum { ST_ALIGN_HORIZ = 0,            // Icon/bitmap on the left, text on the right
       ST_ALIGN_VERT,                 // Icon/bitmap on the top, text on the bottom
       ST_ALIGN_HORIZ_RIGHT,          // Icon/bitmap on the right, text on the left
       ST_ALIGN_OVERLAP          // Icon/bitmap on the same space as text
     };
enum { BTNST_COLOR_BK_IN = 0,         // Background color when mouse is INside
       BTNST_COLOR_FG_IN,             // Text color when mouse is INside
       BTNST_COLOR_BK_OUT,            // Background color when mouse is OUTside
       BTNST_COLOR_FG_OUT,            // Text color when mouse is OUTside
       BTNST_COLOR_BK_FOCUS,          // Background color when the button is focused
       BTNST_COLOR_FG_FOCUS,          // Text color when the button is focused
       BTNST_MAX_COLORS
     };
// for m_shadeID
enum { SHS_NOSHADE = 0,
       SHS_NOISE,
       SHS_DIAGSHADE,
       SHS_HSHADE,
       SHS_VSHADE,
       SHS_HBUMP,
       SHS_VBUMP,
       SHS_SOFTBUMP,
       SHS_HARDBUMP,
       SHS_METAL };
typedef struct _STRUCT_CALLBACK {
  HWND hWnd;   // Handle to window
  UINT nMessage;  // Message identifier
  WPARAM wParam;
  LPARAM lParam;
}
STRUCT_CALLBACK;
#ifdef BTNST_USE_SOUND
typedef struct _STRUCT_SOUND {
  TCHAR szSound[ _MAX_PATH ];
  LPCTSTR lpszSound;
  HMODULE hMod;
  DWORD dwFlags;
}
STRUCT_SOUND;
#endif
typedef struct CButtonST {
  CSUBCLASSWNDDEF;
  BOOL m_bShowDisabledBitmap;
  POINT m_ptImageOrg;
  POINT m_ptPressedOffset;
  BOOL m_bIsFlat;        // Is a flat button? 
  BOOL m_bMouseOnButton; // Is mouse over the button?
  BOOL m_bDrawTransparent; // Draw transparent?
  BOOL m_bIsPressed;  // Is button pressed?
  BOOL m_bIsFocused;  // Is button focused?
  BOOL m_bIsDisabled;  // Is button disabled?
  BOOL m_bIsDefault;  // Is default button?
  BOOL m_bIsCheckBox;  // Is the button a checkbox?
  BYTE m_byAlign;   // Align mode
  BOOL m_bDrawBorder;  // Draw border?
  BOOL m_bDrawFlatFocus; // Draw focus rectangle for flat button?
  COLORREF m_crColors[ BTNST_MAX_COLORS ]; // Colors to be used
  HWND m_hParentWndMenu; // Handle to window for menu selection
  BOOL m_bMenuDisplayed; // Is menu displayed ?
#ifdef BTNST_USE_BCMENU
  BCMenu m_menuPopup;  // BCMenu class instance
#else
  HMENU m_hMenu;   // Handle to associated menu
#endif
  HCURSOR m_hCursor;   // Handle to cursor
  HDC m_dcBk;
  HBITMAP m_bmpBk;
  HBITMAP m_pbmpOldBk;
  BOOL m_bAlwaysTrack;  // Always hilight button?
  int m_nCheck;   // Current value for checkbox
  UINT m_nTypeStyle;  // Button style
  TCHAR m_szURL[ _MAX_PATH ]; // URL to open when clicked
  HICON m_csIcons[ 2 ];
  HBITMAP m_csBitmaps[ 2 ];
  COLORREF m_crTransparent[ 2 ];
  STRUCT_CALLBACK m_csCallbacks;
#ifdef BTNST_USE_SOUND
  STRUCT_SOUND m_csSounds[ 2 ]; // Index 0 = Over 1 = Clicked
#endif
  // Shade
  UINT m_shadeID;
  BYTE m_granularity;
  BYTE m_highlight;
  BYTE m_coloring;
  COLORREF m_color;
  BOOL m_bIsXPButton; // for CWinXPButtonST
  BOOL m_bIsRounded; // for CWinXPButtonST
}
CButtonST;
typedef struct XPBUTTONINFO {
  COLORREF crColorLeft;
  COLORREF crColorRight;
  COLORREF crColorTop;
  COLORREF crColorBottom;
  double a[ 3 ];
}
XPBUTTONINFO;
///////////////////////////////////////////////////////////////////////////////
enum CONTROLTYPE { TLCT_TEXT,     /* default */
                   TLCT_EDIT,
                   TLCT_COMBOBOX,
                   TLCT_DATECONTROL,
                 };
typedef struct TLCONTROLINFO {
  int m_controlType;
  COLORREF m_mapClf;
  TCHAR m_szComboBox[ 100 ]; //
}
TLCONTROLINFO;
//should have called this LV_INSERTITEM ..what ever
typedef struct CTreeItem CTreeItem;
typedef struct CSuperGridCtrl {
  CSUBCLASSWNDDEF;
  COLORREF m_crTreeLine;
  COLORREF m_crRectangle;
  COLORREF m_crPlusMinus;
  COLORREF m_brErase;
  COLORREF m_crCellRGB;
  int m_cxImage, m_cyImage; //icon size
  int m_CurItem; //keyboard handling..it is what it says
  int m_CurSubItem; //keyboard handling..it is what it says
  //hmm these represents the state of my expresso machine
  int m_nDragItem, m_nDragTarget;
  BOOL m_bIsDragging;
  BOOL m_bDrag;
  //HIMAGELIST m_image; //Must provide an imagelist
  //struct _IMAGELIST* m_image; //the image list
  //list of rootItems
  CTreeItem* m_Items[ 1000 ];
  int m_ItemsLen;
}
CSuperGridCtrl;
///////////////////////////////////////////////////////////////////////////////
BOOL UI_Init( HWND hWnd, void* wnd, int type );
HBITMAP CreateXpStyleBitmap( XPBUTTONINFO* This, int h, int w );
HBITMAP LoadPicture( LPCTSTR szFilename );
HICON GetIconFromResources( UINT nID, int szIcon_cx, int szIcon_cy );
COLORREF OffsetColor( COLORREF crColors, short shOffset );
HICON CreateGrayscaleIcon( HICON hIcon );
HICON CreateDarkerIcon( HICON hIcon );
HBITMAP CreateGrayscaleBitmap( HBITMAP hBitmap, COLORREF crTrans );
HBITMAP CreateDarkerBitmap( HBITMAP hBitmap, COLORREF crTrans );
HBITMAP CreateBitmapMask( HBITMAP hSourceBitmap, COLORREF crTransColor );
BOOL UI_LoadSkinRes( HWND hWnd, const TCHAR* skinfile );
BOOL InstallSkin( HWND hWnd, const TCHAR * skinfile );
HCURSOR LoadCursorFromModuleFile( LPCTSTR lpFileName, LPCTSTR lpszResourceName );
HANDLE AddTTFFontResource( HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType, LOGFONT* plf );
BOOL AddTTFFontFile( LPCTSTR szFontFile, LOGFONT* plf );
#endif // _SKIN_H_
