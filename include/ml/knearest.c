
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "xtypes.h"
#include "sort.h"
#include "ml.h"
#include "distance.h"

PVECTOR VECTOR_NEW( int size, int dims ) {
  PVECTOR vec = MALLOC( VECTOR, 1 );
  vec->data = MALLOC( unsigned char, ( size * dims ) );
  vec->size = size;
  vec->dims = dims;
  vec->used = 0;
  return vec;
}

void VECTOR_DEL( PVECTOR vec ) {
  SAFEFREE( vec->data );
  SAFEFREE( vec );
}

void* VECTOR_PUSH_BACK( PVECTOR vec, int num, void* ptr, int len, int step ) {
  unsigned char * out, *_ptr = ( unsigned char* ) ptr;
  int i, newused = vec->used + num;
  if ( newused > vec->size ) {
    vec->size = MAX( ( vec->size ) * 2, newused );
    vec->data = ( unsigned char* ) realloc( vec->data, vec->size * vec->dims );
  }
  out = vec->data + vec->used * vec->dims;
  vec->used = newused;
  if ( ptr && len ) {
    len = MIN( len, vec->dims );
    for ( i = 0; i < num; ++i )
      memcpy( out + i * vec->dims, _ptr + i * step, len );
  }
  return out;
}

int find_nearest( PVECTOR vec, void* sample, PNEAREST out, int len,
                  real ( *distor ) ( const void*, const void*, const void* ), const void* userdata ) {
  PNEAREST ptr = MALLOC( NEAREST, vec->used );
  int i;
  for ( i = 0; i < vec->used; ++i ) {
    ptr[ i ].data = ( real* ) ( vec->data + i * vec->dims );
    //printf("%d\n", *(int*)(ptr[i].data+2));
    ptr[ i ].dist = distor( sample, ptr[ i ].data, userdata );
  }
#define NEAREST_LESS(nst1, nst2)  ((nst1).dist < (nst2).dist)
  QSORT( ptr, ptr + vec->used, NEAREST_LESS, NEAREST );
#undef NEAREST_GREAT
  
  len = MIN( len, vec->used );
  memcpy( out, ptr, len * sizeof( NEAREST ) );
  SAFEFREE( ptr );
  return len;
}

real chi_square( const void* a, const void* b, const void* userdata ) {
  real * ra = ( real* ) a;
  real* rb = ( real* ) b;
  int len = *( int* ) userdata;
  real dist = 0.f;
  CHI_SQUARE( len, ra, rb, dist, real );
  return dist;
}
