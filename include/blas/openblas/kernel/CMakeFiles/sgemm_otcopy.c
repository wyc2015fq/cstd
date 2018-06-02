#undef SINGLE
#undef ASMNAME
#undef ASMFNAME
#undef NAME
#undef CNAME
#undef CHAR_NAME
#undef CHAR_CNAME

#define SINGLE
#define ASMNAME _sgemm_otcopy
#define ASMFNAME _sgemm_otcopy_
#define NAME sgemm_otcopy_
#define CNAME sgemm_otcopy
#define CHAR_NAME "sgemm_otcopy_"
#define CHAR_CNAME "sgemm_otcopy"
#include "blas/OpenBLAS/kernel/x86_64/../generic/gemm_tcopy_2.c"