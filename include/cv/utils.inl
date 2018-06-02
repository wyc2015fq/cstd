
CC_IMPL void
cvKMeans2(const img_t* samples, int cluster_count,
           img_t* labels, CTermCriteria termcrit)
{
    int i = 0;
    img_t im[5] = {0};
    img_t* centers = im+i++;
    img_t* old_centers = im+i++;
    img_t* counters = im+i++;

    CC_FUNCNAME("cvKMeans2");

    __BEGIN__;

    img_t* temp = 0;
    CRNG rng = cRNG(-1);
    int i, j, k, sample_count, dims;
    int ids_delta, iter;
    double max_dist;

    if(cluster_count < 1)
        CC_ERROR(CC_StsOutOfRange, "Number of clusters should be positive");

    if(CC_MAT_TYPE(samples) != CC_32F || CC_MAT_TYPE(labels) != CC_32S)
        CC_ERROR(CC_StsUnsupportedFormat,
        "samples should be floating-point matrix, cluster_idx - integer vector");

    if(labels->rows != 1 && (labels->cols != 1 || !CC_IS_MAT_CONT(labels)) ||
        labels->rows + labels->cols - 1 != samples->rows)
        CC_ERROR(CC_StsUnmatchedSizes,
        "cluster_idx should be 1D vector of the same number of elements as samples' number of rows");

    CC_CALL(termcrit = cvCheckTermCriteria(termcrit, 1e-6, 100));

    termcrit.epsilon *= termcrit.epsilon;
    sample_count = samples->rows;

    if(cluster_count > sample_count)
        cluster_count = sample_count;

    dims = samples->cols*CC_MAT_CN(samples);
    ids_delta = labels->step ? labels->step/(int)sizeof(int) : 1;

    cvCreateMat(centers, cluster_count, dims, CC_64F, 1);
    cvCreateMat(old_centers, cluster_count, dims, CC_64F, 1);
    cvCreateMat(counters, 1, cluster_count, CC_32S, 1);

    // init centers
    for(i = 0; i < sample_count; i++)
        labels->tt.i[i] = cvRandInt(&rng) % cluster_count;

    counters->cols = cluster_count; // cut down counters
    max_dist = termcrit.epsilon*2;

    for(iter = 0; iter < termcrit.max_iter; iter++)
    {
        // computer centers
        cvZeroMat(centers);
        cvZeroMat(counters);

        for(i = 0; i < sample_count; i++)
        {
            double* c;
            float* s = (float*)(samples->tt.data + i*samples->step);
            k = labels->tt.i[i*ids_delta];
            c = (double*)(centers->tt.data + k*centers->step);
            for(j = 0; j <= dims - 4; j += 4)
            {
                double t0 = c[j] + s[j];
                double t1 = c[j+1] + s[j+1];

                c[j] = t0;
                c[j+1] = t1;

                t0 = c[j+2] + s[j+2];
                t1 = c[j+3] + s[j+3];

                c[j+2] = t0;
                c[j+3] = t1;
            }
            for(; j < dims; j++)
                c[j] += s[j];
            counters->tt.i[k]++;
        }

        if(iter > 0)
            max_dist = 0;

        for(k = 0; k < cluster_count; k++)
        {
            double* c = (double*)(centers->tt.data + k*centers->step);
            if(counters->tt.i[k] != 0)
            {
                double scale = 1./counters->tt.i[k];
                for(j = 0; j < dims; j++)
                    c[j] *= scale;
            }
            else
            {
                float* s;
                i = cvRandInt(&rng) % sample_count;
                s = (float*)(samples->tt.data + i*samples->step);
                for(j = 0; j < dims; j++)
                    c[j] = s[j];
            }
            
            if(iter > 0)
            {
                double dist = 0;
                double* c_o = (double*)(old_centers->tt.data + k*old_centers->step);
                for(j = 0; j < dims; j++)
                {
                    double t = c[j] - c_o[j];
                    dist += t*t;
                }
                if(max_dist < dist)
                    max_dist = dist;
            }
        }

        // assign labels
        for(i = 0; i < sample_count; i++)
        {
            float* s = (float*)(samples->tt.data + i*samples->step);
            int k_best = 0;
            double min_dist = DBL_MAX;

            for(k = 0; k < cluster_count; k++)
            {
                double* c = (double*)(centers->tt.data + k*centers->step);
                double dist = 0;
                
                j = 0;
                for(; j <= dims - 4; j += 4)
                {
                    double t0 = c[j] - s[j];
                    double t1 = c[j+1] - s[j+1];
                    dist += t0*t0 + t1*t1;
                    t0 = c[j+2] - s[j+2];
                    t1 = c[j+3] - s[j+3];
                    dist += t0*t0 + t1*t1;
                }

                for(; j < dims; j++)
                {
                    double t = c[j] - s[j];
                    dist += t*t;
                }

                if(min_dist > dist)
                {
                    min_dist = dist;
                    k_best = k;
                }
            }

            labels->tt.i[i*ids_delta] = k_best;
        }

        if(max_dist < termcrit.epsilon)
            break;

        CC_SWAP(centers, old_centers, temp);
    }

    cvZeroMat(counters);
    for(i = 0; i < sample_count; i++)
        counters->tt.i[labels->tt.i[i]]++;

    // ensure that we do not have empty clusters
    for(k = 0; k < cluster_count; k++)
        if(counters->tt.i[k] == 0)
            for(;;)
            {
                i = cvRandInt(&rng) % sample_count;
                j = labels->tt.i[i];
                if(counters->tt.i[j] > 1)
                {
                    labels->tt.i[i] = k;
                    counters->tt.i[j]--;
                    counters->tt.i[k]++;
                    break;
                }
            }

    __END__;

    imfrees2(im);
}


