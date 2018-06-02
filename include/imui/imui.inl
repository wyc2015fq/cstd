//#pragma warning (disable: 4244)
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#include "draw/draw2d.inl"
typedef enum {
  // Default: 0
  ImGuiWindowFlags_NoTitleBar = 1 << 0, // Disable title-bar
  ImGuiWindowFlags_NoResize = 1 << 1, // Disable user resizing with the lower-right grip
  ImGuiWindowFlags_NoMove = 1 << 2, // Disable user moving the window
  ImGuiWindowFlags_NoScrollbar = 1 << 3, // Disable scrollbars (window can still scroll with mouse or programatically)
  ImGuiWindowFlags_NoScrollWithMouse = 1 << 4, // Disable user vertically scrolling with mouse wheel
  ImGuiWindowFlags_NoCollapse = 1 << 5, // Disable user collapsing window by double-clicking on it
  ImGuiWindowFlags_AlwaysAutoResize = 1 << 6, // Resize every window to its content every frame
  ImGuiWindowFlags_NoSavedSettings = 1 << 8, // Never load/save settings in .ini file
  ImGuiWindowFlags_NoInputs = 1 << 9, // Disable catching mouse or keyboard inputs
  ImGuiWindowFlags_MenuBar = 1 << 10, // Has a menu-bar
  ImGuiWindowFlags_HorizontalScrollbar = 1 << 11, // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
  ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12, // Disable taking focus when transitioning from hidden to visible state
  ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13, // Disable bringing window to front when taking focus (e.g. clicking on it or programatically giving it focus)
  ImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14, // Always show vertical scrollbar (even if ContentSize.y < Size.y)
  ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15, // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
  ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16, // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
  // [Internal]
  ImGuiWindowFlags_ChildWindow = 1 << 20, // Don't use! For internal use by BeginChild()
  ImGuiWindowFlags_ChildWindowAutoFitX = 1 << 21, // Don't use! For internal use by BeginChild()
  ImGuiWindowFlags_ChildWindowAutoFitY = 1 << 22, // Don't use! For internal use by BeginChild()
  ImGuiWindowFlags_ComboBox = 1 << 23, // Don't use! For internal use by ComboBox()
  ImGuiWindowFlags_Tooltip = 1 << 24, // Don't use! For internal use by BeginTooltip()
  ImGuiWindowFlags_Popup = 1 << 25, // Don't use! For internal use by BeginPopup()
  ImGuiWindowFlags_Modal = 1 << 26, // Don't use! For internal use by BeginPopupModal()
  ImGuiWindowFlags_ChildMenu = 1 << 27 // Don't use! For internal use by BeginMenu()
} ImGuiWindowFlags_;
// Flags for ImGui_InputText()
enum ImGuiInputTextFlags_ {
  // Default: 0
  ImGuiInputTextFlags_CharsDecimal = 1 << 0, // Allow 0123456789.+-*/
  ImGuiInputTextFlags_CharsHexadecimal = 1 << 1, // Allow 0123456789ABCDEFabcdef
  ImGuiInputTextFlags_CharsUppercase = 1 << 2, // Turn a..z into A..Z
  ImGuiInputTextFlags_CharsNoBlank = 1 << 3, // Filter out spaces, tabs
  ImGuiInputTextFlags_AutoSelectAll = 1 << 4, // Select entire text when first taking mouse focus
  ImGuiInputTextFlags_EnterReturnsTrue = 1 << 5, // Return 'true' when Enter is pressed (as opposed to when the value was modified)
  ImGuiInputTextFlags_CallbackCompletion = 1 << 6, // Call user function on pressing TAB (for completion handling)
  ImGuiInputTextFlags_CallbackHistory = 1 << 7, // Call user function on pressing Up/Down arrows (for history handling)
  ImGuiInputTextFlags_CallbackAlways = 1 << 8, // Call user function every time. User code may query cursor position, modify text buffer.
  ImGuiInputTextFlags_CallbackCharFilter = 1 << 9, // Call user function to filter character. Modify data->EventChar to replace/filter input, or return 1 to discard character.
  ImGuiInputTextFlags_AllowTabInput = 1 << 10, // Pressing TAB input a '\t' character into the text field
  ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11, // In multi-line mode, allow exiting edition by pressing Enter. Ctrl+Enter to add new line (by default adds new lines with Enter).
  ImGuiInputTextFlags_NoHorizontalScroll = 1 << 12, // Disable following the cursor horizontally
  ImGuiInputTextFlags_AlwaysInsertMode = 1 << 13, // Insert mode
  ImGuiInputTextFlags_ReadOnly = 1 << 14, // Read-only mode
  ImGuiInputTextFlags_Password = 1 << 15, // Password mode, display all characters as '*'
  // [Internal]
  ImGuiInputTextFlags_Multiline = 1 << 20 // For internal use by InputTextMultiline()
};
// Flags for ImGui_TreeNodeEx(), ImGui_CollapsingHeader*()
enum ImGuiTreeNodeFlags_ {
  ImGuiTreeNodeFlags_Selected = 1 << 0, // Draw as selected
  ImGuiTreeNodeFlags_Framed = 1 << 1, // Full colored frame (e.g. for CollapsingHeader)
  ImGuiTreeNodeFlags_AllowOverlapMode = 1 << 2, // Hit testing to allow subsequent widgets to overlap this one
  ImGuiTreeNodeFlags_NoTreePushOnOpen = 1 << 3, // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
  ImGuiTreeNodeFlags_NoAutoOpenOnLog = 1 << 4, // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
  ImGuiTreeNodeFlags_DefaultOpen = 1 << 5, // Default node to be open
  ImGuiTreeNodeFlags_OpenOnDoubleClick = 1 << 6, // Need double-click to open node
  ImGuiTreeNodeFlags_OpenOnArrow = 1 << 7, // Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
  ImGuiTreeNodeFlags_Leaf = 1 << 8, // No collapsing, no arrow (use as a convenience for leaf nodes).
  ImGuiTreeNodeFlags_Bullet = 1 << 9, // Display a bullet instead of arrow
  //ImGuITreeNodeFlags_SpanAllAvailWidth = 1 << 10, // FIXME: TODO: Extend hit box horizontally even if not framed
  //ImGuiTreeNodeFlags_NoScrollOnOpen = 1 << 11, // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
  ImGuiTreeNodeFlags_CollapsingHeader = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog
};
// Flags for ImGui_Selectable()
enum ImGuiSelectableFlags_ {
  // Default: 0
  ImGuiSelectableFlags_DontClosePopups = 1 << 0, // Clicking this don't close parent popup window
  ImGuiSelectableFlags_SpanAllColumns = 1 << 1, // Selectable frame can span all columns (text will still fit in current column)
  ImGuiSelectableFlags_AllowDoubleClick = 1 << 2 // Generate press events on double clicks too
};
// User fill ImGuiIO.KeyMap[] array with indices into the ImGuiIO.KeysDown[512] array
enum ImGuiKey_ {
  KEY_TAB, // FOR TABBING THROUGH FIELDS
  KEY_LEFT, // FOR TEXT EDIT
  KEY_RIGHT,// FOR TEXT EDIT
  KEY_UP, // FOR TEXT EDIT
  KEY_DOWN, // FOR TEXT EDIT
  KEY_PAGEUP,
  KEY_PAGEDOWN,
  KEY_HOME, // FOR TEXT EDIT
  KEY_END, // FOR TEXT EDIT
  KEY_DELETE, // FOR TEXT EDIT
  KEY_BACK, // FOR TEXT EDIT
  KEY_ENTER, // FOR TEXT EDIT
  KEY_ESCAPE, // FOR TEXT EDIT
  KEY_COUNT
};
// Enumeration for PushStyleVar() / PopStyleVar()
// NB: the enum only refers to fields of ImGuiStyle() which makes sense to be pushed/poped in UI code. Feel free to add others.
enum ImGuiStyleVar_ {
  ImGuiStyleVar_Alpha, // float
  ImGuiStyleVar_WindowPadding, // ImVec2
  ImGuiStyleVar_WindowRounding, // float
  ImGuiStyleVar_WindowMinSize, // ImVec2
  ImGuiStyleVar_ChildWindowRounding, // float
  ImGuiStyleVar_FramePadding, // ImVec2
  ImGuiStyleVar_FrameRounding, // float
  ImGuiStyleVar_ItemSpacing, // ImVec2
  ImGuiStyleVar_ItemInnerSpacing, // ImVec2
  ImGuiStyleVar_IndentSpacing, // float
  ImGuiStyleVar_GrabMinSize, // float
  ImGuiStyleVar_ButtonTextAlign, // flags ImGuiAlign_*
  ImGuiStyleVar_Count_
};
// Enumeration for ColorEditMode()
// FIXME-OBSOLETE: Will be replaced by future color/picker api
enum ImGuiColorEditMode_ {
  ImGuiColorEditMode_UserSelect = -2,
  ImGuiColorEditMode_UserSelectShowButton = -1,
  ImGuiColorEditMode_RGB = 0,
  ImGuiColorEditMode_HSV = 1,
  ImGuiColorEditMode_HEX = 2
};
// Enumeration for GetMouseCursor()
enum ImGuiMouseCursor_ {
  ImGuiMouseCursor_Arrow = 0,
  ImGuiMouseCursor_TextInput, // When hovering over InputText, etc.
  ImGuiMouseCursor_Move, // Unused
  ImGuiMouseCursor_ResizeNS, // Unused
  ImGuiMouseCursor_ResizeEW, // When hovering over a column
  ImGuiMouseCursor_ResizeNESW, // Unused
  ImGuiMouseCursor_ResizeNWSE, // When hovering over the bottom-right corner of a window
  ImGuiMouseCursor_Count_
};
// Condition flags for ImGui_SetWindow***(), SetNextWindow***(), SetNextTreeNode***() functions
// All those functions treat 0 as a shortcut to ImGuiSetCond_Always
enum ImGuiSetCond_ {
  ImGuiSetCond_Always = 1 << 0, // Set the variable
  ImGuiSetCond_Once = 1 << 1, // Set the variable once per runtime session (only the first call with succeed)
  ImGuiSetCond_FirstUseEver = 1 << 2, // Set the variable if the window has no saved data (if doesn't exist in the .ini file)
  ImGuiSetCond_Appearing = 1 << 3 // Set the variable if the window is appearing after being hidden/inactive (or the first time)
};
typedef int ImGuiLayoutType; // enum ImGuiLayoutType_
typedef int ImGuiButtonFlags; // enum ImGuiButtonFlags_
typedef int ImGuiTreeNodeFlags; // enum ImGuiTreeNodeFlags_
typedef int ImGuiSliderFlags; // enum ImGuiSliderFlags_
//-----------------------------------------------------------------------------
// Types
//-----------------------------------------------------------------------------
typedef enum {
  ImGuiButtonFlags_Repeat = 1 << 0, // hold to repeat
  ImGuiButtonFlags_PressedOnClickRelease = 1 << 1, // (default) return pressed on click+release on same item (default if no PressedOn** flag is set)
  ImGuiButtonFlags_PressedOnClick = 1 << 2, // return pressed on click (default requires click+release)
  ImGuiButtonFlags_PressedOnRelease = 1 << 3, // return pressed on release (default requires click+release)
  ImGuiButtonFlags_PressedOnDoubleClick = 1 << 4, // return pressed on double-click (default requires click+release)
  ImGuiButtonFlags_FlattenChilds = 1 << 5, // allow interaction even if a child window is overlapping
  ImGuiButtonFlags_DontClosePopups = 1 << 6, // disable automatically closing parent popup on press
  ImGuiButtonFlags_Disabled = 1 << 7, // disable interaction
  ImGuiButtonFlags_AlignTextBaseLine = 1 << 8, // vertically align button to match text baseline - ButtonEx() only
  ImGuiButtonFlags_NoKeyModifiers = 1 << 9, // disable interaction if a key modifier is held
  ImGuiButtonFlags_AllowOverlapMode = 1 << 10 // require previous frame HoveredId to either match id or be null before being usable
} ImGuiButtonFlags_;
typedef enum {
  ImGuiSliderFlags_Vertical = 1 << 0
} ImGuiSliderFlags_;
typedef enum {
  // NB: need to be in sync with last value of ImGuiSelectableFlags_
  ImGuiSelectableFlags_Menu = 1 << 3,
  ImGuiSelectableFlags_MenuItem = 1 << 4,
  ImGuiSelectableFlags_Disabled = 1 << 5,
  ImGuiSelectableFlags_DrawFillAvailWidth = 1 << 6
} ImGuiSelectableFlagsPrivate_;
// FIXME: this is in development, not exposed/functional as a generic feature yet.
typedef enum {
  ImGuiLayoutType_Vertical,
  ImGuiLayoutType_Horizontal
} ImGuiLayoutType_;
typedef enum {
  ImGuiPlotType_Lines,
  ImGuiPlotType_Histogram
} ImGuiPlotType;
typedef enum {
  ImGuiDataType_Int,
  ImGuiDataType_Float,
  ImGuiDataType_Float2,
} ImGuiDataType;
#define IM_STATIC static
//#define ASSERT assert
#define IM_ASSERT assert
#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_PI 3.14159265358979323846f
#define IM_OFFSETOF(_TYPE,_ELM) ((size_t)&(((_TYPE*)0)->_ELM))
#define vsnprintf _vsnprintf
typedef unsigned char BYTE;
typedef int BOOL;
typedef void* ImGuiID;
typedef ushort ImWchar;
typedef int ImGuiCol;
typedef int ImGuiStyleVar;
typedef int ImGuiKey;
typedef int ImGuiColorEditMode;
typedef int ImGuiMouseCursor;
typedef int ImGuiWindowFlags;
typedef int ImGuiSetCond;
typedef int ImGuiInputTextFlags;
typedef int ImGuiSelectableFlags;
typedef int ImGuiTreeNodeFlags;
#define ImRGBAToBGRA(clr) ((clr) & 0xFF00FF00) | (((clr) & 0xFF0000) >> 16) | (((clr) & 0xFF) << 16)
//#define ImRGBA(r, g, b, a) (ImU32)( ((BYTE)(r)) | (((BYTE)(g)) << 8) | (((BYTE)(b)) << 16) | (((BYTE)(a)) << 24) )
//#define ImRGB(r, g, b) ImRGBA(r, g, b, 255)
//#define ImGetR(clr) (BYTE)((clr) & 0xFF)
//#define ImGetG(clr) (BYTE)((clr>>8) & 0xFF)
//#define ImGetB(clr) (BYTE)((clr>>16) & 0xFF)
//#define ImGetA(clr) (BYTE)((clr>>24) & 0xFF)


