
// CMultiSelTriCheckTreeCtrl


#ifndef SPI_GETFLATMENU
#define SPI_GETFLATMENU 0x1022  // Determines whether native User menus have flat menu appearance. 
// The pvParam parameter must point to a BOOL variable that returns
// TRUE if the flat menu appearance is set, or FALSE otherwise.
#endif

#ifndef COLOR_MENUHILIGHT
#define COLOR_MENUHILIGHT 29  // The color used to highlight menu items when the menu appears as
// a flat menu (see SystemParametersInfo). The highlighted menu item
// is outlined with COLOR_HIGHLIGHT.
#endif

// Don't bother why this coded like this
static COLORREF GetSelectionFillColor(BYTE byPercent, COLORREF clrHiLi)
{
  BYTE byRVal = GetRValue(clrHiLi) + byPercent * (255 - GetRValue(clrHiLi)) / 100;
  BYTE byGVal = GetGValue(clrHiLi) + byPercent * (255 - GetGValue(clrHiLi)) / 100;
  BYTE byBVal = GetBValue(clrHiLi) + byPercent * (255 - GetBValue(clrHiLi)) / 100;
  return _RGB(byRVal, byGVal, byBVal);
}

static COLORREF GetSelectionFillColor()
{
  BOOL bFlatMenu = TRUE;
  SystemParametersInfo(SPI_GETFLATMENU, 0, &bFlatMenu, 0);

  if (bFlatMenu) {
    return _GetSysColor(COLOR_MENUHILIGHT);
  }

  COLORREF clrHiLight = _GetSysColor(COLOR_HIGHLIGHT); // Color of item(s) selected in a control.
  return GetSelectionFillColor(60, clrHiLight);
}

#define HasShiftFlag()        (nFlags & MK_SHIFT)
#define HasCtrlFlag()       (nFlags & MK_CONTROL)
#define HasCtrlOrShiftFlag()    (nFlags & (MK_CONTROL|MK_SHIFT))

// InvalidateRect() & UpdateWindow()
//#define DRAW_SELECTION_ON_PAINT

#define MINIMIZE_MULTREE_UPDATE_SEL_AREA  // try to calculate the minimal update area to reduce drawing time.

CMultiSelTriCheckTreeCtrl::CMultiSelTriCheckTreeCtrl()
{
  m_rcSelBox.SetRectEmpty();
  m_bPendingDragSel = FALSE;
  m_bDuringDragSel  = FALSE;
  m_bMultiSel     = TRUE;

  m_clrSelBorder    = RGB_SELECTION_BORDER;
  m_clrSelFill    = GetSelectionFillColor();
  m_hSelectAnchor   = NULL;
  m_bEmulated     = FALSE;
  m_bAlphaBlendedSelection  = TRUE;
}


void CMultiSelTriCheckTreeCtrl::DrawAlphaBlendedSelection(HDDC hDC, RECT rcPaint)
{
  DrawAlphaBlendedSelection(hDC, rcPaint, m_rcSelBox, m_clrSelFill, m_clrSelBorder);
}

void CMultiSelTriCheckTreeCtrl::OnPaint()
{
  if (IsMultiSelectable() && m_bDuringDragSel && IsAlphaBlendedSelectionEnabled()) {
    RECT rcUpdate;

    if (GetUpdateRect(&rcUpdate, FALSE) && !rcUpdate.IsRectEmpty()) {
      CPaintDC hDC(this);
      DrawAlphaBlendedSelection(hDC, hDC.m_ps.rcPaint);
    }
  }
  else {
    CMultiSelTriCheckTreeCtrlBase::OnPaint();
  }
}

void CMultiSelTriCheckTreeCtrl::OnLButtonDown(UINT nFlags, POINT point)
{
  if (IsMultiSelectable()) {
    if (OnButtonDown(TRUE, nFlags, point)) {
      return;  // has been properly handled with this message.
    }
  }

  CMultiSelTriCheckTreeCtrlBase::OnLButtonDown(nFlags, point);
}

void CMultiSelTriCheckTreeCtrl::OnRButtonDown(UINT nFlags, POINT point)
{
  if (IsMultiSelectable()) {
    if (OnButtonDown(FALSE, nFlags, point)) {
      return;  // has been properly handled with this message.
    }
  }

  CMultiSelTriCheckTreeCtrlBase::OnRButtonDown(nFlags, point);
}

#define TVHT_CLICKABLE    (TVHT_ONITEMBUTTON|TVHT_ONITEM)

BOOL CMultiSelTriCheckTreeCtrl::OnButtonDown(BOOL bLeft, UINT nFlags, POINT point)
{
  ASSERT(IsMultiSelectable());

  if (m_bPendingDragSel) {
    // The user is trying to hold down both the two mouse buttons at the same time...
    ASSERT(!m_bLeftBtnDragSel ^ !bLeft);

    if (HasCapture()) {
      ::ReleaseCapture();
    }

    if (!m_bDuringDragSel && !HasCtrlOrShiftFlag()) {
      SelectAll(FALSE);
    }

    OnEndDragSelection();
    return FALSE;
  }

  ASSERT(!HasCapture());

  UINT uHitTestFlags = 0;
  HTREEITEM hItem = HitTest(point, &uHitTestFlags);

  if (NULL == hItem || !(uHitTestFlags & TVHT_CLICKABLE)) {
    ASSERT(m_itemsInSelBox.empty());
    // set the top left point as the anchor point.
    m_rcSelBox.left   = point.x;
    m_rcSelBox.top    = point.y;
    m_rcSelBox.right  = point.x;
    m_rcSelBox.bottom = point.y;
    m_bPendingDragSel = TRUE;
    m_bLeftBtnDragSel = bLeft;

    if (::GetFocus() != m_hWnd) {
      ::SetFocus(m_hWnd);
    }

    ::SetCapture(m_hWnd);
    return TRUE;  // We have done with this message
  }
  else {
    ASSERT(hItem);

    if (!(uHitTestFlags & (TVHT_ONITEMBUTTON | TVHT_ONITEMSTATEICON))) {
      DoPreSelection(hItem, bLeft, nFlags);
      //DoAction(hItem, bLeft, nFlags, point);
      return TRUE;
    }
  }

  return FALSE;
}

