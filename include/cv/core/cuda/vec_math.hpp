

#ifndef OPENCC_CUDA_VECMATH_HPP
#define OPENCC_CUDA_VECMATH_HPP

#include "vec_traits.hpp"
#include "saturate_cast.hpp"

/** @file
 * @deprecated Use @ref cudev instead.
 */

//! @cond IGNORED

namespace cv { namespace cuda { namespace device
{

// saturate_cast

namespace vec_math_detail
{
    template <int cn, typename VecD> struct SatCastHelper;
    template <typename VecD> struct SatCastHelper<1, VecD>
    {
        template <typename VecS> static __device__ __forceinline__ VecD cast(const VecS& v)
        {
            typedef typename VecTraits<VecD>::elem_type D;
            return VecTraits<VecD>::make(saturate_cast<D>(v.x));
        }
    };
    template <typename VecD> struct SatCastHelper<2, VecD>
    {
        template <typename VecS> static __device__ __forceinline__ VecD cast(const VecS& v)
        {
            typedef typename VecTraits<VecD>::elem_type D;
            return VecTraits<VecD>::make(saturate_cast<D>(v.x), saturate_cast<D>(v.y));
        }
    };
    template <typename VecD> struct SatCastHelper<3, VecD>
    {
        template <typename VecS> static __device__ __forceinline__ VecD cast(const VecS& v)
        {
            typedef typename VecTraits<VecD>::elem_type D;
            return VecTraits<VecD>::make(saturate_cast<D>(v.x), saturate_cast<D>(v.y), saturate_cast<D>(v.z));
        }
    };
    template <typename VecD> struct SatCastHelper<4, VecD>
    {
        template <typename VecS> static __device__ __forceinline__ VecD cast(const VecS& v)
        {
            typedef typename VecTraits<VecD>::elem_type D;
            return VecTraits<VecD>::make(saturate_cast<D>(v.x), saturate_cast<D>(v.y), saturate_cast<D>(v.z), saturate_cast<D>(v.w));
        }
    };

