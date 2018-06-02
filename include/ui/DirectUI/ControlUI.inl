
typedef CControlUI_vtable* ControlType;
ControlType g_Ctrls[1000];
static int g_CtrlsCount=0;
ControlType* GetRegisteredControl(UINT i) {
  return i<1000 ? g_Ctrls+i : NULL;
}
ControlType* FindRegisteredControl(const char* Name) {
  int i;
  for (i=0; i<g_CtrlsCount; ++i) {
    if (0==stricmp(g_Ctrls[i]->Name, Name)) return g_Ctrls+i;
  }
  return NULL;
}
int RegisterControl(ControlType type) {
  ControlType* ctrtype = FindRegisteredControl(type->Name);
  if (NULL==ctrtype) {
    ASSERT(g_CtrlsCount<1000);
    ctrtype = g_Ctrls+g_CtrlsCount;
    g_Ctrls[g_CtrlsCount++] = type;
  }
  return ctrtype!=NULL;
}
int CControlUI_CtrInit(CControlUI* ctr) {
  ctr->m_pParent=( NULL );
  ctr->m_bUpdateNeeded=( true );
  ctr->m_bVisible=( true );
  ctr->m_bInternVisible=( true );
  ctr->m_bFocused=( false );
  ctr->m_bEnabled=( true );
  ctr->m_bMouseEnabled=( true );
  ctr->m_bFloat=( false );
  ctr->m_bFloatSetPos=( false );
  ctr->m_chShortcut=( '\0' );
  ctr->m_pTag=( 0 );
  ctr->m_dwBackColor=RGB( 255, 255, 255 );
  ctr->m_dwBackColor2=RGB( 255, 255, 255 );
  ctr->m_dwBorderColor=( 0 );
  ctr->m_nBorderSize=( 1 );
  ctr->m_cXY.cx = ctr->m_cXY.cy = 0;
  ctr->m_cxyFixed.cx = ctr->m_cxyFixed.cy = 0;
  ctr->m_cxyMin.cx = ctr->m_cxyMin.cy = 0;
  ctr->m_cxyMax.cx = ctr->m_cxyMax.cy = 9999;
  //vec_new(ctr->m_items2, 0);
  ctr->m_items = CreatePtrArray(0);
  return 0;
}
CControlUI* FindControlByPoint2(CWindowWnd* ww, CControlUI* ctr, POINT pt);
int SendNotify( CWindowWnd* ww, CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ );
int CControlUI_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event ) {
  if ( event->Type == UIEVENT_FIND_CONTROL_BY_POINT && PtInRect(&ctr->m_rcItem, event->ptMouse)) {
    int it;
    event->pSender = ctr;
    for ( it = 0; it < PtrArray_GetSize(ctr->m_items); it++ ) {
      CControlUI* child = (CControlUI*)PtrArray_GetAt(ctr->m_items, it );
      CControlUI* ret = FindControlByPoint2(ww, child, event->ptMouse);
      if (NULL!=ret) {
        event->pSender = ret;
        return 0;
      }
    }
    return 0;
  }
  if ( event->Type == UIEVENT_SETCURSOR ) {
    SetCursor( LoadCursor( NULL, MAKEINTRESOURCE( IDC_ARROW ) ) );
    return 0;
  }
  if ( event->Type == UIEVENT_SETFOCUS ) {
    ctr->m_bFocused = true;
    Invalidate(ww->m_hWnd);
    return 0;
  }
  if ( event->Type == UIEVENT_KILLFOCUS ) {
    ctr->m_bFocused = false;
    Invalidate(ww->m_hWnd);
    return 0;
  }
  if ( event->Type == UIEVENT_TIMER ) {
    //SendNotify( ww, ctr, _T( "timer" ), event->wParam, event->lParam );
    return 0;
  }
  if ( ctr->m_pParent != NULL )
    Event( ctr->m_pParent, *event );
  return 0;
}
int CControlUI_SetPos( CWindowWnd* ww, CControlUI* ctr, RECT rc ) {
  RECT invalidateRc = ctr->m_rcItem;
  CControlUI* pParent = ctr->m_pParent;
  if ( rc.right < rc.left )
    rc.right = rc.left;
  if ( rc.bottom < rc.top )
    rc.bottom = rc.top;
  if ( IsRectEmpty( &invalidateRc ) )
    invalidateRc = rc;
  ctr->m_rcItem = rc;
  if ( ww == NULL )
    return 0;
  if ( ctr->m_bFloat ) {
    if ( !ctr->m_bFloatSetPos ) {
      ctr->m_bFloatSetPos = true;
      SendNotify( ww, ctr, _T( "setpos" ), 0, 0 );
      ctr->m_bFloatSetPos = false;
    }
    if ( pParent != NULL ) {
      RECT rcParentPos = GetPos(pParent);
      if ( ctr->m_cXY.cx >= 0 )
        ctr->m_cXY.cx = ctr->m_rcItem.left - rcParentPos.left;
      else
        ctr->m_cXY.cx = ctr->m_rcItem.right - rcParentPos.right;
      if ( ctr->m_cXY.cy >= 0 )
        ctr->m_cXY.cy = ctr->m_rcItem.top - rcParentPos.top;
      else
        ctr->m_cXY.cy = ctr->m_rcItem.bottom - rcParentPos.bottom;
    }
  }
  //ctr->m_bUpdateNeeded = false;
  // NOTE: SetPos() is usually called during the WM_PAINT cycle where all controls are
  //       being laid out. Calling UpdateLayout() again would be wrong. Refreshing the
  //       window won't hurt (if we're already inside WM_PAINT we'll just validate it out).
  RectJoin( invalidateRc, ctr->m_rcItem );
  {
    CControlUI* pParent = ctr;
    RECT rcTemp;
    RECT rcParent;
    while ( pParent = pParent->m_pParent ) {
      rcTemp = invalidateRc;
      rcParent = GetPos(pParent);
      if ( !IntersectRect( &invalidateRc, &rcTemp, &rcParent ) ) {
        return 0;
      }
    }
  }
  InvalidateRect( ww->m_hWnd, &invalidateRc, FALSE );
  return 0;
}
int CControlUI_CtrFree(CControlUI* ctr) {
  //ctr->m_pParent->
  if (NULL!=ctr->m_items) {
    int i=0;
    for (; i<PtrArray_GetSize(ctr->m_items); ++i) {
      CControlUI* ch = (CControlUI*)PtrArray_GetAt(ctr->m_items, i);
      CtrFree(ch);
    }
    PtrArray_Release(ctr->m_items);
    ctr->m_items = NULL;
  }
  free_s(ctr->m_sName);
  free_s(ctr->m_sText);
  free_s(ctr->m_sToolTip);
  free_s(ctr->m_sUserData);
  free_s(ctr->m_sBkImage);
  free(ctr);
  return 0;
}
CControlUI* CreateControl(const char* Name) {
  ControlType* ctrtype = FindRegisteredControl(Name);
  CControlUI* ctr = NULL;
  if (ctrtype[0]!=NULL) {
    ctr = (CControlUI*)malloc(ctrtype[0]->Size);
    memset(ctr, 0, ctrtype[0]->Size);
    ctr->vtable = ctrtype[0];
    CtrInit(ctr);
  }
  return ctr;
}

