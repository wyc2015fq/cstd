// BCGPInplaceEdit.cpp : implementation file
//

#include "BCGPInplaceEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPInplaceEdit

CBCGPInplaceEdit::CBCGPInplaceEdit()
{
	m_bResizeEditor = FALSE;// if TRUE, editor changes its height and width on content changing
	m_bWrapText = FALSE;	// if FALSE, editor enlarge its width to fit text

	m_rectInital.SetRectEmpty();	// Initial bounding rect of the editor
	m_nTopMargin = 0;				// Top margin for vertical alignment
	m_rectEditMax.SetRectEmpty();	// Maximal size of the editor

	m_nVStep = 18;
	m_nHLeftStep = 30;
	m_nHRightStep = 30;
	m_VertAlign = VA_Top;
}

CBCGPInplaceEdit::~CBCGPInplaceEdit()
{
}


BEGIN_MESSAGE_MAP(CBCGPInplaceEdit, CEdit)
	//{{AFX_MSG_MAP(CBCGPInplaceEdit)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdateR)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPInplaceEdit message handlers

void CBCGPInplaceEdit::OnUpdateR() 
{
	ResizeEditor();
}
//****
int CBCGPInplaceEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;
	GetWindowRect (rect);
	GetParent()->ScreenToClient (rect);
	m_rectInital = rect;

	DWORD dwStyle = GetStyle();
	BOOL bAutoHScroll = (dwStyle & ES_AUTOHSCROLL) != 0;
	BOOL bMultiLine = (dwStyle & ES_MULTILINE) != 0;
	m_bWrapText = bMultiLine && !bAutoHScroll;

	ResizeEditor();

	return 0;
}
//****
LRESULT CBCGPInplaceEdit::OnSetText (WPARAM, LPARAM)
{
	LRESULT lRes = Default();

	ResizeEditor();

	return lRes;
}
//****
void CBCGPInplaceEdit::ResizeEditor()
{
	ASSERT_VALID (this);

	CString str;
	CWnd::GetWindowText(str);

	CRect rect;
	GetWindowRect (rect);
	GetParent()->ScreenToClient (rect);

	CDC* pDC = GetWindowDC();

	//-------------
	// Select font:
	//-------------
	CFont* pFont = GetFont();
	if (pFont != NULL)
	{
		ASSERT_VALID (pFont);
		pDC->SelectObject (pFont);
	}

	UINT uiTextFlags = DT_LEFT | DT_NOPREFIX | DT_CALCRECT;

	//---------------
	// Word wrapping:
	//---------------
	BOOL bMultiLine = (GetStyle() & ES_MULTILINE) != 0;
	if (!bMultiLine)
	{
		uiTextFlags |= DT_SINGLELINE | DT_VCENTER;
	}
	else if (bMultiLine && m_bWrapText)
	{
		uiTextFlags |= DT_WORDBREAK;
	}
	else // multiline && no wraptext
	{
	}


	//-----------
	// Alignment:
	//-----------
	DWORD dwStyle = GetStyle();
	if ((dwStyle & ES_CENTER) != 0)
	{
		uiTextFlags |= DT_CENTER;
	}
	else if ((dwStyle & ES_RIGHT) != 0)
	{
		uiTextFlags |= DT_RIGHT;
	}

	//-----------------------
	// Calculate text extent:
	//-----------------------
	CRect rectNew = rect;

	if (str.IsEmpty())
	{
		str = _T("W");
	}
	pDC->DrawText (str, rectNew, uiTextFlags);

	const int nMinWidth = pDC->GetTextExtent (_T("W")).w;
	rectNew.r = max (rectNew.l + nMinWidth, rectNew.r);

	//---------
	// Padding:
	//---------
	if (m_VertAlign == VA_Top) // GRID_VTOP
	{
		int nTopMargin = 0;
		m_nTopMargin = max (0, nTopMargin);
	}
	if (m_VertAlign == VA_Center) // GRID_VCENTER
	{
		CPoint ptCenter = m_rectInital.CenterPoint();
		int nTop = ptCenter.y - rectNew.Height() / 2;
		int nTopMargin = nTop - rectNew.t;
		m_nTopMargin = max (0, nTopMargin);
	}
	if (m_VertAlign == VA_Bottom) // GRID_VBOTTOM
	{
		int nTop = m_rectInital.b - rectNew.Height();
		int nTopMargin = nTop - rectNew.t;
		m_nTopMargin = max (0, nTopMargin);
	}
	ASSERT (m_nTopMargin >= 0);

	//---------------------------
	// Inflate editor if allowed:
	//---------------------------
	BOOL bResize = FALSE;
	if (CanEnlargeHeight())
	{
		bResize = (rectNew.Height() != m_rectInital.Height());
		rectNew.b = max (m_rectInital.b, rectNew.b);
	}
	else
	{
		rectNew.b = m_rectInital.b;
	}
	if (CanEnlargeWidth())
	{
		if (rectNew.Width() > m_rectInital.Width())
		{
			bResize = TRUE;

			DWORD dwStyle = GetStyle();
			if ((dwStyle & ES_CENTER) != 0)
			{
				m_rectInital.l -= m_nHLeftStep;
				m_rectInital.r += m_nHRightStep;
			}
			else if ((dwStyle & ES_RIGHT) != 0)
			{
				rectNew.l -= (rectNew.Width() > m_rectInital.Width());
				rectNew.r = m_rectInital.r;
				m_rectInital.l -= m_nHLeftStep;
			}
			else // (dwStyle & ES_LEFT) != 0
			{
				m_rectInital.r += m_nHRightStep;
			}
		}

		rectNew.l = min (rectNew.l, m_rectInital.l);
		rectNew.r = max (rectNew.r, m_rectInital.r);

	}
	else
	{
		rectNew.l = m_rectInital.l;
		rectNew.r = m_rectInital.r;
	}

	//----------------------------
	// Vertically align the editor:
	//----------------------------
	if (bResize)
	{
		SetWindowPos (NULL, rectNew.l, rectNew.t, rectNew.Width(), rectNew.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}

	CRect rectFormating (0, 0, rectNew.Width(), rectNew.Height());
	rectFormating.t += m_nTopMargin;
	SendMessage (EM_SETRECT, 0, (LPARAM)(IRECT*)&rectFormating);
}
