/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "test_precomp.hpp"

using namespace cv;
using namespace std;

class CC_FilterBaseTest : public cvtest::ArrayTest
{
public:
    CC_FilterBaseTest( bool _fp_kernel );

protected:
    int prepare_test_case( int test_case_idx );
    int read_params( CvFileStorage* fs );
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    void get_minmax_bounds( int i, int j, int type, Scalar& low, Scalar& high );
    CvSize aperture_size;
    CvPoint anchor;
    int max_aperture_size;
    bool fp_kernel;
    bool inplace;
    int border;
};


CC_FilterBaseTest::CC_FilterBaseTest( bool _fp_kernel ) : fp_kernel(_fp_kernel)
{
    test_array[INPUT].push_back(NULL);
    test_array[INPUT].push_back(NULL);
    test_array[OUTPUT].push_back(NULL);
    test_array[REF_OUTPUT].push_back(NULL);
    max_aperture_size = 13;
    inplace = false;
    aperture_size = cvSize(0,0);
    anchor = cvPoint(0,0);
    element_wise_relative_error = false;
}


int CC_FilterBaseTest::read_params( CvFileStorage* fs )
{
    int code = cvtest::ArrayTest::read_params( fs );
    if( code < 0 )
        return code;

    max_aperture_size = cvReadInt( find_param( fs, "max_aperture_size" ), max_aperture_size );
    max_aperture_size = cvtest::clipInt( max_aperture_size, 1, 100 );

    return code;
}


void CC_FilterBaseTest::get_minmax_bounds( int i, int j, int type, Scalar& low, Scalar& high )
{
    cvtest::ArrayTest::get_minmax_bounds( i, j, type, low, high );
    if( i == INPUT )
    {
        if( j == 1 )
        {
            if( fp_kernel )
            {
                RNG& rng = ts->get_rng();
                double val = exp( cvtest::randReal(rng)*10 - 4 );
                low = Scalar::all(-val);
                high = Scalar::all(val);
            }
            else
            {
                low = Scalar::all(0);
                high = Scalar::all(2);
            }
        }
        else if( CC_MAT_DEPTH(type) == CC_16U )
        {
            low = Scalar::all(0.);
            high = Scalar::all(40000.);
        }
        else if( CC_MAT_DEPTH(type) == CC_32F )
        {
            low = Scalar::all(-10.);
            high = Scalar::all(10.);
        }
    }
}


void CC_FilterBaseTest::get_test_array_types_and_sizes( int test_case_idx,
                                                        vector<vector<CvSize> >& sizes,
                                                        vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    int depth = cvtest::randInt(rng) % CC_32F;
    int cn = cvtest::randInt(rng) % 3 + 1;
    cvtest::ArrayTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    depth += depth == CC_8S;
    cn += cn == 2;

    types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_MAKETYPE(depth, cn);

    aperture_size.width = cvtest::randInt(rng) % max_aperture_size + 1;
    aperture_size.height = cvtest::randInt(rng) % max_aperture_size + 1;
    anchor.x = cvtest::randInt(rng) % aperture_size.width;
    anchor.y = cvtest::randInt(rng) % aperture_size.height;

    types[INPUT][1] = fp_kernel ? CC_32FC1 : CC_8UC1;
    sizes[INPUT][1] = aperture_size;

    inplace = cvtest::randInt(rng) % 2 != 0;
    border = BORDER_REPLICATE;
}


int CC_FilterBaseTest::prepare_test_case( int test_case_idx )
{
    int code = cvtest::ArrayTest::prepare_test_case( test_case_idx );
    if( code > 0 )
    {
        if( inplace && test_mat[INPUT][0]->tid == test_mat[OUTPUT][0]->tid)
            cvtest::copy( test_mat[INPUT][0], test_mat[OUTPUT][0] );
        else
            inplace = false;
    }
    return code;
}


/////////////////////////

class CC_MorphologyBaseTest : public CC_FilterBaseTest
{
public:
    CC_MorphologyBaseTest();

protected:
    void prepare_to_validation( int test_case_idx );
    int prepare_test_case( int test_case_idx );
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    double get_success_error_level( int test_case_idx, int i, int j );
    int optype, optype_min, optype_max;
    int shape;
    IplConvKernel* element;
};


CC_MorphologyBaseTest::CC_MorphologyBaseTest() : CC_FilterBaseTest( false )
{
    shape = -1;
    element = 0;
    optype = optype_min = optype_max = -1;
}


void CC_MorphologyBaseTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    CC_FilterBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    int depth = cvtest::randInt(rng) % 4;
    depth = depth == 0 ? CC_8U : depth == 1 ? CC_16U : depth == 2 ? CC_16S : CC_32F;
    int cn = CC_MAT_CN(types[INPUT][0]);

    types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_MAKETYPE(depth, cn);
    shape = cvtest::randInt(rng) % 4;
    if( shape >= 3 )
        shape = CC_SHAPE_CUSTOM;
    else
        sizes[INPUT][1] = cvSize(0,0);
    optype = cvtest::randInt(rng) % (optype_max - optype_min + 1) + optype_min;
}


double CC_MorphologyBaseTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    return test_mat[INPUT][0].depth() < CC_32F ||
        (optype == CC_MOP_ERODE || optype == CC_MOP_DILATE ||
        optype == CC_MOP_OPEN || optype == CC_MOP_CLOSE) ? 0 : 1e-5;
}


int CC_MorphologyBaseTest::prepare_test_case( int test_case_idx )
{
    int code = CC_FilterBaseTest::prepare_test_case( test_case_idx );
    vector<int> eldata;

    if( code <= 0 )
        return code;

    if( shape == CC_SHAPE_CUSTOM )
    {
        eldata.resize(aperture_size.width*aperture_size.height);
        const uchar* src = test_mat[INPUT][1].ptr();
        int srcstep = (int)test_mat[INPUT][1].step;
        int i, j, nonzero = 0;

        for( i = 0; i < aperture_size.height; i++ )
        {
            for( j = 0; j < aperture_size.width; j++ )
            {
                eldata[i*aperture_size.width + j] = src[i*srcstep + j];
                nonzero += src[i*srcstep + j] != 0;
            }
        }

        if( nonzero == 0 )
            eldata[anchor.y*aperture_size.width + anchor.x] = 1;
    }

    cvReleaseStructuringElement( &element );
    element = cvCreateStructuringElementEx( aperture_size.width, aperture_size.height,
                                           anchor.x, anchor.y, shape, eldata.empty() ? 0 : &eldata[0] );
    return code;
}


void CC_MorphologyBaseTest::prepare_to_validation( int /*test_case_idx*/ )
{
    CvMat& src = test_mat[INPUT][0], &dst = test_mat[REF_OUTPUT][0];
    CvMat _ielement(element->nRows, element->nCols, CC_32S, element->values);
    CvMat _element;
    _ielement.convertTo(_element, CC_8U);
    Point _anchor(element->anchorX, element->anchorY);
    int _border = BORDER_REPLICATE;

    if( optype == CC_MOP_ERODE )
    {
        cvtest::erode( src, dst, _element, _anchor, _border );
    }
    else if( optype == CC_MOP_DILATE )
    {
        cvtest::dilate( src, dst, _element, _anchor, _border );
    }
    else
    {
        CvMat temp;
        if( optype == CC_MOP_OPEN )
        {
            cvtest::erode( src, temp, _element, _anchor, _border );
            cvtest::dilate( temp, dst, _element, _anchor, _border );
        }
        else if( optype == CC_MOP_CLOSE )
        {
            cvtest::dilate( src, temp, _element, _anchor, _border );
            cvtest::erode( temp, dst, _element, _anchor, _border );
        }
        else if( optype == CC_MOP_GRADIENT )
        {
            cvtest::erode( src, temp, _element, _anchor, _border );
            cvtest::dilate( src, dst, _element, _anchor, _border );
            cvtest::add( dst, 1, temp, -1, Scalar::all(0), dst, dst->tid );
        }
        else if( optype == CC_MOP_TOPHAT )
        {
            cvtest::erode( src, temp, _element, _anchor, _border );
            cvtest::dilate( temp, dst, _element, _anchor, _border );
            cvtest::add( src, 1, dst, -1, Scalar::all(0), dst, dst->tid );
        }
        else if( optype == CC_MOP_BLACKHAT )
        {
            cvtest::dilate( src, temp, _element, _anchor, _border );
            cvtest::erode( temp, dst, _element, _anchor, _border );
            cvtest::add( dst, 1, src, -1, Scalar::all(0), dst, dst->tid );
        }
        else
            CC_Error( CC_StsBadArg, "Unknown operation" );
    }

    cvReleaseStructuringElement( &element );
}


/////////////// erode ///////////////

