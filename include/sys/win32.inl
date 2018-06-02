
#ifndef _WIN32_INL_
#define _WIN32_INL_

#if 0

CC_INLINE int SHFileDelete(const char* strSrc0)
{
  int nOk;
  SHFILEOPSTRUCT FileOp;//?����?SHFILEOPSTRUCT?��11???��;
  int n = strlen(strSrc0);
  char* strSrc = (char*)malloc(n + 10); //?��???t?��??;
  strcpy(strSrc, strSrc0);
  strSrc[n] = '\0';
  strSrc[n + 1] = '\0';
  strSrc[n + 2] = '\0';
  FileOp.hwnd = 0;
  FileOp.wFunc = FO_DELETE; //?��DD???t��?3y2������;
  FileOp.pFrom = strSrc;
  FileOp.pTo = "\0";
  //��?����??��1��?3y???t��?��Y��?Windows??��???
  FileOp.fFlags = FOF_ALLOWUNDO;
  FileOp.hNameMappings = NULL;
  //???t��?3y???��???��?��������a
  FileOp.lpszProgressTitle = "???t??����";
  //?a��?��?3y???t
  nOk = SHFileOperation(&FileOp);
  if (nOk) {
    printf("There is an error: %d\n", nOk);
  }
  else {
    printf("SHFileOperation finished successfully\n");
  }
  free(strSrc);
  return nOk;
}
CC_INLINE char* myGetOpenFileName(char* szFilter)
{
  OPENFILENAME ofn;
  static char szFile[ MAX_PATH ];
  ZeroMemory(szFile, MAX_PATH * sizeof(char));
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  ofn.lpstrFile[ 0 ] = ('\0');
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = szFilter ? szFilter : ("All\0*.*\0��???\0*.bmp\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (GetOpenFileName(&ofn)) {
    //OutputDebugString(szFile);
    //OutputDebugString(("\r\n"));
  }
  return szFile;
}

if (z->res != 0xFFFFFFFF) {
  ZRESULT res = GetFileInfo(hf, &z->attr, &isize, &times, &timestamp);
  if (res != ZR_OK) {
    return res;
  }
  SetFilePointer(hf, 0, NULL, FILE_BEGIN); // because GetFileInfo will have screwed it up
  iseekable = true;
  hfin = hf;
  return ZR_OK;
}
else {
  attr = 0x80000000;     // just a normal file
  isize = -1;            // can't know size until at the end
  if (len != 0) {
    isize = len;  // unless we were told explicitly!
  }
  iseekable = false;
  SYSTEMTIME st;
  GetLocalTime(&st);
  FILETIME ft;
  SystemTimeToFileTime(&st, &ft);
  WORD dosdate, dostime;
  filetime2dosdatetime(ft, &dosdate, &dostime);
  times.atime = filetime2timet(ft);
  times.mtime = times.atime;
  times.ctime = times.atime;
  timestamp = (WORD)dostime | (((ulong)dosdate) << 16);
  hfin = hf;
  return ZR_OK;
}

