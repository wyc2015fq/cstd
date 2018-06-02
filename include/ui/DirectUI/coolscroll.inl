#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#define FLAT_SCROLLBARS
//cool scrollbar styles for Flat scrollbars
#define CSBS_NORMAL                     0
#define CSBS_FLAT                       1
#define CSBS_HOTTRACKED         2
// Draw a standard scrollbar arrow
static int DrawScrollArrow( UINT nBarType, UINT fFlatScrollbar, HDC hdc, RECT *rect, UINT arrow, BOOL fMouseDown, BOOL fMouseOver ) {
  UINT ret;
  UINT flags = arrow;
  //HACKY bit so this routine can be called by vertical and horizontal code
  if ( nBarType == SB_VERT ) {
    if ( flags & DFCS_SCROLLLEFT )
      flags = flags & ~DFCS_SCROLLLEFT | DFCS_SCROLLUP;
    if ( flags & DFCS_SCROLLRIGHT )
      flags = flags & ~DFCS_SCROLLRIGHT | DFCS_SCROLLDOWN;
  }
  if ( fMouseDown )
    flags |= ( DFCS_FLAT | DFCS_PUSHED );
#ifdef FLAT_SCROLLBARS
  if ( fFlatScrollbar != CSBS_NORMAL ) {
    HDC hdcmem1, hdcmem2;
    HBITMAP hbm1, oldbm1;
    HBITMAP hbm2, oldbm2;
    RECT rc;
    int width, height;
    rc = *rect;
    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
    SetRect( &rc, 0, 0, width, height );
    //MONOCHROME bitmap to convert the arrow to black/white mask
    hdcmem1 = CreateCompatibleDC( hdc );
    hbm1 = CreateBitmap( width, height, 1, 1, NULL );
    UnrealizeObject( hbm1 );
    oldbm1 = SelectObject( hdcmem1, hbm1 );
    //NORMAL bitmap to draw the arrow into
    hdcmem2 = CreateCompatibleDC( hdc );
    hbm2 = CreateCompatibleBitmap( hdc, width, height );
    UnrealizeObject( hbm2 );
    oldbm2 = SelectObject( hdcmem2, hbm2 );
    flags = flags & ~DFCS_PUSHED | DFCS_FLAT;       //just in case
    DrawFrameControl( hdcmem2, &rc, DFC_SCROLL, flags );
#ifndef HOT_TRACKING
    if ( fMouseDown ) {
      //uncomment these to make the cool scrollbars
      //look like the common controls flat scrollbars
      //fMouseDown = FALSE;
      //fMouseOver = TRUE;
    }
#endif
    //draw a flat monochrome version of a scrollbar arrow (dark)
    if ( fMouseDown ) {
      SetBkColor( hdcmem2, GetSysColor( COLOR_BTNTEXT ) );
      BitBlt( hdcmem1, 0, 0, width, height, hdcmem2, 0, 0, SRCCOPY );
      SetBkColor( hdc, 0x00ffffff );
      SetTextColor( hdc, GetSysColor( COLOR_3DDKSHADOW ) );
      BitBlt( hdc, rect->left, rect->top, width, height, hdcmem1, 0, 0, SRCCOPY );
    }
    //draw a flat monochrome version of a scrollbar arrow (grey)
    else if ( fMouseOver ) {
      SetBkColor( hdcmem2, GetSysColor( COLOR_BTNTEXT ) );
      FillRect( hdcmem1, &rc, GetStockObject( WHITE_BRUSH ) );
      BitBlt( hdcmem1, 0, 0, width, height, hdcmem2, 0, 0, SRCINVERT );
      SetBkColor( hdc, GetSysColor( COLOR_3DSHADOW ) );
      SetTextColor( hdc, 0x00ffffff );
      BitBlt( hdc, rect->left, rect->top, width, height, hdcmem1, 0, 0, SRCCOPY );
    }
    //draw the arrow normally
    else {
      BitBlt( hdc, rect->left, rect->top, width, height, hdcmem2, 0, 0, SRCCOPY );
    }
    SelectObject( hdcmem1, oldbm1 );
    SelectObject( hdcmem2, oldbm2 );
    DeleteObject( hbm1 );
    DeleteObject( hbm2 );
    DeleteDC( hdcmem1 );
    DeleteDC( hdcmem2 );
    ret = 0;
  } else
#endif
    ret = DrawFrameControl( hdc, rect, DFC_SCROLL, flags );
  return ret;
}
static COLORREF GetSBForeColor( void ) {
  COLORREF c1 = GetSysColor( COLOR_3DHILIGHT );
  COLORREF c2 = GetSysColor( COLOR_WINDOW );
  if ( c1 != 0xffffff && c1 == c2 ) {
    return GetSysColor( COLOR_BTNFACE );
  } else {
    return GetSysColor( COLOR_3DHILIGHT );
  }
}
static COLORREF GetSBBackColor( void ) {
  return GetSysColor( COLOR_SCROLLBAR );
}
// Paint a checkered rectangle, with each alternate
// pixel being assigned a different colour
static void DrawCheckedRect( HDC hdc, RECT *rect, COLORREF fg, COLORREF bg ) {
  static WORD wCheckPat[ 8 ] = { 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555 };
  HBITMAP hbmp;
  HBRUSH hbr, hbrold;
  COLORREF fgold, bgold;
  hbmp = CreateBitmap( 8, 8, 1, 1, wCheckPat );
  hbr = CreatePatternBrush( hbmp );
  UnrealizeObject( hbr );
  SetBrushOrgEx( hdc, rect->left, rect->top, 0 );
  hbrold = ( HBRUSH ) SelectObject( hdc, hbr );
  fgold = SetTextColor( hdc, fg );
  bgold = SetBkColor( hdc, bg );
  PatBlt( hdc, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, PATCOPY );
  SetBkColor( hdc, bgold );
  SetTextColor( hdc, fgold );
  SelectObject( hdc, hbrold );
  DeleteObject( hbr );
  DeleteObject( hbmp );
}
// Fill the specifed rectangle using a solid colour
static void PaintRect( HDC hdc, RECT *rect, COLORREF color ) {
  COLORREF oldcol = SetBkColor( hdc, color );
  ExtTextOut( hdc, 0, 0, ETO_OPAQUE, rect, _T( "" ), 0, 0 );
  SetBkColor( hdc, oldcol );
}
// Draw a simple blank scrollbar push-button. Can be used
// to draw a push button, or the scrollbar thumb
// drawflag - could set to BF_FLAT to make flat scrollbars
void DrawBlankButton( HDC hdc, const RECT *rect, UINT drawflag ) {
  RECT rc = *rect;
#ifndef FLAT_SCROLLBARS
  drawflag &= ~BF_FLAT;
#endif
  DrawEdge( hdc, &rc, EDGE_RAISED, BF_RECT | drawflag | BF_ADJUST );
  FillRect( hdc, &rc, GetSysColorBrush( COLOR_BTNFACE ) );
}
// Draw the specified bitmap centered in the rectangle
static void DrawImage( HDC hdc, HBITMAP hBitmap, RECT *rc ) {
  BITMAP bm;
  int cx;
  int cy;
  HDC memdc;
  HBITMAP hOldBM;
  RECT rcDest = *rc;
  POINT p;
  SIZE delta;
  COLORREF colorOld;
  if ( hBitmap == NULL )
    return ;
  // center bitmap in caller's rectangle
  GetObject( hBitmap, sizeof bm, &bm );
  cx = bm.bmWidth;
  cy = bm.bmHeight;
  delta.cx = ( rc->right - rc->left - cx ) / 2;
  delta.cy = ( rc->bottom - rc->top - cy ) / 2;
  if ( rc->right - rc->left > cx ) {
    SetRect( &rcDest, rc->left + delta.cx, rc->top + delta.cy, 0, 0 );
    rcDest.right = rcDest.left + cx;
    rcDest.bottom = rcDest.top + cy;
    p.x = 0;
    p.y = 0;
  } else {
    p.x = -delta.cx;
    p.y = -delta.cy;
  }
  // select checkmark into memory DC
  memdc = CreateCompatibleDC( hdc );
  hOldBM = ( HBITMAP ) SelectObject( memdc, hBitmap );
  // set BG color based on selected state
  colorOld = SetBkColor( hdc, GetSysColor( COLOR_3DFACE ) );
  BitBlt( hdc, rcDest.left, rcDest.top, rcDest.right - rcDest.left, rcDest.bottom - rcDest.top, memdc, p.x, p.y, SRCCOPY );
  // restore
  SetBkColor( hdc, colorOld );
  SelectObject( memdc, hOldBM );
  DeleteDC( memdc );
}
// Draw the specified metafile
static void DrawMetaFile( HDC hdc, HENHMETAFILE hemf, RECT *rect ) {
  RECT rc;
  POINT pt;
  SetRect( &rc, 0, 0, rect->right - rect->left, rect->bottom - rect->top );
  SetWindowOrgEx( hdc, -rect->left, -rect->top, &pt );
  PlayEnhMetaFile( hdc, hemf, &rc );
  SetWindowOrgEx( hdc, pt.x, pt.y, 0 );
}
