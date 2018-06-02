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
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Copyright (C) 2014, Itseez, Inc, all rights reserved.
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

// Classical Hough Transform
struct LinePolar
{
    float rho;
    float angle;
};


struct hough_cmp_gt
{
    hough_cmp_gt(const int* _aux) : aux(_aux) {}
    bool operator()(int l1, int l2) const
    {
        return aux[l1] > aux[l2] || (aux[l1] == aux[l2] && l1 < l2);
    }
    const int* aux;
};


/*
Here image is an input raster;
step is it's step; size characterizes it's ROI;
rho and theta are discretization steps (in pixels and radians correspondingly).
threshold is the minimum number of pixels in the feature for it
to be a candidate for line. lines is the output
array of (rho, theta) pairs. linesMax is the buffer size (number of pairs).
Functions return the actual number of found lines.
*/
static void
HoughLinesStandard( const img_t& img, float rho, float theta,
                    int threshold, std::vector<Vec2f>& lines, int linesMax,
                    double min_theta, double max_theta )
{
    int i, j;
    float irho = 1 / rho;

    CC_Assert( img->tid == CC_8UC1 );

    const uchar* image = img->tt.data;
    int step = (int)img->step;
    int width = img->cols;
    int height = img->rows;

    if (max_theta < min_theta ) {
        CC_Error( CC_StsBadArg, "max_theta must be greater than min_theta" );
    }
    int numangle = cRound((max_theta - min_theta) / theta);
    int numrho = cRound(((width + height) * 2 + 1) / rho);

#if defined HAVE_IPP && IPP_VERSION_X100 >= 810 && IPP_DISABLE_BLOCK
    CC_IPP_CHECK()
    {
        IppiSize srcSize = { width, height };
        IppPointPolar delta = { rho, theta };
        IppPointPolar dstRoi[2] = {{(Ipp32f) -(width + height), (Ipp32f) min_theta},{(Ipp32f) (width + height), (Ipp32f) max_theta}};
        int bufferSize;
        int nz = countNonZero(img);
        int ipp_linesMax = MIN(linesMax, nz*numangle/threshold);
        int linesCount = 0;
        lines.resize(ipp_linesMax);
        IppStatus ok = ippiHoughLineGetSize_8u_C1R(srcSize, delta, ipp_linesMax, &bufferSize);
        Ipp8u* buffer = ippsMalloc_8u(bufferSize);
        if (ok >= 0) {ok = CC_INSTRUMENT_FUN_IPP(ippiHoughLine_Region_8u32f_C1R,(image, step, srcSize, (IppPointPolar*) &lines[0], dstRoi, ipp_linesMax, &linesCount, delta, threshold, buffer))};
        ippsFree(buffer);
        if (ok >= 0)
        {
            lines.resize(linesCount);
            CC_IMPL_ADD(CC_IMPL_IPP);
            return;
        }
        lines.clear();
        setIppErrorStatus();
    }
#endif

    FREE(); int* = MALLOC(int, ) _accum((numangle+2) * (numrho+2));
    std::vector<int> _sort_buf;
    FREE(); float* = MALLOC(float, ) _tabSin(numangle);
    FREE(); float* = MALLOC(float, ) _tabCos(numangle);
    int *accum = _accum;
    float *tabSin = _tabSin, *tabCos = _tabCos;

    memset( accum, 0, sizeof(accum[0]) * (numangle+2) * (numrho+2) );

    float ang = static_cast<float>(min_theta);
    for(int n = 0; n < numangle; ang += theta, n++ )
    {
        tabSin[n] = (float)(sin((double)ang) * irho);
        tabCos[n] = (float)(cos((double)ang) * irho);
    }

    // stage 1. fill accumulator
    for( i = 0; i < height; i++ )
        for( j = 0; j < width; j++ )
        {
            if( image[i * step + j] != 0 )
                for(int n = 0; n < numangle; n++ )
                {
                    int r = cRound( j * tabCos[n] + i * tabSin[n] );
                    r += (numrho - 1) / 2;
                    accum[(n+1) * (numrho+2) + r+1]++;
                }
        }

    // stage 2. find local maximums
    for(int r = 0; r < numrho; r++ )
        for(int n = 0; n < numangle; n++ )
        {
            int base = (n+1) * (numrho+2) + r+1;
            if( accum[base] > threshold &&
                accum[base] > accum[base - 1] && accum[base] >= accum[base + 1] &&
                accum[base] > accum[base - numrho - 2] && accum[base] >= accum[base + numrho + 2] )
                _sort_buf.push_back(base);
        }

    // stage 3. sort the detected lines by accumulator value
    std::sort(_sort_buf.begin(), _sort_buf.end(), hough_cmp_gt(accum));

    // stage 4. store the first min(total,linesMax) lines to the output buffer
    linesMax = MIN(linesMax, (int)_sort_buf.size());
    double scale = 1./(numrho+2);
    for( i = 0; i < linesMax; i++ )
    {
        LinePolar line;
        int idx = _sort_buf[i];
        int n = cFloor(idx*scale) - 1;
        int r = idx - (n+1)*(numrho+2) - 1;
        line.rho = (r - (numrho - 1)*0.5f) * rho;
        line.angle = static_cast<float>(min_theta) + n * theta;
        lines.push_back(Vec2f(line.rho, line.angle));
    }
}


// Multi-Scale variant of Classical Hough Transform

struct hough_index
{
    hough_index() : value(0), rho(0.f), theta(0.f) {}
    hough_index(int _val, float _rho, float _theta)
    : value(_val), rho(_rho), theta(_theta) {}

    int value;
    float rho, theta;
};


