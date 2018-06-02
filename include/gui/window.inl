// Select platform ......
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) //Microsoft Windows
#define NANA_WINDOWS
// MINGW ...
#if defined(__MINGW32__) || defined(__MINGW64__) || defined(MINGW)
#define NANA_MINGW
#endif // MINGW
#elif defined(APPLE) //Mac OS X
//Symbols for MACOS
#define NANA_MACOS
#define NANA_X11
#elif (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC) //Linux
#define NANA_LINUX
#define NANA_X11
#else
static_assert(false, "Only Windows and Unix are supported now (Mac OS is experimental)");
#endif
//Define a symbol for POSIX operating system.
#if defined(NANA_LINUX) || defined(NANA_MACOS)
#define NANA_POSIX
#endif
#if defined(NANA_WINDOWS)
#elif defined(NANA_X11)
#include <X11/Xlib.h>
#include <X11/Xlocale.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xos.h>
#endif
enum {unicode_utf8, unicode_utf16, unicode_utf32};
enum { mouse_begin, mouse_normal = mouse_begin, mouse_hovered, mouse_pressed, mouse_end };
enum state {
  state_normal,
  state_hovered,
  state_focus_normal,
  state_focus_hovered,
  state_pressed,
  state_disabled
};
typedef unsigned scalar_t;
typedef unsigned char uint8_t;
typedef unsigned long uint32_t;
typedef unsigned uint_t;
#ifdef NANA_WINDOWS
typedef __int64 long_long_t;
#else
typedef long long long_long_t;
#endif
union pixel_argb_t {
  struct element_tag {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha_channel;
  } element;
  unsigned value;
};
/// See extended CSS color keywords (4.3) in http://www.w3.org/TR/2011/REC-css3-color-20110607/
enum {
  color_alice_blue = 0xf0f8ff,
  color_antique_white = 0xfaebd7,
  color_aqua = 0xFFFF,
  color_aquamarine = 0x7fffd4,
  color_azure = 0xf0ffff,
  color_beige = 0xf5f5dc,
  color_bisque = 0xffe4ce,
  color_black = 0x0,
  color_blanched_almond = 0xffebcd,
  color_blue = 0x0000FF,
  color_blue_violet = 0x8a2be2,
  color_brown = 0xa52a2a,
  color_burly_wood = 0xdeb887,
  color_cadet_blue = 0x5f9ea0,
  color_chartreuse = 0x7fff00,
  color_chocolate = 0xd2691e,
  color_coral = 0xff7f50,
  color_cornflower_blue = 0x6495ed,
  color_cornsilk = 0xfff8dc,
  color_crimson = 0xdc143c,
  color_cyan = 0xffff,
  color_dark_blue = 0x8b,
  color_dark_cyan = 0x8b8b,
  color_dark_goldenrod = 0xb8860b,
  color_dark_gray = 0xa9a9a9,
  color_dark_green = 0x6400,
  color_dark_grey = color_dark_gray,
  color_dark_khaki = 0xbdb76b,
  color_dark_magenta = 0x8b008b,
  color_dark_olive_green = 0x556b2f,
  color_dark_orange = 0xff8c00,
  color_dark_orchid = 0x9932cc,
  color_dark_red = 0x8b0000,
  color_dark_salmon = 0xe9976a,
  color_dark_sea_green = 0x8fbc8f,
  color_dark_slate_blue = 0x483d8b,
  color_dark_slate_gray = 0x2f4f4f,
  color_dark_slate_grey = 0x2f4f4f,
  color_dark_turquoise = 0xced1,
  color_dark_violet = 0x9400d3,
  color_deep_pink = 0xff1493,
  color_deep_sky_blue = 0xbfff,
  color_dim_gray = 0x696969,
  color_dim_grey = color_dim_gray,
  color_dodger_blue = 0x1e90ff,
  color_firebrick = 0xb22222,
  color_floral_white = 0xfffaf0,
  color_forest_green = 0x228b22,
  color_fuchsia = 0xFF00FF,
  color_gainsboro = 0xdcdcdc,
  color_ghost_white = 0xf8f8ff,
  color_gold = 0xffd700,
  color_goldenrod = 0xdaa520,
  color_gray = 0x808080,
  color_green = 0x008000,
  color_green_yellow = 0xadff2f,
  color_grey = color_gray,
  color_honeydew = 0xf0fff0,
  color_hot_pink = 0xff69b4,
  color_indian_red = 0xcd5c5c,
  color_indigo = 0x4b0082,
  color_ivory = 0xfffff0,
  color_khaki = 0xf0e68c,
  color_lavendar = 0xe6e6fa,
  color_lavender_blush = 0xfff0f5,
  color_lawn_green = 0x7cfc00,
  color_lemon_chiffon = 0xfffacd,
  color_light_blue = 0xadd8e6,
  color_light_coral = 0xf08080,
  color_light_cyan = 0xe0ffff,
  color_light_goldenrod_yellow = 0xfafad2,
  color_light_gray = 0xd3d3d3,
  color_light_green = 0x90ee90,
  color_light_grey = color_light_gray,
  color_light_pink = 0xffb6c1,
  color_light_salmon = 0xffa07a,
  color_light_sea_green = 0x20b2aa,
  color_light_sky_blue = 0x87cefa,
  color_light_slate_gray = 0x778899,
  color_light_slate_grey = color_light_slate_gray,
  color_light_steel_blue = 0xb0c4de,
  color_light_yellow = 0xffffe0,
  color_lime = 0x00FF00,
  color_lime_green = 0x32cd32,
  color_linen = 0xfaf0e6,
  color_magenta = 0xff00ff,
  color_maroon = 0x800000,
  color_medium_aquamarine = 0x66cdaa,
  color_medium_blue = 0xcd,
  color_medium_orchid = 0xba55d3,
  color_medium_purple = 0x9370db,
  color_medium_sea_green = 0x3cb371,
  color_medium_slate_blue = 0x7b68ee,
  color_medium_spring_green = 0xfa9a,
  color_medium_turquoise = 0x48d1cc,
  color_medium_violet_red = 0xc71585,
  color_midnight_blue = 0x191970,
  color_mint_cream = 0xf5fffa,
  color_misty_rose = 0xffe4e1,
  color_moccasin = 0xffe4b5,
  color_navajo_white = 0xffdead,
  color_navy = 0x000080,
  color_old_lace = 0xfdf5e6,
  color_olive = 0x808000,
  color_olive_drab = 0x6b8e23,
  color_orange = 0xffa500,
  color_orange_red = 0xff4500,
  color_orchid = 0xda70d6,
  color_pale_goldenrod = 0xeee8aa,
  color_pale_green = 0x98fb98,
  color_pale_turquoise = 0xafeeee,
  color_pale_violet_red = 0xdb7093,
  color_papaya_whip = 0xffefd5,
  color_peach_puff = 0xffdab9,
  color_peru = 0xcd853f,
  color_pink = 0xffc0cb,
  color_plum = 0xdda0dd,
  color_powder_blue = 0xb0e0e6,
  color_purple = 0x800080,
  color_red = 0xFF0000,
  color_rosy_brown = 0xbc8f8f,
  color_royal_blue = 0x4169e1,
  color_saddle_brown = 0x8b4513,
  color_salmon = 0xfa8072,
  color_sandy_brown = 0xf4a460,
  color_sea_green = 0x2e8b57,
  color_sea_shell = 0xfff5ee,
  color_sienna = 0xa0522d,
  color_silver = 0xc0c0c0,
  color_sky_blue = 0x87ceeb,
  color_slate_blue = 0x6a5acd,
  color_slate_gray = 0x708090,
  color_slate_grey = 0x708090,
  color_snow = 0xfffafa,
  color_spring_green = 0xff7f,
  color_steel_blue = 0x4682b4,
  color_tan = 0xd2b48c,
  color_teal = 0x008080,
  color_thistle = 0xd8bfd8,
  color_tomato = 0xff6347,
  color_turquoise = 0x40e0d0,
  color_violet = 0xee82ee,
  color_wheat = 0xf5deb3,
  color_white = 0xFFFFFF,
  color_white_smoke = 0xf5f5f5,
  color_yellow = 0xFFFF00,
  color_yellow_green = 0x9acd32,
  //temporary defintions, these will be replaced by color schema
  color_button_face_shadow_start = 0xF5F4F2,
  color_button_face_shadow_end = 0xD5D2CA,
  color_button_face = 0xD4D0C8 , //,light_cyan
  color_dark_border = 0x404040,
  color_gray_border = 0x808080,
  color_highlight = 0x1CC4F7
};
enum {
  arrange_unknown, arrange_horizontal, arrange_vertical, arrange_horizontal_vertical
};
///The definition of horizontal alignment
enum {
  align_left, align_center, align_right
};
///The definition of vertical alignment
enum {
  align_v_top, align_v_center, align_v_bottom
};
///The definition of the four corners of the world
enum {
  direction_north,
  direction_south,
  direction_east,
  direction_west,
  direction_southeast
};
enum {
  appearance_taskbar = 0x01,
  appearance_floating = 0x02,
  appearance_no_activate = 0x04,
  appearance_minimize = 0x08,
  appearance_maximize = 0x10,
  appearance_sizable = 0x20,
  appearance_decoration = 0x40,
  appearance_default = appearance_taskbar | appearance_minimize | appearance_maximize | appearance_sizable | appearance_decoration,
};
enum {
  event_click, ///< A mouse click event.
  event_dbl_click, ///< A mouse double click event.
  event_mouse_enter, ///< A mouse enters a widget.
  event_mouse_move, ///< A mouse moves over a widget.
  event_mouse_leave, ///< A mouse leaves a widget.
  event_mouse_down, ///< A mouse button is pressed on a widget.
  event_mouse_up, ///< A mouse button is released on a widget.
  event_mouse_wheel, ///< A mouse scrolls the wheel on a widget.
  event_mouse_drop, ///< A mouse release over a window that is registered as recipient of drag and drop.
  event_expose, ///<
  event_resizing, ///< A widget's size is sizing. In this event, A widget's size can be overrided with a new size.
  event_resized, ///< A widget's size is changing.
  event_move, ///<
  event_unload, ///< A form is closed by clicking the X button, only works for root widget.
  event_destroy, ///< A widget is about to be destroyed.
  event_focus, ///< A widget's focus is changed.
  event_key_press, ///< A keyboard is pressed on a focus widget.
  event_key_char, ///< The focus widget received a character.
  event_key_release, ///< A keyboard is released on a focus widget.
  event_shortkey, ///< The widgets received a shortkey message.
  event_elapse, ///< Unoperational events? . A widget received a tick that is sended by timer.
  event_end ///< End indicator, it's not an event.
};
enum {
  checkstate_unchecked, checkstate_checked, checkstate_partial
};
enum {
  border_none,
  border_left, border_right, border_top, border_bottom,
  border_top_left, border_top_right, border_bottom_left, border_bottom_right
};
enum {
  bground_none,
  bground_basic,
  bground_blend
};
enum {
  flags_super,
  flags_widget = 0x1,
  flags_lite_widget = 0x3,
  flags_root = 0x5
#ifndef WIDGET_FRAME_DEPRECATED
      , flags_frame = 0x9
#endif
};
enum {
  //Control Code for ASCII
  keyboard_start_of_headline = 0x1, //Ctrl+A
  keyboard_end_of_text = 0x3, //Ctrl+C
  keyboard_backspace = 0x8, tab = 0x9,
  keyboard_alt = 0x12,
  keyboard_enter_n = 0xA, enter = 0xD, enter_r = 0xD,
  keyboard_sync_idel = 0x16, //Ctrl+V
  keyboard_cancel = 0x18, //Ctrl+X
  keyboard_end_of_medium = 0x19, //Ctrl+Y
  keyboard_substitute = 0x1A, //Ctrl+Z
  keyboard_escape = 0x1B,
  keyboard_space = 0x20, //Space
  //The following names are intuitive name of ASCII control codes
  keyboard_select_all = keyboard_start_of_headline,
  keyboard_copy = keyboard_end_of_text,
  keyboard_paste = keyboard_sync_idel,
  keyboard_cut = keyboard_cancel,
  keyboard_redo = keyboard_end_of_medium,
  keyboard_undo = keyboard_substitute,
  //System Code for OS
  keyboard_os_tab = 0x09,
  keyboard_os_shift = 0x10,
  keyboard_os_ctrl = 0x11,
  keyboard_os_pageup = 0x21, keyboard_os_pagedown,
  keyboard_os_arrow_left = 0x25, keyboard_os_arrow_up, keyboard_os_arrow_right, keyboard_os_arrow_down,
  keyboard_os_insert = 0x2D, keyboard_os_del ,
  keyboard_os_end = 0x23 , keyboard_os_home //Pos 1
};
enum {
  cursor_hand = 60, ///< displays a hand to indicate a text or an element is clickable
  cursor_arrow = 68, ///< the default shape
  cursor_wait = 150, ///< indicates the system is currently busy
  cursor_iterm = 152, ///< A text caret. Displays a caret to indicate the UI is input able
  cursor_size_we = 108,
  cursor_size_ns = 116,
  cursor_size_top_left = 134,
  cursor_size_top_right = 136,
  cursor_size_bottom_left = 12,
  cursor_size_bottom_right = 14
};
enum {
  mouse_any_button,
  mouse_left_button,
  mouse_middle_button,
  mouse_right_button
};
enum {
  z_order_action_none,
  z_order_action_bottom, ///< brings a window at the bottom of z-order.
  z_order_action_top, ///< brings a widget at the top of the z-order.
  z_order_action_topmost, ///< brings a window at the top of the z-order and stays here.
  z_order_action_foreground ///< brings a window to the foreground.
};
enum {
  msg_tray = 0x501,
  msg_async_activate,
  msg_async_set_focus,
  msg_remote_flush_surface,
  msg_remote_thread_destroy_window,
  msg_remote_thread_move_window,
  msg_operate_caret, //wParam: 1=Destroy, 2=SetPos
  msg_remote_thread_set_window_pos,
  msg_remote_thread_set_window_text,
  //Execute a function in a thread with is associated with a specified native window
  msg_affinity_execute,
  msg_user,
};
enum { move_window_Pos = 1, move_window_Size = 2};
typedef struct move_window_t move_window_t;
struct move_window_t {
  int x;
  int y;
  unsigned w;
  unsigned h;
  unsigned ignore; //determinate that pos or size would be ignored.
};
typedef struct native_window_t native_window_t;
typedef struct widget_t widget_t;
typedef struct image_t image_t;
typedef struct graphics_t graphics_t;
#if defined(NANA_WINDOWS)
typedef HWND native_window_type;
#elif defined(NANA_X11)
typedef unsigned long native_window_type;
#endif
typedef struct event_arg event_arg;
enum { wheel_vertical, wheel_horizontal };
enum {
  reason_general, ///< the focus is received by OS native window manager.
  reason_tabstop, ///< the focus is received by pressing tab.
  reason_mouse_press ///< the focus is received by pressing a mouse button.
};
struct event_arg {
  int evt_code; ///< what kind of mouse event?
  native_window_type window_handle; ///< A handle to the event window
  IPOINT pos; ///< cursor position in the event window
  int button; ///< indicates a button which triggers the event
  bool left_button; ///< true if mouse left button is pressed
  bool mid_button; ///< true if mouse middle button is pressed
  bool right_button; ///< true if mouse right button is pressed
  bool alt; ///< true if keyboard alt is pressed
  bool shift; ///< true if keyboard Shift is pressed
  bool ctrl; ///< true if keyboard Ctrl is pressed
  bool exposed; ///< the window is visible?
  int which; ///< which wheel is rotated
  bool upwards; ///< true if the wheel is rotated to the top/left, depends on which and false otherwise
  unsigned distance; ///< expressed in multiples or divisions of 120
  char** files; ///< external filenames
  native_window_type receiver; ///< it is a native window handle, and specified which window receives focus
  bool getting; ///< the window received focus?
  int focus_reason; ///< determines how the widget receives keyboard focus, it is ignored when 'getting' is equal to false
  int key; ///< the key corresponding to the key pressed
  bool ignore; ///< this member is only available for key_char event, set 'true' to ignore the input.
  ISIZE sz;
};
struct native_window_t {
  native_window_type hwnd;
  HDC hdc;
  IRECT rect;
  IRECT rcPaint;
  widget_t* form;
  native_window_t* prev;
  native_window_t* next;
  native_window_t* hash_next;
};
native_window_t* opt_native_window(int opt)
{
  static native_window_t* g_root = NULL;
  native_window_t* win = NULL;
  native_window_t* cur = g_root;
  switch (opt) {
  case 1: { //get
    for (; cur; cur = cur->next) {
      if (NULL == cur->form) {
        win = cur;
        break;
      }
    }
    if (NULL == win) {
      MYREALLOC(win, 1);
      win->prev = NULL;
      win->next = g_root;
      if (g_root) {
        g_root->prev = win;
      }
      else {
        g_root = win;
      }
    }
    return win;
  }
  break;
  case 2://del
    break;
  }
  return g_root;
}
native_window_t* opt_native_window_map(int opt, native_window_type hwnd, native_window_t* ptr)
{
  enum {hashsize = 97};
  static native_window_t* g_hashmap[hashsize] = {0};
  unsigned int cc = ((unsigned int)(hwnd)) % (hashsize);
  native_window_t* tt = g_hashmap[cc];
  native_window_t* ret = NULL;
  native_window_t* pre = NULL;
  for (; tt;) {
    if (tt->hwnd == hwnd) {
      ret = tt;
      break;
    }
    pre = tt;
    tt = tt->hash_next;
  }
  switch (opt) {
  case 1:
    if (!ret) {
      ptr->hash_next = g_hashmap[cc];
      g_hashmap[cc] = ptr;
    }
    break;
  case 2:
    if (ret) {
      if (ret == g_hashmap[cc]) {
        g_hashmap[cc] = NULL;
      }
      else {
        pre->hash_next = ret->hash_next;
      }
    }
    break;
  }
  return ret;
}
typedef widget_t widget_t;
typedef widget_t* HWID;
typedef LRESULT(CALLBACK* WIDPROC)(HWID, UINT, WPARAM, LPARAM);
typedef struct WIDCLASS {
  UINT style;
  WIDPROC lpfnWndProc;
  int cbClsExtra;
  int cbWndExtra;
  ULONG_PTR cursor; /* cursor id */
  //HBRUSH hbrBackground;
  char lpszClassName[32];
} WIDCLASS;
struct widget_t {
  WIDCLASS* wc;
  HWID parent;
  HWID owner;
  HWID next;
  HWID prev;
  HWID child_first;
  HWID child_last;
  int nchild;
  int id;
  void* pScroll;
  native_window_t* handle;
  DWORD dwStyle;
  DWORD dwExStyle;
  RECT rect;
  char* text;
  void* userdata;
  DWORD wExtra[1];
};
typedef struct DRAWITEM_T {
  UINT CtlType;
  UINT CtlID;
  UINT itemID;
  UINT itemAction;
  UINT itemState;
  HWID hwndItem;
  HDC hDC;
  RECT rcItem;
  ULONG_PTR itemData;
} DRAWITEM_T;
typedef struct COMPAREITEM_T {
  UINT CtlType;
  UINT CtlID;
  HWID hwndItem;
  UINT itemID1;
  ULONG_PTR itemData1;
  UINT itemID2;
  ULONG_PTR itemData2;
  DWORD dwLocaleId;
} COMPAREITEM_T;
typedef struct DELETEITEM_T {
  UINT CtlType;
  UINT CtlID;
  UINT itemID;
  HWID hwndItem;
  ULONG_PTR itemData;
} DELETEITEM_T;
#define GetWidgetClass(lpszClassName) OptRegisterWidget(0, lpszClassName, NULL)
#define RegisterWidget(lpszClassName, wc) OptRegisterWidget(1, lpszClassName, wc)
#define UnRegisterWidget(lpszClassName) OptRegisterWidget(2, lpszClassName, NULL)
WIDCLASS* OptRegisterWidget(int opt, const char* lpszClassName, WIDCLASS* wc)
{
  static WIDCLASS g_wc[256] = {0};
  static int nwc = 0;
  int i;
  WIDCLASS* wc1 = NULL;
  for (i = 0; i < nwc; ++i) {
    if (0 == stricmp(g_wc[i].lpszClassName, lpszClassName)) {
      wc1 = g_wc + i;
    }
  }
  switch (opt) {
  case 1:
    if (NULL == wc1) {
      ++nwc;
      wc1 = g_wc + nwc - 1;
    }
    *wc1 = *wc;
    strncpy(wc1->lpszClassName, lpszClassName, 32);
    break;
  case 2:
    if (wc1) {
      --nwc;
      if (nwc > 0) {
        *wc1 = g_wc[nwc - 1];
      }
    }
    break;
  }
  return wc1;
}
WIDCLASS* RegisterWidgetClass(const char* lpszClassName, UINT style, WIDPROC lpfnWndProc, int cbClsExtra, int cbWndExtra, ULONG_PTR cursor)
{
  WIDCLASS wc[1] = {0};
  wc->style = style;
  wc->lpfnWndProc = lpfnWndProc;
  wc->cbClsExtra = cbClsExtra;
  wc->cbWndExtra = cbWndExtra;
  wc->cursor = cursor;
  return RegisterWidget(lpszClassName, wc);
}
typedef void* drawable_type;
struct caret {
  int x;
  int y;
  unsigned w;
  unsigned h;
  bool visible;
};
typedef struct map_thread map_thread;
struct map_thread {
  IRECT update_area;
  bool ignore_update_area;
  bool forced;
};
typedef struct font_tag font_tag;
struct font_tag {
  char name[32];
  unsigned height;
  unsigned weight;
  bool italic;
  bool underline;
  bool strikeout;
#if defined(NANA_WINDOWS)
  HFONT handle;
#elif defined(NANA_X11)
#if defined(NANA_USE_XFT)
  XftFont* handle;
#else
  XFontSet handle;
#endif
#endif
};
typedef struct pen_spec pen_spec;
struct pen_spec {
  //HPEN handle;
  unsigned color;
  int style;
  int width;
  //void set(HDC context, int style, int width,unsigned color);
};
enum {brush_Solid, brush_HatchBDiagonal};
typedef struct brush_spec brush_spec;
struct brush_spec {
  //HBRUSH handle;
  int style;
  unsigned color;
  //void set(HDC context, t style, unsigned color);
};
typedef struct round_region_spec round_region_spec;
struct round_region_spec {
  //HRGN handle;
  IRECT r;
  unsigned radius_x;
  unsigned radius_y;
};
typedef struct string_spec string_spec;
struct string_spec {
  unsigned tab_length;
  unsigned tab_pixels;
  unsigned whitespace_pixels;
};
struct graphics_t {
  void* dwptr_;
  //font font_shadow_;
  drawable_type handle_;
  ISIZE size_;
  //pixel_buffer pxbuf_;
  //bool changed_;
  unsigned color_;//{ 0xffffffff };
  unsigned text_color_;//{0xffffffff};
  uint32* pixbuf_ptr;
  size_t bytes_per_line;
  font_tag* font;
  pen_spec pen;
  brush_spec brush;
  round_region_spec round_region;
  string_spec string;
#if defined(NANA_WINDOWS)
  HDC context;
  HBITMAP pixmap;
#elif defined(NANA_X11)
  Pixmap pixmap;
  GC context;
  IPOINT line_begin_pos;
#if defined(NANA_USE_XFT)
  XftDraw* xftdraw;
  XftColor xft_fgcolor;
#endif
#endif
};
struct image_t {
  bool alpha_channel;
  ISIZE sz;
  int (*close)(); //The destructor is defined in ../image.cpp
  int (*openfile)(const char* file);
  int (*openmem)(const void* data, size_t bytes); // reads image from memory
  int (*paste)(const IRECT* src_r, graphics_t* dst, int x_dst, int y_dst);
  int (*stretch)(const IRECT* src_r, graphics_t* dst, const IRECT* r);
};
int get_wheel_scroll(int* plines, int* pchars)
{
  int lines = 3, chars = 3;
#if defined(NANA_WINDOWS)
  // https://msdn.microsoft.com/en-us/library/ms997498.aspx
  //#define SPI_SETWHEELSCROLLCHARS 0x006D
# ifndef SPI_GETWHEELSCROLLCHARS
# define SPI_GETWHEELSCROLLCHARS 0x006C
# endif
  SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &lines, 0);
  SystemParametersInfo(SPI_GETWHEELSCROLLCHARS, 0, &chars, 0);
