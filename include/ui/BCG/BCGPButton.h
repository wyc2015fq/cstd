//

#if !defined(AFX_BCGPBUTTON_H__C159C4C6_B79B_11D3_A712_009027900694__INCLUDED_)
#define AFX_BCGPBUTTON_H__C159C4C6_B79B_11D3_A712_009027900694__INCLUDED_
// BCGPButton.h : header file


typedef const char* cp_char;

#define nImageHorzMargin  10
static const int nVertMargin = 5;
static const COLORREF clrDefault = (COLORREF) - 1;

/////////////////////////////////////////////////////////////////////////////
// CBCGPButton window

#include "button_drawer.inl"

struct CBCGPButton : public CButton, public CBCGPButtonBase, public button_drawer
{
  DECLARE_DYNCREATE(CBCGPButton)

  // Construction
public:
  CBCGPButton();

  // Attributes
public:

  BOOL      m_bHighlightChecked;
  BOOL      m_bResponseOnButtonDown;
  BOOL      m_bWasDblClk;

  DWORD GetStyle() const {
    return c->style;
  }
  BOOL IsPressed() const {
    return m_bPushed && m_bHighlighted;
  }

  BOOL IsPushed() const {
    return m_bPushed;
  }

  BOOL IsHighlighted() const {
    return m_bHighlighted;
  }
  
  BOOL IsHighlightChecked() const {
    return m_bHighlighted;
  }

  BOOL IsChecked() const {
    return m_bChecked;
  }

  BOOL IsIndeterminate() const {
    return m_bIndeterminate;
  }

  BOOL      m_bPushed;
  BOOL      m_bClickiedInside;
  BOOL      m_bHighlighted;
  BOOL      m_bHover;
  BOOL      m_bChecked;
  BOOL      m_bIndeterminate;

  // Operations
public:
  CSize SizeToContent(BOOL bCalcOnly = FALSE) {
    return button_drawer::SizeToContent(c->fo, bCalcOnly, c->text);
  }
  BOOL GetWindowText(CString& str) {
    str = c->text;
    return TRUE;
  }

  void SetFaceColor(COLORREF crFace, BOOL bRedraw = TRUE);

  void EnableFullTextTooltip(BOOL bOn = TRUE);

  // Implementation
  ~CBCGPButton();
  void CleanUp();

  void OnDrawText(CDC* pDC, const CRect& rect, const CString& strText,
      UINT uiDTFlags, UINT uiState);

  int GetImageHorzMargin() const;
  int GetVertMargin() const;

  // Generated message map functions

