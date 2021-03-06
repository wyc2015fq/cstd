//
// BCGPHotSpotImageCtrl.cpp : implementation file
//

#include "bcgprores.h"
#include "bcgglobals.h"

#include "BCGPHotSpotImageCtrl.h"
#include "BCGPDrawManager.h"
#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#include "BCGPPngImage.h"
#endif


#ifndef BCGP_EXCLUDE_HOT_SPOT_IMAGE

/////////////////////////////////////////////////////////////////////////////
// CBCGPHotSpot

IMPLEMENT_SERIAL(CBCGPHotSpot, CObject, 1)

CBCGPHotSpot::CBCGPHotSpot(UINT nID) :
	m_nID (nID)
{
	m_rect.SetRectEmpty();
}
//****
CBCGPHotSpot::~CBCGPHotSpot()
{
}
//****
void CBCGPHotSpot::CreateRect (CRect rect)
{
	m_rect = rect;
}
//****
BOOL CBCGPHotSpot::IsPointWithin (CPoint pt) const
{
	return m_rect.PtInRect (pt);
}
//****
void CBCGPHotSpot::Serialize(CArchive& ar)
{
	CObject::Serialize (ar);

	if (ar.IsLoading())
	{
		ar >> (ULONG&) m_nID;
		ar >> m_rect;
	}
	else
	{
		ar << m_nID;
		ar << m_rect;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPHotSpotImageCtrl

CBCGPHotSpotImageCtrl::CBCGPHotSpotImageCtrl()
{
	m_bAutoDestroyImage	= FALSE;
	m_hbmpImage			= NULL;
	m_hbmpHot			= NULL;
	m_sizeImage			= CSize (0, 0);
	m_clrBack			= _RGB (255, 255, 255);

	m_pHot				= NULL;
	m_pClicked			= NULL;

	m_nSelectedNum		= 0;

	m_lineDev			= CSize (0, 0);
	m_pageDev			= CSize (0, 0);

	m_nHighlightPercentage = 0;
	m_nHighlightTolerance = 0;

	m_clrTransparentCurr = (COLORREF)-1;

	m_bScrollBars		= TRUE;
}
//****
CBCGPHotSpotImageCtrl::~CBCGPHotSpotImageCtrl()
{
	CleanUp();
}

BEGIN_MESSAGE_MAP(CBCGPHotSpotImageCtrl, CButton)
	//{{AFX_MSG_MAP(CBCGPHotSpotImageCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STYLECHANGING, OnStyleChanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPHotSpotImageCtrl message handlers

BOOL CBCGPHotSpotImageCtrl::Create(const IRECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CButton::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);
}
//****
BOOL CBCGPHotSpotImageCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= (BS_OWNERDRAW | WS_HSCROLL | WS_VSCROLL);
	cs.style &= ~BS_DEFPUSHBUTTON;
	cs.dwExStyle &= ~WS_EX_LAYOUTRTL;

	return CButton::PreCreateWindow(cs);
}
//****
void CBCGPHotSpotImageCtrl::PreSubclassWindow() 
{
	ModifyStyle (BS_DEFPUSHBUTTON, BS_OWNERDRAW | WS_HSCROLL | WS_VSCROLL);
	ModifyStyleEx (WS_EX_LAYOUTRTL, 0);

	CButton::PreSubclassWindow();

	UpdateScrollBars();
}
//****
void CBCGPHotSpotImageCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT (lpDIS != NULL);
	ASSERT (lpDIS->CtlType == ODT_BUTTON);

	CDC* pDCPaint = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDCPaint);

	OnPrepareDC (pDCPaint);

	CRect rect = lpDIS->rcItem;

	rect.r += GetScrollPos (SB_HORZ);
	rect.b += GetScrollPos (SB_VERT);

	CRect rectClip;
	int nClipType = pDCPaint->GetClipBox (rectClip);
	if (nClipType != SIMPLEREGION)
	{
		rectClip = rect;
	}

	CBCGPMemDC memDC (*pDCPaint, this);
	CDC* pDC = &memDC.GetDC();

	pDC->FillSolidRect (rectClip, m_clrBack);

	OnDrawImage (pDC, rect);

	CBCGPHotSpot* pHot = m_pHot;
	if (m_pClicked != NULL)
	{
		pHot = m_pClicked;
	}

	if (pHot != NULL)
	{
		CRect rectHotSpot = pHot->m_rect;
		m_clrTransparentCurr = pDC->GetPixel (rectHotSpot.TopLeft());

		pDC->LPtoDP (&rectHotSpot);

		CRgn rgn;
		rgn.CreateRectRgnIndirect (rectHotSpot);

		pDC->SelectClipRgn (&rgn);

		OnDrawHotSpot (pDC, rect, pHot);

		pDC->SelectClipRgn (NULL);
	}
}
//****
void CBCGPHotSpotImageCtrl::OnPrepareDC(CDC* pDC)
{
	ASSERT_VALID(pDC);

	pDC->SetMapMode(MM_TEXT);

	CPoint ptVpOrg(0, 0);       // assume no shift for printing
	ASSERT(pDC->GetWindowOrg() == CPoint(0,0));

	// by default shift viewport origin in negative direction of scroll
	ptVpOrg = -GetDeviceScrollPosition();

	pDC->SetViewportOrg(ptVpOrg);
}
//****
void CBCGPHotSpotImageCtrl::OnDrawImage (CDC* pDC, CRect rectImage)
{
	ASSERT_VALID (pDC);

	if (m_hbmpImage == NULL)
	{
		return;
	}

	rectImage.r = min (rectImage.r, m_sizeImage.w);
	rectImage.b = min (rectImage.b, m_sizeImage.h);

	pDC->DrawState (rectImage.TopLeft(), rectImage.Size(), m_hbmpImage, DSS_NORMAL);
}
//****
void CBCGPHotSpotImageCtrl::OnDrawHotSpot (CDC* pDC, CRect rectImage, CBCGPHotSpot* /*pHotSpot*/)
{
	ASSERT_VALID (pDC);

	rectImage.r = min (rectImage.r, m_sizeImage.w);
	rectImage.b = min (rectImage.b, m_sizeImage.h);

	if (m_hbmpHot != NULL)
	{
		pDC->DrawState (rectImage.TopLeft(), rectImage.Size(), m_hbmpHot, DSS_NORMAL);
	}

	if (m_nHighlightPercentage > 0)
	{
		CBCGPDrawManager dm (*pDC);
		dm.HighlightRect (rectImage, m_nHighlightPercentage,
			m_clrTransparentCurr, m_nHighlightTolerance);
	}
}
//****
BOOL CBCGPHotSpotImageCtrl::SetImage (UINT uiBmpResId, UINT uiBmpHotResId/* = 0*/)
{
	CleanUp();

	if (uiBmpResId == 0)
	{
		UpdateScrollBars();
		return TRUE;
	}

	CBitmap* hbmp = NULL;

	CBCGPPngImage pngImage;
	if (pngImage.Load (uiBmpResId))
	{
		hbmp = (CBitmap*) pngImage.Detach();
	}
	else
	{
		hbmp = (CBitmap*) ::LoadImage (
			AfxGetResourceHandle(),
			MAKEINTRESOURCE (uiBmpResId),
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
	}

	ASSERT (hbmp != NULL);

	CBitmap* hbmpHot = NULL;

	if (uiBmpHotResId != 0)
	{
		if (pngImage.Load (uiBmpHotResId))
		{
			hbmp = (CBitmap*) pngImage.Detach();
		}
		else
		{
			hbmpHot = (CBitmap*) ::LoadImage (
				AfxGetResourceHandle(),
				MAKEINTRESOURCE (uiBmpHotResId),
				IMAGE_BITMAP,
				0, 0,
				LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
		}

		ASSERT (hbmpHot != NULL);
	}

	return SetImage (hbmp, hbmpHot, TRUE /* Auto-destoy */);
}
//****
BOOL CBCGPHotSpotImageCtrl::SetImage (CBitmap* hbmpImage, CBitmap* hbmpImageHot/* = NULL*/, BOOL bAutoDestroy/* = FALSE*/)
{
	CleanUp();

	ASSERT (hbmpImage != NULL);

	m_hbmpImage = hbmpImage;
	m_hbmpHot = hbmpImageHot;

	BITMAP bmp;
	::GetObject (m_hbmpImage, sizeof (BITMAP), (LPVOID) &bmp);

	m_sizeImage.w = bmp.bmWidth;
	m_sizeImage.h = bmp.bmHeight;

	m_pageDev.w = m_sizeImage.w / 10;
	m_pageDev.h = m_sizeImage.h / 10;
	m_lineDev.w = m_pageDev.w / 10;
	m_lineDev.h = m_pageDev.h / 10;

	if (m_hbmpHot != NULL)
	{
		// Hot and cold bitmaps should have the same size!
		::GetObject (m_hbmpHot, sizeof (BITMAP), (LPVOID) &bmp);

		ASSERT (m_sizeImage.w == bmp.bmWidth);
		ASSERT (m_sizeImage.h == bmp.bmHeight);
	}

	m_bAutoDestroyImage = bAutoDestroy;

	if (GetSafeHwnd() != NULL)
	{
		UpdateScrollBars();
		RedrawWindow();
	}

	return TRUE;
}
//****
void CBCGPHotSpotImageCtrl::CleanUp()
{
	if (m_bAutoDestroyImage)
	{
		if (m_hbmpImage != NULL)
		{
			::DeleteObject (m_hbmpImage);
		}

		if (m_hbmpHot != NULL)
		{
			::DeleteObject (m_hbmpHot);
		}
	}

	while (!m_lstHotSpots.IsEmpty())
	{
		delete m_lstHotSpots.RemoveHead();
	}

	m_hbmpImage = NULL;
	m_hbmpHot = NULL;
	m_sizeImage = CSize (0, 0);

	m_lineDev = CSize (0, 0);
	m_pageDev = CSize (0, 0);

	UpdateScrollBars();
}
//****
BOOL CBCGPHotSpotImageCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//****
void CBCGPHotSpotImageCtrl::SetBackgroundColor (COLORREF color)
{
	m_clrBack = color;

	if (GetSafeHwnd() != NULL)
	{
		RedrawWindow();
	}
}
//****
CBCGPHotSpot* CBCGPHotSpotImageCtrl::FindHotSpot (UINT nID) const
{
	for (POSITION pos = m_lstHotSpots.GetHeadPosition(); pos != NULL;)
	{
		CBCGPHotSpot* pHotSpot = m_lstHotSpots.GetNext (pos);
		ASSERT_VALID (pHotSpot);

		if (pHotSpot->m_nID == nID)
		{
			return pHotSpot;
		}
	}

	return NULL;
}
//****
BOOL CBCGPHotSpotImageCtrl::AddHotSpot (UINT nID, CRect rect)
{
	if (FindHotSpot (nID) != NULL)
	{
		// nID should be unique!
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT (nID != 0);

	CBCGPHotSpot* pHotSpot = new CBCGPHotSpot (nID);
	pHotSpot->CreateRect (rect);

	m_lstHotSpots.AddTail (pHotSpot);

	return TRUE;
}
//****
BOOL CBCGPHotSpotImageCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint point;
	GetCursorPos (&point);
	ScreenToClient (&point);

	if (HitTest (point) != NULL)
	{
		::SetCursor (globalData.GetHandCursor());
		return TRUE;
	}
	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}
//****
CBCGPHotSpot* CBCGPHotSpotImageCtrl::HitTest (CPoint pt)
{
	CRect rectClient;
	GetClientRect (rectClient);

	if (!rectClient.PtInRect (pt))
	{
		return NULL;
	}

	CClientDC dc (this);
	OnPrepareDC (&dc);

	CPoint ptScreen = pt;
	ClientToScreen (&ptScreen);

	dc.DPtoLP (&pt);

	for (POSITION pos = m_lstHotSpots.GetHeadPosition(); pos != NULL;)
	{
		CBCGPHotSpot* pHotSpot = m_lstHotSpots.GetNext (pos);
		ASSERT_VALID (pHotSpot);

		if (pHotSpot->IsPointWithin (pt))
		{
			if (::WindowFromPoint (ptScreen) != GetSafeHwnd())
			{
				return NULL;
			}

			return pHotSpot;
		}
	}

	return NULL;
}
//****
void CBCGPHotSpotImageCtrl::RedrawRect (CRect rect)
{
	CClientDC dc (this);
	OnPrepareDC (&dc);

	dc.LPtoDP (&rect);

	RedrawWindow (rect, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
//****
void CBCGPHotSpotImageCtrl::OnMouseMove(UINT /*nFlags*/, CPoint point) 
{
	CBCGPHotSpot* pHot = HitTest (point);

	if (m_pHot == pHot)
	{
		return;
	}

	// ----------------
	// No new hot areas
	// ----------------
	if (pHot == NULL)
	{
		if (m_pHot != NULL)
		{
			// remove old hotlight
			CRect rectUpdate = m_pHot->m_rect;
			m_pHot = NULL;

			if (HasHotImage() || m_nHighlightPercentage > 0)
			{
				RedrawRect (rectUpdate);
			}

			if (m_pClicked == NULL)
			{
				ReleaseCapture();
			}
		}
		
		return;
	}
	else if (pHot != NULL)
	{
		// ------------
		// New hot spot
		// ------------
		if (m_pHot == NULL)
		{
			if (GetCapture() != NULL)
			{
				return;
			}

			SetCapture();
		}
		else
		{
			// remove old hotlight
			CRect rectHotSpot = m_pHot->m_rect;
			m_pHot = NULL;

			if (HasHotImage() || m_nHighlightPercentage > 0)
			{
				RedrawRect (rectHotSpot);
			}
		}
		
		// add new hotlight
		m_pHot = pHot;

		if (HasHotImage() || m_nHighlightPercentage > 0)
		{
			RedrawRect (pHot->m_rect);
		}
	}
}
//****
BOOL CBCGPHotSpotImageCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (CBCGPPopupMenu::GetActiveMenu() != NULL)
	{
		return TRUE;
	}

	if (nFlags & (MK_SHIFT | MK_CONTROL))
	{
		return FALSE;
	}
	
	UNUSED_ALWAYS(pt);

	DWORD dwStyle = GetStyle();
	CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
	BOOL bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_VSCROLL);

	SCROLLINFO scrollInfo;
	ZeroMemory(&scrollInfo, sizeof(SCROLLINFO));

    scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;

	GetScrollInfo (bHasVertBar ? SB_VERT : SB_HORZ, &scrollInfo);

	if (scrollInfo.nPos >= scrollInfo.nMax)
	{
		return FALSE;
	}

	int nSteps = abs(zDelta) / WHEEL_DELTA * 4;

	for (int i = 0; i < nSteps; i++)
	{
		UINT nSBCode = zDelta < 0 ? SB_LINEDOWN : SB_LINEUP;

		if (bHasVertBar)
		{
			OnVScroll (nSBCode, 0, NULL);
		}
		else
		{
			OnHScroll (nSBCode, 0, NULL);
		}
	}

	return TRUE;
}
//****
void CBCGPHotSpotImageCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint point) 
{
	m_nSelectedNum = 0;
	m_pClicked = HitTest (point);

	SetFocus();
}
//****
void CBCGPHotSpotImageCtrl::OnLButtonUp(UINT /*nFlags*/, CPoint point) 
{
	if (m_pHot == NULL && m_pClicked == NULL)
	{
		return;
	}

	ReleaseCapture();

	BOOL bIsHotSpotClick = (m_pHot != NULL && m_pHot == HitTest (point) &&
		m_pClicked == m_pHot);

	CRect rectHotSpot = (m_pHot != NULL) ? m_pHot->m_rect : CRect (0, 0, 0, 0);
	CRect rectClicked = (m_pClicked != NULL) ? m_pClicked->m_rect : CRect (0, 0, 0, 0);

	UINT nSelectedNum = m_pHot != NULL ? m_pHot->m_nID : 0;

	m_pHot = NULL;
	m_pClicked = NULL;

	if (HasHotImage() || m_nHighlightPercentage > 0)
	{
		RedrawRect (rectHotSpot);

		if (!rectClicked.IsRectEmpty() && rectClicked != rectHotSpot)
		{
			RedrawRect (rectClicked);
		}
	}

	if (bIsHotSpotClick)
	{
		CWnd* pParent = GetParent();
		if (pParent != NULL)
		{
			m_nSelectedNum = nSelectedNum;

			HWND hwnd = GetSafeHwnd();

			pParent->SendMessage (	WM_COMMAND,
									MAKEWPARAM (GetDlgCtrlID(), BN_CLICKED),
									(LPARAM) m_hWnd);

			if (::IsWindow (hwnd))
			{
				m_nSelectedNum = 0;
			}
		}
	}
}
//****
void CBCGPHotSpotImageCtrl::OnCancelMode() 
{
	CButton::OnCancelMode();
	
	if (m_pHot != NULL || m_pClicked != NULL)
	{
		CRect rectHotSpot = m_pHot != NULL ? m_pHot->m_rect : CRect (0, 0, 0, 0);
		CRect rectClicked = (m_pClicked != NULL) ? m_pClicked->m_rect : CRect (0, 0, 0, 0);

		m_pHot = NULL;
		m_pClicked = NULL;
		ReleaseCapture();

		if (HasHotImage() || m_nHighlightPercentage > 0)
		{
			RedrawRect (rectHotSpot);

			if (!rectClicked.IsRectEmpty() && rectClicked != rectHotSpot)
			{
				RedrawRect (rectClicked);
			}
		}
	}

	m_pClicked = NULL;
	m_nSelectedNum = 0;
}
//****
void CBCGPHotSpotImageCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CButton::OnSize(nType, cx, cy);
	
	UpdateScrollBars();
}
//****
void CBCGPHotSpotImageCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	OnScroll(MAKEWORD(-1, nSBCode), nPos);
}
//****
void CBCGPHotSpotImageCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
}
//****
void CBCGPHotSpotImageCtrl::UpdateScrollBars()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	if (!m_bScrollBars)
	{
		SetScrollRange (SB_HORZ, 0, 0, TRUE);
		SetScrollRange (SB_VERT, 0, 0, TRUE);
		return;
	}

	static BOOL bInUpdate = FALSE;
	if (bInUpdate)
	{
		return;
	}

	bInUpdate = TRUE;

	CRect rectClient;
	GetClientRect (rectClient);

	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof (SCROLLINFO);
	scrollInfo.fMask = SIF_RANGE | SIF_PAGE;
	scrollInfo.nMin = 0;
	scrollInfo.nMax = m_sizeImage.w;
	scrollInfo.nPage = rectClient.Width();

	if (!SetScrollInfo (SB_HORZ, &scrollInfo))
	{
		SetScrollRange (SB_HORZ, 0, m_sizeImage.w, TRUE);
	}

	scrollInfo.nMax = m_sizeImage.h;
	scrollInfo.nPage = rectClient.Height();
		
	if (!SetScrollInfo (SB_VERT, &scrollInfo))
	{
		SetScrollRange (SB_VERT, 0, m_sizeImage.h, TRUE);
	}

	bInUpdate = FALSE;
}
//****
int CBCGPHotSpotImageCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	UpdateScrollBars();
	return 0;
}
//****
BOOL CBCGPHotSpotImageCtrl::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// calc new x position
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.w;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.w;
		break;
	case SB_PAGEUP:
		x -= m_pageDev.w;
		break;
	case SB_PAGEDOWN:
		x += m_pageDev.w;
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_lineDev.h;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.h;
		break;
	case SB_PAGEUP:
		y -= m_pageDev.h;
		break;
	case SB_PAGEDOWN:
		y += m_pageDev.h;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
		UpdateWindow();

	return bResult;
}
//****
BOOL CBCGPHotSpotImageCtrl::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.h = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.w = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.w;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.h;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
		RedrawWindow();
	}

	return TRUE;
}
//****
CPoint CBCGPHotSpotImageCtrl::GetDeviceScrollPosition() const
{
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);

	return pt;
}
//****
LRESULT CBCGPHotSpotImageCtrl::OnStyleChanging (WPARAM wp, LPARAM lp)
{
	int nStyleType = (int) wp; 
	LPSTYLESTRUCT lpStyleStruct = (LPSTYLESTRUCT) lp;

	CButton::OnStyleChanging (nStyleType, lpStyleStruct);

	if (nStyleType == GWL_EXSTYLE)
	{
		lpStyleStruct->styleNew &= ~WS_EX_LAYOUTRTL;
	}

	return 0;
}
//****
void CBCGPHotSpotImageCtrl::SetHighlightHotArea (int nPercentage, int nTolerance/* = 0*/)
{
	m_nHighlightPercentage = nPercentage;
	m_nHighlightTolerance = nTolerance;
}
//****
void CBCGPHotSpotImageCtrl::EnableScrollBars (BOOL bEnable/* = TRUE*/)
{
	if (m_bScrollBars == bEnable)
	{
		return;
	}

	m_bScrollBars = bEnable;
	UpdateScrollBars();
}

#endif // BCGP_EXCLUDE_HOT_SPOT_IMAGE