static void
HoughLinesSDiv( const img_t& img,
                float rho, float theta, int threshold,
                int srn, int stn,
                std::vector<Vec2f>& lines, int linesMax,
                double min_theta, double max_theta )
{
    #define _POINT(row, column)\
        (image_src[(row)*step+(column)])

    int index, i;
    int ri, ti, ti1, ti0;
    int row, col;
    float r, t;                 /* Current rho and theta */
    float rv;                   /* Some temporary rho value */

    int fn = 0;
    float xc, yc;

    const float d2r = (float)(CC_PI / 180);
    int sfn = srn * stn;
    int fi;
    int count;
    int cmax = 0;

    std::vector<hough_index> lst;

    CC_Assert( img->tid == CC_8UC1 );
    CC_Assert( linesMax > 0 );

    threshold = MIN( threshold, 255 );

    const uchar* image_src = img->tt.data;
    int step = (int)img->step;
    int w = img->cols;
    int h = img->rows;

    float irho = 1 / rho;
    float itheta = 1 / theta;
    float srho = rho / srn;
    float stheta = theta / stn;
    float isrho = 1 / srho;
    float istheta = 1 / stheta;

    int rn = cFloor( sqrt( (double)w * w + (double)h * h ) * irho );
    int tn = cFloor( 2 * CC_PI * itheta );

    lst.push_back(hough_index(threshold, -1.f, 0.f));

    // Precalculate sin table
    std::vector<float> _sinTable( 5 * tn * stn );
    float* sinTable = &_sinTable[0];

    for( index = 0; index < 5 * tn * stn; index++ )
        sinTable[index] = (float)cos( stheta * index * 0.2f );

    std::vector<uchar> _caccum(rn * tn, (uchar)0);
    uchar* caccum = &_caccum[0];

    // Counting all feature pixels
    for( row = 0; row < h; row++ )
        for( col = 0; col < w; col++ )
            fn += _POINT( row, col ) != 0;

    std::vector<int> _x(fn), _y(fn);
    int* x = &_x[0], *y = &_y[0];

    // Full Hough Transform (it's accumulator update part)
    fi = 0;
    for( row = 0; row < h; row++ )
    {
        for( col = 0; col < w; col++ )
        {
            if( _POINT( row, col ))
            {
                int halftn;
                float r0;
                float scale_factor;
                int iprev = -1;
                float phi, phi1;
                float theta_it;     // Value of theta for iterating

                // Remember the feature point
                x[fi] = col;
                y[fi] = row;
                fi++;

                yc = (float) row + 0.5f;
                xc = (float) col + 0.5f;

                /* Update the accumulator */
                t = (float) fabs( cFastArctan( yc, xc ) * d2r );
                r = (float) sqrt( (double)xc * xc + (double)yc * yc );
                r0 = r * irho;
                ti0 = cFloor( (t + CC_PI*0.5) * itheta );

                caccum[ti0]++;

                theta_it = rho / r;
                theta_it = theta_it < theta ? theta_it : theta;
                scale_factor = theta_it * itheta;
                halftn = cFloor( CC_PI / theta_it );
                for( ti1 = 1, phi = theta_it - (float)(CC_PI*0.5), phi1 = (theta_it + t) * itheta;
                     ti1 < halftn; ti1++, phi += theta_it, phi1 += scale_factor )
                {
                    rv = r0 * std::cos( phi );
                    i = (int)rv * tn;
                    i += cFloor( phi1 );
                    assert( i >= 0 );
                    assert( i < rn * tn );
                    caccum[i] = (uchar) (caccum[i] + ((i ^ iprev) != 0));
                    iprev = i;
                    if( cmax < caccum[i] )
                        cmax = caccum[i];
                }
            }
        }
    }

    // Starting additional analysis
    count = 0;
    for( ri = 0; ri < rn; ri++ )
    {
        for( ti = 0; ti < tn; ti++ )
        {
            if( caccum[ri * tn + ti] > threshold )
                count++;
        }
    }

    if( count * 100 > rn * tn )
    {
        HoughLinesStandard( img, rho, theta, threshold, lines, linesMax, min_theta, max_theta );
        return;
    }

    std::vector<uchar> _buffer(srn * stn + 2);
    uchar* buffer = &_buffer[0];
    uchar* mcaccum = buffer + 1;

    count = 0;
    for( ri = 0; ri < rn; ri++ )
    {
        for( ti = 0; ti < tn; ti++ )
        {
            if( caccum[ri * tn + ti] > threshold )
            {
                count++;
                memset( mcaccum, 0, sfn * sizeof( uchar ));

                for( index = 0; index < fn; index++ )
                {
                    int ti2;
                    float r0;

                    yc = (float) y[index] + 0.5f;
                    xc = (float) x[index] + 0.5f;

                    // Update the accumulator
                    t = (float) fabs( cFastArctan( yc, xc ) * d2r );
                    r = (float) sqrt( (double)xc * xc + (double)yc * yc ) * isrho;
                    ti0 = cFloor( (t + CC_PI * 0.5) * istheta );
                    ti2 = (ti * stn - ti0) * 5;
                    r0 = (float) ri *srn;

                    for( ti1 = 0; ti1 < stn; ti1++, ti2 += 5 )
                    {
                        rv = r * sinTable[(int) (std::abs( ti2 ))] - r0;
                        i = cFloor( rv ) * stn + ti1;

                        i = CC_IMAX( i, -1 );
                        i = CC_IMIN( i, sfn );
                        mcaccum[i]++;
                        assert( i >= -1 );
                        assert( i <= sfn );
                    }
                }

                // Find peaks in maccum...
                for( index = 0; index < sfn; index++ )
                {
                    i = 0;
                    int pos = (int)(lst.size() - 1);
                    if( pos < 0 || lst[pos].value < mcaccum[index] )
                    {
                        hough_index vi(mcaccum[index],
                                       index / stn * srho + ri * rho,
                                       index % stn * stheta + ti * theta - (float)(CC_PI*0.5));
                        lst.push_back(vi);
                        for( ; pos >= 0; pos-- )
                        {
                            if( lst[pos].value > vi.value )
                                break;
                            lst[pos+1] = lst[pos];
                        }
                        lst[pos+1] = vi;
                        if( (int)lst.size() > linesMax )
                            lst.pop_back();
                    }
                }
            }
        }
    }

    for( size_t idx = 0; idx < lst.size(); idx++ )
    {
        if( lst[idx].rho < 0 )
            continue;
        lines.push_back(Vec2f(lst[idx].rho, lst[idx].theta));
    }
}


