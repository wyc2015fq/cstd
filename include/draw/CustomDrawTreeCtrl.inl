
// CCustomDrawTreeCtrl

CCustomDrawTreeCtrl::CCustomDrawTreeCtrl()
{
  m_hHotTreeItem = (NULL)
      , m_bDrawHotItem = (TRUE)
          , m_bCustomDraw = (TRUE)
              , m_nLockDrawCount = (0)
                  , m_bExplorerVisualStyle = (FALSE)
                      , m_bMouseEventsTracked = (FALSE)
                          , m_SelStyle = (CDTSS_LABELRIGHT);
}

// MSDN states: TVS_FULLROWSELECT style cannot be used in conjunction with the TVS_HASLINES style.
// http://msdn.microsoft.com/en-us/library/bb760013(VS.85).aspx
static inline BOOL IsFullRowSelectTreeCtrl(DWORD dwStyle)
{
  return (dwStyle & TVS_FULLROWSELECT) && !(dwStyle & TVS_HASLINES);
}

BOOL CCustomDrawTreeCtrl::HasValidFullRowSelectStyle()
{
  return IsFullRowSelectTreeCtrl(m_dwStyle);
}

LRESULT CCustomDrawTreeCtrl::OnStyleChanged(WPARAM wp, LPARAM lp)
{
  int nStyleType = (int) wp;
  LPSTYLESTRUCT lpStyleStruct = (LPSTYLESTRUCT) lp;
  CCustomDrawTreeCtrlBase::OnStyleChanged(nStyleType, lpStyleStruct);

  if (nStyleType & GWL_STYLE) {
    ASSERT(lpStyleStruct);
    BOOL bOldFullRowSel = IsFullRowSelectTreeCtrl(lpStyleStruct->styleOld);
    BOOL bFullRowSel  = IsFullRowSelectTreeCtrl(lpStyleStruct->styleNew);

    if (bFullRowSel ^ bOldFullRowSel) {
      SetSelStyle(bFullRowSel ? CDTSS_FULLROWSELECT : (CDTSS_FULLROWSELECT == m_SelStyle ? CDTSS_LABELRIGHT : m_SelStyle));
    }
  }

  return 0;
}

BOOL CCustomDrawTreeCtrl::OnEraseBkgnd(HDDC hDC)
{
  //return CCustomDrawTreeCtrlBase::OnEraseBkgnd(hDC);
  return TRUE;  // flicker free!
}

void CCustomDrawTreeCtrl::OnPaint()
{
  RECT rect;

  if (GetUpdateRect(&rect, FALSE) && !rect.IsRectEmpty()) {
    // double buffered drawing.
    CPaintDC hDC(this);
    CSimpleMemDC memDC(hDC, hDC.m_ps.rcPaint);
    DefWindowProc(WM_PRINT, (WPARAM)memDC.GetDC().m_hDC, (LPARAM)(PRF_CLIENT | PRF_CHECKVISIBLE));
  }
}

void CCustomDrawTreeCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMTVCUSTOMDRAW pNMTVCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);

  if (!pNMTVCD) {
    return;
  }

  switch (pNMTVCD->nmcd.dwDrawStage) {
  case CDDS_PREPAINT:
    if (IsCustomDrawEnabled()) {
      *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else {
      *pResult = CDRF_DODEFAULT;
    }

    break;

  case CDDS_ITEMPREPAINT: {
    // set the background and foreground color of the item
    // to the background,
    // so you don't see the default drawing of the text
    pNMTVCD->clrTextBk = pNMTVCD->clrText = _GetSysColor(COLOR_WINDOW);

    if (CDTSS_FULLROWSELECT == m_SelStyle) {
      HTREEITEM hItem = reinterpret_cast<HTREEITEM>(pNMTVCD->nmcd.dwItemSpec);
      HDDC hDC = (pNMTVCD->nmcd.hdc);

      RECT rect = GetFillRect(hItem);
      OnFillBackground(hDC, rect, hItem, pNMTVCD->nmcd.uItemState);
    }

    // we want to get the CDDS_ITEMPOSTPAINT notification
    *pResult = CDRF_NOTIFYPOSTPAINT;
  }
  break;

  case CDDS_ITEMPOSTPAINT: {
    HTREEITEM hItem = reinterpret_cast<HTREEITEM>(pNMTVCD->nmcd.dwItemSpec);
    HDDC hDC = (pNMTVCD->nmcd.hdc);

    // draw the item
    OnDrawTreeItem(hDC, hItem, pNMTVCD->nmcd.uItemState);
    pResult = CDRF_DODEFAULT;
    *pResult = CDRF_SKIPDEFAULT;
  }
  break;

  default:
    *pResult = CDRF_DODEFAULT;
    break;
  }
}

