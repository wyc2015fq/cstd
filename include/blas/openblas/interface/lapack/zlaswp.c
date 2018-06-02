


static int (*laswp[])(BLASLONG, BLASLONG, BLASLONG, FLOAT, FLOAT, FLOAT *, BLASLONG, FLOAT *, BLASLONG, blasint *, BLASLONG) = {
#ifdef XDOUBLE
  xlaswp_plus, xlaswp_minus,
#elif defined(DOUBLE)
  zlaswp_plus, zlaswp_minus,
#else
  claswp_plus, claswp_minus,
#endif
};

int NAME(blasint *N, FLOAT *a, blasint *LDA, blasint *K1, blasint *K2, blasint *ipiv, blasint *INCX){

  blasint n    = *N;
  blasint lda  = *LDA;
  blasint k1   = *K1;
  blasint k2   = *K2;
  blasint incx = *INCX;
  int flag;

#ifdef SMP
  int mode;
  FLOAT dummyalpha[2] = {ZERO, ZERO};
  int nthreads;
#endif

  PRINT_DEBUG_NAME;

  if (incx == 0 || n <= 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  flag = (incx < 0);

#ifdef SMP
  nthreads = num_cpu_avail(2);

  if (nthreads == 1) {
#endif

  (laswp[flag])(n, k1, k2, ZERO, ZERO, a, lda, NULL, 0, ipiv, incx);

#ifdef SMP
  } else {

#ifdef XDOUBLE
  mode  =  BLAS_XDOUBLE | BLAS_COMPLEX;
#elif defined(DOUBLE)
  mode  =  BLAS_DOUBLE  | BLAS_COMPLEX;
#else
  mode  =  BLAS_SINGLE  | BLAS_COMPLEX;
#endif

  blas_level1_thread(mode, n, k1, k2, dummyalpha, a, lda, NULL, 0, ipiv, incx, laswp[flag], nthreads);
  }
#endif

  FUNCTION_PROFILE_END(COMPSIZE, n * (k2 - k1), 0);

  IDEBUG_END;

  return 0;
}
