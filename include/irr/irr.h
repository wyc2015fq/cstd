// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __IRR_TYPES_H_INCLUDED__
#define __IRR_TYPES_H_INCLUDED__
//! An enum for all types of drivers the Irrlicht Engine supports.
typedef enum E_DRIVER_TYPE {
  EDT_NULL,
EDT_SOFTWARE,
EDT_BURNINGSVIDEO,
EDT_DIRECT3D8,
EDT_DIRECT3D9,
EDT_OPENGL,
EDT_COUNT
} E_DRIVER_TYPE;
//! An enum for the different device types supported by the Irrlicht Engine.
typedef enum E_DEVICE_TYPE {
  EIDT_WIN32,
EIDT_WINCE,
EIDT_X11,
EIDT_OSX,
EIDT_SDL,
EIDT_FRAMEBUFFER,
EIDT_CONSOLE,
EIDT_BEST
} E_DEVICE_TYPE;

CV_INLINE f32 radToDegf(f32 radians)
{
  return RADTODEG * radians;
}
//! Utility function to convert a radian value to degrees
/** Provided as it can be clearer to write radToDeg(X) than RADTODEG * X
\param radians  The radians value to convert to degrees.
*/
CV_INLINE f64 radToDeg(f64 radians)
{
  return RADTODEG64 * radians;
}
//! Utility function to convert a degrees value to radians
/** Provided as it can be clearer to write degToRad(X) than DEGTORAD * X
\param degrees  The degrees value to convert to radians.
*/
CV_INLINE f32 degToRadf(f32 degrees)
{
  return DEGTORAD * degrees;
}
//! Utility function to convert a degrees value to radians
/** Provided as it can be clearer to write degToRad(X) than DEGTORAD * X
\param degrees  The degrees value to convert to radians.
*/
CV_INLINE f64 degToRad(f64 degrees)
{
  return DEGTORAD64 * degrees;
}

#define equals(a, b, tolerance)    (((a) + tolerance >= (b)) && ((a) - tolerance <= (b)))
#define equals_f32(a, b) equals(a, b, ROUNDING_ERROR_f32)

CV_INLINE bool equalsByUlp(f32 a, f32 b, int maxUlpDiff)
{
  union FloatIntUnion32 {
    //FloatIntUnion32(float f1 = 0.0f) : f(f1) {}
    // Portable sign-extraction
    //bool sign() const {    return (i >> 31) != 0;  }
    s32 i;
    f32 f;
  } fa, fb;
  int ulpsDiff;
  // Based on the ideas and code from Bruce Dawson on
  // http://www.altdevblogaday.com/2012/02/22/comparing-floating-point-numbers-2012-edition/
  // When floats are interpreted as integers the two nearest possible float numbers differ just
  // by one integer number. Also works the other way round, an integer of 1 interpreted as float
  // is for example the smallest possible float number.
  fa.f = (a);
  fb.f = (b);
  // Different signs, we could maybe get difference to 0, but so close to 0 using epsilons is better.
  if ((fa.i >> 31) != (fb.i >> 31)) { // fa.sign() != fb.sign()
    // Check for equality to make sure +0==-0
    if (fa.i == fb.i) {
      return true;
    }
    return false;
  }
  // Find the difference in ULPs.
  ulpsDiff = ABS(fa.i - fb.i);
  if (ulpsDiff <= maxUlpDiff) {
    return true;
  }
  return false;
}
//! returns if a equals zero, taking rounding errors into account
CV_INLINE bool iszero_f64(const f64 a, const f64 tolerance DEFAULT(ROUNDING_ERROR_f64))
{
  return fabs(a) <= tolerance;
}
//! returns if a equals zero, taking rounding errors into account
CV_INLINE bool iszero_f32(const f32 a, const f32 tolerance DEFAULT(ROUNDING_ERROR_f32))
{
  return fabsf(a) <= tolerance;
}
//! returns if a equals not zero, taking rounding errors into account
CV_INLINE bool isnotzero_f32(const f32 a, const f32 tolerance DEFAULT(ROUNDING_ERROR_f32))
{
  return fabsf(a) > tolerance;
}
//! returns if a equals zero, taking rounding errors into account
CV_INLINE bool iszero_s32(const s32 a, const s32 tolerance DEFAULT(0))
{
  return (a & 0x7ffffff) <= tolerance;
}
//! returns if a equals zero, taking rounding errors into account
CV_INLINE bool iszero_u32(const u32 a, const u32 tolerance DEFAULT(0))
{
  return a <= tolerance;
}
#ifdef __IRR_HAS_S64
//! returns if a equals zero, taking rounding errors into account
CV_INLINE bool iszero_s64(const s64 a, const s64 tolerance DEFAULT(0))
{
  return abs_(a) <= tolerance;
}
#endif
CV_INLINE s32 s32_min(s32 a, s32 b)
{
  const s32 mask = (a - b) >> 31;
  return (a & mask) | (b & ~mask);
}
CV_INLINE s32 s32_max(s32 a, s32 b)
{
  const s32 mask = (a - b) >> 31;
  return (b & mask) | (a & ~mask);
}
CV_INLINE s32 s32_clamp(s32 value, s32 low, s32 high)
{
  return s32_min(s32_max(value, low), high);
}
/*
  float IEEE-754 bit represenation
  0      0x00000000
  1.0    0x3f800000
  0.5    0x3f000000
  3      0x40400000
  +inf   0x7f800000
  -inf   0xff800000
  +NaN   0x7fc00000 or 0x7ff00000
  in general: number = (sign ? -1:1) * 2^(exponent) * 1.(mantissa bits)
*/
#define F32_AS_S32(f)   (*((s32 *) &(f)))
#define F32_AS_U32(f)   (*((u32 *) &(f)))
#define F32_AS_U32_POINTER(f) ( ((u32 *) &(f)))
#define F32_VALUE_0   0x00000000
#define F32_VALUE_1   0x3f800000
#define F32_SIGN_BIT    0x80000000U
#define F32_EXPON_MANTISSA  0x7FFFFFFFU
#ifndef AIR
#define AIR(x)        (IR(x)&0x7fffffff)
#endif
#define MYRANDMAX  (2147483399-1)
typedef struct IRandomizer {
  int my_seed;
} IRandomizer;
//static s32 my_seed = 0x0f0f0f0f;
static s32 myrand(IRandomizer* s)
{
  enum { m = MYRANDMAX + 1, // a non-Mersenne prime
      a = 40692,     // another spectral success story
      q = (m / a),
      r = m % a
       };   // again less than q
  // (a*seed)%m with Schrage's method
  s->my_seed = a * (s->my_seed % q) - r * (s->my_seed / q);
  if (s->my_seed < 0) {
    s->my_seed += m;
  }
  return s->my_seed;
}
//! generates a pseudo random number
static f32 myfrand(IRandomizer* s)
{
  return myrand(s) * (1.f / MYRANDMAX);
}
//! resets the randomizer
static void mysrand(IRandomizer* s, s32 value)
{
  s->my_seed = value;
}
//! integer representation of 1.0
#define IEEE_1_0      0x3f800000
//! integer representation of 255.0
#define IEEE_255_0      0x437f0000
#ifdef IRRLICHT_FAST_MATH
#define F32_LOWER_0(f)    (F32_AS_U32(f) >  F32_SIGN_BIT)
#define F32_LOWER_EQUAL_0(f)  (F32_AS_S32(f) <= F32_VALUE_0)
#define F32_GREATER_0(f)  (F32_AS_S32(f) >  F32_VALUE_0)
#define F32_GREATER_EQUAL_0(f)  (F32_AS_U32(f) <= F32_SIGN_BIT)
#define F32_EQUAL_1(f)    (F32_AS_U32(f) == F32_VALUE_1)
#define F32_EQUAL_0(f)    ( (F32_AS_U32(f) & F32_EXPON_MANTISSA ) == F32_VALUE_0)
// only same sign
#define F32_A_GREATER_B(a,b)  (F32_AS_S32((a)) > F32_AS_S32((b)))
#else
#define F32_LOWER_0(n)    ((n) <  0.0f)
#define F32_LOWER_EQUAL_0(n)  ((n) <= 0.0f)
#define F32_GREATER_0(n)  ((n) >  0.0f)
#define F32_GREATER_EQUAL_0(n)  ((n) >= 0.0f)
#define F32_EQUAL_1(n)    ((n) == 1.0f)
#define F32_EQUAL_0(n)    ((n) == 0.0f)
#define F32_A_GREATER_B(a,b)  ((a) > (b))
#endif
#ifndef REALINLINE
#ifdef _MSC_VER
#define REALINLINE __forceinline
#else
#define REALINLINE CV_INLINE
#endif
#endif
#if defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
// 8-bit bools in borland builder
//! conditional set based on mask and arithmetic shift
REALINLINE u32 if_c_a_else_b(const c8 condition, const u32 a, const u32 b)
{
  return ((-condition >> 7) & (a ^ b)) ^ b;
}
//! conditional set based on mask and arithmetic shift
REALINLINE u32 if_c_a_else_0(const c8 condition, const u32 a)
{
  return (-condition >> 31) & a;
}
#else
//! conditional set based on mask and arithmetic shift
REALINLINE u32 if_c_a_else_b(const s32 condition, const u32 a, const u32 b)
{
  return ((-condition >> 31) & (a ^ b)) ^ b;
}
//! conditional set based on mask and arithmetic shift
REALINLINE u16 if_c_a_else_b_16(const s16 condition, const u16 a, const u16 b)
{
  return ((-condition >> 15) & (a ^ b)) ^ b;
}
//! conditional set based on mask and arithmetic shift
REALINLINE u32 if_c_a_else_0(const s32 condition, const u32 a)
{
  return (-condition >> 31) & a;
}
#endif
/*
  if (condition) state |= m; else state &= ~m;
*/
// 0, or any postive to mask
//s32 conmask = -condition >> 31;
#define setbit_cond(state, condition, mask)  ((state) ^= ((-(condition) >> 31) ^ (state)) & (mask))
#define round_(x)  floorf(x + 0.5f)
REALINLINE void clearFPUException()
{
#ifdef IRRLICHT_FAST_MATH
  return;
#ifdef feclearexcept
  feclearexcept(FE_ALL_EXCEPT);
#elif defined(_MSC_VER)
  __asm fnclex;
#elif defined(__GNUC__) && defined(__x86__)
  __asm__ __volatile__("fclex \n\t");
#else
#  warn clearFPUException not supported.
#endif
#endif
}
// calculate: sqrt ( x )
REALINLINE f32 squarerootf(const f32 f)
{
  return sqrtf(f);
}
// calculate: sqrt ( x )
REALINLINE f64 squareroot(const f64 f)
{
  return sqrt(f);
}
// calculate: 1 / sqrt ( x )
REALINLINE f64 reciprocal_squareroot(const f64 x)
{
  return 1.0 / sqrt(x);
}
// calculate: 1 / sqrtf ( x )
REALINLINE f32 reciprocal_squarerootf(const f32 f)
{
#if defined ( IRRLICHT_FAST_MATH )
#if defined(_MSC_VER)
  // SSE reciprocal square root estimate, accurate to 12 significant
  // bits of the mantissa
  f32 recsqrt;
  __asm rsqrtss xmm0, f           // xmm0 = rsqrtss(f)
  __asm movss recsqrt, xmm0       // return xmm0
  return recsqrt;
  /*
      // comes from Nvidia
      u32 tmp = (u32(IEEE_1_0 << 1) + IEEE_1_0 - *(u32*)&x) >> 1;
      f32 y = *(f32*)&tmp;
      return y * (1.47f - 0.47f * x * y * y);
  */
#else
  return 1.f / sqrtf(f);
#endif
#else // no fast math
  return 1.f / sqrtf(f);
#endif
}
// calculate: 1 / sqrtf( x )
// calculate: 1 / x
REALINLINE f32 reciprocalf(const f32 f)
{
#if defined (IRRLICHT_FAST_MATH)
  // SSE Newton-Raphson reciprocal estimate, accurate to 23 significant
  // bi ts of the mantissa
  // One Newtown-Raphson Iteration:
  // f(i+1) = 2 * rcpss(f) - f * rcpss(f) * rcpss(f)
  f32 rec;
  __asm rcpss xmm0, f               // xmm0 = rcpss(f)
  __asm movss xmm1, f               // xmm1 = f
  __asm mulss xmm1, xmm0            // xmm1 = f * rcpss(f)
  __asm mulss xmm1, xmm0            // xmm2 = f * rcpss(f) * rcpss(f)
  __asm addss xmm0, xmm0            // xmm0 = 2 * rcpss(f)
  __asm subss xmm0, xmm1            // xmm0 = 2 * rcpss(f)
  //        - f * rcpss(f) * rcpss(f)
  __asm movss rec, xmm0             // return xmm0
  return rec;
  //! i do not divide through 0.. (fpu expection)
  // instead set f to a high value to get a return value near zero..
  // -1000000000000.f.. is use minus to stay negative..
  // must test's here (plane.normal dot anything ) checks on <= 0.f
  //u32 x = (-(AIR(f) != 0 ) >> 31 ) & ( IR(f) ^ 0xd368d4a5 ) ^ 0xd368d4a5;
  //return 1.f / FR ( x );
#else // no fast math
  return 1.f / f;
#endif
}
// calculate: 1 / x
REALINLINE f64 reciprocal(const f64 f)
{
  return 1.0 / f;
}
// calculate: 1 / x, low precision allowed
REALINLINE f32 reciprocal_approximf(const f32 f)
{
#if defined( IRRLICHT_FAST_MATH)
  // SSE Newton-Raphson reciprocal estimate, accurate to 23 significant
  // bi ts of the mantissa
  // One Newtown-Raphson Iteration:
  // f(i+1) = 2 * rcpss(f) - f * rcpss(f) * rcpss(f)
  f32 rec;
  __asm rcpss xmm0, f               // xmm0 = rcpss(f)
  __asm movss xmm1, f               // xmm1 = f
  __asm mulss xmm1, xmm0            // xmm1 = f * rcpss(f)
  __asm mulss xmm1, xmm0            // xmm2 = f * rcpss(f) * rcpss(f)
  __asm addss xmm0, xmm0            // xmm0 = 2 * rcpss(f)
  __asm subss xmm0, xmm1            // xmm0 = 2 * rcpss(f)
  //        - f * rcpss(f) * rcpss(f)
  __asm movss rec, xmm0             // return xmm0
  return rec;
  /*
      // SSE reciprocal estimate, accurate to 12 significant bits of
      f32 rec;
      __asm rcpss xmm0, f             // xmm0 = rcpss(f)
      __asm movss rec , xmm0          // return xmm0
      return rec;
  */
  /*
      register u32 x = 0x7F000000 - IR ( p );
      const f32 r = FR ( x );
      return r * (2.0f - p * r);
  */
#else // no fast math
  return 1.f / f;
#endif
}
REALINLINE s32 floor32f(f32 x)
{
#ifdef IRRLICHT_FAST_MATH
  const f32 h = 0.5f;
  s32 t;
#if defined(_MSC_VER)
  __asm {
    fld x
    fsub  h
    fistp t
  }
#elif defined(__GNUC__)
  __asm__ __volatile__(
      "fsub %2 \n\t"
      "fistpl %0"
      : "=m"(t)
      : "t"(x), "f"(h)
      : "st"
  );
#else
#  warn IRRLICHT_FAST_MATH not supported.
  return (s32) floorf(x);
#endif
  return t;
#else // no fast math
  return (s32) floorf(x);
#endif
}
REALINLINE s32 ceil32f(f32 x)
{
#ifdef IRRLICHT_FAST_MATH
  const f32 h = 0.5f;
  s32 t;
#if defined(_MSC_VER)
  __asm {
    fld x
    fadd  h
    fistp t
  }
#elif defined(__GNUC__)
  __asm__ __volatile__(
      "fadd %2 \n\t"
      "fistpl %0 \n\t"
      : "=m"(t)
      : "t"(x), "f"(h)
      : "st"
  );
#else
#  warn IRRLICHT_FAST_MATH not supported.
  return (s32) ceilf(x);
#endif
  return t;
#else // not fast math
  return (s32) ceilf(x);
#endif
}
REALINLINE s32 round32(f32 x)
{
#if defined(IRRLICHT_FAST_MATH)
  s32 t;
#if defined(_MSC_VER)
  __asm {
    fld   x
    fistp t
  }
#elif defined(__GNUC__)
  __asm__ __volatile__(
      "fistpl %0 \n\t"
      : "=m"(t)
      : "t"(x)
      : "st"
  );
#else
#  warn IRRLICHT_FAST_MATH not supported.
  return (s32) round_(x);
#endif
  return t;
#else // no fast math
  return (s32) round_(x);
#endif
}
CV_INLINE f32 f32_max3(const f32 a, const f32 b, const f32 c)
{
  return a > b ? (a > c ? a : c) : (b > c ? b : c);
}
CV_INLINE f32 f32_min3(const f32 a, const f32 b, const f32 c)
{
  return a < b ? (a < c ? a : c) : (b < c ? b : c);
}
CV_INLINE f32 fract(f32 x)
{
  return x - floorf(x);
}
CV_INLINE void normalize3fv(float* v, const float* v1) {
  f64 length = v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2];
  
  if (length != 0) {
    // this check isn't an optimization but prevents getting NAN in the sqrt.
    length = reciprocal_squareroot(length);
    v[0] = (f32)(v1[0] * length);
    v[1] = (f32)(v1[1] * length);
    v[2] = (f32)(v1[2] * length);
  }
}
void rotate2fv(float* v, const float* v1, f64 degrees0, const float2 center)
{
  const f64 degrees = degrees0 * DEGTORAD64;
  const f64 cs = cos(degrees);
  const f64 sn = sin(degrees);
  float2 t;
  if (center) {
    VSUB2(t, v1, center);
    VSET2(v, (f32)(t[0] * cs - t[1] * sn), (f32)(t[0] * sn + t[1] * cs));
    VSUB2(v, v, center);
  }
  else {
    VSET2V(t, v1);
    VSET2(v, (f32)(t[0] * cs - t[1] * sn), (f32)(t[0] * sn + t[1] * cs));
  }
}
static F3VECTOR f3ZERO = {0, 0, 0};
static F3VECTOR f3ONE = {1.f, 1.f, 1.f};
CV_INLINE F3VECTOR f3VECTOR(f32 x, f32 y, f32 z)
{
  F3VECTOR v;
  v.x = x, v.y = y, v.z = z;
  return v;
}
typedef F2VECTOR position2df;
typedef IPOINT position2di;
//#define position2d vector2d
typedef struct F3PLANE {
  float3 Normal;
  f32 D; //! Distance from origin.
} F3PLANE;
typedef struct F3LINE {
  float3 start, end;
} F3LINE;
typedef struct F3AABBOX {
  float3 MinEdge, MaxEdge;
} F3AABBOX;
void F3AABBOX_reset(F3AABBOX* box, const float* v) {
  if (v) {
    VSET3V(box->MinEdge, v);
    VSET3V(box->MaxEdge, v);
  } else {
    VSET3(box->MinEdge, FLT_MAX, FLT_MAX, FLT_MAX);
    VSET3(box->MaxEdge, FLT_MIN, FLT_MIN, FLT_MIN);
  }
}
void F3AABBOX_addInternalPoint(F3AABBOX* box, const float* v) {
  box->MaxEdge[0] = MAX(box->MaxEdge[0], v[0]);
  box->MaxEdge[1] = MAX(box->MaxEdge[0], v[1]);
  box->MaxEdge[2] = MAX(box->MaxEdge[0], v[2]);
  box->MinEdge[0] = MIN(box->MinEdge[0], v[0]);
  box->MinEdge[1] = MIN(box->MinEdge[0], v[1]);
  box->MinEdge[2] = MIN(box->MinEdge[0], v[2]);
}
void F3AABBOX_getCenter(const F3AABBOX* box, float* v) {
  VADD3(v, box->MaxEdge, box->MinEdge);
  VSCALE3(v, v, 0.5f);
}
void F3AABBOX_repair(F3AABBOX* box) {
  float t;
  
  if (box->MinEdge[0] > box->MaxEdge[0]) {
    t = box->MinEdge[0];
    box->MinEdge[0] = box->MaxEdge[0];
    box->MaxEdge[0] = t;
  }
  
  if (box->MinEdge[1] > box->MaxEdge[1]) {
    t = box->MinEdge[1];
    box->MinEdge[1] = box->MaxEdge[1];
    box->MaxEdge[1] = t;
  }
  
  if (box->MinEdge[2] > box->MaxEdge[2]) {
    t = box->MinEdge[2];
    box->MinEdge[2] = box->MaxEdge[2];
    box->MaxEdge[2] = t;
  }
}
//! An enum for the color format of textures used by the Irrlicht Engine.
typedef enum ECOLOR_FORMAT {
  //! 16 bit color format used by the software driver.
  ECF_A1R5G5B5 = 0,
  //! Standard 16 bit color format.
  ECF_R5G6B5,
  //! 24 bit color, no alpha channel, but 8 bit for red, green and blue.
  ECF_R8G8B8,
  //! Default 32 bit color format. 8 bits are used for every component: red, green, blue and alpha.
  ECF_A8R8G8B8,
  /** Floating Point formats. The following formats may only be used for render target textures. */
  //! 16 bit floating point format using 16 bits for the red channel.
  ECF_R16F,
  //! 32 bit floating point format using 16 bits for the red channel and 16 bits for the green channel.
  ECF_G16R16F,
  //! 64 bit floating point format 16 bits are used for the red, green, blue and alpha channels.
  ECF_A16B16G16R16F,
  //! 32 bit floating point format using 32 bits for the red channel.
  ECF_R32F,
  //! 64 bit floating point format using 32 bits for the red channel and 32 bits for the green channel.
  ECF_G32R32F,
  //! 128 bit floating point format. 32 bits are used for the red, green, blue and alpha channels.
  ECF_A32B32G32R32F,
  //! Unknown color format:
  ECF_UNKNOWN
} ECOLOR_FORMAT;
typedef u32 SColor;
#if 0
//! Creates a 16 bit A1R5G5B5 color
#define RGBA16(r, g, b, a) (u16)(((a) & 0x80) << 8 | ((r) & 0xF8) << 7 | ((g) & 0xF8) << 2 | ((b) & 0xF8) >> 3)
#define RGB16(r, g, b, a)  RGBA16(r, g, b, 0xff)
//! Creates a 16bit A1R5G5B5 color, based on 16bit input values
#define RGB16from16(r, g, b) (0x8000 | ((r) & 0x1F) << 10 | ((g) & 0x1F) << 5  | ((b) & 0x1F))
//! Converts a 32bit (X8R8G8B8) color to a 16bit A1R5G5B5 color
#define X8R8G8B8toA1R5G5B5(color) (u16)(0x8000 | ((color) & 0x00F80000) >> 9 | ((color) & 0x0000F800) >> 6 | ((color) & 0x000000F8) >> 3)
//! Converts a 32bit (A8R8G8B8) color to a 16bit A1R5G5B5 color
#define A8R8G8B8toA1R5G5B5(color) (u16)(((color) & 0x80000000) >> 16 |((color) & 0x00F80000) >> 9 |((color) & 0x0000F800) >> 6 |((color) & 0x000000F8) >> 3)
//! Converts a 32bit (A8R8G8B8) color to a 16bit R5G6B5 color
#define A8R8G8B8toR5G6B5(color)   (u16)((color & 0x00F80000) >> 8 |(color & 0x0000FC00) >> 5 |(color & 0x000000F8) >> 3)
//! Convert A8R8G8B8 Color from A1R5G5B5 color
/** build a nicer 32bit Color by extending dest lower bits with source high bits. */
#define A1R5G5B5toA8R8G8B8(color) (((-((s32) (color) & 0x00008000) >> (s32) 31) & 0xFF000000) | \
    (((color) & 0x00007C00) << 9) | (((color) & 0x00007000) << 4) | \
    (((color) & 0x000003E0) << 6) | (((color) & 0x00000380) << 1) | \
    (((color) & 0x0000001F) << 3) | (((color) & 0x0000001C) >> 2)
