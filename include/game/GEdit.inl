typedef enum tagDeletePos {
  DP_LEFT = 0,
  DP_RIGHT
} TDP;
enum { COOKIE_SIZE = 1024};
typedef struct GEdit {
  UINT nState;
  UINT m_nTextLen; // 字符长度
  char m_sText[COOKIE_SIZE];
  UINT m_nCursorPos; // 光标位置
  UINT m_nRightPos; // 最右显示的字符位置
  UINT m_nRightPosX; // 最右显示的字符位置
  UINT m_MouseL1CharPos;
  UINT m_MouseL2CharPos;
  int m_IsCuttingClipOn; //剪切板
  int m_MouseLDown;
  int m_nTextPosX; // 字符位置
  float m_LastDtTime;
} GEdit;
GEdit* g_lpFocusEditPtr = 0;
void Edit_DeleteChar(GEdit* btn, TDP aPos)
{
  UINT nIndex;
  int k = 1;
  if (DP_LEFT == aPos) {
    if (btn->m_nCursorPos == 0) {
      return;
    }
    k += (btn->m_nCursorPos > 1 && btn->m_sText[btn->m_nCursorPos - 2] < 0);
    btn->m_nCursorPos -= k;
  }
  else if (DP_RIGHT == aPos) {
    if (btn->m_nCursorPos == btn->m_nTextLen) {
      return;
    }
    k += (btn->m_sText[btn->m_nCursorPos] < 0);
  }
  else {
    return;
  }
  nIndex = btn->m_nCursorPos;
  for (; nIndex < btn->m_nTextLen; ++nIndex) {
    btn->m_sText[nIndex] = btn->m_sText[nIndex + k];
    nIndex++;
  }
  btn->m_nTextLen -= k;
}
void Edit_DeleteMoreChar(GEdit* btn)
{
  int i;
  if (btn->m_MouseL1CharPos < btn->m_MouseL2CharPos) {
    for (i = (int)btn->m_MouseL1CharPos; i < (int)btn->m_MouseL2CharPos; i++) {
      Edit_DeleteChar(btn, DP_LEFT);
    }
  }
  else if (btn->m_MouseL1CharPos > btn->m_MouseL2CharPos) {
    for (i = (int)btn->m_MouseL1CharPos; i > (int)btn->m_MouseL2CharPos; i--) {
      Edit_DeleteChar(btn, DP_RIGHT);
    }
  }
  btn->m_MouseL1CharPos = btn->m_MouseL2CharPos = btn->m_nCursorPos;
}
void Edit_InsertChar(GEdit* btn, int aChar)
{
  UINT nLen;
  if (btn->m_MouseL1CharPos != btn->m_MouseL2CharPos) {
    Edit_DeleteMoreChar(btn);
  }
  nLen = btn->m_nTextLen;
  if (btn->nState & GEDIT_NUMONLY) {
    if (aChar == _T('0') || aChar == _T('1') || aChar == _T('2') || aChar == _T('3') || aChar == _T('4') ||
        aChar == _T('5') || aChar == _T('6') || aChar == _T('7') || aChar == _T('8') || aChar == _T('9') || aChar == _T('.')) {
      if (btn->m_nCursorPos < nLen) {
        while (btn->m_nCursorPos < nLen) {
          btn->m_sText[nLen] = btn->m_sText[nLen - 1];
          --nLen;
        }
      }
      btn->m_sText[btn->m_nCursorPos] = (char)aChar;
      btn->m_nCursorPos++;
      btn->m_nTextLen++;
    }
  }
  else {
    if (btn->m_nCursorPos < nLen) {
      while (btn->m_nCursorPos < nLen) {
        btn->m_sText[nLen] = btn->m_sText[nLen - 1];
        --nLen;
      }
    }
    btn->m_sText[btn->m_nCursorPos] = aChar;
    btn->m_nCursorPos++;
    btn->m_nTextLen++;
  }
}
// 插入字符串到编辑框当前光标处
void Edit_InsertCookie(GEdit* btn, const char* lp)
{
  if (lp) {
    while (*lp) {
      Edit_InsertChar(btn, *lp++);
    }
  }
}
void Edit_Updata(GEdit* btn, float dt)
{
  btn->m_LastDtTime += dt;
  if (btn->m_LastDtTime >= 0.5f) {
    SetStage1(btn, GEDIT_SHOWCURSOR);
    if (btn->m_LastDtTime >= 1.0f) {
      btn->m_LastDtTime = 0;
    }
  }
  else {
    SetStage0(btn, GEDIT_SHOWCURSOR);
  }
  //鼠标
}
BOOL CopyStringToClipboard(const TCHAR* WText)
{
  HGLOBAL hGlobal;
  TCHAR* pGlobal;
  int nlen = (int)_tcslen(WText);
  if (nlen == 0) {
    return FALSE;
  }
  if (!OpenClipboard(NULL)) {
    return FALSE;
  }
  hGlobal = GlobalAlloc(GMEM_DDESHARE, (nlen + 1) * sizeof(TCHAR));
  if (hGlobal == NULL) {
    CloseClipboard();
    return FALSE;
  }
  EmptyClipboard();
  pGlobal = (TCHAR*)GlobalLock(hGlobal);
  _tcscpy(pGlobal, WText);
  GlobalUnlock(hGlobal);
  SetClipboardData(1 == sizeof(TCHAR) ? CF_TEXT : CF_UNICODETEXT, hGlobal);
  CloseClipboard();
  return TRUE;
}
BOOL Edit_SetStrToClipboard(GEdit* btn)
{
  int i = 0;
  int j = 0;
  TCHAR buffer[COOKIE_SIZE];
  memset(buffer, 0, sizeof(buffer));
  if (btn->m_MouseL1CharPos == btn->m_MouseL2CharPos) {
    return FALSE;
  }
  if (btn->m_MouseL1CharPos < btn->m_MouseL2CharPos) {
    for (i = (int)btn->m_MouseL1CharPos; i < (int)btn->m_MouseL2CharPos; i++) {
      buffer[j] = btn->m_sText[i];
      j++;
    }
    _tcsncpy(buffer, buffer, btn->m_MouseL2CharPos - btn->m_MouseL1CharPos);
  }
  else if (btn->m_MouseL1CharPos > btn->m_MouseL2CharPos) {
    for (i = (int)btn->m_MouseL2CharPos; i < (int)btn->m_MouseL1CharPos; i++) {
      buffer[j] = btn->m_sText[i];
      j++;
    }
    _tcsncpy(buffer, buffer, btn->m_MouseL1CharPos - btn->m_MouseL2CharPos);
  }
  CopyStringToClipboard(buffer);
  return TRUE;
}
BOOL Edit_GetStrFromClipboard(GEdit* btn)
{
  PTSTR pGlobal;
  HGLOBAL hGlobal;
  TCHAR buffer[1024];
  if (!OpenClipboard(NULL)) {
    return FALSE;
  }
  if (hGlobal = GetClipboardData(1 == sizeof(TCHAR) ? CF_TEXT : CF_UNICODETEXT)) {
    pGlobal = (TCHAR*)GlobalLock(hGlobal);
    _tcscpy(buffer, (TCHAR*)pGlobal);
    GlobalUnlock(hGlobal);
  }
  else {
    CloseClipboard();
    return FALSE;
  }
  if (btn->m_MouseL1CharPos != btn->m_MouseL2CharPos) {
    Edit_DeleteMoreChar(btn);
  }
  Edit_InsertCookie(btn, buffer);
  if (btn->m_IsCuttingClipOn) {
    btn->m_IsCuttingClipOn = FALSE;
    EmptyClipboard();
    CloseClipboard();
    return FALSE;
  }
  CloseClipboard();
  return TRUE;
}
BOOL Edit_SetCuttingClip(GEdit* btn)
{
  if (btn->m_MouseL1CharPos == btn->m_MouseL2CharPos) {
    return FALSE;
  }
  Edit_SetStrToClipboard(btn);
  Edit_DeleteMoreChar(btn);
  btn->m_IsCuttingClipOn = TRUE;
  return TRUE;
}
void Edit_SelectAllChar(GEdit* btn, WINDEF* wd)
{
  btn->m_MouseL1CharPos = 0;
  btn->m_MouseL2CharPos = btn->m_nCursorPos = btn->m_nTextLen;
}
LRESULT Edit_DoKeyDown(GEdit* btn, WINDEF* wd)
{
  int i;
  if (Input_GetKeyState(HGEK_TAB) && !(btn->nState & GEDIT_NUMONLY)) { // key tab
    for (i = 0; i < 4; ++i) {
      Edit_InsertChar(btn, _T(' ')); // insert 7 space
    }
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_BACKSPACE)) { // key back space
    if (btn->m_MouseL1CharPos != btn->m_MouseL2CharPos) {
      Edit_DeleteMoreChar(btn);
    }
    else {
      Edit_DeleteChar(btn, DP_LEFT);
    }
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_DELETE)) { // key delete
    Edit_DeleteChar(btn, DP_RIGHT);
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_HOME)) { //HOME
    btn->m_nCursorPos = 0;
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_END)) { //END
    btn->m_nCursorPos = btn->m_nTextLen;
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_LEFT)) { // key left
    if (btn->m_nCursorPos > 0) {
      --btn->m_nCursorPos;
    }
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_RIGHT)) { // key right
    if (btn->m_nCursorPos < btn->m_nTextLen) {
      ++btn->m_nCursorPos;
    }
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_ESCAPE)) { // key ESC
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_CTRL) && Input_GetKeyState(HGEK_Z)) { //CTRL+Z，反悔
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_CTRL) && Input_GetKeyState(HGEK_C)) { //CTRL+C，复制
    Edit_SetStrToClipboard(btn);
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_CTRL) && Input_GetKeyState(HGEK_V)) { //CTRL+V，粘帖
    Edit_GetStrFromClipboard(btn);
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_CTRL) && Input_GetKeyState(HGEK_X)) { //CTRL+X，剪切
    Edit_SetCuttingClip(btn);
    return TRUE;
  }
  else if (Input_GetKeyState(HGEK_CTRL) && Input_GetKeyState(HGEK_A)) { //CTRL+A，全选
    Edit_SelectAllChar(btn, wd);
    return TRUE;
  }
  return FALSE;
}
void Edit_OnChar(GEdit* btn, WINDEF* wd, UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (Edit_DoKeyDown(btn, wd))
  {}
  else if (btn->m_nCursorPos < COOKIE_SIZE) {
    Edit_InsertChar(btn, nChar);
  }
  btn->m_LastDtTime = 0.5f; //光标显示
  //当有键盘输入操作时，取肖光标选字状态
  if (Input_GetKeyState(HGEK_CTRL) && Input_GetKeyState(HGEK_A)) //CTRL+A，全选
  {}
  else {
    btn->m_MouseLDown = FALSE;
    btn->m_MouseL1CharPos = btn->m_MouseL2CharPos = btn->m_nCursorPos;
  }
}
void Edit_OnCharHZ(GEdit* btn, UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (btn->m_nCursorPos < COOKIE_SIZE) {
    char szImeChar[3];
    if (nChar < 128) {
      szImeChar[0] = (char)nChar;
      szImeChar[1] = '\0';
    }
    else {
      szImeChar[0] = (char)(nChar >> 8);
      szImeChar[1] = (char)nChar;
      szImeChar[2] = '\0';
    }
    Edit_InsertChar(btn, *(szImeChar));
  }
  btn->m_LastDtTime = 0.5f; //光标显示
  //当有键盘输入操作时，取肖光标选字状态
  btn->m_MouseLDown = FALSE;
  btn->m_MouseL1CharPos = btn->m_MouseL2CharPos = btn->m_nCursorPos;
}
int Edit_UpdataTextPosX(GEdit* btn, WINDEF* wd, LPCRECT lpRect)
{
  int nEditW = RCWIDTH(lpRect);
  int text_x = lpRect->left;
  SIZE szTmp = FontGetTextSize(btn->m_sText, btn->m_nCursorPos);
  SIZE szTmp2 = FontGetTextSize(btn->m_sText, btn->m_nTextLen);
  btn->m_nTextPosX = MAX(btn->m_nTextPosX - nEditW / 4, nEditW - szTmp2.w);
  if (btn->m_nTextPosX + szTmp.w < nEditW / 4) {
    btn->m_nTextPosX = 0;
  }
  btn->m_nTextPosX = MIN(btn->m_nTextPosX, 0);
  //printf("%d %d\n", btn->m_nTextPosX, btn->m_MouseL2CharPos);
  return 0;
}
void Edit_MouseLMove(GEdit* btn, WINDEF* wd, POINT pt, LPCRECT lpRect)
{
  int text_x, nEditW = RCWIDTH(lpRect);
  if (pt.x > lpRect->right - 10) {
    btn->m_nTextPosX -= nEditW / 4;
  }
  if (pt.x < lpRect->left + 10) {
    btn->m_nTextPosX += nEditW / 4;
  }
  Edit_UpdataTextPosX(btn, wd, lpRect);
  text_x = btn->m_nTextPosX + lpRect->left;
  btn->m_MouseL2CharPos = FontGetCharIndexFromPosX(btn->m_sText, btn->m_nTextLen, pt.x - text_x);
  btn->m_nCursorPos = btn->m_MouseL2CharPos;
}

