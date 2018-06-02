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
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2014-2015, Itseez Inc., all rights reserved.
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


#include "opencl_kernels_imgproc.hpp"

namespace cv
{

static void calcMinEigenVal( const img_t& _cov, img_t& _dst )
{
    int i, j;
    CSize size = _cov.size();
#if CC_SSE
    volatile bool simd = checkHardwareSupport(CC_CPU_SSE);
#endif

    if( _cov CC_IS_CONT_MAT() && _dst CC_IS_CONT_MAT() )
    {
        size.width *= size.height;
        size.height = 1;
    }

    for( i = 0; i < size.height; i++ )
    {
        const float* cov = _cov.ptr<float>(i);
        float* dst = _dst.ptr<float>(i);
        j = 0;
    #if CC_SSE
        if( simd )
        {
            __m128 half = _mm_set1_ps(0.5f);
            for( ; j <= size.width - 4; j += 4 )
            {
                __m128 t0 = _mm_loadu_ps(cov + j*3); // a0 b0 c0 x
                __m128 t1 = _mm_loadu_ps(cov + j*3 + 3); // a1 b1 c1 x
                __m128 t2 = _mm_loadu_ps(cov + j*3 + 6); // a2 b2 c2 x
                __m128 t3 = _mm_loadu_ps(cov + j*3 + 9); // a3 b3 c3 x
                __m128 a, b, c, t;
                t = _mm_unpacklo_ps(t0, t1); // a0 a1 b0 b1
                c = _mm_unpackhi_ps(t0, t1); // c0 c1 x x
                b = _mm_unpacklo_ps(t2, t3); // a2 a3 b2 b3
                c = _mm_movelh_ps(c, _mm_unpackhi_ps(t2, t3)); // c0 c1 c2 c3
                a = _mm_movelh_ps(t, b);
                b = _mm_movehl_ps(b, t);
                a = _mm_mul_ps(a, half);
                c = _mm_mul_ps(c, half);
                t = _mm_sub_ps(a, c);
                t = _mm_add_ps(_mm_mul_ps(t, t), _mm_mul_ps(b,b));
                a = _mm_sub_ps(_mm_add_ps(a, c), _mm_sqrt_ps(t));
                _mm_storeu_ps(dst + j, a);
            }
        }
    #elif CC_NEON
        float32x4_t v_half = vdupq_n_f32(0.5f);
        for( ; j <= size.width - 4; j += 4 )
        {
            float32x4x3_t v_src = vld3q_f32(cov + j * 3);
            float32x4_t v_a = vmulq_f32(v_src.val[0], v_half);
            float32x4_t v_b = v_src.val[1];
            float32x4_t v_c = vmulq_f32(v_src.val[2], v_half);

            float32x4_t v_t = vsubq_f32(v_a, v_c);
            v_t = vmlaq_f32(vmulq_f32(v_t, v_t), v_b, v_b);
            vst1q_f32(dst + j, vsubq_f32(vaddq_f32(v_a, v_c), cv_vsqrtq_f32(v_t)));
        }
    #endif
        for( ; j < size.width; j++ )
        {
            float a = cov[j*3]*0.5f;
            float b = cov[j*3+1];
            float c = cov[j*3+2]*0.5f;
            dst[j] = (float)((a + c) - sqrt((a - c)*(a - c) + b*b));
        }
    }
}


static void calcHarris( const img_t& _cov, img_t& _dst, double k )
{
    int i, j;
    CSize size = _cov.size();
#if CC_SSE
    volatile bool simd = checkHardwareSupport(CC_CPU_SSE);
#endif

    if( _cov CC_IS_CONT_MAT() && _dst CC_IS_CONT_MAT() )
    {
        size.width *= size.height;
        size.height = 1;
    }

    for( i = 0; i < size.height; i++ )
    {
        const float* cov = _cov.ptr<float>(i);
        float* dst = _dst.ptr<float>(i);
        j = 0;

    #if CC_SSE
        if( simd )
        {
            __m128 k4 = _mm_set1_ps((float)k);
            for( ; j <= size.width - 4; j += 4 )
            {
                __m128 t0 = _mm_loadu_ps(cov + j*3); // a0 b0 c0 x
                __m128 t1 = _mm_loadu_ps(cov + j*3 + 3); // a1 b1 c1 x
                __m128 t2 = _mm_loadu_ps(cov + j*3 + 6); // a2 b2 c2 x
                __m128 t3 = _mm_loadu_ps(cov + j*3 + 9); // a3 b3 c3 x
                __m128 a, b, c, t;
                t = _mm_unpacklo_ps(t0, t1); // a0 a1 b0 b1
                c = _mm_unpackhi_ps(t0, t1); // c0 c1 x x
                b = _mm_unpacklo_ps(t2, t3); // a2 a3 b2 b3
                c = _mm_movelh_ps(c, _mm_unpackhi_ps(t2, t3)); // c0 c1 c2 c3
                a = _mm_movelh_ps(t, b);
                b = _mm_movehl_ps(b, t);
                t = _mm_add_ps(a, c);
                a = _mm_sub_ps(_mm_mul_ps(a, c), _mm_mul_ps(b, b));
                t = _mm_mul_ps(_mm_mul_ps(k4, t), t);
                a = _mm_sub_ps(a, t);
                _mm_storeu_ps(dst + j, a);
            }
        }
    #elif CC_NEON
        float32x4_t v_k = vdupq_n_f32((float)k);

        for( ; j <= size.width - 4; j += 4 )
        {
            float32x4x3_t v_src = vld3q_f32(cov + j * 3);
            float32x4_t v_a = v_src.val[0], v_b = v_src.val[1], v_c = v_src.val[2];
            float32x4_t v_ac_bb = vmlsq_f32(vmulq_f32(v_a, v_c), v_b, v_b);
            float32x4_t v_ac = vaddq_f32(v_a, v_c);
            vst1q_f32(dst + j, vmlsq_f32(v_ac_bb, v_k, vmulq_f32(v_ac, v_ac)));
        }
    #endif

        for( ; j < size.width; j++ )
        {
            float a = cov[j*3];
            float b = cov[j*3+1];
            float c = cov[j*3+2];
            dst[j] = (float)(a*c - b*b - k*(a + c)*(a + c));
        }
    }
}


static void eigen2x2( const float* cov, float* dst, int n )
{
    for( int j = 0; j < n; j++ )
    {
        double a = cov[j*3];
        double b = cov[j*3+1];
        double c = cov[j*3+2];

        double u = (a + c)*0.5;
        double v = sqrt((a - c)*(a - c)*0.25 + b*b);
        double l1 = u + v;
        double l2 = u - v;

        double x = b;
        double y = l1 - a;
        double e = fabs(x);

        if( e + fabs(y) < 1e-4 )
        {
            y = b;
            x = l1 - c;
            e = fabs(x);
            if( e + fabs(y) < 1e-4 )
            {
                e = 1./(e + fabs(y) + FLT_EPSILON);
                x *= e, y *= e;
            }
        }

        double d = 1./sqrt(x*x + y*y + DBL_EPSILON);
        dst[6*j] = (float)l1;
        dst[6*j + 2] = (float)(x*d);
        dst[6*j + 3] = (float)(y*d);

        x = b;
        y = l2 - a;
        e = fabs(x);

        if( e + fabs(y) < 1e-4 )
        {
            y = b;
            x = l2 - c;
            e = fabs(x);
            if( e + fabs(y) < 1e-4 )
            {
                e = 1./(e + fabs(y) + FLT_EPSILON);
                x *= e, y *= e;
            }
        }

        d = 1./sqrt(x*x + y*y + DBL_EPSILON);
        dst[6*j + 1] = (float)l2;
        dst[6*j + 4] = (float)(x*d);
        dst[6*j + 5] = (float)(y*d);
    }
}

static void calcEigenValsVecs( const img_t& _cov, img_t& _dst )
{
    CSize size = _cov.size();
    if( _cov CC_IS_CONT_MAT() && _dst CC_IS_CONT_MAT() )
    {
        size.width *= size.height;
        size.height = 1;
    }

    for( int i = 0; i < size.height; i++ )
    {
        const float* cov = _cov.ptr<float>(i);
        float* dst = _dst.ptr<float>(i);

        eigen2x2(cov, dst, size.width);
    }
}


enum { MINEIGENVAL=0, HARRIS=1, EIGENVALSVECS=2 };


static void
cornerEigenValsVecs( const img_t& src, img_t& eigenv, int block_size,
                     int aperture_size, int op_type, double k=0.,
                     int borderType=CC_BORDER_DEFAULT )
{
#ifdef HAVE_TEGRA_OPTIMIZATION
    if (tegra::useTegra() && tegra::cornerEigenValsVecs(src, eigenv, block_size, aperture_size, op_type, k, borderType))
        return;
#endif
#if CC_SSE2
    bool haveSSE2 = checkHardwareSupport(CC_CPU_SSE2);
#endif

    int depth = src CC_MAT_DEPTH();
    double scale = (double)(1 << ((aperture_size > 0 ? aperture_size : 3) - 1)) * block_size;
    if( aperture_size < 0 )
        scale *= 2.0;
    if( depth == CC_8U )
        scale *= 255.0;
    scale = 1.0/scale;

    CC_Assert( src->tid == CC_8UC1 || src->tid == CC_32FC1 );

    img_t Dx, Dy;
    if( aperture_size > 0 )
    {
        Sobel( src, Dx, CC_32F, 1, 0, aperture_size, scale, 0, borderType );
        Sobel( src, Dy, CC_32F, 0, 1, aperture_size, scale, 0, borderType );
    }
    else
    {
        Scharr( src, Dx, CC_32F, 1, 0, scale, 0, borderType );
        Scharr( src, Dy, CC_32F, 0, 1, scale, 0, borderType );
    }

    CSize size = cvGetSize(src);
    img_t cov( size, CC_32FC3 );
    int i, j;

    for( i = 0; i < size.height; i++ )
    {
        float* cov_data = cov.ptr<float>(i);
        const float* dxdata = Dx.ptr<float>(i);
        const float* dydata = Dy.ptr<float>(i);
        j = 0;

        #if CC_NEON
        for( ; j <= size.width - 4; j += 4 )
        {
            float32x4_t v_dx = vld1q_f32(dxdata + j);
            float32x4_t v_dy = vld1q_f32(dydata + j);

            float32x4x3_t v_dst;
            v_dst.val[0] = vmulq_f32(v_dx, v_dx);
            v_dst.val[1] = vmulq_f32(v_dx, v_dy);
            v_dst.val[2] = vmulq_f32(v_dy, v_dy);

            vst3q_f32(cov_data + j * 3, v_dst);
        }
        #elif CC_SSE2
        if (haveSSE2)
        {
            for( ; j <= size.width - 8; j += 8 )
            {
                __m128 v_dx_0 = _mm_loadu_ps(dxdata + j);
                __m128 v_dx_1 = _mm_loadu_ps(dxdata + j + 4);
                __m128 v_dy_0 = _mm_loadu_ps(dydata + j);
                __m128 v_dy_1 = _mm_loadu_ps(dydata + j + 4);

                __m128 v_dx2_0 = _mm_mul_ps(v_dx_0, v_dx_0);
                __m128 v_dxy_0 = _mm_mul_ps(v_dx_0, v_dy_0);
                __m128 v_dy2_0 = _mm_mul_ps(v_dy_0, v_dy_0);
                __m128 v_dx2_1 = _mm_mul_ps(v_dx_1, v_dx_1);
                __m128 v_dxy_1 = _mm_mul_ps(v_dx_1, v_dy_1);
                __m128 v_dy2_1 = _mm_mul_ps(v_dy_1, v_dy_1);

                _mm_interleave_ps(v_dx2_0, v_dx2_1, v_dxy_0, v_dxy_1, v_dy2_0, v_dy2_1);

                _mm_storeu_ps(cov_data + j * 3, v_dx2_0);
                _mm_storeu_ps(cov_data + j * 3 + 4, v_dx2_1);
                _mm_storeu_ps(cov_data + j * 3 + 8, v_dxy_0);
                _mm_storeu_ps(cov_data + j * 3 + 12, v_dxy_1);
                _mm_storeu_ps(cov_data + j * 3 + 16, v_dy2_0);
                _mm_storeu_ps(cov_data + j * 3 + 20, v_dy2_1);
            }
        }
        #endif

        for( ; j < size.width; j++ )
        {
            float dx = dxdata[j];
            float dy = dydata[j];

            cov_data[j*3] = dx*dx;
            cov_data[j*3+1] = dx*dy;
            cov_data[j*3+2] = dy*dy;
        }
    }

    boxFilter(cov, cov, cov CC_MAT_DEPTH(), CSize(block_size, block_size),
        Point(-1,-1), false, borderType );

    if( op_type == MINEIGENVAL )
        calcMinEigenVal( cov, eigenv );
    else if( op_type == HARRIS )
        calcHarris( cov, eigenv, k );
    else if( op_type == EIGENVALSVECS )
        calcEigenValsVecs( cov, eigenv );
}

#ifdef HAVE_OPENCL

static bool extractCovData(const img_t* _src, UMat & Dx, UMat & Dy, int depth,
                           float scale, int aperture_size, int borderType)
{
    UMat src = _src.getUMat();

    CSize wholeSize;
    Point ofs;
    src.locateROI(wholeSize, ofs);

    const int sobel_lsz = 16;
    if ((aperture_size == 3 || aperture_size == 5 || aperture_size == 7 || aperture_size == -1) &&
        wholeSize.height > sobel_lsz + (aperture_size >> 1) &&
        wholeSize.width > sobel_lsz + (aperture_size >> 1))
    {
        CC_Assert(depth == CC_8U || depth == CC_32F);

        Dx cvSetMat(cvGetSize(src), CC_32FC1);
        Dy cvSetMat(cvGetSize(src), CC_32FC1);

        size_t localsize[2] = { (size_t)sobel_lsz, (size_t)sobel_lsz };
        size_t globalsize[2] = { localsize[0] * (1 + (src->cols - 1) / localsize[0]),
                                 localsize[1] * (1 + (src->rows - 1) / localsize[1]) };

        int src_offset_x = (int)((src.offset % src->step) / src CC_ELEM_SIZE());
        int src_offset_y = (int)(src.offset / src->step);

        const char * const borderTypes[] = { "CC_BORDER_CONSTANT", "CC_BORDER_REPLICATE", "CC_BORDER_REFLECT",
                                             "CC_BORDER_WRAP", "CC_BORDER_REFLECT101" };

        ocl::Kernel k(format("sobel%d", aperture_size).c_str(), ocl::imgproc::covardata_oclsrc,
                      format("-D BLK_X=%d -D BLK_Y=%d -D %s -D SRCTYPE=%s%s",
                                 (int)localsize[0], (int)localsize[1], borderTypes[borderType], ocl::typeToStr(depth),
                                 aperture_size < 0 ? " -D SCHARR" : ""));
        if (k.empty())
            return false;

        k.args(ocl::KernelArg::PtrReadOnly(src), (int)src->step, src_offset_x, src_offset_y,
               ocl::KernelArg::WriteOnlyNoSize(Dx), ocl::KernelArg::WriteOnly(Dy),
               wholeSize.height, wholeSize.width, scale);

        return k.run(2, globalsize, localsize, false);
    }
    else
    {
        if (aperture_size > 0)
        {
            Sobel(_src, Dx, CC_32F, 1, 0, aperture_size, scale, 0, borderType);
            Sobel(_src, Dy, CC_32F, 0, 1, aperture_size, scale, 0, borderType);
        }
        else
        {
            Scharr(_src, Dx, CC_32F, 1, 0, scale, 0, borderType);
            Scharr(_src, Dy, CC_32F, 0, 1, scale, 0, borderType);
        }
    }

    return true;
}

static bool ocl_cornerMinEigenValVecs(const img_t* _src, img_t* dst, int block_size,
                                      int aperture_size, double k, int borderType, int op_type)
{
    CC_Assert(op_type == HARRIS || op_type == MINEIGENVAL);

    if ( !(borderType == CC_BORDER_CONSTANT || borderType == CC_BORDER_REPLICATE ||
           borderType == CC_BORDER_REFLECT || borderType == CC_BORDER_REFLECT_101) )
        return false;

    int type = _src->tid, depth = CC_MAT_DEPTH(type);
    if ( !(type == CC_8UC1 || type == CC_32FC1) )
        return false;

    const char * const borderTypes[] = { "CC_BORDER_CONSTANT", "CC_BORDER_REPLICATE", "CC_BORDER_REFLECT",
                                         "CC_BORDER_WRAP", "CC_BORDER_REFLECT101" };
    const char * const cornerType[] = { "CORNER_MINEIGENVAL", "CORNER_HARRIS", 0 };


    double scale = (double)(1 << ((aperture_size > 0 ? aperture_size : 3) - 1)) * block_size;
    if (aperture_size < 0)
        scale *= 2.0;
    if (depth == CC_8U)
        scale *= 255.0;
    scale = 1.0 / scale;

    UMat Dx, Dy;
    if (!extractCovData(_src, Dx, Dy, depth, (float)scale, aperture_size, borderType))
        return false;

    ocl::Kernel cornelKernel("corner", ocl::imgproc::corner_oclsrc,
                             format("-D anX=%d -D anY=%d -D ksX=%d -D ksY=%d -D %s -D %s",
                                    block_size / 2, block_size / 2, block_size, block_size,
                                    borderTypes[borderType], cornerType[op_type]));
    if (cornelKernel.empty())
        return false;

    _dst.createSameSize(_src, CC_32FC1);
    UMat dst = _dst.getUMat();

    cornelKernel.args(ocl::KernelArg::ReadOnly(Dx), ocl::KernelArg::ReadOnly(Dy),
                      ocl::KernelArg::WriteOnly(dst), (float)k);

    size_t blockSizeX = 256, blockSizeY = 1;
    size_t gSize = blockSizeX - block_size / 2 * 2;
    size_t globalSizeX = (Dx->cols) % gSize == 0 ? Dx->cols / gSize * blockSizeX : (Dx->cols / gSize + 1) * blockSizeX;
    size_t rows_per_thread = 2;
    size_t globalSizeY = ((Dx->rows + rows_per_thread - 1) / rows_per_thread) % blockSizeY == 0 ?
                         ((Dx->rows + rows_per_thread - 1) / rows_per_thread) :
                         (((Dx->rows + rows_per_thread - 1) / rows_per_thread) / blockSizeY + 1) * blockSizeY;

    size_t globalsize[2] = { globalSizeX, globalSizeY }, localsize[2] = { blockSizeX, blockSizeY };
    return cornelKernel.run(2, globalsize, localsize, false);
}

static bool ocl_preCornerDetect( const img_t* _src, img_t* dst, int ksize, int borderType, int depth )
{
    UMat Dx, Dy, D2x, D2y, Dxy;

    if (!extractCovData(_src, Dx, Dy, depth, 1, ksize, borderType))
        return false;

    Sobel( _src, D2x, CC_32F, 2, 0, ksize, 1, 0, borderType );
    Sobel( _src, D2y, CC_32F, 0, 2, ksize, 1, 0, borderType );
    Sobel( _src, Dxy, CC_32F, 1, 1, ksize, 1, 0, borderType );

    _dst cvSetMat( _cvGetSize(src), CC_32FC1 );
    UMat dst = _dst.getUMat();

    double factor = 1 << (ksize - 1);
    if( depth == CC_8U )
        factor *= 255;
    factor = 1./(factor * factor * factor);

    ocl::Kernel k("preCornerDetect", ocl::imgproc::precornerdetect_oclsrc);
    if (k.empty())
        return false;

    k.args(ocl::KernelArg::ReadOnlyNoSize(Dx), ocl::KernelArg::ReadOnlyNoSize(Dy),
           ocl::KernelArg::ReadOnlyNoSize(D2x), ocl::KernelArg::ReadOnlyNoSize(D2y),
           ocl::KernelArg::ReadOnlyNoSize(Dxy), ocl::KernelArg::WriteOnly(dst), (float)factor);

    size_t globalsize[2] = { (size_t)dst->cols, (size_t)dst->rows };
    return k.run(2, globalsize, NULL, false);
}

#endif

}

