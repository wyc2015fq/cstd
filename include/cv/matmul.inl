
/****************************************************************************************\
*                                         cvGEMM                                         *
\****************************************************************************************/


static void
icvGEMM_CopyBlock(const uchar* src, int src_step,
                   uchar* dst, int dst_step,
                   CSize size, int pix_size)
{
    int j;
    size.width = size.width * (pix_size / sizeof(int));

    for(; size.height--; src += src_step, dst += dst_step)
    {
        for(j = 0; j <= size.width - 4; j += 4)
        {
            int t0 = ((const int*)src)[j];
            int t1 = ((const int*)src)[j+1];
            ((int*)dst)[j] = t0;
            ((int*)dst)[j+1] = t1;
            t0 = ((const int*)src)[j+2];
            t1 = ((const int*)src)[j+3];
            ((int*)dst)[j+2] = t0;
            ((int*)dst)[j+3] = t1;
        }

        for(; j < size.width; j++)
            ((int*)dst)[j] = ((const int*)src)[j];
    }
}


static void
icvGEMM_TransposeBlock(const uchar* src, int src_step,
                        uchar* dst, int dst_step,
                        CSize size, int pix_size)
{
    int i, j;
    for(i = 0; i < size.width; i++, dst += dst_step, src += pix_size)
    {
        const uchar* _src = src;
        switch(pix_size)
        {
        case sizeof(int):
            for(j = 0; j < size.height; j++, _src += src_step)
                ((int*)dst)[j] = ((int*)_src)[0];
            break;
        case sizeof(int)*2:
            for(j = 0; j < size.height*2; j += 2, _src += src_step)
            {
                int t0 = ((int*)_src)[0];
                int t1 = ((int*)_src)[1];
                ((int*)dst)[j] = t0;
                ((int*)dst)[j+1] = t1;
            }
            break;
        case sizeof(int)*4:
            for(j = 0; j < size.height*4; j += 4, _src += src_step)
            {
                int t0 = ((int*)_src)[0];
                int t1 = ((int*)_src)[1];
                ((int*)dst)[j] = t0;
                ((int*)dst)[j+1] = t1;
                t0 = ((int*)_src)[2];
                t1 = ((int*)_src)[3];
                ((int*)dst)[j+2] = t0;
                ((int*)dst)[j+3] = t1;
            }
            break;
        default:
            assert(0);
            return;
        }
    }
}

#define CCFUNC(NAME)  NAME##_32f_C1R
#define arrtype  float
#define worktype double
#define worktype0  0
#define to_worktype(x)  (double)(x)
#define to_arrtype(x)  (float)(x)
#define add(a, b)  ((a)*(b))
#define muladd2(d, a, b)  d+=((a)*(b))
#define mulsadd2(d, a, b)  d+=((a)*(b))
#define mul(a, b)  ((a)*(b))
#define muls(a, b)  ((a)*(b))
#include "matmul_impl.inl"

#define CCFUNC(NAME)  NAME##_64f_C1R
#define arrtype  double
#define worktype double
#define worktype0  0
#define to_worktype(x)  (x)
#define to_arrtype(x)  (x)
#define add(a, b)  ((a)*(b))
#define muladd2(d, a, b)  d+=((a)*(b))
#define mulsadd2(d, a, b)  d+=((a)*(b))
#define mul(a, b)  ((a)*(b))
#define muls(a, b)  ((a)*(b))
#include "matmul_impl.inl"

#if 0
#define CCFUNC(NAME)  NAME##_32f_C2R
#define arrtype  CComplex32f
#define worktype CComplex64f
#define worktype0  CComplex64f_0
#define to_worktype(x)  cvtCComplex_32f_64f(x)
#define to_arrtype(x)  cvtCComplex_64f_32f(x)
#define add(a, b)  CComplex_add_64f((a), (b))
#define muladd2(d, a, b)  d=CComplex_muladd_64f((a), (b), d)
#define mulsadd2(d, a, b)  d=CComplex_mulsadd_64f((a), (b), d)
#define mul(a, b)  CComplex_mul_64f((a), (b))
#define muls(a, b)  CComplex_muls_64f((a), (b))
#else
#define CCFUNC(NAME)  NAME##_32f_C2R
#define arrtype  CComplex32f
#define worktype CComplex32f
#define worktype0  CComplex32f_0
#define to_worktype(x)  (x)
#define to_arrtype(x)  (x)
#define add(a, b)  CComplex_add_32f((a), (b))
#define muladd2(d, a, b)  d=CComplex_muladd_32f((a), (b), d)
#define mulsadd2(d, a, b)  d=CComplex_mulsadd_32f((a), (b), d)
#define mul(a, b)  CComplex_mul_32f((a), (b))
#define muls(a, b)  CComplex_muls_32f((a), (b))
#endif
#include "matmul_impl.inl"

#define CCFUNC(NAME)  NAME##_64f_C2R
#define arrtype  CComplex64f
#define worktype CComplex64f
#define worktype0  CComplex64f_0
#define to_worktype(x)  (x)
#define to_arrtype(x)  (x)
#define add(a, b)  CComplex_add_64f((a), (b))
#define muladd2(d, a, b)  d=CComplex_muladd_64f((a), (b), d)
#define mulsadd2(d, a, b)  d=CComplex_mulsadd_64f((a), (b), d)
#define mul(a, b)  CComplex_mul_64f((a), (b))
#define muls(a, b)  CComplex_muls_64f((a), (b))
#include "matmul_impl.inl"


#if 0

ICC_DEF_GEMM_SINGLE_MUL(32f_C1R, float, double)
ICC_DEF_GEMM_BLOCK_MUL(32f_C1R, float, double)
ICC_DEF_GEMM_STORE(32f_C1R, float, double)

ICC_DEF_GEMM_SINGLE_MUL(64f_C1R, double, double)
ICC_DEF_GEMM_BLOCK_MUL(64f_C1R, double, double)
ICC_DEF_GEMM_STORE(64f_C1R, double, double)

ICC_DEF_GEMM_SINGLE_MUL(32f_C2R, CComplex32f, CComplex64f)
ICC_DEF_GEMM_BLOCK_MUL(32f_C2R, CComplex32f, CComplex64f)
ICC_DEF_GEMM_STORE(32f_C2R, CComplex32f, CComplex64f)

ICC_DEF_GEMM_SINGLE_MUL(64f_C2R, CComplex64f, CComplex64f)
ICC_DEF_GEMM_BLOCK_MUL(64f_C2R, CComplex64f, CComplex64f)
ICC_DEF_GEMM_STORE(64f_C2R, CComplex64f, CComplex64f)

#endif

typedef CStatus (C_STDCALL *CvGEMMSingleMulFunc)(const void* src1, size_t step1,
                   const void* src2, size_t step2, const void* src3, size_t step3,
                   void* dst, size_t dststep, CSize srcsize, CSize dstsize,
                   double alpha, double beta, int flags);

typedef CStatus (C_STDCALL *CvGEMMBlockMulFunc)(const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   CSize srcsize, CSize dstsize, int flags);

typedef CStatus (C_STDCALL *CvGEMMStoreFunc)(const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   CSize dstsize, double alpha, double beta, int flags);


static void icvInitGEMMTable(CvBigFuncTable  single_mul_tab,
                              CvBigFuncTable  block_mul_tab,
                              CvBigFuncTable  store_tab)
{
    single_mul_tab[CC_32F][1] = (void*)icvGEMMSingleMul_32f_C1R;
    single_mul_tab[CC_64F][1] = (void*)icvGEMMSingleMul_64f_C1R;
    single_mul_tab[CC_32F][2] = (void*)icvGEMMSingleMul_32f_C2R;
    single_mul_tab[CC_64F][2] = (void*)icvGEMMSingleMul_64f_C2R;
    block_mul_tab[CC_32F][1] = (void*)icvGEMMBlockMul_32f_C1R;
    block_mul_tab[CC_64F][1] = (void*)icvGEMMBlockMul_64f_C1R;
    block_mul_tab[CC_32F][2] = (void*)icvGEMMBlockMul_32f_C2R;
    block_mul_tab[CC_64F][2] = (void*)icvGEMMBlockMul_64f_C2R;
    store_tab[CC_32F][1] = (void*)icvGEMMStore_32f_C1R;
    store_tab[CC_64F][1] = (void*)icvGEMMStore_64f_C1R;
    store_tab[CC_32F][2] = (void*)icvGEMMStore_32f_C2R;
    store_tab[CC_64F][2] = (void*)icvGEMMStore_64f_C2R;
}


#define cvMatMulAddEx cvGEMM
#define cvMatMulAdd(src1, src2, src3, dst) cvGEMM((src1), (src2), 1., (src3), 1., (dst), 0)
#define cvMatMul(src1, src2, dst)  cvMatMulAdd((src1), (src2), NULL, (dst))
#define cvMatMul2(src1, src2, dst, flags)  cvGEMM((src1), (src2), 1., NULL, 0, (dst), flags)

