

#ifndef OPENCC_CORE_FAST_MATH_HPP
#define OPENCC_CORE_FAST_MATH_HPP

#include "opencv2/core/cvdef.h"

//! @addtogroup core_utils
//! @{

/****************************************************************************************\
*                                      fast math                                         *
\****************************************************************************************/

#if defined __BORLANDC__
#  include <fastmath.h>
#elif defined __cplusplus
#  include <cmath>
#else
#  include <math.h>
#endif

#ifdef HAVE_TEGRA_OPTIMIZATION
#  include "tegra_round.hpp"
#endif

#if CC_VFP
    // 1. general scheme
    #define ARM_ROUND(_value, _asm_string) \
        int res; \
        float temp; \
        (void)temp; \
        asm(_asm_string : [res] "=r" (res), [temp] "=w" (temp) : [value] "w" (_value)); \
        return res
    // 2. version for double
    #ifdef __clang__
        #define ARM_ROUND_DBL(value) ARM_ROUND(value, "vcvtr.s32.f64 %[temp], %[value] \n vmov %[res], %[temp]")
    #else
        #define ARM_ROUND_DBL(value) ARM_ROUND(value, "vcvtr.s32.f64 %[temp], %P[value] \n vmov %[res], %[temp]")
    #endif
    // 3. version for float
    #define ARM_ROUND_FLT(value) ARM_ROUND(value, "vcvtr.s32.f32 %[temp], %[value]\n vmov %[res], %[temp]")
#endif // CC_VFP

/** @brief Rounds floating-point number to the nearest integer

 @param value floating-point number. If the value is outside of INT_MIN ... INT_MAX range, the
 result is not defined.
 */
CC_INLINE int
cvRound( double value )
{
#if ((defined _MSC_VER && defined _M_X64) || (defined __GNUC__ && defined __x86_64__ \
    && defined __SSE2__ && !defined __APPLE__)) && !defined(__CUDACC__)
    __m128d t = _mm_set_sd( value );
    return _mm_cvtsd_si32(t);
#elif defined _MSC_VER && defined _M_IX86
    int t;
    __asm
    {
        fld value;
        fistp t;
    }
    return t;
#elif ((defined _MSC_VER && defined _M_ARM) || defined CC_ICC || \
        defined __GNUC__) && defined HAVE_TEGRA_OPTIMIZATION
    TEGRA_ROUND_DBL(value);
#elif defined CC_ICC || defined __GNUC__
# if CC_VFP
    ARM_ROUND_DBL(value);
# else
    return (int)lrint(value);
# endif
#else
    /* it's ok if round does not comply with IEEE754 standard;
       the tests should allow +/-1 difference when the tested functions use round */
    return (int)(value + (value >= 0 ? 0.5 : -0.5));
#endif
}


/** @brief Rounds floating-point number to the nearest integer not larger than the original.

 The function computes an integer i such that:
 \f[i \le \texttt{value} < i+1\f]
 @param value floating-point number. If the value is outside of INT_MIN ... INT_MAX range, the
 result is not defined.
 */
CC_INLINE int cvFloor( double value )
{
#if (defined _MSC_VER && defined _M_X64 || (defined __GNUC__ && defined __SSE2__ && !defined __APPLE__)) && !defined(__CUDACC__)
    __m128d t = _mm_set_sd( value );
    int i = _mm_cvtsd_si32(t);
    return i - _mm_movemask_pd(_mm_cmplt_sd(t, _mm_cvtsi32_sd(t,i)));
#elif defined __GNUC__
    int i = (int)value;
    return i - (i > value);
#else
    int i = cvRound(value);
    float diff = (float)(value - i);
    return i - (diff < 0);
#endif
}

/** @brief Rounds floating-point number to the nearest integer not smaller than the original.

 The function computes an integer i such that:
 \f[i \le \texttt{value} < i+1\f]
 @param value floating-point number. If the value is outside of INT_MIN ... INT_MAX range, the
 result is not defined.
 */
CC_INLINE int cvCeil( double value )
{
#if (defined _MSC_VER && defined _M_X64 || (defined __GNUC__ && defined __SSE2__&& !defined __APPLE__)) && !defined(__CUDACC__)
    __m128d t = _mm_set_sd( value );
    int i = _mm_cvtsd_si32(t);
    return i + _mm_movemask_pd(_mm_cmplt_sd(_mm_cvtsi32_sd(t,i), t));
#elif defined __GNUC__
    int i = (int)value;
    return i + (i < value);
#else
    int i = cvRound(value);
    float diff = (float)(i - value);
    return i + (diff < 0);
#endif
}

