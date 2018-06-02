
//
// BCGPVisualManagerScenic.cpp: implementation of the CBCGPVisualManagerScenic class.
//
//////////////////////////////////////////////////////////////////////

#include "bcgcbpro.h"
#include "bcgglobals.h"
#include "BCGCBProVer.h"
#include "BCGPVisualManagerScenic.h"
#include "BCGPDrawManager.h"
#include "BCGPTagManager.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPGlobalUtils.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPFrameWnd.h"
#include "CustomizeButton.h"
#include "BCGPToolbarComboBoxButton.h"

#ifndef BCGP_EXCLUDE_RIBBON
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonQuickAccessToolbar.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonMainPanel.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonStatusBar.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPRibbonProgressBar.h"
#include "BCGPRibbonHyperlink.h"
#include "BCGPRibbonSlider.h"
#endif

IMPLEMENT_DYNCREATE (CBCGPVisualManagerScenic, CBCGPWinXPVisualManager)

#define RT_BCG_STYLE_XML _T("STYLE_XML")

CString	CBCGPVisualManagerScenic::m_strStylePrefix;
HINSTANCE CBCGPVisualManagerScenic::m_hinstRes = NULL;
BOOL CBCGPVisualManagerScenic::m_bAutoFreeRes = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManagerScenic::CBCGPVisualManagerScenic()
 : m_bLoaded(FALSE)
{
}

CBCGPVisualManagerScenic::~CBCGPVisualManagerScenic()
{
}


CString CBCGPVisualManagerScenic::MakeResourceID(LPCTSTR lpszID)
{
	CString strResID(lpszID);
	ASSERT(!strResID.IsEmpty());

	if (!m_strStylePrefix.IsEmpty())
	{
		strResID = m_strStylePrefix + strResID;
	}

	return strResID;
}

CString CBCGPVisualManagerScenic::GetStyleResourceID()
{
	CString strResID (_T("IDX_STYLE"));

#if !defined _AFXDLL || defined _BCGCBPRO_STATIC_

	strResID = _T("SCENIC_") + strResID;

#endif

	return strResID;
};

void CBCGPVisualManagerScenic::SetResourceHandle(HINSTANCE hinstRes)
{
	m_bAutoFreeRes = FALSE;

	if (m_hinstRes != hinstRes)
	{
		m_hinstRes = hinstRes;

		if (CBCGPVisualManager::GetInstance()->IsKindOf(RUNTIME_CLASS(CBCGPVisualManagerScenic)))
		{
			CBCGPVisualManager::GetInstance()->OnUpdateSystemColors();
		}
	}
}

BOOL CBCGPVisualManagerScenic::SetStyle(LPCTSTR lpszPath)
{
	if (m_hinstRes > (HINSTANCE) 32)
	{
		return TRUE;
	}

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_

	CString strVer;
	strVer.Format (_T("%d%d"), _BCGCBPRO_VERSION_MAJOR, _BCGCBPRO_VERSION_MINOR);

	CString strStyleDLLName = _T("BCGPStyleScenic") + strVer + _T(".dll");

	CString strStyleDLLPath;

	if (lpszPath != NULL && _tcslen (lpszPath) > 0)
	{
		strStyleDLLPath = lpszPath;

		if (strStyleDLLPath [strStyleDLLPath.GetLength () - 1] != _T('\\'))
		{
			strStyleDLLPath += _T('\\');
		}

		strStyleDLLPath += strStyleDLLName;
	}
	else
	{
		strStyleDLLPath = strStyleDLLName;
	}

	CleanStyle ();

	HINSTANCE hinstRes = LoadLibrary (strStyleDLLPath);

	if (hinstRes <= (HINSTANCE) 32)
	{
		TRACE(_T("Cannot load Style DLL: %s\r\n"), strStyleDLLPath);
		ASSERT (FALSE);
		return FALSE;
	}

	SetResourceHandle (hinstRes);
	m_bAutoFreeRes = TRUE;

#else

	UNREFERENCED_PARAMETER (lpszPath);

	CString strStyle (GetStyleResourceID ());
	HINSTANCE hinstRes = AfxFindResourceHandle (strStyle, RT_BCG_STYLE_XML);

	if (::FindResource(hinstRes, strStyle, RT_BCG_STYLE_XML) == NULL)
	{
		TRACE(_T("Cannot load Style: %s\r\n"), strStyle);
		ASSERT (FALSE);
		return FALSE;
	}

	CleanStyle ();

	SetResourceHandle (hinstRes);

#endif

	return TRUE;
}

void CBCGPVisualManagerScenic::CleanStyle()
{
	if (m_bAutoFreeRes && m_hinstRes >(HINSTANCE) 32)
	{
		::FreeLibrary(m_hinstRes);
	}

	m_hinstRes = NULL;
	m_strStylePrefix.Empty();
}

void CBCGPVisualManagerScenic::DrawSeparator (CDC* pDC, const CRect& rect, CPen& pen1, CPen& pen2, BOOL bHorz)
{
    CRect rect1 (rect);
    CRect rect2;

    if (bHorz)
    {
        rect1.top += rect.Height () / 2 - 1;
        rect1.bottom = rect1.top;
        rect2 = rect1;
        rect2.OffsetRect (0, 1);
    }
    else
    {
        rect1.left += rect.Width () / 2 - 1;
        rect1.right = rect1.left;
        rect2 = rect1;
        rect2.OffsetRect (1, 0);
    }

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);

		LOGPEN logpen;

		pen1.GetLogPen (&logpen);
		dm.DrawLine (rect1.left, rect1.top, rect1.right, rect1.bottom, logpen.lopnColor);

		pen2.GetLogPen (&logpen);
		dm.DrawLine (rect2.left, rect2.top, rect2.right, rect2.bottom, logpen.lopnColor);
	}
	else
	{
		CPen* pOldPen = pDC->SelectObject (&pen1);
		pDC->MoveTo (rect1.TopLeft ());
		pDC->LineTo (rect1.BottomRight ());

		pDC->SelectObject (&pen2);
		pDC->MoveTo (rect2.TopLeft ());
		pDC->LineTo (rect2.BottomRight ());

		pDC->SelectObject (pOldPen);
	}
}

void CBCGPVisualManagerScenic::DrawNcBtn(CDC* pDC, const CRect& rect, UINT nButton, 
										BCGBUTTON_STATE state, BOOL bSmall, 
										BOOL bActive, BOOL bMDI/* = FALSE*/)
{
	ASSERT_VALID(pDC);

	if (m_hThemeWindow == NULL)
	{
		UINT nState = 0;

		switch (nButton)
		{
		case SC_CLOSE:
			nState = DFCS_CAPTIONCLOSE;
			break;

		case SC_MINIMIZE:
			nState = DFCS_CAPTIONMIN;
			break;

		case SC_MAXIMIZE:
			nState = DFCS_CAPTIONMAX;
			break;

		case SC_RESTORE:
			nState = DFCS_CAPTIONRESTORE;
			break;

		case SC_CONTEXTHELP:
			nState = DFCS_CAPTIONHELP;
			break;

		default:
			return;
		}

		if (!bActive)
		{
			nState |= DFCS_INACTIVE;
		}

		if (state != ButtonsIsRegular)
		{
			nState |= state == ButtonsIsHighlighted ? 0x1000/*DFCS_HOT*/ : DFCS_PUSHED;
		}

		CRect rt (rect);
		pDC->DrawFrameControl (rt, DFC_CAPTION, nState);

		return;
	}

    int nPart = 0;
    int nState = 0;
	if (nButton == SC_CLOSE)
	{
		if (bMDI)
		{
            nPart = /*WP_MDICLOSEBUTTON*/20;
		}
		else
		{
			nPart = bSmall ? /*WP_SMALLCLOSEBUTTON*/19 : /*WP_CLOSEBUTTON*/18;
		}

		nState = bActive ? /*CBS_NORMAL*/1 : 5;
		if (state != ButtonsIsRegular)
		{
			nState = state == ButtonsIsHighlighted ? /*CBS_HOT*/2 : /*CBS_PUSHED*/3;
		}
	}
	else if (nButton == SC_MINIMIZE)
	{
		if (bMDI)
		{
            nPart = /*WP_MDIMINBUTTON*/16;
		}
		else if (!bSmall)
		{
			nPart = /*WP_MINBUTTON*/15;
		}

        nState = bActive ? /*MINBS_NORMAL*/1 : 5;
		if (state != ButtonsIsRegular)
		{
			nState = state == ButtonsIsHighlighted ? /*MINBS_HOT*/2 : /*MINBS_PUSHED*/3;
		}
	}
	else if (nButton == SC_MAXIMIZE)
	{
		if (!bMDI && !bSmall)
		{
            nPart = /*WP_MAXBUTTON*/17;
		}

        nState = bActive ? /*MAXBS_NORMAL*/1 : 5;
		if (state != ButtonsIsRegular)
		{
			nState = state == ButtonsIsHighlighted ? /*MAXBS_HOT*/2 : /*MAXBS_PUSHED*/3;
		}
	}
	else if (nButton == SC_RESTORE)
	{
		if (bMDI)
		{
            nPart = /*WP_MDIRESTOREBUTTON*/22;
		}
		else
		{
			nPart = /*WP_RESTOREBUTTON*/21;
		}

        nState = bActive ? /*RBS_NORMAL*/1 : 5;
		if (state != ButtonsIsRegular)
		{
			nState = state == ButtonsIsHighlighted ? /*RBS_HOT*/2 : /*RBS_PUSHED*/3;
		}
	}
	else if (nButton == SC_CONTEXTHELP)
	{
		if (bMDI)
		{
            nPart = /*WP_MDIHELPBUTTON*/24;
		}
		else if (!bSmall)
		{
			nPart = /*WP_HELPBUTTON*/23;
		}

        nState = bActive ? /*HBS_NORMAL*/1 : /*HBS_DISABLED*/4;
		if (state != ButtonsIsRegular)
		{
			nState = state == ButtonsIsHighlighted ? /*HBS_HOT*/2 : /*HBS_PUSHED*/3;
		}
	}

    if (nPart == 0)
	{
		return;
	}

	(*m_pfDrawThemeBackground)(m_hThemeWindow, pDC->GetSafeHdc(), nPart, nState, &rect, 0);
}

void CBCGPVisualManagerScenic::DrawNcText (CDC* pDC, CRect& rect, 
										 const CString& strTitle, 
										 BOOL bActive, BOOL bIsRTL, 
										 BOOL bTextCenter,
										 BOOL bGlass/* = FALSE*/, int nGlassGlowSize/* = 0*/, 
										 COLORREF clrGlassText/* = (COLORREF)-1*/)
{
	if (strTitle.IsEmpty () || rect.right <= rect.left)
	{
		return;
	}

	ASSERT_VALID (pDC);

	int nOldMode = pDC->SetBkMode (TRANSPARENT);
	COLORREF clrOldText = pDC->GetTextColor ();

	DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER |
		(bIsRTL ? DT_RTLREADING : 0);

	COLORREF clrText = bActive 
		? globalData.clrCaptionText
		: globalData.clrInactiveCaptionText;

	int widthFull = rect.Width ();
	int width = pDC->GetTextExtent (strTitle).cx;

	if (bTextCenter && width < widthFull)
	{
		rect.left += (widthFull - width) / 2;
	}

	rect.right = min (rect.left + width, rect.right);

	if (rect.right > rect.left)
	{
		if (bGlass)
		{
			DrawTextOnGlass (pDC, strTitle, rect, dwTextStyle, nGlassGlowSize, clrGlassText);
		}
		else
		{
			pDC->SetTextColor (clrText);
			pDC->DrawText (strTitle, rect, dwTextStyle);
		}
	}

	pDC->SetBkMode    (nOldMode);
	pDC->SetTextColor (clrOldText);
}

