
#ifndef _GUI_BASIC_HPP_
#define _GUI_BASIC_HPP_

#if _MSC_VER < 1300
#define constexpr
#define noexcept
#define nullptr NULL
#define lround(x) (int)(x+0.5)
#define override
#define DISABLE_D2D
#pragma warning(disable:4786)
#endif

#define MIN(a, b)       ((a)<(b) ? (a) : (b))
#define MAX(a, b)       ((a)>(b) ? (a) : (b))
#define CLAMP(a, amin, amax) ((a)<(amin) ? (amin) : (a)>(amax) ? (amax) : (a))
#define countof(arr)  (sizeof(arr)/sizeof(*(arr)))

#define NSBEGIN(x) namespace x {
#define NSEND() }

typedef float XYPOSITION;
typedef double XYACCUMULATOR;

//#include <inttypes.h>
#include "auto_inttypes.h"

typedef unsigned char BYTE;
typedef unsigned char uchar;
typedef void* FontID;
typedef void* SurfaceID;
typedef void* WindowID;
typedef void* MenuID;
typedef void* TickerID;
typedef void* Function;
typedef void* IdlerID;


/**
* A geometric point class.
* Point is similar to the Win32 POINT and GTK+ GdkPoint types.
*/
class Point
{
public:
  int x;
  int y;

constexpr explicit Point(int x_ = 0, int y_ = 0) noexcept :
  x(x_), y(y_) {}
// Other automatically defined methods (assignment, copy constructor, destructor) are fine
  static Point FromInts(int x_, int y_) noexcept {
    return Point(static_cast<int>(x_), static_cast<int>(y_));
  }
};
class PointF
{
public:
  XYPOSITION x;
  XYPOSITION y;

constexpr explicit PointF(XYPOSITION x_ = 0, XYPOSITION y_ = 0) noexcept :
  x(x_), y(y_) {}
constexpr explicit PointF(int x_, int y_) noexcept :
  x((XYPOSITION)x_), y((XYPOSITION)y_) {}

  static PointF FromInts(int x_, int y_) noexcept {
    return PointF(static_cast<XYPOSITION>(x_), static_cast<XYPOSITION>(y_));
  }

// Other automatically defined methods (assignment, copy constructor, destructor) are fine
};

class Size
{
public:
  union { int cx; int w; };
  union { int cy; int h; };
  
  constexpr explicit Size(int w_ = 0, int h_ = 0) noexcept :
  w(w_), h(h_) {}
};
class SizeF
{
public:
  union { float cx; float w; };
  union { float cy; float h; };
  
  constexpr explicit SizeF(float w_ = 0, float h_ = 0) noexcept :
  w(w_), h(h_) {}
};
/**
* A geometric rectangle class.
* RectF is similar to Win32 RECT.
* PRectangles contain their top and left sides, but not their right and bottom sides.
*/
class RectF
{
public:
  XYPOSITION left;
  XYPOSITION top;
  XYPOSITION right;
  XYPOSITION bottom;

constexpr explicit RectF(XYPOSITION left_ = 0, XYPOSITION top_ = 0, XYPOSITION right_ = 0, XYPOSITION bottom_ = 0) noexcept :
  left(left_), top(top_), right(right_), bottom(bottom_) {
  }

  static RectF FromInts(int left_, int top_, int right_, int bottom_) noexcept {
    return RectF(static_cast<XYPOSITION>(left_), static_cast<XYPOSITION>(top_),
    static_cast<XYPOSITION>(right_), static_cast<XYPOSITION>(bottom_));
  }

// Other automatically defined methods (assignment, copy constructor, destructor) are fine

  bool operator==(const RectF & rc) const noexcept {
    return (rc.left == left) && (rc.right == right) &&
           (rc.top == top) && (rc.bottom == bottom);
  }
  bool Contains(Point pt) const noexcept {
    return (pt.x >= left) && (pt.x <= right) &&
           (pt.y >= top) && (pt.y <= bottom);
  }
  bool ContainsWholePixel(Point pt) const noexcept {
    // Does the rectangle contain all of the pixel to left/below the point
    return (pt.x >= left) && ((pt.x + 1) <= right) &&
           (pt.y >= top) && ((pt.y + 1) <= bottom);
  }
  bool Contains(RectF rc) const noexcept {
    return (rc.left >= left) && (rc.right <= right) &&
           (rc.top >= top) && (rc.bottom <= bottom);
  }
  bool Intersects(RectF other) const noexcept {
    return (right > other.left) && (left < other.right) &&
           (bottom > other.top) && (top < other.bottom);
  }
  void Move(XYPOSITION xDelta, XYPOSITION yDelta) noexcept {
    left += xDelta;
    top += yDelta;
    right += xDelta;
    bottom += yDelta;
  }
  XYPOSITION Width() const noexcept {
    return right - left;
  }
  XYPOSITION Height() const noexcept {
    return bottom - top;
  }
  bool Empty() const noexcept {
    return (Height() <= 0) || (Width() <= 0);
  }
};

