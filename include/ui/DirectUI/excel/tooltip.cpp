#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "tooltip.h"
HWND hToolTip;
HWND hSkugga;
HFONT hfFont;
BOOL bFirst = TRUE;
LPGETTEXT g_lpfnGetText;
LPARAM g_lParam;
BOOL g_bCenter = 0;
#include "htmlviewer.h"
class CToolTipProgress : public CHTMLProgress {
public:
  virtual void Reading( BOOL bImage, int nPos, int nSize ) {}
  virtual void LinkClicked( char *szLink ) {}
  virtual void SetCursor( BOOL bLink, char *szLink ) {}
  virtual void SetTitle( char *szTitle ) {}
  virtual char *ReadHTTPData( char *szUrl, int *pnSize ) {
    if ( !strnicmp( szUrl, "res://", 6 ) ) {
      HRSRC hr;
      HGLOBAL hg;
      char *lpv;
      char *szData;
      if ( !( hr = FindResource( GetModuleHandle( NULL ), &szUrl[ 6 ], RT_HTML ) ) ||
           !( hg = LoadResource( GetModuleHandle( NULL ), hr ) ) ||
           !( lpv = ( char* ) LockResource( hg ) ) ) {
        if ( !( hr = FindResource( GetModuleHandle( NULL ), strrchr( szUrl, '/' ) + 1, RT_HTML ) ) ||
             !( hg = LoadResource( GetModuleHandle( NULL ), hr ) ) ||
             !( lpv = ( char* ) LockResource( hg ) ) ) {
          return NULL;
        }
      }
      *pnSize = SizeofResource( GetModuleHandle( NULL ), hr );
      szData = new char[ *pnSize + 1 ];
      memcpy( szData, lpv, *pnSize );
      szData[ *pnSize ] = 0;
      return szData;
    }
    return NULL;
  }
  virtual void ImageLoaded( int nDone, int nCount ) {}
  // Control functions, return -1 if no control support.
  virtual int CreateForm( char *szAction, char *szMethod ) {
    return -1;
  }
  virtual int CreateTextBox( int nSize, int nMaxChar, SIZE scSize, char *szText, BOOL bMultiline, char *szName, int nForm, BOOL bPassword ) {
    return -1;
  }
  virtual int CreateButton( SIZE scSize, char *szText, char *szName, int nForm ) {
    return -1;
  }
  virtual int CreateRadio( SIZE scSize, char *szValue, char *szName, int nForm ) {
    return -1;
  }
  virtual int CreateCheckbox( SIZE scSize, char *szValue, char *szName, int nForm ) {
    return -1;
  }
  virtual int CreateListbox( SIZE scSize, CHTMLStringTable &stOptions, int nSelected, char *szName, int nForm ) {
    return -1;
  }
  virtual void MoveControl( int nControlID, POINT pPos ) {}
  virtual void UpdateScreen( RECT &r ) {}
}
g_ToolTipProgress;
typedef void ( __stdcall *lpfnSetLayeredWindowAttributes ) ( HWND, int, int, int );
lpfnSetLayeredWindowAttributes g_lpfnSetLayeredWindowAttributes = NULL;
CHTMLViewer *g_pWC = NULL;
BOOL IsActiveApp( HWND hWnd ) {
  BOOL bActive = FALSE;
  hWnd = GetParent( hWnd );
  while ( hWnd && !bActive ) {
    if ( GetActiveWindow() == hWnd )
      bActive = TRUE;
    else
      hWnd = GetParent( hWnd );
  }
  return bActive;
}
void SetRegions( HWND hWnd ) {
  HRGN hRgn1, hRgn2, hRgn3;
  RECT r;
  POINT p[ 3 ];
  GetWindowRect( hWnd, &r );
  hRgn1 = CreateRoundRectRgn( 0, 10, r.right - r.left + 1, r.bottom - r.top + 1, 30, 25 );
  p[ 0 ].x = 0;
  p[ 0 ].y = 0;
  p[ 1 ].x = 8;
  p[ 1 ].y = 20;
  p[ 2 ].x = 20;
  p[ 2 ].y = 15;
  hRgn2 = CreatePolygonRgn( p, 3, WINDING );
  hRgn3 = CreateRectRgn( 0, 0, 0, 0 );
  CombineRgn( hRgn3, hRgn1, hRgn2, RGN_OR );
  DeleteObject( hRgn1 );
  DeleteObject( hRgn2 );
  // hRgn1 = CreateRoundRectRgn(20,7,r.right-r.left+1-20,r.bottom-r.top+1,30,25);
  // CombineRgn(hRgn3,hRgn1,hRgn3,RGN_OR);
  // DeleteObject(hRgn1);
  // hRgn1 = CreateRoundRectRgn(15,5,r.right-r.left-14,r.bottom-r.top+5,20,20);
  // CombineRgn(hRgn3,hRgn1,hRgn3,RGN_OR);
  // DeleteObject(hRgn1);
  SetWindowRgn( hWnd, hRgn3, TRUE );
}
HWND g_hwToolTip = NULL;
long WINAPI ButtonProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  POINT p, t;
  RECT r, wr;
  static BOOL bShow = FALSE;
  BOOL bPrev;
  static POINT pos;
  int i = 0;
  char szTemp[ 5012 ];
  HDC hDC;
  HFONT hfOld;
  char *sz;
  switch ( uMsg ) {
  case WM_TIMER:
    GetClientRect( hWnd, &r );
    GetCursorPos( &p );
    ScreenToClient( hWnd, &p );
    GetCursorPos( &t );
    bPrev = bShow;
    if ( g_hwToolTip && g_hwToolTip != hWnd )
      break;
    if ( ( WindowFromPoint( t ) == hWnd || GetParent( WindowFromPoint( t ) ) == hWnd || GetParent( GetParent( WindowFromPoint( t ) ) ) == hWnd ) && p.x > 0 && p.y > 0 && p.x < r.right && p.y < r.bottom ) {
      if ( g_hwToolTip == hWnd )
        break;
      bShow = TRUE;
      g_hwToolTip = hWnd;
    } else if ( g_hwToolTip == hWnd ) {
      ShowWindow( hToolTip, SW_HIDE );
      ShowWindow( hSkugga, SW_HIDE );
      bShow = FALSE;
      g_hwToolTip = NULL;
    }
    if ( !bShow || !IsActiveApp( hWnd ) )
      break;
    GetCursorPos( &p );
    sz = g_lpfnGetText( GetDlgCtrlID( hWnd ), g_lParam );
    if ( sz && *sz )
      strcpy( szTemp, sz );
    else
      break;
    if ( g_pWC ) {
      char * szTmp = new char[ strlen( sz ) + 200 ];
      strcpy( szTmp, "<html><body bgcolor=#ffffc8 topmargin=0 leftmargin=0 SCROLL=NO><table cellpadding=0 cellspacing=0><tr><td align=center>" );
      strcat( szTmp, sz );
      strcat( szTmp, "</td></tr></table></body></html>" );
      g_pWC->SetHTML( szTmp, "Arial", 14, 0, "" );
      delete szTmp;
    }
    hDC = GetDC( hToolTip );
    hfOld = ( HFONT ) SelectObject( hDC, hfFont );
    memset( &r, 0, sizeof( RECT ) );
    r.right = 300;
    if ( g_pWC ) {
      g_pWC->CalcPositions( r );
    }
    SelectObject( hDC, hfOld );
    ReleaseDC( hToolTip, hDC );
    GetClientRect( GetDesktopWindow(), &wr );
    if ( p.x + 15 + r.right + 12 > wr.right )
      p.x = wr.right - 15 - r.right - 20;
    if ( p.y + 15 + r.bottom + 12 > wr.bottom )
      p.y = wr.bottom - 15 - r.bottom - 30;
    SetWindowText( hToolTip, szTemp );
    SetWindowPos( hSkugga, HWND_TOPMOST, p.x + 15 + 10, p.y + 15 + 4, r.right + 20 - 4, r.bottom + 20, SWP_NOACTIVATE );
    SetRegions( hSkugga );
    ShowWindow( hSkugga, SW_SHOWNA );
    SetWindowPos( hToolTip, HWND_TOPMOST, p.x + 15, p.y + 15, r.right + 20, r.bottom + 20, SWP_NOACTIVATE );
    SetRegions( hToolTip );
    ShowWindow( hToolTip, SW_SHOWNA );
    break;
  case WM_CAPTURECHANGED:
  case WM_LBUTTONDOWN:
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_LBUTTONUP:
  case WM_RBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_SHOWWINDOW:
  case WM_DESTROY:
    if ( !bShow )
      break;
    ShowWindow( hToolTip, SW_HIDE );
    ShowWindow( hSkugga, SW_HIDE );
    if ( uMsg == WM_DESTROY )
      KillTimer( hWnd, 0 );
    bShow = FALSE;
    break;
  }
  return CallWindowProc( ( WNDPROC ) GetClassLong( hWnd, GCL_WNDPROC ), hWnd, uMsg, wParam, lParam );
}
long WINAPI SkuggaProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  PAINTSTRUCT ps;
  HDC hDC;
  RECT r;
  // int i;
  HBRUSH br;
  HBITMAP bmo, bm;
  if ( uMsg == WM_CREATE ) {
    SetWindowLong( hWnd, GWL_EXSTYLE, GetWindowLong( hWnd, GWL_EXSTYLE ) | 0x00080000 /*WS_EX_LAYERED*/ );
    g_lpfnSetLayeredWindowAttributes( hWnd, 0, 100, 2 /*LWA_ALPHA*/ );
  } else if ( uMsg == WM_PAINT ) {
    if ( g_lpfnSetLayeredWindowAttributes )
      br = CreateSolidBrush( RGB( 0, 0, 0 ) );
    else
      br = CreateSolidBrush( RGB( 111, 111, 111 ) );
    GetClientRect( hWnd, &r );
    BeginPaint( hWnd, &ps );
    hDC = CreateCompatibleDC( ps.hdc );
    bm = CreateCompatibleBitmap( ps.hdc, r.right, r.bottom );
    bmo = ( HBITMAP ) SelectObject( hDC, bm );
    if ( g_lpfnSetLayeredWindowAttributes ) {
      int a = r.bottom;
      int nCol = 180;
      for ( ;r.bottom > a / 2;r.bottom-- ) {
        DeleteObject( br );
        br = CreateSolidBrush( RGB( nCol, nCol, nCol ) );
        nCol -= 20;
        FillRect( ps.hdc, &r, br );
        r.right--;
        r.left++;
        r.top++;
        if ( !nCol )
          break;
      }
    } else {
      FillRect( hDC, &r, br );
      BitBlt( ps.hdc, 0, 0, r.right, r.bottom, hDC, 0, 0, SRCAND );
    }
    SelectObject( hDC, bmo );
    DeleteObject( bm );
    DeleteObject( br );
    DeleteDC( hDC );
    EndPaint( hWnd, &ps );
  }
  return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