void CMultiSelTriCheckTreeCtrl::OnLButtonUp(UINT nFlags, POINT point)
{
  if (!OnButtonUp(TRUE, nFlags, point)) {
    CMultiSelTriCheckTreeCtrlBase::OnLButtonUp(nFlags, point);
  }
}

void CMultiSelTriCheckTreeCtrl::OnRButtonUp(UINT nFlags, POINT point)
{
  if (!OnButtonUp(FALSE, nFlags, point)) {
    CMultiSelTriCheckTreeCtrlBase::OnRButtonUp(nFlags, point);
  }
}

BOOL CMultiSelTriCheckTreeCtrl::OnButtonUp(BOOL bLeft, UINT nFlags, POINT point)
{
  UINT uHitTestFlags = 0;
  HTREEITEM hItem = HitTest(point, &uHitTestFlags);

  if (m_bPendingDragSel) {
    if (HasCapture()) {
      ::ReleaseCapture();
    }

    if (hItem) {
      NMTREEVIEW nmtv;
      nmtv.hdr.hwndFrom = m_hWnd;
      nmtv.hdr.idFrom = ::GetDlgCtrlID(m_hWnd);
      nmtv.itemNew.mask = TVIF_HANDLE | TVIF_PARAM;
      nmtv.itemNew.hItem = hItem;
      nmtv.itemNew.lParam = GetItemData(hItem);
      nmtv.hdr.code = m_bLeftBtnDragSel ? NM_CLICK : NM_RCLICK;
      _SendNotify(&nmtv.hdr);
    }
  }

  if (!m_bDuringDragSel && bLeft && hItem) {
    BOOL bInItemSelRect = PtInItemSelectRect(hItem, point);

    if (!HasCtrlOrShiftFlag()) {
      SelectAllIgnore(FALSE, hItem);

      //SelectItem(hItem);
      if (bInItemSelRect) {
        SetItemState(hItem, TVIS_FOCUSED | TVIS_SELECTED, TVIS_FOCUSED | TVIS_SELECTED);
        m_hSelectAnchor = hItem;
      }
    }
    else {
      if (bInItemSelRect) {
        if (HasShiftFlag()) {
          if (!(uHitTestFlags & TVHT_CLICKABLE)) {
            if (!m_hSelectAnchor) {
              m_hSelectAnchor = GetSelectedItem();  //focus
            }

            //
#ifdef _DEBUG
            str_t strAnchorText = GetItemText(m_hSelectAnchor);
            str_t strTargetText = GetItemText(hItem);
            TRACE("\t====== Selection from [%s] to [%s]\n", strAnchorText, strTargetText);
#endif // _DEBUG
            * /
            SelectRange(m_hSelectAnchor, hItem, !HasCtrlFlag());
            //SelectItem(hItem);
            SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED);  //focus changes to last clicked

            if (m_hSelectAnchor != hItem) {
              SetItemState(m_hSelectAnchor, TVIS_SELECTED, TVIS_SELECTED);
            }
          }
        }
        else {
          UINT nState = TVIS_SELECTED;

          if (HasCtrlFlag()) {
            nState ^= (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED);
          }
          else {
            SelectAllIgnore(FALSE, hItem);
          }

          HTREEITEM hSelItem = GetSelectedItem();

          SetItemState(hItem, TVIS_FOCUSED | nState, TVIS_FOCUSED | TVIS_SELECTED);
          m_hSelectAnchor = hItem;

          //          if ( hSelItem && hSelItem != hItem )
          //            SetItemState( hSelItem, TVIS_SELECTED, TVIS_SELECTED);
        }
      }
    }
  }

  if (m_bPendingDragSel) {
    OnEndDragSelection();
  }

  if (::GetFocus() != m_hWnd) {
    ::SetFocus(m_hWnd);
  }

  return TRUE;
}