/****************************************************************************************\
*                              Probabilistic Hough Transform                             *
\****************************************************************************************/

static void
HoughLinesProbabilistic( img_t& image,
                         float rho, float theta, int threshold,
                         int lineLength, int lineGap,
                         std::vector<Vec4i>& lines, int linesMax )
{
    Point pt;
    float irho = 1 / rho;
    RNG rng((uint64)-1);

    CC_Assert( image->tid == CC_8UC1 );

    int width = image->cols;
    int height = image->rows;

    int numangle = cRound(CC_PI / theta);
    int numrho = cRound(((width + height) * 2 + 1) / rho);

#if defined HAVE_IPP && IPP_VERSION_X100 >= 810 && IPP_DISABLE_BLOCK
    CC_IPP_CHECK()
    {
        IppiSize srcSize = { width, height };
        IppPointPolar delta = { rho, theta };
        IppiHoughProbSpec* pSpec;
        int bufferSize, specSize;
        int ipp_linesMax = MIN(linesMax, numangle*numrho);
        int linesCount = 0;
        lines.resize(ipp_linesMax);
        IppStatus ok = ippiHoughProbLineGetSize_8u_C1R(srcSize, delta, &specSize, &bufferSize);
        Ipp8u* buffer = ippsMalloc_8u(bufferSize);
        pSpec = (IppiHoughProbSpec*) malloc(specSize);
        if (ok >= 0) ok = ippiHoughProbLineInit_8u32f_C1R(srcSize, delta, ippAlgHintNone, pSpec);
        if (ok >= 0) {ok = CC_INSTRUMENT_FUN_IPP(ippiHoughProbLine_8u32f_C1R,(image.data, image->step, srcSize, threshold, lineLength, lineGap, (IppiPoint*) &lines[0], ipp_linesMax, &linesCount, buffer, pSpec))};

        free(pSpec);
        ippsFree(buffer);
        if (ok >= 0)
        {
            lines.resize(linesCount);
            CC_IMPL_ADD(CC_IMPL_IPP);
            return;
        }
        lines.clear();
        setIppErrorStatus();
    }
#endif

    img_t accum = img_t::zeros( numangle, numrho, CC_32SC1 );
    img_t mask( height, width, CC_8UC1 );
    std::vector<float> trigtab(numangle*2);

    for( int n = 0; n < numangle; n++ )
    {
        trigtab[n*2] = (float)(cos((double)n*theta) * irho);
        trigtab[n*2+1] = (float)(sin((double)n*theta) * irho);
    }
    const float* ttab = &trigtab[0];
    uchar* mdata0 = mask->tt.data;
    std::vector<Point> nzloc;

    // stage 1. collect non-zero image points
    for( pt.y = 0; pt.y < height; pt.y++ )
    {
        const uchar* data = image.ptr(pt.y);
        uchar* mdata = mask.ptr(pt.y);
        for( pt.x = 0; pt.x < width; pt.x++ )
        {
            if( data[pt.x] )
            {
                mdata[pt.x] = (uchar)1;
                nzloc.push_back(pt);
            }
            else
                mdata[pt.x] = 0;
        }
    }

    int count = (int)nzloc.size();

    // stage 2. process all the points in random order
    for( ; count > 0; count-- )
    {
        // choose random point out of the remaining ones
        int idx = rng.uniform(0, count);
        int max_val = threshold-1, max_n = 0;
        Point point = nzloc[idx];
        Point line_end[2];
        float a, b;
        int* adata = accum img_ptr(int, );
        int i = point.y, j = point.x, k, x0, y0, dx0, dy0, xflag;
        int good_line;
        const int shift = 16;

        // "remove" it by overriding it with the last element
        nzloc[idx] = nzloc[count-1];

        // check if it has been excluded already (i.e. belongs to some other line)
        if( !mdata0[i*width + j] )
            continue;

        // update accumulator, find the most probable line
        for( int n = 0; n < numangle; n++, adata += numrho )
        {
            int r = cRound( j * ttab[n*2] + i * ttab[n*2+1] );
            r += (numrho - 1) / 2;
            int val = ++adata[r];
            if( max_val < val )
            {
                max_val = val;
                max_n = n;
            }
        }

        // if it is too "weak" candidate, continue with another point
        if( max_val < threshold )
            continue;

        // from the current point walk in each direction
        // along the found line and extract the line segment
        a = -ttab[max_n*2+1];
        b = ttab[max_n*2];
        x0 = j;
        y0 = i;
        if( fabs(a) > fabs(b) )
        {
            xflag = 1;
            dx0 = a > 0 ? 1 : -1;
            dy0 = cRound( b*(1 << shift)/fabs(a) );
            y0 = (y0 << shift) + (1 << (shift-1));
        }
        else
        {
            xflag = 0;
            dy0 = b > 0 ? 1 : -1;
            dx0 = cRound( a*(1 << shift)/fabs(b) );
            x0 = (x0 << shift) + (1 << (shift-1));
        }

        for( k = 0; k < 2; k++ )
        {
            int gap = 0, x = x0, y = y0, dx = dx0, dy = dy0;

            if( k > 0 )
                dx = -dx, dy = -dy;

            // walk along the line using fixed-point arithmetics,
            // stop at the image border or in case of too big gap
            for( ;; x += dx, y += dy )
            {
                uchar* mdata;
                int i1, j1;

                if( xflag )
                {
                    j1 = x;
                    i1 = y >> shift;
                }
                else
                {
                    j1 = x >> shift;
                    i1 = y;
                }

                if( j1 < 0 || j1 >= width || i1 < 0 || i1 >= height )
                    break;

                mdata = mdata0 + i1*width + j1;

                // for each non-zero point:
                //    update line end,
                //    clear the mask element
                //    reset the gap
                if( *mdata )
                {
                    gap = 0;
                    line_end[k].y = i1;
                    line_end[k].x = j1;
                }
                else if( ++gap > lineGap )
                    break;
            }
        }

        good_line = std::abs(line_end[1].x - line_end[0].x) >= lineLength ||
                    std::abs(line_end[1].y - line_end[0].y) >= lineLength;

        for( k = 0; k < 2; k++ )
        {
            int x = x0, y = y0, dx = dx0, dy = dy0;

            if( k > 0 )
                dx = -dx, dy = -dy;

            // walk along the line using fixed-point arithmetics,
            // stop at the image border or in case of too big gap
            for( ;; x += dx, y += dy )
            {
                uchar* mdata;
                int i1, j1;

                if( xflag )
                {
                    j1 = x;
                    i1 = y >> shift;
                }
                else
                {
                    j1 = x >> shift;
                    i1 = y;
                }

                mdata = mdata0 + i1*width + j1;

                // for each non-zero point:
                //    update line end,
                //    clear the mask element
                //    reset the gap
                if( *mdata )
                {
                    if( good_line )
                    {
                        adata = accum img_ptr(int, );
                        for( int n = 0; n < numangle; n++, adata += numrho )
                        {
                            int r = cRound( j1 * ttab[n*2] + i1 * ttab[n*2+1] );
                            r += (numrho - 1) / 2;
                            adata[r]--;
                        }
                    }
                    *mdata = 0;
                }

                if( i1 == line_end[k].y && j1 == line_end[k].x )
                    break;
            }
        }

        if( good_line )
        {
            Vec4i lr(line_end[0].x, line_end[0].y, line_end[1].x, line_end[1].y);
            lines.push_back(lr);
            if( (int)lines.size() >= linesMax )
                return;
        }
    }
}

