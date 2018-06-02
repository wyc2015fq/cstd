
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "macro.h"
#include "findfile.h"
#include "parse.h"

int ttd( int argc, char** argv );

int main( int argc, char** argv ) {
  ttd( argc, argv );
  return 0;
}

char* ttd_impl( char* f, char* l, char* p ) {
  int len = 0;
  for ( ; *f; ) {
    char* f0 = f + strspn( f, " \t\n" );
    char* f1 = f;
    char* f2 = NULL;
    if ( f0 > l ) {
      f0 = l;
    }
    for ( ; f1 && *f1 && f1 <= f0; ) {
      f1 = strchr( f1 + 1, '\n' );
      if ( f1 ) {
        f2 = f1;
      } else {
        break;
      }
    }
    if ( f1 && f1 > f0 ) {
      f2 = f;
      f0 = f1;
    }
    if ( f2 ) {
      //++f2;
      len = f0 - f2;
      memcpy( p, f2, len );
      f = f2 + len;
      p += len;
      *p++ = '\n';
      continue;
    }
    strcpy( p, f );
    p += strlen( p );
    break;
  }
  return p;
}

int ttd( int argc, char** argv ) {
  char * buf = 0;
  char* out = 0;
  char* out_end;
  size_t bufsize = 0;
  //char outname[256];
  char* spc = "*.txt";
  if ( argc > 1 ) {
    spc = argv[ 1 ];
  }
  begin_find( info, spc )
  char * fname = fileinfo_filename( info );
  size_t fsize = fileinfo_filesize( info );
  FILE* pf = fopen( fname, "r" );
  //printf("%s\n", fileinfo_filename(info));
  if ( pf ) {
    if ( !fsize ) {
      continue;
    }
    if ( bufsize < fsize ) {
      SAFEFREE( buf );
      bufsize = fsize;
      buf = ( char* ) malloc( bufsize + 2 );
      out = buf;
    }
    memset( buf, 0, bufsize + 2 );
    fread( buf, fsize, 1, pf );
    fclose( pf );
    //remove(fname);
    *( buf + fsize ) = '\0';
    out_end = ttd_impl( buf, buf + fsize, out );
    {
      char* p;
      FILE* pfout;
      // _snprintf(outname, 256, "%s", fname);
      // p = strrchr(outname, '.');
      // strcpy(p, ".txt");
      pfout = fopen( fname, "w" );
      if ( pfout ) {
        int len;
        printf( "%s\n", fname );
        p = out;
        SKIP_WHITESPACE( p, out_end );
#if 0
        
        *out_end = '\0';
        dellines( p, 3, 71 );
        len = strlen( p );
#else
        
        len = out_end - out;
#endif
        
        fwrite( p, len, 1, pfout );
        fclose( pfout );
      }
    }
  }
  end_find( info );
  SAFEFREE( buf );
  return 0;
}
