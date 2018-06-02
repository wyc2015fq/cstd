
/* Creates new histogram */
CvHistogram *
cvCreateHist(int dims, int *sizes, CvHistType type, float** ranges, int uniform)
{
    CvHistogram *hist = 0;

    CC_FUNCNAME("cvCreateHist");
    __BEGIN__;

    if((unsigned)dims > CC_MAX_DIM)
        CC_ERROR(CC_BadOrder, "Number of dimensions is out of range");

    if(!sizes)
        CC_ERROR(CC_HeaderIsNull, "Null <sizes> pointer");

    CC_CALL(hist = (CvHistogram *)cAlloc(sizeof(CvHistogram)));

    hist->tid = CC_HIST_MAGIC_VAL;
    hist->thresh2 = 0;
    hist->bins = 0;
    if(type == CC_HIST_ARRAY)
    {
        CC_CALL(hist->bins = cvInitMatNDHeader(&hist->mat, dims, sizes,
                                                 CC_HIST_DEFAULT_TYPE));
        CC_CALL(cvCreateData(hist->bins));
    }
    else if(type == CC_HIST_SPARSE)
    {
        CC_CALL(hist->bins = cvCreateSparseMat(dims, sizes, CC_HIST_DEFAULT_TYPE));
    }
    else
    {
        CC_ERROR(CC_StsBadArg, "Invalid histogram type");
    }

    if(ranges)
        CC_CALL(cvSetHistBinRanges(hist, ranges, uniform));

    __END__;

    if(cvGetErrStatus() < 0)
        cvReleaseHist(&hist);

    return hist;
}


/* Creates histogram wrapping header for given array */
CC_IMPL CvHistogram*
cvMakeHistHeaderForArray(int dims, int *sizes, CvHistogram *hist,
                          float *data, float **ranges, int uniform)
{
    CvHistogram* result = 0;
    
    CC_FUNCNAME("cvMakeHistHeaderForArray");

    __BEGIN__;

    if(!hist)
        CC_ERROR(CC_StsNullPtr, "Null histogram header pointer");

    if(!data)
        CC_ERROR(CC_StsNullPtr, "Null data pointer");

    hist->thresh2 = 0;
    hist->tid = CC_HIST_MAGIC_VAL;
    CC_CALL(hist->bins = cvInitMatNDHeader(&hist->mat, dims, sizes,
                                             CC_HIST_DEFAULT_TYPE, data));

    if(ranges)
    {
        if(!uniform)
            CC_ERROR(CC_StsBadArg, "Only uniform bin ranges can be used here "
                                    "(to avoid memory allocation)");
        CC_CALL(cvSetHistBinRanges(hist, ranges, uniform));
    }

    result = hist;

    __END__;

    if(cvGetErrStatus() < 0 && hist)
    {
        hist->tid = 0;
        hist->bins = 0;
    }

    return result;
}


CC_IMPL void
cvReleaseHist(CvHistogram **hist)
{
    CC_FUNCNAME("cvReleaseHist");
    
    __BEGIN__;

    if(!hist)
        CC_ERROR(CC_StsNullPtr, "");

    if(*hist)
    {
        CvHistogram* temp = *hist;

        if(!CC_IS_HIST(temp))
            CC_ERROR(CC_StsBadArg, "Invalid histogram header");

        *hist = 0;

        if(CC_IS_SPARSE_HIST(temp))
            cvRelease(&temp->bins);
        else
        {
            cvReleaseData(temp->bins);
            temp->bins = 0;
        }
        
        if(temp->thresh2)
            cFree(&temp->thresh2);

        cFree(&temp);
    }

    __END__;
}

CC_IMPL void
cvClearHist(CvHistogram *hist)
{
    CC_FUNCNAME("cvClearHist");
    
    __BEGIN__;

    if(!CC_IS_HIST(hist))
        CC_ERROR(CC_StsBadArg, "Invalid histogram header");

    cvZero(hist->bins);

    __END__;
}


// Clears histogram bins that are below than threshold
CC_IMPL void
cvThreshHist(CvHistogram* hist, double thresh)
{
    CC_FUNCNAME("cvThreshHist");

    __BEGIN__;

    if(!CC_IS_HIST(hist))
        CC_ERROR(CC_StsBadArg, "Invalid histogram header");

    if(!CC_IS_SPARSE_MAT(hist->bins))
    {
        img_t mat;
        CC_CALL(cvGetMat(hist->bins, &mat, 0, 1));
        CC_CALL(cvThreshold(&mat, &mat, thresh, 0, CC_THRESH_TOZERO));
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)hist->bins;
        CvSparseMatIterator iterator;
        CvSparseNode *node;
        
        for(node = cvInitSparseMatIterator(mat, &iterator);
             node != 0; node = cvGetNextSparseNode(&iterator))
        {
            float* val = (float*)CC_NODE_VAL(mat, node);
            if(*val <= thresh)
                *val = 0;
        }
    }
    
    __END__;
}


// Normalizes histogram (make sum of the histogram bins == factor)
CC_IMPL void
cvNormalizeHist(CvHistogram* hist, double factor)
{
    double sum = 0;

    CC_FUNCNAME("cvNormalizeHist");
    __BEGIN__;

    if(!CC_IS_HIST(hist))
        CC_ERROR(CC_StsBadArg, "Invalid histogram header");

    if(!CC_IS_SPARSE_HIST(hist))
    {
        img_t mat;
        CC_CALL(cvGetMat(hist->bins, &mat, 0, 1));
        CC_CALL(sum = cvSum(&mat).val[0]);
        if(fabs(sum) < DBL_EPSILON)
            sum = 1;
        CC_CALL(cvScale(&mat, &mat, factor/sum, 0));
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)hist->bins;
        CvSparseMatIterator iterator;
        CvSparseNode *node;
        float scale;
        
        for(node = cvInitSparseMatIterator(mat, &iterator);
             node != 0; node = cvGetNextSparseNode(&iterator))
        {
            sum += *(float*)CC_NODE_VAL(mat,node);
        }

        if(fabs(sum) < DBL_EPSILON)
            sum = 1;
        scale = (float)(factor/sum);

        for(node = cvInitSparseMatIterator(mat, &iterator);
             node != 0; node = cvGetNextSparseNode(&iterator))
        {
            *(float*)CC_NODE_VAL(mat,node) *= scale;
        }
    }

    __END__;
}


// Retrieves histogram global min, max and their positions
CC_IMPL void
cvGetMinMaxHistValue(const CvHistogram* hist,
                      float *value_min, float* value_max,
                      int* idx_min, int* idx_max)
{
    double minVal, maxVal;

    CC_FUNCNAME("cvGetMinMaxHistValue");

    __BEGIN__;

    int i, dims, size[CC_MAX_DIM];

    if(!CC_IS_HIST(hist))
        CC_ERROR(CC_StsBadArg, "Invalid histogram header");

    dims = cvGetDims(hist->bins, size);

    if(!CC_IS_SPARSE_HIST(hist))
    {
        img_t mat;
        CPoint minPt, maxPt;

        CC_CALL(cvGetMat(hist->bins, &mat, 0, 1));
        CC_CALL(cvMinMaxLoc(&mat, &minVal, &maxVal, &minPt, &maxPt));

        if(dims == 1)
        {
            if(idx_min)
                *idx_min = minPt.y + minPt.x;
            if(idx_max)
                *idx_max = maxPt.y + maxPt.x;
        }
        else if(dims == 2)
        {
            if(idx_min)
                idx_min[0] = minPt.y, idx_min[1] = minPt.x;
            if(idx_max)
                idx_max[0] = maxPt.y, idx_max[1] = maxPt.x;
        }
        else if(idx_min || idx_max)
        {
            int imin = minPt.y*mat->cols + minPt.x;
            int imax = maxPt.y*mat->cols + maxPt.x;
            int i;
           
            for(i = dims - 1; i >= 0; i--)
            {
                if(idx_min)
                {
                    int t = imin / size[i];
                    idx_min[i] = imin - t*size[i];
                    imin = t;
                }

                if(idx_max)
                {
                    int t = imax / size[i];
                    idx_max[i] = imax - t*size[i];
                    imax = t;
                }
            }
        }
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)hist->bins;
        CvSparseMatIterator iterator;
        CvSparseNode *node;
        int minv = INT_MAX;
        int maxv = INT_MIN;
        CvSparseNode* minNode = 0;
        CvSparseNode* maxNode = 0;
        const int *_idx_min = 0, *_idx_max = 0;
        suf32_t m;

        for(node = cvInitSparseMatIterator(mat, &iterator);
             node != 0; node = cvGetNextSparseNode(&iterator))
        {
            int value = *(int*)CC_NODE_VAL(mat,node);
            value = CC_TOGGLE_FLT(value);
            if(value < minv)
            {
                minv = value;
                minNode = node;
            }

            if(value > maxv)
            {
                maxv = value;
                maxNode = node;
            }
        }

        if(minNode)
        {
            _idx_min = CC_NODE_IDX(mat,minNode);
            _idx_max = CC_NODE_IDX(mat,maxNode);
            m.i = CC_TOGGLE_FLT(minv); minVal = m.f;
            m.i = CC_TOGGLE_FLT(maxv); maxVal = m.f;
        }
        else
        {
            minVal = maxVal = 0;
        }

        for(i = 0; i < dims; i++)
        {
            if(idx_min)
                idx_min[i] = _idx_min ? _idx_min[i] : -1;
            if(idx_max)
                idx_max[i] = _idx_max ? _idx_max[i] : -1;
        }
    }

    if(value_min)
        *value_min = (float)minVal;

    if(value_max)
        *value_max = (float)maxVal;

    __END__;
}


