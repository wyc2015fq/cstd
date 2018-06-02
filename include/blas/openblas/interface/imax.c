


#undef MAX_K

#ifdef USE_ABS

#ifndef USE_MIN

/* ABS & MAX */
#ifndef COMPLEX
#ifdef XDOUBLE
#define MAX_K	IQAMAX_K
#elif defined(DOUBLE)
#define MAX_K	IDAMAX_K
#else
#define MAX_K	ISAMAX_K
#endif
#else
#ifdef XDOUBLE
#define MAX_K	IXAMAX_K
#elif defined(DOUBLE)
#define MAX_K	IZAMAX_K
#else
#define MAX_K	ICAMAX_K
#endif
#endif

#else

/* ABS & MIN */
#ifndef COMPLEX
#ifdef XDOUBLE
#define MAX_K	IQAMIN_K
#elif defined(DOUBLE)
#define MAX_K	IDAMIN_K
#else
#define MAX_K	ISAMIN_K
#endif
#else
#ifdef XDOUBLE
#define MAX_K	IXAMIN_K
#elif defined(DOUBLE)
#define MAX_K	IZAMIN_K
#else
#define MAX_K	ICAMIN_K
#endif
#endif

#endif

#else

#ifndef USE_MIN

/* MAX */
#ifdef XDOUBLE
#define MAX_K	IQMAX_K
#elif defined(DOUBLE)
#define MAX_K	IDMAX_K
#else
#define MAX_K	ISMAX_K
#endif

#else

/* MIN */
#ifdef XDOUBLE
#define MAX_K	IQMIN_K
#elif defined(DOUBLE)
#define MAX_K	IDMIN_K
#else
#define MAX_K	ISMIN_K
#endif

#endif

#endif

#ifndef CBLAS

blasint NAME(blasint *N, FLOAT *x, blasint *INCX){

  BLASLONG n    = *N;
  BLASLONG incx = *INCX;
  blasint ret;

  PRINT_DEBUG_NAME;

  if (n <= 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  ret = (blasint)MAX_K(n, x, incx);

  if(ret > n) ret=n;

  FUNCTION_PROFILE_END(COMPSIZE, n, 0);

  IDEBUG_END;

  return ret;
}

#else

CBLAS_INDEX CNAME(blasint n, FLOAT *x, blasint incx){

  CBLAS_INDEX ret;

  PRINT_DEBUG_CNAME;

  if (n <= 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  ret = MAX_K(n, x, incx);

  if (ret > n) ret=n;

  if (ret) ret --;

  FUNCTION_PROFILE_END(COMPSIZE, n, 0);

  IDEBUG_END;

  return ret;
}

#endif