BOOL CBCGPVisualManagerScenic::IsOwnerDrawMenuCheck ()
{
	return CanDrawImage () ? FALSE : CBCGPWinXPVisualManager::IsOwnerDrawMenuCheck ();
}

BOOL CBCGPVisualManagerScenic::IsHighlightWholeMenuItem ()
{
	return CanDrawImage () ? TRUE : CBCGPWinXPVisualManager::IsHighlightWholeMenuItem ();
}

void CBCGPVisualManagerScenic::CleanUp ()
{
	m_AppCaptionFont.DeleteObject ();
	m_penSeparatorDark.DeleteObject ();
	m_penSeparatorLight.DeleteObject ();

	m_brRibbonBarBkgnd.DeleteObject ();

	m_ctrlMenuItemBack.CleanUp();
    m_MenuItemMarkerC.Clear ();
    m_MenuItemMarkerR.Clear ();
	m_ctrlMenuHighlighted[0].CleanUp();
	m_ctrlMenuHighlighted[1].CleanUp();

	m_ctrlRibbonComboBoxBtn.CleanUp ();

	m_ctrlRibbonCaptionQA.CleanUp ();
	m_ctrlRibbonCategoryBack.CleanUp ();
	m_ctrlRibbonCategoryTab.CleanUp ();
	m_ctrlRibbonCategoryTabSep.CleanUp ();
	m_ctrlRibbonPanelBackSep.CleanUp ();
	m_ctrlRibbonMainPanel.CleanUp ();
	m_ctrlRibbonBtnMainPanel.CleanUp();
	m_ctrlRibbonBtnGroup_S.CleanUp();
	m_ctrlRibbonBtnGroup_F.CleanUp();
	m_ctrlRibbonBtnGroup_M.CleanUp();
	m_ctrlRibbonBtnGroup_L.CleanUp();
	m_ctrlRibbonBtnGroupMenu_F[0].CleanUp();
	m_ctrlRibbonBtnGroupMenu_F[1].CleanUp();
	m_ctrlRibbonBtnGroupMenu_M[0].CleanUp();
	m_ctrlRibbonBtnGroupMenu_M[1].CleanUp();
	m_ctrlRibbonBtnGroupMenu_L[0].CleanUp();
	m_ctrlRibbonBtnGroupMenu_L[1].CleanUp();
	m_ctrlRibbonBtn[0].CleanUp();
	m_ctrlRibbonBtn[1].CleanUp();
	m_ctrlRibbonBtnMenuH[0].CleanUp();
	m_ctrlRibbonBtnMenuH[1].CleanUp();
	m_ctrlRibbonBtnMenuV[0].CleanUp();
	m_ctrlRibbonBtnMenuV[1].CleanUp();
	m_ctrlRibbonBtnLaunch.CleanUp();
	m_RibbonBtnLaunchIcon.Clear();
	m_ctrlRibbonBtnMain.CleanUp();
	m_ctrlRibbonSliderBtnPlus.CleanUp();
	m_ctrlRibbonSliderBtnMinus.CleanUp();
	m_RibbonBtnDefaultImage.Clear ();
	m_ctrlRibbonBtnDefault.CleanUp();
	m_ctrlRibbonBtnDefaultIcon.CleanUp();
	m_ctrlRibbonBtnDefaultQAT.CleanUp();
	m_ctrlRibbonBtnStatusPane.CleanUp();
	m_ctrlRibbonBtnPalette[0].CleanUp();
	m_ctrlRibbonBtnPalette[1].CleanUp();
	m_ctrlRibbonBtnPalette[2].CleanUp();

	m_ctrlRibbonBorder_QAT.CleanUp ();

	m_cacheRibbonCategoryBack.Clear ();
	m_cacheRibbonBtnGroup_S.Clear();
	m_cacheRibbonBtnGroup_F.Clear();
	m_cacheRibbonBtnGroup_M.Clear();
	m_cacheRibbonBtnGroup_L.Clear();
	m_cacheRibbonBtnGroupMenu_F[0].Clear();
	m_cacheRibbonBtnGroupMenu_M[0].Clear();
	m_cacheRibbonBtnGroupMenu_L[0].Clear();
	m_cacheRibbonBtnGroupMenu_F[1].Clear();
	m_cacheRibbonBtnGroupMenu_M[1].Clear();
	m_cacheRibbonBtnGroupMenu_L[1].Clear();
	m_cacheRibbonBtnDefault.Clear();

	m_ActivateFlag.RemoveAll();

	m_bLoaded = FALSE;
}