class CC_ErodeTest : public CC_MorphologyBaseTest
{
public:
    CC_ErodeTest();
protected:
    void run_func();
};


CC_ErodeTest::CC_ErodeTest()
{
    optype_min = optype_max = CC_MOP_ERODE;
}


void CC_ErodeTest::run_func()
{
    cvErode( inplace ? test_array[OUTPUT][0] : test_array[INPUT][0],
             test_array[OUTPUT][0], element, 1 );
}


/////////////// dilate ///////////////

class CC_DilateTest : public CC_MorphologyBaseTest
{
public:
    CC_DilateTest();
protected:
    void run_func();
};


CC_DilateTest::CC_DilateTest()
{
    optype_min = optype_max = CC_MOP_DILATE;
}


void CC_DilateTest::run_func()
{
    cvDilate( inplace ? test_array[OUTPUT][0] : test_array[INPUT][0],
             test_array[OUTPUT][0], element, 1 );
}

/////////////// morphEx ///////////////

class CC_MorphExTest : public CC_MorphologyBaseTest
{
public:
    CC_MorphExTest();
protected:
    void run_func();
};


CC_MorphExTest::CC_MorphExTest()
{
    optype_min = CC_MOP_ERODE;
    optype_max = CC_MOP_BLACKHAT;
}


void CC_MorphExTest::run_func()
{
    cvMorphologyEx( test_array[inplace ? OUTPUT : INPUT][0],
             test_array[OUTPUT][0], 0, element, optype, 1 );
}

/////////////// generic filter ///////////////

class CC_FilterTest : public CC_FilterBaseTest
{
public:
    CC_FilterTest();

protected:
    void prepare_to_validation( int test_case_idx );
    void run_func();
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    double get_success_error_level( int test_case_idx, int i, int j );
};


CC_FilterTest::CC_FilterTest() : CC_FilterBaseTest( true )
{
}


void CC_FilterTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    CC_FilterBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    RNG& rng = ts->get_rng();
    int depth = cvtest::randInt(rng)%3;
    int cn = CC_MAT_CN(types[INPUT][0]);
    depth = depth == 0 ? CC_8U : depth == 1 ? CC_16U : CC_32F;
    types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_MAKETYPE(depth, cn);
}


double CC_FilterTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    int depth = test_mat[INPUT][0].depth();
    return depth <= CC_8S ? 2 : depth <= CC_32S ? 32 :
           depth == CC_32F ? 1e-4 : 1e-10;
}


void CC_FilterTest::run_func()
{
    CvMat kernel = test_mat[INPUT][1];
    cvFilter2D( test_array[inplace ? OUTPUT : INPUT][0],
                test_array[OUTPUT][0], &kernel, anchor );
}


void CC_FilterTest::prepare_to_validation( int /*test_case_idx*/ )
{
    cvtest::filter2D( test_mat[INPUT][0], test_mat[REF_OUTPUT][0], test_mat[REF_OUTPUT][0]->tid,
                      test_mat[INPUT][1], anchor, 0, BORDER_REPLICATE );
}


////////////////////////

class CC_DerivBaseTest : public CC_FilterBaseTest
{
public:
    CC_DerivBaseTest();
protected:
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    double get_success_error_level( int test_case_idx, int i, int j );
    int _aperture_size;
};


CC_DerivBaseTest::CC_DerivBaseTest() : CC_FilterBaseTest( true )
{
    max_aperture_size = 7;
}


void CC_DerivBaseTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    CC_FilterBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    int depth = cvtest::randInt(rng) % 2;
    depth = depth == 0 ? CC_8U : CC_32F;
    types[INPUT][0] = CC_MAKETYPE(depth,1);
    types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_MAKETYPE(depth==CC_8U?CC_16S:CC_32F,1);
    _aperture_size = (cvtest::randInt(rng)%5)*2 - 1;
    sizes[INPUT][1] = aperture_size = cvSize(_aperture_size, _aperture_size);
}


double CC_DerivBaseTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    int depth = test_mat[INPUT][0].depth();
    return depth <= CC_8S ? 2 : 5e-4;
}


/////////////// sobel ///////////////

class CC_SobelTest : public CC_DerivBaseTest
{
public:
    CC_SobelTest();

protected:
    void prepare_to_validation( int test_case_idx );
    void run_func();
    void get_test_array_types_and_sizes( int test_case_idx,
        vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    int dx, dy, origin;
};


CC_SobelTest::CC_SobelTest() {}


void CC_SobelTest::get_test_array_types_and_sizes( int test_case_idx,
                                                   vector<vector<CvSize> >& sizes,
                                                   vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    CC_DerivBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    int max_d = _aperture_size > 0 ? 2 : 1;
    origin = cvtest::randInt(rng) % 2;
    dx = cvtest::randInt(rng) % (max_d + 1);
    dy = cvtest::randInt(rng) % (max_d + 1 - dx);
    if( dx == 0 && dy == 0 )
        dx = 1;
    if( cvtest::randInt(rng) % 2 )
    {
        int t;
        CC_SWAP( dx, dy, t );
    }

    if( _aperture_size < 0 )
        aperture_size = cvSize(3, 3);
    else if( _aperture_size == 1 )
    {
        if( dx == 0 )
            aperture_size = cvSize(1, 3);
        else if( dy == 0 )
            aperture_size = cvSize(3, 1);
        else
        {
            _aperture_size = 3;
            aperture_size = cvSize(3, 3);
        }
    }
    else
        aperture_size = cvSize(_aperture_size, _aperture_size);

    sizes[INPUT][1] = aperture_size;
    anchor.x = aperture_size.width / 2;
    anchor.y = aperture_size.height / 2;
}


void CC_SobelTest::run_func()
{
    cvSobel( test_array[inplace ? OUTPUT : INPUT][0],
             test_array[OUTPUT][0], dx, dy, _aperture_size );
    /*Sobel( test_mat[inplace ? OUTPUT : INPUT][0],
               test_mat[OUTPUT][0], test_mat[OUTPUT][0].depth(),
               dx, dy, _aperture_size, 1, 0, border );*/
}


void CC_SobelTest::prepare_to_validation( int /*test_case_idx*/ )
{
    CvMat kernel = cvtest::calcSobelKernel2D( dx, dy, _aperture_size, 0 );
    cvtest::filter2D( test_mat[INPUT][0], test_mat[REF_OUTPUT][0], test_mat[REF_OUTPUT][0].depth(),
                      kernel, anchor, 0, BORDER_REPLICATE);
}


/////////////// spatialGradient ///////////////

class CC_SpatialGradientTest : public CC_DerivBaseTest
{
public:
    CC_SpatialGradientTest();

protected:
    void prepare_to_validation( int test_case_idx );
    void run_func();
    void get_test_array_types_and_sizes( int test_case_idx,
        vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    int ksize;
};

CC_SpatialGradientTest::CC_SpatialGradientTest() {
    test_array[OUTPUT].push_back(NULL);
    test_array[REF_OUTPUT].push_back(NULL);
    inplace = false;
}


void CC_SpatialGradientTest::get_test_array_types_and_sizes( int test_case_idx,
                                                             vector<vector<CvSize> >& sizes,
                                                             vector<vector<int> >& types )
{
    CC_DerivBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );

    sizes[OUTPUT][1] = sizes[REF_OUTPUT][1] = sizes[OUTPUT][0];

    // Inputs are only CC_8UC1 for now
    types[INPUT][0] = CC_8UC1;

    // Outputs are only CC_16SC1 for now
    types[OUTPUT][0] = types[OUTPUT][1] = types[REF_OUTPUT][0]
                     = types[REF_OUTPUT][1] = CC_16SC1;

    ksize = 3;
    border = BORDER_DEFAULT; // TODO: Add BORDER_REPLICATE
}


void CC_SpatialGradientTest::run_func()
{
    spatialGradient( test_mat[INPUT][0], test_mat[OUTPUT][0],
                     test_mat[OUTPUT][1], ksize, border );
}

void CC_SpatialGradientTest::prepare_to_validation( int /*test_case_idx*/ )
{
    int dx, dy;

    dx = 1; dy = 0;
    Sobel( test_mat[INPUT][0], test_mat[REF_OUTPUT][0], CC_16SC1, dx, dy, ksize,
           1, 0, border );

    dx = 0; dy = 1;
    Sobel( test_mat[INPUT][0], test_mat[REF_OUTPUT][1], CC_16SC1, dx, dy, ksize,
           1, 0, border );
}


/////////////// laplace ///////////////

class CC_LaplaceTest : public CC_DerivBaseTest
{
public:
    CC_LaplaceTest();

protected:
    int prepare_test_case( int test_case_idx );
    void prepare_to_validation( int test_case_idx );
    void run_func();
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
};


CC_LaplaceTest::CC_LaplaceTest()
{
}


void CC_LaplaceTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    CC_DerivBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    if( _aperture_size <= 1 )
    {
        if( _aperture_size < 0 )
            _aperture_size = 1;
        aperture_size = cvSize(3, 3);
    }
    else
        aperture_size = cvSize(_aperture_size, _aperture_size);