//! Returns A8R8G8B8 Color from R5G6B5 color
#define R5G6B5toA8R8G8B8(color)  (u16)(0xFF000000 |((color & 0xF800) << 8) |((color & 0x07E0) << 5) |((color & 0x001F) << 3) )
//! Returns A1R5G5B5 Color from R5G6B5 color
#define R5G6B5toA1R5G5B5(color)  (u16)(0x8000 | ((((color) & 0xFFC0) >> 1) | ((color) & 0x1F)))
//! Returns R5G6B5 Color from A1R5G5B5 color
#define A1R5G5B5toR5G6B5(u16 color)   (u16)((((color) & 0x7FE0) << 1) | ((color) & 0x1F))
//! Returns the alpha component from A1R5G5B5 color
/** In Irrlicht, alpha refers to opacity.
\return The alpha value of the color. 0 is transparent, 1 is opaque. */
#define getA16(color) (((color) >> 15) & 0x1)
//! Returns the red component from A1R5G5B5 color.
/** Shift left by 3 to get 8 bit value. */
#define getR16(color) (((color) >> 10) & 0x1F)
//! Returns the green component from A1R5G5B5 color
/** Shift left by 3 to get 8 bit value. */
#define getG16(color) (((color) >> 5) & 0x1F)
//! Returns the blue component from A1R5G5B5 color
/** Shift left by 3 to get 8 bit value. */
#define getB16(color) ((color) & 0x1F)
//! Returns the average from a 16 bit A1R5G5B5 color
#define getAverage16(color) (((getR16(color) << 3) + (getG16(color) << 3) + (getB16(color) << 3)) / 3)
#else
//! Creates a 16 bit A1R5G5B5 color
CV_INLINE u16 RGBA16(u32 r, u32 g, u32 b, u32 a)
{
  return (u16)((a & 0x80) << 8 | (r & 0xF8) << 7 | (g & 0xF8) << 2 | (b & 0xF8) >> 3);
}
//! Creates a 16 bit A1R5G5B5 color
CV_INLINE u16 RGB16(u32 r, u32 g, u32 b)
{
  return RGBA16(r, g, b, 0xFF);
}
//! Creates a 16bit A1R5G5B5 color, based on 16bit input values
CV_INLINE u16 RGB16from16(u16 r, u16 g, u16 b)
{
  return (0x8000 | (r & 0x1F) << 10 | (g & 0x1F) << 5  | (b & 0x1F));
}
//! Converts a 32bit (X8R8G8B8) color to a 16bit A1R5G5B5 color
CV_INLINE u16 X8R8G8B8toA1R5G5B5(u32 color)
{
  return (u16)(0x8000 | (color & 0x00F80000) >> 9 | (color & 0x0000F800) >> 6 | (color & 0x000000F8) >> 3);
}
//! Converts a 32bit (A8R8G8B8) color to a 16bit A1R5G5B5 color
CV_INLINE u16 A8R8G8B8toA1R5G5B5(u32 color)
{
  return (u16)((color & 0x80000000) >> 16 | (color & 0x00F80000) >> 9 | (color & 0x0000F800) >> 6 | (color & 0x000000F8) >> 3);
}
//! Converts a 32bit (A8R8G8B8) color to a 16bit R5G6B5 color
CV_INLINE u16 A8R8G8B8toR5G6B5(u32 color)
{
  return (u16)((color & 0x00F80000) >> 8 | (color & 0x0000FC00) >> 5 | (color & 0x000000F8) >> 3);
}
//! Convert A8R8G8B8 Color from A1R5G5B5 color
/** build a nicer 32bit Color by extending dest lower bits with source high bits. */
CV_INLINE u32 A1R5G5B5toA8R8G8B8(u16 color)
{
  return (((-((s32) color & 0x00008000) >> (s32) 31) & 0xFF000000) |
      ((color & 0x00007C00) << 9) | ((color & 0x00007000) << 4) |
      ((color & 0x000003E0) << 6) | ((color & 0x00000380) << 1) |
      ((color & 0x0000001F) << 3) | ((color & 0x0000001C) >> 2)
         );
}
//! Returns A8R8G8B8 Color from R5G6B5 color
CV_INLINE u32 R5G6B5toA8R8G8B8(u16 color)
{
  return 0xFF000000 | ((color & 0xF800) << 8) | ((color & 0x07E0) << 5) | ((color & 0x001F) << 3);
}
//! Returns A1R5G5B5 Color from R5G6B5 color
CV_INLINE u16 R5G6B5toA1R5G5B5(u16 color)
{
  return 0x8000 | (((color & 0xFFC0) >> 1) | (color & 0x1F));
}
//! Returns R5G6B5 Color from A1R5G5B5 color
CV_INLINE u16 A1R5G5B5toR5G6B5(u16 color)
{
  return (((color & 0x7FE0) << 1) | (color & 0x1F));
}
//! Returns the alpha component from A1R5G5B5 color
/** In Irrlicht, alpha refers to opacity.
\return The alpha value of the color. 0 is transparent, 1 is opaque. */
CV_INLINE u32 getA16(u16 color)
{
  return ((color >> 15) & 0x1);
}
//! Returns the red component from A1R5G5B5 color.
/** Shift left by 3 to get 8 bit value. */
CV_INLINE u32 getR16(u16 color)
{
  return ((color >> 10) & 0x1F);
}
//! Returns the green component from A1R5G5B5 color
/** Shift left by 3 to get 8 bit value. */
CV_INLINE u32 getG16(u16 color)
{
  return ((color >> 5) & 0x1F);
}
//! Returns the blue component from A1R5G5B5 color
/** Shift left by 3 to get 8 bit value. */
CV_INLINE u32 getB16(u16 color)
{
  return (color & 0x1F);
}
//! Returns the average from a 16 bit A1R5G5B5 color
CV_INLINE s32 getAverage16(s16 color)
{
  return ((getR16(color) << 3) + (getG16(color) << 3) + (getB16(color) << 3)) / 3;
}
#endif
#define getA(color) ((color) >> 24)
#define getR(color) (((color) >> 16) & 0xff)
#define getG(color) (((color) >> 8) & 0xff)
#define getB(color) ((color) & 0xff)
//! Get lightness of the color in the range [0,255]
CV_INLINE f32 getLightness(COLOR color)
{
  return 0.5f * (MAX(MAX(getR(color), getG(color)), getB(color)) + MIN(MIN(getR(color), getG(color)), getB(color)));
}
//! Get luminance of the color in the range [0,255].
#define getLuminance(color) (0.3f * getR(color) + 0.59f * getG(color) + 0.11f * getB(color))
//! Get average intensity of the color in the range [0,255].
#define getAverage(color)    (getR(color) + getG(color) + getB(color)) / 3
#define setA(color, a)  (color = (((a) & 0xff) << 24) | (color & 0x00ffffff))
#define setR(color, a)  (color = (((r) & 0xff) << 16) | (color & 0xff00ffff))
#define setG(color, a)  (color = (((g) & 0xff) << 8) | (color & 0xffff00ff))
#define setB(color, a)  (color = ((b) & 0xff) | (color & 0xffffff00))
#define toOpenGLColor(color, dest) (*(dest+0) = (u8)getR(color), *(dest+1) = (u8)getG(color), *(dest+2) = (u8)getB(color), *(dest+3) = (u8)getA(color) )
CV_INLINE SColor color_add(SColor a, SColor b)
{
  return _ARGB(MIN(getA(a) + getA(b), (u32)255u),
      MIN(getR(a) + getR(b), (u32)255u),
      MIN(getG(a) + getG(b), (u32)255u),
      MIN(getB(a) + getB(b), (u32)255u));
}
//! Interpolates the color with a f32 value to another color
/** \param other: Other color
\param d: value between 0.0f and 1.0f
\return Interpolated color. */
SColor getInterpolated(SColor a, SColor b, f32 d0)
{
  const f32 d = BOUND(d0, 0.f, 1.f);
  const f32 inv = 1.0f - d;
  return _ARGB((u32)round32(getA(b) * inv + getA(a) * d),
      (u32)round32(getR(b) * inv + getR(a) * d),
      (u32)round32(getG(b) * inv + getG(a) * d),
      (u32)round32(getB(b) * inv + getB(a) * d));
}
#define floor32(x)   (f32)floor((f32)(x))
//! Returns interpolated color. ( quadratic )
/** \param c1: first color to interpolate with
\param c2: second color to interpolate with
\param d: value between 0.0f and 1.0f. */
SColor getInterpolated_quadratic(SColor a, SColor c1, SColor c2, f32 d0)
{
  // this*(1-d)*(1-d) + 2 * c1 * (1-d) + c2 * d * d;
  const f32 d = BOUND(d0, 0.f, 1.f);
  const f32 inv = 1.f - d;
  const f32 mul0 = inv * inv;
  const f32 mul1 = 2.f * d * inv;
  const f32 mul2 = d * d;
  return _ARGB(
      BOUND((s32)floor32(getA(a) * mul0 + getA(c1) * mul1 + getA(c2) * mul2), (s32)0, (s32)255),
      BOUND((s32)floor32(getR(a) * mul0 + getR(c1) * mul1 + getR(c2) * mul2), (s32)0, (s32)255),
      BOUND((s32)floor32(getG(a) * mul0 + getG(c1) * mul1 + getG(c2) * mul2), (s32)0, (s32)255),
      BOUND((s32)floor32(getB(a) * mul0 + getB(c1) * mul1 + getB(c2) * mul2), (s32)0, (s32)255));
}
//! set the color by expecting data in the given format
/** \param data: must point to valid memory containing color information in the given format
\param format: tells the format in which data is available
*/
SColor data2color(const void* data, ECOLOR_FORMAT format)
{
  SColor color = 0;
  switch (format) {
  case ECF_A1R5G5B5:
      color = A1R5G5B5toA8R8G8B8(*(u16*)data);
    break;
  case ECF_R5G6B5:
      color = R5G6B5toA8R8G8B8(*(u16*)data);
    break;
  case ECF_A8R8G8B8:
      color = *(u32*)data;
    break;
  case ECF_R8G8B8: {
      u8* p = (u8*)data;
      color = _ARGB(255, p[0], p[1], p[2]);
    }
    break;
  default:
      color = 0xffffffff;
    break;
  }
  return color;
}
//! Write the color to data in the defined format
/** \param data: target to write the color. Must contain sufficiently large memory to receive the number of bytes neede for format
\param format: tells the format used to write the color into data
*/
void color2data(COLOR color, void* data, ECOLOR_FORMAT format)
{
  switch (format) {
  case ECF_A1R5G5B5: {
      u16* dest = (u16*)data;
      *dest = A8R8G8B8toA1R5G5B5(color);
    }
    break;
  case ECF_R5G6B5: {
      u16* dest = (u16*)data;
      *dest = A8R8G8B8toR5G6B5(color);
    }
    break;
  case ECF_R8G8B8: {
      u8* dest = (u8*)data;
      dest[0] = (u8)getR(color);
      dest[1] = (u8)getG(color);
      dest[2] = (u8)getB(color);
    }
    break;
  case ECF_A8R8G8B8: {
      u32* dest = (u32*)data;
      *dest = color;
    }
    break;
  default:
      break;
  }
}
//! Class representing a color with four floats.

//typedef struct SColorf {  f32 r, g, b, a;} SColorf;
void SColorf_setSColorf3(float* v, f32 r, f32 g, f32 b)
{
  VSET4(v, r, g, b, 1.f);
}
void setSColori(float* v, SColor c)
{
  const f32 inv = 1.0f / 255.0f;
  VSET4(v, getR(c) * inv, getG(c) * inv, getB(c) * inv, getA(c) * inv);
}
//! Converts this color to a SColor without floats.
SColor SColorftoSColor(const float* v)
{
  return _ARGB((u32)round32(v[3] * 255.0f), (u32)round32(v[0] * 255.0f), (u32)round32(v[1] * 255.0f), (u32)round32(v[2] * 255.0f));
}
void SColorf_getInterpolated(float* v, const float* s, const float* c, f32 d0)
{
  const f32 d = BOUND(d0, 0.f, 1.f);
  const f32 inv = 1.0f - d;
  VSET4(v, c[0] * inv + s[0] * d, c[1] * inv + s[1] * d, c[2] * inv + s[1] * d, c[3] * inv + s[3] * d);
}
//! Returns interpolated color. ( quadratic )
/** \param c1: first color to interpolate with
\param c2: second color to interpolate with
\param d: value between 0.0f and 1.0f. */
static void SColorf_getInterpolated_quadratic(float* v, const float* s, const float* c1, const float* c2, f32 d0)
{
  const f32 d = BOUND(d0, 0.f, 1.f);
  // this*(1-d)*(1-d) + 2 * c1 * (1-d) + c2 * d * d;
  const f32 inv = 1.f - d;
  const f32 mul0 = inv * inv;
  const f32 mul1 = 2.f * d * inv;
  const f32 mul2 = d * d;
  VSET4(v,
      s[0] * mul0 + c1[0] * mul1 + c2[0] * mul2,
      s[1] * mul0 + c1[1] * mul1 + c2[1] * mul2,
      s[2] * mul0 + c1[2] * mul1 + c2[2] * mul2,
      s[3] * mul0 + c1[3] * mul1 + c2[3] * mul2);
}
#define EQLIMITF32    (0.0000001f)
#define ISZERO_F32(a)   (-EQLIMITF32<(a) && (a) <EQLIMITF32)
#define EQ_F32(a, b)    ISZERO_F32((a)-(b))
static void RGBtoHSLfv(const float* rgb, float* hsl)
{
  const f32 maxVal = MAX3(rgb[0], rgb[1], rgb[2]);
  const f32 minVal = (f32)MIN3(rgb[0], rgb[1], rgb[2]);
  hsl[2] = (maxVal + minVal) * 50;
  if (EQ_F32(maxVal, minVal)) {
    hsl[0] = 0.f;
    hsl[1] = 0.f;
    return;
  }
  {
    const f32 delta = maxVal - minVal;
    if (hsl[2] <= 50) {
      hsl[1] = (delta) / (maxVal + minVal);
    }
    else {
      hsl[1] = (delta) / (2 - maxVal - minVal);
    }
    hsl[1] *= 100;
    if (EQ_F32(maxVal, rgb[0])) {
      hsl[0] = (rgb[1] - rgb[2]) / delta;
    }
    else if (EQ_F32(maxVal, rgb[1])) {
      hsl[0] = 2 + ((rgb[2] - rgb[0]) / delta);
    }
    else { // blue is max
      hsl[0] = 4 + ((rgb[0] - rgb[1]) / delta);
    }
    hsl[0] *= 60.0f;
    while (hsl[0] < 0.f) {
      hsl[0] += 360;
    }
  }
}
// algorithm from Foley/Van-Dam
CV_INLINE f32 HSLtoRGB1(f32 rm1, f32 rm2, f32 rh)
{
  if (rh < 0) {
    rh += 1;
  }
  if (rh > 1) {
    rh -= 1;
  }
  if (rh < 1.f / 6.f) {
    rm1 = rm1 + (rm2 - rm1) * rh * 6.f;
  }
  else if (rh < 0.5f) {
    rm1 = rm2;
  }
  else if (rh < 2.f / 3.f) {
    rm1 = rm1 + (rm2 - rm1) * ((2.f / 3.f) - rh) * 6.f;
  }
  return rm1;
}
static void HSLtoRGBfv(const float* hsl, float* rgb)
{
  f32 rm2;
  const f32 l = hsl[2] / 100;
  if (ISZERO_F32(hsl[1])) { // grey
    VSET3(rgb, l, l, l);
    return;
  }
  if (hsl[2] <= 50) {
    rm2 = l + l * (hsl[1] / 100);
  }
  else {
    rm2 = l + (1 - l) * (hsl[1] / 100);
  }
  {
    const f32 rm1 = 2.0f * l - rm2;
    const f32 h = hsl[0] / 360.0f;
    VSET3(rgb, HSLtoRGB1(rm1, rm2, h + 1.f / 3.f),
        HSLtoRGB1(rm1, rm2, h),
        HSLtoRGB1(rm1, rm2, h - 1.f / 3.f)
         );
  }
}
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
  if (0==_imginfo->bpp) {
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
  return _imginfo+format;
}
//! Class representing a 32 bit ARGB color.
/** The color values for alpha, red, green, and blue are
stored in a single u32. So all four values may be between 0 and 255.
Alpha in Irrlicht is opacity, so 0 is fully transparent, 255 is fully opaque (solid).
This class is used by most parts of the Irrlicht Engine
to specify a color. Another way is using the class SColorf, which
stores the color values in 4 floats.
This class must consist of only one u32 and must not use functions.
*/
//! Sinks an element into the heap.
#define HEAPSINK(T, ARRAY, X, N) { \
    int nn = N; \
    while ((X << 1) < nn) { /*there is a left child*/ \
      s32 j = (X << 1); \
      if (j + 1 < nn && array[j] < array[j + 1]) {  \
        j = j + 1;  /* take right child */  \
      } \
      if (array[X] < array[j]) { \
        T t = array[j]; /*swap elements*/  \
        array[j] = array[element]; \
        array[X] = t; \
        X = j; \
      } else { \
        break; \
      } \
    } \
  }
//! Sorts an array with size 'size' using heapsort.
// for heapsink we pretent this is not c++, where
// arrays start with index 0. So we decrease the array pointer,
// the maximum always +2 and the element always +1
#define HEAPSORT(T, ARRAY_, size) { s32 i; \
    T* virtualArray = array_ - 1; \
    s32 virtualSize = size + 2; \
    /* build heap */  \
    for (i = ((size - 1) / 2); i >= 0; --i) { \
      HEAPSINK(T, virtualArray, i + 1, virtualSize - 1); \
    }  \
    /* sort array, leave out the last element (0) */  \
    for (i = size - 1; i > 0; --i) { \
      T t = array_[0];  \
      array_[0] = array_[i];  \
      array_[i] = t;  \
      HEAPSINK(T, virtualArray, 1, i + 1);  \
    } \
  }
