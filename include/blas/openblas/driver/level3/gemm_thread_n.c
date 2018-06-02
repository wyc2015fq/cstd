




int CNAME(int mode, blas_arg_t *arg, BLASLONG *range_m, BLASLONG *range_n, int (*function)(), void *sa, void *sb, BLASLONG nthreads) {

  blas_queue_t queue[MAX_CPU_NUMBER];
  BLASLONG range[MAX_CPU_NUMBER + 1];

  BLASLONG width, i, num_cpu;

  if (!range_n) {
    range[0] = 0;
    i        = arg -> n;
  } else {
    range[0] = range_n[0];
    i        = range_n[1] - range_n[0];
  }

  num_cpu  = 0;

  while (i > 0){

    width  = blas_quickdivide(i + nthreads - num_cpu - 1, nthreads - num_cpu);

    i -= width;
    if (i < 0) width = width + i;

    range[num_cpu + 1] = range[num_cpu] + width;

    queue[num_cpu].mode    = mode;
    queue[num_cpu].routine = function;
    queue[num_cpu].args    = arg;
    queue[num_cpu].range_m = range_m;
    queue[num_cpu].range_n = &range[num_cpu];
#if 0   //defined(LOONGSON3A)
    queue[num_cpu].sa      = sa	+ GEMM_OFFSET_A1 * num_cpu;
    queue[num_cpu].sb      = queue[num_cpu].sa + GEMM_OFFSET_A1 * 5;
#else
	queue[num_cpu].sa      = NULL;
    queue[num_cpu].sb      = NULL;
#endif
    queue[num_cpu].next    = &queue[num_cpu + 1];
    num_cpu ++;
  }

  if (num_cpu) {
#if 0 //defined(LOONGSON3A)
    queue[0].sa = sa;
    queue[0].sb = sa + GEMM_OFFSET_A1 * 5;
#else
	queue[0].sa = sa;
	queue[0].sb = sb;
#endif
    queue[num_cpu - 1].next = NULL;

    exec_blas(num_cpu,
	      queue);
  }

  return 0;
}
