#undef SINGLE
#undef ASMNAME
#undef ASMFNAME
#undef NAME
#undef CNAME
#undef CHAR_NAME
#undef CHAR_CNAME


#define SINGLE
#define ASMNAME _sgemm_oncopy
#define ASMFNAME _sgemm_oncopy_
#define NAME sgemm_oncopy_
#define CNAME sgemm_oncopy
#define CHAR_NAME "sgemm_oncopy_"
#define CHAR_CNAME "sgemm_oncopy"
#include "blas/OpenBLAS/kernel/x86_64/../generic/gemm_ncopy_2.c"