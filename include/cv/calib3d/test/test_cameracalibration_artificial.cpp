

#include "test_precomp.hpp"

#include <string>
#include <limits>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "test_chessboardgenerator.hpp"

using namespace cv;
using namespace std;

//template<class T> ostream& operator<<(ostream& out, const Mat_<T>& mat)
//{
//    for(Mat_<T>::const_iterator pos = mat.begin(), end = mat.end(); pos != end; ++pos)
//        out << *pos << " ";
//    return out;
//}
//ostream& operator<<(ostream& out, const CvMat& mat) { return out << Mat_<double>(mat); }

CvMat calcRvec(const vector<Point3f>& points, const CvSize& cornerSize)
{
    Point3f p00 = points[0];
    Point3f p10 = points[1];
    Point3f p01 = points[cornerSize.width];

    Vec3d ex(p10.x - p00.x, p10.y - p00.y, p10.z - p00.z);
    Vec3d ey(p01.x - p00.x, p01.y - p00.y, p01.z - p00.z);
    Vec3d ez = ex.cross(ey);

    CvMat rot(3, 3, CC_64F);
    *rot.ptr<Vec3d>(0) = ex;
    *rot.ptr<Vec3d>(1) = ey;
    *rot.ptr<Vec3d>(2) = ez * (1.0/norm(ez));

    CvMat res;
    Rodrigues(rot.t(), res);
    return res.reshape(1, 1);
}

class CC_CalibrateCameraArtificialTest : public cvtest::BaseTest
{
public:
    CC_CalibrateCameraArtificialTest() :
        r(0)
    {
    }
    ~CC_CalibrateCameraArtificialTest() {}
protected:
    int r;

    const static int JUST_FIND_CORNERS = 0;
    const static int USE_CORNERS_SUBPIX = 1;
    const static int USE_4QUAD_CORNERS = 2;
    const static int ARTIFICIAL_CORNERS = 4;


    bool checkErr(double a, double a0, double eps, double delta)
    {
        return fabs(a - a0) > eps * (fabs(a0) + delta);
    }

    void compareCameraMatrs(const Mat_<double>& camMat, const CvMat& camMat_est)
    {
        if ( camMat_est.at<double>(0, 1) != 0 || camMat_est.at<double>(1, 0) != 0 ||
            camMat_est.at<double>(2, 0) != 0 || camMat_est.at<double>(2, 1) != 0 ||
            camMat_est.at<double>(2, 2) != 1)
        {
            ts->printf( cvtest::TS::LOG, "Bad shape of camera matrix returned \n");
            ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        }

        double fx_e = camMat_est.at<double>(0, 0), fy_e = camMat_est.at<double>(1, 1);
        double cx_e = camMat_est.at<double>(0, 2), cy_e = camMat_est.at<double>(1, 2);

        double fx = camMat(0, 0), fy = camMat(1, 1), cx = camMat(0, 2), cy = camMat(1, 2);

        const double eps = 1e-2;
        const double dlt = 1e-5;

        bool fail = checkErr(fx_e, fx, eps, dlt) || checkErr(fy_e, fy, eps, dlt) ||
            checkErr(cx_e, cx, eps, dlt) || checkErr(cy_e, cy, eps, dlt);

        if (fail)
        {
            ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
        }
        ts->printf( cvtest::TS::LOG, "%d) Expected  [Fx Fy Cx Cy] = [%.3f %.3f %.3f %.3f]\n", r, fx, fy, cx, cy);
        ts->printf( cvtest::TS::LOG, "%d) Estimated [Fx Fy Cx Cy] = [%.3f %.3f %.3f %.3f]\n", r, fx_e, fy_e, cx_e, cy_e);
    }

