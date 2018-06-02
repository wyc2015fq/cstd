
#define PtrArray_IsEmpty(p)  (0==PtrArray_GetSize(p))
int CContainerUI_CtrInit(CControlUI* ctr) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  CControlUI_CtrInit(ctr);
  ctn->m_iChildPadding=( 0 ),
    ctn->m_bAutoDestroy=( true ),
    ctn->m_bMouseChildEnabled=( true ),
    ctn->m_pVerticalScrollbar=( NULL ),
    ctn->m_pHorizontalScrollbar=( NULL ),
    ctn->m_bScrollProcess=( false );
  ZeroMemory( &ctn->m_rcInset, sizeof( ctn->m_rcInset ) );
  return 0;
}
int CContainerUI_CtrFree(CControlUI* ctr) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  //RemoveAll();
  if ( ctn->m_pVerticalScrollbar )
    CtrFree(ctn->m_pVerticalScrollbar);
  if ( ctn->m_pHorizontalScrollbar )
    CtrFree(ctn->m_pHorizontalScrollbar);
  CControlUI_CtrFree(ctr);
  return 0;
}
int CContainerUI_SetPos( CWindowWnd* ww, CControlUI* ctr, RECT rc ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  int it;
  CControlUI_SetPos( ww, ctr, rc );
  if ( PtrArray_IsEmpty(ctr->m_items) )
    return 0;
  rc.left += ctn->m_rcInset.left;
  rc.top += ctn->m_rcInset.top;
  rc.right -= ctn->m_rcInset.right;
  rc.bottom -= ctn->m_rcInset.bottom;
  for ( it = 0; it < PtrArray_GetSize(ctr->m_items); it++ ) {
    CControlUI* pControl = (CControlUI*)PtrArray_GetAt( ctr->m_items, it );
    if ( !IsVisible(pControl) )
      continue;
    //if ( IsFloat(pControl) ) {
    //  SetFloatPos( it );
    //} else 
    {
      SetPos( pControl, rc ); // 所有非float子控件放大到整个客户区
    }
  }
  return 0;
}

#define CControlUI_SetMouseEnabled( ctr, bEnabled )  ((ctr)->m_bMouseEnabled = bEnabled)

void CContainerUI_SetMouseEnabled( CWindowWnd* ww, CControlUI* ctr, bool bEnabled ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  if ( ctn->m_pVerticalScrollbar != NULL )
    CControlUI_SetMouseEnabled( ctn->m_pVerticalScrollbar, bEnabled );
  if ( ctn->m_pHorizontalScrollbar != NULL )
    CControlUI_SetMouseEnabled(ctn->m_pHorizontalScrollbar, bEnabled );
  CControlUI_SetMouseEnabled( ctr, bEnabled );
}
#define cast(T, p)  ((T)(p))
#define UIScrollbar_GetScrollPos(ctr) cast(UIScrollbarBase*, ctr)->m_nScrollPos
#define UIScrollbar_GetLastScrollPos(ctr) cast(UIScrollbarBase*, ctr)->m_nLastScrollPos
#define UIScrollbar_GetScrollRange(ctr) cast(UIScrollbarBase*, ctr)->m_nRange
// 实际大小位置使用GetPos获取，这里得到的是预设的参考值
#define CControlUI_GetFixed(ctr) cast(CControlUI*, ctr)->m_cxyFixed

