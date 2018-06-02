
#define UIScrollbarBase_CtrFree CControlUI_CtrFree

int UIScrollbarBase_CtrInit(CControlUI* ctr) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  CControlUI_CtrInit(ctr);
  sbar->m_bHorizontal=( false ),
    sbar->m_nRange=( 100 ),
    sbar->m_nScrollPos=( 0 ),
    sbar->m_nLineSize=( 8 ),
    //ctr->m_pParent=( NULL ), 
    sbar->m_nLastScrollPos=( 0 ), sbar->m_nLastScrollOffset=( 0 ),
    sbar->m_nScrollRepeatDelay=( 0 ), sbar->m_uButton1State=( 0 ), 
    sbar->m_uButton2State=( 0 ), sbar->m_uThumbState=( 0 );
  ctr->m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
  sbar->ptLastMouse.x = sbar->ptLastMouse.y = 0;
  ZeroMemory( &sbar->m_rcThumb, sizeof( sbar->m_rcThumb ) );
  ZeroMemory( &sbar->m_rcButton1, sizeof( sbar->m_rcButton1 ) );
  ZeroMemory( &sbar->m_rcButton2, sizeof( sbar->m_rcButton2 ) );
  return 0;
}
int UIScrollbarBase_SetHorizontal( CControlUI* ctr, bool bHorizontal ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  if ( sbar->m_bHorizontal == bHorizontal )
    return 0;
  sbar->m_bHorizontal = bHorizontal;
  if ( sbar->m_bHorizontal ) {
    if ( ctr->m_cxyFixed.cy == 0 ) {
      ctr->m_cxyFixed.cx = 0;
      ctr->m_cxyFixed.cy = DEFAULT_SCROLLBAR_SIZE;
    }
  } else {
    if ( ctr->m_cxyFixed.cx == 0 ) {
      ctr->m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
      ctr->m_cxyFixed.cy = 0;
    }
  }
  NeedUpdate(0, ctr->m_pParent);
  return 0;
}
int UIScrollbarBase_SetPos( CWindowWnd* ww, CControlUI* ctr, RECT rc ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  CControlUI_SetPos( ww, ctr, rc );
  rc = ctr->m_rcItem;
  if ( sbar->m_bHorizontal ) {
    int cx = rc.right - rc.left - 2 * ctr->m_cxyFixed.cy;
    if ( cx > ctr->m_cxyFixed.cy ) {
      sbar->m_rcButton1.left = rc.left;
      sbar->m_rcButton1.top = rc.top;
      sbar->m_rcButton1.right = rc.left + ctr->m_cxyFixed.cy;
      sbar->m_rcButton1.bottom = rc.top + ctr->m_cxyFixed.cy;
      sbar->m_rcButton2.left = rc.right - ctr->m_cxyFixed.cy;
      sbar->m_rcButton2.top = rc.top;
      sbar->m_rcButton2.right = rc.right;
      sbar->m_rcButton2.bottom = rc.top + ctr->m_cxyFixed.cy;
      sbar->m_rcThumb.top = rc.top;
      sbar->m_rcThumb.bottom = rc.top + ctr->m_cxyFixed.cy;
      if ( sbar->m_nRange > 0 ) {
        int cxThumb = cx * ( rc.right - rc.left ) / ( sbar->m_nRange + rc.right - rc.left );
        if ( cxThumb < ctr->m_cxyFixed.cy )
          cxThumb = ctr->m_cxyFixed.cy;
        sbar->m_rcThumb.left = sbar->m_nScrollPos * ( cx - cxThumb ) / sbar->m_nRange + sbar->m_rcButton1.right;
        sbar->m_rcThumb.right = sbar->m_rcThumb.left + cxThumb;
        if ( sbar->m_rcThumb.right > sbar->m_rcButton2.left ) {
          sbar->m_rcThumb.left = sbar->m_rcButton2.left - cxThumb;
          sbar->m_rcThumb.right = sbar->m_rcButton2.left;
        }
      } else {
        sbar->m_rcThumb.left = sbar->m_rcButton1.right;
        sbar->m_rcThumb.right = sbar->m_rcButton2.left;
      }
    } else {
      int cxButton = ( rc.right - rc.left ) / 2;
      if ( cxButton > ctr->m_cxyFixed.cy )
        cxButton = ctr->m_cxyFixed.cy;
      sbar->m_rcButton1.left = rc.left;
      sbar->m_rcButton1.top = rc.top;
      sbar->m_rcButton1.right = rc.left + cxButton;
      sbar->m_rcButton1.bottom = rc.top + ctr->m_cxyFixed.cy;
      sbar->m_rcButton2.left = rc.right - cxButton;
      sbar->m_rcButton2.top = rc.top;
      sbar->m_rcButton2.right = rc.right;
      sbar->m_rcButton2.bottom = rc.top + ctr->m_cxyFixed.cy;
      ZeroMemory( &sbar->m_rcThumb, sizeof( sbar->m_rcThumb ) );
    }
  } else {
    int cy = rc.bottom - rc.top - 2 * ctr->m_cxyFixed.cx;
    if ( cy > ctr->m_cxyFixed.cx ) {
      sbar->m_rcButton1.left = rc.left;
      sbar->m_rcButton1.top = rc.top;
      sbar->m_rcButton1.right = rc.left + ctr->m_cxyFixed.cx;
      sbar->m_rcButton1.bottom = rc.top + ctr->m_cxyFixed.cx;
      sbar->m_rcButton2.left = rc.left;
      sbar->m_rcButton2.top = rc.bottom - ctr->m_cxyFixed.cx;
      sbar->m_rcButton2.right = rc.left + ctr->m_cxyFixed.cx;
      sbar->m_rcButton2.bottom = rc.bottom;
      sbar->m_rcThumb.left = rc.left;
      sbar->m_rcThumb.right = rc.left + ctr->m_cxyFixed.cx;
      if ( sbar->m_nRange > 0 ) {
        int cyThumb = cy * ( rc.bottom - rc.top ) / ( sbar->m_nRange + rc.bottom - rc.top );
        if ( cyThumb < ctr->m_cxyFixed.cx )
          cyThumb = ctr->m_cxyFixed.cx;
        sbar->m_rcThumb.top = sbar->m_nScrollPos * ( cy - cyThumb ) / sbar->m_nRange + sbar->m_rcButton1.bottom;
        sbar->m_rcThumb.bottom = sbar->m_rcThumb.top + cyThumb;
        if ( sbar->m_rcThumb.bottom > sbar->m_rcButton2.top ) {
          sbar->m_rcThumb.top = sbar->m_rcButton2.top - cyThumb;
          sbar->m_rcThumb.bottom = sbar->m_rcButton2.top;
        }
      } else {
        sbar->m_rcThumb.top = sbar->m_rcButton1.bottom;
        sbar->m_rcThumb.bottom = sbar->m_rcButton2.top;
      }
    } else {
      int cyButton = ( rc.bottom - rc.top ) / 2;
      if ( cyButton > ctr->m_cxyFixed.cx )
        cyButton = ctr->m_cxyFixed.cx;
      sbar->m_rcButton1.left = rc.left;
      sbar->m_rcButton1.top = rc.top;
      sbar->m_rcButton1.right = rc.left + ctr->m_cxyFixed.cx;
      sbar->m_rcButton1.bottom = rc.top + cyButton;
      sbar->m_rcButton2.left = rc.left;
      sbar->m_rcButton2.top = rc.bottom - cyButton;
      sbar->m_rcButton2.right = rc.left + ctr->m_cxyFixed.cx;
      sbar->m_rcButton2.bottom = rc.bottom;
      ZeroMemory( &sbar->m_rcThumb, sizeof( sbar->m_rcThumb ) );
    }
  }
  return 0;
}
int UIScrollbarBase_SetScrollRange( CWindowWnd* ww, CControlUI* ctr, int nRange ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  if ( sbar->m_nRange == nRange )
    return 0;
  sbar->m_nRange = nRange;
  if ( sbar->m_nRange < 0 )
    sbar->m_nRange = 0;
  if ( sbar->m_nScrollPos > sbar->m_nRange )
    sbar->m_nScrollPos = sbar->m_nRange;
  UIScrollbarBase_SetPos( ww, ctr, ctr->m_rcItem );
  return 0;
}
void UIScrollbarBase_SetScrollPos( CWindowWnd* ww, CControlUI* ctr, int nPos ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  if ( sbar->m_nScrollPos == nPos )
    return ;
  sbar->m_nScrollPos = nPos;
  if ( sbar->m_nScrollPos < 0 )
    sbar->m_nScrollPos = 0;
  if ( sbar->m_nScrollPos > sbar->m_nRange )
    sbar->m_nScrollPos = sbar->m_nRange;
  UIScrollbarBase_SetPos( ww, ctr, ctr->m_rcItem );
}
void CContainerUI_LineUp(CWindowWnd* ww, CControlUI* ctr);
void CContainerUI_LineDown(CWindowWnd* ww, CControlUI* ctr);
void CContainerUI_LineLeft(CWindowWnd* ww, CControlUI* ctr);
void CContainerUI_LineRight(CWindowWnd* ww, CControlUI* ctr);
void CContainerUI_PageUp(CWindowWnd* ww, CControlUI* ctr);
void CContainerUI_PageDown(CWindowWnd* ww, CControlUI* ctr);
void CContainerUI_PageLeft(CWindowWnd* ww, CControlUI* ctr);
void CContainerUI_PageRight(CWindowWnd* ww, CControlUI* ctr);
SIZE CContainerUI_GetScrollPos(CControlUI* ctr);
int CContainerUI_SetScrollPos( CWindowWnd* ww, CControlUI* ctr, SIZE szPos );
SIZE cSize(int cx, int cy) {
  SIZE sz;sz.cx=cx, sz.cy=cy;
  return sz;
}
#define IsMouseEnabled(ctr)  ((ctr)->m_bMouseEnabled)
int UIScrollbarBase_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  if ( !IsMouseEnabled(ctr) && event->Type > UIEVENT__MOUSEBEGIN && event->Type < UIEVENT__MOUSEEND ) {
    if ( ctr->m_pParent != NULL )
      Event( ctr->m_pParent, *event );
    else
      CControlUI_Event( ww, ctr, event );
    return 0;
  }
  if ( event->Type == UIEVENT_SETFOCUS ) {
    return 0;
  }
  if ( event->Type == UIEVENT_KILLFOCUS ) {
    return 0;
  }
  if ( event->Type == UIEVENT_BUTTONDOWN || event->Type == UIEVENT_DBLCLICK ) {
    if ( !IsEnabled(ctr) )
      return 0;
    sbar->m_nLastScrollOffset = 0;
    sbar->m_nScrollRepeatDelay = 0;
    UISetTimer( ww, ctr, DEFAULT_TIMERID, 50U );
    if ( PtInRect( &sbar->m_rcButton1, event->ptMouse ) ) {
      sbar->m_uButton1State |= UISTATE_PUSHED;
      if ( !sbar->m_bHorizontal ) {
        if ( ctr->m_pParent != NULL )
          CContainerUI_LineUp(ww, ctr->m_pParent);
        else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos - sbar->m_nLineSize );
      } else {
        if ( ctr->m_pParent != NULL )
          CContainerUI_LineLeft(ww, ctr->m_pParent);
        else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos - sbar->m_nLineSize );
      }
    } else if ( PtInRect( &sbar->m_rcButton2, event->ptMouse ) ) {
      sbar->m_uButton2State |= UISTATE_PUSHED;
      if ( !sbar->m_bHorizontal ) {
        if ( ctr->m_pParent != NULL )
          CContainerUI_LineDown(ww, ctr->m_pParent);
        else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos + sbar->m_nLineSize );
      } else {
        if ( ctr->m_pParent != NULL )
          CContainerUI_LineRight(ww, ctr->m_pParent);
        else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos + sbar->m_nLineSize );
      }
    } else if ( PtInRect( &sbar->m_rcThumb, event->ptMouse ) ) {
      sbar->m_uThumbState |= UISTATE_CAPTURED | UISTATE_PUSHED;
      sbar->ptLastMouse = event->ptMouse;
      sbar->m_nLastScrollPos = sbar->m_nScrollPos;
    } else {
      if ( !sbar->m_bHorizontal ) {
        if ( event->ptMouse.y < sbar->m_rcThumb.top ) {
          if ( ctr->m_pParent != NULL )
            CContainerUI_PageUp(ww, ctr->m_pParent);
          else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos + ctr->m_rcItem.top - ctr->m_rcItem.bottom );
        } else if ( event->ptMouse.y > sbar->m_rcThumb.bottom ) {
          if ( ctr->m_pParent != NULL )
            CContainerUI_PageDown(ww, ctr->m_pParent);
          else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos - ctr->m_rcItem.top + ctr->m_rcItem.bottom );
        }
      } else {
        if ( event->ptMouse.x < sbar->m_rcThumb.left ) {
          if ( ctr->m_pParent != NULL )
            CContainerUI_PageLeft(ww, ctr->m_pParent);
          else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos + ctr->m_rcItem.left - ctr->m_rcItem.right );
        } else if ( event->ptMouse.x > sbar->m_rcThumb.right ) {
          if ( ctr->m_pParent != NULL )
            CContainerUI_PageRight(ww, ctr->m_pParent);
          else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos - ctr->m_rcItem.left + ctr->m_rcItem.right );
        }
      }
    }
    return 0;
  }
  if ( event->Type == UIEVENT_BUTTONUP ) {
    sbar->m_nScrollRepeatDelay = 0;
    sbar->m_nLastScrollOffset = 0;
    UIKillTimer( ww, ctr, DEFAULT_TIMERID );
    if ( ( sbar->m_uThumbState & UISTATE_CAPTURED ) != 0 ) {
      sbar->m_uThumbState &= ~( UISTATE_CAPTURED | UISTATE_PUSHED );
      Invalidate(ww->m_hWnd);
    } else if ( ( sbar->m_uButton1State & UISTATE_PUSHED ) != 0 ) {
      sbar->m_uButton1State &= ~UISTATE_PUSHED;
      Invalidate(ww->m_hWnd);
    } else if ( ( sbar->m_uButton2State & UISTATE_PUSHED ) != 0 ) {
      sbar->m_uButton2State &= ~UISTATE_PUSHED;
      Invalidate(ww->m_hWnd);
    }
    return 0;
  }
  if ( event->Type == UIEVENT_MOUSEMOVE ) {
    if ( ( sbar->m_uThumbState & UISTATE_CAPTURED ) != 0 ) {
      if ( !sbar->m_bHorizontal ) {
        sbar->m_nLastScrollOffset = ( event->ptMouse.y - sbar->ptLastMouse.y ) * sbar->m_nRange / \
          ( ctr->m_rcItem.bottom - ctr->m_rcItem.top - sbar->m_rcThumb.bottom + sbar->m_rcThumb.top - 2 * ctr->m_cxyFixed.cx );
      } else {
        sbar->m_nLastScrollOffset = ( event->ptMouse.x - sbar->ptLastMouse.x ) * sbar->m_nRange / \
          ( ctr->m_rcItem.right - ctr->m_rcItem.left - sbar->m_rcThumb.right + sbar->m_rcThumb.left - 2 * ctr->m_cxyFixed.cy );
      }
      //UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nLastScrollPos + sbar->m_nLastScrollOffset );
    } else {
      if ( ( sbar->m_uThumbState & UISTATE_HOT ) != 0 ) {
        if ( !PtInRect( &sbar->m_rcThumb, event->ptMouse ) ) {
          sbar->m_uThumbState &= ~UISTATE_HOT;
          Invalidate(ww->m_hWnd);
        }
      } else {
        if ( !IsEnabled(ctr) )
          return 0;
        if ( PtInRect( &sbar->m_rcThumb, event->ptMouse ) ) {
          sbar->m_uThumbState |= UISTATE_HOT;
          Invalidate(ww->m_hWnd);
        }
      }
    }
    return 0;
  }
  if ( event->Type == UIEVENT_TIMER && event->wParam == DEFAULT_TIMERID ) {
    ++sbar->m_nScrollRepeatDelay;
    if ( ( sbar->m_uThumbState & UISTATE_CAPTURED ) != 0 ) {
      int cx, cy;
      SIZE sz;
      if ( !sbar->m_bHorizontal ) {
        if ( ctr->m_pParent != NULL ) {
          cx = CContainerUI_GetScrollPos(ctr->m_pParent).cx;
          sz = cSize( cx, sbar->m_nLastScrollPos + sbar->m_nLastScrollOffset );
          CContainerUI_SetScrollPos( ww, ctr->m_pParent, sz );
        } else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nLastScrollPos + sbar->m_nLastScrollOffset );
      } else {
        if ( ctr->m_pParent != NULL ) {
          cy = CContainerUI_GetScrollPos(ctr->m_pParent).cy;
          sz = cSize( sbar->m_nLastScrollPos + sbar->m_nLastScrollOffset, cy );
          CContainerUI_SetScrollPos( ww, ctr->m_pParent, sz );
        } else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nLastScrollPos + sbar->m_nLastScrollOffset );
      }
      Invalidate(ww->m_hWnd);
    } else if ( ( sbar->m_uButton1State & UISTATE_PUSHED ) != 0 ) {
      if ( sbar->m_nScrollRepeatDelay <= 5 )
        return 0;
      if ( !sbar->m_bHorizontal ) {
        if ( ctr->m_pParent != NULL )
          CContainerUI_LineUp(ww, ctr->m_pParent);
        else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos - sbar->m_nLineSize );
      } else {
        if ( ctr->m_pParent != NULL )
          CContainerUI_LineLeft(ww, ctr->m_pParent);
        else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos - sbar->m_nLineSize );
      }
    } else if ( ( sbar->m_uButton2State & UISTATE_PUSHED ) != 0 ) {
      if ( sbar->m_nScrollRepeatDelay <= 5 )
        return 0;
      if ( !sbar->m_bHorizontal ) {
        if ( ctr->m_pParent != NULL )
          CContainerUI_LineDown(ww, ctr->m_pParent);
        else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos + sbar->m_nLineSize );
      } else {
        if ( ctr->m_pParent != NULL )
          CContainerUI_LineRight(ww, ctr->m_pParent);
        else
        UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos + sbar->m_nLineSize );
      }
    } else {
      POINT pt = { 0 };
      if ( sbar->m_nScrollRepeatDelay <= 5 )
        return 0;
      GetCursorPos( &pt );
      ScreenToClient( GetPaintWindow(ww), &pt );
      if ( !sbar->m_bHorizontal ) {
        if ( pt.y < sbar->m_rcThumb.top ) {
          if ( ctr->m_pParent != NULL )
            CContainerUI_PageUp(ww, ctr->m_pParent);
          else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos + ctr->m_rcItem.top - ctr->m_rcItem.bottom );
        } else if ( pt.y > sbar->m_rcThumb.bottom ) {
          if ( ctr->m_pParent != NULL )
            CContainerUI_PageDown(ww, ctr->m_pParent);
          else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos - ctr->m_rcItem.top + ctr->m_rcItem.bottom );
        }
      } else {
        if ( pt.x < sbar->m_rcThumb.left ) {
          if ( ctr->m_pParent != NULL )
            CContainerUI_PageLeft(ww, ctr->m_pParent);
          else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos + ctr->m_rcItem.left - ctr->m_rcItem.right );
        } else if ( pt.x > sbar->m_rcThumb.right ) {
          if ( ctr->m_pParent != NULL )
            CContainerUI_PageRight(ww, ctr->m_pParent);
          else
          UIScrollbarBase_SetScrollPos( ww, ctr, sbar->m_nScrollPos - ctr->m_rcItem.left + ctr->m_rcItem.right );
        }
      }
    }
    return 0;
  }
  if ( event->Type == UIEVENT_MOUSEENTER ) {
    if ( IsEnabled(ctr) ) {
      sbar->m_uButton1State |= UISTATE_HOT;
      sbar->m_uButton2State |= UISTATE_HOT;
      if ( PtInRect( &sbar->m_rcThumb, event->ptMouse ) )
        sbar->m_uThumbState |= UISTATE_HOT;
      Invalidate(ww->m_hWnd);
    }
    return 0;
  }
  if ( event->Type == UIEVENT_MOUSELEAVE ) {
    if ( IsEnabled(ctr) ) {
      sbar->m_uButton1State &= ~UISTATE_HOT;
      sbar->m_uButton2State &= ~UISTATE_HOT;
      sbar->m_uThumbState &= ~UISTATE_HOT;
      Invalidate(ww->m_hWnd);
    }
    return 0;
  }
  //if ( ctr->m_pParent != NULL )
  //  Event( ctr->m_pParent, *event );
  //else
  //CControlUI_Event( ww, ctr, event );
  return 0;
}
int UIScrollbarBase_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  if ( _tcscmp( pstrName, _T( "hor" ) ) == 0 )
    UIScrollbarBase_SetHorizontal( ctr, _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  else if ( _tcscmp( pstrName, _T( "linesize" ) ) == 0 )
    ( sbar->m_nLineSize = _ttoi( pstrValue ) );
  //else if ( _tcscmp( pstrName, _T( "range" ) ) == 0 )
  //  SetScrollRange( _ttoi( pstrValue ) );
  //else if ( _tcscmp( pstrName, _T( "value" ) ) == 0 )
  //  SetScrollPos( _ttoi( pstrValue ) );
  else
    CControlUI_SetAttribute( res, ctr, pstrName, pstrValue );
  return 0;
}
int UIScrollbarBase_PaintBk( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  UIResource* res = ww->m_pResource;
  COLORREF crCheck1   = GetSBForeColor();
  COLORREF crCheck2   = GetSBBackColor();
  if ( !IsEnabled(ctr) )
    sbar->m_uThumbState |= UISTATE_DISABLED;
  else
    sbar->m_uThumbState &= ~ UISTATE_DISABLED;
  
  DrawCheckedRect(hDC, &ctr->m_rcItem, crCheck1, crCheck2);
  return 0;
}
int UIScrollbarBase_PaintButton1( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  UIResource* res = ww->m_pResource;
  RECT rc = sbar->m_rcButton1;
  int pushed = sbar->m_uButton1State&UISTATE_PUSHED;
  int hot = 0;//sbar->m_uButton1State&UISTATE_HOT;
  if ( !IsEnabled(ctr) )
    sbar->m_uButton1State |= UISTATE_DISABLED;
  else
    sbar->m_uButton1State &= ~ UISTATE_DISABLED;
  
  //RectOffSet(rc, -ctr->m_rcItem.left, -ctr->m_rcItem.top);
  DrawScrollArrow(SB_VERT, 0, hDC, &rc, DFCS_SCROLLUP, pushed, hot);
  return 0;
}
int UIScrollbarBase_PaintButton2( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  UIResource* res = ww->m_pResource;
  RECT rc = sbar->m_rcButton2;
  int pushed = sbar->m_uButton2State&UISTATE_PUSHED;
  int hot = sbar->m_uButton2State&UISTATE_HOT;
  if ( !IsEnabled(ctr) )
    sbar->m_uButton2State |= UISTATE_DISABLED;
  else
    sbar->m_uButton2State &= ~ UISTATE_DISABLED;
  
  //RectOffSet(rc, -ctr->m_rcItem.left, -ctr->m_rcItem.top);
  DrawScrollArrow(SB_VERT, 0, hDC, &rc, DFCS_SCROLLDOWN, pushed, hot);
  return 0;
}
int UIScrollbarBase_PaintThumb( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  UIResource* res = ww->m_pResource;
  RECT rc = sbar->m_rcThumb;
  if ( sbar->m_rcThumb.left == 0 && sbar->m_rcThumb.top == 0 && sbar->m_rcThumb.right == 0 && sbar->m_rcThumb.bottom == 0 )
    return 0;
  if ( !IsEnabled(ctr) )
    sbar->m_uThumbState |= UISTATE_DISABLED;
  else
    sbar->m_uThumbState &= ~ UISTATE_DISABLED;
  
  //RectOffSet(rc, -ctr->m_rcItem.left, -ctr->m_rcItem.top);
  DrawBlankButton(hDC, &rc, 0);
  return 0;
}
int UIScrollbarBase_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  UIScrollbarBase* sbar = (UIScrollbarBase*)ctr;
  //if ( !IntersectRect( &sbar->m_rcPaint, &rcPaint, &ctr->m_rcItem ) )
  //  return 0;
  UIScrollbarBase_PaintBk( ww, ctr, hDC );
  UIScrollbarBase_PaintButton1( ww, ctr, hDC );
  UIScrollbarBase_PaintButton2( ww, ctr, hDC );
  UIScrollbarBase_PaintThumb( ww, ctr, hDC );
  return 0;
}


