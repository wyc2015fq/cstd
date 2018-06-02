
#include <stdio.h>
#include "codec.h"

#define DATAFILE "data.txt"

// BOOL edcfile(const char* infile, const char* oufile, const char* ppass, int type);

int usage( const char* exename ) {
  static const char * usage_str =
    "usage:  %s input ouput pass [type=0]\n"
    "\ttype=[0=º”√‹, 1=Ω‚√‹]\n";
  printf( usage_str, exename );
  return 0;
}

int main( int argc, char** argv ) {
  int type = 0;
  if ( argc < 4 ) {
    usage( argv[ 0 ] );
    return 0;
  }
  if ( argc > 4 ) {
    sscanf( argv[ 4 ], "%d", &type );
  }
  edcfile( argv[ 1 ], argv[ 2 ], argv[ 3 ], type );
  return 0;
}