void CBCGPVisualManagerScenic::OnUpdateSystemColors ()
{
	HINSTANCE hinstResOld = NULL;

	if (m_hinstRes == NULL)
	{
		SetStyle();
		return;
	}

	if (m_hinstRes != NULL)
	{
		hinstResOld = AfxGetResourceHandle();
		AfxSetResourceHandle(m_hinstRes);
	}

	CleanUp ();

	CBCGPWinXPVisualManager::OnUpdateSystemColors ();

	CBCGPTagManager tm;

	if (!tm.LoadFromResource(GetStyleResourceID(), RT_BCG_STYLE_XML))
	{
#if !defined _AFXDLL
		TRACE(_T("\r\nImportant: to enable the Scenic look in static link,\r\n"));
		TRACE(_T("include afxribbon.rc from the RC file in your project.\r\n\r\n"));
		ASSERT(FALSE);
#endif
		if (hinstResOld != NULL)
		{
			AfxSetResourceHandle(hinstResOld);
		}

		return;
	}

	{
		CString strStyle;
		tm.ExcludeTag(_T("STYLE"), strStyle);
		tm.SetBuffer(strStyle);
	}

	CString strItem;

	if (!tm.IsEmpty())
	{
		int nVersion = 0;

		if (tm.ExcludeTag(_T("VERSION"), strItem))
		{
			CBCGPTagManager tmItem(strItem);

			tmItem.ReadInt(_T("NUMBER"), nVersion);

			int nType = 20;
			if (nVersion == 2007)
			{
				tmItem.ReadInt(_T("TYPE"), nType);

				m_bLoaded = TRUE;
			}

			if (m_bLoaded)
			{
				if (tmItem.ExcludeTag(_T("ID_PREFIX"), strItem))
				{
					strItem.TrimLeft();
					strItem.TrimRight();
					m_strStylePrefix = strItem;
				}
			}
		}
	}

	if (!m_bLoaded)
	{
		if (hinstResOld != NULL)
		{
			::AfxSetResourceHandle(hinstResOld);
		}

		return;
	}

	// menu
	if (tm.ExcludeTag(_T("MENU"), strItem))
	{
		CBCGPTagManager tmItem(strItem);

		tmItem.ReadInt(_T("BorderSize"), m_nMenuBorderSize);

		tmItem.ReadControlRenderer(_T("ItemBack"), m_ctrlMenuItemBack, MakeResourceID(_T("IDB_MENU_ITEM_BACK")));
		tmItem.ReadToolBarImages (_T("ItemCheck"), m_MenuItemMarkerC, MakeResourceID(_T("IDB_MENU_ITEM_MARKER_C")));
		tmItem.ReadToolBarImages (_T("ItemRadio"), m_MenuItemMarkerR, MakeResourceID(_T("IDB_MENU_ITEM_MARKER_R")));
		tmItem.ReadControlRenderer(_T("Highlighted"), m_ctrlMenuHighlighted[0], MakeResourceID(_T("IDB_MENU_BTN")));
		tmItem.ReadControlRenderer(_T("HighlightedDisabled"), m_ctrlMenuHighlighted[1], MakeResourceID(_T("IDB_MENU_BTN_DISABLED")));
	}

	m_brMenuRarelyUsed.DeleteObject();
	m_brMenuRarelyUsed.CreateSolidBrush(m_clrMenuRarelyUsed);

	m_clrRibbonEdit            = globalData.clrWindow;
	m_clrRibbonEditHighlighted = globalData.clrWindow;
	m_clrRibbonEditPressed     = m_clrRibbonEditHighlighted;
	m_clrRibbonEditDisabled    = globalData.clrBtnFace;

	m_clrRibbonEditBorder            = globalData.clrWindow;
	m_clrRibbonEditBorderDisabled    = globalData.clrBtnShadow;
	m_clrRibbonEditBorderHighlighted = m_clrMenuItemBorder;
	m_clrRibbonEditBorderPressed     = m_clrRibbonEditBorderHighlighted;
	m_clrRibbonEditSelection         = globalData.clrHilite;

	m_clrRibbonBarBkgnd         = globalData.clrBarFace;
	m_clrRibbonBarGradientLight = globalData.clrBarLight;
	m_clrRibbonBarGradientDark  = m_clrRibbonBarGradientLight;

	// bars
	if (tm.ExcludeTag (_T("BARS"), strItem))
	{
		CBCGPTagManager tmItem (strItem);

		CString strBar;
		if (tmItem.ExcludeTag (_T("RIBBONBAR"), strBar))
		{
			CBCGPTagManager tmBar (strBar);

			tmBar.ReadColor (_T("Bkgnd"), m_clrRibbonBarBkgnd);
			tmBar.ReadColor (_T("GradientLight"), m_clrRibbonBarGradientLight);
			m_clrRibbonBarGradientDark = m_clrRibbonBarGradientLight;
			tmBar.ReadColor (_T("GradientDark"), m_clrRibbonBarGradientDark);
		}
	}

	m_brRibbonBarBkgnd.DeleteObject ();
	m_brRibbonBarBkgnd.CreateSolidBrush  (m_clrRibbonBarBkgnd);

	if (tm.ExcludeTag(_T("RIBBON"), strItem))
	{
		CBCGPTagManager tmItem(strItem);

		CString str;

		if (tmItem.ExcludeTag (_T("CATEGORY"), str))
		{
			CBCGPTagManager tmCategory (str);
			tmCategory.ReadControlRenderer(_T("BACK"), m_ctrlRibbonCategoryBack, MakeResourceID(_T("IDB_RIBBON_CATEGORY_BACK")));

			CString strTab;
			if (tmCategory.ExcludeTag (_T("TAB"), strTab))
			{
				CBCGPTagManager tmTab (strTab);
				tmTab.ReadControlRenderer(_T("BUTTON"), m_ctrlRibbonCategoryTab, MakeResourceID(_T("IDB_RIBBON_CATEGORY_TAB")));
			}

			tmCategory.ReadControlRenderer (_T("TAB_SEPARATOR"), m_ctrlRibbonCategoryTabSep, MakeResourceID(_T("IDB_RIBBON_CATEGORY_TAB_SEP")));

//			tmCategory.ReadControlRenderer (_T("BUTTON_PAGE_L"), m_ctrlRibbonCategoryBtnPage[0], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_PAGE_L")));
//			tmCategory.ReadControlRenderer (_T("BUTTON_PAGE_R"), m_ctrlRibbonCategoryBtnPage[1], MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BTN_PAGE_R")));
		}

		if (tmItem.ExcludeTag(_T("PANEL"), str))
		{
			CBCGPTagManager tmPanel(str);

			{
				CString strBack;
				if (tmPanel.ExcludeTag (_T("BACK"), strBack))
				{
					CBCGPTagManager tmBack (strBack);

					tmBack.ReadControlRenderer (_T("SEPARATOR"), m_ctrlRibbonPanelBackSep, MakeResourceID(_T("IDB_RIBBON_PANEL_BACK_SEP")));
				}
			}

			{
				CString strCaption;
				if (tmPanel.ExcludeTag(_T("CAPTION"), strCaption))
				{
					CBCGPTagManager tmCaption(strCaption);

					tmCaption.ReadControlRenderer(_T("LAUNCH_BTN"), m_ctrlRibbonBtnLaunch, MakeResourceID(_T("IDB_RIBBON_BTN_LAUNCH")));
					tmCaption.ReadToolBarImages(_T("LAUNCH_ICON"), m_RibbonBtnLaunchIcon, MakeResourceID(_T("IDB_RIBBON_BTN_LAUNCH_ICON")));
					tmCaption.ReadColor(_T("TextNormal"), m_clrRibbonPanelCaptionText);
					tmCaption.ReadColor(_T("TextHighlighted"), m_clrRibbonPanelCaptionTextHighlighted);
				}
			}

			{
				CString strButtons;
				if (tmPanel.ExcludeTag(_T("BUTTONS"), strButtons))
				{
					CBCGPTagManager tmButtons(strButtons);

					tmButtons.ReadControlRenderer(_T("BUTTON_GROUP_F"), m_ctrlRibbonBtnGroup_F, MakeResourceID(_T("IDB_RIBBON_BTN_GROUP_F")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUP_M"), m_ctrlRibbonBtnGroup_M, MakeResourceID(_T("IDB_RIBBON_BTN_GROUP_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUP_L"), m_ctrlRibbonBtnGroup_L, MakeResourceID(_T("IDB_RIBBON_BTN_GROUP_L")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUP_S"), m_ctrlRibbonBtnGroup_S, MakeResourceID(_T("IDB_RIBBON_BTN_GROUP_S")));

					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_F_C"), m_ctrlRibbonBtnGroupMenu_F[0], MakeResourceID(_T("IDB_RIBBON_BTN_GROUPMENU_F_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_F_M"), m_ctrlRibbonBtnGroupMenu_F[1], MakeResourceID(_T("IDB_RIBBON_BTN_GROUPMENU_F_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_M_C"), m_ctrlRibbonBtnGroupMenu_M[0], MakeResourceID(_T("IDB_RIBBON_BTN_GROUPMENU_M_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_M_M"), m_ctrlRibbonBtnGroupMenu_M[1], MakeResourceID(_T("IDB_RIBBON_BTN_GROUPMENU_M_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_L_C"), m_ctrlRibbonBtnGroupMenu_L[0], MakeResourceID(_T("IDB_RIBBON_BTN_GROUPMENU_L_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_GROUPMENU_L_M"), m_ctrlRibbonBtnGroupMenu_L[1], MakeResourceID(_T("IDB_RIBBON_BTN_GROUPMENU_L_M")));

					tmButtons.ReadControlRenderer(_T("BUTTON_NORMAL_S"), m_ctrlRibbonBtn[0], MakeResourceID(_T("IDB_RIBBON_BTN_NORMAL_S")));
					tmButtons.ReadControlRenderer(_T("BUTTON_NORMAL_B"), m_ctrlRibbonBtn[1], MakeResourceID(_T("IDB_RIBBON_BTN_NORMAL_B")));

					tmButtons.ReadControlRenderer (_T("BUTTON_DEFAULT"), m_ctrlRibbonBtnDefault, MakeResourceID(_T("IDB_RIBBON_BTN_DEFAULT")));
					tmButtons.ReadControlRenderer (_T("BUTTON_DEFAULT_ICON"), m_ctrlRibbonBtnDefaultIcon, MakeResourceID(_T("IDB_RIBBON_BTN_DEFAULT_ICON")));
					tmButtons.ReadToolBarImages(_T("BUTTON_DEFAULT_IMAGE"), m_RibbonBtnDefaultImage, MakeResourceID(_T("IDB_RIBBON_BTN_DEFAULT_IMAGE")));
					tmButtons.ReadControlRenderer(_T("BUTTON_DEFAULT_QAT"), m_ctrlRibbonBtnDefaultQAT, MakeResourceID(_T("IDB_RIBBON_BTN_DEFAULT_QAT")));

					tmButtons.ReadControlRenderer(_T("BUTTON_MENU_H_C"), m_ctrlRibbonBtnMenuH[0], MakeResourceID(_T("IDB_RIBBON_BTN_MENU_H_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_MENU_H_M"), m_ctrlRibbonBtnMenuH[1], MakeResourceID(_T("IDB_RIBBON_BTN_MENU_H_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_MENU_V_C"), m_ctrlRibbonBtnMenuV[0], MakeResourceID(_T("IDB_RIBBON_BTN_MENU_V_C")));
					tmButtons.ReadControlRenderer(_T("BUTTON_MENU_V_M"), m_ctrlRibbonBtnMenuV[1], MakeResourceID(_T("IDB_RIBBON_BTN_MENU_V_M")));

					tmButtons.ReadControlRenderer(_T("BUTTON_PNL_T"), m_ctrlRibbonBtnPalette[0], MakeResourceID(_T("IDB_RIBBON_BTN_PALETTE_T")));
					tmButtons.ReadControlRenderer(_T("BUTTON_PNL_M"), m_ctrlRibbonBtnPalette[1], MakeResourceID(_T("IDB_RIBBON_BTN_PALETTE_M")));
					tmButtons.ReadControlRenderer(_T("BUTTON_PNL_B"), m_ctrlRibbonBtnPalette[2], MakeResourceID(_T("IDB_RIBBON_BTN_PALETTE_B")));
				}
			}

			{
				CString strEdit;
				if (tmPanel.ExcludeTag (_T("EDIT"), strEdit))
				{
					CBCGPTagManager tmEdit (strEdit);

					tmEdit.ReadColor (_T("Normal"), m_clrRibbonEdit);
					tmEdit.ReadColor (_T("Highlighted"), m_clrRibbonEditHighlighted);
					tmEdit.ReadColor (_T("Disabled"), m_clrRibbonEditDisabled);
					tmEdit.ReadColor (_T("Pressed"), m_clrRibbonEditPressed);

					tmEdit.ReadColor (_T("BorderNormal"), m_clrRibbonEditBorder);
					tmEdit.ReadColor (_T("BorderHighlighted"), m_clrRibbonEditBorderHighlighted);
					tmEdit.ReadColor (_T("BorderDisabled"), m_clrRibbonEditBorderDisabled);
					tmEdit.ReadColor (_T("BorderPressed"), m_clrRibbonEditBorderPressed);
					tmEdit.ReadColor (_T("Selection"), m_clrRibbonEditSelection);

					CString strButton;
					if (tmEdit.ExcludeTag (_T("BUTTON"), strButton))
					{
						CBCGPTagManager tmButton (strButton);

						tmButton.ReadControlRenderer (_T("IMAGE"), m_ctrlRibbonComboBoxBtn, MakeResourceID(_T("IDB_COMBOBOX_BTN")));
					}
				}
			}
		}


		tmItem.ReadControlRenderer(_T("MAIN_BUTTON"), m_ctrlRibbonBtnMain, MakeResourceID(_T("IDB_RIBBON_BTN_MAIN")));

		if (tmItem.ExcludeTag(_T("MAIN"), str))
		{
			CBCGPTagManager tmMain(str);

			tmMain.ReadControlRenderer (_T("BACK"), m_ctrlRibbonMainPanel, MakeResourceID(_T("IDB_RIBBON_PANEL_MAIN")));
			tmMain.ReadControlRenderer(_T("BUTTON"), m_ctrlRibbonBtnMainPanel, MakeResourceID(_T("IDB_RIBBON_BTN_PANEL_MAIN")));
		}

		if (tmItem.ExcludeTag (_T("CAPTION"), str))
		{
			CBCGPTagManager tmCaption (str);

			tmCaption.ReadControlRenderer(_T("QA"), m_ctrlRibbonCaptionQA, MakeResourceID(_T("IDB_RIBBON_CAPTION_QA")));
		}

		if (tmItem.ExcludeTag(_T("STATUS"), str))
		{
			CBCGPTagManager tmStatus(str);
			tmStatus.ReadControlRenderer(_T("PANE_BUTTON"), m_ctrlRibbonBtnStatusPane, MakeResourceID(_T("IDB_RIBBON_BTN_STATUS_PANE")));

			CString strSlider;
			if (tmStatus.ExcludeTag(_T("SLIDER"), strSlider))
			{
				CBCGPTagManager tmSlider(strSlider);

				tmSlider.ReadControlRenderer(_T("PLUS"), m_ctrlRibbonSliderBtnPlus, MakeResourceID(_T("IDB_RIBBON_SLIDER_BTN_PLUS")));
				tmSlider.ReadControlRenderer(_T("MINUS"), m_ctrlRibbonSliderBtnMinus, MakeResourceID(_T("IDB_RIBBON_SLIDER_BTN_MINUS")));
			}
		}

		if (tmItem.ExcludeTag (_T("BORDERS"), str))
		{
			CBCGPTagManager tmBorders (str);

			tmBorders.ReadControlRenderer (_T("QAT"), m_ctrlRibbonBorder_QAT, MakeResourceID(_T("IDB_RIBBON_BORDER_QAT")));
			//tmBorders.ReadControlRenderer (_T("FLOATY"), m_ctrlRibbonBorder_Floaty, MakeResourceID(_T("IDB_OFFICE2007_RIBBON_BORDER_FLOATY")));
		}
	}

	if (hinstResOld != NULL)
	{
		AfxSetResourceHandle(hinstResOld);
	}


	NONCLIENTMETRICS ncm;
	globalData.GetNonClientMetrics (ncm);
	m_AppCaptionFont.CreateFontIndirect (&ncm.lfCaptionFont);

	m_penSeparatorDark.CreatePen (PS_SOLID, 0, RGB (198, 212, 227));
	m_penSeparatorLight.CreatePen (PS_SOLID, 0, RGB (237, 243, 248));
}

void CBCGPVisualManagerScenic::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawMenuBorder (pDC, pMenu, rect);
		return;
	}

#ifndef BCGP_EXCLUDE_RIBBON
	if (pMenu != NULL)
	{
		CBCGPRibbonPanelMenuBar* pRibbonMenuBar = 
			DYNAMIC_DOWNCAST (CBCGPRibbonPanelMenuBar, pMenu->GetMenuBar ());

		if (pRibbonMenuBar != NULL)
		{
			ASSERT_VALID (pRibbonMenuBar);

			if (pRibbonMenuBar->IsMainPanel ())
			{
				if (m_ctrlRibbonMainPanel.IsValid ())
				{
					m_ctrlRibbonMainPanel.DrawFrame (pDC, rect);
				}

				return;
			}

			if (!pRibbonMenuBar->IsMenuMode ())
			{
				if (pRibbonMenuBar->IsQATPopup () &&
					m_ctrlRibbonBorder_QAT.IsValid ())
				{
					m_ctrlRibbonBorder_QAT.DrawFrame (pDC, rect);
					return;
				}
				else if (pRibbonMenuBar->IsCategoryPopup ())
				{
					return;
				}
				else if (pRibbonMenuBar->IsFloaty () /*&&
						 m_ctrlRibbonBorder_Floaty.IsValid ()*/)
				{
/*
					m_ctrlRibbonBorder_Floaty.DrawFrame (pDC, rect);
					return;
*/
				}
				else
				{
					if (pRibbonMenuBar->GetPanel () != NULL)
					{
						return;
					}
				}
			}
		}
	}
#endif
/*
	CBCGPBaseControlBar* pTopLevelBar = NULL;

	for (CBCGPPopupMenu* pParentMenu = pMenu; 
		pParentMenu != NULL; pParentMenu = pParentMenu->GetParentPopupMenu ())
	{
		CBCGPToolbarMenuButton* pParentButton = pParentMenu->GetParentButton ();
		if (pParentButton == NULL)
		{
			break;
		}
	
		pTopLevelBar = 
			DYNAMIC_DOWNCAST (CBCGPBaseControlBar, pParentButton->GetParentWnd ());
	}

	if (pTopLevelBar == NULL || pTopLevelBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		m_ctrlPopupBorder.DrawFrame (pDC, rect);
	}
	else
	{
		CBCGPVisualManager2003::OnDrawMenuBorder (pDC, pMenu, rect);
	}
*/

	CBCGPWinXPVisualManager::OnDrawMenuBorder (pDC, pMenu, rect);
}

