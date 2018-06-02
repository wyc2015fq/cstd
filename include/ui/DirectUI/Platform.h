
#ifndef PLATFORM_H
#define PLATFORM_H

// PLAT_GTK = GTK+ on Linux or Win32
// PLAT_GTK_WIN32 is defined additionally when running PLAT_GTK under Win32
// PLAT_WIN = Win32 API on Win32 OS
// PLAT_WX is wxWindows on any supported platform

#define PLAT_GTK 0
#define PLAT_GTK_WIN32 0
#define PLAT_MACOSX 0
#define PLAT_WIN 0
#define PLAT_WX  0
#define PLAT_FOX 0

#if defined(FOX)
#undef PLAT_FOX
#define PLAT_FOX 1

#elif defined(__WX__)
#undef PLAT_WX
#define PLAT_WX  1

#elif defined(GTK)
#undef PLAT_GTK
#define PLAT_GTK 1

#if defined(__WIN32__) || defined(_MSC_VER)
#undef PLAT_GTK_WIN32
#define PLAT_GTK_WIN32 1
#endif

#elif defined(__APPLE__)

#undef PLAT_MACOSX
#define PLAT_MACOSX 1

#else
#undef PLAT_WIN
#define PLAT_WIN 1

#endif

typedef int bool;
#define false 0
#define true 1

// Underlying the implementation of the platform classes are platform specific types.
// Sometimes these need to be passed around by client code so they are defined here

typedef void *FontID;
typedef void *SurfaceID;
typedef void *WindowID;
typedef void *MenuID;
typedef void *TickerID;
typedef void *Function;
typedef void *IdlerID;

/**
 * A geometric point struct.
 * Point is exactly the same as the Win32 POINT and GTK+ GdkPoint so can be used interchangeably.
 */
typedef struct _Point {
  int x;
  int y;
} Point;

Point ptPoint(int x, int y) { Point pt; pt.x = x, pt.y = y; return pt; }

/**
 * A geometric rectangle struct.
 * PRectangle is exactly the same as the Win32 RECT so can be used interchangeably.
 * PRectangles contain their top and left sides, but not their right and bottom sides.
 */
typedef struct _PRectangle {
  int left;
  int top;
  int right;
  int bottom;
}PRectangle;


PRectangle rcPRectangle(int left, int top, int right, int bottom) {
  PRectangle rc;
  rc.left=(left), rc.top=(top), rc.right=(right), rc.bottom=(bottom);
  return rc;
}

// Other automatically defined methods (assignment, copy constructor, destructor) are fine

#define rcEq(me, rc) ((rc.left == me.left) && (rc.right == me.right) && (rc.top == me.top) && (rc.bottom == me.bottom))
bool rcContainsPt(PRectangle me, Point pt) {
  return (pt.x >= me.left) && (pt.x <= me.right) &&
    (pt.y >= me.top) && (pt.y <= me.bottom);
}
bool rcContains(PRectangle me, PRectangle rc) {
    return (rc.left >= me.left) && (rc.right <= me.right) &&
      (rc.top >= me.top) && (rc.bottom <= me.bottom);
}
bool rcIntersects(PRectangle me, PRectangle other) {
    return (me.right > other.left) && (me.left < other.right) &&
      (me.bottom > other.top) && (me.top < other.bottom);
}
#define rcMove(me, xDelta, yDelta) (me.left += xDelta, me.top += yDelta, me.right += xDelta, me.bottom += yDelta)
#define rcWidth(me) (me.right - me.left)
#define rcHeight(me) (me.bottom - me.top)
#define rcEmpty(me)   ((rcHeight() <= 0) || (rcWidth() <= 0))

long clrRGB(unsigned int red, unsigned int green, unsigned int blue) {
    return red | (green << 8) | (blue << 16);
}

static unsigned int ValueOfHex(const char ch) {
    if (ch >= '0' && ch <= '9')
      return ch - '0';
    else if (ch >= 'A' && ch <= 'F')
      return ch - 'A' + 10;
    else if (ch >= 'a' && ch <= 'f')
      return ch - 'a' + 10;
    else
      return 0;
}

long clrStr(const char *val) {
  unsigned int r, g, b;
  if (*val == '#') {
    val++;
  }
  r = ValueOfHex(val[0]) * 16 + ValueOfHex(val[1]);
  g = ValueOfHex(val[2]) * 16 + ValueOfHex(val[3]);
  b = ValueOfHex(val[4]) * 16 + ValueOfHex(val[5]);
  return clrRGB(r, g, b);
}

unsigned int clrGetRed(long co) {
    return co & 0xff;
}

