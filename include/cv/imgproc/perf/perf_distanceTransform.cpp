/*#include "perf_precomp.hpp"
#include "distransform.cpp"

using namespace std;
using namespace cv;
using namespace perf;

typedef perf::TestBaseWithParam<CvSize> Size_DistanceTransform;

PERF_TEST_P(Size_DistanceTransform, icvTrueDistTrans, testing::Values(TYPICAL_MAT_SIZES))
{
    CvSize size = GetParam();
    CvMat src(size, CC_8UC1);
    CvMat dst(size, CC_32FC1);
    CvMat srcStub = src;
    CvMat dstStub = dst;

    declare.in(src, WARMUP_RNG).out(dst);

    TEST_CYCLE() icvTrueDistTrans(&srcStub, &dstStub);

    SANITY_CHECK(dst, 1);
}*/

#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

CC_ENUM(DistanceType, DIST_L1, DIST_L2 , DIST_C)
CC_ENUM(MaskSize, DIST_MASK_3, DIST_MASK_5, DIST_MASK_PRECISE)
CC_ENUM(DstType, CC_8U, CC_32F)
CC_ENUM(LabelType, DIST_LABEL_CCOMP, DIST_LABEL_PIXEL)

typedef std::tr1::tuple<CvSize, DistanceType, MaskSize, DstType> SrcSize_DistType_MaskSize_DstType;
typedef std::tr1::tuple<CvSize, DistanceType, MaskSize, LabelType> SrcSize_DistType_MaskSize_LabelType;
typedef perf::TestBaseWithParam<SrcSize_DistType_MaskSize_DstType> DistanceTransform_Test;
typedef perf::TestBaseWithParam<SrcSize_DistType_MaskSize_LabelType> DistanceTransform_NeedLabels_Test;

PERF_TEST_P(DistanceTransform_Test, distanceTransform,
            testing::Combine(
                testing::Values(CvSize(640, 480), CvSize(800, 600), CvSize(1024, 768), CvSize(1280, 1024)),
                DistanceType::all(),
                MaskSize::all(),
                DstType::all()
                )
            )
{
    CvSize srcSize = get<0>(GetParam());
    int distanceType = get<1>(GetParam());
    int maskSize = get<2>(GetParam());
    int dstType = get<3>(GetParam());

    CvMat src(srcSize, CC_8U);
    CvMat dst(srcSize, dstType);

    declare
        .in(src, WARMUP_RNG)
        .out(dst, WARMUP_RNG)
        .time(30);

    TEST_CYCLE() distanceTransform( src, dst, distanceType, maskSize, dstType);

    double eps = 2e-4;

    SANITY_CHECK(dst, eps);
}

PERF_TEST_P(DistanceTransform_NeedLabels_Test, distanceTransform_NeedLabels,
            testing::Combine(
                testing::Values(CvSize(640, 480), CvSize(800, 600), CvSize(1024, 768), CvSize(1280, 1024)),
                DistanceType::all(),
                MaskSize::all(),
                LabelType::all()
                )
    )
{
    CvSize srcSize = get<0>(GetParam());
    int distanceType = get<1>(GetParam());
    int maskSize = get<2>(GetParam());
    int labelType = get<3>(GetParam());

    CvMat src(srcSize, CC_8U);
    CvMat label(srcSize, CC_32S);
    CvMat dst(srcSize, CC_32F);

    declare
        .in(src, WARMUP_RNG)
        .out(label, WARMUP_RNG)
        .out(dst, WARMUP_RNG)
        .time(30);

    TEST_CYCLE() distanceTransform( src, dst, label, distanceType, maskSize, labelType);

    double eps = 2e-4;

    SANITY_CHECK(label, eps);
    SANITY_CHECK(dst, eps);
}
