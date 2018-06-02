
#pragma comment(lib,"wininet.lib")

void CHTMLControl_Reading( CHTMLControl* me, BOOL bImage, int nPos, int nSize ) {
  printf( "%s %d of %d", bImage ? "Image" : "Parsing", nPos, nSize );
}
CHTMLControl* CreateCHTMLControl() {
  CHTMLControl* me = (CHTMLControl*)malloc(sizeof(CHTMLControl));
  *me->m_szCurrUrl = *me->m_szCurrStatus = 0;
  me->m_szActiveUrl = NULL;
  me->m_pViewer = CreateCHTMLViewer( 0, FALSE );
  me->m_bmBmp = NULL;
  me->m_bMainRequest = 0;
  me->m_nFontSize = 15;
  strcpy( me->m_szFontName, "Arial" );
  return me;
}
void DeleteCHTMLControl(CHTMLControl* me) {
  if ( me->m_bmBmp )
    DeleteObject( me->m_bmBmp );
  DeleteCHTMLViewer(me->m_pViewer);
  if ( me->m_szActiveUrl )
    free(me->m_szActiveUrl);
}
void CHTMLControl_Submit( CHTMLControl* me,int nForm, HWND hWnd, int nControlID ) {}
void CHTMLControl_Tick( CHTMLControl* me,int nPos, int nSize ) {
  // if(!me->m_bMainRequest)
  //  return;
  char szStatus[ 80 ];
  if ( nPos < 0 )
    return ;
  if ( nSize > 0 )
    wsprintf( szStatus, "Reading %d of %d", nPos, nSize );
  else
    wsprintf( szStatus, "Reading %d", nPos );
  printf( "%s\n", szStatus );
}
char *CHTMLControl_ReadHTTPData( CHTMLControl* me,char *szUrl, int *pnSize ) {
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
    szData = (char*)malloc( *pnSize + 1 );
    memcpy( szData, lpv, *pnSize );
    szData[ *pnSize ] = 0;
    if ( me->m_bMainRequest )
      strcpy( me->m_szCurrUrl, szUrl );
    return szData;
  }
  
  {
    HINTERNET hSession = NULL, hURL = NULL;
    int k, len;
    char *szData = NULL;
    DWORD dwRead;
    char szSize[ 25 ] = {0};
    char szLocation[ 2048 ];
    hSession = InternetOpen( "Kalles Browser", INTERNET_OPEN_TYPE_DIRECT , NULL, NULL, 0 );
    if ( !hSession )
      return NULL;
    strcpy( szLocation, "Referer: " );
    strcat( szLocation, me->m_szCurrUrl );
    if ( me->m_bMainRequest )
      strcpy( me->m_szCurrUrl, szUrl );
    if ( !( hURL = InternetOpenUrl( hSession, szUrl, szLocation, -1, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_NO_AUTO_REDIRECT , 0 ) ) )
      return NULL;
    len = sizeof( szSize );
    k = 0;
    HttpQueryInfo( hURL, HTTP_QUERY_CONTENT_LENGTH , szSize, ( ULONG* ) & len, ( ULONG* ) & k );
    len = sizeof( szLocation );
    k = 0;
    *szLocation = 0;
    HttpQueryInfo( hURL, HTTP_QUERY_LOCATION, szLocation, ( ULONG* ) & len, ( ULONG* ) & k );
    for ( len = 0, k = 0, len = 1000;;len += 1000 ) {
      szData = ( char* ) realloc( szData, len );
      if ( !InternetReadFile( hURL, &szData[ k ], len - k, &dwRead ) || !dwRead )
        break;
      k += dwRead;
      CHTMLControl_Tick( me, k, atoi( szSize ) );
    }
    szData = ( char* ) realloc( szData, len + 1 );
    *pnSize = k + dwRead;
    szData[ k + dwRead ] = 0;
    if ( me->m_bMainRequest ) {
      for ( k = 0;k < *pnSize;k++ ) {
        if ( !szData[ k ] )
          szData[ k ] = ' ';
      }
    }
    InternetCloseHandle( hURL );
    InternetCloseHandle( hSession );
    if ( me->m_bMainRequest && *szLocation ) {
      CHTMLViewer_AddBaseToUrl( me->m_pViewer, szLocation, me->m_szCurrUrl, szUrl );
      return CHTMLControl_ReadHTTPData( me, me->m_szCurrUrl, pnSize );
    } else
      return szData;
  }
}