CC_IMPL img_t*
cvGEMM(const img_t* Aarr, const img_t* Barr, double alpha,
       const img_t* Carr, double beta, img_t* Darr, int flags CC_DEFAULT(0))
{
    const int block_lin_size = 4;
    const int block_size = block_lin_size * block_lin_size;

    static CvBigFuncTable single_mul_tab, block_mul_tab, store_tab;
    static int inittab = 0;
    static double zero[] = {0,0,0,0};
    static float zerof[] = {0,0,0,0};
    
    uchar* buffer = 0;
    int local_alloc = 0;
    uchar* block_buffer = 0;
    img_t tmp_d[1] = {0};
    img_t* Darr0 = Darr;

    CC_FUNCNAME("cvGEMM");

    __BEGIN__;

    img_t *A = (img_t*)Aarr;
    img_t *B = (img_t*)Barr;
    img_t *C = (img_t*)Carr;
    img_t *D = (img_t*)Darr;
    int len = 0;
    
    img_t stub3;
    CSize a_size, d_size;
    TypeId type = CC_MAT_DEPTH(A);
    int cn = CC_MAT_CN(A);
    ASSERT(1==cn || 2==cn);

    if (Darr==Aarr || Darr==Barr) {
      Darr = tmp_d;
    }
    if (Darr!=Aarr && Darr!=Barr) {
      int is_a_t = (flags & CC_GEMM_A_T)!=0;
      int is_b_t = (flags & CC_GEMM_B_T)!=0;
      int is_c_t = (flags & CC_GEMM_C_T)!=0;
      int h = is_a_t==is_c_t ? Aarr->h : Aarr->w;
      int w = is_b_t==is_c_t ? Barr->w : Barr->h;
      cvSetMat(Darr, h, w, Aarr->tid, CC_MAT_CN(Aarr), NULL, CC_AUTO_STEP);
      cvZeroMat(Darr);
    }


    if(beta == 0)
        C = 0;

    if(C)
    {
        if(!CC_ARE_TYPES_EQ(C, D))
            CC_ERROR(CC_StsUnmatchedFormats, "");

        if((flags&CC_GEMM_C_T) == 0 && (C->cols != D->cols || C->rows != D->rows) ||
            (flags&CC_GEMM_C_T) != 0 && (C->rows != D->cols || C->cols != D->rows))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        if((flags & CC_GEMM_C_T) != 0 && C->tt.data == D->tt.data)
        {
            cvTranspose(C, D);
            C = D;
            flags &= ~CC_GEMM_C_T;
        }
    }
    else
    {
        C = &stub3;
        C->tt.data = 0;
        C->step = 0;
    }

    if(!CC_ARE_TYPES_EQ(A, B) || !CC_ARE_TYPES_EQ(A, D))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    a_size.width = A->cols;
    a_size.height = A->rows;
    d_size.width = D->cols;
    d_size.height = D->rows;

    switch(flags & (CC_GEMM_A_T|CC_GEMM_B_T))
    {
    case 0:
        len = B->rows;
        if(a_size.width != len ||
            B->cols != d_size.width ||
            a_size.height != d_size.height)
            CC_ERROR(CC_StsUnmatchedSizes, "");
        break;
    case 1:
        len = B->rows;
        if(a_size.height != len ||
            B->cols != d_size.width ||
            a_size.width != d_size.height)
            CC_ERROR(CC_StsUnmatchedSizes, "");
        break;
    case 2:
        len = B->cols;
        if(a_size.width != len ||
            B->rows != d_size.width ||
            a_size.height != d_size.height)
            CC_ERROR(CC_StsUnmatchedSizes, "");
        break;
    case 3:
        len = B->cols;
        if(a_size.height != len ||
            B->rows != d_size.width ||
            a_size.width != d_size.height)
            CC_ERROR(CC_StsUnmatchedSizes, "");
        break;
    }

    if(flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height))
    {
        int i;
        if(type == CC_64F)
        {
            double* d = D->tt.db;
            const double *a = A->tt.db, *b = B->tt.db, *c = C->tt.db;
            size_t d_step = D->step/sizeof(d[0]),
                   a_step = A->step/sizeof(a[0]),
                   b_step = B->step/sizeof(b[0]),
                   c_step = C->step/sizeof(c[0]);

            if(!c)
                c = zero;

            switch(len)
            {
            case 2:
                if(len == d_size.width && b != d)
                {
                    for(i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step)
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                else if(a != d)
                {
                    int c_step0 = 1;
                    if(c == zero)
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for(i = 0; i < d_size.width; i++, d++, b++, c += c_step0)
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                else
                    break;
                EXIT;
            case 3:
                if(len == d_size.width && b != d)
                {
                    for(i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step)
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        double t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                else if(a != d)
                {
                    int c_step0 = 1;
                    if(c == zero)
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for(i = 0; i < d_size.width; i++, d++, b++, c += c_step0)
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        double t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                else
                    break;
                EXIT;
            case 4:
                if(len == d_size.width && b != d)
                {
                    for(i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step)
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        double t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        double t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                else if(d_size.width <= 16 && a != d)
                {
                    int c_step0 = 1;
                    if(c == zero)
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for(i = 0; i < d_size.width; i++, d++, b++, c += c_step0)
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        double t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        double t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                else
                    break;
                EXIT;
            }
        }

        if(type == CC_32F)
        {
            float* d = D->tt.fl;
            const float *a = A->tt.fl, *b = B->tt.fl, *c = C->tt.fl;
            size_t d_step = D->step/sizeof(d[0]),
                   a_step = A->step/sizeof(a[0]),
                   b_step = B->step/sizeof(b[0]),
                   c_step = C->step/sizeof(c[0]);

            if(!c)
                c = zerof;

            switch(len)
            {
            case 2:
                if(len == d_size.width && b != d)
                {
                    for(i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step)
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                    }
                }
                else if(a != d)
                {
                    int c_step0 = 1;
                    if(c == zerof)
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for(i = 0; i < d_size.width; i++, d++, b++, c += c_step0)
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                    }
                }
                else
                    break;
                EXIT;
            case 3:
                if(len == d_size.width && b != d)
                {
                    for(i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step)
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        float t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                        d[2] = (float)(t2*alpha + c[2]*beta);
                    }
                }
                else if(a != d)
                {
                    int c_step0 = 1;
                    if(c == zerof)
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for(i = 0; i < d_size.width; i++, d++, b++, c += c_step0)
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        float t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (float)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                else
                    break;
                EXIT;
            case 4:
                if(len == d_size.width && b != d)
                {
                    for(i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step)
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        float t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        float t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                        d[2] = (float)(t2*alpha + c[2]*beta);
                        d[3] = (float)(t3*alpha + c[3]*beta);
                    }
                }
                else if(len <= 16 && a != d)
                {
                    int c_step0 = 1;
                    if(c == zerof)
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for(i = 0; i < d_size.width; i++, d++, b++, c += c_step0)
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        float t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        float t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (float)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (float)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                else
                    break;
                EXIT;
            }
        }
    }
 
    {
        int b_step = B->step;
        CvGEMMSingleMulFunc single_mul_func;
        img_t tmat[1], *D0 = D;
        icvBLAS_GEMM_32f_t blas_func = 0;

        if(!inittab)
        {
            icvInitGEMMTable(single_mul_tab, block_mul_tab, store_tab);
            inittab = 1;
        }

        single_mul_func = (CvGEMMSingleMulFunc)single_mul_tab[type][cn];
        if(!single_mul_func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(D->tt.data == A->tt.data || D->tt.data == B->tt.data)
        {
            int buf_size = d_size.width*d_size.height*CC_TYPE_SIZE(type);
            if(d_size.width <= CC_MAX_LOCAL_MAT_SIZE)
            {
                buffer = (uchar*)cvStackAlloc(buf_size);
                local_alloc = 1;
            }
            else
                CC_CALL(buffer = (uchar*)cAlloc(buf_size));

            cvMat(tmat, d_size.height, d_size.width, type, cn, buffer, CC_AUTO_STEP);
            D = tmat;
        }

        if((d_size.width == 1 || len == 1) && !(flags & CC_GEMM_B_T) && CC_IS_MAT_CONT(B))
        {
            b_step = d_size.width == 1 ? 0 : CC_TYPE_SIZE(type);
            flags |= CC_GEMM_B_T;
        }

        if(blas_func)
        {
            const char* transa = flags & CC_GEMM_A_T ? "t" : "n";
            const char* transb = flags & CC_GEMM_B_T ? "t" : "n";
            int lda, ldb, ldd;
            
            if(C->tt.data)
            {
                if(C->tt.data != D->tt.data)
                {
                    if(!(flags & CC_GEMM_C_T))
                        cvCopy(C, D);
                    else
                        cvTranspose(C, D);
                }
            }

            if(type == CC_32F)
            {
                CComplex32f _alpha, _beta;
                
                lda = A->step/sizeof(float);
                ldb = b_step/sizeof(float);
                ldd = D->step/sizeof(float);
                _alpha.re = (float)alpha;
                _alpha.im = 0;
                _beta.re = C->tt.data ? (float)beta : 0;
                _beta.im = 0;
                if(CC_MAT_CN(A) == 2)
                    lda /= 2, ldb /= 2, ldd /= 2;

                blas_func(transb, transa, &d_size.width, &d_size.height, &len,
                       &_alpha, B->tt.data, &ldb, A->tt.data, &lda,
                       &_beta, D->tt.data, &ldd);
            }
            else
            {
                CComplex64f _alpha, _beta;
                
                lda = A->step/sizeof(double);
                ldb = b_step/sizeof(double);
                ldd = D->step/sizeof(double);
                _alpha.re = alpha;
                _alpha.im = 0;
                _beta.re = C->tt.data ? beta : 0;
                _beta.im = 0;
                if(CC_MAT_CN(A) == 2)
                    lda /= 2, ldb /= 2, ldd /= 2;

                blas_func(transb, transa, &d_size.width, &d_size.height, &len,
                       &_alpha, B->tt.data, &ldb, A->tt.data, &lda,
                       &_beta, D->tt.data, &ldd);
            }
        }
        else if(d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2 || len <= 10 ||
            d_size.width <= block_lin_size && d_size.height <= block_lin_size && len <= block_lin_size)
        {
            single_mul_func(A->tt.data, A->step, B->tt.data, b_step,
                             C->tt.data, C->step, D->tt.data, D->step,
                             a_size, d_size, alpha, beta, flags);
        }
        else
        {
            int is_a_t = flags & CC_GEMM_A_T;
            int is_b_t = flags & CC_GEMM_B_T;
            int elem_size = CC_TYPE_SIZE(type);
            int dk0_1, dk0_2;
            int a_buf_size = 0, b_buf_size, d_buf_size;
            uchar* a_buf = 0;
            uchar* b_buf = 0;
            uchar* d_buf = 0;
            int i, j, k, di = 0, dj = 0, dk = 0;
            int dm0, dn0, dk0;
            int a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
            int work_elem_size = elem_size << (type == CC_32F ? 1 : 0);
            CvGEMMBlockMulFunc block_mul_func = (CvGEMMBlockMulFunc)block_mul_tab[type][cn];
            CvGEMMStoreFunc store_func = (CvGEMMStoreFunc)store_tab[type][cn];

            assert(block_mul_func && store_func);

            if(!is_a_t)
                a_step0 = A->step, a_step1 = elem_size;
            else
                a_step0 = elem_size, a_step1 = A->step;

            if(!is_b_t)
                b_step0 = b_step, b_step1 = elem_size;
            else
                b_step0 = elem_size, b_step1 = b_step;

            if(!C->tt.data)
            {
                c_step0 = c_step1 = 0;
                flags &= ~CC_GEMM_C_T;
            }
            else if(!(flags & CC_GEMM_C_T))
                c_step0 = C->step, c_step1 = elem_size;
            else
                c_step0 = elem_size, c_step1 = C->step;

            dm0 = MIN(block_lin_size, d_size.height);
            dn0 = MIN(block_lin_size, d_size.width);
            dk0_1 = block_size / dm0;
            dk0_2 = block_size / dn0;
            dk0 = MAX(dk0_1, dk0_2);
            dk0 = MIN(dk0, len);
            if(dk0*dm0 > block_size)
                dm0 = block_size / dk0;
            if(dk0*dn0 > block_size)
                dn0 = block_size / dk0;

            dk0_1 = (dn0+dn0/8+2) & -2;
            b_buf_size = (dk0+dk0/8+1)*dk0_1*elem_size;
            d_buf_size = (dk0+dk0/8+1)*dk0_1*work_elem_size;
        
            if(is_a_t)
            {
                a_buf_size = (dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
                flags &= ~CC_GEMM_A_T;
            }

            CC_CALL(block_buffer = (uchar*)cAlloc(a_buf_size + b_buf_size + d_buf_size));
            d_buf = block_buffer;
            b_buf = d_buf + d_buf_size;

            if(is_a_t)
                a_buf = b_buf + b_buf_size;

            for(i = 0; i < d_size.height; i += di)
            {
                di = dm0;
                if(i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height)
                    di = d_size.height - i;

                for(j = 0; j < d_size.width; j += dj)
                {
                    uchar* _d = D->tt.data + i*D->step + j*elem_size;
                    const uchar* _c = C->tt.data + i*c_step0 + j*c_step1;
                    int _d_step = D->step;
                    dj = dn0;

                    if(j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width)
                        dj = d_size.width - j;

                    flags &= 15;
                    if(dk0 < len)
                    {
                        _d = d_buf;
                        _d_step = dj*work_elem_size;
                    }

                    for(k = 0; k < len; k += dk)
                    {
                        const uchar* _a = A->tt.data + i*a_step0 + k*a_step1;
                        int _a_step = A->step;
                        const uchar* _b = B->tt.data + k*b_step0 + j*b_step1;
                        int _b_step = b_step;
                        CSize a_bl_size;

                        dk = dk0;
                        if(k + dk >= len || 8*(k + dk) + dk > 8*len)
                            dk = len - k;

                        if(!is_a_t)
                            a_bl_size.width = dk, a_bl_size.height = di;
                        else
                            a_bl_size.width = di, a_bl_size.height = dk;

                        if(a_buf && is_a_t)
                        {
                            int t;
                            _a_step = dk*elem_size;
                            icvGEMM_TransposeBlock(_a, A->step, a_buf, _a_step, a_bl_size, elem_size);
                            CC_SWAP(a_bl_size.width, a_bl_size.height, t);
                            _a = a_buf;
                        }
                
                        if(dj < d_size.width)
                        {
                            CSize b_size;
                            if(!is_b_t)
                                b_size.width = dj, b_size.height = dk;
                            else
                                b_size.width = dk, b_size.height = dj;

                            _b_step = b_size.width*elem_size;
                            icvGEMM_CopyBlock(_b, b_step, b_buf, _b_step, b_size, elem_size);
                            _b = b_buf;
                        }

                        if(dk0 < len)
                            block_mul_func(_a, _a_step, _b, _b_step, _d, _d_step,
                                            a_bl_size, cSize(dj,di), flags);
                        else
                            single_mul_func(_a, _a_step, _b, _b_step, _c, C->step, _d, _d_step,
                                             a_bl_size, cSize(dj,di), alpha, beta, flags);
                        flags |= 16;
                    }

                    if(dk0 < len)
                        store_func(_c, C->step, _d, _d_step, D->tt.data + i*D->step + j*elem_size,
                                    D->step, cSize(dj,di), alpha, beta, flags);
                }
            }
        }

        if(D0 != D)
            CC_CALL(cvCopy(D, D0));
    }

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);
    if(block_buffer)
        cFree(&block_buffer);
    if (Darr0==Darr) {
       cvCloneMat(Darr0, Darr);
    }
    imfree(tmp_d);
	return Darr;
}


/****************************************************************************************\
*                                        cvTransform                                     *
\****************************************************************************************/

#define  ICC_DEF_TRANSFORM_CASE_C1(arrtype, temptype, _ld_,        \
                                   _cast_macro1_, _cast_macro2_)   \
{                                                                   \
    for(i = 0; i < size.width; i++, dst += dst_cn)                \
    {                                                               \
        const double* _mat = mat;                                   \
        double v0 = _ld_(src[i]);                                   \
        for(k = 0; k < dst_cn; k++, _mat += 2)                    \
        {                                                           \
            temptype t0 = _cast_macro1_(_mat[0]*v0 + _mat[1]);      \
            dst[k] = _cast_macro2_(t0);                             \
        }                                                           \
    }                                                               \
    src += size.width;                                              \
}


#define  ICC_DEF_DIAG_TRANSFORM_CASE_C1(arrtype, temptype, _ld_,   \
                                  _cast_macro1_, _cast_macro2_)    \
    for(i = 0; i < size.width; i++)                               \
    {                                                               \
        double ft0;                                                 \
        temptype t0;                                                \
        ft0 = mat[0]*_ld_(src[i]) + mat[1];                         \
        t0 = _cast_macro1_(ft0);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
    }


#define  ICC_DEF_TRANSFORM_CASE_C2(arrtype, temptype, _ld_,        \
                                  _cast_macro1_, _cast_macro2_)    \
if(dst_cn == 2)                                                   \
{                                                                   \
    for(i = 0; i < size.width*2; i += 2)                          \
    {                                                               \
        double ft0, ft1;                                            \
        temptype t0, t1;                                            \
        ft0 = mat[0]*_ld_(src[i]) + mat[1]*_ld_(src[i+1]) + mat[2]; \
        ft1 = mat[3]*_ld_(src[i]) + mat[4]*_ld_(src[i+1]) + mat[5]; \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
    }                                                               \
    src += size.width*2; dst += size.width*2;                       \
}                                                                   \
else                                                                \
    for(i = 0; i < size.width; i++, src += 2, dst += dst_cn)      \
    {                                                               \
        const double* _mat = mat;                                   \
        double v0 = _ld_(src[0]), v1 = src[1];                      \
        for(k = 0; k < dst_cn; k++, _mat += 3)                    \
        {                                                           \
            temptype t0 =                                           \
                _cast_macro1_(_mat[0]*v0 + _mat[1]*v1 + _mat[2]);   \
            dst[k] = _cast_macro2_(t0);                             \
        }                                                           \
    }


#define  ICC_DEF_DIAG_TRANSFORM_CASE_C2(arrtype, temptype, _ld_,   \
                                  _cast_macro1_, _cast_macro2_)    \
    for(i = 0; i < size.width*2; i += 2)                          \
    {                                                               \
        double ft0, ft1;                                            \
        temptype t0, t1;                                            \
        ft0 = mat[0]*_ld_(src[i]) + mat[2];                         \
        ft1 = mat[4]*_ld_(src[i+1]) + mat[5];                       \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
    }


