
#define UIVerticalLayout_CtrInit CContainerUI_CtrInit
#define UIVerticalLayout_CtrFree CContainerUI_CtrFree
#define UIVerticalLayout_DoPaint CContainerUI_DoPaint
#define UIVerticalLayout_Event CContainerUI_Event
#define UIVerticalLayout_SetAttribute CContainerUI_SetAttribute
typedef CContainerUI UIVerticalLayout;
#define GetMinHeight(ctr)   (ctr)->m_cxyMin.cy
#define GetMinWidth(ctr)    (ctr)->m_cxyMin.cx
#define GetMaxHeight(ctr)   (ctr)->m_cxyMax.cy
#define GetMaxWidth(ctr)    (ctr)->m_cxyMax.cx
#define GetPadding(ctr)     (ctr)->m_rcPadding
SIZE EstimateSize( CWindowWnd* ww, CControlUI* ctr, SIZE szAvailable ) {
  SIZE sz = ctr->m_cxyFixed;
  if ( ctr->m_cxyFixed.cy == 0 ) {
    ctr->m_cxyFixed.cy = UIResource_GetDefaultFontInfo(ww->m_pResource).tmHeight + 8;
  }
  return sz;
}
int UIVerticalLayout_SetPos( CWindowWnd* ww, CControlUI* ctr, RECT rc ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  SIZE sz, szAvailable, szRemaining;
  int it1, it2, nAdjustables = 0, cyFixed = 0, nEstimateNum = 0;
  int cyNeeded = 0, cyExpand = 0;
  int iPosX, iPosY;
  int cyFixedRemaining, iAdjustable;
  CControlUI_SetPos( ww, ctr, rc );
  rc = ctr->m_rcItem;
  // Adjust for inset
  rc.left += ctn->m_rcInset.left;
  rc.top += ctn->m_rcInset.top;
  rc.right -= ctn->m_rcInset.right;
  rc.bottom -= ctn->m_rcInset.bottom;
  if ( ctn->m_pVerticalScrollbar && IsVisible(ctn->m_pVerticalScrollbar) )
    rc.right -= CControlUI_GetFixed(ctn->m_pVerticalScrollbar).cx;
  if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) )
    rc.bottom -= CControlUI_GetFixed(ctn->m_pHorizontalScrollbar).cy;
  if ( PtrArray_GetSize(ctr->m_items) == 0 ) {
    CContainerUI_ProcessScrollbar( ww, ctr, rc, 0, 0 );
    return 0;
  }
  // Determine the minimum size
  szAvailable.cx = rc.right - rc.left, szAvailable.cy = rc.bottom - rc.top;
  if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) )
    szAvailable.cx +=UIScrollbar_GetScrollRange(ctn->m_pHorizontalScrollbar);
  for ( it1 = 0; it1 < PtrArray_GetSize(ctr->m_items); it1++ ) {
    CControlUI* pControl = (CControlUI*)PtrArray_GetAt( ctr->m_items, it1 );
    if ( !IsVisible(pControl) )
      continue;
    //if ( pControl->IsFloat() )
    //  continue;
    sz = EstimateSize( ww, pControl, szAvailable );
    if ( sz.cy == 0 ) {
      nAdjustables++;
    } else {
      if ( sz.cy < GetMinHeight(pControl) )
        sz.cy = GetMinHeight(pControl);
      if ( sz.cy > GetMaxHeight(pControl) )
        sz.cy = GetMaxHeight(pControl);
    }
    cyFixed += sz.cy + GetPadding(pControl).top + GetPadding(pControl).bottom;
    nEstimateNum++;
  }
  cyFixed += ( nEstimateNum - 1 ) * ctn->m_iChildPadding;
  // Place elements
  if ( nAdjustables > 0 )
    cyExpand = MAX( 0, ( szAvailable.cy - cyFixed ) / nAdjustables );
  // Position the elements
  szRemaining = szAvailable;
  iPosY = rc.top;
  if ( ctn->m_pVerticalScrollbar && IsVisible(ctn->m_pVerticalScrollbar) ) {
    iPosY -= UIScrollbar_GetScrollPos(ctn->m_pVerticalScrollbar);
  }
  iPosX = rc.left;
  if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) ) {
    iPosX -= UIScrollbar_GetScrollPos(ctn->m_pHorizontalScrollbar);
  }
  iAdjustable = 0;
  cyFixedRemaining = cyFixed;
  for ( it2 = 0; it2 < PtrArray_GetSize(ctr->m_items); it2++ ) {
    RECT rcPadding;
    SIZE sz={0, 0};
    CControlUI* pControl = (CControlUI*)PtrArray_GetAt( ctr->m_items, it2 );
    if ( !IsVisible(pControl) )
      continue;
    //if ( pControl->IsFloat() ) { SetFloatPos( it2 ); continue; }
    rcPadding = GetPadding(pControl);
    szRemaining.cy -= rcPadding.top;
    sz = EstimateSize( ww, pControl, szRemaining );
    if ( sz.cy == 0 ) {
      iAdjustable++;
      sz.cy = cyExpand;
      // Distribute remaining to last element (usually round-off left-overs)
      if ( iAdjustable == nAdjustables ) {
        sz.cy = MAX( 0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining );
      }
      if ( sz.cy < GetMinHeight(pControl) )
        sz.cy = GetMinHeight(pControl);
      if ( sz.cy > GetMaxHeight(pControl) )
        sz.cy = GetMaxHeight(pControl);
    } else {
      if ( sz.cy < GetMinHeight(pControl) )
        sz.cy = GetMinHeight(pControl);
      if ( sz.cy > GetMaxHeight(pControl) )
        sz.cy = GetMaxHeight(pControl);
      cyFixedRemaining -= sz.cy;
    }
    //if(sz.cx == 0) {
    //  sz.cx = MAX(0, rc.right - rc.left - rcPadding.left - rcPadding.right);
    //}
    sz.cx = MAX( 0, szAvailable.cx - rcPadding.left - rcPadding.right );
    if ( sz.cx < GetMinWidth(pControl) )
      sz.cx = GetMinWidth(pControl);
    if ( sz.cx > GetMaxWidth(pControl) )
      sz.cx = GetMaxWidth(pControl);
    {RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top + rcPadding.bottom };
    SetPos( pControl, rcCtrl );}
    iPosY += sz.cy + ctn->m_iChildPadding + rcPadding.top + rcPadding.bottom;
    cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
    szRemaining.cy -= sz.cy + ctn->m_iChildPadding + rcPadding.bottom;
  }
  cyNeeded += ( nEstimateNum - 1 ) * ctn->m_iChildPadding;
  // Process the scrollbar
  CContainerUI_ProcessScrollbar( ww, ctr, rc, 0, cyNeeded );
  return 0;
}

#include "HorizontalLayout.inl"