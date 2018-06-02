#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using namespace testing;
using std::tr1::make_tuple;
using std::tr1::get;

CC_ENUM(Mat_Type, CC_8UC1, CC_8UC3, CC_32FC1, CC_32FC3)

typedef TestBaseWithParam< tr1::tuple<CvSize, int, Mat_Type> > TestBilateralFilter;

PERF_TEST_P( TestBilateralFilter, BilateralFilter,
             Combine(
                Values( szVGA, sz1080p ), // image size
                Values( 3, 5 ), // d
                Mat_Type::all() // image type
             )
)
{
    CvSize sz;
    int d, type;
    const double sigmaColor = 1., sigmaSpace = 1.;

    sz         = get<0>(GetParam());
    d          = get<1>(GetParam());
    type       = get<2>(GetParam());

    CvMat src(sz, type);
    CvMat dst(sz, type);

    declare.in(src, WARMUP_RNG).out(dst).time(20);

    TEST_CYCLE() bilateralFilter(src, dst, d, sigmaColor, sigmaSpace, BORDER_DEFAULT);

    SANITY_CHECK(dst, .01, ERROR_RELATIVE);
}
