
#ifndef setstr
#define setstr(_d, _s)   (free_s(_d), _d=strdup(_s))
#endif
typedef struct TITEM {
  char* Key;
  LPVOID Data;
  struct TITEM* pNext;
}TITEM;
typedef struct StringPtrMap StringPtrMap; 
typedef struct StringPtrMap_vtable {
  void (*f_Release)(StringPtrMap* sm);
  void (*f_Resize)( StringPtrMap* sm, int nSize );
  LPVOID (*f_Find)( StringPtrMap* sm, LPCTSTR key );
  bool (*f_Insert)( StringPtrMap* sm, LPCTSTR key, LPVOID pData );
  LPVOID (*f_Set)( StringPtrMap* sm, LPCTSTR key, LPVOID pData );
  bool (*f_Remove)( StringPtrMap* sm, LPCTSTR key );
  int (*f_GetSize)(StringPtrMap* sm);
  LPCTSTR (*f_GetAt)( StringPtrMap* sm, int iIndex );
}StringPtrMap_vtable;
typedef struct StringPtrMap {
  StringPtrMap_vtable* vtable;
  TITEM** m_aT;
  int m_nBuckets;
}StringPtrMap;
static UINT HashKey( LPCTSTR Key ) {
  UINT i = 0;
  SIZE_T len = _tcslen( Key );
  while ( len-- > 0 )
    i = ( i << 5 ) + i + Key[ len ];
  return i;
}
void StringPtrMap_Release(StringPtrMap* sm) {
  int len = sm->m_nBuckets;
  while ( len-- ) {
    TITEM * pItem = sm->m_aT[ len ];
    while ( pItem ) {
      TITEM * pKill = pItem;
      pItem = pItem->pNext;
      free_s( pKill->Key );
      free( pKill);
    }
  }
  free(sm->m_aT);
  free(sm);
}
void StringPtrMap_Resize( StringPtrMap* sm, int nSize ) {
  if (nSize==0) nSize=83;
  if ( sm->m_aT ) {
    int len = sm->m_nBuckets;
    while ( len-- ) {
      TITEM * pItem = sm->m_aT[ len ];
      while ( pItem ) {
        TITEM * pKill = pItem;
        pItem = pItem->pNext;
        free_s( pKill->Key );
        free(pKill);
      }
    }
    free(sm->m_aT);
    sm->m_aT = NULL;
  }
  if ( nSize < 0 )
    nSize = 0;
  if ( nSize > 0 ) {
    sm->m_aT = (TITEM **)malloc(sizeof(TITEM* )* nSize );
    memset( sm->m_aT, 0, nSize * sizeof( TITEM* ) );
  }
  sm->m_nBuckets = nSize;
}
LPVOID StringPtrMap_Find( StringPtrMap* sm, LPCTSTR key ) {
  UINT slot;
  const TITEM * pItem;
  if ( sm->m_nBuckets == 0 )
    return NULL;
  slot = HashKey( key ) % sm->m_nBuckets;
  for ( pItem = sm->m_aT[ slot ]; pItem; pItem = pItem->pNext ) {
    if ( 0==strcmp(pItem->Key, key) ) {
      return pItem->Data;
    }
  }
  return NULL;
}
bool StringPtrMap_Insert( StringPtrMap* sm, LPCTSTR key, LPVOID pData ) {
  UINT slot;
  TITEM* pItem;
  if ( sm->m_nBuckets == 0 )
    return false;
  if ( StringPtrMap_Find( sm, key ) )
    return false;
  // Add first in bucket
  slot = HashKey( key ) % sm->m_nBuckets;
  pItem = (TITEM*)malloc(sizeof(TITEM));
  memset(pItem, 0, sizeof(TITEM));
  setstr(pItem->Key, key);
  pItem->Data = pData;
  pItem->pNext = sm->m_aT[ slot ];
  sm->m_aT[ slot ] = pItem;
  return true;
}
LPVOID StringPtrMap_Set( StringPtrMap* sm, LPCTSTR key, LPVOID pData ) {
  UINT slot;
  TITEM* pItem;
  if ( sm->m_nBuckets == 0 )
    return pData;
  slot = HashKey( key ) % sm->m_nBuckets;
  // Modify existing item
  for ( pItem = sm->m_aT[ slot ]; pItem; pItem = pItem->pNext ) {
    if ( pItem->Key == key ) {
      LPVOID pOldData = pItem->Data;
      pItem->Data = pData;
      return pOldData;
    }
  }
  StringPtrMap_Insert( sm, key, pData );
  return NULL;
}
bool StringPtrMap_Remove( StringPtrMap* sm, LPCTSTR key ) {
  UINT slot;
  TITEM** ppItem;
  if ( sm->m_nBuckets == 0 )
    return false;
  slot = HashKey( key ) % sm->m_nBuckets;
  ppItem = &sm->m_aT[ slot ];
  while ( *ppItem ) {
    if ( ( *ppItem ) ->Key == key ) {
      TITEM * pKill = *ppItem;
      *ppItem = ( *ppItem ) ->pNext;
      free(pKill);
      return true;
    }
    ppItem = &( ( *ppItem ) ->pNext );
  }
  return false;
}
int StringPtrMap_GetSize(StringPtrMap* sm) {
  int nCount = 0;
  const TITEM * pItem;
  int len = sm->m_nBuckets;
  while ( len-- ) {
    for ( pItem = sm->m_aT[ len ]; pItem; pItem = pItem->pNext )
      nCount++;
  }
  return nCount;
}
LPCTSTR StringPtrMap_GetAt( StringPtrMap* sm, int iIndex ) {
  int pos = 0;
  int len = sm->m_nBuckets;
  TITEM * pItem;
  while ( len-- ) {
    for ( pItem = sm->m_aT[ len ]; pItem; pItem = pItem->pNext ) {
      if ( pos++ == iIndex ) {
        return pItem->Key;
      }
    }
  }
  return NULL;
}

StringPtrMap* CreateStringPtrMap( int nSize ) {
  static StringPtrMap_vtable vtable = {
    StringPtrMap_Release,
      StringPtrMap_Resize,
      StringPtrMap_Find,
      StringPtrMap_Insert,
      StringPtrMap_Set,
      StringPtrMap_Remove,
      StringPtrMap_GetSize,
      StringPtrMap_GetAt
  };
  StringPtrMap* sm = (StringPtrMap*)malloc(sizeof(StringPtrMap));
  if (nSize==0) nSize=83;
  if ( nSize < 16 )
    nSize = 16;
  sm->m_nBuckets = nSize;
  sm->m_aT = (TITEM**)malloc(sizeof(TITEM*) * nSize );
  memset( sm->m_aT, 0, nSize * sizeof( TITEM* ) );
  sm->vtable = &vtable;
  return sm;
}
