
//
// BCGPRibbonLabel.cpp: implementation of the CBCGPRibbonLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPVisualManager.h"
#include "bcgglobals.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonCategory.h"

#ifndef BCGP_EXCLUDE_RIBBON

IMPLEMENT_DYNCREATE(CBCGPRibbonLabel, CBCGPRibbonButton)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonLabel::CBCGPRibbonLabel()
{
}
//****
CBCGPRibbonLabel::CBCGPRibbonLabel(LPCTSTR lpszText, BOOL bIsMultiLine)
{
	ASSERT (lpszText != NULL);
	m_strText = lpszText;

	m_bIsAlwaysLarge = bIsMultiLine;
}
//****
CBCGPRibbonLabel::~CBCGPRibbonLabel()
{
}
//****
void CBCGPRibbonLabel::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	CRect rectText = m_rect;
	rectText.DeflateRect (m_szMargin.w, 0);

	COLORREF cltTextOld = (COLORREF)-1;

	if (IsMenuMode())
	{
		rectText.b -= 2;

		COLORREF clrText = CBCGPVisualManager::GetInstance()->OnDrawMenuLabel (pDC, m_rect);

		if (clrText != (COLORREF)-1)
		{
			cltTextOld = pDC->SetTextColor (clrText);
		}
	}
	else
	{
		CBCGPVisualManager::GetInstance()->OnDrawRibbonLabel (pDC, 
			this, m_rect);
	}

	CFont* pOldFont = NULL;
	
	if (IsMenuMode())
	{
		pOldFont = pDC->SelectObject(globalData.fontBold);
		ASSERT_VALID (pOldFont);
	}

	UINT uiDTFlags = IsMenuMode() || !m_bIsAlwaysLarge ? 
		DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX :
		DT_WORDBREAK | DT_NOPREFIX;

	if (!IsMenuMode() && m_bIsAlwaysLarge)
	{
		int dy = max (0, (rectText.Height() - m_sizeTextRight.h) / 2);
		rectText.DeflateRect (0, dy);
	}

	DoDrawText (pDC, m_strText, rectText, uiDTFlags);

	if (pOldFont != NULL)
	{
		pDC->SelectObject (pOldFont);
	}

	if (cltTextOld != (COLORREF)-1)
	{
		cltTextOld = pDC->SetTextColor (cltTextOld);
	}
}
//****
void CBCGPRibbonLabel::OnCalcTextSize (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (IsMenuMode() || !m_bIsAlwaysLarge)
	{
		CFont* pOldFont = NULL;
		
		if (IsMenuMode())
		{
			pOldFont = pDC->SelectObject(globalData.fontBold);
			ASSERT_VALID (pOldFont);
		}

		CBCGPRibbonButton::OnCalcTextSize (pDC);

		if (pOldFont != NULL)
		{
			pDC->SelectObject (pOldFont);
		}

		return;
	}

	//------------------
	// Multi-line label:
	//------------------

	ASSERT_VALID (m_pParent);

	const CSize sizeImageLarge = m_pParent->GetImageSize (TRUE);
	if (sizeImageLarge == CSize (0, 0))
	{
		ASSERT (FALSE);
		return;
	}

	const int nMaxHeight = 2 * sizeImageLarge.h;

	int nTextHeight = 0;
	int nTextWidth = 0;

	CString strText = m_strText;

	for (int dx = 10; dx < 200; dx += 10)
	{
		CRect rectText (0, 0, dx, 10000);

		nTextHeight = pDC->DrawText (strText, rectText, 
									DT_WORDBREAK | DT_CALCRECT);

		nTextWidth = rectText.Width();
		
		if (nTextHeight <= nMaxHeight && nTextWidth >= nTextHeight)
		{
			break;
		}
	}

	m_sizeTextRight.w = nTextWidth;
	m_sizeTextRight.h = nTextHeight;
}
//****
CSize CBCGPRibbonLabel::GetIntermediateSize (CDC* pDC)
{
	ASSERT_VALID (this);

	if (IsMenuMode())
	{
		m_szMargin = CSize (3, 3);
	}
	else
	{
		m_szMargin = CSize (2, 4);
	}
	
	OnCalcTextSize (pDC);
	return CSize (m_sizeTextRight.w + 2 * m_szMargin.w, m_sizeTextRight.h + 2 * m_szMargin.h);
}
//****
BOOL CBCGPRibbonLabel::SetACCData (CWnd* pParent, CBCGPAccessibilityData& data)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::SetACCData (pParent, data);

	data.m_nAccRole = ROLE_SYSTEM_GROUPING;
	data.m_bAccState = 0;
	return TRUE;
}

#endif // BCGP_EXCLUDE_RIBBON
