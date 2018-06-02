
//
// BCGPPropertyPage.cpp : implementation file
//

//#include "BCGCBPro.h"
#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#endif
#include "BCGPPropertySheet.h"
#include "BCGPPropertyPage.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropertyPage

IMPLEMENT_DYNCREATE(CBCGPPropertyPage, CPropertyPage)

#pragma warning (disable : 4355)

CBCGPPropertyPage::CBCGPPropertyPage() :
	m_Impl (*this)
{
	CommonInit();
}

CBCGPPropertyPage::CBCGPPropertyPage(UINT nIDTemplate, UINT nIDCaption) :
	CPropertyPage (nIDTemplate, nIDCaption),
	m_Impl (*this)
{
	CommonInit();
}

CBCGPPropertyPage::CBCGPPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption) :
	CPropertyPage (lpszTemplateName, nIDCaption),
	m_Impl (*this)
{
	CommonInit();
}

#pragma warning (default : 4355)

void CBCGPPropertyPage::CommonInit()
{
	m_pCategory = NULL;
	m_nIcon = -1;
	m_nSelIconNum = -1;
	m_hTreeNode = NULL;
}

CBCGPPropertyPage::~CBCGPPropertyPage()
{
}

BEGIN_MESSAGE_MAP(CBCGPPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBCGPPropertyPage)
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropertyPage message handlers

void CBCGPPropertyPage::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	m_Impl.OnActivate (nState, pWndOther);
	CPropertyPage::OnActivate (nState, pWndOther, bMinimized);
}
//****
BOOL CBCGPPropertyPage::OnNcActivate(BOOL bActive) 
{
	m_Impl.OnNcActivate (bActive);

	//-----------------------------------------------------------
	// Do not call the base class because it will call Default()
	// and we may have changed bActive.
	//-----------------------------------------------------------
	return (BOOL) DefWindowProc (WM_NCACTIVATE, bActive, 0L);
}
//****
void CBCGPPropertyPage::SetActiveMenu (CBCGPPopupMenu* pMenu)
{
	m_Impl.SetActiveMenu (pMenu);
}
//****
BOOL CBCGPPropertyPage::PreTranslateMessage(MSG* pMsg) 
{
	if (m_Impl.PreTranslateMessage (pMsg))
	{
		return TRUE;
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}
//****
BOOL CBCGPPropertyPage::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (m_Impl.OnCommand (wParam, lParam))
	{
		return TRUE;
	}

	return CPropertyPage::OnCommand(wParam, lParam);
}
//****
BOOL CBCGPPropertyPage::OnSetActive() 
{
	CBCGPPropertySheet* pParent = DYNAMIC_DOWNCAST(CBCGPPropertySheet, GetParent());
	if (pParent != NULL)
	{
		pParent->OnActivatePage (this);
	}
	
	return CPropertyPage::OnSetActive();
}
//****
BOOL CBCGPPropertyPage::OnInitDialog() 
{
	BOOL bRes = CPropertyPage::OnInitDialog();
	
	if (IsVisualManagerStyle())
	{
		m_Impl.EnableVisualManagerStyle (TRUE);
	}

	CBCGPPropertySheet* pParent = DYNAMIC_DOWNCAST(CBCGPPropertySheet, GetParent());
	if (pParent == NULL || pParent->GetHeaderHeight() == 0)
	{
		return bRes;
	}

	const int nHeaderHeight = pParent->GetHeaderHeight();
	
	CWnd* pWndChild = GetWindow (GW_CHILD);
	while (pWndChild != NULL)
	{
		CRect rectChild;
		pWndChild->GetWindowRect (rectChild);
		ScreenToClient (rectChild);

		rectChild.OffsetRect (0, nHeaderHeight);

		pWndChild->SetWindowPos (NULL, rectChild.l, rectChild.t,
			-1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

		pWndChild = pWndChild->GetNextWindow();
	}

	return bRes;
}
//****
BOOL CBCGPPropertyPage::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bRes = TRUE;

	CRect rectClient;
	GetClientRect (rectClient);

	if (IsVisualManagerStyle())
	{
		if (!CBCGPVisualManager::GetInstance()->OnFillDialog (pDC, this, rectClient))
		{
			bRes = CPropertyPage::OnEraseBkgnd (pDC);
		}
	}
	else
	{
		bRes = CPropertyPage::OnEraseBkgnd(pDC);
	}

	CBCGPPropertySheet* pParent = DYNAMIC_DOWNCAST(CBCGPPropertySheet, GetParent());
	if (pParent != NULL && pParent->GetHeaderHeight() > 0)
	{
		CRect rectHeader = rectClient;
		rectHeader.b = rectHeader.t + pParent->GetHeaderHeight();

		if (pParent->GetLook() == CBCGPPropertySheet::PropSheetLook_OutlookBar ||
			pParent->GetLook() == CBCGPPropertySheet::PropSheetLook_Tree ||
			pParent->GetLook() == CBCGPPropertySheet::PropSheetLook_List)
		{
			CRect rectParent;
			pParent->GetWindowRect (rectParent);

			ScreenToClient (rectParent);

			rectHeader.r = rectParent.r - ::GetSystemMetrics (SM_CXDLGFRAME);
		}

		pParent->OnDrawPageHeader (pDC, pParent->GetPageIndex (this), rectHeader);
	}

	return bRes;
}
//****
void CBCGPPropertyPage::EnableVisualManagerStyle (BOOL bEnable, const CList<UINT,UINT>* plstNonSubclassedItems)
{
	ASSERT_VALID (this);

	m_Impl.EnableVisualManagerStyle (bEnable, FALSE, plstNonSubclassedItems);
}
//****
HBRUSH CBCGPPropertyPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (IsVisualManagerStyle())
	{
		HBRUSH hbr = m_Impl.OnCtlColor (pDC, pWnd, nCtlColor);
		if (hbr != NULL)
		{
			return hbr;
		}
	}	

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}
//****
void CBCGPPropertyPage::OnDestroy() 
{
	m_Impl.OnDestroy();
	CPropertyPage::OnDestroy();
}
//****
void CBCGPPropertyPage::OnNcPaint() 
{
	if (!IsVisualManagerStyle())
	{
		Default();
	}
}
//****
BOOL CBCGPPropertyPage::OnApply() 
{
	BOOL bRes = CPropertyPage::OnApply();

	CBCGPPropertySheet* pParent = DYNAMIC_DOWNCAST(CBCGPPropertySheet, GetParent());
	if (pParent != NULL)
	{
		ASSERT_VALID (pParent);
		pParent->OnApply();
	}

	return bRes;
}
