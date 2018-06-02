#undef NN
#undef ASMNAME
#undef ASMFNAME
#undef NAME
#undef CNAME
#undef CHAR_NAME
#undef CHAR_CNAME
#define NN
#define ASMNAME _sgemm_nn
#define ASMFNAME _sgemm_nn_
#define NAME sgemm_nn_
#define CNAME sgemm_nn
#define CHAR_NAME "sgemm_nn_"
#define CHAR_CNAME "sgemm_nn"
#include "blas/OpenBLAS/driver/level3/gemm.c"