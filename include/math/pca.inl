
typedef struct pca_t {
  img_t eigenvectors[1]; //!< eigenvectors of the covariation matrix
  img_t eigenvalues[1]; //!< eigenvalues of the covariation matrix
  img_t mean[1]; //!< mean value subtracted before the projection and added after the back projection
} pca_t;

#include "cv1/matmul.inl"
#include "cv1/matrix.inl"

int pca_init(pca_t* s, const img_t* data, const img_t* _mean, int flags, int maxComponents)
{
    int covar_flags = CC_COVAR_SCALE;
    int len, in_count, i=1;
    ISIZE mean_sz;
    img_t im[10] = {0};
    img_t* covar = im+i++;

    if( flags & CC_PCA_DATA_AS_COL )
    {
        len = data->h;
        in_count = data->w;
        covar_flags |= CC_COVAR_COLS;
        mean_sz = iSIZE(1, len);
    }
    else
    {
        len = data->w;
        in_count = data->h;
        covar_flags |= CC_COVAR_ROWS;
        mean_sz = iSIZE(len, 1);
    }

    int count = MIN(len, in_count), out_count = count;
    if( maxComponents > 0 )
        out_count = MIN(count, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    if( len <= in_count )
        covar_flags |= CC_COVAR_NORMAL;

    imsetsize_f8( covar, count, count, 1 );
    if( _mean )
    {
      imclone2(_mean, s->mean);
      covar_flags |= CC_COVAR_USE_AVG;
    } else {
      imsetsize_f8( s->mean, mean_sz.h, mean_sz.w, 1);
    }

#if 0
    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    if( !(covar_flags & CC_COVAR_NORMAL) )
    {
        // CC_PCA_DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // CC_PCA_DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        img_t tmp_data, tmp_mean = repeat(mean, data->h/mean->h, data->w/mean->w);
        if( data.type() != ctype || tmp_mean.data == mean.data )
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
        eigenvalues = eigenvalues.rowRange(0,out_count).clone();
        eigenvectors = eigenvectors.rowRange(0,out_count).clone();
    }
#endif
    return 0;
}

#if 0

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
    CC_DbgAssert( eigenvalues.type() == DataType<T>::type );

    img_t g(eigenvalues.size(), DataType<T>::type);

    for(int ig = 0; ig < g->h; ig++)
    {
        g.at<T>(ig, 0) = 0;
        for(int im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    int L;

    for(L = 0; L < eigenvalues->h; L++)
    {
        double energy = g.at<T>(L, 0) / g.at<T>(g->h - 1, 0);
        if(energy > retainedVariance)
            break;
    }

    L = MAX(2, L);

    return L;
}

PCA& PCA::operator()(const img_t* _data, const img_t* __mean, int flags, double retainedVariance)
{
    img_t data = _data.getMat(), _mean = __mean.getMat();
    int covar_flags = CC_COVAR_SCALE;
    int len, in_count;
    Size mean_sz;

    CC_Assert( data.channels() == 1 );
    if( flags & CC_PCA_DATA_AS_COL )
    {
        len = data->h;
        in_count = data->w;
        covar_flags |= CC_COVAR_COLS;
        mean_sz = Size(1, len);
    }
    else
    {
        len = data->w;
        in_count = data->h;
        covar_flags |= CC_COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CC_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    int count = MIN(len, in_count);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    if( len <= in_count )
        covar_flags |= CC_COVAR_NORMAL;

    int ctype = MAX(CC_32F, data.depth());
    mean.create( mean_sz, ctype );

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
        img_t tmp_data, tmp_mean = repeat(mean, data->h/mean->h, data->w/mean->w);
        if( data.type() != ctype || tmp_mean.data == mean.data )
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
        for( i = 0; i < eigenvectors->h; i++ )
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
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    return *this;
}

void PCA::project(const img_t* _data, OutputArray result) const
{
    img_t data = _data.getMat();
    CC_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean->h == 1 && mean->w == data->w) || (mean->w == 1 && mean->h == data->h)));
    img_t tmp_data, tmp_mean = repeat(mean, data->h/mean->h, data->w/mean->w);
    int ctype = mean.type();
    if( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    else
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    if( mean->h == 1 )
        gemm( tmp_data, eigenvectors, 1, img_t(), 0, result, GEMM_2_T );
    else
        gemm( eigenvectors, tmp_data, 1, img_t(), 0, result, 0 );
}

img_t PCA::project(const img_t* data) const
{
    img_t result;
    project(data, result);
    return result;
}

void PCA::backProject(const img_t* _data, OutputArray result) const
{
    img_t data = _data.getMat();
    CC_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean->h == 1 && eigenvectors->h == data->w) ||
         (mean->w == 1 && eigenvectors->h == data->h)));

    img_t tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    if( mean->h == 1 )
    {
        tmp_mean = repeat(mean, data->h, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    else
    {
        tmp_mean = repeat(mean, 1, data->w);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

img_t PCA::backProject(const img_t* data) const
{
    img_t result;
    backProject(data, result);
    return result;
}



void PCACompute(const img_t* data, InputOutputArray mean,
                    OutputArray eigenvectors, int maxComponents)
{
    CC_INSTRUMENT_REGION()

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

void PCACompute(const img_t* data, InputOutputArray mean,
                    OutputArray eigenvectors, double retainedVariance)
{
    CC_INSTRUMENT_REGION()

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

void PCAProject(const img_t* data, const img_t* mean,
                    const img_t* eigenvectors, OutputArray result)
{
    CC_INSTRUMENT_REGION()

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

void PCABackProject(const img_t* data, const img_t* mean,
                    const img_t* eigenvectors, OutputArray result)
{
    CC_INSTRUMENT_REGION()

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}

#endif
