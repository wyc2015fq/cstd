#undef ASMFNAME
#undef ASMNAME
#undef CBLAS
#undef CHAR_CNAME
#undef CHAR_NAME
#undef CNAME
#undef DOUBLE
#undef NAME


#define CBLAS
#define ASMNAME _cblas_dgemm
#define ASMFNAME _cblas_dgemm_
#define NAME cblas_dgemm_
#define CNAME cblas_dgemm
#define CHAR_NAME "cblas_dgemm_"
#define CHAR_CNAME "cblas_dgemm"
#define DOUBLE
#include "blas/OpenBLAS/interface/gemm.c"

#undef ASMFNAME
#undef ASMNAME
#undef CBLAS
#undef CHAR_CNAME
#undef CHAR_NAME
#undef CNAME
#undef DOUBLE
#undef NAME
