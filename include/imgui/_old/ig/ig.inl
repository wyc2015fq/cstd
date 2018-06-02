
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h> // toupper, isprint
#include <stdlib.h> // NULL, malloc, free, qsort, atoi
#include <stdio.h> // vsnprintf, sscanf, printf
#include <limits.h> // INT_MIN, INT_MAX
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h> // intptr_t
#define vsnprintf _vsnprintf
#else
#include <stdint.h> // intptr_t
#endif
#define ASSERT assert
#define IM_ASSERT assert
#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_PI 3.14159265358979323846f
#define IM_OFFSETOF(_TYPE,_ELM) ((size_t)&(((_TYPE*)0)->_ELM))

#ifdef __cplusplus
#define IM_STATIC static
#define IM_INLINE inline
#else
#define IM_STATIC static
#define IM_INLINE static
#define fabsf(x) (float)fabs(x)
#define sqrtf(x) (float)sqrt(x)
#define fmodf(a, b) (float)fmod(a, b)
#define cosf(x) (float)cos(x)
#define sinf(x) (float)sin(x)
#endif

enum ImGuiWindowFlags_ {
  // Default: 0
  ImGuiWindowFlags_NoTitleBar = 1 << 0, // Disable title-bar
  ImGuiWindowFlags_NoResize = 1 << 1, // Disable user resizing with the lower-right grip
  ImGuiWindowFlags_NoMove = 1 << 2, // Disable user moving the window
  ImGuiWindowFlags_NoScrollbar = 1 << 3, // Disable scrollbars (window can still scroll with mouse or programatically)
  ImGuiWindowFlags_NoScrollWithMouse = 1 << 4, // Disable user vertically scrolling with mouse wheel
  ImGuiWindowFlags_NoCollapse = 1 << 5, // Disable user collapsing window by double-clicking on it
  ImGuiWindowFlags_AlwaysAutoResize = 1 << 6, // Resize every window to its content every frame
  ImGuiWindowFlags_ShowBorders = 1 << 7, // Show borders around windows and items
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
};
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
  ImGuiKey_Tab, // for tabbing through fields
  ImGuiKey_LeftArrow, // for text edit
  ImGuiKey_RightArrow,// for text edit
  ImGuiKey_UpArrow, // for text edit
  ImGuiKey_DownArrow, // for text edit
  ImGuiKey_PageUp,
  ImGuiKey_PageDown,
  ImGuiKey_Home, // for text edit
  ImGuiKey_End, // for text edit
  ImGuiKey_Delete, // for text edit
  ImGuiKey_Backspace, // for text edit
  ImGuiKey_Enter, // for text edit
  ImGuiKey_Escape, // for text edit
  ImGuiKey_A, // for text edit CTRL+A: select all
  ImGuiKey_C, // for text edit CTRL+C: copy
  ImGuiKey_V, // for text edit CTRL+V: paste
  ImGuiKey_X, // for text edit CTRL+X: cut
  ImGuiKey_Y, // for text edit CTRL+Y: redo
  ImGuiKey_Z, // for text edit CTRL+Z: undo
  ImGuiKey_COUNT
};
// Enumeration for PushStyleColor() / PopStyleColor()
enum ImGuiCol_ {
  ImGuiCol_Text,
  ImGuiCol_TextDisabled,
  ImGuiCol_WindowBg, // Background of normal windows
  ImGuiCol_ChildWindowBg, // Background of child windows
  ImGuiCol_PopupBg, // Background of popups, menus, tooltips windows
  ImGuiCol_Border,
  ImGuiCol_BorderShadow,
  ImGuiCol_FrameBg, // Background of checkbox, radio button, plot, slider, text input
  ImGuiCol_FrameBgHovered,
  ImGuiCol_FrameBgActive,
  ImGuiCol_TitleBg,
  ImGuiCol_TitleBgCollapsed,
  ImGuiCol_TitleBgActive,
  ImGuiCol_MenuBarBg,
  ImGuiCol_ScrollbarBg,
  ImGuiCol_ScrollbarGrab,
  ImGuiCol_ScrollbarGrabHovered,
  ImGuiCol_ScrollbarGrabActive,
  ImGuiCol_ComboBg,
  ImGuiCol_CheckMark,
  ImGuiCol_SliderGrab,
  ImGuiCol_SliderGrabActive,
  ImGuiCol_Button,
  ImGuiCol_ButtonHovered,
  ImGuiCol_ButtonActive,
  ImGuiCol_Header,
  ImGuiCol_HeaderHovered,
  ImGuiCol_HeaderActive,
  ImGuiCol_Column,
  ImGuiCol_ColumnHovered,
  ImGuiCol_ColumnActive,
  ImGuiCol_ResizeGrip,
  ImGuiCol_ResizeGripHovered,
  ImGuiCol_ResizeGripActive,
  ImGuiCol_CloseButton,
  ImGuiCol_CloseButtonHovered,
  ImGuiCol_CloseButtonActive,
  ImGuiCol_PlotLines,
  ImGuiCol_PlotLinesHovered,
  ImGuiCol_PlotHistogram,
  ImGuiCol_PlotHistogramHovered,
  ImGuiCol_TextSelectedBg,
  ImGuiCol_ModalWindowDarkening, // darken entire screen when a modal window is active
  ImGuiCol_COUNT
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

typedef int BOOL;
typedef unsigned int ImU32;
typedef unsigned short ImDrawIdx;
typedef unsigned int ImGuiID;
typedef unsigned short ImWchar;
typedef void* ImTextureID;
typedef struct ImGuiTextEditCallbackData ImGuiTextEditCallbackData;
typedef struct ImGuiSizeConstraintCallbackData ImGuiSizeConstraintCallbackData;
typedef int ImGuiCol; // a color identifier for styling // enum ImGuiCol_
typedef int ImGuiStyleVar; // a variable identifier for styling // enum ImGuiStyleVar_
typedef int ImGuiKey; // a key identifier (ImGui-side enum) // enum ImGuiKey_
typedef int ImGuiColorEditMode; // color edit mode for ColorEdit*() // enum ImGuiColorEditMode_
typedef int ImGuiMouseCursor; // a mouse cursor identifier // enum ImGuiMouseCursor_
typedef int ImGuiWindowFlags; // window flags for Begin*() // enum ImGuiWindowFlags_
typedef int ImGuiSetCond; // condition flags for Set*() // enum ImGuiSetCond_
typedef int ImGuiInputTextFlags; // flags for InputText*() // enum ImGuiInputTextFlags_
typedef int ImGuiSelectableFlags; // flags for Selectable() // enum ImGuiSelectableFlags_
typedef int ImGuiTreeNodeFlags; // flags for TreeNode*(), Collapsing*() // enum ImGuiTreeNodeFlags_
typedef int (*ImGuiTextEditCallback)(ImGuiTextEditCallbackData* data);
typedef void (*ImGuiSizeConstraintCallback)(ImGuiSizeConstraintCallbackData* data);

typedef struct ImVec2 {
  float x, y;
} ImVec2;

ImVec2 fVec2(float _x, float _y) {
  ImVec2 v;
  v.x = _x;
  v.y = _y;
  return v;
}
typedef struct ImVec4 {
  float x, y, z, w;
} ImVec4;

ImVec4 fVec4(float _x, float _y, float _z, float _w) {
  ImVec4 v;
  v.x = _x;
  v.y = _y;
  v.z = _z;
  v.w = _w;
  return v;
}
typedef struct ImFontGlyph {
  ImWchar Codepoint;
  float XAdvance;
  float X0, Y0, X1, Y1;
  float U0, V0, U1, V1; // Texture coordinates
} ImFontGlyph;

typedef struct ImFont ImFont;
typedef struct ImFontConfig ImFontConfig;
typedef struct ImFontAtlas ImFontAtlas;
struct ImFont {
  // Members: Hot ~62/78 bytes
  float FontSize; // <user set> // Height of characters, set during loading (don't change after loading)
  float Scale; // = 1.f // Base font scale, multiplied by the per-window font scale which you can adjust with SetFontScale()
  ImVec2 DisplayOffset; // = (0.f,1.f) // Offset font rendering by xx pixels
  int Glyphs_Size;
  int IndexXAdvance_Size;
  int IndexLookup_Size;
  ImFontGlyph* Glyphs; // // All glyphs.
  float* IndexXAdvance; // // Sparse. Glyphs->XAdvance in a directly indexable way (more cache-friendly, for CalcTextSize functions which are often bottleneck in large UI).
  unsigned short* IndexLookup; // // Sparse. Index glyphs by Unicode code-point.
  ImFontGlyph* FallbackGlyph; // == FindGlyph(FontFallbackChar)
  float FallbackXAdvance; // == FallbackGlyph->XAdvance
  ImWchar FallbackChar; // = '?' // Replacement glyph if one isn't found. Only set via SetFallbackChar()
  // Members: Cold ~18/26 bytes
  short ConfigDataCount; // ~ 1 // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
  ImFontConfig* ConfigData; // // Pointer within ContainerAtlas->ConfigData
  ImFontAtlas* ContainerAtlas; // // What we has been loaded into
  float Ascent, Descent; // // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
  // Methods
};
struct ImFontConfig {
  void* FontData; // // TTF data
  int FontDataSize; // // TTF data size
  BOOL FontDataOwnedByAtlas; // true // TTF data ownership taken by the container ImFontAtlas (will delete memory itself). Set to true
  int FontNo; // 0 // Index of font within TTF file
  float SizePixels; // // Size in pixels for rasterizer
  int OversampleH, OversampleV; // 3, 1 // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
  BOOL PixelSnapH; // false // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
  ImVec2 GlyphExtraSpacing; // 0, 0 // Extra spacing (in pixels) between glyphs
  ImWchar* GlyphRanges; // // Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
  BOOL MergeMode; // false // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs).
  BOOL MergeGlyphCenterV; // false // When merging (multiple ImFontInput for one ImFont), vertically center new glyphs instead of aligning their baseline
  // [Internal]
  char Name[32]; // Name (strictly for debugging)
  ImFont* DstFont;
};
struct ImFontAtlas {
  // (Access texture data via GetTexData*() calls which will setup a default font for you.)
  void* TexID; // User data to refer to the texture once it has been uploaded to user's graphic systems. It ia passed back to you during rendering.
  unsigned char* TexPixelsAlpha8; // 1 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight
  unsigned int* TexPixelsRGBA32; // 4 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight * 4
  int TexWidth; // Texture width calculated during Build().
  int TexHeight; // Texture height calculated during Build().
  int TexDesiredWidth; // Texture width desired by user before Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.
  ImVec2 TexUvWhitePixel; // Texture coordinates to a white pixel
  ImFont** Fonts; // Hold all the fonts returned by AddFont*. Fonts[0] is the default font upon calling ImGui_NewFrame(), use ImGui_PushFont()/PopFont() to change the current font.
  int Fonts_Size;
  // Private
  ImFontConfig* ConfigData; // Internal data
  int ConfigData_Size;
};

typedef struct ImDrawCmd ImDrawCmd;
typedef struct ImDrawList ImDrawList;
typedef struct ImDrawData ImDrawData;
struct ImDrawData {
  BOOL Valid;
  ImDrawList** CmdLists;
  int CmdListsCount;
  int TotalVtxCount;
  int TotalIdxCount;
};
typedef struct ImGuiIO {
  //------------------------------------------------------------------
  // Settings (fill once) // Default value:
  //------------------------------------------------------------------
  ImVec2 DisplaySize; // <unset> // Display size, in pixels. For clamping windows positions.
  float DeltaTime; // = 1.0f/60.0f // Time elapsed since last frame, in seconds.
  float IniSavingRate; // = 5.0f // Maximum time between saving positions/sizes to .ini file, in seconds.
  const char* IniFilename; // = "imgui.ini" // Path to .ini file. NULL to disable .ini saving.
  const char* LogFilename; // = "imgui_log.txt" // Path to .log file (default parameter to ImGui_LogToFile when no file is specified).
  float MouseDoubleClickTime; // = 0.30f // Time for a double-click, in seconds.
  float MouseDoubleClickMaxDist; // = 6.0f // Distance threshold to stay in to validate a double-click, in pixels.
  float MouseDragThreshold; // = 6.0f // Distance threshold before considering we are dragging
  int KeyMap[ImGuiKey_COUNT]; // <unset> // Map of indices into the KeysDown[512] entries array
  float KeyRepeatDelay; // = 0.250f // When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).
  float KeyRepeatRate; // = 0.020f // When holding a key/button, rate at which it repeats, in seconds.
  void* UserData; // = NULL // Store your own data for retrieval by callbacks.
  ImFontAtlas* Fonts; // <auto> // Load and assemble one or more fonts into a single tightly packed texture. Output to Fonts array.
  float FontGlobalScale; // = 1.0f // Global scale all fonts
  BOOL FontAllowUserScaling; // = false // Allow user scaling text of individual window with CTRL+Wheel.
  ImVec2 DisplayFramebufferScale; // = (1.0f,1.0f) // For retina display or other situations where window coordinates are different from framebuffer coordinates. User storage only, presently not used by ImGui.
  ImVec2 DisplayVisibleMin; // <unset> (0.0f,0.0f) // If you use DisplaySize as a virtual space larger than your screen, set DisplayVisibleMin/Max to the visible area.
  ImVec2 DisplayVisibleMax; // <unset> (0.0f,0.0f) // If the values are the same, we defaults to Min=(0.0f) and Max=DisplaySize
  // Advanced/subtle behaviors
  BOOL OSXBehaviors; // = defined(__APPLE__) // OS X style: Text editing cursor movement using Alt instead of Ctrl, Shortcuts using Cmd/Super instead of Ctrl, Line/Text Start and End using Cmd+Arrows instead of Home/End, Double click selects by word instead of selecting whole text, Multi-selection in lists uses Cmd/Super instead of Ctrl
  
  ImVec2 MousePos; // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
  BOOL MouseDown[5]; // Mouse buttons: left, right, middle + extras. ImGui itself mostly only uses left button (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.
  float MouseWheel; // Mouse wheel: 1 unit scrolls about 5 lines text.
  BOOL MouseDrawCursor; // Request ImGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor).
  BOOL KeyCtrl; // Keyboard modifier pressed: Control
  BOOL KeyShift; // Keyboard modifier pressed: Shift
  BOOL KeyAlt; // Keyboard modifier pressed: Alt
  BOOL KeySuper; // Keyboard modifier pressed: Cmd/Super/Windows
  BOOL KeysDown[512]; // Keyboard keys that are pressed (in whatever storage order you naturally have access to keyboard data)
  ImWchar InputCharacters[16 + 1]; // List of characters input (translated by user from keypress+keyboard state). Fill using AddInputCharacter() helper.
  
  //------------------------------------------------------------------
  // Output - Retrieve after calling NewFrame(), you can use them to discard inputs or hide them from the rest of your application
  //------------------------------------------------------------------
  BOOL WantCaptureMouse; // Mouse is hovering a window or widget is active (= ImGui will use your mouse input)
  BOOL WantCaptureKeyboard; // Widget is active (= ImGui will use your keyboard input)
  BOOL WantTextInput; // Some text input widget is active, which will read input characters from the InputCharacters array.
  float Framerate; // Framerate estimation, in frame per second. Rolling average estimation based on IO.DeltaTime over 120 frames
  int MetricsAllocs; // Number of active memory allocations
  int MetricsRenderVertices; // Vertices output during last call to Render()
  int MetricsRenderIndices; // Indices output during last call to Render() = number of triangles * 3
  int MetricsActiveWindows; // Number of visible windows (exclude child windows)
  //------------------------------------------------------------------
  // [Internal] ImGui will maintain those fields for you
  //------------------------------------------------------------------
  ImVec2 MousePosPrev; // Previous mouse position
  ImVec2 MouseDelta; // Mouse delta. Note that this is zero if either current or previous position are negative to allow mouse enabling/disabling.
  BOOL MouseClicked[5]; // Mouse button went from !Down to Down
  ImVec2 MouseClickedPos[5]; // Position at time of clicking
  float MouseClickedTime[5]; // Time of last click (used to figure out double-click)
  BOOL MouseDoubleClicked[5]; // Has mouse button been double-clicked?
  BOOL MouseReleased[5]; // Mouse button went from Down to !Down
  BOOL MouseDownOwned[5]; // Track if button was clicked inside a window. We don't request mouse capture from the application if click started outside ImGui bounds.
  float MouseDownDuration[5]; // Duration the mouse button has been down (0.0f == just clicked)
  float MouseDownDurationPrev[5]; // Previous time the mouse button has been down
  float MouseDragMaxDistanceSqr[5]; // Squared maximum distance of how much mouse has traveled from the click point
  float KeysDownDuration[512]; // Duration the keyboard key has been down (0.0f == just pressed)
  float KeysDownDurationPrev[512]; // Previous duration the key has been down
  
  //------------------------------------------------------------------
  // User Functions
  //------------------------------------------------------------------
  void (*RenderDrawListsFn)(ImDrawData* data);
  // Optional: access OS clipboard
  // (default to use native Win32 clipboard on Windows, otherwise uses a private clipboard. Override to access OS clipboard on other architectures)
  const char* (*GetClipboardTextFn)();
  void (*SetClipboardTextFn)(const char* text);
  // Optional: override memory allocations. MemFreeFn() may be called with a NULL pointer.
  // (default to posix malloc/free)
  void* (*MemAllocFn)(size_t sz);
  void (*MemFreeFn)(void* ptr);
  // Optional: notify OS Input Method Editor of the screen position of your cursor for text input position (e.g. when using Japanese/Chinese IME in Windows)
  // (default to use native imm32 api on Windows)
  void (*ImeSetInputScreenPosFn)(int x, int y);
  void* ImeWindowHandle; // (Windows) Set this to your HWND to get automatic IME cursor positioning.
} ImGuiIO;
typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);

