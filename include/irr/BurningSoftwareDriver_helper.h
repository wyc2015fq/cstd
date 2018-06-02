
#define reciprocal_approxim(x)  (1.f / (x))
#define BURNINGVIDEO_RENDERER_BEAUTIFUL
// defined now in irrlicht compile config
#ifdef BURNINGVIDEO_RENDERER_BEAUTIFUL
#define SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#define SOFTWARE_DRIVER_2_SUBTEXEL
#define SOFTWARE_DRIVER_2_BILINEAR
#define SOFTWARE_DRIVER_2_LIGHTING
#define SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
#define SOFTWARE_DRIVER_2_32BIT
#define SOFTWARE_DRIVER_2_MIPMAPPING
#define SOFTWARE_DRIVER_2_USE_WBUFFER
#define SOFTWARE_DRIVER_2_TEXTURE_TRANSFORM
#define SOFTWARE_DRIVER_2_TEXTURE_MAXSIZE   0
#endif
//! Set Flags for Windows Mobile
#ifdef BURNINGVIDEO_RENDERER_CE
#define SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#define SOFTWARE_DRIVER_2_SUBTEXEL
//#define SOFTWARE_DRIVER_2_BILINEAR
//#define SOFTWARE_DRIVER_2_LIGHTING
#define SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
#define SOFTWARE_DRIVER_2_16BIT
#define SOFTWARE_DRIVER_2_MIPMAPPING
#define SOFTWARE_DRIVER_2_USE_WBUFFER
//#define SOFTWARE_DRIVER_2_TEXTURE_TRANSFORM
#define SOFTWARE_DRIVER_2_TEXTURE_MAXSIZE   128
#endif
#ifdef BURNINGVIDEO_RENDERER_FAST
#define SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#define SOFTWARE_DRIVER_2_SUBTEXEL
//#define SOFTWARE_DRIVER_2_BILINEAR
//#define SOFTWARE_DRIVER_2_LIGHTING
//#define SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
#define SOFTWARE_DRIVER_2_32BIT
#define SOFTWARE_DRIVER_2_MIPMAPPING
#define SOFTWARE_DRIVER_2_USE_WBUFFER
#define SOFTWARE_DRIVER_2_TEXTURE_MAXSIZE   0
#endif
#ifdef BURNINGVIDEO_RENDERER_ULTRA_FAST
#define BURNINGVIDEO_RENDERER_FAST
//#define SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#define SOFTWARE_DRIVER_2_SUBTEXEL
//#define SOFTWARE_DRIVER_2_BILINEAR
//#define SOFTWARE_DRIVER_2_LIGHTING
//#define SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
//#define SOFTWARE_DRIVER_2_32BIT
#define SOFTWARE_DRIVER_2_MIPMAPPING
#define SOFTWARE_DRIVER_2_USE_WBUFFER
#define SOFTWARE_DRIVER_2_TEXTURE_MAXSIZE   0
#endif
// Derivate flags
// texture format
#ifdef SOFTWARE_DRIVER_2_32BIT
#define BURNINGSHADER_COLOR_FORMAT  ECF_A8R8G8B8
#else
#define BURNINGSHADER_COLOR_FORMAT  ECF_A1R5G5B5
#endif
// mip mapping
#if defined ( SOFTWARE_DRIVER_2_MIPMAPPING )
#if defined( BURNINGVIDEO_RENDERER_BEAUTIFUL )
#define SOFTWARE_DRIVER_2_MIPMAPPING_MAX    8
#define SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS 0
#elif defined ( BURNINGVIDEO_RENDERER_CE )
#define SOFTWARE_DRIVER_2_MIPMAPPING_MAX    4
#define SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS 0
#else
#define SOFTWARE_DRIVER_2_MIPMAPPING_MAX    8
#define SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS 0
#endif
#else
#define SOFTWARE_DRIVER_2_MIPMAPPING_MAX    1
#define SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS 0
#endif
#define SOFTWARE_DRIVER_2_MIPMAPPING_SCALE (8/SOFTWARE_DRIVER_2_MIPMAPPING_MAX)
#ifndef CV_INLINE
#ifdef _MSC_VER
#define CV_INLINE __forceinline
#else
#define CV_INLINE CV_INLINE
#endif
#endif