void CBCGPVisualManagerScenic::OnDrawMenuCheck (CDC* pDC, CBCGPToolbarMenuButton* pButton, 
		CRect rect, BOOL bHighlight, BOOL bIsRadio)
{
	ASSERT_VALID (pButton);

    CBCGPToolBarImages& img = bIsRadio ? m_MenuItemMarkerR : m_MenuItemMarkerC;

	if (!CanDrawImage () || img.GetCount () == 0)
	{
		CBCGPWinXPVisualManager::OnDrawMenuCheck (pDC, pButton, rect, bHighlight, bIsRadio);
		return;
	}

    CSize size (img.GetImageSize ());
    CRect rectImage (0, 0, size.cx, size.cy);

    if ((pButton->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED)
    {
        rectImage.OffsetRect (0, size.cy);
    }

	if (globalData.m_bIsRTL)
	{
		img.Mirror ();
	}

    img.DrawEx (pDC, rect, 0, CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter, rectImage);

	if (globalData.m_bIsRTL)
	{
		img.Mirror ();
	}
}

void CBCGPVisualManagerScenic::OnHighlightMenuItem (CDC *pDC, CBCGPToolbarMenuButton* pButton,
		CRect rect, COLORREF& clrText)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnHighlightMenuItem (pDC, pButton, rect, clrText);
		return;
	}

	clrText = globalData.clrMenuText;
	m_ctrlMenuHighlighted[(pButton->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED ? 1 : 0].Draw (pDC, rect);
}

#ifndef BCGP_EXCLUDE_RIBBON

void CBCGPVisualManagerScenic::OnDrawRibbonCaption (CDC* pDC, CBCGPRibbonBar* pBar,
											  CRect rectCaption, CRect rectText)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonCaption(pDC, pBar, rectCaption, rectText);
		return;
	}

	CWnd* pWnd = pBar->GetParent ();
	ASSERT_VALID (pWnd);

	//const DWORD dwStyle   = pWnd->GetStyle ();
	const DWORD dwStyleEx = pWnd->GetExStyle ();

	const BOOL bIsRTL     = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
	const BOOL bActive    = IsWindowActive (pWnd);
	const BOOL bGlass	  = pBar->IsTransparentCaption ();

    {
		CSize szSysBorder(GetSystemBorders(TRUE));

		if (!bGlass)
		{
			CRect rectCaption1(rectCaption);
			rectCaption1.InflateRect(szSysBorder.cx, szSysBorder.cy, szSysBorder.cx, 0);

			if (m_hThemeWindow != NULL)
			{
				(*m_pfDrawThemeBackground) (m_hThemeWindow, pDC->GetSafeHdc(),
						/*WP_CAPTION*/1, bActive ? /*CS_ACTIVE*/1 : /*CS_INACTIVE*/2, &rectCaption1, 0);
			}
			else
			{
				CBCGPDrawManager dm (*pDC);
				dm.FillGradient (rectCaption1, 
					bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption, 
					bActive ? globalData.clrActiveCaptionGradient : globalData.clrInactiveCaptionGradient, FALSE);
			}
		}

		CRect rectQAT = pBar->GetQuickAccessToolbarLocation ();

		if (rectQAT.left > rectQAT.right)
		{
			rectText.left = rectQAT.left + 1;
		}

		BOOL bHide  = (pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ALL) != 0;
		BOOL bExtra = !bHide && pBar->IsQuickAccessToolbarOnTop () &&
					  rectQAT.left < rectQAT.right && !pBar->IsQATEmpty();

		BOOL bDrawIcon = (bHide && !bExtra) || pBar->IsScenicLook ();

		if (bExtra)
		{
			CRect rectQAFrame (rectQAT);
			rectQAFrame.right = rectText.left - 6;
			rectQAFrame.InflateRect (1, 1, 1, 1);

			const CBCGPControlRendererParams& params = m_ctrlRibbonCaptionQA.GetParams ();

			if (rectQAFrame.Height () < params.m_rectImage.Height ())
			{
				rectQAFrame.top = rectQAFrame.bottom - params.m_rectImage.Height ();
			}
/*
			if (bGlass)
			{
				const int dxFrame = GetSystemMetrics (SM_CXSIZEFRAME) / 2;
				const int nTop = globalData.GetRibbonImageScale () != 1. ? -2 : 1;

				rectQAFrame.DeflateRect (1, nTop, dxFrame, 0);
			}
*/
			m_ctrlRibbonCaptionQA.Draw (pDC, rectQAFrame, bActive ? 0 : 1);
		}
		
		if (bDrawIcon)
		{
			BOOL bDestroyIcon = FALSE;
			HICON hIcon = globalUtils.GetWndIcon (pWnd, &bDestroyIcon);

			if (hIcon != NULL)
			{
				CSize szIcon (::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON));

				long x = rectCaption.left + 2;
				long y = rectCaption.top  + max (0, (rectCaption.Height () - szIcon.cy) / 2);

				if (bGlass)
				{
					globalData.DrawIconOnGlass (m_hThemeWindow, pDC, hIcon, CRect (x, y, x + szIcon.cx, y + szIcon.cy));
				}
				else
				{
					::DrawIconEx (pDC->GetSafeHdc (), x, y, hIcon, szIcon.cx, szIcon.cy,
						0, NULL, DI_NORMAL);
				}

				if (rectText.left < (x + szIcon.cx + 4))
				{
					rectText.left = x + szIcon.cx + 4;
				}

				if (bDestroyIcon)
				{
					::DestroyIcon (hIcon);
				}
			}
		}
    }

	CString strText;
	pWnd->GetWindowText (strText);

	CFont* pOldFont = (CFont*)pDC->SelectObject (&m_AppCaptionFont);
	ASSERT (pOldFont != NULL);

	DrawNcText (pDC, rectText, strText, bActive, 
		bIsRTL, FALSE, bGlass, pWnd->IsZoomed () ? 0 : 10, 
		pWnd->IsZoomed () ? RGB (255, 255, 255) : (COLORREF)-1);

	pDC->SelectObject (pOldFont);
}

void CBCGPVisualManagerScenic::OnDrawRibbonCaptionButton(CDC* pDC, CBCGPRibbonCaptionButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	const BOOL bHighlighted = pButton->IsHighlighted () || pButton->IsFocused ();
	const BOOL bPressed = pButton->IsPressed ();

	BCGBUTTON_STATE state = ButtonsIsRegular;
	if (bPressed)
	{
		if (bHighlighted)
		{
			state = ButtonsIsPressed;
		}
	}
	else if (bHighlighted)
	{
		state = ButtonsIsHighlighted;
	}

	const BOOL bMDI = pButton->IsMDIChildButton ();
	BOOL bActive = TRUE;

	if (!bMDI)
	{
		CBCGPRibbonBar* pBar = pButton->GetParentRibbonBar ();
		if (pBar->GetSafeHwnd () != NULL)
		{
			CWnd* pWnd = pBar->GetParent ();
			ASSERT_VALID (pWnd);

			bActive = IsWindowActive (pWnd);
		}
	}

	CRect rect (pButton->GetRect());
	rect.DeflateRect (1, 1);

	DrawNcBtn(pDC, rect, pButton->GetID(), state, FALSE, bActive, bMDI);
}

COLORREF CBCGPVisualManagerScenic::OnDrawRibbonPanel (
		CDC* pDC,
		CBCGPRibbonPanel* pPanel, 
		CRect rectPanel,
		CRect rectCaption)
{
	if (!CanDrawImage ())
	{
		return CBCGPWinXPVisualManager::OnDrawRibbonPanel (pDC, pPanel, rectPanel, rectCaption);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pPanel);

	if (pPanel->IsKindOf (RUNTIME_CLASS(CBCGPRibbonMainPanel)))
	{
		const int nBorderSize = GetPopupMenuBorderSize ();
		rectPanel.InflateRect (nBorderSize, nBorderSize);
		
		m_ctrlRibbonMainPanel.Draw (pDC, rectPanel);
	}
	else
	{
		if (!pPanel->IsMenuMode () && !pPanel->IsCollapsed ())
		{
			int nWidth = m_ctrlRibbonPanelBackSep.GetParams ().m_rectImage.Width ();
			int nWidth2 = nWidth / 2;

			rectPanel.left = rectPanel.right - nWidth2;
			rectPanel.right += (nWidth - nWidth2);
			rectPanel.DeflateRect (0, 4);

			m_ctrlRibbonPanelBackSep.Draw (pDC, rectPanel);
		}
	}

	return globalData.clrBarText;
}

COLORREF CBCGPVisualManagerScenic::OnFillRibbonPanelCaption (
					CDC* /*pDC*/,
					CBCGPRibbonPanel* /*pPanel*/, 
					CRect /*rectCaption*/)
{
	return m_clrRibbonPanelCaptionText;
}

void CBCGPVisualManagerScenic::OnDrawRibbonCategory (
		CDC* pDC, 
		CBCGPRibbonCategory* pCategory,
		CRect rectCategory)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pCategory);

	if (!CanDrawImage () || pCategory->IsOnDialogBar ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonCategory (pDC, pCategory, rectCategory);
		return;
	}

	CBCGPControlRenderer* pRenderer = &m_ctrlRibbonCategoryBack;
	CBCGPBitmapCache* pCache = &m_cacheRibbonCategoryBack;
/*
	CBCGPBaseRibbonElement* pParentButton = pCategory->GetParentButton ();

	if (pCategory->GetTabColor () != BCGPCategoryColor_None &&
		(pParentButton == NULL || !pParentButton->IsQATMode ()))
	{
		XRibbonContextCategory& context = 
			m_ctrlRibbonContextCategory[pCategory->GetTabColor () - 1];

		pRenderer = &context.m_ctrlBack;
		pCache    = &context.m_cacheBack;
	}
*/
	const CBCGPControlRendererParams& params = pRenderer->GetParams ();

	CBCGPRibbonPanelMenuBar* pMenuBar = pCategory->GetParentMenuBar ();
	if (pMenuBar != NULL)
	{
		if (pMenuBar->GetCategory () != NULL)
		{
			if (rectCategory.left < 0 || rectCategory.top < 0)
			{
				CBCGPDrawManager dm (*pDC);
				dm.FillGradient (rectCategory, m_clrRibbonBarGradientDark, m_clrRibbonBarGradientLight, TRUE);

				return;
			}
		}
	}

	int nCacheIndex = -1;
	if (pCache != NULL)
	{
		CSize size (params.m_rectImage.Width (), rectCategory.Height ());
		nCacheIndex = pCache->FindIndex (size);
		if (nCacheIndex == -1)
		{
			nCacheIndex = pCache->CacheY (size.cy, *pRenderer);
		}
	}

	if (nCacheIndex != -1)
	{
		pCache->Get(nCacheIndex)->DrawY (pDC, rectCategory, 
			CSize (params.m_rectInter.left, params.m_rectImage.right - params.m_rectInter.right));
	}
	else
	{
		pRenderer->Draw (pDC, rectCategory);
	}
}

