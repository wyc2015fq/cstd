
#include "cv/copy.inl"
#include "cv/core/copy.inl"
#include "cv/core/hal_replacement.inl"
#include "cv/core/matmul.inl"

int umem_fillrand(umem_t* a)  {
  srand(10);
  int i, n = a->datasize/sizeof(a->p[0]);
  for (i=0; i<n; ++i) {
    a->p[i] = rand()*1./RAND_MAX;
  }
  return 0;
}

char* vecAdd_progstr = 
"__kernel void vecAdd(__global float* A,"
"        __global float* B,  "
"        __global float* C)  "
"{"
"    int idx = get_global_id(0);"
"    C[idx] = A[idx] + B[idx];  "
"}"
;


void vecAdd(float* A, float* B, float* C, int n)
{
  int idx;
  for (idx=0; idx<n; ++idx) {
    C[idx] = A[idx] + B[idx];
  }
}

char* vecConv_progstr = 
"__kernel void vecConv(__global float* A,"
"        __global float* B,  "
"        __global float* C, int na, int nb)  "
"{"
"    int j, i = get_global_id(0);"
"    float s = 0;"
"    for (j=0; j<=i; ++j) {"
"      if (j<na && (i-j)<nb) {"
"        s += A[j]*B[i-j];"
"      }"
"    }"
"    C[i] = s;"
"}"
;
void vecConv(const float* A, const float* B, float* C, int na, int nb)
{
  int i, j;
  int nc = na+nb-1;
  for (i=0; i<nc; ++i) {
    float s = 0;
    for (j=0; j<=i; ++j) {
      if (j<na && (i-j)<nb) {
        s += A[j]*B[i-j];
      }
    }
    C[i] = s;
  }
}
int pBox_fillrand2(pBox* a, int h, int w, int c = 1)  {
  pBoxsetsize(a, h, w, c);
  umem_fillrand(a->data);
  umem_setu(a->data);
  return 0;
}
int test_ocl_add() {
  pBox a[10] = {0};
  pBox* a_ = a;
  pBox* A = a_++;
  pBox* B = a_++;
  pBox* C = a_++;
  pBox* D = a_++;
  double err=0;
  double err1=0;
  if (1) {
    pBox_fillrand2(A, 1280, 720, 3);
    tic;
    umem_setu(A->data);
    toc_;
  }
  return 0;
  if (1) {
    int n=200;
    pBox_fillrand2(A, n, n, 1);
    pBox_fillrand2(B, n, n, 1);
    pBox_fillrand2(C, n, n, 1);
    pBox_fillrand2(D, n, n, 1);
    size_t _globalsize[] = {n, n};
    int astep=n, ai=1, bstep=1, bi=n, cstep=n, ci=1, M=n, N=n, K=n;
    CBLAS_TRANSPOSE TransA = CblasNoTrans, TransB = CblasTrans;
    if (TransA==CblasTrans) {
      T_SWAP(int, astep, ai);
    }
    if (TransB==CblasTrans) {
      T_SWAP(int, bstep, bi);
    }
    buf_t args[] = {
      clarg(A->data->u),
      clarg(B->data->u),
      clarg(C->data->u),
      clarg(astep),
      clarg(ai),
      clarg(bstep),
      clarg(bi),
      clarg(cstep),
      clarg(ci),
      clarg(M),
      clarg(N),
      clarg(K),
    };
    ProgramSource  matMul_oclsrc(matMul_progstr);
    tic;
    matMul_oclsrc.run("matMul", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc_;
    tic;
    cblas_sgemm(CblasRowMajor, TransA, TransB, n, n, n, 1, A->data->p, n, B->data->p, n, 0, D->data->p, n);
    toc_;
    tic;
    matMul(A->data->p, B->data->p, C->data->p, astep, ai, bstep, bi, cstep, ci, M, N, K);
    toc_;
    err1 = dist_L2(C->data->p, D->data->p, n*n);
    err = umem_checku(C->data);
  }
  if (1) {
    int n=1000;
    int nc = 2*n-1;
    pBox_fillrand2(A, n, 1, 1);
    pBox_fillrand2(B, n, 1, 1);
    pBox_fillrand2(C, nc, 1, 1);
    size_t _globalsize[] = {nc};
    buf_t args[] = {clarg(A->data->u),clarg(B->data->u),clarg(C->data->u),clarg(n),clarg(n), };
    ProgramSource  vecConv_oclsrc(vecConv_progstr);
    tic;
    vecConv_oclsrc.run("vecConv", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc_;
    tic;
    vecConv(A->data->p, B->data->p, C->data->p, n, n);
    toc_;
    err = umem_checku(C->data);
  }
  if (1) {
    int n=1000;
    pBox_fillrand2(A, n, n, 1);
    pBox_fillrand2(B, n, n, 1);
    pBox_fillrand2(C, n, n, 1);
    size_t _globalsize[] = {n*n};
    buf_t args[] = {clarg(A->data->u),clarg(B->data->u),clarg(C->data->u), };
    ProgramSource  vecAdd_oclsrc(vecAdd_progstr);
    tic;
    vecAdd_oclsrc.run("vecAdd", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc_;
    tic;
    vecAdd(A->data->p, B->data->p, C->data->p, _globalsize[0]);
    toc_;
    err = umem_checku(C->data);
  }
  pBoxfrees(a, 10);
  return 0;
}

int test_ocl() {
  //imsetsize
  test_ocl_add();
  return 0;
}
