// PropertySheetCtrl.cpp : implementation file
//
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.

//#include "BCGCBPro.h"
#include "PropertySheetCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetCtrl

IMPLEMENT_DYNAMIC(CPropertySheetCtrl, CBCGPPropertySheet)

CPropertySheetCtrl::CPropertySheetCtrl(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
:CBCGPPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hAccel = NULL;
}

CPropertySheetCtrl::CPropertySheetCtrl(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
:CBCGPPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hAccel = NULL;
}

CPropertySheetCtrl::~CPropertySheetCtrl()
{
}


BEGIN_MESSAGE_MAP(CPropertySheetCtrl, CBCGPPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetCtrl message handlers

void CPropertySheetCtrl::PostNcDestroy()
{
	// Call the base class routine first
	CBCGPPropertySheet::PostNcDestroy();
	
	if (m_bModeless)
	{
		delete this;
	}
}

BOOL CPropertySheetCtrl::OnInitDialog()
{
	ASSERT_VALID(this);
	
	// Call the base class routine
	BOOL bRtnValue = CBCGPPropertySheet::OnInitDialog();
	
	ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	if (IsVisualManagerStyle())
	{
		ModifyStyle(WS_BORDER, 0);
	}

	ResizeControl();
	return bRtnValue;
}

BOOL CPropertySheetCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR* pNMHDR = (NMHDR*) lParam;
	ASSERT (pNMHDR != NULL);

	if (pNMHDR->code == TCN_SELCHANGE)
	{
		ResizeControl();
	}
	
	return CBCGPPropertySheet::OnNotify(wParam, lParam, pResult);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void CPropertySheetCtrl::LoadAcceleratorTable(UINT nAccelTableID /*=0*/)
{
	if (nAccelTableID)
	{
		m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(nAccelTableID));
		ASSERT(m_hAccel);
	}
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
BOOL CPropertySheetCtrl::PreTranslateMessage(MSG* pMsg)
{
	//TRACE("[%s - %d] - CPropertySheetCtrl::PreTranslateMessage().....\n", __FILE__,__LINE__);
	////////
	// Check to see if the property sheet has an accelerator table
	// attached to it. If there is one call it. Return TRUE if it has
	// been processed. Otherwise, past it to the base class function.
	////////
	if (m_hAccel && ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
	{
		return TRUE;
	}

	return CBCGPPropertySheet::PreTranslateMessage(pMsg);
}

void CPropertySheetCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPPropertySheet::OnSize(nType, cx, cy);
	ResizeControl();
}

void CPropertySheetCtrl::ResizeControl()
{
	CWnd* pTabCtrl = GetTabControl();
	if (m_wndTab.GetSafeHwnd() != NULL)
	{
		pTabCtrl = &m_wndTab;
		m_wndTab.SetButtonsVisible (FALSE);
	}

	if (pTabCtrl->GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);

	pTabCtrl->SetWindowPos (NULL, 
		0, 0, rectClient.Width(), rectClient.Height(), 
		SWP_NOZORDER | SWP_NOACTIVATE);

	int nPageCount = CBCGPPropertySheet::GetPageCount();

	int nXBorder = ::GetSystemMetrics(SM_CXEDGE);
	int nYBorder = ::GetSystemMetrics(SM_CYEDGE);
	
	for (int nPage = 0; nPage <= nPageCount - 1; nPage++)
	{
		CPropertyPage* pPage = GetPage(nPage);
		
		if ((pPage != NULL) && (pPage->m_hWnd != NULL))
		{
			CRect rcTabCtrl;
			pPage->GetWindowRect(&rcTabCtrl);
			pTabCtrl->ScreenToClient(rcTabCtrl);
			
			pPage->SetWindowPos (NULL, 
				rcTabCtrl.l, rcTabCtrl.t, 
				rectClient.Width() - (nXBorder * 3), 
				rectClient.Height() - (rcTabCtrl.t + nYBorder), 
				SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
}
