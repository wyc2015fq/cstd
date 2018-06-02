#include "img\imgio.inl"
#include ".\draw\color_space.inl"
typedef struct CBCGPToolTipParams {
  BOOL m_bBallonTooltip;
  BOOL m_bDrawIcon;
  BOOL m_bDrawDescription;
  int m_nMaxDescrWidth;
  BOOL m_bRoundedCorners;
  BOOL m_bBoldLabel;
  COLORREF m_clrFill;
  COLORREF m_clrFillGradient;
  int m_nGradientAngle; // 0 - 360, -1 -default (vertical)
  COLORREF m_clrText;
  COLORREF m_clrBorder;
  BOOL m_bDrawSeparator;
  BOOL m_bVislManagerTheme;
} CBCGPToolTipParams;
int CBCGPToolTipParams_set(CBCGPToolTipParams* s)
{
  s->m_bBallonTooltip = FALSE;
  s->m_bDrawIcon = TRUE;
  s->m_bDrawDescription = TRUE;
  s->m_nMaxDescrWidth = 200;
  s->m_bRoundedCorners = FALSE;
  s->m_bBoldLabel = TRUE;
  s->m_clrFill = (COLORREF) - 1;
  s->m_clrFillGradient = (COLORREF) - 1;
  s->m_nGradientAngle = -1;
  s->m_clrText = (COLORREF) - 1;
  s->m_clrBorder = (COLORREF) - 1;
  s->m_bDrawSeparator = FALSE;
  s->m_bVislManagerTheme = FALSE;
  return 0;
}
typedef enum Style1 {
  VS2007_LunaBlue,
  VS2007_ObsidianBlack,
  VS2007_Aqua,
  VS2007_Silver
} Style1;
typedef img_t CBCGPBitmapCache;
typedef img_t CBCGPControlRenderer;
typedef img_t CBCGPToolBarImages;
typedef struct XRibbonContextCategory {
  CBCGPControlRenderer m_ctrlCaption;
  CBCGPControlRenderer m_ctrlBack;
  CBCGPControlRenderer m_ctrlTab;
  CBCGPControlRenderer m_ctrlBtnDefault;
  COLORREF m_clrCaptionText;
  COLORREF m_clrText;
  COLORREF m_clrTextHighlighted;
  CBCGPBitmapCache m_cacheBack;
  CBCGPBitmapCache m_cacheBtnDefault;
} XRibbonContextCategory;
typedef struct BCGPGLOBAL_DATA res_t;
typedef struct event event;
typedef struct uictrl uictrl;
typedef int (*event_t)(uictrl* ctrl, const event* e);
typedef int (*draw_t)(const uictrl* ctrl, const event* e);
typedef struct BCGPGLOBAL_DATA {
  draw_t dm_draw_f;
  BOOL m_bUseSystemFont; // Use system font for menu/toolbar/ribbons
  // color values of system colors used for CToolBar
  COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
  COLORREF clrBtnText, clrWindowFrame;
  COLORREF clrBtnDkShadow, clrBtnLight;
  COLORREF clrGrayedText;
  COLORREF clrHilite;
  COLORREF clrTextHilite;
  COLORREF clrHotText;
  COLORREF clrHotLinkText;
  COLORREF clrBtnWindow;
  COLORREF clrWindow;
  COLORREF clrWindowText;
  COLORREF clrTextHot;
  COLORREF clrCaptionText;
  COLORREF clrMenuText;
  COLORREF clrActiveCaption;
  COLORREF clrInactiveCaption;
  COLORREF clrActiveCaptionGradient;
  COLORREF clrInactiveCaptionGradient;
  COLORREF clrInactiveCaptionText;
  COLORREF clrActiveBorder;
  COLORREF clrInactiveBorder;
  COLORREF clrLight;
  COLORREF clrBlack;
  COLORREF clrBarFace;
  COLORREF clrBarShadow;
  COLORREF clrBarDkShadow;
  COLORREF clrBarHilite;
  COLORREF clrBarLight;
  // Library cursors:
  HCURSOR m_hcurStretch;
  HCURSOR m_hcurStretchVert;
  HCURSOR m_hcurHand;
  HCURSOR m_hcurSizeAll;
  HCURSOR m_hcurMoveTab;
  HCURSOR m_hcurNoMoveTab;
  HCURSOR m_hcurSelectRow;
  HICON m_hiconTool;
  HICON m_hiconLink;
  HICON m_hiconColors;
  // Shell icon sizes:
  SIZE m_sizeSmallIcon;
  // Toolbar and menu fonts:
  HFONT fontRegular;
  HFONT fontTooltip;
  HFONT fontBold;
  HFONT fontDefaultGUIBold;
  HFONT fontUnderline;
  HFONT fontDefaultGUIUnderline;
  HFONT fontVert;
  HFONT fontVertCaption;
  HFONT fontSmall;
  HFONT fontMarlett; // Standard Windows menu symbols
  BOOL bIsWindowsNT4;
  BOOL bIsWindows9x;
  BOOL bIsWindowsVista;
  BOOL bDisableAero;
  BOOL bIsRemoteSession;
  BOOL bIsOSAlphaBlendingSupport;
  int m_nDragFrameThiknessFloat;
  int m_nDragFrameThiknessDock;
  int m_nAutoHideToolBarSpacing;
  int m_nAutoHideToolBarMargin;
  int m_nCoveredMainWndClientAreaPercent;
  int m_nMaxToolTipWidth;
  BOOL m_bIsBlackHighContrast;
  BOOL m_bIsWhiteHighContrast;
  BOOL m_bUseBuiltIn32BitIcons;
  BOOL m_bUseVisualManagerInBuiltInDialogs;
  RECT m_rectVirtual;
  BOOL m_bMenuAnimation;
  BOOL m_bMenuFadeEffect;
  int m_nShellAutohideBars;
  BOOL m_bIsRTL;
  BOOL m_bEnableAccessibility;
  BOOL m_bInSettingsChange;
  BOOL m_bUnderlineKeyboardShortcuts;
  BOOL m_bSysUnderlineKeyboardShortcuts;
  int m_nTextHeightHorz;
  int m_nTextHeightVert;
  int m_nTextMarginsHorz;
  int m_nTextMarginsVert;
  int m_nTextWidthHorz;
  int m_nTextWidthVert;
  double m_dblRibbonImageScale;
  BOOL m_bIsRibbonImageScale;
  DWORD m_dwComCtlVersion;
  BOOL m_bDisableTrueColorAlpha;
  BOOL m_bMultiThreaded; // Set to TRUE if images are used in different threads
  BOOL m_bIsDrawOnGlass; // Draw image on Vista Glass area
  double m_dblColorTolerance;// Used by 'AddaptColors' method
  // CBCGPVisualManagerXP
  COLORREF m_clrGripperHorz;
  COLORREF m_clrGripperVert;
  COLORREF m_clrBarBkgnd; // Control bar background color (expect menu bar)
  COLORREF m_clrMenuRarelyUsed;
  COLORREF m_clrMenuLight; // Color of the light menu area
  COLORREF m_clrInactiveTabText;
  COLORREF m_clrHighlight; // Highlighted toolbar/menu item color
  COLORREF m_clrHighlightDn; // Highlighted and pressed toolbar item color
  COLORREF m_clrHighlightChecked;
  COLORREF m_clrPressedButtonBorder; // Used in derived classes
  COLORREF m_clrGripper;
  COLORREF m_clrSeparator;
  COLORREF m_clrPaneBorder; // Status bar pane border
  COLORREF m_clrMenuBorder; // Menu border
  COLORREF m_clrMenuItemBorder; // Highlighted menu item border
  COLORREF m_clrFloatToolBarBorder; // Floating toolbar border
  BOOL m_bConnectMenuToParent;
  BOOL m_bShdowDroppedDownMenuButton;
  BOOL m_bDrawLastTabLine;
  COLORREF m_clrTaskPaneHotText;
  // CBCGPVisualManager2003
  COLORREF m_clrBarGradientDark;
  COLORREF m_clrBarGradientLight;
  COLORREF m_clrToolBarGradientDark;
  COLORREF m_clrToolBarGradientLight;
  COLORREF m_clrToolbarDisabled;
  COLORREF m_clrToolBarGradientVertLight;
  COLORREF m_clrToolBarGradientVertDark;
  COLORREF m_clrCustomizeButtonGradientDark;
  COLORREF m_clrCustomizeButtonGradientLight;
  COLORREF m_clrToolBarBottomLine;
  COLORREF m_penBottomLine;
  COLORREF m_colorToolBarCornerTop;
  COLORREF m_colorToolBarCornerBottom;
  COLORREF m_clrHighlightMenuItem;
  COLORREF m_clrHighlightGradientLight;
  COLORREF m_clrHighlightGradientDark;
  COLORREF m_clrHighlightDnGradientLight;
  COLORREF m_clrHighlightDnGradientDark;
  COLORREF m_clrHighlightCheckedGradientLight;
  COLORREF m_clrHighlightCheckedGradientDark;
  COLORREF m_penSeparatorLight;
  COLORREF m_clrCaptionBarGradientLight;
  COLORREF m_clrCaptionBarGradientDark;
  COLORREF m_clrTearOffCaption;
  COLORREF m_clrFace;
  COLORREF m_clrTaskPaneGradientDark;
  COLORREF m_clrTaskPaneGradientLight;
  COLORREF m_clrTaskPaneGroupCaptionDark;
  COLORREF m_clrTaskPaneGroupCaptionLight;
  COLORREF m_clrTaskPaneGroupCaptionSpecDark;
  COLORREF m_clrTaskPaneGroupCaptionSpecLight;
  COLORREF m_clrTaskPaneGroupAreaLight;
  COLORREF m_clrTaskPaneGroupAreaDark;
  COLORREF m_clrTaskPaneGroupAreaSpecLight;
  COLORREF m_clrTaskPaneGroupAreaSpecDark;
  COLORREF m_clrTaskPaneGroupBorder;
  COLORREF m_penTaskPaneGroupBorder;
  COLORREF m_penGridExpandBoxLight;
  COLORREF m_penGridExpandBoxDark;
  COLORREF m_clrPlannerTodayFill;
  COLORREF m_penPlannerTodayLine;
  BOOL m_bIsStandardWinXPTheme;
  //static BOOL m_bUseGlobalTheme;
  //static BOOL m_bStatusBarOfficeXPLook;
  //static BOOL m_bDefaultWinXPColors;
  // CBCGPVisualManagerVS2005
  COLORREF m_colorActiveTabBorder;
  COLORREF m_penActiveTabBorder;
  COLORREF m_clrMenuButtonDroppedDown;
  COLORREF m_clrMenuItemCheckedHighlight;
  // CBCGPVisualManagerVS2008
  BOOL m_bOSColors;
  BOOL m_bLoaded;
  SIZE m_szNcBtnSize[2];
  BOOL m_bNcTextCenter;
  CBCGPToolBarImages m_ToolBarGripper;
  CBCGPToolBarImages m_ToolBarTear;
  CBCGPControlRenderer m_ctrlToolBarBorder;
  CBCGPControlRenderer m_ctrlStatusBarBack;
  CBCGPControlRenderer m_ctrlStatusBarBack_Ext;
  CBCGPToolBarImages m_StatusBarPaneBorder;
  CBCGPToolBarImages m_StatusBarSizeBox;
  CBCGPControlRenderer m_SysBtnBack[2];
  CBCGPToolBarImages m_SysBtnClose[2];
  CBCGPToolBarImages m_SysBtnRestore[2];
  CBCGPToolBarImages m_SysBtnMaximize[2];
  CBCGPToolBarImages m_SysBtnMinimize[2];
  CBCGPToolBarImages m_SysBtnHelp[2];
  // CFont m_AppCaptionFont;
  COLORREF m_clrAppCaptionActiveStart;
  COLORREF m_clrAppCaptionActiveFinish;
  COLORREF m_clrAppCaptionInactiveStart;
  COLORREF m_clrAppCaptionInactiveFinish;
  COLORREF m_clrAppCaptionActiveText;
  COLORREF m_clrAppCaptionInactiveText;
  COLORREF m_clrAppCaptionActiveTitleText;
  COLORREF m_clrAppCaptionInactiveTitleText;
  COLORREF m_clrMainClientArea;
  COLORREF m_clrMenuBarGradientLight;
  COLORREF m_clrMenuBarGradientDark;
  COLORREF m_clrMenuBarGradientVertLight;
  COLORREF m_clrMenuBarGradientVertDark;
  COLORREF m_clrMenuBarBtnText;
  COLORREF m_clrMenuBarBtnTextHighlighted;
  COLORREF m_clrMenuBarBtnTextDisabled;
  COLORREF m_clrToolBarBtnText;
  COLORREF m_clrToolBarBtnTextHighlighted;
  COLORREF m_clrToolBarBtnTextDisabled;
  COLORREF m_clrMenuText;
  COLORREF m_clrMenuTextHighlighted;
  COLORREF m_clrMenuTextDisabled;
  COLORREF m_clrStatusBarText;
  COLORREF m_clrStatusBarTextDisabled;
  COLORREF m_clrExtenedStatusBarTextDisabled;
  COLORREF m_clrEditBorder;
  COLORREF m_clrEditBorderDisabled;
  COLORREF m_clrEditBorderHighlighted;
  COLORREF m_clrEditSelection;
  COLORREF m_clrComboBorder;
  COLORREF m_clrComboBorderDisabled;
  COLORREF m_clrComboBorderPressed;
  COLORREF m_clrComboBorderHighlighted;
  COLORREF m_clrComboBtnStart;
  COLORREF m_clrComboBtnFinish;
  COLORREF m_clrComboBtnBorder;
  COLORREF m_clrComboBtnDisabledStart;
  COLORREF m_clrComboBtnDisabledFinish;
  COLORREF m_clrComboBtnBorderDisabled;
  COLORREF m_clrComboBtnPressedStart;
  COLORREF m_clrComboBtnPressedFinish;
  COLORREF m_clrComboBtnBorderPressed;
  COLORREF m_clrComboBtnHighlightedStart;
  COLORREF m_clrComboBtnHighlightedFinish;
  COLORREF m_clrComboBtnBorderHighlighted;
  COLORREF m_clrComboSelection;
  CBCGPControlRenderer m_ctrlComboBoxBtn;
  COLORREF m_clrHeaderNormalStart;
  COLORREF m_clrHeaderNormalFinish;
  COLORREF m_clrHeaderNormalBorder;
  COLORREF m_clrHeaderHighlightedStart;
  COLORREF m_clrHeaderHighlightedFinish;
  COLORREF m_clrHeaderHighlightedBorder;
  COLORREF m_clrHeaderPressedStart;
  COLORREF m_clrHeaderPressedFinish;
  COLORREF m_clrHeaderPressedBorder;
  COLORREF m_clrBarCaption;
  COLORREF m_clrMiniFrameCaption;
  COLORREF m_clrSeparator1;
  COLORREF m_clrSeparator2;
  COLORREF m_penSeparator2;
  COLORREF m_penSeparatorDark;
  COLORREF m_clrGroupText;
  COLORREF m_clrGroupBackground;
  COLORREF m_clrCaptionBarText;
  CBCGPControlRenderer m_ctrlMainBorder;
  CBCGPControlRenderer m_ctrlMDIChildBorder;
  CBCGPControlRenderer m_ctrlDialogBorder;
  CBCGPControlRenderer m_ctrlMainBorderCaption;
  CBCGPControlRenderer m_ctrlPopupBorder;
  CBCGPControlRenderer m_ctrlPopupResizeBar;
  CBCGPToolBarImages m_PopupResizeBar_HV;
  CBCGPToolBarImages m_PopupResizeBar_HVT;
  CBCGPToolBarImages m_PopupResizeBar_V;
  CBCGPControlRenderer m_ctrlMenuBarBtn;
  CBCGPControlRenderer m_ctrlMenuItemBack;
  CBCGPToolBarImages m_MenuItemMarkerC;
  CBCGPToolBarImages m_MenuItemMarkerR;
  CBCGPControlRenderer m_ctrlMenuItemShowAll;
  CBCGPControlRenderer m_ctrlMenuHighlighted[2];
  CBCGPControlRenderer m_ctrlMenuButtonBorder;
  CBCGPControlRenderer m_ctrlMenuScrollBtn[2];
  CBCGPControlRenderer m_ctrlToolBarBtn;
#ifndef BCGP_EXCLUDE_TASK_PANE
  COLORREF m_clrTaskPaneGroupCaptionHighDark;
  COLORREF m_clrTaskPaneGroupCaptionHighLight;
  COLORREF m_clrTaskPaneGroupCaptionHighSpecDark;
  COLORREF m_clrTaskPaneGroupCaptionHighSpecLight;
  COLORREF m_clrTaskPaneGroupCaptionTextSpec;
  COLORREF m_clrTaskPaneGroupCaptionTextHighSpec;
  COLORREF m_clrTaskPaneGroupCaptionText;
  COLORREF m_clrTaskPaneGroupCaptionTextHigh;
  CBCGPControlRenderer m_ctrlTaskScrollBtn;
#endif
  CBCGPControlRenderer m_ctrlTab3D[2];
  COLORREF m_clrTab3DFace;
  COLORREF m_clrTab3DBlack;
  COLORREF m_clrTab3DDark;
  COLORREF m_clrTab3DDarkShadow;
  COLORREF m_clrTab3DLight;
  COLORREF m_clrTab3DHighlight;
  CBCGPControlRenderer m_ctrlTabFlat[2];
  COLORREF m_clrTabFlatFace;
  COLORREF m_clrTabFlatBlack;
  COLORREF m_clrTabFlatDark;
  COLORREF m_clrTabFlatDarkShadow;
  COLORREF m_clrTabFlatLight;
  COLORREF m_clrTabFlatHighlight;
  COLORREF m_penTabFlatInner[2];
  COLORREF m_penTabFlatOuter[2];
  COLORREF m_clrTabTextActive;
  COLORREF m_clrTabTextInactive;
  CBCGPControlRenderer m_ctrlOutlookWndBar;
  CBCGPControlRenderer m_ctrlOutlookWndPageBtn;
  COLORREF m_clrOutlookPageTextNormal;
  COLORREF m_clrOutlookPageTextHighlighted;
  COLORREF m_clrOutlookPageTextPressed;
  COLORREF m_clrOutlookCaptionTextNormal;
  CBCGPControlRenderer m_ctrlScrollBar_Back[2][2];
  CBCGPControlRenderer m_ctrlScrollBar_Item[2][2];
  CBCGPControlRenderer m_ctrlScrollBar_ThumbBack[2][2];
  CBCGPControlRenderer m_ctrlScrollBar_ThumbIcon[2][2];
  CBCGPControlRenderer m_ctrlSliderThumb[6];
  CBCGPControlRenderer m_ctrlRibbonCaptionQA;
  CBCGPControlRenderer m_ctrlRibbonCaptionQA_Glass;
  COLORREF m_clrRibbonCategoryText;
  COLORREF m_clrRibbonCategoryTextHighlighted;
  COLORREF m_clrRibbonCategoryTextDisabled;
  CBCGPControlRenderer m_ctrlRibbonCategoryBack;
  CBCGPControlRenderer m_ctrlRibbonCategoryTab;
  CBCGPControlRenderer m_ctrlRibbonCategoryTabSep;
  CBCGPControlRenderer m_ctrlRibbonCategoryBtnPage[2];
  COLORREF m_clrRibbonPanelText;
  COLORREF m_clrRibbonPanelTextHighlighted;
  COLORREF m_clrRibbonPanelCaptionText;
  COLORREF m_clrRibbonPanelCaptionTextHighlighted;
  CBCGPControlRenderer m_ctrlRibbonPanelBack_T;
  CBCGPControlRenderer m_ctrlRibbonPanelBack_B;
  CBCGPToolBarImages m_RibbonPanelSeparator;
  CBCGPControlRenderer m_ctrlRibbonPanelQAT;
  CBCGPControlRenderer m_ctrlRibbonMainPanel;
  CBCGPControlRenderer m_ctrlRibbonMainPanelBorder;
  CBCGPControlRenderer m_ctrlRibbonBtnMainPanel;
  CBCGPControlRenderer m_ctrlRibbonBtnGroup_S;
  CBCGPControlRenderer m_ctrlRibbonBtnGroup_F;
  CBCGPControlRenderer m_ctrlRibbonBtnGroup_M;
  CBCGPControlRenderer m_ctrlRibbonBtnGroup_L;
  CBCGPControlRenderer m_ctrlRibbonBtnGroupMenu_F[2];
  CBCGPControlRenderer m_ctrlRibbonBtnGroupMenu_M[2];
  CBCGPControlRenderer m_ctrlRibbonBtnGroupMenu_L[2];
  CBCGPControlRenderer m_ctrlRibbonBtn[2];
  CBCGPControlRenderer m_ctrlRibbonBtnMenuH[2];
  CBCGPControlRenderer m_ctrlRibbonBtnMenuV[2];
  CBCGPControlRenderer m_ctrlRibbonBtnLaunch;
  CBCGPToolBarImages m_RibbonBtnLaunchIcon;
  CBCGPControlRenderer m_RibbonBtnMain;
  CBCGPControlRenderer m_ctrlRibbonBtnDefault;
  CBCGPControlRenderer m_ctrlRibbonBtnDefaultIcon;
  CBCGPToolBarImages m_RibbonBtnDefaultImage;
  CBCGPControlRenderer m_ctrlRibbonBtnDefaultQATIcon;
  CBCGPControlRenderer m_ctrlRibbonBtnDefaultQAT;
  CBCGPControlRenderer m_ctrlRibbonBtnCheck;
  CBCGPControlRenderer m_ctrlRibbonBtnRadio;
  CBCGPControlRenderer m_ctrlRibbonBtnPush;
  CBCGPControlRenderer m_ctrlRibbonBtnGroup;
  CBCGPControlRenderer m_ctrlRibbonBtnPalette[3];
  CBCGPControlRenderer m_ctrlRibbonBtnStatusPane;
  CBCGPControlRenderer m_ctrlRibbonSliderThumb;
  CBCGPControlRenderer m_ctrlRibbonSliderThumbA[5];
  CBCGPControlRenderer m_ctrlRibbonSliderBtnPlus;
  CBCGPControlRenderer m_ctrlRibbonSliderBtnMinus;
  CBCGPControlRenderer m_ctrlRibbonProgressBack;
  CBCGPControlRenderer m_ctrlRibbonProgressNormal;
  CBCGPControlRenderer m_ctrlRibbonProgressNormalExt;
  CBCGPControlRenderer m_ctrlRibbonProgressInfinity;
  CBCGPControlRenderer m_ctrlRibbonBorder_QAT;
  CBCGPControlRenderer m_ctrlRibbonBorder_Floaty;
  CBCGPControlRenderer m_ctrlRibbonKeyTip;
  COLORREF m_clrRibbonKeyTipTextNormal;
  COLORREF m_clrRibbonKeyTipTextDisabled;
  COLORREF m_clrRibbonEdit;
  COLORREF m_clrRibbonEditDisabled;
  COLORREF m_clrRibbonEditHighlighted;
  COLORREF m_clrRibbonEditPressed;
  COLORREF m_clrRibbonEditBorder;
  COLORREF m_clrRibbonEditBorderDisabled;
  COLORREF m_clrRibbonEditBorderHighlighted;
  COLORREF m_clrRibbonEditBorderPressed;
  COLORREF m_clrRibbonEditSelection;
  COLORREF m_clrRibbonComboBtnStart;
  COLORREF m_clrRibbonComboBtnFinish;
  COLORREF m_clrRibbonComboBtnBorder;
  COLORREF m_clrRibbonComboBtnDisabledStart;
  COLORREF m_clrRibbonComboBtnDisabledFinish;
  COLORREF m_clrRibbonComboBtnBorderDisabled;
  COLORREF m_clrRibbonComboBtnPressedStart;
  COLORREF m_clrRibbonComboBtnPressedFinish;
  COLORREF m_clrRibbonComboBtnBorderPressed;
  COLORREF m_clrRibbonComboBtnHighlightedStart;
  COLORREF m_clrRibbonComboBtnHighlightedFinish;
  COLORREF m_clrRibbonComboBtnBorderHighlighted;
  CBCGPControlRenderer m_ctrlRibbonComboBoxBtn;
  CBCGPControlRenderer m_ctrlRibbonContextPanelBack_T;
  CBCGPControlRenderer m_ctrlRibbonContextPanelBack_B;
  COLORREF m_clrRibbonContextPanelText;
  COLORREF m_clrRibbonContextPanelTextHighlighted;
  COLORREF m_clrRibbonContextPanelCaptionText;
  COLORREF m_clrRibbonContextPanelCaptionTextHighlighted;
  CBCGPBitmapCache m_cacheRibbonContextPanelBack_T;
  CBCGPBitmapCache m_cacheRibbonContextPanelBack_B;
  CBCGPControlRenderer m_ctrlRibbonContextSeparator;
#define BCGPRibbonCategoryColorCount 7
  XRibbonContextCategory m_ctrlRibbonContextCategory[BCGPRibbonCategoryColorCount];
  //static Style m_Style;
  //static CString m_strStylePrefix;
  //static HINSTANCE m_hinstRes;
  //static BOOL m_bAutoFreeRes;
  COLORREF m_clrBase;
  COLORREF m_clrTarget;
  CBCGPBitmapCache m_cacheRibbonCategoryBack;
  CBCGPBitmapCache m_cacheRibbonPanelBack_T;
  CBCGPBitmapCache m_cacheRibbonPanelBack_B;
  CBCGPBitmapCache m_cacheRibbonBtnDefault;
  CBCGPBitmapCache m_cacheRibbonBtnGroup_S;
  CBCGPBitmapCache m_cacheRibbonBtnGroup_F;
  CBCGPBitmapCache m_cacheRibbonBtnGroup_M;
  CBCGPBitmapCache m_cacheRibbonBtnGroup_L;
  CBCGPBitmapCache m_cacheRibbonBtnGroupMenu_F[2];
  CBCGPBitmapCache m_cacheRibbonBtnGroupMenu_M[2];
  CBCGPBitmapCache m_cacheRibbonBtnGroupMenu_L[2];
  int m_nType;
  BOOL m_bPlannerBlack;
  COLORREF m_clrPlannerTodayCaption[4];
  COLORREF m_clrPlannerTodayBorder;
  COLORREF m_clrPlannerNcArea;
  COLORREF m_clrPlannerNcLine;
  COLORREF m_clrPlannerNcText;
  COLORREF m_clrGridHeaderNormalStart;
  COLORREF m_clrGridHeaderNormalFinish;
  COLORREF m_clrGridHeaderNormalBorder;
  COLORREF m_clrGridHeaderPressedStart;
  COLORREF m_clrGridHeaderPressedFinish;
  COLORREF m_clrGridHeaderPressedBorder;
  COLORREF m_clrGridHeaderAllNormalBackStart;
  COLORREF m_clrGridHeaderAllNormalBackFinish;
  COLORREF m_clrGridHeaderAllPressedBackStart;
  COLORREF m_clrGridHeaderAllPressedBackFinish;
  COLORREF m_clrGridHeaderAllNormalBorderHighlighted;
  COLORREF m_clrGridHeaderAllNormalBorderShadow;
  COLORREF m_clrGridHeaderAllPressedBorderHighlighted;
  COLORREF m_clrGridHeaderAllPressedBorderShadow;
  COLORREF m_clrGridHeaderAllNormalSignStart;
  COLORREF m_clrGridHeaderAllNormalSignFinish;
  COLORREF m_clrGridHeaderAllPressedSignStart;
  COLORREF m_clrGridHeaderAllPressedSignFinish;
  COLORREF m_clrGridSeparator;
  COLORREF m_penGridSeparator;
  COLORREF m_clrGridLeftOffset;
  COLORREF m_clrGridGroupLine;
  COLORREF m_clrGridGroupSubLine;
  COLORREF m_clrPopupGradientLight;
  COLORREF m_clrPopupGradientDark;
  CBCGPToolTipParams m_ToolTipParams;
  BOOL m_bToolTipParams;
  COLORREF m_clrRibbonHyperlinkInactive;
  COLORREF m_clrRibbonHyperlinkActive;
  COLORREF m_clrRibbonStatusbarHyperlinkInactive;
  COLORREF m_clrRibbonStatusbarHyperlinkActive;
  COLORREF m_clrDlgBackground;
} BCGPGLOBAL_DATA;
BCGPGLOBAL_DATA globalData = {0};
typedef enum BCGP_DOCK_TYPE {
  BCGP_DT_UNDEFINED = 0, // inherit from application
  BCGP_DT_IMMEDIATE = 1, // control bar torn off immediately and follows the mouse
  BCGP_DT_STANDARD = 2, // user drags a frame
  BCGP_DT_SMART = 0x80 // smart docking style
} BCGP_DOCK_TYPE;
// autohide sliding modes
static const UINT BCGP_AHSM_MOVE = 1;
static const UINT BCGP_AHSM_STRETCH = 2;
#define BCGP_AUTOHIDE_LEFT 0x0001
#define BCGP_AUTOHIDE_RIGHT 0x0002
#define BCGP_AUTOHIDE_TOP 0x0004
#define BCGP_AUTOHIDE_BOTTOM 0x0008
// BCGP_BP_BUFFERFORMAT
typedef enum _BCGP_BP_BUFFERFORMAT {
  BCGP_BPBF_COMPATIBLEBITMAP, // Compatible bitmap
  BCGP_BPBF_DIB, // Device-independent bitmap
  BCGP_BPBF_TOPDOWNDIB, // Top-down device-independent bitmap
  BCGP_BPBF_TOPDOWNMONODIB // Top-down monochrome device-independent bitmap
} BCGP_BP_BUFFERFORMAT;
#define BCGP_BP_BUFFERFORMATBPBF_COMPOSITED BCGP_BP_BUFFERFORMATBPBF_DIB
#define IMAGE_MARGIN 4
// MFC Control bar compatibility
#define CX_BORDER 1
#define CY_BORDER 1
#define CX_GRIPPER 3
#define CY_GRIPPER 3
#define CX_BORDER_GRIPPER 2
#define CY_BORDER_GRIPPER 2
// COPYRIGHT NOTES
// This is a part of the BCGControlBar Library
// Copyright(C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions
// of the accompanying license agreement.
#ifndef SPI_GETMENUANIMATION
#define SPI_GETMENUANIMATION 0x1002
#endif
#ifndef SPI_GETMENUFADE
#define SPI_GETMENUFADE 0x1012
#endif
#ifndef SPI_GETMENUUNDERLINES
#define SPI_GETKEYBOARDCUES 0x100A
#define SPI_GETMENUUNDERLINES SPI_GETKEYBOARDCUES
#endif
static const char* strOfficeFontName = _T("Tahoma");
static const char* strOffice2007FontName = _T("Segoe UI");
static const char* strDefaultFontName = _T("MS Sans Serif");
static const char* strVertFontName = _T("Arial");
static const char* strMarlettFontName = _T("Marlett");
int GetTextHeight(BOOL bHorz DEFULT(TRUE))
{
  return bHorz ? globalData.m_nTextHeightHorz : globalData.m_nTextHeightVert;
}
int GetTextWidth(BOOL bHorz DEFULT(TRUE))
{
  return bHorz ? globalData.m_nTextWidthHorz : globalData.m_nTextWidthVert;
}
int GetTextMargins(BOOL bHorz DEFULT(TRUE))
{
  return bHorz ? globalData.m_nTextMarginsHorz : globalData.m_nTextMarginsVert;
}
double GetRibbonImageScale()
{
  return globalData.m_bIsRibbonImageScale ? globalData.m_dblRibbonImageScale : 1.;
}
void EnableRibbonImageScale(BOOL bEnable DEFULT(TRUE))
{
  globalData.m_bIsRibbonImageScale = bEnable;
}
BOOL IsRibbonImageScaleEnabled()
{
  return globalData.m_bIsRibbonImageScale;
}
BOOL Is32BitIcons()
{
  return globalData.m_bUseBuiltIn32BitIcons && !globalData.m_bIsBlackHighContrast && !globalData.m_bIsWhiteHighContrast;
}
BOOL IsHighContastMode()
{
  return globalData.m_bIsWhiteHighContrast || globalData.m_bIsBlackHighContrast;
}
static int CALLBACK FontFamalyProcFonts(const LOGFONT FAR* lplf, const TEXTMETRIC FAR* lptm,
    ULONG ulFontType, LPARAM lParam)
{
  ASSERT(lplf != 0);
  ASSERT(lParam != 0);
  return stricmp(lplf->lfFaceName, (LPCTSTR)lParam) == 0 ? 0 : 1;
}
// Cached system metrics, etc
void UpdateSysColors()
{
  OSVERSIONINFO osvi;
  HDC dc = GetMemDC();
  globalData.m_bIsBlackHighContrast =
      _GetSysColor(COLOR_3DLIGHT) == _RGB(255, 255, 255) &&
      _GetSysColor(COLOR_3DFACE) == _RGB(0, 0, 0);
  globalData.m_bIsWhiteHighContrast =
      _GetSysColor(COLOR_3DDKSHADOW) == _RGB(0, 0, 0) &&
      _GetSysColor(COLOR_3DFACE) == _RGB(255, 255, 255);
  globalData.clrBtnFace = globalData.clrBtnFace = _GetSysColor(COLOR_BTNFACE);
  globalData.clrBtnShadow = globalData.clrBtnShadow = _GetSysColor(COLOR_BTNSHADOW);
  globalData.clrBtnDkShadow = globalData.clrBtnDkShadow = _GetSysColor(COLOR_3DDKSHADOW);
  globalData.clrBtnLight = globalData.clrBtnLight = _GetSysColor(COLOR_3DLIGHT);
  globalData.clrBtnHilite = globalData.clrBtnHilite = _GetSysColor(COLOR_BTNHIGHLIGHT);
  globalData.clrBtnText = globalData.clrBtnText = _GetSysColor(COLOR_BTNTEXT);
  globalData.clrGrayedText = _GetSysColor(COLOR_GRAYTEXT);
  globalData.clrWindowFrame = _GetSysColor(COLOR_WINDOWFRAME);
  globalData.clrHilite = _GetSysColor(COLOR_HIGHLIGHT);
  globalData.clrTextHilite = _GetSysColor(COLOR_HIGHLIGHTTEXT);
  globalData.clrBtnWindow = globalData.clrWindow = _GetSysColor(COLOR_WINDOW);
  globalData.clrWindowText = _GetSysColor(COLOR_WINDOWTEXT);
  globalData.clrTextHot = globalData.clrWindowText;
  globalData.clrCaptionText = _GetSysColor(COLOR_CAPTIONTEXT);
  globalData.clrMenuText = _GetSysColor(COLOR_MENUTEXT);
  globalData.clrActiveCaption = _GetSysColor(COLOR_ACTIVECAPTION);
  globalData.clrInactiveCaption = _GetSysColor(COLOR_INACTIVECAPTION);
  globalData.clrBarFace = globalData.clrBtnFace;
  globalData.clrBarShadow = globalData.clrBtnShadow;
  globalData.clrBarDkShadow = globalData.clrBtnDkShadow;
  globalData.clrBarHilite = globalData.clrBtnHilite;
  globalData.clrBarLight = globalData.clrBtnLight;
  if (globalData.bIsWindowsNT4 || globalData.bIsWindows9x) {
    globalData.clrActiveCaptionGradient = globalData.clrActiveCaption;
    globalData.clrInactiveCaptionGradient = globalData.clrInactiveCaption;
  }
  else {
    globalData.clrActiveCaptionGradient = _GetSysColor(27 /*COLOR_GRADIENTACTIVECAPTION*/);
    globalData.clrInactiveCaptionGradient = _GetSysColor(28 /*COLOR_GRADIENTINACTIVECAPTION*/);
  }
  globalData.clrActiveBorder = _GetSysColor(COLOR_ACTIVEBORDER);
  globalData.clrInactiveBorder = _GetSysColor(COLOR_INACTIVEBORDER);
  globalData.clrInactiveCaptionText = _GetSysColor(COLOR_INACTIVECAPTIONTEXT);
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);
  if (globalData.m_bIsBlackHighContrast) {
    globalData.clrHotText = globalData.clrWindowText;
    globalData.clrHotLinkText = globalData.clrWindowText;
  }
  else {
    if (osvi.dwMajorVersion >= 5) {
      globalData.clrHotText = _GetSysColor(26 /*COLOR_HOTLIGHT*/);
      globalData.clrHotLinkText = _RGB(0, 0, 255); // Light blue
    }
    else {
      globalData.clrHotText = _RGB(0, 0, 255); // Light blue
      globalData.clrHotLinkText = _RGB(255, 0, 255); // Violet
    }
  }
  globalData.clrBtnShadow = _GetSysColor(COLOR_BTNSHADOW);
  globalData.clrBtnHilite = _GetSysColor(COLOR_BTNHIGHLIGHT);
  globalData.clrWindow = _GetSysColor(COLOR_WINDOW);
  globalData.clrLight = _RGB(
      GetRV(globalData.clrBtnFace) + ((GetRV(globalData.clrBtnHilite) -
          GetRV(globalData.clrBtnFace)) / 2),
      GetGV(globalData.clrBtnFace) + ((GetGV(globalData.clrBtnHilite) -
          GetGV(globalData.clrBtnFace)) / 2),
      GetBV(globalData.clrBtnFace) + ((GetBV(globalData.clrBtnHilite) -
          GetBV(globalData.clrBtnFace)) / 2)
      );
}
void UpdateSystemColorsXP(res_t* res)
{
  COLORREF clrTabBack;
  COLORREF clrFloatToolBarBorder;
  if (1) {
    COLORREF clrMix;
    COLORREF clrHL;
    COLORREF clrWindow = res->clrWindow;
    COLORREF clrFace = globalData.clrBarFace;
    double H, S, L;
    double S1;
    double L1;
    res->m_clrMenuLight = _RGB(
        (219 * GetRV(clrWindow) + 36 * GetRV(clrFace)) / 255,
        (219 * GetGV(clrWindow) + 36 * GetGV(clrFace)) / 255,
        (219 * GetBV(clrWindow) + 36 * GetBV(clrFace)) / 255);
    RGBtoHSL(clrFace, &H, &S, &L);
    if (S < 0.1) {
      L1 = MIN(1., L + (1. - L) * .5);
      S1 = S == 0 ? 0 : MIN(1., S + .1);
    }
    else {
      L1 = MIN(1., 0.5 * L + 0.5);
      S1 = MIN(1., S * 2);
    }
    clrTabBack = HLStoRGB_ONE(H, L1, S1);
    res->m_clrBarBkgnd = _RGB(
        (40 * GetRV(clrWindow) + 215 * GetRV(clrFace)) / 255,
        (40 * GetGV(clrWindow) + 215 * GetGV(clrFace)) / 255,
        (40 * GetBV(clrWindow) + 215 * GetBV(clrFace)) / 255);
    res->m_clrMenuRarelyUsed = PixelAlpha2(res->m_clrBarBkgnd, 94);
    res->m_clrInactiveTabText = PixelAlpha2(clrFace, 55);
    clrHL = globalData.clrHilite;
    RGBtoHSL(clrHL, &H, &S, &L);
    clrMix = _RGB(
        (77 * GetRV(clrHL) + 178 * GetRV(res->m_clrMenuLight)) / 255,
        (77 * GetGV(clrHL) + 178 * GetGV(res->m_clrMenuLight)) / 255,
        (77 * GetBV(clrHL) + 178 * GetBV(res->m_clrMenuLight)) / 255);
    if (L > .8) { // The highlight color is very light
      res->m_clrHighlight = PixelAlpha2(clrMix, 91);
      res->m_clrHighlightDn = PixelAlpha2(clrMix, 98);
      res->m_clrMenuItemBorder = PixelAlpha2(globalData.clrHilite, 84);
    }
    else {
      res->m_clrHighlight = PixelAlpha2(clrMix, 102);
      res->m_clrHighlightDn = PixelAlpha2(res->m_clrHighlight, 87);
      res->m_clrMenuItemBorder = globalData.clrHilite;
    }
    res->m_clrHighlightChecked = PixelAlpha2(_RGB(
        (GetRV(clrHL) + 5 * GetRV(res->m_clrMenuLight)) / 6,
        (GetGV(clrHL) + 5 * GetGV(res->m_clrMenuLight)) / 6,
        (GetBV(clrHL) + 5 * GetBV(res->m_clrMenuLight)) / 6),
        100);
    res->m_clrSeparator = PixelAlpha(globalData.clrBarFace, .86, .86, .86);
    res->m_clrPaneBorder = globalData.clrBarShadow;
    res->m_clrMenuBorder = PixelAlpha2(clrFace, 55);
    clrFloatToolBarBorder = PixelAlpha(globalData.clrBarShadow, .85, .85, .85);
    res->m_clrGripper = PixelAlpha2(globalData.clrBarShadow, 110);
  }
  res->m_clrPressedButtonBorder = (COLORREF) - 1; // Used in derived classes
  res->m_clrTaskPaneHotText = globalData.clrHotText;
}
#define VERSION_WIN4 MAKELONG(0, 4)
#define VERSION_IE3 MAKELONG(70, 4)
#define VERSION_IE4 MAKELONG(71, 4)
#define VERSION_IE401 MAKELONG(72, 4)
DWORD GetComCtlVersion()
{
  typedef struct BCGP_DLLVERSIONINFO {
    DWORD cbSize;
    DWORD dwMajorVersion; // Major version
    DWORD dwMinorVersion; // Minor version
    DWORD dwBuildNumber; // Build number
    DWORD dwPlatformID; // DLLVER_PLATFORM_*
  } BCGP_DLLVERSIONINFO;
  HINSTANCE hInst;
  DWORD dwVersion;
  typedef HRESULT(CALLBACK * BCGP_DLLGETVERSIONPROC)(BCGP_DLLVERSIONINFO*);
  BCGP_DLLGETVERSIONPROC pfn;
  if (globalData.m_dwComCtlVersion != -1) {
    return globalData.m_dwComCtlVersion;
  }
  hInst = GetModuleHandle(_T("COMCTL32"));
  ASSERT(hInst != NULL);
  pfn = (BCGP_DLLGETVERSIONPROC)GetProcAddress(hInst, "DllGetVersion");
  dwVersion = VERSION_WIN4;
  if (pfn != NULL) {
    HRESULT hr;
    BCGP_DLLVERSIONINFO dvi;
    memset(&dvi, 0, sizeof(dvi));
    dvi.cbSize = sizeof(dvi);
    hr = (*pfn)(&dvi);
    if (SUCCEEDED(hr)) {
      ASSERT(dvi.dwMajorVersion <= 0xFFFF);
      ASSERT(dvi.dwMinorVersion <= 0xFFFF);
      dwVersion = MAKELONG(dvi.dwMinorVersion, dvi.dwMajorVersion);
    }
  }
  globalData.m_dwComCtlVersion = dwVersion;
  return globalData.m_dwComCtlVersion;
}
BOOL GetNonClientMetrics(NONCLIENTMETRICS* ncm)
{
  typedef struct BCGP_OLDNONCLIENTMETRICS {
    UINT cbSize;
    int iBorderWidth;
    int iScrollWidth;
    int iScrollHeight;
    int iCaptionWidth;
    int iCaptionHeight;
    LOGFONT lfCaptionFont;
    int iSmCaptionWidth;
    int iSmCaptionHeight;
    LOGFONT lfSmCaptionFont;
    int iMenuWidth;
    int iMenuHeight;
    LOGFONT lfMenuFont;
    LOGFONT lfStatusFont;
    LOGFONT lfMessageFont;
  } BCGP_OLDNONCLIENTMETRICS;
  const UINT cbProperSize = (GetComCtlVersion() < MAKELONG(1, 6))
      ? sizeof(BCGP_OLDNONCLIENTMETRICS) : sizeof(NONCLIENTMETRICS);
  ZeroMemory(ncm, sizeof(NONCLIENTMETRICS));
  ncm->cbSize = cbProperSize;
  return SystemParametersInfo(SPI_GETNONCLIENTMETRICS, cbProperSize, ncm, 0);
}
void UpdateTextMetrics()
{
  HDC dc = GetMemDC();
  TEXTMETRIC tm;
  HFONT pOldFont = (HFONT)SelectObject(dc, globalData.fontRegular);
  ASSERT(pOldFont != NULL);
  GetTextMetrics(dc, &tm);
  globalData.m_nTextMarginsHorz = tm.tmHeight < 15 ? 2 : 5;
  globalData.m_nTextHeightHorz = tm.tmHeight + globalData.m_nTextMarginsHorz;
  globalData.m_nTextWidthHorz = tm.tmMaxCharWidth + globalData.m_nTextMarginsHorz;
  SelectObject(dc, globalData.fontVert);
  GetTextMetrics(dc, &tm);
  globalData.m_nTextMarginsVert = tm.tmHeight < 15 ? 2 : 5;
  globalData.m_nTextHeightVert = tm.tmHeight + globalData.m_nTextMarginsVert;
  globalData.m_nTextWidthVert = tm.tmMaxCharWidth + globalData.m_nTextMarginsVert;
  SelectObject(dc, pOldFont);
}
void UpdateFonts()
{
  HDC dc = GetMemDC();
  NONCLIENTMETRICS info;
  LOGFONT lf;
  int nFontHeight;
  BOOL fUseSystemFont;
  int lfHeightSaved;
  NONCLIENTMETRICS ncm;
  BYTE bCharSet;
  HFONT font;
  HFONT pDefaultGUIFont;
  globalData.m_dblRibbonImageScale = GetDeviceCaps(dc, LOGPIXELSX) / 96.0f;
  if (globalData.m_dblRibbonImageScale > 1. && globalData.m_dblRibbonImageScale < 1.1) {
    globalData.m_dblRibbonImageScale = 1.;
  }
  if (globalData.fontRegular != NULL) {
    DeleteObject(globalData.fontRegular);
  }
  if (globalData.fontTooltip != NULL) {
    DeleteObject(globalData.fontTooltip);
  }
  if (globalData.fontBold != NULL) {
    DeleteObject(globalData.fontBold);
  }
  if (globalData.fontDefaultGUIBold != NULL) {
    DeleteObject(globalData.fontDefaultGUIBold);
  }
  if (globalData.fontUnderline != NULL) {
    DeleteObject(globalData.fontUnderline);
  }
  if (globalData.fontDefaultGUIUnderline != NULL) {
    DeleteObject(globalData.fontDefaultGUIUnderline);
  }
  if (globalData.fontVert != NULL) {
    DeleteObject(globalData.fontVert);
  }
  if (globalData.fontVertCaption != NULL) {
    DeleteObject(globalData.fontVertCaption);
  }
  if (globalData.fontMarlett != NULL) {
    DeleteObject(globalData.fontMarlett);
  }
  if (globalData.fontSmall != NULL) {
    DeleteObject(globalData.fontSmall);
  }
  // Initialize globalData.fonts:
  GetNonClientMetrics(&info);
  memset(&lf, 0, sizeof(LOGFONT));
  lf.lfCharSet = (BYTE) GetTextCharsetInfo(dc, NULL, 0);
  lf.lfHeight = info.lfMenuFont.lfHeight;
  lf.lfWeight = info.lfMenuFont.lfWeight;
  lf.lfItalic = info.lfMenuFont.lfItalic;
  // Adjust globalData.font size:
  nFontHeight = lf.lfHeight < 0 ? -lf.lfHeight : lf.lfHeight;
  if (nFontHeight <= 12) {
    nFontHeight = 11;
  }
  else {
    nFontHeight--;
  }
  lf.lfHeight = (lf.lfHeight < 0) ? -nFontHeight : nFontHeight;
  lstrcpy(lf.lfFaceName, info.lfMenuFont.lfFaceName);
  // Check if we should use system globalData.font:
  fUseSystemFont = globalData.m_bUseSystemFont || (info.lfMenuFont.lfCharSet > SYMBOL_CHARSET);
  if (!fUseSystemFont) {
    //--
    // Check for "Tahoma" globalData.font existance:
    //--
    if (EnumFontFamilies(dc, NULL, FontFamalyProcFonts,
        (LPARAM)(LPCTSTR) strOffice2007FontName) == 0) {
      //--
      // Found! Use MS Office 2007 globalData.font!
      //--
      lstrcpy(lf.lfFaceName, strOffice2007FontName);
      lf.lfQuality = 5 /*CLEARTYPE_QUALITY*/;
    }
    else if (EnumFontFamilies(dc, NULL, FontFamalyProcFonts,
        (LPARAM)(LPCTSTR) strOfficeFontName) == 0) {
      // Found! Use MS Office globalData.font!
      lstrcpy(lf.lfFaceName, strOfficeFontName);
    }
    else {
      // Not found. Use default globalData.font:
      lstrcpy(lf.lfFaceName, strDefaultFontName);
    }
  }
  globalData.fontRegular = CreateFontIndirect(&lf);
  // Create small globalData.font:
  lfHeightSaved = lf.lfHeight;
  lf.lfHeight = (long)((1. + abs(lf.lfHeight)) * 2 / 3);
  if (lfHeightSaved < 0) {
    lf.lfHeight = -lf.lfHeight;
  }
  globalData.fontSmall = CreateFontIndirect(&lf);
  lf.lfHeight = lfHeightSaved;
  // Create tooltip globalData.font:
  GetNonClientMetrics(&ncm);
  lf.lfItalic = ncm.lfStatusFont.lfItalic;
  lf.lfWeight = ncm.lfStatusFont.lfWeight;
  globalData.fontTooltip = CreateFontIndirect(&lf);
  lf.lfItalic = info.lfMenuFont.lfItalic;
  lf.lfWeight = info.lfMenuFont.lfWeight;
  // Create "underline" globalData.font:
  lf.lfUnderline = TRUE;
  globalData.fontUnderline = CreateFontIndirect(&lf);
  lf.lfUnderline = FALSE;
  // Create bold globalData.font:
  lf.lfWeight = FW_BOLD;
  globalData.fontBold = CreateFontIndirect(&lf);
  // Create Marlett globalData.font:
  bCharSet = lf.lfCharSet;
  lf.lfWeight = info.lfMenuFont.lfWeight;
  lf.lfCharSet = SYMBOL_CHARSET;
  lf.lfWeight = 0;
  lf.lfHeight = GetSystemMetrics(SM_CYMENUCHECK) - 1;
  lstrcpy(lf.lfFaceName, strMarlettFontName);
  globalData.fontMarlett = CreateFontIndirect(&lf);
  lf.lfCharSet = bCharSet; // Restore charset
  // Create vertical globalData.font:
  font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
  if (font != NULL) {
    if (GetLogFont(font, &lf) != 0) {
      lf.lfOrientation = 900;
      lf.lfEscapement = 2700;
      lf.lfHeight = info.lfMenuFont.lfHeight;
      lf.lfWeight = info.lfMenuFont.lfWeight;
      lf.lfItalic = info.lfMenuFont.lfItalic;
      lstrcpy(lf.lfFaceName, strVertFontName);
      globalData.fontVert = CreateFontIndirect(&lf);
      lf.lfEscapement = 900;
      globalData.fontVertCaption = CreateFontIndirect(&lf);
    }
  }
  // Create dialog underline and bold globalData.fonts:
  pDefaultGUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
  ASSERT(pDefaultGUIFont);
  GetLogFont(pDefaultGUIFont, &lf);
  lf.lfUnderline = TRUE;
  globalData.fontDefaultGUIUnderline = CreateFontIndirect(&lf);
  lf.lfUnderline = FALSE;
  lf.lfWeight = FW_BOLD;
  globalData.fontDefaultGUIBold = CreateFontIndirect(&lf);
  UpdateTextMetrics();
}
static BOOL CALLBACK InfoEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
    LPRECT lprcMonitor, LPARAM dwData)
{
  RECT* pRect = (RECT*) dwData;
  MONITORINFO mi;
  mi.cbSize = sizeof(MONITORINFO);
  if (GetMonitorInfo(hMonitor, &mi)) {
    RECT rectMon = mi.rcWork;
    pRect->left = MIN(pRect->left, rectMon.left);
    pRect->right = MAX(pRect->right, rectMon.right);
    pRect->top = MIN(pRect->top, rectMon.top);
    pRect->bottom = MAX(pRect->bottom, rectMon.bottom);
  }
  return TRUE;
}
void OnSettingChange()
{
  OSVERSIONINFO osvi;
  globalData.m_bInSettingsChange = TRUE;
  globalData.m_sizeSmallIcon.w = GetSystemMetrics(SM_CXSMICON);
  globalData.m_sizeSmallIcon.h = GetSystemMetrics(SM_CYSMICON);
  SetRectEmpty(&globalData.m_rectVirtual);
  if (!EnumDisplayMonitors(NULL, NULL, InfoEnumProc, (LPARAM) &globalData.m_rectVirtual)) {
    SystemParametersInfo(SPI_GETWORKAREA, 0, &globalData.m_rectVirtual, 0);
  }
  // Get system menu animation type:
  globalData.m_bMenuAnimation = FALSE;
  globalData.m_bMenuFadeEffect = FALSE;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);
  if (!globalData.bIsRemoteSession && !globalData.bIsWindows9x && osvi.dwMajorVersion >= 5) { // Ignore Win 98/ME
    SystemParametersInfo(SPI_GETMENUANIMATION,
        0, &globalData.m_bMenuAnimation, 0);
    if (globalData.m_bMenuAnimation) {
      SystemParametersInfo(SPI_GETMENUFADE,
          0, &globalData.m_bMenuFadeEffect, 0);
    }
  }
  globalData.m_nShellAutohideBars = 0;
