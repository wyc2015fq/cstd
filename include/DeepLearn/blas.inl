
#if 1
#include "lib/OpenBLAS/cblas.h"
//typedef CBLAS_ORDER CBLAS_LAYOUT;
#pragma comment(lib,"libopenblas.lib")
#else
#include "lib/mkl/mkl.h"
#pragma comment(lib,"mkl_rt.lib")
#pragma comment(lib,"mkl_core.lib")
#pragma comment(lib,"mkl_blas95.lib")
// C(matrix) °˚ alpha * A * B + beta * C
void cblas_sgemm(const  CBLAS_LAYOUT Layout, const  CBLAS_TRANSPOSE TransA,
    const  CBLAS_TRANSPOSE TransB, const MKL_INT M, const MKL_INT N,
    const MKL_INT K, const float alpha, const float* A,
    const MKL_INT lda, const float* B, const MKL_INT ldb,
    const float beta, float* C, const MKL_INT ldc);
#endif

void caffe_cpu_gemm_float(CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, int M, int N, int K,
    float alpha, const float* A, const float* B, float beta, float* C)
{
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
}

void caffe_cpu_gemm_double(CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, int M, int N, int K,
    double alpha, const double* A, const double* B, double beta, double* C)
{
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_dgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B,
      ldb, beta, C, N);
}

// Y=alpha * X + Y
template <typename T> inline
int axpy(int n, T alpha, const T* X, int incX, T* Y, int incY)
{
  int i = 0, ix, iy;

  /* Dereference inputs */
  if (n > 0 && alpha != 0.0) {
    if (incX == 1 && incY == 1) {
      /* code for both increments equal to 1 */
      if (1.0 == alpha) {
        memcpy(Y, X, n * sizeof(*X));
      }
      else {
        for (; i < n - 3; i += 4) {
          Y[i] += alpha * X[i];
          Y[i + 1] += alpha * X[i + 1];
          Y[i + 2] += alpha * X[i + 2];
          Y[i + 3] += alpha * X[i + 3];
        }

        for (; i < n; ++i) {
          Y[i] += alpha * X[i];
        }
      }
    }
    else {
      ix = incX >= 0 ? 0 : (1 - n) * incX;
      iy = incY >= 0 ? 0 : (1 - n) * incY;

      if (1.0 == alpha) {
        for (; i < n; ++i) {
          Y[iy] += X[ix];
          ix += incX;
          iy += incY;
        }
      }
      else {
        for (; i < n; ++i) {
          Y[iy] += alpha * X[ix];
          ix += incX;
          iy += incY;
        }
      }
    }
  }

  return 0;
}

// Y=alpha * X + beta * Y
template <typename T> inline
int axpby(int n, T alpha, const T* X, int incX, T beta, T* Y, int incY)
{
  int i = 0, ix, iy;

  /* Dereference inputs */
  if (n > 0 && alpha != 0.0) {
    if (1.0 == beta) {
      return axpy(n, alpha, X, incX, Y, incY);
    }
    else {
      if (incX == 1 && incY == 1) {
        if (1.0 == alpha) {
          for (; i < m; i += 4) {
            Y[i] = X[i] + beta * Y[i];
            Y[i + 1] = X[i + 1] + beta * Y[i + 1];
            Y[i + 2] = X[i + 2] + beta * Y[i + 2];
            Y[i + 3] = X[i + 3] + beta * Y[i + 3];
          }

          for (; i < n; ++i) {
            Y[i] = X[i] + beta * Y[i];
          }
        }
        else {
          for (; i < m; i += 4) {
            Y[i] = alpha * X[i] + beta * Y[i];
            Y[i + 1] = alpha * X[i + 1] + beta * Y[i + 1];
            Y[i + 2] = alpha * X[i + 2] + beta * Y[i + 2];
            Y[i + 3] = alpha * X[i + 3] + beta * Y[i + 3];
          }

          for (; i < n; ++i) {
            Y[i] = alpha * X[i] + beta * Y[i];
          }
        }
      }
      else {
        ix = incX >= 0 ? 0 : (1 - n) * incX;
        iy = incY >= 0 ? 0 : (1 - n) * incY;

        for (; i < n; ++i) {
          Y[iy] = alpha * X[ix] + beta * Y[iy];
          ix += incX;
          iy += incY;
        }
      }
    }
  }

  return 0;
}

