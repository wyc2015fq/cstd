
//
// BCGPVisualManager.h: interface for the CBCGPPVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPVISUALMANAGER_H__8070C021_60CE_11D5_A304_8B7130518C10__INCLUDED_)
#define AFX_BCGPVISUALMANAGER_H__8070C021_60CE_11D5_A304_8B7130518C10__INCLUDED_

#define SPIN_PRESSEDUP      0x0001
#define SPIN_PRESSEDDOWN    0x0002
#define SPIN_HIGHLIGHTEDUP    0x0004
#define SPIN_HIGHLIGHTEDDOWN  0x0008
#define SPIN_DISABLED     0x0010

#define RGB_TO_RGBQUAD(r,g,b)   (_RGB(b,g,r))
#define CLR_TO_RGBQUAD(clr)     (_RGB(GetBV(clr), GetGV(clr), GetRV(clr)))
#define RGBQUAD_TO_CLR(clr)     (_RGB(GetBV(clr), GetGV(clr), GetRV(clr)))

// Raster Ops
#define ROP_DSPDxax  0x00E20746L
#define ROP_PSDPxax  0x00B8074AL

// Internal images:
#define BCGImage_Light  0
#define BCGImage_Shadow 1

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

static inline double clamp(double value, double low, double high)
{
  return value < low ? low : (value > high ? high : value);
}

static inline double clamp_to_byte(double value)
{
  return clamp(value, 0.0, 255.0);
}

enum WinXpTheme {
  WinXpTheme_None = -1,
  WinXpTheme_NonStandard,
  WinXpTheme_Blue,
  WinXpTheme_Olive,
  WinXpTheme_Silver
};


class CBCGPVisualManager : public BCGPGLOBAL_DATA
{
  friend class CBCGPSkinManager;

  DECLARE_DYNCREATE(CBCGPVisualManager)

public:
  CBCGPVisualManager();
  virtual ~CBCGPVisualManager();

  virtual BOOL IsWinXPThemeSupported() const  {
    return FALSE;
  }
  static CBCGPVisualManager* GetInstance();

  // Overrides:
public:

  void CBCGPVisualManager::FillRebarPane(CDC* pDC, CBCGPBaseControlBar* pBar, CRect rectClient);
  virtual void OnUpdateSystemColors();

  virtual void OnFillBarBackground(CDC* pDC, CBCGPBaseControlBar* pBar,
      CRect rectClient, CRect rectClip,
      BOOL bNCArea = FALSE);
  virtual BOOL OnFillParentBarBackground(CWnd* pWnd, CDC* pDC, IRECT* rectClip = NULL);

  virtual void OnDrawBarBorder(CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect);
  virtual void OnDrawMenuBorder(CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect);
  virtual void OnDrawMenuShadow(CDC* pDC, const CRect& rectClient, const CRect& rectExclude,
      int nDepth,  int iMinBrightness,  int iMaxBrightness,
      CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight, BOOL bRTL);
  virtual void OnDrawBarGripper(CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar);
  virtual void OnDrawSeparator(CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bIsHoriz);
  virtual COLORREF OnDrawMenuLabel(CDC* pDC, CRect rect);
  virtual COLORREF OnDrawControlBarCaption(CDC* pDC, CBCGPDockingControlBar* pBar,
      BOOL bActive, CRect rectCaption, CRect rectButtons);
  virtual void OnDrawCaptionButton(CDC* pDC, CBCGPCaptionButton* pButton, BOOL bActive, BOOL bHorz,
      BOOL bMaximized, BOOL bDisabled, int nImageID = -1);
  virtual void OnDrawMenuSystemButton(CDC* pDC, CRect rect, UINT uiSystemCommand,
      UINT nStyle, BOOL bHighlight);
  virtual void OnDrawStatusBarPaneBorder(CDC* pDC, CBCGPStatusBar* pBar,
      CRect rectPane, UINT uiID, UINT nStyle);
  virtual COLORREF GetStatusBarPaneTextColor(CBCGPStatusBar* pStatusBar,
      CBCGStatusBarPaneInfo* pPane);

  virtual void OnDrawStatusBarProgress(CDC* pDC, CBCGPStatusBar* /*pStatusBar*/,
      CRect rectProgress, int nProgressTotal,
      int nProgressCurr,
      COLORREF clrBar,
      COLORREF clrProgressBarDest,
      COLORREF clrProgressText,
      BOOL bProgressText);
  virtual void OnDrawStatusBarSizeBox(CDC* pDC, CBCGPStatusBar* pStatBar,
      CRect rectSizeBox);

  virtual void OnDrawComboDropButton(CDC* pDC, CRect rect,
      BOOL bDisabled,
      BOOL bIsDropped,
      BOOL bIsHighlighted,
      CBCGPToolbarComboBoxButton* pButton);
  virtual void OnDrawComboBorder(CDC* pDC, CRect rect,
      BOOL bDisabled,
      BOOL bIsDropped,
      BOOL bIsHighlighted,
      CBCGPToolbarComboBoxButton* pButton);
  virtual void OnDrawEditBorder(CDC* pDC, CRect rect,
      BOOL bDisabled,
      BOOL bIsHighlighted,
      CBCGPToolbarEditBoxButton* pButton);
  virtual void OnDrawTearOffCaption(CDC* pDC, CRect rect, BOOL bIsActive);

  virtual void OnDrawMenuResizeBar(CDC* pDC, CRect rect, int nResizeFlags);
  virtual void OnDrawMenuScrollButton(CDC* pDC, CRect rect, BOOL bIsScrollDown,
      BOOL bIsHighlited, BOOL bIsPressed, BOOL bIsDisabled);

  virtual void OnFillButtonInterior(CDC* pDC,
      CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state);

  virtual void OnDrawButtonBorder(CDC* pDC,
      CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state);

  virtual void OnDrawButtonSeparator(CDC* pDC,
      CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state,
      BOOL bHorz);

  virtual void OnHighlightMenuItem(CDC* pDC, CBCGPToolbarMenuButton* pButton,
      CRect rect, COLORREF& clrText);
  virtual COLORREF GetHighlightedMenuItemTextColor(CBCGPToolbarMenuButton* pButton);
  virtual BOOL IsHighlightWholeMenuItem() {
    return FALSE;
  }

  virtual COLORREF GetMenuItemTextColor(CBCGPToolbarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled);
  virtual int GetMenuDownArrowState(CBCGPToolbarMenuButton* pButton, BOOL bHightlight, BOOL bPressed, BOOL bDisabled);
  virtual void OnHighlightRarelyUsedMenuItems(CDC* pDC, CRect rectRarelyUsed);

  virtual void OnFillMenuImageRect(CDC* pDC,
      CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state) {
    OnFillButtonInterior(pDC, pButton, rect, state);
  }

  virtual void OnDrawMenuImageRectBorder(CDC* pDC,
      CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state) {
    OnDrawButtonBorder(pDC, pButton, rect, state);
  }

  virtual void OnDrawMenuCheck(CDC* pDC, CBCGPToolbarMenuButton* pButton,
      CRect rect, BOOL bHighlight, BOOL bIsRadio);

  virtual void OnDrawMenuItemButton(CDC* pDC, CBCGPToolbarMenuButton* pButton,
      CRect rectButton, BOOL bHighlight, BOOL bDisabled);

  virtual BOOL IsOwnerDrawMenuCheck() {
    return FALSE;
  }

  virtual COLORREF GetToolbarButtonTextColor(CBCGPToolbarButton* pButton,
      BCGBUTTON_STATE state);

  virtual BOOL IsToolBarButtonDefaultBackground(CBCGPToolbarButton* pButton,
      BCGBUTTON_STATE state);