#define  ICC_DEF_TRANSFORM_CASE_C3(arrtype, temptype, _ld_,        \
                                  _cast_macro1_, _cast_macro2_)    \
if(dst_cn == 3)                                                   \
{                                                                   \
    for(i = 0; i < size.width*3; i += 3)                          \
    {                                                               \
        double ft0, ft1, ft2;                                       \
        temptype t0, t1, t2;                                        \
        ft0 = mat[0]*_ld_(src[i]) + mat[1]*_ld_(src[i+1]) +         \
              mat[2]*_ld_(src[i+2]) + mat[3];                       \
        ft1 = mat[4]*_ld_(src[i]) + mat[5]*_ld_(src[i+1]) +         \
              mat[6]*_ld_(src[i+2]) + mat[7];                       \
        ft2 = mat[8]*_ld_(src[i]) + mat[9]*_ld_(src[i+1]) +         \
              mat[10]*_ld_(src[i+2]) + mat[11];                     \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        t2 = _cast_macro1_(ft2);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
        dst[i+2] = _cast_macro2_(t2);                               \
    }                                                               \
    src += size.width*3; dst += size.width*3;                       \
}                                                                   \
else if(dst_cn == 1)                                              \
{                                                                   \
    for(i = 0; i < size.width; i++, src += 3)                     \
    {                                                               \
        temptype t0 = _cast_macro1_(mat[0]*_ld_(src[0]) +           \
            mat[1]*_ld_(src[1]) + mat[2]*_ld_(src[2]) + mat[3]);    \
        dst[i] = _cast_macro2_(t0);                                 \
    }                                                               \
    dst += size.width;                                              \
}                                                                   \
else                                                                \
    for(i = 0; i < size.width; i++, src += 3, dst += dst_cn)      \
    {                                                               \
        const double* _mat = mat;                                   \
        double v0=_ld_(src[0]), v1=_ld_(src[1]), v2=_ld_(src[2]);   \
        for(k = 0; k < dst_cn; k++, _mat += 4)                    \
        {                                                           \
            temptype t0 = _cast_macro1_(_mat[0]*v0 +                \
                    _mat[1]*v1 + _mat[2]*v2 + _mat[3]);             \
            dst[k] = _cast_macro2_(t0);                             \
        }                                                           \
    }


#define  ICC_DEF_DIAG_TRANSFORM_CASE_C3(arrtype, temptype, _ld_,   \
                                  _cast_macro1_, _cast_macro2_)    \
    for(i = 0; i < size.width*3; i += 3)                          \
    {                                                               \
        double ft0, ft1, ft2;                                       \
        temptype t0, t1, t2;                                        \
        ft0 = mat[0]*_ld_(src[i]) + mat[3];                         \
        ft1 = mat[5]*_ld_(src[i+1]) + mat[7];                       \
        ft2 = mat[10]*_ld_(src[i+2]) + mat[11];                     \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        t2 = _cast_macro1_(ft2);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
        dst[i+2] = _cast_macro2_(t2);                               \
    }


#define  ICC_DEF_TRANSFORM_CASE_C4(arrtype, temptype, _ld_,        \
                                  _cast_macro1_, _cast_macro2_)    \
for(i = 0; i < size.width; i++, src += 4, dst += dst_cn)          \
{                                                                   \
    const double* _mat = mat;                                       \
    double v0 = _ld_(src[0]), v1 = _ld_(src[1]),                    \
           v2 = _ld_(src[2]), v3 = _ld_(src[3]);                    \
    for(k = 0; k < dst_cn; k++, _mat += 5)                        \
    {                                                               \
        temptype t0 =_cast_macro1_(_mat[0]*v0+_mat[1]*v1+           \
                                   _mat[2]*v2+_mat[3]*v3+_mat[4]);  \
        dst[k] = _cast_macro2_(t0);                                 \
    }                                                               \
}


#define  ICC_DEF_DIAG_TRANSFORM_CASE_C4(arrtype, temptype, _ld_,   \
                                  _cast_macro1_, _cast_macro2_)    \
    for(i = 0; i < size.width*4; i += 4)                          \
    {                                                               \
        double ft0, ft1;                                            \
        temptype t0, t1;                                            \
        ft0 = mat[0]*_ld_(src[i]) + mat[4];                         \
        ft1 = mat[6]*_ld_(src[i+1]) + mat[9];                       \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        dst[i] = _cast_macro2_(t0);                                 \
        dst[i+1] = _cast_macro2_(t1);                               \
        ft0 = mat[12]*_ld_(src[i+2]) + mat[14];                     \
        ft1 = mat[18]*_ld_(src[i+3]) + mat[19];                     \
        t0 = _cast_macro1_(ft0);                                    \
        t1 = _cast_macro1_(ft1);                                    \
        dst[i+2] = _cast_macro2_(t0);                               \
        dst[i+3] = _cast_macro2_(t1);                               \
    }



#define  ICC_DEF_TRANSFORM_FUNC(flavor, arrtype, temptype, _ld_,   \
                                 _cast_macro1_, _cast_macro2_, cn)\
static CStatus C_STDCALL                                          \
icvTransform_##flavor(const arrtype* src, int srcstep,             \
                       arrtype* dst, int dststep, CSize size,      \
                       const double* mat, int dst_cn)              \
{                                                                   \
    srcstep = srcstep/sizeof(src[0]) - size.width*cn;               \
    dststep = dststep/sizeof(dst[0]) - size.width*dst_cn;           \
    for(; size.height--; src += srcstep, dst += dststep)          \
    {                                                               \
        int i, k;                                                   \
        ICC_DEF_TRANSFORM_CASE_C##cn(arrtype, temptype, _ld_,      \
                                     _cast_macro1_, _cast_macro2_) \
    }                                                               \
                                                                    \
    return CC_OK;                                                   \
}


#define  ICC_DEF_DIAG_TRANSFORM_FUNC(flavor, arrtype, temptype, _ld_, \
                                 _cast_macro1_, _cast_macro2_, cn)\
static CStatus C_STDCALL                                          \
icvDiagTransform_##flavor(const arrtype* src, int srcstep,         \
                       arrtype* dst, int dststep, CSize size,      \
                       const double* mat)                          \
{                                                                   \
    srcstep /= sizeof(src[0]);                                      \
    dststep /= sizeof(dst[0]);                                      \
    for(; size.height--; src += srcstep, dst += dststep)          \
    {                                                               \
        int i;                                                      \
        ICC_DEF_DIAG_TRANSFORM_CASE_C##cn(arrtype, temptype, _ld_, \
                                     _cast_macro1_, _cast_macro2_) \
    }                                                               \
                                                                    \
    return CC_OK;                                                   \
}


ICC_DEF_TRANSFORM_FUNC(8u_C1R, uchar, int, CC_8TO32F, cRound, CC_CAST_8U, 1)
ICC_DEF_TRANSFORM_FUNC(8u_C2R, uchar, int, CC_8TO32F, cRound, CC_CAST_8U, 2)
ICC_DEF_TRANSFORM_FUNC(8u_C3R, uchar, int, CC_8TO32F, cRound, CC_CAST_8U, 3)
ICC_DEF_TRANSFORM_FUNC(8u_C4R, uchar, int, CC_8TO32F, cRound, CC_CAST_8U, 4)

ICC_DEF_TRANSFORM_FUNC(16u_C1R, ushort, int, CC_NOP, cRound, CC_CAST_16U, 1)
ICC_DEF_TRANSFORM_FUNC(16u_C2R, ushort, int, CC_NOP, cRound, CC_CAST_16U, 2)
ICC_DEF_TRANSFORM_FUNC(16u_C3R, ushort, int, CC_NOP, cRound, CC_CAST_16U, 3)
ICC_DEF_TRANSFORM_FUNC(16u_C4R, ushort, int, CC_NOP, cRound, CC_CAST_16U, 4)

ICC_DEF_TRANSFORM_FUNC(16s_C1R, short, int, CC_NOP, cRound, CC_CAST_16S, 1)
ICC_DEF_TRANSFORM_FUNC(16s_C2R, short, int, CC_NOP, cRound, CC_CAST_16S, 2)
ICC_DEF_TRANSFORM_FUNC(16s_C3R, short, int, CC_NOP, cRound, CC_CAST_16S, 3)
ICC_DEF_TRANSFORM_FUNC(16s_C4R, short, int, CC_NOP, cRound, CC_CAST_16S, 4)

ICC_DEF_TRANSFORM_FUNC(32s_C1R, int, int, CC_NOP, cRound, CC_NOP, 1)
ICC_DEF_TRANSFORM_FUNC(32s_C2R, int, int, CC_NOP, cRound, CC_NOP, 2)
ICC_DEF_TRANSFORM_FUNC(32s_C3R, int, int, CC_NOP, cRound, CC_NOP, 3)
ICC_DEF_TRANSFORM_FUNC(32s_C4R, int, int, CC_NOP, cRound, CC_NOP, 4)

ICC_DEF_TRANSFORM_FUNC(32f_C1R, float, double, CC_NOP, CC_NOP, CC_CAST_32F, 1)
ICC_DEF_TRANSFORM_FUNC(32f_C2R, float, double, CC_NOP, CC_NOP, CC_CAST_32F, 2)
ICC_DEF_TRANSFORM_FUNC(32f_C3R, float, double, CC_NOP, CC_NOP, CC_CAST_32F, 3)
ICC_DEF_TRANSFORM_FUNC(32f_C4R, float, double, CC_NOP, CC_NOP, CC_CAST_32F, 4)

ICC_DEF_TRANSFORM_FUNC(64f_C1R, double, double, CC_NOP, CC_NOP, CC_CAST_64F, 1)
ICC_DEF_TRANSFORM_FUNC(64f_C2R, double, double, CC_NOP, CC_NOP, CC_CAST_64F, 2)
ICC_DEF_TRANSFORM_FUNC(64f_C3R, double, double, CC_NOP, CC_NOP, CC_CAST_64F, 3)
ICC_DEF_TRANSFORM_FUNC(64f_C4R, double, double, CC_NOP, CC_NOP, CC_CAST_64F, 4)

ICC_DEF_DIAG_TRANSFORM_FUNC(16u_C1R, ushort, int, CC_NOP, cRound, CC_CAST_16U, 1)
ICC_DEF_DIAG_TRANSFORM_FUNC(16u_C2R, ushort, int, CC_NOP, cRound, CC_CAST_16U, 2)
ICC_DEF_DIAG_TRANSFORM_FUNC(16u_C3R, ushort, int, CC_NOP, cRound, CC_CAST_16U, 3)
ICC_DEF_DIAG_TRANSFORM_FUNC(16u_C4R, ushort, int, CC_NOP, cRound, CC_CAST_16U, 4)

ICC_DEF_DIAG_TRANSFORM_FUNC(16s_C1R, short, int, CC_NOP, cRound, CC_CAST_16S, 1)
ICC_DEF_DIAG_TRANSFORM_FUNC(16s_C2R, short, int, CC_NOP, cRound, CC_CAST_16S, 2)
ICC_DEF_DIAG_TRANSFORM_FUNC(16s_C3R, short, int, CC_NOP, cRound, CC_CAST_16S, 3)
ICC_DEF_DIAG_TRANSFORM_FUNC(16s_C4R, short, int, CC_NOP, cRound, CC_CAST_16S, 4)

ICC_DEF_DIAG_TRANSFORM_FUNC(32s_C1R, int, int, CC_NOP, cRound, CC_NOP, 1)
ICC_DEF_DIAG_TRANSFORM_FUNC(32s_C2R, int, int, CC_NOP, cRound, CC_NOP, 2)
ICC_DEF_DIAG_TRANSFORM_FUNC(32s_C3R, int, int, CC_NOP, cRound, CC_NOP, 3)
ICC_DEF_DIAG_TRANSFORM_FUNC(32s_C4R, int, int, CC_NOP, cRound, CC_NOP, 4)

ICC_DEF_DIAG_TRANSFORM_FUNC(32f_C1R, float, double, CC_NOP, CC_NOP, CC_CAST_32F, 1)
ICC_DEF_DIAG_TRANSFORM_FUNC(32f_C2R, float, double, CC_NOP, CC_NOP, CC_CAST_32F, 2)
ICC_DEF_DIAG_TRANSFORM_FUNC(32f_C3R, float, double, CC_NOP, CC_NOP, CC_CAST_32F, 3)
ICC_DEF_DIAG_TRANSFORM_FUNC(32f_C4R, float, double, CC_NOP, CC_NOP, CC_CAST_32F, 4)

