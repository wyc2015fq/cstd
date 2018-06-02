
IPCVAPI_IMPL(CStatus, icvUpdateMotionHistory_8u32f_C1IR,
    (const uchar * silIm, int silStep, float *mhiIm, int mhiStep,
     CSize size, float timestamp, float mhi_duration),
     (silIm, silStep, mhiIm, mhiStep, size, timestamp, mhi_duration))
{
    int x, y;

    /* function processes floating-point images using integer arithmetics */
    suf32_t v;
    int ts, delbound;
    int *mhi = (int *) mhiIm;

    v.f = timestamp;
    ts = v.i;

    if(!silIm || !mhiIm)
        return CC_NULLPTR_ERR;

    if(size.height <= 0 || size.width <= 0 ||
        silStep < size.width || mhiStep < size.width * CC_SIZEOF_FLOAT ||
        (mhiStep & (CC_SIZEOF_FLOAT - 1)) != 0)
        return CC_BADSIZE_ERR;

    if(mhi_duration < 0)
        return CC_BADFACTOR_ERR;

    mhi_duration = timestamp - mhi_duration;

    v.f = mhi_duration;
    delbound = CC_TOGGLE_FLT(v.i);

    mhiStep /= sizeof(mhi[0]);

    if(mhiStep == size.width && silStep == size.width)
    {
        size.width *= size.height;
        size.height = 1;
    }

    if(delbound > 0)
        for(y = 0; y < size.height; y++, silIm += silStep, mhi += mhiStep)
            for(x = 0; x < size.width; x++)
            {
                int val = mhi[x];

                /* val = silIm[x] ? ts : val < delbound ? 0 : val; */
                val &= (val < delbound) - 1;
                val ^= (ts ^ val) & ((silIm[x] == 0) - 1);
                mhi[x] = val;
            }
    else
        for(y = 0; y < size.height; y++, silIm += silStep, mhi += mhiStep)
            for(x = 0; x < size.width; x++)
            {
                int val = mhi[x];

                /* val = silIm[x] ? ts : val < delbound ? 0 : val; */
                val &= (CC_TOGGLE_FLT(val) < delbound) - 1;
                val ^= (ts ^ val) & ((silIm[x] == 0) - 1);
                mhi[x] = val;
            }

    return CC_OK;
}


/* motion templates */
CC_IMPL void
cvUpdateMotionHistory(const void* silhouette, void* mhimg,
                       double timestamp, double mhi_duration)
{
    CSize size;
    img_t silhstub, *silh = silhouette;
    img_t mhistub, *mhi = mhimg;
    int mhi_step, silh_step;

    CC_FUNCNAME("cvUpdateMHIByTime");

    __BEGIN__;

    CC_CALL(silh = cvGetMat(silh, &silhstub));
    CC_CALL(mhi = cvGetMat(mhi, &mhistub));

    if(!CC_IS_MASK_ARR(silh))
        CC_ERROR(CC_StsBadMask, "");

    if(CC_MAT_CN(mhi->tid) > 1)
        CC_ERROR(CC_BadNumChannels, "");

    if(CC_MAT_TYPE(mhi->tid) != CC_32F)
        CC_ERROR(CC_BadDepth, "");

    if(!CC_ARE_SIZES_EQ(mhi, silh))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    size = cvGetMatSize(mhi);

    mhi_step = mhi->step;
    silh_step = silh->step;

    if(CC_IS_MAT_CONT(mhi->tid & silh->tid))
    {
        size.width *= size.height;
        mhi_step = silh_step = CC_STUB_STEP;
        size.height = 1;
    }

    IPPI_CALL(icvUpdateMotionHistory_8u32f_C1IR((const uchar*)(silh->tt.data), silh_step,
                                                  mhi->tt.fl, mhi_step, size,
                                                  (float)timestamp, (float)mhi_duration));
    __END__;
}


