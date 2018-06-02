
#include <math.h>
#include <stdio.h>

CHTMLViewer* CreateCHTMLViewer( CHTMLProgress *pProgress, BOOL bWaitForImages ) {
  CHTMLViewer* me = (CHTMLViewer*)malloc(sizeof(CHTMLViewer));
  me->m_Images = CreateCHTMLBitmaps(pProgress);
  me->m_bWaitForImages = bWaitForImages;
  me->m_pProgress = pProgress;
  me->m_ppItems = NULL;
  me->m_nItems = 0;
  me->m_bgCol = -1;
  me->m_nBmpBkg = -1;
  me->m_nItemCount = 0;
  me->m_nLevelCount = 0;
  me->m_nActiveLink = -1;
  me->m_nListLevel = 0;
  me->m_nCurrForm = -1;
  me->m_bReading = FALSE;
  //CHTMLBitmaps_SetViewer( me->m_Images, this );
  return me;
}
void CHTMLViewer_Clear(CHTMLViewer* me) {
  int i;
  CHTMLBitmaps_Stop(me->m_Images);
  for ( i = 0;i < me->m_nItems;i++ )
    DeleteCHTMLItem(me->m_ppItems[ i ]);
  free( me->m_ppItems );
}
void DeleteCHTMLViewer(CHTMLViewer* me) {
  CHTMLViewer_Clear(me);free(me);
}
void CHTMLViewer_AddBaseToUrl( CHTMLViewer* me, char *szUrl, char *szFullUrl, char *szBaseUrl ) {
  int nPPos;
  int i;
  for ( nPPos = 0;szUrl[ nPPos ] && szUrl[ nPPos ] != '/';nPPos++ )
    if ( !strncmp( &szUrl[ nPPos ], "://", 3 ) )
      break;
  if ( !szUrl[ nPPos ] || szUrl[ nPPos ] == '/' )
    nPPos = -1;
  if ( nPPos >= 0 )
    strcpy( szFullUrl, szUrl );
  else {
    strcpy( szFullUrl, szBaseUrl );
    if ( strrchr( szFullUrl, '?' ) )
      * strrchr( szFullUrl, '?' ) = 0;
    for ( nPPos = 0;szFullUrl[ nPPos ];nPPos++ )
      if ( !strncmp( &szFullUrl[ nPPos ], "://", 3 ) )
        break;
    if ( !szFullUrl[ nPPos ] )
      nPPos = 0;
    for ( i = 0;szFullUrl[ i ];i++ )
      if ( szFullUrl[ i ] == '\\' )
        szFullUrl[ i ] = '/';
    if ( !strchr( &szFullUrl[ nPPos + 3 ], '/' ) )
      strcat( szFullUrl, "/" );
    if ( strrchr( szFullUrl, '/' ) && strncmp( strrchr( szFullUrl, '/' ) - 3, "://", 3 ) )
      * ( strrchr( szFullUrl, '/' ) + 1 ) = 0;
    else if ( szFullUrl[ strlen( szFullUrl ) - 1 ] != '/' )
      strcat( szFullUrl, "/" );
    while ( !strncmp( szUrl, "../", 3 ) ) {
      if ( szFullUrl[ strlen( szFullUrl ) - 1 ] == '/' ) {
        szFullUrl[ strlen( szFullUrl ) - 1 ] = 0;
      }
      if ( strrchr( szFullUrl, '/' ) && ( int ) strrchr( szFullUrl, '/' ) - ( int ) szFullUrl > 8 )
        strcpy( strrchr( szFullUrl, '/' ) + 1, "\0" );
      szUrl += 3;
    }
    if ( *szUrl == '/' && strchr( &szFullUrl[ nPPos + 4 ], '/' ) )
      * strchr( &szFullUrl[ nPPos + 3 ], '/' ) = 0;
    if ( *szUrl != '/' && szFullUrl[ strlen( szFullUrl ) - 1 ] != '/' )
      strcat( szFullUrl, "/" );
    for ( i = strlen( szFullUrl );i < 1023 && *szUrl;i++ )
      szFullUrl[ i ] = *( szUrl++ );
    szFullUrl[ i ] = 0;
    if ( !strncmp( szFullUrl, "file://", nPPos + 3 ) )
      for ( i = nPPos + 3;szFullUrl[ i ];i++ )
        if ( szFullUrl[ i ] == '/' )
          szFullUrl[ i ] = '\\';
  }
}
CHTMLStringTable* CHTMLViewer_GetProperties( CHTMLViewer* me, char *szData) {
  int i, j, k, e;
  CHTMLStringTable* st = CreateCHTMLStringTable();
  for ( e = 0;szData[ e ] && szData[ e ] != '>';e++ )
    ;
  for ( i = 0;i < e && szData[ i ] != ' ';i++ )
    ;
  while ( i < e ) {
    while ( szData[ i ] == ' ' && i < e )
      i++;
    j = i;
    for ( ;i < e && szData[ i ] != '=' && szData[ i ] != ' ';i++ )
      ;
    CHTMLStringTable_AddRow0(st);
    CHTMLStringTable_AddString( st, CHTMLStringTable_GetCount(st) - 1, &szData[ j ], i - j );
    if ( szData[ i ] == '=' ) {
      i++;
      while ( szData[ i ] == ' ' && i < e )
        i++;
      if ( szData[ i ] == '\'' || szData[ i ] == '"' ) {
        k = szData[ i ];
        i++;
      } else
        k = 0;
      j = i;
      for ( ;i < e && ( ( k && szData[ i ] != k ) || ( !k && szData[ i ] != ' ' ) );i++ )
        ;
      CHTMLStringTable_AddString( st, CHTMLStringTable_GetCount(st) - 1, &szData[ j ], i - j );
      if ( k && i < e )
        i++;
    }
  }
  return st;
}
void CHTMLViewer_AddItem( CHTMLViewer* me, CHTMLItem *** pppItems, int* pItems ) {
  (*pItems)++;
  me->m_nItemCount++;
  *pppItems = ( CHTMLItem** ) realloc( *pppItems, sizeof( CHTMLItem* ) * (*pItems) );
  if ( !*pppItems )
    Beep( 1000, 10 );
}
COLORREF CHTMLViewer_GetCol( CHTMLViewer* me, char *szData ) {
  int i;
  int r, g, b;
  char *s;
  char szTmp[ 3 ] = {0};
  for ( i = 0;g_ColTable[ i ].szName;i++ )
    if ( !stricmp( szData, g_ColTable[ i ].szName ) )
      return g_ColTable[ i ].colVal;
  s = szData;
  if ( *s == '#' )
    s++;
  szTmp[ 0 ] = s[ 0 ];
  szTmp[ 1 ] = s[ 1 ];
  sscanf( szTmp, "%x", &r );
  szTmp[ 0 ] = s[ 2 ];
  szTmp[ 1 ] = s[ 3 ];
  sscanf( szTmp, "%x", &g );
  szTmp[ 0 ] = s[ 4 ];
  szTmp[ 1 ] = s[ 5 ];
  sscanf( szTmp, "%x", &b );
  return RGB( r, g, b );
}
void CHTMLViewer_AddHTML( CHTMLViewer* me, char *szHTMLData, CHTMLItem *** pppItems, int* pItems, char *szFont, int nFontSize,
                          COLORREF colText, char *szBaseUrl, int nLevel, int nHFormat, BOOL bNoBreak ) {
  BOOL bTextStarted = 0;
  int nCurrLink = -1;
  int nCurrFontSize;
  int i = 0, t, nLen;
  BOOL bBold = 0, bItalic = 0, bUnderline = 0, bLink = 0, bSmall = 0, bBig = 0, bCenter = 0, bSubText = 0, bSupText = 0, bStrike = 0;
  COLORREF colCurText;
  char szCurrFont[ 100 ];
  if ( nLevel > me->m_nLevelCount )
    me->m_nLevelCount = nLevel;
  nCurrFontSize = nFontSize;
  colCurText = colText;
  strcpy( szCurrFont, szFont );
  if ( !nLevel )
    nLen = strlen( szHTMLData );
  while ( szHTMLData[ i ] == ' ' )
    i++;
  for ( i = 0;szHTMLData[ i ]; i++ ) {
    if ( !nLevel && me->m_pProgress )
      me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
    if ( szHTMLData[ i ] == '<' ) {
      if ( ( !strnicmp( &szHTMLData[ i ], "<div", 4 ) && szHTMLData[ i + 4 ] == ' ' ) ||
        ( !strnicmp( &szHTMLData[ i ], "<p", 2 ) && szHTMLData[ i + 2 ] == ' ' ) ) {
        BOOL bP = FALSE;
        BOOL bTableBorder = 0;
        int bSub = 0;
        int e, a, nTableHFormat, nRestorePos = i;
        COLORREF colTableBkg = -1;
        CHTMLStringTable* test;
        if ( !strnicmp( &szHTMLData[ i ], "<p", 2 ) )
          bP = TRUE;
        test = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
        nTableHFormat = nHFormat;
        if ( bCenter )
          nTableHFormat = 1;
        for ( a = 0; a < CHTMLStringTable_GetCount(test);a++ ) {
          char* str0 = CHTMLStringTable_GetString(test, a, 0, 0);
          char* str1 = CHTMLStringTable_GetString(test, a, 1, 0);
          if ( !stricmp( str0, "bgcolor" ) )
            colTableBkg = CHTMLViewer_GetCol( me, str1 );
          else if ( !stricmp( str0, "border" ) )
            bTableBorder = atoi( str1 );
          else if ( !stricmp( str0, "align" ) ) {
            if ( !stricmp( str1, "left" ) )
              nTableHFormat = 0;
            else if ( !stricmp( str1, "right" ) )
              nTableHFormat = 1;
            else if ( !stricmp( str1, "center" ) || !stricmp( str1, "middle" ) )
              nTableHFormat = 2;
            else if ( !stricmp( str1, "justify" ) )
              nTableHFormat = 3;
          }
        }
        DeleteCHTMLStringTable(test); test=0;
        for ( ;szHTMLData[ i ] && szHTMLData[ i ] != '>';i++ )
          ;
        if ( szHTMLData[ i ] == '>' )
          i++;
        for ( e = i;szHTMLData[ e ];e++ ) {
          if ( !strnicmp( &szHTMLData[ e ], "</table", 7 ) || ( !bP && !strnicmp( &szHTMLData[ e ], "</div", 5 ) ) ) {
            if ( bSub )
              bSub--;
            else
              break;
          } else if ( !strnicmp( &szHTMLData[ e ], "<table", 6 ) || ( !bP && !strnicmp( &szHTMLData[ e ], "<div", 4 ) ) ) {
            bSub++;
          } else if ( bP && !bSub && ( !strnicmp( &szHTMLData[ e ], "</p", 3 ) || !strnicmp( &szHTMLData[ e ], "<p", 2 ) || !strnicmp( &szHTMLData[ e ], "<hr", 3 ) ) )
            break;
        }
        bSub = 0;
        if ( !szHTMLData[ e ] )
          for ( e = i;szHTMLData[ e ];e++ ) {
            if ( !strnicmp( &szHTMLData[ e ], "</table", 7 ) ) {
              if ( bSub )
                bSub--;
            } else if ( !strnicmp( &szHTMLData[ e ], "<table", 6 ) ) {
              bSub++;
            }
            if ( !bSub )
              if ( ( !bP && !strnicmp( &szHTMLData[ e ], "<div", 4 ) ) || ( bP && !strnicmp( &szHTMLData[ e ], "<p", 2 ) ) ) {
                CHTMLStringTable* st;
                st = CHTMLViewer_GetProperties( me, &szHTMLData[ e ] );
                for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
                  char* str0 = CHTMLStringTable_GetString(test, a, 0, 0);
                  if ( !stricmp( str0, "align" ) )
                    break;
                }
                if ( a < CHTMLStringTable_GetCount(st) )
                  break;
                bSub++;
                DeleteCHTMLStringTable(st);
              }
          }
        if ( nTableHFormat != nHFormat || ( bCenter && nTableHFormat != 1 ) ) {
          int Restore = szHTMLData[ e ];
          szHTMLData[ e ] = 0;
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, nTableHFormat, 0 );
          if ( bP ) {
            /*
            CHTMLViewer_AddItem( me,ppItems, (*pItems));
            (*pppItems)[(*pItems)-1] = CHTMLItem_NewText(" ",15,2,-1,"Courier New",15,
                   0,0,0,RGB(255,255,255),nTableHFormat,0,-1,bNoBreak);
            */
            CHTMLViewer_AddItem( me,  pppItems,   pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, nTableHFormat, 0 );
          }
          CHTMLViewer_AddHTML( me, &szHTMLData[ i ], pppItems, pItems, szCurrFont, nCurrFontSize, colCurText, szBaseUrl, nLevel + 1, nTableHFormat, bNoBreak );
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
          szHTMLData[ e ] = Restore;
          if ( szHTMLData[ e ] && !( szHTMLData[ e ] == '<' && szHTMLData[ e + 1 ] != '/' ) ) {
            e++;
            for ( i = e;szHTMLData[ i ] && szHTMLData[ i ] != '>';i++ )
              ;
            if ( !szHTMLData[ i ] )
              i--;
          } else
            i = e - 1;
        } else
          i = nRestorePos;
      } else if ( !strnicmp( &szHTMLData[ i ], "<pre", 4 ) ) {
          int e;
        int Restore;
        int bSub = 0;
        for ( ;szHTMLData[ i ] && szHTMLData[ i ] != '>';i++ )
          ;
        if ( szHTMLData[ i ] == '>' )
          i++;
        for ( e = i;szHTMLData[ e ];e++ ) {
          if ( !strnicmp( &szHTMLData[ e ], "</pre", 5 ) ) {
            if ( bSub )
              bSub--;
            else
              break;
          } else if ( !strnicmp( &szHTMLData[ e ], "<pre", 4 ) )
            bSub++;
        }
        Restore = szHTMLData[ e ];
        szHTMLData[ e ] = 0;
        CHTMLViewer_AddItem( me, pppItems, pItems );
        (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
        CHTMLViewer_AddItem( me, pppItems, pItems );
        (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewCompound( 0, 0, 0, 0, 0, -1, 0, 1, 1, 0, 0, 0, -1, 0, 0, 0, -1 );
        CHTMLViewer_AddHTML( me, &szHTMLData[ i ], &((*pppItems)[ (*pItems) - 1 ] ->m_ppSubItems), &((*pppItems)[ (*pItems) - 1 ] ->m_nSubItems), "Courier New", nFontSize, colText, szBaseUrl, nLevel + 1, 0, TRUE );
        CHTMLViewer_AddItem( me, pppItems, pItems );
        (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
        szHTMLData[ e ] = Restore;
        if ( szHTMLData[ e ] )
          e++;
        for ( i = e;szHTMLData[ i ] && szHTMLData[ i ] != '>';i++ )
          ;
        while ( szHTMLData[ i ] && szHTMLData[ i + 1 ] == ' ' )
          i++;
        if ( !szHTMLData[ i ] )
          i--;
      }
      if ( !strnicmp( &szHTMLData[ i ], "<table", 6 ) ) {
        COLORREF colTableBkg = -1;
        COLORREF colTableBorder = -1;
        BOOL bTableBorder = 0;
        BOOL bPrevCenter = bCenter;
        int a;
        int e;
        int nTables = 0;
        int nTableWidth = 0;
        int nTableHeight = 0;
        int nCellPadding = 4;
        int nCellSpacing = 4;
        int nTableBmp = -1;
        int nPrevHFormat = nHFormat;
        int nAbsoluteAlign = -1;
        int nRow = 0, nCol = 0, nWidth = 0, bgCol = -1, nColSpan = 1, nRowSpan = 1, bgTrCol = -1, TrHFormat = 0;
        CHTMLStringTable* test = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
        for ( a = 0;a < CHTMLStringTable_GetCount(test);a++ ) {
          char* str0 = CHTMLStringTable_GetString(test, a, 0, 0);
          char* str1 = CHTMLStringTable_GetString(test, a, 1, 0);
          if ( !stricmp( str0, "bgcolor" ) )
            colTableBkg = CHTMLViewer_GetCol( me, str1 );
          else if ( !stricmp( str0, "bordercolor" ) )
            colTableBorder = CHTMLViewer_GetCol( me, str1 );
          else if ( !stricmp( str0, "width" ) ) {
            if ( !strstr( str1, "%" ) )
              nTableWidth = atoi( str1 );
            else
              nTableWidth = -atoi( str1 );
          } else if ( !stricmp( str0, "height" ) && !strstr( str1, "%" ) ) {
            nTableHeight = atoi( str1 );
          } else if ( !stricmp( str0, "border" ) )
            bTableBorder = atoi( str1 );
          else if ( !stricmp( str0, "cellpadding" ) )
            nCellPadding = atoi( str1 );
          else if ( !stricmp( str0, "cellspacing" ) )
            nCellSpacing = atoi( str1 );
          else if ( !stricmp( str0, "background" ) && *str1 ) {
            char szFullUrl[ 2048 ];
            CHTMLItem *pTmp;
            CHTMLViewer_AddBaseToUrl( me, str1, szFullUrl, szBaseUrl );
            pTmp = CHTMLItem_NewImage( szFullUrl, 0, 0, 0, 0, me->m_Images, 0, bNoBreak, TRUE, 0 );
            nTableBmp = pTmp->m_nBmp;
            free(pTmp);
          } else if ( !stricmp( str0, "align" ) && ( !stricmp( str1, "center" ) || !stricmp( str1, "middle" ) ) ) {
            bCenter = 1;
            nHFormat = 2;
          } else if ( !stricmp( str0, "align" ) && !stricmp( str1, "right" ) ) {
            nHFormat = 1;
            nAbsoluteAlign = 1;
          } else if ( !stricmp( str0, "align" ) && !stricmp( str1, "left" ) ) {
            nHFormat = 0;
            nAbsoluteAlign = 0;
          }
        }
        DeleteCHTMLStringTable(test);
        CHTMLViewer_AddItem( me, pppItems, pItems );
        (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
        (*pppItems)[ (*pItems) - 1 ] ->m_nBmpBkg = nTableBmp;
        (*pppItems)[ (*pItems) - 1 ] ->m_colBkg = colTableBkg;
        if ( !nLevel && me->m_pProgress )
          me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
        for ( e = i + 1;szHTMLData[ e ];e++ ) {
          if ( !strnicmp( &szHTMLData[ e ], "<table", 6 ) )
            nTables++;
          if ( !strnicmp( &szHTMLData[ e ], "</table", 7 ) ) {
            if ( !nTables )
              break;
            nTables--;
          }
        }
        do {
          int a;
          for ( ;i < e && szHTMLData[ i ] && strnicmp( &szHTMLData[ i ], "<tr", 3 ) && strnicmp( &szHTMLData[ i ], "<td", 3 ) && strnicmp( &szHTMLData[ i ], "<th", 3 );i++ ) {
            if ( !strnicmp( &szHTMLData[ i ], "<form", 5 ) && me->m_pProgress ) {
              char szAction[ 1024 ];
              char *szMethod = "get";
              CHTMLStringTable* st;
              *szAction = 0;
              st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
              for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
                char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
                char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
                if ( !stricmp( str0, "action" ) )
                  CHTMLViewer_AddBaseToUrl( me, str1, szAction, szBaseUrl );
                else if ( !stricmp( str0, "method" ) )
                  szMethod = str1;
              }
              me->m_nCurrForm = me->m_pProgress->vtable->CreateForm( me->m_pProgress, szAction, szMethod );
              DeleteCHTMLStringTable(st);
            } else if ( !strnicmp( &szHTMLData[ i ], "</form", 6 ) )
              me->m_nCurrForm = -1;
          }
          nCol = 0;
          bgCol = bgTrCol = -1;
          do {
            int tt;
            int nTr = 0;
            int nTD_HFormat;
            int nTD_VFormat = 0;
            int nBmpBkg = -1;
            int nTD_Height = 0;
            int nTD_NoBreak = 0;
            int nTD_Header = 0;
            int nStart, nTd, nSubTable, Restore;
            CHTMLStringTable* test;
            if ( !strnicmp( &szHTMLData[ i ], "<tr", 3 ) ) {
              CHTMLStringTable* st;
              st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
              for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
                char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
                char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
                if ( !stricmp( str0, "bgcolor" ) )
                  bgCol = bgTrCol = CHTMLViewer_GetCol( me, str1 );
                else if ( !stricmp( str0, "align" ) ) {
                  if ( !stricmp( str1, "center" ) || !stricmp( str1, "middle" ) )
                    TrHFormat = 2;
                  else if ( !stricmp( str1, "right" ) )
                    TrHFormat = 1;
                  else if ( !stricmp( str1, "justify" ) )
                    TrHFormat = 3;
                }
              }
              i++;
              DeleteCHTMLStringTable(st);
            }
            for ( ;i < e && szHTMLData[ i ] && strnicmp( &szHTMLData[ i ], "<tr", 3 ) && strnicmp( &szHTMLData[ i ], "</tr", 4 ) && strnicmp( &szHTMLData[ i ], "<td", 3 ) && strnicmp( &szHTMLData[ i ], "<th", 3 );i++ ) {
              if ( !strnicmp( &szHTMLData[ i ], "<form", 5 ) && me->m_pProgress ) {
                char szAction[ 1024 ];
                char *szMethod = "get";
                CHTMLStringTable* st;
                st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
                for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
                  char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
                  char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
                  if ( !stricmp( str0, "action" ) )
                    CHTMLViewer_AddBaseToUrl( me, str1, szAction, szBaseUrl );
                  else if ( !stricmp( str0, "method" ) )
                    szMethod = str1;
                }
                me->m_nCurrForm = me->m_pProgress->vtable->CreateForm( me->m_pProgress, szAction, szMethod );
                DeleteCHTMLStringTable(st);
              } else if ( !strnicmp( &szHTMLData[ i ], "</form", 6 ) )
                me->m_nCurrForm = -1;
            }
            test = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
            nTD_HFormat = TrHFormat;
            if ( !strnicmp( &szHTMLData[ i ], "<th", 3 ) ) {
              nTD_Header = 1;
              nTD_HFormat = 2;
            }
            for ( tt = 0;tt < CHTMLStringTable_GetCount(test);tt++ ) {
              char* str0 = CHTMLStringTable_GetString(test, tt, 0, 0);
              char* str1 = CHTMLStringTable_GetString(test, tt, 1, 0);
              if ( !stricmp( str0, "width" ) && !strstr( str1, "%" ) )
                nWidth = atoi( str1 );
              else if ( !stricmp( str0, "width" ) && strstr( str1, "%" ) && nTableWidth > 0 )
                nWidth = atoi( str1 ) * nTableWidth / 100;
              else if ( !stricmp( str0, "height" ) && !strstr( str1, "%" ) )
                nTD_Height = atoi( str1 );
              else if ( !stricmp( str0, "bgcolor" ) )
                bgCol = CHTMLViewer_GetCol( me, str1 );
              else if ( !stricmp( str0, "colspan" ) )
                nColSpan = atoi( str1 );
              else if ( !stricmp( str0, "rowspan" ) )
                nRowSpan = atoi( str1 );
              else if ( !stricmp( str0, "align" ) ) {
                if ( !stricmp( str1, "right" ) )
                  nTD_HFormat = 1;
                else if ( !stricmp( str1, "center" ) || !stricmp( str1, "middle" ) )
                  nTD_HFormat = 2;
                else if ( !stricmp( str1, "justify" ) )
                  nTD_HFormat = 3;
              } else if ( !stricmp( str0, "valign" ) ) {
                if ( !stricmp( str1, "bottom" ) )
                  nTD_VFormat = 1;
                if ( !stricmp( str1, "center" ) || !stricmp( str1, "middle" ) )
                  nTD_VFormat = 2;
              } else if ( !stricmp( str0, "background" ) && *str1 ) {
                char szFullUrl[ 2048 ];
                CHTMLItem *pTmp;
                CHTMLViewer_AddBaseToUrl( me, str1, szFullUrl, szBaseUrl );
                pTmp = CHTMLItem_NewImage( szFullUrl, 0, 0, 0, 0, me->m_Images, 0, bNoBreak, TRUE, 0 );
                nBmpBkg = pTmp->m_nBmp;
                free(pTmp);
              } else if ( !stricmp( str0, "nobreak" ) || !stricmp( str0, "nowrap" ) )
                nTD_NoBreak = 1;
            }
            DeleteCHTMLStringTable(test);
            if ( nWidth && nTD_NoBreak )
              nTD_NoBreak = 0;
            if ( !strnicmp( &szHTMLData[ i ], "</tr", 4 ) || !strnicmp( &szHTMLData[ i ], "<tr", 3 ) )
              break;
            for ( ;i < e && szHTMLData[ i ] && szHTMLData[ i ] != '>';i++ )
              ;
            if ( szHTMLData[ i ] == '>' )
              i++;
            nStart = i;
            nTd = 0, nSubTable = 0;
            for ( ;i < e && szHTMLData[ i ];i++ ) {
              if ( !strnicmp( &szHTMLData[ i ], "<table", 6 ) )
                nSubTable++;
              else if ( !strnicmp( &szHTMLData[ i ], "</table", 7 ) )
                nSubTable--;
              if ( nSubTable )
                continue;
              if ( !strnicmp( &szHTMLData[ i ], "<td", 3 ) || !strnicmp( &szHTMLData[ i ], "</td", 4 ) || !strnicmp( &szHTMLData[ i ], "<th", 3 ) || !strnicmp( &szHTMLData[ i ], "</th", 4 ) || !strnicmp( &szHTMLData[ i ], "</tr", 4 ) || !strnicmp( &szHTMLData[ i ], "<tr", 3 ) )
                break;
            }
            Restore = szHTMLData[ i ];
            if ( nStart == e )
              break;
            szHTMLData[ i ] = 0;
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewCompound( nRow, nCol, nWidth, bCenter ? 2 : nHFormat, nTD_VFormat, bgCol, bTableBorder, nColSpan, nRowSpan, nCellPadding, nCellSpacing, nTableWidth, nBmpBkg, nTD_Height, nTableHeight, colTableBorder, nAbsoluteAlign );
            while ( szHTMLData[ nStart ] == ' ' )
              nStart++;
            CHTMLViewer_AddHTML( me, &szHTMLData[ nStart ], &((*pppItems)[ (*pItems) - 1 ] ->m_ppSubItems), &((*pppItems)[ (*pItems) - 1 ] ->m_nSubItems), szFont, nFontSize + ( nTD_Header ? 4 : 0 ), colText, szBaseUrl, nLevel + 1, nTD_HFormat, nTD_NoBreak );
            if ( !nLevel && me->m_pProgress )
              me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
            nCol++;
            if ( nColSpan )
              nCol += nColSpan - 1;
            nWidth = 0;
            bgCol = bgTrCol;
            nColSpan = 1;
            nRowSpan = 1;
            szHTMLData[ i ] = Restore;
            if ( !strnicmp( &szHTMLData[ i ], "</tr", 4 ) || !strnicmp( &szHTMLData[ i ], "<tr", 3 ) )
              break;
          } while ( i < e );
          if ( nCol ) {
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
            if ( !nLevel && me->m_pProgress )
              me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
            nRow++;
          }
        } while ( i < e );
        bCenter = bPrevCenter;
        nHFormat = nPrevHFormat;
        CHTMLViewer_AddItem( me, pppItems, pItems );
        (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
        if ( !nLevel && me->m_pProgress )
          me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
        for ( i = e;szHTMLData[ i ] && szHTMLData[ i ] != '>';i++ )
          ;
        if ( !szHTMLData[ i ] )
          i--;
        else
          while ( szHTMLData[ i ] && szHTMLData[ i + 1 ] == ' ' )
            i++;
        continue;
      }
      for ( t = 0;pszTags[ t ];t++ ) {
        int nTL = strlen( pszTags[ t ] );
        if ( !strnicmp( &szHTMLData[ i ], pszTags[ t ], nTL ) &&
             ( !szHTMLData[ i + nTL ] || szHTMLData[ i + nTL ] == ' ' || szHTMLData[ i + nTL ] == '>' ) )
          break;
      }
      if ( pszTags[ t ] ) {
        int a;
        if ( t == 18 ) { // Font
          CHTMLStringTable* st;
          st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
          for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
            char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
            char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
            if ( !stricmp( str0, "face" ) )
              strcpy( szCurrFont, str1 );
            else if ( !stricmp( str0, "color" ) )
              colCurText = CHTMLViewer_GetCol( me, str1 );
            else if ( !stricmp( str0, "size" ) && atoi( str1 ) ) {
              if ( str1[ 0 ] == '+' || str1[ 0 ] == '-' )
                nCurrFontSize = nFontSize + atoi( str1 );
              else if ( str1[ 0 ] )
                nCurrFontSize = 4 * atoi( str1 ) + 8;
            }
          }
          DeleteCHTMLStringTable(st);
        } else if ( t == 20 ) { // Image
          CHTMLStringTable* st;
          int nImgWidth = 0, nImgHeight = 0, a, bCheat = 0, nAbsAlign = -1, nBorder = 0;
          st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
          for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
            char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
            char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
            if ( !stricmp( str0, "width" ) && !strstr( str1, "%" ) ) {
              nImgWidth = atoi( str1 );
              if ( !nImgWidth )
                bCheat = 1;
            } else if ( !stricmp( str0, "height" ) && !strstr( str1, "%" ) ) {
              nImgHeight = atoi( str1 );
              if ( !nImgHeight )
                bCheat = 1;
            } else if ( !stricmp( str0, "align" ) ) {
              if ( !stricmp( str1, "left" ) )
                nAbsAlign = 0;
              else if ( !stricmp( str1, "right" ) )
                nAbsAlign = 1;
            } else if ( !stricmp( str0, "border" ) )
              nBorder = atoi( str1 );
          }
          if ( nAbsAlign != -1 && !bCheat ) {
            int e, Restore;
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, nAbsAlign, 0 );
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewCompound( 0, 0, 0, nAbsAlign, 0, -1, 0, 1, 1, 4, 0, 0, -1, 0, 0, -1, nAbsAlign );
            for ( e = i;szHTMLData[ e ] && szHTMLData[ e ] != '>';e++ ) {
              if ( !strnicmp( &szHTMLData[ e ], "align", 5 ) )
                szHTMLData[ e ] = '_';
            }
            if ( szHTMLData[ e ] == '>' )
              e++;
            Restore = szHTMLData[ e ];
            szHTMLData[ e ] = 0;
            CHTMLViewer_AddHTML( me, &szHTMLData[ i ], &((*pppItems)[ (*pItems) - 1 ] ->m_ppSubItems), &((*pppItems)[ (*pItems) - 1 ] ->m_nSubItems), szFont, nFontSize, colText, szBaseUrl, nLevel + 1, nAbsAlign, bNoBreak );
            szHTMLData[ e ] = Restore;
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
          } else {
            for ( a = 0;!bCheat && a < CHTMLStringTable_GetCount(st);a++ ) {
              char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
              char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
              if ( !stricmp( str0, "src" ) ) {
                char szFullUrl[ 2048 ];
                CHTMLViewer_AddItem( me, pppItems, pItems );
                CHTMLViewer_AddBaseToUrl( me, str1, szFullUrl, szBaseUrl );
                (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewImage( szFullUrl, 0, 0, 0, bCenter ? 2 : nHFormat, me->m_Images, nCurrLink, bNoBreak, FALSE, nBorder );
                if ( nImgWidth )
                  (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx = nImgWidth;
                if ( nImgHeight )
                  (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy = nImgHeight;
                break;
              }
            }
          }
          DeleteCHTMLStringTable(st);
          if ( !nLevel && me->m_pProgress )
            me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
        } else if ( t == 9 ) { // Anchor
          CHTMLStringTable* st;
          int a;
          st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
          if ( CHTMLStringTable_GetCount(me->m_Links) == 0 )
            CHTMLStringTable_AddRow0(me->m_Links);
          for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
            char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
            char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
            if ( !stricmp( str0, "href" ) ) {
              char szFullUrl[ 2048 ];
              CHTMLStringVector* sv = CHTMLStringTable_GetAt(me->m_Links, 0);
              CHTMLViewer_AddBaseToUrl( me, str1, szFullUrl, szBaseUrl );
              for ( nCurrLink = 0;nCurrLink < CHTMLStringVector_GetCount(sv);nCurrLink++ )
                if ( !stricmp( CHTMLStringVector_GetString(sv, nCurrLink, 0), szFullUrl ) )
                  break;
              if ( nCurrLink == CHTMLStringVector_GetCount(sv) )
                CHTMLStringVector_AddString( sv, szFullUrl, -1 );
              bLink = 1;
            }
          }
          DeleteCHTMLStringTable(st);
        }
        i += strlen( pszTags[ t ] );
        if ( t == 1 )   // Italic
          bItalic = 1;
        else if ( t == 2 )
          bItalic = 0;
        else if ( t == 3 || t == 5 )   // Bold
          bBold = 1;
        else if ( t == 4 || t == 6 )
          bBold = 0;
        else if ( t == 7 )   // Underline
          bUnderline = 1;
        else if ( t == 8 )
          bUnderline = 0;
        else if ( t == 10 ) {
          bLink = 0;
          nCurrLink = -1;
        } else if ( t == 11 ) { // Small
          bSmall = 1;
          nCurrFontSize -= nCurrFontSize / 3;
        } else if ( t == 12 ) {
          bSmall = 0;
          nCurrFontSize = nFontSize;
        } else if ( t == 13 ) { // Big
          bBig = 1;
          nCurrFontSize += nCurrFontSize / 3;
        } else if ( t == 14 ) {
          bBig = 0;
          nCurrFontSize = nFontSize;
        } else if ( t == 15 )   // Center
          bCenter = 1;
        else if ( t == 16 )
          bCenter = 0;
        else if ( t == 19 ) { // End of Font
          nCurrFontSize = nFontSize;
          colCurText = colText;
          strcpy( szCurrFont, szFont );
        } else if ( t == 21 ) { // Heading text 1
          nCurrFontSize += nCurrFontSize;
          bBold = 1;
        } else if ( t == 32 ) { // Heading text 2
          nCurrFontSize += nCurrFontSize / 2;
          bBold = 1;
        } else if ( t == 34 ) { // Heading text 3
          nCurrFontSize += nCurrFontSize / 3;
          bBold = 1;
        } else if ( t == 22 || t == 33 || t == 35 ) { // End of Heading text
          nCurrFontSize = nFontSize;
          bBold = 0;
        } else if ( t == 38 ) { // Input
          int a;
          int nType = 0;
          int nSize = 10;
          int nMaxSize = 0;
          char *szValue = "";
          char *szSrc = NULL;
          int nImgWidth = 0, nImgHeight = 0;
          int bPassword = 0;
          char *szName = "";
          CHTMLStringTable* st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
          for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
            char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
            char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
            if ( !stricmp( str0, "type" ) ) {
              if ( !stricmp( str1, "text" ) || !stricmp( str1, "password" ) ) {
                nType = 0;
                if ( !stricmp( str1, "password" ) )
                  bPassword = 1;
              } else if ( !stricmp( str1, "button" ) || !stricmp( str1, "submit" ) )
                nType = 1;
              else if ( !stricmp( str1, "radio" ) )
                nType = 2;
              else if ( !stricmp( str1, "checkbox" ) )
                nType = 3;
              else if ( !stricmp( str1, "hidden" ) )
                nType = 4;
              else if ( !stricmp( str1, "image" ) )
                nType = 5;
              else
                nType = -1;
            } else if ( !stricmp( str0, "size" ) )
              nSize = atoi( str1 );
            else if ( !stricmp( str0, "maxlength" ) )
              nMaxSize = atoi( str1 );
            else if ( !stricmp( str0, "value" ) )
              szValue = str1;
            else if ( !stricmp( str0, "name" ) )
              szName = str1;
            else if ( !stricmp( str0, "src" ) )
              szSrc = str1;
            else if ( !stricmp( str0, "width" ) )
              nImgWidth = atoi( str1 );
            else if ( !stricmp( str0, "height" ) )
              nImgHeight = atoi( str1 );
          }
          if ( nType == 0 ) {
            char * szSize = (char*)malloc( nSize + 1 );
            for ( a = 0;a < nSize;a++ )
              szSize[ a ] = '0';
            szSize[ a ] = 0;
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( szSize, 15, 2, -1, "MS Sans serif", 15,
                                    0, 0, 0, RGB( 255, 255, 255 ), bCenter ? 2 : nHFormat, 0, -1, bNoBreak );
            free(szSize);
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx += 8;
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy += 4;
            free((*pppItems)[ (*pItems) - 1 ] ->m_szText);
            (*pppItems)[ (*pItems) - 1 ] ->m_szText = (char*)malloc( strlen( szValue ) + 1 );
            strcpy( (*pppItems)[ (*pItems) - 1 ] ->m_szText, szValue );
            ConvertFromHTMLCodes( (*pppItems)[ (*pItems) - 1 ] ->m_szText );
            ConvertFromUtf8Codes( (*pppItems)[ (*pItems) - 1 ] ->m_szText );
            if ( me->m_pProgress )
              (*pppItems)[ (*pItems) - 1 ] ->m_nControlID = me->m_pProgress->vtable->CreateTextBox(me->m_pProgress, nSize, nMaxSize, (*pppItems)[ (*pItems) - 1 ] ->m_scBounds, (*pppItems)[ (*pItems) - 1 ] ->m_szText, 0, szName, me->m_nCurrForm, bPassword );
          } else if ( nType == 1 ) {
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( szValue, 15, 2, -2, "MS Sans serif", 15,
                                    0, 0, 0, RGB( 192, 192, 192 ), bCenter ? 2 : nHFormat, 0, -1, bNoBreak );
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx += 8;
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy += 4;
            if ( me->m_pProgress )
              (*pppItems)[ (*pItems) - 1 ] ->m_nControlID = me->m_pProgress->vtable->CreateButton( me->m_pProgress, (*pppItems)[ (*pItems) - 1 ] ->m_scBounds, (*pppItems)[ (*pItems) - 1 ] ->m_szText, szName, me->m_nCurrForm );
          } else if ( nType == 2 ) {
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( "    ", 15, 2, -1, szCurrFont, nCurrFontSize,
                                    0, 0, 0, RGB( 255, 255, 255 ), bCenter ? 2 : nHFormat, 0, -1, bNoBreak );
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx = 13;
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy = 13;
            if ( me->m_pProgress )
              (*pppItems)[ (*pItems) - 1 ] ->m_nControlID = me->m_pProgress->vtable->CreateRadio(me->m_pProgress, (*pppItems)[ (*pItems) - 1 ] ->m_scBounds, szValue, szName, me->m_nCurrForm );
          } else if ( nType == 3 ) {
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( "    ", 15, 2, -1, szCurrFont, nCurrFontSize,
                                    0, 0, 0, RGB( 255, 255, 255 ), bCenter ? 2 : nHFormat, 0, -1, bNoBreak );
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx = 13;
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy = 13;
            if ( me->m_pProgress )
              (*pppItems)[ (*pItems) - 1 ] ->m_nControlID = me->m_pProgress->vtable->CreateCheckbox(me->m_pProgress, (*pppItems)[ (*pItems) - 1 ] ->m_scBounds, szValue, szName, me->m_nCurrForm );
          } else if ( nType == 4 ) {
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( szValue, 15, 2, -1, szCurrFont, nCurrFontSize,
                                    0, 0, 0, RGB( 255, 255, 255 ), bCenter ? 2 : nHFormat, 0, -1, bNoBreak );
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx = 0;
            (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy = 0;
            if ( me->m_pProgress )
              (*pppItems)[ (*pItems) - 1 ] ->m_nControlID = me->m_pProgress->vtable->CreateTextBox(me->m_pProgress, nSize, 0, (*pppItems)[ (*pItems) - 1 ] ->m_scBounds, (*pppItems)[ (*pItems) - 1 ] ->m_szText, 0, szName, me->m_nCurrForm, 0 );
          } else if ( nType == 5 ) {
            int nLink;
            char szFullUrl[ 2048 ];
            char szLink[ 25 ];
            CHTMLStringVector* sv = CHTMLStringTable_GetAt(me->m_Links, 0);
            CHTMLViewer_AddItem( me, pppItems, pItems );
            CHTMLViewer_AddBaseToUrl( me, szSrc, szFullUrl, szBaseUrl );
            strcpy( szLink, "Submit(" );
            itoa( me->m_nCurrForm, &szLink[ strlen( szLink ) ], 10 );
            strcat( szLink, ")" );
            for ( nLink = 0;nLink < CHTMLStringVector_GetCount(sv);nLink++ )
              if ( !stricmp( CHTMLStringVector_GetString(sv, nLink, 0), szLink ) )
                break;
            if ( nLink == CHTMLStringVector_GetCount(sv) )
              CHTMLStringVector_AddString( sv, szLink, -1 );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewImage( szFullUrl, 0, 0, 0, bCenter ? 2 : nHFormat, me->m_Images, nLink, bNoBreak, FALSE, 0 );
            if ( nImgWidth )
              (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx = nImgWidth;
            if ( nImgHeight )
              (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy = nImgHeight;
          }
        } else if ( t == 39 || t == 40 ) { // Lists
          int nEnd;
          int nSub = 0;
          int nRow = 0;
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
          // Find the ending of this list, allowing sub lists
          for ( nEnd = i;szHTMLData[ nEnd ];nEnd++ ) {
            if ( !strnicmp( &szHTMLData[ nEnd ], t == 39 ? "<ul" : "<ol", 3 ) )
              nSub++;
            if ( !strnicmp( &szHTMLData[ nEnd ], t == 39 ? "</ul" : "</ol", 4 ) ) {
              if ( !nSub )
                break;
              else
                nSub--;
            }
          }
          // Go to beginning of list items
          for ( ;i < nEnd && szHTMLData[ i ] != '>';i++ )
            ;
          if ( szHTMLData[ i ] == '>' )
            i++;
          // Find first list item
          for ( ;i < nEnd && strnicmp( &szHTMLData[ i ], "<li", 3 );i++ )
            ;
          nSub = 0;
          while ( i < nEnd ) {
            int Restore, e;
            char szRow[ 135 ];
            for ( ;i < nEnd && szHTMLData[ i - 1 ] != '>';i++ )
              ;
            // Find next list item
            e = i + 1;
            for ( ;e < nEnd;e++ ) {
              if ( !strnicmp( &szHTMLData[ e ], "<ul", 3 ) || !strnicmp( &szHTMLData[ e ], "<ol", 3 ) )
                nSub++;
              if ( !strnicmp( &szHTMLData[ e ], "</ul", 4 ) || !strnicmp( &szHTMLData[ e ], "</ol", 4 ) )
                nSub--;
              if ( !nSub && !strnicmp( &szHTMLData[ e ], "<li", 3 ) )
                break;
            }
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewCompound( nRow, 0, 0, 0, 0, -1, 0, 1, 1, 2, 0, 0, -1, 0, 0, 0, -1 );
            if ( t == 39 ) {
              if ( me->m_nListLevel % 2 == 0 )
                strcpy( szRow, " l " );
              else
                strcpy( szRow, " m " );
              CHTMLViewer_AddHTML( me, szRow, &((*pppItems)[ (*pItems) - 1 ] ->m_ppSubItems), &((*pppItems)[ (*pItems) - 1 ] ->m_nSubItems), "Wingdings", nFontSize, colText, szBaseUrl, nLevel + 1, 0, 1 );
            } else {
              strcpy( szRow, "   " );
              if ( me->m_nListLevel % 2 == 0 )
                itoa( nRow + 1, &szRow[ strlen( szRow ) ], 10 );
              else {
                int n = strlen( szRow );
                szRow[ n ] = 'a' + nRow;
                szRow[ n + 1 ] = 0;
              }
              strcat( szRow, ".    " );
              CHTMLViewer_AddHTML( me, szRow, &((*pppItems)[ (*pItems) - 1 ] ->m_ppSubItems), &((*pppItems)[ (*pItems) - 1 ] ->m_nSubItems), szFont, nFontSize, colText, szBaseUrl, nLevel + 1, 0, 1 );
            }
            me->m_nListLevel++;
            Restore = szHTMLData[ e ];
            szHTMLData[ e ] = 0;
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewCompound( nRow, 1, 0, 0, 0, -1, 0, 1, 1, 2, 0, 0, -1, 0, 0, 0, -1 );
            CHTMLViewer_AddHTML( me, &szHTMLData[ i ], &((*pppItems)[ (*pItems) - 1 ] ->m_ppSubItems), &((*pppItems)[ (*pItems) - 1 ] ->m_nSubItems), szFont, nFontSize, colText, szBaseUrl, nLevel + 1, nHFormat, bNoBreak );
            szHTMLData[ e ] = Restore;
            i = e;
            nRow++;
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
            me->m_nListLevel--;
            bTextStarted = 0;
          }
        } else if ( t == 44 )   // Sub
          bSubText = 1;
        else if ( t == 45 )
          bSubText = 0;
        else if ( t == 46 )   // Sup
          bSupText = 1;
        else if ( t == 47 )
          bSupText = 0;
        else if ( t == 48 )   // Strike
          bStrike = 1;
        else if ( t == 49 )
          bStrike = 0;
        else if ( t == 50 ) { // Select
          CHTMLStringTable* stSelect;
          char *szName = "";
          int a, e, nEnd, Restore;
          CHTMLStringTable* stOptions;
          int nSelected = 0;
          int nMaxWidth = 0;
          stOptions = CreateCHTMLStringTable();
          stSelect = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
          for ( a = 0;a < CHTMLStringTable_GetCount(stSelect);a++ ) {
            char* str0 = CHTMLStringTable_GetString(stSelect, a, 0, 0);
            char* str1 = CHTMLStringTable_GetString(stSelect, a, 1, 0);
            if ( !stricmp( str0, "name" ) )
              szName = str1;
          }
          DeleteCHTMLStringTable(stSelect);
          // Find the ending of this list
          for ( nEnd = i;szHTMLData[ nEnd ] && strnicmp( &szHTMLData[ nEnd ], "</select", 8 );nEnd++ )
            ;
          // Go to beginning of list items
          for ( ;i < nEnd && szHTMLData[ i ] != '>';i++ )
            ;
          if ( szHTMLData[ i ] == '>' )
            i++;
          // Find first list item
          for ( ;i < nEnd && strnicmp( &szHTMLData[ i ], "<option", 7 );i++ )
            ;
          while ( i < nEnd ) {
            CHTMLStringTable* st;
            char *szValue = "";
            CHTMLItem *pTmp;
            st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
            for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
              char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
              char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
              if ( !stricmp( str0, "value" ) )
                szValue = str1;
              else if ( !stricmp( str0, "selected" ) )
                nSelected = CHTMLStringTable_GetCount(stOptions);
            }
            DeleteCHTMLStringTable(st);
            for ( ;i < nEnd && szHTMLData[ i ] != '>';i++ )
              ;
            if ( i < nEnd && szHTMLData[ i ] == '>' )
              i++;
            // Find next list item
            e = i + 1;
            for ( ;e < nEnd && strnicmp( &szHTMLData[ e ], "<option", 7 );e++ )
              ;
            Restore = szHTMLData[ e ];
            szHTMLData[ e ] = 0;
            CHTMLStringTable_AddRow0(stOptions);
            CHTMLStringTable_AddString( stOptions, CHTMLStringTable_GetCount(stOptions) - 1, &szHTMLData[ i ], -1 );
            ConvertFromHTMLCodes( CHTMLStringTable_GetString(stOptions, CHTMLStringTable_GetCount(stOptions) - 1, 0, 0 ) );
            ConvertFromUtf8Codes( CHTMLStringTable_GetString(stOptions, CHTMLStringTable_GetCount(stOptions) - 1, 0, 0 ) );
            CHTMLStringTable_AddString( stOptions, CHTMLStringTable_GetCount(stOptions) - 1, szValue, -1 );
            pTmp = CHTMLItem_NewText( &szHTMLData[ i ], 0, 0, 0, "MS Sans serif", 15,
                                                  0, 0, 0, 0, 0, 0, 0, 0 );
            if ( nMaxWidth < pTmp->m_scBounds.cx )
              nMaxWidth = pTmp->m_scBounds.cx;
            free(pTmp);
            szHTMLData[ e ] = Restore;
            i = e;
          }
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( CHTMLStringTable_GetString(stOptions, 0, 0, 0), 15, 2, -1, "MS Sans serif", 15,
                                  0, 0, 0, RGB( 255, 255, 255 ), bCenter ? 2 : nHFormat, 0, -1, bNoBreak );
          (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx = nMaxWidth + GetSystemMetrics( SM_CXHSCROLL ) + 8;
          (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy += 4;
          if ( me->m_pProgress )
            (*pppItems)[ (*pItems) - 1 ] ->m_nControlID = me->m_pProgress->vtable->CreateListbox(me->m_pProgress, (*pppItems)[ (*pItems) - 1 ] ->m_scBounds, stOptions, nSelected, szName, me->m_nCurrForm );
          DeleteCHTMLStringTable(stOptions);
        } else if ( t == 53 ) { // Textarea
          int a, nEnd, Restore;
          CHTMLStringTable* st;
          int nRows = 3;
          int nCols = 15;
          char *szName = "";
          st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
          for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
            char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
            char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
            if ( !stricmp( str0, "cols" ) )
              nCols = atoi( str1 );
            else if ( !stricmp( str0, "rows" ) )
              nRows = atoi( str1 );
            else if ( !stricmp( str0, "name" ) )
              szName = str1;
          }
          // Find the ending of this textarea
          for ( nEnd = i;szHTMLData[ nEnd ] && strnicmp( &szHTMLData[ nEnd ], "</textarea", 10 );nEnd++ )
            ;
          // Go to beginning of content
          for ( ;i < nEnd && szHTMLData[ i ] != '>';i++ )
            ;
          if ( szHTMLData[ i ] == '>' )
            i++;
          Restore = szHTMLData[ nEnd ];
          szHTMLData[ nEnd ] = 0;
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( " ", 15, 2, -1, "Courier New", 15,
                                  0, 0, 0, RGB( 255, 255, 255 ), bCenter ? 2 : nHFormat, 0, -1, bNoBreak );
          free((*pppItems)[ (*pItems) - 1 ] ->m_szText);
          (*pppItems)[ (*pItems) - 1 ] ->m_szText = (char*)malloc( strlen( &szHTMLData[ i ] ) + 1 );
          strcpy( (*pppItems)[ (*pItems) - 1 ] ->m_szText, &szHTMLData[ i ] );
          ConvertFromHTMLCodes( (*pppItems)[ (*pItems) - 1 ] ->m_szText );
          ConvertFromUtf8Codes( (*pppItems)[ (*pItems) - 1 ] ->m_szText );
          (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx = 4 + nCols * (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cx;
          (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy = 4 + nRows * (*pppItems)[ (*pItems) - 1 ] ->m_scBounds.cy;
          szHTMLData[ nEnd ] = Restore;
          i = nEnd;
          if ( me->m_pProgress )
            (*pppItems)[ (*pItems) - 1 ] ->m_nControlID = me->m_pProgress->vtable->CreateTextBox(me->m_pProgress, -1, 0, (*pppItems)[ (*pItems) - 1 ] ->m_scBounds, (*pppItems)[ (*pItems) - 1 ] ->m_szText, TRUE, szName, me->m_nCurrForm, 0 );
        } else if ( t == 57 && me->m_pProgress ) {
          int a;
          char szAction[ 1024 ];
          char *szMethod = "get";
          CHTMLStringTable* st;
          *szAction = 0;
          st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
          for ( a = 0; a < CHTMLStringTable_GetCount(st);a++ ) {
            char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
            char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
            if ( !stricmp( str0, "action" ) )
              CHTMLViewer_AddBaseToUrl( me, str1, szAction, szBaseUrl );
            else if ( !stricmp( str0, "method" ) )
              szMethod = str1;
          }
          DeleteCHTMLStringTable(st);
          me->m_nCurrForm = me->m_pProgress->vtable->CreateForm( me->m_pProgress, szAction, szMethod );
        } else if ( t == 58 )
          me->m_nCurrForm = -1;
        else if ( t == 59 ) {
          int e, Restore;
          for ( ;szHTMLData[ i ] && szHTMLData[ i ] != '>';i++ )
            ;
          if ( szHTMLData[ i ] == '>' )
            i++;
          for ( e = i;szHTMLData[ e ] && strnicmp( &szHTMLData[ e ], "</blockquote", 12 );e++ )
            ;
          Restore = szHTMLData[ e ];
          szHTMLData[ e ] = 0;
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewCompound( 0, 0, 0, bCenter ? 2 : nHFormat, 0, -1, 0, 1, 1, 15, 0, 0, -1, 0, 0, -1, -1 );
          CHTMLViewer_AddHTML( me, &szHTMLData[ i ], &((*pppItems)[ (*pItems) - 1 ] ->m_ppSubItems), &((*pppItems)[ (*pItems) - 1 ] ->m_nSubItems), szFont, nFontSize, colText, szBaseUrl, nLevel + 1, nHFormat, bNoBreak );
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
          szHTMLData[ e ] = Restore;
          i = e;
          if ( szHTMLData[ i ] )
            i--;
          bTextStarted = 0;
        }
        if ( t == 15 || t == 16 || t == 17 || t == 0 || t == 21 || t == 22 || t == 23 || t == 24 || t == 25 || t == 32 || t == 33 || t == 34 || t == 35 ) {
          int a, bClear = 0;
#ifdef ABSOLUTE_ALIGN
          if ( t == 0 ) {
            CHTMLStringTable* st;
            st = CHTMLViewer_GetProperties( me, &szHTMLData[ i ] );
            for ( a = 0;a < CHTMLStringTable_GetCount(st);a++ ) {
              char* str0 = CHTMLStringTable_GetString(st, a, 0, 0);
              char* str1 = CHTMLStringTable_GetString(st, a, 1, 0);
              if ( !stricmp( str0, "clear" ) ) {
                bClear = 1;
                break;
              }
            }
            DeleteCHTMLStringTable(st);
          }
#endif
          if ( t == 21 || t == 32 || t == 34 ) {
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( " ", ( bLink || bUnderline ) ? 8 : 0, ( bLink || bUnderline ) ? 1 : 0, bLink ? RGB( 0, 0, 255 ) : 0, szCurrFont, nCurrFontSize,
                                    bBold, bItalic, bLink ? RGB( 0, 0, 255 ) : colCurText, -1, bCenter ? 2 : nHFormat, 0, nCurrLink, bNoBreak );
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, bClear );
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( " ", ( bLink || bUnderline ) ? 8 : 0, ( bLink || bUnderline ) ? 1 : 0, bLink ? RGB( 0, 0, 255 ) : 0, szCurrFont, nCurrFontSize,
                                    bBold, bItalic, bLink ? RGB( 0, 0, 255 ) : colCurText, -1, bCenter ? 2 : nHFormat, 0, nCurrLink, bNoBreak );
          }
          if ( bTextStarted || t == 0 ) {
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( " ", ( bLink || bUnderline ) ? 8 : 0, ( bLink || bUnderline ) ? 1 : 0, bLink ? RGB( 0, 0, 255 ) : 0, szCurrFont, nCurrFontSize,
                                    bBold, bItalic, bLink ? RGB( 0, 0, 255 ) : colCurText, -1, bCenter ? 2 : nHFormat, 0, nCurrLink, bNoBreak );
          }
          CHTMLViewer_AddItem( me, pppItems, pItems );
          if ( t == 25 )
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 2, 1, bCenter ? 2 : nHFormat, 0 );
          else
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, bClear );
          if ( ( bTextStarted && t == 17 ) || t == 25 ) {
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( " ", ( bLink || bUnderline ) ? 8 : 0, ( bLink || bUnderline ) ? 1 : 0, bLink ? RGB( 0, 0, 255 ) : 0, szCurrFont, nCurrFontSize,
                                    bBold, bItalic, bLink ? RGB( 0, 0, 255 ) : colCurText, -1, bCenter ? 2 : nHFormat, 0, nCurrLink, bNoBreak );
            CHTMLViewer_AddItem( me, pppItems, pItems );
            (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, bCenter ? 2 : nHFormat, 0 );
          }
          if ( !nLevel && me->m_pProgress )
            me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
          if ( !bNoBreak && ( t == 0 || t == 17 ) )
            bTextStarted = 0;
        }
        while ( szHTMLData[ i ] && szHTMLData[ i ] != '>' )
          i++;
        if ( szHTMLData[ i ] == '>' )
          i++;
        i--;
        continue;
      }
    } else {
      if ( szHTMLData[ i ] == ' ' || szHTMLData[ i ] == '\r' ||
           szHTMLData[ i ] == '\n' || szHTMLData[ i ] == '\t' ) {
        if ( szHTMLData[ i ] == '\t' ) {
          int nTab;
          int nChars = 0;
          char szTab[ 6 ];
          CHTMLViewer_AddItem( me, pppItems, pItems );
          for ( nTab = (*pItems) - 1;nTab > 0 && (*pppItems)[ nTab - 1 ] ->m_nType == TEXT; nTab-- ) {
            nChars += strlen( (*pppItems)[ nTab - 1 ] ->m_szText );
          }
          nChars = 5 - nChars % 5;
          for ( nTab = 0;nTab < nChars;nTab++ )
            szTab[ nTab ] = ' ';
          szTab[ nTab ] = 0;
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( szTab, ( bLink || bUnderline ) ? 8 : 0, ( bLink || bUnderline ) ? 1 : 0, bLink ? RGB( 0, 0, 255 ) : 0, szCurrFont, bSubText ? 2 * nCurrFontSize / 3 : nCurrFontSize,
                                  bBold, bItalic, bLink ? RGB( 0, 0, 255 ) : colCurText, -1, bCenter ? 2 : nHFormat, 0, nCurrLink, bNoBreak );
          if ( bSupText )
            (*pppItems)[ (*pItems) - 1 ] ->m_nFontSize = 2 * nCurrFontSize / 3;
          if ( bStrike ) {
            (*pppItems)[ (*pItems) - 1 ] ->m_nFrames |= 16;
            (*pppItems)[ (*pItems) - 1 ] ->m_nFrameSize = 1;
          }
        } else if ( bTextStarted || bNoBreak ) {
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( " ", ( bLink || bUnderline ) ? 8 : 0, ( bLink || bUnderline ) ? 1 : 0, bLink ? RGB( 0, 0, 255 ) : 0, szCurrFont, bSubText ? 2 * nCurrFontSize / 3 : nCurrFontSize,
                                  bBold, bItalic, bLink ? RGB( 0, 0, 255 ) : colCurText, -1, bCenter ? 2 : nHFormat, 0, nCurrLink, bNoBreak );
          if ( bSupText )
            (*pppItems)[ (*pItems) - 1 ] ->m_nFontSize = 2 * nCurrFontSize / 3;
          if ( bStrike ) {
            (*pppItems)[ (*pItems) - 1 ] ->m_nFrames |= 16;
            (*pppItems)[ (*pItems) - 1 ] ->m_nFrameSize = 1;
          }
        }
        if ( szHTMLData[ i ] == '\r' || szHTMLData[ i ] == '\n' ) {
          CHTMLViewer_AddItem( me, pppItems, pItems );
          (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewBreak( 0, 0, nHFormat, 0 );
          if ( szHTMLData[ i ] == '\r' && szHTMLData[ i + 1 ] == '\n' )
            i++;
        }
      } else {
        int j;
        char Restore;
        for ( j = i;szHTMLData[ j ] && szHTMLData[ j ] != ' ' && szHTMLData[ j ] != '\r' &&
              szHTMLData[ j ] != '\n' && szHTMLData[ j ] != '\t' && szHTMLData[ j ] != '<';j++ )
          ;
        Restore = szHTMLData[ j ];
        szHTMLData[ j ] = 0;
        CHTMLViewer_AddItem( me, pppItems, pItems );
        (*pppItems)[ (*pItems) - 1 ] = CHTMLItem_NewText( &szHTMLData[ i ], ( bLink || bUnderline ) ? 8 : 0, ( bLink || bUnderline ) ? 1 : 0, bLink ? RGB( 0, 0, 255 ) : 0, szCurrFont, bSubText ? 2 * nCurrFontSize / 3 : nCurrFontSize,
                                bBold, bItalic, bLink ? RGB( 0, 0, 255 ) : colCurText, -1, bCenter ? 2 : nHFormat, 0, nCurrLink, bNoBreak );
        if ( bSupText )
          (*pppItems)[ (*pItems) - 1 ] ->m_nFontSize = 2 * nCurrFontSize / 3;
        if ( bStrike ) {
          (*pppItems)[ (*pItems) - 1 ] ->m_nFrames |= 16;
          (*pppItems)[ (*pItems) - 1 ] ->m_nFrameSize = 1;
        }
        szHTMLData[ j ] = Restore;
        i = j - 1;
        bTextStarted = TRUE;
      }
      if ( !nLevel && me->m_pProgress )
        me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
    }
  }
  if ( !nLevel && me->m_pProgress )
    me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, i, nLen );
}
void CHTMLViewer_CalcPositions( CHTMLViewer* me, RECT* rPos, CHTMLItem*** pppItems, int* pItems ) {
  POINT pStart = {rPos->left, rPos->top};
  int nPrevLine = 0, i;
  int nColumnWidths = 0;
  int *pnMinColumnWidths = NULL;
  int *pnAMinColumnWidths = NULL;
  int *pnMaxColumnWidths = NULL;
  int *pnFixedColumnWidths = NULL;
  int *pnRowSpans = NULL;
  int nCurrWidth = 0;
  int nRowSpanOffset = 0;
  int nRowSpanPosOffset = 0;
  int *pnRowHeights = NULL;
  int *pnFixedRowHeights = NULL;
  int nRowHeights = 0;
  int nCurrTableWidth = 0;
  int nRestoredTopPos;
  int nBreakBeforeTable;
  int nAbsoluteAlign = 0;
  int *pnAbsoluteAlignLeftOffset = NULL;
  int *pnAbsoluteAlignRightOffset = NULL;
  int *pnAbsoluteAlignBottom = NULL;
  for ( i = 0;i < (*pItems) + 1;i++ ) {
    // Make sure that all images are loaded before the size of them are used
    if ( i < (*pItems) && (*pppItems)[ i ] ->m_nType == IMAGE ) {
      if ( me->m_pProgress )
        me->m_pProgress->vtable->Reading( me->m_pProgress, TRUE, (*pppItems)[ i ] ->m_nBmp, CHTMLBitmaps_GetCount(me->m_Images) );
      CHTMLItem_WaitForImage( (*pppItems)[ i ], me->m_Images, me->m_bWaitForImages );
    }
    // Set <HR> width to something small
    if ( i < (*pItems) && (*pppItems)[ i ] ->m_nType == BREAK && (*pppItems)[ i ] ->m_scBounds.cx )
      (*pppItems)[ i ] ->m_scBounds.cx = 1;
    // New table, create list with max/min column widths, fixed widths and rowspans
    if ( i < (*pItems) - 1 && (*pppItems)[ i ] ->m_nType == COMPOUND && !nColumnWidths ) {
      int k, nTest, nTableWidth;
      nRestoredTopPos = pStart.y;
      nBreakBeforeTable = i - 1;
      for ( nTest = 0;nTest < 2;nTest++ ) {
        int col, k = i, t;
        int nX = 0, nY = 0, nXR = 0;
        do {
          for ( k;k < (*pItems);k++ ) {
            // Check if end of row in table
            if ( (*pppItems)[ k ] ->m_nType != COMPOUND || (*pppItems)[ k ] ->m_nCol != nX || (*pppItems)[ k ] ->m_nRow != nY )
              break;
            // Handle Rowspan
            while ( nX + nXR < nColumnWidths && pnRowSpans[ nX + nXR ] ) {
              pnRowSpans[ nX + nXR ] --;
              nXR++;
            }
            nX += (*pppItems)[ k ] ->m_nColSpan - 1;
            // Create more columns if we passed the edge
            if ( nX + nXR >= nColumnWidths ) {
              pnMinColumnWidths = ( int* ) realloc( pnMinColumnWidths, sizeof( int ) * ( nX + nXR + 1 ) );
              pnAMinColumnWidths = ( int* ) realloc( pnAMinColumnWidths, sizeof( int ) * ( nX + nXR + 1 ) );
              pnMaxColumnWidths = ( int* ) realloc( pnMaxColumnWidths, sizeof( int ) * ( nX + nXR + 1 ) );
              pnFixedColumnWidths = ( int* ) realloc( pnFixedColumnWidths, sizeof( int ) * ( nX + nXR + 1 ) );
              pnRowSpans = ( int* ) realloc( pnRowSpans, sizeof( int ) * ( nX + nXR + 1 ) );
              for ( ;nColumnWidths <= nX + nXR;nColumnWidths++ ) {
                pnMinColumnWidths[ nColumnWidths ] = 0;
                pnAMinColumnWidths[ nColumnWidths ] = 0;
                pnMaxColumnWidths[ nColumnWidths ] = 0;
                pnFixedColumnWidths[ nColumnWidths ] = 0;
                pnRowSpans[ nColumnWidths ] = 0;
              }
            }
            // Save Rowspan
            if ( (*pppItems)[ k ] ->m_nRowSpan > 1 ) {
              for ( col = 0;col < (*pppItems)[ k ] ->m_nColSpan;col++ )
                pnRowSpans[ (*pppItems)[ k ] ->m_nCol + nXR + col ] = (*pppItems)[ k ] ->m_nRowSpan - 1;
            }
            // Cached widths (doesn't improve performance as much as I wanted...)
            if ( (*pppItems)[ k ] ->m_nColSpan == 1 && (*pppItems)[ k ] ->m_nMinColumnWidth >= 0 && (*pppItems)[ k ] ->m_nMaxColumnWidth >= 0 ) {
              if ( pnMaxColumnWidths[ nX + nXR ] < (*pppItems)[ k ] ->m_nMaxColumnWidth )
                pnMaxColumnWidths[ nX + nXR ] = (*pppItems)[ k ] ->m_nMaxColumnWidth;
              if ( pnMinColumnWidths[ nX + nXR ] < (*pppItems)[ k ] ->m_nMinColumnWidth )
                pnMinColumnWidths[ nX + nXR ] = (*pppItems)[ k ] ->m_nMinColumnWidth;
              if ( pnAMinColumnWidths[ nX + nXR ] < (*pppItems)[ k ] ->m_nAMinColumnWidth )
                pnAMinColumnWidths[ nX + nXR ] = (*pppItems)[ k ] ->m_nAMinColumnWidth;
              if ( (*pppItems)[ k ] ->m_nWidth )
                pnFixedColumnWidths[ nX + nXR ] = 1;
            } else {
              // Handle TD with fixed width without colspan
              if ( !nTest && (*pppItems)[ k ] ->m_nWidth && (*pppItems)[ k ] ->m_nColSpan == 1 ) {
                RECT min = {0};
                CHTMLViewer_CalcPositions( me, &min, &(*pppItems)[ k ] ->m_ppSubItems, &(*pppItems)[ k ] ->m_nSubItems );
                for ( t = 0;t < (*pppItems)[ k ] ->m_nSubItems;t++ ) {
                  if ( !(*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_bNoBreak && (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_nType == TEXT && !strcmp( (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_szText, " " ) )
                    continue;
                  if ( nX + nXR < nColumnWidths && pnMinColumnWidths[ nX + nXR ] < (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx )
                    pnMinColumnWidths[ nX + nXR ] = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx;
                  if ( nX + nXR < nColumnWidths && pnAMinColumnWidths[ nX + nXR ] < (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx )
                    pnAMinColumnWidths[ nX + nXR ] = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx;
                }
                if ( (*pppItems)[ k ] ->m_nWidth > pnMinColumnWidths[ nX + nXR ] )
                  pnMinColumnWidths[ nX + nXR ] = (*pppItems)[ k ] ->m_nWidth;
                pnMaxColumnWidths[ nX + nXR ] = pnMinColumnWidths[ nX + nXR ];
                pnFixedColumnWidths[ nX + nXR ] = 1;
              }
              // Handle TD with fixed width with colspan
              else if ( nTest && (*pppItems)[ k ] ->m_nWidth && (*pppItems)[ k ] ->m_nColSpan > 1 ) {
                int nTdW = 0, nMaxR = 0, nMinL = 0, nW = 0, q;
                RECT max = {0, 0, 0, 0};
                CHTMLViewer_CalcPositions( me, &max, &(*pppItems)[ k ] ->m_ppSubItems, &(*pppItems)[ k ] ->m_nSubItems );
                for ( t = 0;t < (*pppItems)[ k ] ->m_nSubItems;t++ ) {
                  if ( (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_nType == BREAK && ( t == (*pppItems)[ k ] ->m_nSubItems - 1 || (*pppItems)[ k ] ->m_ppSubItems[ t + 1 ] ->m_nType != COMPOUND ) )
                    continue;
                  if ( nMaxR < (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx )
                    nMaxR = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx;
                  if ( t == 0 || nMinL > (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x )
                    nMinL = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x;
                }
                nTdW = nMaxR - nMinL;
                for ( q = nX + nXR - (*pppItems)[ k ] ->m_nColSpan + 1;q < nX + nXR + 1 && q < nColumnWidths;q++ )
                  nW += pnMinColumnWidths[ q ] + (*pppItems)[ k ] ->m_nCellPadding * 2 + (*pppItems)[ k ] ->m_nCellSpacing;
                nW -= (*pppItems)[ k ] ->m_nCellPadding * 2 + (*pppItems)[ k ] ->m_nCellSpacing;
                if ( nTdW < (*pppItems)[ k ] ->m_nWidth )
                  nTdW = (*pppItems)[ k ] ->m_nWidth;
                while ( nW < nTdW ) {
                  int nTest = 0;
                  for ( q = nX + nXR - (*pppItems)[ k ] ->m_nColSpan + 1;q < nX + nXR + 1 && q < nColumnWidths;q++ ) {
                    if ( pnMinColumnWidths[ q ] && nW < nTdW && !pnFixedColumnWidths[ q ] ) {
                      nTest++;
                      pnMinColumnWidths[ q ] ++;
                      if ( pnMaxColumnWidths[ q ] < pnMinColumnWidths[ q ] )
                        pnMaxColumnWidths[ q ] = pnMinColumnWidths[ q ];
                      nW++;
                    }
                  }
                  if ( !nTest ) {
                    for ( q = nX + nXR - (*pppItems)[ k ] ->m_nColSpan + 1;q < nX + nXR + 1 && q < nColumnWidths && nW < nTdW;q++ ) {
                      if ( !pnFixedColumnWidths[ q ] ) {
                        pnMaxColumnWidths[ q ] ++;
                        pnMinColumnWidths[ q ] ++;
                        pnAMinColumnWidths[ q ] = pnMinColumnWidths[ q ];
                        nW++;
                        nTest++;
                      }
                    }
                  }
                  if ( !nTest )
                    break;
                }
              }
              // Handle TD without fixed width without colspan
              else if ( !nTest && (*pppItems)[ k ] ->m_nColSpan == 1 && !(*pppItems)[ k ] ->m_nWidth ) {
                RECT min = {0}, max = {0, 0, 2000, rPos->bottom};
                CHTMLViewer_CalcPositions( me, &min, &(*pppItems)[ k ] ->m_ppSubItems, &(*pppItems)[ k ] ->m_nSubItems );
                for ( t = 0;t < (*pppItems)[ k ] ->m_nSubItems;t++ ) {
                  if ( !(*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_bNoBreak && (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_nType == TEXT && !strcmp( (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_szText, " " ) )
                    continue;
                  if ( nX + nXR < nColumnWidths && pnMinColumnWidths[ nX + nXR ] < (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx )
                    pnMinColumnWidths[ nX + nXR ] = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx;
                  if ( nX + nXR < nColumnWidths && pnAMinColumnWidths[ nX + nXR ] < (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx )
                    pnAMinColumnWidths[ nX + nXR ] = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx;
                }
                if ( rPos->right ) {
                  int nMaxR = 0, nMinL;
                  CHTMLViewer_CalcPositions( me, &max, &(*pppItems)[ k ] ->m_ppSubItems, &(*pppItems)[ k ] ->m_nSubItems );
                  nMinL = max.right;
                  for ( t = 0;t < (*pppItems)[ k ] ->m_nSubItems;t++ ) {
                    if ( (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_nType == BREAK && ( t == (*pppItems)[ k ] ->m_nSubItems - 1 || (*pppItems)[ k ] ->m_ppSubItems[ t + 1 ] ->m_nType != COMPOUND ) )
                      continue;
                    if ( !(*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_bNoBreak && (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_nType == TEXT && !strcmp( (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_szText, " " ) )
                      continue;
                    if ( nMaxR < (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx )
                      nMaxR = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx;
                    if ( t == 0 || nMinL > (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x )
                      nMinL = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x;
                  }
                  if ( nX + nXR < nColumnWidths && pnMaxColumnWidths[ nX + nXR ] < nMaxR - nMinL )
                    pnMaxColumnWidths[ nX + nXR ] = nMaxR - nMinL;
                } else if ( pnMaxColumnWidths[ nX + nXR ] < pnMinColumnWidths[ nX + nXR ] )
                  pnMaxColumnWidths[ nX + nXR ] = pnMinColumnWidths[ nX + nXR ];
              }
              // Handle TD without fixed width with colspan
              else if ( nTest && (*pppItems)[ k ] ->m_nColSpan > 1 && !(*pppItems)[ k ] ->m_nWidth ) {
                RECT max = {0, 0, 0, 0};
                int nTdW = 0, nMaxR = 0, nMinL = 0, nW = 0, q;
                CHTMLViewer_CalcPositions( me, &max, &(*pppItems)[ k ] ->m_ppSubItems, &(*pppItems)[ k ] ->m_nSubItems );
                for ( t = 0;t < (*pppItems)[ k ] ->m_nSubItems;t++ ) {
                  if ( (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_nType == BREAK && ( t == (*pppItems)[ k ] ->m_nSubItems - 1 || (*pppItems)[ k ] ->m_ppSubItems[ t + 1 ] ->m_nType != COMPOUND ) )
                    continue;
                  if ( nMaxR < (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx )
                    nMaxR = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x + (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_scBounds.cx;
                  if ( t == 0 || nMinL > (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x )
                    nMinL = (*pppItems)[ k ] ->m_ppSubItems[ t ] ->m_pPos.x;
                }
                nTdW = nMaxR - nMinL;
                for ( q = nX + nXR - (*pppItems)[ k ] ->m_nColSpan + 1;q < nX + nXR + 1 && q < nColumnWidths;q++ )
                  nW += pnMinColumnWidths[ q ] + (*pppItems)[ k ] ->m_nCellPadding * 2 + (*pppItems)[ k ] ->m_nCellSpacing;
                nW -= (*pppItems)[ k ] ->m_nCellPadding * 2 + (*pppItems)[ k ] ->m_nCellSpacing;
                while ( nW < nTdW ) {
                  int nTest = 0;
                  for ( q = nX + nXR - (*pppItems)[ k ] ->m_nColSpan + 1;q < nX + nXR + 1 && q < nColumnWidths;q++ ) {
                    if ( pnMinColumnWidths[ q ] && nW < nTdW && !pnFixedColumnWidths[ q ] ) {
                      nTest++;
                      pnMinColumnWidths[ q ] ++;
                      if ( pnMaxColumnWidths[ q ] < pnMinColumnWidths[ q ] )
                        pnMaxColumnWidths[ q ] = pnMinColumnWidths[ q ];
                      nW++;
                    }
                  }
                  if ( !nTest ) {
                    for ( q = nX + nXR - (*pppItems)[ k ] ->m_nColSpan + 1;q < nX + nXR + 1 && q < nColumnWidths;q++ )
                      if ( !pnFixedColumnWidths[ q ] ) {
                        pnMaxColumnWidths[ nX + nXR ] ++;
                        pnMinColumnWidths[ nX + nXR ] ++;
                        pnAMinColumnWidths[ nX + nXR ] = pnMinColumnWidths[ nX + nXR ];
                        nW++;
                        nTest++;
                      }
                  }
                  if ( !nTest )
                    break;
                }
              }
              if ( rPos->right && nTest && (*pppItems)[ k ] ->m_nColumnWidth == -1 ) {
                (*pppItems)[ k ] ->m_nMinColumnWidth = pnMinColumnWidths[ nX + nXR ];
                (*pppItems)[ k ] ->m_nAMinColumnWidth = pnAMinColumnWidths[ nX + nXR ];
                (*pppItems)[ k ] ->m_nMaxColumnWidth = pnMaxColumnWidths[ nX + nXR ];
                (*pppItems)[ k ] ->m_nColumnWidth = rPos->right;
              }
            }
            nX++;
          }
          nY++;
          while ( nX + nXR < nColumnWidths ) {
            if ( pnRowSpans[ nX + nXR ] )
              pnRowSpans[ nX + nXR ] --;
            nXR++;
          }
          nX = 0;
          nXR = 0;
          // Check if end of table
          if ( k >= (*pItems) - 2 || (*pppItems)[ k ] ->m_nType != BREAK || (*pppItems)[ k + 1 ] ->m_nType != COMPOUND )
            break;
          if ( (*pppItems)[ k + 1 ] ->m_nCol != nX || (*pppItems)[ k + 1 ] ->m_nRow != nY )
            break;
          k += 1;
        } while ( 1 );
        for ( k = 0;k < nColumnWidths;k++ )
          pnRowSpans[ k ] = 0;
      }
      // Handle fixed width on the whole table
      nTableWidth = (*pppItems)[ i ] ->m_nTableWidth;
      if ( nTableWidth < 0 ) {
        if ( rPos->right )
          nTableWidth = -rPos->right * nTableWidth / 100;
        else
          nTableWidth = 0;
      }
      if ( !nTableWidth )   // If the table does not have fixed width, set the width to the document width
        nTableWidth = rPos->right;
      if ( nTableWidth ) {
        int k, nW, nBreak = 1;
        nW = (*pppItems)[ i ] ->m_nCellSpacing + (*pppItems)[ i ] ->m_nTableFrameSize * 2;
        for ( k = 0;k < nColumnWidths;k++ ) {
          nW += pnMinColumnWidths[ k ] + (*pppItems)[ i ] ->m_nCellPadding * 2 + (*pppItems)[ i ] ->m_nCellSpacing;
        }
        do {
          nBreak = 0;
          if ( nW < nTableWidth ) {
            for ( k = 0;k < nColumnWidths;k++ ) {
              if ( !pnFixedColumnWidths[ k ] && nW < nTableWidth && pnMinColumnWidths[ k ] < pnMaxColumnWidths[ k ] ) {
                pnMinColumnWidths[ k ] ++;
                nBreak++;
                nW++;
              }
            }
          }
          if ( !nBreak && (*pppItems)[ i ] ->m_nTableWidth && nW < nTableWidth ) {
            for ( k = 0;k < nColumnWidths;k++ ) {
              if ( nW < nTableWidth && !pnFixedColumnWidths[ k ] ) {
                pnMinColumnWidths[ k ] ++;
                nBreak++;
                nW++;
              }
            }
          }
          // Here is a fix for shrinking fixed-width columns to the table width when
          // the sum of the column widths are larger than the table width
          if ( nW > nTableWidth ) { // && (nW-nTableWidth)<20)
            int nMax = -1;
            for ( k = 0;k < nColumnWidths;k++ ) {
              if ( pnFixedColumnWidths[ k ] && nW > nTableWidth && pnMinColumnWidths[ k ] > 10 ) {
                if ( pnAMinColumnWidths[ k ] < pnMinColumnWidths[ k ] && ( nMax == -1 || pnMinColumnWidths[ k ] > pnMinColumnWidths[ nMax ] ) )
                  nMax = k;
              }
            }
            if ( nMax != -1 ) {
              pnMinColumnWidths[ nMax ] --;
              pnMaxColumnWidths[ nMax ] = pnMinColumnWidths[ nMax ];
              nBreak++;
              nW--;
            }
          }
        } while ( nBreak && nW != nTableWidth );
      }
      for ( k = 0;k < nColumnWidths;k++ ) {
        pnRowSpans[ k ] = 0;
      }
      // Get TD height by positioning the TDs
      for ( nTest = 0;nTest < 2;nTest++ ) {
        int a, colspan, col, nY = 0, nXR = 0, nX = 0;
        k = i;
        do {
          for ( k;k < (*pItems);k++ ) {
            RECT r = {0};
            // Check if end of table row
            if ( (*pppItems)[ k ] ->m_nType != COMPOUND || (*pppItems)[ k ] ->m_nCol != nX || (*pppItems)[ k ] ->m_nRow != nY )
              break;
            // Check that we are not over the edge of rowheights
            while ( nY + (*pppItems)[ k ] ->m_nRowSpan > nRowHeights ) {
              nRowHeights++;
              pnRowHeights = ( int* ) realloc( pnRowHeights, sizeof( int ) * nRowHeights );
              pnFixedRowHeights = ( int* ) realloc( pnFixedRowHeights, sizeof( int ) * nRowHeights );
              pnRowHeights[ nRowHeights - 1 ] = 0;
              pnFixedRowHeights[ nRowHeights - 1 ] = 0;
            }
            // Handle Rowspan
            while ( (*pppItems)[ k ] ->m_nCol + nXR < nColumnWidths && pnRowSpans[ (*pppItems)[ k ] ->m_nCol + nXR ] ) {
              pnRowSpans[ (*pppItems)[ k ] ->m_nCol + nXR ] --;
              nXR++;
            }
            nX += (*pppItems)[ k ] ->m_nColSpan - 1;
            // Save rowspan
            if ( (*pppItems)[ k ] ->m_nRowSpan > 1 ) {
              for ( col = 0;col < (*pppItems)[ k ] ->m_nColSpan;col++ )
                pnRowSpans[ (*pppItems)[ k ] ->m_nCol + nXR + col ] = (*pppItems)[ k ] ->m_nRowSpan - 1;
            }
            // Set width according to list of TD-widths
            if ( nColumnWidths > (*pppItems)[ k ] ->m_nCol + nXR ) {
              r.right = pnMinColumnWidths[ (*pppItems)[ k ] ->m_nCol + nXR ];
              for ( colspan = 1;colspan < (*pppItems)[ k ] ->m_nColSpan && nColumnWidths > (*pppItems)[ k ] ->m_nCol + nXR + colspan;colspan++ )
                r.right += pnMinColumnWidths[ (*pppItems)[ k ] ->m_nCol + nXR + colspan ] + 2 * (*pppItems)[ k ] ->m_nCellPadding + (*pppItems)[ k ] ->m_nCellSpacing;
            }
            // Make positioning on this TD
            if ( !nTest )
              CHTMLViewer_CalcPositions( me, &r, &(*pppItems)[ k ] ->m_ppSubItems, &(*pppItems)[ k ] ->m_nSubItems );
            // Handle TD without rowspan
            if ( !nTest && (*pppItems)[ k ] ->m_nRowSpan == 1 ) {
              int nHeight = 0;
              for ( a = 0;a < (*pppItems)[ k ] ->m_nSubItems;a++ )
                if ( nHeight < (*pppItems)[ k ] ->m_ppSubItems[ a ] ->m_pPos.y + (*pppItems)[ k ] ->m_ppSubItems[ a ] ->m_scBounds.cy )
                  nHeight = (*pppItems)[ k ] ->m_ppSubItems[ a ] ->m_pPos.y + (*pppItems)[ k ] ->m_ppSubItems[ a ] ->m_scBounds.cy;
              if ( pnRowHeights[ nY ] < nHeight )
                pnRowHeights[ nY ] = nHeight;
            }
            // Handle TD with rowspan
            else if ( nTest && (*pppItems)[ k ] ->m_nRowSpan > 1 ) {
              int nHeight = 0;
              int nRowHeight = 0;
              int row;
              for ( a = 0;a < (*pppItems)[ k ] ->m_nSubItems;a++ )
                if ( nHeight < (*pppItems)[ k ] ->m_ppSubItems[ a ] ->m_pPos.y + (*pppItems)[ k ] ->m_ppSubItems[ a ] ->m_scBounds.cy )
                  nHeight = (*pppItems)[ k ] ->m_ppSubItems[ a ] ->m_pPos.y + (*pppItems)[ k ] ->m_ppSubItems[ a ] ->m_scBounds.cy;
              for ( row = 0;nRowHeights > nY + row && row < (*pppItems)[ k ] ->m_nRowSpan;row++ ) {
                nRowHeight += pnRowHeights[ nY + row ];
                if ( row )
                  nRowHeight += 2 * (*pppItems)[ k ] ->m_nCellPadding + (*pppItems)[ k ] ->m_nCellSpacing;
              }
              // If the height of this row is to small, increase height on all including rows
              while ( nRowHeight < nHeight ) {
                for ( row = 0;nRowHeights > nY + row && row < (*pppItems)[ k ] ->m_nRowSpan && nRowHeight < nHeight;row++ ) {
                  pnRowHeights[ nY + row ] ++;
                  nRowHeight++;
                }
              }
            }
            // Handle fixed TD height without rowspan
            if ( !nTest && (*pppItems)[ k ] ->m_nHeight && (*pppItems)[ k ] ->m_nRowSpan <= 1 ) {
              if ( pnRowHeights[ nY ] < (*pppItems)[ k ] ->m_nHeight )
                pnRowHeights[ nY ] = (*pppItems)[ k ] ->m_nHeight;
              pnFixedRowHeights[ nY ] = 1;
            }
            // Handle fixed TD height with rowspan
            else if ( nTest && (*pppItems)[ k ] ->m_nHeight && (*pppItems)[ k ] ->m_nRowSpan > 1 ) {
              int row, nTDHeight = 0;
              for ( row = nY;row < nRowHeights && row < nY + (*pppItems)[ k ] ->m_nRowSpan;row++ ) {
                nTDHeight += pnRowHeights[ row ];
                pnFixedRowHeights[ row ] = 1;
              }
              while ( nTDHeight < (*pppItems)[ i ] ->m_nHeight ) {
                int nTest = 0;
                for ( row = nY;row < nRowHeights && row < nY + (*pppItems)[ k ] ->m_nRowSpan && nTDHeight < (*pppItems)[ i ] ->m_nHeight;row++ ) {
                  if ( pnFixedRowHeights[ row ] )
                    continue;
                  nTDHeight++;
                  pnRowHeights[ row ] ++;
                  nTest++;
                }
                if ( !nTest ) {
                  for ( row = nY;row < nRowHeights && row < nY + (*pppItems)[ k ] ->m_nRowSpan && nTDHeight < (*pppItems)[ i ] ->m_nHeight;row++ ) {
                    nTDHeight++;
                    pnRowHeights[ row ] ++;
                    nTest++;
                  }
                }
              }
            }
            nX++;
          }
          nY++;
          while ( nX + nXR < nColumnWidths ) {
            if ( pnRowSpans[ nX + nXR ] )
              pnRowSpans[ nX + nXR ] --;
            nXR++;
          }
          nX = 0;
          nXR = 0;
          // Check if end of table
          if ( k >= (*pItems) - 2 || (*pppItems)[ k ] ->m_nType != BREAK || (*pppItems)[ k + 1 ] ->m_nType != COMPOUND )
            break;
          if ( (*pppItems)[ k + 1 ] ->m_nCol != nX || (*pppItems)[ k + 1 ] ->m_nRow != nY )
            break;
          k += 1;
        } while ( 1 );
        for ( k = 0;k < nColumnWidths;k++ )
          pnRowSpans[ k ] = 0;
      }
      nCurrTableWidth = (*pppItems)[ i ] ->m_nCellSpacing + (*pppItems)[ i ] ->m_nTableFrameSize * 2;
      for ( nTest = 0;nTest < nColumnWidths;nTest++ )
        nCurrTableWidth += pnMinColumnWidths[ nTest ] + (*pppItems)[ i ] ->m_nCellPadding * 2 + (*pppItems)[ i ] ->m_nCellSpacing;
      // Handle fixed tabel height
      if ( (*pppItems)[ i ] ->m_nTableHeight ) {
        int nTableHeight = 0;
        for ( k = 0;k < nRowHeights;k++ )
          nTableHeight += pnRowHeights[ k ];
        while ( nTableHeight < (*pppItems)[ i ] ->m_nTableHeight ) {
          int nTest = 0;
          for ( k = 0;k < nRowHeights && nTableHeight < (*pppItems)[ i ] ->m_nTableHeight;k++ ) {
            if ( pnFixedRowHeights[ k ] )
              continue;
            nTableHeight++;
            pnRowHeights[ k ] ++;
            nTest++;
          }
          if ( !nTest )
            break;
        }
      }
      // Position and size of the break element representing the outer frame of the table
      if ( nBreakBeforeTable >= 0 ) {
        (*pppItems)[ nBreakBeforeTable ] ->m_pPos = pStart;
        (*pppItems)[ nBreakBeforeTable ] ->m_scBounds.cx = nCurrTableWidth;
        (*pppItems)[ nBreakBeforeTable ] ->m_scBounds.cy = (*pppItems)[ i ] ->m_nCellSpacing + (*pppItems)[ i ] ->m_nTableFrameSize * 2;
        for ( k = 0;k < nRowHeights;k++ )
          (*pppItems)[ nBreakBeforeTable ] ->m_scBounds.cy += pnRowHeights[ k ] + (*pppItems)[ i ] ->m_nCellPadding * 2 + (*pppItems)[ i ] ->m_nCellSpacing;
        (*pppItems)[ nBreakBeforeTable ] ->m_nFrames = 15;
        if ( ( int ) (*pppItems)[ i ] ->m_colFrame == -1 )
          (*pppItems)[ nBreakBeforeTable ] ->m_colFrame = -2;
        else
          (*pppItems)[ nBreakBeforeTable ] ->m_colFrame = (*pppItems)[ i ] ->m_colFrame;
        (*pppItems)[ nBreakBeforeTable ] ->m_nFrameSize = (*pppItems)[ i ] ->m_nTableFrameSize;
      }
    }
    // Positioning of TD
    if ( i < (*pItems) - 1 && (*pppItems)[ i ] ->m_nType == COMPOUND ) {
      int a, colspan, col, nTopPos, nHeight, row;
      RECT r = {0};
      if ( (*pppItems)[ i ] ->m_nCol == 0 ) {
        pStart.y += (*pppItems)[ i ] ->m_nCellSpacing;
        if ( (*pppItems)[ i ] ->m_nRow == 0 )
          pStart.y += (*pppItems)[ i ] ->m_nTableFrameSize;
        pStart.x += (*pppItems)[ i ] ->m_nCellSpacing + (*pppItems)[ i ] ->m_nTableFrameSize;
      }
      // Handle rowspan
      while ( (*pppItems)[ i ] ->m_nCol + nRowSpanOffset < nColumnWidths && pnRowSpans[ (*pppItems)[ i ] ->m_nCol + nRowSpanOffset ] ) {
        nRowSpanPosOffset += pnMinColumnWidths[ (*pppItems)[ i ] ->m_nCol + nRowSpanOffset ] + (*pppItems)[ i ] ->m_nCellPadding * 2 + (*pppItems)[ i ] ->m_nCellSpacing;
        pnRowSpans[ (*pppItems)[ i ] ->m_nCol + nRowSpanOffset ] --;
        nRowSpanOffset++;
      }
      if ( (*pppItems)[ i ] ->m_nRowSpan > 1 ) {
        for ( col = 0;col < (*pppItems)[ i ] ->m_nColSpan;col++ )
          pnRowSpans[ (*pppItems)[ i ] ->m_nCol + nRowSpanOffset + col ] = (*pppItems)[ i ] ->m_nRowSpan - 1;
      }
      // Set width according to list of TD-widths
      if ( nColumnWidths > (*pppItems)[ i ] ->m_nCol + nRowSpanOffset ) {
        r.right = pnMinColumnWidths[ (*pppItems)[ i ] ->m_nCol + nRowSpanOffset ];
        for ( colspan = 1;colspan < (*pppItems)[ i ] ->m_nColSpan && nColumnWidths > (*pppItems)[ i ] ->m_nCol + nRowSpanOffset + colspan;colspan++ )
          r.right += pnMinColumnWidths[ (*pppItems)[ i ] ->m_nCol + nRowSpanOffset + colspan ];
      }
      nHeight = pnRowHeights[ (*pppItems)[ i ] ->m_nRow ];
      for ( row = 1;nRowHeights > (*pppItems)[ i ] ->m_nRow + row && row < (*pppItems)[ i ] ->m_nRowSpan;row++ )
        nHeight += pnRowHeights[ (*pppItems)[ i ] ->m_nRow + row ] + 2 * (*pppItems)[ i ] ->m_nCellPadding + (*pppItems)[ i ] ->m_nCellSpacing;
      // Move this TD to it's position
      (*pppItems)[ i ] ->m_pPos.x = pStart.x + nRowSpanPosOffset;
      (*pppItems)[ i ] ->m_pPos.y = pStart.y;
      (*pppItems)[ i ] ->m_scBounds.cx = r.right + (*pppItems)[ i ] ->m_nCellPadding * 2 * (*pppItems)[ i ] ->m_nColSpan + ( (*pppItems)[ i ] ->m_nColSpan - 1 ) * (*pppItems)[ i ] ->m_nCellSpacing;
      (*pppItems)[ i ] ->m_scBounds.cy = nHeight + (*pppItems)[ i ] ->m_nCellPadding * 2;
      nTopPos = 0;
      if ( (*pppItems)[ i ] ->m_nVFormat ) {
        nHeight = 0;
        for ( a = 0;a < (*pppItems)[ i ] ->m_nSubItems;a++ )
          if ( nHeight < (*pppItems)[ i ] ->m_ppSubItems[ a ] ->m_pPos.y + (*pppItems)[ i ] ->m_ppSubItems[ a ] ->m_scBounds.cy )
            nHeight = (*pppItems)[ i ] ->m_ppSubItems[ a ] ->m_pPos.y + (*pppItems)[ i ] ->m_ppSubItems[ a ] ->m_scBounds.cy;
        if ( (*pppItems)[ i ] ->m_nVFormat == 1 )
          nTopPos = (*pppItems)[ i ] ->m_scBounds.cy - nHeight - (*pppItems)[ i ] ->m_nCellPadding * 2;
        else if ( (*pppItems)[ i ] ->m_nVFormat == 2 )
          nTopPos = ( (*pppItems)[ i ] ->m_scBounds.cy - nHeight - (*pppItems)[ i ] ->m_nCellPadding * 2 ) / 2;
      }
      CHTMLItem_Offset( (*pppItems)[ i ], pStart.x + nRowSpanPosOffset + (*pppItems)[ i ] ->m_nCellPadding, pStart.y + (*pppItems)[ i ] ->m_nCellPadding + nTopPos );
      if ( (*pppItems)[ i + 1 ] ->m_nType == BREAK ) {
        for ( a = (*pppItems)[ i ] ->m_nCol + nRowSpanOffset + 1;a < nColumnWidths;a++ )
          if ( pnRowSpans[ a ] )
            pnRowSpans[ a ] --;
      }
    }
    // Row-break
    if ( ( i == (*pItems) ||
           ( !( (*pppItems)[ i ] ->m_nType == TEXT && !strcmp( (*pppItems)[ i ] ->m_szText, " " ) ) &&
             (*pppItems)[ i ] ->m_scBounds.cx + pStart.x > rPos->right ) ||
           (*pppItems)[ i ] ->m_nType == BREAK ) ) {
      if ( i < (*pItems) && (*pppItems)[ i ] ->m_nType == BREAK && (*pppItems)[ i ] ->m_scBounds.cy && ( i == (*pItems) - 1 || (*pppItems)[ i + 1 ] ->m_nType != COMPOUND ) )
        (*pppItems)[ i ] ->m_scBounds.cx = rPos->right - rPos->left;
      if ( ( i == (*pItems) || !(*pppItems)[ i ] ->m_bNoBreak || (*pppItems)[ i ] ->m_nType == BREAK ) && ( i == (*pItems) || (*pppItems)[ i ] ->m_nType != COMPOUND ) ) {
        int a, nMaxHeight = 0, nWidth = 0, nCenter;
        BOOL bCenter;
        nRowSpanOffset = 0;
        nRowSpanPosOffset = 0;
        bCenter = ( nPrevLine < (*pItems) && (*pppItems)[ nPrevLine ] ->m_nHFormat == 2 );
        for ( a = nPrevLine;a < i;a++ ) {
          if ( (*pppItems)[ a ] ->m_nType != TEXT && (*pppItems)[ a ] ->m_nType != IMAGE && (*pppItems)[ a ] ->m_nType != COMPOUND )
            continue;
          if ( (*pppItems)[ a ] ->m_nType == COMPOUND && (*pppItems)[ a ] ->m_nRowSpan > 1 && nMaxHeight < (*pppItems)[ a ] ->m_nCellPadding * 2 )
            nMaxHeight = (*pppItems)[ a ] ->m_nCellPadding * 2;
          if ( (*pppItems)[ a ] ->m_nType == COMPOUND && (*pppItems)[ a ] ->m_nRowSpan > 1 )
            continue;
          if ( (*pppItems)[ a ] ->m_nHFormat )
            nWidth += (*pppItems)[ a ] ->m_scBounds.cx;
          if ( nMaxHeight < (*pppItems)[ a ] ->m_scBounds.cy )
            nMaxHeight = (*pppItems)[ a ] ->m_scBounds.cy;
        }
        if ( i && (*pppItems)[ i - 1 ] ->m_nType == COMPOUND ) {
          nWidth = nCurrTableWidth;
        }
        nCenter = 0;
        // Do not include the last space when adjusting for alignment
        if ( i && (*pppItems)[ i - 1 ] ->m_szText && !strcmp( (*pppItems)[ i - 1 ] ->m_szText, " " ) )
          nWidth -= (*pppItems)[ i - 1 ] ->m_scBounds.cx;
        // Move elements if center or right alignment
        if ( bCenter ) {
          nCenter = ( rPos->right - rPos->left - nWidth ) / 2;
          if ( nCenter < 0 )
            nCenter = 0;
        } else if ( nPrevLine < (*pItems) && (*pppItems)[ nPrevLine ] ->m_nHFormat == 1 )
          nCenter = ( rPos->right - rPos->left - nWidth );
        for ( a = nPrevLine;a <= i && a < (*pItems);a++ ) {
          // Align elements to underline of row
          if ( i >= (*pItems) - 1 || (*pppItems)[ i + 1 ] ->m_nType != COMPOUND ) {
            if ( (*pppItems)[ a ] ->m_nType != COMPOUND && !( a < (*pItems) - 1 && (*pppItems)[ a ] ->m_nType == BREAK && (*pppItems)[ a + 1 ] ->m_nType == COMPOUND ) )
              (*pppItems)[ a ] ->m_pPos.y += nMaxHeight - (*pppItems)[ a ] ->m_scBounds.cy;
          }
          // Move elements if center or right alignment
          if ( (*pppItems)[ a ] ->m_scBounds.cx < rPos->right && ( (*pppItems)[ a ] ->m_nType != BREAK || ( a < (*pItems) - 1 && (*pppItems)[ a ] ->m_nType == BREAK && (*pppItems)[ a + 1 ] ->m_nType == COMPOUND ) ) ) {
            (*pppItems)[ a ] ->m_pPos.x += nCenter;
            if ( (*pppItems)[ a ] ->m_nType == COMPOUND )
              CHTMLItem_Offset( (*pppItems)[ a ], nCenter, 0 );
          }
        }
        // Justify
        if ( rPos->right && nPrevLine < (*pItems) && i < (*pItems) && (*pppItems)[ i ] ->m_nType != BREAK && (*pppItems)[ nPrevLine ] ->m_nHFormat == 3 && i - nPrevLine > 2 && (*pItems) - nPrevLine > 2 ) {
          int b;
          while ( nWidth < rPos->right - rPos->left ) {
            int bBreak = TRUE;
            for ( a = nPrevLine + 2;nWidth < rPos->right - rPos->left && a < i && a < (*pItems);a++ ) {
              if ( (*pppItems)[ a ] ->m_szText && !strcmp( (*pppItems)[ a ] ->m_szText, " " ) )
                continue;
              bBreak = FALSE;
              (*pppItems)[ a ] ->m_pPos.x++;
              for ( b = a + 1;b <= i && b < (*pItems);b++ )
                (*pppItems)[ b ] ->m_pPos.x++;
              nWidth++;
            }
            if ( bBreak )
              break;
          }
        }
        nPrevLine = i;
        if ( i < (*pItems) && (*pppItems)[ i ] ->m_nType == BREAK ) {
          (*pppItems)[ i ] ->m_pPos.x = 0;
          (*pppItems)[ i ] ->m_pPos.y = pStart.y + nMaxHeight;
          if ( i < (*pItems) - 1 && (*pppItems)[ i + 1 ] ->m_nType == TEXT )
            (*pppItems)[ i ] ->m_pPos.y -= (*pppItems)[ i + 1 ] ->m_scBounds.cy / 2;
        }
        pStart.y += nMaxHeight;
#ifdef ABSOLUTE_ALIGN
        {
          int q, p;
          int nUnder = 0;
          // Check if pStart is below absolute aligned objects or if the break has the clear attribute
          for ( q = 0;q < nAbsoluteAlign;q++ ) {
            if ( !( ( i && (*pppItems)[ i - 1 ] ->m_nType == COMPOUND && (*pppItems)[ i - 1 ] ->m_nAbsoluteAlign != -1 ) || ( i > 1 && (*pppItems)[ i - 2 ] ->m_nType == COMPOUND && (*pppItems)[ i - 2 ] ->m_nAbsoluteAlign != -1 ) ) &&
              ( pStart.y >= pnAbsoluteAlignBottom[ q ] || i == (*pItems) || ( i < (*pItems) && (*pppItems)[ i ] ->m_nType == BREAK && (*pppItems)[ i ] ->m_bClear ) ) ) {
              if ( !( i < (*pItems) && (*pppItems)[ i ] ->m_nType == BREAK && (*pppItems)[ i ] ->m_bClear ) ) {
                for ( p = 0;p < nAbsoluteAlign;p++ ) {
                  if ( p == q )
                    continue;
                  if ( pnAbsoluteAlignLeftOffset[ p ] && pnAbsoluteAlignLeftOffset[ q ] && pnAbsoluteAlignBottom[ p ] > pStart.y )
                    break;
                  if ( pnAbsoluteAlignRightOffset[ p ] && pnAbsoluteAlignRightOffset[ q ] && pnAbsoluteAlignBottom[ p ] > pStart.y )
                    break;
                }
                if ( p < nAbsoluteAlign )
                  continue;
              }
              rPos->left -= pnAbsoluteAlignLeftOffset[ q ];
              rPos->right += pnAbsoluteAlignRightOffset[ q ];
              if ( i < (*pItems) && (*pppItems)[ i ] ->m_nType == BREAK && (*pppItems)[ i ] ->m_bClear ) {
                if ( pStart.y < pnAbsoluteAlignBottom[ q ] )
                  pStart.y = pnAbsoluteAlignBottom[ q ];
              }
              nAbsoluteAlign--;
              if ( !nAbsoluteAlign ) {
                free( pnAbsoluteAlignLeftOffset );
                pnAbsoluteAlignLeftOffset = NULL;
                free( pnAbsoluteAlignBottom );
                pnAbsoluteAlignBottom = NULL;
                free( pnAbsoluteAlignRightOffset );
                pnAbsoluteAlignRightOffset = NULL;
              } else {
                for ( p = q;p < nAbsoluteAlign;p++ ) {
                  pnAbsoluteAlignBottom[ p ] = pnAbsoluteAlignBottom[ p + 1 ];
                  pnAbsoluteAlignLeftOffset[ p ] = pnAbsoluteAlignLeftOffset[ p + 1 ];
                  pnAbsoluteAlignRightOffset[ p ] = pnAbsoluteAlignRightOffset[ p + 1 ];
                }
                q--;
              }
            }
          }
          // If the content does not fix between absolut aligned objects, move pStart below all those objects
          if ( i < (*pItems) && nAbsoluteAlign && (*pppItems)[ i ] ->m_nType == TEXT && *(*pppItems)[ i ] ->m_szText != ' ' && (*pppItems)[ i ] ->m_scBounds.cx > rPos->right - rPos->left ) {
            for ( p = 0;p < nAbsoluteAlign;p++ ) {
              if ( pStart.y < pnAbsoluteAlignBottom[ p ] )
                pStart.y = pnAbsoluteAlignBottom[ p ];
              rPos->left -= pnAbsoluteAlignLeftOffset[ p ];
              rPos->right += pnAbsoluteAlignRightOffset[ p ];
            }
            free( pnAbsoluteAlignLeftOffset );
            pnAbsoluteAlignLeftOffset = NULL;
            free( pnAbsoluteAlignBottom );
            pnAbsoluteAlignBottom = NULL;
            free( pnAbsoluteAlignRightOffset );
            pnAbsoluteAlignRightOffset = NULL;
            nAbsoluteAlign = 0;
          }
        }
#endif
        pStart.x = rPos->left;
      }
    }
    if ( i == (*pItems) )
      break;
    // Set position of ordinary texts and images and compound objects
    if ( (*pppItems)[ i ] ->m_nType != BREAK ) {
      (*pppItems)[ i ] ->m_pPos.x = pStart.x + nRowSpanPosOffset;
      (*pppItems)[ i ] ->m_pPos.y = pStart.y;
      pStart.x += (*pppItems)[ i ] ->m_scBounds.cx;
      // Offset x-pos with cellspacing
      if ( (*pppItems)[ i ] ->m_nType == COMPOUND )
        pStart.x += (*pppItems)[ i ] ->m_nCellSpacing;
    }
    // Just to make sure that those nasty rowspans that exceed the number of rows in a table doesn't cause any misalignment!
    if ( i && i < (*pItems) - 1 && (*pppItems)[ i ] ->m_nType == BREAK && (*pppItems)[ i - 1 ] ->m_nType == COMPOUND && !nColumnWidths )
      pStart.y = (*pppItems)[ nBreakBeforeTable ] ->m_pPos.y + (*pppItems)[ nBreakBeforeTable ] ->m_scBounds.cy;
    // Check if end of table and free list of TD widths if so
    if ( i < (*pItems) - 1 && (*pppItems)[ i ] ->m_nType == COMPOUND ) {
      if ( ( i >= (*pItems) - 2 ) ||
           !( ( (*pppItems)[ i + 1 ] ->m_nType == BREAK && (*pppItems)[ i + 2 ] ->m_nType == COMPOUND ) || (*pppItems)[ i + 1 ] ->m_nType == COMPOUND ) ||
           ( (*pppItems)[ i + 1 ] ->m_nType == COMPOUND && (*pppItems)[ i ] ->m_nCol + (*pppItems)[ i ] ->m_nColSpan != (*pppItems)[ i + 1 ] ->m_nCol ) ||
           ( (*pppItems)[ i + 1 ] ->m_nType == BREAK && (*pppItems)[ i + 2 ] ->m_nType == COMPOUND && ( (*pppItems)[ i + 2 ] ->m_nCol != 0 || (*pppItems)[ i + 2 ] ->m_nRow != (*pppItems)[ i ] ->m_nRow + 1 ) ) ) {
        free( pnMinColumnWidths );
        pnMinColumnWidths = NULL;
        free( pnAMinColumnWidths );
        pnAMinColumnWidths = NULL;
        free( pnMaxColumnWidths );
        pnMaxColumnWidths = NULL;
        free( pnFixedColumnWidths );
        pnFixedColumnWidths = NULL;
        free( pnRowSpans );
        pnRowSpans = NULL;
        nColumnWidths = 0;
        free( pnRowHeights );
        free( pnFixedRowHeights );
        pnRowHeights = NULL;
        pnFixedRowHeights = NULL;
        nRowHeights = 0;
        pStart.y += (*pppItems)[ i ] ->m_nCellSpacing;
        pStart.y += (*pppItems)[ i ] ->m_nTableFrameSize;
      }
    }
#ifdef ABSOLUTE_ALIGN
    if ( i > 2 && (*pppItems)[ i ] ->m_nType == BREAK && (*pppItems)[ i - 1 ] ->m_nType == BREAK && (*pppItems)[ i - 2 ] ->m_nType == COMPOUND && (*pppItems)[ i - 2 ] ->m_nAbsoluteAlign != -1 ) {
      int a, b;
      a = i - 2;
      for ( ;a > 0;a-- )
        if ( (*pppItems)[ a ] ->m_nType == BREAK && (*pppItems)[ a - 1 ] ->m_nType != COMPOUND )
          break;
      pStart.y = (*pppItems)[ a ] ->m_pPos.y;
      b = nAbsoluteAlign++;
      pnAbsoluteAlignLeftOffset = ( int* ) realloc( pnAbsoluteAlignLeftOffset, sizeof( int ) * nAbsoluteAlign );
      if ( (*pppItems)[ i - 2 ] ->m_nAbsoluteAlign == 0 ) {
        pnAbsoluteAlignLeftOffset[ b ] = (*pppItems)[ a ] ->m_scBounds.cx;
        rPos->left += (*pppItems)[ a ] ->m_scBounds.cx;
        pStart.x = (*pppItems)[ a ] ->m_scBounds.cx;
      } else
        pnAbsoluteAlignLeftOffset[ b ] = 0;
      pnAbsoluteAlignRightOffset = ( int* ) realloc( pnAbsoluteAlignRightOffset, sizeof( int ) * nAbsoluteAlign );
      pnAbsoluteAlignRightOffset[ b ] = (*pppItems)[ a ] ->m_scBounds.cx;
      if ( (*pppItems)[ i - 2 ] ->m_nAbsoluteAlign == 1 ) {
        pnAbsoluteAlignRightOffset[ b ] = (*pppItems)[ a ] ->m_scBounds.cx;
        rPos->right -= (*pppItems)[ a ] ->m_scBounds.cx;
      } else
        pnAbsoluteAlignRightOffset[ b ] = 0;
      pnAbsoluteAlignBottom = ( int* ) realloc( pnAbsoluteAlignBottom, sizeof( int ) * nAbsoluteAlign );
      pnAbsoluteAlignBottom[ b ] = pStart.y + (*pppItems)[ a ] ->m_scBounds.cy;
      pStart.x = rPos->left;
    }
#endif
  }
  if ( nAbsoluteAlign ) {
    free( pnAbsoluteAlignLeftOffset );
    pnAbsoluteAlignLeftOffset = NULL;
    free( pnAbsoluteAlignBottom );
    pnAbsoluteAlignBottom = NULL;
    free( pnAbsoluteAlignRightOffset );
    pnAbsoluteAlignRightOffset = NULL;
    nAbsoluteAlign = 0;
  }
}

void CHTMLViewer_SetHTML( CHTMLViewer* me, char *szHTMLData, char *szFont, int nFontSize, COLORREF colText, char *szBaseUrl ) {
  int i, a, b, e, q, bTag = 0, t, nPre = 0;
  BOOL bNoBreak = 0;
  char *szText;
  char *szUseFont;

  me->m_bReading = TRUE;
  me->m_bgCol = RGB( 255, 255, 255 );
  me->m_nBmpBkg = -1;
  me->m_nItemCount = 0;
  me->m_nLevelCount = 0;
  me->m_nCurrForm = -1;
  CHTMLStringTable_Reset(me->m_Links);
  for ( i = 0;i < me->m_nItems;i++ )
    DeleteCHTMLItem(me->m_ppItems[ i ]);
  free( me->m_ppItems );
  me->m_ppItems = NULL;
  me->m_nItems = 0;
  for ( i = 0;i < CHTMLBitmaps_GetCount(me->m_Images);i++ )
    CHTMLBitmap_DestroyBitmap(CHTMLBitmaps_GetAt(me->m_Images, i));
  szText = (char*)malloc( strlen( szHTMLData ) + 1 );
  strcpy( szText, szHTMLData );
  szUseFont = szFont;
  if ( strrchr( szBaseUrl, '.' ) && !stricmp( strrchr( szBaseUrl, '.' ) + 1, "txt" ) ) {
    bNoBreak = 1;
    szUseFont = "Courier New";
    nPre = 1;
  }
  for ( a = b = 0;szText[ a ];a++ ) {
    if ( !strnicmp( &szText[ a ], "<script", 7 ) ) {
      for ( ;szText[ a ] && strnicmp( &szText[ a ], "</script", 8 );a++ )
        ;
      for ( ;szText[ a ] && szText[ a ] != '>';a++ )
        ;
      continue;
    } else if ( !strnicmp( &szText[ a ], "<!--", 4 ) ) {
      for ( ;szText[ a ] && strnicmp( &szText[ a ], "-->", 3 );a++ )
        ;
      for ( ;szText[ a ] && szText[ a ] != '>';a++ )
        ;
      continue;
    } else if ( !strnicmp( &szText[ a ], "<title", 6 ) ) {
      int w = a + 7;
      while ( szText[ w ] && ( szText[ w ] == ' ' || szText[ w ] == '\t' || szText[ w ] == '\r' || szText[ w ] == '\n' ) )
        w++;
      for ( e = w;szText[ e ] && strnicmp( &szText[ e ], "</title>", 8 );e++ )
        ;
      while ( e > w && ( szText[ e - 1 ] == ' ' || szText[ e - 1 ] == '\t' || szText[ e - 1 ] == '\r' || szText[ e - 1 ] == '\n' ) )
        e--;
      q = szText[ e ];
      szText[ e ] = 0;
      if ( me->m_pProgress ) {
        char * szTitle = (char*)malloc( strlen( &szText[ w ] ) + 1 );
        strcpy( szTitle, &szText[ w ] );
        ConvertFromHTMLCodes( szTitle );
        ConvertFromUtf8Codes( szTitle );
        me->m_pProgress->vtable->SetTitle(me->m_pProgress,  szTitle );
        free(szTitle);
      }
      szText[ e ] = q;
      for ( ;szText[ a ] && strnicmp( &szText[ a ], "</title", 7 );a++ )
        ;
      for ( ;szText[ a ] && szText[ a ] != '>';a++ )
        ;
      continue;
    } else if ( !strnicmp( &szText[ a ], "<body", 5 ) ) {
      CHTMLStringTable* st;
      st = CHTMLViewer_GetProperties( me, &szText[ a ]);
      for ( t = 0;t < CHTMLStringTable_GetCount(st);t++ ) {
        char* str0 = CHTMLStringTable_GetString(st, t, 0, 0);
        char* str1 = CHTMLStringTable_GetString(st, t, 1, 0);
        if ( !stricmp( str0, "bgcolor" ) )
          me->m_bgCol = CHTMLViewer_GetCol( me, str1 );
        else if ( !stricmp( str0, "background" ) && *str1 ) {
          char szFullUrl[ 2048 ];
          CHTMLItem *pTmp;
          CHTMLViewer_AddBaseToUrl( me, str1, szFullUrl, szBaseUrl );
          pTmp = CHTMLItem_NewImage( szFullUrl, 0, 0, 0, 0, me->m_Images, 0, 0, TRUE, 0 );
          me->m_nBmpBkg = pTmp->m_nBmp;
          free(pTmp);
        }
      }
      DeleteCHTMLStringTable(st);
    } else if ( !strnicmp( &szText[ a ], "<style", 6 ) ) {
      int q = a;
      for ( ;szText[ a ] && strnicmp( &szText[ a ], "</style", 7 );a++ )
        ;
      if ( !szText[ a ] )
        a = q;
      for ( ;szText[ a ] && szText[ a ] != '>';a++ )
        ;
      continue;
    } else if ( !strnicmp( &szText[ a ], "<pre", 4 ) )
      nPre++;
    else if ( !strnicmp( &szText[ a ], "</pre", 5 ) )
      nPre--;
    if ( !bTag && szText[ a ] == '<' ) {
      for ( t = 0;pszTags[ t ];t++ ) {
        int nTL = strlen( pszTags[ t ] );
        if ( !strnicmp( &szText[ a ], pszTags[ t ], nTL ) &&
             ( !szText[ a + nTL ] || szText[ a + nTL ] == ' ' || szText[ a + nTL ] == '\t' || szText[ a + nTL ] == '\r' || szText[ a + nTL ] == '\n' || szText[ a + nTL ] == '>' ) )
          break;
      }
      if ( !pszTags[ t ] )
        bTag = 1;
    }
    if ( bTag && szText[ a ] == '>' ) {
      bTag = 0;
      continue;
    }
    if ( !bTag ) {
      if ( !nPre && ( szText[ a ] == '\t' || szText[ a ] == '\n' || szText[ a ] == '\r' || szText[ a ] == ' ' ) ) {
        if ( b && szText[ b - 1 ] == ' ' )
          continue;
        else
          szText[ b++ ] = ' ';
      } else
        szText[ b++ ] = szText[ a ];
    }
  }
  szText[ b ] = 0;
  CHTMLViewer_AddHTML( me, szText, &me->m_ppItems, &me->m_nItems, szUseFont, nFontSize, colText, szBaseUrl, 0, 0, bNoBreak );
  free(szText);
  if ( me->m_pProgress )
    me->m_pProgress->vtable->Reading( me->m_pProgress, FALSE, b, b );
  me->m_bReading = FALSE;
}

void CHTMLViewer_AdjustHR( CHTMLViewer* me, int nWidth, CHTMLItem*** pppItems, int* pItems ) {
  int i;
  for ( i = 0;i < (*pItems);i++ ) {
    (*pppItems)[ i ] ->m_nMaxColumnWidth = (*pppItems)[ i ] ->m_nMinColumnWidth = (*pppItems)[ i ] ->m_nAMinColumnWidth = (*pppItems)[ i ] ->m_nColumnWidth = -1;
    //  if((*pppItems)[i]->m_nType==BREAK && (*pppItems)[i]->m_scBounds.cy && (i==(*pItems)-1 || (*pppItems)[i+1]->m_nType!=COMPOUND))
    //   (*pppItems)[i]->m_scBounds.cx = nWidth;
    if ( (*pppItems)[ i ] ->m_ppSubItems )
      CHTMLViewer_AdjustHR( me, nWidth ? (*pppItems)[ i ] ->m_scBounds.cx - 2 * (*pppItems)[ i ] ->m_nCellPadding : 0, &(*pppItems)[ i ] ->m_ppSubItems, &(*pppItems)[ i ] ->m_nSubItems );
    if ( (*pppItems)[ i ] ->m_nControlID && me->m_pProgress )
      me->m_pProgress->vtable->MoveControl(me->m_pProgress, (*pppItems)[ i ] ->m_nControlID, (*pppItems)[ i ] ->m_pPos );
  }
}
void CHTMLViewer_CalcPositions1( CHTMLViewer* me, RECT* rPos ) {
  int i, nWidth;
  rPos->right -= 2;
  nWidth = rPos->right;
  CHTMLViewer_AdjustHR( me, 0, &me->m_ppItems, &me->m_nItems );
  CHTMLViewer_CalcPositions( me, rPos, &me->m_ppItems, &me->m_nItems );
  // Measure document size
  rPos->right = rPos->bottom = 0;
  for ( i = 0;i < me->m_nItems;i++ ) {
    if ( me->m_ppItems[ i ] ->m_nType == TEXT && !strcmp( me->m_ppItems[ i ] ->m_szText, " " ) )
      continue;
    if ( rPos->right < me->m_ppItems[ i ] ->m_pPos.x + me->m_ppItems[ i ] ->m_scBounds.cx )
      rPos->right = me->m_ppItems[ i ] ->m_pPos.x + me->m_ppItems[ i ] ->m_scBounds.cx;
    if ( rPos->bottom < me->m_ppItems[ i ] ->m_pPos.y + me->m_ppItems[ i ] ->m_scBounds.cy )
      rPos->bottom = me->m_ppItems[ i ] ->m_pPos.y + me->m_ppItems[ i ] ->m_scBounds.cy;
  }
  CHTMLViewer_AdjustHR( me, nWidth, &me->m_ppItems, &me->m_nItems );
  rPos->right;
  rPos->bottom;
}
void CHTMLViewer_Draw0( CHTMLViewer* me, HDC hDC, const RECT rClient, CHTMLItem*** pppItems, int* pItems, int nX, int nY ) {
  int i, n;
  HPEN pn;
  RECT r;
  SetBkMode( hDC, TRANSPARENT );
  SetTextAlign( hDC, TA_TOP | TA_LEFT );
  for ( i = 0;i < (*pItems);i++ ) {
    CHTMLBitmap* bmpBkg = CHTMLBitmaps_GetAt(me->m_Images, (*pppItems)[ i ] ->m_nBmpBkg);
    if ( (*pppItems)[ i ] ->m_nControlID != -1 )
      continue;
    SetRect(&r, (*pppItems)[ i ] ->m_pPos.x - nX, (*pppItems)[ i ] ->m_pPos.y - nY, (*pppItems)[ i ] ->m_scBounds.cx, (*pppItems)[ i ] ->m_scBounds.cy);
    r.right += r.left;
    r.bottom += r.top;
    if ( rClient.bottom < r.top || rClient.top > r.bottom || rClient.left > r.right || rClient.right < r.left )
      continue;
    if ( (*pppItems)[ i ] ->m_colBkg != -1 || (*pppItems)[ i ] ->m_bSelected ) {
      HBRUSH br = CreateSolidBrush( (*pppItems)[ i ] ->m_bSelected ? RGB( 0, 0, 128 ) : (*pppItems)[ i ] ->m_colBkg );
      FillRect( hDC, &r, br );
      DeleteObject( br );
    }
    if ( (*pppItems)[ i ] ->m_nBmpBkg != -1 && !CHTMLBitmap_IsEmptyBitmap(bmpBkg) ) {
      int x, y;
      CHTMLBitmap_GetBitmap(bmpBkg, FALSE);
      if ( CHTMLBitmap_Width(bmpBkg) > 0 && CHTMLBitmap_Height(bmpBkg) > 0 ) {
        for ( x = 0;x < r.right + nX;x += CHTMLBitmap_Width(bmpBkg) ) {
          for ( y = 0;y < r.bottom + nY;y += CHTMLBitmap_Height(bmpBkg) ) {
            CHTMLBitmap_BitBlt(bmpBkg, 
              hDC, r.left + x, r.top + y,
              CHTMLBitmap_Width(bmpBkg) < (*pppItems)[ i ] ->m_scBounds.cx - x ? CHTMLBitmap_Width(bmpBkg) : (*pppItems)[ i ] ->m_scBounds.cx - x,
              CHTMLBitmap_Height(bmpBkg) < (*pppItems)[ i ] ->m_scBounds.cy - y ? CHTMLBitmap_Height(bmpBkg) : (*pppItems)[ i ] ->m_scBounds.cy - y );
          }
        }
      }
    }
    if ( (*pppItems)[ i ] ->m_nType == TEXT && (*pppItems)[ i ] ->m_scBounds.cx ) {
      HFONT hf = CreateFont( (*pppItems)[ i ] ->m_nFontSize, 0, 0, 0, (*pppItems)[ i ] ->m_nFontBold ? FW_BOLD : FW_NORMAL, (*pppItems)[ i ] ->m_nFontItalic, (*pppItems)[ i ] ->m_nFrames == 8 ? 1 : 0, (*pppItems)[ i ] ->m_nFrames & 16 ? 1 : 0, !stricmp( (*pppItems)[ i ] ->m_szFont, "Wingdings" ) ? SYMBOL_CHARSET : 0, 0, 0, 0, 0, (*pppItems)[ i ] ->m_szFont );
      HFONT hfOld = ( HFONT ) SelectObject( hDC, hf );
      SetTextColor( hDC, (*pppItems)[ i ] ->m_bSelected ? RGB( 255, 255, 255 ) : (*pppItems)[ i ] ->m_colText );
      if ( me->m_nActiveLink >= 0 && me->m_nActiveLink == (*pppItems)[ i ] ->m_nLink )
        SetTextColor( hDC, RGB( 255, 0, 0 ) );
      //   ExtTextOut(hDC,r.left+((*pppItems)[i]->m_nFrames & 1)*(*pppItems)[i]->m_nFrameSize,r.top+((*pppItems)[i]->m_nFrames & 2)*(*pppItems)[i]->m_nFrameSize,ETO_CLIPPED,&r,(*pppItems)[i]->m_szText,strlen((*pppItems)[i]->m_szText),NULL);
      TextOut( hDC, r.left + ( (*pppItems)[ i ] ->m_nFrames & 1 ) * (*pppItems)[ i ] ->m_nFrameSize,
               r.top + ( (*pppItems)[ i ] ->m_nFrames & 2 ) * (*pppItems)[ i ] ->m_nFrameSize, (*pppItems)[ i ] ->m_szText, strlen( (*pppItems)[ i ] ->m_szText ) );
      SelectObject( hDC, hfOld );
      DeleteObject( hf );
    } else if ( (*pppItems)[ i ] ->m_nType == IMAGE && (*pppItems)[ i ] ->m_nBmp != -1 ) {
      CHTMLBitmap* bmp = CHTMLBitmaps_GetAt(me->m_Images, (*pppItems)[ i ] ->m_nBmp);
      CHTMLBitmap_StretchBlt( bmp, hDC, r.left, r.top, (*pppItems)[ i ] ->m_scBounds.cx, (*pppItems)[ i ] ->m_scBounds.cy );
    } else if ( (*pppItems)[ i ] ->m_nType == COMPOUND ) {
      CHTMLViewer_Draw0( me, hDC, rClient, &(*pppItems)[ i ] ->m_ppSubItems, &(*pppItems)[ i ] ->m_nSubItems, nX, nY );
    }
    if ( !(*pppItems)[ i ] ->m_nFrames || ( (*pppItems)[ i ] ->m_nType == TEXT && ( (*pppItems)[ i ] ->m_nFrames == 8 || (*pppItems)[ i ] ->m_nFrames == 16 ) ) )
      continue;
    for ( n = 0;n < (*pppItems)[ i ] ->m_nFrameSize;n++ ) {
      HPEN pnOld;
      if ( (*pppItems)[ i ] ->m_colFrame == -1 )
        pn = CreatePen( PS_SOLID, 1, RGB( 128, 128, 128 ) );
      else if ( (*pppItems)[ i ] ->m_colFrame == -2 )
        pn = CreatePen( PS_SOLID, 1, RGB( 255, 255, 255 ) );
      else {
        if ( me->m_nActiveLink >= 0 && me->m_nActiveLink == (*pppItems)[ i ] ->m_nLink )
          pn = CreatePen( PS_SOLID, 1, RGB( 255, 0, 0 ) );
        else
          pn = CreatePen( PS_SOLID, 1, (*pppItems)[ i ] ->m_colFrame );
      }
      pnOld = ( HPEN ) SelectObject( hDC, pn );
      if ( (*pppItems)[ i ] ->m_nFrames & 1 ) {
        MoveToEx( hDC, r.left, r.top, NULL );
        LineTo( hDC, r.left, r.bottom - 1 );
      }
      if ( (*pppItems)[ i ] ->m_nFrames & 2 ) {
        MoveToEx( hDC, r.left, r.top, NULL );
        LineTo( hDC, r.right, r.top );
      }
      if ( (*pppItems)[ i ] ->m_colFrame == -1 || (*pppItems)[ i ] ->m_colFrame == -2 ) {
        SelectObject( hDC, pnOld );
        DeleteObject( pn );
        if ( (*pppItems)[ i ] ->m_colFrame == -1 )
          pn = CreatePen( PS_SOLID, 1, RGB( 255, 255, 255 ) );
        else
          pn = CreatePen( PS_SOLID, 1, RGB( 128, 128, 128 ) );
        pnOld = ( HPEN ) SelectObject( hDC, pn );
      }
      if ( (*pppItems)[ i ] ->m_nFrames & 4 ) {
        MoveToEx( hDC, r.right - 1, r.top, NULL );
        LineTo( hDC, r.right - 1, r.bottom - 1 );
      }
      if ( (*pppItems)[ i ] ->m_nFrames & 8 ) {
        MoveToEx( hDC, r.left, r.bottom - 1, NULL );
        LineTo( hDC, r.right, r.bottom - 1 );
      }
      if ( (*pppItems)[ i ] ->m_nFrames & 16 ) {
        MoveToEx( hDC, r.left, r.bottom - 1 - ( r.bottom - r.top ) / 2, NULL );
        LineTo( hDC, r.right, r.bottom - 1 - ( r.bottom - r.top ) / 2 );
      }
      r.left++;
      r.top++;
      r.right--;
      r.bottom--;
      SelectObject( hDC, pnOld );
      DeleteObject( pn );
    }
  }
}
void CHTMLViewer_Draw( CHTMLViewer* me, HDC hDC, const RECT r, int nX, int nY ) {
  CHTMLBitmap* bmpBkg=NULL;
  if ( me->m_bgCol != -1 ) {
    HBRUSH br = CreateSolidBrush( me->m_bgCol );
    FillRect( hDC, &r, br );
    DeleteObject( br );
  }
  if ( me->m_nBmpBkg != -1 && (bmpBkg=CHTMLBitmaps_GetAt(me->m_Images, me->m_nBmpBkg )) ) {
    if (CHTMLBitmap_GetBitmap(bmpBkg, FALSE) && CHTMLBitmap_GetBackground(bmpBkg) && !CHTMLBitmap_IsEmptyBitmap(bmpBkg) ) {
      int x = -nX, y;
      for ( ;x + CHTMLBitmap_Width(bmpBkg) < 0;x += CHTMLBitmap_Width(bmpBkg) )
        ;
      for ( ;x < r.right;x += CHTMLBitmap_Width(bmpBkg) ) {
        y = -nY;
        for ( ;y + CHTMLBitmap_Height(bmpBkg) < 0;y += CHTMLBitmap_Height(bmpBkg) )
          ;
        for ( ;y < r.bottom;y +=  CHTMLBitmap_Height(bmpBkg) )
          CHTMLBitmap_BitBlt( bmpBkg, hDC, r.left + x, r.top + y, CHTMLBitmap_Width(bmpBkg), CHTMLBitmap_Height(bmpBkg) );
      }
    }
  }
  CHTMLViewer_Draw0( me, hDC, r, &(me->m_ppItems), &(me->m_nItems), nX, nY );
}
void CHTMLViewer_Statistics( CHTMLViewer* me, int* pItems, int* pImages, int* pLinks, int* pLevel ) {
  CHTMLStringVector* sv = CHTMLStringTable_GetAt(me->m_Links, 0);
  *pItems = me->m_nItemCount;
  *pImages = CHTMLBitmaps_GetCount(me->m_Images);
  *pLinks = CHTMLStringVector_GetCount(sv);
  *pLevel = me->m_nLevelCount;
}
void CHTMLViewer_Select1( CHTMLViewer* me, RECT* r, CHTMLItem*** pppItems, int* pItems ) {
#if 0
  int i;
  for ( i = 0;i < (*pItems);i++ ) {
    if ( ( (*pppItems)[ i ] ->m_nType == TEXT || (*pppItems)[ i ] ->m_nType == BREAK ) && !( i < (*pItems) - 1 && (*pppItems)[ i ] ->m_nType == BREAK && (*pppItems)[ i + 1 ] ->m_nType == COMPOUND ) ) {
      if ( r.top < (*pppItems)[ i ] ->m_pPos.y && r.bottom >= (*pppItems)[ i ] ->m_pPos.y && r.right > (*pppItems)[ i ] ->m_pPos.x )
        (*pppItems)[ i ] ->m_bSelected = TRUE;
      else if ( r.top < (*pppItems)[ i ] ->m_pPos.y + (*pppItems)[ i ] ->m_scBounds.cy && r.bottom >= (*pppItems)[ i ] ->m_pPos.y + (*pppItems)[ i ] ->m_scBounds.cy && r.left < (*pppItems)[ i ] ->m_pPos.x )
        (*pppItems)[ i ] ->m_bSelected = TRUE;
      else if ( (*pppItems)[ i ] ->m_pPos.x > r.right || (*pppItems)[ i ] ->m_pPos.x + (*pppItems)[ i ] ->m_scBounds.cx < r.left ||
                (*pppItems)[ i ] ->m_pPos.y > r.bottom || (*pppItems)[ i ] ->m_pPos.y + (*pppItems)[ i ] ->m_scBounds.cy < r.top )
        (*pppItems)[ i ] ->m_bSelected = FALSE;
      else
        (*pppItems)[ i ] ->m_bSelected = TRUE;
    }
    if ( (*pppItems)[ i ] ->m_nSubItems )
      CHTMLViewer_Select1( me, r, (*pppItems)[ i ] ->m_ppSubItems, (*pppItems)[ i ] ->m_nSubItems );
  }
#endif
}
void CHTMLViewer_Select( CHTMLViewer* me, RECT* r ) {
  CHTMLViewer_Select1( me, r, &me->m_ppItems, &me->m_nItems );
}
void CHTMLViewer_CopySelected1( CHTMLViewer* me, CHTMLItem*** pppItems, int* pItems, char **szCopy, int* pLen ) {
#if 0
  int i;
  for ( i = 0;i < (*pItems);i++ ) {
    if ( (*pppItems)[ i ] ->m_bSelected ) {
      if ( (*pppItems)[ i ] ->m_nType == TEXT ) {
        int nNewLen = nLen + strlen( (*pppItems)[ i ] ->m_szText );
        szCopy = ( char* ) realloc( szCopy, nNewLen + 1 );
        strcpy( &szCopy[ nLen ], (*pppItems)[ i ] ->m_szText );
        nLen = nNewLen;
      } else if ( (*pppItems)[ i ] ->m_nType == BREAK ) {
        int nNewLen = nLen + 2;
        szCopy = ( char* ) realloc( szCopy, nNewLen + 1 );
        strcpy( &szCopy[ nLen ], "\r\n" );
        nLen = nNewLen;
      }
    }
    int nPLen = nLen;
    if ( (*pppItems)[ i ] ->m_nSubItems )
      CopySelected( (*pppItems)[ i ] ->m_ppSubItems, (*pppItems)[ i ] ->m_nSubItems, szCopy, nLen );
    if ( nLen > nPLen ) {
      int nNewLen = nLen + 1;
      szCopy = ( char* ) realloc( szCopy, nNewLen + 1 );
      strcpy( &szCopy[ nLen ], " " );
      nLen = nNewLen;
    }
  }
#endif
}
void CHTMLViewer_CopySelected(CHTMLViewer* me) {
  char * szCopy = NULL;
  int nLen = 0;
  CHTMLViewer_CopySelected1( me, &me->m_ppItems, &me->m_nItems, &szCopy, &nLen );
  if ( !nLen )
    return ;
  {
    HANDLE hTemp = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, nLen + 1 );
    char *szTemp = ( char* ) GlobalLock( hTemp );
    strcpy( szTemp, szCopy );
    GlobalUnlock( hTemp );
    OpenClipboard( NULL );
    EmptyClipboard();
    SetClipboardData( CF_TEXT, hTemp );
    CloseClipboard();
    free( szCopy );
  }
}
void CHTMLViewer_UpdateImage1( CHTMLViewer* me, CHTMLItem*** pppItems, int* pItems, int nImage ) {
#if 0
  int i;
  if ( !me->m_pProgress )
    return ;
  for ( i = 0;i < (*pItems);i++ ) {
    if ( (*pppItems)[ i ] ->m_nType == IMAGE && (*pppItems)[ i ] ->m_nBmp == nImage ) {
      RECT r = {(*pppItems)[ i ] ->m_pPos.x, (*pppItems)[ i ] ->m_pPos.y, (*pppItems)[ i ] ->m_scBounds.cx + (*pppItems)[ i ] ->m_pPos.x, (*pppItems)[ i ] ->m_scBounds.cy + (*pppItems)[ i ] ->m_pPos.y};
      me->m_pProgress->UpdateScreen( r );
    } else if ( (*pppItems)[ i ] ->m_nType == COMPOUND )
      UpdateImage( (*pppItems)[ i ] ->m_ppSubItems, (*pppItems)[ i ] ->m_nSubItems, nImage );
  }
#endif
}
void CHTMLViewer_UpdateImage( CHTMLViewer* me, int nImage ) {
  //UpdateImage( me->m_ppItems, me->m_nItems, nImage );
}
void CHTMLViewer_UpdateLink1( CHTMLViewer* me, CHTMLItem*** pppItems, int* pItems, int nLink ) {
#if 0
  if ( !me->m_pProgress )
    return ;
  int i;
  for ( i = 0;i < (*pItems);i++ ) {
    if ( (*pppItems)[ i ] ->m_nLink == nLink ) {
      RECT r = {(*pppItems)[ i ] ->m_pPos.x, (*pppItems)[ i ] ->m_pPos.y, (*pppItems)[ i ] ->m_scBounds.cx + (*pppItems)[ i ] ->m_pPos.x, (*pppItems)[ i ] ->m_scBounds.cy + (*pppItems)[ i ] ->m_pPos.y};
      me->m_pProgress->UpdateScreen( r );
    } else if ( (*pppItems)[ i ] ->m_nType == COMPOUND )
      UpdateLink( (*pppItems)[ i ] ->m_ppSubItems, (*pppItems)[ i ] ->m_nSubItems, nLink );
  }
#endif
}
void CHTMLViewer_UpdateLink( CHTMLViewer* me, char *szLink ) {
#if 0
  int i;
  for ( i = 0;i < CHTMLStringVector_GetCount(sv);i++ ) {
    if ( !strcmp( me->m_Links[ 0 ][ i ], szLink ) ) {
      UpdateLink( me->m_ppItems, me->m_nItems, i );
      break;
    }
  }
#endif
}
BOOL CHTMLViewer_IsReading(CHTMLViewer* me) {
  return me->m_bReading;
}

#if 0
int CHTMLViewer_OnMouseOver( CHTMLItem*** pppItems, int* pItems, int nX, int nY, BOOL bClick ) {
  int i;
  for ( i = 0;i < (*pItems);i++ ) {
    if ( (*pppItems)[ i ] ->m_nType == COMPOUND && OnMouseOver( (*pppItems)[ i ] ->m_ppSubItems, (*pppItems)[ i ] ->m_nSubItems, nX, nY, bClick ) )
      return 1;
    if ( (*pppItems)[ i ] ->m_nLink == -1 )
      continue;
    RECT r = {(*pppItems)[ i ] ->m_pPos.x, (*pppItems)[ i ] ->m_pPos.y, (*pppItems)[ i ] ->m_pPos.x + (*pppItems)[ i ] ->m_scBounds.cx, (*pppItems)[ i ] ->m_pPos.y + (*pppItems)[ i ] ->m_scBounds.cy};
    POINT p = {nX, nY};
    if ( PtInRect( &r, p ) ) {
      me->m_nActiveLink = (*pppItems)[ i ] ->m_nLink;
      if ( bClick )
        me->m_pProgress->LinkClicked( me->m_Links[ 0 ][ (*pppItems)[ i ] ->m_nLink ] );
      else
        me->m_pProgress->SetCursor( TRUE, me->m_Links[ 0 ][ (*pppItems)[ i ] ->m_nLink ] );
      return 1;
    }
  }
  me->m_nActiveLink = -1;
  return 0;
}
void CHTMLViewer_OnMouseOver( int x, int y ) {
  if ( !me->m_pProgress )
    return ;
  if ( !OnMouseOver( me->m_ppItems, me->m_nItems, x, y, 0 ) )
    me->m_pProgress->SetCursor( FALSE, "" );
}
void CHTMLViewer_OnMouseClick( int x, int y ) {
  if ( !me->m_pProgress )
    return ;
  OnMouseOver( me->m_ppItems, me->m_nItems, x, y, 1 );
}

#endif
