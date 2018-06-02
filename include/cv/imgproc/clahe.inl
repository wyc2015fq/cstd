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
// Copyright (C) 2013, NVIDIA Corporation, all rights reserved.
// Copyright (C) 2014, Itseez Inc., all rights reserved.
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
// In no event shall the copyright holders or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/


#include "opencl_kernels_imgproc.hpp"

// ----------------------------------------------------------------------
// CLAHE

#ifdef HAVE_OPENCL

namespace clahe
{
    static bool calcLut(const img_t* _src, img_t* dst,
        const int tilesX, const int tilesY, const CSize tileSize,
        const int clipLimit, const float lutScale)
    {
        ocl::Kernel _k("calcLut", ocl::imgproc::clahe_oclsrc);

        bool is_cpu = ocl::Device::getDefault()->tid == ocl::Device::TYPE_CPU;
        String opts;
        if(is_cpu)
            opts = "-D CPU ";
        else
            opts = format("-D WAVE_SIZE=%d", _k.preferedWorkGroupSizeMultiple());

        ocl::Kernel k("calcLut", ocl::imgproc::clahe_oclsrc, opts);
        if(k.empty())
            return false;

        UMat src = _src.getUMat();
        _dst cvSetMat(tilesX * tilesY, 256, CC_8UC1);
        UMat dst = _dst.getUMat();

        int tile_size[2];
        tile_size[0] = tileSize.width;
        tile_size[1] = tileSize.height;

        size_t localThreads[3]  = { 32, 8, 1 };
        size_t globalThreads[3] = { tilesX * localThreads[0], tilesY * localThreads[1], 1 };

        int idx = 0;
        idx = k.set(idx, ocl::KernelArg::ReadOnlyNoSize(src));
        idx = k.set(idx, ocl::KernelArg::WriteOnlyNoSize(dst));
        idx = k.set(idx, tile_size);
        idx = k.set(idx, tilesX);
        idx = k.set(idx, clipLimit);
        k.set(idx, lutScale);

        return k.run(2, globalThreads, localThreads, false);
    }

    static bool transform(const img_t* _src, img_t* dst, const img_t* _lut,
        const int tilesX, const int tilesY, const CSize & tileSize)
    {

        ocl::Kernel k("transform", ocl::imgproc::clahe_oclsrc);
        if(k.empty())
            return false;

        int tile_size[2];
        tile_size[0] = tileSize.width;
        tile_size[1] = tileSize.height;

        UMat src = _src.getUMat();
        _dst cvSetMat(cvGetSize(src), src->tid);
        UMat dst = _dst.getUMat();
        UMat lut = _lut.getUMat();

        size_t localThreads[3]  = { 32, 8, 1 };
        size_t globalThreads[3] = { (size_t)src->cols, (size_t)src->rows, 1 };

        int idx = 0;
        idx = k.set(idx, ocl::KernelArg::ReadOnlyNoSize(src));
        idx = k.set(idx, ocl::KernelArg::WriteOnlyNoSize(dst));
        idx = k.set(idx, ocl::KernelArg::ReadOnlyNoSize(lut));
        idx = k.set(idx, src->cols);
        idx = k.set(idx, src->rows);
        idx = k.set(idx, tile_size);
        idx = k.set(idx, tilesX);
        k.set(idx, tilesY);

        return k.run(2, globalThreads, localThreads, false);
    }
}

#endif

namespace
{
    template <class T, int histSize, int shift>
    class CLAHE_CalcLut_Body : public ParallelLoopBody
    {
    public:
        CLAHE_CalcLut_Body(const img_t& src, const img_t& lut, const CSize& tileSize, const int& tilesX, const int& clipLimit, const float& lutScale) :
            src_(src), lut_(lut), tileSize_(tileSize), tilesX_(tilesX), clipLimit_(clipLimit), lutScale_(lutScale)
        {
        }

