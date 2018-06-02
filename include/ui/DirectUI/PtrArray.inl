
#ifdef __cplusplus1

template <typename T, typename Y>
inline T* _ptr_cast( T* p, Y* q ) {
  return ( T* ) q;
}
#define ptr_cast(_p, _q)  (_p = _ptr_cast(_p, _q))
#else
#define ptr_cast(_p, _q)  (_p = (void*)(_q))
#endif

#define S_NEW(_PTR, _PTR2, _N)  ((_PTR) ? realloc(_PTR2, _N) : memset(malloc(_N), 0, _N))
#define S_DEL(_PTR, _PTR2)      (_PTR ? (free(_PTR2),_PTR=0) : _PTR)
#ifndef MEMMOVE
#define MEMMOVE(_F, _P, _N)     memmove(_F, _P, sizeof(*(_F))*(_N))
#endif

// vector
#define vec_size(v)          ((size_t*)(v))[-2]
#define vec_used(v)          ((size_t*)(v))[-1]
#define vec_begin(v)         (v)
#define vec_end(v)           (v+vec_used(v))
#define vec_new(v, n)        (ptr_cast(v, ((size_t*)S_NEW(v, &vec_size(v), sizeof(size_t)*2+n*sizeof(*v))+2)), vec_size(v)=(n), v)
#define vec_del(v)           S_DEL(v, &vec_size(v))
#define vec_resize(v, g)     (vec_new((v), (g)), (g)>vec_used(v) ? MEMSET(v+vec_used(v), 0, (g)-vec_used(v)):0)
#define vec_grow(v, g)       (v ? (vec_used(v)+=(g), v = (vec_size(v)<vec_used(v) ? vec_new(v, (vec_used(v)*2)): v)) : vec_new(v, g))
#define vec_push_back(v, x)  (vec_grow(v, 1)[vec_used(v)-1] = x)
#define vec_copy(v, v2, n2)  (((v)!=(v2)) ? (vec_new((v), n2), MEMMOVE(v, v2, (n2)), vec_used(v)=(n2)) : (0))
#define vec_insert(v, i, x)  (vec_grow(v, 1), MEMMOVE(v+i+1, v+i, (vec_used(v)-1-i)), v[i] = x)
#define vec_erase(v, i, n)   (vec_used(v)-=n, MEMMOVE(v+i, v+i+n, (vec_used(v)-i)))
#define vec_clear(v)         (vec_used(v)=0)
#define vec_pop(v)           (v[--vec_used(v)])
#define vec_foreach(v, _do)  {char* _h=(char*)v, *_vend=(char*)vec_end(v); for (; (char*)v!=_vend; ++v) {_do;} ptr_cast(v, _h); }
#define vec_printf(v, fmt)   vec_foreach(v, printf(fmt, *v))
#define vec_find(v, _x, _i)  vec_foreach(v, if (_x==*v) {_i=v; break;})
#define vec_sum(v, _sum)     vec_foreach(v, _sum+=*v)
#define vec_mean(v, _mean)   { vec_foreach(v, _mean+=*v); _mean/=vec_used(v); }
#define vec_find_if(v, _docmp, _i)  vec_foreach(v, if (_docmp) {_i=v; break;})

typedef struct PtrArray PtrArray;
typedef struct PtrArray_vtable {
  void (*f_Release)(PtrArray* sm);
  void (*f_RemoveAll)(PtrArray* sm);
  void (*f_Resize)(PtrArray* sm, int iSize );
  bool (*f_Add)(PtrArray* sm, LPVOID pData );
  bool (*f_InsertAt)(PtrArray* sm, int iIndex, LPVOID pData );
  bool (*f_InsertArrayAt)(PtrArray* sm, int iIndex, LPVOID pData, int nCount );
  bool (*f_SetAt)(PtrArray* sm, int iIndex, LPVOID pData );
  bool (*f_RemoveAt)(PtrArray* sm, int iIndex );
  int (*f_Find)(PtrArray* sm, LPVOID pData );
  LPVOID (*f_GetAt)(PtrArray* sm, int iIndex );
}PtrArray_vtable;
typedef struct PtrArray {
  PtrArray_vtable* vtable;
  LPVOID* m_ppVoid;
  int m_nCount;
  int m_nAllocated;
}PtrArray;

static void PtrArray_Release(PtrArray* sm) {
  if ( sm->m_ppVoid != NULL )
    free( sm->m_ppVoid );
  free(sm);
}
static void PtrArray_RemoveAll(PtrArray* sm) {
  if ( sm->m_ppVoid != NULL )
    free( sm->m_ppVoid );
  sm->m_ppVoid = NULL;
  sm->m_nCount = sm->m_nAllocated = 0;
}
static void PtrArray_Resize( PtrArray* sm, int iSize ) {
  //RemoveAll();
  sm->m_ppVoid = (LPVOID*)( malloc( iSize * sizeof( LPVOID ) ) );
  ZeroMemory( sm->m_ppVoid, iSize * sizeof( LPVOID ) );
  sm->m_nAllocated = iSize;
  sm->m_nCount = iSize;
}

