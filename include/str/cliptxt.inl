
#include <stdio.h>
 
#define AfxMessageBox printf

#include "cstd.h"
#include "cfile.h"

// 写文件
int test_ren1line_0( const char* txt ) {
  const char * ext = ".txt";
  int ret = 1, i, len;
  FILE* pf;
  char buf[ 256 ] = {0};
  char name[ 256 ] = {0};
  len = strlen( txt );
  i = str2filename( txt, len, buf, 256 );
  buf[ i ] = '\0';
  _snprintf( name, 256, ".\\%s%s", buf, ext );
  {
    int namelen = strlen( name );
    char* pp = name + namelen;
    for ( i = 0; file_exist( name ); ++i ) {
      _snprintf( pp, 256 - namelen, "_%d%s", i, ext );
    }
  }
  printf( "%s\n", name );
  pf = fopen( name, "wb" );
  if ( pf ) {
    fwrite( txt, strlen( txt ), 1, pf );
    fclose( pf );
  } else {
    ret = 0;
    printf( "打开文件%s失败", name );
  }
  return ret;
}

int test_FreePic2Pdf_bkmk() {
  return 0;
}

int cliptxt() {
  while ( 1 ) {
    char* pchData = sys_get_clipboard_text_static();
    if ( strlen( pchData ) > 256 ) {
      test_ren1line_0( pchData );
    }
    sys_sleep( 500 );
  }
  return 0;
}
