
static LPCTSTR __TraceMsg( UINT uMsg ) {
#define MSGDEF(x) if(uMsg==x) return _T(#x)
  MSGDEF( WM_SETCURSOR );
  MSGDEF( WM_NCHITTEST );
  MSGDEF( WM_NCPAINT );
  MSGDEF( WM_PAINT );
  MSGDEF( WM_ERASEBKGND );
  MSGDEF( WM_NCMOUSEMOVE );
  MSGDEF( WM_MOUSEMOVE );
  MSGDEF( WM_MOUSELEAVE );
  MSGDEF( WM_MOUSEHOVER );
  MSGDEF( WM_NOTIFY );
  MSGDEF( WM_COMMAND );
  MSGDEF( WM_MEASUREITEM );
  MSGDEF( WM_DRAWITEM );
  MSGDEF( WM_LBUTTONDOWN );
  MSGDEF( WM_LBUTTONUP );
  MSGDEF( WM_LBUTTONDBLCLK );
  MSGDEF( WM_RBUTTONDOWN );
  MSGDEF( WM_RBUTTONUP );
  MSGDEF( WM_RBUTTONDBLCLK );
  MSGDEF( WM_SETFOCUS );
  MSGDEF( WM_KILLFOCUS );
  MSGDEF( WM_MOVE );
  MSGDEF( WM_SIZE );
  MSGDEF( WM_SIZING );
  MSGDEF( WM_MOVING );
  MSGDEF( WM_GETMINMAXINFO );
  MSGDEF( WM_CAPTURECHANGED );
  MSGDEF( WM_WINDOWPOSCHANGED );
  MSGDEF( WM_WINDOWPOSCHANGING );
  MSGDEF( WM_NCCALCSIZE );
  MSGDEF( WM_NCCREATE );
  MSGDEF( WM_NCDESTROY );
  MSGDEF( WM_TIMER );
  MSGDEF( WM_KEYDOWN );
  MSGDEF( WM_KEYUP );
  MSGDEF( WM_CHAR );
  MSGDEF( WM_SYSKEYDOWN );
  MSGDEF( WM_SYSKEYUP );
  MSGDEF( WM_SYSCOMMAND );
  MSGDEF( WM_SYSCHAR );
  MSGDEF( WM_VSCROLL );
  MSGDEF( WM_HSCROLL );
  MSGDEF( WM_CHAR );
  MSGDEF( WM_SHOWWINDOW );
  MSGDEF( WM_PARENTNOTIFY );
  MSGDEF( WM_CREATE );
  MSGDEF( WM_NCACTIVATE );
  MSGDEF( WM_ACTIVATE );
  MSGDEF( WM_ACTIVATEAPP );
  MSGDEF( WM_CLOSE );
  MSGDEF( WM_DESTROY );
  MSGDEF( WM_GETICON );
  MSGDEF( WM_GETTEXT );
  MSGDEF( WM_GETTEXTLENGTH );
  {static TCHAR szMsg[ 10 ];
  wsprintf( szMsg, _T( "0x%04X" ), uMsg );
  return szMsg;
  }
}