/*  For description look into the help() function. */

#include "opencv2/core.hpp"
#include <iostream>

using namespace std;
using namespace cv;

static void help()
{
    cout
    << "\n--------------------------------------------------------------------------" << endl
    << "This program shows how to create matrices(CvMat) in OpenCV and its serial"
    << " out capabilities"                                                            << endl
    << "That is, CvMat M(...); M.create and cout << M. "                            << endl
    << "Shows how output can be formated to OpenCV, python, numpy, csv and C styles." << endl
    << "Usage:"                                                                       << endl
    << "./mat_the_basic_image_container"                                              << endl
    << "--------------------------------------------------------------------------"   << endl
    << endl;
}

int main(int,char**)
{
    help();
    // create by using the constructor
    //! [constructor]
    CvMat M(2,2, CC_8UC3, Scalar(0,0,255));
    cout << "M = " << endl << " " << M << endl << endl;
    //! [constructor]

    // create by using the create function()
    //! [create]
    M.create(4,4, CC_8UC(2));
    cout << "M = "<< endl << " "  << M << endl << endl;
    //! [create]

    // create multidimensional matrices
    //! [init]
    int sz[3] = {2,2,2};
    CvMat L(3,sz, CC_8UC(1), Scalar::all(0));
    //! [init]

    // Cannot print via operator <<

    // Create using MATLAB style eye, ones or zero matrix
    //! [matlab]
    CvMat E = CvMat::eye(4, 4, CC_64F);
    cout << "E = " << endl << " " << E << endl << endl;
    CvMat O = CvMat::ones(2, 2, CC_32F);
    cout << "O = " << endl << " " << O << endl << endl;
    CvMat Z = CvMat::zeros(3,3, CC_8UC1);
    cout << "Z = " << endl << " " << Z << endl << endl;
    //! [matlab]

    // create a 3x3 double-precision identity matrix
    //! [comma]
    CvMat C = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    cout << "C = " << endl << " " << C << endl << endl;
    //! [comma]

    //! [clone]
    CvMat RowClone = C.row(1).clone();
    cout << "RowClone = " << endl << " " << RowClone << endl << endl;
    //! [clone]

    // Fill a matrix with random values
    //! [random]
    CvMat R = CvMat(3, 2, CC_8UC3);
    randu(R, Scalar::all(0), Scalar::all(255));
    //! [random]

    // Demonstrate the output formating options
    //! [out-default]
    cout << "R (default) = " << endl <<        R           << endl << endl;
    //! [out-default]
    //! [out-python]
    cout << "R (python)  = " << endl << format(R, Formatter::FMT_PYTHON) << endl << endl;
    //! [out-python]
    //! [out-numpy]
    cout << "R (numpy)   = " << endl << format(R, Formatter::FMT_NUMPY ) << endl << endl;
    //! [out-numpy]
    //! [out-csv]
    cout << "R (csv)     = " << endl << format(R, Formatter::FMT_CSV   ) << endl << endl;
    //! [out-csv]
    //! [out-c]
    cout << "R (c)       = " << endl << format(R, Formatter::FMT_C     ) << endl << endl;
    //! [out-c]

    //! [out-point2]
    Point2f P(5, 1);
    cout << "Point (2D) = " << P << endl << endl;
    //! [out-point2]

    //! [out-point3]
    Point3f P3f(2, 6, 7);
    cout << "Point (3D) = " << P3f << endl << endl;
    //! [out-point3]

    //! [out-vector]
    vector<float> v;
    v.push_back( (float)CC_PI);   v.push_back(2);    v.push_back(3.01f);
    cout << "Vector of floats via CvMat = " << CvMat(v) << endl << endl;
    //! [out-vector]

    //! [out-vector-points]
    vector<Point2f> vPoints(20);
    for (size_t i = 0; i < vPoints.size(); ++i)
        vPoints[i] = Point2f((float)(i * 5), (float)(i % 7));
    cout << "A vector of 2D Points = " << vPoints << endl << endl;
    //! [out-vector-points]
    return 0;
}
