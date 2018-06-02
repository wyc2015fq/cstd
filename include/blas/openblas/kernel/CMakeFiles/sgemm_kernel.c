
#undef ASMNAME
#undef ASMFNAME
#undef NAME
#undef CNAME
#undef CHAR_NAME
#undef CHAR_CNAME


#define ASMNAME _sgemm_kernel
#define ASMFNAME _sgemm_kernel_
#define NAME sgemm_kernel_
#define CNAME sgemm_kernel
#define CHAR_NAME "sgemm_kernel_"
#define CHAR_CNAME "sgemm_kernel"
#include "blas/OpenBLAS/kernel/x86_64/../generic/gemmkernel_2x2.c"