    sizes[INPUT][1] = aperture_size;
    anchor.x = aperture_size.width / 2;
    anchor.y = aperture_size.height / 2;
}


void CC_LaplaceTest::run_func()
{
    cvLaplace( test_array[inplace ? OUTPUT : INPUT][0],
               test_array[OUTPUT][0], _aperture_size );
}


int CC_LaplaceTest::prepare_test_case( int test_case_idx )
{
    int code = CC_DerivBaseTest::prepare_test_case( test_case_idx );
    return _aperture_size < 0 ? 0 : code;
}


void CC_LaplaceTest::prepare_to_validation( int /*test_case_idx*/ )
{
    CvMat kernel = cvtest::calcLaplaceKernel2D( _aperture_size );
    cvtest::filter2D( test_mat[INPUT][0], test_mat[REF_OUTPUT][0], test_mat[REF_OUTPUT][0].depth(),
                      kernel, anchor, 0, BORDER_REPLICATE );
}


////////////////////////////////////////////////////////////

class CC_SmoothBaseTest : public CC_FilterBaseTest
{
public:
    CC_SmoothBaseTest();

protected:
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    double get_success_error_level( int test_case_idx, int i, int j );
    const char* smooth_type;
};


CC_SmoothBaseTest::CC_SmoothBaseTest() : CC_FilterBaseTest( true )
{
    smooth_type = "";
}


void CC_SmoothBaseTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    CC_FilterBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    int depth = cvtest::randInt(rng) % 2;
    int cn = CC_MAT_CN(types[INPUT][0]);
    depth = depth == 0 ? CC_8U : CC_32F;
    types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_MAKETYPE(depth,cn);
    anchor.x = cvtest::randInt(rng)%(max_aperture_size/2+1);
    anchor.y = cvtest::randInt(rng)%(max_aperture_size/2+1);
    aperture_size.width = anchor.x*2 + 1;
    aperture_size.height = anchor.y*2 + 1;
    sizes[INPUT][1] = aperture_size;
}


double CC_SmoothBaseTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    int depth = test_mat[INPUT][0].depth();
    return depth <= CC_8S ? 1 : 1e-5;
}


/////////////// blur ///////////////

class CC_BlurTest : public CC_SmoothBaseTest
{
public:
    CC_BlurTest();

protected:
    int prepare_test_case( int test_case_idx );
    void prepare_to_validation( int test_case_idx );
    void run_func();
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    bool normalize;
};


CC_BlurTest::CC_BlurTest()
{
}


void CC_BlurTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    CC_SmoothBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    int depth = cvtest::randInt(rng) % 4;
    int cn = (cvtest::randInt(rng) % 4) + 1;
    depth = depth == 0 ? CC_8U : depth == 1 ? CC_16U : depth == 2 ? CC_16S : CC_32F;
    types[OUTPUT][0] = types[REF_OUTPUT][0] = types[INPUT][0] = CC_MAKETYPE(depth, cn);
    normalize = cvtest::randInt(rng) % 2 != 0;
    if( !normalize )
    {
        types[INPUT][0] = CC_MAKETYPE(depth, 1);
        types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_MAKETYPE(depth==CC_8U?CC_16S:CC_32F,1);
    }
}


void CC_BlurTest::run_func()
{
    cvSmooth( inplace ? test_array[OUTPUT][0] : test_array[INPUT][0],
              test_array[OUTPUT][0], normalize ? CC_BLUR : CC_BLUR_NO_SCALE,
              aperture_size.width, aperture_size.height );
}


int CC_BlurTest::prepare_test_case( int test_case_idx )
{
    int code = CC_SmoothBaseTest::prepare_test_case( test_case_idx );
    return code > 0 && !normalize && test_mat[INPUT][0].channels() > 1 ? 0 : code;
}


void CC_BlurTest::prepare_to_validation( int /*test_case_idx*/ )
{
    CvMat kernel(aperture_size, CC_64F);
    kernel.setTo(Scalar::all(normalize ? 1./(aperture_size.width*aperture_size.height) : 1.));
    cvtest::filter2D( test_mat[INPUT][0], test_mat[REF_OUTPUT][0], test_mat[REF_OUTPUT][0].depth(),
                      kernel, anchor, 0, BORDER_REPLICATE );
}


/////////////// gaussian ///////////////

class CC_GaussianBlurTest : public CC_SmoothBaseTest
{
public:
    CC_GaussianBlurTest();

protected:
    void prepare_to_validation( int test_case_idx );
    void run_func();
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    double get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ );
    double sigma;
    int param1, param2;
};


CC_GaussianBlurTest::CC_GaussianBlurTest() : CC_SmoothBaseTest()
{
    sigma = 0.;
    smooth_type = "Gaussian";
}


double CC_GaussianBlurTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    int depth = test_mat[INPUT][0].depth();
    return depth <= CC_8S ? 8 : 1e-5;
}


void CC_GaussianBlurTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    int kernel_case = cvtest::randInt(rng) % 2;
    CC_SmoothBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    anchor = cvPoint(aperture_size.width/2,aperture_size.height/2);

    sigma = exp(cvtest::randReal(rng)*5-2);
    param1 = aperture_size.width;
    param2 = aperture_size.height;

    if( kernel_case == 0 )
        sigma = 0.;
}

void CC_GaussianBlurTest::run_func()
{
    cvSmooth( test_array[inplace ? OUTPUT : INPUT][0],
              test_array[OUTPUT][0], CC_GAUSSIAN,
              param1, param2, sigma, sigma );
}


// !!! Copied from cvSmooth, if the code is changed in cvSmooth,
// make sure to update this one too.
#define SMALL_GAUSSIAN_SIZE 7
static void
calcGaussianKernel( int n, double sigma, vector<float>& kernel )
{
    static const float small_gaussian_tab[][SMALL_GAUSSIAN_SIZE] =
    {
        {1.f},
        {0.25f, 0.5f, 0.25f},
        {0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f},
        {0.03125, 0.109375, 0.21875, 0.28125, 0.21875, 0.109375, 0.03125}
    };

    kernel.resize(n);
    if( n <= SMALL_GAUSSIAN_SIZE && sigma <= 0 )
    {
        assert( n%2 == 1 );
        memcpy( &kernel[0], small_gaussian_tab[n>>1], n*sizeof(kernel[0]));
    }
    else
    {
        double sigmaX = sigma > 0 ? sigma : (n/2 - 1)*0.3 + 0.8;
        double scale2X = -0.5/(sigmaX*sigmaX);
        double sum = 1.;
        int i;
        sum = kernel[n/2] = 1.f;

        for( i = 1; i <= n/2; i++ )
        {
            kernel[n/2+i] = kernel[n/2-i] = (float)exp(scale2X*i*i);
            sum += kernel[n/2+i]*2;
        }

        sum = 1./sum;
        for( i = 0; i <= n/2; i++ )
            kernel[n/2+i] = kernel[n/2-i] = (float)(kernel[n/2+i]*sum);
    }
}


static CvMat calcGaussianKernel2D( CvSize ksize, double sigma )
{
    vector<float> kx, ky;
    CvMat kernel(ksize, CC_32F);

    calcGaussianKernel( kernel.cols, sigma, kx );
    calcGaussianKernel( kernel.rows, sigma, ky );

    for( int i = 0; i < kernel.rows; i++ )
        for( int j = 0; j < kernel.cols; j++ )
            kernel.at<float>(i, j) = kx[j]*ky[i];
    return kernel;
}


void CC_GaussianBlurTest::prepare_to_validation( int /*test_case_idx*/ )
{
    CvMat kernel = calcGaussianKernel2D( aperture_size, sigma );
    cvtest::filter2D( test_mat[INPUT][0], test_mat[REF_OUTPUT][0], test_mat[REF_OUTPUT][0].depth(),
                      kernel, anchor, 0, border & ~BORDER_ISOLATED );
}


/////////////// median ///////////////

class CC_MedianBlurTest : public CC_SmoothBaseTest
{
public:
    CC_MedianBlurTest();

protected:
    void prepare_to_validation( int test_case_idx );
    double get_success_error_level( int test_case_idx, int i, int j );
    void run_func();
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
};


CC_MedianBlurTest::CC_MedianBlurTest()
{
    smooth_type = "Median";
}


