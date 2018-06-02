
// CIconWnd

void CIconEdit::RecalcLayout()
{
  if (m_iconWnd.m_hIcon) {
    int width = GetSystemMetrics(SM_CXSMICON);
    DWORD dwMargins = GetMargins();
    SetMargins(width, HIWORD(dwMargins));

    RECT rectEditArea;
    GetClientRect(rectEditArea);
    rectEditArea.left = width + 4;
    SetRect(rectEditArea);
  }
}

void CIconEdit::OnSize(UINT nType, int cx, int cy)
{
  CIconEditBase::OnSize(nType, cx, cy);

  RecalcLayout();
}

LRESULT CIconEdit::OnSetFont(WPARAM wParam, LPARAM lParam)
{
  DefWindowProc(WM_SETFONT, wParam, lParam);

  RecalcLayout();

  return 0;
}

void CIconEdit::DrawIcon(HDDC hDC)
{
  m_iconWnd.DrawIcon(hDC);
}

BOOL CIconEdit::OnEraseBkgnd(HDDC hDC)
{
  DrawIcon(hDC);
  return TRUE;
}

void CIconEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  //this will draw the background again
  //so that the button will be drawn if the text exists

  InvalidateRect(NULL, FALSE);

  CIconEditBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CIconEdit::OnSetCursor(UINT nHitTest, UINT message)
{
  POINT pntCursor;
  GetCursorPos(&pntCursor);
  ScreenToClient(&pntCursor);
  //if mouse is not in the edit area then
  //show arrow cursor
  RECT rectEditArea;
  GetRect(rectEditArea);

  if (!rectEditArea.PtInRect(pntCursor)) {
    SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW)));
    return TRUE;
  }

  return CIconEditBase::OnSetCursor(pWnd, nHitTest, message);
}
