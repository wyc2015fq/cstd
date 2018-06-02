#define ASMNAME _zgemm_kernel
#define ASMFNAME _zgemm_kernel_
#define NAME zgemm_kernel_
#define CNAME zgemm_kernel
#define CHAR_NAME "zgemm_kernel_"
#define CHAR_CNAME "zgemm_kernel"
#define DOUBLE
#define COMPLEX
#include "blas/OpenBLAS/kernel/x86_64/../generic/zgemmkernel_2x2.c"