/*
  Finds real roots of cubic, quadratic or linear equation.
  The original code has been taken from Ken Turkowski web page
  (http://www.worldserver.com/turk/opensource/) and adopted for OpenCV.
  Here is the copyright notice.

  -----------------------------------------------------------------------
  Copyright (C) 1978-1999 Ken Turkowski. <turk@computer.org>
 
    All rights reserved.
 
    Warranty Information
      Even though I have reviewed this software, I make no warranty
      or representation, either express or implied, with respect to this
      software, its quality, accuracy, merchantability, or fitness for a
      particular purpose.  As a result, this software is provided "as is,"
      and you, its user, are assuming the entire risk as to its quality
      and accuracy.
 
    This code may be used and freely distributed as long as it includes
    this copyright notice and the above warranty information.
  -----------------------------------------------------------------------
*/
CC_IMPL int
cvSolveCubic(const img_t* coeffs, img_t* roots)
{
    int n = 0;
    
    CC_FUNCNAME("cvSolveCubic");

    __BEGIN__;

    double a0 = 1., a1, a2, a3;
    double x0 = 0., x1 = 0., x2 = 0.;
    int step = 1, coeff_count;
    
    if(!(coeffs))
        CC_ERROR(!coeffs ? CC_StsNullPtr : CC_StsBadArg, "Input parameter is not a valid matrix");

    if(!(roots))
        CC_ERROR(!roots ? CC_StsNullPtr : CC_StsBadArg, "Output parameter is not a valid matrix");

    if(CC_MAT_TYPE(coeffs) != CC_32F && CC_MAT_TYPE(coeffs) != CC_64F ||
        CC_MAT_TYPE(roots) != CC_32F && CC_MAT_TYPE(roots) != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "Both matrices should be floating-point (single or double precision)");

    coeff_count = coeffs->rows + coeffs->cols - 1;

    if(coeffs->rows != 1 && coeffs->cols != 1 || coeff_count != 3 && coeff_count != 4)
        CC_ERROR(CC_StsBadSize,
        "The matrix of coefficients must be 1-dimensional vector of 3 or 4 elements");

    if(roots->rows != 1 && roots->cols != 1 ||
        roots->rows + roots->cols - 1 != 3)
        CC_ERROR(CC_StsBadSize,
        "The matrix of roots must be 1-dimensional vector of 3 elements");

    if(CC_MAT_TYPE(coeffs) == CC_32F)
    {
        const float* c = coeffs->tt.fl;
        if(coeffs->rows > 1)
            step = coeffs->step/sizeof(c[0]);
        if(coeff_count == 4)
            a0 = c[0], c += step;
        a1 = c[0];
        a2 = c[step];
        a3 = c[step*2];
    }
    else
    {
        const double* c = coeffs->tt.db;
        if(coeffs->rows > 1)
            step = coeffs->step/sizeof(c[0]);
        if(coeff_count == 4)
            a0 = c[0], c += step;
        a1 = c[0];
        a2 = c[step];
        a3 = c[step*2];
    }

    if(a0 == 0)
    {
        if(a1 == 0)
        {
            if(a2 == 0)
                n = a3 == 0 ? -1 : 0;
            else
            {
                // linear equation
                x0 = a3/a2;
                n = 1;
            }
        }
        else
        {
            // quadratic equation
            double d = a2*a2 - 4*a1*a3;
            if(d >= 0)
            {
                double q;
                d = sqrt(d);
                q = (-a2 + (a2 < 0 ? -d : d)) * 0.5;
                x0 = q / a1;
                x1 = a3 / q;
                n = d > 0 ? 2 : 1;
            }
        }
    }
    else
    {
        double Q, R, Qcubed, d;
        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        Q = (a1 * a1 - 3 * a2) * (1./9);
        R = (2 * a1 * a1 * a1 - 9 * a1 * a2 + 27 * a3) * (1./54);
        Qcubed = Q * Q * Q;
        d = Qcubed - R * R;
    
        if(d >= 0)
        {
            double theta = acos(R / sqrt(Qcubed));
            double sqrtQ = sqrt(Q);
            double t0 = -2 * sqrtQ;
            double t1 = theta * (1./3);
            double t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CC_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CC_PI/3)) - t2;
            n = 3;
        }
        else
        {
            double e;
            d = sqrt(-d);
            e = pow(d + fabs(R), 0.333333333333);
            if(R > 0)
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    step = 1;

    if(CC_MAT_TYPE(roots) == CC_32F)
    {
        float* r = roots->tt.fl;
        if(roots->rows > 1)
            step = roots->step/sizeof(r[0]);
        r[0] = (float)x0;
        r[step] = (float)x1;
        r[step*2] = (float)x2;
    }
    else
    {
        double* r = roots->tt.db;
        if(roots->rows > 1)
            step = roots->step/sizeof(r[0]);
        r[0] = x0;
        r[step] = x1;
        r[step*2] = x2;
    }

    __END__;

    return n;
}

