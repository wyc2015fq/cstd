
#ifndef __TREELISTWND_H__
#define __TREELISTWND_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <commctrl.h>

typedef int ( CALLBACK *PFNTVCOMPAREEX ) ( HWND hWnd, HTREEITEM hItem1, HTREEITEM hItem2, LPARAM lParam1, LPARAM lParam2, LPARAM lParam );
typedef int ( CALLBACK *PFNTVSORTEX ) ( HWND hWnd, HTREEITEM hItem , LPCTSTR pTextItem, LPCTSTR pTextInsert, LPARAM lParamItem, LPARAM lParamInsert );

typedef struct {
  HTREEITEM hParent;
  PFNTVCOMPAREEX lpfnCompare;
  LPARAM lParam;
}
TVSORTEX;

typedef struct {
  UINT uFlags;
  UINT uColumn;
  UINT uState;
  UINT uStateMask;
  LPARAM lParam;
  LPCTSTR pText;
}
TVFIND;

typedef struct {
  NMHDR hdr;
  WORD wVKey;
  WORD wScan;
  UINT flags;
}
TV_KEYDOWN_EX;

typedef struct {
  NMHDR hdr;
  TVITEM item;
  UINT uAction;
  UINT uHeight;
  UINT uMaxEntries;
  LPCTSTR pTextEntries;
  LPCTSTR *pTextList;
  POINT ptAction;
}
TV_STARTEDIT;

typedef TVSORTEX *LPTVSORTEX;
typedef TVSORTEX *LPTV_SORTEX;
typedef TVSORTEX TV_SORTEX;


#define TVCOLUMN            LV_COLUMN
#define TV_COLUMN           LV_COLUMN
#define TV_FIND             TVFIND
#define TV_NOIMAGE          (-2)
#define TV_NOCOLOR          0xFFFFFFFF
#define TVLE_DONOTIFY       0xF5A5A500
#define TVIF_TEXTPTR        0x80000000
#define TVIF_TOOLTIPTIME    0x40000000
#define TVIF_TEXTCHANGED    0x20000000
#define TVIF_RETURNEXIT     0x10000000
#define TVIF_CASE           0x08000000
#define TVIF_NEXT           0x04000000
#define TVIF_CHILD          0x02000000
#define TVIF_SUBITEM        0x8000
#define TVIF_SUBNUMBER      0x4000
#define TVIS_UNTERLINE      0x0001
#define TVSIL_CHECK         0x0003
#define TVSIL_SUBIMAGES     0x0004
#define TVSCW_AUTOSIZE      LVSCW_AUTOSIZE
#define TVN_COLUMNCLICK     HDN_ITEMCLICK
#define TVN_COLUMNDBLCLICK  HDN_ITEMDBLCLICK
#define TVI_SORTEX          ((HTREEITEM) 0xFFFF0007)
#define TVI_BEFORE          ((HTREEITEM) 0xFFFF0006)
#define TVI_AFTER           ((HTREEITEM) 0xFFFF0005)
#define TVI_ROW(n)          ((HTREEITEM)(0xFFE00000+(n)))
#ifndef VK_DBLCLK
#define VK_DBLCLK     0x10000
#endif
#ifndef VK_ICONCLK
#define VK_ICONCLK    0x10001
#endif
#ifndef VK_EDITCLK
#define VK_EDITCLK    0x10002
#endif


#define TVC_BK         0
#define TVC_ODD        1
#define TVC_EVEN       2
#define TVC_FRAME      3
#define TVC_TEXT       4
#define TVC_LINE       5
#define TVC_BOX        6
#define TVC_TRACK      7
#define TVC_MARK       8
#define TVC_MARKODD    8
#define TVC_MARKEVEN   9
#define TVC_INSERT    10
#define TVC_BOXBG     11
#define TVC_COLBK     12
#define TVC_COLODD    13
#define TVC_COLEVEN   14
#define TVC_GRAYED    15



#define TVGN_DROPHILITESUB  0x000C
#define TVGN_CARETSUB       0x000D
#ifndef TVGN_NEXTSELECTED
#define TVGN_NEXTSELECTED   0x000E
#endif
#define TVGN_FOCUS          0x000F
#define TVGN_FOCUSSUB       0x0010
#define TVGN_NEXTSELCHILD   0x0011
#define TVGN_LASTCHILD      0x0012