        void operator ()(const CRange& range) const;

    private:
        img_t src_;
        mutable img_t lut_;

        CSize tileSize_;
        int tilesX_;
        int clipLimit_;
        float lutScale_;
    };

    template <class T, int histSize, int shift>
    void CLAHE_CalcLut_Body<T,histSize,shift>::operator ()(const CRange& range) const
    {
        T* tileLut = lut_.ptr<T>(range.start);
        const size_t lut_step = lut_->step / sizeof(T);

        for (int k = range.start; k < range.end; ++k, tileLut += lut_step)
        {
            const int ty = k / tilesX_;
            const int tx = k % tilesX_;

            // retrieve tile submatrix

            Rect tileROI;
            tileROI.x = tx * tileSize_.width;
            tileROI.y = ty * tileSize_.height;
            tileROI.width = tileSize_.width;
            tileROI.height = tileSize_.height;

            const img_t tile = src_(tileROI);

            // calc histogram

            int tileHist[histSize] = {0, };

            int height = tileROI.height;
            const size_t sstep = src_->step / sizeof(T);
            for (const T* ptr = tile.ptr<T>(0); height--; ptr += sstep)
            {
                int x = 0;
                for (; x <= tileROI.width - 4; x += 4)
                {
                    int t0 = ptr[x], t1 = ptr[x+1];
                    tileHist[t0 >> shift]++; tileHist[t1 >> shift]++;
                    t0 = ptr[x+2]; t1 = ptr[x+3];
                    tileHist[t0 >> shift]++; tileHist[t1 >> shift]++;
                }

                for (; x < tileROI.width; ++x)
                    tileHist[ptr[x] >> shift]++;
            }

            // clip histogram

            if (clipLimit_ > 0)
            {
                // how many pixels were clipped
                int clipped = 0;
                for (int i = 0; i < histSize; ++i)
                {
                    if (tileHist[i] > clipLimit_)
                    {
                        clipped += tileHist[i] - clipLimit_;
                        tileHist[i] = clipLimit_;
                    }
                }

                // redistribute clipped pixels
                int redistBatch = clipped / histSize;
                int residual = clipped - redistBatch * histSize;

                for (int i = 0; i < histSize; ++i)
                    tileHist[i] += redistBatch;

                if (residual != 0)
                {
                    int residualStep = MAX(histSize / residual, 1);
                    for (int i = 0; i < histSize && residual > 0; i += residualStep, residual--)
                        tileHist[i]++;
                }
            }

            // calc Lut

            int sum = 0;
            for (int i = 0; i < histSize; ++i)
            {
                sum += tileHist[i];
                tileLut[i] = saturate_cast<T>(sum * lutScale_);
            }
        }
    }

    template <class T, int shift>
    class CLAHE_Interpolation_Body : public ParallelLoopBody
    {
    public:
        CLAHE_Interpolation_Body(const img_t& src, const img_t& dst, const img_t& lut, const CSize& tileSize, const int& tilesX, const int& tilesY) :
            src_(src), dst_(dst), lut_(lut), tileSize_(tileSize), tilesX_(tilesX), tilesY_(tilesY)
        {
            buf.allocate(src->cols << 2);
            ind1_p = (int *)buf;
            ind2_p = ind1_p + src->cols;
            xa_p = (float *)(ind2_p + src->cols);
            xa1_p = xa_p + src->cols;

            int lut_step = static_cast<int>(lut_->step / sizeof(T));
            float inv_tw = 1.0f / tileSize_.width;

            for (int x = 0; x < src->cols; ++x)
            {
                float txf = x * inv_tw - 0.5f;

                int tx1 = cFloor(txf);
                int tx2 = tx1 + 1;

                xa_p[x] = txf - tx1;
                xa1_p[x] = 1.0f - xa_p[x];

                tx1 = MAX(tx1, 0);
                tx2 = MIN(tx2, tilesX_ - 1);

                ind1_p[x] = tx1 * lut_step;
                ind2_p[x] = tx2 * lut_step;
            }
        }

