

#include "test_precomp.hpp"

using namespace cv;
using namespace std;

namespace cvtest
{

/// phase correlation
class CC_PhaseCorrelatorTest : public cvtest::ArrayTest
{
public:
    CC_PhaseCorrelatorTest();
protected:
    void run( int );
};

CC_PhaseCorrelatorTest::CC_PhaseCorrelatorTest() {}

void CC_PhaseCorrelatorTest::run( int )
{
    ts->set_failed_test_info(cvtest::TS::OK);

    CvMat r1 = CvMat::ones(CvSize(129, 128), CC_64F);
    CvMat r2 = CvMat::ones(CvSize(129, 128), CC_64F);

    double expectedShiftX = -10.0;
    double expectedShiftY = -20.0;

    // draw 10x10 rectangles @ (100, 100) and (90, 80) should see ~(-10, -20) shift here...
    rectangle(r1, Point(100, 100), Point(110, 110), Scalar(0, 0, 0), CC_FILLED);
    rectangle(r2, Point(90, 80), Point(100, 90), Scalar(0, 0, 0), CC_FILLED);

    CvMat hann;
    createHanningWindow(hann, r1.size(), CC_64F);
    Point2d phaseShift = phaseCorrelate(r1, r2, hann);

    // test accuracy should be less than 1 pixel...
    if((expectedShiftX - phaseShift.x) >= 1 || (expectedShiftY - phaseShift.y) >= 1)
    {
         ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
    }
}

TEST(Imgproc_PhaseCorrelatorTest, accuracy) { CC_PhaseCorrelatorTest test; test.safe_run(); }

}