void CC_MedianBlurTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    CC_SmoothBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    int depth = CC_8U;
    int cn = CC_MAT_CN(types[INPUT][0]);
    types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_MAKETYPE(depth,cn);
    types[INPUT][1] = CC_MAKETYPE(depth,1);

    aperture_size.height = aperture_size.width;
    anchor.x = anchor.y = aperture_size.width / 2;
    sizes[INPUT][1] = cvSize(aperture_size.width,aperture_size.height);

    sizes[OUTPUT][0] = sizes[INPUT][0];
    sizes[REF_OUTPUT][0] = sizes[INPUT][0];

    inplace = false;
    border = BORDER_REPLICATE | BORDER_ISOLATED;
}


double CC_MedianBlurTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    return 0;
}


void CC_MedianBlurTest::run_func()
{
    cvSmooth( test_array[INPUT][0], test_array[OUTPUT][0],
              CC_MEDIAN, aperture_size.width );
}


struct median_pair
{
    int col;
    int val;
    median_pair() { }
    median_pair( int _col, int _val ) : col(_col), val(_val) { }
};


static void test_medianFilter( const CvMat& src, CvMat& dst, int m )
{
    int i, j, k, l, m2 = m*m, n;
    vector<int> col_buf(m+1);
    vector<median_pair> _buf0(m*m+1), _buf1(m*m+1);
    median_pair *buf0 = &_buf0[0], *buf1 = &_buf1[0];
    int step = (int)(src.step/src.elemSize());

    assert( src.rows == dst.rows + m - 1 && src.cols == dst.cols + m - 1 &&
            src->tid == dst->tid && src->tid == CC_8UC1 );

    for( i = 0; i < dst.rows; i++ )
    {
        uchar* dst1 = dst.ptr<uchar>(i);
        for( k = 0; k < m; k++ )
        {
            const uchar* src1 = src.ptr<uchar>(i+k);
            for( j = 0; j < m-1; j++ )
                *buf0++ = median_pair(j, src1[j]);
        }

        n = m2 - m;
        buf0 -= n;
        for( k = n-1; k > 0; k-- )
        {
            int f = 0;
            for( j = 0; j < k; j++ )
            {
                if( buf0[j].val > buf0[j+1].val )
                {
                    median_pair t;
                    CC_SWAP( buf0[j], buf0[j+1], t );
                    f = 1;
                }
            }
            if( !f )
                break;
        }

        for( j = 0; j < dst.cols; j++ )
        {
            int ins_col = j + m - 1;
            int del_col = j - 1;
            const uchar* src1 = src.ptr<uchar>(i) + ins_col;
            for( k = 0; k < m; k++, src1 += step )
            {
                col_buf[k] = src1[0];
                for( l = k-1; l >= 0; l-- )
                {
                    int t;
                    if( col_buf[l] < col_buf[l+1] )
                        break;
                    CC_SWAP( col_buf[l], col_buf[l+1], t );
                }
            }

            col_buf[m] = INT_MAX;

            for( k = 0, l = 0; k < n; )
            {
                if( buf0[k].col == del_col )
                    k++;
                else if( buf0[k].val < col_buf[l] )
                    *buf1++ = buf0[k++];
                else
                {
                    assert( col_buf[l] < INT_MAX );
                    *buf1++ = median_pair(ins_col,col_buf[l++]);
                }
            }

            for( ; l < m; l++ )
                *buf1++ = median_pair(ins_col,col_buf[l]);

            if( del_col < 0 )
                n += m;
            buf1 -= n;
            assert( n == m2 );
            dst1[j] = (uchar)buf1[n/2].val;
            median_pair* tbuf;
            CC_SWAP( buf0, buf1, tbuf );
        }
    }
}


void CC_MedianBlurTest::prepare_to_validation( int /*test_case_idx*/ )
{
    // CC_SmoothBaseTest::prepare_to_validation( test_case_idx );
    const CvMat& src0 = test_mat[INPUT][0];
    CvMat& dst0 = test_mat[REF_OUTPUT][0];
    int i, cn = src0.channels();
    int m = aperture_size.width;
    CvMat src(src0.rows + m - 1, src0.cols + m - 1, src0.depth());
    CvMat dst;
    if( cn == 1 )
        dst = dst0;
    else
        dst.create(src0.size(), src0.depth());

    for( i = 0; i < cn; i++ )
    {
        CvMat ptr = src0;
        if( cn > 1 )
        {
            cvtest::extract( src0, dst, i );
            ptr = dst;
        }
        cvtest::copyMakeBorder( ptr, src, m/2, m/2, m/2, m/2, border & ~BORDER_ISOLATED );
        test_medianFilter( src, dst, m );
        if( cn > 1 )
            cvtest::insert( dst, dst0, i );
    }
}


/////////////// pyramid tests ///////////////

class CC_PyramidBaseTest : public CC_FilterBaseTest
{
public:
    CC_PyramidBaseTest( bool downsample );

protected:
    double get_success_error_level( int test_case_idx, int i, int j );
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    bool downsample;
    CvMat kernel;
};


CC_PyramidBaseTest::CC_PyramidBaseTest( bool _downsample ) : CC_FilterBaseTest(true)
{
    static float kdata[] = { 1.f, 4.f, 6.f, 4.f, 1.f };
    downsample = _downsample;
    CvMat kernel1d(1, 5, CC_32F, kdata);
    kernel = (kernel1d.t()*kernel1d)*((downsample ? 1 : 4)/256.);
}


double CC_PyramidBaseTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    int depth = test_mat[INPUT][0].depth();
    return depth < CC_32F ? 1 : 1e-5;
}


void CC_PyramidBaseTest::get_test_array_types_and_sizes( int test_case_idx,
                                                         vector<vector<CvSize> >& sizes,
                                                         vector<vector<int> >& types )
{
    const int channels[] = {1, 3, 4};
    const int depthes[] = {CC_8U, CC_16S, CC_16U, CC_32F};

    RNG& rng = ts->get_rng();
    CvSize sz;
    CC_FilterBaseTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );

    int depth = depthes[cvtest::randInt(rng) % (sizeof(depthes)/sizeof(depthes[0]))];
    int cn = channels[cvtest::randInt(rng) % (sizeof(channels)/sizeof(channels[0]))];

    aperture_size = cvSize(5,5);
    anchor = cvPoint(aperture_size.width/2, aperture_size.height/2);

    types[INPUT][0] = types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_MAKETYPE(depth, cn);

    sz.width = MAX( sizes[INPUT][0].width/2, 1 );
    sz.height = MAX( sizes[INPUT][0].height/2, 1 );

    if( downsample )
    {
        sizes[OUTPUT][0] = sizes[REF_OUTPUT][0] = sz;
        sz.width *= 2;
        sz.height *= 2;
        sizes[INPUT][0] = sz;
    }
    else
    {
        sizes[INPUT][0] = sz;
        sz.width *= 2;
        sz.height *= 2;
        sizes[OUTPUT][0] = sizes[REF_OUTPUT][0] = sz;
    }

    sizes[INPUT][1] = aperture_size;
    inplace = false;
}


/////// pyrdown ////////

class CC_PyramidDownTest : public CC_PyramidBaseTest
{
public:
    CC_PyramidDownTest();

protected:
    void run_func();
    void prepare_to_validation( int );
};


CC_PyramidDownTest::CC_PyramidDownTest() : CC_PyramidBaseTest( true )
{
}


void CC_PyramidDownTest::run_func()
{
    cvPyrDown( test_array[INPUT][0], test_array[OUTPUT][0], CC_GAUSSIAN_5x5 );
}


void CC_PyramidDownTest::prepare_to_validation( int /*test_case_idx*/ )
{
    CvMat& src = test_mat[INPUT][0], &dst = test_mat[REF_OUTPUT][0];
    CvMat temp;
    cvtest::filter2D(src, temp, src.depth(),
                     kernel, Point(kernel.cols/2, kernel.rows/2),
                     0, BORDER_REFLECT_101);

    size_t elem_size = temp.elemSize();
    size_t ncols = dst.cols*elem_size;

    for( int i = 0; i < dst.rows; i++ )
    {
        const uchar* src_row = temp.ptr(i*2);
        uchar* dst_row = dst.ptr(i);

        for( size_t j = 0; j < ncols; j += elem_size )
        {
            for( size_t k = 0; k < elem_size; k++ )
                dst_row[j+k] = src_row[j*2+k];
        }
    }
}


/////// pyrup ////////

class CC_PyramidUpTest : public CC_PyramidBaseTest
{
public:
    CC_PyramidUpTest();

protected:
    void run_func();
    void prepare_to_validation( int );
};


CC_PyramidUpTest::CC_PyramidUpTest() : CC_PyramidBaseTest( false )
{
}