        void operator ()(const CRange& range) const;

    private:
        img_t src_;
        mutable img_t dst_;
        img_t lut_;

        CSize tileSize_;
        int tilesX_;
        int tilesY_;

        FREE(); int* = MALLOC(int, ) buf;
        int * ind1_p, * ind2_p;
        float * xa_p, * xa1_p;
    };

    template <class T, int shift>
    void CLAHE_Interpolation_Body<T, shift>::operator ()(const CRange& range) const
    {
        float inv_th = 1.0f / tileSize_.height;

        for (int y = range.start; y < range.end; ++y)
        {
            const T* srcRow = src_.ptr<T>(y);
            T* dstRow = dst_.ptr<T>(y);

            float tyf = y * inv_th - 0.5f;

            int ty1 = cFloor(tyf);
            int ty2 = ty1 + 1;

            float ya = tyf - ty1, ya1 = 1.0f - ya;

            ty1 = MAX(ty1, 0);
            ty2 = MIN(ty2, tilesY_ - 1);

            const T* lutPlane1 = lut_.ptr<T>(ty1 * tilesX_);
            const T* lutPlane2 = lut_.ptr<T>(ty2 * tilesX_);

            for (int x = 0; x < src_->cols; ++x)
            {
                int srcVal = srcRow[x] >> shift;

                int ind1 = ind1_p[x] + srcVal;
                int ind2 = ind2_p[x] + srcVal;

                float res = (lutPlane1[ind1] * xa1_p[x] + lutPlane1[ind2] * xa_p[x]) * ya1 +
                            (lutPlane2[ind1] * xa1_p[x] + lutPlane2[ind2] * xa_p[x]) * ya;

                dstRow[x] = saturate_cast<T>(res) << shift;
            }
        }
    }

    class CLAHE_Impl : public CLAHE
    {
    public:
        CLAHE_Impl(double clipLimit = 40.0, int tilesX = 8, int tilesY = 8);

        void apply(const img_t* src, img_t* dst);

        void setClipLimit(double clipLimit);
        double getClipLimit() const;

        void setTilesGridSize(CSize tileGridSize);
        CSize getTilesGridSize() const;

        void collectGarbage();

    private:
        double clipLimit_;
        int tilesX_;
        int tilesY_;

        img_t srcExt_;
        img_t lut_;

#ifdef HAVE_OPENCL
        UMat usrcExt_;
        UMat ulut_;
#endif
    };

    CLAHE_Impl::CLAHE_Impl(double clipLimit, int tilesX, int tilesY) :
        clipLimit_(clipLimit), tilesX_(tilesX), tilesY_(tilesY)
    {
    }

