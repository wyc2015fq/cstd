
//
// BCGPGlobalUtils.h: interface for the CBCGPGlobalUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPGLOBALUTILS_H__FD3579BF_C46C_439C_AD13_5DFDA777F240__INCLUDED_)
#define AFX_BCGPGLOBALUTILS_H__FD3579BF_C46C_439C_AD13_5DFDA777F240__INCLUDED_

#define CBCGPRibbonComboEdit CBCGPRibbonEditCtrl

enum {
  CLASS_DEFULT,
  CLASS_REBAR,
  CLASS_OUTLOOKBAR,
  CLASS_CAPTIONBAR,
  CLASS_POPUPMENUBAR,

  CLASS_ShowAllButton,
  CLASS_OutlookButton,

  CLASS_ToolBox,
  CLASS_PlannerViewDay,
  CLASS_RibbonEdit,
};
#include "bcgpstyle.h"

struct CBCGPDockManager;
struct CBCGPBaseControl {
  virtual DWORD GetStyle() const = 0;
};
struct CBCGPBaseControlBar {
  virtual BOOL IsDialogControl() const = 0;
  virtual BOOL IsFloating() const = 0;
  virtual BOOL CanFloat() const = 0;
  virtual BOOL IsOnGlass() const = 0;
  virtual BOOL IsMessageBarMode() const = 0;
  virtual BOOL IsDropDownListMode() const = 0;
  virtual int GetClass() const = 0;
  virtual int GetBarStyle() const = 0;  // CBRS_BORDER_ANY
  virtual CBitmap* GetBackBitmap() const;
  virtual COLOR GetBackColor() const;
};
struct CBCGPDockingControlBar;
struct CBCGPBarContainerManager;
struct CBCGPPopupMenu;
struct CBCGPBaseToolBar;
struct CBCGPToolBar;
struct CBCGPButtonBase {
  virtual int GetClass() const {
    return 0;
  }
  virtual DWORD GetStyle() const {
    return 0;
  }
  virtual BOOL IsPushed() const {
    return 0;
  }
  virtual BOOL IsHighlightChecked() const {
    return 0;
  }
  virtual BOOL IsPressed() const {
    return 0;
  }
  virtual BOOL IsFocused() const {
    return 0;
  }
  virtual BOOL IsDroppedDown() const {
    return 0;
  }
  virtual BOOL IsEnabled() const {
    return 0;
  }
  virtual BOOL IsHighlighted() const {
    return 0;
  }
  virtual BOOL GetCheck() const {
    return 0;
  }
  virtual BOOL IsChecked() const {
    return 0;
  }
  virtual BOOL IsCaptionButton() const {
    return 0;
  }
  virtual IRECT GetRect() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual int GetIconID(BOOL bHorz, BOOL bMaximized = FALSE) const {
    return 0;
  }
  virtual COLOR GetForegroundColor() const {
    return 0;
  }

  virtual int GetID() const {
    return 0;
  }
  virtual BOOL IsDisabled() const {
    return 0;
  }
  virtual BOOL IsQATMode() const {
    return 0;
  }
  virtual BOOL IsMenuMode() const {
    return 0;
  }
  virtual BOOL IsPaletteIcon() const {
    return 0;
  }
  virtual BOOL IsMenuOnBottom() const {
    return 0;
  }
  virtual ISIZE GetImageSize(RibbonImageType type) const {
    return iSIZE(0, 0);
  }
  virtual void DrawImage(CDC* pDC, RibbonImageType type, CRect rectImage) {}
  virtual ISIZE DrawBottomText(CDC* pDC, BOOL bCalcOnly) {
    return iSIZE(0, 0);
  }
  virtual IRECT GetCommandRect() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual IRECT GetMenuRect() const {
    return iRECT(0, 0, 0, 0);
  }
};
struct CBCGPToolbarButton : public CBCGPButtonBase {};
struct CBCGPToolbarMenuButton : public CBCGPButtonBase {};
struct CBCGPCaptionButton : public CBCGPButtonBase {};
struct CBCGPCaptionMenuButton : public CBCGPButtonBase {};
struct CBCGPToolbarComboBoxButton : public CBCGPButtonBase {};
//struct CBCGPButton : public CBCGPButtonBase {};
struct CBCGPToolBoxButton : public CBCGPButtonBase {};
struct CBCGPPopupWndButton : public CBCGPButtonBase {};

typedef CBCGPButtonBase CBCGPRibbonButton;
typedef CBCGPButtonBase CBCGPRibbonCaptionButton;

typedef CBCGPButtonBase CBCGPRibbonLaunchButton;
typedef CBCGPButtonBase CBCGPRibbonPaletteButton;
typedef CBCGPButtonBase CBCGPRibbonColorButton;
typedef CBCGPButtonBase CBCGPRibbonRadioButton;
typedef CBCGPButtonBase CBCGPURLLinkButton;
typedef CBCGPButtonBase CBCGPRibbonPaletteIcon;