    void compareDistCoeffs(const Mat_<double>& distCoeffs, const CvMat& distCoeffs_est)
    {
        const double *dt_e = distCoeffs_est.ptr<double>();

        double k1_e = dt_e[0], k2_e = dt_e[1], k3_e = dt_e[4];
        double p1_e = dt_e[2], p2_e = dt_e[3];

        double k1 = distCoeffs(0, 0), k2 = distCoeffs(0, 1), k3 = distCoeffs(0, 4);
        double p1 = distCoeffs(0, 2), p2 = distCoeffs(0, 3);

        const double eps = 5e-2;
        const double dlt = 1e-3;

        const double eps_k3 = 5;
        const double dlt_k3 = 1e-3;

        bool fail = checkErr(k1_e, k1, eps, dlt) || checkErr(k2_e, k2, eps, dlt) || checkErr(k3_e, k3, eps_k3, dlt_k3) ||
            checkErr(p1_e, p1, eps, dlt) || checkErr(p2_e, p2, eps, dlt);

        if (fail)
        {
            // commented according to vp123's recomendation. TODO - improve accuaracy
            //ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY); ss
        }
        ts->printf( cvtest::TS::LOG, "%d) DistCoeff exp=(%.2f, %.2f, %.4f, %.4f %.2f)\n", r, k1, k2, p1, p2, k3);
        ts->printf( cvtest::TS::LOG, "%d) DistCoeff est=(%.2f, %.2f, %.4f, %.4f %.2f)\n", r, k1_e, k2_e, p1_e, p2_e, k3_e);
        ts->printf( cvtest::TS::LOG, "%d) AbsError = [%.5f %.5f %.5f %.5f %.5f]\n", r, fabs(k1-k1_e), fabs(k2-k2_e), fabs(p1-p1_e), fabs(p2-p2_e), fabs(k3-k3_e));
    }

    void compareShiftVecs(const vector<CvMat>& tvecs, const vector<CvMat>& tvecs_est)
    {
        const double eps = 1e-2;
        const double dlt = 1e-4;

        int err_count = 0;
        const int errMsgNum = 4;
        for(size_t i = 0; i < tvecs.size(); ++i)
        {
            const Point3d& tvec = *tvecs[i].ptr<Point3d>();
            const Point3d& tvec_est = *tvecs_est[i].ptr<Point3d>();

            if (norm(tvec_est - tvec) > eps* (norm(tvec) + dlt))
            {
                if (err_count++ < errMsgNum)
                {
                    if (err_count == errMsgNum)
                        ts->printf( cvtest::TS::LOG, "%d) ...\n", r);
                    else
                    {
                        ts->printf( cvtest::TS::LOG, "%d) Bad accuracy in returned tvecs. Index = %d\n", r, i);
                        ts->printf( cvtest::TS::LOG, "%d) norm(tvec_est - tvec) = %f, norm(tvec_exp) = %f \n", r, norm(tvec_est - tvec), norm(tvec));
                    }
                }
                ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
            }
        }
    }

    void compareRotationVecs(const vector<CvMat>& rvecs, const vector<CvMat>& rvecs_est)
    {
        const double eps = 2e-2;
        const double dlt = 1e-4;

        CvMat rmat, rmat_est;
        int err_count = 0;
        const int errMsgNum = 4;
        for(size_t i = 0; i < rvecs.size(); ++i)
        {
            Rodrigues(rvecs[i], rmat);
            Rodrigues(rvecs_est[i], rmat_est);

            if (cvtest::norm(rmat_est, rmat, NORM_L2) > eps* (cvtest::norm(rmat, NORM_L2) + dlt))
            {
                if (err_count++ < errMsgNum)
                {
                    if (err_count == errMsgNum)
                        ts->printf( cvtest::TS::LOG, "%d) ...\n", r);
                    else
                    {
                        ts->printf( cvtest::TS::LOG, "%d) Bad accuracy in returned rvecs (rotation matrs). Index = %d\n", r, i);
                        ts->printf( cvtest::TS::LOG, "%d) norm(rot_mat_est - rot_mat_exp) = %f, norm(rot_mat_exp) = %f \n", r,
                                   cvtest::norm(rmat_est, rmat, NORM_L2), cvtest::norm(rmat, NORM_L2));

                    }
                }
                ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
            }
        }
    }

    double reprojectErrorWithoutIntrinsics(const vector<Point3f>& cb3d, const vector<CvMat>& _rvecs_exp, const vector<CvMat>& _tvecs_exp,
        const vector<CvMat>& rvecs_est, const vector<CvMat>& tvecs_est)
    {
        const static CvMat eye33 = CvMat::eye(3, 3, CC_64F);
        const static CvMat zero15 = CvMat::zeros(1, 5, CC_64F);
        CvMat _chessboard3D(cb3d);
        vector<Point2f> uv_exp, uv_est;
        double res = 0;

        for(size_t i = 0; i < rvecs_exp.size(); ++i)
        {
            projectPoints(_chessboard3D, _rvecs_exp[i], _tvecs_exp[i], eye33, zero15, uv_exp);
            projectPoints(_chessboard3D, rvecs_est[i], tvecs_est[i], eye33, zero15, uv_est);
            for(size_t j = 0; j < cb3d.size(); ++j)
                res += norm(uv_exp[i] - uv_est[i]);
        }
        return res;
    }