RECT CCustomDrawTreeCtrl::GetFillRect(HTREEITEM hItem)
{
  RECT rcText;
  GetItemRect(hItem, rcText, TRUE);

  if (CDTSS_LABEL == m_SelStyle) {
    return rcText;
  }

  RECT rcBounds;
  GetItemRect(hItem, &rcBounds, FALSE);

  SCROLLINFO si = {0};
  GetScrollInfo(SB_HORZ, &si, SIF_POS | SIF_RANGE);

  rcBounds.right = MAX(rcBounds.right, si.nMax - si.nPos);

  if (m_bExplorerVisualStyle) {
    rcBounds.left = min(rcBounds.left, -si.nPos);
  }
  else {
    rcBounds.left = rcText.left;
  }

  if (CDTSS_FULLROWSELECT == m_SelStyle) {
    return rcBounds;
  }

  ASSERT(CDTSS_LABELRIGHT == m_SelStyle);

  rcText.right = rcBounds.right;

  return rcText;
}

RECT CCustomDrawTreeCtrl::GetSelectRect(HTREEITEM hItem)
{
  //  RECT rcSel;
  //  GetItemRect(hItem, &rcSel, m_bDragSelTextOnly);
  //  return rcSel;
  return GetFillRect(hItem);
}

BOOL CCustomDrawTreeCtrl::PtInItemSelectRect(HTREEITEM hItem, POINT pt)
{
  RECT rcSelect = GetSelectRect(hItem);
  return rcSelect.PtInRect(pt);
}

//#define DRAW_TREE_ITEM_VIA_MEMDC  // no flicker already, no need to double buffer it

void CCustomDrawTreeCtrl::OnDrawTreeItem(HDDC hDC, HTREEITEM hItem, UINT nItemState)
{
  RECT rcItem;
  GetItemRect(hItem, rcItem, TRUE);

  RECT rcFill = GetFillRect(hItem);
  rcItem.right = rcFill.right;

#ifdef DRAW_TREE_ITEM_VIA_MEMDC
  CSimpleMemDC memDC(*hDC, rcItem);
  HDDC hDC = &memDC.GetDC();
#else
  CDC*& hDC = hDC;
#endif // DRAW_TREE_ITEM_VIA_MEMDC

  CFont* pFont = hDC->GetCurrentFont();
  CFont* pOldFont = hDC->SelectObject(pFont);

  OnFillBackground(hDC, rcFill, hItem, nItemState);

  //rcItem.left += 5; // makes some gap to the text.
  OnDrawText(hDC, rcItem, hItem, nItemState, FALSE);

  hDC->SelectObject(pOldFont);
}

void CCustomDrawTreeCtrl::OnFillBackground(HDDC hDC, RECT rect, HTREEITEM hItem, UINT nItemState)
{
  BOOL bIsWndEnabled = m_bIsWindowEnabled;
  // Erase the background
  TFillSolidRect(hDC, &rect, _GetSysColor(bIsWndEnabled ? COLOR_WINDOW : COLOR_BTNFACE));

  if (!bIsWndEnabled) {
    return;
  }

  //rect.left += 2; // make some gap

  CCustomDrawHelper itemDrawer;
  itemDrawer.m_bWndHasFocus   = ::GetFocus() == GetSafeHwnd();
  itemDrawer.m_bFocusItem     = (nItemState & CDIS_FOCUS) ? TRUE : FALSE;
  itemDrawer.m_bSelected      = (nItemState & CDIS_SELECTED) ? TRUE : FALSE;
  itemDrawer.m_bIsHotItem     = m_bDrawHotItem && m_hHotTreeItem == hItem;
  itemDrawer.m_bDrawBorderWhenFill  = TRUE;
  itemDrawer.DrawItemBackground(hDC, rect);
}