SIZE CContainerUI_GetScrollPos(CControlUI* ctr) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  SIZE sz = {0, 0};
  if ( ctn->m_pVerticalScrollbar && IsVisible(ctn->m_pVerticalScrollbar) )
    sz.cy = UIScrollbar_GetScrollPos(ctn->m_pVerticalScrollbar);
  if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) )
    sz.cx = UIScrollbar_GetScrollPos(ctn->m_pHorizontalScrollbar);
  return sz;
}
SIZE CContainerUI_GetScrollRange(CControlUI* ctr) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  SIZE sz = {0, 0};
  if ( ctn->m_pVerticalScrollbar && IsVisible(ctn->m_pVerticalScrollbar) )
    sz.cy = UIScrollbar_GetScrollRange(ctn->m_pVerticalScrollbar);
  if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) )
    sz.cx = UIScrollbar_GetScrollRange(ctn->m_pHorizontalScrollbar);
  return sz;
}
int CContainerUI_SetScrollPos( CWindowWnd* ww, CControlUI* ctr, SIZE szPos ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  int cx = 0, cy = 0, it2;
  RECT rcPos;
  if ( ctn->m_pVerticalScrollbar && IsVisible(ctn->m_pVerticalScrollbar) ) {
    int iLastScrollPos = UIScrollbar_GetScrollPos(ctn->m_pVerticalScrollbar);
    UIScrollbar_SetScrollPos(ww, ctn->m_pVerticalScrollbar, szPos.cy );
    cy = UIScrollbar_GetScrollPos(ctn->m_pVerticalScrollbar) - iLastScrollPos;
  }
  if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) ) {
    int iLastScrollPos = UIScrollbar_GetScrollPos(ctn->m_pHorizontalScrollbar);
    UIScrollbar_SetScrollPos(ww, ctn->m_pHorizontalScrollbar, szPos.cx );
    cx = UIScrollbar_GetScrollPos(ctn->m_pHorizontalScrollbar) - iLastScrollPos;
  }
  if ( cx == 0 && cy == 0 )
    return 0;
  for ( it2 = 0; it2 < PtrArray_GetSize(ctr->m_items); it2++ ) {
    CControlUI* pControl = (CControlUI*)PtrArray_GetAt( ctr->m_items, it2 );
    if ( !IsVisible(pControl) )
      continue;
    //if ( IsFloat(pControl) )  continue;
    rcPos = GetPos(pControl);
    rcPos.left -= cx;
    rcPos.right -= cx;
    rcPos.top -= cy;
    rcPos.bottom -= cy;
    SetPos( pControl, rcPos );
  }
  Invalidate(ww->m_hWnd);
  return 0;
}
void CContainerUI_LineUp(CWindowWnd* ww, CControlUI* ctr) {
  int cyLine = 8;
  SIZE sz;
  if ( ww->m_pResource )
    cyLine = UIResource_GetDefaultFontInfo(ww->m_pResource).tmHeight + 8;
  sz = CContainerUI_GetScrollPos(ctr);
  sz.cy -= cyLine;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_LineDown(CWindowWnd* ww, CControlUI* ctr) {
  int cyLine = 8;
  SIZE sz;
  if ( ww->m_pResource )
    cyLine = UIResource_GetDefaultFontInfo(ww->m_pResource).tmHeight + 8;
  sz = CContainerUI_GetScrollPos(ctr);
  sz.cy += cyLine;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_PageUp(CWindowWnd* ww, CControlUI* ctr) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  int iOffset = ctr->m_rcItem.right - ctr->m_rcItem.left - ctn->m_rcInset.left - ctn->m_rcInset.right;
  if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) )
    iOffset -= CControlUI_GetFixed(ctn->m_pHorizontalScrollbar).cy;
  sz.cy -= iOffset;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_PageDown(CWindowWnd* ww, CControlUI* ctr) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  int iOffset = ctr->m_rcItem.right - ctr->m_rcItem.left - ctn->m_rcInset.left - ctn->m_rcInset.right;
  if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) )
    iOffset -= CControlUI_GetFixed(ctn->m_pHorizontalScrollbar).cy;
  sz.cy += iOffset;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_PageLeft(CWindowWnd* ww, CControlUI* ctr) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  int iOffset = ctr->m_rcItem.right - ctr->m_rcItem.left - ctn->m_rcInset.left - ctn->m_rcInset.right;
  if ( ctn->m_pVerticalScrollbar && IsVisible(ctn->m_pVerticalScrollbar) )
    iOffset -= CControlUI_GetFixed(ctn->m_pVerticalScrollbar).cx;
  sz.cx -= iOffset;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_PageRight(CWindowWnd* ww, CControlUI* ctr) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  int iOffset = ctr->m_rcItem.right - ctr->m_rcItem.left - ctn->m_rcInset.left - ctn->m_rcInset.right;
  if ( ctn->m_pVerticalScrollbar && IsVisible(ctn->m_pVerticalScrollbar) )
    iOffset -= CControlUI_GetFixed(ctn->m_pVerticalScrollbar).cx;
  sz.cx += iOffset;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_HomeUp(CWindowWnd* ww, CControlUI* ctr) {
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  sz.cy = 0;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_EndDown(CWindowWnd* ww, CControlUI* ctr) {
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  sz.cy = CContainerUI_GetScrollRange(ctr).cy;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_LineLeft(CWindowWnd* ww, CControlUI* ctr) {
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  sz.cx -= 8;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_LineRight(CWindowWnd* ww, CControlUI* ctr) {
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  sz.cx += 8;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_HomeLeft(CWindowWnd* ww, CControlUI* ctr) {
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  sz.cx = 0;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
void CContainerUI_EndRight(CWindowWnd* ww, CControlUI* ctr) {
  SIZE sz = CContainerUI_GetScrollPos(ctr);
  sz.cx = CContainerUI_GetScrollRange(ctr).cx;
  CContainerUI_SetScrollPos( ww, ctr, sz );
}
int CContainerUI_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  if ( event->Type == UIEVENT_FIND_CONTROL_BY_POINT && PtInRect(&ctr->m_rcItem, event->ptMouse)) {
    CControlUI* ret;
    event->pSender = ctr;
    if ( ctn->m_pVerticalScrollbar != NULL ) {
      ret = FindControlByPoint2( ww, ctn->m_pVerticalScrollbar, event->ptMouse );
      if (NULL!=ret) {
        event->pSender = ret;
        return 0;
      }
    }
    if ( ctn->m_pHorizontalScrollbar != NULL ) {
      ret = FindControlByPoint2( ww, ctn->m_pHorizontalScrollbar, event->ptMouse );
      if (NULL!=ret) {
        event->pSender = ret;
        return 0;
      }
    }
    return CControlUI_Event(ww, ctr, event);
  }
  if ( !IsMouseEnabled(ctr) && event->Type > UIEVENT__MOUSEBEGIN && event->Type < UIEVENT__MOUSEEND ) {
    if ( ctr->m_pParent != NULL )
      Event( ctr->m_pParent, *event );
    else
      CControlUI_Event( ww, ctr, event );
    return 0;
  }
  if ( event->Type == UIEVENT_SETFOCUS ) {
    ctr->m_bFocused = true;
    return 0;
  }
  if ( event->Type == UIEVENT_KILLFOCUS ) {
    ctr->m_bFocused = false;
    return 0;
  }
  if ( ctn->m_pVerticalScrollbar != NULL && PtInRect(&ctn->m_pVerticalScrollbar->m_rcItem, event->ptMouse)) {
    UIScrollbarBase_Event(ww, ctn->m_pVerticalScrollbar, event);
  }
  if ( ctn->m_pHorizontalScrollbar != NULL && PtInRect(&ctn->m_pHorizontalScrollbar->m_rcItem, event->ptMouse)) {
    UIScrollbarBase_Event(ww, ctn->m_pHorizontalScrollbar, event);
  }
  if ( ctn->m_pVerticalScrollbar != NULL && IsVisible(ctn->m_pVerticalScrollbar) && IsEnabled(ctn->m_pVerticalScrollbar) ) {
    if ( event->Type == UIEVENT_KEYDOWN ) {
      switch ( event->chKey ) {
      case VK_DOWN:
        CContainerUI_LineDown(ww, ctr);
        return 0;
      case VK_UP:
        CContainerUI_LineUp(ww, ctr);
        return 0;
      case VK_NEXT:
        CContainerUI_PageDown(ww, ctr);
        return 0;
      case VK_PRIOR:
        CContainerUI_PageUp(ww, ctr);
        return 0;
      case VK_HOME:
        CContainerUI_HomeUp(ww, ctr);
        return 0;
      case VK_END:
        CContainerUI_EndDown(ww, ctr);
        return 0;
      }
    } else if ( event->Type == UIEVENT_SCROLLWHEEL ) {
      switch ( LOWORD( event->wParam ) ) {
      case SB_LINEUP:
        CContainerUI_LineUp(ww, ctr);
        return 0;
      case SB_LINEDOWN:
        CContainerUI_LineDown(ww, ctr);
        return 0;
      }
    }
  } else if ( ctn->m_pHorizontalScrollbar != NULL && IsVisible(ctn->m_pHorizontalScrollbar) && IsEnabled(ctn->m_pHorizontalScrollbar) ) {
    if ( event->Type == UIEVENT_KEYDOWN ) {
      switch ( event->chKey ) {
      case VK_DOWN:
        CContainerUI_LineRight(ww, ctr);
        return 0;
      case VK_UP:
        CContainerUI_LineLeft(ww, ctr);
        return 0;
      case VK_NEXT:
        CContainerUI_PageRight(ww, ctr);
        return 0;
      case VK_PRIOR:
        CContainerUI_PageLeft(ww, ctr);
        return 0;
      case VK_HOME:
        CContainerUI_HomeLeft(ww, ctr);
        return 0;
      case VK_END:
        CContainerUI_EndRight(ww, ctr);
        return 0;
      }
    } else if ( event->Type == UIEVENT_SCROLLWHEEL ) {
      switch ( LOWORD( event->wParam ) ) {
      case SB_LINEUP:
        CContainerUI_LineLeft(ww, ctr);
        return 0;
      case SB_LINEDOWN:
        CContainerUI_LineRight(ww, ctr);
        return 0;
      }
    }
  }
  CControlUI_Event( ww, ctr, event );
  return 0;
}
#define CContainerUI_GetVerticalScrollbar()    cast(CContainerUI*, ctn)->m_pVerticalScrollbar
#define CContainerUI_GetHorizontalScrollbar()  cast(CContainerUI*, ctn)->m_pHorizontalScrollbar
void CContainerUI_EnableScrollBar( UIResource* res, CControlUI* ctr, bool bEnableVertical, bool bEnableHorizontal ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
#define SCROLLBARTYPE  "ScrollbarBase"
  if ( bEnableVertical && !ctn->m_pVerticalScrollbar ) {
    ctn->m_pVerticalScrollbar = CreateControl(SCROLLBARTYPE);
    ctn->m_pVerticalScrollbar->m_pParent = ctr;
    //ctn->m_pVerticalScrollbar->SetOwner( this );
    //ctn->m_pVerticalScrollbar->SetManager( ww->m_pResource, NULL, false );
    if ( res ) {
      LPCTSTR pDefaultAttributes = UIResource_GetDefaultAttributeList( res, _T( "VScrollBar" ) );
      if ( pDefaultAttributes ) {
        CControlUI_ApplyAttributeList( res, ctn->m_pVerticalScrollbar, pDefaultAttributes );
      }
    }
  } else if ( !bEnableVertical && ctn->m_pVerticalScrollbar ) {
    CtrFree(ctn->m_pVerticalScrollbar);
    ctn->m_pVerticalScrollbar = NULL;
  }
  if ( bEnableHorizontal && !ctn->m_pHorizontalScrollbar ) {
    ctn->m_pHorizontalScrollbar = CreateControl(SCROLLBARTYPE);
    UIScrollbarBase_SetHorizontal( ctn->m_pHorizontalScrollbar, true );
    ctn->m_pHorizontalScrollbar->m_pParent = ctr;
    //ctn->m_pHorizontalScrollbar->SetOwner( this );
    //ctn->m_pHorizontalScrollbar->SetManager( res, NULL, false );
    if ( res ) {
      LPCTSTR pDefaultAttributes = UIResource_GetDefaultAttributeList( res, _T( "HScrollBar" ) );
      if ( pDefaultAttributes ) {
        CControlUI_ApplyAttributeList( res, ctn->m_pHorizontalScrollbar, pDefaultAttributes );
      }
    }
  } else if ( !bEnableHorizontal && ctn->m_pHorizontalScrollbar ) {
    CtrFree(ctn->m_pHorizontalScrollbar);
    ctn->m_pHorizontalScrollbar = NULL;
  }
  NeedUpdate(0, ctr);
}

int CContainerUI_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  if ( _tcscmp( pstrName, _T( "inset" ) ) == 0 ) {
    RECT rcInset = { 0 };
    LPTSTR pstr = NULL;
    rcInset.left = _tcstol( pstrValue, &pstr, 10 );
    ASSERT( pstr );
    rcInset.top = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    rcInset.right = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    rcInset.bottom = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    ctn->m_rcInset = ( rcInset );
  } else if ( _tcscmp( pstrName, _T( "mousechild" ) ) == 0 )
    ctn->m_bMouseChildEnabled = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  else if ( _tcscmp( pstrName, _T( "vscrollbar" ) ) == 0 ) {
    CContainerUI_EnableScrollBar( res, ctr, _tcscmp( pstrValue, _T( "true" ) ) == 0, CContainerUI_GetHorizontalScrollbar() != NULL );
  } else if ( _tcscmp( pstrName, _T( "hscrollbar" ) ) == 0 ) {
    CContainerUI_EnableScrollBar( res, ctr, CContainerUI_GetVerticalScrollbar() != NULL, _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  } else if ( _tcscmp( pstrName, _T( "childpadding" ) ) == 0 )
    ctn->m_iChildPadding = ( _ttoi( pstrValue ) );
  else
    CControlUI_SetAttribute( res, ctr, pstrName, pstrValue );
  return 0;
}
int CContainerUI_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  RECT rcTemp = { 0 };
  CRenderClip clip;
  int it;
  if ( !IntersectRect( &rcTemp, &rcPaint, &ctr->m_rcItem ) )
    return 0;
  GenerateClip( hDC, rcTemp, &clip );
  CControlUI_DoPaint( ww, ctr, hDC, rcPaint );
  if ( PtrArray_GetSize(ctr->m_items) > 0 ) {
    RECT rc = ctr->m_rcItem;
    rc.left += ctn->m_rcInset.left;
    rc.top += ctn->m_rcInset.top;
    rc.right -= ctn->m_rcInset.right;
    rc.bottom -= ctn->m_rcInset.bottom;
    if ( ctn->m_pVerticalScrollbar && IsVisible(ctn->m_pVerticalScrollbar) )
      rc.right -= CControlUI_GetFixed(ctn->m_pVerticalScrollbar).cx;
    if ( ctn->m_pHorizontalScrollbar && IsVisible(ctn->m_pHorizontalScrollbar) )
      rc.bottom -= CControlUI_GetFixed(ctn->m_pHorizontalScrollbar).cy;
    if ( !IntersectRect( &rcTemp, &rcPaint, &rc ) ) {
      for ( it = 0; it < PtrArray_GetSize(ctr->m_items); it++ ) {
        CControlUI* pControl = (CControlUI*)PtrArray_GetAt( ctr->m_items, it );
        if ( !IsVisible(pControl) )
          continue;
        if ( !IntersectRect( &rcTemp, &rcPaint, &GetPos(pControl) ) )
          continue;
        //if ( pControl ->IsFloat() ) {
        //  if ( !IntersectRect( &rcTemp, &ctr->m_rcItem, &GetPos(pControl) ) )
        //    continue;
        //  pControl->DoPaint( hDC, rcPaint );
        //}
      }
    } else {
      CRenderClip childClip;
      GenerateClip( hDC, rcTemp, &childClip );
      for ( it = 0; it < PtrArray_GetSize(ctr->m_items); it++ ) {
        CControlUI* pControl = (CControlUI*)PtrArray_GetAt( ctr->m_items, it );
        if ( !IsVisible(pControl) )
          continue;
        if ( !IntersectRect( &rcTemp, &rcPaint, &GetPos(pControl) ) )
          continue;
        //if ( pControl ->IsFloat() ) {
        //  if ( !IntersectRect( &rcTemp, &ctr->m_rcItem, &GetPos(pControl) ) )
        //    continue;
        //  CRenderClipUseOldClipBegin( hDC, childClip );
        //  pControl->DoPaint( hDC, rcPaint );
        //  CRenderClipUseOldClipEnd( hDC, childClip );
        //} else 
        //if (ctr->m_bUpdateNeeded) 
        {
          if ( !IntersectRect( &rcTemp, &rc, &GetPos(pControl) ) )
            continue;
          DoPaint( pControl, hDC, rcPaint );
          //ctr->m_bUpdateNeeded=0;
        }
      }
      CRenderClip_Clear(&childClip);
    }
  }
  if ( ctn->m_pVerticalScrollbar != NULL && IsVisible(ctn->m_pVerticalScrollbar) ) {
    if ( IntersectRect( &rcTemp, &rcPaint, &GetPos(ctn->m_pVerticalScrollbar) ) ) {
      DoPaint( ctn->m_pVerticalScrollbar, hDC, rcPaint );
    }
  }
  if ( ctn->m_pHorizontalScrollbar != NULL && IsVisible(ctn->m_pHorizontalScrollbar) ) {
    if ( IntersectRect( &rcTemp, &rcPaint, &GetPos(ctn->m_pHorizontalScrollbar) ) ) {
      DoPaint( ctn->m_pHorizontalScrollbar, hDC, rcPaint );
    }
  }
  CRenderClip_Clear(&clip);
  return 0;
}
#define CControlUI_SetVisible(ctr, bVisible) (cast(CControlUI*, ctr)->m_bVisible=bVisible)
int CContainerUI_ProcessScrollbar( CWindowWnd* ww, CControlUI* ctr, RECT rc, int cxRequired, int cyRequired ) {
  CContainerUI* ctn = (CContainerUI*)ctr;
  int cyScroll;
  if ( ctn->m_pHorizontalScrollbar != NULL && IsVisible(ctn->m_pHorizontalScrollbar) ) {
    RECT rcScrollbarPos = { rc.left, rc.bottom, rc.right, rc.bottom + CControlUI_GetFixed(ctn->m_pHorizontalScrollbar).cy };
    SetPos( ctn->m_pHorizontalScrollbar, rcScrollbarPos );
  }
  if ( ctn->m_pVerticalScrollbar == NULL )
    return 0;
  if ( cyRequired > rc.bottom - rc.top && !IsVisible(ctn->m_pVerticalScrollbar) ) {
    CControlUI_SetVisible( ctn->m_pVerticalScrollbar, true );
    UIScrollbar_SetScrollRange( ww, ctn->m_pVerticalScrollbar, cyRequired - ( rc.bottom - rc.top ) );
    UIScrollbar_SetScrollPos(ww, ctn->m_pVerticalScrollbar, 0 );
    ctn->m_bScrollProcess = true;
    SetPos( ctr, ctr->m_rcItem );
    ctn->m_bScrollProcess = false;
    return 0;
  }
  // No scrollbar required
  if ( !IsVisible(ctn->m_pVerticalScrollbar) )
    return 0;
  // Scroll not needed anymore?
  cyScroll = cyRequired - ( rc.bottom - rc.top );
  if ( cyScroll <= 0 && !ctn->m_bScrollProcess ) {
    CControlUI_SetVisible( ctn->m_pVerticalScrollbar, false );
    UIScrollbar_SetScrollPos(ww, ctn->m_pVerticalScrollbar, 0 );
    UIScrollbar_SetScrollRange( ww, ctn->m_pVerticalScrollbar, 0 );
    SetPos( ctr, ctr->m_rcItem );
  } else {
    RECT rcScrollbarPos = { rc.right, rc.top, rc.right + CControlUI_GetFixed(ctn->m_pVerticalScrollbar).cx, rc.bottom };
    SetPos( ctn->m_pVerticalScrollbar, rcScrollbarPos );
    if ( UIScrollbar_GetScrollRange(ctn->m_pVerticalScrollbar) != cyScroll ) {
      int iScrollPos = UIScrollbar_GetScrollPos(ctn->m_pVerticalScrollbar);
      UIScrollbar_SetScrollRange( ww, ctn->m_pVerticalScrollbar, abs( cyScroll ) );
      if ( UIScrollbar_GetScrollRange(ctn->m_pVerticalScrollbar) == 0 ) {
        CControlUI_SetVisible( ctn->m_pVerticalScrollbar, false );
        UIScrollbar_SetScrollPos(ww, ctn->m_pVerticalScrollbar, 0 );
      }
      if ( iScrollPos > UIScrollbar_GetScrollPos(ctn->m_pVerticalScrollbar) ) {
        SetPos( ctr, ctr->m_rcItem );
      }
    }
  }
  return 0;
}

