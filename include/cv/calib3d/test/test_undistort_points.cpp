#include "test_precomp.hpp"
#include <string>

using namespace cv;
using namespace std;

class CC_UndistortTest : public cvtest::BaseTest
{
public:
    CC_UndistortTest();
    ~CC_UndistortTest();
protected:
    void run(int);
private:
    void generate3DPointCloud(vector<Point3f>& points, Point3f pmin = Point3f(-1,
    -1, 5), Point3f pmax = Point3f(1, 1, 10));
    void generateCameraMatrix(CvMat& cameraMatrix);
    void generateDistCoeffs(CvMat& distCoeffs, int count);

    double thresh;
    RNG rng;
};

CC_UndistortTest::CC_UndistortTest()
{
    thresh = 1.0e-2;
}
CC_UndistortTest::~CC_UndistortTest() {}

void CC_UndistortTest::generate3DPointCloud(vector<Point3f>& points, Point3f pmin, Point3f pmax)
{
    const Point3f delta = pmax - pmin;
    for (size_t i = 0; i < points.size(); i++)
    {
        Point3f p(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX,
            float(rand()) / RAND_MAX);
        p.x *= delta.x;
        p.y *= delta.y;
        p.z *= delta.z;
        p = p + pmin;
        points[i] = p;
    }
}
void CC_UndistortTest::generateCameraMatrix(CvMat& cameraMatrix)
{
    const double fcMinVal = 1e-3;
    const double fcMaxVal = 100;
    cameraMatrix.create(3, 3, CC_64FC1);
    cameraMatrix.setTo(Scalar(0));
    cameraMatrix.at<double>(0,0) = rng.uniform(fcMinVal, fcMaxVal);
    cameraMatrix.at<double>(1,1) = rng.uniform(fcMinVal, fcMaxVal);
    cameraMatrix.at<double>(0,2) = rng.uniform(fcMinVal, fcMaxVal);
    cameraMatrix.at<double>(1,2) = rng.uniform(fcMinVal, fcMaxVal);
    cameraMatrix.at<double>(2,2) = 1;
}
void CC_UndistortTest::generateDistCoeffs(CvMat& distCoeffs, int count)
{
    distCoeffs = CvMat::zeros(count, 1, CC_64FC1);
    for (int i = 0; i < count; i++)
        distCoeffs.at<double>(i,0) = rng.uniform(0.0, 1.0e-3);
}

void CC_UndistortTest::run(int /* start_from */)
{
    CvMat intrinsics, distCoeffs;
    generateCameraMatrix(intrinsics);
    vector<Point3f> points(500);
    generate3DPointCloud(points);
    vector<Point2f> projectedPoints;
    projectedPoints.resize(points.size());

    int modelMembersCount[] = {4,5,8};
    for (int idx = 0; idx < 3; idx++)
    {
        generateDistCoeffs(distCoeffs, modelMembersCount[idx]);
        projectPoints(CvMat(points), CvMat::zeros(3,1,CC_64FC1), CvMat::zeros(3,1,CC_64FC1), intrinsics, distCoeffs, projectedPoints);

        vector<Point2f> realUndistortedPoints;
        projectPoints(CvMat(points), CvMat::zeros(3,1,CC_64FC1), CvMat::zeros(3,1,CC_64FC1), intrinsics,  CvMat::zeros(4,1,CC_64FC1), realUndistortedPoints);

        CvMat undistortedPoints;
        undistortPoints(CvMat(projectedPoints), undistortedPoints, intrinsics, distCoeffs);

        CvMat p;
        perspectiveTransform(undistortedPoints, p, intrinsics);
        undistortedPoints = p;
        double diff = cvtest::norm(CvMat(realUndistortedPoints), undistortedPoints, NORM_L2);
        if (diff > thresh)
        {
            ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
            return;
        }
        ts->set_failed_test_info(cvtest::TS::OK);
    }
}

TEST(Calib3d_Undistort, accuracy) { CC_UndistortTest test; test.safe_run(); }