void CHTMLControl_CalcPos(CHTMLControl* me) {
  RECT r;
  CHTMLViewer_CalcPositions1( me->m_pViewer, &r );
}
void CHTMLControl_LinkClicked( CHTMLControl* me, char *szLink ) {
  SetCursor( LoadCursor( NULL, IDC_WAIT ) );
  *me->m_szCurrStatus = 0;
  if ( !strnicmp( szLink, "Submit(", 7 ) ) {
    //Submit( atoi( &szLink[ 7 ] ) );
    return ;
  }
  if ( strrchr( szLink, '.' ) && ( !stricmp( strrchr( szLink, '.' ) + 1, "jpg" ) || !stricmp( strrchr( szLink, '.' ) + 1, "gif" ) || !stricmp( strrchr( szLink, '.' ) + 1, "png" ) ) ) {
    char *szTmp = (char*)malloc(strlen( szLink ) + 100 );
    strcpy( szTmp, "<body bgcolor=ffff00><img src=\"" );
    strcat( szTmp, szLink );
    strcat( szTmp, "\"></body>" );
    CHTMLViewer_SetHTML( me->m_pViewer, szTmp, me->m_szFontName, me->m_nFontSize, RGB( 0, 0, 0 ), "" );
    free(szTmp);
  } else {
    static int nIndex = 0;
    char szFileName[ 256 ];
    int nSize;
    char *szData;
    char *szUrl;
    HANDLE hFile;
    DWORD dw;

    szUrl = (char*)malloc( strlen( szLink ) + 1 );
    strcpy( szUrl, szLink );
    printf( "From Internet" );
    me->m_bMainRequest = 1;
    szData = CHTMLControl_ReadHTTPData( me, szUrl, &nSize );
    me->m_bMainRequest = 0;
    if ( !szData ) {
      free(szUrl);
      return ;
    }
    GetModuleFileName( GetModuleHandle( NULL ), szFileName, sizeof( szFileName ) );
    *strrchr( szFileName, '.' ) = 0;
    strcat( szFileName, ".txt" );
    hFile = CreateFile( szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
    WriteFile( hFile, szData, nSize, &dw, NULL );
    CloseHandle( hFile );

    CHTMLViewer_SetHTML( me->m_pViewer, szData, me->m_szFontName, me->m_nFontSize, RGB( 0, 0, 0 ), me->m_szCurrUrl );
    printf( "%s\n", me->m_szCurrUrl );
    free(szUrl);
    free(szData);
  }
  CHTMLControl_CalcPos(me);
  SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}
void CHTMLControl_SetCursor( CHTMLControl* me,BOOL bLink, char *szLink ) {
  if ( !me->m_szActiveUrl || strcmp( me->m_szActiveUrl, szLink ) ) {
    if ( me->m_szActiveUrl )
      CHTMLViewer_UpdateLink( me->m_pViewer, me->m_szActiveUrl );
    free(me->m_szActiveUrl);
    me->m_szActiveUrl = (char*)malloc( strlen( szLink ) + 1 );
    strcpy( me->m_szActiveUrl, szLink );
  }
#define PrintStatus(x)  printf("%s\n", x)
  if ( bLink ) {
    SetCursor( LoadCursor( NULL, MAKEINTRESOURCE( 32649 ) ) );
    CHTMLViewer_UpdateLink( me->m_pViewer, szLink );
    printf( "%s\n", szLink );
  } else {
    SetCursor( LoadCursor( NULL, IDC_ARROW ) );
    PrintStatus( me->m_szCurrStatus );
  }
}
void CHTMLControl_SetTitle( CHTMLControl* me,char *szTitle ) {
  PrintStatus( szTitle );
}
char *CHTMLControl_PostHTTPData( CHTMLControl* me,char *szUrl, char *szPostData, int *pnSize ) {
  char szHeader[] = "Content-Type: application/x-www-form-urlencoded";
  char szAccept[] = "Accept: */*";
  HINTERNET hSession, hConnect, hRequest;
  char szHost[ 256 ];
  char szStatus[ 100 ];
  char *szData = NULL;
  DWORD dwRead;
  char szSize[ 25 ] = {0};
  int i, j, len, k;
  *me->m_szCurrStatus = 0;
  for ( i = 0;szUrl[ i ] && strnicmp( &szUrl[ i ], "://", 3 );i++ )
    ;
  if ( szUrl[ i ] )
    i += 3;
  for ( j = 0;szUrl[ i ] && szUrl[ i ] != '/';i++ )
    szHost[ j++ ] = szUrl[ i ];
  szHost[ j ] = 0;
  if ( ( hSession = InternetOpen( "Kalles Browser",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0 ) ) &&
    // Connect to Internet server
    ( hConnect = InternetConnect( hSession,szHost,INTERNET_DEFAULT_HTTP_PORT,NULL,NULL,INTERNET_SERVICE_HTTP,0,1 ) ) &&
    // Open POST-request to the web-page
    ( hRequest = HttpOpenRequest( hConnect,"POST",&szUrl[ i ],NULL,NULL,( const char** ) & szAccept,INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD,1 ) ) &&
    // Send Request with postfields
    HttpSendRequest( hRequest,szHeader,strlen( szHeader ),szPostData,strlen( szPostData ) ) ) {
    len = sizeof( szSize );
    k = 0;
    HttpQueryInfo( hRequest, HTTP_QUERY_CONTENT_LENGTH , szSize, ( ULONG* ) & len, ( ULONG* ) & k );
    for ( len = 0, k = 0, len = 1000;;len += 1000 ) {
      szData = ( char* ) realloc( szData, len );
      if ( !InternetReadFile( hRequest, &szData[ k ], len - k, &dwRead ) || !dwRead )
        break;
      k += dwRead;
      wsprintf( szStatus, "Reading %d of %s", k, szSize );
      PrintStatus( szStatus );
    }
    szData = ( char* ) realloc( szData, len + 1 );
    *pnSize = k + dwRead;
    szData[ k + dwRead ] = 0;
    InternetCloseHandle( hRequest );
    InternetCloseHandle( hConnect );
    InternetCloseHandle( hSession );
    return szData;
  }
  return NULL;
}
int CHTMLControl_CreateForm( CHTMLControl* me, char *szAction, char *szMethod ) {
  return 0;
}
int CHTMLControl_CreateTextBox( int nSize, int nMaxChar, SIZE scSize, char *szText, BOOL bMultiline, char *szName, int nForm, BOOL bPassword ) {
  return 0;
}
int CHTMLControl_CreateButton( CHTMLControl* me,SIZE scSize, char *szText, char *szName, int nForm ) {
  return 0;
}
int CHTMLControl_CreateRadio( CHTMLControl* me,SIZE scSize, char *szValue, char *szName, int nForm ) {
  return 0;
}
int CHTMLControl_CreateCheckbox( CHTMLControl* me,SIZE scSize, char *szValue, char *szName, int nForm ) {
  return 0;
}
int CHTMLControl_CreateListbox( CHTMLControl* me,SIZE scSize, CHTMLStringTable* stOptions, int nSelected, char *szName, int nForm ) {
  return 0;
}
void CHTMLControl_MoveControl( CHTMLControl* me,int nControlID, POINT pPos ) {
  return ;
}
int CHTMLControl_ProcessMessage( CHTMLControl* me,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  return 0;
}
void CHTMLControl_SetHTML( CHTMLControl* me,char *szHTMLData, char *szBaseUrl ) {
  CHTMLViewer_SetHTML( me->m_pViewer, szHTMLData, me->m_szFontName, me->m_nFontSize, RGB( 0, 0, 0 ), szBaseUrl );
  CHTMLControl_CalcPos(me);
}
int CHTMLControl_EnableHistory(CHTMLControl* me) {
  return 0;
}
void CHTMLControl_GoBack() {
}
void CHTMLControl_Copy(CHTMLControl* me) {
  CHTMLViewer_CopySelected(me->m_pViewer);
}
char *CHTMLControl_UrlEncode( CHTMLControl* me,char *szStr ) {
  int i, j;
  int nLen = 0;
  char *szRet;
  for ( i = 0;szStr[ i ];i++ ) {
    if ( ( szStr[ i ] >= 'a' && szStr[ i ] <= 'z' ) || ( szStr[ i ] >= 'A' && szStr[ i ] <= 'Z' ) || ( szStr[ i ] >= '0' && szStr[ i ] <= '9' ) || szStr[ i ] == ' ' )
      nLen++;
    else
      nLen += 3;
  }
  szRet = (char*)malloc( nLen + 1 );
  for ( i = j = 0;szStr[ i ];i++ ) {
    if ( ( szStr[ i ] >= 'a' && szStr[ i ] <= 'z' ) || ( szStr[ i ] >= 'A' && szStr[ i ] <= 'Z' ) || ( szStr[ i ] >= '0' && szStr[ i ] <= '9' ) )
      szRet[ j++ ] = szStr[ i ];
    else if ( szStr[ i ] == ' ' )
      szRet[ j++ ] = '+';
    else {
      szRet[ j++ ] = '%';
      wsprintf( &szRet[ j ], "%02x", ( unsigned char ) szStr[ i ] );
      j += 2;
    }
  }
  szRet[ j ] = 0;
  return szRet;
}
void CHTMLControl_ImageLoaded( CHTMLControl* me,int nDone, int nCount ) {
  int nAntal = 10;
  while ( nAntal < nCount )
    nAntal *= 10;
  nAntal /= 10;
  if ( nDone == nCount ) {
    int nItems, nImages, nLinks, nLevel;
    CHTMLViewer_Statistics( me->m_pViewer, &nItems, &nImages, &nLinks, &nLevel );
    wsprintf( me->m_szCurrStatus, "Ready, items:%d, images:%d, links:%d, level:%d", nItems, nImages, nLinks, nLevel );
    PrintStatus( me->m_szCurrStatus );
  } else {
    char szStatus[ 50 ];
    wsprintf( szStatus, "Loaded Image %d of %d", nDone, nCount );
  }
}
void CHTMLControl_UpdateScreen( CHTMLControl* me,RECT* r ) {
}

