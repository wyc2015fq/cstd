

#include "test_precomp.hpp"
#include "opencv2/imgproc/imgproc_c.h"

using namespace cv;
using namespace std;

class CC_DefaultNewCameraMatrixTest : public cvtest::ArrayTest
{
public:
    CC_DefaultNewCameraMatrixTest();
protected:
    int prepare_test_case (int test_case_idx);
    void prepare_to_validation( int test_case_idx );
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    void run_func();

private:
    CvSize img_size;
    CvMat camera_mat;
    CvMat new_camera_mat;

    int matrix_type;

    bool center_principal_point;

    static const int MAX_X = 2048;
    static const int MAX_Y = 2048;
    static const int MAX_VAL = 10000;
};

CC_DefaultNewCameraMatrixTest::CC_DefaultNewCameraMatrixTest()
{
    test_array[INPUT].push_back(NULL);
    test_array[OUTPUT].push_back(NULL);
    test_array[REF_OUTPUT].push_back(NULL);

    matrix_type = 0;
    center_principal_point = false;
}

void CC_DefaultNewCameraMatrixTest::get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    cvtest::ArrayTest::get_test_array_types_and_sizes(test_case_idx,sizes,types);
    RNG& rng = ts->get_rng();
    matrix_type = types[INPUT][0] = types[OUTPUT][0]= types[REF_OUTPUT][0] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;
    sizes[INPUT][0] = sizes[OUTPUT][0] = sizes[REF_OUTPUT][0] = cvSize(3,3);
}

