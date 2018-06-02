
#define CHTMLBitmaps_GetAt(me, nIndex)  (me)->m_ppBitmaps[nIndex]
#define CHTMLBitmaps_GetCount(me)  (me)->m_nBitmaps

void CHTMLBitmaps_ThTimer(CHTMLBitmaps* me);

int WINAPI ThTimerFunc( CHTMLBitmaps* pBitmaps ) {
  CHTMLBitmaps_ThTimer(pBitmaps);
  return 0;
}
CHTMLBitmaps* CreateCHTMLBitmaps( CHTMLProgress *pProgress ) {
  HDC hDC;
  HDC dcEmpty;
  HBITMAP bmOld;
  DWORD dw;
  RECT r = {0, 0, 60, 40};
  CHTMLBitmaps* me = (CHTMLBitmaps*)malloc(sizeof(CHTMLBitmaps));
  me->m_pProgress = pProgress;
  //me->m_pViewer = NULL;
  me->m_ppBitmaps = NULL;
  me->m_nBitmaps = 0;
  me->m_bStop = FALSE;
  me->m_nDownloads = 0;
  me->m_hMutex = CreateMutex( NULL, 0, NULL );
  hDC = GetDC( NULL );
  me->m_bmEmpty = CreateCompatibleBitmap( hDC, r.right, r.bottom );
  dcEmpty = CreateCompatibleDC( hDC );
  bmOld = ( HBITMAP ) SelectObject( dcEmpty, me->m_bmEmpty );
  FillRect( dcEmpty, &r, ( HBRUSH ) GetStockObject( WHITE_BRUSH ) );
  MoveToEx( dcEmpty, 0, 0, NULL );
  LineTo( dcEmpty, r.right - 1, 0 );
  LineTo( dcEmpty, r.right - 1, r.bottom - 1 );
  LineTo( dcEmpty, 0, r.bottom - 1 );
  LineTo( dcEmpty, 0, 0 );
  LineTo( dcEmpty, r.right - 1, r.bottom - 1 );
  MoveToEx( dcEmpty, r.right - 1, 0, NULL );
  LineTo( dcEmpty, 0, r.bottom - 1 );
  SelectObject( dcEmpty, bmOld );
  DeleteDC( dcEmpty );
  ReleaseDC( NULL, hDC );
  CloseHandle( CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) ThTimerFunc, ( LPVOID ) me, 0, &dw ) );
  return me;
}
//void CHTMLBitmaps_SetViewer( CHTMLBitmaps* me, CHTMLViewer *pViewer ) {
  //me->m_pViewer = pViewer;
//}
void CHTMLBitmaps_Clear(CHTMLBitmaps* me) {
  int i;
  for ( i = 0;i < me->m_nBitmaps;i++ )
    DeleteCHTMLBitmap(me->m_ppBitmaps[ i ]);
  free( me->m_ppBitmaps );
}
void CHTMLBitmaps_ThTimer(CHTMLBitmaps* me) {
  int i;
  while ( !me->m_bStop ) {
    Sleep( 10 );
    if ( me->m_bStop )
      break;
    //if ( me->m_pViewer->IsReading() )
    //  continue;
    WaitForSingleObject( me->m_hMutex, INFINITE );
    for ( i = 0;i < me->m_nBitmaps;i++ )
      if (  CHTMLBitmap_IsCreated(me->m_ppBitmaps[ i ]) && CHTMLBitmap_TimerTick(me->m_ppBitmaps[ i ]) ) {
        //me->m_pViewer->UpdateImage( i );
      }
      ReleaseMutex( me->m_hMutex );
  }
  me->m_bStop = FALSE;
}
int CHTMLBitmaps_AddBitmap( CHTMLBitmaps* me, char *szImageName, BOOL bBackground ) {
  int i, a;
  WaitForSingleObject( me->m_hMutex, INFINITE );
  i = me->m_nBitmaps++;
  me->m_ppBitmaps = ( CHTMLBitmap** ) realloc( me->m_ppBitmaps, sizeof( CHTMLBitmap ) * me->m_nBitmaps );
  me->m_ppBitmaps[ i ] = CreateCHTMLBitmap( szImageName, me->m_pProgress, me, bBackground );
  if ( me->m_nBitmaps % 10 == 0 ) {
    for ( a = 0;a < me->m_nBitmaps;a++ )
      if ( !CHTMLBitmap_IsLoaded(me->m_ppBitmaps[ a ]) )
        CHTMLBitmap_GetBitmap(me->m_ppBitmaps[ a ], FALSE);
  }
  ReleaseMutex( me->m_hMutex );
  return i;
}
void CHTMLBitmaps_ImageLoaded(CHTMLBitmaps* me) {
  int i;
  int nDone = 0;
  WaitForSingleObject( me->m_hMutex, INFINITE );
  for ( i = 0;i < me->m_nBitmaps;i++ )
    if ( CHTMLBitmap_IsLoaded(me->m_ppBitmaps[ i ]) )
      nDone++;
    me->m_nDownloads--;
    ReleaseMutex( me->m_hMutex );
    if ( me->m_pProgress )
      me->m_pProgress->vtable->ImageLoaded( me->m_pProgress, nDone, me->m_nBitmaps );
}
void CHTMLBitmaps_Stop(CHTMLBitmaps* me) {
  me->m_bStop = TRUE;
  while ( me->m_bStop )
    Sleep( 10 );
}
void CHTMLBitmaps_WaitForDownload(CHTMLBitmaps* me) {
  while ( 1 ) {
    WaitForSingleObject( me->m_hMutex, INFINITE );
    if ( me->m_nDownloads > 10 ) {
      ReleaseMutex( me->m_hMutex );
      Sleep( 20 );
      continue;
    }
    me->m_nDownloads++;
    ReleaseMutex( me->m_hMutex );
    break;
  }
}
