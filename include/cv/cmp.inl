
/* ////////////////////////////////////////////////////////////////////
//
//  img_t logical operations: &, |, ^ ...
//
// */


/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                             Macros for logic operations                             //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

/* //////////////////////////////////////////////////////////////////////////////////////
                                img_t op img_t
////////////////////////////////////////////////////////////////////////////////////// */


#define ICC_DEF_BIN_LOG_OP_2D(__op__, name)                                         \
IPCVAPI_IMPL(CStatus, icv##name##_8u_C1R,                                           \
(const uchar* src1, int step1, const uchar* src2, int step2,                        \
  uchar* dst, int step, CSize size), (src1, step1, src2, step2, dst, step, size))   \
{                                                                                   \
    for(; size.height--; src1 += step1, src2 += step2, dst += step)                 \
    {                                                                               \
        int i = 0;                                                                  \
                                                                                    \
        if((((size_t)src1 | (size_t)src2 | (size_t)dst) & 3) == 0)                  \
        {                                                                           \
            for(; i <= size.width - 16; i += 16)                                    \
            {                                                                       \
                int t0 = __op__(((const int*)(src1+i))[0], ((const int*)(src2+i))[0]);\
                int t1 = __op__(((const int*)(src1+i))[1], ((const int*)(src2+i))[1]);\
                                                                                    \
                ((int*)(dst+i))[0] = t0;                                            \
                ((int*)(dst+i))[1] = t1;                                            \
                                                                                    \
                t0 = __op__(((const int*)(src1+i))[2], ((const int*)(src2+i))[2]);  \
                t1 = __op__(((const int*)(src1+i))[3], ((const int*)(src2+i))[3]);  \
                                                                                    \
                ((int*)(dst+i))[2] = t0;                                            \
                ((int*)(dst+i))[3] = t1;                                            \
            }                                                                       \
                                                                                    \
            for(; i <= size.width - 4; i += 4)                                      \
            {                                                                       \
                int t = __op__(*(const int*)(src1+i), *(const int*)(src2+i));       \
                *(int*)(dst+i) = t;                                                 \
            }                                                                       \
        }                                                                           \
                                                                                    \
        for(; i < size.width; i++)                                                  \
        {                                                                           \
            int t = __op__(((const uchar*)src1)[i],((const uchar*)src2)[i]);        \
            dst[i] = (uchar)t;                                                      \
        }                                                                           \
    }                                                                               \
                                                                                    \
    return  CC_OK;                                                                  \
}


/* //////////////////////////////////////////////////////////////////////////////////////
                                     img_t op Scalar
////////////////////////////////////////////////////////////////////////////////////// */


#define ICC_DEF_UN_LOG_OP_2D(__op__, name)                                              \
static CStatus C_STDCALL icv##name##_8u_CnR                                             \
(const uchar* src0, int step1, uchar* dst0, int step, CSize size,                       \
  const uchar* scalar, int pix_size)                                                    \
{                                                                                       \
    int delta = 12*pix_size;                                                            \
                                                                                        \
    for(; size.height--; src0 += step1, dst0 += step)                                   \
    {                                                                                   \
        const uchar* src = (const uchar*)src0;                                          \
        uchar* dst = dst0;                                                              \
        int i, len = size.width;                                                        \
                                                                                        \
        if((((size_t)src|(size_t)dst) & 3) == 0)                                        \
        {                                                                               \
            while((len -= delta) >= 0)                                                  \
            {                                                                           \
                for(i = 0; i < (delta); i += 12)                                        \
                {                                                                       \
                    int t0 = __op__(((const int*)(src+i))[0], ((const int*)(scalar+i))[0]); \
                    int t1 = __op__(((const int*)(src+i))[1], ((const int*)(scalar+i))[1]); \
                    ((int*)(dst+i))[0] = t0;                                            \
                    ((int*)(dst+i))[1] = t1;                                            \
                                                                                        \
                    t0 = __op__(((const int*)(src+i))[2], ((const int*)(scalar+i))[2]); \
                    ((int*)(dst+i))[2] = t0;                                            \
                }                                                                       \
                src += delta;                                                           \
                dst += delta;                                                           \
            }                                                                           \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            while((len -= delta) >= 0)                                                  \
            {                                                                           \
                for(i = 0; i < (delta); i += 4)                                         \
                {                                                                       \
                    int t0 = __op__(src[i], scalar[i]);                                 \
                    int t1 = __op__(src[i+1], scalar[i+1]);                             \
                    dst[i] = (uchar)t0;                                                 \
                    dst[i+1] = (uchar)t1;                                               \
                                                                                        \
                    t0 = __op__(src[i+2], scalar[i+2]);                                 \
                    t1 = __op__(src[i+3], scalar[i+3]);                                 \
                    dst[i+2] = (uchar)t0;                                               \
                    dst[i+3] = (uchar)t1;                                               \
                }                                                                       \
                src += delta;                                                           \
                dst += delta;                                                           \
            }                                                                           \
        }                                                                               \
                                                                                        \
        for(len += delta, i = 0; i < len; i++)                                          \
        {                                                                               \
            int t = __op__(src[i],scalar[i]);                                           \
            dst[i] = (uchar)t;                                                          \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    return CC_OK;                                                                       \
}

/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                                LOGIC OPERATIONS                                     //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

static void
icvLogicS(const img_t* src, CScalar* scalar, img_t* dst,
           const img_t* mask, CvFunc2D_2A1P1I fn_2d)
{
    uchar* buffer = 0;
    int local_alloc = 1;
    
    CC_FUNCNAME("icvLogicS");
    
    __BEGIN__;

    img_t dstbuf[1], *tdst;
    CvCopyMaskFunc copym_func = 0;
    
    int y, dy;
    int coi1 = 0, coi2 = 0;
    int cont_flag = 0;
    int elem_size, elem_size1;
    TypeId type;
    double buf[12];
    CSize size, tsize;
    int src_step, dst_step, tdst_step, mask_step;

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    size = cvGetMatSize(src);
    type = CC_MAT_DEPTH(src);
    
    elem_size = CC_TYPE_SIZE(type);
    elem_size1 = CC_ELEMTYPE_SIZE(src);

    if(!mask)
    {
        cont_flag = CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst);
        dy = size.height;
        tdst = dst;
    }
    else
    {
        int buf_size;
        
        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mask, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        cont_flag = CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst) && CC_IS_MAT_CONT(mask);
        dy = CC_MAX_LOCAL_SIZE/(elem_size*size.height);
        dy = MAX(dy,1);
        dy = MIN(dy,size.height);
        cvMat(dstbuf, dy, size.width, type);
        if(!cont_flag)
            dstbuf->step = cvAlign(dstbuf->step, 8);
        buf_size = dstbuf->step ? dstbuf->step*dy : size.width*elem_size;
        if(buf_size > CC_MAX_LOCAL_SIZE)
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
            local_alloc = 0;
        }
        else
            buffer = (uchar*)cvStackAlloc(buf_size);
        dstbuf->tt.data = buffer;
        tdst = dstbuf;
        
        copym_func = icvGetCopyMaskFunc(elem_size);
    }

    src_step = src->step;
    dst_step = dst->step;
    tdst_step = tdst->step;
    mask_step = mask ? mask->step : 0;
    CC_CALL(cvScalarToRawData(scalar, buf, type, 1));

    for(y = 0; y < size.height; y += dy)
    {
        tsize.width = size.width;
        tsize.height = dy;
        if(y + dy > size.height)
            tsize.height = size.height - y;
        if(cont_flag || tsize.height == 1)
        {
            tsize.width *= tsize.height;
            tsize.height = 1;
            src_step = tdst_step = dst_step = mask_step = CC_STUB_STEP;
        }
        IPPI_CALL(fn_2d(src->tt.data + y*src->step, src_step, tdst->tt.data, tdst_step,
                          cSize(tsize.width*elem_size, tsize.height), buf, elem_size1));
        if(mask)
        {
            IPPI_CALL(copym_func(tdst->tt.data, tdst_step, dst->tt.data + y*dst->step,
                                   dst_step, tsize, mask->tt.data + y*mask->step, mask_step));
        }
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
}