COLORREF CBCGPVisualManagerScenic::OnDrawRibbonCategoryTab (
					CDC* pDC, 
					CBCGPRibbonTab* pTab,
					BOOL bIsActive)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTab);

	if (!CanDrawImage ())
	{
		return CBCGPWinXPVisualManager::OnDrawRibbonCategoryTab (pDC, 
											pTab, bIsActive);
	}

	CBCGPRibbonCategory* pCategory = pTab->GetParentCategory ();
	ASSERT_VALID (pCategory);
	CBCGPRibbonBar* pBar = pCategory->GetParentRibbonBar ();
	ASSERT_VALID (pBar);

	bIsActive = bIsActive && 
		((pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0 || pTab->GetDroppedDown () != NULL);

	const BOOL bPressed     = pTab->IsPressed ();
	const BOOL bIsFocused	= pTab->IsFocused () && (pBar->GetHideFlags () & BCGPRIBBONBAR_HIDE_ELEMENTS);
	const BOOL bIsHighlight = (pTab->IsHighlighted () || bIsFocused) && !pTab->IsDroppedDown ();

	CRect rectTab (pTab->GetRect ());
	rectTab.bottom++;

	int ratio = 0;
	if (m_ctrlRibbonCategoryTabSep.IsValid ())
	{
		ratio = pBar->GetTabTrancateRatio ();
	}

	if (ratio > 0)
	{
		rectTab.left++;
	}

	int nImage = bIsActive ? 3 : 0;

	if (bPressed)
	{
		if (bIsHighlight)
		{
			nImage = bIsActive ? 2 : 1;
		}
	}
	
	if(bIsHighlight)
	{
		nImage += 1;
	}

	CBCGPControlRenderer* pRenderer = &m_ctrlRibbonCategoryTab;
/*
	COLORREF clrText = m_clrRibbonCategoryText;
	COLORREF clrTextHighlighted = m_clrRibbonCategoryTextHighlighted;

	if (pCategory->GetTabColor () != BCGPCategoryColor_None || pTab->IsSelected ())
	{
		XRibbonContextCategory& context = 
				m_ctrlRibbonContextCategory [
				(pTab->IsSelected () || nImage == 4)
					? BCGPCategoryColor_Orange - 1
					: pCategory->GetTabColor () - 1];

		pRenderer = &context.m_ctrlTab;
		clrText  = context.m_clrText;
		clrTextHighlighted = context.m_clrTextHighlighted;
	}
*/
	pRenderer->Draw (pDC, rectTab, nImage);

	if (ratio > 0)
	{
		CRect rectSep (rectTab);
		rectSep.left = rectSep.right;
		rectSep.right++;
		rectSep.bottom--;

		m_ctrlRibbonCategoryTabSep.Draw (pDC, rectSep, 0, (BYTE)min(ratio * 255 / 100, 255));
	}

	return globalData.clrBarText;
}

COLORREF CBCGPVisualManagerScenic::OnDrawRibbonTabsFrame (
					CDC* pDC, 
					CBCGPRibbonBar* pWndRibbonBar, 
					CRect rectTab)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pWndRibbonBar);
	UNREFERENCED_PARAMETER(rectTab);

	return (COLORREF)-1;
}

void CBCGPVisualManagerScenic::OnFillMenuImageRect (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnFillMenuImageRect (pDC, pButton, rect, state);
		return;
	}

	OnFillButtonInterior (pDC, pButton, rect, state);
}

void CBCGPVisualManagerScenic::OnFillButtonInterior (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnFillButtonInterior (pDC, pButton, rect, state);
		return;
	}

	CCustomizeButton* pCustButton = DYNAMIC_DOWNCAST (CCustomizeButton, pButton);

	if (pCustButton == NULL)
	{
		if (CBCGPToolBar::IsCustomizeMode () && 
			!CBCGPToolBar::IsAltCustomizeMode () && !pButton->IsLocked ())
		{
			return;
		}

		CBCGPControlRenderer* pRenderer = NULL;
		int index = 0;

		BOOL bDisabled = (pButton->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED;
		//BOOL bPressed  = (pButton->m_nStyle & TBBS_PRESSED ) == TBBS_PRESSED;
		BOOL bChecked  = (pButton->m_nStyle & TBBS_CHECKED ) == TBBS_CHECKED;

		CBCGPBaseControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPBaseControlBar, pButton->GetParentWnd ());

		CBCGPToolbarMenuButton* pMenuButton = 
			DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
		if (pMenuButton != NULL && pBar != NULL)
		{
			if (!pBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)) &&
				pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
			{
				if (bChecked)
				{
					pRenderer = &m_ctrlMenuItemBack;

					if (bDisabled)
					{
						index = 1;
					}

					rect.InflateRect (0, 0, 0, 1);
				}
				else if (state == ButtonsIsPressed || state == ButtonsIsHighlighted)
				{
					pRenderer = &m_ctrlMenuHighlighted[bDisabled ? 1 : 0];
				}
				else
				{
					return;
				}
			}
		}

		if (pRenderer != NULL)
		{
			pRenderer->Draw (pDC, rect, index);
			return;
		}
	}

	CBCGPWinXPVisualManager::OnFillButtonInterior (pDC, pButton, rect, state);
}

void CBCGPVisualManagerScenic::OnDrawRibbonMenuCheckFrame (
					CDC* pDC,
					CBCGPRibbonButton* pButton, 
					CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonMenuCheckFrame (pDC, pButton, rect);
		return;
	}

	ASSERT_VALID (pDC);

	m_ctrlMenuItemBack.Draw (pDC, rect);
}

void CBCGPVisualManagerScenic::OnFillRibbonQATPopup (
				CDC* pDC, CBCGPRibbonPanelMenuBar* pMenuBar, CRect rect)
{
	if (!CanDrawImage () || !m_ctrlRibbonBorder_QAT.IsValid ())
	{
		CBCGPWinXPVisualManager::OnFillRibbonQATPopup (pDC, pMenuBar, rect);
		return;
	}

	ASSERT_VALID (pDC);

	m_ctrlRibbonBorder_QAT.FillInterior (pDC, rect);
}

int CBCGPVisualManagerScenic::GetRibbonPopupBorderSize (const CBCGPRibbonPanelMenu* pPopup) const
{
	if (!CanDrawImage ())
	{
		return CBCGPWinXPVisualManager::GetRibbonPopupBorderSize (pPopup);
	}

	if (pPopup != NULL)
	{
		ASSERT_VALID (pPopup);

		CBCGPRibbonPanelMenuBar* pRibbonMenuBar = 
			DYNAMIC_DOWNCAST (CBCGPRibbonPanelMenuBar, 
				(const_cast<CBCGPRibbonPanelMenu*>(pPopup))->GetMenuBar ());

		if (pRibbonMenuBar != NULL)
		{
			if (pRibbonMenuBar->IsMainPanel ())
			{
				return m_ctrlRibbonMainPanel.GetParams ().m_rectSides.left;
			}

			if (!pRibbonMenuBar->IsMenuMode ())
			{
				if (pRibbonMenuBar->IsQATPopup ())
				{
					if (m_ctrlRibbonBorder_QAT.IsValid ())
					{
						return m_ctrlRibbonBorder_QAT.GetParams ().m_rectSides.left;
					}
				}
				else if (pRibbonMenuBar->IsCategoryPopup ())
				{
					return 0;
				}
				else if (pRibbonMenuBar->IsFloaty ())
				{
/*
					if (m_ctrlRibbonBorder_Floaty.IsValid ())
					{
						return m_ctrlRibbonBorder_Floaty.GetParams ().m_rectSides.left;
					}
*/
				}
				else
				{
					if (pRibbonMenuBar->GetPanel () != NULL)
					{
						return 0;
					}

					// standard size
				}
			}
		}
	}

	return (int)GetPopupMenuBorderSize ();
}

#endif

void CBCGPVisualManagerScenic::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	CBCGPToolbarMenuButton* pMenuButton = DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	if (CanDrawImage () && pMenuButton != NULL &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		return;
	}

	CBCGPWinXPVisualManager::OnDrawButtonBorder (pDC, pButton, rect, state);
}

void CBCGPVisualManagerScenic::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pBar);

	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip, bNCArea);
		return;
	}

	if (pBar->IsOnGlass ())
	{
		pDC->FillSolidRect (rectClient, RGB (0, 0, 0));
		return;
	}

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonStatusBar)))
	{
		if (m_hThemeWindow != NULL)
		{
			(*m_pfDrawThemeBackground) (m_hThemeStatusBar, pDC->GetSafeHdc(),
				0, 0, &rectClient, 0);
			return;
		}
	}
	else if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonBar)))
	{
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rectClient, m_clrRibbonBarGradientDark, m_clrRibbonBarGradientLight, TRUE);

		return;
	}

	CBCGPWinXPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip, bNCArea);
}

void CBCGPVisualManagerScenic::OnDrawStatusBarSizeBox (CDC* pDC, CBCGPStatusBar* pStatBar,
			CRect rectSizeBox)
{
	if (!CanDrawImage () || m_hThemeStatusBar == NULL)
	{
		CBCGPWinXPVisualManager::OnDrawStatusBarSizeBox(pDC, pStatBar, rectSizeBox);
		return;
	}

	(*m_pfDrawThemeBackground) (m_hThemeStatusBar, pDC->GetSafeHdc(),
			/*SP_GRIPPER*/3, 0, &rectSizeBox, 0);
}

#ifndef BCGP_EXCLUDE_RIBBON

void CBCGPVisualManagerScenic::OnDrawRibbonPaletteButton (
					CDC* pDC, 
					CBCGPRibbonPaletteIcon* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonPaletteButton (pDC, pButton);
		return;
	}

	int index = 0;
	if (pButton->IsDisabled ())
	{
		index = 3;
	}
	else
	{
		if (pButton->IsPressed ())
		{
			if (pButton->IsHighlighted ())
			{
				index = 2;
			}
		}
		else if (pButton->IsHighlighted () || pButton->IsFocused ())
		{
			index = 1;
		}
	}

	int nBtn = 1;
	if (pButton->IsLast ())
	{
		nBtn = 2;
	}
	else if (pButton->IsFirst ())
	{
		nBtn = 0;
	}

	m_ctrlRibbonBtnPalette[nBtn].Draw (pDC, pButton->GetRect (), index);
}

COLORREF CBCGPVisualManagerScenic::OnDrawRibbonStatusBarPane (CDC* pDC, CBCGPRibbonStatusBar* pBar,
					CBCGPRibbonStatusBarPane* pPane)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);
	ASSERT_VALID (pPane);

	if (!CanDrawImage ())
	{
		return CBCGPWinXPVisualManager::OnDrawRibbonStatusBarPane (pDC, pBar, pPane);
	}

	CRect rectPane = pPane->GetRect ();

	const BOOL bHighlighted = pPane->IsHighlighted () || pPane->IsFocused ();
	const BOOL bChecked     = pPane->IsChecked ();

	if (bHighlighted || bChecked)
	{
		CRect rectButton = rectPane;
		rectButton.DeflateRect (1, 1);

		int index = 0;
		if (pPane->IsPressed ())
		{
			if (bHighlighted)
			{
				index = 1;
			}
		}
		else if (bChecked)
		{
			if (bHighlighted)
			{
				index = 0;
			}
			else
			{
				index = 1;
			}
		}

		m_ctrlRibbonBtnStatusPane.Draw (pDC, rectButton, index);
	}

	return (COLORREF)-1;
}

