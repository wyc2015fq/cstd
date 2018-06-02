/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
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
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#ifndef __OPENCC_PRECOMP_H__
#define __OPENCC_PRECOMP_H__

#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/core/utility.hpp"

#include "opencv2/core/private.hpp"

#include "opencv2/core/ocl.hpp"

#ifdef HAVE_TEGRA_OPTIMIZATION
#include "opencv2/calib3d/calib3d_tegra.hpp"
#else
#define GET_OPTIMIZED(func) (func)
#endif


namespace cv
{

int RANSACUpdateNumIters( double p, double ep, int modelPoints, int maxIters );

class CC_EXPORTS LMSolver : public Algorithm
{
public:
    class CC_EXPORTS Callback
    {
    public:
        virtual ~Callback() {}
        virtual bool compute(const img_t* param, img_t* err, img_t* J) const = 0;
    };

    virtual void setCallback(const Ptr<LMSolver::Callback>& cb) = 0;
    virtual int run(InputCvArr* param0) const = 0;
};

CC_EXPORTS Ptr<LMSolver> createLMSolver(const Ptr<LMSolver::Callback>& cb, int maxIters);

class CC_EXPORTS PointSetRegistrator : public Algorithm
{
public:
    class CC_EXPORTS Callback
    {
    public:
        virtual ~Callback() {}
        virtual int runKernel(const img_t* m1, const img_t* m2, img_t* model) const = 0;
        virtual void computeError(const img_t* m1, const img_t* m2, const img_t* model, img_t* err) const = 0;
        virtual bool checkSubset(const img_t*, const img_t*, int) const { return true; }
    };

    virtual void setCallback(const Ptr<PointSetRegistrator::Callback>& cb) = 0;
    virtual bool run(const img_t* m1, const img_t* m2, img_t* model, img_t* mask) const = 0;
};

CC_EXPORTS Ptr<PointSetRegistrator> createRANSACPointSetRegistrator(const Ptr<PointSetRegistrator::Callback>& cb,
                                                                    int modelPoints, double threshold,
                                                                    double confidence=0.99, int maxIters=1000 );

CC_EXPORTS Ptr<PointSetRegistrator> createLMeDSPointSetRegistrator(const Ptr<PointSetRegistrator::Callback>& cb,
                                                                   int modelPoints, double confidence=0.99, int maxIters=1000 );

template<typename T> inline int compressElems( T* ptr, const uchar* mask, int mstep, int count )
{
    int i, j;
    for( i = j = 0; i < count; i++ )
        if( mask[i*mstep] )
        {
            if( i > j )
                ptr[j] = ptr[i];
            j++;
        }
    return j;
}

static inline bool haveCollinearPoints( const img_t& m, int count )
{
    int j, k, i = count-1;
    const CPoint2f* ptr = m(CPoint2f*)(->tt.data);

    // check that the i-th selected point does not belong
    // to a line connecting some previously selected points
    // also checks that points are not too close to each other
    for( j = 0; j < i; j++ )
    {
        double dx1 = ptr[j].x - ptr[i].x;
        double dy1 = ptr[j].y - ptr[i].y;
        for( k = 0; k < j; k++ )
        {
            double dx2 = ptr[k].x - ptr[i].x;
            double dy2 = ptr[k].y - ptr[i].y;
            if( fabs(dx2*dy1 - dy2*dx1) <= FLT_EPSILON*(fabs(dx1) + fabs(dy1) + fabs(dx2) + fabs(dy2)))
                return true;
        }
    }
    return false;
}

} // namespace cv

int checkChessboard(const img_t & img, const CSize & size);
int checkChessboardBinary(const img_t & img, const CSize & size);

#endif
