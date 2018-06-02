//
// BCGPFormView.cpp : implementation file
//

#include "BCGPFormView.h"
#include "BCGPVisualManager.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPFormView

IMPLEMENT_DYNAMIC(CBCGPFormView, CFormView)

#pragma warning (disable : 4355)

CBCGPFormView::CBCGPFormView(LPCTSTR lpszTemplateName)
	: CFormView(lpszTemplateName),
	m_Impl (*this)
{
}

CBCGPFormView::CBCGPFormView(UINT nIDTemplate)
	: CFormView(nIDTemplate),
	m_Impl (*this)
{
}

#pragma warning (default : 4355)

CBCGPFormView::~CBCGPFormView()
{
}

BEGIN_MESSAGE_MAP(CBCGPFormView, CFormView)
	//{{AFX_MSG_MAP(CBCGPFormView)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPFormView diagnostics

#ifdef _DEBUG
void CBCGPFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBCGPFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPFormView message handlers

HBRUSH CBCGPFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (IsVisualManagerStyle())
	{
		HBRUSH hbr = m_Impl.OnCtlColor (pDC, pWnd, nCtlColor);
		if (hbr != NULL)
		{
			return hbr;
		}
	}	

	return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}
//****
BOOL CBCGPFormView::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (rectClient);

	if (IsVisualManagerStyle() &&
		CBCGPVisualManager::GetInstance()->OnFillDialog (pDC, this, rectClient))
	{
		return TRUE;
	}

	return CFormView::OnEraseBkgnd(pDC);
}
//****
void CBCGPFormView::EnableVisualManagerStyle (BOOL bEnable, const CList<UINT,UINT>* plstNonSubclassedItems)
{
	ASSERT_VALID (this);

	m_Impl.EnableVisualManagerStyle (bEnable, FALSE, plstNonSubclassedItems);
	m_Impl.m_bTransparentStaticCtrls = FALSE;
}
//****
void CBCGPFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if (IsVisualManagerStyle())
	{
		m_Impl.EnableVisualManagerStyle (TRUE);
	}
}
//****
void CBCGPFormView::OnDestroy() 
{
	m_Impl.OnDestroy();
	CFormView::OnDestroy();
}
