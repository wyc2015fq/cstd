


#ifndef CBLAS

void NAME(blasint *N, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY, FLOAT *C, FLOAT *S){

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  BLASLONG incy = *INCY;
  FLOAT c = *C;
  FLOAT s = *S;

  PRINT_DEBUG_NAME;

#else

void CNAME(blasint n, FLOAT *x, blasint incx, FLOAT *y, blasint incy, FLOAT c, FLOAT s){

  PRINT_DEBUG_CNAME;

#endif

  if (n <= 0) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx;
  if (incy < 0) y -= (n - 1) * incy;

  ROT_K(n, x, incx, y, incy, c, s);

  FUNCTION_PROFILE_END(1, n, n);

  IDEBUG_END;

  return;

}
