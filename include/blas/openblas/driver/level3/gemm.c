
#include "../../typedef.h"


#undef  TIMING

#ifdef PARAMTEST
#undef GEMM_P
#undef GEMM_Q
#undef GEMM_R

#define GEMM_P	(args -> gemm_p)
#define GEMM_Q	(args -> gemm_q)
#define GEMM_R	(args -> gemm_r)
#endif

#if 0
#undef GEMM_P
#undef GEMM_Q

#define GEMM_P 504
#define GEMM_Q 128
#endif

#ifdef THREADED_LEVEL3
#include "level3_thread.c"
#else
#include "level3.c"
#endif

#undef ASMNAME
#undef ASMFNAME
#undef NAME
#undef CNAME
#undef CHAR_NAME
#undef CHAR_CNAME
#undef TT
#undef NN
#undef NT
#undef TN

#undef DOUBLE
#undef XDOUBLE
#undef COMPLEX