#if defined(HAVE_IPP)
namespace cv
{
static bool ipp_cornerMinEigenVal( const img_t* _src, img_t* dst, int blockSize, int ksize, int borderType )
{
    CC_INSTRUMENT_REGION_IPP()

#if IPP_VERSION_X100 >= 800
    img_t *src = _src;
    _dst cvSetMat( cvGetSize(src), CC_32FC1 );
    img_t *dst = _dst;

    {
        typedef IppStatus (C_STDCALL * ippiMinEigenValGetBufferSize)(IppiSize, int, int, int*);
        typedef IppStatus (C_STDCALL * ippiMinEigenVal)(const void*, int, Ipp32f*, int, IppiSize, IppiKernelType, int, int, Ipp8u*);
        IppiKernelType kerType;
        int kerSize = ksize;
        if (ksize < 0)
        {
            kerType = ippKernelScharr;
            kerSize = 3;
        } else
        {
            kerType = ippKernelSobel;
        }
        bool isolated = (borderType & CC_BORDER_ISOLATED) != 0;
        int borderTypeNI = borderType & ~CC_BORDER_ISOLATED;
        if ((borderTypeNI == CC_BORDER_REPLICATE && (!src.isSubmatrix() || isolated)) &&
            (kerSize == 3 || kerSize == 5) && (blockSize == 3 || blockSize == 5))
        {
            ippiMinEigenValGetBufferSize getBufferSizeFunc = 0;
            ippiMinEigenVal ippiMinEigenVal_C1R = 0;
            float norm_coef = 0.f;

            if (src->tid == CC_8UC1)
            {
                getBufferSizeFunc = (ippiMinEigenValGetBufferSize) ippiMinEigenValGetBufferSize_8u32f_C1R;
                ippiMinEigenVal_C1R = (ippiMinEigenVal) ippiMinEigenVal_8u32f_C1R;
                norm_coef = 1.f / 255.f;
            } else if (src->tid == CC_32FC1)
            {
                getBufferSizeFunc = (ippiMinEigenValGetBufferSize) ippiMinEigenValGetBufferSize_32f_C1R;
                ippiMinEigenVal_C1R = (ippiMinEigenVal) ippiMinEigenVal_32f_C1R;
                norm_coef = 255.f;
            }
            norm_coef = kerType == ippKernelSobel ? norm_coef : norm_coef / 2.45f;

            if (getBufferSizeFunc && ippiMinEigenVal_C1R)
            {
                int bufferSize;
                IppiSize srcRoi = { src->cols, src->rows };
                IppStatus ok = getBufferSizeFunc(srcRoi, kerSize, blockSize, &bufferSize);
                if (ok >= 0)
                {
                    CAutoBuffer<uchar> buffer(bufferSize);
                    ok = CC_INSTRUMENT_FUN_IPP(ippiMinEigenVal_C1R, src->tt.data, (int) src->step, dst.ptr<Ipp32f>(), (int) dst->step, srcRoi, kerType, kerSize, blockSize, buffer);
                    CC_SUPPRESS_DEPRECATED_START
                    if (ok >= 0) ok = CC_INSTRUMENT_FUN_IPP(ippiMulC_32f_C1IR, norm_coef, dst.ptr<Ipp32f>(), (int) dst->step, srcRoi);
                    CC_SUPPRESS_DEPRECATED_END
                    if (ok >= 0)
                    {
                        CC_IMPL_ADD(CC_IMPL_IPP);
                        return true;
                    }
                }
            }
        }
    }
#else
    CC_UNUSED(_src); CC_UNUSED(_dst); CC_UNUSED(blockSize); CC_UNUSED(borderType);
#endif
    return false;
}
}
#endif

