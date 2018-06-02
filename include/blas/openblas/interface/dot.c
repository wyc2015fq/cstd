


#ifndef CBLAS

FLOATRET NAME(blasint *N, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY){

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  BLASLONG incy = *INCY;
  FLOATRET ret;

  PRINT_DEBUG_NAME;

  if (n <= 0) return 0.;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx;
  if (incy < 0) y -= (n - 1) * incy;

  ret = (FLOATRET)DOTU_K(n, x, incx, y, incy);

  FUNCTION_PROFILE_END(1, 2 * n, 2 * n);

  IDEBUG_END;

  return ret;
}

#else

FLOAT CNAME(blasint n, FLOAT *x, blasint incx, FLOAT *y, blasint incy){

  FLOAT ret;

  PRINT_DEBUG_CNAME;

  if (n <= 0) return 0.;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx;
  if (incy < 0) y -= (n - 1) * incy;

  ret = DOTU_K(n, x, incx, y, incy);

  FUNCTION_PROFILE_END(1, 2 * n, 2 * n);

  IDEBUG_END;

  return ret;

}

#endif