#define TVCF_FMT      LVCF_FMT
#define TVCF_IMAGE    LVCF_IMAGE
#define TVCF_TEXT     LVCF_TEXT
#define TVCF_WIDTH    LVCF_WIDTH
#define TVCF_VWIDTH   LVCF_SUBITEM
#define TVCF_MIN      LVCF_ORDER
#define TVCF_MARK     0x80000000
#define TVCF_FIXED    0x40000000



#define TVAE_NONE         (0<<TVAE_MODEPOS)
#define TVAE_EDIT         (1<<TVAE_MODEPOS)
#define TVAE_COMBO        (2<<TVAE_MODEPOS)
#define TVAE_CBLIST       (3<<TVAE_MODEPOS)
#define TVAE_STEP         (4<<TVAE_MODEPOS)
#define TVAE_STEPED       (5<<TVAE_MODEPOS)
#define TVAE_CHECK        (6<<TVAE_MODEPOS)
#define TVAE_CHECKED      (7<<TVAE_MODEPOS)
#define TVAE_NEXTLINE     0x0001
#define TVAE_DBLCLICK     0x0002
#define TVAE_FULLWIDTH    0x0004
#define TVAE_PTRLIST      0x0008
#define TVAE_ONLYRETURN   0x0010
#define TVAE_STATEENABLE  0x0020
#define TVAE_ICONCLICK    0x0040
#define TVAE_DROPDOWN     0x0080
#define TVAE_COL(c)       (((c)&0x3F)<<11)
#define TVAE_CHAR(c)      (((c)&0xFF)<<17)
#define TVAE_COUNT(c)     (((c)&0x7F)<<25)
#define TVAE_MODEMASK     (7<<TVAE_MODEPOS)
#define TVAE_MODEPOS      0x0008
#define TVIS_DISABLEBIT   0x8000



#define TVHT_SUBTOCOL(u)   (((unsigned)u)>>24)
#define TVHT_SUBMASK       0xFF000000
#define TVHT_ONRIGHTSPACE  0x00800000
#define TVHT_ONSUBLABEL    0x00400000
#define TVHT_ONSUBICON     0x00200000
#define TVHT_ONSUBRIGHT    0x00100000
#define TVHT_ONSUBITEM     (TVHT_ONSUBICON|TVHT_ONSUBLABEL)



#define TVIR_COLTOSUB(u)  ((u)<<24)
#define TVIR_GETCOLUMN   0x00000080
#define TVIR_TEXT        0x00000001



#define TVIS_WITHCHILDS   0x00000001
#define TVIS_DESELECT     0x00000002


#define TVOP_AUTOEXPANDOFF  0x00000001
#define TVOP_WRITEOPTION    0x80000000



#define TVIR_EDITCOMBOCHAR(n) (((n)&0xFF)<<8)
#define TVIR_EDITCOMBODEL     0x00000080
#define TVIR_EDITCOMBODOWN    0x10000000
#define TVIR_EDITCOMBOBOX     0x20000000
#define TVIR_EDITCOMBOLIST    0x40000000
#define TVIR_EDITFULL         0x80000000
#define TVIR_EDITTEXT         0x00000001
#define TVIR_EDITCOL(u)       ((u)&0xFF)
#define TVIR_SELALL           0x00000000
#define TVIR_SELAREA(a,b)     ((0x0C0000|(a&0x1FF)|((b&0x1FF)<<9))<<8)
#define TVIR_SETCURSOR(a)     ((0x080000|(a&0x3FFFF))<<8)
#define TVIR_SETAT(a)         ((0x040000|(a&0x3FFFF))<<8)



#define TVCFMT_BITMAP_ON_RIGHT LVCFMT_BITMAP_ON_RIGHT
#define TVCFMT_COL_HAS_IMAGES  LVCFMT_COL_HAS_IMAGES
#define TVCFMT_CENTER          LVCFMT_CENTER
#define TVCFMT_IMAGE           LVCFMT_IMAGE
#define TVCFMT_LEFT            LVCFMT_LEFT
#define TVCFMT_RIGHT           LVCFMT_RIGHT
#define TVCFMT_FIXED           0x20000000
#define TVCFMT_MARK            0x10000000



