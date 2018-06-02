
int UIHorizontalLayout_SetPos( CWindowWnd* ww, CControlUI* ctr, RECT rc ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  SIZE szRemaining, sz, szAvailable;
  int it1, it2, cxExpand, iPosX, cxFixedRemaining, iAdjustable;
  int nEstimateNum, cxFixed, nAdjustables;
  CControlUI_SetPos( ww, ctr, rc );
  rc = ctr->m_rcItem;
  // Adjust for inset
  rc.left += ctn->m_rcInset.left;
  rc.top += ctn->m_rcInset.top;
  rc.right -= ctn->m_rcInset.right;
  rc.bottom -= ctn->m_rcInset.bottom;
  if ( PtrArray_GetSize(ctr->m_items) == 0 ) {
    CContainerUI_ProcessScrollbar( ww, ctr, rc, 0, 0 );
    return 0;
  }
  if ( ctn->m_pVerticalScrollbar && IsVisible( ctn->m_pVerticalScrollbar) )
    rc.right -= CControlUI_GetFixed(ctn->m_pVerticalScrollbar).cx;
  if ( ctn->m_pHorizontalScrollbar &&  IsVisible(ctn->m_pHorizontalScrollbar) )
    rc.bottom -= CControlUI_GetFixed(ctn->m_pHorizontalScrollbar).cy;
  // Determine the width of elements that are sizeable
  szAvailable = cSize(rc.right - rc.left, rc.bottom - rc.top );
  if ( ctn->m_pHorizontalScrollbar &&  IsVisible(ctn->m_pHorizontalScrollbar) )
    szAvailable.cx += UIScrollbar_GetScrollRange(ctn->m_pHorizontalScrollbar);
  nAdjustables = 0;
  cxFixed = 0;
  nEstimateNum = 0;
  for ( it1 = 0; it1 < PtrArray_GetSize(ctr->m_items); it1++ ) {
    CControlUI* pControl = (CControlUI*)PtrArray_GetAt( ctr->m_items, it1 );
    if ( ! IsVisible(pControl) )
      continue;
    //if ( pControl->IsFloat() )  continue;
    sz = EstimateSize( ww, pControl, szAvailable );
    if ( sz.cx == 0 ) {
      nAdjustables++;
    } else {
      if ( sz.cx < GetMinWidth(pControl) )
        sz.cx = GetMinWidth(pControl);
      if ( sz.cx > GetMaxWidth(pControl) )
        sz.cx = GetMaxWidth(pControl);
    }
    cxFixed += sz.cx + GetPadding(pControl).left + GetPadding(pControl).right;
    nEstimateNum++;
  }
  cxFixed += ( nEstimateNum - 1 ) * ctn->m_iChildPadding;
  cxExpand = 0;
  if ( nAdjustables > 0 )
    cxExpand = MAX( 0, ( szAvailable.cx - cxFixed ) / nAdjustables );
  // Position the elements
  szRemaining = szAvailable;
  iPosX = rc.left;
  if ( ctn->m_pHorizontalScrollbar &&  IsVisible(ctn->m_pHorizontalScrollbar) ) {
    iPosX -=  UIScrollbar_GetScrollPos(ctn->m_pHorizontalScrollbar);
  }
  iAdjustable = 0;
  cxFixedRemaining = cxFixed;
  for ( it2 = 0; it2 < PtrArray_GetSize(ctr->m_items); it2++ ) {
    RECT rcPadding, rcCtrl;
    CControlUI* pControl = (CControlUI*)PtrArray_GetAt( ctr->m_items, it2 );
    if ( ! IsVisible(pControl) )
      continue;
    //if ( pControl->IsFloat() ) { SetFloatPos( it2 ); continue; }
    rcPadding =  GetPadding(pControl);
    szRemaining.cx -= rcPadding.left;
    sz = EstimateSize( ww, pControl, szRemaining );
    if ( sz.cx == 0 ) {
      iAdjustable++;
      sz.cx = cxExpand;
      // Distribute remaining to last element (usually round-off left-overs)
      if ( iAdjustable == nAdjustables ) {
        sz.cx = MAX( 0, szRemaining.cx - rcPadding.right - cxFixedRemaining );
      }
      if ( sz.cx < GetMinWidth(pControl) )
        sz.cx = GetMinWidth(pControl);
      if ( sz.cx > GetMaxWidth(pControl) )
        sz.cx = GetMaxWidth(pControl);
    } else {
      if ( sz.cx < GetMinWidth(pControl) )
        sz.cx = GetMinWidth(pControl);
      if ( sz.cx > GetMaxWidth(pControl) )
        sz.cx = GetMaxWidth(pControl);
      cxFixedRemaining -= sz.cx;
    }
    //if(sz.cy == 0) {
    //    sz.cy = MAX(0, rc.bottom - rc.top - rcPadding.top - rcPadding.bottom);
    //}
    sz.cy = MAX( 0, rc.bottom - rc.top - rcPadding.top - rcPadding.bottom );
    if ( sz.cy < GetMinHeight(pControl) )
      sz.cy = GetMinHeight(pControl);
    if ( sz.cy > GetMaxHeight(pControl) )
      sz.cy = GetMaxHeight(pControl);
    rcCtrl = cRect( iPosX + rcPadding.left, rc.top + rcPadding.top, iPosX + sz.cx + rcPadding.left + rcPadding.right, rc.top + rcPadding.top + sz.cy);
    SetPos( pControl, rcCtrl );
    iPosX += sz.cx + ctn->m_iChildPadding + rcPadding.left + rcPadding.right;
    szRemaining.cx -= sz.cx + ctn->m_iChildPadding + rcPadding.right;
  }
  // Process the scrollbar
  CContainerUI_ProcessScrollbar( ww, ctr, rc, 0, 0 );
  return 0;
}
int UIHorizontalLayout_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event ) {
  UIHorizontalLayout* this = (UIHorizontalLayout*)ctr;
  if ( this->m_iSepWidth != 0 ) {
    if ( event->Type == UIEVENT_BUTTONDOWN && IsEnabled(ctr) ) {
      RECT rcSeparator = UIHorizontalLayout_GetThumbRect( ctr, false );
      if ( PtInRect( &rcSeparator, event->ptMouse ) ) {
        this->m_uButtonState |= UISTATE_CAPTURED;
        this->ptLastMouse = event->ptMouse;
        this->m_rcNewPos = ctr->m_rcItem;
        //if ( !m_bImmMode && m_pManager ) m_pManager->AddPostPaint( this );
        return 0;
      }
    }
    if ( event->Type == UIEVENT_BUTTONUP ) {
      if ( ( this->m_uButtonState & UISTATE_CAPTURED ) != 0 ) {
        this->m_uButtonState &= ~UISTATE_CAPTURED;
        ctr->m_rcItem = this->m_rcNewPos;
        //if ( !m_bImmMode && m_pManager ) m_pManager->RemovePostPaint( this );
        //NeedParentUpdate();
        return 0;
      }
    }
    if ( event->Type == UIEVENT_MOUSEMOVE ) {
      if ( ( this->m_uButtonState & UISTATE_CAPTURED ) != 0 ) {
        LONG cx = event->ptMouse.x - this->ptLastMouse.x;
        RECT rc = this->m_rcNewPos;
        RECT rcInvalidate;
        this->ptLastMouse = event->ptMouse;
        if ( this->m_iSepWidth >= 0 ) {
          if ( cx > 0 && event->ptMouse.x < this->m_rcNewPos.right - this->m_iSepWidth )
            return 0;
          if ( cx < 0 && event->ptMouse.x > this->m_rcNewPos.right )
            return 0;
          rc.right += cx;
          if ( rc.right - rc.left <= GetMinWidth(ctr) ) {
            if ( this->m_rcNewPos.right - this->m_rcNewPos.left <= GetMinWidth(ctr) )
              return 0;
            rc.right = rc.left + GetMinWidth(ctr);
          }
          if ( rc.right - rc.left >= GetMaxWidth(ctr) ) {
            if ( this->m_rcNewPos.right - this->m_rcNewPos.left >= GetMaxWidth(ctr) )
              return 0;
            rc.right = rc.left + GetMaxWidth(ctr);
          }
        } else {
          if ( cx > 0 && event->ptMouse.x < this->m_rcNewPos.left )
            return 0;
          if ( cx < 0 && event->ptMouse.x > this->m_rcNewPos.left - this->m_iSepWidth )
            return 0;
          rc.left += cx;
          if ( rc.right - rc.left <= GetMinWidth(ctr) ) {
            if ( this->m_rcNewPos.right - this->m_rcNewPos.left <= GetMinWidth(ctr) )
              return 0;
            rc.left = rc.right - GetMinWidth(ctr);
          }
          if ( rc.right - rc.left >= GetMaxWidth(ctr) ) {
            if ( this->m_rcNewPos.right - this->m_rcNewPos.left >= GetMaxWidth(ctr) )
              return 0;
            rc.left = rc.right - GetMaxWidth(ctr);
          }
        }
        rcInvalidate = UIHorizontalLayout_GetThumbRect( ctr, true );
        this->m_rcNewPos = rc;
        ctr->m_cxyFixed.cx = this->m_rcNewPos.right - this->m_rcNewPos.left;
        if ( this->m_bImmMode ) {
          ctr->m_rcItem = this->m_rcNewPos;
          //NeedParentUpdate();
        } else {
          rc = UIHorizontalLayout_GetThumbRect( ctr, true );
          RectJoin( rcInvalidate, rc );
          rc = UIHorizontalLayout_GetThumbRect( ctr, false );
          RectJoin( rcInvalidate, rc );
          InvalidateRect( ww->m_hWnd, &rcInvalidate, 0 );
        }
        return 0;
      }
    }
    if ( event->Type == UIEVENT_SETCURSOR ) {
      RECT rcSeparator = UIHorizontalLayout_GetThumbRect( ctr, false );
      if ( IsEnabled(ctr) && PtInRect( &rcSeparator, event->ptMouse ) ) {
        SetCursor( LoadCursor( NULL, MAKEINTRESOURCE( IDC_SIZEWE ) ) );
        return 0;
      }
    }
  }
  CContainerUI_Event( ww, ctr, event );
  return 0;
}