#define cvNormalize1(src, dst)  cvNormalize(src, dst, 1, 0, CC_L2, NULL)
CC_IMPL void cvNormalize(const img_t* src, img_t* dst,
                          double a CC_DEFAULT(1.), double b CC_DEFAULT(0.), int norm_type CC_DEFAULT(CC_L2), const img_t* mask CC_DEFAULT(NULL))
{
    img_t tmp[1] = {0};

    CC_FUNCNAME("cvNormalize");

    __BEGIN__;

    double scale, shift;
    
    if(norm_type == CC_MINMAX)
    {
        double smin = 0, smax = 0;
        double dmin = MIN(a, b), dmax = MAX(a, b);
        cvMinMaxLoc(src, &smin, &smax, 0, 0, mask);
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        shift = dmin - smin*scale;
    }
    else if(norm_type == CC_L2 || norm_type == CC_L1 || norm_type == CC_C)
    {
        img_t *s = (img_t*)src, *d = (img_t*)dst;
        
        if(CC_IS_MAT_CONT(s) && CC_IS_MAT_CONT(d) &&
            CC_ARE_TYPES_EQ(s,d) && CC_ARE_SIZES_EQ(s,d) && !mask &&
            s->cols*s->rows <= CC_MAX_INLINE_MAT_OP_SIZE*CC_MAX_INLINE_MAT_OP_SIZE)
        {
            int i, len = s->cols*s->rows;
            double norm = 0, v;

            if(CC_MAT_TYPE(s) == CC_32F)
            {
                const float* sptr = s->tt.fl;
                float* dptr = d->tt.fl;
                
                if(norm_type == CC_L2)
                {
                    for(i = 0; i < len; i++)
                    {
                        v = sptr[i];
                        norm += v*v;
                    }
                    norm = sqrt(norm);
                }
                else if(norm_type == CC_L1)
                    for(i = 0; i < len; i++)
                    {
                        v = fabs((double)sptr[i]);
                        norm += v;
                    }
                else
                    for(i = 0; i < len; i++)
                    {
                        v = fabs((double)sptr[i]);
                        norm = MAX(norm,v);
                    }

                norm = norm > DBL_EPSILON ? 1./norm : 0.;
                for(i = 0; i < len; i++)
                    dptr[i] = (float)(sptr[i]*norm);
                EXIT;
            }

            if(CC_MAT_TYPE(s) == CC_64F)
            {
                const double* sptr = s->tt.db;
                double* dptr = d->tt.db;
                
                if(norm_type == CC_L2)
                {
                    for(i = 0; i < len; i++)
                    {
                        v = sptr[i];
                        norm += v*v;
                    }
                    norm = sqrt(norm);
                }
                else if(norm_type == CC_L1)
                    for(i = 0; i < len; i++)
                    {
                        v = fabs(sptr[i]);
                        norm += v;
                    }
                else
                    for(i = 0; i < len; i++)
                    {
                        v = fabs(sptr[i]);
                        norm = MAX(norm,v);
                    }

                norm = norm > DBL_EPSILON ? 1./norm : 0.;
                for(i = 0; i < len; i++)
                    dptr[i] = sptr[i]*norm;
                EXIT;
            }
        }
        
        scale = cvNorm(src, 0, norm_type, mask);
        scale = scale > DBL_EPSILON ? 1./scale : 0.;
        shift = 0;
    }
    else
        CC_ERROR(CC_StsBadArg, "Unknown/unsupported norm type");
    
    if(!mask)
        cvConvertScale(src, dst, scale, shift);
    else
    {
        img_t stub, *dmat;
        CC_CALL(dmat = cvGetMat(dst, &stub));
        cvCreateMat(tmp, dmat->rows, dmat->cols, dmat->tid, CC_MAT_CN(dmat));
        cvConvertScale(src, tmp, scale, shift);
        cvCopyMask(tmp, dst, mask);
    }

    __END__;

    imfree(tmp);
}

