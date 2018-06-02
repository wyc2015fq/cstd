
#include "truetype.hpp"
#include "basic_types.hpp"

#ifdef NANA_WINDOWS

#	ifndef _WIN32_WINNT
#		define _WIN32_WINNT  0x0501
#	endif

#	include <windows.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <specstrings.h>    // for _In_, etc.

#if !defined(__midl) && !defined(SORTPP_PASS)

#if (NTDDI_VERSION >= NTDDI_WINXP)

#ifdef __cplusplus

#define VERSIONHELPERAPI inline bool

#else  // __cplusplus

#define VERSIONHELPERAPI FORCEINLINE BOOL

#endif // __cplusplus

VERSIONHELPERAPI
IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
  OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, { 0 }, 0, 0 };
  DWORDLONG        const dwlConditionMask = VerSetConditionMask(
    VerSetConditionMask(
    VerSetConditionMask(
    0, VER_MAJORVERSION, VER_GREATER_EQUAL),
    VER_MINORVERSION, VER_GREATER_EQUAL),
    VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
  
  osvi.dwMajorVersion = wMajorVersion;
  osvi.dwMinorVersion = wMinorVersion;
  osvi.wServicePackMajor = wServicePackMajor;
  
  return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}

#ifndef _WIN32_WINNT_WINXP
#	define _WIN32_WINNT_WINXP                  0x0501
#endif // _WIN32_WINNT_WINXP

#ifndef _WIN32_WINNT_VISTA
#	define _WIN32_WINNT_VISTA                  0x0600
#endif // _WIN32_WINNT_VISTA

#ifndef _WIN32_WINNT_WIN7
#	define _WIN32_WINNT_WIN7                   0x0601
#endif // _WIN32_WINNT_WIN7


VERSIONHELPERAPI
IsWindowsXPOrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0);
}

VERSIONHELPERAPI
IsWindowsXPSP1OrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 1);
}

VERSIONHELPERAPI
IsWindowsXPSP2OrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 2);
}

VERSIONHELPERAPI
IsWindowsXPSP3OrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 3);
}

VERSIONHELPERAPI
IsWindowsVistaOrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0);
}

VERSIONHELPERAPI
IsWindowsVistaSP1OrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 1);
}

VERSIONHELPERAPI
IsWindowsVistaSP2OrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 2);
}

VERSIONHELPERAPI
IsWindows7OrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0);
}

VERSIONHELPERAPI
IsWindows7SP1OrGreater()
{
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 1);
}

#ifndef	_WIN32_WINNT_WIN8    //  (0x0602)
#define	_WIN32_WINNT_WIN8 (0x0602)
#endif  //	_WIN32_WINNT_WIN8(0x0602)

VERSIONHELPERAPI
IsWindows8OrGreater()
{
  
  return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0);
}

#ifndef	_WIN32_WINNT_WINBLUE   // (0x0602)
#define	_WIN32_WINNT_WINBLUE (0x0602)
#endif  //	_WIN32_WINNT_WINBLUE (0x0602)


#endif // NTDDI_VERSION

#endif // defined(__midl)
#else
#	include "posix/platform_spec.hpp"
#	include <fontconfig/fontconfig.h>
#	if defined(NANA_USE_XFT)
#		include <X11/Xft/Xft.h>
#		include <iconv.h>
#		include <fstream>
#	endif
#endif

native_font_type font_factory(const char* font_family, double size_pt, const font_style& fs, const char* ttf)
{
#ifdef NANA_WINDOWS
  
  //Make sure the length of font family less than LF_FACESIZE which is defined by Windows
  ASSERT(strlen(font_family) + 1 < LF_FACESIZE);
  
  //Translate pt to px
  HDC hDC = ::GetDC(NULL);
  LONG font_height = -static_cast<LONG>(size_pt * ::GetDeviceCaps(hDC, LOGPIXELSY) / 72);
  ::ReleaseDC(NULL, hDC);
  
  if (*(font_family) || (0 == font_height))
  {
    //Create default font object.
    NONCLIENTMETRICSA metrics = {0};
    metrics.cbSize = sizeof metrics;
#if(WINVER >= 0x0600)
#if defined(NANA_MINGW)
    OSVERSIONINFOA osvi = {};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    ::GetVersionExA(&osvi);
    if (osvi.dwMajorVersion < 6)
      metrics.cbSize -= sizeof(metrics.iPaddedBorderWidth);
#else
    if (!IsWindowsVistaOrGreater())
      metrics.cbSize -= sizeof(metrics.iPaddedBorderWidth);
#endif
#endif
    ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof metrics, &metrics, 0);
    
    if (0 == font_height)
      font_height = metrics.lfMessageFont.lfHeight;
  }
  
  ::LOGFONTA lf = {0};
  
  strcpy(lf.lfFaceName, font_family);
  lf.lfHeight = font_height;
  lf.lfCharSet = DEFAULT_CHARSET;
  lf.lfWeight = fs.weight;
  lf.lfQuality = PROOF_QUALITY;
  lf.lfPitchAndFamily = FIXED_PITCH;
  lf.lfItalic = fs.italic;
  lf.lfUnderline = fs.underline;
  lf.lfStrikeOut = fs.strike_out;
  
  HFONT fd = ::CreateFontIndirectA(&lf);
#elif defined(NANA_X11)
  auto disp = platform_spec::instance().open_display();
#	ifdef NANA_USE_XFT
  if(font_family.empty())
    font_family = '*';
  
  string pat_str = font_family + '-' + to_string(size_pt ? size_pt : 10);
  auto pat = ::XftNameParse(pat_str);
  XftResult res;
  auto match_pat = ::XftFontMatch(disp, ::XDefaultScreen(disp), pat, &res);
  
  ::XftFont* fd = NULL;
  if (match_pat)
    fd = ::XftFontOpenPattern(disp, match_pat);
#	else
  string pat_str;
  if (font_family.empty())
    pat_str = "-misc-fixed-*";
  else
    pat_str = "-misc-fixed-" + font_family;
  
  char ** missing_list;
  int missing_count;
  char * defstr;
  XFontSet fd = ::XCreateFontSet(display_, const_cast<char*>(pat_str), &missing_list, &missing_count, &defstr);
#	endif
#endif
  return (native_font_type)fd;
}

void font_resource(bool try_add, const char* ttf)
{
#ifdef NANA_WINDOWS
  if (try_add)
    ::AddFontResourceExA(ttf, FR_PRIVATE, NULL);
  else
    ::RemoveFontResourceExA(ttf, FR_PRIVATE, NULL);
#else
  auto & fc = platform_storage().fontconfig_counts;
  if(try_add)
  {
    if(1 == ++(fc[ttf.string()]))
    {
      ::FcConfigAppFontAddFile(NULL, reinterpret_cast<const FcChar8*>(ttf.string()));
    }
  }
  else
  {
    auto i = fc.find(ttf.string());
    if(i != fc.end())
    {
      if(0 == --(i->second))
        fc.erase(i);
      
      if(0 == fc.size())
        ::FcConfigAppFontClear(NULL);
    }
  }
#endif
}

native_font_type make_font(const char* font_family, double size_pt, const font_style& fs)
{
  return font_factory(font_family, size_pt, fs, NULL);
}

native_font_type make_font_from_ttf(const char* ttf, double size_pt, const font_style& fs)
{
  truetype truetype(ttf);
  if (truetype.font_family())
    return NULL;
  
  font_resource(true, ttf);
  return font_factory(truetype.font_family(), size_pt, fs, ttf);
}

native_font_type default_font(const font_style& fs)
{
#ifdef NANA_WINDOWS
  return (native_font_type)::GetStockObject(DEFAULT_GUI_FONT);
#else
  return NULL;
#endif
}


#if defined(NANA_WINDOWS)
#include "platform_spec_windows.inl"
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
#include "platform_spec_posix.inl"
#endif

//sleep
//@brief:	Suspend current thread for a specified milliseconds.
//			its precision is depended on hardware.
void sleep(unsigned milliseconds)
{
#if defined(NANA_WINDOWS)
  ::Sleep(milliseconds);
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
  struct timespec timeOut, remains;
  timeOut.tv_sec = milliseconds / 1000;
  timeOut.tv_nsec = (milliseconds % 1000) * 1000000;
  while(-1 == ::nanosleep(&timeOut, &remains))
  {
    if(errno == EINTR)
      timeOut = remains;
    else
      break;
  }
#endif
}

