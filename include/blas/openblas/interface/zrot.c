


void NAME(blasint *N, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY, FLOAT *C, FLOAT *S){

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  BLASLONG incy = *INCY;
  FLOAT c = *C;
  FLOAT s = *S;

  PRINT_DEBUG_NAME;

  if (n <= 0) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * 2 * incx;
  if (incy < 0) y -= (n - 1) * 2 * incy;

  ROT_K(n, x, incx, y, incy, c, s);

  FUNCTION_PROFILE_END(4, n, n);

  IDEBUG_END;

  return;

}
