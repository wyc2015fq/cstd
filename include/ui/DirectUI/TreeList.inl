
#include "TreeList2.inl"
#include    <tchar.h>
#include    "TreeList.h"

#define UITreeList_SetPos CContainerUI_SetPos
#define UITreeList_Event CContainerUI_Event
#define UITreeList_SetAttribute CContainerUI_SetAttribute

int UITreeList_CtrFree(CControlUI* ctr) {
  UITreeList* pData = (UITreeList* )( ctr );
  unsigned uVal;
  LOCK( pData );

  if ( pData->hStates == THEMEIMGLIST )
    pData->hStates = 0;
  if ( pData->hChecks == THEMEIMGLIST )
    pData->hChecks = 0;

  if ( pData->hStates ) {
    ImageList_Destroy( pData->hStates );
    pData->hStates = 0;
  }

  if ( pData->uStyleEx & TVS_EX_SHAREIMAGELISTS ) {
    if ( pData->hStates && pData->iStatesMode ) {
      ImageList_Destroy( pData->hStates );
      pData->hStates = 0;
    }
    if ( pData->hChecks && pData->iChecksMode ) {
      ImageList_Destroy( pData->hChecks );
      pData->hChecks = 0;
    }
    pData->hImages = 0;
    pData->hSubImg = 0;
  } else {
    if ( pData->hStates ) {
      ImageList_Destroy( pData->hStates );
      pData->hStates = 0;
    }
    if ( pData->hChecks ) {
      ImageList_Destroy( pData->hChecks );
      pData->hChecks = 0;
    }
    if ( pData->hImages ) {
      ImageList_Destroy( pData->hImages );
      pData->hImages = 0;
    }
    if ( pData->hSubImg ) {
      ImageList_Destroy( pData->hSubImg );
      pData->hSubImg = 0;
    }
  }

  for ( uVal = 1; uVal < pData->uColumnCount; uVal++ ) {
    free( pData->pExtraItems[ uVal - 1 ]);
    pData->pExtraItems[ uVal - 1 ] = 0;
  }

  pData->uColumnCount = 0;

  UNLOCK( pData );

  CContainerUI_CtrFree(ctr);
  return 0;
}
static void GlobalInit() {
  LOGBRUSH sLog;
  long lCount;

  lCount = InterlockedIncrement( &lWindowCount );
  if ( lCount > 0 )
    return ;

  sLog.lbColor = GetSysColor( COLOR_BTNSHADOW );
  sLog.lbStyle = PS_SOLID;
  sLog.lbHatch = 0;
  hPatternPen = ExtCreatePen( PS_COSMETIC | PS_ALTERNATE, 1, &sLog, 0, NULL );

  if ( !hPatternPen ) {
    hPatternPen = CreatePen( PS_DOT, 1, RGB( 0, 0, 0 ) );
  }

  if ( pBufferedPtInit ) {
    pBufferedPtInit();
  }

}

static void GlobalDeinit() {
  int lCount;

  lCount = InterlockedDecrement( &lWindowCount );
  if ( lCount >= 0 )
    return ;

  if ( hDefaultFontN ) {
    DeleteObject( hDefaultFontN );
    hDefaultFontN = NULL;
  }

  if ( hDefaultFontB ) {
    DeleteObject( hDefaultFontB );
    hDefaultFontB = NULL;
  }

  if ( hPatternPen ) {
    DeleteObject( hPatternPen );
    hPatternPen = NULL;
  }

  if ( pBufferedPtExit ) {
    pBufferedPtExit();
  }

}


static int UpdateFont( UITreeList *pData ) {
  int iPos;
  int iRet;
  HDC hDc;
  LOGFONT sLog;
  SIZE sSize;
  TEXTMETRIC sMetrics;
  BaseItem *pEntry;
  BaseItem **pList;
  ExtraItem *pExtra;
  ExtraItem **pItems;
  unsigned uSub;

  if ( !hDefaultFontN )
  {
    SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( sLog ), &sLog, 0 );
    sLog.lfWeight = FW_NORMAL;
    hDefaultFontN = CreateFontIndirect( &sLog );
    sLog.lfWeight = FW_BOLD ;
    hDefaultFontB = CreateFontIndirect( &sLog );
  }


  if ( !pData->hFontN )
    pData->hFontN = hDefaultFontN;

  if ( pData->hFontN == hDefaultFontN )
  {
    pData->hFontB = hDefaultFontB;
  } else {
    pData->hFontB = pData->hFontN;
  }

  if ( pData->hFontN != pData->hFontL ) {
    pData->hFontL = pData->hFontN;

    hDc = GetDC( NULL );
    SelectObject( hDc, pData->hFontN );
    GetTextMetrics( hDc, &sMetrics );
    pData->iFontHeight = sMetrics.tmHeight;
    pData->iFontLine = sMetrics.tmAscent + 1;
    pData->iFontOff = ( sMetrics.tmPitchAndFamily & TMPF_FIXED_PITCH ) ? 0 : -1;
    ReleaseDC( NULL, hDc );

    pList = pData->pTreeItems;
    iPos = pData->uTreeItemsMax;

    for ( ; iPos >= 0; iPos-- )
    {
      pEntry = pList[ iPos ];
      if ( !pEntry )
        continue;

      pEntry->iTextPixels = 0;
    }


    for ( uSub = 1; uSub < pData->uColumnCount; uSub++ ) {
      iPos = pData->uTreeItemsMax;
      pItems = pData->pExtraItems[ uSub - 1 ];

      for ( ; iPos >= 0; iPos-- ) {
        pExtra = pItems[ iPos ];
        if ( !pExtra )
          continue;

        pExtra->iTextPixels = 0;
      }
    }

    iRet = 1;
  } else {
    iRet = 0;
  }

  hDc = GetDC( NULL );
  SelectObject( hDc, pData->hFontN );
  GetTextExtentExPoint( hDc, _T( "..." ), 3, 256, NULL, NULL, &sSize );
  pData->uTrippleN = sSize.cx;
  SelectObject( hDc, pData->hFontB );
  GetTextExtentExPoint( hDc, _T( "..." ), 3, 256, NULL, NULL, &sSize );
  pData->uTrippleB = sSize.cx;
  ReleaseDC( NULL, hDc );

  return iRet;
}

static int UpdateHeight( UITreeList *pData ) {
  int iHeight;

  if ( pData->cFixedHeight )
    return 0;

  iHeight = 10;

  if ( pData->hChecks )
    if ( iHeight < pData->iChecksYsize + 2 )
      iHeight = pData->iChecksYsize + 2;
  if ( pData->hStates )
    if ( iHeight < pData->iStatesYsize + 2 )
      iHeight = pData->iStatesYsize + 2;
  if ( iHeight < pData->iSubImgYsize + 2 )
    iHeight = pData->iSubImgYsize + 2;
  if ( iHeight < pData->iImagesYsize + 2 )
    iHeight = pData->iImagesYsize + 2;
  if ( iHeight < pData->iFontHeight + 2 )
    iHeight = pData->iFontHeight + 2;
  if ( pData->uStyleEx & TVS_EX_ITEMLINES )
    iHeight++;
  if ( pData->uStyle & TVS_NONEVENHEIGHT && ( iHeight & 1 ) )
    iHeight++;
  if ( pData->iRowHeight == iHeight )
    return 0;

  pData->iRowHeight = iHeight;

  if ( pData->uSizeY > pData->uStartPixel ) {
    pData->uMaxEnties = pData->uSizeY;
    pData->uMaxEnties -= pData->uStartPixel;
  } else {
    pData->uMaxEnties = 0;
  }

  pData->uPageEnties = pData->uMaxEnties;
  pData->uMaxEnties += pData->iRowHeight - 1;
  pData->uMaxEnties /= pData->iRowHeight;
  pData->uPageEnties /= pData->iRowHeight;

  return 1;
}