SIZE CCustomDrawTreeCtrl::OnDrawText(HDDC hDC, RECT rect, HTREEITEM hItem, UINT nItemState, BOOL bCalcOnly DEFAULT(FALSE))
{
  str_t strItem = GetItemText(hItem);

  COLORREF clrTxtColor;

  if (nItemState & CDIS_DISABLED) {
    clrTxtColor = _GetSysColor(COLOR_GRAYTEXT);
  }

  else if ((nItemState & CDIS_SELECTED) && ::GetFocus() == GetSafeHwnd()) {
    clrTxtColor = _GetSysColor(COLOR_HIGHLIGHTTEXT);
  }

  else {
    clrTxtColor = _GetSysColor(COLOR_WINDOWTEXT);
  }

  return DrawItemText(hDC, rect, hItem, strItem, clrTxtColor, bCalcOnly);
}

SIZE CCustomDrawTreeCtrl::DrawItemText(HDDC hDC, RECT rect, HTREEITEM hItem, LPCTSTR lpcszText, COLORREF clrTxtColor, BOOL bCalcOnly DEFAULT(FALSE))
{
  int nOldBkMode      =
      COLORREF clrOldTxtColor = clrText = (clrTxtColor);

  DRAWTEXTPARAMS dtp = {sizeof(DRAWTEXTPARAMS)};
  dtp.iTabLength = 4;

  UINT nBoldState = GetItemState(hItem, TVIS_BOLD);
  BOOL bBoldText = nBoldState & TVIS_BOLD;

  CFont* pOldFont = NULL;
  CFont fnt;

  if (bBoldText) {
    CFont* pCurFont = hDC->GetCurrentFont();
    LOGFONT lf = {0};
    pCurFont->GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;  // make it bold

    fnt.CreateFontIndirect(&lf);
    pOldFont = hDC->SelectObject(&fnt);
  }

  UINT uDTFormat = DT_VCENTER | DT_SINGLELINE | DT_TABSTOP | DT_EXPANDTABS | DT_END_ELLIPSIS;

  if (bCalcOnly) {
    uDTFormat |= DT_CALCRECT;
  }

  int nTextHeight;

  if (GetTextDrawer()) {
    nTextHeight = GetTextDrawer()->Draw(hDC, lpcszText, rect, uDTFormat);
  }
  else {
    nTextHeight = DrawTextEx(hDC, (LPTSTR)lpcszText, _tcslen(lpcszText), rect, uDTFormat, &dtp);
  }

  SIZE sizeText(rect.Width(), nTextHeight);

  if (bBoldText) {

    fnt.DeleteObject();
  }

  clrText = (clrOldTxtColor);
  hDC->SetBkMode(nOldBkMode);
  return sizeText;
}

void CCustomDrawTreeCtrl::InvalidateItem(HTREEITEM hItem, BOOL bErase DEFAULT(TRUE))
{
  RECT rcItem = GetFillRect(hItem);
  InvalidateRect(rcItem, bErase);
}

