
#include <math.h>
#include "BCGPCalculator.h"

//static const int SEPARATOR_SIZE = 2;
static const int nDefaultButtonSize = 25;

/////////////////////////////////////////////////////////////////////////////
// CCalculatorButton

#if 0
void CCalculatorButton::OnDraw (CDC* pDC, const CRect& rect, img_t* /*pImages*/,
								BOOL /*bHorz*/, BOOL /*bCustomizeMode*/, BOOL bHighlight,
								BOOL /*bDrawBorder*/, BOOL /*bGrayDisabledButtons*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParent);

	BCGBUTTON_STATE state = ButtonsIsRegular;

	//if (!IsCustomizeMode())
	{
		if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
		{
			//-----------------------
			// Pressed in or checked:
			//-----------------------
			state = ButtonsIsPressed;
		}
		else if (bHighlight)
		{
			state = ButtonsIsHighlighted;
		}
	}

	if (m_pParent->OnDrawButton(pDC, rect, this, state, m_uiCmd))
	{
		COLORREF clrText = _RGB (0, 0, 255);

		if (m_bIsUserCommand)
		{
			clrText = _RGB (255, 0, 255);
		}
		else if (m_uiCmd != idCommandNone &&
			m_uiCmd != idCommandDot)
		{
			clrText = _RGB (255, 0, 0);
		}

		pDC->SetTextColor (clrText);

		CRect rectText = rect;
		pDC->DrawText (m_strText, &rectText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
}

#endif

#if 0

void CCalculatorButton::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* /*pImages*/,
								BOOL /*bHorz*/, BOOL /*bCustomizeMode*/, BOOL /*bHighlight*/,
								BOOL /*bDrawBorder*/, BOOL /*bGrayDisabledButtons*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParent);

	const CString strMem = _T("M");

	if (m_pParent->OnDrawDisplay (pDC, rect, m_strText, m_bMem))
	{
		CRect rectText = rect;

		if (m_bMem)
		{
			rectText.l += pDC->GetTextExtent (strMem).w;
		}

		rectText.DeflateRect (4, 2);

		pDC->DrawText (m_strText, &rectText, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

		if (m_bMem)
		{
			rectText = rect;
			rectText.DeflateRect (4, 2);

			pDC->DrawText (strMem, &rectText, DT_SINGLELINE);
		}
	}
}

#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPCalculator
CBCGPCalculator::CBCGPCalculator()
{
	m_dblValue = 0.;
	m_nCommandID = 0;

	CommonInit();
}
//****
CBCGPCalculator::CBCGPCalculator(double dblValue, UINT nID, CBCGPEdit* pParentEdit) :
		m_dblValue (dblValue),
		m_nCommandID (nID)
{
	CommonInit();
}
//****
void CBCGPCalculator::CommonInit()
{
	m_bLocked = TRUE;
	m_bIsEnabled = TRUE;
	m_bInternal = FALSE;
	m_nVertMargin = 3;
	m_nHorzMargin = 3;
	m_dblSecondValue = 0.;
	m_dblMemValue = 0.;
	m_uiLastCmd = 0;

	m_bIsClearBuffer = TRUE;
	m_bIsError = FALSE;
	m_bSeqResult = FALSE;

	m_nRows = 5;
	m_nColumns = 5;

	UpdateBuffer();
}
//****
void CBCGPCalculator::AdjustLocations(const CRect& rcClient)
{
	if (m_bInUpdateShadow)
	{
		return;
	}

	CRect rectClient = rcClient;	// Client area rectangle

	if (rectClient.Width() < 150 || rectClient.Height() < 150)
	{
		m_nVertMargin = 1;
		m_nHorzMargin = 1;
	}
	else
	{
		m_nVertMargin = 3;
		m_nHorzMargin = 3;
	}

	rectClient.DeflateRect (m_nHorzMargin, m_nVertMargin);

	int cx = max (nDefaultButtonSize, rectClient.Width() / m_nColumns);

	CCalculatorButton* pDisplayButton =  m_Buttons;

  int tmHeight = font_text_size(c->fo, "H", 1, 0).h;
	const int nDisplayHeight = tmHeight + 2 +  4 * m_nVertMargin;

	m_rectDisplay = CRect (rectClient.TopLeft(),  CSize (rectClient.Width(), nDisplayHeight));
	m_rectDisplay.DeflateRect (m_nHorzMargin, m_nVertMargin);

	CRect rectBackspace = m_rectDisplay;
	rectBackspace.l = rectBackspace.r - cx;
	rectBackspace.DeflateRect (m_nHorzMargin, 0);

	m_rectDisplay.r = rectBackspace.l - m_nHorzMargin;
	pDisplayButton->SetRect (m_rectDisplay);

	CCalculatorButton* pBackspaceButton =  m_Buttons;

	pBackspaceButton->SetRect (rectBackspace);

	rectClient.t = m_rectDisplay.b + m_nVertMargin;

	int x = rectClient.l;
	int y = rectClient.t;

	CSize sizeButton (	cx, max (nDefaultButtonSize, rectClient.Height() / m_nRows));

	for (int i = 0; i<m_nButton; i++)
	{
		if (i == m_nColumns)
		{
			rectBackspace.r = x - m_nHorzMargin;
			rectBackspace.l = x - sizeButton.w + m_nHorzMargin;

			pBackspaceButton->SetRect (rectBackspace);

			m_rectDisplay.r = rectBackspace.l - 2 * m_nHorzMargin;
			pDisplayButton->SetRect (m_rectDisplay);

			i = 0;

			x = rectClient.l;
			y += sizeButton.h;
		}

		CCalculatorButton* pButton =  m_Buttons + i;
		ASSERT_VALID (pButton);

		CRect rectButton = CRect (CPoint (x, y), sizeButton);
		rectButton.DeflateRect (m_nHorzMargin, m_nVertMargin);

		pButton->SetRect (rectButton);

		x += sizeButton.w;
	}

}
//****
CSize CBCGPCalculator::CalcSize (BOOL /*bVertDock*/)
{
	CSize sizeBox (nDefaultButtonSize + 2 * m_nHorzMargin, nDefaultButtonSize + 2 * m_nVertMargin);
  
	for (int i = 0; i<m_nButton; i++)
	{
		CCalculatorButton* pButton = m_Buttons + i;
		ASSERT_VALID (pButton);

		CSize sizeText = font_text_size(c->fo, pButton->m_strText, -1, 0);

		sizeBox.w = max (sizeBox.w, sizeText.w + 2 * m_nHorzMargin + 8);
		sizeBox.h = max (sizeBox.h, sizeText.h + 2 * m_nVertMargin + 8);
	}

	return CSize(
		m_nColumns * sizeBox.w + 2 * m_nHorzMargin, 
		globalData.GetTextHeight() + 4 * m_nVertMargin + m_nRows * sizeBox.h + 2 * m_nVertMargin);
}