static void
icvLogic(const img_t* src1, const img_t* src2, img_t* dst,
          const img_t* mask, CvFunc2D_3A fn_2d)
{
    uchar* buffer = 0;
    int local_alloc = 1;

    CC_FUNCNAME("icvLogic");
    
    __BEGIN__;

    int y, dy;
    int coi1 = 0, coi2 = 0, coi3 = 0;
    TypeId type;
    int elem_size;
    int cont_flag = 0;
    
    img_t dstbuf[1], *tdst;
    int src1_step, src2_step, tdst_step, dst_step, mask_step;
    CSize size, tsize;
    CvCopyMaskFunc copym_func = 0;

    if(coi1 != 0 || coi2 != 0 || coi3 != 0)
        CC_ERROR_FROM_CODE(CC_BadCOI);

    if(!CC_ARE_TYPES_EQ(src1, src2))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src1, src2))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    if(!CC_ARE_TYPES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);
    
    if(!CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    size = cvGetMatSize(src1);
    type = CC_MAT_DEPTH(src1);
    elem_size = CC_TYPE_SIZE(type);

    if(!mask)
    {
        cont_flag = CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst);
        dy = size.height;
        tdst = dst;
    }
    else
    {
        int buf_size;
        
        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mask, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        cont_flag = CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst) && CC_IS_MAT_CONT(mask);
        dy = CC_MAX_LOCAL_SIZE/(elem_size*size.height);
        dy = MAX(dy,1);
        dy = MIN(dy,size.height);
        cvMat(dstbuf, dy, size.width, type);
        if(!cont_flag)
            dstbuf->step = cvAlign(dstbuf->step, 8);
        buf_size = dstbuf->step ? dstbuf->step*dy : size.width*elem_size;
        if(buf_size > CC_MAX_LOCAL_SIZE)
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
            local_alloc = 0;
        }
        else
            buffer = (uchar*)cvStackAlloc(buf_size);
        dstbuf->tt.data = buffer;
        tdst = dstbuf;
        
        copym_func = icvGetCopyMaskFunc(elem_size);
    }

    src1_step = src1->step;
    src2_step = src2->step;
    dst_step = dst->step;
    tdst_step = tdst->step;
    mask_step = mask ? mask->step : 0;

    for(y = 0; y < size.height; y += dy)
    {
        tsize.width = size.width;
        tsize.height = dy;
        if(y + dy > size.height)
            tsize.height = size.height - y;
        if(cont_flag || tsize.height == 1)
        {
            tsize.width *= tsize.height;
            tsize.height = 1;
            src1_step = src2_step = tdst_step = dst_step = mask_step = CC_STUB_STEP;
        }
        IPPI_CALL(fn_2d(src1->tt.data + y*src1->step, src1_step,
                          src2->tt.data + y*src2->step, src2_step,
                          tdst->tt.data, tdst_step,
                          cSize(tsize.width*elem_size, tsize.height)));
        if(mask)
        {
            IPPI_CALL(copym_func(tdst->tt.data, tdst_step, dst->tt.data + y*dst->step,
                                   dst_step, tsize, mask->tt.data + y*mask->step, mask_step));
        }
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
}

ICC_DEF_BIN_LOG_OP_2D(CC_XOR, Xor)
ICC_DEF_UN_LOG_OP_2D(CC_XOR, XorC)

ICC_DEF_BIN_LOG_OP_2D(CC_AND, And)
ICC_DEF_UN_LOG_OP_2D(CC_AND, AndC)

ICC_DEF_BIN_LOG_OP_2D(CC_OR, Or)
ICC_DEF_UN_LOG_OP_2D(CC_OR, OrC)


/////////////////////////////////////////////////////////////////////////////////////////
//                                    X O R                                            //
/////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL void
cvXorS(const img_t* src, CScalar scalar, img_t* dst, const img_t* mask)
{
    icvLogicS(src, &scalar, dst, mask, (CvFunc2D_2A1P1I)icvXorC_8u_CnR);
}


CC_IMPL void
cvXor(const img_t* src1, const img_t* src2, img_t* dst, const img_t* mask)
{
    icvLogic(src1, src2, dst, mask, (CvFunc2D_3A)icvXor_8u_C1R);
}

/////////////////////////////////////////////////////////////////////////////////////////
//                                    A N D                                            //
/////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL void
cvAndS(const img_t* src, CScalar scalar, img_t* dst, const img_t* mask)
{
    icvLogicS(src, &scalar, dst, mask, (CvFunc2D_2A1P1I)icvAndC_8u_CnR);
}


CC_IMPL void
cvAnd(const img_t* src1, const img_t* src2, img_t* dst, const img_t* mask)
{
    icvLogic(src1, src2, dst, mask, (CvFunc2D_3A)icvAnd_8u_C1R);
}


/////////////////////////////////////////////////////////////////////////////////////////
//                                      O R                                            //
/////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL void
cvOrS(const img_t* src, CScalar scalar, img_t* dst, const img_t* mask)
{
    icvLogicS(src, &scalar, dst, mask, (CvFunc2D_2A1P1I)icvOrC_8u_CnR);
}


CC_IMPL void
cvOr(const img_t* src1, const img_t* src2, img_t* dst, const img_t* mask)
{
    icvLogic(src1, src2, dst, mask, (CvFunc2D_3A)icvOr_8u_C1R);
}


/////////////////////////////////////////////////////////////////////////////////////////
//                                      N O T                                          //
/////////////////////////////////////////////////////////////////////////////////////////


IPCVAPI_IMPL(CStatus, icvNot_8u_C1R,
(const uchar* src1, int step1, uchar* dst, int step, CSize size),
  (src1, step1, dst, step, size))
{
    for(; size.height--; src1 += step1, dst += step)
    {
        int i = 0;

        if((((size_t)src1 | (size_t)dst) & 3) == 0)
        {
            for(; i <= size.width - 16; i += 16)
            {
                int t0 = ~((const int*)(src1+i))[0];
                int t1 = ~((const int*)(src1+i))[1];

                ((int*)(dst+i))[0] = t0;
                ((int*)(dst+i))[1] = t1;

                t0 = ~((const int*)(src1+i))[2];
                t1 = ~((const int*)(src1+i))[3];

                ((int*)(dst+i))[2] = t0;
                ((int*)(dst+i))[3] = t1;
            }

            for(; i <= size.width - 4; i += 4)
            {
                int t = ~*(const int*)(src1+i);
                *(int*)(dst+i) = t;
            }
        }

        for(; i < size.width; i++)
        {
            int t = ~((const uchar*)src1)[i];
            dst[i] = (uchar)t;
        }
    }

    return  CC_OK;
}


CC_IMPL void
cvNot(const img_t* src, img_t* dst)
{
    CC_FUNCNAME("cvNot");
    
    __BEGIN__;

    
    
    
    int coi1 = 0, coi2 = 0;
    TypeId type;
    CSize size;
    int src_step, dst_step;

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    size = cvGetMatSize(src);
    src_step = src->step;
    dst_step = dst->step;

    if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }

    type = CC_MAT_DEPTH(src);
    size.width *= CC_TYPE_SIZE(type);

    IPPI_CALL(icvNot_8u_C1R(src->tt.data, src_step, dst->tt.data, dst_step, size));

    __END__;
}