    void CLAHE_Impl::apply(const img_t* _src, img_t* dst)
    {
        CC_INSTRUMENT_REGION()

        CC_Assert( _src->tid == CC_8UC1 || _src->tid == CC_16UC1 );

#ifdef HAVE_OPENCL
        bool useOpenCL = ocl::useOpenCL() && _src.isUMat() && _src.dims()<=2 && _src->tid == CC_8UC1;
#endif

        int histSize = _src->tid == CC_8UC1 ? 256 : 65536;

        CSize tileSize;
        _InputArray _srcForLut;

        if (_cvGetSize(src).width % tilesX_ == 0 && _cvGetSize(src).height % tilesY_ == 0)
        {
            tileSize = CSize(_cvGetSize(src).width / tilesX_, _cvGetSize(src).height / tilesY_);
            _srcForLut = _src;
        }
        else
        {
#ifdef HAVE_OPENCL
            if(useOpenCL)
            {
                copyMakeBorder(_src, usrcExt_, 0, tilesY_ - (_cvGetSize(src).height % tilesY_), 0, tilesX_ - (_cvGetSize(src).width % tilesX_), CC_BORDER_REFLECT_101);
                tileSize = CSize(usrcExt_.size().width / tilesX_, usrcExt_.size().height / tilesY_);
                _srcForLut = usrcExt_;
            }
            else
#endif
            {
                copyMakeBorder(_src, srcExt_, 0, tilesY_ - (_cvGetSize(src).height % tilesY_), 0, tilesX_ - (_cvGetSize(src).width % tilesX_), CC_BORDER_REFLECT_101);
                tileSize = CSize(srcExt_.size().width / tilesX_, srcExt_.size().height / tilesY_);
                _srcForLut = srcExt_;
            }
        }

        const int tileSizeTotal = tileSize.area();
        const float lutScale = static_cast<float>(histSize - 1) / tileSizeTotal;

        int clipLimit = 0;
        if (clipLimit_ > 0.0)
        {
            clipLimit = static_cast<int>(clipLimit_ * tileSizeTotal / histSize);
            clipLimit = MAX(clipLimit, 1);
        }

#ifdef HAVE_OPENCL
        if (useOpenCL && clahe::calcLut(_srcForLut, ulut_, tilesX_, tilesY_, tileSize, clipLimit, lutScale) )
            if( clahe::transform(_src, _dst, ulut_, tilesX_, tilesY_, tileSize) )
            {
                CC_IMPL_ADD(CC_IMPL_OCL);
                return;
            }
#endif

        img_t *src = _src;
        _dst cvSetMat( cvGetSize(src), src->tid );
        img_t *dst = _dst;
        img_t srcForLut = _srcForLut;
        lut_ cvSetMat(tilesX_ * tilesY_, histSize, _src->tid);

        Ptr<ParallelLoopBody> calcLutBody;
        if (_src->tid == CC_8UC1)
            calcLutBody = makePtr<CLAHE_CalcLut_Body<uchar, 256, 0> >(srcForLut, lut_, tileSize, tilesX_, clipLimit, lutScale);
        else if (_src->tid == CC_16UC1)
            calcLutBody = makePtr<CLAHE_CalcLut_Body<ushort, 65536, 0> >(srcForLut, lut_, tileSize, tilesX_, clipLimit, lutScale);
        else
            CC_Error( CC_StsBadArg, "Unsupported type" );

        parallel_for_(CRange(0, tilesX_ * tilesY_), *calcLutBody);

        Ptr<ParallelLoopBody> interpolationBody;
        if (_src->tid == CC_8UC1)
            interpolationBody = makePtr<CLAHE_Interpolation_Body<uchar, 0> >(src, dst, lut_, tileSize, tilesX_, tilesY_);
        else if (_src->tid == CC_16UC1)
            interpolationBody = makePtr<CLAHE_Interpolation_Body<ushort, 0> >(src, dst, lut_, tileSize, tilesX_, tilesY_);

        parallel_for_(CRange(0, src->rows), *interpolationBody);
    }

    void CLAHE_Impl::setClipLimit(double clipLimit)
    {
        clipLimit_ = clipLimit;
    }

    double CLAHE_Impl::getClipLimit() const
    {
        return clipLimit_;
    }

    void CLAHE_Impl::setTilesGridSize(CSize tileGridSize)
    {
        tilesX_ = tileGridSize.width;
        tilesY_ = tileGridSize.height;
    }

    CSize CLAHE_Impl::getTilesGridSize() const
    {
        return CSize(tilesX_, tilesY_);
    }

    void CLAHE_Impl::collectGarbage()
    {
        srcExt_.release();
        lut_.release();
#ifdef HAVE_OPENCL
        usrcExt_.release();
        ulut_.release();
#endif
    }
}

Ptr<CLAHE> createCLAHE(double clipLimit, CSize tileGridSize)
{
    return makePtr<CLAHE_Impl>(clipLimit, tileGridSize.width, tileGridSize.height);
}