//! Enumeration for intersection relations of 3d objects
typedef enum EIntersectionRelation3D {
  ISREL3D_FRONT = 0,
  ISREL3D_BACK,
  ISREL3D_PLANAR,
  ISREL3D_SPANNING,
  ISREL3D_CLIPPED
} EIntersectionRelation3D;
typedef enum E_INDEX_TYPE {
  EIT_16BIT = 0,
  EIT_32BIT
} E_INDEX_TYPE;
//! Abstracted and easy to use fixed function/programmable pipeline material modes.
typedef enum E_MATERIAL_TYPE {
  EMT_SOLID = 0,
EMT_SOLID_2_LAYER,
EMT_LIGHTMAP,
EMT_LIGHTMAP_ADD,
EMT_LIGHTMAP_M2,
EMT_LIGHTMAP_M4,
EMT_LIGHTMAP_LIGHTING,
EMT_LIGHTMAP_LIGHTING_M2,
EMT_LIGHTMAP_LIGHTING_M4,
EMT_DETAIL_MAP,
EMT_SPHERE_MAP,
EMT_REFLECTION_2_LAYER,
EMT_TRANSPARENT_ADD_COLOR,
EMT_TRANSPARENT_ALPHA_CHANNEL,
EMT_TRANSPARENT_ALPHA_CHANNEL_REF,
EMT_TRANSPARENT_VERTEX_ALPHA,
EMT_TRANSPARENT_REFLECTION_2_LAYER,
EMT_NORMAL_MAP_SOLID,
EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR,
EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA,
EMT_PARALLAX_MAP_SOLID,
EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR,
EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA,
EMT_ONETEXTURE_BLEND,
EMT_FORCE_32BIT = 0x7fffffff
} E_MATERIAL_TYPE;
//! Array holding the built in material type names
static const char* const sBuiltInMaterialTypeNames[] = {
  "solid",
  "solid_2layer",
  "lightmap",
  "lightmap_add",
  "lightmap_m2",
  "lightmap_m4",
  "lightmap_light",
  "lightmap_light_m2",
  "lightmap_light_m4",
  "detail_map",
  "sphere_map",
  "reflection_2layer",
  "trans_add",
  "trans_alphach",
  "trans_alphach_ref",
  "trans_vertex_alpha",
  "trans_reflection_2layer",
  "normalmap_solid",
  "normalmap_trans_add",
  "normalmap_trans_vertexalpha",
  "parallaxmap_solid",
  "parallaxmap_trans_add",
  "parallaxmap_trans_vertexalpha",
  "onetexture_blend",
  0
};
//! Enumeration for all vertex types there are.
typedef enum E_VERTEX_TYPE {
  EVT_STANDARD = 0,
  EVT_2TCOORDS,
  EVT_TANGENTS
} E_VERTEX_TYPE;
//! Array holding the built in vertex type names
const char* const sBuiltInVertexTypeNames[] = {
  "standard",
  "2tcoords",
  "tangents",
  0
};
//! Material flags
typedef enum E_MATERIAL_FLAG {
  //! Draw as wireframe or filled triangles? Default: false
  EMF_WIREFRAME = 0x1,
  //! Draw as point cloud or filled triangles? Default: false
  EMF_POINTCLOUD = 0x2,
  //! Flat or Gouraud shading? Default: true
  EMF_GOURAUD_SHADING = 0x4,
  //! Will this material be lighted? Default: true
  EMF_LIGHTING = 0x8,
  //! Is the ZBuffer enabled? Default: true
  EMF_ZBUFFER = 0x10,
  //! May be written to the zbuffer or is it readonly. Default: true
  /** This flag is ignored, if the material type is a transparent type. */
  EMF_ZWRITE_ENABLE = 0x20,
  //! Is backface culling enabled? Default: true
  EMF_BACK_FACE_CULLING = 0x40,
  //! Is frontface culling enabled? Default: false
  /** Overrides EMF_BACK_FACE_CULLING if both are enabled. */
  EMF_FRONT_FACE_CULLING = 0x80,
  //! Is bilinear filtering enabled? Default: true
  EMF_BILINEAR_FILTER = 0x100,
  //! Is trilinear filtering enabled? Default: false
  /** If the trilinear filter flag is enabled,
  the bilinear filtering flag is ignored. */
  EMF_TRILINEAR_FILTER = 0x200,
  //! Is anisotropic filtering? Default: false
  EMF_ANISOTROPIC_FILTER = 0x400,
  //! Is fog enabled? Default: false
  EMF_FOG_ENABLE = 0x800,
  //! Normalizes normals. Default: false
  EMF_NORMALIZE_NORMALS = 0x1000,
  //! Access to all layers texture wrap settings. Overwrites separate layer settings.
  EMF_TEXTURE_WRAP = 0x2000,
  //! AntiAliasing mode
  EMF_ANTI_ALIASING = 0x4000,
  //! ColorMask bits, for enabling the color planes
  EMF_COLOR_MASK = 0x8000,
  //! ColorMaterial enum for vertex color interpretation
  EMF_COLOR_MATERIAL = 0x10000,
  //! Flag for enabling/disabling mipmap usage
  EMF_USE_MIP_MAPS = 0x20000,
  //! Flag for blend operation
  EMF_BLEND_OPERATION = 0x40000,
  //! Flag for polygon offset
  EMF_POLYGON_OFFSET = 0x80000
} E_MATERIAL_FLAG;
//! Texture coord BOUND mode outside [0.0, 1.0]
enum E_TEXTURE_CLAMP {
  //! Texture repeats
  ETC_REPEAT = 0,
  //! Texture is clamped to the last pixel
  ETC_CLAMP,
  //! Texture is clamped to the edge pixel
  ETC_CLAMP_TO_EDGE,
  //! Texture is clamped to the border pixel (if exists)
  ETC_CLAMP_TO_BORDER,
  //! Texture is alternatingly mirrored (0..1..0..1..0..)
  ETC_MIRROR,
  //! Texture is mirrored once and then clamped (0..1..0)
  ETC_MIRROR_CLAMP,
  //! Texture is mirrored once and then clamped to edge
  ETC_MIRROR_CLAMP_TO_EDGE,
  //! Texture is mirrored once and then clamped to border
  ETC_MIRROR_CLAMP_TO_BORDER
};
static const char* const aTextureClampNames[] = {
  "texture_clamp_repeat",
  "texture_clamp_clamp",
  "texture_clamp_clamp_to_edge",
  "texture_clamp_clamp_to_border",
  "texture_clamp_mirror",
  "texture_clamp_mirror_clamp",
  "texture_clamp_mirror_clamp_to_edge",
  "texture_clamp_mirror_clamp_to_border", 0
};
#define LOCALE_DECIMAL_POINTS "."
static char* PathToName(char* p)
{
  cstr_replacechr(p, -1, '\\', '/');
  cstr_tolower(p, -1);
  return p;
}
//! Selection of characters which count as decimal point in fast_atof
// TODO: This should probably also be used in string, but the float-to-string code
//    used there has to be rewritten first.
// we write [17] here instead of [] to work around a swig bug
const float fast_atof_table[17] = {
  0.f,
  0.1f,
  0.01f,
  0.001f,
  0.0001f,
  0.00001f,
  0.000001f,
  0.0000001f,
  0.00000001f,
  0.000000001f,
  0.0000000001f,
  0.00000000001f,
  0.000000000001f,
  0.0000000000001f,
  0.00000000000001f,
  0.000000000000001f,
  0.0000000000000001f
};
//! Convert a simple string of base 10 digits into an unsigned 32 bit integer.
CV_INLINE u32 strtoul10(const char* in, const char** out)
{
  bool overflow;
  u32 unsignedValue;
  if (!in) {
    if (out) {
      *out = in;
    }
    return 0;
  }
  overflow = false;
  unsignedValue = 0;
  while ((*in >= '0') && (*in <= '9')) {
    const u32 tmp = (unsignedValue * 10) + (*in - '0');
    if (tmp < unsignedValue) {
      unsignedValue = (u32)0xffffffff;
      overflow = true;
    }
    if (!overflow) {
      unsignedValue = tmp;
    }
    ++in;
  }
  if (out) {
    *out = in;
  }
  return unsignedValue;
}
//! Convert a simple string of base 10 digits into a signed 32 bit integer.
/** \param[in] in: The string of digits to convert. Only a leading - or +
    followed by digits 0 to 9 will be considered. Parsing stops at the first
    non-digit.
    \param[out] out: (optional) If provided, it will be set to point at the
    first character not used in the calculation.
    \return The signed integer value of the digits. If the string specifies
    too many digits to encode in an s32 then +INT_MAX or -INT_MAX will be
    returned.
*/
CV_INLINE s32 strtol10(const char* in, const char** out)
{
  bool negative;
  u32 unsignedValue;
  if (!in) {
    if (out) {
      *out = in;
    }
    return 0;
  }
  negative = ('-' == *in);
  if (negative || ('+' == *in)) {
    ++in;
  }
  unsignedValue = strtoul10(in, out);
  if (unsignedValue > (u32)INT_MAX) {
    if (negative) {
      return (s32)INT_MIN;
    }
    else {
      return (s32)INT_MAX;
    }
  }
  else {
    if (negative) {
      return -((s32)unsignedValue);
    }
    else {
      return (s32)unsignedValue;
    }
  }
}
//! Convert a hex-encoded character to an unsigned integer.
/** \param[in] in The digit to convert. Only digits 0 to 9 and chars A-F,a-f
    will be considered.
    \return The unsigned integer value of the digit. 0xffffffff if the input is
    not hex
*/
CV_INLINE u32 ctoul16(char in)
{
  if (in >= '0' && in <= '9') {
    return in - '0';
  }
  else if (in >= 'a' && in <= 'f') {
    return 10u + in - 'a';
  }
  else if (in >= 'A' && in <= 'F') {
    return 10u + in - 'A';
  }
  else {
    return 0xffffffff;
  }
}
//! Convert a simple string of base 16 digits into an unsigned 32 bit integer.
/** \param[in] in: The string of digits to convert. No leading chars are
    allowed, only digits 0 to 9 and chars A-F,a-f are allowed. Parsing stops
    at the first illegal char.
    \param[out] out: (optional) If provided, it will be set to point at the
    first character not used in the calculation.
    \return The unsigned integer value of the digits. If the string specifies
    too many digits to encode in an u32 then INT_MAX will be returned.
*/
CV_INLINE u32 strtoul16(const char* in, const char** out)
{
  bool overflow;
  u32 unsignedValue;
  if (!in) {
    if (out) {
      *out = in;
    }
    return 0;
  }
  overflow = false;
  unsignedValue = 0;
  while (true) {
    u32 tmp = 0;
    if ((*in >= '0') && (*in <= '9')) {
      tmp = (unsignedValue << 4u) + (*in - '0');
    }
    else if ((*in >= 'A') && (*in <= 'F')) {
      tmp = (unsignedValue << 4u) + (*in - 'A') + 10;
    }
    else if ((*in >= 'a') && (*in <= 'f')) {
      tmp = (unsignedValue << 4u) + (*in - 'a') + 10;
    }
    else {
      break;
    }
    if (tmp < unsignedValue) {
      unsignedValue = (u32)INT_MAX;
      overflow = true;
    }
    if (!overflow) {
      unsignedValue = tmp;
    }
    ++in;
  }
  if (out) {
    *out = in;
  }
  return unsignedValue;
}
//! Convert a simple string of base 8 digits into an unsigned 32 bit integer.
/** \param[in] in The string of digits to convert. No leading chars are
    allowed, only digits 0 to 7 are allowed. Parsing stops at the first illegal
    char.
    \param[out] out (optional) If provided, it will be set to point at the
    first character not used in the calculation.
    \return The unsigned integer value of the digits. If the string specifies
    too many digits to encode in an u32 then INT_MAX will be returned.
*/
CV_INLINE u32 strtoul8(const char* in, const char** out)
{
  bool overflow;
  u32 unsignedValue;
  if (!in) {
    if (out) {
      *out = in;
    }
    return 0;
  }
  overflow = false;
  unsignedValue = 0;
  while (true) {
    u32 tmp = 0;
    if ((*in >= '0') && (*in <= '7')) {
      tmp = (unsignedValue << 3u) + (*in - '0');
    }
    else {
      break;
    }
    if (tmp < unsignedValue) {
      unsignedValue = (u32)INT_MAX;
      overflow = true;
    }
    if (!overflow) {
      unsignedValue = tmp;
    }
    ++in;
  }
  if (out) {
    *out = in;
  }
  return unsignedValue;
}
//! Convert a C-style prefixed string (hex, oct, integer) into an unsigned 32 bit integer.
/** \param[in] in The string of digits to convert. If string starts with 0x the
    hex parser is used, if only leading 0 is used, oct parser is used. In all
    other cases, the usual unsigned parser is used.
    \param[out] out (optional) If provided, it will be set to point at the
    first character not used in the calculation.
    \return The unsigned integer value of the digits. If the string specifies
    too many digits to encode in an u32 then INT_MAX will be returned.
*/
CV_INLINE u32 strtoul_prefix(const char* in, const char** out)
{
  if (!in) {
    if (out) {
      *out = in;
    }
    return 0;
  }
  if ('0' == in[0]) {
    return ('x' == in[1] ? strtoul16(in + 2, out) : strtoul8(in + 1, out));
  }
  return strtoul10(in, out);
}
//! Converts a sequence of digits into a whole positive floating point value.
/** Only digits 0 to 9 are parsed.  Parsing stops at any other character,
    including sign characters or a decimal point.
    \param in: the sequence of digits to convert.
    \param out: (optional) will be set to point at the first non-converted
    character.
    \return The whole positive floating point representation of the digit
    sequence.
*/
CV_INLINE f32 strtof10(const char* in, const char** out)
{
  enum { MAX_SAFE_U32_VALUE = UINT_MAX / 10 - 10 };
  u32 intValue;
  f32 floatValue;
  if (!in) {
    if (out) {
      *out = in;
    }
    return 0.f;
  }
  intValue = 0;
  // Use integer arithmetic for as long as possible, for speed
  // and precision.
  while ((*in >= '0') && (*in <= '9')) {
    // If it looks like we're going to overflow, bail out
    // now and start using floating point.
    if (intValue >= MAX_SAFE_U32_VALUE) {
      break;
    }
    intValue = (intValue * 10) + (*in - '0');
    ++in;
  }
  floatValue = (f32)intValue;
  // If there are any digits left to parse, then we need to use
  // floating point arithmetic from here.
  while ((*in >= '0') && (*in <= '9')) {
    floatValue = (floatValue * 10.f) + (f32)(*in - '0');
    ++in;
    if (floatValue > FLT_MAX) { // Just give up.
      break;
    }
  }
  if (out) {
    *out = in;
  }
  return floatValue;
}
//! Provides a fast function for converting a string into a float.
/** This is not guaranteed to be as accurate as atof(), but is
    approximately 6 to 8 times as fast.
    \param[in] in The string to convert.
    \param[out] result The resultant float will be written here.
    \return Pointer to the first character in the string that wasn't used
    to create the float value.
*/
CV_INLINE const char* fast_atof_move(const char* in, f32* result)
{
  bool negative;
  f32 value;
  // Please run the regression test when making any modifications to this function.
  *result = 0.f;
  if (!in) {
    return 0;
  }
  negative = ('-' == *in);
  if (negative || ('+' == *in)) {
    ++in;
  }
  value = strtof10(in, &in);
  if (cstr_findchr(LOCALE_DECIMAL_POINTS, -1, *in, 0) >= 0) {
    const char* afterDecimal = ++in;
    const f32 decimal = strtof10(in, &afterDecimal);
    value += decimal * fast_atof_table[afterDecimal - in];
    in = afterDecimal;
  }
  if ('e' == *in || 'E' == *in) {
    ++in;
    // Assume that the exponent is a whole number.
    // strtol10() will deal with both + and - signs,
    // but calculate as f32 to prevent overflow at FLT_MAX
    value *= powf(10.f, (f32)strtol10(in, &in));
  }
  *result = negative ? -value : value;
  return in;
}
//! Convert a string to a floating point number
/** \param floatAsString The string to convert.
    \param out Optional pointer to the first character in the string that
    wasn't used to create the float value.
    \result Float value parsed from the input string
*/
CV_INLINE float fast_atof(const char* floatAsString, const char** out)
{
  float ret;
  if (out) {
    *out = fast_atof_move(floatAsString, &ret);
  }
  else {
    fast_atof_move(floatAsString, &ret);
  }
  return ret;
}
/*! \file coreutil.h
  \brief File containing useful basic utility functions
*/
// ----------- some basic quite often used string functions -----------------
//! search if a filename has a proper extension
CV_INLINE s32 isFileExtension(const char* filename,    const char* ext1,    const char* ext2,    const char* ext3)
{
  if (cstr_endwith(filename, -1, ext1, -1, 1)) {
    return 1;
  }
  if (cstr_endwith(filename, -1, ext2, -1, 1)) {
    return 2;
  }
  if (cstr_endwith(filename, -1, ext3, -1, 1)) {
    return 3;
  }
  return 0;
}
//! search if a filename has a proper extension
CV_INLINE bool hasFileExtension(const char* filename, const char* ext1, const char* ext2, const char* ext3)
{
  return isFileExtension(filename, ext1, ext2, ext3) > 0;
}
//! cut the filename extension from a source file path and store it in a dest file path
CV_INLINE char* cutFilenameExtension(char* ext, int len, const char* source)
{
  char* endPos = cstr_rchr(source, -1, '.');
  if (ext && endPos) {
    cstr_cpy(ext, len, endPos, -1);
    endPos = ext;
  }
  return endPos;
}
//! get the filename extension from a file path
CV_INLINE char* getFileNameExtension(char* ext, int len, const char* source)
{
  char* endPos = cstr_rchr(source, -1, '.');
  if (ext && endPos) {
    cstr_cpy(ext, len, endPos, -1);
    endPos = ext;
  }
  return endPos;
}
//! delete path from filename
CV_INLINE char* deletePathFromFilename(char* filename)
{
  // delete path from filename
  const char* s = filename;
  const char* e = s + cstr_len(filename);
  const char* p = e;
  // search for path separator or beginning
  while (*p != '/' && *p != '\\' && p != s) {
    p--;
  }
  if (p != s) {
    ++p;
    filename = (char*)p;
  }
  return filename;
}
//! trim paths
CV_INLINE char* deletePathFromPath(char* filename, s32 pathCount)
{
  // delete path from filename
  s32 i = cstr_len(filename);
  // search for path separator or beginning
  while (i >= 0) {
    if (filename[i] == '/' || filename[i] == '\\') {
      if (--pathCount <= 0) {
        break;
      }
    }
    --i;
  }
  if (i > 0) {
    filename[ i + 1 ] = 0;
  }
  else {
    filename[0] = 0;
  }
  return filename;
}
//! looks if file is in the same directory of path. returns offset of directory.
//! 0 means in same directory. 1 means file is direct child of path
CV_INLINE s32 isInSameDirectory(const char* path, const char* file)
{
  s32 subA = 0;
  s32 subB = 0;
  s32 pos;
  int path_size = cstr_len(path);
  if (path_size && cstr_icmp(path, path_size, file, -1, 1)) {
    return -1;
  }
  pos = 0;
  while ((pos = cstr_findchr(path, path_size, '/', pos)) >= 0) {
    subA += 1;
    pos += 1;
  }
  pos = 0;
  while ((pos = cstr_findchr(file, -1, '/', pos)) >= 0) {
    subB += 1;
    pos += 1;
  }
  return subB - subA;
}
// splits a path into components
static void splitFilename(char* name, char** path, char** filename, char** extension, bool make_lower)
{
  s32 i = cstr_len(name);
  s32 extpos = i;
  if (make_lower) {
    cstr_tolower(name, -1);
  }
  // search for path separator or beginning
  while (i >= 0) {
    if (name[i] == '.') {
      extpos = i;
      name[i] = 0;
      if (extension) {
        *extension = name + (extpos + 1);
      }
    }
    else if (name[i] == '/' || name[i] == '\\') {
      name[i] = 0;
      if (filename) {
        *filename = name + (i + 1);
      }
      if (path) {
        *path = name;
        cstr_replacechr(name, -1, '\\', '/');
      }
      return;
    }
    i -= 1;
  }
  if (filename) {
    *filename = name;
  }
}
//! some standard function ( to remove dependencies )
#undef isdigit
#undef isspace
#undef isupper
CV_INLINE s32 isdigit(s32 c)
{
  return c >= '0' && c <= '9';
}
CV_INLINE s32 isspace(s32 c)
{
  return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}