CC_IMPL void
cvCalcMotionGradient(const img_t* mhiimg, img_t* maskimg,
                      img_t* orientation,
                      double delta1, double delta2,
                      int aperture_size)
{
    img_t *dX_min = 0, *dY_max = 0;
    IplConvKernel* el = 0;

    CC_FUNCNAME("cvCalcMotionGradient");

    __BEGIN__;

    img_t mhistub, *mhi = mhiimg;
    img_t maskstub, *mask = maskimg;
    img_t orientstub, *orient = orientation;
    img_t dX_min_row, dY_max_row, orient_row, mask_row;
    CSize size;
    int x, y;

    float  gradient_epsilon = 1e-4f * aperture_size * aperture_size;
    float  min_delta, max_delta;

    CC_CALL(mhi = cvGetMat(mhi, &mhistub));
    CC_CALL(mask = cvGetMat(mask, &maskstub));
    CC_CALL(orient = cvGetMat(orient, &orientstub));

    if(!CC_IS_MASK_ARR(mask))
        CC_ERROR(CC_StsBadMask, "");

    if(aperture_size < 3 || aperture_size > 7 || (aperture_size & 1) == 0)
        CC_ERROR(CC_StsOutOfRange, "aperture_size must be 3, 5 or 7");

    if(delta1 <= 0 || delta2 <= 0)
        CC_ERROR(CC_StsOutOfRange, "both delta's must be positive");

    if(CC_MAT_TYPE(mhi->tid) != CC_32F || CC_MAT_TYPE(orient->tid) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "MHI and orientation must be single-channel floating-point images");

    if(!CC_ARE_SIZES_EQ(mhi, mask) || !CC_ARE_SIZES_EQ(orient, mhi))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(orient->tt.data == mhi->tt.data)
        CC_ERROR(CC_StsInplaceNotSupported, "orientation image must be different from MHI");

    if(delta1 > delta2)
    {
        double t;
        CC_SWAP(delta1, delta2, t);
    }

    size = cvGetMatSize(mhi);
    min_delta = (float)delta1;
    max_delta = (float)delta2;
    CC_CALL(dX_min = cvCreateMat(mhi->rows, mhi->cols, CC_32F));
    CC_CALL(dY_max = cvCreateMat(mhi->rows, mhi->cols, CC_32F));

    /* calc Dx and Dy */
    CC_CALL(cvSobel(mhi, dX_min, 1, 0, aperture_size));
    CC_CALL(cvSobel(mhi, dY_max, 0, 1, aperture_size));
    cvGetRow(dX_min, &dX_min_row, 0);
    cvGetRow(dY_max, &dY_max_row, 0);
    cvGetRow(orient, &orient_row, 0);
    cvGetRow(mask, &mask_row, 0);

    /* calc gradient */
    for(y = 0; y < size.height; y++)
    {
        dX_min_row->tt.data = dX_min->tt.data + y*dX_min->step;
        dY_max_row->tt.data = dY_max->tt.data + y*dY_max->step;
        orient_row->tt.data = orient->tt.data + y*orient->step;
        mask_row->tt.data = mask->tt.data + y*mask->step;
        cvCartToPolar(&dX_min_row, &dY_max_row, 0, &orient_row, 1);

        /* make orientation zero where the gradient is very small */
        for(x = 0; x < size.width; x++)
        {
            float dY = dY_max_row->tt.fl[x];
            float dX = dX_min_row->tt.fl[x];

            if(fabs(dX) < gradient_epsilon && fabs(dY) < gradient_epsilon)
            {
                mask_row->tt.data[x] = 0;
                orient_row->tt.i[x] = 0;
            }
            else
                mask_row->tt.data[x] = 1;
        }
    }

    CC_CALL(el = cvCreateStructuringElementEx(aperture_size, aperture_size,
                            aperture_size/2, aperture_size/2, CC_SHAPE_RECT));
    cvErode(mhi, dX_min, el);
    cvDilate(mhi, dY_max, el);

    /* mask off pixels which have little motion difference in their neighborhood */
    for(y = 0; y < size.height; y++)
    {
        dX_min_row->tt.data = dX_min->tt.data + y*dX_min->step;
        dY_max_row->tt.data = dY_max->tt.data + y*dY_max->step;
        mask_row->tt.data = mask->tt.data + y*mask->step;
        orient_row->tt.data = orient->tt.data + y*orient->step;
        
        for(x = 0; x < size.width; x++)
        {
            float d0 = dY_max_row->tt.fl[x] - dX_min_row->tt.fl[x];

            if(mask_row->tt.data[x] == 0 || d0 < min_delta || max_delta < d0)
            {
                mask_row->tt.data[x] = 0;
                orient_row->tt.i[x] = 0;
            }
        }
    }

    __END__;

    cvReleaseMat(&dX_min);
    cvReleaseMat(&dY_max);
    cvReleaseStructuringElement(&el);
}