long WINAPI ToolTipProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  char szTemp[ 512 ];
  PAINTSTRUCT ps;
  HDC hDC;
  HFONT hfOld;
  RECT r;
  HRGN rgn;
  if ( uMsg == WM_CREATE ) {
    RECT r = {0, 0, 100, 0};
    g_pWC = new CHTMLViewer( &g_ToolTipProgress, TRUE );
  } else if ( uMsg == WM_PAINT ) {
    SendMessage( hWnd, WM_GETTEXT, sizeof( szTemp ) - 1, ( LPARAM ) szTemp );
    hDC = BeginPaint( hWnd, &ps );
    hfOld = ( HFONT ) SelectObject( hDC, ( HFONT ) hfFont );
    SetTextAlign( hDC, TA_LEFT | TA_TOP );
    SetBkMode( hDC, TRANSPARENT );
    GetClientRect( hWnd, &r );
    r.top = 14;
    if ( !g_bCenter )
      r.left += 10;
    if ( !g_pWC )
      DrawText( hDC, szTemp, strlen( szTemp ), &r, DT_TOP | DT_EXPANDTABS | ( g_bCenter ? DT_CENTER : 0 ) );
    else
      g_pWC->Draw( hDC, r, -9, -14 );
    SelectObject( hDC, hfOld );
    rgn = CreateRectRgn( 0, 0, 0, 0 );
    GetWindowRgn( hWnd, rgn );
    OffsetRgn( rgn, -1, -1 );
    FrameRgn( hDC, rgn, ( HBRUSH ) GetStockObject( BLACK_BRUSH ), 1, 1 );
    DeleteObject( rgn );
    EndPaint( hWnd, &ps );
  } else if ( uMsg == WM_DESTROY && g_pWC ) {
    delete g_pWC;
    g_pWC = NULL;
  }
  return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
