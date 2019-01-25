#ifndef _STD_BITMAP_C_H_
#define _STD_BITMAP_C_H_

#include "types_c.h"

typedef struct PaletteEntry {
  uchar b, g, r, a;
}
PaletteEntry;
////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PIXFMTDEF_DEF(PIXFMTDEF) \
  PIXFMTDEF( 1, 1, Indexed, PixelFormatIndexed) \
  PIXFMTDEF( 2, 4, Indexed, PixelFormatIndexed) \
  PIXFMTDEF( 3, 8, Indexed, PixelFormatIndexed) \
  PIXFMTDEF( 4, 1, Gray, 0) \
  PIXFMTDEF( 5, 4, Gray, 0) \
  PIXFMTDEF( 6, 8, Gray, PixelFormatExtended) \
  PIXFMTDEF( 7, 16, Gray, 0) \
  PIXFMTDEF( 8, 32, GrayFloat, 0) \
  PIXFMTDEF( 9, 64, GrayDouble, 0) \
  PIXFMTDEF(10, 16, RGB555, 0) \
  PIXFMTDEF(11, 16, RGB565, 0) \
  PIXFMTDEF(12, 16, ARGB1555, PixelFormatAlpha) \
  PIXFMTDEF(13, 24, RGB, 0) \
  PIXFMTDEF(14, 32, RGB, 0) \
  PIXFMTDEF(15, 32, ARGB, PixelFormatAlpha | PixelFormatCanonical) \
  PIXFMTDEF(16, 32, PARGB, PixelFormatAlpha | PixelFormatPAlpha) \
  PIXFMTDEF(17, 48, RGB, PixelFormatExtended) \
  PIXFMTDEF(18, 64, ARGB, PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended) \
  PIXFMTDEF(19, 64, PARGB, PixelFormatAlpha | PixelFormatPAlpha | PixelFormatExtended) \
  PIXFMTDEF(20, 128, ARGBFloat, PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended) \
  PIXFMTDEF(21, 128, ABGRFloat, PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended) \
  PIXFMTDEF(22, 96, BGRFloat, PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended) \

typedef enum {
  PixelFormatIndexed = 0x00010000, // Indexes into a palette
  PixelFormatAlpha = 0x00040000, // Has an alpha component
  PixelFormatPAlpha = 0x00080000, // Pre-multiplied alpha
  PixelFormatExtended = 0x00100000, // Extended color 16 bits/channel
  PixelFormatCanonical = 0x00200000,
  PixelFormatUndefined = 0,
  PixelFormatDontCare = 0,

#define PIXFMTDEF(_ID, _B, _NAME, _FLAG)  PF_ ## _B ##bpp## _NAME  = (_ID) | (_B<<8)|(_FLAG), PixelFormat ## _B ##bpp## _NAME = PF_ ## _B ##bpp## _NAME,
  PIXFMTDEF_DEF(PIXFMTDEF)
  PixFmtMax = 23,
#undef PIXFMTDEF
  PixelFormat16bppGrayScale = PixelFormat16bppGray
} PixFmt;
typedef PixFmt PixelFormat;

#define PixelFormatBpp(pf)   (pf>>8)&0xff
//////////////////////////////////////////
static const COLOR map32_1bppGray[] = { 0x00000000, 0xFFFFFFFF };
static const uchar map8_1bppGray[] = { 0, 0xff };

