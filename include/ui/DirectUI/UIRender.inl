
typedef struct CRenderClip {
  RECT rcItem;
  HDC hDC;
  HRGN hRgn;
  HRGN hOldRgn;
} CRenderClip;
void CRenderClip_Clear(CRenderClip* clip) {
  ASSERT( GetObjectType( clip->hDC ) == OBJ_DC || GetObjectType( clip->hDC ) == OBJ_MEMDC );
  ASSERT( GetObjectType( clip->hRgn ) == OBJ_REGION );
  ASSERT( GetObjectType( clip->hOldRgn ) == OBJ_REGION );
  SelectClipRgn( clip->hDC, clip->hOldRgn );
  DeleteObject( clip->hOldRgn );
  DeleteObject( clip->hRgn );
}
void GenerateClip( HDC hDC, RECT rcItem, CRenderClip* clip ) {
  RECT rcClip = { 0 };
  GetClipBox( hDC, &rcClip );
  clip->hOldRgn = CreateRectRgnIndirect( &rcClip );
  clip->hRgn = CreateRectRgnIndirect( &rcItem );
  ExtSelectClipRgn( hDC, clip->hRgn, RGN_AND );
  clip->hDC = hDC;
  clip->rcItem = rcItem;
}
#define UseOldClipBegin( hDC, clip )   SelectClipRgn( hDC, clip.hOldRgn )
#define UseOldClipEnd( hDC, clip )     SelectClipRgn( hDC, clip.hRgn )

//
static COLORREF PixelAlpha( COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken ) {
  return RGB ( GetRValue ( clrSrc ) * src_darken + GetRValue ( clrDest ) * dest_darken,
               GetGValue ( clrSrc ) * src_darken + GetGValue ( clrDest ) * dest_darken,
               GetBValue ( clrSrc ) * src_darken + GetBValue ( clrDest ) * dest_darken );
}
static BOOL WINAPI AlphaBitBlt( HDC hDC, int nDestX, int nDestY, int dwWidth, int dwHeight, HDC hSrcDC, 
                               int nSrcX, int nSrcY, int wSrc, int hSrc, BLENDFUNCTION ftn ) {
  LPBITMAPINFO lpbiSrc;
  COLORREF* pSrcBits;
  HBITMAP hSrcDib;
  HBITMAP hOldTempBmp;
  LPBITMAPINFO lpbiDest;
  COLORREF* pDestBits;
  HBITMAP hDestDib;
  double src_darken;
  int pixel;
  BYTE nAlpha;

  HDC hTempDC = CreateCompatibleDC( hDC );
  if ( NULL == hTempDC )
    return FALSE;
  //Creates Source DIB
  // Fill in the BITMAPINFOHEADER
  lpbiSrc = ( LPBITMAPINFO ) malloc( sizeof( BITMAPINFOHEADER ) );
  lpbiSrc->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
  lpbiSrc->bmiHeader.biWidth = dwWidth;
  lpbiSrc->bmiHeader.biHeight = dwHeight;
  lpbiSrc->bmiHeader.biPlanes = 1;
  lpbiSrc->bmiHeader.biBitCount = 32;
  lpbiSrc->bmiHeader.biCompression = BI_RGB;
  lpbiSrc->bmiHeader.biSizeImage = dwWidth * dwHeight;
  lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
  lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
  lpbiSrc->bmiHeader.biClrUsed = 0;
  lpbiSrc->bmiHeader.biClrImportant = 0;
  pSrcBits = NULL;
  hSrcDib = CreateDIBSection( hSrcDC, lpbiSrc, DIB_RGB_COLORS, ( void ** ) (&pSrcBits), NULL, 0 );
  if ( ( NULL == hSrcDib ) || ( NULL == pSrcBits ) ) {
    DeleteDC( hTempDC );
    return FALSE;
  }
  hOldTempBmp = ( HBITMAP ) SelectObject ( hTempDC, hSrcDib );
  StretchBlt( hTempDC, 0, 0, dwWidth, dwHeight, hSrcDC, nSrcX, nSrcY, wSrc, hSrc, SRCCOPY );
  SelectObject ( hTempDC, hOldTempBmp );
  //Creates Destination DIB
  // Fill in the BITMAPINFOHEADER
  lpbiDest = ( LPBITMAPINFO ) malloc( sizeof( BITMAPINFOHEADER ) );
  lpbiDest->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
  lpbiDest->bmiHeader.biWidth = dwWidth;
  lpbiDest->bmiHeader.biHeight = dwHeight;
  lpbiDest->bmiHeader.biPlanes = 1;
  lpbiDest->bmiHeader.biBitCount = 32;
  lpbiDest->bmiHeader.biCompression = BI_RGB;
  lpbiDest->bmiHeader.biSizeImage = dwWidth * dwHeight;
  lpbiDest->bmiHeader.biXPelsPerMeter = 0;
  lpbiDest->bmiHeader.biYPelsPerMeter = 0;
  lpbiDest->bmiHeader.biClrUsed = 0;
  lpbiDest->bmiHeader.biClrImportant = 0;
  pDestBits = NULL;
  hDestDib = CreateDIBSection (hDC, lpbiDest, DIB_RGB_COLORS, ( void ** ) & pDestBits, NULL, 0 );
  if ( ( NULL == hDestDib ) || ( NULL == pDestBits ) ) {
    free(lpbiSrc);
    DeleteObject( hSrcDib );
    DeleteDC( hTempDC );
    return FALSE;
  }
  SelectObject ( hTempDC, hDestDib );
  BitBlt ( hTempDC, 0, 0, dwWidth, dwHeight, hDC, nDestX, nDestY, SRCCOPY );
  SelectObject ( hTempDC, hOldTempBmp );
  for ( pixel = 0; pixel < dwWidth * dwHeight; pixel++, pSrcBits++, pDestBits++ ) {
    nAlpha = LOBYTE( *pSrcBits >> 24 );
    src_darken = ( double ) ( nAlpha * ftn.SourceConstantAlpha ) / 255.0 / 255.0;
    if ( src_darken < 0.0 )
      src_darken = 0.0;
    *pDestBits = PixelAlpha( *pSrcBits, src_darken, *pDestBits, 1.0 - src_darken );
  } //for
  SelectObject ( hTempDC, hDestDib );
  BitBlt ( hDC, nDestX, nDestY, dwWidth, dwHeight, hTempDC, 0, 0, SRCCOPY );
  SelectObject ( hTempDC, hOldTempBmp );
  free(lpbiDest);
  DeleteObject( hDestDib );
  free(lpbiSrc);
  DeleteObject( hSrcDib );
  DeleteDC( hTempDC );
  return TRUE;
}

