#undef ASMFNAME
#undef ASMNAME
#undef CHAR_CNAME
#undef CHAR_NAME
#undef CNAME
#undef DOUBLE
#undef NAME

#define ASMNAME _dgemm_oncopy
#define ASMFNAME _dgemm_oncopy_
#define NAME dgemm_oncopy_
#define CNAME dgemm_oncopy
#define CHAR_NAME "dgemm_oncopy_"
#define CHAR_CNAME "dgemm_oncopy"
#define DOUBLE
#include "blas/OpenBLAS/kernel/x86_64/../generic/gemm_ncopy_2.c"
#undef DOUBLE
