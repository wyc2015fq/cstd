#undef ASMFNAME
#undef ASMNAME
#undef CHAR_CNAME
#undef CHAR_NAME
#undef CNAME
#undef DOUBLE
#undef NAME
#define ASMNAME _dgemm_kernel
#define ASMFNAME _dgemm_kernel_
#define NAME dgemm_kernel_
#define CNAME dgemm_kernel
#define CHAR_NAME "dgemm_kernel_"
#define CHAR_CNAME "dgemm_kernel"
#define DOUBLE
#include "blas/OpenBLAS/kernel/x86_64/../generic/gemmkernel_2x2.c"
#undef DOUBLE
