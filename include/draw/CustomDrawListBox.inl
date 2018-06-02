
// CCustomDrawListBox


CCustomDrawListBox::CCustomDrawListBox()
{

  m_pImageList = (NULL);
  m_nHotItem = (-1);
  m_bMouseEventsTracked = (FALSE);
  m_bExplorerVisualStyle = (FALSE);
  m_nLockDrawCount = (0);
}

void CCustomDrawListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  if ((int)lpDrawItemStruct->itemID < 0) {
    return;
  }

  HDDC hDC = (lpDrawItemStruct->hDC);
  int nSavedDC = hDC->SaveDC();

  RECT rectClient;
  GetClientRect(rectClient);

  // * Somehow lpDrawItemStruct->rcItem gives wrong value for selected item when they are scrolled outside the listbox
  // and SelectClipRgn not works in this case, so I have to use memory hDC to do "clipping"
  RECT rectItem(lpDrawItemStruct->rcItem);

  CSimpleMemDC memDC(*hDC, rectItem);
  HDDC hDC = &memDC.GetDC();

  CFont* pFont = hDC->GetCurrentFont();
  CFont* pOldFont = hDC->SelectObject(pFont);

  OnFillBackground(hDC, rectItem, lpDrawItemStruct);

  if (OnDrawIcon(hDC, rectItem, lpDrawItemStruct)) {
    int cx, cy;
    ImageList_GetIconSize(m_ImageList, &cx, &cy);
    rectItem.left += cx + 4;
  }

  rectItem.left += 2; // some gap to the text
  OnDrawText(hDC, rectItem, lpDrawItemStruct);

  hDC->SelectObject(pOldFont);

  hDC->RestoreDC(nSavedDC);
}

void CCustomDrawListBox::OnFillBackground(HDDC hDC, RECT rect, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  BOOL bIsWndEnabled = m_bIsWindowEnabled;
  TFillSolidRect(hDC, &rect, _GetSysColor(bIsWndEnabled ? COLOR_WINDOW : COLOR_BTNFACE));

  if (!bIsWndEnabled) {
    return;
  }

  BOOL bIsComboListBox = IsComboboxList();

  CCustomDrawHelper itemDrawer;
  itemDrawer.m_bWndHasFocus   = bIsComboListBox || ::GetFocus() == GetSafeHwnd();
  itemDrawer.m_bIsHotItem     = ShouldDrawHotItem() && m_nHotItem == lpDrawItemStruct->itemID;
  itemDrawer.m_bFocusItem     = (lpDrawItemStruct->itemState & ODS_FOCUS) ? TRUE : FALSE;
  itemDrawer.m_bSelected      = (lpDrawItemStruct->itemState & ODS_SELECTED) ? TRUE : FALSE;
  itemDrawer.DrawItemBackground(hDC, rect);
}

BOOL CCustomDrawListBox::OnDrawIcon(HDDC hDC, RECT rect, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  if (m_ImageList) { // not thread safe
    int nImageIndex = GetItemIconIndex(lpDrawItemStruct->itemID);

    if (nImageIndex >= 0) {
      int nImgCount = m_ImageList->GetImageCount();
      ASSERT(nImageIndex < nImgCount);

      POINT ptIcon = rect.TopLeft();
      //m_ImageList->Draw(hDC, nImageIndex, ptIcon, ILD_TRANSPARENT);

      int cx, cy;
      ImageList_GetIconSize(m_ImageList, &cx, &cy);

      // vertical center the icon
      int ny = (rect.Height() - cy) / 2;
      ptIcon.Offset(4, ny);

      HICON hIcon = m_ImageList->ExtractIcon(nImageIndex);
      VERIFY(::DrawIconEx(hDC, ptIcon.x, ptIcon.y, hIcon, cx, cy, 0, NULL, DI_NORMAL));
      ::DestroyIcon(hIcon);
      return TRUE;
    }
  }

  return FALSE;
}