ICC_DEF_DIAG_TRANSFORM_FUNC(64f_C1R, double, double, CC_NOP, CC_NOP, CC_CAST_64F, 1)
ICC_DEF_DIAG_TRANSFORM_FUNC(64f_C2R, double, double, CC_NOP, CC_NOP, CC_CAST_64F, 2)
ICC_DEF_DIAG_TRANSFORM_FUNC(64f_C3R, double, double, CC_NOP, CC_NOP, CC_CAST_64F, 3)
ICC_DEF_DIAG_TRANSFORM_FUNC(64f_C4R, double, double, CC_NOP, CC_NOP, CC_CAST_64F, 4)

#define icvTransform_8s_C1R 0
#define icvTransform_8s_C2R 0
#define icvTransform_8s_C3R 0
#define icvTransform_8s_C4R 0

#define icvDiagTransform_8s_C1R 0
#define icvDiagTransform_8s_C2R 0
#define icvDiagTransform_8s_C3R 0
#define icvDiagTransform_8s_C4R 0

#define icvDiagTransform_8u_C1R 0
#define icvDiagTransform_8u_C2R 0
#define icvDiagTransform_8u_C3R 0
#define icvDiagTransform_8u_C4R 0

CC_DEF_INIT_BIG_FUNC_TAB_2D(Transform, R)
CC_DEF_INIT_BIG_FUNC_TAB_2D(DiagTransform, R)

typedef CStatus (C_STDCALL * CvTransformFunc)(
                       const void* src, int srcstep,
                       void* dst, int dststep, CSize size,
                       const void* mat, int dst_cn);

typedef CStatus (C_STDCALL * CvDiagTransformFunc)(
                       const void* src, int srcstep,
                       void* dst, int dststep, CSize size,
                       const void* mat);

typedef CStatus (C_STDCALL * CvDiagTransformFunc)(
                       const void* src, int srcstep,
                       void* dst, int dststep, CSize size,
                       const void* mat);

///////////////////// IPP transform functions //////////////////

typedef CStatus (C_STDCALL * CvColorTwistIPPFunc)(const void* src, int srcstep,
                        void* dst, int dststep, CSize size, const float* coeffs);


////////////////////////////////////////////////////////////////

CC_IMPL void
cvTransform(const img_t* src, img_t* dst,
             const img_t* rot, const img_t* shift)
{
    static CvBigFuncTable transform_tab, diag_transform_tab;
    static int inittab = 0;
    img_t lut[1] = {0};
    
    CC_FUNCNAME("cvTransform");

    __BEGIN__;

    TypeId type;
    int i, j, cn, dst_cn;
    int coi = 0, coi2 = 0;
    double* buffer = (double*)cvStackAlloc(CC_CN_MAX*(CC_CN_MAX+1)*sizeof(buffer[0]));

    if(!inittab)
    {
        icvInitTransformRTable(transform_tab);
        icvInitDiagTransformRTable(diag_transform_tab);
        inittab = 1;
    }

    if(coi != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    if(!CC_ARE_DEPTHS_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    type = CC_MAT_DEPTH(src);
    cn = CC_MAT_CN(src);
    dst_cn = CC_MAT_CN(dst);

    if(cn > 4 || dst_cn > 4)
        CC_ERROR(CC_StsOutOfRange, "Both input and output array must have at most 4 channels");

    if(rot->rows != dst_cn)
        CC_ERROR(CC_StsBadSize,
        "The height of transmat matrix must be equal to number of channels");

    if(rot->cols == cn + 1 || rot->cols == cn)
    {
        if(CC_MAT_TYPE(rot) == CC_64F)
        {
            for(i = 0; i < dst_cn; i++)
            {
                buffer[i*(cn+1) + cn] = 0;
                for(j = 0; j < rot->cols; j++)
                    buffer[i*(cn+1) + j] = ((double*)(rot->tt.data + rot->step*i))[j];
            }
        }
        else if(CC_MAT_TYPE(rot) == CC_32F)
        {
            for(i = 0; i < dst_cn; i++)
            {
                buffer[i*(cn+1) + cn] = 0;
                for(j = 0; j < rot->cols; j++)
                    buffer[i*(cn+1) + j] = ((float*)(rot->tt.data + rot->step*i))[j];
            }
        }
        else
            CC_ERROR(CC_StsUnsupportedFormat, "Rotation matrix must be 32fC1 or 64fC1");
    }
    else
        CC_ERROR(CC_StsUnmatchedSizes, "If the source array has <cn> channels, "
           "the transformation matrix must have <cn> x <cn>+1 or <cn> x <cn> size");

    if(shift)
    {
        if(CC_MAT_CN(shift) * shift->cols * shift->rows == dst_cn &&
            (shift->rows == 1 || shift->rows == dst_cn) ||
            (shift->cols == 1 || shift->cols == dst_cn))
        {
            if(CC_MAT_TYPE(shift) == CC_64F)
            {
                int step = shift->step ? shift->step/sizeof(double) : 1;
                for(i = 0; i < dst_cn; i++)
                    buffer[i*(cn+1) + cn] += shift->tt.db[i*step];
            }
            else if(CC_MAT_TYPE(shift) == CC_32F)
            {
                int step = shift->step ? shift->step/sizeof(float) : 1;
                for(i = 0; i < dst_cn; i++)
                    buffer[i*(cn+1) + cn] += shift->tt.fl[i*step];
            }
            else
                CC_ERROR(CC_StsUnsupportedFormat, "Shift vector must be 32f or 64f");
        }
        else
        {
            CC_ERROR(CC_StsUnmatchedSizes,
                "Shift (if present) must be 1 dimensional vector with the number "
                "of elements equal to number of channels in the processed array");
        }
    }

    if(coi != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    {
        CvTransformFunc func = (CvTransformFunc)(transform_tab[type][cn]);
        CvDiagTransformFunc diag_func = 0;
        CvLUT_TransformFunc lut_func = 0;
        int diag_transform = 0;
        CSize size;
        float* ipp_coeffs = (float*)cvStackAlloc(16*sizeof(ipp_coeffs[0]));

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(dst_cn == cn)
        {
            diag_transform = 1;
            for(i = 0; i < dst_cn; i++)
                for(j = 0; j < cn; j++)
                {
                    if(i != j && fabs(buffer[i*(cn+1) + j]) > DBL_EPSILON)
                    {
                        diag_transform = 0;
                        break;
                    }
                }
            
            if(diag_transform)
            {
                if(type == CC_8U)
                {
                    CC_CALL(cvCreateMat(lut, 1, 256, type, cn));
                    for(i = 0; i < cn; i++)
                    {
                        double a = buffer[i*(cn+1) + i], b = buffer[i*(cn+1) + cn];
                        uchar* ltab = lut->tt.data;
                        for(j = 0; j < 256; j++)
                        {
                            int t = cRound(a*j + b);
                            ltab[j*cn + i] = CC_CAST_8U(t);
                        }
                    }
                    lut_func = cn == 1 ? (CvLUT_TransformFunc)icvLUT_Transform8u_8u_C1R :
                               cn == 2 ? (CvLUT_TransformFunc)icvLUT_Transform8u_8u_C2R :
                               cn == 3 ? (CvLUT_TransformFunc)icvLUT_Transform8u_8u_C3R :
                               (CvLUT_TransformFunc)icvLUT_Transform8u_8u_C4R;
                }
                else
                    diag_func = (CvDiagTransformFunc)(diag_transform_tab[type][cn]);
            }
        }

        {
            int srcstep = src->step;
            int dststep = dst->step;
            size = cvGetMatSize(src);
            
            if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
            {
                size.width *= size.height;
                size.height = 1;
                srcstep = dststep = CC_STUB_STEP;
            }
            
            if(lut_func) {
                lut_func(src->tt.data, src->step, dst->tt.data,
                          dst->step, size, lut->tt.data);
            } else if(diag_transform) {
                diag_func(src->tt.data, src->step, dst->tt.data,
                           dst->step, size, buffer);
            } else {
                func(src->tt.data, src->step, dst->tt.data,
                      dst->step, size, buffer, dst_cn);
            }
        }
    }

    __END__;

    imfree(lut);
}


/****************************************************************************************\
*                                        cvPerspectiveTransform                          *
\****************************************************************************************/

#define ICC_PERSPECTIVE_TRANSFORM_FUNC_2(flavor, arrtype)                               \
static CStatus C_STDCALL                                                              \
icvPerspectiveTransform_##flavor##_C2R(const arrtype* src, int srcstep,                 \
                                        arrtype* dst, int dststep,                      \
                                        CSize size, const double* mat)                 \
{                                                                                       \
    int i;                                                                              \
    size.width *= 2;                                                                    \
    srcstep /= sizeof(src[0]); dststep /= sizeof(dst[0]);                               \
                                                                                        \
    for(; size.height--; src += srcstep, dst += dststep)                                \
    {                                                                                   \
        for(i = 0; i < size.width; i += 2)                                              \
        {                                                                               \
            arrtype x = src[i], y = src[i + 1];                                         \
            double w = x*mat[6] + y*mat[7] + mat[8];                                    \
                                                                                        \
            if(fabs(w) > FLT_EPSILON)                                                   \
            {                                                                           \
                w = 1./w;                                                               \
                dst[i] = (arrtype)((x*mat[0] + y*mat[1] + mat[2]) * w);                 \
                dst[i+1] = (arrtype)((x*mat[3] + y*mat[4] + mat[5]) * w);               \
            }                                                                           \
            else                                                                        \
            {                                                                           \
                dst[i] = (arrtype)0;                                                    \
                dst[i+1] = (arrtype)0;                                                  \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    return CC_OK;                                                                       \
}


#define ICC_PERSPECTIVE_TRANSFORM_FUNC_3(flavor, arrtype)                               \
static CStatus C_STDCALL                                                              \
icvPerspectiveTransform_##flavor##_C3R(const arrtype* src, int srcstep,                 \
                                             arrtype* dst, int dststep,                 \
                                             CSize size, const double* mat)            \
{                                                                                       \
    int i;                                                                              \
    size.width *= 3;                                                                    \
    srcstep /= sizeof(src[0]); dststep /= sizeof(dst[0]);                               \
                                                                                        \
    for(; size.height--; src += srcstep, dst += dststep)                                \
    {                                                                                   \
        for(i = 0; i < size.width; i += 3)                                              \
        {                                                                               \
            arrtype x = src[i], y = src[i + 1], z = src[i + 2];                         \
            double w = x*mat[12] + y*mat[13] + z*mat[14] + mat[15];                     \
                                                                                        \
            if(fabs(w) > FLT_EPSILON)                                                   \
            {                                                                           \
                w = 1./w;                                                               \
                dst[i] = (arrtype)((x*mat[0] + y*mat[1] + z*mat[2] + mat[3]) * w);      \
                dst[i+1] = (arrtype)((x*mat[4] + y*mat[5] + z*mat[6] + mat[7]) * w);    \
                dst[i+2] = (arrtype)((x*mat[8] + y*mat[9] + z*mat[10] + mat[11]) * w);  \
            }                                                                           \
            else                                                                        \
            {                                                                           \
                dst[i] = (arrtype)0;                                                    \
                dst[i+1] = (arrtype)0;                                                  \
                dst[i+2] = (arrtype)0;                                                  \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    return CC_OK;                                                                       \
}

ICC_PERSPECTIVE_TRANSFORM_FUNC_2(32f, float)
ICC_PERSPECTIVE_TRANSFORM_FUNC_2(64f, double)
ICC_PERSPECTIVE_TRANSFORM_FUNC_3(32f, float)
ICC_PERSPECTIVE_TRANSFORM_FUNC_3(64f, double)

static void icvInitPerspectiveTransformTable(CvFuncTable  tab2, CvFuncTable  tab3)\
{                                                                                   \
    tab2[CC_32F] = (void*)icvPerspectiveTransform_32f_C2R;                   \
    tab2[CC_64F] = (void*)icvPerspectiveTransform_64f_C2R;                   \
    tab3[CC_32F] = (void*)icvPerspectiveTransform_32f_C3R;                   \
    tab3[CC_64F] = (void*)icvPerspectiveTransform_64f_C3R;                   \
}


CC_IMPL void
cvPerspectiveTransform(const img_t* src, img_t* dst, const img_t* mat)
{
    static CvFuncTable tab[2];
    static int inittab = 0;
    double buffer[16];

    CC_FUNCNAME("cvPerspectiveProject");

    __BEGIN__;

    int i, j, cn;
    TypeId type;
    CvFunc2D_2A1P func = 0;
    CSize size;

    if(!inittab)
    {
        icvInitPerspectiveTransformTable(tab[0], tab[1]);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    type = CC_MAT_DEPTH(src);
    cn = CC_MAT_CN(src);

    if(cn != 2 && cn != 3)
        CC_ERROR(CC_BadNumChannels, cvUnsupportedFormat);

    if(mat->rows != cn + 1 && mat->cols != mat->rows)
        CC_ERROR(CC_StsBadSize,
        "The size of transform matrix must be equal to number of channels");

    if(CC_MAT_TYPE(mat) == CC_64F && cn==1)
    {
        for(i = 0; i <= cn; i++)
        {
            for(j = 0; j <= cn; j++)
                buffer[i*(cn+1) + j] = ((double*)(mat->tt.data + mat->step*i))[j];
        }
    }
    else if(CC_MAT_TYPE(mat) == CC_32F && cn==1)
    {
        for(i = 0; i <= cn; i++)
        {
            for(j = 0; j <= cn; j++)
                buffer[i*(cn+1) + j] = ((float*)(mat->tt.data + mat->step*i))[j];
        }
    }
    else
    {
        CC_ERROR(CC_StsUnsupportedFormat, "Rotation matrix must be 32fC1 or 64fC1");
    }

    func = (CvFunc2D_2A1P)tab[cn == 3][type];

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    size = cvGetMatSize(src);

    if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        size.height = 1;
    }

    IPPI_CALL(func(src->tt.data, src->step, dst->tt.data, dst->step, size, buffer));

    CC_CHECK_NANS(dst);

    __END__;
}


/****************************************************************************************\
*                                       cvScaleAdd                                       *
\****************************************************************************************/

#define  ICC_DEF_MULC_CASE_C1(arrtype, temptype, src1, dst, len)                \
{                                                                               \
    int i;                                                                      \
                                                                                \
    for(i = 0; i <= (len) - 4; i += 4)                                          \
    {                                                                           \
        temptype t0 = (src1)[i]*s0;                                             \
        temptype t1 = (src1)[i+1]*s0;                                           \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
                                                                                \
        t0 = (src1)[i+2]*s0;                                                    \
        t1 = (src1)[i+3]*s0;                                                    \
                                                                                \
        (dst)[i+2] = (arrtype)t0;                                               \
        (dst)[i+3] = (arrtype)t1;                                               \
    }                                                                           \
                                                                                \
    for(; i < (len); i++)                                                       \
    {                                                                           \
        temptype t0 = (src1)[i]*s0;                                             \
        (dst)[i] = (arrtype)t0;                                                 \
    }                                                                           \
}


#define  ICC_DEF_MULC_CASE_C2(arrtype, temptype, src1, dst, len)                \
{                                                                               \
    int i;                                                                      \
                                                                                \
    for(i = 0; i <= (len) - 4; i += 4)                                          \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 - (src1)[i+1]*s1;                            \
        temptype t1 = (src1)[i]*s1 + (src1)[i+1]*s0;                            \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
                                                                                \
        t0 = (src1)[i+2]*s0 - (src1)[i+3]*s1;                                   \
        t1 = (src1)[i+2]*s1 + (src1)[i+3]*s0;                                   \
                                                                                \
        (dst)[i+2] = (arrtype)t0;                                               \
        (dst)[i+3] = (arrtype)t1;                                               \
    }                                                                           \
                                                                                \
    for(; i < (len); i += 2)                                                    \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 - (src1)[i+1]*s1;                            \
        temptype t1 = (src1)[i]*s1 + (src1)[i+1]*s0;                            \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
    }                                                                           \
}


#define  ICC_DEF_MULADDC_CASE_C1(arrtype, temptype, src1, src2, dst, len)       \
{                                                                               \
    int i;                                                                      \
                                                                                \
    for(i = 0; i <= (len) - 4; i += 4)                                          \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 + (src2)[i];                                 \
        temptype t1 = (src1)[i+1]*s0 + (src2)[i+1];                             \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
                                                                                \
        t0 = (src1)[i+2]*s0 + (src2)[i+2];                                      \
        t1 = (src1)[i+3]*s0 + (src2)[i+3];                                      \
                                                                                \
        (dst)[i+2] = (arrtype)t0;                                               \
        (dst)[i+3] = (arrtype)t1;                                               \
    }                                                                           \
                                                                                \
    for(; i < (len); i++)                                                       \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 + (src2)[i];                                 \
        (dst)[i] = (arrtype)t0;                                                 \
    }                                                                           \
}


#define  ICC_DEF_MULADDC_CASE_C2(arrtype, temptype, src1, src2, dst, len)       \
{                                                                               \
    int i;                                                                      \
                                                                                \
    for(i = 0; i <= (len) - 4; i += 4)                                          \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 - (src1)[i+1]*s1 + (src2)[i];                \
        temptype t1 = (src1)[i]*s1 + (src1)[i+1]*s0 + (src2)[i+1];              \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
                                                                                \
        t0 = (src1)[i+2]*s0 - (src1)[i+3]*s1 + (src2)[i+2];                     \
        t1 = (src1)[i+2]*s1 + (src1)[i+3]*s0 + (src2)[i+3];                     \
                                                                                \
        (dst)[i+2] = (arrtype)t0;                                               \
        (dst)[i+3] = (arrtype)t1;                                               \
    }                                                                           \
                                                                                \
    for(; i < (len); i += 2)                                                    \
    {                                                                           \
        temptype t0 = (src1)[i]*s0 - (src1)[i+1]*s1 + (src2)[i];                \
        temptype t1 = (src1)[i]*s1 + (src1)[i+1]*s0 + (src2)[i+1];              \
                                                                                \
        (dst)[i] = (arrtype)t0;                                                 \
        (dst)[i+1] = (arrtype)t1;                                               \
    }                                                                           \
}


#define  ICC_DEF_MULADDS_FUNC(flavor, arrtype, scalartype, entry, cn)       \
static CStatus C_STDCALL                                                  \
icvMulAddC_##flavor(const arrtype* src1, int srcstep1,                      \
                      const arrtype* src2, int srcstep2,                    \
                      arrtype* dst, int dststep, CSize size,               \
                      const scalartype* scalar)                             \
{                                                                           \
    entry(scalartype);                                                      \
    size.width *= (cn);                                                     \
    srcstep1 /= sizeof(src1[0]);                                            \
    dststep /= sizeof(dst[0]);                                              \
                                                                            \
    if (src2) {                                                             \
        srcstep2 /= sizeof(src2[0]);                                        \
        for(; size.height--; src1+=srcstep1, src2+=srcstep2, dst+=dststep)  \
        {                                                                   \
            ICC_DEF_MULADDC_CASE_C##cn(arrtype, scalartype, src1, src2,     \
                                        dst, size.width)                    \
        }                                                                   \
    } else {                                                                \
        for(; size.height--; src1+=srcstep1, dst+=dststep)                  \
        {                                                                   \
            ICC_DEF_MULC_CASE_C##cn(arrtype, scalartype, src1,              \
                                        dst, size.width)                    \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


ICC_DEF_MULADDS_FUNC(32f_C1R, float, double, CC_UN_ENTRY_C1, 1)
ICC_DEF_MULADDS_FUNC(32f_C2R, float, double, CC_UN_ENTRY_C2, 2)
ICC_DEF_MULADDS_FUNC(64f_C1R, double, double, CC_UN_ENTRY_C1, 1)
ICC_DEF_MULADDS_FUNC(64f_C2R, double, double, CC_UN_ENTRY_C2, 2)


static void
icvInitMulAddCTable(CvBigFuncTable  tab)
{
    tab[CC_32F][1] = (void*)icvMulAddC_32f_C1R;
    tab[CC_32F][2] = (void*)icvMulAddC_32f_C2R;
    tab[CC_64F][1] = (void*)icvMulAddC_64f_C1R;
    tab[CC_64F][2] = (void*)icvMulAddC_64f_C2R;
}

CC_IMPL img_t*
cvScaleAdd(const img_t* src1, CScalar scale, const img_t* src2, img_t* dst)
{
    static CvBigFuncTable muladds_tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvScaleAdd");

    __BEGIN__;

    img_t src2stub[1] = {0};
    CSize size = cvGetMatSize(src1);
    TypeId type = CC_MAT_DEPTH(src1);
    int cn = CC_MAT_CN(src1);

    if (!src2) {
        src2 = src2stub;
    } else {
        if(!CC_ARE_TYPES_EQ(src2, dst))
            CC_ERROR(CC_StsUnmatchedFormats, "");

        if(!CC_ARE_SIZES_EQ(src2, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "");
    }

    cvSetMat(dst, size.height, size.width, type, cn, NULL, CC_AUTO_STEP);

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;

        if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE)
        {
            if(type == CC_32F && cn==1)
            {
                float* mA = src1->tt.fl;
                float* mB = src2->tt.fl;
                float* mC = dst->tt.fl;

                if (mB) {
                    do
                    {
                        mC[size.width - 1] = (float)(mA[size.width - 1]*scale.val[0] +
                                             mB[size.width - 1]);
                    }
                    while(--size.width);
                } else {
                    do
                    {
                        mC[size.width - 1] = (float)(mA[size.width - 1]*scale.val[0]);
                    }
                    while(--size.width);
                }

                EXIT;
            }

            if(type == CC_64F && 1==cn)
            {
                double* mA = src1->tt.db;
                double* mB = src2->tt.db;
                double* mC = dst->tt.db;

                if (mB) {
                    do
                    {
                        mC[size.width - 1] = mA[size.width - 1]*scale.val[0] +
                                             mB[size.width - 1];
                    }
                    while(--size.width);
                } else {
                    do
                    {
                        mC[size.width - 1] = mA[size.width - 1]*scale.val[0];
                    }
                    while(--size.width);
                }

                EXIT;
            }
        }

        size.height = 1;
    }

    if(!inittab)
    {
        icvInitMulAddCTable(muladds_tab);
        inittab = 1;
    }

    if(CC_MAT_CN(src1) > 2)
        CC_ERROR(CC_StsOutOfRange, "The function only supports 1- and 2-channel arrays");

    {
        CvFunc2D_3A1P func = (CvFunc2D_3A1P)(muladds_tab[type][cn]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src1->tt.data, src1->step, src2->tt.data, src2->step,
                         dst->tt.data, dst->step, size, scale.val));
    }

    CC_CHECK_NANS(dst);

    __END__;
    return dst;
}

