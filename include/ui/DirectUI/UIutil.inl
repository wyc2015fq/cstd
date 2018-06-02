
#include "PtrArray.inl"
#include "StringPtrMap.inl"

#define DeflateRect( lprc, cx, cy ) InflateRect( lprc, -(cx), -(cy) )

HGDIOBJ SelectStockObject( HDC m_hDC, int nIndex ) {
  HGDIOBJ hObject = GetStockObject( nIndex );
  HGDIOBJ hOldObj = NULL;
  ASSERT( m_hDC != NULL );
  ASSERT( hObject != NULL );
  hOldObj = SelectObject( m_hDC, hObject );
  return ( hOldObj );
}
void DeflateRect4( LPRECT lprc, int l, int t, int r, int b ) {
  lprc->left += l, lprc->top += t, lprc->right -= r, lprc->bottom -= b;
}
LPTSTR SmallFormat( LPCTSTR pstrFormat, ... ) {
  int iRet;
  TCHAR szBuffer[ 256 ] = { 0 };
  va_list argList;
  va_start( argList, pstrFormat );
  iRet = wvsprintf( szBuffer, pstrFormat, argList );
  va_end( argList );
  return strdup(szBuffer);
}
#define RectOffSet(src, xoff, yoff)  ((rc).left += xoff, (rc).top += yoff, (rc).right += xoff, (rc).bottom += yoff)

static BOOL MessageError() {
  LPVOID lpMsgBuf;
  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),                                               // Default language
    ( LPTSTR ) & lpMsgBuf,
    0,
    NULL
    );
  MessageBox( NULL, ( LPCTSTR ) lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
  LocalFree( lpMsgBuf );
  return 0;
}

static POINT cPOINT( int initX, int initY ) {
  POINT pt;
  pt.x = initX, pt.y = initY;
  return pt;
}
#define cPOINT1(dwPoint)  cPOINT(LOWORD( dwPoint ), HIWORD( dwPoint ))

static RECT iRECT( int l, int t, int r, int b ) {
  RECT rt;
  rt.left = l, rt.top = t, rt.right = r, rt.bottom = b;
  return rt;
}
static SIZE iSIZE( int initCX, int initCY ) {
  SIZE sz;
  sz.cx = initCX, sz.cy = initCY;
  return sz;
}
POINT CenterPoint( const RECT* rect ) {
  return cPOINT( ( ( rect ) ->left + ( rect ) ->right ) / 2, ( ( rect ) ->top + ( rect ) ->bottom ) / 2 );
}
void make_bitmap_info_header( int iWidth, int iHeight, int dd, BITMAPINFOHEADER* bmih ) {
  bmih->biSize = sizeof( BITMAPINFOHEADER );
  bmih->biWidth = iWidth;
  bmih->biHeight = iHeight;
  bmih->biPlanes = 1;
  bmih->biBitCount = ( unsigned short ) dd;
  bmih->biCompression = BI_RGB;
  // 每行数据存储空间的四字节对齐
  bmih->biSizeImage = ( ( iWidth * dd / 8 + 3 ) & ~3 ) * iHeight;
  bmih->biXPelsPerMeter = 0;
  bmih->biYPelsPerMeter = 0;
  bmih->biClrUsed = 0;
  bmih->biClrImportant = 0;
}
#define  BMPMARK(A)  (((A)+3)&~3)

void make_bitmap_rgbquad( RGBQUAD pRGB[], int n ) {
  int i;
  for ( i = 0; i < n; ++i ) {
    pRGB->rgbBlue = pRGB->rgbGreen = pRGB->rgbRed = ( BYTE ) i;
    pRGB->rgbReserved = 0;
    pRGB++;
  }
}

// 矩阵拷贝  dst[n×sl] = src[n×dl]
void mat_copy( int h, int w, void* dst, int dl, const void* src, int sl ) {
  char * cdst, * csrc;
  cdst = ( char * ) dst;
  csrc = ( char * ) src;
  for ( ; h--; cdst += dl, csrc += sl ) {
    memcpy( cdst, csrc, w );
  }
}