/**
* Holds an RGB colour with 8 bits for each component.
*/
constexpr const float componentMaximum = 255.0f;
typedef uint8_t BYTE;
typedef uint32_t ARGB;
struct Color {
  ARGB Argb;
  enum {
    AlphaShift = 24,
    RedShift = 16,
    GreenShift = 8,
    BlueShift = 0
  };

  enum {
    AlphaMask = 0xff000000,
    RedMask = 0x00ff0000,
    GreenMask = 0x0000ff00,
    BlueMask = 0x000000ff
  };

// Assemble A, R, G, B values into a 32-bit integer

  static ARGB MakeARGB(int a, int r, int g, int b) {
    a = CLAMP(a, 0, 255);
    r = CLAMP(r, 0, 255);
    g = CLAMP(g, 0, 255);
    b = CLAMP(b, 0, 255);
    return (((ARGB)(b) << BlueShift) |
            ((ARGB)(g) << GreenShift) |
            ((ARGB)(r) << RedShift) |
            ((ARGB)(a) << AlphaShift));
  }
  static ARGB MakeARGB(int r, int g, int b) {
    return MakeARGB(255, r, g, b);
  }
  static ARGB MakeARGB(float a, float r, float g, float b) {
    return MakeARGB((int)(a * componentMaximum), (int)(r * componentMaximum), (int)(g * componentMaximum), (int)(b * componentMaximum));
  }
  static ARGB MakeARGB(float r, float g, float b) {
    return MakeARGB(255, (int)(r * componentMaximum), (int)(g * componentMaximum), (int)(b * componentMaximum));
  }

  explicit Color(ARGB co_ = Black) noexcept : Argb(co_) {}
  Color(enum Colors co_) noexcept : Argb(co_) {}

Color(int r, int g, int b) noexcept :
  Argb(MakeARGB(r, g, b)) {
  }

Color(int a, int r, int g, int b) noexcept :
  Argb(MakeARGB(a, r, g, b)) {
  }

  void set(float r, float g, float b) noexcept {
    Argb = (MakeARGB(r, g, b));
  }

  void set(float a, float r, float g, float b) noexcept {
    Argb = (MakeARGB(a, r, g, b));
  }

  bool operator==(const Color & other) const noexcept {
    return Argb == other.Argb;
  }
  
  ARGB GetBGR() const noexcept {
    return MakeARGB(0, GetB(), GetG(), GetR());
  }
  ARGB GetRGB() const noexcept {
    return Argb&0x00ffffff;
  }
  ARGB GetABGR() const noexcept {
    return MakeARGB(255, GetB(), GetG(), GetR());
  }
  ARGB GetARGB() const noexcept {
    return Argb;
  }
  
  // Red, green and blue values as bytes 0..255
  BYTE GetAlpha() const {
    return (BYTE)(Argb >> AlphaShift);
  }
  BYTE GetA() const {
    return (BYTE)(Argb >> AlphaShift);
  }
  BYTE GetRed() const noexcept {
    return (BYTE)(Argb >> RedShift);
  }
  BYTE GetR() const noexcept {
    return (BYTE)(Argb >> RedShift);
  }
  BYTE GetGreen() const noexcept {
    return (BYTE)(Argb >> GreenShift);
  }
  BYTE GetG() const noexcept {
    return (BYTE)(Argb >> GreenShift);
  }
  BYTE GetBlue() const noexcept {
    return (BYTE)(Argb >> BlueShift);
  }
  BYTE GetB() const noexcept {
    return (BYTE)(Argb >> BlueShift);
  }

// Red, green and blue values as float 0..1.0
  float GetRedComponent() const noexcept {
    return GetRed() / componentMaximum;
  }
  float GetGreenComponent() const noexcept {
    return GetGreen() / componentMaximum;
  }
  float GetBlueComponent() const noexcept {
    return GetBlue() / componentMaximum;
  }
  