#ifdef HAVE_OPENCL

#define OCL_MAX_LINES 4096

static bool ocl_makePointsList(const img_t* _src, img_t* pointsList, InputCvArr* counters)
{
    UMat src = _src.getUMat();
    _pointsList cvSetMat(1, (int) src CC_MAT_TOTAL(), CC_32SC1);
    UMat pointsList = _pointsList.getUMat();
    UMat counters = _counters.getUMat();
    ocl::Device dev = ocl::Device::getDefault();

    const int pixPerWI = 16;
    int workgroup_size = min((int) dev.maxWorkGroupSize(), (src->cols + pixPerWI - 1)/pixPerWI);
    ocl::Kernel pointListKernel("make_point_list", ocl::imgproc::hough_lines_oclsrc,
                                format("-D MAKE_POINTS_LIST -D GROUP_SIZE=%d -D LOCAL_SIZE=%d", workgroup_size, src->cols));
    if (pointListKernel.empty())
        return false;

    pointListKernel.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnlyNoSize(pointsList),
                         ocl::KernelArg::PtrWriteOnly(counters));

    size_t localThreads[2]  = { (size_t)workgroup_size, 1 };
    size_t globalThreads[2] = { (size_t)workgroup_size, (size_t)src->rows };

    return pointListKernel.run(2, globalThreads, localThreads, false);
}

static bool ocl_fillAccum(const img_t* _pointsList, img_t* accum, int total_points, double rho, double theta, int numrho, int numangle)
{
    UMat pointsList = _pointsList.getUMat();
    _accum cvSetMat(numangle + 2, numrho + 2, CC_32SC1);
    UMat accum = _accum.getUMat();
    ocl::Device dev = ocl::Device::getDefault();

    float irho = (float) (1 / rho);
    int workgroup_size = min((int) dev.maxWorkGroupSize(), total_points);

    ocl::Kernel fillAccumKernel;
    size_t localThreads[2];
    size_t globalThreads[2];

    size_t local_memory_needed = (numrho + 2)*sizeof(int);
    if (local_memory_needed > dev.localMemSize())
    {
        accum.setTo(Scalar::all(0));
        fillAccumKernel cvSetMat("fill_accum_global", ocl::imgproc::hough_lines_oclsrc,
                                format("-D FILL_ACCUM_GLOBAL"));
        if (fillAccumKernel.empty())
            return false;
        globalThreads[0] = workgroup_size; globalThreads[1] = numangle;
        fillAccumKernel.args(ocl::KernelArg::ReadOnlyNoSize(pointsList), ocl::KernelArg::WriteOnlyNoSize(accum),
                        total_points, irho, (float) theta, numrho, numangle);
        return fillAccumKernel.run(2, globalThreads, NULL, false);
    }
    else
    {
        fillAccumKernel cvSetMat("fill_accum_local", ocl::imgproc::hough_lines_oclsrc,
                                format("-D FILL_ACCUM_LOCAL -D LOCAL_SIZE=%d -D BUFFER_SIZE=%d", workgroup_size, numrho + 2));
        if (fillAccumKernel.empty())
            return false;
        localThreads[0] = workgroup_size; localThreads[1] = 1;
        globalThreads[0] = workgroup_size; globalThreads[1] = numangle+2;
        fillAccumKernel.args(ocl::KernelArg::ReadOnlyNoSize(pointsList), ocl::KernelArg::WriteOnlyNoSize(accum),
                        total_points, irho, (float) theta, numrho, numangle);
        return fillAccumKernel.run(2, globalThreads, localThreads, false);
    }
}

