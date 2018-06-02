#include "sys/win32.inl"
typedef struct SJoystickWin32Control {
  int x;
} SJoystickWin32Control;
typedef struct CursorFrameW32 {
  HCURSOR IconHW; // hardware cursor
} CursorFrameW32;
typedef struct CursorW32 {
  CursorFrameW32 Frames[1];
  u32 FrameTime;
} CursorW32;
typedef struct COperatingSystemWin32 COperatingSystemWin32;
static HKL KEYBOARD_INPUT_HKL;     //  0
static int KEYBOARD_INPUT_CODEPAGE;//  1252
typedef struct CDeviceWin32 CDeviceWin32;
struct CDeviceWin32 {
  IDevice c[1];
  HWND hWnd;
  bool ChangedToFullScreen;
  bool Resized;
  bool ExternalWindow;
  DEVMODE DesktopMode;
  SJoystickWin32Control* JoyControl;
  s32 BorderX, BorderY;
};
#if 0
typedef struct CSystemWin32 {
  ISystem c[1];
  int nVideoModes;
  CDeviceWin32* DeviceWin32;
  CursorW32 Cursors[100];
  int Cursors_size;
  ECURSOR_ICON ActiveIcon;
  u32 ActiveIconStartTime;
  position2di CursorPos;
  ISIZE WindowSize;
  FSIZE InvWindowSize;
  bool UseReferenceRect;
} CSystemWin32;
#endif
void CDeviceWin32_update(IDevice* s)
{
#if 0
  if (!Cursors[ActiveIcon].Frames.empty() && Cursors[ActiveIcon].FrameTime) {
    // update animated cursors. This could also be done by X11 in case someone wants to figure that out (this way was just easier to implement)
    u32 now = Device->getTimer()->getRealTime();
    u32 frame = ((now - ActiveIconStartTime) / Cursors[ActiveIcon].FrameTime) % Cursors[ActiveIcon].Frames_size;
    SetCursor(Cursors[ActiveIcon].Frames[frame].IconHW);
  }
#endif
}
#if 0
void CCursorControl_free()
{
  int i, f;
  CSystemWin32* c = (CSystemWin32*)(x);
  for (i = 0; i < c->Cursors_size; ++i) {
    for (f = 0; f < Cursors[i].Frames_size; ++f) {
      DestroyCursor(Cursors[i].Frames[f].IconHW);
    }
  }
}
//! Sets the active cursor icon
void CIrrDeviceWin32::CCursorControl::setActiveIcon(ECURSOR_ICON iconId)
{
  if (iconId >= (s32)Cursors_size) {
    return;
  }
  ActiveIcon = iconId;
  ActiveIconStartTime = Device->getTimer()->getRealTime();
  if (Cursors[ActiveIcon].Frames_size) {
    SetCursor(Cursors[ActiveIcon].Frames[0].IconHW);
  }
}
//! Add a custom sprite as cursor icon.
ECURSOR_ICON CIrrDeviceWin32::CCursorControl::addIcon(const SCursorSprite& icon)
{
  if (icon.SpriteId >= 0) {
    CursorW32 cW32;
    cW32.FrameTime = icon.SpriteBank->getSprites()[icon.SpriteId].frameTime;
    int i;
    for (i = 0; i < icon.SpriteBank->getSprites()[icon.SpriteId].Frames_size; ++i) {
      u32 texId = icon.SpriteBank->getSprites()[icon.SpriteId].Frames[i].textureNumber;
      u32 rectId = icon.SpriteBank->getSprites()[icon.SpriteId].Frames[i].rectNumber;
      IRECT rectIcon = icon.SpriteBank->getPositions()[rectId];
      HCURSOR hc = Device->TextureToCursor(hWnd, icon.SpriteBank->getTexture(texId), rectIcon, icon.HotSpot);
      cW32.Frames.push_back(CursorFrameW32(hc));
    }
    Cursors.push_back(cW32);
    return (ECURSOR_ICON)(Cursors_size - 1);
  }
  return ECI_NORMAL;
}
//! replace the given cursor icon.
void CIrrDeviceWin32::CCursorControl::changeIcon(ECURSOR_ICON iconId, const SCursorSprite& icon)
{
  if (iconId >= (s32)Cursors_size) {
    return;
  }
  int i;
  for (i = 0; i < Cursors[iconId].Frames_size; ++i) {
    DestroyCursor(Cursors[iconId].Frames[i].IconHW);
  }
  if (icon.SpriteId >= 0) {
    CursorW32 cW32;
    cW32.FrameTime = icon.SpriteBank->getSprites()[icon.SpriteId].frameTime;
    int i;
    for (i = 0; i < icon.SpriteBank->getSprites()[icon.SpriteId].Frames_size; ++i) {
      u32 texId = icon.SpriteBank->getSprites()[icon.SpriteId].Frames[i].textureNumber;
      u32 rectId = icon.SpriteBank->getSprites()[icon.SpriteId].Frames[i].rectNumber;
      IRECT rectIcon = icon.SpriteBank->getPositions()[rectId];
      HCURSOR hc = Device->TextureToCursor(hWnd, icon.SpriteBank->getTexture(texId), rectIcon, icon.HotSpot);
      cW32.Frames.push_back(CursorFrameW32(hc));
    }
    Cursors[iconId] = cW32;
  }
}
//! Return a system-specific size which is supported for cursors. Larger icons will fail, smaller icons might work.
ISIZE getSupportedIconSize()
{
  ISIZE result;
  result.w = GetSystemMetrics(SM_CXCURSOR);
  result.h = GetSystemMetrics(SM_CYCURSOR);
  return result;
}
#endif
//! copies text to the clipboard
void copyToClipboard(const c8* text)
{
  HGLOBAL clipbuffer;
  char* buffer;
  int len = cstr_len(text);
  if (len == 0) {
    return;
  }
  // Windows version
  if (!OpenClipboard(NULL) || text == 0) {
    return;
  }
  EmptyClipboard();
  clipbuffer = GlobalAlloc(GMEM_DDESHARE, len + 1);
  buffer = (char*)GlobalLock(clipbuffer);
  strcpy(buffer, text);
  GlobalUnlock(clipbuffer);
  SetClipboardData(CF_TEXT, clipbuffer);
  CloseClipboard();
}
//! gets text from the clipboard
//! \return Returns 0 if no string is in there.
const c8* getTextFromClipboard()
{
  if (!OpenClipboard(NULL)) {
    return 0;
  }
  else {
    char* buffer = 0;
    HANDLE hData = GetClipboardData(CF_TEXT);
    buffer = (char*)GlobalLock(hData);
    GlobalUnlock(hData);
    CloseClipboard();
    return buffer;
  }
}
u32 getProcessorSpeedMHz()
{
  LONG Error;
  HKEY Key;
  u32 MHz = 0;
  Error = RegOpenKeyExA(HKEY_LOCAL_MACHINE, ("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &Key);
  if (Error != ERROR_SUCCESS) {
    return false;
  }
  {
    DWORD Speed = 0;
    DWORD Size = sizeof(Speed);
    Error = RegQueryValueExA(Key, ("~MHz"), NULL, NULL, (LPBYTE)&Speed, &Size);
    RegCloseKey(Key);
    if (Error != ERROR_SUCCESS) {
      return 0;
    }
    else if (MHz) {
      MHz = Speed;
    }
  }
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return MHz;
}
bool getSystemMemory(u32* Total, u32* Avail)
{
  MEMORYSTATUS MemoryStatus;
  MemoryStatus.dwLength = sizeof(MEMORYSTATUS);
  // cannot fail
  GlobalMemoryStatus(&MemoryStatus);
  if (Total) {
    *Total = (u32)(MemoryStatus.dwTotalPhys >> 10);
  }
  if (Avail) {
    *Avail = (u32)(MemoryStatus.dwAvailPhys >> 10);
  }
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return true;
}

void debug_print(const c8* message)
{
  // tmp += "\n";
  OutputDebugStringA(message);
  //printf("%s", tmp.c_str());
}
int getVideoMode(int index, SVideoMode* out)
{
  static SVideoMode VideoModes[100] = {0};
  static SVideoMode DesktopMode = {0};
  static int nVideoModes = 0;
  if (!nVideoModes) {
    // enumerate video modes.
    int i = 0, j;
    DEVMODE mode;
    memset(&mode, 0, sizeof(mode));
    mode.dmSize = sizeof(mode);
    while (EnumDisplaySettingsA(NULL, i, &mode)) {
      SVideoMode vm[1];
      vm->size.w = mode.dmPelsWidth;
      vm->size.h = mode.dmPelsHeight;
      vm->depth = mode.dmBitsPerPel;
      for (j = 0; j < nVideoModes; ++j) {
        if (SVideoMode_eq(vm, VideoModes + j)) {
          break;
        }
      }
      if (j == nVideoModes) {
        VideoModes[nVideoModes++] = *vm;
      }
      ++i;
    }
    if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &mode)) {
      DesktopMode.depth = mode.dmBitsPerPel;
      DesktopMode.size = iSIZE(mode.dmPelsWidth, mode.dmPelsHeight);
    }
  }
  if (out) {
    *out = (index >= 0 && index < nVideoModes) ? VideoModes[index] : DesktopMode;
  }
  return nVideoModes;
}
typedef BOOL (WINAPI* PGPI)(DWORD, DWORD, DWORD, DWORD, DWORD*);
const char* getOperatingSystemVersion()
{
  static char winversion[256] = {0};
  static char* out = NULL;
  if (NULL == out) {
    int len = 256;
    OSVERSIONINFOEXA osvi;
    PGPI pGPI;
    BOOL bOsVersionInfoEx;
    out = winversion;
    if (len > 0 && out) {
      *out = 0;
    }
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    bOsVersionInfoEx = GetVersionExA((OSVERSIONINFO*)&osvi);
    if (!bOsVersionInfoEx) {
      osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      if (! GetVersionExA((OSVERSIONINFO*)&osvi)) {
        return winversion;
      }
    }
    switch (osvi.dwPlatformId) {
    case VER_PLATFORM_WIN32_NT:
      if (osvi.dwMajorVersion <= 4) {
        cstr_cat(out, len, "Microsoft Windows NT ", -1);
      }
      else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
        cstr_cat(out, len, "Microsoft Windows 2000 ", -1);
      }
      else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {
        cstr_cat(out, len, "Microsoft Windows XP ", -1);
      }
      else if (osvi.dwMajorVersion == 6) {
        if (osvi.dwMinorVersion == 0) {
          if (osvi.wProductType == VER_NT_WORKSTATION) {
            cstr_cat(out, len, "Microsoft Windows Vista ", -1);
          }
          else {
            cstr_cat(out, len, "Microsoft Windows Server 2008 ", -1);
          }
        }
        else if (osvi.dwMinorVersion == 1) {
          if (osvi.wProductType == VER_NT_WORKSTATION) {
            cstr_cat(out, len, "Microsoft Windows 7 ", -1);
          }
          else {
            cstr_cat(out, len, "Microsoft Windows Server 2008 R2 ", -1);
          }
        }
      }
      if (bOsVersionInfoEx) {
        if (osvi.dwMajorVersion == 6) {
          DWORD dwType;
          pGPI = (PGPI)GetProcAddress(GetModuleHandleA(("kernel32.dll")), "GetProductInfo");
          pGPI(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);
          switch (dwType) {
          case PRODUCT_ULTIMATE:
          case PRODUCT_ULTIMATE_E:
          case PRODUCT_ULTIMATE_N:
            cstr_cat(out, len, "Ultimate Edition ", -1);
            break;
          case PRODUCT_PROFESSIONAL:
          case PRODUCT_PROFESSIONAL_E:
          case PRODUCT_PROFESSIONAL_N:
            cstr_cat(out, len, "Professional Edition ", -1);
            break;
          case PRODUCT_HOME_BASIC:
          case PRODUCT_HOME_BASIC_E:
          case PRODUCT_HOME_BASIC_N:
            cstr_cat(out, len, "Home Basic Edition ", -1);
            break;
          case PRODUCT_HOME_PREMIUM:
          case PRODUCT_HOME_PREMIUM_E:
          case PRODUCT_HOME_PREMIUM_N:
            cstr_cat(out, len, "Home Premium Edition ", -1);
            break;
          case PRODUCT_ENTERPRISE:
          case PRODUCT_ENTERPRISE_E:
          case PRODUCT_ENTERPRISE_N:
            cstr_cat(out, len, "Enterprise Edition ", -1);
            break;
          case PRODUCT_BUSINESS:
          case PRODUCT_BUSINESS_N:
            cstr_cat(out, len, "Business Edition ", -1);
            break;
          case PRODUCT_STARTER:
          case PRODUCT_STARTER_E:
          case PRODUCT_STARTER_N:
            cstr_cat(out, len, "Starter Edition ", -1);
            break;
          }
        }
#ifdef VER_SUITE_ENTERPRISE
        else if (osvi.wProductType == VER_NT_WORKSTATION) {
#ifndef __BORLANDC__
          if (osvi.wSuiteMask & VER_SUITE_PERSONAL) {
            cstr_cat(out, len, "Personal ", -1);
          }
          else {
            cstr_cat(out, len, "Professional ", -1);
          }
#endif
        }
        else if (osvi.wProductType == VER_NT_SERVER) {
          if (osvi.wSuiteMask & VER_SUITE_DATACENTER) {
            cstr_cat(out, len, "DataCenter Server ", -1);
          }
          else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) {
            cstr_cat(out, len, "Advanced Server ", -1);
          }
          else {
            cstr_cat(out, len, "Server ", -1);
          }
        }
#endif
      }
      else {
        HKEY hKey;
        char szProductType[80];
        DWORD dwBufLen;
        RegOpenKeyExA(HKEY_LOCAL_MACHINE, ("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
            0, KEY_QUERY_VALUE, &hKey);
        RegQueryValueExA(hKey, ("ProductType"), NULL, NULL,
            (LPBYTE) szProductType, &dwBufLen);
        RegCloseKey(hKey);
        if (_strcmpi("WINNT", szProductType) == 0) {
          cstr_cat(out, len, "Professional ", -1);
        }
        if (_strcmpi("LANMANNT", szProductType) == 0) {
          cstr_cat(out, len, "Server ", -1);
        }
        if (_strcmpi("SERVERNT", szProductType) == 0) {
          cstr_cat(out, len, "Advanced Server ", -1);
        }
      }
      // Display version, service pack (if any), and build number.
      {
        char tmp[256];
        if (osvi.dwMajorVersion <= 4) {
          _snprintf(tmp, 256, "version %ld.%ld %s (Build %ld)", osvi.dwMajorVersion, osvi.dwMinorVersion,
              (char*)(osvi.szCSDVersion), osvi.dwBuildNumber & 0xFFFF);
        }
        else {
          _snprintf(tmp, 256, "%s (Build %ld)", (osvi.szCSDVersion),
              osvi.dwBuildNumber & 0xFFFF);
        }
        cstr_cat(out, len, tmp, -1);
      }
      break;
    case VER_PLATFORM_WIN32_WINDOWS:
      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
        cstr_cat(out, len, "Microsoft Windows 95 ", -1);
        if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B') {
          cstr_cat(out, len, "OSR2 ", -1);
        }
      }
      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
        cstr_cat(out, len, "Microsoft Windows 98 ", -1);
        if (osvi.szCSDVersion[1] == 'A') {
          cstr_cat(out, len, "SE ", -1);
        }
      }
      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
        cstr_cat(out, len, "Microsoft Windows Me ", -1);
      }
      break;
    case VER_PLATFORM_WIN32s:
      cstr_cat(out, len, "Microsoft Win32s ", -1);
      break;
    }
  }
  return winversion;
}
//! Changes the visible state of the mouse cursor.
bool setCursorVisible(int visible)
{
  static bool IsCursorVisible = true;
  CURSORINFO info;
  BOOL gotCursorInfo;
  if (visible >= 0 && IsCursorVisible != visible) {
    info.cbSize = sizeof(CURSORINFO);
    gotCursorInfo = GetCursorInfo(&info);
    while (gotCursorInfo) {
#ifdef CURSOR_SUPPRESSED
      // new flag for Windows 8, where cursor
      // might be suppressed for touch interface
      if (info.flags == CURSOR_SUPPRESSED) {
        visible = false;
        break;
      }
#endif
      if ((visible && info.flags == CURSOR_SHOWING) ||  // visible
          (!visible && info.flags == 0)) { // hidden
        break;
      }
      // this only increases an internal
      // display counter in windows, so it
      // might have to be called some more
      {
        const int showResult = ShowCursor(visible);
        // if result has correct sign we can
        // stop here as well
        if ((!visible && showResult < 0) || (visible && showResult >= 0)) {
          break;
        }
      }
      // yes, it really must be set each time
      info.cbSize = sizeof(CURSORINFO);
      gotCursorInfo = GetCursorInfo(&info);
    }
    IsCursorVisible = !!visible;
  }
  return IsCursorVisible;
}
bool isCursorVisible()
{
  return setCursorVisible(-1);
}
void setCursorPositioni(s32 x, s32 y)
{
  SetCursorPos(x, y);
}
IPOINT getCursorPosition()
{
  IPOINT pt = {0};
  GetCursorPos((POINT*)&pt);
  return pt;
}
#if 0
ISystem* getSys()
{
  static ISystem sSystemWin32[1] = {0};
  ISystem* s = sSystemWin32;
  static getSys_inited = 0;
  if (!getSys_inited) {
    getSys_inited = 1;
#define TTSET(a, b, c)  s->b = b;
    ISYSTEM_DEF(TTSET)
#undef TTSET
    if (1) {
#if 0
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_ARROW)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_CROSS)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_HAND)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_HELP)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_IBEAM)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_NO)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_WAIT)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_SIZEALL)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_SIZENESW)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_SIZENWSE)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_SIZENS)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_SIZEWE)));
      Cursors.push_back(CursorW32(LoadCursor(NULL, IDC_UPARROW)));
