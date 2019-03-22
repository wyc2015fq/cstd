#ifndef _CMATH_INL_
#define _CMATH_INL_
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "algo.h"
#include "math_def.txt"
// Some fixes for MS Visual C++
#if defined(_MSC_VER)
#pragma warning(disable:4786) // Identifier was truncated...
#endif
#ifndef INLINE
#if defined(_MSC_VER)
#define INLINE __forceinline
#else
#define INLINE inline
#endif
#endif // INLINE
#define rect_init(_R, X1_, Y1_, X2_, Y2_) ((_R)->x1 = X1_, (_R)->y1 = Y1_, (_R)->x2 = X2_, (_R)->y2 = Y2_)
#define IFSWAP(OP, _A, _B, t)  {if((_A) OP (_B)) { SWAP(_A, _B, t); }}
#define rect_normalize(_R, t) { IFSWAP(>, (_R)->x1, (_R)->x2);IFSWAP(>, (_R)->y1, (_R)->y2); }
#define rect_clip(_R, X1_, Y1_, X2_, Y2_)  ((_R)->x2 = MIN((_R)->x2, X2_),(_R)->y2 = MIN((_R)->y2, Y2_),(_R)->x1 = MAX((_R)->x1, X1_),(_R)->y1 = MAX((_R)->y1, Y1_),(x1 <= x2 && y1 <= y2))
#define rect_valid(_R)  ((_R)->x1 <= (_R)->x2 && (_R)->y1 <= (_R)->y2)
#define rect_hit_test(x, y)  (x >= (_R)->x1 && x <= (_R)->x2 && y >= (_R)->y1 && y <= (_R)->y2)



#ifndef FLT_MAX
#define FLT_MAX 3.402823466E+38F
#endif
#ifndef FLT_MIN
#define FLT_MIN 1.17549435e-38F
#endif

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
#define AIR(x)        (F32_AS_U32(x)&0x7fffffff)

//! Rounding error constant often used when comparing f32 values.
#define ROUNDING_ERROR_S32  0
#define ROUNDING_ERROR_S64  0
#define ROUNDING_ERROR_f32  0.000001f
#define ROUNDING_ERROR_f64  0.00000001
#define ISZERO_F32(X)   ((X)>ROUNDING_ERROR_f32 && (-(X))<(ROUNDING_ERROR_f32))
#define EQUALS_(a, b, tolerance)    (((a) + tolerance >= (b)) && ((a) - tolerance <= (b)))
#define EQUALS_F32(a, b) EQUALS_(a, b, ROUNDING_ERROR_f32)
//! Constant for PI.
#define  PI32     3.14159265359f
//! Constant for reciprocal of PI.
#define RECIPROCAL_PI  (1.0f / PI)
//! Constant for half of PI.
#define HALF_PI  (PI / 2.0f)
#ifdef PI64 // make sure we don't collide with a define
#undef PI64
#endif
//! Constant for 64bit PI.
#define PI64    (3.1415926535897932384626433832795028841971693993751)
//! Constant for 64bit reciprocal of PI.
#define RECIPROCAL_PI64  (1.0 / PI64)
//! 32bit Constant for converting from degrees to radians
#define DEGTORAD    (PI / 180.0f)
//! 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
#define RADTODEG    (180.0f / PI)
//! 64bit constant for converting from degrees to radians (formally known as GRAD_PI2)
#define DEGTORAD64  (PI64 / 180.0)
//! 64bit constant for converting from radians to degrees
#define RADTODEG64  (180.0 / PI64)
//! Utility function to convert a radian value to degrees
#ifndef ROUND
#define ROUND(x)  (int)(x+0.5)
#endif // round
#define Q_UINT64_C(x)  (uint64)(x)
#ifndef M_PI
#define M_PI 3.141592653589793
#endif
#ifndef M_PI_2
#define M_PI_2 (M_PI*2)
#endif // M_PI_2
//?????????????Ч?????NaN???????
#define ISNAN(x)    ((x) != (x))
static int dayofweek(int y, int m, int d)  /* 0 = Sunday */ 
{ 
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; 
  y -= m < 3; 
  return (y + y/4 - y/100 + y/400 + t[m - 1] + d) % 7; 
}
static char* ToHexString_f32(float t, char* s, int l)
{
  union {
    float f;
    unsigned int i;
  } u;
  if (IsNaN_f32(t)) {
    strncpy(s, "fff80000", l);
    return s;
  }
  u.f = t;
  _snprintf(s, l, "%08x", u.i);
  return s;
}
static char* ToHexString_f64(double t, char* s, int l)
{
  union {
    double f;
    unsigned int i[2];
  } u;
  if (IsNaN_f64(t)) {
    strncpy(s, "fff8000000000000", l);
    return s;
  }
  u.f = t;
  if ('b'!=ENDIANNESS) {
    _snprintf(s, l, "%08x%08x", u.i[1], u.i[0]);
  }
  else {
    _snprintf(s, l, "%08x%08x", u.i[0], u.i[1]);
  }
  return s;
}

static double NaN() {
  union {
    float f;
    unsigned int i;
  } u;
  u.i = 0x7fC00000;
  return u.f;
}

static double Inf() {
  union {
    float f;
    unsigned int i;
  } u;
  u.i = 0x7f800000;
  return u.f;
}

#ifdef _MSC_VER
#if _MSC_VER < 1300
static double rint(double x)
{
  __asm {
    fld x
    frndint
    fstp x
  };
  return x;
}
static float rintf(float x)
{
	return (float)rint(x);
}
//VC doesn't define log1p so we borrow the definition from libm (mingw)
#if 0
static double log1p(double x)
{
  const double limit = 0.29;
  const double one = 1.0;
  __asm {
    /* BASED ON log1p.S from mingw
     * Written by J.T. Conklin <jtc@netbsd.org>.
     * Public domain.
     * Removed header file dependency for use in libmingwex.a by
     *   Danny Smith <dannysmith@users.sourceforge.net>
     */
    /* The fyl2xp1 can only be used for values in
       -1 + sqrt(2) / 2 <= x <= 1 - sqrt(2) / 2
       0.29 is a safe value.
     */
    fldln2
    fld     x //  [esp+4]
    fxam
    fnstsw  ax
    fld        st
    sahf
    jc        l3        // in case x is NaN or ?Inf
    l4:
    fabs
    fcomp        limit
    fnstsw  ax
    sahf
    jc        l2
    fadd        one
    fyl2x
    fstp x
    jmp l5
    l2:
    fyl2xp1
    fstp x
    jmp l5
    l3:
    jp        l4        // in case x is ?Inf
    fstp        st(1)
    fstp        st(1)
    l5:
  };
  return x;
}
#else
static double log1p(const double x) {
  if (fabs(x) > 1e-4) {  
    // x is large enough that the obvious evaluation is OK  
    return log(1.0 + x);  
  }  
  // Use Taylor approx. log(1 + x) = x - x^2/2 with error roughly x^3/3  
  // Since |x| < 10^-4, |x|^3 < 10^-12, relative error less than 10^-8  
  //return (-0.5 * x + 1.0) * x;
  return (x * (x * (2 * x - 3) + 6)) / 6;  
}
#endif // 0
#endif
CC_INLINE double gsl_log1p (const double x)  
{
  volatile double y, z;  
  y = 1 + x;  
  z = y - 1;  
  return log(y) - (z - x) / y ;  /* cancels errors with IEEE arithmetic */  
}
const long double LOGE2L  = 6.9314718055994530941723E-1L;
const long double LOG2EL  = 1.4426950408889634073599E0L;

#undef M_LN2
#undef GSL_DBL_EPSILON
#define GSL_DBL_EPSILON        2.2204460492503131e-16
#define M_LN2      0.69314718055994530941723212146      /* ln(2) */  
CC_INLINE double gsl_expm1(const double x)  
{
  /* FIXME: this should be improved */  
  
  if (fabs(x) < M_LN2)  
  {  
    /* Compute the taylor series S = x + (1/2!) x^2 + (1/3!) x^3 + ... */  
    
    double i = 1.0;  
    double sum = x;  
    double term = x / 1.0;  
    
    do  
    {  
      i++ ;  
      term *= x/i;  
      sum += term;  
    }  
    while (fabs(term) > fabs(sum) * GSL_DBL_EPSILON) ;  
    
    return sum ;  
  }  
  else  
  {  
    return exp(x) - 1;  
  }  
}
#else
//static double rint(double x) {  return x>0 ? round(x) : -round(-x);}
#ifdef _MSC_VER
long rint(double x)
{
  return (x >= 0.) ? (long)(x + 0.5) : (long)(x - 0.5);
}
#endif // _MSC_VER
#endif // _MSVER

CC_INLINE int sqrti(int x) {
  return (int)sqrt((double)x);
}

