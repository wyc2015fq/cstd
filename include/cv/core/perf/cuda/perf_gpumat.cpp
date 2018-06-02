

#include "../perf_precomp.hpp"

#ifdef HAVE_CUDA

#include "opencv2/core/cuda.hpp"
#include "opencv2/ts/cuda_perf.hpp"

using namespace std;
using namespace testing;
using namespace perf;

//////////////////////////////////////////////////////////////////////
// SetTo

PERF_TEST_P(Sz_Depth_Cn, CUDA_GpuMat_SetTo,
            Combine(CUDA_TYPICAL_MAT_SIZES,
                    Values(CC_8U, CC_16U, CC_32F, CC_64F),
                    CUDA_CHANNELS_1_3_4))
{
    const Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CC_MAKE_TYPE(depth, channels);

    const Scalar val(1, 2, 3, 4);

    if (PERF_RUN_CUDA())
    {
        cuda::GpuMat dst(size, type);

        TEST_CYCLE() dst.setTo(val);
    }
    else
    {
        CvMat dst(size, type);

        TEST_CYCLE() dst.setTo(val);
    }

    SANITY_CHECK_NOTHING();
}

//////////////////////////////////////////////////////////////////////
// SetToMasked

PERF_TEST_P(Sz_Depth_Cn, CUDA_GpuMat_SetToMasked,
            Combine(CUDA_TYPICAL_MAT_SIZES,
                    Values(CC_8U, CC_16U, CC_32F, CC_64F),
                    CUDA_CHANNELS_1_3_4))
{
    const Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CC_MAKE_TYPE(depth, channels);

    CvMat src(size, type);
    CvMat mask(size, CC_8UC1);
    declare.in(src, mask, WARMUP_RNG);

    const Scalar val(1, 2, 3, 4);

    if (PERF_RUN_CUDA())
    {
        cuda::GpuMat dst(src);
        const cuda::GpuMat d_mask(mask);

        TEST_CYCLE() dst.setTo(val, d_mask);
    }
    else
    {
        CvMat dst = src;

        TEST_CYCLE() dst.setTo(val, mask);
    }

    SANITY_CHECK_NOTHING();
}

//////////////////////////////////////////////////////////////////////
// CopyToMasked

PERF_TEST_P(Sz_Depth_Cn, CUDA_GpuMat_CopyToMasked,
            Combine(CUDA_TYPICAL_MAT_SIZES,
                    Values(CC_8U, CC_16U, CC_32F, CC_64F),
                    CUDA_CHANNELS_1_3_4))
{
    const Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CC_MAKE_TYPE(depth, channels);

    CvMat src(size, type);
    CvMat mask(size, CC_8UC1);
    declare.in(src, mask, WARMUP_RNG);

    if (PERF_RUN_CUDA())
    {
        const cuda::GpuMat d_src(src);
        const cuda::GpuMat d_mask(mask);
        cuda::GpuMat dst(d_src.size(), d_src CC_MAT_TYPE(), Scalar::all(0));

        TEST_CYCLE() d_src.copyTo(dst, d_mask);
    }
    else
    {
        CvMat dst(src.size(), src CC_MAT_TYPE(), Scalar::all(0));

        TEST_CYCLE() src.copyTo(dst, mask);
    }

    SANITY_CHECK_NOTHING();
}

//////////////////////////////////////////////////////////////////////
// ConvertTo

DEF_PARAM_TEST(Sz_2Depth, Size, MatDepth, MatDepth);

PERF_TEST_P(Sz_2Depth, CUDA_GpuMat_ConvertTo,
            Combine(CUDA_TYPICAL_MAT_SIZES,
                    Values(CC_8U, CC_16U, CC_32F, CC_64F),
                    Values(CC_8U, CC_16U, CC_32F, CC_64F)))
{
    const Size size = GET_PARAM(0);
    const int depth1 = GET_PARAM(1);
    const int depth2 = GET_PARAM(2);

    CvMat src(size, depth1);
    declare.in(src, WARMUP_RNG);

    const double a = 0.5;
    const double b = 1.0;

    if (PERF_RUN_CUDA())
    {
        const cuda::GpuMat d_src(src);
        cuda::GpuMat dst;

        TEST_CYCLE() d_src.convertTo(dst, depth2, a, b);
    }
    else
    {
        CvMat dst;

        TEST_CYCLE() src.convertTo(dst, depth2, a, b);
    }

    SANITY_CHECK_NOTHING();
}

#endif