void CMultiSelTriCheckTreeCtrl::UpdateAlphaBlendedSelection(RECT rcOldSelBox, RECT rcNewSelBox, RECT rcUpdateSelBox)
{
  CClientDC hDC(this);
  int nOldWidth = rcOldSelBox.Width();
  int nOldHeight  = rcOldSelBox.Height();
  int nNewWidth = rcNewSelBox.Width();
  int nNewHeight  = rcNewSelBox.Height();

  rcOldSelBox.NormalizeRect();
  rcNewSelBox.NormalizeRect();

  // If we want to minimize the dirty area, we need to do some tricky calculation
  // to make sure that only the area that is required to be redraw become dirty.

  // The simplest solution is to calculate the dirty area by combining the two rectangle.

#ifdef MINIMIZE_MULTREE_UPDATE_SEL_AREA
  BOOL bTwoUpdateRect = FALSE;
  RECT rcUpdateSelBox2;

  // If any of them is zero, updating the union area is enough.
  if (nOldWidth != 0 && nOldHeight != 0 && nNewWidth != 0 && nNewHeight != 0) {
#define IsSameSign(_nVal1, _nVal2)    ( !  ((_nVal1 >= 0) ^ (_nVal2 >= 0))  )

    // Flags to see if the selection is flipping.
    const BOOL bIsHorSameSign = IsSameSign(nOldWidth, nNewWidth);
    const BOOL bIsVerSameSign = IsSameSign(nOldHeight, nNewHeight);

    // 1: If the selection is flipping in both directions, then the two rects need to be update.
    // 2: If the selection is flipping in only one direction, then the union area is good enough.
    // 3: If the selection is not flipping, which is the most complicate case, we need to calculate
    // the dirty area.
    if (!bIsHorSameSign && !bIsVerSameSign) {
      rcUpdateSelBox = rcOldSelBox;
      rcUpdateSelBox2 = rcNewSelBox;
      bTwoUpdateRect = TRUE;
    }
    else if (bIsHorSameSign && bIsVerSameSign) {
      rcUpdateSelBox2 = rcUpdateSelBox;

      // First case: the selection extend/shrink in only one direction (horizontally/vertically)
      if (nOldWidth == nNewWidth || nOldHeight == nNewHeight) {
        if (nOldWidth != nNewWidth) {
          if (nNewWidth > 0) {
            rcUpdateSelBox.left = (nNewWidth > nOldWidth ? rcOldSelBox.right : rcNewSelBox.right) - 1;
          }
          else {
            rcUpdateSelBox.right = (nNewWidth < nOldWidth ? rcOldSelBox.left : rcNewSelBox.left) + 1;
          }
        }
        else {
          if (nNewHeight > 0) {
            rcUpdateSelBox.top = (nNewHeight > nOldHeight ? rcOldSelBox.bottom : rcNewSelBox.bottom) - 1;
          }
          else {
            rcUpdateSelBox.bottom = (nNewHeight < nOldHeight ? rcOldSelBox.top : rcNewSelBox.top) + 1;
          }
        }
      }
      else {
        // Second case: the selection extend/shrink in both directions.
        bTwoUpdateRect = TRUE;

        if (nNewWidth > 0) {
          if (nNewWidth > nOldWidth) {
            rcUpdateSelBox2.right = rcUpdateSelBox.left = rcOldSelBox.right - 1;
          }
          else {
            rcUpdateSelBox2.right = rcUpdateSelBox.left = rcNewSelBox.right - 1;
          }
        }
        else {
          if (nNewWidth < nOldWidth) {
            rcUpdateSelBox2.left = rcUpdateSelBox.right = rcOldSelBox.left + 1;
          }
          else {
            rcUpdateSelBox2.left = rcUpdateSelBox.right = rcNewSelBox.left + 1;
          }
        }

        if (nNewHeight > 0) {
          rcUpdateSelBox2.top = (nNewHeight > nOldHeight ? rcOldSelBox.bottom : rcNewSelBox.bottom) - 1;
        }
        else {
          rcUpdateSelBox2.bottom = (nNewHeight < nOldHeight ? rcOldSelBox.top : rcNewSelBox.top) + 1;
        }
      }
    }
  }

#endif // MINIMIZE_MULTREE_UPDATE_SEL_AREA

#ifdef DRAW_SELECTION_ON_PAINT
  InvalidateRect(rcUpdateSelBox);

#ifdef MINIMIZE_MULTREE_UPDATE_SEL_AREA

  if (bTwoUpdateRect) {
    InvalidateRect(rcUpdateSelBox2);
  }

#endif // MINIMIZE_MULTREE_UPDATE_SEL_AREA

  UpdateWindow();
#else
  UpdateWindow();

  DrawAlphaBlendedSelection(hDC, rcUpdateSelBox);

#ifdef MINIMIZE_MULTREE_UPDATE_SEL_AREA

  if (bTwoUpdateRect) {
    DrawAlphaBlendedSelection(hDC, rcUpdateSelBox2);
  }

#endif // MINIMIZE_MULTREE_UPDATE_SEL_AREA
#endif // DRAW_SELECTION_ON_PAINT
}

