#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

#define TYPICAL_MAT_TYPES_ADWEIGHTED  CC_8UC1, CC_8UC4, CC_8SC1, CC_16UC1, CC_16SC1, CC_32SC1
#define TYPICAL_MATS_ADWEIGHTED       testing::Combine(testing::Values(szVGA, sz720p, sz1080p), testing::Values(TYPICAL_MAT_TYPES_ADWEIGHTED))

PERF_TEST_P(Size_MatType, addWeighted, TYPICAL_MATS_ADWEIGHTED)
{
    Size size = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat src1(size, type);
    CvMat src2(size, type);
    double alpha = 3.75;
    double beta = -0.125;
    double gamma = 100.0;

    CvMat dst(size, type);

    declare.in(src1, src2, dst, WARMUP_RNG).out(dst);

    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        // there might be not enough precision for integers
        src1 /= 2048;
        src2 /= 2048;
    }

    TEST_CYCLE() addWeighted( src1, alpha, src2, beta, gamma, dst, dst CC_MAT_TYPE() );

    SANITY_CHECK(dst, 1);
}
