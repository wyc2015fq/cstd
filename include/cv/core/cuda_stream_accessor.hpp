

#ifndef OPENCC_CORE_CUDA_STREAM_ACCESSOR_HPP
#define OPENCC_CORE_CUDA_STREAM_ACCESSOR_HPP

#ifndef __cplusplus
#  error cuda_stream_accessor.hpp header must be compiled as C++
#endif

/** @file cuda_stream_accessor.hpp
 * This is only header file that depends on CUDA Runtime API. All other headers are independent.
 */

#include <cuda_runtime.h>
#include "opencv2/core/cuda.hpp"

namespace cv
{
    namespace cuda
    {

//! @addtogroup cudacore_struct
//! @{

        /** @brief Class that enables getting cudaStream_t from cuda::Stream
         */
        struct StreamAccessor
        {
            CC_EXPORTS static cudaStream_t getStream(const Stream& stream);
            CC_EXPORTS static Stream wrapStream(cudaStream_t stream);
        };

        /** @brief Class that enables getting cudaEvent_t from cuda::Event
         */
        struct EventAccessor
        {
            CC_EXPORTS static cudaEvent_t getEvent(const Event& event);
            CC_EXPORTS static Event wrapEvent(cudaEvent_t event);
        };

//! @}

    }
}

#endif /* OPENCC_CORE_CUDA_STREAM_ACCESSOR_HPP */
