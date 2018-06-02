
typedef struct CvFFillSegment
{
    ushort y;
    ushort l;
    ushort r;
    ushort prevl;
    ushort prevr;
    short dir;
}
CvFFillSegment;

#define UP 1
#define DOWN -1             

#define ICC_PUSH(Y, L, R, PREV_L, PREV_R, DIR)\
{                                               \
    tail->y = (ushort)(Y);                      \
    tail->l = (ushort)(L);                      \
    tail->r = (ushort)(R);                      \
    tail->prevl = (ushort)(PREV_L);             \
    tail->prevr = (ushort)(PREV_R);             \
    tail->dir = (short)(DIR);                   \
    if(++tail >= buffer_end)                  \
        tail = buffer;                          \
}


#define ICC_POP(Y, L, R, PREV_L, PREV_R, DIR) \
{                                               \
    Y = head->y;                                \
    L = head->l;                                \
    R = head->r;                                \
    PREV_L = head->prevl;                       \
    PREV_R = head->prevr;                       \
    DIR = head->dir;                            \
    if(++head >= buffer_end)                  \
        head = buffer;                          \
}


#define ICC_EQ_C3(p1, p2) \
    ((p1)[0] == (p2)[0] && (p1)[1] == (p2)[1] && (p1)[2] == (p2)[2])

#define ICC_SET_C3(p, q) \
    ((p)[0] = (q)[0], (p)[1] = (q)[1], (p)[2] = (q)[2])

/****************************************************************************************\
*              Simple Floodfill (repainting single-color connected component)            *
\****************************************************************************************/

static CStatus
icvFloodFill_8u_CnIR(uchar* pImage, int step, CSize roi, CPoint seed,
                      uchar* _newVal, CConnectedComp* region, int flags,
                      CvFFillSegment* buffer, int buffer_size, int cn)
{
    uchar* img = pImage + step * seed.y;
    int i, L, R; 
    int area = 0;
    int val0[] = {0,0,0};
    uchar newVal[] = {0,0,0};
    int XMin, XMax, YMin = seed.y, YMax = seed.y;
    int _8_connectivity = (flags & 255) == 8;
    CvFFillSegment* buffer_end = buffer + buffer_size, *head = buffer, *tail = buffer;

    L = R = XMin = XMax = seed.x;

    if(cn == 1)
    {
        val0[0] = img[L];
        newVal[0] = _newVal[0];

        img[L] = newVal[0];

        while(++R < roi.width && img[R] == val0[0])
            img[R] = newVal[0];

        while(--L >= 0 && img[L] == val0[0])
            img[L] = newVal[0];
    }
    else
    {
        assert(cn == 3);
        ICC_SET_C3(val0, img + L*3);
        ICC_SET_C3(newVal, _newVal);
        
        ICC_SET_C3(img + L*3, newVal);
    
        while(--L >= 0 && ICC_EQ_C3(img + L*3, val0))
            ICC_SET_C3(img + L*3, newVal);
    
        while(++R < roi.width && ICC_EQ_C3(img + R*3, val0))
            ICC_SET_C3(img + R*3, newVal);
    }

    XMax = --R;
    XMin = ++L;
    ICC_PUSH(seed.y, L, R, R + 1, R, UP);

    while(head != tail)
    {
        int k, YC, PL, PR, dir;
        ICC_POP(YC, L, R, PL, PR, dir);

        int data[][3] =
        {
            {-dir, L - _8_connectivity, R + _8_connectivity},
            {dir, L - _8_connectivity, PL - 1},
            {dir, PR + 1, R + _8_connectivity}
        };

        if(region)
        {
            area += R - L + 1;

            if(XMax < R) XMax = R;
            if(XMin > L) XMin = L;
            if(YMax < YC) YMax = YC;
            if(YMin > YC) YMin = YC;
        }

        for(k = 0/*(unsigned)(YC - dir) >= (unsigned)roi.height*/; k < 3; k++)
        {
            dir = data[k][0];
            img = pImage + (YC + dir) * step;
            int left = data[k][1];
            int right = data[k][2];

            if((unsigned)(YC + dir) >= (unsigned)roi.height)
                continue;

            if(cn == 1)
                for(i = left; i <= right; i++)
                {
                    if((unsigned)i < (unsigned)roi.width && img[i] == val0[0])
                    {
                        int j = i;
                        img[i] = newVal[0];
                        while(--j >= 0 && img[j] == val0[0])
                            img[j] = newVal[0];

                        while(++i < roi.width && img[i] == val0[0])
                            img[i] = newVal[0];

                        ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                    }
                }
            else
                for(i = left; i <= right; i++)
                {
                    if((unsigned)i < (unsigned)roi.width && ICC_EQ_C3(img + i*3, val0))
                    {
                        int j = i;
                        ICC_SET_C3(img + i*3, newVal);
                        while(--j >= 0 && ICC_EQ_C3(img + j*3, val0))
                            ICC_SET_C3(img + j*3, newVal);

                        while(++i < roi.width && ICC_EQ_C3(img + i*3, val0))
                            ICC_SET_C3(img + i*3, newVal);

                        ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                    }
                }
        }
    }

    if(region)
    {
        region->area = area;
        region->rect.x = XMin;
        region->rect.y = YMin;
        region->rect.width = XMax - XMin + 1;
        region->rect.height = YMax - YMin + 1;
        region->value = cScalar(newVal[0], newVal[1], newVal[2], 0);
    }

    return CC_NO_ERR;
}


