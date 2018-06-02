


#ifndef CBLAS

FLOATRET NAME(blasint *N, FLOAT *x, blasint *INCX){

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  FLOATRET ret;

  PRINT_DEBUG_NAME;

  if (n <= 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  ret = (FLOATRET)ASUM_K(n, x, incx);

  FUNCTION_PROFILE_END(COMPSIZE, n, n);

  IDEBUG_END;

  return ret;
}

#else

FLOAT CNAME(blasint n, FLOAT *x, blasint incx){

  FLOAT ret;

  PRINT_DEBUG_CNAME;

  if (n <= 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  ret = ASUM_K(n, x, incx);

  FUNCTION_PROFILE_END(COMPSIZE, n, n);

  IDEBUG_END;

  return ret;
}

#endif
