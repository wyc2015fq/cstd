
#define MKL_STDCALL
#include "math/test_matmul.inl"

#include "math/mymatmul.inl"
//#include "cache_line_size.inl"
//#include "thread.h"

int test_matmul2()
{
  int n = 1021;
  int i, m = 1;
  void* buf = NULL;
  float_t* A = NULL;
  float_t* B = NULL;
  float_t* C = NULL;
  float_t* C0 = NULL;
  float_t* C1 = NULL;
  double dd;
  int sz = sizeof(float_t) * n * n;
  multialloc(&buf, &A, sz, &B, sz, &C, sz, &C0, sz, &C1, sz, NULL);
  GenerateMatrix(A, n);
  GenerateMatrix(B, n);
  //mkl_set_num_threads(0);

  ClearMatrix(C, n);

  if (1) {
    utime_start(_start_time);

    //BlockCacul(A, B, C, n, thread_num, m);
    for (i = 0; i < m; ++i) {
      caffe_cpu_gemm_double(CblasNoTrans, CblasTrans, n, n, n, 1, A, B, 0, C);
    }

    printf("%f\n", utime_elapsed(_start_time) / m);
  }

  if (0) {
    utime_start(_start_time);

    for (i = 0; i < m; ++i) {
      mymatmul(A, n, B, n, C0, n, n, n, n, 0);
    }

    printf("%f\n", utime_elapsed(_start_time) / m);
  }

  ClearMatrix(C0, n);

  if (0) {
    utime_start(_start_time);

    for (i = 0; i < m; ++i) {
      cvGEMM_64f(A, n, B, n, 1, NULL, 0, 0, C0, n, 0, n, n, n);
    }

    printf("%f\n", utime_elapsed(_start_time) / m);
  }

  if (0) {
    utime_start(_start_time);

    for (i = 0; i < m; ++i) {
      BlockCacul(A, B, C, n, 4, m);
    }

    printf("%f\n", utime_elapsed(_start_time) / m);
  }

  if (1) {
    utime_start(_start_time);

    for (i = 0; i < m; ++i) {
      mul_NN<double>(n, n, n, 1, A, n, B, n, 0, C0, n);
    }

    printf("%f\n", utime_elapsed(_start_time) / m);
  }

  if (0) {
    utime_start(_start_time);

    for (i = 0; i < m; ++i) {
      GeneralMul(A, B, C0, n);
    }

    printf("%f\n", utime_elapsed(_start_time) / m);
  }

  //PrintMatrix(C, n);
  //PrintMatrix(C0, n);
  dd = diff(C0, C, n);
  printf("Difference of two result: %f\n", dd);
  FREE(buf);
  return 0;
}