typedef struct ImDrawVert {
  ImVec2 pos;
  ImVec2 uv;
  ImU32 col;
} ImDrawVert;

typedef struct ImDrawCmd {
  unsigned int ElemCount; // Number of indices (multiple of 3) to be rendered as triangles. Vertices are stored in the callee ImDrawList's vtx_buffer[] array, indices in idx_buffer[].
  ImVec4 ClipRect; // Clipping rectangle (x1, y1, x2, y2)
  ImTextureID TextureId; // User-provided texture ID. Set by user in ImfontAtlas::SetTexID() for fonts or passed to Image*() functions. Ignore if never using images or multiple fonts atlas.
  ImDrawCallback UserCallback; // If != NULL, call the function instead of rendering the vertices. clip_rect and texture_id will be set normally.
  void* UserCallbackData; // The draw callback code can access this.
} ImDrawCmd;

typedef struct ImDrawChannel {
  ImDrawCmd* CmdBuffer;
  ImDrawIdx* IdxBuffer;
  int CmdBuffer_Size;
  int IdxBuffer_Size;
} ImDrawChannel;
typedef struct ImDrawList {
  ImDrawCmd* CmdBuffer;
  ImDrawIdx* IdxBuffer;
  ImDrawVert* VtxBuffer;
  int CmdBuffer_Size;
  int IdxBuffer_Size;
  int VtxBuffer_Size;
  // [Internal, used while building lists]
  const char* _OwnerName; // Pointer to owner window's name (if any) for debugging
  unsigned int _VtxCurrentIdx; // [Internal] == VtxBuffer.Size
  ImDrawVert* _VtxWritePtr; // [Internal] point within VtxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
  ImDrawIdx* _IdxWritePtr; // [Internal] point within IdxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
  ImVec4* _ClipRectStack; // [Internal]
  ImTextureID* _TextureIdStack; // [Internal]
  ImVec2* _Path; // [Internal] current path building
  int _ChannelsCurrent; // [Internal] current channel number (0)
  int _ChannelsCount; // [Internal] number of active channels (1+)
  ImDrawChannel* _Channels;
  int _Channels_Size;
  int _TextureIdStack_Size;
  int _ClipRectStack_Size;
  int _Path_Size;
} ImDrawList;

