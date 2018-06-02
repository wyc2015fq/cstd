
#include "sys/threadpool.inl"

int test_parallel_for_fun(void* arg, int beg, int end) {
  void** args = (void**)arg;
  double* a = (double*)args[0];
  double* b = (double*)args[1];
  double* c = (double*)args[2];
  int i;
  //int id = sys_thread_id(); printf("id = %d\n", id);
  for (i=beg; i<end; ++i) {
    c[i] = pow(a[i], b[i]) + pow(b[i], a[i]) + sin(a[i]) + cos(b[i]);
    if (c[i]<00001) {
      int asdf=0;
    }
    printf("%5d %d\n", sys_thread_id(), i);
    sys_sleep(100);
  }
  return 0;
}

#include "math/test_trans_inplace.inl"

int test_parallel_for() {
 
  tic;
  int aa = test_trans_inplace();
  toc;
  tictoc;
  return 0;
  int n = 10;
  
  double* a = MALLOC(double, n);
  double* b = MALLOC(double, n);
  double* c0 = MALLOC(double, n);
  double* c1 = MALLOC(double, n);
  int i, times=1;
  for (i=0; i<n; ++i) {
    a[i] = 1+rand()*1./RAND_MAX;
    b[i] = 1+rand()*1./RAND_MAX;
    c0[i] = 0;
    c1[i] = 0;
  }
  
  if (1) {
    void* args[] = {a, b, c0};
    tic;
    for (i=0; i<times; ++i) {
      test_parallel_for_fun(args, 0, n);
    }
    toc_;
  }
  
  if (1) {
    void* args[] = {a, b, c1};
    threadpool_t pool[1] = {0};
    threadpool_init(pool, 4);
    tic;
    for (i=0; i<times; ++i) {
      parallel_for(args, 0, n, test_parallel_for_fun);
      //printf("\n");
    }
    toc_;
  }
  {
    double d=0;
    for (i=0; i<n; ++i) {
      double t11 = c0[i]-c1[i];
      d += t11*t11;
      if (t11*t11>0.000001) {
        int adsf=0;
        printf("%f %f %f \n", c0[i], c1[i], t11);
      }
    }
    d = sqrt(d/n);
    printf("%f\n", d);
  }
  FREE(a);
  FREE(b);
  FREE(c0);
  FREE(c1);
  return 0;
}