void CCustomDrawListBox::OnDrawText(HDDC hDC, RECT rect, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  str_t strText = GetItemText(lpDrawItemStruct->itemID);
  int nOldBkMode =

      COLORREF clrTxtColor = GetTextColor(lpDrawItemStruct);
  COLORREF clrOldTxtColor;

  if (clrTxtColor != CLR_INVALID) {
    clrOldTxtColor = clrText = (clrTxtColor);
  }

  const UINT nDTFormat = DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX;

  if (GetTextDrawer()) {
    GetTextDrawer()->Draw(hDC, strText, rect, nDTFormat);
  }
  else {
    hDC->DrawText(strText, strText.GetLength(), rect, nDTFormat);
  }

  if (clrTxtColor != CLR_INVALID) {
    clrText = (clrOldTxtColor);
  }

  hDC->SetBkMode(nOldBkMode);
}

COLORREF CCustomDrawListBox::GetTextColor(LPDRAWITEMSTRUCT lpDIS)
{
  // Gray text does not looks good
#ifdef CUSTOMDRAW_GRADIENT
  if (!m_bIsWindowEnabled) {
    return _GetSysColor(COLOR_GRAYTEXT);
  }

#else

  if ((lpDIS->itemState & ODS_SELECTED) && ::GetFocus() == GetSafeHwnd()) {
    return _GetSysColor(COLOR_HIGHLIGHTTEXT);
  }

#endif // CUSTOMDRAW_GRADIENT
  return CLR_INVALID;
}

str_t CCustomDrawListBox::GetItemText(UINT nItem)
{
  str_t str;
  GetText(nItem, str);
  return str;
}

int CCustomDrawListBox::GetItemIconIndex(UINT nItem)
{
  return -1;
}

int CCustomDrawListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CCustomDrawListBoxBase::OnCreate(lpCreateStruct) == -1) {
    return -1;
  }

  OnInitListBox();
  return 0;
}

void CCustomDrawListBox::PreSubclassWindow()
{
  CCustomDrawListBoxBase::PreSubclassWindow();

  _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

  if (pThreadState->m_pWndInit == NULL) {
    OnInitListBox();
  }
}

BOOL CCustomDrawListBox::PreTranslateMessage(MSG* pMsg)
{
  if (GetCustomDrawToolTips().m_hWnd) {
    GetCustomDrawToolTips().RelayEvent(pMsg);
  }

  return CCustomDrawListBoxBase::PreTranslateMessage(pMsg);
}

BOOL CCustomDrawListBox::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
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

  // see if the point falls onto a list item
  BOOL bOutSide;
  UINT nItem = ItemFromPoint(pt, bOutSide);

  if (!bOutSide && ShouldShowToolTipForItem(nItem)) {
    str_t strToolTip = GetToolTipLabelForItem(nItem);

    if (strToolTip.IsEmpty()) {
      strToolTip = GetItemText(nItem);
    }

    if (!strToolTip.IsEmpty()) {
      GetCustomDrawToolTips().SetText(pNMHDR, strToolTip);
      return TRUE;
    }
  }

  return FALSE; // we didn't handle the message, let the
  // framework continue propagating the message
}