CC_IMPL img_t*
cvMulS1(const img_t* A, double real_scalar, img_t* B) {
  //return cvScaleAdd(A, cRealScalar(real_scalar), NULL, B);
  return cvConvertTo(A, B, A->tid, real_scalar, 0);
}
CC_IMPL img_t*
axpy(const img_t* A, double real_scalar, const img_t* B, img_t* C) {
  return cvScaleAdd(A, cRealScalar(real_scalar), B, C);
}

CC_INLINE img_t*
cvMulS(const img_t* src, CScalar value, img_t* dst)
{
    return cvScaleAdd(src, value, NULL, dst);
}

CC_INLINE img_t*
cvMulS(const img_t* src, double value, img_t* dst)
{
    return cvScaleAdd(src, cRealScalar(value), NULL, dst);
}

CC_INLINE img_t*
cvDivS(const img_t* src, CScalar value, img_t* dst)
{
    CScalar inv = {0};
    int i;
    for (i=0; i<4; ++i) {
      if (fabs(value.val[i])>0.0000001) {
        inv.val[i] = 1./value.val[i];
      }
    }
    return cvScaleAdd(src, inv, NULL, dst);
}


/****************************************************************************************\
*                                        cvMulTransposed                                 *
\****************************************************************************************/

#define ICC_DEF_MULTRANS_R_FUNC(flavor, srctype, dsttype, load_macro)           \
static CStatus C_STDCALL                                                      \
icvMulTransposedR_##flavor(const srctype* src, int srcstep,                     \
                       dsttype* dst, int dststep,                               \
                       const dsttype* delta, int deltastep,                     \
                       CSize size, int delta_cols, double scale)               \
{                                                                               \
    int i, j, k;                                                                \
    dsttype* tdst = dst;                                                        \
    dsttype* col_buf = 0;                                                       \
    dsttype* delta_buf = 0;                                                     \
    int local_alloc = 0;                                                        \
    int buf_size = size.height*sizeof(dsttype);                                 \
                                                                                \
    if(delta && delta_cols < size.width)                                        \
    {                                                                           \
        assert(delta_cols == 1);                                                \
        buf_size += 4*buf_size;                                                 \
    }                                                                           \
                                                                                \
    if(buf_size <= CC_MAX_LOCAL_SIZE)                                           \
    {                                                                           \
        col_buf = (dsttype*)cvStackAlloc(buf_size);                             \
        local_alloc = 1;                                                        \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        col_buf = (dsttype*)cAlloc(buf_size);                                  \
        if(!col_buf)                                                            \
            return CC_OUTOFMEM_ERR;                                             \
    }                                                                           \
                                                                                \
    srcstep /= sizeof(src[0]); dststep /= sizeof(dst[0]);                       \
    deltastep /= sizeof(delta[0]);                                              \
                                                                                \
    if(delta && delta_cols < size.width)                                        \
    {                                                                           \
        delta_buf = col_buf + size.height;                                      \
        for(i = 0; i < size.height; i++)                                        \
            delta_buf[i*4] = delta_buf[i*4+1] =                                 \
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];       \
        delta = delta_buf;                                                      \
        deltastep = deltastep ? 4 : 0;                                          \
    }                                                                           \
                                                                                \
    if(!delta)                                                                  \
        for(i = 0; i < size.width; i++, tdst += dststep)                        \
        {                                                                       \
            for(k = 0; k < size.height; k++)                                    \
                col_buf[k] = src[k*srcstep+i];                                  \
                                                                                \
            for(j = i; j <= size.width - 4; j += 4)                             \
            {                                                                   \
                double s0 = 0, s1 = 0, s2 = 0, s3 = 0;                          \
                const srctype *tsrc = src + j;                                  \
                                                                                \
                for(k = 0; k < size.height; k++, tsrc += srcstep)               \
                {                                                               \
                    double a = col_buf[k];                                      \
                    s0 += a * load_macro(tsrc[0]);                              \
                    s1 += a * load_macro(tsrc[1]);                              \
                    s2 += a * load_macro(tsrc[2]);                              \
                    s3 += a * load_macro(tsrc[3]);                              \
                }                                                               \
                                                                                \
                tdst[j] = (dsttype)(s0*scale);                                  \
                tdst[j+1] = (dsttype)(s1*scale);                                \
                tdst[j+2] = (dsttype)(s2*scale);                                \
                tdst[j+3] = (dsttype)(s3*scale);                                \
            }                                                                   \
                                                                                \
            for(; j < size.width; j++)                                          \
            {                                                                   \
                double s0 = 0;                                                  \
                const srctype *tsrc = src + j;                                  \
                                                                                \
                for(k = 0; k < size.height; k++, tsrc += srcstep)               \
                    s0 += col_buf[k] * tsrc[0];                                 \
                                                                                \
                tdst[j] = (dsttype)(s0*scale);                                  \
            }                                                                   \
        }                                                                       \
    else                                                                        \
        for(i = 0; i < size.width; i++, tdst += dststep)                        \
        {                                                                       \
            if(!delta_buf)                                                      \
                for(k = 0; k < size.height; k++)                                \
                    col_buf[k] = load_macro(src[k*srcstep+i]) - delta[k*deltastep+i]; \
            else                                                                \
                for(k = 0; k < size.height; k++)                                \
                    col_buf[k] = load_macro(src[k*srcstep+i]) - delta_buf[k*deltastep]; \
                                                                                \
            for(j = i; j <= size.width - 4; j += 4)                             \
            {                                                                   \
                double s0 = 0, s1 = 0, s2 = 0, s3 = 0;                          \
                const srctype *tsrc = src + j;                                  \
                const dsttype *d = delta_buf ? delta_buf : delta + j;           \
                                                                                \
                for(k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep)   \
                {                                                               \
                    double a = col_buf[k];                                      \
                    s0 += a * (load_macro(tsrc[0]) - d[0]);                     \
                    s1 += a * (load_macro(tsrc[1]) - d[1]);                     \
                    s2 += a * (load_macro(tsrc[2]) - d[2]);                     \
                    s3 += a * (load_macro(tsrc[3]) - d[3]);                     \
                }                                                               \
                                                                                \
                tdst[j] = (dsttype)(s0*scale);                                  \
                tdst[j+1] = (dsttype)(s1*scale);                                \
                tdst[j+2] = (dsttype)(s2*scale);                                \
                tdst[j+3] = (dsttype)(s3*scale);                                \
            }                                                                   \
                                                                                \
            for(; j < size.width; j++)                                          \
            {                                                                   \
                double s0 = 0;                                                  \
                const srctype *tsrc = src + j;                                  \
                const dsttype *d = delta_buf ? delta_buf : delta + j;           \
                                                                                \
                for(k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep)   \
                    s0 += col_buf[k] * (load_macro(tsrc[0]) - d[0]);            \
                                                                                \
                tdst[j] = (dsttype)(s0*scale);                                  \
            }                                                                   \
        }                                                                       \
                                                                                \
    /* fill the lower part of the destination matrix */                         \
    for(i = 1; i < size.width; i++)                                             \
        for(j = 0; j < i; j++)                                                  \
            dst[dststep*i + j] = dst[dststep*j + i];                            \
                                                                                \
    if(col_buf && !local_alloc)                                                 \
        cFree(&col_buf);                                                       \
                                                                                \
    return CC_NO_ERR;                                                           \
}


