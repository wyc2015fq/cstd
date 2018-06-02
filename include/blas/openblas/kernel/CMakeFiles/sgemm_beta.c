#undef ASMNAME
#undef ASMFNAME
#undef NAME
#undef CNAME
#undef CHAR_NAME
#undef CHAR_CNAME

#define ASMNAME _sgemm_beta
#define ASMFNAME _sgemm_beta_
#define NAME sgemm_beta_
#define CNAME sgemm_beta
#define CHAR_NAME "sgemm_beta_"
#define CHAR_CNAME "sgemm_beta"
#include "blas/OpenBLAS/kernel/x86_64/../generic/gemm_beta.c"