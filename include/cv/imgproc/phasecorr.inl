/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2008-2011, William Lucas
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/


#include <vector>

namespace cv
{

static void magSpectrums( const img_t* _src, img_t* dst)
{
    img_t *src = _src;
    int depth = src CC_MAT_DEPTH(), cn = src CC_MAT_CN(), type = src->tid;
    int rows = src->rows, cols = src->cols;
    int j, k;

    CC_Assert( type == CC_32FC1 || type == CC_32FC2 || type == CC_64FC1 || type == CC_64FC2 );

    if(src CC_MAT_DEPTH() == CC_32F)
        _dst cvSetMat( src->rows, src->cols, CC_32FC1 );
    else
        _dst cvSetMat( src->rows, src->cols, CC_64FC1 );

    img_t *dst = _dst;
    dst.setTo(0);//img_t elements are not equal to zero by default!

    bool is_1d = (rows == 1 || (cols == 1 && src CC_IS_CONT_MAT() && dst CC_IS_CONT_MAT()));

    if( is_1d )
        cols = cols + rows - 1, rows = 1;

    int ncols = cols*cn;
    int j0 = cn == 1;
    int j1 = ncols - (cols % 2 == 0 && cn == 1);

    if( depth == CC_32F )
    {
        const float* dataSrc = src img_ptr(float, );
        float* dataDst = dst img_ptr(float, );

        size_t stepSrc = src->step/sizeof(dataSrc[0]);
        size_t stepDst = dst->step/sizeof(dataDst[0]);

        if( !is_1d && cn == 1 )
        {
            for( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                if( k == 1 )
                    dataSrc += cols - 1, dataDst += cols - 1;
                dataDst[0] = dataSrc[0]*dataSrc[0];
                if( rows % 2 == 0 )
                    dataDst[(rows-1)*stepDst] = dataSrc[(rows-1)*stepSrc]*dataSrc[(rows-1)*stepSrc];

                for( j = 1; j <= rows - 2; j += 2 )
                {
                    dataDst[j*stepDst] = (float)sqrt((double)dataSrc[j*stepSrc]*dataSrc[j*stepSrc] +
                                                          (double)dataSrc[(j+1)*stepSrc]*dataSrc[(j+1)*stepSrc]);
                }

                if( k == 1 )
                    dataSrc -= cols - 1, dataDst -= cols - 1;
            }
        }

        for( ; rows--; dataSrc += stepSrc, dataDst += stepDst )
        {
            if( is_1d && cn == 1 )
            {
                dataDst[0] = dataSrc[0]*dataSrc[0];
                if( cols % 2 == 0 )
                    dataDst[j1] = dataSrc[j1]*dataSrc[j1];
            }

            for( j = j0; j < j1; j += 2 )
            {
                dataDst[j] = (float)sqrt((double)dataSrc[j]*dataSrc[j] + (double)dataSrc[j+1]*dataSrc[j+1]);
            }
        }
    }
    else
    {
        const double* dataSrc = src img_ptr(double, );
        double* dataDst = dst img_ptr(double, );

        size_t stepSrc = src->step/sizeof(dataSrc[0]);
        size_t stepDst = dst->step/sizeof(dataDst[0]);

        if( !is_1d && cn == 1 )
        {
            for( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                if( k == 1 )
                    dataSrc += cols - 1, dataDst += cols - 1;
                dataDst[0] = dataSrc[0]*dataSrc[0];
                if( rows % 2 == 0 )
                    dataDst[(rows-1)*stepDst] = dataSrc[(rows-1)*stepSrc]*dataSrc[(rows-1)*stepSrc];

                for( j = 1; j <= rows - 2; j += 2 )
                {
                    dataDst[j*stepDst] = sqrt(dataSrc[j*stepSrc]*dataSrc[j*stepSrc] +
                                                   dataSrc[(j+1)*stepSrc]*dataSrc[(j+1)*stepSrc]);
                }

                if( k == 1 )
                    dataSrc -= cols - 1, dataDst -= cols - 1;
            }
        }

        for( ; rows--; dataSrc += stepSrc, dataDst += stepDst )
        {
            if( is_1d && cn == 1 )
            {
                dataDst[0] = dataSrc[0]*dataSrc[0];
                if( cols % 2 == 0 )
                    dataDst[j1] = dataSrc[j1]*dataSrc[j1];
            }

            for( j = j0; j < j1; j += 2 )
            {
                dataDst[j] = sqrt(dataSrc[j]*dataSrc[j] + dataSrc[j+1]*dataSrc[j+1]);
            }
        }
    }
}

static void divSpectrums( const img_t* _srcA, const img_t* _srcB, img_t* dst, int flags, bool conjB)
{
    img_t srcA = _srcA, srcB = _srcB;
    int depth = srcA CC_MAT_DEPTH(), cn = srcA CC_MAT_CN(), type = srcA->tid;
    int rows = srcA->rows, cols = srcA->cols;
    int j, k;

    CC_Assert( type == srcB->tid && srccvGetSize(A) == srcB.size() );
    CC_Assert( type == CC_32FC1 || type == CC_32FC2 || type == CC_64FC1 || type == CC_64FC2 );

    _dst cvSetMat( srcA->rows, srcA->cols, type );
    img_t *dst = _dst;

    CC_Assert(dst.data != srcA.data); // non-inplace check
    CC_Assert(dst.data != srcB.data); // non-inplace check

    bool is_1d = (flags & DFT_ROWS) || (rows == 1 || (cols == 1 &&
             srcA CC_IS_CONT_MAT() && srcB CC_IS_CONT_MAT() && dst CC_IS_CONT_MAT()));

    if( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    int ncols = cols*cn;
    int j0 = cn == 1;
    int j1 = ncols - (cols % 2 == 0 && cn == 1);

    if( depth == CC_32F )
    {
        const float* dataA = srcA img_ptr(float, );
        const float* dataB = srcB img_ptr(float, );
        float* dataC = dst img_ptr(float, );
        float eps = FLT_EPSILON; // prevent div0 problems

        size_t stepA = srcA->step/sizeof(dataA[0]);
        size_t stepB = srcB->step/sizeof(dataB[0]);
        size_t stepC = dst->step/sizeof(dataC[0]);

        if( !is_1d && cn == 1 )
        {
            for( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                if( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0] / (dataB[0] + eps);
                if( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA] / (dataB[(rows-1)*stepB] + eps);
                if( !conjB )
                    for( j = 1; j <= rows - 2; j += 2 )
                    {
                        double denom = (double)dataB[j*stepB]*dataB[j*stepB] +
                                       (double)dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + (double)eps;

                        double re = (double)dataA[j*stepA]*dataB[j*stepB] +
                                    (double)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        double im = (double)dataA[(j+1)*stepA]*dataB[j*stepB] -
                                    (double)dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = (float)(re / denom);
                        dataC[(j+1)*stepC] = (float)(im / denom);
                    }
                else
                    for( j = 1; j <= rows - 2; j += 2 )
                    {

                        double denom = (double)dataB[j*stepB]*dataB[j*stepB] +
                                       (double)dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + (double)eps;

                        double re = (double)dataA[j*stepA]*dataB[j*stepB] -
                                    (double)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        double im = (double)dataA[(j+1)*stepA]*dataB[j*stepB] +
                                    (double)dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = (float)(re / denom);
                        dataC[(j+1)*stepC] = (float)(im / denom);
                    }
                if( k == 1 )
                    dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        for( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            if( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0] / (dataB[0] + eps);
                if( cols % 2 == 0 )
                    dataC[j1] = dataA[j1] / (dataB[j1] + eps);
            }

            if( !conjB )
                for( j = j0; j < j1; j += 2 )
                {
                    double denom = (double)(dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps);
                    double re = (double)(dataA[j]*dataB[j] + dataA[j+1]*dataB[j+1]);
                    double im = (double)(dataA[j+1]*dataB[j] - dataA[j]*dataB[j+1]);
                    dataC[j] = (float)(re / denom);
                    dataC[j+1] = (float)(im / denom);
                }
            else
                for( j = j0; j < j1; j += 2 )
                {
                    double denom = (double)(dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps);
                    double re = (double)(dataA[j]*dataB[j] - dataA[j+1]*dataB[j+1]);
                    double im = (double)(dataA[j+1]*dataB[j] + dataA[j]*dataB[j+1]);
                    dataC[j] = (float)(re / denom);
                    dataC[j+1] = (float)(im / denom);
                }
        }
    }
    else
    {
        const double* dataA = srcA img_ptr(double, );
        const double* dataB = srcB img_ptr(double, );
        double* dataC = dst img_ptr(double, );
        double eps = DBL_EPSILON; // prevent div0 problems

        size_t stepA = srcA->step/sizeof(dataA[0]);
        size_t stepB = srcB->step/sizeof(dataB[0]);
        size_t stepC = dst->step/sizeof(dataC[0]);

        if( !is_1d && cn == 1 )
        {
            for( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                if( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0] / (dataB[0] + eps);
                if( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA] / (dataB[(rows-1)*stepB] + eps);
                if( !conjB )
                    for( j = 1; j <= rows - 2; j += 2 )
                    {
                        double denom = dataB[j*stepB]*dataB[j*stepB] +
                                       dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + eps;

                        double re = dataA[j*stepA]*dataB[j*stepB] +
                                    dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        double im = dataA[(j+1)*stepA]*dataB[j*stepB] -
                                    dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = re / denom;
                        dataC[(j+1)*stepC] = im / denom;
                    }
                else
                    for( j = 1; j <= rows - 2; j += 2 )
                    {
                        double denom = dataB[j*stepB]*dataB[j*stepB] +
                                       dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + eps;

                        double re = dataA[j*stepA]*dataB[j*stepB] -
                                    dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        double im = dataA[(j+1)*stepA]*dataB[j*stepB] +
                                    dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = re / denom;
                        dataC[(j+1)*stepC] = im / denom;
                    }
                if( k == 1 )
                    dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        for( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            if( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0] / (dataB[0] + eps);
                if( cols % 2 == 0 )
                    dataC[j1] = dataA[j1] / (dataB[j1] + eps);
            }

            if( !conjB )
                for( j = j0; j < j1; j += 2 )
                {
                    double denom = dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps;
                    double re = dataA[j]*dataB[j] + dataA[j+1]*dataB[j+1];
                    double im = dataA[j+1]*dataB[j] - dataA[j]*dataB[j+1];
                    dataC[j] = re / denom;
                    dataC[j+1] = im / denom;
                }
            else
                for( j = j0; j < j1; j += 2 )
                {
                    double denom = dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps;
                    double re = dataA[j]*dataB[j] - dataA[j+1]*dataB[j+1];
                    double im = dataA[j+1]*dataB[j] + dataA[j]*dataB[j+1];
                    dataC[j] = re / denom;
                    dataC[j+1] = im / denom;
                }
        }
    }

}

static void fftShift(InputCvArr* out)
{
    img_t out = _out;

    if(out->rows == 1 && out->cols == 1)
    {
        // trivially shifted.
        return;
    }

    std::vector<img_t> planes;
    split(out, planes);

    int xMid = out->cols >> 1;
    int yMid = out->rows >> 1;

    bool is_1d = xMid == 0 || yMid == 0;

    if(is_1d)
    {
        xMid = xMid + yMid;

        for(size_t i = 0; i < planes.size(); i++)
        {
            img_t tmp;
            img_t half0(planes[i], Rect(0, 0, xMid, 1));
            img_t half1(planes[i], Rect(xMid, 0, xMid, 1));

            half0.copyTo(tmp);
            half1.copyTo(half0);
            tmp.copyTo(half1);
        }
    }
    else
    {
        for(size_t i = 0; i < planes.size(); i++)
        {
            // perform quadrant swaps...
            img_t tmp;
            img_t q0(planes[i], Rect(0,    0,    xMid, yMid));
            img_t q1(planes[i], Rect(xMid, 0,    xMid, yMid));
            img_t q2(planes[i], Rect(0,    yMid, xMid, yMid));
            img_t q3(planes[i], Rect(xMid, yMid, xMid, yMid));

            q0.copyTo(tmp);
            q3.copyTo(q0);
            tmp.copyTo(q3);

            q1.copyTo(tmp);
            q2.copyTo(q1);
            tmp.copyTo(q2);
        }
    }

    merge(planes, out);
}

static CPoint2d weightedCentroid(const img_t* _src, Point peakLocation, CSize weightBoxSize, double* response)
{
    img_t *src = _src;

    int type = src->tid;
    CC_Assert( type == CC_32FC1 || type == CC_64FC1 );

    int minr = peakLocation.y - (weightBoxSize.height >> 1);
    int maxr = peakLocation.y + (weightBoxSize.height >> 1);
    int minc = peakLocation.x - (weightBoxSize.width  >> 1);
    int maxc = peakLocation.x + (weightBoxSize.width  >> 1);

    CPoint2d centroid;
    double sumIntensity = 0.0;

    // clamp the values to min and max if needed.
    if(minr < 0)
    {
        minr = 0;
    }

    if(minc < 0)
    {
        minc = 0;
    }

    if(maxr > src->rows - 1)
    {
        maxr = src->rows - 1;
    }

    if(maxc > src->cols - 1)
    {
        maxc = src->cols - 1;
    }

    if(type == CC_32FC1)
    {
        const float* dataIn = src img_ptr(float, );
        dataIn += minr*src->cols;
        for(int y = minr; y <= maxr; y++)
        {
            for(int x = minc; x <= maxc; x++)
            {
                centroid.x   += (double)x*dataIn[x];
                centroid.y   += (double)y*dataIn[x];
                sumIntensity += (double)dataIn[x];
            }

            dataIn += src->cols;
        }
    }
    else
    {
        const double* dataIn = src img_ptr(double, );
        dataIn += minr*src->cols;
        for(int y = minr; y <= maxr; y++)
        {
            for(int x = minc; x <= maxc; x++)
            {
                centroid.x   += (double)x*dataIn[x];
                centroid.y   += (double)y*dataIn[x];
                sumIntensity += dataIn[x];
            }

            dataIn += src->cols;
        }
    }

    if(response)
        *response = sumIntensity;

    sumIntensity += DBL_EPSILON; // prevent div0 problems...

    centroid.x /= sumIntensity;
    centroid.y /= sumIntensity;

    return centroid;
}

}

CPoint2d phaseCorrelate(const img_t* _src1, const img_t* _src2, const img_t* _window, double* response)
{
    CC_INSTRUMENT_REGION()

    img_t src1 = _src1;
    img_t src2 = _src2;
    img_t window = _window;

    CC_Assert( src1->tid == src2->tid);
    CC_Assert( src1->tid == CC_32FC1 || src1->tid == CC_64FC1 );
    CC_Assert( src1.size == src2.size);

    if(!window.empty())
    {
        CC_Assert( src1->tid == window->tid);
        CC_Assert( src1.size == window.size);
    }

    int M = getOptimalDFTSize(src1->rows);
    int N = getOptimalDFTSize(src1->cols);

    img_t padded1, padded2, paddedWin;

    if(M != src1->rows || N != src1->cols)
    {
        copyMakeBorder(src1, padded1, 0, M - src1->rows, 0, N - src1->cols, CC_BORDER_CONSTANT, Scalar::all(0));
        copyMakeBorder(src2, padded2, 0, M - src2->rows, 0, N - src2->cols, CC_BORDER_CONSTANT, Scalar::all(0));

        if(!window.empty())
        {
            copyMakeBorder(window, paddedWin, 0, M - window->rows, 0, N - window->cols, CC_BORDER_CONSTANT, Scalar::all(0));
        }
    }
    else
    {
        padded1 = src1;
        padded2 = src2;
        paddedWin = window;
    }

    img_t FFT1, FFT2, P, Pm, C;

    // perform window multiplication if available
    if(!paddedWin.empty())
    {
        // apply window to both images before proceeding...
        multiply(paddedWin, padded1, padded1);
        multiply(paddedWin, padded2, padded2);
    }

    // execute phase correlation equation
    // Reference: http://en.wikipedia.org/wiki/Phase_correlation
    dft(padded1, FFT1, DFT_REAL_OUTPUT);
    dft(padded2, FFT2, DFT_REAL_OUTPUT);

    mulSpectrums(FFT1, FFT2, P, 0, true);

    magSpectrums(P, Pm);
    divSpectrums(P, Pm, C, 0, false); // FF* / |FF*| (phase correlation equation completed here...)

    idft(C, C); // gives us the nice peak shift location...

    fftShift(C); // shift the energy to the center of the frame.

    // locate the highest peak
    Point peakLoc;
    minMaxLoc(C, NULL, NULL, NULL, &peakLoc);

    // get the phase shift with sub-pixel accuracy, 5x5 window seems about right here...
    CPoint2d t;
    t = weightedCentroid(C, peakLoc, CSize(5, 5), response);

    // max response is M*N (not exactly, might be slightly larger due to rounding errors)
    if(response)
        *response /= M*N;

    // adjust shift relative to image center...
    CPoint2d center((double)padded1->cols / 2.0, (double)padded1->rows / 2.0);

    return (center - t);
}


void createHanningWindow(img_t* dst, CSize winSize, int type)
{
    CC_INSTRUMENT_REGION()

    CC_Assert( type == CC_32FC1 || type == CC_64FC1 );

    _dst cvSetMat(winSize, type);
    img_t *dst = _dst;

    int rows = dst->rows, cols = dst->cols;

    FREE(); double* = MALLOC(double, ) _wc(cols);
    double * const wc = (double *)_wc;

    double coeff0 = 2.0 * CC_PI / (double)(cols - 1), coeff1 = 2.0f * CC_PI / (double)(rows - 1);
    for(int j = 0; j < cols; j++)
        wc[j] = 0.5 * (1.0 - cos(coeff0 * j));

    if(dst CC_MAT_DEPTH() == CC_32F)
    {
        for(int i = 0; i < rows; i++)
        {
            float* dstData = dst.ptr<float>(i);
            double wr = 0.5 * (1.0 - cos(coeff1 * i));
            for(int j = 0; j < cols; j++)
                dstData[j] = (float)(wr * wc[j]);
        }
    }
    else
    {
        for(int i = 0; i < rows; i++)
        {
            double* dstData = dst.ptr<double>(i);
            double wr = 0.5 * (1.0 - cos(coeff1 * i));
            for(int j = 0; j < cols; j++)
                dstData[j] = wr * wc[j];
        }
    }

    // perform batch sqrt for SSE performance gains
    sqrt(dst, dst);
}
