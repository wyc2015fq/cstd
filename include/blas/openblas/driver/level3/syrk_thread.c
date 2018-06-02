


#include <math.h>


int CNAME(int mode, blas_arg_t *arg, BLASLONG *range_m, BLASLONG *range_n, int (*function)(), void *sa, void *sb, BLASLONG nthreads) {

  blas_queue_t queue[MAX_CPU_NUMBER];
  BLASLONG range[MAX_CPU_NUMBER + 1];

  BLASLONG width, i;
  BLASLONG n_from, n_to;
  double dnum, nf, nt, di;

  int num_cpu;
  int mask = 0;

  if (!(mode & BLAS_COMPLEX)) {

    switch (mode & BLAS_PREC) {
    case BLAS_SINGLE:
      mask = SGEMM_UNROLL_MN - 1;
      break;
    case BLAS_DOUBLE:
      mask = DGEMM_UNROLL_MN - 1;
      break;
#ifdef EXPRECISION
    case BLAS_XDOUBLE:
      mask = MAX(QGEMM_UNROLL_M, QGEMM_UNROLL_N) - 1;
      break;
#endif
    }
  } else {
    switch (mode & BLAS_PREC) {
    case BLAS_SINGLE:
      mask = CGEMM_UNROLL_MN - 1;
      break;
    case BLAS_DOUBLE:
      mask = ZGEMM_UNROLL_MN - 1;
      break;
#ifdef EXPRECISION
    case BLAS_XDOUBLE:
      mask = MAX(XGEMM_UNROLL_M, XGEMM_UNROLL_N) - 1;
      break;
#endif
    }
  }

  n_from = 0;
  n_to   = arg -> n;

  if (range_n) {
    n_from = *(range_n + 0);
    n_to   = *(range_n + 1);
  }

  if (!(mode & BLAS_UPLO)) {

    nf = (double)(n_from);
    nt = (double)(n_to);

    dnum = (nt * nt - nf * nf) / (double)nthreads;

    num_cpu  = 0;

    range[0] = n_from;
    i        = n_from;

    while (i < n_to){

      if (nthreads - num_cpu > 1) {

	di = (double)i;
	width = ((BLASLONG)( sqrt(di * di + dnum) - di) + mask) & ~mask;

	if ((width <= 0) || (width > n_to - i)) width = n_to - i;

      } else {
	width = n_to - i;
      }

      range[num_cpu + 1] = range[num_cpu] + width;

      queue[num_cpu].mode    = mode;
      queue[num_cpu].routine = function;
      queue[num_cpu].args    = arg;
      queue[num_cpu].range_m = range_m;
      queue[num_cpu].range_n = &range[num_cpu];
      queue[num_cpu].sa      = NULL;
      queue[num_cpu].sb      = NULL;
      queue[num_cpu].next    = &queue[num_cpu + 1];

      num_cpu ++;
      i += width;
    }

  } else {

    nf = (double)(arg -> n - n_from);
    nt = (double)(arg -> n - n_to);

    dnum = (nt * nt - nf * nf) / (double)nthreads;

    num_cpu  = 0;

    range[0] = n_from;
    i        = n_from;

    while (i < n_to){

      if (nthreads - num_cpu > 1) {

	di = (double)(arg -> n - i);
	width = ((BLASLONG)(-sqrt(di * di + dnum) + di) + mask) & ~mask;

	if ((width <= 0) || (width > n_to - i)) width = n_to - i;

      } else {
	width = n_to - i;
      }

      range[num_cpu + 1] = range[num_cpu] + width;

      queue[num_cpu].mode    = mode;
      queue[num_cpu].routine = function;
      queue[num_cpu].args    = arg;
      queue[num_cpu].range_m = range_m;
      queue[num_cpu].range_n = &range[num_cpu];
      queue[num_cpu].sa      = NULL;
      queue[num_cpu].sb      = NULL;
      queue[num_cpu].next    = &queue[num_cpu + 1];

      num_cpu ++;
      i += width;
    }

  }

  if (num_cpu) {
    queue[0].sa = sa;
    queue[0].sb = sb;
    queue[num_cpu - 1].next = NULL;

    exec_blas(num_cpu, queue);
  }

  return 0;
}