#endif
  if (plines) {
    *plines = lines;
  }
  if (pchars) {
    *pchars = chars;
  }
  return 0;
}
void close_window(native_window_type wd);
bool calc_screen_point(native_window_type wd, IPOINT* pos);
void capture_window(native_window_type wd, bool cap);
ISIZE primary_monitor_size();
IRECT make_center(int w, int h)
{
  ISIZE screen = primary_monitor_size();
  return iRECT2((w > screen.w ? 0 : (screen.w - w) >> 1), (h > screen.h ? 0 : (screen.h - h) >> 1), w, h);
}
#if defined(NANA_WINDOWS)
#include "window_win32.inl"
#elif defined(NANA_X11)
#include "window_x11.inl"
#endif
ISIZE primary_monitor_size()
{
#if defined(NANA_WINDOWS)
  return iSIZE(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
#elif defined(NANA_X11)
  Screen* s = XScreenOfDisplay(open_display(), XDefaultScreen(open_display()));
  return iSIZE(XWidthOfScreen(s), XHeightOfScreen(s));
#endif
}
void get_window_rect(native_window_type wd, IRECT* r)
{
#if defined(NANA_WINDOWS)
  GetWindowRect((wd), (RECT*)r);
#elif defined(NANA_X11)
  Window root;
  int x, y;
  unsigned int w, h;
  unsigned border, depth;
  XGetGeometry(open_display(), (wd), &root, &x, &y, &w, &h, &border, &depth);
  *r = iRECT2(x, y, w, h);
#endif
}
//struct native_interface
void affinity_execute(native_window_type native_handle, void (*fn)())
{
  if (!fn) {
    return;
  }
#if defined(NANA_WINDOWS)
  {
    HWND mswin = (native_handle);
    if (IsWindow(mswin)) {
      if (GetCurrentThreadId() != GetWindowThreadProcessId(mswin, NULL)) {
        SendMessage(mswin, msg_affinity_execute, (WPARAM)(fn), 0);
        return;
      }
    }
    fn();
  }
#else
  fn();
#endif
}
IRECT screen_area_from_point(int x, int y)
{
#if defined(NANA_WINDOWS)
  typedef HMONITOR(__stdcall * MonitorFromPointT)(POINT, DWORD);
  MonitorFromPointT mfp = (MonitorFromPointT)(GetProcAddress(GetModuleHandleA("User32.DLL"), "MonitorFromPoint"));
  if (mfp) {
    POINT native_pos = {x, y};
    HMONITOR monitor = mfp(native_pos, 2 /*MONITOR_DEFAULTTONEAREST*/);
    MONITORINFO mi;
    mi.cbSize = sizeof mi;
    if (GetMonitorInfo(monitor, &mi)) {
      return iRECT(mi.rcWork.left, mi.rcWork.top, mi.rcWork.right, mi.rcWork.bottom);
    }
  }
#else
  //(void)(pos); //eliminate unused parameter compiler warning.
#endif
  return iRECT_sz(primary_monitor_size());
}
native_window_type create_child_window(native_window_type parent, IRECT r)
{
  if (NULL == parent) {
    return NULL;
  }
  else {
#if defined(NANA_WINDOWS)
    HWND handle = CreateWindowEx(WS_EX_CONTROLPARENT, // Extended possibilites for variation
        "NanaWindowInternal",
        "Nana Child Window", // Title Text
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS,
        r.l, r.t, RCW(&r), RCH(&r),
        (HWND)(parent), // The window is a child-window to desktop
        0, GetModuleHandle(0), 0);
#elif defined(NANA_X11)
    XSetWindowAttributes win_attr;
    unsigned long attr_mask = CWBackPixmap | CWBackPixel | CWBorderPixel |
        CWWinGravity | CWBitGravity | CWColormap | CWEventMask;
    Display* disp = open_display();
    win_attr.colormap = colormap();
    win_attr.background_pixmap = None;
    win_attr.background_pixel = 0xFFFFFF;
    win_attr.border_pixmap = None;
    win_attr.border_pixel = 0x0;
    win_attr.bit_gravity = 0;
    win_attr.win_gravity = NorthWestGravity;
    win_attr.backing_store = 0;
    win_attr.backing_planes = 0;
    win_attr.backing_pixel = 0;
    win_attr.colormap = colormap();
    win_attr.override_redirect = True;
    attr_mask |= CWOverrideRedirect;
    IPOINT pos[1] = {r.l, r.t};
    win_attr.event_mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask;
    Window handle = XCreateWindow(disp, (Window)(parent),
        pos->x, pos->y, (RCW(&r) ? RCW(&r) : 1), (RCH(&r) ? RCH(&r) : 1), 0,
        screen_depth(), InputOutput, screen_visual(),
        attr_mask, &win_attr);
    if (handle) {
      XTextProperty name;
      char text[] = "Nana Child Window";
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
      hints.x = pos->x;
      hints.y = pos->y;
      hints.min_width = hints.max_width = RCW(&r);
      hints.min_height = hints.max_height = RCH(&r);
      hints.flags |= (PMinSize | PMaxSize);
      XSetWMNormalHints(disp, handle, &hints);
      XChangeProperty(disp, handle, ab->motif_wm_hints, ab->motif_wm_hints, 32, PropModeReplace,
          (unsigned char*)(&motif), sizeof(motif) / sizeof(long));
      XChangeProperty(disp, handle, ab->net_wm_state, XA_ATOM, 32, PropModeAppend,
          (unsigned char*)((Atom*)(&ab->net_wm_state_skip_taskbar)), 1);
    }
#endif
    return (native_window_type)(handle);
  }
}
bool is_window_visible(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  return (FALSE != IsWindowVisible((HWND)(wd)));
#elif defined(NANA_X11)
  XWindowAttributes attr;
  set_error_handler();
  XGetWindowAttributes(open_display(), (Window)(wd), &attr);
  return (BadWindow != rev_error_handler() &&
      attr.map_state != IsUnmapped);
#endif
}
#if defined(NANA_X11)
void set_modal(native_window_type wd)
{
  Window owner = (Window)(get_owner(wd));
  if (wd && owner) {
    if (is_window_visible(wd)) {
      show_window(wd, false, true);
    }
    Display* disp = open_display();
    const atombase_tag* ab = atombase();
    XSetTransientForHint(disp, (Window)(wd), owner);
    XChangeProperty(disp, (Window)(wd),
        ab->net_wm_state, XA_ATOM, sizeof(int) * 8,
        PropModeReplace,
        (const unsigned char*)(&ab->net_wm_state_modal), 1);
  }
}
#endif
void enable_dropfiles(native_window_type wd, bool enb)
{
#if defined(NANA_WINDOWS)
  DragAcceptFiles((HWND)(wd), enb);
#else
  int dndver = (enb ? 4 : 0);
  XChangeProperty(open_display(), (Window)(wd), atombase()->xdnd_aware, XA_ATOM, sizeof(int) * 8,
      PropModeReplace, (unsigned char*)(&dndver), 1);
#endif
}
void enable_window(native_window_type wd, bool is_enabled)
{
#if defined(NANA_WINDOWS)
  EnableWindow((HWND)(wd), is_enabled);
#else
  int mask = ExposureMask | StructureNotifyMask;
  if (is_enabled) {
    mask |= (ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    mask |= (KeyPressMask | KeyReleaseMask);
    mask |= (EnterWindowMask | LeaveWindowMask | FocusChangeMask);
  }
  XSelectInput(open_display(), (Window)(wd), mask);
#endif
}
#if 0
bool window_icon(native_window_type wd, const image* sml_icon, const image* big_icon)
{
#if defined(NANA_WINDOWS)
  HICON sml_handle = image_geticon(sml_icon);
  HICON big_handle = image_geticon(big_icon);
  if (sml_handle || big_handle) {
    keep_window_icon(wd, sml_icon, big_icon);
    if (sml_handle) {
      SendMessage((HWND)(wd), WM_SETICON, ICON_SMALL, (WPARAM)(sml_handle));
    }
    if (big_handle) {
      SendMessage((HWND)(wd), WM_SETICON, ICON_BIG, (LPARAM)(big_handle));
    }
    return true;
  }
#elif defined(NANA_X11)
  if (wd && (!sml_icon.empty() || !big_icon.empty())) {
    auto& img = (sml_icon.empty() ? big_icon : sml_icon);
    const paint graphics& graph = keep_window_icon(wd, img);
    XWMHints hints;
    hints.flags = IconPixmapHint;
    hints.icon_pixmap = graph.handle()->pixmap;
    XSetWMHints(open_display(), (Window)(wd), &hints);
    return true;
  }
#endif
  return false;
}
void activate_owner(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  activate_window((native_window_type)(
      GetWindow((HWND)(wd), GW_OWNER)
      ));
#else
  (void)(wd); //eliminate unused parameter compiler warning.
#endif
}
void activate_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  auto native_wd = (HWND)(wd);
  if (IsWindow(native_wd)) {
    if (GetWindowThreadProcessId(native_wd, NULL) == GetCurrentThreadId()) {
      EnableWindow(native_wd, true);
      SetActiveWindow(native_wd);
      SetForegroundWindow(native_wd);
    }
    else {
      PostMessage(native_wd, msg_async_activate, 0, 0);
    }
  }
#else
  (void)(wd); //eliminate unused parameter compiler warning.
#endif
}
#endif
void restore_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  msw_show_window((HWND)(wd), SW_RESTORE);
#elif defined(NANA_X11)
  //Restore the window by removing NET_WM_STATE_MAXIMIZED_HORZ,
  //_NET_WM_STATE_MAXIMIZED_VERT and _NET_WM_STATE_FULLSCREEN.
  Display* disp = open_display();
  Window default_root = XDefaultRootWindow(disp);
  const atombase_tag* ab = atombase();
  XEvent evt;
  evt.xclient.type = ClientMessage;
  evt.xclient.display = open_display();
  evt.xclient.message_type = ab->net_wm_state;
  evt.xclient.format = 32;
  evt.xclient.window = (Window)(wd);
  evt.xclient.data.l[0] = 0; //_NET_WM_STATE_REMOVE
  evt.xclient.data.l[1] = ab->net_wm_state_maximized_horz;
  evt.xclient.data.l[2] = ab->net_wm_state_maximized_vert;
  evt.xclient.data.l[3] = evt.xclient.data.l[4] = 0;
  XSendEvent(disp, default_root, False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
  evt.xclient.data.l[1] = ab->net_wm_state_fullscreen;
  evt.xclient.data.l[2] = 0;
  XSendEvent(disp, default_root, False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
  //Transfer the window from IconState to NormalState.
  evt.xclient.message_type = ab->wm_change_state;
  evt.xclient.data.l[0] = NormalState;
  evt.xclient.data.l[1] = 0;
  XSendEvent(disp, default_root, False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
  XMapWindow(disp, (Window)(wd));
  set_error_handler();
  XSetInputFocus(disp, (Window)(wd), RevertToPointerRoot, CurrentTime);
#endif
}
void zoom_window(native_window_type wd, bool ask_for_max)
{
#if defined(NANA_WINDOWS)
  msw_show_window((HWND)(wd), ask_for_max ? SW_MAXIMIZE : SW_MINIMIZE);
#elif defined(NANA_X11)
  Display* disp = open_display();
  if (ask_for_max) {
    const atombase_tag* ab = atombase();
    XEvent evt;
    evt.xclient.type = ClientMessage;
    evt.xclient.display = open_display();
    evt.xclient.message_type = ab->net_wm_state;
    evt.xclient.format = 32;
    evt.xclient.window = (Window)(wd);
    evt.xclient.data.l[0] = 1; //_NET_WM_STATE_ADD
    evt.xclient.data.l[1] = ab->net_wm_state_maximized_horz;
    evt.xclient.data.l[2] = ab->net_wm_state_maximized_vert;
    evt.xclient.data.l[3] = evt.xclient.data.l[4] = 0;
    XSendEvent(disp, XDefaultRootWindow(disp), False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
    XMapWindow(disp, (Window)(wd));
  }
  else {
    XIconifyWindow(disp, (Window)(wd), XDefaultScreen(disp));
  }
#endif
}
void refresh_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  InvalidateRect((HWND)(wd), NULL, true);
#elif defined(NANA_X11)
  (void)(wd); //eliminate unused parameter compiler warning.
#endif
}
bool is_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  return (FALSE != IsWindow((HWND)(wd)));
#elif defined(NANA_X11)
  XWindowAttributes attr;
  set_error_handler();
  XGetWindowAttributes(open_display(), (Window)(wd), &attr);
  return (BadWindow != rev_error_handler());
#endif
}
bool is_window_zoomed(native_window_type wd, bool ask_for_max)
{
#if defined(NANA_WINDOWS)
  return (FALSE != (ask_for_max ? IsZoomed((HWND)(wd)) : IsIconic((HWND)(wd))));
#elif defined(NANA_X11)
  bool zoomed = false;
  unsigned long n, i;
  Atom type;
  unsigned char* prop;
  int format;
  if (Success == XGetWindowProperty(open_display(), (Window)(wd), atombase()->net_wm_state, 0, 2, false, AnyPropertyType, &type, &format, &n, &i, &prop)) {
    if (32 == format) {
      if (ask_for_max) {
        if (type == XA_ATOM) {
          for (i = 0; i < n; i++) {
            if (((Atom*)prop)[i] == atombase()->net_wm_state_fullscreen) {
              zoomed = true;
              break;
            }
          }
        }
      }
      else {
        zoomed = (IconicState == *(unsigned*)(prop));
      }
    }
    XFree(prop);
  }
  return zoomed;
#endif
}
native_window_type get_parent_window(native_window_type wd)
{
#ifdef NANA_WINDOWS
  return (native_window_type)(GetParent((HWND)(wd)));
#elif defined(NANA_X11)
  Window root;
  Window parent;
  Window* children;
  unsigned size;
  if (0 != XQueryTree(open_display(), (Window)(wd), &root, &parent, &children, &size)) {
    XFree(children);
    return (native_window_type)(parent);
  }
  return NULL;
#endif
}
native_window_type set_parent_window(native_window_type child, native_window_type new_parent)
{
#ifdef NANA_WINDOWS
  HWND prev = SetParent((HWND)(child), (HWND)(new_parent));
  if (prev) {
    PostMessage(prev, /*WM_CHANGEUISTATE*/0x0127, /*UIS_INITIALIZE*/ 3, 0);
  }
  SetWindowPos((HWND)(child), NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
  return (native_window_type)(prev);
#elif defined(NANA_X11)
  native_window_type prev = NULL;
  prev = get_parent_window(child);
  XReparentWindow(open_display(), (Window)(child), (Window)(new_parent),
      0, 0);
  return prev;
#endif
}
IPOINT window_position(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  RECT r;
  HWND coord_wd;
  GetWindowRect((HWND)(wd), & r);
  coord_wd = GetWindow((HWND)(wd), GW_OWNER);
  if (!coord_wd) {
    coord_wd = GetParent((HWND)(wd));
  }
  if (coord_wd) {
    POINT pos[1] = {r.left, r.top};
    ScreenToClient(coord_wd, pos);
    return iPOINT(pos->x, pos->y);
  }
  return iPOINT(r.left, r.top);
#elif defined(NANA_X11)
  int x, y;
  Window coord_wd = (Window)(get_owner(wd));
  if (!coord_wd) {
    coord_wd = (Window)(get_parent_window(wd));
    if (!coord_wd) {
      coord_wd = root_window();
    }
  }
  Window child;
  if (True == XTranslateCoordinates(open_display(), (Window)(wd), coord_wd, 0, 0, &x, &y, &child)) {
    return iPOINT(x, y);
  }
  return iPOINT(0, 0);
#endif
}
void move_window(native_window_type wd, int x, int y)
{
#if defined(NANA_WINDOWS)
  if (GetWindowThreadProcessId((HWND)(wd), 0) != GetCurrentThreadId()) {
    move_window_t* mw = NULL;
    MYREALLOC(mw, 1);
    mw->x = x;
    mw->y = y;
    mw->ignore = move_window_Size;
    PostMessage((HWND)(wd), msg_remote_thread_move_window, (WPARAM)(mw), 0);
  }
  else {
    RECT r;
    HWND owner;
    GetWindowRect((HWND)(wd), &r);
    owner = GetWindow((HWND)(wd), GW_OWNER);
    if (owner) {
      RECT owner_rect;
      GetWindowRect(owner, &owner_rect);
      {
        POINT pos[1] = {owner_rect.left, owner_rect.top};
        ScreenToClient(owner, pos);
        x += (owner_rect.left - pos->x);
        y += (owner_rect.top - pos->y);
      }
    }
    MoveWindow((HWND)(wd), x, y, r.right - r.left, r.bottom - r.top, true);
  }
#elif defined(NANA_X11)
  Display* disp = open_display();
  Window owner = (Window)(get_owner(wd));
  if (owner) {
    Window child;
    XTranslateCoordinates(disp, owner, root_window(),
        x, y, &x, &y, &child);
  }
  XWindowAttributes attr;
  XGetWindowAttributes(disp, (Window)(wd), &attr);
  if (attr.map_state == IsUnmapped) {
    XSizeHints hints;
    hints.flags = USPosition;
    hints.x = x;
    hints.y = y;
    XSetWMNormalHints(disp, (Window)(wd), &hints);
  }
  XMoveWindow(disp, (Window)(wd), x, y);
#endif
}
void move_window2(native_window_type wd, IRECT r)
{
#if defined(NANA_WINDOWS)
  if (GetWindowThreadProcessId((HWND)(wd), 0) != GetCurrentThreadId()) {
    move_window_t* mw = NULL;
    MYREALLOC(mw, 1);
    mw->x = r.l;
    mw->y = r.t;
    mw->w = RCW(&r);
    mw->h = RCH(&r);
    mw->ignore = 0;
    PostMessage((HWND)(wd), msg_remote_thread_move_window, (WPARAM)(mw), 0);
  }
  else {
    RECT client, wd_area;
    int x = r.l, y = r.t;
    HWND owner = GetWindow((wd), GW_OWNER);
    if (owner) {
      RECT owner_rect;
      GetWindowRect(owner, &owner_rect);
      {
        POINT pos[1] = {owner_rect.left, owner_rect.top};
        ScreenToClient(owner, pos);
        x += (owner_rect.left - pos->x);
        y += (owner_rect.top - pos->y);
      }
    }
    GetClientRect((HWND)(wd), &client);
    GetWindowRect((HWND)(wd), &wd_area);
    {
      int ext_w = (wd_area.right - wd_area.left) - client.right;
      int ext_h = (wd_area.bottom - wd_area.top) - client.bottom;
      MoveWindow((HWND)(wd), x, y, RCW(&r) + ext_w, RCH(&r) + ext_h, true);
    }
  }
#elif defined(NANA_X11)
  Display* disp = open_display();
  long supplied;
  XSizeHints hints;
  XGetWMNormalHints(disp, (Window)(wd), &hints, &supplied);
  if ((hints.flags & (PMinSize | PMaxSize)) && (hints.min_width == hints.max_width) && (hints.min_height == hints.max_height)) {
    hints.flags = PMinSize | PMaxSize;
    hints.min_width = hints.max_width = RCW(&r);
    hints.min_height = hints.max_height = RCH(&r);
  }
  else {
    hints.flags = 0;
  }
  Window owner = (Window)(get_owner(wd));
  int x = r.l;
  int y = r.t;
  if (owner) {
    Window child;
    XTranslateCoordinates(disp, owner, root_window(),
        r.l, r.t, &x, &y, &child);
  }
  XWindowAttributes attr;
  XGetWindowAttributes(disp, (Window)(wd), &attr);
  if (attr.map_state == IsUnmapped) {
    hints.flags |= (USPosition | USSize);
    hints.x = x;
    hints.y = y;
    hints.width = RCW(&r);
    hints.height = RCH(&r);
  }
  if (hints.flags) {
    XSetWMNormalHints(disp, (Window)(wd), &hints);
  }
  XMoveResizeWindow(disp, (Window)(wd), x, y, RCW(&r), RCH(&r));
#endif
}
void set_window_z_order(native_window_type wd, native_window_type wd_after, int action_if_no_wd_after)
{
#if defined(NANA_WINDOWS)
  HWND wa = (HWND)(wd_after);
  if (wa == 0) {
    switch (action_if_no_wd_after) {
    case z_order_action_bottom :
      wa = HWND_BOTTOM;
      break;
    case z_order_action_top:
      wa = HWND_TOP;
      break;
    case z_order_action_topmost:
      wa = HWND_TOPMOST;
      break;
    case z_order_action_foreground:
      SetForegroundWindow((HWND)(wd));
      return;
    default:
      wa = HWND_NOTOPMOST;
    }
  }
  if (GetCurrentThreadId() != GetWindowThreadProcessId((HWND)(wd), 0)) {
    PostMessage((HWND)(wd), msg_remote_thread_set_window_pos, (WPARAM)(wa), SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
  }
  else {
    SetWindowPos((HWND)(wd), wa, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
  }
#elif defined(NANA_X11)
  Display* disp = open_display();
  if (0 == wd_after) {
    switch (action_if_no_wd_after) {
    case z_order_action_bottom:
      XLowerWindow(disp, (Window)(wd));
      break;
    case z_order_action_foreground:
    case z_order_action_top:
    case z_order_action_topmost:
      XRaiseWindow(disp, (Window)(wd));
      break;
    default: //z_order_action_none
      break;
    }
  }
  else {
    //If the handle wd is a top level, XConfigureWindow() will be failed with a BadMatch.
    //The fix is to use XReconfigureWMWindow() instead.
    XWindowChanges values;
    values.sibling = (Window)(wd_after);
    values.stack_mode = Below;
    XReconfigureWMWindow(disp, (Window)(wd), XDefaultScreen(disp), CWSibling | CWStackMode, &values);
  }
#endif
}
void bring_top(native_window_type wd, bool activated)
{
#if defined(NANA_WINDOWS)
  HWND native_wd = (HWND)(wd);
  if (FALSE == IsWindow(native_wd)) {
    return;
  }
  else {
    HWND fg_wd = GetForegroundWindow();
    DWORD fg_tid = GetWindowThreadProcessId(fg_wd, NULL);
    AttachThreadInput(GetCurrentThreadId(), fg_tid, TRUE);
    ShowWindow(native_wd, activated ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE);
    SetWindowPos(native_wd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    SetWindowPos(native_wd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    AttachThreadInput(GetCurrentThreadId(), fg_tid, FALSE);
  }
#else
  (void)(activated); //eliminate unused parameter compiler warning.
  set_window_z_order(wd, NULL, z_order_action_top);
#endif
}
void window_size(native_window_type wd, int w, int h)
{
#if defined(NANA_WINDOWS)
  if (GetWindowThreadProcessId((HWND)(wd), 0) != GetCurrentThreadId()) {
    move_window_t* mw = NULL;
    MYREALLOC(mw, 1);
    mw->w = w;
    mw->h = h;
    mw->ignore = move_window_Pos;
    PostMessage((HWND)(wd), msg_remote_thread_move_window, (WPARAM)(mw), 0);
  }
  else {
    RECT r;
    HWND owner, parent;
    GetWindowRect((HWND)(wd), &r);
    owner = GetWindow((HWND)(wd), GW_OWNER);
    parent = GetParent((HWND)(wd));
    if (parent && (parent != owner)) {
      POINT pos[1] = {r.left, r.top};
      ScreenToClient(parent, pos);
      r.left = pos->x;
      r.top = pos->y;
    }
    MoveWindow((HWND)(wd), r.left, r.top, (int)(w), (int)(h), true);
  }
#elif defined(NANA_X11)
  Display* disp = open_display();
  //Check the XSizeHints for testing whether the window is sizable.
  XSizeHints hints;
  long supplied;
  XGetWMNormalHints(disp, (Window)(wd), &hints, &supplied);
  if ((hints.flags & (PMinSize | PMaxSize)) && (hints.min_width == hints.max_width) && (hints.min_height == hints.max_height)) {
    hints.flags = PMinSize | PMaxSize;
    hints.min_width = hints.max_width = w;
    hints.min_height = hints.max_height = h;
    XSetWMNormalHints(disp, (Window)(wd), &hints);
  }
  XResizeWindow(disp, (Window)(wd), w, h);
#endif
}
void set_window_caption(native_window_type wd, const char* title)
{
#if defined(NANA_WINDOWS)
  if (GetCurrentThreadId() != GetWindowThreadProcessId((HWND)(wd), 0)) {
    char* str = strdup(title);
    PostMessage((HWND)(wd), msg_remote_thread_set_window_text, (WPARAM)(str), 0);
  }
  else {
    SetWindowTextA((HWND)(wd), title);
  }
#elif defined(NANA_X11)
  XTextProperty name;
  char* text = (char*)(title);
  XStringListToTextProperty(&text, 1, &name);
  XSetWMName(open_display(), (Window)(wd), &name);
  XChangeProperty(open_display(), (Window)(wd),
      atombase()->net_wm_name, atombase()->utf8_string, 8,
      PropModeReplace, (unsigned char*)(text), strlen(title));
#endif
}
int get_window_caption(native_window_type wd, char* title, int maxlen)
{
  int length = 0;
#if defined(NANA_WINDOWS)
  WD_MANAGER_INTERNAL_LOCK();
  if (title) {
    length = GetWindowTextA((HWND)(wd), title, maxlen);
  }
  else {
    length = GetWindowTextLengthA((HWND)(wd));
  }
  return length;
  WD_MANAGER_INTERNAL_UNLOCK();
#elif defined(NANA_X11)
  platform_scope_guard_psg();
  XTextProperty txtpro;
  if (XGetWMName(open_display(), (Window)(wd), &txtpro)) {
    char** strlist;
    int size;
    if (XTextPropertyToStringList(&txtpro, &strlist, &size)) {
      if (size > 1) {
        strncpy(title, *strlist, maxlen);
        XFreeStringList(strlist);
      }
    }
  }
#endif
  return length;
}
void capture_window(native_window_type wd, bool cap)
{
#if defined(NANA_WINDOWS)
  if (cap) {
    SetCapture((HWND)(wd));
  }
  else {
    ReleaseCapture();
  }
#elif defined(NANA_X11)
  if (cap) {
    const unsigned mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask;
    if (GrabNotViewable == XGrabPointer(open_display(), (Window)(wd), false, mask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime)) {
      grab((Window)(wd));
    }
  }
  else {
    XUngrabPointer(open_display(), CurrentTime);
    XFlush(open_display());
    grab(0);
  }
#endif
}
IPOINT cursor_position()
{
#if defined(NANA_WINDOWS)
  POINT point;
  GetCursorPos(&point);
  return iPOINT(point.x, point.y);
#elif defined(NANA_X11)
  IPOINT pos = {0};
  Window drop_wd;
  int x, y;
  unsigned mask;
  XQueryPointer(open_display(), root_window(), &drop_wd, &drop_wd, &pos.x, &pos.y, &x, &y, &mask);
  return pos;
#endif
}
native_window_type get_owner_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  return (native_window_type)(GetWindow((HWND)(wd), GW_OWNER));
#elif defined(NANA_X11)
  return get_owner(wd);
#endif
}
void caret_create(native_window_type wd, int w, int h)
{
#if defined(NANA_WINDOWS)
  CreateCaret((HWND)(wd), 0, w, h);
#elif defined(NANA_X11)
  //caret_open(wd, w, h);
#endif
}
void caret_destroy(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  if (GetCurrentThreadId() != GetWindowThreadProcessId((HWND)(wd), 0)) {
    PostMessage((HWND)(wd), msg_operate_caret, 1, 0);
  }
  else {
    DestroyCaret();
  }
#elif defined(NANA_X11)
  //caret_close(wd);
#endif
}
void caret_pos(native_window_type wd, int x, int y)
{
#if defined(NANA_WINDOWS)
  if (GetCurrentThreadId() != GetWindowThreadProcessId((HWND)(wd), 0)) {
    IPOINT* cp = NULL;
    MYREALLOC(cp, 1);
    cp->x = x;
    cp->y = y;
    PostMessage((HWND)(wd), msg_operate_caret, 2, (LPARAM)(cp));
  }
  else {
    SetCaretPos(x, y);
  }
#elif defined(NANA_X11)
  //caret_pos(wd, pos);
#endif
}
void caret_visible(native_window_type wd, bool vis)
{
#if defined(NANA_WINDOWS)
  (vis ? ShowCaret : HideCaret)((HWND)(wd));
#elif defined(NANA_X11)
  //caret_visible(wd, vis);
#endif
}
void set_focus(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  if (wd && (GetFocus() != (HWND)(wd))) {
    if (GetCurrentThreadId() != GetWindowThreadProcessId((HWND)(wd), NULL)) {
      PostMessage((HWND)(wd), msg_async_set_focus, 0, 0);
    }
    else {
      SetFocus((HWND)(wd));
    }
  }
#elif defined(NANA_X11)
  XWindowAttributes attr;
  XGetWindowAttributes(open_display(), (Window)(wd), &attr);
  //Make sure the window is mapped before setting focus.
  if (IsViewable == attr.map_state) {
    XSetInputFocus(open_display(), (Window)(wd), RevertToPointerRoot, CurrentTime);
  }
#endif
}
native_window_type get_focus_window()
{
#if defined(NANA_WINDOWS)
  return (native_window_type)(GetFocus());
#elif defined(NANA_X11)
  Window wd;
  int revert;
  XGetInputFocus(open_display(), &wd, &revert);
  return (native_window_type)(wd);
#endif
}
bool calc_screen_point(native_window_type wd, IPOINT* pos)
{
#if defined(NANA_WINDOWS)
  POINT point = {pos->x, pos->y};
  if (ClientToScreen((wd), &point)) {
    pos->x = point.x;
    pos->y = point.y;
    return true;
  }
  return false;
#elif defined(NANA_X11)
  int x = pos->x, y = pos->y;
  Window child;
  return (True == XTranslateCoordinates(open_display(),
      (Window)(wd), root_window(), x, y, &pos->x, &pos->y, &child));
#endif
}
bool calc_window_point(native_window_type wd, IPOINT* pos)
{
#if defined(NANA_WINDOWS)
  POINT point = {pos->x, pos->y};
  if (ScreenToClient((HWND)(wd), &point)) {
    pos->x = point.x;
    pos->y = point.y;
    return true;
  }
  return false;
#elif defined(NANA_X11)
  int x = pos->x, y = pos->y;
  Window child;
  return (True == XTranslateCoordinates(open_display(),
      root_window(), (Window)(wd), x, y, &pos->x, &pos->y, &child));
#endif
}
void close_window(native_window_type wd)
{
#if defined(NANA_WINDOWS)
  if (wd && (DestroyWindow((wd)) == false)) {
    //DestroyWindow would be failed if the calling thread is not the window thread
    //It should send a WM_DESTROY message into window thread for destroying window
    if (GetLastError() == ERROR_ACCESS_DENIED) {
      PostMessage((wd), msg_remote_thread_destroy_window, 0, 0);
    }
  }
#elif defined(NANA_X11)
  //Under X, XDestroyWindow destroys the specified window and generats a DestroyNotify
  //event, when the client receives the event, the specified window has been already
  //destroyed. This is a feature which is different from Windows. So the following
  //works should be handled before calling XDestroyWindow.
  Display* disp = open_display();
#if 0
  auto& brock = bedrock::instance();
  if (wd == brock.get_menu()) {
    brock.erase_menu(false);
    brock.delay_restore(3); //Restores if delay_restore is not decleard
  }
  restrict::spec.remove(wd);
  auto iwd = brock.wd_manager().root(wd);
  if (iwd) {
    {
      //Before calling window_manager::destroy, make sure the window is invisible.
      //It is a behavior like Windows.
      platform_scope_guard lock;
      restrict::spec.set_error_handler();
      ::XUnmapWindow(disp, (Window)(wd));
      ::XFlush(disp);
      restrict::spec.rev_error_handler();
    }
    brock.wd_manager().destroy(iwd);
    brock.manage_form_loader(iwd, false);
    brock.wd_manager().destroy_handle(iwd);
  }
  platform_scope_guard_psg();
  restrict::spec.set_error_handler();
#endif
  ::XDestroyWindow(disp, (Window)(wd));
  //rev_error_handler();
#endif
}
native_window_type find_window(int x, int y)
{
#if defined(NANA_WINDOWS)
  POINT pos = {x, y};
  return (native_window_type)(WindowFromPoint(pos));
#elif defined(NANA_X11)
  Window root = root_window();
  Window wd = root;
  Window child = 0;
  int dropx = 0, dropy = 0;
  while (True == XTranslateCoordinates(open_display(), root, wd, x, y, &dropx, &dropy, &child)) {
    if (0 == child) {
      break;
    }
    wd = child;
  }
  return (native_window_type)(wd);
#endif
}
ISIZE check_track_size(ISIZE sz, unsigned ext_width, unsigned ext_height, bool true_for_max)
{
#if defined(NANA_WINDOWS)
  int x, y;
  if (true_for_max) {
    x = GetSystemMetrics(SM_CXMAXTRACK);
    y = GetSystemMetrics(SM_CYMAXTRACK);
    if ((unsigned)(x) < sz.w + ext_width) {
      sz.w = (unsigned)(x);
    }
    if ((unsigned)(y) < sz.h + ext_height) {
      sz.h = (unsigned)(y);
    }
  }
  else {
    x = GetSystemMetrics(SM_CXMINTRACK);
    y = GetSystemMetrics(SM_CYMINTRACK);
    if ((unsigned)(x) > sz.w + ext_width) {
      sz.w = (unsigned)(x);
    }
    if ((unsigned)(y) > sz.h + ext_height) {
      sz.h = (unsigned)(y);
    }
  }
#else
  //eliminate unused parameter compiler warning.
  (void)(ext_width);
  (void)(ext_height);
  (void)(true_for_max);
#endif
  return sz;
}

