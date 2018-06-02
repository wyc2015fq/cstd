
typedef struct _CHTMLStringVector {
  char **m_pszStrings;
  int *m_pnStrings;
  int *m_pnIndexValues;
  int m_nStrings;
} CHTMLStringVector;

CHTMLStringVector* CreateCHTMLStringVector() {
  CHTMLStringVector* me = malloc(sizeof(CHTMLStringVector));
  me->m_nStrings=( 0 ), me->m_pszStrings=( NULL ), me->m_pnStrings=( NULL ), me->m_pnIndexValues=( NULL );
  return me;
}

void CHTMLStringVector_Clean(CHTMLStringVector* me) {
  int i;
  for ( i = 0;i < me->m_nStrings;i++ )
    free(me->m_pszStrings[ i ]);
  free( me->m_pszStrings );
  free( me->m_pnStrings );
  free( me->m_pnIndexValues );
  me->m_nStrings = 0;
  me->m_pszStrings = NULL;
  me->m_pnStrings = NULL;
  me->m_pnIndexValues = NULL;
}

void DeleteCHTMLStringVector(CHTMLStringVector* me) {
  CHTMLStringVector_Clean(me);
  free(me);
}
int MakeIndex( char *szString, int nLenght ) {
  int nIndex = 0, i;
  for ( i = 0;i < nLenght;i++ )
    nIndex = ( 255 * nIndex + szString[ i ] );
  return nIndex;
}
int CHTMLStringVector_AddString( CHTMLStringVector* me, char *szString, int nSize ) {
  int i = me->m_nStrings++;
  if ( !szString )
    return 0;
  me->m_pnStrings = ( int* ) realloc( me->m_pnStrings, sizeof( int ) * me->m_nStrings );
  me->m_pnIndexValues = ( int* ) realloc( me->m_pnIndexValues, sizeof( int ) * me->m_nStrings );
  if ( nSize == -1 )
    me->m_pnStrings[ i ] = strlen( szString );
  else
    me->m_pnStrings[ i ] = nSize;
  me->m_pszStrings = ( char** ) realloc( me->m_pszStrings, sizeof( char* ) * me->m_nStrings );
  me->m_pszStrings[ i ] = (char*)malloc( me->m_pnStrings[ i ] + 1 );
  if ( me->m_pnStrings[ i ] )
    memcpy( me->m_pszStrings[ i ], szString, me->m_pnStrings[ i ] );
  me->m_pszStrings[ i ][ me->m_pnStrings[ i ] ] = 0;
  if ( me->m_pnStrings[ i ] )
    me->m_pnIndexValues[ i ] = MakeIndex( szString, me->m_pnStrings[ i ] );
  else
    me->m_pnIndexValues[ i ] = 0;
  return 1;
}
int CHTMLStringVector_SetString( CHTMLStringVector* me, int nIndex, char *szString, int nSize ) {
  char *szTmp;
  if ( nIndex < 0 || nIndex >= me->m_nStrings )
    return 0;
  szTmp = me->m_pszStrings[ nIndex ];
  if ( nSize == -1 )
    me->m_pnStrings[ nIndex ] = strlen( szString );
  else
    me->m_pnStrings[ nIndex ] = nSize;
  me->m_pszStrings[ nIndex ] = (char*)malloc( me->m_pnStrings[ nIndex ] + 1 );
  memcpy( me->m_pszStrings[ nIndex ], szString, me->m_pnStrings[ nIndex ] + 1 );
  me->m_pnIndexValues[ nIndex ] = MakeIndex( szString, me->m_pnStrings[ nIndex ] );
  free(szTmp);
  return 1;
}
char *CHTMLStringVector_GetString( CHTMLStringVector* me, int nIndex, int *pnSize ) {
  if ( nIndex < 0 || nIndex >= me->m_nStrings ) {
    if ( pnSize )
      * pnSize = 0;
    return "";
  }
  if ( pnSize )
    * pnSize = me->m_pnStrings[ nIndex ];
  return me->m_pszStrings[ nIndex ] ? me->m_pszStrings[ nIndex ] : "";
}
int CHTMLStringVector_GetIndex( CHTMLStringVector* me, int nIndex ) {
  if ( nIndex < 0 || nIndex >= me->m_nStrings )
    return -1;
  return me->m_pnIndexValues[ nIndex ];
}
int CHTMLStringVector_GetCount(CHTMLStringVector* me) {
  return me->m_nStrings;
}
int CHTMLStringVector_SetCount( CHTMLStringVector* me, int nCount ) {
  int i;
  if ( nCount < me->m_nStrings ) {
    for ( i = nCount;i < me->m_nStrings;i++ )
      free(me->m_pszStrings[ i ]);
  } else {
    me->m_pszStrings = ( char** ) realloc( me->m_pszStrings, sizeof( char* ) * nCount );
    me->m_pnStrings = ( int* ) realloc( me->m_pnStrings, sizeof( int ) * nCount );
    me->m_pnIndexValues = ( int* ) realloc( me->m_pnIndexValues, sizeof( int ) * nCount );
    for ( i = me->m_nStrings;i < nCount;i++ ) {
      me->m_pszStrings[ i ] = (char*)malloc( 1 );
      *me->m_pszStrings[ i ] = 0;
      me->m_pnStrings[ i ] = 0;
      me->m_pnIndexValues[ i ] = -1;
    }
  }
  return ( me->m_nStrings = nCount );
}
CHTMLStringVector* CHTMLStringVector_Set( CHTMLStringVector* me, CHTMLStringVector* s ) {
  int i, n;
  CHTMLStringVector_Clean(me);
  for ( i = 0;i < CHTMLStringVector_GetCount(s);i++ ) {
    char *sz = CHTMLStringVector_GetString( s, i, &n );
    CHTMLStringVector_AddString( me, sz, n );
  }
  return me;
}
void CHTMLStringVector_DeleteString( CHTMLStringVector* me, int nIndex ) {
  int i;
  if ( nIndex < 0 || nIndex >= me->m_nStrings )
    return ;
  free(me->m_pszStrings[ nIndex ]);
  me->m_nStrings--;
  for ( i = nIndex;i < me->m_nStrings;i++ ) {
    me->m_pszStrings[ i ] = me->m_pszStrings[ i + 1 ];
    me->m_pnStrings[ i ] = me->m_pnStrings[ i + 1 ];
    me->m_pnIndexValues[ i ] = me->m_pnIndexValues[ i + 1 ];
  }
}
int CHTMLStringVector_MoveString( CHTMLStringVector* me, int nFrom, int nTo ) {
  char *szTmpStr;
  int nTmp;
  if ( nFrom < 0 || nFrom >= me->m_nStrings )
    return 0;
  if ( nTo < 0 || nTo >= me->m_nStrings )
    return 0;
  szTmpStr = me->m_pszStrings[ nFrom ];
  me->m_pszStrings[ nFrom ] = me->m_pszStrings[ nTo ];
  me->m_pszStrings[ nTo ] = szTmpStr;
  nTmp = me->m_pnStrings[ nFrom ];
  me->m_pnStrings[ nFrom ] = me->m_pnStrings[ nTo ];
  me->m_pnStrings[ nTo ] = nTmp;
  nTmp = me->m_pnIndexValues[ nFrom ];
  me->m_pnIndexValues[ nFrom ] = me->m_pnIndexValues[ nTo ];
  me->m_pnIndexValues[ nTo ] = nTmp;
  return 1;
}