void cornerMinEigenVal( const img_t* _src, img_t* dst, int blockSize, int ksize, int borderType )
{
    CC_INSTRUMENT_REGION()

    CC_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_cornerMinEigenValVecs(_src, _dst, blockSize, ksize, 0.0, borderType, MINEIGENVAL))

#ifdef HAVE_IPP
    int kerSize = (ksize < 0)?3:ksize;
    bool isolated = (borderType & CC_BORDER_ISOLATED) != 0;
    int borderTypeNI = borderType & ~CC_BORDER_ISOLATED;
#endif
    CC_IPP_RUN(((borderTypeNI == CC_BORDER_REPLICATE && (!_src.isSubmatrix() || isolated)) &&
            (kerSize == 3 || kerSize == 5) && (blockSize == 3 || blockSize == 5)) && IPP_VERSION_X100 >= 800,
        ipp_cornerMinEigenVal( _src, _dst, blockSize, ksize, borderType ));


    img_t *src = _src;
    _dst cvSetMat( cvGetSize(src), CC_32FC1 );
    img_t *dst = _dst;

    cornerEigenValsVecs( src, dst, blockSize, ksize, MINEIGENVAL, 0, borderType );
}


#if defined(HAVE_IPP)
namespace cv
{
static bool ipp_cornerHarris( const img_t* _src, img_t* dst, int blockSize, int ksize, double k, int borderType )
{
    CC_INSTRUMENT_REGION_IPP()

#if IPP_VERSION_X100 >= 810 && IPP_DISABLE_BLOCK
    img_t *src = _src;
    _dst cvSetMat( cvGetSize(src), CC_32FC1 );
    img_t *dst = _dst;

    {
        int type = src->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
        int borderTypeNI = borderType & ~CC_BORDER_ISOLATED;
        bool isolated = (borderType & CC_BORDER_ISOLATED) != 0;

        if ( (ksize == 3 || ksize == 5) && (type == CC_8UC1 || type == CC_32FC1) &&
            (borderTypeNI == CC_BORDER_CONSTANT || borderTypeNI == CC_BORDER_REPLICATE) && cn == 1 && (!src.isSubmatrix() || isolated) )
        {
            IppiSize roisize = { src->cols, src->rows };
            IppiMaskSize masksize = ksize == 5 ? ippMskSize5x5 : ippMskSize3x3;
            IppDataType datatype = type == CC_8UC1 ? ipp8u : ipp32f;
            Ipp32s bufsize = 0;

            double scale = (double)(1 << ((ksize > 0 ? ksize : 3) - 1)) * blockSize;
            if (ksize < 0)
                scale *= 2.0;
            if (depth == CC_8U)
                scale *= 255.0;
            scale = std::pow(scale, -4.0);

            if (ippiHarrisCornerGetBufferSize(roisize, masksize, blockSize, datatype, cn, &bufsize) >= 0)
            {
                Ipp8u * buffer = ippsMalloc_8u(bufsize);
                IppiDifferentialKernel filterType = ksize > 0 ? ippFilterSobel : ippFilterScharr;
                IppiBorderType borderTypeIpp = borderTypeNI == CC_BORDER_CONSTANT ? ippBorderConst : ippBorderRepl;
                IppStatus status = (IppStatus)-1;

                if (depth == CC_8U)
                    status = CC_INSTRUMENT_FUN_IPP(ippiHarrisCorner_8u32f_C1R,((const Ipp8u *)src.data, (int)src->step, (Ipp32f *)dst.data, (int)dst->step, roisize,
                        filterType, masksize, blockSize, (Ipp32f)k, (Ipp32f)scale, borderTypeIpp, 0, buffer));
                else if (depth == CC_32F)
                    status = CC_INSTRUMENT_FUN_IPP(ippiHarrisCorner_32f_C1R,((const Ipp32f *)src.data, (int)src->step, (Ipp32f *)dst.data, (int)dst->step, roisize,
                        filterType, masksize, blockSize, (Ipp32f)k, (Ipp32f)scale, borderTypeIpp, 0, buffer));
                ippsFree(buffer);

                if (status >= 0)
                {
                    CC_IMPL_ADD(CC_IMPL_IPP);
                    return true;
                }
            }
        }
    }
#else
    CC_UNUSED(_src); CC_UNUSED(_dst); CC_UNUSED(blockSize);  CC_UNUSED(ksize); CC_UNUSED(k); CC_UNUSED(borderType);
#endif
    return false;
}
}
#endif

