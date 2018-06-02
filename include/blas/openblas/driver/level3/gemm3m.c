



#undef TIMING

#ifdef PARAMTEST
#undef GEMM_P
#undef GEMM_Q
#undef GEMM_R

#define GEMM_P	(args -> gemm_p)
#define GEMM_Q	(args -> gemm_q)
#define GEMM_R	(args -> gemm_r)
#endif

#ifdef THREADED_LEVEL3
#include "level3_gemm3m_thread.c"
#else
#include "gemm3m_level3.c"
#endif
