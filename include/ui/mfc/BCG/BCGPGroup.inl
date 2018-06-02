//
// BCGPGroup.cpp : implementation file
//

#include "BCGPGroup.h"
#include "Bcgglobals.h"
#include "BCGPVisualManager.h"
#include "BCGPDlgImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPGroup

IMPLEMENT_DYNAMIC(CBCGPGroup, CButton)

CBCGPGroup::CBCGPGroup()
{
	m_bVisualManagerStyle = FALSE;
	m_bOnGlass = FALSE;
}

CBCGPGroup::~CBCGPGroup()
{
}

#ifndef WM_UPDATEUISTATE
#define	WM_UPDATEUISTATE 0x0128
#endif

BEGIN_MESSAGE_MAP(CBCGPGroup, CButton)
	//{{AFX_MSG_MAP(CBCGPGroup)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLAERO, OnBCGSetControlAero)
	ON_MESSAGE(WM_UPDATEUISTATE, OnUpdateUIState)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPGroup message handlers

BOOL CBCGPGroup::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//****
void CBCGPGroup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBCGPMemDC* pMemDC = NULL;
	CDC* pDC = &dc;

	if (m_bOnGlass)
	{
		pMemDC = new CBCGPMemDC (dc, this);
		pDC = &pMemDC->GetDC ();
		globalData.DrawParentBackground (this, pDC);
	}

	CRect rectClient;
	GetClientRect (rectClient);

	CString strName;
	GetWindowText (strName);

	CFont* pOldFont = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT(pOldFont != NULL);

	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (globalData.clrBarText);

	CBCGPVisualManager::GetInstance ()->OnDrawGroup (pDC, this, rectClient, strName);

	pDC->SelectObject (pOldFont);

	if (pMemDC != NULL)
	{
		delete pMemDC;
	}
}
//****
LRESULT CBCGPGroup::OnBCGSetControlVMMode (WPARAM wp, LPARAM)
{
	m_bVisualManagerStyle = (BOOL) wp;
	return 0;
}
//****
LRESULT CBCGPGroup::OnBCGSetControlAero (WPARAM wp, LPARAM)
{
	m_bOnGlass = (BOOL) wp;
	return 0;
}
//****
void CBCGPGroup::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);

	if (GetParent () != NULL)
	{
		CRect rect;
		GetWindowRect (rect);

		GetParent ()->ScreenToClient (&rect);
		GetParent ()->RedrawWindow (rect);
	}
}
//****
LRESULT CBCGPGroup::OnUpdateUIState (WPARAM, LPARAM)
{
	return 0;
}
