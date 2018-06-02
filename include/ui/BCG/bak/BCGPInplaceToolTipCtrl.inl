
// BCGPInplaceToolTipCtrl.cpp : implementation file
//

//#include "multimon.h"
//#include "BCGCBPro.h"
#include "BCGGlobals.h"
#include "BCGPInplaceToolTipCtrl.h"

#ifndef _BCGSUITE_
	#include "BCGPToolTipCtrl.h"
	#define visualManagerMFC	CBCGPVisualManager::GetInstance()
#else
	#define visualManagerMFC	CMFCVisualManager::GetInstance()
#endif

#include "BCGPVisualManager.h"
#include "BCGPDrawManager.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPInplaceToolTipCtrl

CString CBCGPInplaceToolTipCtrl::m_strClassName;

IMPLEMENT_DYNAMIC(CBCGPInplaceToolTipCtrl, CWnd)

CBCGPInplaceToolTipCtrl::CBCGPInplaceToolTipCtrl()
{
	m_rectLast.SetRectEmpty();
	m_nTextMargin = 10;
	m_pFont	= NULL;
	m_pWndParent = NULL;
}

CBCGPInplaceToolTipCtrl::~CBCGPInplaceToolTipCtrl()
{
}


BEGIN_MESSAGE_MAP(CBCGPInplaceToolTipCtrl, CWnd)
	//{{AFX_MSG_MAP(CBCGPInplaceToolTipCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPInplaceToolTipCtrl message handlers

BOOL CBCGPInplaceToolTipCtrl::Create (CWnd* pWndParent) 
{
	ASSERT_VALID (pWndParent);
	m_pWndParent = pWndParent;

	if (m_strClassName.IsEmpty())
	{
		m_strClassName = ::AfxRegisterWndClass (
			CS_SAVEBITS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_BTNFACE + 1));
	}  

	return CreateEx (0,
					m_strClassName, _T (""), WS_POPUP,
					0, 0, 0, 0,
					pWndParent->GetSafeHwnd(), (HMENU) NULL);
}

BOOL CBCGPInplaceToolTipCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CBCGPInplaceToolTipCtrl::OnPaint(CDC* pDC) 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect (rect);

	CBCGPToolTipParams params;
	visualManagerMFC->GetToolTipParams (params);

	if (params.m_clrFill == (COLORREF)-1)
	{
		::FillRect (dc.GetSafeHdc(), rect, GetSysColorBrush (COLOR_INFOBK));
	}
	else
	{
		if (params.m_clrFillGradient == (COLORREF)-1)
		{
			CBrush br (params.m_clrFill);
			dc.FillRect (rect, &br);
		}
		else
		{
			CBCGPDrawManager dm (dc);

			dm.FillGradient2 (rect, 
				params.m_clrFillGradient, params.m_clrFill,
				params.m_nGradientAngle == -1 ? 90 : params.m_nGradientAngle);
		}
	}

	COLORREF clrLine = params.m_clrBorder == (COLORREF)-1 ?
		_GetSysColor(CLR_INFOTEXT) : params.m_clrBorder;

	COLORREF clrText = params.m_clrText == (COLORREF)-1 ?
		_GetSysColor(CLR_INFOTEXT) : params.m_clrText;

	dc.Draw3dRect (rect, clrLine, clrLine);

	CFont* pPrevFont = m_pFont == NULL ?
		(CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT) :
		dc.SelectObject (m_pFont);

	dc.SetBkMode (TRANSPARENT);
	dc.SetTextColor(clrText);

	if (m_strText.FindOneOf (_T("\n")) != -1)	// multi-line tooltip
	{
		rect.DeflateRect (m_nTextMargin, m_nTextMargin);
		if (rect.Height() < m_rectLast.Height())
		{
			// center tooltip vertically
			rect.t += (m_rectLast.Height() - rect.Height()) / 2;
		}

		dc.DrawText (m_strText, rect, DT_LEFT | DT_WORDBREAK);
	}
	else // single line tooltip
	{
		rect.DeflateRect (m_nTextMargin, 0);
		dc.DrawText (m_strText, rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}

	if (pPrevFont != NULL)
	{
		dc.SelectObject (pPrevFont);
	}
}
//****
void CBCGPInplaceToolTipCtrl::Track (CRect rect, const CString& strText)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_rectLast == rect && m_strText == strText)
	{
		return;
	}

	ASSERT_VALID (m_pWndParent);

	m_rectLast = rect;
	m_strText = strText;

	CClientDC dc (this);

    ASSERT_VALID(m_pFont);
	CFont* pPrevFont = m_pFont == NULL ?
		(CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT) :
		dc.SelectObject (m_pFont);
	ASSERT (pPrevFont != NULL);

	int nTextHeight = rect.Height();
	int nTextWidth = rect.Width();
	if (m_strText.FindOneOf (_T("\n")) != -1)	// multi-line tooltip
	{
		const int nDefaultHeight = globalData.GetTextHeight();
		const int nDefaultWidth = 200;
		CRect rectText (0, 0, nDefaultWidth, nDefaultHeight);
 
		nTextHeight = dc.DrawText (m_strText, rectText, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
		nTextWidth = rectText.Width();
		nTextHeight += 2 * m_nTextMargin;
		nTextWidth += 2 * m_nTextMargin;
	}
	else
	{
		nTextWidth = dc.GetTextExtent (m_strText).w + 2 * m_nTextMargin;
	}

	dc.SelectObject (pPrevFont);

	if (m_pWndParent->GetExStyle() & WS_EX_LAYOUTRTL)
	{
		rect.l = rect.r - nTextWidth;
	}
	else
	{
		rect.r = rect.l + nTextWidth;
	}
	rect.b = rect.t + nTextHeight;
	if (rect.Height() < m_rectLast.Height())
	{
		rect.t = m_rectLast.t;
		rect.b = m_rectLast.b;
	}

	MONITORINFO mi;
	mi.cbSize = sizeof (MONITORINFO);

	CRect rectScreen;

	if (GetMonitorInfo (MonitorFromPoint (rect.TopLeft(), MONITOR_DEFAULTTONEAREST), &mi))
	{
		rectScreen = mi.rcWork;
	}
	else
	{
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
	}

	if (rect.Width() > rectScreen.Width())
	{
		rect.l = rectScreen.l;
		rect.r = rectScreen.r;
	}
	else if (rect.r > rectScreen.r)
	{
		rect.r = rectScreen.r;
		rect.l = rect.r - nTextWidth;
	}
	else if (rect.l < rectScreen.l)
	{
		rect.l = rectScreen.l;
		rect.r = rect.l + nTextWidth;
	}

	if (rect.Height() > rectScreen.Height())
	{
		rect.t = rectScreen.t;
		rect.b = rectScreen.b;
	}
	else if (rect.b > rectScreen.b)
	{
		rect.b = rectScreen.b;
		rect.t = rect.b - nTextHeight;
	}
	else if (rect.t < rectScreen.t)
	{
		rect.t = rectScreen.t;
		rect.b = rect.b + nTextHeight;
	}

	SetWindowPos (&wndTop, rect.l, rect.t, 
		rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOOWNERZORDER);
  
    ShowWindow (SW_SHOWNOACTIVATE);
	Invalidate();
	UpdateWindow();

	SetCursor (AfxGetApp()->LoadStandardCursor (IDC_ARROW));
}
//****
void CBCGPInplaceToolTipCtrl::Hide()
{
	if (GetSafeHwnd() != NULL)
	{
		ShowWindow (SW_HIDE);
	}
}
//****
void CBCGPInplaceToolTipCtrl::Deactivate()
{
	m_strText.Empty();
	m_rectLast.SetRectEmpty();

	Hide();
}
//****
LRESULT CBCGPInplaceToolTipCtrl::OnSetFont (WPARAM wParam, LPARAM lParam)
{
	BOOL bRedraw = (BOOL) LOWORD (lParam);

    m_pFont = CFont::FromHandle((HFONT) wParam);

	if (bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}

	return 0;
}
//****
BOOL CBCGPInplaceToolTipCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_MOUSEFIRST &&
		pMsg->message <= WM_MOUSELAST)
	{
		if (pMsg->message != WM_MOUSEMOVE)
		{
			Hide();
		}

		ASSERT_VALID (m_pWndParent);

		// the parent should receive the mouse message in its client coordinates
		CPoint pt (LOWORD (pMsg->lParam), HIWORD (pMsg->lParam));
		MapWindowPoints (m_pWndParent, &pt, 1);
		LPARAM lParam = MAKELPARAM (pt.x, pt.y);

		m_pWndParent->SendMessage (pMsg->message, pMsg->wParam, lParam);
		return TRUE;
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}