// ±³¾°ÑÕÉ«
void CControlUI_PaintBkColor( CControlUI* ctr, HDC hDC ) {
  if ( ctr->m_dwBackColor != 0 ) {
    if ( ctr->m_dwBackColor2 != 0 )
      DrawGradient( hDC, ctr->m_rcItem, ctr->m_dwBackColor, ctr->m_dwBackColor2, true, 16 );
    else if ( ctr->m_dwBackColor >= 0xFF000000 )
      DrawColor( hDC, ctr->m_rcPaint, ctr->m_dwBackColor );
    else
      DrawColor( hDC, ctr->m_rcItem, ctr->m_dwBackColor );
  }
}
bool CWindowWnd_DrawImage( UIResource* res, CControlUI* ctr, HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify ) {
  ctr->m_rcPaint = ctr->m_rcItem;
  return DrawImageString( hDC, res, ctr->m_rcItem, ctr->m_rcPaint, pStrImage, pStrModify );
}
// ±³¾°Í¼
void CControlUI_PaintBkImage( UIResource* res, CControlUI* ctr, HDC hDC ) {
  if ( NULL==ctr->m_sBkImage )
    return ;
  if ( !CWindowWnd_DrawImage( res, ctr, hDC, ( LPCTSTR ) ctr->m_sBkImage, NULL ) )
    DEL(ctr->m_sBkImage);
}
// ±ß½ç
void CControlUI_PaintBorder( CControlUI* ctr, HDC hDC ) {
  if ( ctr->m_dwBorderColor != 0 && ctr->m_nBorderSize > 0 )
    UIDrawRect( hDC, ctr->m_rcItem, ctr->m_nBorderSize, ctr->m_dwBorderColor );
}
int CControlUI_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  UIResource* res = ww->m_pResource;
  if ( !IntersectRect( &ctr->m_rcPaint, &rcPaint, &ctr->m_rcItem ) )
    return 0;
  // »æÖÆÑ­Ðò£º±³¾°ÑÕÉ«->±³¾°Í¼->×´Ì¬Í¼->ÎÄ±¾
  CControlUI_PaintBkColor( ctr, hDC );
  CControlUI_PaintBkImage( res, ctr, hDC );
  CControlUI_PaintBorder( ctr, hDC );
  return 0;
}
int CControlUI_GetItemIndex( CControlUI* ctr, CControlUI* pControl ) {
  int it;
  for ( it = 0; it < PtrArray_GetSize(ctr->m_items); it++ ) {
    if ( (CControlUI*)PtrArray_GetAt(ctr->m_items, it ) == pControl ) {
      return it;
    }
  }
  return -1;
}
int UINeedUpdate(CWindowWnd* ww, CControlUI* ctr) {
  if (ww) {
    Invalidate(ww->m_hWnd);
  }
  if (ctr) {
    if (0==ctr->m_bUpdateNeeded) {
      ctr->m_bUpdateNeeded = 1;
    }
  }
  return 0;
}
int CControlUI_AddLast(CControlUI* ctr, CControlUI* child) {
  ASSERT( ctr );
  ASSERT( child );
  ASSERT( ctr->m_items );
  if (ctr->m_items==NULL) {
    return false;
  }
  if ( child == NULL )
    return false;
  //if ( m_pManager != NULL )
  //  m_pManager->InitControls( child, this );
  NeedUpdate(0, ctr);
  if ( CControlUI_GetItemIndex( ctr, child ) == -1 ) {
    child->m_pParent = ctr;
    return PtrArray_Add(ctr->m_items, child);
  } else {
    return false;
  }
  return 1;
}

