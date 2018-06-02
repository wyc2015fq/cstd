


#ifdef RETURN_BY_STRUCT
#ifdef XDOUBLE
#define MYTYPE myxcomplex_t
#elif defined DOUBLE
#define MYTYPE myzcomplex_t
#else
#define MYTYPE myccomplex_t
#endif
#endif

#ifndef CBLAS

#ifdef RETURN_BY_STRUCT
MYTYPE         NAME(                        blasint *N, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY) {
#elif defined RETURN_BY_STACK
void           NAME(OPENBLAS_COMPLEX_FLOAT *result, blasint *N, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY) {
#else
OPENBLAS_COMPLEX_FLOAT NAME(                        blasint *N, FLOAT *x, blasint *INCX, FLOAT *y, blasint *INCY) {
#endif

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  BLASLONG incy = *INCY;
#ifndef RETURN_BY_STACK
  OPENBLAS_COMPLEX_FLOAT ret;
#endif
#ifdef RETURN_BY_STRUCT
  MYTYPE  myret;
#endif

#ifndef RETURN_BY_STRUCT
  OPENBLAS_COMPLEX_FLOAT zero=OPENBLAS_MAKE_COMPLEX_FLOAT(0.0, 0.0);
#endif

  PRINT_DEBUG_NAME;

  if (n <= 0) {
#ifdef RETURN_BY_STRUCT
    myret.r = 0.;
    myret.i = 0.;
    return myret;
#elif defined RETURN_BY_STACK
    *result = zero;
    return;
#else
    return zero;
#endif
  }

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0) x -= (n - 1) * incx * 2;
  if (incy < 0) y -= (n - 1) * incy * 2;

#ifdef RETURN_BY_STRUCT

#ifndef CONJ
  ret = DOTU_K(n, x, incx, y, incy);
#else
  ret = DOTC_K(n, x, incx, y, incy);
#endif

  myret.r = CREAL ret;
  myret.i = CIMAG ret;

  FUNCTION_PROFILE_END(4, 2 * n, 2 * n);

  IDEBUG_END;

  return myret;

#elif defined RETURN_BY_STACK

#ifndef CONJ
  *result = DOTU_K(n, x, incx, y, incy);
#else
  *result = DOTC_K(n, x, incx, y, incy);
#endif

  FUNCTION_PROFILE_END(4, 2 * n, 2 * n);

  IDEBUG_END;

#else

#ifndef CONJ
  ret = DOTU_K(n, x, incx, y, incy);
#else
  ret = DOTC_K(n, x, incx, y, incy);
#endif

  FUNCTION_PROFILE_END(4, 2 * n, 2 * n);

  IDEBUG_END;

  return ret;

#endif

}

#else

#ifdef FORCE_USE_STACK
void           CNAME(blasint n, FLOAT *x, blasint incx, FLOAT *y, blasint incy, OPENBLAS_COMPLEX_FLOAT *result){
#else
OPENBLAS_COMPLEX_FLOAT CNAME(blasint n, FLOAT *x, blasint incx, FLOAT *y, blasint incy){

  OPENBLAS_COMPLEX_FLOAT ret;
  OPENBLAS_COMPLEX_FLOAT zero=OPENBLAS_MAKE_COMPLEX_FLOAT(0.0, 0.0);
#endif

  PRINT_DEBUG_CNAME;

  if (n <= 0) {
#ifdef FORCE_USE_STACK
    //*result = OPENBLAS_MAKE_COMPLEX_FLOAT(0.0, 0.0);
	CREAL(*result) = 0.0;
	CIMAG(*result) = 0.0;
    return;
#else
    return zero;
#endif
  }

  if (incx < 0) x -= (n - 1) * incx * 2;
  if (incy < 0) y -= (n - 1) * incy * 2;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

#ifdef FORCE_USE_STACK

#ifndef CONJ
  *result = DOTU_K(n, x, incx, y, incy);
#else
  *result = DOTC_K(n, x, incx, y, incy);
#endif

  FUNCTION_PROFILE_END(4, 2 * n, 2 * n);

  IDEBUG_END;

#else

#ifndef CONJ
  ret = DOTU_K(n, x, incx, y, incy);
#else
  ret = DOTC_K(n, x, incx, y, incy);
#endif

  FUNCTION_PROFILE_END(4, 2 * n, 2 * n);

  IDEBUG_END;

  return ret;

#endif

}

#endif