static void UpdateColors1( UITreeList *pData ) {
  unsigned uColOdd;
  unsigned uColor;
  int iDiff;
  int iSum;

  if ( !pData->cColorChanged[ TVC_BK ] )
    pData->uColors[ TVC_BK ] = GetSysColor( COLOR_WINDOW );
  if ( !pData->cColorChanged[ TVC_BOX ] )
    pData->uColors[ TVC_BOX ] = GetSysColor( COLOR_BTNSHADOW );
  if ( !pData->cColorChanged[ TVC_EVEN ] )
    pData->uColors[ TVC_EVEN ] = GetSysColor( COLOR_WINDOW );
  if ( !pData->cColorChanged[ TVC_TEXT ] )
    pData->uColors[ TVC_TEXT ] = GetSysColor( COLOR_WINDOWTEXT );
  if ( !pData->cColorChanged[ TVC_LINE ] )
    pData->uColors[ TVC_LINE ] = GetSysColor( COLOR_WINDOWTEXT );
  if ( !pData->cColorChanged[ TVC_FRAME ] )
    pData->uColors[ TVC_FRAME ] = GetSysColor( COLOR_3DFACE );
  if ( !pData->cColorChanged[ TVC_TRACK ] )
    pData->uColors[ TVC_TRACK ] = GetSysColor( COLOR_WINDOWTEXT ) ^ RGB( 0, 0, 255 );
  if ( !pData->cColorChanged[ TVC_INSERT ] )
    pData->uColors[ TVC_INSERT ] = GetSysColor( COLOR_INFOBK );
  if ( !pData->cColorChanged[ TVC_ODD ] )
    pData->uColors[ TVC_ODD ] = GetSysColor( COLOR_INFOBK );
  if ( !pData->cColorChanged[ TVC_BOXBG ] )
    pData->uColors[ TVC_BOXBG ] = GetSysColor( COLOR_WINDOW );
  if ( !pData->cColorChanged[ TVC_COLBK ] )
    pData->uColors[ TVC_COLBK ] = GetSysColor( COLOR_WINDOW );
  if ( !pData->cColorChanged[ TVC_COLODD ] )
    pData->uColors[ TVC_COLODD ] = GetSysColor( COLOR_BTNSHADOW );
  if ( !pData->cColorChanged[ TVC_COLEVEN ] )
    pData->uColors[ TVC_COLEVEN ] = GetSysColor( COLOR_WINDOW );
  if ( !pData->cColorChanged[ TVC_GRAYED ] )
    pData->uColors[ TVC_GRAYED ] = GetSysColor( COLOR_SCROLLBAR );

  pData->cGlyphOk = 0;

  if ( !pData->cColorChanged[ TVC_GRAYED ] ) {
    pData->uColors[ TVC_GRAYED ] = ( GetSysColor( COLOR_SCROLLBAR ) & 0x00FEFEFE ) >> 1;
    pData->uColors[ TVC_GRAYED ] += ( GetSysColor( COLOR_WINDOW ) & 0x00FEFEFE ) >> 1;
  }

  if ( !pData->cColorChanged[ TVC_ODD ] ) {
    uColOdd = pData->uColors[ TVC_ODD ];
    iDiff = ( ( uColOdd ) & 0xFF ) - ( ( pData->uColors[ TVC_EVEN ] ) & 0xFF );
    iSum = iDiff * iDiff;
    iDiff = ( ( uColOdd >> 8 ) & 0xFF ) - ( ( pData->uColors[ TVC_EVEN ] >> 8 ) & 0xFF );
    iSum += iDiff * iDiff;
    iDiff = ( ( uColOdd >> 16 ) & 0xFF ) - ( ( pData->uColors[ TVC_EVEN ] >> 16 ) & 0xFF );
    iSum += iDiff * iDiff;

    if ( iSum < 64 )
    {
      uColOdd = pData->uColors[ TVC_EVEN ] & 0x0000FFFF;
      uColOdd |= ( ( pData->uColors[ TVC_EVEN ] & 0x00FF0000 ) - 0x00080000 ) & 0x00FF0000;
    }

    pData->uColors[ TVC_ODD ] = uColOdd;
  }

  if ( !pData->cColorChanged[ TVC_COLBK ] ) {
    uColor = GetSysColor( COLOR_WINDOW );
    if ( uColor & 0x00F00000 )
      uColor -= 0x00100000;
    if ( uColor & 0x0000F000 )
      uColor -= 0x00001000;
    if ( uColor & 0x000000F0 )
      uColor -= 0x00000010;

    pData->uColors[ TVC_COLBK ] = uColor;
  }

  if ( !pData->cColorChanged[ TVC_COLODD ] ) {
    uColor = pData->uColors[ TVC_ODD ];
    if ( uColor & 0x00F00000 )
      uColor -= 0x00100000;
    if ( uColor & 0x0000F000 )
      uColor -= 0x00001000;
    if ( uColor & 0x000000F0 )
      uColor -= 0x00000010;

    pData->uColors[ TVC_COLODD ] = uColor;
  }

  if ( !pData->cColorChanged[ TVC_COLEVEN ] ) {
    uColor = GetSysColor( COLOR_WINDOW );
    if ( uColor & 0x00F00000 )
      uColor -= 0x00100000;
    if ( uColor & 0x0000F000 )
      uColor -= 0x00001000;
    if ( uColor & 0x000000F0 )
      uColor -= 0x00000010;

    pData->uColors[ TVC_COLEVEN ] = uColor;
  }

  if ( !pData->cColorChanged[ TVC_MARKODD ] )
    pData->uColors[ TVC_MARKODD ] = ( ( pData->uColors[ TVC_ODD ] >> 3 ) & 0x1F1F1F ) * 7;
  if ( !pData->cColorChanged[ TVC_MARKODD ] )
    pData->uColors[ TVC_MARKODD ] += ( ( GetSysColor( COLOR_HIGHLIGHT ) >> 3 ) & 0x1F1F1F ) * 1;
  if ( !pData->cColorChanged[ TVC_MARKEVEN ] )
    pData->uColors[ TVC_MARKEVEN ] = ( ( pData->uColors[ TVC_EVEN ] >> 3 ) & 0x1F1F1F ) * 7;
  if ( !pData->cColorChanged[ TVC_MARKEVEN ] )
    pData->uColors[ TVC_MARKEVEN ] += ( ( GetSysColor( COLOR_HIGHLIGHT ) >> 3 ) & 0x1F1F1F ) * 1;
}