int CC_DefaultNewCameraMatrixTest::prepare_test_case(int test_case_idx)
{
    int code = cvtest::ArrayTest::prepare_test_case( test_case_idx );

    if (code <= 0)
        return code;

    RNG& rng = ts->get_rng();

    img_size.width = cvtest::randInt(rng) % MAX_X + 1;
    img_size.height = cvtest::randInt(rng) % MAX_Y + 1;

    center_principal_point = ((cvtest::randInt(rng) % 2)!=0);

    // Generating camera_mat matrix
    double sz = MAX(img_size.width, img_size.height);
    double aspect_ratio = cvtest::randReal(rng)*0.6 + 0.7;
    double a[9] = {0,0,0,0,0,0,0,0,1};
    CvMat _a(3,3,CC_64F,a);
    a[2] = (img_size.width - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
    a[5] = (img_size.height - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
    a[0] = sz/(0.9 - cvtest::randReal(rng)*0.6);
    a[4] = aspect_ratio*a[0];

    CvMat& _a0 = test_mat[INPUT][0];
    cvtest::convert(_a, _a0, _a0->tid);
    camera_mat = _a0;

    return code;

}

void CC_DefaultNewCameraMatrixTest::run_func()
{
    new_camera_mat = getDefaultNewCameraMatrix(camera_mat,img_size,center_principal_point);
}

void CC_DefaultNewCameraMatrixTest::prepare_to_validation( int /*test_case_idx*/ )
{
    const CvMat& src = test_mat[INPUT][0];
    CvMat& dst = test_mat[REF_OUTPUT][0];
    CvMat& test_output = test_mat[OUTPUT][0];
    CvMat& output = new_camera_mat;
    cvtest::convert( output, test_output, test_output->tid );
    if (!center_principal_point)
    {
        cvtest::copy(src, dst);
    }
    else
    {
        double a[9] = {0,0,0,0,0,0,0,0,1};
        CvMat _a(3,3,CC_64F,a);
        if (matrix_type == CC_64F)
        {
            a[0] = src.at<double>(0,0);
            a[4] = src.at<double>(1,1);
        }
        else
        {
            a[0] = src.at<float>(0,0);
            a[4] = src.at<float>(1,1);
        }
        a[2] = (img_size.width - 1)*0.5;
        a[5] = (img_size.height - 1)*0.5;
        cvtest::convert( _a, dst, dst->tid );
    }
}

//---------

class CC_UndistortPointsTest : public cvtest::ArrayTest
{
public:
    CC_UndistortPointsTest();
protected:
    int prepare_test_case (int test_case_idx);
    void prepare_to_validation( int test_case_idx );
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    double get_success_error_level( int test_case_idx, int i, int j );
    void run_func();
    void distortPoints(const CvMat* _src, CvMat* _dst, const CvMat* _cameraMatrix,
                       const CvMat* _distCoeffs, const CvMat* matR, const CvMat* matP);

private:
    bool useCPlus;
    bool useDstMat;
    static const int N_POINTS = 10;
    static const int MAX_X = 2048;
    static const int MAX_Y = 2048;

    bool zero_new_cam;
    bool zero_distortion;
    bool zero_R;

    CvSize img_size;
    CvMat dst_points_mat;

    CvMat camera_mat;
    CvMat R;
    CvMat P;
    CvMat distortion_coeffs;
    CvMat src_points;
    std::vector<Point2f> dst_points;
};

CC_UndistortPointsTest::CC_UndistortPointsTest()
{
    test_array[INPUT].push_back(NULL); // points matrix
    test_array[INPUT].push_back(NULL); // camera matrix
    test_array[INPUT].push_back(NULL); // distortion coeffs
    test_array[INPUT].push_back(NULL); // R matrix
    test_array[INPUT].push_back(NULL); // P matrix
    test_array[OUTPUT].push_back(NULL); // distorted dst points
    test_array[TEMP].push_back(NULL); // dst points
    test_array[REF_OUTPUT].push_back(NULL);

    useCPlus = useDstMat = false;
    zero_new_cam = zero_distortion = zero_R = false;
}

void CC_UndistortPointsTest::get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    cvtest::ArrayTest::get_test_array_types_and_sizes(test_case_idx,sizes,types);
    RNG& rng = ts->get_rng();
    useCPlus = ((cvtest::randInt(rng) % 2)!=0);
    //useCPlus = 0;
    if (useCPlus)
    {
        types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = types[TEMP][0]= CC_32FC2;
    }
    else
    {
        types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = types[TEMP][0]= cvtest::randInt(rng)%2 ? CC_64FC2 : CC_32FC2;
    }
    types[INPUT][1] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;
    types[INPUT][2] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;
    types[INPUT][3] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;
    types[INPUT][4] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;

    sizes[INPUT][0] = sizes[OUTPUT][0] = sizes[REF_OUTPUT][0] = sizes[TEMP][0]= cvtest::randInt(rng)%2 ? cvSize(1,N_POINTS) : cvSize(N_POINTS,1);
    sizes[INPUT][1] = sizes[INPUT][3] = cvSize(3,3);
    sizes[INPUT][4] = cvtest::randInt(rng)%2 ? cvSize(3,3) : cvSize(4,3);

    if (cvtest::randInt(rng)%2)
    {
        if (cvtest::randInt(rng)%2)
        {
            sizes[INPUT][2] = cvSize(1,4);
        }
        else
        {
            sizes[INPUT][2] = cvSize(1,5);
        }
    }
    else
    {
        if (cvtest::randInt(rng)%2)
        {
            sizes[INPUT][2] = cvSize(4,1);
        }
        else
        {
            sizes[INPUT][2] = cvSize(5,1);
        }
    }
}

int CC_UndistortPointsTest::prepare_test_case(int test_case_idx)
{
    RNG& rng = ts->get_rng();
    int code = cvtest::ArrayTest::prepare_test_case( test_case_idx );

    if (code <= 0)
        return code;

    useDstMat = (cvtest::randInt(rng) % 2) == 0;

    img_size.width = cvtest::randInt(rng) % MAX_X + 1;
    img_size.height = cvtest::randInt(rng) % MAX_Y + 1;
    int dist_size = test_mat[INPUT][2].cols > test_mat[INPUT][2].rows ? test_mat[INPUT][2].cols : test_mat[INPUT][2].rows;
    double cam[9] = {0,0,0,0,0,0,0,0,1};
    vector<double> dist(dist_size);
    vector<double> proj(test_mat[INPUT][4].cols * test_mat[INPUT][4].rows);
    vector<Point2d> points(N_POINTS);

    CvMat _camera(3,3,CC_64F,cam);
    CvMat _distort(test_mat[INPUT][2].rows,test_mat[INPUT][2].cols,CC_64F,&dist[0]);
    CvMat _proj(test_mat[INPUT][4].size(), CC_64F, &proj[0]);
    CvMat _points(test_mat[INPUT][0].size(), CC_64FC2, &points[0]);

    _proj = Scalar::all(0);

    //Generating points
    for( int i = 0; i < N_POINTS; i++ )
    {
        points[i].x = cvtest::randReal(rng)*img_size.width;
        points[i].y = cvtest::randReal(rng)*img_size.height;
    }

    //Generating camera matrix
    double sz = MAX(img_size.width,img_size.height);
    double aspect_ratio = cvtest::randReal(rng)*0.6 + 0.7;
    cam[2] = (img_size.width - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
    cam[5] = (img_size.height - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
    cam[0] = sz/(0.9 - cvtest::randReal(rng)*0.6);
    cam[4] = aspect_ratio*cam[0];

    //Generating distortion coeffs
    dist[0] = cvtest::randReal(rng)*0.06 - 0.03;
    dist[1] = cvtest::randReal(rng)*0.06 - 0.03;
    if( dist[0]*dist[1] > 0 )
        dist[1] = -dist[1];
    if( cvtest::randInt(rng)%4 != 0 )
    {
        dist[2] = cvtest::randReal(rng)*0.004 - 0.002;
        dist[3] = cvtest::randReal(rng)*0.004 - 0.002;
        if (dist_size > 4)
            dist[4] = cvtest::randReal(rng)*0.004 - 0.002;
    }
    else
    {
        dist[2] = dist[3] = 0;
        if (dist_size > 4)
            dist[4] = 0;
    }

    //Generating P matrix (projection)
    if( test_mat[INPUT][4].cols != 4 )
    {
        proj[8] = 1;
        if (cvtest::randInt(rng)%2 == 0) // use identity new camera matrix
        {
            proj[0] = 1;
            proj[4] = 1;
        }
        else
        {
            proj[0] = cam[0] + (cvtest::randReal(rng) - (double)0.5)*0.2*cam[0]; //10%
            proj[4] = cam[4] + (cvtest::randReal(rng) - (double)0.5)*0.2*cam[4]; //10%
            proj[2] = cam[2] + (cvtest::randReal(rng) - (double)0.5)*0.3*img_size.width; //15%
            proj[5] = cam[5] + (cvtest::randReal(rng) - (double)0.5)*0.3*img_size.height; //15%
        }
    }
    else
    {
        proj[10] = 1;
        proj[0] = cam[0] + (cvtest::randReal(rng) - (double)0.5)*0.2*cam[0]; //10%
        proj[5] = cam[4] + (cvtest::randReal(rng) - (double)0.5)*0.2*cam[4]; //10%
        proj[2] = cam[2] + (cvtest::randReal(rng) - (double)0.5)*0.3*img_size.width; //15%
        proj[6] = cam[5] + (cvtest::randReal(rng) - (double)0.5)*0.3*img_size.height; //15%

        proj[3] = (img_size.height + img_size.width - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
        proj[7] = (img_size.height + img_size.width - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
        proj[11] = (img_size.height + img_size.width - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
    }

    //Generating R matrix
    CvMat _rot(3,3,CC_64F);
    CvMat rotation(1,3,CC_64F);
    rotation.at<double>(0) = CC_PI*(cvtest::randReal(rng) - (double)0.5); // phi
    rotation.at<double>(1) = CC_PI*(cvtest::randReal(rng) - (double)0.5); // ksi
    rotation.at<double>(2) = CC_PI*(cvtest::randReal(rng) - (double)0.5); //khi
    cvtest::Rodrigues(rotation, _rot);

    //copying data
    //src_points = &_points;
    _points.convertTo(test_mat[INPUT][0], test_mat[INPUT][0]->tid);
    _camera.convertTo(test_mat[INPUT][1], test_mat[INPUT][1]->tid);
    _distort.convertTo(test_mat[INPUT][2], test_mat[INPUT][2]->tid);
    _rot.convertTo(test_mat[INPUT][3], test_mat[INPUT][3]->tid);
    _proj.convertTo(test_mat[INPUT][4], test_mat[INPUT][4]->tid);

    zero_distortion = (cvtest::randInt(rng)%2) == 0 ? false : true;
    zero_new_cam = (cvtest::randInt(rng)%2) == 0 ? false : true;
    zero_R = (cvtest::randInt(rng)%2) == 0 ? false : true;

    if (useCPlus)
    {
        _points.convertTo(src_points, CC_32F);

        camera_mat = test_mat[INPUT][1];
        distortion_coeffs = test_mat[INPUT][2];
        R = test_mat[INPUT][3];
        P = test_mat[INPUT][4];
    }

    return code;
}

void CC_UndistortPointsTest::prepare_to_validation(int /*test_case_idx*/)
{
    int dist_size = test_mat[INPUT][2].cols > test_mat[INPUT][2].rows ? test_mat[INPUT][2].cols : test_mat[INPUT][2].rows;
    double cam[9] = {0,0,0,0,0,0,0,0,1};
    double rot[9] = {1,0,0,0,1,0,0,0,1};

    double* dist = new double[dist_size ];
    double* proj = new double[test_mat[INPUT][4].cols * test_mat[INPUT][4].rows];
    double* points = new double[N_POINTS*2];
    double* r_points = new double[N_POINTS*2];
    //Run reference calculations
    CvMat ref_points= cvMat(test_mat[INPUT][0].rows,test_mat[INPUT][0].cols,CC_64FC2,r_points);
    CvMat _camera = cvMat(3,3,CC_64F,cam);
    CvMat _rot = cvMat(3,3,CC_64F,rot);
    CvMat _distort = cvMat(test_mat[INPUT][2].rows,test_mat[INPUT][2].cols,CC_64F,dist);
    CvMat _proj = cvMat(test_mat[INPUT][4].rows,test_mat[INPUT][4].cols,CC_64F,proj);
    CvMat _points= cvMat(test_mat[TEMP][0].rows,test_mat[TEMP][0].cols,CC_64FC2,points);

    CvMat __camera = cvarrToMat(&_camera);
    CvMat __distort = cvarrToMat(&_distort);
    CvMat __rot = cvarrToMat(&_rot);
    CvMat __proj = cvarrToMat(&_proj);
    CvMat __points = cvarrToMat(&_points);
    CvMat _ref_points = cvarrToMat(&ref_points);

    cvtest::convert(test_mat[INPUT][1], __camera, __camera->tid);
    cvtest::convert(test_mat[INPUT][2], __distort, __distort->tid);
    cvtest::convert(test_mat[INPUT][3], __rot, __rot->tid);
    cvtest::convert(test_mat[INPUT][4], __proj, __proj->tid);

    if (useCPlus)
    {
        if (useDstMat)
        {
            CvMat temp = dst_points_mat;
            for (int i=0;i<N_POINTS*2;i++)
            {
                points[i] = temp.data.fl[i];
            }
        }
        else
        {
            for (int i=0;i<N_POINTS;i++)
            {
                points[2*i] = dst_points[i].x;
                points[2*i+1] = dst_points[i].y;
            }
        }
    }
    else
    {
        cvtest::convert(test_mat[TEMP][0],__points, __points->tid);
    }

    CvMat* input2 = zero_distortion ? 0 : &_distort;
    CvMat* input3 = zero_R ? 0 : &_rot;
    CvMat* input4 = zero_new_cam ? 0 : &_proj;
    distortPoints(&_points,&ref_points,&_camera,input2,input3,input4);

    CvMat& dst = test_mat[REF_OUTPUT][0];
    cvtest::convert(_ref_points, dst, dst->tid);

    cvtest::copy(test_mat[INPUT][0], test_mat[OUTPUT][0]);

    delete[] dist;
    delete[] proj;
    delete[] points;
    delete[] r_points;
}

void CC_UndistortPointsTest::run_func()
{

    if (useCPlus)
    {
        CvMat input2,input3,input4;
        input2 = zero_distortion ? CvMat() : CvMat(test_mat[INPUT][2]);
        input3 = zero_R ? CvMat() : CvMat(test_mat[INPUT][3]);
        input4 = zero_new_cam ? CvMat() : CvMat(test_mat[INPUT][4]);

        if (useDstMat)
        {
            //undistortPoints(src_points,dst_points_mat,camera_mat,distortion_coeffs,R,P);
            undistortPoints(src_points,dst_points_mat,camera_mat,input2,input3,input4);
        }
        else
        {
            //undistortPoints(src_points,dst_points,camera_mat,distortion_coeffs,R,P);
            undistortPoints(src_points,dst_points,camera_mat,input2,input3,input4);
        }
    }
    else
    {
        CvMat _input0 = test_mat[INPUT][0], _input1 = test_mat[INPUT][1], _input2, _input3, _input4;
        CvMat _output = test_mat[TEMP][0];
        if(!zero_distortion)
            _input2 = test_mat[INPUT][2];
        if(!zero_R)
            _input3 = test_mat[INPUT][3];
        if(!zero_new_cam)
            _input4 = test_mat[INPUT][4];
        cvUndistortPoints(&_input0, &_output, &_input1,
                          zero_distortion ? 0 : &_input2,
                          zero_R ? 0 : &_input3,
                          zero_new_cam ? 0 : &_input4);
    }
}

void CC_UndistortPointsTest::distortPoints(const CvMat* _src, CvMat* _dst, const CvMat* _cameraMatrix,
                                            const CvMat* _distCoeffs,
                                            const CvMat* matR, const CvMat* matP)
{
    double a[9];

    CvMat* __P;
    if ((!matP)||(matP->cols == 3))
        __P = cvCreateMat(3,3,CC_64F);
    else
        __P = cvCreateMat(3,4,CC_64F);
    if (matP)
    {
        cvtest::convert(cvarrToMat(matP), cvarrToMat(__P), -1);
    }
    else
    {
        cvZero(__P);
        __P->tt.db[0] = 1;
        __P->tt.db[4] = 1;
        __P->tt.db[8] = 1;
    }
    CvMat* __R = cvCreateMat(3,3,CC_64F);
    if (matR)
    {
        cvCopy(matR,__R);
    }
    else
    {
        cvZero(__R);
        __R->tt.db[0] = 1;
        __R->tt.db[4] = 1;
        __R->tt.db[8] = 1;
    }
    for (int i=0;i<N_POINTS;i++)
    {
        int movement = __P->cols > 3 ? 1 : 0;
        double x = (_src->tt.db[2*i]-__P->tt.db[2])/__P->tt.db[0];
        double y = (_src->tt.db[2*i+1]-__P->tt.db[5+movement])/__P->tt.db[4+movement];
        CvMat inverse = cvMat(3,3,CC_64F,a);
        cvInvert(__R,&inverse);
        double w1 = x*inverse.tt.db[6]+y*inverse.tt.db[7]+inverse.tt.db[8];
        double _x = (x*inverse.tt.db[0]+y*inverse.tt.db[1]+inverse.tt.db[2])/w1;
        double _y = (x*inverse.tt.db[3]+y*inverse.tt.db[4]+inverse.tt.db[5])/w1;

        //Distortions

        double __x = _x;
        double __y = _y;
        if (_distCoeffs)
        {
            double r2 = _x*_x+_y*_y;

            __x = _x*(1+_distCoeffs->tt.db[0]*r2+_distCoeffs->tt.db[1]*r2*r2)+
            2*_distCoeffs->tt.db[2]*_x*_y+_distCoeffs->tt.db[3]*(r2+2*_x*_x);
            __y = _y*(1+_distCoeffs->tt.db[0]*r2+_distCoeffs->tt.db[1]*r2*r2)+
            2*_distCoeffs->tt.db[3]*_x*_y+_distCoeffs->tt.db[2]*(r2+2*_y*_y);
            if ((_distCoeffs->cols > 4) || (_distCoeffs->rows > 4))
            {
                __x+=_x*_distCoeffs->tt.db[4]*r2*r2*r2;
                __y+=_y*_distCoeffs->tt.db[4]*r2*r2*r2;
            }
        }


        _dst->tt.db[2*i] = __x*_cameraMatrix->tt.db[0]+_cameraMatrix->tt.db[2];
        _dst->tt.db[2*i+1] = __y*_cameraMatrix->tt.db[4]+_cameraMatrix->tt.db[5];

    }

    cvReleaseMat(&__R);
    cvReleaseMat(&__P);

}


double CC_UndistortPointsTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    return 5e-2;
}

//------------------------------------------------------

class CC_InitUndistortRectifyMapTest : public cvtest::ArrayTest
{
public:
    CC_InitUndistortRectifyMapTest();
protected:
    int prepare_test_case (int test_case_idx);
    void prepare_to_validation( int test_case_idx );
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    double get_success_error_level( int test_case_idx, int i, int j );
    void run_func();

private:
    bool useCPlus;
    static const int N_POINTS = 100;
    static const int MAX_X = 2048;
    static const int MAX_Y = 2048;
    bool zero_new_cam;
    bool zero_distortion;
    bool zero_R;


    CvSize img_size;

    CvMat camera_mat;
    CvMat R;
    CvMat new_camera_mat;
    CvMat distortion_coeffs;
    CvMat mapx;
    CvMat mapy;
    CvMat* _mapx;
    CvMat* _mapy;
    int mat_type;
};

CC_InitUndistortRectifyMapTest::CC_InitUndistortRectifyMapTest()
{
    test_array[INPUT].push_back(NULL); // test points matrix
    test_array[INPUT].push_back(NULL); // camera matrix
    test_array[INPUT].push_back(NULL); // distortion coeffs
    test_array[INPUT].push_back(NULL); // R matrix
    test_array[INPUT].push_back(NULL); // new camera matrix
    test_array[OUTPUT].push_back(NULL); // distorted dst points
    test_array[REF_OUTPUT].push_back(NULL);

    useCPlus = false;
    zero_distortion = zero_new_cam = zero_R = false;
    _mapx = _mapy = NULL;
    mat_type = 0;
}

void CC_InitUndistortRectifyMapTest::get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    cvtest::ArrayTest::get_test_array_types_and_sizes(test_case_idx,sizes,types);
    RNG& rng = ts->get_rng();
    useCPlus = ((cvtest::randInt(rng) % 2)!=0);
    //useCPlus = 0;
    types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_64FC2;

    types[INPUT][1] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;
    types[INPUT][2] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;
    types[INPUT][3] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;
    types[INPUT][4] = cvtest::randInt(rng)%2 ? CC_64F : CC_32F;

    sizes[INPUT][0] = sizes[OUTPUT][0] = sizes[REF_OUTPUT][0] = cvSize(N_POINTS,1);
    sizes[INPUT][1] = sizes[INPUT][3] = cvSize(3,3);
    sizes[INPUT][4] = cvSize(3,3);

    if (cvtest::randInt(rng)%2)
    {
        if (cvtest::randInt(rng)%2)
        {
            sizes[INPUT][2] = cvSize(1,4);
        }
        else
        {
            sizes[INPUT][2] = cvSize(1,5);
        }
    }
    else
    {
        if (cvtest::randInt(rng)%2)
        {
            sizes[INPUT][2] = cvSize(4,1);
        }
        else
        {
            sizes[INPUT][2] = cvSize(5,1);
        }
    }
}


int CC_InitUndistortRectifyMapTest::prepare_test_case(int test_case_idx)
{
    RNG& rng = ts->get_rng();
    int code = cvtest::ArrayTest::prepare_test_case( test_case_idx );

    if (code <= 0)
        return code;

    img_size.width = cvtest::randInt(rng) % MAX_X + 1;
    img_size.height = cvtest::randInt(rng) % MAX_Y + 1;

    if (useCPlus)
    {
        mat_type = (cvtest::randInt(rng) % 2) == 0 ? CC_32FC1 : CC_16SC2;
        if ((cvtest::randInt(rng) % 4) == 0)
            mat_type = -1;
        if ((cvtest::randInt(rng) % 4) == 0)
            mat_type = CC_32FC2;
        _mapx = 0;
        _mapy = 0;
    }
    else
    {
        int typex = (cvtest::randInt(rng) % 2) == 0 ? CC_32FC1 : CC_16SC2;
        //typex = CC_32FC1; ///!!!!!!!!!!!!!!!!
        int typey = (typex == CC_32FC1) ? CC_32FC1 : CC_16UC1;

        _mapx = cvCreateMat(img_size.height,img_size.width,typex);
        _mapy = cvCreateMat(img_size.height,img_size.width,typey);


    }

    int dist_size = test_mat[INPUT][2].cols > test_mat[INPUT][2].rows ? test_mat[INPUT][2].cols : test_mat[INPUT][2].rows;
    double cam[9] = {0,0,0,0,0,0,0,0,1};
    vector<double> dist(dist_size);
    vector<double> new_cam(test_mat[INPUT][4].cols * test_mat[INPUT][4].rows);
    vector<Point2d> points(N_POINTS);

    CvMat _camera(3,3,CC_64F,cam);
    CvMat _distort(test_mat[INPUT][2].size(),CC_64F,&dist[0]);
    CvMat _new_cam(test_mat[INPUT][4].size(),CC_64F,&new_cam[0]);
    CvMat _points(test_mat[INPUT][0].size(),CC_64FC2, &points[0]);

    //Generating points
    for (int i=0;i<N_POINTS;i++)
    {
        points[i].x = cvtest::randReal(rng)*img_size.width;
        points[i].y = cvtest::randReal(rng)*img_size.height;
    }

    //Generating camera matrix
    double sz = MAX(img_size.width,img_size.height);
    double aspect_ratio = cvtest::randReal(rng)*0.6 + 0.7;
    cam[2] = (img_size.width - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
    cam[5] = (img_size.height - 1)*0.5 + cvtest::randReal(rng)*10 - 5;
    cam[0] = sz/(0.9 - cvtest::randReal(rng)*0.6);
    cam[4] = aspect_ratio*cam[0];

    //Generating distortion coeffs
    dist[0] = cvtest::randReal(rng)*0.06 - 0.03;
    dist[1] = cvtest::randReal(rng)*0.06 - 0.03;
    if( dist[0]*dist[1] > 0 )
        dist[1] = -dist[1];
    if( cvtest::randInt(rng)%4 != 0 )
    {
        dist[2] = cvtest::randReal(rng)*0.004 - 0.002;
        dist[3] = cvtest::randReal(rng)*0.004 - 0.002;
        if (dist_size > 4)
            dist[4] = cvtest::randReal(rng)*0.004 - 0.002;
    }
    else
    {
        dist[2] = dist[3] = 0;
        if (dist_size > 4)
            dist[4] = 0;
    }

    //Generating new camera matrix
    _new_cam = Scalar::all(0);
    new_cam[8] = 1;

    //new_cam[0] = cam[0];
    //new_cam[4] = cam[4];
    //new_cam[2] = cam[2];
    //new_cam[5] = cam[5];

    new_cam[0] = cam[0] + (cvtest::randReal(rng) - (double)0.5)*0.2*cam[0]; //10%
    new_cam[4] = cam[4] + (cvtest::randReal(rng) - (double)0.5)*0.2*cam[4]; //10%
    new_cam[2] = cam[2] + (cvtest::randReal(rng) - (double)0.5)*0.3*img_size.width; //15%
    new_cam[5] = cam[5] + (cvtest::randReal(rng) - (double)0.5)*0.3*img_size.height; //15%


    //Generating R matrix
    CvMat _rot(3,3,CC_64F);
    CvMat rotation(1,3,CC_64F);
    rotation.at<double>(0) = CC_PI/8*(cvtest::randReal(rng) - (double)0.5); // phi
    rotation.at<double>(1) = CC_PI/8*(cvtest::randReal(rng) - (double)0.5); // ksi
    rotation.at<double>(2) = CC_PI/3*(cvtest::randReal(rng) - (double)0.5); //khi
    cvtest::Rodrigues(rotation, _rot);

    //cvSetIdentity(_rot);
    //copying data
    cvtest::convert( _points, test_mat[INPUT][0], test_mat[INPUT][0]->tid);
    cvtest::convert( _camera, test_mat[INPUT][1], test_mat[INPUT][1]->tid);
    cvtest::convert( _distort, test_mat[INPUT][2], test_mat[INPUT][2]->tid);
    cvtest::convert( _rot, test_mat[INPUT][3], test_mat[INPUT][3]->tid);
    cvtest::convert( _new_cam, test_mat[INPUT][4], test_mat[INPUT][4]->tid);

    zero_distortion = (cvtest::randInt(rng)%2) == 0 ? false : true;
    zero_new_cam = (cvtest::randInt(rng)%2) == 0 ? false : true;
    zero_R = (cvtest::randInt(rng)%2) == 0 ? false : true;

    if (useCPlus)
    {
        camera_mat = test_mat[INPUT][1];
        distortion_coeffs = test_mat[INPUT][2];
        R = test_mat[INPUT][3];
        new_camera_mat = test_mat[INPUT][4];
    }

    return code;
}

void CC_InitUndistortRectifyMapTest::prepare_to_validation(int/* test_case_idx*/)
{
#if 0
    int dist_size = test_mat[INPUT][2].cols > test_mat[INPUT][2].rows ? test_mat[INPUT][2].cols : test_mat[INPUT][2].rows;
    double cam[9] = {0,0,0,0,0,0,0,0,1};
    double rot[9] = {1,0,0,0,1,0,0,0,1};
    vector<double> dist(dist_size);
    vector<double> new_cam(test_mat[INPUT][4].cols * test_mat[INPUT][4].rows);
    vector<Point2d> points(N_POINTS);
    vector<Point2d> r_points(N_POINTS);
    //Run reference calculations
    CvMat ref_points(test_mat[INPUT][0].size(),CC_64FC2,&r_points[0]);
    CvMat _camera(3,3,CC_64F,cam);
    CvMat _rot(3,3,CC_64F,rot);
    CvMat _distort(test_mat[INPUT][2].size(),CC_64F,&dist[0]);
    CvMat _new_cam(test_mat[INPUT][4].size(),CC_64F,&new_cam[0]);
    CvMat _points(test_mat[INPUT][0].size(),CC_64FC2,&points[0]);

    cvtest::convert(test_mat[INPUT][1],_camera,_camera->tid);
    cvtest::convert(test_mat[INPUT][2],_distort,_distort->tid);
    cvtest::convert(test_mat[INPUT][3],_rot,_rot->tid);
    cvtest::convert(test_mat[INPUT][4],_new_cam,_new_cam->tid);

    //Applying precalculated undistort rectify map
    if (!useCPlus)
    {
        mapx = CvMat(_mapx);
        mapy = CvMat(_mapy);
    }
    CvMat map1,map2;
    convertMaps(mapx,mapy,map1,map2,CC_32FC1);
    CvMat _map1 = map1;
    CvMat _map2 = map2;
    const Point2d* sptr = (const Point2d*)test_mat[INPUT][0].data;
    for( int i = 0;i < N_POINTS; i++ )
    {
        int u = saturate_cast<int>(sptr[i].x);
        int v = saturate_cast<int>(sptr[i].y);
        points[i].x = _map1.data.fl[v*_map1.cols + u];
        points[i].y = _map2.data.fl[v*_map2.cols + u];
    }

    //---

    undistortPoints(_points, ref_points, _camera,
                        zero_distortion ? CvMat() : _distort,
                        zero_R ? CvMat::eye(3,3,CC_64F) : _rot,
                        zero_new_cam ? _camera : _new_cam);
    //cvTsDistortPoints(&_points,&ref_points,&_camera,&_distort,&_rot,&_new_cam);
    cvtest::convert(ref_points, test_mat[REF_OUTPUT][0], test_mat[REF_OUTPUT][0]->tid);
    cvtest::copy(test_mat[INPUT][0],test_mat[OUTPUT][0]);

    cvReleaseMat(&_mapx);
    cvReleaseMat(&_mapy);
#else
    int dist_size = test_mat[INPUT][2].cols > test_mat[INPUT][2].rows ? test_mat[INPUT][2].cols : test_mat[INPUT][2].rows;
    double cam[9] = {0,0,0,0,0,0,0,0,1};
    double rot[9] = {1,0,0,0,1,0,0,0,1};
    double* dist = new double[dist_size ];
    double* new_cam = new double[test_mat[INPUT][4].cols * test_mat[INPUT][4].rows];
    double* points = new double[N_POINTS*2];
    double* r_points = new double[N_POINTS*2];
    //Run reference calculations
    CvMat ref_points= cvMat(test_mat[INPUT][0].rows,test_mat[INPUT][0].cols,CC_64FC2,r_points);
    CvMat _camera = cvMat(3,3,CC_64F,cam);
    CvMat _rot = cvMat(3,3,CC_64F,rot);
    CvMat _distort = cvMat(test_mat[INPUT][2].rows,test_mat[INPUT][2].cols,CC_64F,dist);
    CvMat _new_cam = cvMat(test_mat[INPUT][4].rows,test_mat[INPUT][4].cols,CC_64F,new_cam);
    CvMat _points= cvMat(test_mat[INPUT][0].rows,test_mat[INPUT][0].cols,CC_64FC2,points);

    CvMat _input1 = test_mat[INPUT][1];
    CvMat _input2 = test_mat[INPUT][2];
    CvMat _input3 = test_mat[INPUT][3];
    CvMat _input4 = test_mat[INPUT][4];

    cvtest::convert(cvarrToMat(&_input1), cvarrToMat(&_camera), -1);
    cvtest::convert(cvarrToMat(&_input2), cvarrToMat(&_distort), -1);
    cvtest::convert(cvarrToMat(&_input3), cvarrToMat(&_rot), -1);
    cvtest::convert(cvarrToMat(&_input4), cvarrToMat(&_new_cam), -1);

    //Applying precalculated undistort rectify map
    if (!useCPlus)
    {
        mapx = cvarrToMat(_mapx);
        mapy = cvarrToMat(_mapy);
    }
    CvMat map1,map2;
    convertMaps(mapx,mapy,map1,map2,CC_32FC1);
    CvMat _map1 = map1;
    CvMat _map2 = map2;
    for (int i=0;i<N_POINTS;i++)
    {
        double u = test_mat[INPUT][0].ptr<double>()[2*i];
        double v = test_mat[INPUT][0].ptr<double>()[2*i+1];
        _points.tt.db[2*i] = (double)_map1.data.fl[(int)v*_map1.cols+(int)u];
        _points.tt.db[2*i+1] = (double)_map2.data.fl[(int)v*_map2.cols+(int)u];
    }

    //---

    cvUndistortPoints(&_points,&ref_points,&_camera,
                      zero_distortion ? 0 : &_distort, zero_R ? 0 : &_rot, zero_new_cam ? &_camera : &_new_cam);
    //cvTsDistortPoints(&_points,&ref_points,&_camera,&_distort,&_rot,&_new_cam);
    CvMat dst = test_mat[REF_OUTPUT][0];
    cvtest::convert(cvarrToMat(&ref_points), cvarrToMat(&dst), -1);

    cvtest::copy(test_mat[INPUT][0],test_mat[OUTPUT][0]);

    delete[] dist;
    delete[] new_cam;
    delete[] points;
    delete[] r_points;
    cvReleaseMat(&_mapx);
    cvReleaseMat(&_mapy);
#endif
}

void CC_InitUndistortRectifyMapTest::run_func()
{
    if (useCPlus)
    {
        CvMat input2,input3,input4;
        input2 = zero_distortion ? CvMat() : test_mat[INPUT][2];
        input3 = zero_R ? CvMat() : test_mat[INPUT][3];
        input4 = zero_new_cam ? CvMat() : test_mat[INPUT][4];
        initUndistortRectifyMap(camera_mat,input2,input3,input4,img_size,mat_type,mapx,mapy);
    }
    else
    {
        CvMat input1 = test_mat[INPUT][1], input2, input3, input4;
        if( !zero_distortion )
            input2 = test_mat[INPUT][2];
        if( !zero_R )
            input3 = test_mat[INPUT][3];
        if( !zero_new_cam )
            input4 = test_mat[INPUT][4];
        cvInitUndistortRectifyMap(&input1,
                                  zero_distortion ? 0 : &input2,
                                  zero_R ? 0 : &input3,
                                  zero_new_cam ? 0 : &input4,
                                  _mapx,_mapy);
    }
}

double CC_InitUndistortRectifyMapTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    return 8;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Calib3d_DefaultNewCameraMatrix, accuracy) { CC_DefaultNewCameraMatrixTest test; test.safe_run(); }
TEST(Calib3d_UndistortPoints, accuracy) { CC_UndistortPointsTest test; test.safe_run(); }
TEST(Calib3d_InitUndistortRectifyMap, accuracy) { CC_InitUndistortRectifyMapTest test; test.safe_run(); }