  //{{AFX_MSG(CBCGPButton)
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnSize(UINT nType, int cx, int cy) {}
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnPaint(CDC* pDC);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPButton

IMPLEMENT_DYNCREATE(CBCGPButton, CButton)

CBCGPButton::CBCGPButton()
{
  m_bPushed      = FALSE;
  m_bClickiedInside  = FALSE;
  m_bHighlighted    = FALSE;
  m_bHover      = FALSE;
  m_bChecked      = FALSE;
  m_bIndeterminate  = FALSE;
  m_bHighlightChecked  = TRUE;
  m_bResponseOnButtonDown = FALSE;
  m_bWasDblClk    = FALSE;
}
//
CBCGPButton::~CBCGPButton()
{
}


BEGIN_MESSAGE_MAP(CBCGPButton, CButton)
//{{AFX_MSG_MAP(CBCGPButton)
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_SIZE()
  ON_WM_KILLFOCUS()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_PAINT()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPButton message handlers
int DoDrawCheckRadioBox(CDC* pDC, CRect rectClient, UINT itemState, BOOL m_bIsLeftText, BOOL bCheckButton, BOOL bIsChecked, BOOL bIsHighlighted, BOOL bIsWindowEnabled, BOOL bIsPressed, BOOL m_b3State)
{
  // Draw check box:
  CRect rectCheck = rectClient;
  const int nCheckHeight = min(rectClient.Height() - 2, globalData.GetTextHeight() + 2);

  globalData.DrawParentBackground(pDC);

  if (m_bIsLeftText) {
    rectCheck.l = rectCheck.r - nCheckHeight;
  }
  else {
    rectCheck.r = rectCheck.l + nCheckHeight;
  }

  rectCheck.t = rectCheck.CenterPoint().y - nCheckHeight / 2;
  rectCheck.b = rectCheck.t + nCheckHeight;

  bIsHighlighted = bIsHighlighted && bIsWindowEnabled;

  if (bCheckButton) {
    int nCheck = bIsChecked ? 1 : 0;

    if (m_b3State) {
      nCheck = 2;
    }

    globalData.OnDrawCheckBoxEx(pDC, rectCheck, nCheck, bIsHighlighted, bIsPressed && bIsHighlighted, bIsWindowEnabled);
  }
  else {
    globalData.OnDrawRadioButton(pDC,
        rectCheck, bIsChecked || (bIsPressed && bIsHighlighted),
        bIsHighlighted, bIsPressed && bIsHighlighted, bIsWindowEnabled);
  }

  if (m_bIsLeftText) {
    rectClient.r = rectCheck.l;
    rectClient.l++;
  }
  else {
    rectClient.l = rectCheck.r + 1;
  }

  return TRUE;
}

//
void CBCGPButton::OnMouseMove(UINT nFlags, CPoint point)
{
  m_bHover = FALSE;

  if (nFlags & MK_LBUTTON) {
    BOOL bRedraw = FALSE;

    CRect rectClient = c->rc;

    if (rectClient.PtInRect(point)) {
      m_bHover = TRUE;

      if (!m_bHighlighted) {
        m_bHighlighted = TRUE;
        bRedraw = TRUE;
      }

      if ((nFlags & MK_LBUTTON) && !m_bPushed && m_bClickiedInside) {
        m_bPushed = TRUE;
        bRedraw = TRUE;
      }
    }
    else {
      if (nFlags & MK_LBUTTON) {
        if (m_bPushed) {
          m_bPushed = FALSE;
          bRedraw = TRUE;
        }
      }
      else if (m_bHighlighted) {
        m_bHighlighted = FALSE;
        bRedraw = TRUE;
      }

      if (!(nFlags & MK_LBUTTON)) {
        bRedraw = TRUE;
      }
    }

    if (bRedraw) {
      Invalidate(c);
      UpdateWindow(c);
    }
  }

  //CButton::OnMouseMove(nFlags, point);
}
//
void CBCGPButton::OnLButtonDown(UINT nFlags, CPoint point)
{
  //if (iPtInRect(&c->rc, point.x, point.y))
  if (m_bResponseOnButtonDown) {
    SendMessage(WM_COMMAND, BN_CLICKED, (LPARAM)this);
  }
  else {
    m_bPushed = TRUE;
    m_bClickiedInside = TRUE;
    m_bHighlighted = TRUE;

    Invalidate(c);
    UpdateWindow(c);
  }
  
  //CButton::OnLButtonDown(nFlags, point);
}
//
void CBCGPButton::OnLButtonUp(UINT nFlags, CPoint point)
{
  BOOL bClicked = m_bPushed && m_bClickiedInside && m_bHighlighted;
  int style = c->style & 0x0f;

  m_bPushed = FALSE;
  m_bClickiedInside = FALSE;
  m_bHighlighted = FALSE;

  if (bClicked) {
    if (BS_3STATE==style) {
      if (m_bIndeterminate) {
        m_bIndeterminate = FALSE;
        m_bChecked = FALSE;
      }
      else if (m_bChecked) {
        m_bIndeterminate = TRUE;
      }
      else {
        m_bChecked = TRUE;
      }
    }
    else {
      m_bChecked = !m_bChecked;
    }
  }

  if (m_bWasDblClk) {
    m_bWasDblClk = FALSE;
    SendMessage(WM_COMMAND, BN_CLICKED, (LPARAM) this);
  }

  RedrawWindow(c);
  
  //CButton::OnLButtonUp(nFlags, point);
}

//
void CBCGPButton::OnKillFocus(CWnd* pNewWnd)
{
  m_bPushed = FALSE;
  m_bClickiedInside = FALSE;
  m_bHighlighted = FALSE;
  m_bHover = FALSE;

  Invalidate(c);
  UpdateWindow(c);
}
//
void CBCGPButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  m_bPushed = TRUE;
  m_bClickiedInside = TRUE;
  m_bHighlighted = TRUE;

  Invalidate(c);
  UpdateWindow(c);

  m_bWasDblClk = TRUE;
}

//
void CBCGPButton::OnPaint(CDC* pDC)
{
  //const BOOL bIsRadio = m_bRadioButton && !m_bAutoCheck;
  DoDrawItem(pDC, c->rc, IsFocused() ? ODS_FOCUS : 0, this, c->text, c->fo);
}

#undef nImageHorzMargin

#endif // !defined(AFX_BCGPBUTTON_H__C159C4C6_B79B_11D3_A712_009027900694__INCLUDED_)