CV_INLINE s32 isupper(s32 c)
{
  return c >= 'A' && c <= 'Z';
}
#define PATH_IGNORE_CASE  1
//! flatten a path and file name for example: "/you/me/../." becomes "/you"
int flattenFilename(const char* directory0, int l_directory, const char* root, int l_root, char* out, int l_out)
{
  char256 directory = {0};
  char256 dir = {0};
  s32 lastpos = 0;
  s32 pos = 0;
  bool lastWasRealDir = false;
  uchar256 umap_slash = {0};

  l_directory = l_directory<0 ? strlen(directory) : l_directory;
  l_root = l_root<0 ? strlen(root) : l_root;
  cstr_replacechr_cpy(directory, 256, directory0, l_directory, '\\', '/', 1);
  if (l_directory<=0) {
    return 0;
  }

  //directory.replace('\\', '/');
  if (umap_slash[directory[l_directory-1]]) {
    //directory.append('/');
  }
  while ((pos = cstr_findchrs(directory, l_directory, umap_slash, lastpos)) >= 0) {
    char* subdir = directory + lastpos;
    int l_subdir = pos - lastpos + 1;
    if (0 == cstr_icmp(subdir, l_subdir, "../", 3, 0)) {
      if (lastWasRealDir) {
        deletePathFromPath(dir, 2);
        lastWasRealDir = (strlen(dir) != 0);
      }
      else {
        cstr_cat(dir, 256, subdir, l_subdir);
        lastWasRealDir = false;
      }
    }
    else if (0 == cstr_icmp(subdir, l_subdir, "/", 1, 0)) {
      cstr_cpy(dir, 256, root, l_root);
    }
    else if (0 != cstr_icmp(subdir, l_subdir, "./", 2, 0)) {
      cstr_cat(dir, 256, subdir, l_subdir);
      lastWasRealDir = true;
    }
    lastpos = pos + 1;
  }
  cstr_cpy(out, l_out, dir, -1);
  return 0;
}
//! Enumeration flags telling the video driver in which format textures should be created.
typedef enum E_TEXTURE_CREATION_FLAG {
ETCF_ALWAYS_16_BIT = 0x00000001,
ETCF_ALWAYS_32_BIT = 0x00000002,
ETCF_OPTIMIZED_FOR_QUALITY = 0x00000004,
ETCF_OPTIMIZED_FOR_SPEED = 0x00000008,
ETCF_CREATE_MIP_MAPS = 0x00000010,
ETCF_NO_ALPHA_CHANNEL = 0x00000020,
ETCF_ALLOW_NON_POWER_2 = 0x00000040,
ETCF_FORCE_32_BIT_DO_NOT_USE = 0x7fffffff

} E_TEXTURE_CREATION_FLAG;
//! Enum for the mode for texture locking. Read-Only, write-only or read/write.
typedef enum E_TEXTURE_LOCK_MODE {
  //! The default mode. Texture can be read and written to.
  ETLM_READ_WRITE = 0,
  //! Read only. The texture is downloaded, but not uploaded again.
  /** Often used to read back shader generated textures. */
  ETLM_READ_ONLY,
  //! Write only. The texture is not downloaded and might be uninitialised.
  /** The updated texture is uploaded to the GPU.
  Used for initialising the shader from the CPU. */
  ETLM_WRITE_ONLY
} E_TEXTURE_LOCK_MODE;
//! Flag for EMT_ONETEXTURE_BLEND, ( BlendFactor ) BlendFunc = source * sourceFactor + dest * destFactor
typedef enum E_BLEND_FACTOR {
  EBF_ZERO  = 0,      //!< src & dest (0, 0, 0, 0)
  EBF_ONE,          //!< src & dest (1, 1, 1, 1)
  EBF_DST_COLOR,        //!< src  (destR, destG, destB, destA)
  EBF_ONE_MINUS_DST_COLOR,  //!< src  (1-destR, 1-destG, 1-destB, 1-destA)
  EBF_SRC_COLOR,        //!< dest (srcR, srcG, srcB, srcA)
  EBF_ONE_MINUS_SRC_COLOR,  //!< dest (1-srcR, 1-srcG, 1-srcB, 1-srcA)
  EBF_SRC_ALPHA,        //!< src & dest (srcA, srcA, srcA, srcA)
  EBF_ONE_MINUS_SRC_ALPHA,  //!< src & dest (1-srcA, 1-srcA, 1-srcA, 1-srcA)
  EBF_DST_ALPHA,        //!< src & dest (destA, destA, destA, destA)
  EBF_ONE_MINUS_DST_ALPHA,  //!< src & dest (1-destA, 1-destA, 1-destA, 1-destA)
  EBF_SRC_ALPHA_SATURATE    //!< src  (min(srcA, 1-destA), idem, ...)
} E_BLEND_FACTOR;
//! Values defining the blend operation used when blend is enabled
typedef enum E_BLEND_OPERATION {
  EBO_NONE = 0, //!< No blending happens
  EBO_ADD,    //!< Default blending adds the color values
  EBO_SUBTRACT, //!< This mode subtracts the color values
  EBO_REVSUBTRACT,//!< This modes subtracts destination from source
  EBO_MIN,    //!< Choose minimum value of each color channel
  EBO_MAX,    //!< Choose maximum value of each color channel
  EBO_MIN_FACTOR, //!< Choose minimum value of each color channel after applying blend factors, not widely supported
  EBO_MAX_FACTOR, //!< Choose maximum value of each color channel after applying blend factors, not widely supported
  EBO_MIN_ALPHA,  //!< Choose minimum value of each color channel based on alpha value, not widely supported
  EBO_MAX_ALPHA //!< Choose maximum value of each color channel based on alpha value, not widely supported
} E_BLEND_OPERATION;
//! MaterialTypeParam: e.g. DirectX: D3DTOP_MODULATE, D3DTOP_MODULATE2X, D3DTOP_MODULATE4X
typedef enum E_MODULATE_FUNC {
  EMFN_MODULATE_1X  = 1,
  EMFN_MODULATE_2X  = 2,
  EMFN_MODULATE_4X  = 4
} E_MODULATE_FUNC;
//! Comparison function, e.g. for depth buffer test
typedef enum E_COMPARISON_FUNC {
  //! Test never succeeds, this equals disable
  ECFN_NEVER = 0,
  //! <= test, default for e.g. depth test
  ECFN_LESSEQUAL = 1,
  //! Exact equality
  ECFN_EQUAL = 2,
  //! exclusive less comparison, i.e. <
  ECFN_LESS,
  //! Succeeds almost always, except for exact equality
  ECFN_NOTEQUAL,
  //! >= test
  ECFN_GREATEREQUAL,
  //! inverse of <=
  ECFN_GREATER,
  //! test succeeds always
  ECFN_ALWAYS
} E_COMPARISON_FUNC;
//! Enum values for enabling/disabling color planes for rendering
typedef enum E_COLOR_PLANE {
  //! No color enabled
  ECP_NONE = 0,
  //! Alpha enabled
  ECP_ALPHA = 1,
  //! Red enabled
  ECP_RED = 2,
  //! Green enabled
  ECP_GREEN = 4,
  //! Blue enabled
  ECP_BLUE = 8,
  //! All colors, no alpha
  ECP_RGB = 14,
  //! All planes enabled
  ECP_ALL = 15
} E_COLOR_PLANE;
//! Source of the alpha value to take
/** This is currently only supported in EMT_ONETEXTURE_BLEND. You can use an
or'ed combination of values. Alpha values are modulated (multiplicated). */
typedef enum E_ALPHA_SOURCE {
  //! Use no alpha, somewhat redundant with other settings
  EAS_NONE = 0,
  //! Use vertex color alpha
  EAS_VERTEX_COLOR,
  //! Use texture alpha channel
  EAS_TEXTURE
} E_ALPHA_SOURCE;
//! EMT_ONETEXTURE_BLEND: pack srcFact, dstFact, Modulate and alpha source to MaterialTypeParam
/** alpha source can be an OR'ed combination of E_ALPHA_SOURCE values. */
CV_INLINE f32 pack_textureBlendFunc(const E_BLEND_FACTOR srcFact, const E_BLEND_FACTOR dstFact,
    const E_MODULATE_FUNC modulate DEFAULT(EMFN_MODULATE_1X), const u32 alphaSource DEFAULT(EAS_TEXTURE))
{
  const u32 tmp = (alphaSource << 12) | (modulate << 8) | (srcFact << 4) | dstFact;
  return FR_u32(tmp);
}
//! EMT_ONETEXTURE_BLEND: unpack srcFact & dstFact and Modulo to MaterialTypeParam
/** The fields don't use the full byte range, so we could pack even more... */
CV_INLINE void unpack_textureBlendFunc(E_BLEND_FACTOR* srcFact, E_BLEND_FACTOR* dstFact,
    E_MODULATE_FUNC* modulo, u32* alphaSource, const f32 param)
{
  const u32 state = IR(param);
  *alphaSource = (state & 0x0000F000) >> 12;
  *modulo  = (E_MODULATE_FUNC)((state & 0x00000F00) >> 8);
  *srcFact = (E_BLEND_FACTOR)((state & 0x000000F0) >> 4);
  *dstFact = (E_BLEND_FACTOR)((state & 0x0000000F));
}
//! EMT_ONETEXTURE_BLEND: has BlendFactor Alphablending
CV_INLINE bool textureBlendFunc_hasAlpha(const E_BLEND_FACTOR factor)
{
  switch (factor) {
  case EBF_SRC_ALPHA:
    case EBF_ONE_MINUS_SRC_ALPHA:
      case EBF_DST_ALPHA:
        case EBF_ONE_MINUS_DST_ALPHA:
          case EBF_SRC_ALPHA_SATURATE:
              return true;
  default:
      return false;
  }
}
//! These flags are used to specify the anti-aliasing and smoothing modes
/** Techniques supported are multisampling, geometry smoothing, and alpha
to coverage.
Some drivers don't support a per-material setting of the anti-aliasing
modes. In those cases, FSAA/multisampling is defined by the device mode
chosen upon creation via SIrrCreationParameters.
*/
typedef enum E_ANTI_ALIASING_MODE {
  //! Use to turn off anti-aliasing for this material
  EAAM_OFF = 0,
  //! Default anti-aliasing mode
  EAAM_SIMPLE = 1,
  //! High-quality anti-aliasing, not always supported, automatically enables SIMPLE mode
  EAAM_QUALITY = 3,
  //! Line smoothing
  EAAM_LINE_SMOOTH = 4,
  //! point smoothing, often in software and slow, only with OpenGL
  EAAM_POINT_SMOOTH = 8,
  //! All typical anti-alias and smooth modes
  EAAM_FULL_BASIC = 15,
  //! Enhanced anti-aliasing for transparent materials
  /** Usually used with EMT_TRANSPARENT_ALPHA_REF and multisampling. */
  EAAM_ALPHA_TO_COVERAGE = 16
} E_ANTI_ALIASING_MODE;
//! These flags allow to define the interpretation of vertex color when lighting is enabled
/** Without lighting being enabled the vertex color is the only value defining the fragment color.
Once lighting is enabled, the four values for diffuse, ambient, emissive, and specular take over.
With these flags it is possible to define which lighting factor shall be defined by the vertex color
instead of the lighting factor which is the same for all faces of that material.
The default is to use vertex color for the diffuse value, another pretty common value is to use
vertex color for both diffuse and ambient factor. */
typedef enum E_COLOR_MATERIAL {
  //! Don't use vertex color for lighting
  ECM_NONE = 0,
  //! Use vertex color for diffuse light, this is default
  ECM_DIFFUSE,
  //! Use vertex color for ambient light
  ECM_AMBIENT,
  //! Use vertex color for emissive light
  ECM_EMISSIVE,
  //! Use vertex color for specular light
  ECM_SPECULAR,
  //! Use vertex color for both diffuse and ambient light
  ECM_DIFFUSE_AND_AMBIENT
} E_COLOR_MATERIAL;
//! Flags for the definition of the polygon offset feature
/** These flags define whether the offset should be into the screen, or towards the eye. */
typedef enum E_POLYGON_OFFSET {
  //! Push pixel towards the far plane, away from the eye
  /** This is typically used for rendering inner areas. */
  EPO_BACK = 0,
  //! Pull pixels towards the camera.
  /** This is typically used for polygons which should appear on top
  of other elements, such as decals. */
  EPO_FRONT = 1
} E_POLYGON_OFFSET;
//! Names for polygon offset direction
const c8* const PolygonOffsetDirectionNames[] = {
  "Back",
  "Front",
  0
};
//! Maximum number of texture an SMaterial can have.
#define  MATERIAL_MAX_TEXTURES  _IRR_MATERIAL_MAX_TEXTURES_
typedef enum E_HARDWARE_MAPPING {
  EHM_NEVER = 0, //! Don't store on the hardware
  EHM_STATIC,  //! Rarely changed, usually stored completely on the hardware
  EHM_DYNAMIC,  //! Sometimes changed, driver optimized placement
  EHM_STREAM  //! Always changed, cache optimizing on the GPU
} E_HARDWARE_MAPPING;
typedef enum E_BUFFER_TYPE {
  EBT_NONE = 0,  //! Does not change anything
  EBT_VERTEX,  //! Change the vertex mapping
  EBT_INDEX,  //! Change the index mapping
  EBT_VERTEX_AND_INDEX  //! Change both vertex and index mapping to the same value
} E_BUFFER_TYPE;
//! Enumeration for all primitive types there are.
typedef enum E_PRIMITIVE_TYPE {
  EPT_POINTS = 0,  //! All vertices are non-connected points.
  EPT_LINE_STRIP,  //! All vertices form a single connected line.
  EPT_LINE_LOOP,  //! Just as LINE_STRIP, but the last and the first vertex is also connected.
  EPT_LINES,  //! Every two vertices are connected creating n/2 lines.
  EPT_TRIANGLE_STRIP,  //! After the first two vertices each vertex defines a new triangle.
  EPT_TRIANGLE_FAN,  //! After the first two vertices each vertex defines a new triangle.
  EPT_TRIANGLES,  //! Explicitly set all vertices for each triangle.
  EPT_QUAD_STRIP,  //! After the first two vertices each further tw vetices create a quad with the preceding two.
  EPT_QUADS,  //! Every four vertices create a quad.
  EPT_POLYGON,  //! Just as LINE_LOOP, but filled.
  EPT_POINT_SPRITES  //! The single vertices are expanded to quad billboards on the GPU.
} E_PRIMITIVE_TYPE;
//! standard vertex used by the Irrlicht engine.
typedef struct S3DVertex {
  float3 Pos; //! Position
  float3 Normal; //! Normal vector
  SColor Color; //! Color
  float2 TCoords2;
  float2 TCoords; //! Texture coordinates
  float3 Tangent; //! Tangent vector along the x-axis of the texture
  float3 Binormal; //! Binormal vector (tangent x normal)
} S3DVertex;
#define getVertexPitchFromType(vertexType)   sizeof(S3DVertex)
typedef struct CVertexBuffer {
  S3DVertex* Vertices;
  int m_size;
  E_VERTEX_TYPE type; // E_VERTEX_TYPE
  E_HARDWARE_MAPPING MappingHint;
  u32 ChangedID;
} CVertexBuffer;
static bool hasAlpha(ECOLOR_FORMAT ColorFormat)
{
  return ColorFormat == ECF_A8R8G8B8 || ColorFormat == ECF_A1R5G5B5;
}
static E_TEXTURE_CREATION_FLAG getTextureFormatFromFlags(u32 flags)
{
  if (flags & ETCF_OPTIMIZED_FOR_SPEED) {
    return ETCF_OPTIMIZED_FOR_SPEED;
  }
  if (flags & ETCF_ALWAYS_16_BIT) {
    return ETCF_ALWAYS_16_BIT;
  }
  if (flags & ETCF_ALWAYS_32_BIT) {
    return ETCF_ALWAYS_32_BIT;
  }
  if (flags & ETCF_OPTIMIZED_FOR_QUALITY) {
    return ETCF_OPTIMIZED_FOR_QUALITY;
  }
  return ETCF_OPTIMIZED_FOR_SPEED;
}
typedef struct IImage {
  u8* Data;
  ISIZE Size;
  u32 BytesPerPixel;
  u32 Pitch;
  ECOLOR_FORMAT Format;
} IImage;
#define VFUNDEF(ret, name, args)   ret (*name) args;
#define ITEXTURE_DEF(TT) \
  char NamedPath[256]; \
  ISIZE OriginalSize; \
  ISIZE TextureSize; \
  ECOLOR_FORMAT ColorFormat; \
  int Pitch; \
  bool HasMipMaps; \
  TT(void*, lock, (ITexture* s, E_TEXTURE_LOCK_MODE mode DEFAULT(ETLM_READ_WRITE), u32 mipmapLevel DEFAULT(0))) \
  TT(void, unlock, (ITexture* s)) \
  TT(void, regenerateMipMapLevels, (ITexture* s, void* mipmapData))
typedef struct ITexture ITexture;
struct ITexture {
  IImage Image[1];
  IImage Texture[1];
  bool IsRenderTarget;
  ITEXTURE_DEF(VFUNDEF)
};
typedef struct SMaterialLayer {
  //! Texture
  ITexture* Texture;
  //! Texture Clamp Mode
  /** Values are taken from E_TEXTURE_CLAMP. */
  u8 TextureWrapU: 4;
  u8 TextureWrapV: 4;
  //! Is bilinear filtering enabled? Default: true
  bool BilinearFilter: 1;
  //! Is trilinear filtering enabled? Default: false
  /** If the trilinear filter flag is enabled,
  the bilinear filtering flag is ignored. */
  bool TrilinearFilter: 1;
  //! Is anisotropic filtering enabled? Default: 0, disabled
  u8 AnisotropicFilter;
  //! Bias for the mipmap choosing decision.
  s8 LODBias;
  //! Texture Matrix
  float16 TextureMatrix;
} SMaterialLayer;
//! Maximum number of texture an SMaterial can have, up to 8 are supported by Irrlicht.
#define _IRR_MATERIAL_MAX_TEXTURES_ 4
typedef struct SMaterial {
  SMaterialLayer TextureLayer[MATERIAL_MAX_TEXTURES];  //! Texture layer array.
  E_MATERIAL_TYPE MaterialType;  //! Type of the material. Specifies how everything is blended together
  SColor AmbientColor;  //! How much ambient light (a global light) is reflected by this material.
  SColor DiffuseColor;  //! How much diffuse light coming from a light source is reflected by this material.
  SColor EmissiveColor;  //! Light emitted by this material. Default is to emit no light.
  SColor SpecularColor;  //! How much specular light (highlights from a light) is reflected.
  f32 Shininess;  //! Value affecting the size of specular highlights.
  f32 MaterialTypeParam;  //! Free parameter, dependent on the material type.
  f32 MaterialTypeParam2;  //! Second free parameter, dependent on the material type.
  f32 Thickness;  //! Thickness of non-3dimensional elements such as lines and points.
  u8 ZBuffer;  //! Is the ZBuffer enabled? Default: ECFN_LESSEQUAL
  u8 AntiAliasing;  //! Sets the antialiasing mode
  u8 ColorMask: 4;  //! Defines the enabled color planes
  u8 ColorMaterial: 3;  //! Defines the interpretation of vertex color in the lighting equation
  E_BLEND_OPERATION BlendOperation: 4;  //! Store the blend operation of choice
  u8 PolygonOffsetFactor: 3;  //! Factor specifying how far the polygon offset should be made
  E_POLYGON_OFFSET PolygonOffsetDirection: 1;  //! Flag defining the direction the polygon offset is applied to.
  bool Wireframe: 1;  //! Draw as wireframe or filled triangles? Default: false
  bool PointCloud: 1;  //! Draw as point cloud or filled triangles? Default: false
  bool GouraudShading: 1;  //! Flat or Gouraud shading? Default: true
  bool Lighting: 1;  //! Will this material be lighted? Default: true
  bool ZWriteEnable: 1;  //! Is the zbuffer writeable or is it read-only. Default: true.
  bool BackfaceCulling: 1;  //! Is backface culling enabled? Default: true
  bool FrontfaceCulling: 1;  //! Is frontface culling enabled? Default: false
  bool FogEnable: 1;  //! Is fog enabled? Default: false
  bool NormalizeNormals: 1;  //! Should normals be normalized?
  bool UseMipMaps: 1;  //! Shall mipmaps be used if available
} SMaterial;
#define IMESHBUFFER_DEF(TT)  \
TT(const F3AABBOX*, getBoundingBox, ()) \
TT(E_HARDWARE_MAPPING, getHardwareMappingHint_Index, ()) \
TT(E_HARDWARE_MAPPING, getHardwareMappingHint_Vertex, ()) \
TT(E_INDEX_TYPE, getIndexType, ()) \
TT(E_VERTEX_TYPE, getVertexType, ()) \
TT(float*, getNormal, (u32 i)) \
TT(float*, getPosition, (u32 i)) \
TT(float*, getTCoords, (u32 i)) \
TT(SMaterial*, getMaterial, ()) \
TT(u16*, getIndices, ()) \
TT(u32, getChangedID_Index, ()) \
TT(u32, getChangedID_Vertex, ()) \
TT(u32, getIndexCount, ()) \
TT(u32, getVertexCount, ()) \
TT(void*, getVertices, ()) \
TT(void, append, (const IMeshBuffer* other)) \
TT(void, append2, (const void* vertices, u32 numVertices, const u16* indices, u32 numIndices)) \
TT(void, recalculateBoundingBox, ()) \
TT(void, setBoundingBox, (const F3AABBOX* box)) \
TT(void, setDirty, (E_BUFFER_TYPE buffer DEFAULT(EBT_VERTEX_AND_INDEX))) \
TT(void, setHardwareMappingHint, (E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer DEFAULT(EBT_VERTEX_AND_INDEX))) \
void* x;