//this_thread_id
//@brief: get the identifier of calling thread.
unsigned long this_thread_id()
{
#if defined(NANA_WINDOWS)
  return ::GetCurrentThreadId();
#elif defined(NANA_LINUX)
  return ::syscall(__NR_gettid);
#elif defined(NANA_MACOS)
  return ::syscall(SYS_thread_selfid);
#endif
}

unsigned long timestamp()
{
#if defined(NANA_WINDOWS)
  return ::GetTickCount();
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
  struct timeval tv;
  ::gettimeofday(&tv, 0);
  return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

bool get_async_mouse_state(int button)
{
#if defined(NANA_WINDOWS)
  bool swap = (::GetSystemMetrics(SM_SWAPBUTTON) != 0);
  switch(button)
  {
  case 1: //Right
    button = swap ? VK_LBUTTON : VK_RBUTTON;
    break;
  case 2:
    button = VK_MBUTTON;
    break;
  default:
    button = swap ? VK_RBUTTON : VK_LBUTTON;
    break;
  }
  
  return (::GetAsyncKeyState(button) != 0);
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
  static_cast<void>(button);	//eliminate unused parameter compiler warning.
  return false;
#endif
}

//open an url through a default browser
void open_url(const char* url)
{
  if(!url)
    return;
  
#if defined(NANA_WINDOWS)
  // char* url = codecvt(url_gb, );
  if(::ShellExecuteA(0, "open", url, 0, 0, SW_SHOWNORMAL) < reinterpret_cast<HINSTANCE>(32))
  {
    //Because ShellExecute can delegate execution to Shell extensions (data sources, context menu handlers,
    //verb implementations) that are activated using Component Object Model (COM), COM should be initialized
    //before ShellExecute is called. Some Shell extensions require the COM single-threaded apartment (STA) type.
    //In that case, COM should be initialized under WinXP.
    co_initialized();
    ShellExecuteA(0, "open", url, 0, 0, SW_SHOWNORMAL);
  }
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
#endif
}

//This function is a proxy for ShowWindow/ShowWindowAsync
//It determines which API should be called.
void msw_show_window(HWND wd, int cmd)
{
  bool async = true;
  const DWORD tid = ::GetCurrentThreadId();
  if(tid == ::GetWindowThreadProcessId(wd, NULL))
  {
    HWND owner = ::GetWindow(wd, GW_OWNER);
    if ((NULL == owner) || (tid == ::GetWindowThreadProcessId(owner, NULL)))
    {
      async = false;
      HWND owned = ::GetWindow(wd, GW_HWNDPREV);
      while (owned)
      {
        if (::GetWindow(owned, GW_OWNER) == wd)
        {
          if (tid != ::GetWindowThreadProcessId(owned, NULL))
          {
            async = true;
            break;
          }
        }
        owned = ::GetWindow(owned, GW_HWNDPREV);
      }
    }
  }
  if (async)
  {
    ::ShowWindowAsync(wd, cmd);
    return;
  }
  
  ::ShowWindow(wd, cmd);
}

//struct native_interface
void affinity_execute(native_window_type native_handle, const void* fn)
{
  if (!fn)
    return;
  
#if defined(NANA_WINDOWS)
  HWND mswin = reinterpret_cast<HWND>(native_handle);
  if (::IsWindow(mswin))
  {
    if (::GetCurrentThreadId() != ::GetWindowThreadProcessId(mswin, NULL))
    {
      messages::arg_affinity_execute arg;
      arg.function_ptr = &fn;
      
      ::SendMessage(mswin, messages::affinity_execute, reinterpret_cast<WPARAM>(&arg), 0);
      
      return;
    }
  }
  
#else
  static_cast<void>(native_handle);
#endif	
}

size primary_monitor_size()
{
#if defined(NANA_WINDOWS)
  return size(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
#elif defined(NANA_X11)
  platform_scope_guard psg;
  Screen* s = ::XScreenOfDisplay(spec.open_display(), ::XDefaultScreen(spec.open_display()));
  return size(::XWidthOfScreen(s), ::XHeightOfScreen(s));
#endif
}

rectangle screen_area_from_point(const point& pos)
{
#if defined(NANA_WINDOWS)
  typedef HMONITOR (__stdcall * MonitorFromPointT)(POINT,DWORD);
  
  MonitorFromPointT mfp = reinterpret_cast<MonitorFromPointT>(::GetProcAddress(::GetModuleHandleA("User32.DLL"), "MonitorFromPoint"));
  if(mfp)
  {
    POINT native_pos = {pos.x, pos.y};
    HMONITOR monitor = mfp(native_pos, 2 /*MONITOR_DEFAULTTONEAREST*/);
    
    MONITORINFO mi;
    mi.cbSize = sizeof mi;
    if(::GetMonitorInfo(monitor, &mi))
    {
      return rectangle(mi.rcWork.left, mi.rcWork.top,
        mi.rcWork.right - mi.rcWork.left, mi.rcWork.bottom - mi.rcWork.top);
    }
  }
#else
  static_cast<void>(pos); //eliminate unused parameter compiler warning.
#endif
  return rectangle(primary_monitor_size());
}

#ifdef NANA_X11
//The XMoveWindow and XMoveResizeWindow don't take effect if the specified window is
//unmapped, and the members x and y of XSetSizeHints is obsoluted. So the position that
//set to a unmapped windows should be kept and use the position when the window is mapped.
map<Window, point> exposed_positions;	//locked by platform_scope_guard
#endif

#if defined(NANA_WINDOWS)
#endif


#ifdef NANA_WINDOWS


#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL	0x020A
#endif

#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL	0x020E
#endif

typedef void (CALLBACK *win_event_proc_t)(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

typedef int (__stdcall* track_mouse_event_type)(LPTRACKMOUSEEVENT);

int __stdcall dummy_track_mouse_event(LPTRACKMOUSEEVENT)
{
  return 1;
}

track_mouse_event_type track_mouse_event;

typedef HIMC (__stdcall * imm_get_context_type)(HWND);
imm_get_context_type imm_get_context;

typedef BOOL (__stdcall* imm_release_context_type)(HWND, HIMC);
imm_release_context_type imm_release_context;

typedef BOOL (__stdcall* imm_set_composition_font_type)(HIMC, LOGFONTW*);
imm_set_composition_font_type imm_set_composition_font;

typedef BOOL (__stdcall* imm_set_composition_window_type)(HIMC, LPCOMPOSITIONFORM);
imm_set_composition_window_type imm_set_composition_window;

#pragma pack(1)
//Decoder of WPARAM and LPARAM
struct wparam_button
{
  bool left:1;
  bool right:1;
  bool shift:1;
  bool ctrl:1;
  bool middle:1;
  bool place_holder:3;
  char place_holder_c[1];
  short wheel_delta;
};

struct param_mouse
{
  wparam_button button;
  short x;
  short y;
};

struct param_size
{
  unsigned long state;
  short width;
  short height;
};


union parameter_decoder
{
  struct {
    WPARAM wparam;
    LPARAM lparam;
  }raw_param;
  
  param_mouse mouse;
  param_size size;
};
#pragma pack()

static LRESULT CALLBACK Bedrock_WIN32_WindowProc(HWND root_window, UINT message, WPARAM wParam, LPARAM lParam);

struct window;
struct bedrock {
  window* hg_windows;
  bedrock()
  {
    //co_initialized();//to guaranty the platform_spec object is initialized before using.
    hg_windows = NULL;
    WNDCLASSEXA wincl;
    wincl.hInstance = ::GetModuleHandle(0);
    wincl.lpszClassName = "NanaWindowInternal";
    wincl.lpfnWndProc = Bedrock_WIN32_WindowProc;
    wincl.style = CS_DBLCLKS | CS_OWNDC;
    wincl.cbSize = sizeof(wincl);
    wincl.hIcon = ::LoadIcon (0, IDI_APPLICATION);
    wincl.hIconSm = wincl.hIcon;
    wincl.hCursor = ::LoadCursor (0, IDC_ARROW);
    wincl.lpszMenuName = 0;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    
    ::RegisterClassExA(&wincl);
    
    track_mouse_event = (track_mouse_event_type)::GetProcAddress(::GetModuleHandleA("User32.DLL"), "TrackMouseEvent");
    
    if(!track_mouse_event)
      track_mouse_event = dummy_track_mouse_event;
    
    HMODULE imm32 = ::GetModuleHandleA("Imm32.DLL");
    imm_get_context = reinterpret_cast<imm_get_context_type>(
      ::GetProcAddress(imm32, "ImmGetContext"));
    
    imm_release_context = reinterpret_cast<imm_release_context_type>(
      ::GetProcAddress(imm32, "ImmReleaseContext"));
    
    imm_set_composition_font = reinterpret_cast<imm_set_composition_font_type>(
      ::GetProcAddress(imm32, "ImmSetCompositionFontW"));
    
    imm_set_composition_window = reinterpret_cast<imm_set_composition_window_type>(
      ::GetProcAddress(imm32, "ImmSetCompositionWindow"));
  }
  
  ~bedrock()
  {
    if(0)
    {
      char *msg = "Nana.GUI detects a memory leaks in window_manager, native_window_type(s) are not uninstalled.\n";
      printf(msg);  /// \todo add list of cations of open windows and if aut testin GUI do auto Ok after 2 sec.
      ::MessageBoxA(0, msg, ("Nana C++ Library"), MB_OK);
    }
  }
  
};

static bedrock* bedrock_instance()
{
  static bedrock bedrock_object;
  return &bedrock_object;
}

#endif


//platform-dependent
native_window_type create_window(void* user, native_window_type owner, bool nested, const rectangle& r, const appearance& app = appearance())
{
  bedrock_instance();
#if defined(NANA_WINDOWS)
  DWORD style = WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE;
  DWORD style_ex= WS_EX_NOPARENTNOTIFY;
  
  if(app.minimize)	style |= WS_MINIMIZEBOX;
  if(app.maximize)	style |= WS_MAXIMIZEBOX;
  
  if(app.sizable)	style |= WS_THICKFRAME;
  
  if(app.decoration)
    style |= WS_OVERLAPPED | WS_CAPTION;
  
  style |= (nested ? WS_CHILD : WS_POPUP);
  style_ex |= (app.taskbar ? WS_EX_APPWINDOW : WS_EX_TOOLWINDOW);
  
  if(app.floating)	style_ex |= WS_EX_TOPMOST;
  
  POINT pt = {r.x, r.y};
  
  if(owner && (nested == false))::ClientToScreen(reinterpret_cast<HWND>(owner), &pt);
  
  HWND native_wd = ::CreateWindowEx(style_ex, L"NanaWindowInternal", L"Nana Window",
    style,
    pt.x, pt.y, 100, 100,
    reinterpret_cast<HWND>(owner), 0, ::GetModuleHandle(0), 0);
  
  SetWindowLongPtrA(native_wd, GWLP_USERDATA, (LONG)user);

  //A window may have a border, this should be adjusted the client area fit for the specified size.
  ::RECT client;
  ::GetClientRect(native_wd, &client);	//The right and bottom of client by GetClientRect indicate the width and height of the area
  ::RECT wd_area;
  ::GetWindowRect(native_wd, &wd_area);
  
  //a dimension with borders and caption title
  wd_area.right -= wd_area.left;	//wd_area.right = width
  wd_area.bottom -= wd_area.top;	//wd_area.bottom = height
  if (nested)
  {
    wd_area.left = pt.x;
    wd_area.top = pt.y;
  }
  
  int delta_w = static_cast<int>(r.width) - client.right;
  int delta_h = static_cast<int>(r.height) - client.bottom;
  
  ::MoveWindow(native_wd, wd_area.left, wd_area.top, wd_area.right + delta_w, wd_area.bottom + delta_h, true);
  
  ::GetClientRect(native_wd, &client);
  ::GetWindowRect(native_wd, &wd_area);
  
  wd_area.right -= wd_area.left;
  wd_area.bottom -= wd_area.top;
  
  native_window_type result = (native_window_type)(native_wd);
#elif defined(NANA_X11)
  platform_scope_guard psg;
  
  XSetWindowAttributes win_attr;
  unsigned long attr_mask = CWBackPixmap | CWBackPixel | CWBorderPixel |
    CWWinGravity | CWBitGravity | CWColormap | CWEventMask;
  
  Display * disp = spec.open_display();
  win_attr.colormap = spec.colormap();
  
  win_attr.background_pixmap = None;
  win_attr.background_pixel = 0xFFFFFF;
  win_attr.border_pixmap = None;
  win_attr.border_pixel = 0x0;
  win_attr.bit_gravity = 0;
  win_attr.win_gravity = NorthWestGravity;
  win_attr.backing_store = 0;
  win_attr.backing_planes = 0;
  win_attr.backing_pixel = 0;
  win_attr.colormap = spec.colormap();
  
  if(app.decoration == false)
  {
    win_attr.override_redirect = True;
    attr_mask |= CWOverrideRedirect;
  }
  
  Window parent = (owner ? reinterpret_cast<Window>(owner) : spec.root_window());
  point pos(r.x, r.y);
  if((false == nested) && owner)
  {
    win_attr.save_under = True;
    attr_mask |= CWSaveUnder;
    
    parent = spec.root_window();
    calc_screen_point(owner, pos);
  }
  
  win_attr.event_mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask;
  
  Window handle = ::XCreateWindow(disp, parent,
    pos.x, pos.y, (r.width ? r.width : 1), (r.height ? r.height : 1), 0,
    spec.screen_depth(), InputOutput, spec.screen_visual(),
    attr_mask, &win_attr);
  if(handle)
  {
    //make owner if it is a popup window
    if((!nested) && owner)
    {
      spec.make_owner(owner, reinterpret_cast<native_window_type>(handle));
      exposed_positions[handle] = pos;
    }
    
    XChangeWindowAttributes(disp, handle, attr_mask, &win_attr);
    
    XTextProperty name;
    char text[] = "Nana Window";
    char * str = text;
    ::XStringListToTextProperty(&str, 1, &name);
    ::XSetWMName(disp, handle, &name);
    
    const atombase_tag & ab = spec.atombase();
    ::XSetWMProtocols(disp, handle, const_cast<Atom*>(&ab.wm_delete_window), 1);
    
    struct
    {
      long flags;
      long functions;
      long decorations;
      long input;
      long status;
    }motif;
    //MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;// | MWM_HINTS_INPUT_MODE;
    motif.flags = 1 | 2; //| 4;
    motif.functions = 4;//MWM_FUNC_MOVE;
    motif.decorations = 0;
    motif.input = 0;//MWM_INPUT_MODELESS;
    motif.status = 0;
    
    XSizeHints hints;
    hints.flags = USPosition;
    hints.x = pos.x;
    hints.y = pos.y;
    
    if(app.sizable)
    {
      motif.decorations |= 4; //MWM_DECOR_RESIZEH;
      motif.functions |= 2; //MWM_FUNC_RESIZE;
    }
    else
    {
      hints.min_width = hints.max_width = r.width;
      hints.min_height = hints.max_height = r.height;
      hints.flags |= (PMinSize | PMaxSize);
    }
    ::XSetWMNormalHints(disp, handle, &hints);
    
    if(app.decoration)
    {
      if(app.minimize)
      {
        motif.decorations |= (1 << 5);	//MWM_DECOR_MINIMIZE;
        motif.functions |= (1 << 3);	//MWM_FUNC_MINIMIZE;
      }
      
      if(app.maximize)
      {
        motif.decorations |= (1 << 6);//MWM_DECOR_MAXIMIZE;
        motif.functions |= ( 1 << 4);//MWM_FUNC_MAXIMIZE;
      }
      motif.functions |= (1<<5); //MWM_FUNC_CLOSE
      motif.decorations |= (2) | 8; //MWM_DECOR_BORDER | MWM_DECOR_TITLE
    }
    
    if((false == nested) && owner)
    {
      ::XChangeProperty(disp, handle, ab.net_wm_window_type, XA_ATOM, 32, PropModeReplace,
        reinterpret_cast<unsigned char*>(const_cast<Atom*>(&ab.net_wm_window_type_dialog)), 1);
      ::XSetTransientForHint(disp, handle, reinterpret_cast<Window>(owner));
    }
    
    ::XChangeProperty(disp, handle, ab.motif_wm_hints, ab.motif_wm_hints, 32, PropModeReplace,
      reinterpret_cast<unsigned char*>(&motif), sizeof(motif)/sizeof(long));
    
    if(app.floating)
    {
      ::XChangeProperty(disp, handle, ab.net_wm_window_type, XA_ATOM, 32, PropModeReplace,
        reinterpret_cast<unsigned char*>(const_cast<Atom*>(&ab.net_wm_window_type_normal)), 1);
      ::XSetTransientForHint(disp, handle, spec.root_window());
    }
    
    if(false == app.taskbar)
    {
      ::XChangeProperty(disp, handle, ab.net_wm_state, XA_ATOM, 32, PropModeAppend,
        reinterpret_cast<unsigned char*>(const_cast<Atom*>(&ab.net_wm_state_skip_taskbar)), 1);
    }
  }
  window_result result = {reinterpret_cast<native_window_type>(handle), r.width, r.height, 0, 0};
  spec.msg_insert(reinterpret_cast<native_window_type>(handle));
#endif
  return result;
}

native_window_type create_child_window(native_window_type parent, const rectangle& r)
{
  if(NULL == parent) return NULL;
#if defined(NANA_WINDOWS)
  HWND handle = ::CreateWindowExA(WS_EX_CONTROLPARENT,		// Extended possibilites for variation
    "NanaWindowInternal",
    "Nana Child Window",	// Title Text
    WS_CHILD | WS_VISIBLE | WS_TABSTOP  | WS_CLIPSIBLINGS,
    r.x, r.y, r.width, r.height,
    reinterpret_cast<HWND>(parent),	// The window is a child-window to desktop
    0, ::GetModuleHandle(0), 0);
#elif defined(NANA_X11)
  platform_scope_guard psg;
  
  XSetWindowAttributes win_attr;
  unsigned long attr_mask = CWBackPixmap | CWBackPixel | CWBorderPixel |
    CWWinGravity | CWBitGravity | CWColormap | CWEventMask;
  
  Display * disp = spec.open_display();
  win_attr.colormap = spec.colormap();
  
  win_attr.background_pixmap = None;
  win_attr.background_pixel = 0xFFFFFF;
  win_attr.border_pixmap = None;
  win_attr.border_pixel = 0x0;
  win_attr.bit_gravity = 0;
  win_attr.win_gravity = NorthWestGravity;
  win_attr.backing_store = 0;
  win_attr.backing_planes = 0;
  win_attr.backing_pixel = 0;
  win_attr.colormap = spec.colormap();
  
  win_attr.override_redirect = True;
  attr_mask |= CWOverrideRedirect;
  
  point pos(r.x, r.y);
  win_attr.event_mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask;
  
  Window handle = ::XCreateWindow(disp, reinterpret_cast<Window>(parent),
    pos.x, pos.y, (r.width ? r.width : 1), (r.height ? r.height : 1), 0,
    spec.screen_depth(), InputOutput, spec.screen_visual(),
    attr_mask, &win_attr);
  
  if(handle)
  {
    XTextProperty name;
    char text[] = "Nana Child Window";
    char * str = text;
    ::XStringListToTextProperty(&str, 1, &name);
    ::XSetWMName(disp, handle, &name);
    
    const atombase_tag & ab = spec.atombase();
    ::XSetWMProtocols(disp, handle, const_cast<Atom*>(&ab.wm_delete_window), 1);
    
    struct
    {
      long flags;
      long functions;
      long decorations;
      long input;
      long status;
    }motif;
    //MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;// | MWM_HINTS_INPUT_MODE;
    motif.flags = 1 | 2; //| 4;
    motif.functions = 4;//MWM_FUNC_MOVE;
    motif.decorations = 0;
    motif.input = 0;//MWM_INPUT_MODELESS;
    motif.status = 0;
    
    XSizeHints hints;
    hints.flags = USPosition;
    hints.x = pos.x;
    hints.y = pos.y;
    hints.min_width = hints.max_width = r.width;
    hints.min_height = hints.max_height = r.height;
    hints.flags |= (PMinSize | PMaxSize);
    ::XSetWMNormalHints(disp, handle, &hints);
    
    ::XChangeProperty(disp, handle, ab.motif_wm_hints, ab.motif_wm_hints, 32, PropModeReplace,
      reinterpret_cast<unsigned char*>(&motif), sizeof(motif)/sizeof(long));
    
    ::XChangeProperty(disp, handle, ab.net_wm_state, XA_ATOM, 32, PropModeAppend,
      reinterpret_cast<unsigned char*>(const_cast<Atom*>(&ab.net_wm_state_skip_taskbar)), 1);
  }
#endif
  return reinterpret_cast<native_window_type>(handle);
}

#if defined(NANA_X11)
void set_modal(native_window_type wd)
{
  Window owner = reinterpret_cast<Window>(spec.get_owner(wd));
  if(wd && owner)
  {
    if(is_window_visible(wd))
      show_window(wd, false, true);
    auto disp = spec.open_display();
    auto & atombase = spec.atombase();
    ::XSetTransientForHint(disp, reinterpret_cast<Window>(wd), owner);
    ::XChangeProperty(disp, reinterpret_cast<Window>(wd),
      atombase.net_wm_state, XA_ATOM, sizeof(int) * 8,
      PropModeReplace,
      reinterpret_cast<const unsigned char*>(&atombase.net_wm_state_modal), 1);
  }
}
#endif

void enable_dropfiles(native_window_type wd, bool enb)
{
#if defined(NANA_WINDOWS)
  ::DragAcceptFiles(reinterpret_cast<HWND>(wd), enb);
#else
  int dndver = (enb ? 4: 0);
  ::XChangeProperty(spec.open_display(), reinterpret_cast<Window>(wd), spec.atombase().xdnd_aware, XA_ATOM, sizeof(int) * 8,
    PropModeReplace, reinterpret_cast<unsigned char*>(&dndver), 1);
#endif
}

void enable_window(native_window_type wd, bool is_enabled)
{
#if defined(NANA_WINDOWS)
  ::EnableWindow(reinterpret_cast<HWND>(wd), is_enabled);
#else
  int mask = ExposureMask | StructureNotifyMask;
  if(is_enabled)
  {
    mask |= (ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    mask |= (KeyPressMask | KeyReleaseMask);
    mask |= (EnterWindowMask | LeaveWindowMask | FocusChangeMask);
  }
  
  ::XSelectInput(spec.open_display(), reinterpret_cast<Window>(wd), mask);
#endif
}

bool window_icon(native_window_type wd, const native_icon_type sml_icon, const native_icon_type big_icon)
{
#if defined(NANA_WINDOWS)
  if(sml_icon || big_icon)
  {
    if (sml_icon)
      ::SendMessage(reinterpret_cast<HWND>(wd), WM_SETICON, ICON_SMALL, reinterpret_cast<WPARAM>(sml_icon));
    
    if (big_icon)
      ::SendMessage(reinterpret_cast<HWND>(wd), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(big_icon));
    return true;
  }
#elif defined(NANA_X11)
  if(wd && (!sml_icon.empty() || !big_icon.empty()))
  {
    auto & img = (sml_icon.empty() ? big_icon : sml_icon);
    
    const graphics & graph = spec.keep_window_icon(wd, img);
    XWMHints hints;
    hints.flags = IconPixmapHint;
    hints.icon_pixmap = graph.handle()->pixmap;
    
    platform_scope_guard psg;
    ::XSetWMHints(spec.open_display(), reinterpret_cast<Window>(wd), &hints);
    return true;
  }
#endif
  return false;
}

void activate_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  HWND native_wd = reinterpret_cast<HWND>(wd);
  if (::IsWindow(native_wd))
  {
    if (::GetWindowThreadProcessId(native_wd, NULL) == ::GetCurrentThreadId())
    {
      ::EnableWindow(native_wd, true);
      ::SetActiveWindow(native_wd);
      ::SetForegroundWindow(native_wd);
    }
    else
      ::PostMessage(native_wd, messages::async_activate, 0, 0);
  }
#else
  static_cast<void>(wd);	//eliminate unused parameter compiler warning.
#endif
}

void activate_owner(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  activate_window(reinterpret_cast<native_window_type>(::GetWindow(reinterpret_cast<HWND>(wd), GW_OWNER)));
#else
  static_cast<void>(wd);	//eliminate unused parameter compiler warning.
#endif
}

//close_window
//Destroy a window
void close_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  if(wd && (::DestroyWindow(reinterpret_cast<HWND>(wd)) == false))
  {
    //DestroyWindow would be failed if the calling thread is not the window thread
    //It should send a WM_DESTROY message into window thread for destroying window
    if(::GetLastError() == ERROR_ACCESS_DENIED)
      ::PostMessage(reinterpret_cast<HWND>(wd), messages::remote_thread_destroy_window, 0, 0);
  }
#elif defined(NANA_X11)
  //Under X, XDestroyWindow destroys the specified window and generats a DestroyNotify
  //event, when the client receives the event, the specified window has been already
  //destroyed. This is a feature which is different from Windows. So the following
  //works should be handled before calling XDestroyWindow.
  auto & brock = bedrock::instance();
  if(wd == brock.get_menu())
  {
    brock.erase_menu(false);
    brock.delay_restore(3);	//Restores if delay_restore is not decleard
  }
  
  Display* disp = spec.open_display();
  spec.remove(wd);
  auto iwd = brock.wd_manager().root(wd);
  if(iwd)
  {
    {
      //Before calling window_manager::destroy, make sure the window is invisible.
      //It is a behavior like Windows.
      platform_scope_guard lock;
      spec.set_error_handler();
      ::XUnmapWindow(disp, reinterpret_cast<Window>(wd));
      ::XFlush(disp);
      spec.rev_error_handler();
    }
    brock.wd_manager().destroy(iwd);
    brock.manage_form_loader(iwd, false);
    brock.wd_manager().destroy_handle(iwd);
  }
  
  platform_scope_guard psg;
  spec.set_error_handler();
  ::XDestroyWindow(disp, reinterpret_cast<Window>(wd));
  spec.rev_error_handler();
#endif
}

void show_window(native_window_type wd, bool show, bool active)
{
#if defined(NANA_WINDOWS)
  int cmd = (show ? (active ? SW_SHOW : SW_SHOWNA) : SW_HIDE);
  msw_show_window(reinterpret_cast<HWND>(wd), cmd);
#elif defined(NANA_X11)
  if(wd)
  {
    platform_scope_guard psg;
    Display* disp = spec.open_display();
    if(show)
    {
      ::XMapWindow(disp, reinterpret_cast<Window>(wd));
      
      auto i = exposed_positions.find(reinterpret_cast<Window>(wd));
      if(i != exposed_positions.end())
      {
        ::XMoveWindow(disp, reinterpret_cast<Window>(wd), i->second.x, i->second.y);
        exposed_positions.erase(i);
      }
      
      Window grab = spec.grab(0);
      if(grab == reinterpret_cast<Window>(wd))
        capture_window(wd, true);
    }
    else
      ::XUnmapWindow(disp, reinterpret_cast<Window>(wd));
    
    ::XFlush(disp);
  }
  static_cast<void>(active);	//eliminate unused parameter compiler warning.
#endif
}

void restore_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  msw_show_window(reinterpret_cast<HWND>(wd), SW_RESTORE);
#elif defined(NANA_X11)
  //Restore the window by removing NET_WM_STATE_MAXIMIZED_HORZ,
  //_NET_WM_STATE_MAXIMIZED_VERT and _NET_WM_STATE_FULLSCREEN.
  Display * disp = spec.open_display();
  Window default_root = XDefaultRootWindow(disp);
  const atombase_tag & atombase = spec.atombase();
  XEvent evt;
  evt.xclient.type = ClientMessage;
  evt.xclient.display = spec.open_display();
  evt.xclient.message_type = atombase.net_wm_state;
  evt.xclient.format = 32;
  evt.xclient.window = reinterpret_cast<Window>(wd);
  evt.xclient.data.l[0] = 0;	//_NET_WM_STATE_REMOVE
  evt.xclient.data.l[1] = atombase.net_wm_state_maximized_horz;
  evt.xclient.data.l[2] = atombase.net_wm_state_maximized_vert;
  evt.xclient.data.l[3] = evt.xclient.data.l[4] = 0;
  
  platform_scope_guard psg;
  ::XSendEvent(disp, default_root, False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
  evt.xclient.data.l[1] = atombase.net_wm_state_fullscreen;
  evt.xclient.data.l[2] = 0;
  ::XSendEvent(disp, default_root, False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
  
  //Transfer the window from IconState to NormalState.
  evt.xclient.message_type = atombase.wm_change_state;
  evt.xclient.data.l[0] = NormalState;
  evt.xclient.data.l[1] = 0;
  ::XSendEvent(disp, default_root, False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
  ::XMapWindow(disp, reinterpret_cast<Window>(wd));
  spec.set_error_handler();
  ::XSetInputFocus(disp, reinterpret_cast<Window>(wd), RevertToPointerRoot, CurrentTime);
#endif
}

void zoom_window(native_window_type wd, bool ask_for_max)
{
#if defined(NANA_WINDOWS)
  msw_show_window(reinterpret_cast<HWND>(wd), ask_for_max ? SW_MAXIMIZE : SW_MINIMIZE);
#elif defined(NANA_X11)
  Display * disp = spec.open_display();
  if (ask_for_max)
  {
    const atombase_tag & atombase = spec.atombase();
    XEvent evt;
    evt.xclient.type = ClientMessage;
    evt.xclient.display = spec.open_display();
    evt.xclient.message_type = atombase.net_wm_state;
    evt.xclient.format = 32;
    evt.xclient.window = reinterpret_cast<Window>(wd);
    evt.xclient.data.l[0] = 1;	//_NET_WM_STATE_ADD
    evt.xclient.data.l[1] = atombase.net_wm_state_maximized_horz;
    evt.xclient.data.l[2] = atombase.net_wm_state_maximized_vert;
    evt.xclient.data.l[3] = evt.xclient.data.l[4] = 0;
    
    platform_scope_guard psg;
    ::XSendEvent(disp, XDefaultRootWindow(disp), False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
    ::XMapWindow(disp, reinterpret_cast<Window>(wd));
  }
  else
    ::XIconifyWindow(disp, reinterpret_cast<Window>(wd), XDefaultScreen(disp));
#endif
}

void refresh_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  ::InvalidateRect(reinterpret_cast<HWND>(wd), NULL, true);
#elif defined(NANA_X11)
  static_cast<void>(wd); //eliminate unused parameter compiler warning.
#endif
}

bool is_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  return (FALSE != ::IsWindow(reinterpret_cast<HWND>(wd)));
#elif defined(NANA_X11)
  platform_scope_guard psg;
  XWindowAttributes attr;
  spec.set_error_handler();
  ::XGetWindowAttributes(spec.open_display(), reinterpret_cast<Window>(wd), &attr);
  return (BadWindow != spec.rev_error_handler());
#endif
}

bool is_window_visible(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  return (FALSE != ::IsWindowVisible(reinterpret_cast<HWND>(wd)));
#elif defined(NANA_X11)
  platform_scope_guard psg;
  XWindowAttributes attr;
  spec.set_error_handler();
  ::XGetWindowAttributes(spec.open_display(), reinterpret_cast<Window>(wd), &attr);
  return (BadWindow != spec.rev_error_handler() &&
    attr.map_state != IsUnmapped);
#endif
}

bool is_window_zoomed(native_window_type wd, bool ask_for_max)
{
#if defined(NANA_WINDOWS)
  return (FALSE != (ask_for_max ? ::IsZoomed(reinterpret_cast<HWND>(wd)) : ::IsIconic(reinterpret_cast<HWND>(wd))));
#elif defined(NANA_X11)
  platform_scope_guard psg;
  bool zoomed = false;
  unsigned long n,i; Atom type; unsigned char *prop; int format;
  if(Success== ::XGetWindowProperty(spec.open_display(), reinterpret_cast<Window>(wd), spec.atombase().net_wm_state, 0, 2, false, AnyPropertyType, &type, &format, &n, &i, &prop))
  {
    if(32 == format)
    {
      if(ask_for_max)
      {
        if(type == XA_ATOM)
        {
          for(i=0; i<n; i++)
          {
            if(reinterpret_cast<Atom*>(prop)[i] == spec.atombase().net_wm_state_fullscreen)
            {
              zoomed = true;
              break;
            }
          }
        }
      }
      else
        zoomed = (IconicState == *reinterpret_cast<unsigned*>(prop));
    }
    XFree(prop);
  }
  return zoomed;
#endif
}

point window_position(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  ::RECT r;
  ::GetWindowRect(reinterpret_cast<HWND>(wd), & r);
  HWND coord_wd = ::GetWindow(reinterpret_cast<HWND>(wd), GW_OWNER);
  
  if (!coord_wd)
    coord_wd = ::GetParent(reinterpret_cast<HWND>(wd));
  
  if (coord_wd)
  {
    ::POINT pos = {r.left, r.top};
    ::ScreenToClient(coord_wd, &pos);
    return point(pos.x, pos.y);
  }
  return point(r.left, r.top);
#elif defined(NANA_X11)
  int x, y;
  platform_scope_guard psg;
  Window coord_wd = reinterpret_cast<Window>(spec.get_owner(wd));
  if(!coord_wd)
  {
    coord_wd = reinterpret_cast<Window>(parent_window(wd));
    if(!coord_wd)
      coord_wd = spec.root_window();
  }
  Window child;
  if(True == ::XTranslateCoordinates(spec.open_display(), reinterpret_cast<Window>(wd), coord_wd, 0, 0, &x, &y, &child))
    return point(x, y);
  return point(0, 0);
#endif
}

void move_window(native_window_type wd, int x, int y)
{
#if defined(NANA_WINDOWS)
  ::RECT r;
  ::GetWindowRect(reinterpret_cast<HWND>(wd), &r);
  HWND owner = ::GetWindow(reinterpret_cast<HWND>(wd), GW_OWNER);
  if(owner)
  {
    ::RECT owner_rect;
    ::GetWindowRect(owner, &owner_rect);
    ::POINT pos = {owner_rect.left, owner_rect.top};
    ::ScreenToClient(owner, &pos);
    x += (owner_rect.left - pos.x);
    y += (owner_rect.top - pos.y);
  }
  
  
  if (::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), 0) != ::GetCurrentThreadId())
  {
    ::MoveWindow(reinterpret_cast<HWND>(wd), x, y, r.right - r.left, r.bottom - r.top, true);
  }
  else
    ::MoveWindow(reinterpret_cast<HWND>(wd), x, y, r.right - r.left, r.bottom - r.top, true);
#elif defined(NANA_X11)
  Display * disp = spec.open_display();
  
  platform_scope_guard psg;
  Window owner = reinterpret_cast<Window>(spec.get_owner(wd));
  if(owner)
  {
    Window child;
    ::XTranslateCoordinates(disp, owner, spec.root_window(),
      x, y, &x, &y, &child);
  }
  
  XWindowAttributes attr;
  ::XGetWindowAttributes(disp, reinterpret_cast<Window>(wd), &attr);
  if(attr.map_state == IsUnmapped)
    exposed_positions[reinterpret_cast<Window>(wd)] = point{x, y};
  
  ::XMoveWindow(disp, reinterpret_cast<Window>(wd), x, y);
#endif
}

bool move_window(native_window_type wd, const rectangle& r)
{
#if defined(NANA_WINDOWS)
  
  int x = r.x;
  int y = r.y;
  HWND owner = ::GetWindow(reinterpret_cast<HWND>(wd), GW_OWNER);
  if(owner)
  {
    ::RECT owner_rect;
    ::GetWindowRect(owner, &owner_rect);
    ::POINT pos = {owner_rect.left, owner_rect.top};
    ::ScreenToClient(owner, &pos);
    x += (owner_rect.left - pos.x);
    y += (owner_rect.top - pos.y);
  }
  
  RECT client, wd_area;
  ::GetClientRect(reinterpret_cast<HWND>(wd), &client);
  ::GetWindowRect(reinterpret_cast<HWND>(wd), &wd_area);
  unsigned ext_w = (wd_area.right - wd_area.left) - client.right;
  unsigned ext_h = (wd_area.bottom - wd_area.top) - client.bottom;
  
  if (::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), 0) != ::GetCurrentThreadId())
  {
    return (FALSE != ::MoveWindow(reinterpret_cast<HWND>(wd), x, y, r.width + ext_w, r.height + ext_h, true));
  }
  
  return (FALSE != ::MoveWindow(reinterpret_cast<HWND>(wd), x, y, r.width + ext_w, r.height + ext_h, true));
