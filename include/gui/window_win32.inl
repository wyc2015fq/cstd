#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#pragma comment(lib, "Comctl32.lib")
typedef void (CALLBACK* win_event_proc_t)(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
typedef int (__stdcall* track_mouse_event_type)(LPTRACKMOUSEEVENT);
int __stdcall dummy_track_mouse_event(LPTRACKMOUSEEVENT x)
{
  return 1;
}
track_mouse_event_type track_mouse_event;
typedef HIMC(__stdcall* imm_get_context_type)(HWND);
imm_get_context_type imm_get_context;
typedef BOOL (__stdcall* imm_release_context_type)(HWND, HIMC);
imm_release_context_type imm_release_context;
typedef BOOL (__stdcall* imm_set_composition_font_type)(HIMC, LOGFONTW*);
imm_set_composition_font_type imm_set_composition_font;
typedef BOOL (__stdcall* imm_set_composition_window_type)(HIMC, LPCOMPOSITIONFORM);
imm_set_composition_window_type imm_set_composition_window;
static LRESULT WINAPI Bedrock_WIN32_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int window_init()
{
  static int inited = 0;
  if (!inited) {
    WNDCLASSEXA wincl;
    wincl.hInstance = GetModuleHandle(0);
    wincl.lpszClassName = "NanaWindowInternal";
    wincl.lpfnWndProc = &Bedrock_WIN32_WindowProc;
    wincl.style = CS_DBLCLKS | CS_OWNDC;
    wincl.cbSize = sizeof(wincl);
    wincl.hIcon = LoadIcon(0, IDI_APPLICATION);
    wincl.hIconSm = wincl.hIcon;
    wincl.hCursor = LoadCursor(0, IDC_ARROW);
    wincl.lpszMenuName = 0;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    RegisterClassExA(&wincl);
    track_mouse_event = (track_mouse_event_type)GetProcAddress(GetModuleHandleA("User32.DLL"), "TrackMouseEvent");
    if (!track_mouse_event) {
      track_mouse_event = dummy_track_mouse_event;
    }
    {
      HMODULE imm32 = GetModuleHandleA("Imm32.DLL");
      imm_get_context = (imm_get_context_type)(GetProcAddress(imm32, "ImmGetContext"));
      imm_release_context = (imm_release_context_type)(GetProcAddress(imm32, "ImmReleaseContext"));
      imm_set_composition_font = (imm_set_composition_font_type)(GetProcAddress(imm32, "ImmSetCompositionFontW"));
      imm_set_composition_window = (imm_set_composition_window_type)(GetProcAddress(imm32, "ImmSetCompositionWindow"));
    }
    inited = 1;
  }
  return inited;
}
int msw_show_window(HWND hwnd, int cmd)
{
  bool async = true;
  const DWORD tid = GetCurrentThreadId();
  if (tid == GetWindowThreadProcessId(hwnd, NULL)) {
    HWND owner = GetWindow(hwnd, GW_OWNER);
    if ((NULL == owner) || (tid == GetWindowThreadProcessId(owner, NULL))) {
      HWND owned = GetWindow(hwnd, GW_HWNDPREV);
      async = false;
      while (owned) {
        if (GetWindow(owned, GW_OWNER) == hwnd) {
          if (tid != GetWindowThreadProcessId(owned, NULL)) {
            async = true;
            break;
          }
        }
        owned = GetWindow(owned, GW_HWNDPREV);
      }
    }
  }
  if (async) {
    ShowWindowAsync(hwnd, cmd);
    return 0;
  }
  ShowWindow(hwnd, cmd);
  return 0;
}
int pump_event(native_window_t* modal_window, bool is_modal)
{
  //const unsigned tid = GetCurrentThreadId();
  native_window_t* g_root = opt_native_window(0);
  MSG msg;
  if (0 == g_root) {
    //test if there is not a window
    //GetMessage may block if there is not a window
    return 0;
  }
  if (modal_window) {
    HWND native_handle = (modal_window->hwnd);
    if (is_modal) {
      HWND owner = GetWindow(native_handle, GW_OWNER);
      if (owner && owner != GetDesktopWindow()) {
        EnableWindow(owner, false);
      }
      while (IsWindow(native_handle)) {
        WaitMessage();
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
          if (msg.message == WM_QUIT) {
            break;
          }
          if ((WM_KEYFIRST <= msg.message && msg.message <= WM_KEYLAST) || !IsDialogMessage(native_handle, &msg)) {
            //auto menu_wd = get_menu((native_window_type)(msg.hwnd), true);
            //if (menu_wd) interior_helper_for_menu(msg, menu_wd);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
          }
        }
      }
    }
    else {
      while (IsWindow(native_handle)) {
        if (-1 != GetMessage(&msg, 0, 0, 0)) {
          //auto menu_wd = get_menu((native_window_type)(msg.hwnd), true);
          //if (menu_wd) interior_helper_for_menu(msg, menu_wd);
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
        //wd_manager().call_safe_place(tid);
        //wd_manager().remove_trash_handle(tid);
        if (msg.message == WM_DESTROY && msg.hwnd == native_handle) {
          break;
        }
      }//end while
    }
  }
  else {
    while (opt_native_window(0)) {
      if (-1 != GetMessage(&msg, 0, 0, 0)) {
        //auto menu_wd = get_menu((native_window_type)(msg.hwnd), true);
        //if(menu_wd) interior_helper_for_menu(msg, menu_wd);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      //wd_manager().call_safe_place(tid);
      //wd_manager().remove_trash_handle(tid);
    }//end while
    //Empty these rest messages, there is not a window to process these messages.
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE));
  }
  return 0;
}
native_window_t* create_window(native_window_type owner, bool nested, IRECT r, const int app)
{
  int inited = window_init();
  native_window_t* nw = opt_native_window(1);
  DWORD style = WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE;
  DWORD style_ex = WS_EX_NOPARENTNOTIFY;
  RECT client, wd_area;
  int delta_w, delta_h;
  POINT pt = {r.l, r.t};
  if (app & appearance_minimize) {
    style |= WS_MINIMIZEBOX;
  }
  if (app & appearance_maximize) {
    style |= WS_MAXIMIZEBOX;
  }
  if (app & appearance_sizable) {
    style |= WS_THICKFRAME;
  }
  if (app & appearance_decoration) {
    style |= WS_OVERLAPPED | WS_CAPTION;
  }
  style |= (nested ? WS_CHILD : WS_POPUP);
  style_ex |= (app & appearance_taskbar ? WS_EX_APPWINDOW : WS_EX_TOOLWINDOW);
  if (app & appearance_floating) {
    style_ex |= WS_EX_TOPMOST;
  }
  if (owner && (nested == false)) {
    ClientToScreen(owner, &pt);
  }
  if (NULL == nw->hwnd) {
    nw->hwnd = CreateWindowExA(style_ex, "NanaWindowInternal", "Nana Window", style, pt.x, pt.y, 100, 100, (owner), 0, GetModuleHandle(0), 0);
    opt_native_window_map(1, nw->hwnd, nw);
  }
  //A window may have a border, this should be adjusted the client area fit for the specified size.
  GetClientRect(nw->hwnd, &client); //The right and bottom of client by GetClientRect indicate the w and h of the area
  GetWindowRect(nw->hwnd, &wd_area);
  //a dimension with borders and caption title
  wd_area.right -= wd_area.left; //wd_area.right = w
  wd_area.bottom -= wd_area.top; //wd_area.bottom = h
  if (nested) {
    wd_area.left = pt.x;
    wd_area.top = pt.y;
  }
  delta_w = (int)RCW(&r) - client.right;
  delta_h = (int)RCH(&r) - client.bottom;
  MoveWindow(nw->hwnd, wd_area.left, wd_area.top, wd_area.right + delta_w, wd_area.bottom + delta_h, true);
  GetClientRect(nw->hwnd, &client);
  GetWindowRect(nw->hwnd, &wd_area);
  wd_area.right -= wd_area.left;
  wd_area.bottom -= wd_area.top;
  nw->rect = iRECT(client.right, client.bottom, wd_area.right, wd_area.bottom);
  return nw;
}
int show_window(native_window_type hwnd, bool show, bool active)
{
  int cmd = (show ? (active ? SW_SHOW : SW_SHOWNA) : SW_HIDE);
  return msw_show_window((hwnd), cmd);
}
#define WidgetGetFocus() WidgetSetFocus(NULL)
HWID WidgetSetFocus(HWID hWid)
{
  static HWID pre = NULL;
  HWID ret = pre;
  pre = hWid;
  if (hWid) {
    if (hWid->handle) {
      SetFocus(hWid->handle->hwnd);
      return 0;
    }
  }
  return pre;
}
#define WidgetGetCapture() WidgetSetCapture(NULL)
HWID WidgetSetCapture(HWID hWid)
{
  static HWID pre = NULL;
  HWID ret = pre;
  pre = hWid;
  if (hWid) {
    if (hWid->handle) {
      //SetCapture(hWid->handle->hwnd);
    }
  }
  return pre;
}
LRESULT call_widget_proc(widget_t* wid, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = NULL;
  if (wid) {
    (ret = call_widget_proc(wid->child_first, uMsg, wParam, lParam)) ||
    (ret = call_widget_proc(wid->next, uMsg, wParam, lParam)) ||
    (ret = wid->wc->lpfnWndProc(wid, uMsg, wParam, lParam)) ||
    1;
  }
  return ret;
}
LRESULT call_widget_proc_mouse(widget_t* wid, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = NULL;
  POINT pt;
  pt.x = (short)LOWORD(lParam);
  pt.y = (short)HIWORD(lParam);
  if (wid) {
    (ret = call_widget_proc_mouse(wid->child_first, uMsg, wParam, lParam)) ||
    (ret = call_widget_proc_mouse(wid->next, uMsg, wParam, lParam)) ||
    (ret = ((WidgetGetFocus() == wid || PtInRect(&wid->rect, pt))) && wid->wc->lpfnWndProc(wid, uMsg, wParam, lParam)) ||
    1;
  }
  return ret;
}
LRESULT DefWidgetProc(HWID hWid, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Bedrock_WIN32_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = NULL;
  native_window_t* win = opt_native_window_map(0, hwnd, NULL);
  if (win && win->form) {
    switch (uMsg) {
    case WM_SIZE:
      break;
    case WM_PAINT:
      if (0) {
        PAINTSTRUCT ps = {0};
        HDC hDC = BeginPaint(hwnd, &ps);
        HDC hMemDC = CreateCompatibleDC(hDC);
        int w, h;
        HBITMAP hBmp = CreateCompatibleBitmap(hDC, w = ps.rcPaint.right - ps.rcPaint.left, h = ps.rcPaint.bottom - ps.rcPaint.top);
        SelectObject(hMemDC, hBmp);
        win->hdc = hMemDC;
        wParam = (WPARAM)hMemDC;
        BitBlt(hMemDC, 0, 0, w, h, hDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
        //SetBkColor(hMemDC, GetBackgroundColor());
        //ExtTextOutA(m_hDC, 0, 0, ETO_OPAQUE, (RECT*)lpRect, NULL, 0, NULL);
        //这一句只是为了填充一个背景，
        //FillSolidRect(hMemDC, rectCanvas.left,rectCanvas.top,rectCanvas.Width(),rectCanvas.Height(),GetBackgroundColor());
        DefWidgetProc(win->form, uMsg, wParam, lParam);
        call_widget_proc(win->form, uMsg, wParam, lParam);
        BitBlt(hDC, ps.rcPaint.left, ps.rcPaint.top, w, h, hMemDC, 0, 0, SRCCOPY);
        DeleteObject(hBmp);
        DeleteDC(hMemDC);
        EndPaint(hwnd, &ps);
      }
      else {
        PAINTSTRUCT ps = {0};
        HDC hDC = BeginPaint(hwnd, &ps);
        win->hdc = hDC;
        wParam = (WPARAM)hDC;
        DefWidgetProc(win->form, uMsg, wParam, lParam);
        call_widget_proc(win->form, uMsg, wParam, lParam);
        EndPaint(hwnd, &ps);
      }
      return 1;
      break;
    case WM_NCLBUTTONDOWN : {
      int asdf = 0;
      printf("WM_NCLBUTTONDOWN %d %d\n", (short)LOWORD(lParam), (short)HIWORD(lParam));
    }
    break;
    case WM_MOUSEMOVE :
    case WM_LBUTTONDOWN :
    case WM_LBUTTONUP :
    case WM_LBUTTONDBLCLK :
    case WM_RBUTTONDOWN :
    case WM_RBUTTONUP :
    case WM_RBUTTONDBLCLK :
    case WM_MBUTTONDOWN :
    case WM_MBUTTONUP :
    case WM_MBUTTONDBLCLK :
      if (WM_LBUTTONDOWN == uMsg) {
        TRACKMOUSEEVENT csTME = {sizeof(TRACKMOUSEEVENT)};
        csTME.dwFlags = TME_LEAVE | TME_HOVER | TME_NONCLIENT;
        csTME.hwndTrack = hwnd; // 指定要 追踪 的窗口
        csTME.dwHoverTime = 0; // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
        //_TrackMouseEvent(&csTME); // 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持
        SetCapture(hwnd);
      }
      else if (WM_LBUTTONUP == uMsg) {
        int asdf = 0;
        ReleaseCapture();
      }
      //printf("WM_MOUSEMOVE %d %d\n", (short)LOWORD(lParam), (short)HIWORD(lParam));
      if (ret = call_widget_proc_mouse(win->form, uMsg, wParam, lParam)) {
        return ret;
      }
      break;
    case WM_NCHITTEST:
      break;
    default:
      if (ret = call_widget_proc(win->form, uMsg, wParam, lParam)) {
        return ret;
      }
      break;
    }
    if (ret = DefWidgetProc(win->form, uMsg, wParam, lParam)) {
      return ret;
    }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
#if 0
  POINT pos;
  event_arg arg = {0};
  GetCursorPos(&pos);
  arg.pos.x = pos.x;
  arg.pos.y = pos.y;
  switch (uMsg) {
  case WM_KEYDOWN:
  case WM_LBUTTONDBLCLK:
  case WM_MBUTTONDBLCLK:
  case WM_RBUTTONDBLCLK:
    arg.evt_code = event_dbl_click;
    arg.left_button = (uMsg == WM_LBUTTONDBLCLK);
    arg.mid_button = (uMsg == WM_MBUTTONDBLCLK);
    arg.right_button = (uMsg == WM_RBUTTONDBLCLK);
    break;
  case WM_LBUTTONDOWN:
  case WM_MBUTTONDOWN:
  case WM_RBUTTONDOWN:
    arg.evt_code = event_mouse_down;
    arg.left_button = (uMsg == WM_LBUTTONDOWN);
    arg.mid_button = (uMsg == WM_MBUTTONDOWN);
    arg.right_button = (uMsg == WM_RBUTTONDOWN);
    break;
  case WM_LBUTTONUP:
  case WM_MBUTTONUP:
  case WM_RBUTTONUP:
    arg.evt_code = event_mouse_up;
    arg.left_button = (uMsg == WM_LBUTTONUP);
    arg.mid_button = (uMsg == WM_MBUTTONUP);
    arg.right_button = (uMsg == WM_RBUTTONUP);
    break;
  case WM_MOUSEMOVE:
  case WM_MOUSELEAVE:
    arg.evt_code = (WM_MOUSEMOVE == uMsg ? event_mouse_move : event_mouse_leave);
    arg.left_button = false;
    arg.mid_button = false;
    arg.right_button = false;
    break;
  case msg_async_activate:
    EnableWindow(hwnd, true);
    SetActiveWindow(hwnd);
    return true;
  case msg_async_set_focus:
    SetFocus(hwnd);
    return true;
  case msg_operate_caret:
    //Refer to basis.hpp for this specification.
    switch (wParam) {
    case 1: //Delete
      DestroyCaret();
      break;
    case 2: //SetPos
      SetCaretPos(((IPOINT*)(lParam))->x, ((IPOINT*)(lParam))->y);
      free((IPOINT*)(lParam));
      break;
    }
    return true;
  case msg_remote_flush_surface: {
    //auto stru = (msg_map_thread*)(lParam);
    //bedrock.wd_manager().map((bedrock core_window_t*)(wParam), stru->forced, (stru->ignore_update_area ? nullptr : &stru->update_area));
    //UpdateWindow(hwnd);
    //HeapFree( GetProcessHeap(), 0, stru);
  }
  return true;
  case msg_remote_thread_move_window: {
    move_window_t* mw = (move_window_t*)(wParam);
    RECT r;
    GetWindowRect(hwnd, &r);
    if (mw->ignore & move_window_Pos) {
      mw->x = r.left;
      mw->y = r.top;
    }
    else {
      HWND owner = GetWindow(hwnd, GW_OWNER);
      if (owner) {
        RECT owr;
        GetWindowRect(owner, &owr);
        {
          POINT pos = {owr.left, owr.top};
          ScreenToClient(owner, &pos);
          mw->x += (owr.left - pos.x);
          mw->y += (owr.top - pos.y);
        }
      }
    }
    if (mw->ignore & move_window_Size) {
      mw->w = r.right - r.left;
      mw->h = r.bottom - r.top;
    }
    MoveWindow(hwnd, mw->x, mw->y, mw->w, mw->h, true);
    free(mw);
  }
  return true;
  case msg_remote_thread_set_window_pos:
    SetWindowPos(hwnd, (HWND)(wParam), 0, 0, 0, 0, (UINT)(lParam));
    return true;
  case msg_remote_thread_set_window_text:
    SetWindowTextA(hwnd, (char*)(wParam));
    free((char*)(wParam));
    return true;
  case msg_remote_thread_destroy_window:
    close_window(hwnd); //The owner would be actived before the message has posted in current thread.
    if (NULL == opt_native_window(0)) {
      PostQuitMessage(0);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  case msg_tray:
    //notifications_window_proc(hwnd, wParam, lParam);
    return true;
  case msg_affinity_execute:
    if (wParam) {
      //auto arg = (msg_arg_affinity_execute*)(wParam);
      //if (arg->function_ptr) (*arg->function_ptr)();
    }
    break;
  default:
    break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
#endif
}

