
#define CHTMLStringTable_GetCount(me)  (me)->m_nVektors

typedef struct _CHTMLStringTable {
  CHTMLStringVector **m_pVektors;
  CHTMLStringVector* m_err;
  int m_nVektors;
  int m_nRowSize;
} CHTMLStringTable;

CHTMLStringTable* CreateCHTMLStringTable() {
  CHTMLStringTable* me = malloc(sizeof(CHTMLStringTable));
  me->m_nVektors=( 0 ), me->m_pVektors=( NULL ), me->m_nRowSize=( 0 );
  return me;
}

void CHTMLStringTable_Clean(CHTMLStringTable* me) {
  int i;
  for ( i = 0;i < me->m_nVektors;i++ )
    DeleteCHTMLStringVector(me->m_pVektors[ i ]);
  free( me->m_pVektors );
  DeleteCHTMLStringVector(me->m_err);
}
void DeleteCHTMLStringTable(CHTMLStringTable* me) {
  CHTMLStringTable_Clean(me);
  free(me);
}
int CHTMLStringTable_AddRow0(CHTMLStringTable* me ) {
  int i = me->m_nVektors++;
  me->m_pVektors = ( CHTMLStringVector** ) realloc( me->m_pVektors, sizeof( CHTMLStringVector* ) * me->m_nVektors );
  me->m_pVektors[ i ] = CreateCHTMLStringVector();
  return me->m_nVektors;
}
int CHTMLStringTable_AddRow( CHTMLStringTable* me, char **pszRow, int nRow ) {
  int j, i;
  if ( nRow > me->m_nRowSize )
    me->m_nRowSize = nRow;
  i = CHTMLStringTable_AddRow0(me) - 1;
  for ( j = 0;j < nRow;j++ )
    CHTMLStringVector_AddString( me->m_pVektors[ i ], pszRow[ j ], -1 );
  return me->m_nVektors;
}
int CHTMLStringTable_InsertRow( CHTMLStringTable* me, int nRow ) {
  int i;
  if ( nRow < 0 || nRow > me->m_nVektors )
    return 0;
  if ( nRow == me->m_nVektors )
    CHTMLStringTable_AddRow0(me);
  me->m_nVektors++;
  me->m_pVektors = ( CHTMLStringVector** ) realloc( me->m_pVektors, sizeof( CHTMLStringVector* ) * me->m_nVektors );
  for ( i = me->m_nVektors - 1;i > nRow;i-- )
    me->m_pVektors[ i ] = me->m_pVektors[ i - 1 ];
  me->m_pVektors[ nRow ] = CreateCHTMLStringVector();
  return 1;
}
int CHTMLStringTable_AddString( CHTMLStringTable* me, int nRow, char *szString, int nString ) {
  if ( nRow < 0 || nRow >= me->m_nVektors )
    return 0;
  CHTMLStringVector_AddString( me->m_pVektors[ nRow ], szString, nString );
  if ( nRow > CHTMLStringVector_GetCount(me->m_pVektors[ nRow ]) )
    me->m_nRowSize = CHTMLStringVector_GetCount(me->m_pVektors[ nRow ]);
  return 1;
}
int CHTMLStringTable_DeleteRow( CHTMLStringTable* me, int nRow ) {
  CHTMLStringVector *p;
  if ( nRow < 0 || nRow > me->m_nVektors )
    return 0;
  p = me->m_pVektors[ nRow ];
  me->m_nVektors--;
  for ( ;nRow < me->m_nVektors;nRow++ )
    me->m_pVektors[ nRow ] = me->m_pVektors[ nRow + 1 ];
  DeleteCHTMLStringVector(p);
  return 1;
}
int CHTMLStringTable_SetString( CHTMLStringTable* me, int nRow, int nIndex, char *szString, int nString ) {
  if ( nRow < 0 || nRow >= me->m_nVektors )
    return 0;
  return  CHTMLStringVector_SetString( me->m_pVektors[ nRow ], nIndex, szString, nString );
}
char *CHTMLStringTable_GetString( CHTMLStringTable* me, int nRow, int nIndex, int *pnSize ) {
  if ( nRow < 0 || nRow >= me->m_nVektors )
    return "";
  return CHTMLStringVector_GetString( me->m_pVektors[ nRow ], nIndex, pnSize );
}

CHTMLStringVector* CHTMLStringTable_GetAt( CHTMLStringTable* me, int nRow ) {
  if ( nRow<0 || nRow >= me->m_nVektors )
    return me->m_err;
  return me->m_pVektors[ nRow ];
}
CHTMLStringTable* CHTMLStringTable_Set( CHTMLStringTable* me, CHTMLStringTable* st ) {
  int i, j, n;
  while ( CHTMLStringTable_GetCount(me) )
    CHTMLStringTable_DeleteRow( me, 0 );
  for ( i = 0;i < CHTMLStringTable_GetCount(st);i++ ) {
    CHTMLStringVector* sv = CHTMLStringTable_GetAt(st, i);
    CHTMLStringTable_AddRow0(me);
    for ( j = 0;j < CHTMLStringVector_GetCount(sv);j++ ) {
      char *sz = CHTMLStringTable_GetString( st, i, j, &n );
      CHTMLStringTable_AddString( me, i, sz, n );
    }
  }
  return me;
}
int CHTMLStringTable_MoveRow( CHTMLStringTable* me, int nFrom, int nTo ) {
  CHTMLStringVector *pTmp;
  if ( nFrom < 0 || nFrom >= me->m_nVektors || nTo < 0 || nTo >= me->m_nVektors )
    return 0;
  pTmp = me->m_pVektors[ nFrom ];
  me->m_pVektors[ nFrom ] = me->m_pVektors[ nTo ];
  me->m_pVektors[ nTo ] = pTmp;
  return 1;
}
void CHTMLStringTable_Reset(CHTMLStringTable* me) {
  while ( CHTMLStringTable_GetCount(me) )
    CHTMLStringTable_DeleteRow( me, CHTMLStringTable_GetCount(me) - 1 );
}
int CHTMLStringTable_SplitString( CHTMLStringTable* me, char *szData, char *szFieldSep, char *szRowSep ) {
  int q, p;
  int nFieldSep = 0;
  int nRowSep = 0;
  CHTMLStringTable_AddRow0(me);
  if ( szFieldSep )
    nFieldSep = strlen( szFieldSep );
  if ( szRowSep )
    nRowSep = strlen( szRowSep );
  for ( p = q = 0;szData[ q ];q++ ) {
    if ( !szData[ q ] || ( nFieldSep && !strncmp( &szData[ q ], szFieldSep, nFieldSep ) ) ||
         ( nRowSep && !strncmp( &szData[ q ], szRowSep, nRowSep ) ) ) {
      CHTMLStringTable_AddString( me, CHTMLStringTable_GetCount(me) - 1, &szData[ p ], q - p );
      if ( nRowSep && !strncmp( &szData[ q ], szRowSep, nRowSep ) ) {
        q += nRowSep - 1;
        CHTMLStringTable_AddRow0(me);
      } else
        q += nFieldSep - 1;
      p = q + 1;
    }
  }
  if ( szData[ p ] )
    CHTMLStringTable_AddString( me, CHTMLStringTable_GetCount(me) - 1, &szData[ p ], -1 );
  else if ( CHTMLStringVector_GetCount(me->m_pVektors[ CHTMLStringTable_GetCount(me) - 1 ]) == 0 )
    CHTMLStringTable_DeleteRow( me, CHTMLStringTable_GetCount(me) - 1 );
  return 1;
}