void CBCGPVisualManagerScenic::OnDrawRibbonSliderZoomButton (
			CDC* pDC, CBCGPRibbonSlider* pSlider, 
			CRect rect, BOOL bIsZoomOut, 
			BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
	ASSERT_VALID (pDC);

	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonSliderZoomButton (pDC, pSlider, rect,
			bIsZoomOut, bIsHighlighted, bIsPressed, bIsDisabled);
		return;
	}

	CBCGPControlRenderer* pRenderer = bIsZoomOut 
		? &m_ctrlRibbonSliderBtnMinus
		: &m_ctrlRibbonSliderBtnPlus;

	int index = 0;
	if (bIsDisabled)
	{
		index = 3;
	}
	else
	{
		if (bIsPressed)
		{
			if (bIsHighlighted)
			{
				index = 2;
			}
		}
		else if (bIsHighlighted)
		{
			index = 1;
		}
	}

	pRenderer->FillInterior (pDC, rect, 
		globalData.GetRibbonImageScale () != 1. ? CBCGPToolBarImages::ImageAlignHorzStretch : CBCGPToolBarImages::ImageAlignHorzCenter, 
		globalData.GetRibbonImageScale () != 1. ? CBCGPToolBarImages::ImageAlignVertStretch : CBCGPToolBarImages::ImageAlignVertCenter,
		index);
}

void CBCGPVisualManagerScenic::OnDrawRibbonProgressBar (CDC* pDC, 
												  CBCGPRibbonProgressBar* pProgress, 
												  CRect rectProgress, CRect rectChunk,
												  BOOL bInfiniteMode)
{
	if (!CanDrawImage () || m_hThemeProgress == NULL)
	{
		CBCGPWinXPVisualManager::OnDrawRibbonProgressBar(pDC, pProgress, rectProgress, 
			rectChunk, bInfiniteMode);
		return;
	}

	(*m_pfDrawThemeBackground) (m_hThemeProgress, pDC->GetSafeHdc(),
			/*PP_BAR*/1, 0, &rectProgress, 0);

	if (!bInfiniteMode)
	{
		if (!rectChunk.IsRectEmpty () || pProgress->GetPos () != pProgress->GetRangeMin ())
		{
			(*m_pfDrawThemeBackground) (m_hThemeProgress, pDC->GetSafeHdc(),
					/*PP_CHUNK*/3, 0, &rectChunk, 0);
		}
	}
	else
	{
//		(*m_pfDrawThemeBackground) (m_hThemeProgress, pDC->GetSafeHdc(),
//				/*PP_FILL*/5, 0, &rectProgress, 0);

		CRgn rgn;
		rgn.CreateRectRgnIndirect (rectProgress);
		pDC->SelectClipRgn (&rgn);

		double index = (pProgress->GetPos () - pProgress->GetRangeMin ()) * 2.0 / 
			double(pProgress->GetRangeMax () - pProgress->GetRangeMin ());

		rectProgress.OffsetRect((int)(rectProgress.Width () * (index - 1.0)), 0);

		(*m_pfDrawThemeBackground) (m_hThemeProgress, pDC->GetSafeHdc(),
				/*PP_MOVEOVERLAY*/8, 0, &rectProgress, 0);

	        pDC->SelectClipRgn (NULL);
	}
}

void CBCGPVisualManagerScenic::OnDrawDefaultRibbonImage (CDC* pDC, CRect rectImage,
					BOOL bIsDisabled/* = FALSE*/,
					BOOL bIsPressed/* = FALSE*/,
					BOOL bIsHighlighted/* = FALSE*/)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawDefaultRibbonImage(pDC, rectImage, bIsDisabled, 
			bIsPressed, bIsHighlighted);
		return;
	}

	m_RibbonBtnDefaultImage.DrawEx (pDC, rectImage, bIsDisabled ? 1 : 0,
		CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter);
}

void CBCGPVisualManagerScenic::OnDrawRibbonMainButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonMainButton(pDC, pButton);
		return;
	}

	BOOL bIsHighlighted = pButton->IsHighlighted () || pButton->IsFocused ();
	BOOL bIsPressed = pButton->IsPressed () || pButton->IsDroppedDown ();

	if (pButton->IsDroppedDown ())
	{
		bIsPressed = TRUE;
		bIsHighlighted = TRUE;
	}

	CRect rect = pButton->GetRect ();

	int index = 0;
	if (bIsPressed)
	{
		if (bIsHighlighted)
		{
			index = 2;
		}
	}
	else if (bIsHighlighted)
	{
		index = 1;
	}

	m_ctrlRibbonBtnMain.Draw (pDC, pButton->GetRect (), index);
}

void CBCGPVisualManagerScenic::OnDrawCheckBoxEx (CDC *pDC, CRect rect, 
											int nState,
											BOOL bHighlighted, 
											BOOL bPressed,
											BOOL bEnabled)
{
	if (m_hThemeButton == NULL)
	{
		CBCGPVisualManager::OnDrawCheckBoxEx (pDC, rect, nState, bHighlighted, bPressed, bEnabled);
		return;
	}

	DrawCheckBox (pDC, rect, bHighlighted, nState, bEnabled, bPressed);
}

void CBCGPVisualManagerScenic::OnDrawRadioButton (CDC *pDC, CRect rect, 
									 BOOL bOn,
									 BOOL bHighlighted, 
									 BOOL bPressed,
									 BOOL bEnabled)
{
	if (m_hThemeButton == NULL)
	{
		CBCGPVisualManager::OnDrawRadioButton (pDC, rect, bOn, bHighlighted, bPressed, bEnabled);
		return;
	}

	DrawRadioButton (pDC, rect, bHighlighted, bOn, bEnabled, bPressed);
}

void CBCGPVisualManagerScenic::OnDrawRibbonButtonBorder (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonButtonBorder (pDC, pButton);
		return;
	}

	if (pButton->IsKindOf (RUNTIME_CLASS(CBCGPRibbonEdit)))
	{
		CRect rect (pButton->GetRect ());

		COLORREF colorBorder = m_clrRibbonEditBorder;

		if (pButton->IsDisabled ())
		{
			colorBorder = m_clrRibbonEditBorderDisabled;
		}
		else if (pButton->IsHighlighted () || pButton->IsDroppedDown () || pButton->IsFocused ())
		{
			colorBorder = pButton->IsDroppedDown ()
				? m_clrRibbonEditBorderPressed
				: m_clrRibbonEditBorderHighlighted;
		}

		rect.left = pButton->GetCommandRect ().left;

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, colorBorder);
		}
		else
		{
			pDC->Draw3dRect (rect, colorBorder, colorBorder);
		}
	}
}

void CBCGPVisualManagerScenic::OnDrawRibbonLaunchButton (
					CDC* pDC,
					CBCGPRibbonLaunchButton* pButton,
					CBCGPRibbonPanel* pPanel)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pPanel);

	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonLaunchButton(pDC, pButton, pPanel);
		return;
	}

	CRect rect (pButton->GetRect ());
	rect.right--;
	rect.bottom--;

	BOOL bHighlighted = pButton->IsHighlighted () || pButton->IsFocused ();

	int index = 0;

	if (m_RibbonBtnLaunchIcon.GetCount () > 3)
	{
		if (pButton->IsDisabled ())
		{
			index = 3;
		}
		else if (pButton->IsPressed ())
		{
			if (bHighlighted)
			{
				index = 2;
			}
		}
		else if (bHighlighted)
		{
			index = 1;
		}
	}
	else
	{
		if (!pButton->IsDisabled ())
		{
			if (pButton->IsPressed ())
			{
				if (bHighlighted)
				{
					index = 2;
				}
			}
			else if (bHighlighted)
			{
				index = 1;
			}
		}
	}

	if (m_ctrlRibbonBtnLaunch.IsValid ())
	{
		m_ctrlRibbonBtnLaunch.Draw (pDC, rect, index);
	}

	if (m_RibbonBtnLaunchIcon.IsValid ())
	{
		const double dblImageScale = globalData.GetRibbonImageScale ();

		if (dblImageScale == 1.)
		{
			m_RibbonBtnLaunchIcon.DrawEx (pDC, rect, index,
				CBCGPToolBarImages::ImageAlignHorzCenter, CBCGPToolBarImages::ImageAlignVertCenter);
		}
		else
		{
			CSize sizeImage = m_RibbonBtnLaunchIcon.GetImageSize ();
			
			sizeImage.cx = (int)(.5 + dblImageScale * sizeImage.cx);
			sizeImage.cy = (int)(.5 + dblImageScale * sizeImage.cy);

			rect.left = rect.CenterPoint ().x - sizeImage.cx / 2;
			rect.right = rect.left + sizeImage.cx;

			rect.top = rect.CenterPoint ().y - sizeImage.cy / 2;
			rect.bottom = rect.top + sizeImage.cy;

			m_RibbonBtnLaunchIcon.DrawEx (pDC, rect, index, 
				CBCGPToolBarImages::ImageAlignHorzStretch, 
				CBCGPToolBarImages::ImageAlignVertStretch);
		}
	}
}