ImGuiIO* ImGui_GetIO() {
  static ImGuiIO io[1] = {0};
  return io;
}

#define ImMin(a, b)   ((a)<(b)?(a):(b))
#define ImMax(a, b)   ((a)>(b)?(a):(b))
#define ImClamp(v, mn, mx)   (((v) < (mn)) ? (mn) : ((v) > (mx)) ? (mx) : (v))
//-----------------------------------------------------------------------------
void ImGui_MemAlloc_(void* ptr, int sz) {
  void** pptr = (void**)ptr;
  *pptr = realloc(*pptr, sz);
}
#define ImGui_MemAlloc(ptr, sz)   ImGui_MemAlloc_(&ptr, sizeof(*ptr)*sz)
#define ImGui_MemFree(ptr)  if (ptr) { free(ptr); ptr = NULL; }
#define myImGui_AllocAdd(ptr, oldsz, sz)  (ImGui_MemAlloc(ptr, sz), oldsz = sz, memset(ptr + oldsz - sz, 0, sizeof(*ptr)*(sz)), ptr + oldsz - sz )
#define myImGui_Free(ptr, oldsz)  {ImGui_MemFree(ptr); oldsz = 0; }
#define ImVec2_mul_s(lhs, rhs)  fVec2(lhs.x * rhs, lhs.y * rhs)
#define ImVec2_div_s(lhs, rhs)  fVec2(lhs.x / rhs, lhs.y / rhs)
#define ImVec2_add_s(lhs, rhs)  fVec2(lhs.x + rhs.x, lhs.y + rhs.y)
#define ImVec2_sub_s(lhs, rhs)  fVec2(lhs.x - rhs.x, lhs.y - rhs.y)
#define ImVec2_mul(lhs, rhs)  fVec2(lhs.x * rhs.x, lhs.y * rhs.y)
#define ImVec2_div(lhs, rhs)  fVec2(lhs.x / rhs.x, lhs.y / rhs.y)
#define ImVec2_add_to(lhs, rhs)  (lhs.x += rhs.x, lhs.y += rhs.y)
#define ImVec2_sub_to(lhs, rhs)  (lhs.x -= rhs.x, lhs.y -= rhs.y, lsh)
#define ImVec2_mul_s_to(lhs, rhs) (lhs.x *= rhs, lhs.y *= rhs, lsh)
#define ImVec2_div_s_to(lhs, rhs) (lhs.x /= rhs, lhs.y /= rhs, lsh)
#define ImVec4_sub(lhs, rhs) fVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w)
#define ImVec2_Min(lhs, rhs) fVec2(ImMin(lhs.x, rhs.x), ImMin(lhs.y, rhs.y))
#define ImVec2_Max(lhs, rhs) fVec2(ImMax(lhs.x, rhs.x), ImMax(lhs.y, rhs.y))
#define ImVec2_Clamp(f, mn, mx) fVec2(ImClamp(f.x, mn.x, mx.x), ImClamp(f.y, mn.y, mx.y))