int CControlUI_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  LPTSTR pstr;
  DWORD clrColor;
  if ( _tcscmp( pstrName, _T( "pos" ) ) == 0 ) {
    RECT rcPos = { 0 };
    SIZE szXY;
    LPTSTR pstr = NULL;
    rcPos.left = _tcstol( pstrValue, &pstr, 10 );
    ASSERT( pstr );
    rcPos.top = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    rcPos.right = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    rcPos.bottom = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    szXY.cx = rcPos.left >= 0 ? rcPos.left : rcPos.right;
    szXY.cy = rcPos.top >= 0 ? rcPos.top : rcPos.bottom;
    ctr->m_cXY = ( szXY );
    ctr->m_cxyFixed.cx = ( rcPos.right - rcPos.left );
    ctr->m_cxyFixed.cy = ( rcPos.bottom - rcPos.top );
  } else if ( _tcscmp( pstrName, _T( "padding" ) ) == 0 ) {
    RECT rcPadding = { 0 };
    LPTSTR pstr = NULL;
    rcPadding.left = _tcstol( pstrValue, &pstr, 10 );
    ASSERT( pstr );
    rcPadding.top = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    rcPadding.right = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    rcPadding.bottom = _tcstol( pstr + 1, &pstr, 10 );
    ASSERT( pstr );
    ctr->m_rcPadding = rcPadding;
  } else if ( _tcscmp( pstrName, _T( "bkcolor" ) ) == 0 ) {
    if ( *pstrValue == _T( '#' ) )
      pstrValue = CharNext( pstrValue );
    pstr = NULL;
    clrColor = _tcstoul( pstrValue, &pstr, 16 );
    ctr->m_dwBackColor = clrColor;
  } else if ( _tcscmp( pstrName, _T( "bkcolor2" ) ) == 0 ) {
    if ( *pstrValue == _T( '#' ) )
      pstrValue = CharNext( pstrValue );
    clrColor = _tcstoul( pstrValue, &pstr, 16 );
    ctr->m_dwBackColor2 = clrColor;
  } else if ( _tcscmp( pstrName, _T( "bordercolor" ) ) == 0 ) {
    if ( *pstrValue == _T( '#' ) )
      pstrValue = CharNext( pstrValue );
    clrColor = _tcstoul( pstrValue, &pstr, 16 );
    ctr->m_dwBorderColor = clrColor;
  } else if ( _tcscmp( pstrName, _T( "bordersize" ) ) == 0 )
    ctr->m_nBorderSize = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "bkimage" ) ) == 0 )
    setstr( ctr->m_sBkImage, pstrValue );
  else if ( _tcscmp( pstrName, _T( "width" ) ) == 0 )
    ctr->m_cxyFixed.cx = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "height" ) ) == 0 )
    ctr->m_cxyFixed.cy = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "minwidth" ) ) == 0 )
    ctr->m_cxyMin.cx = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "minheight" ) ) == 0 )
    ctr->m_cxyMin.cy = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "maxwidth" ) ) == 0 )
    ctr->m_cxyMax.cx = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "maxheight" ) ) == 0 )
    ctr->m_cxyMax.cy = ( _ttoi( pstrValue ) );
  else if ( _tcscmp( pstrName, _T( "name" ) ) == 0 ) 
    setstr( ctr->m_sName, pstrValue );
  else if ( _tcscmp( pstrName, _T( "text" ) ) == 0 )
    setstr( ctr->m_sText, pstrValue );
  else if ( _tcscmp( pstrName, _T( "tooltip" ) ) == 0 )
    setstr( ctr->m_sToolTip, pstrValue );
  else if ( _tcscmp( pstrName, _T( "userdata" ) ) == 0 )
    setstr( ctr->m_sUserData, pstrValue );
  else if ( _tcscmp( pstrName, _T( "enabled" ) ) == 0 )
    ctr->m_bEnabled = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  else if ( _tcscmp( pstrName, _T( "mouse" ) ) == 0 )
    ctr->m_bMouseEnabled = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  else if ( _tcscmp( pstrName, _T( "visible" ) ) == 0 )
    ctr->m_bVisible = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  else if ( _tcscmp( pstrName, _T( "float" ) ) == 0 )
    ctr->m_bFloat = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  else if ( _tcscmp( pstrName, _T( "shortcut" ) ) == 0 )
    ctr->m_chShortcut = ( pstrValue[ 0 ] );
  return 0;
}