#undef ASMFNAME
#undef ASMNAME
#undef CHAR_CNAME
#undef CHAR_NAME
#undef CNAME
#undef DOUBLE
#undef NAME
#define DOUBLE
#define ASMNAME _dgemm_otcopy
#define ASMFNAME _dgemm_otcopy_
#define NAME dgemm_otcopy_
#define CNAME dgemm_otcopy
#define CHAR_NAME "dgemm_otcopy_"
#define CHAR_CNAME "dgemm_otcopy"
#define DOUBLE
#include "blas/OpenBLAS/kernel/x86_64/../generic/gemm_tcopy_2.c"
#undef DOUBLE