/* ////////////////////////////////////////////////////////////////////
//
//  img_t comparison functions: range checking, min, max
//
// */

/****************************************************************************************\
*                                      InRange[S]                                        *
\****************************************************************************************/

#define ICC_DEF_IN_RANGE_CASE_C1(worktype, _toggle_macro_)    \
for(x = 0; x < size.width; x++)                               \
{                                                               \
    worktype a1 = _toggle_macro_(src1[x]),                      \
             a2 = src2[x], a3 = src3[x];                        \
    dst[x] = (uchar)-(_toggle_macro_(a2) <= a1 &&               \
                     a1 < _toggle_macro_(a3));                  \
}


#define ICC_DEF_IN_RANGE_CASE_C2(worktype, _toggle_macro_)        \
for(x = 0; x < size.width; x++)                                   \
{                                                                   \
    worktype a1 = _toggle_macro_(src1[x*2]),                        \
             a2 = src2[x*2], a3 = src3[x*2];                        \
    int f = _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);    \
    a1 = _toggle_macro_(src1[x*2+1]);                               \
    a2 = src2[x*2+1];                                               \
    a3 = src3[x*2+1];                                               \
    f &= _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);       \
    dst[x] = (uchar)-f;                                             \
}


#define ICC_DEF_IN_RANGE_CASE_C3(worktype, _toggle_macro_)        \
for(x = 0; x < size.width; x++)                                   \
{                                                                   \
    worktype a1 = _toggle_macro_(src1[x*3]),                        \
             a2 = src2[x*3], a3 = src3[x*3];                        \
    int f = _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);    \
    a1 = _toggle_macro_(src1[x*3+1]);                               \
    a2 = src2[x*3+1];                                               \
    a3 = src3[x*3+1];                                               \
    f &= _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);       \
    a1 = _toggle_macro_(src1[x*3+2]);                               \
    a2 = src2[x*3+2];                                               \
    a3 = src3[x*3+2];                                               \
    f &= _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);       \
    dst[x] = (uchar)-f;                                             \
}


#define ICC_DEF_IN_RANGE_CASE_C4(worktype, _toggle_macro_)        \
for(x = 0; x < size.width; x++)                                   \
{                                                                   \
    worktype a1 = _toggle_macro_(src1[x*4]),                        \
             a2 = src2[x*4], a3 = src3[x*4];                        \
    int f = _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);    \
    a1 = _toggle_macro_(src1[x*4+1]);                               \
    a2 = src2[x*4+1];                                               \
    a3 = src3[x*4+1];                                               \
    f &= _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);       \
    a1 = _toggle_macro_(src1[x*4+2]);                               \
    a2 = src2[x*4+2];                                               \
    a3 = src3[x*4+2];                                               \
    f &= _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);       \
    a1 = _toggle_macro_(src1[x*4+3]);                               \
    a2 = src2[x*4+3];                                               \
    a3 = src3[x*4+3];                                               \
    f &= _toggle_macro_(a2) <= a1 && a1 < _toggle_macro_(a3);       \
    dst[x] = (uchar)-f;                                             \
}


#define ICC_DEF_IN_RANGE_FUNC(flavor, arrtype, worktype,           \
                               _toggle_macro_, cn)                 \
static CStatus C_STDCALL                                          \
icvInRange_##flavor##_C##cn##R(const arrtype* src1, int step1,     \
                                const arrtype* src2, int step2,     \
                                const arrtype* src3, int step3,     \
                                uchar* dst, int step, CSize size) \
{                                                                   \
    step1 /= sizeof(src1[0]); step2 /= sizeof(src2[0]);             \
    step3 /= sizeof(src3[0]); step /= sizeof(dst[0]);               \
                                                                    \
    for(; size.height--; src1 += step1, src2 += step2,             \
                          src3 += step3, dst += step)              \
    {                                                               \
        int x;                                                      \
        ICC_DEF_IN_RANGE_CASE_C##cn(worktype, _toggle_macro_)     \
    }                                                               \
                                                                    \
    return CC_OK;                                                   \
}


#define ICC_DEF_IN_RANGE_CASE_CONST_C1(worktype, _toggle_macro_)  \
for(x = 0; x < size.width; x++)                                   \
{                                                                   \
    worktype a1 = _toggle_macro_(src1[x]);                          \
    dst[x] = (uchar)-(scalar[0] <= a1 && a1 < scalar[1]);           \
}


#define ICC_DEF_IN_RANGE_CASE_CONST_C2(worktype, _toggle_macro_)  \
for(x = 0; x < size.width; x++)                                   \
{                                                                   \
    worktype a1 = _toggle_macro_(src1[x*2]);                        \
    int f = scalar[0] <= a1 && a1 < scalar[2];                      \
    a1 = _toggle_macro_(src1[x*2+1]);                               \
    f &= scalar[1] <= a1 && a1 < scalar[3];                         \
    dst[x] = (uchar)-f;                                             \
}


#define ICC_DEF_IN_RANGE_CASE_CONST_C3(worktype, _toggle_macro_)  \
for(x = 0; x < size.width; x++)                                   \
{                                                                   \
    worktype a1 = _toggle_macro_(src1[x*3]);                        \
    int f = scalar[0] <= a1 && a1 < scalar[3];                      \
    a1 = _toggle_macro_(src1[x*3+1]);                               \
    f &= scalar[1] <= a1 && a1 < scalar[4];                         \
    a1 = _toggle_macro_(src1[x*3+2]);                               \
    f &= scalar[2] <= a1 && a1 < scalar[5];                         \
    dst[x] = (uchar)-f;                                             \
}


#define ICC_DEF_IN_RANGE_CASE_CONST_C4(worktype, _toggle_macro_)  \
for(x = 0; x < size.width; x++)                                   \
{                                                                   \
    worktype a1 = _toggle_macro_(src1[x*4]);                        \
    int f = scalar[0] <= a1 && a1 < scalar[4];                      \
    a1 = _toggle_macro_(src1[x*4+1]);                               \
    f &= scalar[1] <= a1 && a1 < scalar[5];                         \
    a1 = _toggle_macro_(src1[x*4+2]);                               \
    f &= scalar[2] <= a1 && a1 < scalar[6];                         \
    a1 = _toggle_macro_(src1[x*4+3]);                               \
    f &= scalar[3] <= a1 && a1 < scalar[7];                         \
    dst[x] = (uchar)-f;                                             \
}


#define ICC_DEF_IN_RANGE_CONST_FUNC(flavor, arrtype, worktype,     \
                                     _toggle_macro_, cn)           \
static CStatus C_STDCALL                                          \
icvInRangeC_##flavor##_C##cn##R(const arrtype* src1, int step1,    \
                                 uchar* dst, int step, CSize size, \
                                 const worktype* scalar)           \
{                                                                   \
    step1 /= sizeof(src1[0]); step /= sizeof(dst[0]);               \
                                                                    \
    for(; size.height--; src1 += step1, dst += step)              \
    {                                                               \
        int x;                                                      \
        ICC_DEF_IN_RANGE_CASE_CONST_C##cn(worktype, _toggle_macro_)\
    }                                                               \
                                                                    \
    return CC_OK;                                                   \
}


#define ICC_DEF_IN_RANGE_ALL(flavor, arrtype, worktype, _toggle_macro_)   \
ICC_DEF_IN_RANGE_FUNC(flavor, arrtype, worktype, _toggle_macro_, 1)       \
ICC_DEF_IN_RANGE_FUNC(flavor, arrtype, worktype, _toggle_macro_, 2)       \
ICC_DEF_IN_RANGE_FUNC(flavor, arrtype, worktype, _toggle_macro_, 3)       \
ICC_DEF_IN_RANGE_FUNC(flavor, arrtype, worktype, _toggle_macro_, 4)       \
                                                                            \
