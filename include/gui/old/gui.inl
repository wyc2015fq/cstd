
#ifndef _GUI_INL_
#define _GUI_INL_
typedef enum E_DRIVER_TYPE {
  EDT_NULL,
  EDT_SOFTWARE,
  EDT_BURNINGSVIDEO,
  EDT_DIRECT3D8,
  EDT_DIRECT3D9,
  EDT_OPENGL,
  EDT_COUNT
} E_DRIVER_TYPE;
#define EDCLEAR_TARGET   0x0001
#define EDCLEAR_ZBUFFER  0x0002
#define EDCLEAR_STENCIL  0x0004
typedef enum EKEY_CODE {
  KEY_LBUTTON          = 0x01,  // Left mouse button
  KEY_RBUTTON          = 0x02,  // Right mouse button
  KEY_CANCEL           = 0x03,  // Control-break processing
  KEY_MBUTTON          = 0x04,  // Middle mouse button (three-button mouse)
  KEY_XBUTTON1         = 0x05,  // Windows 2000/XP: X1 mouse button
  KEY_XBUTTON2         = 0x06,  // Windows 2000/XP: X2 mouse button
  KEY_BACK             = 0x08,  // BACKSPACE key
  KEY_TAB              = 0x09,  // TAB key
  KEY_CLEAR            = 0x0C,  // CLEAR key
  KEY_RETURN           = 0x0D,  // ENTER key
  KEY_SHIFT            = 0x10,  // SHIFT key
  KEY_CONTROL          = 0x11,  // CTRL key
  KEY_MENU             = 0x12,  // ALT key
  KEY_PAUSE            = 0x13,  // PAUSE key
  KEY_CAPITAL          = 0x14,  // CAPS LOCK key
  KEY_KANA             = 0x15,  // IME Kana mode
  KEY_HANGUEL          = 0x15,  // IME Hanguel mode (maintained for compatibility use KEY_HANGUL)
  KEY_HANGUL           = 0x15,  // IME Hangul mode
  KEY_JUNJA            = 0x17,  // IME Junja mode
  KEY_FINAL            = 0x18,  // IME final mode
  KEY_HANJA            = 0x19,  // IME Hanja mode
  KEY_KANJI            = 0x19,  // IME Kanji mode
  KEY_ESCAPE           = 0x1B,  // ESC key
  KEY_CONVERT          = 0x1C,  // IME convert
  KEY_NONCONVERT       = 0x1D,  // IME nonconvert
  KEY_ACCEPT           = 0x1E,  // IME accept
  KEY_MODECHANGE       = 0x1F,  // IME mode change request
  KEY_SPACE            = 0x20,  // SPACEBAR
  KEY_PRIOR            = 0x21,  // PAGE UP key
  KEY_NEXT             = 0x22,  // PAGE DOWN key
  KEY_END              = 0x23,  // END key
  KEY_HOME             = 0x24,  // HOME key
  KEY_LEFT             = 0x25,  // LEFT ARROW key
  KEY_UP               = 0x26,  // UP ARROW key
  KEY_RIGHT            = 0x27,  // RIGHT ARROW key
  KEY_DOWN             = 0x28,  // DOWN ARROW key
  KEY_SELECT           = 0x29,  // SELECT key
  KEY_PRINT            = 0x2A,  // PRINT key
  KEY_EXECUT           = 0x2B,  // EXECUTE key
  KEY_SNAPSHOT         = 0x2C,  // PRINT SCREEN key
  KEY_INSERT           = 0x2D,  // INS key
  KEY_DELETE           = 0x2E,  // DEL key
  KEY_HELP             = 0x2F,  // HELP key
  KEY_KEY_0            = 0x30,  // 0 key
  KEY_KEY_1            = 0x31,  // 1 key
  KEY_KEY_2            = 0x32,  // 2 key
  KEY_KEY_3            = 0x33,  // 3 key
  KEY_KEY_4            = 0x34,  // 4 key
  KEY_KEY_5            = 0x35,  // 5 key
  KEY_KEY_6            = 0x36,  // 6 key
  KEY_KEY_7            = 0x37,  // 7 key
  KEY_KEY_8            = 0x38,  // 8 key
  KEY_KEY_9            = 0x39,  // 9 key
  KEY_KEY_A            = 0x41,  // A key
  KEY_KEY_B            = 0x42,  // B key
  KEY_KEY_C            = 0x43,  // C key
  KEY_KEY_D            = 0x44,  // D key
  KEY_KEY_E            = 0x45,  // E key
  KEY_KEY_F            = 0x46,  // F key
  KEY_KEY_G            = 0x47,  // G key
  KEY_KEY_H            = 0x48,  // H key
  KEY_KEY_I            = 0x49,  // I key
  KEY_KEY_J            = 0x4A,  // J key
  KEY_KEY_K            = 0x4B,  // K key
  KEY_KEY_L            = 0x4C,  // L key
  KEY_KEY_M            = 0x4D,  // M key
  KEY_KEY_N            = 0x4E,  // N key
  KEY_KEY_O            = 0x4F,  // O key
  KEY_KEY_P            = 0x50,  // P key
  KEY_KEY_Q            = 0x51,  // Q key
  KEY_KEY_R            = 0x52,  // R key
  KEY_KEY_S            = 0x53,  // S key
  KEY_KEY_T            = 0x54,  // T key
  KEY_KEY_U            = 0x55,  // U key
  KEY_KEY_V            = 0x56,  // V key
  KEY_KEY_W            = 0x57,  // W key
  KEY_KEY_X            = 0x58,  // X key
  KEY_KEY_Y            = 0x59,  // Y key
  KEY_KEY_Z            = 0x5A,  // Z key
  KEY_LWIN             = 0x5B,  // Left Windows key (Microsoft?Natural?keyboard)
  KEY_RWIN             = 0x5C,  // Right Windows key (Natural keyboard)
  KEY_APPS             = 0x5D,  // Applications key (Natural keyboard)
  KEY_SLEEP            = 0x5F,  // Computer Sleep key
  KEY_NUMPAD0          = 0x60,  // Numeric keypad 0 key
  KEY_NUMPAD1          = 0x61,  // Numeric keypad 1 key
  KEY_NUMPAD2          = 0x62,  // Numeric keypad 2 key
  KEY_NUMPAD3          = 0x63,  // Numeric keypad 3 key
  KEY_NUMPAD4          = 0x64,  // Numeric keypad 4 key
  KEY_NUMPAD5          = 0x65,  // Numeric keypad 5 key
  KEY_NUMPAD6          = 0x66,  // Numeric keypad 6 key
  KEY_NUMPAD7          = 0x67,  // Numeric keypad 7 key
  KEY_NUMPAD8          = 0x68,  // Numeric keypad 8 key
  KEY_NUMPAD9          = 0x69,  // Numeric keypad 9 key
  KEY_MULTIPLY         = 0x6A,  // Multiply key
  KEY_ADD              = 0x6B,  // Add key
  KEY_SEPARATOR        = 0x6C,  // Separator key
  KEY_SUBTRACT         = 0x6D,  // Subtract key
  KEY_DECIMAL          = 0x6E,  // Decimal key
  KEY_DIVIDE           = 0x6F,  // Divide key
  KEY_F1               = 0x70,  // F1 key
  KEY_F2               = 0x71,  // F2 key
  KEY_F3               = 0x72,  // F3 key
  KEY_F4               = 0x73,  // F4 key
  KEY_F5               = 0x74,  // F5 key
  KEY_F6               = 0x75,  // F6 key
  KEY_F7               = 0x76,  // F7 key
  KEY_F8               = 0x77,  // F8 key
  KEY_F9               = 0x78,  // F9 key
  KEY_F10              = 0x79,  // F10 key
  KEY_F11              = 0x7A,  // F11 key
  KEY_F12              = 0x7B,  // F12 key
  KEY_F13              = 0x7C,  // F13 key
  KEY_F14              = 0x7D,  // F14 key
  KEY_F15              = 0x7E,  // F15 key
  KEY_F16              = 0x7F,  // F16 key
  KEY_F17              = 0x80,  // F17 key
  KEY_F18              = 0x81,  // F18 key
  KEY_F19              = 0x82,  // F19 key
  KEY_F20              = 0x83,  // F20 key
  KEY_F21              = 0x84,  // F21 key
  KEY_F22              = 0x85,  // F22 key
  KEY_F23              = 0x86,  // F23 key
  KEY_F24              = 0x87,  // F24 key
  KEY_NUMLOCK          = 0x90,  // NUM LOCK key
  KEY_SCROLL           = 0x91,  // SCROLL LOCK key
  KEY_LSHIFT           = 0xA0,  // Left SHIFT key
  KEY_RSHIFT           = 0xA1,  // Right SHIFT key
  KEY_LCONTROL         = 0xA2,  // Left CONTROL key
  KEY_RCONTROL         = 0xA3,  // Right CONTROL key
  KEY_LMENU            = 0xA4,  // Left MENU key
  KEY_RMENU            = 0xA5,  // Right MENU key
  KEY_OEM_1            = 0xBA,  // for US    ";:"
  KEY_PLUS             = 0xBB,  // Plus Key   "+"
  KEY_COMMA            = 0xBC,  // Comma Key  ","
  KEY_MINUS            = 0xBD,  // Minus Key  "-"
  KEY_PERIOD           = 0xBE,  // Period Key "."
  KEY_OEM_2            = 0xBF,  // for US    "/?"
  KEY_OEM_3            = 0xC0,  // for US    "`~"
  KEY_OEM_4            = 0xDB,  // for US    "[{"
  KEY_OEM_5            = 0xDC,  // for US    "\|"
  KEY_OEM_6            = 0xDD,  // for US    "]}"
  KEY_OEM_7            = 0xDE,  // for US    "'""
  KEY_OEM_8            = 0xDF,  // None
  KEY_OEM_AX           = 0xE1,  // for Japan "AX"
  KEY_OEM_102          = 0xE2,  // "<>" or "\|"
  KEY_ATTN             = 0xF6,  // Attn key
  KEY_CRSEL            = 0xF7,  // CrSel key
  KEY_EXSEL            = 0xF8,  // ExSel key
  KEY_EREOF            = 0xF9,  // Erase EOF key
  KEY_PLAY             = 0xFA,  // Play key
  KEY_ZOOM             = 0xFB,  // Zoom key
  KEY_PA1              = 0xFD,  // PA1 key
  KEY_OEM_CLEAR        = 0xFE,   // Clear key
  KEY_KEY_CODES_COUNT  = 0xFF // this is not a key, but the amount of keycodes there are.
} EKEY_CODE;
typedef enum EEVENT_TYPE {
  EET_NULL = 0,
  EET_EXIT,
  EET_GUI_EVENT,  //! An event of the graphical user interface.
  EET_MOUSE_EVENT,  //! A mouse input event.
  EET_KEY_EVENT,  //! A key input event.
  EET_JOYSTICK_EVENT,  //! A joystick (joypad, gamepad) input event.
  EET_LOG_EVENT,  //! A log event
  EET_USER_EVENT,
  //EGUIET_FORCE_32_BIT = 0x7fffffff
} EEVENT_TYPE;
//! Enumeration for all mouse input events
typedef enum EMOUSE_INPUT_EVENT {
  EMIE_LBUTTONDOWN = 0,  //! Left mouse button was pressed down.
  EMIE_RBUTTONDOWN,  //! Right mouse button was pressed down.
  EMIE_MBUTTONDOWN,  //! Middle mouse button was pressed down.
  EMIE_LBUTTONUP,  //! Left mouse button was left up.
  EMIE_RBUTTONUP,  //! Right mouse button was left up.
  EMIE_MBUTTONUP,  //! Middle mouse button was left up.
  EMIE_MOUSEMOVE,  //! The mouse cursor changed its position.
  EMIE_MOUSEWHEEL,  //! The mouse wheel was moved. Use Wheel value in event data to find out in what direction and how fast.
  EMIE_LDOUBLE_CLICK,  //! Left mouse button double click. This event is generated after the second EMIE_LPRESSED_DOWN event.
  EMIE_RDOUBLE_CLICK,  //! Right mouse button double click. This event is generated after the second EMIE_RPRESSED_DOWN event.
  EMIE_MDOUBLE_CLICK,  //! Middle mouse button double click. This event is generated after the second EMIE_MPRESSED_DOWN event.
  EMIE_LTRIPLE_CLICK,  //! Left mouse button triple click. This event is generated after the third EMIE_LPRESSED_DOWN event.
  EMIE_RTRIPLE_CLICK,  //! Right mouse button triple click. This event is generated after the third EMIE_RPRESSED_DOWN event.
  EMIE_MTRIPLE_CLICK,  //! Middle mouse button triple click. This event is generated after the third EMIE_MPRESSED_DOWN event.
  EMIE_COUNT  //! No real event. Just for convenience to get number of events
} EMOUSE_INPUT_EVENT;
//! Masks for mouse button states
typedef enum E_MOUSE_BUTTON_STATE_MASK {
  EMBSM_LBUTTON  = 0x01,
  EMBSM_RBUTTON  = 0x02,
  EMBSM_SHIFT    = 0x04,
  EMBSM_CONTROL  = 0x08,
  EMBSM_MBUTTON  = 0x10,
  EMBSM_XBUTTON1 = 0x20, //! currently only on windows
  EMBSM_XBUTTON2 = 0x40, //! currently only on windows
  EMBSM_FORCE_32_BIT = 0x7fffffff
} E_MOUSE_BUTTON_STATE_MASK;
#define MAKE_KEY(C, K)  ((C)<<16)|(K)
//! Enumeration for all events which are sendable by the gui system
typedef enum EGUI_EVENT_TYPE {
  EGET_CLOSE,
  EGET_GETMINSIZE, // ISIZE* lParam
  EGET_SETSIZE,
  EGET_PAINT,
  EGET_ELEMENT_FOCUS_LOST, //! A gui element has lost its focus.
  EGET_ELEMENT_FOCUSED, //! A gui element has got the focus.
  EGET_ELEMENT_HOVERED, //! The mouse cursor hovered over a gui element.
  EGET_ELEMENT_LEFT, //! The mouse cursor left the hovered element.
  EGET_ELEMENT_CLOSED, //! An element would like to close.
  EGET_BUTTON_CLICKED,//! A button was clicked.
  EGET_SCROLL_BAR_CHANGED, //! A scrollbar has changed its position.
  EGET_CHECKBOX_CHANGED, //! A checkbox has changed its check state.
  EGET_LISTBOX_CHANGED,   //! A new item in a listbox was selected.
  EGET_LISTBOX_SELECTED_AGAIN,   //! An item in the listbox was selected, which was already selected.
  EGET_FILE_SELECTED,  //! A file has been selected in the file dialog
  EGET_DIRECTORY_SELECTED,  //! A directory has been selected in the file dialog
  EGET_FILE_CHOOSE_DIALOG_CANCELLED,  //! A file open dialog has been closed without choosing a file
  EGET_MESSAGEBOX_YES,  //! 'Yes' was clicked on a messagebox
  EGET_MESSAGEBOX_NO,  //! 'No' was clicked on a messagebox
  EGET_MESSAGEBOX_OK,  //! 'OK' was clicked on a messagebox
  EGET_MESSAGEBOX_CANCEL,  //! 'Cancel' was clicked on a messagebox
  EGET_EDITBOX_ENTER,  //! In an editbox 'ENTER' was pressed
  EGET_EDITBOX_CHANGED,  //! The text in an editbox was changed. This does not include automatic changes in text-breaking.
  EGET_EDITBOX_MARKING_CHANGED,  //! The marked area in an editbox was changed.
  EGET_TAB_CHANGED,  //! The tab was changed in an tab control
  EGET_MENU_ITEM_SELECTED,  //! A menu item was selected in a (context) menu
  EGET_COMBO_BOX_CHANGED,  //! The selection in a combo box has been changed
  EGET_SPINBOX_CHANGED,  //! The value of a spin box has changed
  EGET_TABLE_CHANGED,  //! A table has changed
  EGET_TABLE_HEADER_CHANGED,
  EGET_TABLE_SELECTED_AGAIN,
  EGET_TREEVIEW_NODE_DESELECT,  //! A tree view node lost selection. See IGUITreeView::getLastEventNode().
  EGET_TREEVIEW_NODE_SELECT,  //! A tree view node was selected. See IGUITreeView::getLastEventNode().
  EGET_TREEVIEW_NODE_EXPAND,  //! A tree view node was expanded. See IGUITreeView::getLastEventNode().
  EGET_TREEVIEW_NODE_COLLAPSE,  //! A tree view node was collapsed. See IGUITreeView::getLastEventNode().
  EGET_TREEVIEW_NODE_COLLAPS = EGET_TREEVIEW_NODE_COLLAPSE,  //! deprecated - use EGET_TREEVIEW_NODE_COLLAPSE instead. This may be removed by Irrlicht 1.9
  EGET_COUNT  //! No real event. Just for convenience to get number of events
} EGUI_EVENT_TYPE;
//! An enum for the color format of textures used by the Irrlicht Engine.
typedef enum ECOLOR_FORMAT {
  ECF_A1R5G5B5 = 0, //! 16 bit color format used by the software driver.
  ECF_R5G6B5, //! Standard 16 bit color format.
  ECF_R8G8B8, //! 24 bit color, no alpha channel, but 8 bit for red, green and blue.
  ECF_A8R8G8B8, //! Default 32 bit color format. 8 bits are used for every component: red, green, blue and alpha.
  ECF_R16F, //! 16 bit floating point format using 16 bits for the red channel.
  ECF_G16R16F, //! 32 bit floating point format using 16 bits for the red channel and 16 bits for the green channel.
  ECF_A16B16G16R16F, //! 64 bit floating point format 16 bits are used for the red, green, blue and alpha channels.
  ECF_R32F, //! 32 bit floating point format using 32 bits for the red channel.
  ECF_G32R32F, //! 64 bit floating point format using 32 bits for the red channel and 32 bits for the green channel.
  ECF_A32B32G32R32F, //! 128 bit floating point format. 32 bits are used for the red, green, blue and alpha channels.
  ECF_UNKNOWN //! Unknown color format:
} ECOLOR_FORMAT;//! Material flags
typedef enum E_MATERIAL_FLAG {
  EMF_WIREFRAME = 0x1, //! Draw as wireframe or filled triangles? Default: false
  EMF_POINTCLOUD = 0x2,//! Draw as point cloud or filled triangles? Default: false
  EMF_GOURAUD_SHADING = 0x4,//! Flat or Gouraud shading? Default: true
  EMF_LIGHTING = 0x8,//! Will this material be lighted? Default: true
  EMF_ZBUFFER = 0x10,  //! Is the ZBuffer enabled? Default: true
  EMF_ZWRITE_ENABLE = 0x20,  //! May be written to the zbuffer or is it readonly. Default: true
  EMF_BACK_FACE_CULLING = 0x40,  //! Is backface culling enabled? Default: true
  EMF_FRONT_FACE_CULLING = 0x80,  //! Is frontface culling enabled? Default: false
  EMF_BILINEAR_FILTER = 0x100,  //! Is bilinear filtering enabled? Default: true
  EMF_TRILINEAR_FILTER = 0x200,  //! Is trilinear filtering enabled? Default: false
  EMF_ANISOTROPIC_FILTER = 0x400,  //! Is anisotropic filtering? Default: false
  EMF_FOG_ENABLE = 0x800,  //! Is fog enabled? Default: false
  EMF_NORMALIZE_NORMALS = 0x1000,  //! Normalizes normals. Default: false
  EMF_TEXTURE_WRAP = 0x2000,  //! Access to all layers texture wrap settings. Overwrites separate layer settings.
  EMF_ANTI_ALIASING = 0x4000,  //! AntiAliasing mode
  EMF_COLOR_MASK = 0x8000,  //! ColorMask bits, for enabling the color planes
  EMF_COLOR_MATERIAL = 0x10000,  //! ColorMaterial enum for vertex color interpretation
  EMF_USE_MIP_MAPS = 0x20000,  //! Flag for enabling/disabling mipmap usage
  EMF_BLEND_OPERATION = 0x40000,  //! Flag for blend operation
  EMF_POLYGON_OFFSET = 0x80000  //! Flag for polygon offset
} E_MATERIAL_FLAG;
typedef struct ImageInfo {
  u32 bpp;
  u32 rMask;
  u32 gMask;
  u32 bMask;
  u32 aMask;
} ImageInfo;
static const ImageInfo* getImageInfoFromFormat(const ECOLOR_FORMAT format)
{
  static ImageInfo _imginfo[20] = {0};
  ImageInfo* info = NULL;

  if (0 == _imginfo->bpp) {
#define IMAGEINFO_DEF(FMT, BBP, RM, GM, BM, AM)  info = _imginfo + FMT; {info->bpp = BBP, info->rMask = RM, info->gMask = GM, info->bMask = BM, info->aMask = AM;}
    IMAGEINFO_DEF(ECF_A1R5G5B5, 16, (0x1F << 10), (0x1F << 5), 0x1F, 0x1 << 15)
    IMAGEINFO_DEF(ECF_R5G6B5, 16, (0x1F << 11), 0x3F << 5, 0x1F, 0)
    IMAGEINFO_DEF(ECF_R8G8B8, 24, 0x00FF0000, 0x0000FF00, 0x000000FF, 0)
    IMAGEINFO_DEF(ECF_A8R8G8B8, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000)
    IMAGEINFO_DEF(ECF_R16F, 16, 0, 0, 0, 0)
    IMAGEINFO_DEF(ECF_G16R16F, 32, 0, 0, 0, 0)
    IMAGEINFO_DEF(ECF_A16B16G16R16F, 64, 0, 0, 0, 0)
    IMAGEINFO_DEF(ECF_R32F, 32, 0, 0, 0, 0)
    IMAGEINFO_DEF(ECF_G32R32F, 64, 0, 0, 0, 0)
    IMAGEINFO_DEF(ECF_A32B32G32R32F, 128, 0, 0, 0, 0)
#undef IMAGEINFO_DEF
  }

  return _imginfo + format;
}
typedef struct event_t {
  uint type, msg, wparam, lparam;
} event_t;
typedef int (*on_event_t)(void* s, uint uMsg, uint wParam, uint lParam);