  float GetAlphaComponent() const noexcept {
    return GetAlpha() / componentMaximum;
  }
  
  Color MixedWith(Color other) const noexcept {
    const unsigned int red = (GetRed() + other.GetRed()) / 2;
    const unsigned int green = (GetGreen() + other.GetGreen()) / 2;
    const unsigned int blue = (GetBlue() + other.GetBlue()) / 2;
    const unsigned int alpha = (GetAlpha() + other.GetAlpha()) / 2;
    return Color(red, green, blue, alpha);
  }

  enum Colors {
    AliceBlue = 0xFFF0F8FF,
    AntiqueWhite = 0xFFFAEBD7,
    Aqua = 0xFF00FFFF,
    Aquamarine = 0xFF7FFFD4,
    Azure = 0xFFF0FFFF,
    Beige = 0xFFF5F5DC,
    Bisque = 0xFFFFE4C4,
    Black = 0xFF000000,
    BlanchedAlmond = 0xFFFFEBCD,
    Blue = 0xFF0000FF,
    BlueViolet = 0xFF8A2BE2,
    Brown = 0xFFA52A2A,
    BurlyWood = 0xFFDEB887,
    CadetBlue = 0xFF5F9EA0,
    Chartreuse = 0xFF7FFF00,
    Chocolate = 0xFFD2691E,
    Coral = 0xFFFF7F50,
    CornflowerBlue = 0xFF6495ED,
    Cornsilk = 0xFFFFF8DC,
    Crimson = 0xFFDC143C,
    Cyan = 0xFF00FFFF,
    DarkBlue = 0xFF00008B,
    DarkCyan = 0xFF008B8B,
    DarkGoldenrod = 0xFFB8860B,
    DarkGray = 0xFFA9A9A9,
    DarkGreen = 0xFF006400,
    DarkKhaki = 0xFFBDB76B,
    DarkMagenta = 0xFF8B008B,
    DarkOliveGreen = 0xFF556B2F,
    DarkOrange = 0xFFFF8C00,
    DarkOrchid = 0xFF9932CC,
    DarkRed = 0xFF8B0000,
    DarkSalmon = 0xFFE9967A,
    DarkSeaGreen = 0xFF8FBC8B,
    DarkSlateBlue = 0xFF483D8B,
    DarkSlateGray = 0xFF2F4F4F,
    DarkTurquoise = 0xFF00CED1,
    DarkViolet = 0xFF9400D3,
    DeepPink = 0xFFFF1493,
    DeepSkyBlue = 0xFF00BFFF,
    DimGray = 0xFF696969,
    DodgerBlue = 0xFF1E90FF,
    Firebrick = 0xFFB22222,
    FloralWhite = 0xFFFFFAF0,
    ForestGreen = 0xFF228B22,
    Fuchsia = 0xFFFF00FF,
    Gainsboro = 0xFFDCDCDC,
    GhostWhite = 0xFFF8F8FF,
    Gold = 0xFFFFD700,
    Goldenrod = 0xFFDAA520,
    Gray = 0xFF808080,
    Green = 0xFF008000,
    GreenYellow = 0xFFADFF2F,
    Honeydew = 0xFFF0FFF0,
    HotPink = 0xFFFF69B4,
    IndianRed = 0xFFCD5C5C,
    Indigo = 0xFF4B0082,
    Ivory = 0xFFFFFFF0,
    Khaki = 0xFFF0E68C,
    Lavender = 0xFFE6E6FA,
    LavenderBlush = 0xFFFFF0F5,
    LawnGreen = 0xFF7CFC00,
    LemonChiffon = 0xFFFFFACD,
    LightBlue = 0xFFADD8E6,
    LightCoral = 0xFFF08080,
    LightCyan = 0xFFE0FFFF,
    LightGoldenrodYellow = 0xFFFAFAD2,
    LightGray = 0xFFD3D3D3,
    LightGreen = 0xFF90EE90,
    LightPink = 0xFFFFB6C1,
    LightSalmon = 0xFFFFA07A,
    LightSeaGreen = 0xFF20B2AA,
    LightSkyBlue = 0xFF87CEFA,
    LightSlateGray = 0xFF778899,
    LightSteelBlue = 0xFFB0C4DE,
    LightYellow = 0xFFFFFFE0,
    Lime = 0xFF00FF00,
    LimeGreen = 0xFF32CD32,
    Linen = 0xFFFAF0E6,
    Magenta = 0xFFFF00FF,
    Maroon = 0xFF800000,
    MediumAquamarine = 0xFF66CDAA,
    MediumBlue = 0xFF0000CD,
    MediumOrchid = 0xFFBA55D3,
    MediumPurple = 0xFF9370DB,
    MediumSeaGreen = 0xFF3CB371,
    MediumSlateBlue = 0xFF7B68EE,
    MediumSpringGreen = 0xFF00FA9A,
    MediumTurquoise = 0xFF48D1CC,
    MediumVioletRed = 0xFFC71585,
    MidnightBlue = 0xFF191970,
    MintCream = 0xFFF5FFFA,
    MistyRose = 0xFFFFE4E1,
    Moccasin = 0xFFFFE4B5,
    NavajoWhite = 0xFFFFDEAD,
    Navy = 0xFF000080,
    OldLace = 0xFFFDF5E6,
    Olive = 0xFF808000,
    OliveDrab = 0xFF6B8E23,
    Orange = 0xFFFFA500,
    OrangeRed = 0xFFFF4500,
    Orchid = 0xFFDA70D6,
    PaleGoldenrod = 0xFFEEE8AA,
    PaleGreen = 0xFF98FB98,
    PaleTurquoise = 0xFFAFEEEE,
    PaleVioletRed = 0xFFDB7093,
    PapayaWhip = 0xFFFFEFD5,
    PeachPuff = 0xFFFFDAB9,
    Peru = 0xFFCD853F,
    Pink = 0xFFFFC0CB,
    Plum = 0xFFDDA0DD,
    PowderBlue = 0xFFB0E0E6,
    Purple = 0xFF800080,
    Red = 0xFFFF0000,
    RosyBrown = 0xFFBC8F8F,
    RoyalBlue = 0xFF4169E1,
    SaddleBrown = 0xFF8B4513,
    Salmon = 0xFFFA8072,
    SandyBrown = 0xFFF4A460,
    SeaGreen = 0xFF2E8B57,
    SeaShell = 0xFFFFF5EE,
    Sienna = 0xFFA0522D,
    Silver = 0xFFC0C0C0,
    SkyBlue = 0xFF87CEEB,
    SlateBlue = 0xFF6A5ACD,
    SlateGray = 0xFF708090,
    Snow = 0xFFFFFAFA,
    SpringGreen = 0xFF00FF7F,
    SteelBlue = 0xFF4682B4,
    Tan = 0xFFD2B48C,
    Teal = 0xFF008080,
    Thistle = 0xFFD8BFD8,
    Tomato = 0xFFFF6347,
    Transparent = 0x00FFFFFF,
    Turquoise = 0xFF40E0D0,
    Violet = 0xFFEE82EE,
    Wheat = 0xFFF5DEB3,
    White = 0xFFFFFFFF,
    WhiteSmoke = 0xFFF5F5F5,
    Yellow = 0xFFFFFF00,
    YellowGreen = 0xFF9ACD32
  };

};


