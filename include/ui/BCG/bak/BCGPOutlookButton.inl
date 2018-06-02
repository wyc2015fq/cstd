
// BCGOutlookButton.cpp: implementation of the CBCGPOutlookButton class.
//
//////////////////////////////////////////////////////////////////////


//#include "BCGCBPro.h"
#include "BCGPOutlookButton.h"
#include "BCGPOutlookBarPane.h"
#include "MenuImages.h"
#include "BCGPWorkspace.h"
#include "BCGPVisualManager.h"
#include "BCGPDrawManager.h"

IMPLEMENT_SERIAL(CBCGPOutlookButton, CBCGPToolbarButton, 1)

#define BUTTON_OFFSET			10
#define HIGHLIGHT_PERCENTAGE	85

extern CBCGPWorkspace* g_pWorkspace;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPOutlookButton::CBCGPOutlookButton()
{
	m_pWndParentBar = NULL;
	m_sizeImage = CSize (0, 0);
	m_bIsWholeText = TRUE;
}
//****
CBCGPOutlookButton::~CBCGPOutlookButton()
{
}
//****
void CBCGPOutlookButton::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
						BOOL bHorz, BOOL bCustomizeMode,
						BOOL bHighlight,
						BOOL /*bDrawBorder*/,
						BOOL /*bGrayDisabledButtons*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndParentBar);

	CSize csOffset (0, 0);
	if (!bCustomizeMode &&
		(bHighlight && (m_nStyle & TBBS_PRESSED)))
	{
		csOffset = CSize (1, 1);
	}

	CRect rectInternal = rect;
	CRect rectText = rect;

	if (m_bExtraSize)
	{
		CSize sizeExtra = CBCGPVisualManager::GetInstance()->GetButtonExtraBorder();
		if (sizeExtra != CSize (0, 0))
		{
			rectInternal.DeflateRect (sizeExtra.w / 2 + 1, sizeExtra.h / 2 + 1);

			if (!bHorz)
			{
				rectText.OffsetRect (0, sizeExtra.h);
			}
			else
			{
				rectText.OffsetRect (sizeExtra.w, 0);
			}
		}
	}

	CRect rectBorder = rectInternal;
	rectText.t += BUTTON_OFFSET / 2;

	if (pImages != NULL && GetImage() >= 0)
	{
		int x, y;

		CSize csImage = pImages->GetImageSize();

		if (!bHorz)
		{
			int iImageHorzOffset = (rectInternal.Width() - csImage.w) / 2;
			x = rectInternal.l + iImageHorzOffset;
			y = rectInternal.t + BUTTON_OFFSET / 2;

			rectText.t += csImage.h + 2;
		}
		else
		{
			int iImageVertOffset = (rectInternal.Height() - csImage.h) / 2;
			x = rectInternal.l + BUTTON_OFFSET / 2;
			y = rectInternal.t + iImageVertOffset;

			rectText.l += csImage.w + BUTTON_OFFSET;

			CRect rectInternal = rectText;
			int iTextHeight = pDC->DrawText (m_strText, rectInternal, 
				DT_CALCRECT | DT_WORDBREAK);

			rectText.t = rectInternal.t + (rectInternal.Height() - iTextHeight) / 2;
		}

		rectBorder = CRect (CPoint (x, y), csImage);
		rectBorder.InflateRect (2, 2);

		//----------------------
		// Fill button interior:
		//----------------------
		if (m_pWndParentBar->IsDrawShadedHighlight())
		{
			if (bHighlight && !bCustomizeMode)
			{
				CBCGPDrawManager dm (*pDC);
				dm.HighlightRect (rectBorder, HIGHLIGHT_PERCENTAGE);
			}
		}
		else
		{
			if (m_bExtraSize)
			{
				CSize sizeExtra = CBCGPVisualManager::GetInstance()->GetButtonExtraBorder();
				if (sizeExtra != CSize (0, 0))
				{
					rectBorder.InflateRect (sizeExtra.w / 2 - 1, sizeExtra.h / 2 - 1);
				}
			}

			FillInterior (pDC, rectBorder, bHighlight);
		}

		pImages->Draw (pDC, 
			x + csOffset.w, y + csOffset.h, GetImage(), FALSE, 
			(m_nStyle & TBBS_DISABLED));
	}
	else
	{
		if (bHighlight &&
			m_pWndParentBar->IsDrawShadedHighlight() && !bCustomizeMode)
		{
			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rectBorder, HIGHLIGHT_PERCENTAGE);
		}
	}

	if (!bCustomizeMode &&
		(bHighlight || (m_nStyle & TBBS_PRESSED) || (m_nStyle & TBBS_CHECKED)))
	{
		if (((m_nStyle & TBBS_PRESSED) && bHighlight) ||
			(m_nStyle & TBBS_CHECKED))
		{
			CBCGPVisualManager::GetInstance()->OnDrawButtonBorder (pDC,
				this, rectBorder, CBCGPVisualManager::ButtonsIsPressed);
		}
		else
		{							  
			CBCGPVisualManager::GetInstance()->OnDrawButtonBorder (pDC,
				this, rectBorder, CBCGPVisualManager::ButtonsIsHighlighted);
		}
	}

	if (m_bTextBelow && !m_strText.IsEmpty())
	{
		COLORREF clrText = (COLORREF)-1;
		CBCGPVisualManager::BCGBUTTON_STATE state = CBCGPVisualManager::ButtonsIsRegular;

		if (bHighlight)
		{
			state = CBCGPVisualManager::ButtonsIsHighlighted;
		}
		else if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
		{
			//-----------------------
			// Pressed in or checked:
			//-----------------------
			state = CBCGPVisualManager::ButtonsIsPressed;
		}

		if (m_nStyle & TBBS_DISABLED)
		{
			if (m_pWndParentBar->IsBackgroundTexture())
			{
				clrText = globalData.clrGrayedText;
			}
		}
		else
		{
			clrText = m_pWndParentBar->GetRegularColor();
		}

		if (clrText == (COLORREF)-1)
		{
			if (m_pWndParentBar->IsBackgroundTexture())
			{
				clrText = globalData.clrWindowText;
			}
			else
			{
				clrText = CBCGPVisualManager::GetInstance()->GetToolbarButtonTextColor (
														this, state);
			}
		}

		pDC->SetTextColor (clrText);

		if (m_bIsWholeText)
		{
			pDC->DrawText (m_strText, rectText, 
						DT_WORDBREAK | DT_CENTER);
		}
		else
		{
			CString strText = m_strText;
			pDC->DrawText (strText, rectText, DT_WORDBREAK | DT_END_ELLIPSIS);
		}
	}
}
//****
ISIZE CBCGPOutlookButton::OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CSize sizeResult = sizeDefault;

	if (!bHorz)
	{
		int nCXMargin = pDC->GetTextExtent (_T("   ")).w;
		CRect rectText (0, 0, sizeDefault.w - nCXMargin, 1);

		int iTextHeight = m_bTextBelow ? pDC->DrawText (m_strText, rectText, 
			DT_CALCRECT | DT_WORDBREAK) : 0;

		sizeResult.h = sizeDefault.h + iTextHeight + BUTTON_OFFSET;
		sizeResult.w = max (m_sizeImage.w + 4, min (sizeDefault.w, rectText.Width()));

		m_bIsWholeText = rectText.Width() <= sizeDefault.w;
	}
	else
	{
		CRect rectText (0, 0, 0, sizeDefault.h);
		int iTextHeight = 0;

		if (m_bTextBelow)
		{
			do
			{
				rectText.r ++;
				iTextHeight = pDC->DrawText (m_strText, rectText, 
								DT_CALCRECT | DT_WORDBREAK);
			}
			while (iTextHeight < pDC->GetTextExtent (m_strText).h &&
					rectText.Height() > sizeDefault.h);
		}

		sizeResult.w = sizeDefault.w + rectText.Width() + BUTTON_OFFSET;
		sizeResult.h = max (m_sizeImage.h, min (sizeDefault.h, rectText.Height()));

		m_bIsWholeText = TRUE;
	}

	return sizeResult;
}
//****
void CBCGPOutlookButton::OnChangeParentWnd (CWnd* pWndParent)
{
	CBCGPToolbarButton::OnChangeParentWnd (pWndParent);

	m_pWndParentBar = DYNAMIC_DOWNCAST (CBCGPOutlookBarPane, pWndParent);
	ASSERT_VALID (m_pWndParentBar);
}
//****
BOOL CBCGPOutlookButton::CanBeDropped (CBCGPToolBar* pToolbar)
{
	ASSERT_VALID (pToolbar);
	return pToolbar->IsKindOf (RUNTIME_CLASS (CBCGPOutlookBarPane));
}
//****
void CBCGPOutlookButton::SetImage (int iImage)
{
	// Don't add image to hash!
	m_iImage = iImage;
}