static void UIDrawImage( HDC hDC, HBITMAP hBitmap, RECT rc, RECT rcPaint,
                      RECT rcBmpPart, RECT rcCorners, bool alphaChannel, BYTE uFade/* = 255*/,
                      bool hole/* = false*/, bool xtiled/* = false*/, bool ytiled/* = false*/ ) {
  typedef BOOL ( WINAPI * LPALPHABLEND ) ( HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION );
  static LPALPHABLEND lpAlphaBlend = NULL;
  HDC hCloneDC;
  HBITMAP hOldBitmap;
  RECT rcTemp = {0};
  RECT rcDest = {0};
  int i, j;
  if (NULL==lpAlphaBlend) {
  lpAlphaBlend = ( LPALPHABLEND ) GetProcAddress( GetModuleHandle( _T( "msimg32.dll" ) ), "AlphaBlend" );}
  ASSERT( GetObjectType( hDC ) == OBJ_DC || GetObjectType( hDC ) == OBJ_MEMDC );
  if ( lpAlphaBlend == NULL )
    lpAlphaBlend = AlphaBitBlt;
  if ( hBitmap == NULL )
    return ;
  hCloneDC = CreateCompatibleDC( hDC );
  hOldBitmap = ( HBITMAP ) SelectObject( hCloneDC, hBitmap );
  SetStretchBltMode( hDC, COLORONCOLOR );
  if ( lpAlphaBlend && ( alphaChannel || uFade < 255 ) ) {
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, uFade, AC_SRC_ALPHA };
    // middle
    if ( !hole ) {
      rcDest.left = rc.left + rcCorners.left;
      rcDest.top = rc.top + rcCorners.top;
      rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
      rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        if ( !xtiled && !ytiled ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
            rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
            rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf );
        } else if ( xtiled && ytiled ) {
          LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right;
          LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom;
          int iTimesX = ( rcDest.right - rcDest.left + lWidth - 1 ) / lWidth;
          int iTimesY = ( rcDest.bottom - rcDest.top + lHeight - 1 ) / lHeight;
          for ( j = 0; j < iTimesY; ++j ) {
            LONG lDestTop = rcDest.top + lHeight * j;
            LONG lDestBottom = rcDest.top + lHeight * ( j + 1 );
            LONG lDrawHeight = lHeight;
            if ( lDestBottom > rcDest.bottom ) {
              lDrawHeight -= lDestBottom - rcDest.bottom;
              lDestBottom = rcDest.bottom;
            }
            for ( i = 0; i < iTimesX; ++i ) {
              LONG lDestLeft = rcDest.left + lWidth * i;
              LONG lDestRight = rcDest.left + lWidth * ( i + 1 );
              LONG lDrawWidth = lWidth;
              if ( lDestRight > rcDest.right ) {
                lDrawWidth -= lDestRight - rcDest.right;
                lDestRight = rcDest.right;
              }
              lpAlphaBlend( hDC, rcDest.left + lWidth * i, rcDest.top + lHeight * j,
                lDestRight - lDestLeft, lDestBottom - lDestTop, hCloneDC,
                rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, lDrawWidth, lDrawHeight, bf );
            }
          }
        } else if ( xtiled ) {
          LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right;
          int iTimes = ( rcDest.right - rcDest.left + lWidth - 1 ) / lWidth;
          for ( i = 0; i < iTimes; ++i ) {
            LONG lDestLeft = rcDest.left + lWidth * i;
            LONG lDestRight = rcDest.left + lWidth * ( i + 1 );
            LONG lDrawWidth = lWidth;
            if ( lDestRight > rcDest.right ) {
              lDrawWidth -= lDestRight - rcDest.right;
              lDestRight = rcDest.right;
            }
            lpAlphaBlend( hDC, lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom,
              hCloneDC, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
              lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf );
          }
        } else { // ytiled
          LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom;
          int iTimes = ( rcDest.bottom - rcDest.top + lHeight - 1 ) / lHeight;
          for ( i = 0; i < iTimes; ++i ) {
            LONG lDestTop = rcDest.top + lHeight * i;
            LONG lDestBottom = rcDest.top + lHeight * ( i + 1 );
            LONG lDrawHeight = lHeight;
            if ( lDestBottom > rcDest.bottom ) {
              lDrawHeight -= lDestBottom - rcDest.bottom;
              lDestBottom = rcDest.bottom;
            }
            lpAlphaBlend( hDC, rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop,
              hCloneDC, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
              rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, lDrawHeight, bf );
          }
        }
      }
    }
    // left-top
    if ( rcCorners.left > 0 && rcCorners.top > 0 ) {
      rcDest.left = rc.left;
      rcDest.top = rc.top;
      rcDest.right = rcCorners.left;
      rcDest.bottom = rcCorners.top;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        rcDest.right -= rcDest.left;
        rcDest.bottom -= rcDest.top;
        lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
          rcBmpPart.left, rcBmpPart.top, rcCorners.left, rcCorners.top, bf );
      }
    }
    // top
    if ( rcCorners.top > 0 ) {
      rcDest.left = rc.left + rcCorners.left;
      rcDest.top = rc.top;
      rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
      rcDest.bottom = rcCorners.top;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        rcDest.right -= rcDest.left;
        rcDest.bottom -= rcDest.top;
        lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
          rcBmpPart.left + rcCorners.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
          rcCorners.left - rcCorners.right, rcCorners.top, bf );
      }
    }
    // right-top
    if ( rcCorners.right > 0 && rcCorners.top > 0 ) {
      rcDest.left = rc.right - rcCorners.right;
      rcDest.top = rc.top;
      rcDest.right = rcCorners.right;
      rcDest.bottom = rcCorners.top;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        rcDest.right -= rcDest.left;
        rcDest.bottom -= rcDest.top;
        lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
          rcBmpPart.right - rcCorners.right, rcBmpPart.top, rcCorners.right, rcCorners.top, bf );
      }
    }
    // left
    if ( rcCorners.left > 0 ) {
      rcDest.left = rc.left;
      rcDest.top = rc.top + rcCorners.top;
      rcDest.right = rcCorners.left;
      rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        rcDest.right -= rcDest.left;
        rcDest.bottom -= rcDest.top;
        lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
          rcBmpPart.left, rcBmpPart.top + rcCorners.top, rcCorners.left, rcBmpPart.bottom - \
          rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf );
      }
    }
    // right
    if ( rcCorners.right > 0 ) {
      rcDest.left = rc.right - rcCorners.right;
      rcDest.top = rc.top + rcCorners.top;
      rcDest.right = rcCorners.right;
      rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        rcDest.right -= rcDest.left;
        rcDest.bottom -= rcDest.top;
        lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
          rcBmpPart.right - rcCorners.right, rcBmpPart.top + rcCorners.top, rcCorners.right, \
          rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf );
      }
    }
    // left-bottom
    if ( rcCorners.left > 0 && rcCorners.bottom > 0 ) {
      rcDest.left = rc.left;
      rcDest.top = rc.bottom - rcCorners.bottom;
      rcDest.right = rcCorners.left;
      rcDest.bottom = rcCorners.bottom;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        rcDest.right -= rcDest.left;
        rcDest.bottom -= rcDest.top;
        lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
          rcBmpPart.left, rcBmpPart.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, bf );
      }
    }
    // bottom
    if ( rcCorners.bottom > 0 ) {
      rcDest.left = rc.left + rcCorners.left;
      rcDest.top = rc.bottom - rcCorners.bottom;
      rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
      rcDest.bottom = rcCorners.bottom;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        rcDest.right -= rcDest.left;
        rcDest.bottom -= rcDest.top;
        lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
          rcBmpPart.left + rcCorners.left, rcBmpPart.bottom - rcCorners.bottom, \
          rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, rcCorners.bottom, bf );
      }
    }
    // right-bottom
    if ( rcCorners.right > 0 && rcCorners.bottom > 0 ) {
      rcDest.left = rc.right - rcCorners.right;
      rcDest.top = rc.bottom - rcCorners.bottom;
      rcDest.right = rcCorners.right;
      rcDest.bottom = rcCorners.bottom;
      rcDest.right += rcDest.left;
      rcDest.bottom += rcDest.top;
      if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
        rcDest.right -= rcDest.left;
        rcDest.bottom -= rcDest.top;
        lpAlphaBlend( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
          rcBmpPart.right - rcCorners.right, rcBmpPart.bottom - rcCorners.bottom, rcCorners.right, \
          rcCorners.bottom, bf );
      }
    }
  } else {
    if ( rc.right - rc.left == rcBmpPart.right - rcBmpPart.left \
      && rc.bottom - rc.top == rcBmpPart.bottom - rcBmpPart.top \
      && rcCorners.left == 0 && rcCorners.right == 0 && rcCorners.top == 0 && rcCorners.bottom == 0 ) {
      if ( IntersectRect( &rcTemp, &rcPaint, &rc ) ) {
        BitBlt( hDC, rcTemp.left, rcTemp.top, rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top, \
          hCloneDC, rcBmpPart.left + rcTemp.left - rc.left, rcBmpPart.top + rcTemp.top - rc.top, SRCCOPY );
      }
    } else {
      // middle
      if ( !hole ) {
        rcDest.left = rc.left + rcCorners.left;
        rcDest.top = rc.top + rcCorners.top;
        rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
        rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          if ( !xtiled && !ytiled ) {
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
              rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
              rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
              rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY );
          } else if ( xtiled && ytiled ) {
            LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right;
            LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom;
            int iTimesX = ( rcDest.right - rcDest.left + lWidth - 1 ) / lWidth;
            int iTimesY = ( rcDest.bottom - rcDest.top + lHeight - 1 ) / lHeight;
            for ( j = 0; j < iTimesY; ++j ) {
              LONG lDestTop = rcDest.top + lHeight * j;
              LONG lDestBottom = rcDest.top + lHeight * ( j + 1 );
              LONG lDrawHeight = lHeight;
              if ( lDestBottom > rcDest.bottom ) {
                lDrawHeight -= lDestBottom - rcDest.bottom;
                lDestBottom = rcDest.bottom;
              }
              for ( i = 0; i < iTimesX; ++i ) {
                LONG lDestLeft = rcDest.left + lWidth * i;
                LONG lDestRight = rcDest.left + lWidth * ( i + 1 );
                LONG lDrawWidth = lWidth;
                if ( lDestRight > rcDest.right ) {
                  lDrawWidth -= lDestRight - rcDest.right;
                  lDestRight = rcDest.right;
                }
                BitBlt( hDC, rcDest.left + lWidth * i, rcDest.top + lHeight * j, \
                  lDestRight - lDestLeft, lDestBottom - lDestTop, hCloneDC, \
                  rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, SRCCOPY );
              }
            }
          } else if ( xtiled ) {
            LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right;
            int iTimes = ( rcDest.right - rcDest.left + lWidth - 1 ) / lWidth;
            for ( i = 0; i < iTimes; ++i ) {
              LONG lDestLeft = rcDest.left + lWidth * i;
              LONG lDestRight = rcDest.left + lWidth * ( i + 1 );
              LONG lDrawWidth = lWidth;
              if ( lDestRight > rcDest.right ) {
                lDrawWidth -= lDestRight - rcDest.right;
                lDestRight = rcDest.right;
              }
              StretchBlt( hDC, lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom,
                hCloneDC, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
                lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY );
            }
          } else { // ytiled
            LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom;
            int iTimes = ( rcDest.bottom - rcDest.top + lHeight - 1 ) / lHeight;
            for ( i = 0; i < iTimes; ++i ) {
              LONG lDestTop = rcDest.top + lHeight * i;
              LONG lDestBottom = rcDest.top + lHeight * ( i + 1 );
              LONG lDrawHeight = lHeight;
              if ( lDestBottom > rcDest.bottom ) {
                lDrawHeight -= lDestBottom - rcDest.bottom;
                lDestBottom = rcDest.bottom;
              }
              StretchBlt( hDC, rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop,
                hCloneDC, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
                rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, lDrawHeight, SRCCOPY );
            }
          }
        }
      }
      // left-top
      if ( rcCorners.left > 0 && rcCorners.top > 0 ) {
        rcDest.left = rc.left;
        rcDest.top = rc.top;
        rcDest.right = rcCorners.left;
        rcDest.bottom = rcCorners.top;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.left, rcBmpPart.top, rcCorners.left, rcCorners.top, SRCCOPY );
        }
      }
      // top
      if ( rcCorners.top > 0 ) {
        rcDest.left = rc.left + rcCorners.left;
        rcDest.top = rc.top;
        rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
        rcDest.bottom = rcCorners.top;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.left + rcCorners.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
            rcCorners.left - rcCorners.right, rcCorners.top, SRCCOPY );
        }
      }
      // right-top
      if ( rcCorners.right > 0 && rcCorners.top > 0 ) {
        rcDest.left = rc.right - rcCorners.right;
        rcDest.top = rc.top;
        rcDest.right = rcCorners.right;
        rcDest.bottom = rcCorners.top;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.right - rcCorners.right, rcBmpPart.top, rcCorners.right, rcCorners.top, SRCCOPY );
        }
      }
      // left
      if ( rcCorners.left > 0 ) {
        rcDest.left = rc.left;
        rcDest.top = rc.top + rcCorners.top;
        rcDest.right = rcCorners.left;
        rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.left, rcBmpPart.top + rcCorners.top, rcCorners.left, rcBmpPart.bottom - \
            rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY );
        }
      }
      // right
      if ( rcCorners.right > 0 ) {
        rcDest.left = rc.right - rcCorners.right;
        rcDest.top = rc.top + rcCorners.top;
        rcDest.right = rcCorners.right;
        rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.right - rcCorners.right, rcBmpPart.top + rcCorners.top, rcCorners.right, \
            rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY );
        }
      }
      // left-bottom
      if ( rcCorners.left > 0 && rcCorners.bottom > 0 ) {
        rcDest.left = rc.left;
        rcDest.top = rc.bottom - rcCorners.bottom;
        rcDest.right = rcCorners.left;
        rcDest.bottom = rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.left, rcBmpPart.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, SRCCOPY );
        }
      }
      // bottom
      if ( rcCorners.bottom > 0 ) {
        rcDest.left = rc.left + rcCorners.left;
        rcDest.top = rc.bottom - rcCorners.bottom;
        rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
        rcDest.bottom = rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.left + rcCorners.left, rcBmpPart.bottom - rcCorners.bottom, \
            rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, rcCorners.bottom, SRCCOPY );
        }
      }
      // right-bottom
      if ( rcCorners.right > 0 && rcCorners.bottom > 0 ) {
        rcDest.left = rc.right - rcCorners.right;
        rcDest.top = rc.bottom - rcCorners.bottom;
        rcDest.right = rcCorners.right;
        rcDest.bottom = rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if ( IntersectRect( &rcTemp, &rcPaint, &rcDest ) ) {
          rcDest.right -= rcDest.left;
          rcDest.bottom -= rcDest.top;
          StretchBlt( hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
            rcBmpPart.right - rcCorners.right, rcBmpPart.bottom - rcCorners.bottom, rcCorners.right, \
            rcCorners.bottom, SRCCOPY );
        }
      }
    }
  }
  SelectObject( hCloneDC, hOldBitmap );
  DeleteDC( hCloneDC );
}
char* strcatch(char* s, int ch) {int n=strlen(s); s[n]=ch, s[n+1]=0;return s;}
static bool DrawImageString( HDC hDC, UIResource* pResource, RECT rc, RECT rcPaint,
                            LPCTSTR pStrImage, LPCTSTR pStrModify/* = NULL*/ ) {
  // 1、aaa.jpg
  // 2、file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0'
  // mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false'
  RECT rcItem = rc;
  RECT rcBmpPart = {0};
  RECT rcCorner = {0};
  DWORD dwMask = 0;
  BYTE bFade = 0xFF;
  bool bHole = false;
  bool bTiledX = false;
  bool bTiledY = false;
  TCHAR sImageName[256]="";
  TCHAR sImageResType[256]="";
  TCHAR sItem[256];
  TCHAR sValue[256];
  const TImageInfo* data = NULL;
  LPTSTR pstr = NULL;
  int i;
  strncpy(sImageName, pStrImage, 256);
  for ( i = 0; i < 2; ++i ) {
    if ( i == 1 ) {
      pStrImage = pStrModify;
    }
    if ( !pStrImage )
      continue;
    while ( *pStrImage != _T( '\0' ) ) {
      sItem[0]=0;
      sValue[0]=0;
      while ( *pStrImage > _T( '\0' ) && *pStrImage <= _T( ' ' ) )
        pStrImage = CharNext( pStrImage );
      while ( *pStrImage != _T( '\0' ) && *pStrImage != _T( '=' ) && *pStrImage > _T( ' ' ) ) {
        LPTSTR pstrTemp = CharNext( pStrImage );
        while ( pStrImage < pstrTemp ) {
          strcatch(sItem, *pStrImage++);
        }
      }
      while ( *pStrImage > _T( '\0' ) && *pStrImage <= _T( ' ' ) )
        pStrImage = CharNext( pStrImage );
      if ( *pStrImage++ != _T( '=' ) )
        break;
      while ( *pStrImage > _T( '\0' ) && *pStrImage <= _T( ' ' ) )
        pStrImage = CharNext( pStrImage );
      if ( *pStrImage++ != _T( '\'' ) )
        break;
      while ( *pStrImage != _T( '\0' ) && *pStrImage != _T( '\'' ) ) {
        LPTSTR pstrTemp = CharNext( pStrImage );
        while ( pStrImage < pstrTemp ) {
          strcatch(sValue, *pStrImage++);
        }
      }
      if ( *pStrImage++ != _T( '\'' ) )
        break;
      if ( 0!=strlen(sValue) ) {
        if ( 0==strcmp(sItem, _T( "file" )) || 0==strcmp(sItem, _T( "res" )) ) {
          strncpy(sImageName, sValue, 256);
        } else if ( 0==strcmp(sItem, _T( "restype" )) ) {
          strncpy(sImageResType, sValue, 256);
        } else if ( 0==strcmp(sItem, _T( "dest" )) ) {
          rcItem.left = rc.left + _tcstol( sValue, &pstr, 10 );
          ASSERT( pstr );
          rcItem.top = rc.top + _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
          rcItem.right = rc.left + _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
          rcItem.bottom = rc.top + _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
        } else if ( 0==strcmp(sItem, _T( "source" )) ) {
          rcBmpPart.left = _tcstol( sValue, &pstr, 10 );
          ASSERT( pstr );
          rcBmpPart.top = _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
          rcBmpPart.right = _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
          rcBmpPart.bottom = _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
        } else if ( 0==strcmp(sItem, _T( "corner" )) ) {
          rcCorner.left = _tcstol( sValue, &pstr, 10 );
          ASSERT( pstr );
          rcCorner.top = _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
          rcCorner.right = _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
          rcCorner.bottom = _tcstol( pstr + 1, &pstr, 10 );
          ASSERT( pstr );
        } else if ( 0==strcmp(sItem, _T( "mask" )) ) {
          if ( sValue[ 0 ] == _T( '#' ) )
            dwMask = _tcstoul( sValue + 1, &pstr, 16 );
          else
            dwMask = _tcstoul( sValue, &pstr, 16 );
        } else if ( 0==strcmp(sItem, _T( "fade" )) ) {
          bFade = ( BYTE ) _tcstoul( sValue, &pstr, 10 );
        } else if ( 0==strcmp(sItem, _T( "hole" )) ) {
          bHole = ( _tcscmp( sValue, _T( "true" ) ) == 0 );
        } else if ( 0==strcmp(sItem, _T( "xtiled" )) ) {
          bTiledX = ( _tcscmp( sValue, _T( "true" ) ) == 0 );
        } else if ( 0==strcmp(sItem, _T( "ytiled" )) ) {
          bTiledY = ( _tcscmp( sValue, _T( "true" ) ) == 0 );
        }
      }
      if ( *pStrImage++ != _T( ' ' ) )
        break;
    }
  }
  if ( IsEmpty(sImageResType) ) {
    data = UIResource_GetImageEx( pResource, ( LPCTSTR ) sImageName, NULL, dwMask );
  } else {
    data = UIResource_GetImageEx( pResource, ( LPCTSTR ) sImageName, ( LPCTSTR ) sImageResType, dwMask );
  }
  if ( !data )
    return false;
  if ( hDC == NULL )
    return true;
  if ( rcBmpPart.left == 0 && rcBmpPart.right == 0 && rcBmpPart.top == 0 && rcBmpPart.bottom == 0 ) {
    rcBmpPart.right = data->nX;
    rcBmpPart.bottom = data->nY;
  }
  {
    RECT rcTemp;
    if ( !IntersectRect( &rcTemp, &rcItem, &rc ) )
      return true;
    if ( !IntersectRect( &rcTemp, &rcItem, &rcPaint ) )
      return true;
    UIDrawImage( hDC, data->hBitmap, rcItem, rcPaint, rcBmpPart, rcCorner, data->alphaChannel, bFade, bHole, bTiledX, bTiledY );
  }
  return true;
}
static void DrawColor( HDC hDC, RECT rc, DWORD color ) {
  if ( color <= 0x00FFFFFF )
    return ;
  if ( color >= 0xFF000000 ) {
    SetBkColor( hDC, RGB( GetBValue( color ), GetGValue( color ), GetRValue( color ) ) );
    ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );
  } else {
    LPDWORD pDest = NULL;
    // Create a new 32bpp bitmap with room for an alpha channel
    BITMAPINFO bmi = { 0 };
    HBITMAP hBitmap;
    RECT rcBmpPart = {0, 0, 1, 1};
    RECT rcCorners = {0};
    bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    bmi.bmiHeader.biWidth = 1;
    bmi.bmiHeader.biHeight = 1;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 1 * 1 * sizeof( DWORD );
    hBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS, ( LPVOID* ) & pDest, NULL, 0 );
    if ( !hBitmap )
      return ;
    *pDest = color;
    UIDrawImage( hDC, hBitmap, rc, rc, rcBmpPart, rcCorners, true, 255, 0, 0, 0 );
    DeleteObject( hBitmap );
  }
}
// 以下函数中的颜色参数alpha值无效
static void DrawGradient( HDC hDC, RECT rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps ) {
  typedef BOOL ( WINAPI * PGradientFill ) ( HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG );
  static PGradientFill lpGradientFill = NULL;
  if (lpGradientFill==NULL) {
    lpGradientFill = ( PGradientFill ) GetProcAddress( GetModuleHandle( _T( "msimg32.dll" ) ), "GradientFill" ); }
  if ( lpGradientFill != NULL ) {
    TRIVERTEX triv[ 2 ] =
    {
      { rc.left, rc.top, GetBValue( dwFirst ) << 8, GetGValue( dwFirst ) << 8, GetRValue( dwFirst ) << 8, 0xFF00 },
      { rc.right, rc.bottom, GetBValue( dwSecond ) << 8, GetGValue( dwSecond ) << 8, GetRValue( dwSecond ) << 8, 0xFF00 }
    };
    GRADIENT_RECT grc = { 0, 1 };
    lpGradientFill( hDC, triv, 2, &grc, 1, bVertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H );
  } else {
    // Determine how many shades
    int i, nLines, nShift = 1;
    if ( nSteps >= 64 )
      nShift = 6;
    else if ( nSteps >= 32 )
      nShift = 5;
    else if ( nSteps >= 16 )
      nShift = 4;
    else if ( nSteps >= 8 )
      nShift = 3;
    else if ( nSteps >= 4 )
      nShift = 2;
    nLines = 1 << nShift;
    for ( i = 0; i < nLines; i++ ) {
      // Do a little alpha blending
      BYTE bR = ( BYTE ) ( ( GetBValue( dwSecond ) * ( nLines - i ) + GetBValue( dwFirst ) * i ) >> nShift );
      BYTE bG = ( BYTE ) ( ( GetGValue( dwSecond ) * ( nLines - i ) + GetGValue( dwFirst ) * i ) >> nShift );
      BYTE bB = ( BYTE ) ( ( GetRValue( dwSecond ) * ( nLines - i ) + GetRValue( dwFirst ) * i ) >> nShift );
      // ... then paint with the resulting color
      HBRUSH hBrush = CreateSolidBrush( RGB( bR, bG, bB ) );
      RECT r2 = rc;
      if ( bVertical ) {
        r2.bottom = rc.bottom - ( ( i * ( rc.bottom - rc.top ) ) >> nShift );
        r2.top = rc.bottom - ( ( ( i + 1 ) * ( rc.bottom - rc.top ) ) >> nShift );
        if ( ( r2.bottom - r2.top ) > 0 )
          FillRect( hDC, &r2, hBrush );
      } else {
        r2.left = rc.right - ( ( ( i + 1 ) * ( rc.right - rc.left ) ) >> nShift );
        r2.right = rc.right - ( ( i * ( rc.right - rc.left ) ) >> nShift );
        if ( ( r2.right - r2.left ) > 0 )
          FillRect( hDC, &r2, hBrush );
      }
      DeleteObject( hBrush );
    }
  }
}
static void UIDrawLine( HDC hDC, RECT rc, int nSize, DWORD dwPenColor ) {
  POINT ptTemp = { 0 };
  HPEN hPen, hOldPen;
  ASSERT( GetObjectType( hDC ) == OBJ_DC || GetObjectType( hDC ) == OBJ_MEMDC );
  hPen = CreatePen( PS_SOLID, nSize, RGB( GetBValue( dwPenColor ), GetGValue( dwPenColor ), GetRValue( dwPenColor ) ) );
  hOldPen = ( HPEN ) SelectObject( hDC, hPen );
  MoveToEx( hDC, rc.left, rc.top, &ptTemp );
  LineTo( hDC, rc.right, rc.bottom );
  SelectObject( hDC, hOldPen );
  DeleteObject( hPen );
}
static void UIDrawRect( HDC hDC, RECT rc, int nSize, DWORD dwPenColor ) {
  HPEN hPen, hOldPen;
  ASSERT( GetObjectType( hDC ) == OBJ_DC || GetObjectType( hDC ) == OBJ_MEMDC );
  hPen = CreatePen( PS_SOLID | PS_INSIDEFRAME, nSize, RGB( GetBValue( dwPenColor ), GetGValue( dwPenColor ), GetRValue( dwPenColor ) ) );
  hOldPen = ( HPEN ) SelectObject( hDC, hPen );
  SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
  Rectangle( hDC, rc.left, rc.top, rc.right, rc.bottom );
  SelectObject( hDC, hOldPen );
  DeleteObject( hPen );
}
static void UIDrawText( HDC hDC, UIResource* pResource, RECT rc, LPCTSTR pstrText,
                       DWORD dwTextColor, int iFont, UINT uStyle ) {
  HFONT hOldFont;
  ASSERT( GetObjectType( hDC ) == OBJ_DC || GetObjectType( hDC ) == OBJ_MEMDC );
  SetBkMode( hDC, TRANSPARENT );
  SetTextColor( hDC, RGB( GetBValue( dwTextColor ), GetGValue( dwTextColor ), GetRValue( dwTextColor ) ) );
  hOldFont = ( HFONT ) SelectObject( hDC, UIResource_GetFont( pResource, iFont ) );
  DrawText( hDC, pstrText, -1, &rc, uStyle );
  SelectObject( hDC, hOldFont );
}
// 考虑到在xml编辑器中使用<>符号不方便，可以使用{}符号代替
// The string formatter supports a kind of "mini-html" that consists of various short tags:
//
//   Link:         <a x>text</a>        where x(optional) = link content, normal like app:notepad or http:www.xxx.com
//   Bold:         <b>text</b>
//   Color:        <c #xxxxxx>text</c>  where x = RGB in hex
//   Change font:  <f x>text</f>        where x = font id
//   Image:        <i x y>              where x = image name and y(optional) = imagelist id
//   NewLine       <n>
//   Paragraph:    <p>
//   X Indent:     <x i>                where i = hor indent in pixels
//   Y Indent:     <y i>                where i = ver indent in pixels
static void DrawHtmlText( HDC hDC, UIResource* pResource, RECT rc, LPCTSTR pstrText,
                         DWORD dwTextColor, RECT* prcLinks, char** sLinks, int* pLinkRects , UINT uStyle, POINT ptMouse) {
#if 0
  int i, nLinkRects = *pLinkRects;
  RECT rcClip = { 0 };
  bool bDraw, bHoverLink;
  HRGN hOldRgn, hRgn;
  HFONT hOldFont;
  ASSERT( GetObjectType( hDC ) == OBJ_DC || GetObjectType( hDC ) == OBJ_MEMDC );
  if ( IsRectEmpty( &rc ) )
    return ;
  bDraw = ( uStyle & DT_CALCRECT ) == 0;
  GetClipBox( hDC, &rcClip );
  hOldRgn = CreateRectRgnIndirect( &rcClip );
  hRgn = CreateRectRgnIndirect( &rc );
  if ( bDraw )
    ExtSelectClipRgn( hDC, hRgn, RGN_AND );
  hOldFont = ( HFONT ) SelectObject( hDC, UIResource_GetDefaultFont(pResource) );
  SetBkMode( hDC, TRANSPARENT );
  SetTextColor( hDC, RGB( GetBValue( dwTextColor ), GetGValue( dwTextColor ), GetRValue( dwTextColor ) ) );
  // If the drawstyle include a alignment, we'll need to first determine the text-size so
  // we can draw it at the correct position...
  if ( ( uStyle & DT_SINGLELINE ) != 0 && ( uStyle & DT_VCENTER ) != 0 && ( uStyle & DT_CALCRECT ) == 0 ) {
    RECT rcText = { 0, 0, 9999, 100 };
    int nLinks = 0;
    DrawHtmlText( hDC, pResource, rcText, pstrText, dwTextColor, NULL, NULL, &nLinks, uStyle | DT_CALCRECT, ptMouse );
    rc.top = rc.top + ( ( rc.bottom - rc.top ) / 2 ) - ( ( rcText.bottom - rcText.top ) / 2 );
    rc.bottom = rc.top + ( rcText.bottom - rcText.top );
  }
  if ( ( uStyle & DT_SINGLELINE ) != 0 && ( uStyle & DT_CENTER ) != 0 && ( uStyle & DT_CALCRECT ) == 0 ) {
    RECT rcText = { 0, 0, 9999, 100 };
    int nLinks = 0;
    DrawHtmlText( hDC, pResource, rcText, pstrText, dwTextColor, NULL, NULL, &nLinks, uStyle | DT_CALCRECT, ptMouse );
    rc.left = rc.left + ( ( rc.right - rc.left ) / 2 ) - ( ( rcText.right - rcText.left ) / 2 );
    rc.right = rc.left + ( rcText.right - rcText.left );
  }
  if ( ( uStyle & DT_SINGLELINE ) != 0 && ( uStyle & DT_RIGHT ) != 0 && ( uStyle & DT_CALCRECT ) == 0 ) {
    RECT rcText = { 0, 0, 9999, 100 };
    int nLinks = 0;
    DrawHtmlText( hDC, pResource, rcText, pstrText, dwTextColor, NULL, NULL, &nLinks, uStyle | DT_CALCRECT, ptMouse );
    rc.left = rc.right - ( rcText.right - rcText.left );
  }
  // Determine if we're hovering over a link, because we would like to
  // indicate it by coloring the link text.
  // BUG: This assumes that the prcLink has already been filled once with
  //      link coordinates! That is usually not the case at first repaint. We'll clear
  //      the remanining entries at exit.
  bHoverLink = false;
  for ( i = 0; !bHoverLink && i < nLinkRects; i++ ) {
    char* pStr = sLinks[i];
    pStr[0]=0;
    if ( PtInRect( prcLinks + i, ptMouse ) )
      bHoverLink = true;
  }
  {

  TEXTMETRIC tm = UIResource_GetDefaultFontInfo(pResource);
  POINT pt = { rc.left, rc.top };
  int iLinkIndex = 0;
  int cyLine = tm.tmHeight + tm.tmExternalLeading;
  int cyMinHeight = 0;
  int cxMaxWidth = 0;
  POINT ptLinkStart = { 0 };
  bool bInLink = false;
  while ( *pstrText != _T( '\0' ) ) {
    if ( pt.x >= rc.right || *pstrText == _T( '\n' ) ) {
      // A new link was detected/requested. We'll adjust the line height
      // for the next line and expand the link hitbox (if any)
      if ( bInLink && iLinkIndex < nLinkRects )
        SetRect( &prcLinks[ iLinkIndex++ ], ptLinkStart.x, ptLinkStart.y, MIN( pt.x, rc.right ), pt.y + tm.tmHeight );
      if ( bInLink && iLinkIndex < nLinkRects ) {
        CStdString * pStr1 = ( CStdString* ) ( sLinks + iLinkIndex - 1 );
        CStdString *pStr2 = ( CStdString* ) ( sLinks + iLinkIndex );
        *pStr2 = *pStr1;
      }
      if ( ( uStyle & DT_SINGLELINE ) != 0 )
        break;
      if ( *pstrText == _T( '\n' ) )
        pstrText++;
      pt.x = rc.left;
      pt.y += cyLine /* - tm.tmDescent*/;
      if ( pt.y > rc.bottom && bDraw )
        break;
      ptLinkStart = pt;
      cyLine = tm.tmHeight + tm.tmExternalLeading;
      if ( pt.x >= rc.right )
        break;
      while ( *pstrText == _T( ' ' ) )
        pstrText++;
    } else if ( ( *pstrText == _T( '<' ) || *pstrText == _T( '{' ) )
      && ( pstrText[ 1 ] >= _T( 'a' ) && pstrText[ 1 ] <= _T( 'z' ) )
      && ( pstrText[ 2 ] == _T( ' ' ) || pstrText[ 2 ] == _T( '>' ) || pstrText[ 2 ] == _T( '}' ) ) ) {
      pstrText++;
      switch ( *pstrText ) {
      case _T( 'a' ) :                         // Link
        {
          pstrText++;
          if ( *pstrText == _T( ' ' ) )
            pstrText++;
          if ( iLinkIndex < nLinkRects ) {
            CStdString * pStr = ( CStdString* ) ( sLinks + iLinkIndex );
            while ( *pstrText != _T( '\0' ) && *pstrText != _T( '>' ) && *pstrText != _T( '}' ) ) {
              LPCTSTR pstrTemp = CharNext( pstrText );
              while ( pstrText < pstrTemp ) {
                *pStr += *pstrText++;
              }
            }
          }
          DWORD clrColor = bHoverLink ? pResource->GetDefaultLinkFontHoverColor() : pResource->GetDefaultLinkFontColor();
          SetTextColor( hDC, RGB( GetBValue( clrColor ), GetGValue( clrColor ), GetRValue( clrColor ) ) );
          SelectObject( hDC, pResource->GetDefaultLinkFont() );
          tm = pResource->GetDefaultLinkFontInfo();
          cyLine = MAX( cyLine, tm.tmHeight + tm.tmExternalLeading );
          ptLinkStart = pt;
          bInLink = true;
        }
        break;
      case _T( 'f' ) :                         // Font
        {
          pstrText++;
          int iFont = ( int ) _tcstol( pstrText, const_cast<LPTSTR*>( &pstrText ), 10 );
          SelectObject( hDC, pResource->GetFont( iFont ) );
          tm = pResource->GetFontInfo( iFont );
          cyLine = MAX( cyLine, tm.tmHeight + tm.tmExternalLeading );
        }
        break;
      case _T( 'b' ) :                         // Bold text
        {
          pstrText++;
          SelectObject( hDC, pResource->GetDefaultBoldFont() );
          tm = pResource->GetDefaultBoldFontInfo();
          cyLine = MAX( cyLine, tm.tmHeight + tm.tmExternalLeading );
        }
        break;
      case _T( 'x' ) :                         // Indent
        {
          pstrText++;
          int iWidth = ( int ) _tcstol( pstrText, const_cast<LPTSTR*>( &pstrText ), 10 );
          pt.x += iWidth;
          cxMaxWidth = MAX( cxMaxWidth, pt.x );
        }
        break;
      case _T( 'n' ) :                         // Newline
        {
          pstrText++;
          if ( ( uStyle & DT_SINGLELINE ) != 0 )
            break;
          if ( bInLink && iLinkIndex < nLinkRects )
            SetRect( &prcLinks[ iLinkIndex++ ], ptLinkStart.x, ptLinkStart.y, MIN( pt.x, rc.right ), pt.y + tm.tmHeight );
          if ( bInLink && iLinkIndex < nLinkRects ) {
            CStdString * pStr1 = ( CStdString* ) ( sLinks + iLinkIndex - 1 );
            CStdString *pStr2 = ( CStdString* ) ( sLinks + iLinkIndex );
            *pStr2 = *pStr1;
          }
          pt.x = rc.left;
          pt.y += cyLine /* - tm.tmDescent*/;
          if ( pt.y > rc.bottom && bDraw )
            break;
          ptLinkStart = pt;
          cyLine = tm.tmHeight + tm.tmExternalLeading;
          if ( pt.x >= rc.right )
            break;
        }
        break;
      case _T( 'p' ) :                         // Paragraph
        {
          pstrText++;
          pt.x = rc.right;
          cyLine = MAX( cyLine, tm.tmHeight + tm.tmExternalLeading ) + 5;
          SelectObject( hDC, UIResource_GetDefaultFont(pResource) );
          DWORD clrColor = pResource->GetDefaultFontColor();
          SetTextColor( hDC, RGB( GetBValue( clrColor ), GetGValue( clrColor ), GetRValue( clrColor ) ) );
          tm = pResource->GetDefaultFontInfo();
        }
        break;
      case _T( 'y' ) : {
        pstrText++;
        cyLine = ( int ) _tcstol( pstrText, const_cast<LPTSTR*>( &pstrText ), 10 );
                       }
        break;
      case _T( 'i' ) :                         // Image
        {
          pstrText++;
          int iWidth = 0;
          int iHeight = 0;
          if ( *pstrText == _T( ' ' ) )
            pstrText++;
          TImageInfo* pImageInfo = NULL;
          CStdString sName;
          int imageListIndex = -1;
          while ( *pstrText != _T( '\0' ) && *pstrText != _T( '>' ) && *pstrText != _T( '}' ) && *pstrText != _T( ' ' ) ) {
            LPCTSTR pstrTemp = CharNext( pstrText );
            while ( pstrText < pstrTemp ) {
              sName += *pstrText++;
            }
          }
          if ( *pstrText == _T( ' ' ) )
            pstrText++;
          if ( isdigit( *pstrText ) ) {
            imageListIndex = ( int ) _tcstol( pstrText, const_cast<LPTSTR*>( &pstrText ), 10 );
          }
          if ( !sName.IsEmpty() )
            pImageInfo = pResource->GetImage( ( LPCTSTR ) sName );
          if ( pImageInfo ) {
            iWidth = pImageInfo->nX;
            iHeight = pImageInfo->nY;
            if ( imageListIndex != -1 ) {
              if ( imageListIndex >= pImageInfo->nX / pImageInfo->nY )
                imageListIndex = 0;
              iWidth = iHeight;
            }
            if ( pt.x + iWidth >= rc.right && iWidth <= rc.right - rc.left && ( uStyle & DT_SINGLELINE ) == 0 ) {
              if ( bInLink && iLinkIndex < nLinkRects )
                SetRect( &prcLinks[ iLinkIndex++ ], ptLinkStart.x, ptLinkStart.y, MIN( pt.x, rc.right ), pt.y + tm.tmHeight );
              if ( bInLink && iLinkIndex < nLinkRects ) {
                CStdString * pStr1 = ( CStdString* ) ( sLinks + iLinkIndex - 1 );
                CStdString *pStr2 = ( CStdString* ) ( sLinks + iLinkIndex );
                *pStr2 = *pStr1;
              }
              pt.x = rc.left;
              pt.y += cyLine /* - tm.tmDescent*/;
              if ( pt.y > rc.bottom && bDraw )
                break;
              ptLinkStart = pt;
            }
            if ( bDraw ) {
              CRect rcImage( pt.x, pt.y, pt.x + iWidth, pt.y + iHeight );
              CRect rcBmpPart( 0, 0, iWidth, iHeight );
              if ( imageListIndex != -1 ) {
                rcBmpPart.left = iWidth * imageListIndex;
                rcBmpPart.right = iWidth * ( imageListIndex + 1 );
              }
              CRect rcCorner( 0, 0, 0, 0 );
              DrawImage( hDC, pImageInfo->hBitmap, rcImage, rcImage, rcBmpPart, rcCorner, \
                pImageInfo->alphaChannel, 255 );
            }
            cyLine = MAX( iHeight, cyLine );
            pt.x += iWidth;
            cyMinHeight = pt.y + iHeight;
            cxMaxWidth = MAX( cxMaxWidth, pt.x );
          }
        }
        break;
      case _T( 'c' ) :                         // Color
        {
          pstrText++;
          if ( *pstrText == _T( ' ' ) )
            pstrText++;
          if ( *pstrText == _T( '#' ) )
            pstrText++;
          DWORD clrColor = _tcstol( pstrText, const_cast<LPTSTR*>( &pstrText ), 16 );
          SetTextColor( hDC, RGB( GetBValue( clrColor ), GetGValue( clrColor ), GetRValue( clrColor ) ) );
        }
        break;
      }
      while ( *pstrText != _T( '\0' ) && *pstrText != _T( '>' ) && *pstrText != _T( '}' ) )
        pstrText = CharNext( pstrText );
      pstrText = CharNext( pstrText );
    } else if ( ( *pstrText == _T( '<' ) || *pstrText == _T( '{' ) ) && pstrText[ 1 ] == _T( '/' ) ) {
      pstrText++;
      pstrText++;
      switch ( *pstrText ) {
      case _T( 'a' ) :
        pstrText++;
        if ( iLinkIndex < nLinkRects )
          SetRect( &prcLinks[ iLinkIndex++ ], ptLinkStart.x, ptLinkStart.y, MIN( pt.x, rc.right ), pt.y + tm.tmHeight + tm.tmExternalLeading );
        SetTextColor( hDC, RGB( GetBValue( dwTextColor ), GetGValue( dwTextColor ), GetRValue( dwTextColor ) ) );
        SelectObject( hDC, UIResource_GetDefaultFont(pResource) );
        tm = pResource->GetDefaultFontInfo();
        bInLink = false;
        break;
      case _T( 'f' ) :
      case _T( 'b' ) :
        pstrText++;
        SelectObject( hDC, UIResource_GetDefaultFont(pResource) );
        tm = pResource->GetDefaultFontInfo();
        break;
      case _T( 'c' ) :
        pstrText++;
        SetTextColor( hDC, RGB( GetBValue( dwTextColor ), GetGValue( dwTextColor ), GetRValue( dwTextColor ) ) );
        break;
      }
      while ( *pstrText != _T( '\0' ) && *pstrText != _T( '>' ) && *pstrText != _T( '}' ) )
        pstrText = CharNext( pstrText );
      pstrText = CharNext( pstrText );
    } else if ( *pstrText == _T( '<' ) && pstrText[ 2 ] == _T( '>' ) && ( pstrText[ 1 ] == _T( '{' ) || pstrText[ 1 ] == _T( '}' ) ) ) {
      SIZE szSpace = { 0 };
      GetTextExtentPoint32( hDC, &pstrText[ 1 ], 1, &szSpace );
      if ( bDraw )
        TextOut( hDC, pt.x, pt.y, &pstrText[ 1 ], 1 );
      pt.x += szSpace.cx;
      cxMaxWidth = MAX( cxMaxWidth, pt.x );
      pstrText++;
      pstrText++;
      pstrText++;
    } else if ( *pstrText == _T( '{' ) && pstrText[ 2 ] == _T( '}' ) && ( pstrText[ 1 ] == _T( '<' ) || pstrText[ 1 ] == _T( '>' ) ) ) {
      SIZE szSpace = { 0 };
      GetTextExtentPoint32( hDC, &pstrText[ 1 ], 1, &szSpace );
      if ( bDraw )
        TextOut( hDC, pt.x, pt.y, &pstrText[ 1 ], 1 );
      pt.x += szSpace.cx;
      cxMaxWidth = MAX( cxMaxWidth, pt.x );
      pstrText++;
      pstrText++;
      pstrText++;
    } else if ( *pstrText == _T( ' ' ) ) {
      SIZE szSpace = { 0 };
      GetTextExtentPoint32( hDC, _T( " " ), 1, &szSpace );
      // Still need to paint the space because the font might have
      // underline formatting.
      if ( bDraw )
        TextOut( hDC, pt.x, pt.y, _T( " " ), 1 );
      pt.x += szSpace.cx;
      cxMaxWidth = MAX( cxMaxWidth, pt.x );
      pstrText++;
    } else {
      POINT ptPos = pt;
      int cchChars = 0;
      int cchSize = 0;
      int cchLastGoodWord = 0;
      int cchLastGoodSize = 0;
      LPCTSTR p = pstrText;
      SIZE szText = { 0 };
      if ( *p == _T( '<' ) || *p == _T( '{' ) )
        p++, cchChars++, cchSize++;
      while ( *p != _T( '\0' ) && *p != _T( '<' ) && *p != _T( '{' ) && *p != _T( '\n' ) ) {
        // This part makes sure that we're word-wrapping if needed or providing support
        // for DT_END_ELLIPSIS. Unfortunately the GetTextExtentPoint32() call is pretty
        // slow when repeated so often.
        // TODO: Rewrite and use GetTextExtentExPoint() instead!
        LPCTSTR pstrTemp = CharNext( p );
        cchChars++;
        cchSize += ( int ) ( pstrTemp - p );
        szText.cx = cchChars * tm.tmMaxCharWidth;
        if ( pt.x + szText.cx >= rc.right ) {
          GetTextExtentPoint32( hDC, pstrText, cchSize, &szText );
        }
        if ( pt.x + szText.cx > rc.right ) {
          if ( pt.x + szText.cx > rc.right && pt.x != rc.left ) {
            cchChars--;
            cchSize -= ( int ) ( pstrTemp - p );
          }
          if ( ( uStyle & DT_WORDBREAK ) != 0 && cchLastGoodWord > 0 ) {
            cchChars = cchLastGoodWord;
            cchSize = cchLastGoodSize;
          }
          if ( ( uStyle & DT_END_ELLIPSIS ) != 0 && cchChars > 2 ) {
            cchChars = cchLastGoodWord;
            cchSize = cchLastGoodSize;
          }
          pt.x = rc.right;
          cxMaxWidth = MAX( cxMaxWidth, pt.x );
          break;
        }
        if ( !( ( p[ 0 ] >= _T( 'a' ) && p[ 0 ] <= _T( 'z' ) ) || ( p[ 0 ] >= _T( 'A' ) && p[ 0 ] <= _T( 'Z' ) ) ) ) {
          cchLastGoodWord = cchChars;
          cchLastGoodSize = cchSize;
        }
        if ( *p == _T( ' ' ) ) {
          cchLastGoodWord = cchChars;
          cchLastGoodSize = cchSize;
        }
        p = CharNext( p );
      }
      if ( cchChars > 0 ) {
        GetTextExtentPoint32( hDC, pstrText, cchSize, &szText );
        if ( bDraw ) {
          TextOut( hDC, ptPos.x, ptPos.y, pstrText, cchSize );
          if ( pt.x >= rc.right && ( uStyle & DT_END_ELLIPSIS ) != 0 )
            TextOut( hDC, rc.right - 10, ptPos.y, _T( "..." ), 3 );
        }
        pt.x += szText.cx;
        cxMaxWidth = MAX( cxMaxWidth, pt.x );
        pstrText += cchSize;
      }
    }
    ASSERT( iLinkIndex <= nLinkRects );
  }
  }
  // Clear remaining link rects and return number of used rects
  for ( i = iLinkIndex; i < nLinkRects; i++ )
    ZeroMemory( prcLinks + i, sizeof( RECT ) );
  *pLinkRects = iLinkIndex;
  // Return size of text when requested
  if ( ( uStyle & DT_CALCRECT ) != 0 ) {
    rc.bottom = MAX( cyMinHeight, pt.y + cyLine );
    rc.right = MIN( rc.right, cxMaxWidth );
  }
  if ( bDraw )
    SelectClipRgn( hDC, hOldRgn );
  DeleteObject( hOldRgn );
  DeleteObject( hRgn );
  SelectObject( hDC, hOldFont );
#endif
}