/**
* Holds an element of a gradient with an RGBA colour and a relative position.
*/
class ColourStop
{
public:
  float position;
  Color colour;
ColourStop(float position_, Color colour_) noexcept :
  position(position_), colour(colour_) {
  }
};

//#include "img/bitmap.inl"

struct Image {
  int w;
  int h;
  BYTE* data;
  Image() : w(0), h(0), data(NULL) {}
  ~Image() {
    if (data) {
      free(data);
      data = NULL;
    }
    w = 0;
    h = 0;
  }
  int resize(int w_, int h_) {
    w = w_;
    h = h_;
    data = (BYTE*)realloc(data, w * h * 4);
    return 0;
  }
};

/**
* Font management.
*/

struct FontParameters {
  const char* faceName;
  float size;
  int weight;
  bool italic;
  int extraFontFlag;
  int technology;
  int characterSet;

  FontParameters(
    const char* faceName_,
    float size_ = 10,
    int weight_ = 400,
    bool italic_ = false,
    int extraFontFlag_ = 0,
    int technology_ = 0,
  int characterSet_ = 0) noexcept :

  faceName(faceName_),
           size(size_),
           weight(weight_),
           italic(italic_),
           extraFontFlag(extraFontFlag_),
           technology(technology_),
  characterSet(characterSet_) {
  }

};