ICC_DEF_IN_RANGE_CONST_FUNC(flavor, arrtype, worktype, _toggle_macro_, 1) \
ICC_DEF_IN_RANGE_CONST_FUNC(flavor, arrtype, worktype, _toggle_macro_, 2) \
ICC_DEF_IN_RANGE_CONST_FUNC(flavor, arrtype, worktype, _toggle_macro_, 3) \
ICC_DEF_IN_RANGE_CONST_FUNC(flavor, arrtype, worktype, _toggle_macro_, 4)

ICC_DEF_IN_RANGE_ALL(8u, uchar, int, CC_NOP)
ICC_DEF_IN_RANGE_ALL(16u, ushort, int, CC_NOP)
ICC_DEF_IN_RANGE_ALL(16s, short, int, CC_NOP)
ICC_DEF_IN_RANGE_ALL(32s, int, int, CC_NOP)
ICC_DEF_IN_RANGE_ALL(32f, float, float, CC_NOP)
ICC_DEF_IN_RANGE_ALL(64f, double, double, CC_NOP)

#define icvInRange_8s_C1R 0
#define icvInRange_8s_C2R 0
#define icvInRange_8s_C3R 0
#define icvInRange_8s_C4R 0

#define icvInRangeC_8s_C1R 0
#define icvInRangeC_8s_C2R 0
#define icvInRangeC_8s_C3R 0
#define icvInRangeC_8s_C4R 0

CC_DEF_INIT_BIG_FUNC_TAB_2D(InRange, R)
CC_DEF_INIT_BIG_FUNC_TAB_2D(InRangeC, R)

typedef CStatus (C_STDCALL * CvInRangeCFunc)(const void* src, int srcstep,
                                                uchar* dst, int dststep,
                                                CSize size, const void* scalar);

/*************************************** InRange ****************************************/

CC_IMPL void
cvInRange(const img_t* src1, const img_t* src2,
           const img_t* src3, img_t* dst)
{
    static CvBigFuncTable inrange_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvInRange");

    __BEGIN__;

    TypeId type;
    int cn, coi = 0;
    int src1_step, src2_step, src3_step, dst_step;
   
    CSize size;
    CvFunc2D_4A func;

    if(!inittab)
    {
        icvInitInRangeRTable(inrange_tab);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src1, src2) ||
        !CC_ARE_TYPES_EQ(src1, src3))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_IS_MASK_ARR(dst))
        CC_ERROR(CC_StsUnsupportedFormat, "Destination image should be 8uC1 or 8sC1");

    if(!CC_ARE_SIZES_EQ(src1, src2) ||
        !CC_ARE_SIZES_EQ(src1, src3) ||
        !CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    type = CC_MAT_DEPTH(src1);
    size = cvGetMatSize(src1);
    cn = CC_MAT_CN(src1);

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(src3) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src1_step = src2_step = src3_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }
    else
    {
        src1_step = src1->step;
        src2_step = src2->step;
        src3_step = src3->step;
        dst_step = dst->step;
    }

    if(CC_MAT_CN(src1) > 4)
        CC_ERROR(CC_StsOutOfRange, "The number of channels must be 1, 2, 3 or 4");

    func = (CvFunc2D_4A)(inrange_tab[type][cn]);

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    IPPI_CALL(func(src1->tt.data, src1_step, src2->tt.data, src2_step,
                     src3->tt.data, src3_step, dst->tt.data, dst_step, size));

    __END__;
}


/************************************** InRangeS ****************************************/

CC_IMPL void
cvInRangeS(const img_t* src1, CScalar lower, CScalar upper, img_t* dst)
{
    static CvBigFuncTable inrange_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvInRangeS");

    __BEGIN__;

    TypeId sctype, type;
    int cn, coi = 0;
    int src1_step, dst_step;
    
    CSize size;
    CvInRangeCFunc func;
    double buf[8];

    if(!inittab)
    {
        icvInitInRangeCRTable(inrange_tab);
        inittab = 1;
    }

    if(!CC_IS_MASK_ARR(dst))
        CC_ERROR(CC_StsUnsupportedFormat, "Destination image should be 8uC1 or 8sC1");

    if(!CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    cn = CC_MAT_CN(src1);
    sctype = type = CC_MAT_DEPTH(src1);
    if(sctype < CC_32S)
        sctype = CC_32S;

    size = cvGetMatSize(src1);

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src1_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }
    else
    {
        src1_step = src1->step;
        dst_step = dst->step;
    }

    if(CC_MAT_CN(src1) > 4)
        CC_ERROR(CC_StsOutOfRange, "The number of channels must be 1, 2, 3 or 4");

    func = (CvInRangeCFunc)(inrange_tab[type][cn]);

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    cvScalarToRawData(&lower, buf, sctype, 0);
    cvScalarToRawData(&upper, (char*)buf + CC_TYPE_SIZE(sctype), sctype, 0);

    IPPI_CALL(func(src1->tt.data, src1_step, dst->tt.data,
                     dst_step, size, buf));

    __END__;
}

CC_IMPL void
cvInRangeS_C1(const img_t* src, double lower, double upper, img_t* dst) {
    CScalar lo = cScalar(lower, 0, 0, 0);
    CScalar up = cScalar(upper, 0, 0, 0);
    cvInRangeS(src, lo, up, dst);
}

/****************************************************************************************\
*                                         Cmp                                            *
\****************************************************************************************/

#define ICC_DEF_CMP_CASE_C1(__op__, _toggle_macro_)                   \
for(x = 0; x <= size.width - 4; x += 4)                               \
{                                                                       \
    int f0 = __op__(_toggle_macro_(src1[x]), _toggle_macro_(src2[x])); \
    int f1 = __op__(_toggle_macro_(src1[x+1]), _toggle_macro_(src2[x+1])); \
    dst[x] = (uchar)-f0;                                                \
    dst[x+1] = (uchar)-f1;                                              \
    f0 = __op__(_toggle_macro_(src1[x+2]), _toggle_macro_(src2[x+2])); \
    f1 = __op__(_toggle_macro_(src1[x+3]), _toggle_macro_(src2[x+3])); \
    dst[x+2] = (uchar)-f0;                                              \
    dst[x+3] = (uchar)-f1;                                              \
}                                                                       \
                                                                        \
for(; x < size.width; x++)                                            \
{                                                                       \
    int f0 = __op__(_toggle_macro_(src1[x]), _toggle_macro_(src2[x])); \
    dst[x] = (uchar)-f0;                                                \
}


#define ICC_DEF_CMP_FUNC(__op__, name, flavor, arrtype,        \
                          worktype, _toggle_macro_)            \
static CStatus C_STDCALL                                      \
icv##name##_##flavor##_C1R(const arrtype* src1, int step1,     \
                            const arrtype* src2, int step2,     \
                            uchar* dst, int step, CSize size) \
{                                                               \
    step1 /= sizeof(src1[0]); step2 /= sizeof(src2[0]);         \
    step /= sizeof(dst[0]);                                     \
                                                                \
    for(; size.height--; src1 += step1, src2 += step2,         \
                          dst += step)                         \
    {                                                           \
        int x;                                                  \
        ICC_DEF_CMP_CASE_C1(__op__, _toggle_macro_)           \
    }                                                           \
                                                                \
    return CC_OK;                                               \
}