ZRESULT TZip_open_mem(TZip* z, void* src, unsigned int len)
{
  z->hfin = 0;
  crc = CRCVAL_INITIAL;
  ired = 0;
  csize = 0;
  ired = 0;
  lenin = len;
  posin = 0;
  if (src == 0 || len == 0) {
    return ZR_ARGS;
  }
  attr = 0x80000000; // just a normal file
  isize = len;
  iseekable = true;
  SYSTEMTIME st;
  GetLocalTime(&st);
  FILETIME ft;
  SystemTimeToFileTime(&st, &ft);
  WORD dosdate, dostime;
  filetime2dosdatetime(ft, &dosdate, &dostime);
  times.atime = filetime2timet(ft);
  times.mtime = times.atime;
  times.ctime = times.atime;
  timestamp = (WORD)dostime | (((ulong)dosdate) << 16);
  return ZR_OK;
}
ZRESULT TZip_open_dir(TZip* z)
{
  hfin = 0;
  bufin = 0;
  selfclosehf = false;
  crc = CRCVAL_INITIAL;
  isize = 0;
  csize = 0;
  ired = 0;
  attr = 0x41C00010; // a readable writable directory, and again directory
  isize = 0;
  iseekable = false;
  SYSTEMTIME st;
  GetLocalTime(&st);
  FILETIME ft;
  SystemTimeToFileTime(&st, &ft);
  WORD dosdate, dostime;
  filetime2dosdatetime(ft, &dosdate, &dostime);
  times.atime = filetime2timet(ft);
  times.mtime = times.atime;
  times.ctime = times.atime;
  timestamp = (WORD)dostime | (((ulong)dosdate) << 16);
  return ZR_OK;
}
FILETIME timet2filetime(const lutime_t t)
{
  LONGLONG i = Int32x32To64(t, 10000000) + 116444736000000000;
  FILETIME ft;
  ft.dwLowDateTime = (DWORD) i;
  ft.dwHighDateTime = (DWORD)(i >> 32);
  return ft;
}
FILETIME dosdatetime2filetime(WORD dosdate, WORD dostime)
{
  FILETIME ft;
  // date: bits 0-4 are day of month 1-31. Bits 5-8 are month 1..12. Bits 9-15 are year-1980
  // time: bits 0-4 are seconds/2, bits 5-10 are minute 0..59. Bits 11-15 are hour 0..23
  SYSTEMTIME st;
  st.wYear = (WORD)(((dosdate >> 9) & 0x7f) + 1980);
  st.wMonth = (WORD)((dosdate >> 5) & 0xf);
  st.wDay = (WORD)(dosdate & 0x1f);
  st.wHour = (WORD)((dostime >> 11) & 0x1f);
  st.wMinute = (WORD)((dostime >> 5) & 0x3f);
  st.wSecond = (WORD)((dostime & 0x1f) * 2);
  st.wMilliseconds = 0;
  SystemTimeToFileTime(&st, &ft);
  return ft;
}
ZRESULT GetFileInfo(HANDLE hf, ulg* attr, long* size, iztimes* times, ulg* timestamp)
{
  // The handle must be a handle to a file
  // The date and time is returned in a long with the date most significant to allow
  // unsigned integer comparison of absolute times. The attributes have two
  // high bytes unix attr, and two low bytes a mapping of that to DOS attr.
  //struct stat s; int res=stat(fn,&s); if (res!=0) return false;
  // translate windows file attributes into zip ones.
  BY_HANDLE_FILE_INFORMATION bhi;
  BOOL res = GetFileInformationByHandle(hf, &bhi);
  if (!res) {
    return ZR_NOFILE;
  }
  DWORD fa = bhi.dwFileAttributes;
  ulg a = 0;
  // Zip uses the lower word for its interpretation of windows stuff
  if (fa & FILE_ATTRIBUTE_READONLY) {
    a |= 0x01;
  }
  if (fa & FILE_ATTRIBUTE_HIDDEN) {
    a |= 0x02;
  }
  if (fa & FILE_ATTRIBUTE_SYSTEM) {
    a |= 0x04;
  }
  if (fa & FILE_ATTRIBUTE_DIRECTORY) {
    a |= 0x10;
  }
  if (fa & FILE_ATTRIBUTE_ARCHIVE) {
    a |= 0x20;
  }
  // It uses the upper word for standard unix attr, which we manually construct
  if (fa & FILE_ATTRIBUTE_DIRECTORY) {
    a |= 0x40000000;  // directory
  }
  else {
    a |= 0x80000000;  // normal file
  }
  a |= 0x01000000;    // readable
  if (fa & FILE_ATTRIBUTE_READONLY) {}
  else {
    a |= 0x00800000;  // writeable
  }
  // now just a small heuristic to check if it's an executable:
  DWORD red, hsize = GetFileSize(hf, NULL);
  if (hsize > 40) {
    SetFilePointer(hf, 0, NULL, FILE_BEGIN);
    unsigned short magic;
    ReadFile(hf, &magic, sizeof(magic), &red, NULL);
    SetFilePointer(hf, 36, NULL, FILE_BEGIN);
    unsigned long hpos;
    ReadFile(hf, &hpos, sizeof(hpos), &red, NULL);
    if (magic == 0x54AD && hsize > hpos + 4 + 20 + 28) {
      SetFilePointer(hf, hpos, NULL, FILE_BEGIN);
      unsigned long signature;
      ReadFile(hf, &signature, sizeof(signature), &red, NULL);
      if (signature == IMAGE_DOS_SIGNATURE || signature == IMAGE_OS2_SIGNATURE
        || signature == IMAGE_OS2_SIGNATURE_LE || signature == IMAGE_NT_SIGNATURE) {
        a |= 0x00400000; // executable
      }
    }
  }
  //
  if (attr != NULL) {
    *attr = a;
  }
  if (size != NULL) {
    *size = hsize;
  }
  if (times != NULL) {
    // lutime64_t is 32bit number of seconds elapsed since 0:0:0GMT, Jan1, 1970.
    // but FILETIME is 64bit number of 100-nanosecs since Jan1, 1601
    times->atime = filetime2timet(bhi.ftLastAccessTime);
    times->mtime = filetime2timet(bhi.ftLastWriteTime);
    times->ctime = filetime2timet(bhi.ftCreationTime);
  }
  if (timestamp != NULL) {
    WORD dosdate, dostime;
    filetime2dosdatetime(bhi.ftLastWriteTime, &dosdate, &dostime);
    *timestamp = (WORD)dostime | (((DWORD)dosdate) << 16);
  }
  return ZR_OK;
}
lutime64_t filetime2timet(const FILETIME ft)
{
  __int64 i = *(__int64*)&ft;
  return (lutime64_t)((i - 116444736000000000) / 10000000);
}
void filetime2dosdatetime(const FILETIME ft, WORD* dosdate, WORD* dostime)
{
  // date: bits 0-4 are day of month 1-31. Bits 5-8 are month 1..12. Bits 9-15 are year-1980
  // time: bits 0-4 are seconds/2, bits 5-10 are minute 0..59. Bits 11-15 are hour 0..23
  SYSTEMTIME st;
  FileTimeToSystemTime(&ft, &st);
  *dosdate = (WORD)(((st.wYear - 1980) & 0x7f) << 9);
  *dosdate |= (WORD)((st.wMonth & 0xf) << 5);
  *dosdate |= (WORD)((st.wDay & 0x1f));
  *dostime = (WORD)((st.wHour & 0x1f) << 11);
  *dostime |= (WORD)((st.wMinute & 0x3f) << 5);
  *dostime |= (WORD)((st.wSecond * 2) & 0x1f);
}
void EnsureDirectory(const char* rootdir, const char* dir)
{
  const char* lastslash, *c;
  const char* name;
  char cd[MAX_PATH];
  stat_t st[1] = {0};
  if (rootdir != 0 && stat(rootdir, st) == 0xFFFFFFFF) {
    CreateDirectory(rootdir, 0);
  }
  if (*dir == 0) {
    return;
  }
  lastslash = dir, c = lastslash;
  while (*c != 0) {
    if (*c == '/' || *c == '\\') {
      lastslash = c;
    }
    c++;
  }
  name = lastslash;
  if (lastslash != dir) {
    char tmp[MAX_PATH];
    memcpy(tmp, dir, sizeof(char) * (lastslash - dir));
    tmp[lastslash - dir] = 0;
    EnsureDirectory(rootdir, tmp);
    name++;
  }
  *cd = 0;
  if (rootdir != 0) {
    strcpy(cd, rootdir);
  }
  strcat(cd, dir);
  if (GetFileAttributes(cd) == 0xFFFFFFFF) {
    CreateDirectory(cd, NULL);
  }
}
#endif