BEGIN_MESSAGE_MAP(CBCGPCalculator, CBCGPPopupMenuBar)
	//{{AFX_MSG_MAP(CBCGPCalculator)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPCalculator message handlers

//****
void CBCGPCalculator::Rebuild()
{
  int i=0;
	m_Buttons[i++].init(0);

	m_Buttons[i++].init(_T("<-"),	idCommandBackspace, VK_BACK, 0, TRUE);

	m_Buttons[i++].init(_T("MC"),	idCommandMemClear, _T('L'), _T('l'), TRUE, TRUE);
	m_Buttons[i++].init(_T("MR"),	idCommandMemRead, _T('R'), _T('r'), TRUE, TRUE);
	m_Buttons[i++].init(_T("M+"),	idCommandMemAdd, _T('P'), _T('p'), TRUE, TRUE);
	m_Buttons[i++].init(_T("C"),	idCommandReset, VK_ESCAPE, 0, TRUE);
	m_Buttons[i++].init(_T("CE"),	idCommandClear, VK_DELETE, 0, TRUE);
	
	m_Buttons[i++].init(7);
	m_Buttons[i++].init(8);
	m_Buttons[i++].init(9);

	m_Buttons[i++].init(_T("/"),	idCommandDiv, _T('/'));
	m_Buttons[i++].init(_T("*"),	idCommandMult, _T('*'));

	m_Buttons[i++].init(4);
	m_Buttons[i++].init(5);
	m_Buttons[i++].init(6);

	m_Buttons[i++].init(_T("+/-"),	idCommandSign, VK_F9, 0, TRUE);
	m_Buttons[i++].init(_T("-"),	idCommandSub, _T('-'));

	m_Buttons[i++].init(1);
	m_Buttons[i++].init(2);
	m_Buttons[i++].init(3);

	m_Buttons[i++].init(_T("Sqrt"),idCommandSqrt, _T('@'));
	m_Buttons[i++].init(_T("+"),	idCommandAdd, _T('+'));

	m_Buttons[i++].init(0);
	m_Buttons[i++].init(_T("."),	idCommandDot, _T('.'), _T(','));
	m_Buttons[i++].init(_T("="),	idCommandResult, VK_RETURN, 0, TRUE);
	m_Buttons[i++].init(_T("%"),	idCommandPercent, _T('%'));
	m_Buttons[i++].init(_T("1/x"),	idCommandRev, _T('R'), _T('r'));

	UINT uiCommand = idCommandUser;
#if 0
	for (int j = 0; j<m_lstAdditionalCommands.GetCount(); ++j, uiCommand++)
	{
		CString strCommand = m_lstAdditionalCommands.GetAt(j);

		int nKey = 0;

		int nAmpIndex = strCommand.Find (_T("&"));
		if (nAmpIndex >= 0 && nAmpIndex != strCommand.GetLength() - 1)
		{
			CString strKey;
			strKey += strCommand.GetAt (nAmpIndex + 1);
			strKey.MakeLower();

			nKey = strKey [0];
		}

		m_Buttons[i++].init(strCommand, (CalculatorCommands) uiCommand, nKey, 0, FALSE, FALSE, TRUE);
	}

	if (m_lstAdditionalCommands.IsEmpty())
	{
		m_nRows = 5;
		m_nColumns = 5;
	}
	else
	{
		const int nCount = m_lstAdditionalCommands.GetCount();

		m_nColumns = 5;
		m_nRows = (int) (.5 + (double) nCount / m_nColumns);
  }
#else
		m_nRows = 5;
		m_nColumns = 5;
#endif
}

