

#ifndef OPENCC_CUDA_COMMON_HPP
#define OPENCC_CUDA_COMMON_HPP

#include <cuda_runtime.h>
#include "opencv2/core/cuda_types.hpp"
#include "opencv2/core/cvdef.h"


/** @file
 * @deprecated Use @ref cudev instead.
 */

//! @cond IGNORED

#ifndef CC_PI_F
    #ifndef CC_PI
        #define CC_PI_F 3.14159265f
    #else
        #define CC_PI_F ((float)CC_PI)
    #endif
#endif

namespace cv { namespace cuda {
    static inline void checkCudaError(cudaError_t err, const char* file, const int line, const char* func)
    {
        if (cudaSuccess != err)
            error(Error::GpuApiCallError, cudaGetErrorString(err), func, file, line);
    }
}}

#ifndef cudaSafeCall
    #define cudaSafeCall(expr)  cuda::checkCudaError(expr, __FILE__, __LINE__, CC_Func)
#endif

namespace cv { namespace cuda
{
    template <typename T> static inline bool isAligned(const T* ptr, size_t size)
    {
        return reinterpret_cast<size_t>(ptr) % size == 0;
    }

    static inline bool isAligned(size_t step, size_t size)
    {
        return step % size == 0;
    }
}}

namespace cv { namespace cuda
{
    namespace device
    {
        __host__ __device__ __forceinline__ int divUp(int total, int grain)
        {
            return (total + grain - 1) / grain;
        }

        template<class T> inline void bindTexture(const textureReference* tex, const PtrStepSz<T>& img)
        {
            cudaChannelFormatDesc desc = cudaCreateChannelDesc<T>();
            cudaSafeCall( cudaBindTexture2D(0, tex, img.ptr(), &desc, img.cols, img.rows, img.step) );
        }
    }
}}

//! @endcond

#endif // OPENCC_CUDA_COMMON_HPP