//returns distance to the next nearest double value
CC_INLINE double feps(double x)
{
  if (x >= 0) {
    return nextafter(x , DBL_MAX) - x;
  }
  else {
    return x - nextafter(x, -DBL_MAX);
  }
}
//returns distance to the next nearest float value
CC_INLINE float fepsf(float x)
{
  if (x >= 0) {
    return nextafterf(x , FLT_MAX) - x;
  }
  else {
    return x - nextafterf(x, -FLT_MAX);
  }
}
CC_INLINE int complex_divide(double ar, double ai, double br, double bi, double* c0, double* c1)
{
  double ratio, den;
  double abr, abi, cr;
  if ((ai == 0) && (bi == 0)) {
    *c1 = 0;
    *c0 = ar / br;
    return 0;
  }
  if (bi == 0) {
    *c0 = ar / br;
    *c1 = ai / br;
    return 0;
  }
  if ((ar == 0) && (bi == 0)) {
    *c0 = 0;
    *c1 = ai / br;
    return 0;
  }
  if ((ai == 0) && (br == 0)) {
    *c0 = 0;
    *c1 = -ar / bi;
    return 0;
  }
  //   if (ar == 0) {
  //     c0 = (ai*bi)/(br*br + bi*bi);
  //     c1 = (ai*br)/(br*br + bi*bi);
  //     return 0;
  //   }
  if ((ar == br) && (ai == bi)) {
    *c0 = 1, *c1 = 0;
    return 0;
  }
  if ((abr = br) < 0.) {
    abr = - abr;
  }
  if ((abi = bi) < 0.) {
    abi = - abi;
  }
  if (abr <= abi) {
    if (abi == 0) {
      if (ai != 0 || ar != 0) {
        abi = 1.;
      }
      *c1 = *c0 = abi / abr;
      return 0;
    }
    ratio = br / bi ;
    den = bi * (1 + ratio * ratio);
    cr = (ar * ratio + ai) / den;
    *c1 = (ai * ratio - ar) / den;
  }
  else {
    ratio = bi / br ;
    den = br * (1 + ratio * ratio);
    cr = (ar + ai * ratio) / den;
    *c1 = (ai - ar * ratio) / den;
  }
  *c0 = cr;
  return 0;
}
CC_INLINE int complex_powi(double ar, double ai, int b, double* pr, double* pi)
{
  int n;
  unsigned long u;
  double t;
  double q[2], x[2];
  static double one[2] = {1.0, 0.0};
  n = b;
  q[0] = 1;
  q[1] = 0;
  if (n) {
    if (n < 0) {
      n = -n;
      complex_divide(one[0], one[1], ar, ai, x, x + 1);
    }
    else {
      x[0] = ar;
      x[1] = ai;
    }
    for (u = n; ;) {
      if (u & 01) {
        t = q[0] * x[0] - q[1] * x[1];
        q[1] = q[0] * x[1] + q[1] * x[0];
        q[0] = t;
      }
      if (u >>= 1) {
        t = x[0] * x[0] - x[1] * x[1];
        x[1] = 2 * x[0] * x[1];
        x[0] = t;
      }
      else {
        break;
      }
    }
  }
  *pi = q[1];
  *pr = q[0];
  return 0;
}
CC_INLINE int complex_recip(double ar, double ai, double* cr, double* ci)
{
  if (ai == 0) {
    *ci = 0;
    *cr = 1 / ar;
    return 0;
  }
  complex_divide(1, 0, ar, ai, cr, ci);
  return 0;
}
CC_INLINE double complex_abs(double real, double imag)
{
  double swap, temp;
  if (real < 0) {
    real = -real;
  }
  if (imag < 0) {
    imag = -imag;
  }
  if (imag > real) {
    swap = real;
    real = imag;
    imag = swap;
  }
  if ((real + imag) == real) {
    return (real);
  }
  temp = (imag) / (real);
  temp = real * sqrt(1.0 + temp * temp); /*overflow!!*/
  return ((temp));
}
CC_INLINE double complex_phase(double ar, double ai)
{
  return atan2(ai, ar);
}
CC_INLINE int complex_pow(double ar, double ai, double br, double bi, double* cr, double* ci)
{
  double logr, logi, x, y;
  double mag = complex_abs(ar, ai);
  if (mag == 0) {
    *cr = 0;
    *ci = 0;
    return 0;
  }
  logr = log(mag);
  logi = atan2(ai, ar);
  x = exp(logr * br - logi * bi);
  y = logr * bi + logi * br;
  *cr = x * cos(y);
  *ci = x * sin(y);
  return 0;
}
#define complex_pow2(c, a, b) complex_pow((a)[0], (a)[1], (b)[0], (b)[1], (c), (c)+1)
CC_INLINE int complex_multiply(double ar, double ai, double br, double bi, double* cr, double* ci)
{
  if ((ai == 0) && (bi == 0)) {
    *cr = ar * br;
    *ci = 0;
  }
  else if (ai == 0) {
    *cr = ar * br;
    *ci = ar * bi;
  }
  else if (bi == 0) {
    *cr = br * ar;
    *ci = br * ai;
  }
  else {
    *cr = ar * br - ai * bi;
    *ci = ar * bi + ai * br;
  }
  return 0;
}
CC_INLINE int complex_cmp(double ar, double ai, double br, double bi)
{
  double mag_a, mag_b, pha, phb;
  if ((ar == br) && (ai == bi)) {
    return 0;
  }
  mag_a = complex_abs(ar, ai);
  mag_b = complex_abs(br, bi);
  if ((mag_b - mag_a) > feps(mag_a) * 4) {
    return -1;
  }
  if ((mag_a - mag_b) > feps(mag_b) * 4) {
    return 1;
  }
  pha = complex_phase(ar, ai);
  phb = complex_phase(br, bi);
  return CC_CMP(pha, phb);
}
#define complex_lt(ar, ai, br, bi)  (complex_cmp(ar, ai, br, bi)<0)
#define complex_gt(ar, ai, br, bi)  (complex_cmp(ar, ai, br, bi)>0)
#define complex_le(ar, ai, br, bi)  (complex_cmp(ar, ai, br, bi)<=0)
#define complex_ge(ar, ai, br, bi)  (complex_cmp(ar, ai, br, bi)>=0)
#define complex_eq(ar, ai, br, bi)  (complex_cmp(ar, ai, br, bi)==0)
#define complex_ne(ar, ai, br, bi)  (complex_cmp(ar, ai, br, bi)!=0)
CC_INLINE int complex_log(double xr, double xi, double* yr, double* yi)
{
  *yr = log(complex_abs(xr, xi));
  *yi = atan2(xi, xr);
  return 0;
}
CC_INLINE int complex_exp(double xr, double xi, double* yr, double* yi)
{
  *yr = exp(xr) * cos(xi);
  *yi = exp(xr) * sin(xi);
  return 0;
}
CC_INLINE int complex_sqrt(double xr, double xi, double* yr, double* yi)
{
  double tr, ti;
  if ((xr >= 0) && (xi == 0)) {
    *yr = sqrt(xr);
    *yi = 0;
    return 0;
  }
  if ((xr < 0) && (xi == 0)) {
    *yi = sqrt(-xr);
    *yr = 0;
    return 0;
  }
  complex_log(xr, xi, &tr, &ti);
  tr /= 2.0;
  ti /= 2.0;
  complex_exp(tr, ti, yr, yi);
  return 0;
}
CC_INLINE int complex_square(double xr, double xi, double* yr, double* yi)
{
  *yr = xr * xr - xi * xi;
  *yi = 2 * xr * xi;
  return 0;
}
static const int sin360[] = {
#include "math/table/table_sin360.txt"
};
static const int cos360[] = {
#include "math/table/table_cos360.txt"
};
CC_INLINE int fixatan2(int y, int x)
{
  static const int atan256[] = {
#include "math/table/table_atan256.txt"
  };
  int ret = 0, ax = IABS(x), ay = IABS(y);
  const int aa[] = {0, 90, -90, -180};
  if (0 == x) {
    if (0 == y) {
      return 0;
    }
    return y < 0 ? -90 : 90;
  }
  if (0 == y) {
    return x < 0 ? 180 : 0;
  }
  ret = (ax == ay) ? (ax ? 45 : 0) : ((ax > ay) ? atan256[(ay << 8) / ax] : 90 - atan256[(ax << 8) / ay]);
  if (y * x < 0) {
    ret = (ax == ay) ? (ax ? 45 : 0) : ((ax > ay) ? 90 - atan256[(ay << 8) / ax] : atan256[(ax << 8) / ay]);
  }
  return ret + aa[((y < 0) << 1) | (x < 0)];
}
// 0-255
CC_INLINE int fixatan256(int y, int x) {
  int a = fixatan2(y, x)*256/360;
  return a&255;
}
CC_INLINE double vec_sum_f64(int n, const double* vec)
{
  double sum = 0.0;
  int i = 0;
  for (; i < n-3; i+=4) {
    sum += vec[i]+vec[i+1]+vec[i+2]+vec[i+3];
  }
  for (; i < n; i++) {
    sum += vec[i];
  }
  return sum;
}
CC_INLINE double vec_mean_f64(int n, const double* vec)
{
  double sum = vec_sum_f64(n, vec);
  return sum / n;
}
CC_INLINE double vec_varianceM_f64(int n, const double* vec, double mean)
{
  double variance = 0.0, x;
  int i;
  for (i = 0; i < n; i++) {
    x = vec[i] - mean;
    variance += x * x;
  }
  
  // Sample variance
  return variance / (n - 1);
}
CC_INLINE double vec_variance_f64(int n, const double* vec)
{
  double mean = vec_mean_f64(n, vec);
  return vec_varianceM_f64(n, vec, mean);
}
// sqrt(x*x + y*y)
#define MAG2(x, y)  ((x)*(x)+(y)*(y))
// sqrt(x*x + y*y)
CC_INLINE int fixmag(int x, int y)
{
  static const int mag256[] = {
#include "math/table/table_mag256.txt"
  };
  int t = 180;
  x = IABS(x), y = IABS(y);
  if (x != y) {
    t = mag256[(MIN(x, y) << 8) / MAX(x, y)];
  }
  return (int)((((int64)t) * (x + y)) >> 8);
}
//icross_product
CC_INLINE int icross_product(int x1, int y1, int x2, int y2, int x, int y)
{
  return (x - x2) * (y2 - y1) - (y - y2) * (x2 - x1);
}
//point_in_triangle
CC_INLINE BOOL ipoint_in_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{
  BOOL cp1 = icross_product(x1, y1, x2, y2, x, y) < 0;
  BOOL cp2 = icross_product(x2, y2, x3, y3, x, y) < 0;
  BOOL cp3 = icross_product(x3, y3, x1, y1, x, y) < 0;
  return cp1 == cp2 && cp2 == cp3 && cp3 == cp1;
}
//?ж??q?????????????
CC_INLINE BOOL ipoint_in_poly(const IPOINT* pt, int len, int x, int y)
{
  int i, j, k = 0;
  int Xcoss;
  for (i = 0; i < len; i++) {
    j = (i == len - 1) ? 0 : (i + 1);
    if ((y < pt[i].y && y >= pt[j].y) || (y >= pt[i].y && y < pt[j].y)) {
      Xcoss = (y - pt[i].y) * (pt[i].x - pt[j].x) / (pt[i].y - pt[j].y) + pt[i].x;
      if (x <= Xcoss) {
        k++;
      }
    }
  }
  return k % 2;
}
CC_INLINE int ipoly_offset(IPOINT* pt, int len, int x, int y)
{
  int i;
  for (i = 0; i < len; i++) {
    pt[i].x += x;
    pt[i].y += y;
  }
  return 0;
}
CC_INLINE int ipoly_scale(IPOINT* pt, int len, int sc)
{
  int i;
  for (i = 0; i < len; i++) {
    pt[i].x *= sc;
    pt[i].y *= sc;
  }
  return 0;
}
// sqrt(X * X + Y * Y) err = 3.5%
CC_INLINE int FastDistance2D(int x, int y)
{
  int mn;
  x = IABS(x);
  y = IABS(y);
  mn = IMIN(x, y);
  return (x + y - (mn >> 1) - (mn >> 2) + (mn >> 4));
}
CC_INLINE int FastDistance2D22(int x, int y)
{
  return (int)(sqrt((double)(x * x + y * y)) + 0.5);
}
//calc_distance
CC_INLINE double calc_distance(double x1, double y1, double x2, double y2)
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  return sqrt(dx*dx + dy*dy);
}
CC_INLINE int calc_distance_i(int x1, int y1, int x2, int y2)
{
  //return (int)sqrt(((double)x2 - x1) * (x2 - x1) + ((double)y2 - y1)*(y2 - y1));
  return FastDistance2D(x2 - x1, y2 - y1);
}
#if 0
//calc_sq_distance
CC_INLINE int calc_sq_distance(int x1, int y1, int x2, int y2)
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  return dx * dx + dy * dy;
}
#define intersection_epsilon 1
#define vertex_dist_epsilon 1
//calc_line_point_distance
CC_INLINE int calc_line_point_distance(int x1, int y1, int x2, int y2, int x, int y)
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int d = FastDistance2D(dx, dy);
  if (d < vertex_dist_epsilon) {
    return calc_distance(x1, y1, x, y);
  }
  return ((x - x2) * dy - (y - y2) * dx) / d;
}
//calc_line_point_u
CC_INLINE int calc_segment_point_u(int x1, int y1,    int x2, int y2,    int x, int y)
{
  int pdx, pdy, dx = x2 - x1, dy = y2 - y1;
  if (dx == 0 && dy == 0) {
    return 0;
  }
  pdx = x - x1;
  pdy = y - y1;
  return (pdx * dx + pdy * dy) / (dx * dx + dy * dy);
}
//calc_line_point_sq_distance
CC_INLINE int calc_segment_point_sq_distance(int x1, int y1, int x2, int y2, int x, int y, int u)
{
  if (u <= 0) {
    return calc_sq_distance(x, y, x1, y1);
  }
  else if (u >= 1) {
    return calc_sq_distance(x, y, x2, y2);
  }
  return calc_sq_distance(x, y, x1 + u * (x2 - x1), y1 + u * (y2 - y1));
}
//calc_line_point_sq_distance
CC_INLINE int calc_segment_point_sq_distance1(int x1, int y1,    int x2, int y2,    int x, int y)
{
  return calc_segment_point_sq_distance(x1, y1, x2, y2, x, y, calc_segment_point_u(x1, y1, x2, y2, x, y));
}
//calc_intersection
CC_INLINE BOOL calc_intersection(int ax, int ay, int bx, int by,
    int cx, int cy, int dx, int dy, int* x, int* y)
{
  int64 r;
  int64 num = (int64)(ay - cy) * (dx - cx) - (ax - cx) * (dy - cy);
  int64 den = (int64)(bx - ax) * (dy - cy) - (by - ay) * (dx - cx);
  if (0 == den) {
    return FALSE;
  }
  r = num / den;
  *x = (int)(ax + r * (bx - ax));
  *y = (int)(ay + r * (by - ay));
  return TRUE;
}
CC_INLINE int area_of_polygon(int n, IPOINT* pt)
{
  int i;
  int64 s;
  if (n < 3) {
    return 0;
  }
  s = (int64)pt[0].y * (pt[n - 1].x - pt[1].x);
  for (i = 1; i < n; i++) {
    s += (int64)pt[i].y * (pt[(i - 1)].x - pt[(i + 1) % n].x);
  }
  return (int)(s >> 16);
}
//intersection_exists
CC_INLINE BOOL intersection_exists(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
  // It's less expensive but you can't control the
  // boundary conditions: Less or LessEqual
  int dx1 = x2 - x1;
  int dy1 = y2 - y1;
  int dx2 = x4 - x3;
  int dy2 = y4 - y3;
  return ((x3 - x2) * dy1 - (y3 - y2) * dx1 < 0.0) !=
      ((x4 - x2) * dy1 - (y4 - y2) * dx1 < 0.0) &&
      ((x1 - x4) * dy2 - (y1 - y4) * dx2 < 0.0) !=
      ((x2 - x4) * dy2 - (y2 - y4) * dx2 < 0.0);
  // It's is more expensive but more flexible
  // in terms of boundary conditions.
  //
  //int den = (x2-x1) * (y4-y3) - (y2-y1) * (x4-x3);
  //if(fabs(den) < intersection_epsilon) return FALSE;
  //int nom1 = (x4-x3) * (y1-y3) - (y4-y3) * (x1-x3);
  //int nom2 = (x2-x1) * (y1-y3) - (y2-y1) * (x1-x3);
  //int ua = nom1 / den;
  //int ub = nom2 / den;
  //return ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0;
}
//calc_orthogonal
CC_INLINE void calc_orthogonal(int thickness,    int x1, int y1,    int x2, int y2,    int* x, int* y)
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int d = FastDistance2D(dx, dy);
  *x = thickness * dy / d;
  *y = -thickness * dx / d;
}
//dilate_triangle
CC_INLINE void dilate_triangle(int x1, int y1,    int x2, int y2,    int x3, int y3,    int* x, int* y,    int d)
{
  int dx1 = 0;
  int dy1 = 0;
  int dx2 = 0;
  int dy2 = 0;
  int dx3 = 0;
  int dy3 = 0;
  int loc = icross_product(x1, y1, x2, y2, x3, y3);
  if (ABS(loc) > intersection_epsilon) {
    if (icross_product(x1, y1, x2, y2, x3, y3) > 0.0) {
      d = -d;
    }
    calc_orthogonal(d, x1, y1, x2, y2, &dx1, &dy1);
    calc_orthogonal(d, x2, y2, x3, y3, &dx2, &dy2);
    calc_orthogonal(d, x3, y3, x1, y1, &dx3, &dy3);
  }
  *x++ = x1 + dx1;
  *y++ = y1 + dy1;
  *x++ = x2 + dx1;
  *y++ = y2 + dy1;
  *x++ = x2 + dx2;
  *y++ = y2 + dy2;
  *x++ = x3 + dx2;
  *y++ = y3 + dy2;
  *x++ = x3 + dx3;
  *y++ = y3 + dy3;
  *x++ = x1 + dx3;
  *y++ = y1 + dy3;
}
//calc_triangle_area
int calc_triangle_area(int x1, int y1, int x2, int y2, int x3, int y3)
{
  return (x1 * y2 - x2 * y1 + x2 * y3 - x3 * y2 + x3 * y1 - x1 * y3) / 2;
}
//calc_polygon_area
int calc_polygon_area(const IPOINT* st, unsigned stlen)
{
  unsigned i;
  int sum = 0;
  int x = st[0].x;
  int y = st[0].y;
  int xs = x;
  int ys = y;
  for (i = 1; i < stlen; i++) {
    IPOINT v = st[i];
    sum += x * v.y - y * v.x;
    x = v.x;
    y = v.y;
  }
  return (sum + x * ys - y * xs) / 2;
}
//|xa, ya, za|
//|xb, yb, zb|
//|xc, yc, zc|
//#define DET3(xa, ya, za, xb, yb, zb, xc, yc, zc) (xa*yb*zc-xa*yc*zb+yc*xb*za+xc*ya*zb-yb*xc*za-xb*ya*zc)
#define DET3x(x, y, z) DET3((int64)(x)[0], (int64)(y)[0], (int64)(z)[0], (int64)(x)[1], (int64)(y)[1], (int64)(z)[1], (int64)(x)[2], (int64)(y)[2], (int64)(z)[2])
//#define DET3x(x, y, z) DET3((x)[0], (x)[1], (x)[2], (y)[0], (y)[1], (y)[2], (z)[0], (z)[1], (z)[2])
CC_INLINE int64 slove3(int* A, int* b, int* x)
{
  int64 d = DET3x(A, A + 3, A + 6);
  x[0] = (int)(DET3x(b, A + 3, A + 6) * (1 << 16) / d);
  x[1] = (int)(DET3x(A + 0, b, A + 6) * (1 << 16) / d);
  x[2] = (int)(DET3x(A + 0, A + 3, b) * (1 << 16) / d);
  return d;
}
#undef DET3x
CC_INLINE int slove3n(int* A, int* b, int* x, int n)
{
  int i;
  for (i = 0; i < n; ++i) {
    slove3(A, b + i * 3, x + i * 3);
  }
  return 0;
}
#endif
//|xa, ya, za|
//|xb, yb, zb|
//|xc, yc, zc|
//#define DET3(xa, ya, za, xb, yb, zb, xc, yc, zc) (xa*yb*zc-xa*yc*zb+yc*xb*za+xc*ya*zb-yb*xc*za-xb*ya*zc)