#define ICC_DEF_CMP_CONST_CASE_C1(__op__, _toggle_macro_)     \
for(x = 0; x <= size.width - 4; x += 4)                       \
{                                                               \
    int f0 = __op__(_toggle_macro_(src1[x]), scalar);         \
    int f1 = __op__(_toggle_macro_(src1[x+1]), scalar);       \
    dst[x] = (uchar)-f0;                                        \
    dst[x+1] = (uchar)-f1;                                      \
    f0 = __op__(_toggle_macro_(src1[x+2]), scalar);           \
    f1 = __op__(_toggle_macro_(src1[x+3]), scalar);           \
    dst[x+2] = (uchar)-f0;                                      \
    dst[x+3] = (uchar)-f1;                                      \
}                                                               \
                                                                \
for(; x < size.width; x++)                                    \
{                                                               \
    int f0 = __op__(_toggle_macro_(src1[x]), scalar);         \
    dst[x] = (uchar)-f0;                                        \
}


#define ICC_DEF_CMP_CONST_FUNC(__op__, name, flavor, arrtype,  \
                                worktype, _toggle_macro_)       \
static CStatus C_STDCALL                                      \
icv##name##C_##flavor##_C1R(const arrtype* src1, int step1,    \
                             uchar* dst, int step,              \
                             CSize size, worktype* pScalar)   \
{                                                               \
    worktype scalar = *pScalar;                                 \
    step1 /= sizeof(src1[0]); step /= sizeof(dst[0]);           \
                                                                \
    for(; size.height--; src1 += step1, dst += step)          \
    {                                                           \
        int x;                                                  \
        ICC_DEF_CMP_CONST_CASE_C1(__op__, _toggle_macro_)     \
    }                                                           \
                                                                \
    return CC_OK;                                               \
}


#define ICC_DEF_CMP_ALL(flavor, arrtype, worktype, _toggle_macro_)            \
ICC_DEF_CMP_FUNC(CC_GT, CmpGT, flavor, arrtype, worktype, _toggle_macro_)     \
ICC_DEF_CMP_FUNC(CC_EQ, CmpEQ, flavor, arrtype, worktype, _toggle_macro_)     \
ICC_DEF_CMP_CONST_FUNC(CC_GT, CmpGT, flavor, arrtype, worktype, _toggle_macro_)\
ICC_DEF_CMP_CONST_FUNC(CC_GE, CmpGE, flavor, arrtype, worktype, _toggle_macro_)\
ICC_DEF_CMP_CONST_FUNC(CC_EQ, CmpEQ, flavor, arrtype, worktype, _toggle_macro_)

ICC_DEF_CMP_ALL(8u, uchar, int, CC_NOP)
ICC_DEF_CMP_ALL(16u, ushort, int, CC_NOP)
ICC_DEF_CMP_ALL(16s, short, int, CC_NOP)
ICC_DEF_CMP_ALL(32s, int, int, CC_NOP)
ICC_DEF_CMP_ALL(32f, float, double, CC_NOP)
ICC_DEF_CMP_ALL(64f, double, double, CC_NOP)

#define icvCmpGT_8s_C1R     0
#define icvCmpEQ_8s_C1R     0
#define icvCmpGTC_8s_C1R    0
#define icvCmpGEC_8s_C1R    0
#define icvCmpEQC_8s_C1R    0

CC_DEF_INIT_FUNC_TAB_2D(CmpGT, C1R)
CC_DEF_INIT_FUNC_TAB_2D(CmpEQ, C1R)
CC_DEF_INIT_FUNC_TAB_2D(CmpGTC, C1R)
CC_DEF_INIT_FUNC_TAB_2D(CmpGEC, C1R)
CC_DEF_INIT_FUNC_TAB_2D(CmpEQC, C1R)


/***************************************** cvCmp ****************************************/

CC_IMPL void
cvCmp(const img_t* src1, const img_t* src2,
       img_t* dst, CCmpType cmp_op)
{
    static CvFuncTable cmp_tab[2];
    static int inittab = 0;

    CC_FUNCNAME("cvCmp");

    __BEGIN__;

    TypeId type;
    int invflag = 0, coi = 0;
    CvCmpOp ipp_cmp_op;
    int src1_step, src2_step, dst_step;
    
    const img_t *temp;
    CSize size;
    CvFunc2D_3A func;

    if(!inittab)
    {
        icvInitCmpGTC1RTable(cmp_tab[0]);
        icvInitCmpEQC1RTable(cmp_tab[1]);
        inittab = 1;
    }

    switch(cmp_op)
    {
    case CC_CMP_GT:
    case CC_CMP_EQ:
        break;
    case CC_CMP_GE:
        CC_SWAP(src1, src2, temp);
        invflag = 1;
        break;
    case CC_CMP_LT:
        CC_SWAP(src1, src2, temp);
        break;
    case CC_CMP_LE:
        invflag = 1;
        break;
    case CC_CMP_NE:
        cmp_op = CC_CMP_EQ;
        invflag = 1;
        break;
    default:
        CC_ERROR(CC_StsBadArg, "Unknown comparison operation");
    }

    if(!CC_ARE_TYPES_EQ(src1, src2))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(CC_MAT_CN(src1) != 1)
        CC_ERROR(CC_StsUnsupportedFormat, "Input arrays must be single-channel");

    if(!CC_IS_MASK_ARR(dst))
        CC_ERROR(CC_StsUnsupportedFormat, "Destination array should be 8uC1 or 8sC1");

    if(!CC_ARE_SIZES_EQ(src1, src2) ||
        !CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    type = CC_MAT_DEPTH(src1);
    size = cvGetMatSize(src1);

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src1_step = src2_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }
    else
    {
        src1_step = src1->step;
        src2_step = src2->step;
        dst_step = dst->step;
    }

    func = (CvFunc2D_3A)(cmp_tab[cmp_op == CC_CMP_EQ][type]);

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    ipp_cmp_op = cmp_op == CC_CMP_EQ ? cvCmpEq : cvCmpGreater;

    {
        IPPI_CALL(func(src1->tt.data, src1_step, src2->tt.data, src2_step,
                         dst->tt.data, dst_step, size));
    }

    if(invflag)
        IPPI_CALL(icvNot_8u_C1R(dst->tt.data, dst_step,
                           dst->tt.data, dst_step, size));

    __END__;
}


/*************************************** cvCmpS *****************************************/

