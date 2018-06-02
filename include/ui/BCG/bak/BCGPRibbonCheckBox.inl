
//
// BCGPRibbonCheckBox.cpp: implementation of the CBCGPRibbonCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#include "bcgglobals.h"
#include "BCGPRibbonCheckBox.h"
#include "BCGPVisualManager.h"

#ifndef BCGP_EXCLUDE_RIBBON

IMPLEMENT_DYNCREATE(CBCGPRibbonCheckBox, CBCGPRibbonButton)

#define CHECK_BOX_DEFAULT_SIZE	(globalData.GetRibbonImageScale() == 1. ? 16 : 20)

const int nTextMarginLeft = 4;
const int nTextMarginRight = 6;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonCheckBox::CBCGPRibbonCheckBox()
{
}
//****
CBCGPRibbonCheckBox::CBCGPRibbonCheckBox(UINT nID, LPCTSTR lpszText) :
	CBCGPRibbonButton (nID, lpszText)
{
}
//****
CBCGPRibbonCheckBox::~CBCGPRibbonCheckBox()
{
}

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

CSize CBCGPRibbonCheckBox::GetIntermediateSize (CDC* /*pDC*/)
{
	ASSERT_VALID (this);

	m_szMargin = CSize (2, 3);

	const CSize sizeCheckBox = CSize (CHECK_BOX_DEFAULT_SIZE, CHECK_BOX_DEFAULT_SIZE);

	int cx = sizeCheckBox.w + m_sizeTextRight.w + nTextMarginLeft + nTextMarginRight + m_szMargin.w;
	int cy = max (sizeCheckBox.h, m_sizeTextRight.h) + 2 * m_szMargin.h;

	return CSize (cx, cy);
}
//****
void CBCGPRibbonCheckBox::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	const CSize sizeCheckBox = CSize (CHECK_BOX_DEFAULT_SIZE, CHECK_BOX_DEFAULT_SIZE);

	//----------------
	// Draw check box:
	//----------------
	CRect rectCheck = m_rect;
	rectCheck.DeflateRect (m_szMargin);
	rectCheck.l++;
	rectCheck.r = rectCheck.l + sizeCheckBox.w;
	rectCheck.t = rectCheck.CenterPoint().y - sizeCheckBox.w / 2;

	rectCheck.b = rectCheck.t + sizeCheckBox.h;

	OnDrawMark (pDC, rectCheck);

	//-----------
	// Draw text:
	//-----------
	COLORREF clrTextOld = (COLORREF)-1;

	if (m_bIsDisabled)
	{
		if (m_bQuickAccessMode)
		{
			clrTextOld = pDC->SetTextColor (
					CBCGPVisualManager::GetInstance()->GetRibbonQATTextColor (TRUE));
		}
		else
		{
			clrTextOld = pDC->SetTextColor (CBCGPVisualManager::GetInstance()->GetToolbarDisabledTextColor());
		}
	}

	CRect rectText = m_rect;
	rectText.l = rectCheck.r + nTextMarginLeft;

	DoDrawText (pDC, m_strText, rectText, DT_SINGLELINE | DT_VCENTER);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}

	if (IsFocused())
	{
		CRect rectFocus = rectText;
		rectFocus.OffsetRect (-nTextMarginLeft / 2, 0);
		rectFocus.DeflateRect (0, 2);

		pDC->DrawFocusRect (rectFocus);
	}
}
//****
void CBCGPRibbonCheckBox::OnDrawOnList (CDC* pDC, CString strText,
									  int nTextOffset, CRect rect,
									  BOOL bIsSelected, BOOL bHighlighted)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	BOOL bIsDisabled = m_bIsDisabled;
	m_bIsDisabled = FALSE;

	CRect rectCheck = rect;
	rectCheck.r = rect.l + nTextOffset;

	if (rectCheck.Width() > rectCheck.Height())
	{
		rectCheck.l = rectCheck.CenterPoint().x - rectCheck.Height() / 2;
		rectCheck.r = rectCheck.l + rectCheck.Height();
	}
	else
	{
		rectCheck.t = rectCheck.CenterPoint().y - rectCheck.Width() / 2;
		rectCheck.b = rectCheck.t + rectCheck.Width();
	}

	OnDrawMarkOnList (pDC, rectCheck, bIsSelected, bHighlighted);

	rect.l += nTextOffset;
	
	const int xMargin = 3;
	rect.DeflateRect (xMargin, 0);

	pDC->DrawText (strText, rect, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	m_bIsDisabled = bIsDisabled;
}
//****
void CBCGPRibbonCheckBox::OnDrawMark (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	const BOOL bIsHighlighted = (IsHighlighted() || IsFocused()) && !IsDisabled();

	CBCGPVisualManager::GetInstance()->OnDrawCheckBoxEx (pDC,
		rect, IsChecked() || (IsPressed() && bIsHighlighted) ? 1 : 0,
		bIsHighlighted, IsPressed() && bIsHighlighted, !IsDisabled());
}
//****
void CBCGPRibbonCheckBox::OnDrawMarkOnList (CDC* pDC, CRect rect, BOOL bIsSelected, BOOL bHighlighted)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPVisualManager::GetInstance()->OnDrawRibbonCheckBoxOnList (
		pDC, this, rect, bIsSelected, bHighlighted);
}

//////////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonRadioButton

IMPLEMENT_DYNCREATE(CBCGPRibbonRadioButton, CBCGPRibbonCheckBox)

CBCGPRibbonRadioButton::CBCGPRibbonRadioButton()
{
}
//****
CBCGPRibbonRadioButton::CBCGPRibbonRadioButton(UINT nID, LPCTSTR lpszText) :
	CBCGPRibbonCheckBox (nID, lpszText)
{
}
//****
CBCGPRibbonRadioButton::~CBCGPRibbonRadioButton()
{
}
//****
void CBCGPRibbonRadioButton::OnDrawMark (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	const BOOL bIsHighlighted = (IsHighlighted() || IsFocused()) && !IsDisabled();

	CBCGPVisualManager::GetInstance()->OnDrawRadioButton (pDC,
		rect, IsChecked() || (IsPressed() && bIsHighlighted) ? 1 : 0,
		bIsHighlighted, IsPressed() && bIsHighlighted, !IsDisabled());
}
//****
void CBCGPRibbonRadioButton::OnDrawMarkOnList (CDC* pDC, CRect rect, BOOL bIsSelected, BOOL bHighlighted)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPVisualManager::GetInstance()->OnDrawRibbonRadioButtonOnList (
		pDC, this, rect, bIsSelected, bHighlighted);
}

#endif // BCGP_EXCLUDE_RIBBON
