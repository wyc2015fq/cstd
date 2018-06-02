#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

#define TYPICAL_MAT_SIZES_CORE_ARITHM   ::szVGA, ::sz720p, ::sz1080p
#define TYPICAL_MAT_TYPES_CORE_ARITHM   CC_8UC1, CC_8SC1, CC_16SC1, CC_16SC2, CC_16SC3, CC_16SC4, CC_8UC4, CC_32SC1, CC_32FC1
#define TYPICAL_MATS_CORE_ARITHM        testing::Combine( testing::Values( TYPICAL_MAT_SIZES_CORE_ARITHM ), testing::Values( TYPICAL_MAT_TYPES_CORE_ARITHM ) )

PERF_TEST_P(Size_MatType, min, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    CvMat b = CvMat(sz, type);
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    TEST_CYCLE() min(a, b, c);

    SANITY_CHECK(c);
}

PERF_TEST_P(Size_MatType, minScalar, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    Scalar b;
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    TEST_CYCLE() min(a, b, c);

    SANITY_CHECK(c);
}

PERF_TEST_P(Size_MatType, max, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    CvMat b = CvMat(sz, type);
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    TEST_CYCLE() max(a, b, c);

    SANITY_CHECK(c);
}

PERF_TEST_P(Size_MatType, maxScalar, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    Scalar b;
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    TEST_CYCLE() max(a, b, c);

    SANITY_CHECK(c);
}

PERF_TEST_P(Size_MatType, absdiff, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    CvMat b = CvMat(sz, type);
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        //see ticket 1529: absdiff can be without saturation on 32S
        a /= 2;
        b /= 2;
    }

    TEST_CYCLE() absdiff(a, b, c);

    SANITY_CHECK(c, 1e-8);
}

PERF_TEST_P(Size_MatType, absdiffScalar, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    Scalar b;
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        //see ticket 1529: absdiff can be without saturation on 32S
        a /= 2;
        b /= 2;
    }

    TEST_CYCLE() absdiff(a, b, c);

    SANITY_CHECK(c, 1e-8);
}

PERF_TEST_P(Size_MatType, add, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    CvMat b = CvMat(sz, type);
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);
    declare.time(50);

    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        //see ticket 1529: add can be without saturation on 32S
        a /= 2;
        b /= 2;
    }

    TEST_CYCLE() add(a, b, c);

    SANITY_CHECK(c, 1e-8);
}

PERF_TEST_P(Size_MatType, addScalar, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    Scalar b;
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        //see ticket 1529: add can be without saturation on 32S
        a /= 2;
        b /= 2;
    }

    TEST_CYCLE() add(a, b, c);

    SANITY_CHECK(c, 1e-8);
}

PERF_TEST_P(Size_MatType, subtract, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    CvMat b = CvMat(sz, type);
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        //see ticket 1529: subtract can be without saturation on 32S
        a /= 2;
        b /= 2;
    }

    TEST_CYCLE() subtract(a, b, c);

    SANITY_CHECK(c, 1e-8);
}

PERF_TEST_P(Size_MatType, subtractScalar, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a = CvMat(sz, type);
    Scalar b;
    CvMat c = CvMat(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);

    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        //see ticket 1529: subtract can be without saturation on 32S
        a /= 2;
        b /= 2;
    }

    TEST_CYCLE() subtract(a, b, c);

    SANITY_CHECK(c, 1e-8);
}

PERF_TEST_P(Size_MatType, multiply, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a(sz, type), b(sz, type), c(sz, type);

    declare.in(a, b, WARMUP_RNG).out(c);
    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        //According to docs, saturation is not applied when result is 32bit integer
        a /= (2 << 16);
        b /= (2 << 16);
    }

    TEST_CYCLE() multiply(a, b, c);

    SANITY_CHECK(c, 1e-8);
}

PERF_TEST_P(Size_MatType, multiplyScale, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a(sz, type), b(sz, type), c(sz, type);
    double scale = 0.5;

    declare.in(a, b, WARMUP_RNG).out(c);

    if (CC_MAT_DEPTH(type) == CC_32S)
    {
        //According to docs, saturation is not applied when result is 32bit integer
        a /= (2 << 16);
        b /= (2 << 16);
    }

    TEST_CYCLE() multiply(a, b, c, scale);

    SANITY_CHECK(c, 1e-8);
}

PERF_TEST_P(Size_MatType, divide, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat a(sz, type), b(sz, type), c(sz, type);
    double scale = 0.5;

    declare.in(a, b, WARMUP_RNG).out(c);

    TEST_CYCLE() divide(a, b, c, scale);

    SANITY_CHECK_NOTHING();
}

PERF_TEST_P(Size_MatType, reciprocal, TYPICAL_MATS_CORE_ARITHM)
{
    Size sz = get<0>(GetParam());
    int type = get<1>(GetParam());
    CvMat b(sz, type), c(sz, type);
    double scale = 0.5;

    declare.in(b, WARMUP_RNG).out(c);

    TEST_CYCLE() divide(scale, b, c);

    SANITY_CHECK_NOTHING();
}