void CC_PyramidUpTest::run_func()
{
    cvPyrUp( test_array[INPUT][0], test_array[OUTPUT][0], CC_GAUSSIAN_5x5 );
}


void CC_PyramidUpTest::prepare_to_validation( int /*test_case_idx*/ )
{
    CvMat& src = test_mat[INPUT][0], &dst = test_mat[REF_OUTPUT][0];
    CvMat temp(dst.size(), dst->tid);

    size_t elem_size = src.elemSize();
    size_t ncols = src.cols*elem_size;

    for( int i = 0; i < src.rows; i++ )
    {
        const uchar* src_row = src.ptr(i);
        uchar* dst_row = temp.ptr(i*2);

        if( i*2 + 1 < temp.rows )
            memset( temp.ptr(i*2+1), 0, temp.cols*elem_size );
        for( size_t j = 0; j < ncols; j += elem_size )
        {
            for( size_t k = 0; k < elem_size; k++ )
            {
                dst_row[j*2+k] = src_row[j+k];
                dst_row[j*2+k+elem_size] = 0;
            }
        }
    }

    cvtest::filter2D(temp, dst, dst.depth(),
                     kernel, Point(kernel.cols/2, kernel.rows/2),
                     0, BORDER_REFLECT_101);
}


//////////////////////// feature selection //////////////////////////

class CC_FeatureSelBaseTest : public cvtest::ArrayTest
{
public:
    CC_FeatureSelBaseTest( int width_factor );

protected:
    int read_params( CvFileStorage* fs );
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    void get_minmax_bounds( int i, int j, int type, Scalar& low, Scalar& high );
    double get_success_error_level( int test_case_idx, int i, int j );
    int aperture_size, block_size;
    int max_aperture_size;
    int max_block_size;
    int width_factor;
};


CC_FeatureSelBaseTest::CC_FeatureSelBaseTest( int _width_factor )
{
    max_aperture_size = 7;
    max_block_size = 21;
    // 1 input, 1 output, temp arrays are allocated in the reference functions
    test_array[INPUT].push_back(NULL);
    test_array[OUTPUT].push_back(NULL);
    test_array[REF_OUTPUT].push_back(NULL);
    element_wise_relative_error = false;
    width_factor = _width_factor;
}


int CC_FeatureSelBaseTest::read_params( CvFileStorage* fs )
{
    int code = cvtest::BaseTest::read_params( fs );
    if( code < 0 )
        return code;

    max_aperture_size = cvReadInt( find_param( fs, "max_aperture_size" ), max_aperture_size );
    max_aperture_size = cvtest::clipInt( max_aperture_size, 1, 9 );
    max_block_size = cvReadInt( find_param( fs, "max_block_size" ), max_block_size );
    max_block_size = cvtest::clipInt( max_aperture_size, 1, 100 );

    return code;
}


double CC_FeatureSelBaseTest::get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ )
{
    int depth = test_mat[INPUT][0].depth();
    return depth <= CC_8S ? 3e-2 : depth == CC_32F ? 1e-3 : 1e-10;
}


void CC_FeatureSelBaseTest::get_minmax_bounds( int i, int j, int type, Scalar& low, Scalar& high )
{
    cvtest::ArrayTest::get_minmax_bounds( i, j, type, low, high );
    if( i == INPUT && CC_MAT_DEPTH(type) == CC_32F )
    {
        low = Scalar::all(-10.);
        high = Scalar::all(10.);
    }
}


void CC_FeatureSelBaseTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    cvtest::ArrayTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    int depth = cvtest::randInt(rng) % 2, asz;

    depth = depth == 0 ? CC_8U : CC_32F;
    types[INPUT][0] = depth;
    types[OUTPUT][0] = types[REF_OUTPUT][0] = CC_32FC1;

    aperture_size = (cvtest::randInt(rng) % (max_aperture_size+2) - 1) | 1;
    if( aperture_size == 1 )
        aperture_size = 3;
    if( depth == CC_8U )
        aperture_size = MIN( aperture_size, 5 );
    block_size = (cvtest::randInt(rng) % max_block_size + 1) | 1;
    if( block_size <= 3 )
        block_size = 3;
    asz = aperture_size > 0 ? aperture_size : 3;

    sizes[INPUT][0].width = MAX( sizes[INPUT][0].width, asz + block_size );
    sizes[INPUT][0].height = MAX( sizes[INPUT][0].height, asz + block_size );
    sizes[OUTPUT][0].height = sizes[REF_OUTPUT][0].height = sizes[INPUT][0].height;
    sizes[OUTPUT][0].width = sizes[REF_OUTPUT][0].width = sizes[INPUT][0].width*width_factor;
}


static void
test_cornerEigenValsVecs( const CvMat& src, CvMat& eigenv, CvMat& ocv_eigenv,
                          int block_size, int _aperture_size, int mode )
{
    int i, j;
    int aperture_size = _aperture_size < 0 ? 3 : _aperture_size;
    Point anchor( aperture_size/2, aperture_size/2 );

    CC_Assert( src->tid == CC_8UC1 || src->tid == CC_32FC1 );
    CC_Assert( eigenv->tid == CC_32FC1 );
    CC_Assert( src.rows == eigenv.rows &&
              ((mode > 0 && src.cols == eigenv.cols) ||
              (mode == 0 && src.cols*6 == eigenv.cols)) );

    int type = src->tid;
    int ftype = CC_32FC1;
    double kernel_scale = type != ftype ? 1./255 : 1;

    CvMat dx2, dy2, dxdy(src.size(), CC_32F), kernel;

    kernel = cvtest::calcSobelKernel2D( 1, 0, _aperture_size );
    cvtest::filter2D( src, dx2, ftype, kernel*kernel_scale, anchor, 0, BORDER_REPLICATE );
    kernel = cvtest::calcSobelKernel2D( 0, 1, _aperture_size );
    cvtest::filter2D( src, dy2, ftype, kernel*kernel_scale, anchor, 0, BORDER_REPLICATE );

    double denom = (1 << (aperture_size-1))*block_size;
    denom = denom * denom;
    if( _aperture_size < 0 )
        denom *= 4;
    denom = 1./denom;

    for( i = 0; i < src.rows; i++ )
    {
        float* dxdyp = dxdy.ptr<float>(i);
        float* dx2p = dx2.ptr<float>(i);
        float* dy2p = dy2.ptr<float>(i);

        for( j = 0; j < src.cols; j++ )
        {
            double xval = dx2p[j], yval = dy2p[j];
            dxdyp[j] = (float)(xval*yval*denom);
            dx2p[j] = (float)(xval*xval*denom);
            dy2p[j] = (float)(yval*yval*denom);
        }
    }

    kernel = CvMat::ones(block_size, block_size, CC_32F);
    anchor = Point(block_size/2, block_size/2);

    cvtest::filter2D( dx2, dx2, ftype, kernel, anchor, 0, BORDER_REPLICATE );
    cvtest::filter2D( dy2, dy2, ftype, kernel, anchor, 0, BORDER_REPLICATE );
    cvtest::filter2D( dxdy, dxdy, ftype, kernel, anchor, 0, BORDER_REPLICATE );

    if( mode == 0 )
    {
        for( i = 0; i < src.rows; i++ )
        {
            float* eigenvp = eigenv.ptr<float>(i);
            float* ocv_eigenvp = ocv_eigenv.ptr<float>(i);
            const float* dxdyp = dxdy.ptr<float>(i);
            const float* dx2p = dx2.ptr<float>(i);
            const float* dy2p = dy2.ptr<float>(i);

            for( j = 0; j < src.cols; j++ )
            {
                double a = dx2p[j], b = dxdyp[j], c = dy2p[j];
                double d = sqrt((a-c)*(a-c) + 4*b*b);
                double l1 = 0.5*(a + c + d);
                double l2 = 0.5*(a + c - d);
                double x1, y1, x2, y2, s;

                if( fabs(a - l1) + fabs(b) >= 1e-3 )
                    x1 = b, y1 = l1 - a;
                else
                    x1 = l1 - c, y1 = b;
                s = 1./(sqrt(x1*x1+y1*y1)+DBL_EPSILON);
                x1 *= s; y1 *= s;

                if( fabs(a - l2) + fabs(b) >= 1e-3 )
                    x2 = b, y2 = l2 - a;
                else
                    x2 = l2 - c, y2 = b;
                s = 1./(sqrt(x2*x2+y2*y2)+DBL_EPSILON);
                x2 *= s; y2 *= s;

                /* the orientation of eigen vectors might be inversed relative to OpenCV function,
                   which is normal */
                if( (fabs(x1) >= fabs(y1) && ocv_eigenvp[j*6+2]*x1 < 0) ||
                    (fabs(x1) < fabs(y1) && ocv_eigenvp[j*6+3]*y1 < 0) )
                    x1 = -x1, y1 = -y1;

                if( (fabs(x2) >= fabs(y2) && ocv_eigenvp[j*6+4]*x2 < 0) ||
                    (fabs(x2) < fabs(y2) && ocv_eigenvp[j*6+5]*y2 < 0) )
                    x2 = -x2, y2 = -y2;

                eigenvp[j*6] = (float)l1;
                eigenvp[j*6+1] = (float)l2;
                eigenvp[j*6+2] = (float)x1;
                eigenvp[j*6+3] = (float)y1;
                eigenvp[j*6+4] = (float)x2;
                eigenvp[j*6+5] = (float)y2;
            }
        }
    }
    else if( mode == 1 )
    {
        for( i = 0; i < src.rows; i++ )
        {
            float* eigenvp = eigenv.ptr<float>(i);
            const float* dxdyp = dxdy.ptr<float>(i);
            const float* dx2p = dx2.ptr<float>(i);
            const float* dy2p = dy2.ptr<float>(i);

            for( j = 0; j < src.cols; j++ )
            {
                double a = dx2p[j], b = dxdyp[j], c = dy2p[j];
                double d = sqrt((a-c)*(a-c) + 4*b*b);
                eigenvp[j] = (float)(0.5*(a + c - d));
            }
        }
    }
}


