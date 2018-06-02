
// BCGPMenuButton.cpp : implementation file
//

#include "BCGPMenuButton.h"
//#include "BCGCBPro.h"
#include "BCGGlobals.h"
#include "BCGPDrawManager.h"

#ifndef _BCGSUITE_
#include "MenuImages.h"
#include "BCGPContextMenuManager.h"
#include "BCGPPopupMenu.h"
#endif

static const int nImageHorzMargin = 10;

/////////////////////////////////////////////////////////////////////////////
// CBCGPMenuButton

IMPLEMENT_DYNAMIC(CBCGPMenuButton, CBCGPButton)

CBCGPMenuButton::CBCGPMenuButton()
{
  m_bRightArrow = FALSE;
  m_hMenu = NULL;
  m_nMenuResult = 0;
  m_bMenuIsActive = FALSE;
  m_bStayPressed = FALSE;
  m_bOSMenu = TRUE;
  m_bDefaultClick = FALSE;
  m_bClickOnMenu = FALSE;
}
//****
CBCGPMenuButton::~CBCGPMenuButton()
{
}


BEGIN_MESSAGE_MAP(CBCGPMenuButton, CBCGPButton)
  //{{AFX_MSG_MAP(CBCGPMenuButton)
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONDOWN()
  ON_WM_GETDLGCODE()
  ON_WM_LBUTTONUP()
  ON_WM_KILLFOCUS()
  ON_WM_LBUTTONDBLCLK()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPMenuButton message handlers

CSize CBCGPMenuButton::SizeToContent(BOOL bCalcOnly)
{
  CSize size = CBCGPButton::SizeToContent(FALSE);
  size.w += CBCGPMenuImages::Size().w;

  if (!bCalcOnly) {
    SetWindowPos(NULL, -1, -1, size.w, size.h,
        SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
  }

  return size;
}
//****
void CBCGPMenuButton::OnDraw(CDC* pDC, const CRect& rect, UINT uiState)
{
  ASSERT_VALID(pDC);

  CBCGPDrawOnGlass dog(m_bOnGlass);

  CSize sizeArrow = CBCGPMenuImages::Size();

  CRect rectParent = rect;
  rectParent.r -= sizeArrow.w + nImageHorzMargin;

  CBCGPButton::OnDraw(pDC, rectParent, uiState);

  CRect rectArrow = rect;
  rectArrow.l = rectParent.r;

  CBCGPMenuImages::Draw(pDC,
      m_bRightArrow ? CBCGPMenuImages::IdArowRightLarge : CBCGPMenuImages::IdArowDownLarge,
      rectArrow,
      (uiState & ODS_DISABLED) ? CBCGPMenuImages::ImageGray : CBCGPMenuImages::ImageBlack);

  if (m_bDefaultClick) {
    //----------------
    // Draw separator:
    //----------------
    CRect rectSeparator = rectArrow;
    rectSeparator.r = rectSeparator.l + 2;
    rectSeparator.DeflateRect(0, 2);

    if (!m_bWinXPTheme || m_bDontUseWinXPTheme) {
      rectSeparator.l += m_sizePushOffset.w;
      rectSeparator.t += m_sizePushOffset.h;
    }

    if (m_bOnGlass) {
      CBCGPDrawManager dm(*pDC);
      dm.DrawLine(rectSeparator.l, rectSeparator.t, rectSeparator.l, rectSeparator.b,
          globalData.clrBarDkShadow);
    }
    else {
      pDC->Draw3dRect(rectSeparator, globalData.clrBtnDkShadow, globalData.clrBtnHilite);
    }
  }
}
//****
void CBCGPMenuButton::OnShowMenu()
{
  if (m_hMenu == NULL || m_bMenuIsActive) {
    return;
  }

  CRect rectWindow;
  GetWindowRect(rectWindow);

  int x, y;

  if (m_bRightArrow) {
    x = rectWindow.r;
    y = rectWindow.t;
  }
  else {
    x = rectWindow.l;
    y = rectWindow.b;
  }

  if (m_bStayPressed) {
    m_bPushed = TRUE;
    m_bHighlighted = TRUE;
  }

  m_bMenuIsActive = TRUE;
  Invalidate();

#ifndef _BCGSUITE_

  if (!m_bOSMenu && g_pContextMenuManager != NULL) {
    m_nMenuResult = g_pContextMenuManager->TrackPopupMenu(
        m_hMenu, x, y, this);
    SetFocus();
  }
  else
#endif
  {
    m_nMenuResult = ::TrackPopupMenu(m_hMenu,
        TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
        x, y, 0, GetSafeHwnd(), NULL);
  }

  if (m_nMenuResult != 0) {
    //-------------------------------------------------------
    // Trigger mouse up event (to button click notification):
    //-------------------------------------------------------
    CWnd* pParent = GetParent();

    if (pParent != NULL) {
      pParent->SendMessage(WM_COMMAND,
          MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED),
          (LPARAM) m_hWnd);
    }
  }

  m_bPushed = FALSE;
  m_bHighlighted = FALSE;
  m_bMenuIsActive = FALSE;

  Invalidate();
  UpdateWindow();

  if (m_bCaptured) {
    ReleaseCapture();
    m_bCaptured = FALSE;
  }
}
//****
void CBCGPMenuButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (nChar == VK_SPACE || nChar == VK_DOWN) {
    m_bClickOnMenu = TRUE;
    OnShowMenu();
    return;
  }

  CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}
