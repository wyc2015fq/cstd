/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
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
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the OpenCV Foundation or contributors be liable for any direct,
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

enum
{
    HAL_EXP = 0,
    HAL_LOG = 1,
    HAL_SQRT = 2
};

TEST(Core_HAL, mathfuncs)
{
    for( int hcase = 0; hcase < 6; hcase++ )
    {
        int depth = hcase % 2 == 0 ? CC_32F : CC_64F;
        double eps = depth == CC_32F ? 1e-5 : 1e-10;
        int nfunc = hcase / 2;
        int n = 100;

        img_t src(1, n, depth), dst(1, n, depth), dst0(1, n, depth);
        randu(src, 1, 10);

        double min_hal_t = DBL_MAX, min_ocv_t = DBL_MAX;

        for( int iter = 0; iter < 10; iter++ )
        {
            double t = (double)cGetTickCount();
            switch (nfunc)
            {
            case HAL_EXP:
                if( depth == CC_32F )
                    hal_exp32f(src img_ptr(float, ), dst img_ptr(float, ), n);
                else
                    hal_exp64f(src img_ptr(double, ), dst img_ptr(double, ), n);
                break;
            case HAL_LOG:
                if( depth == CC_32F )
                    hal_log32f(src img_ptr(float, ), dst img_ptr(float, ), n);
                else
                    hal_log64f(src img_ptr(double, ), dst img_ptr(double, ), n);
                break;
            case HAL_SQRT:
                if( depth == CC_32F )
                    hal_sqrt32f(src img_ptr(float, ), dst img_ptr(float, ), n);
                else
                    hal_sqrt64f(src img_ptr(double, ), dst img_ptr(double, ), n);
                break;
            default:
                CC_Error(CC_StsBadArg, "unknown function");
            }
            t = (double)cGetTickCount() - t;
            min_hal_t = MIN(min_hal_t, t);

            t = (double)cGetTickCount();
            switch (nfunc)
            {
            case HAL_EXP:
                exp(src, dst0);
                break;
            case HAL_LOG:
                log(src, dst0);
                break;
            case HAL_SQRT:
                pow(src, 0.5, dst0);
                break;
            default:
                CC_Error(CC_StsBadArg, "unknown function");
            }
            t = (double)cGetTickCount() - t;
            min_ocv_t = MIN(min_ocv_t, t);
        }
        EXPECT_LE(norm(dst, dst0, NORM_INF | NORM_RELATIVE), eps);

        double freq = getTickFrequency();
        printf("%s (N=%d, %s): hal time=%.2fusec, ocv time=%.2fusec\n",
               (nfunc == HAL_EXP ? "exp" : nfunc == HAL_LOG ? "log" : nfunc == HAL_SQRT ? "sqrt" : "???"),
               n, (depth == CC_32F ? "f32" : "f64"), min_hal_t*1e6/freq, min_ocv_t*1e6/freq);
    }
}

namespace {

enum
{
    HAL_LU = 0,
    HAL_CHOL = 1
};

typedef testing::TestWithParam<int> HAL;

TEST_P(HAL, mat_decomp)
{
    int hcase = GetParam();
    SCOPED_TRACE(format("hcase=%d", hcase));
    {
        int depth = hcase % 2 == 0 ? CC_32F : CC_64F;
        int size = (hcase / 2) % 4;
        size = size == 0 ? 3 : size == 1 ? 4  : size == 2 ? 6 : 15;
        int nfunc = (hcase / 8);
        double eps = depth == CC_32F ? 1e-5 : 1e-10;

        if( size == 3 )
            return; // TODO ???

        img_t a0(size, size, depth), a(size, size, depth), b(size, 1, depth), x(size, 1, depth), x0(size, 1, depth);
        randu(a0, -1, 1);
        a0 = a0*a0.t();
        randu(b, -1, 1);

        double min_hal_t = DBL_MAX, min_ocv_t = DBL_MAX;
        size_t asize = size*size*a CC_ELEM_SIZE();
        size_t bsize = size*b CC_ELEM_SIZE();

        for( int iter = 0; iter < 10; iter++ )
        {
            memcpy(x->tt.data, b->tt.data, bsize);
            memcpy(a->tt.data, a0->tt.data, asize);

            double t = (double)cGetTickCount();
            switch (nfunc)
            {
            case HAL_LU:
                if( depth == CC_32F )
                    hal_LU32f(a img_ptr(float, ), a->step, size, x img_ptr(float, ), x->step, 1);
                else
                    hal_LU64f(a img_ptr(double, ), a->step, size, x img_ptr(double, ), x->step, 1);
                break;
            case HAL_CHOL:
                if( depth == CC_32F )
                    hal_Cholesky32f(a img_ptr(float, ), a->step, size, x img_ptr(float, ), x->step, 1);
                else
                    hal_Cholesky64f(a img_ptr(double, ), a->step, size, x img_ptr(double, ), x->step, 1);
                break;
            default:
                CC_Error(CC_StsBadArg, "unknown function");
            }
            t = (double)cGetTickCount() - t;
            min_hal_t = MIN(min_hal_t, t);

            t = (double)cGetTickCount();
            bool solveStatus = solve(a0, b, x0, (nfunc == HAL_LU ? DECOMP_LU : DECOMP_CHOLESKY));
            t = (double)cGetTickCount() - t;
            EXPECT_TRUE(solveStatus);
            min_ocv_t = MIN(min_ocv_t, t);
        }
        //std::cout << "x: " << img_t(x.t()) << std::endl;
        //std::cout << "x0: " << img_t(x0.t()) << std::endl;

        EXPECT_LE(norm(x, x0, NORM_INF | NORM_RELATIVE), eps)
            << "x:  " << img_t(x.t())
            << "\nx0: " << img_t(x0.t())
            << "\na0: " << a0
            << "\nb: " << b;

        double freq = getTickFrequency();
        printf("%s (%d x %d, %s): hal time=%.2fusec, ocv time=%.2fusec\n",
               (nfunc == HAL_LU ? "LU" : nfunc == HAL_CHOL ? "Cholesky" : "???"),
               size, size,
               (depth == CC_32F ? "f32" : "f64"),
               min_hal_t*1e6/freq, min_ocv_t*1e6/freq);
    }
}

INSTANTIATE_TEST_CASE_P(Core, HAL, testing::CRange(0, 16));

} // namespace