#include "SignalSlot.inl"

struct Event {
  enum Type {
    None = 0,// Not an event.
    ActionAdded = 114,// A new action has been added(QActionEvent).
    ActionChanged = 113,// An action has been changed(QActionEvent).
    ActionRemoved = 115,// An action has been removed(QActionEvent).
    ActivationChange = 99,// A widget's top-level window activation state has changed.
    ApplicationActivate = 121,// This enum has been deprecated.Use ApplicationStateChange instead.
    ApplicationActivated = ApplicationActivate,//This enum has been deprecated.Use ApplicationStateChange instead.
    ApplicationDeactivate = 122,//This enum has been deprecated.Use ApplicationStateChange instead.
    ApplicationFontChange = 36,//The default application font has changed.
    ApplicationLayoutDirectionChange = 37,//The default application layout direction has changed.
    ApplicationPaletteChange = 38,//The default application palette has changed.
    ApplicationStateChange = 214,//The state of the application has changed.
    ApplicationWindowIconChange = 35,//The application's icon has changed.
    ChildAdded = 68,//An object gets a child(QChildEvent).
    ChildPolished = 69,//A widget child gets polished(QChildEvent).
    ChildRemoved = 71,//An object loses a child(QChildEvent).
    Clipboard = 40,//The clipboard contents have changed.
    Close = 19,//Widget was closed(QCloseEvent).
    CloseSoftwareInputPanel = 200,// A widget wants to close the software input panel(SIP).
    ContentsRectChange = 178,//The margins of the widget's content rect changed.
    ContextMenu = 82,//Context popup menu(QContextMenuEvent).
    CursorChange = 183,//The widget's cursor has changed.
    DeferredDelete = 52,//The object will be deleted after it has cleaned up(QDeferredDeleteEvent)
    DragEnter = 60,//The cursor enters a widget during a drag and drop operation(QDragEnterEvent).
    DragLeave = 62,//The cursor leaves a widget during a drag and drop operation(QDragLeaveEvent).
    DragMove = 61,//A drag and drop operation is in progress(QDragMoveEvent).
    Drop = 63,//A drag and drop operation is completed(QDropEvent).
    DynamicPropertyChange = 170,//A dynamic property was added, changed, or removed from the object.
    EnabledChange = 98,//Widget's enabled state has changed.
    Enter = 10,//Mouse enters widget's boundaries (QEnterEvent).
    EnterEditFocus = 150,// An editor widget gains focus for editing.QT_KEYPAD_NAVIGATION must be defined.
    EnterWhatsThisMode = 124,//Send to toplevel widgets when the application enters "What's This?" mode.
    Expose = 206,//Sent to a window when its on - screen contents are invalidated and need to be flushed from the backing store.
    FileOpen = 116,//File open request(QFileOpenEvent).
    FocusIn = 8,//Widget or Window gains keyboard focus(QFocusEvent).
    FocusOut = 9,//Widget or Window loses keyboard focus(QFocusEvent).
    FocusAboutToChange = 23,//Widget or Window focus is about to change(QFocusEvent)
    FontChange = 97,//Widget's font has changed.
    Gesture = 198,//A gesture was triggered(QGestureEvent).
    GestureOverride = 202,//A gesture override was triggered(QGestureEvent).
    GrabKeyboard = 188,//Item gains keyboard grab(QGraphicsItem only).
    GrabMouse = 186,//Item gains mouse grab(QGraphicsItem only).
    GraphicsSceneContextMenu = 159,//Context popup menu over a graphics scene(QGraphicsSceneContextMenuEvent).
    GraphicsSceneDragEnter = 164,//The cursor enters a graphics scene during a drag and drop operation(QGraphicsSceneDragDropEvent).
    GraphicsSceneDragLeave = 166,//The cursor leaves a graphics scene during a drag and drop operation(QGraphicsSceneDragDropEvent).
    GraphicsSceneDragMove = 165,//A drag and drop operation is in progress over a scene(QGraphicsSceneDragDropEvent).
    GraphicsSceneDrop = 167,//A drag and drop operation is completed over a scene(QGraphicsSceneDragDropEvent).
    GraphicsSceneHelp = 163,//The user requests help for a graphics scene(QHelpEvent).
    GraphicsSceneHoverEnter = 160,//The mouse cursor enters a hover item in a graphics scene(QGraphicsSceneHoverEvent).
    GraphicsSceneHoverLeave = 162,//The mouse cursor leaves a hover item in a graphics scene(QGraphicsSceneHoverEvent).
    GraphicsSceneHoverMove = 161,//The mouse cursor moves inside a hover item in a graphics scene(QGraphicsSceneHoverEvent).
    GraphicsSceneMouseDoubleClick = 158,// Mouse press again(double click) in a graphics scene(QGraphicsSceneMouseEvent).
    GraphicsSceneMouseMove = 155,//Move mouse in a graphics scene(QGraphicsSceneMouseEvent).
    GraphicsSceneMousePress = 156,//Mouse press in a graphics scene(QGraphicsSceneMouseEvent).
    GraphicsSceneMouseRelease = 157,//Mouse release in a graphics scene(QGraphicsSceneMouseEvent).
    GraphicsSceneMove = 182,//Widget was moved(QGraphicsSceneMoveEvent).
    GraphicsSceneResize = 181,//Widget was resized(QGraphicsSceneResizeEvent).
    GraphicsSceneWheel = 168,//Mouse wheel rolled in a graphics scene(QGraphicsSceneWheelEvent).
    Hide = 18,//Widget was hidden(QHideEvent).
    HideToParent = 27,//A child widget has been hidden.
    HoverEnter = 127,//The mouse cursor enters a hover widget(QHoverEvent).
    HoverLeave = 128,//The mouse cursor leaves a hover widget(QHoverEvent).
    HoverMove = 129,//The mouse cursor moves inside a hover widget(QHoverEvent).
    IconDrag = 96,//The main icon of a window has been dragged away(QIconDragEvent).
    IconTextChange = 101,//Widget's icon text has been changed. (Deprecated)
    InputMethod = 83,//An input method is being used(QInputMethodEvent).
    InputMethodQuery = 207,//A input method query event(QInputMethodQueryEvent)
    KeyboardLayoutChange = 169,//The keyboard layout has changed.
    KeyPress = 6,//Key press(QKeyEvent).
    KeyRelease = 7,//Key release(QKeyEvent).
    LanguageChange = 89,//The application translation changed.
    LayoutDirectionChange = 90,//The direction of layouts changed.
    LayoutRequest = 76,//Widget layout needs to be redone.
    Leave = 11,//Mouse leaves widget's boundaries.
    LeaveEditFocus = 151,//An editor widget loses focus for editing.QT_KEYPAD_NAVIGATION must be defined.
    LeaveWhatsThisMode = 125,//Send to toplevel widgets when the application leaves "What's This?" mode.
    LocaleChange = 88,//The system locale has changed.
    NonClientAreaMouseButtonDblClick = 176,//A mouse double click occurred outside the client area(QMouseEvent).
    NonClientAreaMouseButtonPress = 174,//A mouse button press occurred outside the client area(QMouseEvent).
    NonClientAreaMouseButtonRelease = 175,//A mouse button release occurred outside the client area(QMouseEvent).
    NonClientAreaMouseMove = 173,//A mouse move occurred outside the client area(QMouseEvent).
    MacSizeChange = 177,//The user changed his widget sizes(macOS only).
    MetaCall = 43,//An asynchronous method invocation via QMetaObject::invokeMethod().
    ModifiedChange = 102,//Widgets modification state has been changed.
    MouseButtonDblClick = 4,//Mouse press again(QMouseEvent).
    MouseButtonPress = 2,//Mouse press(QMouseEvent).
    MouseButtonRelease = 3,//Mouse release(QMouseEvent).
    MouseMove = 5,//Mouse move(QMouseEvent).
    MouseTrackingChange = 109,//The mouse tracking state has changed.
    Move = 13,//Widget's position changed (QMoveEvent).
    NativeGesture = 197,//The system has detected a gesture(QNativeGestureEvent).
    OrientationChange = 208,//The screens orientation has changes(QScreenOrientationChangeEvent).
    Paint = 12,//Screen update necessary(QPaintEvent).
    PaletteChange = 39,//Palette of the widget changed.
    ParentAboutToChange = 131,//The widget parent is about to change.
    ParentChange = 21,//The widget parent has changed.
    PlatformPanel = 212,//A platform specific panel has been requested.
    PlatformSurface = 217,//A native platform surface has been created or is about to be destroyed(QPlatformSurfaceEvent).
    Polish = 75,//The widget is polished.
    PolishRequest = 74,//The widget should be polished.
    QueryWhatsThis = 123,//The widget should accept the event if it has "What's This?" help(QHelpEvent).
    ReadOnlyChange = 106,//Widget's read-only state has changed (since Qt 5.4).
    RequestSoftwareInputPanel = 199,//A widget wants to open a software input panel(SIP).
    Resize = 14,//Widget's size changed (QResizeEvent).
    ScrollPrepare = 204,//The object needs to fill in its geometry information(QScrollPrepareEvent).
    Scroll = 205,//The object needs to scroll to the supplied position(QScrollEvent).
    Shortcut = 117,//Key press in child for shortcut key handling(QShortcutEvent).
    ShortcutOverride = 51,//Key press in child, for overriding shortcut key handling(QKeyEvent).
    Show = 17,//Widget was shown on screen(QShowEvent).
    ShowToParent = 26,//A child widget has been shown.
    SockAct = 50,//Socket activated, used to implement QSocketNotifier.
    StateMachineSignal = 192,//A signal delivered to a state machine(QStateMachine::SignalEvent).
    StateMachineWrapped = 193,//The event is a wrapper for, i.e., contains, another event(QStateMachine::WrappedEvent).
    StatusTip = 112,//A status tip is requested(QStatusTipEvent).
    StyleChange = 100,//Widget's style has been changed.
    TabletMove = 87,// Wacom tablet move(QTabletEvent).
    TabletPress = 92,//Wacom tablet press(QTabletEvent).
    TabletRelease = 93,//Wacom tablet release(QTabletEvent).
    TabletEnterProximity = 171,//Wacom tablet enter proximity event(QTabletEvent), sent to QApplication.
    TabletLeaveProximity = 172,//Wacom tablet leave proximity event(QTabletEvent), sent to QApplication.
    TabletTrackingChange = 219,//The Wacom tablet tracking state has changed(since Qt 5.9).
    ThreadChange = 22,//The object is moved to another thread.This is the last event sent to this object in the previous thread.See QObject::moveToThread().
    Timer = 1,//Regular timer events(QTimerEvent).
    ToolBarChange = 120,// The toolbar button is toggled on macOS.
    ToolTip = 110,//A tooltip was requested(QHelpEvent).
    ToolTipChange = 184,//The widget's tooltip has changed.
    TouchBegin = 194,//Beginning of a sequence of touch - screen or track - pad events(QTouchEvent).
    TouchCancel = 209,//Cancellation of touch - event sequence(QTouchEvent).
    TouchEnd = 196,//End of touch - event sequence(QTouchEvent).
    TouchUpdate = 195,//Touch - screen event(QTouchEvent).
    UngrabKeyboard = 189,//Item loses keyboard grab(QGraphicsItem only).
    UngrabMouse = 187,//Item loses mouse grab(QGraphicsItem, QQuickItem).
    UpdateLater = 78,//The widget should be queued to be repainted at a later time.
    UpdateRequest = 77,// The widget should be repainted.
    WhatsThis = 111,//The widget should reveal "What's This?" help(QHelpEvent).
    WhatsThisClicked = 118,//A link in a widget's "What's This ? " help was clicked.
    Wheel = 31,//Mouse wheel rolled(QWheelEvent).
    WinEventAct = 132,//A Windows - specific activation event has occurred.
    WindowActivate = 24,//Window was activated.
    WindowBlocked = 103,//The window is blocked by a modal dialog.
    WindowDeactivate = 25,//Window was deactivated.
    WindowIconChange = 34,//The window's icon has changed.
    WindowStateChange = 105,//The window's state (minimized, maximized or full-screen) has changed (QWindowStateChangeEvent).
    WindowTitleChange = 33,//The window title has changed.
    WindowUnblocked = 104,//The window is unblocked after a modal dialog exited.
    WinIdChange = 203,//The window system identifer for this native widget has changed.
    ZOrderChange = 126,//The widget's z-order has changed. This event is never sent to top level windows.
  };
};