COLORREF CBCGPVisualManagerScenic::OnFillRibbonButton(CDC* pDC, CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (!CanDrawImage ())
	{
		return CBCGPWinXPVisualManager::OnFillRibbonButton(pDC, pButton);
	}

	const BOOL bIsMenuMode = pButton->IsMenuMode ();

	CRect rect (pButton->GetRect ());

	CBCGPControlRenderer* pRenderer = NULL;
	CBCGPBitmapCache* pCache = NULL;
	int index = 0;

	BOOL bDisabled    = pButton->IsDisabled ();
	BOOL bWasDisabled = bDisabled;
	BOOL bFocused     = pButton->IsFocused ();
	BOOL bDroppedDown = pButton->IsDroppedDown ();
	BOOL bPressed     = pButton->IsPressed () && !bIsMenuMode;
	BOOL bChecked     = pButton->IsChecked ();
	BOOL bHighlighted = pButton->IsHighlighted () || bFocused;

	BOOL bDefaultPanelButton = pButton->IsDefaultPanelButton () && !pButton->IsQATMode ();
	if (bFocused)
	{
		bDisabled = FALSE;
	}

	if (pButton->IsDroppedDown () && !bIsMenuMode)
	{
		bChecked     = TRUE;
		bPressed     = FALSE;
		bHighlighted = FALSE;
	}

	CBCGPBaseRibbonElement::RibbonElementLocation location = 
		pButton->GetLocationInGroup ();

	if (pButton->IsKindOf (RUNTIME_CLASS(CBCGPRibbonEdit)))
	{
		COLORREF color1 = m_clrRibbonEdit;
		if (bDisabled)
		{
			color1 = m_clrRibbonEditDisabled;
		}
		else if (bChecked || bHighlighted)
		{
			color1 = m_clrRibbonEditHighlighted;
		}

		COLORREF color2 = color1;

		rect.left = pButton->GetCommandRect ().left;

		{
			CBCGPDrawManager dm (*pDC);
			dm.FillGradient (rect, color1, color2, TRUE);
		}

		return (COLORREF)-1;
	}

	if (bChecked && bIsMenuMode && !pButton->IsPaletteIcon ())
	{
		bChecked = FALSE;
	}

	if (location != CBCGPBaseRibbonElement::RibbonElementNotInGroup &&
		pButton->IsShowGroupBorder ())
	{
		if (!pButton->GetMenuRect().IsRectEmpty ())
		{
			CRect rectC = pButton->GetCommandRect();
			CRect rectM = pButton->GetMenuRect();

			CBCGPControlRenderer* pRendererC = NULL;
			CBCGPControlRenderer* pRendererM = NULL;

			CBCGPBitmapCache* pCacheC = NULL;
			CBCGPBitmapCache* pCacheM = NULL;

			if (location == CBCGPBaseRibbonElement::RibbonElementSingleInGroup)
			{
				pRendererC = &m_ctrlRibbonBtnGroupMenu_F[0];
				pRendererM = &m_ctrlRibbonBtnGroupMenu_L[1];

				pCacheC = &m_cacheRibbonBtnGroupMenu_F[0];
				pCacheM = &m_cacheRibbonBtnGroupMenu_L[1];
			}
			else if (location == CBCGPBaseRibbonElement::RibbonElementFirstInGroup)
			{
				pRendererC = &m_ctrlRibbonBtnGroupMenu_F[0];
				pRendererM = &m_ctrlRibbonBtnGroupMenu_F[1];

				pCacheC = &m_cacheRibbonBtnGroupMenu_F[0];
				pCacheM = &m_cacheRibbonBtnGroupMenu_F[1];
			}
			else if (location == CBCGPBaseRibbonElement::RibbonElementLastInGroup)
			{
				pRendererC = &m_ctrlRibbonBtnGroupMenu_L[0];
				pRendererM = &m_ctrlRibbonBtnGroupMenu_L[1];

				pCacheC = &m_cacheRibbonBtnGroupMenu_L[0];
				pCacheM = &m_cacheRibbonBtnGroupMenu_L[1];
			}
			else
			{
				pRendererC = &m_ctrlRibbonBtnGroupMenu_M[0];
				pRendererM = &m_ctrlRibbonBtnGroupMenu_M[1];

				pCacheC = &m_cacheRibbonBtnGroupMenu_M[0];
				pCacheM = &m_cacheRibbonBtnGroupMenu_M[1];
			}

			int indexC = 0;
			int indexM = 0;

			BOOL bHighlightedC = pButton->IsCommandAreaHighlighted ();
			BOOL bHighlightedM = pButton->IsMenuAreaHighlighted ();

			if (bChecked)
			{
				indexC = 3;

				if (bHighlighted)
				{
					indexM = 5;
				}
			}

			if (bDisabled)
			{
				if (bChecked)
				{
					indexC = 5;
					indexM = 4;
				}
			}
			else
			{
				if (pButton->IsDroppedDown () && !bIsMenuMode)
				{
					indexC = pButton->IsChecked () ? 3 : 6;
					indexM = 3;
				}
				else
				{
					if (bFocused)
					{
						indexC = 6;
						indexM = 5;
					}

					if (bHighlightedC || bHighlightedM)
					{
						if (bChecked)
						{
							indexC = bHighlightedC ? 4 : 3;
						}
						else
						{
							indexC = bHighlightedC ? 1 : 6;
						}

						indexM = bHighlightedM ? 1 : 5;
					}

					if (bPressed)
					{
						if (bHighlightedC)
						{
							indexC = 2;
						}
					}
				}
			}

			if (indexC != -1 && indexM != -1)
			{
				int nCacheIndex = -1;
				if (pCacheC != NULL)
				{
					CSize size (rectC.Size ());
					nCacheIndex = pCacheC->FindIndex (size);
					if (nCacheIndex == -1)
					{
						nCacheIndex = pCacheC->Cache (size, *pRendererC);
					}
				}

				if (nCacheIndex != -1)
				{
					pCacheC->Get (nCacheIndex)->Draw (pDC, rectC, indexC);
				}
				else
				{
					pRendererC->Draw (pDC, rectC, indexC);
				}

				nCacheIndex = -1;
				if (pCacheM != NULL)
				{
					CSize size (rectM.Size ());
					nCacheIndex = pCacheM->FindIndex (size);
					if (nCacheIndex == -1)
					{
						nCacheIndex = pCacheM->Cache (size, *pRendererM);
					}
				}

				if (nCacheIndex != -1)
				{
					pCacheM->Get (nCacheIndex)->Draw (pDC, rectM, indexM);
				}
				else
				{
					pRendererM->Draw (pDC, rectM, indexM);
				}
			}

			return (COLORREF)-1;
		}
		else
		{
			if (location == CBCGPBaseRibbonElement::RibbonElementSingleInGroup)
			{
				pRenderer = &m_ctrlRibbonBtnGroup_S;
				pCache    = &m_cacheRibbonBtnGroup_S;
			}
			else if (location == CBCGPBaseRibbonElement::RibbonElementFirstInGroup)
			{
				pRenderer = &m_ctrlRibbonBtnGroup_F;
				pCache    = &m_cacheRibbonBtnGroup_F;
			}
			else if (location == CBCGPBaseRibbonElement::RibbonElementLastInGroup)
			{
				pRenderer = &m_ctrlRibbonBtnGroup_L;
				pCache    = &m_cacheRibbonBtnGroup_L;
			}
			else
			{
				pRenderer = &m_ctrlRibbonBtnGroup_M;
				pCache    = &m_cacheRibbonBtnGroup_M;
			}

			if (bChecked)
			{
				index = 3;
			}

			if (bDisabled && !bFocused)
			{
				index = 0;
			}
			else
			{
				if (bPressed)
				{
					if (bHighlighted)
					{
						index = 2;
					}
				}
				else if (bHighlighted)
				{
					index++;
				}
			}
		}
	}
	else if (bDefaultPanelButton)
	{
		if (bPressed)
		{
			if (bHighlighted)
			{
				index = 2;
			}
		}
		else if (bHighlighted)
		{
			index = 1;
		}
		else if (bChecked)
		{
			index = 2;
		}

		if (bFocused && !bDroppedDown && m_ctrlRibbonBtnDefault.GetImageCount () > 3)
		{
			index = 3;
		}

		if (index != -1)
		{
			pRenderer = &m_ctrlRibbonBtnDefault;
			CBCGPBitmapCache* pCache = &m_cacheRibbonBtnDefault;

			CBCGPRibbonCategory* pCategory = pButton->GetParentCategory ();
			if (pCategory != NULL)
			{
				ASSERT_VALID (pCategory);
/*
				if (pCategory->GetTabColor () != BCGPCategoryColor_None)
				{
					XRibbonContextCategory& context = 
						m_ctrlRibbonContextCategory[pCategory->GetTabColor () - 1];

					pRenderer = &context.m_ctrlBtnDefault;
					pCache    = &context.m_cacheBtnDefault;
				}
*/
			}

			const CBCGPControlRendererParams& params = pRenderer->GetParams ();

			int nCacheIndex = -1;
			if (pCache != NULL)
			{
				CSize size (params.m_rectImage.Width (), rect.Height ());
				nCacheIndex = pCache->FindIndex (size);
				if (nCacheIndex == -1)
				{
					nCacheIndex = pCache->CacheY (size.cy, *pRenderer);
				}
			}

			if (nCacheIndex != -1)
			{
				pCache->Get(nCacheIndex)->DrawY (pDC, rect, 
					CSize (params.m_rectInter.left, params.m_rectImage.right - params.m_rectInter.right),
					index);

				return globalData.clrBtnText;
			}
		}
	}
	else if ((!bDisabled && (bPressed || bChecked || bHighlighted)) || 
		    (bDisabled && bFocused))
	{
		if (!pButton->GetMenuRect().IsRectEmpty ()/* &&
			(pButton->IsHighlighted () || bChecked)*/)
		{
			CRect rectC = pButton->GetCommandRect();
			CRect rectM = pButton->GetMenuRect();

			CBCGPControlRenderer* pRendererC = pButton->IsMenuOnBottom () 
				? &m_ctrlRibbonBtnMenuV[0]
				: &m_ctrlRibbonBtnMenuH[0];
			CBCGPControlRenderer* pRendererM = pButton->IsMenuOnBottom () 
				? &m_ctrlRibbonBtnMenuV[1]
				: &m_ctrlRibbonBtnMenuH[1];

			int indexC = -1;
			int indexM = -1;

			BOOL bDropped      = pButton->IsDroppedDown ();
			BOOL bHighlightedC = pButton->IsCommandAreaHighlighted ();
			BOOL bHighlightedM = pButton->IsMenuAreaHighlighted ();

			if (bDisabled)
			{
				if (bHighlightedC || bHighlightedM)
				{
					indexC = 4;
					indexM = 4;

					if (bHighlightedM)
					{
						indexM = 0;

						if (bDropped && !bIsMenuMode)
						{
							indexC = 5;
							indexM = 2;
						}
						else if (bPressed)
						{
							indexM = 1;
						}
					}
				}
			}
			else
			{
				if (bDropped && !bIsMenuMode)
				{
					indexC = 5;
					indexM = 2;
				}
				else
				{
					if (bFocused)
					{
						indexC = 5;
						indexM = 4;
					}

					if (bChecked)
					{
						indexC = 2;
						indexM = 2;
					}

					if (bHighlightedC || bHighlightedM)
					{
						indexM = 4;

						if (bPressed)
						{
							if (bHighlightedC)
							{
								indexC = 1;
							}
							else if (bHighlightedM)
							{
								indexC = bChecked ? 3 : 5;
							}
						}
						else
						{
							indexC = bChecked ? 3 : 0;

							if (bHighlightedM)
							{
								indexC = bChecked ? 3 : 5;
								indexM = 0;
							}
						}
					}
				}
			}

			if (indexC != -1)
			{
				pRendererC->Draw (pDC, rectC, indexC);
			}

			if (indexM != -1)
			{
				pRendererM->Draw (pDC, rectM, indexM);
			}

			return (COLORREF)-1;
		}
		else
		{
			index = -1;

			pRenderer = &m_ctrlRibbonBtn[0];
			if (rect.Height () > pRenderer->GetParams ().m_rectImage.Height () * 1.5 &&
				m_ctrlRibbonBtn[1].IsValid ())
			{
				pRenderer = &m_ctrlRibbonBtn[1];
			}

			if (bDisabled && bFocused)
			{
				if (pRenderer->GetImageCount () > 4)
				{
					index = 4;
				}
				else
				{
					index = 0;
				}
			}
			
			if (!bDisabled)
			{
				if (bChecked)
				{
					index = 2;
				}

				if (bPressed)
				{
					if (bHighlighted)
					{
						index = 1;
					}
				}
				else if (bHighlighted)
				{
					index++;
				}
			}
		}
	}

	COLORREF clrText = bWasDisabled
		? globalData.clrGrayedText
		: COLORREF (-1);

	if (pRenderer != NULL)
	{
		if (index != -1)
		{
			int nCacheIndex = -1;
			if (pCache != NULL)
			{
				CSize size (rect.Size ());
				nCacheIndex = pCache->FindIndex (size);
				if (nCacheIndex == -1)
				{
					nCacheIndex = pCache->Cache (size, *pRenderer);
				}
			}

			if (nCacheIndex != -1)
			{
				pCache->Get (nCacheIndex)->Draw (pDC, rect, index);
			}
			else
			{
				pRenderer->Draw (pDC, rect, index);
			}

			if (!bWasDisabled)
			{
				clrText = globalData.clrBtnText;
			}
		}
	}

	return clrText;
}

void CBCGPVisualManagerScenic::OnDrawRibbonDefaultPaneButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonDefaultPaneButton (pDC, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	OnFillRibbonButton (pDC, pButton);

	BOOL bIsQATMode = pButton->IsQATMode ();

	CRect rectFrame (pButton->GetRect ());

	if (!bIsQATMode)
	{
		if (m_ctrlRibbonBtnDefaultIcon.IsValid ())
		{
			const CSize sizeImage = pButton->GetImageSize (CBCGPRibbonButton::RibbonImageSmall);
			const int nMarginX = 11;
			const int nMarginY = 10;
			
			rectFrame.top += nMarginY / 2;
			rectFrame.bottom = rectFrame.top + sizeImage.cy + 2 * nMarginY;
			rectFrame.top -= 2;
			rectFrame.left = rectFrame.CenterPoint ().x - sizeImage.cx / 2 - nMarginX;
			rectFrame.right = rectFrame.left + sizeImage.cx + 2 * nMarginX;

			m_ctrlRibbonBtnDefaultIcon.Draw (pDC, rectFrame);
		}
	}
	else
	{
		if (m_ctrlRibbonBtnDefaultQAT.IsValid ())
		{
			int index = 0;
			if (pButton->IsDroppedDown ())
			{
				index = 2;
			}
			else if (pButton->IsPressed ())
			{
				if (pButton->IsHighlighted ())
				{
					index = 2;
				}
			}
			else if (pButton->IsHighlighted () || pButton->IsFocused ())
			{
				index = 1;
			}

			m_ctrlRibbonBtnDefaultQAT.Draw (pDC, rectFrame, index);
		}
	}

	OnDrawRibbonDefaultPaneButtonContext (pDC, pButton);
}