typedef IPOINT charpos_t;
typedef struct {
  int c;
  int nch;
  COLOR fg, bg;
} char_color_t;
typedef struct {
  int bbeg, bend;
  int cbeg, cend;
  COLOR fg, bg;
} text_range_t;
typedef struct strdata_t_ strdata_t;
struct strdata_t_ {
  int (*get_line)(strdata_t* s, int irow, int icol, text_range_t* out, int maxoutlen, int stage); // 获取i行str不含 \n
  int (*ins_str)(strdata_t* s, const char* str, int len); // 用str 替换 [p1,p2]的内容
  int (*set_pos)(strdata_t* s, int x, int y);
  int (*move_pos)(strdata_t* s, int ichr, int iline);
  int (*get_sel)(strdata_t* s, str_t* str);
  charpos_t beg, pos;
  str_t s[1];
  vstr_t sv[1];
  buf_t bf[1];
};
typedef struct {
  IRECT rc;
  int fmt;
  IPOINT org;
  strdata_t sd[1];
} editctrl_t;
//typedef enum {ImGuiWindow_;} ImGuiWindowStage;
typedef struct window_t_ window_t; //ImGuiWidget
struct window_t_ {
  IRECT rect;
  char text[16];
  window_t* prev;
  window_t* next;
  window_t* parent;
  window_t* child;
  gc_t ctx[1];
};
#define ISHOT(c) (io->HoveredId==(c))
#define SETHOT(c) (io->HoveredId=(c))
#define SETHOT2(c, hot) (io->HoveredId=(hot) ? (c) : NULL)
#define ISHIT(c) (io->MouseDownID[0]==(c))
#define SETHIT1(c) (io->MouseDownID[0]=(c))
#define SETHITV(v) (io->HitValue = v)
#define ISFOCUS(c) (io->FocusedId==(c))
#define SETFOCUS(c) (io->FocusedId=(c))
#define SETFOCUSWIN(c) (io->FocusedWindow=(c))
#define ISFOCUSWIN(c) (io->FocusedWindow==(c))
#define FORCE_REDRAW(c) (0)
#define ISHOVEREDWIN(c) (host->HoveredWindow==(c))
#define ISHOVEREDWIN2() (host->HoveredWindow==io->CurrentWindow)
typedef struct {
  void* c;
  IPOINT sz;
} layersize_t;
typedef struct ImGuiHost_ ImGuiHost;
struct ImGuiHost_ {
  int (*DrvUnInit)(ImGuiHost* host);
  int (*DrvRander)(ImGuiHost* host, const gc_t* ctx);
  int (*DrvSetTexture)(ImGuiHost* host, texture_t* tex, int fmt, int height, int width, void* data, int bytes_per_pixel);
  int (*DrvMeasureText)(ImGuiHost* host, const char* text, const char* text_end, texfont_t* font, int formatFlags, IRECT* rc);
  ImColor clrbg;
  //------------------------------------------------------------------
  //------------------------------------------------------------------
  void* Window;
  void* Driver;
  BOOL NoSoothMode;
  ImGuiHost* prev;
  ImGuiHost* next;
  window_t rootWindow[1];
  window_t tooltip[1];
  window_t popup[8];
  int pop_deep;
  layersize_t lsz[16];
  int ilsz;
  window_t* HoveredWindow;
  IRECT rcDirty;
  int w, h;
};
int host_clear(ImGuiHost* host, COLOR clr)
{
  if (clr != host->clrbg) {
    host->clrbg = clr;
  }
  host->rcDirty = iRECT2(0, 0, host->w, host->h);
  return 0;
}
typedef enum {
  MSG_NOP,
  MSG_TIMER,
  MSG_LBUTTONDBLCLK,
  MSG_LBUTTONDOWN,
  MSG_LBUTTONUP,
  MSG_RBUTTONDOWN,
  MSG_RBUTTONUP,
  MSG_MBUTTONDOWN,
  MSG_MBUTTONUP,
  MSG_MOUSEWHEEL,
  MSG_MOUSEMOVE,
  MSG_KEYDOWN,
  MSG_KEYUP,
  MSG_CHAR,
} WindowMsg;
typedef struct {
  IRECT rect;
  IRECT rcLT;
  IRECT rcRB;
  IPOINT Size;
  IPOINT Pos;
  BOOL horz;
  int irc;
  int nrcs;
  int gap;
  int ilsz;
  int layoutflags;
} layerinfo_t;
typedef struct {
  //------------------------------------------------------------------
  // Settings (fill once) // Default value:
  //------------------------------------------------------------------
  float DeltaTime;
  float IniSavingRate;
  const char* IniFilename;
  const char* LogFilename;
  float MouseDoubleClickTime;
  float MouseDoubleClickMaxDist;
  float MouseDragThreshold;
  int KeyMap[KEY_COUNT];
  float KeyRepeatDelay;
  float KeyRepeatRate;
  void* UserData;
  //ImFontAtlas* Fonts;
  float FontGlobalScale;
  bool FontAllowUserScaling;
  ImVec2 DisplayFramebufferScale;
  ImVec2 DisplayVisibleMin;
  ImVec2 DisplayVisibleMax;
  BOOL OSXBehaviors; // = defined(__APPLE__) // OS X style: Text editing cursor movement using Alt instead of Ctrl, Shortcuts using Cmd/Super instead of Ctrl, Line/Text Start and End using Cmd+Arrows instead of Home/End, Double click selects by word instead of selecting whole text, Multi-selection in lists uses Cmd/Super instead of Ctrl
  //------------------------------------------------------------------
  // Input - Fill before calling NewFrame()
  //------------------------------------------------------------------
  IPOINT lastMouseDownPos;
  WindowMsg lastMsg;
  ushort lastChar;
  IPOINT MousePos; // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
  bool MouseDown[5]; // Mouse buttons: left, right, middle + extras. ImGui itself mostly only uses left button (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.
  float MouseWheel; // Mouse wheel: 1 unit scrolls about 5 lines text.
  bool MouseDrawCursor; // Request ImGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor).
  bool KeyCtrl; // Keyboard modifier pressed: Control
  bool KeyShift; // Keyboard modifier pressed: Shift
  bool KeyAlt; // Keyboard modifier pressed: Alt
  bool KeySuper; // Keyboard modifier pressed: Cmd/Super/Windows
  bool KeysDown[512]; // Keyboard keys that are pressed (in whatever storage order you naturally have access to keyboard data)
  ImWchar InputCharacters[16 + 1]; // List of characters input (translated by user from keypress+keyboard state). Fill using AddInputCharacter() helper.
  //------------------------------------------------------------------
  // Output - Retrieve after calling NewFrame(), you can use them to discard inputs or hide them from the rest of your application
  //------------------------------------------------------------------
  bool WantCaptureMouse; // Mouse is hovering a window or widget is active (= ImGui will use your mouse input)
  bool WantCaptureKeyboard; // Widget is active (= ImGui will use your keyboard input)
  bool WantTextInput; // Some text input widget is active, which will read input characters from the InputCharacters array.
  float Framerate; // Framerate estimation, in frame per second. Rolling average estimation based on IO.DeltaTime over 120 frames
  int MetricsAllocs; // Number of active memory allocations
  int MetricsRenderVertices; // Vertices output during last call to Render()
  int MetricsRenderIndices; // Indices output during last call to Render() = number of triangles * 3
  int MetricsActiveWindows; // Number of visible windows (exclude child windows)
  //------------------------------------------------------------------
  // [Internal] ImGui will maintain those fields for you
  //------------------------------------------------------------------
  ImGuiID MouseDownID[5];
  ImVec2 MousePosPrev; // Previous mouse position
  ImVec2 MouseDelta; // Mouse delta. Note that this is zero if either current or previous position are negative to allow mouse enabling/disabling.
  bool MouseClicked[5]; // Mouse button went from !Down to Down
  ImVec2 MouseClickedPos[5]; // Position at time of clicking
  int64 MouseClickedTime[5]; // Time of last click (used to figure out double-click)
  bool MouseDoubleClicked[5]; // Has mouse button been double-clicked?
  bool MouseReleased[5]; // Mouse button went from Down to !Down
  bool MouseDownOwned[5]; // Track if button was clicked inside a window. We don't request mouse capture from the application if click started outside ImGui bounds.
  float MouseDownDuration[5]; // Duration the mouse button has been down (0.0f == just clicked)
  float MouseDownDurationPrev[5]; // Previous time the mouse button has been down
  float MouseDragMaxDistanceSqr[5]; // Squared maximum distance of how much mouse has traveled from the click point
  float KeysDownDuration[512]; // Duration the keyboard key has been down (0.0f == just pressed)
  float KeysDownDurationPrev[512]; // Previous duration the key has been down
  //------------------------------------------------------------------
  bool ActiveIdIsAlive;
  bool ActiveIdIsJustActivated; // Set at the time of activation for one frame
  bool ActiveIdAllowOverlap; // Set only by active widget
  bool HoveredIdAllowOverlap;
  void* FocusedId; // Hovered widget
  void* HoveredId; // Hovered widget
  void* HoveredIdPreviousFrame;
  void* ActiveId; // Active widget
  void* ActiveIdPreviousFrame;
  int HitValue;
  window_t* CurrentWindow;
  window_t* FocusedWindow;
  window_t* HoveredRootWindow;
  window_t* RootWindow;
  texfont_t* font;
  //------------------------------------------------------------------
  char def_font_name[32];
  int def_font_size;
  //------------------------------------------------------------------
  ImGuiHost* HeadHost;
  ImGuiHost* CurrentHost;
  ImGuiHost* HotHost;
  int deep;
  layerinfo_t layinfo[16];
  IRECT rcs[100];
  int nrcs;
  //------------------------------------------------------------------
  int64 Time;
  //------------------------------------------------------------------
#if 0
  IRECT* rcstack;
  int* rclen;
  int rspos;
  int rcstack_size;
#endif
  int gap;
  int layoutflags;
  //------------------------------------------------------------------
} ImGui;
typedef int (*ImGuiDriverInit_t)(ImGuiHost* host);
/////////////////////////////////////////////////////////////////////////////////
int imuiLoop(int delay);
int imuiUnInit();
int ImGuiHost_Init(ImGuiHost* host, ImGuiDriverInit_t drv_init, const char* lpWindowName, int x, int y, int nWidth, int nHeight);
int gdi_Init(ImGuiHost* host);
int soft_Init(ImGuiHost* host);
int gdip_Init(ImGuiHost* host);
int d3d9_Init(ImGuiHost* host);
int gl2_Init(ImGuiHost* host);
int x11_Init(ImGuiHost* host);
/////////////////////////////////////////////////////////////////////////////////
#if 0
void triImage(ImDrawList* g, ImTextureID user_texture_id, ImVec2 a, ImVec2 b, ImVec2 uv0, ImVec2 uv1)
{
  if ((col >> 24) == 0) {
    return;
  }
  // FIXME-OPT: This is wasting draw calls.
  const bool push_texture_id = g->_TextureIdStack.empty() || user_texture_id != g->_TextureIdStack.back();
  if (push_texture_id) {
    triPushTextureID(g, user_texture_id);
  }
  triPrimReserve(g, 6, 4);
  triPrimRectUV(g, a, b, uv0, uv1, col);
  if (push_texture_id) {
    triPopTextureID(g);
  }
}
#endif
//-----------------------------------------------------------------------------
// HELPERS
//-----------------------------------------------------------------------------
#define IM_F32_TO_INT8_UNBOUND(_VAL) ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f))) // Unsaturated, for display purpose 
#define IM_F32_TO_INT8_SAT(_VAL) ((int)(ImSaturate(_VAL) * 255.0f + 0.5f)) // Saturated, always output 0..255
// Play it nice with Windows users. Notepad in 2015 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif
#define ImIsPointInTriangle(p, a, b, c) ImIsPointInTriangle_f32(p.x, p.y, a.x, a.y, b.x, b.y, c.x, c.y)
BOOL ImIsPointInTriangle_f32(float px, float py, float ax, float ay, float bx, float by, float cx, float cy)
{
  BOOL b1 = ((px - bx) * (ay - by) - (py - by) * (ax - bx)) < 0.0f;
  BOOL b2 = ((px - cx) * (by - cy) - (py - cy) * (bx - cx)) < 0.0f;
  BOOL b3 = ((px - ax) * (cy - ay) - (py - ay) * (cx - ax)) < 0.0f;
  return ((b1 == b2) && (b2 == b3));
}
int ImStricmp(const char* str1, const char* str2)
{
  int d;
  while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
    str1++;
    str2++;
  }
  return d;
}
int ImStrnicmp(const char* str1, const char* str2, int count)
{
  int d = 0;
  while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
    str1++;
    str2++;
    count--;
  }
  return d;
}
void ImStrncpy(char* dst, const char* src, int count)
{
  if (count < 1) {
    return;
  }
  strncpy(dst, src, (size_t)count);
  dst[count - 1] = 0;
}
char* ImStrdup(const char* str)
{
  size_t len = strlen(str) + 1;
  char* buff = NULL;
  myReAlloc(buff, len);
  return (char*)memcpy(buff, (const void*)str, len);
}
int ImStrlenW(const ImWchar* str)
{
  int n = 0;
  while (*str++) {
    n++;
  }
  return n;
}
const ImWchar* ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin) // find beginning-of-line
{
  while (buf_mid_line > buf_begin && buf_mid_line[-1] != '\n') {
    buf_mid_line--;
  }
  return buf_mid_line;
}
const char* ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end)
{
  char un0;
  if (!needle_end) {
    needle_end = needle + strlen(needle);
  }
  un0 = (char)toupper(*needle);
  while ((!haystack_end && *haystack) || (haystack_end && haystack < haystack_end)) {
    if (toupper(*haystack) == un0) {
      const char* a;
      const char* b = needle + 1;
      for (a = haystack + 1; b < needle_end; a++, b++)
        if (toupper(*a) != toupper(*b)) {
          break;
        }
      if (b == needle_end) {
        return haystack;
      }
    }
    haystack++;
  }
  return NULL;
}
// MSVC version appears to return -1 on overflow, whereas glibc appears to return total count (which may be >= buf_size).
// Ideally we would test for only one of those limits at runtime depending on the behavior the vsnprintf(), but trying to deduct it at compile time sounds like a pandora can of worm.
int ImFormatString(char* buf, int buf_size, const char* fmt, ...)
{
  int w;
  va_list args;
  IM_ASSERT(buf_size > 0);
  va_start(args, fmt);
  w = vsnprintf(buf, buf_size, fmt, args);
  va_end(args);
  if (w == -1 || w >= buf_size) {
    w = buf_size - 1;
  }
  buf[w] = 0;
  return w;
}
int ImFormatStringV(char* buf, int buf_size, const char* fmt, va_list args)
{
  int w;
  IM_ASSERT(buf_size > 0);
  w = vsnprintf(buf, buf_size, fmt, args);
  if (w == -1 || w >= buf_size) {
    w = buf_size - 1;
  }
  buf[w] = 0;
  return w;
}
/////////////////////////////////////////////////////////////////////////////////
#if 1
#define GETGUIIO1() ImGuiHost* host = io->CurrentHost
static ImGui io[1] = {0};
#else
#define GETGUIIO1() ImGui* io = imuiGetIO1(); ImGuiHost* host = io->CurrentHost
ImGui* imuiGetIO1()
{
  static ImGui g_io[1] = {0};
  return g_io;
}
#endif
void imuiInit()
{
  GETGUIIO1();
  io->MouseDoubleClickTime = 0.3f;
  io->gap = 4;
}
/*
GUIIO_BEGIN();
GUIIO_END();
*/
#define GETGUIIO2() GETGUIIO1(); gc_t* ctx = io->CurrentWindow->ctx
#define GUIIO_END() ImGuiBase_End(io, host)
#define GUIIO_BEGIN() GETGUIIO1();gc_t* ctx = ImGuiBase_Begin(io, host, c)
gc_t* ImGuiBase_Begin(ImGui* io, ImGuiHost* host, window_t* c)
{
  if (c->parent == NULL) {
    c->parent = io->CurrentWindow;
    if (io->CurrentWindow->child) {
      window_t* last = io->CurrentWindow->child;
      for (; last->next; last = last->next);
      if (c->prev = last) {
        last->next = c;
      }
    }
    else {
      io->CurrentWindow->child = c;
    }
  }
  io->CurrentWindow = c;
  return io->CurrentWindow->ctx;
}
void ImGuiBase_End(ImGui* io, ImGuiHost* host)
{
  ASSERT(io->CurrentWindow);
  io->CurrentWindow = io->CurrentWindow->parent;
}
void ImGuiBase_FreeAllCtx(window_t* c)
{
  if (c) {
    if (c->ctx->buf) {
      free(c->ctx->buf);
      c->ctx->buf = NULL;
    }
    ImGuiBase_FreeAllCtx(c->next);
    ImGuiBase_FreeAllCtx(c->child);
  }
}
// Pass data_size==0 for zero-terminated strings
// FIXME-OPT: Replace with e.g. FNV1a hash? CRC32 pretty much randomly access 1KB. Need to do proper measurements.
ImU32 ImHash(const void* data, int data_size, ImU32 seed)
{
  static ImU32 crc32_lut[256] = { 0 };
  ImU32 i, j, crc;
  const unsigned char* current;
  if (!crc32_lut[1]) {
    const ImU32 polynomial = 0xEDB88320;
    for (i = 0; i < 256; i++) {
      crc = i;
      for (j = 0; j < 8; j++) {
        crc = (crc >> 1) ^ ((ImU32)(-(int)(crc & 1)) & polynomial);
      }
      crc32_lut[i] = crc;
    }
  }
  seed = ~seed;
  crc = seed;
  current = (const unsigned char*)data;
  if (data_size > 0) {
    // Known size
    while (data_size--) {
      crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *current++];
    }
  }
  else {
    unsigned char c;
    // Zero-terminated string
    while (c = *current++) {
      // We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
      // Because this syntax is rarely used we are optimizing for the common case.
      // - If we reach ### in the string we discard the hash so far and reset to the seed.
      // - We don't do 'current += 2; continue;' after handling ### to keep the code smaller.
      if (c == '#' && current[0] == '#' && current[1] == '#') {
        crc = seed;
      }
      crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
    }
  }
  return ~crc;
}
ImVec4 imuiColorConvertU32ToFloat4(ImU32 in)
{
  float s = 1.0f / 255.0f;
  return fVec4((in & 0xFF) * s, ((in >> 8) & 0xFF) * s, ((in >> 16) & 0xFF) * s, (in >> 24) * s);
}
// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void imuiColorConvertRGBtoHSV(float r, float g, float b, float* out_h, float* out_s, float* out_v)
{
  float K = 0.f, chroma, tmp;
  if (g < b) {
    tmp = g;
    g = b;
    b = tmp;
    K = -1.f;
  }
  if (r < g) {
    tmp = r;
    r = g;
    g = tmp;
    K = -2.f / 6.f - K;
  }
  chroma = r - (g < b ? g : b);
  *out_h = fabsf(K + (g - b) / (6.f * chroma + 1e-20f));
  *out_s = chroma / (r + 1e-20f);
  *out_v = r;
}
// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
void imuiColorConvertHSVtoRGB(float h, float s, float v, float* out_r, float* out_g, float* out_b)
{
  int i;
  float f, p, q, t;
  if (s == 0.0f) {
    // gray
    *out_r = *out_g = *out_b = v;
    return;
  }
  h = fmodf(h, 1.0f) / (60.0f / 360.0f);
  i = (int)h;
  f = (h - (float)i);
  p = v * (1.0f - s);
  q = v * (1.0f - s * f);
  t = v * (1.0f - s * (1.0f - f));
  switch (i) {
  case 0:
    *out_r = v;
    *out_g = t;
    *out_b = p;
    break;
  case 1:
    *out_r = q;
    *out_g = v;
    *out_b = p;
    break;
  case 2:
    *out_r = p;
    *out_g = v;
    *out_b = t;
    break;
  case 3:
    *out_r = p;
    *out_g = q;
    *out_b = v;
    break;
  case 4:
    *out_r = t;
    *out_g = p;
    *out_b = v;
    break;
  case 5:
  default:
    *out_r = v;
    *out_g = p;
    *out_b = q;
    break;
  }
}
// Load file content into memory
// Memory allocated with imuiReAlloc(), must be freed by user using imuiMemFree()
void* ImLoadFileToMemory(const char* filename, const char* file_open_mode, int* out_file_size, int padding_bytes)
{
  FILE* f = NULL;
  long file_size_signed = 0;
  int file_size = 0;
  char* file_data = NULL;
  IM_ASSERT(filename && file_open_mode);
  if (out_file_size) {
    *out_file_size = 0;
  }
  if ((f = fopen(filename, file_open_mode)) == NULL) {
    return NULL;
  }
  if (fseek(f, 0, SEEK_END) || (file_size_signed = ftell(f)) == -1 || fseek(f, 0, SEEK_SET)) {
    fclose(f);
    return NULL;
  }
  file_size = (int)file_size_signed;
  myReAlloc(file_data, file_size + padding_bytes);
  if (file_data == NULL) {
    fclose(f);
    return NULL;
  }
  if (fread(file_data, 1, (size_t)file_size, f) != (size_t)file_size) {
    fclose(f);
    myMemFree(file_data);
    return NULL;
  }
  if (padding_bytes > 0) {
    memset((void*)(((char*)file_data) + file_size), 0, padding_bytes);
  }
  fclose(f);
  if (out_file_size) {
    *out_file_size = file_size;
  }
  return file_data;
}
void ImGuiIO_AddInputCharacter(ImGui* io, ImWchar c)
{
  const int n = ImStrlenW(io->InputCharacters);
  if (n + 1 < IM_ARRAYSIZE(io->InputCharacters)) {
    io->InputCharacters[n] = c;
    io->InputCharacters[n + 1] = '\0';
  }
}
void ImGuiIO_AddInputCharactersUTF8(ImGui* io, const char* utf8_chars)
{
  int i;
  // We can't pass more wchars than ImGui::InputCharacters[] can hold so don't convert more
  enum { wchars_buf_len = IM_ARRAYSIZE(io->InputCharacters) };
  ImWchar wchars[wchars_buf_len];
  TextStrFromUtf8(wchars, wchars_buf_len, utf8_chars, NULL, NULL);
  for (i = 0; i < wchars_buf_len && wchars[i] != 0; i++) {
    ImGuiIO_AddInputCharacter(io, wchars[i]);
  }
}
unsigned int Font_hash(const char* name, int len, int size)
{
  // SDBMHash
  unsigned int hash = (unsigned int)size;
  int i = 0;
  len = len <= 0 ? strlen(name) : len;
  for (i = 0; i < len; name++, i++) {
    hash = 65599 * hash + (*name);
  }
  return hash;
}
ISIZE imuimeasureText(const char* text, const char* text_end, texfont_t* font_id, int formatFlags, IRECT* prc)
{
  GETGUIIO1();
  ISIZE sz = {0};
  if (host->DrvMeasureText) {
    IRECT rc = prc ? *prc : iRECT2(0, 0, 10000, 10000);
    host->DrvMeasureText(host, text, text_end, font_id, formatFlags, &rc);
    sz = iSIZE(RCW(&rc), RCH(&rc));
    if (prc) {
      *prc = rc;
    }
  }
  return sz;
}