void cornerHarris( const img_t* _src, img_t* dst, int blockSize, int ksize, double k, int borderType )
{
    CC_INSTRUMENT_REGION()

    CC_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_cornerMinEigenValVecs(_src, _dst, blockSize, ksize, k, borderType, HARRIS))

#ifdef HAVE_IPP
    int borderTypeNI = borderType & ~CC_BORDER_ISOLATED;
    bool isolated = (borderType & CC_BORDER_ISOLATED) != 0;
#endif
    CC_IPP_RUN(((ksize == 3 || ksize == 5) && (_src->tid == CC_8UC1 || _src->tid == CC_32FC1) &&
        (borderTypeNI == CC_BORDER_CONSTANT || borderTypeNI == CC_BORDER_REPLICATE) && CC_MAT_CN(_src->tid) == 1 &&
        (!_src.isSubmatrix() || isolated)) && IPP_VERSION_X100 >= 810 && IPP_DISABLE_BLOCK, ipp_cornerHarris( _src, _dst, blockSize, ksize, k, borderType ));


    img_t *src = _src;
    _dst cvSetMat( cvGetSize(src), CC_32FC1 );
    img_t *dst = _dst;


    cornerEigenValsVecs( src, dst, blockSize, ksize, HARRIS, k, borderType );
}


