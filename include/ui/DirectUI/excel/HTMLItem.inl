
CHTMLItem* CreateCHTMLItem() {
  CHTMLItem* me = (CHTMLItem*)malloc(sizeof(CHTMLItem));
  me->m_nType = -1;
  me->m_scBounds.cx = me->m_scBounds.cy = 0;
  me->m_pPos.x = me->m_pPos.y = 0;
  me->m_nFrames = 0;
  me->m_nFrameSize = 0;
  me->m_colFrame = 0;
  *me->m_szFont = 0;
  me->m_nFontSize = 0;
  me->m_nFontBold = 0;
  me->m_nFontItalic = 0;
  me->m_colText = 0;
  me->m_colBkg = -1;
  me->m_nHFormat = 0;
  me->m_nVFormat = 0;
  me->m_szText = NULL;
  me->m_nBmp = -1;
  me->m_nBmpBkg = -1;
  me->m_ppSubItems = NULL;
  me->m_nSubItems = 0;
  me->m_nRow = 0;
  me->m_nCol = 0;
  me->m_nWidth = 0;
  me->m_nColSpan = 0;
  me->m_nRowSpan = 0;
  me->m_nCellPadding = 0;
  me->m_nCellSpacing = 0;
  me->m_nLink = -1;
  me->m_nTableWidth = 0;
  me->m_nTableFrameSize = 0;
  me->m_hEvent = NULL;
  me->m_nHeight = 0;
  me->m_nTableHeight = 0;
  me->m_nMinColumnWidth = -1;
  me->m_nMaxColumnWidth = -1;
  me->m_nAMinColumnWidth = -1;
  me->m_nColumnWidth = -1;
  me->m_bSelected = FALSE;
  me->m_bNoBreak = FALSE;
  me->m_nControlID = -1;
  return me;
}
void CHTMLItem_Clear(CHTMLItem* me) {
  int i;
  for ( i = 0;i < me->m_nSubItems;i++ )
    free(me->m_ppSubItems[ i ]);
  free( me->m_ppSubItems );
  me->m_nSubItems = 0;
  me->m_ppSubItems = NULL;
  if ( me->m_szText )
    free(me->m_szText);
}

void DeleteCHTMLItem(CHTMLItem* me) {
  CHTMLItem_Clear(me);
  free(me);
}

