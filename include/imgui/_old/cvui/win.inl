#include "wincv.inl"
BOOL ShowFileInfo(HWND hWnd, HDC hDC, HDROP hDropInfo)
{
  POINT pt;
  WORD cFiles;
  HINSTANCE hCurrentInst;
  HICON hIcon;
  SHFILEINFO sfi;
  char lpszFile[ 80 ];
  char szBuff[ 1024 ];
  DragQueryPoint(hDropInfo, &pt);
  cFiles = DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, (LPTSTR)NULL, (UINT)NULL);
  if (cFiles > 1) {
    wsprintf(szBuff, "DragFileNumber: %d", (int)cFiles);
    TextOut(hDC, pt.x, pt.y, szBuff, lstrlen(szBuff));
    return FALSE;
  }
  else {
    DragQueryFile(hDropInfo, 0, lpszFile, sizeof(lpszFile));
    if (SHGetFileInfo(lpszFile, 0, &sfi, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME | SHGFI_TYPENAME)) {
      memset(szBuff, 0, sizeof(szBuff));
      wsprintf(szBuff, "Name: %s Type: %s", sfi.szDisplayName, sfi.szTypeName);
    }
    hCurrentInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
    hIcon = ExtractIcon(hCurrentInst, lpszFile, 0);
    if (hIcon == NULL || hIcon == (HICON)1) {
      TextOut(hDC, pt.x, pt.y, szBuff, lstrlen(szBuff));
    }
    else {
      DrawIcon(hDC, pt.x, pt.y, hIcon);
      TextOut(hDC, pt.x, pt.y + 32, szBuff, lstrlen(szBuff));
    }
  }
  return TRUE;
}
BOOL ShowFileInfo2(HWND hWnd, HDROP hDropInfo)
{
  POINT pt;
  WORD cFiles;
  HINSTANCE hCurrentInst;
  HICON hIcon;
  SHFILEINFO sfi;
  char lpszFile[ 80 ];
  char szBuff[ 1024 ];
  DragQueryPoint(hDropInfo, &pt);
  cFiles = DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, (LPTSTR)NULL, (UINT)NULL);
  if (cFiles > 1) {
    wsprintf(szBuff, "DragFileNumber: %d", (int)cFiles);
    printf("%d %d %s\n", pt.x, pt.y, szBuff);
    return FALSE;
  }
  else {
    DragQueryFile(hDropInfo, 0, lpszFile, sizeof(lpszFile));
    if (SHGetFileInfo(lpszFile, 0, &sfi, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME | SHGFI_TYPENAME)) {
      memset(szBuff, 0, sizeof(szBuff));
      wsprintf(szBuff, "Name: %s Type: %s", sfi.szDisplayName, sfi.szTypeName);
    }
    hCurrentInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
    hIcon = ExtractIcon(hCurrentInst, lpszFile, 0);
    if (hIcon == NULL || hIcon == (HICON)1) {
      //TextOut( hDC,pt.x,pt.y,szBuff,lstrlen( szBuff ) );
      printf("%d %d %s\n", pt.x, pt.y, szBuff);
    }
    else {
      //DrawIcon( hDC,pt.x,pt.y,hIcon );
      //TextOut( hDC,pt.x,pt.y + 32,szBuff,lstrlen( szBuff ) );
      printf("%d %d %s\n", pt.x, pt.y, szBuff);
    }
  }
  return TRUE;
}
BOOL ModifyStyle(HWND hWnd, int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
  DWORD dwStyle, dwNewStyle;
  ASSERT(hWnd != NULL);
  dwStyle = GetWindowLong(hWnd, nStyleOffset);
  dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
  if (dwStyle == dwNewStyle) {
    return FALSE;
  }
  SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
  if (nFlags != 0) {
    SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
  }
  return TRUE;
}
#if 0
//#include "test_win_WndProc_facerecong.inl"
#include "test_win_WndProc.inl"
//#include "test_win_stock.inl"
//#include "test_win_BinPacking.inl"
#include "test_win_cam.inl"
//#include "test_win_WndProc_MapEdit.inl"
int test_win()
{
  HWND hWnd;
  INITCOMMONCONTROLSEX InitCtrlEx;
  InitCommonControlsEx(&InitCtrlEx);
  hWnd = Win_Create(0, test_win_WndProc);
  Win_Main();
  return 0;
}
#endif

