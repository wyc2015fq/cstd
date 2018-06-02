


#ifdef SMP
#ifdef __64BIT__
#define SMPTEST 1
#endif
#endif

#ifdef XDOUBLE
#define ERROR_NAME "QGER  "
#elif defined DOUBLE
#define ERROR_NAME "DGER  "
#else
#define ERROR_NAME "SGER  "
#endif

#define GER		GERU_K

#if   defined XDOUBLE
#define GER_THREAD	qger_thread
#elif defined DOUBLE
#define GER_THREAD	dger_thread
#else
#define GER_THREAD	sger_thread
#endif


#ifndef CBLAS

void NAME(blasint *M, blasint *N, FLOAT *Alpha,
	  FLOAT *x, blasint *INCX,
	  FLOAT *y, blasint *INCY,
	  FLOAT *a, blasint *LDA){

  blasint    m     = *M;
  blasint    n     = *N;
  FLOAT  alpha = *Alpha;
  blasint    incx  = *INCX;
  blasint    incy  = *INCY;
  blasint    lda   = *LDA;
  FLOAT *buffer;
#ifdef SMPTEST
  int nthreads;
#endif

  blasint info;

  PRINT_DEBUG_NAME;

  info = 0;

  if (lda < MAX(1,m)) info = 9;
  if (incy == 0)      info = 7;
  if (incx == 0)      info = 5;
  if (n < 0)          info = 2;
  if (m < 0)          info = 1;

  if (info){
    BLASFUNC(xerbla)(ERROR_NAME, &info, sizeof(ERROR_NAME));
    return;
  }

#else

void CNAME(enum CBLAS_ORDER order,
	   blasint m, blasint n,
	   FLOAT alpha,
	   FLOAT  *x, blasint incx,
	   FLOAT  *y, blasint incy,
	   FLOAT  *a, blasint lda) {

  FLOAT *buffer;
  blasint info, t;
#ifdef SMPTEST
  int nthreads;
#endif

  PRINT_DEBUG_CNAME;

  info  =  0;

  if (order == CblasColMajor) {
    info = -1;

    if (lda < MAX(1,m)) info = 9;
    if (incy == 0)      info = 7;
    if (incx == 0)      info = 5;
    if (n < 0)          info = 2;
    if (m < 0)          info = 1;
  }

  if (order == CblasRowMajor) {
    info = -1;

    t = n;
    n = m;
    m = t;

    t    = incx;
    incx = incy;
    incy = t;

    buffer = x;
    x = y;
    y = buffer;

    if (lda < MAX(1,m)) info = 9;
    if (incy == 0)      info = 7;
    if (incx == 0)      info = 5;
    if (n < 0)          info = 2;
    if (m < 0)          info = 1;
  }

  if (info >= 0) {
    BLASFUNC(xerbla)(ERROR_NAME, &info, sizeof(ERROR_NAME));
    return;
  }

#endif

  /*     Quick return if possible. */
  if (m == 0 || n == 0) return;
  if (alpha == 0.) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incy < 0) y -= (n - 1) * incy;
  if (incx < 0) x -= (m - 1) * incx;

  STACK_ALLOC(m, FLOAT, buffer);

#ifdef SMPTEST
  // Threshold chosen so that speed-up is > 1 on a Xeon E5-2630
  if(1L * m * n > 2048L * GEMM_MULTITHREAD_THRESHOLD)
    nthreads = num_cpu_avail(2);
  else
    nthreads = 1;

  if (nthreads == 1) {
#endif

    GER(m, n, 0, alpha, x, incx, y, incy, a, lda, buffer);

#ifdef SMPTEST
  } else {

    GER_THREAD(m, n, alpha, x, incx, y, incy, a, lda, buffer, nthreads);

  }
#endif

  STACK_FREE(buffer);
  FUNCTION_PROFILE_END(1, m * n + m + n, 2 * m * n);

  IDEBUG_END;

  return;
}
