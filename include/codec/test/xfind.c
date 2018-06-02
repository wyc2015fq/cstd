
#define XFIND_MAX_DEEP (16)
#define STATOP(lpFindFileInfo)  ((lpFindFileInfo->hStackTop - lpFindFileInfo->hStack) < XFIND_MAX_DEEP)


typedef struct XFindInfo {
  WIN32_FIND_DATA fd;
  HANDLE hFind;
  HANDLE hStack[ XFIND_MAX_DEEP ];
  HANDLE* hStackTop;
  TCHAR szSourceDir[ _MAX_PATH_LEN ];
  LPCTSTR szFileExt;
  BOOL m_subFolder;
}
XFindInfo;

HANDLE XFindFirstFileImpl( XFindInfo* lpFindFileInfo ) {
  HANDLE hFind = INVALID_HANDLE_VALUE;
  HANDLE hFind2 = INVALID_HANDLE_VALUE;
  LPTSTR szSourceDir = lpFindFileInfo->szSourceDir;
  LPCTSTR szFileExt = lpFindFileInfo->szFileExt;
  LPTSTR pszDir;
  _tcscat( szSourceDir, szFileExt );
  hFind = FindFirstFile( ( LPCSTR ) szSourceDir, &( lpFindFileInfo->fd ) );
  assert( STATOP( lpFindFileInfo ) );
  if ( ( hFind ) != INVALID_HANDLE_VALUE ) {
    lpFindFileInfo->hFind = hFind;
    return hFind;
  }
  if ( lpFindFileInfo->m_subFolder ) {
    WIN32_FIND_DATA fd;
    pszDir = _tcsrchr( szSourceDir, '\\' );
    pszDir[ 1 ] = '\0';
    _tcscat( szSourceDir, "*.*" );
    /* Get all the contents of source directory */
    if ( ( hFind2 = FindFirstFile( ( LPCSTR ) szSourceDir, &fd ) ) != INVALID_HANDLE_VALUE ) {
      /* Get the name of the directory's contents */
      pszDir = _tcsrchr( szSourceDir, '\\' );
      pszDir[ 1 ] = '\0';
      do {
        /* If content is a sub directory */
        LPCTSTR szFileName = fd.cFileName;
        if ( _tcscmp( szFileName, _T( "." ) ) && _tcscmp( szFileName, _T( ".." ) ) ) {
          if ( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) { //如果目录
            _tcscat( szSourceDir, szFileName );
            _tcscat( szSourceDir, "\\" );
            hFind = XFindFirstFileImpl( lpFindFileInfo );
            if ( hFind != INVALID_HANDLE_VALUE ) {
              lpFindFileInfo->hFind = hFind;
              *( lpFindFileInfo->hStackTop++ ) = hFind2;
              break;
            }
            pszDir[ 1 ] = '\0';
          }
        }
      } while ( FindNextFile( hFind2, &fd ) );
      FindClose( hFind2 );
    }
  }
  pszDir = _tcsrchr( szSourceDir, '\\' );
  pszDir[ 1 ] = '\0';
  return lpFindFileInfo;
}

HANDLE XFindFirstFile( LPCTSTR lpFileName, XFindInfo* lpFindFileInfo, BOOL m_subFolder ) {
  LPCTSTR szFileExt = NULL;
  LPTSTR szSourceDir = lpFindFileInfo->szSourceDir;
  lpFindFileInfo->hStackTop = lpFindFileInfo->hStack;
  lpFindFileInfo->m_subFolder = m_subFolder;
  _tcscpy( szSourceDir, lpFileName );
  StrReplaceChr( szSourceDir, '/', '\\' );
  szFileExt = _tcsrchr( szSourceDir, '\\' );
  if ( !szFileExt ) {
    szFileExt = "*.*";
    _tcscpy( szSourceDir, ".\\" );
    _tcscat( szSourceDir, szSourceDir );
  } else {
    int nPos = szFileExt - szSourceDir + 1;
    szSourceDir[ nPos ] = '\0';
    szFileExt = lpFileName + nPos;
  }
  lpFindFileInfo->szFileExt = szFileExt;
  return XFindFirstFileImpl( lpFindFileInfo );
}

BOOL XFindNextFile( HANDLE hFindFile, XFindInfo* lpFindFileInfo ) {
  BOOL bFind = 0;
  LPTSTR szSourceDir = lpFindFileInfo->szSourceDir;
  LPCTSTR szFileExt = lpFindFileInfo->szFileExt;
  hFindFile = lpFindFileInfo->hFind;
  bFind = FindNextFile( hFindFile, &( lpFindFileInfo->fd ) );
  if ( bFind ) {
    return bFind;
  }
  FindClose( hFindFile );
  lpFindFileInfo->hFind = INVALID_HANDLE_VALUE;
  if ( lpFindFileInfo->m_subFolder ) {
    HANDLE hFind = XFindFirstFileImpl( lpFindFileInfo );
    if ( ( hFind ) != INVALID_HANDLE_VALUE ) {
      lpFindFileInfo->hFind = hFind;
      return 1;
    }
    if ( lpFindFileInfo->hStackTop != lpFindFileInfo->hStack ) {
      WIN32_FIND_DATA fd;
      HANDLE hFind2 = *( lpFindFileInfo->hStackTop-- );
      FindClose( hFind2 );
      hFind2 = *( lpFindFileInfo->hStackTop );
      if ( FindNextFile( hFind2, &fd ) ) {
        TCHAR szSource[ _MAX_PATH_LEN ];
        LPTSTR pszDir;
        szSourceDir[ _tcsclen( szSourceDir ) - 1 ] = '\0';
        pszDir = _tcsrchr( szSourceDir, '\\' );
        /* Get the name of the directory's contents */
        do {
          /* If content is a sub directory */
          LPCTSTR szFileName = fd.cFileName;
          if ( _tcscmp( szFileName, _T( "." ) ) && _tcscmp( szFileName, _T( ".." ) ) ) {
            if ( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) { //如果目录
              _tcscat( szSourceDir, szFileName );
              _tcscat( szSource, "\\" );
              hFind = XFindFirstFileImpl( lpFindFileInfo );
              if ( hFind != INVALID_HANDLE_VALUE ) {
                lpFindFileInfo->hFind = hFind;
                return 1;
              }
              pszDir[ 1 ] = '\0';
            }
          }
        } while ( FindNextFile( hFind2, &fd ) );
        FindClose( hFind2 );
      }
    }
  }
  return bFind;
}

int test_xfind() {
  XFindInfo fi;
  HANDLE hFile = XFindFirstFile( "C:/aa/*.txt", &fi, 1 );
  if ( hFile != INVALID_HANDLE_VALUE ) {
    do {
      printf( "%s%s\n", fi.szSourceDir, fi.fd.cFileName );
    } while ( XFindNextFile( hFile, &fi ) );
  }
  return 0;
}