#define TVS_EX_BITCHECKBOX     0x00008000
#define TVS_EX_ITEMLINES       0x00010000
#define TVS_EX_ALTERNATECOLOR  0x00020000
#define TVS_EX_SUBSELECT       0x00040000
#define TVS_EX_FULLROWMARK     0x00080000
#define TVS_EX_TOOLTIPNOTIFY   0x00100000
#define TVS_EX_AUTOEXPANDICON  0x00200000
#define TVS_EX_NOCHARSELCET    0x00400000
#define TVS_EX_NOCOLUMNRESIZE  0x00800000
#define TVS_EX_HIDEHEADERS     0x01000000
#define TVS_EX_GRAYEDDISABLE   0x02000000
#define TVS_EX_FULLROWITEMS    0x04000000
#define TVS_EX_FIXEDCOLSIZE    0x08000000
#define TVS_EX_HOMEENDSELECT   0x10000000
#define TVS_EX_SHAREIMAGELISTS 0x20000000
#define TVS_EX_EDITCLICK       0x40000000
#define TVS_EX_NOCURSORSET     0x80000000
#ifndef TVS_EX_MULTISELECT
#define TVS_EX_MULTISELECT     0x00001000
#endif
#ifndef TVS_EX_AUTOHSCROLL
#define TVS_EX_AUTOHSCROLL     0x00002000
#endif



#define TVN_ITEMTOOLTIP      (TVN_FIRST-32)
#define TVN_CBSTATECHANGED   (TVN_FIRST-33)
#define TVN_STEPSTATECHANGED (TVN_FIRST-34)
#define TVN_STARTEDIT        (TVN_FIRST-35)



#define TVM_GETHEADER        (TV_FIRST+96-1)
#define TVM_GETCOLUMNCOUNT   (TV_FIRST+96-2)
#define TVM_DELETECOLUMN     (TV_FIRST+96-3)
#define TVM_INSERTCOLUMN     (TV_FIRST+96-4)
#define TVM_SELECTSUBITEM    (TV_FIRST+96-5)
#define TVM_SELECTDROP       (TV_FIRST+96-6)
#define TVM_SETITEMBKCOLOR   (TV_FIRST+96-7)
#define TVM_GETITEMBKCOLOR   (TV_FIRST+96-8)
#define TVM_SETITEMTEXTCOLOR (TV_FIRST+96-9)
#define TVM_GETITEMTEXTCOLOR (TV_FIRST+96-10)
#define TVM_GETITEMOFROW     (TV_FIRST+96-11)
#define TVM_GETROWCOUNT      (TV_FIRST+96-12)
#define TVM_GETROWOFITEM     (TV_FIRST+96-13)
#define TVM_SETCOLUMN        (TV_FIRST+96-14)
#define TVM_GETCOLUMN        (TV_FIRST+96-15)
#define TVM_SETCOLUMNWIDTH   (TV_FIRST+96-16)
#define TVM_GETCOLUMNWIDTH   (TV_FIRST+96-17)
#define TVM_SETUSERDATASIZE  (TV_FIRST+96-18)
#define TVM_GETUSERDATASIZE  (TV_FIRST+96-19)
#define TVM_GETUSERDATA      (TV_FIRST+96-20)
#define TVM_SORTCHILDRENEX   (TV_FIRST+96-21)
#define TVM_COLUMNAUTOEDIT   (TV_FIRST+96-22)
#define TVM_COLUMNAUTOICON   (TV_FIRST+96-23)
#define TVM_GETCOUNTPERPAGE  (TV_FIRST+96-24)
#define TVM_FINDITEM         (TV_FIRST+96-25)
#define TVM_SELECTCHILDS     (TV_FIRST+96-26)
#define TVM_GETSETOPTION     (TV_FIRST+96-27)
#define TVM_ISITEMVISIBLE    (TV_FIRST+96-28)
#define TVM_SETFOCUSITEM     (TV_FIRST+96-29)
#ifndef TVM_GETITEMSTATE
#define TVM_GETITEMSTATE     (TV_FIRST+39)
#endif
#ifndef TVM_GETEXTENDEDSTYLE
#define TVM_GETEXTENDEDSTYLE (TV_FIRST+45)
#endif
#ifndef TVM_SETEXTENDEDSTYLE
#define TVM_SETEXTENDEDSTYLE (TV_FIRST+44)
#endif
#ifndef TVM_GETLINECOLOR
#define TVM_GETLINECOLOR  (TV_FIRST+41)
#endif
#ifndef TVM_SETLINECOLOR
#define TVM_SETLINECOLOR  (TV_FIRST+40)
#endif


#ifndef TVNRET_DEFAULT
#define TVNRET_DEFAULT   0
#endif
#ifndef TVNRET_SKIPOLD
#define TVNRET_SKIPOLD   1
#endif
#ifndef TVNRET_SKIPNEW
#define TVNRET_SKIPNEW   2
#endif