/* because all the operations on floats that are done during non-gradient floodfill
   are just copying and comparison on equality,
   we can do the whole op on 32-bit integers instead */
static CStatus
icvFloodFill_32f_CnIR(int* pImage, int step, CSize roi, CPoint seed,
                       int* _newVal, CConnectedComp* region, int flags,
                       CvFFillSegment* buffer, int buffer_size, int cn)
{
    int* img = pImage + (step /= sizeof(pImage[0])) * seed.y;
    int i, L, R; 
    int area = 0;
    int val0[] = {0,0,0};
    int newVal[] = {0,0,0};
    int XMin, XMax, YMin = seed.y, YMax = seed.y;
    int _8_connectivity = (flags & 255) == 8;
    CvFFillSegment* buffer_end = buffer + buffer_size, *head = buffer, *tail = buffer;

    L = R = XMin = XMax = seed.x;

    if(cn == 1)
    {
        val0[0] = img[L];
        newVal[0] = _newVal[0];

        img[L] = newVal[0];

        while(++R < roi.width && img[R] == val0[0])
            img[R] = newVal[0];

        while(--L >= 0 && img[L] == val0[0])
            img[L] = newVal[0];
    }
    else
    {
        assert(cn == 3);
        ICC_SET_C3(val0, img + L*3);
        ICC_SET_C3(newVal, _newVal);
        
        ICC_SET_C3(img + L*3, newVal);
    
        while(--L >= 0 && ICC_EQ_C3(img + L*3, val0))
            ICC_SET_C3(img + L*3, newVal);
    
        while(++R < roi.width && ICC_EQ_C3(img + R*3, val0))
            ICC_SET_C3(img + R*3, newVal);
    }

    XMax = --R;
    XMin = ++L;
    ICC_PUSH(seed.y, L, R, R + 1, R, UP);

    while(head != tail)
    {
        int k, YC, PL, PR, dir;
        ICC_POP(YC, L, R, PL, PR, dir);

        int data[][3] =
        {
            {-dir, L - _8_connectivity, R + _8_connectivity},
            {dir, L - _8_connectivity, PL - 1},
            {dir, PR + 1, R + _8_connectivity}
        };

        if(region)
        {
            area += R - L + 1;

            if(XMax < R) XMax = R;
            if(XMin > L) XMin = L;
            if(YMax < YC) YMax = YC;
            if(YMin > YC) YMin = YC;
        }

        for(k = 0/*(unsigned)(YC - dir) >= (unsigned)roi.height*/; k < 3; k++)
        {
            dir = data[k][0];
            img = pImage + (YC + dir) * step;
            int left = data[k][1];
            int right = data[k][2];

            if((unsigned)(YC + dir) >= (unsigned)roi.height)
                continue;

            if(cn == 1)
                for(i = left; i <= right; i++)
                {
                    if((unsigned)i < (unsigned)roi.width && img[i] == val0[0])
                    {
                        int j = i;
                        img[i] = newVal[0];
                        while(--j >= 0 && img[j] == val0[0])
                            img[j] = newVal[0];

                        while(++i < roi.width && img[i] == val0[0])
                            img[i] = newVal[0];

                        ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                    }
                }
            else
                for(i = left; i <= right; i++)
                {
                    if((unsigned)i < (unsigned)roi.width && ICC_EQ_C3(img + i*3, val0))
                    {
                        int j = i;
                        ICC_SET_C3(img + i*3, newVal);
                        while(--j >= 0 && ICC_EQ_C3(img + j*3, val0))
                            ICC_SET_C3(img + j*3, newVal);

                        while(++i < roi.width && ICC_EQ_C3(img + i*3, val0))
                            ICC_SET_C3(img + i*3, newVal);

                        ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                    }
                }
        }
    }

    if(region)
    {
        suf32_t v0, v1, v2;
        region->area = area;
        region->rect.x = XMin;
        region->rect.y = YMin;
        region->rect.width = XMax - XMin + 1;
        region->rect.height = YMax - YMin + 1;
        v0.i = newVal[0]; v1.i = newVal[1]; v2.i = newVal[2];
        region->value = cScalar(v0.f, v1.f, v2.f);
    }

    return CC_NO_ERR;
}

