//
// BCGPRibbonPanel.cpp: implementation of the CBCGPRibbonPanel class.
//
//////////////////////////////////////////////////////////////////////


#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPDrawManager.h"
#include "bcgglobals.h"

#include "bcgprores.h"
#include "BCGPVisualManager.h"
#include "BCGPToolBar.h"
#include "BCGPRibbonButtonsGroup.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonUndoButton.h"
#include "BCGPKeyboardManager.h"

#ifndef BCGP_EXCLUDE_RIBBON

//////////////////////////////////////////////////////////////////////
// CBCGPRibbonLaunchButton

IMPLEMENT_DYNCREATE(CBCGPRibbonLaunchButton, CBCGPRibbonButton)

CBCGPRibbonLaunchButton::CBCGPRibbonLaunchButton()
{
	m_pParentPanel = NULL;
}
//****
void CBCGPRibbonLaunchButton::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	if (m_pParentPanel != NULL)
	{
		ASSERT_VALID (m_pParentPanel);

		CBCGPVisualManager::GetInstance()->OnDrawRibbonLaunchButton (
			pDC, this, m_pParentPanel);
	}
	else
	{
		CBCGPRibbonButton::OnDraw (pDC);
	}
}
//****
CSize CBCGPRibbonLaunchButton::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);

	if (m_pParentPanel != NULL)
	{
		return CSize (0, 0);
	}

	return CBCGPRibbonButton::GetRegularSize (pDC);
}
//****
void CBCGPRibbonLaunchButton::OnClick (CPoint point)
{
	ASSERT_VALID (this);

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);
		m_pParentMenu->OnClickButton (this, point);
		return;
	}

	NotifyCommand();
}
//****
CRect CBCGPRibbonLaunchButton::GetKeyTipRect (CDC* pDC, BOOL bIsMenu)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_pParentPanel == NULL)
	{
		return CBCGPRibbonButton::GetKeyTipRect (pDC, bIsMenu);
	}

	ASSERT_VALID (m_pParentPanel);

	CSize sizeKeyTip = GetKeyTipSize (pDC);
	CRect rectKeyTip (0, 0, 0, 0);

	if (sizeKeyTip == CSize (0, 0) || m_rect.IsRectEmpty())
	{
		return rectKeyTip;
	}

	rectKeyTip.t = m_rect.b;
	rectKeyTip.r = m_pParentPanel->GetRect().r;
	rectKeyTip.l = rectKeyTip.r - sizeKeyTip.w;
	rectKeyTip.b = rectKeyTip.t + sizeKeyTip.h;

	return rectKeyTip;
}
//****
BOOL CBCGPRibbonLaunchButton::SetACCData (CWnd* pParent, CBCGPAccessibilityData& data)
{
	if (!CBCGPRibbonButton::SetACCData (pParent, data))
	{
		return FALSE;
	}

	data.m_bAccState |= STATE_SYSTEM_HASPOPUP;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CBCGPRibbonDefaultPanelButton

IMPLEMENT_DYNCREATE(CBCGPRibbonDefaultPanelButton, CBCGPRibbonButton)

CBCGPRibbonDefaultPanelButton::CBCGPRibbonDefaultPanelButton (CBCGPRibbonPanel* pPanel)
{
	m_hIcon = NULL;
	m_pPanel = pPanel;
}
//****
CBCGPRibbonDefaultPanelButton::~CBCGPRibbonDefaultPanelButton()
{
	if (m_bAutoDestroyIcon && m_hIcon != NULL)
	{
		::DestroyIcon (m_hIcon);
	}
}
//****
void CBCGPRibbonDefaultPanelButton::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPVisualManager::GetInstance()->OnDrawRibbonDefaultPaneButton 
		(pDC, this);
}
//****
int CBCGPRibbonDefaultPanelButton::AddToListBox (CBCGPRibbonCommandsListBox* pWndListBox, BOOL bDeep)
{
	ASSERT_VALID (this);

	if (m_pPanel != NULL)
	{
		ASSERT_VALID (m_pPanel);
		if (m_pPanel->IsMainPanel())
		{
			return -1;
		}
	}

	return CBCGPRibbonButton::AddToListBox (pWndListBox, bDeep);
}
//****
void CBCGPRibbonDefaultPanelButton::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	CBCGPRibbonDefaultPanelButton& src = (CBCGPRibbonDefaultPanelButton&) s;

	m_pPanel = src.m_pPanel;
	m_pParent = src.m_pParent;

	if (m_pPanel != NULL)
	{
		ASSERT_VALID (m_pPanel);
		m_strToolTip = m_pPanel->GetName();
	}
}
//****
void CBCGPRibbonDefaultPanelButton::OnLButtonDown (CPoint point)
{
	ASSERT_VALID (this);
	
	CBCGPBaseRibbonElement::OnLButtonDown (point);
	OnShowPopupMenu();
}
//****
void CBCGPRibbonDefaultPanelButton::OnShowPopupMenu()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pPanel);

	m_pPanel->ShowPopup (this);
}
//****
void CBCGPRibbonDefaultPanelButton::OnDrawOnList (CDC* pDC, CString strText,
									  int nTextOffset, CRect rect,
									  BOOL bIsSelected,
									  BOOL bHighlighted)
{
	BOOL bIsDisabled = m_bIsDisabled;
	m_bIsDisabled = FALSE;

	CRect rectText = rect;

	rectText.l += nTextOffset;
	const int xMargin = 3;
	rectText.DeflateRect (xMargin, 0);

	pDC->DrawText (strText, rectText, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	m_bIsDisabled = bIsDisabled;

	CBCGPVisualManager::GetInstance()->OnDrawRibbonDefaultPaneButtonIndicator
		(pDC, this, rect, bIsSelected, bHighlighted);
}
//****
void CBCGPRibbonDefaultPanelButton::DrawImage (CDC* pDC, 
	RibbonImageType type, CRect rectImage)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPRibbonDefaultPanelButton* pOrigButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonDefaultPanelButton, m_pOriginal);

	if (pOrigButton != NULL)
	{
		ASSERT_VALID (pOrigButton);

		pOrigButton->DrawImage (pDC, type, rectImage);
		return;
	}

	if (m_hIcon == NULL)
	{
		CBCGPVisualManager::GetInstance()->OnDrawDefaultRibbonImage (
			pDC, rectImage);
		return;
	}

	CSize sizeIcon (16, 16);

	if (globalData.GetRibbonImageScale() != 1.)
	{
		sizeIcon.w = (int) (.5 + globalData.GetRibbonImageScale() * sizeIcon.w);
		sizeIcon.h = (int) (.5 + globalData.GetRibbonImageScale() * sizeIcon.h);
	}

	BOOL bIsRTL = FALSE;

	CBCGPRibbonBar* pTopLevelRibbon = GetTopLevelRibbonBar();
	if (pTopLevelRibbon != NULL && (pTopLevelRibbon->GetExStyle() & WS_EX_LAYOUTRTL))
	{
		bIsRTL = TRUE;
	}

	if (globalData.GetRibbonImageScale() != 1. || bIsRTL)
	{
		UINT diFlags = DI_NORMAL;

		if (bIsRTL)
		{
			diFlags |= 0x0010 /*DI_NOMIRROR*/;
		}

		::DrawIconEx (pDC, 
			rectImage.CenterPoint().x - sizeIcon.w / 2, 
			rectImage.CenterPoint().y - sizeIcon.h / 2, 
			m_hIcon, sizeIcon.w, sizeIcon.h, 0, NULL,
			diFlags);
	}
	else
	{
		pDC->DrawState (
			CPoint (
				rectImage.CenterPoint().x - sizeIcon.w / 2,
				rectImage.CenterPoint().y - sizeIcon.h / 2),
			sizeIcon, m_hIcon, DSS_NORMAL, (HBRUSH) NULL);
	}
}
//****
BOOL CBCGPRibbonDefaultPanelButton::OnKey (BOOL /*bIsMenuKey*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pPanel);

	if (IsDisabled())
	{
		return FALSE;
	}

	if (!m_pPanel->m_rect.IsRectEmpty() && !m_pPanel->IsCollapsed() && !IsQATMode())
	{
		return FALSE;
	}

	OnShowPopupMenu();

	if (m_pPopupMenu != NULL)
	{
		ASSERT_VALID (m_pPopupMenu);
		m_pPopupMenu->SendMessage (WM_KEYDOWN, VK_DOWN);
	}

	return FALSE;
}
//****
BOOL CBCGPRibbonDefaultPanelButton::SetACCData (CWnd* pParent, CBCGPAccessibilityData& data)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pPanel);
	ASSERT_VALID (pParent);

	if (!CBCGPRibbonButton::SetACCData (pParent, data))
	{
		return FALSE;
	}

	if (m_rect.Width() == 0 && m_rect.Height() == 0)
	{
		data.m_nAccRole = ROLE_SYSTEM_TOOLBAR;
		data.m_strAccValue = _T("group");
		data.m_rectAccLocation = m_pPanel->GetRect();
		pParent->ClientToScreen (&data.m_rectAccLocation);
		data.m_bAccState = 0;
		data.m_strAccDefAction = _T("");
		return TRUE;
	}

	data.m_nAccRole = ROLE_SYSTEM_BUTTONDROPDOWNGRID;
	data.m_bAccState |= STATE_SYSTEM_HASPOPUP;
	data.m_strAccDefAction = _T("Open");

	if (IsDroppedDown())
	{
		data.m_bAccState |= STATE_SYSTEM_PRESSED;
		data.m_strAccDefAction = _T("Close");
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CBCGPRibbonPanel

UINT CBCGPRibbonPanel::m_nNextPanelID = (UINT)-10;

IMPLEMENT_DYNCREATE(CBCGPRibbonPanel, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4355)

CBCGPRibbonPanel::CBCGPRibbonPanel(LPCTSTR lpszName, CBitmap* hIcon, BOOL bAutoDestroyIcon) :
	m_btnDefault (this)
{
	CommonInit (lpszName, hIcon, bAutoDestroyIcon);
}
//****
CBCGPRibbonPanel::CBCGPRibbonPanel (CBCGPRibbonPaletteButton* pPaletteButton) :
	m_btnDefault (this)
{
	CommonInit();

	ASSERT_VALID (pPaletteButton);
	m_pPaletteButton = pPaletteButton;
}
//****
void CBCGPRibbonPanel::CopyFrom (CBCGPRibbonPanel& src)
{
	m_strName = src.m_strName;
	m_dwData = src.m_dwData;
	m_pParent = src.m_pParent;
	m_nXMargin = src.m_nXMargin;
	m_nYMargin = src.m_nYMargin;
	m_bShowCaption = src.m_bShowCaption;
	m_bAlignByColumn = src.m_bAlignByColumn;
	m_bCenterColumnVert = src.m_bCenterColumnVert;
	m_bJustifyColumns = src.m_bJustifyColumns;
	m_nMaxRows = src.m_nMaxRows;
	m_bPreserveElementOrder = src.m_bPreserveElementOrder;

	int i = 0;

	for (i = 0; i < src.m_arWidths.GetSize(); i++)
	{
		m_arWidths.Add (src.m_arWidths [i]);
	}

	for (i = 0; i < src.m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pSrcElem = src.m_arElements [i];
		ASSERT_VALID (pSrcElem);

		CBCGPBaseRibbonElement* pElem =
			(CBCGPBaseRibbonElement*) pSrcElem->GetRuntimeClass()->CreateObject();
		ASSERT_VALID (pElem);

		pElem->CopyFrom (*pSrcElem);
		pElem->SetOriginal (pSrcElem);

		m_arElements.Add (pElem);
	}

	m_btnLaunch.CopyFrom (src.m_btnLaunch);
	m_btnLaunch.SetOriginal (&src.m_btnLaunch);
}
//****
void CBCGPRibbonPanel::CommonInit (LPCTSTR lpszName, CBitmap* hIcon, BOOL bAutoDestroyIcon)
{
	m_strName = lpszName != NULL ? lpszName : _T("");

	int nIndex = m_strName.Find (_T('\n'));
	if (nIndex >= 0)
	{
		m_btnDefault.SetKeys (m_strName.Mid (nIndex + 1));
		m_strName = m_strName.Left (nIndex);
	}

	m_dwData = 0;
	m_btnDefault.m_bAutoDestroyIcon = bAutoDestroyIcon;
	m_btnDefault.m_hIcon = hIcon;
	m_btnDefault.SetText (m_strName);

	// jump across a ribbon (system) id
	if ((UINT)-108 <= m_nNextPanelID && m_nNextPanelID <= (UINT)-102)
	{
		m_nNextPanelID = (UINT)-109;
	}

	m_btnDefault.SetID (m_nNextPanelID--);

	m_rect.SetRectEmpty();
	m_pParent = NULL;
	m_pParentMenuBar = NULL;
	m_nCurrWidthIndex = 0;
	m_nFullWidth = 0;
	m_nRows = 0;
	m_nXMargin = 4;
	m_nYMargin = 2;
	m_bShowCaption = FALSE;
	m_bForceCollpapse = FALSE;
	m_bIsHighlighted = FALSE;
	m_bIsCalcWidth = FALSE;
	m_pHighlighted = NULL;
	m_bAlignByColumn = TRUE;
	m_bCenterColumnVert = FALSE;
	m_bFloatyMode = FALSE;
	m_bIsQATPopup = FALSE;
	m_bMenuMode = FALSE;
	m_bIsDefaultMenuLook = FALSE;
	m_rectCaption.SetRectEmpty();
	m_pPaletteButton = NULL;
	m_rectMenuAreaTop.SetRectEmpty();
	m_rectMenuAreaBottom.SetRectEmpty();
	m_pScrollBar = NULL;
	m_nScrollOffset = 0;
	m_bSizeIsLocked = FALSE;
	m_bJustifyColumns = FALSE;
	m_nMaxRows = 3;
	m_bOnDialogBar = FALSE;
	m_bScrollDnAvailable = FALSE;
	m_bTrancateCaption = FALSE;
	m_bPreserveElementOrder = FALSE;
}

#pragma warning (default : 4355)

CBCGPRibbonPanel::~CBCGPRibbonPanel()
{
	CBCGPBaseRibbonElement* pDroppedDown = GetDroppedDown();
	if (pDroppedDown != NULL)
	{
		ASSERT_VALID (pDroppedDown);
		pDroppedDown->ClosePopupMenu();
	}

	RemoveAll();
}
//****
void CBCGPRibbonPanel::EnableLaunchButton (UINT uiCmdID,
										   int nIconIndex,
										   LPCTSTR lpszKeys)
{
	ASSERT_VALID (this);

	m_btnLaunch.SetID (uiCmdID);
	m_btnLaunch.m_nSmallImageIndex = nIconIndex;
	m_btnLaunch.SetKeys (lpszKeys);
}
//****
void CBCGPRibbonPanel::Add (CBCGPBaseRibbonElement* pElem)
{
	Insert (pElem, (int) m_arElements.GetSize());
}
//****
void CBCGPRibbonPanel::AddSeparator()
{
	InsertSeparator ((int) m_arElements.GetSize());
}
//****
BOOL CBCGPRibbonPanel::Insert (CBCGPBaseRibbonElement* pElem, int nIndex)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	if (nIndex == -1)
	{
		nIndex = (int) m_arElements.GetSize();
	}

	if (nIndex < 0 || nIndex > m_arElements.GetSize())
	{
		return FALSE;
	}

	pElem->SetParentCategory (m_pParent);

	if (!pElem->IsAlignByColumn() && m_bAlignByColumn)
	{
		//---------------------------------------------------
		// If 2 or more elements are aligned by row, set this
		// flag for whole panel:
		//---------------------------------------------------
		for (int i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pListElem = m_arElements [i];
			ASSERT_VALID (pListElem);

			if (!pListElem->IsAlignByColumn())
			{
				m_bAlignByColumn = FALSE;
				break;
			}
		}
	}

	if (nIndex == m_arElements.GetSize())
	{
		m_arElements.Add (pElem);
	}
	else
	{
		m_arElements.InsertAt (nIndex, pElem);
	}

	pElem->m_bOnDialogBar = m_bOnDialogBar;
	return TRUE;
}
//****
BOOL CBCGPRibbonPanel::InsertSeparator (int nIndex)
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex > m_arElements.GetSize())
	{
		return FALSE;
	}

	CBCGPRibbonSeparator* pSeparator = new CBCGPRibbonSeparator;
	ASSERT_VALID (pSeparator);

	pSeparator->m_pParent = m_pParent;

	if (nIndex == m_arElements.GetSize())
	{
		m_arElements.Add (pSeparator);
	}
	else
	{
		m_arElements.InsertAt (nIndex, pSeparator);
	}

	return TRUE;
}
//****
CBCGPRibbonButtonsGroup* CBCGPRibbonPanel::AddToolBar (	
									UINT uiToolbarResID, UINT uiColdResID,
									UINT uiHotResID, UINT uiDisabledResID)
{
	ASSERT_VALID (this);

	//-------------------------------------------
	// Create temporary toolbar and load bitmaps:
	//-------------------------------------------
	CBCGPToolBar wndToolbar;
	if (!wndToolbar.LoadToolBar (uiToolbarResID, uiColdResID, 0, 
								TRUE, uiDisabledResID, 0, uiHotResID))
	{
		return NULL;
	}

	CBCGPToolBarImages* pImages = wndToolbar.GetLockedImages();
	CBCGPToolBarImages* pColdImages = wndToolbar.GetLockedColdImages();
	CBCGPToolBarImages* pDisabledImages = wndToolbar.GetLockedDisabledImages();
	CBCGPToolBarImages* pHotImages = NULL;

	if (pColdImages != NULL && pColdImages->GetCount() > 0)
	{
		pHotImages = uiHotResID != 0 ? pImages : NULL;
		pImages = pColdImages;
	}

	CList<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>	lstButtons;

	for (int i = 0; i < wndToolbar.GetCount(); i++)
	{
		CBCGPToolbarButton* pToolbarButton = wndToolbar.GetButton (i);
		ASSERT_VALID (pToolbarButton);

		if (pToolbarButton->m_nStyle & TBBS_SEPARATOR)
		{
			if (!lstButtons.IsEmpty())
			{
				CBCGPRibbonButtonsGroup* pGroup = new CBCGPRibbonButtonsGroup;

				pGroup->AddButtons (lstButtons);
				pGroup->SetImages (pImages, pHotImages, pDisabledImages);

				Add (pGroup);

				lstButtons.RemoveAll();
			}
		}
		else
		{
			CBCGPRibbonButton* pButton = new CBCGPRibbonButton;

			pButton->SetID (pToolbarButton->m_nID);
			pButton->SetText (pToolbarButton->m_strText);	// TODO
			pButton->m_nSmallImageIndex = pToolbarButton->GetImage();

			lstButtons.AddTail (pButton);
		}
	}

	if (!lstButtons.IsEmpty())
	{
		CBCGPRibbonButtonsGroup* pGroup = new CBCGPRibbonButtonsGroup;

		pGroup->AddButtons (lstButtons);
		pGroup->SetImages (pImages, pHotImages, pDisabledImages);

		Add (pGroup);
		return pGroup;
	}

	return NULL;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetElement (int nIndex) const
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= m_arElements.GetSize())
	{
		return NULL;
	}

	return m_arElements [nIndex];
}
//****
int CBCGPRibbonPanel::GetCount() const
{
	ASSERT_VALID (this);
	return (int) m_arElements.GetSize();
}
//****
BOOL CBCGPRibbonPanel::Remove (int nIndex, BOOL bDelete)
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= m_arElements.GetSize())
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CBCGPBaseRibbonElement* pElem = m_arElements [nIndex];
	ASSERT_VALID (pElem);

	if (pElem == m_pHighlighted)
	{
		m_pHighlighted = NULL;
	}

	m_arElements.RemoveAt (nIndex);

	if (bDelete)
	{
		delete pElem;
	}

	return TRUE;
}
//****
void CBCGPRibbonPanel::RemoveAll()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		delete m_arElements [i];
	}

	m_arElements.RemoveAll();
	m_bAlignByColumn = TRUE;
	m_pHighlighted = NULL;
}
//****
void CBCGPRibbonPanel::DoPaint (CDC* pDC)
{
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	CRect rectClip;
	pDC->GetClipBox (rectClip);

	CRect rectInter;

	if (!rectInter.IntersectRect (m_rect, rectClip))
	{
		return;
	}

	COLORREF clrTextOld = pDC->GetTextColor();

	//-----------------------
	// Fill panel background:
	//-----------------------
	COLORREF clrText = m_pParent == NULL || m_pPaletteButton != NULL ? 
		globalData.clrBarText :
		CBCGPVisualManager::GetInstance()->OnDrawRibbonPanel (
		pDC, this, m_rect, m_rectCaption);

	//--------------------
	// Draw panel caption:
	//--------------------
	if (!m_rectCaption.IsRectEmpty() &&
		rectInter.IntersectRect (m_rectCaption, rectClip))
	{
		CBCGPVisualManager::GetInstance()->OnDrawRibbonPanelCaption (
			pDC, this, m_rectCaption);
	}

	//--------------------
	// Draw launch button:
	//--------------------
	if (rectInter.IntersectRect (m_btnLaunch.GetRect(), rectClip))
	{
		m_btnLaunch.OnDraw (pDC);
	}

	pDC->SetTextColor (clrText);

	if (!m_btnDefault.GetRect().IsRectEmpty())
	{
		//----------------------------------------------
		// Panel is collapsed, draw default button only:
		//----------------------------------------------
		if (rectInter.IntersectRect (m_btnDefault.GetRect(), rectClip))
		{
			m_btnDefault.OnDraw (pDC);
		}
	}
	else if (m_pPaletteButton != NULL)
	{
		OnDrawPaletteMenu (pDC);
	}
	else
	{
		if (m_bIsDefaultMenuLook && m_pParentMenuBar != NULL)
		{
			ASSERT_VALID (m_pParentMenuBar);

			BOOL bDisableSideBarInXPMode = m_pParentMenuBar->m_bDisableSideBarInXPMode;
			m_pParentMenuBar->m_bDisableSideBarInXPMode = FALSE;

			CBCGPVisualManager::GetInstance()->OnFillBarBackground (
				pDC, m_pParentMenuBar, m_rect, m_rect);

			m_pParentMenuBar->m_bDisableSideBarInXPMode = bDisableSideBarInXPMode;
		}

		//---------------------
		// Draw panel elements:
		//---------------------
		for (int i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (rectInter.IntersectRect (pElem->GetRect(), rectClip))
			{
				pDC->SetTextColor (clrText);

				BOOL bIsHighlighted = pElem->m_bIsHighlighted;

				if (IsMenuMode() && pElem->IsDroppedDown() && m_pHighlighted == NULL)
				{
					pElem->m_bIsHighlighted = TRUE;
				}

				pElem->OnDraw (pDC);

				pElem->m_bIsHighlighted = bIsHighlighted;
			}
		}
	}

	pDC->SetTextColor (clrTextOld);
}
//****
void CBCGPRibbonPanel::OnDrawPaletteMenu (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pPaletteButton);

	const BOOL bNoSideBar = m_pPaletteButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonUndoButton));
	const BOOL bIsXPSideBar = !bNoSideBar && m_pPaletteButton->IsMenuSideBar();

	int i = 0;

	CRect rectIcons = m_rect;

	CRect rectSeparatorBottom;
	rectSeparatorBottom.SetRectEmpty();

	CRect rectSeparatorTop;
	rectSeparatorTop.SetRectEmpty();

	if (!m_rectMenuAreaBottom.IsRectEmpty())
	{
		if (m_pParentMenuBar != NULL && !bIsXPSideBar && !bNoSideBar)
		{
			BOOL bDisableSideBarInXPMode = m_pParentMenuBar->m_bDisableSideBarInXPMode;
			m_pParentMenuBar->m_bDisableSideBarInXPMode = FALSE;

			CBCGPVisualManager::GetInstance()->OnFillBarBackground (
				pDC, m_pParentMenuBar, m_rectMenuAreaBottom, m_rectMenuAreaBottom);

			m_pParentMenuBar->m_bDisableSideBarInXPMode = bDisableSideBarInXPMode;
		}

		rectSeparatorBottom = m_rectMenuAreaBottom;
		rectSeparatorBottom.t--;
		rectSeparatorBottom.b = rectSeparatorBottom.t + 1;

		rectIcons.b = m_rectMenuAreaBottom.t - 1;
	}

	if (!m_rectMenuAreaTop.IsRectEmpty())
	{
		if (m_pParentMenuBar != NULL && !bIsXPSideBar)
		{
			BOOL bDisableSideBarInXPMode = m_pParentMenuBar->m_bDisableSideBarInXPMode;
			m_pParentMenuBar->m_bDisableSideBarInXPMode = FALSE;

			CBCGPVisualManager::GetInstance()->OnFillBarBackground (
				pDC, m_pParentMenuBar, m_rectMenuAreaTop, m_rectMenuAreaTop);

			m_pParentMenuBar->m_bDisableSideBarInXPMode = bDisableSideBarInXPMode;
		}

		rectSeparatorTop = m_rectMenuAreaTop;
		rectSeparatorTop.b++;
		rectSeparatorTop.t = rectSeparatorTop.b - 1;

		rectIcons.t = m_rectMenuAreaTop.b + 1;
	}

	if (m_pParentMenuBar != NULL && bIsXPSideBar)
	{
		BOOL bDisableSideBarInXPMode = m_pParentMenuBar->m_bDisableSideBarInXPMode;
		m_pParentMenuBar->m_bDisableSideBarInXPMode = FALSE;

		CBCGPVisualManager::GetInstance()->OnFillBarBackground (
			pDC, m_pParentMenuBar, m_rect, m_rect);

		m_pParentMenuBar->m_bDisableSideBarInXPMode = bDisableSideBarInXPMode;
	}

	CRgn rgnClip;

	rgnClip.CreateRectRgnIndirect (rectIcons);
	pDC->SelectClipRgn (&rgnClip);

	CBCGPDrawState ds;

	if (m_pPaletteButton->m_imagesPalette.GetCount() > 0)
	{
		m_pPaletteButton->m_imagesPalette.SetTransparentColor (globalData.clrBtnFace);
		m_pPaletteButton->m_imagesPalette.PrepareDrawImage (ds, m_pPaletteButton->GetIconSize());
	}

	// First, draw icons + labels:
	for (i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
		BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

		if (bIsLabel || bIsIcon)
		{
			pElem->OnDraw (pDC);
		}
	}

	pDC->SelectClipRgn (NULL);

	// Draw rest of elements:
	for (i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
		BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

		if (!bIsLabel && !bIsIcon)
		{
			pElem->m_bIsDefaultMenuLook = TRUE;
			pElem->OnDraw (pDC);
		}
	}

	if (!rectSeparatorTop.IsRectEmpty())
	{
		CBCGPVisualManager::GetInstance()->OnDrawSeparator (pDC, 
			m_pParentMenuBar, rectSeparatorTop, FALSE);
	}

	if (!rectSeparatorBottom.IsRectEmpty())
	{
		CBCGPVisualManager::GetInstance()->OnDrawSeparator (pDC, 
			m_pParentMenuBar, rectSeparatorBottom, FALSE);
	}

	if (m_pPaletteButton->m_imagesPalette.GetCount() > 0)
	{
		m_pPaletteButton->m_imagesPalette.EndDrawImage (ds);
	}
}
//****
int CBCGPRibbonPanel::GetHeight (CDC* pDC) const
{
	const int nVertMargin = 3;

	ASSERT_VALID (pDC);

	((CBCGPRibbonPanel*)this)->m_btnDefault.OnCalcTextSize (pDC);

	int nRowHeight = 0;
	if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);

		TEXTMETRIC tm;
		pDC->GetTextMetrics (&tm);

		nRowHeight = max (m_pParent->GetImageSize (FALSE).h, tm.tmHeight) + 2 * nVertMargin;
	}

	int nMaxHeight = max (nRowHeight * m_nMaxRows, ((CBCGPRibbonPanel*)this)->m_btnDefault.GetRegularSize (pDC).h);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (pDC);
		nMaxHeight = max (nMaxHeight, pElem->GetRegularSize (pDC).h);
	}

	return nMaxHeight + 2 * m_nYMargin + nVertMargin;
}
//****
void CBCGPRibbonPanel::Repos (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	m_rectCaption.SetRectEmpty();
	m_rectMenuAreaTop.SetRectEmpty();
	m_rectMenuAreaBottom.SetRectEmpty();

	if (m_pPaletteButton != NULL)
	{
		ReposPalette (pDC, rect);
		return;
	}

	if (m_bMenuMode)
	{
		ReposMenu (pDC, rect);
		return;
	}

	m_btnDefault.m_pParent = m_pParent;
	m_btnLaunch.m_pParent = m_pParent;
	m_btnLaunch.m_pParentPanel = this;

	m_btnDefault.OnCalcTextSize (pDC);
	const int cxDefaultButton = m_btnDefault.GetRegularSize (pDC).w;

	m_rect = rect;

	m_btnLaunch.SetRect (CRect (0, 0, 0, 0));

	if (m_bForceCollpapse)
	{
		ASSERT (!m_bIsQATPopup);

		ShowDefaultButton (pDC);
		return;
	}

	m_btnDefault.SetRect (CRect (0, 0, 0, 0));

	m_nFullWidth = 0;
	m_nRows = 0;
	m_bShowCaption = TRUE;

	const CSize sizeCaption = GetCaptionSize (pDC);

	if (!m_bTrancateCaption)
	{
		m_rect.r = m_rect.l + max (rect.Width(), sizeCaption.w);
	}

	CSize size = rect.Size();
	size.w -= m_nXMargin;
	size.h -= sizeCaption.h + m_nYMargin;

	//---------------------------------------------
	// First, set all elements to the initial mode:
	//---------------------------------------------
	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (pDC);

		if (!m_bFloatyMode)
		{
			pElem->SetInitialMode();
		}
		else
		{
			pElem->SetCompactMode (TRUE);
		}

		pElem->m_bFloatyMode = m_bFloatyMode;
		pElem->m_nRow = -1;
	}

	//----------------------------------------------
	// Now, try to repos all elements inside bounds:
	//----------------------------------------------
	int x = 0;
	int y = 0;

	if (!m_bAlignByColumn || m_bFloatyMode)
	{
		int nRowHeight = 0;
		int nBottom = 0;
		int xStart = 0;
		int i = 0;
		BOOL bIsFullHeight = (m_rect.b == 32767);
		int cx = size.w;

		int cxFull = 0;

		if (!m_bIsCalcWidth && m_mapNonOptWidths.Lookup (cx, cxFull))
		{
			cx = cxFull;
		}

		CArray<int,int>	arRowWidths;

		if (!m_bFloatyMode)
		{
			//-----------------------------
			// Put all large buttons first:
			//-----------------------------
			BOOL bPrevLarge = FALSE;
			BOOL bPrevSeparator = FALSE;

			CSize sizePrevLargeButton (0, 0);

			for (i = 0; i < m_arElements.GetSize(); i++)
			{
				CBCGPBaseRibbonElement* pElem = m_arElements [i];
				ASSERT_VALID (pElem);

				CSize sizeElem = pElem->GetSize (pDC);

				BOOL bIsLargeButton = pElem->HasLargeMode() &&
					!pElem->m_bCompactMode && !pElem->m_bIntermediateMode;

				BOOL bDrawSeparator = FALSE;

				if (pElem->IsSeparator())
				{
					bDrawSeparator = bPrevLarge && !bPrevSeparator;
				}

				if (bIsLargeButton || bDrawSeparator)
				{
					if (pElem->IsSeparator())
					{
						if (sizePrevLargeButton != CSize (0, 0))
						{
							sizeElem.h = sizePrevLargeButton.h;
						}
					}
					else
					{
						sizePrevLargeButton = sizeElem;
					}

					CRect rectElem (
						CPoint (rect.l + x + m_nXMargin, rect.t + m_nYMargin), 
						CSize (sizeElem.w, bIsFullHeight ? sizeElem.h : size.h));

					pElem->SetRect (rectElem);
					pElem->m_nRow = 999;

					x += sizeElem.w + m_nXMargin;
					xStart = x;
					y = 0;
				}

				bPrevLarge = bIsLargeButton;
				bPrevSeparator = bDrawSeparator;
			}
		}

		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CSize sizeElem = pElem->GetSize (pDC);

			if (sizeElem == CSize (0, 0))
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				continue;
			}

			if (pElem->m_nRow != -1)
			{
				//--------------------
				// Already positioned
				//--------------------
				continue;
			}

			if (pElem->IsSeparator())
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				continue;
			}

			if (x + sizeElem.w + m_nXMargin - 1 > cx)
			{
				//------------------------------
				// We should start next row now:
				//------------------------------

				if (x == xStart)
				{
					ShowDefaultButton (pDC);
					return;
				}

				y += nRowHeight;

				if (m_bFloatyMode)
				{
					y += m_nYMargin;
				}

				arRowWidths.Add (x);

				m_nRows++;

				x = xStart;
				nRowHeight = 0;
			}

			if (y + sizeElem.h > size.h)
			{
				//------------------------------------------
				// Cannot repos elemnts: panel is too small:
				//------------------------------------------
				ShowDefaultButton (pDC);
				return;
			}

			CRect rectElem (CPoint (rect.l + x + m_nXMargin, rect.t + y + m_nYMargin), 
							sizeElem);

			pElem->SetRect (rectElem);
			pElem->m_nRow = m_nRows;

			nRowHeight = max (nRowHeight, sizeElem.h);
			x += sizeElem.w + m_nXMargin - 1;

			m_nFullWidth = max (m_nFullWidth, x - 1);
			
			nBottom = max (nBottom, rectElem.b);
		}

		arRowWidths.Add (x);
		m_nRows++;

		if (bIsFullHeight)
		{
			m_rect.b = nBottom + sizeCaption.h + m_nYMargin;
			size.h = m_rect.Height() - sizeCaption.h - m_nYMargin;
		}

		if (!m_bIsQATPopup && m_nRows > 1 && !m_bPreserveElementOrder)
		{
			//-----------------------------
			// Optimize elemnents location:
			//-----------------------------
			BOOL bRecalcFullWidth = FALSE;

			while (TRUE)
			{
				//-----------------
				// Find widest row:
				//-----------------
				int nMaxRowWidth = 0;
				int nMaxRowIndex = -1;

				for (i = 0; i < arRowWidths.GetSize(); i++)
				{
					if (arRowWidths [i] > nMaxRowWidth)
					{
						nMaxRowWidth = arRowWidths [i];
						nMaxRowIndex = i;
					}
				}

				if (nMaxRowIndex < 0)
				{
					break;
				}

				//-----------------------------------------
				// Find smallest element in the widest row:
				//-----------------------------------------
				int nMinWidth = 9999;
				CBCGPBaseRibbonElement* pMinElem = NULL;

				for (i = 0; i < m_arElements.GetSize(); i++)
				{
					CBCGPBaseRibbonElement* pElem = m_arElements [i];
					ASSERT_VALID (pElem);

					if (pElem->m_nRow == nMaxRowIndex)
					{
						CRect rectElem = pElem->GetRect();

						if (!rectElem.IsRectEmpty() && rectElem.Width() < nMinWidth)
						{
							nMinWidth = rectElem.Width();
							pMinElem = pElem;
						}
					}
				}

				if (pMinElem == NULL)
				{
					break;
				}

				//----------------------------------------
				// Try to move this elemnt to another row:
				//----------------------------------------
				BOOL bMoved = FALSE;

				for (i = nMaxRowIndex + 1; i < arRowWidths.GetSize(); i++)
				{
					if (arRowWidths [i] + nMinWidth < nMaxRowWidth)
					{
						//--------------------------------------
						// There is enough space in current row,
						// move element to here
						//--------------------------------------

						int x = 0;
						int y = 0;

						for (int j = 0; j < m_arElements.GetSize(); j++)
						{
							CBCGPBaseRibbonElement* pElem = m_arElements [j];
							ASSERT_VALID (pElem);

							if (pElem->m_nRow == i)
							{
								x = max (pElem->GetRect().r + m_nXMargin, x);
								y = pElem->GetRect().t;
							}
							else if (pElem->m_nRow == nMaxRowIndex)
							{
								CRect rectElem = pElem->GetRect();

								if (rectElem.l > pMinElem->GetRect().l)
								{
									rectElem.OffsetRect (-(nMinWidth + m_nXMargin), 0);
									pElem->SetRect (rectElem);
								}
							}
						}

						pMinElem->SetRect (CRect (CPoint (x, y), pMinElem->GetRect().Size()));
						pMinElem->m_nRow = i;

						arRowWidths [i] += nMinWidth;
						arRowWidths [nMaxRowIndex] -= nMinWidth;

						bRecalcFullWidth = TRUE;
						bMoved = TRUE;
						break;
					}
				}

				if (!bMoved)
				{
					break;
				}
			}

			if (bRecalcFullWidth)
			{
				m_nFullWidth = 0;

				for (i = 0; i < m_arElements.GetSize(); i++)
				{
					CBCGPBaseRibbonElement* pElem = m_arElements [i];
					ASSERT_VALID (pElem);

					m_nFullWidth = max (m_nFullWidth, pElem->GetRect().r);
				}

				m_nFullWidth -= m_rect.l + m_nXMargin;
			}
		}

		if (!bIsFullHeight && !m_bFloatyMode && m_nRows > 1)
		{
			//-----------------------------
			// Add some space between rows:
			//-----------------------------
			int yOffset = (size.h - m_nRows * nRowHeight) / (m_nRows + 1);
			if (yOffset > 0)
			{
				for (i = 0; i < m_arElements.GetSize(); i++)
				{
					CBCGPBaseRibbonElement* pElem = m_arElements [i];
					ASSERT_VALID (pElem);

					int nRow = pElem->m_nRow;
					CRect rectElem = pElem->GetRect();

					if (nRow != 999 && !rectElem.IsRectEmpty())
					{
						rectElem.OffsetRect (0, yOffset * (nRow + 1) - nRow);
						pElem->SetRect (rectElem);
					}
				}
			}
		}

		if (m_bIsQATPopup && nRowHeight > 0 && m_arElements.GetSize() > 0)
		{
			//--------------------------------------------------------------------
			// Last element (customize button) should occopy the whole row height:
			//--------------------------------------------------------------------
			CBCGPBaseRibbonElement* pElem = m_arElements [m_arElements.GetSize() - 1];
			ASSERT_VALID (pElem);

			CRect rectElem = pElem->GetRect();
			rectElem.b = rectElem.t + nRowHeight;

			pElem->SetRect (rectElem);
		}
	}
	else
	{
		const int nElementsInColumn = m_nMaxRows;

		while (TRUE)
		{
			int nColumnWidth = 0;
			int i = 0;
			x = 0;
			y = 0;

			CMap<int,int,int,int>	mapColumnElements;

			for (i = 0; i < m_arElements.GetSize(); i++)
			{
				CBCGPBaseRibbonElement* pElem = m_arElements [i];
				ASSERT_VALID (pElem);

				CSize sizeElem = pElem->GetSize (pDC);

				if (sizeElem == CSize (0, 0))
				{
					pElem->SetRect (CRect (0, 0, 0, 0));
					continue;
				}

				if (pElem->IsSeparator())
				{
					x += nColumnWidth;

					CRect rectSeparator (
						CPoint (rect.l + x + m_nXMargin, rect.t + m_nYMargin), 
						CSize (sizeElem.w, size.h));

					pElem->SetRect (rectSeparator);

					x += sizeElem.w + m_nXMargin;
					y = 0;
					nColumnWidth = 0;
					continue;
				}

				if (pElem->IsWholeRowHeight())
				{
					sizeElem.h = size.h;
				}

				if (y + sizeElem.h > size.h)
				{
					//---------------------------------
					// We should start next column now:
					//---------------------------------
					if (y == 0)
					{
						ShowDefaultButton (pDC);
						return;
					}

					x += nColumnWidth;
					y = 0;

					nColumnWidth = 0;
				}

				const int xColumn = rect.l + x + m_nXMargin;

				CRect rectElem (CPoint (xColumn, rect.t + y + m_nYMargin), 
								sizeElem);
				pElem->SetRect (rectElem);

				int nCount = 1;
				
				if (mapColumnElements.Lookup (xColumn, nCount))
				{
					nCount++;
				}

				mapColumnElements.SetAt (xColumn, nCount);

				nColumnWidth = max (nColumnWidth, sizeElem.w);
				y += sizeElem.h;
			}

			const int nFullWidth = x + nColumnWidth;

			if (nFullWidth <= size.w)
			{
				m_nFullWidth = nFullWidth;
				break;
			}

			if (nColumnWidth == 0)
			{
				ShowDefaultButton (pDC);
				return;
			}

			BOOL bChanged = FALSE;

			//-----------------------------------------------------
			// Find element that can can be stretched horizontally:
			//-----------------------------------------------------
			for (i = 0; i < m_arElements.GetSize() && !bChanged; i++)
			{
				CBCGPBaseRibbonElement* pElem = m_arElements [i];
				ASSERT_VALID (pElem);

				if (!pElem->GetRect().IsRectEmpty() &&
					pElem->CanBeStretchedHorizontally())
				{
					pElem->StretcheHorizontally();
					bChanged = TRUE;
				}
			}

			if (bChanged)
			{
				continue;
			}

			//------------------------------------------------------------------------------
			// Find 'nElementsInColumn' large buttons and make them intermediate or compact:
			//------------------------------------------------------------------------------
			int nLargeCount = 0;
			int nLargeTotal = min (nElementsInColumn, (int) m_arElements.GetSize());

			for (i = (int) m_arElements.GetSize() - 1; !bChanged && i >= 0; i--)
			{
				CBCGPBaseRibbonElement* pElem = m_arElements [i];
				ASSERT_VALID (pElem);

				if (pElem->GetRect().IsRectEmpty())
				{
					continue;
				}

				if (!pElem->IsLargeMode() || !pElem->CanBeCompacted())
				{
					nLargeCount = 0;
					continue;
				}

				nLargeCount++;

				if (nLargeCount == nLargeTotal)
				{
					bChanged = TRUE;

					for (int j = 0; j < nLargeCount; j++)
					{
						pElem = m_arElements [i + j];
						ASSERT_VALID (pElem);

						if (pElem->GetRect().IsRectEmpty())
						{
							j++;
						}
						else
						{
							pElem->SetCompactMode();
						}
					}
				}
			}

			if (bChanged)
			{
				continue;
			}

			//-----------------------------------------------------------------
			// Find 'nElementsInColumn' intermediate buttons in one column and 
			// make them compact:
			//-----------------------------------------------------------------
			int nIntermediateCount = 0;
			int xColumn = -1;

			for (i = (int) m_arElements.GetSize() - 1; !bChanged && i >= 0; i--)
			{
				CBCGPBaseRibbonElement* pElem = m_arElements [i];
				ASSERT_VALID (pElem);

				if (pElem->GetRect().IsRectEmpty())
				{
					continue;
				}

				if (xColumn != -1 && pElem->GetRect().l != xColumn)
				{
					nIntermediateCount = 0;
					xColumn = -1;
					continue;
				}

				xColumn = pElem->GetRect().l;

				if (!pElem->IsIntermediateMode() || !pElem->HasCompactMode())
				{
					nIntermediateCount = 0;
					xColumn = -1;
					continue;
				}

				nIntermediateCount++;

				if (nIntermediateCount == nElementsInColumn)
				{
					bChanged = TRUE;

					for (int j = 0; j < nIntermediateCount; j++)
					{
						pElem = m_arElements [i + j];
						ASSERT_VALID (pElem);

						if (pElem->GetRect().IsRectEmpty())
						{
							j++;
						}
						else
						{
							pElem->SetCompactMode();
						}
					}
				}
			}

			if (bChanged)
			{
				continue;
			}

			const int nStart = m_arElements.GetSize() < 3 ? 0 : 1;

			//--------------------------------------------------------------------
			// Find 1 large button near intermediate and make it intermediate too:
			//--------------------------------------------------------------------
			for (i = nStart; i < m_arElements.GetSize() && !bChanged; i++)
			{
				CBCGPBaseRibbonElement* pElem = m_arElements [i];
				ASSERT_VALID (pElem);

				if (pElem->GetRect().IsRectEmpty())
				{
					continue;
				}

				if (!pElem->IsLargeMode() || !pElem->CanBeCompacted())
				{
					continue;
				}

				if (i < m_arElements.GetSize() - 1 && m_arElements [i + 1]->m_bIntermediateMode)
				{
					int nColumnElements = 0;

					if (mapColumnElements.Lookup (m_arElements [i + 1]->GetRect().l, nColumnElements) &&
						nColumnElements < nElementsInColumn)
					{
						pElem->m_bIntermediateMode = TRUE;
						pElem->m_bCompactMode = FALSE;

						bChanged = TRUE;
					}
					break;
				}
			}

			if (bChanged)
			{
				continue;
			}

			//----------------------------------------------
			// Last step - try to compact rest of elements:
			//----------------------------------------------
			for (i = nStart; i < m_arElements.GetSize(); i++)
			{
				CBCGPBaseRibbonElement* pElem = m_arElements [i];
				ASSERT_VALID (pElem);

				if (!pElem->GetRect().IsRectEmpty() &&
					pElem->m_bIntermediateMode &&
					pElem->HasCompactMode())
				{
					pElem->m_bIntermediateMode = FALSE;
					pElem->m_bCompactMode = TRUE;
					bChanged = TRUE;
				}
			}

			if (bChanged)
			{
				continue;
			}

			ShowDefaultButton (pDC);
			return;
		}
	}

	if (m_bFloatyMode)
	{
		return;
	}

	if (m_bOnDialogBar && m_arElements.GetSize() == 1)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [0];
		ASSERT_VALID (pElem);

		if (pElem->CanBeStretchedOnDialogBar())
		{
			CRect rectElem = pElem->m_rect;
			
			if (!rectElem.IsRectEmpty())
			{
				rectElem.r = rectElem.l + size.w;
				pElem->SetRect (rectElem);
			}
		}
	}

	if (m_bAlignByColumn && (m_bCenterColumnVert || m_bJustifyColumns))
	{
		int nFirstInColumnIndex = -1;
		int nLastInColumnIndex = -1;
		int x = -1;

		for (int i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CRect rectElem = pElem->m_rect;
			if (rectElem.IsRectEmpty())
			{
				continue;
			}

			if (nFirstInColumnIndex == -1)
			{
				nLastInColumnIndex = nFirstInColumnIndex = i;
				x = rectElem.l;
			}

			if (x != rectElem.l)
			{
				if (m_bCenterColumnVert)
				{
					CenterElementsInColumn (nFirstInColumnIndex, nLastInColumnIndex, sizeCaption.h);
				}

				if (m_bJustifyColumns)
				{
					JustifyElementsInColumn (nFirstInColumnIndex, nLastInColumnIndex);
				}

				nLastInColumnIndex = nFirstInColumnIndex = i;
				x = rectElem.l;
			}
			else
			{
				nLastInColumnIndex = i;
			}
		}

		if (m_bCenterColumnVert)
		{
			CenterElementsInColumn (nFirstInColumnIndex, nLastInColumnIndex, sizeCaption.h);
		}

		if (m_bJustifyColumns)
		{
			JustifyElementsInColumn (nFirstInColumnIndex, nLastInColumnIndex);
		}
	}

	int nTotalWidth = !m_bAlignByColumn || m_bFloatyMode ? m_nFullWidth - 1 : CalcTotalWidth();

	if (nTotalWidth < sizeCaption.w && !m_bTrancateCaption)
	{
		//--------------------------------------------
		// Panel is too narrow: center it horizontaly:
		//--------------------------------------------
		const int xOffset = (sizeCaption.w - nTotalWidth) / 2;

		for (int i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CRect rectElem = pElem->m_rect;
			rectElem.OffsetRect (xOffset, 0);

			pElem->SetRect (rectElem);
		}

		nTotalWidth = sizeCaption.w;
	}

	if (m_arElements.GetSize() == 0)
	{
		m_nFullWidth = cxDefaultButton + m_nXMargin;
	}

	if (nTotalWidth < cxDefaultButton)
	{
		m_rect.r = m_rect.l + cxDefaultButton + m_nXMargin;
	}
	else
	{
		m_rect.r = m_rect.l + nTotalWidth + 2 * m_nXMargin;
	}

	//-----------------------------
	// Set launch button rectangle:
	//-----------------------------
	if (m_btnLaunch.GetID() > 0 && CBCGPToolBar::IsCommandPermitted (m_btnLaunch.GetID()))
	{
		CRect rectLaunch = m_rect;

		rectLaunch.DeflateRect (1, 1);

		rectLaunch.t = rectLaunch.b - sizeCaption.h + 1;
		rectLaunch.l = rectLaunch.r - sizeCaption.h;
		rectLaunch.b--;
		rectLaunch.r--;

		m_btnLaunch.SetRect (rectLaunch);
	}

	//-----------------------
	// Set caption rectangle:
	//-----------------------
	if (m_bShowCaption)
	{
		m_rectCaption = m_rect;
		m_rectCaption.t = m_rectCaption.b - sizeCaption.h - 1;
	}
}
//****
void CBCGPRibbonPanel::ReposMenu (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	m_bScrollDnAvailable = FALSE;

	m_nXMargin = 0;
	m_nYMargin = 0;

	CSize size = rect.Size();

	int y = 0;
	int i = 0;

	int nImageWidth = 0;

	if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		nImageWidth = m_pParent->GetImageSize (TRUE).w;
	}

	int nColumnWidth = 0;
	int yOffset = 0;

	for (i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (i == m_nScrollOffset)
		{
			yOffset = y;
		}

		pElem->OnCalcTextSize (pDC);
		pElem->SetCompactMode (FALSE);
		pElem->SetTextAlwaysOnRight();

		CSize sizeElem = pElem->GetSize (pDC);

		if (sizeElem == CSize (0, 0))
		{
			pElem->SetRect (CRect (0, 0, 0, 0));
			continue;
		}

		if (!rect.IsRectEmpty())
		{
			sizeElem.w = rect.Width();

			if (m_bIsDefaultMenuLook)
			{
				pElem->m_nImageOffset = CBCGPToolBar::GetMenuImageSize().w;
			}
		}

		CRect rectElem = CRect
			(CPoint (rect.l + m_nXMargin, rect.t + y + m_nYMargin), 
			sizeElem);

		pElem->SetRect (rectElem);

		nColumnWidth = max (nColumnWidth, sizeElem.w);
		y += sizeElem.h;
	}

	for (i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CRect rectElem = pElem->GetRect();

		if (yOffset > 0 && !rectElem.IsRectEmpty())
		{
			rectElem.OffsetRect (0, -yOffset);
			pElem->SetRect (rectElem);
		}

		if (rectElem.b > rect.b)
		{
			m_bScrollDnAvailable = TRUE;
		}
	}

	if (m_bIsDefaultMenuLook)
	{
		nColumnWidth += CBCGPToolBar::GetMenuImageSize().w + 
			2 * CBCGPVisualManager::GetInstance()->GetMenuImageMargin();
	}

	m_nFullWidth = nColumnWidth;

	if (rect.IsRectEmpty())
	{
		//----------------------------------------------
		// All menu elements should have the same width:
		//----------------------------------------------
		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			CRect rectElem = pElem->GetRect();

			if (!rectElem.IsRectEmpty())
			{
				rectElem.r = rectElem.l + nColumnWidth;
				
				if (nImageWidth > 0 && 
					pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
				{
					rectElem.l += nImageWidth + m_nXMargin;
				}

				pElem->SetRect (rectElem);
			}

			pElem->OnAfterChangeRect (pDC);
		}
	}

	m_rect = rect;
	m_rect.b = m_rect.t + y;
	m_rect.r = m_rect.l + m_nFullWidth;
}
//****
void CBCGPRibbonPanel::ReposPalette (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pScrollBar);
	ASSERT_VALID (m_pPaletteButton);

	const int yOffset = 4;

	if (rect == CRect (0, 0, 0, 0))
	{
		return;
	}

	const BOOL bNoSideBar = m_pPaletteButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonUndoButton));

	BOOL bShowAllItems = FALSE;

	m_nScrollOffset = 0;

	CRect rectInitial = rect;

	if (rectInitial.b <= 0)
	{
		rectInitial.b = rectInitial.t + 32676;
		bShowAllItems = TRUE;
	}

	m_nXMargin = 0;
	m_nYMargin = 0;

	const int cxScroll = bShowAllItems && !m_pPaletteButton->IsMenuResizeEnabled() ? 
		0 : ::GetSystemMetrics (SM_CXVSCROLL);

	int nScrollTotal = 0;

	int x = rectInitial.l;
	int y = rectInitial.t;

	m_rect = rectInitial;

	if (m_bSizeIsLocked)
	{
		rectInitial.r -= cxScroll;
	}
	else
	{
		m_rect.r += cxScroll;
	}

	int i = 0;
	BOOL bHasBottomItems = FALSE;
	BOOL bHasTopItems = FALSE;

	int nMaxItemHeight = 0;
	int nMaxImageWidth = 0;
		
	if (m_bSizeIsLocked)
	{
		int i = 0;

		// Calculate the total "bottom" elements height:
		int nBottomHeight = 0;

		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
			BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

			if (bIsIcon || bIsLabel || pElem->m_bIsOnPaletteTop)
			{
				continue;
			}

			pElem->OnCalcTextSize (pDC);
			pElem->SetCompactMode (TRUE);

			if (pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageLarge) == CSize (0, 0))
			{
				pElem->SetCompactMode (FALSE);
			}

			pElem->SetTextAlwaysOnRight();

			nBottomHeight += pElem->GetSize (pDC).h;
		}

		// Find all menu items and place them on top/bottom:
		int yTop = rectInitial.t;
		int yBottom = rectInitial.b - nBottomHeight;

		rectInitial.b = yBottom - yOffset;

		m_rectMenuAreaTop = m_rect;
		m_rectMenuAreaBottom = m_rect;
		m_rectMenuAreaBottom.t = yBottom;

		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
			BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

			if (bIsIcon || bIsLabel)
			{
				continue;
			}

			if (pElem->m_bIsOnPaletteTop)
			{
				bHasTopItems = TRUE;
			}
			else
			{
				bHasBottomItems = TRUE;
			}

			pElem->OnCalcTextSize (pDC);
			pElem->SetCompactMode (TRUE);

			if (pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageLarge) == CSize (0, 0))
			{
				pElem->SetCompactMode (FALSE);
			}

			pElem->SetTextAlwaysOnRight();

			CSize sizeElem = pElem->GetSize (pDC);
			sizeElem.w = m_rect.Width();

			CRect rectElem (0, 0, 0, 0);

			if (pElem->m_bIsOnPaletteTop)
			{
				rectElem = CRect (CPoint (rectInitial.l, yTop), sizeElem);

				yTop += sizeElem.h;
				rectInitial.t = yTop + yOffset;
				m_rectMenuAreaTop.b = yTop;

				y += sizeElem.h;
			}
			else
			{
				rectElem = CRect (CPoint (rectInitial.l, yBottom), sizeElem);
				yBottom += sizeElem.h;
			}

			pElem->SetRect (rectElem);
		}
	}
	else
	{
		// Repos all top items:
		m_rectMenuAreaTop = m_rect;

		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)) ||
				pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel)))
			{
				continue;
			}

			pElem->OnCalcTextSize (pDC);
			pElem->SetCompactMode (TRUE);

			if (pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageLarge) == CSize (0, 0))
			{
				pElem->SetCompactMode (FALSE);
			}

			pElem->SetTextAlwaysOnRight();

			CSize sizeElem = pElem->GetSize (pDC);
			sizeElem.w += 2 * TEXT_MARGIN;

			nMaxItemHeight = max (nMaxItemHeight, sizeElem.h);
			nMaxImageWidth = max (nMaxImageWidth, 
				pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageSmall).w);
		}

		if (nMaxImageWidth == 0 && !bNoSideBar)
		{
			nMaxImageWidth = CBCGPToolBar::GetMenuImageSize().w + 
				2 * CBCGPVisualManager::GetInstance()->GetMenuImageMargin();

			if (m_pParent != NULL)
			{
				nMaxImageWidth = m_pParent->GetImageSize (FALSE).w;
			}
		}

		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)) ||
				pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel)) ||
				!pElem->m_bIsOnPaletteTop)
			{
				continue;
			}

			CSize sizeElem = pElem->GetSize (pDC);

			if (sizeElem == CSize (0, 0))
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				continue;
			}

			pElem->m_nImageOffset = nMaxImageWidth;

			sizeElem.w = m_rect.Width();
			sizeElem.h = nMaxItemHeight;

			CRect rectElem = CRect
				(CPoint (rectInitial.l, rectInitial.t + y), 
				sizeElem);

			pElem->SetRect (rectElem);

			y += sizeElem.h;
		}

		m_rectMenuAreaTop.b = y;
	}

	// Set palette icons location:
	int yNextLine = m_rect.b;
	BOOL bIsFirstPaletteElem = TRUE;

	if (!m_bSizeIsLocked)
	{
		m_rect.b = y;
	}

	for (i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
		BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

		if (!bIsIcon && !bIsLabel)
		{
			if (pElem->m_bIsOnPaletteTop)
			{
				bHasTopItems = TRUE;
			}
			else
			{
				bHasBottomItems = TRUE;
			}
		}

		CSize sizeElem (0, 0);

		if (bIsLabel)
		{
			if (x > rectInitial.l)
			{
				y = yNextLine;
			}

			if (i > 0)
			{
				y++;
			}

			CString strLabel = pElem->GetText();
			CRect rectElem (0, 0, 0, 0);

			if (strLabel.IsEmpty())
			{
				if (!bIsFirstPaletteElem)
				{
					y += m_pPaletteButton->GetGroupOffset();
				}
			}
			else
			{
				pElem->OnCalcTextSize (pDC);

				sizeElem = pElem->GetSize (pDC);
				sizeElem.w = rectInitial.Width();

				rectElem = CRect
					(CPoint (rectInitial.l, y), sizeElem);

				y += sizeElem.h + m_pPaletteButton->GetGroupOffset();
				
				bIsFirstPaletteElem = FALSE;
			}

			pElem->SetRect (rectElem);

			if (!m_bSizeIsLocked)
			{
				m_rect.b = rectElem.b;
			}

			nScrollTotal = yNextLine = rectElem.b;

			x = rectInitial.l;
		}
		else if (bIsIcon)
		{
			bIsFirstPaletteElem = FALSE;

			pElem->SetCompactMode (FALSE);

			sizeElem = pElem->GetSize (pDC);

			if (x + sizeElem.w > rectInitial.r && x != rectInitial.l)
			{
				x = rectInitial.l;
				y += sizeElem.h;
			}

			CRect rectElem = CRect (CPoint (x, y), sizeElem);

			pElem->SetRect (rectElem);

			if (!m_bSizeIsLocked)
			{
				m_rect.b = rectElem.b;
			}

			nScrollTotal = yNextLine = rectElem.b;

			x += sizeElem.w;
		}
	}

	if (!m_bSizeIsLocked)
	{
		m_rect.b = min (m_rect.b, rectInitial.b);
	}

	m_nFullWidth = m_rect.Width();

	if (bHasBottomItems && !m_bSizeIsLocked)
	{
		// Set menu items location (on bottom):
		y = m_rect.b + yOffset;
		m_rectMenuAreaBottom = m_rect;
		m_rectMenuAreaBottom.t = y;

		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)) ||
				pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel)) ||
				pElem->m_bIsOnPaletteTop)
			{
				continue;
			}

			CSize sizeElem = pElem->GetSize (pDC);

			if (sizeElem == CSize (0, 0))
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				continue;
			}

			pElem->m_nImageOffset = nMaxImageWidth;

			sizeElem.w = m_rect.Width();
			sizeElem.h = nMaxItemHeight;

			CRect rectElem = CRect
				(CPoint (rectInitial.l, rectInitial.t + y), 
				sizeElem);

			m_rect.b = rectElem.b;

			pElem->SetRect (rectElem);

			y += sizeElem.h;
		}

		m_rectMenuAreaBottom.b = y;
	}

	if (!bHasBottomItems)
	{
		m_rectMenuAreaBottom.SetRectEmpty();
	}

	if (!bHasTopItems)
	{
		m_rectMenuAreaTop.SetRectEmpty();
	}

	// Define icon postions in matrix:
	for (i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (CBCGPRibbonPaletteIcon, m_arElements [i]);
		if (pIcon == NULL)
		{
			continue;
		}

		ASSERT_VALID (pIcon);

		pIcon->m_bIsFirstInRow = FALSE;
		pIcon->m_bIsLastInRow = FALSE;
		pIcon->m_bIsFirstInColumn = FALSE;
		pIcon->m_bIsLastInColumn = FALSE;

		CRect rectIcon = pIcon->GetRect();

		if (rectIcon.IsRectEmpty())
		{
			continue;
		}

		pIcon->m_bIsFirstInRow = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, HitTest (CPoint (rectIcon.l - 2, rectIcon.CenterPoint().y))) == NULL;

		pIcon->m_bIsLastInRow = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, HitTest (CPoint (rectIcon.r + 2, rectIcon.CenterPoint().y))) == NULL;

		pIcon->m_bIsFirstInColumn = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, HitTest (CPoint (rectIcon.CenterPoint().x, rectIcon.t - 2))) == NULL;

		pIcon->m_bIsLastInColumn = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, HitTest (CPoint (rectIcon.CenterPoint().x, rectIcon.b + 2))) == NULL;
	}

	if (!bShowAllItems || m_pPaletteButton->IsMenuResizeEnabled())
	{
		const int ySB = bHasTopItems ? m_rectMenuAreaTop.b + 1 : rectInitial.t;
		const int ySBBottom = bHasBottomItems ? m_rectMenuAreaBottom.t - 1 : m_rect.b;

		m_pScrollBar->SetWindowPos (NULL, 
			m_rect.r - cxScroll, ySB,
			cxScroll, ySBBottom - ySB - 1,
			SWP_NOZORDER | SWP_NOACTIVATE);

		SCROLLINFO si;

		ZeroMemory (&si, sizeof (SCROLLINFO));
		si.cbSize = sizeof (SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;

		si.nMin = 0;

		nScrollTotal -= m_rectMenuAreaTop.Height();

		if (nScrollTotal > rectInitial.Height())
		{
			si.nMax = nScrollTotal;
			si.nPage = rectInitial.Height();

			m_pScrollBar->SetScrollInfo (&si, TRUE);
			m_pScrollBar->EnableScrollBar (ESB_ENABLE_BOTH);
			m_pScrollBar->EnableWindow();
		}
		else if (!bShowAllItems)
		{
			m_pScrollBar->EnableScrollBar (ESB_DISABLE_BOTH);
		}
	}
}
//****
void CBCGPRibbonPanel::ShowDefaultButton (CDC* pDC)
{
	//-------------------------------
	// Show panel default button only
	//-------------------------------
	const int cxDefaultButton = m_btnDefault.GetRegularSize (pDC).w;

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->SetRect (CRect (0, 0, 0, 0));
	}

	m_rect.r = m_rect.l + cxDefaultButton;

	m_btnDefault.SetRect (m_rect);
	m_nRows = 0;
	m_bShowCaption = FALSE;
	m_bForceCollpapse = FALSE;
}
//****
int CBCGPRibbonPanel::CalcTotalWidth()
{
	//------------------------------------------------------------
	// Total width will be right edge of the last visible element
	// in the right column
	//------------------------------------------------------------
	int xRight = 0;

	for (int i = (int) m_arElements.GetSize() - 1; i >= 0; i--)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->m_rect.IsRectEmpty())
		{
			continue;
		}

		xRight = max (xRight, pElem->m_rect.r);
	}

	return max (0, xRight - m_rect.l - m_nXMargin / 2 - 1);
}
//****
CSize CBCGPRibbonPanel::GetCaptionSize (CDC* pDC) const
{
	ASSERT_VALID (pDC);

	if (m_bFloatyMode)
	{
		return CSize (0, 0);
	}

	CSize size = pDC->GetTextExtent (m_strName.IsEmpty() ? _T(" ") : m_strName);

	size.h += m_nYMargin + 1;

	if (m_btnLaunch.GetID() > 0)
	{
		size.w += size.h + 1;
	}

	return size;
}
//****
void CBCGPRibbonPanel::CenterElementsInColumn (int nFirstInColumnIndex, int nLastInColumnIndex,
											   int nCaptionHeight)
{
	if (nFirstInColumnIndex > nLastInColumnIndex ||
		nFirstInColumnIndex < 0 ||
		nLastInColumnIndex < 0)
	{
		return;
	}

	//------------------------------------------
	// Center all elements in column vertically:
	//------------------------------------------
	CBCGPBaseRibbonElement* pLastElem = m_arElements [nLastInColumnIndex];
	ASSERT_VALID (pLastElem);

	const int nColumnHeight = m_rect.Height() - nCaptionHeight - 2 * m_nYMargin;
	const int nTotalHeight = pLastElem->m_rect.b - m_rect.t - m_nYMargin;
	const int nOffset = max (0, (nColumnHeight - nTotalHeight) / 2);

	for (int i = nFirstInColumnIndex; i <= nLastInColumnIndex; i++)
	{
		CBCGPBaseRibbonElement* pColumnElem = m_arElements [i];
		ASSERT_VALID (pColumnElem);

		CRect rectElem = pColumnElem->m_rect;
		rectElem.OffsetRect (0, nOffset);
		pColumnElem->SetRect (rectElem);
	}
}
//****
void CBCGPRibbonPanel::JustifyElementsInColumn (int nFirstInColumnIndex, int nLastInColumnIndex)
{
	if (nFirstInColumnIndex > nLastInColumnIndex ||
		nFirstInColumnIndex < 0 ||
		nLastInColumnIndex < 0)
	{
		return;
	}

	//-------------------------------------------------
	// Set same width (largets) to all column elements:
	//-------------------------------------------------
	int nColumnWidth = 0;
	int i = 0;

	for (i = nFirstInColumnIndex; i <= nLastInColumnIndex; i++)
	{
		CBCGPBaseRibbonElement* pColumnElem = m_arElements [i];
		ASSERT_VALID (pColumnElem);

		nColumnWidth = max (nColumnWidth, pColumnElem->m_rect.Width());
	}

	for (i = nFirstInColumnIndex; i <= nLastInColumnIndex; i++)
	{
		CBCGPBaseRibbonElement* pColumnElem = m_arElements [i];
		ASSERT_VALID (pColumnElem);

		CRect rectElem = pColumnElem->m_rect;
		rectElem.r = rectElem.l + nColumnWidth;

		pColumnElem->SetRect (rectElem);
	}
}
//****
void CBCGPRibbonPanel::RecalcWidths (CDC* pDC, int nHeight)
{
	ASSERT_VALID (pDC);

	CRect rectScreen;
	::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);

	m_btnDefault.OnCalcTextSize (pDC);

	const int cxDefaultButton = m_btnDefault.GetRegularSize (pDC).w;

	m_arWidths.RemoveAll();
	m_mapNonOptWidths.RemoveAll();

	m_nCurrWidthIndex = 0;
	m_bIsCalcWidth = TRUE;

	int nLastWidth = -1;
	const int dx = 16;

	if (m_bAlignByColumn && !m_bFloatyMode)
	{
		CRect rect (0, 0, 32767, nHeight);

		do
		{
			Repos (pDC, rect);

			if (!m_bShowCaption)
			{
				break;
			}

			if (nLastWidth == -1 || m_nFullWidth < nLastWidth)
			{
				nLastWidth = m_nFullWidth;

				if (nLastWidth <= cxDefaultButton ||
					(nLastWidth <= 3 * cxDefaultButton / 2 && m_arElements.GetSize() == 1))
				{
					if (m_arWidths.GetSize() == 0)
					{
						//-----------------------------------------------------------
						// Panel has onle one layout and it smaller then collapsed.
						// Use this layout only and don't allow to collapse the panel
						//-----------------------------------------------------------
						m_arWidths.Add (nLastWidth);
						m_bIsCalcWidth = FALSE;
						return;
					}

					break;
				}

				m_arWidths.Add (nLastWidth);
				rect.r = nLastWidth - dx;
			}
			else
			{
				rect.r -= dx;
			}
		}
		while (rect.Width() > 2 * m_nXMargin);
	}
	else if (m_bIsQATPopup)
	{
		CRect rect (0, 0, rectScreen.Width() - 10, nHeight);

		Repos (pDC, rect);
		m_arWidths.Add (m_nFullWidth);
	}
	else
	{
		const int nMaxRows = m_bIsQATPopup ? 50 : m_nMaxRows;

		for (int nRows = 1; nRows <= nMaxRows; nRows++)
		{
			CRect rect (0, 0, cxDefaultButton + 1, nHeight);

			for (;; rect.r += dx)
			{
				if (rect.Width() >= rectScreen.Width())
				{
					break;
				}

				Repos (pDC, rect);

				if (nLastWidth != -1 && m_nFullWidth > nLastWidth)
				{
					break;
				}

				if (m_nRows == nRows && m_nFullWidth > 0)
				{
					if (m_nRows == nMaxRows - 1 && !m_bFloatyMode)
					{
						//------------------------
						// Don't add 1 row layout:
						//------------------------
						m_arWidths.RemoveAll();
					}

					m_arWidths.Add (m_nFullWidth);
					m_mapNonOptWidths.SetAt (m_nFullWidth + m_nXMargin, rect.Width());
					nLastWidth = m_nFullWidth;
					break;
				}
			}
		}
	}

	m_arWidths.Add (cxDefaultButton);

	m_bIsCalcWidth = FALSE;
}
//****
int CBCGPRibbonPanel::GetMinWidth (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	m_btnDefault.OnCalcTextSize (pDC);
	return m_btnDefault.GetRegularSize (pDC).w;
}
//****
void CBCGPRibbonPanel::Highlight (BOOL bHighlight, CPoint point)
{
	ASSERT_VALID (this);

	BOOL bRedrawPanel = m_bIsHighlighted != bHighlight;

	m_bIsHighlighted = bHighlight;

	CBCGPBaseRibbonElement* pHighlighted = NULL;
	if (bHighlight)
	{
		pHighlighted = HitTest (point);

		if (pHighlighted != NULL)
		{
			ASSERT_VALID (pHighlighted);
			pHighlighted->OnMouseMove (point);
		}
	}

	BOOL bNotifyParent = FALSE;
	BOOL bSetFocus = FALSE;

	if (pHighlighted != m_pHighlighted)
	{
		if (m_pParent != NULL && m_pParent->GetParentRibbonBar() != NULL &&
			pHighlighted != NULL)
		{
			m_pParent->GetParentRibbonBar()->PopTooltip();
		}

		if (m_pParentMenuBar != NULL)
		{
			ASSERT_VALID (m_pParentMenuBar);
			m_pParentMenuBar->PopTooltip();
		}

		if (m_pHighlighted != NULL)
		{
			ASSERT_VALID (m_pHighlighted);

			m_pHighlighted->m_bIsHighlighted = FALSE;
			m_pHighlighted->OnHighlight (FALSE);

			if (IsMenuMode() && m_pHighlighted->m_bIsFocused)
			{
				bSetFocus = TRUE;
				m_pHighlighted->m_bIsFocused = FALSE;
				m_pHighlighted->OnSetFocus (FALSE);
			}

			RedrawElement (m_pHighlighted);
			m_pHighlighted = NULL;
		}

		bNotifyParent = TRUE;
	}

	if (pHighlighted != NULL)
	{
		ASSERT_VALID (pHighlighted);

		m_pHighlighted = pHighlighted;

		if (!m_pHighlighted->m_bIsHighlighted)
		{
			m_pHighlighted->OnHighlight (TRUE);
			m_pHighlighted->m_bIsHighlighted = TRUE;

			if (bSetFocus)
			{
				m_pHighlighted->m_bIsFocused = TRUE;
				m_pHighlighted->OnSetFocus (TRUE);
			}

			RedrawElement (m_pHighlighted);
		}
	}

	if (bRedrawPanel && m_pParent != NULL && GetParentWnd() != NULL)
	{
		GetParentWnd()->RedrawWindow (m_rect);
	}

	if (m_bFloatyMode && bRedrawPanel)
	{
		ASSERT_VALID (m_pParentMenuBar);
		m_pParentMenuBar->SetActive (m_bIsHighlighted);
	}

	if (bNotifyParent)
	{
		if (m_pParentMenuBar != NULL)
		{
			ASSERT_VALID (m_pParentMenuBar);
			m_pParentMenuBar->OnChangeHighlighted (m_pHighlighted);
		}
	}
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::HitTest (CPoint point, BOOL bCheckPanelCaption)
{
	if (!m_btnDefault.m_rect.IsRectEmpty() &&
		m_btnDefault.m_rect.PtInRect (point))
	{
		return &m_btnDefault;
	}

	if (!m_btnLaunch.m_rect.IsRectEmpty() &&
		m_btnLaunch.m_rect.PtInRect (point))
	{
		return &m_btnLaunch;
	}

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (!pElem->m_rect.IsRectEmpty() &&
			pElem->m_rect.PtInRect (point))
		{
			return pElem->HitTest (point);
		}
	}

	if (bCheckPanelCaption && m_rectCaption.PtInRect (point))
	{
		return &m_btnDefault;
	}

	return NULL;
}
int CBCGPRibbonPanel::HitTestEx (CPoint point)
{
	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (!pElem->m_rect.IsRectEmpty() &&
			pElem->m_rect.PtInRect (point))
		{
			return i;
		}
	}

	return -1;
}
//****
int CBCGPRibbonPanel::GetIndex (CBCGPBaseRibbonElement* pElem)
{
	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pNextElem = m_arElements [i];
		ASSERT_VALID (pNextElem);

		if (pNextElem == pElem)
		{
			return i;
		}
	}

	return -1;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::MouseButtonDown (CPoint point)
{
	ASSERT_VALID (this);

	if (m_pHighlighted != NULL)
	{
		ASSERT_VALID (m_pHighlighted);

		BOOL bSetPressed = TRUE;

		if (m_pHighlighted->HasMenu())
		{
			CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (CBCGPRibbonButton, m_pHighlighted);
			if (pButton != NULL)
			{
				ASSERT_VALID (pButton);

				const CRect rectCmd = pButton->GetCommandRect();
				bSetPressed = !rectCmd.IsRectEmpty() && rectCmd.PtInRect (point);
			}
		}

		if (bSetPressed)
		{
			m_pHighlighted->m_bIsPressed = TRUE;
			RedrawElement (m_pHighlighted);
		}

		HWND hwndMenu = m_pParentMenuBar->GetSafeHwnd();

		m_pHighlighted->OnLButtonDown (point);

		if (hwndMenu != NULL && !::IsWindow (hwndMenu))
		{
			return NULL;
		}
	}

	return m_pHighlighted;
}
//****
void CBCGPRibbonPanel::MouseButtonUp (CPoint point)
{
	ASSERT_VALID (this);

	if (m_pHighlighted != NULL)
	{
		ASSERT_VALID (m_pHighlighted);

		HWND hwndParent = GetParentWnd()->GetSafeHwnd();

		CBCGPBaseRibbonElement* pHighlighted = m_pHighlighted;
		m_pHighlighted->OnLButtonUp (point);

		if (::IsWindow (hwndParent) && pHighlighted->m_bIsPressed)
		{
			pHighlighted->m_bIsPressed = FALSE;
			RedrawElement (pHighlighted);

			if (m_pHighlighted != NULL && m_pHighlighted != pHighlighted)
			{
				RedrawElement (m_pHighlighted);
			}
		}
	}
}
//****
void CBCGPRibbonPanel::CancelMode()
{
	ASSERT_VALID (this);

	if (m_pHighlighted != NULL)
	{
		ASSERT_VALID (m_pHighlighted);

		m_pHighlighted->m_bIsHighlighted = FALSE;
		m_pHighlighted->OnHighlight (FALSE);
		m_pHighlighted->m_bIsPressed = FALSE;
		m_pHighlighted->m_bIsFocused = FALSE;
		m_pHighlighted->OnSetFocus (FALSE);

		RedrawElement (m_pHighlighted);
		m_pHighlighted = NULL;
	}

	if (m_bIsHighlighted)
	{
		m_bIsHighlighted = FALSE;

		if (GetParentWnd()->GetSafeHwnd() != NULL)
		{
			GetParentWnd()->RedrawWindow (m_rect);
		}
	}
}
//****
void CBCGPRibbonPanel::OnUpdateCmdUI (CBCGPRibbonCmdUI* pCmdUI,
									  CFrameWnd* pTarget,
									  BOOL bDisableIfNoHndler)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnUpdateCmdUI (pCmdUI, pTarget, bDisableIfNoHndler);
	}

	m_btnLaunch.OnUpdateCmdUI (pCmdUI, pTarget, bDisableIfNoHndler);
}
//****
BOOL CBCGPRibbonPanel::NotifyControlCommand (
	BOOL bAccelerator, int nNotifyCode, WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->NotifyControlCommand (bAccelerator, nNotifyCode, wParam, lParam))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//****
void CBCGPRibbonPanel::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnShow (!pElem->GetRect().IsRectEmpty());
		pElem->OnAfterChangeRect (pDC);
	}

	m_btnDefault.OnShow (!m_btnDefault.GetRect().IsRectEmpty());
	m_btnDefault.OnAfterChangeRect (pDC);

	m_btnLaunch.OnAfterChangeRect (pDC);
}
//****
void CBCGPRibbonPanel::OnShow (BOOL bShow)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnShow (bShow && !pElem->GetRect().IsRectEmpty());
	}
}
//****
BOOL CBCGPRibbonPanel::IsCollapsed() const
{
	ASSERT_VALID (this);

	return !m_btnDefault.GetRect().IsRectEmpty();
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::FindByID (UINT uiCmdID) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElemFromList = m_arElements [i];
		ASSERT_VALID (pElemFromList);

		CBCGPBaseRibbonElement* pElem = pElemFromList->FindByID (uiCmdID);
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}

	CBCGPBaseRibbonElement* pElem = ((CBCGPRibbonPanel*) this)->m_btnLaunch.FindByID (uiCmdID);
	if (pElem != NULL)
	{
		ASSERT_VALID (pElem);
		return pElem;
	}

	if (m_btnDefault.GetID() == uiCmdID)
	{
		return &(((CBCGPRibbonPanel*) this)->m_btnDefault);
	}

	return NULL;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::FindByData (DWORD_PTR dwData) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElemFromList = m_arElements [i];
		ASSERT_VALID (pElemFromList);

		CBCGPBaseRibbonElement* pElem = pElemFromList->FindByData (dwData);
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}

	CBCGPBaseRibbonElement* pElem = ((CBCGPRibbonPanel*) this)->m_btnLaunch.FindByData (dwData);
	if (pElem != NULL)
	{
		ASSERT_VALID (pElem);
		return pElem;
	}

	if (m_btnDefault.GetData() == dwData)
	{
		return &(((CBCGPRibbonPanel*) this)->m_btnDefault);
	}

	return NULL;
}
//****
BOOL CBCGPRibbonPanel::SetElementMenu (UINT uiCmdID, HMENU hMenu, 
	BOOL bIsDefautCommand, BOOL bRightAlign)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonButton, FindByID (uiCmdID));

	if (pButton == NULL)
	{
		TRACE(_T("Cannot find element with ID: %d\n"), uiCmdID);
		return FALSE;
	}

	ASSERT_VALID (pButton);
	pButton->SetMenu (hMenu, bIsDefautCommand, bRightAlign);

	return TRUE;
}
//****
BOOL CBCGPRibbonPanel::SetElementMenu (UINT uiCmdID, UINT uiMenuResID,
	BOOL bIsDefautCommand, BOOL bRightAlign)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonButton, FindByID (uiCmdID));

	if (pButton == NULL)
	{
		TRACE(_T("Cannot find element with ID: %d\n"), uiCmdID);
		return FALSE;
	}

	ASSERT_VALID (pButton);
	pButton->SetMenu (uiMenuResID, bIsDefautCommand, bRightAlign);

	return TRUE;
}
//****
BOOL CBCGPRibbonPanel::Replace (int nIndex, CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	if (nIndex < 0 || nIndex >= m_arElements.GetSize())
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CBCGPBaseRibbonElement* pOldElem = m_arElements [nIndex];
	ASSERT_VALID (pOldElem);

	pElem->CopyFrom (*pOldElem);
	m_arElements [nIndex] = pElem;

	delete pOldElem;
	return TRUE;
}
//****
BOOL CBCGPRibbonPanel::ReplaceByID (UINT uiCmdID, CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	if (uiCmdID == 0 || uiCmdID == (UINT)-1)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElemFromList = m_arElements [i];
		ASSERT_VALID (pElemFromList);

		if (pElemFromList->GetID() == uiCmdID)
		{
			return Replace (i, pElem);
		}

		if (pElemFromList->ReplaceByID (uiCmdID, pElem))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::SetElementRTC (int nIndex, 
														 CRuntimeClass* pRTC)
{
	ASSERT_VALID (this);
	ASSERT (pRTC != NULL);

	if (!pRTC->IsDerivedFrom (RUNTIME_CLASS (CBCGPBaseRibbonElement)))
	{
		ASSERT (FALSE);
		return NULL;
	}

	CBCGPBaseRibbonElement* pNewElement = DYNAMIC_DOWNCAST (
		CBCGPBaseRibbonElement, pRTC->CreateObject());
	ASSERT_VALID (pNewElement);

	if (!Replace (nIndex, pNewElement))
	{
		delete pNewElement;
	}

	return pNewElement;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::SetElementRTCByID (UINT uiCmdID, 
	CRuntimeClass* pRTC)
{
	ASSERT_VALID (this);
	ASSERT (pRTC != NULL);

	if (!pRTC->IsDerivedFrom (RUNTIME_CLASS (CBCGPBaseRibbonElement)))
	{
		ASSERT (FALSE);
		return NULL;
	}

	CBCGPBaseRibbonElement* pNewElement = DYNAMIC_DOWNCAST (
		CBCGPBaseRibbonElement, pRTC->CreateObject());
	ASSERT_VALID (pNewElement);

	if (!ReplaceByID (uiCmdID, pNewElement))
	{
		delete pNewElement;
	}

	return pNewElement;
}
//****
CWnd* CBCGPRibbonPanel::GetParentWnd() const
{
	ASSERT_VALID (this);

	CWnd* pParentWnd = NULL;

	if (m_pParentMenuBar != NULL)
	{
		ASSERT_VALID (m_pParentMenuBar);
		pParentWnd = m_pParentMenuBar;
	}
	else if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		pParentWnd = m_pParent->GetParentRibbonBar();
	}

	return pParentWnd;
}
//****
void CBCGPRibbonPanel::RedrawElement (CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	const CRect rectElem = pElem->GetRect();

	if (rectElem.IsRectEmpty())
	{
		return;
	}

	CWnd* pParentWnd = GetParentWnd();

	if (pParentWnd->GetSafeHwnd() != NULL)
	{
		ASSERT_VALID (pParentWnd);

		pParentWnd->InvalidateRect (rectElem);
		pParentWnd->UpdateWindow();
	}
}
//****
BOOL CBCGPRibbonPanel::HasElement (const CBCGPBaseRibbonElement* pElem) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElemFromList = m_arElements [i];
		ASSERT_VALID (pElemFromList);

		if (pElemFromList->Find (pElem))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//****
void CBCGPRibbonPanel::GetElements (
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->GetElements (arElements);
	}

	if (m_btnLaunch.GetID() > 0)
	{
		arElements.Add (&m_btnLaunch);
	}

	if (!IsMainPanel())
	{
		arElements.Add (&m_btnDefault);
	}
}
//****
void CBCGPRibbonPanel::GetItemIDsList (CList<UINT,UINT>& lstItems) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->GetItemIDsList (lstItems);
	}

	m_btnDefault.GetItemIDsList (lstItems);
}
//****
void CBCGPRibbonPanel::GetElementsByID (UINT uiCmdID,
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->GetElementsByID (uiCmdID, arElements);
	}

	m_btnDefault.GetElementsByID (uiCmdID, arElements);
	m_btnLaunch.GetElementsByID (uiCmdID, arElements);
}
//****
void CBCGPRibbonPanel::GetVisibleElements (
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->GetVisibleElements (arElements);
	}

	m_btnDefault.GetVisibleElements (arElements);
	m_btnLaunch.GetVisibleElements (arElements);
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetPressed() const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CBCGPBaseRibbonElement* pPressedElem = pElem->GetPressed();
		if (pPressedElem != NULL)
		{
			ASSERT_VALID (pPressedElem);
			return pPressedElem;
		}
	}

	return NULL;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetDroppedDown() const
{
	ASSERT_VALID (this);

	if (!m_btnDefault.m_rect.IsRectEmpty())
	{
		CBCGPBaseRibbonElement* pDroppedElem = 
			((CBCGPRibbonPanel*) this)->m_btnDefault.GetDroppedDown();

		if (pDroppedElem != NULL)
		{
			ASSERT_VALID (pDroppedElem);
			return pDroppedElem;
		}
	}

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CBCGPBaseRibbonElement* pDroppedElem = pElem->GetDroppedDown();
		if (pDroppedElem != NULL)
		{
			ASSERT_VALID (pDroppedElem);
			return pDroppedElem;
		}
	}

	return NULL;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetFocused() const
{
	ASSERT_VALID (this);

	if (!m_btnDefault.m_rect.IsRectEmpty() && m_btnDefault.IsFocused())
	{
		return (CBCGPBaseRibbonElement*)&m_btnDefault;
	}

	if (m_btnLaunch.IsFocused())
	{
		return (CBCGPBaseRibbonElement*)&m_btnLaunch;
	}

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CBCGPBaseRibbonElement* pFocusedElem = pElem->GetFocused();
		if (pFocusedElem != NULL)
		{
			ASSERT_VALID (pFocusedElem);
			return pFocusedElem;
		}
	}

	return NULL;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetHighlighted() const
{
	ASSERT_VALID (this);
	return m_pHighlighted;
}
//****
BOOL CBCGPRibbonPanel::OnKey (UINT nChar)
{
	ASSERT_VALID (this);

	if (m_arElements.GetSize() == NULL)
	{
		return FALSE;
	}

	if (m_pHighlighted != NULL)
	{
		ASSERT_VALID (m_pHighlighted);

		if (m_pHighlighted->OnProcessKey (nChar))
		{
			return TRUE;
		}
	}

	CBCGPBaseRibbonElement* pNewHighlighted = NULL;
	BOOL bInvokeCommand = FALSE;

	switch (nChar)
	{
	case VK_HOME:
		if (m_bMenuMode)
		{
			pNewHighlighted = GetFirstTabStop();
		}
		break;

	case VK_END:
		if (m_bMenuMode)
		{
			pNewHighlighted = GetLastTabStop();
		}
		break;

	case VK_RIGHT:
		if (m_bMenuMode &&
			m_pHighlighted != NULL && m_pHighlighted->HasMenu() &&
			!m_pHighlighted->IsDroppedDown())
		{
			m_pHighlighted->OnShowPopupMenu();

			if (m_pHighlighted->m_pPopupMenu != NULL)
			{
				ASSERT_VALID (m_pHighlighted->m_pPopupMenu);
				m_pHighlighted->m_pPopupMenu->SendMessage (WM_KEYDOWN, VK_HOME);
			}
			break;
		}

	case VK_LEFT:
		if (m_bMenuMode && nChar == VK_LEFT && m_pParentMenuBar != NULL)
		{
			ASSERT_VALID (m_pParentMenuBar);

			const BOOL bIsPaletteIcon = m_pHighlighted != NULL && m_pHighlighted->IsPaletteIcon();

			CBCGPPopupMenu* pParentMenu = 
				DYNAMIC_DOWNCAST (CBCGPPopupMenu, m_pParentMenuBar->GetParent());

			if (!bIsPaletteIcon && pParentMenu->GetParentPopupMenu() != NULL)
			{
				CBCGPRibbonBar* pRibbonBar = m_pParentMenuBar->GetTopLevelRibbonBar();
				if (pRibbonBar != NULL && pRibbonBar->GetKeyboardNavLevelCurrent() == this)
				{
					pRibbonBar->SetKeyboardNavigationLevel (pRibbonBar->GetKeyboardNavLevelParent());
				}

				pParentMenu->CloseMenu();
				return TRUE;
			}
		}

	case VK_DOWN:
	case VK_UP:
	case VK_TAB:
		if (m_pHighlighted != NULL)
		{
			if (m_pPaletteButton != NULL && (nChar == VK_DOWN || nChar == VK_UP) &&
				m_pParentMenuBar->GetSafeHwnd() != NULL &&
				m_pScrollBar->GetSafeHwnd() != NULL)
			{
				CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (CBCGPRibbonPaletteIcon, m_pHighlighted);
				if (pIcon != NULL)
				{
					ASSERT_VALID (pIcon);
					ASSERT_VALID (m_pParentMenuBar);

					const CRect rectIcon = pIcon->GetRect();
					const CRect rectPalette = GetPaletteRect();

					int nDelta = 0;

					if (nChar == VK_DOWN)
					{
						if (rectIcon.b >= rectPalette.b &&
							m_pScrollBar->GetScrollPos() < m_pScrollBar->GetScrollLimit())
						{
							nDelta = rectIcon.Height();
						}
					}
					else
					{
						if (rectIcon.t <= rectPalette.t &&
							m_pScrollBar->GetScrollPos() > 0)
						{
							nDelta = -rectIcon.Height();
						}
					}

					if (nDelta != 0)
					{
						ScrollPalette (nDelta, TRUE);
						m_pParentMenuBar->RedrawWindow (rectPalette);

						m_pScrollBar->SetScrollPos (m_nScrollOffset);
					}
				}
			}

			CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*> arElems;
			GetVisibleElements (arElems);

			int nScroll = 0;

			pNewHighlighted = CBCGPRibbonBar::FindNextFocusedElement (
				nChar, arElems, m_rect, m_pHighlighted, FALSE, FALSE, nScroll);

			if (IsMenuMode() && pNewHighlighted == NULL)
			{
				pNewHighlighted = nChar == VK_DOWN ? 
					GetFirstTabStop() : GetLastTabStop();
			}
		}
		else
		{
			pNewHighlighted = nChar == VK_RIGHT || nChar == VK_DOWN ? 
				GetFirstTabStop() : GetLastTabStop();
		}
		break;

	case VK_RETURN:
	case VK_SPACE:
		bInvokeCommand = TRUE;
		break;

	default:
		if (IsMenuMode())
		{
			BOOL bKeyIsPrintable = CBCGPKeyboardManager::IsKeyPrintable (nChar);

			UINT nUpperChar = nChar;
			if (bKeyIsPrintable)
			{
				nUpperChar = CBCGPKeyboardManager::TranslateCharToUpper (nChar);
			}

			for (int i = 0; i < m_arElements.GetSize(); i++)
			{
				CBCGPBaseRibbonElement* pElem = m_arElements [i];
				ASSERT_VALID (pElem);

				if (pElem->OnMenuKey (nUpperChar))
				{
					return TRUE;
				}

				CString strLabel = pElem->GetText();

				int iAmpOffset = strLabel.Find (_T('&'));
				if (iAmpOffset >= 0 && iAmpOffset < strLabel.GetLength() - 1)
				{
					TCHAR szChar [2] = { strLabel.GetAt (iAmpOffset + 1), '\0' };
					CharUpper (szChar);

					UINT uiHotKey = (UINT) (szChar [0]);

					if (uiHotKey == nUpperChar)
					{
						if (!pElem->IsDisabled())
						{
							pNewHighlighted = pElem;
							bInvokeCommand = TRUE;
						}
						break;
					}
				}
			}
		}
	}

	BOOL bRes = FALSE;

	if (pNewHighlighted != NULL)
	{
		ASSERT_VALID (pNewHighlighted);

		if (m_pHighlighted != pNewHighlighted)
		{
			if (m_pHighlighted != NULL)
			{
				ASSERT_VALID (m_pHighlighted);

				if (m_pHighlighted->m_bIsHighlighted)
				{
					m_pHighlighted->m_bIsHighlighted = FALSE;
					m_pHighlighted->OnHighlight (FALSE);
				}

				if (m_pHighlighted->m_bIsFocused)
				{
					m_pHighlighted->m_bIsFocused = FALSE;
					m_pHighlighted->OnSetFocus (FALSE);
				}

				m_pHighlighted->Redraw();
				m_pHighlighted = NULL;
			}

			if (globalData.IsAccessibilitySupport() && m_pParentMenuBar != NULL && IsMenuMode())
			{
				CPoint pt = pNewHighlighted->GetRect().TopLeft();
				m_pParentMenuBar->ClientToScreen (&pt);

				m_pParentMenuBar->OnSetAccData  ((LONG) MAKELPARAM (pt.x, pt.y));

				globalData.NotifyWinEvent (EVENT_OBJECT_FOCUS, 
						m_pParentMenuBar->GetSafeHwnd(), OBJID_CLIENT, 
						GetIndex (pNewHighlighted) + 1);
			}


			m_pHighlighted = pNewHighlighted;
			pNewHighlighted->OnHighlight (TRUE);

			if (m_pPaletteButton != NULL)
			{
				MakePaletteItemVisible (pNewHighlighted);
			}

			pNewHighlighted->m_bIsHighlighted = TRUE;
			pNewHighlighted->m_bIsFocused = TRUE;
			pNewHighlighted->OnSetFocus (TRUE);

			m_pHighlighted->Redraw();

			if (m_pParentMenuBar != NULL)
			{
				ASSERT_VALID (m_pParentMenuBar);

				CBCGPRibbonPanelMenu* pParentMenu = 
					DYNAMIC_DOWNCAST (CBCGPRibbonPanelMenu, m_pParentMenuBar->GetParent());

				if (pParentMenu != NULL)
				{
					ASSERT_VALID (pParentMenu);

					if (pParentMenu->IsScrollable())
					{
						CRect rectHighlighted = m_pHighlighted->GetRect();

						CRect rectParent;
						m_pParentMenuBar->GetClientRect (rectParent);

						if (rectHighlighted.b > rectParent.b)
						{
							while (pParentMenu->IsScrollDnAvailable())
							{
								m_pParentMenuBar->SetOffset (m_pParentMenuBar->GetOffset() + 1);
								pParentMenu->AdjustScroll();

								if (m_pHighlighted->GetRect().b <= rectParent.b)
								{
									m_pParentMenuBar->RedrawWindow();
									break;
								}
							}
						}
						else if (rectHighlighted.t < rectParent.t)
						{
							while (pParentMenu->IsScrollUpAvailable())
							{
								m_pParentMenuBar->SetOffset (m_pParentMenuBar->GetOffset() - 1);
								pParentMenu->AdjustScroll();

								if (m_pHighlighted->GetRect().t >= rectParent.t)
								{
									m_pParentMenuBar->RedrawWindow();
									break;
								}
							}
						}
					}

					if (pParentMenu->GetParentRibbonElement() != NULL)
					{
						ASSERT_VALID (pParentMenu->GetParentRibbonElement());
						pParentMenu->GetParentRibbonElement()->OnChangeMenuHighlight (m_pParentMenuBar, m_pHighlighted);
					}
				}
			}
		}

		bRes = TRUE;
	}

	if (bInvokeCommand && m_pHighlighted != NULL)
	{
		ASSERT_VALID (m_pHighlighted);

		CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (
			CBCGPRibbonButton, m_pHighlighted);

		if (pButton != NULL)
		{
			if (pButton->HasMenu())
			{
				HWND hWndParent = GetParentMenuBar()->GetSafeHwnd();

				pButton->OnShowPopupMenu();

				if (hWndParent != NULL && !::IsWindow (hWndParent))
				{
					return TRUE;
				}

				if (pButton->m_pPopupMenu != NULL)
				{
					ASSERT_VALID (pButton->m_pPopupMenu);
					pButton->m_pPopupMenu->SendMessage (WM_KEYDOWN, VK_HOME);
				}
			}
			else if (!pButton->IsDisabled())
			{
				CBCGPRibbonBar* pRibbonBar = m_pParentMenuBar->GetTopLevelRibbonBar();
				if (pRibbonBar != NULL && pRibbonBar->GetKeyboardNavLevelCurrent() == this)
				{
					pRibbonBar->DeactivateKeyboardFocus (TRUE);
				}

				pButton->OnClick (pButton->GetRect().TopLeft());

				if (pRibbonBar != NULL && pRibbonBar->GetTopLevelFrame() != NULL)
				{
					pRibbonBar->GetTopLevelFrame()->SetFocus();
				}
			}

			bRes = TRUE;
		}
	}

	return bRes;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetFirstTabStop() const
{
	ASSERT_VALID (this);

	int i = 0;

	if (m_pPaletteButton != NULL)
	{
		ASSERT_VALID (m_pPaletteButton);

		// First, find the "top element":
		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
			BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

			if (bIsIcon || bIsLabel)
			{
				continue;
			}

			if (pElem->m_bIsOnPaletteTop)
			{
				CBCGPBaseRibbonElement* pTabStop = pElem->GetFirstTabStop();
				if (pTabStop != NULL)
				{
					return pTabStop;
				}
			}
		}

		// Not found, return the first icon:
		for (i = 0; i < m_arElements.GetSize(); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)))
			{
				CBCGPBaseRibbonElement* pTabStop = pElem->GetFirstTabStop();
				if (pTabStop != NULL)
				{
					return pTabStop;
				}
			}
		}
	}

	for (i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CBCGPBaseRibbonElement* pTabStop = pElem->GetFirstTabStop();
		if (pTabStop != NULL)
		{
			return pTabStop;
		}
	}

	return NULL;
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetLastTabStop() const
{
	ASSERT_VALID (this);

	int i = 0;

	if (m_pPaletteButton != NULL)
	{
		ASSERT_VALID (m_pPaletteButton);

		// First, find the "bottom element":
		for (i = (int) m_arElements.GetSize() - 1; i >= 0; i--)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
			BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

			if (bIsIcon || bIsLabel)
			{
				continue;
			}

			if (!pElem->m_bIsOnPaletteTop)
			{
				CBCGPBaseRibbonElement* pTabStop = pElem->GetFirstTabStop();
				if (pTabStop != NULL)
				{
					return pTabStop;
				}
			}
		}

		// Not found, return the last icon:
		for (i = (int) m_arElements.GetSize() - 1; i >= 0; i--)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)))
			{
				CBCGPBaseRibbonElement* pTabStop = pElem->GetFirstTabStop();
				if (pTabStop != NULL)
				{
					return pTabStop;
				}
			}
		}
	}

	for (i = (int) m_arElements.GetSize() - 1; i >= 0; i--)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CBCGPBaseRibbonElement* pTabStop = pElem->GetLastTabStop();
		if (pTabStop != NULL)
		{
			return pTabStop;
		}
	}

	return NULL;
}
//****
void CBCGPRibbonPanel::CleanUpSizes()
{
	ASSERT_VALID (this);
	m_arWidths.RemoveAll();

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->CleanUpSizes();
	}

	m_btnDefault.CleanUpSizes();
}
//****
void CBCGPRibbonPanel::ScrollPalette (int nScrollOffset, BOOL bIsDelta)
{
	ASSERT_VALID (this);

	const int nDelta = bIsDelta ? nScrollOffset : m_nScrollOffset - nScrollOffset;

	if (nDelta == 0)
	{
		return;
	}

	m_nScrollOffset = bIsDelta ? m_nScrollOffset - nDelta : nScrollOffset;

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsLabel = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel));
		BOOL bIsIcon = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon));

		if (bIsLabel || bIsIcon)
		{
			pElem->m_rect.OffsetRect (0, nDelta);
		}
	}
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonPanel::GetParentButton() const
{
	ASSERT_VALID (this);

	if (m_pParentMenuBar == NULL)
	{
		return NULL;
	}

	return ((CBCGPPopupMenu*)m_pParentMenuBar->GetParent())->GetParentRibbonElement();
}
//****
CSize CBCGPRibbonPanel::GetPaletteMinSize() const
{
	ASSERT_VALID (this);

	if (m_pPaletteButton == NULL)
	{
		ASSERT (FALSE);
		return CSize (-1, -1);
	}

	ASSERT_VALID (m_pPaletteButton);

	const BOOL bNoSideBar = m_pPaletteButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonUndoButton));

	CBCGPRibbonBar* pRibbonBar = m_pPaletteButton->GetTopLevelRibbonBar();
	ASSERT_VALID (pRibbonBar);

	CClientDC dc (pRibbonBar);

	CFont* pOldFont = dc.SelectObject (pRibbonBar->GetFont());
	ASSERT (pOldFont != NULL);

	const int cxScroll = ::GetSystemMetrics (SM_CXVSCROLL);

	int cxIcon = m_pPaletteButton->GetIconSize().w;
	int cyIcon = m_pPaletteButton->GetIconSize().h;

	int cxLabel = 0;
	int cyLabel = 0;

	int cxBottom = 0;
	int cyBottom = 0;

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->SetInitialMode();
		pElem->OnCalcTextSize (&dc);

		CSize sizeElem = pElem->GetSize (&dc);

		if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)))
		{
		}
		else if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel)))
		{
			cxLabel = max (cxLabel, sizeElem.w);
			cyLabel = max (cyLabel, sizeElem.h + m_pPaletteButton->GetGroupOffset());
		}
		else
		{
			if (!bNoSideBar)
			{
				sizeElem.w += 4 * TEXT_MARGIN + CBCGPToolBar::GetMenuImageSize().w + 
					2 * CBCGPVisualManager::GetInstance()->GetMenuImageMargin();
			}

			cxBottom = max (cxBottom, sizeElem.w);
			cyBottom += sizeElem.h;
		}
	}

	dc.SelectObject (pOldFont);

	int cx = max (cxIcon, cxLabel);

	return CSize (max (cx + cxScroll, cxBottom), cyIcon + cyBottom + cyLabel);
}
//****
void CBCGPRibbonPanel::SetKeys (LPCTSTR lpszKeys)
{
	ASSERT_VALID (this);
	m_btnDefault.SetKeys (lpszKeys);
}
//****
void CBCGPRibbonPanel::OnRTLChanged (BOOL bIsRTL)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnRTLChanged (bIsRTL);
	}

	m_btnDefault.OnRTLChanged (bIsRTL);
	m_btnLaunch.OnRTLChanged (bIsRTL);
}
//****
CBCGPRibbonPanelMenu* CBCGPRibbonPanel::ShowPopup (CBCGPRibbonDefaultPanelButton* pButton/* = NULL*/)
{
	ASSERT_VALID (this);

	if (pButton == NULL)
	{
		pButton = &m_btnDefault;
	}

	ASSERT_VALID (pButton);

	CWnd* pWndParent = pButton->GetParentWnd();
	if (pWndParent == NULL)
	{
		return NULL;
	}

	if (m_pParent != NULL && !pButton->IsQATMode())
	{
		ASSERT_VALID(m_pParent);
		m_pParent->EnsureVisible(pButton);
	}

	const BOOL bIsRTL = (pWndParent->GetExStyle() & WS_EX_LAYOUTRTL);

	if (m_arWidths.GetSize() == NULL)
	{
		ASSERT_VALID (m_pParent);

		CBCGPRibbonBar* pRibbonBar = pButton->GetTopLevelRibbonBar();
		ASSERT_VALID (pRibbonBar);

		CClientDC dc (pRibbonBar);

		CFont* pOldFont = dc.SelectObject (pRibbonBar->GetFont());
		ASSERT (pOldFont != NULL);

		int nHeight = m_pParent->GetMaxHeight (&dc);
		RecalcWidths (&dc, nHeight);

		dc.SelectObject (pOldFont);
	}

	CRect rectBtn = pButton->m_rect;
	pWndParent->ClientToScreen (&rectBtn);

	CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (this);
	pMenu->SetParentRibbonElement (pButton);

	pMenu->Create (pWndParent, 
		bIsRTL ? rectBtn.r : rectBtn.l, 
		rectBtn.b, (HMENU) NULL);

	pButton->SetDroppedDown (pMenu);

	return pMenu;
}
//****
CRect CBCGPRibbonPanel::GetPaletteRect()
{
	ASSERT_VALID (this);

	CRect rectPalette = m_rect;

	if (!m_rectMenuAreaTop.IsRectEmpty())
	{
		rectPalette.t = m_rectMenuAreaTop.b;
	}

	if (!m_rectMenuAreaBottom.IsRectEmpty())
	{
		rectPalette.b = m_rectMenuAreaBottom.t;
	}

	return rectPalette;
}
//****
void CBCGPRibbonPanel::MakePaletteItemVisible (CBCGPBaseRibbonElement* pItem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	if (!pItem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteIcon)))
	{
		return;
	}

	CRect rectItem = pItem->GetRect();
	CRect rectPalette = GetPaletteRect();

	int nDelta = 0;

	if (rectItem.t < rectPalette.t)
	{
		nDelta = rectPalette.t - rectItem.t;
	}
	else if (rectItem.b > rectPalette.b)
	{
		nDelta = rectPalette.b - rectItem.b;
	}

	if (nDelta != 0)
	{
		ScrollPalette (nDelta, TRUE);

		if (GetParentWnd() != NULL)
		{
			GetParentWnd()->RedrawWindow (rectPalette);
		}

		if (m_pScrollBar->GetSafeHwnd() != NULL)
		{
			m_pScrollBar->SetScrollPos (m_nScrollOffset);
		}
	}
}
//****
void CBCGPRibbonPanel::SetFocused (CBCGPBaseRibbonElement* pNewFocus)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement* pFocused = GetFocused();

	if (pNewFocus == pFocused)
	{
		return;
	}
			
	if (pFocused != NULL)
	{
		ASSERT_VALID (pFocused);

		pFocused->m_bIsFocused = FALSE;
		pFocused->OnSetFocus (FALSE);

		if (pFocused->m_bIsHighlighted)
		{
			pFocused->m_bIsHighlighted = FALSE;
			pFocused->OnHighlight (FALSE);

			if (m_pHighlighted == pFocused)
			{
				m_pHighlighted = NULL;
			}
		}

		pFocused->Redraw();
	}

	if (pNewFocus != NULL)
	{
		ASSERT_VALID (pNewFocus);

		pNewFocus->m_bIsFocused = pNewFocus->m_bIsHighlighted = TRUE;
		pNewFocus->OnSetFocus (TRUE);
		pNewFocus->OnHighlight (TRUE);
		pNewFocus->Redraw();

		m_pHighlighted = pNewFocus;
	}
}
//****
BOOL CBCGPRibbonPanel::IsScenicLook() const
{
	CBCGPRibbonCategory* pCategory = GetParentCategory();
	if (pCategory == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return pCategory->IsScenicLook();
}

#endif // BCGP_EXCLUDE_RIBBON
