#include "../perf_precomp.hpp"
#include "opencv2/ts/ocl_perf.hpp"

#ifdef HAVE_OPENCL

namespace cvtest {

namespace ocl {

CC_ENUM(MethodType, TM_SQDIFF, TM_SQDIFF_NORMED, TM_CCORR, TM_CCORR_NORMED, TM_CCOEFF, TM_CCOEFF_NORMED)

typedef std::tr1::tuple<CvSize, CvSize, MethodType, MatType> ImgSize_TmplSize_Method_MatType_t;
typedef TestBaseWithParam<ImgSize_TmplSize_Method_MatType_t> ImgSize_TmplSize_Method_MatType;

OCL_PERF_TEST_P(ImgSize_TmplSize_Method_MatType, MatchTemplate,
        ::testing::Combine(
            testing::Values(CvSize(640, 480), CvSize(1280, 1024)),
            testing::Values(CvSize(11, 11), CvSize(16, 16), CvSize(41, 41)),
            MethodType::all(),
            testing::Values(CC_8UC1, CC_8UC3, CC_32FC1, CC_32FC3)
            )
        )
{
    const ImgSize_TmplSize_Method_MatType_t params = GetParam();
    const CvSize imgSz = get<0>(params), tmplSz = get<1>(params);
    const int method = get<2>(params);
    int type = get<3>(GetParam());

    UMat img(imgSz, type), tmpl(tmplSz, type);
    UMat result(imgSz - tmplSz + CvSize(1, 1), CC_32F);

    declare.in(img, tmpl, WARMUP_RNG).out(result);

    OCL_TEST_CYCLE() matchTemplate(img, tmpl, result, method);

    bool isNormed =
        method == TM_CCORR_NORMED ||
        method == TM_SQDIFF_NORMED ||
        method == TM_CCOEFF_NORMED;
    double eps = isNormed ? 3e-2
        : 255 * 255 * tmpl.total() * 1e-4;

    SANITY_CHECK(result, eps, ERROR_RELATIVE);
}

/////////// matchTemplate (performance tests from 2.4) ////////////////////////

typedef Size_MatType CC_TM_CCORRFixture;

OCL_PERF_TEST_P(CC_TM_CCORRFixture, matchTemplate,
                ::testing::Combine(::testing::Values(CvSize(1000, 1000), CvSize(2000, 2000)),
                               OCL_PERF_ENUM(CC_32FC1, CC_32FC4)))
{
    const Size_MatType_t params = GetParam();
    const CvSize srcSize = get<0>(params), templSize(5, 5);
    const int type = get<1>(params);

    UMat src(srcSize, type), templ(templSize, type);
    const CvSize dstSize(src.cols - templ.cols + 1, src.rows - templ.rows + 1);
    UMat dst(dstSize, CC_32F);

    declare.in(src, templ, WARMUP_RNG).out(dst);

    OCL_TEST_CYCLE() matchTemplate(src, templ, dst, CC_TM_CCORR);

    SANITY_CHECK(dst, 1e-4);
}

typedef TestBaseWithParam<CvSize> CC_TM_CCORR_NORMEDFixture;

OCL_PERF_TEST_P(CC_TM_CCORR_NORMEDFixture, matchTemplate,
                ::testing::Values(CvSize(1000, 1000), CvSize(2000, 2000), CvSize(4000, 4000)))
{
    const CvSize srcSize = GetParam(), templSize(5, 5);

    UMat src(srcSize, CC_8UC1), templ(templSize, CC_8UC1);
    const CvSize dstSize(src.cols - templ.cols + 1, src.rows - templ.rows + 1);
    UMat dst(dstSize, CC_8UC1);

    declare.in(src, templ, WARMUP_RNG).out(dst);

    OCL_TEST_CYCLE() matchTemplate(src, templ, dst, CC_TM_CCORR_NORMED);

    SANITY_CHECK(dst, 3e-2);
}

} }

#endif // HAVE_OPENCL
