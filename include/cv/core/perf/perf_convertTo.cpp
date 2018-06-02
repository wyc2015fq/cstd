#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

typedef std::tr1::tuple<Size, MatType, MatType, int, double> Size_DepthSrc_DepthDst_Channels_alpha_t;
typedef perf::TestBaseWithParam<Size_DepthSrc_DepthDst_Channels_alpha_t> Size_DepthSrc_DepthDst_Channels_alpha;

PERF_TEST_P( Size_DepthSrc_DepthDst_Channels_alpha, convertTo,
             testing::Combine
             (
                 testing::Values(szVGA, sz1080p),
                 testing::Values(CC_8U, CC_8S, CC_16U, CC_16S, CC_32S, CC_32F, CC_64F),
                 testing::Values(CC_8U, CC_8S, CC_16U, CC_16S, CC_32S, CC_32F, CC_64F),
                 testing::Values(1, 4),
                 testing::Values(1.0, 1./255)
             )
           )
{
    Size sz = get<0>(GetParam());
    int depthSrc = get<1>(GetParam());
    int depthDst = get<2>(GetParam());
    int channels = get<3>(GetParam());
    double alpha = get<4>(GetParam());

    int maxValue = 255;

    CvMat src(sz, CC_MAKETYPE(depthSrc, channels));
    randu(src, 0, maxValue);
    CvMat dst(sz, CC_MAKETYPE(depthDst, channels));

    int runs = (sz.width <= 640) ? 8 : 1;
    TEST_CYCLE_MULTIRUN(runs) src.convertTo(dst, depthDst, alpha);

    double eps = depthSrc <= CC_32S ? 1e-12 : (FLT_EPSILON * maxValue);
    eps = eps * MAX(1.0, fabs(alpha));
    SANITY_CHECK(dst, eps);
}