#if 0
  class UILIB_API CRenderClip {
  public:
    RECT rcItem;
    HDC hDC;
    HRGN hRgn;
    HRGN hOldRgn;
    ~CRenderClip() {
      ASSERT( GetObjectType( hDC ) == OBJ_DC || GetObjectType( hDC ) == OBJ_MEMDC );
      ASSERT( GetObjectType( hRgn ) == OBJ_REGION );
      ASSERT( GetObjectType( hOldRgn ) == OBJ_REGION );
      SelectClipRgn( hDC, hOldRgn );
      DeleteObject( hOldRgn );
      DeleteObject( hRgn );
    }
    static void GenerateClip( HDC hDC, RECT rcItem, CRenderClip& clip ) {
      RECT rcClip = { 0 };
      GetClipBox( hDC, &rcClip );
      clip.hOldRgn = CreateRectRgnIndirect( &rcClip );
      clip.hRgn = CreateRectRgnIndirect( &rcItem );
      ExtSelectClipRgn( hDC, clip.hRgn, RGN_AND );
      clip.hDC = hDC;
      clip.rcItem = rcItem;
    }
    static void UseOldClipBegin( HDC hDC, CRenderClip& clip ) {
      SelectClipRgn( hDC, clip.hOldRgn );
    }
    static void UseOldClipEnd( HDC hDC, CRenderClip& clip ) {
      SelectClipRgn( hDC, clip.hRgn );
    }
  };
#endif