// min eigenval
class CC_MinEigenValTest : public CC_FeatureSelBaseTest
{
public:
    CC_MinEigenValTest();

protected:
    void run_func();
    void prepare_to_validation( int );
};


CC_MinEigenValTest::CC_MinEigenValTest() : CC_FeatureSelBaseTest( 1 )
{
}


void CC_MinEigenValTest::run_func()
{
    cvCornerMinEigenVal( test_array[INPUT][0], test_array[OUTPUT][0],
                         block_size, aperture_size );
}


void CC_MinEigenValTest::prepare_to_validation( int /*test_case_idx*/ )
{
    test_cornerEigenValsVecs( test_mat[INPUT][0], test_mat[REF_OUTPUT][0],
                    test_mat[OUTPUT][0], block_size, aperture_size, 1 );
}


// eigenval's & vec's
class CC_EigenValVecTest : public CC_FeatureSelBaseTest
{
public:
    CC_EigenValVecTest();

protected:
    void run_func();
    void prepare_to_validation( int );
};


CC_EigenValVecTest::CC_EigenValVecTest() : CC_FeatureSelBaseTest( 6 )
{
}


void CC_EigenValVecTest::run_func()
{
    cvCornerEigenValsAndVecs( test_array[INPUT][0], test_array[OUTPUT][0],
                              block_size, aperture_size );
}


void CC_EigenValVecTest::prepare_to_validation( int /*test_case_idx*/ )
{
    test_cornerEigenValsVecs( test_mat[INPUT][0], test_mat[REF_OUTPUT][0],
                    test_mat[OUTPUT][0], block_size, aperture_size, 0 );
}


// precornerdetect
class CC_PreCornerDetectTest : public CC_FeatureSelBaseTest
{
public:
    CC_PreCornerDetectTest();

protected:
    void run_func();
    void prepare_to_validation( int );
    int prepare_test_case( int );
};


CC_PreCornerDetectTest::CC_PreCornerDetectTest() : CC_FeatureSelBaseTest( 1 )
{
}


void CC_PreCornerDetectTest::run_func()
{
    cvPreCornerDetect( test_array[INPUT][0], test_array[OUTPUT][0], aperture_size );
}


int CC_PreCornerDetectTest::prepare_test_case( int test_case_idx )
{
    int code = CC_FeatureSelBaseTest::prepare_test_case( test_case_idx );
    if( aperture_size < 0 )
        aperture_size = 3;
    return code;
}


void CC_PreCornerDetectTest::prepare_to_validation( int /*test_case_idx*/ )
{
    /*cvTsCornerEigenValsVecs( test_mat[INPUT][0], test_mat[REF_OUTPUT][0],
                             block_size, aperture_size, 0 );*/
    const CvMat& src = test_mat[INPUT][0];
    CvMat& dst = test_mat[REF_OUTPUT][0];

    int type = src->tid, ftype = CC_32FC1;
    Point anchor(aperture_size/2, aperture_size/2);

    double kernel_scale = type != ftype ? 1./255 : 1.;

    CvMat dx, dy, d2x, d2y, dxy, kernel;

    kernel = cvtest::calcSobelKernel2D(1, 0, aperture_size);
    cvtest::filter2D(src, dx, ftype, kernel*kernel_scale, anchor, 0, BORDER_REPLICATE);
    kernel = cvtest::calcSobelKernel2D(2, 0, aperture_size);
    cvtest::filter2D(src, d2x, ftype, kernel*kernel_scale, anchor, 0, BORDER_REPLICATE);
    kernel = cvtest::calcSobelKernel2D(0, 1, aperture_size);
    cvtest::filter2D(src, dy, ftype, kernel*kernel_scale, anchor, 0, BORDER_REPLICATE);
    kernel = cvtest::calcSobelKernel2D(0, 2, aperture_size);
    cvtest::filter2D(src, d2y, ftype, kernel*kernel_scale, anchor, 0, BORDER_REPLICATE);
    kernel = cvtest::calcSobelKernel2D(1, 1, aperture_size);
    cvtest::filter2D(src, dxy, ftype, kernel*kernel_scale, anchor, 0, BORDER_REPLICATE);

    double denom = 1 << (aperture_size-1);
    denom = denom * denom * denom;
    denom = 1./denom;

    for( int i = 0; i < src.rows; i++ )
    {
        const float* _dx = dx.ptr<float>(i);
        const float* _dy = dy.ptr<float>(i);
        const float* _d2x = d2x.ptr<float>(i);
        const float* _d2y = d2y.ptr<float>(i);
        const float* _dxy = dxy.ptr<float>(i);
        float* corner = dst.ptr<float>(i);

        for( int j = 0; j < src.cols; j++ )
        {
            double x = _dx[j];
            double y = _dy[j];

            corner[j] = (float)(denom*(x*x*_d2y[j] + y*y*_d2x[j] - 2*x*y*_dxy[j]));
        }
    }
}


///////// integral /////////

class CC_IntegralTest : public cvtest::ArrayTest
{
public:
    CC_IntegralTest();

protected:
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<CvSize> >& sizes, vector<vector<int> >& types );
    void get_minmax_bounds( int i, int j, int type, Scalar& low, Scalar& high );
    double get_success_error_level( int test_case_idx, int i, int j );
    void run_func();
    void prepare_to_validation( int );

    int prepare_test_case( int test_case_idx );
};


CC_IntegralTest::CC_IntegralTest()
{
    test_array[INPUT].push_back(NULL);
    test_array[OUTPUT].push_back(NULL);
    test_array[OUTPUT].push_back(NULL);
    test_array[OUTPUT].push_back(NULL);
    test_array[REF_OUTPUT].push_back(NULL);
    test_array[REF_OUTPUT].push_back(NULL);
    test_array[REF_OUTPUT].push_back(NULL);
    element_wise_relative_error = true;
}


void CC_IntegralTest::get_minmax_bounds( int i, int j, int type, Scalar& low, Scalar& high )
{
    cvtest::ArrayTest::get_minmax_bounds( i, j, type, low, high );
    int depth = CC_MAT_DEPTH(type);
    if( depth == CC_32F )
    {
        low = Scalar::all(-10.);
        high = Scalar::all(10.);
    }
}


void CC_IntegralTest::get_test_array_types_and_sizes( int test_case_idx,
                                                vector<vector<CvSize> >& sizes, vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    int depth = cvtest::randInt(rng) % 2, sum_depth;
    int cn = cvtest::randInt(rng) % 3 + 1;
    cvtest::ArrayTest::get_test_array_types_and_sizes( test_case_idx, sizes, types );
    CvSize sum_size;

    depth = depth == 0 ? CC_8U : CC_32F;
    cn += cn == 2;
    int b = (cvtest::randInt(rng) & 1) != 0;
    sum_depth = depth == CC_8U && b ? CC_32S : b ? CC_32F : CC_64F;

    types[INPUT][0] = CC_MAKETYPE(depth,cn);
    types[OUTPUT][0] = types[REF_OUTPUT][0] =
        types[OUTPUT][2] = types[REF_OUTPUT][2] = CC_MAKETYPE(sum_depth, cn);
    types[OUTPUT][1] = types[REF_OUTPUT][1] = CC_MAKETYPE(CC_64F, cn);

    sum_size.width = sizes[INPUT][0].width + 1;
    sum_size.height = sizes[INPUT][0].height + 1;

    sizes[OUTPUT][0] = sizes[REF_OUTPUT][0] = sum_size;
    sizes[OUTPUT][1] = sizes[REF_OUTPUT][1] =
        sizes[OUTPUT][2] = sizes[REF_OUTPUT][2] = CvSize(0,0);

    if( cvtest::randInt(rng) % 3 > 0 )
    {
        sizes[OUTPUT][1] = sizes[REF_OUTPUT][1] = sum_size;
        if( cvtest::randInt(rng) % 2 > 0 )
            sizes[REF_OUTPUT][2] = sizes[OUTPUT][2] = sum_size;
    }
}