//typedef enum { CblasRowMajor, CblasColMajor } BLAS_ORDER;

// X'*Y
template <typename T> inline
T dot(int n, T* X, int incX, T* Y, int incY)
{
  int i, m, n;
  T stemp;
  int ix, iy;

  stemp = 0.0;

  if (n > 0) {
    if (incX == 1 && incY == 1) { /* code for both increments equal to 1 */
      m = n - 3;

      for (i = 0; i < m; i += 4) {
        stemp += X[i] * Y[i] + X[i + 1] * Y[i + 1] + X[i + 2] * Y[i + 2] + X[i + 3] * Y[i + 3];
      }

      for (; i < n; i++) {       /* clean-up loop */
        stemp += X[i] * Y[i];
      }
    }
    else { /* code for unequal increments or equal increments not equal to 1 */
      ix = 0;
      iy = 0;

      if (incX < 0) {
        ix = (1 - n) * incX;
      }

      if (incY < 0) {
        iy = (1 - n) * incY;
      }

      for (i = 0; i < n; i++) {
        stemp += X[ix] * Y[iy];
        ix += incX;
        iy += incY;
      }
    }
  }

  return stemp;
}

typedef enum {blasRowMajor, blasColMajor} BLAS_LAYOUT;
typedef enum {blasNoTrans, blasTrans, blasConjTrans} BLAS_TRANSPOSE;
typedef enum {blasUpper, blasLower} BLAS_UPLO;
typedef enum {blasNonUnit, blasUnit} BLAS_DIAG;
typedef enum {blasLeft, blasRight} BLAS_SIDE;
typedef enum {blasPacked} BLAS_STORAGE;
typedef enum {blasAMatrix, blasBMatrix} BLAS_IDENTIFIER;

//#include "mat/matmul.inl"

#include "cv1/matmul.inl"

// alpha * x * y' + A
// Multiplies vector X by the transform of vector Y, then adds matrix A
template <typename T> inline
int ger(BLAS_LAYOUT Order, int M, int N, T alpha, const T* X, int incX, const T* Y, int incY, T* A, int lda)
{
  int i, j;
  return 0;
}
#define REP4(HH)  (HH(0), HH(1), HH(2), HH(3))
#define REP8(HH)  (HH(0), HH(1), HH(2), HH(3), HH(4), HH(5), HH(6), HH(7))

// ∆’Õ®æÿ’Ûœ‡≥À
template <typename T> inline
void mul_NN(int M, int N, int K, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc)
{
  int i, j, k;

  for (i = 0; i < M; i++) {
    T* c0 = C + i * ldc;
    const T* a0 = A + i * lda;

    for (j = 0; j < N; j++) {
      const T* b0 = B + j * ldb;
      T t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;
      k = 0;

      for (; k < K - 3; k += 4) {
#define HH(N)  (t##N += a0[k+N] * b0[k+N])
        REP4(HH);
#undef HH
      }

      for (; k < K; k++) {
        t0 += a0[k] * b0[k];
      }

      c0[j] = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    }
  }
}

// ∆’Õ®æÿ’Ûœ‡≥À
template <typename T> inline
void mul_NN1(int M, int N, int K, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc)
{
  int i, j, k;

  for (i = 0; i < M - 3; i += 4) {
    T* c0 = C + i * ldc;
    T* c1 = C + (i + 1) * ldc;
    T* c2 = C + (i + 2) * ldc;
    T* c3 = C + (i + 3) * ldc;
    const T* a0 = A + i * lda;
    const T* a1 = A + (i + 1) * lda;
    const T* a2 = A + (i + 1) * lda;
    const T* a3 = A + (i + 1) * lda;

    for (j = 0; j < N; j++) {
      const T* b0 = B + j * ldb;
      T t00 = 0, t01 = 0, t02 = 0, t03 = 0;
      T t10 = 0, t11 = 0, t12 = 0, t13 = 0;
      T t20 = 0, t21 = 0, t22 = 0, t23 = 0;
      T t30 = 0, t31 = 0, t32 = 0, t33 = 0;
      k = 0;

      for (; k < K - 3; k += 4) {
#define HH(N)  (t0##N += a0[k+N] * b0[k+N], t1##N += a1[k+N] * b0[k+N], t2##N += a2[k+N] * b0[k+N], t3##N += a3[k+N] * b0[k+N])
        REP4(HH);
#undef HH
      }

      for (; k < K; k++) {
        t00 += a0[k] * b0[k];
        t10 += a1[k] * b0[k];
        t20 += a2[k] * b0[k];
        t30 += a3[k] * b0[k];
      }

      c0[j] = t00 + t01 + t02 + t03;
      c1[j] = t10 + t11 + t12 + t13;
      c2[j] = t20 + t21 + t22 + t23;
      c3[j] = t30 + t31 + t32 + t33;
    }
  }
}

