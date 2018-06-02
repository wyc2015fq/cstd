
#include <windows.h>
#include <direct.h>
#include <assert.h>
#include <stdio.h>
#include "strex.h"

#define _MAX_PATH_LEN  (256)

long ReplaceFileNameRecursion( LPCTSTR szSourceDir, LPCTSTR szFileExt, LPCTSTR szCmd, BOOL m_subFolder ) {
  int m_cnt = 0;
  long lPackageSize = 0;
  long ltempsize = 0;
  HANDLE hFind;
  WIN32_FIND_DATA fd;
  TCHAR SrcFullFileName[ _MAX_PATH_LEN ];
  TCHAR DstFullFileName[ _MAX_PATH_LEN ];
  _tcscpy( SrcFullFileName, szSourceDir );
  _tcscat( SrcFullFileName, szFileExt );
  
  /* Get all the contents of source directory */
  if ( ( hFind = FindFirstFile( SrcFullFileName, &fd ) ) != INVALID_HANDLE_VALUE ) {
    do {
      /* Get the name of the directory's contents */
      LPCTSTR szFileName = fd.cFileName;
      if ( _tcscmp( szFileName, _T( "." ) ) && _tcscmp( szFileName, _T( ".." ) ) ) {
        /* If content is a sub directory */
        LPTSTR stmp = SrcFullFileName;
        _tcscpy( stmp, szFileName );
        StrOpt( stmp, szCmd );
        _tcscpy( DstFullFileName, szSourceDir );
        _tcscat( DstFullFileName, stmp );
        _tcscpy( SrcFullFileName, szSourceDir );
        _tcscat( SrcFullFileName, szFileName );
        _trename( SrcFullFileName, DstFullFileName );
        m_cnt++;
      }
      lPackageSize += fd.nFileSizeLow;
    } while ( FindNextFile( hFind, &fd ) );
    FindClose( hFind );
  }
  
  if ( m_subFolder ) {
    _tcscpy( DstFullFileName, szSourceDir );
    _tcscat( DstFullFileName, _T( "*.*" ) );
    /* Get all the contents of source directory */
    if ( ( hFind = FindFirstFile( DstFullFileName, &fd ) ) != INVALID_HANDLE_VALUE ) {
      /* Get the name of the directory's contents */
      do {
        /* If content is a sub directory */
        LPCTSTR szFileName = fd.cFileName;
        if ( _tcscmp( szFileName, _T( "." ) ) && _tcscmp( szFileName, _T( ".." ) ) ) {
          _tcscpy( DstFullFileName, szSourceDir );
          _tcscat( DstFullFileName, szFileName );
          _tcscat( DstFullFileName, _T( "\\" ) );
          if ( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) { //如果目录
            ReplaceFileNameRecursion( DstFullFileName, szFileExt, szCmd, m_subFolder );
          }
        }
        lPackageSize += fd.nFileSizeLow;
      } while ( FindNextFile( hFind, &fd ) );
      FindClose( hFind );
    }
  }
  return lPackageSize;
}

long ReplaceFileName( LPCTSTR szSource, LPCTSTR szCmd, BOOL m_subFolder ) {
  TCHAR szSourceDir[ _MAX_PATH_LEN ];
  LPCTSTR szFileExt = NULL;
  _tcscpy( szSourceDir, szSource );
  StrReplaceChr( szSourceDir, '/', '\\' );
  szFileExt = _tcsrchr( szSourceDir, '\\' );
  if ( !szFileExt ) {
    szFileExt = szSource[ 0 ] ? szSource : _T( "*.*" );
    _tcscpy( szSourceDir, _T( ".\\" ) );
  } else {
    int nPos = szFileExt - szSourceDir + 1;
    szSourceDir[ nPos ] = '\0';
    szFileExt = szSource + nPos;
  }
  return ReplaceFileNameRecursion( szSourceDir, szFileExt, szCmd, m_subFolder );
}

int test_renx( int argc, TCHAR* argv[] ) {
  int i, j;
  int m_subFolder = 0;
  LPCTSTR argstr[ 2 ] = {_T( "." ), _T( "" ) };
  if ( argc < 2 ) {
    printf( "renx [filename] [cmd string]\n" );
    printf( "renx *.txt r/123/45\n 用45替换123\n" );
    return 0;
  }
  for ( j = 0, i = 1; i < argc; ++i ) {
    if ( 0 == _tcscmp( _T( "/s" ), argv[ i ] ) ) {
      m_subFolder = 1;
    } else {
      argstr[ j++ ] = argv[ i ];
    }
  }
  if ( argstr[ 1 ][ 0 ] ) {
    ReplaceFileName( argstr[ 0 ], argstr[ 1 ], m_subFolder );
  }
  return 0;
}

int test_strex() {
  TCHAR str[ 256 ] = _T( "1234567" );
  StrOpt( str, _T( "r/123/45" ) );
  StrOpt( str, _T( "d/1/2" ) );
  StrOpt( str, _T( "i/^/123" ) );
  StrOpt( str, _T( "i/$/89" ) );
  _tcscpy( str, _T( "a str delete" ) );
  StrOpt( str, _T( "c" ) );
  return 0;
}

int main( int argc, TCHAR* argv[] ) {
  _chdir("F:/book/_iask/end/__/科学史(112)");
  test_renx( argc, argv );
  //test_xfind();
  return 0;
}
