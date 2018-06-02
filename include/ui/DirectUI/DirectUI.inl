
#include <tchar.h>
#include <OleCtl.h>
//#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "Imm32.lib" )
#pragma comment( lib, "shlwapi.lib" )
#pragma comment( lib, "comctl32.lib" )
#pragma comment( lib, "Riched20.lib" )
#include "debug.inl"
#include "ControlUI.inl"
UINT GetClassStyle() {
  return 0;
}
typedef int (*INotifyUI)( CWindowWnd* ww, TNotifyUI* msg );
bool UIAddNotifier( CWindowWnd* ww, INotifyUI pNotifier ) {
  ASSERT( PtrArray_Find( ww->m_aNotifiers, pNotifier ) < 0 );
  return PtrArray_Add( ww->m_aNotifiers, pNotifier );
}
static LRESULT CALLBACK __WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
HWND Subclass( CWindowWnd* ww, HWND hWnd ) {
  ASSERT( IsWindow( hWnd ) );
  ASSERT( ww->m_hWnd == NULL );
  ww->m_OldWndProc = SubclassWindow( hWnd, __WndProc );
  if ( ww->m_OldWndProc == NULL )
    return NULL;
  ww->m_bSubclassed = true;
  ww->m_hWnd = hWnd;
  return ww->m_hWnd;
}
void Unsubclass(CWindowWnd* ww) {
  ASSERT( IsWindow( ww->m_hWnd ) );
  if ( !IsWindow( ww->m_hWnd ) )
    return ;
  if ( !ww->m_bSubclassed )
    return ;
  SubclassWindow( ww->m_hWnd, ww->m_OldWndProc );
  ww->m_OldWndProc = DefWindowProc;
  ww->m_bSubclassed = false;
}
BOOL MessageHandler( CWindowWnd* ww, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plRes );
bool UIKillTimer( CWindowWnd* ww, CControlUI* pControl, UINT nTimerID );

