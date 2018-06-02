



#ifndef SMP
#define blas_cpu_number 1
#else

int blas_cpu_number = 1;

int blas_get_cpu_number(void){

  return blas_cpu_number;
}
#endif

#define FIXED_PAGESIZE 4096

void *sa = NULL;
void *sb = NULL;
static double static_buffer[BUFFER_SIZE/sizeof(double)];

void *blas_memory_alloc(int numproc){

  if (sa == NULL){
#if 1
    sa = (void *)qalloc(QFAST, BUFFER_SIZE);
#else
    sa = (void *)malloc(BUFFER_SIZE);
#endif
    sb = (void *)&static_buffer[0];
  }

  return sa;
}

void blas_memory_free(void *free_area){
  return;
}