typedef struct rander_t {
  void* x;
} rander_t;
typedef struct window_t window_t;
struct window_t {
  window_t* next;
  char name[32];
  void* hwnd;
  on_event_t on_event;
  void* user;
  rander_t rander[1];
};
#define FUNCS_SET(A, B)  { void* ptmp = (void*)(B); memcpy(&(A), &ptmp, sizeof(void*));}
#define FUNCS_SET2(f, CLASS, ret, FUN, arg)  FUNCS_SET((f)->FUN, CLASS##_## FUN )
#define FUNCS_DEF(f, CLASS, ret, FUN, arg)   ret (*FUN) arg

#define RANDER_FUNCS_DEF(f, CLASS, TT)  TT(f, CLASS, void, Close, (void* s)); \
  TT(f, CLASS, void, Clear, (void* s, int Count, const IRECT* pRects, DWORD Flags, COLOR Color)); \
  TT(f, CLASS, void, DrawPrimitiveUP, (void* s, int t, int count, const void* vertex, int stride)); \
  TT(f, CLASS, void, BeginScene, (void* s)); \
  TT(f, CLASS, void, SetRenderState, (void* s, int type, int b)); \
TT(f, CLASS, void, EndScene, (void* s));

#define RANDER_FUNCS_SET(f, CLASS)  RANDER_FUNCS_DEF(f, CLASS, FUNCS_SET2)