static bool ocl_HoughLines(const img_t* _src, img_t* lines, double rho, double theta, int threshold,
                           double min_theta, double max_theta)
{
    CC_Assert(_src->tid == CC_8UC1);

    if (max_theta < 0 || max_theta > CC_PI ) {
        CC_Error( CC_StsBadArg, "max_theta must fall between 0 and pi" );
    }
    if (min_theta < 0 || min_theta > max_theta ) {
        CC_Error( CC_StsBadArg, "min_theta must fall between 0 and max_theta" );
    }
    if (!(rho > 0 && theta > 0)) {
        CC_Error( CC_StsBadArg, "rho and theta must be greater 0" );
    }

    UMat src = _src.getUMat();
    int numangle = cRound((max_theta - min_theta) / theta);
    int numrho = cRound(((src->cols + src->rows) * 2 + 1) / rho);

    UMat pointsList;
    UMat counters(1, 2, CC_32SC1, Scalar::all(0));

    if (!ocl_makePointsList(src, pointsList, counters))
        return false;

    int total_points = counters.getMat(ACCESS_READ).at<int>(0, 0);
    if (total_points <= 0)
    {
        _lines.assign(UMat(0,0,CC_32FC2));
        return true;
    }

    UMat accum;
    if (!ocl_fillAccum(pointsList, accum, total_points, rho, theta, numrho, numangle))
        return false;

    const int pixPerWI = 8;
    ocl::Kernel getLinesKernel("get_lines", ocl::imgproc::hough_lines_oclsrc,
                               format("-D GET_LINES"));
    if (getLinesKernel.empty())
        return false;

    int linesMax = threshold > 0 ? min(total_points*numangle/threshold, OCL_MAX_LINES) : OCL_MAX_LINES;
    UMat lines(linesMax, 1, CC_32FC2);

    getLinesKernel.args(ocl::KernelArg::ReadOnly(accum), ocl::KernelArg::WriteOnlyNoSize(lines),
                        ocl::KernelArg::PtrWriteOnly(counters), linesMax, threshold, (float) rho, (float) theta);

    size_t globalThreads[2] = { ((size_t)numrho + pixPerWI - 1)/pixPerWI, (size_t)numangle };
    if (!getLinesKernel.run(2, globalThreads, NULL, false))
        return false;

    int total_lines = min(counters.getMat(ACCESS_READ).at<int>(0, 1), linesMax);
    if (total_lines > 0)
        _lines.assign(lines cvGetRows(,CRange(0, total_lines)));
    else
        _lines.assign(UMat(0,0,CC_32FC2));
    return true;
}

static bool ocl_HoughLinesP(const img_t* _src, img_t* lines, double rho, double theta, int threshold,
                           double minLineLength, double maxGap)
{
    CC_Assert(_src->tid == CC_8UC1);

    if (!(rho > 0 && theta > 0)) {
        CC_Error( CC_StsBadArg, "rho and theta must be greater 0" );
    }

    UMat src = _src.getUMat();
    int numangle = cRound(CC_PI / theta);
    int numrho = cRound(((src->cols + src->rows) * 2 + 1) / rho);

    UMat pointsList;
    UMat counters(1, 2, CC_32SC1, Scalar::all(0));

    if (!ocl_makePointsList(src, pointsList, counters))
        return false;

    int total_points = counters.getMat(ACCESS_READ).at<int>(0, 0);
    if (total_points <= 0)
    {
        _lines.assign(UMat(0,0,CC_32SC4));
        return true;
    }

    UMat accum;
    if (!ocl_fillAccum(pointsList, accum, total_points, rho, theta, numrho, numangle))
        return false;

    ocl::Kernel getLinesKernel("get_lines", ocl::imgproc::hough_lines_oclsrc,
                               format("-D GET_LINES_PROBABOLISTIC"));
    if (getLinesKernel.empty())
        return false;

    int linesMax = threshold > 0 ? min(total_points*numangle/threshold, OCL_MAX_LINES) : OCL_MAX_LINES;
    UMat lines(linesMax, 1, CC_32SC4);

    getLinesKernel.args(ocl::KernelArg::ReadOnly(accum), ocl::KernelArg::ReadOnly(src),
                        ocl::KernelArg::WriteOnlyNoSize(lines), ocl::KernelArg::PtrWriteOnly(counters),
                        linesMax, threshold, (int) minLineLength, (int) maxGap, (float) rho, (float) theta);

    size_t globalThreads[2] = { (size_t)numrho, (size_t)numangle };
    if (!getLinesKernel.run(2, globalThreads, NULL, false))
        return false;

    int total_lines = min(counters.getMat(ACCESS_READ).at<int>(0, 1), linesMax);
    if (total_lines > 0)
        _lines.assign(lines cvGetRows(,CRange(0, total_lines)));
    else
        _lines.assign(UMat(0,0,CC_32SC4));

    return true;
}

#endif /* HAVE_OPENCL */

}