CC_INLINE int MyAdjustWindowRect(HWND hwnd, RECT* prc)
{
  AdjustWindowRectEx(prc, GetWindowLong(hwnd, GWL_STYLE), 0, GetWindowLong(hwnd, GWL_EXSTYLE));
  return 0;
}

static __int64 HighPerformanceFreq = 0;
static BOOL MultiCore = FALSE;
static BOOL usePerformanceTimer = TRUE;
u32 getRealTime()
{
  if (usePerformanceTimer) {
    static BOOL HighPerformanceTimerSupport = FALSE;
    if (0 == HighPerformanceFreq) {
      HighPerformanceTimerSupport = usePerformanceTimer ? QueryPerformanceFrequency((LARGE_INTEGER*)&HighPerformanceFreq) : FALSE;
    }
    if (HighPerformanceTimerSupport) {
      __int64 nTime;
      BOOL queriedOK = QueryPerformanceCounter((LARGE_INTEGER*)&nTime);
      if (queriedOK) {
        return (u32)((nTime) * 1000 / HighPerformanceFreq);
      }
    }
  }
  return GetTickCount();
}
#ifdef _GUI_INL_
#define MYWINDOWCLASSNAME "MyWindowClass"

//! Compares to the last call of this function to return double and triple clicks.
u32 CDevice_checkSuccessiveClicks(s32 mouseX, s32 mouseY, EMOUSE_INPUT_EVENT inputEvent)
{
  const s32 MAX_MOUSEMOVE = 3;
  static u32 DoubleClickTime = 300;
  static u32 CountSuccessiveClicks;
  static u32 LastClickTime;
  static IPOINT LastClick;
  static EMOUSE_INPUT_EVENT LastMouseInputEvent;
  u32 clickTime = getRealTime();
  if ((clickTime - LastClickTime) < DoubleClickTime
    && ABS(LastClick.x - mouseX) <= MAX_MOUSEMOVE
    && ABS(LastClick.y - mouseY) <= MAX_MOUSEMOVE
    && CountSuccessiveClicks < 3
    && LastMouseInputEvent == inputEvent
    ) {
    ++CountSuccessiveClicks;
  }
  else {
    CountSuccessiveClicks = 1;
  }
  LastMouseInputEvent = inputEvent;
  LastClickTime = clickTime;
  LastClick.x = mouseX;
  LastClick.y = mouseY;
  return CountSuccessiveClicks;
}
static LRESULT CALLBACK sysWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define WINBMPCN 3
#define CALL_ON_EVENT(ret, s, uMsgType, uMsg, wParam, lParam)  if (s->on_event) { ret = s->on_event(s->user, (uMsgType<<16)|uMsg, wParam, lParam); }
#define GetHWND(s)  ((HWND)(s)->hwnd)
//! presents a surface in the client area
BOOL sysShowImage(const char* name, const img_t* im, ECOLOR_FORMAT format, IRECT* srcRect, IRECT* dstRect)
{
  HDC dc;
  window_t* s = sysWindowOpt(name, 0, 0, 0, 0);
  if (s && (dc = GetDC(GetHWND(s)))) {
    IRECT rc[2] = {0};
    uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
    BITMAPINFO* bi = (BITMAPINFO*) buffer;
    const ImageInfo* info = getImageInfoFromFormat(format);
    FillBitmapInfo(bi, im->w, im->h, im->c * 8, 0, 0);
    if (NULL==dstRect) {
      GetClientRect(GetHWND(s), (RECT*)rc);
      dstRect = rc;
    }
    if (NULL==srcRect) {
      srcRect = rc+1;
      srcRect->r = im->w;
      srcRect->b = im->h;
    }
    if (0) {
      StretchDIBits(dc, 0, 0, RCW(dstRect), RCH(dstRect), srcRect->l, srcRect->t, RCW(srcRect), RCH(srcRect),
        im->tt.data, (const BITMAPINFO*)(bi), DIB_RGB_COLORS, SRCCOPY);
    } else {
      int w = MIN(RCW(srcRect), RCW(dstRect));
      int h = MIN(RCH(srcRect), RCH(dstRect));
      SetDIBitsToDevice(dc, dstRect->l, dstRect->t, w, h, 0, 0, 0, RCH(srcRect),
        (const void*)(im->tt.data + srcRect->l * im->c + srcRect->t*im->s), (const BITMAPINFO*)(bi), DIB_RGB_COLORS);
    }
    
    ReleaseDC(GetHWND(s), dc);
  }
  return 0;
}

