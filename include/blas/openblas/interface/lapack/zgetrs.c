


#ifdef XDOUBLE
#define ERROR_NAME "XGETRS"
#elif defined(DOUBLE)
#define ERROR_NAME "ZGETRS"
#else
#define ERROR_NAME "CGETRS"
#endif

static blasint (*getrs_single[])(blas_arg_t *, BLASLONG *, BLASLONG *, FLOAT *, FLOAT *, BLASLONG) ={
  GETRS_N_SINGLE, GETRS_T_SINGLE, GETRS_R_SINGLE, GETRS_C_SINGLE,
};

#ifdef SMP
static blasint (*getrs_parallel[])(blas_arg_t *, BLASLONG *, BLASLONG *, FLOAT *, FLOAT *, BLASLONG) ={
  GETRS_N_PARALLEL, GETRS_T_PARALLEL, GETRS_R_PARALLEL, GETRS_C_PARALLEL,
};
#endif

int NAME(char *TRANS, blasint *N, blasint *NRHS, FLOAT *a, blasint *ldA,
	    blasint *ipiv, FLOAT *b, blasint *ldB, blasint *Info){

  char trans_arg = *TRANS;

  blas_arg_t args;

  blasint info;
  int trans;
  FLOAT *buffer;
#ifdef PPC440
  extern
#endif
  FLOAT *sa, *sb;

  PRINT_DEBUG_NAME;

  args.m    = *N;
  args.n    = *NRHS;
  args.a    = (void *)a;
  args.lda  = *ldA;
  args.b    = (void *)b;
  args.ldb  = *ldB;
  args.c    = (void *)ipiv;

  info = 0;

  TOUPPER(trans_arg);
  trans = -1;

  if (trans_arg == 'N') trans = 0;
  if (trans_arg == 'T') trans = 1;
  if (trans_arg == 'R') trans = 2;
  if (trans_arg == 'C') trans = 3;

  if (args.ldb  < MAX(1, args.m)) info = 8;
  if (args.lda  < MAX(1, args.m)) info = 5;
  if (args.n    < 0) info = 3;
  if (args.m    < 0) info = 2;
  if (trans     < 0) info = 1;

  if (info != 0) {
    BLASFUNC(xerbla)(ERROR_NAME, &info, sizeof(ERROR_NAME));
    return 0;
  }

  args.alpha = NULL;
  args.beta  = NULL;

  *Info = info;

  if (args.m == 0 || args.n == 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

#ifndef PPC440
  buffer = (FLOAT *)blas_memory_alloc(1);

  sa = (FLOAT *)((BLASLONG)buffer + GEMM_OFFSET_A);
  sb = (FLOAT *)(((BLASLONG)sa + ((GEMM_P * GEMM_Q * COMPSIZE * SIZE + GEMM_ALIGN) & ~GEMM_ALIGN)) + GEMM_OFFSET_B);
#endif

#ifdef SMP
  args.nthreads = num_cpu_avail(4);

  if (args.nthreads == 1) {
#endif

    (getrs_single[trans])(&args, NULL, NULL, sa, sb, 0);

#ifdef SMP
  } else {

    (getrs_parallel[trans])(&args, NULL, NULL, sa, sb, 0);

  }
#endif

#ifndef PPC440
  blas_memory_free(buffer);
#endif

  FUNCTION_PROFILE_END(COMPSIZE * COMPSIZE, args.m * args.n, 2 * args.m * args.m * args.n);

  IDEBUG_END;

  return 0;

}