#define DET1(A00)  (A00)
#define DET2(A00,A01,A10,A11)   (+(A00)*DET1(A11)-(A01)*DET1(A10))
#define DET3(A00,A01,A02,A10,A11,A12,A20,A21,A22)   (+(A00)*DET2(A11,A12,A21,A22)-(A01)*DET2(A10,A12,A20,A22)+(A02)*DET2(A10,A11,A20,A21))
#define DET3x(x, y, z) DET3((x)[0], (y)[0], (z)[0], (x)[1], (y)[1], (z)[1], (x)[2], (y)[2], (z)[2])
//#define DET3x(x, y, z) DET3((x)[0], (x)[1], (x)[2], (y)[0], (y)[1], (y)[2], (z)[0], (z)[1], (z)[2])
// Ax=b   xi=Di/D  D=det(A) Di=det(Ai) 
CC_INLINE double slove3(const double* A, const double* b, double* x)
{
  double d = DET3x(A, A + 3, A + 6);
  x[0] = DET3x(b, A + 3, A + 6) / d;
  x[1] = DET3x(A + 0, b, A + 6) / d;
  x[2] = DET3x(A + 0, A + 3, b) / d;
  return d;
}
CC_INLINE double slove3n(const double* A, const double* b, double* x, int n)
{
  int i;
  for (i = 0; i < n; ++i) {
    slove3(A, b + i * 3, x + i * 3);
  }
  return 0;
}
// ?????????
// ?????
// ???????????:
//  ???????, ??1??????????е????????δ???x?????,???????????????source code(C):
CC_INLINE uint32 isqrt_linear_search(uint32 x)
{
  uint32 sum_n = 1;
  uint32 n = 1;
  if (x <= 1) {
    return x;
  }
  while (sum_n <= x) {
    n++;
    sum_n += (n << 1) - 1;
  }
  return (n - 1);
}
CC_INLINE uint32 isqrt_bi_search(uint32 x)
{
  uint32 sum_n = 0;
  uint32 n = (x >> 1);
  uint32 top = x;
  uint32 bottom = 0;
  if (x <= 1) {
    return x;
  }
  for (;;) {
    sum_n = n * n;
    if (sum_n < x) {
      bottom = n;
      n += ((top - bottom) >> 1);
      if (n == bottom) {
        return n;
      }
    }
    else if (sum_n > x) {
      top = n;
      n -= ((top - bottom) >> 1);
      if (n == top) {
        return n - 1;
      }
    }
    else {
      return n;
    }
  }
}
// Newton ??
// ?????????????????????????????????x??
// ????????????????????Newton ?????????????????????????????????????????????????????????????????Newton??????????????????????????????У??
// ???????????????????ο?hacker??s delight??
// ??????????????????????????????????????????????С????????????
CC_INLINE uint32 isqrt_newton_method(uint32 x)
{
  uint32 x1 = x - 1;
  uint32 s = 1;
  uint32 g0, g1;
  /* ????趨 */
  if (x1 > 65535) {
    s += 8;
    x1 >>= 16;
  }
  if (x1 > 255)  {
    s += 4;
    x1 >>= 8;
  }
  if (x1 > 15)  {
    s += 2;
    x1 >>= 4;
  }
  if (x1 > 3)   {
    s += 1;
    x1 >>= 2;
  }
  /*????*/
  g0 = 1 << s;
  g1 = (g0 + (x >> s)) >> 1;
  while (g1 < g0) {
    g0 = g1;
    g1 = (g0 + x / g0) >> 1;
  }
  return g0;
}
// ??????????
// ????????????????2λ????????????????????16λ????????????6λ?????????????????λ???????0????1.??????????????????????????????????????????????????δ??????????λ????λ????ε???????λ?????????????????0??????????????????
//  ???????????James Ulery ????:Computing Integer Square Roots
CC_INLINE uint32 isqrt_bitwise_verification(uint32 x)
{
  uint32 temp = 0;
  uint32 v_bit = 15;
  uint32 n = 0;
  uint32 b = 0x8000;
  if (x <= 1) {
    return x;
  }
  do {
    temp = ((n << 1) + b) << (v_bit--);
    if (x >= temp) {
      n += b;
      x -= temp;
    }
  }
  while (b >>= 1);
  return n;
}
CC_INLINE float InvSqrt(float x)
{
  float xhalf = 0.5f * x;
  suf32_t i;
  i.f = x;
  i.i = 0x5f3759df - (i.i >> 1);// ?????????????
  x = i.f;
  x = x * (1.5f - xhalf * x * x); // ????????
  return x;
}
CC_INLINE float CarmSqrt(float x)
{
  union {
    int intPart;
    float floatPart;
  } convertor;
  union {
    int intPart;
    float floatPart;
  } convertor2;
  convertor.floatPart = x;
  convertor2.floatPart = x;
  convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
  convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
  return 0.5f * (convertor.floatPart + (x * convertor2.floatPart));
}
// ????(0,0,0)??(x,y,z)????????????8%
CC_INLINE float FastDistance3D(float fx, float fy, float fz)
{
  int temp, dist;
  int x, y, z;
  // ??????е?????
  x = (int)(fabs(fx) * 1024);
  y = (int)(fabs(fy) * 1024);
  z = (int)(fabs(fz) * 1024);
  // ????
  if (y < x) {
    CC_SWAP(x, y, temp);
  }
  if (z < y) {
    CC_SWAP(y, z, temp);
  }
  if (y < x) {
    CC_SWAP(x, y, temp);
  }
  dist = (z + 11 * (y >> 5) + (x >> 2));
  return((float)(dist >> 10));
}
/****************************************************************************************\
***??ú???
\****************************************************************************************/
static const float table_fsin[] = { //
#include "math/table/table_fsin.txt"
}; // table_fsin
CC_INLINE float iSin(int angle)
{
  angle += (angle < 0 ? 360 : 0);
  return table_fsin[ angle ];
}
CC_INLINE float iCos(int angle)
{
  angle += (angle < 0 ? 360 : 0);
  return table_fsin[ 450 - angle ];
}
CC_INLINE int iSinCos(int angle, float* cosval, float* sinval)
{
  angle += (angle < 0 ? 360 : 0);
  *sinval = table_fsin[ angle ];
  *cosval = table_fsin[ 450 - angle ];
  return 0;
}
//#define DET2(a00,a01,a10,a11) ((a00) * (a11) - (a01) * (a10))
//#define DET2(A11,A12,A21,A22) (A11*A22-A12*A21)
//#define DET3(A11,A12,A13,A21,A22,A23,A31,A32,A33) (A11*DET2(A22,A23,A32,A33),A12*DET2(A21,A23,A31,A33),A13*DET2(A21,A22,A31,A32))
//#define DET4(A11,A12,A13,A14, A21,A22,A23,A24, A31,A32,A33,A34, A41,A42,A43,A44) D4(1,2,3,4,1,2,3,4)
// b=inv(a)
#define INV2(a00,a01,a10,a11,b00,b01,b10,b11)                 \
  do {                                     \
    double det_a = (DET2(a00,a01,a10,a11));                   \
    if(det_a != 0.) {                              \
      det_a = 1./det_a;                             \
      b11 = ( (a00)*det_a);                           \
      b00 = ( (a11)*det_a);                           \
      b01 = (-(a01)*det_a);                           \
      b10 = (-(a10)*det_a);                           \
    }                                      \
  } while(0)
