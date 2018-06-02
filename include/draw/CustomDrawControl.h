
#define DEFAULT_SORT_ASCENDING true

#ifndef LVS_EX_BORDERSELECT
#define LVS_EX_BORDERSELECT 0x00008000 // border selection style instead of highlight
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP 0x00004000 // listview unfolds partly hidden labels if it does not have infotip text
#endif // LVS_EX_LABELTIP

#ifndef LVS_EX_INFOTIP
#define LVS_EX_INFOTIP 0x00000400 // listview does InfoTips for you
#endif // LVS_EX_INFOTIP

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000 // Paints via double-buffering, which reduces flicker. 
// This extended style also enables alpha-blended marquee selection on systems where it is supported.
#endif // LVS_EX_DOUBLEBUFFER

#ifndef SHIFT_DOWN
#define SHIFT_DOWN (GetKeyState(VK_SHIFT) < 0)
#endif // SHIFT_DOWN

#ifndef CNTRL_DOWN
#define CNTRL_DOWN (GetKeyState(VK_CONTROL) < 0)
#endif // CNTRL_DOWN


#define RedrawWindow()

// Global control


// CCustomDrawToolTipCtrl

// Basically the codes are taken from CMFCToolTipCtrl
//
// Here's the appearance of a CCustomDrawToolTipCtrl
//
// Note:
// 1) The items that enclosed with [] are required
// 2) The items that enclosed with <> are optional
// 3) The separator presents only when description is valid
//
// Example:
// http://msdn.microsoft.com/en-us/library/cc872782.aspx#enhancedTooltips
//
// +---------------------------------------+
// | <icon> [title label] |
// +---------------------------------------+
// | |
// | <preview> <supplemental description> |
// | |
// +--------<separator>--------------------+
// | <desc icon> <description> |
// +---------------------------------------+



// wParam
enum {
  CDTOOLTIP_ONBEFORE_SHOW,
  CDTOOLTIP_ONBEFORE_POP,
};


#ifdef _UNICODE
#define TTTOOLINFO_V1_SIZE TTTOOLINFOW_V1_SIZE
#define TTTOOLINFO_V2_SIZE TTTOOLINFOW_V2_SIZE
#define TTTOOLINFO_V3_SIZE TTTOOLINFOW_V3_SIZE
#else
#define TTTOOLINFO_V1_SIZE TTTOOLINFOA_V1_SIZE
#define TTTOOLINFO_V2_SIZE TTTOOLINFOA_V2_SIZE
#define TTTOOLINFO_V3_SIZE TTTOOLINFOA_V3_SIZE
#endif // _UNICODE

#define WM_CDTOOLTIPCTRL_NOTIFY (WM_USER+0x600)

struct CCustomDrawToolTipCtrl {

  RECT m_rcMargin;
  RECT m_rcClient;
  POINT m_ptMargin;
  str_t m_strDescription[1];
  SIZE m_sizeImage;
  POINT m_ptLocation;

  int m_nFixedWidthRegular;
  int m_nFixedWidthWithImage;

  SIZE m_sizeDescrImage;
  SIZE m_sizeSupplementalImage;
  str_t m_strSupplementalDescription[1];
  str_t m_strLabel[1];

  img_t m_hLabelIcon[1];
  img_t m_hDescrIcon[1];
  img_t m_hSupplementalBmp[1];

  // See SetText()
  str_t m_astrLongTipText[1];

  //CCustomDrawToolTipInfo m_Params;

  int m_nTextTabSize;

  WPARAM m_DisplayIDWParam;
  LPARAM m_DisplayIDLParam;

  // for text alignment
  int m_nLabelHeight;


  BOOL m_bBalloonTooltip;
  BOOL m_bDrawIcon;
  BOOL m_bDrawDescription;
  BOOL m_bRoundedCorners;
  BOOL m_bBoldLabel;
  BOOL m_bDrawSeparator;
  BOOL m_bVislManagerTheme;
  int m_nMaxDescrWidth;
  int m_nGradientAngle; // 0 - 360, -1 -default (vertical), not yet implemented in VS6.0
  COLORREF m_clrFill;
  COLORREF m_clrFillGradient;
  COLORREF m_clrText;
  COLORREF m_clrBorder;


  COLORREF m_clrSeparator;
  BOOL m_bDrawSupplementalDescription;
  BOOL m_bDrawDescrIcon;
  BOOL m_bDrawSupplementalImage;
  BOOL m_bBoldDescription;


#include "CustomDrawToolTipCtrl.inl"
};