#elif defined(NANA_X11)
  Display * disp = spec.open_display();
  long supplied;
  XSizeHints hints;
  platform_scope_guard psg;
  
  ::XGetWMNormalHints(disp, reinterpret_cast<Window>(wd), &hints, &supplied);
  if((hints.flags & (PMinSize | PMaxSize)) && (hints.min_width == hints.max_width) && (hints.min_height == hints.max_height))
  {
    hints.flags = PMinSize | PMaxSize;
    hints.min_width = hints.max_width = r.width;
    hints.min_height = hints.max_height = r.height;
  }
  else
    hints.flags = 0;
  
  Window owner = reinterpret_cast<Window>(spec.get_owner(wd));
  int x = r.x;
  int y = r.y;
  if(owner)
  {
    Window child;
    ::XTranslateCoordinates(disp, owner, spec.root_window(),
      r.x, r.y, &x, &y, &child);
  }
  
  XWindowAttributes attr;
  ::XGetWindowAttributes(disp, reinterpret_cast<Window>(wd), &attr);
  if(attr.map_state == IsUnmapped)
  {
    hints.flags |= USSize;
    hints.width = r.width;
    hints.height = r.height;
    
    exposed_positions[reinterpret_cast<Window>(wd)] = point{x, y};
  }
  
  if(hints.flags)
    ::XSetWMNormalHints(disp, reinterpret_cast<Window>(wd), &hints);
  
  ::XMoveResizeWindow(disp, reinterpret_cast<Window>(wd), x, y, r.width, r.height);
  return true;
