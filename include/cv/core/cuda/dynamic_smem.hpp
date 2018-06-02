

#ifndef OPENCC_CUDA_DYNAMIC_SMEM_HPP
#define OPENCC_CUDA_DYNAMIC_SMEM_HPP

/** @file
 * @deprecated Use @ref cudev instead.
 */

//! @cond IGNORED

namespace cv { namespace cuda { namespace device
{
    template<class T> struct DynamicSharedMem
    {
        __device__ __forceinline__ operator T*()
        {
            extern __shared__ int __smem[];
            return (T*)__smem;
        }

        __device__ __forceinline__ operator const T*() const
        {
            extern __shared__ int __smem[];
            return (T*)__smem;
        }
    };

    // specialize for double to avoid unaligned memory access compile errors
    template<> struct DynamicSharedMem<double>
    {
        __device__ __forceinline__ operator double*()
        {
            extern __shared__ double __smem_d[];
            return (double*)__smem_d;
        }

        __device__ __forceinline__ operator const double*() const
        {
            extern __shared__ double __smem_d[];
            return (double*)__smem_d;
        }
    };
}}}

//! @endcond

#endif // OPENCC_CUDA_DYNAMIC_SMEM_HPP