static void CreateStateImageList( UITreeList *pData, int iMode ) {
  BITMAPINFO sInfo;
  BYTE aMem[ 0x1000 ];
  HDC hDcSrc;
  HDC hDc;
  HBITMAP hBmp;
  HBITMAP hBmpNew;
  RECT sRect;
  int iBits;

  if ( iMode ) {
    if ( pData->hChecks && pData->hChecks != THEMEIMGLIST )
      return ;
  } else {
    if ( pData->hStates && pData->hStates != THEMEIMGLIST )
      return ;
  }

  hDcSrc = GetDC( NULL );
  hDc = CreateCompatibleDC( NULL );
  hBmp = CreateCompatibleBitmap( hDcSrc, 16 * 3, 16 );

  SelectObject( hDc, hBmp );
  SelectObject( hDc, GetStockObject( NULL_PEN ) );
  SetBkMode ( hDc, OPAQUE );
  SetBkColor ( hDc, GetSysColor( COLOR_WINDOW ) );
  SelectObject( hDc, GetSysColorBrush( COLOR_HIGHLIGHT ) );
  Rectangle ( hDc, -1, -1, 16 * 3 + 2, 16 + 2 );

  sRect.top = 8 - 6;
  sRect.bottom = 8 + 7;
  sRect.left = 16 * 1 + 8 - 7;
  sRect.right = 16 * 1 + 8 + 6;

  DrawFrameControl( hDc, &sRect, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT );

  sRect.left = 16 * 2 + 8 - 7;
  sRect.right = 16 * 2 + 8 + 6;

  DrawFrameControl( hDc, &sRect, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT );

  iBits = GetDeviceCaps( hDc, BITSPIXEL );

  sInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
  sInfo.bmiHeader.biWidth = 16 * 3;
  sInfo.bmiHeader.biHeight = 16;
  sInfo.bmiHeader.biPlanes = 1;
  sInfo.bmiHeader.biBitCount = ( WORD ) iBits;
  sInfo.bmiHeader.biCompression = BI_RGB;
  sInfo.bmiHeader.biSizeImage = 0;
  sInfo.bmiHeader.biXPelsPerMeter = 0;
  sInfo.bmiHeader.biYPelsPerMeter = 0;
  sInfo.bmiHeader.biClrUsed = ( iBits > 8 ) ? 0 : 1 << iBits;
  
  sInfo.bmiHeader.biClrImportant = ( iBits > 8 ) ? 0 : 1 << iBits;
  
  GetDIBits( hDc, hBmp, 0, 0 , NULL, &sInfo, ( iBits > 8 ) ? DIB_RGB_COLORS : DIB_PAL_COLORS );
  GetDIBits( hDc, hBmp, 0, 16, aMem, &sInfo, ( iBits > 8 ) ? DIB_RGB_COLORS : DIB_PAL_COLORS );

  hBmpNew = CreateCompatibleBitmap( hDc, 16 * 3, 16 );

  SetDIBits( hDc, hBmpNew, 0, 16, aMem, &sInfo, ( iBits > 8 ) ? DIB_RGB_COLORS : DIB_PAL_COLORS );

  if ( iMode == 0 ) {
    pData->hStates = ImageList_Create( 16, 16, ILC_COLORDDB | ILC_MASK, 3, 14 );
    pData->iStatesXsize = 16;
    pData->iStatesYsize = 16;
    pData->iStatesMode = 1;

    ImageList_AddMasked( pData->hStates, hBmpNew, GetSysColor( COLOR_HIGHLIGHT ) );
  } else {
    pData->hChecks = ImageList_Create( 16, 16, ILC_COLORDDB | ILC_MASK, 3, 14 );
    pData->iChecksXsize = 16;
    pData->iChecksYsize = 16;
    pData->iChecksMode = 1;

    ImageList_AddMasked( pData->hChecks, hBmpNew, GetSysColor( COLOR_HIGHLIGHT ) );
  }


  DeleteObject( hBmpNew );
  DeleteObject( hBmp );
  DeleteDC( hDc );
  ReleaseDC( NULL, hDcSrc );


}