#define TreeList_SelectItem(h,i)              TreeList_Select(h,i,TVGN_CARET)
#define TreeList_SelectDropTarget(h,i)        TreeList_Select(h,i,TVGN_DROPHILITE)
#define TreeList_SelectSetFirstVisible(h,i)   TreeList_Select(h,i,TVGN_FIRSTVISIBLE)

#define TreeList_GetNextItem(h,i,c)           TreeView_GetNextItem(h, i,     c)
#define TreeList_GetChild(h,i)                TreeView_GetNextItem(h, i,     TVGN_CHILD)
#define TreeList_GetParent(h, i)              TreeView_GetNextItem(h, i,     TVGN_PARENT)
#define TreeList_GetNextSibling(h,i)          TreeView_GetNextItem(h, i,     TVGN_NEXT)
#define TreeList_GetPrevSibling(h,i)          TreeView_GetNextItem(h, i,     TVGN_PREVIOUS)
#define TreeList_GetNextSelected(h,i)         TreeView_GetNextItem(h, i,     TVGN_NEXTSELECTED)
#define TreeList_GetNextSelectedChild(h,i)    TreeView_GetNextItem(h, i,    TVGN_NEXTSELCHILD)
#define TreeList_GetNextVisible(h,i)          TreeView_GetNextItem(h, i,     TVGN_NEXTVISIBLE)
#define TreeList_GetPrevVisible(h,i)          TreeView_GetNextItem(h, i,     TVGN_PREVIOUSVISIBLE)
#define TreeList_GetLastChild(h,i)            TreeView_GetNextItem(h, i,     TVGN_LASTCHILD)
#define TreeList_GetSelection(h)              TreeView_GetNextItem(h, NULL,  TVGN_CARET)
#define TreeList_GetDropHilight(h)            TreeView_GetNextItem(h, NULL,  TVGN_DROPHILITE)
#define TreeList_GetFirstVisible(h)           TreeView_GetNextItem(h, NULL,  TVGN_FIRSTVISIBLE)
#define TreeList_GetLastVisible(h)            TreeView_GetNextItem(h, NULL,  TVGN_LASTVISIBLE)
#define TreeList_GetRoot(h)                   TreeView_GetNextItem(h, NULL,  TVGN_ROOT)
#define TreeList_GetFocus(h)                  TreeView_GetNextItem(h, NULL,  TVGN_FOCUS)
#define TreeList_GetFocusColumn(h)            ((int)TreeView_GetNextItem(h, NULL,  TVGN_FOCUSSUB))
#define TreeList_GetSelectionColumn(h)        ((int)TreeView_GetNextItem(h, NULL,  TVGN_CARETSUB))
#define TreeList_GetDropHilightColumn(h)      ((int)TreeView_GetNextItem(h, NULL,  TVGN_DROPHILITESUB))







#ifdef _MSC_VER
#pragma warning( disable :4996 )
#endif

