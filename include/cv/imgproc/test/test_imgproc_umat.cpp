

#include "test_precomp.hpp"
#include <string>

using namespace cv;
using namespace std;

class CC_ImgprocUMatTest : public cvtest::BaseTest
{
public:
    CC_ImgprocUMatTest() {}
    ~CC_ImgprocUMatTest() {}
protected:
    void run(int)
    {
        string imgpath = string(ts->get_data_path()) + "shared/lena.png";
        CvMat img = imread(imgpath, 1), gray, smallimg, result;
        UMat uimg = img.getUMat(ACCESS_READ), ugray, usmallimg, uresult;

        cvtColor(img, gray, COLOR_BGR2GRAY);
        resize(gray, smallimg, CvSize(), 0.75, 0.75, INTER_LINEAR);
        equalizeHist(smallimg, result);

        cvtColor(uimg, ugray, COLOR_BGR2GRAY);
        resize(ugray, usmallimg, CvSize(), 0.75, 0.75, INTER_LINEAR);
        equalizeHist(usmallimg, uresult);

#if 0
        imshow("orig", uimg);
        imshow("small", usmallimg);
        imshow("equalized gray", uresult);
        waitKey();
        destroyWindow("orig");
        destroyWindow("small");
        destroyWindow("equalized gray");
#endif
        ts->set_failed_test_info(cvtest::TS::OK);

        (void)uresult.getMat(ACCESS_READ);
    }
};

TEST(Imgproc_UMat, regression) { CC_ImgprocUMatTest test; test.safe_run(); }
