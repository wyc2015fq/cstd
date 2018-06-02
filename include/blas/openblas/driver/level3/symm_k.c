



#undef TIMING

#ifndef RSIDE
#ifndef LOWER
#define ICOPY_OPERATION(M, N, A, LDA, X, Y, BUFFER) SYMM_IUTCOPY(M, N, A, LDA, Y, X, BUFFER);
#else
#define ICOPY_OPERATION(M, N, A, LDA, X, Y, BUFFER) SYMM_ILTCOPY(M, N, A, LDA, Y, X, BUFFER);
#endif
#endif

#ifdef RSIDE
#ifndef LOWER
#define OCOPY_OPERATION(M, N, A, LDA, X, Y, BUFFER) SYMM_OUTCOPY(M, N, A, LDA, Y, X, BUFFER);
#else
#define OCOPY_OPERATION(M, N, A, LDA, X, Y, BUFFER) SYMM_OLTCOPY(M, N, A, LDA, Y, X, BUFFER);
#endif
#endif

#ifndef RSIDE
#define K		args -> m
#ifndef LOWER
#define GEMM_LOCAL    SYMM_LU
#else
#define GEMM_LOCAL    SYMM_LL
#endif
#else
#define K		args -> n
#ifndef LOWER
#define GEMM_LOCAL    SYMM_RU
#else
#define GEMM_LOCAL    SYMM_RL
#endif
#endif

#ifdef THREADED_LEVEL3
#include "level3_thread.c"
#else
#include "level3.c"
#endif