void CCustomDrawTreeCtrl::OnMouseMove(UINT nFlags, POINT point)
{
  CCustomDrawTreeCtrlBase::OnMouseMove(nFlags, point);

  UINT uHitTestFlags = 0;
  HTREEITEM hItem = HitTest(point, &uHitTestFlags);

  if (IsDrawHotItem()) {
    if (hItem) {
      RECT rcSel = GetFillRect(hItem);

      if (!rcSel.PtInRect(point)) {
        hItem = m_hHotTreeItem;
      }
    }

    if (hItem != m_hHotTreeItem) {
      if (!m_bMouseEventsTracked) {
        m_bMouseEventsTracked = TRUE;

        TRACKMOUSEEVENT trackmouseevent;
        trackmouseevent.cbSize    = sizeof(trackmouseevent);
        trackmouseevent.dwFlags   = TME_LEAVE;
        trackmouseevent.hwndTrack = GetSafeHwnd();
        trackmouseevent.dwHoverTime = HOVER_DEFAULT;
        //::AFXTrackMouse(&trackmouseevent);
        _TrackMouseEvent(&trackmouseevent);
      }

      // It seems that SetRedraw() will actually produce flickering, so better do invalidate instead.
      if (m_hHotTreeItem != NULL) {
        InvalidateItem(m_hHotTreeItem, TRUE);
      }

      if (hItem != NULL) {
        InvalidateItem(hItem, TRUE);
      }

      m_hHotTreeItem = hItem;
      UpdateWindow();
    }
  }

  // Find the item
  if (ShouldShowToolTipForCell(hItem, uHitTestFlags)
      && GetCustomDrawToolTips().CheckDisplayToolTip((WPARAM)hItem, (LPARAM)uHitTestFlags)
     ) {
    // Remove the old tooltip (if available)
    int nToolCount = GetCustomDrawToolTips().GetToolCount();

    //TRACE("CCustomDrawTreeCtrl tooltip tool count: %d\n", nToolCount);
    if (nToolCount > 0) {
      // Not using CToolTipCtrl::DelTool() because it redirects the messages to CTreeCtrl parent
      // If we call DelTool(), you will see that the return value of GetToolCount() still keep increasing!
      //GetCustomDrawToolTips().DelTool(this);
      TOOLINFO ti = {0};
      ti.cbSize = sizeof(TOOLINFO);
      ti.uFlags = TTF_IDISHWND; // Indicate that uId is handle to a control
      ti.uId = (UINT_PTR)m_hWnd;  // Handle to the control
      ti.hwnd = m_hWnd;     // Handle to window to receive the tooltip-messages
      ti.hinst = AfxGetInstanceHandle();
      GetCustomDrawToolTips().SendMessage(TTM_DELTOOL, 0, (LPARAM)(LPTOOLINFO) &ti);

      GetCustomDrawToolTips().Activate(FALSE);
    }

    // Not using CToolTipCtrl::AddTool() because it redirects the messages to CTreeCtrl parent
    //GetCustomDrawToolTips().AddTool(this);
    TOOLINFO ti = {0};
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_IDISHWND; // Indicate that uId is handle to a control
    ti.uId = (UINT_PTR)m_hWnd;  // Handle to the control
    ti.hwnd = m_hWnd;     // Handle to window to receive the tooltip-messages
    ti.hinst = AfxGetInstanceHandle();
    ti.lpszText = LPSTR_TEXTCALLBACK;
    GetCustomDrawToolTips().SendMessage(TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO) &ti);

    PreAddToolTipForItem(hItem, uHitTestFlags);
    GetCustomDrawToolTips().Activate(TRUE);
  }
}

LRESULT CCustomDrawTreeCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
  LRESULT result = Default();
  m_bMouseEventsTracked = FALSE;

  if (m_hHotTreeItem) {
    InvalidateItem(m_hHotTreeItem, FALSE);
    m_hHotTreeItem = NULL;
  }

  return result;
}

void CCustomDrawTreeCtrl::OnLButtonDown(UINT nFlags, POINT point)
{
  if (CDTSS_LABELRIGHT == m_SelStyle && !HasValidFullRowSelectStyle()) {
    UINT uHitTestFlags = 0;
    HTREEITEM hItem = HitTest(point, &uHitTestFlags);

    if (hItem != NULL && (uHitTestFlags & TVHT_ONITEMRIGHT)) {
      Select(hItem, TVGN_CARET);
    }
  }

  CCustomDrawTreeCtrlBase::OnLButtonDown(nFlags, point);
}

int CCustomDrawTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CCustomDrawTreeCtrlBase::OnCreate(lpCreateStruct) == -1) {
    return -1;
  }

  OnInitTreeCtrl();
  return 0;
}

