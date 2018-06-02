
void DrawFocus( HDC hDC, RECT r ) {
  DeflateRect( &r, 2, 2 );
  DrawFocusRect( hDC, &r );
}
BOOL DrawControl( HDC hDC, LPRECT lprc, UINT uState ) {
  return DrawFrameControl( hDC, lprc, DFC_BUTTON, uState );
}
#define GetButtonState() UISTATE_FOCUSED
static int UIButton_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event ) {
  if ( 
    event->Type == UIEVENT_SETFOCUS||
    event->Type == UIEVENT_KILLFOCUS||
    event->Type == UIEVENT_BUTTONUP||
    event->Type == UIEVENT_BUTTONDOWN||
    event->Type == UIEVENT_KILLFOCUS||
    0
    ) {
    Invalidate(ww->m_hWnd);
  }
  if ( event->Type == UIEVENT_BUTTONUP ) {
    if ( PtInRect( &ctr->m_rcItem, event->ptMouse ) )
      SendNotify( ww, ctr, _T( "click" ), 0, 0 );
  }
  return CControlUI_Event(ww, ctr, event);
}
static int UIButton_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  UIResource* res = ww->m_pResource;
  UINT uButtonState = ww->m_uButtonState;
  UINT m_nStyle = DFCS_BUTTONPUSH;
  UINT m_nSize = 16;
  int nState = m_nStyle;
  int bPtIn = PtInRect(&ctr->m_rcItem, ww->m_ptLastMousePos);
  if (ww->m_pFocus==ctr) {
    uButtonState|=UISTATE_FOCUSED;
  }
  nState &= ~DFCS_PUSHED;
  nState &= ~DFCS_HOT;
  if ( ( uButtonState & UISTATE_DISABLED ) != 0 ) {
    nState |= DFCS_INACTIVE;
  } else if ( bPtIn && ( uButtonState & UISTATE_PUSHED ) != 0 ) {
    nState |= DFCS_PUSHED;
  } else if ( ( uButtonState & UISTATE_HOT ) != 0 ) {
    m_nStyle |= DFCS_HOT;
  } else if ( ( uButtonState & UISTATE_FOCUSED ) != 0 ) {
    m_nStyle |= DFCS_HOT;
  }
  if ( ( m_nStyle & 0xff ) == DFCS_BUTTONPUSH ) {
    DrawFrameControl( hDC, &ctr->m_rcItem, DFC_BUTTON, nState );
  } else {
    RECT rc;
    rc.left = ctr->m_rcItem.left + 3;
    rc.right = rc.left + m_nSize;
    rc.top = ctr->m_rcItem.top + ( ctr->m_rcItem.bottom - ctr->m_rcItem.top - m_nSize ) / 2;
    rc.bottom = rc.top + m_nSize;
    DrawControl( hDC, &rc, nState );
  }
  {
    int iFont=0;
    DWORD dwTextColor = UIResource_GetDefaultFontColor(res);
    UIDrawText( hDC, res, ctr->m_rcItem, ctr->m_sText, dwTextColor, iFont, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
  }
  if ( ( uButtonState & UISTATE_FOCUSED ) != 0 ) {
    DrawFocus( hDC, ctr->m_rcItem );
    //printf("asdfasdf\n");
  }
  return 0;
}
#define REGISTERCONTROL(NAME) { \
  static CControlUI_vtable NAME##_vtable = {#NAME, sizeof(UI##NAME), UI##NAME##_CtrInit, UI##NAME##_CtrFree, UI##NAME##_DoPaint, UI##NAME##_Event, UI##NAME##_SetPos, UI##NAME##_SetAttribute}; \
  RegisterControl(&NAME##_vtable); }

#define UIButton_CtrInit CControlUI_CtrInit
#define UIButton_CtrFree CControlUI_CtrFree
#define UIButton_SetPos CControlUI_SetPos
#define UIButton_SetAttribute CControlUI_SetAttribute
typedef CControlUI UIButton;
#define UILabel_CtrInit CControlUI_CtrInit
#define UILabel_CtrFree CControlUI_CtrFree
#define UILabel_SetPos CControlUI_SetPos
#define UILabel_DoPaint UILabel_PaintText
#define UILabel_Event CControlUI_Event

int RegisterButton() {
  REGISTERCONTROL(Label);
  REGISTERCONTROL(Button);
  REGISTERCONTROL(Scrollbar);
  REGISTERCONTROL(ScrollbarBase);
  REGISTERCONTROL(GifAnimation);
  REGISTERCONTROL(VerticalLayout);
  REGISTERCONTROL(HorizontalLayout);
  REGISTERCONTROL(CoolEdit);
  REGISTERCONTROL(ListCtrl);
  //REGISTERCONTROL(GridCtrl);
  REGISTERCONTROL(TreeList);
  REGISTERCONTROL(Cam);
  return 0;
}