/****************************************************************************************\
*                                   Gradient Floodfill                                   *
\****************************************************************************************/

#define DIFF_INT_C1(p1,p2) ((unsigned)((p1)[0] - (p2)[0] + d_lw[0]) <= interval[0])

#define DIFF_INT_C3(p1,p2) ((unsigned)((p1)[0] - (p2)[0] + d_lw[0])<= interval[0] && \
                            (unsigned)((p1)[1] - (p2)[1] + d_lw[1])<= interval[1] && \
                            (unsigned)((p1)[2] - (p2)[2] + d_lw[2])<= interval[2])

#define DIFF_FLT_C1(p1,p2) (fabs((p1)[0] - (p2)[0] + d_lw[0]) <= interval[0])

#define DIFF_FLT_C3(p1,p2) (fabs((p1)[0] - (p2)[0] + d_lw[0]) <= interval[0] && \
                            fabs((p1)[1] - (p2)[1] + d_lw[1]) <= interval[1] && \
                            fabs((p1)[2] - (p2)[2] + d_lw[2]) <= interval[2])

static CStatus
icvFloodFill_Grad_8u_CnIR(uchar* pImage, int step, uchar* pMask, int maskStep,
                           CSize /*roi*/, CPoint seed, uchar* _newVal, uchar* _d_lw,
                           uchar* _d_up, CConnectedComp* region, int flags,
                           CvFFillSegment* buffer, int buffer_size, int cn)
{
    uchar* img = pImage + step*seed.y;
    uchar* mask = (pMask += maskStep + 1) + maskStep*seed.y;
    int i, L, R;
    int area = 0;
    int sum[] = {0,0,0}, val0[] = {0,0,0};
    uchar newVal[] = {0,0,0};
    int d_lw[] = {0,0,0};
    unsigned interval[] = {0,0,0};
    int XMin, XMax, YMin = seed.y, YMax = seed.y;
    int _8_connectivity = (flags & 255) == 8;
    int fixedRange = flags & CC_FLOODFILL_FIXED_RANGE;
    int fillImage = (flags & CC_FLOODFILL_MASK_ONLY) == 0;
    uchar newMaskVal = (uchar)(flags & 0xff00 ? flags >> 8 : 1);
    CvFFillSegment* buffer_end = buffer + buffer_size, *head = buffer, *tail = buffer;

    L = R = seed.x;
    if(mask[L])
        return CC_OK;

    mask[L] = newMaskVal;

    for(i = 0; i < cn; i++)
    {
        newVal[i] = _newVal[i];
        d_lw[i] = _d_lw[i];
        interval[i] = (unsigned)(_d_up[i] + _d_lw[i]);
        if(fixedRange)
            val0[i] = img[L*cn+i];
    }

    if(cn == 1)
    {
        if(fixedRange)
        {
            while(!mask[R + 1] && DIFF_INT_C1(img + (R+1), val0))
                mask[++R] = newMaskVal;

            while(!mask[L - 1] && DIFF_INT_C1(img + (L-1), val0))
                mask[--L] = newMaskVal;
        }
        else
        {
            while(!mask[R + 1] && DIFF_INT_C1(img + (R+1), img + R))
                mask[++R] = newMaskVal;

            while(!mask[L - 1] && DIFF_INT_C1(img + (L-1), img + L))
                mask[--L] = newMaskVal;
        }
    }
    else
    {
        if(fixedRange)
        {
            while(!mask[R + 1] && DIFF_INT_C3(img + (R+1)*3, val0))
                mask[++R] = newMaskVal;

            while(!mask[L - 1] && DIFF_INT_C3(img + (L-1)*3, val0))
                mask[--L] = newMaskVal;
        }
        else
        {
            while(!mask[R + 1] && DIFF_INT_C3(img + (R+1)*3, img + R*3))
                mask[++R] = newMaskVal;

            while(!mask[L - 1] && DIFF_INT_C3(img + (L-1)*3, img + L*3))
                mask[--L] = newMaskVal;
        }
    }

    XMax = R;
    XMin = L;
    ICC_PUSH(seed.y, L, R, R + 1, R, UP);

    while(head != tail)
    {
        int k, YC, PL, PR, dir, curstep;
        ICC_POP(YC, L, R, PL, PR, dir);

        int data[][3] =
        {
            {-dir, L - _8_connectivity, R + _8_connectivity},
            {dir, L - _8_connectivity, PL - 1},
            {dir, PR + 1, R + _8_connectivity}
        };

        unsigned length = (unsigned)(R-L);

        if(region)
        {
            area += (int)length + 1;

            if(XMax < R) XMax = R;
            if(XMin > L) XMin = L;
            if(YMax < YC) YMax = YC;
            if(YMin > YC) YMin = YC;
        }

        if(cn == 1)
        {
            for(k = 0; k < 3; k++)
            {
                dir = data[k][0];
                curstep = dir * step;
                img = pImage + (YC + dir) * step;
                mask = pMask + (YC + dir) * maskStep;
                int left = data[k][1];
                int right = data[k][2];

                if(fixedRange)
                    for(i = left; i <= right; i++)
                    {
                        if(!mask[i] && DIFF_INT_C1(img + i, val0))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_INT_C1(img + j, val0))
                                mask[j] = newMaskVal;

                            while(!mask[++i] && DIFF_INT_C1(img + i, val0))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
                else if(!_8_connectivity)
                    for(i = left; i <= right; i++)
                    {
                        if(!mask[i] && DIFF_INT_C1(img + i, img - curstep + i))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_INT_C1(img + j, img + (j+1)))
                                mask[j] = newMaskVal;

                            while(!mask[++i] &&
                                   (DIFF_INT_C1(img + i, img + (i-1)) ||
                                   (DIFF_INT_C1(img + i, img + i - curstep) && i <= R)))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
                else
                    for(i = left; i <= right; i++)
                    {
                        int idx, val[1];
                
                        if(!mask[i] &&
                            ((val[0] = img[i],
                            (unsigned)(idx = i-L-1) <= length) &&
                            DIFF_INT_C1(val, img - curstep + (i-1)) ||
                            (unsigned)(++idx) <= length &&
                            DIFF_INT_C1(val, img - curstep + i) ||
                            (unsigned)(++idx) <= length &&
                            DIFF_INT_C1(val, img - curstep + (i+1))))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_INT_C1(img + j, img + (j+1)))
                                mask[j] = newMaskVal;

                            while(!mask[++i] &&
                                   ((val[0] = img[i],
                                   DIFF_INT_C1(val, img + (i-1))) ||
                                   ((unsigned)(idx = i-L-1) <= length &&
                                   DIFF_INT_C1(val, img - curstep + (i-1))) ||
                                   (unsigned)(++idx) <= length &&
                                   DIFF_INT_C1(val, img - curstep + i) ||
                                   (unsigned)(++idx) <= length &&
                                   DIFF_INT_C1(val, img - curstep + (i+1))))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
            }

            img = pImage + YC * step;
            if(fillImage)
                for(i = L; i <= R; i++)
                    img[i] = newVal[0];
            else if(region)
                for(i = L; i <= R; i++)
                    sum[0] += img[i];
        }
        else
        {
            for(k = 0; k < 3; k++)
            {
                dir = data[k][0];
                curstep = dir * step;
                img = pImage + (YC + dir) * step;
                mask = pMask + (YC + dir) * maskStep;
                int left = data[k][1];
                int right = data[k][2];

                if(fixedRange)
                    for(i = left; i <= right; i++)
                    {
                        if(!mask[i] && DIFF_INT_C3(img + i*3, val0))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_INT_C3(img + j*3, val0))
                                mask[j] = newMaskVal;

                            while(!mask[++i] && DIFF_INT_C3(img + i*3, val0))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
                else if(!_8_connectivity)
                    for(i = left; i <= right; i++)
                    {
                        if(!mask[i] && DIFF_INT_C3(img + i*3, img - curstep + i*3))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_INT_C3(img + j*3, img + (j+1)*3))
                                mask[j] = newMaskVal;

                            while(!mask[++i] &&
                                   (DIFF_INT_C3(img + i*3, img + (i-1)*3) ||
                                   (DIFF_INT_C3(img + i*3, img + i*3 - curstep) && i <= R)))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
                else
                    for(i = left; i <= right; i++)
                    {
                        int idx, val[3];
                
                        if(!mask[i] &&
                            ((ICC_SET_C3(val, img+i*3),
                            (unsigned)(idx = i-L-1) <= length) &&
                            DIFF_INT_C3(val, img - curstep + (i-1)*3) ||
                            (unsigned)(++idx) <= length &&
                            DIFF_INT_C3(val, img - curstep + i*3) ||
                            (unsigned)(++idx) <= length &&
                            DIFF_INT_C3(val, img - curstep + (i+1)*3)))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_INT_C3(img + j*3, img + (j+1)*3))
                                mask[j] = newMaskVal;

                            while(!mask[++i] &&
                                   ((ICC_SET_C3(val, img + i*3),
                                   DIFF_INT_C3(val, img + (i-1)*3)) ||
                                   ((unsigned)(idx = i-L-1) <= length &&
                                   DIFF_INT_C3(val, img - curstep + (i-1)*3)) ||
                                   (unsigned)(++idx) <= length &&
                                   DIFF_INT_C3(val, img - curstep + i*3) ||
                                   (unsigned)(++idx) <= length &&
                                   DIFF_INT_C3(val, img - curstep + (i+1)*3)))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
            }

            img = pImage + YC * step;
            if(fillImage)
                for(i = L; i <= R; i++)
                    ICC_SET_C3(img + i*3, newVal);
            else if(region)
                for(i = L; i <= R; i++)
                {
                    sum[0] += img[i*3];
                    sum[1] += img[i*3+1];
                    sum[2] += img[i*3+2];
                }
        }
    }
    
    if(region)
    {
        region->area = area;
        region->rect.x = XMin;
        region->rect.y = YMin;
        region->rect.width = XMax - XMin + 1;
        region->rect.height = YMax - YMin + 1;
    
        if(fillImage)
            region->value = cScalar(newVal[0], newVal[1], newVal[2]);
        else
        {
            double iarea = area ? 1./area : 0;
            region->value = cScalar(sum[0]*iarea, sum[1]*iarea, sum[2]*iarea);
        }
    }

    return CC_NO_ERR;
}