#endif
}

void bring_top(native_window_type wd, bool activated)
{
#if defined(NANA_WINDOWS)
  HWND native_wd = reinterpret_cast<HWND>(wd);
  
  if (FALSE == ::IsWindow(native_wd))
    return;
  
  HWND fg_wd = ::GetForegroundWindow();
  DWORD fg_tid = ::GetWindowThreadProcessId(fg_wd, NULL);
  ::AttachThreadInput(::GetCurrentThreadId(), fg_tid, TRUE);
  ::ShowWindow(native_wd, activated ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE);
  ::SetWindowPos(native_wd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
  ::SetWindowPos(native_wd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
  ::AttachThreadInput(::GetCurrentThreadId(), fg_tid, FALSE);
#else
  static_cast<void>(activated); //eliminate unused parameter compiler warning.
  set_window_z_order(wd, NULL, z_order_action_top);
#endif
}

void set_window_z_order(native_window_type wd, native_window_type wd_after, z_order_action action_if_no_wd_after)
{
#if defined(NANA_WINDOWS)
  HWND wa = reinterpret_cast<HWND>(wd_after);
  if(wa == 0)
  {
    switch(action_if_no_wd_after)
    {
    case z_order_action_bottom : wa = HWND_BOTTOM;	break;
    case z_order_action_top: wa = HWND_TOP;		break;
    case z_order_action_topmost: wa = HWND_TOPMOST;	break;
    case z_order_action_foreground:
      ::SetForegroundWindow(reinterpret_cast<HWND>(wd));
      return;
    default:
      wa = HWND_NOTOPMOST;
    }
  }
  if(::GetCurrentThreadId() != ::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), 0))
    ::PostMessage(reinterpret_cast<HWND>(wd), messages::remote_thread_set_window_pos, reinterpret_cast<WPARAM>(wa), SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
  else
    ::SetWindowPos(reinterpret_cast<HWND>(wd), wa, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
#elif defined(NANA_X11)
  platform_scope_guard psg;
  Display * disp = spec.open_display();
  if(0 == wd_after)
  {
    switch(action_if_no_wd_after)
    {
    case z_order_action_bottom:
      ::XLowerWindow(disp, reinterpret_cast<Window>(wd));
      break;
    case z_order_action_foreground:
    case z_order_action_top:
    case z_order_action_topmost:
      ::XRaiseWindow(disp, reinterpret_cast<Window>(wd));
      break;
    default:	//z_order_action_none
      break;
    }
  }
  else
  {
    //If the handle wd is a top level, XConfigureWindow() will be failed with a BadMatch.
    //The fix is to use XReconfigureWMWindow() instead.
    
    XWindowChanges values;
    values.sibling = reinterpret_cast<Window>(wd_after);
    values.stack_mode = Below;
    ::XReconfigureWMWindow(disp, reinterpret_cast<Window>(wd), ::XDefaultScreen(disp), CWSibling | CWStackMode, &values);
  }
#endif
}

bool window_size(native_window_type wd, const size& sz)
{
#if defined(NANA_WINDOWS)
  ::RECT r;
  ::GetWindowRect(reinterpret_cast<HWND>(wd), &r);
  HWND owner = ::GetWindow(reinterpret_cast<HWND>(wd), GW_OWNER);
  HWND parent = ::GetParent(reinterpret_cast<HWND>(wd));
  if(parent && (parent != owner))
  {
    ::POINT pos = {r.left, r.top};
    ::ScreenToClient(parent, &pos);
    r.left = pos.x;
    r.top = pos.y;
  }
  
  if (::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), 0) != ::GetCurrentThreadId())
  {
    return (FALSE != ::MoveWindow(reinterpret_cast<HWND>(wd), r.left, r.top, static_cast<int>(sz.width), static_cast<int>(sz.height), true));
  }
  
  return (FALSE != ::MoveWindow(reinterpret_cast<HWND>(wd), r.left, r.top, static_cast<int>(sz.width), static_cast<int>(sz.height), true));
#elif defined(NANA_X11)
  auto disp = spec.open_display();
  platform_scope_guard psg;
  
  //Check the XSizeHints for testing whether the window is sizable.
  XSizeHints hints;
  long supplied;
  ::XGetWMNormalHints(disp, reinterpret_cast<Window>(wd), &hints, &supplied);
  if((hints.flags & (PMinSize | PMaxSize)) && (hints.min_width == hints.max_width) && (hints.min_height == hints.max_height))
  {
    hints.flags = PMinSize | PMaxSize;
    hints.min_width = hints.max_width = sz.width;
    hints.min_height = hints.max_height = sz.height;
    ::XSetWMNormalHints(disp, reinterpret_cast<Window>(wd), &hints);
  }
  ::XResizeWindow(disp, reinterpret_cast<Window>(wd), sz.width, sz.height);
  return true;
#endif
}