  virtual COLORREF GetToolbarDisabledColor() const {
    return (COLORREF) - 1;
  }

  virtual BOOL IsToolbarRoundShape(CBCGPToolBar* /*pToolBar*/) {
    return FALSE;
  }

  virtual COLORREF GetToolbarHighlightColor();

  virtual COLORREF GetToolbarDisabledTextColor();

  virtual int GetDockingBarCaptionExtraHeight() const {
    return 0;
  }

  // Caption bar:
  virtual COLORREF GetCaptionBarTextColor(CBCGPCaptionBar* pBar);
  virtual void OnDrawCaptionBarInfoArea(CDC* pDC, CBCGPCaptionBar* pBar, CRect rect);
  virtual void OnDrawCaptionBarBorder(CDC* pDC, CBCGPCaptionBar* pBar, CRect rect, COLORREF clrBarBorder, BOOL bFlatBorder);
  virtual COLORREF OnFillCaptionBarButton(CDC* pDC, CBCGPCaptionBar* pBar,
      CRect rect, BOOL bIsPressed, BOOL bIsHighlighted,
      BOOL bIsDisabled, BOOL bHasDropDownArrow,
      BOOL bIsSysButton);
  virtual void OnDrawCaptionBarButtonBorder(CDC* pDC, CBCGPCaptionBar* pBar,
      CRect rect, BOOL bIsPressed, BOOL bIsHighlighted,
      BOOL bIsDisabled, BOOL bHasDropDownArrow,
      BOOL bIsSysButton);

  // Outlook bar:
  virtual void OnFillOutlookPageButton(CDC* pDC, const CRect& rect,
      BOOL bIsHighlighted, BOOL bIsPressed,
      COLORREF& clrText);
  virtual void OnDrawOutlookPageButtonBorder(CDC* pDC, CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed);
  virtual void OnDrawOutlookBarSplitter(CDC* pDC, CRect rectSplitter);
  virtual void OnFillOutlookBarCaption(CDC* pDC, CRect rectCaption, COLORREF& clrText);

  // Tab overrides:
  virtual void OnEraseTabsArea(CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
  virtual void OnDrawTab(CDC* pDC, CRect rectTab,
      int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
  virtual void OnFillTab(CDC* pDC, CRect rectFill, COLOR pbrFill, int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd, IPOINT* pt, int npt);
  virtual void OnDrawTabContent(CDC* pDC, CRect rectTab,
      int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd,
      COLORREF clrText);
  virtual void OnDrawTabCloseButton(CDC* pDC, CRect rect,
      const CBCGPBaseTabWnd* pTabWnd,
      BOOL bIsHighlighted,
      BOOL bIsPressed,
      BOOL bIsDisabled);
  virtual void OnEraseTabsButton(CDC* pDC, CRect rect, CBCGPButtonBase* pButton,
      CBCGPBaseTabWnd* pWndTab);
  virtual void OnDrawTabsButtonBorder(CDC* pDC, CRect& rect,
      CBCGPButtonBase* pButton, UINT uiState, CBCGPBaseTabWnd* pWndTab);
  virtual void OnDrawTabResizeBar(CDC* pDC, CBCGPBaseTabWnd* pWndTab,
      BOOL bIsVert, CRect rect,
      COLOR pbrFace, COLOR pPen);
  virtual void GetTabFrameColors(const CBCGPBaseTabWnd* pTabWnd,
      COLORREF& clrDark,
      COLORREF& clrBlack,
      COLORREF& clrHighlight,
      COLORREF& clrFace,
      COLORREF& clrDarkShadow,
      COLORREF& clrLight,
      COLOR& pbrFace,
      COLOR& pbrBlack);
  virtual BOOL OnEraseTabsFrame(CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
  virtual BOOL IsHighlightOneNoteTabs() const {
    return FALSE;
  }
  virtual BOOL AlwaysHighlight3DTabs() const    {
    return FALSE;
  }
  virtual COLORREF GetTabTextColor(const CBCGPBaseTabWnd* /*pTabWnd*/, int /*iTab*/, BOOL /*bIsActive*/) {
    return (COLORREF) - 1;
  }
  virtual int GetTabHorzMargin(const CBCGPBaseTabWnd* /*pTabWnd*/) {
    return 0;
  }

  virtual int GetMDITabsBordersSize()   {
    return -1;  /* Default */
  }
  virtual BOOL IsMDITabsTopEdge()     {
    return TRUE;
  }
  virtual int GetDockingTabsBordersSize() {
    return -1;  /* Default */
  }
  virtual BOOL IsDockingTabHasBorder()    {
    return TRUE;
  }

  virtual int GetTabButtonState(CBCGPTabWnd* /*pTab*/, CBCGTabButton* /*pButton*/) {
    return -1;
  }

  // Customization dialog:
  virtual COLORREF OnFillCommandsListBackground(CDC* pDC, CRect rect, BOOL bIsSelected = FALSE);
  virtual void OnDrawMenuArrowOnCustomizeList(CDC* pDC, CRect rectCommand, BOOL bSelected);

  virtual CSize GetButtonExtraBorder() const {
    return CSize(0, 0);
  }

  virtual CSize GetCaptionButtonExtraBorder() const {
    return CSize(0, 0);
  }

  virtual COLORREF OnFillMiniFrameCaption(CDC* pDC, CRect rectCaption,
      CBCGPMiniFrameWnd* pFrameWnd,
      BOOL bActive);
  virtual void OnDrawMiniFrameBorder(CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
      CRect rectBorder, CRect rectBorderSize);
  virtual void OnDrawFloatingToolbarBorder(CDC* pDC, CBCGPBaseToolBar* pToolBar,
      CRect rectBorder, CRect rectBorderSize);
  // Header control:
  virtual void OnFillHeaderCtrlBackground(CBCGPHeaderCtrl* pCtrl, CDC* pDC, CRect rect);
  virtual void OnDrawHeaderCtrlBorder(CBCGPHeaderCtrl* pCtrl, CDC* pDC,
      CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted);
  virtual void OnDrawHeaderCtrlSortArrow(CBCGPHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsUp);
  void DoDrawHeaderSortArrow(CDC* pDC, CRect rect, BOOL bIsUp, BOOL bDlgCtrl);

  virtual COLORREF OnFillListBoxItem(CDC* pDC, CBCGPListBox* pListBox, int nItem, CRect rect, BOOL bIsHighlihted, BOOL bIsSelected);

  // Obsolete
  virtual COLORREF OnDrawPropSheetListItem(CDC*, CBCGPPropertySheet*, CRect, BOOL, BOOL) {
    return (COLORREF)0;
  }

  // Tasks pane:
#ifndef BCGP_EXCLUDE_TASK_PANE
  virtual void OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea);

  virtual void OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, BOOL bCanCollapse = FALSE);
  virtual void OnDrawTasksGroupIcon(CDC* pDC, CBCGPTasksGroup* pGroup, int nIconHOffset = 5,
      BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, BOOL bCanCollapse = FALSE);

  virtual void OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL bSpecial = FALSE);
  virtual void OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial = FALSE, BOOL bNoTitle = FALSE);
  virtual void OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE);

  virtual void OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
      int iImage, BOOL bHilited);
  virtual void OnDrawToolBoxFrame(CDC* pDC, const CRect& rect);
#endif

  // Slider
  virtual void OnDrawSlider(CDC* pDC, CBCGPSlider* pSlider, CRect rect, BOOL bAutoHideMode);

  // Property list:
  virtual void OnDrawExpandingBox(CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox);