//****
void CBCGPMenuButton::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (m_bMenuIsActive) {
    Default();
    return;
  }

  m_bClickOnMenu = TRUE;

  if (m_bDefaultClick) {
    CRect rectClient;
    GetClientRect(rectClient);

    CRect rectArrow = rectClient;
    rectArrow.l = rectArrow.r - CBCGPMenuImages::Size().w - nImageHorzMargin;

    if (!rectArrow.PtInRect(point)) {
      m_bClickOnMenu = FALSE;
      m_nMenuResult = 0;
      CBCGPButton::OnLButtonDown(nFlags, point);
      return;
    }
  }

  SetFocus();
  OnShowMenu();
}
//****
UINT CBCGPMenuButton::OnGetDlgCode()
{
  return DLGC_WANTARROWS;
}
//****
void CBCGPMenuButton::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (m_bStayPressed && m_bMenuIsActive && m_bPushed) {
    m_bClickiedInside = FALSE;

    CButton::OnLButtonUp(nFlags, point);

    if (m_bCaptured) {
      ReleaseCapture();
      m_bCaptured = FALSE;
    }
  }
  else if (!m_bClickOnMenu) {
    CBCGPButton::OnLButtonUp(nFlags, point);
  }
}
//****
void CBCGPMenuButton::OnKillFocus(CWnd* pNewWnd)
{
  if (m_bStayPressed && m_bMenuIsActive && m_bPushed) {
    CButton::OnKillFocus(pNewWnd);

    if (m_bCaptured) {
      ReleaseCapture();
      m_bCaptured = FALSE;
    }

    m_bClickiedInside = FALSE;
    m_bHover = FALSE;
  }
  else {
    CBCGPButton::OnKillFocus(pNewWnd);
  }
}
//****
BOOL CBCGPMenuButton::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYDOWN &&
      pMsg->wParam == VK_RETURN &&
      CBCGPPopupMenu::GetActiveMenu() == NULL) {
    m_bClickOnMenu = TRUE;
    OnShowMenu();
    return TRUE;
  }

  return CBCGPButton::PreTranslateMessage(pMsg);
}
//****
void CBCGPMenuButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  if (!m_bMenuIsActive) {
    CBCGPButton::OnLButtonDblClk(nFlags, point);
  }

  if (m_bCaptured) {
    ReleaseCapture();
    m_bCaptured = FALSE;
  }
}
