


#ifndef CBLAS

void NAME(blasint *N, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY){

  blasint n    = *N;
  blasint incx = *INCX;
  blasint incy = *INCY;

#else

void CNAME(blasint n, FLOAT *x, blasint incx, FLOAT *y, blasint incy){

#endif

#ifdef SMP
  int mode;
  FLOAT dummyalpha[2] = {ZERO, ZERO};
  int nthreads;
#endif

#ifndef CBLAS
  PRINT_DEBUG_NAME;
#else
  PRINT_DEBUG_CNAME;
#endif

  if (n <= 0) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx * 2;
  if (incy < 0) y -= (n - 1) * incy * 2;

#ifdef SMP
  nthreads = num_cpu_avail(1);

  //disable multi-thread when incx==0 or incy==0
  //In that case, the threads would be dependent.
  if (incx == 0 || incy == 0)
	  nthreads = 1;

  if (nthreads == 1) {
#endif

  SWAP_K(n, 0, 0, ZERO, ZERO, x, incx, y, incy, NULL, 0);

#ifdef SMP
  } else {

#ifdef XDOUBLE
    mode  =  BLAS_XDOUBLE | BLAS_COMPLEX;
#elif defined(DOUBLE)
    mode  =  BLAS_DOUBLE  | BLAS_COMPLEX;
#else
    mode  =  BLAS_SINGLE  | BLAS_COMPLEX;
#endif

    blas_level1_thread(mode, n, 0, 0, dummyalpha,
		       x, incx, y, incy, NULL, 0, (void *)SWAP_K, nthreads);

  }
#endif

  FUNCTION_PROFILE_END(2, 2 * n, 0);

  IDEBUG_END;

  return;

}