/*
a = [\
a11, a12, a13;\
a21, a22, a23;\
a31, a32, a33\
];
b = [b1; b2; b3];
matdet(a)
matsolve(a, b)
*/
// eqq:={
//  a[0]*x[0]+b[0]*x[1]=c[0],
//  a[1]*x[0]+b[1]*x[1]=c[1]
// };
// solve(eqq, {x[0], x[1]});
CC_INLINE double solve_2x2(double a[], double b[], double c[], double x[])
{
  double dd;
  dd = DET2(a[ 0 ], a[ 1 ], b[ 0 ], b[ 1 ]);
  if (0 != dd) {
    x[ 0 ] = DET2(c[ 0 ], c[ 1 ], b[ 0 ], b[ 1 ]) / dd;
    x[ 1 ] = DET2(a[ 0 ], a[ 1 ], c[ 0 ], c[ 1 ]) / dd;
  }
  return dd;
}
// eqq:={
//  a[0]*x[0]+b[0]*x[1]+c[0]*x[2]=d[0],
//  a[1]*x[0]+b[1]*x[1]+c[1]*x[2]=d[1],
//  a[2]*x[0]+b[2]*x[1]+c[2]*x[2]=d[2]
// };
// solve(eqq, {x[0], x[1], x[2]});
// #define SOLVE_3X3(a0,a1,a2,a3,a4,a5,a6,a7,a8)  ((a0)*((a8)*(a4)-(a7)*(a5))+(a1)*((a6)*(a5)-(a8)*(a3))-(a2)*((a6)*(a4)+(a7)*(a3)))
#define SOLVE_3X3(a0,a1,a2,a3,a4,a5,a6,a7,a8)  ((a0)*((a8)*(a4)-(a7)*(a5)) + (a1)*((a6)*(a5)-(a8)*(a3)) + (a2)*((a7)*(a3)-(a6)*(a4)))
CC_INLINE double solve_3x3_1(double* a, double* b, double* c, double* d, double* x)
{
  double dd;
  dd = SOLVE_3X3(a[ 0 ], a[ 1 ], a[ 2 ], b[ 0 ], b[ 1 ], b[ 2 ], c[ 0 ], c[ 1 ], c[ 2 ]);
  // d[0], d[1], d[2]
  if (0 != dd && x) {
    x[ 0 ] = SOLVE_3X3(d[ 0 ], d[ 1 ], d[ 2 ], b[ 0 ], b[ 1 ], b[ 2 ], c[ 0 ], c[ 1 ], c[ 2 ]) / dd;
    x[ 1 ] = SOLVE_3X3(a[ 0 ], a[ 1 ], a[ 2 ], d[ 0 ], d[ 1 ], d[ 2 ], c[ 0 ], c[ 1 ], c[ 2 ]) / dd;
    x[ 2 ] = SOLVE_3X3(a[ 0 ], a[ 1 ], a[ 2 ], b[ 0 ], b[ 1 ], b[ 2 ], d[ 0 ], d[ 1 ], d[ 2 ]) / dd;
  }
  return dd;
}
// eqq:={
//  a[0]*x[0]+b[0]*x[1]+x[2]=c[0],
//  a[1]*x[0]+b[1]*x[1]+x[2]=c[1],
//  a[2]*x[0]+b[2]*x[1]+x[2]=c[2]
// };
// solve(eqq, {x[0], x[1], x[2]});
CC_INLINE double solve_3x2_1(double* a, double* b, double* c, double* x)
{
  double dd;
  dd = SOLVE_3X3(a[ 0 ], a[ 1 ], a[ 2 ], b[ 0 ], b[ 1 ], b[ 2 ], 1, 1, 1);
  // c[0], c[1], c[2]
  if (0 != dd && x) {
    x[ 0 ] = SOLVE_3X3(c[ 0 ], c[ 1 ], c[ 2 ], b[ 0 ], b[ 1 ], b[ 2 ], 1, 1, 1) / dd;
    x[ 1 ] = SOLVE_3X3(a[ 0 ], a[ 1 ], a[ 2 ], c[ 0 ], c[ 1 ], c[ 2 ], 1, 1, 1) / dd;
    x[ 2 ] = SOLVE_3X3(a[ 0 ], a[ 1 ], a[ 2 ], b[ 0 ], b[ 1 ], b[ 2 ], c[ 0 ], c[ 1 ], c[ 2 ]) / dd;
  }
  return dd;
}
/*
eqq:={
 a[0]*x[0]+b[0]*x[1]+c[0]*x[2]+d[0]*x[3]=e[0],
 a[1]*x[0]+b[1]*x[1]+c[1]*x[2]+d[1]*x[3]=e[1],
 a[2]*x[0]+b[2]*x[1]+c[2]*x[2]+d[2]*x[3]=e[2],
 a[3]*x[0]+b[3]*x[1]+c[3]*x[2]+d[3]*x[3]=e[3]
};
solve(eqq, {x[0], x[1], x[2], x[3]});
*/
#define SOLVE_4X4(a0,a1,a2,a3,b0,b1,b2,b3,c0,c1,c2,c3,d0,d1,d2,d3) \
  -(+(b2)*(c0)*(a3)*(d1)-(a0)*(b1)*(c2)*(d3)+(a2)*(c1)*(b3)*(d0)+(b1)*(a2)*(c0)*(d3) \
      +(a0)*(b1)*(c3)*(d2)+(c2)*(b1)*(a3)*(d0)+(c2)*(a0)*(b3)*(d1)-(a0)*(b3)*(c1)*(d2) \
      -(b3)*(c0)*(a2)*(d1)-(a3)*(c1)*(b2)*(d0)-(b1)*(a3)*(c0)*(d2)+(a0)*(b2)*(c1)*(d3) \
      -(c3)*(b1)*(a2)*(d0)-(c3)*(a0)*(b2)*(d1)-(b0)*(c2)*(a3)*(d1)+(b0)*(a3)*(c1)*(d2) \
      -(b0)*(a2)*(c1)*(d3)+(b0)*(c3)*(a2)*(d1)+(a1)*(b0)*(c2)*(d3)-(a1)*(b0)*(c3)*(d2) \
      +(a1)*(b3)*(c0)*(d2)-(a1)*(b2)*(c0)*(d3)+(a1)*(c3)*(b2)*(d0)-(a1)*(c2)*(b3)*(d0))
CC_INLINE double solve_4x4_1(double* a, double* b, double* c, double* d, double* e, double* x)
{
  double dd;
  dd = SOLVE_4X4(a[ 0 ], a[ 1 ], a[ 2 ], a[ 3 ], b[ 0 ], b[ 1 ], b[ 2 ], b[ 3 ], c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ], d[ 0 ], d[ 1 ], d[ 2 ], d[ 3 ]);
  // d[0], d[1], d[2]
  if (0 != dd && x) {
    x[ 0 ] = SOLVE_4X4(e[ 0 ], e[ 1 ], e[ 2 ], e[ 3 ], b[ 0 ], b[ 1 ], b[ 2 ], b[ 3 ], c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ], d[ 0 ], d[ 1 ], d[ 2 ], d[ 3 ]) / dd;
    x[ 1 ] = SOLVE_4X4(a[ 0 ], a[ 1 ], a[ 2 ], a[ 3 ], e[ 0 ], e[ 1 ], e[ 2 ], e[ 3 ], c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ], d[ 0 ], d[ 1 ], d[ 2 ], d[ 3 ]) / dd;
    x[ 2 ] = SOLVE_4X4(a[ 0 ], a[ 1 ], a[ 2 ], a[ 3 ], b[ 0 ], b[ 1 ], b[ 2 ], b[ 3 ], e[ 0 ], e[ 1 ], e[ 2 ], e[ 3 ], d[ 0 ], d[ 1 ], d[ 2 ], d[ 3 ]) / dd;
    x[ 3 ] = SOLVE_4X4(a[ 0 ], a[ 1 ], a[ 2 ], a[ 3 ], b[ 0 ], b[ 1 ], b[ 2 ], b[ 3 ], c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ], e[ 0 ], e[ 1 ], e[ 2 ], e[ 3 ]) / dd;
  }
  return dd;
}
// ?????????????????
// a*x+b*y = 1;
// |a1, b1|  |x|  |1|
// |   | * | | = | |
// |a2, b2|  |y|  |1|
CC_INLINE void liear_slove(double a1, double b1, double a2, double b2,           /*@out@*/ double* x,
    /*@out@*/ double* y)
{
  double cc = a1 * b2 - a2 * b1;
  *x = (b2 - b1) / cc;
  *y = (a1 - a2) / cc;
  return ;
}
/****************************************************************************************\
***2d???
\****************************************************************************************/
CC_INLINE double NegativeInfinity()
{
  union {
    uint64 i;
    double d;
  } num;
  num.i = 0x7FF0000000000000UL;
  return num.d;
}
//?任???:??P??任????,P1??任?????A??任????,??
//  P1 = P * A
//|P1.x|   |P.x|   |a00,a01,a02|
//|P1.y| = |P.y| * |a10,a11,a12|
//|P1.z|   |P.z|   |a20,a21,a22|
#define M2TRANSFORM_3X3_D(T, _I, X, Y, Z, PX, PY, PZ) \
  (PX = (T)(X * _I.a00 + Y * _I.a10 + Z * _I.a20), \
      PY = (T)(X * _I.a01 + Y * _I.a11 + Z * _I.a21), \
      PZ = (T)(X * _I.a02 + Y * _I.a12 + Z * _I.a22))
#define M2TRANSFORM_3X3_D2(T, _I, X, Y, Z, PX, PY) \
  (PX = (T)(X * _I.a00 + Y * _I.a10 + Z * _I.a20), \
      PY = (T)(X * _I.a01 + Y * _I.a11 + Z * _I.a21))
