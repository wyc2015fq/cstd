

#include "cnn4/math_functions.inl"
#include "utime.h"

// C=alpha*A*B+beta*C 

// OpenCV的gemm与OpenBlas的对比
#define CBLASFUN(NAME)  cblas_s##NAME
typedef float Dtype;
int test_gemm_kernel(int M, int N, int K, const Dtype* A, const Dtype* B, Dtype* C, Dtype* D) {
  CBLAS_TRANSPOSE TransA;
  CBLAS_TRANSPOSE TransB;
  TransA = CblasNoTrans;
  TransB = CblasNoTrans;
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  Dtype alpha = -1;
  Dtype beta = 1;
  utime_start(t1);
  //CBLASFUN(gemm)
  my_sgemm1
    (CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
  double timeOpenCV = utime_elapsed(t1);
  double s1 = my_sasum(M*N, C, 1);

  utime_start(t2);
  my_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, D, N);
  double  timeOpenBlas = utime_elapsed(t2);
  double s2 = my_sasum(M*N, D, 1);
  double d = (s1 - s2);
  if (fabs(d)>0.001) {
    for (int i = 0; i < M*N; ++i) {
      if (fabs(C[i] - D[i])>0.0001) {
        int adsf = 0;
      }
    }
  }
  printf("%8.5f %8.5f %d %d %d %8.5f %8.5f %8.5f\n", timeOpenCV, timeOpenBlas, M, N, K, d, s1, s2);
  return 0;
}
#if 0
Dtype A[] = {
#include "D:\code\git\cstd\include\cnn4\math_functions_impl.inl_A.txt"
};
Dtype B[] = {
#include "D:\code\git\cstd\include\cnn4\math_functions_impl.inl_B.txt"
};
Dtype C[] = {
#include "D:\code\git\cstd\include\cnn4\math_functions_impl.inl_C.txt"
};
Dtype D[] = {
#include "D:\code\git\cstd\include\cnn4\math_functions_impl.inl_C.txt"
};
int test_gemm1() {
  //s1	8783505.0000000000	double
  //s2	9109526.0000000000	double
  //    2373366.25000
  int K = 1;
  int M = countof(A) / K;
  int N = countof(B) / K;
  test_gemm_kernel(M, N, K, A, B, C, D);
  return 0;
}
#endif
int test_gemm() {
  //test_gemm1();
  enum {LEN = 3};
  double timeOpenCV[LEN] = {0};
  double timeOpenBlas[LEN] = {0};

  //double alpha = 1.f, gamma = 0;
  int i=0, factor = 2;
  Dtype* A = 0;
  Dtype* B = 0;
  Dtype* C = 0;
  Dtype* D = 0;
  printf("%s\n", __DATE__);
  printf("%s\n", __TIME__);

  factor += 10;
  for (i = 0; i < LEN; i++) {
    factor += 10;
    int M = 4 * factor + 3, K = 8 * factor + 3, N = 6 * factor + 3;
    //M = 128;    N = 2240;    K = 1;
    //M=K=N=500;
    //M=1;    K=2;    N=1;
    MYREALLOC(A, M*K);
    MYREALLOC(B, K*N);
    MYREALLOC(C, M*N);
    MYREALLOC(D, M*N);
    my_sset(M*N, 0, C);
    my_sset(M*N, 0, D);
#if 1
    cpu_caffe_rng_gaussian(M*K, 0, 10, A);//mean=0,stddev=1
    cpu_caffe_rng_gaussian(K*N, 0, 10, B);
    cpu_caffe_rng_gaussian(M*N, 0, 10, C);
#else
    my_sset(M*K, 1.1, A);
    my_sset(K*N, 1.2, B);
    my_sset(M*N, 1.3, B);
#endif
    memcpy(D, C, M*N * sizeof(Dtype));
    test_gemm_kernel(M, N, K, A, B, C, D);


    FREE(A);
    FREE(B);
    FREE(C);
    FREE(D);
  }
  printf("\n");
  return 0;
}