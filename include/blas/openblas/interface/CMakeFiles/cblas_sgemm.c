
#undef DOUBLE
#undef ASMNAME
#undef ASMFNAME
#undef NAME
#undef CNAME
#undef CHAR_NAME
#undef CHAR_CNAME

#define CBLAS
#define ASMNAME _cblas_sgemm
#define ASMFNAME _cblas_sgemm_
#define NAME cblas_sgemm_
#define CNAME cblas_sgemm
#define CHAR_NAME "cblas_sgemm_"
#define CHAR_CNAME "cblas_sgemm"
#include "blas/OpenBLAS/interface/gemm.c"