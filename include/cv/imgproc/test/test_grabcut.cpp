

#include "test_precomp.hpp"

#include <string>
#include <iostream>

using namespace std;
using namespace cv;

class CC_GrabcutTest : public cvtest::BaseTest
{
public:
    CC_GrabcutTest();
    ~CC_GrabcutTest();
protected:
    bool verify(const CvMat& mask, const CvMat& exp);
    void run(int);
};

CC_GrabcutTest::CC_GrabcutTest() {}
CC_GrabcutTest::~CC_GrabcutTest() {}

bool CC_GrabcutTest::verify(const CvMat& mask, const CvMat& exp)
{
    const float maxDiffRatio = 0.005f;
    int expArea = countNonZero( exp );
    int nonIntersectArea = countNonZero( mask != exp );

    float curRatio = (float)nonIntersectArea / (float)expArea;
    ts->printf( cvtest::TS::LOG, "nonIntersectArea/expArea = %f\n", curRatio );
    return curRatio < maxDiffRatio;
}

void CC_GrabcutTest::run( int /* start_from */)
{
    cvtest::DefaultRngAuto defRng;

    CvMat img = imread(string(ts->get_data_path()) + "shared/airplane.png");
    CvMat mask_prob = imread(string(ts->get_data_path()) + "grabcut/mask_prob.png", 0);
    CvMat exp_mask1 = imread(string(ts->get_data_path()) + "grabcut/exp_mask1.png", 0);
    CvMat exp_mask2 = imread(string(ts->get_data_path()) + "grabcut/exp_mask2.png", 0);

    if (img.empty() || (!mask_prob.empty() && img.size() != mask_prob.size()) ||
                       (!exp_mask1.empty() && img.size() != exp_mask1.size()) ||
                       (!exp_mask2.empty() && img.size() != exp_mask2.size()) )
    {
         ts->set_failed_test_info(cvtest::TS::FAIL_MISSING_TEST_DATA);
         return;
    }

    Rect rect(Point(24, 126), Point(483, 294));
    CvMat exp_bgdModel, exp_fgdModel;

    CvMat mask;
    mask = Scalar(0);
    CvMat bgdModel, fgdModel;
    grabCut( img, mask, rect, bgdModel, fgdModel, 0, GC_INIT_WITH_RECT );
    grabCut( img, mask, rect, bgdModel, fgdModel, 2, GC_EVAL );

    // Multiply images by 255 for more visuality of test data.
    if( mask_prob.empty() )
    {
        mask.copyTo( mask_prob );
        imwrite(string(ts->get_data_path()) + "grabcut/mask_prob.png", mask_prob);
    }
    if( exp_mask1.empty() )
    {
        exp_mask1 = (mask & 1) * 255;
        imwrite(string(ts->get_data_path()) + "grabcut/exp_mask1.png", exp_mask1);
    }

    if (!verify((mask & 1) * 255, exp_mask1))
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return;
    }

    mask = mask_prob;
    bgdModel.release();
    fgdModel.release();
    rect = Rect();
    grabCut( img, mask, rect, bgdModel, fgdModel, 0, GC_INIT_WITH_MASK );
    grabCut( img, mask, rect, bgdModel, fgdModel, 1, GC_EVAL );

    if( exp_mask2.empty() )
    {
        exp_mask2 = (mask & 1) * 255;
        imwrite(string(ts->get_data_path()) + "grabcut/exp_mask2.png", exp_mask2);
    }

    if (!verify((mask & 1) * 255, exp_mask2))
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return;
    }
    ts->set_failed_test_info(cvtest::TS::OK);
}

TEST(Imgproc_GrabCut, regression) { CC_GrabcutTest test; test.safe_run(); }

TEST(Imgproc_GrabCut, repeatability)
{
    cvtest::TS& ts = *cvtest::TS::ptr();

    CvMat image_1 = imread(string(ts.get_data_path()) + "grabcut/image1652.ppm", IMREAD_COLOR);
    CvMat mask_1 = imread(string(ts.get_data_path()) + "grabcut/mask1652.ppm", IMREAD_GRAYSCALE);
    Rect roi_1(0, 0, 150, 150);

    CvMat image_2 = image_1.clone();
    CvMat mask_2 = mask_1.clone();
    Rect roi_2 = roi_1;

    CvMat image_3 = image_1.clone();
    CvMat mask_3 = mask_1.clone();
    Rect roi_3 = roi_1;

    CvMat bgdModel_1, fgdModel_1;
    CvMat bgdModel_2, fgdModel_2;
    CvMat bgdModel_3, fgdModel_3;

    theRNG().state = 12378213;
    grabCut(image_1, mask_1, roi_1, bgdModel_1, fgdModel_1, 1, GC_INIT_WITH_MASK);
    theRNG().state = 12378213;
    grabCut(image_2, mask_2, roi_2, bgdModel_2, fgdModel_2, 1, GC_INIT_WITH_MASK);
    theRNG().state = 12378213;
    grabCut(image_3, mask_3, roi_3, bgdModel_3, fgdModel_3, 1, GC_INIT_WITH_MASK);

    EXPECT_EQ(0, countNonZero(mask_1 != mask_2));
    EXPECT_EQ(0, countNonZero(mask_1 != mask_3));
    EXPECT_EQ(0, countNonZero(mask_2 != mask_3));
}