// supporting different packed pixel needs many defines...
#ifdef SOFTWARE_DRIVER_2_32BIT
typedef u32 tVideoSample;
#define MASK_A  0xFF000000
#define MASK_R  0x00FF0000
#define MASK_G  0x0000FF00
#define MASK_B  0x000000FF
#define SHIFT_A 24
#define SHIFT_R 16
#define SHIFT_G 8
#define SHIFT_B 0
#define COLOR_MAX         0xFF
#define COLOR_MAX_LOG2        8
#define COLOR_BRIGHT_WHITE      0xFFFFFFFF
#define VIDEO_SAMPLE_GRANULARITY  2
#else
typedef u16 tVideoSample;
#define MASK_A  0x8000
#define MASK_R  0x7C00
#define MASK_G  0x03E0
#define MASK_B  0x001F
#define SHIFT_A 15
#define SHIFT_R 10
#define SHIFT_G 5
#define SHIFT_B 0
#define COLOR_MAX         0x1F
#define COLOR_MAX_LOG2        5
#define COLOR_BRIGHT_WHITE      0xFFFF
#define VIDEO_SAMPLE_GRANULARITY  1
#endif
// ----------------------- Generic ----------------------------------
//! a more useful memset for pixel
// (standard memset only works with 8-bit values)
CV_INLINE void memset32(void* dest, const u32 value, u32 bytesize)
{
  u32* d = (u32*) dest;
  u32 i;
  // loops unrolled to reduce the number of increments by factor ~8.
  i = bytesize >> (2 + 3);
  while (i) {
    d[0] = value;
    d[1] = value;
    d[2] = value;
    d[3] = value;
    d[4] = value;
    d[5] = value;
    d[6] = value;
    d[7] = value;
    d += 8;
    i -= 1;
  }
  i = (bytesize >> 2) & 7;
  while (i) {
    d[0] = value;
    d += 1;
    i -= 1;
  }
}
//! a more useful memset for pixel
// (standard memset only works with 8-bit values)
CV_INLINE void memset16(void* dest, const u16 value, u32 bytesize)
{
  u16* d = (u16*) dest;
  u32 i;
  // loops unrolled to reduce the number of increments by factor ~8.
  i = bytesize >> (1 + 3);
  while (i) {
    d[0] = value;
    d[1] = value;
    d[2] = value;
    d[3] = value;
    d[4] = value;
    d[5] = value;
    d[6] = value;
    d[7] = value;
    d += 8;
    --i;
  }
  i = (bytesize >> 1) & 7;
  while (i) {
    d[0] = value;
    ++d;
    --i;
  }
}
/*
  use biased loop counter
  --> 0 byte copy is forbidden
*/
CV_INLINE void memcpy32_small(void* dest, const void* source, u32 bytesize)
{
  u32 c = bytesize >> 2;
  do {
    ((u32*) dest) [ c - 1 ] = ((u32*) source) [ c - 1 ];
  }
  while (--c);
}
// integer log2 of a float ieee 754. TODO: non ieee floating point
CV_INLINE s32 s32_log2_f32(f32 f)
{
  u32 x = IR(f);
  return ((x & 0x7F800000) >> 23) - 127;
}
CV_INLINE s32 s32_log2_s32(u32 x)
{
  return s32_log2_f32((f32) x);
}
CV_INLINE s32 s32_abs(s32 x)
{
  s32 b = x >> 31;
  return (x ^ b) - b;
}
//! conditional set based on mask and arithmetic shift
CV_INLINE u32 if_mask_a_else_b(const u32 mask, const u32 a, const u32 b)
{
  return (mask & (a ^ b)) ^ b;
}
// ------------------ Video---------------------------------------
/*!
  Pixel = dest * ( 1 - alpha ) + source * alpha
  alpha [0;256]
*/
CV_INLINE u32 PixelBlend32(const u32 c2, const u32 c1, u32 alpha)
{
  u32 srcRB = c1 & 0x00FF00FF;
  u32 srcXG = c1 & 0x0000FF00;
  u32 dstRB = c2 & 0x00FF00FF;
  u32 dstXG = c2 & 0x0000FF00;
  u32 rb = srcRB - dstRB;
  u32 xg = srcXG - dstXG;
  rb *= alpha;
  xg *= alpha;
  rb >>= 8;
  xg >>= 8;
  rb += dstRB;
  xg += dstXG;
  rb &= 0x00FF00FF;
  xg &= 0x0000FF00;
  return rb | xg;
}
/*!
  Pixel = dest * ( 1 - alpha ) + source * alpha
  alpha [0;32]
*/
CV_INLINE u16 PixelBlend16(const u16 c2, const u32 c1, const u16 alpha)
{
  const u16 srcRB = c1 & 0x7C1F;
  const u16 srcXG = c1 & 0x03E0;
  const u16 dstRB = c2 & 0x7C1F;
  const u16 dstXG = c2 & 0x03E0;
  u32 rb = srcRB - dstRB;
  u32 xg = srcXG - dstXG;
  rb *= alpha;
  xg *= alpha;
  rb >>= 5;
  xg >>= 5;
  rb += dstRB;
  xg += dstXG;
  rb &= 0x7C1F;
  xg &= 0x03E0;
  return (u16)(rb | xg);
}
/*
  Pixel = c0 * (c1/31). c0 Alpha retain
*/
CV_INLINE u16 PixelMul16(const u16 c0, const u16 c1)
{
  return (u16)(((((c0 & 0x7C00) * (c1 & 0x7C00)) & 0x3E000000) >> 15) |
      ((((c0 & 0x03E0) * (c1 & 0x03E0)) & 0x000F8000) >> 10) |
      ((((c0 & 0x001F) * (c1 & 0x001F)) & 0x000003E0) >> 5) |
      (c0 & 0x8000));
}
/*
  Pixel = c0 * (c1/31).
*/
CV_INLINE u16 PixelMul16_2(u16 c0, u16 c1)
{
  return (u16)((((c0 & 0x7C00) * (c1 & 0x7C00)) & 0x3E000000) >> 15 |
      (((c0 & 0x03E0) * (c1 & 0x03E0)) & 0x000F8000) >> 10 |
      (((c0 & 0x001F) * (c1 & 0x001F)) & 0x000003E0) >> 5  |
      (c0 & c1 & 0x8000));
}
/*
  Pixel = c0 * (c1/255). c0 Alpha Retain
*/
CV_INLINE u32 PixelMul32(const u32 c0, const u32 c1)
{
  return (c0 & 0xFF000000) |
      ((((c0 & 0x00FF0000) >> 12) * ((c1 & 0x00FF0000) >> 12)) & 0x00FF0000) |
      ((((c0 & 0x0000FF00) * (c1 & 0x0000FF00)) >> 16) & 0x0000FF00) |
      ((((c0 & 0x000000FF) * (c1 & 0x000000FF)) >> 8) & 0x000000FF);
}
/*
  Pixel = c0 * (c1/255).
*/
CV_INLINE u32 PixelMul32_2(const u32 c0, const u32 c1)
{
  return ((((c0 & 0xFF000000) >> 16) * ((c1 & 0xFF000000) >> 16)) & 0xFF000000) |
      ((((c0 & 0x00FF0000) >> 12) * ((c1 & 0x00FF0000) >> 12)) & 0x00FF0000) |
      ((((c0 & 0x0000FF00) * (c1 & 0x0000FF00)) >> 16) & 0x0000FF00) |
      ((((c0 & 0x000000FF) * (c1 & 0x000000FF)) >> 8) & 0x000000FF);
}
/*
  Pixel = clamp ( c0 + c1, 0, 255 )
*/
CV_INLINE u32 PixelAdd32(const u32 c2, const u32 c1)
{
  u32 sum = (c2 & 0x00FFFFFF)  + (c1 & 0x00FFFFFF);
  u32 low_bits = (c2 ^ c1) & 0x00010101;
  s32 carries  = (sum - low_bits) & 0x01010100;
  u32 modulo = sum - carries;
  u32 clamp = carries - (carries >> 8);
  return modulo | clamp;
}
#if 0
// 1 - Bit Alpha Blending
CV_INLINE u16 PixelBlend16_1(const u16 destination, const u16 source)
{
  if ((source & 0x8000) == 0x8000) {
    return source;  // The source is visible, so use it.
  }
  else {
    return destination;  // The source is transparent, so use the destination.
  }
}
// 1 - Bit Alpha Blending 16Bit SIMD
CV_INLINE u32 PixelBlend16_simd(const u32 destination, const u32 source)
{
  switch (source & 0x80008000) {
  case 0x80008000: // Both source pixels are visible
    return source;
  case 0x80000000: // Only the first source pixel is visible
    return (source & 0xFFFF0000) | (destination & 0x0000FFFF);
  case 0x00008000: // Only the second source pixel is visible.
    return (destination & 0xFFFF0000) | (source & 0x0000FFFF);
  default: // Neither source pixel is visible.
    return destination;
  }
}
#else
// 1 - Bit Alpha Blending
CV_INLINE u16 PixelBlend16_1(const u16 c2, const u16 c1)
{
  u16 mask = ((c1 & 0x8000) >> 15) + 0x7fff;
  return (c2 & mask) | (c1 & ~mask);
}
// 1 - Bit Alpha Blending 16Bit SIMD
CV_INLINE u32 PixelBlend16_simd(const u32 c2, const u32 c1)
{
  u32 mask = ((c1 & 0x80008000) >> 15) + 0x7fff7fff;
  return (c2 & mask) | (c1 & ~mask);
}
#endif
/*!
  Pixel = dest * ( 1 - SourceAlpha ) + source * SourceAlpha
*/
CV_INLINE u32 PixelBlend32_1(const u32 c2, const u32 c1)
{
  // alpha test
  u32 alpha = c1 & 0xFF000000;
  if (0 == alpha) {
    return c2;
  }
  if (0xFF000000 == alpha) {
    return c1;
  }
  alpha >>= 24;
  // add highbit alpha, if ( alpha > 127 ) alpha += 1;
  alpha += (alpha >> 7);
  {
    u32 srcRB = c1 & 0x00FF00FF;
  u32 srcXG = c1 & 0x0000FF00;
  u32 dstRB = c2 & 0x00FF00FF;
  u32 dstXG = c2 & 0x0000FF00;
  u32 rb = srcRB - dstRB;
  u32 xg = srcXG - dstXG;
  rb *= alpha;
  xg *= alpha;
  rb >>= 8;
  xg >>= 8;
  rb += dstRB;
  xg += dstXG;
  rb &= 0x00FF00FF;
  xg &= 0x0000FF00;
  return (c1 & 0xFF000000) | rb | xg;
  }
}
// ------------------ Fix Point ----------------------------------
typedef s32 tFixPoint;
typedef u32 tFixPointu;
// Fix Point 12
#if 0
#define FIX_POINT_PRE     12
#define FIX_POINT_FRACT_MASK  0xFFF
#define FIX_POINT_SIGNED_MASK 0xFFFFF000
#define FIX_POINT_UNSIGNED_MASK 0x7FFFF000
#define FIX_POINT_ONE     0x1000
#define FIX_POINT_ZERO_DOT_FIVE 0x0800
#define FIX_POINT_F32_MUL   4096.f
#endif
// Fix Point 10
#if 1
#define FIX_POINT_PRE     10
#define FIX_POINT_FRACT_MASK  0x3FF
#define FIX_POINT_SIGNED_MASK 0xFFFFFC00
#define FIX_POINT_UNSIGNED_MASK 0x7FFFFE00
#define FIX_POINT_ONE     0x400
#define FIX_POINT_ZERO_DOT_FIVE 0x200
#define FIX_POINT_F32_MUL   1024.f
#endif
// Fix Point 9
#if 0
#define FIX_POINT_PRE     9
#define FIX_POINT_FRACT_MASK  0x1FF
#define FIX_POINT_SIGNED_MASK 0xFFFFFE00
#define FIX_POINT_UNSIGNED_MASK 0x7FFFFE00
#define FIX_POINT_ONE     0x200
#define FIX_POINT_ZERO_DOT_FIVE 0x100
#define FIX_POINT_F32_MUL   512.f
#endif
// Fix Point 7
#if 0
#define FIX_POINT_PRE     7
#define FIX_POINT_FRACT_MASK  0x7F
#define FIX_POINT_SIGNED_MASK 0xFFFFFF80
#define FIX_POINT_UNSIGNED_MASK 0x7FFFFF80
#define FIX_POINT_ONE     0x80
#define FIX_POINT_ZERO_DOT_FIVE 0x40
#define FIX_POINT_F32_MUL   128.f
#endif
#define FIXPOINT_COLOR_MAX    ( COLOR_MAX << FIX_POINT_PRE )
#define FIX_POINT_HALF_COLOR ( (tFixPoint) ( ((f32) COLOR_MAX / 2.f * FIX_POINT_F32_MUL ) ) )
/*
  convert signed integer to fixpoint
*/
CV_INLINE tFixPoint s32_to_fixPoint(const s32 x)
{
  return x << FIX_POINT_PRE;
}
CV_INLINE tFixPointu u32_to_fixPoint(const u32 x)
{
  return x << FIX_POINT_PRE;
}
CV_INLINE u32 fixPointu_to_u32(const tFixPointu x)
{
  return x >> FIX_POINT_PRE;
}
// 1/x * FIX_POINT
CV_INLINE f32 fix_inverse32(const f32 x)
{
  return FIX_POINT_F32_MUL / x;
}
/*
  convert float to fixpoint
  fast convert (fistp on x86) HAS to be used..
  hints: compileflag /QIfist for msvc7. msvc 8.0 has smth different
  others should use their favourite assembler..
*/
CV_INLINE int f_round2(f32 f)
{
  f += (3 << 22);
  return IR(f) - 0x4b400000;
}
/*
  convert f32 to Fix Point.
  multiply is needed anyway, so scale mulby
*/
CV_INLINE tFixPoint tofix(const f32 x, const f32 mulby DEFAULT(FIX_POINT_F32_MUL))
{
  return (tFixPoint)(x * mulby);
}
/*
  Fix Point , Fix Point Multiply
*/
CV_INLINE tFixPointu imulFixu(const tFixPointu x, const tFixPointu y)
{
  return (x * y) >> (tFixPointu) FIX_POINT_PRE;
}
/*
  Fix Point , Fix Point Multiply
*/
CV_INLINE tFixPoint imulFix(const tFixPoint x, const tFixPoint y)
{
  return (x * y) >> (FIX_POINT_PRE);
}
/*
  Fix Point , Fix Point Multiply x * y * 2
*/
CV_INLINE tFixPoint imulFix2(const tFixPoint x, const tFixPoint y)
{
  return (x * y) >> (FIX_POINT_PRE - 1);
}
/*
  Multiply x * y * 1
*/
CV_INLINE tFixPoint imulFix_tex1(const tFixPoint x, const tFixPoint y)
{
  return (((tFixPointu) x >> 2) * ((tFixPointu) y >> 2)) >> (tFixPointu)(FIX_POINT_PRE + 4);
}
/*
  Multiply x * y * 2
*/
CV_INLINE tFixPoint imulFix_tex2(const tFixPoint x, const tFixPoint y)
{
  return (((tFixPointu) x >> 2) * ((tFixPointu) y >> 2)) >> (tFixPointu)(FIX_POINT_PRE + 3);
}
/*
  Multiply x * y * 4
*/
CV_INLINE tFixPoint imulFix_tex4(const tFixPoint x, const tFixPoint y)
{
#ifdef SOFTWARE_DRIVER_2_32BIT
  return (((tFixPointu) x >> 2) * ((tFixPointu) y >> 2)) >> (tFixPointu)(FIX_POINT_PRE + 2);
#else
  return (x * y) >> (FIX_POINT_PRE + (VIDEO_SAMPLE_GRANULARITY * 3));
#endif
}
/*!
  clamp FixPoint to maxcolor in FixPoint, min(a,31)
*/
CV_INLINE tFixPoint clampfix_maxcolor(const tFixPoint a)
{
  tFixPoint c = (a - FIXPOINT_COLOR_MAX) >> 31;
  return (a & c) | (FIXPOINT_COLOR_MAX & ~c);
}
/*!
  clamp FixPoint to 0 in FixPoint, max(a,0)
*/
CV_INLINE tFixPoint clampfix_mincolor(const tFixPoint a)
{
  return a - (a & (a >> 31));
}
CV_INLINE tFixPoint saturateFix(const tFixPoint a)
{
  return clampfix_mincolor(clampfix_maxcolor(a));
}
// rount fixpoint to int
CV_INLINE s32 roundFix(const tFixPoint x)
{
  return (x + FIX_POINT_ZERO_DOT_FIVE) >> FIX_POINT_PRE;
}
// x in [0;1[
CV_INLINE s32 f32_to_23Bits(const f32 x)
{
  f32 y = x + 1.f;
  return IR(y) & 0x7FFFFF;  // last 23 bits
}
/*!
  return VideoSample from fixpoint
*/
CV_INLINE tVideoSample fix_to_color_rgb(const tFixPoint r, const tFixPoint g, const tFixPoint b)
{
#ifdef __BIG_ENDIAN__
  return  FIXPOINT_COLOR_MAX |
      (r & FIXPOINT_COLOR_MAX) >> (FIX_POINT_PRE - 8) |
      (g & FIXPOINT_COLOR_MAX) << (16 - FIX_POINT_PRE) |
      (b & FIXPOINT_COLOR_MAX) << (24 - FIX_POINT_PRE);
#else
  return (FIXPOINT_COLOR_MAX & FIXPOINT_COLOR_MAX) << (SHIFT_A - FIX_POINT_PRE) |
      (r & FIXPOINT_COLOR_MAX) << (SHIFT_R - FIX_POINT_PRE) |
      (g & FIXPOINT_COLOR_MAX) >> (FIX_POINT_PRE - SHIFT_G) |
      (b & FIXPOINT_COLOR_MAX) >> (FIX_POINT_PRE - SHIFT_B);
#endif
}
/*!
  return VideoSample from fixpoint
*/
CV_INLINE tVideoSample fix4_to_color_argb(const tFixPoint a, const tFixPoint r, const tFixPoint g, const tFixPoint b)
{
#ifdef __BIG_ENDIAN__
  return (a & (FIX_POINT_FRACT_MASK - 1)) >> (FIX_POINT_PRE) |
      (r & FIXPOINT_COLOR_MAX) >> (FIX_POINT_PRE - 8) |
      (g & FIXPOINT_COLOR_MAX) << (16 - FIX_POINT_PRE) |
      (b & FIXPOINT_COLOR_MAX) << (24 - FIX_POINT_PRE);
#else
  return (a & (FIX_POINT_FRACT_MASK - 1)) << (SHIFT_A - 1) |
      (r & FIXPOINT_COLOR_MAX) << (SHIFT_R - FIX_POINT_PRE) |
      (g & FIXPOINT_COLOR_MAX) >> (FIX_POINT_PRE - SHIFT_G) |
      (b & FIXPOINT_COLOR_MAX) >> (FIX_POINT_PRE - SHIFT_B);
#endif
}
// return fixpoint from VideoSample granularity COLOR_MAX
#if 1
CV_INLINE void color_to_fix_rgb(tFixPoint* r, tFixPoint* g, tFixPoint* b, const tVideoSample t00)
{
  *r  = (t00 & MASK_R) >> (SHIFT_R - FIX_POINT_PRE);
  *g  = (t00 & MASK_G) << (FIX_POINT_PRE - SHIFT_G);
  *b  = (t00 & MASK_B) << (FIX_POINT_PRE - SHIFT_B);
}
// return fixpoint from VideoSample granularity COLOR_MAX
CV_INLINE void color_to_fix_argb(tFixPoint* a, tFixPoint* r, tFixPoint* g, tFixPoint* b, const tVideoSample t00)
{
  *a  = (t00 & MASK_A) >> (SHIFT_A - FIX_POINT_PRE);
  *r  = (t00 & MASK_R) >> (SHIFT_R - FIX_POINT_PRE);
  *g  = (t00 & MASK_G) << (FIX_POINT_PRE - SHIFT_G);
  *b  = (t00 & MASK_B) << (FIX_POINT_PRE - SHIFT_B);
}
 // return fixpoint from VideoSample granularity 0..FIX_POINT_ONE
