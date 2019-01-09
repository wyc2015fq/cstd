//#include <stdlib.h> 
//#include "pthread.h" 
//#include <x86intrin.h> 
//#include <xmmintrin.h> 

int THREADS_COUNT = 4;
thread_t threads[16] = {0};

int PRINT = 0;


struct thread_matmul_t { 
  int * a;
  int * b;
  int * buffer;
  int dim;
  int index;
  int step;
};

struct thread_matmul_t params[16];
void output(int * buf, int dim) { 
  for (int i=0;i<dim;i++) { 
    for (int j=0;j<dim;j++) { 
      printf("%d ", buf[i * dim + j]);
    } 
    printf("\n");
  } 
  printf("============\n");
} 

int * multiplySimple(int* a, int* b, int dim) { 
  int * result = (int*)malloc(dim * dim * sizeof(int));
  int sum = 0;

  for (int i=0;i<dim;i++) { 
    for (int j=0;j<dim;j++) { 
      sum = 0;
      for (int k=0;k<dim;k++) { 
        sum += (a[dim * i +k] * b[dim * k + j]);
      } 
      result[dim * i + j] = sum;
    } 
  } 
  return result;
}

int multiplyPthread_thd(void *param) {
  struct thread_matmul_t *p = (struct thread_matmul_t*)param;
  int dim = p->dim;
  int index = p->index;
  int *a = p->a;
  int *b = p->b;
  int *result = p->buffer;
  int step = p->step;
  int sum = 0;

  for (int i=index;i<dim;i+=step) { 
    for (int j=0;j<dim;j+=1) { 
      sum = 0;
      // int sum = mul(a, b, i, j, dim);
      for (int k=0;k<dim;k++) {
        // printf("cal %d, %d, %d\n", i, j, k);
        sum += (a[dim * i +k] * b[dim * k + j]);
      } 
      result[dim * i + j] = sum;
    }
  }
  //sys_thread_exit(NULL);
  return 0;
} 

int *multiplyPthread(int* a, int* b, int dim) { 
  int *result = (int*)malloc(dim * dim * sizeof(int));
  int i;
  for (i=0;i<THREADS_COUNT;i++) {
    params[i].buffer = result;
    params[i].index = i;
    params[i].dim = dim;
    params[i].step = THREADS_COUNT;
    params[i].a = a;
    params[i].b = b;
    threads[i].run = multiplyPthread_thd;
    threads[i].arg = (void *)(&params[i]);
    int rc = sys_thread_create(threads+i);
  } 
  
  for(i=0;i<THREADS_COUNT;i++) { 
    sys_thread_join(threads+i);
    // printf("Completed join with thread %d status= %ld\n",i, (long)status);
  }
  return result;
}

void test_thread_matmul_impl(int dim) { 
  int * a = (int*)malloc(dim * dim * sizeof(int));
  int i, j;
  for (i=0;i<dim;i++) { 
    for (j=0;j<dim;j++) { 
      a[i * dim + j] = i * j + 1;
    } 
  } 
  int * b = (int*)malloc(dim * dim * sizeof(int));
  for (i=0;i<dim;i++) { 
    for (j=0;j<dim;j++) { 
      b[i * dim + j] = i * j + 2;
    } 
  } 
  
  double diff0, diff1;
  int * result = 0;
  int * resultPthread;
  {
    utime_start(_start_time);
    result = multiplySimple(a, b, dim);
    diff0=utime_elapsed(_start_time);
  }
  
  
  {
    utime_start(_start_time);
    resultPthread = multiplyPthread(a, b, dim);
    diff1=utime_elapsed(_start_time);
  }
  
  
  // gettimeofday(&start2,NULL);
  // int * resultPthreadSSE = multiplyPthread(a, b, dim, 1);
  // gettimeofday(&end2,NULL);
  
  if (PRINT) { 
    output(a, dim);
    output(b, dim);
    output(result, dim);
    output(resultPthread, dim);
    // output(resultPthreadSSE, dim);
  } 
  
  //diff2 = 1000000 * (end2.tv_sec-start2.tv_sec)+ end2.tv_usec-start2.tv_usec;
  
  printf("dim=%d time0=%lf time1=%lf\n", dim, diff0, diff1);
  //printf("(%d) the difference for threaded with SSE is %ld\n", dim, diff2);
  
  free(result);
  free(resultPthread);
  free(a);
  free(b);
}

int parallel_matmul(void* arg, int beg, int end) {
  return 0;
}
int test_thread_matmul() 
{
  int size[] = {4, 16, 32, 48, 64, 128, 256, 512, 1024, 2048, 4096};
  for (int i=0;i<countof(size);i++) { 
    test_thread_matmul_impl(size[i]);
  } 
  return 0;
} 

