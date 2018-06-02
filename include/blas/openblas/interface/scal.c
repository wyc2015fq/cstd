


#ifndef CBLAS

void NAME(blasint *N, FLOAT *ALPHA, FLOAT *x, blasint *INCX){

  blasint n    = *N;
  blasint incx = *INCX;
  FLOAT alpha = *ALPHA;

#else

void CNAME(blasint n, FLOAT alpha, FLOAT *x, blasint incx){

#endif

#ifdef SMP
  int mode, nthreads;
#endif

#ifndef CBLAS
  PRINT_DEBUG_NAME;
#else
  PRINT_DEBUG_CNAME;
#endif

  if (incx <= 0 || n <= 0) return;

  if (alpha == ONE) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();


#ifdef SMP
  nthreads = num_cpu_avail(1);

  if (n <= 1048576 )
	nthreads = 1;

  if (nthreads == 1) {
#endif

  SCAL_K(n, 0, 0, alpha, x, incx, NULL, 0, NULL, 0);

#ifdef SMP
  } else {

#ifdef DOUBLE
    mode  =  BLAS_DOUBLE | BLAS_REAL;
#else
    mode  =  BLAS_SINGLE | BLAS_REAL;
#endif

    blas_level1_thread(mode, n, 0, 0,
#ifndef CBLAS
		       ALPHA,
#else
		       &alpha,
#endif
		       x, incx, NULL, 0, NULL, 0, (void *)SCAL_K, nthreads);

  }
#endif

  FUNCTION_PROFILE_END(1, n, n);

  IDEBUG_END;

  return;

}