double CC_IntegralTest::get_success_error_level( int, int i, int j )
{
    int depth = test_mat[i][j].depth();
    return depth == CC_32S ? 0 : depth == CC_64F ? FLT_EPSILON : 5e-3;
}


int CC_IntegralTest::prepare_test_case( int test_case_idx )
{
    int code = cvtest::ArrayTest::prepare_test_case( test_case_idx );
    return code > 0 && ((test_array[OUTPUT][2] && test_mat[OUTPUT][2].channels() > 1) ||
        test_mat[OUTPUT][0].depth() < test_mat[INPUT][0].depth()) ? 0 : code;
}


void CC_IntegralTest::run_func()
{
    cvIntegral( test_array[INPUT][0], test_array[OUTPUT][0],
                test_array[OUTPUT][1], test_array[OUTPUT][2] );
}


static void test_integral( const CvMat& img, CvMat* sum, CvMat* sqsum, CvMat* tilted )
{
    CC_Assert( img.depth() == CC_32F );

    sum->create(img.rows+1, img.cols+1, CC_64F);
    if( sqsum )
        sqsum->create(img.rows+1, img.cols+1, CC_64F);
    if( tilted )
        tilted->create(img.rows+1, img.cols+1, CC_64F);

    const float* data = img.ptr<float>();
    double* sdata = sum->ptr<double>();
    double* sqdata = sqsum ? sqsum->ptr<double>() : 0;
    double* tdata = tilted ? tilted->ptr<double>() : 0;
    int step = (int)(img.step/sizeof(data[0]));
    int sstep = (int)(sum->step/sizeof(sdata[0]));
    int sqstep = sqsum ? (int)(sqsum->step/sizeof(sqdata[0])) : 0;
    int tstep = tilted ? (int)(tilted->step/sizeof(tdata[0])) : 0;
    CvSize size = img.size();

    memset( sdata, 0, (size.width+1)*sizeof(sdata[0]) );
    if( sqsum )
        memset( sqdata, 0, (size.width+1)*sizeof(sqdata[0]) );
    if( tilted )
        memset( tdata, 0, (size.width+1)*sizeof(tdata[0]) );

    for( ; size.height--; data += step )
    {
        double s = 0, sq = 0;
        int x;
        sdata += sstep;
        sqdata += sqstep;
        tdata += tstep;

        for( x = 0; x <= size.width; x++ )
        {
            double t = x > 0 ? data[x-1] : 0, ts = t;
            s += t;
            sq += t*t;

            sdata[x] = s + sdata[x - sstep];
            if( sqdata )
                sqdata[x] = sq + sqdata[x - sqstep];

            if( !tdata )
                continue;

            if( x == 0 )
                ts += tdata[-tstep+1];
            else
            {
                ts += tdata[x-tstep-1];
                if( data > img.ptr<float>() )
                {
                    ts += data[x-step-1];
                    if( x < size.width )
                        ts += tdata[x-tstep+1] - tdata[x-tstep*2];
                }
            }

            tdata[x] = ts;
        }
    }
}


