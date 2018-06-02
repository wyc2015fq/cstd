// struct CTriCheckStateTreeCtrl
#ifndef STATEIMAGEMASKTOINDEX
#define STATEIMAGEMASKTOINDEX(i) ((i) >> 12)
#endif // STATEIMAGEMASKTOINDEX
#define TVICON_W 13
#define TVICON_H 13
CTriCheckStateTreeCtrl::CTriCheckStateTreeCtrl()
  : m_uClickHitTestFlags(0)
{
}
CTriCheckStateTreeCtrl::~CTriCheckStateTreeCtrl()
{
}
void CTriCheckStateTreeCtrl::OnInitTreeCtrl()
{
  CTriCheckStateTreeCtrlBase::OnInitTreeCtrl();
  if (GetSafeHwnd()) {
    VERIFY(InitCheckBoxImage());
    SetImageList(&m_ChkBoxImgList, TVSIL_STATE);
  }
}
//#define CUSTOMDRAW_CHECKBOX
#define TFrameRect(hDC, rc, clr) DrawRect(hDC, rc, 0, clr)
BOOL CTriCheckStateTreeCtrl::InitCheckBoxImage(HDDC hDC)
{
  int cx = TVICON_W; //GetSystemMetrics(SM_CXICON);
  int cy = TVICON_H; //GetSystemMetrics(SM_CYICON);
  RECT rect(0, 0, cx, cy);
  {
    RECT rect(0, 0, cx, cy);
    COLORREF brBorder(_RGB(28, 81, 128));
    for (int ii = TTCS_NONE; ii <= TTCS_CHECKED; ++ii) {
      // Fill the background first
      FillGradient(hDC.GetSafeHdc(), rect, _RGB(223, 222, 215), _RGB(255, 255, 255), GFT_DIAG_TL_BR);
      // Draw the border
      TFrameRect(hDC, rect, brBorder);
      rect.OffsetRect(cx, 0);
    }
    // Draw the icon for partial checked button
    rect.left = cx * TTCS_PARTIALCHECKED;
    rect.right = rect.left + cx;
    RECT rectPartial(rect);
    DeflateRect(&rectPartial, 3, 3);
    TFillSolidRect(hDC, rectPartial, _RGB(115, 193, 114));
    // Draw the icon for normal checked button
    rect.left = cx * TTCS_CHECKED;
    rect.right = rect.left + cx;
    POINT ptsTick[3] = {
      {rect.left + 3, rect.top + 6},
      {rect.left + 5, rect.top + 8},
      {rect.left + 9, rect.top + 4}
    };
    // Draw the tick
    COLORREF penTick(PS_SOLID, 3, _RGB(33, 161, 33));
    COLORREF* pOldPen = hDC.SelectObject(&penTick);
    hDC.Polyline(ptsTick, 3);
  }
  // Finish the bitmap
  bitmapDrawer.EndDraw();
  m_ChkBoxImgList.Create(cx, cy, ILC_COLOR24, 0, 1);
  int nBmpIndex = m_ChkBoxImgList.Add(bitmapDrawer.GetBitmapPtr(), (img_t*)NULL);
  ASSERT(nBmpIndex >= 0);
#ifdef _DEBUG
  int nImgCount = m_ChkBoxImgList.GetImageCount();
  ASSERT(nImgCount > 0);
#endif // _DEBUG
  return TRUE;
}
// CTriCheckStateTreeCtrl message handlers
void CTriCheckStateTreeCtrl::OnClickItem(NMHDR* pNMHDR, LRESULT* pResult)
{
  if (m_uClickHitTestFlags & TVHT_ONITEMSTATEICON) {
    *pResult = 1; // Mark message as handled and suppress default handling,
  }
  // otherwise the original tree control would modify the check state.
  else {
    *pResult = 0;
  }
}
void CTriCheckStateTreeCtrl::OnLButtonDown(UINT nFlags, POINT point)
{
  HTREEITEM hItem = HitTest(point, &m_uClickHitTestFlags);
  if (hItem != NULL && (m_uClickHitTestFlags & TVHT_ONITEMSTATEICON)) {
    OnClickItemStateIcon(hItem);
  }
  CTriCheckStateTreeCtrlBase::OnLButtonDown(nFlags, point);
}
void CTriCheckStateTreeCtrl::OnLButtonDblClk(UINT nFlags, POINT point)
{
  HTREEITEM hItem = HitTest(point, &m_uClickHitTestFlags);
  if (hItem != NULL && (m_uClickHitTestFlags & (TVHT_ONITEMRIGHT | TVHT_ONITEMLABEL))) {
    OnDblClkItem(hItem);
  }
  CTriCheckStateTreeCtrlBase::OnLButtonDblClk(nFlags, point);
}
void CTriCheckStateTreeCtrl::OnClickItemStateIcon(HTREEITEM hItem)
{
  TriCheckState tcs = GetCheckState(hItem);
  if (tcs != TTCS_NONE) {
    ToggleCheck(hItem);
  }
}
void CTriCheckStateTreeCtrl::ToggleCheckSelectedItem()
{
  HTREEITEM hItem = GetSelectedItem();
  if (hItem != NULL) {
    TriCheckState tcs = GetCheckState(hItem);
    if (tcs != TTCS_NONE) {
      ToggleCheck(hItem);
    }
  }
}
void CTriCheckStateTreeCtrl::DeleteSelectedItem()
{
  HTREEITEM hItem = GetSelectedItem();
  if (hItem != NULL) {
    DeleteItem(hItem);
  }
}
void CTriCheckStateTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (VK_SPACE == nChar) {
    ToggleCheckSelectedItem();
  }
  else if (VK_DELETE == nChar) {
    DeleteSelectedItem();
  }
  else {
    CTriCheckStateTreeCtrlBase::OnKeyDown(nChar, nRepCnt, nFlags);
  }
}
void CTriCheckStateTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
  TV_KEYDOWN* pTVKeyDown = reinterpret_cast<TV_KEYDOWN*>(pNMHDR);
  *pResult = 0;
}
BOOL CTriCheckStateTreeCtrl::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearchChild DEFAULT(TRUE))
{
  if ((nStateMask & TVIS_STATEIMAGEMASK) && !CanCheckItem(hItem)) {
    return FALSE;
  }
  BOOL bReturn = CTriCheckStateTreeCtrlBase::SetItemState(hItem, nState, nStateMask);
  if (nStateMask & TVIS_STATEIMAGEMASK) {
    TriCheckState iState = (TriCheckState)STATEIMAGEMASKTOINDEX(nState);
    if (iState != TTCS_NONE) {
      if (bSearchChild) {
        TravelCheckChildren(hItem, iState);
      }
      TravelCheckSiblingAndParent(hItem, iState);
    }
  }
  return bReturn;
}
BOOL CTriCheckStateTreeCtrl::SetCheckState(HTREEITEM hItem, TriCheckState tcs DEFAULT(TTCS_CHECKED))
{
  return SetItemState(hItem, INDEXTOSTATEIMAGEMASK(tcs), TVIS_STATEIMAGEMASK);
}
TriCheckState CTriCheckStateTreeCtrl::GetCheckState(HTREEITEM hItem) const
{
  return (TriCheckState)STATEIMAGEMASKTOINDEX(GetItemState(hItem, TVIS_STATEIMAGEMASK));
}
BOOL CTriCheckStateTreeCtrl::SetCheck(HTREEITEM hItem, BOOL bCheck DEFAULT(TRUE))
{
  return SetCheckState(hItem, bCheck ? TTCS_CHECKED : TTCS_UNCHECKED);
}
BOOL CTriCheckStateTreeCtrl::GetCheck(HTREEITEM hItem) const
{
  TriCheckState tcs = GetCheckState(hItem);
  return TTCS_CHECKED == tcs || TTCS_PARTIALCHECKED == tcs;
}
BOOL CTriCheckStateTreeCtrl::DeleteItem(HTREEITEM hItem)
{
  HTREEITEM hParentItem = GetParentItem(hItem);
  BOOL bRet = CTriCheckStateTreeCtrlBase::DeleteItem(hItem);
  if (GetDeleteParentIfLastChild()) {
    while (hParentItem != NULL) {
      HTREEITEM hSiblingItem = GetChildItem(hParentItem);
      if (NULL == hSiblingItem) {
        // The parent item has no child left
        HTREEITEM hTmpParentItem = hParentItem;
        hParentItem = GetParentItem(hParentItem);
        CTriCheckStateTreeCtrlBase::DeleteItem(hTmpParentItem);
      }
      else {
        break;
      }
    }
  }
  if (hParentItem != NULL) {
    TriCheckState tcs = GetCheckState(hParentItem);
    if (TTCS_PARTIALCHECKED == tcs) {
      HTREEITEM hChildItem = GetChildItem(hParentItem);
      ASSERT(hChildItem != NULL); // it must has been deleted by the above code!
      tcs = GetCheckState(hChildItem);
      TravelCheckSiblingAndParent(hChildItem, tcs);
    }
  }
  return bRet;
}
void CTriCheckStateTreeCtrl::TravelCheckChildren(HTREEITEM hItem, TriCheckState nState)
{
  HTREEITEM hChildItem = GetChildItem(hItem);
  while (hChildItem != NULL) {
    TriCheckState nChildState = GetCheckState(hChildItem);
    if (nChildState != TTCS_NONE && CanCheckItem(hChildItem)) {
      CTriCheckStateTreeCtrlBase::SetItemState(hChildItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
    }
    // recursively check its children
    TravelCheckChildren(hChildItem, nState);
    hChildItem = GetNextSiblingItem(hChildItem);
  }
}
void CTriCheckStateTreeCtrl::TravelCheckSiblingAndParent(HTREEITEM hItem, TriCheckState nState)
{
  HTREEITEM hParentItem = GetParentItem(hItem);
  if (NULL == hParentItem) {
    return;
  }
  // Check the status of all the sibling node
  HTREEITEM hSiblingItem = GetChildItem(hParentItem);
  while (hSiblingItem != NULL) {
    TriCheckState nSiblingState = GetCheckState(hSiblingItem);
    // If any of them is different with hItem, we have to do partial check for all of its parents.
    if (nSiblingState != nState && nSiblingState != TTCS_NONE) {
      while (hParentItem != NULL) {
        TriCheckState nParentState = GetCheckState(hParentItem);
        if (nParentState != TTCS_NONE && CanCheckItem(hParentItem)) {
          CTriCheckStateTreeCtrlBase::SetItemState(hParentItem, INDEXTOSTATEIMAGEMASK(TTCS_PARTIALCHECKED), TVIS_STATEIMAGEMASK);
        }
        hParentItem = GetParentItem(hParentItem);
      }
      return; // Done.
    }
    hSiblingItem = GetNextSiblingItem(hSiblingItem);
  }
  // If we come to here, that means we still need to check the parent's sibling
  TriCheckState nParentState = GetCheckState(hParentItem);
  if (nParentState != TTCS_NONE && CanCheckItem(hParentItem)) {
    CTriCheckStateTreeCtrlBase::SetItemState(hParentItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
  }
  TravelCheckSiblingAndParent(hParentItem, nState);
}
void CTriCheckStateTreeCtrl::OnDblClkItem(HTREEITEM hItem)
{
  TRACE("CTriCheckStateTreeCtrl::OnDblClkItem\n");
}
BOOL CTriCheckStateTreeCtrl::CanCheckItem(HTREEITEM hItem)
{
  return TRUE;
}

