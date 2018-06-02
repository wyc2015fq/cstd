
#define UIScrollbar_CtrInit UIScrollbarBase_CtrInit
#define UIScrollbar_SetHorizontal UIScrollbarBase_SetHorizontal
#define UIScrollbar_SetPos UIScrollbarBase_SetPos
#define UIScrollbar_SetScrollRange UIScrollbarBase_SetScrollRange
#define UIScrollbar_SetScrollPos UIScrollbarBase_SetScrollPos
#define UIScrollbar_Event UIScrollbarBase_Event

int UIScrollbar_CtrFree(CControlUI* ctr) {
  UIScrollbar* sbar = (UIScrollbar*)ctr;
  free_s( sbar->m_sBkNormalImage);
  free_s( sbar->m_sBkHotImage);
  free_s( sbar->m_sBkPushedImage);
  free_s( sbar->m_sBkDisabledImage);
  free_s( sbar->m_sButton1NormalImage);
  free_s( sbar->m_sButton1HotImage);
  free_s( sbar->m_sButton1PushedImage);
  free_s( sbar->m_sButton1DisabledImage);
  free_s( sbar->m_sButton2NormalImage);
  free_s( sbar->m_sButton2HotImage);
  free_s( sbar->m_sButton2PushedImage);
  free_s( sbar->m_sButton2DisabledImage);
  free_s( sbar->m_sThumbNormalImage);
  free_s( sbar->m_sThumbHotImage);
  free_s( sbar->m_sThumbPushedImage);
  free_s( sbar->m_sThumbDisabledImage);
  free_s( sbar->m_sRailNormalImage);
  free_s( sbar->m_sRailHotImage);
  free_s( sbar->m_sRailPushedImage);
  free_s( sbar->m_sRailDisabledImage);
  free_s( sbar->m_sImageModify);
  CControlUI_CtrFree(ctr);
  return 0;
}