struct CBCGPBaseTabWnd {
  virtual COLOR GetTabBkColor(int iTab) const {
    return 0;
  }
  virtual COLOR GetTabTextColor(int iTab) const {
    return 0;
  }
  virtual BOOL IsFlatTab() const {
    return 0;
  }
  virtual BOOL IsLeftRightRounded() const {
    return 0;
  }
  virtual BOOL IsOneNoteStyle() const {
    return 0;
  }
  virtual BOOL IsVS2005Style() const {
    return 0;
  }
  virtual BOOL IsActiveTabCloseButton() const {
    return 0;
  }
  virtual BOOL HasImage(int iTab) const {
    return 0;
  }
  virtual BOOL GetTabLabel(int iTab, CString& str) const {
    return 0;
  }
  virtual BOOL IsTabCloseButtonHighlighted() const {
    return 0;
  }
  virtual BOOL IsTabCloseButtonPressed() const {
    return 0;
  }
  virtual BOOL IsDrawNoPrefix() const {
    return 0;
  }
  virtual BOOL IsDialogControl() const {
    return 0;
  }
  virtual BOOL IsVisualManagerStyle() const {
    return 0;
  }
  virtual int GetTabsHeight() const {
    return 0;
  }
  virtual int GetLocation() const {
    return 0;
  }
  virtual int GetActiveTab() const {
    return 0;
  }
  virtual int GetFirstVisibleTabNum() const {
    return 0;
  }
  virtual void GetTabsRect(IRECT* prc) const {
    return ;
  }
  virtual void GetClientRect(IRECT* prc) const {
    return ;
  }
  virtual IRECT GetTabCloseButton() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual ISIZE GetImageSize() const {
    return iSIZE(0, 0);
  }
  virtual int GetTabIcon(int iTab) const {
    return 0;
  }
  virtual CBitmap* GetTabHicon(int iTab) const {
    return 0;
  }
  virtual CBitmap* GetImageList() const {
    return 0;
  }
  enum Location {
    LOCATION_BOTTOM = 0,
    LOCATION_TOP = 1,
    TAB_TEXT_MARGIN = 4,
    TAB_IMAGE_MARGIN = 4,
  };

};
struct CBCGPStatusBar;
struct CBCGStatusBarPaneInfo {
  virtual DWORD GetStyle() const {
    return 0;
  }
  virtual COLOR GetTextColor() const {
    return 0;
  }
};
struct CBCGPMiniFrameWnd {
  virtual int GetPreDockState() const {
    return 0;
  }
};
struct CBCGPCaptionBar {
  virtual BOOL IsMessageBarMode() const {
    return 0;
  }
};
struct CBCGPHeaderCtrl {
  virtual BOOL IsDialogControl() const {
    return 0;
  }
};
struct CBCGPToolbarEditBoxButton;
struct CBCGPDockingControlBar;
struct CBCGPTasksPane {
  virtual BOOL IsWrapLabelsEnabled() const {
    return 0;
  }
  virtual BOOL IsWrapTasksEnabled() const {
    return 0;
  }
};
struct CBCGPTask {
  virtual IRECT GetRect() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual BOOL IsSeparator() const {
    return 0;
  }
  virtual BOOL IsBold() const {
    return 0;
  }
  virtual BOOL IsEnabled() const {
    return 0;
  }
  virtual int GetIconIndex() const {
    return 0;
  }
  virtual int GetCommandID() const {
    return 0;
  }
  virtual int GetTasksIconHorzOffset() const {
    return 0;
  }
  virtual COLOR GetTextColor() const {
    return 0;
  }
  virtual COLOR GetTextHotColor() const {
    return 0;
  }
  virtual CBCGPTasksPane* GetTaskPane() const {
    return 0;
  }
  virtual CString GetName() const {
    return "";
  }
};
struct CBCGPTasksGroup {
  virtual int GetClass() const {
    return 0;
  }
  virtual int GetGroupCaptionHorzOffset() const {
    return 0;
  }
  virtual int GetGroupCaptionVertOffset() const {
    return 0;
  }
  virtual CString GetStrName() const {
    return "";
  }
  virtual IRECT GetRect() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual BOOL IsCollapsed() const {
    return 0;
  }
  virtual BOOL IsSpecial() const {
    return 0;
  }
  virtual img_t* GetIcon() const {
    return 0;
  }
  virtual COLOR GetTextHotColor() const {
    return 0;
  }
  virtual COLOR GetTextColor() const {
    return 0;
  }
  virtual CString GetName() const {
    return "";
  }
};
struct CBCGPSlider : public CBCGPBaseControlBar {
  virtual int GetClass() const {
    return 0;
  }
  virtual int GetCurrentAlignment() const {
    return 0;
  }
  virtual int ScreenToClient(IRECT* prc) const {
    return 0;
  }
};
struct CBCGPSplitterWnd;
struct CBCGPAutoHideButton;
struct CBCGPCalculator;
struct CBCGPEdit;
struct CBCGPSpinButtonCtrl;
struct CBCGPAppBarWnd;
struct CBCGPPopupWindow;
struct CBCGPPlannerManagerCtrl {
  virtual COLOR GetBackgroundColor() const {
    return 0;
  }
  virtual int GetType() const {
    return 0;
  }
};
struct CBCGPPlannerView {
  virtual CBCGPPlannerManagerCtrl* GetPlanner() {
    return 0;
  }
  virtual int GetClass() const {
    return 0;
  }
  virtual CBrush* GetPlannerBrush() const {
    return 0;
  }
};
struct ColorData {
  COLORREF  m_clrBackground;
  COLORREF  m_clrText;
  COLORREF  m_clrGradient;
  COLORREF  m_clrBorder;
  int       m_nGradientAngle; // 0 - 360

  void InitColors() {
    m_clrBackground = (COLORREF) - 1;
    m_clrText   = (COLORREF) - 1;
    m_clrGradient = (COLORREF) - 1;
    m_nGradientAngle = 90;
    m_clrBorder   = (COLORREF) - 1;
  }

  //BOOL Draw (CDC* pDC, CRect rect, BOOL bNoBorder = FALSE);
};

struct BCGP_GRID_COLOR_DATA {
  COLORREF  m_clrBackground;  // Grid background color
  COLORREF  m_clrText;      // Grid foreground color
  COLORREF  m_clrHeader;    // H foreground color

  ColorData m_EvenColors;   // Even rows color
  ColorData m_OddColors;    // Odd rows color

  ColorData m_GroupColors;    // Group colors
  ColorData m_GroupSelColors; // Selected group colors

  ColorData m_SelColors;    // Selected rows color

  ColorData m_HeaderColors;   // Header item color
  ColorData m_HeaderSelColors;  // Selected header item color

  ColorData m_LeftOffsetColors; // Left offset colors

  COLORREF  m_clrHorzLine;    // Grid horizontal line text
  COLORREF  m_clrVertLine;    // Grid vertical line text

  COLORREF  m_clrPreviewText; // Description text color
};

struct CBCGPGridCtrl {
  virtual BOOL IsControlBarColors() const {
    return 0;
  }
  virtual BOOL IsHighlightGroups() const {
    return 0;
  }
  virtual BOOL IsFocused() const {
    return 0;
  }
  virtual COLOR GetBkColor() const {
    return 0;
  }
  virtual COLOR GetSortedColor() const {
    return 0;
  }
};


struct CBCGPCalendar;

struct CBCGPCalendarColors {
  COLORREF clrCaption;
  COLORREF clrCaptionText;
  COLORREF clrSelected;
  COLORREF clrSelectedText;
  COLORREF clrTodayBorder;
};
struct CBCGPPropList {
  virtual BOOL DrawControlBarColors() const {
    return 0;
  }
};
struct CBCGPRibbonBar {
  virtual BOOL IsQuickAccessToolbarOnTop() const {
    return 0;
  }
  virtual BOOL IsQATEmpty() const {
    return 0;
  }
  virtual BOOL IsTransparentCaption() const {
    return 0;
  }
  virtual BOOL IsScenicLook() const {
    return 0;
  }
  virtual int GetExStyle() const {
    return 0;
  }
  virtual int GetHideFlags() const {
    return 0;
  }
  virtual IRECT GetQuickAccessToolbarLocation() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual CBitmap* GetWndIcon() const {
    return 0;
  }
  virtual int GetWindowText(CString& text) const {
    return 0;
  }
  virtual int GetTabTrancateRatio() const {
    return 0;
  }
};
struct CBCGPRibbonCategory {
  virtual IRECT GetTabRect() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual CBCGPRibbonBar* GetParentRibbonBar() const {
    return 0;
  }
  virtual BCGPRibbonCategoryColor GetTabColor() const {
    return (BCGPRibbonCategoryColor)0;
  }
};
struct CBCGPRibbonCategoryScroll {
  virtual IRECT GetRect() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual BOOL IsHighlighted() const {
    return 0;
  }
  virtual BOOL IsLeftScroll() const {
    return 0;
  }
};
struct CBCGPRibbonPanel {
  virtual BOOL IsCollapsed() const {
    return 0;
  }
  virtual BOOL IsHighlighted() const {
    return 0;
  }
  virtual CBCGPRibbonButton* GetDefaultButton() const {
    return 0;
  }
  virtual CBCGPRibbonButton* GetLaunchButton() const {
    return 0;
  }
  virtual CString GetName() const {
    return "";
  }
};
struct CBCGPBaseRibbonElement {
  virtual BOOL IsDisabled() const {
    return 0;
  }
};
struct CBCGPRibbonTab {
  virtual IRECT GetRect() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual BOOL IsHighlighted() const {
    return 0;
  }
  virtual BOOL IsLeftScroll() const {
    return 0;
  }
  virtual BOOL IsFocused() const {
    return 0;
  }
  virtual BOOL IsDroppedDown() const {
    return 0;
  }
  virtual BOOL IsSelected() const {
    return 0;
  }
  virtual CBCGPRibbonCategory* GetParentCategory() const {
    return 0;
  }
  virtual CBCGPBaseRibbonElement* GetDroppedDown() const {
    return 0;
  }
};
struct CBCGPRibbonButtonsGroup;

