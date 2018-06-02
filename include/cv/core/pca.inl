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
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/



/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

namespace cv
{

PCA::PCA() {}

PCA::PCA(const img_t* data, const img_t* _mean, int flags, int maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(const img_t* data, const img_t* _mean, int flags, double retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(const img_t* _data, const img_t* __mean, int flags, int maxComponents)
{
    img_t data = _data, _mean = __mean;
    int covar_flags = CC_COVAR_SCALE;
    int len, in_count;
    Size mean_sz;

    CC_Assert( data CC_MAT_CN() == 1 );
    if( flags & CC_PCA_DATA_AS_COL )
    {
        len = data->rows;
        in_count = data->cols;
        covar_flags |= CC_COVAR_COLS;
        mean_sz = Size(1, len);
    }
    else
    {
        len = data->cols;
        in_count = data->rows;
        covar_flags |= CC_COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    int count = MIN(len, in_count), out_count = count;
    if( maxComponents > 0 )
        out_count = MIN(count, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    if( len <= in_count )
        covar_flags |= CC_COVAR_NORMAL;

    int ctype = MAX(CC_32F, data CC_MAT_DEPTH());
    mean cvSetMat( mean_sz, ctype );

    img_t covar( count, count, ctype );

    if( !_mean.empty() )
    {
        CC_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= CC_COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    if( !(covar_flags & CC_COVAR_NORMAL) )
    {
        // CC_PCA_DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // CC_PCA_DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        img_t tmp_data, tmp_mean = repeat(mean, data->rows/mean->rows, data->cols/mean->cols);
        if( data CC_MAT_TYPE() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        else
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        img_t evects1(count, len, ctype);
        gemm( eigenvectors, tmp_data, 1, img_t(), 0, evects1,
            (flags & CC_PCA_DATA_AS_COL) ? CC_GEMM_B_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        int i;
        for( i = 0; i < out_count; i++ )
        {
            img_t vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    if( count > out_count )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues cvGetRows(,0,out_count).clone();
        eigenvectors = eigenvectors cvGetRows(,0,out_count).clone();
    }
    return *this;
}

void PCA::write(FileStorage& fs ) const
{
    CC_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

void PCA::read(const FileNode& fn)
{
    CC_Assert( !fn.empty() );
    CC_Assert( (String)fn["name"] == "PCA" );

    read(fn["vectors"], eigenvectors);
    read(fn["values"], eigenvalues);
    read(fn["mean"], mean);
}

template <typename T>
int computeCumulativeEnergy(const img_t& eigenvalues, double retainedVariance)
{
    CC_DbgAssert( eigenvalues CC_MAT_TYPE() == DataType<T>::type );

    img_t g(eigenvalues.size(), DataType<T>::type);

    for(int ig = 0; ig < g->rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        for(int im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    int L;

    for(L = 0; L < eigenvalues->rows; L++)
    {
        double energy = g.at<T>(L, 0) / g.at<T>(g->rows - 1, 0);
        if(energy > retainedVariance)
            break;
    }

    L = MAX(2, L);

    return L;
}

PCA& PCA::operator()(const img_t* _data, const img_t* __mean, int flags, double retainedVariance)
{
    img_t data = _data, _mean = __mean;
    int covar_flags = CC_COVAR_SCALE;
    int len, in_count;
    Size mean_sz;

    CC_Assert( data CC_MAT_CN() == 1 );
    if( flags & CC_PCA_DATA_AS_COL )
    {
        len = data->rows;
        in_count = data->cols;
        covar_flags |= CC_COVAR_COLS;
        mean_sz = Size(1, len);
    }
    else
    {
        len = data->cols;
        in_count = data->rows;
        covar_flags |= CC_COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CC_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    int count = MIN(len, in_count);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    if( len <= in_count )
        covar_flags |= CC_COVAR_NORMAL;

    int ctype = MAX(CC_32F, data CC_MAT_DEPTH());
    mean cvSetMat( mean_sz, ctype );

    img_t covar( count, count, ctype );

    if( !_mean.empty() )
    {
        CC_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    if( !(covar_flags & CC_COVAR_NORMAL) )
    {
        // CC_PCA_DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // CC_PCA_DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        img_t tmp_data, tmp_mean = repeat(mean, data->rows/mean->rows, data->cols/mean->cols);
        if( data CC_MAT_TYPE() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        else
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        img_t evects1(count, len, ctype);
        gemm( eigenvectors, tmp_data, 1, img_t(), 0, evects1,
            (flags & CC_PCA_DATA_AS_COL) ? CC_GEMM_B_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        int i;
        for( i = 0; i < eigenvectors->rows; i++ )
        {
            img_t vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    int L;
    if (ctype == CC_32F)
        L = computeCumulativeEnergy<float>(eigenvalues, retainedVariance);
    else
        L = computeCumulativeEnergy<double>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues cvGetRows(,0,L).clone();
    eigenvectors = eigenvectors cvGetRows(,0,L).clone();

    return *this;
}

void PCA::project(const img_t* _data, img_t* result) const
{
    img_t data = _data;
    CC_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean->rows == 1 && mean->cols == data->cols) || (mean->cols == 1 && mean->rows == data->rows)));
    img_t tmp_data, tmp_mean = repeat(mean, data->rows/mean->rows, data->cols/mean->cols);
    int ctype = mean CC_MAT_TYPE();
    if( data CC_MAT_TYPE() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    else
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    if( mean->rows == 1 )
        gemm( tmp_data, eigenvectors, 1, img_t(), 0, result, CC_GEMM_2_T );
    else
        gemm( eigenvectors, tmp_data, 1, img_t(), 0, result, 0 );
}

img_t PCA::project(const img_t* data) const
{
    img_t result;
    project(data, result);
    return result;
}

void PCA::backProject(const img_t* _data, img_t* result) const
{
    img_t data = _data;
    CC_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean->rows == 1 && eigenvectors->rows == data->cols) ||
         (mean->cols == 1 && eigenvectors->rows == data->rows)));

    img_t tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean CC_MAT_TYPE());
    if( mean->rows == 1 )
    {
        tmp_mean = repeat(mean, data->rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    else
    {
        tmp_mean = repeat(mean, 1, data->cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, CC_GEMM_1_T );
    }
}

img_t PCA::backProject(const img_t* data) const
{
    img_t result;
    backProject(data, result);
    return result;
}

}

void PCACompute(const img_t* data, InputOutputArray mean,
                    img_t* eigenvectors, int maxComponents)
{
    CC_INSTRUMENT_REGION()

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

void PCACompute(const img_t* data, InputOutputArray mean,
                    img_t* eigenvectors, double retainedVariance)
{
    CC_INSTRUMENT_REGION()

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

void PCAProject(const img_t* data, const img_t* mean,
                    const img_t* eigenvectors, img_t* result)
{
    CC_INSTRUMENT_REGION()

    PCA pca;
    pca.mean = mean;
    pca.eigenvectors = eigenvectors;
    pca.project(data, result);
}

void PCABackProject(const img_t* data, const img_t* mean,
                    const img_t* eigenvectors, img_t* result)
{
    CC_INSTRUMENT_REGION()

    PCA pca;
    pca.mean = mean;
    pca.eigenvectors = eigenvectors;
    pca.backProject(data, result);
}