CV_INLINE void color_to_fix1_rgb(tFixPoint* r, tFixPoint* g, tFixPoint* b, const tVideoSample t00)
{
  *r  = (t00 & MASK_R) >> (SHIFT_R + COLOR_MAX_LOG2 - FIX_POINT_PRE);
  *g  = (t00 & MASK_G) >> (SHIFT_G + COLOR_MAX_LOG2 - FIX_POINT_PRE);
  *b  = (t00 & MASK_B) << (FIX_POINT_PRE - COLOR_MAX_LOG2);
}
 // return fixpoint from VideoSample granularity 0..FIX_POINT_ONE
CV_INLINE void color_to_fix1_argb(tFixPoint* a, tFixPoint* r, tFixPoint* g, tFixPoint* b, const tVideoSample t00)
{
  *a  = (t00 & MASK_A) >> (SHIFT_A + COLOR_MAX_LOG2 - FIX_POINT_PRE);
  *r  = (t00 & MASK_R) >> (SHIFT_R + COLOR_MAX_LOG2 - FIX_POINT_PRE);
  *g  = (t00 & MASK_G) >> (SHIFT_G + COLOR_MAX_LOG2 - FIX_POINT_PRE);
  *b  = (t00 & MASK_B) << (FIX_POINT_PRE - COLOR_MAX_LOG2);
}
#else
#define color_to_fix_rgb(r, g, b, t00) ( r = (t00 & MASK_R) >> (SHIFT_R - FIX_POINT_PRE), \
g = (t00 & MASK_G) << (FIX_POINT_PRE - SHIFT_G), b = (t00 & MASK_B) << (FIX_POINT_PRE - SHIFT_B) )
#define color_to_fix_argb(a, r, g, b, t00)  (a  = (t00 & MASK_A) >> (SHIFT_A - FIX_POINT_PRE), color_to_fix_rgb(r, g, b, t00) )


