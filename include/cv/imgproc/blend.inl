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
// Copyright (C) 2010-2012, Multicoreware, Inc., all rights reserved.
// Copyright (C) 2010-2012, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// @Authors
//    Nathan, liujun@multicorewareinc.com
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


#include "opencl_kernels_imgproc.hpp"

namespace cv {

template <typename T>
class BlendLinearInvoker :
        public ParallelLoopBody
{
public:
    BlendLinearInvoker(const img_t & _src1, const img_t & _src2, const img_t & _weights1,
                       const img_t & _weights2, img_t & _dst) :
        src1(&_src1), src2(&_src2), weights1(&_weights1), weights2(&_weights2), dst(&_dst)
    {
    }

    virtual void operator() (const CRange & range) const
    {
        int cn = src1->CC_MAT_CN(), width = src1->cols * cn;

        for (int y = range.start; y < range.end; ++y)
        {
            const float * const weights1_row = weights1->ptr<float>(y);
            const float * const weights2_row = weights2->ptr<float>(y);
            const T * const src1_row = src1->ptr<T>(y);
            const T * const src2_row = src2->ptr<T>(y);
            T * const dst_row = dst->ptr<T>(y);

            for (int x = 0; x < width; ++x)
            {
                int x1 = x / cn;
                float w1 = weights1_row[x1], w2 = weights2_row[x1];
                float den = (w1 + w2 + 1e-5f);
                float num = (src1_row[x] * w1 + src2_row[x] * w2);

                dst_row[x] = saturate_cast<T>(num / den);
            }
        }
    }

private:
    const BlendLinearInvoker & operator= (const BlendLinearInvoker &);
    BlendLinearInvoker(const BlendLinearInvoker &);

    const img_t * src1, * src2, * weights1, * weights2;
    img_t * dst;
};

#ifdef HAVE_OPENCL

static bool ocl_blendLinear( const img_t* _src1, const img_t* _src2, const img_t* _weights1, const img_t* _weights2, img_t* dst )
{
    int type = _src1->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);

    char cvt[30];
    ocl::Kernel k("blendLinear", ocl::imgproc::blend_linear_oclsrc,
                  format("-D T=%s -D cn=%d -D convertToT=%s", ocl::typeToStr(depth),
                         cn, ocl::convertTypeStr(CC_32F, depth, 1, cvt)));
    if (k.empty())
        return false;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), weights1 = _weights1.getUMat(),
            weights2 = _weights2.getUMat(), dst = _dst.getUMat();

    k.args(ocl::KernelArg::ReadOnlyNoSize(src1), ocl::KernelArg::ReadOnlyNoSize(src2),
           ocl::KernelArg::ReadOnlyNoSize(weights1), ocl::KernelArg::ReadOnlyNoSize(weights2),
           ocl::KernelArg::WriteOnly(dst));

    size_t globalsize[2] = { (size_t)dst->cols, (size_t)dst->rows };
    return k.run(2, globalsize, NULL, false);
}

#endif

}

void blendLinear( const img_t* _src1, const img_t* _src2, const img_t* _weights1, const img_t* _weights2, img_t* dst )
{
    CC_INSTRUMENT_REGION()

    int type = _src1->tid, depth = CC_MAT_DEPTH(type);
    CSize size = _src1.size();

    CC_Assert(depth == CC_8U || depth == CC_32F);
    CC_Assert(size == _src2.size() && size == _weights1.size() && size == _weights2.size());
    CC_Assert(type == _src2->tid && _weights1->tid == CC_32FC1 && _weights2->tid == CC_32FC1);

    _dst cvSetMat(size, type);

    CC_OCL_RUN(_dst.isUMat(),
               ocl_blendLinear(_src1, _src2, _weights1, _weights2, _dst))

    img_t src1 = _src1, src2 = _src2, weights1 = _weights1,
            weights2 = _weights2, dst = _dst;

    if (depth == CC_8U)
    {
        BlendLinearInvoker<uchar> invoker(src1, src2, weights1, weights2, dst);
        parallel_for_(CRange(0, src1->rows), invoker, dst CC_MAT_TOTAL()/(double)(1<<16));
    }
    else if (depth == CC_32F)
    {
        BlendLinearInvoker<float> invoker(src1, src2, weights1, weights2, dst);
        parallel_for_(CRange(0, src1->rows), invoker, dst CC_MAT_TOTAL()/(double)(1<<16));
    }
}