void HoughLines( const img_t* _image, img_t* lines,
                    double rho, double theta, int threshold,
                    double srn, double stn, double min_theta, double max_theta )
{
    CC_INSTRUMENT_REGION()

    CC_OCL_RUN(srn == 0 && stn == 0 && _image.isUMat() && _lines.isUMat(),
               ocl_HoughLines(_image, _lines, rho, theta, threshold, min_theta, max_theta));

    img_t image = _image;
    std::vector<Vec2f> lines;

    if( srn == 0 && stn == 0 )
        HoughLinesStandard(image, (float)rho, (float)theta, threshold, lines, INT_MAX, min_theta, max_theta );
    else
        HoughLinesSDiv(image, (float)rho, (float)theta, threshold, cRound(srn), cRound(stn), lines, INT_MAX, min_theta, max_theta);

    img_t(lines).copyTo(_lines);
}


void HoughLinesP(const img_t* _image, img_t* lines,
                     double rho, double theta, int threshold,
                     double minLineLength, double maxGap )
{
    CC_INSTRUMENT_REGION()

    CC_OCL_RUN(_image.isUMat() && _lines.isUMat(),
               ocl_HoughLinesP(_image, _lines, rho, theta, threshold, minLineLength, maxGap));

    img_t image = _image;
    std::vector<Vec4i> lines;
    HoughLinesProbabilistic(image, (float)rho, (float)theta, threshold, cRound(minLineLength), cRound(maxGap), lines, INT_MAX);
    img_t(lines).copyTo(_lines);
}



/* Wrapper function for standard hough transform */
CC_IMPL CvSeq*
cvHoughLines2( img_t* src_image, void* lineStorage, int method,
               double rho, double theta, int threshold,
               double param1, double param2,
               double min_theta, double max_theta )
{
    img_t image = cvarrToMat(src_image);
    std::vector<Vec2f> l2;
    std::vector<Vec4i> l4;
    CvSeq* result = 0;

    img_t* mat = 0;
    CvSeq* lines = 0;
    CvSeq lines_header;
    CvSeqBlock lines_block;
    int lineType, elemSize;
    int linesMax = INT_MAX;
    int iparam1, iparam2;

    if( !lineStorage )
        CC_Error( CC_StsNullPtr, "NULL destination" );

    if( rho <= 0 || theta <= 0 || threshold <= 0 )
        CC_Error( CC_StsOutOfRange, "rho, theta and threshold must be positive" );

    if( method != CC_HOUGH_PROBABILISTIC )
    {
        lineType = CC_32FC2;
        elemSize = sizeof(float)*2;
    }
    else
    {
        lineType = CC_32SC4;
        elemSize = sizeof(int)*4;
    }

    if( CC_IS_STORAGE( lineStorage ))
    {
        lines = cvCreateSeq( lineType, sizeof(CvSeq), elemSize, (CvMemStorage*)lineStorage );
    }
    else if( CC_IS_MAT( lineStorage ))
    {
        mat = lineStorage;

        if( !CC_IS_MAT_CONT( mat->tid ) || (mat->rows != 1 && mat->cols != 1) )
            CC_Error( CC_StsBadArg,
            "The destination matrix should be continuous and have a single row or a single column" );

        if( CC_MAT_TYPE( mat->tid ) != lineType )
            CC_Error( CC_StsBadArg,
            "The destination matrix data type is inappropriate, see the manual" );

        lines = cvMakeSeqHeaderForArray( lineType, sizeof(CvSeq), elemSize, mat->data.ptr,
                                         mat->rows + mat->cols - 1, &lines_header, &lines_block );
        linesMax = lines->total;
        cvClearSeq( lines );
    }
    else
        CC_Error( CC_StsBadArg, "Destination is not CvMemStorage* nor img_t*" );

    iparam1 = cRound(param1);
    iparam2 = cRound(param2);

    switch( method )
    {
    case CC_HOUGH_STANDARD:
        HoughLinesStandard( image, (float)rho,
                (float)theta, threshold, l2, linesMax, min_theta, max_theta );
        break;
    case CC_HOUGH_MULTI_SCALE:
        HoughLinesSDiv( image, (float)rho, (float)theta,
                threshold, iparam1, iparam2, l2, linesMax, min_theta, max_theta );
        break;
    case CC_HOUGH_PROBABILISTIC:
        HoughLinesProbabilistic( image, (float)rho, (float)theta,
                threshold, iparam1, iparam2, l4, linesMax );
        break;
    default:
        CC_Error( CC_StsBadArg, "Unrecognized method id" );
    }

    int nlines = (int)(l2.size() + l4.size());

    if( mat )
    {
        if( mat->cols > mat->rows )
            mat->cols = nlines;
        else
            mat->rows = nlines;
    }

    if( nlines )
    {
        img_t lx = method == CC_HOUGH_STANDARD || method == CC_HOUGH_MULTI_SCALE ?
            img_t(nlines, 1, CC_32FC2, &l2[0]) : img_t(nlines, 1, CC_32SC4, &l4[0]);

        if( mat )
        {
            img_t dst(nlines, 1, lx->tid, mat->data.ptr);
            lx.copyTo(dst);
        }
        else
        {
            cvSeqPushMulti(lines, lx->tt.data, nlines);
        }
    }

    if( !mat )
        result = lines;
    return result;
}


/****************************************************************************************\
*                                     Circle Detection                                   *
\****************************************************************************************/