void CBCGPVisualManagerScenic::OnDrawRibbonRecentFilesFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* pPanel,
					CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonRecentFilesFrame (pDC, 
					pPanel, rect);
		return;
	}

	ASSERT_VALID (pDC);

	rect.right += 2; //TODO
	pDC->FillRect (rect, &m_brRibbonBarBkgnd);

	CRect rectSeparator = rect;
	rectSeparator.right = rectSeparator.left + 2;

	pDC->Draw3dRect (rectSeparator, globalData.clrBtnShadow,
									globalData.clrBtnHilite);
}

#endif

BOOL CBCGPVisualManagerScenic::IsWindowActive(CWnd* pWnd) const
{
	BOOL bActive = FALSE;

	HWND hWnd = pWnd->GetSafeHwnd();

	if (hWnd != NULL)
	{
		if (!m_ActivateFlag.Lookup(pWnd->GetSafeHwnd(), bActive))
		{
			//ASSERT(FALSE);
			bActive = TRUE;
		}
	}

	return bActive;
}

BOOL CBCGPVisualManagerScenic::OnNcPaint (CWnd* pWnd, const CObList& lstSysButtons, CRect rectRedraw)
{
	UNREFERENCED_PARAMETER(lstSysButtons);

#ifdef BCGP_EXCLUDE_RIBBON

	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(rectRedraw);

#else

	if (globalData.DwmIsCompositionEnabled ())
	{
		return FALSE;
	}

	if (pWnd->GetSafeHwnd() == NULL || m_hThemeWindow == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pWnd);

	CBCGPRibbonBar* pBar = GetRibbonBar(pWnd);
	BOOL bRibbonCaption  = pBar != NULL && pBar->IsWindowVisible() && pBar->IsReplaceFrameCaption();

	if (!bRibbonCaption)
	{
		return FALSE;
	}
	else
	{
		const DWORD dwStyle = pWnd->GetStyle();
		BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;
        if (bMaximized)
		{
            return TRUE;
		}
	}

	CWindowDC dc(pWnd);

	if (dc.GetSafeHdc() != NULL)
	{
		CRgn rgn;
		if (!rectRedraw.IsRectEmpty())
		{
			rgn.CreateRectRgnIndirect(rectRedraw);
			dc.SelectClipRgn(&rgn);
		}

		CRect rtWindow;
		pWnd->GetWindowRect(rtWindow);
		pWnd->ScreenToClient(rtWindow);

		CRect rtClient;
		pWnd->GetClientRect(rtClient);

		rtClient.OffsetRect(-rtWindow.TopLeft());
		dc.ExcludeClipRect(rtClient);

		rtWindow.OffsetRect(-rtWindow.TopLeft());

		BOOL bActive = IsWindowActive(pWnd);

		// Modify bActive (if currently TRUE) for owner-drawn MDI child windows: draw child
		// frame active only if window is active MDI child and the MDI frame window is active.
		if (bActive && IsOwnerDrawCaption() && pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
		{
			CMDIFrameWnd *pParent = ((CMDIChildWnd *)pWnd)->GetMDIFrame();
			if (pParent)
			{
				CMDIChildWnd *pActiveChild = pParent->MDIGetActive(NULL);
				if (pActiveChild)
				{
					bActive = ((pActiveChild->GetSafeHwnd() == pWnd->GetSafeHwnd()) && IsWindowActive(pParent));
				}
			}
		}

		CRect rectCaption(rtWindow);
		CSize szSysBorders(GetSystemBorders(bRibbonCaption));

		rectCaption.bottom = rectCaption.top + szSysBorders.cy + pBar->GetCaptionHeight();

		(*m_pfDrawThemeBackground) (m_hThemeWindow, dc.GetSafeHdc(),
				/*WP_CAPTION*/1, bActive ? /*CS_ACTIVE*/1 : /*CS_INACTIVE*/2, &rectCaption, 0);

		rtWindow.top = rectCaption.bottom;
		dc.ExcludeClipRect(rectCaption);

		int framestate = bActive ? /*FS_ACTIVE*/1 : /*FS_INACTIVE*/2;

        CRect rectPart (rtWindow);
        rectPart.top = rectPart.bottom - szSysBorders.cy;
		(*m_pfDrawThemeBackground) (m_hThemeWindow, dc.GetSafeHdc(),
				/*WP_FRAMEBOTTOM*/9, framestate, &rectPart, 0);

        rectPart.bottom = rectPart.top;
        rectPart.top = rtWindow.top;
		rectPart.right = rectPart.left + szSysBorders.cx;
		(*m_pfDrawThemeBackground) (m_hThemeWindow, dc.GetSafeHdc(),
				/*WP_FRAMELEFT*/7, framestate, &rectPart, 0);

        rectPart.right = rtWindow.right;
        rectPart.left = rectPart.right - szSysBorders.cx;
		(*m_pfDrawThemeBackground) (m_hThemeWindow, dc.GetSafeHdc(),
				/*WP_FRAMERIGHT*/8, framestate, &rectPart, 0);

		dc.SelectClipRgn(NULL);

		return TRUE;
	}
#endif

	return FALSE;
}

BOOL CBCGPVisualManagerScenic::OnNcActivate(CWnd* pWnd, BOOL bActive)
{
	ASSERT_VALID(pWnd);

	if (pWnd->GetSafeHwnd() == NULL)
	{
		return FALSE;
	}

	if (globalData.DwmIsCompositionEnabled())
	{
		return FALSE;
	}

	// stay active if WF_STAYACTIVE bit is on
	if (pWnd->m_nFlags & WF_STAYACTIVE)
	{
		bActive = TRUE;
	}

	// but do not stay active if the window is disabled
	if (!pWnd->IsWindowEnabled())
	{
		bActive = FALSE;
	}

	BOOL bIsMDIFrame = FALSE;
	BOOL bWasActive = FALSE;

	// If the active state of an owner-draw MDI frame window changes, we need to
	// invalidate the MDI client area so the MDI child window captions are redrawn.
	if (IsOwnerDrawCaption())
	{
		bIsMDIFrame = pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd));
		bWasActive = IsWindowActive(pWnd);
	}

	m_ActivateFlag[pWnd->GetSafeHwnd()] = bActive;
	pWnd->SendMessage(WM_NCPAINT, 0, 0);

	if (IsOwnerDrawCaption())
	{
		if (bIsMDIFrame && (bWasActive != bActive))
		{
			::RedrawWindow(((CMDIFrameWnd *)pWnd)->m_hWndMDIClient, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
		}
	}

	return TRUE;
}

CSize CBCGPVisualManagerScenic::GetSystemBorders(BOOL bRibbonPresent) const
{
	CSize size(::GetSystemMetrics(SM_CYSIZEFRAME), ::GetSystemMetrics(SM_CXSIZEFRAME));

	if (bRibbonPresent)
	{
		size.cx--;
		size.cy--;
	}

	return size;
}

void CBCGPVisualManagerScenic::OnDrawComboDropButton (CDC* pDC, CRect rect,
											    BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* pButton)
{
	if (!CanDrawImage () || !pButton->IsRibbonButton ())
	{
		CBCGPWinXPVisualManager::OnDrawComboDropButton (pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
		return;
	}

	rect.InflateRect (0, 1, 1, 1);

	int nIndex = 0;
	if (bDisabled)
	{
		nIndex = 3;
	}
	else
	{
		if (bIsDropped)
		{
			nIndex = 2;
		}
		else if (bIsHighlighted)
		{
			nIndex = 1;
		}
	}

	m_ctrlRibbonComboBoxBtn.Draw (pDC, rect, nIndex);

	rect.DeflateRect (0, 1, 1, 1);

	rect.bottom -= 2;

	CBCGPMenuImages::Draw (pDC, CBCGPMenuImages::IdArowDown, rect,
		bDisabled 
		? CBCGPMenuImages::ImageGray 
		: CBCGPMenuImages::ImageBlack);
}


#ifndef BCGP_EXCLUDE_RIBBON

CBCGPRibbonBar*	CBCGPVisualManagerScenic::GetRibbonBar (CWnd* pWnd) const
{
	CBCGPRibbonBar* pBar = NULL;

	if (pWnd == NULL)
	{
		pWnd = AfxGetMainWnd ();
	}

	if (pWnd->GetSafeHwnd () == NULL)
	{
		return NULL;
	}

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
	{
		pBar = ((CBCGPFrameWnd*) pWnd)->GetRibbonBar ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
	{
		pBar = ((CBCGPMDIFrameWnd*) pWnd)->GetRibbonBar ();
	}

	return pBar;
}

BOOL CBCGPVisualManagerScenic::IsRibbonPresent (CWnd* pWnd) const
{
	CBCGPRibbonBar* pBar = GetRibbonBar (pWnd);

	return pBar != NULL && pBar->IsWindowVisible ();
}

COLORREF CBCGPVisualManagerScenic::GetRibbonEditBackgroundColor (
					CBCGPRibbonEditCtrl* pEdit,
					BOOL bIsHighlighted,
					BOOL bIsPaneHighlighted,
					BOOL bIsDisabled)
{
	if (!CanDrawImage ())
	{
		return CBCGPWinXPVisualManager::GetRibbonEditBackgroundColor ( 
			pEdit, bIsHighlighted, bIsPaneHighlighted, bIsDisabled);
	}

	COLORREF color = m_clrRibbonEdit;

	if (bIsDisabled)
	{
		color = m_clrRibbonEditDisabled;
	}
	else
	{
		if (bIsHighlighted)
		{
			color = m_clrRibbonEditHighlighted;
		}
	}

	return color;
}

void CBCGPVisualManagerScenic::OnDrawRibbonPaletteBorder (
				CDC* pDC, 
				CBCGPRibbonPaletteButton* pButton, 
				CRect rectBorder)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnDrawRibbonPaletteBorder (pDC, pButton, rectBorder);
		return;
	}

	rectBorder.right -= 5;

	ASSERT_VALID (pDC);
	pDC->Draw3dRect (rectBorder, m_clrRibbonEditBorder, m_clrRibbonEditBorder);
}

#endif

COLORREF CBCGPVisualManagerScenic::OnDrawMenuLabel (CDC* pDC, CRect rect)
{
	if (!CanDrawImage ())
	{
		return CBCGPWinXPVisualManager::OnDrawMenuLabel (pDC, rect);
	}

	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brRibbonBarBkgnd);

	CRect rectSeparator = rect;
	rectSeparator.top = rectSeparator.bottom - 2;

	DrawSeparator (pDC, rectSeparator, m_penSeparatorDark, m_penSeparatorLight, TRUE);

	return globalData.clrBarText;
}
//****
void CBCGPVisualManagerScenic::OnFillPopupMenuBackground (CDC* pDC, CRect rect)
{
	if (!CanDrawImage ())
	{
		CBCGPWinXPVisualManager::OnFillPopupMenuBackground (pDC, rect);
		return;
	}

	CBrush br (RGB (255, 255, 255));
	pDC->FillRect (rect, &br);
}
