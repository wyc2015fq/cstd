

#ifndef OPENCC_CUDA_DATAMOV_UTILS_HPP
#define OPENCC_CUDA_DATAMOV_UTILS_HPP

#include "common.hpp"

/** @file
 * @deprecated Use @ref cudev instead.
 */

//! @cond IGNORED

namespace cv { namespace cuda { namespace device
{
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 200

        // for Fermi memory space is detected automatically
        template <typename T> struct ForceGlob
        {
            __device__ __forceinline__ static void Load(const T* ptr, int offset, T& val)  { val = ptr[offset];  }
        };

    #else // __CUDA_ARCH__ >= 200

        #if defined(_WIN64) || defined(__LP64__)
            // 64-bit register modifier for inlined asm
            #define OPENCC_CUDA_ASM_PTR "l"
        #else
            // 32-bit register modifier for inlined asm
            #define OPENCC_CUDA_ASM_PTR "r"
        #endif

        template<class T> struct ForceGlob;

        #define OPENCC_CUDA_DEFINE_FORCE_GLOB(base_type, ptx_type, reg_mod) \
            template <> struct ForceGlob<base_type> \
            { \
                __device__ __forceinline__ static void Load(const base_type* ptr, int offset, base_type& val) \
                { \
                    asm("ld.global."#ptx_type" %0, [%1];" : "="#reg_mod(val) : OPENCC_CUDA_ASM_PTR(ptr + offset)); \
                } \
            };

        #define OPENCC_CUDA_DEFINE_FORCE_GLOB_B(base_type, ptx_type) \
            template <> struct ForceGlob<base_type> \
            { \
                __device__ __forceinline__ static void Load(const base_type* ptr, int offset, base_type& val) \
                { \
                    asm("ld.global."#ptx_type" %0, [%1];" : "=r"(*reinterpret_cast<uint*>(&val)) : OPENCC_CUDA_ASM_PTR(ptr + offset)); \
                } \
            };

            OPENCC_CUDA_DEFINE_FORCE_GLOB_B(uchar,  u8)
            OPENCC_CUDA_DEFINE_FORCE_GLOB_B(schar,  s8)
            OPENCC_CUDA_DEFINE_FORCE_GLOB_B(char,   b8)
            OPENCC_CUDA_DEFINE_FORCE_GLOB  (ushort, u16, h)
            OPENCC_CUDA_DEFINE_FORCE_GLOB  (short,  s16, h)
            OPENCC_CUDA_DEFINE_FORCE_GLOB  (uint,   u32, r)
            OPENCC_CUDA_DEFINE_FORCE_GLOB  (int,    s32, r)
            OPENCC_CUDA_DEFINE_FORCE_GLOB  (float,  f32, f)
            OPENCC_CUDA_DEFINE_FORCE_GLOB  (double, f64, d)

        #undef OPENCC_CUDA_DEFINE_FORCE_GLOB
        #undef OPENCC_CUDA_DEFINE_FORCE_GLOB_B
        #undef OPENCC_CUDA_ASM_PTR

    #endif // __CUDA_ARCH__ >= 200
}}} // namespace cv { namespace cuda { namespace cudev

//! @endcond

#endif // OPENCC_CUDA_DATAMOV_UTILS_HPP
