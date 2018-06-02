#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

typedef std::tr1::tuple<CvSize, MatType, MatDepth> Size_MatType_OutMatDepth_t;
typedef perf::TestBaseWithParam<Size_MatType_OutMatDepth_t> Size_MatType_OutMatDepth;

PERF_TEST_P(Size_MatType_OutMatDepth, integral,
            testing::Combine(
                testing::Values(TYPICAL_MAT_SIZES),
                testing::Values(CC_8UC1, CC_8UC4),
                testing::Values(CC_32S, CC_32F, CC_64F)
                )
            )
{
    CvSize sz = get<0>(GetParam());
    int matType = get<1>(GetParam());
    int sdepth = get<2>(GetParam());

    CvMat src(sz, matType);
    CvMat sum(sz, sdepth);

    declare.in(src, WARMUP_RNG).out(sum);

    TEST_CYCLE() integral(src, sum, sdepth);

    SANITY_CHECK(sum, 1e-6);
}

PERF_TEST_P(Size_MatType_OutMatDepth, integral_sqsum,
            testing::Combine(
                testing::Values(::perf::szVGA, ::perf::sz1080p),
                testing::Values(CC_8UC1, CC_8UC4),
                testing::Values(CC_32S, CC_32F)
                )
            )
{
    CvSize sz = get<0>(GetParam());
    int matType = get<1>(GetParam());
    int sdepth = get<2>(GetParam());

    CvMat src(sz, matType);
    CvMat sum(sz, sdepth);
    CvMat sqsum(sz, sdepth);

    declare.in(src, WARMUP_RNG).out(sum, sqsum);
    declare.time(100);

    TEST_CYCLE() integral(src, sum, sqsum, sdepth);

    SANITY_CHECK(sum, 1e-6);
    SANITY_CHECK(sqsum, 1e-6);
}

PERF_TEST_P( Size_MatType_OutMatDepth, integral_sqsum_tilted,
             testing::Combine(
                 testing::Values( ::perf::szVGA, ::perf::szODD , ::perf::sz1080p ),
                 testing::Values( CC_8UC1, CC_8UC4 ),
                 testing::Values( CC_32S, CC_32F )
                 )
             )
{
    CvSize sz = get<0>(GetParam());
    int matType = get<1>(GetParam());
    int sdepth = get<2>(GetParam());

    CvMat src(sz, matType);
    CvMat sum(sz, sdepth);
    CvMat sqsum(sz, sdepth);
    CvMat tilted(sz, sdepth);

    declare.in(src, WARMUP_RNG).out(sum, sqsum, tilted);
    declare.time(100);

    TEST_CYCLE() integral(src, sum, sqsum, tilted, sdepth);

    SANITY_CHECK(sum, 1e-6);
    SANITY_CHECK(sqsum, 1e-6);
    SANITY_CHECK(tilted, 1e-6, tilted.depth() > CC_32S ? ERROR_RELATIVE : ERROR_ABSOLUTE);
}