// Compares two histograms using one of a few methods
CC_IMPL double
cvCompareHist(const CvHistogram* hist1,
               const CvHistogram* hist2,
               int method)
{
    double _result = -1;
    
    CC_FUNCNAME("cvCompareHist");

    __BEGIN__;

    int i, dims1, dims2;
    int size1[CC_MAX_DIM], size2[CC_MAX_DIM], total = 1;
    double result = 0;
        
    if(!CC_IS_HIST(hist1) || !CC_IS_HIST(hist2))
        CC_ERROR(CC_StsBadArg, "Invalid histogram header[s]");

    if(CC_IS_SPARSE_MAT(hist1->bins) != CC_IS_SPARSE_MAT(hist2->bins))
        CC_ERROR(CC_StsUnmatchedFormats, "One of histograms is sparse and other is not");

    CC_CALL(dims1 = cvGetDims(hist1->bins, size1));
    CC_CALL(dims2 = cvGetDims(hist2->bins, size2));
    
    if(dims1 != dims2)
        CC_ERROR(CC_StsUnmatchedSizes,
                  "The histograms have different numbers of dimensions");

    for(i = 0; i < dims1; i++)
    {
        if(size1[i] != size2[i])
            CC_ERROR(CC_StsUnmatchedSizes, "The histograms have different sizes");
        total *= size1[i];
    }


    if(!CC_IS_SPARSE_MAT(hist1->bins))
    {
        union { float* fl; uchar* ptr; } v;
        float *ptr1, *ptr2;
        v.fl = 0;
        CC_CALL(cvGetRawData(hist1->bins, &v.ptr));
        ptr1 = v.fl;
        CC_CALL(cvGetRawData(hist2->bins, &v.ptr));
        ptr2 = v.fl;

        switch(method)
        {
        case CC_COMP_CHISQR:
            for(i = 0; i < total; i++)
            {
                double a = ptr1[i] - ptr2[i];
                double b = ptr1[i] + ptr2[i];
                if(fabs(b) > DBL_EPSILON)
                    result += a*a/b;
            }
            break;
        case CC_COMP_CORREL:
            {
                double s1 = 0, s11 = 0;
                double s2 = 0, s22 = 0;
                double s12 = 0;
                double num, denom2, scale = 1./total;
                
                for(i = 0; i < total; i++)
                {
                    double a = ptr1[i];
                    double b = ptr2[i];

                    s12 += a*b;
                    s1 += a;
                    s11 += a*a;
                    s2 += b;
                    s22 += b*b;
                }

                num = s12 - s1*s2*scale;
                denom2 = (s11 - s1*s1*scale)*(s22 - s2*s2*scale);
                result = fabs(denom2) > DBL_EPSILON ? num/sqrt(denom2) : 1;
            }
            break;
        case CC_COMP_INTERSECT:
            for(i = 0; i < total; i++)
            {
                float a = ptr1[i];
                float b = ptr2[i];
                if(a <= b)
                    result += a;
                else
                    result += b;
            }
            break;
        case CC_COMP_BHATTACHARYYA:
            {
                double s1 = 0, s2 = 0;
                for(i = 0; i < total; i++)
                {
                    double a = ptr1[i];
                    double b = ptr2[i];
                    result += sqrt(a*b);
                    s1 += a;
                    s2 += b;
                }
                s1 *= s2;
                s1 = fabs(s1) > FLT_EPSILON ? 1./sqrt(s1) : 1.;
                result = 1. - result*s1;
                result = sqrt(MAX(result,0.));
            }
            break;
        default:
            CC_ERROR(CC_StsBadArg, "Unknown comparison method");
        }
    }
    else
    {
        CvSparseMat* mat1 = (CvSparseMat*)(hist1->bins);
        CvSparseMat* mat2 = (CvSparseMat*)(hist2->bins);
        CvSparseMatIterator iterator;
        CvSparseNode *node1, *node2;

        if(mat1->heap->active_count > mat2->heap->active_count)
        {
            CvSparseMat* t;
            CC_SWAP(mat1, mat2, t);
        }

        switch(method)
        {
        case CC_COMP_CHISQR:
            for(node1 = cvInitSparseMatIterator(mat1, &iterator);
                 node1 != 0; node1 = cvGetNextSparseNode(&iterator))
            {
                double v1 = *(float*)CC_NODE_VAL(mat1,node1);
                uchar* node2_data = cvPtrND(mat2, CC_NODE_IDX(mat1,node1), 0, 0, &node1->hashval);
                if(!node2_data)
                    result += v1;
                else
                {
                    double v2 = *(float*)node2_data;
                    double a = v1 - v2;
                    double b = v1 + v2;
                    if(fabs(b) > DBL_EPSILON)
                        result += a*a/b;
                }
            }

            for(node2 = cvInitSparseMatIterator(mat2, &iterator);
                 node2 != 0; node2 = cvGetNextSparseNode(&iterator))
            {
                double v2 = *(float*)CC_NODE_VAL(mat2,node2);
                if(!cvPtrND(mat1, CC_NODE_IDX(mat2,node2), 0, 0, &node2->hashval))
                    result += v2;
            }
            break;
        case CC_COMP_CORREL:
            {
                double s1 = 0, s11 = 0;
                double s2 = 0, s22 = 0;
                double s12 = 0;
                double num, denom2, scale = 1./total;
                
                for(node1 = cvInitSparseMatIterator(mat1, &iterator);
                     node1 != 0; node1 = cvGetNextSparseNode(&iterator))
                {
                    double v1 = *(float*)CC_NODE_VAL(mat1,node1);
                    uchar* node2_data = cvPtrND(mat2, CC_NODE_IDX(mat1,node1),
                                                 0, 0, &node1->hashval);
                    if(node2_data)
                    {
                        double v2 = *(float*)node2_data;
                        s12 += v1*v2;
                    }
                    s1 += v1;
                    s11 += v1*v1;
                }

                for(node2 = cvInitSparseMatIterator(mat2, &iterator);
                     node2 != 0; node2 = cvGetNextSparseNode(&iterator))
                {
                    double v2 = *(float*)CC_NODE_VAL(mat2,node2);
                    s2 += v2;
                    s22 += v2*v2;
                }

                num = s12 - s1*s2*scale;
                denom2 = (s11 - s1*s1*scale)*(s22 - s2*s2*scale);
                result = fabs(denom2) > DBL_EPSILON ? num/sqrt(denom2) : 1;
            }
            break;
        case CC_COMP_INTERSECT:
            {
                for(node1 = cvInitSparseMatIterator(mat1, &iterator);
                     node1 != 0; node1 = cvGetNextSparseNode(&iterator))
                {
                    float v1 = *(float*)CC_NODE_VAL(mat1,node1);
                    uchar* node2_data = cvPtrND(mat2, CC_NODE_IDX(mat1,node1),
                                                 0, 0, &node1->hashval);
                    if(node2_data)
                    {
                        float v2 = *(float*)node2_data;
                        if(v1 <= v2)
                            result += v1;
                        else
                            result += v2;
                    }
                }
            }
            break;
        case CC_COMP_BHATTACHARYYA:
            {
                double s1 = 0, s2 = 0;
                
                for(node1 = cvInitSparseMatIterator(mat1, &iterator);
                     node1 != 0; node1 = cvGetNextSparseNode(&iterator))
                {
                    double v1 = *(float*)CC_NODE_VAL(mat1,node1);
                    uchar* node2_data = cvPtrND(mat2, CC_NODE_IDX(mat1,node1),
                                                 0, 0, &node1->hashval);
                    s1 += v1;
                    if(node2_data)
                    {
                        double v2 = *(float*)node2_data;
                        result += sqrt(v1 * v2);
                    }
                }

                for(node1 = cvInitSparseMatIterator(mat2, &iterator);
                     node1 != 0; node1 = cvGetNextSparseNode(&iterator))
                {
                    double v2 = *(float*)CC_NODE_VAL(mat2,node1);
                    s2 += v2;
                }

                s1 *= s2;
                s1 = fabs(s1) > FLT_EPSILON ? 1./sqrt(s1) : 1.;
                result = 1. - result*s1;
                result = sqrt(MAX(result,0.));
            }
            break;
        default:
            CC_ERROR(CC_StsBadArg, "Unknown comparison method");
        }
    }

    _result = result;
    
    __END__;
    
    return _result;
}