#define ICC_DEF_MULTRANS_L_FUNC(flavor, srctype, dsttype, load_macro)           \
static CStatus C_STDCALL                                                      \
icvMulTransposedL_##flavor(const srctype* src, int srcstep,                     \
                            dsttype* dst, int dststep,                          \
                            dsttype* delta, int deltastep,                      \
                            CSize size, int delta_cols, double scale)          \
{                                                                               \
    int i, j, k;                                                                \
    dsttype* tdst = dst;                                                        \
                                                                                \
    srcstep /= sizeof(src[0]); dststep /= sizeof(dst[0]);                       \
    deltastep /= sizeof(delta[0]);                                              \
                                                                                \
    if(!delta)                                                                  \
        for(i = 0; i < size.height; i++, tdst += dststep)                       \
            for(j = i; j < size.height; j++)                                    \
            {                                                                   \
                double s = 0;                                                   \
                const srctype *tsrc1 = src + i*srcstep;                         \
                const srctype *tsrc2 = src + j*srcstep;                         \
                                                                                \
                for(k = 0; k <= size.width - 4; k += 4)                         \
                    s += tsrc1[k]*tsrc2[k] + tsrc1[k+1]*tsrc2[k+1] +            \
                         tsrc1[k+2]*tsrc2[k+2] + tsrc1[k+3]*tsrc2[k+3];         \
                for(; k < size.width; k++)                                      \
                    s += tsrc1[k] * tsrc2[k];                                   \
                tdst[j] = (dsttype)(s*scale);                                   \
            }                                                                   \
    else                                                                        \
    {                                                                           \
        dsttype* row_buf = 0;                                                   \
        int local_alloc = 0;                                                    \
        int buf_size = size.width*sizeof(dsttype);                              \
        dsttype delta_buf[4];                                                   \
        int delta_shift = delta_cols == size.width ? 4 : 0;                     \
                                                                                \
        if(buf_size <= CC_MAX_LOCAL_SIZE)                                       \
        {                                                                       \
            row_buf = (dsttype*)cvStackAlloc(buf_size);                         \
            local_alloc = 1;                                                    \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            row_buf = (dsttype*)cAlloc(buf_size);                              \
            if(!row_buf)                                                        \
                return CC_OUTOFMEM_ERR;                                         \
        }                                                                       \
                                                                                \
        for(i = 0; i < size.height; i++, tdst += dststep)                       \
        {                                                                       \
            const srctype *tsrc1 = src + i*srcstep;                             \
            const dsttype *tdelta1 = delta + i*deltastep;                       \
                                                                                \
            if(delta_cols < size.width)                                         \
                for(k = 0; k < size.width; k++)                                 \
                    row_buf[k] = tsrc1[k] - tdelta1[0];                         \
            else                                                                \
                for(k = 0; k < size.width; k++)                                 \
                    row_buf[k] = tsrc1[k] - tdelta1[k];                         \
                                                                                \
            for(j = i; j < size.height; j++)                                    \
            {                                                                   \
                double s = 0;                                                   \
                const srctype *tsrc2 = src + j*srcstep;                         \
                const dsttype *tdelta2 = delta + j*deltastep;                   \
                if(delta_cols < size.width)                                     \
                {                                                               \
                    delta_buf[0] = delta_buf[1] =                               \
                        delta_buf[2] = delta_buf[3] = tdelta2[0];               \
                    tdelta2 = delta_buf;                                        \
                }                                                               \
                for(k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift)   \
                    s += row_buf[k]*(load_macro(tsrc2[k]) - tdelta2[0]) +       \
                         row_buf[k+1]*(load_macro(tsrc2[k+1]) - tdelta2[1]) +   \
                         row_buf[k+2]*(load_macro(tsrc2[k+2]) - tdelta2[2]) +   \
                         row_buf[k+3]*(load_macro(tsrc2[k+3]) - tdelta2[3]);    \
                for(; k < size.width; k++, tdelta2++)                           \
                    s += row_buf[k]*(load_macro(tsrc2[k]) - tdelta2[0]);        \
                tdst[j] = (dsttype)(s*scale);                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        if(row_buf && !local_alloc)                                             \
            cFree(&row_buf);                                                   \
    }                                                                           \
                                                                                \
    /* fill the lower part of the destination matrix */                         \
    for(j = 0; j < size.height - 1; j++)                                        \
        for(i = j; i < size.height; i++)                                        \
            dst[dststep*i + j] = dst[dststep*j + i];                            \
                                                                                \
    return CC_NO_ERR;                                                           \
}


ICC_DEF_MULTRANS_R_FUNC(8u32f, uchar, float, CC_8TO32F)
ICC_DEF_MULTRANS_R_FUNC(8u64f, uchar, double, CC_8TO32F)
ICC_DEF_MULTRANS_R_FUNC(32f, float, float, CC_NOP)
ICC_DEF_MULTRANS_R_FUNC(32f64f, float, double, CC_NOP)
ICC_DEF_MULTRANS_R_FUNC(64f, double, double, CC_NOP)
ICC_DEF_MULTRANS_R_FUNC(16u32f, ushort, float, CC_NOP)
ICC_DEF_MULTRANS_R_FUNC(16u64f, ushort, double, CC_NOP)
ICC_DEF_MULTRANS_R_FUNC(16s32f, short, float, CC_NOP)
ICC_DEF_MULTRANS_R_FUNC(16s64f, short, double, CC_NOP)

ICC_DEF_MULTRANS_L_FUNC(8u32f, uchar, float, CC_8TO32F)
ICC_DEF_MULTRANS_L_FUNC(8u64f, uchar, double, CC_8TO32F)
ICC_DEF_MULTRANS_L_FUNC(32f, float, float, CC_NOP)
ICC_DEF_MULTRANS_L_FUNC(32f64f, float, double, CC_NOP)
ICC_DEF_MULTRANS_L_FUNC(64f, double, double, CC_NOP)
ICC_DEF_MULTRANS_L_FUNC(16u32f, ushort, float, CC_NOP)
ICC_DEF_MULTRANS_L_FUNC(16u64f, ushort, double, CC_NOP)
ICC_DEF_MULTRANS_L_FUNC(16s32f, short, float, CC_NOP)
ICC_DEF_MULTRANS_L_FUNC(16s64f, short, double, CC_NOP)


typedef CStatus (C_STDCALL * CvMulTransposedFunc)
    (const void* src, int srcstep,
      void* dst, int dststep, const void* delta,
      int deltastep, CSize size, int delta_cols, double scale);

CC_IMPL void
cvMulTransposed(const img_t* src, img_t* dst, int order,
                const img_t* deltaarr CC_DEFAULT(NULL), double scale CC_DEFAULT(1.))
{
    const int gemm_level = 100; // boundary above which GEMM is faster.
    img_t src2[1] = {0};

    CC_FUNCNAME("cvMulTransposed");

    __BEGIN__;

    const img_t* delta = deltaarr;
    img_t deltastub[1] = {0};
    int stype, dtype;
    int cn = CC_MAT_CN(src);

    if(delta)
    {
        if(!CC_ARE_TYPES_EQ(dst, delta))
            CC_ERROR(CC_StsUnmatchedFormats, "");

        if((delta->rows != src->rows && delta->rows != 1) ||
            (delta->cols != src->cols && delta->cols != 1))
            CC_ERROR(CC_StsUnmatchedSizes, "");
    }
    else
    {
        delta = deltastub;
    }

    stype = CC_MAT_DEPTH(src);
    dtype = CC_MAT_DEPTH(dst);

    if(dst->rows != dst->cols)
        CC_ERROR(CC_StsBadSize, "The destination matrix must be square");

    if((order != 0 && src->cols != dst->cols) ||
        (order == 0 && src->rows != dst->rows))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(src->tt.data == dst->tt.data || stype == dtype &&
        (dst->cols >= gemm_level && dst->rows >= gemm_level &&
         src->cols >= gemm_level && src->rows >= gemm_level))
    {
        if(deltaarr)
        {
            cvCreateMat(src2, src->rows, src->cols, src->tid, cn);
            cvRepeat(delta, src2);
            cvSub(src, src2, src2, NULL);
            src = src2;
        }
        cvGEMM(src, src, scale, 0, 0, dst, order == 0 ? CC_GEMM_B_T : CC_GEMM_A_T); 
    }
    else
    {
        CvMulTransposedFunc func =
            stype == CC_8U && dtype == CC_32F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_8u32f :
                    (CvMulTransposedFunc)icvMulTransposedL_8u32f) :
            stype == CC_8U && dtype == CC_64F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_8u64f :
                    (CvMulTransposedFunc)icvMulTransposedL_8u64f) :
            stype == CC_16U && dtype == CC_32F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_16u32f :
                    (CvMulTransposedFunc)icvMulTransposedL_16u32f) :
            stype == CC_16U && dtype == CC_64F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_16u64f :
                    (CvMulTransposedFunc)icvMulTransposedL_16u64f) :
            stype == CC_16S && dtype == CC_32F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_16s32f :
                    (CvMulTransposedFunc)icvMulTransposedL_16s32f) :
            stype == CC_16S && dtype == CC_64F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_16s64f :
                    (CvMulTransposedFunc)icvMulTransposedL_16s64f) :
            stype == CC_32F && dtype == CC_32F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_32f :
                    (CvMulTransposedFunc)icvMulTransposedL_32f) :
            stype == CC_32F && dtype == CC_64F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_32f64f :
                    (CvMulTransposedFunc)icvMulTransposedL_32f64f) :
            stype == CC_64F && dtype == CC_64F ?
            (order ? (CvMulTransposedFunc)icvMulTransposedR_64f :
                    (CvMulTransposedFunc)icvMulTransposedL_64f) : 0;

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src->step, dst->tt.data, dst->step,
                         delta->tt.data, delta->step, cvGetMatSize(src),
                         delta->cols, scale));
    }

    __END__;

    imfree(src2);
}



/****************************************************************************************\
*                                    cvCalcCovarMatrix                                   *
\****************************************************************************************/