#endif
    }
  }
  return sSystemWin32;
}
#else
#endif
/** Used to notify the cursor that the window resizable settings changed. */
void CDeviceWin32_updateBorderSize(CDeviceWin32* s, bool fullscreen, bool resizable)
{
  if (!fullscreen) {
    if (resizable) {
      s->BorderX = GetSystemMetrics(SM_CXSIZEFRAME);
      s->BorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME);
    }
    else {
      s->BorderX = GetSystemMetrics(SM_CXDLGFRAME);
      s->BorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
    }
  }
  else {
    s->BorderX = s->BorderY = 0;
  }
}
// Get the codepage from the locale language id
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
//! switches to fullscreen
bool switchToFullScreen(IDevice* s, bool reset)
{
  bool ret;
  LONG res;
  DEVMODE dm;
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  if (!s->Fullscreen) {
    return true;
  }
  if (reset) {
    if (c->ChangedToFullScreen) {
      return (ChangeDisplaySettingsA(&c->DesktopMode, 0) == DISP_CHANGE_SUCCESSFUL);
    }
    else {
      return true;
    }
  }
  // use default values from current setting
  memset(&dm, 0, sizeof(dm));
  dm.dmSize = sizeof(dm);
  EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm);
  dm.dmPelsWidth = s->WindowSize.w;
  dm.dmPelsHeight = s->WindowSize.h;
  dm.dmBitsPerPel = s->Bits;
  dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
  res = ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN);
  if (res != DISP_CHANGE_SUCCESSFUL) {
    // try again without forcing display frequency
    dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
    res = ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN);
  }
  ret = false;
  switch (res) {
  case DISP_CHANGE_SUCCESSFUL:
    c->ChangedToFullScreen = true;
    ret = true;
    break;
  case DISP_CHANGE_RESTART:
    llogprintf(ELL_ERROR, "Switch to fullscreen: The computer must be restarted in order for the graphics mode to work.");
    break;
  case DISP_CHANGE_BADFLAGS:
    llogprintf(ELL_ERROR, "Switch to fullscreen: An invalid set of flags was passed in.");
    break;
  case DISP_CHANGE_BADPARAM:
    llogprintf(ELL_ERROR, "Switch to fullscreen: An invalid parameter was passed in. This can include an invalid flag or combination of flags.");
    break;
  case DISP_CHANGE_FAILED:
    llogprintf(ELL_ERROR, "Switch to fullscreen: The display driver failed the specified graphics mode.");
    break;
  case DISP_CHANGE_BADMODE:
    llogprintf(ELL_ERROR, "Switch to fullscreen: The graphics mode is not supported.");
    break;
  default:
    llogprintf(ELL_ERROR, "An unknown error occured while changing to fullscreen.");
    break;
  }
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return ret;
}
#if 0
//! Sets the new position of the cursor.
void setPositionf(f32 x, f32 y)
{
  if (!UseReferenceRect) {
    setPosition(round32(x * WindowSize.w), round32(y * WindowSize.h));
  }
  else {
    setPosition(round32(x * ReferenceRect.getWidth()), round32(y * ReferenceRect.getHeight()));
  }
}
//! Sets the new position of the cursor.
void setPosition(s32 x, s32 y)
{
  if (UseReferenceRect) {
    SetCursorPos(ReferenceRect.l + x, ReferenceRect.t + y);
  }
  else {
    IRECT rect;
    if (GetWindowRect(HWnd, &rect)) {
      SetCursorPos(x + rect.l + BorderX, y + rect.t + BorderY);
    }
  }
  CursorPos.X = x;
  CursorPos.Y = y;
}
//! Returns the current position of the mouse cursor.
const position2di& getPosition()
{
  updateInternalCursorPosition();
  return CursorPos;
}
//! Returns the current position of the mouse cursor.
position2df getRelativePosition()
{
  updateInternalCursorPosition();
  if (!UseReferenceRect) {
    return position2df(CursorPos.X * InvWindowSize.w,
        CursorPos.Y * InvWindowSize.h);
  }
  return position2df(CursorPos.X / (f32)ReferenceRect.getWidth(),
      CursorPos.Y / (f32)ReferenceRect.getHeight());
}
#endif
//! Updates the internal cursor position
void updateInternalCursorPosition()
{
  POINT p;
  if (!GetCursorPos(&p)) {
    DWORD xy = GetMessagePos();
    p.x = GET_X_LPARAM(xy);
    p.y = GET_Y_LPARAM(xy);
  }
}
//! create the driver
void CDeviceWin32_createDriver(IDevice* s)
{
#if 0
  SIrrlichtCreationParameters* CreationParams = s->CreationParams;
  switch (CreationParams->DriverType) {
  case EDT_DIRECT3D8:
#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
    VideoDriver = createDirectX8Driver(CreationParams, FileSystem, hWnd);
    if (!VideoDriver) {
      llogprintf("Could not create DIRECT3D8 Driver.", ELL_ERROR);
    }
#else
    llogprintf("DIRECT3D8 Driver was not compiled into this dll. Try another one.", ELL_ERROR);
#endif // _IRR_COMPILE_WITH_DIRECT3D_8_
    break;
  case EDT_DIRECT3D9:
#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
    VideoDriver = createDirectX9Driver(CreationParams, FileSystem, hWnd);
    if (!VideoDriver) {
      llogprintf("Could not create DIRECT3D9 Driver.", ELL_ERROR);
    }
#else
    llogprintf("DIRECT3D9 Driver was not compiled into this dll. Try another one.", ELL_ERROR);
#endif // _IRR_COMPILE_WITH_DIRECT3D_9_
    break;
  case EDT_OPENGL:
#ifdef _IRR_COMPILE_WITH_OPENGL_
    switchToFullScreen();
    VideoDriver = createOpenGLDriver(CreationParams, FileSystem, this);
    if (!VideoDriver) {
      llogprintf("Could not create OpenGL driver.", ELL_ERROR);
    }
#else
    llogprintf("OpenGL driver was not compiled in.", ELL_ERROR);
#endif
    break;
  case EDT_SOFTWARE:
#ifdef _IRR_COMPILE_WITH_SOFTWARE_
    switchToFullScreen();
    VideoDriver = createSoftwareDriver(CreationParams->WindowSize, CreationParams->Fullscreen, FileSystem, this);
#else
    llogprintf("Software driver was not compiled in.", ELL_ERROR);
#endif
    break;
  case EDT_BURNINGSVIDEO:
#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
    switchToFullScreen();
    VideoDriver = createBurningVideoDriver(CreationParams, FileSystem, this);
#else
    llogprintf("Burning's Video driver was not compiled in.", ELL_ERROR);
#endif
    break;
  case EDT_NULL:
    // create null driver
    VideoDriver = createNullDriver(FileSystem, CreationParams->WindowSize);
    break;
  default:
    llogprintf("Unable to create video driver of unknown type.", ELL_ERROR);
    break;
  }
#endif
}
//! destructor
void CDeviceWin32_exit(IDevice* s)
{
  //delete JoyControl;
  switchToFullScreen(s, true);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//! Process system events
void CDeviceWin32_handleSystemMessages(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  MSG msg;
  while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
    // No message translation because we don't use WM_CHAR and it would conflict with our
    // deadkey handling.
    if (c->ExternalWindow && msg.hwnd == c->hWnd) {
      WndProc(c->hWnd, msg.message, msg.wParam, msg.lParam);
    }
    else {
      DispatchMessageA(&msg);
    }
    if (msg.message == WM_QUIT) {
      s->Close = true;
    }
  }
}

