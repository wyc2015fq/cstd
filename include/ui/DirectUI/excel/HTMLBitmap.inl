
#define CHTMLBitmap_Width(me)  (me)->m_Size.cx
#define CHTMLBitmap_Height(me)  (me)->m_Size.cy
#define CHTMLBitmap_GetImageName(me)  (me)->m_szImageName
#define CHTMLBitmap_IsEmptyBitmap(me)  (me)->m_bEmptyBitmap
#define CHTMLBitmap_GetBackground(me)  (me)->m_bBackground
#define CHTMLBitmap_IsLoaded(me)  (me)->m_bLoaded
#define CHTMLBitmap_IsCreated(me)  ((me)->m_bmBmp ? TRUE : FALSE)

void CHTMLBitmap_LoadBitmap(CHTMLBitmap* p);
void ThHTMLBitmap( CHTMLBitmap *p ) {
  CHTMLBitmap_LoadBitmap(p);
}
CHTMLBitmap* CreateCHTMLBitmap( char *szImageName, CHTMLProgress* pProgress, CHTMLBitmaps *pParent, BOOL bBackground ) {
  CHTMLBitmap* me = (CHTMLBitmap*)malloc(sizeof(CHTMLBitmap));
  DWORD dw;
  HANDLE hThread;
  me->m_pProgress = pProgress;
  me->m_pParent = pParent;
  me->m_bLoaded = FALSE;
  me->m_nCounter = 0;
  me->m_bBackground = bBackground;
  me->m_bEmptyBitmap = FALSE;
  me->m_bmBmp = NULL;
  me->m_szImageName = (char*)malloc(strlen( szImageName ) + 1 );
  strcpy( me->m_szImageName, szImageName );
  me->m_szImageData = NULL;
  me->m_nImageData = 0;
  me->m_Size.cx = me->m_Size.cy = 0;
  me->m_hDone = CreateEvent( NULL, TRUE, 0, NULL );
  hThread = CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) ThHTMLBitmap, ( LPVOID ) me, 0, &dw );
  CloseHandle( hThread );
  return me;
}
void CHTMLBitmap_DestroyBitmap(CHTMLBitmap* me) {
  if ( me->m_bmBmp && me->m_bmBmp != GetEmptyBitmap(me->m_pParent) )
    DeleteObject( me->m_bmBmp );
  me->m_bmBmp = NULL;
}
void CHTMLBitmap_Clear(CHTMLBitmap* me) {
  while ( !me->m_bLoaded )
    Sleep( 10 );
  CHTMLBitmap_DestroyBitmap(me);
  free(me->m_szImageName);
  if ( me->m_szImageData )
    free(me->m_szImageData);
  if ( me->m_hDone )
    CloseHandle( me->m_hDone );
}
void DeleteCHTMLBitmap(CHTMLBitmap* me) {
  CHTMLBitmap_Clear(me);
  free(me);
}
void CHTMLBitmaps_WaitForDownload(CHTMLBitmaps* me);
void CHTMLBitmaps_ImageLoaded(CHTMLBitmaps* me);
void CHTMLBitmap_LoadBitmap(CHTMLBitmap* me) {
  CHTMLBitmaps_WaitForDownload(me->m_pParent);
  if ( me->m_pProgress )
    me->m_szImageData = me->m_pProgress->vtable->ReadHTTPData( me->m_pProgress, me->m_szImageName, &me->m_nImageData );
  SetEvent( me->m_hDone );
  me->m_bLoaded = TRUE;
  CHTMLBitmaps_ImageLoaded(me->m_pParent);
}
BOOL CHTMLBitmap_GetBitmap( CHTMLBitmap* me, BOOL bWaitForImage ) {
  if ( me->m_bmBmp )
    return TRUE;
  if ( me->m_hDone && WaitForSingleObject( me->m_hDone, bWaitForImage ? INFINITE : 0 ) != WAIT_TIMEOUT ) {
    CloseHandle( me->m_hDone );
    me->m_hDone = NULL;
  } else if ( me->m_hDone )
    return FALSE;
  if ( me->m_szImageData )
  {
    BITMAP bm;
    HBITMAP bmBmp, bmBmpOld, bmRetOld;
    HDC dcBmp, dcRet, hDC;
    HRESULT hr;
    IStream *pStream;
    IPicture *pPicture;
    HANDLE hMem;
    LPTSTR pszData;
    hMem = GlobalAlloc( GMEM_FIXED, me->m_nImageData );
    pszData = ( LPTSTR ) GlobalLock( hMem );
    memcpy( pszData, me->m_szImageData, me->m_nImageData );
    if ( FAILED( hr = CreateStreamOnHGlobal( hMem, FALSE, &pStream ) ) ) {
      GlobalFree( hMem );
      return 0;
    }
    hr = OLELOADPICTURE( pStream, me->m_nImageData, TRUE, IID_IPicture, pPicture );
    OLECALL0( pStream, Release );
    GlobalFree( hMem );
    if ( !FAILED( hr ) ) {
      if ( !FAILED( hr = OLECALL1(pPicture, get_Handle,  ( OLE_HANDLE* ) &bmBmp ) ) ) {
        GetObject( bmBmp, sizeof( BITMAP ), &bm );
        hDC = GetDC( NULL );
        me->m_bmBmp = CreateCompatibleBitmap( hDC, bm.bmWidth, bm.bmHeight );
        dcBmp = CreateCompatibleDC( hDC );
        dcRet = CreateCompatibleDC( hDC );
        bmBmpOld = ( HBITMAP ) SelectObject( dcBmp, bmBmp );
        bmRetOld = ( HBITMAP ) SelectObject( dcRet, me->m_bmBmp );
        BitBlt( dcRet, 0, 0, bm.bmWidth, bm.bmHeight, dcBmp, 0, 0, SRCCOPY );
        SelectObject( dcBmp, bmBmpOld );
        SelectObject( dcRet, bmRetOld );
        DeleteDC( dcBmp );
        DeleteDC( dcRet );
        ReleaseDC( NULL, hDC );
      }
    }
    if ( pPicture )
      OLECALL0(pPicture, Release);
  }
  if ( !me->m_bmBmp )
    me->m_bmBmp = GetEmptyBitmap(me->m_pParent);
  
  {
    BITMAP bm = {0};
    GetObject( me->m_bmBmp, sizeof( BITMAP ), &bm );
    me->m_Size.cx = bm.bmWidth;
    me->m_Size.cy = bm.bmHeight;
  }
  return TRUE;
}
void CHTMLBitmap_GetSize( CHTMLBitmap* me, SIZE* psc ) {
  SIZE sc = *psc;
  if ( sc.cx && !sc.cy && me->m_Size.cx )
    sc.cy = me->m_Size.cy * sc.cx / me->m_Size.cx;
  else if ( !sc.cx && sc.cy && me->m_Size.cy )
    sc.cx = me->m_Size.cx * sc.cy / me->m_Size.cy;
  if ( !sc.cx )
    sc.cx = me->m_Size.cx;
  if ( !sc.cy )
    sc.cy = me->m_Size.cy;
  *psc = sc;
}
void CHTMLBitmap_BitBlt( CHTMLBitmap* me, HDC hDC, int x, int y, int wx, int wy ) {
  HDC dcBmp = CreateCompatibleDC( hDC );
  HBITMAP bmOld = ( HBITMAP ) SelectObject( dcBmp, me->m_bmBmp );
  BitBlt( hDC, x, y, wx, wy, dcBmp, 0, 0, SRCCOPY );
  SelectObject( dcBmp, bmOld );
  DeleteDC( dcBmp );
}
void CHTMLBitmap_StretchBlt( CHTMLBitmap* me, HDC hDC, int x, int y, int wx, int wy ) {
  HDC dcBmp = CreateCompatibleDC( hDC );
  HBITMAP bmOld = ( HBITMAP ) SelectObject( dcBmp, me->m_bmBmp );
  StretchBlt( hDC, x, y, wx, wy, dcBmp, 0, 0, me->m_Size.cx, me->m_Size.cy, SRCCOPY );
  SelectObject( dcBmp, bmOld );
  DeleteDC( dcBmp );
}
int CHTMLBitmap_TimerTick(CHTMLBitmap* me) {
  return 0;
}