// copies one histogram to another
CC_IMPL void
cvCopyHist(const CvHistogram* src, CvHistogram** _dst)
{
    CC_FUNCNAME("cvCopyHist");

    __BEGIN__;

    int eq = 0;
    int is_sparse;
    int i, dims1, dims2;
    int size1[CC_MAX_DIM], size2[CC_MAX_DIM], total = 1;
    float* ranges[CC_MAX_DIM];
    float** thresh = 0;
    CvHistogram* dst;
    
    if(!_dst)
        CC_ERROR(CC_StsNullPtr, "Destination double pointer is NULL");

    dst = *_dst;

    if(!CC_IS_HIST(src) || (dst && !CC_IS_HIST(dst)))
        CC_ERROR(CC_StsBadArg, "Invalid histogram header[s]");

    is_sparse = CC_IS_SPARSE_MAT(src->bins);
    CC_CALL(dims1 = cvGetDims(src->bins, size1));
    for(i = 0; i < dims1; i++)
        total *= size1[i];

    if(dst && is_sparse == CC_IS_SPARSE_MAT(dst->bins))
    {
        CC_CALL(dims2 = cvGetDims(dst->bins, size2));
    
        if(dims1 == dims2)
        {
            for(i = 0; i < dims1; i++)
                if(size1[i] != size2[i])
                    break;
        }

        eq = i == dims1;
    }

    if(!eq)
    {
        cvReleaseHist(_dst);
        CC_CALL(dst = cvCreateHist(dims1, size1,
                 !is_sparse ? CC_HIST_ARRAY : CC_HIST_SPARSE, 0, 0));
        *_dst = dst;
    }

    if(CC_HIST_HAS_RANGES(src))
    {
        if(CC_IS_UNIFORM_HIST(src))
        {
            for(i = 0; i < dims1; i++)
                ranges[i] = (float*)src->thresh[i];
            thresh = ranges;
        }
        else
            thresh = src->thresh2;
        CC_CALL(cvSetHistBinRanges(dst, thresh, CC_IS_UNIFORM_HIST(src)));
    }

    CC_CALL(cvCopy(src->bins, dst->bins));
    
    __END__;
}


// Sets a value range for every histogram bin
CC_IMPL void
cvSetHistBinRanges(CvHistogram* hist, float** ranges, int uniform)
{
    CC_FUNCNAME("cvSetHistBinRanges");
    
    __BEGIN__;

    int dims, size[CC_MAX_DIM], total = 0;
    int i, j;
    
    if(!ranges)
        CC_ERROR(CC_StsNullPtr, "NULL ranges pointer");

    if(!CC_IS_HIST(hist))
        CC_ERROR(CC_StsBadArg, "Invalid histogram header");

    CC_CALL(dims = cvGetDims(hist->bins, size));
    for(i = 0; i < dims; i++)
        total += size[i]+1;
    
    if(uniform)
    {
        for(i = 0; i < dims; i++)
        {
            if(!ranges[i])
                CC_ERROR(CC_StsNullPtr, "One of <ranges> elements is NULL");
            hist->thresh[i][0] = ranges[i][0];
            hist->thresh[i][1] = ranges[i][1];
        }

        hist->tid |= CC_HIST_UNIFORM_FLAG + CC_HIST_RANGES_FLAG;
    }
    else
    {
        float* dim_ranges;

        if(!hist->thresh2)
        {
            CC_CALL(hist->thresh2 = (float**)cAlloc(
                        dims*sizeof(hist->thresh2[0])+
                        total*sizeof(hist->thresh2[0][0])));
        }
        dim_ranges = (float*)(hist->thresh2 + dims);

        for(i = 0; i < dims; i++)
        {
            float val0 = -FLT_MAX;

            if(!ranges[i])
                CC_ERROR(CC_StsNullPtr, "One of <ranges> elements is NULL");
            
            for(j = 0; j <= size[i]; j++)
            {
                float val = ranges[i][j];
                if(val <= val0)
                    CC_ERROR(CC_StsOutOfRange, "Bin ranges should go in ascenting order");
                val0 = dim_ranges[j] = val;
            }

            hist->thresh2[i] = dim_ranges;
            dim_ranges += size[i] + 1;
        }

        hist->tid |= CC_HIST_RANGES_FLAG;
        hist->tid &= ~CC_HIST_UNIFORM_FLAG;
    }

    __END__;
}


#define  ICC_HIST_DUMMY_IDX  (INT_MIN/3)

static CStatus
icvCalcHistLookupTables8u(const CvHistogram* hist, int dims, int* size, int* tab)
{
    const int lo = 0, hi = 256;
    int is_sparse = CC_IS_SPARSE_HIST(hist);
    int have_range = CC_HIST_HAS_RANGES(hist);
    int i, j;
    
    if(!have_range || CC_IS_UNIFORM_HIST(hist))
    {
        for(i = 0; i < dims; i++)
        {
            double a = have_range ? hist->thresh[i][0] : 0;
            double b = have_range ? hist->thresh[i][1] : 256;
            int sz = size[i];
            double scale = sz/(b - a);
            int step = 1;

            if(!is_sparse)
                step = ((CvMatND*)(hist->bins))->dim[i]->step/sizeof(float);

            for(j = lo; j < hi; j++)
            {
                int idx = cFloor((j - a)*scale);
                if((unsigned)idx < (unsigned)sz)
                    idx *= step;
                else
                    idx = ICC_HIST_DUMMY_IDX;

                tab[i*(hi - lo) + j - lo] = idx;
            }
        }
    }
    else
    {
        for(i = 0; i < dims; i++)
        {
            double limit = hist->thresh2[i][0];
            int idx = -1, write_idx = ICC_HIST_DUMMY_IDX, sz = size[i];
            int step = 1;

            if(!is_sparse)
                step = ((CvMatND*)(hist->bins))->dim[i]->step/sizeof(float);

            if(limit > hi)
                limit = hi;
            
            j = lo;
            for(;;)
            {
                for(; j < limit; j++)
                    tab[i*(hi - lo) + j - lo] = write_idx;

                if((unsigned)(++idx) < (unsigned)sz)
                {
                    limit = hist->thresh2[i][idx+1];
                    if(limit > hi)
                        limit = hi;
                    write_idx = idx*step;
                }
                else
                {
                    for(; j < hi; j++)
                        tab[i*(hi - lo) + j - lo] = ICC_HIST_DUMMY_IDX;
                    break;
                }
            }
        }
    }

    return CC_OK;
}


/***************************** C A L C   H I S T O G R A M *************************/

