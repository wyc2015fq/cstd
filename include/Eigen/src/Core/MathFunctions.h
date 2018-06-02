// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2006-2010 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EIGEN_MATHFUNCTIONS_H
#define EIGEN_MATHFUNCTIONS_H

// source: http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html
// TODO this should better be moved to NumTraits
#define EIGEN_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406L


// On WINCE, std::abs is defined for int only, so let's defined our own overloads:
// This issue has been confirmed with MSVC 2008 only, but the issue might exist for more recent versions too.
#if EIGEN_OS_WINCE && EIGEN_COMP_MSVC && EIGEN_COMP_MSVC<=1500
//long        abs(long        x) { return (labs(x));  }
#if ISDOUBLE
double      abs(double      x) { return (fabs(x));  }
#else
float       abs(float       x) { return (fabsf(x)); }
#endif
//long double abs(long double x) { return (fabsl(x)); }
#endif



/****************************************************************************
* Implementation of hypot                                                *
****************************************************************************/

static inline T hypot_impl(const T& x, const T& y)
{
  T _x = abs(x);
  T _y = abs(y);
  T p, qp;
  if(_x>_y)
  {
    p = _x;
    qp = _y / p;
  }
  else
  {
    p = _y;
    qp = _x / p;
  }
  if(p==(T)(0)) return (T)(0);
  return p * sqrt((T)(1) + qp*qp);
}



/****************************************************************************
* Implementation of round                                                   *
****************************************************************************/

static inline T round(const T& x)
{
  return (x > (T)(0)) ? floor(x + T(0.5)) : ceil(x - T(0.5));
}

EIGEN_DEVICE_FUNC inline Scalar log1p(const Scalar& x) {
  EIGEN_STATIC_ASSERT_NON_INTEGER(Scalar)
  EIGEN_USING_STD_MATH(log);
  Scalar x1p = RealScalar(1) + x;
  return ( x1p == Scalar(1) ) ? x : x * ( log(x1p) / (x1p - RealScalar(1)) );
}

/****************************************************************************
* Implementation of pow                                                  *
****************************************************************************/

static EIGEN_DEVICE_FUNC inline Scalar ipow(Scalar x, int y)
{
  Scalar res(1);
  eigen_assert(y >= 0);
  if(y & 1) res *= x;
  y >>= 1;
  while(y)
  {
    x *= x;
    if(y&1) res *= x;
    y >>= 1;
  }
  return res;
}


/****************************************************************************
* Implementation of random                                               *
****************************************************************************/

static inline Scalar random(const Scalar& x, const Scalar& y)
{
  return x + (y-x) * Scalar(rand()) / Scalar(RAND_MAX);
}
static inline Scalar random()
{
  return random(Scalar(IsSigned ? -1 : 0), Scalar(1));
}


enum {
  meta_floor_log2_terminate,
  meta_floor_log2_move_up,
  meta_floor_log2_move_down,
  meta_floor_log2_bogus
};

template<unsigned int n, int lower, int upper> struct meta_floor_log2_selector
{
  enum { middle = (lower + upper) / 2,
         value = (upper <= lower + 1) ? int(meta_floor_log2_terminate)
               : (n < (1 << middle)) ? int(meta_floor_log2_move_down)
               : (n==0) ? int(meta_floor_log2_bogus)
               : int(meta_floor_log2_move_up)
  };
};

// std::is* do not work with fast-math and gcc, std::is* are available on MSVC 2013 and newer, as well as in clang.
#if (EIGEN_HAS_CXX11_MATH && !(EIGEN_COMP_GNUC_STRICT && __FINITE_MATH_ONLY__)) || (EIGEN_COMP_MSVC>=1800) || (EIGEN_COMP_CLANG)
#define EIGEN_USE_STD_FPCLASSIFY 1
#else
#define EIGEN_USE_STD_FPCLASSIFY 0
#endif

int isfinite(const T& x)
{
  #ifdef __CUDA_ARCH__
    return isfinite(x);
  #elif EIGEN_USE_STD_FPCLASSIFY
    return isfinite(x);
  #else
    return x<=highest() && x>=lowest();
  #endif
}
int isinf(const T& x)
{
  #ifdef __CUDA_ARCH__
    return (isinf)(x);
  #elif EIGEN_USE_STD_FPCLASSIFY
    return isinf EIGEN_NOT_A_MACRO (x);
  #else
    return x>highest() || x<lowest();
  #endif
}
int isnan(const T& x)
{
  #ifdef __CUDA_ARCH__
    return (::isnan)(x);
  #elif EIGEN_USE_STD_FPCLASSIFY
    using std::isnan;
    return isnan EIGEN_NOT_A_MACRO (x);
  #else
    return x != x;
  #endif
}

#if (!EIGEN_USE_STD_FPCLASSIFY)

#if EIGEN_COMP_MSVC

