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

#ifndef __OPENCV_IMGPROC_PRECOMP_H__
#define __OPENCV_IMGPROC_PRECOMP_H__

#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/core/private.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/core/hal/hal.hpp"
#include "opencv2/imgproc/hal/hal.hpp"
#include "hal_replacement.hpp"

#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>

#define GET_OPTIMIZED(func) (func)

/* helper tables */
extern const uchar icvSaturate8u_cv[];
#define CV_FAST_CAST_8U(t)  ( (-256 <= (t) && (t) <= 512) ? icvSaturate8u_cv[(t)+256] : 0 )
#define CV_CALC_MIN_8U(a,b) (a) -= CV_FAST_CAST_8U((a) - (b))
#define CV_CALC_MAX_8U(a,b) (a) += CV_FAST_CAST_8U((b) - (a))

// -256.f ... 511.f
extern const float icv8x32fTab_cv[];
#define CV_8TO32F(x)  icv8x32fTab_cv[(x)+256]

// (-128.f)^2 ... (255.f)^2
extern const float icv8x32fSqrTab[];
#define CV_8TO32F_SQR(x)  icv8x32fSqrTab[(x)+128]

#define  CV_COPY( dst, src, len, idx ) \
    for( (idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (src)[idx]

#define  CV_SET( dst, val, len, idx )  \
    for( (idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (val)

#undef   CV_CALC_MIN
#define  CV_CALC_MIN(a, b) if((a) > (b)) (a) = (b)

#undef   CV_CALC_MAX
#define  CV_CALC_MAX(a, b) if((a) < (b)) (a) = (b)

#ifdef HAVE_IPP
static inline IppiInterpolationType ippiGetInterpolation(int inter)
{
    inter &= cv::INTER_MAX;
    return inter == cv::INTER_NEAREST ? ippNearest :
        inter == cv::INTER_LINEAR ? ippLinear :
        inter == cv::INTER_CUBIC ? ippCubic :
        inter == cv::INTER_LANCZOS4 ? ippLanczos :
        inter == cv::INTER_AREA ? ippSuper :
        (IppiInterpolationType)-1;
}
#endif

#include "_geom.h"
#include "filterengine.hpp"

#include "opencv2/core/sse_utils.hpp"

inline bool isStorageOrMat(void * arr)
{
    if (CV_IS_STORAGE( arr ))
        return true;
    else if (CV_IS_MAT( arr ))
        return false;
    CV_Error( CV_StsBadArg, "Destination is not CvMemStorage* nor CvMat*" );
}
#include <cmath>
using namespace std;

namespace cv
{

  static inline int clip(int x, int a, int b)
  {
    return x >= a ? (x < b ? x : b - 1) : a;
  }

  const int INTER_RESIZE_COEF_BITS = 11;
  const int INTER_RESIZE_COEF_SCALE = 1 << INTER_RESIZE_COEF_BITS;

  static inline void interpolateLinear(float x, float* coeffs)
  {
    coeffs[0] = 1.f - x;
    coeffs[1] = x;
  }

  static inline void interpolateCubic(float x, float* coeffs)
  {
    const float A = -0.75f;

    coeffs[0] = ((A*(x + 1) - 5 * A)*(x + 1) + 8 * A)*(x + 1) - 4 * A;
    coeffs[1] = ((A + 2)*x - (A + 3))*x*x + 1;
    coeffs[2] = ((A + 2)*(1 - x) - (A + 3))*(1 - x)*(1 - x) + 1;
    coeffs[3] = 1.f - coeffs[0] - coeffs[1] - coeffs[2];
  }

  static inline void interpolateLanczos4(float x, float* coeffs)
  {
    static const double s45 = 0.70710678118654752440084436210485;
    static const double cs[][2] =
    { { 1, 0 },{ -s45, -s45 },{ 0, 1 },{ s45, -s45 },{ -1, 0 },{ s45, s45 },{ 0, -1 },{ -s45, s45 } };

    if (x < FLT_EPSILON)
    {
      for (int i = 0; i < 8; i++)
        coeffs[i] = 0;
      coeffs[3] = 1;
      return;
    }

    float sum = 0;
    double y0 = -(x + 3)*CV_PI*0.25, s0 = std::sin(y0), c0 = std::cos(y0);
    for (int i = 0; i < 8; i++)
    {
      double y = -(x + 3 - i)*CV_PI*0.25;
      coeffs[i] = (float)((cs[i][0] * s0 + cs[i][1] * c0) / (y*y));
      sum += coeffs[i];
    }

    sum = 1.f / sum;
    for (int i = 0; i < 8; i++)
      coeffs[i] *= sum;
  }

}

#endif /*__OPENCV_IMGPROC_PRECOMP_H__*/
