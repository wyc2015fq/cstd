

#ifndef OPENCC_CUDA_WARP_REDUCE_HPP__
#define OPENCC_CUDA_WARP_REDUCE_HPP__

/** @file
 * @deprecated Use @ref cudev instead.
 */

//! @cond IGNORED

namespace cv { namespace cuda { namespace device
{
    template <class T>
    __device__ __forceinline__ T warp_reduce(volatile T *ptr , const unsigned int tid = threadIdx.x)
    {
        const unsigned int lane = tid & 31; // index of thread in warp (0..31)

        if (lane < 16)
        {
            T partial = ptr[tid];

            ptr[tid] = partial = partial + ptr[tid + 16];
            ptr[tid] = partial = partial + ptr[tid + 8];
            ptr[tid] = partial = partial + ptr[tid + 4];
            ptr[tid] = partial = partial + ptr[tid + 2];
            ptr[tid] = partial = partial + ptr[tid + 1];
        }

        return ptr[tid - lane];
    }
}}} // namespace cv { namespace cuda { namespace cudev {

//! @endcond

#endif /* OPENCC_CUDA_WARP_REDUCE_HPP__ */
