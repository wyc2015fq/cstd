
#ifndef _CLIPBOARD_H_
#define _CLIPBOARD_H_

static char* GetClipboardText(int clear)
{
  char* ret = 0;

  if (OpenClipboard(0)) {
    if (IsClipboardFormatAvailable(CF_TEXT) || IsClipboardFormatAvailable(CF_OEMTEXT)) {
      HANDLE hClipboardData = GetClipboardData(CF_TEXT);
      char* pchData = (char*) GlobalLock(hClipboardData);

      if (strlen(pchData) > 0) {
        ret = strdup(pchData);

        if (clear) {
          EmptyClipboard();
        }
      }

      GlobalUnlock(hClipboardData);
    }

    CloseClipboard();
  }

  return ret;
}

static int SetClipboardText(const char* str, int nStrLen)
{
  if (nStrLen <= 0) {
    return 0;
  }

  if (OpenClipboard(NULL)) {
    char* pchData;
    HGLOBAL hClipboardData;
    EmptyClipboard();
    hClipboardData = GlobalAlloc(GMEM_DDESHARE, nStrLen + 1);
    pchData = (char*)GlobalLock(hClipboardData);
    memmove(pchData, str, nStrLen);
    GlobalUnlock(hClipboardData);
    SetClipboardData(CF_TEXT, hClipboardData);
    CloseClipboard();
  }

  return 0;
}

static int CopyString(HWND hWnd, LPCSTR str, int nStrLen)
{
  if (nStrLen <= 0) {
    return 0;
  }

  if (OpenClipboard(hWnd)) {
    char* pchData;
    HGLOBAL hClipboardData;
    EmptyClipboard();
    hClipboardData = GlobalAlloc(GMEM_DDESHARE, nStrLen + 1);
    pchData = (char*)GlobalLock(hClipboardData);
    memmove(pchData, str, nStrLen * sizeof(TCHAR));
    GlobalUnlock(hClipboardData);
    SetClipboardData(CF_TEXT, hClipboardData);
    CloseClipboard();
  }

  return 0;
}
static char* PasteString(HWND hWnd)
{
  char* str = NULL;

  if (OpenClipboard(hWnd)) {
    if (IsClipboardFormatAvailable(CF_TEXT) || IsClipboardFormatAvailable(CF_OEMTEXT)) {
      HANDLE hClipboardData = GetClipboardData(CF_TEXT);
      char* pchData = (char*)GlobalLock(hClipboardData);
      str = strdup(pchData);
      GlobalUnlock(hClipboardData);
    }

    CloseClipboard();
  }

  return str;
}
#endif // _CLIPBOARD_H_