#define TRANS_AFFINE_DEF(na) double na[1]={1,0,0,1,0,0};
#define affine_init(ta) (ta[0] = (1.0f), ta[3] = (0.0f), ta[1] = (0.0f), ta[4] = (1.0f), ta[2] = (0.0f), ta[5] = (0.0f))
CC_INLINE double* affine_set(double* ta, double sx, double shy, double shx, double sy, double tx, double ty)
{
  ta[0] = sx, ta[1] = shx, ta[3] = shy, ta[4] = sy, ta[2] = tx, ta[5] = ty;
  return ta;
}
#define affine_translation(ta, x, y) affine_setmul(ta, 1.0, 0.0, 0.0, 1.0, x, y)
#define affine_scaling(ta, x, y)     affine_setmul(ta, x, 0.0, 0.0, y, 0.0, 0.0)
#define affine_skewing(ta, x, y)     affine_setmul(ta, 1.0, tan(y), tan(x), 1.0, 0.0, 0.0)
#define affine_rotation(ta, a)       affine_setmul(ta, cos(a), sin(a), -sin(a), cos(a), 0.0, 0.0)
//
CC_INLINE int affine_transform_2x2(const double* ta, double* px, double* py)
{
  register double x = *px, y = *py;
  *px = x * ta[0] + y * ta[1];
  *py = x * ta[3] + y * ta[4];
  return 0;
}
CC_INLINE int affine_transform_2x2_f32(const double* ta, float* px, float* py)
{
  register double x = *px, y = *py;
  *px = x * ta[0] + y * ta[1];
  *py = x * ta[3] + y * ta[4];
  return 0;
}
CC_INLINE int affine_transform(const double* ta, double* px, double* py)
{
  register double x = *px, y = *py;
  *px = x * ta[0] + y * ta[1] + ta[2];
  *py = x * ta[3] + y * ta[4] + ta[5];
  return 0;
}
CC_INLINE int affine_transform_f32(const double* ta, float* px, float* py)
{
  register double x = *px, y = *py;
  *px = x * ta[0] + y * ta[1] + ta[2];
  *py = x * ta[3] + y * ta[4] + ta[5];
  return 0;
}
CC_INLINE int affine_transform_pt_f32(const double* ta, FPOINT* pt, int n)
{
  int i;
  for (i = 0; i < n; ++i) {
    affine_transform_f32(ta, &pt[i].x, &pt[i].y);
  }
  return 0;
}
CC_INLINE int affine_translation_pt_f32(FPOINT* pt, int n, double x, double y)
{
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x+=x, pt[i].y+=y;
  }
  return 0;
}
CC_INLINE double* affine_multiply(double* ta, const double* m)
{
  double t0 = ta[0] * m[0] + ta[3] * m[1];
  double t2 = ta[1] * m[0] + ta[4] * m[1];
  double t4 = ta[2] * m[0] + ta[5] * m[1] + m[2];
  ta[3] = ta[0] * m[3] + ta[3] * m[4];
  ta[4] = ta[1] * m[3] + ta[4] * m[4];
  ta[5] = ta[2] * m[3] + ta[5] * m[4] + m[5];
  ta[0] = t0;
  ta[1] = t2;
  ta[2] = t4;
  return ta;
}
CC_INLINE double* affine_setmul(double* ta, double v0, double v1, double v2, double v3, double v4, double v5)
{
  double m[6] = {0};
  m[0] = (v0), m[3] = (v1), m[1] = (v2), m[4] = (v3), m[2] = (v4), m[5] = (v5);
  return affine_multiply(ta, m);
}
CC_INLINE double affine_determinant(const double* ta)
{
  return ta[0] * ta[4] - ta[3] * ta[1];
}
CC_INLINE double affine_determinant_reciprocal(const double* ta)
{
  return 1.0 / (ta[0] * ta[4] - ta[3] * ta[1]);
}
CC_INLINE double* affine_invert(double* ta)
{
  double d = affine_determinant_reciprocal(ta);
  double t4, t0 = ta[4] * d;
  ta[4] = ta[0] * d;
  ta[3] = -ta[3] * d;
  ta[1] = -ta[1] * d;
  t4 = -ta[2] * t0 - ta[5] * ta[1];
  ta[5] = -ta[2] * ta[3] - ta[5] * ta[4];
  ta[0] = t0;
  ta[2] = t4;
  return ta;
}
//vertex_dist_epsilon
// Coinciding points maximal distance (Epsilon)
static const double vertex_dist_epsilon = 1e-14;
//intersection_epsilon
// See calc_intersection
static const double intersection_epsilon = 1.0e-30;
//cross_product
CC_INLINE double cross_product(double x1, double y1,  double x2, double y2,  double x, double y)
{
  return (x - x2) * (y2 - y1) - (y - y2) * (x2 - x1);
}
//point_in_triangle
CC_INLINE BOOL point_in_triangle1(double x1, double y1,
    double x2, double y2,
    double x3, double y3,
    double x, double y)
{
  BOOL cp1 = cross_product(x1, y1, x2, y2, x, y) < 0.0;
  BOOL cp2 = cross_product(x2, y2, x3, y3, x, y) < 0.0;
  BOOL cp3 = cross_product(x3, y3, x1, y1, x, y) < 0.0;
  return cp1 == cp2 && cp2 == cp3 && cp3 == cp1;
}
//?ж??q?????????????
CC_INLINE BOOL point_in_poly(const DPOINT* pt, int len, double x, double y)
{
  int i, j, k = 0;
  double Xcoss;
  for (i = 0; i < len; i++) {
    j = (i == len - 1) ? 0 : (i + 1);
    if ((y < pt[i].y && y >= pt[j].y) || (y >= pt[i].y && y < pt[j].y)) {
      Xcoss = (y - pt[i].y) * (pt[i].x - pt[j].x) / (pt[i].y - pt[j].y) + pt[i].x;
      if (x <= Xcoss) {
        k++;
      }
    }
  }
  return k % 2;
}
CC_INLINE int poly_offset(DPOINT* pt, int len, double x, double y)
{
  int i;
  for (i = 0; i < len; i++) {
    pt[i].x += x;
    pt[i].y += y;
  }
  return 0;
}
CC_INLINE int poly_scale(DPOINT* pt, int len, double sc)
{
  int i;
  for (i = 0; i < len; i++) {
    pt[i].x *= sc;
    pt[i].y *= sc;
  }
  return 0;
}
//calc_distance
CC_INLINE double calc_distance1(double x1, double y1, double x2, double y2)
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  return sqrt(dx * dx + dy * dy);
}
//calc_sq_distance
CC_INLINE double calc_sq_distance(double x1, double y1, double x2, double y2)
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  return dx * dx + dy * dy;
}
//calc_line_point_distance
CC_INLINE double calc_line_point_distance(double x1, double y1,
    double x2, double y2,
    double x, double y)
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  double d = sqrt(dx * dx + dy * dy);
  if (d < vertex_dist_epsilon) {
    return calc_distance(x1, y1, x, y);
  }
  return ((x - x2) * dy - (y - y2) * dx) / d;
}
//calc_line_point_u
CC_INLINE double calc_segment_point_u(double x1, double y1,
    double x2, double y2,
    double x, double y)
{
  double pdx, pdy, dx = x2 - x1, dy = y2 - y1;
  if (dx == 0 && dy == 0) {
    return 0;
  }
  pdx = x - x1;
  pdy = y - y1;
  return (pdx * dx + pdy * dy) / (dx * dx + dy * dy);
}
//calc_line_point_sq_distance
CC_INLINE double calc_segment_point_sq_distance(double x1, double y1,    double x2, double y2,    double x, double y,    double u)
{
  if (u <= 0) {
    return calc_sq_distance(x, y, x1, y1);
  }
  else if (u >= 1) {
    return calc_sq_distance(x, y, x2, y2);
  }
  return calc_sq_distance(x, y, x1 + u * (x2 - x1), y1 + u * (y2 - y1));
}
//calc_line_point_sq_distance
CC_INLINE double calc_segment_point_sq_distance1(double x1, double y1,
    double x2, double y2,
    double x, double y)
{
  return
      calc_segment_point_sq_distance(
          x1, y1, x2, y2, x, y,
          calc_segment_point_u(x1, y1, x2, y2, x, y));
}
//calc_intersection
CC_INLINE BOOL calc_intersection(double ax, double ay, double bx, double by,
    double cx, double cy, double dx, double dy,
    double* x, double* y)
{
  double r;
  double num = (ay - cy) * (dx - cx) - (ax - cx) * (dy - cy);
  double den = (bx - ax) * (dy - cy) - (by - ay) * (dx - cx);
  if (fabs(den) < intersection_epsilon) {
    return FALSE;
  }
  r = num / den;
  *x = ax + r * (bx - ax);
  *y = ay + r * (by - ay);
  return TRUE;
}
//intersection_exists
CC_INLINE BOOL intersection_exists(double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4)
{
  // It's less expensive but you can't control the
  // boundary conditions: Less or LessEqual
  double dx1 = x2 - x1;
  double dy1 = y2 - y1;
  double dx2 = x4 - x3;
  double dy2 = y4 - y3;
  return ((x3 - x2) * dy1 - (y3 - y2) * dx1 < 0.0) !=
      ((x4 - x2) * dy1 - (y4 - y2) * dx1 < 0.0) &&
      ((x1 - x4) * dy2 - (y1 - y4) * dx2 < 0.0) !=
      ((x2 - x4) * dy2 - (y2 - y4) * dx2 < 0.0);
  // It's is more expensive but more flexible
  // in terms of boundary conditions.
  //
  //double den = (x2-x1) * (y4-y3) - (y2-y1) * (x4-x3);
  //if(fabs(den) < intersection_epsilon) return FALSE;
  //double nom1 = (x4-x3) * (y1-y3) - (y4-y3) * (x1-x3);
  //double nom2 = (x2-x1) * (y1-y3) - (y2-y1) * (x1-x3);
  //double ua = nom1 / den;
  //double ub = nom2 / den;
  //return ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0;
}
//calc_orthogonal
CC_INLINE void calc_orthogonal(double thickness,
    double x1, double y1,
    double x2, double y2,
    double* x, double* y)
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  double d = sqrt(dx * dx + dy * dy);
  *x = thickness * dy / d;
  *y = -thickness * dx / d;
}
//dilate_triangle
CC_INLINE void dilate_triangle(double x1, double y1,
    double x2, double y2,
    double x3, double y3,
    double* x, double* y,
    double d)
{
  double dx1 = 0.0;
  double dy1 = 0.0;
  double dx2 = 0.0;
  double dy2 = 0.0;
  double dx3 = 0.0;
  double dy3 = 0.0;
  double loc = cross_product(x1, y1, x2, y2, x3, y3);
  if (fabs(loc) > intersection_epsilon) {
    if (cross_product(x1, y1, x2, y2, x3, y3) > 0.0) {
      d = -d;
    }
    calc_orthogonal(d, x1, y1, x2, y2, &dx1, &dy1);
    calc_orthogonal(d, x2, y2, x3, y3, &dx2, &dy2);
    calc_orthogonal(d, x3, y3, x1, y1, &dx3, &dy3);
  }
  *x++ = x1 + dx1;
  *y++ = y1 + dy1;
  *x++ = x2 + dx1;
  *y++ = y2 + dy1;
  *x++ = x2 + dx2;
  *y++ = y2 + dy2;
  *x++ = x3 + dx2;
  *y++ = y3 + dy2;
  *x++ = x3 + dx3;
  *y++ = y3 + dy3;
  *x++ = x1 + dx3;
  *y++ = y1 + dy3;
}
//calc_triangle_area
CC_INLINE double calc_triangle_area(double x1, double y1,
    double x2, double y2,
    double x3, double y3)
{
  return (x1 * y2 - x2 * y1 + x2 * y3 - x3 * y2 + x3 * y1 - x1 * y3) * 0.5;
}
//calc_polygon_area
CC_INLINE double calc_polygon_area(const DPOINT* st, unsigned stlen)
{
  unsigned i;
  double sum = 0.0;
  double x = st[0].x;
  double y = st[0].y;
  double xs = x;
  double ys = y;
  for (i = 1; i < stlen; i++) {
    DPOINT v = st[i];
    sum += x * v.y - y * v.x;
    x = v.x;
    y = v.y;
  }
  return (sum + x * ys - y * xs) * 0.5;
}
// Tables for fast sqrt
static unsigned short g_sqrt_table[1024];
static int8  g_elder_bit_table[256];
//fast_sqrt
//Fast integer Sqrt - really fast: no cycles, divisions or multiplications
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4035) //Disable warning "no return value"
#endif
CC_INLINE unsigned fast_sqrt(unsigned val)
{
#if defined(_M_IX86) && defined(_MSC_VER) && !defined(AGG_NO_ASM)
  //For Ix86 family processors this assembler code is used.
  //The key command here is bsr - determination the number of the most
  //significant bit of the value. For other processors
  //(and maybe compilers) the pure C "#else" section is used.
  __asm {
    mov ebx, val
    mov edx, 11
    bsr ecx, ebx
    sub ecx, 9
    jle less_than_9_bits
    shr ecx, 1
    adc ecx, 0
    sub edx, ecx
    shl ecx, 1
    shr ebx, cl
    less_than_9_bits:
    xor eax, eax
    mov ax, g_sqrt_table[ebx*2]
    mov ecx, edx
    shr eax, cl
  }
#else
  //This code is actually pure C and portable to most
  //arcitectures including 64bit ones.
  unsigned t = val;
  int bit = 0;
  unsigned shift = 11;
  //The following piece of code is just an emulation of the
  //Ix86 assembler command "bsr" (see above). However on old
  //Intels (like Intel MMX 233MHz) this code is about twice
  //faster (sic!) then just one "bsr". On PIII and PIV the
  //bsr is optimized quite well.
  bit = t >> 24;
  if (bit) {
    bit = g_elder_bit_table[bit] + 24;
  }
  else {
    bit = (t >> 16) & 0xFF;
    if (bit) {
      bit = g_elder_bit_table[bit] + 16;
    }
    else {
      bit = (t >> 8) & 0xFF;
      if (bit) {
        bit = g_elder_bit_table[bit] + 8;
      }
      else {
        bit = g_elder_bit_table[t];
      }
    }
  }
  //This code calculates the sqrt.
  bit -= 9;
  if (bit > 0) {
    bit = (bit >> 1) + (bit & 1);
    shift -= bit;
    val >>= (bit << 1);
  }
  return g_sqrt_table[val] >> shift;
#endif
}
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
//besj
// Function BESJ calculates Bessel function of first kind of order n
// Arguments:
//   n - an integer (>=0), the order
//   x - value at which the Bessel function is required
//
// C++ Mathematical Library
// Convereted from equivalent FORTRAN library
// Converetd by Gareth Walker for use by course 392 computational project
// All functions tested and yield the same results as the corresponding
// FORTRAN versions.
//
// If you have any problems using these functions please report them to
// M.Muldoon@UMIST.ac.uk
//
// Documentation available on the web
// http://www.ma.umist.ac.uk/mrm/Teaching/392/libs/392.html
// Version 1.0  8/98
// 29 October, 1999
//
// Adapted for use in AGG library by Andy Wilk (castor.vulgaris@gmail.com)
CC_INLINE double besj(double x, int n)
{
  double d, b, b1, c6;
  int m1, m2, imax, i;
  if (n < 0) {
    return 0;
  }
  d = 1E-6;
  b = 0;
  if (fabs(x) <= d) {
    if (n != 0) {
      return 0;
    }
    return 1;
  }
  b1 = 0; // b1 is the value from the previous iteration
  // Set up a starting order for recurrence
  m1 = (int)fabs(x) + 6;
  if (fabs(x) > 5) {
    m1 = (int)(fabs(1.4 * x + 60 / x));
  }
  m2 = (int)(n + 2 + fabs(x) / 4);
  if (m1 > m2) {
    m2 = m1;
  }
  // Apply recurrence down from curent max order
  for (;;) {
    double c3 = 0;
    double c2 = 1E-30;
    double c4 = 0;
    int m8 = 1;
    if (m2 / 2 * 2 == m2) {
      m8 = -1;
    }
    imax = m2 - 2;
    for (i = 1; i <= imax; i++) {
      double c6 = 2 * (m2 - i) * c2 / x - c3;
      c3 = c2;
      c2 = c6;
      if (m2 - i - 1 == n) {
        b = c6;
      }
      m8 = -1 * m8;
      if (m8 > 0) {
        c4 = c4 + 2 * c6;
      }
    }
    c6 = 2 * c2 / x - c3;
    if (n == 0) {
      b = c6;
    }
    c4 += c6;
    b /= c4;
    if (fabs(b - b1) < d) {
      return b;
    }
    b1 = b;
    m2 += 3;
  }
}