    Size2f sqSile;

    vector<Point3f> chessboard3D;
    vector<CvMat> boards, rvecs_exp, tvecs_exp, rvecs_spnp, tvecs_spnp;
    vector< vector<Point3f> > objectPoints;
    vector< vector<Point2f> > imagePoints_art;
    vector< vector<Point2f> > imagePoints_findCb;


    void prepareForTest(const CvMat& bg, const CvMat& camMat, const CvMat& distCoeffs, size_t brdsNum, const ChessBoardGenerator& cbg)
    {
        sqSile = Size2f(1.f, 1.f);
        CvSize cornersSize = cbg.cornersSize();

        chessboard3D.clear();
        for(int j = 0; j < cornersSize.height; ++j)
            for(int i = 0; i < cornersSize.width; ++i)
                chessboard3D.push_back(Point3f(sqSile.width * i, sqSile.height * j, 0));

        boards.resize(brdsNum);
        rvecs_exp.resize(brdsNum);
        tvecs_exp.resize(brdsNum);
        objectPoints.clear();
        objectPoints.resize(brdsNum, chessboard3D);
        imagePoints_art.clear();
        imagePoints_findCb.clear();

        vector<Point2f> corners_art, corners_fcb;
        for(size_t i = 0; i < brdsNum; ++i)
        {
            for(;;)
            {
                boards[i] = cbg(bg, camMat, distCoeffs, sqSile, corners_art);
                if(findChessboardCorners(boards[i], cornersSize, corners_fcb))
                    break;
            }

            //namedWindow("CB"); imshow("CB", boards[i]); waitKey();

            imagePoints_art.push_back(corners_art);
            imagePoints_findCb.push_back(corners_fcb);

            tvecs_exp[i].create(1, 3, CC_64F);
            *tvecs_exp[i].ptr<Point3d>() = cbg.corners3d[0];
            rvecs_exp[i] = calcRvec(cbg.corners3d, cbg.cornersSize());
        }

    }

    void runTest(const CvSize& imgSize, const Mat_<double>& camMat, const Mat_<double>& distCoeffs, size_t brdsNum, const CvSize& cornersSize, int flag = 0)
    {
        const TermCriteria tc(TermCriteria::EPS|TermCriteria::MAX_ITER, 30, 0.1);

        vector< vector<Point2f> > imagePoints;

        switch(flag)
        {
        case JUST_FIND_CORNERS: imagePoints = imagePoints_findCb; break;
        case ARTIFICIAL_CORNERS: imagePoints = imagePoints_art; break;

        case USE_CORNERS_SUBPIX:
            for(size_t i = 0; i < brdsNum; ++i)
            {
                CvMat gray;
                cvtColor(boards[i], gray, COLOR_BGR2GRAY);
                vector<Point2f> tmp = imagePoints_findCb[i];
                cornerSubPix(gray, tmp, CvSize(5, 5), CvSize(-1,-1), tc);
                imagePoints.push_back(tmp);
            }
            break;
        case USE_4QUAD_CORNERS:
            for(size_t i = 0; i < brdsNum; ++i)
            {
                CvMat gray;
                cvtColor(boards[i], gray, COLOR_BGR2GRAY);
                vector<Point2f> tmp = imagePoints_findCb[i];
                find4QuadCornerSubpix(gray, tmp, CvSize(5, 5));
                imagePoints.push_back(tmp);
            }
            break;
        default:
            throw std::exception();
        }

        CvMat camMat_est = CvMat::eye(3, 3, CC_64F), distCoeffs_est = CvMat::zeros(1, 5, CC_64F);
        vector<CvMat> rvecs_est, tvecs_est;

        int flags = /*CALIB_FIX_K3|*/CALIB_FIX_K4|CALIB_FIX_K5|CALIB_FIX_K6; //CALIB_FIX_K3; //CALIB_FIX_ASPECT_RATIO |  | CALIB_ZERO_TANGENT_DIST;
        TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, DBL_EPSILON);
        double rep_error = calibrateCamera(objectPoints, imagePoints, imgSize, camMat_est, distCoeffs_est, rvecs_est, tvecs_est, flags, criteria);
        rep_error /= brdsNum * cornersSize.area();