typedef struct IMeshBuffer {
  u32 ChangedID_Vertex;
  u32 ChangedID_Index;
  //! hardware mapping hint
  E_HARDWARE_MAPPING MappingHint_Vertex;
  E_HARDWARE_MAPPING MappingHint_Index;
  //! Material for this meshbuffer.
  SMaterial Material;
  //! Vertices of this buffer
  S3DVertex* Vertices;
  int Vertices_size;
  E_VERTEX_TYPE VertexType;
  //! Indices into the vertices of this buffer.
  u16* Indices;
  int Indices_size;
  //! Bounding box of this meshbuffer.
  F3AABBOX BoundingBox;
} IMeshBuffer;
//! An enumeration for all types of automatic culling for built-in scene nodes
typedef enum E_CULLING_TYPE {
  EAC_OFF = 0,
  EAC_BOX = 1,
  EAC_FRUSTUM_BOX = 2,
  EAC_FRUSTUM_SPHERE = 4,
  EAC_OCC_QUERY = 8
} E_CULLING_TYPE;
//! Names for culling type
const c8* const AutomaticCullingNames[] = {
  "false",
  "box",      // camera box against node box
  "frustum_box",    // camera frustum against node box
  "frustum_sphere", // camera frustum against node sphere
  "occ_query",  // occlusion query
  0
};
//! An enumeration for all types of debug data for built-in scene nodes (flags)
typedef enum E_DEBUG_SCENE_TYPE {
  //! No Debug Data ( Default )
  EDS_OFF = 0,
  //! Show Bounding Boxes of SceneNode
  EDS_BBOX = 1,
  //! Show Vertex Normals
  EDS_NORMALS = 2,
  //! Shows Skeleton/Tags
  EDS_SKELETON = 4,
  //! Overlays Mesh Wireframe
  EDS_MESH_WIRE_OVERLAY = 8,
  //! Temporary use transparency Material Type
  EDS_HALF_TRANSPARENCY = 16,
  //! Show Bounding Boxes of all MeshBuffers
  EDS_BBOX_BUFFERS = 32,
  //! EDS_BBOX | EDS_BBOX_BUFFERS
  EDS_BBOX_ALL = EDS_BBOX | EDS_BBOX_BUFFERS,
  //! Show all debug infos
  EDS_FULL = 0xffffffff
} E_DEBUG_SCENE_TYPE;
//! enumeration for querying features of the video driver.
typedef enum E_VIDEO_DRIVER_FEATURE {
  //! Is driver able to render to a surface?
  EVDF_RENDER_TO_TARGET = 0,
  //! Is hardeware transform and lighting supported?
  EVDF_HARDWARE_TL,
  //! Are multiple textures per material possible?
  EVDF_MULTITEXTURE,
  //! Is driver able to render with a bilinear filter applied?
  EVDF_BILINEAR_FILTER,
  //! Can the driver handle mip maps?
  EVDF_MIP_MAP,
  //! Can the driver update mip maps automatically?
  EVDF_MIP_MAP_AUTO_UPDATE,
  //! Are stencilbuffers switched on and does the device support stencil buffers?
  EVDF_STENCIL_BUFFER,
  //! Is Vertex Shader 1.1 supported?
  EVDF_VERTEX_SHADER_1_1,
  //! Is Vertex Shader 2.0 supported?
  EVDF_VERTEX_SHADER_2_0,
  //! Is Vertex Shader 3.0 supported?
  EVDF_VERTEX_SHADER_3_0,
  //! Is Pixel Shader 1.1 supported?
  EVDF_PIXEL_SHADER_1_1,
  //! Is Pixel Shader 1.2 supported?
  EVDF_PIXEL_SHADER_1_2,
  //! Is Pixel Shader 1.3 supported?
  EVDF_PIXEL_SHADER_1_3,
  //! Is Pixel Shader 1.4 supported?
  EVDF_PIXEL_SHADER_1_4,
  //! Is Pixel Shader 2.0 supported?
  EVDF_PIXEL_SHADER_2_0,
  //! Is Pixel Shader 3.0 supported?
  EVDF_PIXEL_SHADER_3_0,
  //! Are ARB vertex programs v1.0 supported?
  EVDF_ARB_VERTEX_PROGRAM_1,
  //! Are ARB fragment programs v1.0 supported?
  EVDF_ARB_FRAGMENT_PROGRAM_1,
  //! Is GLSL supported?
  EVDF_ARB_GLSL,
  //! Is HLSL supported?
  EVDF_HLSL,
  //! Are non-square textures supported?
  EVDF_TEXTURE_NSQUARE,
  //! Are non-power-of-two textures supported?
  EVDF_TEXTURE_NPOT,
  //! Are framebuffer objects supported?
  EVDF_FRAMEBUFFER_OBJECT,
  //! Are vertex buffer objects supported?
  EVDF_VERTEX_BUFFER_OBJECT,
  //! Supports Alpha To Coverage
  EVDF_ALPHA_TO_COVERAGE,
  //! Supports Color masks (disabling color planes in output)
  EVDF_COLOR_MASK,
  //! Supports multiple render targets at once
  EVDF_MULTIPLE_RENDER_TARGETS,
  //! Supports separate blend settings for multiple render targets
  EVDF_MRT_BLEND,
  //! Supports separate color masks for multiple render targets
  EVDF_MRT_COLOR_MASK,
  //! Supports separate blend functions for multiple render targets
  EVDF_MRT_BLEND_FUNC,
  //! Supports geometry shaders
  EVDF_GEOMETRY_SHADER,
  //! Supports occlusion queries
  EVDF_OCCLUSION_QUERY,
  //! Supports polygon offset/depth bias for avoiding z-fighting
  EVDF_POLYGON_OFFSET,
  //! Support for different blend functions. Without, only ADD is available
  EVDF_BLEND_OPERATIONS,
  //! Support for texture coord transformation via texture matrix
  EVDF_TEXTURE_MATRIX,
  //! Support for NVidia's CG shader language
  EVDF_CG,
  //! Only used for counting the elements of this enum
  EVDF_COUNT
} E_VIDEO_DRIVER_FEATURE;
typedef enum EGUI_ALIGNMENT {
  //! Aligned to parent's top or left side (default)
  EGUIA_UPPERLEFT = 0,
  //! Aligned to parent's bottom or right side
  EGUIA_LOWERRIGHT,
  //! Aligned to the center of parent
  EGUIA_CENTER,
  //! Stretched to fit parent
  EGUIA_SCALE
} EGUI_ALIGNMENT;
//! Names for alignments
const c8* const GUIAlignmentNames[] = {
  "upperLeft",
  "lowerRight",
  "center",
  "scale",
  0
};
//! List of all basic Irrlicht GUI elements.
/** An IGUIElement returns this when calling IGUIElement::getType(); */
typedef enum EGUI_ELEMENT_TYPE {
  //! A button (IGUIButton)
  EGUIET_BUTTON = 0,
  //! A check box (IGUICheckBox)
  EGUIET_CHECK_BOX,
  //! A combo box (IGUIComboBox)
  EGUIET_COMBO_BOX,
  //! A context menu (IGUIContextMenu)
  EGUIET_CONTEXT_MENU,
  //! A menu (IGUIMenu)
  EGUIET_MENU,
  //! An edit box (IGUIEditBox)
  EGUIET_EDIT_BOX,
  //! A file open dialog (IGUIFileOpenDialog)
  EGUIET_FILE_OPEN_DIALOG,
  //! A color select open dialog (IGUIColorSelectDialog)
  EGUIET_COLOR_SELECT_DIALOG,
  //! A in/out fader (IGUIInOutFader)
  EGUIET_IN_OUT_FADER,
  //! An image (IGUIImage)
  EGUIET_IMAGE,
  //! A list box (IGUIListBox)
  EGUIET_LIST_BOX,
  //! A mesh viewer (IGUIMeshViewer)
  EGUIET_MESH_VIEWER,
  //! A message box (IGUIWindow)
  EGUIET_MESSAGE_BOX,
  //! A modal screen
  EGUIET_MODAL_SCREEN,
  //! A scroll bar (IGUIScrollBar)
  EGUIET_SCROLL_BAR,
  //! A spin box (IGUISpinBox)
  EGUIET_SPIN_BOX,
  //! A static text (IGUIStaticText)
  EGUIET_STATIC_TEXT,
  //! A tab (IGUITab)
  EGUIET_TAB,
  //! A tab control
  EGUIET_TAB_CONTROL,
  //! A Table
  EGUIET_TABLE,
  //! A tool bar (IGUIToolBar)
  EGUIET_TOOL_BAR,
  //! A Tree View
  EGUIET_TREE_VIEW,
  //! A window
  EGUIET_WINDOW,
  //! Unknown type.
  EGUIET_ELEMENT,
  //! The root of the GUI
  EGUIET_ROOT,
  //! Not an element, amount of elements in there
  EGUIET_COUNT,
  //! This enum is never used, it only forces the compiler to compile this enumeration to 32 bit.
  EGUIET_FORCE_32_BIT = 0x7fffffff
} EGUI_ELEMENT_TYPE;
//! Names for built-in element types
static const c8* const GUIElementTypeNames[] = {
  "button",
  "checkBox",
  "comboBox",
  "contextMenu",
  "menu",
  "editBox",
  "fileOpenDialog",
  "colorSelectDialog",
  "inOutFader",
  "image",
  "listBox",
  "meshViewer",
  "messageBox",
  "modalScreen",
  "scrollBar",
  "spinBox",
  "staticText",
  "tab",
  "tabControl",
  "table",
  "toolBar",
  "treeview",
  "window",
  "element",
  "root",
  0
};
//! An enumeration for all supported types of built-in mesh writers
/** A scene mesh writers is represented by a four character code
such as 'irrm' or 'coll' instead of simple numbers, to avoid
name clashes with external mesh writers.*/
typedef enum EMESH_WRITER_TYPE {
  //! Irrlicht native mesh writer, for static .irrmesh files.
  EMWT_IRR_MESH     = MAKE_IRR_ID('i', 'r', 'r', 'm'),
  //! COLLADA mesh writer for .dae and .xml files
  EMWT_COLLADA      = MAKE_IRR_ID('c', 'o', 'l', 'l'),
  //! STL mesh writer for .stl files
  EMWT_STL          = MAKE_IRR_ID('s', 't', 'l', 0),
  //! OBJ mesh writer for .obj files
  EMWT_OBJ          = MAKE_IRR_ID('o', 'b', 'j', 0),
  //! PLY mesh writer for .ply files
  EMWT_PLY          = MAKE_IRR_ID('p', 'l', 'y', 0)
} EMESH_WRITER_TYPE;
//! flags configuring mesh writing
typedef enum E_MESH_WRITER_FLAGS {
  //! no writer flags
  EMWF_NONE = 0,
  //! write lightmap textures out if possible
  EMWF_WRITE_LIGHTMAPS = 0x1,
  //! write in a way that consumes less disk space
  EMWF_WRITE_COMPRESSED = 0x2,
  //! write in binary format rather than text
  EMWF_WRITE_BINARY = 0x4
} E_MESH_WRITER_FLAGS;
//! enumeration for message box layout flags
typedef enum EMESSAGE_BOX_FLAG {
  //! Flag for the ok button
  EMBF_OK = 0x1,
  //! Flag for the cancel button
  EMBF_CANCEL = 0x2,
  //! Flag for the yes button
  EMBF_YES = 0x4,
  //! Flag for the no button
  EMBF_NO = 0x8,
  //! This value is not used. It only forces this enumeration to compile in 32 bit.
  EMBF_FORCE_32BIT = 0x7fffffff
} EMESSAGE_BOX_FLAG;
//! An enumeration for all types of built-in scene node animators
typedef enum ESCENE_NODE_ANIMATOR_TYPE {
  //! Fly circle scene node animator
  ESNAT_FLY_CIRCLE = 0,
  //! Fly straight scene node animator
  ESNAT_FLY_STRAIGHT,
  //! Follow spline scene node animator
  ESNAT_FOLLOW_SPLINE,
  //! Rotation scene node animator
  ESNAT_ROTATION,
  //! Texture scene node animator
  ESNAT_TEXTURE,
  //! Deletion scene node animator
  ESNAT_DELETION,
  //! Collision respose scene node animator
  ESNAT_COLLISION_RESPONSE,
  //! FPS camera animator
  ESNAT_CAMERA_FPS,
  //! Maya camera animator
  ESNAT_CAMERA_MAYA,
  //! Amount of built-in scene node animators
  ESNAT_COUNT,
  //! Unknown scene node animator
  ESNAT_UNKNOWN,
  //! This enum is never used, it only forces the compiler to compile this enumeration to 32 bit.
  ESNAT_FORCE_32_BIT = 0x7fffffff
} ESCENE_NODE_ANIMATOR_TYPE;
//! An enumeration for all types of built-in scene nodes
/** A scene node type is represented by a four character code
such as 'cube' or 'mesh' instead of simple numbers, to avoid
name clashes with external scene nodes.*/
typedef enum ESCENE_NODE_TYPE {
  //! of type CSceneManager (note that ISceneManager is not(!) an ISceneNode)
  ESNT_SCENE_MANAGER  = MAKE_IRR_ID('s', 'm', 'n', 'g'),
  //! simple cube scene node
  ESNT_CUBE           = MAKE_IRR_ID('c', 'u', 'b', 'e'),
  //! Sphere scene node
  ESNT_SPHERE         = MAKE_IRR_ID('s', 'p', 'h', 'r'),
  //! Text Scene Node
  ESNT_TEXT           = MAKE_IRR_ID('t', 'e', 'x', 't'),
  //! Water Surface Scene Node
  ESNT_WATER_SURFACE  = MAKE_IRR_ID('w', 'a', 't', 'r'),
  //! Terrain Scene Node
  ESNT_TERRAIN        = MAKE_IRR_ID('t', 'e', 'r', 'r'),
  //! Sky Box Scene Node
  ESNT_SKY_BOX        = MAKE_IRR_ID('s', 'k', 'y', '_'),
  //! Sky Dome Scene Node
  ESNT_SKY_DOME       = MAKE_IRR_ID('s', 'k', 'y', 'd'),
  //! Shadow Volume Scene Node
  ESNT_SHADOW_VOLUME  = MAKE_IRR_ID('s', 'h', 'd', 'w'),
  //! Octree Scene Node
  ESNT_OCTREE         = MAKE_IRR_ID('o', 'c', 't', 'r'),
  //! Mesh Scene Node
  ESNT_MESH           = MAKE_IRR_ID('m', 'e', 's', 'h'),
  //! Light Scene Node
  ESNT_LIGHT          = MAKE_IRR_ID('l', 'g', 'h', 't'),
  //! Empty Scene Node
  ESNT_EMPTY          = MAKE_IRR_ID('e', 'm', 't', 'y'),
  //! Dummy Transformation Scene Node
  ESNT_DUMMY_TRANSFORMATION = MAKE_IRR_ID('d', 'm', 'm', 'y'),
  //! Camera Scene Node
  ESNT_CAMERA         = MAKE_IRR_ID('c', 'a', 'm', '_'),
  //! Billboard Scene Node
  ESNT_BILLBOARD      = MAKE_IRR_ID('b', 'i', 'l', 'l'),
  //! Animated Mesh Scene Node
  ESNT_ANIMATED_MESH  = MAKE_IRR_ID('a', 'm', 's', 'h'),
  //! Particle System Scene Node
  ESNT_PARTICLE_SYSTEM = MAKE_IRR_ID('p', 't', 'c', 'l'),
  //! Quake3 Shader Scene Node
  ESNT_Q3SHADER_SCENE_NODE  = MAKE_IRR_ID('q', '3', 's', 'h'),
  //! Quake3 Model Scene Node ( has tag to link to )
  ESNT_MD3_SCENE_NODE  = MAKE_IRR_ID('m', 'd', '3', '_'),
  //! Volume Light Scene Node
  ESNT_VOLUME_LIGHT  = MAKE_IRR_ID('v', 'o', 'l', 'l'),
  //! Maya Camera Scene Node
  /** Legacy, for loading version <= 1.4.x .irr files */
  ESNT_CAMERA_MAYA    = MAKE_IRR_ID('c', 'a', 'm', 'M'),
  //! First Person Shooter Camera
  /** Legacy, for loading version <= 1.4.x .irr files */
  ESNT_CAMERA_FPS     = MAKE_IRR_ID('c', 'a', 'm', 'F'),
  //! Unknown scene node
  ESNT_UNKNOWN        = MAKE_IRR_ID('u', 'n', 'k', 'n'),
  //! Will match with any scene node when checking types
  ESNT_ANY            = MAKE_IRR_ID('a', 'n', 'y', '_')
} ESCENE_NODE_TYPE;
//! enumeration for patch sizes specifying the size of patches in the TerrainSceneNode
typedef enum E_TERRAIN_PATCH_SIZE {
  //! patch size of 9, at most, use 4 levels of detail with this patch size.
  ETPS_9 = 9,
  //! patch size of 17, at most, use 5 levels of detail with this patch size.
  ETPS_17 = 17,
  //! patch size of 33, at most, use 6 levels of detail with this patch size.
  ETPS_33 = 33,
  //! patch size of 65, at most, use 7 levels of detail with this patch size.
  ETPS_65 = 65,
  //! patch size of 129, at most, use 8 levels of detail with this patch size.
  ETPS_129 = 129
} E_TERRAIN_PATCH_SIZE;
//! Possible types of (animated) meshes.
typedef enum E_ANIMATED_MESH_TYPE {
  //! Unknown animated mesh type.
  EAMT_UNKNOWN = 0,
  //! Quake 2 MD2 model file
  EAMT_MD2,
  //! Quake 3 MD3 model file
  EAMT_MD3,
  //! Maya .obj static model
  EAMT_OBJ,
  //! Quake 3 .bsp static Map
  EAMT_BSP,
  //! 3D Studio .3ds file
  EAMT_3DS,
  //! My3D Mesh, the file format by Zhuck Dimitry
  EAMT_MY3D,
  //! Pulsar LMTools .lmts file. This Irrlicht loader was written by Jonas Petersen
  EAMT_LMTS,
  //! Cartography Shop .csm file. This loader was created by Saurav Mohapatra.
  EAMT_CSM,
  //! .oct file for Paul Nette's FSRad or from Murphy McCauley's Blender .oct exporter.
  /** The oct file format contains 3D geometry and lightmaps and
  can be loaded directly by Irrlicht */
  EAMT_OCT,
  //! Halflife MDL model file
  EAMT_MDL_HALFLIFE,
  //! generic skinned mesh
  EAMT_SKINNED
} E_ANIMATED_MESH_TYPE;
#define IREADFILE_DEF(TT) \
  TT(s32, read, (void* buffer, u32 sizeToRead) )  \
  TT(bool,seek, (long finalPos, bool relativeMovement)) \
  TT(long, getSize, () ) \
  TT(long, getPos, () ) \
  TT(int, getFileName, (char* name, int len) )
