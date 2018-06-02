#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

#define TYPICAL_MAT_TYPES_MORPH  CC_8UC1, CC_8UC4
#define TYPICAL_MATS_MORPH       testing::Combine(SZ_ALL_GA, testing::Values(TYPICAL_MAT_TYPES_MORPH))

PERF_TEST_P(Size_MatType, erode, TYPICAL_MATS_MORPH)
{
    CvSize sz = get<0>(GetParam());
    int type = get<1>(GetParam());

    CvMat src(sz, type);
    CvMat dst(sz, type);

    declare.in(src, WARMUP_RNG).out(dst);

    int runs = (sz.width <= 320) ? 15 : 1;
    TEST_CYCLE_MULTIRUN(runs) erode(src, dst, noArray());

    SANITY_CHECK(dst);
}

PERF_TEST_P(Size_MatType, dilate, TYPICAL_MATS_MORPH)
{
    CvSize sz = get<0>(GetParam());
    int type = get<1>(GetParam());

    CvMat src(sz, type);
    CvMat dst(sz, type);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() dilate(src, dst, noArray());

    SANITY_CHECK(dst);
}