template <typename T>
void mul_NN2(int M, int N, int K, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc)
{
  int bi, bj, i, j, k;
  enum {block = 64};

  for (bi = 0; bi < M; bi += block) {
    int bm = MIN(block, M - bi);

    for (bj = 0; bj < N; bj += block) {
      int bn = MIN(block, N - bj);

      for (i = 0; i < bm; ++i) {
        T* c1 = C + (bi + i) * ldc + bj;
        const T* a1 = A + (bi + i) * lda;

        for (j = 0; j < bn; ++j) {
          const T* b1 = B + (bj + j) * ldb;
          T t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;
          k = 0;

          for (; k < K - 3; k += 4) {
#define HH(N)  (t##N += a1[k+N] * b1[k+N])
            REP4(HH);
#undef HH
          }

          for (; k < K; k++) {
            t0 += a1[k] * b1[k];
          }

          c1[j] = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
        }
      }
    }
  }
}


tpool_t gpool[1] = {0};

typedef struct {
  int M, N, K;
  int lda, ldb, ldc;
  double alpha, beta;
  const double* A;
  const double* B;
  double* C;
} mul_NN3_job_t;

int mul_NN3_job_run(void* arg)
{
  mul_NN3_job_t* job = (mul_NN3_job_t*)arg;
  mul_NN<double>(job->M, job->N, job->K, job->alpha, job->A, job->lda, job->B, job->ldb, job->beta, job->C, job->ldc);
  return 0;
}

template <typename T>
void mul_NN3(int M, int N, int K, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc)
{
  int bi, bj, i, j, k;
  enum {block = 64};
  mul_NN3_job_t* data = NULL;
  tjob_t* job = NULL;
  int njob = ((M + block - 1) / block) * ((N + block - 1) / block);
  data = (mul_NN3_job_t*)sys_malloc((sizeof(*job) + sizeof(*data)) * njob);
  job = (tjob_t*)(data + njob);

  if (!gpool->running) {
    tpool_init(gpool, 8);
  }

  i = 0;

  for (bi = 0; bi < M; bi += block) {
    int bm = MIN(block, M - bi);

    for (bj = 0; bj < N; bj += block) {
      int bn = MIN(block, N - bj);
      data[i].M = bm;
      data[i].N = bn;
      data[i].K = K;
      data[i].lda = lda;
      data[i].ldb = ldb;
      data[i].ldc = ldc;
      data[i].A = A + (bi) * lda;
      data[i].B = B + (bj) * lda;
      data[i].C = C + (bi) * ldc + bj;
      job[i].arg = data + i;
      job[i].run = mul_NN3_job_run;
      ++i;
    }
  }

  tpool_waitjobs(gpool, job, njob);
  sys_free(data);
  return ;
}
template <typename T>
void mul_NT(int M, int N, int K, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc)
{
  int i, j, k;

  for (i = 0; i < M; i++) {
    T* c1 = C + i * ldc;
    const T* a1 = A + i * lda;
    const T* b1 = B + i * ldb;

    for (j = 0; j < N; j++) {
      T sc = 0;

      for (k = 0; k < n; k++) {
        sc += a1[k] * B[k * n + j];
      }

      c1[j] = sc;
    }
  }
}
// C(matrix) °˚ alpha * A * B + beta * C
template <typename T>
void gemm(BLAS_LAYOUT Layout, BLAS_TRANSPOSE TransA, BLAS_TRANSPOSE TransB, int M, int N, int K, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc)
{
  return 0;
}
