//
// BCGPPopupWindow.cpp : implementation file
//

//#include "multimon.h"
#include "BCGPVisualManager.h"
#include "BCGPPopupWindow.h"

#ifndef BCGP_EXCLUDE_POPUP_WINDOW

#include "BCGPDrawManager.h"
#include "BCGPContextMenuManager.h"

IMPLEMENT_DYNAMIC(CBCGPPopupWindow, CWnd)

// Timer IDs:
static const int iClosePopupTimerId = 1;
static const int iAnimTimerId_ppw = 2;
static const int iCheckActivityTimerId = 3;

static clock_t nLastAnimTime_ppw = 0;
static const int nSmallCaptionHeight = 7;

UINT BCGM_ON_CLOSEPOPUPWINDOW = ::RegisterWindowMessage (_T("BCGM_ON_CLOSEPOPUPWINDOW"));

/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupWndButton window

void CBCGPPopupWndButton::OnFillBackground (CDC* pDC, const CRect& rectClient)
{
	CBCGPVisualManager::GetInstance()->
		OnErasePopupWindowButton (pDC, rectClient, this);
}
//****
void CBCGPPopupWndButton::OnDrawBorder (CDC* pDC, CRect& rectClient, UINT /*uiState*/)
{
	CBCGPVisualManager::GetInstance()->OnDrawPopupWindowButtonBorder (pDC, rectClient, this);
}
//****
void CBCGPPopupWndButton::OnDraw (CDC* pDC, const CRect& rect, UINT uiState)
{
	if (CBCGPVisualManager::GetInstance()->
		IsDefaultWinXPPopupButton (this))
	{
		return;
	}

	CBCGPButton::OnDraw (pDC, rect, uiState);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupWindow

CBCGPPopupWindow::CBCGPPopupWindow()
{
	m_pWndOwner = NULL;
	m_bIsActive = FALSE;

	m_nAutoCloseTime = 3000;
	m_bSmallCaption = TRUE;
	m_bHasCloseButton = TRUE;
	m_hMenu = NULL;
	m_pWndDlg = NULL;
	m_uiDlgResID = 0;

	m_nBtnMarginVert = 2;
	m_nBtnMarginHorz = 2;

	m_nTransparency = 255;	// Opaque

	m_AnimationType = CBCGPPopupMenu::NO_ANIMATION;
	m_AnimationSpeed = 30;
	m_nAnimationAlpha = 0;
	m_bAnimationIsDone = FALSE;
	m_AnimSize = CSize (0, 0);
	m_FinalSize = CSize (0, 0);
	m_bIsAnimRight = FALSE;
	m_bIsAnimDown = FALSE;
	m_bFadeOutAnimation = FALSE;
	m_ptLastPos = CPoint (-1, -1);
	m_bMoving = FALSE;
	m_ptStartMove = CPoint (-1, -1);
}
//****
CBCGPPopupWindow::~CBCGPPopupWindow()
{
}

BEGIN_MESSAGE_MAP(CBCGPPopupWindow, CWnd)
	//{{AFX_MSG_MAP(CBCGPPopupWindow)
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupWindow message handlers

BOOL CBCGPPopupWindow::Create(CWnd* pWndOwner, UINT uiDlgResID, HMENU hMenu, CPoint ptPos,
							  CRuntimeClass* pRTIDlgBar)
{
	ASSERT (pRTIDlgBar->IsDerivedFrom (RUNTIME_CLASS (CBCGPPopupDlg)));

	m_hMenu = hMenu;
	m_pWndOwner = pWndOwner;
	m_uiDlgResID = uiDlgResID;

	m_pWndDlg = (CBCGPPopupDlg*) pRTIDlgBar->CreateObject();
	ASSERT_VALID (m_pWndDlg);

	return CommonCreate (ptPos);
}
//****
BOOL CBCGPPopupWindow::Create (CWnd* pWndOwner, CBCGPPopupWndParams& params, HMENU hMenu, CPoint ptPos)
{
	m_hMenu = hMenu;
	m_pWndOwner = pWndOwner;

	m_pWndDlg = new CBCGPPopupDlg;
	ASSERT_VALID (m_pWndDlg);

	m_pWndDlg->m_bDefault = TRUE;

	return CommonCreate (ptPos, &params);
}
//****
BOOL CBCGPPopupWindow::CommonCreate (CPoint ptPos, CBCGPPopupWndParams* pParams)
{
	m_ptLastPos = ptPos;

	int nCaptionHeight = GetCaptionHeight();

	HWND hwndFocus = ::GetFocus();
	HWND hwndForeground = ::GetForegroundWindow();

	CString strClassName = ::AfxRegisterWndClass (
			CS_SAVEBITS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_BTNFACE + 1), NULL);

	CRect rectDummy (0, 0, 0, 0);
	DWORD dwStyleEx = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

	if (globalData.IsWindowsLayerSupportAvailable() && 
		globalData.m_nBitsPerPixel > 8 &&
		m_nTransparency < 255)
	{
		dwStyleEx |= WS_EX_LAYERED;
	}

	if (!CWnd::CreateEx (dwStyleEx, strClassName, _T(""), WS_POPUP, rectDummy, NULL, 0))
	{
		return FALSE;
	}

	CSize sizeDialog;

	m_pWndDlg->m_bDontSetFocus = TRUE;

	if (m_uiDlgResID != 0)
	{
		if (!m_pWndDlg->Create (m_uiDlgResID, this))
		{
			return FALSE;
		}

		sizeDialog = GetDialogSize();
	}
	else
	{
		ASSERT (pParams != NULL);

		if (!m_pWndDlg->CreateFromParams (*pParams, this))
		{
			return FALSE;
		}

		sizeDialog = m_pWndDlg->GetDlgSize();
	}

	m_pWndDlg->m_bDontSetFocus = FALSE;

	CSize sizeBtn = CBCGPMenuImages::Size() + CSize (6, 6);
	BOOL bButtonsOnCaption = (sizeBtn.h + 2 <= nCaptionHeight);

	if (!bButtonsOnCaption && (m_bHasCloseButton || m_hMenu != NULL))
	{
		sizeDialog.w += m_nBtnMarginHorz;

		if (m_bHasCloseButton)
		{
			sizeDialog.w += sizeBtn.w;
		}

		if (m_hMenu != NULL)
		{
			sizeDialog.w += sizeBtn.w;
		}
	}

	CRect rectScreen;

	MONITORINFO mi;
	mi.cbSize = sizeof (MONITORINFO);
	if (GetMonitorInfo (MonitorFromPoint (ptPos, MONITOR_DEFAULTTONEAREST), &mi))
	{
		rectScreen = mi.rcWork;
	}
	else
	{
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
	}

	sizeDialog.w = min (rectScreen.Width() - 2, sizeDialog.w);
	sizeDialog.h = min (rectScreen.Height() - nCaptionHeight - 2, sizeDialog.h);

	m_FinalSize = sizeDialog;
	m_FinalSize.h += nCaptionHeight + 2;
	m_FinalSize.w += 2;

	CWnd* pBtnParent = bButtonsOnCaption ? (CWnd*) this : m_pWndDlg;

	int nBtnVertOffset = bButtonsOnCaption ?
		(nCaptionHeight - sizeBtn.h) / 2 + 1 : m_nBtnMarginVert;

	CRect rectBtn = CRect (	CPoint (sizeDialog.w - sizeBtn.w - m_nBtnMarginHorz, nBtnVertOffset),
							sizeBtn);
	if (m_bHasCloseButton)
	{
		m_btnClose.Create (_T(""), WS_CHILD | WS_VISIBLE, rectBtn, pBtnParent, 
			(UINT) -1);

		m_btnClose.SetStdImage (CBCGPMenuImages::IdClose, CBCGPMenuImages::ImageBlack);
		m_btnClose.m_bDrawFocus = FALSE;
		m_btnClose.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		m_btnClose.m_bIsCaptionButton = bButtonsOnCaption;
		m_btnClose.m_bIsCloseButton = TRUE;

		rectBtn.OffsetRect (-sizeBtn.w - 1, 0);
	}

	if (m_hMenu != NULL)
	{
		m_btnMenu.Create (_T(""), WS_CHILD | WS_VISIBLE, rectBtn, pBtnParent, 
			(UINT) -1);

		m_btnMenu.SetStdImage (CBCGPMenuImages::IdArowDownLarge, CBCGPMenuImages::ImageBlack);
		m_btnMenu.m_bDrawFocus = FALSE;
		m_btnMenu.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		m_btnMenu.m_bIsCaptionButton = bButtonsOnCaption;
	}

	if (ptPos == CPoint (-1, -1))
	{
		ptPos.x = rectScreen.r - m_FinalSize.w;
		ptPos.y = rectScreen.b - m_FinalSize.h;
	}
	else
	{
		if (ptPos.x < rectScreen.l)
		{
			ptPos.x = rectScreen.l;
		}
		else if (ptPos.x + m_FinalSize.w > rectScreen.r)
		{
			ptPos.x = rectScreen.r - m_FinalSize.w;
		}

		if (ptPos.y < rectScreen.t)
		{
			ptPos.y = rectScreen.t;
		}
		else if (ptPos.y + m_FinalSize.h > rectScreen.b)
		{
			ptPos.y = rectScreen.b - m_FinalSize.h;
		}
	}

	OnBeforeShow (ptPos);

	SetWindowPos (&wndTop, ptPos.x, ptPos.y, m_FinalSize.w, m_FinalSize.h,
		SWP_NOACTIVATE | SWP_SHOWWINDOW);

	StartAnimation();

	m_pWndDlg->SetWindowPos (NULL, 1, nCaptionHeight + 1,
		sizeDialog.w, sizeDialog.h, SWP_NOZORDER | SWP_NOACTIVATE);

	SetTimer (iCheckActivityTimerId, 100, NULL);

	if (::IsWindow (hwndForeground))
	{
		::SetForegroundWindow (hwndForeground);
	}

	if (::IsWindow (hwndFocus))
	{
		::SetFocus (hwndFocus);
	}

	return TRUE;
}
//****
CSize CBCGPPopupWindow::GetDialogSize()
{
	CDialogTemplate dlgt;
	if (!dlgt.Load (MAKEINTRESOURCE (m_uiDlgResID)))
	{
		ASSERT (FALSE);
		return CSize (0, 0);
	}

	CSize sizeDialog;
	dlgt.GetSizeInPixels (&sizeDialog);

	return sizeDialog;
}
//****
void CBCGPPopupWindow::OnTimer(UINT_PTR nIDEvent) 
{
	switch (nIDEvent)
	{
	case iAnimTimerId_ppw:
		if (!m_bAnimationIsDone)
		{
			clock_t nCurrAnimTime = clock();

			int nDuration = nCurrAnimTime - nLastAnimTime_ppw;
			int nSteps = (int) (.5 + (float) nDuration / m_AnimationSpeed);

			if (m_bFadeOutAnimation)
			{
				nSteps = -nSteps;
			}

			switch (m_AnimationType)
			{
			case CBCGPPopupMenu::UNFOLD:
				m_AnimSize.w += nSteps * m_nAnimStepX;
				// no break intentionally

			case CBCGPPopupMenu::SLIDE:
				m_AnimSize.h += nSteps * m_nAnimStepY;
				break;

			case CBCGPPopupMenu::FADE:
				m_iFadePercent += m_iFadeStep;

				if (m_iFadePercent > 100 + nSteps * m_iFadeStep)
				{
					m_iFadePercent = 101;
				}
				break;
			}

			m_AnimSize.w = max (0, min (m_AnimSize.w, m_FinalSize.w));
			m_AnimSize.h = max (0, min (m_AnimSize.h, m_FinalSize.h));

			if (m_bFadeOutAnimation &&
				!m_bIsActive &&
				(m_AnimSize.w == 0 || m_AnimSize.h == 0 ||
				(m_AnimationType == CBCGPPopupMenu::FADE && m_iFadePercent <= 50)))
			{
				SendMessage (WM_CLOSE);
				return;
			}

			if ((m_AnimationType != CBCGPPopupMenu::FADE && m_AnimSize.h >= m_FinalSize.h && m_AnimSize.w >= m_FinalSize.w) ||
				(m_AnimationType == CBCGPPopupMenu::UNFOLD && m_AnimSize.w >= m_FinalSize.w) ||
				(m_AnimationType == CBCGPPopupMenu::FADE && m_iFadePercent > 100) ||
				m_bIsActive)
			{
				m_AnimSize.w = m_FinalSize.w;
				m_AnimSize.h = m_FinalSize.h;

				KillTimer (iAnimTimerId_ppw);

				if (m_btnClose.GetSafeHwnd() != NULL)
				{
					m_btnClose.ShowWindow (SW_SHOWNOACTIVATE);
				}

				if (m_btnMenu.GetSafeHwnd() != NULL)
				{
					m_btnMenu.ShowWindow (SW_SHOWNOACTIVATE);
				}

				m_pWndDlg->SetWindowPos (NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW|SWP_NOZORDER|SWP_SHOWWINDOW | SWP_NOACTIVATE);
				m_pWndDlg->ValidateRect (NULL);
    
				m_bAnimationIsDone = TRUE;

				if (m_nAutoCloseTime > 0)
				{
					SetTimer (iClosePopupTimerId, m_nAutoCloseTime, NULL);
				}
			}

			if (m_bFadeOutAnimation && m_AnimationType != CBCGPPopupMenu::FADE)
			{
				CRect rectWnd;
				GetWindowRect (rectWnd);

				int x = m_bIsAnimRight ? 
					rectWnd.l : rectWnd.r - m_AnimSize.w;
				int y = m_bIsAnimDown ? 
					rectWnd.t : rectWnd.b - m_AnimSize.h;

				SetWindowPos (NULL, x, y, m_AnimSize.w, m_AnimSize.h,
					SWP_NOZORDER | SWP_NOACTIVATE);
			}
			else
			{
				Invalidate();
				UpdateWindow();
			}

			nLastAnimTime_ppw = nCurrAnimTime;
		}
		break;

	case iClosePopupTimerId:
		if (!m_bIsActive && !m_bMoving)
		{
			KillTimer (iClosePopupTimerId);
			StartAnimation (FALSE);
		}
		return;

	case iCheckActivityTimerId:
		if (!m_bMoving)
		{
			BOOL bWasActive = m_bIsActive;

			CRect rectWnd;
			GetWindowRect (rectWnd);

			CPoint ptCursor;
			GetCursorPos (&ptCursor);

			m_bIsActive = 
				rectWnd.PtInRect (ptCursor) || m_pWndDlg->HasFocus();

			if (m_bIsActive != bWasActive &&
				globalData.IsWindowsLayerSupportAvailable() && 
				globalData.m_nBitsPerPixel > 8 &&
				m_nTransparency < 255)
			{
				BYTE nTransparency = m_bIsActive ? (BYTE) 255 : m_nTransparency;
				globalData.SetLayeredAttrib (GetSafeHwnd(), 0, 
					nTransparency, LWA_ALPHA);
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}
//****
void CBCGPPopupWindow::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
	delete this;
}
//****
BOOL CBCGPPopupWindow::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}
//****
void CBCGPPopupWindow::OnPaint(CDC* pDC) 
{
	CPaintDC dc(this); // device context for painting

	if (!m_bAnimationIsDone)
	{
		DrawAnimation (&dc);
	}
	else
	{
		OnDraw (&dc);
	}
}
//****
CRect CBCGPPopupWindow::GetCaptionRect()
{
	int nCaptionHeight = GetCaptionHeight();

	CRect rectClient;
	GetClientRect (&rectClient);
	
	CRect rectCaption = rectClient;

	rectCaption.DeflateRect (1, 1);
	rectCaption.b = rectCaption.t + nCaptionHeight;

	return rectCaption;
}
//****
BOOL CBCGPPopupWindow::ProcessCommand (HWND hwnd)
{
	ASSERT_VALID (m_pWndDlg);

	if (hwnd == m_btnClose.GetSafeHwnd())
	{
		SendMessage (WM_CLOSE);
		return TRUE;
	}

	if (hwnd == m_btnMenu.GetSafeHwnd() && m_hMenu != NULL)
	{
		CRect rectMenuBtn;
		m_btnMenu.GetWindowRect (rectMenuBtn);

		const int x = rectMenuBtn.l;
		const int y = rectMenuBtn.b;

		UINT nMenuResult = 0;

		m_pWndDlg->m_bMenuIsActive = TRUE;
		
		if (g_pContextMenuManager != NULL)
		{
			const BOOL bMenuShadows = CBCGPMenuBar::IsMenuShadows();
			CBCGPMenuBar::EnableMenuShadows (FALSE);

			nMenuResult = g_pContextMenuManager->TrackPopupMenu (
				m_hMenu, x, y, this);

			CBCGPMenuBar::EnableMenuShadows (bMenuShadows);
		}
		else
		{
			nMenuResult = ::TrackPopupMenu (m_hMenu, 
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, 
				x, y, 0, GetSafeHwnd(), NULL);
		}

		m_pWndDlg->m_bMenuIsActive = FALSE;
		
		if (nMenuResult != 0)
		{
			if (m_pWndOwner != NULL)
			{
				m_pWndOwner->PostMessage (WM_COMMAND, nMenuResult);
			}
			else
			{
				m_pWndDlg->PostMessage (WM_COMMAND, nMenuResult);
			}
		}

		OnCancelMode();
		return TRUE;
	}

	return FALSE;
}
//****
BOOL CBCGPPopupWindow::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (ProcessCommand ((HWND)lParam))
	{
		return TRUE;
	}
	
	return CWnd::OnCommand(wParam, lParam);
}
//****
void CBCGPPopupWindow::OnDestroy() 
{
	if (m_pWndDlg != NULL)
	{
		m_pWndDlg->DestroyWindow();
		delete m_pWndDlg;
		m_pWndDlg = NULL;
	}

	CWnd::OnDestroy();
}
//****
LRESULT CBCGPPopupWindow::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if (lp & PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle ((CDC*) wp);
		ASSERT_VALID (pDC);

		OnDraw (pDC);
	}

	return 0;
}
//****
void CBCGPPopupWindow::OnDraw (CDC* pDC)
{
	ASSERT_VALID (pDC);

	CRect rectClient;
	GetClientRect (&rectClient);

	CBCGPVisualManager::GetInstance()->OnDrawPopupWindowBorder (pDC, rectClient);

	CRect rectCaption = GetCaptionRect();

	COLORREF clrText = CBCGPVisualManager::GetInstance()->OnDrawPopupWindowCaption 
		(pDC, rectCaption, this);

	if (m_bSmallCaption)
	{
		return;
	}

	//--------------------
	// Draw icon and name:
	//--------------------
	CRect rectText = rectCaption;
	rectText.l += IMAGE_MARGIN;

	CWnd* pWndBtn = NULL;

	if (m_btnMenu.GetSafeHwnd() != NULL)
	{
		pWndBtn = &m_btnMenu;
	}
	else if (m_btnClose.GetSafeHwnd() != NULL)
	{
		pWndBtn = &m_btnClose;
	}

	if (pWndBtn != NULL)
	{
		CRect rectBtn;
		pWndBtn->GetWindowRect (&rectBtn);
		ScreenToClient (&rectBtn);

		rectText.r = rectBtn.l - IMAGE_MARGIN;
	}

	CBitmap* hIcon = GetIcon (FALSE);
	if (hIcon != NULL)
	{
		CSize sizeImage = globalData.m_sizeSmallIcon;
		CRect rectImage = rectCaption;

		rectImage.t += (rectCaption.Height() - sizeImage.h) / 2;
		rectImage.b = rectImage.t + sizeImage.h;

		rectImage.l += IMAGE_MARGIN;
		rectImage.r = rectImage.l + sizeImage.w;

		pDC->DrawState (rectImage.TopLeft(), rectImage.Size(), 
			hIcon, DSS_NORMAL, (HBRUSH) NULL);

		rectText.l = rectImage.r + IMAGE_MARGIN;
	}

	CString strText;
	GetWindowText (strText);

	if (!strText.IsEmpty())
	{
		COLORREF clrTextOld = pDC->SetTextColor (clrText);
		pDC->SetBkMode (TRANSPARENT);
		CFont* pOldFont = pDC->SelectObject(globalData.fontRegular);

		pDC->DrawText (strText, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

		pDC->SelectObject (pOldFont);
		pDC->SetTextColor (clrTextOld);
	}
}
//****
void CBCGPPopupWindow::StartAnimation (BOOL bShow/* = TRUE*/)
{
	if (m_AnimationType == CBCGPPopupMenu::NO_ANIMATION ||
		globalData.bIsRemoteSession ||
		(m_AnimationType == CBCGPPopupMenu::FADE && globalData.m_nBitsPerPixel <= 8))
	{
		if (!bShow)
		{
			SendMessage (WM_CLOSE);
			return;
		}

		if (m_btnClose.GetSafeHwnd() != NULL)
		{
			m_btnClose.ShowWindow (SW_SHOWNOACTIVATE);
		}

		if (m_btnMenu.GetSafeHwnd() != NULL)
		{
			m_btnMenu.ShowWindow (SW_SHOWNOACTIVATE);
		}

		m_bAnimationIsDone = TRUE;

		if (m_nAutoCloseTime > 0)
		{
			SetTimer (iClosePopupTimerId, m_nAutoCloseTime, NULL);
		}

		m_pWndDlg->SetWindowPos (NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW|SWP_NOZORDER|SWP_SHOWWINDOW | SWP_NOACTIVATE);
		m_pWndDlg->ValidateRect (NULL);

		if (globalData.IsWindowsLayerSupportAvailable() && 
			globalData.m_nBitsPerPixel > 8 &&
			m_nTransparency < 255)
		{
			globalData.SetLayeredAttrib (GetSafeHwnd(), 0, m_nTransparency, LWA_ALPHA);
		}

		return;
	}

	m_bAnimationIsDone = FALSE;
	m_bFadeOutAnimation = !bShow;

	//-------------------------
	// Set animation direction:
	//-------------------------
	CRect rectScreen;
	::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);

	CRect rectWindow;
	GetWindowRect (&rectWindow);

	m_bIsAnimRight = rectWindow.l < rectScreen.CenterPoint().x;
	m_bIsAnimDown = rectWindow.t < rectScreen.CenterPoint().y;

	m_iFadePercent = m_bFadeOutAnimation ? 100 : 50;
	m_iFadeStep = m_bFadeOutAnimation ? -5 : 5;
	
	if (m_FinalSize.w > m_FinalSize.h)
	{
		m_nAnimStepY = 10;
		m_nAnimStepX = max (1, m_nAnimStepY * m_FinalSize.w / m_FinalSize.h);
	}
	else
	{
		m_nAnimStepX = 10;
		m_nAnimStepY = max (1, m_nAnimStepX * m_FinalSize.h / m_FinalSize.w);
	}

	//--------------------------
	// Adjust initial menu size:
	//--------------------------
	m_AnimSize = m_FinalSize;

	if (bShow)
	{
		switch (m_AnimationType)
		{
		case CBCGPPopupMenu::UNFOLD:
			m_AnimSize.w = m_nAnimStepX;

		case CBCGPPopupMenu::SLIDE:
			m_AnimSize.h = m_nAnimStepY;
			break;
		}
	}

	if (m_pWndDlg != NULL && m_pWndDlg->IsWindowVisible())
	{
		m_pWndDlg->ShowWindow (SW_HIDE);
	}

	SetTimer (iAnimTimerId_ppw, m_AnimationSpeed, NULL);
	nLastAnimTime_ppw = clock();
}
//****
void CBCGPPopupWindow::DrawAnimation (CDC* pPaintDC)
{
	CRect rectClient;
	GetClientRect (&rectClient);

	int cx = m_FinalSize.w;
	int cy = m_FinalSize.h;

	CDC dcMem;
	if (!dcMem.CreateCompatibleDC (pPaintDC))
	{
		return;
	}

	// create the three bitmaps if not done yet
	if (m_bmpScreenDst.GetSafeHandle() == NULL)
	{
		CBitmap* pBmpOld = NULL;

		if (m_AnimationType == CBCGPPopupMenu::FADE || globalData.m_nBitsPerPixel > 8)
		{
			// Fill in the BITMAPINFOHEADER
			BITMAPINFOHEADER bih;
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = cx;
			bih.biHeight = cy;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = cx * cy;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed = 0;
			bih.biClrImportant = 0;

			CBitmap* hmbpDib;
			// Create a DIB section and attach it to the source bitmap
			hmbpDib = CreateDIBSection (
				dcMem.m_hDC, (LPBITMAPINFO)&bih, DIB_RGB_COLORS, (void **)&m_cFadeSrcBits,
				NULL, NULL);
			if (hmbpDib == NULL || m_cFadeSrcBits == NULL)
			{
				return;
			}

			m_bmpScreenSrc.Attach( hmbpDib );

			// Create a DIB section and attach it to the destination bitmap
			hmbpDib = CreateDIBSection (
				dcMem.m_hDC, (LPBITMAPINFO)&bih, DIB_RGB_COLORS, (void **)&m_cFadeDstBits,
				NULL, NULL);
			if (hmbpDib == NULL || m_cFadeDstBits == NULL)
			{
				return;
			}
			m_bmpScreenDst.Attach( hmbpDib );

			// Create a DIB section and attach it to the temporary bitmap
			hmbpDib = CreateDIBSection (
				dcMem.m_hDC, (LPBITMAPINFO)&bih, DIB_RGB_COLORS, (void **)&m_cFadeTmpBits,
				NULL, NULL);
			if (hmbpDib == NULL || m_cFadeTmpBits == NULL)
			{
				return;
			}

			m_bmpScreenTmp.Attach( hmbpDib );

			// get source image, representing the window below the popup menu
			pBmpOld = dcMem.SelectObject (&m_bmpScreenSrc);
			dcMem.BitBlt (0, 0, cx, cy, pPaintDC, rectClient.l, rectClient.t, SRCCOPY);

			// copy it to the destination so that shadow will be ok
			memcpy (m_cFadeDstBits, m_cFadeSrcBits, sizeof(COLORREF)* cx*cy);
			dcMem.SelectObject (&m_bmpScreenDst);
		}
		else
		{
			m_bmpScreenDst.CreateCompatibleBitmap (pPaintDC, cx, cy);
			pBmpOld = dcMem.SelectObject (&m_bmpScreenDst);
		}

		SendMessage (WM_PRINT, (WPARAM) dcMem.GetSafeHdc(), 
			PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND);

		if (m_btnClose.GetSafeHwnd() != NULL)
		{
			m_btnClose.ShowWindow (SW_HIDE);
		}

		if (m_btnMenu.GetSafeHwnd() != NULL)
		{
			m_btnMenu.ShowWindow (SW_HIDE);
		}

		CRect rect;
		m_pWndDlg->GetWindowRect (&rect);
		ScreenToClient (&rect);

		dcMem.SetViewportOrg (rect.TopLeft());
		m_pWndDlg->SendMessage (WM_PRINT, (WPARAM) dcMem.GetSafeHdc(), 
			PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND);

		dcMem.SetViewportOrg (CPoint (0,0));

		dcMem.SelectObject (pBmpOld);

		if (globalData.IsWindowsLayerSupportAvailable() && 
			globalData.m_nBitsPerPixel > 8 &&
			m_nTransparency < 255)
		{
			globalData.SetLayeredAttrib (GetSafeHwnd(), 0, m_nTransparency, LWA_ALPHA);
		}
	}
	
	COLORREF *src = m_cFadeSrcBits;
	COLORREF *dst = m_cFadeDstBits;
	COLORREF *tmp = m_cFadeTmpBits;

	CBitmap* pBmpOld = NULL;

	switch (m_AnimationType)
	{
	case CBCGPPopupMenu::UNFOLD:
	case CBCGPPopupMenu::SLIDE:
		pBmpOld = dcMem.SelectObject (&m_bmpScreenDst);

		pPaintDC->BitBlt (
			m_bIsAnimRight ? rectClient.l : rectClient.r - m_AnimSize.w,
			m_bIsAnimDown ? rectClient.t : rectClient.b - m_AnimSize.h,
			m_AnimSize.w, m_AnimSize.h, &dcMem, 0, 0, SRCCOPY);
		break;

	case CBCGPPopupMenu::FADE:
		pBmpOld = dcMem.SelectObject (&m_bmpScreenTmp);
		for (int pixel = 0; pixel < cx * cy; pixel++)
		{
			*tmp++ = CBCGPDrawManager::PixelAlpha (*src++, *dst++, 100 - m_iFadePercent);
		}

		pPaintDC->BitBlt (rectClient.l, rectClient.t, cx, cy, 
						&dcMem, 0, 0, SRCCOPY);
	}

	dcMem.SelectObject (pBmpOld);
}
//****
int CBCGPPopupWindow::GetCaptionHeight()
{
	if (m_bSmallCaption)
	{
		return nSmallCaptionHeight;
	}
	else
	{
		CSize sizeBtn = CBCGPMenuImages::Size() + CSize (6, 6);
		return max (::GetSystemMetrics (SM_CYSMCAPTION), sizeBtn.h + 2);
	}
}
//****
void CBCGPPopupWindow::OnClose() 
{
	if (m_pWndOwner->GetSafeHwnd() != NULL)
	{
		m_pWndOwner->SendMessage (BCGM_ON_CLOSEPOPUPWINDOW, 0, (LPARAM) this);
	}

	CWnd::OnClose();
}
//****
void CBCGPPopupWindow::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
}
//****
void CBCGPPopupWindow::StartWindowMove()
{
	m_bMoving = TRUE;
	GetCursorPos (&m_ptStartMove);

	SetCapture();
	KillTimer (iClosePopupTimerId);
}
//****
void CBCGPPopupWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bMoving)
	{
		// ---------------------
		// Calc screen rectangle
		// ---------------------
		CRect rectScreen;

		CPoint ptCursor = point;
		ClientToScreen (&ptCursor);

		MONITORINFO mi;
		mi.cbSize = sizeof (MONITORINFO);
		if (GetMonitorInfo (MonitorFromPoint (ptCursor, MONITOR_DEFAULTTONEAREST), &mi))
		{
			rectScreen = mi.rcWork;
		}
		else
		{
			::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
		}

		CPoint ptMouse;
		GetCursorPos (&ptMouse);
		
		CPoint ptOffset = ptMouse - m_ptStartMove;
		m_ptStartMove = ptMouse;
		
		CRect rect;
		GetWindowRect (&rect);
		rect.OffsetRect (ptOffset);
		
		if (rect.l < rectScreen.l)
		{
			rect.OffsetRect (rectScreen.l - rect.l, 0);
		}
		else if (rect.r > rectScreen.r)
		{
			rect.OffsetRect (rectScreen.r - rect.r, 0);
		}

		if (rect.t < rectScreen.t)
		{
			rect.OffsetRect (0, rectScreen.t - rect.t);
		}
		else if (rect.b > rectScreen.b)
		{
			rect.OffsetRect (0, rectScreen.b - rect.b);
		}

		SetWindowPos (NULL, rect.l, rect.t, -1, -1,
			SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}
//****
void CBCGPPopupWindow::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bMoving)
	{
		ReleaseCapture();
		m_bMoving = FALSE;

		if (m_nAutoCloseTime > 0)
		{
			SetTimer (iClosePopupTimerId, m_nAutoCloseTime, NULL);
		}

		CRect rectWnd;
		GetWindowRect (rectWnd);

		m_ptLastPos = rectWnd.TopLeft();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}
//****
void CBCGPPopupWindow::OnCancelMode() 
{
	CWnd::OnCancelMode();
	
	if (m_bMoving)
	{
		ReleaseCapture();
		m_bMoving = FALSE;

		if (m_nAutoCloseTime > 0)
		{
			SetTimer (iClosePopupTimerId, m_nAutoCloseTime, NULL);
		}

		CRect rectWnd;
		GetWindowRect (rectWnd);

		m_ptLastPos = rectWnd.TopLeft();
	}
}
//****
void CBCGPPopupWindow::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
	StartWindowMove();
}

#endif // BCGP_EXCLUDE_POPUP_WINDOW
