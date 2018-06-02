
//#define LAPACK_COMPLEX_CUSTOM
#define LAPACK_COMPLEX_STRUCTURE
#ifdef _WIN32
#include <lib/OpenBLAS/cblas.h>
#include <lib/OpenBLAS/lapacke_config.h>
#include <lib/OpenBLAS/lapacke.h>
#else
#include <cblas.h>
#include <lapacke_config.h>
#include <lapacke.h>
#endif
#pragma comment(lib,"libopenblas.lib")

#undef cv_hal_LU32f
#define cv_hal_LU32f openblas_LU32f
#undef cv_hal_LU64f
#define cv_hal_LU64f openblas_LU64f

#undef cv_hal_Cholesky32f
#define cv_hal_Cholesky32f openblas_Cholesky32f
#undef cv_hal_Cholesky64f
#define cv_hal_Cholesky64f openblas_Cholesky64f


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

static inline int
openblas_LU(void* a, size_t a_step, int m, void* b, size_t b_step, int n, int* info, TypeId type)
{
    int elem_size = CC_ELEM_SIZE(type);
    int lda = a_step / elem_size, sign = 0;
    int ldb = b_step / elem_size;
    int* piv = new int[m];

    if(b)
    {
        if(n == 1 && b_step == elem_size)
        {
            if(type == CC_32F)
                *info = LAPACKE_sgesv(CblasRowMajor, m, n, (float*)a, lda, piv, (float*)b, ldb);
            else if(type == CC_64F)
                *info = LAPACKE_dgesv(CblasRowMajor, m, n, (double*)a, lda, piv, (double*)b, ldb);
        }
        else
        {

            if(type == CC_32F) {
                *info = LAPACKE_sgesv(CblasRowMajor, m, n, (float*)a, lda, piv, (float*)b, ldb);
            } else if(type == CC_64F) {
                *info = LAPACKE_dgesv(CblasRowMajor, m, n, (double*)a, lda, piv, (double*)b, ldb);
            }
        }
    }
    else
    {
        if(type == CC_32F)
            *info = LAPACKE_sgetrf(CblasRowMajor, m, m, (float*)a, lda, piv);
        else if(type == CC_64F)
            *info = LAPACKE_dgetrf(CblasRowMajor, m, m, (double*)a, lda, piv);
    }

    if(*info == 0)
    {
        for(int i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    else
        *info = 0; //in opencv LU function zero means error

    delete[] piv;
    return CC_HAL_ERROR_OK;
}

int openblas_LU32f(float* a, size_t a_step, int m, float* b, size_t b_step, int n, int* info)
{
    if(m < HAL_LU_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_LU(a, a_step, m, b, b_step, n, info, CC_32F);
}

int openblas_LU64f(double* a, size_t a_step, int m, double* b, size_t b_step, int n, int* info)
{
    if(m < HAL_LU_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_LU(a, a_step, m, b, b_step, n, info, CC_64F);
}

static inline int
openblas_Cholesky(void* a, size_t a_step, int m, void* b, size_t b_step, int n, int* info, TypeId type)
{
    int elem_size = CC_ELEM_SIZE(type);
    int lapackStatus = 0;
    int lda = a_step / elem_size;
    int ldb = b_step / elem_size;
    char L = 'L';

    if(b)
    {
        if(n == 1 && b_step == elem_size)
        {
            if(type == CC_32F)
                *info = LAPACKE_sposv(CblasRowMajor, L, m, n, (float*)a, lda, (float*)b, ldb);
            else if(type == CC_64F)
                *info = LAPACKE_dposv(CblasRowMajor, L, m, n, (double*)a, lda, (double*)b, ldb);
        }
        else
        {
            if(type == CC_32F)
                *info = LAPACKE_sposv(CblasRowMajor, L, m, n, (float*)a, lda, (float*)b, ldb);
            else if(type == CC_64F)
                *info = LAPACKE_dposv(CblasRowMajor, L, m, n, (double*)a, lda, (double*)b, ldb);

        }
    }
    else
    {
        if(type == CC_32F)
            *info = LAPACKE_spotrf(CblasRowMajor, L, m, (float*)a, lda);
        else if(type == CC_64F)
            *info = LAPACKE_dpotrf(CblasRowMajor, L, m, (double*)a, lda);
    }

    if(lapackStatus == 0) *info = true;
    else *info = false; //in opencv Cholesky function false means error

    return CC_HAL_ERROR_OK;
}

int openblas_Cholesky32f(float* a, size_t a_step, int m, float* b, size_t b_step, int n, int *info)
{
    //printf("openblas_Cholesky32f\n");
    //if(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_Cholesky(a, a_step, m, b, b_step, n, info, CC_32F);
}

int openblas_Cholesky64f(double* a, size_t a_step, int m, double* b, size_t b_step, int n, int *info)
{
    if(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_Cholesky(a, a_step, m, b, b_step, n, info, CC_64F);
}

static inline int
openblas_SVD(void* a, size_t a_step, void* w, void* u, size_t u_step, void* vt, size_t v_step, int m, int n, int flags, int* info, TypeId type)
{
    int elem_size = CC_ELEM_SIZE(type);
    int lda = a_step / elem_size;
    int ldv = v_step / elem_size;
    int ldu = u_step / elem_size;
    //int* iworkBuf = MALLOC(int, 8*MIN(m, n));
    //int lwork = -1;
    double work1 = 0;

    //A already transposed and m>=n
    char mode = ' ';
    if(flags & CC_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode = 'N';
    }
    else if((flags & CC_HAL_SVD_SHORT_UV) && (flags & CC_HAL_SVD_MODIFY_A)) //short CSVD, U stored in a
        mode = 'O';
    else if((flags & CC_HAL_SVD_SHORT_UV) && !(flags & CC_HAL_SVD_MODIFY_A)) //short CSVD, U stored in u if m>=n
        mode = 'S';
    else if(flags & CC_HAL_SVD_FULL_UV) //full CSVD, U stored in u or in a
        mode = 'A';

    if((flags & CC_HAL_SVD_MODIFY_A) && (flags & CC_HAL_SVD_FULL_UV)) //U stored in a
    {
        u = MALLOC(uchar, m*m*elem_size);
        ldu = m;
    }

    if(type == CC_32F)
        *info = LAPACKE_sgesdd(CblasRowMajor, mode, m, n, (float*)a, lda, (float*)w, (float*)u, ldu, (float*)vt, ldv);
    else if(type == CC_64F)
        *info = LAPACKE_dgesdd(CblasRowMajor, mode, m, n, (double*)a, lda, (double*)w, (double*)u, ldu, (double*)vt, ldv);

    //lwork = (int)ROUND(work1); //optimal buffer size
    //uchar* buffer = MALLOC(uchar, (lwork + 1) * elem_size);

    if(type == CC_32F)
        *info = LAPACKE_sgesdd(CblasRowMajor, mode, m, n, (float*)a, lda, (float*)w, (float*)u, ldu, (float*)vt, ldv);
    else if(type == CC_64F)
        *info = LAPACKE_dgesdd(CblasRowMajor, mode, m, n, (double*)a, lda, (double*)w, (double*)u, ldu, (double*)vt, ldv);

    if(!(flags & CC_HAL_SVD_NO_UV)) {
      if(type == CC_32F) {
        transpose_square_inplace((float*)vt, ldv, n);
      } else if(type == CC_64F) {
        transpose_square_inplace((double*)vt, ldv, n);
      }
    } else {

    }

    if((flags & CC_HAL_SVD_MODIFY_A) && (flags & CC_HAL_SVD_FULL_UV))
    {
      if(type == CC_32F) {
        float* fa = (float*)a;
        float* fu = (float*)u;
        for(int i = 0; i < m; i++)
            for(int j = 0; j < m; j++)
                fa[i*lda + j] = fu[i*m + j];
      } else if(type == CC_64F) {
        double* da = (double*)a;
        double* du = (double*)u;
        for(int i = 0; i < m; i++)
            for(int j = 0; j < m; j++)
                da[i*lda + j] = du[i*m + j];

      }
        FREE(u);
    }

    //FREE(iworkBuf);
    //FREE(buffer);
    return CC_HAL_ERROR_OK;
}

#if 0
#undef cv_hal_SVD32f
#define cv_hal_SVD32f openblas_SVD32f
#undef cv_hal_SVD64f
#define cv_hal_SVD64f openblas_SVD64f

#undef cv_hal_QR32f
#define cv_hal_QR32f openblas_QR32f
#undef cv_hal_QR64f
#define cv_hal_QR64f openblas_QR64f

#undef cv_hal_gemm32f
#define cv_hal_gemm32f openblas_gemm32f
#undef cv_hal_gemm64f
#define cv_hal_gemm64f openblas_gemm64f
#undef cv_hal_gemm32fc
#define cv_hal_gemm32fc openblas_gemm32fc
#undef cv_hal_gemm64fc
#define cv_hal_gemm64fc openblas_gemm64fc

template <typename fptype> static inline int
openblas_QR(fptype* a, size_t a_step, int m, int n, int k, fptype* b, size_t b_step, fptype* dst, int* info)
{
    int lda = a_step / elem_size;
    char mode[] = { 'N', '\0' };
    if(m < n)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;
    int ldtmpA;
    if (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    else
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    int lwork = -1;
    fptype work1 = 0.;

    if (b)
    {
        if (k == 1 && b_step == elem_size)
        {
            if (type == CC_32F)
                sgels(mode, m, n, &k, (float*)tmpA, ldtmpA, (float*)b, &m, (float*)&work1, &lwork, info, CC_32F);
            else if (type == CC_64F)
                dgels(mode, m, n, &k, (double*)tmpA, ldtmpA, (double*)b, &m, (double*)&work1, &lwork, info, CC_32F);

            lwork = (int)round(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            if (type == CC_32F)
                sgels(mode, m, n, &k, (float*)tmpA, ldtmpA, (float*)b, &m, (float*)buffer, &lwork, info, CC_32F);
            else if (type == CC_64F)
                dgels(mode, m, n, &k, (double*)tmpA, ldtmpA, (double*)b, &m, (double*)buffer, &lwork, info, CC_32F);
        }
        else
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            int ldb = b_step / elem_size;
            transpose(b, ldb, b, ldb, m, k);

            if (type == CC_32F)
                sgels(mode, m, n, &k, (float*)tmpA, ldtmpA, (float*)tmpB, &m, (float*)&work1, &lwork, info, CC_32F);
            else if (type == CC_64F)
                dgels(mode, m, n, &k, (double*)tmpA, ldtmpA, (double*)tmpB, &m, (double*)&work1, &lwork, info, CC_32F);

            lwork = (int)round(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            if (type == CC_32F)
                sgels(mode, m, n, &k, (float*)tmpA, ldtmpA, (float*)tmpB, &m, (float*)buffer, &lwork, info, CC_32F);
            else if (type == CC_64F)
                dgels(mode, m, n, &k, (double*)tmpA, ldtmpA, (double*)tmpB, &m, (double*)buffer, &lwork, info, CC_32F);

            transpose(b, ldb, b, ldb, k, m);
        }
    }
    else
    {
        if (type == CC_32F)
            sgeqrf(m, n, (float*)tmpA, ldtmpA, (float*)dst, (float*)&work1, &lwork, info, CC_32F);
        else if (type == CC_64F)
            dgeqrf(m, n, (double*)tmpA, ldtmpA, (double*)dst, (double*)&work1, &lwork, info, CC_32F);

        lwork = (int)round(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        if (type == CC_32F)
            sgeqrf(m, n, (float*)tmpA, ldtmpA, (float*)dst, (float*)buffer, &lwork, info, CC_32F);
        else if (type == CC_64F)
            dgeqrf(m, n, (double*)tmpA, ldtmpA, (double*)dst, (double*)buffer, &lwork, info, CC_32F);
    }

    if (m == n)
        transpose_square_inplace(a, lda, m);
    else
        transpose(tmpA, m, a, lda, n, m);

    if (*info != 0)
        *info = 0;
    else
        *info = 1;

    return CC_HAL_ERROR_OK;
}

template <typename fptype> static inline int
openblas_gemm(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, int a_m, int a_n, int d_n, int flags)
{
    int ldsrc1 = src1_step / elem_size;
    int ldsrc2 = src2_step / elem_size;
    int ldsrc3 = src3_step / elem_size;
    int lddst = dst_step / elem_size;
    int c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

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
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        else
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    else if (src3 == dst && (flags & CC_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    else if(src3_step == 0 && beta != 0.0)
        set_value(dst, lddst, (fptype)0.0, d_m, d_n);

    if(type == CC_32F)
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float)alpha, (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float)beta, (float*)dst, lddst);
    else if(type == CC_64F)
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (double)alpha, (double*)src1, ldsrc1, (double*)src2, ldsrc2, (double)beta, (double*)dst, lddst);

    return CC_HAL_ERROR_OK;
}


template <typename fptype> static inline int
openblas_gemm_c(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, int a_m, int a_n, int d_n, int flags)
{
    int ldsrc1 = src1_step / sizeof(std::complex<fptype>);
    int ldsrc2 = src2_step / sizeof(std::complex<fptype>);
    int ldsrc3 = src3_step / sizeof(std::complex<fptype>);
    int lddst = dst_step / sizeof(std::complex<fptype>);
    int c_m, c_n, d_m;
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
int openblas_SVD32f(float* a, size_t a_step, float *w, float* u, size_t u_step, float* vt, size_t v_step, int m, int n, int flags)
{

    if(m < HAL_SVD_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    int info;
    return openblas_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info, CC_32F);
}

int openblas_SVD64f(double* a, size_t a_step, double *w, double* u, size_t u_step, double* vt, size_t v_step, int m, int n, int flags)
{

    if(m < HAL_SVD_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    int info;
    return openblas_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info, CC_64F);
}

int openblas_QR32f(float* src1, size_t src1_step, int m, int n, int k, float* src2, size_t src2_step, float* dst, int* info)
{
    if (m < HAL_QR_SMALL_MATRIX_THRESH)
      return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info, CC_32F);
}

int openblas_QR64f(double* src1, size_t src1_step, int m, int n, int k, double* src2, size_t src2_step, double* dst, int* info)
{
    if (m < HAL_QR_SMALL_MATRIX_THRESH)
      return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info, CC_64F);
}

int openblas_gemm32f(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, int m, int n, int k, int flags)
{
    if(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags, CC_32F);
}

int openblas_gemm64f(const double *src1, size_t src1_step, const double *src2, size_t src2_step, double alpha,
                   const double *src3, size_t src3_step, double beta, double *dst, size_t dst_step, int m, int n, int k, int flags)
{
    if(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags, CC_64F);
}

int openblas_gemm32fc(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, int m, int n, int k, int flags)
{
    if(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags, CC_32F);
}
int openblas_gemm64fc(const double *src1, size_t src1_step, const double *src2, size_t src2_step, double alpha,
                   const double *src3, size_t src3_step, double beta, double *dst, size_t dst_step, int m, int n, int k, int flags)
{
    if(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        return CC_HAL_ERROR_NOT_IMPLEMENTED;
    return openblas_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags, CC_64F);
}

#endif
