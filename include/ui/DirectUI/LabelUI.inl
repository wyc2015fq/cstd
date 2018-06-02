
typedef struct UILabel {
  CControlUI base;
  DWORD m_dwTextColor;
  DWORD m_dwDisabledTextColor;
  int m_iFont;
  UINT m_uTextStyle;
  RECT m_rcTextPadding;
  bool m_bShowHtml;
} UILabel;

int CControlUI_ApplyAttributeList( UIResource* res, CControlUI* ctr, LPCTSTR pstrList ) {
  TCHAR sItem[256]={0};
  TCHAR sValue[256]={0};
  while ( *pstrList != _T( '\0' ) ) {
    sItem[0]=0;
    sValue[0]=0;
    while ( *pstrList != _T( '\0' ) && *pstrList != _T( '=' ) ) {
      LPTSTR pstrTemp = CharNext( pstrList );
      while ( pstrList < pstrTemp ) {
        strcatch(sItem, *pstrList++);
      }
    }
    ASSERT( *pstrList == _T( '=' ) );
    if ( *pstrList++ != _T( '=' ) )
      return 0;
    ASSERT( *pstrList == _T( '\"' ) );
    if ( *pstrList++ != _T( '\"' ) )
      return 0;
    while ( *pstrList != _T( '\0' ) && *pstrList != _T( '\"' ) ) {
      LPTSTR pstrTemp = CharNext( pstrList );
      while ( pstrList < pstrTemp ) {
        strcatch(sValue, *pstrList++);
      }
    }
    ASSERT( *pstrList == _T( '\"' ) );
    if ( *pstrList++ != _T( '\"' ) )
      return 0;
    SetAttribute( res, ctr, sItem, sValue );
    if ( *pstrList++ != _T( ' ' ) )
      return 0;
  }
  return 0;
}
int UILabel_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  UILabel* pLabel = (UILabel*)ctr;
  LPTSTR pstr = NULL;
  DWORD clrColor;
  if ( _tcscmp( pstrName, _T( "align" ) ) == 0 ) {
    if ( _tcsstr( pstrValue, _T( "left" ) ) != NULL ) {
      pLabel->m_uTextStyle &= ~( DT_CENTER | DT_RIGHT );
      pLabel->m_uTextStyle |= DT_LEFT;
    }
    if ( _tcsstr( pstrValue, _T( "center" ) ) != NULL ) {
      pLabel->m_uTextStyle &= ~( DT_LEFT | DT_RIGHT );
      pLabel->m_uTextStyle |= DT_CENTER;
    }
    if ( _tcsstr( pstrValue, _T( "right" ) ) != NULL ) {
      pLabel->m_uTextStyle &= ~( DT_LEFT | DT_CENTER );
      pLabel->m_uTextStyle |= DT_RIGHT;
    }
  } else if ( _tcscmp( pstrName, _T( "font" ) ) == 0 )
    pLabel->m_iFont = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "textcolor" ) ) == 0 ) {
    if ( *pstrValue == _T( '#' ) )
      pstrValue = CharNext( pstrValue );
    clrColor = _tcstoul( pstrValue, &pstr, 16 );
    pLabel->m_dwTextColor = clrColor;
  } else if ( _tcscmp( pstrName, _T( "disabledtextcolor" ) ) == 0 ) {
    if ( *pstrValue == _T( '#' ) )
      pstrValue = CharNext( pstrValue );
    clrColor = _tcstoul( pstrValue, &pstr, 16 );
    pLabel->m_dwDisabledTextColor = clrColor;
  } else if ( _tcscmp( pstrName, _T( "textpadding" ) ) == 0 ) {
    RECT rcTextPadding = { 0 };
    LPTSTR pstr = NULL;
    rcTextPadding.left = _tcstol( pstrValue, &pstr, 10 );
    ASSERT( pstr );
    rcTextPadding.top = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    rcTextPadding.right = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    rcTextPadding.bottom = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    pLabel->m_rcTextPadding = rcTextPadding;
  } else if ( _tcscmp( pstrName, _T( "showhtml" ) ) == 0 ) {
    pLabel->m_bShowHtml = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  } else {
    CControlUI_SetAttribute( res, ctr, pstrName, pstrValue );
  }
  return 0;
}
static int UILabel_PaintText( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcItem ) {
  int nLinks = 0;
  RECT rc = ctr->m_rcItem;
  UILabel* pLabel = (UILabel*)ctr;
  int iFont = pLabel->m_iFont;
  UIResource* res = ww->m_pResource;
  if ( pLabel->m_dwTextColor == 0 )
    pLabel->m_dwTextColor = UIResource_GetDefaultFontColor(res);
  if ( pLabel->m_dwDisabledTextColor == 0 )
    pLabel->m_dwDisabledTextColor = UIResource_GetDefaultDisabledColor(res);
  if ( strlen(ctr->m_sText)==0 )
    return 0;
  rc.left += pLabel->m_rcTextPadding.left;
  rc.right -= pLabel->m_rcTextPadding.right;
  rc.top += pLabel->m_rcTextPadding.top;
  rc.bottom -= pLabel->m_rcTextPadding.bottom;
  CControlUI_PaintBkColor(ctr, hDC);
  CControlUI_PaintBkImage(res, ctr, hDC);
  if ( IsEnabled(ctr) ) {
    //if ( pLabel->m_bShowHtml )
    //  DrawHtmlText( hDC, ww, rc, pLabel->m_sText, pLabel->m_dwTextColor, NULL, NULL, nLinks, DT_SINGLELINE | pLabel->m_uTextStyle );
    //else
      UIDrawText( hDC, res, rc, ctr->m_sText, pLabel->m_dwTextColor, iFont, DT_SINGLELINE | pLabel->m_uTextStyle );
  } else {
    //if ( pLabel->m_bShowHtml )
    //  DrawHtmlText( hDC, ww, rc, pLabel->m_sText, pLabel->m_dwDisabledTextColor, NULL, NULL, nLinks, DT_SINGLELINE | pLabel->m_uTextStyle );
    //else
      UIDrawText( hDC, res, rc, ctr->m_sText, pLabel->m_dwDisabledTextColor, iFont, DT_SINGLELINE | pLabel->m_uTextStyle );
  }
  return 0;
}
