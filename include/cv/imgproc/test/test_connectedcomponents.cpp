

#include "test_precomp.hpp"
#include <string>
#include <vector>

using namespace cv;
using namespace std;

class CC_ConnectedComponentsTest : public cvtest::BaseTest
{
public:
    CC_ConnectedComponentsTest();
    ~CC_ConnectedComponentsTest();
protected:
    void run(int);
};

CC_ConnectedComponentsTest::CC_ConnectedComponentsTest() {}
CC_ConnectedComponentsTest::~CC_ConnectedComponentsTest() {}

// This function force a row major order for the labels
void normalizeLabels(Mat1i& imgLabels, int iNumLabels) {
    vector<int> vecNewLabels(iNumLabels + 1, 0);
    int iMaxNewLabel = 0;

    for (int r = 0; r<imgLabels.rows; ++r) {
        for (int c = 0; c<imgLabels.cols; ++c) {
            int iCurLabel = imgLabels(r, c);
            if (iCurLabel>0) {
                if (vecNewLabels[iCurLabel] == 0) {
                    vecNewLabels[iCurLabel] = ++iMaxNewLabel;
                }
                imgLabels(r, c) = vecNewLabels[iCurLabel];
            }
        }
    }
}


void CC_ConnectedComponentsTest::run( int /* start_from */)
{

    int ccltype[] = { CCL_WU, CCL_DEFAULT, CCL_GRANA };

    string exp_path = string(ts->get_data_path()) + "connectedcomponents/ccomp_exp.png";
    CvMat exp = imread(exp_path, 0);
    CvMat orig = imread(string(ts->get_data_path()) + "connectedcomponents/concentric_circles.png", 0);

    if (orig.empty())
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_TEST_DATA);
        return;
    }

    CvMat bw = orig > 128;

    for (uint cclt = 0; cclt < sizeof(ccltype)/sizeof(int); ++cclt)
    {

        Mat1i labelImage;
        int nLabels = connectedComponents(bw, labelImage, 8, CC_32S, ccltype[cclt]);

        normalizeLabels(labelImage, nLabels);

        // Validate test results
        for (int r = 0; r < labelImage.rows; ++r){
            for (int c = 0; c < labelImage.cols; ++c){
                int l = labelImage.at<int>(r, c);
                bool pass = l >= 0 && l <= nLabels;
                if (!pass){
                    ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_OUTPUT);
                    return;
                }
            }
        }

        if (exp.empty() || orig.size() != exp.size())
        {
            imwrite(exp_path, labelImage);
            exp = labelImage;
        }

        if (0 != cvtest::norm(labelImage > 0, exp > 0, NORM_INF))
        {
            ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
            return;
        }
        if (nLabels != cvtest::norm(labelImage, NORM_INF) + 1)
        {
            ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
            return;
        }

    }

    ts->set_failed_test_info(cvtest::TS::OK);
}

TEST(Imgproc_ConnectedComponents, regression) { CC_ConnectedComponentsTest test; test.safe_run(); }