struct CBCGPRibbonEditCtrl;
struct CBCGPRibbonMainPanel;
struct CBCGPRibbonLabel;
struct CBCGPRibbonContextCaption {
  virtual BCGPRibbonCategoryColor GetColor() const {
    return (BCGPRibbonCategoryColor)0;
  }
  virtual IRECT GetRect() const {
    return iRECT(0, 0, 0, 0);
  }
};
struct CBCGPToolTipParams;
struct CBCGTabButton;
struct CBCGPTabWnd;
enum BCGPRibbonCategoryColor;
struct CBCGPRibbonStatusBar;
struct CBCGPRibbonStatusBarPane {
  virtual IRECT GetRect() const {
    return iRECT(0, 0, 0, 0);
  }
  virtual BOOL IsHighlighted() const {
    return 0;
  }
  virtual BOOL IsPressed() const {
    return 0;
  }
};
struct CBCGPRibbonSlider {
  virtual BOOL IsVert() const {
    return 0;
  }
  virtual BOOL IsThumbLeftTop() const {
    return 0;
  }
  virtual BOOL IsThumbRightBottom() const {
    return 0;
  }
};
struct CBCGPRibbonProgressBar;
struct CBCGPRibbonPanelMenuBar;
struct CBCGPRibbonPanelMenu;
struct CBCGPRibbonSeparator;
struct CBCGPPropertySheet;
struct CBCGPRibbonCheckBox;
struct CBCGPRibbonHyperlink {
  virtual BOOL IsHighlighted() const {
    return 0;
  }
  virtual BOOL IsDisabled() const {
    return 0;
  }
};
struct CBCGPScrollBar;
struct CBCGPToolTipCtrl;
struct CBCGPGroup {
  virtual BOOL IsOnGlass() const {
    return 0;
  }
};
struct CBCGPSliderCtrl;
struct CBCGPListBox;

struct CBCGPToolTipParams {
  BOOL      m_bBallonTooltip;
  BOOL      m_bDrawIcon;
  BOOL      m_bDrawDescription;
  int       m_nMaxDescrWidth;
  BOOL      m_bRoundedCorners;
  BOOL      m_bBoldLabel;
  COLORREF    m_clrFill;
  COLORREF    m_clrFillGradient;
  int       m_nGradientAngle; // 0 - 360, -1 -default (vertical)
  COLORREF    m_clrText;
  COLORREF    m_clrBorder;
  BOOL      m_bDrawSeparator;
  BOOL      m_bVislManagerTheme;

  CBCGPToolTipParams() {
    m_bBallonTooltip  = FALSE;
    m_bDrawIcon     = TRUE;
    m_bDrawDescription  = TRUE;
    m_nMaxDescrWidth  = 200;
    m_bRoundedCorners = FALSE;
    m_bBoldLabel    = TRUE;
    m_clrFill     = (COLORREF) - 1;
    m_clrFillGradient = (COLORREF) - 1;
    m_nGradientAngle  = -1;
    m_clrText     = (COLORREF) - 1;
    m_clrBorder     = (COLORREF) - 1;
    m_bDrawSeparator  = FALSE;
    m_bVislManagerTheme = FALSE;
  }

  CBCGPToolTipParams& operator= (CBCGPToolTipParams& src) {
    m_bBallonTooltip  = src.m_bBallonTooltip;
    m_bDrawIcon     = src.m_bDrawIcon;
    m_bDrawDescription  = src.m_bDrawDescription;
    m_nMaxDescrWidth  = src.m_nMaxDescrWidth;
    m_bRoundedCorners = src.m_bRoundedCorners;
    m_bBoldLabel    = src.m_bBoldLabel;
    m_clrFill     = src.m_clrFill;
    m_clrFillGradient = src.m_clrFillGradient;
    m_nGradientAngle  = src.m_nGradientAngle;
    m_clrText     = src.m_clrText;
    m_clrBorder     = src.m_clrBorder;
    m_bDrawSeparator  = src.m_bDrawSeparator;
    m_bVislManagerTheme = src.m_bVislManagerTheme;

    return *this;
  }
};


struct BCGP_GANTT_CHART_COLORS {
  COLORREF  clrBackground;
  COLORREF  clrRowBackground;
  COLORREF  clrRowDayOff;
  COLORREF  clrBarFill;
  COLORREF  clrGroupBarFill;
  COLORREF  clrBarComplete;
  COLORREF  clrGridLine0;
  COLORREF  clrGridLine1;
  COLORREF  clrConnectorLines;
  COLORREF  clrShadows;
  COLORREF  clrSelection;
  COLORREF  clrSelectionBorder;

  BCGP_GANTT_CHART_COLORS::BCGP_GANTT_CHART_COLORS()
    : clrBackground(CLR_DEFAULT)
    , clrRowBackground(CLR_DEFAULT)
    , clrBarFill(CLR_DEFAULT)
    , clrBarComplete(CLR_DEFAULT)
    , clrGroupBarFill(CLR_DEFAULT)
    , clrGridLine0(CLR_DEFAULT)
    , clrGridLine1(CLR_DEFAULT)
    , clrConnectorLines(CLR_DEFAULT)
    , clrShadows(CLR_DEFAULT)
    , clrSelection(CLR_DEFAULT)
    , clrSelectionBorder(CLR_DEFAULT)
    , clrRowDayOff(CLR_DEFAULT) {
  }
};

#define  TIMEFORMAT_NONE                0
#define  TIMEFORMAT_HOURS_ONLY          1
#define  TIMEFORMAT_MINUTES_ONLY        2
#define  TIMEFORMAT_HOURS_AND_MINUTES   3

struct BCGP_GANTT_CHART_HEADER {
  BOOL          bVisible;
  BOOL          bAboveChart;
  DWORD         dwAlignment;
  UINT          nHeight;
  COleDateTimeSpan    dtCellTimeDelta;
  CString         sDateFormat;
  DWORD               dwTimeFormat;

  BCGP_GANTT_CHART_HEADER::BCGP_GANTT_CHART_HEADER()
    : bVisible(TRUE)
    , bAboveChart(TRUE)
    , dwAlignment(DT_LEFT)
    , nHeight(21)
    , dtCellTimeDelta(1, 0, 0, 0)
    , dwTimeFormat(TIMEFORMAT_NONE) {
  }

};