void get_window_rect(native_window_type wd, rectangle& r)
{
#if defined(NANA_WINDOWS)
  ::RECT winr;
  ::GetWindowRect(reinterpret_cast<HWND>(wd), &winr);
  r.x = winr.left;
  r.y = winr.top;
  r.width = winr.right - winr.left;
  r.height = winr.bottom - winr.top;
#elif defined(NANA_X11)
  Window root;
  int x, y;
  unsigned border, depth;
  platform_scope_guard psg;
  ::XGetGeometry(spec.open_display(), reinterpret_cast<Window>(wd), &root, &x, &y, &r.width, &r.height, &border, &depth);
#endif
}

void window_caption(native_window_type wd, const char* title)
{
#if defined(NANA_WINDOWS)
  if(::GetCurrentThreadId() != ::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), 0))
  {
    ::PostMessageA(reinterpret_cast<HWND>(wd), messages::remote_thread_set_window_text, reinterpret_cast<WPARAM>(title), 0);
  }
  else
    ::SetWindowTextA(reinterpret_cast<HWND>(wd), title);
#elif defined(NANA_X11)
  ::XTextProperty name;
  char * text = const_cast<char*>(title);
  
  platform_scope_guard psg;
  ::XStringListToTextProperty(&text, 1, &name);
  ::XSetWMName(spec.open_display(), reinterpret_cast<Window>(wd), &name);
  ::XChangeProperty(spec.open_display(), reinterpret_cast<Window>(wd),
    spec.atombase().net_wm_name, spec.atombase().utf8_string, 8,
    PropModeReplace, reinterpret_cast<unsigned char*>(text), title.size());