/** @brief Determines if the argument is Not A Number.

 @param value The input floating-point value

 The function returns 1 if the argument is Not A Number (as defined by IEEE754 standard), 0
 otherwise. */
CC_INLINE int cvIsNaN( double value )
{
    suf64_t ieee754;
    ieee754.f = value;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) +
           ((unsigned)ieee754.u != 0) > 0x7ff00000;
}

/** @brief Determines if the argument is Infinity.

 @param value The input floating-point value

 The function returns 1 if the argument is a plus or minus infinity (as defined by IEEE754 standard)
 and 0 otherwise. */
CC_INLINE int cvIsInf( double value )
{
    suf64_t ieee754;
    ieee754.f = value;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) == 0x7ff00000 &&
            (unsigned)ieee754.u == 0;
}

#ifdef __cplusplus

/** @overload */
CC_INLINE int cvRound(float value)
{
#if ((defined _MSC_VER && defined _M_X64) || (defined __GNUC__ && defined __x86_64__ && \
      defined __SSE2__ && !defined __APPLE__)) && !defined(__CUDACC__)
    __m128 t = _mm_set_ss( value );
    return _mm_cvtss_si32(t);
#elif defined _MSC_VER && defined _M_IX86
    int t;
    __asm
    {
        fld value;
        fistp t;
    }
    return t;
#elif ((defined _MSC_VER && defined _M_ARM) || defined CC_ICC || \
        defined __GNUC__) && defined HAVE_TEGRA_OPTIMIZATION
    TEGRA_ROUND_FLT(value);
#elif defined CC_ICC || defined __GNUC__
# if CC_VFP
    ARM_ROUND_FLT(value);
# else
    return (int)lrintf(value);
# endif
#else
    /* it's ok if round does not comply with IEEE754 standard;
     the tests should allow +/-1 difference when the tested functions use round */
    return (int)(value + (value >= 0 ? 0.5f : -0.5f));
#endif
}

/** @overload */
CC_INLINE int cvRound( int value )
{
    return value;
}

/** @overload */
CC_INLINE int cvFloor( float value )
{
#if (defined _MSC_VER && defined _M_X64 || (defined __GNUC__ && defined __SSE2__ && !defined __APPLE__)) && !defined(__CUDACC__)
    __m128 t = _mm_set_ss( value );
    int i = _mm_cvtss_si32(t);
    return i - _mm_movemask_ps(_mm_cmplt_ss(t, _mm_cvtsi32_ss(t,i)));
#elif defined __GNUC__
    int i = (int)value;
    return i - (i > value);
#else
    int i = cvRound(value);
    float diff = (float)(value - i);
    return i - (diff < 0);
#endif
}

/** @overload */
CC_INLINE int cvFloor( int value )
{
    return value;
}

/** @overload */
CC_INLINE int cvCeil( float value )
{
#if (defined _MSC_VER && defined _M_X64 || (defined __GNUC__ && defined __SSE2__&& !defined __APPLE__)) && !defined(__CUDACC__)
    __m128 t = _mm_set_ss( value );
    int i = _mm_cvtss_si32(t);
    return i + _mm_movemask_ps(_mm_cmplt_ss(_mm_cvtsi32_ss(t,i), t));
#elif defined __GNUC__
    int i = (int)value;
    return i + (i < value);
#else
    int i = cvRound(value);
    float diff = (float)(i - value);
    return i + (diff < 0);
#endif
}

/** @overload */
CC_INLINE int cvCeil( int value )
{
    return value;
}

/** @overload */
CC_INLINE int cvIsNaN( float value )
{
    suf32_t ieee754;
    ieee754.f = value;
    return (ieee754.u & 0x7fffffff) > 0x7f800000;
}

/** @overload */
CC_INLINE int cvIsInf( float value )
{
    suf32_t ieee754;
    ieee754.f = value;
    return (ieee754.u & 0x7fffffff) == 0x7f800000;
}

#endif // __cplusplus

//! @} core_utils

#endif
