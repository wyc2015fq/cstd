

#include "test_precomp.hpp"

#ifdef HAVE_TBB
#include "tbb/task_scheduler_init.h"
#endif

using namespace cv;
using namespace std;

class CC_solvePnPRansac_Test : public cvtest::BaseTest
{
public:
    CC_solvePnPRansac_Test()
    {
        eps[SOLVEPNP_ITERATIVE] = 1.0e-2;
        eps[SOLVEPNP_EPNP] = 1.0e-2;
        eps[SOLVEPNP_P3P] = 1.0e-2;
        eps[SOLVEPNP_DLS] = 1.0e-2;
        eps[SOLVEPNP_UPNP] = 1.0e-2;
        totalTestsCount = 10;
    }
    ~CC_solvePnPRansac_Test() {}
protected:
    void generate3DPointCloud(vector<Point3f>& points, Point3f pmin = Point3f(-1,
        -1, 5), Point3f pmax = Point3f(1, 1, 10))
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

    void generateCameraMatrix(CvMat& cameraMatrix, RNG& rng)
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

    void generateDistCoeffs(CvMat& distCoeffs, RNG& rng)
    {
        distCoeffs = CvMat::zeros(4, 1, CC_64FC1);
        for (int i = 0; i < 3; i++)
            distCoeffs.at<double>(i,0) = rng.uniform(0.0, 1.0e-6);
    }

    void generatePose(CvMat& rvec, CvMat& tvec, RNG& rng)
    {
        const double minVal = 1.0e-3;
        const double maxVal = 1.0;
        rvec.create(3, 1, CC_64FC1);
        tvec.create(3, 1, CC_64FC1);
        for (int i = 0; i < 3; i++)
        {
            rvec.at<double>(i,0) = rng.uniform(minVal, maxVal);
            tvec.at<double>(i,0) = rng.uniform(minVal, maxVal/10);
        }
    }

    virtual bool runTest(RNG& rng, int mode, int method, const vector<Point3f>& points, const double* epsilon, double& maxError)
    {
        CvMat rvec, tvec;
        vector<int> inliers;
        CvMat trueRvec, trueTvec;
        CvMat intrinsics, distCoeffs;
        generateCameraMatrix(intrinsics, rng);
        if (method == 4) intrinsics.at<double>(1,1) = intrinsics.at<double>(0,0);
        if (mode == 0)
            distCoeffs = CvMat::zeros(4, 1, CC_64FC1);
        else
            generateDistCoeffs(distCoeffs, rng);
        generatePose(trueRvec, trueTvec, rng);

        vector<Point2f> projectedPoints;
        projectedPoints.resize(points.size());
        projectPoints(CvMat(points), trueRvec, trueTvec, intrinsics, distCoeffs, projectedPoints);
        for (size_t i = 0; i < projectedPoints.size(); i++)
        {
            if (i % 20 == 0)
            {
                projectedPoints[i] = projectedPoints[rng.uniform(0,(int)points.size()-1)];
            }
        }

        solvePnPRansac(points, projectedPoints, intrinsics, distCoeffs, rvec, tvec,
            false, 500, 0.5f, 0.99, inliers, method);

        bool isTestSuccess = inliers.size() >= points.size()*0.95;

        double rvecDiff = norm(rvec-trueRvec), tvecDiff = norm(tvec-trueTvec);
        isTestSuccess = isTestSuccess && rvecDiff < epsilon[method] && tvecDiff < epsilon[method];
        double error = rvecDiff > tvecDiff ? rvecDiff : tvecDiff;
        //cout << error << " " << inliers.size() << " " << eps[method] << endl;
        if (error > maxError)
            maxError = error;

        return isTestSuccess;
    }

    void run(int)
    {
        ts->set_failed_test_info(cvtest::TS::OK);

        vector<Point3f> points, points_dls;
        const int pointsCount = 500;
        points.resize(pointsCount);
        generate3DPointCloud(points);

        const int methodsCount = 5;
        RNG rng = ts->get_rng();


        for (int mode = 0; mode < 2; mode++)
        {
            for (int method = 0; method < methodsCount; method++)
            {
                double maxError = 0;
                int successfulTestsCount = 0;
                for (int testIndex = 0; testIndex < totalTestsCount; testIndex++)
                {
                    if (runTest(rng, mode, method, points, eps, maxError))
                        successfulTestsCount++;
                }
                //cout <<  maxError << " " << successfulTestsCount << endl;
                if (successfulTestsCount < 0.7*totalTestsCount)
                {
                    ts->printf( cvtest::TS::LOG, "Invalid accuracy for method %d, failed %d tests from %d, maximum error equals %f, distortion mode equals %d\n",
                        method, totalTestsCount - successfulTestsCount, totalTestsCount, maxError, mode);
                    ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
                }
                cout << "mode: " << mode << ", method: " << method << " -> "
                     << ((double)successfulTestsCount / totalTestsCount) * 100 << "%"
                     << " (err < " << maxError << ")" << endl;
            }
        }
    }
    double eps[5];
    int totalTestsCount;
};