CC_IMPL double
cvCalcGlobalOrientation(const void* orientation, const void* maskimg, const void* mhiimg,
                         double curr_mhi_timestamp, double mhi_duration)
{
    double  angle = 0;
    int hist_size = 12;
    CvHistogram* hist = 0;

    CC_FUNCNAME("cvCalcGlobalOrientation");

    __BEGIN__;

    img_t mhistub, *mhi = mhiimg;
    img_t maskstub, *mask = maskimg;
    img_t orientstub, *orient = orientation;
    void*  _orient;
    float _ranges[] = { 0, 360 };
    float* ranges = _ranges;
    int base_orient;
    double shift_orient = 0, shift_weight = 0, fbase_orient;
    double a, b;
    float delbound;
    img_t mhi_row, mask_row, orient_row;
    int x, y, mhi_rows, mhi_cols;

    CC_CALL(mhi = cvGetMat(mhi, &mhistub));
    CC_CALL(mask = cvGetMat(mask, &maskstub));
    CC_CALL(orient = cvGetMat(orient, &orientstub));

    if(!CC_IS_MASK_ARR(mask))
        CC_ERROR(CC_StsBadMask, "");

    if(CC_MAT_TYPE(mhi->tid) != CC_32F || CC_MAT_TYPE(orient->tid) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "MHI and orientation must be single-channel floating-point images");

    if(!CC_ARE_SIZES_EQ(mhi, mask) || !CC_ARE_SIZES_EQ(orient, mhi))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(mhi_duration <= 0)
        CC_ERROR(CC_StsOutOfRange, "MHI duration must be positive");

    if(orient->tt.data == mhi->tt.data)
        CC_ERROR(CC_StsInplaceNotSupported, "orientation image must be different from MHI");

    // calculate histogram of different orientation values
    CC_CALL(hist = cvCreateHist(1, &hist_size, CC_HIST_ARRAY, &ranges));
    _orient = orient;
    cvCalcArrHist(&_orient, hist, 0, mask);

    // find the maximum index (the dominant orientation)
    cvGetMinMaxHistValue(hist, 0, 0, 0, &base_orient);
    base_orient *= 360/hist_size;

    // override timestamp with the maximum value in MHI
    cvMinMaxLoc(mhi, 0, &curr_mhi_timestamp, 0, 0, mask);

    // find the shift relative to the dominant orientation as weighted sum of relative angles
    a = 254. / 255. / mhi_duration;
    b = 1. - curr_mhi_timestamp * a;
    fbase_orient = base_orient;
    delbound = (float)(curr_mhi_timestamp - mhi_duration);
    mhi_rows = mhi->rows;
    mhi_cols = mhi->cols;

    if(CC_IS_MAT_CONT(mhi->tid & mask->tid & orient->tid))
    {
        mhi_cols *= mhi_rows;
        mhi_rows = 1;
    }

    cvGetRow(mhi, &mhi_row, 0);
    cvGetRow(mask, &mask_row, 0);
    cvGetRow(orient, &orient_row, 0);

    /*
       a = 254/(255*dt)
       b = 1 - t*a = 1 - 254*t/(255*dur) =
       (255*dt - 254*t)/(255*dt) =
       (dt - (t - dt)*254)/(255*dt);
       --------------------------------------------------------
       ax + b = 254*x/(255*dt) + (dt - (t - dt)*254)/(255*dt) =
       (254*x + dt - (t - dt)*254)/(255*dt) =
       ((x - (t - dt))*254 + dt)/(255*dt) =
       (((x - (t - dt))/dt)*254 + 1)/255 = (((x - low_time)/dt)*254 + 1)/255
     */
    for(y = 0; y < mhi_rows; y++)
    {
        mhi_row->tt.data = mhi->tt.data + mhi->step*y;
        mask_row->tt.data = mask->tt.data + mask->step*y;
        orient_row->tt.data = orient->tt.data + orient->step*y;

        for(x = 0; x < mhi_cols; x++)
            if(mask_row->tt.data[x] != 0 && mhi_row->tt.fl[x] > delbound)
            {
                /*
                   orient in 0..360, base_orient in 0..360
                   -> (rel_angle = orient - base_orient) in -360..360.
                   rel_angle is translated to -180..180
                 */
                double weight = mhi_row->tt.fl[x] * a + b;
                int rel_angle = cRound(orient_row->tt.fl[x] - fbase_orient);

                rel_angle += (rel_angle < -180 ? 360 : 0);
                rel_angle += (rel_angle > 180 ? -360 : 0);

                if(abs(rel_angle) < 90)
                {
                    shift_orient += weight * rel_angle;
                    shift_weight += weight;
                }
            }
    }

    // add the dominant orientation and the relative shift
    if(shift_weight == 0)
        shift_weight = 0.01;

    base_orient = base_orient + cRound(shift_orient / shift_weight);
    base_orient -= (base_orient < 360 ? 0 : 360);
    base_orient += (base_orient >= 0 ? 0 : 360);

    angle = base_orient;

    __END__;

    cvReleaseHist(&hist);
    return angle;
}


