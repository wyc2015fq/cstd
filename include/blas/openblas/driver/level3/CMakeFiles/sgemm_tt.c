
#undef TT
#undef ASMNAME
#undef ASMFNAME
#undef NAME
#undef CNAME
#undef CHAR_NAME
#undef CHAR_CNAME

#define TT
#define ASMNAME _sgemm_tt
#define ASMFNAME _sgemm_tt_
#define NAME sgemm_tt_
#define CNAME sgemm_tt
#define CHAR_NAME "sgemm_tt_"
#define CHAR_CNAME "sgemm_tt"
#include "blas/OpenBLAS/driver/level3/gemm.c"

