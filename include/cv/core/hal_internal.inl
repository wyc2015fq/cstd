
#include "f2c.h"
//#include "blaswrap.h"
#include "clapack.h"

#pragma comment(lib,"blas.lib")
#pragma comment(lib,"lapack.lib")
#pragma comment(lib,"libf2c.lib")
//#include "cmd/clapack.inl"
//#include "cmd/clapack.inl"

#undef cv_hal_LU32f
#define cv_hal_LU32f lapack_LU32f
#undef cv_hal_LU64f
#define cv_hal_LU64f lapack_LU64f

#undef cv_hal_Cholesky32f
#define cv_hal_Cholesky32f lapack_Cholesky32f
#undef cv_hal_Cholesky64f
#define cv_hal_Cholesky64f lapack_Cholesky64f

#undef cv_hal_SVD32f
#define cv_hal_SVD32f lapack_SVD32f
#undef cv_hal_SVD64f
#define cv_hal_SVD64f lapack_SVD64f

#undef cv_hal_QR32f
#define cv_hal_QR32f lapack_QR32f
#undef cv_hal_QR64f
#define cv_hal_QR64f lapack_QR64f


#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

//lapack stores matrices in column-major order so transposing is neded everywhere
template <typename fptype> static inline void
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    for(size_t i = 0; i < m - 1; i++)
        for(size_t j = i + 1; j < m; j++)
            T_SWAP(fptype, src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> static inline void
transpose(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    for(size_t i = 0; i < m; i++)
        for(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> static inline void
copy_matrix(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    for(size_t i = 0; i < m; i++)
        for(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> static inline void
set_value(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    for(size_t i = 0; i < m; i++)
        for(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

template <typename fptype> static inline long
lapack_LU(fptype* a, size_t a_step, long m, fptype* b, size_t b_step, long n, long* info, TypeId type)
{
    long lda = a_step / sizeof(fptype), sign = 0;
    long* piv = new long[m];

    transpose_square_inplace(a, lda, m);

    if(b)
    {
        if(n == 1 && b_step == sizeof(fptype))
        {
            if(type == CC_32F)
                sgesv_(&m, &n, (float*)a, &lda, piv, (float*)b, &m, info);
            else if(type == CC_64F)
                dgesv_(&m, &n, (double*)a, &lda, piv, (double*)b, &m, info);
        }
        else
        {
            long ldb = b_step / sizeof(fptype);
            fptype* tmpB = new fptype[m*n];

            transpose(b, ldb, tmpB, m, m, n);

            if(type == CC_32F)
                sgesv_(&m, &n, (float*)a, &lda, piv, (float*)tmpB, &m, info);
            else if(type == CC_64F)
                dgesv_(&m, &n, (double*)a, &lda, piv, (double*)tmpB, &m, info);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    else
    {
        if(type == CC_32F)
            sgetrf_(&m, &m, (float*)a, &lda, piv, info);
        else if(type == CC_64F)
            dgetrf_(&m, &m, (double*)a, &lda, piv, info);
    }

    if(*info == 0)
    {
        for(long i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    else
        *info = 0; //in opencv LU function zero means error

    delete[] piv;
    return CC_HAL_ERROR_OK;
}

template <typename fptype> static inline long
lapack_Cholesky(fptype* a, size_t a_step, long m, fptype* b, size_t b_step, long n, bool* info, TypeId type)
{
    long lapackStatus = 0;
    long lda = a_step / sizeof(fptype);
    char L[] = {'L', '\0'};

    if(b)
    {
        if(n == 1 && b_step == sizeof(fptype))
        {
            if(type == CC_32F)
                sposv_(L, &m, &n, (float*)a, &lda, (float*)b, &m, &lapackStatus);
            else if(type == CC_64F)
                dposv_(L, &m, &n, (double*)a, &lda, (double*)b, &m, &lapackStatus);
        }
        else
        {
            long ldb = b_step / sizeof(fptype);
            fptype* tmpB = new fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            if(type == CC_32F)
                sposv_(L, &m, &n, (float*)a, &lda, (float*)tmpB, &m, &lapackStatus);
            else if(type == CC_64F)
                dposv_(L, &m, &n, (double*)a, &lda, (double*)tmpB, &m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    else
    {
        if(type == CC_32F)
            spotrf_(L, &m, (float*)a, &lda, &lapackStatus);
        else if(type == CC_64F)
            dpotrf_(L, &m, (double*)a, &lda, &lapackStatus);
    }

    if(lapackStatus == 0) *info = true;
    else *info = false; //in opencv Cholesky function false means error

    return CC_HAL_ERROR_OK;
}

template <typename fptype> static inline long
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, long m, long n, long flags, long* info, TypeId type)
{
    long lda = a_step / sizeof(fptype);
    long ldv = v_step / sizeof(fptype);
    long ldu = u_step / sizeof(fptype);
    long lwork = -1;
    long* iworkBuf = new long[8*MIN(m, n)];
    fptype work1 = 0;

    //A already transposed and m>=n
    char mode[] = { ' ', '\0'};
    if(flags & CC_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    else if((flags & CC_HAL_SVD_SHORT_UV) && (flags & CC_HAL_SVD_MODIFY_A)) //short CSVD, U stored in a
        mode[0] = 'O';
    else if((flags & CC_HAL_SVD_SHORT_UV) && !(flags & CC_HAL_SVD_MODIFY_A)) //short CSVD, U stored in u if m>=n
        mode[0] = 'S';
    else if(flags & CC_HAL_SVD_FULL_UV) //full CSVD, U stored in u or in a
        mode[0] = 'A';

    if((flags & CC_HAL_SVD_MODIFY_A) && (flags & CC_HAL_SVD_FULL_UV)) //U stored in a
    {
        u = new fptype[m*m];
        ldu = m;
    }

    if(type == CC_32F)
        sgesdd_(mode, &m, &n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&work1, &lwork, iworkBuf, info);
    else if(type == CC_64F)
        dgesdd_(mode, &m, &n, (double*)a, &lda, (double*)w, (double*)u, &ldu, (double*)vt, &ldv, (double*)&work1, &lwork, iworkBuf, info);

    lwork = (long)ROUND(work1); //optimal buffer size
    fptype* buffer = new fptype[lwork + 1];

    if(type == CC_32F)
        sgesdd_(mode, &m, &n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)buffer, &lwork, iworkBuf, info);
    else if(type == CC_64F)
        dgesdd_(mode, &m, &n, (double*)a, &lda, (double*)w, (double*)u, &ldu, (double*)vt, &ldv, (double*)buffer, &lwork, iworkBuf, info);

    if(!(flags & CC_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    if((flags & CC_HAL_SVD_MODIFY_A) && (flags & CC_HAL_SVD_FULL_UV))
    {
        for(long i = 0; i < m; i++)
            for(long j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
        delete[] u;
    }

    delete[] iworkBuf;
    delete[] buffer;
    return CC_HAL_ERROR_OK;
}

template <typename fptype> static inline long
lapack_QR(fptype* a, size_t a_step, long m, long n, long k, fptype* b, size_t b_step, fptype* dst, long* info, TypeId type)
{
    long lda = a_step / sizeof(fptype);
    char mode[] = { 'N', '\0' };
    if(m < n)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;

    fptype* tmpAMemHolder;
    fptype* tmpA;
    long ldtmpA;
    if (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    else
    {
        tmpAMemHolder = MALLOC(fptype, m*n);
        tmpA = tmpAMemHolder;
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    long lwork = -1;
    fptype work1 = 0.;

    if (b)
    {
        if (k == 1 && b_step == sizeof(fptype))
        {
            if (type == CC_32F)
                sgels_(mode, &m, &n, &k, (float*)tmpA, &ldtmpA, (float*)b, &m, (float*)&work1, &lwork, info);
            else if (type == CC_64F)
                dgels_(mode, &m, &n, &k, (double*)tmpA, &ldtmpA, (double*)b, &m, (double*)&work1, &lwork, info);

            lwork = (long)ROUND(work1); //optimal buffer size
            fptype* workBufMemHolder = MALLOC(fptype, lwork + 1);
            fptype* buffer = workBufMemHolder;

            if (type == CC_32F)
                sgels_(mode, &m, &n, &k, (float*)tmpA, &ldtmpA, (float*)b, &m, (float*)buffer, &lwork, info);
            else if (type == CC_64F)
                dgels_(mode, &m, &n, &k, (double*)tmpA, &ldtmpA, (double*)b, &m, (double*)buffer, &lwork, info);
            FREE(workBufMemHolder);
        }
        else
        {
            fptype* tmpBMemHolder = MALLOC(fptype, m*k);
            fptype* tmpB = tmpBMemHolder;
            long ldb = b_step / sizeof(fptype);
            transpose(b, ldb, tmpB, m, m, k);

            if (type == CC_32F)
                sgels_(mode, &m, &n, &k, (float*)tmpA, &ldtmpA, (float*)tmpB, &m, (float*)&work1, &lwork, info);
            else if (type == CC_64F)
                dgels_(mode, &m, &n, &k, (double*)tmpA, &ldtmpA, (double*)tmpB, &m, (double*)&work1, &lwork, info);

            lwork = (long)ROUND(work1); //optimal buffer size
            fptype* workBufMemHolder = MALLOC(fptype, lwork + 1);
            fptype* buffer = workBufMemHolder;

            if (type == CC_32F)
                sgels_(mode, &m, &n, &k, (float*)tmpA, &ldtmpA, (float*)tmpB, &m, (float*)buffer, &lwork, info);
            else if (type == CC_64F)
                dgels_(mode, &m, &n, &k, (double*)tmpA, &ldtmpA, (double*)tmpB, &m, (double*)buffer, &lwork, info);

            transpose(tmpB, m, b, ldb, k, m);
            FREE(workBufMemHolder);
        }
    }
    else
    {
        if (type == CC_32F)
            sgeqrf_(&m, &n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)&work1, &lwork, info);
        else if (type == CC_64F)
            dgeqrf_(&m, &n, (double*)tmpA, &ldtmpA, (double*)dst, (double*)&work1, &lwork, info);

        lwork = (long)ROUND(work1); //optimal buffer size
        fptype* workBufMemHolder = MALLOC(fptype, lwork + 1);
        fptype* buffer = workBufMemHolder;

        if (type == CC_32F)
            sgeqrf_(&m, &n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)buffer, &lwork, info);
        else if (type == CC_64F)
            dgeqrf_(&m, &n, (double*)tmpA, &ldtmpA, (double*)dst, (double*)buffer, &lwork, info);
        FREE(workBufMemHolder);
    }

    if (m == n)
        transpose_square_inplace(a, lda, m);
    else
        transpose(tmpA, m, a, lda, n, m);

    if (*info != 0)
        *info = 0;
    else
        *info = 1;

    if (tmpAMemHolder) {FREE(tmpAMemHolder);}
    return CC_HAL_ERROR_OK;
}

long lapack_LU32f(float* a, size_t a_step, long m, float* b, size_t b_step, long n, int* info)
{
    if(m < HAL_LU_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return lapack_LU(a, a_step, m, b, b_step, n, (long*)info, CC_32F);
}

long lapack_LU64f(double* a, size_t a_step, long m, double* b, size_t b_step, long n, int* info)
{
    if(m < HAL_LU_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return lapack_LU(a, a_step, m, b, b_step, n, (long*)info, CC_64F);
}

long lapack_Cholesky32f(float* a, size_t a_step, long m, float* b, size_t b_step, long n, bool *info)
{
    if(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return lapack_Cholesky(a, a_step, m, b, b_step, n, info, CC_32F);
}

long lapack_Cholesky64f(double* a, size_t a_step, long m, double* b, size_t b_step, long n, bool *info)
{
    if(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return lapack_Cholesky(a, a_step, m, b, b_step, n, info, CC_64F);
}

long lapack_SVD32f(float* a, size_t a_step, float *w, float* u, size_t u_step, float* vt, size_t v_step, long m, long n, long flags)
{

    if(m < HAL_SVD_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    long info;
    return lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info, CC_32F);
}

long lapack_SVD64f(double* a, size_t a_step, double *w, double* u, size_t u_step, double* vt, size_t v_step, long m, long n, long flags)
{

    if(m < HAL_SVD_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    long info;
    return lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info, CC_64F);
}

long lapack_QR32f(float* src1, size_t src1_step, long m, long n, long k, float* src2, size_t src2_step, float* dst, int* info)
{
    if (m < HAL_QR_SMALL_MATRIX_THRESH)
      return CC_HAL_ERROR_NOT_IMPLEMENTED;
    long linfo=0, ret = lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, &linfo, CC_32F);
    *info = linfo;
    return ret;
}

long lapack_QR64f(double* src1, size_t src1_step, long m, long n, long k, double* src2, size_t src2_step, double* dst, int* info)
{
    if (m < HAL_QR_SMALL_MATRIX_THRESH)
      return CC_HAL_ERROR_NOT_IMPLEMENTED;
    long linfo=0, ret = lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, &linfo, CC_64F);
    *info = linfo;
    return ret;
}


#if 0
#undef cv_hal_gemm32f
#define cv_hal_gemm32f lapack_gemm32f
#undef cv_hal_gemm64f
#define cv_hal_gemm64f lapack_gemm64f
#undef cv_hal_gemm32fc
#define cv_hal_gemm32fc lapack_gemm32fc
#undef cv_hal_gemm64fc
#define cv_hal_gemm64fc lapack_gemm64fc

enum {CblasTrans='T', CblasNoTrans='N', };

template <typename fptype> static inline long
lapack_gemm(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, long a_m, long a_n, long d_n, long flags, TypeId type)
{
    long ldsrc1 = src1_step / sizeof(fptype);
    long ldsrc2 = src2_step / sizeof(fptype);
    long ldsrc3 = src3_step / sizeof(fptype);
    long lddst = dst_step / sizeof(fptype);
    long c_m, c_n, d_m;
    char transA, transB;

    if(flags & CC_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        if(flags & CC_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        else
        {
            d_m = a_m;
        }
    }
    else
    {
        transB = CblasNoTrans;
        if(flags & CC_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        else
        {
            d_m = a_m;
        }
    }

    if(flags & CC_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    else
    {
        c_m = d_m;
        c_n = d_n;
    }

    if(flags & CC_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        T_SWAP(long, a_n, a_m);
    }
    else
    {
        transA = CblasNoTrans;
    }

    if(src3 != dst && beta != 0.0 && src3_step != 0) {
        if(flags & CC_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        else
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    else if (src3 == dst && (flags & CC_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    else if(src3_step == 0 && beta != 0.0)
        set_value(dst, lddst, (fptype)0.0, d_m, d_n);

    if(type == CC_32F)
        sgemm_(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float)alpha, (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float)beta, (float*)dst, lddst);
    else if(type == CC_64F)
        dgemm_(CblasRowMajor, transA, transB, a_m, d_n, a_n, (double)alpha, (double*)src1, ldsrc1, (double*)src2, ldsrc2, (double)beta, (double*)dst, lddst);

    return CC_HAL_ERROR_OK;
}


template <typename fptype> static inline long
lapack_gemm_c(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, long a_m, long a_n, long d_n, long flags, TypeId type)
{
    long ldsrc1 = src1_step / sizeof(std::complex<fptype>);
    long ldsrc2 = src2_step / sizeof(std::complex<fptype>);
    long ldsrc3 = src3_step / sizeof(std::complex<fptype>);
    long lddst = dst_step / sizeof(std::complex<fptype>);
    long c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    if(flags & CC_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        if(flags & CC_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        else
        {
            d_m = a_m;
        }
    }
    else
    {
        transB = CblasNoTrans;
        if(flags & CC_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        else
        {
            d_m = a_m;
        }
    }

    if(flags & CC_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    else
    {
        c_m = d_m;
        c_n = d_n;
    }

    if(flags & CC_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        T_SWAP(_Tp, a_n, a_m);
    }
    else
    {
        transA = CblasNoTrans;
    }

    if(src3 != dst && beta != 0.0 && src3_step != 0) {
        if(flags & CC_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        else
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    else if (src3 == dst && (flags & CC_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    else if(src3_step == 0 && beta != 0.0)
        set_value((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    if(type == CC_32F)
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float*)reinterpret_cast<fptype(&)[2]>(cAlpha), (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float*)reinterpret_cast<fptype(&)[2]>(cBeta), (float*)dst, lddst);
    else if(type == CC_64F)
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (double*)reinterpret_cast<fptype(&)[2]>(cAlpha), (double*)src1, ldsrc1, (double*)src2, ldsrc2, (double*)reinterpret_cast<fptype(&)[2]>(cBeta), (double*)dst, lddst);

    return CC_HAL_ERROR_OK;
}
long lapack_gemm32f(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, long m, long n, long k, long flags)
{
    if(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags, CC_32F);
}

long lapack_gemm64f(const double *src1, size_t src1_step, const double *src2, size_t src2_step, double alpha,
                   const double *src3, size_t src3_step, double beta, double *dst, size_t dst_step, long m, long n, long k, long flags)
{
    if(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags, CC_64F);
}

long lapack_gemm32fc(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, long m, long n, long k, long flags)
{
    if(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags, CC_32F);
}
long lapack_gemm64fc(const double *src1, size_t src1_step, const double *src2, size_t src2_step, double alpha,
                   const double *src3, size_t src3_step, double beta, double *dst, size_t dst_step, long m, long n, long k, long flags)
{
    if(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags, CC_64F);
}

#endif