CC_IMPL CvSeq*
cvSegmentMotion(const img_t* mhiimg, img_t* segmask, CvMemStorage* storage,
                 double timestamp, double seg_thresh)
{
    CvSeq* components = 0;
    img_t* mask8u = 0;

    CC_FUNCNAME("cvSegmentMotion");

    __BEGIN__;

    img_t mhistub, *mhi = mhiimg;
    img_t maskstub, *mask = segmask;
    suf32_t v, comp_idx;
    int stub_val, ts;
    int x, y;

    if(!storage)
        CC_ERROR(CC_StsNullPtr, "NULL memory storage");

    CC_CALL(mhi = cvGetMat(mhi, &mhistub));
    CC_CALL(mask = cvGetMat(mask, &maskstub));

    if(CC_MAT_TYPE(mhi->tid) != CC_32F || CC_MAT_TYPE(mask->tid) != CC_32F)
        CC_ERROR(CC_BadDepth, "Both MHI and the destination mask");

    if(!CC_ARE_SIZES_EQ(mhi, mask))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    CC_CALL(mask8u = cvCreateMat(mhi->rows + 2, mhi->cols + 2, CC_8UC1));
    cvZero(mask8u);
    cvZero(mask);
    CC_CALL(components = cvCreateSeq(CC_SEQ_KIND_GENERIC, sizeof(CvSeq),
                                       sizeof(CConnectedComp), storage));
    
    v.f = (float)timestamp; ts = v.i;
    v.f = FLT_MAX*0.1f; stub_val = v.i;
    comp_idx.f = 1;

    for(y = 0; y < mhi->rows; y++)
    {
        int* mhi_row = (int*)(mhi->tt.data + y*mhi->step);
        for(x = 0; x < mhi->cols; x++)
        {
            if(mhi_row[x] == 0)
                mhi_row[x] = stub_val;
        }
    }

    for(y = 0; y < mhi->rows; y++)
    {
        int* mhi_row = (int*)(mhi->tt.data + y*mhi->step);
        uchar* mask8u_row = mask8u->tt.data + (y+1)*mask8u->step + 1;

        for(x = 0; x < mhi->cols; x++)
        {
            if(mhi_row[x] == ts && mask8u_row[x] == 0)
            {
                CConnectedComp comp;
                int x1, y1;
                CScalar _seg_thresh = cRealScalar(seg_thresh);
                CPoint seed = cPoint(x,y);

                CC_CALL(cvFloodFill(mhi, seed, cRealScalar(0), _seg_thresh, _seg_thresh,
                                      &comp, CC_FLOODFILL_MASK_ONLY + 2*256 + 4, mask8u));

                for(y1 = 0; y1 < comp.rect.height; y1++)
                {
                    int* mask_row1 = (int*)(mask->tt.data +
                                    (comp.rect.y + y1)*mask->step) + comp.rect.x;
                    uchar* mask8u_row1 = mask8u->tt.data +
                                    (comp.rect.y + y1+1)*mask8u->step + comp.rect.x+1;

                    for(x1 = 0; x1 < comp.rect.width; x1++)
                    {
                        if(mask8u_row1[x1] > 1)
                        {
                            mask8u_row1[x1] = 1;
                            mask_row1[x1] = comp_idx.i;
                        }
                    }
                }
                comp_idx.f++;
                cvSeqPush(components, &comp);
            }
        }
    }

    for(y = 0; y < mhi->rows; y++)
    {
        int* mhi_row = (int*)(mhi->tt.data + y*mhi->step);
        for(x = 0; x < mhi->cols; x++)
        {
            if(mhi_row[x] == stub_val)
                mhi_row[x] = 0;
        }
    }

    __END__;

    cvReleaseMat(&mask8u);
    return components;
}


