#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

#define TYPICAL_MAT_SIZES_ABS  TYPICAL_MAT_SIZES
#define TYPICAL_MAT_TYPES_ABS  CC_8SC1, CC_8SC4, CC_32SC1, CC_32FC1
#define TYPICAL_MATS_ABS       testing::Combine( testing::Values( TYPICAL_MAT_SIZES_ABS), testing::Values( TYPICAL_MAT_TYPES_ABS) )

PERF_TEST_P(Size_MatType, abs, TYPICAL_MATS_ABS)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());

    CvMat a = CvMat(sz, type);
    CvMat c = CvMat(sz, type);

    declare.in(a, WARMUP_RNG).out(c);

    TEST_CYCLE() c = abs(a);

    SANITY_CHECK(c);
}