// Calculates histogram for one or more 8u arrays
static CStatus C_STDCALL
    icvCalcHist_8u_C1R(uchar** img, int step, uchar* mask, int maskStep,
                        CSize size, CvHistogram* hist)
{
    int* tab;
    int is_sparse = CC_IS_SPARSE_HIST(hist);
    int dims, histsize[CC_MAX_DIM];
    int i, x;
    CStatus status;

    dims = cvGetDims(hist->bins, histsize);

    tab = (int*)cvStackAlloc(dims*256*sizeof(int));
    status = icvCalcHistLookupTables8u(hist, dims, histsize, tab);

    if(status < 0)
        return status;

    if(!is_sparse)
    {
        int total = 1;
        int* bins = ((CvMatND*)(hist->bins))->tt.i;

        for(i = 0; i < dims; i++)
            total *= histsize[i];

        if(dims <= 3 && total >= -ICC_HIST_DUMMY_IDX)
            return CC_BADSIZE_ERR; // too big histogram

        switch(dims)
        {
        case 1:
            {
            int tab1d[256];
            memset(tab1d, 0, sizeof(tab1d));

            for(; size.height--; img[0] += step)
            {
                uchar* ptr = img[0];
                if(!mask)
                {
                    for(x = 0; x <= size.width - 4; x += 4)
                    {
                        int v0 = ptr[x];
                        int v1 = ptr[x+1];

                        tab1d[v0]++;
                        tab1d[v1]++;

                        v0 = ptr[x+2];
                        v1 = ptr[x+3];

                        tab1d[v0]++;
                        tab1d[v1]++;
                    }

                    for(; x < size.width; x++)
                        tab1d[ptr[x]]++;
                }
                else
                {
                    for(x = 0; x < size.width; x++)
                        if(mask[x])
                            tab1d[ptr[x]]++;
                    mask += maskStep;
                }
            }

            for(i = 0; i < 256; i++)
            {
                int idx = tab[i];
                if(idx >= 0)
                    bins[idx] += tab1d[i];
            }
            }
            break;
        case 2:
            for(; size.height--; img[0] += step, img[1] += step)
            {
                uchar* ptr0 = img[0];
                uchar* ptr1 = img[1];
                if(!mask)
                {
                    for(x = 0; x < size.width; x++)
                    {
                        int v0 = ptr0[x];
                        int v1 = ptr1[x];
                        int idx = tab[v0] + tab[256+v1];

                        if(idx >= 0)
                            bins[idx]++;
                    }
                }
                else
                {
                    for(x = 0; x < size.width; x++)
                    {
                        if(mask[x])
                        {
                            int v0 = ptr0[x];
                            int v1 = ptr1[x];

                            int idx = tab[v0] + tab[256+v1];

                            if(idx >= 0)
                                bins[idx]++;
                        }
                    }
                    mask += maskStep;
                }
            }
            break;
        case 3:
            for(; size.height--; img[0] += step, img[1] += step, img[2] += step)
            {
                uchar* ptr0 = img[0];
                uchar* ptr1 = img[1];
                uchar* ptr2 = img[2];
                if(!mask)
                {
                    for(x = 0; x < size.width; x++)
                    {
                        int v0 = ptr0[x];
                        int v1 = ptr1[x];
                        int v2 = ptr2[x];
                        int idx = tab[v0] + tab[256+v1] + tab[512+v2];

                        if(idx >= 0)
                            bins[idx]++;
                    }
                }
                else
                {
                    for(x = 0; x < size.width; x++)
                    {
                        if(mask[x])
                        {
                            int v0 = ptr0[x];
                            int v1 = ptr1[x];
                            int v2 = ptr2[x];
                            int idx = tab[v0] + tab[256+v1] + tab[512+v2];

                            if(idx >= 0)
                                bins[idx]++;
                        }
                    }
                    mask += maskStep;
                }
            }
            break;
        default:
            for(; size.height--;)
            {
                if(!mask)
                {
                    for(x = 0; x < size.width; x++)
                    {
                        int* binptr = bins;
                        for(i = 0; i < dims; i++)
                        {
                            int idx = tab[i*256 + img[i][x]];
                            if(idx < 0)
                                break;
                            binptr += idx;
                        }
                        if(i == dims)
                            binptr[0]++;
                    }
                }
                else
                {
                    for(x = 0; x < size.width; x++)
                    {
                        if(mask[x])
                        {
                            int* binptr = bins;
                            for(i = 0; i < dims; i++)
                            {
                                int idx = tab[i*256 + img[i][x]];
                                if(idx < 0)
                                    break;
                                binptr += idx;
                            }
                            if(i == dims)
                                binptr[0]++;
                        }
                    }
                    mask += maskStep;
                }

                for(i = 0; i < dims; i++)
                    img[i] += step;
            }
        }
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)(hist->bins);
        int node_idx[CC_MAX_DIM];

        for(; size.height--;)
        {
            if(!mask)
            {
                for(x = 0; x < size.width; x++)
                {
                    for(i = 0; i < dims; i++)
                    {
                        int idx = tab[i*256 + img[i][x]];
                        if(idx < 0)
                            break;
                        node_idx[i] = idx;
                    }
                    if(i == dims)
                    {
                        int* bin = (int*)cvPtrND(mat, node_idx, 0, 1);
                        bin[0]++;
                    }
                }
            }
            else
            {
                for(x = 0; x < size.width; x++)
                {
                    if(mask[x])
                    {
                        for(i = 0; i < dims; i++)
                        {
                            int idx = tab[i*256 + img[i][x]];
                            if(idx < 0)
                                break;
                            node_idx[i] = idx;
                        }
                        if(i == dims)
                        {
                            int* bin = (int*)cvPtrND(mat, node_idx, 0, 1, 0);
                            bin[0]++;
                        }
                    }
                }
                mask += maskStep;
            }

            for(i = 0; i < dims; i++)
                img[i] += step;
        }
    }

    return CC_OK;
}


// Calculates histogram for one or more 32f arrays
static CStatus C_STDCALL
    icvCalcHist_32f_C1R(float** img, int step, uchar* mask, int maskStep,
                         CSize size, CvHistogram* hist)
{
    int is_sparse = CC_IS_SPARSE_HIST(hist);
    int uniform = CC_IS_UNIFORM_HIST(hist);
    int dims, histsize[CC_MAX_DIM];
    double uni_range[CC_MAX_DIM][2];
    int i, x;

    dims = cvGetDims(hist->bins, histsize);
    step /= sizeof(img[0][0]);

    if(uniform)
    {
        for(i = 0; i < dims; i++)
        {
            double t = histsize[i]/((double)hist->thresh[i][1] - hist->thresh[i][0]);
            uni_range[i][0] = t;
            uni_range[i][1] = -t*hist->thresh[i][0];
        }
    }

    if(!is_sparse)
    {
        CvMatND* mat = (CvMatND*)(hist->bins);
        int* bins = mat->tt.i;

        if(uniform)
        {
            switch(dims)
            {
            case 1:
                {
                double a = uni_range[0][0], b = uni_range[0][1];
                int sz = histsize[0];

                for(; size.height--; img[0] += step)
                {
                    float* ptr = img[0];

                    if(!mask)
                    {
                        for(x = 0; x <= size.width - 4; x += 4)
                        {
                            int v0 = cFloor(ptr[x]*a + b);
                            int v1 = cFloor(ptr[x+1]*a + b);

                            if((unsigned)v0 < (unsigned)sz)
                                bins[v0]++;
                            if((unsigned)v1 < (unsigned)sz)
                                bins[v1]++;

                            v0 = cFloor(ptr[x+2]*a + b);
                            v1 = cFloor(ptr[x+3]*a + b);

                            if((unsigned)v0 < (unsigned)sz)
                                bins[v0]++;
                            if((unsigned)v1 < (unsigned)sz)
                                bins[v1]++;
                        }

                        for(; x < size.width; x++)
                        {
                            int v0 = cFloor(ptr[x]*a + b);
                            if((unsigned)v0 < (unsigned)sz)
                                bins[v0]++;
                        }
                    }
                    else
                    {
                        for(x = 0; x < size.width; x++)
                            if(mask[x])
                            {
                                int v0 = cFloor(ptr[x]*a + b);
                                if((unsigned)v0 < (unsigned)sz)
                                    bins[v0]++;
                            }
                        mask += maskStep;
                    }
                }
                }
                break;
            case 2:
                {
                double  a0 = uni_range[0][0], b0 = uni_range[0][1];
                double  a1 = uni_range[1][0], b1 = uni_range[1][1];
                int sz0 = histsize[0], sz1 = histsize[1];
                int step0 = ((CvMatND*)(hist->bins))->dim[0]->step/sizeof(float);

                for(; size.height--; img[0] += step, img[1] += step)
                {
                    float* ptr0 = img[0];
                    float* ptr1 = img[1];

                    if(!mask)
                    {
                        for(x = 0; x < size.width; x++)
                        {
                            int v0 = cFloor(ptr0[x]*a0 + b0);
                            int v1 = cFloor(ptr1[x]*a1 + b1);

                            if((unsigned)v0 < (unsigned)sz0 &&
                                (unsigned)v1 < (unsigned)sz1)
                                bins[v0*step0 + v1]++;
                        }
                    }
                    else
                    {
                        for(x = 0; x < size.width; x++)
                        {
                            if(mask[x])
                            {
                                int v0 = cFloor(ptr0[x]*a0 + b0);
                                int v1 = cFloor(ptr1[x]*a1 + b1);

                                if((unsigned)v0 < (unsigned)sz0 &&
                                    (unsigned)v1 < (unsigned)sz1)
                                    bins[v0*step0 + v1]++;
                            }
                        }
                        mask += maskStep;
                    }
                }
                }
                break;
            default:
                for(; size.height--;)
                {
                    if(!mask)
                    {
                        for(x = 0; x < size.width; x++)
                        {
                            int* binptr = bins;
                            for(i = 0; i < dims; i++)
                            {
                                int idx = cFloor((double)img[i][x]*uni_range[i][0]
                                                 + uni_range[i][1]);
                                if((unsigned)idx >= (unsigned)histsize[i])
                                    break;
                                binptr += idx*(mat->dim[i]->step/sizeof(float));
                            }
                            if(i == dims)
                                binptr[0]++;
                        }
                    }
                    else
                    {
                        for(x = 0; x < size.width; x++)
                        {
                            if(mask[x])
                            {
                                int* binptr = bins;
                                for(i = 0; i < dims; i++)
                                {
                                    int idx = cFloor((double)img[i][x]*uni_range[i][0]
                                                     + uni_range[i][1]);
                                    if((unsigned)idx >= (unsigned)histsize[i])
                                        break;
                                    binptr += idx*(mat->dim[i]->step/sizeof(float));
                                }
                                if(i == dims)
                                    binptr[0]++;
                            }
                        }
                        mask += maskStep;
                    }

                    for(i = 0; i < dims; i++)
                        img[i] += step;
                }
            }
        }
        else
        {
            for(; size.height--;)
            {
                for(x = 0; x < size.width; x++)
                {
                    if(!mask || mask[x])
                    {
                        int* binptr = bins;
                        for(i = 0; i < dims; i++)
                        {
                            float v = img[i][x];
                            float* thresh = hist->thresh2[i];
                            int idx = -1, sz = histsize[i];

                            while(v >= thresh[idx+1] && ++idx < sz)
                                /* nop */;

                            if((unsigned)idx >= (unsigned)sz)
                                break;

                            binptr += idx*(mat->dim[i]->step/sizeof(float));
                        }
                        if(i == dims)
                            binptr[0]++;
                    }
                }

                for(i = 0; i < dims; i++)
                    img[i] += step;
                if(mask)
                    mask += maskStep;
            }
        }
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)(hist->bins);
        int node_idx[CC_MAX_DIM];

        for(; size.height--;)
        {
            if(uniform)
            {
                for(x = 0; x < size.width; x++)
                {
                    if(!mask || mask[x])
                    {
                        for(i = 0; i < dims; i++)
                        {
                            int idx = cFloor(img[i][x]*uni_range[i][0]
                                             + uni_range[i][1]);
                            if((unsigned)idx >= (unsigned)histsize[i])
                                break;
                            node_idx[i] = idx;
                        }
                        if(i == dims)
                        {
                            int* bin = (int*)cvPtrND(mat, node_idx, 0, 1, 0);
                            bin[0]++;
                        }
                    }
                }
            }
            else
            {
                for(x = 0; x < size.width; x++)
                {
                    if(!mask || mask[x])
                    {
                        for(i = 0; i < dims; i++)
                        {
                            float v = img[i][x];
                            float* thresh = hist->thresh2[i];
                            int idx = -1, sz = histsize[i];

                            while(v >= thresh[idx+1] && ++idx < sz)
                                /* nop */;

                            if((unsigned)idx >= (unsigned)sz)
                                break;

                            node_idx[i] = idx;
                        }
                        if(i == dims)
                        {
                            int* bin = (int*)cvPtrND(mat, node_idx, 0, 1, 0);
                            bin[0]++;
                        }
                    }
                }
            }

            for(i = 0; i < dims; i++)
                img[i] += step;

            if(mask)
                mask += maskStep;
        }
    }

    return CC_OK;
}