        const double thres = 1;
        if (rep_error > thres)
        {
            ts->printf( cvtest::TS::LOG, "%d) Too big reproject error = %f\n", r, rep_error);
            ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
        }

        compareCameraMatrs(camMat, camMat_est);
        compareDistCoeffs(distCoeffs, distCoeffs_est);
        compareShiftVecs(tvecs_exp, tvecs_est);
        compareRotationVecs(rvecs_exp, rvecs_est);

        double rep_errorWOI = reprojectErrorWithoutIntrinsics(chessboard3D, rvecs_exp, tvecs_exp, rvecs_est, tvecs_est);
        rep_errorWOI /= brdsNum * cornersSize.area();

        const double thres2 = 0.01;
        if (rep_errorWOI > thres2)
        {
            ts->printf( cvtest::TS::LOG, "%d) Too big reproject error without intrinsics = %f\n", r, rep_errorWOI);
            ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
        }

        ts->printf( cvtest::TS::LOG, "%d) Testing solvePnP...\n", r);
        rvecs_spnp.resize(brdsNum);
        tvecs_spnp.resize(brdsNum);
        for(size_t i = 0; i < brdsNum; ++i)
            solvePnP(CvMat(objectPoints[i]), CvMat(imagePoints[i]), camMat, distCoeffs, rvecs_spnp[i], tvecs_spnp[i]);

        compareShiftVecs(tvecs_exp, tvecs_spnp);
        compareRotationVecs(rvecs_exp, rvecs_spnp);
    }

    void run(int)
    {

        ts->set_failed_test_info(cvtest::TS::OK);
        RNG& rng = theRNG();

        int progress = 0;
        int repeat_num = 3;
        for(r = 0; r < repeat_num; ++r)
        {
            const int brds_num = 20;

            CvMat bg(CvSize(640, 480), CC_8UC3);
            randu(bg, Scalar::all(32), Scalar::all(255));
            GaussianBlur(bg, bg, CvSize(5, 5), 2);

            double fx = 300 + (20 * (double)rng - 10);
            double fy = 300 + (20 * (double)rng - 10);

            double cx = bg.cols/2 + (40 * (double)rng - 20);
            double cy = bg.rows/2 + (40 * (double)rng - 20);

            Mat_<double> camMat(3, 3);
            camMat << fx, 0., cx, 0, fy, cy, 0., 0., 1.;

            double k1 = 0.5 + (double)rng/5;
            double k2 = (double)rng/5;
            double k3 = (double)rng/5;

            double p1 = 0.001 + (double)rng/10;
            double p2 = 0.001 + (double)rng/10;

            Mat_<double> distCoeffs(1, 5, 0.0);
            distCoeffs << k1, k2, p1, p2, k3;

            ChessBoardGenerator cbg(CvSize(9, 8));
            cbg.min_cos = 0.9;
            cbg.cov = 0.8;

            progress = update_progress(progress, r, repeat_num, 0);
            ts->printf( cvtest::TS::LOG, "\n");
            prepareForTest(bg, camMat, distCoeffs, brds_num, cbg);

            ts->printf( cvtest::TS::LOG, "artificial corners\n");
            runTest(bg.size(), camMat, distCoeffs, brds_num, cbg.cornersSize(), ARTIFICIAL_CORNERS);
            progress = update_progress(progress, r, repeat_num, 0);

            ts->printf( cvtest::TS::LOG, "findChessboard corners\n");
            runTest(bg.size(), camMat, distCoeffs, brds_num, cbg.cornersSize(), JUST_FIND_CORNERS);
            progress = update_progress(progress, r, repeat_num, 0);

            ts->printf( cvtest::TS::LOG, "cornersSubPix corners\n");
            runTest(bg.size(), camMat, distCoeffs, brds_num, cbg.cornersSize(), USE_CORNERS_SUBPIX);
            progress = update_progress(progress, r, repeat_num, 0);

            ts->printf( cvtest::TS::LOG, "4quad corners\n");
            runTest(bg.size(), camMat, distCoeffs, brds_num, cbg.cornersSize(), USE_4QUAD_CORNERS);
            progress = update_progress(progress, r, repeat_num, 0);
        }
    }
};

TEST(Calib3d_CalibrateCamera_CPP, DISABLED_accuracy_on_artificial_data) { CC_CalibrateCameraArtificialTest test; test.safe_run(); }