typedef struct hditem_t {
  UINT mask;
  //int cxy;
  str_t text[1];
  img_t hbm[1];
  int fmt;
  int iImage;
  //int iOrder;
  UINT type;
  UINT state;
  COLORREF clrText;
  COLORREF clrBack;
  int nWidth;
  RECT rc;
} hditem_t;

// CCustomDrawListCtrl

//#define FIX_LISTCTRL_HILIGHT_UNSELECTED_ITEM_ISSUE
struct list_ctrl {
  BOOL m_bIsMousePressed;
  int m_nHighlightedItem;
  int m_nColCount;
  int m_hit;
  //hditem_t* m_hdItem;

  BOOL m_bAscending;
  int m_nSortColumn;
  BOOL m_bSortable;

#include "CustomDrawHeaderCtrl.inl"

  hditem_t* m_hdItem;
  //hditem_t* m_items;
  int m_nCount;
  BOOL m_bIsWindowEnabled;
  BOOL m_bIsFullRowSelect;

  BOOL m_bMarkSortedColumn;
  COLORREF m_clrSortedColumn;
  RECT m_rcHeader;

  img_t m_SmallImageList[1];

  int m_nHotItem;
  BOOL m_bMouseEventsTracked;
  BOOL m_bDrawHotItem;

  BOOL m_bExplorerVisualStyle;

  int m_nLockDrawCount;
  UINT m_dwStyle;
#include "CustomDrawListCtrl.inl"
};


// CCustomDrawListBox

#ifndef LBS_COMBOBOX
#define LBS_COMBOBOX 0x8000L
#endif

struct CCustomDrawListBox {
  virtual CCustomDrawToolTipCtrl& GetCustomDrawToolTips() {
    return m_OwnToolTipCtrl;
  }

  virtual BOOL ShouldDrawHotItem() const {
    return TRUE;
  }


  img_t* m_pImageList;

  int m_nHotItem;
  BOOL m_bMouseEventsTracked;

  CCustomDrawToolTipCtrl m_OwnToolTipCtrl; // No direct access allowed from derived struct.
  friend struct CCustomDrawComboBox;

  BOOL m_bExplorerVisualStyle;
  int m_nLockDrawCount;
  //#include "CustomDrawListBox.inl"
};


// CCustomDrawTreeCtrl

#ifndef CCustomDrawTreeCtrlBase
#define CCustomDrawTreeCtrlBase CTreeCtrl
#endif // CCustomDrawTreeCtrlBase

// Undefine this underneath macro to see what happen when scrolling the tree control horizontally
#define FIX_TREECTRL_SCROLL_REDRAW_ISSUE

struct CCustomDrawTreeCtrl {
  inline BOOL IsCustomDrawEnabled() const {
    return m_bCustomDraw;
  }
  BOOL IsDrawHotItem() const {
    return m_bDrawHotItem;
  }

  enum SelStyle {
    CDTSS_LABEL,
    CDTSS_LABELRIGHT,
    CDTSS_FULLROWSELECT,
  };

  inline SelStyle GetSelStyle() const {
    return m_SelStyle;
  }
  void SetSelStyle(SelStyle selStyle);

  BOOL m_bCustomDraw;
  HTREEITEM m_hHotTreeItem;
  BOOL m_bDrawHotItem;
  BOOL m_bMouseEventsTracked;
  int m_nLockDrawCount;
  BOOL m_bExplorerVisualStyle;
  SelStyle m_SelStyle;

  //#include "CustomDrawTreeCtrl.inl"

};


// struct CTriCheckStateTreeCtrl

enum TriCheckState {
  TTCS_NONE = 0,
  TTCS_UNCHECKED,
  TTCS_PARTIALCHECKED,
  TTCS_CHECKED,
};

struct CTriCheckStateTreeCtrl : public CCustomDrawTreeCtrl {
  inline BOOL GetDeleteParentIfLastChild() const {
    return m_bDeleteParentIfLast;
  }
  inline void SetDeleteParentIfLastChild(BOOL val) {
    m_bDeleteParentIfLast = val;
  }

  UINT m_uClickHitTestFlags;
  img_t m_ChkBoxImgList;
  BOOL m_bDeleteParentIfLast;
  //#include "TriCheckStateTreeCtrl.inl"
};


// CMultiSelTriCheckTreeCtrl


// Inspired by: CMultiTree by Richard Hazlewood
// http://www.codeguru.com/Cpp/controls/treeview/misc-advanced/article.php/c629/

//take the liberty of using 1 for focused item - it used
// to be defined anyway, before MS removed it.
#ifndef TVIS_FOCUSED
#define TVIS_FOCUSED 1
#else
#if TVIS_FOCUSED != 1
#error TVIS_FOCUSED was assumed to be 1
#endif
#endif