CC_IMPL void
cvCalcArrHist(img_t** img, CvHistogram* hist,
               int do_not_clear, const img_t* mask)
{
    CC_FUNCNAME("cvCalcHist");

    __BEGIN__;

    uchar* ptr[CC_MAX_DIM];
    uchar* maskptr = 0;
    int maskstep = 0, step = 0;
    int i, dims;
    int cont_flag = -1;
    img_t stub0, *mat0 = 0;
    CvMatND dense;
    CSize size;

    if(!CC_IS_HIST(hist))
        CC_ERROR(CC_StsBadArg, "Bad histogram pointer");

    if(!img)
        CC_ERROR(CC_StsNullPtr, "Null double array pointer");

    CC_CALL(dims = cvGetDims(hist->bins));
    
    for(i = 0; i < dims; i++)
    {
        img_t stub, *mat = img[i];
        CC_CALL(mat = cvGetMat(mat, i == 0 ? &stub0 : &stub, 0, 1));

        if(CC_MAT_CN(mat->tid) != 1)
            CC_ERROR(CC_BadNumChannels, "Only 1-channel arrays are allowed here");

        if(i == 0)
        {
            mat0 = mat;
            step = mat0->step;
        }
        else
        {
            if(!CC_ARE_SIZES_EQ(mat0, mat))
                CC_ERROR(CC_StsUnmatchedSizes, "Not all the planes have equal sizes");

            if(mat0->step != mat->step)
                CC_ERROR(CC_StsUnmatchedSizes, "Not all the planes have equal steps");

            if(!CC_ARE_TYPES_EQ(mat0, mat))
                CC_ERROR(CC_StsUnmatchedFormats, "Not all the planes have equal types");
        }

        cont_flag &= mat->tid;
        ptr[i] = mat->tt.data;
    }

    if(mask)
    {
        img_t stub, *mat = mask;
        CC_CALL(mat = cvGetMat(mat, &stub, 0, 1));

        if(!CC_IS_MASK_ARR(mat))
            CC_ERROR(CC_StsBadMask, "Bad mask array");

        if(!CC_ARE_SIZES_EQ(mat0, mat))
            CC_ERROR(CC_StsUnmatchedSizes,
                "Mask size does not match to other arrays\' size");
        maskptr = mat->tt.data;
        maskstep = mat->step;
        cont_flag &= mat->tid;
    }

    size = cvGetMatSize(mat0);
    if(cont_flag)
    {
        size.width *= size.height;
        size.height = 1;
        maskstep = step = CC_STUB_STEP;
    }

    if(!CC_IS_SPARSE_HIST(hist))
    {
        dense = *(CvMatND*)hist->bins;
        dense.type = (dense.type & ~CC_MAT_TYPE_MASK) | CC_32SC1;
    }

    if(!do_not_clear)
    {
        CC_CALL(cvZero(hist->bins));
    }
    else if(!CC_IS_SPARSE_HIST(hist))
    {
        CC_CALL(cvConvert((CvMatND*)hist->bins, &dense));
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)(hist->bins);
        CvSparseMatIterator iterator;
        CvSparseNode* node;

        for(node = cvInitSparseMatIterator(mat, &iterator);
             node != 0; node = cvGetNextSparseNode(&iterator))
        {
            suf32_t* val = (suf32_t*)CC_NODE_VAL(mat, node);
            val->i = cRound(val->f);
        }
    }

    if(CC_MAT_TYPE(mat0->tid) > CC_8S && !CC_HIST_HAS_RANGES(hist))
        CC_ERROR(CC_StsBadArg, "histogram ranges must be set (via cvSetHistBinRanges) "
                                "before calling the function");

    switch(CC_MAT_TYPE(mat0->tid))
    {
    case CC_8U:
        IPPI_CALL(icvCalcHist_8u_C1R(ptr, step, maskptr, maskstep, size, hist));
	    break;
    case CC_32F:
        {
        union { uchar** ptr; float** fl; } v;
        v.ptr = ptr;
	    IPPI_CALL(icvCalcHist_32f_C1R(v.fl, step, maskptr, maskstep, size, hist));
        }
	    break;
    default:
        CC_ERROR(CC_StsUnsupportedFormat, "Unsupported array type");
    }

    if(!CC_IS_SPARSE_HIST(hist))
    {
        CC_CALL(cvConvert(&dense, (CvMatND*)hist->bins));
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)(hist->bins);
        CvSparseMatIterator iterator;
        CvSparseNode* node;

        for(node = cvInitSparseMatIterator(mat, &iterator);
             node != 0; node = cvGetNextSparseNode(&iterator))
        {
            suf32_t* val = (suf32_t*)CC_NODE_VAL(mat, node);
            val->f = (float)val->i;
        }
    }
    
    __END__;
}


/***************************** B A C K   P R O J E C T *****************************/

