#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using namespace testing;
using std::tr1::make_tuple;
using std::tr1::get;

typedef tr1::tuple<CvSize, MatType> Size_Source_t;
typedef TestBaseWithParam<Size_Source_t> Size_Source;
typedef TestBaseWithParam<CvSize> TestMatSize;

static const float rangeHight = 256.0f;
static const float rangeLow = 0.0f;

PERF_TEST_P(Size_Source, calcHist1d,
            testing::Combine(testing::Values(sz3MP, sz5MP),
                             testing::Values(CC_8U, CC_16U, CC_32F) )
            )
{
    CvSize size = get<0>(GetParam());
    MatType type = get<1>(GetParam());
    CvMat source(size.height, size.width, type);
    CvMat hist;
    int channels [] = {0};
    int histSize [] = {256};
    int dims = 1;
    int numberOfImages = 1;

    const float range[] = {rangeLow, rangeHight};
    const float* ranges[] = {range};

    randu(source, rangeLow, rangeHight);

    declare.in(source);

    TEST_CYCLE_MULTIRUN(3)
    {
        calcHist(&source, numberOfImages, channels, CvMat(), hist, dims, histSize, ranges);
    }

    SANITY_CHECK(hist);
}

PERF_TEST_P(Size_Source, calcHist2d,
            testing::Combine(testing::Values(sz3MP, sz5MP),
                             testing::Values(CC_8UC2, CC_16UC2, CC_32FC2) )
            )
{
    CvSize size = get<0>(GetParam());
    MatType type = get<1>(GetParam());
    CvMat source(size.height, size.width, type);
    CvMat hist;
    int channels [] = {0, 1};
    int histSize [] = {256, 256};
    int dims = 2;
    int numberOfImages = 1;

    const float r[] = {rangeLow, rangeHight};
    const float* ranges[] = {r, r};

    randu(source, rangeLow, rangeHight);

    declare.in(source);
    TEST_CYCLE()
    {
        calcHist(&source, numberOfImages, channels, CvMat(), hist, dims, histSize, ranges);
    }

    SANITY_CHECK(hist);
}

PERF_TEST_P(Size_Source, calcHist3d,
            testing::Combine(testing::Values(sz3MP, sz5MP),
                             testing::Values(CC_8UC3, CC_16UC3, CC_32FC3) )
            )
{
    CvSize size = get<0>(GetParam());
    MatType type = get<1>(GetParam());
    CvMat hist;
    int channels [] = {0, 1, 2};
    int histSize [] = {32, 32, 32};
    int dims = 3;
    int numberOfImages = 1;
    CvMat source(size.height, size.width, type);

    const float r[] = {rangeLow, rangeHight};
    const float* ranges[] = {r, r, r};

    randu(source, rangeLow, rangeHight);

    declare.in(source);
    TEST_CYCLE()
    {
        calcHist(&source, numberOfImages, channels, CvMat(), hist, dims, histSize, ranges);
    }

    SANITY_CHECK(hist);
}

#define MatSize TestMatSize
PERF_TEST_P(MatSize, equalizeHist,
            testing::Values(TYPICAL_MAT_SIZES)
            )
{
    CvSize size = GetParam();
    CvMat source(size.height, size.width, CC_8U);
    CvMat destination;
    declare.in(source, WARMUP_RNG);

    TEST_CYCLE()
    {
        equalizeHist(source, destination);
    }

    SANITY_CHECK(destination);
}
#undef MatSize

typedef tr1::tuple<CvSize, double> Sz_ClipLimit_t;
typedef TestBaseWithParam<Sz_ClipLimit_t> Sz_ClipLimit;

PERF_TEST_P(Sz_ClipLimit, CLAHE,
            testing::Combine(testing::Values(::perf::szVGA, ::perf::sz720p, ::perf::sz1080p),
                             testing::Values(0.0, 40.0))
            )
{
    const CvSize size = get<0>(GetParam());
    const double clipLimit = get<1>(GetParam());

    CvMat src(size, CC_8UC1);
    declare.in(src, WARMUP_RNG);

    Ptr<CLAHE> clahe = createCLAHE(clipLimit);
    CvMat dst;

    TEST_CYCLE() clahe->apply(src, dst);

    SANITY_CHECK(dst);
}