IM_INLINE float ImSaturate(float f)
{
  return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f;
}
IM_INLINE float ImLerp(float a, float b, float t)
{
  return a + (b - a) * t;
}
#define ImVec2_ImLerp(a, b, t)  fVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y)
#define ImVec2_LengthSqr(lhs)   (lhs.x * lhs.x + lhs.y * lhs.y)
#define ImVec4_ImLengthSqr(lhs) (lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z + lhs.w * lhs.w)
IM_INLINE float ImVec2_InvLength(ImVec2 lhs, float fail_value)
{
  float d = lhs.x * lhs.x + lhs.y * lhs.y;
  if (d > 0.0f) {
    return 1.0f / sqrtf(d);
  }
  return fail_value;
}
IM_INLINE float ImFloor(float f)
{
  return (float)(int)f;
}
IM_INLINE ImVec2 ImFloorV2(ImVec2 v)
{
  return fVec2((float)(int)v.x, (float)(int)v.y);
}
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
#define ImIsPointInTriangle(p, a, b, c)  ImIsPointInTriangle_f32(p.x, p.y, a.x, a.y, b.x, b.y, c.x, c.y)
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
  ImGui_MemAlloc(buff, len);
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
//-----------------------------------------------------------------------------
// ImText* helpers
//-----------------------------------------------------------------------------
// Convert UTF-8 to 32-bits character, process single character input.
// Based on stb_from_utf8() from github.com/nothings/stb/
// We handle UTF-8 decoding error by skipping forward.
int ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
  unsigned int c = (unsigned int) - 1;
  const unsigned char* str = (const unsigned char*)in_text;

  if (!(*str & 0x80)) {
    c = (unsigned int)(*str++);
    *out_char = c;
    return 1;
  }

  if ((*str & 0xe0) == 0xc0) {
    *out_char = 0xFFFD; // will be invalid but not end of string

    if (in_text_end && in_text_end - (const char*)str < 2) {
      return 1;
    }

    if (*str < 0xc2) {
      return 2;
    }

    c = (unsigned int)((*str++ & 0x1f) << 6);

    if ((*str & 0xc0) != 0x80) {
      return 2;
    }

    c += (*str++ & 0x3f);
    *out_char = c;
    return 2;
  }

  if ((*str & 0xf0) == 0xe0) {
    *out_char = 0xFFFD; // will be invalid but not end of string

    if (in_text_end && in_text_end - (const char*)str < 3) {
      return 1;
    }

    if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) {
      return 3;
    }

    if (*str == 0xed && str[1] > 0x9f) {
      return 3;  // str[1] < 0x80 is checked below
    }

    c = (unsigned int)((*str++ & 0x0f) << 12);

    if ((*str & 0xc0) != 0x80) {
      return 3;
    }

    c += (unsigned int)((*str++ & 0x3f) << 6);

    if ((*str & 0xc0) != 0x80) {
      return 3;
    }

    c += (*str++ & 0x3f);
    *out_char = c;
    return 3;
  }

  if ((*str & 0xf8) == 0xf0) {
    *out_char = 0xFFFD; // will be invalid but not end of string

    if (in_text_end && in_text_end - (const char*)str < 4) {
      return 1;
    }

    if (*str > 0xf4) {
      return 4;
    }

    if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) {
      return 4;
    }

    if (*str == 0xf4 && str[1] > 0x8f) {
      return 4;  // str[1] < 0x80 is checked below
    }

    c = (unsigned int)((*str++ & 0x07) << 18);

    if ((*str & 0xc0) != 0x80) {
      return 4;
    }

    c += (unsigned int)((*str++ & 0x3f) << 12);

    if ((*str & 0xc0) != 0x80) {
      return 4;
    }

    c += (unsigned int)((*str++ & 0x3f) << 6);

    if ((*str & 0xc0) != 0x80) {
      return 4;
    }

    c += (*str++ & 0x3f);

    // utf-8 encodings of values used in surrogate pairs are invalid
    if ((c & 0xFFFFF800) == 0xD800) {
      return 4;
    }

    *out_char = c;
    return 4;
  }

  *out_char = 0;
  return 0;
}
int ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining)
{
  ImWchar* buf_out = buf;
  ImWchar* buf_end = buf + buf_size;

  while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c;
    in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);

    if (c == 0) {
      break;
    }

    if (c < 0x10000) { // FIXME: Losing characters that don't fit in 2 bytes
      *buf_out++ = (ImWchar)c;
    }
  }

  *buf_out = 0;

  if (in_text_remaining) {
    *in_text_remaining = in_text;
  }

  return (int)(buf_out - buf);
}
int ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end)
{
  int char_count = 0;

  while ((!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c;
    in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);

    if (c == 0) {
      break;
    }

    if (c < 0x10000) {
      char_count++;
    }
  }

  return char_count;
}
// Based on stb_to_utf8() from github.com/nothings/stb/
IM_INLINE int ImTextCharToUtf8(char* buf, int buf_size, unsigned int c)
{
  if (c < 0x80) {
    buf[0] = (char)c;
    return 1;
  }

  if (c < 0x800) {
    if (buf_size < 2) {
      return 0;
    }

    buf[0] = (char)(0xc0 + (c >> 6));
    buf[1] = (char)(0x80 + (c & 0x3f));
    return 2;
  }

  if (c >= 0xdc00 && c < 0xe000) {
    return 0;
  }

  if (c >= 0xd800 && c < 0xdc00) {
    if (buf_size < 4) {
      return 0;
    }

    buf[0] = (char)(0xf0 + (c >> 18));
    buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
    buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
    buf[3] = (char)(0x80 + ((c) & 0x3f));
    return 4;
  }

  //else if (c < 0x10000)
  {
    if (buf_size < 3) {
      return 0;
    }

    buf[0] = (char)(0xe0 + (c >> 12));
    buf[1] = (char)(0x80 + ((c >> 6) & 0x3f));
    buf[2] = (char)(0x80 + ((c) & 0x3f));
    return 3;
  }
}
IM_INLINE int ImTextCountUtf8BytesFromChar(unsigned int c)
{
  if (c < 0x80) {
    return 1;
  }

  if (c < 0x800) {
    return 2;
  }

  if (c >= 0xdc00 && c < 0xe000) {
    return 0;
  }

  if (c >= 0xd800 && c < 0xdc00) {
    return 4;
  }

  return 3;
}
int ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end)
{
  char* buf_out = buf;
  const char* buf_end = buf + buf_size;

  while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c = (unsigned int)(*in_text++);

    if (c < 0x80) {
      *buf_out++ = (char)c;
    }
    else {
      buf_out += ImTextCharToUtf8(buf_out, (int)(buf_end - buf_out - 1), c);
    }
  }

  *buf_out = 0;
  return (int)(buf_out - buf);
}
int ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end)
{
  int bytes_count = 0;

  while ((!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c = (unsigned int)(*in_text++);

    if (c < 0x80) {
      bytes_count++;
    }
    else {
      bytes_count += ImTextCountUtf8BytesFromChar(c);
    }
  }

  return bytes_count;
}
ImVec4 ImGui_ColorConvertU32ToFloat4(ImU32 in)
{
  float s = 1.0f / 255.0f;
  return fVec4((in & 0xFF) * s, ((in >> 8) & 0xFF) * s, ((in >> 16) & 0xFF) * s, (in >> 24) * s);
}
ImU32 ImGui_ColorConvertFloat4ToU32(ImVec4 in)
{
  ImU32 out;
  out = ((ImU32)IM_F32_TO_INT8_SAT(in.x));
  out |= ((ImU32)IM_F32_TO_INT8_SAT(in.y)) << 8;
  out |= ((ImU32)IM_F32_TO_INT8_SAT(in.z)) << 16;
  out |= ((ImU32)IM_F32_TO_INT8_SAT(in.w)) << 24;
  return out;
}
// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void ImGui_ColorConvertRGBtoHSV(float r, float g, float b, float* out_h, float* out_s, float* out_v)
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
void ImGui_ColorConvertHSVtoRGB(float h, float s, float v, float* out_r, float* out_g, float* out_b)
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
// Memory allocated with ImGui_MemAlloc(), must be freed by user using ImGui_MemFree()
void* ImLoadFileToMemory(const char* filename, const char* file_open_mode, int* out_file_size, int padding_bytes)
{
  FILE* f = NULL;
  long file_size_signed=0;
  int file_size=0;
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
  ImGui_MemAlloc(file_data, file_size + padding_bytes);

  if (file_data == NULL) {
    fclose(f);
    return NULL;
  }

  if (fread(file_data, 1, (size_t)file_size, f) != (size_t)file_size) {
    fclose(f);
    ImGui_MemFree(file_data);
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


void ImGuiIO_AddInputCharacter(ImGuiIO* g, ImWchar c)
{
  const int n = ImStrlenW(g->InputCharacters);
  
  if (n + 1 < IM_ARRAYSIZE(g->InputCharacters)) {
    g->InputCharacters[n] = c;
    g->InputCharacters[n + 1] = '\0';
  }
}


void ImGuiIO_AddInputCharactersUTF8(ImGuiIO* g, const char* utf8_chars)
{
  int i;
  // We can't pass more wchars than ImGuiIO_InputCharacters[] can hold so don't convert more
  enum { wchars_buf_len = IM_ARRAYSIZE(g->InputCharacters) };
  ImWchar wchars[wchars_buf_len];
  ImTextStrFromUtf8(wchars, wchars_buf_len, utf8_chars, NULL, NULL);
  
  for (i = 0; i < wchars_buf_len && wchars[i] != 0; i++) {
    ImGuiIO_AddInputCharacter(g, wchars[i]);
  }
}
#if 0
ImU32 ImGui_GetColorU32(ImGuiCol idx, float alpha_mul)
{
  ImVec4 c = GImGui->Style.Colors[idx];
  c.w *= GImGui->Style.Alpha * alpha_mul;
  return ImGui_ColorConvertFloat4ToU32(c);
}
ImU32 ImGui_GetColorU32(const ImVec4& col)
{
  ImVec4 c = col;
  c.w *= GImGui->Style.Alpha;
  return ImGui_ColorConvertFloat4ToU32(c);
}

#endif