#define ICC_DOT_PRODUCT_CASE(flavor, srctype, avgtype, load_macro)                    \
static CStatus C_STDCALL                                                              \
icvDotProductShifted_##flavor##_C1R(const srctype* vec1, int vecstep1,                 \
                                     const srctype* vec2, int vecstep2,                 \
                                     const avgtype* avg, int avgstep,                   \
                                     CSize size, double* _result)                     \
{                                                                                       \
    double result = 0;                                                                  \
    vecstep1 /= sizeof(vec1[0]); vecstep2 /= sizeof(vec2[0]); avgstep /= sizeof(avg[0]);\
                                                                                        \
    for(; size.height--; vec1 += vecstep1, vec2 += vecstep2, avg += avgstep)          \
    {                                                                                   \
        int x;                                                                          \
        for(x = 0; x <= size.width - 4; x += 4)                                       \
            result += (load_macro(vec1[x]) - avg[x])*(load_macro(vec2[x]) - avg[x]) +   \
                (load_macro(vec1[x+1]) - avg[x+1])*(load_macro(vec2[x+1]) - avg[x+1]) + \
                (load_macro(vec1[x+2]) - avg[x+2])*(load_macro(vec2[x+2]) - avg[x+2]) + \
                (load_macro(vec1[x+3]) - avg[x+3])*(load_macro(vec2[x+3]) - avg[x+3]);  \
        for(; x < size.width; x++)                                                    \
            result += (load_macro(vec1[x]) - avg[x])*(load_macro(vec2[x]) - avg[x]);    \
    }                                                                                   \
                                                                                        \
    *_result = result;                                                                  \
    return CC_OK;                                                                       \
}


ICC_DOT_PRODUCT_CASE(8u32f, uchar, float, CC_8TO32F)
ICC_DOT_PRODUCT_CASE(8u64f, uchar, double, CC_8TO32F)
ICC_DOT_PRODUCT_CASE(16u32f, ushort, float, CC_NOP)
ICC_DOT_PRODUCT_CASE(16u64f, ushort, double, CC_NOP)
ICC_DOT_PRODUCT_CASE(16s32f, short, float, CC_NOP)
ICC_DOT_PRODUCT_CASE(16s64f, short, double, CC_NOP)
ICC_DOT_PRODUCT_CASE(32f, float, float, CC_NOP)
ICC_DOT_PRODUCT_CASE(32f64f, float, double, CC_NOP)
ICC_DOT_PRODUCT_CASE(64f, double, double, CC_NOP)

static void  icvInitDotProductShiftedTable(CvFuncTable  tabfl, CvFuncTable  tabdb)
{
    tabfl[CC_8U] = (void*)icvDotProductShifted_8u32f_C1R;
    tabfl[CC_8S] = 0;
    tabfl[CC_16U] = (void*)icvDotProductShifted_16u32f_C1R;
    tabfl[CC_16S] = (void*)icvDotProductShifted_16s32f_C1R;
    tabfl[CC_32S] = 0;
    tabfl[CC_32F] = (void*)icvDotProductShifted_32f_C1R;
    tabfl[CC_64F] = 0;

    tabdb[CC_8U] = (void*)icvDotProductShifted_8u64f_C1R;
    tabdb[CC_8S] = 0;
    tabdb[CC_16U] = (void*)icvDotProductShifted_16u64f_C1R;
    tabdb[CC_16S] = (void*)icvDotProductShifted_16s64f_C1R;
    tabdb[CC_32S] = 0;
    tabdb[CC_32F] = (void*)icvDotProductShifted_32f64f_C1R;
    tabdb[CC_64F] = (void*)icvDotProductShifted_64f_C1R;
}

#define ICC_EXT_PRODUCT_CASE(flavor, srctype, avgtype, load_macro)                    \
static CStatus C_STDCALL                                                              \
icvExtProductShifted_##flavor##_C1R(const srctype* vec, int vecstep,                   \
                                     const avgtype* avg, int avgstep,                   \
                                     avgtype* dst, int dststep,                         \
                                     CSize size, avgtype* tempbuf)                    \
{                                                                                       \
    int x, y, dstsize = size.width * size.height;                                       \
                                                                                        \
    vecstep /= sizeof(vec[0]); avgstep /= sizeof(avg[0]);                               \
    for(y = 0; y < size.height; y++, vec += vecstep, avg += avgstep)                  \
        for(x = 0; x < size.width; x++)                                               \
            *tempbuf++ = load_macro(vec[x]) - avg[x];                                   \
    tempbuf -= dstsize;                                                                 \
                                                                                        \
    dststep /= sizeof(dst[0]);                                                          \
    for(y = 0; y < dstsize; y++, dst += dststep)                                      \
    {                                                                                   \
        double ty = tempbuf[y];                                                         \
        for(x = 0; x <= y - 3; x += 4)                                                \
        {                                                                               \
            double t0 = dst[x] + ty*tempbuf[x];                                         \
            double t1 = dst[x+1] + ty*tempbuf[x+1];                                     \
            dst[x] = (avgtype)t0;                                                       \
            dst[x+1] = (avgtype)t1;                                                     \
            t0 = dst[x+2] + ty*tempbuf[x+2];                                            \
            t1 = dst[x+3] + ty*tempbuf[x+3];                                            \
            dst[x+2] = (avgtype)t0;                                                     \
            dst[x+3] = (avgtype)t1;                                                     \
        }                                                                               \
        for(; x <= y; x++)                                                            \
            dst[x] = (avgtype)(dst[x] + ty*tempbuf[x]);                                 \
    }                                                                                   \
                                                                                        \
    return CC_OK;                                                                       \
}

ICC_EXT_PRODUCT_CASE(8u32f, uchar, float, CC_8TO32F)
ICC_EXT_PRODUCT_CASE(8u64f, uchar, double, CC_8TO32F)
ICC_EXT_PRODUCT_CASE(16u32f, ushort, float, CC_NOP)
ICC_EXT_PRODUCT_CASE(16u64f, ushort, double, CC_NOP)
ICC_EXT_PRODUCT_CASE(16s32f, short, float, CC_NOP)
ICC_EXT_PRODUCT_CASE(16s64f, short, double, CC_NOP)
ICC_EXT_PRODUCT_CASE(32f, float, float, CC_NOP)
ICC_EXT_PRODUCT_CASE(32f64f, float, double, CC_NOP)
ICC_EXT_PRODUCT_CASE(64f, double, double, CC_NOP)


static void  icvInitExtProductShiftedTable(CvFuncTable  tabfl, CvFuncTable  tabdb)
{
    tabfl[CC_8U] = (void*)icvExtProductShifted_8u32f_C1R;
    tabfl[CC_8S] = 0;
    tabfl[CC_16U] = (void*)icvExtProductShifted_16u32f_C1R;
    tabfl[CC_16S] = (void*)icvExtProductShifted_16s32f_C1R;
    tabfl[CC_32S] = 0;
    tabfl[CC_32F] = (void*)icvExtProductShifted_32f_C1R;
    tabfl[CC_64F] = 0;

    tabdb[CC_8U] = (void*)icvExtProductShifted_8u64f_C1R;
    tabdb[CC_8S] = 0;
    tabdb[CC_16U] = (void*)icvExtProductShifted_16u64f_C1R;
    tabdb[CC_16S] = (void*)icvExtProductShifted_16s64f_C1R;
    tabdb[CC_32S] = 0;
    tabdb[CC_32F] = (void*)icvExtProductShifted_32f64f_C1R;
    tabdb[CC_64F] = (void*)icvExtProductShifted_64f_C1R;
}


typedef struct vec_data
{
    void* ptr;
    int step;
}
vec_data;

CC_IMPL void
cvCalcCovarMatrix(const img_t** vecarr, int count,
                   img_t* cov, img_t* avg, int flags)
{
    static CvFuncTable dot_tab[2];
    static CvFuncTable ext_tab[2];
    static int inittab = 0;
    vec_data* vecdata = 0;
    img_t tempvec[1] = {0};
    
    CC_FUNCNAME("cvCalcCovarMatrix");

    __BEGIN__;

    img_t vecstub0, *vecmat = 0;
    CSize srcsize, contsize;
    TypeId srctype, dsttype;
    int i, j, cn = CC_MAT_CN(cov);
    int cont_flag, vec_delta = 0, vec_step = 0;
    int is_covar_normal = (flags & CC_COVAR_NORMAL) != 0;
    double scale;
    ASSERT(1==cn);

    if(!inittab)
    {
        icvInitDotProductShiftedTable(dot_tab[0], dot_tab[1]);
        icvInitExtProductShiftedTable(ext_tab[0], ext_tab[1]);
        inittab = 1;
    }

    if(!vecarr)
        CC_ERROR(CC_StsNullPtr, "NULL vec pointer");

    if(!CC_ARE_TYPES_EQ(cov, avg))
        CC_ERROR(CC_StsUnmatchedFormats,
        "Covariation matrix and average vector should have the same types");

    dsttype = CC_MAT_DEPTH(cov);
    if(dsttype != CC_32F && dsttype != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat, "Covariation matrix must be 32fC1 or 64fC1");

    if(cov->rows != cov->cols)
        CC_ERROR(CC_StsBadSize, "Covariation matrix must be square");

    srcsize = cvGetMatSize(avg);
    contsize.width = srcsize.width * srcsize.height;
    contsize.height = 1;
    cont_flag = CC_IS_MAT_CONT(avg);

    if(flags & (CC_COVAR_ROWS|CC_COVAR_COLS))
    {
        CC_CALL(vecmat = cvGetMat(vecarr[0], &vecstub0));
        srctype = CC_MAT_DEPTH(vecmat);
        if(flags & CC_COVAR_COLS)
        {
            count = vecmat->cols;
            if(avg->cols != 1 || avg->rows != vecmat->rows)
                CC_ERROR(CC_StsUnmatchedSizes,
                "The number of input vectors does not match to avg vector size");
            cont_flag = 0;
            vec_delta = CC_TYPE_SIZE(vecmat->tid);
            vec_step = vecmat->step;
        }
        else
        {
            count = vecmat->rows;
            if(avg->rows != 1 || avg->cols != vecmat->cols)
                CC_ERROR(CC_StsUnmatchedSizes,
                "The number of input vectors does not match to avg vector size");
            vec_delta = vecmat->step;
            vec_step = CC_STUB_STEP;
        }
        
        if(!(flags & CC_COVAR_USE_AVG))
            CC_CALL(cvReduce(vecmat, avg, -1, CC_REDUCE_AVG));

        scale = !(flags & CC_COVAR_SCALE) ? 1. : 1./count;

        cvMulTransposed(vecmat, cov, ((flags & CC_COVAR_ROWS)!=0) ^ ((flags & CC_COVAR_NORMAL)==0), avg, scale);
        EXIT;
    }

    scale = !(flags & CC_COVAR_SCALE) ? 1. : 1./count;

    if(is_covar_normal)
    {
        if(count <= 0)
            CC_ERROR(CC_StsBadSize,
            "The number of vectors is zero or negative");
        if(cov->rows != contsize.width)
            CC_ERROR(CC_StsUnmatchedSizes,
            "The size of input vectors does not match with the size of covariation matrix");

        cvCreateMat(tempvec, avg->rows, avg->cols, dsttype, cn);
    }
    else if(count != cov->rows)
        CC_ERROR(CC_StsUnmatchedSizes,
        "The vector count and covariance matrix size do not match");

    if(!(flags & (CC_COVAR_ROWS|CC_COVAR_COLS)))
    {
        if(!(flags & CC_COVAR_USE_AVG))
            cvZeroMat(avg);

        CC_CALL(vecdata = (vec_data*)cAlloc(count*sizeof(vecdata[0])));
    
        for(i = 0; i < count; i++)
        {
            img_t *vec = (img_t*)vecarr[i];
            img_t* temp;

            if(!CC_ARE_SIZES_EQ(vec, avg))
                CC_ERROR(CC_StsUnmatchedSizes,
                "All input vectors and average vector must have the same size");

            vecdata[i].ptr = vec->tt.data;
            vecdata[i].step = vec->step;
            cont_flag &= vec->tid;
            temp = vec;
            if(i == 0)
            {
                srctype = CC_MAT_DEPTH(vec);
                if(CC_MAT_CN(vec) != 1)
                    CC_ERROR(CC_BadNumChannels, "All vectors must have a single channel");
                if(srctype != dsttype && !tempvec && !(flags & CC_COVAR_USE_AVG))
                    CC_CALL(cvCreateMat(tempvec, vec->rows, vec->cols, dsttype, cn));
            }
            else if(CC_MAT_TYPE(vec) != srctype)
                CC_ERROR(CC_StsUnmatchedFormats,
                "All input vectors must have the same type");

            if(!(flags & CC_COVAR_USE_AVG))
            {
                if(tempvec)
                {
                    temp = tempvec;
                    cvConvert(vec, temp);
                }
                cvAdd(temp, avg, avg, NULL);
            }
        }

        if(!(flags & CC_COVAR_USE_AVG))
            cvScale(avg, avg, 1./count, 0);
    }

    if(cont_flag)
        srcsize = contsize;

    if(!is_covar_normal)
    {
        CvFunc2D_3A1P dot_func =
            (CvFunc2D_3A1P)dot_tab[dsttype == CC_64F && 1==cn][srctype];
        
        if(!dot_func)
            CC_ERROR(CC_StsUnsupportedFormat,
            "The format of input vectors is not supported");
        
        for(i = 0; i < count; i++)
        {
            int a, b, delta;
            if(!(i & 1))
                a = 0, b = i+1, delta = 1;
            else
                a = i, b = -1, delta = -1;

            for(j = a; j != b; j += delta)
            {
                double result = 0;
                void *v_i, *v_j;
                int step_i, step_j;

                if(!vecmat)
                {
                    v_i = vecdata[i].ptr;
                    v_j = vecdata[j].ptr;
                    step_i = vecdata[i].step;
                    step_j = vecdata[j].step;
                }
                else
                {
                    v_i = vecmat->tt.data + vec_delta*i;
                    v_j = vecmat->tt.data + vec_delta*j;
                    step_i = step_j = vec_step;
                }

                dot_func(v_i, step_i, v_j, step_j, avg->tt.data, avg->step, srcsize, &result);

                if(dsttype == CC_64F && 1==cn)
                {
                    ((double*)(cov->tt.data + i*cov->step))[j] =
                    ((double*)(cov->tt.data + j*cov->step))[i] = result*scale;
                }
                else
                {
                    ((float*)(cov->tt.data + i*cov->step))[j] =
                    ((float*)(cov->tt.data + j*cov->step))[i] = (float)(result*scale);
                }
            }
        }
    }
    else
    {
        uchar* cov_ptr = cov->tt.data;
        int cov_step = cov->step;
        int cov_size = cov->rows;
        CvFunc2D_3A1P ext_func =
            (CvFunc2D_3A1P)ext_tab[dsttype == CC_64F && 1==cn][srctype];
        if(!ext_func)
            CC_ERROR(CC_StsUnsupportedFormat,
            "The format of input vectors is not supported");
        
        cvZeroMat(cov);
        
        for(i = 0; i < count; i++)
        {
            void* v;
            int vstep;
            if(!vecmat)
            {
                v = vecdata[i].ptr;
                vstep = vecdata[i].step;
            }
            else
            {
                v = vecmat->tt.data + vec_delta*i;
                vstep = vec_step;
            }

            ext_func(v, vstep, avg->tt.data, avg->step,
                      cov_ptr, cov_step, srcsize, tempvec->tt.data);
        }

        if(dsttype == CC_64F && 1==cn)
            for(i = 0; i < cov_size; i++)
                for(j = 0; j <= i; j++)
                {
                    double* cov1 = ((double*)(cov_ptr + i*cov_step)) + j;
                    double* cov2 = ((double*)(cov_ptr + j*cov_step)) + i;

                    if(flags & CC_COVAR_SCALE)
                        *cov1 = *cov2 = *cov1*scale;
                    else
                        *cov2 = *cov1;
                }
        else
            for(i = 0; i < cov_size; i++)
                for(j = 0; j <= i; j++)
                {
                    float* cov1 = ((float*)(cov_ptr + i*cov_step)) + j;
                    float* cov2 = ((float*)(cov_ptr + j*cov_step)) + i;

                    if(flags & CC_COVAR_SCALE)
                        *cov1 = *cov2 = (float)(*cov1*scale);
                    else
                        *cov2 = *cov1;
                }
    }

    __END__;

    cFree(&vecdata);
    imfree(tempvec);
}

