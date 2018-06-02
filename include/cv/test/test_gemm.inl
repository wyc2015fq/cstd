
#include "cstd.h"
#include <lib/OpenBLAS/cblas.h>
#pragma comment(lib,"libopenblas.lib")


#include "cv/cv.inl"

// C=alpha*A*B+beta*C 
CC_IMPL void
cblas_gemm(const img_t* A, const img_t* B, double alpha, img_t* C, double beta, img_t* D, int flags CC_DEFAULT(0)) {
  enum CBLAS_ORDER Order = CblasRowMajor;
  enum CBLAS_TRANSPOSE TransA = flags & CC_GEMM_A_T ? CblasTrans : CblasNoTrans;
  enum CBLAS_TRANSPOSE TransB = flags & CC_GEMM_B_T ? CblasTrans : CblasNoTrans;
  int m = A->h, p = A->w, n = B->w;
  if (beta==0) {
    C = NULL;
  }
  if (C) {
    cvCopy(C, D);
  } else {
    cvZero(D);
  }
  if (CC_32F==A->tid) {
    typedef float arrtype;
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, p,
      (arrtype)alpha, (arrtype*)A->tt.data, A->s/sizeof(arrtype),
      (arrtype*)B->tt.data, B->s/sizeof(arrtype), (arrtype)beta,
      (arrtype*)D->tt.data, D->s/sizeof(arrtype));
  } else if (CC_64F==A->tid) {
    typedef double arrtype;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, p,
      (arrtype)alpha, (arrtype*)A->tt.data, A->s/sizeof(arrtype),
      (arrtype*)B->tt.data, B->s/sizeof(arrtype), (arrtype)beta,
      (arrtype*)D->tt.data, D->s/sizeof(arrtype));
  } else {
    ASSERT(0);
  }
}
// OpenCV的gemm与OpenBlas的对比
int test_gemm() {
  enum {LEN = 30};
  double timeOpenCV[LEN] = {0};
  double timeOpenBlas[LEN] = {0};

  double alpha = 1.f, gamma = 0;
  int i=0, factor = 1;
  img_t im[10] = {0};
  img_t* A = im+i++;
  img_t* B = im+i++;
  img_t* C = im+i++;
  img_t* D = im+i++;
  TypeId type = CC_32F;
  CRNG rng = 10;
  printf("%s\n", __DATE__);
  printf("%s\n", __TIME__);

  for (i = 0; i < LEN; i++) {
    factor += 10;
    int m = 5 * factor, p = 10 * factor, n = 5 * factor;
    //m=p=n=500;
    //m=1;    p=2;    n=1;
    cvSetMat(A, m, p, type);
    cvSetMat(B, p, n, type);
    cvSetMat(C, m, n, type);
    cvSetMat(D, m, n, type);
    cvSetZero(C);
    cvSetZero(D);
    
    cvRandArr(&rng, A, CC_RAND_NORMAL, cScalar(1, 0, 0, 0), cScalar(100, 0, 0, 0));//mean=1,stddev=100
    cvRandArr(&rng, B, CC_RAND_NORMAL, cScalar(2, 0, 0, 0), cScalar(50, 0, 0, 0));
    
    utime_start(t1);
    cvGEMM(A, B, alpha, C, gamma, C);
    timeOpenCV[i] = utime_elapsed(t1);

    utime_start(t2);
    cblas_gemm(A, B, alpha, D, gamma, D);
    timeOpenBlas[i] = utime_elapsed(t2);
    double d = cvNorm(C, D, CC_L2)/(m*n);
    if (0) {
      cvOutMat(stdout, A);
      cvOutMat(stdout, B);
      cvOutMat(stdout, C);
      cvOutMat(stdout, D);
    }
    printf("%8.5f %8.5f %8.5f\n ", timeOpenCV[i], timeOpenBlas[i], d);
  }
  imfrees2(im);
  for (i = 0; i < LEN; i++){
    //printf("%f %f\n ", timeOpenCV[i], timeOpenBlas[i]);
  }
  printf("\n'");
  return 0;
}