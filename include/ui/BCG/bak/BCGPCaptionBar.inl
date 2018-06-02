// BCGPCaptionBar.cpp: implementation of the CBCGPCaptionBar class.
//
// This is a part of BCGControlBar Library Professional Edition
// Copyright (C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
 //////////////////////////////////////////////////////////////////////


#include "BCGPCaptionBar.h"
#include "BCGPVisualManager.h"
#include "BCGPToolBar.h"
#include "trackmouse.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPOleDocIPFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPGlobalUtils.h"
#include "BCGPTooltipManager.h"
#include "BCGPToolTipCtrl.h"

#include "BCGProRes.h"

const int nMenuArrowWidth = 10;
const int nButtonHorzMargin = 10;
const int nButtonVertMargin = 5;
const int nMessageBarMargin = 4;

const int nIdToolTipClose = 1;
const int nIdToolTipText = 2;
const int nIdToolTipImage = 3;
const int nIdToolTipButton = 4;

IMPLEMENT_DYNCREATE(CBCGPCaptionBar, CBCGPControlBar)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPCaptionBar::CBCGPCaptionBar() 
{
	m_pToolTip				= NULL;

	m_clrBarText			= (COLORREF)-1;
	m_clrBarBackground		= (COLORREF)-1;
	m_clrBarBorder			= (COLORREF)-1;

	m_nBorderSize			= 4;
	m_nMargin				= 4;
	m_nHorzElementOffset	= 4;

	m_hIcon					= NULL;
	m_hFont					= NULL;

	m_nDefaultHeight		= -1;
	m_nCurrentHeight		= 0;

	m_btnAlignnment			= ALIGN_LEFT;
	m_iconAlignment			= ALIGN_LEFT;
	m_textAlignment			= ALIGN_LEFT;

	m_bStretchImage			= FALSE;

	m_bFlatBorder			= FALSE;
	m_uiBtnID				= 0;

	m_bIsBtnPressed			= FALSE;
	m_bIsBtnForcePressed	= FALSE;
	m_bIsBtnHighlighted		= FALSE;

	m_bTracked				= FALSE;
	m_bBtnEnabled			= TRUE;
	m_bBtnHasDropDownArrow	= TRUE;

	m_rectImage.SetRectEmpty();
	m_rectText.SetRectEmpty();
	m_rectDrawText.SetRectEmpty();
	m_rectButton.SetRectEmpty();
	m_bTextIsTruncated		= FALSE;

	m_bIsMessageBarMode = FALSE;

	m_bIsCloseBtnPressed	= FALSE;
	m_bIsCloseBtnHighlighted= FALSE;
	m_bCloseTracked			= FALSE;
	
	m_rectClose.SetRectEmpty();
}
//****
CBCGPCaptionBar::~CBCGPCaptionBar()
{
}
//****
BOOL CBCGPCaptionBar::Create (DWORD dwStyle, CWnd* pParentWnd, UINT uID, int nHeight,
							  BOOL bIsMessageBarMode)
{
	SetBarStyle (CBRS_ALIGN_TOP);
	m_nDefaultHeight = nHeight;

	if (bIsMessageBarMode)
	{
		m_dwStyle |= CBRS_HIDE_INPLACE;
	}
	
	if (!CBCGPControlBar::Create (NULL, dwStyle, CRect (0, 0, 0, 0),
		pParentWnd, uID, 0))
	{
		return FALSE;
	}

	// register with parent frames' dock manager!!!
	if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
	{
		((CBCGPFrameWnd*) pParentWnd)->AddControlBar (this);
	}
	else if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
	{
		((CBCGPMDIFrameWnd*) pParentWnd)->AddControlBar (this);
	}
	else if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPOleIPFrameWnd)))
	{
		((CBCGPOleIPFrameWnd*) pParentWnd)->AddControlBar (this);
	}
	else if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPOleDocIPFrameWnd)))
	{
		((CBCGPOleIPFrameWnd*) pParentWnd)->AddControlBar (this);
	}
	else if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIChildWnd)))
	{
		((CBCGPMDIChildWnd*) pParentWnd)->AddControlBar (this);
	}
	else if (pParentWnd->IsKindOf (RUNTIME_CLASS (CDialog)))
	{
		globalUtils.m_bDialogApp = TRUE;
	}
	else
	{
		ASSERT (FALSE);
	}
	
	m_bIsMessageBarMode = bIsMessageBarMode;

	if (m_bIsMessageBarMode)
	{
		m_nBorderSize = 0;
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CBCGPCaptionBar, CBCGPControlBar)
	//{{AFX_MSG_MAP(CBCGPCaptionBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_REGISTERED_MESSAGE(BCGM_UPDATETOOLTIPS, OnBCGUpdateToolTips)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnNeedTipText)
END_MESSAGE_MAP()

void CBCGPCaptionBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
}
//****
int CBCGPCaptionBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPControlBar::OnCreate (lpCreateStruct) == -1)
		return -1;

	CBCGPTooltipManager::CreateToolTip (m_pToolTip, this,
		BCGP_TOOLTIP_TYPE_CAPTIONBAR);

	if (m_pToolTip->GetSafeHwnd() != NULL)
	{
		CRect rectDummy (0, 0, 0, 0);

		m_pToolTip->SetMaxTipWidth (640);

		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, nIdToolTipClose);
		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, nIdToolTipText);
		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, nIdToolTipImage);
		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, nIdToolTipButton);
	}

	SetWindowText (_T("Caption Bar"));
	return 0;
}
//****
void CBCGPCaptionBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPControlBar::OnSize(nType, cx, cy);
	RecalcLayout();
	InvalidateRect (NULL);
}
//****
void CBCGPCaptionBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	lpncsp->rgrc[0].b -= m_nBorderSize;
	lpncsp->rgrc[0].t  += m_nBorderSize;
}
//****
void CBCGPCaptionBar::OnPaint(CDC* pDC) 
{
	CPaintDC dcPaint (this);
	CBCGPMemDC memDC (dcPaint, this);
	CDC& dc = memDC.GetDC();

	CRect rectClient;
	GetClientRect(rectClient);

	OnDrawBackground (&dc, rectClient);

	int nOldBkMode = dc.SetBkMode (TRANSPARENT);
	COLORREF clrOldText = 
		dc.SetTextColor (m_clrBarText != (COLORREF) -1 ? m_clrBarText : 
		CBCGPVisualManager::GetInstance()->GetCaptionBarTextColor (this));

	CFont* pOldFont = dc.SelectObject (
		m_hFont == NULL ? &globalData.fontRegular : CFont::FromHandle (m_hFont));

	OnDrawButton (&dc, m_rectButton, m_strBtnText, m_bBtnEnabled);
	OnDrawText (&dc, m_rectDrawText, m_strText);

	if (!m_rectImage.IsRectEmpty())
	{
		OnDrawImage (&dc, m_rectImage);
	}

	if (!m_rectClose.IsRectEmpty())
	{
		COLORREF clrText = CBCGPVisualManager::GetInstance()->OnFillCaptionBarButton (&dc, this,
			m_rectClose, m_bIsCloseBtnPressed, m_bIsCloseBtnHighlighted, FALSE,
			FALSE, TRUE);

		CBCGPMenuImages::IMAGE_STATE imageState;
		
		if (GetRV (clrText) > 192 &&
			GetGV (clrText) > 192 &&
			GetBV (clrText) > 192)
		{
			imageState = CBCGPMenuImages::ImageWhite;
		}
		else
		{
			imageState = CBCGPMenuImages::ImageBlack;
		}

		CBCGPMenuImages::Draw (&dc, CBCGPMenuImages::IdClose, m_rectClose, imageState);

		CBCGPVisualManager::GetInstance()->OnDrawCaptionBarButtonBorder (&dc, this,
			m_rectClose, m_bIsCloseBtnPressed, m_bIsCloseBtnHighlighted, FALSE,
			FALSE, TRUE);
	}

	dc.SelectObject (pOldFont);
	dc.SetTextColor (clrOldText);
	dc.SetBkMode (nOldBkMode);
}
//****
void CBCGPCaptionBar::OnNcPaint() 
{
	CWindowDC	dcWin (this);

	CRect rectClient;
	GetClientRect(rectClient);

	CRect rectWindow;
	GetWindowRect(rectWindow);

	CRect rectBorder = rectWindow;

	ScreenToClient(rectWindow);

	rectClient.OffsetRect(-rectWindow.l, -rectWindow.t);
	dcWin.ExcludeClipRect (rectClient);
	
	rectBorder.OffsetRect(-rectBorder.l, -rectBorder.t);

	int nTop = rectBorder.t;
	rectBorder.t = rectBorder.b - m_nBorderSize;
	OnDrawBorder  (&dcWin, rectBorder);

	rectBorder.t = nTop;
	rectBorder.b = rectBorder.t + m_nBorderSize;

	OnDrawBorder  (&dcWin, rectBorder);
	dcWin.SelectClipRgn (NULL);
}
//****
void CBCGPCaptionBar::OnDrawBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	CBCGPVisualManager::GetInstance()->OnFillBarBackground (pDC, this,
		rect, rect);

	if (m_bIsMessageBarMode)
	{
		CRect rectInfo = rect;
		rectInfo.DeflateRect (nMessageBarMargin, nMessageBarMargin);
		rectInfo.r -= m_rectClose.Width();

		CBCGPVisualManager::GetInstance()->OnDrawCaptionBarInfoArea (pDC, this, rectInfo);
	}
}
//****
void CBCGPCaptionBar::OnDrawBorder (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	rect.InflateRect (2, 0);

	CBCGPVisualManager::GetInstance()->OnDrawCaptionBarBorder (
		pDC, this, rect, m_clrBarBorder, m_bFlatBorder);
}
//****
void CBCGPCaptionBar::OnDrawButton (CDC* pDC, CRect rect, 
								   const CString& strButton, BOOL bEnabled)
{
	ASSERT_VALID (pDC);

	COLORREF clrText = CBCGPVisualManager::GetInstance()->OnFillCaptionBarButton (pDC, this,
		rect, m_bIsBtnPressed || m_bIsBtnForcePressed, m_bIsBtnHighlighted, !m_bBtnEnabled,
		m_bBtnHasDropDownArrow, FALSE);

	CRect rectText = rect;
	rectText.DeflateRect (m_nHorzElementOffset, 0);

	if (m_bIsMessageBarMode)
	{
		rectText.DeflateRect (nButtonHorzMargin, 0);
	}

	if (m_uiBtnID != 0 && bEnabled && m_bBtnHasDropDownArrow)
	{
		rectText.r -= nMenuArrowWidth;
	}

	COLORREF clrTextOld = (COLORREF)-1;
	if (clrText != (COLORREF)-1)
	{
		clrTextOld = pDC->SetTextColor (clrText);
	}

	pDC->DrawText (strButton, rectText,
					DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}

	if (m_uiBtnID != 0 && bEnabled)
	{
		if (m_uiBtnID != 0 && m_bBtnHasDropDownArrow)
		{
			// Draw menu triangle:
			CRect rectArrow = rect;
			rectArrow.b -= m_nMargin;
			rectArrow.t = rectArrow.b - nMenuArrowWidth;
			rectArrow.l = rectText.r;

			int iXMiddle = rectArrow.l + rectArrow.Width() / 2;

			rectArrow.DeflateRect (0, rectArrow.Height() / 3);
			rectArrow.DeflateRect (rectArrow.Height() / 3, rectArrow.Height() / 3);
			rectArrow.l = iXMiddle - rectArrow.Height() - 1;
			rectArrow.r = iXMiddle + rectArrow.Height() + 1;

			int iHalfWidth =	(rectArrow.Width() % 2 != 0) ?
								(rectArrow.Width() - 1) / 2 :
								rectArrow.Width() / 2;

			CPoint pts [3];
			pts[0].x = rectArrow.l;
			pts[0].y = rectArrow.t;
			pts[1].x = rectArrow.r;
			pts[1].y = rectArrow.t;
			pts[2].x = rectArrow.l + iHalfWidth;
			pts[2].y = rectArrow.b + 1;

			CBrush brArrow (pDC->GetTextColor());

			CPen* pOldPen = (CPen*) pDC->SelectStockObject (NULL_PEN);
			CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brArrow);

			pDC->SetPolyFillMode (WINDING);
			pDC->Polygon (pts, 3);

			pDC->SelectObject (pOldBrush);
			pDC->SelectObject (pOldPen);
		}

		CBCGPVisualManager::GetInstance()->OnDrawCaptionBarButtonBorder (pDC, this,
			rect, m_bIsBtnPressed || m_bIsBtnForcePressed, m_bIsBtnHighlighted, !m_bBtnEnabled,
			m_bBtnHasDropDownArrow, FALSE);
	}
}	
//****
void CBCGPCaptionBar::OnDrawText (CDC* pDC, CRect rect, const CString& strText)
{
	ASSERT_VALID (pDC);

	if (m_arTextParts.GetSize() == 1)
	{
		pDC->DrawText (strText, rect, 
						DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
		return;
	}

	int x = rect.l;

	BOOL bIsBold = FALSE;

	for (int i = 0; i < m_arTextParts.GetSize(); i++)
	{
		if (!m_arTextParts [i].IsEmpty())
		{
			CFont* pOldFont = NULL;

			if (bIsBold)
			{
				pOldFont = pDC->SelectObject(globalData.fontBold);
			}

			CRect rectPart = rect;
			rectPart.l = x;

			CSize sizePart = pDC->GetTextExtent (m_arTextParts [i]);

			pDC->DrawText (m_arTextParts [i], rectPart, 
							DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

			if (pOldFont != NULL)
			{
				pDC->SelectObject (pOldFont);
			}

			x += sizePart.w;
		}

		bIsBold = !bIsBold;
	}
}	
//****
void CBCGPCaptionBar::OnDrawImage (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	if (m_hIcon != NULL)
	{
		DrawIconEx (pDC, rect.l, rect.t, m_hIcon, 
					rect.Width(), rect.Height(), NULL, (HBRUSH)NULL, 
					DI_NORMAL) ;
	}
	else if (m_Bitmap.GetCount() > 0)
	{
		CSize sizeDest;
		if (m_bStretchImage)
		{
			sizeDest = rect.Size();
		}
		else
		{
			sizeDest = m_rectImage.Size();
		}

		CBCGPDrawState ds;
		m_Bitmap.PrepareDrawImage (ds, sizeDest);
		m_Bitmap.Draw (pDC, rect.l, rect.t, 0);
		m_Bitmap.EndDrawImage (ds);
	}
}
//****
void CBCGPCaptionBar::OnSysColorChange() 
{
}
//****
CSize CBCGPCaptionBar::CalcFixedLayout(BOOL /*bStretch*/, BOOL /*bHorz*/)
{
	RecalcLayout();
	return CSize (32767, m_nCurrentHeight);
}
//****
void CBCGPCaptionBar::SetButton (LPCTSTR lpszLabel, UINT uiCmdUI, BarElementAlignment btnAlignmnet,
								 BOOL bHasDropDownArrow)
{
	ASSERT (lpszLabel != NULL);

	m_strBtnText = lpszLabel;
	m_uiBtnID = uiCmdUI;
	m_btnAlignnment = btnAlignmnet;
	m_bBtnHasDropDownArrow = bHasDropDownArrow;

	AdjustLayout();
}
//****
void CBCGPCaptionBar::EnableButton (BOOL bEnable)
{
	m_bBtnEnabled = bEnable;

	if (GetSafeHwnd() != NULL)
	{
		CRect rectButton = m_rectButton;

		RecalcLayout();

		InvalidateRect (rectButton);
		InvalidateRect (m_rectButton);

		UpdateWindow();
	}
}
//****
void CBCGPCaptionBar::SetButtonPressed (BOOL bPresed)
{
	m_bIsBtnForcePressed = bPresed;

	if (GetSafeHwnd() != NULL)
	{
		InvalidateRect (m_rectButton);
		UpdateWindow();
	}
}
//****
void CBCGPCaptionBar::SetButtonToolTip (LPCTSTR lpszToolTip, LPCTSTR lpszDescription)
{
	m_strButtonToolTip = lpszToolTip == NULL ? _T("") : lpszToolTip;
	m_strButtonDescription = lpszDescription == NULL ? _T("") : lpszDescription;

	UpdateTooltips();
}
//****
void CBCGPCaptionBar::RemoveButton()
{
	m_strBtnText.Empty();
	AdjustLayout();
}
//****
void CBCGPCaptionBar::SetIcon (CBitmap* hIcon, BarElementAlignment iconAlignment)
{
	m_Bitmap.Clear();

	m_hIcon = hIcon;
	m_iconAlignment = iconAlignment;

	AdjustLayout();
}
//****
void CBCGPCaptionBar::RemoveIcon()
{
	m_hIcon = NULL;
	AdjustLayout();
}
//****
void CBCGPCaptionBar::SetBitmap (CBitmap* hBitmap, COLORREF clrTransparent, 
								BOOL bStretch, BarElementAlignment bmpAlignment)
{
	ASSERT (hBitmap != NULL);

	m_hIcon = NULL;
	m_Bitmap.Clear();

	BITMAP bmp;
	::GetObject (hBitmap, sizeof (BITMAP), (LPVOID) &bmp);

	m_Bitmap.SetImageSize (CSize (bmp.bmWidth, bmp.bmHeight));
	m_Bitmap.SetTransparentColor (clrTransparent);
	m_Bitmap.AddImage (hBitmap, clrTransparent == (COLORREF)-1);
	
	m_bStretchImage = bStretch;

	m_iconAlignment = bmpAlignment;

	AdjustLayout();
}
//****
void CBCGPCaptionBar::SetBitmap (UINT uiBmpResID, COLORREF clrTransparent, 
								BOOL bStretch, BarElementAlignment bmpAlignment)
{
	m_hIcon = NULL;
	m_Bitmap.Clear();

	m_Bitmap.SetTransparentColor (clrTransparent);
	m_Bitmap.Load (uiBmpResID);
	m_Bitmap.SetSingleImage();
	
	m_bStretchImage = bStretch;

	m_iconAlignment = bmpAlignment;

	AdjustLayout();
}
//****
void CBCGPCaptionBar::RemoveBitmap()
{
	m_Bitmap.Clear();

	AdjustLayout();
}
//****
void CBCGPCaptionBar::SetImageToolTip (LPCTSTR lpszToolTip, LPCTSTR lpszDescription)
{
	m_strImageToolTip = lpszToolTip == NULL ? _T("") : lpszToolTip;
	m_strImageDescription = lpszDescription == NULL ? _T("") : lpszDescription;

	UpdateTooltips();
}
//****
void CBCGPCaptionBar::SetText (const CString& strText, BarElementAlignment textAlignment)
{
	BOOL bWasEmptyText = m_strText.IsEmpty();

	m_arTextParts.RemoveAll();

	int iStart = 0;

	for (int i = 0; i < strText.GetLength(); i++)
	{
		if (strText [i] == _T('\b'))
		{
			m_arTextParts.Add (strText.Mid (iStart, i - iStart));
			iStart = i + 1;
		}
	}

	m_arTextParts.Add (strText.Mid (iStart));

	m_strText = strText;
	m_textAlignment = textAlignment;

	if (m_nCurrentHeight == 0 || m_strText.IsEmpty() || bWasEmptyText)
	{
		AdjustLayout();
	}
	else
	{
		RecalcLayout();
		RedrawWindow();
	}
}
//****
void CBCGPCaptionBar::RemoveText()
{
	m_strText.Empty();
	AdjustLayout();
}
//****
afx_msg LRESULT CBCGPCaptionBar::OnSetFont (WPARAM wParam, LPARAM /*lParam*/)
{
	m_hFont = (HFONT) wParam;

	AdjustLayout();
	return 0;
}
//****
afx_msg LRESULT CBCGPCaptionBar::OnGetFont (WPARAM, LPARAM)
{
	return (LRESULT) m_hFont;
}
//****
CBCGPCaptionBar::BarElementAlignment CBCGPCaptionBar::GetAlignment (BarElement elem)
{
	switch (elem)
	{
	case ELEM_BUTTON:
		return m_btnAlignnment;

	case ELEM_TEXT:
		return m_textAlignment;

	case ELEM_ICON:
		return m_iconAlignment;
	}

	ASSERT (FALSE);
	return ALIGN_INVALID;
}
//****
void CBCGPCaptionBar::RecalcLayout()
{
	CClientDC dc (NULL);

	CFont* pOldFont = dc.SelectObject (
		m_hFont == NULL ? &globalData.fontRegular : CFont::FromHandle (m_hFont));
	ASSERT (pOldFont != NULL);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);

	int nTextHeight = tm.tmHeight + 2;
	CSize sizeImage = GetImageSize();

	//-------------------------------------------------------------------
	// the height is set to the default (provided by the user in Create)
	// or calculated if it is -1
	//-------------------------------------------------------------------
	if (m_nDefaultHeight != -1)
	{
		m_nCurrentHeight = m_nDefaultHeight;
	}
	else
	{
		if (!m_strBtnText.IsEmpty() && m_bIsMessageBarMode)
		{
			nTextHeight += 2 * nButtonVertMargin;
		}

		m_nCurrentHeight = max (nTextHeight, sizeImage.h) + 
			m_nMargin * 2 + m_nBorderSize;
	}

	if (m_bIsMessageBarMode)
	{
		m_nCurrentHeight += nMessageBarMargin * 2;
	}

	// for left and center alignment: icon, button, text
	// for right alignment: text, button, icon

	CRect rectClient;
	GetClientRect (rectClient);
	if (rectClient.IsRectEmpty())
	{
		return;
	}

	if (m_bIsMessageBarMode)
	{
		CSize sizeImage = CBCGPMenuImages::Size();
		sizeImage.w += 2 * nMessageBarMargin;
		sizeImage.h += 2 * nMessageBarMargin;

		m_rectClose = CRect (
			CPoint (rectClient.r - sizeImage.w, rectClient.t + nMessageBarMargin),
			sizeImage);

		rectClient.DeflateRect (nMessageBarMargin, nMessageBarMargin);
		rectClient.r -= m_rectClose.Width();
	}

	BOOL bButtonLeftOfIcon = FALSE;
	BOOL bTextLeftOfButton = FALSE;
	BOOL bTextLeftOfIcon = FALSE;

	BOOL bIconCenter = FALSE;
	BOOL bButtonCenter = FALSE;
	BOOL bButtonAfterText = FALSE;
	BOOL bTextCenter = FALSE;

	// differs from the current height, because the border size is non-client area
	int nBaseLine = rectClient.CenterPoint().y;
	int nCenterOffset = rectClient.CenterPoint().x;

	int nNextXOffsetLeft  = rectClient.l + m_nMargin;
	int nNextXOffsetRight = rectClient.r - m_nMargin;
	int nNextXOffsetCenter = nCenterOffset;

	if (IsImageSet())
	{
		if (sizeImage.h < rectClient.Height())
		{
			// center the icon if its height lesser than client area height
			m_rectImage.t = nBaseLine - sizeImage.h / 2;
		}
		else
		{
			// otherwise, clip it from the buttom
			m_rectImage.t = rectClient.t + m_nMargin;
		}

		if (!m_bStretchImage)
		{
			m_rectImage.b = m_rectImage.t + sizeImage.h;
		}
		else
		{
			m_rectImage.b = rectClient.b - m_nMargin;
		}

		switch (m_iconAlignment)
		{
		case ALIGN_LEFT:
			m_rectImage.l = nNextXOffsetLeft;
			m_rectImage.r = m_rectImage.l + sizeImage.w;
			nNextXOffsetLeft = m_rectImage.r + m_nHorzElementOffset;
			break;

		case ALIGN_RIGHT:
			m_rectImage.l = nNextXOffsetRight - sizeImage.w;
			m_rectImage.r = m_rectImage.l + sizeImage.w;
			nNextXOffsetRight = m_rectImage.l - m_nHorzElementOffset;
			// only in this case button and text is at the left side of the icon
			bButtonLeftOfIcon = TRUE; 
			bTextLeftOfIcon = TRUE;
			break;

		case ALIGN_CENTER:
			bIconCenter = TRUE;
			nNextXOffsetCenter -= sizeImage.w / 2;

			if (m_btnAlignnment == ALIGN_LEFT)
			{
				bButtonLeftOfIcon = TRUE;
			}

			if (m_textAlignment == ALIGN_LEFT)
			{
				bTextLeftOfIcon = TRUE;
			}
			break;

		default:
			ASSERT (FALSE);
		}
	}

	int nButtonWidth = 0;

	if (!m_strBtnText.IsEmpty())
	{
		nButtonWidth = dc.GetTextExtent (m_strBtnText).w + 2 * m_nHorzElementOffset;

		if (m_bIsMessageBarMode)
		{
			nButtonWidth += 2 * nButtonHorzMargin;
		}

		if (m_uiBtnID != 0 && m_bBtnEnabled && m_bBtnHasDropDownArrow)
		{
			nButtonWidth += nMenuArrowWidth;
		}

		// the button always has a height equivalent to the bar's height
		m_rectButton.t = rectClient.t;
		m_rectButton.b = rectClient.b;

		if (m_bIsMessageBarMode)
		{
			m_rectButton.DeflateRect (0, nButtonVertMargin);
		}

		switch (m_btnAlignnment)
		{
		case ALIGN_LEFT:
			if (!m_bIsMessageBarMode || m_textAlignment != ALIGN_LEFT)
			{
				m_rectButton.l = nNextXOffsetLeft;

				if (nNextXOffsetLeft == rectClient.l + m_nMargin)
				{
					m_rectButton.l = rectClient.l + m_nMargin;
				}

				m_rectButton.r = m_rectButton.l + nButtonWidth;
				nNextXOffsetLeft = m_rectButton.r + m_nHorzElementOffset;
			}
			else
			{
				bButtonAfterText = TRUE;
			}
			break;

		case ALIGN_RIGHT:
			m_rectButton.l = nNextXOffsetRight - nButtonWidth;

			if (nNextXOffsetRight == rectClient.r - m_nMargin)
			{
				m_rectButton.l = rectClient.r - nButtonWidth - m_nMargin;
			}

			m_rectButton.r = m_rectButton.l + nButtonWidth;
			nNextXOffsetRight = m_rectButton.l - m_nHorzElementOffset;
			// only in this case text at the left side of the button
			bTextLeftOfButton = TRUE;
			break;

		case ALIGN_CENTER:
			bButtonCenter = TRUE;
			nNextXOffsetCenter -= nButtonWidth / 2;

			if (m_textAlignment == ALIGN_LEFT)
			{
				bTextLeftOfButton = TRUE;
			}
			break;

		default:
			ASSERT (FALSE);
			return;
		}
	}

	CSize sizeText (0, 0);

	if (!m_strText.IsEmpty())
	{
		sizeText = GetTextSize (&dc, m_strText);

		m_rectText.t = nBaseLine - sizeText.h / 2;
		m_rectText.b = m_rectText.t + sizeText.h;

		switch (m_textAlignment)
		{
		case ALIGN_LEFT:
			m_rectText.l = nNextXOffsetLeft;
			nNextXOffsetLeft += sizeText.w + 2 * m_nMargin;
			break;

		case ALIGN_RIGHT:
			m_rectText.l = nNextXOffsetRight - sizeText.w;
			break;

		case ALIGN_CENTER:
			bTextCenter = TRUE;
			nNextXOffsetCenter -= sizeText.w / 2;
			break;

		default:
			ASSERT (FALSE);
			return;
		}

		m_rectText.r = m_rectText.l + sizeText.w;
		AdjustRectToMargin (m_rectText, rectClient, m_nMargin);
		m_rectDrawText = m_rectText;
	}

	if (bIconCenter)
	{
		m_rectImage.l = nNextXOffsetCenter;
		m_rectImage.r = m_rectImage.l + sizeImage.w;
		nNextXOffsetCenter = m_rectImage.r + m_nHorzElementOffset;
	}

	if (bButtonAfterText)
	{
		m_rectButton.l = nNextXOffsetLeft;
		m_rectButton.r = m_rectButton.l + nButtonWidth;

		if (m_rectButton.r + m_nMargin > rectClient.r)
		{
			m_rectButton.r = rectClient.r - m_nMargin;
			m_rectButton.l = m_rectButton.r - nButtonWidth;
		}
	}
	else if (bButtonCenter)
	{
		m_rectButton.l = nNextXOffsetCenter;
		m_rectButton.r = m_rectButton.l + nButtonWidth;
		nNextXOffsetCenter = m_rectButton.r + m_nHorzElementOffset;
	}

	if (bTextCenter)
	{
		m_rectText.l = nNextXOffsetCenter;
		m_rectText.r = m_rectText.l + sizeText.w; 
		AdjustRectToMargin (m_rectText, rectClient, m_nMargin);
		m_rectDrawText = m_rectText;
	}

	if (IsImageSet())
	{
		// do not retain image size if it should be stretched
		AdjustRectToMargin (m_rectImage, rectClient, m_nMargin, !m_bStretchImage);

		if (m_rectImage.l < rectClient.l || 
			m_rectImage.r > rectClient.r)
		{
			m_rectImage.SetRectEmpty();
		}
	}

	CRect rectButtonTemp = m_rectButton;
	if (!m_strBtnText.IsEmpty() && IsImageSet())
	{
		CheckRectangle (rectButtonTemp, m_rectImage, bButtonLeftOfIcon);
	}

	if (!m_strBtnText.IsEmpty())
	{
		AdjustRectToMargin (rectButtonTemp, rectClient, m_nMargin);

		if (m_rectButton.Width() + m_rectImage.Width() + 2 * m_nMargin > rectClient.Width())
		{
			m_rectButton.SetRectEmpty();
		}
	}

	if (!m_strText.IsEmpty())
	{
		CheckRectangle (m_rectDrawText, m_rectImage, bTextLeftOfIcon); 
		CheckRectangle (m_rectDrawText, rectButtonTemp, bTextLeftOfButton || bButtonAfterText);
	}

	dc.SelectObject (pOldFont);

	m_bTextIsTruncated = m_rectDrawText.Width() < sizeText.w;

	UpdateTooltips();
}
//****
BOOL CBCGPCaptionBar::CheckRectangle (CRect& rectSrc, const CRect& rectOther, 
									 BOOL bLeftOf)
{
	if (rectSrc.IsRectEmpty() || rectOther.IsRectEmpty())
	{
		return FALSE;
	}

	CRect rectOtherWithOffset = rectOther;
	rectOtherWithOffset.InflateRect (m_nHorzElementOffset, m_nHorzElementOffset);

	if (rectSrc.l <= rectOtherWithOffset.r && 
		rectSrc.l >= rectOtherWithOffset.l)
	{
		rectSrc.l = rectOtherWithOffset.r;
	}

	if (rectSrc.r >= rectOtherWithOffset.l &&
		rectSrc.r <= rectOtherWithOffset.r)
	{
		rectSrc.r = rectOtherWithOffset.l;
	}

	if (rectSrc.l >= rectOtherWithOffset.l && 
		rectSrc.r <= rectOtherWithOffset.r)
	{
		rectSrc.r = rectSrc.l;
	}

	if (rectSrc.l <= rectOtherWithOffset.l && 
		rectSrc.r >= rectOtherWithOffset.r)
	{
		if (bLeftOf)
		{
			rectSrc.r = rectOtherWithOffset.l;
		}
		else
		{
			rectSrc.l = rectOtherWithOffset.r;
		}
	}

	if (bLeftOf && rectSrc.l >= rectOtherWithOffset.r ||
		!bLeftOf && rectSrc.r <= rectOtherWithOffset.l)
	{
		rectSrc.l = rectSrc.r;
	}

	return FALSE;
}
//****
void CBCGPCaptionBar::AdjustRectToMargin (CRect& rectSrc, const CRect& rectClient, 
										 int nMargin, BOOL bRetainSize)
{
	BOOL bLeftChanged = FALSE;
	BOOL bRightChanged = FALSE;
	int nWidth = rectSrc.Width();
	if (rectSrc.l < rectClient.l + nMargin)
	{
		rectSrc.l = rectClient.l + nMargin;
		bLeftChanged = TRUE;
	}

	if (rectSrc.r > rectClient.r - nMargin)
	{
		rectSrc.r = rectClient.r - nMargin;
		bRightChanged = TRUE;
	}

	if (bRetainSize)
	{
		if (bLeftChanged)   
		{
			rectSrc.r = rectSrc.l + nWidth;
		}
		else if (bRightChanged)
		{
			rectSrc.l = 	rectSrc.r - nWidth;
		}
	}
}
//****
CSize CBCGPCaptionBar::GetImageSize() const
{
	if (m_Bitmap.GetCount() > 0)
	{
		ASSERT (m_hIcon == NULL);
		return m_Bitmap.GetImageSize();
	}

	if (m_hIcon == NULL)
	{
		return CSize (0, 0);
	}

	ICONINFO info;
	memset (&info, 0, sizeof (ICONINFO));

	::GetIconInfo (m_hIcon, &info);
	CBitmap* hBmp = info.hbmColor;

	BITMAP bmp;
	::GetObject (hBmp, sizeof (BITMAP), (LPVOID) &bmp);

	::DeleteObject (info.hbmColor);
	::DeleteObject (info.hbmMask);

	return CSize (bmp.bmWidth, bmp.bmHeight);
}
//****
BOOL CBCGPCaptionBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}
//****
void CBCGPCaptionBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBCGPControlBar::OnLButtonDown(nFlags, point);

	if (m_uiBtnID != 0 && m_bBtnEnabled && m_bIsBtnHighlighted)
	{
		m_bIsBtnPressed = TRUE;
		InvalidateRect (m_rectButton);
		UpdateWindow();

		if (m_bBtnHasDropDownArrow)
		{
			ASSERT_VALID (GetOwner());
			SendMessage(WM_COMMAND, m_uiBtnID);
		}
	}

	if (m_bIsCloseBtnHighlighted)
	{
		m_bIsCloseBtnPressed= TRUE;
		InvalidateRect (m_rectClose);
		UpdateWindow();
	}
}
//****
void CBCGPCaptionBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CBCGPControlBar::OnLButtonUp(nFlags, point);

	if (m_bIsBtnPressed)
	{
		m_bIsBtnPressed = FALSE;
		InvalidateRect (m_rectButton);
		UpdateWindow();

		if (!m_bBtnHasDropDownArrow && m_uiBtnID != 0)
		{
			ASSERT_VALID (GetOwner());
			SendMessage(WM_COMMAND, m_uiBtnID);
		}
	}
	else if (m_bIsCloseBtnPressed)
	{
		m_bIsCloseBtnPressed = FALSE;

		InvalidateRect (m_rectClose);
		UpdateWindow();

		ShowControlBar (FALSE, FALSE, FALSE);
	}
}
//****
void CBCGPCaptionBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	CBCGPControlBar::OnMouseMove(nFlags, point);

	BOOL bTrack = FALSE;

	if (m_uiBtnID != 0 && m_bBtnEnabled)
	{
		BOOL bIsBtnHighlighted = m_rectButton.PtInRect (point);

		if (m_bIsBtnHighlighted != bIsBtnHighlighted)
		{
			m_bIsBtnHighlighted = bIsBtnHighlighted;
			m_bIsBtnPressed = (nFlags & MK_LBUTTON) && m_bIsBtnHighlighted;

			InvalidateRect (m_rectButton);
			UpdateWindow();

			bTrack = bIsBtnHighlighted;
		}
	}

	if (!m_rectClose.IsRectEmpty())
	{
		BOOL bIsBtnHighlighted = m_rectClose.PtInRect (point);

		if (m_bIsCloseBtnHighlighted != bIsBtnHighlighted)
		{
			m_bIsCloseBtnHighlighted = bIsBtnHighlighted;
			m_bIsCloseBtnPressed = (nFlags & MK_LBUTTON) && m_bIsCloseBtnHighlighted;

			InvalidateRect (m_rectClose);
			UpdateWindow();

			bTrack = bIsBtnHighlighted;
		}
	}

	if (!m_bTracked)
	{
		m_bTracked = TRUE;
		
		TRACKMOUSEEVENT trackmouseevent;
		trackmouseevent.cbSize = sizeof(trackmouseevent);
		trackmouseevent.dwFlags = TME_LEAVE;
		trackmouseevent.hwndTrack = GetSafeHwnd();
		trackmouseevent.dwHoverTime = HOVER_DEFAULT;
		::BCGPTrackMouse (&trackmouseevent);	
	}
}
//****
afx_msg LRESULT CBCGPCaptionBar::OnMouseLeave(WPARAM,LPARAM)
{
	m_bTracked = FALSE;

	if (m_bIsBtnPressed || m_bIsBtnHighlighted)
	{
		m_bIsBtnPressed = FALSE;
		m_bIsBtnHighlighted = FALSE;

		InvalidateRect (m_rectButton);
		UpdateWindow();
	}

	if (m_bIsCloseBtnPressed || m_bIsCloseBtnHighlighted)
	{
		m_bIsCloseBtnPressed = FALSE;
		m_bIsCloseBtnHighlighted = FALSE;

		InvalidateRect (m_rectClose);
		UpdateWindow();
	}

	return 0;
}
//****
void CBCGPCaptionBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CFrameWnd* pParent = BCGPGetParentFrame (this);
	if (pParent != NULL && pParent->GetSafeHwnd() != NULL)
	{
		pParent->RecalcLayout();
	}

	RecalcLayout();
}
//****
void CBCGPCaptionBar::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (!CBCGPToolBar::IsCustomizeMode())
	{
		ASSERT_VALID (GetOwner());

		ClientToScreen (&point);
		OnControlBarContextMenu (GetOwner(), point);
		return;
	}
	
	CBCGPControlBar::OnRButtonUp(nFlags, point);
}
//****
void CBCGPCaptionBar::OnDestroy() 
{
	CBCGPTooltipManager::DeleteToolTip (m_pToolTip);
	CBCGPControlBar::OnDestroy();
}
//****
BOOL CBCGPCaptionBar::OnNeedTipText(UINT /*id*/, NMHDR* pNMH, LRESULT* /*pResult*/)
{
	static CString strTipText;

	if (m_pToolTip->GetSafeHwnd() == NULL || 
		pNMH->hwndFrom != m_pToolTip->GetSafeHwnd())
	{
		return FALSE;
	}

	if (CBCGPPopupMenu::GetActiveMenu() != NULL)
	{
		return FALSE;
	}

	LPNMTTDISPINFO	pTTDispInfo	= (LPNMTTDISPINFO) pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	CString strDescr;

	switch (pNMH->idFrom)
	{
	case nIdToolTipClose:
		{
			CBCGPLocalResource locaRes;
			strTipText.LoadString (IDS_BCGBARRES_CLOSEBAR);
		}
		break;

	case nIdToolTipText:
		strTipText = m_strText;
		strTipText.Remove (_T('\b'));
		break;

	case nIdToolTipImage:
		strTipText = m_strImageToolTip;
		strDescr = m_strImageDescription;
		break;

	case nIdToolTipButton:
		strTipText = m_strButtonToolTip;
		strDescr = m_strButtonDescription;
		break;
	}

	if (strTipText.IsEmpty())
	{
		return TRUE;
	}

	CBCGPToolTipCtrl* pToolTip = DYNAMIC_DOWNCAST (
		CBCGPToolTipCtrl, m_pToolTip);

	if (pToolTip != NULL && !strDescr.IsEmpty())
	{
		ASSERT_VALID (pToolTip);
		pToolTip->SetDescription (strDescr);
	}

	pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR) strTipText);
	return TRUE;
}
//****
BOOL CBCGPCaptionBar::PreTranslateMessage(MSG* pMsg) 
{
   	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
		if (m_pToolTip->GetSafeHwnd() != NULL)
		{
			m_pToolTip->RelayEvent(pMsg);
		}
		break;
	}

	return CBCGPControlBar::PreTranslateMessage(pMsg);
}
//****
LRESULT CBCGPCaptionBar::OnBCGUpdateToolTips (WPARAM wp, LPARAM)
{
	UINT nTypes = (UINT) wp;

	if (nTypes & BCGP_TOOLTIP_TYPE_CAPTIONBAR)
	{
		CBCGPTooltipManager::CreateToolTip (m_pToolTip, this,
			BCGP_TOOLTIP_TYPE_CAPTIONBAR);

		CRect rectDummy (0, 0, 0, 0);

		m_pToolTip->SetMaxTipWidth (640);

		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, nIdToolTipClose);
		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, nIdToolTipText);
		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, nIdToolTipImage);
		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, nIdToolTipButton);
	}

	return 0;
}
//****
void CBCGPCaptionBar::UpdateTooltips()
{
	if (m_pToolTip->GetSafeHwnd() != NULL)
	{
		m_pToolTip->SetToolRect (this, nIdToolTipClose, m_rectClose);

		if (m_bTextIsTruncated)
		{
			m_pToolTip->SetToolRect (this, nIdToolTipText, m_rectDrawText);
		}
		else
		{
			m_pToolTip->SetToolRect (this, nIdToolTipText, CRect (0, 0, 0, 0));
		}

		if (!m_strImageToolTip.IsEmpty())
		{
			m_pToolTip->SetToolRect (this, nIdToolTipImage, m_rectImage);
		}
		else
		{
			m_pToolTip->SetToolRect (this, nIdToolTipImage, CRect (0, 0, 0, 0));
		}

		if (!m_strButtonToolTip.IsEmpty())
		{
			m_pToolTip->SetToolRect (this, nIdToolTipButton, m_rectButton);
		}
		else
		{
			m_pToolTip->SetToolRect (this, nIdToolTipButton, CRect (0, 0, 0, 0));
		}
	}
}
//****
CSize CBCGPCaptionBar::GetTextSize (CDC* pDC, const CString& strText)
{
	ASSERT_VALID (pDC);

	if (m_arTextParts.GetSize() == 1)
	{
		return pDC->GetTextExtent (strText);
	}

	CSize sizeText (0, 0);

	BOOL bIsBold = FALSE;

	for (int i = 0; i < m_arTextParts.GetSize(); i++)
	{
		if (!m_arTextParts [i].IsEmpty())
		{
			CFont* pOldFont = NULL;

			if (bIsBold)
			{
				pOldFont = pDC->SelectObject(globalData.fontBold);
			}

			CSize sizePart = pDC->GetTextExtent (m_arTextParts [i]);

			sizeText.w += sizePart.w;
			sizeText.h = max (sizeText.h, sizePart.h);

			if (pOldFont != NULL)
			{
				pDC->SelectObject (pOldFont);
			}
		}

		bIsBold = !bIsBold;
	}

	return sizeText;
}
