#include <X11/Xlib.h>
#include <X11/Xlocale.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xos.h>
#define platform_scope_guard_psg()
//#define NANA_USE_XFT
#if defined(NANA_USE_XFT)
#include <X11/Xft/Xft.h>
#include <iconv.h>
#endif
typedef struct atombase_tag atombase_tag;
struct atombase_tag {
  Atom wm_protocols;
  //window manager support
  Atom wm_change_state;
  Atom wm_delete_window;
  //ext
  Atom net_wm_state;
  Atom net_wm_state_skip_taskbar;
  Atom net_wm_state_fullscreen;
  Atom net_wm_state_maximized_horz;
  Atom net_wm_state_maximized_vert;
  Atom net_wm_state_modal;
  Atom net_wm_name;
  Atom net_wm_window_type;
  Atom net_wm_window_type_normal;
  Atom net_wm_window_type_utility;
  Atom net_wm_window_type_dialog;
  Atom motif_wm_hints;
  Atom clipboard;
  Atom text;
  Atom text_uri_list;
  Atom utf8_string;
  Atom targets;
  Atom xdnd_aware;
  Atom xdnd_enter;
  Atom xdnd_position;
  Atom xdnd_status;
  Atom xdnd_action_copy;
  Atom xdnd_drop;
  Atom xdnd_selection;
  Atom xdnd_typelist;
  Atom xdnd_finished;
};
typedef struct platform_x11 platform_x11;
typedef struct window_context_t window_context_t;
struct window_context_t {
  native_window_type wd;
  native_window_type owner;
  //native_window_type> * owned;
  bool has_input_method_focus;
  bool visible;
  IPOINT pos;
  ISIZE size;
  IRECT rev;
  //graphics rev_graph;
  XIM input_method;
  XIC input_context;
  XFontSet input_font;
  XRectangle input_spot;
  XRectangle input_status_area;
  long input_context_event_mask;
};
struct platform_x11 {
  Display* display_;
  Colormap colormap_;
  atombase_tag atombase_;
  font_tag* def_font_ptr_;
  window_context_t* wincontext_;
  int wincontext_len;
  XKeyEvent key_state_;
  int (*def_X11_error_handler_)(Display*, XErrorEvent*);
  Window grab_;
  int error_code;
};
platform_x11* get_x11()
{
  static platform_x11 g_x11[1] = {0};
  if (NULL == g_x11->display_) {
    XInitThreads();
    const char* langstr = getenv("LC_CTYPE");
    if (0 == langstr) {
      langstr = getenv("LC_ALL");
    }
    char langstr_dup[256] = {0};
    if (langstr) {
      strcpy(langstr_dup, langstr);
      char* dotpos = strchr(langstr_dup, '.');
      if (dotpos) {
        char* beg = dotpos + 1;
        for (; *beg; ++beg) {
          *beg = toupper(*beg);
        }
      }
    }
    else {
      strcpy(langstr_dup, "en_US.UTF-8");
    }
    setlocale(LC_CTYPE, langstr_dup);
    if (XSupportsLocale()) {
      XSetLocaleModifiers(langstr_dup);
    }
    g_x11->display_ = XOpenDisplay(0);
    g_x11->colormap_ = DefaultColormap(g_x11->display_, XDefaultScreen(g_x11->display_));
    //Initialize the member data
    //selection_.content.utf8_string = 0;
    //g_x11->xdnd_.good_type = None;
    g_x11->atombase_.wm_protocols = XInternAtom(g_x11->display_, "WM_PROTOCOLS", False);
    g_x11->atombase_.wm_change_state = XInternAtom(g_x11->display_, "WM_CHANGE_STATE", False);
    g_x11->atombase_.wm_delete_window = XInternAtom(g_x11->display_, "WM_DELETE_WINDOW", False);
    g_x11->atombase_.net_wm_state = XInternAtom(g_x11->display_, "_NET_WM_STATE", False);
    g_x11->atombase_.net_wm_state_skip_taskbar = XInternAtom(g_x11->display_, "_NET_WM_STATE_SKIP_TASKBAR", False);
    g_x11->atombase_.net_wm_state_fullscreen = XInternAtom(g_x11->display_, "_NET_WM_STATE_FULLSCREEN", False);
    g_x11->atombase_.net_wm_state_maximized_horz = XInternAtom(g_x11->display_, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    g_x11->atombase_.net_wm_state_maximized_vert = XInternAtom(g_x11->display_, "_NET_WM_STATE_MAXIMIZED_VERT", False);
    g_x11->atombase_.net_wm_state_modal = XInternAtom(g_x11->display_, "_NET_WM_STATE_MODAL", False);
    g_x11->atombase_.net_wm_name = XInternAtom(g_x11->display_, "_NET_WM_NAME", False);
    g_x11->atombase_.net_wm_window_type = XInternAtom(g_x11->display_, "_NET_WM_WINDOW_TYPE", False);
    g_x11->atombase_.net_wm_window_type_normal = XInternAtom(g_x11->display_, "_NET_WM_WINDOW_TYPE_NORMAL", False);
    g_x11->atombase_.net_wm_window_type_utility = XInternAtom(g_x11->display_, "_NET_WM_WINDOW_TYPE_UTILITY", False);
    g_x11->atombase_.net_wm_window_type_dialog = XInternAtom(g_x11->display_, "_NET_WM_WINDOW_TYPE_DIALOG", False);
    g_x11->atombase_.motif_wm_hints = XInternAtom(g_x11->display_, "_MOTIF_WM_HINTS", False);
    g_x11->atombase_.clipboard = XInternAtom(g_x11->display_, "CLIPBOARD", True);
    g_x11->atombase_.text = XInternAtom(g_x11->display_, "TEXT", True);
    g_x11->atombase_.text_uri_list = XInternAtom(g_x11->display_, "text/uri-list", True);
    g_x11->atombase_.utf8_string = XInternAtom(g_x11->display_, "UTF8_STRING", True);
    g_x11->atombase_.targets = XInternAtom(g_x11->display_, "TARGETS", True);
    g_x11->atombase_.xdnd_aware = XInternAtom(g_x11->display_, "XdndAware", False);
    g_x11->atombase_.xdnd_enter = XInternAtom(g_x11->display_, "XdndEnter", False);
    g_x11->atombase_.xdnd_position = XInternAtom(g_x11->display_, "XdndPosition", False);
    g_x11->atombase_.xdnd_status = XInternAtom(g_x11->display_, "XdndStatus", False);
    g_x11->atombase_.xdnd_action_copy = XInternAtom(g_x11->display_, "XdndActionCopy", False);
    g_x11->atombase_.xdnd_drop = XInternAtom(g_x11->display_, "XdndDrop", False);
    g_x11->atombase_.xdnd_selection = XInternAtom(g_x11->display_, "XdndSelection", False);
    g_x11->atombase_.xdnd_typelist = XInternAtom(g_x11->display_, "XdndTypeList", False);
    g_x11->atombase_.xdnd_finished = XInternAtom(g_x11->display_, "XdndFinished", False);
    //Create default font object.
    //g_x11->def_font_ptr_ = make_native_font(nullptr, font_size_to_height(10), 400, false, false, false);
    //g_x11->msg_dispatcher_ = new msg_dispatcher(g_x11->display_);
  }
  return g_x11;
}
Window grab(Window wd)
{
  Window r = get_x11()->grab_;
  get_x11()->grab_ = wd;
  return r;
}
Display* open_display()
{
  return get_x11()->display_;
}
Colormap colormap()
{
  return get_x11()->colormap_;
}
atombase_tag* atombase()
{
  return &get_x11()->atombase_;
}
Window root_window()
{
  return ::XDefaultRootWindow(get_x11()->display_);
}
int screen_depth()
{
  return ::XDefaultDepth(get_x11()->display_, ::XDefaultScreen(get_x11()->display_));
}
void close_display()
{
  if (get_x11()->display_) {
    ::XSync(get_x11()->display_, true);
    ::XCloseDisplay(get_x11()->display_);
    get_x11()->display_ = 0;
  }
}
int rev_error_handler()
{
  platform_x11* x11 = get_x11();
  if (x11->def_X11_error_handler_) {
    ::XSync(x11->display_, False);
    ::XSetErrorHandler(x11->def_X11_error_handler_);
  }
  return x11->error_code;
}
Visual* screen_visual()
{
  return ::XDefaultVisual(get_x11()->display_, ::XDefaultScreen(get_x11()->display_));
}
window_context_t* find_window_context(native_window_type wd)
{
  platform_x11* x11 = get_x11();
  int i;
  for (i = 0; i < x11->wincontext_len; ++i) {
    if (wd == x11->wincontext_[i].wd) {
      return x11->wincontext_ + i;
    }
  }
  return NULL;
}
int X11_error_handler(Display*, XErrorEvent* err)
{
  get_x11()->error_code = err->error_code;
  return 0;
}
void set_error_handler()
{
  get_x11()->error_code = 0;
  get_x11()->def_X11_error_handler_ = ::XSetErrorHandler(X11_error_handler);
}
native_window_type get_owner(native_window_type wd)
{
  window_context_t* ctx = find_window_context(wd);
  return ctx ? ctx->owner : 0;
}
void make_owner(native_window_type owner, native_window_type wd)
{
  window_context_t* ctx = find_window_context(wd);
  if (ctx) {
    ctx->owner = owner;
  }
  else {
    platform_x11* x11 = get_x11();
    x11->wincontext_len++;
    MYREALLOC(x11->wincontext_, x11->wincontext_len);
    x11->wincontext_[x11->wincontext_len - 1].owner = owner;
    x11->wincontext_[x11->wincontext_len - 1].wd = wd;
  }
}
font_tag* make_native_font(const char* name, unsigned height, unsigned weight, bool italic, bool underline, bool strike_out)
{
  platform_x11* x11 = get_x11();
#if defined(NANA_USE_XFT) //Xft
  if (0 == name || *name == 0) {
    name = "*";
  }
  XftFont* handle = 0;
  std::stringstream ss;
  ss << name << "-" << (height ? height : 10);
  XftPattern* pat = ::XftNameParse(ss.str().c_str());
  XftResult res;
  XftPattern* match_pat = ::XftFontMatch(display_, ::XDefaultScreen(display_), pat, &res);
  if (match_pat) {
    handle = ::XftFontOpenPattern(display_, match_pat);
  }
#else
  char basestr[256];
  if (0 == name || *name == 0) {
    strcpy(basestr, "-misc-fixed-*");
  }
  else {
    strcpy(basestr, "-misc-fixed-*");
  }
  char** missing_list;
  int missing_count;
  char* defstr;
  XFontSet handle = ::XCreateFontSet(x11->display_, basestr, &missing_list, &missing_count, &defstr);
#endif
  if (handle) {
    font_tag* impl = NULL;
    MYREALLOC(impl, 1);
    strncpy(impl->name, name, 32);
    impl->height = height;
    impl->weight = weight;
    impl->italic = italic;
    impl->underline = underline;
    impl->strikeout = strike_out;
    impl->handle = handle;
    return impl;
  }
  return NULL;
}
void write_keystate(const XKeyEvent* xkey)
{
  platform_x11* x11 = get_x11();
  x11->key_state_ = *xkey;
}
void read_keystate(XKeyEvent* xkey)
{
  platform_x11* x11 = get_x11();
  *xkey = x11->key_state_;
}
XIC caret_input_context(native_window_type wd)
{
  platform_scope_guard_psg();
  window_context_t* ctx = find_window_context(wd);
  if (ctx) {
    return ctx->input_context;
  }
  return 0;
}
#if 0
void caret_open(native_window_type wd, ISIZE caret_sz)
{
  bool is_start_routine = false;
  platform_scope_guard_psg();
  window_context_t* ctx = find_window_context(wd);
  if (nullptr == ctx) {
    ::XSetLocaleModifiers("");
    addr = new caret_rep(wd);
    is_start_routine = (caret_holder_.carets.size() == 1);
    addr->input_method = ::XOpenIM(display_, 0, 0, 0);
    if (addr->input_method) {
      XIMStyles* imstyle;
      ::XGetIMValues(addr->input_method, XNQueryInputStyle, &imstyle, nullptr, nullptr);
      if (imstyle) {
        if (imstyle->count_styles) {
          addr->input_font = 0;
          XVaNestedList preedit_attr = ::XVaCreateNestedList(0, XNSpotLocation, &(addr->input_spot), nullptr);
          XVaNestedList status_attr = ::XVaCreateNestedList(0, XNAreaNeeded, &(addr->input_status_area), nullptr);
          XIMStyle* style_end = imstyle->supported_styles + imstyle->count_styles;
          bool has_status = false;
          bool has_preedit = false;
          for (XIMStyle* i = imstyle->supported_styles; i != style_end; ++i) {
            if (*i == (XIMPreeditPosition | XIMStatusArea)) {
              has_status = has_preedit = true;
              break;
            }
            else if (*i == (XIMPreeditPosition | XIMStatusNothing)) {
              has_preedit = true;
            }
          }
          if (has_status) {
            addr->input_context = ::XCreateIC(addr->input_method, XNInputStyle, (XIMPreeditPosition | XIMStatusArea),
                XNPreeditAttributes, preedit_attr, XNStatusAttributes, status_attr,
                XNClientWindow, reinterpret_cast<Window>(wd), nullptr);
          }
          else {
            addr->input_context = 0;
          }
          if ((addr->input_context == 0) && has_preedit) {
            addr->input_context = ::XCreateIC(addr->input_method, XNInputStyle, (XIMPreeditPosition | XIMStatusNothing),
                XNPreeditAttributes, preedit_attr, XNClientWindow, reinterpret_cast<Window>(wd), nullptr);
          }
          if (addr->input_context) {
            XVaNestedList attr = ::XVaCreateNestedList(0, XNAreaNeeded, &(addr->input_status_area), XNClientWindow, reinterpret_cast<Window>(wd), nullptr);
            ::XGetICValues(addr->input_context, XNStatusAttributes, attr, nullptr);
            ::XFree(attr);
          }
          else
            addr->input_context = ::XCreateIC(addr->input_method,
                XNInputStyle, (XIMPreeditNothing | XIMStatusNothing),
                XNClientWindow, reinterpret_cast<Window>(wd),
                XNFocusWindow, reinterpret_cast<Window>(wd), nullptr);
          if (addr->input_context) {
            //Make the IM event filter.
            ::XGetICValues(addr->input_context, XNFilterEvents, &(addr->input_context_event_mask), nullptr);
            XWindowAttributes attr;
            ::XGetWindowAttributes(display_, reinterpret_cast<Window>(wd), &attr);
            XSetWindowAttributes new_attr;
            new_attr.event_mask = (attr.your_event_mask | addr->input_context_event_mask);
            ::XChangeWindowAttributes(display_, reinterpret_cast<Window>(wd), CWEventMask, &new_attr);
            ::XSetICValues(addr->input_context, XNResetState, XIMPreserveState, nullptr);
          }
          ::XFree(preedit_attr);
          ::XFree(status_attr);
        }
        ::XFree(imstyle);
      }
    }
  }
  addr->visible = false;
  addr->rev_graph.make(caret_sz);
  addr->size = caret_sz;
  if (addr->input_context && (false == addr->has_input_method_focus)) {
    ::XSetICFocus(addr->input_context);
    addr->has_input_method_focus = true;
  }
  if (is_start_routine) {
    caret_holder_.exit_thread = false;
    auto fn = [x11]() {
      x11->_m_caret_routine();
    };
    caret_holder_.thr.reset(new std::thread(fn));
  }
}
void caret_close(native_window_type wd)
{
  bool is_end_routine = false;
  {
    platform_scope_guard_psg();
    auto i = caret_holder_.carets.find(wd);
    if (i != caret_holder_.carets.end()) {
      auto addr = i->second;
      if (addr->input_context) {
        if (addr->has_input_method_focus) {
          ::XUnsetICFocus(addr->input_context);
          addr->has_input_method_focus = false;
        }
        //Remove the IM event filter.
        set_error_handler();
        XWindowAttributes attr;
        if (BadWindow != ::XGetWindowAttributes(display_, reinterpret_cast<Window>(wd), &attr)) {
          if ((attr.your_event_mask & addr->input_context_event_mask) == addr->input_context_event_mask) {
            XSetWindowAttributes new_attr;
            //Don't remove the KeyPress and KeyRelease mask(0x3), otherwise the window will not receive
            //Keyboard events after destroying caret
            new_attr.event_mask = (attr.your_event_mask & ~(addr->input_context_event_mask & (~0x3)));
            ::XChangeWindowAttributes(display_, reinterpret_cast<Window>(wd), CWEventMask, &new_attr);
          }
        }
        rev_error_handler();
        ::XDestroyIC(addr->input_context);
      }
      if (addr->input_font) {
        ::XFreeFontSet(display_, addr->input_font);
      }
      if (addr->input_method) {
        ::XCloseIM(addr->input_method);
      }
      delete i->second;
      caret_holder_.carets.erase(i);
    }
    is_end_routine = (caret_holder_.carets.size() == 0);
  }
  if (is_end_routine && (caret_holder_.thr != nullptr) && (caret_holder_.thr->joinable())) {
    caret_holder_.exit_thread = true;
    caret_holder_.thr->join();
    caret_holder_.thr.reset();
  }
}
void caret_pos(native_window_type wd, const point& pos)
{
  platform_scope_guard_psg();
  auto i = caret_holder_.carets.find(wd);
  if (i != caret_holder_.carets.end()) {
    i->second->reinstate();
    i->second->pos = pos;
  }
}
void caret_visible(native_window_type wd, bool vis)
{
  platform_scope_guard_psg();
  auto i = caret_holder_.carets.find(wd);
  if (i != caret_holder_.carets.end()) {
    auto& crt = *i->second;
    if (crt.visible != vis) {
      if (vis == false) {
        crt.reinstate();
        if (crt.input_context && crt.has_input_method_focus) {
          ::XUnsetICFocus(crt.input_context);
          crt.has_input_method_focus = false;
        }
      }
      else {
        if (crt.input_context && (false == crt.has_input_method_focus)) {
          ::XSetICFocus(crt.input_context);
          crt.has_input_method_focus = true;
        }
      }
      crt.visible = vis;
    }
  }
}
bool caret_update(native_window_type wd, paint::graphics& /*root_graph*/, bool after_mapping)
{
  platform_scope_guard_psg();
  auto i = caret_holder_.carets.find(wd);
  if (i != caret_holder_.carets.end()) {
    auto& crt = *i->second;
    if (!after_mapping) {
      return crt.reinstate();
    }
    else {
      crt.twinkle();
    }
  }
  return false;
}
void _m_caret_routine()
{
  while (false == caret_holder_.exit_thread) {
    if (xlib_locker_.try_lock()) {
for (auto i : caret_holder_.carets) {
        i.second->twinkle();
      }
      xlib_locker_.unlock();
    }
    for (int i = 0; i < 5 && (false == caret_holder_.exit_thread); ++i) {
      system::sleep(100);
    }
  }
}
void set_timer(std::size_t id, std::size_t interval, void (*timer_proc)(std::size_t))
{
  std::lock_guard<decltype(timer_.mutex)> lock(timer_.mutex);
  if (0 == timer_.runner) {
    timer_.runner = new timer_runner;
  }
  timer_.runner->set(id, interval, timer_proc);
  timer_.delete_declared = false;
}
void kill_timer(std::size_t id)
{
  if (timer_.runner == 0) {
    return;
  }
  std::lock_guard<decltype(timer_.mutex)> lock(timer_.mutex);
  timer_.runner->kill(id);
  if (timer_.runner->empty()) {
    if (timer_.runner->is_proc_handling() == false) {
      delete timer_.runner;
      timer_.runner = 0;
    }
    else {
      timer_.delete_declared = true;
    }
  }
}
void timer_proc(unsigned tid)
{
  std::lock_guard<decltype(timer_.mutex)> lock(timer_.mutex);
  if (timer_.runner) {
    timer_.runner->timer_proc(tid);
    if (timer_.delete_declared) {
      delete timer_.runner;
      timer_.runner = 0;
      timer_.delete_declared = false;
    }
  }
}
void msg_insert(native_window_type wd)
{
  msg_dispatcher_->insert(reinterpret_cast<Window>(wd));
}
void msg_set(timer_proc_type tp, event_proc_type ep)
{
  msg_dispatcher_->set(tp, ep, &_m_msg_filter);
}
void msg_dispatch(native_window_type modal)
{
  msg_dispatcher_->dispatch(reinterpret_cast<Window>(modal));
}
void* request_selection(native_window_type requestor, Atom type, size_t& size)
{
  if (requestor) {
    Atom clipboard = atombase_.clipboard;
    xlib_locker_.lock();
    Window owner = ::XGetSelectionOwner(display_, clipboard);
    if (owner) {
      selection_tag::item_t* selim = new selection_tag::item_t;
      selim->type = type;
      selim->requestor = reinterpret_cast<Window>(requestor);
      selim->buffer = nullptr;
      selim->bufsize = 0;
      x11->selection_.items.push_back(selim);
      ::XConvertSelection(display_, clipboard, type, clipboard,
          reinterpret_cast<Window>(requestor), CurrentTime);
      ::XFlush(display_);
      xlib_locker_.unlock();
      std::unique_lock < decltype(selim->cond_mutex) > lock(selim->cond_mutex);
      selim->cond.wait(lock);
      size = selim->bufsize;
      void* retbuf = selim->buffer;
      delete selim;
      return retbuf;
    }
    else {
      xlib_locker_.unlock();
    }
  }
  return nullptr;
}
void write_selection(native_window_type owner, Atom type, const void* buf, size_t bufsize)
{
  platform_scope_guard_psg();
  ::XSetSelectionOwner(display_, atombase_.clipboard, reinterpret_cast<Window>(owner), CurrentTime);
  ::XFlush(display_);
  if (XA_STRING == type || atombase_.utf8_string == type) {
    std::string* utf8str = selection_.content.utf8_string;
    if (utf8str) {
      utf8str->clear();
    }
    else {
      utf8str = new std::string;
    }
    utf8str->append(reinterpret_cast<const char*>(buf), reinterpret_cast<const char*>(buf) + bufsize);
    selection_.content.utf8_string = utf8str;
  }
}
//Icon Storage
const paint::graphics& keep_window_icon(native_window_type wd, const paint::image& img)
{
  paint::graphics& graph = iconbase_[wd];
  graph.make(img.size());
  img.paste(graph, {});
  return graph;
}
#endif
//platform-dependent
native_window_t* create_window(native_window_type owner, bool nested, IRECT r, const int app)
{
  native_window_t* nw = opt_native_window(1);
  XSetWindowAttributes win_attr;
  unsigned long attr_mask = CWBackPixmap | CWBackPixel | CWBorderPixel |
      CWWinGravity | CWBitGravity | CWColormap | CWEventMask;
  Display* disp = open_display();
  win_attr.colormap = get_x11()->colormap_;
  win_attr.background_pixmap = None;
  win_attr.background_pixel = 0xFFFFFF;
  win_attr.border_pixmap = None;
  win_attr.border_pixel = 0x0;
  win_attr.bit_gravity = 0;
  win_attr.win_gravity = NorthWestGravity;
  win_attr.backing_store = 0;
  win_attr.backing_planes = 0;
  win_attr.backing_pixel = 0;
  if (!(app & appearance_decoration)) {
    win_attr.override_redirect = True;
    attr_mask |= CWOverrideRedirect;
  }
  Window parent = (owner ? (Window)(owner) : root_window());
  IPOINT pos = {r.l, r.t};
  if ((false == nested) && owner) {
    win_attr.save_under = True;
    attr_mask |= CWSaveUnder;
    parent = root_window();
    calc_screen_point(owner, &pos);
  }
  win_attr.event_mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask;
  Window handle = XCreateWindow(disp, parent,
      pos.x, pos.y, (RCW(&r) ? RCW(&r) : 1), (RCH(&r) ? RCH(&r) : 1), 0,
      screen_depth(), InputOutput, screen_visual(),
      attr_mask, &win_attr);
  if (handle) {
    //make owner if it is a popup window
    if ((!nested) && owner) {
      make_owner(owner, (handle));
    }
    XTextProperty name;
    char text[] = "Nana Window";
    char* str = text;
    XStringListToTextProperty(&str, 1, &name);
    XSetWMName(disp, handle, &name);
    const atombase_tag* ab = atombase();
    XSetWMProtocols(disp, handle, (Atom*)(&ab->wm_delete_window), 1);
    struct {
      long flags;
      long functions;
      long decorations;
      long input;
      long status;
    } motif;
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
    if (app & appearance_sizable) {
      motif.decorations |= 4; //MWM_DECOR_RESIZEH;
      motif.functions |= 2; //MWM_FUNC_RESIZE;
    }
    else {
      hints.min_width = hints.max_width = RCW(&r);
      hints.min_height = hints.max_height = RCH(&r);
      hints.flags |= (PMinSize | PMaxSize);
    }
    XSetWMNormalHints(disp, handle, &hints);
    if (app & appearance_decoration) {
      if (app & appearance_minimize) {
        motif.decorations |= (1 << 5); //MWM_DECOR_MINIMIZE;
        motif.functions |= (1 << 3); //MWM_FUNC_MINIMIZE;
      }
      if (app & appearance_maximize) {
        motif.decorations |= (1 << 6);//MWM_DECOR_MAXIMIZE;
        motif.functions |= (1 << 4); //MWM_FUNC_MAXIMIZE;
      }
      motif.functions |= (1 << 5); //MWM_FUNC_CLOSE
      motif.decorations |= (2) | 8; //MWM_DECOR_BORDER | MWM_DECOR_TITLE
    }
    if ((false == nested) && owner) {
      XChangeProperty(disp, handle, ab->net_wm_window_type, XA_ATOM, 32, PropModeReplace,
          (unsigned char*)((Atom*)(&ab->net_wm_window_type_dialog)), 1);
      XSetTransientForHint(disp, handle, (Window)(owner));
    }
    XChangeProperty(disp, handle, ab->motif_wm_hints, ab->motif_wm_hints, 32, PropModeReplace,
        (unsigned char*)(&motif), sizeof(motif) / sizeof(long));
    if (app & appearance_floating) {
      XChangeProperty(disp, handle, ab->net_wm_window_type, XA_ATOM, 32, PropModeReplace,
          (unsigned char*)((Atom*)(&ab->net_wm_window_type_normal)), 1);
      XSetTransientForHint(disp, handle, root_window());
    }
    if (false == app & appearance_taskbar) {
      XChangeProperty(disp, handle, ab->net_wm_state, XA_ATOM, 32, PropModeAppend,
          (unsigned char*)((Atom*)(&ab->net_wm_state_skip_taskbar)), 1);
    }
  }
  nw->hwnd = (handle);
  nw->rect = r;
  return nw;
}
int show_window(native_window_type wd, bool show, bool active)
{
  if (wd) {
    Display* disp = open_display();
    if (show) {
      XMapWindow(disp, (Window)(wd));
      Window grab1 = grab(0);
      if (grab1 == (Window)(wd)) {
        capture_window(wd, true);
      }
    }
    else {
      XUnmapWindow(disp, (Window)(wd));
    }
    XFlush(disp);
  }
  (void)(active); //eliminate unused parameter compiler warning.
}
typedef struct msg_packet_tag msg_packet_tag;
enum {kind_xevent, kind_mouse_drop, kind_cleanup};
struct msg_packet_tag {
  int kind;
  union {
    XEvent xevent;
    Window packet_window; //Avaiable if the packet is not kind_xevent
    struct mouse_drop_tag {
      Window window;
      int x;
      int y;
      char** files;
    } mouse_drop;
  } u;
};
#if 0
//_m_msg_filter
//@return: _m_msg_filter returns three states
// 0 = msg_dispatcher dispatches the XEvent
// 1 = msg_dispatcher dispatches the msg_packet_tag that modified by _m_msg_filter
// 2 = msg_dispatcher should ignore the msg, because the XEvent is processed by _m_msg_filter
int _m_msg_filter(XEvent& evt, msg_packet_tag& msg)
{
  platform_spec& self = instance();
  if (SelectionNotify == evt.type) {
    if (evt.xselection.property) {
      Atom type;
      int format;
      unsigned long len, bytes_left = 0;
      unsigned char* data;
      ::XGetWindowProperty(self.display_, evt.xselection.requestor, evt.xselection.property, 0, 0, 0,
          AnyPropertyType, &type, &format, &len, &bytes_left, &data);
      if (evt.xselection.property == self.atombase_.clipboard) {
        platform_scope_guard_psg();
        if (self.selection_.items.size()) {
          selection_tag::item_t* im = self.selection_.items.front();
          if (bytes_left > 0 && (type == im->type)) {
            unsigned long dummy_bytes_left;
            if (Success == ::XGetWindowProperty(self.display_, evt.xselection.requestor,
                evt.xselection.property, 0, bytes_left,
                0, AnyPropertyType, &type, &format, &len,
                &dummy_bytes_left, &data)) {
              im->buffer = data;
              im->bufsize = len;
            }
          }
          self.selection_.items.erase(self.selection_.items.begin());
          std::lock_guard < decltype(im->cond_mutex) > lock(im->cond_mutex);
          im->cond.notify_one();
        }
      }
      else if (evt.xselection.property == self.atombase_.xdnd_selection) {
        bool accepted = false;
        msg.kind = msg.kind_mouse_drop;
        msg.u.mouse_drop.window = 0;
        if (bytes_left > 0 && type == self.xdnd_.good_type) {
          unsigned long dummy_bytes_left;
          if (Success == ::XGetWindowProperty(self.display_, evt.xselection.requestor,
              evt.xselection.property, 0, bytes_left,
              0, AnyPropertyType, &type, &format, &len,
              &dummy_bytes_left, &data)) {
            auto files = new std::vector<std::string>;
            std::stringstream ss(reinterpret_cast<char*>(data));
            while (true) {
              std::string file;
              std::getline(ss, file);
              if (false == ss.good()) {
                break;
              }
              if (0 == file.find("file://")) {
                file = file.substr(7);
              }
              while (file.size()) {
                auto ch = file.back();
                if ('\r' == ch || '\n' == ch) {
                  file.pop_back();
                }
                else {
                  break;
                }
              }
              files->push_back(file);
            }
            if (files->size()) {
              msg.u.mouse_drop.window = evt.xselection.requestor;
              msg.u.mouse_drop.x = self.xdnd_.pos.x;
              msg.u.mouse_drop.y = self.xdnd_.pos.y;
              msg.u.mouse_drop.files = files;
            }
            else {
              delete files;
            }
            accepted = true;
            ::XFree(data);
          }
        }
        XEvent respond;
        ::memset(respond.xclient.data.l, 0, sizeof(respond.xclient.data.l));
        respond.xclient.display = self.display_;
        respond.xclient.window = self.xdnd_.wd_src;
        respond.xclient.message_type = self.atombase_.xdnd_finished;
        respond.xclient.format = 32;
        respond.xclient.data.l[0] = evt.xselection.requestor;
        if (accepted) {
          respond.xclient.data.l[1] = 1;
          respond.xclient.data.l[2] = self.atombase_.xdnd_action_copy;
        }
        ::XSendEvent(self.display_, self.xdnd_.wd_src, False, NoEventMask, &respond);
        ::XFlush(self.display_);
        if (msg.u.mouse_drop.window) {
          return 1; //Use the packet directly.
        }
      }
    }
    ::XFlush(self.display_);
    return 2;
  }
  else if (SelectionRequest == evt.type) {
    auto disp = evt.xselectionrequest.display;
    XEvent respond;
    respond.xselection.property = evt.xselectionrequest.property;
    if (self.atombase_.targets == evt.xselectionrequest.target) {
      std::vector<Atom> atoms;
      if (self.selection_.content.utf8_string) {
        atoms.push_back(self.atombase_.utf8_string);
        atoms.push_back(XA_STRING);
      }
      ::XChangeProperty(self.display_, evt.xselectionrequest.requestor,
          evt.xselectionrequest.property, XA_ATOM, 32, 0,
          reinterpret_cast<unsigned char*>(atoms.size() ? &atoms[0] : 0), static_cast<int>(atoms.size()));
    }
    else if (XA_STRING == evt.xselectionrequest.target || self.atombase_.utf8_string == evt.xselectionrequest.target) {
      std::string str;
      if (self.selection_.content.utf8_string) {
        str = *self.selection_.content.utf8_string;
      }
      ::XChangeProperty(self.display_, evt.xselectionrequest.requestor, evt.xselectionrequest.property, evt.xselectionrequest.target, 8, 0,
          reinterpret_cast<unsigned char*>(str.size() ? const_cast<std::string::value_type*>(str.c_str()) : 0), static_cast<int>(str.size()));
    }
    else {
      respond.xselection.property = None;
    }
    respond.xselection.type = SelectionNotify;
    respond.xselection.display = disp;
    respond.xselection.requestor = evt.xselectionrequest.requestor;
    respond.xselection.selection = evt.xselectionrequest.selection;
    respond.xselection.target = evt.xselectionrequest.target;
    respond.xselection.time = evt.xselectionrequest.time;
    platform_scope_guard_psg();
    ::XSendEvent(disp, evt.xselectionrequest.requestor, 0, 0, &respond);
    ::XFlush(disp);
    return 2;
  }
  else if (ClientMessage == evt.type) {
    if (self.atombase_.xdnd_enter == evt.xclient.message_type) {
      const Atom* atoms = reinterpret_cast<const Atom*>(&(evt.xclient.data.l[2]));
      unsigned long len = 3;
      unsigned char* data = 0;
      self.xdnd_.wd_src = evt.xclient.data.l[0];
      //Check whether there is more than three types.
      if (evt.xclient.data.l[1] & 1) {
        Atom type;
        int format;
        unsigned long bytes_left;
        ::XGetWindowProperty(self.display_, self.xdnd_.wd_src, self.atombase_.xdnd_typelist, 0, 0, False,
            XA_ATOM, &type, &format, &len, &bytes_left, &data);
        if (bytes_left > 0) {
          ::XGetWindowProperty(self.display_, self.xdnd_.wd_src, self.atombase_.xdnd_typelist,
              0, bytes_left, False, XA_ATOM,
              &type, &format, &len, &bytes_left, &data);
          if (XA_ATOM == type && len > 0) {
            atoms = reinterpret_cast<const Atom*>(data);
          }
        }
      }
      self.xdnd_.good_type = None;
      for (unsigned long i = 0; i < len; ++i) {
        if (atoms[i] == self.atombase_.text_uri_list) {
          self.xdnd_.good_type = self.atombase_.text_uri_list;
          break;
        }
      }
      if (data) {
        ::XFree(data);
      }
      return 2;
    }
    else if (self.atombase_.xdnd_position == evt.xclient.message_type) {
      Window wd_src = evt.xclient.data.l[0];
      int x = (evt.xclient.data.l[2] >> 16);
      int y = (evt.xclient.data.l[2] & 0xFFFF);
      bool accepted = false;
      //We have got the type what we want.
      if (self.xdnd_.good_type != None) {
        Window child;
        ::XTranslateCoordinates(self.display_, self.root_window(), evt.xclient.window, x, y, &self.xdnd_.pos.x, &self.xdnd_.pos.y, &child);
        typedef detail::bedrock bedrock;
        auto wd = bedrock::instance().wd_manager().find_window(reinterpret_cast<native_window_type>(evt.xclient.window), self.xdnd_.pos.x, self.xdnd_.pos.y);
        if (wd && wd->flags.dropable) {
          accepted = true;
          self.xdnd_.timestamp = evt.xclient.data.l[3];
          self.xdnd_.pos = wd->pos_root;
        }
      }
      XEvent respond;
      memset(&respond, 0, sizeof respond);
      respond.xany.type = ClientMessage;
      respond.xany.display = self.display_;
      respond.xclient.window = wd_src;
      respond.xclient.message_type = self.atombase_.xdnd_status;
      respond.xclient.format = 32;
      //Target window
      respond.xclient.data.l[0] = evt.xclient.window;
      //Accept set
      respond.xclient.data.l[1] = (accepted ? 1 : 0);
      respond.xclient.data.l[2] = 0;
      respond.xclient.data.l[3] = 0;
      respond.xclient.data.l[4] = self.atombase_.xdnd_action_copy;
      ::XSendEvent(self.display_, wd_src, True, NoEventMask, &respond);
      return 2;
    }
    else if (self.atombase_.xdnd_drop == evt.xclient.message_type) {
      ::XConvertSelection(self.display_, self.atombase_.xdnd_selection, self.xdnd_.good_type, self.atombase_.xdnd_selection,
          evt.xclient.window, self.xdnd_.timestamp);
      //The XdndDrop should send a XdndFinished to source window.
      //This operation is implemented in SelectionNotify, because
      //XdndFinished should be sent after retrieving data.
      return 2;
    }
  }
  return 0;
}
#endif
Window event_window(const XEvent& event)
{
  switch (event.type) {
  case MapNotify:
  case UnmapNotify:
  case DestroyNotify:
    return event.xmap.window;
  }
  return event.xkey.window;
}
int window_proc_for_xevent(Display* /*display*/, XEvent& xevent)
{
  static unsigned long last_mouse_down_time;
  //static core_window_t* last_mouse_down_window;
  Window native_window = event_window(xevent);
  //auto msgwnd = root_runtime->window;
  //auto& context = *brock.get_thread_context(msgwnd->thread_id);
  //auto pre_event_window = context.event_window;
  //auto pressed_wd = root_runtime->condition.pressed;
  //auto pressed_wd_space = root_runtime->condition.pressed_by_space;
  //auto hovered_wd = root_runtime->condition.hovered;
  const int message = xevent.type;
  switch (xevent.type) {
#if 0
  case EnterNotify:
    //Ignore mouse events when a window has been pressed by pressing spacebar.
    if (pressed_wd_space) {
      break;
    }
    msgwnd = wd_manager.find_window(native_window, xevent.xcrossing.x, xevent.xcrossing.y);
    if (msgwnd) {
      if (mouse_action::pressed != msgwnd->flags.action) {
        msgwnd->set_action(mouse_action::hovered);
      }
      hovered_wd = msgwnd;
      arg_mouse arg;
      assign_arg(arg, msgwnd, message, xevent);
      brock.emit(event_code::mouse_enter, msgwnd, arg, true, &context);
      arg.evt_code = event_code::mouse_move;
      brock.emit(event_code::mouse_move, msgwnd, arg, true, &context);
      if (!wd_manager.available(hovered_wd)) {
        hovered_wd = nullptr;
      }
    }
    break;
  case LeaveNotify:
    brock.event_msleave(hovered_wd);
    hovered_wd = nullptr;
    break;
  case FocusIn:
    brock.event_focus_changed(msgwnd, native_window, true);
    break;
  case FocusOut:
    if (native_interface::is_window(msgwnd->root)) {
      point pos = native_interface::cursor_position();
      auto recv = native_interface::find_window(pos.x, pos.y);
      brock.event_focus_changed(msgwnd, recv, false);
    }
    break;
  case ConfigureNotify:
    if (msgwnd->dimension.width != static_cast<unsigned>(xevent.xconfigure.width) || msgwnd->dimension.height != static_cast<unsigned>(xevent.xconfigure.height)) {
      auto& cf = xevent.xconfigure;
      wd_manager.size(msgwnd, size {static_cast<unsigned>(cf.width), static_cast<unsigned>(cf.height)}, true, true);
    }
    if (msgwnd->pos_native.x != xevent.xconfigure.x || msgwnd->pos_native.y != xevent.xconfigure.y) {
      msgwnd->pos_native.x = xevent.xconfigure.x;
      msgwnd->pos_native.y = xevent.xconfigure.y;
      brock.event_move(msgwnd, xevent.xconfigure.x, xevent.xconfigure.y);
    }
    break;
  case ButtonPress:
    //Ignore mouse events when a window has been pressed by pressing spacebar
    if (pressed_wd_space) {
      break;
    }
    if (xevent.xbutton.button == Button4 || xevent.xbutton.button == Button5) {
      break;
    }
    msgwnd = wd_manager.find_window(native_window, xevent.xbutton.x, xevent.xbutton.y);
    if (nullptr == msgwnd) {
      break;
    }
    if ((msgwnd == msgwnd->root_widget->other.attribute.root->menubar) && brock.get_menu(msgwnd->root, true)) {
      brock.erase_menu(true);
    }
    else {
      brock.close_menu_if_focus_other_window(msgwnd->root);
    }
    if (msgwnd->flags.enabled) {
      bool dbl_click = (last_mouse_down_window == msgwnd) && (xevent.xbutton.time - last_mouse_down_time <= 400);
      last_mouse_down_time = xevent.xbutton.time;
      last_mouse_down_window = msgwnd;
      if (Button1 == xevent.xbutton.button) { //Sets the focus only if left button is pressed
        auto new_focus = (msgwnd->flags.take_active ? msgwnd : msgwnd->other.active_window);
        if (new_focus && !new_focus->flags.ignore_mouse_focus) {
          context.event_window = new_focus;
          auto kill_focus = brock.wd_manager().set_focus(new_focus, false, arg_focus::reason::mouse_press);
          if (kill_focus != new_focus) {
            wd_manager.do_lazy_refresh(kill_focus, false);
          }
        }
      }
      auto retain = msgwnd->annex.events_ptr;
      context.event_window = msgwnd;
      pressed_wd = nullptr;
      msgwnd->set_action(mouse_action::pressed);
      arg_mouse arg;
      assign_arg(arg, msgwnd, ButtonPress, xevent);
      arg.evt_code = dbl_click ? event_code::dbl_click : event_code::mouse_down;
      if (brock.emit(arg.evt_code, msgwnd, arg, true, &context)) {
        if (wd_manager.available(msgwnd)) {
          pressed_wd = msgwnd;
          //If a root window is created during the mouse_down event, Nana.GUI will ignore the mouse_up event.
          if (msgwnd->root != native_interface::get_focus_window()) {
            //call the drawer mouse up event for restoring the surface graphics
            msgwnd->set_action(mouse_action::normal);
            draw_invoker(&drawer::mouse_up, msgwnd, arg, &context);
            wd_manager.do_lazy_refresh(msgwnd, false);
          }
        }
      }
    }
    break;
  case ButtonRelease:
    //Ignore mouse events when a window has been pressed by pressing spacebar
    if (pressed_wd_space) {
      break;
    }
    if (xevent.xbutton.button == Button4 || xevent.xbutton.button == Button5) {
      //The hovered window receives the message, unlike in Windows, no redirection is required.
      point mspos {xevent.xbutton.x, xevent.xbutton.y};
      while (msgwnd) {
        if (msgwnd->annex.events_ptr->mouse_wheel.length() != 0) {
          mspos -= msgwnd->pos_root;
          arg_wheel arg;
          arg.which = arg_wheel::wheel::vertical;
          assign_arg(arg, msgwnd, xevent);
          brock.emit(event_code::mouse_wheel, msgwnd, arg, true, &context);
          break;
        }
        msgwnd = msgwnd->parent;
      }
    }
    else {
      msgwnd = wd_manager.find_window(native_window, xevent.xbutton.x, xevent.xbutton.y);
      if (nullptr == msgwnd) {
        break;
      }
      msgwnd->set_action(mouse_action::normal);
      if (msgwnd->flags.enabled) {
        auto retain = msgwnd->annex.events_ptr;
        ::arg_mouse arg;
        assign_arg(arg, msgwnd, message, xevent);
        ::arg_click click_arg;
        //the window_handle of click_arg is used as a flag to determinate whether to emit click event.
        click_arg.window_handle = nullptr;
        click_arg.mouse_args = &arg;
        const bool hit = msgwnd->dimension.is_hit(arg.pos);
        if (msgwnd == pressed_wd) {
          if ((arg.button == ::mouse::left_button) && hit) {
            msgwnd->set_action(mouse_action::hovered);
            click_arg.window_handle = reinterpret_cast<window>(msgwnd);
            draw_invoker(&drawer::click, msgwnd, click_arg, &context);
          }
        }
        //Do mouse_up, this handle may be closed by click handler.
        if (wd_manager.available(msgwnd) && msgwnd->flags.enabled) {
          if (hit) {
            msgwnd->set_action(mouse_action::hovered);
          }
          auto retain = msgwnd->annex.events_ptr;
          auto evt_ptr = retain.get();
          arg.evt_code = event_code::mouse_up;
          draw_invoker(&drawer::mouse_up, msgwnd, arg, &context);
          if (click_arg.window_handle) {
            evt_ptr->click.emit(click_arg, reinterpret_cast<window>(msgwnd));
          }
          if (wd_manager.available(msgwnd)) {
            arg.evt_code = event_code::mouse_up;
            evt_ptr->mouse_up.emit(arg, reinterpret_cast<window>(msgwnd));
          }
        }
        else if (click_arg.window_handle) {
          msgwnd->annex.events_ptr->click.emit(click_arg, reinterpret_cast<window>(msgwnd));
        }
        wd_manager.do_lazy_refresh(msgwnd, false);
      }
      pressed_wd = nullptr;
    }
    break;
  case DestroyNotify:
    if (wd_manager.available(msgwnd)) {
      //The msgwnd may be destroyed if the window is destroyed by calling native interface of close_window().
      if (msgwnd->root == brock.get_menu()) {
        brock.erase_menu(false);
        brock.delay_restore(3); //Restores if delay_restore not decleared
      }
      auto& spec = ::detail::platform_spec::instance();
      spec.remove(native_window);
      wd_manager.destroy(msgwnd);
      brock.manage_form_loader(msgwnd, false);
      wd_manager.destroy_handle(msgwnd);
    }
    break;
  case MotionNotify:
    //X may send the MotionNotify with same information repeatly.
    //Nana should ignore the repeated notify.
    if (context.platform.motion_window != native_window || context.platform.motion_pointer_pos != point(xevent.xmotion.x, xevent.xmotion.y)) {
      context.platform.motion_window = native_window;
      context.platform.motion_pointer_pos = point(xevent.xmotion.x, xevent.xmotion.y);
    }
    else {
      break;
    }
    //Ignore mouse events when a window has been pressed by pressing spacebar
    if (pressed_wd_space) {
      break;
    }
    msgwnd = wd_manager.find_window(native_window, xevent.xmotion.x, xevent.xmotion.y);
    if (wd_manager.available(hovered_wd) && (msgwnd != hovered_wd)) {
      brock.event_msleave(hovered_wd);
      hovered_wd->set_action(mouse_action::normal);
      hovered_wd = nullptr;
      //if msgwnd is neither a captured window nor a child of captured window,
      //redirect the msgwnd to the captured window.
      auto cap_wd = wd_manager.capture_redirect(msgwnd);
      if (cap_wd) {
        msgwnd = cap_wd;
      }
    }
    else if (msgwnd) {
      bool prev_captured_inside;
      if (wd_manager.capture_window_entered(xevent.xmotion.x, xevent.xmotion.y, prev_captured_inside)) {
        event_code evt_code;
        if (prev_captured_inside) {
          evt_code = event_code::mouse_leave;
          msgwnd->set_action(mouse_action::normal);
        }
        else {
          evt_code = event_code::mouse_enter;
          if (mouse_action::pressed != msgwnd->flags.action) {
            msgwnd->set_action(mouse_action::hovered);
          }
        }
        arg_mouse arg;
        assign_arg(arg, msgwnd, message, xevent);
        arg.evt_code = evt_code;
        brock.emit(evt_code, msgwnd, arg, true, &context);
      }
    }
    if (msgwnd) {
      arg_mouse arg;
      assign_arg(arg, msgwnd, message, xevent);
      if (mouse_action::pressed != msgwnd->flags.action) {
        msgwnd->set_action(mouse_action::hovered);
      }
      if (hovered_wd != msgwnd) {
        hovered_wd = msgwnd;
        arg.evt_code = event_code::mouse_enter;
        brock.emit(event_code::mouse_enter, msgwnd, arg, true, &context);
      }
      arg.evt_code = event_code::mouse_move;
      brock.emit(event_code::mouse_move, msgwnd, arg, true, &context);
    }
    if (!wd_manager.available(hovered_wd)) {
      hovered_wd = nullptr;
    }
    break;
  case MapNotify:
  case UnmapNotify:
    brock.event_expose(msgwnd, (xevent.type == MapNotify));
    context.platform.motion_window = nullptr;
    break;
  case Expose:
    if (msgwnd->visible && (msgwnd->root_graph->empty() == false)) {
      detail::platform_scope_guard psg;
      //Don't copy root_graph to the window directly, otherwise the edge nimbus effect will be missed.
      ::rectangle update_area(xevent.xexpose.x, xevent.xexpose.y, xevent.xexpose.width, xevent.xexpose.height);
      if (!update_area.empty()) {
        msgwnd->drawer.map(reinterpret_cast<window>(msgwnd), true, &update_area);
      }
    }
    break;
  case KeyPress:
    detail::platform_spec::instance().write_keystate(xevent.xkey);
    if (msgwnd->flags.enabled) {
      auto menu_wd = brock.get_menu();
      if (menu_wd) {
        brock.delay_restore(0); //Enable delay restore
      }
      if (msgwnd->root != menu_wd) {
        msgwnd = brock.focus();
      }
      if (msgwnd) {
        KeySym keysym;
        Status status;
        char fixbuf[33];
        char* keybuf = fixbuf;
        int len = 0;
        XIC input_context = detail::platform_spec::instance().caret_input_context(native_window);
        if (input_context) {
          detail::platform_scope_guard psg;
#if 1 //Utf8
          len = ::Xutf8LookupString(input_context, &xevent.xkey, keybuf, 32, &keysym, &status);
          if (status == XBufferOverflow) {
            keybuf = new char[len + 1];
            len = ::Xutf8LookupString(input_context, &xevent.xkey, keybuf, len, &keysym, &status);
          }
#else
          len = ::XmbLookupString(input_context, &xevent.xkey, keybuf, 32, &keysym, &status);
          if (status == XBufferOverflow) {
            keybuf = new char[len + 1];
            len = ::XmbLookupString(input_context, &xevent.xkey, keybuf, len, &keysym, &status);
          }
#endif
        }
        else {
          detail::platform_scope_guard psg;
          status = XLookupKeySym;
          keysym = ::XLookupKeysym(&xevent.xkey, 0);
        }
        keybuf[len] = 0;
        wchar_t os_code = 0;
        switch (status) {
        case XLookupKeySym:
        case XLookupBoth:
          os_code = os_code_from_keysym(keysym);
          if (os_code == keyboard::tab && (false == (msgwnd->flags.tab & detail::tab_type::eating))) { //Tab
            arg_keyboard argkey;
            brock.get_key_state(argkey);
            auto tstop_wd = wd_manager.tabstop(msgwnd, !argkey.shift);
            if (tstop_wd) {
              root_runtime->condition.ignore_tab = true;
              wd_manager.set_focus(tstop_wd, false, arg_focus::reason::tabstop);
              wd_manager.do_lazy_refresh(msgwnd, false);
              wd_manager.do_lazy_refresh(tstop_wd, true);
            }
          }
          else if ((keyboard::space == os_code) && msgwnd->flags.space_click_enabled) {
            //Clicked by spacebar
            if ((nullptr == pressed_wd) && (nullptr == pressed_wd_space)) {
              arg_mouse arg;
              arg.alt = false;
              arg.button = ::mouse::left_button;
              arg.ctrl = false;
              arg.evt_code = event_code::mouse_down;
              arg.left_button = true;
              arg.mid_button = false;
              arg.pos.x = 0;
              arg.pos.y = 0;
              arg.window_handle = reinterpret_cast<window>(msgwnd);
              msgwnd->set_action(mouse_action::pressed);
              pressed_wd_space = msgwnd;
              auto retain = msgwnd->annex.events_ptr;
              draw_invoker(&drawer::mouse_down, msgwnd, arg, &context);
              wd_manager.do_lazy_refresh(msgwnd, false);
            }
          }
          else if (keyboard::alt == os_code) {
            context.is_alt_pressed = true;
            if (brock.shortkey_occurred() == false) {
              msgwnd = msgwnd->root_widget->other.attribute.root->menubar;
              if (msgwnd) {
                bool focused = (brock.focus() == msgwnd);
                arg_keyboard arg;
                arg.evt_code = event_code::key_press;
                arg.window_handle = reinterpret_cast<window>(msgwnd);
                arg.ignore = false;
                arg.key = os_code;
                brock.get_key_state(arg);
                brock.emit(event_code::key_press, msgwnd, arg, true, &context);
                msgwnd->root_widget->flags.ignore_menubar_focus = (focused && (brock.focus() != msgwnd));
              }
              else {
                brock.erase_menu(true);
              }
            }
          }
          else {
            if (keyboard::os_ctrl == os_code) {
              context.is_ctrl_pressed = true;
            }
            arg_keyboard arg;
            arg.ignore = false;
            arg.key = os_code;
            arg.evt_code = event_code::key_press;
            brock.get_key_state(arg);
            arg.window_handle = reinterpret_cast<window>(msgwnd);
            brock.emit(event_code::key_press, msgwnd, arg, true, &context);
            if (wd_manager.available(msgwnd) && (msgwnd->root_widget->other.attribute.root->menubar == msgwnd)) {
              int cmd = (menu_wd && (keyboard::escape == static_cast<wchar_t>(arg.key)) ? 1 : 0);
              brock.delay_restore(cmd);
            }
          }
          if (XLookupKeySym == status) {
            wd_manager.do_lazy_refresh(msgwnd, false);
            break;
          }
        case XLookupChars:
          if (msgwnd->flags.enabled) {
            const wchar_t* charbuf;
            detail::charset_conv charset("UTF-32", "UTF-8");
            const std::string& str = charset.charset(std::string(keybuf, keybuf + len));
            charbuf = reinterpret_cast<const wchar_t*>(str.c_str()) + 1;
            len = str.size() / sizeof(wchar_t) - 1;
            for (int i = 0; i < len; ++i) {
              arg_keyboard arg;
              arg.ignore = false;
              arg.key = charbuf[i];
              //Only accept tab when it is not ignored.
              if ((keyboard::tab == arg.key) && root_runtime->condition.ignore_tab) {
                continue;
              }
              if (context.is_alt_pressed) {
                arg.ctrl = arg.shift = false;
                arg.evt_code = event_code::shortkey;
                brock.shortkey_occurred(true);
                auto shr_wd = wd_manager.find_shortkey(native_window, arg.key);
                if (shr_wd) {
                  arg.window_handle = reinterpret_cast<window>(shr_wd);
                  brock.emit(event_code::shortkey, shr_wd, arg, true, &context);
                }
                continue;
              }
              arg.evt_code = event_code::key_char;
              arg.window_handle = reinterpret_cast<window>(msgwnd);
              brock.get_key_state(arg);
              msgwnd->annex.events_ptr->key_char.emit(arg, reinterpret_cast<window>(msgwnd));
              if (arg.ignore == false && wd_manager.available(msgwnd)) {
                draw_invoker(&drawer::key_char, msgwnd, arg, &context);
              }
            }
            if (brock.shortkey_occurred(false)) {
              context.is_alt_pressed = false;
            }
          }
          break;
        }
        wd_manager.do_lazy_refresh(msgwnd, false);
        if (keybuf != fixbuf) {
          delete [] keybuf;
        }
      }
    }
    break;
  case KeyRelease:
    detail::platform_spec::instance().write_keystate(xevent.xkey);
    {
      auto os_code = os_code_from_keysym(::XLookupKeysym(&xevent.xkey, 0));
      if (keyboard::alt != os_code) { //MUST NOT BE AN ALT
        if (0x11 == os_code) {
          context.is_ctrl_pressed = false;
        }
        if (('\t' == os_code) && root_runtime->condition.ignore_tab) {
          root_runtime->condition.ignore_tab = false;
        }
        else {
          msgwnd = brock.focus();
          if (msgwnd) {
            if (msgwnd == pressed_wd_space) {
              msgwnd->set_action(mouse_action::normal);
              arg_click click_arg;
              click_arg.mouse_args = nullptr;
              click_arg.window_handle = reinterpret_cast<window>(msgwnd);
              auto retain = msgwnd->annex.events_ptr;
              if (brock.emit(event_code::click, msgwnd, click_arg, true, &context)) {
                arg_mouse arg;
                arg.alt = false;
                arg.button = ::mouse::left_button;
                arg.ctrl = false;
                arg.evt_code = event_code::mouse_up;
                arg.left_button = true;
                arg.mid_button = false;
                arg.pos.x = 0;
                arg.pos.y = 0;
                arg.window_handle = reinterpret_cast<window>(msgwnd);
                draw_invoker(&drawer::mouse_up, msgwnd, arg, &context);
                wd_manager.do_lazy_refresh(msgwnd, false);
              }
              pressed_wd_space = nullptr;
            }
            else {
              arg_keyboard arg;
              arg.evt_code = event_code::key_release;
              arg.window_handle = reinterpret_cast<window>(msgwnd);
              arg.ignore = false;
              arg.key = os_code;
              brock.get_key_state(arg);
              brock.emit(event_code::key_release, msgwnd, arg, true, &context);
            }
          }
        }
        if (os_code < keyboard::os_arrow_left || keyboard::os_arrow_down < os_code) {
          brock.delay_restore(2); //Restores while key release
        }
      }
      else {
        context.is_alt_pressed = false;
        if (brock.shortkey_occurred(false) == false) {
          msgwnd = msgwnd->root_widget->other.attribute.root->menubar;
          if (msgwnd) {
            bool set_focus = (brock.focus() != msgwnd) && (!msgwnd->root_widget->flags.ignore_menubar_focus);
            if (set_focus) {
              wd_manager.set_focus(msgwnd, false, arg_focus::reason::general);
            }
            arg_keyboard arg;
            arg.evt_code = event_code::key_release;
            arg.window_handle = reinterpret_cast<window>(msgwnd);
            arg.ignore = false;
            arg.key = os_code;
            brock.get_key_state(arg);
            brock.emit(event_code::key_release, msgwnd, arg, true, &context);
            if (!set_focus) {
              brock.set_menubar_taken(nullptr);
              msgwnd->root_widget->flags.ignore_menubar_focus = false;
            }
          }
        }
      }
    }
    break;
  default:
    if (message == ClientMessage) {
      auto& atoms = detail::platform_spec::instance().atombase();
      if (atoms.wm_protocols == xevent.xclient.message_type) {
        if (msgwnd->flags.enabled && (atoms.wm_delete_window == static_cast<Atom>(xevent.xclient.data.l[0]))) {
          arg_unload arg;
          arg.window_handle = reinterpret_cast<window>(msgwnd);
          arg.cancel = false;
          brock.emit(event_code::unload, msgwnd, arg, true, &context);
          if (false == arg.cancel) {
            native_interface::close_window(native_window);
          }
        }
      }
    }
#endif
  }
  return 0;
}
void _m_msg_driver()
{
  platform_x11* x11 = get_x11();
  const int fd_X11 = ConnectionNumber(x11->display_);
  msg_packet_tag msg_pack;
  XEvent event;
  while (opt_native_window(0)) {
    int pending;
    {
      //platform_scope_guard_lock;
      pending = ::XPending(x11->display_);
      if (pending) {
        ::XNextEvent(x11->display_, &event);
        if (KeyRelease == event.type) {
          //Check whether the key is pressed, because X will send KeyRelease when pressing and
          //holding a key if auto repeat is on.
          char keymap[32];
          ::XQueryKeymap(x11->display_, keymap);
          if (keymap[event.xkey.keycode / 8] & (1 << (event.xkey.keycode % 8))) {
            continue;
          }
        }
        if (::XFilterEvent(&event, None)) {
          continue;
        }
      }
    }
    if (0 == pending) {
      fd_set fdset;
      FD_ZERO(&fdset);
      FD_SET(fd_X11, &fdset);
      struct timeval tv;
      tv.tv_usec = 10000;
      tv.tv_sec = 0;
      ::select(fd_X11 + 1, &fdset, 0, 0, &tv);
    }
    else {
#if 0
      switch (_m_msg_filter(event, msg_pack)) {
      case 0:
        msg_pack.kind = kind_xevent;
        msg_pack.u.xevent = event;
      case 1:
        _m_msg_dispatch(msg_pack);
      }
#endif
      window_proc_for_xevent(0, event);
    }
  }
}
void pump_event(native_window_t* modal_window, bool is_modal)
{
  _m_msg_driver();
}