static void
icvHoughCirclesGradient( img_t* img, float dp, float min_dist,
                         int min_radius, int max_radius,
                         int canny_threshold, int acc_threshold,
                         CvSeq* circles, int circles_max )
{
    const int SHIFT = 10, ONE = 1 << SHIFT;
    Ptr<img_t> dx, dy;
    Ptr<img_t> edges, accum, dist_buf;
    std::vector<int> sort_buf;
    Ptr<CvMemStorage> storage;

    int x, y, i, j, k, center_count, nz_count;
    float min_radius2 = (float)min_radius*min_radius;
    float max_radius2 = (float)max_radius*max_radius;
    int rows, cols, arows, acols;
    int astep, *adata;
    float* ddata;
    CvSeq *nz, *centers;
    float idp, dr;
    CvSeqReader reader;

    edges.reset(cvCreateMat( img->rows, img->cols, CC_8UC1 ));

    // Use the Canny Edge Detector to detect all the edges in the image.
    cvCanny( img, edges, MAX(canny_threshold/2,1), canny_threshold, 3 );

    dx.reset(cvCreateMat( img->rows, img->cols, CC_16SC1 ));
    dy.reset(cvCreateMat( img->rows, img->cols, CC_16SC1 ));

    /*Use the Sobel Derivative to compute the local gradient of all the non-zero pixels in the edge image.*/
    cvSobel( img, dx, 1, 0, 3 );
    cvSobel( img, dy, 0, 1, 3 );

    if( dp < 1.f )
        dp = 1.f;
    idp = 1.f/dp;
    accum.reset(cvCreateMat( cCeil(img->rows*idp)+2, cCeil(img->cols*idp)+2, CC_32SC1 ));
    cvZero(accum);

    storage.reset(cvCreateMemStorage());
    /* Create sequences for the nonzero pixels in the edge image and the centers of circles
    which could be detected.*/
    nz = cvCreateSeq( CC_32SC2, sizeof(CvSeq), sizeof(CPoint), storage );
    centers = cvCreateSeq( CC_32SC1, sizeof(CvSeq), sizeof(int), storage );

    rows = img->rows;
    cols = img->cols;
    arows = accum->rows - 2;
    acols = accum->cols - 2;
    adata = accum->data.i;
    astep = accum->step/sizeof(adata[0]);
    // Accumulate circle evidence for each edge pixel
    for( y = 0; y < rows; y++ )
    {
        const uchar* edges_row = edges->data.ptr + y*edges->step;
        const short* dx_row = (const short*)(dx->data.ptr + y*dx->step);
        const short* dy_row = (const short*)(dy->data.ptr + y*dy->step);

        for( x = 0; x < cols; x++ )
        {
            float vx, vy;
            int sx, sy, x0, y0, x1, y1, r;
            CPoint pt;

            vx = dx_row[x];
            vy = dy_row[x];

            if( !edges_row[x] || (vx == 0 && vy == 0) )
                continue;

            float mag = sqrt(vx*vx+vy*vy);
            assert( mag >= 1 );
            sx = cRound((vx*idp)*ONE/mag);
            sy = cRound((vy*idp)*ONE/mag);

            x0 = cRound((x*idp)*ONE);
            y0 = cRound((y*idp)*ONE);
            // Step from min_radius to max_radius in both directions of the gradient
            for(int k1 = 0; k1 < 2; k1++ )
            {
                x1 = x0 + min_radius * sx;
                y1 = y0 + min_radius * sy;

                for( r = min_radius; r <= max_radius; x1 += sx, y1 += sy, r++ )
                {
                    int x2 = x1 >> SHIFT, y2 = y1 >> SHIFT;
                    if( (unsigned)x2 >= (unsigned)acols ||
                        (unsigned)y2 >= (unsigned)arows )
                        break;
                    adata[y2*astep + x2]++;
                }

                sx = -sx; sy = -sy;
            }

            pt.x = x; pt.y = y;
            cvSeqPush( nz, &pt );
        }
    }

    nz_count = nz->total;
    if( !nz_count )
        return;
    //Find possible circle centers
    for( y = 1; y < arows - 1; y++ )
    {
        for( x = 1; x < acols - 1; x++ )
        {
            int base = y*(acols+2) + x;
            if( adata[base] > acc_threshold &&
                adata[base] > adata[base-1] && adata[base] > adata[base+1] &&
                adata[base] > adata[base-acols-2] && adata[base] > adata[base+acols+2] )
                cvSeqPush(centers, &base);
        }
    }

    center_count = centers->total;
    if( !center_count )
        return;

    sort_buf.resize( MAX(center_count,nz_count) );
    cvCvtSeqToArray( centers, &sort_buf[0] );
    /*Sort candidate centers in descending order of their accumulator values, so that the centers
    with the most supporting pixels appear first.*/
    std::sort(sort_buf.begin(), sort_buf.begin() + center_count, hough_cmp_gt(adata));
    cvClearSeq( centers );
    cvSeqPushMulti( centers, &sort_buf[0], center_count );

    dist_buf.reset(cvCreateMat( 1, nz_count, CC_32FC1 ));
    ddata = dist_buf->data.fl;

    dr = dp;
    min_dist = MAX( min_dist, dp );
    min_dist *= min_dist;
    // For each found possible center
    // Estimate radius and check support
    for( i = 0; i < centers->total; i++ )
    {
        int ofs = *(int*)cvGetSeqElem( centers, i );
        y = ofs/(acols+2);
        x = ofs - (y)*(acols+2);
        //Calculate circle's center in pixels
        float cx = (float)((x + 0.5f)*dp), cy = (float)(( y + 0.5f )*dp);
        float start_dist, dist_sum;
        float r_best = 0;
        int max_count = 0;
        // Check distance with previously detected circles
        for( j = 0; j < circles->total; j++ )
        {
            float* c = (float*)cvGetSeqElem( circles, j );
            if( (c[0] - cx)*(c[0] - cx) + (c[1] - cy)*(c[1] - cy) < min_dist )
                break;
        }

        if( j < circles->total )
            continue;
        // Estimate best radius
        cvStartReadSeq( nz, &reader );
        for( j = k = 0; j < nz_count; j++ )
        {
            CPoint pt;
            float _dx, _dy, _r2;
            CC_READ_SEQ_ELEM( pt, reader );
            _dx = cx - pt.x; _dy = cy - pt.y;
            _r2 = _dx*_dx + _dy*_dy;
            if(min_radius2 <= _r2 && _r2 <= max_radius2 )
            {
                ddata[k] = _r2;
                sort_buf[k] = k;
                k++;
            }
        }

        int nz_count1 = k, start_idx = nz_count1 - 1;
        if( nz_count1 == 0 )
            continue;
        dist_buf->cols = nz_count1;
        cvPow( dist_buf, dist_buf, 0.5 );
        // Sort non-zero pixels according to their distance from the center.
        std::sort(sort_buf.begin(), sort_buf.begin() + nz_count1, hough_cmp_gt((int*)ddata));

        dist_sum = start_dist = ddata[sort_buf[nz_count1-1]];
        for( j = nz_count1 - 2; j >= 0; j-- )
        {
            float d = ddata[sort_buf[j]];

            if( d > max_radius )
                break;

            if( d - start_dist > dr )
            {
                float r_cur = ddata[sort_buf[(j + start_idx)/2]];
                if( (start_idx - j)*r_best >= max_count*r_cur ||
                    (r_best < FLT_EPSILON && start_idx - j >= max_count) )
                {
                    r_best = r_cur;
                    max_count = start_idx - j;
                }
                start_dist = d;
                start_idx = j;
                dist_sum = 0;
            }
            dist_sum += d;
        }
        // Check if the circle has enough support
        if( max_count > acc_threshold )
        {
            float c[3];
            c[0] = cx;
            c[1] = cy;
            c[2] = (float)r_best;
            cvSeqPush( circles, c );
            if( circles->total > circles_max )
                return;
        }
    }
}