static const COLOR map32_4bppGray[] = { 0xff000000, 0xff111111, 0xff222222, 0xff333333, 0xff444444, 0xff555555, 0xff666666, 0xff777777,
0xff888888, 0xff999999, 0xffaaaaaa, 0xffbbbbbb, 0xffcccccc, 0xffdddddd, 0xffeeeeee, 0xffffffff
};
static const uchar map8_4bppGray[] = { 0x00, 0x11, 0x22, 0x33,  0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

#define RGB_GRAY(r, g, b)       ( ((r)+((g)<<1)+(b))>>2 )
#define RGBA_GRAY(r, g, b, a)   ( ((r*a)+((g*a)<<1)+(b*a))>>10 )
#define GETPIXEL_1bppIndexed(OUT)  {OUT = palette[(row[x>>3] >> (7 - (x & 7))) & 1]; }
#define GETGRAY_1bppIndexed(OUT)   {OUT = palette[(row[x>>3] >> (7 - (x & 7))) & 1]; }
#define GETPIXEL_1bppGray(OUT)     {OUT = map32_1bppGray[(row[x>>3] >> (7 - (x & 7))) & 1]; }
#define GETGRAY_1bppGray(OUT)      {OUT = map8_1bppGray[(row[x>>3] >> (7 - (x & 7))) & 1]; }
#define GETPIXEL_4bppIndexed(OUT)  {OUT = palette[(x & 1) ? (row[x / 2] & 0xf) : (row[x / 2] >> 4)]; }
#define GETGRAY_4bppIndexed(OUT)   {OUT = palette[(x & 1) ? (row[x / 2] & 0xf) : (row[x / 2] >> 4)]; }
#define GETPIXEL_4bppGray(OUT)  {OUT = map32_4bppGray[(x & 1) ? (row[x / 2] & 0xf) : (row[x / 2] >> 4)]; }
#define GETGRAY_4bppGray(OUT)   {OUT = map8_4bppGray[(x & 1) ? (row[x / 2] & 0xf) : (row[x / 2] >> 4)]; }
#define GETPIXEL_8bppIndexed(OUT)  {OUT = palette[row[x]];}
#define GETGRAY_8bppIndexed(OUT)   {OUT = palette[row[x]];}
#define GETPIXEL_8bppGray(OUT)  {BYTE r, g, b;r = g = b = row[x];OUT = _RGB(r, g, b);}
#define GETGRAY_8bppGray(OUT)   {OUT = row[x];}
#define GETPIXEL_16bppGray(OUT)  {BYTE r, g, b; r = g = b = ((ushort*)(row))[x]>>8; OUT = _RGB(r, g, b);}
#define GETGRAY_16bppGray(OUT)   {OUT = ((ushort*)(row))[x]>>8;}
#define GETPIXEL_16bppRGB555_rgba()  { WORD pixel = *((const WORD*)(row) + x); r = (pixel >> 7 & 0xf8) | (pixel >> 12 & 0x7); g = (pixel >> 2 & 0xf8) | (pixel >> 6 & 0x7); b = (pixel << 3 & 0xf8) | (pixel >> 2 & 0x7); }
#define GETPIXEL_16bppRGB555(OUT)  {BYTE r, g, b; GETPIXEL_16bppRGB555_rgba(); OUT = _RGB(r, g, b);}
#define GETGRAY_16bppRGB555(OUT)   {BYTE r, g, b; GETPIXEL_16bppRGB555_rgba(); OUT = RGB_GRAY(r, g, b);}
#define GETPIXEL_16bppRGB565_rgba() {WORD pixel = *((const WORD*)(row) + x); r = (pixel >> 8 & 0xf8) | (pixel >> 13 & 0x7); g = (pixel >> 3 & 0xfc) | (pixel >> 9 & 0x3); b = (pixel << 3 & 0xf8) | (pixel >> 2 & 0x7); }
#define GETPIXEL_16bppRGB565(OUT)  {BYTE r, g, b; GETPIXEL_16bppRGB565_rgba(); OUT = _RGB(r, g, b);}
#define GETGRAY_16bppRGB565(OUT)   {BYTE r, g, b; GETPIXEL_16bppRGB565_rgba(); OUT = RGB_GRAY(r, g, b);}
#define GETPIXEL_16bppARGB1555_rgba()  {WORD pixel = *((const WORD*)(row) + x);r = (pixel >> 7 & 0xf8) | (pixel >> 12 & 0x7);g = (pixel >> 2 & 0xf8) | (pixel >> 6 & 0x7);b = (pixel << 3 & 0xf8) | (pixel >> 2 & 0x7); a = ((pixel & 0x8000) == 0x8000) ? 255 : 0;}
#define GETPIXEL_16bppARGB1555(OUT)  {BYTE r, g, b, a; GETPIXEL_16bppARGB1555_rgba(); OUT = _RGBA(r, g, b, a);}
#define GETGRAY_16bppARGB1555(OUT)   {BYTE r, g, b, a; GETPIXEL_16bppARGB1555_rgba(); OUT = RGBA_GRAY(r, g, b, a);}
#define GETPIXEL_24bppRGB_rgba()  {r = row[x * 3 + 2];g = row[x * 3 + 1]; b = row[x * 3];}
#define GETPIXEL_24bppRGB(OUT)  {BYTE r, g, b; GETPIXEL_24bppRGB_rgba(); OUT = _RGB(r, g, b); }
#define GETGRAY_24bppRGB(OUT)   {BYTE r, g, b; GETPIXEL_24bppRGB_rgba(); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_32bppRGB_rgba()  {r = row[x * 4 + 2];g = row[x * 4 + 1];  b = row[x * 4];}
#define GETPIXEL_32bppRGB(OUT)  {BYTE r, g, b; GETPIXEL_32bppRGB_rgba(); OUT = _RGB(r, g, b); }
#define GETGRAY_32bppRGB(OUT)   {BYTE r, g, b; GETPIXEL_32bppRGB_rgba(); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_32bppARGB_rgba()  {r = row[x * 4 + 2];g = row[x * 4 + 1];b = row[x * 4];  a = row[x * 4 + 3];}
#define GETPIXEL_32bppARGB(OUT)  {BYTE r, g, b, a; GETPIXEL_32bppARGB_rgba(); OUT = _RGBA(r, g, b, a); }
#define GETGRAY_32bppARGB(OUT)   {BYTE r, g, b, a; GETPIXEL_32bppARGB_rgba(); OUT = RGBA_GRAY(r, g, b, a); }
#define GETPIXEL_32bppPARGB_rgba()  {a = row[x * 4 + 3];if (a == 0) {  r = g = b = 0;} else {  r = row[x * 4 + 2] * 255 / a;  g = row[x * 4 + 1] * 255 / a;  b = row[x * 4] * 255 / a;  }}
#define GETPIXEL_32bppPARGB(OUT)  {BYTE r, g, b, a; GETPIXEL_32bppPARGB_rgba(); OUT = _RGBA(r, g, b, a); }
#define GETGRAY_32bppPARGB(OUT)   {BYTE r, g, b, a; GETPIXEL_32bppPARGB_rgba(); OUT = RGBA_GRAY(r, g, b, a); }
#define GETPIXEL_48bppRGB_rgba()  {r = row[x * 6 + 5];g = row[x * 6 + 3];  b = row[x * 6 + 1];}
#define GETPIXEL_48bppRGB(OUT)  {BYTE r, g, b; GETPIXEL_48bppRGB_rgba(); OUT = _RGB(r, g, b); }
#define GETGRAY_48bppRGB(OUT)   {BYTE r, g, b; GETPIXEL_48bppRGB_rgba(); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_64bppARGB_rgba()  {r = row[x * 8 + 5];g = row[x * 8 + 3];b = row[x * 8 + 1];  a = row[x * 8 + 7];}
#define GETPIXEL_64bppARGB(OUT)  {BYTE r, g, b, a; GETPIXEL_64bppARGB_rgba(); OUT = _RGBA(r, g, b, a); }
#define GETGRAY_64bppARGB(OUT)   {BYTE r, g, b, a; GETPIXEL_64bppARGB_rgba(); OUT = RGB_GRAY(r, g, b);(void)a; }
#define GETPIXEL_64bppPARGB_rgba()  {a = row[x * 8 + 7];if (a == 0) {  r = g = b = 0;}else {  r = row[x * 8 + 5] * 255 / a;  g = row[x * 8 + 3] * 255 / a;  b = row[x * 8 + 1] * 255 / a;}}
#define GETPIXEL_64bppPARGB(OUT)  {BYTE r, g, b, a; GETPIXEL_64bppPARGB_rgba(); OUT = _RGBA(r, g, b, a); }
#define GETGRAY_64bppPARGB(OUT)   {BYTE r, g, b, a; GETPIXEL_64bppPARGB_rgba(); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_32bppGrayFloat(OUT)  {BYTE r, g, b;int t = (int)(((float*)(row))[x]*255); r = g = b = BOUND(t, 0, 255); OUT = _RGB(r, g, b); }
#define GETGRAY_32bppGrayFloat(OUT)   {int t = (int)(((float*)(row))[x]*255); OUT = BOUND(t, 0, 255);}
#define GETPIXEL_64bppGrayDouble(OUT)  {BYTE r, g, b;int t = (int)(((double*)(row))[x]*255); r = g = b = BOUND(t, 0, 255); OUT = _RGB(r, g, b); }
#define GETGRAY_64bppGrayDouble(OUT)   {int t = (int)(((double*)(row))[x]*255); OUT = BOUND(t, 0, 255);}
#define GETPIXEL_128bppARGBFloat(OUT)  {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+0]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+2]*255); OUT = _RGB(r, g, b); }
#define GETGRAY_128bppARGBFloat(OUT)   {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+0]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+2]*255); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_128bppABGRFloat(OUT)  {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+2]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+0]*255); OUT = _RGB(r, g, b); }
#define GETGRAY_128bppABGRFloat(OUT)   {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+2]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+0]*255); OUT = RGB_GRAY(r, g, b); }
#define GETPIXEL_96bppBGRFloat(OUT)  {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+2]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+0]*255); OUT = _RGB(r, g, b); }
#define GETGRAY_96bppBGRFloat(OUT)   {BYTE r, g, b; r = (BYTE)(((float*)(row))[x*4+2]*255); g = (BYTE)(((float*)(row))[x*4+1]*255); b = (BYTE)(((float*)(row))[x*4+0]*255); OUT = RGB_GRAY(r, g, b); }

#define GetPixFmtBpp(fmt)  (((fmt)>>8)&0xff)
#define GetPixFmtId(fmt)  ((fmt)&0xff)

CC_INLINE PixFmt cn2PixFmt(int cn)
{
  const PixFmt cn2pf[] = { PF_8bppGray, PF_8bppGray, PF_16bppRGB565, PF_24bppRGB, PF_32bppPARGB, };
  return cn <= 4 ? cn2pf[cn] : PF_32bppARGB;
}
CC_INLINE PixFmt bpp2PixFmt(int bpp)
{
  switch (bpp) {
  case 1:
    return PF_1bppGray;
  case 4:
    return PF_4bppGray;
    //case 8: return PF_8bppIndexed;
  case 8:
    return PF_8bppGray;
  case 16:
    return PF_16bppGray;
  case 24:
    return PF_24bppRGB;
  case 32:
    return PF_32bppARGB;
  default:
    return PixFmtMax;
  }
  return PixFmtMax;
}

//////////////////////////////////////////
typedef struct bitmap_t texture_t;
struct bitmap_t {
  int h, w, s;
  PixFmt fmt;
  uchar* data;
  int(*free_tex)(texture_t* tex);
};

#define BMPINIT(bmp, _H, _W, _DATA, _STEP, _BPP)  ((bmp)->h=_H, (bmp)->w=_W, (bmp)->s=_STEP, (bmp)->data=_DATA, (bmp)->fmt = bpp2PixFmt(_BPP))
#define BMPINITIM(bmp, im)  ((bmp)->h=(im)->h, (bmp)->w=(im)->w, (bmp)->s=(im)->s, (bmp)->data=(im)->tt.data, (bmp)->fmt=cn2PixFmt((im)->c))

struct pixfun_t {
  void (*getpixel)(COLOR* pixel, const BYTE* row, UINT x, const COLOR* palette);
  void (*getspan8)(UINT width, BYTE* span, const BYTE* row, UINT x, const uchar* palette);
  void (*getspan32)(UINT width, COLOR* span, const BYTE* row, UINT x, const COLOR* palette);
};

#define PIXFMTDEF2(_ID, _B, _NAME, _FLAG)  \
CC_INLINE void getpixel_ ## _B ##bpp## _NAME(COLOR* pixel, const BYTE* row, UINT x, const COLOR* palette) { GETPIXEL_ ## _B ##bpp## _NAME(*pixel); } \
CC_INLINE void getspan8_ ## _B ##bpp## _NAME(UINT width, BYTE* span, const BYTE* row, UINT x, const uchar* palette) { \
  for (; width--; ++x, ++span) { \
    GETGRAY_ ## _B ##bpp## _NAME(*span); \
  } \
} \
CC_INLINE void getspan32_ ## _B ##bpp## _NAME(UINT width, COLOR* span, const BYTE* row, UINT x, const COLOR* palette) {\
  for (; width--; ++x, ++span) {\
    GETPIXEL_ ## _B ##bpp## _NAME (*span); \
  }\
}