void CC_IntegralTest::prepare_to_validation( int /*test_case_idx*/ )
{
    CvMat& src = test_mat[INPUT][0];
    int cn = src.channels();

    CvMat* sum0 = &test_mat[REF_OUTPUT][0];
    CvMat* sqsum0 = test_array[REF_OUTPUT][1] ? &test_mat[REF_OUTPUT][1] : 0;
    CvMat* tsum0 = test_array[REF_OUTPUT][2] ? &test_mat[REF_OUTPUT][2] : 0;

    CvMat plane, srcf, psum, psqsum, ptsum, psum2, psqsum2, ptsum2;
    if( cn == 1 )
    {
        plane = src;
        psum2 = *sum0;
        psqsum2 = sqsum0 ? *sqsum0 : CvMat();
        ptsum2 = tsum0 ? *tsum0 : CvMat();
    }

    for( int i = 0; i < cn; i++ )
    {
        if( cn > 1 )
            cvtest::extract(src, plane, i);
        plane.convertTo(srcf, CC_32F);

        test_integral( srcf, &psum, sqsum0 ? &psqsum : 0, tsum0 ? &ptsum : 0 );
        psum.convertTo(psum2, sum0->depth());
        if( sqsum0 )
            psqsum.convertTo(psqsum2, sqsum0->depth());
        if( tsum0 )
            ptsum.convertTo(ptsum2, tsum0->depth());

        if( cn > 1 )
        {
            cvtest::insert(psum2, *sum0, i);
            if( sqsum0 )
                cvtest::insert(psqsum2, *sqsum0, i);
            if( tsum0 )
                cvtest::insert(ptsum2, *tsum0, i);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////

TEST(Imgproc_Erode, accuracy) { CC_ErodeTest test; test.safe_run(); }
TEST(Imgproc_Dilate, accuracy) { CC_DilateTest test; test.safe_run(); }
TEST(Imgproc_MorphologyEx, accuracy) { CC_MorphExTest test; test.safe_run(); }
TEST(Imgproc_Filter2D, accuracy) { CC_FilterTest test; test.safe_run(); }
TEST(Imgproc_Sobel, accuracy) { CC_SobelTest test; test.safe_run(); }
TEST(Imgproc_SpatialGradient, accuracy) { CC_SpatialGradientTest test; test.safe_run(); }
TEST(Imgproc_Laplace, accuracy) { CC_LaplaceTest test; test.safe_run(); }
TEST(Imgproc_Blur, accuracy) { CC_BlurTest test; test.safe_run(); }
TEST(Imgproc_GaussianBlur, accuracy) { CC_GaussianBlurTest test; test.safe_run(); }
TEST(Imgproc_MedianBlur, accuracy) { CC_MedianBlurTest test; test.safe_run(); }
TEST(Imgproc_PyramidDown, accuracy) { CC_PyramidDownTest test; test.safe_run(); }
TEST(Imgproc_PyramidUp, accuracy) { CC_PyramidUpTest test; test.safe_run(); }
TEST(Imgproc_MinEigenVal, accuracy) { CC_MinEigenValTest test; test.safe_run(); }
TEST(Imgproc_EigenValsVecs, accuracy) { CC_EigenValVecTest test; test.safe_run(); }
TEST(Imgproc_PreCornerDetect, accuracy) { CC_PreCornerDetectTest test; test.safe_run(); }
TEST(Imgproc_Integral, accuracy) { CC_IntegralTest test; test.safe_run(); }

//////////////////////////////////////////////////////////////////////////////////

class CC_FilterSupportedFormatsTest : public cvtest::BaseTest
{
public:
    CC_FilterSupportedFormatsTest() {}
    ~CC_FilterSupportedFormatsTest() {}
protected:
    void run(int)
    {
        const int depths[][2] =
        {
            {CC_8U, CC_8U},
            {CC_8U, CC_16U},
            {CC_8U, CC_16S},
            {CC_8U, CC_32F},
            {CC_8U, CC_64F},
            {CC_16U, CC_16U},
            {CC_16U, CC_32F},
            {CC_16U, CC_64F},
            {CC_16S, CC_16S},
            {CC_16S, CC_32F},
            {CC_16S, CC_64F},
            {CC_32F, CC_32F},
            {CC_64F, CC_64F},
            {-1, -1}
        };

        int i = 0;
        volatile int fidx = -1;
        try
        {
            // use some "odd" size to do yet another smoke
            // testing of the non-SIMD loop tails
            CvSize sz(163, 117);
            CvMat small_kernel(5, 5, CC_32F), big_kernel(21, 21, CC_32F);
            CvMat kernelX(11, 1, CC_32F), kernelY(7, 1, CC_32F);
            CvMat symkernelX(11, 1, CC_32F), symkernelY(7, 1, CC_32F);
            randu(small_kernel, -10, 10);
            randu(big_kernel, -1, 1);
            randu(kernelX, -1, 1);
            randu(kernelY, -1, 1);
            flip(kernelX, symkernelX, 0);
            symkernelX += kernelX;
            flip(kernelY, symkernelY, 0);
            symkernelY += kernelY;

            CvMat elem_ellipse = getStructuringElement(MORPH_ELLIPSE, CvSize(7, 7));
            CvMat elem_rect = getStructuringElement(MORPH_RECT, CvSize(7, 7));

            for( i = 0; depths[i][0] >= 0; i++ )
            {
                int sdepth = depths[i][0];
                int ddepth = depths[i][1];
                CvMat src(sz, CC_MAKETYPE(sdepth, 5)), dst;
                randu(src, 0, 100);
                // non-separable filtering with a small kernel
                fidx = 0;
                filter2D(src, dst, ddepth, small_kernel);
                fidx++;
                filter2D(src, dst, ddepth, big_kernel);
                fidx++;
                sepFilter2D(src, dst, ddepth, kernelX, kernelY);
                fidx++;
                sepFilter2D(src, dst, ddepth, symkernelX, symkernelY);
                fidx++;
                Sobel(src, dst, ddepth, 2, 0, 5);
                fidx++;
                Scharr(src, dst, ddepth, 0, 1);
                if( sdepth != ddepth )
                    continue;
                fidx++;
                GaussianBlur(src, dst, CvSize(5, 5), 1.2, 1.2);
                fidx++;
                blur(src, dst, CvSize(11, 11));
                fidx++;
                morphologyEx(src, dst, MORPH_GRADIENT, elem_ellipse);
                fidx++;
                morphologyEx(src, dst, MORPH_GRADIENT, elem_rect);
            }
        }
        catch(...)
        {
            ts->printf(cvtest::TS::LOG, "Combination of depths %d => %d in %s is not supported (yet it should be)",
                       depths[i][0], depths[i][1],
                       fidx == 0 ? "filter2D (small kernel)" :
                       fidx == 1 ? "filter2D (large kernel)" :
                       fidx == 2 ? "sepFilter2D" :
                       fidx == 3 ? "sepFilter2D (symmetrical/asymmetrical kernel)" :
                       fidx == 4 ? "Sobel" :
                       fidx == 5 ? "Scharr" :
                       fidx == 6 ? "GaussianBlur" :
                       fidx == 7 ? "blur" :
                       fidx == 8 || fidx == 9 ? "morphologyEx" :
                       "unknown???");

            ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        }
    }
};

TEST(Imgproc_Filtering, supportedFormats) { CC_FilterSupportedFormatsTest test; test.safe_run(); }

TEST(Imgproc_Blur, borderTypes)
{
    CvSize kernelSize(3, 3);

    /// ksize > src_roi.size()
    CvMat src(3, 3, CC_8UC1, Scalar::all(255)), dst;
    CvMat src_roi = src(Rect(1, 1, 1, 1));
    src_roi.setTo(Scalar::all(0));

    // should work like !BORDER_ISOLATED
    blur(src_roi, dst, kernelSize, Point(-1, -1), BORDER_REPLICATE);
    EXPECT_EQ(227, dst.at<uchar>(0, 0));

    // should work like BORDER_ISOLATED
    blur(src_roi, dst, kernelSize, Point(-1, -1), BORDER_REPLICATE | BORDER_ISOLATED);
    EXPECT_EQ(0, dst.at<uchar>(0, 0));

    /// ksize <= src_roi.size()
    src = CvMat(5, 5, CC_8UC1, Scalar(255));
    src_roi = src(Rect(1, 1, 3, 3));
    src_roi.setTo(0);
    src.at<uchar>(2, 2) = 255;

    // should work like !BORDER_ISOLATED
    blur(src_roi, dst, kernelSize, Point(-1, -1), BORDER_REPLICATE);
    CvMat expected_dst =
            (Mat_<uchar>(3, 3) << 170, 113, 170, 113, 28, 113, 170, 113, 170);
    EXPECT_EQ(expected_dst->tid, dst->tid);
    EXPECT_EQ(expected_dst.size(), dst.size());
    EXPECT_DOUBLE_EQ(0.0, cvtest::norm(expected_dst, dst, NORM_INF));
}

TEST(Imgproc_Morphology, iterated)
{
    RNG& rng = theRNG();
    for( int iter = 0; iter < 30; iter++ )
    {
        int width = rng.uniform(5, 33);
        int height = rng.uniform(5, 33);
        int cn = rng.uniform(1, 5);
        int iterations = rng.uniform(1, 11);
        int op = rng.uniform(0, 2);
        CvMat src(height, width, CC_8UC(cn)), dst0, dst1, dst2;

        randu(src, 0, 256);
        if( op == 0 )
            dilate(src, dst0, CvMat(), Point(-1,-1), iterations);
        else
            erode(src, dst0, CvMat(), Point(-1,-1), iterations);

        for( int i = 0; i < iterations; i++ )
            if( op == 0 )
                dilate(i == 0 ? src : dst1, dst1, CvMat(), Point(-1,-1), 1);
            else
                erode(i == 0 ? src : dst1, dst1, CvMat(), Point(-1,-1), 1);

        CvMat kern = getStructuringElement(MORPH_RECT, CvSize(3,3));
        if( op == 0 )
            dilate(src, dst2, kern, Point(-1,-1), iterations);
        else
            erode(src, dst2, kern, Point(-1,-1), iterations);
        ASSERT_EQ(0.0, norm(dst0, dst1, NORM_INF));
        ASSERT_EQ(0.0, norm(dst0, dst2, NORM_INF));
    }
}

TEST(Imgproc_Sobel, borderTypes)
{
    int kernelSize = 3;

    /// ksize > src_roi.size()
    CvMat src = (Mat_<uchar>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9), dst, expected_dst;
    CvMat src_roi = src(Rect(1, 1, 1, 1));
    src_roi.setTo(Scalar::all(0));

    // should work like !BORDER_ISOLATED, so the function MUST read values in full matrix
    Sobel(src_roi, dst, CC_16S, 1, 0, kernelSize, 1, 0, BORDER_REPLICATE);
    EXPECT_EQ(8, dst.at<short>(0, 0));
    Sobel(src_roi, dst, CC_16S, 1, 0, kernelSize, 1, 0, BORDER_REFLECT);
    EXPECT_EQ(8, dst.at<short>(0, 0));

    // should work like BORDER_ISOLATED
    Sobel(src_roi, dst, CC_16S, 1, 0, kernelSize, 1, 0, BORDER_REPLICATE | BORDER_ISOLATED);
    EXPECT_EQ(0, dst.at<short>(0, 0));
    Sobel(src_roi, dst, CC_16S, 1, 0, kernelSize, 1, 0, BORDER_REFLECT | BORDER_ISOLATED);
    EXPECT_EQ(0, dst.at<short>(0, 0));

    /// ksize <= src_roi.size()
    src = CvMat(5, 5, CC_8UC1, Scalar(5));
    src_roi = src(Rect(1, 1, 3, 3));
    src_roi.setTo(0);

    // should work like !BORDER_ISOLATED, so the function MUST read values in full matrix
    expected_dst =
        (Mat_<short>(3, 3) << -15, 0, 15, -20, 0, 20, -15, 0, 15);
    Sobel(src_roi, dst, CC_16S, 1, 0, kernelSize, 1, 0, BORDER_REPLICATE);
    EXPECT_EQ(expected_dst->tid, dst->tid);
    EXPECT_EQ(expected_dst.size(), dst.size());
    EXPECT_DOUBLE_EQ(0.0, cvtest::norm(expected_dst, dst, NORM_INF));
    Sobel(src_roi, dst, CC_16S, 1, 0, kernelSize, 1, 0, BORDER_REFLECT);
    EXPECT_EQ(expected_dst->tid, dst->tid);
    EXPECT_EQ(expected_dst.size(), dst.size());
    EXPECT_DOUBLE_EQ(0.0, cvtest::norm(expected_dst, dst, NORM_INF));

    // should work like !BORDER_ISOLATED, so the function MUST read values in full matrix
    expected_dst = CvMat::zeros(3, 3, CC_16SC1);
    Sobel(src_roi, dst, CC_16S, 1, 0, kernelSize, 1, 0, BORDER_REPLICATE | BORDER_ISOLATED);
    EXPECT_EQ(expected_dst->tid, dst->tid);
    EXPECT_EQ(expected_dst.size(), dst.size());
    EXPECT_DOUBLE_EQ(0.0, cvtest::norm(expected_dst, dst, NORM_INF));
    Sobel(src_roi, dst, CC_16S, 1, 0, kernelSize, 1, 0, BORDER_REFLECT | BORDER_ISOLATED);
    EXPECT_EQ(expected_dst->tid, dst->tid);
    EXPECT_EQ(expected_dst.size(), dst.size());
    EXPECT_DOUBLE_EQ(0.0, cvtest::norm(expected_dst, dst, NORM_INF));
}