//****
BOOL CBCGPCalculator::OnSendCommand (const CCalculatorButton* pButton)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pButton);

	const CCalculatorButton* pCalcButton = pButton;
	if (pCalcButton == NULL)
	{
		return FALSE;
	}

	if (m_bIsError && pCalcButton->m_uiCmd != idCommandClear &&
		pCalcButton->m_uiCmd != idCommandReset)
	{
		return TRUE;
	}

	if (pCalcButton->m_uiCmd >= idCommandUser)
	{
		CalcResult();
		UpdateBuffer();
		UpdateDisplay();

		UINT uiCmd = (UINT) pCalcButton->m_uiCmd;

    OnUserCommand (uiCmd);

		m_bIsClearBuffer = TRUE;

		UpdateBuffer();
		UpdateDisplay();
		return TRUE;
	}

	if (pCalcButton->m_uiCmd != idCommandResult)
	{
		m_bSeqResult = FALSE;
	}

	if (pCalcButton->m_nDigit >= 0)
	{
		PushDigit (pCalcButton->m_nDigit);
		return TRUE;
	}

	switch (pCalcButton->m_uiCmd)
	{
	case idCommandBackspace:
		if (!m_bIsError && !m_bIsClearBuffer)
		{
      int len = strlen(m_strBuffer);
			if (len > 1)
			{
				m_strBuffer[len-1] = 0;
			}
			else
			{
				strcpy(m_strBuffer, _T("0"));
			}

#if _MSC_VER < 1400
			_stscanf (m_strBuffer, _T("%lf"), &m_dblValue);
#else
			_stscanf_s (m_strBuffer, _T("%lf"), &m_dblValue);
#endif
			UpdateDisplay();
		}

		return TRUE;

	case idCommandReset:
		m_dblSecondValue = 0.;
		m_uiLastCmd = 0;

	case idCommandClear:
		m_dblValue = 0.;
		strcpy(m_strBuffer, _T("0"));
		m_bIsError = FALSE;
		m_bIsClearBuffer = TRUE;
		m_bSeqResult = FALSE;
		break;

	case idCommandMemClear:
		m_dblMemValue = 0.;
		UpdateDisplay();
		return TRUE;

	case idCommandMemRead:
		m_dblValue = m_dblMemValue;
		break;

	case idCommandMemAdd:
		CalcResult();
		m_dblMemValue += m_dblValue;
		m_bIsClearBuffer = TRUE;
		break;

	case idCommandAdd:
	case idCommandSub:
	case idCommandMult:
	case idCommandDiv:
		if (!m_bIsClearBuffer)
		{
			CalcResult();
			UpdateBuffer();
			UpdateDisplay();
		}

		m_uiLastCmd = pCalcButton->m_uiCmd;
		m_dblSecondValue = m_dblValue;
		m_bIsClearBuffer = TRUE;
		return TRUE;

	case idCommandSign:
		m_dblValue = -m_dblValue;
		m_bIsClearBuffer = TRUE;
		break;

	case idCommandRev:
		if (m_dblValue == 0.)
		{
			m_bIsError = TRUE;
		}
		else
		{
			m_dblValue = 1. / m_dblValue;
			m_bIsClearBuffer = TRUE;
		}
		break;

	case idCommandDot:
		if (m_bIsClearBuffer)
		{
			m_dblValue = 0.;
			strcpy(m_strBuffer, _T("0."));
			m_bIsClearBuffer = FALSE;
		}

		if (strchr(m_strBuffer, _T('.')) == NULL)
		{
			strcat(m_strBuffer, _T("."));
		}

		UpdateDisplay();
		return TRUE;

	case idCommandSqrt:
		if (m_dblValue < 0.)
		{
			m_bIsError = TRUE;
		}
		else
		{
			m_dblValue = sqrt (m_dblValue);
			m_bIsClearBuffer = TRUE;
		}
		break;

	case idCommandPercent:
		m_dblValue = m_dblValue * m_dblSecondValue / 100.;
		UpdateBuffer();
		UpdateDisplay();

		m_bIsClearBuffer = TRUE;
		break;

	case idCommandResult:
		{
			double dblValueSaved = m_dblValue;

			CalcResult();

			if (!m_bIsClearBuffer)
			{
				m_dblSecondValue = dblValueSaved;
			}

      m_bIsClearBuffer = TRUE;
      m_bSeqResult = TRUE;
		}

		break;
	}

	UpdateBuffer();
	UpdateDisplay();

	return TRUE;
}