int UITreeList_CtrInit(CControlUI* ctr) {
  UITreeList* pData = (UITreeList*) ctr;
  CContainerUI_CtrInit(ctr);

  if ( !pData )
    return -1;

  pData->pTreeItems = NEW( BaseItem * ,1 );

  if ( !pData->pTreeItems ) {
    return -1;
  }

  pData->pItemPos = NEW(unsigned, 1 );
  if ( !pData->pItemPos ) {
    DEL(pData->pTreeItems);
    DEL( pData );
    return -1;
  }

  GlobalInit();

  pData->pItemPos [ 0 ] = 0;
  pData->pTreeItems[ 0 ] = NULL;

  pData->iIndent = DEFAULT_IDENT;
  pData->iShift = DEFAULT_SHIFT;
  pData->uStyle = 0;

  pData->cIsEnabled = ( char ) 1;
  pData->iAutoAdd = 1;

  if ( !( pData->uStyle & ( TVS_HASBUTTONS | TVS_HASLINES ) ) ) {
    pData->cHasRootRow = 0;
  } else {
    pData->cHasRootRow = ( char ) ( ( pData->uStyle & TVS_LINESATROOT ) ? 1 : 0 );
  }

  if ( !( pData->uStyle & TVS_NOTOOLTIPS ) ) {
  }

  UpdateFont ( pData );
  UpdateHeight ( pData );
  UpdateColors1 ( pData );

  pData->cGlyphOk = ( char ) (  0 );

  if ( pData->uStyle & TVS_CHECKBOXES ) {
    CreateStateImageList( pData, 0 );
  }

  return 0;
}
int UITreeList_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDc, RECT rcPaint ) {
  COLORREF uEcColor,uEvColor,uBkColor,uBtColor,uOdColor,uOcColor,uFrColor,uInColor,uOldColor,uOutColor,uTempColor;
  unsigned uTextSize, uRgnCount, uAutoMask, uStyleEx, uColMark, uState, uStyle, uExtra, uMark, uItem, uBits, uPos, uMax;
  int iXscroll, iHeight, iIndent, iDelta, iImage, iShift, iStart, iCount, iLevel, iLast, iSize, iXpos, iYpos, iMaxX, iAdd, i;
  HRGN hRgnMain;
  HRGN hRgn[ MAX_COLUMNS + 1 ];
  SIZE sSize;
  RECT sRect, sArea, sButton;
  UITreeList *pData;
  BaseItem *pTemp;
  BaseItem *pEntry;
  ExtraItem *pExtra;
  LPCTSTR pText;
  HIMAGELIST hImgList;
  int *pOffsets;
  int iRnType[ MAX_COLUMNS + 1 ];

  pData = (UITreeList*)( ctr );

  LOCK( pData );

  GetClientRect( sRect );

  iXscroll = -( int ) pData->uScrollX;
  pOffsets = pData->aColumnXpos;
  hRgnMain = CreateRectRgn( sRect.left, sRect.top, sRect.right, sRect.bottom );

  uMax = pData->uColumnCount;
  if ( !uMax ) {
    hRgn [ 0 ] = CreateRectRgn( sRect.left, sRect.top, sRect.right, sRect.bottom );
    iRnType[ 0 ] = CombineRgn( hRgn[ 0 ], hRgn[ 0 ], hRgnMain, RGN_AND );
    uRgnCount = 1;
  } else {
    for ( uPos = 0; uPos < uMax; uPos++ ) {
      hRgn [ uPos ] = CreateRectRgn( sRect.left + pOffsets[ uPos ] + iXscroll, sRect.top, sRect.left + pOffsets[ uPos + 1 ] + iXscroll, sRect.bottom );
      iRnType[ uPos ] = CombineRgn( hRgn[ uPos ], hRgn[ uPos ], hRgnMain, RGN_AND );
    }

    hRgn [ uPos ] = CreateRectRgn( sRect.left + pOffsets[ uPos ] + iXscroll, sRect.top, sRect.right, sRect.bottom );
    iRnType[ uPos ] = CombineRgn( hRgn[ uPos ], hRgn[ uPos ], hRgnMain, RGN_AND );

    uRgnCount = uMax + 1;
  }

  iHeight = pData->iRowHeight;
  uStyleEx = pData->uStyleEx;
  uStyle = pData->uStyle;
  iIndent = pData->iIndent;
  iShift = pData->iShift;
  uPos = pData->uScrollY;
  uMax = pData->uMaxEnties + uPos;

  if ( iRnType[ 0 ] == NULLREGION )
    iMaxX = pData->iMaxSizeX;
  else
    iMaxX = pData->aColumnXpos[ 1 ] - 1;

  if ( uStyleEx & TVS_EX_ITEMLINES ) {
    iHeight--;
  }

  if ( uStyleEx & TVS_EX_AUTOEXPANDICON ) {
    uAutoMask = TVIS_EXPANDED;
  } else {
    uAutoMask = 0;
  }

  if ( uMax > pData->uItemPosCount ) {
    uMax = pData->uItemPosCount;
  }


  uBkColor = pData->uColors[ TVC_BK ];
  uFrColor = pData->uColors[ TVC_FRAME ];


  if ( pData->uStyleEx & TVS_EX_ALTERNATECOLOR )
  {
    uOdColor = pData->uColors[ TVC_ODD ];
    uEvColor = pData->uColors[ TVC_EVEN ];
    uOcColor = pData->uColors[ TVC_COLODD ];
    uEcColor = pData->uColors[ TVC_COLEVEN ];
  } else {
    uOdColor = uBkColor;
    uEvColor = uBkColor;
    uOcColor = pData->uColors[ TVC_COLBK ];
    uEcColor = pData->uColors[ TVC_COLBK ];
  }

  if ( !pData->cIsEnabled ) {
    if ( pData->uStyleEx & TVS_EX_GRAYEDDISABLE ) {
      uBkColor = pData->uColors[ TVC_GRAYED ];
      uEvColor = uEcColor;
      uOdColor = uOcColor;
    }
  }

  uInColor = pData->uColors[ TVC_LINE ];
  uBtColor = pData->uColors[ TVC_BOX ];
  iStart = 0;
  iLast = 0;

  sArea.top = sRect.top + pData->uStartPixel;
  SelectObject( hDc, pData->hFontN );
  SelectObject( hDc, hPatternPen );
  SetBkColor ( hDc, uBkColor );
  SetBkMode ( hDc, TRANSPARENT );
  SetTextAlign( hDc, TA_LEFT | TA_TOP );
  SetTextColor( hDc, pData->uColors[ TVC_TEXT ] );

  for ( ; uPos < uMax; uPos++ )
  {
    uItem = pData->pItemPos[ uPos ];

    pEntry = pData->pTreeItems[ uItem ];
    if ( !pEntry )
      break;

    if ( ( pEntry->uState & TVIS_SELECTED ) && ( uStyleEx & TVS_EX_FULLROWMARK ) ) {
      if ( uStyleEx & TVS_EX_ALTERNATECOLOR )
        uOutColor = ( uPos & 1 ) ? pData->uColors[ TVC_MARKODD ] : pData->uColors[ TVC_MARKEVEN ];
      else
        uOutColor = pData->uColors[ TVC_MARK ];

      uMark = ( unsigned ) ~TVIS_BKCOLOR;
      uColMark = 0;
    } else if ( uPos & 1 )
    {
      uColMark = pData->aColumn[ 0 ].bMark;
      uOutColor = ( uColMark ) ? uOcColor : uOdColor;
      uMark = 0xFFFFFFFF;
    } else {
      uColMark = pData->aColumn[ 0 ].bMark;
      uOutColor = ( uColMark ) ? uEcColor : uEvColor;
      uMark = 0xFFFFFFFF;
    }

    sArea.bottom = sArea.top + pData->iRowHeight;
    sArea.left = iXscroll;
    iLevel = pEntry->uLevel;

    if ( iRnType[ 0 ] == NULLREGION ) {
      goto ExtraDraw;
    }

    uBits = pEntry->uState & 0xFFFF;
    uBits |= pEntry->bFlags << 16;
    uBits &= uMark;
    iImage = ( uBits & LVIS_SELECTED ) ? pEntry->iSelectedImage : pEntry->iImage;
    pText = pEntry->pText;
    uTextSize = pEntry->uTextSize;

    if ( pData->uSelectedSub && uItem == pData->uSelectedItem ) {
      if ( pData->uSelectedCount <= 1 || !( pData->uStyleEx & TVS_EX_SUBSELECT ) ) {
        uBits &= ~TVIS_SELECTED;
      }
    }

    if ( pEntry->bCallback ) {
      CallbackEntry( pData, pEntry, uItem, pEntry->bCallback, &iImage, &uTextSize, &pText );

      pEntry = pData->pTreeItems[ uItem ];
      if ( !pEntry )
        break;
    }


    SelectObject( hDc, hRgn[ 0 ] );

    if ( ( uStyleEx & ( TVS_EX_ITEMLINES | TVS_EX_FULLROWITEMS ) ) == ( TVS_EX_ITEMLINES | TVS_EX_FULLROWITEMS ) ) {
      sButton.left = 0;
      sButton.right = iIndent * ( iLevel + 1 ) + 2;
      sButton.bottom = sArea.bottom + 1;
      sButton.top = sArea.bottom - 2;

      SetBkColor( hDc, uFrColor );
      ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sButton, NULL, 0, NULL );
    }

    if ( pData->aColumn[ 0 ].bMark )
    {
      uBkColor = pData->uColors[ TVC_COLBK ];
    }

    SetBkColor( hDc, ( uStyleEx & TVS_EX_FULLROWITEMS ) ? uOutColor : uBkColor );

    if ( pData->cHasRootRow )
      iLevel++;

    if ( iLevel <= 0 )
      goto NoRootLines;

    if ( uStyle & ( TVS_HASBUTTONS | TVS_HASLINES ) ) {
      iLevel--;
    }

    if ( uStyleEx & TVS_EX_FULLROWITEMS ) {
      sArea.bottom--;
      iAdd = 1;
    } else {
      iAdd = 0;
    }

    if ( uStyle & TVS_HASLINES ) {
      pTemp = pData->pTreeItems[ pEntry->uParent ];
      sArea.right = sArea.left + 1;
      ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );
      sArea.left += iIndent * iLevel + 1;


      for ( i = iLevel; i > 0; i-- )
      {
        sArea.right = sArea.left;
        sArea.left -= iIndent;

        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );

        iXpos = sArea.left + iShift;

        if ( pTemp ) {
          if ( pTemp->uNextItem )
          {
            MoveToEx( hDc, iXpos, sArea.top | 1, NULL );
            LineTo ( hDc, iXpos, sArea.bottom + iAdd );
          }

          pTemp = pData->pTreeItems[ pTemp->uParent ];
        }
      }

      sArea.left += iIndent * iLevel;
    } else {
      if ( iLevel > 0 ) {
        sArea.right = sArea.left + iIndent * iLevel;
        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );
        sArea.left += sArea.right;
      }
    }


    if ( uStyle & TVS_HASBUTTONS )
    {
      sArea.right = sArea.left + iIndent;
      iXpos = sArea.left + iShift;
      iYpos = sArea.top + pData->iRowHeight / 2;

      ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );

      if ( uStyle & TVS_HASLINES )
      {
        MoveToEx( hDc, iXpos, sArea.top | 1, NULL );

        if ( pEntry->uNextItem )
          LineTo( hDc, iXpos, sArea.bottom + iAdd );
        else
          LineTo( hDc, iXpos, iYpos + 1 );


        MoveToEx( hDc, iXpos + 1 + ( iYpos & 1 ), iYpos, NULL );
        LineTo ( hDc, sArea.right , iYpos );
      }

      if ( pEntry->bFlags & TVIX_HASBUTTON )
      {
        sButton.left = iXpos - 4;
        sButton.top = iYpos - 4;
        sButton.right = iXpos + 5;
        sButton.bottom = iYpos + 5;

        if ( pData->cGlyphOk )
        {
        } else {
          SetBkColor( hDc, uBtColor );
          ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sButton, NULL, 0, NULL );

          sButton.left += 1;
          sButton.top += 1;
          sButton.right -= 1;
          sButton.bottom -= 1;

          SetBkColor( hDc, pData->uColors[ TVC_BOXBG ] );
          ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sButton, NULL, 0, NULL );

          sButton.left = iXpos - 2;
          sButton.top = iYpos ;
          sButton.right = iXpos + 3;
          sButton.bottom = iYpos + 1;

          SetBkColor( hDc, uInColor );
          ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sButton, NULL, 0, NULL );


          if ( ( uBits ^ TVIS_EXPANDED ) & ( TVIS_EXPANDED | TVIS_EXPANDPARTIAL ) ) {
            sButton.left = iXpos ;
            sButton.top = iYpos - 2;
            sButton.right = iXpos + 1;
            sButton.bottom = iYpos + 3;
            ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sButton, NULL, 0, NULL );
          }
        }

        SetBkColor( hDc, uBkColor );
      }

      sArea.left += iIndent;
    } else if ( uStyle & TVS_HASLINES )
    {
      sArea.right = sArea.left + iIndent;
      ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );

      iYpos = sArea.top + pData->iRowHeight / 2;
      iXpos = sArea.left + iShift;
      MoveToEx( hDc, iXpos, sArea.top | 1, NULL );

      if ( pEntry->uNextItem )
        LineTo( hDc, iXpos, sArea.bottom );
      else
        LineTo( hDc, iXpos, iYpos + 1 );

      MoveToEx( hDc, iXpos + 1 + ( iYpos & 1 ), iYpos, NULL );
      LineTo ( hDc, sArea.right , iYpos );

      sArea.left += iIndent;
    }

    if ( uStyleEx & TVS_EX_FULLROWITEMS )
      sArea.bottom++;