CC_IMPL void
cvCmpS(const img_t* src1, double value, img_t* dst, CCmpType cmp_op)
{
    static CvFuncTable cmps_tab[3];
    static int inittab = 0;

    CC_FUNCNAME("cvCmpS");

    __BEGIN__;

    TypeId type;
    int y, coi = 0;
    int invflag = 0, ipp_cmp_op;
    int src1_step, dst_step;
    
    CSize size;
    int ival = 0;

	ASSERT(dst!=src1);
	cvSetMat(dst, src1->rows, src1->cols, CC_8U);

    if(!inittab)
    {
        icvInitCmpEQCC1RTable(cmps_tab[CC_CMP_EQ]);
        icvInitCmpGTCC1RTable(cmps_tab[CC_CMP_GT]);
        icvInitCmpGECC1RTable(cmps_tab[CC_CMP_GE]);
        inittab = 1;
    }

    switch(cmp_op)
    {
    case CC_CMP_GT:
    case CC_CMP_EQ:
    case CC_CMP_GE:
        break;
    case CC_CMP_LT:
        invflag = 1;
        cmp_op = CC_CMP_GE;
        break;
    case CC_CMP_LE:
        invflag = 1;
        cmp_op = CC_CMP_GT;
        break;
    case CC_CMP_NE:
        invflag = 1;
        cmp_op = CC_CMP_EQ;
        break;
    default:
        CC_ERROR(CC_StsBadArg, "Unknown comparison operation");
    }

    if(!CC_IS_MASK_ARR(dst))
        CC_ERROR(CC_StsUnsupportedFormat, "Destination array should be 8uC1 or 8sC1");

    if(CC_MAT_CN(src1) != 1)
        CC_ERROR(CC_StsUnsupportedFormat, "Input array must be single-channel");

    if(!CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    type = CC_MAT_DEPTH(src1);
    size = cvGetMatSize(src1);

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src1_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }
    else
    {
        src1_step = src1->step;
        dst_step = dst->step;
    }

    if(type <= CC_32S)
    {
        ival = cRound(value);
        if(type == CC_8U || type == CC_16S)
        {
            int minval = type == CC_8U ? 0 : -32768;
            int maxval = type == CC_8U ? 255 : 32767;
            int fillval = -1;
            if(ival < minval)
                fillval = cmp_op == CC_CMP_NE || cmp_op == CC_CMP_GE || cmp_op == CC_CMP_GT ? 255 : 0;
            else if(ival > maxval)
                fillval = cmp_op == CC_CMP_NE || cmp_op == CC_CMP_LE || cmp_op == CC_CMP_LT ? 255 : 0;
            if(fillval >= 0)
            {
                fillval ^= invflag ? 255 : 0;
                for(y = 0; y < size.height; y++)
                    memset(dst->tt.data + y*dst_step, fillval, size.width);
                EXIT;
            }
        }
    }

    ipp_cmp_op = cmp_op == CC_CMP_EQ ? cvCmpEq :
                 cmp_op == CC_CMP_GE ? cvCmpGreaterEq : cvCmpGreater;
                 
    {
        CvFunc2D_2A1P func = (CvFunc2D_2A1P)(cmps_tab[cmp_op][type]);
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(type <= CC_32S)
        {
            IPPI_CALL(func(src1->tt.data, src1_step, dst->tt.data,
                             dst_step, size, &ival));
        }
        else
        {
            IPPI_CALL(func(src1->tt.data, src1_step, dst->tt.data,
                             dst_step, size, &value));
        }
    }

    if(invflag)
        IPPI_CALL(icvNot_8u_C1R(dst->tt.data, dst_step,
                           dst->tt.data, dst_step, size));

    __END__;
}


/****************************************************************************************\
*                                       Min/Max                                          *
\****************************************************************************************/


#define ICC_DEF_MINMAX_FUNC(__op__, name, flavor, arrtype, \
                             worktype, _toggle_macro_)     \
static CStatus C_STDCALL                                  \
icv##name##_##flavor##_C1R(const arrtype* src1, int step1, \
    const arrtype* src2, int step2,                         \
    arrtype* dst, int step, CSize size)                   \
{                                                           \
    step1 /= sizeof(src1[0]); step2 /= sizeof(src2[0]);     \
    step /= sizeof(dst[0]);                                 \
                                                            \
    for(; size.height--; src1 += step1,                    \
            src2 += step2, dst += step)                    \
    {                                                       \
        int x;                                              \
        for(x = 0; x <= size.width - 4; x += 4)           \
        {                                                   \
            worktype a0 = _toggle_macro_(src1[x]);          \
            worktype b0 = _toggle_macro_(src2[x]);          \
            worktype a1 = _toggle_macro_(src1[x+1]);        \
            worktype b1 = _toggle_macro_(src2[x+1]);        \
            a0 = __op__(a0, b0);                          \
            a1 = __op__(a1, b1);                          \
            dst[x] = (arrtype)_toggle_macro_(a0);           \
            dst[x+1] = (arrtype)_toggle_macro_(a1);         \
            a0 = _toggle_macro_(src1[x+2]);                 \
            b0 = _toggle_macro_(src2[x+2]);                 \
            a1 = _toggle_macro_(src1[x+3]);                 \
            b1 = _toggle_macro_(src2[x+3]);                 \
            a0 = __op__(a0, b0);                          \
            a1 = __op__(a1, b1);                          \
            dst[x+2] = (arrtype)_toggle_macro_(a0);         \
            dst[x+3] = (arrtype)_toggle_macro_(a1);         \
        }                                                   \
                                                            \
        for(; x < size.width; x++)                        \
        {                                                   \
            worktype a0 = _toggle_macro_(src1[x]);          \
            worktype b0 = _toggle_macro_(src2[x]);          \
            a0 = __op__(a0, b0);                          \
            dst[x] = (arrtype)_toggle_macro_(a0);           \
        }                                                   \
    }                                                       \
                                                            \
    return CC_OK;                                           \
}


#define ICC_DEF_MINMAX_CONST_FUNC(__op__, name,            \
    flavor, arrtype, worktype, _toggle_macro_)              \
static CStatus C_STDCALL                                  \
icv##name##C_##flavor##_C1R(const arrtype* src1, int step1,\
                             arrtype* dst, int step,        \
                             CSize size, worktype* pScalar)\
{                                                           \
    worktype scalar = _toggle_macro_(*pScalar);             \
    step1 /= sizeof(src1[0]); step /= sizeof(dst[0]);       \
                                                            \
    for(; size.height--; src1 += step1, dst += step)      \
    {                                                       \
        int x;                                              \
        for(x = 0; x <= size.width - 4; x += 4)           \
        {                                                   \
            worktype a0 = _toggle_macro_(src1[x]);          \
            worktype a1 = _toggle_macro_(src1[x+1]);        \
            a0 = __op__(a0, scalar);                      \
            a1 = __op__(a1, scalar);                      \
            dst[x] = (arrtype)_toggle_macro_(a0);           \
            dst[x+1] = (arrtype)_toggle_macro_(a1);         \
            a0 = _toggle_macro_(src1[x+2]);                 \
            a1 = _toggle_macro_(src1[x+3]);                 \
            a0 = __op__(a0, scalar);                      \
            a1 = __op__(a1, scalar);                      \
            dst[x+2] = (arrtype)_toggle_macro_(a0);         \
            dst[x+3] = (arrtype)_toggle_macro_(a1);         \
        }                                                   \
                                                            \
        for(; x < size.width; x++)                        \
        {                                                   \
            worktype a0 = _toggle_macro_(src1[x]);          \
            a0 = __op__(a0, scalar);                      \
            dst[x] = (arrtype)_toggle_macro_(a0);           \
        }                                                   \
    }                                                       \
                                                            \
    return CC_OK;                                           \
}


#define ICC_DEF_MINMAX_ALL(flavor, arrtype, worktype,                             \
                            _toggle_macro_, _min_op_, _max_op_)                   \
ICC_DEF_MINMAX_FUNC(_min_op_, Min, flavor, arrtype, worktype, _toggle_macro_)    \
ICC_DEF_MINMAX_FUNC(_max_op_, Max, flavor, arrtype, worktype, _toggle_macro_)    \
ICC_DEF_MINMAX_CONST_FUNC(_min_op_, Min, flavor, arrtype, worktype, _toggle_macro_)\
ICC_DEF_MINMAX_CONST_FUNC(_max_op_, Max, flavor, arrtype, worktype, _toggle_macro_)

ICC_DEF_MINMAX_ALL(8u, uchar, int, CC_NOP, CC_MIN_8U, CC_MAX_8U)
ICC_DEF_MINMAX_ALL(16u, ushort, int, CC_NOP, CC_IMIN, CC_IMAX)
ICC_DEF_MINMAX_ALL(16s, short, int, CC_NOP, CC_IMIN, CC_IMAX)
ICC_DEF_MINMAX_ALL(32s, int, int, CC_NOP, CC_IMIN, CC_IMAX)
ICC_DEF_MINMAX_ALL(32f, int, int, CC_TOGGLE_FLT, CC_IMIN, CC_IMAX)
ICC_DEF_MINMAX_ALL(64f, double, double, CC_NOP, MIN, MAX)

#define icvMin_8s_C1R     0
#define icvMax_8s_C1R     0
#define icvMinC_8s_C1R    0
#define icvMaxC_8s_C1R    0