void CMultiSelTriCheckTreeCtrl::OnMouseMove(UINT nFlags, POINT point)
{
  if (IsMultiSelectable() && m_bPendingDragSel) {
    if (!HasCapture()) {
      ::SetCapture(m_hWnd);
    }

    const BOOL bFirstTimeDrag = !m_bDuringDragSel;

    POINT ptScroll;
    SCROLLINFO si = {0};
    GetScrollInfo(SB_HORZ, &si, SIF_POS);
    ptScroll.x = si.nPos;
    GetScrollInfo(SB_VERT, &si, SIF_POS);
    ptScroll.y = si.nPos;

    if (bFirstTimeDrag) {
      SIZE sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));

      if (abs(point.x - m_rcSelBox.left - ptScroll.x) <= sizeDrag.w
          && abs(point.y - m_rcSelBox.top + ptScroll.y) <= sizeDrag.h) {
        CMultiSelTriCheckTreeCtrlBase::OnMouseMove(nFlags, point);
        return;
      }

      if (!HasCtrlOrShiftFlag()) {
        SelectAll(FALSE);
      }
    }

    m_bDuringDragSel = TRUE;

    RECT rcClient;
    GetClientRect(rcClient);

    // Do the scrolling
    if (point.y < rcClient.top) {
      SendMessage(WM_VSCROLL, SB_LINEUP);
    }
    else if (point.y >= rcClient.bottom) {
      SendMessage(WM_VSCROLL, SB_LINEDOWN);
    }

    if (point.x < rcClient.left) {
      SendMessage(WM_HSCROLL, SB_LINELEFT);
    }
    else if (point.x >= rcClient.right) {
      SendMessage(WM_HSCROLL, SB_LINERIGHT);
    }

    POINT ptNewScroll;
    GetScrollInfo(SB_HORZ, &si, SIF_POS);
    ptNewScroll.x = si.nPos;
    GetScrollInfo(SB_VERT, &si, SIF_POS);
    ptNewScroll.y = si.nPos;

    if (ptScroll != ptNewScroll) {
      ptScroll -= ptNewScroll;
      UINT nItemHeight = GetItemHeight();
      int nVOffset = ptScroll.y * nItemHeight;
      m_rcSelBox.OffsetRect(ptScroll.x, nVOffset);
    }

    RECT rcOldSelBox(m_rcSelBox), rcOldSelBoxCpy(m_rcSelBox);

    CClientDC hDC(this);
    SIZE sizeEdge(1, 1);

    if (!IsAlphaBlendedSelectionEnabled()) {
      rcOldSelBox.NormalizeRect();
      hDC.DrawDragRect(rcOldSelBox, sizeEdge, NULL, sizeEdge); // remove the old one
    }

    m_rcSelBox.right  = MAX(rcClient.left, point.x);
    m_rcSelBox.bottom = MAX(rcClient.top, point.y);

    m_rcSelBox.right  = min(rcClient.right, m_rcSelBox.right);
    m_rcSelBox.bottom = min(rcClient.bottom, m_rcSelBox.bottom);

    RECT rcNewSelBox(m_rcSelBox), rcNewSelBoxCpy(m_rcSelBox);

    rcOldSelBox.NormalizeRect();
    rcNewSelBox.NormalizeRect();

    // Now we are going to redraw the "dirty" area of the selection,
    // so we need to define what a "dirty" area is.
    if (rcOldSelBox != rcNewSelBox) {
      RECT rcUpdateSelBox;
      rcUpdateSelBox.UnionRect(rcOldSelBox, rcNewSelBox);
      UpdateDragSelection(nFlags, rcNewSelBox, rcUpdateSelBox);

      if (IsAlphaBlendedSelectionEnabled()) {
        UpdateAlphaBlendedSelection(rcOldSelBoxCpy, rcNewSelBoxCpy, rcUpdateSelBox);
      }
      else {
        rcNewSelBox.NormalizeRect();
        hDC.DrawDragRect(rcNewSelBox, sizeEdge, NULL, sizeEdge);
      }
    }

    return; // we have done with this message.
  }

  CMultiSelTriCheckTreeCtrlBase::OnMouseMove(nFlags, point);
}

void CMultiSelTriCheckTreeCtrl::OnCancelMode()
{
  CMultiSelTriCheckTreeCtrlBase::OnCancelMode();

  if (m_bPendingDragSel) {
    if (HasCapture()) {
      ::ReleaseCapture();
    }

    OnEndDragSelection();
  }
}

void CMultiSelTriCheckTreeCtrl::OnEndDragSelection()
{
  if (m_bPendingDragSel) {
    //TRACE("---------CMultiSelTriCheckTreeCtrl::OnEndDragSelection\n");
    m_bPendingDragSel = m_bDuringDragSel = FALSE;

    m_rcSelBox.NormalizeRect();
    InvalidateRect(m_rcSelBox);
    UpdateWindow();

    m_rcSelBox.SetRectEmpty();

    m_itemsInSelBox.clear();
  }
}

void CMultiSelTriCheckTreeCtrl::DoPreSelection(HTREEITEM hItem, BOOL bLeft, UINT nFlags)
{
  if (::GetFocus() != m_hWnd) {
    ::SetFocus(m_hWnd);
  }

  if (bLeft) {
    //if shift key down, select immediately
    if (HasShiftFlag()) {
      if (!m_hSelectAnchor) {
        m_hSelectAnchor = GetSelectedItem();  //focus
      }

      //
#ifdef _DEBUG
      str_t strAnchorText = GetItemText(m_hSelectAnchor);
      str_t strTargetText = GetItemText(hItem);
      TRACE("\t====== Selection from [%s] to [%s]\n", strAnchorText, strTargetText);
#endif // _DEBUG
      * /
      SelectRange(m_hSelectAnchor, hItem, !HasCtrlFlag());
      //SelectItem(hItem);
      SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED);  //focus changes to last clicked

      if (m_hSelectAnchor != hItem) {
        SetItemState(m_hSelectAnchor, TVIS_SELECTED, TVIS_SELECTED);
      }
    }
    else {
      // if ctrl was down, then the selection is delayed until
      // mouse up, otherwise select the one item
      if (!HasCtrlFlag()) {
        if (!IsSelected(hItem)) {
          SelectAllIgnore(FALSE, hItem);
        }

        SetItemState(hItem, TVIS_SELECTED | TVIS_FOCUSED, TVIS_SELECTED | TVIS_FOCUSED);
      }

      //m_hSelectAnchor = NULL; //reset when a non-shift operation occurs
      m_hSelectAnchor = hItem;
    }
  }
  else {
    // right mouse
    if (HasCtrlOrShiftFlag()) {
      if (!HasShiftFlag()) {
        m_hSelectAnchor = hItem;
      }
    }
    else {
      if (!IsSelected(hItem)) {
        SelectAllIgnore(FALSE, hItem);
      }

      SetItemState(hItem, TVIS_SELECTED | TVIS_FOCUSED, TVIS_SELECTED | TVIS_FOCUSED);
    }
  }
}
//
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)    ((int)(short)LOWORD(lParam))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)    ((int)(short)HIWORD(lParam))
#endif