NoRootLines:

    if ( uStyleEx & TVS_EX_ITEMLINES )
    {
      iAdd = 1;
      sArea.right = sArea.left + 1;

      if ( uStyleEx & TVS_EX_FULLROWITEMS ) {
        iStart = sArea.left;
        iAdd = 0;
      } else if ( iLevel >= 0 ) {
        SetBkColor( hDc, uFrColor );
        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );

        sArea.left++;
        sArea.bottom--;
        iStart = sArea.left;
      } else {
        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );

        sArea.left++;
        sArea.bottom--;
        iStart = sArea.left - 1;
      }
    }
    else {
      iAdd = 0;
    }

    SetBkColor ( hDc, ( uBits & TVIS_BKCOLOR ) ? pEntry->uColorBk : uOutColor );
    SelectObject( hDc, ( uBits & TVIS_BOLD ) ? pData->hFontB : pData->hFontN );

    if ( pData->hStates )
    {
      sArea.right = sArea.left + pData->iStatesXsize;
      iYpos = sArea.top + ( iHeight - pData->iStatesYsize ) / 2;
      i = ( uBits & LVIS_STATEIMAGEMASK ) >> 12;

      sArea.right += iAdd;
      ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );
      sArea.left += iAdd;


      if ( pData->hStates == THEMEIMGLIST )
      {
        if ( pData->uStyleEx & TVS_EX_BITCHECKBOX )
          i++;
#define pDrawThemeBackg(a, b, c, d, e, f) assert(0)
        if ( i >= 1 && i <= 2 ) {
          uState = ( i == 1 ) ? CBS_UNCHECKEDNORMAL : CBS_CHECKEDNORMAL;
          pDrawThemeBackg( pData->hThemeBt, hDc, BP_CHECKBOX, uState, &sArea, 0 );
        }
      }
      else {
        ImageList_Draw( pData->hStates, i, hDc, sArea.left, iYpos, ILD_TRANSPARENT );
      }

      sArea.left += pData->iStatesXsize;
      iAdd = 0;
    }

    if ( pData->hImages && iImage != TV_NOIMAGE )
    {
      if ( iImage >= 0x40000000 )
      {
        if ( pEntry->uState & uAutoMask ) {
          if ( pEntry->uFirstChild ) {
            iImage += 1;
          }
        }

        iImage -= 0x40000000;
        sArea.right = sArea.left + pData->iSubImgXsize;
        iYpos = sArea.top + ( iHeight - pData->iSubImgYsize ) / 2;
        pEntry->bFlags |= TVIX_HASIMAGE;

        sArea.right += iAdd;
        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );
        sArea.left += iAdd;
        ImageList_Draw( pData->hImages, iImage, hDc, sArea.left, iYpos, ILD_TRANSPARENT | ( uBits & ( TVIS_OVERLAYMASK | LVIS_CUT ) ) );

        sArea.left += pData->iSubImgXsize;
        iAdd = 0;
      } else {
        if ( pEntry->uState & uAutoMask )
          if ( pEntry->uFirstChild ) {
            iImage += pData->iAutoAdd;
          }

          sArea.right = sArea.left + pData->iImagesXsize;
          iYpos = sArea.top + ( iHeight - pData->iImagesYsize ) / 2;
          pEntry->bFlags |= TVIX_HASIMAGE;

          sArea.right += iAdd;
          ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );
          sArea.left += iAdd;
          ImageList_Draw( pData->hImages, iImage, hDc, sArea.left, iYpos, ILD_TRANSPARENT | ( uBits & ( TVIS_OVERLAYMASK | LVIS_CUT ) ) );

          sArea.left += pData->iImagesXsize;
          iAdd = 0;
      }
    }
    else {
      pEntry->bFlags &= ~TVIX_HASIMAGE;
    }

    sArea.right = pData->aColumnXpos[ 1 ];
    iYpos = sArea.top + ( iHeight - pData->iFontHeight ) / 2;


    if ( uBits & ( TVIS_SELECTED | TVIS_DROPHILITED | TVIS_UNTERLINE | TVIS_TRACKED | TVIS_TEXTCOLOR | TVIS_FOCUSED ) ) {
      TCHAR * pPtr = ( TCHAR* ) alloca( ( uTextSize + 4 ) * sizeof( TCHAR ) );
      INT *pPos = ( INT * ) alloca( ( uTextSize + 4 ) * sizeof( INT ) );

      ExtTextOut( hDc, 0, 0, ETO_OPAQUE | ETO_CLIPPED, &sArea, NULL, 0, NULL );

      sButton.top = iYpos;
      sButton.left = sArea.left + 4;
      sButton.right = sArea.right;
      sButton.bottom = iYpos + pData->iFontHeight + 2;

      if ( !uTextSize )
      {
        sButton.right -= 2;
        sButton.bottom--;
        pEntry->iTextPixels = 0;
      } else {
        if ( pData->uStyleEx & TVS_EX_FULLROWMARK ) {
          if ( pData->cHasFocus == 0 || uItem != pData->uSelectedItem || pData->uSelectedSub ) {
            if ( ( uBits & TVIS_SELECTED ) && !( uBits & ( TVIS_DROPHILITED | TVIS_FOCUSED ) ) ) {
              sButton.left -= 2;
            }
          }
        }

        DrawText( hDc, pText, uTextSize, &sButton, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT );
        pEntry->iTextPixels = sButton.right - sButton.left;
      }


      if ( sButton.left + pEntry->iTextPixels >= ( int ) ( sArea.right - pData->uScrollX ) ) {
        iSize = sArea.right - pData->uScrollX - sButton.left - 2;
        iSize -= ( uBits & TVIS_BOLD ) ? pData->uTrippleB : pData->uTrippleN;
        if ( iSize < 3 ) {
          iCount = 0;
        } else {
          GetTextExtentExPoint( hDc, pText, uTextSize, iSize, &iCount, pPos, &sSize );
        }

        memcpy( pPtr , pText, iCount * sizeof( TCHAR ) );
        memcpy( pPtr + iCount, _T( "..." ), 4 * sizeof( TCHAR ) );

        pText = pPtr;
        uTextSize = iCount + 3;
        sButton.right = sArea.right - 2;
      }


      if ( ( uBits & TVIS_SELECTED ) && pData->cHasFocus && uItem == pData->uSelectedItem && !pData->uSelectedSub ) {
        uTempColor = GetSysColor( COLOR_HIGHLIGHTTEXT );
        SelectObject( hDc, GetSysColorBrush( COLOR_HIGHLIGHT ) );
        Rectangle ( hDc, sButton.left - 2, sButton.top - 1, sButton.right + 2, sButton.bottom + 1 );
      } else {
        if ( uBits & TVIS_DROPHILITED ) {
          uTempColor = GetSysColor ( COLOR_HIGHLIGHTTEXT );
          SelectObject( hDc, GetSysColorBrush( COLOR_HIGHLIGHT ) );
          SelectObject( hDc, GetStockObject ( NULL_PEN ) );
          Rectangle ( hDc, sButton.left - 2, sButton.top - 1, sButton.right + 2, sButton.bottom + 1 );
        }
        else if ( uBits & TVIS_SELECTED ) {
          if ( pData->cHasFocus ) {
            uTempColor = GetSysColor ( COLOR_HIGHLIGHTTEXT );
            SelectObject( hDc, GetSysColorBrush( COLOR_HIGHLIGHT ) );
            SelectObject( hDc, GetStockObject ( NULL_PEN ) );
          } else {
            if ( uBits & TVIS_TEXTCOLOR )
              uTempColor = pEntry->uColorText;
            else
              uTempColor = pData ->uColors[ TVC_TEXT ];

            if ( pData->uStyleEx & TVS_EX_FULLROWMARK )
              SelectObject( hDc, GetStockObject( NULL_BRUSH ) );
            else
              SelectObject( hDc, GetSysColorBrush( COLOR_3DFACE ) );

            SelectObject( hDc, GetStockObject( NULL_PEN ) );
          }

          Rectangle( hDc, sButton.left - 2, sButton.top - 1, sButton.right + 2, sButton.bottom + 1 );
        } else {
          if ( uBits & TVIS_TRACKED )
            uTempColor = pData ->uColors[ TVC_TRACK ];
          else if ( uBits & TVIS_TEXTCOLOR )
            uTempColor = pEntry->uColorText;
          else
            uTempColor = pData ->uColors[ TVC_TEXT ];
          sButton.right--;
          sButton.left --;
        }

        SelectObject( hDc, hPatternPen );

        if ( uBits & TVIS_FOCUSED )
        {
          SelectObject( hDc, GetStockObject( NULL_BRUSH ) );
          Rectangle ( hDc, sButton.left - 2, sButton.top - 1, sButton.right + 2, sButton.bottom + 1 );
        }
      }

      SetTextColor( hDc, uTempColor );
      sButton.left += pData->iFontOff;
      DrawText( hDc, pText, uTextSize, &sButton, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX );


      if ( uBits & ( TVIS_UNTERLINE | TVIS_TRACKED ) ) {
        if ( pText && *pText ) {
          sButton.left -= pData->iFontOff;
          sButton.right -= pData->iFontOff + 1;
          sButton.top += pData->iFontLine;
          sButton.bottom = sButton.top + 1;
          uOldColor = SetBkColor( hDc, uTempColor );
          ExtTextOut( hDc, 0, 0, ETO_OPAQUE | ETO_CLIPPED, &sButton, NULL, 0, NULL );
          SetBkColor( hDc, uOldColor );
        }
      }
      SetTextColor( hDc, pData->uColors[ TVC_TEXT ] );
    } else {
      if ( !pEntry->iTextPixels && uTextSize ) {
        sButton.top = iYpos;
        sButton.left = sArea.left + 4;
        sButton.right = sArea.right;
        sButton.bottom = iYpos + pData->iFontHeight + 2;

        DrawText( hDc, pText, uTextSize, &sButton, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT );

        pEntry->iTextPixels = sButton.right - sButton.left;
      }


      if ( sArea.left + pEntry->iTextPixels >= ( int ) ( sArea.right - pData->uScrollX ) ) {
        INT * pPos = ( INT* ) alloca( uTextSize * sizeof( INT ) );

        iSize = sArea.right - sArea.left - pData->uScrollX;
        iSize -= ( uBits & TVIS_BOLD ) ? pData->uTrippleB : pData->uTrippleN;
        if ( iSize < 3 ) {
          iCount = 0;
        } else {
          GetTextExtentExPoint( hDc, pText, uTextSize, iSize, &iCount, pPos, &sSize );
        }

        if ( iCount > 0 )
        {
          sButton.right = sArea.right;
          sArea.right = sArea.left + 2 + pPos[ iCount - 1 ];

          ExtTextOut( hDc, sArea.left + 2, iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, pText, iCount, NULL );

          sArea.left = sArea.right;
          sArea.right = sButton.right;

          ExtTextOut( hDc, sArea.left , iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, _T( "..." ), 3, NULL );
        } else {
          ExtTextOut( hDc, sArea.left + 2, iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, _T( "..." ), 3, NULL );
        }
      } else {
        ExtTextOut( hDc, sArea.left + 2, iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, pText, uTextSize, NULL );
      }
    }

    i = sArea.left - iXscroll;
    i += pEntry->iTextPixels + 5;
    if ( i > iMaxX )
      iMaxX = i;

    if ( uStyleEx & TVS_EX_ITEMLINES )
    {
      SetBkColor( hDc, uFrColor );

      if ( iLast > iStart )
      {
        sArea.top--;
        sArea.bottom = sArea.top + 1;
        sArea.left = iStart - 1;
        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );
        sArea.top++;
      }

      iLast = iStart;
      sArea.top += iHeight;
      sArea.left = iStart;
      sArea.bottom = sArea.top + 1;

      ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );

      sArea.top -= iHeight;
    }



