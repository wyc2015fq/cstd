
#define UIHorizontalLayout_CtrFree CContainerUI_CtrFree
#define UIHorizontalLayout_DoPaint CContainerUI_DoPaint
int UIHorizontalLayout_CtrInit(CControlUI* ctr) {
  UIHorizontalLayout* this = (UIHorizontalLayout*)ctr;
  CContainerUI_CtrInit(ctr);
  this->m_iSepWidth=( 0 ), this->m_uButtonState=( 0 ), this->m_bImmMode=( false );
  this->ptLastMouse.x = this->ptLastMouse.y = 0;
  ZeroMemory( &this->m_rcNewPos, sizeof( this->m_rcNewPos ) );
  return 0;
}
UINT UIHorizontalLayout_GetControlFlags(CControlUI* ctr) {
  UIHorizontalLayout* this = (UIHorizontalLayout*)ctr;
  if ( IsEnabled(ctr) && this->m_iSepWidth != 0 )
    return UIFLAG_SETCURSOR;
  else
    return 0;
}
int UIHorizontalLayout_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  UIHorizontalLayout* this = (UIHorizontalLayout*)ctr;
  if ( _tcscmp( pstrName, _T( "sepwidth" ) ) == 0 )
    this->m_iSepWidth = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "sepimm" ) ) == 0 )
    this->m_bImmMode = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  else
    CContainerUI_SetAttribute( res, ctr, pstrName, pstrValue );
  return 0;
}
RECT cRect( int l, int t, int r, int b ) {
  RECT rc; rc.left = l, rc.top = t, rc.right = r, rc.bottom = b; return rc;
}
RECT UIHorizontalLayout_GetThumbRect( CControlUI* ctr, bool bUseNew/* = false */ ) {
  UIHorizontalLayout* this = (UIHorizontalLayout*)ctr;
  if ( ( this->m_uButtonState & UISTATE_CAPTURED ) != 0 && bUseNew ) {
    if ( this->m_iSepWidth >= 0 )
      return cRect( this->m_rcNewPos.right - this->m_iSepWidth, this->m_rcNewPos.top, this->m_rcNewPos.right, this->m_rcNewPos.bottom );
    else
      return cRect( this->m_rcNewPos.left, this->m_rcNewPos.top, this->m_rcNewPos.left - this->m_iSepWidth, this->m_rcNewPos.bottom );
  } else {
    if ( this->m_iSepWidth >= 0 )
      return cRect( ctr->m_rcItem.right - this->m_iSepWidth, ctr->m_rcItem.top, ctr->m_rcItem.right, ctr->m_rcItem.bottom );
    else
      return cRect( ctr->m_rcItem.left, ctr->m_rcItem.top, ctr->m_rcItem.left - this->m_iSepWidth, ctr->m_rcItem.bottom );
  }
}
int UIHorizontalLayout_DoPostPaint( CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  UIHorizontalLayout* this = (UIHorizontalLayout*)ctr;
  if ( ( this->m_uButtonState & UISTATE_CAPTURED ) != 0 && !this->m_bImmMode ) {
    RECT rcSeparator = UIHorizontalLayout_GetThumbRect( ctr, true );
    DrawColor( hDC, rcSeparator, 0xAA000000 );
  }
  return 0;
}
#include "HorizontalLayout2.inl"