void CMultiSelTriCheckTreeCtrl::DoAction(HTREEITEM hItem, BOOL bLeft, UINT nFlags, POINT point)
{
  ::SetCapture(m_hWnd);
  ASSERT(::GetCapture() == m_hWnd);

  MSG msg;
  UINT nDone = 0;
  POINT pt;
  SIZE sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));

  while (!nDone && ::GetMessage(&msg, NULL, 0, 0)) {
    if (::GetCapture() != m_hWnd) {
      break;
    }

    switch (msg.message) {
    case WM_MOUSEMOVE:
      pt.x = GET_X_LPARAM(msg.lParam);
      pt.y = GET_Y_LPARAM(msg.lParam);

      if ((abs(pt.x - point.x) > sizeDrag.w)
          || ((abs(pt.y - point.y) > sizeDrag.h))) {
        nDone = 2;
      }

      //because we exit loop, button up will still be dispatched
      // which means WM_CONTEXTMENU will be sent after TVN_BEGINRDRAG
      // - this is the same behaviour as original tree
      break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
      nDone = 1;
      break;

    default:
      ::DispatchMessage(&msg);
      break;
    }
  }

  ::ReleaseCapture();
  ASSERT(::GetCapture() != m_hWnd);

  //construct tree notification info
  NMTREEVIEW nmtv;
  nmtv.hdr.hwndFrom = m_hWnd;
  nmtv.hdr.idFrom = ::GetDlgCtrlID(m_hWnd);
  nmtv.itemNew.mask = TVIF_HANDLE | TVIF_PARAM;
  nmtv.itemNew.hItem = hItem;
  nmtv.itemNew.lParam = GetItemData(hItem);
  DWORD dwStyle = m_dwStyle;

  if (nDone == 1) {
    //click
    if (!HasShiftFlag() && bLeft) {
      UINT nState = TVIS_SELECTED;

      if (HasCtrlFlag()) {
        nState ^= (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED);
      }
      else {
        SelectAllIgnore(FALSE, hItem);
      }

      SetItemState(hItem, TVIS_FOCUSED | nState, TVIS_FOCUSED | TVIS_SELECTED);
    }

    if (::GetFocus() != m_hWnd) {
      ::SetFocus(m_hWnd);
    }

    nmtv.hdr.code = bLeft ? NM_CLICK : NM_RCLICK;
    _SendNotify(&nmtv.hdr);
  }
  else if (nDone == 2) {
    //drag
    SetItemState(hItem, TVIS_FOCUSED | TVIS_SELECTED, TVIS_FOCUSED | TVIS_SELECTED);

    if (!(dwStyle & TVS_DISABLEDRAGDROP)) {
      nmtv.hdr.code = bLeft ? TVN_BEGINDRAG : TVN_BEGINRDRAG;
      nmtv.ptDrag = point;
      _SendNotify(&nmtv.hdr);
    }
  }
}

BOOL CMultiSelTriCheckTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
  if (WM_KEYDOWN == pMsg->message) {
    if (VK_ESCAPE == pMsg->wParam) {
      if (m_bPendingDragSel) {
        if (HasCapture()) {
          ::ReleaseCapture();
        }

        SelectAll(FALSE);
        OnEndDragSelection();
      }
    }
  }

  return CMultiSelTriCheckTreeCtrlBase::PreTranslateMessage(pMsg);
}

void CMultiSelTriCheckTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (!IsMultiSelectable()) {
    CMultiSelTriCheckTreeCtrlBase::OnKeyDown(nChar, nRepCnt, nFlags);
    return;
  }

  const BOOL bCtrl  = CNTRL_DOWN;
  const BOOL bShift = SHIFT_DOWN;

  BOOL bDir = FALSE;
  HTREEITEM hSel = NULL;

  switch (nChar) {
  case VK_UP:
    bDir = TRUE;

  case VK_DOWN:
    //common
    hSel = GetSelectedItem();

    if (!m_hSelectAnchor) {
      m_hSelectAnchor = hSel;
    }

    if (!bCtrl && !bShift) {
      m_hSelectAnchor = NULL; //reset
      SelectAll(FALSE);
    }

    break;
  }

  CMultiSelTriCheckTreeCtrlBase::OnKeyDown(nChar, nRepCnt, nFlags);

  if (!hSel || (!bCtrl && !bShift)) {
    return;
  }

  HTREEITEM hNext = bDir ? GetPrevVisibleItem(hSel) : GetNextVisibleItem(hSel);

  if (!hNext) {
    hNext = hSel;
  }

  if (bShift) {
    SelectRange(m_hSelectAnchor, hNext, TRUE);
  }
  else if (bCtrl) {
    SetItemState(hNext, TVIS_FOCUSED, TVIS_FOCUSED);
  }
}