PIXFMTDEF_DEF(PIXFMTDEF2)

#undef PIXFMTDEF2

struct pixfun_t* get_pixfun(PixFmt fmt) {
  static struct pixfun_t pixfun[PixFmtMax] = {0};
  static int inited = 0;
  if (!inited) {
    inited = 1;
#define PIXFMTDEF(_ID, _B, _NAME, _FLAG)  pixfun[_ID].getpixel = getpixel_ ## _B ##bpp## _NAME;\
  pixfun[_ID].getspan8 = getspan8_ ## _B ##bpp## _NAME;\
  pixfun[_ID].getspan32 = getspan32_ ## _B ##bpp## _NAME;
    PIXFMTDEF_DEF(PIXFMTDEF)
#undef PIXFMTDEF
  }
  return pixfun + (fmt&0xff);
}

COLOR bitmap_getpixel(const texture_t* bitmap, const IRECT* prcSrc, int x, int y, WrapMode wrapX, WrapMode wrapY, COLOR bgclr, const COLOR* palette)
{
  BYTE* row;
  int sw = RCW(prcSrc), sh = RCH(prcSrc);
  //int w = bitmap->w;
  //int h = bitmap->h;
  PixFmt fmt = (PixFmt)bitmap->fmt;
  int id = fmt&0xff;
  struct pixfun_t* fun = get_pixfun(fmt);
  COLOR clr = bgclr;
  //TRACE("%p %d %d\n", bitmap, x, y);
  if ((x = WrapPix1D(x, sw, wrapX))>=0 && (y = WrapPix1D(y, sh, wrapY))>=0) {
    row = bitmap->data + bitmap->s * (prcSrc->t + y);
    if ((uint)id<PixFmtMax) {
      fun->getpixel(&clr, row, prcSrc->l + x, palette);
    } else {
      printf("not implemented for format 0x%x\n", fmt);
    }
  }
  return clr;
}
CC_INLINE UINT get_palette_index(BYTE r, BYTE g, BYTE b, BYTE a, const COLOR* palette, int Count)
{
  BYTE index = 0;
  int best_distance = 0x7fff;
  int distance;
  int i;
  if (!palette) {
    return 0;
  }
  /* This algorithm scans entire palette,
  computes difference from desired color (all color components have equal weight)
  and returns the index of color with least difference.
  Note: Maybe it could be replaced with a better algorithm for better image quality
  and performance, though better algorithm would probably need some pre-built lookup
  tables and thus may actually be slower if this method is called only few times per
  every image.
  */
  for (i = 0; i < Count; i++) {
    COLOR color = palette[i];
    distance = abs((int)(b - (color & 0xff))) + abs((int)(g - (color >> 8 & 0xff))) + abs((int)(r - (color >> 16 & 0xff))) + abs((int)(a - (color >> 24 & 0xff)));
    if (distance < best_distance) {
      best_distance = distance;
      index = i;
    }
  }
  return index;
}
CC_INLINE void setpixel_8bppIndexed(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  BYTE index = get_palette_index(r, g, b, a, palette, Count);
  row[x] = index;
}
CC_INLINE void setpixel_1bppIndexed(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  row[x / 8] = (row[x / 8] & ~(1 << (7 - x % 8))) | (get_palette_index(r, g, b, a, palette, Count) << (7 - x % 8));
}
CC_INLINE void setpixel_4bppIndexed(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  if (x & 1) {
    row[x / 2] = (row[x / 2] & 0xf0) | get_palette_index(r, g, b, a, palette, Count);
  }
  else {
    row[x / 2] = (row[x / 2] & 0x0f) | get_palette_index(r, g, b, a, palette, Count) << 4;
  }
}
CC_INLINE void setpixel_8bppGrayUchar(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count) {
  *((row) + x) = (r + g + g + b)>>2;
}
CC_INLINE void setpixel_32bppGrayFloat(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count) {
  *((float*)(row) + x) = (r + g + g + b)*(1.f/(1024.f));
}
CC_INLINE void setpixel_64bppGrayDouble(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count) {
  *((double*)(row) + x) = (r + g + g + b)*(1./(1024));
}
CC_INLINE void setpixel_16bppGray(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  *((WORD*)(row) + x) = (r + g + b) * 85;
}
CC_INLINE void setpixel_16bppRGB555(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  *((WORD*)(row) + x) = (r << 7 & 0x7c00) | (g << 2 & 0x03e0) | (b >> 3 & 0x001f);
}
CC_INLINE void setpixel_16bppRGB565(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  *((WORD*)(row) + x) = (r << 8 & 0xf800) | (g << 3 & 0x07e0) | (b >> 3 & 0x001f);
}
CC_INLINE void setpixel_16bppARGB1555(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  *((WORD*)(row) + x) = (a << 8 & 0x8000) | (r << 7 & 0x7c00) | (g << 2 & 0x03e0) | (b >> 3 & 0x001f);
}
CC_INLINE void setpixel_24bppRGB(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  row[x * 3 + 2] = r;
  row[x * 3 + 1] = g;
  row[x * 3] = b;
}
CC_INLINE void setpixel_32bppRGB(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  *((DWORD*)(row) + x) = (r << 16) | (g << 8) | b;
}
CC_INLINE void setpixel_32bppARGB(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  *((DWORD*)(row) + x) = (a << 24) | (r << 16) | (g << 8) | b;
}
CC_INLINE void setpixel_32bppPARGB(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  r = r * a / 255;
  g = g * a / 255;
  b = b * a / 255;
  *((DWORD*)(row) + x) = (a << 24) | (r << 16) | (g << 8) | b;
}
CC_INLINE void setpixel_48bppRGB(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  row[x * 6 + 5] = row[x * 6 + 4] = r;
  row[x * 6 + 3] = row[x * 6 + 2] = g;
  row[x * 6 + 1] = row[x * 6] = b;
}
CC_INLINE void setpixel_64bppARGB(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  uint64 a64 = a, r64 = r, g64 = g, b64 = b;
  *((uint64*)(row) + x) = (a64 << 56) | (a64 << 48) | (r64 << 40) | (r64 << 32) | (g64 << 24) | (g64 << 16) | (b64 << 8) | b64;
}
CC_INLINE void setpixel_64bppPARGB(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count)
{
  uint64 a64, r64, g64, b64;
  a64 = a * 257;
  r64 = r * a / 255;
  g64 = g * a / 255;
  b64 = b * a / 255;
  *((uint64*)(row) + x) = (a64 << 48) | (r64 << 32) | (g64 << 16) | b64;
}