CC_IMPL void cvRandShuffle(img_t* arr, CRNG* rng, double iter_factor)
{
    CC_FUNCNAME("cvRandShuffle");

    __BEGIN__;

    const int sizeof_int = (int)sizeof(int);
    img_t *mat = arr;
    int i, j, k, iters, delta = 0;
    int cont_flag, arr_size, elem_size, cols, step;
    const int pair_buf_sz = 100;
    int* pair_buf = (int*)cvStackAlloc(pair_buf_sz*sizeof(pair_buf[0])*2);
    img_t _pair_buf[1];
    CRNG _rng = cRNG(-1);
    uchar* data = 0;
    int* idata = 0;
    cvMat(_pair_buf, 1, pair_buf_sz*2, CC_32S, 1, pair_buf, CC_AUTO_STEP);
    
    if(!rng)
        rng = &_rng;

    cols = mat->cols;
    step = mat->step;
    arr_size = cols*mat->rows;
    iters = cRound(iter_factor*arr_size)*2;
    cont_flag = CC_IS_MAT_CONT(mat);
    elem_size = CC_TYPE_SIZE(mat->tid);
    if(elem_size % sizeof_int == 0 && (cont_flag || step % sizeof_int == 0))
    {
        idata = mat->tt.i;
        step /= sizeof_int;
        elem_size /= sizeof_int;
    }
    else
        data = mat->tt.data;

    for(i = 0; i < iters; i += delta)
    {
        delta = MIN(iters - i, pair_buf_sz*2);
        _pair_buf->cols = delta;
        cvRandArr(rng, _pair_buf, CC_RAND_UNI, cRealScalar(0), cRealScalar(arr_size));
        
        if(cont_flag)
        {
            if(idata)
                for(j = 0; j < delta; j += 2)
                {
                    int* p = idata + pair_buf[j]*elem_size, *q = idata + pair_buf[j+1]*elem_size, t;
                    for(k = 0; k < elem_size; k++)
                        CC_SWAP(p[k], q[k], t);
                }
            else
                for(j = 0; j < delta; j += 2)
                {
                    uchar* p = data + pair_buf[j]*elem_size, *q = data + pair_buf[j+1]*elem_size, t;
                    for(k = 0; k < elem_size; k++)
                        CC_SWAP(p[k], q[k], t);
                }
        }
        else
        {
            if(idata)
                for(j = 0; j < delta; j += 2)
                {
                    int idx1 = pair_buf[j], idx2 = pair_buf[j+1], row1, row2;
                    int* p, *q, t;
                    row1 = idx1/step; row2 = idx2/step;
                    p = idata + row1*step + (idx1 - row1*cols)*elem_size;
                    q = idata + row2*step + (idx2 - row2*cols)*elem_size;
                    
                    for(k = 0; k < elem_size; k++)
                        CC_SWAP(p[k], q[k], t);
                }
            else
                for(j = 0; j < delta; j += 2)
                {
                    int idx1 = pair_buf[j], idx2 = pair_buf[j+1], row1, row2;
                    uchar* p, *q, t;
                    row1 = idx1/step; row2 = idx2/step;
                    p = data + row1*step + (idx1 - row1*cols)*elem_size;
                    q = data + row2*step + (idx2 - row2*cols)*elem_size;
                    
                    for(k = 0; k < elem_size; k++)
                        CC_SWAP(p[k], q[k], t);
                }
        }
    }

    __END__;
}