//
// Operations on each item
//
// ============================================================================================================================
// Key Pressed                    |          Inside Selection Box              |            Outside Selection Box
// ============================================================================================================================
//      No                        |              Select item                   |                Deselect item
// ============================================================================================================================
//              | In itemlist     |                 NOP                        |  Toggle selection& remove item from itemlist
// Control key  | -------------------------------------------------------------------------------------------------------------
//              | Not in itemlist | Toggle selection & add item into itemlist  |                    NOP
// ============================================================================================================================
//              | In itemlist     |                 NOP                        |  Unselect item& remove item from itemlist
// Shift key    | -------------------------------------------------------------------------------------------------------------
//              | Not in itemlist | Select item & add item into itemlist       |                    NOP
// ============================================================================================================================


void CMultiSelTriCheckTreeCtrl::UpdateDragSelection(UINT nFlags, const RECT& rcNewSelBox, const RECT& rcUpdateSelBox)
{
  RECT rcItem;

  const BOOL bHasCtrlOrShiftFlag = !!HasCtrlOrShiftFlag();

  BOOL bFoundFirstItemInsideSelBoxVertically = FALSE;

  //HTREEITEM hItem = GetRootItem();
  HTREEITEM hItem = HitTest(rcUpdateSelBox.TopLeft());

  while (hItem) {
    rcItem = GetSelectRect(hItem);

    RECT rcTmpItem(rcItem);
    const BOOL bInSelBox  = !!rcTmpItem.IntersectRect(rcItem, rcNewSelBox);
    const BOOL bWasSelected = !!IsSelected(hItem);

    BOOL bUpdateSelState  = TRUE;
    BOOL bNowSelected   = bInSelBox;

    if (bHasCtrlOrShiftFlag) {
      TreeItemList::iterator itItem = std::find(m_itemsInSelBox.begin(), m_itemsInSelBox.end(), hItem);
      const BOOL bInSelList     = itItem != m_itemsInSelBox.end();

      if (bInSelList ^ bInSelBox) {
        if (HasCtrlFlag()) {
          bNowSelected = !bWasSelected;
        }

        if (bInSelBox) {
          m_itemsInSelBox.push_back(hItem);
        }
        else {
          m_itemsInSelBox.erase(itItem);
        }
      }
      else {
        bUpdateSelState = FALSE;
      }
    }

    if (bUpdateSelState && (bWasSelected ^ bNowSelected)) {
      SetItemState(hItem, bNowSelected ? TVIS_SELECTED : 0, TVIS_SELECTED);
    }

#define IsValBetween(_nVal, _nMin, _nMax)   (_nMin <= (_nVal) && (_nVal) <= _nMax)
#define IsRectVerticallyIntersect(_rc1, _rc2) ( IsValBetween(_rc1.top, _rc2.top, _rc2.bottom) \
    || IsValBetween(_rc1.bottom, _rc2.top, _rc2.bottom) \
    || IsValBetween(_rc2.top, _rc1.top, _rc1.bottom) )

    const BOOL bIsItemInsideSelBoxVertically = IsRectVerticallyIntersect(rcItem, rcUpdateSelBox);
    bFoundFirstItemInsideSelBoxVertically = bFoundFirstItemInsideSelBoxVertically || bIsItemInsideSelBoxVertically;

    if (bFoundFirstItemInsideSelBoxVertically && !bIsItemInsideSelBoxVertically) {
      break;
    }

    hItem = GetNextVisibleItem(hItem);
  }
}

size_t CMultiSelTriCheckTreeCtrl::GetSelectedCount() const
{
  size_t nCount = 0;
  HTREEITEM hItem = GetFirstSelectedItem();

  while (hItem) {
    ++nCount;
    hItem = GetNextSelectedItem(hItem);
  }

  return nCount;
}

HTREEITEM CMultiSelTriCheckTreeCtrl::GetFirstSelectedItem() const
{
  HTREEITEM hItem = GetRootItem();

  while (hItem) {
    if (IsSelected(hItem)) {
      break;
    }

    hItem = GetNextVisibleItem(hItem);
  }

  ASSERT(!hItem || IsSelected(hItem));
  return hItem;
}

HTREEITEM CMultiSelTriCheckTreeCtrl::GetNextSelectedItem(HTREEITEM hItem) const
{
  hItem = GetNextVisibleItem(hItem);

  while (hItem) {
    if (IsSelected(hItem)) {
      break;
    }

    hItem = GetNextVisibleItem(hItem);
  }

  return hItem;
}

HTREEITEM CMultiSelTriCheckTreeCtrl::GetPrevSelectedItem(HTREEITEM hItem) const
{
  hItem = GetPrevVisibleItem(hItem);

  while (hItem) {
    if (IsSelected(hItem)) {
      break;
    }

    hItem = GetPrevVisibleItem(hItem);
  }

  return hItem;
}

void CMultiSelTriCheckTreeCtrl::SelectAll(BOOL bSelect DEFAULT(TRUE))
{
  UINT nState = bSelect ? TVIS_SELECTED : 0;
  HTREEITEM hItem = GetRootItem();

  while (hItem) {
    if (!IsSelected(hItem) ^ !bSelect) {
      SetItemState(hItem, nState, TVIS_SELECTED);
    }

    hItem = GetNextVisibleItem(hItem);
  }
}

