//
// BCGPSplitterWnd.cpp : implementation file
//

//#include "BCGCBPro.h"
#include "BCGPVisualManager.h"
#include "BCGPSplitterWnd.h"

IMPLEMENT_DYNAMIC(CBCGPSplitterWnd, CSplitterWnd)

/////////////////////////////////////////////////////////////////////////////
// CBCGPSplitterWnd

CBCGPSplitterWnd::CBCGPSplitterWnd()
{
}

CBCGPSplitterWnd::~CBCGPSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CBCGPSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CBCGPSplitterWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPSplitterWnd message handlers

void CBCGPSplitterWnd::OnDrawSplitter (CDC* pDC, ESplitType nType, 
									   const CRect& rectArg)
{
	// if pDC == NULL, then just invalidate
	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}

	CRect rect = rectArg;

	switch (nType)
	{
	case splitBorder:
		CBCGPVisualManager::GetInstance()->OnDrawSplitterBorder (pDC, this, rect);
		return;

	case splitBox:
		CBCGPVisualManager::GetInstance()->OnDrawSplitterBox (pDC, this, rect);
		break;

	case splitIntersection:
	case splitBar:
		break;

	default:
		ASSERT(FALSE);  // unknown splitter type
	}

	// fill the middle
	CBCGPVisualManager::GetInstance()->OnFillSplitterBackground (pDC, this, rect);
}