int isinf_msvc_helper(T x)
{
  return _fpclass(x)==_FPCLASS_NINF || _fpclass(x)==_FPCLASS_PINF;
}

//MSVC defines a _isnan builtin function, but for double only
EIGEN_DEVICE_FUNC inline bool isnan_impl(const long double& x) { return _isnan(x)!=0; }
EIGEN_DEVICE_FUNC inline bool isnan_impl(const double& x)      { return _isnan(x)!=0; }
EIGEN_DEVICE_FUNC inline bool isnan_impl(const float& x)       { return _isnan(x)!=0; }

EIGEN_DEVICE_FUNC inline bool isinf_impl(const long double& x) { return isinf_msvc_helper(x); }
EIGEN_DEVICE_FUNC inline bool isinf_impl(const double& x)      { return isinf_msvc_helper(x); }
EIGEN_DEVICE_FUNC inline bool isinf_impl(const float& x)       { return isinf_msvc_helper(x); }

#elif (defined __FINITE_MATH_ONLY__ && __FINITE_MATH_ONLY__ && EIGEN_COMP_GNUC)

#if EIGEN_GNUC_AT_LEAST(5,0)
  #define EIGEN_TMP_NOOPT_ATTRIB EIGEN_DEVICE_FUNC inline __attribute__((optimize("no-finite-math-only")))
#else
  // NOTE the inline qualifier and noinline attribute are both needed: the former is to avoid linking issue (duplicate symbol),
  //      while the second prevent too aggressive optimizations in fast-math mode:
  #define EIGEN_TMP_NOOPT_ATTRIB EIGEN_DEVICE_FUNC inline __attribute__((noinline,optimize("no-finite-math-only")))
#endif

EIGEN_TMP_NOOPT_ATTRIB bool isnan_impl(const long double& x) { return __builtin_isnan(x); }
EIGEN_TMP_NOOPT_ATTRIB bool isnan_impl(const double& x)      { return __builtin_isnan(x); }
EIGEN_TMP_NOOPT_ATTRIB bool isnan_impl(const float& x)       { return __builtin_isnan(x); }
EIGEN_TMP_NOOPT_ATTRIB bool isinf_impl(const double& x)      { return __builtin_isinf(x); }
EIGEN_TMP_NOOPT_ATTRIB bool isinf_impl(const float& x)       { return __builtin_isinf(x); }
EIGEN_TMP_NOOPT_ATTRIB bool isinf_impl(const long double& x) { return __builtin_isinf(x); }

#undef EIGEN_TMP_NOOPT_ATTRIB

#endif

#endif



/****************************************************************************
* Generic math functions                                                    *
****************************************************************************/

namespace numext {

#ifndef __CUDA_ARCH__

EIGEN_DEVICE_FUNC
EIGEN_ALWAYS_INLINE T mini(const T& x, const T& y)
{
  EIGEN_USING_STD_MATH(MIN);
  return MIN(x,y);
}


EIGEN_DEVICE_FUNC
EIGEN_ALWAYS_INLINE T maxi(const T& x, const T& y)
{
  EIGEN_USING_STD_MATH(max);
  return MAX(x,y);
}
#else

EIGEN_DEVICE_FUNC
EIGEN_ALWAYS_INLINE T mini(const T& x, const T& y)
{
  return y < x ? y : x;
}
template<>
EIGEN_DEVICE_FUNC
EIGEN_ALWAYS_INLINE float mini(const float& x, const float& y)
{
  return fminf(x, y);
}

EIGEN_DEVICE_FUNC
EIGEN_ALWAYS_INLINE T maxi(const T& x, const T& y)
{
  return x < y ? y : x;
}
template<>
EIGEN_DEVICE_FUNC
EIGEN_ALWAYS_INLINE float maxi(const float& x, const float& y)
{
  return fmaxf(x, y);
}
#endif

/** Log base 2 for 32 bits positive integers.
  * Conveniently returns 0 for x==0. */
inline int log2(int x)
{
  eigen_assert(x>=0);
  unsigned int v(x);
  static const int table[32] = { 0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30, 8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31 };
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  return table[(v * 0x07C4ACDDU) >> 27];
}

/****************************************************************************
* Implementation of fuzzy comparisons                                       *
****************************************************************************/


static inline bool isMuchSmallerThan(const Scalar& x, const Scalar& y, const Scalar& prec)
{
  return abs(x) <= abs(y) * prec;
}

static inline bool isApprox(const Scalar& x, const Scalar& y, const Scalar& prec)
{
  return abs(x - y) <= mini(abs(x), abs(y)) * prec;
}

static inline bool isApproxOrLessThan(const Scalar& x, const Scalar& y, const Scalar& prec)
{
  return x <= y || isApprox(x, y, prec);
}


#endif // EIGEN_MATHFUNCTIONS_H