int UIScrollbar_SetAttribute2(UIResource* res, CControlUI* ctr, LPCTSTR pStrControlName) {
  if ( res ) {
    LPCTSTR pDefaultAttributes = UIResource_GetDefaultAttributeList( res, pStrControlName );
    if ( pDefaultAttributes ) {
      CControlUI_ApplyAttributeList( res, ctr, pDefaultAttributes );
    }
  }
  return 0;
}
int UIScrollbar_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  UIScrollbar* sbar = (UIScrollbar*)ctr;
  if ( _tcscmp( pstrName, _T( "button1normalimage" ) ) == 0 )
    setstr( sbar->m_sButton1NormalImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "button1hotimage" ) ) == 0 )
    setstr(sbar->m_sButton1HotImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "button1pushedimage" ) ) == 0 )
    setstr( sbar->m_sButton1PushedImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "button1disabledimage" ) ) == 0 )
    setstr( sbar->m_sButton1DisabledImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "button2normalimage" ) ) == 0 )
    setstr( sbar->m_sButton2NormalImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "button2hotimage" ) ) == 0 )
    setstr( sbar->m_sButton2HotImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "button2pushedimage" ) ) == 0 )
    setstr( sbar->m_sButton2PushedImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "button2disabledimage" ) ) == 0 )
    setstr( sbar->m_sButton2DisabledImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "thumbnormalimage" ) ) == 0 )
    setstr( sbar->m_sThumbNormalImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "thumbhotimage" ) ) == 0 )
    setstr( sbar->m_sThumbHotImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "thumbpushedimage" ) ) == 0 )
    setstr( sbar->m_sThumbPushedImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "thumbdisabledimage" ) ) == 0 )
    setstr( sbar->m_sThumbDisabledImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "railnormalimage" ) ) == 0 )
    setstr( sbar->m_sRailNormalImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "railhotimage" ) ) == 0 )
    setstr( sbar->m_sRailHotImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "railpushedimage" ) ) == 0 )
    setstr( sbar->m_sRailPushedImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "raildisabledimage" ) ) == 0 )
    setstr( sbar->m_sRailDisabledImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "bknormalimage" ) ) == 0 )
    setstr( sbar->m_sBkNormalImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "bkhotimage" ) ) == 0 )
    setstr( sbar->m_sBkHotImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "bkpushedimage" ) ) == 0 )
    setstr( sbar->m_sBkPushedImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "bkdisabledimage" ) ) == 0 )
    setstr( sbar->m_sBkDisabledImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "hor" ) ) == 0 )
    UIScrollbar_SetHorizontal( ctr, _tcscmp( pstrValue, _T( "true" ) ) == 0 );
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
int UIScrollbar_PaintBk( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbar* sbar = (UIScrollbar*)ctr;
  UIResource* res = ww->m_pResource;
  if ( !IsEnabled(ctr) )
    sbar->m_uThumbState |= UISTATE_DISABLED;
  else
    sbar->m_uThumbState &= ~ UISTATE_DISABLED;
  if ( ( sbar->m_uThumbState & UISTATE_DISABLED ) != 0 ) {
    if ( !IsEmpty(sbar->m_sBkDisabledImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sBkDisabledImage, NULL ) )
        free_s(sbar->m_sBkDisabledImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uThumbState & UISTATE_PUSHED ) != 0 ) {
    if ( !IsEmpty(sbar->m_sBkPushedImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sBkPushedImage, NULL ) )
        free_s(sbar->m_sBkPushedImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uThumbState & UISTATE_HOT ) != 0 ) {
    if ( !IsEmpty(sbar->m_sBkHotImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sBkHotImage, NULL ) )
        free_s(sbar->m_sBkHotImage);
      else
        return 0;
    }
  }
  if ( !IsEmpty(sbar->m_sBkNormalImage) ) {
    if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sBkNormalImage, NULL ) )
      free_s(sbar->m_sBkNormalImage);
    else
      return 0;
  }
  return 0;
}
int UIScrollbar_PaintButton1( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbar* sbar = (UIScrollbar*)ctr;
  UIResource* res = ww->m_pResource;
  //RECT rc = sbar->m_rcButton1;
  if ( !IsEnabled(ctr) )
    sbar->m_uButton1State |= UISTATE_DISABLED;
  else
    sbar->m_uButton1State &= ~ UISTATE_DISABLED;
  free_s(sbar->m_sImageModify);
  sbar->m_sImageModify = SmallFormat( _T( "dest='%d,%d,%d,%d'" ), sbar->m_rcButton1.left - ctr->m_rcItem.left, \
    sbar->m_rcButton1.top - ctr->m_rcItem.top, sbar->m_rcButton1.right - ctr->m_rcItem.left, sbar->m_rcButton1.bottom - ctr->m_rcItem.top );
  if ( ( sbar->m_uButton1State & UISTATE_DISABLED ) != 0 ) {
    if ( !IsEmpty(sbar->m_sButton1DisabledImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sButton1DisabledImage, sbar->m_sImageModify ) )
        free_s(sbar->m_sButton1DisabledImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uButton1State & UISTATE_PUSHED ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sButton1PushedImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sButton1PushedImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sButton1PushedImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uButton1State & UISTATE_HOT ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sButton1HotImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sButton1HotImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sButton1HotImage);
      else
        return 0;
    }
  }
  if ( ! IsEmpty(sbar->m_sButton1NormalImage) ) {
    if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sButton1NormalImage, sbar->m_sImageModify ) )
       free_s(sbar->m_sButton1NormalImage);
    else
      return 0;
  }
  {DWORD dwBorderColor = 0xFF85E4FF;
  int nBorderSize = 2;
  UIDrawRect( hDC, sbar->m_rcButton1, nBorderSize, dwBorderColor );}
  return 0;
}
int UIScrollbar_PaintButton2( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbar* sbar = (UIScrollbar*)ctr;
  UIResource* res = ww->m_pResource;
  if ( !IsEnabled(ctr) )
    sbar->m_uButton2State |= UISTATE_DISABLED;
  else
    sbar->m_uButton2State &= ~ UISTATE_DISABLED;
  free_s(sbar->m_sImageModify);
  sbar->m_sImageModify=SmallFormat( _T( "dest='%d,%d,%d,%d'" ), sbar->m_rcButton2.left - ctr->m_rcItem.left, \
    sbar->m_rcButton2.top - ctr->m_rcItem.top, sbar->m_rcButton2.right - ctr->m_rcItem.left, sbar->m_rcButton2.bottom - ctr->m_rcItem.top );
  if ( ( sbar->m_uButton2State & UISTATE_DISABLED ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sButton2DisabledImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sButton2DisabledImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sButton2DisabledImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uButton2State & UISTATE_PUSHED ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sButton2PushedImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sButton2PushedImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sButton2PushedImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uButton2State & UISTATE_HOT ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sButton2HotImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sButton2HotImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sButton2HotImage);
      else
        return 0;
    }
  }
  if ( ! IsEmpty(sbar->m_sButton2NormalImage) ) {
    if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sButton2NormalImage, sbar->m_sImageModify ) )
       free_s(sbar->m_sButton2NormalImage);
    else
      return 0;
  }
  {DWORD dwBorderColor = 0xFF85E4FF;
  int nBorderSize = 2;
  UIDrawRect( hDC, sbar->m_rcButton2, nBorderSize, dwBorderColor );}
  return 0;
}
int UIScrollbar_PaintThumb( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbar* sbar = (UIScrollbar*)ctr;
  UIResource* res = ww->m_pResource;
  if ( sbar->m_rcThumb.left == 0 && sbar->m_rcThumb.top == 0 && sbar->m_rcThumb.right == 0 && sbar->m_rcThumb.bottom == 0 )
    return 0;
  if ( !IsEnabled(ctr) )
    sbar->m_uThumbState |= UISTATE_DISABLED;
  else
    sbar->m_uThumbState &= ~ UISTATE_DISABLED;
   free_s(sbar->m_sImageModify);
  sbar->m_sImageModify=SmallFormat( _T( "dest='%d,%d,%d,%d'" ), sbar->m_rcThumb.left - ctr->m_rcItem.left, \
    sbar->m_rcThumb.top - ctr->m_rcItem.top, sbar->m_rcThumb.right - ctr->m_rcItem.left, sbar->m_rcThumb.bottom - ctr->m_rcItem.top );
  if ( ( sbar->m_uThumbState & UISTATE_DISABLED ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sThumbDisabledImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sThumbDisabledImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sThumbDisabledImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uThumbState & UISTATE_PUSHED ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sThumbPushedImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sThumbPushedImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sThumbPushedImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uThumbState & UISTATE_HOT ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sThumbHotImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sThumbHotImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sThumbHotImage);
      else
        return 0;
    }
  }
  if ( ! IsEmpty(sbar->m_sThumbNormalImage) ) {
    if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sThumbNormalImage, sbar->m_sImageModify ) )
       free_s(sbar->m_sThumbNormalImage);
    else
      return 0;
  }
  {DWORD dwBorderColor = 0xFF85E4FF;
  int nBorderSize = 2;
  UIDrawRect( hDC, sbar->m_rcThumb, nBorderSize, dwBorderColor );}
  return 0;
}
int UIScrollbar_PaintRail( CWindowWnd* ww, CControlUI* ctr, HDC hDC ) {
  UIScrollbar* sbar = (UIScrollbar*)ctr;
  UIResource* res = ww->m_pResource;
  if ( sbar->m_rcThumb.left == 0 && sbar->m_rcThumb.top == 0 && sbar->m_rcThumb.right == 0 && sbar->m_rcThumb.bottom == 0 )
    return 0;
  if ( !IsEnabled(ctr) )
    sbar->m_uThumbState |= UISTATE_DISABLED;
  else
    sbar->m_uThumbState &= ~ UISTATE_DISABLED;
   free_s(sbar->m_sImageModify);
  if ( !sbar->m_bHorizontal ) {
    sbar->m_sImageModify=SmallFormat( _T( "dest='%d,%d,%d,%d'" ), sbar->m_rcThumb.left - ctr->m_rcItem.left, \
      ( sbar->m_rcThumb.top + sbar->m_rcThumb.bottom ) / 2 - ctr->m_rcItem.top - ctr->m_cxyFixed.cx / 2, \
      sbar->m_rcThumb.right - ctr->m_rcItem.left, \
      ( sbar->m_rcThumb.top + sbar->m_rcThumb.bottom ) / 2 - ctr->m_rcItem.top + ctr->m_cxyFixed.cx - ctr->m_cxyFixed.cx / 2 );
  } else {
    sbar->m_sImageModify=SmallFormat( _T( "dest='%d,%d,%d,%d'" ), \
      ( sbar->m_rcThumb.left + sbar->m_rcThumb.right ) / 2 - ctr->m_rcItem.left - ctr->m_cxyFixed.cy / 2, \
      sbar->m_rcThumb.top - ctr->m_rcItem.top, \
      ( sbar->m_rcThumb.left + sbar->m_rcThumb.right ) / 2 - ctr->m_rcItem.left + ctr->m_cxyFixed.cy - ctr->m_cxyFixed.cy / 2, \
      sbar->m_rcThumb.bottom - ctr->m_rcItem.top );
  }
  if ( ( sbar->m_uThumbState & UISTATE_DISABLED ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sRailDisabledImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sRailDisabledImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sRailDisabledImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uThumbState & UISTATE_PUSHED ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sRailPushedImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sRailPushedImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sRailPushedImage);
      else
        return 0;
    }
  } else if ( ( sbar->m_uThumbState & UISTATE_HOT ) != 0 ) {
    if ( ! IsEmpty(sbar->m_sRailHotImage) ) {
      if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sRailHotImage, sbar->m_sImageModify ) )
         free_s(sbar->m_sRailHotImage);
      else
        return 0;
    }
  }
  if ( ! IsEmpty(sbar->m_sRailNormalImage) ) {
    if ( !CWindowWnd_DrawImage( res, ctr, hDC, sbar->m_sRailNormalImage, sbar->m_sImageModify ) )
       free_s(sbar->m_sRailNormalImage);
    else
      return 0;
  }
  return 0;
}

int UIScrollbar_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  UIScrollbar* sbar = (UIScrollbar*)ctr;
  //if ( !IntersectRect( &sbar->m_rcPaint, &rcPaint, &ctr->m_rcItem ) )
  //  return 0;
  UIScrollbar_PaintBk( ww, ctr, hDC );
  UIScrollbar_PaintButton1( ww, ctr, hDC );
  UIScrollbar_PaintButton2( ww, ctr, hDC );
  UIScrollbar_PaintThumb( ww, ctr, hDC );
  UIScrollbar_PaintRail( ww, ctr, hDC );
  return 0;
}