void cornerEigenValsAndVecs( const img_t* _src, img_t* dst, int blockSize, int ksize, int borderType )
{
    CC_INSTRUMENT_REGION()

    img_t *src = _src;
    CSize dsz = _dst.size();
    int dtype = _dst->tid;

    if( dsz.height != src->rows || dsz.width*CC_MAT_CN(dtype) != src->cols*6 || CC_MAT_DEPTH(dtype) != CC_32F )
        _dst cvSetMat( cvGetSize(src), CC_32FC(6) );
    img_t *dst = _dst;
    cornerEigenValsVecs( src, dst, blockSize, ksize, EIGENVALSVECS, 0, borderType );
}


void preCornerDetect( const img_t* _src, img_t* dst, int ksize, int borderType )
{
    CC_INSTRUMENT_REGION()

    int type = _src->tid;
    CC_Assert( type == CC_8UC1 || type == CC_32FC1 );

    CC_OCL_RUN( _src.dims() <= 2 && _dst.isUMat(),
                ocl_preCornerDetect(_src, _dst, ksize, borderType, CC_MAT_DEPTH(type)))

    img_t Dx, Dy, D2x, D2y, Dxy, src = _src;
    _dst cvSetMat( cvGetSize(src), CC_32FC1 );
    img_t *dst = _dst;

    Sobel( src, Dx, CC_32F, 1, 0, ksize, 1, 0, borderType );
    Sobel( src, Dy, CC_32F, 0, 1, ksize, 1, 0, borderType );
    Sobel( src, D2x, CC_32F, 2, 0, ksize, 1, 0, borderType );
    Sobel( src, D2y, CC_32F, 0, 2, ksize, 1, 0, borderType );
    Sobel( src, Dxy, CC_32F, 1, 1, ksize, 1, 0, borderType );

    double factor = 1 << (ksize - 1);
    if( src CC_MAT_DEPTH() == CC_8U )
        factor *= 255;
    factor = 1./(factor * factor * factor);
#if CC_NEON || CC_SSE2
    float factor_f = (float)factor;
#endif

#if CC_SSE2
    volatile bool haveSSE2 = checkHardwareSupport(CC_CPU_SSE2);
    __m128 v_factor = _mm_set1_ps(factor_f), v_m2 = _mm_set1_ps(-2.0f);
#endif

    CSize size = cvGetSize(src);
    int i, j;
    for( i = 0; i < size.height; i++ )
    {
        float* dstdata = dst.ptr<float>(i);
        const float* dxdata = Dx.ptr<float>(i);
        const float* dydata = Dy.ptr<float>(i);
        const float* d2xdata = D2x.ptr<float>(i);
        const float* d2ydata = D2y.ptr<float>(i);
        const float* dxydata = Dxy.ptr<float>(i);

        j = 0;

#if CC_SSE2
        if (haveSSE2)
        {
            for( ; j <= size.width - 4; j += 4 )
            {
                __m128 v_dx = _mm_loadu_ps((const float *)(dxdata + j));
                __m128 v_dy = _mm_loadu_ps((const float *)(dydata + j));

                __m128 v_s1 = _mm_mul_ps(_mm_mul_ps(v_dx, v_dx), _mm_loadu_ps((const float *)(d2ydata + j)));
                __m128 v_s2 = _mm_mul_ps(_mm_mul_ps(v_dy, v_dy), _mm_loadu_ps((const float *)(d2xdata + j)));
                __m128 v_s3 = _mm_mul_ps(_mm_mul_ps(v_dx, v_dy), _mm_loadu_ps((const float *)(dxydata + j)));
                v_s1 = _mm_mul_ps(v_factor, _mm_add_ps(v_s1, _mm_add_ps(v_s2, _mm_mul_ps(v_s3, v_m2))));
                _mm_storeu_ps(dstdata + j, v_s1);
            }
        }
#elif CC_NEON
        for( ; j <= size.width - 4; j += 4 )
        {
            float32x4_t v_dx = vld1q_f32(dxdata + j), v_dy = vld1q_f32(dydata + j);
            float32x4_t v_s = vmulq_f32(v_dx, vmulq_f32(v_dx, vld1q_f32(d2ydata + j)));
            v_s = vmlaq_f32(v_s, vld1q_f32(d2xdata + j), vmulq_f32(v_dy, v_dy));
            v_s = vmlaq_f32(v_s, vld1q_f32(dxydata + j), vmulq_n_f32(vmulq_f32(v_dy, v_dx), -2));
            vst1q_f32(dstdata + j, vmulq_n_f32(v_s, factor_f));
        }
#endif

        for( ; j < size.width; j++ )
        {
            float dx = dxdata[j];
            float dy = dydata[j];
            dstdata[j] = (float)(factor*(dx*dx*d2ydata[j] + dy*dy*d2xdata[j] - 2*dx*dy*dxydata[j]));
        }
    }
}