// Calculates back project for one or more 8u arrays
static CStatus C_STDCALL
    icvCalcBackProject_8u_C1R(uchar** img, int step, uchar* dst, int dstStep,
                               CSize size, const CvHistogram* hist)
{
    const int small_hist_size = 1<<12;
    int* tab = 0;
    int is_sparse = CC_IS_SPARSE_HIST(hist);
    int dims, histsize[CC_MAX_DIM];
    int i, x;
    CStatus status;

    dims = cvGetDims(hist->bins, histsize);

    tab = (int*)cvStackAlloc(dims*256*sizeof(int));
    status = icvCalcHistLookupTables8u(hist, dims, histsize, tab);
    if(status < 0)
        return status;

    if(!is_sparse)
    {
        int total = 1;
        CvMatND* mat = (CvMatND*)(hist->bins);
        float* bins = mat->tt.fl;
        uchar* buffer = 0;

        for(i = 0; i < dims; i++)
            total *= histsize[i];

        if(dims <= 3 && total >= -ICC_HIST_DUMMY_IDX)
            return CC_BADSIZE_ERR; // too big histogram

        if(dims > 1 && total <= small_hist_size && CC_IS_MAT_CONT(mat->tid))
        {
            buffer = (uchar*)cAlloc(total);
            if(!buffer)
                return CC_OUTOFMEM_ERR;
            for(i = 0; i < total; i++)
            {
                int v = cRound(bins[i]);
                buffer[i] = CC_CAST_8U(v);
            }
        }

        switch(dims)
        {
        case 1:
            {
            uchar tab1d[256];
            for(i = 0; i < 256; i++)
            {
                int idx = tab[i];
                if(idx >= 0)
                {
                    int v = cRound(bins[idx]);
                    tab1d[i] = CC_CAST_8U(v);
                }
                else
                    tab1d[i] = 0;
            }

            for(; size.height--; img[0] += step, dst += dstStep)
            {
                uchar* ptr = img[0];
                for(x = 0; x <= size.width - 4; x += 4)
                {
                    uchar v0 = tab1d[ptr[x]];
                    uchar v1 = tab1d[ptr[x+1]];

                    dst[x] = v0;
                    dst[x+1] = v1;

                    v0 = tab1d[ptr[x+2]];
                    v1 = tab1d[ptr[x+3]];

                    dst[x+2] = v0;
                    dst[x+3] = v1;
                }

                for(; x < size.width; x++)
                    dst[x] = tab1d[ptr[x]];
            }
            }
            break;
        case 2:
            for(; size.height--; img[0] += step, img[1] += step, dst += dstStep)
            {
                uchar* ptr0 = img[0];
                uchar* ptr1 = img[1];

                if(buffer)
                {
                    for(x = 0; x < size.width; x++)
                    {
                        int v0 = ptr0[x];
                        int v1 = ptr1[x];
                        int idx = tab[v0] + tab[256+v1];
                        int v = 0;

                        if(idx >= 0)
                            v = buffer[idx];

                        dst[x] = (uchar)v;
                    }
                }
                else
                {
                    for(x = 0; x < size.width; x++)
                    {
                        int v0 = ptr0[x];
                        int v1 = ptr1[x];
                        int idx = tab[v0] + tab[256+v1];
                        int v = 0;

                        if(idx >= 0)
                        {
                            v = cRound(bins[idx]);
                            v = CC_CAST_8U(v);
                        }

                        dst[x] = (uchar)v;
                    }
                }
            }
            break;
        case 3:
            for(; size.height--; img[0] += step, img[1] += step,
                                  img[2] += step, dst += dstStep)
            {
                uchar* ptr0 = img[0];
                uchar* ptr1 = img[1];
                uchar* ptr2 = img[2];

                if(buffer)
                {
                    for(x = 0; x < size.width; x++)
                    {
                        int v0 = ptr0[x];
                        int v1 = ptr1[x];
                        int v2 = ptr2[x];
                        int idx = tab[v0] + tab[256+v1] + tab[512+v2];
                        int v = 0;

                        if(idx >= 0)
                            v = buffer[idx];

                        dst[x] = (uchar)v;
                    }
                }
                else
                {
                    for(x = 0; x < size.width; x++)
                    {
                        int v0 = ptr0[x];
                        int v1 = ptr1[x];
                        int v2 = ptr2[x];
                        int idx = tab[v0] + tab[256+v1] + tab[512+v2];
                        int v = 0;

                        if(idx >= 0)
                        {
                            v = cRound(bins[idx]);
                            v = CC_CAST_8U(v);
                        }
                        dst[x] = (uchar)v;
                    }
                }
            }
            break;
        default:
            for(; size.height--; dst += dstStep)
            {
                if(buffer)
                {
                    for(x = 0; x < size.width; x++)
                    {
                        uchar* binptr = buffer;
                        int v = 0;

                        for(i = 0; i < dims; i++)
                        {
                            int idx = tab[i*256 + img[i][x]];
                            if(idx < 0)
                                break;
                            binptr += idx;
                        }
                        
                        if(i == dims)
                            v = binptr[0];

                        dst[x] = (uchar)v;
                    }
                }
                else
                {
                    for(x = 0; x < size.width; x++)
                    {
                        float* binptr = bins;
                        int v = 0;

                        for(i = 0; i < dims; i++)
                        {
                            int idx = tab[i*256 + img[i][x]];
                            if(idx < 0)
                                break;
                            binptr += idx;
                        }

                        if(i == dims)
                        {
                            v = cRound(binptr[0]);
                            v = CC_CAST_8U(v);
                        }

                        dst[x] = (uchar)v;
                    }
                }

                for(i = 0; i < dims; i++)
                    img[i] += step;
            }
        }

        cFree(&buffer);
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)(hist->bins);
        int node_idx[CC_MAX_DIM];

        for(; size.height--; dst += dstStep)
        {
            for(x = 0; x < size.width; x++)
            {
                int v = 0;

                for(i = 0; i < dims; i++)
                {
                    int idx = tab[i*256 + img[i][x]];
                    if(idx < 0)
                        break;
                    node_idx[i] = idx;
                }
                if(i == dims)
                {
                    float* bin = (float*)cvPtrND(mat, node_idx, 0, 1, 0);
                    v = cRound(bin[0]);
                    v = CC_CAST_8U(v);
                }

                dst[x] = (uchar)v;
            }

            for(i = 0; i < dims; i++)
                img[i] += step;
        }
    }

    return CC_OK;
}


// Calculates back project for one or more 32f arrays
static CStatus C_STDCALL
    icvCalcBackProject_32f_C1R(float** img, int step, float* dst, int dstStep,
                                CSize size, const CvHistogram* hist)
{
    int is_sparse = CC_IS_SPARSE_HIST(hist);
    int uniform = CC_IS_UNIFORM_HIST(hist);
    int dims, histsize[CC_MAX_DIM];
    double uni_range[CC_MAX_DIM][2];
    int i, x;

    dims = cvGetDims(hist->bins, histsize);
    step /= sizeof(img[0][0]);
    dstStep /= sizeof(dst[0]);

    if(uniform)
    {
        for(i = 0; i < dims; i++)
        {
            double t = ((double)histsize[i])/
                ((double)hist->thresh[i][1] - hist->thresh[i][0]);
            uni_range[i][0] = t;
            uni_range[i][1] = -t*hist->thresh[i][0];
        }
    }

    if(!is_sparse)
    {
        CvMatND* mat = (CvMatND*)(hist->bins);
        float* bins = mat->tt.fl;

        if(uniform)
        {
            switch(dims)
            {
            case 1:
                {
                double a = uni_range[0][0], b = uni_range[0][1];
                int sz = histsize[0];

                for(; size.height--; img[0] += step, dst += dstStep)
                {
                    float* ptr = img[0];

                    for(x = 0; x <= size.width - 4; x += 4)
                    {
                        int v0 = cFloor(ptr[x]*a + b);
                        int v1 = cFloor(ptr[x+1]*a + b);

                        if((unsigned)v0 < (unsigned)sz)
                            dst[x] = bins[v0];
                        else
                            dst[x] = 0;

                        if((unsigned)v1 < (unsigned)sz)
                            dst[x+1] = bins[v1];
                        else
                            dst[x+1] = 0;

                        v0 = cFloor(ptr[x+2]*a + b);
                        v1 = cFloor(ptr[x+3]*a + b);

                        if((unsigned)v0 < (unsigned)sz)
                            dst[x+2] = bins[v0];
                        else
                            dst[x+2] = 0;

                        if((unsigned)v1 < (unsigned)sz)
                            dst[x+3] = bins[v1];
                        else
                            dst[x+3] = 0;
                    }

                    for(; x < size.width; x++)
                    {
                        int v0 = cFloor(ptr[x]*a + b);

                        if((unsigned)v0 < (unsigned)sz)
                            dst[x] = bins[v0];
                        else
                            dst[x] = 0;
                    }
                }
                }
                break;
            case 2:
                {
                double a0 = uni_range[0][0], b0 = uni_range[0][1];
                double a1 = uni_range[1][0], b1 = uni_range[1][1];
                int sz0 = histsize[0], sz1 = histsize[1];
                int step0 = ((CvMatND*)(hist->bins))->dim[0]->step/sizeof(float);

                for(; size.height--; img[0] += step, img[1] += step, dst += dstStep)
                {
                    float* ptr0 = img[0];
                    float* ptr1 = img[1];

                    for(x = 0; x < size.width; x++)
                    {
                        int v0 = cFloor(ptr0[x]*a0 + b0);
                        int v1 = cFloor(ptr1[x]*a1 + b1);

                        if((unsigned)v0 < (unsigned)sz0 &&
                            (unsigned)v1 < (unsigned)sz1)
                            dst[x] = bins[v0*step0 + v1];
                        else
                            dst[x] = 0;
                    }
                }
                }
                break;
            default:
                for(; size.height--; dst += dstStep)
                {
                    for(x = 0; x < size.width; x++)
                    {
                        float* binptr = bins;

                        for(i = 0; i < dims; i++)
                        {
                            int idx = cFloor(img[i][x]*uni_range[i][0]
                                             + uni_range[i][1]);
                            if((unsigned)idx >= (unsigned)histsize[i])
                                break;
                            binptr += idx*(mat->dim[i]->step/sizeof(float));
                        }
                        if(i == dims)
                            dst[x] = binptr[0];
                        else
                            dst[x] = 0;
                    }
                }

                for(i = 0; i < dims; i++)
                    img[i] += step;
            }
        }
        else
        {
            for(; size.height--; dst += dstStep)
            {
                for(x = 0; x < size.width; x++)
                {
                    float* binptr = bins;
                    for(i = 0; i < dims; i++)
                    {
                        float v = img[i][x];
                        float* thresh = hist->thresh2[i];
                        int idx = -1, sz = histsize[i];

                        while(v >= thresh[idx+1] && ++idx < sz)
                            /* nop */;

                        if((unsigned)idx >= (unsigned)sz)
                            break;

                        binptr += idx*(mat->dim[i]->step/sizeof(float));
                    }
                    if(i == dims)
                        dst[x] = binptr[0];
                    else
                        dst[x] = 0;
                }

                for(i = 0; i < dims; i++)
                    img[i] += step;
            }
        }
    }
    else
    {
        CvSparseMat* mat = (CvSparseMat*)(hist->bins);
        int node_idx[CC_MAX_DIM];

        for(; size.height--; dst += dstStep)
        {
            if(uniform)
            {
                for(x = 0; x < size.width; x++)
                {
                    for(i = 0; i < dims; i++)
                    {
                        int idx = cFloor(img[i][x]*uni_range[i][0]
                                         + uni_range[i][1]);
                        if((unsigned)idx >= (unsigned)histsize[i])
                            break;
                        node_idx[i] = idx;
                    }
                    if(i == dims)
                    {
                        float* bin = (float*)cvPtrND(mat, node_idx, 0, 1, 0);
                        dst[x] = bin[0];
                    }
                    else
                        dst[x] = 0;
                }
            }
            else
            {
                for(x = 0; x < size.width; x++)
                {
                    for(i = 0; i < dims; i++)
                    {
                        float v = img[i][x];
                        float* thresh = hist->thresh2[i];
                        int idx = -1, sz = histsize[i];

                        while(v >= thresh[idx+1] && ++idx < sz)
                            /* nop */;

                        if((unsigned)idx >= (unsigned)sz)
                            break;

                        node_idx[i] = idx;
                    }
                    if(i == dims)
                    {
                        float* bin = (float*)cvPtrND(mat, node_idx, 0, 1, 0);
                        dst[x] = bin[0];
                    }
                    else
                        dst[x] = 0;
                }
            }

            for(i = 0; i < dims; i++)
                img[i] += step;
        }
    }

    return CC_OK;
}


