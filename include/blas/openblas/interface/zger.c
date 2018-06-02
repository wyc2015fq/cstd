


#ifdef SMP
#ifdef __64BIT__
#define SMPTEST 1
#endif
#endif


#ifdef XDOUBLE
#ifndef CONJ
#define ERROR_NAME "XGERU  "
#else
#define ERROR_NAME "XGERC  "
#endif
#elif defined DOUBLE
#ifndef CONJ
#define ERROR_NAME "ZGERU  "
#else
#define ERROR_NAME "ZGERC  "
#endif
#else
#ifndef CONJ
#define ERROR_NAME "CGERU  "
#else
#define ERROR_NAME "CGERC "
#endif
#endif

#if   defined XDOUBLE
#ifndef CONJ
#define GER		GERU_K
#define GER_THREAD	xger_thread_U
#else
#define GER		GERC_K
#define GER_THREAD	xger_thread_C
#define GERV		GERV_K
#define GERV_THREAD	xger_thread_V
#endif
#elif defined DOUBLE
#ifndef CONJ
#define GER		GERU_K
#define GER_THREAD	zger_thread_U
#else
#define GER		GERC_K
#define GER_THREAD	zger_thread_C
#define GERV		GERV_K
#define GERV_THREAD	zger_thread_V
#endif
#else
#ifndef CONJ
#define GER		GERU_K
#define GER_THREAD	cger_thread_U
#else
#define GER		GERC_K
#define GER_THREAD	cger_thread_C
#define GERV		GERV_K
#define GERV_THREAD	cger_thread_V
#endif
#endif

#ifndef CBLAS

void NAME(blasint *M, blasint *N, FLOAT *Alpha,
	  FLOAT *x, blasint *INCX,
	  FLOAT *y, blasint *INCY,
	  FLOAT *a, blasint *LDA){

  blasint    m     = *M;
  blasint    n     = *N;
  FLOAT  alpha_r = Alpha[0];
  FLOAT  alpha_i = Alpha[1];
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
	   FLOAT *Alpha,
	   FLOAT  *x, blasint incx,
	   FLOAT  *y, blasint incy,
	   FLOAT  *a, blasint lda) {

  FLOAT  alpha_r = Alpha[0];
  FLOAT  alpha_i = Alpha[1];

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

  if ((alpha_r == 0.) && (alpha_i == 0.)) return;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incy < 0) y -= (n - 1) * incy * 2;
  if (incx < 0) x -= (m - 1) * incx * 2;

  STACK_ALLOC(2 * m, FLOAT, buffer);

#ifdef SMPTEST
  // Threshold chosen so that speed-up is > 1 on a Xeon E5-2630
  if(1L * m * n > 36L * sizeof(FLOAT) * sizeof(FLOAT) * GEMM_MULTITHREAD_THRESHOLD)
    nthreads = num_cpu_avail(2);
  else
    nthreads = 1;

  if (nthreads == 1) {
#endif

#if !defined(CBLAS) || !defined(CONJ)
  GER(m, n, 0, alpha_r, alpha_i, x, incx, y, incy, a, lda, buffer);
#else
  if (order == CblasColMajor) {
    GER(m, n, 0, alpha_r, alpha_i, x, incx, y, incy, a, lda, buffer);
  } else {
    GERV(m, n, 0, alpha_r, alpha_i, x, incx, y, incy, a, lda, buffer);
  }
#endif

#ifdef SMPTEST

  } else {

#if !defined(CBLAS) || !defined(CONJ)
      GER_THREAD(m, n, Alpha, x, incx, y, incy, a, lda, buffer, nthreads);
#else
      if (order == CblasColMajor) {
	GER_THREAD(m, n, Alpha, x, incx, y, incy, a, lda, buffer, nthreads);
      } else {
	GERV_THREAD(m, n, Alpha, x, incx, y, incy, a, lda, buffer, nthreads);
      }
#endif

  }
#endif

  STACK_FREE(buffer);

  FUNCTION_PROFILE_END(4, m * n + m + n, 2 * m * n);

  IDEBUG_END;

  return;

}
