#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

CC_ENUM(MethodType, TM_SQDIFF, TM_SQDIFF_NORMED, TM_CCORR, TM_CCORR_NORMED, TM_CCOEFF, TM_CCOEFF_NORMED)

typedef std::tr1::tuple<CvSize, CvSize, MethodType> ImgSize_TmplSize_Method_t;
typedef perf::TestBaseWithParam<ImgSize_TmplSize_Method_t> ImgSize_TmplSize_Method;

PERF_TEST_P(ImgSize_TmplSize_Method, matchTemplateSmall,
            testing::Combine(
                testing::Values(szSmall128, CvSize(320, 240),
                                CvSize(640, 480), CvSize(800, 600),
                                CvSize(1024, 768), CvSize(1280, 1024)),
                testing::Values(CvSize(12, 12), CvSize(28, 9),
                                CvSize(8, 30), CvSize(16, 16)),
                MethodType::all()
                )
            )
{
    CvSize imgSz = get<0>(GetParam());
    CvSize tmplSz = get<1>(GetParam());
    int method = get<2>(GetParam());

    CvMat img(imgSz, CC_8UC1);
    CvMat tmpl(tmplSz, CC_8UC1);
    CvMat result(imgSz - tmplSz + CvSize(1,1), CC_32F);

    declare
        .in(img, WARMUP_RNG)
        .in(tmpl, WARMUP_RNG)
        .out(result)
        .time(30);

    TEST_CYCLE() matchTemplate(img, tmpl, result, method);

    bool isNormed =
        method == TM_CCORR_NORMED ||
        method == TM_SQDIFF_NORMED ||
        method == TM_CCOEFF_NORMED;
    double eps = isNormed ? 1e-5
        : 255 * 255 * tmpl.total() * 1e-6;

    SANITY_CHECK(result, eps);
}

PERF_TEST_P(ImgSize_TmplSize_Method, matchTemplateBig,
            testing::Combine(
                testing::Values(CvSize(1280, 1024)),
                testing::Values(CvSize(1260, 1000), CvSize(1261, 1013)),
                MethodType::all()
                )
    )
{
    CvSize imgSz = get<0>(GetParam());
    CvSize tmplSz = get<1>(GetParam());
    int method = get<2>(GetParam());

    CvMat img(imgSz, CC_8UC1);
    CvMat tmpl(tmplSz, CC_8UC1);
    CvMat result(imgSz - tmplSz + CvSize(1,1), CC_32F);

    declare
        .in(img, WARMUP_RNG)
        .in(tmpl, WARMUP_RNG)
        .out(result)
        .time(30);

    TEST_CYCLE() matchTemplate(img, tmpl, result, method);

    bool isNormed =
        method == TM_CCORR_NORMED ||
        method == TM_SQDIFF_NORMED ||
        method == TM_CCOEFF_NORMED;
    double eps = isNormed ? 1e-6
        : 255.0 * 255.0 * (double)tmpl.total() * 1e-6;

    SANITY_CHECK(result, eps);
}