#ifndef BCGP_EXCLUDE_PROP_LIST
  virtual COLORREF GetPropListGroupColor(CBCGPPropList* pPropList);
  virtual COLORREF GetPropListGroupTextColor(CBCGPPropList* pPropList);
  virtual COLORREF GetPropListDesciptionTextColor(CBCGPPropList* /*pPropList*/)  {
    return (COLORREF) - 1;
  }
  virtual COLORREF GetPropListCommandTextColor(CBCGPPropList* pPropList);
#endif

  // Splitter:
  virtual void OnDrawSplitterBorder(CDC* pDC, CBCGPSplitterWnd* pSplitterWnd, CRect rect);
  virtual void OnDrawSplitterBox(CDC* pDC, CBCGPSplitterWnd* pSplitterWnd, CRect& rect);
  virtual void OnFillSplitterBackground(CDC* pDC, CBCGPSplitterWnd* pSplitterWnd, CRect rect);

  // Date/time controls:
  virtual COLORREF OnFillCalendarBarNavigationRow(CDC* pDC, CRect rect);

  virtual void GetCalendarColors(const CBCGPCalendar* pCalendar,
      CBCGPCalendarColors& colors);

  virtual void OnDrawCheckBox(CDC* pDC, CRect rect,
      BOOL bHighlighted,
      BOOL bChecked,
      BOOL bEnabled);

  virtual void OnDrawCheckBoxEx(CDC* pDC, CRect rect,
      int nState,
      BOOL bHighlighted,
      BOOL bPressed,
      BOOL bEnabled);

  virtual void OnDrawRadioButton(CDC* pDC, CRect rect,
      BOOL bOn,
      BOOL bHighlighted,
      BOOL bPressed,
      BOOL bEnabled);

  virtual void OnDrawControlBorder(CDC* pDC, CRect rect, CBCGPBaseControl* pWndCtrl, BOOL bDrawOnGlass);

  // Auto-hide buttons:
  virtual BOOL HasOverlappedAutoHideButtons() const {
    return FALSE;
  }

  virtual void OnFillAutoHideButtonBackground(CDC* pDC, CRect rect, CBCGPAutoHideButton* pButton);
  virtual void OnDrawAutoHideButtonBorder(CDC* pDC, CRect rectBounds, CRect rectBorderSize, CBCGPAutoHideButton* pButton);
  virtual COLORREF GetAutoHideButtonTextColor(CBCGPAutoHideButton* pButton);

  // Calculator:
  virtual BOOL OnDrawCalculatorButton(CDC* pDC, CRect rect, CBCGPToolbarButton* pButton, BCGBUTTON_STATE state, int cmd /* CBCGPCalculator::CalculatorCommands */, CBCGPCalculator* pCalculator);
  virtual BOOL OnDrawCalculatorDisplay(CDC* pDC, CRect rect, const CString& strText, BOOL bMem, CBCGPCalculator* pCalculator);

  // Edit box:
  virtual BOOL OnDrawBrowseButton(CDC* pDC, CRect rect, CBCGPEdit* pEdit, BCGBUTTON_STATE state, COLORREF& clrText);

  // Spin control:
  virtual void OnDrawSpinButtons(CDC* pDC, CRect rectSpin, int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* pSpinCtrl);

  // Appbar window:
  virtual void OnDrawAppBarBorder(CDC* pDC, CBCGPAppBarWnd* pAppBarWnd,
      CRect rectBorder, CRect rectBorderSize);

  virtual void OnDrawAppBarCaption(CDC* pDC, CBCGPAppBarWnd* pAppBarWnd,
      CRect rectCaption, CString strCaption);

  // Windows XP drawing methods:
  virtual BOOL DrawComboDropButtonWinXP(CDC* /*pDC*/, CRect /*rect*/,
      BOOL /*bDisabled*/,
      BOOL /*bIsDropped*/,
      BOOL /*bIsHighlighted*/)  {
    return FALSE;
  }
  virtual BOOL DrawComboBorderWinXP(CDC* /*pDC*/, CRect /*rect*/,
      BOOL /*bDisabled*/,
      BOOL /*bIsDropped*/,
      BOOL /*bIsHighlighted*/)  {
    return FALSE;
  }

  // Toolbox:
#ifndef BCGP_EXCLUDE_TOOLBOX
  virtual BOOL OnEraseToolBoxButton(CDC* pDC, CRect rect, CBCGPToolBoxButton* pButton);
  virtual BOOL OnDrawToolBoxButtonBorder(CDC* pDC, CRect& rect, CBCGPToolBoxButton* pButton, UINT uiState);
  virtual COLORREF GetToolBoxButtonTextColor(CBCGPToolBoxButton* pButton);
#endif

  // Smart docking marker colors:
  virtual void GetSmartDockingBaseMarkerColors(
      COLORREF& clrBaseGroupBackground,
      COLORREF& clrBaseGroupBorder);

  virtual COLORREF GetSmartDockingMarkerToneColor();

  virtual BCGP_SMARTDOCK_THEME GetSmartDockingTheme() {
    return BCGP_SDT_VS2005;
  }

  // Popup window:
#ifndef BCGP_EXCLUDE_POPUP_WINDOW
  virtual void OnFillPopupWindowBackground(CDC* pDC, CRect rect);
  virtual void OnDrawPopupWindowBorder(CDC* pDC, CRect rect);
  virtual COLORREF OnDrawPopupWindowCaption(CDC* pDC, CRect rectCaption, CBCGPPopupWindow* pPopupWnd);
  virtual void OnErasePopupWindowButton(CDC* pDC, CRect rectClient, CBCGPPopupWndButton* pButton);
  virtual void OnDrawPopupWindowButtonBorder(CDC* pDC, CRect rectClient, CBCGPPopupWndButton* pButton);
  virtual BOOL IsDefaultWinXPPopupButton(CBCGPPopupWndButton* /*pButton*/) const {
    return FALSE;
  }
#endif

  // Day planner:
#ifndef BCGP_EXCLUDE_PLANNER
  virtual COLORREF OnFillPlannerCaption(CDC* pDC, CBCGPPlannerView* pView,
      CRect rect, BOOL bIsToday, BOOL bIsSelected, BOOL bMonth, BOOL bNoBorder = FALSE, BOOL bHorz = TRUE);

  virtual void OnDrawPlannerCaptionText(CDC* pDC,
      CBCGPPlannerView* pView, CRect rect, const CString& strText,
      COLORREF clrText, int nAlign, BOOL bHighlight);

  virtual void OnFillPlanner(CDC* pDC, CBCGPPlannerView* pView,
      CRect rect, BOOL bWorkingArea);

  virtual void GetPlannerAppointmentColors(CBCGPPlannerView* pView,
      BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags,
      COLORREF& clrBack1, COLORREF& clrBack2,
      COLORREF& clrFrame1, COLORREF& clrFrame2, COLORREF& clrText);

  virtual COLORREF GetPlannerAppointmentTimeColor(CBCGPPlannerView* pView,
      BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags);

  virtual COLORREF GetPlannerHourLineColor(CBCGPPlannerView* pView,
      BOOL bWorkingHours, BOOL bHour);

  virtual COLORREF OnFillPlannerTimeBar(CDC* pDC,
      CBCGPPlannerView* pView, CRect rect, COLORREF& clrLine);

  virtual void OnDrawPlannerTimeLine(CDC* pDC, CBCGPPlannerView* pView, CRect rect);

  virtual void OnFillPlannerWeekBar(CDC* pDC,
      CBCGPPlannerView* pView, CRect rect);

  virtual DWORD GetPlannerDrawFlags() const {
    return 0;
  }

  virtual int GetPlannerRowExtraHeight() const {
    return 0;
  }

  virtual COLORREF GetPlannerWorkColor() const {
    return m_clrPlannerWork;
  }

  virtual void OnDrawPlannerHeader(CDC* pDC,
      CBCGPPlannerView* pView, CRect rect);

  virtual void OnDrawPlannerHeaderPane(CDC* pDC,
      CBCGPPlannerView* pView, CRect rect);

  virtual void OnFillPlannerHeaderAllDay(CDC* pDC,
      CBCGPPlannerView* pView, CRect rect);
  virtual void OnDrawPlannerHeaderAllDayItem(CDC* pDC,
      CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected);

  virtual COLORREF GetPlannerSelectionColor(CBCGPPlannerView* pView);
  virtual COLORREF GetPlannerSeparatorColor(CBCGPPlannerView* pView);

  virtual void PreparePlannerBackItem(BOOL bIsToday, BOOL bIsSelected);
  virtual void PreparePlannerCaptionBackItem(BOOL bIsHeader);

