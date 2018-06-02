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


#include "opencl_kernels_imgproc.hpp"

////////////////////////////////////////////////// matchTemplate //////////////////////////////////////////////////////////

namespace cv
{

#ifdef HAVE_OPENCL

/////////////////////////////////////////////////// CCORR //////////////////////////////////////////////////////////////

enum
{
    SUM_1 = 0, SUM_2 = 1
};

static bool extractFirstChannel_32F(const img_t* _image, img_t* result, int cn)
{
    int depth = _image CC_MAT_DEPTH();

    ocl::Device dev = ocl::Device::getDefault();
    int pxPerWIy = (dev.isIntel() && (dev->tid & ocl::Device::TYPE_GPU)) ? 4 : 1;

    ocl::Kernel k("extractFirstChannel", ocl::imgproc::match_template_oclsrc, format("-D FIRST_CHANNEL -D T1=%s -D cn=%d -D PIX_PER_WI_Y=%d",
                                                                            ocl::typeToStr(depth), cn, pxPerWIy));
    if (k.empty())
        return false;

    UMat image  = _image.getUMat();
    UMat result = _result.getUMat();


    size_t globalsize[2] = {(size_t)result->cols, ((size_t)result->rows+pxPerWIy-1)/pxPerWIy};
    return k.args(ocl::KernelArg::ReadOnlyNoSize(image), ocl::KernelArg::WriteOnly(result)).run( 2, globalsize, NULL, false);
}

static bool sumTemplate(const img_t* _src, UMat & result)
{
    int type = _src->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    int wdepth = CC_32F, wtype = CC_MAKE_TYPE(wdepth, cn);
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    int wgs2_aligned = 1;
    while (wgs2_aligned < (int)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    char cvt[40];
    ocl::Kernel k("calcSum", ocl::imgproc::match_template_oclsrc,
                  format("-D CALC_SUM -D T=%s -D T1=%s -D WT=%s -D cn=%d -D convertToWT=%s -D WGS=%d -D WGS2_ALIGNED=%d",
                         ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(wtype), cn,
                         ocl::convertTypeStr(depth, wdepth, cn, cvt),
                         (int)wgs, wgs2_aligned));
    if (k.empty())
        return false;

    UMat src = _src.getUMat();
    result cvSetMat(1, 1, CC_32FC1);

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            resarg = ocl::KernelArg::PtrWriteOnly(result);

    k.args(srcarg, src->cols, (int)src CC_MAT_TOTAL(), resarg);

    size_t globalsize = wgs;
    return k.run(1, &globalsize, &wgs, false);
}

static bool useNaive(CSize size)
{
    int dft_size = 18;
    return size.height < dft_size && size.width < dft_size;
}

struct ConvolveBuf
{
    CSize result_size;
    CSize block_size;
    CSize user_block_size;
    CSize dft_size;

    UMat image_spect, templ_spect, result_spect;
    UMat image_block, templ_block, result_data;

    void create(CSize image_size, CSize templ_size);
};

void ConvolveBuf::create(CSize image_size, CSize templ_size)
{
    result_size = CSize(image_size.width - templ_size.width + 1,
                       image_size.height - templ_size.height + 1);

    const double blockScale = 4.5;
    const int minBlockSize = 256;

    block_size.width = cRound(result_size.width*blockScale);
    block_size.width = MAX( block_size.width, minBlockSize - templ_size.width + 1 );
    block_size.width = MIN( block_size.width, result_size.width );
    block_size.height = cRound(templ_size.height*blockScale);
    block_size.height = MAX( block_size.height, minBlockSize - templ_size.height + 1 );
    block_size.height = MIN( block_size.height, result_size.height );

    dft_size.width = MAX(getOptimalDFTSize(block_size.width + templ_size.width - 1), 2);
    dft_size.height = getOptimalDFTSize(block_size.height + templ_size.height - 1);
    if( dft_size.width <= 0 || dft_size.height <= 0 )
        CC_Error( CC_StsOutOfRange, "the input arrays are too big" );

    // recompute block size
    block_size.width = dft_size.width - templ_size.width + 1;
    block_size.width = MIN( block_size.width, result_size.width);
    block_size.height = dft_size.height - templ_size.height + 1;
    block_size.height = MIN( block_size.height, result_size.height );

    image_block cvSetMat(dft_size, CC_32F);
    templ_block cvSetMat(dft_size, CC_32F);
    result_data cvSetMat(dft_size, CC_32F);

    image_spect cvSetMat(dft_size.height, dft_size.width / 2 + 1, CC_32FC2);
    templ_spect cvSetMat(dft_size.height, dft_size.width / 2 + 1, CC_32FC2);
    result_spect cvSetMat(dft_size.height, dft_size.width / 2 + 1, CC_32FC2);

    // Use maximum result matrix block size for the estimated DFT block size
    block_size.width = MIN(dft_size.width - templ_size.width + 1, result_size.width);
    block_size.height = MIN(dft_size.height - templ_size.height + 1, result_size.height);
}

static bool convolve_dft(const img_t* _image, const img_t* _templ, img_t* result)
{
    ConvolveBuf buf;
    CC_Assert(_image->tid == CC_32F);
    CC_Assert(_templ->tid == CC_32F);

    buf cvSetMat(_image.size(), _templ.size());
    _result cvSetMat(buf.result_size, CC_32F);

    UMat image  = _image.getUMat();
    UMat templ  = _templ.getUMat();

    UMat result = _result.getUMat();

    CSize& block_size = buf.block_size;
    CSize& dft_size = buf.dft_size;

    UMat& image_block = buf.image_block;
    UMat& templ_block = buf.templ_block;
    UMat& result_data = buf.result_data;

    UMat& image_spect = buf.image_spect;
    UMat& templ_spect = buf.templ_spect;
    UMat& result_spect = buf.result_spect;

    UMat templ_roi = templ;
    copyMakeBorder(templ_roi, templ_block, 0, templ_block->rows - templ_roi->rows, 0,
                   templ_block->cols - templ_roi->cols, CC_BORDER_ISOLATED);

    dft(templ_block, templ_spect, 0, templ->rows);

    // Process all blocks of the result matrix
    for (int y = 0; y < result->rows; y += block_size.height)
    {
        for (int x = 0; x < result->cols; x += block_size.width)
        {
            CSize image_roi_size(MIN(x + dft_size.width, image->cols) - x,
                                MIN(y + dft_size.height, image->rows) - y);
            Rect roi0(x, y, image_roi_size.width, image_roi_size.height);

            UMat image_roi(image, roi0);

            copyMakeBorder(image_roi, image_block, 0, image_block->rows - image_roi->rows,
                           0, image_block->cols - image_roi->cols, CC_BORDER_ISOLATED);

            dft(image_block, image_spect, 0);

            mulSpectrums(image_spect, templ_spect, result_spect, 0, true);

            dft(result_spect, result_data, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

            CSize result_roi_size(MIN(x + block_size.width, result->cols) - x,
                                 MIN(y + block_size.height, result->rows) - y);

            Rect roi1(x, y, result_roi_size.width, result_roi_size.height);
            Rect roi2(0, 0, result_roi_size.width, result_roi_size.height);

            UMat result_roi(result, roi1);
            UMat result_block(result_data, roi2);

            result_block.copyTo(result_roi);
        }
    }
    return true;
}

static bool convolve_32F(const img_t* _image, const img_t* _templ, img_t* result)
{
    _result cvSetMat(_image->rows() - _templ->rows() + 1, _image->cols() - _templ->cols() + 1, CC_32F);

    if (_image CC_MAT_CN() == 1)
        return(convolve_dft(_image, _templ, _result));
    else
    {
        UMat image = _image.getUMat();
        UMat templ = _templ.getUMat();
        UMat result_(image->rows-templ->rows+1,(image->cols-templ->cols+1)*image CC_MAT_CN(), CC_32F);
        bool ok = convolve_dft(image cvReshape(,1), templ cvReshape(,1), result_);
        if (ok==false)
            return false;
        UMat result = _result.getUMat();
        return (extractFirstChannel_32F(result_, _result, _image CC_MAT_CN()));
    }
}

static bool matchTemplateNaive_CCORR(const img_t* _image, const img_t* _templ, img_t* result)
{
    int type = _image->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    int wdepth = CC_32F, wtype = CC_MAKE_TYPE(wdepth, cn);

    ocl::Device dev = ocl::Device::getDefault();
    int pxPerWIx = (cn==1 && dev.isIntel() && (dev->tid & ocl::Device::TYPE_GPU)) ? 4 : 1;
    int rated_cn = cn;
    int wtype1 = wtype;

    if (pxPerWIx!=1)
    {
        rated_cn = pxPerWIx;
        type = CC_MAKE_TYPE(depth, rated_cn);
        wtype1 = CC_MAKE_TYPE(wdepth, rated_cn);
    }

    char cvt[40];
    char cvt1[40];
    const char* convertToWT1 = ocl::convertTypeStr(depth, wdepth, cn, cvt);
    const char* convertToWT = ocl::convertTypeStr(depth, wdepth, rated_cn, cvt1);

    ocl::Kernel k("matchTemplate_Naive_CCORR", ocl::imgproc::match_template_oclsrc,
                  format("-D CCORR -D T=%s -D T1=%s -D WT=%s -D WT1=%s -D convertToWT=%s -D convertToWT1=%s -D cn=%d -D PIX_PER_WI_X=%d", ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(wtype1), ocl::typeToStr(wtype),
                         convertToWT, convertToWT1, cn, pxPerWIx));
    if (k.empty())
        return false;

    UMat image = _image.getUMat(), templ = _templ.getUMat();
    _result cvSetMat(image->rows - templ->rows + 1, image->cols - templ->cols + 1, CC_32FC1);
    UMat result = _result.getUMat();

    k.args(ocl::KernelArg::ReadOnlyNoSize(image), ocl::KernelArg::ReadOnly(templ),
           ocl::KernelArg::WriteOnly(result));

    size_t globalsize[2] = { ((size_t)result->cols+pxPerWIx-1)/pxPerWIx, (size_t)result->rows};
    return k.run(2, globalsize, NULL, false);
}


static bool matchTemplate_CCORR(const img_t* _image, const img_t* _templ, img_t* result)
{
    if (useNaive(_templ.size()))
        return( matchTemplateNaive_CCORR(_image, _templ, _result));
    else
    {
        if(_image CC_MAT_DEPTH() == CC_8U)
        {
            UMat imagef, templf;
            UMat image = _image.getUMat();
            UMat templ = _templ.getUMat();
            image.convertTo(imagef, CC_32F);
            templ.convertTo(templf, CC_32F);
            return(convolve_32F(imagef, templf, _result));
        }
        else
        {
            return(convolve_32F(_image, _templ, _result));
        }
    }
}

static bool matchTemplate_CCORR_NORMED(const img_t* _image, const img_t* _templ, img_t* result)
{
    matchTemplate(_image, _templ, _result, CC_TM_CCORR);

    int type = _image->tid, cn = CC_MAT_CN(type);

    ocl::Kernel k("matchTemplate_CCORR_NORMED", ocl::imgproc::match_template_oclsrc,
                  format("-D CCORR_NORMED -D T=%s -D cn=%d", ocl::typeToStr(type), cn));
    if (k.empty())
        return false;

    UMat image = _image.getUMat(), templ = _templ.getUMat();
    _result cvSetMat(image->rows - templ->rows + 1, image->cols - templ->cols + 1, CC_32FC1);
    UMat result = _result.getUMat();

    UMat image_sums, image_sqsums;
    integral(image cvReshape(,1), image_sums, image_sqsums, CC_32F, CC_32F);

    UMat templ_sqsum;
    if (!sumTemplate(templ, templ_sqsum))
        return false;

    k.args(ocl::KernelArg::ReadOnlyNoSize(image_sqsums), ocl::KernelArg::ReadWrite(result),
           templ->rows, templ->cols, ocl::KernelArg::PtrReadOnly(templ_sqsum));

    size_t globalsize[2] = { (size_t)result->cols, (size_t)result->rows };
    return k.run(2, globalsize, NULL, false);
}

////////////////////////////////////// SQDIFF //////////////////////////////////////////////////////////////

static bool matchTemplateNaive_SQDIFF(const img_t* _image, const img_t* _templ, img_t* result)
{
    int type = _image->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    int wdepth = CC_32F, wtype = CC_MAKE_TYPE(wdepth, cn);

    char cvt[40];
    ocl::Kernel k("matchTemplate_Naive_SQDIFF", ocl::imgproc::match_template_oclsrc,
                  format("-D SQDIFF -D T=%s -D T1=%s -D WT=%s -D convertToWT=%s -D cn=%d", ocl::typeToStr(type), ocl::typeToStr(depth),
                         ocl::typeToStr(wtype), ocl::convertTypeStr(depth, wdepth, cn, cvt), cn));
    if (k.empty())
        return false;

    UMat image = _image.getUMat(), templ = _templ.getUMat();
    _result cvSetMat(image->rows - templ->rows + 1, image->cols - templ->cols + 1, CC_32F);
    UMat result = _result.getUMat();

    k.args(ocl::KernelArg::ReadOnlyNoSize(image), ocl::KernelArg::ReadOnly(templ),
           ocl::KernelArg::WriteOnly(result));

    size_t globalsize[2] = { (size_t)result->cols, (size_t)result->rows };
    return k.run(2, globalsize, NULL, false);
}

static bool matchTemplate_SQDIFF(const img_t* _image, const img_t* _templ, img_t* result)
{
    if (useNaive(_templ.size()))
        return( matchTemplateNaive_SQDIFF(_image, _templ, _result));
    else
    {
        matchTemplate(_image, _templ, _result, CC_TM_CCORR);

        int type = _image->tid, cn = CC_MAT_CN(type);

        ocl::Kernel k("matchTemplate_Prepared_SQDIFF", ocl::imgproc::match_template_oclsrc,
                  format("-D SQDIFF_PREPARED -D T=%s -D cn=%d", ocl::typeToStr(type),  cn));
        if (k.empty())
            return false;

        UMat image = _image.getUMat(), templ = _templ.getUMat();
        _result cvSetMat(image->rows - templ->rows + 1, image->cols - templ->cols + 1, CC_32F);
        UMat result = _result.getUMat();

        UMat image_sums, image_sqsums;
        integral(image cvReshape(,1), image_sums, image_sqsums, CC_32F, CC_32F);

        UMat templ_sqsum;
        if (!sumTemplate(_templ, templ_sqsum))
            return false;

        k.args(ocl::KernelArg::ReadOnlyNoSize(image_sqsums), ocl::KernelArg::ReadWrite(result),
           templ->rows, templ->cols, ocl::KernelArg::PtrReadOnly(templ_sqsum));

        size_t globalsize[2] = { (size_t)result->cols, (size_t)result->rows };

        return k.run(2, globalsize, NULL, false);
    }
}

static bool matchTemplate_SQDIFF_NORMED(const img_t* _image, const img_t* _templ, img_t* result)
{
    matchTemplate(_image, _templ, _result, CC_TM_CCORR);

    int type = _image->tid, cn = CC_MAT_CN(type);

    ocl::Kernel k("matchTemplate_SQDIFF_NORMED", ocl::imgproc::match_template_oclsrc,
                  format("-D SQDIFF_NORMED -D T=%s -D cn=%d", ocl::typeToStr(type),  cn));
    if (k.empty())
        return false;

    UMat image = _image.getUMat(), templ = _templ.getUMat();
    _result cvSetMat(image->rows - templ->rows + 1, image->cols - templ->cols + 1, CC_32F);
    UMat result = _result.getUMat();

    UMat image_sums, image_sqsums;
    integral(image cvReshape(,1), image_sums, image_sqsums, CC_32F, CC_32F);

    UMat templ_sqsum;
    if (!sumTemplate(_templ, templ_sqsum))
        return false;

    k.args(ocl::KernelArg::ReadOnlyNoSize(image_sqsums), ocl::KernelArg::ReadWrite(result),
           templ->rows, templ->cols, ocl::KernelArg::PtrReadOnly(templ_sqsum));

    size_t globalsize[2] = { (size_t)result->cols, (size_t)result->rows };

    return k.run(2, globalsize, NULL, false);
}

///////////////////////////////////// CCOEFF /////////////////////////////////////////////////////////////////

static bool matchTemplate_CCOEFF(const img_t* _image, const img_t* _templ, img_t* result)
{
    matchTemplate(_image, _templ, _result, CC_TM_CCORR);

    UMat image_sums, temp;
    integral(_image, image_sums, CC_32F);

    int type = image_sums->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);

    ocl::Kernel k("matchTemplate_Prepared_CCOEFF", ocl::imgproc::match_template_oclsrc,
                  format("-D CCOEFF -D T=%s -D T1=%s -D cn=%d", ocl::typeToStr(type), ocl::typeToStr(depth), cn));
    if (k.empty())
        return false;

    UMat templ  = _templ.getUMat();
    UMat result = _result.getUMat();

    if (cn==1)
    {
        Scalar templMean = mean(templ);
        float templ_sum = (float)templMean[0];

        k.args(ocl::KernelArg::ReadOnlyNoSize(image_sums), ocl::KernelArg::ReadWrite(result), templ->rows, templ->cols, templ_sum);
    }
    else
    {
        Vec4f templ_sum = Vec4f::all(0);
        templ_sum = (Vec4f)mean(templ);

       k.args(ocl::KernelArg::ReadOnlyNoSize(image_sums), ocl::KernelArg::ReadWrite(result), templ->rows, templ->cols, templ_sum);    }

    size_t globalsize[2] = { (size_t)result->cols, (size_t)result->rows };
    return k.run(2, globalsize, NULL, false);
}

static bool matchTemplate_CCOEFF_NORMED(const img_t* _image, const img_t* _templ, img_t* result)
{
    matchTemplate(_image, _templ, _result, CC_TM_CCORR);

    UMat temp, image_sums, image_sqsums;
    integral(_image, image_sums, image_sqsums, CC_32F, CC_32F);

    int type = image_sums->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);

    ocl::Kernel k("matchTemplate_CCOEFF_NORMED", ocl::imgproc::match_template_oclsrc,
        format("-D CCOEFF_NORMED -D T=%s -D T1=%s -D cn=%d", ocl::typeToStr(type), ocl::typeToStr(depth), cn));
    if (k.empty())
        return false;

    UMat templ = _templ.getUMat();
    CSize size = _image.size(), tsize = templ.size();
    _result cvSetMat(size.height - templ->rows + 1, size.width - templ->cols + 1, CC_32F);
    UMat result = _result.getUMat();

    float scale = 1.f / tsize.area();

    if (cn == 1)
    {
        float templ_sum = (float)sum(templ)[0];

        multiply(templ, templ, temp, 1, CC_32F);
        float templ_sqsum = (float)sum(temp)[0];

        templ_sqsum -= scale * templ_sum * templ_sum;
        templ_sum   *= scale;

        if (templ_sqsum < DBL_EPSILON)
        {
            result = Scalar::all(1);
            return true;
        }

        k.args(ocl::KernelArg::ReadOnlyNoSize(image_sums), ocl::KernelArg::ReadOnlyNoSize(image_sqsums),
                      ocl::KernelArg::ReadWrite(result), templ->rows, templ->cols, scale, templ_sum, templ_sqsum);
    }
    else
    {
        Vec4f templ_sum = Vec4f::all(0), templ_sqsum = Vec4f::all(0);
        templ_sum = sum(templ);

        multiply(templ, templ, temp, 1, CC_32F);
        templ_sqsum = sum(temp);

        float templ_sqsum_sum = 0;
        for (int i = 0; i < cn; i ++)
            templ_sqsum_sum += templ_sqsum[i] - scale * templ_sum[i] * templ_sum[i];

        templ_sum *= scale;

        if (templ_sqsum_sum < DBL_EPSILON)
        {
            result = Scalar::all(1);
            return true;
        }

        k.args(ocl::KernelArg::ReadOnlyNoSize(image_sums), ocl::KernelArg::ReadOnlyNoSize(image_sqsums),
                   ocl::KernelArg::ReadWrite(result), templ->rows, templ->cols, scale,
                   templ_sum, templ_sqsum_sum);    }

    size_t globalsize[2] = { (size_t)result->cols, (size_t)result->rows };
    return k.run(2, globalsize, NULL, false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool ocl_matchTemplate( const img_t* _img, const img_t* _templ, img_t* result, int method)
{
    int cn = _img CC_MAT_CN();

    if (cn > 4)
        return false;

    typedef bool (*Caller)(const img_t* _img, const img_t* _templ, img_t* result);

    static const Caller callers[] =
    {
        matchTemplate_SQDIFF, matchTemplate_SQDIFF_NORMED, matchTemplate_CCORR,
        matchTemplate_CCORR_NORMED, matchTemplate_CCOEFF, matchTemplate_CCOEFF_NORMED
    };
    const Caller caller = callers[method];

    return caller(_img, _templ, _result);
}

#endif

#if defined HAVE_IPP

typedef IppStatus (C_STDCALL * ippimatchTemplate)(const void*, int, IppiSize, const void*, int, IppiSize, Ipp32f* , int , IppEnum , Ipp8u*);

static bool ipp_crossCorr(const img_t& src, const img_t& tpl, img_t& dst)
{
    CC_INSTRUMENT_REGION_IPP()

    IppStatus status;

    IppiSize srcRoiSize = {src->cols,src->rows};
    IppiSize tplRoiSize = {tpl->cols,tpl->rows};

    Ipp8u *pBuffer;
    int bufSize=0;

    int depth = src CC_MAT_DEPTH();

    ippimatchTemplate ippiCrossCorrNorm =
            depth==CC_8U ? (ippimatchTemplate)ippiCrossCorrNorm_8u32f_C1R:
            depth==CC_32F? (ippimatchTemplate)ippiCrossCorrNorm_32f_C1R: 0;

    if (ippiCrossCorrNorm==0)
        return false;

    IppEnum funCfg = (IppEnum)(ippAlgAuto | ippiNormNone | ippiROIValid);

    status = ippiCrossCorrNormGetBufferSize(srcRoiSize, tplRoiSize, funCfg, &bufSize);
    if ( status < 0 )
        return false;

    pBuffer = ippsMalloc_8u( bufSize );

    status = CC_INSTRUMENT_FUN_IPP(ippiCrossCorrNorm, src->tt.data, (int)src->step, srcRoiSize, tpl->tt.data, (int)tpl->step, tplRoiSize, dst.ptr<Ipp32f>(), (int)dst->step, funCfg, pBuffer);

    ippsFree( pBuffer );
    return status >= 0;
}

static bool ipp_sqrDistance(const img_t& src, const img_t& tpl, img_t& dst)
{
    CC_INSTRUMENT_REGION_IPP()

    IppStatus status;

    IppiSize srcRoiSize = {src->cols,src->rows};
    IppiSize tplRoiSize = {tpl->cols,tpl->rows};

    Ipp8u *pBuffer;
    int bufSize=0;

    int depth = src CC_MAT_DEPTH();

    ippimatchTemplate ippiSqrDistanceNorm =
            depth==CC_8U ? (ippimatchTemplate)ippiSqrDistanceNorm_8u32f_C1R:
            depth==CC_32F? (ippimatchTemplate)ippiSqrDistanceNorm_32f_C1R: 0;

    if (ippiSqrDistanceNorm==0)
        return false;

    IppEnum funCfg = (IppEnum)(ippAlgAuto | ippiNormNone | ippiROIValid);

    status = ippiSqrDistanceNormGetBufferSize(srcRoiSize, tplRoiSize, funCfg, &bufSize);
    if ( status < 0 )
        return false;

    pBuffer = ippsMalloc_8u( bufSize );

    status = CC_INSTRUMENT_FUN_IPP(ippiSqrDistanceNorm, src->tt.data, (int)src->step, srcRoiSize, tpl->tt.data, (int)tpl->step, tplRoiSize, dst.ptr<Ipp32f>(), (int)dst->step, funCfg, pBuffer);

    ippsFree( pBuffer );
    return status >= 0;
}

#endif

#include "opencv2/core/hal/hal.hpp"

void crossCorr( const img_t& img, const img_t& _templ, img_t& corr,
                CSize corrsize, int ctype,
                Point anchor, double delta, int borderType )
{
    const double blockScale = 4.5;
    const int minBlockSize = 256;
    std::vector<uchar> buf;

    img_t templ = _templ;
    int depth = img CC_MAT_DEPTH(), cn = img CC_MAT_CN();
    int tdepth = templ CC_MAT_DEPTH(), tcn = templ CC_MAT_CN();
    int cdepth = CC_MAT_DEPTH(ctype), ccn = CC_MAT_CN(ctype);

    CC_Assert( img.dims <= 2 && templ.dims <= 2 && corr.dims <= 2 );

    if( depth != tdepth && tdepth != MAX(CC_32F, depth) )
    {
        _templ.convertTo(templ, MAX(CC_32F, depth));
        tdepth = templ CC_MAT_DEPTH();
    }

    CC_Assert( depth == tdepth || tdepth == CC_32F);
    CC_Assert( corrsize.height <= img->rows + templ->rows - 1 &&
               corrsize.width <= img->cols + templ->cols - 1 );

    CC_Assert( ccn == 1 || delta == 0 );

    corr cvSetMat(corrsize, ctype);

    int maxDepth = depth > CC_8S ? CC_64F : MAX(MAX(CC_32F, tdepth), cdepth);
    CSize blocksize, dftsize;

    blocksize.width = cRound(templ->cols*blockScale);
    blocksize.width = MAX( blocksize.width, minBlockSize - templ->cols + 1 );
    blocksize.width = MIN( blocksize.width, corr->cols );
    blocksize.height = cRound(templ->rows*blockScale);
    blocksize.height = MAX( blocksize.height, minBlockSize - templ->rows + 1 );
    blocksize.height = MIN( blocksize.height, corr->rows );

    dftsize.width = MAX(getOptimalDFTSize(blocksize.width + templ->cols - 1), 2);
    dftsize.height = getOptimalDFTSize(blocksize.height + templ->rows - 1);
    if( dftsize.width <= 0 || dftsize.height <= 0 )
        CC_Error( CC_StsOutOfRange, "the input arrays are too big" );

    // recompute block size
    blocksize.width = dftsize.width - templ->cols + 1;
    blocksize.width = MIN( blocksize.width, corr->cols );
    blocksize.height = dftsize.height - templ->rows + 1;
    blocksize.height = MIN( blocksize.height, corr->rows );

    img_t dftTempl( dftsize.height*tcn, dftsize.width, maxDepth );
    img_t dftImg( dftsize, maxDepth );

    int i, k, bufSize = 0;
    if( tcn > 1 && tdepth != maxDepth )
        bufSize = templ->cols*templ->rows*CC_ELEM_SIZE(tdepth);

    if( cn > 1 && depth != maxDepth )
        bufSize = MAX( bufSize, (blocksize.width + templ->cols - 1)*
            (blocksize.height + templ->rows - 1)*CC_ELEM_SIZE(depth));

    if( (ccn > 1 || cn > 1) && cdepth != maxDepth )
        bufSize = MAX( bufSize, blocksize.width*blocksize.height*CC_ELEM_SIZE(cdepth));

    buf.resize(bufSize);

    Ptr<hal_DFT2D> c = hal_DFT2D::create(dftsize.width, dftsize.height, dftTempl CC_MAT_DEPTH(), 1, 1, CC_HAL_DFT_IS_INPLACE, templ->rows);

    // compute DFT of each template plane
    for( k = 0; k < tcn; k++ )
    {
        int yofs = k*dftsize.height;
        img_t src = templ;
        img_t dst(dftTempl, Rect(0, yofs, dftsize.width, dftsize.height));
        img_t dst1(dftTempl, Rect(0, yofs, templ->cols, templ->rows));

        if( tcn > 1 )
        {
            src = tdepth == maxDepth ? dst1 : img_t(templ.size(), tdepth, &buf[0]);
            int pairs[] = {k, 0};
            mixChannels(&templ, 1, &src, 1, pairs, 1);
        }

        if( dst1.data != src.data )
            src.convertTo(dst1, dst1 CC_MAT_DEPTH());

        if( dst->cols > templ->cols )
        {
            img_t part(dst, CRange(0, templ->rows), CRange(templ->cols, dst->cols));
            part = Scalar::all(0);
        }
        c->apply(dst.data, (int)dst->step, dst.data, (int)dst->step);
    }

    int tileCountX = (corr->cols + blocksize.width - 1)/blocksize.width;
    int tileCountY = (corr->rows + blocksize.height - 1)/blocksize.height;
    int tileCount = tileCountX * tileCountY;

    CSize wholeSize = img.size();
    Point roiofs(0,0);
    img_t img0 = img;

    if( !(borderType & CC_BORDER_ISOLATED) )
    {
        img.locateROI(wholeSize, roiofs);
        img0.adjustROI(roiofs.y, wholeSize.height-img->rows-roiofs.y,
                       roiofs.x, wholeSize.width-img->cols-roiofs.x);
    }
    borderType |= CC_BORDER_ISOLATED;

    Ptr<hal_DFT2D> cF, cR;
    int f = CC_HAL_DFT_IS_INPLACE;
    int f_inv = f | CC_HAL_DFT_INVERSE | CC_HAL_DFT_SCALE;
    cF = hal_DFT2D::create(dftsize.width, dftsize.height, maxDepth, 1, 1, f, blocksize.height + templ->rows - 1);
    cR = hal_DFT2D::create(dftsize.width, dftsize.height, maxDepth, 1, 1, f_inv, blocksize.height);

    // calculate correlation by blocks
    for( i = 0; i < tileCount; i++ )
    {
        int x = (i%tileCountX)*blocksize.width;
        int y = (i/tileCountX)*blocksize.height;

        CSize bsz(MIN(blocksize.width, corr->cols - x),
                 MIN(blocksize.height, corr->rows - y));
        CSize dsz(bsz.width + templ->cols - 1, bsz.height + templ->rows - 1);
        int x0 = x - anchor.x + roiofs.x, y0 = y - anchor.y + roiofs.y;
        int x1 = MAX(0, x0), y1 = MAX(0, y0);
        int x2 = MIN(img0->cols, x0 + dsz.width);
        int y2 = MIN(img0->rows, y0 + dsz.height);
        img_t src0(img0, CRange(y1, y2), CRange(x1, x2));
        img_t dst(dftImg, Rect(0, 0, dsz.width, dsz.height));
        img_t dst1(dftImg, Rect(x1-x0, y1-y0, x2-x1, y2-y1));
        img_t cdst(corr, Rect(x, y, bsz.width, bsz.height));

        for( k = 0; k < cn; k++ )
        {
            img_t src = src0;
            dftImg = Scalar::all(0);

            if( cn > 1 )
            {
                src = depth == maxDepth ? dst1 : img_t(y2-y1, x2-x1, depth, &buf[0]);
                int pairs[] = {k, 0};
                mixChannels(&src0, 1, &src, 1, pairs, 1);
            }

            if( dst1.data != src.data )
                src.convertTo(dst1, dst1 CC_MAT_DEPTH());

            if( x2 - x1 < dsz.width || y2 - y1 < dsz.height )
                copyMakeBorder(dst1, dst, y1-y0, dst->rows-dst1->rows-(y1-y0),
                               x1-x0, dst->cols-dst1->cols-(x1-x0), borderType);

            if (bsz.height == blocksize.height)
                cF->apply(dftImg.data, (int)dftImg->step, dftImg.data, (int)dftImg->step);
            else
                dft( dftImg, dftImg, 0, dsz.height );

            img_t dftTempl1(dftTempl, Rect(0, tcn > 1 ? k*dftsize.height : 0,
                                         dftsize.width, dftsize.height));
            mulSpectrums(dftImg, dftTempl1, dftImg, 0, true);

            if (bsz.height == blocksize.height)
                cR->apply(dftImg.data, (int)dftImg->step, dftImg.data, (int)dftImg->step);
            else
                dft( dftImg, dftImg, DFT_INVERSE + DFT_SCALE, bsz.height );

            src = dftImg(Rect(0, 0, bsz.width, bsz.height));

            if( ccn > 1 )
            {
                if( cdepth != maxDepth )
                {
                    img_t plane(bsz, cdepth, &buf[0]);
                    src.convertTo(plane, cdepth, 1, delta);
                    src = plane;
                }
                int pairs[] = {0, k};
                mixChannels(&src, 1, &cdst, 1, pairs, 1);
            }
            else
            {
                if( k == 0 )
                    src.convertTo(cdst, cdepth, 1, delta);
                else
                {
                    if( maxDepth != cdepth )
                    {
                        img_t plane(bsz, cdepth, &buf[0]);
                        src.convertTo(plane, cdepth);
                        src = plane;
                    }
                    add(src, cdst, cdst);
                }
            }
        }
    }
}

static void matchTemplateMask( const img_t* _img, const img_t* _templ, img_t* result, int method, const img_t* _mask )
{
    int type = _img->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    CC_Assert( CC_TM_SQDIFF <= method && method <= CC_TM_CCOEFF_NORMED );
    CC_Assert( (depth == CC_8U || depth == CC_32F) && type == _templ->tid && _img.dims() <= 2 );

    img_t img = _img, templ = _templ, mask = _mask;
    int ttype = templ->tid, tdepth = CC_MAT_DEPTH(ttype), tcn = CC_MAT_CN(ttype);
    int mtype = img->tid, mdepth = CC_MAT_DEPTH(type), mcn = CC_MAT_CN(mtype);

    if (depth == CC_8U)
    {
        depth = CC_32F;
        type = CC_MAKETYPE(CC_32F, cn);
        img.convertTo(img, type, 1.0 / 255);
    }

    if (tdepth == CC_8U)
    {
        tdepth = CC_32F;
        ttype = CC_MAKETYPE(CC_32F, tcn);
        templ.convertTo(templ, ttype, 1.0 / 255);
    }

    if (mdepth == CC_8U)
    {
        mdepth = CC_32F;
        mtype = CC_MAKETYPE(CC_32F, mcn);
        compare(mask, Scalar::all(0), mask, CMP_NE);
        mask.convertTo(mask, mtype, 1.0 / 255);
    }

    CSize corrSize(img->cols - templ->cols + 1, img->rows - templ->rows + 1);
    _result cvSetMat(corrSize, CC_32F);
    img_t result = _result;

    img_t img2 = img.mul(img);
    img_t mask2 = mask.mul(mask);
    img_t mask_templ = templ.mul(mask);
    Scalar templMean, templSdv;

    double templSum2 = 0;
    meanStdDev( mask_templ, templMean, templSdv );

    templSum2 = templSdv[0]*templSdv[0] + templSdv[1]*templSdv[1] + templSdv[2]*templSdv[2] + templSdv[3]*templSdv[3];
    templSum2 += templMean[0]*templMean[0] + templMean[1]*templMean[1] + templMean[2]*templMean[2] + templMean[3]*templMean[3];
    templSum2 *= ((double)templ->rows * templ->cols);

    if (method == CC_TM_SQDIFF)
    {
        img_t mask2_templ = templ.mul(mask2);

        img_t corr(corrSize, CC_32F);
        crossCorr( img, mask2_templ, corr, corr.size(), corr->tid, Point(0,0), 0, 0 );
        crossCorr( img2, mask, result, result.size(), result->tid, Point(0,0), 0, 0 );

        result -= corr * 2;
        result += templSum2;
    }
    else if (method == CC_TM_CCORR_NORMED)
    {
        if (templSum2 < DBL_EPSILON)
        {
            result = Scalar::all(1);
            return;
        }

        img_t corr(corrSize, CC_32F);
        crossCorr( img2, mask2, corr, corr.size(), corr->tid, Point(0,0), 0, 0 );
        crossCorr( img, mask_templ, result, result.size(), result->tid, Point(0,0), 0, 0 );

        sqrt(corr, corr);
        result = result.mul(1/corr);
        result /= sqrt(templSum2);
    }
    else
        CC_Error(CC_StsNotImplemented, "");
}
}


namespace cv
{
static void common_matchTemplate( img_t& img, img_t& templ, img_t& result, int method, int cn )
{
    if( method == CC_TM_CCORR )
        return;

    int numType = method == CC_TM_CCORR || method == CC_TM_CCORR_NORMED ? 0 :
                  method == CC_TM_CCOEFF || method == CC_TM_CCOEFF_NORMED ? 1 : 2;
    bool isNormed = method == CC_TM_CCORR_NORMED ||
                    method == CC_TM_SQDIFF_NORMED ||
                    method == CC_TM_CCOEFF_NORMED;

    double invArea = 1./((double)templ->rows * templ->cols);

    img_t sum, sqsum;
    Scalar templMean, templSdv;
    double *q0 = 0, *q1 = 0, *q2 = 0, *q3 = 0;
    double templNorm = 0, templSum2 = 0;

    if( method == CC_TM_CCOEFF )
    {
        integral(img, sum, CC_64F);
        templMean = mean(templ);
    }
    else
    {
        integral(img, sum, sqsum, CC_64F);
        meanStdDev( templ, templMean, templSdv );

        templNorm = templSdv[0]*templSdv[0] + templSdv[1]*templSdv[1] + templSdv[2]*templSdv[2] + templSdv[3]*templSdv[3];

        if( templNorm < DBL_EPSILON && method == CC_TM_CCOEFF_NORMED )
        {
            result = Scalar::all(1);
            return;
        }

        templSum2 = templNorm + templMean[0]*templMean[0] + templMean[1]*templMean[1] + templMean[2]*templMean[2] + templMean[3]*templMean[3];

        if( numType != 1 )
        {
            templMean = Scalar::all(0);
            templNorm = templSum2;
        }

        templSum2 /= invArea;
        templNorm = sqrt(templNorm);
        templNorm /= sqrt(invArea); // care of accuracy here

        q0 = (double*)sqsum.data;
        q1 = q0 + templ->cols*cn;
        q2 = (double*)(sqsum.data + templ->rows*sqsum->step);
        q3 = q2 + templ->cols*cn;
    }

    double* p0 = (double*)sum.data;
    double* p1 = p0 + templ->cols*cn;
    double* p2 = (double*)(sum.data + templ->rows*sum->step);
    double* p3 = p2 + templ->cols*cn;

    int sumstep = sum.data ? (int)(sum->step / sizeof(double)) : 0;
    int sqstep = sqsum.data ? (int)(sqsum->step / sizeof(double)) : 0;

    int i, j, k;

    for( i = 0; i < result->rows; i++ )
    {
        float* rrow = result.ptr<float>(i);
        int idx = i * sumstep;
        int idx2 = i * sqstep;

        for( j = 0; j < result->cols; j++, idx += cn, idx2 += cn )
        {
            double num = rrow[j], t;
            double wndMean2 = 0, wndSum2 = 0;

            if( numType == 1 )
            {
                for( k = 0; k < cn; k++ )
                {
                    t = p0[idx+k] - p1[idx+k] - p2[idx+k] + p3[idx+k];
                    wndMean2 += t*t;
                    num -= t*templMean[k];
                }

                wndMean2 *= invArea;
            }

            if( isNormed || numType == 2 )
            {
                for( k = 0; k < cn; k++ )
                {
                    t = q0[idx2+k] - q1[idx2+k] - q2[idx2+k] + q3[idx2+k];
                    wndSum2 += t;
                }

                if( numType == 2 )
                {
                    num = wndSum2 - 2*num + templSum2;
                    num = MAX(num, 0.);
                }
            }

            if( isNormed )
            {
                t = sqrt(MAX(wndSum2 - wndMean2,0))*templNorm;
                if( fabs(num) < t )
                    num /= t;
                else if( fabs(num) < t*1.125 )
                    num = num > 0 ? 1 : -1;
                else
                    num = method != CC_TM_SQDIFF_NORMED ? 0 : 1;
            }

            rrow[j] = (float)num;
        }
    }
}
}


#if defined HAVE_IPP
namespace cv
{
static bool ipp_matchTemplate( img_t& img, img_t& templ, img_t& result, int method, int cn )
{
    CC_INSTRUMENT_REGION_IPP()

    bool useIppMT = (templ->rows < img->rows/2 && templ->cols < img->cols/2);

    if(cn == 1 && useIppMT)
    {
        if(method == CC_TM_SQDIFF)
        {
            if (ipp_sqrDistance(img, templ, result))
                return true;
        }
        else
        {
            if(ipp_crossCorr(img, templ, result))
            {
                common_matchTemplate(img, templ, result, method, cn);
                return true;
            }
        }
    }

    return false;
}
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////

void matchTemplate( const img_t* _img, const img_t* _templ, img_t* result, int method, const img_t* _mask )
{
    CC_INSTRUMENT_REGION()

    if (!_mask.empty())
    {
        matchTemplateMask(_img, _templ, _result, method, _mask);
        return;
    }

    int type = _img->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    CC_Assert( CC_TM_SQDIFF <= method && method <= CC_TM_CCOEFF_NORMED );
    CC_Assert( (depth == CC_8U || depth == CC_32F) && type == _templ->tid && _img.dims() <= 2 );

    bool needswap = _img.size().height < _templ.size().height || _img.size().width < _templ.size().width;
    if (needswap)
    {
        CC_Assert(_img.size().height <= _templ.size().height && _img.size().width <= _templ.size().width);
    }

    CC_OCL_RUN(_img.dims() <= 2 && _result.isUMat(),
               (!needswap ? ocl_matchTemplate(_img, _templ, _result, method) : ocl_matchTemplate(_templ, _img, _result, method)))

    img_t img = _img, templ = _templ;
    if (needswap)
        T_SWAP(_Tp, img, templ);

    CSize corrSize(img->cols - templ->cols + 1, img->rows - templ->rows + 1);
    _result cvSetMat(corrSize, CC_32F);
    img_t result = _result;

#ifdef HAVE_TEGRA_OPTIMIZATION
    if (tegra::useTegra() && tegra::matchTemplate(img, templ, result, method))
        return;
#endif

    CC_IPP_RUN(true, ipp_matchTemplate(img, templ, result, method, cn))

    crossCorr( img, templ, result, result.size(), result->tid, Point(0,0), 0, 0);

    common_matchTemplate(img, templ, result, method, cn);
}

CC_IMPL void
cvMatchTemplate( const img_t* _img, const img_t* _templ, img_t* _result, int method )
{
    img_t img = cvarrToMat(_img), templ = cvarrToMat(_templ),
        result = cvarrToMat(_result);
    CC_Assert( result.size() == CSize(std::abs(img->cols - templ->cols) + 1,
                                         std::abs(img->rows - templ->rows) + 1) &&
              result->tid == CC_32F );
    matchTemplate(img, templ, result, method);
}


