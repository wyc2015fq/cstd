

LRESULT TraceMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  char* str = 0;

  switch (msg) {
#include "msg2string.txt"
    //default:str="未知消息";break;
  }

  if (str) {
    printf("%04x %04x %04x ", msg, wparam, lparam);
    printf("%s \n", str);
  }
  else {
    int asdf = 0;
    //printf("%x\n", msg.message);
    //printf("%d\n", msg.message);
    //printf("%x\n", 0x8000);
  }

  return 0;
}

typedef struct TButton {
  UINT nState;
  TCHAR* szText;
  int (*click)(WINDEF* wd, struct TButton* btn);
} TButton;

LRESULT Button_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, WINDEF* wd, TButton* btn, LPCRECT lpRect)
{
  TDrawDC ddc = {0};

  //TraceMsg(hWnd, message, wParam, lParam);
  if (NULL == wd->im) {
    return 0;
  }

  INITDC(&ddc, wd->im->h, wd->im->w, wd->im->bw, wd->im + 1);

  switch (message) {
  case WM_SIZE:
  case WM_PAINT: {
    SetStage1(btn, STATE_REDRAW);
  }
  break;

  case WM_LBUTTONDBLCLK: {
    POINT pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    //btn->nState=0;
    if (PtInRect(lpRect, pt)) {
      SetStage1(btn, ODS_SELECTED);
    }
  }
  break;

  case WM_LBUTTONDOWN: {
    //btn->nState=0;
    if (PtInRect(lpRect, cPOINT(LOWORD(lParam), HIWORD(lParam)))) {
      SetStage1(btn, ODS_SELECTED);
    }
  }
  break;

  case WM_LBUTTONUP: {
    POINT pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    //btn->nState=0;
    if (PtInRect(lpRect, pt)) {
      SetStage0(btn, ODS_SELECTED);

      if (btn->click) {
        btn->click(wd, btn);
      }
    }
  }
  break;

  case WM_MOUSEMOVE: {
    POINT pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    if (PtInRect(lpRect, pt)) {
      SetStage1(btn, ODS_OVER);
    }
    else {
      SetStage0(btn, ODS_OVER);
    }
  }
  break;

  default:
    break;
  }

  if (btn->nState & STATE_REDRAW) {
    TDrawButton(&ddc, lpRect, btn->nState, btn->szText, strlen(btn->szText));
    SetStage0(btn, STATE_REDRAW);
  }

  return 0;
}

#include "GEdit.inl"

#define InsertHead(_A, _N, _X) {int _n=_N; for (;--_n>=0;) {(_A)[_n+1]=(_A)[_n];} (_A)[0]=_X;}


LRESULT Edit_WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam, WINDEF* wd, GEdit* btn, LPCRECT lpRect)
{
  TDrawDC ddc = {0};
  TraceMsg(hWnd, nMsg, wParam, lParam);

  if (NULL == wd->im) {
    return 0;
  }

  INITDC(&ddc, wd->im->h, wd->im->w, wd->im->bw, wd->im + 1);

  switch (nMsg) {
  case WM_SIZE:
  case WM_PAINT: {
    SetStage1(btn, STATE_REDRAW);
  }
  break;

  case WM_LBUTTONUP:
    btn->m_MouseLDown = 0;
    SetStage1(btn, STATE_REDRAW);
    break;

  case WM_LBUTTONDOWN: {
    POINT pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    //btn->nState=0;
    if (PtInRect(lpRect, pt)) {
      int text_x = btn->m_nTextPosX + lpRect->left;
      btn->m_nCursorPos = FontGetCharIndexFromPosX(btn->m_sText, btn->m_nTextLen, pt.x - text_x);
      btn->m_LastDtTime = 0.5f;  //光标显示
      btn->m_MouseLDown = TRUE;
      btn->m_MouseL1CharPos = btn->m_MouseL2CharPos = btn->m_nCursorPos;
      g_lpFocusEditPtr = btn;
      SetStage1(btn, GEDIT_SHOWCURSOR | GEDIT_SHOWOUTERFRAME);
    }
    else {
      g_lpFocusEditPtr = 0;
      SetStage0(btn, GEDIT_SHOWCURSOR | GEDIT_SHOWOUTERFRAME);
    }

    SetStage1(btn, STATE_REDRAW);
  }
  break;

  case WM_MOUSEMOVE: {
    POINT pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    //btn->nState=0;
    if (PtInRect(lpRect, pt) && btn->m_MouseLDown) {
      Edit_MouseLMove(btn, wd, pt, lpRect);
      SetStage1(btn, STATE_REDRAW);
    }
    else {
    }
  }
  break;

  case WM_KEYUP:
    break;

  case WM_KEYDOWN:
    if (g_lpFocusEditPtr == btn) {
      switch (wParam) {
      case VK_DELETE: //key delete
      case VK_LEFT: //key left
      case VK_RIGHT: //key right
      case VK_HOME: //key home
      case VK_END: { //key end
        printf("%x\n", wParam);
        //btn->OnChar((wParam), LOWORD(lParam), HIWORD(lParam));
        Edit_OnChar(btn, wd, (wParam), LOWORD(lParam), HIWORD(lParam));
        SetStage1(btn, STATE_REDRAW);
      }
      break;
      }
    }

    break;

  case WM_CHAR:
    if (g_lpFocusEditPtr) {
      //btn->OnChar((wParam), LOWORD(lParam), HIWORD(lParam));
      //btn->m_nTextLen++;
      //printf("%x\n", wParam);
      //InsertHead(btn->m_sText+btn->m_nCursorPos, btn->m_nTextLen-btn->m_nCursorPos, wParam);
      //btn->m_nCursorPos++;
      Edit_OnChar(btn, wd, (wParam), LOWORD(lParam), HIWORD(lParam));
      SetStage1(btn, STATE_REDRAW);
    }

    break;

  case WM_IME_CHAR:
    if (g_lpFocusEditPtr) {
      //g_lpFocusEditPtr->OnCharHZ((wParam), LOWORD(lParam), HIWORD(lParam));
    }

    break;
  }

  Edit_UpdataTextPosX(btn, wd, lpRect);

  if (btn->nState & STATE_REDRAW) {
    TDrawEdit(&ddc, lpRect, btn->nState, btn->m_nTextPosX, btn->m_sText, btn->m_nTextLen, btn->m_nCursorPos, btn->m_MouseL1CharPos, btn->m_MouseL2CharPos);
    SetStage0(btn, STATE_REDRAW);
  }

  //int TDrawEdit(HDDC hDC, LPCRECT lpRect, UINT nState, GFont* font, const char* lpText, int m_nCursorPos)
  return 0;
}