static CStatus
icvFloodFill_Grad_32f_CnIR(float* pImage, int step, uchar* pMask, int maskStep,
                           CSize /*roi*/, CPoint seed, float* _newVal, float* _d_lw,
                           float* _d_up, CConnectedComp* region, int flags,
                           CvFFillSegment* buffer, int buffer_size, int cn)
{
    float* img = pImage + (step /= sizeof(float))*seed.y;
    uchar* mask = (pMask += maskStep + 1) + maskStep*seed.y;
    int i, L, R;
    int area = 0;
    double sum[] = {0,0,0}, val0[] = {0,0,0};
    float newVal[] = {0,0,0};
    float d_lw[] = {0,0,0};
    float interval[] = {0,0,0};
    int XMin, XMax, YMin = seed.y, YMax = seed.y;
    int _8_connectivity = (flags & 255) == 8;
    int fixedRange = flags & CC_FLOODFILL_FIXED_RANGE;
    int fillImage = (flags & CC_FLOODFILL_MASK_ONLY) == 0;
    uchar newMaskVal = (uchar)(flags & 0xff00 ? flags >> 8 : 1);
    CvFFillSegment* buffer_end = buffer + buffer_size, *head = buffer, *tail = buffer;

    L = R = seed.x;
    if(mask[L])
        return CC_OK;

    mask[L] = newMaskVal;

    for(i = 0; i < cn; i++)
    {
        newVal[i] = _newVal[i];
        d_lw[i] = 0.5f*(_d_lw[i] - _d_up[i]);
        interval[i] = 0.5f*(_d_lw[i] + _d_up[i]);
        if(fixedRange)
            val0[i] = img[L*cn+i];
    }

    if(cn == 1)
    {
        if(fixedRange)
        {
            while(!mask[R + 1] && DIFF_FLT_C1(img + (R+1), val0))
                mask[++R] = newMaskVal;

            while(!mask[L - 1] && DIFF_FLT_C1(img + (L-1), val0))
                mask[--L] = newMaskVal;
        }
        else
        {
            while(!mask[R + 1] && DIFF_FLT_C1(img + (R+1), img + R))
                mask[++R] = newMaskVal;

            while(!mask[L - 1] && DIFF_FLT_C1(img + (L-1), img + L))
                mask[--L] = newMaskVal;
        }
    }
    else
    {
        if(fixedRange)
        {
            while(!mask[R + 1] && DIFF_FLT_C3(img + (R+1)*3, val0))
                mask[++R] = newMaskVal;

            while(!mask[L - 1] && DIFF_FLT_C3(img + (L-1)*3, val0))
                mask[--L] = newMaskVal;
        }
        else
        {
            while(!mask[R + 1] && DIFF_FLT_C3(img + (R+1)*3, img + R*3))
                mask[++R] = newMaskVal;

            while(!mask[L - 1] && DIFF_FLT_C3(img + (L-1)*3, img + L*3))
                mask[--L] = newMaskVal;
        }
    }

    XMax = R;
    XMin = L;
    ICC_PUSH(seed.y, L, R, R + 1, R, UP);

    while(head != tail)
    {
        int k, YC, PL, PR, dir, curstep;
        ICC_POP(YC, L, R, PL, PR, dir);

        int data[][3] =
        {
            {-dir, L - _8_connectivity, R + _8_connectivity},
            {dir, L - _8_connectivity, PL - 1},
            {dir, PR + 1, R + _8_connectivity}
        };

        unsigned length = (unsigned)(R-L);

        if(region)
        {
            area += (int)length + 1;

            if(XMax < R) XMax = R;
            if(XMin > L) XMin = L;
            if(YMax < YC) YMax = YC;
            if(YMin > YC) YMin = YC;
        }

        if(cn == 1)
        {
            for(k = 0; k < 3; k++)
            {
                dir = data[k][0];
                curstep = dir * step;
                img = pImage + (YC + dir) * step;
                mask = pMask + (YC + dir) * maskStep;
                int left = data[k][1];
                int right = data[k][2];

                if(fixedRange)
                    for(i = left; i <= right; i++)
                    {
                        if(!mask[i] && DIFF_FLT_C1(img + i, val0))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_FLT_C1(img + j, val0))
                                mask[j] = newMaskVal;

                            while(!mask[++i] && DIFF_FLT_C1(img + i, val0))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
                else if(!_8_connectivity)
                    for(i = left; i <= right; i++)
                    {
                        if(!mask[i] && DIFF_FLT_C1(img + i, img - curstep + i))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_FLT_C1(img + j, img + (j+1)))
                                mask[j] = newMaskVal;

                            while(!mask[++i] &&
                                   (DIFF_FLT_C1(img + i, img + (i-1)) ||
                                   (DIFF_FLT_C1(img + i, img + i - curstep) && i <= R)))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
                else
                    for(i = left; i <= right; i++)
                    {
                        int idx;
                        float val[1];
                
                        if(!mask[i] &&
                            ((val[0] = img[i],
                            (unsigned)(idx = i-L-1) <= length) &&
                            DIFF_FLT_C1(val, img - curstep + (i-1)) ||
                            (unsigned)(++idx) <= length &&
                            DIFF_FLT_C1(val, img - curstep + i) ||
                            (unsigned)(++idx) <= length &&
                            DIFF_FLT_C1(val, img - curstep + (i+1))))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_FLT_C1(img + j, img + (j+1)))
                                mask[j] = newMaskVal;

                            while(!mask[++i] &&
                                   ((val[0] = img[i],
                                   DIFF_FLT_C1(val, img + (i-1))) ||
                                   ((unsigned)(idx = i-L-1) <= length &&
                                   DIFF_FLT_C1(val, img - curstep + (i-1))) ||
                                   (unsigned)(++idx) <= length &&
                                   DIFF_FLT_C1(val, img - curstep + i) ||
                                   (unsigned)(++idx) <= length &&
                                   DIFF_FLT_C1(val, img - curstep + (i+1))))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
            }

            img = pImage + YC * step;
            if(fillImage)
                for(i = L; i <= R; i++)
                    img[i] = newVal[0];
            else if(region)
                for(i = L; i <= R; i++)
                    sum[0] += img[i];
        }
        else
        {
            for(k = 0; k < 3; k++)
            {
                dir = data[k][0];
                curstep = dir * step;
                img = pImage + (YC + dir) * step;
                mask = pMask + (YC + dir) * maskStep;
                int left = data[k][1];
                int right = data[k][2];

                if(fixedRange)
                    for(i = left; i <= right; i++)
                    {
                        if(!mask[i] && DIFF_FLT_C3(img + i*3, val0))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_FLT_C3(img + j*3, val0))
                                mask[j] = newMaskVal;

                            while(!mask[++i] && DIFF_FLT_C3(img + i*3, val0))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
                else if(!_8_connectivity)
                    for(i = left; i <= right; i++)
                    {
                        if(!mask[i] && DIFF_FLT_C3(img + i*3, img - curstep + i*3))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_FLT_C3(img + j*3, img + (j+1)*3))
                                mask[j] = newMaskVal;

                            while(!mask[++i] &&
                                   (DIFF_FLT_C3(img + i*3, img + (i-1)*3) ||
                                   (DIFF_FLT_C3(img + i*3, img + i*3 - curstep) && i <= R)))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
                else
                    for(i = left; i <= right; i++)
                    {
                        int idx;
                        float val[3];
                
                        if(!mask[i] &&
                            ((ICC_SET_C3(val, img+i*3),
                            (unsigned)(idx = i-L-1) <= length) &&
                            DIFF_FLT_C3(val, img - curstep + (i-1)*3) ||
                            (unsigned)(++idx) <= length &&
                            DIFF_FLT_C3(val, img - curstep + i*3) ||
                            (unsigned)(++idx) <= length &&
                            DIFF_FLT_C3(val, img - curstep + (i+1)*3)))
                        {
                            int j = i;
                            mask[i] = newMaskVal;
                            while(!mask[--j] && DIFF_FLT_C3(img + j*3, img + (j+1)*3))
                                mask[j] = newMaskVal;

                            while(!mask[++i] &&
                                   ((ICC_SET_C3(val, img + i*3),
                                   DIFF_FLT_C3(val, img + (i-1)*3)) ||
                                   ((unsigned)(idx = i-L-1) <= length &&
                                   DIFF_FLT_C3(val, img - curstep + (i-1)*3)) ||
                                   (unsigned)(++idx) <= length &&
                                   DIFF_FLT_C3(val, img - curstep + i*3) ||
                                   (unsigned)(++idx) <= length &&
                                   DIFF_FLT_C3(val, img - curstep + (i+1)*3)))
                                mask[i] = newMaskVal;

                            ICC_PUSH(YC + dir, j+1, i-1, L, R, -dir);
                        }
                    }
            }

            img = pImage + YC * step;
            if(fillImage)
                for(i = L; i <= R; i++)
                    ICC_SET_C3(img + i*3, newVal);
            else if(region)
                for(i = L; i <= R; i++)
                {
                    sum[0] += img[i*3];
                    sum[1] += img[i*3+1];
                    sum[2] += img[i*3+2];
                }
        }
    }
    
    if(region)
    {
        region->area = area;
        region->rect.x = XMin;
        region->rect.y = YMin;
        region->rect.width = XMax - XMin + 1;
        region->rect.height = YMax - YMin + 1;
    
        if(fillImage)
            region->value = cScalar(newVal[0], newVal[1], newVal[2]);
        else
        {
            double iarea = area ? 1./area : 0;
            region->value = cScalar(sum[0]*iarea, sum[1]*iarea, sum[2]*iarea);
        }
    }

    return CC_NO_ERR;
}


