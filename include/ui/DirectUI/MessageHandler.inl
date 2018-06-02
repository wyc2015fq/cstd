

BOOL MessageHandler( CWindowWnd* ww, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plRes ) {
#define lRes *plRes
  //#ifdef _DEBUG
  //    switch( uMsg ) {
  //    case WM_NCPAINT:
  //    case WM_NCHITTEST:
  //    case WM_SETCURSOR:
  //       break;
  //    default:
  //       TRACE(_T("MSG: %-20s (%08ld)"), TRACEMSG(uMsg),  GetTickCount());
  //    }
  //#endif
  // Not ready yet?
  if ( ww->m_hWnd == NULL )
    return false;
  // Cycle through listeners
  //printf("%s\n", __TraceMsg(uMsg));
#if 0
  for ( int i = 0; i < ww->m_aMessageFilters.GetSize(); i++ ) {
    bool bHandled = false;
    LRESULT lResult = static_cast<IMessageFilterUI*>( ww->m_aMessageFilters[ i ] ) ->MessageHandler( uMsg, wParam, lParam, bHandled );
    if ( bHandled ) {
      lRes = lResult;
      return true;
    }
  }
#endif
  // Custom handling of events
  switch ( uMsg ) {
  case WM_APP + 1: {
      // Delayed control-tree cleanup. See AttachDialog() for details.
      //for ( int i = 0; i < ww->m_aDelayedCleanup.GetSize(); i++ )
      //  delete static_cast<CControlUI*>( ww->m_aDelayedCleanup[ i ] );
      //ww->m_aDelayedCleanup.Empty();
    }
    break;
  case WM_CLOSE: 
    {
      HWND hwndParent;
      // Make sure all matching "closing" events are sent
      TEventUI event = { 0 };
      event.ptMouse = ww->m_ptLastMousePos;
      event.dwTimestamp =  GetTickCount();
      if ( ww->m_pEventHover != NULL ) {
        event.Type = UIEVENT_MOUSELEAVE;
        event.pSender = ww->m_pEventHover;
        Event( ww->m_pEventHover, event );
      }
      if ( ww->m_pEventClick != NULL ) {
        event.Type = UIEVENT_BUTTONUP;
        event.pSender = ww->m_pEventClick;
        Event(ww->m_pEventClick,  event );
      }
      UISetFocus( ww, NULL );
      // Hmmph, the usual Windows tricks to avoid
      // focus loss...
      hwndParent = GetWindowOwner( ww->m_hWndPaint );
      if ( hwndParent != NULL )
        SetFocus( hwndParent );
      PostQuitMessage( 0L );
    }
    break;
  case WM_KILLFOCUS: 
    UISetFocus(ww, NULL);
    break;
  case WM_ERASEBKGND: 
    {
      // We'll do the painting here...
      //Invalidate(ww->m_hWnd); //CPU会100%
      lRes = 1;
    }
    return true;
  case WM_PAINT: 
    {
      // Should we paint?
      RECT rcPaint = { 0 };
      if ( ! GetUpdateRect( ww->m_hWndPaint, &rcPaint, FALSE ) )
        return true;
      // Do we need to resize anything?
      // This is the time where we layout the controls on the form.
      // We delay this even from the WM_SIZE messages since resizing can be
      // a very expensize operation.
      if ( ww->m_bUpdateNeeded ) {
        RECT rcClient = { 0 };
        //printf("11111\n");
        ww->m_bUpdateNeeded = false;
        GetClientRect( ww->m_hWndPaint, &rcClient );
        if ( ! IsRectEmpty( &rcClient ) ) {
          SetPos( ww->m_pRoot, rcClient );
          if ( ww->m_hDcOffscreen != NULL )
            DeleteDC( ww->m_hDcOffscreen );
          if ( ww->m_hbmpOffscreen != NULL )
            DeleteObject( ww->m_hbmpOffscreen );
          ww->m_hDcOffscreen = NULL;
          ww->m_hbmpOffscreen = NULL;
          if ( ww->m_pRoot->m_bUpdateNeeded ) {
          } else {
            //CControlUI* pControl = NULL;
            //while ( pControl = ww->m_pRoot->FindControl( __FindControlFromUpdate, NULL, UIFIND_VISIBLE | UIFIND_ME_FIRST ) ) {
            //  pControl->SetPos( pControl->GetPos() );
            //}
          }
          // We'll want to notify the window when it is first initialized
          // with the correct layout. The window form would take the time
          // to submit swipes/animations.
          if ( ww->m_bFirstLayout ) {
            ww->m_bFirstLayout = false;
            SendNotify( ww, ww->m_pRoot, _T( "windowinit" ), 0, 0 );
          }
        }
      }
      // Set focus to first control?
      if ( ww->m_bFocusNeeded ) {
        //SetNextTabControl();
      }
#define m_bOffscreenPaint 1
      // Render screen
      // Prepare offscreen bitmap?
      if ( m_bOffscreenPaint && ww->m_hbmpOffscreen == NULL ) {
        RECT rcClient = { 0 };
        GetClientRect( ww->m_hWndPaint, &rcClient );
        ww->m_hDcOffscreen =  CreateCompatibleDC( ww->m_hDcPaint );
        ww->m_hbmpOffscreen =  CreateCompatibleBitmap( ww->m_hDcPaint, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
        ASSERT( ww->m_hDcOffscreen );
        ASSERT( ww->m_hbmpOffscreen );
      }
      // Begin Windows paint
      if (ww->m_pRoot!=NULL) {
        PAINTSTRUCT ps = { 0 };
        BeginPaint( ww->m_hWndPaint, &ps );
        if ( m_bOffscreenPaint ) {
          //int i;
          // We have an offscreen device to paint on for flickerfree display.
          HBITMAP hOldBitmap = ( HBITMAP ) SelectObject( ww->m_hDcOffscreen, ww->m_hbmpOffscreen );
          // Paint the image on the offscreen bitmap
          int iSaveDC =  SaveDC( ww->m_hDcOffscreen );
          if (ww->m_pRoot->m_bUpdateNeeded) {
            DoPaint(ww->m_pRoot, ww->m_hDcOffscreen, ps.rcPaint );
          }
          RestoreDC( ww->m_hDcOffscreen, iSaveDC );
          //for ( i = 0; i < ww->m_aPostPaintControls.GetSize(); i++ ) {
          //  CControlUI* pPostPaintControl = static_cast<CControlUI*>( ww->m_aPostPaintControls[ i ] );
          //  pPostPaintControl->DoPostPaint( ww->m_hDcOffscreen, ps.rcPaint );
          //}
          // Blit offscreen bitmap back to display
          BitBlt( ps.hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
            ww->m_hDcOffscreen, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY );
          SelectObject( ww->m_hDcOffscreen, hOldBitmap );
          //if ( ww->m_bShowUpdateRect ) {
          //  HPEN hOldPen = ( HPEN ) SelectObject( ps.hdc, ww->m_hUpdateRectPen );
          //  SelectObject( ps.hdc,  GetStockObject( HOLLOW_BRUSH ) );
          //  Rectangle( ps.hdc, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom );
          //  SelectObject( ps.hdc, hOldPen );
          //}
        } else {
          // A standard paint job
          int iSaveDC =  SaveDC( ps.hdc );
          DoPaint(ww->m_pRoot, ps.hdc, ps.rcPaint );
          RestoreDC( ps.hdc, iSaveDC );
        }
        // All Done!
        EndPaint( ww->m_hWndPaint, &ps );
      }
    }
    // If any of the painting requested a resize again, we'll need
    // to invalidate the entire window once more.
    if ( ww->m_bUpdateNeeded ) {
      InvalidateRect( ww->m_hWndPaint, NULL, FALSE );
    }
    return true;
  case WM_PRINTCLIENT: {
      RECT rcClient;
      HDC hDC;
      int save;
      GetClientRect( ww->m_hWndPaint, &rcClient );
      hDC = ( HDC ) wParam;
      save =  SaveDC( hDC );
      DoPaint(ww->m_pRoot, hDC, rcClient );
      // Check for traversing children. The crux is that WM_PRINT will assume
      // that the DC is positioned at frame coordinates and will paint the child
      // control at the wrong position. We'll simulate the entire thing instead.
      if ( ( lParam & PRF_CHILDREN ) != 0 ) {
        HWND hWndChild =  GetWindow( ww->m_hWndPaint, GW_CHILD );
        while ( hWndChild != NULL ) {
          RECT rcPos = { 0 };
           GetWindowRect( hWndChild, &rcPos );
           MapWindowPoints( HWND_DESKTOP, ww->m_hWndPaint, (LPPOINT)( &rcPos ), 2 );
           SetWindowOrgEx( hDC, -rcPos.left, -rcPos.top, NULL );
          // NOTE: We use WM_PRINT here rather than the expected WM_PRINTCLIENT
          //       since the latter will not print the nonclient correctly for
          //       EDIT controls.
           SendMessage( hWndChild, WM_PRINT, wParam, lParam | PRF_NONCLIENT );
          hWndChild =  GetWindow( hWndChild, GW_HWNDNEXT );
        }
      }
      RestoreDC( hDC, save );
    }
    break;
  case WM_GETMINMAXINFO: {
      LPMINMAXINFO lpMMI = ( LPMINMAXINFO ) lParam;
      lpMMI->ptMinTrackSize.x = ww->m_szMinWindow.cx;
      lpMMI->ptMinTrackSize.y = ww->m_szMinWindow.cy;
    }
    break;
  case WM_SIZE: {
      if ( ww->m_pFocus != NULL ) {
        TEventUI event = { 0 };
        event.Type = UIEVENT_WINDOWSIZE;
        event.dwTimestamp =  GetTickCount();
        Event( ww->m_pFocus, event );
      }
      ww->m_bUpdateNeeded=1;
      InvalidateRect(ww->m_hWnd, 0, 0);
      NeedUpdate(ww, 0);
    }
    return true;
  case WM_TIMER: {
    int i;
      for ( i = 0; i < PtrArray_GetSize(ww->m_aTimers); i++ ) {
        const TIMERINFO* pTimer = (const TIMERINFO*)PtrArray_GetAt(ww->m_aTimers, i);
        if ( pTimer->hWnd == ww->m_hWndPaint && pTimer->uWinTimer == LOWORD( wParam ) && pTimer->bKilled == false ) {
          TEventUI event = { 0 };
          event.Type = UIEVENT_TIMER;
          event.wParam = pTimer->nLocalID;
          event.dwTimestamp =  GetTickCount();
          Event( pTimer->pSender, event );
          break;
        }
      }
    }
    break;
  case WM_MOUSEHOVER:
    {
      CControlUI* pHover;
      POINT ptClient, pt = { GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };
      ww->m_bMouseTracking = false;
      pHover = FindControlByPoint( ww->m_pRoot, pt );
      if ( pHover == NULL )
        break;
      // Generate mouse hover event
      if ( ww->m_pEventHover != NULL ) {
        TEventUI event = { 0 };
        event.ptMouse = pt;
        event.Type = UIEVENT_MOUSEHOVER;
        event.pSender = pHover;
        event.dwTimestamp =  GetTickCount();
        Event( ww->m_pEventHover, event );
      }
      // Create tooltip information
      ptClient = pt;
      ScreenToClient( ww->m_hWnd, &ptClient );
      {
        LPCTSTR sToolTip;
        sToolTip = GetToolTip( pHover, ptClient );
        if ( sToolTip==NULL )
          return true;
        ZeroMemory( &ww->m_ToolTip, sizeof( TOOLINFO ) );
        ww->m_ToolTip.cbSize = sizeof( TOOLINFO );
        ww->m_ToolTip.uFlags = TTF_IDISHWND;
        ww->m_ToolTip.hwnd = ww->m_hWndPaint;
        ww->m_ToolTip.uId = ( UINT_PTR ) ww->m_hWndPaint;
        ww->m_ToolTip.hinst = GetResourceInstance();
        ww->m_ToolTip.lpszText = (LPTSTR)( ( LPCTSTR ) sToolTip );
        ww->m_ToolTip.rect = GetPos(pHover);
        if ( ww->m_hwndTooltip == NULL ) {
          ww->m_hwndTooltip =  CreateWindowEx( 0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, ww->m_hWndPaint, NULL, GetResourceInstance(), NULL );
          SendMessage( ww->m_hwndTooltip, TTM_ADDTOOL, 0, ( LPARAM ) & ww->m_ToolTip );
        }
        SendMessage( ww->m_hwndTooltip, TTM_SETTOOLINFO, 0, ( LPARAM ) & ww->m_ToolTip );
        SendMessage( ww->m_hwndTooltip, TTM_TRACKACTIVATE, TRUE, ( LPARAM ) & ww->m_ToolTip );
      }
    }
    return true;
  case WM_MOUSELEAVE:
    {
      if ( ww->m_hwndTooltip != NULL )
         SendMessage( ww->m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, ( LPARAM ) & ww->m_ToolTip );
      if ( ww->m_bMouseTracking )
         SendMessage( ww->m_hWndPaint, WM_MOUSEMOVE, 0, ( LPARAM ) - 1 );
      ww->m_bMouseTracking = false;
    }
    break;
  case WM_MOUSEMOVE:
    {
    POINT pt = { GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };
    CControlUI* pNewHover;
      TEventUI event = { 0 };
      // Start tracking this entire window again...
      if ( !ww->m_bMouseTracking ) {
        TRACKMOUSEEVENT tme = { 0 };
        tme.cbSize = sizeof( TRACKMOUSEEVENT );
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = ww->m_hWndPaint;
        tme.dwHoverTime = ww->m_hwndTooltip == NULL ? 1000UL : ( DWORD )  SendMessage( ww->m_hwndTooltip, TTM_GETDELAYTIME, TTDT_INITIAL, 0L );
        _TrackMouseEvent( &tme );
        ww->m_bMouseTracking = true;
      }
      // Generate the appropriate mouse messages
      ww->m_ptLastMousePos = pt;
      pNewHover = FindControlByPoint( ww->m_pRoot, pt );
      //if ( pNewHover != NULL )
      //  break;
      event.ptMouse = pt;
      event.dwTimestamp =  GetTickCount();
      if ( pNewHover != ww->m_pEventHover && ww->m_pEventHover != NULL ) {
        event.Type = UIEVENT_MOUSELEAVE;
        event.pSender = pNewHover;
        Event( ww->m_pEventHover, event );
        ww->m_pEventHover = NULL;
        if ( ww->m_hwndTooltip != NULL )
           SendMessage( ww->m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, ( LPARAM ) & ww->m_ToolTip );
      }
      if ( pNewHover != ww->m_pEventHover && pNewHover != NULL ) {
        event.Type = UIEVENT_MOUSEENTER;
        event.pSender = ww->m_pEventHover;
        Event(pNewHover, event );
        ww->m_pEventHover = pNewHover;
      }
      if ( ww->m_pEventClick != NULL ) {
        event.Type = UIEVENT_MOUSEMOVE;
        event.pSender = NULL;
        Event(ww->m_pEventClick,  event );
      } else if ( pNewHover != NULL ) {
        event.Type = UIEVENT_MOUSEMOVE;
        event.pSender = NULL;
        Event(pNewHover, event );
      }
    }
    break;
  case WM_LBUTTONDOWN: 
    {
      TEventUI event = { 0 };
      CControlUI* pControl;
      POINT pt = { GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };
      ww->m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
      // We alway set focus back to our app (this helps
      // when Win32 child windows are placed on the dialog
      // and we need to remove them on focus change).
      SetFocus( ww->m_hWndPaint );
      ww->m_ptLastMousePos = pt;
      pControl = FindControlByPoint( ww->m_pRoot, pt );
      UISetFocus(ww, pControl);
      if ( pControl == NULL )
        break;
      //printf("ctr=%s name=%s\n", pControl->vtable->Name, pControl->m_sName);
      ww->m_pEventClick = pControl;
      SetCapture(ww->m_hWnd);
      ww->m_bMouseCapture = true;
      event.Type = UIEVENT_BUTTONDOWN;
      event.wParam = wParam;
      event.lParam = lParam;
      event.ptMouse = pt;
      event.wKeyState = ( WORD ) wParam;
      event.dwTimestamp =  GetTickCount();
      Event( pControl, event );
    }
    break;
  case WM_LBUTTONUP: 
    {
      TEventUI event = { 0 };
      POINT pt = { GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };
      ww->m_uButtonState &= ~( UISTATE_PUSHED | UISTATE_CAPTURED );
      ww->m_ptLastMousePos = pt;
      if ( ww->m_pEventClick == NULL )
        break;
      ReleaseCapture();
      ww->m_bMouseCapture = false;
      event.Type = UIEVENT_BUTTONUP;
      event.wParam = wParam;
      event.lParam = lParam;
      event.ptMouse = pt;
      event.wKeyState = ( WORD ) wParam;
      event.dwTimestamp =  GetTickCount();
      Event(ww->m_pEventClick,  event );
      ww->m_pEventClick = NULL;
    }
    break;
  case WM_LBUTTONDBLCLK: 
    {
      TEventUI event = { 0 };
      CControlUI* pControl;
      POINT pt = { GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };
      ww->m_ptLastMousePos = pt;
      pControl = FindControlByPoint( ww->m_pRoot, pt );
      if ( pControl == NULL )
        break;
      SetCapture( ww->m_hWndPaint );
      ww->m_bMouseCapture = true;
      event.Type = UIEVENT_DBLCLICK;
      event.ptMouse = pt;
      event.wKeyState = ( WORD ) wParam;
      event.dwTimestamp =  GetTickCount();
      Event( pControl, event );
      ww->m_pEventClick = pControl;
    }
    break;
  case WM_IME_COMPOSITION: 
    {
      if ( ww->m_pFocus == NULL )
        break;
      if ( ( lParam & GCS_RESULTSTR ) != 0 ) {
        HWND hWnd = GetPaintWindow(ww);
        HIMC hIMC =  ImmGetContext( hWnd );
        if ( hIMC != NULL ) {
          CHAR sz[ MAX_PATH ];
          LONG cch = ImmGetCompositionString( hIMC, GCS_RESULTSTR, sz, lengthof( sz ) );
          ImmReleaseContext( hWnd, hIMC );
          // 经过测试，有些输入法可能会返回零
          if ( ( cch > 0 ) && ( cch != IMM_ERROR_NODATA ) && ( cch != IMM_ERROR_GENERAL ) ) {
            //OnImmKeyChars( ww->m_pFocus, (LPCTSTR)( sz ), cch );
          }
        }
      }
    }
    break;
  case WM_CHAR: 
    {
      TEventUI event = { 0 };
      if ( ww->m_pFocus == NULL )
        break;
      event.Type = UIEVENT_CHAR;
      event.chKey = ( TCHAR ) wParam;
      event.ptMouse = ww->m_ptLastMousePos;
      event.wKeyState = MapKeyState();
      event.dwTimestamp =  GetTickCount();
      Event(ww->m_pFocus,  event );
    }
    break;
  case WM_KEYDOWN: 
    {
      TEventUI event = { 0 };
      if ( ww->m_pFocus == NULL )
        break;
      event.Type = UIEVENT_KEYDOWN;
      event.chKey = ( TCHAR ) wParam;
      event.ptMouse = ww->m_ptLastMousePos;
      event.wKeyState = MapKeyState();
      event.dwTimestamp =  GetTickCount();
      Event(ww->m_pFocus,  event );
      ww->m_pEventKey = ww->m_pFocus;
    }
    break;
  case WM_KEYUP: 
    {
      TEventUI event = { 0 };
      if ( ww->m_pEventKey == NULL )
        break;
      event.Type = UIEVENT_KEYUP;
      event.chKey = ( TCHAR ) wParam;
      event.ptMouse = ww->m_ptLastMousePos;
      event.wKeyState = MapKeyState();
      event.dwTimestamp =  GetTickCount();
      Event( ww->m_pEventKey, event );
      ww->m_pEventKey = NULL;
    }
    break;
  case WM_SETCURSOR: 
    {
      POINT pt = { 0 };
      TEventUI event = { 0 };
      CControlUI* pControl;
      if ( LOWORD( lParam ) != HTCLIENT )
        break;
      if ( ww->m_bMouseCapture )
        return true;
      GetCursorPos( &pt );
      ScreenToClient( ww->m_hWndPaint, &pt );
      pControl = FindControlByPoint( ww->m_pRoot, pt );
      if ( pControl == NULL )
        break;
      //if ( ( pControl->GetControlFlags() & UIFLAG_SETCURSOR ) == 0 )
      //  break;
      event.Type = UIEVENT_SETCURSOR;
      event.wParam = wParam;
      event.lParam = lParam;
      event.ptMouse = pt;
      event.wKeyState = MapKeyState();
      event.dwTimestamp =  GetTickCount();
      Event( pControl, event );
    }
    return true;
  case WM_MEASUREITEM:
    {
      HWND hWndChild;
      if ( wParam == 0 )
        break;
      hWndChild = GetDlgItem( ww->m_hWndPaint, ( ( LPMEASUREITEMSTRUCT ) lParam ) ->CtlID );
      lRes = SendMessage( hWndChild, OCM__BASE + uMsg, wParam, lParam );
      return true;
    }
    break;
  case WM_DRAWITEM: 
    {
      HWND hWndChild;
      if ( wParam == 0 )
        break;
      hWndChild = ( ( LPDRAWITEMSTRUCT ) lParam ) ->hwndItem;
      lRes =  SendMessage( hWndChild, OCM__BASE + uMsg, wParam, lParam );
      return true;
    }
    break;
  case WM_NOTIFY: 
    {
      LPNMHDR lpNMHDR = ( LPNMHDR ) lParam;
      if ( lpNMHDR != NULL )
        lRes =  SendMessage( lpNMHDR->hwndFrom, OCM__BASE + uMsg, wParam, lParam );
      return true;
    }
    break;
  case WM_COMMAND: 
    {
      HWND hWndChild;
      if ( lParam == 0 )
        break;
      hWndChild = ( HWND ) lParam;
      lRes =  SendMessage( hWndChild, OCM__BASE + uMsg, wParam, lParam );
      return true;
    }
    break;
  default:
    if ( uMsg == WM_MOUSEWHEEL && ww->m_pEventHover != NULL ) {
      int zDelta = ( int ) ( short ) HIWORD( wParam );
      TEventUI event = { 0 };
      event.Type = UIEVENT_SCROLLWHEEL;
      event.wParam = MAKELPARAM( zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0 );
      event.lParam = lParam;
      event.dwTimestamp =  GetTickCount();
      Event(ww->m_pEventHover,  event );
      // Let's make sure that the scroll item below the cursor is the same as before...
      SendMessage( ww->m_hWndPaint, WM_MOUSEMOVE, 0, ( LPARAM ) MAKELPARAM( ww->m_ptLastMousePos.x, ww->m_ptLastMousePos.y ) );
    }
    break;
  }
#if 0
  if ( ww->m_pFocus && ww->m_pFocus->IsVisible() && ww->m_pFocus->IsEnabled() && _tcsstr( ww->m_pFocus->GetClass(), _T( "RichEditUI" ) ) != NULL ) {
    if ( ( uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) || ( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST )
         || uMsg == WM_CHAR || uMsg == WM_IME_CHAR ) {
      LRESULT lResult;
      HRESULT hr = static_cast<CRichEditUI*>( ww->m_pFocus ) ->TxSendMessage( uMsg, wParam, lParam, &lResult );
      if ( hr == S_FALSE )
        return false;
      lRes = lResult;
      return true;
    }
  }
#endif

#undef lRes
  return false;
}