struct Keyboard {
  enum Modifier {
    NoModifier = 0x00000000, // No modifier key is pressed.
    ShiftModifier = 0x02000000, // A Shift key on the keyboard is pressed.
    ControlModifier = 0x04000000, // A Ctrl key on the keyboard is pressed.
    AltModifier = 0x08000000, // An Alt key on the keyboard is pressed.
    MetaModifier = 0x10000000, // A Meta key on the keyboard is pressed.
    KeypadModifier = 0x20000000, // A keypad button is pressed.
    GroupSwitchModifier = 0x40000000, // X11 only.A Mode_switch key on the keyboard is pressed.
  };
};

struct Mouse {
enum Button {
  NoButton = 0x00000000, // The button state does not refer to any button(see MouseEvent::button()).
    AllButtons = 0x07ffffff, // This value corresponds to a mask of all possible mouse buttons.Use to set the 'acceptedButtons' property of a MouseArea to accept ALL mouse buttons.
    LeftButton = 0x00000001, // The left button is pressed, or an event refers to the left button. (The left button may be the right button on left - handed mice.)
    RightButton = 0x00000002, // The right button.
    MidButton = 0x00000004, // The middle button.
    MiddleButton = MidButton, // The middle button.
    BackButton = 0x00000008, // The 'Back' button. (Typically present on the 'thumb' side of a mouse with extra buttons.This is NOT the tilt wheel.)
    XButton1 = BackButton, // The 'Back' Button.
    ExtraButton1 = XButton1, // The 'Back' Button.
    ForwardButton = 0x00000010, // The 'Forward' Button. (Typically present beside the 'Back' button, and also pressed by the thumb.)
    XButton2 = ForwardButton, // The 'Forward Button.
    ExtraButton2 = ForwardButton, // The 'Forward' Button.
    TaskButton = 0x00000020, // The 'Task' Button.
    ExtraButton3 = TaskButton, // The 'Task' Button.
    ExtraButton4 = 0x00000040, // The 7th non - wheel Mouse Button.
    ExtraButton5 = 0x00000080, // The 8th non - wheel Mouse Button.
    ExtraButton6 = 0x00000100, // The 9th non - wheel Mouse Button.
    ExtraButton7 = 0x00000200, // The 10th non - wheel Mouse Button.
    ExtraButton8 = 0x00000400, // The 11th non - wheel Mouse Button.
    ExtraButton9 = 0x00000800, // The 12th non - wheel Mouse Button.
    ExtraButton10 = 0x00001000, // The 13th non - wheel Mouse Button.
    ExtraButton11 = 0x00002000, // The 14th non - wheel Mouse Button.
    ExtraButton12 = 0x00004000, // The 15th non - wheel Mouse Button.
    ExtraButton13 = 0x00008000, // The 16th non - wheel Mouse Button.
    ExtraButton14 = 0x00010000, // The 17th non - wheel Mouse Button.
    ExtraButton15 = 0x00020000, // The 18th non - wheel Mouse Button.
    ExtraButton16 = 0x00040000, // The 19th non - wheel Mouse Button.
    ExtraButton17 = 0x00080000, // The 20th non - wheel Mouse Button.
    ExtraButton18 = 0x00100000, // The 21st non - wheel Mouse Button.
    ExtraButton19 = 0x00200000, // The 22nd non - wheel Mouse Button.
    ExtraButton20 = 0x00400000, // The 23rd non - wheel Mouse Button.
    ExtraButton21 = 0x00800000, // The 24th non - wheel Mouse Button.
    ExtraButton22 = 0x01000000, // The 25th non - wheel Mouse Button.
    ExtraButton23 = 0x02000000, // The 26th non - wheel Mouse Button.
    ExtraButton24 = 0x04000000, // The 27th non - wheel Mouse Button.
};
};

typedef int MouseButtons;
typedef int KeyboardModifiers;

struct MouseEvent {
  Event::Type type;
  Point localPos;
  Point globalPos;
  MouseButtons buttons;
  KeyboardModifiers modifiers;
  enum Mouse::Button button;
  MouseEvent(Event::Type type_, const Point & localPos_, Mouse::Button button_, MouseButtons buttons_, KeyboardModifiers modifiers_)
    : type(type_), localPos(localPos), button(button_), buttons(buttons_), modifiers(modifiers_) {}
};




#endif // _GUI_BASIC_HPP_