//****
void CBCGPCalculator::OnLButtonDown(UINT nFlags, CPoint point) 
{
}
//****
BOOL CBCGPCalculator::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_CHAR && OnProcessKey ((int) pMsg->wParam, FALSE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && OnProcessKey ((int) pMsg->wParam, TRUE))
	{
		return TRUE;
	}

	return 0;
}
//****
LRESULT CBCGPCalculator::OnMouseLeave(WPARAM wp,LPARAM lp)
{
	return 0;
}
//****
void CBCGPCalculator::PushDigit (short nDigit)
{
	ASSERT (nDigit >= 0);
	ASSERT (nDigit <= 9);

	if (m_bIsError)
	{
		return;
	}

	if (m_bIsClearBuffer)
	{
		m_dblValue = 0.;
		strcpy(m_strBuffer, _T("0"));
		m_bIsClearBuffer = FALSE;
	}

	char strDigit[20];
	_snprintf(strDigit, 20, _T("%d"), nDigit);

	if (m_strBuffer == _T("0"))
	{
		m_strBuffer[0] = 0;
	}

	strcat(m_strBuffer, strDigit);
#if _MSC_VER < 1400
	_stscanf (m_strBuffer, _T("%lf"), &m_dblValue);
#else
	_stscanf_s (m_strBuffer, _T("%lf"), &m_dblValue);
#endif

	UpdateDisplay();
}
//****
void CBCGPCalculator::UpdateDisplay()
{
}
//****
void CBCGPCalculator::UpdateBuffer()
{
	if (fabs (m_dblValue) > pow (10., 22))
	{
		_snprintf(m_strBuffer, 32, _T("%le"), m_dblValue);
	}
	else
	{
		_snprintf(m_strBuffer, 32, _T("%.16lf"), m_dblValue);

		while (m_strBuffer [strlen(m_strBuffer) - 1] == _T('0'))
		{
			m_strBuffer[strlen(m_strBuffer) - 1] = 0;
		}

		if (m_strBuffer[strlen(m_strBuffer) - 1] == _T('.'))
		{
			m_strBuffer[strlen(m_strBuffer) - 1] = 0;
		}
	}

	if (strstr(m_strBuffer, _T("#INF")) ||
		strstr(m_strBuffer, _T("#IND")) ||
		strstr(m_strBuffer, _T("#NAN")) )
	{
		m_bIsError = TRUE;
		UpdateDisplay();
	}
}
//****
void CBCGPCalculator::CalcResult()
{
	switch (m_uiLastCmd)
	{
	case idCommandAdd:
		m_dblValue += m_dblSecondValue;
		break;

	case idCommandSub:
		m_dblValue = m_bSeqResult ? m_dblValue - m_dblSecondValue : m_dblSecondValue - m_dblValue;
		break;

	case idCommandMult:
		m_dblValue *= m_dblSecondValue;
		break;

	case idCommandDiv:
		if (m_bSeqResult)
		{
			if (m_dblSecondValue == 0.)
			{
				m_bIsError = TRUE;
			}
			else
			{
				m_dblValue = m_dblValue / m_dblSecondValue;
			}
		}
		else
		{
			if (m_dblValue == 0.)
			{
				m_bIsError = TRUE;
			}
			else
			{
				m_dblValue = m_dblSecondValue / m_dblValue;
			}
		}
	}
}
//****
BOOL CBCGPCalculator::OnProcessKey (int nKey, BOOL bIsVirtKey)
{
	for (int i=0; i<m_nButton; ++i)
	{
		CCalculatorButton* pCalculatorButton = m_Buttons+i;
		if (pCalculatorButton != NULL &&
			pCalculatorButton->m_bIsVitKey == bIsVirtKey &&
			(pCalculatorButton->m_nKbdAccel == nKey || pCalculatorButton->m_nKbdAccel2 == nKey))
		{
			if (pCalculatorButton->m_bIsCtrl &&
				(::GetAsyncKeyState (VK_CONTROL) & 0x8000) == 0)
			{
				continue;
			}

			OnSendCommand(pCalculatorButton);
			return TRUE;
		}
	}

	return FALSE;
}
//****
void CBCGPCalculator::SetValue (double dblValue)
{
	ASSERT_VALID (this);

	m_dblValue = dblValue;

	UpdateBuffer();
	UpdateDisplay();
}


//****
void CBCGPCalculator::OnUserCommand (UINT /*uiCmd*/)
{
	ASSERT (FALSE);	// Must be implemented in derived class
}
//****
BOOL CBCGPCalculator::OnDrawButton (CDC* pDC, CRect rect,
		CCalculatorButton* pButton, BCGBUTTON_STATE state, 
		int cmd)
{
	return CBCGPVisualManager::GetInstance()->OnDrawCalculatorButton (pDC, rect, 
		NULL, state, cmd, this);
}
//****
BOOL CBCGPCalculator::OnDrawDisplay (CDC* pDC, CRect rect, 
									 const CString& strText, BOOL bMem)
{
	return CBCGPVisualManager::GetInstance()->OnDrawCalculatorDisplay (
		pDC, rect, strText, bMem, this);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPCalculatorPopup

#if 0

#endif