//! Interface providing read acess to a file.
typedef struct IReadFile {
  IREADFILE_DEF(VFUNDEF)
} IReadFile;
typedef struct SFileListEntry {
  char Name[256];
  //! The name of the file including the path
  char FullName[256];
  //! The size of the file in bytes
  u32 Size;
  //! The ID of the file in an archive
  /** This is used to link the FileList entry to extra info held about this
  file in an archive, which can hold things like data offset and CRC. */
  u32 ID;
  //! FileOffset inside an archive
  u32 Offset;
  //! True if this is a folder, false if not.
  bool IsDirectory;
} SFileListEntry;
//! Interface for logging messages, warnings and errors
void ILogger_log(const c8* text, const c8* hint, ELOG_LEVEL ll)
{
  static const char* levelstr[] = {"DEBUG", "INFO", "WARNING", "ERROR", "NONE"};
  printf("%s:%s, %s", levelstr[ll], text, hint);
}
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
//! Enumeration for all event types there are.
typedef enum EEVENT_TYPE {
  //! An event of the graphical user interface.
  /** GUI events are created by the GUI environment or the GUI elements in response
  to mouse or keyboard events. When a GUI element receives an event it will either
  process it and return true, or pass the event to its parent. If an event is not absorbed
  before it reaches the root element then it will then be passed to the user receiver. */
  EET_GUI_EVENT = 0,
  //! A mouse input event.
  /** Mouse events are created by the device and passed to IDevice::postEventFromUser
  in response to mouse input received from the operating system.
  Mouse events are first passed to the user receiver, then to the GUI environment and its elements,
  then finally the input receiving scene manager where it is passed to the active camera.
  */
  EET_MOUSE_INPUT_EVENT,
  //! A key input event.
  /** Like mouse events, keyboard events are created by the device and passed to
  IDevice::postEventFromUser. They take the same path as mouse events. */
  EET_KEY_INPUT_EVENT,
  //! A joystick (joypad, gamepad) input event.
  /** Joystick events are created by polling all connected joysticks once per
  device run() and then passing the events to IDevice::postEventFromUser.
  They take the same path as mouse events.
  Windows, SDL: Implemented.
  Linux: Implemented, with POV hat issues.
  MacOS / Other: Not yet implemented.
  */
  EET_JOYSTICK_INPUT_EVENT,
  //! A log event
  /** Log events are only passed to the user receiver if there is one. If they are absorbed by the
  user receiver then no text will be sent to the console. */
  EET_LOG_TEXT_EVENT,
  //! A user event with user data.
  /** This is not used by Irrlicht and can be used to send user
  specific data though the system. The Irrlicht 'window handle'
  can be obtained from IDevice::getExposedVideoData()
  The usage and behavior depends on the operating system:
  Windows: send a WM_USER message to the Irrlicht Window; the
    wParam and lParam will be used to populate the
    UserData1 and UserData2 members of the SUserEvent.
  Linux: send a ClientMessage via XSendEvent to the Irrlicht
    Window; the data.l[0] and data.l[1] members will be
    casted to s32 and used as UserData1 and UserData2.
  MacOS: Not yet implemented
  */
  EET_USER_EVENT,
  //! This enum is never used, it only forces the compiler to
  //! compile these enumeration values to 32 bit.
  //EGUIET_FORCE_32_BIT = 0x7fffffff
} EEVENT_TYPE;
//! Enumeration for all mouse input events
typedef enum EMOUSE_INPUT_EVENT {
  //! Left mouse button was pressed down.
  EMIE_LMOUSE_PRESSED_DOWN = 0,
  //! Right mouse button was pressed down.
  EMIE_RMOUSE_PRESSED_DOWN,
  //! Middle mouse button was pressed down.
  EMIE_MMOUSE_PRESSED_DOWN,
  //! Left mouse button was left up.
  EMIE_LMOUSE_LEFT_UP,
  //! Right mouse button was left up.
  EMIE_RMOUSE_LEFT_UP,
  //! Middle mouse button was left up.
  EMIE_MMOUSE_LEFT_UP,
  //! The mouse cursor changed its position.
  EMIE_MOUSE_MOVED,
  //! The mouse wheel was moved. Use Wheel value in event data to find out
  //! in what direction and how fast.
  EMIE_MOUSE_WHEEL,
  //! Left mouse button double click.
  //! This event is generated after the second EMIE_LMOUSE_PRESSED_DOWN event.
  EMIE_LMOUSE_DOUBLE_CLICK,
  //! Right mouse button double click.
  //! This event is generated after the second EMIE_RMOUSE_PRESSED_DOWN event.
  EMIE_RMOUSE_DOUBLE_CLICK,
  //! Middle mouse button double click.
  //! This event is generated after the second EMIE_MMOUSE_PRESSED_DOWN event.
  EMIE_MMOUSE_DOUBLE_CLICK,
  //! Left mouse button triple click.
  //! This event is generated after the third EMIE_LMOUSE_PRESSED_DOWN event.
  EMIE_LMOUSE_TRIPLE_CLICK,
  //! Right mouse button triple click.
  //! This event is generated after the third EMIE_RMOUSE_PRESSED_DOWN event.
  EMIE_RMOUSE_TRIPLE_CLICK,
  //! Middle mouse button triple click.
  //! This event is generated after the third EMIE_MMOUSE_PRESSED_DOWN event.
  EMIE_MMOUSE_TRIPLE_CLICK,
  //! No real event. Just for convenience to get number of events
  EMIE_COUNT
} EMOUSE_INPUT_EVENT;
//! Masks for mouse button states
typedef enum E_MOUSE_BUTTON_STATE_MASK {
  EMBSM_LEFT    = 0x01,
  EMBSM_RIGHT   = 0x02,
  EMBSM_MIDDLE  = 0x04,
  //! currently only on windows
  EMBSM_EXTRA1  = 0x08,
  //! currently only on windows
  EMBSM_EXTRA2  = 0x10,
  EMBSM_FORCE_32_BIT = 0x7fffffff
} E_MOUSE_BUTTON_STATE_MASK;
//! Enumeration for all events which are sendable by the gui system
typedef enum EGUI_EVENT_TYPE {
  //! A gui element has lost its focus.
  /** GUIEvent.Caller is losing the focus to GUIEvent.Element.
  If the event is absorbed then the focus will not be changed. */
  EGET_ELEMENT_FOCUS_LOST = 0,
  //! A gui element has got the focus.
  /** If the event is absorbed then the focus will not be changed. */
  EGET_ELEMENT_FOCUSED,
  //! The mouse cursor hovered over a gui element.
  /** If an element has sub-elements you also get this message for the subelements */
  EGET_ELEMENT_HOVERED,
  //! The mouse cursor left the hovered element.
  /** If an element has sub-elements you also get this message for the subelements */
  EGET_ELEMENT_LEFT,
  //! An element would like to close.
  /** Windows and context menus use this event when they would like to close,
  this can be cancelled by absorbing the event. */
  EGET_ELEMENT_CLOSED,
  //! A button was clicked.
  EGET_BUTTON_CLICKED,
  //! A scrollbar has changed its position.
  EGET_SCROLL_BAR_CHANGED,
  //! A checkbox has changed its check state.
  EGET_CHECKBOX_CHANGED,
  //! A new item in a listbox was selected.
  /** NOTE: You also get this event currently when the same item was clicked again after more than 500 ms. */
  EGET_LISTBOX_CHANGED,
  //! An item in the listbox was selected, which was already selected.
  /** NOTE: You get the event currently only if the item was clicked again within 500 ms or selected by "enter" or "space". */
  EGET_LISTBOX_SELECTED_AGAIN,
  //! A file has been selected in the file dialog
  EGET_FILE_SELECTED,
  //! A directory has been selected in the file dialog
  EGET_DIRECTORY_SELECTED,
  //! A file open dialog has been closed without choosing a file
  EGET_FILE_CHOOSE_DIALOG_CANCELLED,
  //! 'Yes' was clicked on a messagebox
  EGET_MESSAGEBOX_YES,
  //! 'No' was clicked on a messagebox
  EGET_MESSAGEBOX_NO,
  //! 'OK' was clicked on a messagebox
  EGET_MESSAGEBOX_OK,
  //! 'Cancel' was clicked on a messagebox
  EGET_MESSAGEBOX_CANCEL,
  //! In an editbox 'ENTER' was pressed
  EGET_EDITBOX_ENTER,
  //! The text in an editbox was changed. This does not include automatic changes in text-breaking.
  EGET_EDITBOX_CHANGED,
  //! The marked area in an editbox was changed.
  EGET_EDITBOX_MARKING_CHANGED,
  //! The tab was changed in an tab control
  EGET_TAB_CHANGED,
  //! A menu item was selected in a (context) menu
  EGET_MENU_ITEM_SELECTED,
  //! The selection in a combo box has been changed
  EGET_COMBO_BOX_CHANGED,
  //! The value of a spin box has changed
  EGET_SPINBOX_CHANGED,
  //! A table has changed
  EGET_TABLE_CHANGED,
  EGET_TABLE_HEADER_CHANGED,
  EGET_TABLE_SELECTED_AGAIN,
  //! A tree view node lost selection. See IGUITreeView::getLastEventNode().
  EGET_TREEVIEW_NODE_DESELECT,
  //! A tree view node was selected. See IGUITreeView::getLastEventNode().
  EGET_TREEVIEW_NODE_SELECT,
  //! A tree view node was expanded. See IGUITreeView::getLastEventNode().
  EGET_TREEVIEW_NODE_EXPAND,
  //! A tree view node was collapsed. See IGUITreeView::getLastEventNode().
  EGET_TREEVIEW_NODE_COLLAPSE,
  //! deprecated - use EGET_TREEVIEW_NODE_COLLAPSE instead. This
  //! may be removed by Irrlicht 1.9
  EGET_TREEVIEW_NODE_COLLAPS = EGET_TREEVIEW_NODE_COLLAPSE,
  //! No real event. Just for convenience to get number of events
  EGET_COUNT
} EGUI_EVENT_TYPE;
typedef struct IEventReceiver IGUIElement;
//! Any kind of GUI event.
typedef struct SGUIEvent {
  //! IGUIElement who called the event
  IGUIElement* Caller;
  //! If the event has something to do with another element, it will be held here.
  IGUIElement* Element;
  //! Type of GUI Event
  EGUI_EVENT_TYPE EventType;
} SGUIEvent;
//! Any kind of mouse event.
typedef struct SMouseInput {
  //! X position of mouse cursor
  s32 X;
  //! Y position of mouse cursor
  s32 Y;
  //! mouse wheel delta, often 1.0 or -1.0, but can have other values < 0.f or > 0.f;
  /** Only valid if event was EMIE_MOUSE_WHEEL */
  f32 Wheel;
  //! True if shift was also pressed
  bool Shift: 1;
  //! True if ctrl was also pressed
  bool Control: 1;
  //! A bitmap of button states. You can use isButtonPressed() to determine
  //! if a button is pressed or not.
  //! Currently only valid if the event was EMIE_MOUSE_MOVED
  u32 ButtonStates;
  //! Is the left button pressed down?
  //bool isLeftPressed() const { return 0 != (ButtonStates & EMBSM_LEFT);  }
  //! Is the right button pressed down?
  //bool isRightPressed() const { return 0 != (ButtonStates & EMBSM_RIGHT); }
  //! Is the middle button pressed down?
  //bool isMiddlePressed() const { return 0 != (ButtonStates & EMBSM_MIDDLE); }
  //! Type of mouse event
  EMOUSE_INPUT_EVENT Event;
} SMouseInput;
//! Any kind of keyboard event.
typedef struct SKeyInput {
  //! Character corresponding to the key (0, if not a character)
  char Char;
  //! Key which has been pressed or released
  EKEY_CODE Key;
  //! If not true, then the key was left up
  bool PressedDown: 1;
  //! True if shift was also pressed
  bool Shift: 1;
  //! True if ctrl was also pressed
  bool Control: 1;
} SKeyInput;
enum {
  NUMBER_OF_BUTTONS = 32,
  AXIS_X = 0, // e.g. analog stick 1 left to right
  AXIS_Y,   // e.g. analog stick 1 top to bottom
  AXIS_Z,   // e.g. throttle, or analog 2 stick 2 left to right
  AXIS_R,   // e.g. rudder, or analog 2 stick 2 top to bottom
  AXIS_U,
  AXIS_V,
  NUMBER_OF_AXES
};
//! A joystick event.
typedef struct SJoystickEvent {
  /** A bitmap of button states.  You can use IsButtonPressed() to
  ( check the state of each button from 0 to (NUMBER_OF_BUTTONS - 1) */
  u32 ButtonStates;
  /** For AXIS_X, AXIS_Y, AXIS_Z, AXIS_R, AXIS_U and AXIS_V
  * Values are in the range -32768 to 32767, with 0 representing
  * the center position.  You will receive the raw value from the
  * joystick, and so will usually want to implement a dead zone around
  * the center of the range. Axes not supported by this joystick will
  * always have a value of 0. On Linux, POV hats are represented as axes,
  * usually the last two active axis.
  */
  s16 Axis[NUMBER_OF_AXES];
  /** The POV represents the angle of the POV hat in degrees * 100,
  * from 0 to 35,900.  A value of 65535 indicates that the POV hat
  * is centered (or not present).
  * This value is only supported on Windows.  On Linux, the POV hat
  * will be sent as 2 axes instead. */
  u16 POV;
  //! The ID of the joystick which generated this event.
  /** This is an internal Irrlicht index; it does not map directly
  * to any particular hardware joystick. */
  u8 Joystick;
  //! A helper function to check if a button is pressed.
  //bool IsButtonPressed(u32 button) const { if (button >= (u32)NUMBER_OF_BUTTONS) {  return false; } return (ButtonStates & (1 << button)) ? true : false;    }
} SJoystickEvent;
//! Any kind of log event.
typedef struct SLogEvent {
  //! Pointer to text which has been logged
  const c8* Text;
  //! Log level in which the text has been logged
  ELOG_LEVEL Level;
} SLogEvent;
//! Any kind of user event.
typedef struct SUserEvent {
  //! Some user specified data as int
  s32 UserData1;
  //! Another user specified data as int
  s32 UserData2;
} SUserEvent;
//! SEvents hold information about an event. See IEventReceiver for details on event handling.
typedef struct SEvent {
  EEVENT_TYPE EventType;
  union {
    struct SGUIEvent GUIEvent;
    struct SMouseInput MouseInput;
    struct SKeyInput KeyInput;
    struct SJoystickEvent JoystickEvent;
    struct SLogEvent LogEvent;
    struct SUserEvent UserEvent;
  };
} SEvent;
#define IEVENTRECEIVER_DEF(TT)  TT( bool, OnEvent, (const SEvent* event) )
typedef struct IEventReceiver {
  IEVENTRECEIVER_DEF(VFUNDEF)
} IEventReceiver;
//! Information on a joystick, returned from @ref IDevice::activateJoysticks()
typedef struct SJoystickInfo {
  //! The ID of the joystick
  /** This is an internal Irrlicht index; it does not map directly
   * to any particular hardware joystick. It corresponds to the
   * SJoystickEvent Joystick ID. */
  u8        Joystick;
  //! The name that the joystick uses to identify itself.
  char* Name;
  //! The number of buttons that the joystick has.
  u32       Buttons;
  //! The number of axes that the joystick has, i.e. X, Y, Z, R, U, V.
  /** Note: with a Linux device, the POV hat (if any) will use two axes. These
   *  will be included in this count. */
  u32       Axes;
  //! An indication of whether the joystick has a POV hat.
  /** A Windows device will identify the presence or absence or the POV hat.  A
   *  Linux device cannot, and will always return POV_HAT_UNKNOWN. */
  enum {
    //! A hat is definitely present.
    POV_HAT_PRESENT,
    //! A hat is definitely not present.
    POV_HAT_ABSENT,
    //! The presence or absence of a hat cannot be determined.
    POV_HAT_UNKNOWN
  } PovHat;
} SJoystickInfo; // struct SJoystickInfo
//! enumeration for geometry transformation states
typedef enum E_TRANSFORMATION_STATE {
  //! View transformation
  ETS_VIEW = 0,
  //! World transformation
  ETS_WORLD,
  //! Projection transformation
  ETS_PROJECTION,
  //! Texture transformation
  ETS_TEXTURE_0,
  //! Texture transformation
  ETS_TEXTURE_1,
  //! Texture transformation
  ETS_TEXTURE_2,
  //! Texture transformation
  ETS_TEXTURE_3,
#if _IRR_MATERIAL_MAX_TEXTURES_>4
  //! Texture transformation
  ETS_TEXTURE_4,
#if _IRR_MATERIAL_MAX_TEXTURES_>5
  //! Texture transformation
  ETS_TEXTURE_5,
#if _IRR_MATERIAL_MAX_TEXTURES_>6
  //! Texture transformation
  ETS_TEXTURE_6,
#if _IRR_MATERIAL_MAX_TEXTURES_>7
  //! Texture transformation
  ETS_TEXTURE_7,
#endif
#endif
#endif
#endif
  //! Not used
  ETS_COUNT
} E_TRANSFORMATION_STATE;
//! enumeration for signaling resources which were lost after the last render cycle
/** These values can be signaled by the driver, telling the app that some resources
were lost and need to be recreated. Irrlicht will sometimes recreate the actual objects,
but the content needs to be recreated by the application. */
typedef enum E_LOST_RESOURCE {
  //! The whole device/driver is lost
  ELR_DEVICE = 1,
  //! All texture are lost, rare problem
  ELR_TEXTURES = 2,
  //! The Render Target Textures are lost, typical problem for D3D
  ELR_RTTS = 4,
  //! The HW buffers are lost, will be recreated automatically, but might require some more time this frame
  ELR_HW_BUFFERS = 8
} E_LOST_RESOURCE;
//! Special render targets, which usually map to dedicated hardware
/** These render targets (besides 0 and 1) need not be supported by gfx cards */
typedef enum E_RENDER_TARGET {
  //! Render target is the main color frame buffer
  ERT_FRAME_BUFFER = 0,
  //! Render target is a render texture
  ERT_RENDER_TEXTURE,
  //! Multi-Render target textures
  ERT_MULTI_RENDER_TEXTURES,
  //! Render target is the main color frame buffer
  ERT_STEREO_LEFT_BUFFER,
  //! Render target is the right color buffer (left is the main buffer)
  ERT_STEREO_RIGHT_BUFFER,
  //! Render to both stereo buffers at once
  ERT_STEREO_BOTH_BUFFERS,
  //! Auxiliary buffer 0
  ERT_AUX_BUFFER0,
  //! Auxiliary buffer 1
  ERT_AUX_BUFFER1,
  //! Auxiliary buffer 2
  ERT_AUX_BUFFER2,
  //! Auxiliary buffer 3
  ERT_AUX_BUFFER3,
  //! Auxiliary buffer 4
  ERT_AUX_BUFFER4
} E_RENDER_TARGET;
//! Enum for the types of fog distributions to choose from
typedef enum E_FOG_TYPE {
  EFT_FOG_EXP = 0,
  EFT_FOG_LINEAR,
  EFT_FOG_EXP2
} E_FOG_TYPE;
const c8* const FogTypeNames[] = {
  "FogExp",
  "FogLinear",
  "FogExp2",
  0
};
typedef struct SOverrideMaterial {
  //! The Material values
  SMaterial Material;
  //! Which values are taken for override
  /** OR'ed values from E_MATERIAL_FLAGS. */
  u32 EnableFlags;
  //! Set in which render passes the material override is active.
  /** OR'ed values from E_SCENE_NODE_RENDER_PASS. */
  u16 EnablePasses;
  //! Global enable flag, overwritten by the SceneManager in each pass
  /** The Scenemanager uses the EnablePass array and sets Enabled to
  true if the Override material is enabled in the current pass. */
  bool Enabled;
} SOverrideMaterial;
//! Default constructor
//SOverrideMaterial() : EnableFlags(0), EnablePasses(0), Enabled(false) {}
//! Apply the enabled overrides
CV_INLINE void SOverrideMaterial_apply(const SOverrideMaterial* s, SMaterial* material)
{
  if (s->Enabled) {
    int i;
    const SMaterial* Material = &s->Material;
    for (i = 0; i < 32; ++i) {
      const u32 num = (1 << i);
      if (s->EnableFlags & num) {
        switch (num) {
        case EMF_WIREFRAME:
            material->Wireframe = Material->Wireframe;
          break;
        case EMF_POINTCLOUD:
            material->PointCloud = Material->PointCloud;
          break;
        case EMF_GOURAUD_SHADING:
            material->GouraudShading = Material->GouraudShading;
          break;
        case EMF_LIGHTING:
            material->Lighting = Material->Lighting;
          break;
        case EMF_ZBUFFER:
            material->ZBuffer = Material->ZBuffer;
          break;
        case EMF_ZWRITE_ENABLE:
            material->ZWriteEnable = Material->ZWriteEnable;
          break;
        case EMF_BACK_FACE_CULLING:
            material->BackfaceCulling = Material->BackfaceCulling;
          break;
        case EMF_FRONT_FACE_CULLING:
            material->FrontfaceCulling = Material->FrontfaceCulling;
          break;
        case EMF_BILINEAR_FILTER:
            material->TextureLayer[0].BilinearFilter = Material->TextureLayer[0].BilinearFilter;
          break;
        case EMF_TRILINEAR_FILTER:
            material->TextureLayer[0].TrilinearFilter = Material->TextureLayer[0].TrilinearFilter;
          break;
        case EMF_ANISOTROPIC_FILTER:
            material->TextureLayer[0].AnisotropicFilter = Material->TextureLayer[0].AnisotropicFilter;
          break;
        case EMF_FOG_ENABLE:
            material->FogEnable = Material->FogEnable;
          break;
        case EMF_NORMALIZE_NORMALS:
            material->NormalizeNormals = Material->NormalizeNormals;
          break;
        case EMF_TEXTURE_WRAP:
            material->TextureLayer[0].TextureWrapU = Material->TextureLayer[0].TextureWrapU;
          material->TextureLayer[0].TextureWrapV = Material->TextureLayer[0].TextureWrapV;
          break;
        case EMF_ANTI_ALIASING:
            material->AntiAliasing = Material->AntiAliasing;
          break;
        case EMF_COLOR_MASK:
            material->ColorMask = Material->ColorMask;
          break;
        case EMF_COLOR_MATERIAL:
            material->ColorMaterial = Material->ColorMaterial;
          break;
        case EMF_USE_MIP_MAPS:
            material->UseMipMaps = Material->UseMipMaps;
          break;
        case EMF_BLEND_OPERATION:
            material->BlendOperation = Material->BlendOperation;
          break;
        case EMF_POLYGON_OFFSET:
            material->PolygonOffsetDirection = Material->PolygonOffsetDirection;
          material->PolygonOffsetFactor = Material->PolygonOffsetFactor;
          break;
        }
      }
    }
  }
}
typedef struct IRenderTarget {
  ITexture* RenderTexture;
  E_RENDER_TARGET TargetType: 8;
  E_COLOR_PLANE ColorMask: 8;
  E_BLEND_FACTOR BlendFuncSrc: 4;
  E_BLEND_FACTOR BlendFuncDst: 4;
  E_BLEND_OPERATION BlendOp: 4;
} IRenderTarget;
CV_INLINE void IRenderTarget_set(IRenderTarget* s, ITexture* texture, E_RENDER_TARGET target DEFAULT(ERT_RENDER_TEXTURE),
    E_COLOR_PLANE colorMask DEFAULT(ECP_ALL),
    E_BLEND_FACTOR blendFuncSrc DEFAULT(EBF_ONE),
    E_BLEND_FACTOR blendFuncDst DEFAULT(EBF_ONE_MINUS_SRC_ALPHA),
    E_BLEND_OPERATION blendOp DEFAULT(EBO_NONE))
{
  s->RenderTexture = (texture),
     s->TargetType = (target), s->ColorMask = (colorMask),
        s->BlendFuncSrc = (blendFuncSrc), s->BlendFuncDst = (blendFuncDst),
           s->BlendOp = (blendOp);
}
CV_INLINE bool IRenderTarget_neq(const IRenderTarget* s, const IRenderTarget* other)
{
  return ((s->RenderTexture != other->RenderTexture) ||
      (s->TargetType != other->TargetType) ||
      (s->ColorMask != other->ColorMask) ||
      (s->BlendFuncSrc != other->BlendFuncSrc) ||
      (s->BlendFuncDst != other->BlendFuncDst) ||
      (s->BlendOp != other->BlendOp));
}
typedef struct IDirect3D9 IDirect3D9;
typedef struct IDirect3DDevice9 IDirect3DDevice9;
typedef struct IDirect3D8 IDirect3D8;
typedef struct IDirect3DDevice8 IDirect3DDevice8;
//! structure for holding data describing a driver and operating system specific data.
/** This data can be retrived by IVideoDriver::getExposedVideoData(). Use this with caution.
This only should be used to make it possible to extend the engine easily without
modification of its source. Note that this structure does not contain any valid data, if
you are using the software or the null device.
*/
typedef union SExposedVideoData {
  struct {
    //! Pointer to the IDirect3D9 interface
    IDirect3D9* D3D9;
    //! Pointer to the IDirect3DDevice9 interface
    IDirect3DDevice9* D3DDev9;
    //! Window handle.
    /** Get with for example HWND h = reinterpret_cast<HWND>(exposedData.D3D9.HWnd) */
    void* HWnd;
  } D3D9;
  struct {
    //! Pointer to the IDirect3D8 interface
    IDirect3D8* D3D8;
    //! Pointer to the IDirect3DDevice8 interface
    IDirect3DDevice8* D3DDev8;
    //! Window handle.
    /** Get with for example with: HWND h = reinterpret_cast<HWND>(exposedData.D3D8.HWnd) */
    void* HWnd;
  } D3D8;
  struct {
    //! Private GDI Device Context.
    /** Get if for example with: HDC h = reinterpret_cast<HDC>(exposedData.OpenGLWin32.HDc) */
    void* HDc;
    //! Permanent Rendering Context.
    /** Get if for example with: HGLRC h = reinterpret_cast<HGLRC>(exposedData.OpenGLWin32.HRc) */
    void* HRc;
    //! Window handle.
    /** Get with for example with: HWND h = reinterpret_cast<HWND>(exposedData.OpenGLWin32.HWnd) */
    void* HWnd;
  } OpenGLWin32;
  struct {
    // XWindow handles
    void* X11Display;
    void* X11Context;
    unsigned long X11Window;
  } OpenGLLinux;
} SExposedVideoData;
typedef struct ISceneNodeAnimator {
  int x;
} ISceneNodeAnimator;
typedef struct ITriangleSelector {
  int x;
} ITriangleSelector;
typedef struct ISceneManager ISceneManager;
typedef struct ISceneNode ISceneNode;
typedef enum EMD2_ANIMATION_TYPE {
  EMAT_STAND = 0,
  EMAT_RUN,
  EMAT_ATTACK,
  EMAT_PAIN_A,
  EMAT_PAIN_B,
  EMAT_PAIN_C,
  EMAT_JUMP,
  EMAT_FLIP,
  EMAT_SALUTE,
  EMAT_FALLBACK,
  EMAT_WAVE,
  EMAT_POINT,
  EMAT_CROUCH_STAND,
  EMAT_CROUCH_WALK,
  EMAT_CROUCH_ATTACK,
  EMAT_CROUCH_PAIN,
  EMAT_CROUCH_DEATH,
  EMAT_DEATH_FALLBACK,
  EMAT_DEATH_FALLFORWARD,
  EMAT_DEATH_FALLBACKSLOW,
  EMAT_BOOM,
  //! Not an animation, but amount of animation types.
  EMAT_COUNT
} EMD2_ANIMATION_TYPE;
#define ISCENENODE_DEF(TT)   \
  TT(void, setMaterialFlag, (ISceneNode* s, E_MATERIAL_FLAG flag, bool newvalue) ) \
  TT(void, setMaterialTexture, (ISceneNode* s, u32 textureLayer, ITexture* texture) ) \
  TT(void, drawAll, (ISceneNode* s) ) \
  TT(bool, setMD2Animation, (ISceneNode* s, EMD2_ANIMATION_TYPE anim) )