#endif
}

char* window_caption(native_window_type wd, char* str, int strsize)
{
#if defined(NANA_WINDOWS)
  bool is_current_thread = (::GetCurrentThreadId() == ::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), NULL));
  
  int length = ::GetWindowTextLength(reinterpret_cast<HWND>(wd));
  if(length > 0 && str)
  {
    //One for NULL terminator which GetWindowText will write.
    ::GetWindowTextA(reinterpret_cast<HWND>(wd), &(str[0]), static_cast<int>(strsize));
    
    //Remove the null terminator writtien by GetWindowText
  }
  
#elif defined(NANA_X11)
  platform_scope_guard psg;
  ::XTextProperty txtpro;
  if(::XGetWMName(spec.open_display(), reinterpret_cast<Window>(wd), &txtpro))
  {
    char ** strlist;
    int size;
    if(::XTextPropertyToStringList(&txtpro, &strlist, &size))
    {
      if(size > 1)
      {
        str = *strlist;
        ::XFreeStringList(strlist);
      }
    }
  }
#endif
  return str;
}

void capture_window(native_window_type wd, bool cap)
{
#if defined(NANA_WINDOWS)
  if(cap)
    ::SetCapture(reinterpret_cast<HWND>(wd));
  else
    ::ReleaseCapture();
#elif defined(NANA_X11)
  platform_scope_guard psg;
  if(cap)
  {
    const unsigned mask = ButtonPressMask|ButtonReleaseMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask;
    if(GrabNotViewable == ::XGrabPointer(spec.open_display(), reinterpret_cast<Window>(wd), false, mask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime))
    {
      spec.grab(reinterpret_cast<Window>(wd));
    }
  }
  else
  {
    ::XUngrabPointer(spec.open_display(), CurrentTime);
    ::XFlush(spec.open_display());
    spec.grab(0);
  }
#endif
}