///////////////////////////////////////////////////////////////

CC_INLINE int img_setbitmap_cn4(int h, int w, const void* data, int step, int cn, PixFmt fmt, bool flip_y, bool scaling, uchar* outdata, int outstep, const void* pal)
{
  int i, j;
  const uchar* srcarr = (const uchar*)data;
  double smin[4] = { DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX };
  double smax[4] = { DBL_MIN, DBL_MIN, DBL_MIN, DBL_MIN };
  double su[4] = { 1, 1, 1, 1 };
  if (h <= 0 || w <= 0) {
    return 0;
  }
  switch (fmt) {
  case PF_64bppGrayDouble: {
    typedef double arrtype;
    ASSERT(cn >= (int)sizeof(arrtype));
    if (scaling) {
      for (i = 0; i < h; ++i) {
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, src1 += cn) {
          arrtype s = *(arrtype*)src1;
          if (s < smin[0]) { smin[0] = s; }
          else if (s > smax[0]) { smax[0] = s; }
        }
      }
      su[0] = 255. / (smax[0] - smin[0]);
    }
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h - 1 - i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1 += cn) {
        arrtype s = *(arrtype*)src1;
        int t = (int)((s - smin[0]) * su[0]);
        int gry = BOUND(t, 0, 255);
        dst1[0] = gry;
        dst1[1] = gry;
        dst1[2] = gry;
        dst1[3] = 255;
      }
    }
  }
                           break;
  case PF_32bppGrayFloat: {
    typedef float arrtype;
    ASSERT(cn >= (int)sizeof(arrtype));
    if (scaling) {
      for (i = 0; i < h; ++i) {
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, src1 += cn) {
          arrtype s = *(arrtype*)src1;
          if (s < smin[0]) { smin[0] = s; }
          else if (s > smax[0]) { smax[0] = s; }
        }
      }
      su[0] = 255. / (smax[0] - smin[0]);
    }
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h - 1 - i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1 += cn) {
        arrtype s = *(arrtype*)src1;
        int t = (int)((s - smin[0]) * su[0]);
        int gry = BOUND(t, 0, 255);
        dst1[0] = gry;
        dst1[1] = gry;
        dst1[2] = gry;
        dst1[3] = 255;
      }
    }
  }
                          break;
  case PF_16bppGray: {
    typedef short arrtype;
    ASSERT(cn >= (int)sizeof(arrtype));
    if (scaling) {
      for (i = 0; i < h; ++i) {
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, src1 += cn) {
          arrtype s = *(arrtype*)src1;
          if (s < smin[0]) { smin[0] = s; }
          else if (s > smax[0]) { smax[0] = s; }
        }
      }
      su[0] = 256. / (smax[0] - smin[0]);
    }
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h - 1 - i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1 += cn) {
        arrtype s = *(arrtype*)src1;
        int t = (int)((s - smin[0]) * su[0]);
        int gry = BOUND(t, 0, 255);
        dst1[0] = gry;
        dst1[1] = gry;
        dst1[2] = gry;
        dst1[3] = 255;
      }
    }
  }
                     break;
  case PF_8bppGray: {
    typedef uchar arrtype;
    ASSERT(cn >= (int)sizeof(arrtype));
    if (scaling) {
      for (i = 0; i < h; ++i) {
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, src1 += cn) {
          arrtype s = *(arrtype*)src1;
          if (s < smin[0]) { smin[0] = s; }
          else if (s > smax[0]) { smax[0] = s; }
        }
      }
      su[0] = 256. / (smax[0] - smin[0]);
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h - 1 - i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1 += cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    else {
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h - 1 - i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1 += cn) {
          arrtype gry = *(arrtype*)src1;
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
  }
                    break;
  case PF_1bppIndexed:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h - 1 - i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4) {
        //int t = (src1[j>>3]>>(j&7))&1;
        int t = (src1[j >> 3] >> (7 - (j & 7))) & 1;
        int gry = t ? 255 : 0;
        dst1[0] = gry;
        dst1[1] = gry;
        dst1[2] = gry;
        dst1[3] = 255;
      }
    }
    break;
  case PF_24bppRGB:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h - 1 - i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1 += cn) {
        dst1[0] = src1[0];
        dst1[1] = src1[1];
        dst1[2] = src1[2];
        dst1[3] = 255;
      }
    }
    break;
  case PF_32bppPARGB:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h - 1 - i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1 += cn) {
        uchar a = src1[3];
        dst1[0] = src1[0] * a / 255;
        dst1[1] = src1[1] * a / 255;
        dst1[2] = src1[2] * a / 255;
        dst1[3] = 255;
      }
    }
    break;
  case PF_32bppARGB:
  case PF_32bppRGB:
    for (i = 0; i < h; ++i) {
      uint32* dst1 = (uint32*)(outdata + (flip_y ? (h - 1 - i) : i) * outstep);
      const uint32* src1 = (const uint32*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        *dst1++ = *src1++;
      }
    }
    break;
  case PF_128bppARGBFloat:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h - 1 - i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        dst1[0] = (uchar)(BOUND(src1[0], 0, 1) * 255);
        dst1[1] = (uchar)(BOUND(src1[1], 0, 1) * 255);
        dst1[2] = (uchar)(BOUND(src1[2], 0, 1) * 255);
        dst1[3] = (uchar)(BOUND(src1[3], 0, 1) * 255);
        dst1 += 4;
        src1 += 4;
      }
    }
    break;
  case PF_128bppABGRFloat:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h - 1 - i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        dst1[0] = (uchar)(BOUND(src1[2], 0, 1) * 255);
        dst1[1] = (uchar)(BOUND(src1[1], 0, 1) * 255);
        dst1[2] = (uchar)(BOUND(src1[0], 0, 1) * 255);
        dst1[3] = (uchar)(BOUND(src1[3], 0, 1) * 255);
        dst1 += 4;
        src1 += 4;
      }
    }
    break;
  case PF_96bppBGRFloat:
    if (scaling) {
      for (i = 0; i < h; ++i) {
        const float* src1 = (const float*)(srcarr + i * step);
        for (j = 0; j < w; ++j, src1 += 3) {
          MINMAX_UPDATE(src1[0], smin[0], smax[0]);
          MINMAX_UPDATE(src1[1], smin[1], smax[1]);
          MINMAX_UPDATE(src1[2], smin[2], smax[2]);
        }
      }
      su[0] = 256. / (smax[0] - smin[0]);
      su[1] = 256. / (smax[1] - smin[1]);
      su[2] = 256. / (smax[2] - smin[2]);
    }
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h - 1 - i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        double t0 = ((src1[0] - smin[0]) * su[0]);
        double t1 = ((src1[1] - smin[1]) * su[1]);
        double t2 = ((src1[2] - smin[2]) * su[2]);
        dst1[0] = (uchar)(BOUND(t0, 0, 255));
        dst1[1] = (uchar)(BOUND(t1, 0, 255));
        dst1[2] = (uchar)(BOUND(t2, 0, 255));
        dst1[3] = 255;
        dst1 += 4;
        src1 += 3;
      }
    }
    break;
  case PF_8bppIndexed:
    if (pal) {
      const uint32* upal = (const uint32*)pal;
      for (i = 0; i < h; ++i) {
        uint32* dst1 = (uint32*)(outdata + (flip_y ? (h - 1 - i) : i) * outstep);
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j) {
          uchar t = src1[j];
          dst1[j] = upal[t];
        }
      }
    }
    else {
      for (i = 0; i < h; ++i) {
        uchar* dst1 = (uchar*)(outdata + (flip_y ? (h - 1 - i) : i) * outstep);
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j) {
          uchar t = src1[j];
          dst1[0] = t;
          dst1[1] = t;
          dst1[2] = t;
          dst1[3] = 255;
          dst1 += 4;
        }
      }
    }
    break;
  default:
    ASSERT(0);
    break;
  }
  return 0;
}

#endif // _STD_BITMAP_C_H_