    template <typename VecD, typename VecS> static __device__ __forceinline__ VecD saturate_cast_helper(const VecS& v)
    {
        return SatCastHelper<VecTraits<VecD>::cn, VecD>::cast(v);
    }
}

template<typename T> static __device__ __forceinline__ T saturate_cast(const uchar1& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const char1& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const ushort1& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const short1& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const uint1& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const int1& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const float1& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const double1& v) {return vec_math_detail::saturate_cast_helper<T>(v);}

template<typename T> static __device__ __forceinline__ T saturate_cast(const uchar2& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const char2& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const ushort2& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const short2& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const uint2& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const int2& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const float2& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const double2& v) {return vec_math_detail::saturate_cast_helper<T>(v);}

template<typename T> static __device__ __forceinline__ T saturate_cast(const uchar3& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const char3& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const ushort3& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const short3& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const uint3& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const int3& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const float3& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const double3& v) {return vec_math_detail::saturate_cast_helper<T>(v);}

template<typename T> static __device__ __forceinline__ T saturate_cast(const uchar4& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const char4& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const ushort4& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const short4& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const uint4& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const int4& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const float4& v) {return vec_math_detail::saturate_cast_helper<T>(v);}
template<typename T> static __device__ __forceinline__ T saturate_cast(const double4& v) {return vec_math_detail::saturate_cast_helper<T>(v);}

// unary operators

#define CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(op, input_type, output_type) \
    __device__ __forceinline__ output_type ## 1 operator op(const input_type ## 1 & a) \
    { \
        return VecTraits<output_type ## 1>::make(op (a.x)); \
    } \
    __device__ __forceinline__ output_type ## 2 operator op(const input_type ## 2 & a) \
    { \
        return VecTraits<output_type ## 2>::make(op (a.x), op (a.y)); \
    } \
    __device__ __forceinline__ output_type ## 3 operator op(const input_type ## 3 & a) \
    { \
        return VecTraits<output_type ## 3>::make(op (a.x), op (a.y), op (a.z)); \
    } \
    __device__ __forceinline__ output_type ## 4 operator op(const input_type ## 4 & a) \
    { \
        return VecTraits<output_type ## 4>::make(op (a.x), op (a.y), op (a.z), op (a.w)); \
    }

CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(-, char, char)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(-, short, short)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(-, int, int)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(-, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(-, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(!, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(!, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(!, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(!, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(!, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(!, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(!, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(!, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(~, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(~, char, char)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(~, ushort, ushort)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(~, short, short)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(~, int, int)
CC_CUDEV_IMPLEMENT_VEC_UNARY_OP(~, uint, uint)

#undef CC_CUDEV_IMPLEMENT_VEC_UNARY_OP

// unary functions

#define CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(func_name, func, input_type, output_type) \
    __device__ __forceinline__ output_type ## 1 func_name(const input_type ## 1 & a) \
    { \
        return VecTraits<output_type ## 1>::make(func (a.x)); \
    } \
    __device__ __forceinline__ output_type ## 2 func_name(const input_type ## 2 & a) \
    { \
        return VecTraits<output_type ## 2>::make(func (a.x), func (a.y)); \
    } \
    __device__ __forceinline__ output_type ## 3 func_name(const input_type ## 3 & a) \
    { \
        return VecTraits<output_type ## 3>::make(func (a.x), func (a.y), func (a.z)); \
    } \
    __device__ __forceinline__ output_type ## 4 func_name(const input_type ## 4 & a) \
    { \
        return VecTraits<output_type ## 4>::make(func (a.x), func (a.y), func (a.z), func (a.w)); \
    }

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(abs, /*::abs*/, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(abs, ::abs, char, char)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(abs, /*::abs*/, ushort, ushort)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(abs, ::abs, short, short)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(abs, ::abs, int, int)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(abs, /*::abs*/, uint, uint)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(abs, ::fabsf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(abs, ::fabs, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sqrt, ::sqrtf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sqrt, ::sqrtf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sqrt, ::sqrtf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sqrt, ::sqrtf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sqrt, ::sqrtf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sqrt, ::sqrtf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sqrt, ::sqrtf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sqrt, ::sqrt, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp, ::expf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp, ::expf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp, ::expf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp, ::expf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp, ::expf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp, ::expf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp, ::expf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp, ::exp, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp2, ::exp2f, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp2, ::exp2f, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp2, ::exp2f, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp2, ::exp2f, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp2, ::exp2f, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp2, ::exp2f, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp2, ::exp2f, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp2, ::exp2, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp10, ::exp10f, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp10, ::exp10f, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp10, ::exp10f, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp10, ::exp10f, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp10, ::exp10f, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp10, ::exp10f, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp10, ::exp10f, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(exp10, ::exp10, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log, ::logf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log, ::logf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log, ::logf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log, ::logf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log, ::logf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log, ::logf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log, ::logf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log, ::log, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log2, ::log2f, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log2, ::log2f, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log2, ::log2f, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log2, ::log2f, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log2, ::log2f, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log2, ::log2f, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log2, ::log2f, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log2, ::log2, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log10, ::log10f, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log10, ::log10f, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log10, ::log10f, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log10, ::log10f, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log10, ::log10f, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log10, ::log10f, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log10, ::log10f, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(log10, ::log10, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sin, ::sinf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sin, ::sinf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sin, ::sinf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sin, ::sinf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sin, ::sinf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sin, ::sinf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sin, ::sinf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sin, ::sin, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cos, ::cosf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cos, ::cosf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cos, ::cosf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cos, ::cosf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cos, ::cosf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cos, ::cosf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cos, ::cosf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cos, ::cos, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tan, ::tanf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tan, ::tanf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tan, ::tanf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tan, ::tanf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tan, ::tanf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tan, ::tanf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tan, ::tanf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tan, ::tan, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asin, ::asinf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asin, ::asinf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asin, ::asinf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asin, ::asinf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asin, ::asinf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asin, ::asinf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asin, ::asinf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asin, ::asin, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acos, ::acosf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acos, ::acosf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acos, ::acosf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acos, ::acosf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acos, ::acosf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acos, ::acosf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acos, ::acosf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acos, ::acos, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atan, ::atanf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atan, ::atanf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atan, ::atanf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atan, ::atanf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atan, ::atanf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atan, ::atanf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atan, ::atanf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atan, ::atan, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sinh, ::sinhf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sinh, ::sinhf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sinh, ::sinhf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sinh, ::sinhf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sinh, ::sinhf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sinh, ::sinhf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sinh, ::sinhf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(sinh, ::sinh, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cosh, ::coshf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cosh, ::coshf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cosh, ::coshf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cosh, ::coshf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cosh, ::coshf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cosh, ::coshf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cosh, ::coshf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(cosh, ::cosh, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tanh, ::tanhf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tanh, ::tanhf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tanh, ::tanhf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tanh, ::tanhf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tanh, ::tanhf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tanh, ::tanhf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tanh, ::tanhf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(tanh, ::tanh, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asinh, ::asinhf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asinh, ::asinhf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asinh, ::asinhf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asinh, ::asinhf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asinh, ::asinhf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asinh, ::asinhf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asinh, ::asinhf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(asinh, ::asinh, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acosh, ::acoshf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acosh, ::acoshf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acosh, ::acoshf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acosh, ::acoshf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acosh, ::acoshf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acosh, ::acoshf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acosh, ::acoshf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(acosh, ::acosh, double, double)

CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atanh, ::atanhf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atanh, ::atanhf, char, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atanh, ::atanhf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atanh, ::atanhf, short, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atanh, ::atanhf, int, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atanh, ::atanhf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atanh, ::atanhf, float, float)
CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC(atanh, ::atanh, double, double)

#undef CC_CUDEV_IMPLEMENT_VEC_UNARY_FUNC

// binary operators (vec & vec)

#define CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(op, input_type, output_type) \
    __device__ __forceinline__ output_type ## 1 operator op(const input_type ## 1 & a, const input_type ## 1 & b) \
    { \
        return VecTraits<output_type ## 1>::make(a.x op b.x); \
    } \
    __device__ __forceinline__ output_type ## 2 operator op(const input_type ## 2 & a, const input_type ## 2 & b) \
    { \
        return VecTraits<output_type ## 2>::make(a.x op b.x, a.y op b.y); \
    } \
    __device__ __forceinline__ output_type ## 3 operator op(const input_type ## 3 & a, const input_type ## 3 & b) \
    { \
        return VecTraits<output_type ## 3>::make(a.x op b.x, a.y op b.y, a.z op b.z); \
    } \
    __device__ __forceinline__ output_type ## 4 operator op(const input_type ## 4 & a, const input_type ## 4 & b) \
    { \
        return VecTraits<output_type ## 4>::make(a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w); \
    }

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(+, uchar, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(+, char, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(+, ushort, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(+, short, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(+, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(+, uint, uint)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(+, float, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(+, double, double)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(-, uchar, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(-, char, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(-, ushort, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(-, short, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(-, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(-, uint, uint)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(-, float, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(-, double, double)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(*, uchar, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(*, char, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(*, ushort, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(*, short, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(*, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(*, uint, uint)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(*, float, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(*, double, double)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(/, uchar, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(/, char, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(/, ushort, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(/, short, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(/, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(/, uint, uint)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(/, float, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(/, double, double)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(==, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(==, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(==, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(==, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(==, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(==, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(==, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(==, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(!=, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(!=, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(!=, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(!=, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(!=, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(!=, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(!=, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(!=, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>=, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>=, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>=, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>=, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>=, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>=, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>=, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(>=, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<=, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<=, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<=, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<=, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<=, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<=, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<=, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(<=, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&&, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&&, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&&, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&&, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&&, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&&, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&&, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&&, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(||, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(||, char, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(||, ushort, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(||, short, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(||, int, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(||, uint, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(||, float, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(||, double, uchar)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&, char, char)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&, ushort, ushort)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&, short, short)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(&, uint, uint)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(|, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(|, char, char)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(|, ushort, ushort)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(|, short, short)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(|, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(|, uint, uint)

CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(^, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(^, char, char)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(^, ushort, ushort)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(^, short, short)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(^, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_OP(^, uint, uint)

#undef CC_CUDEV_IMPLEMENT_VEC_BINARY_OP

// binary operators (vec & scalar)

#define CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(op, input_type, scalar_type, output_type) \
    __device__ __forceinline__ output_type ## 1 operator op(const input_type ## 1 & a, scalar_type s) \
    { \
        return VecTraits<output_type ## 1>::make(a.x op s); \
    } \
    __device__ __forceinline__ output_type ## 1 operator op(scalar_type s, const input_type ## 1 & b) \
    { \
        return VecTraits<output_type ## 1>::make(s op b.x); \
    } \
    __device__ __forceinline__ output_type ## 2 operator op(const input_type ## 2 & a, scalar_type s) \
    { \
        return VecTraits<output_type ## 2>::make(a.x op s, a.y op s); \
    } \
    __device__ __forceinline__ output_type ## 2 operator op(scalar_type s, const input_type ## 2 & b) \
    { \
        return VecTraits<output_type ## 2>::make(s op b.x, s op b.y); \
    } \
    __device__ __forceinline__ output_type ## 3 operator op(const input_type ## 3 & a, scalar_type s) \
    { \
        return VecTraits<output_type ## 3>::make(a.x op s, a.y op s, a.z op s); \
    } \
    __device__ __forceinline__ output_type ## 3 operator op(scalar_type s, const input_type ## 3 & b) \
    { \
        return VecTraits<output_type ## 3>::make(s op b.x, s op b.y, s op b.z); \
    } \
    __device__ __forceinline__ output_type ## 4 operator op(const input_type ## 4 & a, scalar_type s) \
    { \
        return VecTraits<output_type ## 4>::make(a.x op s, a.y op s, a.z op s, a.w op s); \
    } \
    __device__ __forceinline__ output_type ## 4 operator op(scalar_type s, const input_type ## 4 & b) \
    { \
        return VecTraits<output_type ## 4>::make(s op b.x, s op b.y, s op b.z, s op b.w); \
    }

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, uchar, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, uchar, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, uchar, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, char, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, char, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, char, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, ushort, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, ushort, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, ushort, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, short, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, short, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, short, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, int, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, int, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, uint, uint, uint)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, uint, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, uint, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, float, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, float, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(+, double, double, double)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, uchar, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, uchar, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, uchar, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, char, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, char, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, char, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, ushort, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, ushort, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, ushort, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, short, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, short, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, short, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, int, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, int, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, uint, uint, uint)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, uint, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, uint, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, float, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, float, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(-, double, double, double)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, uchar, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, uchar, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, uchar, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, char, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, char, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, char, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, ushort, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, ushort, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, ushort, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, short, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, short, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, short, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, int, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, int, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, uint, uint, uint)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, uint, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, uint, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, float, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, float, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(*, double, double, double)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, uchar, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, uchar, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, uchar, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, char, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, char, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, char, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, ushort, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, ushort, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, ushort, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, short, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, short, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, short, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, int, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, int, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, uint, uint, uint)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, uint, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, uint, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, float, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, float, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(/, double, double, double)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(==, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(==, char, char, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(==, ushort, ushort, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(==, short, short, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(==, int, int, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(==, uint, uint, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(==, float, float, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(==, double, double, uchar)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(!=, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(!=, char, char, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(!=, ushort, ushort, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(!=, short, short, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(!=, int, int, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(!=, uint, uint, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(!=, float, float, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(!=, double, double, uchar)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>, char, char, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>, ushort, ushort, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>, short, short, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>, int, int, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>, uint, uint, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>, float, float, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>, double, double, uchar)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<, char, char, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<, ushort, ushort, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<, short, short, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<, int, int, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<, uint, uint, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<, float, float, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<, double, double, uchar)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>=, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>=, char, char, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>=, ushort, ushort, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>=, short, short, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>=, int, int, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>=, uint, uint, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>=, float, float, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(>=, double, double, uchar)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<=, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<=, char, char, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<=, ushort, ushort, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<=, short, short, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<=, int, int, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<=, uint, uint, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<=, float, float, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(<=, double, double, uchar)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&&, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&&, char, char, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&&, ushort, ushort, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&&, short, short, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&&, int, int, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&&, uint, uint, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&&, float, float, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&&, double, double, uchar)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(||, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(||, char, char, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(||, ushort, ushort, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(||, short, short, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(||, int, int, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(||, uint, uint, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(||, float, float, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(||, double, double, uchar)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&, char, char, char)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&, ushort, ushort, ushort)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&, short, short, short)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(&, uint, uint, uint)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(|, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(|, char, char, char)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(|, ushort, ushort, ushort)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(|, short, short, short)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(|, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(|, uint, uint, uint)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(^, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(^, char, char, char)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(^, ushort, ushort, ushort)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(^, short, short, short)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(^, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP(^, uint, uint, uint)

#undef CC_CUDEV_IMPLEMENT_SCALAR_BINARY_OP

// binary function (vec & vec)

#define CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(func_name, func, input_type, output_type) \
    __device__ __forceinline__ output_type ## 1 func_name(const input_type ## 1 & a, const input_type ## 1 & b) \
    { \
        return VecTraits<output_type ## 1>::make(func (a.x, b.x)); \
    } \
    __device__ __forceinline__ output_type ## 2 func_name(const input_type ## 2 & a, const input_type ## 2 & b) \
    { \
        return VecTraits<output_type ## 2>::make(func (a.x, b.x), func (a.y, b.y)); \
    } \
    __device__ __forceinline__ output_type ## 3 func_name(const input_type ## 3 & a, const input_type ## 3 & b) \
    { \
        return VecTraits<output_type ## 3>::make(func (a.x, b.x), func (a.y, b.y), func (a.z, b.z)); \
    } \
    __device__ __forceinline__ output_type ## 4 func_name(const input_type ## 4 & a, const input_type ## 4 & b) \
    { \
        return VecTraits<output_type ## 4>::make(func (a.x, b.x), func (a.y, b.y), func (a.z, b.z), func (a.w, b.w)); \
    }

CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(max, ::max, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(max, ::max, char, char)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(max, ::max, ushort, ushort)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(max, ::max, short, short)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(max, ::max, uint, uint)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(max, ::max, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(max, ::fmaxf, float, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(max, ::fmax, double, double)

CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(min, ::min, uchar, uchar)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(min, ::min, char, char)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(min, ::min, ushort, ushort)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(min, ::min, short, short)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(min, ::min, uint, uint)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(min, ::min, int, int)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(min, ::fminf, float, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(min, ::fmin, double, double)

CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(hypot, ::hypotf, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(hypot, ::hypotf, char, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(hypot, ::hypotf, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(hypot, ::hypotf, short, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(hypot, ::hypotf, uint, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(hypot, ::hypotf, int, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(hypot, ::hypotf, float, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(hypot, ::hypot, double, double)

CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(atan2, ::atan2f, uchar, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(atan2, ::atan2f, char, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(atan2, ::atan2f, ushort, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(atan2, ::atan2f, short, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(atan2, ::atan2f, uint, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(atan2, ::atan2f, int, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(atan2, ::atan2f, float, float)
CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC(atan2, ::atan2, double, double)

#undef CC_CUDEV_IMPLEMENT_VEC_BINARY_FUNC

// binary function (vec & scalar)

#define CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(func_name, func, input_type, scalar_type, output_type) \
    __device__ __forceinline__ output_type ## 1 func_name(const input_type ## 1 & a, scalar_type s) \
    { \
        return VecTraits<output_type ## 1>::make(func ((output_type) a.x, (output_type) s)); \
    } \
    __device__ __forceinline__ output_type ## 1 func_name(scalar_type s, const input_type ## 1 & b) \
    { \
        return VecTraits<output_type ## 1>::make(func ((output_type) s, (output_type) b.x)); \
    } \
    __device__ __forceinline__ output_type ## 2 func_name(const input_type ## 2 & a, scalar_type s) \
    { \
        return VecTraits<output_type ## 2>::make(func ((output_type) a.x, (output_type) s), func ((output_type) a.y, (output_type) s)); \
    } \
    __device__ __forceinline__ output_type ## 2 func_name(scalar_type s, const input_type ## 2 & b) \
    { \
        return VecTraits<output_type ## 2>::make(func ((output_type) s, (output_type) b.x), func ((output_type) s, (output_type) b.y)); \
    } \
    __device__ __forceinline__ output_type ## 3 func_name(const input_type ## 3 & a, scalar_type s) \
    { \
        return VecTraits<output_type ## 3>::make(func ((output_type) a.x, (output_type) s), func ((output_type) a.y, (output_type) s), func ((output_type) a.z, (output_type) s)); \
    } \
    __device__ __forceinline__ output_type ## 3 func_name(scalar_type s, const input_type ## 3 & b) \
    { \
        return VecTraits<output_type ## 3>::make(func ((output_type) s, (output_type) b.x), func ((output_type) s, (output_type) b.y), func ((output_type) s, (output_type) b.z)); \
    } \
    __device__ __forceinline__ output_type ## 4 func_name(const input_type ## 4 & a, scalar_type s) \
    { \
        return VecTraits<output_type ## 4>::make(func ((output_type) a.x, (output_type) s), func ((output_type) a.y, (output_type) s), func ((output_type) a.z, (output_type) s), func ((output_type) a.w, (output_type) s)); \
    } \
    __device__ __forceinline__ output_type ## 4 func_name(scalar_type s, const input_type ## 4 & b) \
    { \
        return VecTraits<output_type ## 4>::make(func ((output_type) s, (output_type) b.x), func ((output_type) s, (output_type) b.y), func ((output_type) s, (output_type) b.z), func ((output_type) s, (output_type) b.w)); \
    }

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::max, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmaxf, uchar, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmax, uchar, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::max, char, char, char)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmaxf, char, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmax, char, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::max, ushort, ushort, ushort)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmaxf, ushort, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmax, ushort, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::max, short, short, short)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmaxf, short, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmax, short, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::max, uint, uint, uint)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmaxf, uint, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmax, uint, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::max, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmaxf, int, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmax, int, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmaxf, float, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmax, float, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(max, ::fmax, double, double, double)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::min, uchar, uchar, uchar)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fminf, uchar, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fmin, uchar, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::min, char, char, char)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fminf, char, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fmin, char, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::min, ushort, ushort, ushort)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fminf, ushort, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fmin, ushort, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::min, short, short, short)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fminf, short, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fmin, short, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::min, uint, uint, uint)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fminf, uint, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fmin, uint, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::min, int, int, int)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fminf, int, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fmin, int, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fminf, float, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fmin, float, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(min, ::fmin, double, double, double)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypotf, uchar, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypot, uchar, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypotf, char, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypot, char, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypotf, ushort, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypot, ushort, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypotf, short, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypot, short, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypotf, uint, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypot, uint, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypotf, int, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypot, int, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypotf, float, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypot, float, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(hypot, ::hypot, double, double, double)

CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2f, uchar, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2, uchar, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2f, char, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2, char, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2f, ushort, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2, ushort, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2f, short, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2, short, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2f, uint, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2, uint, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2f, int, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2, int, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2f, float, float, float)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2, float, double, double)
CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC(atan2, ::atan2, double, double, double)

#undef CC_CUDEV_IMPLEMENT_SCALAR_BINARY_FUNC

}}} // namespace cv { namespace cuda { namespace device

//! @endcond

#endif // OPENCC_CUDA_VECMATH_HPP