struct BCGP_GANTT_CHART_HEADER_CELL_INFO {
  CRect       rectCell;         // cell area (in header)
  CRect       rectColumn;       // column area. Defines visible chart area for this column.
  CRect       rectClip;         // clipping rectangle
  COleDateTime    dtCellLeftMostTime; // starting time for cell
  COleDateTimeSpan  dtCellTimeDelta;
  const BCGP_GANTT_CHART_HEADER*    pHeaderInfo;

  BCGP_GANTT_CHART_HEADER_CELL_INFO::BCGP_GANTT_CHART_HEADER_CELL_INFO()
    : rectCell(0, 0, 0, 0)
    , rectColumn(0, 0, 0, 0)
    , rectClip(0, 0, 0, 0)
    , pHeaderInfo(NULL)
  {}

  bool BCGP_GANTT_CHART_HEADER_CELL_INFO::operator == (const BCGP_GANTT_CHART_HEADER_CELL_INFO& rhs) const {
    return (pHeaderInfo == rhs.pHeaderInfo && dtCellLeftMostTime == rhs.dtCellLeftMostTime);
  }

  bool BCGP_GANTT_CHART_HEADER_CELL_INFO::operator != (const BCGP_GANTT_CHART_HEADER_CELL_INFO& rhs) const {
    return !(*this == rhs);
  }

  bool BCGP_GANTT_CHART_HEADER_CELL_INFO::Exists() const {
    return pHeaderInfo != NULL;
  }

  void BCGP_GANTT_CHART_HEADER_CELL_INFO::Reset() {
    *this = BCGP_GANTT_CHART_HEADER_CELL_INFO();
  }


};




struct CBCGPGanttChart;
struct CBCGPGanttItem;
struct BCGP_GANTT_CHART_COLORS;
struct BCGP_GANTT_CHART_HEADER_CELL_INFO;
struct BCGP_GRID_COLOR_DATA;


struct CBCGPGlobalUtils {
public:
  CBCGPGlobalUtils::CBCGPGlobalUtils() {
    m_bDialogApp = FALSE;
    m_bIsDragging = FALSE;
  }


#if (!defined _BCGSUITE_) && (!defined _BCGSUITE_INC_)
  BOOL CheckAlignment(CPoint point, CBCGPBaseControlBar* pBar,
      int nSencitivity,
      const CBCGPDockManager* pDockManager,
      BOOL bOuterEdge,
      DWORD& dwAlignment,
      DWORD dwEnabledDockBars = CBRS_ALIGN_ANY,
      LPCRECT lpRectBounds = NULL) const;

  CBCGPDockManager* GetDockManager(CWnd* pWnd);

  void FlipRect(CRect& rect, int nDegrees);
  DWORD GetOppositeAlignment(DWORD dwAlign);
  void SetNewParent(CObList& lstControlBars, CWnd* pNewParent,
      BOOL bCheckVisibility = TRUE);

  void CalcExpectedDockedRect(CBCGPBarContainerManager& barContainerManager,
      CWnd* pWndToDock, CPoint ptMouse, CRect& rectResult, BOOL& bDrawTab,
      CBCGPDockingControlBar** ppTargetBar);
  BOOL GetCBAndAlignFromPoint(CBCGPBarContainerManager& barContainerManager,
      CPoint pt, CBCGPDockingControlBar** ppTargetControlBar,
      DWORD& dwAlignment, BOOL& bTabArea, BOOL& bCaption);
  void ForceAdjustLayout(CBCGPDockManager* pDockManager, BOOL bForce = FALSE, BOOL bForceInvisible = FALSE);
  BOOL IsWndCanFloatMulti(CWnd* pWnd) const;
  void AdjustRectToWorkArea(CRect& rect, CRect* pRectDelta = NULL);
  BOOL CanBeAttached(CWnd* pWnd) const;
#endif

  BOOL StringFromCy(CString& str, CY& cy);
  BOOL CyFromString(CY& cy, LPCTSTR psz);
  BOOL StringFromDecimal(CString& str, DECIMAL& decimal);
  BOOL DecimalFromString(DECIMAL& decimal, LPCTSTR psz);

  CBitmap* GetWndIcon(CWnd* pWnd, BOOL* bDestroyIcon = NULL);

  BOOL  m_bDialogApp;
  BOOL  m_bIsDragging;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#if 0

#ifndef _BCGSUITE_

//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::CheckAlignment(CPoint point, CBCGPBaseControlBar* pBar, int nSencitivity,
    const CBCGPDockManager* pDockManager,
    BOOL bOuterEdge, DWORD& dwAlignment,
    DWORD dwEnabledDockBars, LPCRECT lpRectBounds) const
{
  BOOL bSmartDocking = FALSE;
  CBCGPSmartDockingMarker::SDMarkerPlace nHilitedSide = CBCGPSmartDockingMarker::sdNONE;

  if (pDockManager == NULL && pBar != NULL) {
    pDockManager = globalUtils.GetDockManager(pBar->GetParent());
  }

  if (pDockManager != NULL) {
    CBCGPSmartDockingManager* pSDManager = pDockManager->GetSDManagerPermanent();

    if (pSDManager != NULL && pSDManager->IsStarted()) {
      bSmartDocking = TRUE;
      nHilitedSide = pSDManager->GetHilitedMarkerNo();
    }
  }

  CRect rectBounds;

  if (pBar != NULL) {
    pBar->GetWindowRect(rectBounds);
  }
  else if (lpRectBounds != NULL) {
    rectBounds = *lpRectBounds;
  }
  else {
    ASSERT(FALSE);
    return FALSE;
  }

  int nCaptionHeight = 0;
  int nTabAreaTopHeight = 0;
  int nTabAreaBottomHeight = 0;

  CBCGPDockingControlBar* pDockingBar = DYNAMIC_DOWNCAST(CBCGPDockingControlBar, pBar);

  if (pDockingBar != NULL) {
    nCaptionHeight = pDockingBar->GetCaptionHeight();

    CRect rectTabAreaTop;
    CRect rectTabAreaBottom;
    pDockingBar->GetTabArea(rectTabAreaTop, rectTabAreaBottom);
    nTabAreaTopHeight = rectTabAreaTop.Height();
    nTabAreaBottomHeight = rectTabAreaBottom.Height();
  }

  // build rect for top area
  if (bOuterEdge) {
    if (bSmartDocking) {
      switch (nHilitedSide) {
      case CBCGPSmartDockingMarker::sdLEFT:
        dwAlignment = CBRS_ALIGN_LEFT;
        return TRUE;

      case CBCGPSmartDockingMarker::sdRIGHT:
        dwAlignment = CBRS_ALIGN_RIGHT;
        return TRUE;

      case CBCGPSmartDockingMarker::sdTOP:
        dwAlignment = CBRS_ALIGN_TOP;
        return TRUE;

      case CBCGPSmartDockingMarker::sdBOTTOM:
        dwAlignment = CBRS_ALIGN_BOTTOM;
        return TRUE;
      }
    }
    else {
      CRect rectToCheck(rectBounds.l - nSencitivity, rectBounds.t - nSencitivity,
          rectBounds.r + nSencitivity, rectBounds.t);

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_TOP) {
        dwAlignment = CBRS_ALIGN_TOP;
        return TRUE;
      }

      // build rect for left area
      rectToCheck.r = rectBounds.l;
      rectToCheck.b = rectBounds.b + nSencitivity;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_LEFT) {
        dwAlignment = CBRS_ALIGN_LEFT;
        return TRUE;
      }

      // build rect for bottom area
      rectToCheck.l = rectBounds.l - nSencitivity;
      rectToCheck.t = rectBounds.b;
      rectToCheck.r = rectBounds.r + nSencitivity;
      rectToCheck.b = rectBounds.b + nSencitivity;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_BOTTOM) {
        dwAlignment = CBRS_ALIGN_BOTTOM;
        return TRUE;
      }

      // build rect for right area
      rectToCheck.l = rectBounds.r;
      rectToCheck.t = rectBounds.t - nSencitivity;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_RIGHT) {
        dwAlignment = CBRS_ALIGN_RIGHT;
        return TRUE;
      }
    }
  }
  else {
    if (bSmartDocking) {
      switch (nHilitedSide) {
      case CBCGPSmartDockingMarker::sdCLEFT:
        dwAlignment = CBRS_ALIGN_LEFT;
        return TRUE;

      case CBCGPSmartDockingMarker::sdCRIGHT:
        dwAlignment = CBRS_ALIGN_RIGHT;
        return TRUE;

      case CBCGPSmartDockingMarker::sdCTOP:
        dwAlignment = CBRS_ALIGN_TOP;
        return TRUE;

      case CBCGPSmartDockingMarker::sdCBOTTOM:
        dwAlignment = CBRS_ALIGN_BOTTOM;
        return TRUE;
      }
    }
    else {
#ifdef __BOUNDS_FIX__
      CRect rectToCheck(rectBounds.l, rectBounds.t,
          rectBounds.r,
          rectBounds.t + nSencitivity + nCaptionHeight);

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_TOP) {
        dwAlignment = CBRS_ALIGN_TOP;
        return TRUE;
      }


      // build rect for left area
      rectToCheck.r = rectBounds.l + nSencitivity;
      rectToCheck.b = rectBounds.b + nSencitivity;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_LEFT) {
        dwAlignment = CBRS_ALIGN_LEFT;
        return TRUE;
      }

      // build rect for bottom area
      rectToCheck.l = rectBounds.l;
      rectToCheck.t = rectBounds.b - nSencitivity - nTabAreaBottomHeight;
      rectToCheck.r = rectBounds.r;
      rectToCheck.b = rectBounds.b;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_BOTTOM) {
        dwAlignment = CBRS_ALIGN_BOTTOM;
        return TRUE;
      }

      // build rect for right area
      rectToCheck.l = rectBounds.r - nSencitivity;
      rectToCheck.t = rectBounds.t - nSencitivity;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_RIGHT) {
        dwAlignment = CBRS_ALIGN_RIGHT;
        return TRUE;
      }