point cursor_position()
{
#if defined(NANA_WINDOWS)
  POINT pt;
  ::GetCursorPos(&pt);
  return point(pt.x, pt.y);
#elif defined(NANA_X11)
  point pos;
  Window drop_wd;
  int x, y;
  unsigned mask;
  platform_scope_guard psg;
  ::XQueryPointer(spec.open_display(), spec.root_window(), &drop_wd, &drop_wd,  &pos.x, &pos.y, &x, &y, &mask);
  return pos;
#endif
}

native_window_type get_owner_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  return reinterpret_cast<native_window_type>(::GetWindow(reinterpret_cast<HWND>(wd), GW_OWNER));
#elif defined(NANA_X11)
  return spec.get_owner(wd);
#endif
}

native_window_type parent_window(native_window_type wd)
{
#ifdef NANA_WINDOWS
  return reinterpret_cast<native_window_type>(::GetParent(reinterpret_cast<HWND>(wd)));
#elif defined(NANA_X11)
  Window root;
  Window parent;
  Window * children;
  unsigned size;
  
  platform_scope_guard lock;
  
  if(0 != ::XQueryTree(spec.open_display(), reinterpret_cast<Window>(wd),
    &root, &parent, &children, &size))
  {
    ::XFree(children);
    return reinterpret_cast<native_window_type>(parent);
  }
  return NULL;
#endif
}

native_window_type parent_window(native_window_type child, native_window_type new_parent, bool returns_previous)
{
#ifdef NANA_WINDOWS
  HWND prev = ::SetParent(reinterpret_cast<HWND>(child), reinterpret_cast<HWND>(new_parent));
  
  if (prev)
    ::PostMessage(prev, /*WM_CHANGEUISTATE*/0x0127, /*UIS_INITIALIZE*/ 3, 0);
  
  ::SetWindowPos(reinterpret_cast<HWND>(child), NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
  
  return reinterpret_cast<native_window_type>(returns_previous ? prev : NULL);
#elif defined(NANA_X11)
  native_window_type prev = NULL;
  
  platform_scope_guard lock;
  
  if(returns_previous)
    prev = parent_window(child);
  
  ::XReparentWindow(spec.open_display(),
    reinterpret_cast<Window>(child), reinterpret_cast<Window>(new_parent),
    0, 0);
  return prev;
#endif
}

void caret_create(native_window_type wd, const size& caret_sz)
{
#if defined(NANA_WINDOWS)
  ::CreateCaret(reinterpret_cast<HWND>(wd), 0, int(caret_sz.width), int(caret_sz.height));
#elif defined(NANA_X11)
  platform_scope_guard psg;
  spec.caret_open(wd, caret_sz);
#endif
}

void caret_destroy(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  if(::GetCurrentThreadId() != ::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), 0))
    ::PostMessage(reinterpret_cast<HWND>(wd), messages::operate_caret, 1, 0);
  else
    ::DestroyCaret();