CC_IMPL void
cvCalcArrBackProject(img_t** img, img_t* dst, const CvHistogram* hist)
{
    CC_FUNCNAME("cvCalcArrBackProject");

    __BEGIN__;

    uchar* ptr[CC_MAX_DIM];
    uchar* dstptr = 0;
    int dststep = 0, step = 0;
    int i, dims;
    int cont_flag = -1;
    img_t stub0, *mat0 = 0;
    CSize size;

    if(!CC_IS_HIST(hist))
        CC_ERROR(CC_StsBadArg, "Bad histogram pointer");

    if(!img)
        CC_ERROR(CC_StsNullPtr, "Null double array pointer");

    CC_CALL(dims = cvGetDims(hist->bins));
    
    for(i = 0; i <= dims; i++)
    {
        img_t stub, *mat = (i < dims ? img[i] : dst);
        CC_CALL(mat = cvGetMat(mat, i == 0 ? &stub0 : &stub, 0, 1));

        if(CC_MAT_CN(mat->tid) != 1)
            CC_ERROR(CC_BadNumChannels, "Only 1-channel arrays are allowed here");

        if(i == 0)
        {
            mat0 = mat;
            step = mat0->step;
        }
        else
        {
            if(!CC_ARE_SIZES_EQ(mat0, mat))
                CC_ERROR(CC_StsUnmatchedSizes, "Not all the planes have equal sizes");

            if(mat0->step != mat->step)
                CC_ERROR(CC_StsUnmatchedSizes, "Not all the planes have equal steps");

            if(!CC_ARE_TYPES_EQ(mat0, mat))
                CC_ERROR(CC_StsUnmatchedFormats, "Not all the planes have equal types");
        }

        cont_flag &= mat->tid;
        if(i < dims)
            ptr[i] = mat->tt.data;
        else
        {
            dstptr = mat->tt.data;
            dststep = mat->step;
        }
    }

    size = cvGetMatSize(mat0);
    if(cont_flag)
    {
        size.width *= size.height;
        size.height = 1;
        dststep = step = CC_STUB_STEP;
    }

    if(CC_MAT_TYPE(mat0->tid) > CC_8S && !CC_HIST_HAS_RANGES(hist))
        CC_ERROR(CC_StsBadArg, "histogram ranges must be set (via cvSetHistBinRanges) "
                                "before calling the function");

    switch(CC_MAT_TYPE(mat0->tid))
    {
    case CC_8U:
        IPPI_CALL(icvCalcBackProject_8u_C1R(ptr, step, dstptr, dststep, size, hist));
	    break;
    case CC_32F:
        {
        union { uchar** ptr; float** fl; } v;
        v.ptr = ptr;
	    IPPI_CALL(icvCalcBackProject_32f_C1R(v.fl, step,
                                (float*)dstptr, dststep, size, hist));
        }
	    break;
    default:
        CC_ERROR(CC_StsUnsupportedFormat, "Unsupported array type");
    }

    __END__;
}


////////////////////// B A C K   P R O J E C T   P A T C H /////////////////////////

CC_IMPL void
cvCalcArrBackProjectPatch(img_t** arr, img_t* dst, CSize patch_size, CvHistogram* hist,
                           int method, double norm_factor)
{
    CvHistogram* model = 0;
    
    CC_FUNCNAME("cvCalcArrBackProjectPatch");

    __BEGIN__;

    img_t imgstub[CC_MAX_DIM], *img[CC_MAX_DIM];
    IplROI roi;
    img_t dststub, *dstmat;
    int i, dims;
    int x, y;
    CSize size;

    if(!CC_IS_HIST(hist))
        CC_ERROR(CC_StsBadArg, "Bad histogram pointer");

    if(!img)
        CC_ERROR(CC_StsNullPtr, "Null double array pointer");

    if(norm_factor <= 0)
        CC_ERROR(CC_StsOutOfRange,
                  "Bad normalization factor (set it to 1.0 if unsure)");

    if(patch_size.width <= 0 || patch_size.height <= 0)
        CC_ERROR(CC_StsBadSize, "The patch width and height must be positive");

    CC_CALL(dims = cvGetDims(hist->bins));
    CC_CALL(cvCopyHist(hist, &model));
    CC_CALL(cvNormalizeHist(hist, norm_factor));

    for(i = 0; i < dims; i++)
    {
        img_t stub, *mat;
        CC_CALL(mat = cvGetMat(arr[i], &stub, 0, 0));
        CC_CALL(img[i] = cvGetImage(mat, &imgstub[i]));
        img[i]->roi = &roi;
    }

    CC_CALL(dstmat = cvGetMat(dst, &dststub, 0, 0));
    if(CC_MAT_TYPE(dstmat->tid) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat, "Resultant image must have 32fC1 type");

    if(dstmat->cols != img[0]->width - patch_size.width + 1 ||
        dstmat->rows != img[0]->height - patch_size.height + 1)
        CC_ERROR(CC_StsUnmatchedSizes,
            "The output map must be (W-w+1 x H-h+1), "
            "where the input images are (W x H) each and the patch is (w x h)");

    size = cvGetMatSize(dstmat);
    roi.coi = 0;
    roi.width = patch_size.width;
    roi.height = patch_size.height;

    for(y = 0; y < size.height; y++)
    {
        for(x = 0; x < size.width; x++)
        {
            double result;
            
            roi.xOffset = x;
            roi.yOffset = y;

            CC_CALL(cvCalcHist(img, model));

            CC_CALL(cvNormalizeHist(model, norm_factor));
            CC_CALL(result = cvCompareHist(model, hist, method));
            CC_MAT_ELEM(*dstmat, float, y, x) = (float)result;
        }
    }

    __END__;

    cvReleaseHist(&model);
}


// Calculates Bayes probabilistic histograms
CC_IMPL void
cvCalcBayesianProb(CvHistogram** src, int count, CvHistogram** dst)
{
    CC_FUNCNAME("cvCalcBayesianProb");
    
    __BEGIN__;

    int i;
    
    if(!src || !dst)
        CC_ERROR(CC_StsNullPtr, "NULL histogram array pointer");

    if(count < 2)
        CC_ERROR(CC_StsOutOfRange, "Too small number of histograms");
    
    for(i = 0; i < count; i++)
    {
        if(!CC_IS_HIST(src[i]) || !CC_IS_HIST(dst[i]))
            CC_ERROR(CC_StsBadArg, "Invalid histogram header");

        if(!CC_IS_MATND(src[i]->bins) || !CC_IS_MATND(dst[i]->bins))
            CC_ERROR(CC_StsBadArg, "The function supports dense histograms only");
    }
    
    cvZero(dst[0]->bins);
    // dst[0] = src[0] + ... + src[count-1]
    for(i = 0; i < count; i++)
        CC_CALL(cvAdd(src[i]->bins, dst[0]->bins, dst[0]->bins));

    CC_CALL(cvDiv(0, dst[0]->bins, dst[0]->bins));

    // dst[i] = src[i]*(1/dst[0])
    for(i = count - 1; i >= 0; i--)
        CC_CALL(cvMul(src[i]->bins, dst[0]->bins, dst[i]->bins));
    
    __END__;
}


