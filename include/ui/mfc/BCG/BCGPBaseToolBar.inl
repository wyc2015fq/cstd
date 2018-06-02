
//
// BCGPBaseToolBar.cpp : implementation file
//


#include "BCGPDockBar.h"

#include "BCGPBaseToolBar.h"

IMPLEMENT_DYNCREATE(CBCGPBaseToolBar,CBCGPControlBar)

/////////////////////////////////////////////////////////////////////////////
// CBCGPBaseToolBar

CBCGPBaseToolBar::CBCGPBaseToolBar()
{
}

CBCGPBaseToolBar::~CBCGPBaseToolBar()
{
}


BEGIN_MESSAGE_MAP(CBCGPBaseToolBar, CBCGPControlBar)
	//{{AFX_MSG_MAP(CBCGPBaseToolBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CBCGPBaseToolBar message handlers

void CBCGPBaseToolBar::OnAfterChangeParent (CWnd* /*pWndOldParent*/)
{
}

void CBCGPBaseToolBar::OnAfterStretch (int /*nStretchSize*/)
{
	CRect rectClient;
	GetClientRect (&rectClient);

	if (rectClient.Width () != m_rectVirtual.Width ())
	{
		Invalidate ();
		UpdateWindow ();
	}
}