#elif defined(NANA_X11)
  platform_scope_guard psg;
  spec.caret_close(wd);
#endif
}

void caret_pos(native_window_type wd, const point& pos)
{
#if defined(NANA_WINDOWS)
  if(::GetCurrentThreadId() != ::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), 0))
  {
    messages::caret* cp = new messages::caret;
    cp->x = pos.x;
    cp->y = pos.y;
    ::PostMessage(reinterpret_cast<HWND>(wd), messages::operate_caret, 2, reinterpret_cast<LPARAM>(cp));
  }
  else
    ::SetCaretPos(pos.x, pos.y);
#elif defined(NANA_X11)
  platform_scope_guard psg;
  spec.caret_pos(wd, pos);
#endif
}

void caret_visible(native_window_type wd, bool vis)
{
#if defined(NANA_WINDOWS)
  (vis ? ::ShowCaret : ::HideCaret)(reinterpret_cast<HWND>(wd));
#elif defined(NANA_X11)
  platform_scope_guard psg;
  spec.caret_visible(wd, vis);
#endif
}

void set_focus(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  if(wd && (::GetFocus() != reinterpret_cast<HWND>(wd)))
  {
    if(::GetCurrentThreadId() != ::GetWindowThreadProcessId(reinterpret_cast<HWND>(wd), NULL))
      ::PostMessage(reinterpret_cast<HWND>(wd), messages::async_set_focus, 0, 0);
    else
    {
      
      ::SetFocus(reinterpret_cast<HWND>(wd));
    }
  }
#elif defined(NANA_X11)
  platform_scope_guard lock;
  XWindowAttributes attr;
  ::XGetWindowAttributes(spec.open_display(), reinterpret_cast<Window>(wd), &attr);
  //Make sure the window is mapped before setting focus.
  if(IsViewable == attr.map_state)
    ::XSetInputFocus(spec.open_display(), reinterpret_cast<Window>(wd), RevertToPointerRoot, CurrentTime);
#endif
}

native_window_type get_focus_window()
{
#if defined(NANA_WINDOWS)
  return reinterpret_cast<native_window_type>(::GetFocus());
#elif defined(NANA_X11)
  platform_scope_guard psg;
  Window wd;
  int revert;
  ::XGetInputFocus(spec.open_display(), &wd, &revert);
  return reinterpret_cast<native_window_type>(wd);
#endif
}

bool calc_screen_point(native_window_type wd, point& pos)
{
#if defined(NANA_WINDOWS)
  POINT point = {pos.x, pos.y};
  if(::ClientToScreen(reinterpret_cast<HWND>(wd), &point))
  {
    pos.x = point.x;
    pos.y = point.y;
    return true;
  }
  return false;
#elif defined(NANA_X11)
  platform_scope_guard psg;
  int x = pos.x, y = pos.y;
  Window child;
  return (True == ::XTranslateCoordinates(spec.open_display(),
    reinterpret_cast<Window>(wd), spec.root_window(), x, y, &pos.x, &pos.y, &child));
#endif
}

bool calc_window_point(native_window_type wd, point& pos)
{
#if defined(NANA_WINDOWS)
  POINT point = {pos.x, pos.y};
  if(::ScreenToClient(reinterpret_cast<HWND>(wd), &point))
  {
    pos.x = point.x;
    pos.y = point.y;
    return true;
  }
  return false;
#elif defined(NANA_X11)
  platform_scope_guard psg;
  int x = pos.x, y = pos.y;
  Window child;
  return (True == ::XTranslateCoordinates(spec.open_display(),
    spec.root_window(), reinterpret_cast<Window>(wd), x, y, &pos.x, &pos.y, &child));
#endif
}

native_window_type find_window(int x, int y)
{
#if defined(NANA_WINDOWS)
  POINT pos = {x, y};
  return reinterpret_cast<native_window_type>(::WindowFromPoint(pos));
#elif defined(NANA_X11)
  platform_scope_guard psg;
  
  Window root = spec.root_window();
  Window wd = root;
  Window child = 0;
  int dropx = 0, dropy = 0;
  while(True == ::XTranslateCoordinates(spec.open_display(), root, wd, x, y, &dropx, &dropy, &child))
  {
    if(0 == child) break;
    wd = child;
  }
  return reinterpret_cast<native_window_type>(wd);
#endif
}

size check_track_size(size sz, unsigned ext_width, unsigned ext_height, bool true_for_max)
{
#if defined(NANA_WINDOWS)
  int x;
  int y;
  if(true_for_max)
  {
    x = ::GetSystemMetrics(SM_CXMAXTRACK);
    y = ::GetSystemMetrics(SM_CYMAXTRACK);
    if(static_cast<unsigned>(x) < sz.width + ext_width)
      sz.width = static_cast<unsigned>(x);
    if(static_cast<unsigned>(y) < sz.height + ext_height)
      sz.height = static_cast<unsigned>(y);
  }
  else
  {
    x = ::GetSystemMetrics(SM_CXMINTRACK);
    y = ::GetSystemMetrics(SM_CYMINTRACK);
    if(static_cast<unsigned>(x) > sz.width + ext_width)
      sz.width = static_cast<unsigned>(x);
    if(static_cast<unsigned>(y) > sz.height + ext_height)
      sz.height = static_cast<unsigned>(y);
  }
#else
  //eliminate unused parameter compiler warning.
  static_cast<void>(ext_width);
  static_cast<void>(ext_height);
  static_cast<void>(true_for_max);
#endif
  return sz;
}
//end struct native_interface

/// a window data structure descriptor 
struct widget;
struct window
{
#if defined(NANA_LINUX) || defined(NANA_MACOS)
  point	pos_native;
#endif
  point	pos_root;	///< coordinates of the root window
  point	pos_owner;
  size	dimension;
  size	min_track_size;
  size	max_track_size;
  
  bool	visible;
  
  unsigned extra_width;
  unsigned extra_height;
  
  window	*next;
  window	*prev;
  window	*parent;
  window	*owner;
  
  char*		title;
  cursor	predef_cursor;
  
  native_window_type	root;		    ///< root Window handle
  unsigned			thread_id;		///< the identifier of the thread that created the window.
  unsigned			index;
  widget*			children;
  window_flags flags;
  window(native_window_type owner, bool nested, const rectangle& r, const appearance& app = appearance()) {
    bedrock* bed = bedrock_instance();
    children = NULL;
    next = NULL;
    prev = NULL;
    parent = NULL;
    owner = NULL;
    root = create_window(this, NULL, false, r, app);
    if(bed->hg_windows) {
      bed->hg_windows->prev = this;
    }
    prev = NULL;
    next = bed->hg_windows;
    bed->hg_windows = this;
  }
  virtual int paint(native_window_type wnd, native_graphics_type hdc, rectangle* rect);
};

struct widget
{
  public:
  widget* next;
  widget* prev;

  window* win;
  widget(window* win) {
    if (win->children) {
      win->children->prev = this;
    }
    this->prev = NULL;
    this->next = win->children;
    win->children = this;
  }
  ~widget() {
    if (prev) {
      prev->next = next;
    }
    if (next) {
      next->prev = prev;
    }
    win->children = this;
    win->children = next;
  }
  virtual int paint(native_window_type wnd, native_graphics_type hdc, rectangle* rect) {return 0;}
};

int window::paint(native_window_type wnd, native_graphics_type hdc, rectangle* rect) {
  widget* wid = children;
  for (;wid;wid = wid->next) {
    wid->paint(wnd, hdc, rect);
  }
  return 0;
}
