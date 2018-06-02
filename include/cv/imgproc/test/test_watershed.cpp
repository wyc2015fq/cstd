

#include "test_precomp.hpp"
#include <string>

using namespace cv;
using namespace std;

class CC_WatershedTest : public cvtest::BaseTest
{
public:
    CC_WatershedTest();
    ~CC_WatershedTest();
protected:
    void run(int);
};

CC_WatershedTest::CC_WatershedTest() {}
CC_WatershedTest::~CC_WatershedTest() {}

void CC_WatershedTest::run( int /* start_from */)
{
    string exp_path = string(ts->get_data_path()) + "watershed/wshed_exp.png";
    CvMat exp = imread(exp_path, 0);
    CvMat orig = imread(string(ts->get_data_path()) + "inpaint/orig.png");
    FileStorage fs(string(ts->get_data_path()) + "watershed/comp.xml", FileStorage::READ);

    if (orig.empty() || !fs.isOpened())
    {
        ts->set_failed_test_info( cvtest::TS::FAIL_INVALID_TEST_DATA );
        return;
    }

    CvSeq* cnts = (CvSeq*)fs["contours"].readObj();

    CvMat markers(orig.size(), CC_32SC1);
    markers = Scalar(0);
    IplImage iplmrks = markers;

    vector<unsigned char> colors(1);
    for(int i = 0; cnts != 0; cnts = cnts->h_next, ++i )
    {
        cvDrawContours( &iplmrks, cnts, Scalar::all(i + 1), Scalar::all(i + 1), -1, CC_FILLED);
        Point* p = (Point*)cvGetSeqElem(cnts, 0);

        //expected image was added with 1 in order to save to png
        //so now we substract 1 to get real color
        if(!exp.empty())
            colors.push_back(exp.ptr(p->y)[p->x] - 1);
    }
    fs.release();
    const int compNum = (int)(colors.size() - 1);

    watershed(orig, markers);

    for(int j = 0; j < markers.rows; ++j)
    {
        int* line = markers.ptr<int>(j);
        for(int i = 0; i < markers.cols; ++i)
        {
            int& pixel = line[i];

            if (pixel == -1) // border
                continue;

            if (pixel <= 0 || pixel > compNum)
                continue; // bad result, doing nothing and going to get error latter;

            // repaint in saved color to compare with expected;
            if(!exp.empty())
                pixel = colors[pixel];
        }
    }

    CvMat markers8U;
    markers.convertTo(markers8U, CC_8U, 1, 1);

    if( exp.empty() || orig.size() != exp.size() )
    {
        imwrite(exp_path, markers8U);
        exp = markers8U;
    }

    if (0 != norm(markers8U, exp, NORM_INF))
    {
        ts->set_failed_test_info( cvtest::TS::FAIL_MISMATCH );
        return;
    }
    ts->set_failed_test_info(cvtest::TS::OK);
}

TEST(Imgproc_Watershed, regression) { CC_WatershedTest test; test.safe_run(); }