int ImGuiHost_Add(ImGuiHost* host1)
{
  GETGUIIO1();
  host1->prev = NULL;
  host1->next = io->HeadHost;
  if (io->HeadHost) {
    io->HeadHost->prev = host1;
  }
  io->HeadHost = host1;
  return 0;
}
int ImGuiHost_Del(ImGuiHost* host1)
{
  GETGUIIO1();
  if (host1->next) {
    host1->next->prev = NULL;
  }
  io->HeadHost = host1->next;
  host1->prev = NULL;
  host1->next = NULL;
  io->CurrentHost = NULL;
  return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////
CC_INLINE int iRectLayoutEx(IRECT rc, int cx, int cy, UINT fmt, int gap, IRECT* out, IRECT* prc, layerinfo_t* li)
{
  int xf = fmt & 0x07;
  int yf = (fmt >> 4) & 0x7;
  int retx, rety, ret = 0;
  IRECT rc2[1] = {0};
  IRECT* prc1 = (LF_alighParentLeft == yf) ? &li->rcLT : &li->rcRB;
  IRECT* prc2 = (LF_alighParentLeft == xf) ? &li->rcLT : &li->rcRB;
  int cc = (LF_alighParentLeft == xf || LF_alighParentRight == xf) && (LF_alighParentLeft == yf || LF_alighParentRight == yf);
  int cc1 = (LF_alighParentLeft == yf || LF_alighParentRight == yf);
  *prc = *out = *rc2 = rc;
  if (1) {
    if (cc && cx <= RCW(prc1)) {
      retx = iRectLayout1D(prc1->l, prc1->r, cx, xf, gap, &out->l, &out->r, &prc1->l, &prc1->r);
      if (LF_alighParentLeft == yf) {
        prc->t = MAX(prc->t, prc1->t + cy + gap);
        out->t = prc1->t;
        out->b = prc->t - gap;
      }
      else {
        prc->b = MIN(prc->b, prc1->b - cy - gap);
        out->t = prc->b + gap;
        out->b = prc1->b;
      }
      ret = 1;
      {
        int h = RCH(&li->rect) - RCH(prc);
        li->Size.y = MAX(li->Size.y, h);
        li->Size.x = MAX(li->Size.x, cx);
      }
    }
    else {
      li->rcLT = li->rcRB = iRECT(0, 0, 0, 0);
      retx = iRectLayout1D(rc.l, rc.r, cx, xf, gap, &out->l, &out->r, &rc2->l, &rc2->r);
      rety = iRectLayout1D(rc.t, rc.b, cy, yf, gap, &out->t, &out->b, &rc2->t, &rc2->b);
      if (retx && rc2->l < rc2->r && cc) {
        if (LF_alighParentLeft == yf) {
          prc1->t = rc.t;
          prc1->b = rc.t + cy;
        }
        else {
          prc1->t = rc.b - cy;
          prc1->b = rc.b;
        }
        prc1->l = rc2->l;
        prc1->r = rc2->r;
      }
      if (LF_alighParentLeft == yf) {
        prc->t += cy + gap;
        li->Size.x = MAX(li->Size.x, cx);
        li->Size.y += cy + gap;
      }
      else if (LF_alighParentRight == yf) {
        prc->b -= cy + gap;
        li->Size.x = MAX(li->Size.x, cx);
        li->Size.y += cy + gap;
      }
      else {
        if (LF_alighParentLeft == xf) {
          prc->l += cx + gap;
          li->Size.x += cx + gap;
          li->Size.y = MAX(li->Size.y, cy);
        }
        else {
          prc->r -= cx + gap;
          li->Size.x += cx + gap;
          li->Size.y = MAX(li->Size.y, cy);
        }
      }
      ret = retx || rety;
    }
  }
  li->irc += !ret;
  return ret;
}
int calc_rect(int x, int y, int fmt, IRECT* prc)
{
  GETGUIIO1();
  layerinfo_t* li = io->layinfo + io->deep - 1;
  if (prc) {
    if (li->irc < io->nrcs) {
      int ret = iRectLayoutEx(io->rcs[li->irc], x, y, fmt, io->gap, prc, &io->rcs[li->irc], li);
      return 1;
    }
  }
  return 0;
}
void set_layout_flags(int layoutflags)
{
  GETGUIIO1();
  io->layoutflags = layoutflags;
}
IRECT* layer_push(int horz, IRECT rc, int n)
{
  GETGUIIO1();
  layerinfo_t* li;
  //memset(li, 0, sizeof(*li));
  li = io->layinfo + io->deep;
  li->ilsz = host->ilsz++;
  li->Size = iPOINT(0, 0);
  li->rcLT = li->rcRB = iRECT(0, 0, 0, 0);
  li->Pos = iPOINT(rc.l, rc.t);
  li->rect = rc;
  li->horz = horz;
  li->nrcs = li->irc = io->nrcs;
  li->layoutflags = io->layoutflags;
  if (n > 0) {
    io->nrcs += n;
  }
  else {
    io->rcs[io->nrcs++] = rc;
  }
  io->deep++;
  ASSERT(io->nrcs < countof(io->rcs));
  return io->rcs + io->nrcs - n;
}
void layer_pop()
{
  GETGUIIO1();
  int x = 0, y = 0;
  layerinfo_t* li;
  ASSERT(io->deep < countof(io->layinfo));
  io->deep--;
  li = io->layinfo + io->deep;
  y = li->Size.y;
  x = li->Size.x;
  //printf("io->deep %d\n", io->deep);
  if (li->ilsz > 30) {
    int asdf = 0;
  }
  ASSERT(li->gap < 10);
  ASSERT(li->ilsz < 100);
  host->lsz[li->ilsz].sz = iPOINT(x, y);
  //calc_rect(x, y, 0, NULL);
  io->nrcs = li->nrcs;
  io->layoutflags = li->layoutflags;
}
#define force_redraw(c)
void SETHIT(void* c)
{
  GETGUIIO1();
  (io->MouseDownID[0] = c);
}
window_t* find_window_by_pt(window_t* c, int x, int y)
{
  window_t* ret = NULL;
  if (c) {
    if (ret = find_window_by_pt(c->next, x, y)) {
      return ret;
    }
    if (ret = find_window_by_pt(c->child, x, y)) {
      return ret;
    }
    if (iPtInRect(&c->rect, x, y)) {
      return c;
    }
  }
  return ret;
}
int host_begin(ImGuiHost* host1, COLOR clrbg)
{
  if (host1// && io->HotHost==host1
     ) {
    GETGUIIO1();
    window_t* c;
    IRECT rc;
    int x = io->MousePos.x, y = io->MousePos.y;
    host = host1;
    io->CurrentHost = host1;
    io->CurrentWindow = host->rootWindow;
    host->ilsz = 0;
    c = host->rootWindow;
    rc = host->rootWindow->rect;
    host->rcDirty = iRECT(0, 0, 0, 0);
    host_clear(host, clrbg);
    if (1) {
      io->nrcs = 0;
      *layer_push(0, rc, 1) = rc;
    }
    switch (io->lastMsg) {
    case MSG_LBUTTONDOWN:
      if (host->HoveredWindow == c) {
        SETHIT(c);
        SETFOCUSWIN(c);
      }
      break;
    case MSG_MOUSEMOVE:
      if (c) {
        int hot = iPtInRect(&rc, x, y);
        if (!io->MouseDown[0]) {
          host->HoveredWindow = find_window_by_pt(c, x, y);
        }
        if (host->HoveredWindow) {
          //printf("text = %s\n", host->HoveredWindow->text);
        }
        if (ISHOT(c) != hot) {
          SETHOT2(c, hot);
        }
      }
      break;
    }
    return host1->Window && host1->Driver;
  }
  return 0;
}
void host_PushAllCtx(gc_t* g, window_t* c)
{
  if (c) {
    if (c->ctx->buf && c->ctx->len > 0) {
      char* p = (char*)myAllocPush(g->buf, g->len, c->ctx->len);
      memcpy(p, c->ctx->buf, c->ctx->len);
      c->ctx->len = 0;
    }
    host_PushAllCtx(g, c->child);
    host_PushAllCtx(g, c->next);
  }
}
int host_end()
{
  GETGUIIO1();
  window_t* c = io->CurrentWindow;
  gc_t ctx[1] = {0};
  ASSERT(c == host->rootWindow);
  host_PushAllCtx(ctx, host->rootWindow);
  host_PushAllCtx(ctx, host->tooltip);
  host->DrvRander(host, ctx);
  myFree(ctx->buf, ctx->len);
  layer_pop();
  io->CurrentHost = NULL;
  io->CurrentWindow = NULL;
  return 0;
}
#include "ctrl_draw.inl"
#include "imui_ctrl.inl"
#include "res.inl"