struct ISceneNode {
  char Name[32];
  //! Absolute transformation of the node.
  float16 AbsoluteTransformation;
  //! Relative translation of the scene node.
  float3 RelativeTranslation;
  //! Relative rotation of the scene node.
  float3 RelativeRotation;
  //! Relative scale of the scene node.
  float3 RelativeScale;
  //! Pointer to the parent
  ISceneNode* Parent;
  //! List of all children of this node
  ISceneNode* Children;
  //! List of all animator nodes
  ISceneNodeAnimator** Animators;
  //! Pointer to the scene manager
  ISceneManager* SceneManager;
  //! Pointer to the triangle selector
  ITriangleSelector* TriangleSelector;
  //! ID of the node.
  s32 ID;
  //! Automatic culling state
  u32 AutomaticCullingState;
  //! Flag if debug data should be drawn, such as Bounding Boxes.
  u32 DebugDataVisible;
  //! Is the node visible?
  bool IsVisible;
  //! Is debug object?
  bool IsDebugObject;
  ISCENENODE_DEF(VFUNDEF)
};
//! Interface to driver which is able to perform 2d and 3d graphics functions.
/** This interface is one of the most important interfaces of
the Irrlicht Engine: All rendering and texture manipulation is done with
this interface. You are able to use the Irrlicht Engine by only
invoking methods of this interface if you like to, although the
ISceneManager interface provides a lot of powerful classes
and methods to make the programmer's life easier.
*/
typedef struct IMesh {
  //! The meshbuffers of this mesh
  IMeshBuffer** MeshBuffers;
  int MeshBuffers_size;
  //! The bounding box of this mesh
  F3AABBOX BoundingBox;
} IMesh;
typedef struct triangle3df {
  float3 pointA;
  float3 pointB;
  float3 pointC;
} triangle3df;
//! Enumeration for different types of lights
typedef enum E_LIGHT_TYPE {
  //! point light, it has a position in space and radiates light in all directions
  ELT_POINT,
  //! spot light, it has a position in space, a direction, and a limited cone of influence
  ELT_SPOT,
  //! directional light, coming from a direction from an infinite distance
  ELT_DIRECTIONAL,
  //! Only used for counting the elements of this enum
  ELT_COUNT
} E_LIGHT_TYPE;
//! Names for light types
const c8* const LightTypeNames[] = {
  "Point",
  "Spot",
  "Directional",
  0
};
//! structure for holding data describing a dynamic point light.
/** Irrlicht supports point lights, spot lights, and directional lights.
*/
typedef struct SLight {
  //! Ambient color emitted by the light
  float4 AmbientColor;
  //! Diffuse color emitted by the light.
  /** This is the primary color you want to set. */
  float4 DiffuseColor;
  //! Specular color emitted by the light.
  /** For details how to use specular highlights, see SMaterial::Shininess */
  float4 SpecularColor;
  //! Attenuation factors (constant, linear, quadratic)
  /** Changes the light strength fading over distance.
  Can also be altered by setting the radius, Attenuation will change to
  (0,1.f/radius,0). Can be overridden after radius was set. */
  float3 Attenuation;
  //! The angle of the spot's outer cone. Ignored for other lights.
  f32 OuterCone;
  //! The angle of the spot's inner cone. Ignored for other lights.
  f32 InnerCone;
  //! The light strength's decrease between Outer and Inner cone.
  f32 Falloff;
  //! Read-ONLY! Position of the light.
  /** If Type is ELT_DIRECTIONAL, it is ignored. Changed via light scene node's position. */
  float3 Position;
  //! Read-ONLY! Direction of the light.
  /** If Type is ELT_POINT, it is ignored. Changed via light scene node's rotation. */
  float3 Direction;
  //! Read-ONLY! Radius of light. Everything within this radius will be lighted.
  f32 Radius;
  //! Read-ONLY! Type of the light. Default: ELT_POINT
  E_LIGHT_TYPE Type;
  //! Read-ONLY! Does the light cast shadows?
  bool CastShadows: 1;
} SLight;
void SLight_set(SLight* s)
{
  VSET3_(s->AmbientColor, 0.f, 0.f, 0.f);
  VSET3_(s->DiffuseColor, 1.f, 1.f, 1.f);
  VSET3_(s->SpecularColor, 1.f, 1.f, 1.f);
  VSET3(s->Attenuation, 1.f, 0.f, 0.f);
  s->OuterCone = (45.f), s->InnerCone = (0.f), s->Falloff = (2.f);
  VSET3(s->Position, 0.f, 0.f, 0.f);
  VSET3(s->Direction, 0.f, 0.f, 1.f);
  s->Radius = (100.f), s->Type = (ELT_POINT), s->CastShadows = (true);
}
typedef IRECT recti;
typedef F3PLANE plane3df;
typedef struct IMaterialRenderer {
  int x;
} IMaterialRenderer;
typedef struct IWriteFile {
  int x;
} IWriteFile;
#define IVIDEODRIVER_DEF(TT) \
  TT(bool, beginScene, (IVideoDriver* s, bool backBuffer DEFAULT(true), bool zBuffer DEFAULT(true), SColor color DEFAULT(_ARGB(255, 0, 0, 0)), const SExposedVideoData* videoData DEFAULT(NULL), const IRECT* sourceRect DEFAULT(0) )) \
  TT(bool, endScene, (IVideoDriver* s) ) \
  TT(bool, queryFeature, (IVideoDriver* s, E_VIDEO_DRIVER_FEATURE feature) ) \
  TT(void, disableFeature, (IVideoDriver* s, E_VIDEO_DRIVER_FEATURE feature, bool flag DEFAULT(true)) ) \
  TT(bool, checkDriverReset, (IVideoDriver* s) ) \
  TT(void, setTransform, (IVideoDriver* s, E_TRANSFORMATION_STATE state, const float* mat16) ) \
  TT(void, getTransform, (IVideoDriver* s, E_TRANSFORMATION_STATE state, float* mat16) ) \
  TT(void, setMaterial, (IVideoDriver* s, const SMaterial* material) ) \
  TT(ITexture*, getTextureFile, (IVideoDriver* s, const char* filename) ) \
  TT(ITexture*, getTexture, (IVideoDriver* s, IReadFile* file) ) \
  TT(ITexture*, getTextureIndex, (IVideoDriver* s, u32 index) ) \
  TT(u32, getTextureCount, (IVideoDriver* s) ) \
  TT(void, renameTexture, (IVideoDriver* s, ITexture* texture, const char* newName) ) \
  TT(ITexture*, addTexture, (IVideoDriver* s, const ISIZE* size, const char* name, ECOLOR_FORMAT format DEFAULT(ECF_A8R8G8B8)) ) \
  TT(ITexture*, addTextureByName, (IVideoDriver* s, const char* name, IImage* image, void* mipmapData DEFAULT(0)) ) \
  TT(ITexture*, addRenderTargetTexture, (IVideoDriver* s, ISIZE size, const char* name DEFAULT("rt"), const ECOLOR_FORMAT format DEFAULT(ECF_UNKNOWN)) ) \
  TT(void, removeTexture, (IVideoDriver* s, ITexture* texture) ) \
  TT(void, removeAllTextures, (IVideoDriver* s) ) \
  TT(void, removeHardwareBuffer, (IVideoDriver* s, const IMeshBuffer* mb) ) \
  TT(void, removeAllHardwareBuffers, (IVideoDriver* s) ) \
  TT(void, addOcclusionQuery, (IVideoDriver* s, ISceneNode* node, const IMesh* mesh DEFAULT(0)) ) \
  TT(void, removeOcclusionQuery, (IVideoDriver* s, ISceneNode* node) ) \
  TT(void, removeAllOcclusionQueries, (IVideoDriver* s) ) \
  TT(void, runOcclusionQuery, (IVideoDriver* s, ISceneNode* node, bool visible DEFAULT(false)) ) \
  TT(void, runAllOcclusionQueries, (IVideoDriver* s, bool visible DEFAULT(false)) ) \
  TT(void, updateOcclusionQuery, (IVideoDriver* s, ISceneNode* node, bool block DEFAULT(true)) ) \
  TT(void, updateAllOcclusionQueries, (IVideoDriver* s, bool block DEFAULT(true)) ) \
  TT(u32, getOcclusionQueryResult, (IVideoDriver* s, ISceneNode* node)) \
  TT(void, makeColorKeyTexture, (IVideoDriver* s, ITexture* texture,  SColor color,  bool zeroTexels DEFAULT(false))) \
  TT(void, makeColorKeyTexture1, (IVideoDriver* s, ITexture* texture,  position2di colorKeyPixelPos,  bool zeroTexels DEFAULT(false))) \
  TT(void, makeNormalMapTexture, (IVideoDriver* s, ITexture* texture, f32 amplitude DEFAULT(1.0f))) \
  TT(bool, setRenderTarget, (IVideoDriver* s, ITexture* texture,  bool clearBackBuffer DEFAULT(true), bool clearZBuffer DEFAULT(true),  SColor color DEFAULT(0))) \
  TT(bool, setRenderTarget1, (IVideoDriver* s, E_RENDER_TARGET target, bool clearTarget DEFAULT(true),  bool clearZBuffer DEFAULT(true),  SColor color DEFAULT(0))) \
  TT(bool, setRenderTarget2, (IVideoDriver* s, const IRenderTarget* texture,  bool clearBackBuffer DEFAULT(true), bool clearZBuffer DEFAULT(true),  SColor color DEFAULT(0))) \
  TT(void, setViewPort, (IVideoDriver* s, const IRECT* area)) \
  TT(IRECT, getViewPort, (IVideoDriver* s)) \
  TT(void, drawVertexPrimitiveList, (IVideoDriver* s, const void* vertices, u32 vertexCount,  const void* indexList, u32 primCount,  E_VERTEX_TYPE vType DEFAULT(EVT_STANDARD),  E_PRIMITIVE_TYPE pType DEFAULT(EPT_TRIANGLES),  E_INDEX_TYPE iType DEFAULT(EIT_16BIT))) \
  TT(void, draw2DVertexPrimitiveList, (IVideoDriver* s, const void* vertices, u32 vertexCount,  const void* indexList, u32 primCount,  E_VERTEX_TYPE vType DEFAULT(EVT_STANDARD),  E_PRIMITIVE_TYPE pType DEFAULT(EPT_TRIANGLES),  E_INDEX_TYPE iType DEFAULT(EIT_16BIT))) \
  TT(void, draw3DLine, (IVideoDriver* s, const float3* start, const float3* end, SColor color DEFAULT(_ARGB(255, 255, 255, 255)))) \
  TT(void, draw3DTriangle, (IVideoDriver* s, const triangle3df* triangle,  SColor color DEFAULT(_ARGB(255, 255, 255, 255)))) \
  TT(void, draw3DBox, (IVideoDriver* s, const F3AABBOX* box, SColor color DEFAULT(_ARGB(255, 255, 255, 255)))) \
  TT(void, draw2DImage1, (IVideoDriver* s, const ITexture* texture, const position2di* destPos)) \
  TT(void, draw2DImage, (IVideoDriver* s, const ITexture* texture, position2di destPos, const IRECT* sourceRect, const IRECT* clipRect DEFAULT(0), SColor color DEFAULT(_ARGB(255, 255, 255, 255)), bool useAlphaChannelOfTexture DEFAULT(false))) \
  TT(void, draw2DImage2, (IVideoDriver* s, const ITexture* texture, const IRECT* destRect,  const IRECT* sourceRect, const IRECT* clipRect DEFAULT(0),  const SColor* const colors DEFAULT(0), bool useAlphaChannelOfTexture DEFAULT(false))) \
  TT(void, draw2DRectangle, (IVideoDriver* s, SColor color, const IRECT* pos,  const IRECT* clip DEFAULT(0))) \
  TT(void, draw2DRectangle1, (IVideoDriver* s, const IRECT* pos,  SColor colorLeftUp, SColor colorRightUp,  SColor colorLeftDown, SColor colorRightDown,  const IRECT* clip DEFAULT(0))) \
  TT(void, draw2DRectangleOutline, (IVideoDriver* s, const recti* pos,  SColor color DEFAULT(_ARGB(255, 255, 255, 255)))) \
  TT(void, draw2DLine, (IVideoDriver* s, position2di start,  position2di end,  SColor color DEFAULT(_ARGB(255, 255, 255, 255)))) \
  TT(void, drawPixel, (IVideoDriver* s, u32 x, u32 y, const SColor* color)) \
  TT(void, draw2DPolygon, (IVideoDriver* s, position2di center,  f32 radius,  SColor color DEFAULT(_ARGB(100, 255, 255, 255)),  s32 vertexCount DEFAULT(10))) \
  TT(void, drawStencilShadowVolume, (IVideoDriver* s, const float3* triangles, int triangles_size, bool zfail DEFAULT(true), u32 debugDataVisible DEFAULT(0))) \
  TT(void, drawStencilShadow, (IVideoDriver* s, bool clearStencilBuffer DEFAULT(false),  SColor leftUpEdge DEFAULT(_ARGB(255, 0, 0, 0)),  SColor rightUpEdge DEFAULT(_ARGB(255, 0, 0, 0)),  SColor leftDownEdge DEFAULT(_ARGB(255, 0, 0, 0)),  SColor rightDownEdge DEFAULT(_ARGB(255, 0, 0, 0)))) \
  TT(void, drawMeshBuffer, (IVideoDriver* s, const IMeshBuffer* mb)) \
  TT(void, drawMeshBufferNormals, (IVideoDriver* s, const IMeshBuffer* mb, f32 length DEFAULT(10.f), SColor color DEFAULT(0xffffffff))) \
  TT(void, setFog, (IVideoDriver* s, SColor color DEFAULT(_ARGB(0, 255, 255, 255)),  E_FOG_TYPE fogType DEFAULT(EFT_FOG_LINEAR),  f32 start DEFAULT(50.0f), f32 end DEFAULT(100.0f), f32 density DEFAULT(0.01f),  bool pixelFog DEFAULT(false), bool rangeFog DEFAULT(false))) \
  TT(void, getFog, (IVideoDriver* s, SColor* color, E_FOG_TYPE* fogType,  f32* start, f32* end, f32* density,  bool* pixelFog, bool* rangeFog)) \
  TT(ECOLOR_FORMAT, getColorFormat, (IVideoDriver* s)) \
  TT(ISIZE, getScreenSize, (IVideoDriver* s)) \
  TT(ISIZE, getCurrentRenderTargetSize, (IVideoDriver* s)) \
  TT(u32, getPrimitiveCountDrawn, (IVideoDriver* s, u32 mode DEFAULT(0))) \
  TT(void, deleteAllDynamicLights, (IVideoDriver* s)) \
  TT(u32, getMaximalDynamicLightAmount, (IVideoDriver* s)) \
  TT(u32, getDynamicLightCount, (IVideoDriver* s)) \
  TT(const SLight*, getDynamicLight, (IVideoDriver* s, u32 idx)) \
  TT(void, turnLightOn, (IVideoDriver* s, s32 lightIndex, bool turnOn)) \
  TT(const char*, getName, (IVideoDriver* s)) \
  TT(u32, getMaximalPrimitiveCount, (IVideoDriver* s)) \
  TT(void, setTextureCreationFlag, (IVideoDriver* s, E_TEXTURE_CREATION_FLAG flag, bool enabled DEFAULT(true))) \
  TT(bool, getTextureCreationFlag, (IVideoDriver* s, E_TEXTURE_CREATION_FLAG flag)) \
  TT(IImage*, createImageFromFile, (IVideoDriver* s, const char* filename)) \
  TT(IImage*, createImageFromFile1, (IVideoDriver* s, IReadFile* file)) \
  TT(bool, writeImageToFile, (IVideoDriver* s, IImage* image, const char* filename, u32 param DEFAULT(0))) \
  TT(bool, writeImageToFile1, (IVideoDriver* s, IImage* image, IWriteFile* file, u32 param DEFAULT(0))) \
  TT(IImage*, createImageFromData, (IVideoDriver* s, ECOLOR_FORMAT format,  ISIZE size, void* data)) \
  TT(IImage*, createImageFromData1, (IVideoDriver* s, ECOLOR_FORMAT format,  ISIZE size)) \
  TT(IImage*, createImageFromImage, (IVideoDriver* s, ECOLOR_FORMAT format, IImage* imageToCopy)) \
  TT(IImage*, createImageCopy, (IVideoDriver* s, IImage* imageToCopy,  position2di pos,  ISIZE size)) \
  TT(IImage*, createImageFromTexture, (IVideoDriver* s, ITexture* texture,  position2di pos,  ISIZE size)) \
  TT(void, OnResize, (IVideoDriver* s, ISIZE size)) \
  TT(s32, addMaterialRenderer, (IVideoDriver* s, IMaterialRenderer* renderer, const c8* name DEFAULT(0))) \
  TT(IMaterialRenderer*, getMaterialRenderer, (IVideoDriver* s, u32 idx)) \
  TT(u32, getMaterialRendererCount, (IVideoDriver* s)) \
  TT(const c8*, getMaterialRendererName, (IVideoDriver* s, u32 idx)) \
  TT(void, setMaterialRendererName, (IVideoDriver* s, s32 idx, const c8* name)) \
  TT(const SExposedVideoData*, getExposedVideoData, (IVideoDriver* s)) \
  TT(E_DRIVER_TYPE, getDriverType, (IVideoDriver* s)) \
  TT(IGPUProgrammingServices*, getGPUProgrammingServices, (IVideoDriver* s)) \
  TT(IMeshManipulator*, getMeshManipulator, (IVideoDriver* s)) \
  TT(void, clearZBuffer, (IVideoDriver* s)) \
  TT(IImage*, createScreenShot, (IVideoDriver* s, ECOLOR_FORMAT format DEFAULT(ECF_UNKNOWN), E_RENDER_TARGET target DEFAULT(ERT_FRAME_BUFFER))) \
  TT(ITexture*, findTexture, (IVideoDriver* s, const char* filename)) \
  TT(bool, setClipPlane, (IVideoDriver* s, u32 index, const plane3df* plane, bool enable DEFAULT(false))) \
  TT(void, enableClipPlane, (IVideoDriver* s, u32 index, bool enable)) \
  TT(void, setMinHardwareBufferVertexCount, (IVideoDriver* s, u32 count)) \
  TT(SOverrideMaterial*, getOverrideMaterial, (IVideoDriver* s)) \
  TT(SMaterial*, getMaterial2D, (IVideoDriver* s))