CC_DEF_INIT_FUNC_TAB_2D(Min, C1R)
CC_DEF_INIT_FUNC_TAB_2D(Max, C1R)
CC_DEF_INIT_FUNC_TAB_2D(MinC, C1R)
CC_DEF_INIT_FUNC_TAB_2D(MaxC, C1R)

/*********************************** cvMin & cvMax **************************************/

static void
icvMinMax(const img_t* src1, const img_t* src2,
           img_t* dst, int is_max)
{
    static CvFuncTable minmax_tab[2];
    static int inittab = 0;

    CC_FUNCNAME("icvMinMax");

    __BEGIN__;

    TypeId type;
    int coi = 0;
    int src1_step, src2_step, dst_step;
   
    CSize size;
    CvFunc2D_3A func;

    if(!inittab)
    {
        icvInitMinC1RTable(minmax_tab[0]);
        icvInitMaxC1RTable(minmax_tab[1]);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src1, src2) ||
        !CC_ARE_TYPES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(CC_MAT_CN(src1) != 1)
        CC_ERROR(CC_StsUnsupportedFormat, "Input arrays must be single-channel");

    if(!CC_ARE_SIZES_EQ(src1, src2) ||
        !CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    type = CC_MAT_DEPTH(src1);
    size = cvGetMatSize(src1);

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src1_step = src2_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }
    else
    {
        src1_step = src1->step;
        src2_step = src2->step;
        dst_step = dst->step;
    }

    func = (CvFunc2D_3A)(minmax_tab[is_max != 0][type]);

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    IPPI_CALL(func(src1->tt.data, src1_step, src2->tt.data, src2_step,
                     dst->tt.data, dst_step, size));

    __END__;
}


CC_IMPL void
cvMin(const img_t* src1, const img_t* src2, img_t* dst)
{
    icvMinMax(src1, src2, dst, 0);
}


CC_IMPL void
cvMax(const img_t* src1, const img_t* src2, img_t* dst)
{
    icvMinMax(src1, src2, dst, 1);
}


/********************************* cvMinS / cvMaxS **************************************/

