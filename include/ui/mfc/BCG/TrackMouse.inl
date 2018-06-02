
#include "BCGCBPro.h"
#include "TrackMouse.h"

VOID CALLBACK BCGPTrackMouseTimerProc (HWND hWnd, 
									  UINT /*uMsg*/,
									  UINT idEvent, 
									  DWORD /*dwTime*/)
{
	RECT	rect;
	POINT	pt;
	
	::GetClientRect (hWnd, &rect);
	::MapWindowPoints (hWnd, NULL, (LPPOINT)&rect, 2);

	::GetCursorPos (&pt);
	if (!::PtInRect (&rect, pt) || (WindowFromPoint(pt) != hWnd)) 
	{
		if (!::KillTimer (hWnd, idEvent))
		{
			// Error killing the timer!
		}
		
		::PostMessage (hWnd,WM_MOUSELEAVE, 0, 0);
	}
}
//****
BOOL BCGPTrackMouse (LPTRACKMOUSEEVENT ptme)
{
	ASSERT (ptme != NULL);
	if (ptme->cbSize < sizeof (TRACKMOUSEEVENT))
	{
		ASSERT (FALSE);
		return FALSE;
	}
	
	if (!::IsWindow(ptme->hwndTrack)) 
	{
		ASSERT (FALSE);
		return FALSE;
	}
	
	if (!(ptme->dwFlags & TME_LEAVE)) 
	{
		ASSERT (FALSE);
		return FALSE;
	}
	
	return (BOOL) ::SetTimer (ptme->hwndTrack, ptme->dwFlags, 100,
			(TIMERPROC) BCGPTrackMouseTimerProc);
}