#else

      // build rect for top area
      CRect rectToCheck(rectBounds.l - nSencitivity, rectBounds.t - nSencitivity,
          rectBounds.r + nSencitivity,
          rectBounds.t + nSencitivity + nCaptionHeight);

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_TOP) {
        dwAlignment = CBRS_ALIGN_TOP;
        return TRUE;
      }


      // build rect for left area
      rectToCheck.r = rectBounds.l + nSencitivity;
      rectToCheck.b = rectBounds.b + nSencitivity;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_LEFT) {
        dwAlignment = CBRS_ALIGN_LEFT;
        return TRUE;
      }

      // build rect for bottom area
      rectToCheck.l = rectBounds.l - nSencitivity;
      rectToCheck.t = rectBounds.b - nSencitivity - nTabAreaBottomHeight;
      rectToCheck.r = rectBounds.r + nSencitivity;
      rectToCheck.b = rectBounds.b + nSencitivity;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_BOTTOM) {
        dwAlignment = CBRS_ALIGN_BOTTOM;
        return TRUE;
      }

      // build rect for right area
      rectToCheck.l = rectBounds.r - nSencitivity;
      rectToCheck.t = rectBounds.t - nSencitivity;

      if (rectToCheck.PtInRect(point) && dwEnabledDockBars & CBRS_ALIGN_RIGHT) {
        dwAlignment = CBRS_ALIGN_RIGHT;
        return TRUE;
      }