#endif // BCGP_EXCLUDE_PLANNER

  // Grid control:
#ifndef BCGP_EXCLUDE_GRID_CTRL
  virtual void OnFillGridBackground(CDC* pDC, CRect rect);
  virtual void OnDrawGridExpandingBox(CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox);
  virtual void OnFillGridHeaderBackground(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);
  virtual BOOL OnDrawGridHeaderItemBorder(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
  virtual void OnFillGridRowHeaderBackground(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);
  virtual BOOL OnDrawGridRowHeaderItemBorder(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
  virtual void OnFillGridSelectAllAreaBackground(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
  virtual BOOL OnDrawGridSelectAllAreaBorder(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
  virtual void OnDrawGridSortArrow(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectArrow, BOOL bIsUp);
  virtual COLORREF OnFillGridGroupByBoxBackground(CDC* pDC, CRect rect);
  virtual COLORREF OnFillGridGroupByBoxTitleBackground(CDC* pDC, CRect rect);
  virtual COLORREF GetGridGroupByBoxLineColor() const;
  virtual void OnDrawGridGroupByBoxItemBorder(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);
  virtual COLORREF GetGridLeftOffsetColor(CBCGPGridCtrl* pCtrl);
  virtual COLORREF GetGridItemSortedColor(CBCGPGridCtrl* pCtrl);
  virtual void OnFillGridGroupBackground(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill);
  virtual void OnDrawGridGroupUnderline(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill);
  virtual COLORREF OnFillGridRowBackground(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill, BOOL bSelected);
  virtual COLORREF OnFillGridItem(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill,
      BOOL bSelected, BOOL bActiveItem, BOOL bSortedColumn);
  virtual void OnDrawGridHeaderMenuButton(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect,
      BOOL bHighlighted, BOOL bPressed, BOOL bDisabled);
  virtual void OnDrawGridSelectionBorder(CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);

  virtual BOOL OnSetGridColorTheme(CBCGPGridCtrl* pCtrl, BCGP_GRID_COLOR_DATA& theme);

  // Report control:
  virtual COLORREF GetReportCtrlGroupBackgoundColor();
  virtual COLORREF OnFillReportCtrlRowBackground(CBCGPGridCtrl* pCtrl, CDC* pDC,
      CRect rectFill, BOOL bSelected, BOOL bGroup);
#endif // BCGP_EXCLUDE_GRID_CTRL

  // Gantt control:
#if !defined (BCGP_EXCLUDE_GRID_CTRL) && !defined (BCGP_EXCLUDE_GANTT)
  virtual void GetGanttColors(const CBCGPGanttChart* pChart, BCGP_GANTT_CHART_COLORS& colors, COLORREF clrBack = CLR_DEFAULT) const;
  virtual void DrawGanttChartBackground(const CBCGPGanttChart* pChart, CDC& dc, const CRect& rectChart, COLORREF clrFill);
  virtual void DrawGanttItemBackgroundCell(const CBCGPGanttChart* pChart, CDC& dc, const CRect& rectItem, const CRect& rectClip, const BCGP_GANTT_CHART_COLORS& colors, BOOL bDayOff);
  virtual void DrawGanttHeaderCell(const CBCGPGanttChart* pChart, CDC& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL bHilite);
  virtual void DrawGanttHeaderText(const CBCGPGanttChart* pChart, CDC& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, const CString& sCellText, BOOL bHilite) const;
  virtual COLORREF GetGanttHeaderTextColor(BOOL bHilite) const;

  virtual void FillGanttBar(const CBCGPGanttItem* pItem, CDC& dc, const CRect& rectFill, COLORREF color, double dGlowLine);

#endif // !defined (BCGP_EXCLUDE_GRID_CTRL) && !defined (BCGP_EXCLUDE_GANTT)

  // Window apperance:
  virtual BOOL IsOwnerDrawCaption() {
    return FALSE;
  }
  virtual BOOL OnSetWindowRegion(CWnd* pWnd, CSize sizeWindow);
  virtual void OnActivateApp(CWnd* pWnd, BOOL bActive);
  virtual BOOL OnNcPaint(CWnd* pWnd, const CObList& lstSysButtons, CRect rectRedraw);
  virtual BOOL OnNcActivate(CWnd* pWnd, BOOL bActive);
  virtual CSize GetNcBtnSize(BOOL bSmall) const;
  virtual BOOL OnUpdateNcButtons(CWnd* pWnd, const CObList& lstSysButtons, CRect rectCaption);

  // Dialog:
  virtual BOOL OnFillDialog(CDC* pDC, CRect rect);
  virtual COLOR GetDlgBackBrush();

#ifndef BCGP_EXCLUDE_RIBBON
  // Ribbon control:
  virtual COLORREF OnDrawRibbonTabsFrame(
      CDC* pDC,
      CBCGPRibbonBar* pWndRibbonBar,
      CRect rectTab);

  virtual void OnDrawRibbonMainButton(
      CDC* pDC,
      CBCGPRibbonButton* pButton);

  virtual void OnDrawRibbonCategory(
      CDC* pDC,
      CBCGPRibbonCategory* pCategory,
      CRect rectCategory);

  virtual void OnDrawRibbonCategoryScroll(
      CDC* pDC,
      CBCGPRibbonCategoryScroll* pScroll);

  virtual COLORREF OnDrawRibbonCategoryTab(
      CDC* pDC,
      CBCGPRibbonTab* pTab,
      BOOL bIsActive);

  virtual COLORREF OnDrawRibbonPanel(
      CDC* pDC,
      CBCGPRibbonPanel* pPanel,
      CRect rectPanel,
      CRect rectCaption);

  virtual void OnDrawRibbonPanelCaption(
      CDC* pDC,
      CBCGPRibbonPanel* pPanel,
      CRect rectCaption);

  virtual COLORREF OnFillRibbonPanelCaption(
      CDC* pDC,
      CBCGPRibbonPanel* pPanel,
      CRect rectCaption);

  virtual void OnDrawRibbonLaunchButton(
      CDC* pDC,
      CBCGPRibbonLaunchButton* pButton,
      CBCGPRibbonPanel* pPanel);

  virtual void OnDrawRibbonDefaultPaneButton(
      CDC* pDC,
      CBCGPRibbonButton* pButton);

  virtual void OnDrawRibbonDefaultPaneButtonContext(
      CDC* pDC,
      CBCGPRibbonButton* pButton);

  virtual void OnDrawRibbonDefaultPaneButtonIndicator(
      CDC* pDC,
      CBCGPRibbonButton* pButton,
      CRect rect,
      BOOL bIsSelected,
      BOOL bHighlighted);

  virtual COLORREF OnFillRibbonButton(
      CDC* pDC,
      CBCGPRibbonButton* pButton);

  virtual void OnFillRibbonEdit(
      CDC* pDC,
      CBCGPRibbonEditCtrl* pEdit,
      CRect rect,
      BOOL bIsHighlighted,
      BOOL bIsPaneHighlighted,
      BOOL bIsDisabled,
      COLORREF& clrText, COLORREF& clrSelBackground, COLORREF& clrSelText);

  virtual COLORREF GetRibbonEditBackgroundColor(
      CBCGPRibbonEditCtrl* pEdit,
      BOOL bIsHighlighted,
      BOOL bIsPaneHighlighted,
      BOOL bIsDisabled);

  virtual void OnDrawRibbonButtonBorder(
      CDC* pDC,
      CBCGPRibbonButton* pButton);

  virtual void OnDrawRibbonMenuCheckFrame(
      CDC* pDC,
      CBCGPRibbonButton* pButton,
      CRect rect);

  virtual COLORREF OnFillRibbonMainPanelButton(
      CDC* pDC,
      CBCGPRibbonButton* pButton);

  virtual void OnDrawRibbonMainPanelButtonBorder(
      CDC* pDC,
      CBCGPRibbonButton* pButton);

  virtual void OnDrawRibbonCaption(
      CDC* pDC, CBCGPRibbonBar* pBar, CRect rect,
      CRect rectText);

  virtual int GetRibbonQATChevronOffset() {
    return 0;
  }

  virtual int GetRibbonQATRightMargin()   {
    return 0;
  }

  virtual COLORREF GetRibbonQATTextColor(BOOL bDisabled = FALSE) {
    return bDisabled ? GetToolbarDisabledTextColor() : (COLORREF) - 1;
  }

  virtual void OnDrawRibbonCaptionButton(
      CDC* pDC, CBCGPRibbonCaptionButton* pButton);

  virtual COLORREF OnDrawRibbonButtonsGroup(
      CDC* pDC,
      CBCGPRibbonButtonsGroup* pGroup,
      CRect rectGroup);

  virtual void OnDrawDefaultRibbonImage(
      CDC* pDC,
      CRect rectImage,
      BOOL bIsDisabled = FALSE,
      BOOL bIsPressed = FALSE,
      BOOL bIsHighlighted = FALSE);

  virtual void OnDrawRibbonMainPanelFrame(
      CDC* pDC,
      CBCGPRibbonMainPanel* pPanel,
      CRect rect);

  virtual void OnFillRibbonMenuFrame(
      CDC* pDC,
      CBCGPRibbonMainPanel* pPanel,
      CRect rect);

  virtual void OnDrawRibbonRecentFilesFrame(
      CDC* pDC,
      CBCGPRibbonMainPanel* pPanel,
      CRect rect);

  virtual void OnDrawRibbonLabel(
      CDC* pDC,
      CBCGPRibbonLabel* pLabel,
      CRect rect);

  virtual void OnDrawRibbonPaletteButton(
      CDC* pDC,
      CBCGPRibbonPaletteIcon* pButton);

  virtual void OnDrawRibbonPaletteBorder(
      CDC* pDC,
      CBCGPRibbonPaletteButton* pButton,
      CRect rectBorder);

  virtual COLORREF RibbonCategoryColorToRGB(BCGPRibbonCategoryColor color);

  virtual COLORREF OnDrawRibbonCategoryCaption(
      CDC* pDC,
      CBCGPRibbonContextCaption* pContextCaption);

  virtual COLORREF OnDrawRibbonStatusBarPane(CDC* pDC, CBCGPRibbonStatusBar* pBar,
      CBCGPRibbonStatusBarPane* pPane);

  virtual void GetRibbonSliderColors(CBCGPRibbonSlider* pSlider,
      BOOL bIsHighlighted,
      BOOL bIsPressed,
      BOOL bIsDisabled,
      COLORREF& clrLine,
      COLORREF& clrFill);

  virtual void OnDrawRibbonSliderZoomButton(
      CDC* pDC, CBCGPRibbonSlider* pSlider,
      CRect rect, BOOL bIsZoomOut,
      BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);

  virtual void OnDrawRibbonSliderChannel(
      CDC* pDC, CBCGPRibbonSlider* pSlider,
      CRect rect);

  virtual void OnDrawRibbonSliderThumb(
      CDC* pDC, CBCGPRibbonSlider* pSlider,
      CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);

  virtual void OnDrawRibbonProgressBar(
      CDC* pDC, CBCGPRibbonProgressBar* pProgress,
      CRect rectProgress, CRect rectChunk, BOOL bInfiniteMode);

  virtual void OnFillRibbonQATPopup(
      CDC* pDC, CBCGPRibbonPanelMenuBar* pMenuBar, CRect rect);

  virtual int GetRibbonPopupBorderSize(const CBCGPRibbonPanelMenu* /*pPopup*/) const {
    return m_nMenuBorderSize;
  }

  virtual void OnDrawRibbonQATSeparator(CDC* pDC, CBCGPRibbonSeparator* pSeparator, CRect rect);

  virtual BOOL IsLayeredRibbonKeyTip() const  {
    return FALSE;
  }
  virtual void OnDrawRibbonKeyTip(CDC* pDC, CBCGPBaseRibbonElement* pElement, CRect rect, CString str);

  virtual void OnDrawRibbonCheckBoxOnList(CDC* pDC, CBCGPRibbonCheckBox* pCheckBox, CRect rect, BOOL bIsSelected, BOOL bHighlighted);
  virtual void OnDrawRibbonRadioButtonOnList(CDC* pDC, CBCGPRibbonRadioButton* pRadioButton, CRect rect, BOOL bIsSelected, BOOL bHighlighted);

  virtual COLORREF GetRibbonHyperlinkTextColor(CBCGPRibbonHyperlink* pHyperLink);
  virtual COLORREF GetRibbonStatusBarTextColor(CBCGPRibbonStatusBar* pStatusBar);

  virtual void OnDrawRibbonColorPaletteBox(CDC* pDC, CBCGPRibbonColorButton* pColorButton,
      CBCGPRibbonPaletteIcon* pIcon,
      COLORREF color, CRect rect, BOOL bDrawTopEdge, BOOL bDrawBottomEdge,
      BOOL bIsHighlighted, BOOL bIsChecked, BOOL bIsDisabled);

  const CPoint& GetRibbonMainImageOffset() const  {
    return m_ptRibbonMainImageOffset;
  }

#endif

  // Vista support:
  virtual BOOL DrawTextOnGlass(CDC* pDC, CString strText, CRect rect, DWORD dwFlags, int nGlowSize = 0,
      COLORREF clrText = (COLORREF) - 1);

  // MDI Client area
  virtual BOOL OnEraseMDIClientArea(CDC* pDC, CRect rectClient);

  // ToolTip
  virtual BOOL GetToolTipParams(CBCGPToolTipParams& params, UINT nType = (UINT)(-1));
  virtual void OnFillToolTip(CDC* pDC, CBCGPToolTipCtrl* pToolTip, CRect rect, COLORREF& clrText, COLORREF& clrLine);

  // Scrollbar
  virtual BOOL IsOwnerDrawScrollBar() const {
    return FALSE;
  }

  virtual void OnScrollBarDrawThumb(CDC* /*pDC*/, CBCGPScrollBar* /*pScrollBar*/, CRect /*rect*/,
      BOOL /*bHorz*/, BOOL /*bHighlighted*/, BOOL /*bPressed*/, BOOL /*bDisabled*/) {
    ASSERT(FALSE);
  }

  virtual void OnScrollBarDrawButton(CDC* /*pDC*/, CBCGPScrollBar* /*pScrollBar*/, CRect /*rect*/,
      BOOL /*bHorz*/, BOOL /*bHighlighted*/, BOOL /*bPressed*/, BOOL /*bFirst*/, BOOL /*bDisabled*/) {
    ASSERT(FALSE);
  }

  virtual void OnScrollBarFillBackground(CDC* /*pDC*/, CBCGPScrollBar* /*pScrollBar*/, CRect /*rect*/,
      BOOL /*bHorz*/, BOOL /*bHighlighted*/, BOOL /*bPressed*/, BOOL /*bFirst*/, BOOL /*bDisabled*/) {
    ASSERT(FALSE);
  }

  // Push button:
  virtual BOOL OnDrawPushButton(CDC* /*pDC*/, CRect /*rect*/, CBCGPButtonBase* /*pButton*/, COLORREF& /*clrText*/) {
    return FALSE;
  }
  virtual COLORREF GetURLLinkColor(CBCGPURLLinkButton* pButton, BOOL bHover);

  // Group:
  virtual void OnDrawGroup(CDC* pDC, CBCGPGroup* pGroup, CRect rect, const CString& strName);

  // Slider:
  virtual void OnDrawSliderChannel(CDC* pDC, CBCGPSliderCtrl* pSlider, BOOL bVert, CRect rect, BOOL bDrawOnGlass);

  virtual void OnDrawSliderThumb(CDC* pDC, CBCGPSliderCtrl* pSlider,
      CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled,
      BOOL bVert, BOOL bLeftTop, BOOL bRightBottom,
      BOOL bDrawOnGlass);

  // Attributes:
public:

  void SetMenuShadowDepth(int nDepth) { // Pixels
    m_nMenuShadowDepth = nDepth;
  }

  int GetMenuShadowDepth() const {
    return m_nMenuShadowDepth;
  }

  BOOL IsLook2000() const     {
    return m_bLook2000;
  }
  void SetLook2000(BOOL bLook2000 = TRUE) {
    m_bLook2000 = bLook2000;
  }

  BOOL IsMenuFlatLook() const     {
    return m_bMenuFlatLook;
  }
  void SetMenuFlatLook(BOOL bMenuFlatLook = TRUE) {
    m_bMenuFlatLook = bMenuFlatLook;
  }

  BOOL IsAutoDestroy() const {
    return m_bAutoDestroy;
  }

  void SetShadowHighlightedImage(BOOL bShadow = TRUE) {
    m_bShadowHighlightedImage = bShadow;
  }

  BOOL IsShadowHighlightedImage() const {
    return m_bShadowHighlightedImage;
  }

  void EnableToolbarButtonFill(BOOL bEnable = TRUE) {
    m_bEnableToolbarButtonFill = bEnable;
  }

  BOOL IsToolbarButtonFillEnabled() const {
    return m_bEnableToolbarButtonFill;
  }

  BOOL IsEmbossDisabledImage() const {
    return m_bEmbossDisabledImage;
  }

  void SetEmbossDisabledImage(BOOL bEmboss = TRUE) {
    m_bEmbossDisabledImage = bEmboss;
  }

  BOOL IsFadeInactiveImage() const {
    return m_bFadeInactiveImage;
  }

  void SetFadeInactiveImage(BOOL bFade = TRUE) {
    m_bFadeInactiveImage = bFade;
  }

  virtual int GetMenuImageMargin() const {
    return 2;
  }

  virtual COLORREF GetMenuShadowBaseColor() {
    return m_clrMenuShadowBase;
  }

  virtual CRect GetMenuImageFrameOffset() const {
    return CRect(2, 1, 0, 2);
  }

  virtual int GetPopupMenuGap() const {
    return 1;
  }

  virtual BOOL IsLook2000Allowed() const
  // Allows choose "Look 2000" in the customization dialog
  {
    return TRUE;
  }

  // TasksPane:
  int GetTasksPaneVertMargin() const {
    return m_nVertMargin;
  }

  int GetTasksPaneHorzMargin() const {
    return m_nHorzMargin;
  }

  int GetTasksPaneGroupVertOffset() const {
    return m_nGroupVertOffset;
  }

  int GetTasksPaneGroupCaptionHeight() const {
    return m_nGroupCaptionHeight;
  }

  int GetTasksPaneGroupCaptionHorzOffset() const {
    return m_nGroupCaptionHorzOffset;
  }

  int GetTasksPaneGroupCaptionVertOffset() const {
    return m_nGroupCaptionVertOffset;
  }

  int GetTasksPaneTaskHorzOffset() const {
    return m_nTasksHorzOffset;
  }

  int GetTasksPaneIconHorzOffset() const {
    return m_nTasksIconHorzOffset;
  }

  int GetTasksPaneIconVertOffset() const {
    return m_nTasksIconVertOffset;
  }

  virtual int GetToolBarCustomizeButtonMargin() const {
    return 2;
  }

  virtual BOOL IsOffsetPressedButton() const {
    return TRUE;
  }

  virtual BOOL IsOfficeXPStyleMenus() const {
    return m_bOfficeXPStyleMenus;
  }

  virtual BOOL GetPopupMenuBorderSize() const {
    return m_nMenuBorderSize;
  }

  BOOL IsFrameMenuCheckedItems() const {
    return m_bFrameMenuCheckedItems;
  }

  // "Show all menu items" methods:
  virtual int GetShowAllMenuItemsHeight(CDC* pDC, const CSize& sizeDefault);
  virtual void OnDrawShowAllMenuItems(CDC* pDC, CRect rect, BCGBUTTON_STATE state);

  virtual BOOL IsEmbossDisabledBtnText() {
    return TRUE;
  }

  // Attributes:
protected:

  BOOL  m_bLook2000;        // Single grippers
  int   m_nMenuShadowDepth;
  BOOL  m_bMenuFlatLook;      // Menu item is always stil unpressed
  BOOL  m_bShadowHighlightedImage;
  BOOL  m_bEmbossDisabledImage;
  BOOL  m_bFadeInactiveImage;
  BOOL  m_bEnableToolbarButtonFill;
  BOOL  m_bFrameMenuCheckedItems;

  int   m_nVertMargin;
  int   m_nHorzMargin;
  int   m_nGroupVertOffset;
  int   m_nGroupCaptionHeight;
  int   m_nGroupCaptionHorzOffset;
  int   m_nGroupCaptionVertOffset;
  int   m_nTasksHorzOffset;
  int   m_nTasksIconHorzOffset;
  int   m_nTasksIconVertOffset;
  BOOL  m_bActiveCaptions;

  COLORREF  m_clrReportGroupText;

  BOOL  m_bOfficeXPStyleMenus;
  int   m_nMenuBorderSize;

  BOOL  m_bAlwaysFillTab;
  BOOL  m_b3DTabWideBorder;

  COLORREF  m_clrMenuShadowBase;
  COLORREF  m_clrPlannerWork;
  COLORREF  m_clrPalennerLine;

  CBrush    m_brPlanner;

  BOOL    m_bPlannerBackItemToday;
  BOOL    m_bPlannerBackItemSelected;
  BOOL    m_bPlannerCaptionBackItemHeader;

  CPoint    m_ptRibbonMainImageOffset;

private:
  BOOL  m_bAutoDestroy;
};

static CBCGPVisualManager globalData;

CBCGPVisualManager* CBCGPVisualManager::GetInstance() {
  return &globalData;
}
//****
BOOL HighlightRect(img_t* im, const IRECT* pclip, CRect rect, int nPercentage = -1,     // < 100 - darker, > 100 - lighter, -1 - default light
    COLORREF clrTransparent =  0, // Don't change this color
    int nTolerance = 0, COLORREF clrBlend = 0)
{
  rect = iRectInter(rect, iRectClip(pclip, 0, 0, im->w, im->h));

  if (nPercentage == 100) {
    // Nothing to do
    return TRUE;
  }

  if (rect.Height() <= 0 || rect.Width() <= 0) {
    return TRUE;
  }

  if (clrBlend != 0 && nPercentage > 100) {
    return FALSE;
  }

  if (clrTransparent != -1) {
    clrTransparent = _RGB(GetBV(clrTransparent), GetGV(clrTransparent), GetRV(clrTransparent));
  }

  int cx = rect.Width();
  int cy = rect.Height();

  for (int y = 0; y < cy; y++) {
    COLORREF* pBits = (COLORREF*)(im->tt.data + (rect.t + y) * im->s + rect.l * im->c);

    for (int x = 0; x < cx; x++, pBits++) {
      COLORREF color = (COLORREF) * pBits;

      BOOL bIgnore = FALSE;

      if (nTolerance > 0) {
        bIgnore = (abs(GetRV(color) - GetRV(clrTransparent)) < nTolerance &&
            abs(GetGV(color) - GetGV(clrTransparent)) < nTolerance &&
            abs(GetBV(color) - GetBV(clrTransparent)) < nTolerance);
      }
      else {
        bIgnore = color == clrTransparent;
      }

      if (!bIgnore) {
        if (nPercentage == -1) {
          *pBits = RGB_TO_RGBA(
              min(255, (2 * GetRV(color) + GetBV(globalData.clrBtnHilite)) / 3),
              min(255, (2 * GetGV(color) + GetGV(globalData.clrBtnHilite)) / 3),
              min(255, (2 * GetBV(color) + GetRV(globalData.clrBtnHilite)) / 3));
        }
        else {
          if (clrBlend == 0) {
            *pBits = CLR_TO_RGBA(PixelAlpha(color,
                .01 * nPercentage, .01 * nPercentage, .01 * nPercentage));
          }
          else {
            long R = GetRV(color);
            long G = GetGV(color);
            long B = GetBV(color);

            *pBits = RGB_TO_RGBA(
                min(255, R + ::MulDiv(GetBV(clrBlend) - R, nPercentage, 100)),
                min(255, G + ::MulDiv(GetGV(clrBlend) - G, nPercentage, 100)),
                min(255, B + ::MulDiv(GetRV(clrBlend) - B, nPercentage, 100))
                );
          }
        }
      }
    }
  }

  //-------------------------------------------
  // Copy highligted bitmap back to the screen:
  //-------------------------------------------
  return TRUE;
}

void FillDitheredRect(img_t* im, const CRect& rect)
{
#if 0
  ASSERT_VALID(pDC);

  if (m_bIsDrawOnGlass) {
    globalData.brLight;


    dm.DrawRect(rect, br.lbColor, 0);
  }
  else {
    pDC->FillRect(&rect, &globalData.brLight);
  }

#endif
}

//****
BOOL GrayRect(img_t* im, const IRECT* pclip, CRect rect, int nPercentage = -1,    // < 100 - darker, > 100 - lighter, -1 - default light
    COLORREF clrTransparent =  0,// Don't change this color
    COLORREF clrDisabled = 0 // Use this color for desaturation
             )
{
  rect = iRectInter(rect, iRectClip(pclip, 0, 0, im->w, im->h));

  if (rect.Height() <= 0 || rect.Width() <= 0) {
    return TRUE;
  }

  int cx = rect.Width();
  int cy = rect.Height();

  if (im->tt.data == NULL) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (clrTransparent != 0) {
    clrTransparent = _RGB(GetBV(clrTransparent), GetGV(clrTransparent), GetRV(clrTransparent));
  }

  if (clrDisabled == 0) {
    clrDisabled = globalData.clrBtnHilite;
  }

  for (int y = 0; y <= cy / 2; y++) {
    COLORREF* pBits = (COLORREF*)(im->tt.data + (rect.t + y) * im->s + rect.l * im->c);

    for (int x = 0; x < cx; x++, pBits++) {
      COLORREF color = (COLORREF) * pBits;

      if (color != clrTransparent) {
        double H, S, L;
        RGBtoHSL(color, &H, &S, &L);
        color = HLStoRGB_ONE(H, L, 0);

        if (nPercentage == -1) {
          *pBits = RGB_TO_RGBA(
              min(255, GetRV(color) + ((GetBV(clrDisabled) - GetRV(color)) / 2)),
              min(255, GetGV(color) + ((GetGV(clrDisabled) - GetGV(color)) / 2)),
              min(255, GetBV(color) + ((GetRV(clrDisabled) - GetBV(color)) / 2)));
        }
        else {
          *pBits = CLR_TO_RGBA(PixelAlpha(color, .01 * nPercentage, .01 * nPercentage, .01 * nPercentage));
        }
      }
    }
  }

  return TRUE;
}

COLORREF MapToSysColorAlpha(COLORREF color)
{
  BYTE r = GetRV(color);
  BYTE g = GetGV(color);
  BYTE b = GetBV(color);

  const int nDelta = 10;

  if (abs(r - b) > nDelta || abs(r - g) > nDelta || abs(b - g) > nDelta) {
    return color;
  }

  return PixelAlpha(globalData.clrBarFace,
      1. + ((double) r - 192) / 255,
      1. + ((double) g - 192) / 255,
      1. + ((double) b - 192) / 255);
}
COLORREF MapFromSysColor(COLORREF color, BOOL bUseRGBQUAD = TRUE)
{
  struct COLORMAP {
    // use DWORD instead of RGBQUAD so we can compare two RGBQUADs easily
    DWORD rgbTo;
    int iSysColorFrom;
  };
  static const COLORMAP sysColorMap[] = {
    // mapping from color in DIB to system color
    { _RGB(0x00, 0x00, 0x00),  COLOR_BTNTEXT },       // black
    { _RGB(0x80, 0x80, 0x80),  COLOR_BTNSHADOW },     // dark grey
    { _RGB(0xC0, 0xC0, 0xC0),  COLOR_BTNFACE },       // bright grey
    { _RGB(0xFF, 0xFF, 0xFF),  COLOR_BTNHIGHLIGHT }   // white
  };
  const int nMaps = 4;

  // look for matching RGBQUAD color in original
  for (int i = 0; i < nMaps; i++) {
    COLORREF clrSystem = globalData.GetColor(sysColorMap[i].iSysColorFrom);

    if (bUseRGBQUAD) {
      if (color == CLR_TO_RGBQUAD(clrSystem)) {
        return CLR_TO_RGBQUAD(sysColorMap[i].rgbTo);
      }
    }
    else {
      if (color == clrSystem) {
        return sysColorMap[i].rgbTo;
      }
    }
  }

  return color;
}

//*******************************************************************************
COLORREF MapToSysColor(COLORREF color, BOOL bUseRGBQUAD = TRUE)
{
  struct COLORMAP {
    // use DWORD instead of RGBQUAD so we can compare two RGBQUADs easily
    DWORD rgbqFrom;
    int iSysColorTo;
  };
  static const COLORMAP sysColorMap[] = {
    // mapping from color in DIB to system color
    { RGB_TO_RGBQUAD(0x00, 0x00, 0x00),  COLOR_BTNTEXT },       // black
    { RGB_TO_RGBQUAD(0x80, 0x80, 0x80),  COLOR_BTNSHADOW },     // dark grey
    { RGB_TO_RGBQUAD(0xC0, 0xC0, 0xC0),  COLOR_BTNFACE },       // bright grey
    { RGB_TO_RGBQUAD(0xFF, 0xFF, 0xFF),  COLOR_BTNHIGHLIGHT }   // white
  };
  const int nMaps = 4;

  // look for matching RGBQUAD color in original
  for (int i = 0; i < nMaps; i++) {
    if (color == sysColorMap[i].rgbqFrom) {
      return bUseRGBQUAD ?
          CLR_TO_RGBQUAD(globalData.GetColor(sysColorMap[i].iSysColorTo)) :
          globalData.GetColor(sysColorMap[i].iSysColorTo);
    }
  }

  return color;
}
//***********************************************************************************
BOOL MapBmpTo3dColors(img_t* im,
    BOOL bUseRGBQUAD = TRUE,
    COLORREF clrSrc = 0,
    COLORREF clrDest = 0,
    BOOL m_bDisableTrueColorAlpha = TRUE)
{
  if (im == NULL) {
    return FALSE;
  }

  if (clrSrc != 0 && clrDest == 0) {
    ASSERT(FALSE);
    return FALSE;
  }

  //-------------------------------------------------------
  // Create memory source DC and select an original bitmap:
  //-------------------------------------------------------

  CBitmap* hOldBitmapSrc = NULL;

  int iBitmapWidth;
  int iBitmapHeight;

  //-------------------------------
  // Get original bitmap attrbutes:
  //-------------------------------

  iBitmapWidth = im->w;
  iBitmapHeight = im->h;

  //------------------------------------
  // Change a specific colors to system:
  //------------------------------------
  for (int y = 0; y < iBitmapHeight; y ++) {
    COLORREF* pix = (COLORREF*)(im->tt.data + y * im->s);

    for (int x = 0; x < iBitmapWidth; ++x, ++pix) {
      COLORREF clrOrig = *pix;

      if (clrSrc != 0) {
        if (clrOrig == clrSrc) {
          *pix = clrDest;
        }
      }
      else {
        COLORREF clrNew = !m_bDisableTrueColorAlpha ? MapToSysColorAlpha(clrOrig) : MapToSysColor(clrOrig, bUseRGBQUAD);

        if (clrOrig != clrNew) {
          *pix = clrNew;
        }
      }
    }
  }

  return TRUE;
}

//****************************************************************************
BOOL MirrorBitmapVert(img_t* im, int cyImage = 0)
{
  imflip_ud(im);
  return TRUE;
}

int PrepareShadowMask(CBitmap* hBitmap, int nDepth,
    COLORREF clrBase,
    int iMinBrightness = 0, int iMaxBrightness = 100)
{
#if 0

  if (nDepth == 0) {
    return NULL;
  }

  int nSize     = nDepth < 3 ? 3 : nDepth;
  int nDestSize = nSize * 2 + 1;

  LPBYTE lpBits = NULL;
  CreateBitmap_32(hBitmap, CSize(nDestSize, nDestSize), (void**)&lpBits);
  imresize(hBitmap, nDestSize, nDestSize);

  if (hBitmap == NULL || lpBits == NULL) {
    return NULL;
  }

  // Create mask
  int nDestLength = nDestSize * nDestSize;
  double* mask = new double[nDestLength];

  double dispersion = 1.0;
  double minValue   = iMinBrightness / 100.0;
  double maxValue   = iMaxBrightness / 100.0;
  double delta      = maxValue - minValue;

  long size2      = nDestSize / 2;
  double size2S   = nDestSize * nDestSize / 4.0;

  double* pMask = mask;

  for (long y = -size2; y <= size2; y++) {
    double y2 = y * y;

    for (long x = -size2; x <= size2; x++) {
      double d = y2 + x * x;
      double e = 0.0;

      if (d <= size2S) {
        e = min(maxValue, max(exp(-(d / size2S) / dispersion * 2.0) * delta + minValue, minValue));
      }

      *pMask++ = e;
    }
  }

  BYTE r = (BYTE)(GetRV(clrBase) / 4);
  BYTE g = (BYTE)(GetGV(clrBase) / 4);
  BYTE b = (BYTE)(GetBV(clrBase) / 4);

  pMask = mask;
  LPRGBQUAD pQuad = (LPRGBQUAD)lpBits;

  for (int i = 0; i < nDestLength; i++) {
    pQuad->rgbRed      = (BYTE)(*pMask * r);
    pQuad->rgbGreen    = (BYTE)(*pMask * g);
    pQuad->rgbBlue     = (BYTE)(*pMask * b);
    pQuad->rgbReserved = (BYTE)(*pMask * 255);

    pMask++;
    pQuad++;
  }

  if (mask != NULL) {
    delete [] mask;
  }

#endif
  return 0;
}


// CBCGPAlphaDC class implementation

#if 0

CBCGPAlphaDC::CBCGPAlphaDC(CDC& originalDC, double dblOpacity)
  : m_hOriginalDC(originalDC.m_hDC)
  , m_hBufferBitmap(NULL)
  , m_hTempBitmap(NULL)
  , m_nOpacity((BYTE)(dblOpacity * 255))
  , m_pImageBits(NULL)
{
  ::GetClipBox(originalDC, m_rcClipBox);
  Initialize();
}

CBCGPAlphaDC::CBCGPAlphaDC(CDC& originalDC, const CRect& rcArea, double dblOpacity)
  : m_hOriginalDC(originalDC.m_hDC)
  , m_hBufferBitmap(NULL)
  , m_hTempBitmap(NULL)
  , m_rcClipBox(rcArea)
  , m_nOpacity((BYTE)(dblOpacity * 255))
  , m_pImageBits(NULL)
{
  Initialize();
}

void CBCGPAlphaDC::Initialize()
{
  Attach(::CreateCompatibleDC(m_hOriginalDC));

  CRect rcClip = m_rcClipBox;
  ::LPtoDP(m_hOriginalDC, (IPOINT*)&rcClip, 2);

  m_hBufferBitmap = CreateBitmap_32(rcClip.Size(), (void**)&m_pImageBits);

  if (m_pImageBits == NULL) {
    ASSERT(FALSE);
    return;
  }

  for (int i = 0; i < rcClip.Width() * rcClip.Height(); ++i) {
    m_pImageBits[i] = (DWORD)(255 << 24);
  }

  m_hTempBitmap = (CBitmap*)::SelectObject(m_hDC, m_hBufferBitmap);
  ::SetMapMode(m_hDC, ::GetMapMode(m_hOriginalDC));

  ISIZE szTemp;
  ::GetWindowExtEx(m_hOriginalDC, &szTemp);
  ::SetWindowExtEx(m_hDC, szTemp.w, szTemp.h, NULL);
  ::GetViewportExtEx(m_hOriginalDC, &szTemp);
  ::SetViewportExtEx(m_hDC, szTemp.w, szTemp.h, NULL);
  ::SetWindowOrgEx(m_hDC, m_rcClipBox.l, m_rcClipBox.t, NULL);
}

CBCGPAlphaDC::~CBCGPAlphaDC()
{
  if (m_nOpacity > 255) {
    m_nOpacity = 255;
  }

  if (m_nOpacity > 0) {
    if (globalData.bIsOSAlphaBlendingSupport) {
      // invert alpha
      DWORD alphaMask = (DWORD)(255 << 24);

      for (int i = 0; i < m_rcClipBox.Width() * m_rcClipBox.Height(); ++i) {
        m_pImageBits[i] = (((alphaMask - (m_pImageBits[i] & alphaMask))) & alphaMask) + (m_pImageBits[i] & ~alphaMask);
      }
    }

    DrawAlpha(CDC::FromHandle(m_hOriginalDC), m_rcClipBox,
        this, m_rcClipBox, m_nOpacity);
  }

  ::SelectObject(m_hDC, m_hTempBitmap);
  ::DeleteObject(m_hBufferBitmap);
  ::DeleteDC(Detach());
}

#endif

//BCGCBPRODLLEXPORT extern UINT BCGM_CHANGEVISUALMANAGER;

#endif // !defined(AFX_BCGPVISUALMANAGER_H__8070C021_60CE_11D5_A304_8B7130518C10__INCLUDED_)