BOOL sysShowImage1(const char* name, const img_t* im, ECOLOR_FORMAT format, IRECT* srcRect, IRECT* dstRect)
{
  HDC dc;
  window_t* s = sysWindowOpt(name, 0, 0, 0, 0);
  if (s && (dc = GetDC(GetHWND(s)))) {
    IRECT rc[2] = {0};
    BITMAPV4HEADER bi[1] = {0};
    const ImageInfo* info = getImageInfoFromFormat(format);
    bi->bV4Size = sizeof(BITMAPINFOHEADER);
    bi->bV4BitCount = (WORD)info->bpp;
    bi->bV4Planes = 1;
    bi->bV4Width = im->w;
    bi->bV4Height = -((s32)im->h);
    bi->bV4V4Compression = BI_BITFIELDS;
    bi->bV4AlphaMask = info->aMask;
    bi->bV4RedMask = info->rMask;
    bi->bV4GreenMask = info->gMask;
    bi->bV4BlueMask = info->bMask;
    if (NULL==dstRect) {
      GetClientRect(GetHWND(s), (RECT*)rc);
      dstRect = rc;
    }
    if (NULL==srcRect) {
      srcRect = rc+1;
      srcRect->r = im->w;
      srcRect->b = im->h;
    }
    if (1) {
      StretchDIBits(dc, 0, 0, RCW(dstRect), RCH(dstRect), srcRect->l, srcRect->t, RCW(srcRect), RCH(srcRect),
        im->tt.data, (const BITMAPINFO*)(bi), DIB_RGB_COLORS, SRCCOPY);
    } else {
      SetDIBitsToDevice(dc, dstRect->l, dstRect->t, RCW(srcRect), RCH(srcRect), 0, 0, 0, RCH(srcRect),
        (const void*)(im->tt.data + srcRect->l * im->c + srcRect->t*im->s), (const BITMAPINFO*)(bi), DIB_RGB_COLORS);
    }
    
    ReleaseDC(GetHWND(s), dc);
  }
  return 0;
}// Get the codepage from the locale language id
// Based on the table from http://www.science.co.il/Language/Locale-Codes.asp?s=decimal
static unsigned int LocaleIdToCodepage(unsigned int lcid)
{
  switch (lcid) {
  case 1098:  // Telugu
  case 1095:  // Gujarati
  case 1094:  // Punjabi
  case 1103:  // Sanskrit
  case 1111:  // Konkani
  case 1114:  // Syriac
  case 1099:  // Kannada
  case 1102:  // Marathi
  case 1125:  // Divehi
  case 1067:  // Armenian
  case 1081:  // Hindi
  case 1079:  // Georgian
  case 1097:  // Tamil
    return 0;
  case 1054:  // Thai
    return 874;
  case 1041:  // Japanese
    return 932;
  case 2052:  // Chinese (PRC)
  case 4100:  // Chinese (Singapore)
    return 936;
  case 1042:  // Korean
    return 949;
  case 5124:  // Chinese (Macau S.A.R.)
  case 3076:  // Chinese (Hong Kong S.A.R.)
  case 1028:  // Chinese (Taiwan)
    return 950;
  case 1048:  // Romanian
  case 1060:  // Slovenian
  case 1038:  // Hungarian
  case 1051:  // Slovak
  case 1045:  // Polish
  case 1052:  // Albanian
  case 2074:  // Serbian (Latin)
  case 1050:  // Croatian
  case 1029:  // Czech
    return 1250;
  case 1104:  // Mongolian (Cyrillic)
  case 1071:  // FYRO Macedonian
  case 2115:  // Uzbek (Cyrillic)
  case 1058:  // Ukrainian
  case 2092:  // Azeri (Cyrillic)
  case 1092:  // Tatar
  case 1087:  // Kazakh
  case 1059:  // Belarusian
  case 1088:  // Kyrgyz (Cyrillic)
  case 1026:  // Bulgarian
  case 3098:  // Serbian (Cyrillic)
  case 1049:  // Russian
    return 1251;
  case 8201:  // English (Jamaica)
  case 3084:  // French (Canada)
  case 1036:  // French (France)
  case 5132:  // French (Luxembourg)
  case 5129:  // English (New Zealand)
  case 6153:  // English (Ireland)
  case 1043:  // Dutch (Netherlands)
  case 9225:  // English (Caribbean)
  case 4108:  // French (Switzerland)
  case 4105:  // English (Canada)
  case 1110:  // Galician
  case 10249:  // English (Belize)
  case 3079:  // German (Austria)
  case 6156:  // French (Monaco)
  case 12297:  // English (Zimbabwe)
  case 1069:  // Basque
  case 2067:  // Dutch (Belgium)
  case 2060:  // French (Belgium)
  case 1035:  // Finnish
  case 1080:  // Faroese
  case 1031:  // German (Germany)
  case 3081:  // English (Australia)
  case 1033:  // English (United States)
  case 2057:  // English (United Kingdom)
  case 1027:  // Catalan
  case 11273:  // English (Trinidad)
  case 7177:  // English (South Africa)
  case 1030:  // Danish
  case 13321:  // English (Philippines)
  case 15370:  // Spanish (Paraguay)
  case 9226:  // Spanish (Colombia)
  case 5130:  // Spanish (Costa Rica)
  case 7178:  // Spanish (Dominican Republic)
  case 12298:  // Spanish (Ecuador)
  case 17418:  // Spanish (El Salvador)
  case 4106:  // Spanish (Guatemala)
  case 18442:  // Spanish (Honduras)
  case 3082:  // Spanish (International Sort)
  case 13322:  // Spanish (Chile)
  case 19466:  // Spanish (Nicaragua)
  case 2058:  // Spanish (Mexico)
  case 10250:  // Spanish (Peru)
  case 20490:  // Spanish (Puerto Rico)
  case 1034:  // Spanish (Traditional Sort)
  case 14346:  // Spanish (Uruguay)
  case 8202:  // Spanish (Venezuela)
  case 1089:  // Swahili
  case 1053:  // Swedish
  case 2077:  // Swedish (Finland)
  case 5127:  // German (Liechtenstein)
  case 1078:  // Afrikaans
  case 6154:  // Spanish (Panama)
  case 4103:  // German (Luxembourg)
  case 16394:  // Spanish (Bolivia)
  case 2055:  // German (Switzerland)
  case 1039:  // Icelandic
  case 1057:  // Indonesian
  case 1040:  // Italian (Italy)
  case 2064:  // Italian (Switzerland)
  case 2068:  // Norwegian (Nynorsk)
  case 11274:  // Spanish (Argentina)
  case 1046:  // Portuguese (Brazil)
  case 1044:  // Norwegian (Bokmal)
  case 1086:  // Malay (Malaysia)
  case 2110:  // Malay (Brunei Darussalam)
  case 2070:  // Portuguese (Portugal)
    return 1252;
  case 1032:  // Greek
    return 1253;
  case 1091:  // Uzbek (Latin)
  case 1068:  // Azeri (Latin)
  case 1055:  // Turkish
    return 1254;
  case 1037:  // Hebrew
    return 1255;
  case 5121:  // Arabic (Algeria)
  case 15361:  // Arabic (Bahrain)
  case 9217:  // Arabic (Yemen)
  case 3073:  // Arabic (Egypt)
  case 2049:  // Arabic (Iraq)
  case 11265:  // Arabic (Jordan)
  case 13313:  // Arabic (Kuwait)
  case 12289:  // Arabic (Lebanon)
  case 4097:  // Arabic (Libya)
  case 6145:  // Arabic (Morocco)
  case 8193:  // Arabic (Oman)
  case 16385:  // Arabic (Qatar)
  case 1025:  // Arabic (Saudi Arabia)
  case 10241:  // Arabic (Syria)
  case 14337:  // Arabic (U.A.E.)
  case 1065:  // Farsi
  case 1056:  // Urdu
  case 7169:  // Arabic (Tunisia)
    return 1256;
  case 1061:  // Estonian
  case 1062:  // Latvian
  case 1063:  // Lithuanian
    return 1257;
  case 1066:  // Vietnamese
    return 1258;
  }
  return 65001;   // utf-8
}
LRESULT CALLBACK sysWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif
  LRESULT ret = 0;
  static HKL KEYBOARD_INPUT_HKL;
  static int KEYBOARD_INPUT_CODEPAGE;
  window_t* s = sysWindowOpt(0, hwnd, 0, 0, 0);
  static s32 ClickCount = 0;
  typedef struct messageMap {
    s32 group;
    UINT winMessage;
    s32 irrMessage;
  } messageMap;
  static messageMap mouseMap[] = {
    {0, WM_LBUTTONDOWN, EMIE_LBUTTONDOWN},
    {1, WM_LBUTTONUP,   EMIE_LBUTTONUP},
    {0, WM_RBUTTONDOWN, EMIE_RBUTTONDOWN},
    {1, WM_RBUTTONUP,   EMIE_RBUTTONUP},
    {0, WM_MBUTTONDOWN, EMIE_MBUTTONDOWN},
    {1, WM_MBUTTONUP,   EMIE_MBUTTONUP},
    {2, WM_MOUSEMOVE,   EMIE_MOUSEMOVE},
    {3, WM_MOUSEWHEEL,  EMIE_MOUSEWHEEL},
    { -1, 0, 0}
  };
  // handle grouped events
  messageMap* m = mouseMap;
  if (GetCapture() != hwnd && ClickCount > 0) {
    ClickCount = 0;
  }
  while (m->group >= 0 && m->winMessage != message) {
    m += 1;
  }
  if (m->group >= 0) {
    EMOUSE_INPUT_EVENT Event = (EMOUSE_INPUT_EVENT)m->irrMessage;
    int x = LOWORD(lParam), y = HIWORD(lParam);
    if (m->group == 0) { // down
      ClickCount++;
      SetCapture(hwnd);
    }
    else if (m->group == 1) { // up
      ClickCount--;
      if (ClickCount < 1) {
        ClickCount = 0;
        ReleaseCapture();
      }
    }
    // wheel
    if (m->group == 3) {
      IPOINT p = {0}; // fixed by jox
      ClientToScreen(hwnd, (POINT*)&p);
      x -= p.x;
      y -= p.y;
      lParam = MAKELPARAM(x, y);
    }
    
    CALL_ON_EVENT(ret, s, EET_MOUSE_EVENT, Event, wParam, lParam);
    if (Event >= EMIE_LBUTTONDOWN && Event <= EMIE_MBUTTONDOWN) {
      u32 clicks = CDevice_checkSuccessiveClicks(x, y, Event);
      if (clicks == 2) {
        Event = (EMOUSE_INPUT_EVENT)(EMIE_LDOUBLE_CLICK + Event - EMIE_LBUTTONDOWN);
        CALL_ON_EVENT(ret, s, EET_MOUSE_EVENT, Event, wParam, lParam);
      }
      else if (clicks == 3) {
        Event = (EMOUSE_INPUT_EVENT)(EMIE_LTRIPLE_CLICK + Event - EMIE_LBUTTONDOWN);
        CALL_ON_EVENT(ret, s, EET_MOUSE_EVENT, Event, wParam, lParam);
      }
    }
    return 0;
  }
  switch (message) {
  case WM_PAINT:
#if 0
    if (0) {
      PAINTSTRUCT ps;
      BeginPaint(hwnd, &ps);
      EndPaint(hwnd, &ps);
    }
    if (s) {
      PAINTSTRUCT ps = {0};
      HDC hDC = BeginPaint(hwnd, &ps);
      img_t im[1] = {0};
      hdc_get_image(s->hdc, im);
      //ctrl_setredraw(s);
      //BitBlt(hMemDC, 0, 0, w, h, hDC, 0, 0, SRCCOPY);
      matflip(im->h, im->w, im->tt.data, im->s);
      {
        CALL_ON_EVENT(ret, s, EET_GUI_EVENT, EGET_PAINT, (WPARAM)im, 0);
      }
      if (g_sys->cursor_speed>0) {
        imdraw_cursor(im, 0, sys->rcCursor, sys->cursor_speed);
      }
      if (!Cursors[ActiveIcon].Frames.empty() && Cursors[ActiveIcon].FrameTime) {
        // update animated cursors. This could also be done by X11 in case someone wants to figure that out (this way was just easier to implement)
        u32 now = Device->getTimer()->getRealTime();
        u32 frame = ((now - ActiveIconStartTime) / Cursors[ActiveIcon].FrameTime) % Cursors[ActiveIcon].Frames_size;
        SetCursor(Cursors[ActiveIcon].Frames[frame].IconHW);
      }
      s->rcDirty = iRECT(0,0,0,0);
      matflip(im->h, im->w, im->tt.data, im->s);
      BitBlt(hDC, 0, 0, im->w, im->h, s->hdc, 0, 0, SRCCOPY);
      EndPaint(hwnd, &ps);
    }
#endif
  return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_KEYDOWN:
  case WM_KEYUP:
    {
      const UINT MY_MAPVK_VSC_TO_VK_EX = 3; // MAPVK_VSC_TO_VK_EX should be in SDK according to MSDN, but isn't in mine.
      BYTE allKeys[256];
      int Key = wParam;
      int PressedDown = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
      bool Shift, Control;
      int Char = 0;
      if (Key == KEY_SHIFT) {
        // this will fail on systems before windows NT/2000/XP, not sure _what_ will return there instead.
        Key = MapVirtualKeyA(((lParam >> 16) & 255), MY_MAPVK_VSC_TO_VK_EX);
      }
      if (Key == KEY_CONTROL) {
        Key = MapVirtualKeyA(((lParam >> 16) & 255), MY_MAPVK_VSC_TO_VK_EX);
        // some keyboards will just return LEFT for both - left and right keys. So also check extend bit.
        if (lParam & 0x1000000) {
          Key = KEY_RCONTROL;
        }
      }
      if (Key == KEY_MENU) {
        Key = MapVirtualKeyA(((lParam >> 16) & 255), MY_MAPVK_VSC_TO_VK_EX);
        if (lParam & 0x1000000) {
          Key = KEY_RMENU;
        }
      }
      GetKeyboardState(allKeys);
      Shift = ((allKeys[VK_SHIFT] & 0x80) != 0);
      Control = ((allKeys[VK_CONTROL] & 0x80) != 0);
      // Handle unicode and deadkeys in a way that works since Windows 95 and nt4.0
      // Using ToUnicode instead would be shorter, but would to my knowledge not run on 95 and 98.
      if (0) {
        WORD keyChars[2];
        UINT scanCode = HIWORD(lParam);
        int conversionResult = ToAsciiEx(wParam, scanCode, allKeys, keyChars, 0, KEYBOARD_INPUT_HKL);
        if (conversionResult == 1) {
          WORD unicodeChar;
          MultiByteToWideChar(KEYBOARD_INPUT_CODEPAGE, MB_PRECOMPOSED, (const char*)keyChars, sizeof(keyChars), (LPWSTR)&unicodeChar, 1);
          Char = unicodeChar;
        }
        else {
          Char = 0;
        }
      }
      else {
        Char = HIWORD(lParam);
      }
      // allow composing characters like '@' with Alt Gr on non-US keyboards
      if ((allKeys[VK_MENU] & 0x80) != 0) {
        Control = 0;
      }
      if (s) {
        Key |= Shift ? MAKE_KEY(EMBSM_SHIFT, Key) : 0;
        Key |= Control ? MAKE_KEY(EMBSM_CONTROL, Key) : 0;
        CALL_ON_EVENT(ret, s, EET_KEY_EVENT, PressedDown, Key, Char);
      }
      if (message == WM_SYSKEYDOWN || message == WM_SYSKEYUP) {
        return DefWindowProcA(hwnd, message, wParam, lParam);
      }
      else {
        return 0;
      }
    }
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_SIZE: {
    // resize
    return 0;
  }
  case WM_SYSCOMMAND:
    // prevent screensaver or monitor powersave mode from starting
    if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
        (wParam & 0xFFF0) == SC_MONITORPOWER ||
        (wParam & 0xFFF0) == SC_KEYMENU
       ) {
      return 0;
    }
    break;
