


#ifndef CBLAS

double NAME(blasint *N, float *x, blasint *INCX, float *y, blasint *INCY){

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  BLASLONG incy = *INCY;
  double ret = 0.0;

  PRINT_DEBUG_NAME;

  if (n <= 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx;
  if (incy < 0) y -= (n - 1) * incy;

  ret=DSDOT_K(n, x, incx, y, incy);

  FUNCTION_PROFILE_END(1, n, n);

  IDEBUG_END;

  return ret;

}

#else

double CNAME(blasint n, float *x, blasint incx, float *y, blasint incy){

  double ret = 0.0;

  PRINT_DEBUG_CNAME;

  if (n <= 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx;
  if (incy < 0) y -= (n - 1) * incy;

  ret=DSDOT_K(n, x, incx, y, incy);

  FUNCTION_PROFILE_END(1, n, n);

  IDEBUG_END;

  return ret;

}

#endif
