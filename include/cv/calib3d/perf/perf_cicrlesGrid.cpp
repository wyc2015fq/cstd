#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

typedef std::tr1::tuple<std::string, CvSize> String_Size_t;
typedef perf::TestBaseWithParam<String_Size_t> String_Size;

PERF_TEST_P(String_Size, asymm_circles_grid, testing::Values(
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles1.png", CvSize(7,13)),
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles2.png", CvSize(7,13)),
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles3.png", CvSize(7,13)),
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles4.png", CvSize(5,5)),
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles5.png", CvSize(5,5)),
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles6.png", CvSize(5,5)),
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles7.png", CvSize(3,9)),
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles8.png", CvSize(3,9)),
                String_Size_t("cv/cameracalibration/asymmetric_circles/acircles9.png", CvSize(3,9))
                )
            )
{
    string filename = getDataPath(get<0>(GetParam()));
    CvSize gridSize = get<1>(GetParam());

    CvMat frame = imread(filename);
    if (frame.empty())
        FAIL() << "Unable to load source image " << filename;

    vector<Point2f> ptvec;
    ptvec.resize(gridSize.area());

    cvtColor(frame, frame, COLOR_BGR2GRAY);

    declare.in(frame).out(ptvec);

    TEST_CYCLE() ASSERT_TRUE(findCirclesGrid(frame, gridSize, ptvec, CALIB_CB_CLUSTERING | CALIB_CB_ASYMMETRIC_GRID));

    SANITY_CHECK(ptvec, 2);
}