void CMultiSelTriCheckTreeCtrl::SelectRange(HTREEITEM hFirst, HTREEITEM hLast, BOOL bOnly DEFAULT(TRUE))
{
  //locate (and select) either first or last
  // (so order is arbitrary)
  HTREEITEM hItem = GetRootItem();

  while (hItem) {
    if ((hItem == hFirst) || (hItem == hLast)) {
      if (hFirst != hLast) {
        if (!IsSelected(hItem)) {
          SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
        }

        hItem = GetNextVisibleItem(hItem);
      }

      break;
    }

    if (bOnly && IsSelected(hItem)) {
      SetItemState(hItem, 0, TVIS_SELECTED);
    }

    hItem = GetNextVisibleItem(hItem);
  }

  //select rest of range
  while (hItem) {
    if (!IsSelected(hItem)) {
      SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
    }

    if ((hItem == hFirst) || (hItem == hLast)) {
      hItem = GetNextVisibleItem(hItem);
      break;
    }

    hItem = GetNextVisibleItem(hItem);
  }

  if (!bOnly) {
    return;
  }

  while (hItem) {
    if (IsSelected(hItem)) {
      SetItemState(hItem, 0, TVIS_SELECTED);
    }

    hItem = GetNextVisibleItem(hItem);
  }
}

void CMultiSelTriCheckTreeCtrl::SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore)
{
  // special case to avoid multiple notifications for
  // the same item
  UINT nState = bSelect ? TVIS_SELECTED : 0;
  HTREEITEM hItem = GetRootItem();

  while (hItem) {
    if (hItem != hIgnore) {
      if (!IsSelected(hItem) ^ !bSelect) {
        SetItemState(hItem, nState, TVIS_SELECTED);
      }
    }

    hItem = GetNextVisibleItem(hItem);
  }
}

void CMultiSelTriCheckTreeCtrl::SetMultiSelectable(BOOL bMultiSel DEFAULT(TRUE))
{
  m_bMultiSel = bMultiSel;

  if (!m_bMultiSel) {
    HTREEITEM hItem = GetSelectedItem();

    if (hItem && !IsSelected(hItem)) {
      hItem = NULL;
    }

    SelectAllIgnore(FALSE, hItem);

    if (hItem) {
      SelectItem(hItem);
    }
  }
}

void CMultiSelTriCheckTreeCtrl::EnableAlphaBlendedSelection(BOOL bEnable DEFAULT(TRUE))
{
  m_bAlphaBlendedSelection = bEnable;
  RedrawWindow();
}

void CMultiSelTriCheckTreeCtrl::RedrawSelectedItem(BOOL bErase DEFAULT(FALSE))
{
  if (IsMultiSelectable()) {
    HTREEITEM hItem = GetRootItem();

    while (hItem) {
      if (IsSelected(hItem)) {
        InvalidateItem(hItem, FALSE);
      }

      hItem = GetNextVisibleItem(hItem);
    }
  }

  CMultiSelTriCheckTreeCtrlBase::RedrawSelectedItem();
}

void CMultiSelTriCheckTreeCtrl::SetSelectedItemCheckState(TriCheckState tcs)
{
  HTREEITEM hItem = GetFirstSelectedItem();

  while (hItem) {
    SetCheckState(hItem, tcs);
    hItem = GetNextSelectedItem(hItem);
  }
}

void CMultiSelTriCheckTreeCtrl::OnClickItemStateIcon(HTREEITEM hItem)
{
  if (IsMultiSelectable() && IsSelected(hItem) && !CNTRL_DOWN) {
    TriCheckState tcs = GetCheckState(hItem);

    if (tcs != TTCS_NONE) {
      SetSelectedItemCheckState(TTCS_CHECKED == tcs ? TTCS_UNCHECKED : TTCS_CHECKED);
    }
  }
  else {
    CMultiSelTriCheckTreeCtrlBase::OnClickItemStateIcon(hItem);
  }
}

void CMultiSelTriCheckTreeCtrl::ToggleCheckSelectedItem()
{
  if (IsMultiSelectable()) {
    BOOL bHasAnyCheckedItem = FALSE;
    HTREEITEM hItem = GetFirstSelectedItem();

    while (hItem) {
      if (GetCheck(hItem)) {
        bHasAnyCheckedItem = TRUE;
        break;
      }

      hItem = GetNextSelectedItem(hItem);
    }

    SetSelectedItemCheckState(bHasAnyCheckedItem ? TTCS_UNCHECKED : TTCS_CHECKED);
  }
  else {
    CMultiSelTriCheckTreeCtrlBase::ToggleCheckSelectedItem();
  }
}

void CMultiSelTriCheckTreeCtrl::DeleteSelectedItem()
{
  if (IsMultiSelectable()) {
    // I can not think of a better solution right now, maybe this can be
    // improved later, if I have the time :)
    TreeItemList selItems;
    GetSelectedList(selItems);

    for (TreeItemList::const_iterator iter = selItems.begin(); iter != selItems.end(); ++iter) {
      DeleteItem(*iter);
    }
  }
  else {
    CMultiSelTriCheckTreeCtrlBase::DeleteSelectedItem();
  }
}