#if 0
  case WM_ACTIVATE:
    // we need to take care for screen changes, e.g. Alt-Tab
    if (s && s->Fullscreen) {
      if ((wParam & 0xFF) == WA_INACTIVE) {
        // If losing focus we minimize the app to show other one
        ShowWindow(hwnd, SW_MINIMIZE);
        // and switch back to default resolution
        switchToFullScreen(s, true);
      }
      else {
        // Otherwise we retore the fullscreen Irrlicht app
        SetForegroundWindow(hwnd);
        ShowWindow(hwnd, SW_RESTORE);
        // and set the fullscreen resolution again
        switchToFullScreen(s, false);
      }
    }
    break;
  case WM_USER:
    event->EventType = EET_USER_EVENT;
    event->UserEvent.UserData1 = (s32)wParam;
    event->UserEvent.UserData2 = (s32)lParam;
    if (s) {
      CDeviceWin32_postEventFromUser(s, event);
    }
    return 0;
  case WM_SETCURSOR:
    // because Windows forgot about that in the meantime
    if (s) {
      //setActiveIcon(getActiveIcon());
      //setVisible(isVisible());
    }
    break;
#endif
  case WM_INPUTLANGCHANGE:
    // get the new codepage used for keyboard input
    KEYBOARD_INPUT_HKL = GetKeyboardLayout(0);
    KEYBOARD_INPUT_CODEPAGE = LocaleIdToCodepage(LOWORD(KEYBOARD_INPUT_HKL));
    return 0;
  }
  return DefWindowProcA(hwnd, message, wParam, lParam);
}