#ifndef GWLP_USERDATA
#define GWLP_USERDATA       GWL_USERDATA
#endif
#ifndef GWLP_WNDPROC
#define GWLP_WNDPROC        GWL_WNDPROC
#endif
#ifndef _WIN64
#ifndef SetWindowLongPtr
#define SetWindowLongPtr    SetWindowLong
#endif
#ifndef GetWindowLongPtr
#define GetWindowLongPtr    GetWindowLong
#endif
#ifndef DWORD_PTR
#define DWORD_PTR           DWORD
#endif
#ifndef LONG_PTR
#define LONG_PTR            LONG
#endif
#endif
#ifdef UNICODE
#define str_len             (unsigned)wcslen
#define str_cmp             wcscmp
#define str_ncpy            wcsncpy
#define str_ncmp            wcsncmp
#define str_icmp            wcsicmp
#else
#define str_len             (unsigned)strlen
#define str_cmp             strcmp
#define str_ncpy            strncpy
#define str_ncmp            strncmp
#define str_icmp            _stricmp
#endif
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL        0x020A
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA            120
#endif
#ifndef MAX_COLUMNS
#define MAX_COLUMNS            32
#endif
#define MAX_COLORS           16
#define EN_SETTEXT           0x1000
#define EN_RETURN            0x1578
#define EN_ESCAPE            0x1579
#define ID_TOOLTIPCHECK      0x3912
#define SORT_NOUPDATE        1234567
#define FIRST_LINE           0xFFFFFFFE
#define I_CCB                I_CHILDRENCALLBACK
#define U(h)                 ((unsigned)(h))
#define THEMEIMGLIST         ((HIMAGELIST)1)
#define GetHandle(h)         ((UITreeList*)GetWindowLongPtr(h,0))
#define TVIF_ALL             (TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_STATE|TVIF_TEXT)
#define UNLOCK(d)
#define LOCK(d)
#define TVIS_EDIT(m)         ((1<<m)&((1<<TVAX_EDIT)|(1<<TVAX_COMBO)|(1<<TVAX_STEPED)|(1<<TVAX_CHECKED)))
#define TVIS_BASEFLAGS       (TVIS_EXPANDED|TVIS_EXPANDEDONCE|TVIS_EXPANDPARTIAL|TVIS_SELECTED)
#define TVIS_TRACKED         (TVIX_TRACKED<<16)
#define TVIS_BKCOLOR         (TVIX_BKCOLOR<<16)
#define TVIS_FOCUSED         (TVIX_FOCUSED<<16)
#define TVIS_TEXTCOLOR       (TVIX_TEXTCOLOR<<16)
#define TVC_UNSELECT         0x4000
#define TVC_DESELECT         0x8000
#define DEFAULT_IDENT        19
#define DEFAULT_SHIFT        7
#ifndef BPBF_COMPATIBLEBITMAP
#define BPBF_COMPATIBLEBITMAP    0
#endif
#ifndef TVP_GLYPH
#define TVP_GLYPH              2
#endif
#ifndef GLPS_CLOSED
#define GLPS_CLOSED            1
#endif
#ifndef GLPS_OPENED
#define GLPS_OPENED            2
#endif
#ifndef BP_CHECKBOX
#define BP_CHECKBOX            3
#endif
#ifndef CBS_UNCHECKEDNORMAL
#define CBS_UNCHECKEDNORMAL  1
#endif
#ifndef CBS_CHECKEDNORMAL
#define CBS_CHECKEDNORMAL    5
#endif


#define TVAX_NONE            (TVAE_NONE   >>TVAE_MODEPOS)
#define TVAX_EDIT            (TVAE_EDIT     >>TVAE_MODEPOS)
#define TVAX_COMBO           (TVAE_COMBO     >>TVAE_MODEPOS)
#define TVAX_CBLIST          (TVAE_CBLIST >>TVAE_MODEPOS)
#define TVAX_STEP            (TVAE_STEP     >>TVAE_MODEPOS)
#define TVAX_STEPED          (TVAE_STEPED >>TVAE_MODEPOS)
#define TVAX_CHECK           (TVAE_CHECK     >>TVAE_MODEPOS)
#define TVAX_CHECKED         (TVAE_CHECKED>>TVAE_MODEPOS)

#define TVIX_VARBUTTON      0x01
#define TVIX_HASBUTTON      0x02
#define TVIX_HASIMAGE       0x04
#define TVIX_TRACKED        0x08
#define TVIX_TEXTCOLOR      0x10
#define TVIX_BKCOLOR        0x20
#define TVIX_FOCUSED        0x40


typedef struct _BaseItem {
  LPARAM lParam;
  LPTSTR pText;
  UINT uState;
  int iImage;
  int iSelectedImage;
  unsigned uShowPos;
  unsigned uFirstChild;
  unsigned uLastChild;
  unsigned uPrevItem;
  unsigned uNextItem;
  unsigned uParent;
  unsigned uLevel;
  int iTextPixels;
  WORD uTextSize;
  BYTE bCallback;
  BYTE bFlags;
  COLORREF uColorText;
  COLORREF uColorBk;
}
BaseItem;

typedef struct _ExtraItem {
  LPTSTR pText;
  UINT uState;
  int iImage;
  int iTextPixels;
  WORD uTextSize;
  BYTE bCallback;
  BYTE bFlags;
  COLORREF uColorText;
  COLORREF uColorBk;
}
ExtraItem;

typedef struct _ColumnData {
  INT iCbIcon;
  short sSize;
  short sReal;
  short sMin;
  BYTE bMinEx;
  BYTE bWeight;
  BYTE bAlign;
  BYTE bEdit;
  BYTE bFlags;
  BYTE bEnable;
  BYTE bCbSize;
  BYTE bCbChar;
  BYTE bFixed;
  BYTE bMark;
}
ColumnData;