class CC_solvePnP_Test : public CC_solvePnPRansac_Test
{
public:
    CC_solvePnP_Test()
    {
        eps[SOLVEPNP_ITERATIVE] = 1.0e-6;
        eps[SOLVEPNP_EPNP] = 1.0e-6;
        eps[SOLVEPNP_P3P] = 1.0e-4;
        eps[SOLVEPNP_DLS] = 1.0e-4;
        eps[SOLVEPNP_UPNP] = 1.0e-4;
        totalTestsCount = 1000;
    }

    ~CC_solvePnP_Test() {}
protected:
    virtual bool runTest(RNG& rng, int mode, int method, const vector<Point3f>& points, const double* epsilon, double& maxError)
    {
        CvMat rvec, tvec;
        CvMat trueRvec, trueTvec;
        CvMat intrinsics, distCoeffs;
        generateCameraMatrix(intrinsics, rng);
        if (method == 4) intrinsics.at<double>(1,1) = intrinsics.at<double>(0,0);
        if (mode == 0)
            distCoeffs = CvMat::zeros(4, 1, CC_64FC1);
        else
            generateDistCoeffs(distCoeffs, rng);
        generatePose(trueRvec, trueTvec, rng);

        std::vector<Point3f> opoints;
        if (method == 2)
        {
            opoints = std::vector<Point3f>(points.begin(), points.begin()+4);
        }
        else if(method == 3)
        {
            opoints = std::vector<Point3f>(points.begin(), points.begin()+50);
        }
        else
            opoints = points;

        vector<Point2f> projectedPoints;
        projectedPoints.resize(opoints.size());
        projectPoints(CvMat(opoints), trueRvec, trueTvec, intrinsics, distCoeffs, projectedPoints);

        solvePnP(opoints, projectedPoints, intrinsics, distCoeffs, rvec, tvec,
            false, method);

        double rvecDiff = norm(rvec-trueRvec), tvecDiff = norm(tvec-trueTvec);
        bool isTestSuccess = rvecDiff < epsilon[method] && tvecDiff < epsilon[method];

        double error = rvecDiff > tvecDiff ? rvecDiff : tvecDiff;
        if (error > maxError)
            maxError = error;

        return isTestSuccess;
    }
};

TEST(Calib3d_SolvePnPRansac, accuracy) { CC_solvePnPRansac_Test test; test.safe_run(); }
TEST(Calib3d_SolvePnP, accuracy) { CC_solvePnP_Test test; test.safe_run(); }

TEST(Calib3d_SolvePnPRansac, concurrency)
{
    int count = 7*13;

    CvMat object(1, count, CC_32FC3);
    randu(object, -100, 100);

    CvMat camera_mat(3, 3, CC_32FC1);
    randu(camera_mat, 0.5, 1);
    camera_mat.at<float>(0, 1) = 0.f;
    camera_mat.at<float>(1, 0) = 0.f;
    camera_mat.at<float>(2, 0) = 0.f;
    camera_mat.at<float>(2, 1) = 0.f;

    CvMat dist_coef(1, 8, CC_32F, Scalar::all(0));

    vector<Point2f> image_vec;
    CvMat rvec_gold(1, 3, CC_32FC1);
    randu(rvec_gold, 0, 1);
    CvMat tvec_gold(1, 3, CC_32FC1);
    randu(tvec_gold, 0, 1);
    projectPoints(object, rvec_gold, tvec_gold, camera_mat, dist_coef, image_vec);

    CvMat image(1, count, CC_32FC2, &image_vec[0]);

    CvMat rvec1, rvec2;
    CvMat tvec1, tvec2;

    {
        // limit concurrency to get deterministic result
        theRNG().state = 20121010;
        setNumThreads(1);
        solvePnPRansac(object, image, camera_mat, dist_coef, rvec1, tvec1);
    }

    {
        CvMat rvec;
        CvMat tvec;
        // parallel executions
        for(int i = 0; i < 10; ++i)
        {
            theRNG().state = 20121010;
            solvePnPRansac(object, image, camera_mat, dist_coef, rvec, tvec);
        }
    }

    {
        // single thread again
        theRNG().state = 20121010;
        setNumThreads(1);
        solvePnPRansac(object, image, camera_mat, dist_coef, rvec2, tvec2);
    }

    double rnorm = cvtest::norm(rvec1, rvec2, NORM_INF);
    double tnorm = cvtest::norm(tvec1, tvec2, NORM_INF);

    EXPECT_LT(rnorm, 1e-6);
    EXPECT_LT(tnorm, 1e-6);
}