window_t* sysNameWindow(const char* name, int w, int h, on_event_t on_event, void* user) {
  HINSTANCE hInstance = NULL;
  static WNDCLASSEXA g_winclass[1] = {0};
  static int inited = 0;
  window_t* s = sysWindowOpt(0, 0, name, 0, 0);
  s->on_event = on_event;
  s->user = user;
  ASSERT(s);
  if (!inited) {
    inited = 1;
    g_winclass->cbSize = sizeof(WNDCLASSEXA);
    g_winclass->style = CS_HREDRAW | CS_VREDRAW;// | CS_DBLCLKS;
    g_winclass->lpfnWndProc = sysWindowProc;
    //g_winclass->lpfnWndProc = DefWindowProc;
    g_winclass->cbClsExtra = 0;
    g_winclass->cbWndExtra = 0;
    g_winclass->hInstance = hInstance;
    g_winclass->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    g_winclass->hCursor = LoadCursor(NULL, IDC_ARROW);
    //g_winclass->hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    g_winclass->hbrBackground = (HBRUSH)COLOR_WINDOW;
    g_winclass->lpszMenuName = NULL;
    g_winclass->lpszClassName = MYWINDOWCLASSNAME;
    g_winclass->hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassExA(g_winclass);
  }
  {
#define MYWS_POPUP  (WS_POPUP | WS_CHILD)
#define MYWS_WINDOW1 (WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS)
#define MYWS_WINDOW (WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME)
    IRECT clientSize = iRECT2(0,0,w, h);
    DWORD style = MYWS_WINDOW;
    int xx = AdjustWindowRect((RECT*)&clientSize, style, FALSE);
    const s32 realW = clientSize.r - clientSize.l;
    const s32 realH = clientSize.b - clientSize.t;
    s32 windowL = (GetSystemMetrics(SM_CXSCREEN) - realW) / 2;
    s32 windowT = (GetSystemMetrics(SM_CYSCREEN) - realH) / 2;
    windowL = MAX(windowL, 0);
    windowT = MAX(windowT, 0);
    // create window
    if (NULL==GetHWND(s)) {
      s->hwnd = (void*)CreateWindowExA(0, MYWINDOWCLASSNAME, name, style, windowL, windowT,
        realW, realH, NULL, NULL, hInstance, NULL);
    }
    if (GetHWND(s)) {
      SetWindowLong(GetHWND(s), GWL_WNDPROC, (LONG)sysWindowProc);
      ShowWindow(GetHWND(s), SW_SHOWNORMAL);
      SetActiveWindow(GetHWND(s));
      UpdateWindow(GetHWND(s));
      if (realW>0 && realH>0) {
        MoveWindow(GetHWND(s), windowL, windowT, realW, realH, TRUE);
      }
    }
  }
  return (s);
}

int sysWaitEvent(int delay)
{
  int time0 = GetTickCount();
  void* p = NULL;
  while (1) {
    MSG msg[1] = {0};
    if (delay > 0) {
      if (abs((int)(GetTickCount() - time0)) >= delay) {
        return -1;
      }
      if (!PeekMessage(msg, 0, 0, 0, PM_REMOVE)) {
        Sleep(1);
        continue;
      }
    } else {
      GetMessage(msg, 0, 0, 0);
    }
    if (p = sysWindowOpt(0, (void*)msg->hwnd, NULL, NULL, 0)) {
      sysWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
    }
    else {
      DispatchMessageA(msg);
    }
    if (msg->message == WM_QUIT) {
      return 0;
    }
  }
  return 1;
}

#undef MYWINDOWCLASSNAME
#endif // _GUI_INL_

#endif // _WIN32_INL_