void CCustomDrawListBox::OnMouseMove(UINT nFlags, POINT point)
{
  CCustomDrawListBoxBase::OnMouseMove(nFlags, point);

  if (ShouldDrawHotItem() && !m_bMouseEventsTracked) {
    m_bMouseEventsTracked = TRUE;

    TRACKMOUSEEVENT trackmouseevent;
    trackmouseevent.cbSize    = sizeof(trackmouseevent);
    trackmouseevent.dwFlags   = TME_LEAVE;
    trackmouseevent.hwndTrack = GetSafeHwnd();
    trackmouseevent.dwHoverTime = HOVER_DEFAULT;
    //::AFXTrackMouse(&trackmouseevent);
    _TrackMouseEvent(&trackmouseevent);
  }

  // Find the subitem
  BOOL bOutSide;
  UINT nItem = ItemFromPoint(point, bOutSide);

  if (bOutSide) {
    nItem = -1;
  }

  if (ShouldDrawHotItem() && m_nHotItem != nItem) {
    int nOldHotItem = m_nHotItem;
    m_nHotItem = nItem;

    //LockSetRedraw(FALSE);

    RECT rect;
    GetItemRect(m_nHotItem, rect);
    InvalidateRect(rect, FALSE);  // don't erase background, less flicker

    GetItemRect(nOldHotItem, rect);
    InvalidateRect(rect, FALSE);  // don't erase background, less flicker
    //LockSetRedraw(TRUE);
    UpdateWindow();
  }

  if (ShouldShowToolTipForItem(nItem) && GetCustomDrawToolTips().CheckDisplayToolTip((WPARAM)nItem)) {
    // Remove the old tooltip (if available)
    int nToolCount = GetCustomDrawToolTips().GetToolCount();

    //TRACE("CCustomDrawListBox tooltip tool count: %d\n", nToolCount);
    if (nToolCount > 0) {
      // Not using CToolTipCtrl::DelTool() because it redirects the messages to CListBox parent
      // If we call DelTool(), you will see that the return value of GetToolCount() still keep increasing!
      //GetCustomDrawToolTips().DelTool(this);
      TOOLINFO ti = {0};
      ti.cbSize = sizeof(TOOLINFO);
      ti.uFlags = TTF_IDISHWND;   // Indicate that uId is handle to a control
      ti.uId = (UINT_PTR)m_hWnd;    // Handle to the control
      ti.hwnd = m_hWnd;       // Handle to window to receive the tooltip-messages
      ti.hinst = AfxGetInstanceHandle();
      GetCustomDrawToolTips().SendMessage(TTM_DELTOOL, 0, (LPARAM)(LPTOOLINFO) &ti);

      GetCustomDrawToolTips().Activate(FALSE);
    }

    // Add the new tooltip (if available)
    if (!bOutSide) {
      // Not using CToolTipCtrl::AddTool() because it redirects the messages to CListBox parent
      //GetCustomDrawToolTips().AddTool(this);
      TOOLINFO ti = {0};
      ti.cbSize = sizeof(TOOLINFO);
      ti.uFlags = TTF_IDISHWND;   // Indicate that uId is handle to a control
      ti.uId = (UINT_PTR)m_hWnd;    // Handle to the control
      ti.hwnd = m_hWnd;       // Handle to window to receive the tooltip-messages
      ti.hinst = AfxGetInstanceHandle();
      ti.lpszText = LPSTR_TEXTCALLBACK;
      GetCustomDrawToolTips().SendMessage(TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO) &ti);

      PreAddToolTipForItem(nItem);

      GetCustomDrawToolTips().Activate(TRUE);
    }
  }
}

void CCustomDrawListBox::OnInitListBox()
{
  // Prevents the system from stripping ampersand characters from a string or terminating a string at a tab character
  VERIFY(GetCustomDrawToolTips().Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX));
  GetCustomDrawToolTips().Activate(TRUE);

  EnableExplorerVisualStyles();
}

BOOL CCustomDrawListBox::ShouldShowToolTipForItem(int nItem)
{
  return TRUE;
}

str_t CCustomDrawListBox::GetToolTipLabelForItem(int nItem)
{
  return GetItemText(nItem);
}

LRESULT CCustomDrawListBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
  LRESULT result = Default();
  m_bMouseEventsTracked = FALSE;

  if (m_nHotItem >= 0) {
    RECT rect;
    GetItemRect(m_nHotItem, rect);
    m_nHotItem = -1;
    InvalidateRect(rect, FALSE);
  }

  return result;
}

LRESULT CCustomDrawListBox::OnCustomToolTipNotify(WPARAM wParam, LPARAM lParam)
{
  switch (wParam) {
  case CDTOOLTIP_ONBEFORE_SHOW: {
    int nItem = (int)GetCustomDrawToolTips().GetDisplayWParam();
    PreShowToolTipForItem(nItem);
  }
  break;

  case CDTOOLTIP_ONBEFORE_POP:
    PrePopToolTip();
    break;
  }

  return 0;
}
