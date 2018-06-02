


#if defined(DOUBLE)
#define ERROR_NAME "ZGEADD "
#else
#define ERROR_NAME "CGEADD "
#endif

#ifndef CBLAS

void NAME(blasint *M, blasint *N, FLOAT *ALPHA, FLOAT *a, blasint *LDA,
		                  FLOAT *BETA,  FLOAT *c, blasint *LDC)
{

  blasint m = *M;
  blasint n = *N;
  blasint lda = *LDA;
  blasint ldc = *LDC; 

  blasint info;

  PRINT_DEBUG_NAME;

  info = 0;


  if (lda < MAX(1, m))	info = 6;
  if (ldc < MAX(1, m))	info = 8;

  if (n < 0)		info = 2;
  if (m < 0)		info = 1;

  if (info != 0){
    BLASFUNC(xerbla)(ERROR_NAME, &info, sizeof(ERROR_NAME));
    return;
  }

#else
void CNAME( enum CBLAS_ORDER order, blasint m,  blasint n,  FLOAT *ALPHA, FLOAT *a,  blasint lda, FLOAT *BETA, 
		  FLOAT *c,  blasint ldc)
{
/* 
void CNAME(enum CBLAS_ORDER order,
	   blasint m, blasint n,
	   FLOAT alpha,
	   FLOAT  *a, blasint lda,
	   FLOAT beta,
	   FLOAT  *c, blasint ldc){ */

  blasint info, t;

  PRINT_DEBUG_CNAME;

  info  =  0;

  if (order == CblasColMajor) {

    info = -1;

    if (ldc < MAX(1, m))  info = 8;
    if (lda < MAX(1, m))  info = 5;
    if (n < 0)		  info = 2;
    if (m < 0)		  info = 1;

  }

  if (order == CblasRowMajor) {
    info = -1;

    t = n;
    n = m;
    m = t;

    if (ldc < MAX(1, m))  info = 8;
    if (lda < MAX(1, m))  info = 5;
    if (n < 0)		  info = 2;
    if (m < 0)		  info = 1;
  }

  if (info >= 0) {
    BLASFUNC(xerbla)(ERROR_NAME, &info, sizeof(ERROR_NAME));
    return;
  }

#endif

  if ((m==0) || (n==0)) return;


  IDEBUG_START;

  FUNCTION_PROFILE_START();


  GEADD_K(m,n,ALPHA[0],ALPHA[1], a, lda, BETA[0], BETA[1], c, ldc); 


  FUNCTION_PROFILE_END(1, 2* m * n ,  2 * m * n);

  IDEBUG_END;

  return;

}
