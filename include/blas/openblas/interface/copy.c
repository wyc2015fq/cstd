


#ifndef CBLAS

void NAME(blasint *N, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY){

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  BLASLONG incy = *INCY;

  PRINT_DEBUG_NAME;

#else

void CNAME(blasint n, FLOAT *x, blasint incx, FLOAT *y, blasint incy){

  PRINT_DEBUG_CNAME;

#endif

  if (n <= 0) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx * COMPSIZE;
  if (incy < 0) y -= (n - 1) * incy * COMPSIZE;

  COPY_K(n, x, incx, y, incy);

  FUNCTION_PROFILE_END(COMPSIZE, COMPSIZE * n, 0);

  IDEBUG_END;

  return;

}
