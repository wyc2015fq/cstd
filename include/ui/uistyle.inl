enum BUTTON_STATE {
  ButtonsIsRegular,
  ButtonsIsPressed,
  ButtonsIsHighlighted,
};
enum SMARTDOCK_THEME {
  BCGP_SDT_DEFAULT = 0, // Visual Manager-specific theme
  BCGP_SDT_VS2005 = 1, // VS 2005-style
  BCGP_SDT_VS2008 = 2 // VS 2008-style
};
enum {
  VS2007_LunaBlue,
  VS2007_ObsidianBlack,
  VS2007_Aqua,
  VS2007_Silver
};
typedef struct ColorData {
  COLOR m_clrBackground;
  COLOR m_clrText;
  COLOR m_clrGradient;
  COLOR m_clrBorder;
  int m_nGradientAngle; // 0 - 360
} ColorData;
typedef struct BCGP_GRID_COLOR_DATA {
  COLOR m_clrBackground; // Grid background color
  COLOR m_clrText; // Grid foreground color
  COLOR m_clrHeader; // H foreground color
  ColorData m_EvenColors; // Even rows color
  ColorData m_OddColors; // Odd rows color
  ColorData m_GroupColors; // Group colors
  ColorData m_GroupSelColors; // Selected group colors
  ColorData m_SelColors; // Selected rows color
  ColorData m_HeaderColors; // Header item color
  ColorData m_HeaderSelColors; // Selected header item color
  ColorData m_LeftOffsetColors; // Left offset colors
  COLOR m_clrHorzLine; // Grid horizontal line text
  COLOR m_clrVertLine; // Grid vertical line text
  COLOR m_clrPreviewText; // Description text color
} BCGP_GRID_COLOR_DATA;
typedef struct CBCGPControlRenderer {
  UINT m_uiBmpResID;
  const char* m_strBmpResID;
  IRECT m_rectImage;
  IRECT m_rectCorners;
  IRECT m_rectSides;
  IRECT m_rectInter;
  COLOR m_clrTransparent;
  BOOL m_bPreMultiplyCheck;
  COLOR m_clrBase;
  COLOR m_clrTarget;
} CBCGPControlRenderer;
typedef struct CBCGPBitmapCache {
  int asdf;
} CBCGPBitmapCache;
typedef struct XRibbonContextCategory {
  CBCGPControlRenderer m_ctrlCaption;
  CBCGPControlRenderer m_ctrlBack;
  CBCGPControlRenderer m_ctrlTab;
  CBCGPControlRenderer m_ctrlBtnDefault;
  COLOR m_clrCaptionText;
  COLOR m_clrText;
  COLOR m_clrTextHighlighted;
  //CBCGPBitmapCache m_cacheBack;
  // CBCGPBitmapCache m_cacheBtnDefault;
} XRibbonContextCategory;
#if 0
void CleanUp()
{
  m_ctrlCaption.CleanUp();
  m_ctrlBack.CleanUp();
  m_ctrlTab.CleanUp();
  m_ctrlBtnDefault.CleanUp();
  m_cacheBack.Clear();
  m_cacheBtnDefault.Clear();
}
#endif
typedef struct uidraw_t {
  sys_t* sys;
#define UIGLOBAL_FUN_DEF(XXX) \
  XXX(int, OnUpdateSystemColors, (uidraw_t* gd)) \
  XXX(COLOR, GetWindowColor, (uidraw_t* gd)) \
  XXX(COLOR, GetToolbarDisabledTextColor, (uidraw_t* gd)) \
  XXX(int, OnDrawButtonBorder, (uidraw_t* gd, img_t* pDC, IRECT rect, int state)) \
  XXX(int, OnFillHighlightedArea, (img_t* im, IRECT rect, COLOR pBrush)) \
  XXX(BOOL, OnDrawCalculatorButton, (uidraw_t* gd, img_t* pDC, IRECT rect, int state, const char* strText)) \
  XXX(BOOL, OnDrawCalculatorDisplay, (uidraw_t* gd, img_t* pDC, IRECT rect, const char* strText)) \
  XXX(int, OnFillButtonInterior, (uidraw_t* gd, img_t* pDC, IRECT rect, int state))
#define XXX(type, name, args) type (*name) args;
  UIGLOBAL_FUN_DEF(XXX)
#undef XXX
  int m_nBitsPerPixel;
  BOOL m_bIsDrawOnGlass;
  BOOL m_bUseSystemFont; // Use system font for menu/toolbar/ribbons
  // solid brushes with convenient gray colors and system colors
  COLOR hbrBtnHilite, hbrBtnShadow;
  COLOR hbrWindow;
  // color values of system colors used for CToolBar
  COLOR clrBtnFace, clrBtnShadow, clrBtnHilite;
  COLOR clrBtnText, clrWindowFrame;
  COLOR clrBtnDkShadow, clrBtnLight;
  COLOR clrGrayedText;
  COLOR clrHilite;
  COLOR clrTextHilite;
  COLOR clrHotText;
  COLOR clrHotLinkText;
  COLOR clrBarWindow;
  COLOR clrBarFace;
  COLOR clrBarShadow, clrBarHilite;
  COLOR clrBarDkShadow, clrBarLight;
  COLOR clrBarText;
  COLOR clrWindow;
  COLOR clrWindowText;
  COLOR clrCaptionText;
  COLOR clrMenuText;
  COLOR clrActiveCaption;
  COLOR clrInactiveCaption;
  COLOR clrActiveCaptionGradient;
  COLOR clrInactiveCaptionGradient;
  COLOR clrInactiveCaptionText;
  COLOR clrActiveBorder;
  COLOR clrInactiveBorder;
  COLOR clrBlack;
  COLOR clrLight;
  // Library cursors:
  img_t* m_hcurStretch;
  img_t* m_hcurStretchVert;
  img_t* m_hcurHand;
  img_t* m_hcurSizeAll;
  img_t* m_hcurMoveTab;
  img_t* m_hcurNoMoveTab;
  img_t* m_hcurSelectRow;
  img_t* m_hiconTool;
  img_t* m_hiconLink;
  img_t* m_hiconColors;
  // Shell icon sizes:
  ISIZE m_sizeSmallIcon;
  // Toolbar and menu fonts:
  font_t* fontRegular;
  font_t* fontTooltip;
  font_t* fontBold;
  font_t* fontDefaultGUIBold;
  font_t* fontUnderline;
  font_t* fontDefault;
  font_t* fontDefaultGUIUnderline;
  font_t* fontVert;
  font_t* fontVertCaption;
  font_t* fontSmall;
  font_t* fontMarlett; // Standard Windows menu symbols
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
  IRECT m_rectVirtual;
  BOOL m_bMenuAnimation;
  BOOL m_bMenuFadeEffect;
  int m_nShellAutohideBars;
  BOOL m_bIsRTL;
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
  // BCGPVisualManager
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
  COLOR m_clrReportGroupText;
  BOOL m_bOfficeXPStyleMenus;
  int m_nMenuBorderSize;
  BOOL m_bAlwaysFillTab;
  BOOL m_b3DTabWideBorder;
  COLOR m_clrMenuShadowBase;
  COLOR m_clrPlannerWork;
  COLOR m_clrPalennerLine;
  COLOR m_clrPlanner;
  BOOL m_bPlannerBackItemToday;
  BOOL m_bPlannerBackItemSelected;
  BOOL m_bPlannerCaptionBackItemHeader;
  IPOINT m_ptRibbonMainImageOffset;
  BOOL m_bAutoDestroy;
  // BCGPVisualManagerXP
  COLOR m_clrGripperHorz;
  COLOR m_clrGripperVert;
  COLOR m_clrBarBkgnd; // Control bar background color (expect menu bar)
  COLOR m_clrMenuRarelyUsed;
  COLOR m_clrMenuLight; // Color of the light menu area
  COLOR m_clrInactiveTabText;
  COLOR m_clrTabBack;
  COLOR m_clrHighlight; // Highlighted toolbar/menu item color
  COLOR m_clrHighlightDn; // Highlighted and pressed toolbar item color
  COLOR m_clrHighlightChecked;
  COLOR m_clrPressedButtonBorder; // Used in derived classes
  COLOR m_clrGripper;
  COLOR m_clrSeparator;
  COLOR m_clrPaneBorder; // Status bar pane border
  COLOR m_clrMenuBorder; // Menu border
  COLOR m_clrMenuItemBorder; // Highlighted menu item border
  COLOR m_clrFloatToolBarBorder; // Floating toolbar border
  BOOL m_bConnectMenuToParent;
  BOOL m_bShdowDroppedDownMenuButton;
  BOOL m_bDrawLastTabLine;
  COLOR m_clrTaskPaneHotText;
  // CBCGPWinXPVisualManager : public CBCGPVisualManagerXP
  BOOL m_bOfficeStyleMenus;
  COLOR m_clrCalendarCaption;
  COLOR m_clrCalendarCaptionText;
  // CBCGPVisualManagerScenic : public CBCGPWinXPVisualManager
  font_t* m_AppCaptionFont;
  COLOR m_penSeparatorDark;
  COLOR m_penSeparatorLight;
  BOOL m_bLoaded;
  COLOR m_clrRibbonBarBkgnd;
  COLOR m_brRibbonBarBkgnd;
  COLOR m_clrRibbonBarGradientLight;
  COLOR m_clrRibbonBarGradientDark;
  COLOR m_clrRibbonPanelCaptionText;
  COLOR m_clrRibbonPanelCaptionTextHighlighted;
  COLOR m_clrRibbonEdit;
  COLOR m_clrRibbonEditDisabled;
  COLOR m_clrRibbonEditHighlighted;
  COLOR m_clrRibbonEditPressed;
  COLOR m_clrRibbonEditBorder;
  COLOR m_clrRibbonEditBorderDisabled;
  COLOR m_clrRibbonEditBorderHighlighted;
  COLOR m_clrRibbonEditBorderPressed;
  COLOR m_clrRibbonEditSelection;
  CBCGPControlRenderer m_ctrlRibbonComboBoxBtn;
  CBCGPControlRenderer m_ctrlMenuItemBack;
  img_t m_MenuItemMarkerC;
  img_t m_MenuItemMarkerR;
  CBCGPControlRenderer m_ctrlMenuHighlighted[2];
  CBCGPControlRenderer m_ctrlRibbonCaptionQA;
  CBCGPControlRenderer m_ctrlRibbonCategoryBack;
  CBCGPControlRenderer m_ctrlRibbonCategoryTab;
  CBCGPControlRenderer m_ctrlRibbonCategoryTabSep;
  CBCGPControlRenderer m_ctrlRibbonPanelBackSep;
  CBCGPControlRenderer m_ctrlRibbonMainPanel;
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
  img_t m_RibbonBtnLaunchIcon;
  CBCGPControlRenderer m_ctrlRibbonBtnMain;
  CBCGPControlRenderer m_ctrlRibbonSliderBtnPlus;
  CBCGPControlRenderer m_ctrlRibbonSliderBtnMinus;
  img_t m_RibbonBtnDefaultImage;
  CBCGPControlRenderer m_ctrlRibbonBtnDefault;
  CBCGPControlRenderer m_ctrlRibbonBtnDefaultIcon;
  CBCGPControlRenderer m_ctrlRibbonBtnDefaultQAT;
  CBCGPControlRenderer m_ctrlRibbonBtnStatusPane;
  CBCGPControlRenderer m_ctrlRibbonBtnPalette[3];
  CBCGPControlRenderer m_ctrlRibbonBorder_QAT;
  CBCGPBitmapCache m_cacheRibbonCategoryBack;
  CBCGPBitmapCache m_cacheRibbonBtnGroup_S;
  CBCGPBitmapCache m_cacheRibbonBtnGroup_F;
  CBCGPBitmapCache m_cacheRibbonBtnGroup_M;
  CBCGPBitmapCache m_cacheRibbonBtnGroup_L;
  CBCGPBitmapCache m_cacheRibbonBtnGroupMenu_F[2];
  CBCGPBitmapCache m_cacheRibbonBtnGroupMenu_M[2];
  CBCGPBitmapCache m_cacheRibbonBtnGroupMenu_L[2];
  CBCGPBitmapCache m_cacheRibbonBtnDefault;
  const char* m_strStylePrefix;
  BOOL m_bAutoFreeRes;
  //CMap<HWND, HWND, BOOL, BOOL> m_ActivateFlag;
  // VisualManager2003
  COLOR m_clrBarGradientDark;
  COLOR m_clrBarGradientLight;
  COLOR m_clrToolBarGradientDark;
  COLOR m_clrToolBarGradientLight;
  COLOR m_clrToolbarDisabled;
  COLOR m_clrToolBarGradientVertLight;
  COLOR m_clrToolBarGradientVertDark;
  COLOR m_clrCustomizeButtonGradientDark;
  COLOR m_clrCustomizeButtonGradientLight;
  COLOR m_clrToolBarBottomLine;
  COLOR m_penBottomLine;
  COLOR m_colorToolBarCornerTop;
  COLOR m_colorToolBarCornerBottom;
  COLOR m_clrHighlightMenuItem;
  COLOR m_clrHighlightGradientLight;
  COLOR m_clrHighlightGradientDark;
  COLOR m_clrHighlightDnGradientLight;
  COLOR m_clrHighlightDnGradientDark;
  COLOR m_clrHighlightCheckedGradientLight;
  COLOR m_clrHighlightCheckedGradientDark;
  COLOR m_clrCaptionBarGradientLight;
  COLOR m_clrCaptionBarGradientDark;
  COLOR m_brTearOffCaption;
  COLOR m_brFace;
  COLOR m_clrTaskPaneGradientDark;
  COLOR m_clrTaskPaneGradientLight;
  COLOR m_clrTaskPaneGroupCaptionDark;
  COLOR m_clrTaskPaneGroupCaptionLight;
  COLOR m_clrTaskPaneGroupCaptionSpecDark;
  COLOR m_clrTaskPaneGroupCaptionSpecLight;
  COLOR m_clrTaskPaneGroupAreaLight;
  COLOR m_clrTaskPaneGroupAreaDark;
  COLOR m_clrTaskPaneGroupAreaSpecLight;
  COLOR m_clrTaskPaneGroupAreaSpecDark;
  COLOR m_clrTaskPaneGroupBorder;
  COLOR m_penTaskPaneGroupBorder;
  COLOR m_penGridExpandBoxLight;
  COLOR m_penGridExpandBoxDark;
  COLOR m_clrPlannerTodayFill;
  COLOR m_penPlannerTodayLine;
  BOOL m_bIsStandardWinXPTheme;
  BOOL m_bUseGlobalTheme;
  BOOL m_bStatusBarOfficeXPLook;
  BOOL m_bDefaultWinXPColors;
  // CBCGPVisualManagerVS2005 : public CBCGPVisualManager2003
  COLOR m_colorActiveTabBorder;
  COLOR m_penActiveTabBorder;
  COLOR m_brMenuButtonDroppedDown;
  COLOR m_brMenuItemCheckedHighlight;
  BOOL m_bRoundedAutohideButtons;
  //WinXpTheme m_CurrAppTheme;
  // CBCGPVisualManagerVS2008 : public CBCGPVisualManagerVS2005
  BOOL m_bOSColors;
  // CBCGPVisualManager2007 : public CBCGPVisualManager2003
  ISIZE m_szNcBtnSize[2];
  BOOL m_bNcTextCenter;
  img_t m_ToolBarGripper;
  img_t m_ToolBarTear;
  CBCGPControlRenderer m_ctrlToolBarBorder;
  CBCGPControlRenderer m_ctrlStatusBarBack;
  CBCGPControlRenderer m_ctrlStatusBarBack_Ext;
  img_t m_StatusBarPaneBorder;
  img_t m_StatusBarSizeBox;
  CBCGPControlRenderer m_SysBtnBack[2];
  img_t m_SysBtnClose[2];
  img_t m_SysBtnRestore[2];
  img_t m_SysBtnMaximize[2];
  img_t m_SysBtnMinimize[2];
  img_t m_SysBtnHelp[2];
  COLOR m_clrAppCaptionActiveStart;
  COLOR m_clrAppCaptionActiveFinish;
  COLOR m_clrAppCaptionInactiveStart;
  COLOR m_clrAppCaptionInactiveFinish;
  COLOR m_clrAppCaptionActiveText;
  COLOR m_clrAppCaptionInactiveText;
  COLOR m_clrAppCaptionActiveTitleText;
  COLOR m_clrAppCaptionInactiveTitleText;
  COLOR m_clrMainClientArea;
  COLOR m_brMainClientArea;
  COLOR m_clrMenuBarGradientLight;
  COLOR m_clrMenuBarGradientDark;
  COLOR m_clrMenuBarGradientVertLight;
  COLOR m_clrMenuBarGradientVertDark;
  COLOR m_clrMenuBarBtnText;
  COLOR m_clrMenuBarBtnTextHighlighted;
  COLOR m_clrMenuBarBtnTextDisabled;
  COLOR m_clrToolBarBtnText;
  COLOR m_clrToolBarBtnTextHighlighted;
  COLOR m_clrToolBarBtnTextDisabled;
  COLOR m_clrMenuText;
  COLOR m_clrMenuTextHighlighted;
  COLOR m_clrMenuTextDisabled;
  COLOR m_clrStatusBarText;
  COLOR m_clrStatusBarTextDisabled;
  COLOR m_clrExtenedStatusBarTextDisabled;
  COLOR m_clrEditBorder;
  COLOR m_clrEditBorderDisabled;
  COLOR m_clrEditBorderHighlighted;
  COLOR m_clrEditSelection;
  COLOR m_clrComboBorder;
  COLOR m_clrComboBorderDisabled;
  COLOR m_clrComboBorderPressed;
  COLOR m_clrComboBorderHighlighted;
  COLOR m_clrComboBtnStart;
  COLOR m_clrComboBtnFinish;
  COLOR m_clrComboBtnBorder;
  COLOR m_clrComboBtnDisabledStart;
  COLOR m_clrComboBtnDisabledFinish;
  COLOR m_clrComboBtnBorderDisabled;
  COLOR m_clrComboBtnPressedStart;
  COLOR m_clrComboBtnPressedFinish;
  COLOR m_clrComboBtnBorderPressed;
  COLOR m_clrComboBtnHighlightedStart;
  COLOR m_clrComboBtnHighlightedFinish;
  COLOR m_clrComboBtnBorderHighlighted;
  COLOR m_clrComboSelection;
  CBCGPControlRenderer m_ctrlComboBoxBtn;
  COLOR m_clrHeaderNormalStart;
  COLOR m_clrHeaderNormalFinish;
  COLOR m_clrHeaderNormalBorder;
  COLOR m_clrHeaderHighlightedStart;
  COLOR m_clrHeaderHighlightedFinish;
  COLOR m_clrHeaderHighlightedBorder;
  COLOR m_clrHeaderPressedStart;
  COLOR m_clrHeaderPressedFinish;
  COLOR m_clrHeaderPressedBorder;
  COLOR m_clrBarCaption;
  COLOR m_clrMiniFrameCaption;
  COLOR m_clrSeparator1;
  COLOR m_clrSeparator2;
  COLOR m_penSeparator2;
  COLOR m_clrGroupText;
  COLOR m_brGroupBackground;
  COLOR m_clrCaptionBarText;
  CBCGPControlRenderer m_ctrlMainBorder;
  CBCGPControlRenderer m_ctrlMDIChildBorder;
  CBCGPControlRenderer m_ctrlDialogBorder;
  CBCGPControlRenderer m_ctrlMainBorderCaption;
  CBCGPControlRenderer m_ctrlPopupBorder;
  CBCGPControlRenderer m_ctrlPopupResizeBar;
  img_t m_PopupResizeBar_HV;
  img_t m_PopupResizeBar_HVT;
  img_t m_PopupResizeBar_V;
  CBCGPControlRenderer m_ctrlMenuBarBtn;
  CBCGPControlRenderer m_ctrlMenuItemShowAll;
  CBCGPControlRenderer m_ctrlMenuButtonBorder;
  CBCGPControlRenderer m_ctrlMenuScrollBtn[2];
  CBCGPControlRenderer m_ctrlToolBarBtn;
#ifndef BCGP_EXCLUDE_TASK_PANE
  COLOR m_clrTaskPaneGroupCaptionHighDark;
  COLOR m_clrTaskPaneGroupCaptionHighLight;
  COLOR m_clrTaskPaneGroupCaptionHighSpecDark;
  COLOR m_clrTaskPaneGroupCaptionHighSpecLight;
  COLOR m_clrTaskPaneGroupCaptionTextSpec;
  COLOR m_clrTaskPaneGroupCaptionTextHighSpec;
  COLOR m_clrTaskPaneGroupCaptionText;
  COLOR m_clrTaskPaneGroupCaptionTextHigh;
  CBCGPControlRenderer m_ctrlTaskScrollBtn;
#endif
  CBCGPControlRenderer m_ctrlTab3D[2];
  COLOR m_clrTab3DFace;
  COLOR m_clrTab3DBlack;
  COLOR m_clrTab3DDark;
  COLOR m_clrTab3DDarkShadow;
  COLOR m_clrTab3DLight;
  COLOR m_clrTab3DHighlight;
  CBCGPControlRenderer m_ctrlTabFlat[2];
  COLOR m_clrTabFlatFace;
  COLOR m_clrTabFlatBlack;
  COLOR m_clrTabFlatDark;
  COLOR m_clrTabFlatDarkShadow;
  COLOR m_clrTabFlatLight;
  COLOR m_clrTabFlatHighlight;
  COLOR m_penTabFlatInner[2];
  COLOR m_penTabFlatOuter[2];
  COLOR m_clrTabTextActive;
  COLOR m_clrTabTextInactive;
  CBCGPControlRenderer m_ctrlOutlookWndBar;
  CBCGPControlRenderer m_ctrlOutlookWndPageBtn;
  COLOR m_clrOutlookPageTextNormal;
  COLOR m_clrOutlookPageTextHighlighted;
  COLOR m_clrOutlookPageTextPressed;
  COLOR m_clrOutlookCaptionTextNormal;
  CBCGPControlRenderer m_ctrlScrollBar_Back[2][2];
  CBCGPControlRenderer m_ctrlScrollBar_Item[2][2];
  CBCGPControlRenderer m_ctrlScrollBar_ThumbBack[2][2];
  CBCGPControlRenderer m_ctrlScrollBar_ThumbIcon[2][2];
  CBCGPControlRenderer m_ctrlSliderThumb[6];
  CBCGPControlRenderer m_ctrlRibbonCaptionQA_Glass;
  COLOR m_clrRibbonCategoryText;
  COLOR m_clrRibbonCategoryTextHighlighted;
  COLOR m_clrRibbonCategoryTextDisabled;
  CBCGPControlRenderer m_ctrlRibbonCategoryBtnPage[2];
  COLOR m_clrRibbonPanelText;
  COLOR m_clrRibbonPanelTextHighlighted;
  CBCGPControlRenderer m_ctrlRibbonPanelBack_T;
  CBCGPControlRenderer m_ctrlRibbonPanelBack_B;
  img_t m_RibbonPanelSeparator;
  CBCGPControlRenderer m_ctrlRibbonPanelQAT;
  CBCGPControlRenderer m_ctrlRibbonMainPanelBorder;
  CBCGPControlRenderer m_RibbonBtnMain;
  CBCGPControlRenderer m_ctrlRibbonBtnDefaultQATIcon;
  CBCGPControlRenderer m_ctrlRibbonBtnCheck;
  CBCGPControlRenderer m_ctrlRibbonBtnRadio;
  CBCGPControlRenderer m_ctrlRibbonBtnPush;
  CBCGPControlRenderer m_ctrlRibbonBtnGroup;
  CBCGPControlRenderer m_ctrlRibbonSliderThumb;
  CBCGPControlRenderer m_ctrlRibbonSliderThumbA[5];
  CBCGPControlRenderer m_ctrlRibbonProgressBack;
  CBCGPControlRenderer m_ctrlRibbonProgressNormal;
  CBCGPControlRenderer m_ctrlRibbonProgressNormalExt;
  CBCGPControlRenderer m_ctrlRibbonProgressInfinity;
  CBCGPControlRenderer m_ctrlRibbonBorder_Floaty;
  CBCGPControlRenderer m_ctrlRibbonKeyTip;
  COLOR m_clrRibbonKeyTipTextNormal;
  COLOR m_clrRibbonKeyTipTextDisabled;
  COLOR m_clrRibbonComboBtnStart;
  COLOR m_clrRibbonComboBtnFinish;
  COLOR m_clrRibbonComboBtnBorder;
  COLOR m_clrRibbonComboBtnDisabledStart;
  COLOR m_clrRibbonComboBtnDisabledFinish;
  COLOR m_clrRibbonComboBtnBorderDisabled;
  COLOR m_clrRibbonComboBtnPressedStart;
  COLOR m_clrRibbonComboBtnPressedFinish;
  COLOR m_clrRibbonComboBtnBorderPressed;
  COLOR m_clrRibbonComboBtnHighlightedStart;
  COLOR m_clrRibbonComboBtnHighlightedFinish;
  COLOR m_clrRibbonComboBtnBorderHighlighted;
  CBCGPControlRenderer m_ctrlRibbonContextPanelBack_T;
  CBCGPControlRenderer m_ctrlRibbonContextPanelBack_B;
  COLOR m_clrRibbonContextPanelText;
  COLOR m_clrRibbonContextPanelTextHighlighted;
  COLOR m_clrRibbonContextPanelCaptionText;
  COLOR m_clrRibbonContextPanelCaptionTextHighlighted;
  //CBCGPBitmapCache m_cacheRibbonContextPanelBack_T;
  //CBCGPBitmapCache m_cacheRibbonContextPanelBack_B;
  CBCGPControlRenderer m_ctrlRibbonContextSeparator;
#define BCGPRibbonCategoryColorCount 7
  XRibbonContextCategory m_ctrlRibbonContextCategory[BCGPRibbonCategoryColorCount];
  int m_Style;
  COLOR m_clrBase;
  COLOR m_clrTarget;
  CBCGPBitmapCache m_cacheRibbonPanelBack_T;
  CBCGPBitmapCache m_cacheRibbonPanelBack_B;
  int m_nType;
  BOOL m_bPlannerBlack;
  COLOR m_clrPlannerTodayCaption[4];
  COLOR m_clrPlannerTodayBorder;
  COLOR m_clrPlannerNcArea;
  COLOR m_clrPlannerNcLine;
  COLOR m_clrPlannerNcText;
  COLOR m_clrGridHeaderNormalStart;
  COLOR m_clrGridHeaderNormalFinish;
  COLOR m_clrGridHeaderNormalBorder;
  COLOR m_clrGridHeaderPressedStart;
  COLOR m_clrGridHeaderPressedFinish;
  COLOR m_clrGridHeaderPressedBorder;
  COLOR m_clrGridHeaderAllNormalBackStart;
  COLOR m_clrGridHeaderAllNormalBackFinish;
  COLOR m_clrGridHeaderAllPressedBackStart;
  COLOR m_clrGridHeaderAllPressedBackFinish;
  COLOR m_clrGridHeaderAllNormalBorderHighlighted;
  COLOR m_clrGridHeaderAllNormalBorderShadow;
  COLOR m_clrGridHeaderAllPressedBorderHighlighted;
  COLOR m_clrGridHeaderAllPressedBorderShadow;
  COLOR m_clrGridHeaderAllNormalSignStart;
  COLOR m_clrGridHeaderAllNormalSignFinish;
  COLOR m_clrGridHeaderAllPressedSignStart;
  COLOR m_clrGridHeaderAllPressedSignFinish;
  COLOR m_clrGridSeparator;
  COLOR m_penGridSeparator;
  COLOR m_clrGridLeftOffset;
  COLOR m_clrGridGroupLine;
  COLOR m_clrGridGroupSubLine;
  COLOR m_clrPopupGradientLight;
  COLOR m_clrPopupGradientDark;
  //CBCGPToolTipParams m_ToolTipParams;
  BOOL m_bToolTipParams;
  COLOR m_clrRibbonHyperlinkInactive;
  COLOR m_clrRibbonHyperlinkActive;
  COLOR m_clrRibbonStatusbarHyperlinkInactive;
  COLOR m_clrRibbonStatusbarHyperlinkActive;
  COLOR m_clrDlgBackground;
  COLOR m_brDlgBackground;
#ifndef BCGP_EXCLUDE_GRID_CTRL
  BCGP_GRID_COLOR_DATA m_GridColors;
#endif
  //BOOL IsBeta1 () const { return m_nType == 10; }
  //BOOL IsBeta () const { return m_nType < 20; }
  //CMap<HWND, BOOL> m_ActivateFlag;
  // CBCGPVisualManagerCarbon : public CBCGPVisualManager2007
  CBCGPControlRenderer m_ctrlBaseMainBorderCaption;
  CBCGPControlRenderer m_ctrlBaseMainBorderL;
  CBCGPControlRenderer m_ctrlBaseMainBorderR;
  CBCGPControlRenderer m_ctrlBaseMainBorderTB;
  CBCGPControlRenderer m_ctrlBaseMainBorder;
  CBCGPControlRenderer m_ctrlBaseMDIChildBorder;
  CBCGPControlRenderer m_ctrlBaseMiniBorderCaption;
  CBCGPControlRenderer m_ctrlBaseMiniBorderTB;
  CBCGPControlRenderer m_ctrlBaseMiniBorder;
  CBCGPControlRenderer m_ctrlBaseDialogBorder;
  CBCGPControlRenderer m_ctrlBaseSysBtnBack[2];
  CBCGPControlRenderer m_ctrlBaseSysBtnBackC[2];
  CBCGPControlRenderer m_ctrlBaseStatusBarBack;
  CBCGPControlRenderer m_ctrlBaseStatusBarBack_Ext;
  CBCGPControlRenderer m_ctrlBaseMenuBarBtn;
  CBCGPControlRenderer m_ctrlBasePopupBorder;
  img_t m_BaseToolBarGripper;
  img_t m_BaseToolBarTear;
  img_t m_BaseStatusBarPaneBorder;
  img_t m_BaseStatusBarSizeBox;
  CBCGPControlRenderer m_ctrlBaseToolBarBtn;
  CBCGPControlRenderer m_ctrlBaseMenuItemBack;
  img_t m_BaseMenuItemMarkerC;
  img_t m_BaseMenuItemMarkerR;
  CBCGPControlRenderer m_ctrlBaseRibbonBtnPush;
  COLOR m_clrBaseBarFace;
  COLOR m_clrBaseActiveCaption;
  COLOR m_clrBaseInactiveCaption;
  COLOR m_clrBaseBarShadow;
  COLOR m_clrBaseBarDkShadow;
  COLOR m_clrBaseBarLight;
  COLOR m_clrBaseFloatToolBarBorder;
  COLOR m_clrBaseHighlightGradientDark;
  COLOR m_clrBaseHighlightGradientLight;
  COLOR m_clrBaseHighlightDnGradientDark;
  COLOR m_clrBaseHighlightDnGradientLight;
  COLOR m_clrBaseHighlightCheckedGradientDark;
  COLOR m_clrBaseHighlightCheckedGradientLight;
  COLOR m_clrBasePressedButtonBorder;
  COLOR m_clrBaseDlgBackground;
  //CBCGPToolTipParams m_BaseToolTipParams;
  COLOR m_clrBaseMainClientArea;
  COLOR m_clrBaseMenuLight;
  COLOR m_clrBaseMenuRarelyUsed;
  COLOR m_clrBaseMenuBorder;
  COLOR m_clrBaseMenuItemBorder;
  COLOR m_clrBaseMenuGutterLight;
  COLOR m_clrBaseMenuGutterDark;
  COLOR m_clrBaseSeparator1;
  COLOR m_clrBaseSeparator2;
  COLOR m_clrBaseBarBkgnd;
  COLOR m_clrBaseBarGradientLight;
  COLOR m_clrBaseBarGradientDark;
  COLOR m_clrBaseToolBarGradientLight;
  COLOR m_clrBaseToolBarGradientDark;
  COLOR m_clrBaseToolbarDisabled;
  COLOR m_clrBaseToolBarGradientVertLight;
  COLOR m_clrBaseToolBarGradientVertDark;
  COLOR m_clrBaseCustomizeButtonGradientLight;
  COLOR m_clrBaseCustomizeButtonGradientDark;
  COLOR m_clrBaseToolBarBottomLine;
  COLOR m_clrBaseSeparatorLight;
  COLOR m_clrBaseSeparatorDark;
  COLOR m_clrBaseMenuBarGradientLight;
  COLOR m_clrBaseMenuBarGradientDark;
  COLOR m_clrBaseMenuBarGradientVertLight;
  COLOR m_clrBaseMenuBarGradientVertDark;
  COLOR m_clrBaseCaptionBarGradientLight;
  COLOR m_clrBaseCaptionBarGradientDark;
  COLOR m_clrBaseTab3DFace;
  COLOR m_clrBaseTab3DBlack;
  COLOR m_clrBaseTab3DDark;
  COLOR m_clrBaseTab3DDarkShadow;
  COLOR m_clrBaseTab3DLight;
  COLOR m_clrBaseTab3DHighlight;
  COLOR m_clrBaseTabFlatFace;
  COLOR m_clrBaseTabFlatBlack;
  COLOR m_clrBaseTabFlatDark;
  COLOR m_clrBaseTabFlatDarkShadow;
  COLOR m_clrBaseTabFlatLight;
  COLOR m_clrBaseTabFlatHighlight;
  COLOR m_clrBasePopupGradientLight;
  COLOR m_clrBasePopupGradientDark;
  CBCGPControlRenderer m_ctrlMainBorderL;
  CBCGPControlRenderer m_ctrlMainBorderR;
  CBCGPControlRenderer m_ctrlMainBorderTB;
  CBCGPControlRenderer m_ctrlMiniBorderCaption;
  CBCGPControlRenderer m_ctrlMiniBorderTB;
  CBCGPControlRenderer m_ctrlMiniBorder;
  CBCGPControlRenderer m_SysBtnBackC[2];
  CBCGPControlRenderer m_SysBtnBackH[2];
  CBCGPControlRenderer m_SysBtnBackCH[2];
  CBCGPControlRenderer m_ctrlMiniSysBtn;
  COLOR m_clrMenuGutterLight;
  COLOR m_clrMenuGutterDark;
  COLOR m_clrSeparatorLight;
  COLOR m_clrSeparatorDark;
  COLOR m_clrLinkText;
  COLOR m_clrLinkHotText;
  COLOR m_clrActionText;
  BOOL m_bUpdateColors;
  //XLinkImagesArray m_arLinkImages;
  //XLinkColorsArray m_arLinkColors;
  double m_dblHue;
  double m_dblSaturation;
} uidraw_t;

