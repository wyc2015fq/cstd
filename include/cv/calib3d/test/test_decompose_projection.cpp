

#include "test_precomp.hpp"

class CC_DecomposeProjectionMatrixTest : public cvtest::BaseTest
{
public:
    CC_DecomposeProjectionMatrixTest();
protected:
    void run(int);
};

CC_DecomposeProjectionMatrixTest::CC_DecomposeProjectionMatrixTest()
{
    test_case_count = 30;
}


void CC_DecomposeProjectionMatrixTest::run(int start_from)
{

    ts->set_failed_test_info(cvtest::TS::OK);

    RNG& rng = ts->get_rng();
    int progress = 0;


    for (int iter = start_from; iter < test_case_count; ++iter)
    {
        ts->update_context(this, iter, true);
        progress = update_progress(progress, iter, test_case_count, 0);

        // Create the original (and random) camera matrix, rotation, and translation
        Vec2d f, c;
        rng.fill(f, RNG::UNIFORM, 300, 1000);
        rng.fill(c, RNG::UNIFORM, 150, 600);

        double alpha = 0.01*rng.gaussian(1);

        Matx33d origK(f(0), alpha*f(0), c(0),
                          0,          f(1), c(1),
                          0,             0,   1);


        Vec3d rVec;
        rng.fill(rVec, RNG::UNIFORM, -CC_PI, CC_PI);

        Matx33d origR;
        Rodrigues(rVec, origR);

        Vec3d origT;
        rng.fill(origT, RNG::NORMAL, 0, 1);


        // Compose the projection matrix
        Matx34d P(3,4);
        hconcat(origK*origR, origK*origT, P);


        // Decompose
        Matx33d K, R;
        Vec4d homogCameraCenter;
        decomposeProjectionMatrix(P, K, R, homogCameraCenter);


        // Recover translation from the camera center
        Vec3d cameraCenter(homogCameraCenter(0), homogCameraCenter(1), homogCameraCenter(2));
        cameraCenter /= homogCameraCenter(3);

        Vec3d t = -R*cameraCenter;


        const double thresh = 1e-6;
        if ( norm(origK, K, NORM_INF) > thresh )
        {
            ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
            break;
        }

        if ( norm(origR, R, NORM_INF) > thresh )
        {
            ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
            break;
        }

        if ( norm(origT, t, NORM_INF) > thresh )
        {
            ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);
            break;
        }

    }

}

TEST(Calib3d_DecomposeProjectionMatrix, accuracy)
{
    CC_DecomposeProjectionMatrixTest test;
    test.safe_run();
}
