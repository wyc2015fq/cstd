


#ifndef CBLAS

void NAME(blasint *N, FLOAT *ALPHA, FLOAT *x, blasint *INCX){

  blasint n    = *N;
  blasint incx = *INCX;

#ifndef SSCAL
  FLOAT *alpha=ALPHA;
#else
  FLOAT alpha[2] = {ALPHA[0], ZERO};
#endif

#else

#ifndef SSCAL
void CNAME(blasint n, FLOAT *ALPHA, FLOAT *x, blasint incx){

  FLOAT *alpha=ALPHA;
#else
void CNAME(blasint n, FLOAT alpha_r, FLOAT *x, blasint incx){

  FLOAT alpha[2] = {alpha_r, ZERO};
#endif
#endif

#ifdef SMP
  int mode;
  int nthreads;
#endif

#ifndef CBLAS
  PRINT_DEBUG_NAME;
#else
  PRINT_DEBUG_CNAME;
#endif

  if (incx <= 0 || n <= 0) return;

  if ((alpha[0] == ONE) && (alpha[1] == ZERO)) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

#ifdef SMP
  nthreads = num_cpu_avail(1);

  if ( n <= 1048576 )
	nthreads = 1;

  if (nthreads == 1) {
#endif

  SCAL_K(n, 0, 0, alpha[0], alpha[1], x, incx, NULL, 0, NULL, 0);

#ifdef SMP
  } else {
#ifdef DOUBLE
    mode  =  BLAS_DOUBLE | BLAS_COMPLEX;
#else
    mode  =  BLAS_SINGLE | BLAS_COMPLEX;
#endif

    blas_level1_thread(mode, n, 0, 0,  alpha, x, incx, NULL, 0, NULL, 0, (void *)SCAL_K, nthreads);

  }
#endif

  FUNCTION_PROFILE_END(4, n, n);

  IDEBUG_END;

  return;

}
