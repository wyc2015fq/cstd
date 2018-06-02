
int trans_inplace(int h, int w, int* a) {
  int tmp;
  int i, j;
  for (i=0; i<h; ++i) {
    for (j=0; j<w; ++j) {
      int p0 = i*w+j, p1 = p0, p2;
      while((p1 = (p1%h)*w + p1/h)>p0);
      if (p1==p0) {
        tmp = a[p0];
        for(; (p2 = (p1%h)*w + p1/h)!=p0; p1 = p2) {
          a[p1] = a[p2];
        }
        a[p1] = tmp;
      }
    }
  }
  return 0;
}
int trans(int h, int w, int* a) {
  int* b = MALLOC(int, h*w);
  MEMCPY(b, a, h*w);
  int i, j;
  for (i=0; i<h; ++i) {
    for (j=0; j<w; ++j) {
      a[i+j*h] = b[i*w+j];
    }
  }
  FREE(b);
  return 0;
}

int test_trans_inplace() {
  enum {maxn=100};
  int times, i;
  for (times=0; times<1000; ++times) {
    int h=rand()%maxn+1;
    int w=rand()%maxn+2;
    int n = h*w;
    int* a = MALLOC(int, n);
    int* b = MALLOC(int, n);
    for (i=0; i<n; ++i) {
      b[i] = a[i] = i;
    }
    tic;
    trans_inplace(h, w, a);
    toc;
    tic;
    trans(h, w, b);
    toc;
    for (i=0; i<n; ++i) {
      ASSERT(b[i] == a[i]);
    }
    FREE(a);
    FREE(a);
  }
  tictoc;
  return 0;
}