struct CMultiSelTriCheckTreeCtrl : public CTriCheckStateTreeCtrl {
  inline BOOL IsMultiSelectable() const {
    return m_bMultiSel;
  }

  inline BOOL IsAlphaBlendedSelectionEnabled() const {
    return m_bAlphaBlendedSelection;
  }

  BOOL IsEmulatedNotify() const {
    return m_bEmulated;
  }
  BOOL m_bMultiSel;
  COLORREF m_clrSelFill;
  COLORREF m_clrSelBorder;
  BOOL m_bAlphaBlendedSelection; // enables alpha-blended marquee selection

  // States
  BOOL m_bPendingDragSel; // mouse button is being holded but may not move yet
  BOOL m_bDuringDragSel; // m_bPendingDragSel must also be true when this boolean is true
  BOOL m_bLeftBtnDragSel;
  RECT m_rcSelBox;

  HTREEITEM m_hSelectAnchor;
  //TreeItemList m_itemsInSelBox; // temporary used during dragging

  BOOL m_bEmulated;
  //#include "MultiSelTriCheckTreeCtrl.inl"
};


// CIconEdit


#define HYBRID_ICONEDIT

struct CIconWnd {
  img_t m_hIcon;
  BOOL m_bAutoDestroyIcon;
  void OnPaint(HDDC hDC, RECT rect) {
    // Clearing the background
    TFillSolidRect(hDC, &rect, _GetSysColor(COLOR_WINDOW));

    // Drawing the icon
    {
      // Drawing the icon
      int width = GetSystemMetrics(SM_CXSMICON);
      int height = GetSystemMetrics(SM_CYSMICON);

      //::DrawIconEx(hDC,1,1,m_hIcon,width,height,0,NULL,DI_NORMAL);
    }

  }

};

struct CIconEdit {
  CIconWnd m_iconWnd;
  //#include "IconEdit.inl"
};


// CComboListBox

struct CComboListBox {
  void* m_pComboBox;
};


// CCustomDrawComboBox


#define MAX_VISIBLE_COMBOLISTITEM_COUNT 30

//
// If we need to make the listbox control (within the combobox control) a "virtual list box", we need to make it have
// LBS_NODATA style, however, this can not be done after its creation. To make the listbox become virtual, we can hook
// the creation of the combobox and modify the listbox's style.
//
// However, this solution seems fail if Common Control version 6 is enabled (when a manifest file is used), if we decide
// to use ComCtl v6, then we have to turn off this macro.
//
// If we don't make the listbox virtual (with the style LBS_NODATA, then we will have to insert/delete items during runtime,
// that's the trade-off.

#define USE_HOOK_CHANGE_LISTBOX_STYLE

// Here's the suggested way to subclass the internal edit and list box control:
// How to subclass CListBox and CEdit inside of CComboBox
// http://support.microsoft.com/kb/174667
#define SUBCLASS_LISTBOX_EDIT_IN_CTLCOLOR

struct CCustomDrawComboBox {
  // Attributes
  HWND m_hComboList;
  HWND m_hComboEdit;

  //#include "CustomDrawComboBox.inl"
  //CComboListBox m_ComboListBox; // No direct access allowed from derived struct.
  //CEdit m_ComboEdit;
};


// CXEditPrompt



// Initial version by Hans Dietrich, see http://www.codeproject.com/KB/edit/XEditPrompt.aspx


#ifndef CXEditPromptBase
#define CXEditPromptBase CEdit
#endif

struct CXEditPrompt {
  COLORREF GetPromptColor() {
    return m_crPromptColor;
  }
  str_t GetPromptText() {
    return m_strPromptText;
  }
  COLORREF GetBKColor() const {
    return m_crBkColor;
  }
  DWORD GetCueBannerAlign() {
    return m_dwCueBannerAlign;
  }
  BOOL IsShowingCueBanner() const {
    return m_bShowCueBanner;
  }

  BOOL m_bShowCueBanner; // TRUE = user has not yet selected this control
  str_t m_strPromptText; // prompt text to display initially
  COLORREF m_crPromptColor; // color to use for prompt text
  COLORREF m_brush; // background brush for WM_CTLCOLOR

  COLORREF m_crBkColor; // color of background
  COLORREF m_robrush; // Read-only background brush for WM_CTLCOLOR
  DWORD m_dwCueBannerAlign; // ES_LEFT
  DWORD m_dwOldAlignStyle;

  //#include "XEditPrompt.inl"
};