#define color_to_fix1_rgb(r, g, b, t00) ( r  = (t00 & MASK_R) >> (SHIFT_R + COLOR_MAX_LOG2 - FIX_POINT_PRE), \
  g = (t00 & MASK_G) >> (SHIFT_G + COLOR_MAX_LOG2 - FIX_POINT_PRE), b = (t00 & MASK_B) << (FIX_POINT_PRE - COLOR_MAX_LOG2) )
#define color_to_fix1_argb(a, r, g, b, t00) (a  = (t00 & MASK_A) >> (SHIFT_A + COLOR_MAX_LOG2 - FIX_POINT_PRE), color_to_fix1_rgb(r, g, b, t00))

#endif
// ----- FP24 ---- floating point z-buffer
#if 1
typedef f32 fp24;
#else
struct fp24 {
  u32 v;
  fp24() {}
  fp24(const f32 f) {
    f32 y = f + 1.f;
    v = ((u32&)y) & 0x7FFFFF; // last 23 bits
  }
  void operator=(const f32 f) {
    f32 y = f + 1.f;
    v = ((u32&)y) & 0x7FFFFF; // last 23 bits
  }
  void operator+=(const fp24& other) {
    v += other.v;
  }
  operator f32() const {
    f32 r = FR(v);
    return r + 1.f;
  }
};
#endif
// ------------------------ Internal Texture -----------------------------
typedef struct CSoftwareTexture2 CSoftwareTexture2;
typedef struct sInternalTexture {
  u32 textureXMask;
  u32 textureYMask;
  u32 pitchlog2;
  void* data;
  CSoftwareTexture2* Texture;
  s32 lodLevel;
} sInternalTexture;
// get video sample plain
CV_INLINE tVideoSample getTexel_plain(const sInternalTexture* t, const tFixPointu tx, const tFixPointu ty)
{
  u32 ofs;
  ofs = ((ty & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  ofs |= (tx & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  // texel
  return *((tVideoSample*)((u8*) t->data + ofs));
}
// get video sample to fix
CV_INLINE void getTexel_fix_rgb(tFixPoint* r, tFixPoint* g, tFixPoint* b, const sInternalTexture* t, tFixPointu tx, tFixPointu ty )
{
  tVideoSample t00;
  u32 ofs;
  ofs = ((ty & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  ofs |= (tx & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  // texel
  t00 = *((tVideoSample*)((u8*) t->data + ofs));
  *r  = (t00 & MASK_R) >> (SHIFT_R - FIX_POINT_PRE);
  *g  = (t00 & MASK_G) << (FIX_POINT_PRE - SHIFT_G);
  *b  = (t00 & MASK_B) << (FIX_POINT_PRE - SHIFT_B);
}
// get video sample to fixpoint
CV_INLINE void getTexel_fix_a(tFixPoint* a, const sInternalTexture* t, tFixPointu tx, tFixPointu ty )
{
  tVideoSample t00;
  u32 ofs;
  ofs = ((ty & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  ofs |= (tx & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  // texel
  t00 = *((tVideoSample*)((u8*) t->data + ofs));
  *a  = (t00 & MASK_A) >> (SHIFT_A - FIX_POINT_PRE);
}
CV_INLINE void getSample_texture_dither(tFixPoint* r, tFixPoint* g, tFixPoint* b, const sInternalTexture* t, const tFixPointu tx, const tFixPointu ty, const u32 x, const u32 y)
{
  static const tFixPointu dithermask[] = {
    0x00, 0x80, 0x20, 0xa0,
    0xc0, 0x40, 0xe0, 0x60,
    0x30, 0xb0, 0x10, 0x90,
    0xf0, 0x70, 0xd0, 0x50
  };
  tVideoSample t00;
  const u32 index = (y & 3) << 2 | (x & 3);
  const tFixPointu _ntx = (tx + dithermask [ index ]) & t->textureXMask;
  const tFixPointu _nty = (ty + dithermask [ index ]) & t->textureYMask;
  u32 ofs;
  ofs = ((_nty) >> FIX_POINT_PRE) << t->pitchlog2;
  ofs |= (_ntx) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  // texel
  t00 = *((tVideoSample*)((u8*) t->data + ofs));
  *r   = (t00 & MASK_R) >> (SHIFT_R - FIX_POINT_PRE);
  *g   = (t00 & MASK_G) << (FIX_POINT_PRE - SHIFT_G);
  *b   = (t00 & MASK_B) << (FIX_POINT_PRE - SHIFT_B);
}
/*
  load a sample from internal texture at position tx,ty to fixpoint
*/
#ifndef SOFTWARE_DRIVER_2_BILINEAR
// get Sample linear == getSample_fixpoint
CV_INLINE void getSample_texture_rgb(tFixPoint* r, tFixPoint* g, tFixPoint* b, const sInternalTexture* t, tFixPointu tx, tFixPointu ty )
{
  tVideoSample t00;
  u32 ofs;
  ofs = ((ty & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  ofs |= (tx & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  // texel
  t00 = *((tVideoSample*)((u8*) t->data + ofs));
  *r   = (t00 & MASK_R) >> (SHIFT_R - FIX_POINT_PRE);
  *g   = (t00 & MASK_G) << (FIX_POINT_PRE - SHIFT_G);
  *b   = (t00 & MASK_B) << (FIX_POINT_PRE - SHIFT_B);
}
CV_INLINE void getSample_texture_argb(tFixPointu* a, tFixPointu* r, tFixPointu* g, tFixPointu* b, const sInternalTexture* t, const tFixPointu tx, const tFixPointu ty)
{
  tVideoSample t00;
  u32 ofs;
  ofs = ((ty & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  ofs |= (tx & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  // texel
  t00 = *((tVideoSample*)((u8*) t->data + ofs));
  *a  = (t00 & MASK_A) >> (SHIFT_A - FIX_POINT_PRE);
  *r  = (t00 & MASK_R) >> (SHIFT_R - FIX_POINT_PRE);
  *g  = (t00 & MASK_G) << (FIX_POINT_PRE - SHIFT_G);
  *b  = (t00 & MASK_B) << (FIX_POINT_PRE - SHIFT_B);
}
#else
// get sample linear
CV_INLINE void getSample_linear_rgb(tFixPointu* r, tFixPointu* g, tFixPointu* b, const sInternalTexture* t, const tFixPointu tx, const tFixPointu ty)
{
  tVideoSample t00;
  u32 ofs;
  ofs = ((ty & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  ofs |= (tx & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  // texel
  t00 = *((tVideoSample*)((u8*) t->data + ofs));
  *r  = (t00 & MASK_R) >> SHIFT_R;
  *g  = (t00 & MASK_G) >> SHIFT_G;
  *b  = (t00 & MASK_B);
}
// get Sample bilinear
CV_INLINE void getSample_texture_rgb(tFixPoint* r, tFixPoint* g, tFixPoint* b, const sInternalTexture* t, const tFixPointu tx, const tFixPointu ty)
{
  tFixPointu r00, g00, b00;
  tFixPointu r01, g01, b01;
  tFixPointu r10, g10, b10;
  tFixPointu r11, g11, b11;
#if 0
  getSample_linear_rgb(r00, g00, b00, t, tx, ty);
  getSample_linear_rgb(r10, g10, b10, t, tx + FIX_POINT_ONE, ty);
  getSample_linear_rgb(r01, g01, b01, t, tx, ty + FIX_POINT_ONE);
  getSample_linear_rgb(r11, g11, b11, t, tx + FIX_POINT_ONE, ty + FIX_POINT_ONE);
#else
  u32 o0, o1, o2, o3;
  tVideoSample t00;
  o0 = (((ty) & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  o1 = (((ty + FIX_POINT_ONE) & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  o2 = ((tx) & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  o3 = ((tx + FIX_POINT_ONE) & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  t00 = *((tVideoSample*)((u8*) t->data + (o0 | o2)));
  r00  = (t00 & MASK_R) >> SHIFT_R;
  g00  = (t00 & MASK_G) >> SHIFT_G;
  b00   = (t00 & MASK_B);
  t00 = *((tVideoSample*)((u8*) t->data + (o0 | o3)));
  r10  = (t00 & MASK_R) >> SHIFT_R;
  g10  = (t00 & MASK_G) >> SHIFT_G;
  b10   = (t00 & MASK_B);
  t00 = *((tVideoSample*)((u8*) t->data + (o1 | o2)));
  r01  = (t00 & MASK_R) >> SHIFT_R;
  g01  = (t00 & MASK_G) >> SHIFT_G;
  b01   = (t00 & MASK_B);
  t00 = *((tVideoSample*)((u8*) t->data + (o1 | o3)));
  r11  = (t00 & MASK_R) >> SHIFT_R;
  g11  = (t00 & MASK_G) >> SHIFT_G;
  b11   = (t00 & MASK_B);
#endif
  {
    const tFixPointu txFract = tx & FIX_POINT_FRACT_MASK;
    const tFixPointu txFractInv = FIX_POINT_ONE - txFract;
    const tFixPointu tyFract = ty & FIX_POINT_FRACT_MASK;
    const tFixPointu tyFractInv = FIX_POINT_ONE - tyFract;
    const tFixPointu w00 = imulFixu(txFractInv, tyFractInv);
    const tFixPointu w10 = imulFixu(txFract  , tyFractInv);
    const tFixPointu w01 = imulFixu(txFractInv, tyFract);
    const tFixPointu w11 = imulFixu(txFract  , tyFract);
    *r = (r00 * w00) +(r01 * w01) +(r10 * w10) +(r11 * w11);
    *g = (g00 * w00) +(g01 * w01) +(g10 * w10) +(g11 * w11);
    *b = (b00 * w00) +(b01 * w01) +(b10 * w10) +(b11 * w11);
  }
}
// get sample linear
CV_INLINE void getSample_linear_argb(tFixPointu* a, tFixPointu* r, tFixPointu* g, tFixPointu* b, const sInternalTexture* t, tFixPointu tx, tFixPointu ty)
{
  tVideoSample t00;
  u32 ofs;
  ofs = ((ty & t->textureYMask) >> FIX_POINT_PRE) << t->pitchlog2;
  ofs |= (tx & t->textureXMask) >> (FIX_POINT_PRE - VIDEO_SAMPLE_GRANULARITY);
  // texel
  t00 = *((tVideoSample*)((u8*) t->data + ofs));
  *a  = (t00 & MASK_A) >> SHIFT_A;
  *r  = (t00 & MASK_R) >> SHIFT_R;
  *g  = (t00 & MASK_G) >> SHIFT_G;
  *b  = (t00 & MASK_B);
}
// get Sample bilinear
CV_INLINE void getSample_texture_argb(tFixPoint* a, tFixPoint* r, tFixPoint* g, tFixPoint* b, const sInternalTexture* t, tFixPointu tx, tFixPointu ty)
{
  tFixPointu a00, r00, g00, b00;
  tFixPointu a01, r01, g01, b01;
  tFixPointu a10, r10, g10, b10;
  tFixPointu a11, r11, g11, b11;
  getSample_linear_argb(&a00, &r00, &g00, &b00, t, tx, ty);
  getSample_linear_argb(&a10, &r10, &g10, &b10, t, tx + FIX_POINT_ONE, ty);
  getSample_linear_argb(&a01, &r01, &g01, &b01, t, tx, ty + FIX_POINT_ONE);
  getSample_linear_argb(&a11, &r11, &g11, &b11, t, tx + FIX_POINT_ONE, ty + FIX_POINT_ONE);
  {
    const tFixPointu txFract = tx & FIX_POINT_FRACT_MASK;
    const tFixPointu txFractInv = FIX_POINT_ONE - txFract;
    const tFixPointu tyFract = ty & FIX_POINT_FRACT_MASK;
    const tFixPointu tyFractInv = FIX_POINT_ONE - tyFract;
    const tFixPointu w00 = imulFixu(txFractInv, tyFractInv);
    const tFixPointu w10 = imulFixu(txFract  , tyFractInv);
    const tFixPointu w01 = imulFixu(txFractInv, tyFract);
    const tFixPointu w11 = imulFixu(txFract  , tyFract);
    *a = (a00 * w00) +(a01 * w01) +(a10 * w10) +(a11 * w11);
    *r = (r00 * w00) +(r01 * w01) +(r10 * w10) +(r11 * w11);
    *g = (g00 * w00) +(g01 * w01) +(g10 * w10) +(g11 * w11);
    *b = (b00 * w00) +(b01 * w01) +(b10 * w10) +(b11 * w11);
  }
}
#endif
//! 2D Intersection test
CV_INLINE bool iRect_intersect(IRECT* dest, const IRECT* a, const IRECT* b)
{
  dest->l = s32_max(a->l, b->l);
  dest->t = s32_max(a->t, b->t);
  dest->r = s32_min(a->r, b->r);
  dest->b = s32_min(a->b, b->b);
  return dest->l < dest->r && dest->t < dest->b;
}
// some 1D defines
typedef struct sIntervall {
  s32 start;
  s32 end;
} sIntervall;
// returning intersection width
CV_INLINE s32 intervall_intersect_test(const sIntervall* a, const sIntervall* b)
{
  return s32_min(a->end, b->end) - s32_max(a->start, b->start);
}

typedef enum E_TRANSFORMATION_STATE_BURNING_VIDEO {
  ETS_VIEW_PROJECTION = ETS_COUNT,
    ETS_CURRENT,
    ETS_CLIPSCALE,
    ETS_VIEW_INVERSE,
    ETS_WORLD_INVERSE,
    ETS_COUNT_BURNING
} E_TRANSFORMATION_STATE_BURNING_VIDEO;
typedef enum E_TRANSFORMATION_FLAG {
  ETF_IDENTITY = 1,
    ETF_TEXGEN_CAMERA_NORMAL = 2,
    ETF_TEXGEN_CAMERA_REFLECTION = 4,
} E_TRANSFORMATION_FLAG;
typedef float2 sVec2;
typedef float3 sVec3;
typedef float4 sVec4;

typedef struct SBurningShaderLight {
  //SLight org;
  bool LightIsOn;
  E_LIGHT_TYPE Type;
  f32 radius;
  f32 linearAttenuation;
  f32 constantAttenuation;
  f32 quadraticAttenuation;
  sVec4 pos;
  sVec3 AmbientColor;
  sVec3 DiffuseColor;
  sVec3 SpecularColor;
  sVec4 pos_objectspace;
} SBurningShaderLight;
typedef enum eLightFlags {
  ENABLED   = 0x01,
    POINTLIGHT  = 0x02,
    SPECULAR  = 0x04,
    FOG     = 0x08,
    NORMALIZE = 0x10,
    VERTEXTRANSFORM = 0x20,
} eLightFlags;
typedef struct SBurningShaderLightSpace {
  SBurningShaderLight* Light;
  int nLight;
  sVec3 Global_AmbientLight;
  sVec4 FogColor;
  sVec4 campos;
  sVec4 vertex;
  sVec4 normal;
  u32 Flags;
} SBurningShaderLightSpace;
typedef struct SBurningShaderMaterial {
  SMaterial org;
  sVec3 AmbientColor;
  sVec3 DiffuseColor;
  sVec3 SpecularColor;
  sVec3 EmissiveColor;
} SBurningShaderMaterial;
enum EBurningFFShader {
  ETR_FLAT = 0,
    ETR_FLAT_WIRE,
    ETR_GOURAUD,
    ETR_GOURAUD_WIRE,
    ETR_TEXTURE_FLAT,
    ETR_TEXTURE_FLAT_WIRE,
    ETR_TEXTURE_GOURAUD,
    ETR_TEXTURE_GOURAUD_WIRE,
    ETR_TEXTURE_GOURAUD_NOZ,
    ETR_TEXTURE_GOURAUD_ADD,
    ETR_TEXTURE_GOURAUD_ADD_NO_Z,
    ETR_TEXTURE_GOURAUD_VERTEX_ALPHA,
    ETR_TEXTURE_GOURAUD_LIGHTMAP_M1,
    ETR_TEXTURE_GOURAUD_LIGHTMAP_M2,
    ETR_TEXTURE_GOURAUD_LIGHTMAP_M4,
    ETR_TEXTURE_LIGHTMAP_M4,
    ETR_TEXTURE_GOURAUD_DETAIL_MAP,
    ETR_TEXTURE_GOURAUD_LIGHTMAP_ADD,
    ETR_GOURAUD_ALPHA,
    ETR_GOURAUD_ALPHA_NOZ,
    ETR_TEXTURE_GOURAUD_ALPHA,
    ETR_TEXTURE_GOURAUD_ALPHA_NOZ,
    ETR_NORMAL_MAP_SOLID,
    ETR_STENCIL_SHADOW,
    ETR_TEXTURE_BLEND,
    ETR_REFERENCE,
    ETR_INVALID,
    ETR2_COUNT
};
#if 0
enum ETriangleRenderer {
  ETR_FLAT = 0,
    ETR_FLAT_WIRE,
    ETR_GOURAUD,
    ETR_GOURAUD_WIRE,
    ETR_TEXTURE_FLAT,
    ETR_TEXTURE_FLAT_WIRE,
    ETR_TEXTURE_GOURAUD,
    ETR_TEXTURE_GOURAUD_WIRE,
    ETR_TEXTURE_GOURAUD_NOZ,
    ETR_TEXTURE_GOURAUD_ADD,
    ETR_COUNT
};
#else
enum {ETR_COUNT = ETR_TEXTURE_GOURAUD_ADD + 1};
typedef enum EBurningFFShader ETriangleRenderer;
#endif
typedef enum e4DVertexFlag {
  VERTEX4D_INSIDE   = 0x0000003F,
  VERTEX4D_CLIPMASK = 0x0000003F,
  VERTEX4D_PROJECTED  = 0x00000100,
  VERTEX4D_FORMAT_MASK      = 0xFFFF0000,
  VERTEX4D_FORMAT_MASK_TEXTURE  = 0x000F0000,
  VERTEX4D_FORMAT_TEXTURE_1   = 0x00010000,
  VERTEX4D_FORMAT_TEXTURE_2   = 0x00020000,
  VERTEX4D_FORMAT_TEXTURE_3   = 0x00030000,
  VERTEX4D_FORMAT_TEXTURE_4   = 0x00040000,
  VERTEX4D_FORMAT_MASK_COLOR    = 0x00F00000,
  VERTEX4D_FORMAT_COLOR_1     = 0x00100000,
  VERTEX4D_FORMAT_COLOR_2     = 0x00200000,
  VERTEX4D_FORMAT_MASK_BUMP   = 0x0F000000,
  VERTEX4D_FORMAT_BUMP_DOT3   = 0x01000000,
} e4DVertexFlag;
#define MATERIAL_MAX_COLORS   1
#define BURNING_MATERIAL_MAX_TEXTURES 2
#define BURNING_MATERIAL_MAX_TANGENT  1
// dummy Vertex. used for calculation vertex memory size
typedef struct s4DVertex_proxy {
  u32 flag;
  sVec4 Pos;
  sVec2 Tex[BURNING_MATERIAL_MAX_TEXTURES];
#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
  sVec4 Color[MATERIAL_MAX_COLORS];
#endif
  sVec3 LightTangent[BURNING_MATERIAL_MAX_TANGENT];
} s4DVertex_proxy;
#define SIZEOF_SVERTEX  64
#define SIZEOF_SVERTEX_LOG2 6

// Internal BurningVideo Vertex
typedef struct s4DVertex {
  u32 flag;
  sVec4 Pos;
  sVec2 Tex[ BURNING_MATERIAL_MAX_TEXTURES ];
#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
  sVec4 Color[ MATERIAL_MAX_COLORS ];
#endif
  sVec3 LightTangent[BURNING_MATERIAL_MAX_TANGENT];
  //u8 fill [ SIZEOF_SVERTEX - sizeof (s4DVertex_proxy) ];
} s4DVertex;
// f = a * t + b * ( 1 - t )
#define INTERPOLATE(a, b, t)   (b + ((a - b) * t))
void interpolate2fv(float* v, const float* a, const float* b, float t) {
  v[0] = INTERPOLATE(a[0], b[0], t);
  v[1] = INTERPOLATE(a[1], b[1], t);
}
void interpolate3fv(float* v, const float* a, const float* b, float t) {
  v[0] = INTERPOLATE(a[0], b[0], t);
  v[1] = INTERPOLATE(a[1], b[1], t);
  v[2] = INTERPOLATE(a[2], b[2], t);
}
void interpolate4fv(float* v, const float* a, const float* b, float t) {
  v[0] = INTERPOLATE(a[0], b[0], t);
  v[1] = INTERPOLATE(a[1], b[1], t);
  v[2] = INTERPOLATE(a[2], b[2], t);
  v[3] = INTERPOLATE(a[3], b[3], t);
}
void s4DVertex_interpolate(s4DVertex* s, const s4DVertex* b, const s4DVertex* a, f32 t) {
  u32 i;
  u32 size;
  interpolate4fv(s->Pos, a->Pos, b->Pos, t);
#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
  size = (s->flag & VERTEX4D_FORMAT_MASK_COLOR) >> 20;
  for (i = 0; i != size; ++i) {
    interpolate4fv(s->Color[i], a->Color[i], b->Color[i], t);
  }
#endif
  size = (s->flag & VERTEX4D_FORMAT_MASK_TEXTURE) >> 16;
  for (i = 0; i != size; ++i) {
    interpolate2fv(s->Tex[i], a->Tex[i], b->Tex[i], t);
  }
  size = (s->flag & VERTEX4D_FORMAT_MASK_BUMP) >> 24;
  for (i = 0; i != size; ++i) {
    interpolate3fv(s->LightTangent[i], a->LightTangent[i], b->LightTangent[i], t);
  }
}
//! constructor
typedef enum eTex2Flags {
  GEN_MIPMAP  = 1,
    IS_RENDERTARGET = 2,
    NP2_SIZE  = 4,
    HAS_ALPHA = 8
} eTex2Flags;
typedef  struct CBurningVideoDriver CBurningVideoDriver;
typedef struct CSoftwareTexture2 {
  ITEXTURE_DEF(VFUNDEF)
  f32 OrigImageDataSizeInPixels;
  ISIZE OrigSize;
  IImage* MipMap[SOFTWARE_DRIVER_2_MIPMAPPING_MAX];
  u32 MipMapLOD;
  u32 Flags;
  ECOLOR_FORMAT OriginalFormat;
} CSoftwareTexture2;
typedef struct IDepthBuffer {
  u8* Data;
  ISIZE Size;
  u32 TotalSize;
  u32 Pitch;
} IDepthBuffer;
static const tFixPointu dithermask[ 4 * 4] = {
  0x00, 0x80, 0x20, 0xa0,
    0xc0, 0x40, 0xe0, 0x60,
    0x30, 0xb0, 0x10, 0x90,
    0xf0, 0x70, 0xd0, 0x50
};

#if 0

IBurningShader::IBurningShader(CBurningVideoDriver* driver)
{
#ifdef _DEBUG
  setDebugName("IBurningShader");
#endif
  int i;
  for (i = 0; i != BURNING_MATERIAL_MAX_TEXTURES; ++i) {
    IT[i].Texture = 0;
  }
  Driver = driver;
  RenderTarget = 0;
  ColorMask = COLOR_BRIGHT_WHITE;
  DepthBuffer = (CDepthBuffer*) driver->getDepthBuffer();
  if (DepthBuffer) {
    DepthBuffer->grab();
  }
  Stencil = (CStencilBuffer*) driver->getStencilBuffer();
  if (Stencil) {
    Stencil->grab();
  }
}
//! destructor
IBurningShader::~IBurningShader()
{
  if (RenderTarget) {
    RenderTarget->drop();
  }
  if (DepthBuffer) {
    DepthBuffer->drop();
  }
  if (Stencil) {
    Stencil->drop();
  }
  int i;
  for (i = 0; i != BURNING_MATERIAL_MAX_TEXTURES; ++i) {
    if (IT[i].Texture) {
      IT[i].Texture->drop();
    }
  }
}
//! sets a render target
void IBurningShader::setRenderTarget(IImage* surface, const IRECT& viewPort)
{
  if (RenderTarget) {
    RenderTarget->drop();
  }
  RenderTarget = (CImage*) surface;
  if (RenderTarget) {
    RenderTarget->grab();
    //(tVideoSample*)RenderTarget->lock() = (tVideoSample*)RenderTarget->lock();
    //(fp24*) DepthBuffer->lock() = DepthBuffer->lock();
  }
}
//! sets the Texture
void IBurningShader::setTextureParam(u32 stage, CSoftwareTexture2* texture, s32 lodLevel)
{
  sInternalTexture* it = &IT[stage];
  if (it->Texture) {
    it->Texture->drop();
  }
  it->Texture = texture;
  if (it->Texture) {
    it->Texture->grab();
    // select mignify and magnify ( lodLevel )
    //SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS
    it->lodLevel = lodLevel;
    it->data = (tVideoSample*) it->Texture->lock(ETLM_READ_ONLY,
      s32_clamp(lodLevel + SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS, 0, SOFTWARE_DRIVER_2_MIPMAPPING_MAX - 1));
    // prepare for optimal fixpoint
    it->pitchlog2 = s32_log2_s32(it->Texture->getPitch());
    const dimension2du& dim = it->Texture->getSize();
    it->textureXMask = s32_to_fixPoint(dim.Width - 1) & FIX_POINT_UNSIGNED_MASK;
    it->textureYMask = s32_to_fixPoint(dim.Height - 1) & FIX_POINT_UNSIGNED_MASK;
  }
}
#endif
#define VERTEXCACHE_ELEMENT 16
#define VERTEXCACHE_MISS 0xFFFFFFFF
typedef struct SAlignedVertex {
  s4DVertex* data;
  u8 mem[((VERTEXCACHE_ELEMENT * 2)<<SIZEOF_SVERTEX_LOG2) + 128];
  u32 ElementSize;
} SAlignedVertex;
typedef struct SVSize {
  u32 Format;
  u32 Pitch;
  u32 TexSize;
} SVSize;
// a cache info
typedef struct SCacheInfo {
  u32 index;
  u32 hit;
} SCacheInfo;
typedef struct SVertexCache {
  //SVertexCache(): mem(VERTEXCACHE_ELEMENT * 2, 128) {}
  SCacheInfo info[VERTEXCACHE_ELEMENT];
  // Transformed and lite, clipping state
  // + Clipped, Projected
  SAlignedVertex mem;
  // source
  void* vertices;
  u32 vertexCount;
  const void* indices;
  u32 indexCount;
  u32 indicesIndex;
  u32 indicesRun;
  // primitives consist of x vertices
  u32 primitivePitch;
  u32 vType;    //E_VERTEX_TYPE
  u32 pType;    //E_PRIMITIVE_TYPE
  u32 iType;    //E_INDEX_TYPE iType
} SVertexCache;
REALINLINE void swapVertexPointer(const s4DVertex** v1, const s4DVertex** v2)
{
  const s4DVertex* b = *v1;
  *v1 = *v2;
  *v2 = b;
}
// ------------------------ Internal Scanline Rasterizer -----------------------------
// internal scan convert
typedef struct sScanConvertData {
  u8 left;      // major edge left/right
  u8 right;     // !left
  f32 invDeltaY[3]; // inverse edge delta y
  f32 x[2];     // x coordinate
  f32 slopeX[2];    // x slope along edges
#if defined ( SOFTWARE_DRIVER_2_USE_WBUFFER ) || defined ( SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT )
  f32 w[2];     // w coordinate
  fp24 slopeW[2];   // w slope along edges
#else
  f32 z[2];     // z coordinate
  f32 slopeZ[2];    // z slope along edges
#endif
  sVec4 c[MATERIAL_MAX_COLORS][2];      // color
  sVec4 slopeC[MATERIAL_MAX_COLORS][2]; // color slope along edges
  sVec2 t[BURNING_MATERIAL_MAX_TEXTURES][2];    // texture
  sVec2 slopeT[BURNING_MATERIAL_MAX_TEXTURES][2]; // texture slope along edges
  sVec3 l[BURNING_MATERIAL_MAX_TANGENT][2];   // Light Tangent
  sVec3 slopeL[BURNING_MATERIAL_MAX_TEXTURES][2]; // tanget slope along edges
} sScanConvertData;
// passed to scan Line
typedef struct sScanLineData {
  s32 y;        // y position of scanline
  f32 x[2];     // x start, x end of scanline
#if defined ( SOFTWARE_DRIVER_2_USE_WBUFFER ) || defined ( SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT )
  f32 w[2];     // w start, w end of scanline
#else
  f32 z[2];     // z start, z end of scanline
#endif
#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
  sVec4 c[MATERIAL_MAX_COLORS][2];      // color start, color end of scanline
#endif
  sVec2 t[BURNING_MATERIAL_MAX_TEXTURES][2];    // texture start, texture end of scanline
  sVec3 l[BURNING_MATERIAL_MAX_TANGENT][2];   // Light Tangent start, end
} sScanLineData;
// passed to pixel Shader
struct sPixelShaderData {
  tVideoSample* dst;
  fp24* z;
  s32 xStart;
  s32 xEnd;
  s32 dx;
  s32 i;
};
/*
  load a color value
*/
CV_INLINE void getTexel_plain2_rgb(tFixPoint* r, tFixPoint* g, tFixPoint* b, const sVec4 v)
{
  *r = tofix(v[1], FIX_POINT_F32_MUL);
  *g = tofix(v[2], FIX_POINT_F32_MUL);
  *b = tofix(v[3], FIX_POINT_F32_MUL);
}
/*
  load a color value
*/
CV_INLINE void getSample_color_argb(tFixPoint* a, tFixPoint* r, tFixPoint* g, tFixPoint* b, const sVec4 v)
{
  *a = tofix(v[0], FIX_POINT_F32_MUL);
  *r = tofix(v[1], COLOR_MAX * FIX_POINT_F32_MUL);
  *g = tofix(v[2], COLOR_MAX * FIX_POINT_F32_MUL);
  *b = tofix(v[3], COLOR_MAX * FIX_POINT_F32_MUL);
}
/*
  load a color value
*/
CV_INLINE void getSample_color_rgb(tFixPoint* r, tFixPoint* g, tFixPoint* b, const sVec4 v)
{
  *r = tofix(v[1], COLOR_MAX * FIX_POINT_F32_MUL);
  *g = tofix(v[2], COLOR_MAX * FIX_POINT_F32_MUL);
  *b = tofix(v[3], COLOR_MAX * FIX_POINT_F32_MUL);
}
/*
  load a color value
*/
CV_INLINE void getSample_color_rgb2(tFixPoint* r, tFixPoint* g, tFixPoint* b, const sVec4 v, const f32 mulby)
{
  *r = tofix(v[1], mulby);
  *g = tofix(v[2], mulby);
  *b = tofix(v[3], mulby);
}

typedef struct IDepthBuffer IStencilBuffer;
typedef struct IBurningShader IBurningShader;
typedef void (*tFragmentShader)(IBurningShader* s);
struct IBurningShader {
  //! sets a render target
  void (*setRenderTarget)(IImage* surface, const IRECT* viewPort);
  //! sets the Texture
  void (*setTextureParam)(u32 stage, CSoftwareTexture2* texture, s32 lodLevel);
  void (*drawTriangle)(const s4DVertex* a, const s4DVertex* b, const s4DVertex* c);
  void (*drawLine)(const s4DVertex* a, const s4DVertex* b);
  void (*setParam)(u32 index, f32 value);
  void (*setZCompareFunc)(u32 func);
  void (*setMaterial)(const SBurningShaderMaterial* material);
  CBurningVideoDriver* Driver;
  IImage* RenderTarget;
  IDepthBuffer* DepthBuffer;
  IStencilBuffer* Stencil;
  tVideoSample ColorMask;
  sInternalTexture IT[ BURNING_MATERIAL_MAX_TEXTURES ];
  sScanConvertData scan[1];
  sScanLineData line[1];
  // for CTRTextureBlend
  u32 ZCompare; // DEFAULT(1)
  tFragmentShader fragmentShader;
  // for CTRTextureGouraudAlphaNoZ
  u32 AlphaRef; // DEFAULT(0)
};

// Ïà½»
bool isRectCollided(const IRECT* r, const IRECT* other) {
  return (r->b > other->t && r->t < other->b && r->r > other->l && r->l < other->r);
}

//! vertauscht zwei vertizen
CV_INLINE void swapVertices(const S2DVertex** v1, const S2DVertex** v2) {
  const S2DVertex* b = *v1;
  *v1 = *v2;
  *v2 = b;
}

void IImage_free(IImage* im) {
  if (im->Data) {
    free(im->Data);
  }
  memset(im, 0, sizeof(IImage));
}
void IImage_setsize(IImage* im, ECOLOR_FORMAT fmt, ISIZE sz) {
  int cn = getImageInfoFromFormat(fmt)->bpp>>3;
  int step = cn*sz.w;
  int n = step*sz.h;
  myrealloc(&im->Data, n);
  im->Size = sz;
  im->Format = fmt;
  im->BytesPerPixel = cn;
  im->Pitch = step;
}


void SMaterial_init(SMaterial* s) {
   s->MaterialType = (EMT_SOLID);
   s->AmbientColor = _ARGB(255, 255, 255, 255);
   s->DiffuseColor = _ARGB(255, 255, 255, 255);
   s->EmissiveColor = _ARGB(0, 0, 0, 0);
   s->SpecularColor = _ARGB(255, 255, 255, 255);
   s->Shininess = (0.0f);
   s->MaterialTypeParam = (0.0f);
   s->MaterialTypeParam2 = (0.0f);
   s->Thickness = (1.0f);
   s->ZBuffer = (ECFN_LESSEQUAL);
   s->AntiAliasing = (EAAM_SIMPLE);
   s->ColorMask = (ECP_ALL);
   s->ColorMaterial = (ECM_DIFFUSE);
   s->BlendOperation = (EBO_NONE);
   s->PolygonOffsetFactor = (0);
   s->PolygonOffsetDirection = (EPO_FRONT);
   s->Wireframe = (false);
   s->PointCloud = (false);
   s->GouraudShading = (true);
   s->Lighting = (true);
   s->ZWriteEnable = (true);
   s->BackfaceCulling = (true);
   s->FrontfaceCulling = (false);
   s->FogEnable = (false);
   s->NormalizeNormals = (false);
   s->UseMipMaps = (true);
}

typedef struct ITriangleRenderer ITriangleRenderer;
#define ITRIANGLERENDERER_DEF(TT) \
TT(void, setRenderTarget, (IImage* surface, const IRECT* viewPort)) \
TT(void, setBackfaceCulling, (bool enabled DEFAULT(true))) \
TT(void, setTexture, (IImage* texture)) \
TT(void, drawIndexedTriangleList, (S2DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount))

struct ITriangleRenderer { //! sets a render target
  //IImage* RenderTarget;
  //IZBuffer* ZBuffer;
  IRECT ViewPortRect;
  s32 SurfaceWidth;
  s32 SurfaceHeight;
  bool BackFaceCullingEnabled;
  TZBufferType* lockedZBuffer;
  u16* lockedSurface;
  u16* lockedTexture;
  s32 lockedTextureWidth;
  s32 textureXMask, textureYMask;
  //IImage* Texture;
  //ITRIANGLERENDERER_DEF(VFUNDEF)
};
