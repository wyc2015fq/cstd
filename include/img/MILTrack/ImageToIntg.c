
#include <string.h>
#include "_MILTrack.h"

#define cast_macro(X) (X)

int icvIntegralImage( const unsigned char* src, int srcstep, int* sum, int sumstep, int size_height, int size_width ) {
  int x, y;
  int s;
  memset( sum, 0, ( size_width + 1 ) * sizeof( sum[ 0 ] ) );
  sum += sumstep + 1;
  
  for ( y = 0; y < size_height; y++, src += srcstep, sum += sumstep ) {
    sum[ -1 ] = 0;
    for ( x = 0, s = 0; x < size_width; x++ ) {
      int t = cast_macro( src[ x ] );
      s += t;
      sum[ x ] = sum[ x - sumstep ] + s;
    }
  }
  return 0;
}