typedef struct fixed_t {
#ifndef _MAC
  ushort  fract;
  short   value;
#else
  short   value;
  ushort   fract;
#endif
} fixed_t;

CC_INLINE fixed_t dbl_to_fx(double d)
{
  int l;
  l = (int)(d * 65536.0);
  return *(fixed_t*)&l;
}
CC_INLINE int dbl_to_plain_fx(double d)
{
  return (int)(d * 65536.0);
}
CC_INLINE fixed_t negate_fx(const fixed_t fx)
{
  int l = -(*(int*)(&fx));
  return *(fixed_t*)&l;
}
#define fx_to_dbl(_p)  ((_p).value + (_p).fract / 65536.0)
CC_INLINE int fx_to_plain_int(const fixed_t fx)
{
  return *(int*)(&fx);
}
CC_INLINE int fx_to_int26p6(const fixed_t p)
{
  return ((int)(p.value) << 6) + ((int)(p.fract) >> 10);
}
CC_INLINE int dbl_to_int26p6(double p)
{
  return (int)(p * 64.0 + 0.5);
}
static const double affine_epsilon = 1e-14;
static const double epsilon = 1e-14;
// Identity matrix
CC_INLINE void perspective_init(double* p)
{
  p[0] = (1), p[3] = (0), p[6] = (0);
  p[1] = (0), p[4] = (1), p[7] = (0);
  p[2] = (0), p[5] = (0), p[8] = (1);
}
// Custom matrix
CC_INLINE void perspective_set(double* p,
    double v0, double v1, double v2,
    double v3, double v4, double v5,
    double v6, double v7, double v8)
{
  p[0] = (v0), p[3] = (v1), p[6] = (v2);
  p[1] = (v3), p[4] = (v4), p[7] = (v5);
  p[2] = (v6), p[5] = (v7), p[8] = (v8);
}
// Custom matrix. Usually used in derived classes
CC_INLINE void perspective_set2(double* p, double v0, double v1, double v2, double v3, double v4, double v5)
{
  perspective_init(p);
  p[0] = (v0), p[3] = (v1), p[1] = (v2), p[4] = (v3), p[2] = (v4), p[5] = (v5);
}
// Custom matrix from m[9]
CC_INLINE void perspective_setv(double* p, const double* m)
{
  p[0] = (m[0]), p[3] = (m[1]), p[6] = (m[2]),
      p[1] = (m[3]), p[4] = (m[4]), p[7] = (m[5]),
          p[2] = (m[6]), p[5] = (m[7]), p[8] = (m[8]);
}
// Custom matrix from m[6]
CC_INLINE void perspective_setv2(double* p, const double* m)
{
  perspective_init(p);
  p[0] = (m[0]), p[3] = (m[1]), p[1] = (m[2]), p[4] = (m[3]), p[2] = (m[4]), p[5] = (m[5]);
}
CC_INLINE void perspective_transform_2x3(const double* p, double* x, double* y)
{
  register double tmp = *x;
  *x = tmp * p[0] + *y * p[1] + p[2];
  *y = tmp * p[3] + *y * p[4] + p[5];
}
CC_INLINE void perspective_transform(const double* p, double* px, double* py)
{
  double x = *px;
  double y = *py;
  double m = 1.0 / (x * p[6] + y * p[7] + p[8]);
  *px = m * (x * p[0] + y * p[1] + p[2]);
  *py = m * (x * p[3] + y * p[4] + p[5]);
}
CC_INLINE void perspective_transform_2x2(double* p, double* x, double* y)
{
  register double tmp = *x;
  *x = tmp * p[0] + *y * p[1];
  *y = tmp * p[3] + *y * p[4];
}
CC_INLINE double perspective_determinant(const double* p)
{
  return
      p[0] * (p[4] * p[8] - p[5] * p[7]) +
      p[1] * (p[5] * p[6] - p[3] * p[8]) +
      p[2] * (p[3] * p[7] - p[4] * p[6]);
}
CC_INLINE double perspective_determinant_reciprocal(const double* p)
{
  return 1.0 / perspective_determinant(p);
}
CC_INLINE int perspective_invert_2x2(double* p, const double* a)
{
  double d = perspective_determinant_reciprocal(a);
  double t4, t0 = p[4] * d;
  p[4] = p[0] * d;
  p[3] = -p[3] * d;
  p[1] = -p[1] * d;
  t4 = -p[2] * t0 - p[5] * p[1];
  p[5] = -p[2] * p[3] - p[5] * p[4];
  p[0] = t0;
  p[2] = t4;
  return 0;
}
CC_INLINE BOOL perspective_invert(double* o, const double* p)
{
  double a[9];
  double d0 = p[4] * p[8] - p[7] * p[5];
  double d1 = p[6] * p[5] - p[3] * p[8];
  double d2 = p[3] * p[7] - p[6] * p[4];
  double d = p[0] * d0 + p[1] * d1 + p[2] * d2;
  if (d == 0.0) {
    o[0] = o[3] = o[6] = o[1] = o[4] = o[7] = o[2] = o[5] = o[8] = 0.0;
    return FALSE;
  }
  d = 1.0 / d;
  MEMCPY(a, p, 9);
  o[0] = d * d0;
  o[3] = d * d1;
  o[6] = d * d2;
  o[1] = d * (a[7] * a[2] - a[1] * a[8]);
  o[4] = d * (a[0] * a[8] - a[6] * a[2]);
  o[7] = d * (a[6] * a[1] - a[0] * a[7]);
  o[2] = d * (a[1] * a[5] - a[4] * a[2]);
  o[5] = d * (a[3] * a[2] - a[0] * a[5]);
  o[8] = d * (a[0] * a[4] - a[3] * a[1]);
  return TRUE;
}
CC_INLINE int perspective_multiply_2x2(double* p, const double* a, const double* b)
{
  double c[9];
  double t0 = a[0] * b[0] + a[3] * b[1];
  double t2 = a[1] * b[0] + a[4] * b[1];
  double t4 = a[2] * b[0] + a[5] * b[1] + b[2];
  c[3] = a[0] * b[3] + a[3] * b[4];
  c[4] = a[1] * b[3] + a[4] * b[4];
  c[5] = a[2] * b[3] + a[5] * b[4] + b[5];
  c[0] = t0;
  c[1] = t2;
  c[2] = t4;
  MEMCPY(p, c, 9);
  return 0;
}
CC_INLINE int perspective_multiply(double* p, const double* a, const double* b)
{
  double c[9];
#if 0
  c[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];
  c[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];
  c[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];
  c[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
  c[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
  c[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];
  c[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
  c[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
  c[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];
#else
  c[0] = b[0] * a[0] + b[1] * a[3] + b[2] * a[6];
  c[1] = b[0] * a[1] + b[1] * a[4] + b[2] * a[7];
  c[2] = b[0] * a[2] + b[1] * a[5] + b[2] * a[8];
  c[3] = b[3] * a[0] + b[4] * a[3] + b[5] * a[6];
  c[4] = b[3] * a[1] + b[4] * a[4] + b[5] * a[7];
  c[5] = b[3] * a[2] + b[4] * a[5] + b[5] * a[8];
  c[6] = b[6] * a[0] + b[7] * a[3] + b[8] * a[6];
  c[7] = b[6] * a[1] + b[7] * a[4] + b[8] * a[7];
  c[8] = b[6] * a[2] + b[7] * a[5] + b[8] * a[8];
#endif
  MEMCPY(p, c, 9);
  return 0;
}
CC_INLINE void perspective_inverse_transform(double* p, double* x, double* y)
{
  double t;
  if (perspective_invert(&t, p)) {
    perspective_transform(&t, x, y);
  }
}
CC_INLINE void perspective_inverse_transform_2x3(double* p, double* x, double* y)
{
  register double d = perspective_determinant_reciprocal(p);
  register double a = (*x - p[2]) * d;
  register double b = (*y - p[5]) * d;
  *x = a * p[4] - b * p[1];
  *y = b * p[0] - a * p[3];
}
CC_INLINE double perspective_get_scale(double* p)
{
  double x = 0.707106781 * p[0] + 0.707106781 * p[1];
  double y = 0.707106781 * p[3] + 0.707106781 * p[4];
  return sqrt(x * x + y * y);
}
CC_INLINE int perspective_translate(double* p, double x, double y)
{
  p[2] += x;
  p[5] += y;
  return 0;
}
//====================================================trans_rotation
// Rotation matrix. sin() and cos() are calculated twice for the same angle.
// There's no harm because the performance of sin()/cos() is very good on all
// modern processors. Besides, this operation is not going to be invoked too
// often.
#define perspective_set_rotation(p, a)  perspective_set2(p, cos(a), sin(a), -sin(a), cos(a), 0.0, 0.0)
//====================================================trans_scaling
// Scaling matrix. x, y - scale coefficients by X and Y respectively
#define perspective_set_scaling2(p, x, y) perspective_set2(p, x, 0.0, 0.0, y, 0.0, 0.0)
#define perspective_set_scaling(p, s) perspective_set2(p, s, 0.0, 0.0, s, 0.0, 0.0)
//================================================trans_translation
// Translation matrix
#define perspective_set_translation(p, x, y) perspective_set2(p, 1.0, 0.0, 0.0, 1.0, x, y)
//====================================================trans_skewing
// Sckewing (shear) matrix
#define perspective_set_skewing(p, x, y) perspective_set(p, 1.0, tan(y), tan(x), 1.0, 0.0, 0.0)
//===============================================trans_line_segment
// Rotate, Scale and Translate, associating 0...dist with line segment
// x1,y1,x2,y2
CC_INLINE void perspective_line_segment(double* p, double x1, double y1, double x2, double y2, double dist)
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  double t;
  if (dist > 0.0) {
    perspective_set_scaling(&t, sqrt(dx * dx + dy * dy) / dist);
    perspective_multiply(p, p, &t);
  }
  perspective_set_rotation(&t, atan2(dy, dx));
  perspective_multiply(p, p, &t);
  perspective_set_translation(&t, x1, y1);
  perspective_multiply(p, p, &t);
}
//============================================trans_reflection_unit
// Reflection matrix. Reflect coordinates across the line through
// the origin containing the unit vector (ux, uy).
// Contributed by John Horigan
CC_INLINE void perspective_set_reflection_unit(double* p, double ux, double uy)
{
  perspective_set2(p, 2.0 * ux * ux - 1.0, 2.0 * ux * uy, 2.0 * ux * uy, 2.0 * uy * uy - 1.0, 0.0, 0.0);
}
//=================================================trans_reflection
// Reflection matrix. Reflect coordinates across the line through
// the origin at the angle a or containing the non-unit vector (x, y).
// Contributed by John Horigan
#define perspective_set_reflection(p, a) perspective_set_reflection_unit(p, cos(a), sin(a))
#define perspective_set_reflection2(p, x, y) perspective_set_reflection_unit(p, x / sqrt(x * x + y * y), y / sqrt(x * x + y * y))
CC_INLINE int perspective_rotate(double* p, double a)
{
  double t;
  perspective_set_rotation(&t, a);
  perspective_multiply(p, p, &t);
  return 0;
}
CC_INLINE int perspective_scale1(double* p, double x, double y)
{
  double mm0 = x; // Possible hint for the optimizer
  double mm3 = y;
  p[0] *= mm0;
  p[1] *= mm0;
  p[2] *= mm0;
  p[3] *= mm3;
  p[4] *= mm3;
  p[5] *= mm3;
  return 0;
}
CC_INLINE int perspective_scale2(double* p, double x, double y)
{
  double t;
  perspective_set_scaling2(&t, x, y);
  perspective_multiply(p, p, &t);
  return 0;
}
CC_INLINE int perspective_parl_to_parl(double* p, const double* src,
    const double* dst)
{
  double t;
  p[0] = src[2] - src[0];
  p[3] = src[3] - src[1];
  p[1] = src[4] - src[0];
  p[4] = src[5] - src[1];
  p[2] = src[0];
  p[5] = src[1];
  perspective_invert(p, p);
  perspective_set2(&t, dst[2] - dst[0], dst[3] - dst[1], dst[4] - dst[0], dst[5] - dst[1], dst[0], dst[1]);
  perspective_multiply(p, p, &t);
  return 0;
}
CC_INLINE int perspective_rect_to_parl(double* p, double x1, double y1,
    double x2, double y2,
    const double* parl)
{
  double src[6];
  src[0] = x1;
  src[1] = y1;
  src[2] = x2;
  src[3] = y1;
  src[4] = x2;
  src[5] = y2;
  perspective_parl_to_parl(p, src, parl);
  return 0;
}
CC_INLINE int perspective_parl_to_rect(double* p, const double* parl,    double x1, double y1,    double x2, double y2)
{
  double dst[6];
  dst[0] = x1;
  dst[1] = y1;
  dst[2] = x2;
  dst[3] = y1;
  dst[4] = x2;
  dst[5] = y2;
  perspective_parl_to_parl(p, parl, dst);
  return 0;
}
CC_INLINE int perspective_flip_x(double* p)
{
  p[0] = -p[0];
  p[3] = -p[3];
  p[2] = -p[2];
  return 0;
}
CC_INLINE int perspective_flip_y(double* p)
{
  p[1] = -p[1];
  p[4] = -p[4];
  p[5] = -p[5];
  return 0;
}
CC_INLINE int perspective_reset(double* p)
{
  p[0] = 1, p[3] = 0, p[6] = 0;
  p[1] = 0, p[4] = 1, p[7] = 0;
  p[2] = 0, p[5] = 0, p[8] = 1;
  return 0;
}
#define is_equal_eps(v1, v2, epsilon)  (fabs((v1) - (v2)) <= (double)(epsilon))
CC_INLINE BOOL perspective_is_identity(double* p, double epsilon)
{
  return is_equal_eps(p[0], 1.0, epsilon) &&
      is_equal_eps(p[3], 0.0, epsilon) &&
      is_equal_eps(p[6], 0.0, epsilon) &&
      is_equal_eps(p[1], 0.0, epsilon) &&
      is_equal_eps(p[4], 1.0, epsilon) &&
      is_equal_eps(p[7], 0.0, epsilon) &&
      is_equal_eps(p[2], 0.0, epsilon) &&
      is_equal_eps(p[5], 0.0, epsilon) &&
      is_equal_eps(p[8], 1.0, epsilon);
}
CC_INLINE BOOL perspective_is_valid(double* p, double epsilon)
{
  return fabs(p[0]) > epsilon && fabs(p[4]) > epsilon && fabs(p[8]) > epsilon;
}
CC_INLINE BOOL perspective_is_equal(double* p, const double* m, double epsilon)
{
  return is_equal_eps(p[0], m[0], epsilon) &&
      is_equal_eps(p[3], m[3], epsilon) &&
      is_equal_eps(p[6], m[6], epsilon) &&
      is_equal_eps(p[1], m[1], epsilon) &&
      is_equal_eps(p[4], m[4], epsilon) &&
      is_equal_eps(p[7], m[7], epsilon) &&
      is_equal_eps(p[2], m[2], epsilon) &&
      is_equal_eps(p[5], m[5], epsilon) &&
      is_equal_eps(p[8], m[8], epsilon);
}
CC_INLINE double perspective_rotation(double* p)
{
  double x1 = 0.0;
  double y1 = 0.0;
  double x2 = 1.0;
  double y2 = 0.0;
  perspective_transform(p, &x1, &y1);
  perspective_transform(p, &x2, &y2);
  return atan2(y2 - y1, x2 - x1);
}
CC_INLINE void perspective_translation(double* p, double* dx, double* dy)
{
  *dx = p[2];
  *dy = p[5];
}
CC_INLINE void perspective_scaling(double* p, double* x, double* y)
{
  double x1 = 0.0;
  double y1 = 0.0;
  double x2 = 1.0;
  double y2 = 1.0;
  double t2, t = *p;
  double r = perspective_rotation(p);
  perspective_set_rotation(&t2, -r);
  perspective_multiply(&t, &t, &t2);
  perspective_transform(&t, &x1, &y1);
  perspective_transform(&t, &x2, &y2);
  *x = x2 - x1;
  *y = y2 - y1;
}
#define perspective_premultiply(p, m)  perspective_multiply(p, m, p)
CC_INLINE int perspective_multiply_inv(double* p, const double* m)
{
  double t;
  perspective_invert(&t, m);
  perspective_multiply(p, p, &t);
  return 0;
}
CC_INLINE int perspective_premultiply_inv(double* p, const double* m)
{
  double t;
  perspective_invert(&t, m);
  perspective_multiply(p, &t, p);
  return 0;
}
CC_INLINE void perspective_scaling_abs(double* p, double* x, double* y)
{
  // Used to calculate scaling coefficients in image resampling.
  // When there is considerable shear this method gives us much
  // better estimation than just sx, sy.
  *x = sqrt(p[0] * p[0] + p[1] * p[1]);
  *y = sqrt(p[3] * p[3] + p[4] * p[4]);
}
CC_INLINE BOOL perspective_square_to_quad(double* p, const double* q)
{
  double dx = q[0] - q[2] + q[4] - q[6];
  double dy = q[1] - q[3] + q[5] - q[7];
  if (dx == 0.0 && dy == 0.0) {
    // Affine case (parallelogram)
    //
    p[0] = q[2] - q[0];
    p[3] = q[3] - q[1];
    p[6] = 0.0;
    p[1] = q[4] - q[2];
    p[4] = q[5] - q[3];
    p[7] = 0.0;
    p[2] = q[0];
    p[5] = q[1];
    p[8] = 1.0;
  }
  else {
    double u, v;
    double dx1 = q[2] - q[4];
    double dy1 = q[3] - q[5];
    double dx2 = q[6] - q[4];
    double dy2 = q[7] - q[5];
    double den = dx1 * dy2 - dx2 * dy1;
    if (den == 0.0) {
      // Singular case
      p[0] = p[3] = p[6] = p[1] = p[4] = p[7] = p[2] = p[5] = p[8] = 0.0;
      return FALSE;
    }
    // General case
    //
    u = (dx * dy2 - dy * dx2) / den;
    v = (dy * dx1 - dx * dy1) / den;
    p[0] = q[2] - q[0] + u * q[2];
    p[3] = q[3] - q[1] + u * q[3];
    p[6] = u;
    p[1] = q[6] - q[0] + v * q[6];
    p[4] = q[7] - q[1] + v * q[7];
    p[7] = v;
    p[2] = q[0];
    p[5] = q[1];
    p[8] = 1.0;
  }
  return TRUE;
}
CC_INLINE BOOL perspective_quad_to_square(double* p, const double* q)
{
  if (!perspective_square_to_quad(p, q)) {
    return FALSE;
  }
  perspective_invert(p, p);
  return TRUE;
}
CC_INLINE BOOL perspective_quad_to_quad(double* p0, const double* qs, const double* qd)
{
  double p;
  if (!perspective_quad_to_square(p0, qs)) {
    return FALSE;
  }
  if (!perspective_square_to_quad(&p, qd)) {
    return FALSE;
  }
  perspective_multiply(p0, p0, &p);
  return TRUE;
}
CC_INLINE BOOL perspective_rect_to_quad(double* p, double x1, double y1,
    double x2, double y2,
    const double* q)
{
  double r[8];
  r[0] = r[6] = x1;
  r[2] = r[4] = x2;
  r[1] = r[3] = y1;
  r[5] = r[7] = y2;
  return perspective_quad_to_quad(p, r, q);
}
CC_INLINE BOOL perspective_quad_to_rect(double* p, const double* q,
    double x1, double y1,
    double x2, double y2)
{
  double r[8];
  r[0] = r[6] = x1;
  r[2] = r[4] = x2;
  r[1] = r[3] = y1;
  r[5] = r[7] = y2;
  return perspective_quad_to_quad(p, q, r);
}
CC_INLINE int perspective_scale(double* p, double s)
{
  double t;
  perspective_set_scaling(&t, s);
  perspective_multiply(p, p, &t);
  return 0;
}
CC_INLINE void perspective_store_to(double* p, double* m)
{
  *m++ = p[0], *m++ = p[3], *m++ = p[6];
  *m++ = p[1], *m++ = p[4], *m++ = p[7];
  *m++ = p[2], *m++ = p[5], *m++ = p[8];
}
CC_INLINE int perspective_load_from(double* p, const double* m)
{
  p[0] = *m++, p[3] = *m++, p[6] = *m++;
  p[1] = *m++, p[4] = *m++, p[7] = *m++;
  p[2] = *m++, p[5] = *m++, p[8] = *m++;
  return 0;
}
CC_INLINE double perspective_get_rotation(double* p)
{
  double x1 = 0.0;
  double y1 = 0.0;
  double x2 = 1.0;
  double y2 = 0.0;
  perspective_transform(p, &x1, &y1);
  perspective_transform(p, &x2, &y2);
  return atan2(y2 - y1, x2 - x1);
}
CC_INLINE void perspective_get_translation(double* p, double* dx, double* dy)
{
  *dx = p[2];
  *dy = p[5];
}
CC_INLINE void perspective_get_scaling(double* p, double* x, double* y)
{
  double x1 = 0.0;
  double y1 = 0.0;
  double x2 = 1.0;
  double y2 = 1.0;
  double t2, t = (*p);
  double r = perspective_get_rotation(p);
  perspective_set_rotation(&t2, -r);
  perspective_multiply(&t, &t, &t2);
  perspective_transform(&t, &x1, &y1);
  perspective_transform(&t, &x2, &y2);
  *x = x2 - x1;
  *y = y2 - y1;
}
CC_INLINE void perspective_get_scaling_abs(double* p, double* x, double* y)
{
  *x = sqrt(p[0] * p[0] + p[1] * p[1]);
  *y = sqrt(p[3] * p[3] + p[4] * p[4]);
}
CC_INLINE int fvec_set(float* v, float x, float y, float z)
{
  v[0] = x, v[1] = y, v[2] = z;
  return 0;
}
CC_INLINE int fvec_setv(float* v1, const float* v2)
{
  v1[0] = v2[0], v1[1] = v2[1], v1[2] = v2[2];
  return 0;
}
CC_INLINE float fvec_sqrlen(const float* v)
{
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
CC_INLINE float fvec_len(const float* v)
{
  return (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
CC_INLINE int fvec_normalize(float* v, const float* v1)
{
  float inv = 1 / fvec_len(v1);
  return fvec_set(v, v1[0] * inv, v1[1] * inv, v1[2] * inv);
}
CC_INLINE int fvec_negate(float* v, const float* v1)
{
  return fvec_set(v, -v1[0], -v1[1], -v1[2]);
}
CC_INLINE int fvec_add(float* v, const float* v1, const float* v2)
{
  return fvec_set(v, v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}
CC_INLINE int fvec_sub(float* v, const float* v1, const float* v2)
{
  return fvec_set(v, v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}
CC_INLINE int fvec_muls(float* v, const float* v1, float f)
{
  return fvec_set(v, v1[0] * f, v1[1] * f, v1[2] * f);
}
CC_INLINE float fvec_dot(const float* v1, const float* v2)
{
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
CC_INLINE int fvec_cross(float* v, const float* v1, const float* v2)
{
  return fvec_set(v, -v1[2] * v2[1] + v1[1] * v2[2], v1[2] * v2[0] - v1[0] * v2[2], -v1[1] * v2[0] + v1[0] * v2[1]);
}
CC_INLINE int fvec_zero(float* v)
{
  return fvec_set(v, 0, 0, 0);
}
CC_INLINE int fvec_getpoint(float* v, const float* v1, const float* v2, float t)
{
  return fvec_set(v, v1[0] + v2[0] * t, v1[1] + v2[1] * t, v1[2] + v2[2] * t);
}
#define SETRGB(rgb, r, g, b)  (rgb[0]=(uchar)(r), rgb[1]=(uchar)(g), rgb[2]=(uchar)(b))
#define setrgb(rgb, r, g, b)  (rgb[0]=(uchar)(r*255), rgb[1]=(uchar)(g*255), rgb[2]=(uchar)(b*255))
CC_INLINE int fvec_setrgb(float* v, COLOR clr)
{
  v[0] = GetRV(clr) / 255.f;
  v[1] = GetGV(clr) / 255.f;
  v[2] = GetBV(clr) / 255.f;
  return 0;
}
CC_INLINE int fvec_modulate(float* v, const float* v1, const float* v2)
{
  return fvec_set(v, v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}
/****************************************************************************************\
***
\****************************************************************************************/
#define RAD(x) ((x)*PI/180.0f)
#define DEG(x) ((x)*180.0f/PI)
#define RAD2DEG(x)  ((x)*180.f/PI);
#define DEG2RAD(x)  ((x)*PI / 180.0f);
#define VT(_V, _I)  (_V)[(_I)]
#define VX(_V)  (_V)[0]
#define VY(_V)  (_V)[1]
#define VZ(_V)  (_V)[2]
#define VW(_V)  (_V)[3]
#if 0
#define V0(_V)  (_V)[0]
#define V1(_V)  (_V)[1]
#define V2(_V)  (_V)[2]
#define V3(_V)  (_V)[3]
#endif
//#define V2SET(_V,A0,A1)     (V0(_V)=A0,V1(_V)=A1)
#define V2FSET(_V,i,A0,A1)  V2SET((_V)+i*2,A0,A1)
//#define V3SET(_V,A0,A1,A2)     (V0(_V)=A0,V1(_V)=A1,V2(_V)=A2)
#define V3FSET(_V,i,A0,A1,A2)  V3SET((_V)+i*3,A0,A1,A2)
//#define V4SET(_V,A0,A1,A2,A3)     (V0(_V)=A0,V1(_V)=A1,V2(_V)=A2,V3(_V)=A3)
#define V4FSET(_V,i,A0,A1,A2,A3)  V4SET((_V)+i*4,A0,A1,A2,A3)
#define MV2PUSH(_V,A0,A1)     ((_V)[0]=2,V2SET(_V+1,A0,A1),(_V)+=3)
#define MV3PUSH(_V,A0,A1,A2)  ((_V)[0]=3,V3SET(_V+1,A0,A1,A2),(_V)+=4)
#define MV4PUSH(_V,A0,A1,A2,A3) ((_V)[0]=4,V4SET(_V+1,A0,A1,A2,A3),(_V)+=5)
// +-
#define V2ADD(c, a, b)    (V0(c)=V0(a)+V0(b), V1(c)=V1(a)+V1(b))
#define V3ADD(c, a, b)    (V2ADD(c, a, b), V2(c)=V2(a)+V2(b))
#define V4ADD(c, a, b)    (V3ADD(c, a, b), V3(c)=V3(a)+V3(b))
#define V2SUB(c, a, b)    (V0(c)=V0(a)-V0(b), V1(c)=V1(a)-V1(b))
#define V3SUB(c, a, b)    (V2SUB(c, a, b), V2(c)=V2(a)-V2(b))
#define V4SUB(c, a, b)    (V3SUB(c, a, b), V3(c)=V3(a)-V3(b))
//#define V2DOT(a, b)   (V0(a)*V0(b) + V1(a)*V1(b))
//#define V3DOT(a, b)   (V2DOT(a, b) + V2(a)*V2(b))
//#define V4DOT(a, b)   (V3DOT(a, b) + V3(a)*V3(b))
#define V3MAG(a)      sqrt(V3DOT(a, a))
#define V4MAG(a)      sqrt(V4DOT(a, a))
// Cross product
#define V3CROSS(c, a, b)   V3SET(c, (V1(a) * V2(b)) - (V2(a) * V1(b)) , (V2(a) * V0(b)) - (V0(a) * V2(b)) , (V0(a) * V1(b)) - (V1(a) * V0(b)))
#define V4CROSS(c, a, b)   V4SET(c, (V1(a) * V2(b)) - (V2(a) * V1(b)) , (V2(a) * V0(b)) - (V0(a) * V2(b)) , (V0(a) * V1(b)) - (V1(a) * V0(b)), 1.f)
#define M3SET(_C,A00,A01,A02,A10,A11,A12,A20,A21,A22) \
  M300(_C) = A00,M301(_C) = A01,M302(_C) = A02, \
      M310(_C) = A10,M311(_C) = A11,M312(_C) = A12, \
          M320(_C) = A20,M321(_C) = A21,M322(_C) = A22
//??λ????
//#define M3ID(_C)   M3SET(_C, 1.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f)
#define M4MULM4(c, a, b) {float tmp[16]; M4MUL(tmp, a, b); M4SETV(c, tmp);}
#define M4MULM4(c, a, b) {float tmp[16]; M4MUL(tmp, a, b); M4SETV(c, tmp);}
#if 0
BOOL m4_inv(const float* s, float* d)
{
  float fDet, tmp[12];
  tmp[0] = M422(s) * M433(s), tmp[1] = M432(s) * M423(s), tmp[2] = M412(s) * M433(s), tmp[3] = M413(s) * M432(s);
  tmp[4] = M412(s) * M423(s), tmp[5] = M422(s) * M413(s), tmp[6] = M402(s) * M433(s), tmp[7] = M432(s) * M403(s);
  tmp[8] = M402(s) * M423(s), tmp[9] = M422(s) * M403(s), tmp[10] = M402(s) * M413(s), tmp[11] = M412(s) * M403(s);  // # 12*
  // ????????? (???????????
  M400(d) = M411(s) * (tmp[0] - tmp[1]) + M421(s) * (tmp[3] - tmp[2]) + M431(s) * (tmp[4] - tmp[5]); // 3* 3- 2+
  M401(d) = M401(s) * (tmp[1] - tmp[0]) + M421(s) * (tmp[6] - tmp[7]) + M431(s) * (tmp[9] - tmp[8]);
  M402(d) = M401(s) * (tmp[2] - tmp[3]) + M411(s) * (tmp[7] - tmp[6]) + M431(s) * (tmp[10] - tmp[11]);
  M403(d) = M401(s) * (tmp[5] - tmp[4]) + M411(s) * (tmp[8] - tmp[9]) + M421(s) * (tmp[11] - tmp[10]);
  M410(d) = M410(s) * (tmp[1] - tmp[0]) + M420(s) * (tmp[2] - tmp[3]) + M430(s) * (tmp[5] - tmp[4]);
  M411(d) = M400(s) * (tmp[0] - tmp[1]) + M420(s) * (tmp[7] - tmp[6]) + M430(s) * (tmp[8] - tmp[9]);
  M412(d) = M400(s) * (tmp[3] - tmp[2]) + M410(s) * (tmp[6] - tmp[7]) + M430(s) * (tmp[11] - tmp[10]);
  M413(d) = M400(s) * (tmp[4] - tmp[5]) + M410(s) * (tmp[9] - tmp[8]) + M420(s) * (tmp[10] - tmp[11]);
  //36* 24- 16+
  tmp[0] = M420(s) * M31(s), tmp[1] = M430(s) * M421(s), tmp[2] = M410(s) * M431(s), tmp[3] = M430(s) * M411(s);
  tmp[4] = M410(s) * M21(s), tmp[5] = M420(s) * M411(s), tmp[6] = M400(s) * M431(s), tmp[7] = M430(s) * M401(s);
  tmp[8] = M400(s) * M21(s), tmp[9] = M420(s) * M401(s), tmp[10] = M400(s) * M411(s), tmp[11] = M410(s) * M401(s);
  M420(d) = M413(s) * (tmp[0] - tmp[1]) + M423(s) * (tmp[3] - tmp[2]) + M433(s) * (tmp[4] - tmp[5]),
  M421(d) = M403(s) * (tmp[1] - tmp[0]) + M423(s) * (tmp[6] - tmp[7]) + M433(s) * (tmp[9] - tmp[8]),
  M422(d) = M403(s) * (tmp[2] - tmp[3]) + M412(s) * (tmp[7] - tmp[6]) + M433(s) * (tmp[10] - tmp[11]),
  M423(d) = M403(s) * (tmp[5] - tmp[4]) + M413(s) * (tmp[8] - tmp[9]) + M423(s) * (tmp[11] - tmp[10]),
  M430(d) = M412(s) * (tmp[1] - tmp[0]) + M422(s) * (tmp[2] - tmp[3]) + M432(s) * (tmp[5] - tmp[4]),
  M431(d) = M422(s) * (tmp[7] - tmp[6]) + M402(s) * (tmp[0] - tmp[1]) + M432(s) * (tmp[8] - tmp[9]),
  M432(d) = M412(s) * (tmp[6] - tmp[7]) + M402(s) * (tmp[3] - tmp[2]) + M432(s) * (tmp[11] - tmp[10]),
  M433(d) = M402(s) * (tmp[4] - tmp[5]) + M422(s) * (tmp[10] - tmp[11]) + M412(s) * (tmp[9] - tmp[8]);
  // 72* 48- 32+
  fDet = (M400(s) * M400(d) + M410(s) * M401(d) + M420(s) * M402(d) + M430(s) * M403(d)); // 4* 3+
  if (fDet < 0.000001f && fDet > -0.000001f) {
    M4ID(d);
    return FALSE;
  }
  fDet = 1.0f / fDet,
  M400(d) *= fDet, M401(d) *= fDet, M402(d) *= fDet, M403(d) *= fDet;
  M410(d) *= fDet, M411(d) *= fDet, M412(d) *= fDet, M413(d) *= fDet;
  M420(d) *= fDet, M421(d) *= fDet, M422(d) *= fDet, M423(d) *= fDet;
  M430(d) *= fDet, M431(d) *= fDet, M432(d) *= fDet, M433(d) *= fDet; //16*
  return TRUE;
}
// Transform a vector by the matrix
#define M4MULV4(m, v, o)  \
  V0(o) = V0(v) * M400(m) + V1(v) * M401(m) + V2(v) * M402(m) + V3(v) * M403(m), \
      V1(o) = V0(v) * M410(m) + V1(v) * M411(m) + V2(v) * M412(m) + V3(v) * M413(m), \
          V2(o) = V0(v) * M420(m) + V1(v) * M421(m) + V2(v) * M422(m) + V3(v) * M423(m), \
              V3(o) = V0(v) * M430(m) + V1(v) * M431(m) + V2(v) * M432(m) + V3(v) * M433(m)
#define M4MULV3(m, v, o)  \
  V0(o) = V0(v) * M400(m) + V1(v) * M401(m) + V2(v) * M402(m) + M403(m), \
      V1(o) = V0(v) * M410(m) + V1(v) * M411(m) + V2(v) * M412(m) + M413(m), \
          V2(o) = V0(v) * M420(m) + V1(v) * M421(m) + V2(v) * M422(m) + M423(m), \
              V3(o) = V0(v) * M430(m) + V1(v) * M431(m) + V2(v) * M432(m) + M433(m)
#endif

#ifdef _MSC_VER
#if _MSC_VER < 1300
#include "math_vc6.h"
#endif
#endif



#endif // _CMATH_INL_