void CHTMLItem_GetStringSize( char *szFont, int nFontSize, int bBold, int bItalic, SIZE *sc, char *szString, int nMaxWidth, int nLen ) {
  HDC hDC = GetDC( NULL );
  HFONT hf = CreateFont( nFontSize, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, 0, 0, !stricmp( szFont, "Wingdings" ) ? SYMBOL_CHARSET : 0, 0, 0, 0, 0, szFont );
  HFONT hfOld = ( HFONT ) SelectObject( hDC, hf );
  GetTextExtentPoint( hDC, szString, nLen == -1 ? strlen( szString ) : nLen, sc );
  // sc->cy+=2;
  SelectObject( hDC, hfOld );
  DeleteObject( hf );
  ReleaseDC( NULL, hDC );
}
void CHTMLItem_Offset( CHTMLItem* me, int nX, int nY ) {
  int i;
  for ( i = 0;i < me->m_nSubItems;i++ ) {
    me->m_ppSubItems[ i ] ->m_pPos.x += nX;
    me->m_ppSubItems[ i ] ->m_pPos.y += nY;
    CHTMLItem_Offset( me->m_ppSubItems[ i ], nX, nY );
  }
}
int g_nTexts = 0;
// Text-object
// Represents texts
CHTMLItem *CHTMLItem_NewText( char *szText, int nFrames, int nFrameSize, COLORREF colFrame, char *szFont, int nFontSize,
                               int nFontBold, int nFontItalic, COLORREF colText, COLORREF colBkg, int nHFormat, int nVFormat, int nLink, BOOL bNoBreak ) {
  CHTMLItem * pRet = CreateCHTMLItem();
  pRet->m_nType = TEXT;
  pRet->m_nFrames = nFrames;
  pRet->m_nFrameSize = nFrameSize;
  pRet->m_colFrame = colFrame;
  strcpy( pRet->m_szFont, szFont );
  if ( strchr( pRet->m_szFont, ',' ) )
    * strchr( pRet->m_szFont, ',' ) = 0;
  pRet->m_nFontSize = nFontSize;
  pRet->m_nFontBold = nFontBold;
  pRet->m_nFontItalic = nFontItalic;
  pRet->m_colText = colText;
  pRet->m_colBkg = colBkg;
  pRet->m_nHFormat = nHFormat;
  pRet->m_nVFormat = nVFormat;
  pRet->m_szText = (char*)malloc( strlen( szText ) + 1 );
  pRet->m_nLink = nLink;
  pRet->m_bNoBreak = bNoBreak;
  strcpy( pRet->m_szText, szText );
  ConvertFromHTMLCodes( pRet->m_szText );
  ConvertFromUtf8Codes( pRet->m_szText );
  CHTMLItem_GetStringSize( pRet->m_szFont, nFontSize, nFontBold, nFontItalic, &pRet->m_scBounds, pRet->m_szText, 0, strlen( pRet->m_szText ) );
  g_nTexts++;
  return pRet;
}
CHTMLItem *CHTMLItem_NewImage( char *szImageName, int nFrames, int nFrameSize, COLORREF colFrame, int nHFormat,
                              CHTMLBitmaps* Images, int nLink, BOOL bNoBreak, BOOL bBackground, int nBorder ) {
  CHTMLItem * pRet = CreateCHTMLItem();
  int nImage;
  pRet->m_nType = IMAGE;
  pRet->m_nHFormat = nHFormat;
  pRet->m_nLink = nLink;
  for ( nImage = 0;nImage < CHTMLBitmaps_GetCount(Images);nImage++ )
    if ( !strcmp( szImageName, CHTMLBitmap_GetImageName( CHTMLBitmaps_GetAt(Images, nImage )) ) &&
      bBackground == CHTMLBitmap_GetBackground(CHTMLBitmaps_GetAt(Images, nImage )) )
      break;
  if ( nImage == CHTMLBitmaps_GetCount(Images) )
    CHTMLBitmaps_AddBitmap( Images, szImageName, bBackground );
  pRet->m_nBmp = nImage;
  pRet->m_bNoBreak = bNoBreak;
  if ( nBorder ) {
    pRet->m_nFrames = 15;
    pRet->m_nFrameSize = nBorder;
  }
  return pRet;
}
// Break-object
// Represents <BR> and <HR> and are also used for the outer frame of tables
CHTMLItem *CHTMLItem_NewBreak( int nHeight, int nWidth, int nHFormat, int bClear ) {
  CHTMLItem * pRet = CreateCHTMLItem();
  pRet->m_nType = BREAK;
  pRet->m_scBounds.cy = nHeight;
  pRet->m_scBounds.cx = nWidth;
  pRet->m_nHFormat = nHFormat;
  if ( nWidth )
    pRet->m_nFrames = 15;
  pRet->m_nFrameSize = 1;
  pRet->m_colFrame = -1;
  pRet->m_bClear = bClear;
  return pRet;
}
// COMPOUND-object
// Represents <TD> and <DIV> as a new HTML document
CHTMLItem *CHTMLItem_NewCompound( int nRow, int nCol, int nWidth, int nHFormat, int nVFormat, COLORREF colBkg, int bBorder, int nColSpan, int nRowSpan, int nCellPadding, int nCellSpacing, int nTableWidth, int nBmpBkg, int nHeight, int nTableHeight, COLORREF colBorder, int nAbsoluteAlign ) {
  CHTMLItem * pRet = CreateCHTMLItem();
  pRet->m_nType = COMPOUND;
  pRet->m_nFrames = ( bBorder ? 15 : 0 );
  pRet->m_colFrame = colBorder;
  pRet->m_nFrameSize = ( bBorder ? 1 : 0 );
  pRet->m_colBkg = colBkg;
  pRet->m_nRow = nRow;
  pRet->m_nCol = nCol;
  pRet->m_nWidth = nWidth;
  pRet->m_nColSpan = nColSpan;
  if ( pRet->m_nColSpan < 1 )
    pRet->m_nColSpan = 1;
  else if ( pRet->m_nColSpan > 1000 )
    pRet->m_nColSpan = 1000;
  pRet->m_nRowSpan = nRowSpan;
  if ( pRet->m_nRowSpan < 1 )
    pRet->m_nRowSpan = 1;
  else if ( pRet->m_nRowSpan > 1000 )
    pRet->m_nRowSpan = 1000;
  pRet->m_nCellPadding = nCellPadding + ( bBorder ? 1 : 0 );
  pRet->m_nCellSpacing = nCellSpacing;
  pRet->m_nTableWidth = nTableWidth;
  pRet->m_nTableFrameSize = bBorder;
  pRet->m_nHFormat = nHFormat;
  pRet->m_nVFormat = nVFormat;
  pRet->m_nBmpBkg = nBmpBkg;
  pRet->m_nHeight = nHeight;
  pRet->m_nTableHeight = nTableHeight;
  pRet->m_nAbsoluteAlign = nAbsoluteAlign;
  return pRet;
}
BOOL CHTMLItem_WaitForImage( CHTMLItem* me, CHTMLBitmaps *Images, BOOL bWaitForImages ) {
  CHTMLBitmap_GetBitmap( CHTMLBitmaps_GetAt(Images, me->m_nBmp), bWaitForImages );
  CHTMLBitmap_GetSize( CHTMLBitmaps_GetAt(Images, me->m_nBmp), &me->m_scBounds );
  return TRUE;
}

