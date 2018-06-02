#include "perf_precomp.hpp"

#ifdef HAVE_TBB
#include "tbb/task_scheduler_init.h"
#endif

using namespace std;
using namespace cv;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

CC_ENUM(pnpAlgo, SOLVEPNP_ITERATIVE, SOLVEPNP_EPNP, SOLVEPNP_P3P, SOLVEPNP_DLS, SOLVEPNP_UPNP)

typedef std::tr1::tuple<int, pnpAlgo> PointsNum_Algo_t;
typedef perf::TestBaseWithParam<PointsNum_Algo_t> PointsNum_Algo;

typedef perf::TestBaseWithParam<int> PointsNum;

PERF_TEST_P(PointsNum_Algo, solvePnP,
            testing::Combine(
                testing::Values(5, 3*9, 7*13), //TODO: find why results on 4 points are too unstable
                testing::Values((int)SOLVEPNP_ITERATIVE, (int)SOLVEPNP_EPNP, (int)SOLVEPNP_UPNP, (int)SOLVEPNP_DLS)
                )
            )
{
    int pointsNum = get<0>(GetParam());
    pnpAlgo algo = get<1>(GetParam());

    vector<Point2f> points2d(pointsNum);
    vector<Point3f> points3d(pointsNum);
    CvMat rvec = CvMat::zeros(3, 1, CC_32FC1);
    CvMat tvec = CvMat::zeros(3, 1, CC_32FC1);

    CvMat distortion = CvMat::zeros(5, 1, CC_32FC1);
    CvMat intrinsics = CvMat::eye(3, 3, CC_32FC1);
    intrinsics.at<float> (0, 0) = 400.0;
    intrinsics.at<float> (1, 1) = 400.0;
    intrinsics.at<float> (0, 2) = 640 / 2;
    intrinsics.at<float> (1, 2) = 480 / 2;

    warmup(points3d, WARMUP_RNG);
    warmup(rvec, WARMUP_RNG);
    warmup(tvec, WARMUP_RNG);

    projectPoints(points3d, rvec, tvec, intrinsics, distortion, points2d);

    //add noise
    CvMat noise(1, (int)points2d.size(), CC_32FC2);
    randu(noise, 0, 0.01);
    add(points2d, noise, points2d);

    declare.in(points3d, points2d);
    declare.time(100);

    TEST_CYCLE_N(1000)
    {
        solvePnP(points3d, points2d, intrinsics, distortion, rvec, tvec, false, algo);
    }

    SANITY_CHECK(rvec, 1e-6);
    SANITY_CHECK(tvec, 1e-6);
}

PERF_TEST_P(PointsNum_Algo, solvePnPSmallPoints,
            testing::Combine(
                testing::Values(5),
                testing::Values((int)SOLVEPNP_P3P, (int)SOLVEPNP_EPNP, (int)SOLVEPNP_DLS, (int)SOLVEPNP_UPNP)
                )
            )
{
    int pointsNum = get<0>(GetParam());
    pnpAlgo algo = get<1>(GetParam());
    if( algo == SOLVEPNP_P3P )
        pointsNum = 4;

    vector<Point2f> points2d(pointsNum);
    vector<Point3f> points3d(pointsNum);
    CvMat rvec = CvMat::zeros(3, 1, CC_32FC1);
    CvMat tvec = CvMat::zeros(3, 1, CC_32FC1);

    CvMat distortion = CvMat::zeros(5, 1, CC_32FC1);
    CvMat intrinsics = CvMat::eye(3, 3, CC_32FC1);
    intrinsics.at<float> (0, 0) = 400.0f;
    intrinsics.at<float> (1, 1) = 400.0f;
    intrinsics.at<float> (0, 2) = 640 / 2;
    intrinsics.at<float> (1, 2) = 480 / 2;

    warmup(points3d, WARMUP_RNG);
    warmup(rvec, WARMUP_RNG);
    warmup(tvec, WARMUP_RNG);

    // normalize Rodrigues vector
    CvMat rvec_tmp = CvMat::eye(3, 3, CC_32F);
    Rodrigues(rvec, rvec_tmp);
    Rodrigues(rvec_tmp, rvec);

    projectPoints(points3d, rvec, tvec, intrinsics, distortion, points2d);

    //add noise
    CvMat noise(1, (int)points2d.size(), CC_32FC2);
    randu(noise, -0.001, 0.001);
    add(points2d, noise, points2d);

    declare.in(points3d, points2d);
    declare.time(100);

    TEST_CYCLE_N(1000)
    {
        solvePnP(points3d, points2d, intrinsics, distortion, rvec, tvec, false, algo);
    }

    SANITY_CHECK(rvec, 1e-1);
    SANITY_CHECK(tvec, 1e-2);
}

PERF_TEST_P(PointsNum, DISABLED_SolvePnPRansac, testing::Values(5, 3*9, 7*13))
{
    int count = GetParam();

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

    CvMat rvec;
    CvMat tvec;

#ifdef HAVE_TBB
    // limit concurrency to get deterministic result
    tbb::task_scheduler_init one_thread(1);
#endif

    TEST_CYCLE()
    {
        solvePnPRansac(object, image, camera_mat, dist_coef, rvec, tvec);
    }

    SANITY_CHECK(rvec, 1e-6);
    SANITY_CHECK(tvec, 1e-6);
}