void CDeviceWin32_resizeIfNecessary(IDevice* s)
{
  IRECT r;
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  if (!c->Resized || !s->VideoDriver) {
    return;
  }
  GetClientRect(c->hWnd, (RECT*)&r);
  if (r.r < 2 || r.b < 2) {
    llogprintf(ELL_NONE, "Ignoring resize operation to (%ld %ld)", r.r, r.b);
  }
  else {
    llogprintf(ELL_NONE, "Resizing window (%ld %ld)", r.r, r.b);
    s->VideoDriver->OnResize(s->VideoDriver, iSIZE((u32)r.r, (u32)r.b));
    //getWin32CursorControl()->OnResize(getVideoDriver()->getScreenSize());
  }
  c->Resized = false;
}
//! runs the device. Returns false if device wants to be deleted
bool CDeviceWin32_run(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  getRealTime();
  s->update(s);
  CDeviceWin32_handleSystemMessages(s);
  if (!s->Close) {
    CDeviceWin32_resizeIfNecessary(s);
  }
#if 0
  if (!s->Close && s->JoyControl) {
    JoyControl->pollJoysticks();
  }
#endif
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return !s->Close;
}
//! sets the caption of the window
void CDeviceWin32_setWindowCaption(IDevice* s, const char* text)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  // We use SendMessage instead of SetText to ensure proper
  // function even in cases where the HWND was created in a different thread
  DWORD dwResult;
  SendMessageTimeoutA(c->hWnd, WM_SETTEXT, 0, (LPARAM)(text), SMTO_ABORTIFHUNG, 2000, &dwResult);
}
//! Pause the current process for the minimum time allowed only to allow other processes to execute
void CDeviceWin32_yield(IDevice* s)
{
  Sleep(1);
}
//! Pause execution and let other processes to run for a specified amount of time.
void CDeviceWin32_sleep(IDevice* s, u32 timeMs, bool pauseTimer)
{
#if 0
  const bool wasStopped = Timer ? Timer->isStopped() : true;
  if (pauseTimer && !wasStopped) {
    Timer->stop();
  }
  Sleep(timeMs);
  if (pauseTimer && !wasStopped) {
    Timer->start();
  }
#else
  Sleep(timeMs);
#endif
}
//! presents a surface in the client area
bool CDeviceWin32_present(IDevice* s, IImage* image, void* windowId, IRECT* src)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  HWND hwnd = c->hWnd;
  HDC dc;
  if (windowId) {
    hwnd = (HWND)(windowId);
  }
  dc = GetDC(hwnd);
  if (dc) {
    IRECT rect;
    const void* memory = (const void*)image->Data;
    BITMAPV4HEADER bi;
    const ImageInfo* info = getImageInfoFromFormat(image->Format);
    GetClientRect(hwnd, (RECT*)&rect);
    ZeroMemory(&bi, sizeof(bi));
    bi.bV4Size = sizeof(BITMAPINFOHEADER);
    bi.bV4BitCount = (WORD)info->bpp;
    bi.bV4Planes = 1;
    bi.bV4Width = image->Size.w;
    bi.bV4Height = -((s32)image->Size.h);
    bi.bV4V4Compression = BI_BITFIELDS;
    bi.bV4AlphaMask = info->aMask;
    bi.bV4RedMask = info->rMask;
    bi.bV4GreenMask = info->gMask;
    bi.bV4BlueMask = info->bMask;
    if (src) {
      StretchDIBits(dc, 0, 0, rect.r, rect.b, src->l, src->t, RCW(src), RCH(src),
          memory, (const BITMAPINFO*)(&bi), DIB_RGB_COLORS, SRCCOPY);
    }
    else {
      StretchDIBits(dc, 0, 0, rect.r, rect.b, 0, 0, image->Size.w, image->Size.h,
          memory, (const BITMAPINFO*)(&bi), DIB_RGB_COLORS, SRCCOPY);
    }
    ReleaseDC(hwnd, dc);
  }
  return true;
}
//! notifies the device that it should close itself
void CDeviceWin32_closeDevice(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  MSG msg;
  PeekMessageA(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
  PostQuitMessage(0);
  PeekMessageA(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
  if (!c->ExternalWindow) {
    const char* ClassName = ("CDeviceWin32");
    HINSTANCE hInstance = GetModuleHandleA(0);
    DestroyWindow(c->hWnd);
    UnregisterClassA(ClassName, hInstance);
  }
  s->Close = true;
}
//! returns if window is active. if not, nothing needs to be drawn
bool CDeviceWin32_isWindowActive(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return (GetActiveWindow() == c->hWnd);
}
//! returns if window has focus
bool CDeviceWin32_isWindowFocused(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  bool ret = (GetFocus() == c->hWnd);
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return ret;
}
//! returns if window is minimized
bool CDeviceWin32_isWindowMinimized(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  WINDOWPLACEMENT plc;
  bool ret = false;
  plc.length = sizeof(WINDOWPLACEMENT);
  if (GetWindowPlacement(c->hWnd, &plc)) {
    ret = (plc.showCmd & SW_SHOWMINIMIZED) != 0;
  }
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return ret;
}
//! Notifies the device, that it has been resized
void CDeviceWin32_OnResized(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  c->Resized = true;
}
//! Sets if the window should be resizable in windowed mode.
void CDeviceWin32_setResizable(IDevice* s, bool resize)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  IRECT clientSize;
  LONG style = WS_POPUP;
  if (c->ExternalWindow || !s->VideoDriver || s->Fullscreen) {
    return;
  }
  if (!resize) {
    style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
  }
  else {
    style = WS_THICKFRAME | WS_SYSMENU | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
  }
  if (!SetWindowLongA(c->hWnd, GWL_STYLE, style)) {
    llogprintf(ELL_NONE, "Could not change window style.");
  }
  clientSize.t = 0;
  clientSize.l = 0;
  clientSize.r = s->VideoDriver->ScreenSize.w;
  clientSize.b = s->VideoDriver->ScreenSize.h;
  AdjustWindowRect((RECT*)&clientSize, style, FALSE);
  {
    const s32 realW = clientSize.r - clientSize.l;
    const s32 realH = clientSize.b - clientSize.t;
    const s32 windowL = (GetSystemMetrics(SM_CXSCREEN) - realW) / 2;
    const s32 windowT = (GetSystemMetrics(SM_CYSCREEN) - realH) / 2;
    SetWindowPos(c->hWnd, HWND_TOP, windowL, windowT, realW, realH,
        SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);
    CDeviceWin32_updateBorderSize(c, s->Fullscreen, resize);
  }
}
//! Minimizes the window.
void CDeviceWin32_minimizeWindow(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  WINDOWPLACEMENT wndpl;
  wndpl.length = sizeof(WINDOWPLACEMENT);
  GetWindowPlacement(c->hWnd, &wndpl);
  wndpl.showCmd = SW_SHOWMINNOACTIVE;
  SetWindowPlacement(c->hWnd, &wndpl);
}
//! Maximizes the window.
void CDeviceWin32_maximizeWindow(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  WINDOWPLACEMENT wndpl;
  wndpl.length = sizeof(WINDOWPLACEMENT);
  GetWindowPlacement(c->hWnd, &wndpl);
  wndpl.showCmd = SW_SHOWMAXIMIZED;
  SetWindowPlacement(c->hWnd, &wndpl);
}
//! Restores the window to its original size.
void CDeviceWin32_restoreWindow(IDevice* s)
{
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  WINDOWPLACEMENT wndpl;
  wndpl.length = sizeof(WINDOWPLACEMENT);
  GetWindowPlacement(c->hWnd, &wndpl);
  wndpl.showCmd = SW_SHOWNORMAL;
  SetWindowPlacement(c->hWnd, &wndpl);
}
//! Set the current Gamma Value for the Display
bool CDeviceWin32_setGammaRamp(IDevice* s, f32 red, f32 green, f32 blue, f32 brightness, f32 contrast)
{
  bool r;
  HDC dc;
  u16 ramp[3][256];
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  calculateGammaRamp(ramp[0], red, brightness, contrast);
  calculateGammaRamp(ramp[1], green, brightness, contrast);
  calculateGammaRamp(ramp[2], blue, brightness, contrast);
  dc = GetDC(0);
  r = SetDeviceGammaRamp(dc, ramp) == TRUE;
  ReleaseDC(c->hWnd, dc);
  return r;
}
//! Get the current Gamma Value for the Display
bool CDeviceWin32_getGammaRamp(IDevice* s, f32* red, f32* green, f32* blue, f32* brightness, f32* contrast)
{
  bool r;
  u16 ramp[3][256];
  HDC dc = GetDC(0);
  CDeviceWin32* c = (CDeviceWin32*)(s->x);
  r = GetDeviceGammaRamp(dc, ramp) == TRUE;
  ReleaseDC(c->hWnd, dc);
  if (r) {
    *red = calculateGammaFromRamp(ramp[0]);
    *green = calculateGammaFromRamp(ramp[1]);
    *blue = calculateGammaFromRamp(ramp[2]);
  }
  *brightness = 0.f;
  *contrast = 0.f;
  return r;
}
//! Remove all messages pending in the system message loop
void CDeviceWin32_clearSystemMessages(IDevice* s)
{
  MSG msg;
  while (PeekMessageA(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
  {}
  while (PeekMessageA(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
  {}
}
// shows last error in a messagebox to help internal debugging.
void CDeviceWin32_ReportLastWinApiError(IDevice* s)
{
  // (based on code from ovidiucucu from http://www.codeguru.com/forum/showthread.php?t=318721)
  const char* pszCaption = ("Windows SDK Error Report");
  DWORD dwError = GetLastError();
  if (NOERROR == dwError) {
    MessageBoxA(NULL, ("No error"), pszCaption, MB_OK);
  }
  else {
    const DWORD dwFormatControl = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM;
    char pTextBuffer[256];
    DWORD dwCount = FormatMessageA(dwFormatControl, NULL, dwError, 0, pTextBuffer, 256, NULL);
    if (0 != dwCount) {
      MessageBoxA(NULL, pTextBuffer, pszCaption, MB_OK | MB_ICONERROR);
      LocalFree(pTextBuffer);
    }
    else {
      MessageBoxA(NULL, ("Unknown error"), pszCaption, MB_OK | MB_ICONERROR);
    }
  }
}
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))
#define PALETTEINDEX(i)     ((COLORREF)(0x01000000 | (DWORD)(WORD)(i)))
// Convert an Irrlicht texture to a Windows cursor
// Based on http://www.codeguru.com/cpp/w-p/win32/cursors/article.php/c4529/
HCURSOR TextureToCursor(HWND hwnd, ITexture* tex, const IRECT* sourceRect, IPOINT hotspot)
{
  // create the bitmaps needed for cursors from the texture
  HDC dc = GetDC(hwnd);
  HDC andDc = CreateCompatibleDC(dc);
  HDC xorDc = CreateCompatibleDC(dc);
  HBITMAP andBitmap = CreateCompatibleBitmap(dc, RCW(sourceRect), RCH(sourceRect));
  HBITMAP xorBitmap = CreateCompatibleBitmap(dc, RCW(sourceRect), RCH(sourceRect));
  HBITMAP oldAndBitmap = (HBITMAP)SelectObject(andDc, andBitmap);
  HBITMAP oldXorBitmap = (HBITMAP)SelectObject(xorDc, xorBitmap);
  ECOLOR_FORMAT format = tex->ColorFormat;
  const ImageInfo* info = getImageInfoFromFormat(format);
  ICONINFO iconinfo;
  HCURSOR cursor;
  s32 x, y;
  u32 bytesPerPixel = info->bpp / 8;
  const u8* data = (const u8*)tex->lock(tex, ETLM_READ_ONLY, 0);
  data += sourceRect->t * tex->Pitch + sourceRect->l * bytesPerPixel;
  for (y = 0; y < RCH(sourceRect); ++y) {
    for (x = 0; x < RCW(sourceRect); ++x) {
      SColor pixelCol;
      pixelCol = data2color((const void*)data, format);
      data += bytesPerPixel;
      if (getA(pixelCol) == 0) {  // transparent
        SetPixel(andDc, x, y, RGB(255, 255, 255));
        SetPixel(xorDc, x, y, RGB(0, 0, 0));
      }
      else { // color
        SetPixel(andDc, x, y, RGB(0, 0, 0));
        SetPixel(xorDc, x, y, RGB(getR(pixelCol), getG(pixelCol), getB(pixelCol)));
      }
    }
    data += tex->Pitch;
  }
  tex->unlock(tex);
  SelectObject(andDc, oldAndBitmap);
  SelectObject(xorDc, oldXorBitmap);
  DeleteDC(xorDc);
  DeleteDC(andDc);
  ReleaseDC(hwnd, dc);
  // create the cursor
  iconinfo.fIcon = false; // type is cursor not icon
  iconinfo.xHotspot = hotspot.x;
  iconinfo.yHotspot = hotspot.y;
  iconinfo.hbmMask = andBitmap;
  iconinfo.hbmColor = xorBitmap;
  cursor = CreateIconIndirect(&iconinfo);
  DeleteObject(andBitmap);
  DeleteObject(xorBitmap);
  return cursor;
}
#define CDeviceWin32_postEventFromUser  CDevice_postEventFromUser
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif
  SEvent event[1];
  static s32 ClickCount = 0;
  typedef struct messageMap {
    s32 group;
    UINT winMessage;
    s32 irrMessage;
  } messageMap;
  static messageMap mouseMap[] = {
    {0, WM_LBUTTONDOWN, EMIE_LMOUSE_PRESSED_DOWN},
    {1, WM_LBUTTONUP,   EMIE_LMOUSE_LEFT_UP},
    {0, WM_RBUTTONDOWN, EMIE_RMOUSE_PRESSED_DOWN},
    {1, WM_RBUTTONUP,   EMIE_RMOUSE_LEFT_UP},
    {0, WM_MBUTTONDOWN, EMIE_MMOUSE_PRESSED_DOWN},
    {1, WM_MBUTTONUP,   EMIE_MMOUSE_LEFT_UP},
    {2, WM_MOUSEMOVE,   EMIE_MOUSE_MOVED},
    {3, WM_MOUSEWHEEL,  EMIE_MOUSE_WHEEL},
    { -1, 0, 0}
  };
  IDevice* s = NULL;
  CDeviceWin32* c = NULL;
  // handle grouped events
  messageMap* m = mouseMap;
  if (GetCapture() != hWnd && ClickCount > 0) {
    ClickCount = 0;
  }
  while (m->group >= 0 && m->winMessage != message) {
    m += 1;
  }
  if (m->group >= 0) {
    if (m->group == 0) { // down
      ClickCount++;
      SetCapture(hWnd);
    }
    else if (m->group == 1) { // up
      ClickCount--;
      if (ClickCount < 1) {
        ClickCount = 0;
        ReleaseCapture();
      }
    }
    event->EventType = EET_MOUSE_INPUT_EVENT;
    event->MouseInput.Event = (EMOUSE_INPUT_EVENT) m->irrMessage;
    event->MouseInput.X = (short)LOWORD(lParam);
    event->MouseInput.Y = (short)HIWORD(lParam);
    event->MouseInput.Shift = ((LOWORD(wParam) & MK_SHIFT) != 0);
    event->MouseInput.Control = ((LOWORD(wParam) & MK_CONTROL) != 0);
    // left and right mouse buttons
    event->MouseInput.ButtonStates = wParam & (MK_LBUTTON | MK_RBUTTON);
    // middle and extra buttons
    if (wParam & MK_MBUTTON) {
      event->MouseInput.ButtonStates |= EMBSM_MIDDLE;
    }
#if(_WIN32_WINNT >= 0x0500)
    if (wParam & MK_XBUTTON1) {
      event->MouseInput.ButtonStates |= EMBSM_EXTRA1;
    }
    if (wParam & MK_XBUTTON2) {
      event->MouseInput.ButtonStates |= EMBSM_EXTRA2;
    }
#endif
    event->MouseInput.Wheel = 0.f;
    // wheel
    if (m->group == 3) {
      IPOINT p; // fixed by jox
      p.x = 0;
      p.y = 0;
      ClientToScreen(hWnd, (POINT*)&p);
      event->MouseInput.X -= p.x;
      event->MouseInput.Y -= p.y;
      event->MouseInput.Wheel = ((f32)((short)HIWORD(wParam))) / (f32)WHEEL_DELTA;
    }
    {
      s = getDeviceFromWindowId(hWnd);
      if (s) {
        c = (CDeviceWin32*)(s->x);
        CDevice_postEventFromUser(s, event);
        if (event->MouseInput.Event >= EMIE_LMOUSE_PRESSED_DOWN && event->MouseInput.Event <= EMIE_MMOUSE_PRESSED_DOWN) {
          u32 clicks = CDevice_checkSuccessiveClicks(s, event->MouseInput.X, event->MouseInput.Y, event->MouseInput.Event);
          if (clicks == 2) {
            event->MouseInput.Event = (EMOUSE_INPUT_EVENT)(EMIE_LMOUSE_DOUBLE_CLICK + event->MouseInput.Event - EMIE_LMOUSE_PRESSED_DOWN);
            CDevice_postEventFromUser(s, event);
          }
          else if (clicks == 3) {
            event->MouseInput.Event = (EMOUSE_INPUT_EVENT)(EMIE_LMOUSE_TRIPLE_CLICK + event->MouseInput.Event - EMIE_LMOUSE_PRESSED_DOWN);
            CDevice_postEventFromUser(s, event);
          }
        }
      }
    }
    return 0;
  }
  else {
    s = getDeviceFromWindowId(hWnd);
    if (s) {
      c = (CDeviceWin32*)(s->x);
    }
  }
  switch (message) {
  case WM_PAINT: {
    PAINTSTRUCT ps;
    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
  }
  return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_KEYDOWN:
  case WM_KEYUP: {
    const UINT MY_MAPVK_VSC_TO_VK_EX = 3; // MAPVK_VSC_TO_VK_EX should be in SDK according to MSDN, but isn't in mine.
    BYTE allKeys[256];
    event->EventType = EET_KEY_INPUT_EVENT;
    event->KeyInput.Key = (EKEY_CODE)wParam;
    event->KeyInput.PressedDown = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
    if (event->KeyInput.Key == KEY_SHIFT) {
      // this will fail on systems before windows NT/2000/XP, not sure _what_ will return there instead.
      event->KeyInput.Key = (EKEY_CODE)MapVirtualKeyA(((lParam >> 16) & 255), MY_MAPVK_VSC_TO_VK_EX);
    }
    if (event->KeyInput.Key == KEY_CONTROL) {
      event->KeyInput.Key = (EKEY_CODE)MapVirtualKeyA(((lParam >> 16) & 255), MY_MAPVK_VSC_TO_VK_EX);
      // some keyboards will just return LEFT for both - left and right keys. So also check extend bit.
      if (lParam & 0x1000000) {
        event->KeyInput.Key = KEY_RCONTROL;
      }
    }
    if (event->KeyInput.Key == KEY_MENU) {
      event->KeyInput.Key = (EKEY_CODE)MapVirtualKeyA(((lParam >> 16) & 255), MY_MAPVK_VSC_TO_VK_EX);
      if (lParam & 0x1000000) {
        event->KeyInput.Key = KEY_RMENU;
      }
    }
    GetKeyboardState(allKeys);
    event->KeyInput.Shift = ((allKeys[VK_SHIFT] & 0x80) != 0);
    event->KeyInput.Control = ((allKeys[VK_CONTROL] & 0x80) != 0);
    // Handle unicode and deadkeys in a way that works since Windows 95 and nt4.0
    // Using ToUnicode instead would be shorter, but would to my knowledge not run on 95 and 98.
    if (1) {
      WORD keyChars[2];
      UINT scanCode = HIWORD(lParam);
      int conversionResult = ToAsciiEx(wParam, scanCode, allKeys, keyChars, 0, KEYBOARD_INPUT_HKL);
      if (conversionResult == 1) {
        WORD unicodeChar;
        MultiByteToWideChar(KEYBOARD_INPUT_CODEPAGE, MB_PRECOMPOSED, (const char*)keyChars, sizeof(keyChars), (ushort*)&unicodeChar, 1);
        event->KeyInput.Char = unicodeChar;
      }
      else {
        event->KeyInput.Char = 0;
      }
    }
    else {
      event->KeyInput.Char = HIWORD(lParam);
    }
    // allow composing characters like '@' with Alt Gr on non-US keyboards
    if ((allKeys[VK_MENU] & 0x80) != 0) {
      event->KeyInput.Control = 0;
    }
    if (s) {
      CDeviceWin32_postEventFromUser(s, event);
    }
    if (message == WM_SYSKEYDOWN || message == WM_SYSKEYUP) {
      return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    else {
      return 0;
    }
  }
  case WM_SIZE: {
    // resize
    if (c) {
      c->Resized = true;
    }
    return 0;
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_SYSCOMMAND:
    // prevent screensaver or monitor powersave mode from starting
    if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
        (wParam & 0xFFF0) == SC_MONITORPOWER ||
        (wParam & 0xFFF0) == SC_KEYMENU
       ) {
      return 0;
    }
    break;
  case WM_ACTIVATE:
    // we need to take care for screen changes, e.g. Alt-Tab
    if (s && s->Fullscreen) {
      if ((wParam & 0xFF) == WA_INACTIVE) {
        // If losing focus we minimize the app to show other one
        ShowWindow(hWnd, SW_MINIMIZE);
        // and switch back to default resolution
        switchToFullScreen(s, true);
      }
      else {
        // Otherwise we retore the fullscreen Irrlicht app
        SetForegroundWindow(hWnd);
        ShowWindow(hWnd, SW_RESTORE);
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
  case WM_INPUTLANGCHANGE:
    // get the new codepage used for keyboard input
    KEYBOARD_INPUT_HKL = GetKeyboardLayout(0);
    KEYBOARD_INPUT_CODEPAGE = LocaleIdToCodepage(LOWORD(KEYBOARD_INPUT_HKL));
    return 0;
  }
  return DefWindowProcA(hWnd, message, wParam, lParam);
}
static IDevice* createDevice(E_DRIVER_TYPE driverType, ISIZE windowSize, u32 bits, bool fullscreen, bool stencilbuffer, bool vsync, IEventReceiver* res)
{
  HINSTANCE hInstance;
  CDeviceWin32* c = (CDeviceWin32*)malloc(sizeof(CDeviceWin32));
  IDevice* s = c->c;
  // get windows version and create OS operator
  const char* winversion = getOperatingSystemVersion();
  memset(c, 0, sizeof(CDeviceWin32));
  s->DriverType = driverType;
  s->WindowSize = windowSize;
  s->Bits = (u8)bits;
  s->Fullscreen = fullscreen;
  s->Stencilbuffer = stencilbuffer;
  s->Vsync = vsync;
  s->UserReceiver = res;
  s->Close = (false);
  s->ColorFormat = ECF_R5G6B5;
  s->Randomizer->my_seed = 0xf0f0f0f0;
  s->FileSystem = createFileSystem();
#if 0
  VideoModeList = new CVideoModeList();
  char* s = "Irrlicht Engine version ";
  s.append(getVersion());
  os::Printer::log(s, ELL_INFORMATION);
  checkVersion(params.SDK_version_do_not_use);
#endif
  s->x = c;
  llogprintf(ELL_INFORMATION, winversion);
  // get handle to exe file
  hInstance = GetModuleHandleA(0);
  // Store original desktop mode.
  memset(&c->DesktopMode, 0, sizeof(c->DesktopMode));
  c->DesktopMode.dmSize = sizeof(c->DesktopMode);
  EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &c->DesktopMode);
  // create the window if we need to and we do not use the null device
#define TTSET(a, b, c)  s->b = CDeviceWin32_ ## b;
  IRRLICHTDEVICE_DEF(TTSET)
#undef TTSET
  if (!s->WindowId) {
    const char* ClassName = ("CDeviceWin32");
    // Register Class
    WNDCLASSEXA wcex;
    IRECT clientSize;
    DWORD style = WS_POPUP;
    wcex.cbSize     = sizeof(WNDCLASSEXA);
    wcex.style      = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc  = WndProc;
    wcex.cbClsExtra   = 0;
    wcex.cbWndExtra   = 0;
    wcex.hInstance    = hInstance;
    wcex.hIcon      = NULL;
    wcex.hCursor    = 0; // LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName  = ClassName;
    wcex.hIconSm    = 0;
    // if there is an icon, load it
    wcex.hIcon = (HICON)LoadImageA(hInstance, ("irrlicht.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    RegisterClassExA(&wcex);
    // calculate client size
    clientSize.t = 0;
    clientSize.l = 0;
    clientSize.r = s->WindowSize.w;
    clientSize.b = s->WindowSize.h;
    if (!s->Fullscreen) {
      style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    }
    AdjustWindowRect((RECT*)&clientSize, style, FALSE);
    {
      const s32 realW = clientSize.r - clientSize.l;
      const s32 realH = clientSize.b - clientSize.t;
      s32 windowL = (GetSystemMetrics(SM_CXSCREEN) - realW) / 2;
      s32 windowT = (GetSystemMetrics(SM_CYSCREEN) - realH) / 2;
      if (windowL < 0) {
        windowL = 0;
      }
      if (windowT < 0) {
        windowT = 0;  // make sure window menus are in screen on creation
      }
      if (s->Fullscreen) {
        windowL = 0;
        windowT = 0;
      }
      // create window
      c->hWnd = CreateWindowExA(0, ClassName, (""), style, windowL, windowT,
          realW, realH, NULL, NULL, hInstance, NULL);
      s->WindowId = c->hWnd;
      //    CreationParams->WindowSize.w = realW;
      //    CreationParams->WindowSize.h = realH;
      ShowWindow(c->hWnd, SW_SHOWNORMAL);
      UpdateWindow(c->hWnd);
      // fix ugly ATI driver bugs. Thanks to ariaci
      MoveWindow(c->hWnd, windowL, windowT, realW, realH, TRUE);
      // make sure everything gets updated to the real sizes
      c->Resized = true;
    }
  }
  else if (s->WindowId) {
    IRECT r;
    // attach external window
    c->hWnd = (HWND)(s->WindowId);
    GetWindowRect(c->hWnd, (RECT*)&r);
    s->WindowSize.w = r.r - r.l;
    s->WindowSize.h = r.b - r.t;
    s->Fullscreen = false;
    c->ExternalWindow = true;
  }
  // create cursor control
#if 0
  Win32CursorControl = new CCursorControl(this, CreationParams->WindowSize, hWnd, CreationParams->Fullscreen);
  CursorControl = Win32CursorControl;
  JoyControl = new SJoystickWin32Control(this);
#endif
  // initialize doubleclicks with system values
  s->MouseMultiClicks.DoubleClickTime = GetDoubleClickTime();
  // create driver
  //s->createDriver();
  if (s->VideoDriver) {
    //createGUIAndScene();
  }
  // register environment
  AddDevice(s);
  // set this as active window
  if (!c->ExternalWindow) {
    SetActiveWindow(c->hWnd);
    SetForegroundWindow(c->hWnd);
  }
  // get the codepage used for keyboard input
  KEYBOARD_INPUT_HKL = GetKeyboardLayout(0);
  KEYBOARD_INPUT_CODEPAGE = LocaleIdToCodepage(LOWORD(KEYBOARD_INPUT_HKL));
  // inform driver about the window size etc.
  CDeviceWin32_resizeIfNecessary(s);
  return s;
}