#if(!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)
  if (!globalData.bIsWindows9x && !globalData.bIsWindowsNT4) {
    APPBARDATA abd;
    ZeroMemory(&abd, sizeof(APPBARDATA));
    abd.cbSize = sizeof(APPBARDATA);
    abd.uEdge = ABE_BOTTOM;
    if (SHAppBarMessage(ABM_GETAUTOHIDEBAR, &abd)) {
      globalData.m_nShellAutohideBars |= BCGP_AUTOHIDE_BOTTOM;
    }
    abd.uEdge = ABE_TOP;
    if (SHAppBarMessage(ABM_GETAUTOHIDEBAR, &abd)) {
      globalData.m_nShellAutohideBars |= BCGP_AUTOHIDE_TOP;
    }
    abd.uEdge = ABE_LEFT;
    if (SHAppBarMessage(ABM_GETAUTOHIDEBAR, &abd)) {
      globalData.m_nShellAutohideBars |= BCGP_AUTOHIDE_LEFT;
    }
    abd.uEdge = ABE_RIGHT;
    if (SHAppBarMessage(ABM_GETAUTOHIDEBAR, &abd)) {
      globalData.m_nShellAutohideBars |= BCGP_AUTOHIDE_RIGHT;
    }
  }
#endif
  if (!globalData.bIsWindows9x) {
    SystemParametersInfo(SPI_GETMENUUNDERLINES, 0, &globalData.m_bSysUnderlineKeyboardShortcuts, 0);
  }
  else {
    globalData.m_bSysUnderlineKeyboardShortcuts = TRUE;
  }
  globalData.m_bUnderlineKeyboardShortcuts = globalData.m_bSysUnderlineKeyboardShortcuts;
  globalData.m_bInSettingsChange = FALSE;
}
// Initialization code
int BCGPGLOBAL_DATA_set()
{
  // Detect the kind of OS:
  OSVERSIONINFO osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);
  globalData.bIsWindowsNT4 = ((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
      (osvi.dwMajorVersion < 5));
  globalData.bIsWindows9x = (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
  globalData.bIsOSAlphaBlendingSupport = (osvi.dwMajorVersion > 4) ||
      ((osvi.dwMajorVersion == 4) && (osvi.dwMinorVersion > 0));
  if (globalData.bIsWindowsNT4) {
    globalData.bIsOSAlphaBlendingSupport = FALSE;
  }
  globalData.bIsRemoteSession = FALSE;
  if ((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
      (osvi.dwMajorVersion >= 5)) {
    globalData.bIsRemoteSession = GetSystemMetrics(0x1000 /* SM_REMOTESESSION */);
  }
  globalData.bIsWindowsVista = (osvi.dwMajorVersion >= 6);
  globalData.bDisableAero = FALSE;
  globalData.m_bIsRibbonImageScale = TRUE;
  // Cached system values(updated in CWnd OnSysColorChange)
  globalData.clrBtnShadow = 0;
  globalData.clrBtnHilite = 0;
  globalData.clrWindow = 0;
  UpdateSysColors();
  UpdateSystemColorsXP(&globalData);
  globalData.m_hcurStretch = NULL;
  globalData.m_hcurStretchVert = NULL;
  globalData.m_hcurHand = NULL;
  globalData.m_hcurSizeAll = NULL;
  globalData.m_hiconTool = NULL;
  globalData.m_hiconLink = NULL;
  globalData.m_hiconColors = NULL;
  globalData.m_hcurMoveTab = NULL;
  globalData.m_hcurNoMoveTab = NULL;
  globalData.m_hcurSelectRow = NULL;
  globalData.m_bUseSystemFont = FALSE;
  globalData.m_bInSettingsChange = FALSE;
  UpdateFonts();
  OnSettingChange();
  globalData.m_bIsRTL = FALSE;
  //--
  // Small icon sizes:
  //--
  globalData.m_nDragFrameThiknessFloat = 4; // pixels
  globalData.m_nDragFrameThiknessDock = 3; // pixels
  globalData.m_nAutoHideToolBarSpacing = 14; // pixels
  globalData.m_nAutoHideToolBarMargin = 4; // pixels
  globalData.m_nCoveredMainWndClientAreaPercent = 50; // percents
  globalData.m_nMaxToolTipWidth = -1;
  globalData.m_bIsBlackHighContrast = FALSE;
  globalData.m_bIsWhiteHighContrast = FALSE;
  globalData.m_bUseBuiltIn32BitIcons = TRUE;
  globalData.m_bUseVisualManagerInBuiltInDialogs = FALSE;
  globalData.m_dwComCtlVersion = (DWORD) - 1;
  return 0;
}
BOOL SetMenuFont(LPLOGFONT lpLogFont, BOOL bHorz)
{
  ASSERT(lpLogFont != NULL);
  if (bHorz) {
    long lSavedWeight;
    // Create regular globalData.font:
    DeleteObject(globalData.fontRegular);
    globalData.fontRegular = CreateFontIndirect(lpLogFont);
    if (!globalData.fontRegular) {
      ASSERT(FALSE);
      return FALSE;
    }
    // Create underline globalData.font:
    lpLogFont->lfUnderline = TRUE;
    DeleteObject(globalData.fontUnderline);
    globalData.fontUnderline = CreateFontIndirect(lpLogFont);
    lpLogFont->lfUnderline = FALSE;
    // Create bold globalData.font(used in the default menu items):
    lSavedWeight = lpLogFont->lfWeight;
    lpLogFont->lfWeight = 700;
    DeleteObject(globalData.fontBold);
    globalData.fontBold = CreateFontIndirect(lpLogFont);
    lpLogFont->lfWeight = lSavedWeight; // Restore weight
    if (!globalData.fontBold) {
      ASSERT(FALSE);
      return FALSE;
    }
  }
  else { // Vertical globalData.font
    DeleteObject(globalData.fontVert);
    globalData.fontVert = CreateFontIndirect(lpLogFont);
    if (!globalData.fontVert) {
      ASSERT(FALSE);
      return FALSE;
    }
  }
  UpdateTextMetrics();
  return TRUE;
}
HBITMAP CreateDitherBitmap(HDC hDC)
{
  COLORREF clr;
  long patGray[8];
  int i;
  HBITMAP hbm;
  struct { // BITMAPINFO with 16 colors
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[16];
  }
  bmi;
  memset(&bmi, 0, sizeof(bmi));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = 8;
  bmi.bmiHeader.biHeight = 8;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 1;
  bmi.bmiHeader.biCompression = BI_RGB;
  clr = globalData.clrBtnFace;
  bmi.bmiColors[0].rgbBlue = GetBV(clr);
  bmi.bmiColors[0].rgbGreen = GetGV(clr);
  bmi.bmiColors[0].rgbRed = GetRV(clr);
  clr = _GetSysColor(COLOR_BTNHIGHLIGHT);
  bmi.bmiColors[1].rgbBlue = GetBV(clr);
  bmi.bmiColors[1].rgbGreen = GetGV(clr);
  bmi.bmiColors[1].rgbRed = GetRV(clr);
  // initialize the clrushes
  for (i = 0; i < 8; i++) {
    patGray[i] = (i & 1) ? 0xAAAA5555L : 0x5555AAAAL;
  }
  hbm = CreateDIBitmap(hDC, &bmi.bmiHeader, CBM_INIT,
      (LPBYTE)patGray, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS);
  return hbm;
}
void CleanUp()
{
  // cleanup globalData.fonts:
  DeleteObject(globalData.fontRegular);
  DeleteObject(globalData.fontBold);
  DeleteObject(globalData.fontUnderline);
  DeleteObject(globalData.fontVert);
  DeleteObject(globalData.fontVertCaption);
  DeleteObject(globalData.fontTooltip);
}
COLORREF GetColor(int nColor)
{
  switch (nColor) {
  case COLOR_BTNFACE:
    return globalData.clrBtnFace;
  case COLOR_BTNSHADOW:
    return globalData.clrBtnShadow;
  case COLOR_3DDKSHADOW:
    return globalData.clrBtnDkShadow;
  case COLOR_3DLIGHT:
    return globalData.clrBtnLight;
  case COLOR_BTNHIGHLIGHT:
    return globalData.clrBtnHilite;
  case COLOR_BTNTEXT:
    return globalData.clrBtnText;
  case COLOR_GRAYTEXT:
    return globalData.clrGrayedText;
  case COLOR_WINDOWFRAME:
    return globalData.clrWindowFrame;
  case COLOR_HIGHLIGHT:
    return globalData.clrHilite;
  case COLOR_HIGHLIGHTTEXT:
    return globalData.clrTextHilite;
  case COLOR_WINDOW:
    return globalData.clrWindow;
  case COLOR_WINDOWTEXT:
    return globalData.clrWindowText;
  case COLOR_CAPTIONTEXT:
    return globalData.clrCaptionText;
  case COLOR_MENUTEXT:
    return globalData.clrMenuText;
  case COLOR_ACTIVECAPTION:
    return globalData.clrActiveCaption;
  case COLOR_INACTIVECAPTION:
    return globalData.clrInactiveCaption;
  case COLOR_ACTIVEBORDER:
    return globalData.clrActiveBorder;
  case COLOR_INACTIVEBORDER:
    return globalData.clrInactiveBorder;
  case COLOR_INACTIVECAPTIONTEXT:
    return globalData.clrInactiveCaptionText;
  }
  return _GetSysColor(nColor);
}
HCURSOR GetHandCursor()
{
  globalData.m_hcurHand = LoadCursor(NULL, MAKEINTRESOURCE(32649)/*IDC_HAND*/);
  return globalData.m_hcurHand;
}
UINT GetRebarBandInfoSize()
{
  typedef struct BCGP_OLDREBARBANDINFO {
    UINT cbSize;
    UINT fMask;
    UINT fStyle;
    COLORREF clrFore;
    COLORREF clrBack;
    LPTSTR lpText;
    UINT cch;
    int iImage;
    HWND hwndChild;
    UINT cxMinChild;
    UINT cyMinChild;
    UINT cx;
    HBITMAP hbmBack;
    UINT wID;
#if(_WIN32_IE >= 0x0400)
    UINT cyChild;
    UINT cyMaxChild;
    UINT cyIntegral;
    UINT cxIdeal;
    LPARAM lParam;
    UINT cxHeader;
#endif
  } BCGP_OLDREBARBANDINFO;
  return(GetComCtlVersion() < MAKELONG(1, 6)) ? sizeof(BCGP_OLDREBARBANDINFO) : sizeof(REBARBANDINFO);
}
typedef struct VisualManager {
  BOOL m_bLook2000; // Single grippers
  int m_nMenuShadowDepth;
  BOOL m_bMenuFlatLook; // Menu item is always stil unpressed
  BOOL m_bShadowHighlightedImage;
  BOOL m_bEmbossDisabledImage;
  BOOL m_bFadeInactiveImage;
  BOOL m_bEnableToolbarButtonFill;
  BOOL m_bFrameMenuCheckedItems;
  int m_nVertMargin;
  int m_nHorzMargin;
  int m_nGroupVertOffset;
  int m_nGroupCaptionHeight;
  int m_nGroupCaptionHorzOffset;
  int m_nGroupCaptionVertOffset;
  int m_nTasksHorzOffset;
  int m_nTasksIconHorzOffset;
  int m_nTasksIconVertOffset;
  BOOL m_bActiveCaptions;
  COLORREF m_clrReportGroupText;
  COLORREF m_clrToolbarHighlightColor;
  COLORREF m_clrToolbarDisabledTextColor;
  BOOL m_bOfficeXPStyleMenus;
  int m_nMenuBorderSize;
  BOOL m_bAlwaysFillTab;
  BOOL m_b3DTabWideBorder;
  COLORREF m_clrMenuShadowBase;
  COLORREF m_clrPlannerWork;
  COLORREF m_clrPalennerLine;
  COLORREF m_clrPlanner;
  BOOL m_bPlannerBackItemToday;
  BOOL m_bPlannerBackItemSelected;
  BOOL m_bPlannerCaptionBackItemHeader;
  POINT m_ptRibbonMainImageOffset;
  BOOL m_bAutoDestroy;
} VisualManager;
int resimg_loadstr(int i, str_t s)
{
  int id = 0, n = 0;
  char strpath[256];
  str_t s1;
  char* pstr;
  char ch = 0;
  COLORREF clrTransparent = 0;
  img_t* im = 0;
  s1 = STRsplit(s, ',', " \t", &s), STRncpy(strpath, 256, s1);
  s1 = STRsplit(s, ',', " \t#", &s), clrTransparent = strtoul(s1.s, &pstr, 16);
  g_res.img = (img_t*)my_realloc(g_res.img, g_res.nimg * sizeof(img_t), (g_res.nimg + 1) * sizeof(img_t));
  memset(g_res.img + g_res.nimg, 0, sizeof(img_t));
  im = g_res.img + g_res.nimg;
  GetImage(im, strpath, clrTransparent);
  ++g_res.nimg;
  s1 = STRsplit(s, ',', " \t", &s);
  ch = tolower(*s1.s);
  switch (ch) {
  case 't':
    if (1) {
      SIZE sz;
      int i, row, col;
      s1 = STRsplit(s, ',', " \t", &s), sz.h = atoi(s1.s);
      s1 = STRsplit(s, ',', " \t", &s), sz.w = atoi(s1.s);
      s1 = STRsplit(s, ',', " \t", &s), id = atoi(s1.s);
      s1 = STRsplit(s, ',', " \t", &s), n = atoi(s1.s);
      sz.w = MIN(sz.w, im->w);
      sz.h = MIN(sz.h, im->h);
      row = im->h / sz.h, col = im->w / sz.w;
      n = MIN(n, row * col);
      if (n > 0) {
        g_res.ref = (img_t*)my_memins(g_res.ref, g_res.nref, id, n, sizeof(img_t));
        memset(g_res.ref + g_res.nref, 0, sizeof(img_t));
        for (i = 0; i < n; ++i) {
          IMINIT(g_res.ref + i + id, sz.h, sz.w, im->data + (i / col)*sz.h * im->s + (i % col)*sz.w * im->c, im->s, im->c, 1);
        }
        g_res.nref += n;
      }
    }
    break;
  default:
    assert(0);
    break;
  }
  return n;
}
int resload(const char* type, int id, const char* str)
{
  int n = 0;
  str_t s = STR1(str);
  s = STRtrim(s, "()");
  if (0 == strcmp(type, "font")) {
    n = font_setstr(id, s);
  }
  else if (0 == strcmp(type, "image")) {
    n = resimg_loadstr(id, s);
  }
  return n;
}