void CCustomDrawTreeCtrl::PreSubclassWindow()
{
  CCustomDrawTreeCtrlBase::PreSubclassWindow();

  _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

  if (pThreadState->m_pWndInit == NULL) {
    OnInitTreeCtrl();
  }
}

BOOL CCustomDrawTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
  if (GetCustomDrawToolTips().m_hWnd) {
    GetCustomDrawToolTips().RelayEvent(pMsg);
  }

  return CCustomDrawTreeCtrlBase::PreTranslateMessage(pMsg);
}

void CCustomDrawTreeCtrl::OnInitTreeCtrl()
{
  // disables the automatic tooltip feature of tree view controls
  ModifyStyle(0, TVS_NOTOOLTIPS);

  // Enable our own tooltip-ctrl and make it show tooltip even if not having focus
  // Prevents the system from stripping ampersand characters from a string or terminating a string at a tab character
  VERIFY(GetCustomDrawToolTips().Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX));
  GetCustomDrawToolTips().Activate(TRUE);

  EnableExplorerVisualStyles(TRUE);

  if (HasValidFullRowSelectStyle()) {
    m_SelStyle = CDTSS_FULLROWSELECT;
  }
}

BOOL CCustomDrawTreeCtrl::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
  UINT nID = pNMHDR->idFrom;

  // check if this is the automatic tooltip of the control
  if (nID == 0) {
    return TRUE;  // do not allow display of automatic tooltip,
  }

  // or our tooltip will disappear

  *pResult = 0;

  // get the mouse position
  const MSG* pMessage = GetCurrentMessage();
  ASSERT(pMessage);
  POINT pt;
  pt = pMessage->pt;    // get the point from the message
  ScreenToClient(&pt);  // convert the point's coords to be relative to this control

  // see if the point falls onto a item
  UINT uHitTestFlags = 0;
  HTREEITEM hItem = HitTest(pt, &uHitTestFlags);

  if (hItem != NULL && (uHitTestFlags & (TVHT_ONITEM | TVHT_ONITEMRIGHT))) {
    str_t strToolTip = GetToolTipLabelForItem(hItem, uHitTestFlags);

    if (!strToolTip.IsEmpty()) {
      GetCustomDrawToolTips().SetText(pNMHDR, strToolTip);
      return TRUE;
    }

  }

  return FALSE; // we didn't handle the message, let the
  // framework continue propagating the message
}

BOOL CCustomDrawTreeCtrl::ShouldShowToolTipForCell(HTREEITEM hItem, UINT uHitTestFlags)
{
  UINT uHT = TVHT_ONITEM | (CDTSS_LABEL == m_SelStyle ? 0 : TVHT_ONITEMRIGHT);
  return hItem != NULL && (uHitTestFlags & uHT);
}

str_t CCustomDrawTreeCtrl::GetToolTipLabelForItem(HTREEITEM hItem, UINT uHitTestFlags)
{
  return GetItemText(hItem);
}

void CCustomDrawTreeCtrl::PreAddToolTipForItem(HTREEITEM hItem, UINT uHitTestFlags)
{
  // nothing to do
}

void CCustomDrawTreeCtrl::PreShowToolTipForItem(HTREEITEM hItem, UINT uHitTestFlags)
{
  // nothing to do
}

void CCustomDrawTreeCtrl::PrePopToolTip()
{
  // nothing to do
}

LRESULT CCustomDrawTreeCtrl::OnCustomToolTipNotify(WPARAM wParam, LPARAM lParam)
{
  switch (wParam) {
  case CDTOOLTIP_ONBEFORE_SHOW: {
    HTREEITEM hItem   = (HTREEITEM)GetCustomDrawToolTips().GetDisplayWParam();
    UINT uHitTestFlags  = (UINT)GetCustomDrawToolTips().GetDisplayLParam();
    PreShowToolTipForItem(hItem, uHitTestFlags);
  }
  break;

  case CDTOOLTIP_ONBEFORE_POP:
    PrePopToolTip();
    break;
  }

  return 0;
}
