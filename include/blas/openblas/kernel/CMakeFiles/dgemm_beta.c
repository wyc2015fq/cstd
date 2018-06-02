#undef ASMFNAME
#undef ASMNAME
#undef CHAR_CNAME
#undef CHAR_NAME
#undef CNAME
#undef DOUBLE
#undef NAME
#define ASMNAME _dgemm_beta
#define ASMFNAME _dgemm_beta_
#define NAME dgemm_beta_
#define CNAME dgemm_beta
#define CHAR_NAME "dgemm_beta_"
#define CHAR_CNAME "dgemm_beta"
#define DOUBLE
#include "blas/OpenBLAS/kernel/x86_64/../generic/gemm_beta.c"
#undef DOUBLE
