#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

typedef std::tr1::tuple<CvSize, MatType, int> Size_MatType_kSize_t;
typedef perf::TestBaseWithParam<Size_MatType_kSize_t> Size_MatType_kSize;

PERF_TEST_P(Size_MatType_kSize, medianBlur,
            testing::Combine(
                testing::Values(szODD, szQVGA, szVGA, sz720p),
                testing::Values(CC_8UC1, CC_8UC4, CC_16UC1, CC_16SC1, CC_32FC1),
                testing::Values(3, 5)
                )
            )
{
    CvSize size = get<0>(GetParam());
    int type = get<1>(GetParam());
    int ksize = get<2>(GetParam());

    CvMat src(size, type);
    CvMat dst(size, type);

    declare.in(src, WARMUP_RNG).out(dst);

    if (CC_MAT_DEPTH(type) > CC_16S || CC_MAT_CN(type) > 1)
        declare.time(15);

    TEST_CYCLE() medianBlur(src, dst, ksize);

    SANITY_CHECK(dst);
}

CC_ENUM(BorderType3x3, BORDER_REPLICATE, BORDER_CONSTANT)
CC_ENUM(BorderType, BORDER_REPLICATE, BORDER_CONSTANT, BORDER_REFLECT, BORDER_REFLECT101)

typedef std::tr1::tuple<CvSize, MatType, BorderType3x3> Size_MatType_BorderType3x3_t;
typedef perf::TestBaseWithParam<Size_MatType_BorderType3x3_t> Size_MatType_BorderType3x3;

typedef std::tr1::tuple<CvSize, MatType, BorderType> Size_MatType_BorderType_t;
typedef perf::TestBaseWithParam<Size_MatType_BorderType_t> Size_MatType_BorderType;

PERF_TEST_P(Size_MatType_BorderType3x3, gaussianBlur3x3,
            testing::Combine(
                testing::Values(szODD, szQVGA, szVGA, sz720p),
                testing::Values(CC_8UC1, CC_8UC4, CC_16UC1, CC_16SC1, CC_32FC1),
                BorderType3x3::all()
                )
            )
{
    CvSize size = get<0>(GetParam());
    int type = get<1>(GetParam());
    BorderType3x3 btype = get<2>(GetParam());

    CvMat src(size, type);
    CvMat dst(size, type);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() GaussianBlur(src, dst, CvSize(3,3), 0, 0, btype);

    SANITY_CHECK(dst, 1);
}

PERF_TEST_P(Size_MatType_BorderType3x3, blur3x3,
            testing::Combine(
                testing::Values(szODD, szQVGA, szVGA, sz720p),
                testing::Values(CC_8UC1, CC_8UC4, CC_16UC1, CC_16SC1, CC_32FC1),
                BorderType3x3::all()
                )
            )
{
    CvSize size = get<0>(GetParam());
    int type = get<1>(GetParam());
    BorderType3x3 btype = get<2>(GetParam());

    CvMat src(size, type);
    CvMat dst(size, type);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() blur(src, dst, CvSize(3,3), Point(-1,-1), btype);

    SANITY_CHECK(dst, 1);
}

PERF_TEST_P(Size_MatType_BorderType, blur16x16,
            testing::Combine(
                testing::Values(szVGA, sz720p),
                testing::Values(CC_8UC1, CC_8UC4, CC_16UC1, CC_16SC1, CC_32FC1),
                BorderType::all()
                )
            )
{
    CvSize size = get<0>(GetParam());
    int type = get<1>(GetParam());
    BorderType btype = get<2>(GetParam());
    double eps = 1e-3;

    eps = CC_MAT_DEPTH(type) <= CC_32S ? 1 : eps;

    CvMat src(size, type);
    CvMat dst(size, type);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() blur(src, dst, CvSize(16,16), Point(-1,-1), btype);

    SANITY_CHECK(dst, eps);
}

PERF_TEST_P(Size_MatType_BorderType3x3, box3x3,
            testing::Combine(
                testing::Values(szODD, szQVGA, szVGA, sz720p),
                testing::Values(CC_8UC1, CC_16SC1, CC_32SC1, CC_32FC1, CC_32FC3),
                BorderType3x3::all()
                )
            )
{
    CvSize size = get<0>(GetParam());
    int type = get<1>(GetParam());
    BorderType3x3 btype = get<2>(GetParam());

    CvMat src(size, type);
    CvMat dst(size, type);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() boxFilter(src, dst, -1, CvSize(3,3), Point(-1,-1), false, btype);

    SANITY_CHECK(dst, 1e-6, ERROR_RELATIVE);
}

PERF_TEST_P(Size_MatType_BorderType3x3, box3x3_inplace,
            testing::Combine(
                testing::Values(szODD, szQVGA, szVGA, sz720p),
                testing::Values(CC_8UC1, CC_16SC1, CC_32SC1, CC_32FC1, CC_32FC3),
                BorderType3x3::all()
                )
            )
{
    CvSize size = get<0>(GetParam());
    int type = get<1>(GetParam());
    BorderType3x3 btype = get<2>(GetParam());

    CvMat src(size, type);
    CvMat dst(size, type);

    declare.in(src, WARMUP_RNG).out(dst);

    while(next())
    {
        src.copyTo(dst);
        startTimer();
        boxFilter(dst, dst, -1, CvSize(3,3), Point(-1,-1), false, btype);
        stopTimer();
    }

    SANITY_CHECK(dst, 1e-6, ERROR_RELATIVE);
}

PERF_TEST_P(Size_MatType_BorderType, gaussianBlur5x5,
            testing::Combine(
                testing::Values(szODD, szQVGA, szVGA, sz720p),
                testing::Values(CC_8UC1, CC_8UC4, CC_16UC1, CC_16SC1, CC_32FC1),
                BorderType::all()
                )
            )
{
    CvSize size = get<0>(GetParam());
    int type = get<1>(GetParam());
    BorderType btype = get<2>(GetParam());

    CvMat src(size, type);
    CvMat dst(size, type);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() GaussianBlur(src, dst, CvSize(5,5), 0, 0, btype);

    SANITY_CHECK(dst, 1);
}

PERF_TEST_P(Size_MatType_BorderType, blur5x5,
            testing::Combine(
                testing::Values(szVGA, sz720p),
                testing::Values(CC_8UC1, CC_8UC4, CC_16UC1, CC_16SC1, CC_32FC1, CC_32FC3),
                BorderType::all()
                )
            )
{
    CvSize size = get<0>(GetParam());
    int type = get<1>(GetParam());
    BorderType btype = get<2>(GetParam());

    CvMat src(size, type);
    CvMat dst(size, type);

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() blur(src, dst, CvSize(5,5), Point(-1,-1), btype);

    SANITY_CHECK(dst, 1);
}