/****************************************************************************************\
*                                        cvMahalanobis                                   *
\****************************************************************************************/

#define ICC_MAHALANOBIS(flavor, arrtype)                                              \
static CStatus C_STDCALL                                                              \
icvMahalanobis_##flavor##_C1R(const arrtype* mat, int matstep,                         \
                               const arrtype* vec, int len, double* _result)           \
{                                                                                       \
    int i, j;                                                                           \
    double result = 0;                                                                  \
                                                                                        \
    matstep /= sizeof(mat[0]);                                                          \
    for(i = 0; i < len; i++, mat += matstep)                                          \
    {                                                                                   \
        double row_sum = 0;                                                             \
        for(j = 0; j <= len - 4; j += 4)                                              \
            row_sum += vec[j]*mat[j] + vec[j+1]*mat[j+1] +                              \
                       vec[j+2]*mat[j+2] + vec[j+3]*mat[j+3];                           \
        for(; j < len; j++)                                                           \
            row_sum += vec[j]*mat[j];                                                   \
        result += row_sum * vec[i];                                                     \
    }                                                                                   \
    *_result = result;                                                                  \
                                                                                        \
    return CC_OK;                                                                       \
}

ICC_MAHALANOBIS(32f, float)
ICC_MAHALANOBIS(64f, double)

static void  icvInitMahalanobisTable(CvFuncTable  tab)
{
    tab[CC_32F] = (void*)icvMahalanobis_32f_C1R;
    tab[CC_64F] = (void*)icvMahalanobis_64f_C1R;
}

typedef CStatus (C_STDCALL * CvMahalanobisFunc)(const void* mat, int matstep,
                                                   const void* vec, int len, double* _result);

CC_IMPL double
cvMahalanobis(const img_t* srcA, const img_t* srcB, img_t* matarr)
{
    static CvFuncTable mahal_tab;
    static int inittab = 0;
    uchar* buffer = 0;
    int local_alloc = 0;
    double dist = 0;

    CC_FUNCNAME("cvMahalanobis");

    __BEGIN__;

    int buf_size, elem_size, len;
    
    
    img_t *mat = matarr;
    img_t temp[1];
    CvMahalanobisFunc func;

    if(!inittab)
    {
        icvInitMahalanobisTable(mahal_tab);
        inittab = 1;
    }

    if(srcA->rows != 1 && srcA->cols != 1)
        CC_ERROR(CC_StsBadSize, "Input matrices must be 1-d vectors");

    len = srcA->rows + srcA->cols - 1;

    if(!CC_ARE_SIZES_EQ(srcA,srcB))
        CC_ERROR(CC_StsUnmatchedSizes, "Input vectors have different sizes");
    
    if(mat->rows != len || mat->cols != len)
        CC_ERROR(CC_StsUnmatchedSizes, "Input vectors and covariation matrix have different sizes");

    func = (CvMahalanobisFunc)mahal_tab[srcA->tid];

    if(CC_MAT_CN(srcA) > 1 || !func)
        CC_ERROR(CC_StsUnsupportedFormat,
        "Only single-channel floating-point vectors are supported");

    if(!CC_ARE_TYPES_EQ(srcA,srcB) || !CC_ARE_TYPES_EQ(srcA,mat))
        CC_ERROR(CC_StsUnmatchedSizes, "Input vectors have different sizes");

    elem_size = CC_TYPE_SIZE(srcA->tid);
    buf_size = len*elem_size;
    
    if(buf_size <= CC_MAX_LOCAL_SIZE)
    {
        buffer = (uchar*)cvStackAlloc(buf_size);
        local_alloc = 1;
    }
    else
    {
        CC_CALL(buffer = (uchar*)cAlloc(buf_size));
    }

    cvMat(temp, srcA->rows, srcA->cols, srcA->tid, 1, buffer, CC_AUTO_STEP);
    cvSub(srcA, srcB, temp, NULL);

    IPPI_CALL(func(mat->tt.data, mat->step, temp->tt.data, len, &dist));
    dist = sqrt(dist);

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);

    return  dist;
}


/****************************************************************************************\
*                                        cvDotProduct                                    *
\****************************************************************************************/

#define ICC_DEF_DOT_PROD_FUNC_2D(flavor, arrtype, temptype, sumtype)  \
static CStatus C_STDCALL                                              \
icvDotProduct_##flavor##_C1R(const arrtype* src1, int step1,           \
                              const arrtype* src2, int step2,           \
                              CSize size, sumtype* _sum)              \
{                                                                       \
    sumtype sum = 0;                                                    \
    step1 /= sizeof(src1[0]); step2 /= sizeof(src2[0]);                 \
                                                                        \
    for(; size.height--; src1 += step1, src2 += step2)                \
    {                                                                   \
        int i;                                                          \
                                                                        \
        for(i = 0; i <= size.width - 4; i += 4)                       \
        {                                                               \
            temptype t0 = (temptype)src1[i]*src2[i];                    \
            temptype t1 = (temptype)src1[i+1]*src2[i+1];                \
            t0 += (temptype)src1[i+2]*src2[i+2];                        \
            t1 += (temptype)src1[i+3]*src2[i+3];                        \
            sum += t0 + t1;                                             \
        }                                                               \
                                                                        \
        for(; i < size.width; i++)                                    \
        {                                                               \
            sum += (temptype)src1[i]*src2[i];                           \
        }                                                               \
    }                                                                   \
                                                                        \
    *_sum = sum;                                                        \
    return CC_OK;                                                       \
}


ICC_DEF_DOT_PROD_FUNC_2D(8u, uchar, int, int64)
ICC_DEF_DOT_PROD_FUNC_2D(16u, ushort, int64, int64)
ICC_DEF_DOT_PROD_FUNC_2D(16s, short, int64, int64)
ICC_DEF_DOT_PROD_FUNC_2D(32s, int, double, double)
ICC_DEF_DOT_PROD_FUNC_2D(32f, float, double, double)
ICC_DEF_DOT_PROD_FUNC_2D(64f, double, double, double)

#define icvDotProduct_8s_C1R 0

CC_DEF_INIT_FUNC_TAB_2D(DotProduct, C1R)

CC_IMPL double
cvDotProduct(const img_t* srcA, const img_t* srcB)
{
    static CvFuncTable tab_2d;
    static int inittab = 0;

    suf64_t result;
    
    CC_FUNCNAME("cvDotProduct");

    __BEGIN__;
    CSize size;
    TypeId type;
    CvFunc2D_2A1P func;

    result.f = 0;

    if(!inittab)
    {
        icvInitDotProductC1RTable(tab_2d);
        inittab = 1;
    }

    if(srcB != srcA)
    {
        if(!CC_ARE_TYPES_EQ(srcA, srcB))
            CC_ERROR(CC_StsUnmatchedFormats, "");

        if(!CC_ARE_SIZES_EQ(srcA, srcB))
            CC_ERROR(CC_StsUnmatchedSizes, "");
    }

    type = CC_MAT_DEPTH(srcA);
    size = cvGetMatSize(srcA);

    size.width *= CC_MAT_CN(srcA);

    if(CC_IS_MAT_CONT(srcA) && CC_IS_MAT_CONT(srcB))
    {
        size.width *= size.height;

        if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE)
        {
            if(type == CC_32F)
            {
                float* mA = srcA->tt.fl;
                float* mB = srcB->tt.fl;
                double sum = 0;
                do
                    sum += (double)mA[size.width - 1]*mB[size.width - 1];
                while(--size.width);
                result.f = sum;
                EXIT;
            }
            
            if(type == CC_64F)
            {
                double* mA = srcA->tt.db;
                double* mB = srcB->tt.db;
                double sum = 0;
                do
                    sum += mA[size.width - 1]*mB[size.width - 1];
                while(--size.width);
                result.f = sum;
                EXIT;
            }
        }
        size.height = 1;
    }

    func = (CvFunc2D_2A1P)(tab_2d[type]);
    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    IPPI_CALL(func(srcA->tt.data, srcA->step,
                     srcB->tt.data, srcB->step,
                     size, &result));

    if(type < CC_32S)
        result.f = (double)result.i;

    __END__;

    return result.f;
}


#define ICVMATMUL2X2_C1R(arrtype)                     \
{   int i = 0;\
  for (;i<len; ++i) { \
    double s0, s1, d0, d1; \
    const uchar* _src = src + i*src_step; \
    uchar* _dst = dst + i*dst_step; \
    s0 = *(arrtype*)(_src); \
    s1 = *(arrtype*)(_src + src_cn); \
    d0 = s0 * mat22[0] + s1 * mat22[2]; \
    d1 = s0 * mat22[1] + s1 * mat22[3]; \
    *(arrtype*)(_dst) = d0; \
    *(arrtype*)(_dst + src_cn) = d1; \
  } \
}


static void icvMatMul2x2_C1R(int len, const uchar* src, int src_step, int src_cn, const double4 mat22, uchar* dst, int dst_step, int dst_cn, TypeId type) {

  if (type==CC_32F) {
    ICVMATMUL2X2_C1R(float);
  } else if (type==CC_64F) {
    ICVMATMUL2X2_C1R(double);
  } else {
    CC_ERROR(CC_StsUnsupportedFormat, "");
  }
}

CC_IMPL void
cvMatMul2x2(const img_t* src, const double4 mat22, img_t* dst, int flags CC_DEFAULT(0)) {
  CC_FUNCNAME("icvMatMul2x2");
  __BEGIN__;
  double4 mat22_ = {0};
  int is_a_t = (flags & CC_GEMM_A_T)!=0;
  int is_c_t = (flags & CC_GEMM_C_T)!=0;
  if (dst!=src) {
    if (is_c_t!=is_a_t) {
      cvSetMat(dst, src->w, src->h, src->tid, CC_MAT_CN(src), NULL, CC_AUTO_STEP);
    } else {
      cvSetMatFromMat(dst, src);
    }
  }
  if (flags & CC_GEMM_B_T) {
    mat22_[0] = mat22[0];
    mat22_[1] = mat22[2];
    mat22_[2] = mat22[1];
    mat22_[3] = mat22[3];
    mat22 = mat22_;
  }
  {
    int tt = is_a_t*2+is_c_t;
  int ssc[] = {src->step, src->cn};
  int dsc[] = {dst->step, dst->cn};
  int len = is_a_t ? src->w : src->h;
  icvMatMul2x2_C1R(len, src->tt.data, ssc[is_a_t], ssc[!is_a_t], mat22, dst->tt.data, dsc[is_c_t], dsc[!is_c_t], CC_MAT_DEPTH(src));
  }
  __END__;
  return ;
}

CC_IMPL void
cvMatMul2x2f(const img_t* src, const float4 mat22, img_t* dst, int flags CC_DEFAULT(0)) {
  double4 dmat22;
  COPY4(dmat22, mat22);
  cvMatMul2x2(src, dmat22, dst, flags);
  return ;
}