BOOL CMultiSelTriCheckTreeCtrl::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
{
  ASSERT(hItem);

  if (!IsMultiSelectable()) {
    return CMultiSelTriCheckTreeCtrlBase::SetItemState(hItem, nState, nStateMask);
  }

  HTREEITEM hFocus = GetSelectedItem();   //current focus
  BOOL bWasFocus = (hFocus == hItem);
  BOOL bFocusWasSel = hFocus && IsSelected(hFocus); //selection state of current focus
  BOOL bWasSel = IsSelected(hItem);   //select state of acting item

  UINT nS = nState & ~TVIS_FOCUSED;
  UINT nSM = nStateMask & ~TVIS_FOCUSED;

  if (nStateMask & TVIS_FOCUSED) {
    //wanted to affect focus
    if (nState & TVIS_FOCUSED) {
      //wanted to set focus
      if (!bWasFocus && bFocusWasSel) {
        //because SelectItem would de-select the current 'real' selection
        // (the one with focus), need to make the tree ctrl think there is
        // no 'real' selection but still keep the the old item selected
        //it has to be done before the SelectItem call because
        // otherwise the TVN_SELCHANGING/ED notification handlers
        // wouldn't be able to get the proper list of selected items
        CMultiSelTriCheckTreeCtrlBase::SelectItem(NULL);  //will cause notify, but can be taken as focus change
        CMultiSelTriCheckTreeCtrlBase::SetItemState(hFocus, TVIS_SELECTED, TVIS_SELECTED);
        UpdateWindow();
      }

      if (!CMultiSelTriCheckTreeCtrlBase::SelectItem(hItem)) { //set focus (will consequently select, if not already focused)
        return FALSE;
      }

      if (nStateMask & TVIS_SELECTED) {
        //wanted to affect select state
        if (nState & TVIS_SELECTED) {
          //wanted to select, already done if wasn't focused
          if (!bWasFocus || bFocusWasSel) {
            nS &= ~TVIS_SELECTED;
            nSM &= ~TVIS_SELECTED;
          }
        }

        //else wanted to clear, base call will do that
      }
      else {
        //didn't want to affect select state
        if (!bWasSel) {
          //it wasn't previously selected, let base clear (correct)
          nS &= ~TVIS_SELECTED;
          nSM |= TVIS_SELECTED;
        }

        //else was already selected, no harm done
      }
    }
    else {
      //wanted to clear focus
      if (bWasFocus) {
        //it had the focus
        CMultiSelTriCheckTreeCtrlBase::SelectItem(NULL);  //clear focus

        if (!(nStateMask & TVIS_SELECTED)) {
          //didn't want to affect select state
          if (bWasSel) {
            //it was selected, so restore
            ASSERT(!(nS & TVIS_SELECTED));
            ASSERT(!(nSM & TVIS_SELECTED));
            //set state here, to avoid double-notify
            CMultiSelTriCheckTreeCtrlBase::SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
            //let base do other states
          }
        }
        else if (nState & TVIS_SELECTED) {
          //wanted to select (but clear focus)
          if (bWasSel) {
            //if was selected, restore
            CMultiSelTriCheckTreeCtrlBase::SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
          }

          //don't want to notify, default did it
          nS &= ~TVIS_SELECTED;
          nSM &= ~TVIS_SELECTED;
        }
      }
    }
  }

  if (!nSM) {
    return TRUE;  //no other states to alter
  }

  if (nSM & TVIS_SELECTED) {
    //still need to alter selection state
    NMTREEVIEW nmtv;
    nmtv.hdr.hwndFrom = m_hWnd;
    nmtv.hdr.idFrom = ::GetDlgCtrlID(m_hWnd);
    nmtv.hdr.code = TVN_SELCHANGING;
    nmtv.itemOld.mask = nmtv.itemNew.mask = 0;
    nmtv.itemOld.hItem = nmtv.itemNew.hItem = NULL;
    TVITEM& item = (nS & TVIS_SELECTED) ? nmtv.itemNew : nmtv.itemOld;
    item.mask = TVIF_HANDLE | TVIF_PARAM;
    item.hItem = hItem;
    item.lParam = GetItemData(hItem);

    if (_SendNotify(&nmtv.hdr)) {
      return FALSE;  //sel-changing stopped
    }

    VERIFY(CMultiSelTriCheckTreeCtrlBase::SetItemState(hItem, nS, nSM));
    nmtv.hdr.code = TVN_SELCHANGED;
    _SendNotify(&nmtv.hdr);
    nS &= ~TVIS_SELECTED;
    nSM &= ~TVIS_SELECTED;
  }

  if (!nSM) {
    return TRUE;
  }

  return CMultiSelTriCheckTreeCtrlBase::SetItemState(hItem, nS, nSM);
}

UINT CMultiSelTriCheckTreeCtrl::GetItemState(HTREEITEM hItem, UINT nStateMask) const
{
  UINT nState = CMultiSelTriCheckTreeCtrlBase::GetItemState(hItem, nStateMask & ~TVIS_FOCUSED);

  if (nStateMask & TVIS_FOCUSED)
    if (GetSelectedItem() == hItem) {
      nState |= TVIS_FOCUSED;
    }

  return nState;
}


// _SendNotify
// - helper to distinguish between default control generated notifications
//   and this classes emulated ones (so can tell if focus or select notify)
BOOL CMultiSelTriCheckTreeCtrl::_SendNotify(LPNMHDR pNMHDR)
{
  ASSERT(::GetParent(m_hWnd));  //never expected this

  BOOL b = m_bEmulated;
  m_bEmulated = TRUE;
  BOOL bRes = ::SendMessage(::GetParent(m_hWnd), WM_NOTIFY, (WPARAM)pNMHDR->idFrom, (LPARAM)pNMHDR);
  m_bEmulated = b;
  return bRes;
}