typedef struct _UITreeList {
  CContainerUI base;
  HIMAGELIST hStates;
  HIMAGELIST hImages;
  HIMAGELIST hChecks;
  HIMAGELIST hSubImg;
  HFONT hFontN;
  HFONT hFontB;
  HFONT hFontL;
  HFONT hFontT;
  WNDPROC pToolProc;
  COLORREF uColors[ MAX_COLORS ];
  int iFontHeight;
  int iFontLine;
  int iFontOff;
  int iStatesMode;
  int iStatesXsize;
  int iStatesYsize;
  int iChecksMode;
  int iChecksXsize;
  int iChecksYsize;
  int iImagesXsize;
  int iImagesYsize;
  int iSubImgMode;
  int iSubImgXsize;
  int iSubImgYsize;
  int iRowHeight;
  int iAllWeight;
  int iVarSize;
  int iFixSize;
  int iIndent;
  int iShift;
  int iAutoAdd;
  int iMaxSizeX;
  unsigned uItemPosCount;
  unsigned *pItemPos;
  BaseItem **pTreeItems;
  ExtraItem **pExtraItems[ MAX_COLUMNS - 1 ];
  unsigned uTreeItemsMax;
  unsigned uTreeItemsCount;
  unsigned uNextSeachPos;
  unsigned uUserDataSize;
  unsigned uFirstChild;
  unsigned uLastChild;
  unsigned uScrollX;
  unsigned uScrollY;
  unsigned uSizeX;
  unsigned uSizeY;
  unsigned uSizeYsub;
  unsigned uStyle;
  unsigned uStyleEx;
  unsigned uStartPixel;
  unsigned uMaxEnties;
  unsigned uPageEnties;
  unsigned uColumnCount;
  unsigned uColumnCountVar;
  unsigned uSelectedCount;
  unsigned uSelectedBase;
  unsigned uSelectedItem;
  unsigned uSelectedSub;
  unsigned uFocusItem;
  unsigned uFocusSub;
  unsigned uToolTipItem;
  unsigned uToolTipShow;
  unsigned uToolTipSub;
  unsigned uEditMode;
  unsigned uEditItem;
  unsigned uEditSub;
  unsigned uOldXPage;
  unsigned uOldXCount;
  unsigned uOldYPage;
  unsigned uOldYCount;
  unsigned uTrippleB;
  unsigned uTrippleN;
  unsigned uTrackedItem;
  unsigned uTrackedSub;
  unsigned uInsertMark;
  unsigned uMarkedCols;
  unsigned uDragFlags;
  unsigned uDragItem;
  unsigned uDragSub;
  unsigned uLastSel;
  unsigned uLastMove;
  int aColumnXpos [ MAX_COLUMNS + 2 ];
  ColumnData aColumn [ MAX_COLUMNS ];
  char cColorChanged[ MAX_COLORS ];
  char cColumnStart;
  char cFixedHeight;
  char cLockChanges;
  char cHasRootRow;
  char cKeyIgnore;
  char cClickFlag;
  char cClickEdit;
  char cIsEnabled;
  char cHasFocus;
  char cReSelect;
  char cGlyphOk;
  char cEditCb;
}
UITreeList;

typedef HRESULT ( WINAPI *EndBufferedPtT ) ( HANDLE, BOOL );
typedef HANDLE ( WINAPI *BeginBufferedPnT ) ( HDC, RECT*, DWORD, LPVOID, HDC* );
typedef HRESULT ( WINAPI *BufferedPtInitT ) ( VOID );
typedef HRESULT ( WINAPI *BufferedPtInitT ) ( VOID );

static EndBufferedPtT pEndBufferedPt = NULL;
static BeginBufferedPnT pBeginBufferedPt = NULL;
static BufferedPtInitT pBufferedPtInit = NULL;
static BufferedPtInitT pBufferedPtExit = NULL;

static HPEN hPatternPen = NULL;
static HFONT hDefaultFontN = NULL;
static HFONT hDefaultFontB = NULL;
static LONG lWindowCount = -1;
static RECT sToolRect = { -2, 0, 2, 64};
static TCHAR cKeyData[ 16 ];
static unsigned uKeyLast;
static unsigned uKeyPos;
static void CallbackEntry( UITreeList *pData, BaseItem *pEntry, unsigned uItem,
                          unsigned uFlags, int* iImage, unsigned *uTextSize, LPCTSTR*pText ) {}
static void CallbackExtra( UITreeList *pData, BaseItem *pEntry, ExtraItem *pExtra,
                          unsigned uItem, unsigned uSub, unsigned uFlags, int *iImage, unsigned *uTextSize, LPCTSTR *pText ) {}

static void UpdateScrollX( UITreeList *pData ) {}

#endif
