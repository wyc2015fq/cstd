#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using namespace testing;
using std::tr1::make_tuple;
using std::tr1::get;

typedef std::tr1::tuple<CvSize, int, int> Size_Ksize_BorderType_t;
typedef perf::TestBaseWithParam<Size_Ksize_BorderType_t> Size_Ksize_BorderType;

PERF_TEST_P( Size_Ksize_BorderType, spatialGradient,
    Combine(
        SZ_ALL_HD,
        Values( 3 ),
        Values( BORDER_DEFAULT, BORDER_REPLICATE )
    )
)
{
    CvSize size = std::tr1::get<0>(GetParam());
    int ksize = std::tr1::get<1>(GetParam());
    int borderType = std::tr1::get<2>(GetParam());

    CvMat src(size, CC_8UC1);
    CvMat dx(size, CC_16SC1);
    CvMat dy(size, CC_16SC1);

    declare.in(src, WARMUP_RNG).out(dx, dy);

    TEST_CYCLE() spatialGradient(src, dx, dy, ksize, borderType);

    SANITY_CHECK(dx);
    SANITY_CHECK(dy);
}