//#define IsEmpty(sm) (sm->m_nCount == 0)

static bool PtrArray_Add( PtrArray* sm, LPVOID pData ) {
  if ( ++sm->m_nCount >= sm->m_nAllocated ) {
    sm->m_nAllocated *= 2;
    if ( sm->m_nAllocated == 0 )
      sm->m_nAllocated = 11;
    sm->m_ppVoid = (LPVOID)( realloc( sm->m_ppVoid, sm->m_nAllocated * sizeof( LPVOID ) ) );
    if ( sm->m_ppVoid == NULL )
      return false;
  }
  sm->m_ppVoid[ sm->m_nCount - 1 ] = pData;
  return true;
}
static bool PtrArray_InsertAt( PtrArray* sm, int iIndex, LPVOID pData ) {
  if ( iIndex == sm->m_nCount )
    return PtrArray_Add( sm, pData );
  if ( iIndex < 0 || iIndex > sm->m_nCount )
    return false;
  if ( ++sm->m_nCount >= sm->m_nAllocated ) {
    sm->m_nAllocated *= 2;
    if ( sm->m_nAllocated == 0 )
      sm->m_nAllocated = 11;
    sm->m_ppVoid = (LPVOID*)( realloc( sm->m_ppVoid, sm->m_nAllocated * sizeof( LPVOID ) ) );
    if ( sm->m_ppVoid == NULL )
      return false;
  }
  memmove( &sm->m_ppVoid[ iIndex + 1 ], &sm->m_ppVoid[ iIndex ], ( sm->m_nCount - iIndex - 1 ) * sizeof( LPVOID ) );
  sm->m_ppVoid[ iIndex ] = pData;
    return true;
}
static bool PtrArray_InsertArrayAt( PtrArray* sm, int iIndex, LPVOID pData, int nCount ) {
  if ( nCount < 0 )
    return false;
  if ( 0 == nCount )
    return true;
  if ( iIndex < 0 || iIndex > sm->m_nCount )
    return false;
  sm->m_nCount += nCount;
  if ( sm->m_nCount >= sm->m_nAllocated ) {
    while ( sm->m_nCount >= sm->m_nAllocated ) {
      sm->m_nAllocated *= 2;
    }
    if ( sm->m_nAllocated == 0 )
      sm->m_nAllocated = 11;
    sm->m_ppVoid = (LPVOID*)( realloc( sm->m_ppVoid, sm->m_nAllocated * sizeof( LPVOID ) ) );
    if ( sm->m_ppVoid == NULL )
      return false;
  }
  memmove( &sm->m_ppVoid[ iIndex + nCount ], &sm->m_ppVoid[ iIndex ], ( sm->m_nCount - iIndex - 1 ) * sizeof( LPVOID ) );
  memmove( &sm->m_ppVoid[ iIndex ], pData, nCount * sizeof( LPVOID ) );
  return true;
}
bool PtrArray_SetAt( PtrArray* sm, int iIndex, LPVOID pData ) {
  if ( iIndex < 0 || iIndex >= sm->m_nCount )
    return false;
  sm->m_ppVoid[ iIndex ] = pData;
  return true;
}
static bool PtrArray_RemoveAt( PtrArray* sm, int iIndex ) {
  if ( iIndex < 0 || iIndex >= sm->m_nCount )
    return false;
  if ( iIndex < --sm->m_nCount )
    CopyMemory( sm->m_ppVoid + iIndex, sm->m_ppVoid + iIndex + 1, ( sm->m_nCount - iIndex ) * sizeof( LPVOID ) );
  return true;
}
static int PtrArray_Find( PtrArray* sm, LPVOID pData ) {
  int i;
  for ( i = 0; i < sm->m_nCount; i++ )
    if ( sm->m_ppVoid[ i ] == pData )
      return i;
    return -1;
}
#define PtrArray_GetSize(sm) ((sm) ? (sm->m_nCount) : 0)

static LPVOID PtrArray_GetAt( PtrArray* sm, int iIndex ) {
  if ( iIndex < 0 || iIndex >= sm->m_nCount )
    return NULL;
  return sm->m_ppVoid[ iIndex ];
}
PtrArray* CreatePtrArray( int iPreallocSize ) {
  static PtrArray_vtable vtable = {
    PtrArray_Release,
      PtrArray_RemoveAll,
      PtrArray_Resize,
      PtrArray_Add,
      PtrArray_InsertAt,
      PtrArray_InsertArrayAt,
      PtrArray_SetAt,
      PtrArray_RemoveAt,
      PtrArray_Find,
      PtrArray_GetAt
  };
  PtrArray* sm = (PtrArray*)malloc(sizeof(PtrArray));
  sm->m_ppVoid=( NULL ), sm->m_nCount=( 0 ), sm->m_nAllocated=( iPreallocSize );
  ASSERT( iPreallocSize >= 0 );
  if ( iPreallocSize > 0 )
    sm->m_ppVoid = (LPVOID*)( malloc( iPreallocSize * sizeof( LPVOID ) ) );
  sm->vtable = &vtable;
  return sm;
}