void SetNewProc( HWND hwNext ) {
  char szClassName[ 256 ];
  hwNext = GetWindow( hwNext, GW_CHILD );
  while ( hwNext ) {
    GetClassName( hwNext, szClassName, sizeof( szClassName ) );
    if ( !stricmp( szClassName, "AtlAxWin" ) ) {
      hwNext = GetWindow( hwNext, GW_HWNDNEXT );
      continue;
    }
    SetWindowLong( hwNext, GWL_WNDPROC, ( LPARAM ) ButtonProc );
    SetTimer( hwNext, 0, 100, NULL );
    SetNewProc( hwNext );
    hwNext = GetWindow( hwNext, GW_HWNDNEXT );
  }
}
BOOL InitToolTip( HWND hWnd, HINSTANCE hInst, LPGETTEXT lpfnGetText, LPARAM lParam, BOOL bCenter ) {
  if ( !g_lpfnSetLayeredWindowAttributes ) {
    HINSTANCE hMod = LoadLibrary( "User32.dll" );
    g_lpfnSetLayeredWindowAttributes =
      ( lpfnSetLayeredWindowAttributes ) GetProcAddress( hMod, "SetLayeredWindowAttributes" );
  }
  g_bCenter = bCenter;
  g_lpfnGetText = lpfnGetText;
  g_lParam = lParam;
  WNDCLASS wc;
  HBRUSH brGul;
  hfFont = ( HFONT ) GetStockObject( ANSI_VAR_FONT );
  if ( bFirst ) {
    bFirst = FALSE;
    brGul = CreateSolidBrush( RGB( 255, 255, 200 ) );
    memset( &wc, 0, sizeof( WNDCLASS ) );
    wc.lpfnWndProc = ToolTipProc;
    wc.hInstance = ( HINSTANCE ) hInst;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = brGul;
    wc.lpszClassName = "TOOLTIP";
    if ( RegisterClass( &wc ) )
      hToolTip = CreateWindow( "TOOLTIP", "", WS_POPUP | WS_BORDER, 0, 0, 120, 120, hWnd, NULL, ( HINSTANCE ) hInst, 0 );
    wc.lpfnWndProc = SkuggaProc;
    wc.hbrBackground = NULL;
    wc.lpszClassName = "SKUGGA";
    if ( RegisterClass( &wc ) )
      hSkugga = CreateWindow( "SKUGGA", "", WS_POPUP, 0, 0, 120, 120, hWnd, NULL, hInst, 0 );
  }
  // SetParent(hToolTip,hWnd);
  // SetParent(hSkugga,hWnd);
  SetNewProc( hWnd );
  return TRUE;
}
void ExitToolTip() {
  DestroyWindow( hToolTip );
  DestroyWindow( hSkugga );
}