CC_IMPL void
cvCalcProbDensity(const CvHistogram* hist, const CvHistogram* hist_mask,
                   CvHistogram* hist_dens, double scale)
{
    CC_FUNCNAME("cvCalcProbDensity");

    __BEGIN__;

    if(scale <= 0)
        CC_ERROR(CC_StsOutOfRange, "scale must be positive");

    if(!CC_IS_HIST(hist) || !CC_IS_HIST(hist_mask) || !CC_IS_HIST(hist_dens))
        CC_ERROR(CC_StsBadArg, "Invalid histogram pointer[s]");

    {
        img_t* arrs[] = { hist->bins, hist_mask->bins, hist_dens->bins };
        CvMatND stubs[3];
        CvNArrayIterator iterator;

        CC_CALL(cvInitNArrayIterator(3, arrs, 0, stubs, &iterator));

        if(CC_MAT_TYPE(iterator.hdr[0]->tid) != CC_32F)
            CC_ERROR(CC_StsUnsupportedFormat, "All histograms must have 32fC1 type");

        do
        {
            const float* srcdata = (const float*)(iterator.ptr[0]);
            const float* maskdata = (const float*)(iterator.ptr[1]);
            float* dstdata = (float*)(iterator.ptr[2]);
            int i;

            for(i = 0; i < iterator.size.width; i++)
            {
                float s = srcdata[i];
                float m = maskdata[i];
                if(s > FLT_EPSILON)
                    if(m <= s)
                        dstdata[i] = (float)(m*scale/s);
                    else
                        dstdata[i] = (float)scale;
                else
                    dstdata[i] = (float)0;
            }
        }
        while(cvNextNArraySlice(&iterator));
    }

    __END__;
}


CC_IMPL void cvEqualizeHist(const img_t* src, img_t* dst)
{
    CvHistogram* hist = 0;
    img_t* lut = 0;
    
    CC_FUNCNAME("cvEqualizeHist");

    __BEGIN__;

    int i, hist_sz = 256;
    CSize img_sz;
    float scale;
    float* h;
    int sum = 0;
    TypeId type;
    
    CC_CALL(type = cvGetElemType(src));
    if(type != CC_8UC1)
        CC_ERROR(CC_StsUnsupportedFormat, "Only 8uC1 images are supported");

    CC_CALL(hist = cvCreateHist(1, &hist_sz, CC_HIST_ARRAY));
    CC_CALL(lut = cvCreateMat(1, 256, CC_8UC1));
    CC_CALL(cvCalcArrHist((img_t**)&src, hist));
    CC_CALL(img_sz = cvGetSize(src));
    scale = 255.f/(img_sz.width*img_sz.height);
    h = (float*)cvPtr1D(hist->bins, 0);

    for(i = 0; i < hist_sz; i++)
    {
        sum += cRound(h[i]);
        lut->tt.data[i] = (uchar)cRound(sum*scale);
    }

    lut->tt.data[0] = 0;
    CC_CALL(cvLUT(src, dst, lut));

    __END__;

    cvReleaseHist(&hist);
    cvReleaseMat(&lut);
}

/* Implementation of RTTI and Generic Functions for CvHistogram */
#define CC_TYPE_NAME_HIST "opencv-hist"

static int icvIsHist(const void * ptr){
	return CC_IS_HIST(((CvHistogram*)ptr));
}

static CvHistogram * icvCloneHist(const CvHistogram * src){
	CvHistogram * dst=NULL;
	cvCopyHist(src, &dst);
	return dst;
}

static void *icvReadHist(CvFileStorage * fs, CvFileNode * node){
	CvHistogram * h = (CvHistogram *) cAlloc(sizeof(CvHistogram));
	int is_uniform = 0;
	int have_ranges = 0;

	CC_FUNCNAME("icvReadHist");
	__BEGIN__;

	is_uniform = cvReadIntByName(fs, node, "is_uniform", 0);
	have_ranges = cvReadIntByName(fs, node, "have_ranges", 0);
	h->tid = CC_HIST_MAGIC_VAL | 
		      (is_uniform ? CC_HIST_UNIFORM_FLAG : 0) |
			  (have_ranges ? CC_HIST_RANGES_FLAG : 0);

	if(is_uniform){
		// read histogram bins
		CvMatND * mat = (CvMatND *) cvReadByName(fs, node, "mat");
		int sizes[CC_MAX_DIM];
		int i;
		if(!CC_IS_MATND(mat)){
			CC_ERROR(CC_StsError, "Expected CvMatND");
		}
		for(i=0; i<mat->dims; i++){
			sizes[i] = mat->dim[i].size;
		}

		cvInitMatNDHeader(&(h->mat), mat->dims, sizes, mat->tid, mat->tt.data);
		h->bins = &(h->mat);
		
		// take ownership of refcount pointer as well
		h->mat.refcount = mat->refcount;

		// increase refcount so freeing temp header doesn't free data
		cvIncRefData(mat); 
		
		// free temporary header
		cvReleaseMatND(&mat);
	}
	else{
		h->bins = cvReadByName(fs, node, "bins");
		if(!CC_IS_SPARSE_MAT(h->bins)){
			CC_ERROR(CC_StsError, "Unknown Histogram type");
		}
	}

	// read thresholds
	if(have_ranges){
		int i;
		int dims;
		int size[CC_MAX_DIM];
		int total = 0;
		CvSeqReader reader;
		CvFileNode * thresh_node;

		CC_CALL(dims = cvGetDims(h->bins, size));
		for(i = 0; i < dims; i++){
			total += size[i]+1;
		}

		thresh_node = cvGetFileNodeByName(fs, node, "thresh");
		if(!thresh_node){
			CC_ERROR(CC_StsError, "'thresh' node is missing");
		}
		cvStartReadRawData(fs, thresh_node, &reader);

		if(is_uniform){
			for(i=0; i<dims; i++){
				cvReadRawDataSlice(fs, &reader, 2, h->thresh[i], "f");
			}
		}
		else{
			float* dim_ranges;
			CC_CALL(h->thresh2 = (float**)cAlloc(
						dims*sizeof(h->thresh2[0])+
						total*sizeof(h->thresh2[0][0])));
			dim_ranges = (float*)(h->thresh2 + dims);
			for(i=0; i < dims; i++){
				h->thresh2[i] = dim_ranges;
				cvReadRawDataSlice(fs, &reader, size[i]+1, dim_ranges, "f");
				dim_ranges += size[i] + 1;
			}
		}

	}
	
	__END__;

	return h;
}

static void icvWriteHist(CvFileStorage* fs, const char* name, const void* struct_ptr, 
		CvAttrList /*attributes*/){
	const CvHistogram * hist = (const CvHistogram *) struct_ptr;
	int sizes[CC_MAX_DIM];
	int dims;
	int i;
	int is_uniform, have_ranges;

	CC_FUNCNAME("icvWriteHist");
	__BEGIN__;
 
	cvWriteStructBegin(fs, name, CC_NODE_MAP, CC_TYPE_NAME_HIST);

	is_uniform = (CC_IS_UNIFORM_HIST(hist) ? 1 : 0);
	have_ranges = (hist->tid & CC_HIST_RANGES_FLAG ? 1 : 0);
	
	cvWriteInt(fs, "is_uniform", is_uniform);
	cvWriteInt(fs, "have_ranges", have_ranges);
	if(CC_IS_UNIFORM_HIST(hist)){
		cvWrite(fs, "mat", &(hist->mat));
	}
	else if(CC_IS_SPARSE_HIST(hist)){
		cvWrite(fs, "bins", hist->bins);
	}
	else{
		CC_ERROR(CC_StsError, "Unknown Histogram Type");
	}

	// write thresholds
	if(have_ranges){
		dims = cvGetDims(hist->bins, sizes);
		cvWriteStructBegin(fs, "thresh", CC_NODE_SEQ + CC_NODE_FLOW);
		if(is_uniform){
			for(i=0; i<dims; i++){
				cvWriteRawData(fs, hist->thresh[i], 2, "f");
			}
		}
		else{
			for(i=0; i<dims; i++){
				cvWriteRawData(fs, hist->thresh2[i], sizes[i]+1, "f");
			}
		}
		cvWriteStructEnd(fs);
	}

	cvWriteStructEnd(fs);
	__END__;
}


CvType hist_type(CC_TYPE_NAME_HIST, icvIsHist, (CvReleaseFunc)cvReleaseHist,
                  icvReadHist, icvWriteHist, (CvCloneFunc)icvCloneHist);



