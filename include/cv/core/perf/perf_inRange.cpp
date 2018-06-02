#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

#define TYPICAL_MAT_TYPES_INRANGE  CC_8UC1, CC_8UC4, CC_8SC1, CC_16UC1, CC_16SC1, CC_32SC1, CC_32FC1, CC_32FC4
#define TYPICAL_MATS_INRANGE       testing::Combine(testing::Values(szVGA, sz720p, sz1080p), testing::Values(TYPICAL_MAT_TYPES_INRANGE))

PERF_TEST_P(Size_MatType, inRange, TYPICAL_MATS_INRANGE)
{
    Size size = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat src1(size, type);
    CvMat src2(size, type);
    CvMat src3(size, type);
    CvMat dst(size, type);

    declare.in(src1, src2, src3, WARMUP_RNG).out(dst);

    TEST_CYCLE() inRange( src1, src2, src3, dst );

    SANITY_CHECK(dst);
}