typedef struct rander_funcs_t {
  int dt;
  RANDER_FUNCS_DEF(f, CLASS, FUNCS_DEF)
} rander_funcs_t;
typedef struct rander_params_t {
  int Bits;
  int ZBufferBits;
  bool Doublebuffer;
  bool Stereobuffer;
  bool Stencilbuffer;
  bool MultiSamples;
} rander_params_t;


window_t* sysWindowOpt(const char* name_find, const void* winID, const char* name_add, const char* name_close, BOOL delall) {
  static window_t* g_win = NULL;
  static window_t* g_win_free = NULL;
  if (winID) {
    window_t* s = g_win;
    for (;s; s = s->next) {
      if (winID==(void*)s->hwnd) {
        return s;
      }
    }
  }
  if (delall) {
    window_t* s = g_win;
    for (;s;) {
      window_t* w_next = s->next;
      free(s);
      s = w_next;
    }
  }
  if (name_add) {
    window_t* s = g_win;
    for (;s; s = s->next) {
      if (0==strcmp(name_add, s->name)) {
        break;
      }
    }
    if (NULL==s) {
      if (g_win_free) {
        s = g_win_free;
        g_win_free = g_win_free->next;
      } else {
        s = (window_t*)malloc(sizeof(window_t));
        memset(s, 0, sizeof(*s));
      }
      s->next = g_win;
      g_win = s;
      strncpy(s->name, name_add, 32);
    }
    return s;
  }
  if (name_find) {
    window_t* s = g_win;
    for (;s; s = s->next) {
      if (0==strcmp(name_find, s->name)) {
        return s;
      }
    }
  }
  if (name_close) {
    window_t* s = g_win;
    window_t* s_prev = NULL;
    for (;s; s_prev = s, s = s->next) {
      if (0==strcmp(name_close, s->name)) {
        //ShowWindow(s->hwnd, SW_HIDE);
        //SetWindowLong(s->hwnd, GWL_WNDPROC, (LONG)DefWindowProc);
        if (s==g_win) {
          g_win = s->next;
        } else {
          s_prev->next = s->next;
        }
        s->next = g_win_free;
        g_win_free = s;
        return s;
      }
    }
  }
  return NULL;
}
#endif // _GUI_INL_
