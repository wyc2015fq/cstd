//
// BCGPStatic.cpp : implementation file
//

#include "Bcgglobals.h"
#include "BCGPVisualManager.h"
#include "BCGPStatic.h"
#include "BCGPDlgImpl.h"

IMPLEMENT_DYNAMIC(CBCGPStatic, CStatic)

/////////////////////////////////////////////////////////////////////////////
// CBCGPStatic

CBCGPStatic::CBCGPStatic()
{
	m_bOnGlass = FALSE;
	m_bVisualManagerStyle = FALSE;
}

CBCGPStatic::~CBCGPStatic()
{
}


BEGIN_MESSAGE_MAP(CBCGPStatic, CStatic)
	//{{AFX_MSG_MAP(CBCGPStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLAERO, OnBCGSetControlAero)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPStatic message handlers

BOOL CBCGPStatic::OnEraseBkgnd(CDC* pDC) 
{
	if (!m_bOnGlass)
	{
		return CStatic::OnEraseBkgnd (pDC);
	}

	return TRUE;
}
//****
void CBCGPStatic::OnPaint() 
{
	BOOL bDisabled = !m_bOnGlass && m_bVisualManagerStyle &&
		!CBCGPVisualManager::GetInstance ()->IsEmbossDisabledBtnText () &&
		!IsWindowEnabled ();

	if (!m_bOnGlass && !bDisabled)
	{
		Default ();
		return;
	}

	CPaintDC dc(this); // device context for painting

	CBCGPMemDC memDC (dc, this);
	CDC* pDC = &memDC.GetDC ();

	CRect rectText;
	GetClientRect (rectText);

	globalData.DrawParentBackground (this, pDC);

	CFont* pOldFont = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT(pOldFont != NULL);

	UINT uiDTFlags = DT_WORDBREAK;

	const DWORD dwStyle = GetStyle ();

	if (dwStyle & SS_CENTER)
	{
		uiDTFlags |= DT_CENTER;
	}
	else if (dwStyle & SS_RIGHT)
	{
		uiDTFlags |= DT_RIGHT;
	}

	if (dwStyle & SS_NOPREFIX)
	{
		uiDTFlags |= DT_NOPREFIX;
	}

	CString strText;
	GetWindowText (strText);

	if (bDisabled)
	{
		COLORREF clrTextOld = pDC->SetTextColor (globalData.clrGrayedText);
		pDC->SetBkMode (TRANSPARENT);
		pDC->DrawText (strText, rectText, uiDTFlags);
		pDC->SetTextColor (clrTextOld);
	}
	else
	{
		CBCGPVisualManager::GetInstance ()->DrawTextOnGlass (pDC, strText, rectText, uiDTFlags, 6, 
			IsWindowEnabled () ? globalData.clrBarText : globalData.clrGrayedText);
	}

	pDC->SelectObject (pOldFont);
}
//****
LRESULT CBCGPStatic::OnBCGSetControlAero (WPARAM wp, LPARAM)
{
	m_bOnGlass = (BOOL) wp;
	return 0;
}
//****
LRESULT CBCGPStatic::OnBCGSetControlVMMode (WPARAM wp, LPARAM)
{
	m_bVisualManagerStyle = (BOOL) wp;
	return 0;
}
//****
LRESULT CBCGPStatic::OnSetText (WPARAM, LPARAM)
{
	LRESULT lr = Default ();

	if (GetParent () != NULL)
	{
		CRect rect;
		GetWindowRect (rect);

		GetParent ()->ScreenToClient (&rect);
		GetParent ()->RedrawWindow (rect);
	}

	return lr;
}
//****
void CBCGPStatic::OnEnable(BOOL bEnable) 
{
	CStatic::OnEnable(bEnable);

	if (GetParent () != NULL)
	{
		CRect rect;
		GetWindowRect (rect);

		GetParent ()->ScreenToClient (&rect);
		GetParent ()->RedrawWindow (rect);
	}
}
