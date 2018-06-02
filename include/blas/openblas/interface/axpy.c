


#ifndef CBLAS

void NAME(blasint *N, FLOAT *ALPHA, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY){

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  BLASLONG incy = *INCY;
  FLOAT alpha = *ALPHA;

#else

void CNAME(blasint n, FLOAT alpha, FLOAT *x, blasint incx, FLOAT *y, blasint incy){

#endif

#ifdef SMP
  int mode, nthreads;
#endif

#ifndef CBLAS
  PRINT_DEBUG_NAME;
#else
  PRINT_DEBUG_CNAME;
#endif

  if (n <= 0) return;

  if (alpha == ZERO) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx;
  if (incy < 0) y -= (n - 1) * incy;

#ifdef SMP
  nthreads = num_cpu_avail(1);

  //disable multi-thread when incx==0 or incy==0
  //In that case, the threads would be dependent.
  if (incx == 0 || incy == 0)
	  nthreads = 1;

  //Temporarily work-around the low performance issue with small imput size &
  //multithreads.
  if (n <= 10000)
	  nthreads = 1;

  if (nthreads == 1) {
#endif

  AXPYU_K(n, 0, 0, alpha, x, incx, y, incy, NULL, 0);

#ifdef SMP
  } else {

#ifdef XDOUBLE
    mode  =  BLAS_XDOUBLE | BLAS_REAL;
#elif defined(DOUBLE)
    mode  =  BLAS_DOUBLE  | BLAS_REAL;
#else
    mode  =  BLAS_SINGLE  | BLAS_REAL;
#endif

    blas_level1_thread(mode, n, 0, 0, &alpha,
		       x, incx, y, incy, NULL, 0, (void *)AXPYU_K, nthreads);

  }
#endif

  FUNCTION_PROFILE_END(1, 2 * n, 2 * n);

  IDEBUG_END;

  return;

}
