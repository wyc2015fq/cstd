#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

PERF_TEST_P(Size_MatType, pyrDown, testing::Combine(
                testing::Values(sz1080p, sz720p, szVGA, szQVGA, szODD),
                testing::Values(CC_8UC1, CC_8UC3, CC_8UC4, CC_16SC1, CC_16SC3, CC_16SC4, CC_32FC1, CC_32FC3, CC_32FC4)
                )
            )
{
    CvSize sz = get<0>(GetParam());
    int matType = get<1>(GetParam());
    const double eps = CC_MAT_DEPTH(matType) <= CC_32S ? 1 : 1e-5;
    perf::ERROR_TYPE error_type = CC_MAT_DEPTH(matType) <= CC_32S ? ERROR_ABSOLUTE : ERROR_RELATIVE;

    CvMat src(sz, matType);
    CvMat dst((sz.height + 1)/2, (sz.width + 1)/2, matType);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() pyrDown(src, dst);

    SANITY_CHECK(dst, eps, error_type);
}

PERF_TEST_P(Size_MatType, pyrUp, testing::Combine(
                testing::Values(sz720p, szVGA, szQVGA, szODD),
                testing::Values(CC_8UC1, CC_8UC3, CC_8UC4, CC_16SC1, CC_16SC3, CC_16SC4, CC_32FC1, CC_32FC3, CC_32FC4)
                )
            )
{
    CvSize sz = get<0>(GetParam());
    int matType = get<1>(GetParam());
    const double eps = CC_MAT_DEPTH(matType) <= CC_32S ? 1 : 1e-5;
    perf::ERROR_TYPE error_type = CC_MAT_DEPTH(matType) <= CC_32S ? ERROR_ABSOLUTE : ERROR_RELATIVE;

    CvMat src(sz, matType);
    CvMat dst(sz.height*2, sz.width*2, matType);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() pyrUp(src, dst);

    SANITY_CHECK(dst, eps, error_type);
}

PERF_TEST_P(Size_MatType, buildPyramid, testing::Combine(
                testing::Values(sz1080p, sz720p, szVGA, szQVGA, szODD),
                testing::Values(CC_8UC1, CC_8UC3, CC_8UC4, CC_32FC1, CC_32FC3, CC_32FC4)
                )
            )
{
    CvSize sz = get<0>(GetParam());
    int matType = get<1>(GetParam());
    int maxLevel = 5;
    const double eps = CC_MAT_DEPTH(matType) <= CC_32S ? 1 : 1e-5;
    perf::ERROR_TYPE error_type = CC_MAT_DEPTH(matType) <= CC_32S ? ERROR_ABSOLUTE : ERROR_RELATIVE;
    CvMat src(sz, matType);
    std::vector<CvMat> dst(maxLevel);

    declare.in(src, WARMUP_RNG);

    TEST_CYCLE() buildPyramid(src, dst, maxLevel);

    CvMat dst0 = dst[0], dst1 = dst[1], dst2 = dst[2], dst3 = dst[3], dst4 = dst[4];

    SANITY_CHECK(dst0, eps, error_type);
    SANITY_CHECK(dst1, eps, error_type);
    SANITY_CHECK(dst2, eps, error_type);
    SANITY_CHECK(dst3, eps, error_type);
    SANITY_CHECK(dst4, eps, error_type);
}
