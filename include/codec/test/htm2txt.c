
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "win.h"
#include "chalg.h"
#include "sys/findfile.h"
#include "str/iconv.inl"
//#include "parse.h"
//#include "codec.inl"
//#include "fileio.h"
//#include "debug.h"

typedef const char* II;
typedef char* OI;

struct spchar {
  char chr;
  int len;
  char* str;
};

static const struct spchar htmchar[] =   //
  {
    {' ', 6, "&nbsp;"
    },
    {'<', 4, "&lt;" },
    {'>', 4, "&gt;" },
    {'&', 5, "&amp;" },
    {'"', 6, "&quot;" },
    {'<', 7, "&laquo;" },
    {'>', 7, "&raquo;" },
    {'<', 8, "&ldquo;" },
    {'>', 8, "&rdquo;" },
  };
  
const static char* strmap[] = {
                                "&ldquo;", "“"
                                "&rdquo;", "”"
                                "&middot;", "·"
                                "&hellip;", "…"
                              };
                              
static const struct spchar htmflag[] =   //
  {
    {'\0', 4, "span"},
    {'\0', 6, "script" },
    {'\0', 5, "style" },
    {'\n', 2, "br" },
    {'\n', 2, "h3" },
    {'\n', 2, "li" },
    {'\n', 3, "div" },
    {'\n', 3, "pre" },
    {'\n', 1, "p" },
    {'\0', 2, "--" },
    {'\0', 1, "a" },
    {'\0', 5, "title" },
    {'\0', 4, "html" },
    {'\0', 4, "head" },
    {'\0', 4, "meta" },
    {'\0', 4, "body" },
    {'\0', 5, "table" },
    {'\0', 4, "link" },
    {'\0', 8, "!doctype" },
  };
  
const struct spchar* ishtmflag( const char* p, const struct spchar* f, const struct spchar* l ) {
  for ( ; f != l; ++f ) {
    if ( !memicmp( p, f->str, f->len ) ) {
      return f;
    }
  }
  return ( 0 );
}

#define ISDIGIT(ch)   ('0'<=(ch) && (ch)<='9')
#define CHAR2NUMBER(is_pos, ch)   ((is_pos) ? ((ch) - '0') : ('0' - (ch)))
const char* strnchr( const char* f, const char* l, int chr ) {
  char * p = memchr( ( f ), ( chr ), ( char* ) ( l ) - ( char* ) ( f ) );
  return p ? p : l;
}

size_t htm2txt_impl( II src, size_t srclen, OI dst ) {
  II f = src;
  II l = src + srclen;
  OI p = dst;
  OI q = p;
  const struct spchar* flag;
  for ( ; f < l; ) {
    if ( '<' == *f ) {
      if ( flag = ishtmflag( f + 1, htmflag, htmflag + 9 ) ) {
        int k = flag->len;
        if ( flag->chr ) {
          *p++ = flag->chr;
          f = strnchr( f + k + 1, l, '>' );
          f += f != l;
        } else {
          II m = strnchr( f + k + 1, l, '>' );
          if ( !memicmp( f, "<span", 5 ) ) {
            II t = strnchr( f, m, 'd' );
            if ( t && ( t == m || memicmp( t, "display:none", 12 ) ) ) {
              f = m + ( f != l );
              continue;
            }
          }
          f = m + ( m != l );
          for ( ; f != l; f += f != l ) {
            f = strnchr( f, l, '>' );
            if ( !memicmp( f - k, flag->str, k ) ) {
              break;
            }
          }
          f += f != l;
        }
      } else if ( !memicmp( f + 1, "!--", 3 ) ) {
        II m = strstr( f + 3, "-->" );
        f = ( m ) ? m + 3 : l;
        SKIP_WHITESPACE( f, l );
      } else {
        II m = strchr( f + 1, '>' );
        f = ( m ) ? m + 1 : l;
      }
    } else if ( '&' == *f && ( flag = ishtmflag( f, htmchar, htmchar + 9 ) ) ) {
      *p++ = flag->chr;
      f += flag->len;
    } else if ( '&' == *f && '#' == f[ 1 ] ) {
      unsigned short wi = 0;
      II i;
      II m;
      m = strchr( f += 2, ';' );
      if ( m ) {
        if ( 'x' == *f || 'X' == *f ) {
          sscanf( f + 1, "%x", &wi );
        } else {
          sscanf( f, "%d", &wi );
        }
        i = ( char* ) & wi;
        // UNICODETOGB2312(i, p, i, p);
        p += chr_wctomb( &wi, 1, p, 10 );
        f = m + 1;
      }
      //} else if ('\n'==*f) {      ++f;
      //} else if (0x0a==*f && 0x0d!=f[-1]) {   *p++ = '\n'; ++f;
    } else {
      *p++ = *f++;
    }
  }
#define RSKIP_WHITESPACE(q, p)  for (;q<p && (' '==p[-1] || '\t'==p[-1] );--p);
  RSKIP_WHITESPACE( q, p );
  //if (p)
  *p = '\0';
  return p -dst;
}

// 第n次chr出现的指针
char* strchrn( char* a, int chr, size_t n ) {
  for ( ; a && n--; ++a ) {
    a = strchr( a, chr );
  }
  return a;
}

// str出现的下一行指针
char* strchrstr( char* a, int chr, const char* str ) {
  a = strstr( a, str );
  if ( a && ( a = strchr( a, chr ) ) ) {
    ++a;
  }
  return a;
}

char* dellines( char* p, int from, int to ) {
  char * a = p;
  char* b;
  int i;
  if ( from > to ) {
    i = from;
    from = to;
    to = i;
  }
  a = strchrn( a, '\n', from );
  b = strchrn( a, '\n', to - from );
  return strcpy( a, b );
}

int htm2txt(const char* ext, int isutf8) {
  size_t bufsize = 0;
  char* buf = 0;
  char* out = 0;
  size_t outlen = 0;
  char outname[ 256 ];
  begin_find( info, ext );
  char * fname = fileinfo_filename( info );
  size_t fsize = fileinfo_filesize( info );
  FILE* pfin = fopen( fname, "r" );
  char* p;
  FILE* pfout;
  //printf("%s\n", fileinfo_filename(info));
  if ( pfin ) {
    if ( bufsize < fsize ) {
      bufsize = fsize + 2;
      buf = ( char* ) realloc( buf, bufsize );
      out = buf;
    }
    memset( buf, 0, bufsize );
    //FILESIZE(pfin, fsize);
    fread( buf, fsize, 1, pfin );
    fclose( pfin );
    fsize = iconv_s( -1, buf, fsize, CE_GB2312, buf, fsize );
    buf[ fsize ] = '\0';
    outlen = htm2txt_impl( buf, fsize, out );
    if ( outlen > 0) {
      _snprintf( outname, 256, "%s", fname );
      p = strrchr( outname, '.' );
      strcpy( p, ".txt" );
      pfout = fopen( outname, "wb" );
      if ( pfout ) {
        printf( "%s\r", outname );
        p = out;
        SKIP_WHITESPACE( p, out + outlen );
        fwrite( p, outlen, 1, pfout );
        fclose( pfout );
#ifdef _DEBUG
#define REMOVE(_FNAME) {}
#else
#define REMOVE(_FNAME) remove(_FNAME)
#endif
        
        //REMOVE( fname );
      }
    }
  }
  end_find( info );
  if ( buf ) {
    free( buf );
  }
  return 0;
}

int main( int argc, char** argv ) {
  //_chdir("D:\\www\\mv");
  htm2txt(argc>1?argv[1]:"*.html", argc>2);
  return 0;
}
