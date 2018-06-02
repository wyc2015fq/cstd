




int CNAME(int mode, blas_arg_t *arg, BLASLONG *range_m, BLASLONG *range_n, int (*function)(), void *sa, void *sb, BLASLONG nthreads) {

  blas_queue_t queue[MAX_CPU_NUMBER];
  BLASLONG range[MAX_CPU_NUMBER + 1];

  BLASLONG width, i, num_cpu;

  if (!range_m) {
    range[0] = 0;
    i        = arg -> m;
  } else {
    range[0] = range_m[0];
    i        = range_m[1] - range_m[0];
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
    queue[num_cpu].range_m = &range[num_cpu];
    queue[num_cpu].range_n = range_n;
    queue[num_cpu].sa      = NULL;
    queue[num_cpu].sb      = NULL;
    queue[num_cpu].next    = &queue[num_cpu + 1];
    num_cpu ++;
  }

  if (num_cpu) {
    queue[0].sa = sa;
    queue[0].sb = sb;

    queue[num_cpu - 1].next = NULL;

    exec_blas(num_cpu, queue);
  }

  return 0;
}
