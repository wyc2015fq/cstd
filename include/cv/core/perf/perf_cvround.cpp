#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

template <typename T>
static void CvRoundMat(const CvMat & src, CvMat & dst)
{
    for (int y = 0; y < dst.rows; ++y)
    {
        const T * sptr = src.ptr<T>(y);
        int * dptr = dst.ptr<int>(y);

        for (int x = 0; x < dst.cols; ++x)
            dptr[x] = cvRound(sptr[x]);
    }
}

PERF_TEST_P(Size_MatType, CvRound_Float,
            testing::Combine(testing::Values(TYPICAL_MAT_SIZES),
                             testing::Values(CC_32FC1, CC_64FC1)))
{
    Size size = get<0>(GetParam());
    int type = get<1>(GetParam()), depth = CC_MAT_DEPTH(type);

    CvMat src(size, type), dst(size, CC_32SC1);

    declare.in(src, WARMUP_RNG).out(dst);

    if (depth == CC_32F)
    {
        TEST_CYCLE()
            CvRoundMat<float>(src, dst);
    }
    else if (depth == CC_64F)
    {
        TEST_CYCLE()
            CvRoundMat<double>(src, dst);
    }

    SANITY_CHECK_NOTHING();
}