/****************************************************************************************\
*                                    External Functions                                  *
\****************************************************************************************/

typedef  CStatus (C_CDECL* CvFloodFillFunc)(
               void* img, int step, CSize size, CPoint seed, void* newval,
               CConnectedComp* comp, int flags, void* buffer, int buffer_size, int cn);

typedef  CStatus (C_CDECL* CvFloodFillGradFunc)(
               void* img, int step, uchar* mask, int maskStep, CSize size,
               CPoint seed, void* newval, void* d_lw, void* d_up, void* ccomp,
               int flags, void* buffer, int buffer_size, int cn);

static  void  icvInitFloodFill(void** ffill_tab,
                                void** ffillgrad_tab)
{
    ffill_tab[0] = (void*)icvFloodFill_8u_CnIR;
    ffill_tab[1] = (void*)icvFloodFill_32f_CnIR;

    ffillgrad_tab[0] = (void*)icvFloodFill_Grad_8u_CnIR;
    ffillgrad_tab[1] = (void*)icvFloodFill_Grad_32f_CnIR;
}


CC_IMPL void
cvFloodFill(img_t* arr, CPoint seed_point,
             CScalar newVal, CScalar lo_diff, CScalar up_diff,
             CConnectedComp* comp, int flags, img_t* mask)
{
    static void* ffill_tab[4];
    static void* ffillgrad_tab[4];
    static int inittab = 0;

    img_t tempMask[1] = {0};
    CvFFillSegment* buffer = 0;
    CC_FUNCNAME("cvFloodFill");

    if(comp)
        memset(comp, 0, sizeof(*comp));

    __BEGIN__;

    TypeId type;
    int i, cn, is_simple, idx;
    int buffer_size, connectivity = flags & 255;
    double nv_buf[4] = {0,0,0,0};
    union { uchar b[4]; float f[4]; } ld_buf, ud_buf;
    img_t stub, *img = arr;
    
    CSize size;

    if(!inittab)
    {
        icvInitFloodFill(ffill_tab, ffillgrad_tab);
        inittab = 1;
    }

    CC_CALL(img = cvGetMat(img, &stub));
    type = CC_MAT_DEPTH(img);
    cn = CC_MAT_CN(img);

    idx = (type == CC_8U && cn==1) || (type == CC_8U && cn==3) ? 0 :
          (type == CC_32F && cn==1) || (type == CC_32F && cn==3) ? 1 : -1;

    if(idx < 0)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    if(connectivity == 0)
        connectivity = 4;
    else if(connectivity != 4 && connectivity != 8)
        CC_ERROR(CC_StsBadFlag, "Connectivity must be 4, 0(=4) or 8");

    is_simple = mask == 0 && (flags & CC_FLOODFILL_MASK_ONLY) == 0;

    for(i = 0; i < cn; i++)
    {
        if(lo_diff.val[i] < 0 || up_diff.val[i] < 0)
            CC_ERROR(CC_StsBadArg, "lo_diff and up_diff must be non-negative");
        is_simple &= fabs(lo_diff.val[i]) < DBL_EPSILON && fabs(up_diff.val[i]) < DBL_EPSILON;
    }

    size = cvGetMatSize(img);

    if((unsigned)seed_point.x >= (unsigned)size.width ||
        (unsigned)seed_point.y >= (unsigned)size.height)
        CC_ERROR(CC_StsOutOfRange, "Seed point is outside of image");

    cvScalarToRawData(&newVal, &nv_buf, type, 0);
    buffer_size = MAX(size.width, size.height)*2;
    CC_CALL(buffer = (CvFFillSegment*)cAlloc(buffer_size*sizeof(buffer[0])));

    if(is_simple)
    {
        CvFloodFillFunc func = (CvFloodFillFunc)ffill_tab[idx];
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");
        
        IPPI_CALL(func(img->tt.data, img->step, size,
                         seed_point, &nv_buf, comp, flags,
                         buffer, buffer_size, cn));
    }
    else
    {
        CvFloodFillGradFunc func = (CvFloodFillGradFunc)ffillgrad_tab[idx];
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");
        
        if(!mask)
        {
            /* created mask will be 8-byte aligned */
            mask = cvCreateMat(tempMask, size.height + 2, (size.width + 9) & -8, CC_8U, 1);
        }
        else
        {
            if(!CC_IS_MASK_ARR(mask))
                CC_ERROR(CC_StsBadMask, "");

            if(mask->width != size.width + 2 || mask->height != size.height + 2)
                CC_ERROR(CC_StsUnmatchedSizes, "mask must be 2 pixel wider "
                                       "and 2 pixel taller than filled image");
        }

        {
            int width = tempMask ? mask->step : size.width + 2;
            uchar* mask_row = mask->tt.data + mask->step;
            memset(mask_row - mask->step, 1, width);

            for(i = 1; i <= size.height; i++, mask_row += mask->step)
            {
                if(tempMask)
                    memset(mask_row, 0, width);
                mask_row[0] = mask_row[size.width+1] = (uchar)1;
            }
            memset(mask_row, 1, width);
        }

        if(type == CC_8U)
            for(i = 0; i < cn; i++)
            {
                int t = cFloor(lo_diff.val[i]);
                ld_buf.b[i] = CC_CAST_8U(t);
                t = cFloor(up_diff.val[i]);
                ud_buf.b[i] = CC_CAST_8U(t);
            }
        else
            for(i = 0; i < cn; i++)
            {
                ld_buf.f[i] = (float)lo_diff.val[i];
                ud_buf.f[i] = (float)up_diff.val[i];
            }

        IPPI_CALL(func(img->tt.data, img->step, mask->tt.data, mask->step,
                         size, seed_point, &nv_buf, ld_buf.f, ud_buf.f,
                         comp, flags, buffer, buffer_size, cn));
    }

    __END__;

    cFree(&buffer);
    imfree(tempMask);
}