CC_IMPL CvSeq*
cvHoughCircles( img_t* src_image, void* circle_storage,
                int method, double dp, double min_dist,
                double param1, double param2,
                int min_radius, int max_radius )
{
    CvSeq* result = 0;

    img_t stub, *img = src_image;
    img_t* mat = 0;
    CvSeq* circles = 0;
    CvSeq circles_header;
    CvSeqBlock circles_block;
    int circles_max = INT_MAX;
    int canny_threshold = cRound(param1);
    int acc_threshold = cRound(param2);

    img = cvGetMat( img, &stub );

    if( !CC_IS_MASK_ARR(img))
        CC_Error( CC_StsBadArg, "The source image must be 8-bit, single-channel" );

    if( !circle_storage )
        CC_Error( CC_StsNullPtr, "NULL destination" );

    if( dp <= 0 || min_dist <= 0 || canny_threshold <= 0 || acc_threshold <= 0 )
        CC_Error( CC_StsOutOfRange, "dp, min_dist, canny_threshold and acc_threshold must be all positive numbers" );

    min_radius = MAX( min_radius, 0 );
    if( max_radius <= 0 )
        max_radius = MAX( img->rows, img->cols );
    else if( max_radius <= min_radius )
        max_radius = min_radius + 2;

    if( CC_IS_STORAGE( circle_storage ))
    {
        circles = cvCreateSeq( CC_32FC3, sizeof(CvSeq),
            sizeof(float)*3, (CvMemStorage*)circle_storage );
    }
    else if( CC_IS_MAT( circle_storage ))
    {
        mat = circle_storage;

        if( !CC_IS_MAT_CONT( mat->tid ) || (mat->rows != 1 && mat->cols != 1) ||
            CC_MAT_TYPE(mat->tid) != CC_32FC3 )
            CC_Error( CC_StsBadArg,
            "The destination matrix should be continuous and have a single row or a single column" );

        circles = cvMakeSeqHeaderForArray( CC_32FC3, sizeof(CvSeq), sizeof(float)*3,
                mat->data.ptr, mat->rows + mat->cols - 1, &circles_header, &circles_block );
        circles_max = circles->total;
        cvClearSeq( circles );
    }
    else
        CC_Error( CC_StsBadArg, "Destination is not CvMemStorage* nor img_t*" );

    switch( method )
    {
    case CC_HOUGH_GRADIENT:
        icvHoughCirclesGradient( img, (float)dp, (float)min_dist,
                                min_radius, max_radius, canny_threshold,
                                acc_threshold, circles, circles_max );
          break;
    default:
        CC_Error( CC_StsBadArg, "Unrecognized method id" );
    }

    if( mat )
    {
        if( mat->cols > mat->rows )
            mat->cols = circles->total;
        else
            mat->rows = circles->total;
    }
    else
        result = circles;

    return result;
}


namespace cv
{

const int STORAGE_SIZE = 1 << 12;

static void seqToMat(const CvSeq* seq, img_t* arr)
{
    if( seq && seq->total > 0 )
    {
        _arr cvSetMat(1, seq->total, seq->flags, -1, true);
        img_t arr = _arr;
        cvCvtSeqToArray(seq, arr->tt.data);
    }
    else
        _arr.release();
}

}

void HoughCircles( const img_t* _image, img_t* circles,
                       int method, double dp, double min_dist,
                       double param1, double param2,
                       int minRadius, int maxRadius )
{
    CC_INSTRUMENT_REGION()

    Ptr<CvMemStorage> storage(cvCreateMemStorage(STORAGE_SIZE));
    img_t image = _image;
    img_t c_image = image;
    CvSeq* seq = cvHoughCircles( &c_image, storage, method,
                    dp, min_dist, param1, param2, minRadius, maxRadius );
    seqToMat(seq, _circles);
}


