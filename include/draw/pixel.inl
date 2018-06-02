#ifndef _PIXEL_INL_
#define _PIXEL_INL_

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


#endif // _PIXEL_INL_