TEST(Calib3d_SolvePnPRansac, input_type)
{
    const int numPoints = 10;
    Matx33d intrinsics(5.4794130238156129e+002, 0., 2.9835545700043139e+002, 0.,
        5.4817724002728005e+002, 2.3062194051986233e+002, 0., 0., 1.);

    std::vector<Point3f> points3d;
    std::vector<Point2f> points2d;
    for (int i = 0; i < numPoints; i+=2)
    {
        points3d.push_back(Point3i(5+i, 3, 2));
        points3d.push_back(Point3i(5+i, 3+i, 2+i));
        points2d.push_back(Point2i(0, i));
        points2d.push_back(Point2i(-i, i));
    }
    CvMat R1, t1, R2, t2, R3, t3, R4, t4;

    EXPECT_TRUE(solvePnPRansac(points3d, points2d, intrinsics, CvMat(), R1, t1));

    CvMat points3dMat(points3d);
    CvMat points2dMat(points2d);
    EXPECT_TRUE(solvePnPRansac(points3dMat, points2dMat, intrinsics, CvMat(), R2, t2));

    points3dMat = points3dMat.reshape(3, 1);
    points2dMat = points2dMat.reshape(2, 1);
    EXPECT_TRUE(solvePnPRansac(points3dMat, points2dMat, intrinsics, CvMat(), R3, t3));

    points3dMat = points3dMat.reshape(1, numPoints);
    points2dMat = points2dMat.reshape(1, numPoints);
    EXPECT_TRUE(solvePnPRansac(points3dMat, points2dMat, intrinsics, CvMat(), R4, t4));

    EXPECT_LE(norm(R1, R2, NORM_INF), 1e-6);
    EXPECT_LE(norm(t1, t2, NORM_INF), 1e-6);
    EXPECT_LE(norm(R1, R3, NORM_INF), 1e-6);
    EXPECT_LE(norm(t1, t3, NORM_INF), 1e-6);
    EXPECT_LE(norm(R1, R4, NORM_INF), 1e-6);
    EXPECT_LE(norm(t1, t4, NORM_INF), 1e-6);
}

TEST(Calib3d_SolvePnP, double_support)
{
    Matx33d intrinsics(5.4794130238156129e+002, 0., 2.9835545700043139e+002, 0.,
                       5.4817724002728005e+002, 2.3062194051986233e+002, 0., 0., 1.);
    std::vector<Point3d> points3d;
    std::vector<Point2d> points2d;
    std::vector<Point3f> points3dF;
    std::vector<Point2f> points2dF;
    for (int i = 0; i < 10 ; i+=2)
    {
        points3d.push_back(Point3d(5+i, 3, 2));
        points3dF.push_back(Point3d(5+i, 3, 2));
        points3d.push_back(Point3d(5+i, 3+i, 2+i));
        points3dF.push_back(Point3d(5+i, 3+i, 2+i));
        points2d.push_back(Point2d(0, i));
        points2dF.push_back(Point2d(0, i));
        points2d.push_back(Point2d(-i, i));
        points2dF.push_back(Point2d(-i, i));
    }
    CvMat R,t, RF, tF;
    vector<int> inliers;

    solvePnPRansac(points3dF, points2dF, intrinsics, CvMat(), RF, tF, true, 100, 8.f, 0.999, inliers, SOLVEPNP_P3P);
    solvePnPRansac(points3d, points2d, intrinsics, CvMat(), R, t, true, 100, 8.f, 0.999, inliers, SOLVEPNP_P3P);

    EXPECT_LE(norm(R, Mat_<double>(RF), NORM_INF), 1e-3);
    EXPECT_LE(norm(t, Mat_<double>(tF), NORM_INF), 1e-3);
}

TEST(Calib3d_SolvePnP, translation)
{
    CvMat cameraIntrinsic = CvMat::eye(3,3, CC_32FC1);
    vector<float> crvec;
    crvec.push_back(0.f);
    crvec.push_back(0.f);
    crvec.push_back(0.f);
    vector<float> ctvec;
    ctvec.push_back(100.f);
    ctvec.push_back(100.f);
    ctvec.push_back(0.f);
    vector<Point3f> p3d;
    p3d.push_back(Point3f(0,0,0));
    p3d.push_back(Point3f(0,0,10));
    p3d.push_back(Point3f(0,10,10));
    p3d.push_back(Point3f(10,10,10));
    p3d.push_back(Point3f(2,5,5));

    vector<Point2f> p2d;
    projectPoints(p3d, crvec, ctvec, cameraIntrinsic, noArray(), p2d);
    CvMat rvec;
    CvMat tvec;
    rvec =(Mat_<float>(3,1) << 0, 0, 0);
    tvec = (Mat_<float>(3,1) << 100, 100, 0);

    solvePnP(p3d, p2d, cameraIntrinsic, noArray(), rvec, tvec, true);
    EXPECT_TRUE(checkRange(rvec));
    EXPECT_TRUE(checkRange(tvec));

    rvec =(Mat_<double>(3,1) << 0, 0, 0);
    tvec = (Mat_<double>(3,1) << 100, 100, 0);
    solvePnP(p3d, p2d, cameraIntrinsic, noArray(), rvec, tvec, true);
    EXPECT_TRUE(checkRange(rvec));
    EXPECT_TRUE(checkRange(tvec));

    solvePnP(p3d, p2d, cameraIntrinsic, noArray(), rvec, tvec, false);
    EXPECT_TRUE(checkRange(rvec));
    EXPECT_TRUE(checkRange(tvec));
}