CC_IMPL img_t*
cvRange(img_t* mat, double start, double end)
{
    int ok = 0;
    
    CC_FUNCNAME("cvRange");

    __BEGIN__;
    
    double delta;
    TypeId type;
    int step;
    double val = start;
    int i, j;
    int rows, cols;
    int cn = CC_MAT_CN(mat);
    
    rows = mat->rows;
    cols = mat->cols;
    type = CC_MAT_DEPTH(mat);
    delta = (end-start)/(rows*cols);

    if(CC_IS_MAT_CONT(mat))
    {
        cols *= rows;
        rows = 1;
        step = 1;
    }
    else
        step = mat->step / CC_TYPE_SIZE(type);

    if(type == CC_32S)
    {
        int* idata = mat->tt.i;
        int ival = cRound(val), idelta = cRound(delta);

        if(fabs(val - ival) < DBL_EPSILON &&
            fabs(delta - idelta) < DBL_EPSILON)
        {
            for(i = 0; i < rows; i++, idata += step)
                for(j = 0; j < cols; j++, ival += idelta)
                    idata[j] = ival;
        }
        else
        {
            for(i = 0; i < rows; i++, idata += step)
                for(j = 0; j < cols; j++, val += delta)
                    idata[j] = cRound(val);
        }
    }
    else if(type == CC_32F && cn==1)
    {
        float* fdata = mat->tt.fl;
        for(i = 0; i < rows; i++, fdata += step)
            for(j = 0; j < cols; j++, val += delta)
                fdata[j] = (float)val;
    }
    else
        CC_ERROR(CC_StsUnsupportedFormat, "The function only supports 32sC1 and 32fC1 datatypes");

    ok = 1;

    __END__;

    return ok ? mat : 0;
}


