// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#include "opencv2/core/utility.hpp"
//#define CC_OPENCL_RUN_ASSERT

#ifdef HAVE_OPENCL

#ifdef CC_OPENCL_RUN_VERBOSE
#define CC_OCL_RUN_(condition, func, ...)                                   \
    {                                                                       \
        if (ocl::useOpenCL() && (condition) && func)                    \
        {                                                                   \
            printf("%s: OpenCL implementation is running\n", CC_Func);      \
            fflush(stdout);                                                 \
            CC_IMPL_ADD(CC_IMPL_OCL);                                       \
            return __VA_ARGS__;                                             \
        }                                                                   \
        else                                                                \
        {                                                                   \
            printf("%s: Plain implementation is running\n", CC_Func);       \
            fflush(stdout);                                                 \
        }                                                                   \
    }
#elif defined CC_OPENCL_RUN_ASSERT
#define CC_OCL_RUN_(condition, func, ...)                                   \
    {                                                                       \
        if (ocl::useOpenCL() && (condition))                            \
        {                                                                   \
            if(func)                                                        \
            {                                                               \
                CC_IMPL_ADD(CC_IMPL_OCL);                                   \
            }                                                               \
            else                                                            \
            {                                                               \
                CC_Error(Error::StsAssert, #func);                      \
            }                                                               \
            return __VA_ARGS__;                                             \
        }                                                                   \
    }
#else
#define CC_OCL_RUN_(condition, func, ...)                                   \
    if (ocl::useOpenCL() && (condition) && func)                        \
    {                                                                       \
        CC_IMPL_ADD(CC_IMPL_OCL);                                           \
        return __VA_ARGS__;                                                 \
    }
#endif

#else
#define CC_OCL_RUN_(condition, func, ...)
#endif

#define CC_OCL_RUN(condition, func) CC_OCL_RUN_(condition, func)
