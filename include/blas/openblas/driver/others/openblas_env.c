


static int openblas_env_verbose=0;
static unsigned int openblas_env_thread_timeout=0;
static int openblas_env_block_factor=0;
static int openblas_env_openblas_num_threads=0;
static int openblas_env_goto_num_threads=0;
static int openblas_env_omp_num_threads=0;

int openblas_verbose() { return openblas_env_verbose;}
unsigned int openblas_thread_timeout() { return openblas_env_thread_timeout;}
int openblas_block_factor() { return openblas_env_block_factor;}
int openblas_num_threads_env() { return openblas_env_openblas_num_threads;}
int openblas_goto_num_threads_env() { return openblas_env_goto_num_threads;}
int openblas_omp_num_threads_env() { return openblas_env_omp_num_threads;}

void openblas_read_env() {
  int ret=0;
  env_var_t p;
  if (readenv(p,"OPENBLAS_VERBOSE")) ret = atoi(p);
  if(ret<0) ret=0;
  openblas_env_verbose=ret;

  ret=0;
  if (readenv(p,"OPENBLAS_BLOCK_FACTOR")) ret = atoi(p);
  if(ret<0) ret=0;
  openblas_env_block_factor=ret;

  ret=0;
  if (readenv(p,"OPENBLAS_THREAD_TIMEOUT")) ret = atoi(p);
  if(ret<0) ret=0;
  openblas_env_thread_timeout=(unsigned int)ret;

  ret=0;
  if (readenv(p,"OPENBLAS_NUM_THREADS")) ret = atoi(p);
  if(ret<0) ret=0;
  openblas_env_openblas_num_threads=ret;

  ret=0;
  if (readenv(p,"GOTO_NUM_THREADS")) ret = atoi(p);
  if(ret<0) ret=0;
  openblas_env_goto_num_threads=ret;

  ret=0;
  if (readenv(p,"OMP_NUM_THREADS")) ret = atoi(p);
  if(ret<0) ret=0;
  openblas_env_omp_num_threads=ret;

}