#endif
    }
  }

  return FALSE;
}
//------------------------------------------------------------------------//
CBCGPDockManager* CBCGPGlobalUtils::GetDockManager(CWnd* pWnd)
{
  if (pWnd == NULL) {
    return NULL;
  }

  ASSERT_VALID(pWnd);

  if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPFrameWnd))) {
    return ((CBCGPFrameWnd*) pWnd)->GetDockManager();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPMDIFrameWnd))) {
    return ((CBCGPMDIFrameWnd*) pWnd)->GetDockManager();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPOleIPFrameWnd))) {
    return ((CBCGPOleIPFrameWnd*) pWnd)->GetDockManager();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPOleDocIPFrameWnd))) {
    return ((CBCGPOleDocIPFrameWnd*) pWnd)->GetDockManager();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPMDIChildWnd))) {
    return ((CBCGPMDIChildWnd*) pWnd)->GetDockManager();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(CDialog)) ||
      pWnd->IsKindOf(RUNTIME_CLASS(CPropertySheet))) {
    m_bDialogApp = TRUE;
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPOleCntrFrameWnd))) {
    return ((CBCGPOleCntrFrameWnd*) pWnd)->GetDockManager();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPMiniFrameWnd))) {
    CBCGPMiniFrameWnd* pMiniFrameWnd = DYNAMIC_DOWNCAST(CBCGPMiniFrameWnd, pWnd);
    ASSERT_VALID(pMiniFrameWnd);

    CBCGPDockManager* pManager = pMiniFrameWnd->GetDockManager();
    return pManager != NULL ? pManager : GetDockManager(pWnd->GetParent());
  }

  return NULL;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::FlipRect(CRect& rect, int nDegrees)
{
  CRect rectTmp = rect;

  switch (nDegrees) {
  case 90:
    rect.t = rectTmp.l;
    rect.r = rectTmp.t;
    rect.b = rectTmp.r;
    rect.l = rectTmp.b;
    break;

  case 180:
    rect.t = rectTmp.b;
    rect.b = rectTmp.t;
    break;

  case 275:
  case -90:
    rect.l = rectTmp.t;
    rect.t = rectTmp.r;
    rect.r = rectTmp.b;
    rect.b = rectTmp.l;
    break;
  }
}
//------------------------------------------------------------------------//
DWORD CBCGPGlobalUtils::GetOppositeAlignment(DWORD dwAlign)
{
  switch (dwAlign & CBRS_ALIGN_ANY) {
  case CBRS_ALIGN_LEFT:
    return CBRS_ALIGN_RIGHT;

  case CBRS_ALIGN_RIGHT:
    return CBRS_ALIGN_LEFT;

  case CBRS_ALIGN_TOP:
    return CBRS_ALIGN_BOTTOM;

  case CBRS_ALIGN_BOTTOM:
    return CBRS_ALIGN_TOP;
  }

  return 0;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::SetNewParent(CObList& lstControlBars, CWnd* pNewParent,
    BOOL bCheckVisibility)
{
  ASSERT_VALID(pNewParent);

  for (POSITION pos = lstControlBars.GetHeadPosition(); pos != NULL;) {
    CBCGPBaseControlBar* pBar = (CBCGPBaseControlBar*) lstControlBars.GetNext(pos);

    if (bCheckVisibility && !pBar->IsBarVisible()) {
      continue;
    }

    if (!pBar->IsKindOf(RUNTIME_CLASS(CBCGPSlider))) {
      pBar->ShowWindow(SW_HIDE);
      pBar->SetParent(pNewParent);
      CRect rectWnd;
      pBar->GetWindowRect(rectWnd);
      pNewParent->ScreenToClient(rectWnd);

      pBar->SetWindowPos(NULL, -rectWnd.Width(), -rectWnd.Height(),
          100, 100, SWP_NOZORDER | SWP_NOSIZE  | SWP_NOACTIVATE);
      pBar->ShowWindow(SW_SHOW);
    }
    else {
      pBar->SetParent(pNewParent);
    }
  }
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::CalcExpectedDockedRect(CBCGPBarContainerManager& barContainerManager,
    CWnd* pWndToDock, CPoint ptMouse,
    CRect& rectResult, BOOL& bDrawTab,
    CBCGPDockingControlBar** ppTargetBar)
{
  ASSERT(ppTargetBar != NULL);

  DWORD dwAlignment = CBRS_ALIGN_LEFT;
  BOOL bTabArea = FALSE;
  BOOL bCaption = FALSE;
  bDrawTab = FALSE;
  *ppTargetBar = NULL;

  rectResult.SetRectEmpty();

  if (GetKeyState(VK_CONTROL) < 0) {
    return;
  }

  if (!GetCBAndAlignFromPoint(barContainerManager, ptMouse,
      ppTargetBar, dwAlignment, bTabArea, bCaption) ||
      *ppTargetBar == NULL) {
    return;
  }

  CBCGPControlBar* pBar = NULL;

  if (pWndToDock->IsKindOf(RUNTIME_CLASS(CBCGPMiniFrameWnd))) {
    CBCGPMiniFrameWnd* pMiniFrame =
        DYNAMIC_DOWNCAST(CBCGPMiniFrameWnd, pWndToDock);
    ASSERT_VALID(pMiniFrame);
    pBar = DYNAMIC_DOWNCAST(CBCGPControlBar, pMiniFrame->GetFirstVisibleBar());
  }
  else {
    pBar = DYNAMIC_DOWNCAST(CBCGPControlBar, pWndToDock);
  }

  if (*ppTargetBar != NULL) {
    DWORD dwTargetEnabledAlign = (*ppTargetBar)->GetEnabledAlignment();
    DWORD dwTargetCurrentAlign = (*ppTargetBar)->GetCurrentAlignment();
    BOOL bTargetBarIsFloating = ((*ppTargetBar)->GetParentMiniFrame() != NULL);

    if (pBar != NULL) {
      if (pBar->GetEnabledAlignment() != dwTargetEnabledAlign && bTargetBarIsFloating ||
          (pBar->GetEnabledAlignment() & dwTargetCurrentAlign) == 0 && !bTargetBarIsFloating) {
        return;
      }
    }
  }

  if (bTabArea || bCaption) {
    // can't make tab on miniframe
    bDrawTab = ((*ppTargetBar) != NULL);

    if (bDrawTab) {
      bDrawTab = (*ppTargetBar)->CanBeAttached() && CanBeAttached(pWndToDock) &&
          pBar != NULL && ((*ppTargetBar)->GetEnabledAlignment() == pBar->GetEnabledAlignment());
    }

    if (!bDrawTab) {
      return;
    }

  }

  if ((*ppTargetBar) != NULL && (*ppTargetBar)->GetParentMiniFrame() != NULL &&
      !IsWndCanFloatMulti(pWndToDock)) {
    bDrawTab = FALSE;
    return;
  }

  if ((*ppTargetBar) != NULL &&
      pWndToDock->IsKindOf(RUNTIME_CLASS(CBCGPBaseControlBar)) &&
      !(*ppTargetBar)->CanAcceptBar((CBCGPBaseControlBar*) pWndToDock)) {
    bDrawTab = FALSE;
    return;
  }

  CRect rectOriginal;
  (*ppTargetBar)->GetWindowRect(rectOriginal);

  if ((*ppTargetBar) == pWndToDock ||
      pWndToDock->IsKindOf(RUNTIME_CLASS(CBCGPMiniFrameWnd)) &&
      (*ppTargetBar)->GetParentMiniFrame() == pWndToDock) {
    bDrawTab = FALSE;
    return;
  }

  CRect rectInserted;
  CRect rectSlider;
  DWORD dwSliderStyle;
  CSize sizeMinOriginal(0, 0);
  CSize sizeMinInserted(0, 0);


  pWndToDock->GetWindowRect(rectInserted);

  if ((dwAlignment & pBar->GetEnabledAlignment()) != 0 ||
      CBCGPDockManager::m_bIgnoreEnabledAlignment) {
    barContainerManager.CalcRects(rectOriginal, rectInserted, rectSlider, dwSliderStyle,
        dwAlignment, sizeMinOriginal, sizeMinInserted);
    rectResult = rectInserted;
  }

}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::CanBeAttached(CWnd* pWnd) const
{
  ASSERT_VALID(pWnd);

  if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPMiniFrameWnd))) {
    return ((CBCGPMiniFrameWnd*) pWnd)->CanBeAttached();
  }

  if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPControlBar))) {
    return ((CBCGPControlBar*) pWnd)->CanBeAttached();
  }

  return FALSE;

}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::IsWndCanFloatMulti(CWnd* pWnd) const
{
  CBCGPControlBar* pBar = NULL;

  CBCGPMiniFrameWnd* pMiniFrame =
      DYNAMIC_DOWNCAST(CBCGPMiniFrameWnd, pWnd);

  if (pMiniFrame != NULL) {
    pBar = DYNAMIC_DOWNCAST(CBCGPControlBar, pMiniFrame->GetControlBar());
  }
  else {
    pBar = DYNAMIC_DOWNCAST(CBCGPControlBar, pWnd);

  }

  if (pBar == NULL) {
    return FALSE;
  }

  if (pBar->IsTabbed()) {
    CWnd* pParentMiniFrame = pBar->GetParentMiniFrame();

    // tabbed bar that is floating in multi miniframe
    if (pParentMiniFrame != NULL && pParentMiniFrame->IsKindOf(RUNTIME_CLASS(CBCGPMultiMiniFrameWnd))) {
      return TRUE;
    }
  }


  return ((pBar->GetBarStyle() & CBRS_FLOAT_MULTI) != 0);
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::GetCBAndAlignFromPoint(CBCGPBarContainerManager& barContainerManager,
    CPoint pt,
    CBCGPDockingControlBar** ppTargetControlBar,
    DWORD& dwAlignment,
    BOOL& bTabArea,
    BOOL& bCaption)
{
  ASSERT(ppTargetControlBar != NULL);
  *ppTargetControlBar = NULL;

  BOOL bOuterEdge = FALSE;

  // if the mouse is over a miniframe's caption and this miniframe has only one
  // visible docking control bar, we need to draw a tab
  bCaption = barContainerManager.CheckForMiniFrameAndCaption(pt, ppTargetControlBar);

  if (bCaption) {
    return TRUE;
  }


  *ppTargetControlBar =
      barContainerManager.ControlBarFromPoint(pt, CBCGPDockManager::m_nDockSencitivity,
          TRUE, bTabArea, bCaption);

  if ((bCaption || bTabArea) && *ppTargetControlBar != NULL) {
    return TRUE;
  }

  if (*ppTargetControlBar == NULL) {
    barContainerManager.ControlBarFromPoint(pt, CBCGPDockManager::m_nDockSencitivity,
        FALSE, bTabArea, bCaption);
    // the exact bar was not found - it means the docked frame at the outer edge
    bOuterEdge = TRUE;
    return TRUE;
  }

  if (*ppTargetControlBar != NULL) {
    if (!globalUtils.CheckAlignment(pt, *ppTargetControlBar,
        CBCGPDockManager::m_nDockSencitivity, NULL,
        bOuterEdge, dwAlignment)) {
      // unable for some reason to determine alignment
      *ppTargetControlBar = NULL;
    }
  }

  return TRUE;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::AdjustRectToWorkArea(CRect& rect, CRect* pRectDelta)
{
  CPoint ptStart;

  if (m_bIsDragging) {
    ::GetCursorPos(&ptStart);
  }
  else {
    ptStart = rect.TopLeft();
  }

  CRect rectScreen;
  MONITORINFO mi;
  mi.cbSize = sizeof(MONITORINFO);

  if (GetMonitorInfo(
      MonitorFromPoint(ptStart, MONITOR_DEFAULTTONEAREST),
      &mi)) {
    rectScreen = mi.rcWork;
  }
  else {
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
  }

  int nDelta = pRectDelta != NULL ? pRectDelta->l : 0;

  if (rect.r <= rectScreen.l + nDelta) {
    rect.OffsetRect(rectScreen.l - rect.r + nDelta, 0);
  }

  nDelta = pRectDelta != NULL ? pRectDelta->r : 0;

  if (rect.l >= rectScreen.r - nDelta) {
    rect.OffsetRect(rectScreen.r - rect.l - nDelta, 0);
  }

  nDelta = pRectDelta != NULL ? pRectDelta->b : 0;

  if (rect.t >= rectScreen.b - nDelta) {
    rect.OffsetRect(0, rectScreen.b - rect.t - nDelta);
  }

  nDelta = pRectDelta != NULL ? pRectDelta->t : 0;

  if (rect.b < rectScreen.t + nDelta) {
    rect.OffsetRect(0, rectScreen.t - rect.b + nDelta);
  }
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::ForceAdjustLayout(CBCGPDockManager* pDockManager, BOOL bForce,
    BOOL bForceInvisible)
{
  if (pDockManager != NULL &&
      (CBCGPControlBar::m_bHandleMinSize || bForce)) {
    CWnd* pDockSite = pDockManager->GetDockSite();

    if (pDockSite == NULL) {
      return;
    }

    if (!pDockSite->IsWindowVisible() && !bForceInvisible) {
      return;
    }

    CRect rectWnd;
    pDockManager->GetDockSite()->SetRedraw(FALSE);
    pDockManager->GetDockSite()->GetWindowRect(rectWnd);
    pDockManager->GetDockSite()->SetWindowPos(NULL, -1, -1,
        rectWnd.Width() + 1, rectWnd.Height() + 1,
        SWP_NOZORDER |  SWP_NOMOVE | SWP_NOACTIVATE);
    pDockManager->GetDockSite()->SetWindowPos(NULL, -1, -1,
        rectWnd.Width(), rectWnd.Height(),
        SWP_NOZORDER |  SWP_NOMOVE  | SWP_NOACTIVATE);
    pDockManager->GetDockSite()->SetRedraw(TRUE);
    pDockManager->GetDockSite()->RedrawWindow(NULL, NULL,
        RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
  }
}

#endif

//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::StringFromCy(CString& str, CY& cy)
{
  VARIANTARG varCy;
  VARIANTARG varBstr;
  AfxVariantInit(&varCy);
  AfxVariantInit(&varBstr);
  V_VT(&varCy) = VT_CY;
  V_CY(&varCy) = cy;

  if (FAILED(VariantChangeType(&varBstr, &varCy, 0, VT_BSTR))) {
    VariantClear(&varCy);
    VariantClear(&varBstr);
    return FALSE;
  }

  str = V_BSTR(&varBstr);
  VariantClear(&varCy);
  VariantClear(&varBstr);
  return TRUE;
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::CyFromString(CY& cy, LPCTSTR psz)
{
  USES_CONVERSION;

  if (psz == NULL || _tcslen(psz) == 0) {
    psz = _T("0");
  }

  VARIANTARG varBstr;
  VARIANTARG varCy;
  AfxVariantInit(&varBstr);
  AfxVariantInit(&varCy);
  V_VT(&varBstr) = VT_BSTR;
  V_BSTR(&varBstr) = SysAllocString(T2COLE(psz));

  if (FAILED(VariantChangeType(&varCy, &varBstr, 0, VT_CY))) {
    VariantClear(&varBstr);
    VariantClear(&varCy);
    return FALSE;
  }

  cy = V_CY(&varCy);
  VariantClear(&varBstr);
  VariantClear(&varCy);
  return TRUE;
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::StringFromDecimal(CString& str, DECIMAL& decimal)
{
  VARIANTARG varDecimal;
  VARIANTARG varBstr;
  AfxVariantInit(&varDecimal);
  AfxVariantInit(&varBstr);
  V_VT(&varDecimal) = VT_DECIMAL;
  V_DECIMAL(&varDecimal) = decimal;

  if (FAILED(VariantChangeType(&varBstr, &varDecimal, 0, VT_BSTR))) {
    VariantClear(&varDecimal);
    VariantClear(&varBstr);
    return FALSE;
  }

  str = V_BSTR(&varBstr);
  VariantClear(&varDecimal);
  VariantClear(&varBstr);
  return TRUE;
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::DecimalFromString(DECIMAL& decimal, LPCTSTR psz)
{
  USES_CONVERSION;

  if (psz == NULL || _tcslen(psz) == 0) {
    psz = _T("0");
  }

  VARIANTARG varBstr;
  VARIANTARG varDecimal;
  AfxVariantInit(&varBstr);
  AfxVariantInit(&varDecimal);
  V_VT(&varBstr) = VT_BSTR;
  V_BSTR(&varBstr) = SysAllocString(T2COLE(psz));

  if (FAILED(VariantChangeType(&varDecimal, &varBstr, 0, VT_DECIMAL))) {
    VariantClear(&varBstr);
    VariantClear(&varDecimal);
    return FALSE;
  }

  decimal = V_DECIMAL(&varDecimal);
  VariantClear(&varBstr);
  VariantClear(&varDecimal);
  return TRUE;
}
//------------------------------------------------------------------------//
CBitmap* CBCGPGlobalUtils::GetWndIcon(CWnd* pWnd, BOOL* bDestroyIcon)
{
  ASSERT_VALID(pWnd);

  if (pWnd->GetSafeHwnd() == NULL) {
    return NULL;
  }

  if (bDestroyIcon != NULL) {
    *bDestroyIcon = FALSE;
  }

  CBitmap* hIcon = pWnd->GetIcon(FALSE);

  if (hIcon == NULL) {
    hIcon = pWnd->GetIcon(TRUE);

    if (hIcon != NULL) {
      CImageList il;
      il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1);
      il.Add(hIcon);

      if (il.GetImageCount() == 1) {
        hIcon = il.ExtractIcon(0);

        if (bDestroyIcon != NULL) {
          *bDestroyIcon = hIcon != NULL;
        }
      }
    }
  }

  if (hIcon == NULL) {
    hIcon = (CBitmap*)(LONG_PTR)::GetClassLongPtr(pWnd->GetSafeHwnd(), GCLP_HICONSM);
  }

  if (hIcon == NULL) {
    hIcon = (CBitmap*)(LONG_PTR)::GetClassLongPtr(pWnd->GetSafeHwnd(), GCLP_HICON);
  }

  return hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPAboutDlg dialog

class CBCGPAboutDlg : public CBCGPDialog
{
  // Construction
public:
  CBCGPAboutDlg(LPCTSTR lpszAppName, CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CBCGPAboutDlg)
  enum { IDD = IDD_BCGBARRES_ABOUT_DLG };
  CBCGPURLLinkButton  m_wndURL;
  CButton m_wndPurchaseBtn;
  CStatic m_wndAppName;
  CBCGPURLLinkButton  m_wndEMail;
  CString m_strAppName;
  CString m_strVersion;
  CString m_strYear;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CBCGPAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CBCGPAboutDlg)
  afx_msg void OnBcgbarresPurchase();
  virtual BOOL OnInitDialog();
  afx_msg void OnPaint(CDC* pDC);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  CFont       m_fontBold;
  CBCGPToolBarImages  m_Logo;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPAboutDlg dialog

CBCGPAboutDlg::CBCGPAboutDlg(LPCTSTR lpszAppName, CWnd* pParent /*=NULL*/)
  : CBCGPDialog(CBCGPAboutDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CBCGPAboutDlg)
  m_strAppName = _T("");
  m_strVersion = _T("");
  m_strYear = _T("");
  //}}AFX_DATA_INIT

  m_strVersion.Format(_T("%d.%d"), _BCGCBPRO_VERSION_MAJOR, _BCGCBPRO_VERSION_MINOR);

  CString strCurrDate = _T(__DATE__);
  m_strYear.Format(_T("1998-%s"), (LPCTSTR)strCurrDate.Right(4));

  m_strAppName = lpszAppName;

#ifndef _BCGSUITE_
  EnableVisualManagerStyle(globalData.m_bUseVisualManagerInBuiltInDialogs, TRUE);
#endif
}

void CBCGPAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CBCGPDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CBCGPAboutDlg)
  DDX_Control(pDX, IDC_BCGBARRES_URL, m_wndURL);
  DDX_Control(pDX, IDC_BCGBARRES_PURCHASE, m_wndPurchaseBtn);
  DDX_Control(pDX, IDC_BCGBARRES_NAME, m_wndAppName);
  DDX_Control(pDX, IDC_BCGBARRES_MAIL, m_wndEMail);
  DDX_Text(pDX, IDC_BCGBARRES_NAME, m_strAppName);
  DDX_Text(pDX, IDC_BCGBARRES_VERSION, m_strVersion);
  DDX_Text(pDX, IDC_BCGBARRES_YEAR, m_strYear);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBCGPAboutDlg, CBCGPDialog)
  //{{AFX_MSG_MAP(CBCGPAboutDlg)
  ON_BN_CLICKED(IDC_BCGBARRES_PURCHASE, OnBcgbarresPurchase)
  ON_WM_PAINT()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPAboutDlg message handlers

void CBCGPAboutDlg::OnBcgbarresPurchase()
{
#ifndef _BCGSUITE_
  CString strURL = _T("http://www.bcgsoft.com/register-bcgcbpe.htm");
#else
  CString strURL = _T("http://www.bcgsoft.com/register-bcgsuite.htm");
#endif

  ::ShellExecute(NULL, _T("open"), strURL, NULL, NULL, SW_SHOWNORMAL);

  EndDialog(IDOK);
}

BOOL CBCGPAboutDlg::OnInitDialog()
{
  CBCGPDialog::OnInitDialog();

  //------------------
  // Create bold font:
  //------------------
  CFont* pFont = m_wndAppName.GetFont();
  ASSERT_VALID(pFont);

  LOGFONT lf;
  memset(&lf, 0, sizeof(LOGFONT));

  pFont->GetLogFont(&lf);

  lf.lfWeight = FW_BOLD;
  m_fontBold.CreateFontIndirect(&lf);

  m_wndAppName.SetFont(&m_fontBold);

  //-----------------------------
  // Setup URL and e-mail fields:
  //-----------------------------
  m_wndEMail.SetURLPrefix(_T("mailto:"));
  m_wndEMail.SetURL(_T("info@bcgsoft.com"));
  m_wndEMail.SizeToContent();
  m_wndEMail.SetTooltip(_T("Send mail to us"));
  m_wndEMail.m_bDrawFocus = FALSE;

  m_wndURL.m_bDrawFocus = FALSE;
  m_wndURL.SizeToContent();

  //--------------------
  // Set dialog caption:
  //--------------------
  CString strCaption;
  strCaption.Format(_T("About %s"), m_strAppName);

  SetWindowText(strCaption);

  //----------------------------
  // Hide Logo in High DPI mode:
  //----------------------------
  if (globalData.GetRibbonImageScale() > 1.) {
    CWnd* pWndLogo = GetDlgItem(IDC_BCGBARRES_DRAW_AREA);

    if (pWndLogo->GetSafeHwnd() != NULL) {
      m_Logo.Load(IDB_BCGBARRES_LOGO);
      m_Logo.SetSingleImage();
      m_Logo.SmoothResize(globalData.GetRibbonImageScale());

      pWndLogo->ShowWindow(SW_HIDE);
    }
  }

  //------------------------------------------
  // Hide "Purchase" button in retail version:
  //------------------------------------------
#ifndef _BCGCBPRO_EVAL_
  m_wndPurchaseBtn.EnableWindow(FALSE);
  m_wndPurchaseBtn.ShowWindow(SW_HIDE);
#endif

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void BCGPShowAboutDlg(LPCTSTR lpszAppName)
{
  CBCGPLocalResource locaRes;

  CBCGPAboutDlg dlg(lpszAppName);
  dlg.DoModal();
}

void CBCGPAboutDlg::OnPaint(CDC* pDC)
{
  CPaintDC dc(this); // device context for painting

  if (!m_Logo.IsValid()) {
    return;
  }

  CRect rectClient;
  GetClientRect(&rectClient);

  m_Logo.DrawEx(&dc, rectClient, 0);
}

void BCGPShowAboutDlg(UINT uiAppNameResID)
{
  CString strAppName;
  strAppName.LoadString(uiAppNameResID);

  BCGPShowAboutDlg(strAppName);
}

#endif

CBCGPGlobalUtils globalUtils;

#endif // !defined(AFX_BCGPGLOBALUTILS_H__FD3579BF_C46C_439C_AD13_5DFDA777F240__INCLUDED_)