static void
icvMinMaxS(const img_t* src1, double value, img_t* dst, int is_max)
{
    static CvFuncTable minmaxs_tab[2];
    static int inittab = 0;

    CC_FUNCNAME("icvMinMaxS");

    __BEGIN__;

    TypeId type;
    int coi = 0;
    int src1_step, dst_step;
    CSize size;
    CvFunc2D_2A1P func;
    union
    {
        int i;
        float f;
        double d;
    }
    buf;

    if(!inittab)
    {
        icvInitMinCC1RTable(minmaxs_tab[0]);
        icvInitMaxCC1RTable(minmaxs_tab[1]);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(CC_MAT_CN(src1) != 1)
        CC_ERROR(CC_StsUnsupportedFormat, "Input array must be single-channel");

    if(!CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    type = CC_MAT_DEPTH(src1);

    if(type <= CC_32S)
    {
        buf.i = cRound(value);
        if(type == CC_8U)
            buf.i = CC_CAST_8U(buf.i);
        else if(type == CC_8S)
            buf.i = CC_CAST_8S(buf.i);
        else if(type == CC_16U)
            buf.i = CC_CAST_16U(buf.i);
        else if(type == CC_16S)
            buf.i = CC_CAST_16S(buf.i);
    }
    else if(type == CC_32F)
        buf.f = (float)value;
    else
        buf.d = value;

    size = cvGetMatSize(src1);

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src1_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }
    else
    {
        src1_step = src1->step;
        dst_step = dst->step;
    }

    func = (CvFunc2D_2A1P)(minmaxs_tab[is_max][type]);

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    if(type == CC_8U && size.width*size.height >= 1024)
    {
        int i;
        uchar tab[256];
        img_t _tab;
        cvMat(&_tab, 1, 256, CC_8U, 1, tab);

        if(is_max)
        {
            for(i = 0; i < buf.i; i++)
                tab[i] = (uchar)buf.i;
            for(; i < 256; i++)
                tab[i] = (uchar)i;
        }
        else
        {
            for(i = 0; i < buf.i; i++)
                tab[i] = (uchar)i;
            for(; i < 256; i++)
                tab[i] = (uchar)buf.i;
        }

        cvLUT(src1, dst, &_tab);
        EXIT;
    }

    IPPI_CALL(func(src1->tt.data, src1_step, dst->tt.data,
                     dst_step, size, &buf));

    __END__;
}


CC_IMPL void
cvMinS(const img_t* srcarr, double value, img_t* dst)
{
    icvMinMaxS(srcarr, value, dst, 0);
}


CC_IMPL void
cvMaxS(const img_t* srcarr, double value, img_t* dst)
{
    icvMinMaxS(srcarr, value, dst, 1);
}


/****************************************************************************************\
*                                  Absolute Difference                                   *
\****************************************************************************************/

#define  ICC_DEF_BIN_ABS_DIFF_2D(name, arrtype, temptype, abs_macro, cast_macro)\
IPCVAPI_IMPL(CStatus,                                 \
name,(const arrtype* src1, int step1,                  \
       const arrtype* src2, int step2,                  \
       arrtype* dst, int step, CSize size),           \
       (src1, step1, src2, step2, dst, step, size))     \
{                                                       \
    step1 /= sizeof(src1[0]); step2 /= sizeof(src2[0]); \
    step /= sizeof(dst[0]);                             \
                                                        \
    for(; size.height--; src1 += step1, src2 += step2, \
                          dst += step)                 \
    {                                                   \
        int i;                                          \
                                                        \
        for(i = 0; i <= size.width - 4; i += 4)       \
        {                                               \
            temptype t0 = src1[i] - src2[i];            \
            temptype t1 = src1[i+1] - src2[i+1];        \
                                                        \
            t0 = (temptype)abs_macro(t0);               \
            t1 = (temptype)abs_macro(t1);               \
                                                        \
            dst[i] = cast_macro(t0);                    \
            dst[i+1] = cast_macro(t1);                  \
                                                        \
            t0 = src1[i+2] - src2[i+2];                 \
            t1 = src1[i+3] - src2[i+3];                 \
                                                        \
            t0 = (temptype)abs_macro(t0);               \
            t1 = (temptype)abs_macro(t1);               \
                                                        \
            dst[i+2] = cast_macro(t0);                  \
            dst[i+3] = cast_macro(t1);                  \
        }                                               \
                                                        \
        for(; i < size.width; i++)                    \
        {                                               \
            temptype t0 = src1[i] - src2[i];            \
            t0 = (temptype)abs_macro(t0);               \
            dst[i] = cast_macro(t0);                    \
        }                                               \
    }                                                   \
                                                        \
    return CC_OK;                                       \
}


#define  ICC_DEF_UN_ABS_DIFF_2D(name, arrtype, temptype, abs_macro, cast_macro)\
static CStatus C_STDCALL                              \
name(const arrtype* src0, int step1,                   \
      arrtype* dst0, int step,                          \
      CSize size, const temptype* scalar)             \
{                                                       \
    step1 /= sizeof(src0[0]); step /= sizeof(dst0[0]);  \
                                                        \
    for(; size.height--; src0 += step1, dst0 += step) \
    {                                                   \
        int i, len = size.width;                        \
        const arrtype* src = src0;                      \
        arrtype* dst = dst0;                            \
                                                        \
        for(; (len -= 12) >= 0; dst += 12, src += 12) \
        {                                               \
            temptype t0 = src[0] - scalar[0];           \
            temptype t1 = src[1] - scalar[1];           \
                                                        \
            t0 = (temptype)abs_macro(t0);               \
            t1 = (temptype)abs_macro(t1);               \
                                                        \
            dst[0] = cast_macro(t0);                  \
            dst[1] = cast_macro(t1);                  \
                                                        \
            t0 = src[2] - scalar[2];                    \
            t1 = src[3] - scalar[3];                    \
                                                        \
            t0 = (temptype)abs_macro(t0);               \
            t1 = (temptype)abs_macro(t1);               \
                                                        \
            dst[2] = cast_macro(t0);                  \
            dst[3] = cast_macro(t1);                  \
                                                        \
            t0 = src[4] - scalar[4];                    \
            t1 = src[5] - scalar[5];                    \
                                                        \
            t0 = (temptype)abs_macro(t0);               \
            t1 = (temptype)abs_macro(t1);               \
                                                        \
            dst[4] = cast_macro(t0);                  \
            dst[5] = cast_macro(t1);                  \
                                                        \
            t0 = src[6] - scalar[6];                    \
            t1 = src[7] - scalar[7];                    \
                                                        \
            t0 = (temptype)abs_macro(t0);               \
            t1 = (temptype)abs_macro(t1);               \
                                                        \
            dst[6] = cast_macro(t0);                  \
            dst[7] = cast_macro(t1);                  \
                                                        \
            t0 = src[8] - scalar[8];                    \
            t1 = src[9] - scalar[9];                    \
                                                        \
            t0 = (temptype)abs_macro(t0);               \
            t1 = (temptype)abs_macro(t1);               \
                                                        \
            dst[8] = cast_macro(t0);                  \
            dst[9] = cast_macro(t1);                  \
                                                        \
            t0 = src[10] - scalar[10];                  \
            t1 = src[11] - scalar[11];                  \
                                                        \
            t0 = (temptype)abs_macro(t0);               \
            t1 = (temptype)abs_macro(t1);               \
                                                        \
            dst[10] = cast_macro(t0);                 \
            dst[11] = cast_macro(t1);                 \
        }                                               \
                                                        \
        for((len) += 12, i = 0; i < (len); i++)       \
        {                                               \
            temptype t0 = src[i] - scalar[i];           \
            t0 = (temptype)abs_macro(t0);               \
            dst[i] = cast_macro(t0);                  \
        }                                               \
    }                                                   \
                                                        \
    return CC_OK;                                       \
}


#define  ICC_TO_8U(x)     ((uchar)(x))
#define  ICC_TO_16U(x)    ((ushort)(x))

ICC_DEF_BIN_ABS_DIFF_2D(icvAbsDiff_8u_C1R, uchar, int, CC_IABS, ICC_TO_8U)
ICC_DEF_BIN_ABS_DIFF_2D(icvAbsDiff_16u_C1R, ushort, int, CC_IABS, ICC_TO_16U)
ICC_DEF_BIN_ABS_DIFF_2D(icvAbsDiff_16s_C1R, short, int, CC_IABS, CC_CAST_16S)
ICC_DEF_BIN_ABS_DIFF_2D(icvAbsDiff_32s_C1R, int, int, CC_IABS, CC_CAST_32S)
ICC_DEF_BIN_ABS_DIFF_2D(icvAbsDiff_32f_C1R, float, float, fabs, CC_CAST_32F)
ICC_DEF_BIN_ABS_DIFF_2D(icvAbsDiff_64f_C1R, double, double, fabs, CC_CAST_64F)

ICC_DEF_UN_ABS_DIFF_2D(icvAbsDiffC_8u_CnR, uchar, int, CC_IABS, CC_CAST_8U)
ICC_DEF_UN_ABS_DIFF_2D(icvAbsDiffC_16u_CnR, ushort, int, CC_IABS, CC_CAST_16U)
ICC_DEF_UN_ABS_DIFF_2D(icvAbsDiffC_16s_CnR, short, int, CC_IABS, CC_CAST_16S)
ICC_DEF_UN_ABS_DIFF_2D(icvAbsDiffC_32s_CnR, int, int, CC_IABS, CC_CAST_32S)
ICC_DEF_UN_ABS_DIFF_2D(icvAbsDiffC_32f_CnR, float, float, fabs, CC_CAST_32F)
ICC_DEF_UN_ABS_DIFF_2D(icvAbsDiffC_64f_CnR, double, double, fabs, CC_CAST_64F)


#define  ICC_INIT_MINI_FUNC_TAB_2D(FUNCNAME, suffix)          \
static void icvInit##FUNCNAME##Table(CvFuncTable  tab)        \
{                                                               \
    tab[CC_8U] = (void*)icv##FUNCNAME##_8u_##suffix;     \
    tab[CC_16U] = (void*)icv##FUNCNAME##_16u_##suffix;   \
    tab[CC_16S] = (void*)icv##FUNCNAME##_16s_##suffix;   \
    tab[CC_32S] = (void*)icv##FUNCNAME##_32s_##suffix;   \
    tab[CC_32F] = (void*)icv##FUNCNAME##_32f_##suffix;   \
    tab[CC_64F] = (void*)icv##FUNCNAME##_64f_##suffix;   \
}


ICC_INIT_MINI_FUNC_TAB_2D(AbsDiff, C1R)
ICC_INIT_MINI_FUNC_TAB_2D(AbsDiffC, CnR)


CC_IMPL  void
cvAbsDiff(const img_t* src1, const img_t* src2, img_t* dst)
{
    static CvFuncTable adiff_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvAbsDiff");

    __BEGIN__;

    int coi1 = 0, coi2 = 0, coi3 = 0;
    int src1_step, src2_step, dst_step;
    CSize size;
    TypeId type;

    if(!inittab)
    {
        icvInitAbsDiffTable(adiff_tab);
        inittab = 1;
    }

    if(coi1 != 0 || coi2 != 0 || coi3 != 0)
        CC_ERROR(CC_BadCOI, "");

    if(!CC_ARE_SIZES_EQ(src1, src2))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    size = cvGetMatSize(src1);
    type = CC_MAT_DEPTH(src1);

    if(!CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    if(!CC_ARE_TYPES_EQ(src1, src2))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_TYPES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    size.width *= CC_MAT_CN(src1);

    src1_step = src1->step;
    src2_step = src2->step;
    dst_step = dst->step;

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        size.height = 1;
        src1_step = src2_step = dst_step = CC_STUB_STEP;
    }

    {
        CvFunc2D_3A func = (CvFunc2D_3A)
            (adiff_tab[type]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src1->tt.data, src1_step, src2->tt.data, src2_step,
                         dst->tt.data, dst_step, size));
    }

    __END__;
}


CC_IMPL void
cvAbsDiffS(const img_t* src, img_t* dst, CScalar scalar)
{
    static CvFuncTable adiffs_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvAbsDiffS");

    __BEGIN__;

    int coi1 = 0, coi2 = 0;
    TypeId type, sctype;
    
    
    int src_step, dst_step;
    double buf[12];
    CSize size;
    int cn = CC_MAT_CN(src);

    if(!inittab)
    {
        icvInitAbsDiffCTable(adiffs_tab);
        inittab = 1;
    }

    
    

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    sctype = type = CC_MAT_DEPTH(src);
    if(type < CC_32S)
        sctype = CC_32S;

    size = cvGetMatSize(src);
    size.width *= CC_MAT_CN(src);

    src_step = src->step;
    dst_step = dst->step;

    if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        size.height = 1;
        src_step = dst_step = CC_STUB_STEP;
    }

    CC_CALL(cvScalarToRawData(&scalar, buf, sctype, cn, 1));

    {
        CvFunc2D_2A1P func = (CvFunc2D_2A1P)
            (adiffs_tab[type]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src_step, dst->tt.data,
                         dst_step, size, buf));
    }

    __END__;
}