unsigned int clrGetGreen(long co) {
    return (co >> 8) & 0xff;
}

unsigned int clrGetBlue(long co) {
    return (co >> 16) & 0xff;
}
/**
 * Holds an allocated RGB colour which may be an approximation to the desired colour.
 */
struct ColourAllocated {
  long coAllocated;
};

/**
 * Colour pairs hold a desired colour and an allocated colour.
 */
typedef struct _ColourPair {
  long desired;
  long allocated;
}ColourPair;

struct Window;  // Forward declaration for Palette

/**
 * Colour palette management.
 */
typedef struct _Palette {
  int used;
  int size;
  ColourPair *entries;
#if PLAT_GTK
  void *allocatedPalette; // GdkColor *
  int allocatedLen;
#endif

#if PLAT_WIN
  void *hpal;
#endif
  bool allowRealization;
}Palette;

typedef struct _Font {
  FontID fid;
#if PLAT_WX
  int ascent;
#endif
}Font;

#define PLATFORM_ASSERT assert
typedef struct _Surface_vtable Surface_vtable;
typedef struct _Surface {
  Surface_vtable* vtable;
} Surface;
typedef long ColourAllocated;

Surface *sfAllocate();
// A surface abstracts a place to draw.
typedef struct _Surface_vtable {
  Surface* (*suClone)(Surface* s, const Surface* );
  void (*sfInitPixMap)(Surface* s, int width, int height, Surface *surface_, WindowID wid);
  void (*sfRelease)(Surface* s);
  bool (*sfInitialised)(Surface* s);
  void (*sfPenColour)(Surface* s, ColourAllocated fore);
  int (*sfLogPixelsY)(Surface* s);
  int (*sfDeviceHeightFont)(Surface* s, int points);
  void (*sfMoveTo)(Surface* s, int x_, int y_);
  void (*sfLineTo)(Surface* s, int x_, int y_);
  void (*sfPolygon)(Surface* s, Point *pts, int npts, ColourAllocated fore, ColourAllocated back);
  void (*sfRectangleDraw)(Surface* s, PRectangle rc, ColourAllocated fore, ColourAllocated back);
  void (*sfFillRectangle)(Surface* s, PRectangle rc, ColourAllocated back);
  void (*sfFillRectangle2)(Surface* s, PRectangle rc, Surface* surfacePattern);
  void (*sfRoundedRectangle)(Surface* s, PRectangle rc, ColourAllocated fore, ColourAllocated back);
  void (*sfAlphaRectangle)(Surface* s, PRectangle rc, int cornerSize, ColourAllocated fill, int alphaFill, ColourAllocated outline, int alphaOutline, int flags);
  void (*sfEllipse)(Surface* s, PRectangle rc, ColourAllocated fore, ColourAllocated back);
  void (*sfCopy)(Surface* s, PRectangle rc, Point from, Surface* surfaceSource);

  void (*sfDrawTextNoClip)(Surface* s0, PRectangle rc, Font* font_, int ybase, const char *s, int len, ColourAllocated fore, ColourAllocated back);
  void (*sfDrawTextClipped)(Surface* s0, PRectangle rc, Font* font_, int ybase, const char *s, int len, ColourAllocated fore, ColourAllocated back);
  void (*sfDrawTextTransparent)(Surface* s0, PRectangle rc, Font* font_, int ybase, const char *s, int len, ColourAllocated fore);
  void (*sfMeasureWidths)(Surface* s0, Font* font_, const char *s, int len, int *positions);
  int (*sfWidthText)(Surface* s0, Font* font_, const char *s, int len);
  int (*sfWidthChar)(Surface* s, Font* font_, char ch);
  int (*sfAscent)(Surface* s, Font* font_);
  int (*sfDescent)(Surface* s, Font* font_);
  int (*sfInternalLeading)(Surface* s, Font* font_);
  int (*sfExternalLeading)(Surface* s, Font* font_);
  int (*sfHeight)(Surface* s, Font* font_);
  int (*sfAverageCharWidth)(Surface* s, Font* font_);

  int (*sfSetPalette)(Surface* s, Palette *pal, bool inBackGround);
  void (*sfSetClip)(Surface* s, PRectangle rc);
  void (*sfFlushCachedState)(Surface* s);

  void (*sfSetUnicodeMode)(Surface* s, bool unicodeMode_);
  void (*sfSetDBCSMode)(Surface* s, int codePage);
} Surface_vtable;

typedef struct _ElapsedTime {
  long bigBit;
  long littleBit;
}ElapsedTime;

typedef void* DynamicLibrary;

typedef long ColourDesired;
#endif
