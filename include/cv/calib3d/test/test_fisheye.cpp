

#include "test_precomp.hpp"
#include <opencv2/ts/cuda_test.hpp>
#include "../src/fisheye.hpp"
#include "opencv2/videoio.hpp"

class fisheyeTest : public ::testing::Test {

protected:
    const static CvSize imageSize;
    const static Matx33d K;
    const static Vec4d D;
    const static Matx33d R;
    const static Vec3d T;
    std::string datasets_repository_path;

    virtual void SetUp() {
        datasets_repository_path = combine(cvtest::TS::ptr()->get_data_path(), "cv/cameracalibration/fisheye");
    }

protected:
    std::string combine(const std::string& _item1, const std::string& _item2);
    CvMat mergeRectification(const CvMat& l, const CvMat& r);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  TESTS::

TEST_F(fisheyeTest, projectPoints)
{
    double cols = this->imageSize.width,
           rows = this->imageSize.height;

    const int N = 20;
    CvMat distorted0(1, N*N, CC_64FC2), undist1, undist2, distorted1, distorted2;
    undist2.create(distorted0.size(), CC_MAKETYPE(distorted0.depth(), 3));
    Vec2d* pts = distorted0.ptr<Vec2d>();

    Vec2d c(this->K(0, 2), this->K(1, 2));
    for(int y = 0, k = 0; y < N; ++y)
        for(int x = 0; x < N; ++x)
        {
            Vec2d point(x*cols/(N-1.f), y*rows/(N-1.f));
            pts[k++] = (point - c) * 0.85 + c;
        }

    fisheye::undistortPoints(distorted0, undist1, this->K, this->D);

    Vec2d* u1 = undist1.ptr<Vec2d>();
    Vec3d* u2 = undist2.ptr<Vec3d>();
    for(int i = 0; i  < (int)distorted0.total(); ++i)
        u2[i] = Vec3d(u1[i][0], u1[i][1], 1.0);

    fisheye::distortPoints(undist1, distorted1, this->K, this->D);
    fisheye::projectPoints(undist2, distorted2, Vec3d::all(0), Vec3d::all(0), this->K, this->D);

    EXPECT_MAT_NEAR(distorted0, distorted1, 1e-10);
    EXPECT_MAT_NEAR(distorted0, distorted2, 1e-10);
}

TEST_F(fisheyeTest, DISABLED_undistortImage)
{
    Matx33d theK = this->K;
    CvMat theD = CvMat(this->D);
    std::string file = combine(datasets_repository_path, "/calib-3_stereo_from_JY/left/stereo_pair_014.jpg");
    Matx33d newK = theK;
    CvMat distorted = imread(file), undistorted;
    {
        newK(0, 0) = 100;
        newK(1, 1) = 100;
        fisheye::undistortImage(distorted, undistorted, theK, theD, newK);
        CvMat correct = imread(combine(datasets_repository_path, "new_f_100.png"));
        if (correct.empty())
            CC_Assert(imwrite(combine(datasets_repository_path, "new_f_100.png"), undistorted));
        else
            EXPECT_MAT_NEAR(correct, undistorted, 1e-10);
    }
    {
        double balance = 1.0;
        fisheye::estimateNewCameraMatrixForUndistortRectify(theK, theD, distorted.size(), noArray(), newK, balance);
        fisheye::undistortImage(distorted, undistorted, theK, theD, newK);
        CvMat correct = imread(combine(datasets_repository_path, "balance_1.0.png"));
        if (correct.empty())
            CC_Assert(imwrite(combine(datasets_repository_path, "balance_1.0.png"), undistorted));
        else
            EXPECT_MAT_NEAR(correct, undistorted, 1e-10);
    }

    {
        double balance = 0.0;
        fisheye::estimateNewCameraMatrixForUndistortRectify(theK, theD, distorted.size(), noArray(), newK, balance);
        fisheye::undistortImage(distorted, undistorted, theK, theD, newK);
        CvMat correct = imread(combine(datasets_repository_path, "balance_0.0.png"));
        if (correct.empty())
            CC_Assert(imwrite(combine(datasets_repository_path, "balance_0.0.png"), undistorted));
        else
            EXPECT_MAT_NEAR(correct, undistorted, 1e-10);
    }
}

TEST_F(fisheyeTest, jacobians)
{
    int n = 10;
    CvMat X(1, n, CC_64FC3);
    CvMat om(3, 1, CC_64F), theT(3, 1, CC_64F);
    CvMat f(2, 1, CC_64F), c(2, 1, CC_64F);
    CvMat k(4, 1, CC_64F);
    double alpha;

    RNG r;

    r.fill(X, RNG::NORMAL, 2, 1);
    X = abs(X) * 10;

    r.fill(om, RNG::NORMAL, 0, 1);
    om = abs(om);

    r.fill(theT, RNG::NORMAL, 0, 1);
    theT = abs(theT); theT.at<double>(2) = 4; theT *= 10;

    r.fill(f, RNG::NORMAL, 0, 1);
    f = abs(f) * 1000;

    r.fill(c, RNG::NORMAL, 0, 1);
    c = abs(c) * 1000;

    r.fill(k, RNG::NORMAL, 0, 1);
    k*= 0.5;

    alpha = 0.01*r.gaussian(1);

    CvMat x1, x2, xpred;
    Matx33d theK(f.at<double>(0), alpha * f.at<double>(0), c.at<double>(0),
                     0,            f.at<double>(1), c.at<double>(1),
                     0,            0,    1);

    CvMat jacobians;
    fisheye::projectPoints(X, x1, om, theT, theK, k, alpha, jacobians);

    //test on T:
    CvMat dT(3, 1, CC_64FC1);
    r.fill(dT, RNG::NORMAL, 0, 1);
    dT *= 1e-9*norm(theT);
    CvMat T2 = theT + dT;
    fisheye::projectPoints(X, x2, om, T2, theK, k, alpha, noArray());
    xpred = x1 + CvMat(jacobians.colRange(11,14) * dT).reshape(2, 1);
    CC_Assert (norm(x2 - xpred) < 1e-10);

    //test on om:
    CvMat dom(3, 1, CC_64FC1);
    r.fill(dom, RNG::NORMAL, 0, 1);
    dom *= 1e-9*norm(om);
    CvMat om2 = om + dom;
    fisheye::projectPoints(X, x2, om2, theT, theK, k, alpha, noArray());
    xpred = x1 + CvMat(jacobians.colRange(8,11) * dom).reshape(2, 1);
    CC_Assert (norm(x2 - xpred) < 1e-10);

    //test on f:
    CvMat df(2, 1, CC_64FC1);
    r.fill(df, RNG::NORMAL, 0, 1);
    df *= 1e-9*norm(f);
    Matx33d K2 = theK + Matx33d(df.at<double>(0), df.at<double>(0) * alpha, 0, 0, df.at<double>(1), 0, 0, 0, 0);
    fisheye::projectPoints(X, x2, om, theT, K2, k, alpha, noArray());
    xpred = x1 + CvMat(jacobians.colRange(0,2) * df).reshape(2, 1);
    CC_Assert (norm(x2 - xpred) < 1e-10);

    //test on c:
    CvMat dc(2, 1, CC_64FC1);
    r.fill(dc, RNG::NORMAL, 0, 1);
    dc *= 1e-9*norm(c);
    K2 = theK + Matx33d(0, 0, dc.at<double>(0), 0, 0, dc.at<double>(1), 0, 0, 0);
    fisheye::projectPoints(X, x2, om, theT, K2, k, alpha, noArray());
    xpred = x1 + CvMat(jacobians.colRange(2,4) * dc).reshape(2, 1);
    CC_Assert (norm(x2 - xpred) < 1e-10);

    //test on k:
    CvMat dk(4, 1, CC_64FC1);
    r.fill(dk, RNG::NORMAL, 0, 1);
    dk *= 1e-9*norm(k);
    CvMat k2 = k + dk;
    fisheye::projectPoints(X, x2, om, theT, theK, k2, alpha, noArray());
    xpred = x1 + CvMat(jacobians.colRange(4,8) * dk).reshape(2, 1);
    CC_Assert (norm(x2 - xpred) < 1e-10);

    //test on alpha:
    CvMat dalpha(1, 1, CC_64FC1);
    r.fill(dalpha, RNG::NORMAL, 0, 1);
    dalpha *= 1e-9*norm(f);
    double alpha2 = alpha + dalpha.at<double>(0);
    K2 = theK + Matx33d(0, f.at<double>(0) * dalpha.at<double>(0), 0, 0, 0, 0, 0, 0, 0);
    fisheye::projectPoints(X, x2, om, theT, theK, k, alpha2, noArray());
    xpred = x1 + CvMat(jacobians.col(14) * dalpha).reshape(2, 1);
    CC_Assert (norm(x2 - xpred) < 1e-10);
}

TEST_F(fisheyeTest, Calibration)
{
    const int n_images = 34;

    std::vector<std::vector<Point2d> > imagePoints(n_images);
    std::vector<std::vector<Point3d> > objectPoints(n_images);

    const std::string folder =combine(datasets_repository_path, "calib-3_stereo_from_JY");
    FileStorage fs_left(combine(folder, "left.xml"), FileStorage::READ);
    CC_Assert(fs_left.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_left[format("image_%d", i )] >> imagePoints[i];
    fs_left.release();

    FileStorage fs_object(combine(folder, "object.xml"), FileStorage::READ);
    CC_Assert(fs_object.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_object[format("image_%d", i )] >> objectPoints[i];
    fs_object.release();

    int flag = 0;
    flag |= fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flag |= fisheye::CALIB_CHECK_COND;
    flag |= fisheye::CALIB_FIX_SKEW;

    Matx33d theK;
    Vec4d theD;

    fisheye::calibrate(objectPoints, imagePoints, imageSize, theK, theD,
                           noArray(), noArray(), flag, TermCriteria(3, 20, 1e-6));

    EXPECT_MAT_NEAR(theK, this->K, 1e-10);
    EXPECT_MAT_NEAR(theD, this->D, 1e-10);
}

TEST_F(fisheyeTest, Homography)
{
    const int n_images = 1;

    std::vector<std::vector<Point2d> > imagePoints(n_images);
    std::vector<std::vector<Point3d> > objectPoints(n_images);

    const std::string folder =combine(datasets_repository_path, "calib-3_stereo_from_JY");
    FileStorage fs_left(combine(folder, "left.xml"), FileStorage::READ);
    CC_Assert(fs_left.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_left[format("image_%d", i )] >> imagePoints[i];
    fs_left.release();

    FileStorage fs_object(combine(folder, "object.xml"), FileStorage::READ);
    CC_Assert(fs_object.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_object[format("image_%d", i )] >> objectPoints[i];
    fs_object.release();

    internal::IntrinsicParams param;
    param.Init(Vec2d(max(imageSize.width, imageSize.height) / CC_PI, max(imageSize.width, imageSize.height) / CC_PI),
               Vec2d(imageSize.width  / 2.0 - 0.5, imageSize.height / 2.0 - 0.5));

    CvMat _imagePoints (imagePoints[0]);
    CvMat _objectPoints(objectPoints[0]);

    CvMat imagePointsNormalized = NormalizePixels(_imagePoints, param).reshape(1).t();
    _objectPoints = _objectPoints.reshape(1).t();
    CvMat objectPointsMean, covObjectPoints;

    int Np = imagePointsNormalized.cols;
    calcCovarMatrix(_objectPoints, covObjectPoints, objectPointsMean, COVAR_NORMAL | COVAR_COLS);
    SVD svd(covObjectPoints);
    CvMat theR(svd.vt);

    if (norm(theR(Rect(2, 0, 1, 2))) < 1e-6)
        theR = CvMat::eye(3,3, CC_64FC1);
    if (determinant(theR) < 0)
        theR = -theR;

    CvMat theT = -theR * objectPointsMean;
    CvMat X_new = theR * _objectPoints + theT * CvMat::ones(1, Np, CC_64FC1);
    CvMat H = internal::ComputeHomography(imagePointsNormalized, X_new.rowRange(0, 2));

    CvMat M = CvMat::ones(3, X_new.cols, CC_64FC1);
    X_new.rowRange(0, 2).copyTo(M.rowRange(0, 2));
    CvMat mrep = H * M;

    divide(mrep, CvMat::ones(3,1, CC_64FC1) * mrep.row(2).clone(), mrep);

    CvMat merr = (mrep.rowRange(0, 2) - imagePointsNormalized).t();

    Vec2d std_err;
    meanStdDev(merr.reshape(2), noArray(), std_err);
    std_err *= sqrt((double)merr.reshape(2).total() / (merr.reshape(2).total() - 1));

    Vec2d correct_std_err(0.00516740156010384, 0.00644205331553901);
    EXPECT_MAT_NEAR(std_err, correct_std_err, 1e-12);
}

TEST_F(fisheyeTest, EtimateUncertainties)
{
    const int n_images = 34;

    std::vector<std::vector<Point2d> > imagePoints(n_images);
    std::vector<std::vector<Point3d> > objectPoints(n_images);

    const std::string folder =combine(datasets_repository_path, "calib-3_stereo_from_JY");
    FileStorage fs_left(combine(folder, "left.xml"), FileStorage::READ);
    CC_Assert(fs_left.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_left[format("image_%d", i )] >> imagePoints[i];
    fs_left.release();

    FileStorage fs_object(combine(folder, "object.xml"), FileStorage::READ);
    CC_Assert(fs_object.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_object[format("image_%d", i )] >> objectPoints[i];
    fs_object.release();

    int flag = 0;
    flag |= fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flag |= fisheye::CALIB_CHECK_COND;
    flag |= fisheye::CALIB_FIX_SKEW;

    Matx33d theK;
    Vec4d theD;
    std::vector<Vec3d> rvec;
    std::vector<Vec3d> tvec;

    fisheye::calibrate(objectPoints, imagePoints, imageSize, theK, theD,
                           rvec, tvec, flag, TermCriteria(3, 20, 1e-6));

    internal::IntrinsicParams param, errors;
    Vec2d err_std;
    double thresh_cond = 1e6;
    int check_cond = 1;
    param.Init(Vec2d(theK(0,0), theK(1,1)), Vec2d(theK(0,2), theK(1, 2)), theD);
    param.isEstimate = std::vector<uchar>(9, 1);
    param.isEstimate[4] = 0;

    errors.isEstimate = param.isEstimate;

    double rms;

    internal::EstimateUncertainties(objectPoints, imagePoints, param,  rvec, tvec,
                                        errors, err_std, thresh_cond, check_cond, rms);

    EXPECT_MAT_NEAR(errors.f, Vec2d(1.29837104202046,  1.31565641071524), 1e-10);
    EXPECT_MAT_NEAR(errors.c, Vec2d(0.890439368129246, 0.816096854937896), 1e-10);
    EXPECT_MAT_NEAR(errors.k, Vec4d(0.00516248605191506, 0.0168181467500934, 0.0213118690274604, 0.00916010877545648), 1e-10);
    EXPECT_MAT_NEAR(err_std, Vec2d(0.187475975266883, 0.185678953263995), 1e-10);
    CC_Assert(fabs(rms - 0.263782587133546) < 1e-10);
    CC_Assert(errors.alpha == 0);
}

#ifdef HAVE_TEGRA_OPTIMIZATION
// not passing accuracy constrains
TEST_F(fisheyeTest, DISABLED_rectify)
#else
TEST_F(fisheyeTest, rectify)
#endif
{
    const std::string folder =combine(datasets_repository_path, "calib-3_stereo_from_JY");

    CvSize calibration_size = this->imageSize, requested_size = calibration_size;
    Matx33d K1 = this->K, K2 = K1;
    CvMat D1 = CvMat(this->D), D2 = D1;

    Vec3d theT = this->T;
    Matx33d theR = this->R;

    double balance = 0.0, fov_scale = 1.1;
    CvMat R1, R2, P1, P2, Q;
    fisheye::stereoRectify(K1, D1, K2, D2, calibration_size, theR, theT, R1, R2, P1, P2, Q,
                      CALIB_ZERO_DISPARITY, requested_size, balance, fov_scale);

    CvMat lmapx, lmapy, rmapx, rmapy;
    //rewrite for fisheye
    fisheye::initUndistortRectifyMap(K1, D1, R1, P1, requested_size, CC_32F, lmapx, lmapy);
    fisheye::initUndistortRectifyMap(K2, D2, R2, P2, requested_size, CC_32F, rmapx, rmapy);

    CvMat l, r, lundist, rundist;
    VideoCapture lcap(combine(folder, "left/stereo_pair_%03d.jpg")),
                     rcap(combine(folder, "right/stereo_pair_%03d.jpg"));

    for(int i = 0;; ++i)
    {
        lcap >> l; rcap >> r;
        if (l.empty() || r.empty())
            break;

        int ndisp = 128;
        rectangle(l, Rect(255,       0, 829,       l.rows-1), Scalar(0, 0, 255));
        rectangle(r, Rect(255,       0, 829,       l.rows-1), Scalar(0, 0, 255));
        rectangle(r, Rect(255-ndisp, 0, 829+ndisp ,l.rows-1), Scalar(0, 0, 255));
        remap(l, lundist, lmapx, lmapy, INTER_LINEAR);
        remap(r, rundist, rmapx, rmapy, INTER_LINEAR);

        CvMat rectification = mergeRectification(lundist, rundist);

        CvMat correct = imread(combine(datasets_repository_path, format("rectification_AB_%03d.png", i)));

        if (correct.empty())
            imwrite(combine(datasets_repository_path, format("rectification_AB_%03d.png", i)), rectification);
         else
             EXPECT_MAT_NEAR(correct, rectification, 1e-10);
     }
}

TEST_F(fisheyeTest, stereoCalibrate)
{
    const int n_images = 34;

    const std::string folder =combine(datasets_repository_path, "calib-3_stereo_from_JY");

    std::vector<std::vector<Point2d> > leftPoints(n_images);
    std::vector<std::vector<Point2d> > rightPoints(n_images);
    std::vector<std::vector<Point3d> > objectPoints(n_images);

    FileStorage fs_left(combine(folder, "left.xml"), FileStorage::READ);
    CC_Assert(fs_left.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_left[format("image_%d", i )] >> leftPoints[i];
    fs_left.release();

    FileStorage fs_right(combine(folder, "right.xml"), FileStorage::READ);
    CC_Assert(fs_right.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_right[format("image_%d", i )] >> rightPoints[i];
    fs_right.release();

    FileStorage fs_object(combine(folder, "object.xml"), FileStorage::READ);
    CC_Assert(fs_object.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_object[format("image_%d", i )] >> objectPoints[i];
    fs_object.release();

    Matx33d K1, K2, theR;
    Vec3d theT;
    Vec4d D1, D2;

    int flag = 0;
    flag |= fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flag |= fisheye::CALIB_CHECK_COND;
    flag |= fisheye::CALIB_FIX_SKEW;
   // flag |= fisheye::CALIB_FIX_INTRINSIC;

    fisheye::stereoCalibrate(objectPoints, leftPoints, rightPoints,
                    K1, D1, K2, D2, imageSize, theR, theT, flag,
                    TermCriteria(3, 12, 0));

    Matx33d R_correct(   0.9975587205950972,   0.06953016383322372, 0.006492709911733523,
                           -0.06956823121068059,    0.9975601387249519, 0.005833595226966235,
                          -0.006071257768382089, -0.006271040135405457, 0.9999619062167968);
    Vec3d T_correct(-0.099402724724121, 0.00270812139265413, 0.00129330292472699);
    Matx33d K1_correct (561.195925927249,                0, 621.282400272412,
                                   0, 562.849402029712, 380.555455380889,
                                   0,                0,                1);

    Matx33d K2_correct (560.395452535348,                0, 678.971652040359,
                                   0,  561.90171021422, 380.401340535339,
                                   0,                0,                1);

    Vec4d D1_correct (-7.44253716539556e-05, -0.00702662033932424, 0.00737569823650885, -0.00342230256441771);
    Vec4d D2_correct (-0.0130785435677431, 0.0284434505383497, -0.0360333869900506, 0.0144724062347222);

    EXPECT_MAT_NEAR(theR, R_correct, 1e-10);
    EXPECT_MAT_NEAR(theT, T_correct, 1e-10);

    EXPECT_MAT_NEAR(K1, K1_correct, 1e-10);
    EXPECT_MAT_NEAR(K2, K2_correct, 1e-10);

    EXPECT_MAT_NEAR(D1, D1_correct, 1e-10);
    EXPECT_MAT_NEAR(D2, D2_correct, 1e-10);

}

TEST_F(fisheyeTest, stereoCalibrateFixIntrinsic)
{
    const int n_images = 34;

    const std::string folder =combine(datasets_repository_path, "calib-3_stereo_from_JY");

    std::vector<std::vector<Point2d> > leftPoints(n_images);
    std::vector<std::vector<Point2d> > rightPoints(n_images);
    std::vector<std::vector<Point3d> > objectPoints(n_images);

    FileStorage fs_left(combine(folder, "left.xml"), FileStorage::READ);
    CC_Assert(fs_left.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_left[format("image_%d", i )] >> leftPoints[i];
    fs_left.release();

    FileStorage fs_right(combine(folder, "right.xml"), FileStorage::READ);
    CC_Assert(fs_right.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_right[format("image_%d", i )] >> rightPoints[i];
    fs_right.release();

    FileStorage fs_object(combine(folder, "object.xml"), FileStorage::READ);
    CC_Assert(fs_object.isOpened());
    for(int i = 0; i < n_images; ++i)
    fs_object[format("image_%d", i )] >> objectPoints[i];
    fs_object.release();

    Matx33d theR;
    Vec3d theT;

    int flag = 0;
    flag |= fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flag |= fisheye::CALIB_CHECK_COND;
    flag |= fisheye::CALIB_FIX_SKEW;
    flag |= fisheye::CALIB_FIX_INTRINSIC;

    Matx33d K1 (561.195925927249,                0, 621.282400272412,
                                   0, 562.849402029712, 380.555455380889,
                                   0,                0,                1);

    Matx33d K2 (560.395452535348,                0, 678.971652040359,
                                   0,  561.90171021422, 380.401340535339,
                                   0,                0,                1);

    Vec4d D1 (-7.44253716539556e-05, -0.00702662033932424, 0.00737569823650885, -0.00342230256441771);
    Vec4d D2 (-0.0130785435677431, 0.0284434505383497, -0.0360333869900506, 0.0144724062347222);

    fisheye::stereoCalibrate(objectPoints, leftPoints, rightPoints,
                    K1, D1, K2, D2, imageSize, theR, theT, flag,
                    TermCriteria(3, 12, 0));

    Matx33d R_correct(   0.9975587205950972,   0.06953016383322372, 0.006492709911733523,
                           -0.06956823121068059,    0.9975601387249519, 0.005833595226966235,
                          -0.006071257768382089, -0.006271040135405457, 0.9999619062167968);
    Vec3d T_correct(-0.099402724724121, 0.00270812139265413, 0.00129330292472699);


    EXPECT_MAT_NEAR(theR, R_correct, 1e-10);
    EXPECT_MAT_NEAR(theT, T_correct, 1e-10);
}

TEST_F(fisheyeTest, CalibrationWithDifferentPointsNumber)
{
  const int n_images = 2;

  std::vector<std::vector<Point2d> > imagePoints(n_images);
  std::vector<std::vector<Point3d> > objectPoints(n_images);

  std::vector<Point2d> imgPoints1(10);
  std::vector<Point2d> imgPoints2(15);

  std::vector<Point3d> objectPoints1(imgPoints1.size());
  std::vector<Point3d> objectPoints2(imgPoints2.size());

  for (size_t i = 0; i < imgPoints1.size(); i++)
  {
    imgPoints1[i] = Point2d((double)i, (double)i);
    objectPoints1[i] = Point3d((double)i, (double)i, 10.0);
  }

  for (size_t i = 0; i < imgPoints2.size(); i++)
  {
    imgPoints2[i] = Point2d(i + 0.5, i + 0.5);
    objectPoints2[i] = Point3d(i + 0.5, i + 0.5, 10.0);
  }

  imagePoints[0] = imgPoints1;
  imagePoints[1] = imgPoints2;
  objectPoints[0] = objectPoints1;
  objectPoints[1] = objectPoints2;

  Matx33d theK = Matx33d::eye();
  Vec4d theD;

  int flag = 0;
  flag |= fisheye::CALIB_RECOMPUTE_EXTRINSIC;
  flag |= fisheye::CALIB_USE_INTRINSIC_GUESS;
  flag |= fisheye::CALIB_FIX_SKEW;

  fisheye::calibrate(objectPoints, imagePoints, CvSize(100, 100), theK, theD,
    noArray(), noArray(), flag, TermCriteria(3, 20, 1e-6));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  fisheyeTest::

const CvSize fisheyeTest::imageSize(1280, 800);

const Matx33d fisheyeTest::K(558.478087865323,               0, 620.458515360843,
                              0, 560.506767351568, 381.939424848348,
                              0,               0,                1);

const Vec4d fisheyeTest::D(-0.0014613319981768, -0.00329861110580401, 0.00605760088590183, -0.00374209380722371);

const Matx33d fisheyeTest::R ( 9.9756700084424932e-01, 6.9698277640183867e-02, 1.4929569991321144e-03,
                            -6.9711825162322980e-02, 9.9748249845531767e-01, 1.2997180766418455e-02,
                            -5.8331736398316541e-04,-1.3069635393884985e-02, 9.9991441852366736e-01);

const Vec3d fisheyeTest::T(-9.9217369356044638e-02, 3.1741831972356663e-03, 1.8551007952921010e-04);

std::string fisheyeTest::combine(const std::string& _item1, const std::string& _item2)
{
    std::string item1 = _item1, item2 = _item2;
    std::replace(item1.begin(), item1.end(), '\\', '/');
    std::replace(item2.begin(), item2.end(), '\\', '/');

    if (item1.empty())
        return item2;

    if (item2.empty())
        return item1;

    char last = item1[item1.size()-1];
    return item1 + (last != '/' ? "/" : "") + item2;
}

CvMat fisheyeTest::mergeRectification(const CvMat& l, const CvMat& r)
{
    CC_Assert(l->tid == r->tid && l.size() == r.size());
    CvMat merged(l.rows, l.cols * 2, l->tid);
    CvMat lpart = merged.colRange(0, l.cols);
    CvMat rpart = merged.colRange(l.cols, merged.cols);
    l.copyTo(lpart);
    r.copyTo(rpart);

    for(int i = 0; i < l.rows; i+=20)
        line(merged, Point(0, i), Point(merged.cols, i), Scalar(0, 255, 0));

    return merged;
}