ExtraDraw:

    if ( pData->aColumn[ 0 ].bMark )
    {
      uBkColor = pData->uColors[ TVC_BK ];
    }

    for ( uExtra = 1; uExtra <= pData->uColumnCount; uExtra++ )
    {
      if ( uColMark != pData->aColumn[ uExtra ].bMark ) {
        if ( !( pEntry->uState & TVIS_SELECTED ) || !( uStyleEx & TVS_EX_FULLROWMARK ) ) {
          if ( uPos & 1 ) {
            uColMark = pData->aColumn[ uExtra ].bMark;
            uOutColor = ( uColMark ) ? uOcColor : uOdColor;
          } else {
            uColMark = pData->aColumn[ uExtra ].bMark;
            uOutColor = ( uColMark ) ? uEcColor : uEvColor;
          }
        }
      }

      GetRgnBox( hRgn[ uExtra ], &sButton );

      if ( iRnType[ uExtra ] == NULLREGION )
        continue;

      SelectObject( hDc, hRgn[ uExtra ] );

      sArea.left = pData->aColumnXpos[ uExtra ];
      sArea.left += iXscroll;


      if ( uStyleEx & TVS_EX_ITEMLINES )
      {
        SetBkColor( hDc, uFrColor );

        sArea.right = sArea.left + 1;
        sArea.bottom += 1;

        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );

        sArea.left += 1;
        sArea.top += iHeight;
        sArea.bottom = sArea.top + 1;
        sArea.right = pData->aColumnXpos[ uExtra + 1 ];
        if ( uExtra < pData->uColumnCount )
          sArea.right += iXscroll;

        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );

        sArea.top -= iHeight;
        sArea.bottom -= 1;
        iAdd = 1;
      }

      if ( sArea.left > ( int ) pData->uSizeX )
        break;

      sArea.right = pData->aColumnXpos[ uExtra + 1 ];


      if ( uExtra < pData->uColumnCount )
      {
        sArea.right += iXscroll;
        pExtra = pData->pExtraItems[ uExtra - 1 ][ uItem ];
      } else {
        pExtra = 0;
      }

      if ( !pExtra )
      {
        SetBkColor( hDc, uOutColor );
        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );
      } else {
        iSize = pData->iSubImgXsize;
        hImgList = pData->hSubImg;
        iImage = pExtra->iImage;
        pText = pExtra->pText;
        uTextSize = pExtra->uTextSize;
        uBits = ( pExtra->uState & 0xFFFF );
        uBits |= ( pExtra->bFlags << 16 );
        uBits |= pEntry->uState & TVIS_BASEFLAGS;
        uBits &= uMark;

        if ( uExtra != pData->uSelectedSub ) {
          uBits &= ~TVIS_SELECTED;
        }

        if ( pExtra->bCallback )
        {
          CallbackExtra( pData, pEntry, pExtra, uItem, uExtra, pExtra->bCallback, &iImage, &uTextSize, &pText );
          pExtra = pData->pExtraItems[ uExtra - 1 ][ uItem ];
          if ( !pExtra )
            break;
        }

        SetBkColor( hDc, ( uBits & TVIS_BKCOLOR ) ? pExtra->uColorBk : uOutColor );

        if ( pData->aColumn[ uExtra ].bEdit >= TVAX_CHECK )
        {
          hImgList = pData->hChecks;
          iSize = pData->iChecksXsize;
          iImage = ( pExtra->uState & TVIS_STATEIMAGEMASK ) >> 12;
          uBits &= ~TVIS_OVERLAYMASK;

          if ( iImage & 8 ) {
            if ( pData->aColumn[ uExtra ].bFlags & TVAE_STATEENABLE ) {
              iImage &= 7;
            }
          }
        }

        if ( hImgList && iImage > TV_NOIMAGE )
        {
          sArea.right = sArea.left + iSize + 2;
          iYpos = sArea.top + ( iHeight - iSize ) / 2;
          pExtra->bFlags |= TVIX_HASIMAGE;

          SelectObject( hDc, ( uBits & TVIS_BOLD ) ? pData->hFontB : pData->hFontN );
          ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sArea, NULL, 0, NULL );


          if ( hImgList == THEMEIMGLIST )
          {
            if ( pData->uStyleEx & TVS_EX_BITCHECKBOX )
              iImage++;

            if ( iImage == 1 || iImage == 2 ) {
              uState = ( iImage == 1 ) ? CBS_UNCHECKEDNORMAL : CBS_CHECKEDNORMAL;
              pDrawThemeBackg( pData->hThemeBt, hDc, BP_CHECKBOX, uState, &sArea, 0 );
            }
          }
          else {
            ImageList_Draw( hImgList, iImage, hDc, sArea.left + 1, iYpos, ILD_TRANSPARENT | ( uBits & ( TVIS_OVERLAYMASK | LVIS_CUT ) ) );
          }

          sArea.left += iSize + 1;
          sArea.right = pData->aColumnXpos[ uExtra + 1 ];
          sArea.right += iXscroll;
        } else {
          pExtra->bFlags &= ~TVIX_HASIMAGE;
        }

        iYpos = sArea.top + ( iHeight - pData->iFontHeight ) / 2;
        SelectObject( hDc, ( uBits & TVIS_BOLD ) ? pData->hFontB : pData->hFontN );

        if ( uBits & ( TVIS_SELECTED | TVIS_DROPHILITED | TVIS_UNTERLINE | TVIS_TRACKED | TVIS_TEXTCOLOR | TVIS_FOCUSED ) ) {
          TCHAR * pPtr = ( TCHAR* ) alloca( ( uTextSize + 4 ) * sizeof( TCHAR ) );
          INT *pPos = ( INT * ) alloca( ( uTextSize + 4 ) * sizeof( INT ) );

          ExtTextOut( hDc, 0, 0, ETO_OPAQUE | ETO_CLIPPED, &sArea, NULL, 0, NULL );

          sButton.top = iYpos;
          sButton.left = sArea.left + 4;
          sButton.right = sArea.right;
          sButton.bottom = iYpos + pData->iFontHeight + 2;

          if ( !uTextSize ) {
            sButton.left--;
            sButton.right -= 2;
            sButton.bottom--;
            pExtra->iTextPixels = 0;
          } else {
            if ( pData->uStyleEx & TVS_EX_FULLROWMARK ) {
              if ( pData->cHasFocus == 0 || uItem != pData->uSelectedItem ) {
                if ( ( uBits & TVIS_SELECTED ) && !( uBits & ( TVIS_DROPHILITED | TVIS_FOCUSED ) ) ) {
                  sButton.left -= 2;
                }
              }
            }

            DrawText( hDc, pText, uTextSize, &sButton, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT );
            pExtra->iTextPixels = sButton.right - sButton.left;
          }

          if ( sButton.left + pExtra->iTextPixels >= sArea.right ) {
            if ( uTextSize > 253 )
              uTextSize = 253;

            iSize = sArea.right - sButton.left - 2;
            iSize -= ( uBits & TVIS_BOLD ) ? pData->uTrippleB : pData->uTrippleN;
            if ( iSize < 3 ) {
              iCount = 0;
            } else {
              GetTextExtentExPoint( hDc, pText, uTextSize, iSize, &iCount, pPos, &sSize );
            }

            memcpy( pPtr , pText, iCount * sizeof( TCHAR ) );
            memcpy( pPtr + iCount, _T( "..." ), 4 * sizeof( TCHAR ) );

            pText = pPtr;
            uTextSize = iCount + 3;
            sButton.right = sArea.right - 2;
          }


          switch ( pData->aColumn[ uExtra ].bAlign )
          {
          case DT_CENTER:
            iDelta = sArea .right - sArea .left;
            iDelta -= sButton.right - sButton.left;
            iDelta -= 6;
            iDelta /= 2;
            sButton.right += iDelta;
            sButton.left += iDelta;
            break;

          case DT_RIGHT:
            iDelta = sArea .right - sArea .left;
            iDelta -= sButton.right - sButton.left;
            iDelta -= 6;
            sButton.right += iDelta;
            sButton.left += iDelta;
            break;

          }

          if ( ( uBits & TVIS_SELECTED ) && pData->cHasFocus && uItem == pData->uSelectedItem ) {
            uTempColor = GetSysColor( COLOR_HIGHLIGHTTEXT );
            SelectObject( hDc, GetSysColorBrush( COLOR_HIGHLIGHT ) );
            Rectangle ( hDc, sButton.left - 2, sButton.top - 1, sButton.right + 2, sButton.bottom + 1 );
          } else {
            if ( uBits & TVIS_DROPHILITED ) {
              uTempColor = GetSysColor ( COLOR_HIGHLIGHTTEXT );
              SelectObject( hDc, GetSysColorBrush( COLOR_HIGHLIGHT ) );
              SelectObject( hDc, GetStockObject ( NULL_PEN ) );
              Rectangle ( hDc, sButton.left - 2, sButton.top - 1, sButton.right + 2, sButton.bottom + 1 );
            } else if ( uBits & TVIS_SELECTED && uItem == pData->uSelectedItem ) {
              if ( uBits & TVIS_TEXTCOLOR )
                uTempColor = pEntry->uColorText;
              else
                uTempColor = pData->uColors[ TVC_TEXT ];

              if ( pData->uStyleEx & TVS_EX_FULLROWMARK )
                SelectObject( hDc, GetStockObject( NULL_BRUSH ) );
              else
                SelectObject( hDc, GetSysColorBrush( COLOR_3DFACE ) );

              SelectObject( hDc, GetStockObject( NULL_PEN ) );
              Rectangle( hDc, sButton.left - 2, sButton.top - 1, sButton.right + 2, sButton.bottom + 1 );
            } else {
              if ( uBits & TVIS_TRACKED )
                uTempColor = pData ->uColors[ TVC_TRACK ];
              else if ( uBits & TVIS_TEXTCOLOR )
                uTempColor = pExtra->uColorText;
              else
                uTempColor = pData ->uColors[ TVC_TEXT ];
              sButton.right--;
              sButton.left --;
            }

            SelectObject( hDc, hPatternPen );

            if ( uBits & TVIS_FOCUSED )
            {
              SelectObject( hDc, GetStockObject( NULL_BRUSH ) );
              Rectangle ( hDc, sButton.left - 2, sButton.top - 1, sButton.right + 2, sButton.bottom + 1 );
            }
          }

          SetTextColor( hDc, uTempColor );
          sButton.left += pData->iFontOff;

          DrawText( hDc, pText, uTextSize, &sButton, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX );

          if ( uBits & ( TVIS_UNTERLINE | TVIS_TRACKED ) )
            if ( pText && *pText ) {
              sButton.left -= pData->iFontOff;
              sButton.right -= pData->iFontOff + 1;
              sButton.top += pData->iFontLine;
              sButton.bottom = sButton.top + 1;
              uOldColor = SetBkColor( hDc, uTempColor );
              ExtTextOut( hDc, 0, 0, ETO_OPAQUE | ETO_CLIPPED, &sButton, NULL, 0, NULL );
              SetBkColor( hDc, uOldColor );
            }

            SetTextColor( hDc, pData->uColors[ TVC_TEXT ] );
        } else {
          if ( !pExtra->iTextPixels ) {
            sButton.top = iYpos;
            sButton.left = sArea.left + 4;
            sButton.right = sArea.right;
            sButton.bottom = iYpos + pData->iFontHeight + 2;

            if ( uTextSize )
            {
              DrawText( hDc, pText, uTextSize, &sButton, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT );
              pExtra->iTextPixels = sButton.right - sButton.left;
            } else {
              pExtra->iTextPixels = 0;
            }
          }

          if ( sArea.left + pExtra->iTextPixels >= sArea.right ) {
            INT * pPos = ( INT* ) alloca( uTextSize * sizeof( INT ) );

            iSize = sArea.right - sArea.left;
            iSize -= ( uBits & TVIS_BOLD ) ? pData->uTrippleB : pData->uTrippleN;
            if ( iSize < 3 ) {
              iCount = 0;
            } else {
              GetTextExtentExPoint( hDc, pText, uTextSize, iSize, &iCount, pPos, &sSize );
            }

            if ( iCount > 0 )
            {
              sButton.right = sArea.right;
              sArea.right = sArea.left + 2 + pPos[ iCount - 1 ];

              ExtTextOut( hDc, sArea.left + 2, iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, pText, iCount, NULL );

              sArea.left = sArea.right;
              sArea.right = sButton.right;

              ExtTextOut( hDc, sArea.left , iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, _T( "..." ), 3, NULL );
            } else {
              ExtTextOut( hDc, sArea.left + 2, iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, _T( "..." ), 3, NULL );
            }
          } else {
            switch ( pData->aColumn[ uExtra ].bAlign )
            {
            case DT_CENTER:
              SetTextAlign( hDc, TA_CENTER | TA_TOP );
              ExtTextOut( hDc, ( sArea.right + sArea.left ) / 2, iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, pText, uTextSize, NULL );
              SetTextAlign( hDc, TA_LEFT | TA_TOP );
              break;

            case DT_RIGHT:
              SetTextAlign( hDc, TA_RIGHT | TA_TOP );
              ExtTextOut( hDc, sArea.right - 2, iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, pText, uTextSize, NULL );
              SetTextAlign( hDc, TA_LEFT | TA_TOP );
              break;

            default:
              ExtTextOut( hDc, sArea.left + 2, iYpos, ETO_OPAQUE | ETO_CLIPPED, &sArea, pText, uTextSize, NULL );
              break;
            }
          }
        }
      }
    }

    sArea.top += pData->iRowHeight;
  }


  if ( sArea.top < sRect.bottom )
  {
    SelectObject( hDc, hRgnMain );


    if ( pData->uMarkedCols > 0 && ( pData->cIsEnabled || !( pData->uStyleEx & TVS_EX_GRAYEDDISABLE ) ) ) {
      sRect.right = 0 - pData->uScrollX;

      for ( uExtra = 0; uExtra < pData->uColumnCount; uExtra++ ) {
        uMark = pData->aColumn[ uExtra ].bMark;
        sRect.top = sArea.top;
        sRect.left = sRect.right;
        sRect.right = pData->aColumnXpos[ uExtra + 1 ] + uMark;
        sRect.right -= pData->uScrollX;
        SetBkColor( hDc, ( uMark ) ? pData->uColors[ TVC_COLBK ] : uBkColor );
        ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sRect, NULL, 0, NULL );
      }

      sRect.top = sArea.top;
      sRect.left = sRect.right;
      sRect.right = pData->uSizeX;
      SetBkColor( hDc, uBkColor );
      ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sRect, NULL, 0, NULL );
    } else {
      sRect.top = sArea.top;
      SetBkColor( hDc, uBkColor );
      ExtTextOut( hDc, 0, 0, ETO_OPAQUE, &sRect, NULL, 0, NULL );
    }
  }


  if ( pData->iMaxSizeX != iMaxX )
  {
    pData->iMaxSizeX = iMaxX;
    if ( pData->uColumnCount == 0 ) {
      if ( pData->iMaxSizeX != ( int ) pData->uOldXCount ) {
        UpdateScrollX( pData );
      }
    }
  }

  UNLOCK( pData );

  DeleteObject( hRgnMain );

  for ( uPos = 0; uPos < uRgnCount; uPos++ ) {
    DeleteObject( hRgn[ uPos ] );
  }

  return 0;
}