#if 0
TT(void, enableMaterial2D, (bool enable DEFAULT(true))) \
TT(char* getVendorInfo, ()) \
TT(void, setAmbientLight, (SColorf color)) \
TT(void, setAllowZWriteOnTransparent, (bool flag)) \
TT(ISIZE, getMaxTextureSize, ()) \
TT(void, convertColor, (const void* sP, ECOLOR_FORMAT sF, s32 sN, void* dP, ECOLOR_FORMAT dF))
#endif
typedef struct IImageLoader {
  int x;
} IImageLoader;
typedef struct IImageWriter {
  int x;
} IImageWriter;
typedef struct SAttributeReadWriteOptions {
  int x;
} SAttributeReadWriteOptions;
typedef struct IGPUProgrammingServices {
  int x;
} IGPUProgrammingServices;
typedef struct IMeshManipulator {
  int x;
} IMeshManipulator;
typedef struct CFPSCounter {
  s32 FPS;
  u32 Primitive;
  u32 StartTime;
  u32 FramesCounted;
  u32 PrimitivesCounted;
  u32 PrimitiveAverage;
  u32 PrimitiveTotal;
} CFPSCounter;
typedef struct SMaterialRenderer SMaterialRenderer;
struct SMaterialRenderer {
  char Name[32];
  IMaterialRenderer* Renderer;
};
typedef struct SHWBufferLink SHWBufferLink;
struct SHWBufferLink {
  IMeshBuffer* MeshBuffer;
  u32 ChangedID_Vertex;
  u32 ChangedID_Index;
  u32 LastUsed;
  E_HARDWARE_MAPPING Mapped_Vertex;
  E_HARDWARE_MAPPING Mapped_Index;
};
typedef struct SOccQuery SOccQuery;
struct SOccQuery {
  ISceneNode* Node;
  const IMesh* Mesh;
  union {
    void* PID;
    unsigned int UID;
  };
  u32 Result;
  u32 Run;
};
typedef signed short TZBufferType;
typedef struct S2DVertex {
  IPOINT Pos;  // position
  IPOINT TCoords;  // texture coordinates
  TZBufferType ZValue;    // zvalue
  u16 Color;
} S2DVertex;
typedef struct IVideoDriver IVideoDriver;
struct IVideoDriver {
  SOccQuery* OcclusionQueries;
  int OcclusionQueries_size;
  SLight* Lights;
#if 0
  SMaterialRenderer* MaterialRenderers;
  //SHWBufferLink*> HWBufferLinks;
  map< const IMeshBuffer* , SHWBufferLink* > HWBufferMap;
  IFileSystem* FileSystem;
#endif
  ITexture* Textures;
  //! mesh manipulator
  IMeshManipulator* MeshManipulator;
  IRECT ViewPort;
  ISIZE ScreenSize;
  CFPSCounter FPSCounter;
  u32 PrimitivesDrawn;
  u32 MinVertexCountForVBO;
  u32 TextureCreationFlags;
  f32 FogStart;
  f32 FogEnd;
  f32 FogDensity;
  SColor FogColor;
  SExposedVideoData ExposedData;
  SOverrideMaterial OverrideMaterial;
  SMaterial OverrideMaterial2D;
  SMaterial InitMaterial2D;
  bool OverrideMaterial2DEnabled;
  E_FOG_TYPE FogType;
  bool PixelFog;
  bool RangeFog;
  bool AllowZWriteOnTransparent;
  bool FeatureEnabled[EVDF_COUNT];
  
  // for CSoftwareDriver
  void* WindowId;
  IRECT* SceneSourceRect;
  S2DVertex* TransformedPoints;
  IImage RenderTargetSurface[1];
  IImage ZBuffer[1];
  IPOINT Render2DTranslation;
  ISIZE RenderTargetSize;
  ISIZE ViewPortSize;
  float16 TransformationMatrix[ETS_COUNT];
  //ITriangleRenderer* CurrentTriangleRenderer;
  //ITriangleRenderer* TriangleRenderers[ETR_COUNT];
  //ETriangleRenderer CurrentRenderer;
  ITexture* Texture;
  SMaterial Material;

  IVIDEODRIVER_DEF(VFUNDEF)
};
#define IFILESYSTEM_DEF(TT) \
  TT(IReadFile*, createAndOpenFile, (const char* filename)) \
  TT(bool, addFileArchive, (IFileArchive* archive)) \
  TT(bool, removeFileArchive, (const char* filename)) \
  TT(IReadFile*, createMemoryReadFile, (void* memory, s32 len, const char* fileName))
typedef struct IFileSystem {
  int x;
} IFileSystem;
#define IGUIENVIRONMENT_DEF(TT) \
  TT(void, drawAll, ()) \
  TT(bool, setFocus, (IGUIElement* element)) \
  TT(bool, postEventFromUser, (const SEvent* event)) \
  TT(IGUIElement*, addStaticText, (struct IGUIEnvironment* s, const char* text, IRECT rectangle, bool border, bool wordWrap, IGUIElement* parent, s32 id, bool drawBackground) )
typedef struct IGUIEnvironment {
  IGUIENVIRONMENT_DEF(VFUNDEF)
} IGUIEnvironment;
//! Enumeration for render passes.
/** A parameter passed to the registerNodeForRendering() method of the ISceneManager,
specifying when the node wants to be drawn in relation to the other nodes. */
typedef enum E_SCENE_NODE_RENDER_PASS {
  //! No pass currently active
  ESNRP_NONE = 0,
  //! Camera pass. The active view is set up here. The very first pass.
  ESNRP_CAMERA = 1,
  //! In this pass, lights are transformed into camera space and added to the driver
  ESNRP_LIGHT = 2,
  //! This is used for sky boxes.
  ESNRP_SKY_BOX = 4,
  //! All normal objects can use this for registering themselves.
  /** This value will never be returned by
  ISceneManager::getSceneNodeRenderPass(). The scene manager
  will determine by itself if an object is transparent or solid
  and register the object as SNRT_TRANSPARENT or SNRT_SOLD
  automatically if you call registerNodeForRendering with this
  value (which is default). Note that it will register the node
  only as ONE type. If your scene node has both solid and
  transparent material types register it twice (one time as
  SNRT_SOLID, the other time as SNRT_TRANSPARENT) and in the
  render() method call getSceneNodeRenderPass() to find out the
  current render pass and render only the corresponding parts of
  the node. */
  ESNRP_AUTOMATIC = 24,
  //! Solid scene nodes or special scene nodes without materials.
  ESNRP_SOLID = 8,
  //! Transparent scene nodes, drawn after solid nodes. They are sorted from back to front and drawn in that order.
  ESNRP_TRANSPARENT = 16,
  //! Transparent effect scene nodes, drawn after Transparent nodes. They are sorted from back to front and drawn in that order.
  ESNRP_TRANSPARENT_EFFECT = 32,
  //! Drawn after the solid nodes, before the transparent nodes, the time for drawing shadow volumes
  ESNRP_SHADOW = 64
} E_SCENE_NODE_RENDER_PASS;
#define IANIMATEDMESH_DEF(TT) \
  TT(u32, getFrameCount, ()) \
  TT(f32, getAnimationSpeed, ()) \
  TT(void, setAnimationSpeed, (f32 fps)) \
  TT(IMesh*, getMesh, (s32 frame, s32 detailLevel DEFAULT(255), s32 startFrameLoop DEFAULT(-1), s32 endFrameLoop DEFAULT(-1)))
typedef struct IAnimatedMesh {
  IANIMATEDMESH_DEF(VFUNDEF)
} IAnimatedMesh;
#define IMESHCACHE_DEF(TT) \
  TT(void, addMesh, (const char* name, IAnimatedMesh* mesh)) \
  TT(void, removeMesh, (const IMesh* const mesh)) \
  TT(u32, getMeshCount, ()) \
  TT(s32, getMeshIndex, (const IMesh* const mesh)) \
  TT(IAnimatedMesh*, getMeshByIndex, (u32 index))
typedef struct IMeshCache {
  IMESHCACHE_DEF(VFUNDEF)
} IMeshCache;
//! A single sprite frame.
typedef struct SGUISpriteFrame {
  u32 textureNumber;
  u32 rectNumber;
} SGUISpriteFrame;
//! A sprite composed of several frames.
typedef struct SGUISprite {
  SGUISpriteFrame* Frames;
  u32 frameTime;
} SGUISprite;
//! Sprite bank interface.
/** See http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=25742&highlight=spritebank
* for more information how to use the spritebank.
*/
typedef struct IGUISpriteBank IGUISpriteBank;
struct IGUISpriteBank {
  VFUNDEF(int, getPositions, (IRECT* prc))
  VFUNDEF(SGUISprite*, getSprites, ())
  VFUNDEF(u32, getTextureCount, ())
  VFUNDEF(ITexture*, getTexture, (u32 index))
  VFUNDEF(void, addTexture, (ITexture* texture))
  VFUNDEF(void, setTexture, (u32 index, ITexture* texture))
  VFUNDEF(s32, addTextureAsSprite, (ITexture* texture))
  VFUNDEF(void, clear, ())
  VFUNDEF(void, draw2DSprite, (u32 index, const position2di pos, const IRECT* clip DEFAULT(0), SColor color DEFAULT(SColor(255, 255, 255, 255)),
      u32 starttime DEFAULT(0), u32 currenttime DEFAULT(0), bool loop DEFAULT(true), bool center DEFAULT(false)))
  //! Draws a sprite batch in 2d using an array of positions and a color
  VFUNDEF(void, draw2DSpriteBatch, (const u32* indices, const position2di* pos, int npos, const IRECT* clip DEFAULT(0),
      SColor color DEFAULT(SColor(255, 255, 255, 255)), u32 starttime DEFAULT(0), u32 currenttime DEFAULT(0), bool loop DEFAULT(true), bool center DEFAULT(false)))
};
typedef enum ECURSOR_ICON {
  // Following cursors might be system specific, or might use an Irrlicht icon-set. No guarantees so far.
  ECI_NORMAL,   // arrow
  ECI_CROSS,    // Crosshair
  ECI_HAND,     // Hand
  ECI_HELP,   // Arrow and question mark
  ECI_IBEAM,    // typical text-selection cursor
  ECI_NO,     // should not click icon
  ECI_WAIT,     // hourclass
  ECI_SIZEALL,    // arrow in all directions
  ECI_SIZENESW, // resizes in direction north-east or south-west
  ECI_SIZENWSE,   // resizes in direction north-west or south-east
  ECI_SIZENS,   // resizes in direction north or south
  ECI_SIZEWE,   // resizes in direction west or east
  ECI_UP,     // up-arrow
  // Implementer note: Should we add system specific cursors, which use guaranteed the system icons,
  // then I would recommend using a naming scheme like ECI_W32_CROSS, ECI_X11_CROSSHAIR and adding those
  // additionally.
  ECI_COUNT   // maximal of defined cursors. Note that higher values can be created at runtime
} ECURSOR_ICON;
//! Names for ECURSOR_ICON
const c8* const GUICursorIconNames[ECI_COUNT + 1] = {
  "normal",
  "cross",
  "hand",
  "help",
  "ibeam",
  "no",
  "wait",
  "sizeall",
  "sizenesw",
  "sizenwse",
  "sizens",
  "sizewe",
  "sizeup",
  0
};
//! structure used to set sprites as cursors.
typedef struct SCursorSprite {
  IGUISpriteBank* SpriteBank;
  s32 SpriteId;
  position2di HotSpot;
} SCursorSprite;
void SCursorSprite_set(SCursorSprite* s)
{
  s->SpriteBank = (0), s->SpriteId = (-1);
}
void SCursorSprite_set2(SCursorSprite* s, IGUISpriteBank* spriteBank, s32 spriteId, const position2di* hotspot)
{
  s->SpriteBank = (spriteBank), s->SpriteId = (spriteId);
  if (hotspot) {
    s->HotSpot = *(hotspot);
  }
}
typedef struct SVideoMode {
  ISIZE size;
  s32 depth;
} SVideoMode;
//! platform specific behavior flags for the cursor
enum ECURSOR_PLATFORM_BEHAVIOR {
  //! default - no platform specific behavior
  ECPB_NONE = 0,
  //! On X11 try caching cursor updates as XQueryPointer calls can be expensive.
  /** Update cursor positions only when the irrlicht timer has been updated or the timer is stopped.
    This means you usually get one cursor update per device->run() which will be fine in most cases.
    See this forum-thread for a more detailed explanation:
    http://irrlicht.sourceforge.net/forum/viewtopic.php?f=7&t=45525
  */
  ECPB_X11_CACHE_UPDATES = 1
};
#define ISYSTEM_DEF(TT) \
  TT(const char*, getOperatingSystemVersion, ()) \
  TT(bool, isCursorVisible, ()) \
  TT(bool, setCursorVisible, (int visible)) \
  TT(void, setCursorPositioni,(s32 x, s32 y)) \
  TT(IPOINT, getCursorPosition,()) \
  TT(u32, getRealTime, ()) \
  TT(void, copyToClipboard, (const c8* text)) \
  TT(const c8*, getTextFromClipboard, ()) \
  TT(u32, getProcessorSpeedMHz, ()) \
  TT(bool, getSystemMemory, (u32* Total, u32* Avail)) \
  TT(s32, getVideoModeCount, ()) \
  TT(s32, getVideoModeDepth, (s32 modeNumber)) \
  TT(ISIZE, getVideoModeResolution, (s32 modeNumber)) \
  TT(ISIZE, getVideoModeResolution2, (ISIZE minSize, ISIZE maxSize))
#if 0
TT(ECURSOR_ICON, getCursorActiveIcon, ()) \
TT(FPOINT, getCursorRelativePosition, ()) \
TT(void, setReferenceRect, (const IRECT* rect)) \
TT(void, setActiveIcon, (ECURSOR_ICON iconId))
#endif
int getVideoMode(int index, SVideoMode* out);
#if 1
typedef struct ISystem ISystem;
typedef struct IDevice IDevice;
#define TTSET(a, b, c)  a b c;
ISYSTEM_DEF(TTSET)
#undef TTSET
struct ISystem {
  IDevice* Device;
  bool IsCursorVisible;
  ISYSTEM_DEF(VFUNDEF)
};
//ISystem* getSys();
#else
#define TTSET(a, b, c)  a b c;
ISYSTEM_DEF(TTSET)
#undef TTSET
#endif
typedef struct ISceneNode IVolumeLightSceneNode;
typedef struct ISceneNode IMeshSceneNode;
typedef struct ISceneNode ICameraSceneNode;
typedef enum E_JOINT_UPDATE_ON_RENDER {
  //! do nothing
  EJUOR_NONE = 0,
  //! get joints positions from the mesh (for attached nodes, etc)
  EJUOR_READ,
  //! control joint positions in the mesh (eg. ragdolls, or set the animation from animateJoints() )
  EJUOR_CONTROL
} E_JOINT_UPDATE_ON_RENDER;
typedef ISceneNode IAnimatedMeshSceneNode;
#define ISCENEMANAGER_DEF(TT) \
  TT(IAnimatedMesh* , getMesh, (const char* filename)) \
  TT(IAnimatedMesh* , getMesh1, (IReadFile* file)) \
  TT(void, drawAll, (ISceneManager* s)) \
  TT(bool, postEventFromUser, (const SEvent* event)) \
  TT(IMeshCache* , getMeshCache, ()) \
  TT(IVolumeLightSceneNode*, addVolumeLightSceneNode, (ISceneNode* parent, s32 id, u32 subdivU DEFAULT(32), u32 subdivV DEFAULT(32), \
      SColor foot DEFAULT(_ARGB(51, 0, 230, 180)), SColor tail DEFAULT(_ARGB(0, 0, 0, 0)), \
      const float* position DEFAULT(0), const float* rotation DEFAULT(0), const float* scale DEFAULT(1.0f)) ) \
  TT(IMeshSceneNode*, addCubeSceneNode, (f32 size DEFAULT(10.0f), ISceneNode* parent DEFAULT(0), s32 id DEFAULT(-1), \
      const float* position DEFAULT(0), const float* rotation DEFAULT(0), const float* scale DEFAULT(0) )) \
  TT(IMeshSceneNode*, addSphereSceneNode, (f32 radius DEFAULT(5.0f), s32 polyCount DEFAULT(16), ISceneNode* parent DEFAULT(0), s32 id DEFAULT(-1), \
      const F3VECTOR position DEFAULT(0), const F3VECTOR rotation DEFAULT(0), const F3VECTOR scale DEFAULT(1.0f)) ) \
  TT(IAnimatedMeshSceneNode*, addAnimatedMeshSceneNode, (IAnimatedMesh* mesh,ISceneNode* parent DEFAULT(0), s32 id DEFAULT(-1),const F3VECTOR position DEFAULT(f3VECTOR(0, 0, 0)), \
      const F3VECTOR rotation DEFAULT(f3VECTOR(0, 0, 0)),const F3VECTOR scale DEFAULT(f3VECTOR(1.0f, 1.0f, 1.0f)), bool alsoAddIfMeshPointerZero DEFAULT(false)) ) \
  TT(ICameraSceneNode*, addCameraSceneNode,(ISceneManager* s, ISceneNode* parent DEFAULT(0), const F3VECTOR position DEFAULT(f3VECTOR(0, 0, 0)), \
      const F3VECTOR lookat DEFAULT(f3VECTOR(0, 0, 100)), s32 id DEFAULT(-1), bool makeActive DEFAULT(true)) )
struct ISceneManager {
  ISCENEMANAGER_DEF(VFUNDEF)
};
typedef struct IOSOperator {
  int x;
} IOSOperator;
typedef struct ITimer {
  int x;
} ITimer;
typedef struct SMouseMultiClicks SMouseMultiClicks;
struct SMouseMultiClicks {
  // DoubleClickTime(500), CountSuccessiveClicks(0), LastClickTime(0), LastMouseInputEvent(EMIE_COUNT)
  u32 DoubleClickTime;
  u32 CountSuccessiveClicks;
  u32 LastClickTime;
  position2di LastClick;
  EMOUSE_INPUT_EVENT LastMouseInputEvent;
};
//! The Irrlicht device. You can create it with createDevice() or createDeviceEx().
/** This is the most important class of the Irrlicht Engine. You can
access everything in the engine if you have a pointer to an instance of
this class.  There should be only one instance of this class at any
time.
*/
#define IRRLICHTDEVICE_DEF(TT) \
  TT(bool, run, (IDevice* s)) \
  TT(void, yield, (IDevice* s)) \
  TT(void, sleep, (IDevice* s, u32 timeMs, bool pauseTimer)) \
  TT(void, setWindowCaption, (IDevice* s, const char* text)) \
  TT(bool, isWindowActive, (IDevice* s) ) \
  TT(bool, isWindowFocused, (IDevice* s) ) \
  TT(bool, isWindowMinimized, (IDevice* s) ) \
  TT(void, closeDevice, (IDevice* s)) \
  TT(bool, postEventFromUser, (IDevice* s, const SEvent* event)) \
  TT(void, setResizable, (IDevice* s, bool resize DEFAULT(false))) \
  TT(void, minimizeWindow, (IDevice* s)) \
  TT(void, maximizeWindow, (IDevice* s)) \
  TT(void, restoreWindow, (IDevice* s)) \
  TT(bool, setGammaRamp, (IDevice* s, f32 red, f32 green, f32 blue, f32 relativebrightness, f32 relativecontrast)) \
  TT(bool, getGammaRamp, (IDevice* s, f32* red, f32* green, f32* blue, f32* brightness, f32* contrast)) \
  TT(void, clearSystemMessages, (IDevice* s)) \
  TT(void, update, (IDevice* s))
#if 0
TT(E_DEVICE_TYPE, getType, (IDevice* s))
TT(void, setCursorPositionf, (f32 x, f32 y)) 
TT(void, setEventReceiver, (IDevice* s, IEventReceiver* receiver)) 
TT(void, setInputReceivingSceneManager, (IDevice* s, ISceneManager* sceneManager)) 
#endif
typedef struct IDevice IDevice;
struct IDevice {
  //! Type of the device.
  E_DEVICE_TYPE DeviceType;
  E_DRIVER_TYPE DriverType;
  //! Size of the window or the video mode in fullscreen mode. Default: 800x600
  ISIZE WindowSize;
  //! Minimum Bits per pixel of the color buffer in fullscreen mode. Ignored if windowed mode. Default: 16.
  u8 Bits;
  //! Minimum Bits per pixel of the depth buffer. Default: 16.
  u8 ZBufferBits;
  //! Should be set to true if the device should run in fullscreen.
  /** Otherwise the device runs in windowed mode. Default: false. */
  bool Fullscreen;
  //! Specifies if the stencil buffer should be enabled.
  bool Stencilbuffer;
  //! Specifies vertical syncronisation.
  bool Vsync;
  //! Specifies if the device should use fullscreen anti aliasing
  u8 AntiAlias;
  //! Flag to enable proper sRGB and linear color handling
  bool HandleSRGB;
  //! Whether the main framebuffer uses an alpha channel.
  //Default value: false
  bool WithAlphaChannel;
  //! Whether the main framebuffer uses doublebuffering.
  bool Doublebuffer;
  //! Specifies if the device should ignore input events
  bool IgnoreInput;
  //! Specifies if the device should use stereo buffers
  bool Stereobuffer;
  //! Specifies if the device should use high precision FPU setting
  //Default value: false
  bool HighPrecisionFPU;
  //! Window Id.
  void* WindowId;
  //! Specifies the logging level used in the logging interface.
  ELOG_LEVEL LoggingLevel;
  //! Allows to select which graphic card is used for rendering when more than one card is in the system.
  u32 DisplayAdapter;
  //! Create the driver multithreaded.
  bool DriverMultithreaded;
  //! Enables use of high performance timers on Windows platform.
  bool UsePerformanceTimer;
  //! Don't use or change this parameter.
  c8* SDK_version_do_not_use;
  ECOLOR_FORMAT ColorFormat;
  void* x;
  IVideoDriver* VideoDriver;
  IGUIEnvironment* GUIEnvironment;
  ISceneManager* SceneManager;
  ITimer* Timer;
  IEventReceiver* UserReceiver;
  IRandomizer Randomizer[1];
  IFileSystem* FileSystem;
  ISceneManager* InputReceivingSceneManager;
  SMouseMultiClicks MouseMultiClicks;
  bool Close;
  IDevice* next;
  IDevice* prev;
  IRRLICHTDEVICE_DEF(VFUNDEF)
};
static bool isDriverSupported(E_DRIVER_TYPE driver)
{
  switch (driver) {
  case EDT_NULL:
      return true;
  case EDT_SOFTWARE:
#ifdef _IRR_COMPILE_WITH_SOFTWARE_
      return true;
#else
      return false;
#endif
  case EDT_BURNINGSVIDEO:
#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
      return true;
#else
      return false;
#endif
  case EDT_DIRECT3D8:
#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
      return true;
#else
      return false;
#endif
  case EDT_DIRECT3D9:
#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
      return true;
#else
      return false;
#endif
  case EDT_OPENGL:
#ifdef _IRR_COMPILE_WITH_OPENGL_
      return true;
#else
      return false;
#endif
  default:
      return false;
  }
}
#endif // __IRR_TYPES_H_INCLUDED__

