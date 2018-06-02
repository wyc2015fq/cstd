
#include <Mmsystem.h>
#pragma comment (lib, "Winmm.lib") // for Multimedia Timers.

static void CALLBACK TimeProc( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 ) {
  TIMERINFO* pTimer = (TIMERINFO*)dwUser;
  CWindowWnd* ww= pTimer->ww;
  TEventUI event = { 0 };
  event.Type = UIEVENT_TIMER;
  event.wParam = pTimer->nLocalID;
  event.dwTimestamp =  GetTickCount();
  Event( pTimer->pSender, event );
  return ;
}
//
bool UISetTimer( CWindowWnd* ww, CControlUI* ctr, UINT id, int time /*ms*/ ) {
  int i;
  UINT m_uTimerID;
  TIMERINFO* pTimer;
  ASSERT( ctr != NULL );
  ASSERT( time > 0 );
  for ( i = 0; i < PtrArray_GetSize(ww->m_aTimers); i++ ) {
    TIMERINFO* pTimer = (TIMERINFO*)PtrArray_GetAt( ww->m_aTimers, i );
    if ( pTimer->pSender == ctr
      && pTimer->hWnd == ww->m_hWnd
      && pTimer->nLocalID == id ) {
      if ( pTimer->bKilled == true ) {
        pTimer->m_uTimerID = timeSetEvent( time, 0, TimeProc, ( DWORD ) pTimer, TIME_PERIODIC );
        if ( pTimer->m_uTimerID ) {
          pTimer->bKilled = false;
          return true;
        }
        return false;
      }
      return false;
    }
  }
  ww->m_uTimerID = ( ++(ww->m_uTimerID )) % 0xFF;
  i = PtrArray_GetSize(ww->m_aTimers);
  pTimer = NEW(TIMERINFO, 1);
  m_uTimerID = timeSetEvent( time, 0, TimeProc, ( DWORD )pTimer, TIME_PERIODIC );
  if ( !m_uTimerID ) {
    free(pTimer);
    return FALSE;
  }
  if ( pTimer == NULL )
    return FALSE;
  pTimer->m_uTimerID = m_uTimerID;
  pTimer->hWnd = ww->m_hWnd;
  pTimer->pSender = ctr;
  pTimer->nLocalID = id;
  pTimer->uWinTimer = ww->m_uTimerID;
  pTimer->bKilled = false;
  pTimer->ww = ww;
  return PtrArray_Add( ww->m_aTimers, pTimer );
}
#define KILLTIMER(pTimer) timeKillEvent(pTimer->m_uTimerID )
bool UIKillTimer( CWindowWnd* ww, CControlUI* pControl, UINT nTimerID ) {
  int i;
  ASSERT( pControl != NULL );
  for ( i = 0; i < PtrArray_GetSize(ww->m_aTimers); i++ ) {
    TIMERINFO* pTimer = (TIMERINFO*)( PtrArray_GetAt( ww->m_aTimers, i ) );
    if ( pTimer->pSender == pControl
      && pTimer->hWnd == ww->m_hWnd
      && pTimer->nLocalID == nTimerID ) {
      if ( pTimer->bKilled == false ) {
        //KillTimer( pTimer->hWnd, pTimer->uWinTimer );
        timeKillEvent( pTimer->m_uTimerID );
        pTimer->bKilled = true;
        return true;
      }
    }
  }
  return false;
}