#include "Timer.inl"
#define KILLTIMER1(pTimer) KillTimer(pTimer->hWnd, pTimer->uWinTimer )
int ReleaseWnd(CWindowWnd* ww) {
  int i;
  if (ww->m_aTimers!=NULL) {
    for ( i = 0; i < PtrArray_GetSize(ww->m_aTimers); i++ ) {
      TIMERINFO* pTimer = (TIMERINFO*)( PtrArray_GetAt( ww->m_aTimers, i ) );
      if ( pTimer->bKilled == false ) {
        //KillTimer( pTimer->hWnd, pTimer->uWinTimer );
        KILLTIMER( pTimer );
      }
      DEL( pTimer );
    }
    PtrArray_Release(ww->m_aTimers);
  }
  if (ww->m_aNotifiers!=NULL) {
    PtrArray_Release(ww->m_aNotifiers);
  }
  CtrFree(ww->m_pRoot);
  if ( ww->m_hwndTooltip != NULL )
    DestroyWindow( ww->m_hwndTooltip );
  if ( ww->m_hDcOffscreen != NULL )
    DeleteDC( ww->m_hDcOffscreen );
  if ( ww->m_hbmpOffscreen != NULL )
    DeleteObject( ww->m_hbmpOffscreen );
  if ( ww->m_hDcPaint != NULL )
    ReleaseDC( ww->m_hWnd, ww->m_hDcPaint );
  // Reset other parts...
  //res->m_aPreMessages.Remove( res->m_aPreMessages.Find( this ) );
  ReleaseDC( ww->m_hWnd, ww->m_hDcPaint );
  free(ww);
  return 0;
}
static LRESULT CALLBACK __WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  CWindowWnd * pThis = NULL;
  if ( uMsg == WM_NCCREATE ) {
    LPCREATESTRUCT lpcs = (LPCREATESTRUCT)( lParam );
    pThis = (CWindowWnd*)( lpcs->lpCreateParams );
    pThis->m_hWnd = hWnd;
    SetWindowLongPtr( hWnd, GWLP_USERDATA, (LPARAM)( pThis ) );
  } else {
    pThis = (CWindowWnd*)( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
    if ( uMsg == WM_NCDESTROY && pThis != NULL ) {
      LRESULT lRes = CallWindowProc( pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam );
      SetWindowLongPtr( pThis->m_hWnd, GWLP_USERDATA, 0L );
      Unsubclass(pThis);
      //pThis->OnFinalMessage( hWnd );
      if (pThis->Final)
        pThis->Final( pThis );
      pThis->m_hWnd = NULL;
      return lRes;
    }
  }
  if (uMsg==WM_TIMER) 
    printf("WM_TIMER\n");
  if ( pThis != NULL ) {
    //return pThis->HandleMessage( uMsg, wParam, lParam );
    LRESULT Ret;
    MessageHandler(pThis, uMsg, wParam, lParam, &Ret);
  }
  return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
LPCTSTR GetWindowClassName() {
  return "WindowWnd";
}
BOOL RegisterWindowClass() {
  ATOM ret;
  WNDCLASS wc = { 0 };
  wc.style = GetClassStyle();
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hIcon = NULL;
  wc.lpfnWndProc = __WndProc;
  wc.hInstance = GetResourceInstance();
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = GetWindowClassName();
  //wc.hbrBackground = ( HBRUSH ) GetStockObject( WHITE_BRUSH );
  ret = RegisterClass( &wc );
  ASSERT( ret != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS );
  return ret != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}
HWND Create( CWindowWnd* ww, HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle,
            int x, int y, int cx, int cy, HMENU hMenu ) {
  if ( !RegisterWindowClass() )
    return NULL;
  ww->m_hWnd = CreateWindowEx( dwExStyle, GetWindowClassName(), pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, GetResourceInstance(), ww );
  ASSERT( ww->m_hWnd != NULL );
  return ww->m_hWnd;
}
BOOL PreMessageHandler( CWindowWnd* ww, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plRes ) {
  LRESULT* lRes=0;
#if 0
  switch ( uMsg ) {
  case WM_KEYDOWN: 
    {
      // Tabbing between controls
      if ( wParam == VK_TAB ) {
        if ( m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr( m_pFocus->GetClass(), _T( "RichEditUI" ) ) != NULL ) {
          if ( static_cast<CRichEditUI*>( m_pFocus ) ->IsWantTab() )
            return false;
        }
        SetNextTabControl(  GetKeyState( VK_SHIFT ) >= 0 );
        return true;
      }
    }
    break;
  case WM_SYSCHAR: 
    {
      // Handle ALT-shortcut key-combinations
      FINDSHORTCUT fs = { 0 };
      fs.ch = toupper( ( int ) wParam );
      CControlUI* pControl = m_pRoot->FindControl( __FindControlFromShortcut, &fs, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST );
      if ( pControl != NULL ) {
        pControl->SetFocus();
        pControl->Activate();
        return true;
      }
    }
    break;
  case WM_SYSKEYDOWN: 
    {
      if ( m_pFocus != NULL ) {
        TEventUI event = { 0 };
        event.Type = UIEVENT_SYSKEY;
        event.chKey = ( TCHAR ) wParam;
        event.ptMouse = m_ptLastMousePos;
        event.wKeyState = MapKeyState();
        event.dwTimestamp =  GetTickCount();
        m_pFocus->Event( event );
      }
    }
    break;
  }
#endif
  return false;
}
bool UIKillTimer1( CWindowWnd* ww, CControlUI* pControl, UINT nTimerID ) {
  int i;
  ASSERT( pControl != NULL );
  for ( i = 0; i < PtrArray_GetSize(ww->m_aTimers); i++ ) {
    TIMERINFO* pTimer = (TIMERINFO*)( PtrArray_GetAt( ww->m_aTimers, i ) );
    if ( pTimer->pSender == pControl
      && pTimer->hWnd == ww->m_hWnd
      && pTimer->nLocalID == nTimerID ) {
      if ( pTimer->bKilled == false ) {
        KillTimer( pTimer->hWnd, pTimer->uWinTimer );
        pTimer->bKilled = true;
        return true;
      }
    }
  }
  return false;
}
bool UISetTimer1( CWindowWnd* ww, CControlUI* pControl, UINT nTimerID, UINT uElapse ) {
  int i;
  TIMERINFO* pTimer;
  ASSERT( pControl != NULL );
  ASSERT( uElapse > 0 );
  for ( i = 0; i < PtrArray_GetSize(ww->m_aTimers); i++ ) {
    TIMERINFO* pTimer = (TIMERINFO*)PtrArray_GetAt( ww->m_aTimers, i );
    if ( pTimer->pSender == pControl
      && pTimer->hWnd == ww->m_hWnd
      && pTimer->nLocalID == nTimerID ) {
      if ( pTimer->bKilled == true ) {
        if ( SetTimer( ww->m_hWnd, pTimer->uWinTimer, uElapse, NULL ) ) {
          pTimer->bKilled = false;
          return true;
        }
        return false;
      }
      return false;
    }
  }
  ww->m_uTimerID = ( ++(ww->m_uTimerID )) % 0xFF;
  if ( !SetTimer( ww->m_hWnd, ww->m_uTimerID, uElapse, NULL ) )
    return FALSE;
  pTimer = NEW(TIMERINFO, 1);
  if ( pTimer == NULL )
    return FALSE;
  pTimer->hWnd = ww->m_hWnd;
  pTimer->pSender = pControl;
  pTimer->nLocalID = nTimerID;
  pTimer->uWinTimer = ww->m_uTimerID;
  pTimer->bKilled = false;
  return PtrArray_Add( ww->m_aTimers, pTimer );
}
int SendNotify( CWindowWnd* ww, CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ ) {
  TNotifyUI Msg;
  int i;
  Msg.pSender = pControl;
  Msg.sType = pstrMessage;
  Msg.wParam = wParam;
  Msg.lParam = lParam;
  
  Msg.ptMouse = ww->m_ptLastMousePos;
  Msg.dwTimestamp = GetTickCount();
  // Send to all listeners
  for ( i = 0; i < PtrArray_GetSize(ww->m_aNotifiers); i++ ) {
    INotifyUI pNotify = (INotifyUI)PtrArray_GetAt(ww->m_aNotifiers, i);
    pNotify( ww, &Msg );
  }
  return 0;
}
CControlUI* FindControlByName(CControlUI* ctr, LPCTSTR name) {
  if (ctr->m_sName && 0==stricmp( ctr->m_sName, name)) {
    return ctr;
  }
  if (ctr->m_items) {
    int it;
    for ( it = 0; it < PtrArray_GetSize(ctr->m_items); it++ ) {
      CControlUI* child = (CControlUI*)PtrArray_GetAt(ctr->m_items, it );
      CControlUI* ret = FindControlByName(child, name);
      if (NULL!=ret) {
        return ret;
      }
    }
  }
  return NULL;
}
CControlUI* FindControlByPoint1(CControlUI* ctr, POINT pt) {
  if (PtInRect( &ctr->m_rcItem, pt)) {
    if (ctr->m_items) {
      int it;
      for ( it = 0; it < PtrArray_GetSize(ctr->m_items); it++ ) {
        CControlUI* child = (CControlUI*)PtrArray_GetAt(ctr->m_items, it );
        CControlUI* ret = FindControlByPoint1(child, pt);
        if (NULL!=ret) {
          return ret;
        }
      }
    }
    return ctr;
  }
  return 0;
}
//#define FindControlByPoint(ctr, pt) FindControlByPoint2(ww, ctr, pt)
#define FindControlByPoint(ctr, pt) FindControlByPoint1(ctr, pt)
CControlUI* FindControlByPoint2(CWindowWnd* ww, CControlUI* ctr, POINT pt) {
  TEventUI event = { 0 };
  event.Type = UIEVENT_FIND_CONTROL_BY_POINT;
  event.pSender = 0;
  event.wParam = 0;
  event.lParam = 0;
  event.ptMouse = pt;
  event.wKeyState = ( WORD ) 0;
  event.dwTimestamp =  GetTickCount();
  Event( ctr, event );
  return event.pSender;
}
static UINT MapKeyState() {
  UINT uState = 0;
  if ( GetKeyState( VK_CONTROL ) < 0 )
    uState |= MK_CONTROL;
  if ( GetKeyState( VK_RBUTTON ) < 0 )
    uState |= MK_LBUTTON;
  if ( GetKeyState( VK_LBUTTON ) < 0 )
    uState |= MK_RBUTTON;
  if ( GetKeyState( VK_SHIFT ) < 0 )
    uState |= MK_SHIFT;
  if ( GetKeyState( VK_MENU ) < 0 )
    uState |= MK_ALT;
  return uState;
}
BOOL DUI_TranslateMessage( const LPMSG pMsg ) {
  HWND hwndParent = GetParent( pMsg->hwnd );
  UINT uStyle = GetWindowStyle( pMsg->hwnd );
  LRESULT lRes = 0;
  int i;
  for ( i = 0; i < m_aPreMessagesSize; i++ ) {
    CWindowWnd* pT = ( m_aPreMessages[ i ] );
    if ( pMsg->hwnd == GetPaintWindow(pT)
      || ( hwndParent == GetPaintWindow(pT) && ( ( uStyle & WS_CHILD ) != 0 ) ) ) {
      if ( PreMessageHandler( pT, pMsg->message, pMsg->wParam, pMsg->lParam, &lRes ) )
        return true;
    }
  }
  return false;
}
void MessageLoop() {
  MSG msg = { 0 };
  while ( GetMessage( &msg, NULL, 0, 0 ) ) {
    if ( !DUI_TranslateMessage( &msg ) ) {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }
}
CWindowWnd* CreateWnd(LPCTSTR pstrName, int x, int y, int cx, int cy) {
  CWindowWnd* ww = (CWindowWnd*)malloc(sizeof(CWindowWnd));
  HWND hWnd;
    ww->m_hDcPaint=( NULL ),
    ww->m_hDcOffscreen=( NULL ),
    ww->m_hbmpOffscreen=( NULL ),
    ww->m_hwndTooltip=( NULL ),
    ww->m_uTimerID=( 0x1000 ),
    ww->m_pRoot=( NULL ),
    ww->m_pFocus=( NULL ),
    ww->m_pEventHover=( NULL ),
    ww->m_pEventClick=( NULL ),
    ww->m_pEventKey=( NULL ),
    ww->m_bFirstLayout=( true ),
    ww->m_bFocusNeeded=( false ),
    ww->m_bUpdateNeeded=( false ),
    ww->m_bMouseTracking=( false ),
    ww->m_bMouseCapture=( false ),
    ww->m_bOffscreenPaint=( true );
    ww->m_szMinWindow.cx = 0;
    ww->m_szMinWindow.cy = 0;
    ww->m_szInitWindowSize.cx = 0;
    ww->m_szInitWindowSize.cy = 0;
    ww->m_szRoundCorner.cx = ww->m_szRoundCorner.cy = 0;
    ZeroMemory( &ww->m_rcSizeBox, sizeof( ww->m_rcSizeBox ) );
    ZeroMemory( &ww->m_rcCaption, sizeof( ww->m_rcCaption ) );
  ww->m_ptLastMousePos.x = ww->m_ptLastMousePos.y = -1;
  memset(ww, 0, sizeof(CWindowWnd));
  hWnd = Create(ww, 0, pstrName, 
    //WS_OVERLAPPEDWINDOW, WS_EX_OVERLAPPEDWINDOW,
    UI_WNDSTYLE_FRAME,0,
    x,y,cx, cy, 0);
  ww->m_hDcPaint = GetDC( ww->m_hWnd );
  ww->m_aTimers = CreatePtrArray(0);
  ww->m_aNotifiers = CreatePtrArray(0);
  return ww;
}

#define m_hWndPaint m_hWnd
void UISetFocus( CWindowWnd* ww, CControlUI* pControl ) {
  // Paint manager window has focus?
  if ( GetFocus() != ww->m_hWndPaint )
    SetFocus( ww->m_hWndPaint );
  // Already has focus?
  if ( pControl == ww->m_pFocus )
    return ;
  // Remove focus from old control
  if ( ww->m_pFocus != NULL ) {
    TEventUI event = { 0 };
    event.Type = UIEVENT_KILLFOCUS;
    event.pSender = pControl;
    event.dwTimestamp = GetTickCount();
    Event( ww->m_pFocus, event );
    SendNotify( ww, ww->m_pFocus, _T( "killfocus" ), 0, 0 );
    ww->m_pFocus = NULL;
  }
  // Set focus to new control
  if ( pControl != NULL && IsVisible(pControl) && IsEnabled(pControl) ) {
    TEventUI event = { 0 };
    ww->m_pFocus = pControl;
    event.Type = UIEVENT_SETFOCUS;
    event.pSender = pControl;
    event.dwTimestamp = GetTickCount();
    Event( ww->m_pFocus, event );
    SendNotify( ww, ww->m_pFocus, _T( "setfocus" ), 0, 0 );
  }
  ww->m_pFocus = pControl;
}

bool UIInitControls( CWindowWnd* ww, CControlUI* ctr, CControlUI* pParent /*= NULL*/ ) {
  TEventUI event = { 0 };
  ASSERT( ctr );
  if ( ctr == NULL )
    return false;
  //pControl->SetManager( this, pParent != NULL ? pParent : pControl->GetParent(), true );
  //pControl->FindControl( __FindControlFromNameHash, this, UIFIND_ALL );
  event.ptMouse = ww->m_ptLastMousePos;
  event.dwTimestamp =  GetTickCount();
  event.Type = UIEVENT_INIT;
  Event(ctr,  event );
  if (ctr->m_items!=NULL) {
    int i=0;
    for (; i<PtrArray_GetSize(ctr->m_items); ++i) {
      CControlUI* ch = (CControlUI*)PtrArray_GetAt(ctr->m_items, i);
      UIInitControls(ww, ch, pParent);
    }
  }
  return true;
}
bool UIAttachDialog( CWindowWnd* ww, CControlUI* ctr ) {
  ASSERT( IsWindow( ww->m_hWnd ) );
  // Reset any previous attachment
  SetFocus( ww->m_hWnd );
  ww->m_pEventKey = NULL;
  ww->m_pEventHover = NULL;
  ww->m_pEventClick = NULL;
  // Remove the existing control-tree. We might have gotten inside this function as
  // a result of an event fired or similar, so we cannot just delete the objects and
  // pull the internal memory of the calling code. We'll delay the cleanup.
  if ( ww->m_pRoot != NULL ) {
    //ww->m_aPostPaintControls.Empty();
    //m_aDelayedCleanup.Add( m_pRoot );
    //PostMessage( m_hWndPaint, WM_APP + 1, 0, 0L );
  }
  // Set the dialog root element
  ww->m_pRoot = ctr;
  // Go ahead...
  ww->m_bUpdateNeeded = true;
  ww->m_bFirstLayout = true;
  ww->m_bFocusNeeded = true;
  // Initiate all control
  return UIInitControls( ww, ctr, NULL );
}
#include "MessageHandler.inl"