CC_IMPL void
cvCornerMinEigenVal( const img_t* src, img_t* dst,
                     int block_size, int aperture_size )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);

    CC_Assert( cvGetSize(src) == dst.size() && dst->tid == CC_32FC1 );
    cornerMinEigenVal( src, dst, block_size, aperture_size, CC_BORDER_REPLICATE );
}

CC_IMPL void
cvCornerHarris( const img_t* src, img_t* dst,
                int block_size, int aperture_size, double k )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);

    CC_Assert( cvGetSize(src) == dst.size() && dst->tid == CC_32FC1 );
    cornerHarris( src, dst, block_size, aperture_size, k, CC_BORDER_REPLICATE );
}


CC_IMPL void
cvCornerEigenValsAndVecs( const img_t* src, img_t* dst,
                          int block_size, int aperture_size )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);

    CC_Assert( src->rows == dst->rows && src->cols*6 == dst->cols*dst CC_MAT_CN() && dst CC_MAT_DEPTH() == CC_32F );
    cornerEigenValsAndVecs( src, dst, block_size, aperture_size, CC_BORDER_REPLICATE );
}


CC_IMPL void
cvPreCornerDetect( const img_t* src, img_t* dst, int aperture_size )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);

    CC_Assert( cvGetSize(src) == dst.size() && dst->tid == CC_32FC1 );
    preCornerDetect( src, dst, aperture_size, CC_BORDER_REPLICATE );
}

/* End of file */
