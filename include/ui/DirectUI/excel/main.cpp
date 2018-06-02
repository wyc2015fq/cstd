// main
// Dialog functions
//
// ?2004 Karl Runmo ,runmo@hotmail.com
//
// COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
// THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
// OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
// CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
// THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
// SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
// PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
// THIS DISCLAIMER.
#include <windows.h>
#include "htmlcontrol.h"
#include <wininet.h>
#include <commctrl.h>
#include "resource.h"
#include "tooltip.h"
#ifdef _DEBUG
#include <crtdbg.h>
#endif
HWND hwHTML = NULL;
HBITMAP g_bmBmp = NULL;
CHTMLControl *g_pControl = NULL;
HWND g_hwStatus = NULL;
int WINAPI HTTPProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  if ( g_pControl )
    return g_pControl->ProcessMessage( hWnd, uMsg, wParam, lParam );
  return 0;
}
int WINAPI MainProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  if ( uMsg == WM_CTLCOLORDLG )
    return ( int ) GetStockObject( NULL_BRUSH );
  if ( uMsg == 0x020A && hwHTML ) {
    g_pControl->ProcessMessage( hwHTML, uMsg, wParam, lParam );
    g_pControl->ProcessMessage( hwHTML, uMsg, wParam, lParam );
  }
  if ( uMsg == WM_INITDIALOG ) {
    SendMessage( hWnd, WM_SETICON, ICON_BIG, ( LPARAM ) LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_ICON1 ) ) );
    ShowWindow( hWnd, SW_SHOW );
    RECT r;
    GetClientRect( hWnd, &r );
    hwHTML = CreateDialog( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDD_DIALOG2 ), hWnd, HTTPProc );
    MoveWindow( hwHTML, 0, r.top + 20, r.right, r.bottom - 40, TRUE );
    ShowWindow( hwHTML, SW_SHOW );
    PostMessage( hWnd, WM_SIZE, 0, 0 );
    EnableWindow( GetDlgItem( hWnd, IDC_BUTTON1 ), FALSE );
    g_hwStatus = CreateStatusWindow( WS_CHILD | WS_VISIBLE, "", hWnd, 0 );
    g_pControl = new CHTMLControl( hwHTML, hWnd, g_hwStatus, GetDlgItem( hWnd, IDC_EDIT1 ), GetDlgItem( hWnd, IDC_BUTTON1 ) );
    g_pControl->LinkClicked( "res://test.htm" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.codeproject.com/" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.codeproject.com/cpp/krhtmlviewer.asp" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.libpng.org/pub/png/pngs-img.html" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://entropymine.com/jason/testbed/pngtrans/" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.w3.org/Graphics/PNG/inline-alpha-table32.html" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.gameon.se/kk/" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.gameon.se/cc/" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.fof.se/" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.google.com/" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.yahoo.com/" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.altavista.com/" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.arthouse.org/dfly5.html" );
    SendDlgItemMessage( hWnd, IDC_EDIT1, CB_ADDSTRING, 0, ( LPARAM ) "http://www.physto.se/~vetfolk/aktuellt.html" );
  } else if ( uMsg == WM_COMMAND && wParam == IDCANCEL ) {
    EndDialog( hWnd, 0 );
    if ( g_pControl )
      delete g_pControl;
    PostQuitMessage( 0 );
  } else if ( uMsg == WM_COMMAND && wParam == IDOK ) {
    char szUrl[ 1024 ];
    SetFocus( GetDlgItem( hWnd, IDOK ) );
    GetDlgItemText( hWnd, IDC_EDIT1, szUrl, sizeof( szUrl ) );
    g_pControl->LinkClicked( szUrl );
    EnableWindow( GetDlgItem( hWnd, IDC_BUTTON1 ), g_pControl->EnableHistory() );
  } else if ( uMsg == WM_COMMAND && wParam == IDC_BUTTON1 ) {
    g_pControl->GoBack();
    EnableWindow( GetDlgItem( hWnd, IDC_BUTTON1 ), g_pControl->EnableHistory() );
  } else if ( uMsg == WM_SIZE ) {
    RECT r;
    GetClientRect( hWnd, &r );
    GetClientRect( hWnd, &r );
    MoveWindow( g_hwStatus, 0, r.bottom - 20, r.right, 20, TRUE );
    MoveWindow( hwHTML, 0, r.top + 20, r.right, r.bottom - 40, TRUE );
    MoveWindow( GetDlgItem( hWnd, IDC_EDIT1 ), 0, 0, r.right - 120, 20, TRUE );
    MoveWindow( GetDlgItem( hWnd, IDOK ), r.right - 120, 0, 60, 20, TRUE );
    MoveWindow( GetDlgItem( hWnd, IDC_BUTTON1 ), r.right - 60, 0, 60, 20, TRUE );
    InvalidateRect( GetDlgItem( hWnd, IDC_BUTTON1 ), NULL, TRUE );
    InvalidateRect( GetDlgItem( hWnd, IDOK ), NULL, TRUE );
    InvalidateRect( g_hwStatus, NULL, TRUE );
  }
  return 0;
}
#define COLUMNS 6
int Test() {
  char szTmp[ 256 ];
  DWORD dw;
  HANDLE hFile = CreateFile( "C:\\pojects\\ltest\\htmlviewer\\rand.htm", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
  strcpy( szTmp, "<BODY bgcolor=cccccc>\r\n<TABLE BORDER=5>\r\n" );
  WriteFile( hFile, szTmp, strlen( szTmp ), &dw, NULL );
  int nRowSpan[ COLUMNS ] = {0};
  int x, y, i;
  int nNum = 1;
  for ( y = 0;y < COLUMNS;y++ ) {
    WriteFile( hFile, "<TR>\r\n", 6, &dw, NULL );
    for ( x = 0;x < COLUMNS;x++ )
      if ( nRowSpan[ x ] )
        nRowSpan[ x ] --;
    for ( x = 0;x < COLUMNS; ) {
      while ( nRowSpan[ x ] && x < COLUMNS )
        x++;
      if ( x == COLUMNS )
        break;
      int nRow = rand() % ( 1 + COLUMNS / 3 ) + 1;
      int nCol = rand() % ( 1 + COLUMNS / 3 ) + 1;
      if ( nRow + y >= COLUMNS )
        nRow = COLUMNS - y;
      if ( nCol + x >= COLUMNS - 1 )
        nCol = COLUMNS - x;
      for ( i = x;i < x + nCol;i++ )
        if ( nRowSpan[ i ] )
          break;
      nCol = i - x;
      for ( i = x;i < x + nCol;i++ )
        nRowSpan[ i ] = nRow;
      wsprintf( szTmp, "<TD ROWSPAN=%d COLSPAN=%d ALING=CENTER VALIGN=CENTER>%d</TD>\r\n", nRow, nCol, nNum++ );
      WriteFile( hFile, szTmp, strlen( szTmp ), &dw, NULL );
      x += nCol;
    }
    //  strcpy(szTmp,"<TD>e</TD></TR>\r\n");
    //  WriteFile(hFile,szTmp,strlen(szTmp),&dw,NULL);
  }
  WriteFile( hFile, "<TR>\r\n", 6, &dw, NULL );
  // for(i=0;i<COLUMNS+1;i++)
  //  WriteFile(hFile,"<TD>e</TD>\r\n",12,&dw,NULL);
  WriteFile( hFile, "</TABLE>", 8, &dw, NULL );
  CloseHandle( hFile );
  return 0;
}
char * GetText( int nID, LPARAM lParam ) {
  if ( nID == IDC_EDIT1 )
    return "<table cellpadding=0 cellspacing=0 width=100%><tr><td bgcolor=ffaaaa align=center><b>Address field</b></td></tr></table><hr>Here you can type in an address to an URL that you want to display. The list contains example URLs with interesting content"
           "<table border=1 bordercolor=000000 bgcolor=ffeeff>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.codeproject.com/</td><td nobreak>This is the main page of CodeProject</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.codeproject.com/cpp/krhtmlviewer.asp<td nobreak>This is my page on CodeProject</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.libpng.org/pub/png/pngs-img.html<td rowspan=3>Shows some sample transparent and semitransparent PNG images</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://entropymine.com/jason/testbed/pngtrans/</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.w3.org/Graphics/PNG/inline-alpha-table32.html</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.gameon.se/kk/<td rowspan=2>Here you can download and try games created by me.</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.gameon.se/cc/"
           "<tr><td nobreak bgcolor=bbbbff>http://www.fof.se/<td nobreak>Swedish science magazine</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.google.com/<td rowspan=3>Here are some searchengines that shows that my HTML class can send form data</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.yahoo.com/</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.altavista.com/</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.arthouse.org/dfly5.html<td>This page shows some sample animated GIF images</td></tr>"
           "<tr><td nobreak bgcolor=bbbbff>http://www.physto.se/~vetfolk/aktuellt.html<td>This page shows large chunks of text with align=justify</td></tr>"
           "</table>";
  else if ( nID == IDC_BUTTON1 )
    return "<table cellpadding=0 cellspacing=0 width=100%><tr><td bgcolor=ffaaaa align=center><b>Back to previous view</b></td></tr></table><hr>Pages are not reloaded when using the <b><u><i>back</i></u></b> button. Instead, they are read from a list of visited URLs resided in memory.<img src=res://pig.gif>";
  else if ( nID == IDOK )
    return "<table cellpadding=0 cellspacing=0 width=100%><tr><td bgcolor=aaffaa align=center><b>Open an URL</b></td></tr></table><hr>This button must be pressed in order to open the URL.<br>It is not enough to just select an address in the dopdown box<br><i><b>To open an URL do the following steps</b></i><ul><li>Type an URL or select one in the list<li>Click on the button labeled <font color=0000ff>Go</font> or hit enter key<li>Wait for the URL to be loaded</ul><img src=res://img.png>";
  else if ( nID == IDCANCEL )
    return "Avsluta programmet";
  return "";
}
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int ) {
  // Test();
  HWND hwTmp = CreateDialogParam( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, MainProc, 0 );
  InitToolTip( hwTmp, hInstance, GetText, 0 );
  ShowWindow( hwTmp, SW_SHOW );
  MSG msg;
  while ( GetMessage( &msg, 0, 0, 0 ) ) {
    if ( msg.message == WM_KEYDOWN && msg.wParam == 'C' )
      g_pControl->Copy();
    if ( IsDialogMessage( hwTmp, &msg ) )
      continue;
    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }
  ExitToolTip();
#ifdef _DEBUG
  if ( _CrtDumpMemoryLeaks() )
    MessageBox( NULL, "Memory leaks found", "Router.exe debug", MB_OK );
#endif
  return 0;
}
#ifdef _DEBUG
void *operator new( unsigned int uSize ) {
  void *p = malloc( uSize );
  // if((int)p==0x00994A00 || (int)p==0x00994A30 || (int)p==0x00994A60 || (int)p==0x00994A90)
  //  Beep(1000,10);
  return p;
}
void operator delete( void *p ) {
  free( p );
}
#endif
